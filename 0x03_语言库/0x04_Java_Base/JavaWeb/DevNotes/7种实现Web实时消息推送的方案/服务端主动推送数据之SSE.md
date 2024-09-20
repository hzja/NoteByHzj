JavaWeb
<a name="C1eSA"></a>
## 1、SSE
首先来看一个概念叫做 SSE。<br />SSE 全称是 Server-Sent Events，它的作用和 WebSocket 的作用相似，都是建立浏览器与服务器之间的通信渠道，然后服务器向浏览器推送信息，不同的是，WebSocket 是一种全双工通信协议，而 SSE 则是一种单工通信协议，即使用 SSE 只能服务器向浏览器推送信息流，浏览器如果向服务器发送信息，就是一个普通的 HTTP 请求。<br />使用 SSE，当服务端给客户端响应的时候，他不是发送一个一次性数据包，而是会发送一个数据流，这个时候客户端的连接不会关闭，会一直等待服务端发送过来的数据流，常见的视频播放其实就是这样的例子。<br />SSE 和 WebSocket 主要有如下区别：

- SSE 使用 HTTP 协议，现有的服务器软件都支持。WebSocket 是一个独立协议。
- SSE 属于轻量级，使用简单；WebSocket 协议相对复杂。
- SSE 默认支持断线重连，WebSocket 需要自己实现。
- SSE 一般只用来传送文本，二进制数据需要编码后传送，WebSocket 默认支持传送二进制数据。
- SSE 支持自定义发送的消息类型。

接下来通过一个简单的例子来向大家展示 SSE 的用法。
<a name="xQN8R"></a>
## 2、开发服务端
根据第一小节的描述，大家也能看出来，SSE 其实和框架没有关系，所以这里就创建一个普通的 Java Web 项目，用最最基本的 Servlet 来向大家演示 SSE 的功能。<br />首先创建一个 SseServlet，内容如下：
```java
@WebServlet(urlPatterns = "/sse")
public class SseServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doPost(req,resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("text/event-stream;charset=utf-8");
        PrintWriter out = resp.getWriter();
        for (int i = 0; i < 10; i++) {
            out.write("data: 江南一点雨:" + i+"\n\n");
            out.flush();
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
```
往出写 10 条数据，每写一条就睡眠 1 秒钟。代码并不难，但是这里有几个细节需要注意下：

1. 响应的 `Content-Type` 记得设置为 `text/event-stream`，这是关键。
2. 每一次发送的信息，由若干个 `message` 组成，每个 `message` 之间用 `\n\n` 分隔，每个 message 内部由若干行组成。在上面的案例中，每一个 for 循环中就是发送一个 `message`。
3. 每一行的数据格式是 ：`[field]: value\n`。`field` 有四种不同取值：
   - `data`：`data` 用来表示数据内容，就像上面的例子。
   - `id`：`id` 相当于是每一条数据的唯一编号，浏览器用 `lastEventId` 属性读取这个值。一旦连接断线，浏览器会发送一个 `HTTP` 头，里面包含一个特殊的 `Last-Event-ID` 头信息，将这个值发送回来，用来帮助服务器端重建连接。因此，这个头信息可以被视为一种同步机制。
   - `event`：`event` 字段表示自定义的事件类型，默认是 `message` 事件。
   - `retry`：服务器可以用 `retry` 字段，指定浏览器重新发起连接的时间间隔。

开发完成后，启动服务端访问 /sse 接口来看看效果：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1624262091242-253edb90-3bf3-47a6-8007-19bf135366eb.gif#averageHue=%23fcfcfc&clientId=uc08396c2-10b5-4&from=paste&id=u21ccdf59&originHeight=372&originWidth=310&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue1333130-9738-4cc0-b476-3b73a8abebe&title=)<br />可以看到，客户端每隔 1 秒就能收到服务端的数据。
<a name="yplYA"></a>
## 3、开发客户端
前面是一个服务端的案例，接下来看看客户端的案例，新建一个 html 页面，添加如下 js：
```javascript
var es = new EventSource("/sse");
es.onopen = function (e) {
    console.log("open")
};
es.onmessage = function (e) {
    console.log(e.data);
}
es.onerror = function (e) {
    console.log("error")
    es.close()
}
```
关于上面这段代码：

1. 首先新建一个 `EventSource` 对象，参数就是服务端的地址。它还有一个可选的参数，可选参数重可以描述是否将 Cookie 一起发送出去 `var es = new EventSource("/es", { withCredentials: true });`（可在跨域时使用该参数）。
2. 当建立连接后，就会触发 `onopen` 函数，当收到服务端发送来的消息，就会触发 `onmessage` 函数，当连接出错的时候，就会触发 `onerror` 函数。
3. `es.close` 表示关闭 SSE 连接。

这三种类型的事件，还可以通过如下方式来定义：
```javascript
var es = new EventSource("/sse");
es.addEventListener("open", function (e) {
    console.log("open");
})
es.addEventListener("message", function (e) {
    console.log(e.data);
})
es.addEventListener("error", function (e) {
    console.log("error")
    es.close();
})
```
效果与上面的一致，来看看运行效果图：<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1624262091211-57e36b42-0df3-4b12-ba69-c4c4e4e40187.gif#clientId=uc08396c2-10b5-4&from=paste&id=u2968e553&originHeight=336&originWidth=333&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u04e51ff2-62a3-47b1-9d64-d8debb45232&title=)<br />消息接收完后，会触发 `onerror` 事件，此时可以关闭 SSE 连接，否则就会从头开始继续接收数据。
<a name="iHuys"></a>
## 4、自定义事件
也可以自定义 SSE 事件。<br />先来看服务端如何自定义：
```java
@WebServlet(urlPatterns = "/sse")
public class SseServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("text/event-stream;charset=utf-8");
        PrintWriter out = resp.getWriter();
        for (int i = 0; i < 10; i++) {
            out.write("event:javaboy\n");
            out.write("data: 江南一点雨:" + i + "\n\n");
            out.flush();
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
```
如上，在每一行消息之前添加 `out.write("event:javaboy\n");` 表示自定义事件类型，当然也可以添加事件 id，方式如下：
```java
@WebServlet(urlPatterns = "/sse")
public class SseServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("text/event-stream;charset=utf-8");
        PrintWriter out = resp.getWriter();
        for (int i = 0; i < 10; i++) {
            out.write("event:javaboy\n");
            out.write("id:" + i + "\n");
            out.write("data: 江南一点雨:" + i + "\n\n");
            out.flush();
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
```
服务端定义完成后，接下来再来看看前端该如何接收数据：
```javascript
var es = new EventSource("/sse");
es.addEventListener("open", function (e) {
    console.log("open");
})
es.addEventListener("javaboy", function (e) {
    console.log(e.data, e.lastEventId, e);
})
es.addEventListener("error", function (e) {
    console.log("error")
    es.close();
})
```
此时在 `addEventListener` 方法中，输入自定义的事件名称，然后在回调函数中处理事件。<br />可以通过 `e.lastEventId` 访问到消息的 id。
