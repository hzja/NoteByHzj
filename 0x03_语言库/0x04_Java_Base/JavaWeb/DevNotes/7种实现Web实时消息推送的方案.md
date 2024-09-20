Web<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369326677-04e0b305-a650-41a3-8b00-8fde0b4cb3e4.png#averageHue=%23fdfbfa&clientId=u53d28cb3-ecaa-4&from=paste&id=ud246c348&originHeight=617&originWidth=901&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u43305634-a852-435a-85e9-d3790642aef&title=)
<a name="e5m9U"></a>
## 什么是消息推送（push）
推送的场景比较多，比如淘宝发货的物流进度推送。<br />消息推送(push)通常是指网站的运营工作等人员，通过某种工具对用户当前网页或移动设备APP进行的主动消息推送。<br />消息推送一般又分为web端消息推送和移动端消息推送。<br />web端消息推送常见的诸如站内信、未读邮件数量、监控报警数量等，应用的也非常广泛。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369326642-aa3c842b-3758-40d0-a46d-337fbd76de7c.png#averageHue=%23f7f7f6&clientId=u53d28cb3-ecaa-4&from=paste&id=u07283bca&originHeight=283&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub32a5e54-2d6d-4384-8da1-bf6191e2fcd&title=)<br />在具体实现之前，再来分析一下前边的需求，其实功能很简单，只要触发某个事件（主动分享了资源或者后台主动推送消息），web页面的通知小红点就会实时的+1就可以了。<br />通常在服务端会有若干张消息推送表，用来记录用户触发不同事件所推送不同类型的消息，前端主动查询（拉）或者被动接收（推）用户所有未读的消息数。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369326732-e47e9468-b547-4937-b954-0d2c9e5babb2.png#averageHue=%23f3f3f3&clientId=u53d28cb3-ecaa-4&from=paste&id=u094f3b8d&originHeight=458&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u51cd212f-0118-4527-a864-9ada8f57c86&title=)<br />消息推送无非是推（push）和拉（pull）两种形式，下边逐个了解下。
<a name="fBGhJ"></a>
## 短轮询
轮询(polling)应该是实现消息推送方案中最简单的一种，这里暂且将轮询分为短轮询和长轮询。<br />短轮询很好理解，指定的时间间隔，由浏览器向服务器发出HTTP请求，服务器实时返回未读消息数据给客户端，浏览器再做渲染显示。<br />一个简单的JS定时器就可以搞定，每秒钟请求一次未读消息数接口，返回的数据展示即可。
```javascript
setInterval(() => {
	// 方法请求
	messageCount().then((res) => {
		if (res.code === 200) {
			this.messageCount = res.data
		}
	})
}, 1000);
```
效果还是可以的，短轮询实现固然简单，缺点也是显而易见，由于推送数据并不会频繁变更，无论后端此时是否有新的消息产生，客户端都会进行请求，势必会对服务端造成很大压力，浪费带宽和服务器资源。
<a name="e2blP"></a>
## 长轮询
长轮询是对上边短轮询的一种改进版本，在尽可能减少对服务器资源浪费的同时，保证消息的相对实时性。长轮询在中间件中应用的很广泛，比如Nacos和apollo配置中心，消息队列kafka、RocketMQ中都有用到长轮询。<br />这次使用apollo配置中心实现长轮询的方式，应用了一个类`DeferredResult`，它是在servelet3.0后经过Spring封装提供的一种异步请求机制，直意就是延迟结果。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369327429-02704043-b28e-4e94-98eb-a06d8d0590cd.png#averageHue=%23f6c86d&clientId=u53d28cb3-ecaa-4&from=paste&id=ue07eadd0&originHeight=559&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04c73de6-7324-4148-b4c8-7808aeb4397&title=)<br />`DeferredResult`可以允许容器线程快速释放占用的资源，不阻塞请求线程，以此接受更多的请求提升系统的吞吐量，然后启动异步工作线程处理真正的业务逻辑，处理完成调用`DeferredResult.setResult(200)`提交响应结果。<br />下边用长轮询来实现消息推送。<br />因为一个ID可能会被多个长轮询请求监听，所以采用了guava包提供的`Multimap`结构存放长轮询，一个key可以对应多个value。一旦监听到key发生变化，对应的所有长轮询都会响应。前端得到非请求超时的状态码，知晓数据变更，主动查询未读消息数接口，更新页面数据。
```java
@Controller
@RequestMapping("/polling")
public class PollingController {

	// 存放监听某个Id的长轮询集合
	// 线程同步结构
	public static Multimap<String, DeferredResult<String>> watchRequests = Multimaps.synchronizedMultimap(HashMultimap.create());

	/**
     * 设置监听
     */
	@GetMapping(path = "watch/{id}")
	@ResponseBody
	public DeferredResult<String> watch(@PathVariable String id) {
		// 延迟对象设置超时时间
		DeferredResult<String> deferredResult = new DeferredResult<>(TIME_OUT);
		// 异步请求完成时移除 key，防止内存溢出
		deferredResult.onCompletion(() -> {
			watchRequests.remove(id, deferredResult);
		});
		// 注册长轮询请求
		watchRequests.put(id, deferredResult);
		return deferredResult;
	}

	/**
     * 变更数据
     */
	@GetMapping(path = "publish/{id}")
	@ResponseBody
	public String publish(@PathVariable String id) {
		// 数据变更 取出监听ID的所有长轮询请求，并一一响应处理
		if (watchRequests.containsKey(id)) {
			Collection<DeferredResult<String>> deferredResults = watchRequests.get(id);
			for (DeferredResult<String> deferredResult : deferredResults) {
				deferredResult.setResult("我更新了" + new Date());
			}
		}
		return "success";
	}
}
```
当请求超过设置的超时时间，会抛出`AsyncRequestTimeoutException`异常，这里直接用`@ControllerAdvice`全局捕获统一返回即可，前端获取约定好的状态码后再次发起长轮询请求，如此往复调用。
```java
@ControllerAdvice
public class AsyncRequestTimeoutHandler {

    @ResponseStatus(HttpStatus.NOT_MODIFIED)
    @ResponseBody
    @ExceptionHandler(AsyncRequestTimeoutException.class)
    public String asyncRequestTimeoutHandler(AsyncRequestTimeoutException e) {
        System.out.println("异步请求超时");
        return "304";
    }
}
```
来测试一下，首先页面发起长轮询请求/polling/watch/10086监听消息更变，请求被挂起，不变更数据直至超时，再次发起了长轮询请求；紧接着手动变更数据/polling/publish/10086，长轮询得到响应，前端处理业务逻辑完成后再次发起请求，如此循环往复。<br />![2022-07-21-10-10-01.788542700.gif](https://cdn.nlark.com/yuque/0/2022/gif/396745/1658370053889-d72c19be-4ea6-4806-b1b0-59b6106471ee.gif#averageHue=%23d3e2e2&clientId=u53d28cb3-ecaa-4&from=ui&id=u6cd2ad57&originHeight=631&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=243893&status=done&style=shadow&taskId=u06c53bb0-7c49-4c58-b727-c94a9e75d35&title=)<br />长轮询相比于短轮询在性能上提升了很多，但依然会产生较多的请求，这是它的一点不完美的地方。
<a name="zu35a"></a>
## `iframe`流
iframe流就是在页面中插入一个隐藏的`<iframe>`标签，通过在src中请求消息数量API接口，由此在服务端和客户端之间创建一条长连接，服务端持续向iframe传输数据。<br />传输的数据通常是HTML、或是内嵌的javascript脚本，来达到实时更新页面的效果。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369327550-f45d1395-badc-4c5d-ba3e-6cb3a7606cf9.png#averageHue=%23fefbf4&clientId=u53d28cb3-ecaa-4&from=paste&id=u2ae443a1&originHeight=492&originWidth=926&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uafe208de-ab9c-4fae-957c-15e727ff178&title=)<br />这种方式实现简单，前端只要一个`<iframe>`标签搞定了
```html
<iframe src="/iframe/message" style="display:none"></iframe>
```
服务端直接组装html、js脚本数据向response写入就行了
```java
@Controller
@RequestMapping("/iframe")
public class IframeController {
	@GetMapping(path = "message")
	public void message(HttpServletResponse response) throws IOException, InterruptedException {
		while (true) {
			response.setHeader("Pragma", "no-cache");
			response.setDateHeader("Expires", 0);
			response.setHeader("Cache-Control", "no-cache,no-store");
			response.setStatus(HttpServletResponse.SC_OK);
			response.getWriter().print(" <script type=\"text/javascript\">\n" +
									   "parent.document.getElementById('clock').innerHTML = \"" + count.get() + "\";" +
									   "parent.document.getElementById('count').innerHTML = \"" + count.get() + "\";" +
									   "</script>");
		}
	}
}
```
但不推荐，因为它在浏览器上会显示请求未加载完，图标会不停旋转，简直是强迫症杀手。<br />![2022-07-21-10-10-02.248305200.gif](https://cdn.nlark.com/yuque/0/2022/gif/396745/1658370019522-392bf72e-3875-4b87-9676-100a6841e18a.gif#averageHue=%23658393&clientId=u53d28cb3-ecaa-4&from=ui&id=uc464b22f&originHeight=234&originWidth=362&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31314&status=done&style=shadow&taskId=u927ef535-d852-4c8c-9442-273a7dd20d3&title=)
<a name="Alg8c"></a>
## SSE (推荐的方式)
很多人可能不知道，服务端向客户端推送消息，其实除了可以用WebSocket这种耳熟能详的机制外，还有一种服务器发送事件(Server-sent events)，简称SSE。<br />SSE它是基于HTTP协议的，一般意义上的HTTP协议是无法做到服务端主动向客户端推送消息的，但SSE是个例外，它变换了一种思路。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369328111-38b06595-b146-4094-9a8a-cf84a8093045.png#averageHue=%230a110a&clientId=u53d28cb3-ecaa-4&from=paste&id=ua65e1a23&originHeight=300&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8e44cb2d-a664-4412-821d-0f945995678&title=)<br />SSE在服务器和客户端之间打开一个单向通道，服务端响应的不再是一次性的数据包而是text/event-stream类型的数据流信息，在有数据变更时从服务器流式传输到客户端。<br />整体的实现思路有点类似于在线视频播放，视频流会连续不断的推送到浏览器，也可以理解成，客户端在完成一次用时很长（网络不畅）的下载。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369328641-6a7dd0f9-76d5-45d0-8771-92969f055c11.png#averageHue=%23f4f4f4&clientId=u53d28cb3-ecaa-4&from=paste&id=ue2a2fb55&originHeight=560&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60b61284-07c9-4e58-81bc-2c9f2d15377&title=)<br />SSE与WebSocket作用相似，都可以建立服务端与浏览器之间的通信，实现服务端向客户端推送消息，但还是有些许不同：

- **SSE 是基于HTTP协议的，它们不需要特殊的协议或服务器实现即可工作；WebSocket需单独服务器来处理协议。**
- **SSE 单向通信，只能由服务端向客户端单向通信；webSocket全双工通信，即通信的双方可以同时发送和接受信息。**
- **SSE 实现简单开发成本低，无需引入其他组件；WebSocket传输数据需做二次解析，开发门槛高一些。**
- **SSE 默认支持断线重连；WebSocket则需要自己实现。**
- **SSE 只能传送文本消息，二进制数据需要经过编码后传送；WebSocket默认支持传送二进制数据。**
<a name="WwR0i"></a>
### SSE 与 WebSocket 该如何选择？
技术并没有好坏之分，只有哪个更合适<br />SSE好像一直不被大家所熟知，一部分原因是出现了WebSockets，这个提供了更丰富的协议来执行双向、全双工通信。对于游戏、即时通信以及需要双向近乎实时更新的场景，拥有双向通道更具吸引力。<br />但是，在某些情况下，不需要从客户端发送数据。只需要一些服务器操作的更新。比如：站内信、未读消息数、状态更新、股票行情、监控数量等场景，SEE不管是从实现的难易和成本上都更加有优势。此外，SSE 具有WebSockets在设计上缺乏的多种功能，例如：自动重新连接、事件ID和发送任意事件的能力。<br />前端只需进行一次HTTP请求，带上唯一ID，打开事件流，监听服务端推送的事件就可以了
```html
<script>
	let source = null;
	let userId = 7777
	if (window.EventSource) {
		// 建立连接
		source = new EventSource('http://localhost:7777/sse/sub/'+userId);
		setMessageInnerHTML("连接用户=" + userId);
		/**
		* 连接一旦建立，就会触发open事件
		* 另一种写法：source.onopen = function (event) {}
		*/
		source.addEventListener('open', function (e) {
			setMessageInnerHTML("建立连接。。。");
		}, false);
		/**
		* 客户端收到服务器发来的数据
		* 另一种写法：source.onmessage = function (event) {}
		*/
		source.addEventListener('message', function (e) {
			setMessageInnerHTML(e.data);
		});
	} else {
		setMessageInnerHTML("你的浏览器不支持SSE");
	}
</script>
```
服务端的实现更简单，创建一个`SseEmitter`对象放入`sseEmitterMap`进行管理
```java
private static Map<String, SseEmitter> sseEmitterMap = new ConcurrentHashMap<>();

/**
* 创建连接
*/
public static SseEmitter connect(String userId) {
	try {
		// 设置超时时间，0表示不过期。默认30秒
		SseEmitter sseEmitter = new SseEmitter(0L);
		// 注册回调
		sseEmitter.onCompletion(completionCallBack(userId));
		sseEmitter.onError(errorCallBack(userId));
		sseEmitter.onTimeout(timeoutCallBack(userId));
		sseEmitterMap.put(userId, sseEmitter);
		count.getAndIncrement();
		return sseEmitter;
	} catch (Exception e) {
		log.info("创建新的sse连接异常，当前用户：{}", userId);
	}
	return null;
}

/**
* 给指定用户发送消息
*/
public static void sendMessage(String userId, String message) {
	
	if (sseEmitterMap.containsKey(userId)) {
		try {
			sseEmitterMap.get(userId).send(message);
		} catch (IOException e) {
			log.error("用户[{}]推送异常:{}", userId, e.getMessage());
			removeUser(userId);
		}
	}
}
```
模拟服务端推送消息，看下客户端收到了消息，和预期的效果一致。![image.png](https://cdn.nlark.com/yuque/0/2022/gif/396745/1658369328764-926bda82-b3dd-4650-b926-8e09fe8a4441.gif#averageHue=%23000000&clientId=u53d28cb3-ecaa-4&from=paste&id=u63cdbdfa&originHeight=1&originWidth=1&originalType=url&ratio=1&rotation=0&showTitle=false&size=70&status=done&style=shadow&taskId=u7dc74f0d-1774-4630-9ad7-016e46d5ff7&title=)<br />**注意：** SSE不支持IE浏览器，对其他主流浏览器兼容性做的还不错。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369328889-9f351f10-b914-4d15-ac56-538cdeb71874.png#averageHue=%23ebdcc7&clientId=u53d28cb3-ecaa-4&from=paste&id=u72e6bd25&originHeight=339&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf67b71ac-4a29-4c94-8194-4a5bd442d7d&title=)
<a name="KycU0"></a>
## MQTT
<a name="zcYGN"></a>
### 什么是 MQTT协议？
MQTT 全称(Message Queue Telemetry Transport)：一种基于发布/订阅（publish/subscribe）模式的轻量级通讯协议，通过订阅相应的主题来获取消息，是物联网（Internet of Thing）中的一个标准传输协议。<br />该协议将消息的发布者（publisher）与订阅者（subscriber）进行分离，因此可以在不可靠的网络环境中，为远程连接的设备提供可靠的消息服务，使用方式与传统的MQ有点类似。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369328920-f9273521-8b70-428f-9b52-e72fe0e2437b.png#averageHue=%23f3eddb&clientId=u53d28cb3-ecaa-4&from=paste&id=uf305c342&originHeight=570&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubd00bcc0-1dcf-4804-a92d-a7098d4bd9c&title=)<br />TCP协议位于传输层，MQTT 协议位于应用层，MQTT 协议构建于TCP/IP协议上，也就是说只要支持TCP/IP协议栈的地方，都可以使用MQTT协议。<br />为什么要用 MQTT协议？<br />MQTT协议为什么在物联网（IOT）中如此受偏爱？而不是其它协议，比如更为熟悉的 HTTP协议呢？

- **首先HTTP协议它是一种同步协议，客户端请求后需要等待服务器的响应。而在物联网（IOT）环境中，设备会很受制于环境的影响，比如带宽低、网络延迟高、网络通信不稳定等，显然异步消息协议更为适合IOT应用程序。**
- **HTTP是单向的，如果要获取消息客户端必须发起连接，而在物联网（IOT）应用程序中，设备或传感器往往都是客户端，这意味着它们无法被动地接收来自网络的命令。**
- **通常需要将一条命令或者消息，发送到网络上的所有设备上。HTTP要实现这样的功能不但很困难，而且成本极高。**
<a name="AnCBv"></a>
## Websocket
websocket应该是大家都比较熟悉的一种实现消息推送的方式，上边在讲SSE的时候也和websocket进行过比较。<br />WebSocket是一种在TCP连接上进行全双工通信的协议，建立客户端和服务器之间的通信渠道。浏览器和服务器仅需一次握手，两者之间就直接可以创建持久性的连接，并进行双向数据传输。<br />![2022-07-21-10-10-03.610359900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369454404-092dbb96-3a96-44ec-89ea-010d47de4446.png#averageHue=%23fbfbfb&clientId=u53d28cb3-ecaa-4&from=ui&id=u3d5e5b7c&originHeight=583&originWidth=808&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1416138&status=done&style=shadow&taskId=u6f06c940-2f96-4d08-a4d0-52cf2711f66&title=)<br />SpringBoot整合websocket，先引入websocket相关的工具包，和SSE相比额外的开发成本。
```xml
<!-- 引入websocket -->
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-websocket</artifactId>
</dependency>
```
服务端使用`@ServerEndpoint`注解标注当前类为一个websocket服务器，客户端可以通过ws://localhost:7777/webSocket/10086来连接到WebSocket服务器端。
```java
@Component
@Slf4j
@ServerEndpoint("/websocket/{userId}")
public class WebSocketServer {
    //与某个客户端的连接会话，需要通过它来给客户端发送数据
    private Session session;
    private static final CopyOnWriteArraySet<WebSocketServer> webSockets = new CopyOnWriteArraySet<>();
    // 用来存在线连接数
    private static final Map<String, Session> sessionPool = new HashMap<String, Session>();
    /**
     * 链接成功调用的方法
     */
    @OnOpen
    public void onOpen(Session session, @PathParam(value = "userId") String userId) {
        try {
            this.session = session;
            webSockets.add(this);
            sessionPool.put(userId, session);
            log.info("websocket消息: 有新的连接，总数为:" + webSockets.size());
        } catch (Exception e) {
        }
    }
    /**
     * 收到客户端消息后调用的方法
     */
    @OnMessage
    public void onMessage(String message) {
        log.info("websocket消息: 收到客户端消息:" + message);
    }
    /**
     * 此为单点消息
     */
    public void sendOneMessage(String userId, String message) {
        Session session = sessionPool.get(userId);
        if (session != null && session.isOpen()) {
            try {
                log.info("websocket消: 单点消息:" + message);
                session.getAsyncRemote().sendText(message);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
```
前端初始化打开WebSocket连接，并监听连接状态，接收服务端数据或向服务端发送数据。
```html
<script>
	var ws = new WebSocket('ws://localhost:7777/webSocket/10086');
	// 获取连接状态
	console.log('ws连接状态：' + ws.readyState);
	//监听是否连接成功
	ws.onopen = function () {
		console.log('ws连接状态：' + ws.readyState);
		//连接成功则发送一个数据
		ws.send('test1');
	}
	// 接听服务器发回的信息并处理展示
	ws.onmessage = function (data) {
		console.log('接收到来自服务器的消息：');
		console.log(data);
		//完成通信后关闭WebSocket连接
		ws.close();
	}
	// 监听连接关闭事件
	ws.onclose = function () {
		// 监听整个过程中websocket的状态
		console.log('ws连接状态：' + ws.readyState);
	}
	// 监听并处理error事件
	ws.onerror = function (error) {
		console.log(error);
	}
	function sendMessage() {
		var content = $("#message").val();
		$.ajax({
			url: '/socket/publish?userId=10086&message=' + content,
			type: 'GET',
			data: { "id": "7777", "content": content },
			success: function (data) {
				console.log(data)
			}
		})
	}
</script>
```
页面初始化建立websocket连接，之后就可以进行双向通信了，效果还不错<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369329292-6517fb2c-af6e-4484-9e3a-17064790f3ff.png#averageHue=%23f6f4f2&clientId=u53d28cb3-ecaa-4&from=paste&id=u60a2873c&originHeight=102&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04d5a11d-2484-4749-9464-6186d005dc6&title=)
<a name="HYhKF"></a>
## 自定义推送
上边给出了6种方案的原理和代码实现，但在实际业务开发过程中，不能盲目的直接拿过来用，还是要结合自身系统业务的特点和实际场景来选择合适的方案。<br />推送最直接的方式就是使用第三推送平台，毕竟**钱能解决的需求都不是问题**，无需复杂的开发运维，直接可以使用，省时、省力、省心，像goEasy、极光推送都是很不错的三方服务商。<br />一般大型公司都有自研的消息推送平台，像本次实现的web站内信只是平台上的一个触点而已，短信、邮件、微信公众号、小程序凡是可以触达到用户的渠道都可以接入进来。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658369330309-939b9dd6-6d6a-424f-82be-594b1f3af619.png#clientId=u53d28cb3-ecaa-4&from=paste&id=u032233fb&originHeight=380&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u34334956-ad27-4616-836a-c96a0990d8c&title=)<br />消息推送系统内部是相当复杂的，诸如消息内容的维护审核、圈定推送人群、触达过滤拦截（推送的规则频次、时段、数量、黑白名单、关键词等等）、推送失败补偿非常多的模块，技术上涉及到大数据量、高并发的场景也很多。所以今天的实现方式在这个庞大的系统面前只是小打小闹。
