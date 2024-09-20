Java<br />WebSocket的各种集成方式：

- Javax
- WebMVC
- WebFlux
- Java-WebSocket
- SocketIO
- Netty

主要介绍一下前3种方式，毕竟现在的主流框架还是Spring Boot<br />而后3种其实和Spring Boot并不强行绑定，基于Java就可以支持，不过也会对后3种做个简单的介绍，先混个眼熟就行了<br />那么接下来就来看看前3种方式（Javax，WebMVC，WebFlux）在Spring Boot中的服务端和客户端配置（客户端配置也超重要的有木有，平时用不到，用到了却基本找不到文档，这也太绝望了）
<a name="xyjkF"></a>
## Javax
在java的扩展包javax.websocket中就定义了一套WebSocket的接口规范
<a name="CD5Zf"></a>
### 服务端
一般使用注解的方式来进行配置
<a name="Y9D3G"></a>
#### 第一步
```java
@Component
@ServerEndpoint("/websocket/{type}")
public class JavaxWebSocketServerEndpoint {

    @OnOpen
    public void onOpen(Session session, EndpointConfig config,
                       @PathParam(value = "type") String type) {
        //连接建立
    }

    @OnClose
    public void onClose(Session session, CloseReason reason) {
        //连接关闭
    }

    @OnMessage
    public void onMessage(Session session, String message) {
        //接收文本信息
    }

    @OnMessage
    public void onMessage(Session session, PongMessage message) {
        //接收pong信息
    }

    @OnMessage
    public void onMessage(Session session, ByteBuffer message) {
        //接收二进制信息，也可以用byte[]接收
    }

    @OnError
    public void onError(Session session, Throwable e) {
        //异常处理
    }
}
```
在类上添加`@ServerEndpoint`注解来表示这是一个服务端点，同时可以在注解中配置路径，这个路径可以配置成动态的，使用`{}`包起来就可以了

