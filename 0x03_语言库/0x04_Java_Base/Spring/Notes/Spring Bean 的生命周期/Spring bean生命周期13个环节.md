JavaSpring

1. 阶段1：Bean元信息配置阶段
2. 阶段2：Bean元信息解析阶段
3. 阶段3：将Bean注册到容器中
4. 阶段4：BeanDefinition合并阶段
5. 阶段5：Bean Class加载阶段
6. 阶段6：Bean实例化阶段（2个小阶段）
- Bean实例化前阶段
- Bean实例化阶段
7. 阶段7：合并后的BeanDefinition处理
8. 阶段8：属性赋值阶段（3个小阶段）
- Bean实例化后阶段
- Bean属性赋值前阶段
- Bean属性赋值阶段
9. 阶段9：Bean初始化阶段（5个小阶段）
- Bean Aware接口回调阶段
- Bean初始化前阶段
- Bean初始化阶段
- Bean初始化后阶段
10. 阶段10：所有单例bean初始化完成后阶段
11. 阶段11：Bean的使用阶段
12. 阶段12：Bean销毁前阶段
13. 阶段13：Bean销毁阶段
<a name="THjgw"></a>
## 阶段1：Bean元信息配置阶段
这个阶段主要是bean信息的定义阶段。
<a name="LWvz5"></a>
### Bean信息定义4种方式

- API的方式
- Xml文件方式
- properties文件的方式
- 注解的方式
<a name="ToWM7"></a>
### API的方式
先来说这种方式，因为其他几种方式最终都会采用这种方式来定义bean配置信息。<br />**Spring容器启动的过程中，会将Bean解析成Spring内部的**`**BeanDefinition**`**结构**。<br />不管是是通过xml配置文件的`<Bean>`标签，还是通过注解配置的`@Bean`，还是`@Compontent`标注的类，还是扫描得到的类，它最终都会被解析成一个`BeanDefinition`对象，最后的Bean工厂就会根据这份Bean的定义信息，对bean进行实例化、初始化等等操作。<br />可以把`BeanDefinition`丢给Bean工厂，然后Bean工厂就会根据这个信息帮你生产一个Bean实例，拿去使用。<br />`BeanDefinition`里面里面包含了bean定义的各种信息，如：bean对应的class、scope、lazy信息、dependOn信息、`autowireCandidate`（是否是候选对象）、primary（是否是主要的候选者）等信息。<br />`BeanDefinition`是个接口，有几个实现类，看一下类图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689307883754-050ad6ca-dc95-4686-b242-53d9e0ee4dd5.png#averageHue=%23fbfbf3&clientId=ud49c7d06-c45a-4&from=paste&id=udef5cb04&originHeight=233&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud264bd6f-846d-4ca8-9ec4-a5608b254f9&title=)
<a name="hMPFN"></a>
#### `BeanDefinition`接口：bean定义信息接口
表示bean定义信息的接口，里面定义了一些获取bean定义配置信息的各种方法，来看一下源码：
```java
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement {

    /**
     * 设置此bean的父bean名称（对应xml中bean元素的parent属性）
     */
    void setParentName(@Nullable String parentName);

    /**
     * 返回此bean定义时指定的父bean的名称
     */
    @Nullable
    String getParentName();

    /**
     * 指定此bean定义的bean类名(对应xml中bean元素的class属性)
     */
    void setBeanClassName(@Nullable String beanClassName);

    /**
     * 返回此bean定义的当前bean类名
     * 注意，如果子定义重写/继承其父类的类名，则这不一定是运行时使用的实际类名。此外，这可能只是调用工厂方法的类，或者在调用方法的工厂bean引用的情况下，它甚至可能是空的。因此，不要认为这是运行时的最终bean类型，而只将其用于单个bean定义级别的解析目的。
     */
    @Nullable
    String getBeanClassName();

    /**
     * 设置此bean的生命周期，如：singleton、prototype（对应xml中bean元素的scope属性）
     */
    void setScope(@Nullable String scope);

    /**
     * 返回此bean的生命周期，如：singleton、prototype
     */
    @Nullable
    String getScope();

    /**
     * 设置是否应延迟初始化此bean（对应xml中bean元素的lazy属性）
     */
    void setLazyInit(boolean lazyInit);

    /**
     * 返回是否应延迟初始化此bean，只对单例bean有效
     */
    boolean isLazyInit();

    /**
     * 设置此bean依赖于初始化的bean的名称,bean工厂将保证dependsOn指定的bean会在当前bean初始化之前先初始化好
     */
    void setDependsOn(@Nullable String... dependsOn);

    /**
     * 返回此bean所依赖的bean名称
     */
    @Nullable
    String[] getDependsOn();

    /**
     * 设置此bean是否作为其他bean自动注入时的候选者
     * autowireCandidate
     */
    void setAutowireCandidate(boolean autowireCandidate);

    /**
     * 返回此bean是否作为其他bean自动注入时的候选者
     */
    boolean isAutowireCandidate();

    /**
     * 设置此bean是否为自动注入的主要候选者
     * primary：是否为主要候选者
     */
    void setPrimary(boolean primary);

    /**
     * 返回此bean是否作为自动注入的主要候选者
     */
    boolean isPrimary();

    /**
     * 指定要使用的工厂bean（如果有）。这是要对其调用指定工厂方法的bean的名称。
     * factoryBeanName：工厂bean名称
     */
    void setFactoryBeanName(@Nullable String factoryBeanName);

    /**
     * 返回工厂bean名称（如果有）（对应xml中bean元素的factory-bean属性）
     */
    @Nullable
    String getFactoryBeanName();

    /**
     * 指定工厂方法（如果有）。此方法将使用构造函数参数调用，如果未指定任何参数，则不使用任何参数调用。该方法将在指定的工厂bean（如果有的话）上调用，或者作为本地bean类上的静态方法调用。
     * factoryMethodName：工厂方法名称
     */
    void setFactoryMethodName(@Nullable String factoryMethodName);

    /**
     * 返回工厂方法名称（对应xml中bean的factory-method属性）
     */
    @Nullable
    String getFactoryMethodName();

    /**
     * 返回此bean的构造函数参数值
     */
    ConstructorArgumentValues getConstructorArgumentValues();

    /**
     * 是否有构造器参数值设置信息（对应xml中bean元素的<constructor-arg />子元素）
     */
    default boolean hasConstructorArgumentValues() {
        return !getConstructorArgumentValues().isEmpty();
    }

    /**
     * 获取bean定义是配置的属性值设置信息
     */
    MutablePropertyValues getPropertyValues();

    /**
     * 这个bean定义中是否有属性设置信息（对应xml中bean元素的<property />子元素）
     */
    default boolean hasPropertyValues() {
        return !getPropertyValues().isEmpty();
    }

    /**
     * 设置bean初始化方法名称
     */
    void setInitMethodName(@Nullable String initMethodName);

    /**
     * bean初始化方法名称
     */
    @Nullable
    String getInitMethodName();

    /**
     * 设置bean销毁方法的名称
     */
    void setDestroyMethodName(@Nullable String destroyMethodName);

    /**
    * bean销毁的方法名称
    */
    @Nullable
    String getDestroyMethodName();

    /**
    * 设置bean的role信息
    */
    void setRole(int role);

    /**
    * bean定义的role信息
    */
    int getRole();

    /**
    * 设置bean描述信息
    */
    void setDescription(@Nullable String description);

    /**
    * bean描述信息
    */
    @Nullable
    String getDescription();

    /**
    * bean类型解析器
    */
    ResolvableType getResolvableType();

    /**
    * 是否是单例的bean
    */
    boolean isSingleton();

    /**
    * 是否是多列的bean
    */
    boolean isPrototype();

    /**
    * 对应xml中bean元素的abstract属性，用来指定是否是抽象的
    */
    boolean isAbstract();

    /**
    * 返回此bean定义来自的资源的描述（以便在出现错误时显示上下文）
    */
    @Nullable
    String getResourceDescription();

    @Nullable
    BeanDefinition getOriginatingBeanDefinition();

    }
```
`BeanDefinition`接口上面还继承了2个接口：

