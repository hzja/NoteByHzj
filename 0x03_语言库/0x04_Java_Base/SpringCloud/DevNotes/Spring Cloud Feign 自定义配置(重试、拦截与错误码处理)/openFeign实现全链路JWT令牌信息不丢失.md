Java SpringCloud
<a name="lyT5a"></a>
## 什么是令牌中继？
令牌中继通俗的讲则是让令牌在微服务链路调用中传递下去，保证各个微服务能够获取令牌中的用户信息。<br />以下订单的例子来说，如下图：<br />![下单流程](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642465302403-1bb22c9f-3dfc-409b-8c04-d6f762bc2e44.webp#clientId=u26b6ea24-c40d-4&from=paste&id=u7f7b6e13&originHeight=244&originWidth=623&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3dd4e35b-c2bf-4d07-99b3-6498330e4a2&title=%E4%B8%8B%E5%8D%95%E6%B5%81%E7%A8%8B "下单流程")<br />客户端携带令牌请求网关，网关鉴权成功后会将令牌中的用户信息解析出来放在请求头中下发给订单服务，同样的，订单服务需要将用户信息传递给账户服务获取该用户的账户信息。<br />那么问题来了？如何保证网关服务->订单服务->账户服务这条链路中的用户信息传递下去是个痛点
<a name="IGpbV"></a>
## 解决方案
令牌在openFeign调用过程中是不能自动中继的，因此必须手动的将令牌信息传递下去。<br />注意：openFeign在开启熔断降级后内部调用开启了子线程，因此传统的方案直接在`RequestInterceptor`中设置是不可行的。<br />那么如何保证子线程也能获取请求头中的用户信息呢？<br />答案是：`RequestContextHolder`这个神器。<br />`RequestContextHolder`内部通过`InheritableThreadLocal`实现子线程共享信息。<br />在`FeignCircuitBreakerInvocationHandler`这个类中也是有如下一行代码：
```java
RequestContextHolder.setRequestAttributes(requestAttributes);
```
正是使用`RequestContextHolder`将request的信息保存在其中，因此实现令牌中继只需要读取`RequestContextHolder`的信息即可。<br />详细代码如下：
```java
@Component
public class FeignRequestInterceptor implements RequestInterceptor {
    @Override
    public void apply(RequestTemplate template) {
        //从RequestContextHolder中获取HttpServletRequest
        HttpServletRequest httpServletRequest = RequestContextUtils.getRequest();
        //获取RequestContextHolder中的信息
        Map<String, String> headers = getHeaders(httpServletRequest);
        //放入feign的RequestTemplate中
        for (Map.Entry<String, String> entry : headers.entrySet()) {
            template.header(entry.getKey(), entry.getValue());
        }
    }

    /**
     * 获取原请求头
     */
    private Map<String, String> getHeaders(HttpServletRequest request) {
        Map<String, String> map = new LinkedHashMap<>();
        Enumeration<String> enumeration = request.getHeaderNames();
        if (enumeration != null) {
            while (enumeration.hasMoreElements()) {
                String key = enumeration.nextElement();
                String value = request.getHeader(key);
                map.put(key, value);
            }
        }
        return map;
    }
}
```