- `@OnOpen`用来标记对应的方法作为客户端连接上来之后的回调，Session就相当于和客户端的连接，可以把它缓存起来用于发送消息；通过`@PathParam`注解就可以获得动态路径中对应值了
- `@OnClose`用来标记对应的方法作为客户端断开连接之后的回调，可以在这个方法中移除对应Session的缓存，同时可以接受一个`CloseReason`的参数用于获取关闭原因
- `@OnMessage`用来标记对应的方法作为接收到消息之后的回调，可以接受文本消息，二进制消息和pong消息
- `@OnError`用来标记对应的方法作为抛出异常之后的回调，可以获得对应的Session和异常对象
<a name="f9bZY"></a>
#### 第二步
```
implementation 'org.springframework.boot:spring-boot-starter-websocket'
```
```java
@Configuration(proxyBeanMethods = false)
public class JavaxWebSocketConfiguration {

    @Bean
    public ServerEndpointExporter serverEndpointExporter() {
        return new ServerEndpointExporter();
    }
}
```
依赖Spring的WebSocket模块，手动注入`ServerEndpointExporter`就可以了<br />需要注意`ServerEndpointExporter`是Spring中的类，算是Spring为了支持javax.websocket的原生用法所提供的支持类
<a name="nlYBV"></a>
#### 冷知识
javax.websocket库中定义了`PongMessage`而没有`PingMessage`<br />通过测试发现基本上所有的WebSocket包括前端js自带的，都实现了自动回复；也就是说当接收到一个ping消息之后，是会自动回应一个pong消息，所以没有必要再自己接受ping消息来处理了，即不会接受到ping消息<br />当然上面讲的ping和pong都是需要使用框架提供的api，如果是自己通过Message来自定义心跳数据的话是没有任何的处理的，下面是对应的api
```java
//发送ping
session.getAsyncRemote().sendPing(ByteBuffer buffer);

//发送pong
session.getAsyncRemote().sendPong(ByteBuffer buffer);
```
然后又发现js自带的WebSocket是没有发送ping的api的，所以是不是可以猜想当初就是约定服务端发送ping，客户端回复pong
<a name="T2ln7"></a>
### 客户端
客户端也是使用注解配置
<a name="Mh7jr"></a>
#### 第一步
```java
@ClientEndpoint
public class JavaxWebSocketClientEndpoint {

    @OnOpen
    public void onOpen(Session session) {
        //连接建立
    }

    @OnClose
    public void onClose(Session session, CloseReason reason) {
        //连接关闭
    }

    @OnMessage
    public void onMessage(Session session, String message) {
        //接收文本消息
    }

    @OnMessage
    public void onMessage(Session session, PongMessage message) {
        //接收pong消息
    }

    @OnMessage
    public void onMessage(Session session, ByteBuffer message) {
        //接收二进制消息
    }

    @OnError
    public void onError(Session session, Throwable e) {
        //异常处理
    }
}
```
客户端使用`@ClientEndpoint`来标记，其他的`@OnOpen`，`@OnClose`，`@OnMessage`，`@OnError`和服务端一模一样。
<a name="yti61"></a>
### 第二步
```java
WebSocketContainer container = ContainerProvider.getWebSocketContainer();
Session session = container.connectToServer(JavaxWebSocketClientEndpoint.class, uri);
```
可以通过`ContainerProvider`来获得一个`WebSocketContainer`，然后调用`connectToServer`方法将客户端类和连接的uri传入就行了
<a name="aZ4EP"></a>
#### 冷知识
通过`ContainerProvider#getWebSocketContainer`获得`WebSocketContainer`其实是基于SPI实现的<br />在Spring的环境中更推荐大家使用`ServletContextAware`来获得，代码如下
```java
@Component
public class JavaxWebSocketContainer implements ServletContextAware {

    private volatile WebSocketContainer container;

    public WebSocketContainer getContainer() {
        if (container == null) {
            synchronized (this) {
                if (container == null) {
                    container = ContainerProvider.getWebSocketContainer();
                }
            }
        }
        return container;
    }

    @Override
    public void setServletContext(@NonNull ServletContext servletContext) {
        if (container == null) {
            container = (WebSocketContainer) servletContext
                .getAttribute("javax.websocket.server.ServerContainer");
        }
    }
}
```
<a name="vUJnn"></a>
### 发消息
```java
Session session = ...

//发送文本消息
session.getAsyncRemote().sendText(String message);

//发送二进制消息
session.getAsyncRemote().sendBinary(ByteBuffer message);

//发送对象消息，会尝试使用Encoder编码
session.getAsyncRemote().sendObject(Object message);

//发送ping
session.getAsyncRemote().sendPing(ByteBuffer buffer);

//发送pong
session.getAsyncRemote().sendPong(ByteBuffer buffer);
```
<a name="WnTPI"></a>
## WebMVC
依赖肯定是必不可少的
```
implementation 'org.springframework.boot:spring-boot-starter-websocket'
```
<a name="Zz11V"></a>
### 服务端
<a name="gL076"></a>
#### 第一步
```java
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.WebSocketMessage;
import org.springframework.web.socket.WebSocketSession;

public class ServletWebSocketServerHandler implements WebSocketHandler {

    @Override
    public void afterConnectionEstablished(@NonNull WebSocketSession session) throws Exception {
        //连接建立
    }

    @Override
    public void handleMessage(@NonNull WebSocketSession session, @NonNull WebSocketMessage<?> message) throws Exception {
        //接收消息
    }

    @Override
    public void handleTransportError(@NonNull WebSocketSession session, @NonNull Throwable exception) throws Exception {
        //异常处理
    }

    @Override
    public void afterConnectionClosed(@NonNull WebSocketSession session, @NonNull CloseStatus closeStatus) throws Exception {
        //连接关闭
    }

    @Override
    public boolean supportsPartialMessages() {
        //是否支持接收不完整的消息
        return false;
    }
}
```
实现一个`WebSocketHandler`来处理WebSocket的连接，关闭，消息和异常
<a name="XboPp"></a>
#### 第二步
```java
@Configuration
@EnableWebSocket
public class ServletWebSocketServerConfigurer implements WebSocketConfigurer {

    @Override
    public void registerWebSocketHandlers(@NonNull WebSocketHandlerRegistry registry) {
        registry
        //添加处理器到对应的路径
        .addHandler(new ServletWebSocketServerHandler(), "/websocket")
        .setAllowedOrigins("*");
    }
}
```
首先需要添加`@EnableWebSocket`来启用WebSocket<br />然后实现`WebSocketConfigurer`来注册WebSocket路径以及对应的`WebSocketHandler`
<a name="g90mU"></a>
#### 握手拦截
提供了`HandshakeInterceptor`来拦截握手
```java
@Configuration
@EnableWebSocket
public class ServletWebSocketServerConfigurer implements WebSocketConfigurer {

    @Override
    public void registerWebSocketHandlers(@NonNull WebSocketHandlerRegistry registry) {
        registry
        //添加处理器到对应的路径
        .addHandler(new ServletWebSocketServerHandler(), "/websocket")
        //添加握手拦截器
        .addInterceptors(new ServletWebSocketHandshakeInterceptor())
        .setAllowedOrigins("*");
    }

    public static class ServletWebSocketHandshakeInterceptor implements HandshakeInterceptor {

        @Override
        public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {
            //握手之前
            //继续握手返回true, 中断握手返回false
            return false;
        }

        @Override
        public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception) {
            //握手之后
        }
    }
}
```
<a name="zfWaQ"></a>
#### 冷知识
在集成的时候发现这种方式没办法动态匹配路径，它的路径就是固定的，没办法使用如/websocket/**这样的通配符<br />在研究了一下之后发现可以在`UrlPathHelper`上做点文章
```java
@Configuration
@EnableWebSocket
public class ServletWebSocketServerConfigurer implements WebSocketConfigurer {

    @Override
    public void registerWebSocketHandlers(@NonNull WebSocketHandlerRegistry registry) {
        if (registry instanceof ServletWebSocketHandlerRegistry) {
            //替换UrlPathHelper
            ((ServletWebSocketHandlerRegistry) registry)
            .setUrlPathHelper(new PrefixUrlPathHelper("/websocket"));
        }

        registry
        //添加处理器到对应的路径
        .addHandler(new ServletWebSocketServerHandler(), "/websocket/**")
        .setAllowedOrigins("*");
    }

    public class PrefixUrlPathHelper extends UrlPathHelper {

        private String prefix;

        @Override
        public @NonNull String resolveAndCacheLookupPath(@NonNull HttpServletRequest request) {
            //获得原本的Path
            String path = super.resolveAndCacheLookupPath(request);
            //如果是指定前缀就返回对应的通配路径
            if (path.startsWith(prefix)) {
                return prefix + "/**";
            }
            return path;
        }
    }
}
```
因为它内部实际上就是用一个`Map<String, WebSocketHandler>`来存的，所以没有办法用通配符<br />主要是有现成的`AntPathMatcher`实现通配应该不麻烦才对
<a name="TXeH5"></a>
### 客户端
<a name="gnJ8p"></a>
#### 第一步
```java
public class ServletWebSocketClientHandler implements WebSocketHandler {

    @Override
    public void afterConnectionEstablished(@NonNull WebSocketSession session) throws Exception {
        //连接建立
    }

    @Override
    public void handleMessage(@NonNull WebSocketSession session, @NonNull WebSocketMessage<?> message) throws Exception {
        //接收消息
    }

    @Override
    public void handleTransportError(@NonNull WebSocketSession session, @NonNull Throwable exception) throws Exception {
        //异常处理
    }

    @Override
    public void afterConnectionClosed(@NonNull WebSocketSession session, @NonNull CloseStatus closeStatus) throws Exception {
        //连接关闭
    }

    @Override
    public boolean supportsPartialMessages() {
        //是否支持接收不完整的消息
        return false;
    }
}
```
和服务端一样需要先实现一个`WebSocketHandler`来处理WebSocket的连接，关闭，消息和异常
<a name="SoPf5"></a>
#### 第二步
```java
WebSocketClient client = new StandardWebSocketClient();
WebSocketHandler handler = new ServletWebSocketClientHandler();
WebSocketConnectionManager manager = new WebSocketConnectionManager(client, handler, uri);
manager.start();
```
首先需要先new一个`StandardWebSocketClient`，可以传入一个`WebSocketContainer`参数，获得该对象的方式之前已经介绍过了，这边就先略过<br />然后new一个`WebSocketConnectionManager`传入`WebSocketClient`，`WebSocketHandler`还有路径uri<br />最后调用一下`WebSocketConnectionManager`的`start`方法就可以了
<a name="lXrJ4"></a>
#### 冷知识
这里如果大家去看WebSocketClient的实现类就会发现有`StandardWebSocketClient`还有`JettyWebSocketClient`等等，所以大家可以根据自身项目所使用的容器来选择不同的`WebSocketClient`实现类<br />这里给大家贴一小段Spring适配不同容器WebSocket的代码
```java
public abstract class AbstractHandshakeHandler implements HandshakeHandler, Lifecycle {

    private static final boolean tomcatWsPresent;

    private static final boolean jettyWsPresent;

    private static final boolean jetty10WsPresent;

    private static final boolean undertowWsPresent;

    private static final boolean glassfishWsPresent;

    private static final boolean weblogicWsPresent;

    private static final boolean websphereWsPresent;

    static {
        ClassLoader classLoader = AbstractHandshakeHandler.class.getClassLoader();
        tomcatWsPresent = ClassUtils.isPresent(
            "org.apache.tomcat.websocket.server.WsHttpUpgradeHandler", classLoader);
        jetty10WsPresent = ClassUtils.isPresent(
            "org.eclipse.jetty.websocket.server.JettyWebSocketServerContainer", classLoader);
        jettyWsPresent = ClassUtils.isPresent(
            "org.eclipse.jetty.websocket.server.WebSocketServerFactory", classLoader);
        undertowWsPresent = ClassUtils.isPresent(
            "io.undertow.websockets.jsr.ServerWebSocketContainer", classLoader);
        glassfishWsPresent = ClassUtils.isPresent(
            "org.glassfish.tyrus.servlet.TyrusHttpUpgradeHandler", classLoader);
        weblogicWsPresent = ClassUtils.isPresent(
            "weblogic.websocket.tyrus.TyrusServletWriter", classLoader);
        websphereWsPresent = ClassUtils.isPresent(
            "com.ibm.websphere.wsoc.WsWsocServerContainer", classLoader);
    }
}
```
<a name="NnfJt"></a>
### 发消息
```java
import org.springframework.web.socket.*;

WebSocketSession session = ...

//发送文本消息
session.sendMessage(new TextMessage(CharSequence message);

//发送二进制消息
session.sendMessage(new BinaryMessage(ByteBuffer message));

//发送ping
session.sendMessage(new PingMessage(ByteBuffer message));

//发送pong
session.sendMessage(new PongMessage(ByteBuffer message));
```
<a name="S0A29"></a>
## WebFlux
WebFlux的WebSocket不需要额外的依赖包
<a name="voAaK"></a>
### 服务端
<a name="SKsft"></a>
#### 第一步
```java
import org.springframework.web.reactive.socket.WebSocketHandler;
import org.springframework.web.reactive.socket.WebSocketSession;

public class ReactiveWebSocketServerHandler implements WebSocketHandler {

    @NonNull
    @Override
    public Mono<Void> handle(WebSocketSession session) {
        Mono<Void> send = session.send(Flux.create(sink -> {
            //可以持有sink对象在任意时候调用next发送消息
            sink.next(WebSocketMessage message);
        })).doOnError(it -> {
            //异常处理
        });

        Mono<Void> receive = session.receive()
                .doOnNext(it -> {
                    //接收消息
                })
                .doOnError(it -> {
                    //异常处理
                })
                .then();

        @SuppressWarnings("all")
        Disposable disposable = session.closeStatus()
                .doOnError(it -> {
                    //异常处理
                })
                .subscribe(it -> {
                    //连接关闭
                });

        return Mono.zip(send, receive).then();
    }
}
```
首先需要注意这里的`WebSocketHandler`和`WebSocketSession`是reactive包下的

- 通过`WebSocketSession#send`方法来持有一个`FluxSink<WebSocketMessage>`来用于发送消息
- 通过`WebSocketSession#receive`来订阅消息
- 通过`WebSocketSession#closeStatus`来订阅连接关闭事件
<a name="vdNQ3"></a>
#### 第二步
```java
@Component
public class ReactiveWebSocketServerHandlerMapping extends SimpleUrlHandlerMapping {

    public ReactiveWebSocketServerHandlerMapping() {
        Map<String, WebSocketHandler> map = new HashMap<>();
        map.put("/websocket/**", new ReactiveWebSocketServerHandler());
        setUrlMap(map);
        setOrder(100);
    }
}
```
注册一个`HandlerMapping`同时配置路径和对应的`WebSocketHandler`
<a name="QxBcm"></a>
#### 第三步
```java
@Configuration(proxyBeanMethods = false)
public class ReactiveWebSocketConfiguration {

    @Bean
    public WebSocketHandlerAdapter webSocketHandlerAdapter() {
        return new WebSocketHandlerAdapter();
    }
}
```
注入`WebSocketHandlerAdapter`
<a name="TTHAg"></a>
#### 冷知识
自定义的`HandlerMapping`需要设置order，如果不设置，默认为`Ordered.LOWEST_PRECEDENCE`，会导致这个`HandlerMapping`被放在最后，当有客户端连接上来时会被其他的`HandlerMapping`优先匹配上而连接失败
<a name="WDdt9"></a>
### 客户端
<a name="Wzv67"></a>
#### 第一步
```java
public class ReactiveWebSocketClientHandler implements WebSocketHandler {

    @NonNull
    @Override
    public Mono<Void> handle(WebSocketSession session) {
        Mono<Void> send = session.send(Flux.create(sink -> {
            //可以持有sink对象在任意时候调用next发送消息
            sink.next(WebSocketMessage message);
        })).doOnError(it -> {
            //处理异常
        });

        Mono<Void> receive = session.receive()
        .doOnNext(it -> {
            //接收消息
        })
        .doOnError(it -> {
            //异常处理
        })
        .then();

        @SuppressWarnings("all")
        Disposable disposable = session.closeStatus()
        .doOnError(it -> {
            //异常处理
        })
        .subscribe(it -> {
            //连接关闭
        });

        return Mono.zip(send, receive).then();
    }
}
```
客户端`WebSocketHandler`的写法和服务端的一样
<a name="a94RB"></a>
#### 第二步
```java
import org.springframework.web.reactive.socket.client.WebSocketClient;

WebSocketClient client = ReactorNettyWebSocketClient();
WebSocketHandler handler = new ReactiveWebSocketClientHandler();
client.execute(uri, handler).subscribe();
```
首先需要先new一个`ReactorNettyWebSocketClient`<br />然后调用一下`WebSocketClient`的`execute`方法传入路径uri和`WebSocketHandler`并继续调用`subscribe`方法就行了。
<a name="Bu0QO"></a>
#### 冷知识
和WebMVC中的`WebSocketClient`一样，Reactive包中的`WebSocketClient`也有很多实现类，比如`ReactorNettyWebSocketClient`，`JettyWebSocketClient`，`UndertowWebSocketClient`，`TomcatWebSocketClient`等等，也是需要大家基于自身项目的容器使用不同的实现类<br />这里贴一小段`Reactive`适配不同容器WebSocket的代码
```java
public class HandshakeWebSocketService implements WebSocketService, Lifecycle {

    private static final boolean tomcatPresent;

    private static final boolean jettyPresent;

    private static final boolean jetty10Present;

    private static final boolean undertowPresent;

    private static final boolean reactorNettyPresent;

    static {
        ClassLoader loader = HandshakeWebSocketService.class.getClassLoader();
        tomcatPresent = ClassUtils.isPresent("org.apache.tomcat.websocket.server.WsHttpUpgradeHandler", loader);
        jettyPresent = ClassUtils.isPresent("org.eclipse.jetty.websocket.server.WebSocketServerFactory", loader);
        jetty10Present = ClassUtils.isPresent("org.eclipse.jetty.websocket.server.JettyWebSocketServerContainer", loader);
        undertowPresent = ClassUtils.isPresent("io.undertow.websockets.WebSocketProtocolHandshakeHandler", loader);
        reactorNettyPresent = ClassUtils.isPresent("reactor.netty.http.server.HttpServerResponse", loader);
    }
}
```
<a name="rA1yX"></a>
### 发消息
需要使用在`WebSocketHandler`中获得的`FluxSink<WebSocketMessage>`来发送消息
```java
import org.springframework.web.reactive.socket.CloseStatus;
import org.springframework.web.reactive.socket.WebSocketMessage;
import org.springframework.web.reactive.socket.WebSocketSession;

public class ReactiveWebSocket {

    private final WebSocketSession session;

    private final FluxSink<WebSocketMessage> sender;

    public ReactiveWebSocket(WebSocketSession session, FluxSink<WebSocketMessage> sender) {
        this.session = session;
        this.sender = sender;
    }

    public String getId() {
        return session.getId();
    }

    public URI getUri() {
        return session.getHandshakeInfo().getUri();
    }

    public void send(Object message) {
        if (message instanceof WebSocketMessage) {
            sender.next((WebSocketMessage) message);
        } else if (message instanceof String) {
            //发送文本消息
            sender.next(session.textMessage((String) message));
        } else if (message instanceof DataBuffer) {
            //发送二进制消息
            sender.next(session.binaryMessage(factory -> (DataBuffer) message));
        } else if (message instanceof ByteBuffer) {
            发送二进制消息
            sender.next(session.binaryMessage(factory -> factory.wrap((ByteBuffer) message)));
        } else if (message instanceof byte[]) {
            发送二进制消息
            sender.next(session.binaryMessage(factory -> factory.wrap((byte[]) message)));
        } else {
            throw new IllegalArgumentException("Message type not match");
        }
    }

    public void ping() {
        //发送ping
        sender.next(session.pingMessage(factory -> factory.wrap(ByteBuffer.allocate(0))));
    }

    public void pong() {
        //发送pong
        sender.next(session.pongMessage(factory -> factory.wrap(ByteBuffer.allocate(0))));
    }

    public void close(CloseStatus reason) {
        sender.complete();
        session.close(reason).subscribe();
    }
}
```
<a name="zICxa"></a>
## Java-WebSocket
这是一个纯java的第三方库，专门用于实现WebSocket<br />Github上已经有很详细的使用教程了，现在有9k+的Star<br />传送门：[https://github.com/TooTallNate/Java-WebSocket](https://github.com/TooTallNate/Java-WebSocket)
<a name="BzC3s"></a>
## SocketIO
该库使用的协议是经过自己封装的，支持很多的语言，提供了统一的接口，所以需要使用它提供的Server和Client来连接，如socket.io-server-java和socket.io-client-java<br />这个库了解下来主要用于实时聊天等场景，所以如果只是普通的WebSocket功能就有点大材小用了<br />Github上也有非常详细的使用文档，大家如果有兴趣可以研究一下<br />传送门：[https://github.com/socketio](https://github.com/socketio)
<a name="Be7zv"></a>
## Netty
这个大家应该都比较熟悉了，就算没用过肯定也听过<br />网上的文档和示例也非常多，Github传送门<br />传送门：[https://github.com/netty/netty](https://github.com/netty/netty)
