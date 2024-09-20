<a name="zgNdH"></a>
## 1、引入依赖
```xml
<dependency>
    <groupId>com.dtflys.forest</groupId>
    <artifactId>spring-boot-starter-forest</artifactId>
    <version>1.3.8</version>
</dependency>
```
<a name="QKaUe"></a>
## 2、配置后端 HTTP API-在SpringBoot配置文件application.yml
目前 Forest 支持`okhttp3`和`httpclient`两种后端 HTTP API，若不配置该属性，默认为`okhttp3`. 当然也可以改为`httpclient`
```yaml
forest:
  bean-id: config0 # 在spring上下文中bean的id, 默认值为forestConfiguration
  backend: okhttp3 # 后端HTTP API： okhttp3
  max-connections: 1000 # 连接池最大连接数，默认值为500
  max-route-connections: 500 # 每个路由的最大连接数，默认值为500
  timeout: 3000 # 请求超时时间，单位为毫秒, 默认值为3000
  connect-timeout: 3000 # 连接超时时间，单位为毫秒, 默认值为2000
  retry-count: 1 # 请求失败后重试次数，默认为0次不重试
  ssl-protocol: SSLv3 # 单向验证的HTTPS的默认SSL协议，默认为SSLv3
  logEnabled: true # 打开或关闭日志，默认为true
```
> 注意： 这里retry-count只是简单机械的请求失败后的重试次数，所以一般建议设置为0。 如果一定要多次重试，请一定要在保证服务端的幂等性的基础上进行重试，否则容易引发生产事故！

<a name="J1Cft"></a>
## 3、在SpringBoot中开启Forest的包扫描进行动态代理
在SpringBoot启动类添加注解  `@ForestScan(basePackages = "com.fcant.service.forest")`` `
```java
@SpringBootApplication
@ForestScan(basePackages = "com.example.demo.forest")
public class DemoApplication {
    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }
}
```
<a name="TNQHM"></a>
## 4、请求接口构建-方法上面使用 `@Request` 注解定制请求
 `@Request` 默认使用`GET`方式，且将请求响应的数据以`String`的方式返回给调用者。
```java
public interface MyClient {
    @Request(url = "http://localhost:5000/hello")
    String simpleRequest();
}
```
<a name="JrmtU"></a>
### `@Request` 注解的参数
| **参数** | **类型** | **说明** |
| --- | --- | --- |
| url | String | 请求的主机地址和端口，或者域名映射 |
| headers | 字符串数组 |  |
| type | String | 请求类型，默认get，大小写不敏感 |
| data |  | 请求的数据体参数 |
| contentType | String | 请求头数据类型 |
| dataType | String/JSON | 接受的数据类型 |

<a name="J3hyI"></a>
#### type：支持的请求类型
通过`@Request`注解的`type`参数指定 HTTP 请求的方式。<br />除了`GET`和`POST`，也可以指定成其他几种 HTTP 请求方式(`PUT`, `HEAD`, `OPTIONS`, `DELETE`)。<br />其中`type`属性的大小写不敏感，写成`POST`和`post`效果相同。
```java
public interface MyClient {

    // GET请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "get"
    )
    String simpleGet();

    // POST请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "post"
    )
    String simplePost();

    // PUT请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "put"
    )
    String simplePut();

    // HEAD请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "head"
    )
    String simpleHead();

    // Options请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "options"
    )
    String simpleOptions();

    // Delete请求
    @Request(
            url = "http://localhost:5000/hello",
            type = "delete"
    )
    String simpleDelete();
}
```
<a name="tEnFW"></a>
#### header：定义请求头
`headers`属性接受的是一个字符串数组，在接受多个请求头信息时以以下形式填入请求头，其中组数每一项都是一个字符串，每个字符串代表一个请求头。请求头的名称和值用`:`分割。
```java
{
    "请求头名称1: 请求头值1",
    "请求头名称2: 请求头值2",
    "请求头名称3: 请求头值3",
    ...
 }
```
使用示例
```java
public interface MyClient {
    @Request(
            url = "http://localhost:5000/hello/user",
            headers = {
                "Accept-Charset: utf-8",
                "Content-Type: text/plain"
            }
    )
    String multipleHeaders();
}
```
实际产生的 HTTP 请求如下
```http
GET http://localhost:5000/hello/user
HEADER:
    Accept-Charset: utf-8
    Content-Type: text/plain
```
<a name="nyCXd"></a>
#### data：请求体数据
通过`@Request`注解的`data`属性把数据添加到请求体。需要注意的是只有当`type`为`POST`、`PUT`、`PATCH`这类 HTTP Method 时，`data`属性中的值才会绑定到请求体中，而`GET`请求在有些情况会绑定到`url`的参数中。<br />具体`type`属性和`data`属性数据绑定位置的具体关系如下表：

