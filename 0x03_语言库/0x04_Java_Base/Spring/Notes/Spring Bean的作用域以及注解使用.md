Java Spring
<a name="Lu5hB"></a>
## Spring Bean的作用域
Spring Bean 的作用域即为`对象的作用范围`。Spring Bean的作用域由配置项 `scope` 来限定。
<a name="IfZE6"></a>
### Scope配置项
作用域限定了 Spring Bean 的作用范围，在 Spring 配置文件定义 Bean 时，通过声明 scope 配置项，可以灵活定义 Bean 的作用范围。<br />想要容器创建的 bean 为单例模式，就设置 Scope 为 `singleton`；想要每次创建的 bean 都是不同的对象则使用 `prototype`。
<a name="eM4YW"></a>
### 六大作用域
官方文档给出的bean作用域的简单介绍。![image.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1613999551480-95f6ac90-feb4-46c3-aece-ce5518f0c3ae.gif#align=left&display=inline&height=1&originHeight=1&originWidth=1&size=70&status=done&style=none&width=1)文档的表格写着是六个作用域，但是很多时候都会被认为只有五个作用域。因为 `application` 和 `websocket` 的作用差不多。所以会被认为是一个。

| 范围 | 描述 |
| :--- | :--- |
| singleton | （默认）将每个Spring IoC容器的单个 bean 定义范围限定为单个对象实例。 |
| prototype | 将单个 bean 定义的作用域限定为任意数量的对象实例。 |
| request | 将单个 bean 定义的范围限定为单个 HTTP 请求的生命周期。也就是说，每个 HTTP 请求都有一个自己的 bean 实例，它是在单个 bean 定义的后面创建的。仅在可感知网络的 Spring 上下文中有效 `ApplicationContext`。 |
| session | 将单个 bean 定义的范围限定为 HTTP 的生命周期 `Session`。仅在可感知网络的 Spring 上下文中有效 `ApplicationContext`。 |
| application | 将单个 bean 定义的作用域限定为的生命周期 `ServletContext`。仅在可感知网络的Spring上下文中有效 `ApplicationContext`。 |
| websocket | 将单个 bean 定义的作用域限定为的生命周期 `WebSocket`。仅在可感知网络的Spring上下文中有效 `ApplicationContext`。 |

重点放在 singleton 和 prototype ，后面四个是对于`WebMVC`而言的，暂不进行详细阐述。
<a name="hUS7U"></a>
## 单例作用域
先看第一个`单例范围`，单例范围即无论使用多少个 Dao 去获取 bean ，每次获取到的都是一个相同的 bean对象。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1613999550495-db7a07ef-4ccb-46a9-8209-8a1bb3a36a65.webp#align=left&display=inline&height=398&originHeight=398&originWidth=800&size=0&status=done&style=shadow&width=800)**「singleton」** 与所说的单例模式基本是相同的，singleton 也是默认的作用域，如果没用指定 bean 对象的作用域则在整个上下文环境中通过 Spring IOC 获取到的 bean 都是同一个实例。<br />配置 bean 为 singleton 作用域代码：
```xml
<bean id="accountService" class="com.something.DefaultAccountService" scope="singleton"/>
```
接下来定义一个POJO类 -- User来演示 scope 属性对实例的影响。
```java
public class User {
    private String name;
    private int age;
    public User() {
    }
    public User(String name, int age) {
        this.name = name;
        this.age = age;
    }
    /*toString和getter/setter*/
    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
}
```
再显示的把该 bean 的作用域设置为 singleton。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:p="http://www.springframework.org/schema/p"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd ">
        <bean id="user" class="com.javastudyway.pojo.User" p:name="Hello" p:age="4" scope="singleton"/>
</beans>
```
最后就是测试类了：
```java
import com.javastudyway.pojo.Student;
import com.javastudyway.pojo.User;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class MyTest {
    @Test
    public void testSingleton(){
        ApplicationContext context = new ClassPathXmlApplicationContext("userbean.xml");
        User user1 = context.getBean("user",User.class);
        User user2 = context.getBean("user",User.class);
        System.out.println(user1 == user2);
    }
}
```
测试结果如下：从比较结果可以看出，程序两次从 IOC 容器中获取的 User 实例**「地址比较结果相同」**，说明 IOC 容器返回的是**「同一个实例」**。![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1613999550498-083ffed2-3e4b-4f0d-914e-787d2474b0ed.webp#align=left&display=inline&height=475&originHeight=475&originWidth=830&size=0&status=done&style=none&width=830)
<a name="BoWBd"></a>
## 原型作用域
与 singleton 刚好相反，`prototype`，是程序每次从 IOC 容器获取的 Bean 都是一个新的实例。这次把 bean 的配置修改成 **「prototype」**，再进行测试。
```xml
<bean id="user" class="com.javastudyway.pojo.User" p:name="Java学习之道" p:age="4" scope="prototype"/>
```
在测试类中增加一个测试方法：
```java
@Test
public void testPrototype(){
    ApplicationContext context = new ClassPathXmlApplicationContext("userbean.xml");
    User user1 = context.getBean("user", User.class);
    User user2 = context.getBean("user", User.class);
    System.out.println("两次对象地址比较结果为：" + (user1 == user2));
    System.out.println("user1 的hasCode为：" + user1.hashCode());
    System.out.println("user2 的hasCode为：" + user2.hashCode());
}
```
测试结果为：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1613999550597-8a58c866-c415-4d48-a657-61be2b5def5a.webp#align=left&display=inline&height=479&originHeight=479&originWidth=1011&size=0&status=done&style=none&width=1011)可以看到，程序两次从 IOC 容器中获取的 user1 和 user2 实例的 hashCode 不相同，说明 User 添加 prototype 作用域后，IOC 容器每次返回的都是一个新的实例。<br />对于作用域使用场景的总结：
> 有状态的bean应该使用prototype作用域；
> 对无状态的bean则应该使用singleton作用域。

<a name="i6vYD"></a>
## 使用注解声明bean的作用域
大多数时候都不想很麻烦的去修改 `applicationContext.xml` ，所以可以使用注解来配置 bean 的作用范围。具体步骤为

- 使用 **「**`**@Component**`**」** 注解将 POJO 类声明为一个 bean；
- 使用 **「**`**@Scope**`**」** 注解声明 bean 的作用域；
- 修改 **「applicationContext.xml」** 开启扫描 POJO 包中的所有类的注解。
<a name="C5ILi"></a>
### 修改POJO类
在 POJO 类上增加 `@Component`，让 Spring 将 User 视为一个 bean。<br />在 POJO 类上增加 `@Scope` 注解，标识该 bean 的作用域为 singleton。
```java
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;
@Component("user")
@Scope("singleton")
public class User {
    private String name;
    private int age;
    public User() {
    }
    public User(String name, int age) {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString() {
        return "User{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
}
```
<a name="wbMa9"></a>
### 开启注解扫描
增加完注解后只需要对 applicationContext.xml 这个 Spring 的配置文件做上一小部分修改，即可实现注解开发了。<br />开启 `com.javastudyway.pojo` 包中的所有类的注解扫描。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:p="http://www.springframework.org/schema/p"
       xmlns:c="http://www.springframework.org/schema/c"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        https://www.springframework.org/schema/beans/spring-beans.xsd http://www.springframework.org/schema/context https://www.springframework.org/schema/context/spring-context.xsd">
    <!-- 开启注解扫描 -->
    <context:component-scan base-package="com.javastudyway.pojo"/>
</beans>
```
<a name="EpmMl"></a>
### 测试
对 POJO 类加上注解和开启注解扫描后，就可以通过简单的注解来设置 bean 的作用域而不再需要去配置文件中慢慢修找修改了。用刚刚 `测试原型作用域的方法` 来测试用注解修改成 singleton 的 bean。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1613999550503-a9c2bb02-9120-4a5b-a0f4-596b524ca9ec.webp#align=left&display=inline&height=464&originHeight=464&originWidth=1080&size=0&status=done&style=none&width=1080)<br />结果从原来的 **「false，hashCode 不相等」**变成了 **「true，hashCode 相等」**了。