- `AttributeAccessor`
- `BeanMetadataElement`
<a name="YJFme"></a>
##### `AttributeAccessor`接口：属性访问接口
```java
public interface AttributeAccessor {

    /**
     * 设置属性->值
     */
    void setAttribute(String name, @Nullable Object value);

    /**
     * 获取某个属性对应的值
     */
    @Nullable
    Object getAttribute(String name);

    /**
     * 移除某个属性
     */
    @Nullable
    Object removeAttribute(String name);

    /**
     * 是否包含某个属性
     */
    boolean hasAttribute(String name);

    /**
     * 返回所有的属性名称
     */
    String[] attributeNames();

}
```
这个接口相当于key->value数据结构的一种操作，`BeanDefinition`继承这个，内部实际上是使用了`LinkedHashMap`来实现这个接口中的所有方法，通常通过这些方法来保存`BeanDefinition`定义过程中产生的一些附加信息。
<a name="NM4Q0"></a>
##### `BeanMetadataElement`接口
看一下其源码：
```java
public interface BeanMetadataElement {

    @Nullable
    default Object getSource() {
        return null;
    }

}
```
`BeanDefinition`继承这个接口，`getSource`返回`BeanDefinition`定义的来源，比如通过xml定义`BeanDefinition`的，此时`getSource`就表示定义bean的xml资源；若通过api的方式定义`BeanDefinition`，可以将source设置为定义`BeanDefinition`时所在的类，出错时，可以根据这个来源方便排错。
<a name="aFa9c"></a>
#### `RootBeanDefinition`类：表示根bean定义信息
通常bean中没有父bean的就使用这种表示
<a name="T17vI"></a>
#### `ChildBeanDefinition`类：表示子bean定义信息
如果需要指定父bean的，可以使用`ChildBeanDefinition`来定义子bean的配置信息，里面有个parentName属性，用来指定父bean的名称。
<a name="dcDNU"></a>
#### `GenericBeanDefinition`类：通用的bean定义信息
既可以表示没有父bean的bean配置信息，也可以表示有父bean的子bean配置信息，这个类里面也有parentName属性，用来指定父bean的名称。
<a name="JGAX5"></a>
#### `ConfigurationClassBeanDefinition`类：表示通过配置类中`@Bean`方法定义bean信息
可以通过配置类中使用`@Bean`来标注一些方法，通过这些方法来定义bean，这些方法配置的bean信息最后会转换为`ConfigurationClassBeanDefinition`类型的对象
<a name="hrwkY"></a>
#### `AnnotatedBeanDefinition`接口：表示通过注解的方式定义的bean信息
里面有个方法
```java
AnnotationMetadata getMetadata();
```
用来获取定义这个bean的类上的所有注解信息。
<a name="GXWHV"></a>
#### `BeanDefinitionBuilder`：构建`BeanDefinition`的工具类
Spring中为了方便操作`BeanDefinition`，提供了一个类：`BeanDefinitionBuilder`，内部提供了很多静态方法，通过这些方法可以非常方便的组装`BeanDefinition`对象，下面通过案例来感受一下。
<a name="Z0vG4"></a>
#### 案例1：组装一个简单的bean
<a name="D6BkS"></a>
##### 来个简单的类
```java
package com.javacode2018.lesson002.demo1;

public class Car {
    private String name;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Car{" +
        "name='" + name + '\'' +
        '}';
    }
}
```
<a name="kLeuj"></a>
##### 测试用例
```java
@Test
public void test1() {
    //指定class
    BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.rootBeanDefinition(Car.class.getName());
    //获取BeanDefinition
    BeanDefinition beanDefinition = beanDefinitionBuilder.getBeanDefinition();
    System.out.println(beanDefinition);
}
```
等效于
```xml
<bean class="com.javacode2018.lesson002.demo1.Car" />
```
<a name="DQQhf"></a>
##### 运行输出
```
Root bean: class [com.javacode2018.lesson002.demo1.Car]; scope=; abstract=false; lazyInit=null; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
```
<a name="IjBXN"></a>
#### 案例2：组装一个有属性的bean
<a name="pM9M1"></a>
##### 代码
```java
@Test
public void test2() {
    //指定class
    BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.rootBeanDefinition(Car.class.getName());
    //设置普通类型属性
    beanDefinitionBuilder.addPropertyValue("name", "奥迪"); //@1
    //获取BeanDefinition
    BeanDefinition carBeanDefinition = beanDefinitionBuilder.getBeanDefinition();
    System.out.println(carBeanDefinition);

    //创建spring容器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory(); //@2
    //调用registerBeanDefinition向容器中注册bean
    factory.registerBeanDefinition("car", carBeanDefinition); //@3
    Car bean = factory.getBean("car", Car.class); //@4
    System.out.println(bean);
}
```
@1：调用`addPropertyValue`给Car中的name设置值<br />@2：创建了一个Spring容器<br />@3：将`carBeanDefinition`这个bean配置信息注册到Spring容器中，bean的名称为car<br />@4：从容器中获取car这个bean，最后进行输出
<a name="Pvqyz"></a>
##### 运行输出
```
Root bean: class [com.javacode2018.lesson002.demo1.Car]; scope=; abstract=false; lazyInit=null; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
Car{name='奥迪'}
```
第二行输出了从容器中获取的car这个bean实例对象。
<a name="GAixn"></a>
#### 案例3：组装一个有依赖关系的bean
<a name="vrNqn"></a>
##### 再来个类
下面这个类中有个car属性，通过Spring将这个属性注入进来。
```java
package com.javacode2018.lesson002.demo1;

public class User {
    private String name;

    private Car car;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Car getCar() {
        return car;
    }

    public void setCar(Car car) {
        this.car = car;
    }

    @Override
    public String toString() {
        return "User{" +
        "name='" + name + '\'' +
        ", car=" + car +
        '}';
    }
}
```
<a name="baLb8"></a>
##### 重点代码
```java
@Test
public void test3() {
    //先创建car这个BeanDefinition
    BeanDefinition carBeanDefinition = BeanDefinitionBuilder.rootBeanDefinition(Car.class.getName()).addPropertyValue("name", "奥迪").getBeanDefinition();
    //创建User这个BeanDefinition
    BeanDefinition userBeanDefinition = BeanDefinitionBuilder.rootBeanDefinition(User.class.getName()).
            addPropertyValue("name", "Java").
            addPropertyReference("car", "car"). //@1
            getBeanDefinition();

    //创建spring容器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
    //调用registerBeanDefinition向容器中注册bean
    factory.registerBeanDefinition("car", carBeanDefinition); 
    factory.registerBeanDefinition("user", userBeanDefinition);
    System.out.println(factory.getBean("car"));
    System.out.println(factory.getBean("user"));
}
```
@1：注入依赖的bean，需要使用`addPropertyReference`方法，2个参数，第一个为属性的名称，第二个为需要注入的bean的名称<br />上面代码等效于
```xml
<bean id="car" class="com.javacode2018.lesson002.demo1.Car">
  <property name="name" value="奥迪"/>
</bean>

<bean id="user" class="com.javacode2018.lesson002.demo1.User">
  <property name="name" value="Java"/>
  <property name="car" ref="car"/>
</bean>
```
<a name="x1nfv"></a>
##### 运行输出
```
Car{name='奥迪'}
User{name='Java', car=Car{name='奥迪'}}
```
<a name="uI4D3"></a>
#### 案例4：来2个有父子关系的bean
```java
@Test
public void test4() {
    //先创建car这个BeanDefinition
    BeanDefinition carBeanDefinition1 = BeanDefinitionBuilder.
            genericBeanDefinition(Car.class).
            addPropertyValue("name", "保时捷").
            getBeanDefinition();

    BeanDefinition carBeanDefinition2 = BeanDefinitionBuilder.
            genericBeanDefinition(). //内部生成一个GenericBeanDefinition对象
            setParentName("car1"). //@1：设置父bean的名称为car1
            getBeanDefinition();

    //创建spring容器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
    //调用registerBeanDefinition向容器中注册bean
    //注册car1->carBeanDefinition1
    factory.registerBeanDefinition("car1", carBeanDefinition1);
    //注册car2->carBeanDefinition2
    factory.registerBeanDefinition("car2", carBeanDefinition2);
    //从容器中获取car1
    System.out.println(String.format("car1->%s", factory.getBean("car1")));
    //从容器中获取car2
    System.out.println(String.format("car2->%s", factory.getBean("car2")));
}
```
等效于
```xml
<bean id="car1" class="com.javacode2018.lesson002.demo1.Car">
  <property name="name" value="保时捷"/>
</bean>
<bean id="car2" parent="car1" />
```
<a name="W2kE1"></a>
##### 运行输出
```
car1->Car{name='保时捷'}
car2->Car{name='保时捷'}
```
<a name="dDPRm"></a>
#### 案例5：通过api设置（Map、Set、List）属性
下面来演示注入List、Map、Set，内部元素为普通类型及其他bean元素。
<a name="nlTXd"></a>
##### 来个类
```java
package com.javacode2018.lesson002.demo1;

import java.util.List;
import java.util.Map;
import java.util.Set;

public class CompositeObj {

    private String name;
    private Integer salary;

    private Car car1;
    private List<String> stringList;
    private List<Car> carList;

    private Set<String> stringSet;
    private Set<Car> carSet;

    private Map<String, String> stringMap;
    private Map<String, Car> stringCarMap;

    //此处省略了get和set方法，大家写的时候记得补上

    @Override
    public String toString() {
        return "CompositeObj{" +
        "name='" + name + '\'' +
        "\n\t\t\t, salary=" + salary +
        "\n\t\t\t, car1=" + car1 +
        "\n\t\t\t, stringList=" + stringList +
        "\n\t\t\t, carList=" + carList +
        "\n\t\t\t, stringSet=" + stringSet +
        "\n\t\t\t, carSet=" + carSet +
        "\n\t\t\t, stringMap=" + stringMap +
        "\n\t\t\t, stringCarMap=" + stringCarMap +
        '}';
    }
}
```
**注意：上面省略了get和set方法，大家写的时候记得补上**
<a name="H2dZl"></a>
##### 先用xml来定义一个CompositeObj的bean，如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="car1" class="com.javacode2018.lesson002.demo1.Car">
    <property name="name" value="奥迪"/>
  </bean>

  <bean id="car2" class="com.javacode2018.lesson002.demo1.Car">
    <property name="name" value="保时捷"/>
  </bean>

  <bean id="compositeObj" class="com.javacode2018.lesson002.demo1.CompositeObj">
    <property name="name" value="Java"/>
    <property name="salary" value="50000"/>
    <property name="car1" ref="car1"/>
    <property name="stringList">
      <list>
        <value>java高并发</value>
        <value>mysql</value>
        <value>maven高手</value>
      </list>
    </property>
    <property name="carList">
      <list>
        <ref bean="car1"/>
        <ref bean="car2"/>
      </list>
    </property>
    <property name="stringSet">
      <set>
        <value>java高并发</value>
        <value>mysql</value>
        <value>maven高手</value>
      </set>
    </property>
    <property name="carSet">
      <set>
        <ref bean="car1"/>
        <ref bean="car2"/>
      </set>
    </property>
    <property name="stringMap">
      <map>
        <entry key="系列1" value="java高并发"/>
        <entry key="系列2" value="Maven高手"/>
        <entry key="系列3" value="mysql"/>
      </map>
    </property>
    <property name="stringCarMap">
      <map>
        <entry key="car1" value-ref="car1"/>
        <entry key="car2" value-ref="car2"/>
      </map>
    </property>
  </bean>
</beans>
```
<a name="Zm9oY"></a>
##### 下面采用纯api的方式实现，如下
```java
@Test
public void test5() {
    //定义car1
    BeanDefinition car1 = BeanDefinitionBuilder.
            genericBeanDefinition(Car.class).
            addPropertyValue("name", "奥迪").
            getBeanDefinition();
    //定义car2
    BeanDefinition car2 = BeanDefinitionBuilder.
            genericBeanDefinition(Car.class).
            addPropertyValue("name", "保时捷").
            getBeanDefinition();

    //定义CompositeObj这个bean
    //创建stringList这个属性对应的值
    ManagedList<String> stringList = new ManagedList<>();
    stringList.addAll(Arrays.asList("java高并发", "mysql", "maven高手"));

    //创建carList这个属性对应的值,内部引用其他两个bean的名称[car1,car2]
    ManagedList<RuntimeBeanReference> carList = new ManagedList<>();
    carList.add(new RuntimeBeanReference("car1"));
    carList.add(new RuntimeBeanReference("car2"));

    //创建stringList这个属性对应的值
    ManagedSet<String> stringSet = new ManagedSet<>();
    stringSet.addAll(Arrays.asList("java高并发", "mysql", "maven高手"));

    //创建carSet这个属性对应的值,内部引用其他两个bean的名称[car1,car2]
    ManagedList<RuntimeBeanReference> carSet = new ManagedList<>();
    carSet.add(new RuntimeBeanReference("car1"));
    carSet.add(new RuntimeBeanReference("car2"));

    //创建stringMap这个属性对应的值
    ManagedMap<String, String> stringMap = new ManagedMap<>();
    stringMap.put("系列1", "java高并发");
    stringMap.put("系列2", "Maven高手");
    stringMap.put("系列3", "mysql");

    ManagedMap<String, RuntimeBeanReference> stringCarMap = new ManagedMap<>();
    stringCarMap.put("car1", new RuntimeBeanReference("car1"));
    stringCarMap.put("car2", new RuntimeBeanReference("car2"));


    //下面我们使用原生的api来创建BeanDefinition
    GenericBeanDefinition compositeObj = new GenericBeanDefinition();
    compositeObj.setBeanClassName(CompositeObj.class.getName());
    compositeObj.getPropertyValues().add("name", "Java").
            add("salary", 50000).
            add("car1", new RuntimeBeanReference("car1")).
            add("stringList", stringList).
            add("carList", carList).
            add("stringSet", stringSet).
            add("carSet", carSet).
            add("stringMap", stringMap).
            add("stringCarMap", stringCarMap);

    //将上面bean 注册到容器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
    factory.registerBeanDefinition("car1", car1);
    factory.registerBeanDefinition("car2", car2);
    factory.registerBeanDefinition("compositeObj", compositeObj);

    //下面我们将容器中所有的bean输出
    for (String beanName : factory.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, factory.getBean(beanName)));
    }
}
```
有几点需要说一下：<br />`RuntimeBeanReference`：用来表示bean引用类型，类似于xml中的ref<br />`ManagedList`：属性如果是List类型的，t需要用到这个类进行操作，这个类继承了`ArrayList`<br />`ManagedSet`：属性如果是Set类型的，t需要用到这个类进行操作，这个类继承了`LinkedHashSet`<br />`ManagedMap`：属性如果是Map类型的，t需要用到这个类进行操作，这个类继承了`LinkedHashMap`<br />上面也就是这几个类结合的结果。
<a name="h7OdY"></a>
##### 看一下效果，运行输出
```
car1->Car{name='奥迪'}
car2->Car{name='保时捷'}
compositeObj->CompositeObj{name='Java'
            , salary=50000
            , car1=Car{name='奥迪'}
            , stringList=[java高并发, mysql, maven高手]
            , carList=[Car{name='奥迪'}, Car{name='保时捷'}]
            , stringSet=[java高并发, mysql系列, maven高手]
            , carSet=[Car{name='奥迪'}, Car{name='保时捷'}]
            , stringMap={系列1=java高并发, 系列2=Maven高手, 系列3=mysql}
            , stringCarMap={car1=Car{name='奥迪'}, car2=Car{name='保时捷'}}}
```
<a name="kIIyn"></a>
### Xml文件方式
这种方式已经讲过很多次了，大家也比较熟悉，即通过xml的方式来定义bean，如下
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="bean名称" class="bean完整类名"/>

</beans>
```
xml中的bean配置信息会被解析器解析为`BeanDefinition`对象，一会在第二阶段详解。
<a name="CkyPZ"></a>
### properties文件的方式
这种方式估计大家比较陌生，将bean定义信息放在properties文件中，然后通过解析器将配置信息解析为`BeanDefinition`对象。<br />properties内容格式如下：
```java
employee.(class)=MyClass       // 等同于：<bean class="MyClass" />
employee.(abstract)=true       // 等同于：<bean abstract="true" />
employee.group=Insurance       // 为属性设置值，等同于：<property name="group" value="Insurance" />
employee.usesDialUp=false      // 为employee这个bean中的usesDialUp属性设置值,等同于：等同于：<property name="usesDialUp" value="false" />

salesrep.(parent)=employee     // 定义了一个id为salesrep的bean，指定父bean为employee，等同于：<bean id="salesrep" parent="employee" />
salesrep.(lazy-init)=true      // 设置延迟初始化，等同于：<bean lazy-init="true" />
salesrep.manager(ref)=tony     // 设置这个bean的manager属性值，是另外一个bean，名称为tony，等同于：<property name="manager" ref="tony" />
salesrep.department=Sales      // 等同于：<property name="department" value="Sales" />

techie.(parent)=employee       // 定义了一个id为techie的bean，指定父bean为employee，等同于：<bean id="techie" parent="employee" />
techie.(scope)=prototype       // 设置bean的作用域，等同于<bean scope="prototype" />
techie.manager(ref)=jeff       // 等同于：<property name="manager" ref="jeff" />
techie.department=Engineering  // <property name="department" value="Engineering" />
techie.usesDialUp=true         // <property name="usesDialUp" value="true" />

ceo.$0(ref)=secretary          // 设置构造函数第1个参数值，等同于：<constructor-arg index="0" ref="secretary" />
ceo.$1=1000000                 // 设置构造函数第2个参数值，等同于：<constructor-arg index="1" value="1000000" />
```
<a name="CnHjn"></a>
### 注解的方式
常见的2种：

