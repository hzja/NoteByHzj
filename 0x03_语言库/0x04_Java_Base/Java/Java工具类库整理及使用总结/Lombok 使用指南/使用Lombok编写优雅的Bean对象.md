JavaLombok<br />使用java编写代码，十之八九都是在写java类，从而构建java对象。lombok之前也说了不少，但使用了这么多年，感觉还是有很多技巧可以使用的。<br />毫无疑问，使用lombok，编写的java代码很优雅，而使用起来和普通的java编码方式创建的类毫无二致。不过，这样就满足了吗？实际上lombok很多注解，让这个java类在使用的时候，也可以更优雅。<br />**本文就从ORM实体类、**`**Builder**`**模式工具类、**`**Wither**`**工具类以及**`**Accessors**`**工具类几个层面对比一下。**<br />首先说明，不同的方式本质上没有优劣之分，不过在不同的应用场景就会变得很奇妙了。
<a name="atsmP"></a>
## ORM实体类
当一个java Bean类作为ORM实体类，或者xml、json的映射类时，需要这个类有这几个特征：

- **拥有无参构造器**
- **拥有setter方法，用以反序列化；**
- **拥有getter方法，用以序列化。**

那么最简单的情况就是：
```java
@Data  
public class UserBean{  
  private Integer id;  
  private String userName;  
}
```
复习一下，Data 注解相当于装配了 `@Getter` `@Setter` `@RequiredArgsConstructor` `@ToString` `@EqualsAndHashCode`<br />那么，作为实体类、或者序列化的Bean类，足够用了。
<a name="StWP9"></a>
## Builder
构造器模式，在很多工具类中频繁的使用。
```java
import lombok.Builder;  
  
@Builder  
public class UserBean {  
      private Integer id;  
      private String userName;  
}
```
它做了什么事？

- **它创建了一个private 的全参构造器。也就意味着 无参构造器没有; 同时也意味着这个类不可以直接构造对象。**
- **它为每一个属性创建了一个同名的方法用于赋值，代替了setter，而该方法的返回值为对象本身。**

使用一下：
```java
UserBean u=UserBean.builder()  
    .id(1001)  
    .userName("polly")  
    .build();  
System.out.println(u);
```
还不错，然并卵，由于这个Bean并没有getter方法，里边的数据没办法直接使用。光说没用，继续执行会发现输出是这个东西：com.fcant.builder.UserBean@20322d26，连看都看不出是什么东东。因此，Builder提供了一个种可能性，实际使用还需要更多的东西。<br />那么，为了测试方便需要添加 `@ToString()` 注解，就会输出 `UserBean(id=1001, userName=polly)`<br />换一个思路，可能想不添加`ToString`注解，把他转成json输出：
```java
UserBean u=UserBean.builder()  
    .id(1001)  
    .userName("polly")  
    .build();  
ObjectMapper mapper=new ObjectMapper();  
System.out.println(mapper.writeValueAsString(u));
```
很不幸，会收到下面的异常：
> com.fasterxml.jackson.databind.exc.InvalidDefinitionException: No serializer found for class com.fcant.builder.UserBean and no properties discovered to create BeanSerializer (to avoid exception, disable SerializationFeature.FAIL_ON_EMPTY_BEANS)  

看到 no properties discovered 了吧，没错，工具类无法找到属性，因为没有 getter，那么加上 `@Getter` 就可以了。
```java
import lombok.Builder;  
import lombok.Getter;  
  
@Builder  
@Getter  
public class UserBean {  
      private Integer id;  
      private String userName;  
}
```
序列化为json可以了，那么从一个json反序列化为对象呢？
```java
@Builder  
@Getter  
@Setter  
public class UserBean {  
      private Integer id;  
      private String userName;  
}
```
还是不行，如无意外，会遇到
> com.fasterxml.jackson.databind.exc.InvalidDefinitionException: Cannot construct instance ofcom.fcant.builder.UserBean(no Creators, like default construct, exist): cannot deserialize from Object value (no delegate- or property-based Creator)

