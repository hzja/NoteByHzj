Java SpringBootWebSocket<br />消防的设备巡检，如果巡检发现异常，通过手机端提交，后台的实时监控页面实时获取到该设备的信息及位置，然后安排员工去处理。<br />因为需要服务端主动向客户端发送消息，所以很容易的就想到了用WebSocket来实现这一功能。<br />WebSocket介绍：[https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket](https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket)<br />前端略微复杂，需要在一张位置分布图上进行鼠标描点定位各个设备和根据不同屏幕大小渲染，只是简单地用页面样式进行效果呈现。<br />**绿色代表正常，红色代表异常**<br />预期效果，未接收到请求前----->id为3的提交了异常，id为3的王五变成了红色<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1700483368741-5131e23e-9972-4e67-b18b-8466b617619c.png#averageHue=%23f8f7f7&clientId=u4b22cdbe-af0e-4&from=paste&id=ud3f4fdaf&originHeight=272&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9c53ed94-1682-434a-8ec3-02d17f73dfe&title=)
<a name="awOeJ"></a>
## **实现**
<a name="YvqMU"></a>
### 前端：
直接贴代码
```html
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>实时监控</title>
    </head>
    <style>
        .item {
            display: flex;
            border-bottom: 1px solid #000000;
            justify-content: space-between;
            width: 30%;
            line-height: 50px;
            height: 50px;
        }

        .item span:nth-child(2){
            margin-right: 10px;
            margin-top: 15px;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #55ff00;
        }
        .nowI{
            background: #ff0000 !important;
        }
    </style>
    <body>
        <div id="app">
            <div v-for="item in list" class="item">
                <span>{{item.id}}.{{item.name}}</span>
                <span :class='item.state==-1?"nowI":""'></span>
            </div>
        </div>
    </body>
    <script src="./js/vue.min.js"></script>
    <script type="text/javascript">
        var vm = new Vue({
            el: "#app",
            data: {
                list: [{
                        id: 1,
                        name: '张三',
                        state: 1
                    },
                    {
                        id: 2,
                        name: '李四',
                        state: 1
                    },
                    {
                        id: 3,
                        name: '王五',
                        state: 1
                    },
                    {
                        id: 4,
                        name: '韩梅梅',
                        state: 1
                    },
                    {
                        id: 5,
                        name: '李磊',
                        state: 1
                    },
                ]
            }
        })

        var webSocket = null;
        if ('WebSocket' in window) {
            //创建WebSocket对象
            webSocket = new WebSocket("ws://localhost:18801/webSocket/" + getUUID());

            //连接成功
            webSocket.onopen = function() {
                console.log("已连接");
                webSocket.send("消息发送测试")
            }
            //接收到消息
            webSocket.onmessage = function(msg) {
                //处理消息
                var serverMsg = msg.data;
                var t_id = parseInt(serverMsg)    //服务端发过来的消息，ID，string需转化为int类型才能比较
                for (var i = 0; i < vm.list.length; i++) {
                    var item = vm.list[i];
                    if(item.id == t_id){
                        item.state = -1;
                        vm.list.splice(i,1,item)
                        break;
                    }
                }
            };

            //关闭事件
            webSocket.onclose = function() {
                console.log("websocket已关闭");
            };
            //发生了错误事件
            webSocket.onerror = function() {
                console.log("websocket发生了错误");
            }
        } else {
            alert("很遗憾，您的浏览器不支持WebSocket！")
        }

        function getUUID() {    //获取唯一的UUID
            return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {

                var r = Math.random() * 16 | 0,
                    v = c == 'x' ? r : (r & 0x3 | 0x8);
                return v.toString(16);
            });
        }
    </script>
</html>
```
<a name="nDUoQ"></a>
### 后端：
项目结构是这样子的，后面的代码关键注释都有，就不重复描述了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090333-31d3cd6d-8203-4ae3-8662-8aea02dac624.png#averageHue=%233a4044&clientId=uba1a580b-0905-4&from=paste&id=u1cd0d567&originHeight=265&originWidth=301&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fffd8d5-8474-47ee-a451-940b9baf7f0&title=)
<a name="ObQka"></a>
#### 1、新建SpringBoot工程，选择web和WebSocket依赖
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090235-4c048af9-8692-4021-a69b-605da2378f0b.png#averageHue=%233d4248&clientId=uba1a580b-0905-4&from=paste&id=u943b130d&originHeight=263&originWidth=665&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub3e19e97-c38b-45a1-8eec-c93b5a45a45&title=)
<a name="Ar92J"></a>
#### 2、配置application.yml
```yaml
#端口
server:
	port: 18801

#密码，因为接口不需要权限，所以加了个密码做校验
mySocket:
  myPwd: 123
```
<a name="Dpw1u"></a>
#### 3、WebSocketConfig配置类
```java
@Configuration
public class WebSocketConfig {

    /**
     * 注入一个ServerEndpointExporter,该Bean会自动注册使用@ServerEndpoint注解申明的websocket endpoint
     */
    @Bean
    public ServerEndpointExporter serverEndpointExporter(){
        return new ServerEndpointExporter();
    }
}
```
<a name="MJ6ei"></a>
#### 4、WebSocketServer类，用来进行服务端和客户端之间的交互
```java
/**
 * @ServerEndpoint("/webSocket/{uid}") 前端通过此URI与后端建立链接
 */

@ServerEndpoint("/webSocket/{uid}")
@Component
public class WebSocketServer {

    private static Logger log = LoggerFactory.getLogger(WebSocketServer.class);

    //静态变量，用来记录当前在线连接数。应该把它设计成线程安全的。
    private static final AtomicInteger onlineNum = new AtomicInteger(0);

    //concurrent包的线程安全Set，用来存放每个客户端对应的WebSocketServer对象。
    private static CopyOnWriteArraySet<Session> sessionPools = new CopyOnWriteArraySet<Session>();

    /**
     * 有客户端连接成功
     */
    @OnOpen
    public void onOpen(Session session, @PathParam(value = "uid") String uid){
        sessionPools.add(session);
        onlineNum.incrementAndGet();
        log.info(uid + "加入webSocket！当前人数为" + onlineNum);
    }

    /**
     * 连接关闭调用的方法
     */
    @OnClose
    public void onClose(Session session) {
        sessionPools.remove(session);
        int cnt = onlineNum.decrementAndGet();
        log.info("有连接关闭，当前连接数为：{}", cnt);
    }

    /**
     * 发送消息
     */
    public void sendMessage(Session session, String message) throws IOException {
        if(session != null){
            synchronized (session) {
                session.getBasicRemote().sendText(message);
            }
        }
    }

    /**
     * 群发消息
     */
    public void broadCastInfo(String message) throws IOException {
        for (Session session : sessionPools) {
            if(session.isOpen()){
                sendMessage(session, message);
            }
        }
    }

    /**
     * 发生错误
     */
    @OnError
    public void onError(Session session, Throwable throwable){
        log.error("发生错误");
        throwable.printStackTrace();
    }

}
```
<a name="aPKUO"></a>
#### 5、WebSocketController类，用于进行接口测试
```java
@RestController
@RequestMapping("/open/socket")
public class WebSocketController {

    @Value("${mySocket.myPwd}")
    public String myPwd;

    @Autowired
    private WebSocketServer webSocketServer;

    /**
     * 手机客户端请求接口
     * @param id    发生异常的设备ID
     * @param pwd   密码（实际开发记得加密）
     * @throws IOException
     */
    @PostMapping(value = "/onReceive")
    public void onReceive(String id,String pwd) throws IOException {
        if(pwd.equals(myPwd)){  //密码校验一致（这里举例，实际开发还要有个密码加密的校验的），则进行群发
            webSocketServer.broadCastInfo(id);
        }
    }

}
```
<a name="cGY3H"></a>
## **测试**
<a name="BciWF"></a>
### 1、打开前端页面，进行WebSocket连接
控制台输出，连接成功<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090235-be68d2e9-d0a1-4f1f-85a8-cdf34ab34441.png#averageHue=%23333230&clientId=uba1a580b-0905-4&from=paste&id=ucb9d7a74&originHeight=30&originWidth=564&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9d4fc309-7849-4a0e-ab49-36c4010a40d&title=)
<a name="yaSLe"></a>
### 2、因为是模拟数据，所以全部显示正常，没有异常提交时的页面呈现
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090260-c65f40e8-69d8-45cd-83eb-ae229c937f3a.png#averageHue=%23f7f7f6&clientId=uba1a580b-0905-4&from=paste&id=u6d4ce313&originHeight=258&originWidth=215&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9c471838-26b6-4e22-86f7-8fd1a9ae5cd&title=)
<a name="txwqp"></a>
### 3、接下来，用接口测试工具Postman提交一个异常
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090629-b73f9193-1187-4496-9806-0e6e2347936c.png#averageHue=%23fbf8f7&clientId=uba1a580b-0905-4&from=paste&id=u727129c2&originHeight=277&originWidth=812&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0c6afc2a-2fdf-405a-b7ab-5f6286d8754&title=)<br />注意id为3的这个数据的状态变化<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1649243090610-4eb083fa-ff00-47b6-85f1-9d196bc2c2e7.png#averageHue=%23f7f5f4&clientId=uba1a580b-0905-4&from=paste&id=u72d06832&originHeight=254&originWidth=208&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u74adac34-9b1b-4180-a30e-38f55ae5084&title=)<br />可以看到，id为3的王五状态已经变成异常的了，实时通讯成功。<br />参考：[https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket](https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket)
