Java<br />Forest 是一个开源的 Java HTTP 客户端框架，它能够将 HTTP 所有请求信息（包括 URL、Header 以及 Body 等信息）绑定到自定义的 Interface 方法上，能够通过调用本地接口方法的方式发送 HTTP 请求。<br />使用 Forest 就像使用类似 Dubbo 那样的 RPC 框架一样，只需要定义接口，调用接口即可，不必关心具体发送 HTTP 请求的细节。同时将 HTTP 请求信息与业务代码解耦，方便统一管理大量 HTTP 的 URL、Header 等信息。而请求的调用方完全不必在意 HTTP 的具体内容，即使该 HTTP 请求信息发生变更，大多数情况也不需要修改调用发送请求的代码。<br />[![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680228238775-6e78a9b9-4b21-46fb-9623-4aebce296ce1.png#averageHue=%23f4f4f4&clientId=u3718c76a-6699-4&from=paste&height=713&id=u6363b0ba&originHeight=1783&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=438675&status=done&style=none&taskId=u2a5fd09a-18be-4706-9cba-ceba2e9099c&title=&width=1536)](https://forest.dtflyx.com/)
<a name="jUwu6"></a>
## 功能特性

- **声明式接口：** 通过定义地接口+注解的方式封装Http请求，实现了业务逻辑与Http协议之间的解耦
- **多种底层HTTP框架：** 以Httpclient和OkHttp为后端框架（可根据需要二选一）
- **不依赖中间件：** 因为针对第三方接口，所以不需要依赖Spring Cloud和任何注册中心
- **支持所有请求方法：** GET, HEAD, OPTIONS, TRACE, POST, DELETE, PUT, PATCH
- **约定大于配置：** 只要添加好依赖，基本上可以什么都不配置，就能马上运行
- **自动 JSON 转换：** 内置 Jackson、Fastjson、Gson 三种JSON转换器
- **自动 XML 转换：** 内置 JAXB 形式的 XML 转换器
- **自动 Protobuf 转换：** 内置 Protobuf 格式数据转换器
- **多种验签方式：** Basic Auth、 OAuth2、以及通过自定义拦截器或注解来验签
- **Spring/Spring boot：** 支持Spring和Springboot集成
- **上传下载：** 支持多种上传和下载的方式，并能监听数据传输进度
- **模板表达式：** 支持灵活的模板表达式，来配合声明式注解使用
- **拦截器：** 支持拦截器处理请求的各个生命周期
- **自定义注解：** 支持自定义注解，来极大增强扩展能力
- **回调函数：** 支持通过OnSuccess和OnError接口参数实现请求结果的回调
- **异步请求：** 支持方便的异步请求调用方式
- **编程式接口：** 除声明式接口外，也支持直观的编程式接口
<a name="HimKe"></a>
## 工作原理
Forest 会将定义好的接口通过动态代理的方式生成一个具体的实现类，然后组织、验证 HTTP 请求信息，绑定动态数据，转换数据形式，SSL 验证签名，调用后端 HTTP API(httpclient 等 API)执行实际请求，等待响应，失败重试，转换响应数据到 Java 类型等脏活累活都由这动态代理的实现类给包了。请求发送方调用这个接口时，实际上就是在调用这个干脏活累活的实现类。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1680228278647-b3be05bf-f7b8-4c5f-abfd-2c65c0fb95b5.png#averageHue=%232c494b&clientId=u3718c76a-6699-4&from=paste&height=441&id=u53423aab&originHeight=1102&originWidth=1737&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=97406&status=done&style=none&taskId=u8d1a1915-5558-4b41-a9d8-73e85587880&title=&width=694.8)
<a name="enkX7"></a>
## 快速开始
<a name="QteYX"></a>
### 添加Maven依赖
SpringBoot环境：
```xml
<dependency>  
  <groupId>com.dtflys.forest</groupId>  
  <artifactId>forest-spring-boot-starter</artifactId>  
  <version>1.5.25</version>  
</dependency>
```
Spring环境：
```xml
<dependency>  
  <groupId>com.dtflys.forest</groupId>  
  <artifactId>forest-core</artifactId>  
  <version>1.5.25</version>  
</dependency>  

<dependency>  
  <groupId>com.dtflys.forest</groupId>  
  <artifactId>forest-spring</artifactId>  
  <version>1.5.25</version>  
</dependency>
```
原生Java环境：
```xml
<dependency>  
  <groupId>com.dtflys.forest</groupId>  
  <artifactId>forest-core</artifactId>  
  <version>1.5.25</version>  
</dependency>
```
<a name="f9AJx"></a>
### 创建一个interface
以高德地图API为例：
```java
package com.yoursite.client;  

import com.dtflys.forest.annotation.Request;  
import com.dtflys.forest.annotation.DataParam;  

public interface AmapClient {  

    /**  
     * 聪明的你一定看出来了@Get注解代表该方法专做GET请求  
     * 在url中的{0}代表引用第一个参数，{1}引用第二个参数  
     */  
    @Get("http://ditu.amap.com/service/regeo?longitude={0}&latitude={1}")  
    Map getLocation(String longitude, String latitude);  
}
```
<a name="udJ56"></a>
### 扫描接口
在Spring Boot的配置类或者启动类上加上 `**@ForestScan**` 注解，并在`**basePackages**`属性里填上远程接口的所在的包名
```java
@SpringBootApplication  
@Configuration  
@ForestScan(basePackages = "com.yoursite.client")  
public class MyApplication {  
    public static void main(String[] args) {  
        SpringApplication.run(MyApplication.class, args);  
    }  
}
```
<a name="XMysj"></a>
### 调用接口
```java
// 注入接口实例  
@Autowired  
private AmapClient amapClient;  
...  

// 调用接口  
Map result = amapClient.getLocation("121.475078", "31.223577");  
System.out.println(result);
```
<a name="GCpHF"></a>
### 发送JSON数据
```java
/**  
 * 将对象参数解析为JSON字符串，并放在请求的Body进行传输  
 */  
@Post("/register")  
String registerUser(@JSONBody MyUser user);  

/**  
 * 将Map类型参数解析为JSON字符串，并放在请求的Body进行传输  
 */  
@Post("/test/json")  
String postJsonMap(@JSONBody Map mapObj);  

/**  
 * 直接传入一个JSON字符串，并放在请求的Body进行传输  
 */  
@Post("/test/json")  
String postJsonText(@JSONBody String jsonText);
```
<a name="gPkT6"></a>
### 发送XML数据
```java
/**  
 * 将一个通过JAXB注解修饰过的类型对象解析为XML字符串  
 * 并放在请求的Body进行传输  
 */  
@Post("/message")  
String sendXmlMessage(@XMLBody MyMessage message);  

/**  
 * 直接传入一个XML字符串，并放在请求的Body进行传输  
 */  
@Post("/test/xml")  
String postXmlBodyString(@XMLBody String xml);
```
<a name="pgNkX"></a>
### 发送Protobuf数据
```java
/**  
 * ProtobufProto.MyMessage 为 Protobuf 生成的数据类  
 * 将 Protobuf 生成的数据对象转换为 Protobuf 格式的字节流  
 * 并放在请求的Body进行传输  
 *   
 * 注: 需要引入 google protobuf 依赖  
 */  
@Post(url = "/message", contentType = "application/octet-stream")  
String sendProtobufMessage(@ProtobufBody ProtobufProto.MyMessage message);
```
<a name="CPVLs"></a>
### 文件上传
```java
/**  
 * 用@DataFile注解修饰要上传的参数对象  
 * OnProgress参数为监听上传进度的回调函数  
 */  
@Post("/upload")  
Map upload(@DataFile("file") String filePath, OnProgress onProgress);
```
可以用一个方法加Lambda同时解决文件上传和上传的进度监听
```java
Map result = myClient.upload("D:\\TestUpload\\xxx.jpg", progress -> {  
    System.out.println("progress: " + Math.round(progress.getRate() * 100) + "%");  // 已上传百分比  
    if (progress.isDone()) {   // 是否上传完成  
        System.out.println("--------   Upload Completed!   --------");  
    }  
});
```
<a name="Cjy6t"></a>
### 多文件批量上传
```java
/**  
 * 上传Map包装的文件列表，其中 {_key} 代表Map中每一次迭代中的键值  
 */  
@Post("/upload")  
ForestRequest<Map> uploadByteArrayMap(@DataFile(value = "file", fileName = "{_key}") Map<String, byte[]> byteArrayMap);  

/**  
 * 上传List包装的文件列表，其中 {_index} 代表每次迭代List的循环计数（从零开始计）  
 */  
@Post("/upload")  
ForestRequest<Map> uploadByteArrayList(@DataFile(value = "file", fileName = "test-img-{_index}.jpg") List<byte[]> byteArrayList);
```
<a name="x0Z6O"></a>
### 文件下载
下载文件也是同样的简单
```java
/**  
 * 在方法上加上@DownloadFile注解 
 * dir属性表示文件下载到哪个目录 
 * OnProgress参数为监听上传进度的回调函数  
 * {0}代表引用第一个参数  
 */  

@Get("http://localhost:8080/images/xxx.jpg")  
@DownloadFile(dir = "{0}")  
File downloadFile(String dir, OnProgress onProgress);
```
调用下载接口以及监听下载进度的代码如下：
```java
File file = myClient.downloadFile("D:\\TestDownload", progress -> {  

    System.out.println("progress: " + Math.round(progress.getRate() * 100) + "%"); // 已下载百分比   
    if (progress.isDone()) { // 是否下载完成  
        System.out.println("-------- Download Completed! --------");  

    }  
});
```
<a name="IwwHV"></a>
## 传送门
开源地址：[https://github.com/dromara/forest](https://github.com/dromara/forest)<br />官方主页：[https://forest.dtflyx.com/](https://forest.dtflyx.com/)