| **type** | **data属性数据绑定位置** | **支持的contentType或Content-Type请求头** |
| --- | --- | --- |
| GET | url参数部分 | 只有application/x-www-form-urlencoded |
| POST | 请求体 | 任何contentType |
| PUT | 请求体 | 任何contentType |
| PATCH | 请求体 | 任何contentType |
| HEAD | url参数部分 | 只有application/x-www-form-urlencoded |
| OPTIONS | url参数部分 | 只有application/x-www-form-urlencoded |
| DELETE | url参数部分 | 只有application/x-www-form-urlencoded |
| TRACE | url参数部分 | 只有application/x-www-form-urlencoded |

 `data` 属性中进行数据绑定
```java
public interface MyClient {

    @Request(
            url = "http://localhost:5000/hello/user",
            type = "post",
            data = "username=${0}&password=${1}",
            headers = {"Accept:text/plan"}
    )
    String dataPost(String username, String password);
}
```
<a name="mAZ7p"></a>
#### dataType：接受的数据类型
Forest请求会自动将响应的返回数据反序列化成需要的数据类型。想要接受指定类型的数据需要完成两步操作：<br />第一步：定义`dataType`属性<br />`dataType`属性指定了该请求响应返回的数据类型，目前可选的数据类型有三种: `text`, `json`, `xml`<br />Forest会根据指定的`dataType`属性选择不同的反序列化方式。其中`dataType`的默认值为`text`，如果不指定其他数据类型，那么Forest就不会做任何形式的序列化，并以文本字符串的形式返回。
```java
/**
 * dataType为text或不填时，请求响应的数据将以文本字符串的形式返回回来
 */
@Request(
    url = "http://localhost:8080/text/data",
    dataType = "text"
)
String getData();
```
若指定为`json`或`xml`，那就告诉了Forest该请求的响应数据类型为JSON或XML形式的数据，就会以相应的形式进行反序列化。
```java
/**
 * dataType为json或xml时，Forest会进行相应的反序列化
 */
@Request(
    url = "http://localhost:8080/text/data",
    dataType = "json"
)
Map getData();
```
> 注意： Forest需要指明返回类型（如User）的同时，也需要指明数据类型dataType为json或其他类型。

<a name="JeJQe"></a>
#### 回调函数
在Forest中的回调函数使用单方法的接口定义，这样可以在 `Java 8` 或 `Kotlin` 语言中方便使用 `Lambda` 表达式。<br />使用的时候只需在接口方法加入`OnSuccess<T>`类型或`OnError`类型的参数：
```java
@Request(
        url = "http://localhost:5000/hello/user",
        headers = {"Accept:text/plan"},
        data = "username=${username}"
)
String send(@DataVariable("username") String username, OnSuccess<String> onSuccess, OnError onError);
```
`OnSuccess<T>`在请求成功调用响应时会被调用，而`OnError`在失败或出现错误的时候被调用。<br />其中`OnSuccess<T>`的泛型参数`T`定义为请求响应返回结果的数据类型。
```java
myClient.send("foo", (String resText, ForestRequest request, ForestResponse response) -> {
        // 成功响应回调
        System.out.println(resText);    
    },
    (ForestRuntimeException ex, ForestRequest request, ForestResponse response) -> {
        // 异常回调
        System.out.println(ex.getMessage());
    });
```
> 提示： 在异步请求中只能通`OnSuccess<T>`回调函数接或`Future`返回值接受数据。 而在同步请求中，`OnSuccess<T>`回调函数和任何类型的返回值都能接受到请求响应的数据。 `OnError`回调函数可以用于异常处理，一般在同步请求中使用`try-catch`也能达到同样的效果。

