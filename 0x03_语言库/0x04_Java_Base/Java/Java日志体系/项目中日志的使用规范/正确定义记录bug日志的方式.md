日志<br />日常工作中，程序员需要经常处理线上的各种大小故障，如果业务代码没打印日志或者日志打印的不好，会极大的加大了定位问题的难度，使得解决bug的时间变长了。对于那种影响比较大的bug，处理时间是分秒必争的，慢几秒处理完，可能GMV就哗啦啦的掉了很多。
<a name="zvlHL"></a>
## 请统一日志格式
日志格式最好是统一的，即方便查看定位问题又方便统计收集。一般定义一个LogObject对象，里面定义日志的各个字段。例如：
```java
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonInclude.Include;
import com.fasterxml.jackson.annotation.JsonProperty;

public class LogObject {


    @JsonProperty(index = 1)
    private String eventName;

    @JsonProperty(index = 2)
    private String traceId;

    @JsonProperty(index = 3)
    private String msg;

    @JsonProperty(index = 4)
    private long costTime;

    @JsonProperty(index = 6)
    private Integer userId;

    @JsonProperty(index = 7)
    private Object others;

    @JsonProperty(index = 8)
    private Object request;

    @JsonProperty(index = 9)
    private Object response;


    public String getEventName() {


        return eventName;
    }

    public LogObject setEventName(String eventName) {


        this.eventName = eventName;
        return this;
    }

    public Object getRequest() {


        return request;
    }

    public LogObject setRequest(Object request) {


        this.request = request;
        return this;
    }

    public Object getResponse() {


        return response;
    }

    public LogObject setResponse(Object response) {


        this.response = response;
        return this;
    }

    public String getMsg() {


        return msg;
    }

    public LogObject setMsg(String msg) {


        this.msg = msg;
        return this;
    }



    public long getCostTime() {


        return costTime;
    }

    public LogObject setCostTime(long costTime) {


        this.costTime = costTime;
        return this;
    }


    public Integer getUserId() {


        return userId;
    }

    public LogObject setUserId(Integer userId) {


        this.userId = userId;
        return this;
    }

    public Object getOthers() {


        return others;
    }

    public LogObject setOthers(Object others) {


        this.others = others;
        return this;
    }

    public String getTraceId() {


        return traceId;
    }

    public LogObject setTraceId(String traceId) {


        this.traceId = traceId;
        return this;
    }
}
```

- traceId: 调用链id
- eventName: 事件名称,一般就是业务方法名称
- userId: C端用户id
- msg: 结果消息
- costTime: 接口响应时间
- request: 接口请求入参
- response: 接口返回值
- others: 其他业务参数

使用链式的风格，方便设置字段的值：
```java
long endTime = System.currentTimeMillis();
LogObject logObject = new LogObject();
logObject.setEventName(methodName)
    .setMsg(msg)
    .setTraceId(traceId)
    .setUserId(backendId)
    .setRequest(liveRoomPushOrderReqDto)
    .setResponse(response)
    .setCostTime((endTime - beginTime));

LOGGER.info(JSON.toJSONString(logObject));
```
当然最好还是封装出一个工具类出来,例如叫：LogTemplate，作为一个统一的入口。另外可以使用JsonProperty注解，指定字段的顺序，例如通过index=1，将eventName放置在最前面。
```java
@JsonProperty(index = 1)
private String eventName;
```
<a name="jZcQq"></a>
## 将request和response放置在一起
将请求和返回值，放置在同一条日志里，有个好处，就是非常方便查看上下文日志。如果打印成两条，返回值那条可能被冲到很后面，而且也得再做一次grep操作，影响效率。
```java
{
  "eventName": "createOrder",
  "traceId": "createOrder_1574923602015",
  "msg": "success",
  "costTime": 317,
  "request": {
    "uId": 111111111,
    "skuList": [
      {
        "skuId": 22222222,
        "buyNum": 1,
        "buyPrice": 8800
      }
    ]
  },
  "response": {
    "code": 0,
    "message": "操作成功",
    "data": {
      "bigOrderId": "BIG2019",
      "m2LOrderIds": {
        "MID2019": {
          "22222222": "LIT2019"
        }
      }
    }
  }
}
```
为了能拼成一条，有两种方案，一种是比较low的，直接在代码里使用try catch finally，例如：
```java
@PostMapping(value = "/createOrder")
public JsonResult createOrder(@RequestBody Object request) throws Exception {


    String methodName = "/createOrder";
    Integer backendId = null;
    String msg = "success";
    long beginTime = System.currentTimeMillis();
    String traceId = "createOrder_"+beginTime;
    JsonResult response = null;
    try {


        OrderCreateRsp orderCreateRsp = orderOperateService.createOrder(request, traceId);
        response = JsonResult.success(orderCreateRsp);
    }
    catch (Exception e) {


        msg = e.getMessage();
        LOGGER.error(methodName+",userId:"+backendId+",request:"+ JsonHelper.toJson(request),e);
        throw new BizException(0,"下单失败");
    }
    finally {


        long endTime = System.currentTimeMillis();
        LogObject logObject = new LogObject();
        logObject.setEventName(methodName)
            .setMsg(msg)
            .setTraceId(traceId)
            .setUserId(backendId)
            .setRequest(request)
            .setResponse(response)
            .setCostTime((endTime - beginTime));

        LOGGER.info(JSON.toJSONString(logObject));
    }

    return response;
}
```
这种方案呢，有个缺点，就是每个业务方法都得处理日志，更好的方案是使用aop加thread local的方式，将请求统一拦截且将返回值和请求参数串起来，这个网络上的方案很多，这里就不阐述了。<br />对于对性能要求比较高的应用，反而推荐第一种方案，因为使用aop，有一些性能损耗。
<a name="D3eHO"></a>
## 日志里加入traceId
如果应用中已经使用了统一调用链监控方案，且能根据调用链id查询接口情况的，可以不用在代码里手动加入traceId。如果应用还没接入调用链系统，建议加一下traceId，尤其是针对聚合服务，需要调用中台各种微服务接口的。像聚合层下单业务，需要调用的微服务就有如下这么些：

