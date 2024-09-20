Java<br />Javalin 是一个轻量的 Web 框架。支持 WebSocket，HTTP2 和异步请求。简单的看了一下官方的说明文档，确实非常轻量，几行代码就可以启动一个 HTTP 服务。<br />Javalin 最初是 SparkJava 的一个分支，后来受到 JavaScript 框架 koa.js 的影响，逐渐独立成一个新的项目发展。<br />首先来看看一个比 Hello World 稍微复杂一些的例子：
```java
var app = Javalin.create(config -> {
    config.defaultContentType = "application/json";
    config.autogenerateEtags = true;
    config.addStaticFiles("/public");
    config.asyncRequestTimeout = 10_000L;
    config.dynamicGzip = true;
    config.enforceSsl = true;
}).routes(() -> {
    path("users", () -> {
        get(UserController::getAll);
        post(UserController::create);
        path(":user-id"(() -> {
            get(UserController::getOne);
            patch(UserController::update);
            delete(UserController::delete);
        });
        ws("events", userController::webSocketEvents);
    });
}).start(port);
```
<a name="RCvbP"></a>
## 验证路径参数
```java
var myQpStr = ctx.queryParam("my-qp"); // 没有验证，返回字符串或空
var myQpInt = ctx.pathParam("my-qp", Integer.class).get(); // 返回一个整数或抛出异常
var myQpInt = ctx.formParam("my-qp", Integer.class).check(i -> i > 4).get(); // 整数 > 4

// 验证两个依赖的查询参数 :
var fromDate = ctx.queryParam("from", Instant.class).get();
var toDate = ctx.queryParam("to", Instant.class)
    .check(it -> it.isAfter(fromDate), "'to' has to be after 'from'")
    .get();

// 验证一个json消息体:
var myObject = ctx.bodyValidator(MyObject.class)
    .check(obj -> obj.myObjectProperty == someValue)
    .get();
```
<a name="Oc5vv"></a>
## handler
```java
//前置handler
app.before(ctx -> {
    // 在所有请求之前运行
});
app.before("/path/*", ctx -> {
    // 在/path/*请求之前运行
});

//端点handler
app.get("/", ctx -> {
    // 一些代码
    ctx.json(object);
});

app.get("/hello/*, ctx -> {
    // 捕获所有对/hello/子路径的请求 
});

//后置handler
app.after(ctx -> {
    // 在所有请求之后运行
});
app.after("/path/*", ctx -> {
    // 在/path/*请求之后运行 
});
```
使用 `AccessManager` 接口来实现验证和授权。<br />如果要部署 Javalin 应用程序，开发人员只需创建一个包含了依赖（使用 maven-assembly-plugin）的 jar，然后用 `java -jar filename.jar` 发布该 jar。Javalin 自带一个嵌入式 Jetty 服务器，无需额外的应用程序服务器。<br />Javalin 还有 专门为教育工作者准备的页面 ，该页面强调学生可以从 Javalin 受益，因为 Javalin 提供了嵌入式的 Jetty 服务器，所以不需要 Servlet Container/Application 服务器配置就可以开始编码。<br />有一系列教程可供使用，如 Running on GraalVM 和 Kotlin CRUD REST API 。可以在教程页面找到完整的列表。<br />文档页面 提供了有关 Javalin 的更多细节。用户可以通过 maven 或从手动 maven中央库 下载 Javalin。
<a name="YT39Z"></a>
## 部署执行
通过 `mvn package` 就可以打包一个 jar 文件，直接运行 `java -jar xxx.jar` 就可以启动。
<a name="rzpMU"></a>
## 启动 7000 端口占用问题
因为在 macOS 下启动 Javalin 程序，默认是使用的 7000 端口，但是起来的时候发现端口被占用了。<br />用 lsof 查看
```bash
❯ sudo lsof -nP -i4TCP |grep 7000
Password:
Swinsian   1563 einverne   36u  IPv4 0xa107511eb4d4e74b      0t0  TCP 127.0.0.1:50677->127.0.0.1:7000 (CLOSED)
Swinsian   1563 einverne   37u  IPv4 0xa107511eb4d4e74b      0t0  TCP 127.0.0.1:50677->127.0.0.1:7000 (CLOSED)
ControlCe  1578 einverne   29u  IPv4 0xa107511eb42171fb      0t0  TCP *:7000 (LISTEN)
```
查看进程
```bash
❯ sudo ps aux | grep 1578
einverne         46918   0.7  0.0 34253900    968 s000  S+    2:37PM   0:00.00 grep --color=auto 1578
einverne          1578   0.0  0.1 36594320  36324   ??  S    Sun12PM   1:24.15 /System/Library/CoreServices/ControlCenter.app/Contents/MacOS/ControlCenter
```
发现竟然是系统的 ControlCenter 占用了本地 7000 端口，用如下的方法禁用。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676960762437-25ef9bc5-de8f-41c7-bb5b-b8011a5354f3.png#averageHue=%23ecebeb&clientId=u8d493647-081a-4&from=paste&id=ud8d95d7e&originHeight=879&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0ddc78ed-d6be-46dd-904d-e5fc23c961c&title=)
<a name="V6vLi"></a>
## 参考文档
官方文档：[https://javalin.io/documentation](https://javalin.io/documentation)<br />官方教程：[https://javalin.io/tutorials/](https://javalin.io/tutorials/)<br />Running Javalin on GraalVM：[https://javalin.io/2018/09/27/javalin-graalvm-example.html](https://javalin.io/2018/09/27/javalin-graalvm-example.html)