<a name="Iz0MK"></a>
#### 异步请求
在Forest使用异步请求，可以通过设置`@Request`注解的`async`属性为`true`实现，不设置或设置为`false`即为同步请求。
```java
@Request(
        url = "http://localhost:5000/hello/user?username=${0}",
        async = true,
        headers = {"Accept:text/plan"}
)
void asyncGet(String username， OnSuccess<String> onSuccess);
```
一般情况下，异步请求都通过`OnSuccess<T>`回调函数来接受响应返回的数据，而不是通过接口方法的返回值，所以这里的返回值类型一般会定义为`void`。
```java
// 异步执行
myClient.asyncGet("foo", (result, request, response) -> {
    // 处理响应结果
    System.out.println(result);
});
```
不习惯函数式的编程方式，也可以用`Future<T>`类型定义方法返回值的方式来接受响应数据。
```java
@Request(
        url = "http://localhost:5000/hello/user?username=foo",
        async = true,
        headers = {"Accept:text/plan"}
)
Future<String> asyncFuture();
```
获取Future的结果
```java
// 异步执行
Future<String> future = myClient.asyncFuture();
// 获取数据
String result = future.get();
```
<a name="wHx5a"></a>
## 5、数据绑定
通过数据绑定参数化定制请求
<a name="AmXoR"></a>
### A.参数序号绑定
可以使用`${数字}`的方式引用对应顺序的参数，其中`${...}`是模板表达式的语法形式。<br />序号所对应的参数在接口方法调用时传入的值，会被自动绑定到`${数字}`所在的位置。<br />`注`：参数序号从`0`开始计数。<br />比如`${0}`表示的就是第一个参数，`${1}`表示的第二个参数，以此类推。
```java
@Request(
    url = "${0}/send?un=${1}&pw=${2}&da=${3}&sm=${4}",
    type = "get",
    dataType = "json"
)
public Map send(
    String base,
    String userName,
    String password,
    String phoneList,
    String content
);
```
方法调用传递参数
```java
myClient.send("http://localhost:8080", "DT", "123456", "123888888", "Hahaha");
```
效果
```http
GET http://localhost:8080/send?un=DT&pw=123456&da=123888888&sm=Hahaha
```
<a name="Xpj4l"></a>
### B.通过注解 `@DataParam` 进行参数绑定
在接口方法的参数前加上`@DataParam`注解并在`value`属性中给予一个名词，就能实现参数绑定。
<a name="qlgL9"></a>
#### ①参数绑定的位置
被`@DataParam`注解修饰的参数数据的绑定位置较为灵活多变，它可以出现在请求`url`的参数部分，也可以出现在请求 Body 中。具体出现位置取决于由`type`属性定义的 HTTP Method，其绑定的具体位置如下表：

| **type** | **@ParaParam注解绑定位置** |
| --- | --- |
| GET | url参数部分 |
| POST | 请求体 |
| PUT | 请求体 |
| PATCH | 请求体 |
| HEAD | url参数部分 |
| OPTIONS | url参数部分 |
| DELETE | url参数部分 |
| TRACE | url参数部分 |

`GET`、`HEAD`等方法会将参数值直接绑定到 URL 的参数中。
```java
@Request(
    url = "${0}/send",
    type = "get",
    dataType = "json"
)
public Map send(
    String base,
    @DataParam("un") String userName,
    @DataParam("pw") String password,
    @DataParam("da") String phoneList,
    @DataParam("sm") String content
);
```
方法调用
```java
myClient.send("http://localhost:8080", "DT", "123456", "123888888", "Hahaha");
```
效果
```http
GET http://localhost:8080/send?un=DT&pw=123456&da=123888888&sm=Hahaha
```
`POST`、`PUT`等方法会将参数值绑定到请求体中，同时在请求中的数据在`默认`情况下按`x-www-form-urlencoded`格式绑定。
```java
@Request(
        url = "http://localhost:5000/hello",
        type = "post",
        headers = {"Accept:text/plan"}
)
String send(@DataParam("username") String username, @DataParam("password") String password);
```
方法调用
```java
myClient.send("foo", "bar");
```
效果
```http
POST http://localhost:5000/hello
HEADER:
    Accept: text/plan
BODY:
    username=foo&password=bar
```
<a name="UGiyA"></a>
#### ②数据绑定格式
若想参数值在请求中传输`JSON`或`XML`等格式绑定，可以通过修改`contentType`或`Content-Type`请求头来实现对应的数据格式， 具体数据格式和`contentType`属性的关系请参考下表：