- 营销系统
- 订单系统
- 支付系统

下单业务调用这些接口的时候，如果没有使用traceId进行跟踪的话，当下单失败的时候，到底是哪个微服务接口失败了，就比较难找。下面以小程序端，调用聚合层下单接口的例子作为展示：
```json
//营销系统
{
  "eventName": "pms/getInfo",
  "traceId": "createOrder_1575270928956",
  "msg": "success",
  "costTime": 2,
  "userId": 1111111111,
  "request": {
    "userId": 1111111111,
    "skuList": [
      {
        "skuId": 2222,
        "skuPrice": 65900,
        "buyNum": 1,
        "activityType": 0,
        "activityId": 0
      }
    ]
  },
  "response": {
    "result": 1,
    "msg": "success",
    "data": {
      "realPayFee": 100
    }
  }
}

//订单系统
{
  "eventName": "orderservice/createOrder",
  "traceId": "createOrder_1575270928956",
  "msg": "success",
  "costTime": 29,
  "userId": null,
  "request": {
    "skuList": [
      {
        "skuId": 2222,
        "buyNum": 1,
        "buyPrice": 65900
      }
    ]
  },
  "response": {
    "result": "200",
    "msg": "调用成功",
    "data": {
      "bigOrderId": "BIG2019",
      "m2LOrderIds": {
        "MID2019": {
          "88258135": "LIT2019"
        }
      }
    }
  }
}

//支付系统

{
  "eventName": "payservice/pay",
  "traceId": "createOrder_1575270928956",
  "msg": "success",
  "costTime": 301,
  "request": {
    "orderId": "BIG2019",
    "paySubject": "测试",
    "totalFee": 65900
  },
  "response": {
    "requestId": "test",
    "code": 0,
    "message": "操作成功",
    "data": {
      "payId": 123,
      "orderId": "BIG2019",
      "tradeType": "JSAPI",
      "perpayId": "test",
      "nonceStr": "test",
      "appId": "test",
      "signType": "MD5",
      "sign": "test",
      "timeStamp": "1575270929"
    }
  }
}
```
可以看到聚合层需要调用营销、订单和支付三个应用的接口，调用的过程中，使用traceId为createOrder_1575270928956的串了起来，这样只需要`grep`这个`traceId`就可以把所有相关的调用和上下文找出来。<br />traceId如何生成呢，一种简单的做法是，使用`System.currentTimeMillis()` 加上业务接口名字，如：
```java
 long beginTime = System.currentTimeMillis();
 String traceId = "createOrder_"+beginTime;
```
加traceId会侵入到业务方法里，比如说：
```java
public void createOrder(Object obj) {
    long beginTime = System.currentTimeMillis();
    String traceId = "createOrder_"+beginTime;
    pmsService.getInfo(obj,traceId);
    orderService.createOrder(obj,traceId);
    payService.getPrepayId(obj,traceId);
}
```
像`pmsService`这些内部的`service`方法，都需要加一个`traceId`字段，目前觉得还好，要是觉得入侵了，也可以考虑thread local的方式，处理请求的时候，为当前线程存储一下traceId，然后在业务方法里，再从当前线程里拿出来，避免接口方法里的traceId满天飞。