1. 类上标注`@Compontent`注解来定义一个bean
2. 配置类中使用`@Bean`注解来定义bean
<a name="zQvPh"></a>
### 小结
**bean注册者只识别**`**BeanDefinition**`**对象，不管什么方式最后都会将这些bean定义的信息转换为**`**BeanDefinition**`**对象，然后注册到Spring容器中。**
<a name="O909L"></a>
## 阶段2：Bean元信息解析阶段
Bean元信息的解析就是将各种方式定义的bean配置信息解析为`BeanDefinition`对象。
<a name="rXRlt"></a>
### Bean元信息的解析主要有3种方式

1. xml文件定义bean的解析
2. properties文件定义bean的解析
3. 注解方式定义bean的解析
<a name="T4Vrk"></a>
### XML方式解析：`XmlBeanDefinitionReader`
Spring中提供了一个类`XmlBeanDefinitionReader`，将xml中定义的bean解析为`BeanDefinition`对象。<br />直接来看案例代码
<a name="ovcD5"></a>
#### 来一个bean xml配置文件
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="car" class="com.javacode2018.lesson002.demo1.Car">
    <property name="name" value="奥迪"/>
  </bean>

  <bean id="car1" class="com.javacode2018.lesson002.demo1.Car">
    <property name="name" value="保时捷"/>
  </bean>

  <bean id="car2" parent="car1"/>

  <bean id="user" class="com.javacode2018.lesson002.demo1.User">
    <property name="name" value="Java"/>
    <property name="car" ref="car1"/>
  </bean>
</beans>
```
上面注册了4个bean，不多解释了。
<a name="uexWt"></a>
#### 将bean xml解析为`BeanDefinition`对象
```java
/**
 * xml方式bean配置信息解析
 */
@Test
public void test1() {
    //定义一个spring容器，这个容器默认实现了BeanDefinitionRegistry，所以本身就是一个bean注册器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    //定义一个xml的BeanDefinition读取器，需要传递一个BeanDefinitionRegistry（bean注册器）对象
    XmlBeanDefinitionReader xmlBeanDefinitionReader = new XmlBeanDefinitionReader(factory);

    //指定bean xml配置文件的位置
    String location = "classpath:/com/javacode2018/lesson002/demo2/beans.xml";
    //通过XmlBeanDefinitionReader加载bean xml文件，然后将解析产生的BeanDefinition注册到容器容器中
    int countBean = xmlBeanDefinitionReader.loadBeanDefinitions(location);
    System.out.println(String.format("共注册了 %s 个bean", countBean));

    //打印出注册的bean的配置信息
    for (String beanName : factory.getBeanDefinitionNames()) {
        //通过名称从容器中获取对应的BeanDefinition信息
        BeanDefinition beanDefinition = factory.getBeanDefinition(beanName);
        //获取BeanDefinition具体使用的是哪个类
        String beanDefinitionClassName = beanDefinition.getClass().getName();
        //通过名称获取bean对象
        Object bean = factory.getBean(beanName);
        //打印输出
        System.out.println(beanName + ":");
        System.out.println("    beanDefinitionClassName：" + beanDefinitionClassName);
        System.out.println("    beanDefinition：" + beanDefinition);
        System.out.println("    bean：" + bean);
    }
}
```
上面注释比较详细，这里就不解释了。<br />注意一点：创建`XmlBeanDefinitionReader`的时候需要传递一个bean注册器(`BeanDefinitionRegistry`)，解析过程中生成的`BeanDefinition`会丢到bean注册器中。
<a name="wiJWS"></a>
#### 运行输出
```
共注册了 4 个bean
car:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.Car]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo2/beans.xml]
    bean：Car{name='奥迪'}
car1:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.Car]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo2/beans.xml]
    bean：Car{name='保时捷'}
car2:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean with parent 'car1': class [null]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo2/beans.xml]
    bean：Car{name='保时捷'}
user:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.User]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo2/beans.xml]
    bean：User{name='Java', car=Car{name='奥迪'}}
```
上面的输出认真看一下，这几个`BeanDefinition`都是`GenericBeanDefinition`这种类型的，也就是说xml中定义的bean被解析之后都是通过`GenericBeanDefinition`这种类型表示的。
<a name="XELgn"></a>
### properties文件定义bean的解析：`PropertiesBeanDefinitionReader`
Spring中提供了一个类`XmlBeanDefinitionReader`，将xml中定义的bean解析为`BeanDefinition`对象，过程和xml的方式类似。<br />来看案例代码。<br />下面通过properties文件的方式实现上面xml方式定义的bean。
<a name="lW3UW"></a>
#### 来个properties文件：beans.properties
```
car.(class)=com.javacode2018.lesson002.demo1.Car
car.name=奥迪

car1.(class)=com.javacode2018.lesson002.demo1.Car
car1.name=保时捷

car2.(parent)=car1

user.(class)=com.javacode2018.lesson002.demo1.User
user.name=Java
user.car(ref)=car
```
<a name="VwrSi"></a>
#### 将bean properties文件解析为`BeanDefinition`对象
```java
/**
 * properties文件方式bean配置信息解析
 */
@Test
public void test2() {
    //定义一个spring容器，这个容器默认实现了BeanDefinitionRegistry，所以本身就是一个bean注册器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    //定义一个properties的BeanDefinition读取器，需要传递一个BeanDefinitionRegistry（bean注册器）对象
    PropertiesBeanDefinitionReader propertiesBeanDefinitionReader = new PropertiesBeanDefinitionReader(factory);

    //指定bean xml配置文件的位置
    String location = "classpath:/com/javacode2018/lesson002/demo2/beans.properties";
    //通过PropertiesBeanDefinitionReader加载bean properties文件，然后将解析产生的BeanDefinition注册到容器容器中
    int countBean = propertiesBeanDefinitionReader.loadBeanDefinitions(location);
    System.out.println(String.format("共注册了 %s 个bean", countBean));

    //打印出注册的bean的配置信息
    for (String beanName : factory.getBeanDefinitionNames()) {
        //通过名称从容器中获取对应的BeanDefinition信息
        BeanDefinition beanDefinition = factory.getBeanDefinition(beanName);
        //获取BeanDefinition具体使用的是哪个类
        String beanDefinitionClassName = beanDefinition.getClass().getName();
        //通过名称获取bean对象
        Object bean = factory.getBean(beanName);
        //打印输出
        System.out.println(beanName + ":");
        System.out.println("    beanDefinitionClassName：" + beanDefinitionClassName);
        System.out.println("    beanDefinition：" + beanDefinition);
        System.out.println("    bean：" + bean);
    }
}
```
<a name="ci5br"></a>
#### 运行输出
```
user:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.User]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：User{name='Java', car=Car{name='奥迪'}}
car1:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.Car]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：Car{name='保时捷'}
car:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo1.Car]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：Car{name='奥迪'}
car2:
    beanDefinitionClassName：org.springframework.beans.factory.support.GenericBeanDefinition
    beanDefinition：Generic bean with parent 'car1': class [null]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：Car{name='保时捷'}
```
输出和xml方式输出基本上一致。<br />properties方式使用起来并不是太方便，所以平时很少看到有人使用。
<a name="F8pbU"></a>
### 注解方式：`PropertiesBeanDefinitionReader`
注解的方式定义的bean，需要使用`PropertiesBeanDefinitionReader`这个类来进行解析，方式也和上面2种方式类似，直接来看案例。
<a name="SpCCt"></a>
#### 通过注解来标注2个类
<a name="C7Tga"></a>
##### Service1
```java
package com.javacode2018.lesson002.demo2;


import org.springframework.beans.factory.config.ConfigurableBeanFactory;
import org.springframework.context.annotation.Lazy;
import org.springframework.context.annotation.Primary;
import org.springframework.context.annotation.Scope;

@Scope(ConfigurableBeanFactory.SCOPE_PROTOTYPE)
@Primary
@Lazy
public class Service1 {
}
```
这个类上面使用了3个注解，这些注解前面都介绍过，可以用来配置bean的信息<br />上面这个bean是个多例的。
<a name="HZ7Bn"></a>
##### Service2
```java
package com.javacode2018.lesson002.demo2;

import org.springframework.beans.factory.annotation.Autowired;

public class Service2 {

    @Autowired
    private Service1 service1; //@1

    @Override
    public String toString() {
        return "Service2{" +
                "service1=" + service1 +
                '}';
    }
}
```
@1：标注了`@Autowired`，说明需要注入这个对象
<a name="fTp7y"></a>
#### 注解定义的bean解析为`BeanDefinition`，如下：
```java
@Test
public void test3() {
    //定义一个spring容器，这个容器默认实现了BeanDefinitionRegistry，所以本身就是一个bean注册器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    //定义一个注解方式的BeanDefinition读取器，需要传递一个BeanDefinitionRegistry（bean注册器）对象
    AnnotatedBeanDefinitionReader annotatedBeanDefinitionReader = new AnnotatedBeanDefinitionReader(factory);

    //通过PropertiesBeanDefinitionReader加载bean properties文件，然后将解析产生的BeanDefinition注册到容器容器中
    annotatedBeanDefinitionReader.register(Service1.class, Service2.class);

    //打印出注册的bean的配置信息
    for (String beanName : new String[]{"service1", "service2"}) {
        //通过名称从容器中获取对应的BeanDefinition信息
        BeanDefinition beanDefinition = factory.getBeanDefinition(beanName);
        //获取BeanDefinition具体使用的是哪个类
        String beanDefinitionClassName = beanDefinition.getClass().getName();
        //通过名称获取bean对象
        Object bean = factory.getBean(beanName);
        //打印输出
        System.out.println(beanName + ":");
        System.out.println("    beanDefinitionClassName：" + beanDefinitionClassName);
        System.out.println("    beanDefinition：" + beanDefinition);
        System.out.println("    bean：" + bean);
    }
}
```
<a name="KrGaF"></a>
#### 运行输出
```
service1:
    beanDefinitionClassName：org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo2.Service1]; scope=prototype; abstract=false; lazyInit=true; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=true; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：com.javacode2018.lesson002.demo2.Service1@21a947fe
service2:
    beanDefinitionClassName：org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo2.Service2]; scope=singleton; abstract=false; lazyInit=null; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：Service2{service1=null}
```
输出中可以看出service1这个bean的`beanDefinition`中`lazyInit`确实为true，primary也为true，scope为prototype，说明类Service1注解上标注3个注解信息被解析之后放在了`beanDefinition`中。<br />**注意下：最后一行中的service1为什么为null，不是标注了**`**@Autowired**`**么？**<br />这个地方提前剧透一下，看不懂的没关系，这篇文章都结束之后，就明白了。<br />调整一下上面的代码，加上下面@1这行代码，如下：
```java
@Test
public void test3() {
    //定义一个spring容器，这个容器默认实现了BeanDefinitionRegistry，所以本身就是一个bean注册器
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    //定义一个注解方式的BeanDefinition读取器，需要传递一个BeanDefinitionRegistry（bean注册器）对象
    AnnotatedBeanDefinitionReader annotatedBeanDefinitionReader = new AnnotatedBeanDefinitionReader(factory);

    //通过PropertiesBeanDefinitionReader加载bean properties文件，然后将解析产生的BeanDefinition注册到容器容器中
    annotatedBeanDefinitionReader.register(Service1.class, Service2.class);

    factory.getBeansOfType(BeanPostProcessor.class).values().forEach(factory::addBeanPostProcessor); // @1
    //打印出注册的bean的配置信息
    for (String beanName : new String[]{"service1", "service2"}) {
        //通过名称从容器中获取对应的BeanDefinition信息
        BeanDefinition beanDefinition = factory.getBeanDefinition(beanName);
        //获取BeanDefinition具体使用的是哪个类
        String beanDefinitionClassName = beanDefinition.getClass().getName();
        //通过名称获取bean对象
        Object bean = factory.getBean(beanName);
        //打印输出
        System.out.println(beanName + ":");
        System.out.println("    beanDefinitionClassName：" + beanDefinitionClassName);
        System.out.println("    beanDefinition：" + beanDefinition);
        System.out.println("    bean：" + bean);
    }
}
```
再次运行一下，最后一行有值了：
```
service1:
    beanDefinitionClassName：org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo2.Service1]; scope=prototype; abstract=false; lazyInit=true; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=true; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：com.javacode2018.lesson002.demo2.Service1@564718df
service2:
    beanDefinitionClassName：org.springframework.beans.factory.annotation.AnnotatedGenericBeanDefinition
    beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo2.Service2]; scope=singleton; abstract=false; lazyInit=null; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
    bean：Service2{service1=com.javacode2018.lesson002.demo2.Service1@52aa2946}
