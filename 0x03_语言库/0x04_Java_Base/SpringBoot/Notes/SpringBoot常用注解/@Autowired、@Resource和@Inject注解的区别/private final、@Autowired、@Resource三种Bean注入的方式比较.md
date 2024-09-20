Java
<a name="v7qgK"></a>
## 一、引言
项目中出现了三种注入bean的方式，`@Resource`、`@Autowired`、`private final`，一起来看一看他们的用法和区别。
<a name="cLec3"></a>
## 二、Bean的注入
这里是一个简单的示例代码，演示了 `@Resource`、`@Autowired`和 `private final`三种注入方式的使用：
```java
public class MyService{
    @Resource
    private MyDependency resourceDependency;

    @Autowired
    private MyDependency autowiredDependency;

    privatefinal MyDependency finalDependency;

    publicMyService(MyDependency finalDependency){
        this.finalDependency = finalDependency;
    }

    // ...
}

public class MyDependency{
    // ...
}
```
在上面的示例中，MyService类使用了三种不同的方式来注入 MyDependency依赖。<br />`@Resource`和 `@Autowired`注解用于注入可变的依赖，而 `private final`方式用于注入不可变的依赖。<br />这三种方式都是目前注入bean的方式，也常被面试官提及发问，不管使用哪种方式，都是可以的。
<a name="KOAuM"></a>
## 三、@Resource
<a name="AeJQb"></a>
### `@Resource`的原理：
`@Resource`注解是Java标准库提供的一种依赖注入方式。<br />在Spring中，`@Resource`注解的处理由 `CommonAnnotationBeanPostProcessor`类负责。<br />该类是一个Bean后置处理器，用于处理使用了 `@Resource`注解的字段或方法参数。<br />`CommonAnnotationBeanPostProcessor`在Spring的容器初始化过程中起作用，它会扫描所有的Bean定义，并检查是否存在使用了 `@Resource`注解的字段或方法参数。<br />当检测到这样的注解时，它会解析注解中指定的名称（name），并尝试查找匹配的依赖。<br />具体地说，`CommonAnnotationBeanPostProcessor`使用了Java的反射机制来解析注解，并通过 javax.annotation.Resource规范中定义的算法来查找匹配的依赖。根据规范，`@Resource`注解默认按照字段或方法参数的名称进行依赖的匹配，也可以通过 name属性指定特定的依赖名称。
<a name="zOP7h"></a>
### `@Resource`的使用方式：
`@Resource`注解可以用于字段、方法和构造函数上，用于指定要注入的依赖。下面是几种常见的使用方式：
<a name="o5BZr"></a>
#### 字段注入：
```java
public class MyService{
    @Resource
    private MyDependency dependency;
    // ...
}
```
<a name="rW5CI"></a>
#### 方法注入：
```java
public class MyService{
    private MyDependency dependency;

    @Resource
    publicvoidsetDependency(MyDependency dependency){
        this.dependency = dependency;
    }
    // ...

}
```
<a name="o7chw"></a>
#### 构造函数注入：
```java
public class MyService{
    private MyDependency dependency;

    @Resource
    publicMyService(MyDependency dependency){
        this.dependency = dependency;
    }
    // ...

}
```
在上述示例中，`@Resource`注解用于标记要注入的依赖，并根据默认的名称匹配规则或显式指定的名称来查找依赖。<br />需要注意的是，`@Resource`注解是Java标准库中的注解，因此它可以与Java SE和Java EE应用程序一起使用，不仅限于Spring框架。<br />总结起来，`@Resource`注解是Java标准库提供的一种依赖注入方式，在Spring中借助 `CommonAnnotationBeanPostProcessor`来实现。它可以用于标记要注入的依赖，并根据名称匹配规则或显式指定的名称来查找依赖。
<a name="quqiA"></a>
## 四、`@Autowired`
当涉及到 `@Autowired`注解的原理、使用方式以及与 `@Resource`注解的区别时，需要了解它在Spring中的实现方式。下面详细解释 `@Autowired`的原理、使用方式以及与 `@Resource`注解的区别。
<a name="je8Zv"></a>
### `**@Autowired**`**的原理：**
`@Autowired`注解是Spring Framework提供的一种依赖注入方式。它通过 `AutowiredAnnotationBeanPostProcessor`类来实现。`AutowiredAnnotationBeanPostProcessor`是一个Bean后置处理器，用于处理使用了 `@Autowired`注解的字段、方法或构造函数参数。<br />在Spring的容器初始化过程中，`AutowiredAnnotationBeanPostProcessor`会扫描所有的Bean定义，检查是否存在使用了 `@Autowired`注解的字段、方法或构造函数参数。当检测到这样的注解时，它会解析注解，并根据依赖的类型来寻找匹配的Bean，并将其注入到目标对象中。<br />具体来说，`AutowiredAnnotationBeanPostProcessor`使用了Java的反射机制来解析 `@Autowired`注解，并利用Spring容器中的Bean定义信息来查找匹配的依赖。它支持按照类型、名称和限定符等方式进行依赖的解析和注入。
<a name="VJ2Dm"></a>
### `**@Autowired**`**的使用方式：**
`@Autowired`注解可以用于字段、方法和构造函数上，用于指定要注入的依赖。下面是几种常见的使用方式：