| **contentType / Content-Type** | **数据格式** |
| --- | --- |
| 不设置 | x-www-form-urlencoded表单格式 |
| application/x-www-form-urlencoded | x-www-form-urlencoded表单格式 |
| application/json | JSON格式 |
| application/xml | XML格式 |

若传输的 Body 数据是标准表单格式，就不用设置`cotentType`或请求头`Content-Type`了
```java
public interface MyClient {
    @Request(
            url = "http://localhost:5000/hello/user",
            type = "post",
            headers = {"Accept:text/plan"}
    )
    String postBody(@DataParam("username") String username, @DataParam("password") String password);
}
```
如果调用方代码如下所示：
```java
myClient.postBody("foo", "bar");
```
实际产生的 HTTP 请求如下：
```http
POST http://localhost:5000/hello/user
HEADER:
    Accept: text/plan
BODY:
    username=foo&password=bar
```
若要将 Body 中的数据内容转换成 JSON 格式，只要设置`contentType`属性为`application/json`即可
```java
public interface MyClient {
    @Request(
            url = "http://localhost:5000/hello/user",
            type = "post",
            contentType = "application/json"
    )
    String postBody(@DataParam("username") String username, @DataParam("password") String password);
}
```
如果调用方代码如下所示：
```java
myClient.postBody("foo", "bar");
```
实际产生的 HTTP 请求如下：
```http
POST http://localhost:5000/hello/user
HEADER:
    Content-Type: application/json
BODY:
    {"username":"foo","password":"bar"}
```
或者不用`contentType`属性，使用请求头`Content-Type: application/json`效果相同
```java
public interface MyClient {
    @Request(
            url = "http://localhost:5000/hello/user",
            type = "post",
            headers = {"Content-Type: application/json"}
    )
    String postBody(@DataParam("username") String username, @DataParam("password") String password);
}
```
<a name="nXS3N"></a>
### C.`@DataVariable` 参数绑定
在接口方法中定义的参数前加上`@DataVariable`注解并`value`中输入一个名称，便可以实现参数的`变量名`绑定。<br />`@DataVariable`注解的`value`的值便是该参数在 Forest 请求中对应的`变量名`。<br />意思就是在`@Request`的多个不同属性（`url`, `headers`, `data`）中通过`${变量名}`的模板表达式的语法形式引用之前在`@DataVariable`注解上定义的`变量名`，实际引用到的值就是调用该方法时传入该参数的实际值。<br />`@DataVariable`注解修饰的参数数据可以出现在请求的任意部分（`url`, `header`, `data`），具体在哪个部分取决于在哪里引用它。如果没有任何地方引用该变量名，该变量的值就不会出现在任何地方。
```java
@Request(
    url = "${base}/send?un=${un}&pw=${pw}&da=${da}&sm=${sm}",
    type = "get",
    dataType = "json"
)
public Map send(
    @DataVariable("base") String base,
    @DataVariable("un") String userName,
    @DataVariable("pw") String password,
    @DataVariable("da") String phoneList,
    @DataVariable("sm") String content
);
```
如果调用方代码如下所示：
```java
myClient.send("http://localhost:8080", "DT", "123456", "123888888", "Hahaha");
```
实际产生的 HTTP 请求如下：
```http
GET http://localhost:8080/send?un=DT&pw=123456&da=123888888&sm=Hahaha
```
<a name="tfMLL"></a>
### D.`@DataObject` 对象绑定
上面的提到的`@DataParam`绑定，以及在`data`属性中的绑定可以方便解决大部分数据传输要求。 但缺点是参数要一个一个写，如果有很多参数或属性会写的十分复杂，而且请求要传输一个十分复杂的对象就会变得非常无能为力。<br />所以为了解决这个问题，Forest引入了`@DataObject`注解针对对象进行数据绑定。<br />请看下面的例子：
```java
@Request(url = "http://localhost:5000/hello/user")
String send(@DataObject User user);
```
如果调用请求接口`send`方法，并传入一个`User`类的对象，会得到以下结果：
```http
GET http://localhost:5000/hello/user?username=foo&password=bar
```
如上所示，`@DataObject`注解会将传入的对象拆成一个一个属性键值对放入`url`的`query`参数部分。用这种方式，即使有十几二十个参数，接口的参数也只需传一个对象即可。<br />`@DataObject`绑定的数据不仅可以出现在`url`参数上，还可以出现在请求体中，不仅可以产生`query`参数的数据格式,也可以变成`json`格式或是`xml`格式。<br />具体数据绑定的出现位置请看下表：