```
**目前进行到第二个阶段了。**
<a name="teDIi"></a>
## 阶段3：Spring Bean注册阶段
bean注册阶段需要用到一个非常重要的接口：`BeanDefinitionRegistry`
<a name="kfU1y"></a>
### Bean注册接口：`BeanDefinitionRegistry`
这个接口中定义了注册bean常用到的一些方法，源码如下：
```java
public interface BeanDefinitionRegistry extends AliasRegistry {

    /**
     * 注册一个新的bean定义
     * beanName：bean的名称
     * beanDefinition：bean定义信息
     */
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
            throws BeanDefinitionStoreException;

    /**
     * 通过bean名称移除已注册的bean
     * beanName：bean名称
     */
    void removeBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;

    /**
     * 通过名称获取bean的定义信息
     * beanName：bean名称
     */
    BeanDefinition getBeanDefinition(String beanName) throws NoSuchBeanDefinitionException;

    /**
     * 查看beanName是否注册过
     */
    boolean containsBeanDefinition(String beanName);

    /**
     * 获取已经定义（注册）的bean名称列表
     */
    String[] getBeanDefinitionNames();

    /**
     * 返回注册器中已注册的bean数量
     */
    int getBeanDefinitionCount();

    /**
     * 确定给定的bean名称或者别名是否已在此注册表中使用
     * beanName：可以是bean名称或者bean的别名
     */
    boolean isBeanNameInUse(String beanName);

}
```
<a name="BaQPY"></a>
### 别名注册接口：`AliasRegistry`
`BeanDefinitionRegistry`接口继承了`AliasRegistry`接口，这个接口中定义了操作bean别名的一些方法，看一下其源码：
```java
public interface AliasRegistry {

    /**
     * 给name指定别名alias
     */
    void registerAlias(String name, String alias);

    /**
     * 从此注册表中删除指定的别名
     */
    void removeAlias(String alias);

    /**
     * 判断name是否作为别名已经被使用了
     */
    boolean isAlias(String name);

    /**
     * 返回name对应的所有别名
     */
    String[] getAliases(String name);

}
```
<a name="XlssP"></a>
### `BeanDefinitionRegistry`唯一实现：`DefaultListableBeanFactory`
Spring中`BeanDefinitionRegistry`接口有一个唯一的实现类：
```
org.springframework.beans.factory.support.DefaultListableBeanFactory
```
大家可能看到有很多类也实现了`BeanDefinitionRegistry`接口，比如经常用到的`AnnotationConfigApplicationContext`，但实际上其内部是转发给了`DefaultListableBeanFactory`进行处理的，所以真正实现这个接口的类是`DefaultListableBeanFactory`。<br />大家再回头看一下开头的几个案例，都使用的是`DefaultListableBeanFactory`作为bean注册器，此时应该可以理解为什么了。<br />下面来个案例演示一下上面常用的一些方法。
<a name="H494m"></a>
### 案例
<a name="r1EQ1"></a>
#### 代码
```java
package com.javacode2018.lesson002.demo3;

import org.junit.Test;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.beans.factory.support.GenericBeanDefinition;

import java.util.Arrays;

/**
 * BeanDefinitionRegistry 案例
 */
public class BeanDefinitionRegistryTest {

    @Test
    public void test1() {
        //创建一个bean工厂，这个默认实现了BeanDefinitionRegistry接口，所以也是一个bean注册器
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        //定义一个bean
        GenericBeanDefinition nameBdf = new GenericBeanDefinition();
        nameBdf.setBeanClass(String.class);
        nameBdf.getConstructorArgumentValues().addIndexedArgumentValue(0, "Java");

        //将bean注册到容器中
        factory.registerBeanDefinition("name", nameBdf);

        //通过名称获取BeanDefinition
        System.out.println(factory.getBeanDefinition("name"));
        //通过名称判断是否注册过BeanDefinition
        System.out.println(factory.containsBeanDefinition("name"));
        //获取所有注册的名称
        System.out.println(Arrays.asList(factory.getBeanDefinitionNames()));
        //获取已注册的BeanDefinition的数量
        System.out.println(factory.getBeanDefinitionCount());
        //判断指定的name是否使用过
        System.out.println(factory.isBeanNameInUse("name"));

        //别名相关方法
        //为name注册2个别名
        factory.registerAlias("name", "alias-name-1");
        factory.registerAlias("name", "alias-name-2");

        //判断alias-name-1是否已被作为别名使用
        System.out.println(factory.isAlias("alias-name-1"));

        //通过名称获取对应的所有别名
        System.out.println(Arrays.asList(factory.getAliases("name")));

        //最后我们再来获取一下这个bean
        System.out.println(factory.getBean("name"));


    }
}
```
<a name="YQ70Y"></a>
#### 运行输出
```
Generic bean: class [java.lang.String]; scope=; abstract=false; lazyInit=null; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null
true
[name]
1
true
true
[alias-name-2, alias-name-1]
Java
```
**下面要介绍的从阶段4到阶段14，也就是从：**`**BeanDefinition**`**合并阶段到Bean初始化完成阶段，都是在调用getBean从容器中获取bean对象的过程中发送的操作，要注意细看了，大家下去了建议去看getBean这个方法的源码，以下过程均来自于这个方法：**
```
org.springframework.beans.factory.support.AbstractBeanFactory#doGetBean
```
<a name="qs2Qc"></a>
## 阶段4：`BeanDefinition`合并阶段
<a name="ogXBt"></a>
### 合并阶段是做什么的？
可能定义bean的时候有父子bean关系，此时子`BeanDefinition`中的信息是不完整的，比如设置属性的时候配置在父`BeanDefinition`中，此时子`BeanDefinition`中是没有这些信息的，需要将子bean的`BeanDefinition`和父bean的`BeanDefinition`进行合并，得到最终的一个`RootBeanDefinition`，合并之后得到的`RootBeanDefinition`包含bean定义的所有信息，包含了从父bean中继继承过来的所有信息，后续bean的所有创建工作就是依靠合并之后`BeanDefinition`来进行的。<br />合并`BeanDefinition`会使用下面这个方法：
```
org.springframework.beans.factory.support.AbstractBeanFactory#getMergedBeanDefinition
```
**bean定义可能存在多级父子关系，合并的时候进进行递归合并，最终得到一个包含完整信息的**`**RootBeanDefinition**`
<a name="uVxw0"></a>
### 案例
<a name="otkox"></a>
#### 来一个普通的类
```java
package com.javacode2018.lesson002.demo4;

public class LessonModel {
    //课程名称
    private String name;
    //课时
    private int lessonCount;
    //描述信息
    private String description;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getLessonCount() {
        return lessonCount;
    }

