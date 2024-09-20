Java
<a name="jBYhD"></a>
## 一、摘要
现在开源的市场上，能网络请求的工具非常的多，比如 HttpURLConnection 、Apache HttpClient、okHttp、Retrofit 等等。<br />虽然可选择的工具很多，但是作为一名开发者，希望在写代码的时候，能够轻松地调试应用程序的网络通信，选择适合的工具至关重要！<br />就目前所熟悉的，其中 Apache HttpClient 因其高效的性能、丰富的 api，在开源项目中使用非常广泛，Android 系统最早默认的网络请求工具也是使用 Apache HttpClient，但因为兼容性问题，Android 后期的版本中谷歌不愿意维护相关包，改而使用 okHttp。<br />现在 Android 系统中的网络请求框架，基本都是 okhttp 和 Retrofit 一统天下，两者其实都是 square 公司出品的，不同的地方在于 Retrofit 是基于 OkHttp 封装的一套 RESTful 网络请求框架，使用方面更加灵活，属于后起之秀！<br />既然大家都觉得 OkHttp 好用，一起来认识一下它！<br />来着网上对于 OkHttp 相关的介绍如下！<br />OkHttp 是 Square 公司基于 Java 和 Android 程序，封装的一个高性能 http 网络请求客户端，并且对外开源，它的设计初衷是为了更快地加载资源并节省带宽。<br />以下是使用 OkHttp 的主要优势：

- 支持HTTP/2（有效使用套接字）
- 连接池（在没有HTTP/2的情况下减少请求延迟）
- GZIP压缩（缩小下载大小）
- 响应缓存（避免了重新获取相同的数据）
- 从常见的连接问题中无声恢复
- 替代 IP 地址检测（在 IPv4 和 IPv6 环境下）
- 支持现代TLS功能（TLS 1.3，ALPN，证书钉子）
- 支持同步和异步调用

目前 OkHttp 在开源项目中被广泛使用，同时也是 Retrofit、Picasso 等库的核心库。<br />既然这么厉害，在网络通信中如何使用呢，一起来学习一下！
<a name="GFL9F"></a>
## 二、案例实践
<a name="oSj6s"></a>
#### 2.1、添加依赖包
在使用之前，需要先导入okhttp依赖包，不同的版本号，相关 api 稍有区别，本次介绍的 api 操作基于3.14.9版本号。
```xml
<dependency>
  <groupId>com.squareup.okhttp3</groupId>
  <artifactId>okhttp</artifactId>
  <version>3.14.9</version>
</dependency>
```
<a name="Wcjwv"></a>
#### 2.2、get 同步请求
okhttp发起get同步请求非常的简单，只需要几行代码就可以搞定。<br />案例如下：
```java
String url = "https://www.baidu.com/";

OkHttpClient client = new OkHttpClient();
// 配置GET请求
Request request = new Request.Builder()
        .url(url)
        .get()
        .build();

// 发起同步请求
try (Response response = client.newCall(request).execute()){
    // 打印返回结果
    System.out.println(response.body().string());
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="lRJ7t"></a>
#### 2.3、post 表单同步请求
okhttp发起post表单格式的数据提交，同步请求编程也非常的简单，只需要几行代码就可以搞定。<br />案例如下：
```java
String url = "https://www.baidu.com/";

OkHttpClient client = new OkHttpClient();
// 配置 POST + FORM 格式数据请求
RequestBody body = new FormBody.Builder()
        .add("userName", "zhangsan")
        .add("userPwd", "123456")
        .build();
Request request = new Request.Builder()
        .url(url)
        .post(body)
        .build();

