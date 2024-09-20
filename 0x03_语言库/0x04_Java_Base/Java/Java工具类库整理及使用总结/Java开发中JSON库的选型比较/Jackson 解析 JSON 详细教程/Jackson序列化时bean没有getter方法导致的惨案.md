JavaJackson
<a name="iz0uE"></a>
## 1、需求
最近有一个需求，调用其他服务的REST接口，感觉很简单，于是迅速就搞起来了<br />构造Request类
```java
public class User {
    private String name;
    private Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }
}
```
上来就一new
```java
service.sendRequest(new User("niu", 18));
```
<a name="D3Wco"></a>
## 2、定位
但是，某天晚上8点，测试人员突然说调用失败，同时本身又缺少打印，没有办法具体哪出问题了。<br />经过网络抓包后发现，收到的参数都是null，但是这边明明调用构造器传入参数了<br />难道出现灵异事件了？<br />经过分析，整体数据流为：<br />![2022-07-14-22-13-27.685589100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1657808052575-9c6fc138-f376-4a47-b812-e71d590d2d50.png#clientId=u4b8055bc-930b-4&from=ui&id=u8f56dcca&originHeight=134&originWidth=636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=256302&status=done&style=shadow&taskId=u16b41e0f-7670-4a27-9898-2db56c11c6e&title=)<br />能出现问题的地方只能是**序列化JSON**地方，于是本地测试验证了这一结论：
```java
public static void main(String[] args) throws IOException {
    ObjectMapper objectMapper = new ObjectMapper();
    String request = objectMapper.writeValueAsString(new User("niu", 18));
    System.out.println(request);
}
```
虽然是出问题了，但是序列化并没有转为属性为null的对象，而是直接抛出异常
```java
Exception in thread "main" com.fasterxml.jackson.databind.exc.InvalidDefinitionException: No serializer found for class online.jvm.bean.User and no properties discovered to create BeanSerializer (to avoid exception, disable SerializationFeature.FAIL_ON_EMPTY_BEANS)
 at com.fasterxml.jackson.databind.exc.InvalidDefinitionException.from(InvalidDefinitionException.java:77)
```
通过查询异常资料，解决掉这种异常需要在增加Jackson的序列化配置FAIL_ON_EMPTY_BEANS，FAIL_ON_EMPTY_BEANS这个配置表示如果某个bean序列化为空时不会异常失败
```java
public static void main(String[] args) throws IOException {
    ObjectMapper objectMapper = new ObjectMapper();
    objectMapper.configure(FAIL_ON_EMPTY_BEANS, false);
    String request = objectMapper.writeValueAsString(new User("niu", 18));
    System.out.println(request);
}
```
这种就不会报错，而是返回序列化成空串，也就导致接受方为属性都为null<br />通过看自研RPC框架看到是有该FAIL_ON_EMPTY_BEANS的配置
<a name="zbBIO"></a>
## 3、解决
再来分析一下原因，Jackson序列化时需要调用bean的getter方法
<a name="ssCK2"></a>
### 1、写上getter后再看下结果：
```java
public class User {
    private String name;
    private Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public Integer getAge() {
        return age;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        // 输出正常 ： {"name":"niu","age":18}
    }
}
```
<a name="FK0ih"></a>
### 2、或者把属性访问权限改为`public`
```java
public class User {
    public String name;
    public Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        // 输出正常 ： {"name":"niu","age":18}
    }
}
```
但是如果要求**不能暴露bean的属性**即使是getter也不行呢？
<a name="n4aXH"></a>
### 3、注解 `@JsonProperty`
这是就需要使用Jackson提供的注解 `@JsonProperty`
```java
public class User {
    @JsonProperty("userName")
    private String name;
    @JsonProperty
    private Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        //   {"userName":"niu","age":18}
    }
}
```
来看下注解`@JsonProperty`的源码注释
> Marker annotation that can be used to define a non-static method as a "setter" or "getter" for a logical property (depending on its signature), or non-static object field to be used (serialized, deserialized) as a logical property.

大体意思是注解如果**用在属性上**相当于为该属性**定义getter和setter**。<br />那如果既有getter又有`@JsonProperty`注解，以哪个为准呢？
```java
public class User {
    @JsonProperty("userName")
    private String name;
    @JsonProperty
    private Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        // {"age":18,"userName":"niu"}
    }
}
```
如果getter一个没有的属性，效果如何呢？
```java
public class User {
    @JsonProperty("userName")
    private String name;
    @JsonProperty
    private Integer age;

    public User(String name, Integer age) {
        this.name = name;
        this.age = age;
    }

    public String getName2() {
        return name;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        // {"age":18,"name2":"niu","userName":"niu"}
    }
}
```
**这说明如果有**`**@JsonProperty**`**注解，先以注解为准**<br />然后**利用反射找到对象类的所有get方法**，接下来去get，然后小写化，作为json的每个key值，而get方法的返回值作为value。接下来**再反射field**，添加到json中。
<a name="K7Smt"></a>
### 4、特殊情况
还有一种比较特殊的情况，getter方法由lombok生成，且属性的次首字母是大写：
```java
@Getter
public class User {
    @JsonProperty
    private String nAme;
    @JsonProperty
    private Integer age;

    public User(String name, Integer age) {
        this.nAme = name;
        this.age = age;
    }

    public static void main(String[] args) throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        String request = objectMapper.writeValueAsString(new User("niu", 18));
        System.out.println(request);
        // {"nAme":"niu","age":18,"name":"niu"}
    }
}
```
这是因为lombok生成的getter会把属性的第一个字母变成大写，<br />序列化时会把get后与小写字母中间的大写变成小写，也就是会把NA变成小写<br />所以序列化结果会有name（getter获取）和nAme（注解获取）两个属性
```java
public String getNAme() {
	return this.nAme;
}
```
如果自己用idea快捷键生成getter，<br />此时之后序列化nAme
```java
public String getnAme() {
    return nAme;
}
```
<a name="oCuzU"></a>
## 4、小结
许多bug都是在自以为没有问题的地方产生，看似简单，更需要小心，同时也需要多注意序列化原理，整体感觉序列化还是用Gson更省心，完全不用关心Getter和Setter方法，会完全按照属性名来序列化。
