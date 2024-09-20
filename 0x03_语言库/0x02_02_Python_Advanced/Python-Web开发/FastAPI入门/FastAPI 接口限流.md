Python FastAPI<br />如果没有接口限流，可能会导致服务器负载不平衡，暴力破解密码，恶意请请求，导致服务器额外费用，拒绝服务攻击等。<br />因此做好接口限流很有必要。
<a name="FV6Uf"></a>
## 接口限流算法
怎么做接口限流呢？常见的接口限流算法有 4 种：
<a name="jKB1Y"></a>
### 1、固定窗口计数器
比如说每小时限制请求 10 次，超过 10 次的直接丢弃。它有个缺点，就是有时会超过 10 次，最多达到 2 倍。比如说固定窗口为整点，8 点 50 到 9 点之间发送了 10 个请求，9 点 到 9 点 10 分又发送了 10 个请求，虽说都被放行，但 8 点 50 到 9 点 50 这一个小时内，发送了 20 个请求。
<a name="A7DQI"></a>
### 2、滑动窗口计数器
这个解决了 1 的问题，但是时间区间的精度划分越高，算法所需的空间容量就越大。
<a name="m74Xo"></a>
### 3、漏桶算法
漏桶算法多使用队列实现，服务的请求会存到队列中，服务的提供方则按照固定的速率从队列中取出请求并执行，过多的请求则放在队列中排队或直接拒绝。漏桶算法的缺陷也很明显，当短时间内有大量的突发请求时，即便此时服务器没有任何负载，每个请求也都得在队列中等待一段时间才能被响应。
<a name="YUgSV"></a>
### 4、令牌桶算法
令牌以固定速率生成。生成的令牌放入令牌桶中存放，如果令牌桶满了则多余的令牌会直接丢弃，当请求到达时，会尝试从令牌桶中取令牌，取到了令牌的请求可以执行。如果桶空了，那么尝试取令牌的请求会被直接丢弃。令牌桶算法既能够将所有的请求平均分布到时间区间内，又能接受服务器能够承受范围内的突发请求。<br />可能有朋友会问，为啥不根据 IP 地址进行限流？其实做是可以做的，只是不那么主流。<br />既然要根据 IP 地址进行限流，那就会产生两个问题，一是 IP 地址的保存就是一个问题，如果接口是集群，还要将 IP 地址保存在一个集中的数据库里，最好是 redis。二是会误伤正常请求，因为一个大的局域网，其出口 IP 是一个，那么限制了这个 IP 的请求，可能导致正常用户被困。<br />以上 4 种方法中，最简单实用的就是滑动窗口计数器。<br />Django REST Framework [自带限流](https://www.django-rest-framework.org/api-guide/throttling/):
```python
REST_FRAMEWORK = {
    'DEFAULT_THROTTLE_CLASSES': [
        'rest_framework.throttling.AnonRateThrottle',
        'rest_framework.throttling.UserRateThrottle'
    ],
    'DEFAULT_THROTTLE_RATES': {
        'anon': '100/day',
        'user': '1000/day'
    }
}
```
<a name="smcq8"></a>
## FastAPI 限流的 3 个方法
这里分享一下 FastAPI 限流的 3 个方法：
<a name="N7agI"></a>
### 1、[slowapi](https://github.com/laurents/slowapi)
slowapi 是有人根据 flask-limiter 改写的，计数器默认保存在内存中，具体用法如下：
```python
from fastapi import FastAPI
from slowapi.errors import RateLimitExceeded
from slowapi import Limiter, _rate_limit_exceeded_handler
from slowapi.util import get_remote_address


limiter = Limiter(key_func=get_remote_address)
app = FastAPI()
app.state.limiter = limiter
app.add_exception_handler(RateLimitExceeded, _rate_limit_exceeded_handler)

@app.get("/home")
@limiter.limit("5/minute")
async def homepage(request: Request):
    return PlainTextResponse("test")

@app.get("/mars")
@limiter.limit("5/minute")
async def homepage(request: Request, response: Response):
    return {"key": "value"}
```
<a name="je03q"></a>
### 2、 [fastapi-limiter](https://github.com/long2ice/fastapi-limiter)
需要一个 redis 来保存计数器：
```python
import aioredis
import uvicorn
from fastapi import Depends, FastAPI

from fastapi_limiter import FastAPILimiter
from fastapi_limiter.depends import RateLimiter

app = FastAPI()


@app.on_event("startup")
async def startup():
    redis = await aioredis.create_redis_pool("redis://localhost")
    FastAPILimiter.init(redis)


@app.get("/", dependencies=[Depends(RateLimiter(times=2, seconds=5))])
async def index():
    return {"msg": "Hello World"}


if __name__ == "__main__":
    uvicorn.run("main:app", debug=True, reload=True)
```
<a name="RJmjJ"></a>
### 3、[asgi-ratelimit](https://github.com/abersheeran/asgi-ratelimit)
比如超过每秒五次访问限制后，阻止特定用户 60 秒:
```python
app.add_middleware(
	RateLimitMiddleware,
	authenticate=AUTH_FUNCTION,
	backend=RedisBackend(),
	config={
		r"^/user": [Rule(second=5, block_time=60)],
	},
)
```
以上推荐 slowapi。
<a name="ksHce"></a>
## 参考资料
自带限流: [https://www.django-rest-framework.org/api-guide/throttling/](https://www.django-rest-framework.org/api-guide/throttling/)<br />slowapi: [https://github.com/laurents/slowapi](https://github.com/laurents/slowapi)<br />fastapi-limiter: [https://github.com/long2ice/fastapi-limiter](https://github.com/long2ice/fastapi-limiter)<br />asgi-ratelimit: [https://github.com/abersheeran/asgi-ratelimit](https://github.com/abersheeran/asgi-ratelimit)
