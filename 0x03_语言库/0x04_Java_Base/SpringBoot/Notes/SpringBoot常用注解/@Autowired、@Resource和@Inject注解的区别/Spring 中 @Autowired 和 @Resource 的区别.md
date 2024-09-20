JavaSpring<br />`**@Autowired**`**注解是由Spring提供的**，它可以用来对构造方法、成员变量及方法参数进行标注，它能够根据对象类型完成自动注入，代码如下。
```java
public class Service {

    // 构造方法注入
    @Autowired
    public Service(Service service) {
        this.service = service;
    }

    // 成员变量注入
    @Autowired
    private Service service;
 
    // 方法参数注入
    @Autowired
    public void setService(Service service) {
        this.service = service;
 
    }
}
```
再来看`@Resource`注解，代码如下。
```java
public class Service {
    
    @Resource(name = "service1")
    private Service service1;
    
    @Resource(name = "service2")
    private Service service2;
 

    @Reource
    private Service service3;
 
    @Reource
    private Service service4;

}
```
**它是由JDK提供的**，遵循JSR-250规范，是JDK 1.6及以上加入的新特性。作为Java的标准，它的作用和`@Autowired`无区别。与`@Autowired`不同的是，它适用于所有的Java框架，而`@Autowired`只适用于Spring。可以简单地理解为，`@Resource`能够支持对象类型注入，也能够支持对象名称注入。<br />`@Resource`和`@Autowired`之间**具体有哪些区别呢？**<br />可以从以下5个方面来分析。
<a name="GS66n"></a>
### 1、注解内部定义的参数不同
`@Autowired`只包含一个`required`参数，默认为true，表示开启自动注入。
```java
public @interface Autowired {

     // 是否开启自动注入，在不开启自动装配时，可设为false

    boolean required() default true;
 
}
```
`@Resource` 包含7个参数，其中最重要的两个是name和type。
```java
public @interface Resource {
    // Bean的名称

    String name() default "";

    String lookup() default "";

    // Java类，被解析为Bean的类型
    Class<?> type() default java.lang.Object.class;

    enum AuthenticationType {
        CONTAINER,
        APPLICATION
    }
    
    // 身份验证类型
    
    AuthenticationType authenticationType() default AuthenticationType.CONTAINER;
    
    // 组件是否可以与其他组件共享
    
    boolean shareable() default true;
    
    String mappedName() default "";
    
    // 描述
    String description() default "";
}
```
<a name="uDSV0"></a>
### 2、装配方式的默认值不同
`@Autowired`默认按type自动装配，而`@Resource`默认按name自动装配。`@Resource`注解可以自定义选择装配方式，如果指定name，则按name自动装配。如果指定type，则按type自动装配。
<a name="bNMvY"></a>
###  3、注解应用的范围不同
`@Autowired`能够用在构造方法、成员变量、方法参数及注解上，而`@Resource`能用在类、成员变量和方法参数上，源码如下。
```java
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD, ElementType.PARAMETER, ElementType.FIELD, ElementType.ANNOTATION_TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Autowired { ... }

@Target({TYPE, FIELD, METHOD})
@Retention(RUNTIME)
public @interface Resource {  ... }
```
<a name="MZ4aV"></a>
### 4、出处不同
`@Autowired`是Spring定义的注解，而`@Resource`遵循JSR-250的规范，定义在JDK中。所以`@Autowired`只能在Spring框架下使用，而`@Resource`则可以与其他框架一起使用。
<a name="qnjmM"></a>
### 5、装配顺序不同
`@Autowired`默认先与byType进行匹配，如果发现找到多个Bean，则又按照byName方式进行匹配，如果还有多个Bean，则报出异常。装配顺序如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687828640167-093d413c-c7cc-4e8c-ab3b-361b6982c6c4.png#averageHue=%23f7f7f7&clientId=ub39edddd-0f3c-4&from=paste&id=uc1cfe378&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u19ecd438-cbd8-473f-8d78-897bdcc258d&title=)<br />而`@Resource`的装载顺序分为如下4种情况。<br />1）如果同时指定name和type，则从Spring上下文中找到与它们唯一匹配的Bean进行装配，如果找不到则抛出异常，具体流程如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687828640050-ce885ab0-1354-4e82-8aef-b83d31bf349c.png#averageHue=%23f7f7f7&clientId=ub39edddd-0f3c-4&from=paste&id=u13e90734&originHeight=304&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u99af249c-151a-4ddd-a1aa-1ad39785099&title=)<br />2）如果指定name，则从上下文中查找与名称（ID）匹配的Bean进行装配，如果找不到则抛出异常，具体流程如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687828639959-19079a38-1b15-4d42-8a83-37e4c5b069a6.png#averageHue=%23f7f7f7&clientId=ub39edddd-0f3c-4&from=paste&id=u11709296&originHeight=261&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua5a33555-f4e6-4472-b9f7-05d69f68760&title=)<br />3）如果指定type，则从上下文中找到与类型匹配的唯一Bean进行装配，如果找不到或者找到多个就会抛出异常，具体流程如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687828640105-a67545ad-9e3a-41cc-b328-106f7d3692fa.png#averageHue=%23f5f5f5&clientId=ub39edddd-0f3c-4&from=paste&id=u6eca5e10&originHeight=248&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9144ecd6-a3ff-4658-9517-fa0537f4f27&title=)<br />4）如果既没有指定name，也没有指定type，则自动按byName方式进行装配。如果没有匹配成功，则仍按照type进行匹配，具体流程如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1687828639965-a0076e61-92ed-48e8-9140-c37a20cc69c5.png#averageHue=%23f5f5f5&clientId=ub39edddd-0f3c-4&from=paste&id=ue24f08ba&originHeight=678&originWidth=595&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc1eacdf7-023f-464d-919e-6bbb3d5289c&title=)<br />下面这张表可以帮助大家更好地理解和区分`@Autowired`和`@Resource`。

|  | `@Autowired` | `@Resource` |
| --- | --- | --- |
| 是否支持优先级 | 支持。在装配Bean时加`@Primary`即可 | 不支持 |
| Bean是否必须存在 | 默认必须存在。<br />如果允许不存在则可以这样：<br />`@Autowired(required= false)` | 必须存在。<br />无法配置允许不存在 |
| 提供者 | Spring | JDK(需要1.6版本及以上) |
| 默认注入方式 | 默认按照类型。按名字注入可以这样装配：<br />`@Component("bean的名字")`或<br />`@Bean("bean的名字")`<br />注入：<br />`@Autowired`<br />`@Qualifier("bean的名字")` | 默认根据名字(byName)按类型注<br />入可以这样注入：`@Resource(type= Xxx.class)` |

总结一下，两者在功能上差别不大，使用起来也差不多。但是，**在日常开发中建议使用**`**@Autowired**`**，有以下3个理由。**<br />第1：`@Autowired`功能略强大。支持优先注入、可以配置允许Bean不存在。<br />第2：若使用Spring框架，则使用其特有的注解更好一点。<br />第3：有人认为`@Resource`更加通用，因为它是一个规范，其他框架也会支持。目前后端都在用Spring，没有必要考虑其他框架。
