Java<br />Java11 HttpClient 不仅可以发送 http/https，对 websocket 的也是支持的，而且简洁易用。
<a name="zqfjo"></a>
## 一、构造 WebSocket Builder
构造一个 WebSocket 也比较容易，使用 `HttpClient.newHttpClient().newWebSocketBuilder()` 创建一个构造器即可。
```java
// 构建一个 webSocket Builder
WebSocket.Builder webSocketBuilder = HttpClient.newHttpClient()
 .newWebSocketBuilder();

// 支持 websocket 子协议，当然子协议需要自行实现
webSocketBuilder.subprotocols("mqtt");

// 超时配置，示例：3s
webSocketBuilder.connectTimeout(Duration.ofSeconds(3));

// 异步构建 WebSocket 请求
CompletableFuture<WebSocket> future = webSocketBuilder.buildAsync(uri, listener);
```
<a name="LYVv8"></a>
## 二、WebSocket Listener 监听器
WebSocket 消息监听也比较简单，实现 `WebSocket.Listener` 即可，可选择性的实现 `onOpen`、`onBinary`、`onPing`、`onPong`、`onClose`、`onError` 和 `onText` 方法。对于大的数据包也可以拆分成多个包，部分消息接口都有 boolean last 参数，用来判断半包数据（半包处理详见 **onText** 方法）。
```java
/**
 * WebSocket 消息监听器
 */
public class WebSocketListener implements WebSocket.Listener {

    @Override
    public void onOpen(WebSocket webSocket) {
        // WebSocket 已经连接
    }

    @Override
    public CompletionStage<?> onBinary(WebSocket webSocket, ByteBuffer data, boolean last) {
        // 二进制消息
        return WebSocket.Listener.super.onBinary(webSocket, data, last);
    }

    @Override
    public CompletionStage<?> onPing(WebSocket webSocket, ByteBuffer message) {
        // 收到收到 Ping 报文
        return WebSocket.Listener.super.onPing(webSocket, message);
    }

    @Override
    public CompletionStage<?> onPong(WebSocket webSocket, ByteBuffer message) {
        // 收到心跳 pong 消息
        return WebSocket.Listener.super.onPong(webSocket, message);
    }

    @Override
    public CompletionStage<?> onClose(WebSocket webSocket, int statusCode, String reason) {
        // 接收一个Close消息，表示WebSocket的输入已经关闭。
        return WebSocket.Listener.super.onClose(webSocket, statusCode, reason);
    }

    @Override
    public void onError(WebSocket webSocket, Throwable error) {
        // 发生错误
    }

    private List<CharSequence> parts = new ArrayList<>();
    private CompletableFuture<?> accumulatedMessage = new CompletableFuture<>();

    @Override
    public CompletionStage<?> onText(WebSocket webSocket, CharSequence message, boolean last) {
        System.out.println(message);
        parts.add(message);
        // 增加接收方法调用的计数器，请求获取下一次的消息
        webSocket.request(1);
        if (last) {
            // 如果为 last 包，处理整个包
            processWholeText(parts);
            parts = new ArrayList<>();
            accumulatedMessage.complete(null);
            CompletionStage<?> cf = accumulatedMessage;
            accumulatedMessage = new CompletableFuture<>();
            return cf;
        }
        return accumulatedMessage;
    }

    /**
     * 处理整个消息
     *
 	 * @param parts 消息片段列表
     */
    private static void processWholeText(List<CharSequence> parts) {
        parts.forEach(System.out::println);
    }

}
```
<a name="mao2a"></a>
## 三、发送消息
发送消息支持文本（sendText）和二进制（sendBinary），特别需要注意的是这个 last 参数。它用来标记是否是最后一包数据，如图片、文件等大报文消息，通常会拆包后服务端再组装。
```java
// WebSocket 连接
WebSocket webSocket = future.join();

// 是否最后一包数据，用于大数据包拆分成多个数据包依次发送。
boolean last = true;
// 发送文本消息
webSocket.sendText("mica 666!!!", last);
// 发送二进制消息
byte[] data = "pig 666!!!".getBytes(StandardCharsets.UTF_8);
webSocket.sendBinary(ByteBuffer.wrap(data), last);
```
<a name="Lclwh"></a>
## 四、完整示例
多说无益，上代码：
```java
WebSocket webSocket = HttpClient.newHttpClient()
    .newWebSocketBuilder()
    .buildAsync(URI.create("ws://121.40.165.18:8800"), new WebSocketListener())
    .join();

// 模拟定时发送消息
Timer timer = new Timer();
timer.schedule(new TimerTask() {
    @Override
    public void run() {
        webSocket.sendText("mica 666,pig 666!!!", true);
    }
}, 3000, 3000);
```
使用 Java11 HttpClient 收、发消息都非常容易。