| **type** | **数据绑定的位置** |
| --- | --- |
| GET | url参数部分 |
| POST | 请求体 |
| PUT | 请求体 |
| PATCH | 请求体 |
| HEAD | url参数部分 |
| OPTIONS | url参数部分 |
| DELETE | url参数部分 |
| TRACE | url参数部分 |

<a name="ZKpp9"></a>
#### ①绑定到请求体
绑定到请求体：
```java
@Request(
    url = "http://localhost:5000/hello/user",
    type = "post"
)
String send(@DataObject User user);
```
调用后产生的结果如下：
```http
POST http://localhost:5000/hello/user
BODY:
    username=foo&password=bar
```
如上所示，`User`对象的数据出现在了Body中。这里出现的是`x-www-form-urlencoded`表单数据格式，是在没指定内容格式情况下的默认格式。<br />若要以其它形式(比如`JSON`)传输数据，需要做一些改动。
<a name="BxukN"></a>
#### ②绑定为JSON格式
要让`@DataObject`绑定的对象转换成`JSON`格式也非常简单，只要将`contentType`属性或`Content-Type`请求头指定为`application/json`便可。
```java
@Request(
    url = "http://localhost:5000/hello/user",
    contentType = "application/json", 
    type = "post"
)
String send(@DataObject User user);
```
调用后产生的结果如下：
```http
POST http://localhost:5000/hello/user
HEADER:
    Content-Type: application/json
BODY:
    {"username": "foo", "password": "bar"}
```
<a name="oJGsy"></a>
#### ③绑定为XML格式
`@DataObject`较为特殊，除了指定`contentType`属性或`Content-Type`请求头为`application/xml`外，还需要设置`@DataObject`的`filter`属性为`xml`。
```java
@Request(
    url = "http://localhost:5000/hello/user",
    contentType = "application/xml", 
    type = "post"
)
String send(@DataObject(filter = "xml") User user);
```
此外，这里的`User`对象也要绑定`JAXB`注解：
```java
@XmlRootElement(name = "misc")
public User {
    private String usrname;
    private String password;
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
}
```
调用传入`User`对象后的结果如下：
```http
POST http://localhost:5000/hello/user
HEADER:
    Content-Type: application/xml
BODY:
    <misc><username>foo</username><password>bar</password></misc>
```
<a name="EmrMN"></a>
### E.全局变量绑定
若已经在SpringBoot的application.yml或者application.propertise文件中定义好全局变量，那便可以直接在请求定义中绑定全局变量了。<br />若有全局变量：
```yaml
basetUrl: http://localhost:5050
usrename: foo
userpwd: bar
phoneList: 123888888
```
```java
@Request(
        url = "${basetUrl}/send?un=${usrename}&pw=${userpwd}&da=${phoneList}&sm=${sm}",
        type = "get",
        dataType = "json"
)
Map testVar(@DataVariable("sm") String content);
```
如果调用方代码如下所示：
```java
myClient.send("Xxxxxx");
```
实际产生的 HTTP 请求如下：
```http
GET http://localhost:5050/send?un=foo&pw=bar&da=123888888&sm=Xxxxxx
```

