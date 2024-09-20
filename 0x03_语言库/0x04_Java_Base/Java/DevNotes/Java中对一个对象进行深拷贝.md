Java
<a name="BW3jZ"></a>
## 介绍
在Java语言里，当需要拷贝一个对象时，有两种类型的拷贝：浅拷贝与深拷贝。浅拷贝只是拷贝了源对象的地址，所以源对象的值发生变化时，拷贝对象的值也会发生变化。而深拷贝则是拷贝了源对象的所有值，所以即使源对象的值发生变化时，拷贝对象的值也不会改变。如下图描述：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639822997143-b74d02e7-7721-4c94-a8ae-8bad229fdb57.webp#clientId=u10f523df-37fd-4&from=paste&id=u84994320&originHeight=655&originWidth=881&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9ad76545-548f-49d2-b272-d7c1618302a&title=)<br />了解了浅拷贝和深拷贝的区别之后，了解几种深拷贝的方法。
<a name="ioNvh"></a>
## 拷贝对象
首先，定义一下需要拷贝的简单对象。
```java
/**  
 * 用户  
 */  
public class User {  
  
    private String name;  
    private Address address;  
  
    // constructors, getters and setters  
  
}  
  
/**  
 * 地址  
 */  
public class Address {  
  
    private String city;  
    private String country;  
  
    // constructors, getters and setters  
  
}  
```
如上述代码，定义了一个User用户类，包含name姓名，和address地址，其中address并不是字符串，而是另一个Address类，包含country国家和city城市。构造方法和成员变量的`get()`、`set()`方法此处省略不写。接下来将详细描述如何深拷贝User对象。
<a name="n8QH1"></a>
## 方法一 构造函数
可以通过在调用构造函数进行深拷贝，形参如果是基本类型和字符串则直接赋值，如果是对象则重新`new`一个。
<a name="yVJTl"></a>
### 测试用例
```java
@Test  
public void constructorCopy() {  
  
    Address address = new Address("杭州", "中国");  
    User user = new User("大山", address);  
  
    // 调用构造函数时进行深拷贝  
    User copyUser = new User(user.getName(), new Address(address.getCity(), address.getCountry()));  
  
    // 修改源对象的值  
    user.getAddress().setCity("深圳");  
  
    // 检查两个对象的值不同  
    assertNotSame(user.getAddress().getCity(), copyUser.getAddress().getCity());  
  
}  
```
<a name="vEQVA"></a>
## 方法二 重载`clone()`方法
Object父类有个`clone()`的拷贝方法，不过它是`protected`类型的，需要重写它并修改为`public`类型。除此之外，子类还需要实现`Cloneable`接口来告诉JVM这个类是可以拷贝的。
<a name="Qs2ar"></a>
### 重写代码
修改一下User类，Address类，实现`Cloneable`接口，使其支持深拷贝。
```java
/**  
 * 地址  
 */  
public class Address implements Cloneable {  
  
    private String city;  
    private String country;  
  
    // constructors, getters and setters  
  
    @Override  
    public Address clone() throws CloneNotSupportedException {  
        return (Address) super.clone();  
    }  
  
}  

/**  
 * 用户  
 */  
public class User implements Cloneable {  
  
    private String name;  
    private Address address;  
  
    // constructors, getters and setters  
  
    @Override  
    public User clone() throws CloneNotSupportedException {  
        User user = (User) super.clone();  
        user.setAddress(this.address.clone());  
        return user;  
    }  
  
}  
```
需要注意的是，`super.clone()`其实是浅拷贝，所以在重写User类的`clone()`方法时，address对象需要调用`address.clone()`重新赋值。
<a name="kHNgj"></a>
### 测试用例
```java
@Test  
public void cloneCopy() throws CloneNotSupportedException {  
  
    Address address = new Address("杭州", "中国");  
    User user = new User("大山", address);  
  
    // 调用clone()方法进行深拷贝  
    User copyUser = user.clone();  
  
    // 修改源对象的值  
    user.getAddress().setCity("深圳");  
  
    // 检查两个对象的值不同  
    assertNotSame(user.getAddress().getCity(), copyUser.getAddress().getCity());  
  
}  
```
<a name="rXNoy"></a>
## 方法三 Apache Commons Lang序列化
Java提供了序列化的能力，可以先将源对象进行序列化，再反序列化生成拷贝对象。但是，使用序列化的前提是拷贝的类（包括其成员变量）需要实现`Serializable`接口。Apache Commons Lang包对Java序列化进行了封装，可以直接使用它。
<a name="o5u2u"></a>
### 重写代码
修改一下User类，Address类，实现`Serializable`接口，使其支持序列化。
```java
/**  
 * 地址  
 */  
public class Address implements Serializable {  
  
    private String city;  
    private String country;  
  
    // constructors, getters and setters  
  
}  

/**  
 * 用户  
 */  
public class User implements Serializable {  
  
    private String name;  
    private Address address;  
  
    // constructors, getters and setters  
  
}  
```
<a name="H2xSu"></a>
### 测试用例
```java
@Test  
public void serializableCopy() {  
  
    Address address = new Address("杭州", "中国");  
    User user = new User("大山", address);  
  
    // 使用Apache Commons Lang序列化进行深拷贝  
    User copyUser = (User) SerializationUtils.clone(user);  
  
    // 修改源对象的值  
    user.getAddress().setCity("深圳");  
  
    // 检查两个对象的值不同  
    assertNotSame(user.getAddress().getCity(), copyUser.getAddress().getCity());  
  
}  
```
<a name="L1kiX"></a>
## 方法四 Gson序列化
Gson可以将对象序列化成JSON，也可以将JSON反序列化成对象，所以可以用它进行深拷贝。
<a name="oUOeZ"></a>
### 测试用例
```java
@Test  
public void gsonCopy() {  
  
    Address address = new Address("杭州", "中国");  
    User user = new User("大山", address);  
  
    // 使用Gson序列化进行深拷贝  
    Gson gson = new Gson();  
    User copyUser = gson.fromJson(gson.toJson(user), User.class);  
  
    // 修改源对象的值  
    user.getAddress().setCity("深圳");  
  
    // 检查两个对象的值不同  
    assertNotSame(user.getAddress().getCity(), copyUser.getAddress().getCity());  
  
}  
```
<a name="ejQki"></a>
## 方法五 Jackson序列化
Jackson与Gson相似，可以将对象序列化成JSON，明显不同的地方是拷贝的类（包括其成员变量）需要有默认的无参构造函数。
<a name="FtpY0"></a>
### 重写代码
修改一下User类，Address类，实现默认的无参构造函数，使其支持Jackson。
```java
/**  
 * 用户  
 */  
public class User {  
  
    private String name;  
    private Address address;  
  
    // constructors, getters and setters  
  
    public User() {  
    }  
  
}  

/**  
 * 地址  
 */  
public class Address {  
  
    private String city;  
    private String country;  
  
    // constructors, getters and setters  
  
    public Address() {  
    }  
  
} 
```
<a name="pCWqy"></a>
### 测试用例
```java
@Test  
public void jacksonCopy() throws IOException {  
  
    Address address = new Address("杭州", "中国");  
    User user = new User("大山", address);  
  
    // 使用Jackson序列化进行深拷贝  
    ObjectMapper objectMapper = new ObjectMapper();  
    User copyUser = objectMapper.readValue(objectMapper.writeValueAsString(user), User.class);  
  
    // 修改源对象的值  
    user.getAddress().setCity("深圳");  
  
    // 检查两个对象的值不同  
    assertNotSame(user.getAddress().getCity(), copyUser.getAddress().getCity());  
  
}  
```
<a name="X3umM"></a>
## 总结
说了这么多深拷贝的实现方法，哪一种方法才是最好的呢？最简单的判断就是根据拷贝的类（包括其成员变量）是否提供了深拷贝的构造函数、是否实现了Cloneable接口、是否实现了Serializable接口、是否实现了默认的无参构造函数来进行选择。如果需要详细的考虑，则可以参考下面的表格：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639822996986-0c3d3f12-5af6-45d5-9412-d2652134d172.webp#clientId=u10f523df-37fd-4&from=paste&id=u12a24547&originHeight=794&originWidth=795&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u76025a37-e783-4847-8e71-065bdfd764e&title=)

 