1. 字段注入：
```java
public class MyService {
    @Autowired
    private MyDependency dependency;
    // ...
}
```

2. 方法注入：
```java
public class MyService {
    private MyDependency dependency;

    @Autowired
    public void setDependency(MyDependency dependency) {
        this.dependency = dependency;
    }
    // ...
}
```

3. 构造函数注入：
```java
public class MyService {
    private MyDependency dependency;

    @Autowired
    public MyService(MyDependency dependency) {
        this.dependency = dependency;
    }
    // ...
}
```
在上述示例中，`@Autowired`注解用于标记要注入的依赖。Spring会根据依赖的类型（或名称和限定符）自动查找匹配的Bean，并将其注入到目标对象中。
<a name="GSuiB"></a>
### `**@Autowired**`**与**`**@Resource**`**的区别：**
尽管 `@Autowired`和 `@Resource`都是用于依赖注入的注解，但它们在一些方面有一些区别。

1. 来源和依赖关系：`@Autowired`是Spring特有的注解，用于实现Spring的依赖注入机制。它通过类型匹配来解析依赖关系。而 `@Resource`是Java标准库中的注解，它可以与Java SE和Java EE应用程序一起使用，不仅限于Spring框架。`@Resource`通过名称匹配来解析依赖关系。
2. 配置方式：`@Autowired`注解通常与 `@Component`、`@Service`等Spring注解一起使用，通过组件扫描和自动装配来实现依赖注入。而 `@Resource`注解不需要特定的配置，它可以直接用于字段、方法和构造函数上。
3. 名称匹配规则：`@Autowired`注解默认按照类型进行依赖的匹配。如果存在多个匹配的Bean，可以使用 `@Qualifier`注解指定特定的Bean名称或限定符。而 `@Resource`注解默认按照名称进行依赖的匹配，也可以通过 name属性指定特定的依赖名称。
4. 兼容性：由于 `@Resource`是Java标准库中的注解，因此它可以与Java SE和Java EE应用程序一起使用，并且在不同的容器中具有较好的兼容性。相比之下，`@Autowired`注解是Spring特有的注解，主要用于Spring框架中，因此在非Spring环境中可能无法正常使用。
5. Null处理：当无法找到匹配的依赖时，`@Autowired`注解的字段或方法参数可以为 null，而 `@Resource`注解要求必须找到匹配的依赖，否则会抛出异常。

需要注意的是，尽管 `@Autowired`和 `@Resource`有一些区别，但在大多数情况下，它们可以互换使用。<br />如果您只是简单地进行依赖注入，并且不需要关注兼容性问题，那么两者的选择取决于个人偏好。
<a name="v6Qak"></a>
## 五、`priavet final`
<a name="Qr8wQ"></a>
### `**private final**`**字段的自动注入原理：**
在Spring Boot中，对于 `private final`字段的自动注入，Spring使用构造函数注入（Constructor Injection）来实现。当一个Bean有一个或多个 `private final`字段需要注入时，Spring会尝试在容器中查找与这些字段类型匹配的Bean，并使用构造函数注入来实例化Bean对象。<br />构造函数注入的原理是，Spring会查找与构造函数参数类型匹配的Bean，并将它们作为参数传递给构造函数，从而实现对 `private final`字段的赋值。
<a name="PzkRv"></a>
### `**private final**`**字段的使用方式：**
在使用 `private final`字段进行自动注入时，需要遵循以下几个步骤：

1. 定义一个带有 `private final`字段的类，并确保该类是一个Spring Bean。
```java
@Component
public class MyService {
    private final MyDependency dependency;

    public MyService(MyDependency dependency) {
        this.dependency = dependency;
    }

    // ...
}
```

2. 在类的构造函数中接收依赖参数，并将其赋值给 `private final`字段。
3. Spring Boot会自动扫描并创建Bean实例，同时检测到带有 `private final`字段的构造函数，会自动解析依赖并进行注入。

需要注意的是，由于 `private final`字段是不可变的，一旦注入后就无法修改。这种不可变性有助于确保字段的安全性和线程安全性。