// 发起同步请求
try (Response response = client.newCall(request).execute()){
    // 打印返回结果
    System.out.println(response.body().string());
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="FvNZI"></a>
#### 2.4、post 表单 + 文件上传，同步请求
如果在发起表单请求的时候，还需要上传文件，该如何实现呢？<br />案例如下：
```java
String url = "https://www.baidu.com/";

OkHttpClient client = new OkHttpClient();

// 要上传的文件
File file = new File("/doc/Downloads/429545913565844e9b26f97dbb57a1c3.jpeg");
RequestBody fileBody = RequestBody.create(MediaType.parse("image/jpg"), file);

// 表单 + 文件数据提交
RequestBody multipartBody = new MultipartBody.Builder()
        .setType(MultipartBody.FORM)
        .addFormDataPart("userName", "zhangsan")
        .addFormDataPart("userPwd", "123456")
        .addFormDataPart("userFile", "00.png", fileBody)
        .build();
Request request = new Request.Builder()
        .url(url)
        .post(multipartBody)
        .build();

// 发起同步请求
try (Response response = client.newCall(request).execute()){
    // 打印返回结果
    System.out.println(response.body().string());
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="VYEhq"></a>
#### 2.5、post + json 数据，同步请求
okhttp发起post + json格式的数据提交，同步请求编程也很简单。<br />案例如下：
```java
MediaType contentType = MediaType.get("application/json; charset=utf-8");
String url = "https://www.baidu.com/";
String json = "{}";

OkHttpClient client = new OkHttpClient();
// 配置 POST + JSON 请求
RequestBody body = RequestBody.create(contentType, json);
Request request = new Request.Builder()
        .url(url)
        .post(body)
        .build();

// 发起同步请求
try (Response response = client.newCall(request).execute()){
    // 打印返回结果
    System.out.println(response.body().string());
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="tmwqn"></a>
#### 2.5、文件下载，同步请求
文件下载，通常是get方式请求，只需要在响应端使用字节流接受数据即可！<br />案例如下：
```java
public static void main(String[] args)  {
    //目标存储文件
    String targetFile = "/doc/Downloads/1.png";
    //需要下载的原始文件
    String url = "https://www.baidu.com/img/PCtm_d9c8750bed0b3c7d089fa7d55720d6cf.png";

    OkHttpClient client = new OkHttpClient();
    // 配置GET请求
    Request request = new Request.Builder()
            .url(url)
            .build();

    // 发起同步请求
    try (Response response = client.newCall(request).execute()){
        // 获取文件字节流
        byte[] stream = response.body().bytes();
        // 写入目标文件
        writeFile(targetFile, stream);
    } catch (Exception e) {
        e.printStackTrace();
    }
}


/**
 * 写入目标文件
 * @param targetFile
 * @param stream
 * @throws IOException
 */
private static void writeFile(String targetFile, byte[] stream) throws IOException {
    String filePath = StringUtils.substringBeforeLast(targetFile, "/");
    Path folderPath = Paths.get(filePath);
    if(!Files.exists(folderPath)){
        Files.createDirectories(folderPath);
    }
    Path targetFilePath = Paths.get(targetFile);
    if(!Files.exists(targetFilePath)){
        Files.write(targetFilePath, stream, StandardOpenOption.CREATE);
    }
}
```
<a name="vjuzC"></a>
#### 2.6、其他方式的同步请求
在实际的项目开发中，有的接口需要使用put或者delete方式请求，应该如何处理呢？<br />put方式请求，案例如下：
```java
// 只需要在 Request 配置类中，换成 put 方式即可
Request request = new Request.Builder()
        .url(url)
        .put(body)
        .build();
```
同样的，delete方式请求也类似，案例如下：
```java
// 只需要在 Request 配置中，换成 delete 方式即可
Request request = new Request.Builder()
        .url(url)
        .delete(body)
        .build();
```
<a name="W5Krh"></a>
#### 2.7、自定义添加请求头部
大部分的时候，基于安全的考虑，很多时候需要把相关的鉴权参数放在请求头部，应该如何处理呢？<br />以post + json格式请求为例，添加头部请求参数，案例如下：
```java
MediaType contentType = MediaType.get("application/json; charset=utf-8");
String url = "https://www.baidu.com/";
String json = "{}";

OkHttpClient client = new OkHttpClient();

// 配置 header 头部请求参数
Headers headers = new Headers.Builder()
        .add("token", "11111-22222-333")
        .build();

// 配置 POST + JSON 请求
RequestBody body = RequestBody.create(contentType, json);
Request request = new Request.Builder()
        .url(url)
        .headers(headers)
        .post(body)
        .build();

// 发起同步请求
try (Response response = client.newCall(request).execute()){
    // 打印返回结果
    System.out.println(response.body().string());
} catch (Exception e) {
    e.printStackTrace();
}
```
<a name="ZCmBm"></a>
#### 2.8、发起异步请求
在上文中介绍的都是同步请求，在最开始也说到 OkHttp 不仅支持同步调用，也异步调用，那么如何进行异步请求编程呢？其实操作很简单，案例如下：
```java
String url = "https://www.baidu.com/";
OkHttpClient client = new OkHttpClient().newBuilder().build();
Request request = new Request.Builder()
        .url(url)
        .get()
        .build();

// 发起异步请求
client.newCall(request).enqueue(new Callback() {
    @Override
    public void onFailure(Call call, IOException e) {
        System.out.println("请求异常 + " + e.getMessage());
    }

    @Override
    public void onResponse(Call call, Response response) throws IOException {
        System.out.println("请求完成，返回结果：" + response.body().string());
    }
});
```
<a name="N2plN"></a>
## 三、小结
以上就是针对 OkHttp 在使用上，做了一次简单的内容总结，整体下来，从使用上来讲，api 的操作确实比 Apache HttpClient 要简单很多。<br />关于 OkHttp 的内容其实还有很多，比如请求参数的全局配置、全局拦截器、websocket 等功能。