前面已经交代了，光增加 `@Setter` 还不够，他还需要一个无参构造器。那么，下面可以吗？
```java
import lombok.Builder;  
import lombok.Data;  
  
@Builder  
@Data  
public class UserBean {  
      private Integer id;  
      private String userName;  
}
```
同样不行，因为虽然 Data使用的时候可以直接使用无参构造器，但由于 Builder 引入了全参构造器，那么按照java原生的规则，无参构造器就没有了。那么就再加一个无参构造器
```java
@Builder  
@Data  
@NoArgsConstructor
```
很不幸，Builder又报错了，它找不到全参构造器了。好吧，最终的效果如下：
```java
@Builder  
@Data  
@AllArgsConstructor(access = AccessLevel.PRIVATE)  
@NoArgsConstructor
```
注意全参构造器的访问级别，不要破坏Builder的规则。<br />更进一步，看如下示例：
```java
import java.util.List;  
  
import lombok.AccessLevel;  
import lombok.AllArgsConstructor;  
import lombok.Builder;  
import lombok.Data;  
import lombok.NoArgsConstructor;  
  
@Builder  
@Data  
@AllArgsConstructor(access = AccessLevel.PRIVATE)  
@NoArgsConstructor  
public class UserBean {  
      private Integer id;  
      private String userName;  
      private List<String> addresses;  
}
```
思考一下，这个List 也需要在外面new 一个 ArrayList，然后build进去，使用起来并不舒服。lombok提供了另一个注解配合使用，那就是 `@Singular`，如下：
```java
import java.util.List;  
  
import lombok.AccessLevel;  
import lombok.AllArgsConstructor;  
import lombok.Builder;  
import lombok.Data;  
import lombok.NoArgsConstructor;  
  
@Builder  
@Data  
@AllArgsConstructor(access = AccessLevel.PRIVATE)  
@NoArgsConstructor  
public class UserBean {  
      private Integer id;  
      private String userName;  
      @Singular  
      private List<String> favorites;  
}
```
那么就可以这样操作这个列表了。
```java
UserBean u = UserBean.builder()  
    .id(1001)  
    .userName("polly")  
    .favorite("music")  
    .favorite("movie")  
    .build();
```
是不是很方便？同时还提供了一个 `clearXXX`方法，清空集合。<br />还有一个小坑，如果增加一个example属性，然后给它一个默认值：
```java
@Builder  
@Data  
@AllArgsConstructor(access = AccessLevel.PRIVATE)  
@NoArgsConstructor  
public class UserBean {  
      private Integer id;  
      private String userName;  
      private String example="123456";  
}
```
测试一下看：
```java
UserBean u = UserBean.builder()  
    .id(1001)  
    .userName("polly")  
    .build();  
System.out.println(u.toString());  
```
输出结果：`UserBean(id=1001, userName=polly, example=null)`<br />咦，不对呀？缺省值呢？？这要从Builder的原理来解释，他实际上是分别设置了一套属性列表的值，然后使用全参构造器创建对象。那么，默认值在Bean上，不在Builder上，那么Builder没赋值，它的值就是null，最后把所有属性都复制给UserBean，从而null覆盖了默认值。<br />如何让Builder实体来有默认值呢？只需要给该字段增加 `@Default` 注解级可。
```java
import lombok.AccessLevel;  
import lombok.AllArgsConstructor;  
import lombok.Builder;  
import lombok.Builder.Default;  
import lombok.Data;  
import lombok.NoArgsConstructor;  
  
@Builder  
@Data  
@AllArgsConstructor(access = AccessLevel.PRIVATE)  
@NoArgsConstructor  
public class UserBean {  
      private Integer id;  
      private String userName;  
      @Default  
      private String example="123456";  
}
```
重新执行测试用例，输出：`UserBean(id=1001, userName=polly, example=123456)`，OK，没毛病了。
<a name="MKJkL"></a>
## Wither
用wither方式构建对象，这在Objective-C 中比较多见。<br />适用的场景是，使用几个必要的参数构建对象，其他参数，动态的拼装。举个例子，构建一个`ApiClient`，它的用户名和密码是必须的，他的ApiService的地址有一个默认值，然后可以自己定制这个地址。
```java
import lombok.AllArgsConstructor;  
import lombok.experimental.Wither;  
  
@Wither  
@AllArgsConstructor //WITHER NEED IT.  
public class ApiClient {  
    private String appId;  
    private String appKey;  
    private String endpoint="http://api.pollyduan.com/myservice";  
}
```
如何使用呢？
```java
@Test  
public void test1() {  
    ApiClient client1=new ApiClient(null, null,null);  
    System.out.println(client1);  
  
    Object client2 = client1.withAppId("10001")  
        .withAppKey("abcdefg")  
        .withEndpoint("http://127.0.0.1/");  
    System.out.println(client2);  
}
```
默认的使用null去初始化一个对象还是很奇怪的。和 `Builder`一样，`Wither`也是提供了可能性，实际使用还需要调整一下。<br />可以设置一个必选参数的构造器，如下：
```java
import lombok.AllArgsConstructor;  
import lombok.NonNull;  
import lombok.RequiredArgsConstructor;  
import lombok.experimental.Wither;  
  
@RequiredArgsConstructor  
@Wither  
@AllArgsConstructor  
public class ApiClient {  
    @NonNull  
    private String appId;  
    @NonNull  
    private String appKey;  
    private String endpoint="http://api.pollyduan.com/myservice";  
}
```
这样使用时就可以这样：
```java
@Test  
public void test1() {  
    ApiClient client1=new ApiClient("10001", "abcdefgh");  
    System.out.println(client1);  

    Object client2 = client1.withEndpoint("http://127.0.0.1/");  
    System.out.println(client2);  
}
```
是不是优雅了很多？而且实际上使用时也使用链式语法：
```java
ApiClient client1=new ApiClient("10001", "abcdefgh")  
    withEndpoint("http://127.0.0.1/");
```
另外还有一个小细节，前面的示例输出如下：
```java
com.fcant.wither.ApiClient@782830e  
com.fcant.wither.ApiClient@470e2030  
```
这个日志表明，`with()` 返回的对象并不是原来的对象，而是一个新对象，这很重要。
<a name="QIjTH"></a>
## Accessors
访问器模式，是给一个普通的Bean增加一个便捷的访问器，包括读和写。<br />它有两种工作模式，fluent和chain，举例说明：
```java
import lombok.Data;  
import lombok.experimental.Accessors;  
  
@Accessors(fluent = true)  
@Data  
public class UserBean {  
    private Integer id;  
    private String userName;  
    private String password;  
  
}
```
使用代码：
```java
UserBean u=new UserBean()  
    .id(10001)  
    .userName("polly")  
    .password("123456");  
  
u.userName("Tom");  
System.out.println(u.userName());  
```
这和 Builder 类似，但更小巧，而且不影响属性的读写，只不过使用属性同名字符串代替了getter和setter。<br />另一个模式是 chain：
```java
UserBean u=new UserBean()  
    .setId(10001)  
    .setUserName("polly")  
    .setPassword("123456");  

u.setUserName("Tom");  
System.out.println(u.getUserName());  
```
可以看得出来，这fluent的区别就在于使用getter和setter。