    public void setLessonCount(int lessonCount) {
        this.lessonCount = lessonCount;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    @Override
    public String toString() {
        return "LessonModel{" +
        "name='" + name + '\'' +
        ", lessonCount=" + lessonCount +
        ", description='" + description + '\'' +
        '}';
    }
}
```
<a name="um90P"></a>
#### 通过xml定义3个具有父子关系的bean
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <bean id="lesson1" class="com.javacode2018.lesson002.demo4.LessonModel"/>

  <bean id="lesson2" parent="lesson1">
    <property name="name" value="spring高手"/>
    <property name="lessonCount" value="100"/>
  </bean>

  <bean id="lesson3" parent="lesson2">
    <property name="description" value="Java"/>
  </bean>

</beans>
```
lesson2相当于lesson1的儿子，lesson3相当于lesson1的孙子。
<a name="S8Ds8"></a>
#### 解析xml注册bean
下面将解析xml，进行bean注册，然后遍历输出bean的名称，解析过程中注册的原始的`BeanDefinition`，合并之后的`BeanDefinition`，以及合并前后`BeanDefinition`中的属性信息
```java
package com.javacode2018.lesson002.demo4;

import org.junit.Test;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.beans.factory.xml.XmlBeanDefinitionReader;

/**
 * BeanDefinition 合并
 */
public class MergedBeanDefinitionTest {
    @Test
    public void test1() {
        //创建bean容器
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
        //创建一个bean xml解析器
        XmlBeanDefinitionReader beanDefinitionReader = new XmlBeanDefinitionReader(factory);
        //解析bean xml，将解析过程中产生的BeanDefinition注册到DefaultListableBeanFactory中
        beanDefinitionReader.loadBeanDefinitions("com/javacode2018/lesson002/demo4/beans.xml");
        //遍历容器中注册的所有bean信息
        for (String beanName : factory.getBeanDefinitionNames()) {
            //通过bean名称获取原始的注册的BeanDefinition信息
            BeanDefinition beanDefinition = factory.getBeanDefinition(beanName);
            //获取合并之后的BeanDefinition信息
            BeanDefinition mergedBeanDefinition = factory.getMergedBeanDefinition(beanName);

            System.out.println(beanName);
            System.out.println("解析xml过程中注册的beanDefinition：" + beanDefinition);
            System.out.println("beanDefinition中的属性信息" + beanDefinition.getPropertyValues());
            System.out.println("合并之后得到的mergedBeanDefinition：" + mergedBeanDefinition);
            System.out.println("mergedBeanDefinition中的属性信息" + mergedBeanDefinition.getPropertyValues());
            System.out.println("---------------------------");
        }
    }
}
```
<a name="HyDk9"></a>
#### 运行输出
```
lesson1
解析xml过程中注册的beanDefinition：Generic bean: class [com.javacode2018.lesson002.demo4.LessonModel]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
beanDefinition中的属性信息PropertyValues: length=0
合并之后得到的mergedBeanDefinition：Root bean: class [com.javacode2018.lesson002.demo4.LessonModel]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
mergedBeanDefinition中的属性信息PropertyValues: length=0
---------------------------
lesson2
解析xml过程中注册的beanDefinition：Generic bean with parent 'lesson1': class [null]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
beanDefinition中的属性信息PropertyValues: length=2; bean property 'name'; bean property 'lessonCount'
合并之后得到的mergedBeanDefinition：Root bean: class [com.javacode2018.lesson002.demo4.LessonModel]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
mergedBeanDefinition中的属性信息PropertyValues: length=2; bean property 'name'; bean property 'lessonCount'
---------------------------
lesson3
解析xml过程中注册的beanDefinition：Generic bean with parent 'lesson2': class [null]; scope=; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
beanDefinition中的属性信息PropertyValues: length=1; bean property 'description'
合并之后得到的mergedBeanDefinition：Root bean: class [com.javacode2018.lesson002.demo4.LessonModel]; scope=singleton; abstract=false; lazyInit=false; autowireMode=0; dependencyCheck=0; autowireCandidate=true; primary=false; factoryBeanName=null; factoryMethodName=null; initMethodName=null; destroyMethodName=null; defined in class path resource [com/javacode2018/lesson002/demo4/beans.xml]
mergedBeanDefinition中的属性信息PropertyValues: length=3; bean property 'name'; bean property 'lessonCount'; bean property 'description'
---------------------------
```
从输出的结果中可以看到，合并之前，`BeanDefinition`是不完整的，比lesson2和lesson3中的class是null，属性信息也不完整，但是合并之后这些信息都完整了。<br />合并之前是`GenericBeanDefinition`类型的，合并之后得到的是`RootBeanDefinition`类型的。<br />获取lesson3合并的`BeanDefinition`时，内部会递归进行合并，先将lesson1和lesson2合并，然后将lesson2再和lesson3合并，最后得到合并之后的BeanDefinition。<br />**后面的阶段将使用合并产生的**`**RootBeanDefinition**`**。**
<a name="dzuOj"></a>
## 阶段5：Bean Class加载阶段
**这个阶段就是将bean的class名称转换为Class类型的对象。**<br />`BeanDefinition`中有个Object类型的字段：beanClass
```java
private volatile Object beanClass;
```
用来表示bean的class对象，通常这个字段的值有2种类型，一种是bean对应的Class类型的对象，另一种是bean对应的Class的完整类名，第一种情况不需要解析，第二种情况：即这个字段是bean的类名的时候，就需要通过类加载器将其转换为一个Class对象。<br />此时会对阶段4中合并产生的`RootBeanDefinition`中的`beanClass`进行解析，将bean的类名转换为Class对象，然后赋值给beanClass字段。<br />源码位置：
```
org.springframework.beans.factory.support.AbstractBeanFactory#resolveBeanClass
```
上面得到了Bean Class对象以及合并之后的BeanDefinition，下面就开始进入实例化这个对象的阶段了。<br />**Bean实例化分为3个阶段：前阶段、实例化阶段、后阶段；下面详解介绍。**
<a name="CYZ9Y"></a>
## 阶段6：Bean实例化阶段
<a name="LUQUD"></a>
### 分2个小的阶段

1. Bean实例化前操作
2. Bean实例化操作
<a name="OGVZQ"></a>
### Bean实例化前操作
先来看一下`DefaultListableBeanFactory`，这个类中有个非常非常重要的字段：
```java
private final List<BeanPostProcessor> beanPostProcessors = new CopyOnWriteArrayList<>();
```
是一个`BeanPostProcessor`类型的集合<br />`**BeanPostProcessor**`**是一个接口，还有很多子接口，这些接口中提供了很多方法，Spring在bean生命周期的不同阶段，会调用上面这个列表中的**`**BeanPostProcessor**`**中的一些方法，来对生命周期进行扩展，bean生命周期中的所有扩展点都是依靠这个集合中的**`**BeanPostProcessor**`**来实现的，所以如果大家想对bean的生命周期进行干预，这块一定要掌握好。**<br />**注意：本文中很多以**`**BeanPostProcessor**`**结尾的，都实现了**`**BeanPostProcessor**`**接口，有些是直接实现的，有些是实现了它的子接口。**<br />Bean实例化之前会调用一段代码：
```java
@Nullable
protected Object applyBeanPostProcessorsBeforeInstantiation(Class<?> beanClass, String beanName) {
    for (BeanPostProcessor bp : getBeanPostProcessors()) {
        if (bp instanceof InstantiationAwareBeanPostProcessor) {
            InstantiationAwareBeanPostProcessor ibp = (InstantiationAwareBeanPostProcessor) bp;
            Object result = ibp.postProcessBeforeInstantiation(beanClass, beanName);
            if (result != null) {
                return result;
            }
        }
    }
    return null;
}
```
这段代码在bean实例化之前给开发者留了个口子，开发者自己可以在这个地方直接去创建一个对象作为bean实例，而跳过Spring内部实例化bean的过程。<br />上面代码中轮询`beanPostProcessors`列表，如果类型是`InstantiationAwareBeanPostProcessor`， 尝试调用`InstantiationAwareBeanPostProcessor#postProcessBeforeInstantiation`获取bean的实例对象，如果能够获取到，那么将返回值作为当前bean的实例，那么Spring自带的实例化bean的过程就被跳过了。<br />`postProcessBeforeInstantiation`方法如下：
```java
default Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) throws BeansException {
    return null;
}
```
这个地方给开发者提供了一个扩展点，允许开发者在这个方法中直接返回bean的一个实例。<br />下面来个案例看一下。
<a name="B5ix8"></a>
#### 案例
```java
package com.javacode2018.lesson002.demo5;

import com.javacode2018.lesson002.demo1.Car;
import org.junit.Test;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor;
import org.springframework.beans.factory.support.AbstractBeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.lang.Nullable;

/**
 * bean初始化前阶段，会调用：{@link org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor#postProcessBeforeInitialization(Object, String)}
 */
public class InstantiationAwareBeanPostProcessorTest {
    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        //添加一个BeanPostProcessor：InstantiationAwareBeanPostProcessor
        factory.addBeanPostProcessor(new InstantiationAwareBeanPostProcessor() { //@1
            @Nullable
            @Override
            public Object postProcessBeforeInstantiation(Class<?> beanClass, String beanName) throws BeansException {
                System.out.println("调用postProcessBeforeInstantiation()");
                //发现类型是Car类型的时候，硬编码创建一个Car对象返回
                if (beanClass == Car.class) {
                    Car car = new Car();
                    car.setName("保时捷");
                    return car;
                }
                return null;
            }
        });

        //定义一个car bean,车名为：奥迪
        AbstractBeanDefinition carBeanDefinition = BeanDefinitionBuilder.
        genericBeanDefinition(Car.class).
        addPropertyValue("name", "奥迪").  //@2
        getBeanDefinition();
        factory.registerBeanDefinition("car", carBeanDefinition);
        //从容器中获取car这个bean的实例，输出
        System.out.println(factory.getBean("car"));

    }
}
```
@1：创建了一个`InstantiationAwareBeanPostProcessor`，丢到了容器中的`BeanPostProcessor`列表中<br />@2：创建了一个car bean，name为奥迪
<a name="bDCWP"></a>
#### 运行输出
```
调用postProcessBeforeInstantiation()
Car{name='保时捷'}
```
bean定义的时候，名称为：奥迪，最后输出的为：保时捷<br />定义和输出不一致的原因是因为在`InstantiationAwareBeanPostProcessor#postProcessBeforeInstantiation`方法中手动创建了一个实例直接返回了，而不是依靠Spring内部去创建这个实例。
<a name="BuIDu"></a>
#### 小结
实际上，在实例化前阶段对bean的创建进行干预的情况，用的非常少，所以大部分bean的创建还会继续走下面的阶段。
<a name="jaFcX"></a>
### Bean实例化操作
<a name="GGOTy"></a>
#### 这个过程可以干什么？
这个过程会通过反射来调用bean的构造器来创建bean的实例。<br />具体需要使用哪个构造器，Spring为开发者提供了一个接口，允许开发者自己来判断用哪个构造器。<br />看一下这块的代码逻辑：
```java
for (BeanPostProcessor bp : getBeanPostProcessors()) {
    if (bp instanceof SmartInstantiationAwareBeanPostProcessor) {
        SmartInstantiationAwareBeanPostProcessor ibp = (SmartInstantiationAwareBeanPostProcessor) bp;
        Constructor<?>[] ctors = ibp.determineCandidateConstructors(beanClass, beanName);
        if (ctors != null) {
            return ctors;
        }
    }
}
```
会调用`SmartInstantiationAwareBeanPostProcessor`接口的`determineCandidateConstructors`方法，这个方法会返回候选的构造器列表，也可以返回空，看一下这个方法的源码：
```java
@Nullable
default Constructor<?>[] determineCandidateConstructors(Class<?> beanClass, String beanName)
throws BeansException {

    return null;
}
```
这个方法有个比较重要的实现类
```
org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor
```
可以将`@Autowired`标注的方法作为候选构造器返回，有兴趣的可以去看一下代码。
<a name="aqEy8"></a>
#### 案例
**下面来个案例，自定义一个注解，当构造器被这个注解标注的时候，让Spring自动选择使用这个构造器创建对象。**
<a name="KEcF2"></a>
##### 自定义一个注解
下面这个注解可以标注在构造器上面，使用这个标注之后，创建bean的时候将使用这个构造器。
```java
package com.javacode2018.lesson002.demo6;

import java.lang.annotation.*;

@Target(ElementType.CONSTRUCTOR)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface MyAutowried {
}
```
<a name="eB7az"></a>
##### 来个普通的类
下面这个类3个构造器，其中一个使用`@MyAutowried`，让其作为bean实例化的方法。
```java
package com.javacode2018.lesson002.demo6;

public class Person {
    private String name;
    private Integer age;

    public Person() {
        System.out.println("调用 Person()");
    }

    @MyAutowried
    public Person(String name) {
        System.out.println("调用 Person(String name)");
        this.name = name;
    }

    public Person(String name, Integer age) {
        System.out.println("调用 Person(String name, int age)");
        this.name = name;
        this.age = age;
    }

    @Override
    public String toString() {
        return "Person{" +
        "name='" + name + '\'' +
        ", age=" + age +
        '}';
    }
}
```
<a name="eDKai"></a>
##### 自定义一个`SmartInstantiationAwareBeanPostProcessor`
代码的逻辑：将`@MyAutowried`标注的构造器列表返回
```java
package com.javacode2018.lesson002.demo6;


import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.SmartInstantiationAwareBeanPostProcessor;
import org.springframework.lang.Nullable;

import java.lang.reflect.Constructor;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class MySmartInstantiationAwareBeanPostProcessor implements SmartInstantiationAwareBeanPostProcessor {
    @Nullable
    @Override
    public Constructor<?>[] determineCandidateConstructors(Class<?> beanClass, String beanName) throws BeansException {
        System.out.println(beanClass);
        System.out.println("调用 MySmartInstantiationAwareBeanPostProcessor.determineCandidateConstructors 方法");
        Constructor<?>[] declaredConstructors = beanClass.getDeclaredConstructors();
        if (declaredConstructors != null) {
            //获取有@MyAutowried注解的构造器列表
            List<Constructor<?>> collect = Arrays.stream(declaredConstructors).
            filter(constructor -> constructor.isAnnotationPresent(MyAutowried.class)).
            collect(Collectors.toList());
            Constructor[] constructors = collect.toArray(new Constructor[collect.size()]);
            return constructors.length != 0 ? constructors : null;
        } else {
            return null;
        }
    }
}
```
<a name="p0zpz"></a>
##### 来个测试用例
```java
package com.javacode2018.lesson002.demo6;

import org.junit.Test;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

/**
 * 通过{@link org.springframework.beans.factory.config.SmartInstantiationAwareBeanPostProcessor#determineCandidateConstructors(Class, String)}来确定使用哪个构造器来创建bean实例
 */
public class SmartInstantiationAwareBeanPostProcessorTest {
    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        //创建一个SmartInstantiationAwareBeanPostProcessor,将其添加到容器中
        factory.addBeanPostProcessor(new MySmartInstantiationAwareBeanPostProcessor());

        factory.registerBeanDefinition("name",
                BeanDefinitionBuilder.
                        genericBeanDefinition(String.class).
                        addConstructorArgValue("Java").
                        getBeanDefinition());

        factory.registerBeanDefinition("age",
                BeanDefinitionBuilder.
                        genericBeanDefinition(Integer.class).
                        addConstructorArgValue(30).
                        getBeanDefinition());

        factory.registerBeanDefinition("person",
                BeanDefinitionBuilder.
                        genericBeanDefinition(Person.class).
                        getBeanDefinition());

        Person person = factory.getBean("person", Person.class);
        System.out.println(person);

    }
}
```
<a name="GHRok"></a>
##### 运行输出
```
class com.javacode2018.lesson002.demo6.Person
调用 MySmartInstantiationAwareBeanPostProcessor.determineCandidateConstructors 方法
class java.lang.String
调用 MySmartInstantiationAwareBeanPostProcessor.determineCandidateConstructors 方法
调用 Person(String name)
Person{name='Java', age=null}
```
从输出中可以看出调用了Person中标注`@MyAutowired`标注的构造器。<br />到目前为止bean实例化阶段结束了，继续进入后面的阶段。
<a name="st7PT"></a>
## 阶段7：合并后的`BeanDefinition`处理
这块的源码如下
```java
protected void applyMergedBeanDefinitionPostProcessors(RootBeanDefinition mbd, Class<?> beanType, String beanName) {
    for (BeanPostProcessor bp : getBeanPostProcessors()) {
        if (bp instanceof MergedBeanDefinitionPostProcessor) {
            MergedBeanDefinitionPostProcessor bdp = (MergedBeanDefinitionPostProcessor) bp;
            bdp.postProcessMergedBeanDefinition(mbd, beanType, beanName);
        }
    }
}
```
会调用`MergedBeanDefinitionPostProcessor`接口的`postProcessMergedBeanDefinition`方法，看一下这个方法的源码：
```
void postProcessMergedBeanDefinition(RootBeanDefinition beanDefinition, Class<?> beanType, String beanName);
```
Spring会轮询`BeanPostProcessor`，依次调用`MergedBeanDefinitionPostProcessor#postProcessMergedBeanDefinition`<br />第一个参数为`beanDefinition`，表示合并之后的`RootBeanDefinition`，可以在这个方法内部对合并之后的`BeanDefinition`进行再次处理<br />`**postProcessMergedBeanDefinition**`**有2个实现类，前面介绍过，用的也比较多，面试的时候也会经常问的：**
```
org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor
在 postProcessMergedBeanDefinition 方法中对 @Autowired、@Value 标注的方法、字段进行缓存

org.springframework.context.annotation.CommonAnnotationBeanPostProcessor
在 postProcessMergedBeanDefinition 方法中对 @Resource 标注的字段、@Resource 标注的方法、 @PostConstruct 标注的字段、 @PreDestroy标注的方法进行缓存
```
<a name="AGAMH"></a>
## 阶段8：Bean属性设置阶段
<a name="ixGZE"></a>
### 属性设置阶段分为3个小的阶段

- 实例化后阶段
- Bean属性赋值前处理
- Bean属性赋值
<a name="uhm95"></a>
### 实例化后阶段
会调用`InstantiationAwareBeanPostProcessor`接口的`postProcessAfterInstantiation`这个方法，调用逻辑如下：<br />看一下具体的调用逻辑如下：
```java
for (BeanPostProcessor bp : getBeanPostProcessors()) {
    if (bp instanceof InstantiationAwareBeanPostProcessor) {
        InstantiationAwareBeanPostProcessor ibp = (InstantiationAwareBeanPostProcessor) bp;
        if (!ibp.postProcessAfterInstantiation(bw.getWrappedInstance(), beanName)) {
            return;
        }
    }
}
```
`postProcessAfterInstantiation`方法返回false的时候，后续的**Bean属性赋值前处理、Bean属性赋值**都会被跳过了。<br />来看一下`postProcessAfterInstantiation`这个方法的定义
```java
default boolean postProcessAfterInstantiation(Object bean, String beanName) throws BeansException {
    return true;
}
```
**来看个案例，案例中返回false，跳过属性的赋值操作。**
<a name="Lm4vY"></a>
#### 案例
<a name="eRLua"></a>
##### 来个类
```java
package com.javacode2018.lesson002.demo7;


public class UserModel {
    private String name;
    private Integer age;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    @Override
    public String toString() {
        return "UserModel{" +
        "name='" + name + '\'' +
        ", age=" + age +
        '}';
    }
}
```
<a name="DAJRF"></a>
##### 测试用例
下面很简单，来注册一个UserModel的bean
```java
package com.javacode2018.lesson002.demo7;


import org.junit.Test;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

/**
 * {@link InstantiationAwareBeanPostProcessor#postProcessAfterInstantiation(java.lang.Object, java.lang.String)}
 * 返回false，可以阻止bean属性的赋值
 */
public class InstantiationAwareBeanPostProcessoryTest1 {

    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        factory.registerBeanDefinition("user1", BeanDefinitionBuilder.
                genericBeanDefinition(UserModel.class).
                addPropertyValue("name", "Java").
                addPropertyValue("age", 30).
                getBeanDefinition());

        factory.registerBeanDefinition("user2", BeanDefinitionBuilder.
                genericBeanDefinition(UserModel.class).
                addPropertyValue("name", "刘德华").
                addPropertyValue("age", 50).
                getBeanDefinition());

        for (String beanName : factory.getBeanDefinitionNames()) {
            System.out.println(String.format("%s->%s", beanName, factory.getBean(beanName)));
        }
    }

}
```
上面定义了2个bean：[user1,user2]，获取之后输出
<a name="uFTZL"></a>
##### 运行输出
```
user1->UserModel{name='Java', age=30}
user2->UserModel{name='刘德华', age=50}
```
此时UserModel中2个属性都是有值的。<br />下面来阻止user1的赋值，对代码进行改造，加入下面代码：
```java
factory.addBeanPostProcessor(new InstantiationAwareBeanPostProcessor() {
    @Override
    public boolean postProcessAfterInstantiation(Object bean, String beanName) throws BeansException {
        if ("user1".equals(beanName)) {
            return false;
        } else {
            return true;
        }
    }
});
```
再次运行测试输出：
```
user1->UserModel{name='null', age=null}
user2->UserModel{name='刘德华', age=50}
```
user1的属性赋值被跳过了。
<a name="ijY7n"></a>
### Bean属性赋值前阶段
这个阶段会调用`InstantiationAwareBeanPostProcessor`接口的`postProcessProperties`方法，调用逻辑：
```java
for (BeanPostProcessor bp : getBeanPostProcessors()) {
    if (bp instanceof InstantiationAwareBeanPostProcessor) {
        InstantiationAwareBeanPostProcessor ibp = (InstantiationAwareBeanPostProcessor) bp;
        PropertyValues pvsToUse = ibp.postProcessProperties(pvs, bw.getWrappedInstance(), beanName);
        if (pvsToUse == null) {
            if (filteredPds == null) {
                filteredPds = filterPropertyDescriptorsForDependencyCheck(bw, mbd.allowCaching);
            }
            pvsToUse = ibp.postProcessPropertyValues(pvs, filteredPds, bw.getWrappedInstance(), beanName);
            if (pvsToUse == null) {
                return;
            }
        }
        pvs = pvsToUse;
    }
}
```
从上面可以看出，如果`InstantiationAwareBeanPostProcessor`中的`postProcessProperties`和`postProcessPropertyValues`都返回空的时候，表示这个bean不需要设置属性，直接返回了，直接进入下一个阶段。<br />来看一下`postProcessProperties`这个方法的定义：
```java
@Nullable
default PropertyValues postProcessProperties(PropertyValues pvs, Object bean, String beanName)
    throws BeansException {

    return null;
}
```
`PropertyValues`中保存了bean实例对象中所有属性值的设置，所以可以在这个这个方法中对`PropertyValues`值进行修改。
<a name="uCEvn"></a>
#### 这个方法有2个比较重要的实现类
<a name="dX2GJ"></a>
##### `AutowiredAnnotationBeanPostProcessor`在这个方法中对`@Autowired`、`@Value`标注的字段、方法注入值。
<a name="vrEK7"></a>
##### `CommonAnnotationBeanPostProcessor`在这个方法中对`@Resource`标注的字段和方法注入值。
**来个案例，在案例中对pvs进行修改。**
<a name="dvSL1"></a>
#### 案例
<a name="g4arZ"></a>
##### 案例代码
```java
@Test
public void test3() {
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    factory.addBeanPostProcessor(new InstantiationAwareBeanPostProcessor() { // @0
        @Nullable
        @Override
        public PropertyValues postProcessProperties(PropertyValues pvs, Object bean, String beanName) throws BeansException {
            if ("user1".equals(beanName)) {
                if (pvs == null) {
                    pvs = new MutablePropertyValues();
                }
                if (pvs instanceof MutablePropertyValues) {
                    MutablePropertyValues mpvs = (MutablePropertyValues) pvs;
                    //将姓名设置为：Java
                    mpvs.add("name", "Java");
                    //将年龄属性的值修改为18
                    mpvs.add("age", 18);
                }
            }
            return null;
        }
    });

    //注意 user1 这个没有给属性设置值
    factory.registerBeanDefinition("user1", BeanDefinitionBuilder.
                                   genericBeanDefinition(UserModel.class).
                                   getBeanDefinition()); //@1

    factory.registerBeanDefinition("user2", BeanDefinitionBuilder.
                                   genericBeanDefinition(UserModel.class).
                                   addPropertyValue("name", "刘德华").
                                   addPropertyValue("age", 50).
                                   getBeanDefinition());

    for (String beanName : factory.getBeanDefinitionNames()) {
        System.out.println(String.format("%s->%s", beanName, factory.getBean(beanName)));
    }
}
```
@1：user1这个bean没有设置属性的值<br />@0：这个实现 `org.springframework.beans.factory.config.InstantiationAwareBeanPostProcessor#postProcessProperties` 方法，在其内部对 user1 这个bean进行属性值信息进行修改。
<a name="Nzyk0"></a>
##### 运行输出
```
user1->UserModel{name='Java', age=18}
user2->UserModel{name='刘德华', age=50}
```
上面过程都ok，进入bean赋值操作
<a name="d0o0m"></a>
### Bean属性赋值阶段
这个过程比较简单了，循环处理`PropertyValues`中的属性值信息，通过反射调用set方法将属性的值设置到bean实例中。<br />`PropertyValues`中的值是通过bean xml中property元素配置的，或者调用`MutablePropertyValues`中add方法设置的值。
<a name="TU3mM"></a>
## 阶段9：Bean初始化阶段
<a name="o475P"></a>
### 这个阶段分为5个小的阶段

- Bean Aware接口回调
- Bean初始化前操作
- Bean初始化操作
- Bean初始化后操作
- Bean初始化完成操作
<a name="We6uO"></a>
### Bean Aware接口回调
这块的源码：
```java
private void invokeAwareMethods(final String beanName, final Object bean) {
    if (bean instanceof Aware) {
        if (bean instanceof BeanNameAware) {
            ((BeanNameAware) bean).setBeanName(beanName);
        }
        if (bean instanceof BeanClassLoaderAware) {
            ClassLoader bcl = getBeanClassLoader();
            if (bcl != null) {
                ((BeanClassLoaderAware) bean).setBeanClassLoader(bcl);
            }
        }
        if (bean instanceof BeanFactoryAware) {
            ((BeanFactoryAware) bean).setBeanFactory(AbstractAutowireCapableBeanFactory.this);
        }
    }
}
```
如果bean实例实现了上面的接口，会按照下面的顺序依次进行调用：
```
BeanNameAware：将bean的名称注入进去
BeanClassLoaderAware：将BeanClassLoader注入进去
BeanFactoryAware：将BeanFactory注入进去
```
来个案例感受一下<br />来个类，实现上面3个接口。
```java
package com.javacode2018.lesson002.demo8;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanClassLoaderAware;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.BeanFactoryAware;
import org.springframework.beans.factory.BeanNameAware;

public class AwareBean implements BeanNameAware, BeanClassLoaderAware, BeanFactoryAware {

    @Override
    public void setBeanName(String name) {
        System.out.println("setBeanName：" + name);
    }

    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        System.out.println("setBeanFactory：" + beanFactory);
    }

    @Override
    public void setBeanClassLoader(ClassLoader classLoader) {
        System.out.println("setBeanClassLoader：" + classLoader);
    }

}
```
来个测试类，创建上面这个对象的的bean
```java
package com.javacode2018.lesson002.demo8;

import org.junit.Test;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

public class InvokeAwareTest {

    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
        factory.registerBeanDefinition("awareBean", BeanDefinitionBuilder.genericBeanDefinition(AwareBean.class).getBeanDefinition());
        //调用getBean方法获取bean，将触发bean的初始化
        factory.getBean("awareBean");
    }
}
```
运行输出
```
setBeanName：awareBean
setBeanClassLoader：sun.misc.Launcher$AppClassLoader@18b4aac2
setBeanFactory：org.springframework.beans.factory.support.DefaultListableBeanFactory@5bb21b69: defining beans [awareBean]; root of factory hierarchy
```
<a name="cH1sX"></a>
### Bean初始化前操作
这个阶段的源码：
```java
@Override
public Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName)
throws BeansException {

    Object result = existingBean;
    for (BeanPostProcessor processor : getBeanPostProcessors()) {
        Object current = processor.postProcessBeforeInitialization(result, beanName);
        if (current == null) {
            return result;
        }
        result = current;
    }
    return result;
}
```
会调用`BeanPostProcessor`的`postProcessBeforeInitialization`方法，若返回null，当前方法将结束。<br />**通常称**`**postProcessBeforeInitialization**`**这个方法为：bean初始化前操作。**<br />这个接口有2个实现类，比较重要：
```
org.springframework.context.support.ApplicationContextAwareProcessor
org.springframework.context.annotation.CommonAnnotationBeanPostProcessor
```
<a name="y3kIO"></a>
#### `ApplicationContextAwareProcessor`注入6个Aware接口对象
如果bean实现了下面的接口，在`ApplicationContextAwareProcessor#postProcessBeforeInitialization`中会依次调用下面接口中的方法，将Aware前缀对应的对象注入到bean实例中。
```
EnvironmentAware：注入Environment对象
EmbeddedValueResolverAware：注入EmbeddedValueResolver对象
ResourceLoaderAware：注入ResourceLoader对象
ApplicationEventPublisherAware：注入ApplicationEventPublisher对象
MessageSourceAware：注入MessageSource对象
ApplicationContextAware：注入ApplicationContext对象
```
从名称上可以看出这个类以`ApplicationContext`开头的，说明这个类只能在`ApplicationContext`环境中使用。
<a name="VQKf6"></a>
#### `CommonAnnotationBeanPostProcessor`调用`@PostConstruct`标注的方法
`CommonAnnotationBeanPostProcessor#postProcessBeforeInitialization`中会调用bean中所有标注`@PostConstruct`注解的方法<br />来个案例，感受一下。
<a name="XsrjC"></a>
#### 案例
<a name="BA8GV"></a>
##### 来个类
下面的类有2个方法标注了`@PostConstruct`，并且实现了上面说的那6个Aware接口。
```java
package com.javacode2018.lesson002.demo9;

import org.springframework.beans.BeansException;
import org.springframework.context.*;
import org.springframework.core.env.Environment;
import org.springframework.core.io.ResourceLoader;
import org.springframework.util.StringValueResolver;

import javax.annotation.PostConstruct;

public class Bean1 implements EnvironmentAware, EmbeddedValueResolverAware, ResourceLoaderAware, ApplicationEventPublisherAware, MessageSourceAware, ApplicationContextAware {

    @PostConstruct
    public void postConstruct1() { //@1
        System.out.println("postConstruct1()");
    }

    @PostConstruct
    public void postConstruct2() { //@2
        System.out.println("postConstruct2()");
    }

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        System.out.println("setApplicationContext:" + applicationContext);
    }

    @Override
    public void setApplicationEventPublisher(ApplicationEventPublisher applicationEventPublisher) {
        System.out.println("setApplicationEventPublisher:" + applicationEventPublisher);
    }

    @Override
    public void setEmbeddedValueResolver(StringValueResolver resolver) {
        System.out.println("setEmbeddedValueResolver:" + resolver);
    }

    @Override
    public void setEnvironment(Environment environment) {
        System.out.println("setEnvironment:" + environment.getClass());
    }

    @Override
    public void setMessageSource(MessageSource messageSource) {
        System.out.println("setMessageSource:" + messageSource);
    }

    @Override
    public void setResourceLoader(ResourceLoader resourceLoader) {
        System.out.println("setResourceLoader:" + resourceLoader);
    }
}
```
<a name="g49qz"></a>
##### 来个测试案例
```java
package com.javacode2018.lesson002.demo9;


import org.junit.Test;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;

public class PostProcessBeforeInitializationTest {

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(Bean1.class);
        context.refresh();
    }
}
```
<a name="WHCCB"></a>
##### 运行输出
```
setEmbeddedValueResolver:org.springframework.beans.factory.config.EmbeddedValueResolver@15b204a1
setResourceLoader:org.springframework.context.annotation.AnnotationConfigApplicationContext@64bf3bbf, started on Sun Apr 05 21:16:00 CST 2020
setApplicationEventPublisher:org.springframework.context.annotation.AnnotationConfigApplicationContext@64bf3bbf, started on Sun Apr 05 21:16:00 CST 2020
setMessageSource:org.springframework.context.annotation.AnnotationConfigApplicationContext@64bf3bbf, started on Sun Apr 05 21:16:00 CST 2020
setApplicationContext:org.springframework.context.annotation.AnnotationConfigApplicationContext@64bf3bbf, started on Sun Apr 05 21:16:00 CST 2020
postConstruct1()
postConstruct2()
```
大家可以去看一下`AnnotationConfigApplicationContext`的源码，其内部会添加很多`BeanPostProcessor`到`DefaultListableBeanFactory`中。
<a name="uAZto"></a>
### Bean初始化阶段
<a name="gVP7C"></a>
#### 2个步骤

1. 调用`InitializingBean`接口的`afterPropertiesSet`方法
2. 调用定义bean的时候指定的初始化方法。
<a name="oj4Q2"></a>
#### 调用`InitializingBean`接口的`afterPropertiesSet`方法
来看一下`InitializingBean`这个接口
```java
public interface InitializingBean {

    void afterPropertiesSet() throws Exception;

}
```
当bean实现了这个接口的时候，会在这个阶段被调用
<a name="qOdla"></a>
#### 调用bean定义的时候指定的初始化方法
**先来看一下如何指定bean的初始化方法，3种方式**
<a name="iFayz"></a>
##### 方式1：xml方式指定初始化方法
```xml
<bean init-method="bean中方法名称"/>
```
<a name="S9zfb"></a>
##### 方式2：`@Bean`的方式指定初始化方法
```java
@Bean(initMethod = "初始化的方法")
```
<a name="xYJ0A"></a>
##### 方式3：api的方式指定初始化方法
```java
this.beanDefinition.setInitMethodName(methodName);
```
初始化方法最终会赋值给下面这个字段
```
org.springframework.beans.factory.support.AbstractBeanDefinition#initMethodName
```
<a name="hT34l"></a>
#### 案例
<a name="G8hIX"></a>
##### 来个类
```java
package com.javacode2018.lesson002.demo10;

import org.springframework.beans.factory.InitializingBean;

public class Service implements InitializingBean{
    public void init() {
        System.out.println("调用init()方法");
    }

    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("调用afterPropertiesSet()");
    }
}
```
<a name="JtZVD"></a>
##### 下面定义Service这个bean，指定init方法为初始化方法
```java
package com.javacode2018.lesson002.demo10;

import org.junit.Test;
import org.springframework.beans.factory.config.BeanDefinition;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

/**
 * 初始化方法测试
 */
public class InitMethodTest {

    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
        BeanDefinition service = BeanDefinitionBuilder.genericBeanDefinition(Service.class).
        setInitMethodName("init"). //@1：指定初始化方法
        getBeanDefinition();

        factory.registerBeanDefinition("service", service);

        System.out.println(factory.getBean("service"));
    }
}
```
<a name="Twcnv"></a>
##### 运行输出
```
调用afterPropertiesSet()
调用init()方法
com.javacode2018.lesson002.demo10.Service@12f41634
```
调用顺序：`InitializingBean`中的`afterPropertiesSet`、然后在调用自定义的初始化方法
<a name="RKyrQ"></a>
### Bean初始化后阶段
这块的源码：
```java
@Override
public Object applyBeanPostProcessorsAfterInitialization(Object existingBean, String beanName)
throws BeansException {

    Object result = existingBean;
    for (BeanPostProcessor processor : getBeanPostProcessors()) {
        Object current = processor.postProcessAfterInitialization(result, beanName);
        if (current == null) {
            return result;
        }
        result = current;
    }
    return result;
}
```
调用`BeanPostProcessor`接口的`postProcessAfterInitialization`方法，返回null的时候，会中断上面的操作。<br />**通常称**`**postProcessAfterInitialization**`**这个方法为：bean初始化后置操作。**<br />来个案例：
```java
package com.javacode2018.lesson002.demo11;

import org.junit.Test;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;
import org.springframework.lang.Nullable;

/**
 * {@link BeanPostProcessor#postProcessAfterInitialization(java.lang.Object, java.lang.String)}
 * bean初始化后置处理
 */
public class PostProcessAfterInitializationTest {

    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        //加入bean初始化后置处理器方法实现
        factory.addBeanPostProcessor(new BeanPostProcessor() {
            @Nullable
            @Override
            public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
                System.out.println("postProcessAfterInitialization：" + beanName);
                return bean;
            }
        });

        //下面注册2个String类型的bean
        factory.registerBeanDefinition("name",
                                       BeanDefinitionBuilder.
                                       genericBeanDefinition(String.class).
                                       addConstructorArgValue("Java").
                                       getBeanDefinition());
        factory.registerBeanDefinition("personInformation",
                                       BeanDefinitionBuilder.genericBeanDefinition(String.class).
                                       addConstructorArgValue("java高手！").
                                       getBeanDefinition());

        System.out.println("-------输出bean信息---------");

        for (String beanName : factory.getBeanDefinitionNames()) {
            System.out.println(String.format("%s->%s", beanName, factory.getBean(beanName)));
        }
    }
}
```
运行输出
```
-------输出bean信息---------
postProcessAfterInitialization：name
name->Java
postProcessAfterInitialization：personInformation
personInformation->java高手！
```
<a name="Fydbp"></a>
## 阶段10：所有单例bean初始化完成后阶段
所有单例bean实例化完成之后，Spring会回调下面这个接口：
```java
public interface SmartInitializingSingleton {
    void afterSingletonsInstantiated();
}
```
调用逻辑在下面这个方法中
```
/**
 * 确保所有非lazy的单例都被实例化，同时考虑到FactoryBeans。如果需要，通常在工厂设置结束时调用。
 */
org.springframework.beans.factory.support.DefaultListableBeanFactory#preInstantiateSingletons
```
这个方法内部会先触发所有非延迟加载的单例bean初始化，然后从容器中找到类型是`SmartInitializingSingleton`的bean，调用他们的`afterSingletonsInstantiated`方法。<br />有兴趣的可以去看一下带有`ApplicationContext`的容器，内部最终都会调用上面这个方法触发所有单例bean的初始化。<br />来个2个案例演示一下`SmartInitializingSingleton`的使用。
<a name="fyOmW"></a>
### 案例1：`ApplicationContext`自动回调`SmartInitializingSingleton`接口
Service1：
```java
package com.javacode2018.lesson002.demo12;

import org.springframework.stereotype.Component;

@Component
public class Service1 {

    public Service1() {
        System.out.println("create " + this.getClass());
    }
}
```
Service2：
```java
package com.javacode2018.lesson002.demo12;

import org.springframework.stereotype.Component;

@Component
public class Service2 {
    public Service2() {
        System.out.println("create " + this.getClass());
    }
}
```
自定义一个`SmartInitializingSingleton`
```java
package com.javacode2018.lesson002.demo12;

import org.springframework.beans.factory.SmartInitializingSingleton;
import org.springframework.stereotype.Component;

@Component
public class MySmartInitializingSingleton implements SmartInitializingSingleton {
    @Override
    public void afterSingletonsInstantiated() {
        System.out.println("所有bean初始化完毕！");
    }
}
```
来个测试类，通过包扫描的方式注册上面3个bean
```java
package com.javacode2018.lesson002.demo12;

import org.junit.Test;
import org.springframework.beans.factory.SmartInitializingSingleton;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.ComponentScan;

/**
 * 所有bean初始化完毕，容器会回调{@link SmartInitializingSingleton#afterSingletonsInstantiated()}
 */
@ComponentScan
public class SmartInitializingSingletonTest {
    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(SmartInitializingSingletonTest.class);
        System.out.println("开始启动容器!");
        context.refresh();
        System.out.println("容器启动完毕!");
    }
}
```
运行输出
```
开始启动容器!
create class com.javacode2018.lesson002.demo12.Service1
create class com.javacode2018.lesson002.demo12.Service2
所有bean初始化完毕！
容器启动完毕!
```
<a name="wCe7Y"></a>
### 案例2：通过api的方式让`DefaultListableBeanFactory`去回调`SmartInitializingSingleton`
```java
@Test
public void test2() {
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
    factory.registerBeanDefinition("service1", BeanDefinitionBuilder.genericBeanDefinition(Service1.class).getBeanDefinition());
    factory.registerBeanDefinition("service2", BeanDefinitionBuilder.genericBeanDefinition(Service2.class).getBeanDefinition());
    factory.registerBeanDefinition("mySmartInitializingSingleton", BeanDefinitionBuilder.genericBeanDefinition(MySmartInitializingSingleton.class).getBeanDefinition());
    System.out.println("准备触发所有单例bean初始化");
    //触发所有bean初始化，并且回调 SmartInitializingSingleton#afterSingletonsInstantiated 方法
    factory.preInstantiateSingletons();
}
```
上面通过api的方式注册bean<br />最后调用`factory.preInstantiateSingletons`触发所有非lazy单例bean初始化，所有bean装配完毕之后，会回调SmartInitializingSingleton接口。
<a name="YPYdV"></a>
## 阶段11：Bean使用阶段
这个阶段就不说了，调用getBean方法得到了bean之后，大家可以随意使用，任意发挥。
<a name="GQJ7S"></a>
## 阶段12：Bean销毁阶段
<a name="kqunB"></a>
### 触发bean销毁的几种方式

1. 调用`org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory#destroyBean`
2. 调用`org.springframework.beans.factory.config.ConfigurableBeanFactory#destroySingletons`
3. 调用`ApplicationContext`中的close方法
<a name="UmX4Z"></a>
### Bean销毁阶段会依次执行

1. 轮询`beanPostProcessors`列表，如果是`DestructionAwareBeanPostProcessor`这种类型的，会调用其内部的`postProcessBeforeDestruction`方法
2. 如果bean实现了`org.springframework.beans.factory.DisposableBean`接口，会调用这个接口中的`destroy`方法
3. 调用bean自定义的销毁方法
<a name="AGacl"></a>
### `DestructionAwareBeanPostProcessor`接口
看一下源码：
```java
public interface DestructionAwareBeanPostProcessor extends BeanPostProcessor {

    /**
     * bean销毁前调用的方法
     */
    void postProcessBeforeDestruction(Object bean, String beanName) throws BeansException;

    /**
     * 用来判断bean是否需要触发postProcessBeforeDestruction方法
     */
    default boolean requiresDestruction(Object bean) {
        return true;
    }

}
```
这个接口有个关键的实现类：
```
org.springframework.context.annotation.CommonAnnotationBeanPostProcessor
```
`**CommonAnnotationBeanPostProcessor#postProcessBeforeDestruction**`**方法中会调用bean中所有标注了**`**@PreDestroy**`**的方法。**
<a name="FKAWF"></a>
### 再来说一下自定义销毁方法有3种方式
<a name="YY184"></a>
#### 方式1：xml中指定销毁方法
```xml
<bean destroy-method="bean中方法名称"/>
```
<a name="dUXLn"></a>
#### 方式2：`@Bean`中指定销毁方法
```java
@Bean(destroyMethod = "初始化的方法")
```
<a name="gNb4V"></a>
#### 方式3：api的方式指定销毁方法
```java
this.beanDefinition.setDestroyMethodName(methodName);
```
初始化方法最终会赋值给下面这个字段
```
org.springframework.beans.factory.support.AbstractBeanDefinition#destroyMethodName
```
下面来看销毁的案例
<a name="zamzQ"></a>
### 案例1：自定义`DestructionAwareBeanPostProcessor`
<a name="V7wXK"></a>
#### 来个类
```java
package com.javacode2018.lesson002.demo13;

public class ServiceA {
    public ServiceA() {
        System.out.println("create " + this.getClass());
    }
}
```
<a name="qooeW"></a>
#### 自定义一个`DestructionAwareBeanPostProcessor`
```java
package com.javacode2018.lesson002.demo13;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.DestructionAwareBeanPostProcessor;

public class MyDestructionAwareBeanPostProcessor implements DestructionAwareBeanPostProcessor {
    @Override
    public void postProcessBeforeDestruction(Object bean, String beanName) throws BeansException {
        System.out.println("准备销毁bean：" + beanName);
    }
}
```
<a name="XD0ZX"></a>
#### 来个测试类
```java
package com.javacode2018.lesson002.demo13;

import org.junit.Test;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.springframework.beans.factory.support.DefaultListableBeanFactory;

/**
 * 自定义 {@link org.springframework.beans.factory.config.DestructionAwareBeanPostProcessor}
 */
public class DestructionAwareBeanPostProcessorTest {

    @Test
    public void test1() {
        DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

        //添加自定义的DestructionAwareBeanPostProcessor
        factory.addBeanPostProcessor(new MyDestructionAwareBeanPostProcessor());

        //向容器中注入3个单例bean
        factory.registerBeanDefinition("serviceA1", BeanDefinitionBuilder.genericBeanDefinition(ServiceA.class).getBeanDefinition());
        factory.registerBeanDefinition("serviceA2", BeanDefinitionBuilder.genericBeanDefinition(ServiceA.class).getBeanDefinition());
        factory.registerBeanDefinition("serviceA3", BeanDefinitionBuilder.genericBeanDefinition(ServiceA.class).getBeanDefinition());

        //触发所有单例bean初始化
        factory.preInstantiateSingletons(); //@1

        System.out.println("销毁serviceA1"); 
        //销毁指定的bean
        factory.destroySingleton("serviceA1");//@2

        System.out.println("触发所有单例bean的销毁");
        factory.destroySingletons();
    }
}
```
上面使用了2种方式来触发bean的销毁[@1和@2]
<a name="LC6xl"></a>
#### 运行输出
```
create class com.javacode2018.lesson002.demo13.ServiceA
create class com.javacode2018.lesson002.demo13.ServiceA
create class com.javacode2018.lesson002.demo13.ServiceA
销毁serviceA1
准备要销毁bean：serviceA1
触发所有单例bean的销毁
准备要销毁bean：serviceA3
准备要销毁bean：serviceA2
```
可以看到`postProcessBeforeDestruction`被调用了3次，依次销毁3个自定义的bean
<a name="PxAjX"></a>
### 案例2：触发`@PreDestroy`标注的方法被调用
上面说了这个注解是在`CommonAnnotationBeanPostProcessor#postProcessBeforeDestruction`中被处理的，所以只需要将这个加入`BeanPostProcessor`列表就可以了。
<a name="O3gk4"></a>
#### 再来个类
```java
package com.javacode2018.lesson002.demo13;

import javax.annotation.PreDestroy;

public class ServiceB {
    public ServiceB() {
        System.out.println("create " + this.getClass());
    }

    @PreDestroy
    public void preDestroy() { //@1
        System.out.println("preDestroy()");
    }
}
```
@1：标注了`@PreDestroy`注解
<a name="YpY41"></a>
#### 测试用例
```java
@Test
public void test2() {
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();

    //添加自定义的DestructionAwareBeanPostProcessor
    factory.addBeanPostProcessor(new MyDestructionAwareBeanPostProcessor()); //@1
    //将CommonAnnotationBeanPostProcessor加入
    factory.addBeanPostProcessor(new CommonAnnotationBeanPostProcessor()); //@2

    //向容器中注入bean
    factory.registerBeanDefinition("serviceB", BeanDefinitionBuilder.genericBeanDefinition(ServiceB.class).getBeanDefinition());

    //触发所有单例bean初始化
    factory.preInstantiateSingletons();

    System.out.println("销毁serviceB");
    //销毁指定的bean
    factory.destroySingleton("serviceB");
}
```
@1：放入了一个自定义的`DestructionAwareBeanPostProcessor`<br />@2：放入了`CommonAnnotationBeanPostProcessor`，这个会处理bean中标注`@PreDestroy`注解的方法
<a name="I3isR"></a>
#### 看效果运行输出
```
create class com.javacode2018.lesson002.demo13.ServiceB
销毁serviceB
准备销毁bean：serviceB
preDestroy()
```
<a name="fTSlL"></a>
### 案例3：看一下销毁阶段的执行顺序
实际上`ApplicationContext`内部已经将Spring内部一些常见的必须的`BeannPostProcessor`自动装配到`beanPostProcessors`列表中，比如熟悉的下面的几个：
```
1.org.springframework.context.annotation.CommonAnnotationBeanPostProcessor
  用来处理@Resource、@PostConstruct、@PreDestroy的
2.org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor
  用来处理@Autowired、@Value注解
3.org.springframework.context.support.ApplicationContextAwareProcessor
  用来回调Bean实现的各种Aware接口
```
所以通过`ApplicationContext`来销毁bean，会触发3中方式的执行。<br />下面就以`AnnotationConfigApplicationContext`来演示一下销毁操作。<br />来一个类
```java
package com.javacode2018.lesson002.demo14;

import org.springframework.beans.factory.DisposableBean;

import javax.annotation.PreDestroy;

public class ServiceA implements DisposableBean {

    public ServiceA() {
        System.out.println("创建ServiceA实例");
    }

    @PreDestroy
    public void preDestroy1() {
        System.out.println("preDestroy1()");
    }

    @PreDestroy
    public void preDestroy2() {
        System.out.println("preDestroy2()");
    }

    @Override
    public void destroy() throws Exception {
        System.out.println("DisposableBean接口中的destroy()");
    }

    //自定义的销毁方法
    public void customDestroyMethod() { //@1
        System.out.println("我是自定义的销毁方法:customDestroyMethod()");
    }
}
```
上面的类中有2个方法标注了`@PreDestroy`<br />这个类实现了`DisposableBean`接口，重写了接口的中的`destroy`方法<br />@1：这个`destroyMethod`一会通过`@Bean`注解的方式，将其指定为自定义方法。<br />来看测试用例
```java
package com.javacode2018.lesson002.demo14;

import org.junit.Test;
import org.springframework.beans.factory.annotation.Configurable;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.context.annotation.Bean;

@Configurable
public class DestroyTest {

    @Bean(destroyMethod = "customDestroyMethod") //@1
    public ServiceA serviceA() {
        return new ServiceA();
    }

    @Test
    public void test1() {
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
        context.register(DestroyTest.class);
        //启动容器
        System.out.println("准备启动容器");
        context.refresh();
        System.out.println("容器启动完毕");
        System.out.println("serviceA：" + context.getBean(ServiceA.class));
        //关闭容器
        System.out.println("准备关闭容器");
        //调用容器的close方法，会触发bean的销毁操作
        context.close(); //@2
        System.out.println("容器关闭完毕");
    }
}
```
上面这个类标注了`@Configuration`，表示是一个配置类，内部有个`@Bean`标注的方法，表示使用这个方法来定义一个bean。<br />@1：通过`destroyMethod`属性将`customDestroyMethod`指定为自定义销毁方法<br />@2：关闭容器，触发bean销毁操作<br />来运行test1，输出
```
准备启动容器
创建ServiceA实例
容器启动完毕
serviceA：com.javacode2018.lesson002.demo14.ServiceA@243c4f91
准备关闭容器
preDestroy1()
preDestroy2()
DisposableBean接口中的destroy()
我是自定义的销毁方法:customDestroyMethod()
容器关闭完毕
```
可以看出销毁方法调用的顺序：

1. `@PreDestroy`标注的所有方法
2. `DisposableBean`接口中的`destroy()`
3. 自定义的销毁方法

下面来说一个非常非常重要的类，打起精神，一定要注意看。
<a name="PBeg8"></a>
## `AbstractApplicationContext`类（非常重要的类）
来看一下UML图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689307985831-30edf7af-d5f7-4fe6-8449-091d6ff73d59.png#averageHue=%23fcfbfa&clientId=ud49c7d06-c45a-4&from=paste&id=u15efb552&originHeight=292&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua26795f5-65c6-492b-b044-b2c5ddf1b5e&title=)
<a name="ZT6MM"></a>
### `BeanFactory`接口
这个已经很熟悉了，Bean工厂的顶层接口
<a name="R75a9"></a>
### `DefaultListableBeanFactory`类
实现了`BeanFactory`接口，可以说这个可以是`BeanFactory`接口真正的唯一实现，内部真正实现了bean生命周期中的所有代码。<br />其他的一些类都是依赖于`DefaultListableBeanFactory`类，将请求转发给`DefaultListableBeanFactory`进行bean的处理的。
<a name="qtC1M"></a>
### 其他3个类
经常用到的就是这3个类：`AnnotationConfigApplicationContext`/`ClassPathXmlApplicationContext`/`FileSystemXmlApplicationContext`这3个类，他们的主要内部的功能是依赖他的父类`AbstractApplicationContext`来实现的，所以主要看`AbstractApplicationContext`这个类。
<a name="ZEOkR"></a>
### `AbstractApplicationContext`类
这个类中有2个比较重要的方法
```java
public abstract ConfigurableListableBeanFactory getBeanFactory() throws IllegalStateException;
protected void registerBeanPostProcessors(ConfigurableListableBeanFactory beanFactory)
```
大家是否注意过使用`AnnotationConfigApplicationContext`的时候，经常调用`reflush`方法，这个方法内部就会调用上面这2个方法。
<a name="eK9fr"></a>
#### 第一个方法：`getBeanFactory()`
返回当前应用上下文中的`ConfigurableListableBeanFactory`，这也是个接口类型的，这个接口有一个唯一的实现类：`DefaultListableBeanFactory`。<br />有没有很熟悉，上面说过：`DefaultListableBeanFactory`是`BeanFactory`真正的唯一实现。<br />应用上线文中就会使用这个`ConfigurableListableBeanFactory`来操作Spring容器。
<a name="vr37K"></a>
#### 第二个方法：`registerBeanPostProcessors`
**说的通俗点：这个方法就是向**`**ConfigurableListableBeanFactory**`**中注册**`**BeanPostProcessor**`**，内容会从Spring容器中获取所有类型的**`**BeanPostProcessor**`**，将其添加到**`**DefaultListableBeanFactory#beanPostProcessors**`**列表中**<br />看一下这个方法的源码：
```java
protected void registerBeanPostProcessors(ConfigurableListableBeanFactory beanFactory) {
    PostProcessorRegistrationDelegate.registerBeanPostProcessors(beanFactory, this);
}
```
会将请求转发给`PostProcessorRegistrationDelegate#registerBeanPostProcessors`。<br />内部比较长，大家可以去看一下源码，这个方法内部主要用到了4个`BeanPostProcessor`类型的List集合。
```java
List<BeanPostProcessor> priorityOrderedPostProcessors = new ArrayList<>();
List<BeanPostProcessor> orderedPostProcessors
List<BeanPostProcessor> nonOrderedPostProcessors;
List<BeanPostProcessor> internalPostProcessors = new ArrayList<>();
```
**先说一下：当到方法的时候，Spring容器中已经完成了所有Bean的注册。**<br />Spring会从容器中找出所有类型的`BeanPostProcessor`列表，然后按照下面的规则将其分别放到上面的4个集合中，上面4个集合中的`BeanPostProcessor`会被依次添加到`DefaultListableBeanFactory#beanPostProcessors`列表中，来看一下4个集合的分别放的是那些`BeanPostProcessor`：
<a name="toCDi"></a>
##### `priorityOrderedPostProcessors`（指定优先级的`BeanPostProcessor`）
实现`org.springframework.core.PriorityOrdered`接口的`BeanPostProcessor`，但是不包含`MergedBeanDefinitionPostProcessor`类型的
<a name="VOn2m"></a>
##### `orderedPostProcessors`（指定了顺序的`BeanPostProcessor`）
标注有@Order注解，或者实现了`org.springframework.core.annotation.Order`接口的`BeanPostProcessor`，但是不包含`MergedBeanDefinitionPostProcessor`类型的
<a name="VPH7j"></a>
##### `nonOrderedPostProcessors`（未指定顺序的`BeanPostProcessor`）
上面2中类型置为以及`MergedBeanDefinitionPostProcessor`之外的
<a name="DnAd6"></a>
##### `internalPostProcessors`
`MergedBeanDefinitionPostProcessor`类型的`BeanPostProcessor`列表。<br />可以去看一下`CommonAnnotationBeanPostProcessor`和`AutowiredAnnotationBeanPostProcessor`，这两个类都实现了`PriorityOrdered`接口，但是他们也实现了`MergedBeanDefinitionPostProcessor`接口，所以最终他们会被丢到`internalPostProcessors`这个集合中，会被放入`BeanPostProcessor`的最后面。
<a name="mBICD"></a>
## Bean生命周期流程图
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1689307985814-5a964d66-b4bc-4ec1-aff8-f96a1ade1822.jpeg#averageHue=%23f2f2f2&clientId=ud49c7d06-c45a-4&from=paste&id=ue2c62ad9&originHeight=2336&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16880701-255a-4b88-9fa3-3dd81654f5d&title=)
