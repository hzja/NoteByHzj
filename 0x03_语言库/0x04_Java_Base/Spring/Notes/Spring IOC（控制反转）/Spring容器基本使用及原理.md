JavaSpring
<a name="PcOW6"></a>
## IOC容器
IOC容器是具有依赖注入功能的容器，负责**对象的实例化、对象的初始化，对象和对象之间依赖关系配置、对象的销毁、对外提供对象的查找**等操作，对象的整个生命周期都是由容器来控制。需要使用的对象都由ioc容器进行管理，不需要再去手动通过new的方式去创建对象，由ioc容器直接组装好，当需要使用的时候直接从ioc容器中直接获取就可以了。<br />**那么Spring ioc容器是如何知道需要管理哪些对象呢？**<br />需要给ioc容器提供一个配置清单，这个配置**支持xml格式**和**java注解的方式**，在配置文件中列出需要让ioc容器管理的对象，以及可以指定让ioc容器如何构建这些对象，当Spring容器启动的时候，就会去加载这个配置文件，然后将这些对象给组装好以供外部访问者使用。<br />这里所说的IOC容器也叫Spring容器。
<a name="st7fX"></a>
## Bean概念
由Spring容器管理的对象统称为Bean对象。Bean就是普通的java对象，和自己new的对象其实是一样的，只是这些对象是由Spring去创建和管理的，需要在配置文件中告诉Spring容器需要创建哪些bean对象，所以需要先在配置文件中定义好需要创建的bean对象，这些配置统称为bean定义配置元数据信息，Spring容器通过读取这些bean配置元数据信息来构建和组装需要的对象。
<a name="rbYQa"></a>
### Spring容器使用步骤

1. 引入Spring相关的maven配置
2. 创建bean配置文件，比如bean xml配置文件
3. 在bean xml文件中定义好需要Spring容器管理的bean对象
4. 创建Spring容器，并给容器指定需要装载的bean配置文件，当Spring容器启动之后，会加载这些配置文件，然后创建好配置文件中定义好的bean对象，将这些对象放在容器中以供使用
5. 通过容器提供的方法获取容器中的对象，然后使用
<a name="A2Co9"></a>
## Spring容器对象
Spring内部提供了很多表示Spring容器的接口和对象，来看看比较常见的几个容器接口和具体的实现类。
<a name="kL4WC"></a>
### BeanFactory接口
```
org.springframework.beans.factory.BeanFactory
```
Spring容器中具有代表性的容器就是`BeanFactory`接口，这个是Spring容器的顶层接口，提供了容器最基本的功能。
<a name="QwrBs"></a>
#### 常用的几个方法
```java
//按bean的id或者别名查找容器中的bean
Object getBean(String name) throws BeansException

//这个是一个泛型方法，按照bean的id或者别名查找指定类型的bean，返回指定类型的bean对象
<T> T getBean(String name, Class<T> requiredType) throws BeansException;

//返回容器中指定类型的bean对象
<T> T getBean(Class<T> requiredType) throws BeansException;

//获取指定类型bean对象的获取器，这个方法比较特别，以后会专门来讲
<T> ObjectProvider<T> getBeanProvider(Class<T> requiredType);
```
<a name="Xqcew"></a>
### `ApplicationContext`接口
```
org.springframework.context.ApplicationContext
```
这个接口继承了BeanFactory接口，所以内部包含了`BeanFactory`所有的功能，并且在其上进行了扩展，增加了很多企业级功能，比如AOP、国际化、事件支持等等。。
<a name="ET813"></a>
#### `ClassPathXmlApplicationContext`类
```
org.springframework.context.support.ClassPathXmlApplicationContext
```
这个类实现了`ApplicationContext`接口，注意一下这个类名称包含了ClassPath Xml，说明这个容器类可以从classpath中加载bean xml配置文件，然后创建xml中配置的bean对象，一会后面的案例就会用到这个类。
<a name="M4lfa"></a>
### `AnnotationConfigApplicationContext`类
```
org.springframework.context.annotation.AnnotationConfigApplicationContext
```
这个类也实现了`ApplicationContext`接口，注意其类名包含了Annotation和config两个单词，上面有说过，bean的定义支持xml的方式和注解的方式，当使用注解的方式定义bean的时候，就需要用到这个容器来装载了，这个容器内部会解析注解来构建构建和管理需要的bean。<br />注解的方式相对于xml方式更方便一些，也是比较推荐的方式，后面会大量使用这种方式，具体会详解。
<a name="MDf4v"></a>
## 案例
来个helloworld来详细看一下spring如何使用。
<a name="oeKbL"></a>
### 创建项目spring-series
使用IDEA创建Maven项目spring-series，项目坐标：
```xml
<groupId>com.javacode2018</groupId>
<artifactId>spring-series</artifactId>
<packaging>pom</packaging>
<version>1.0-SNAPSHOT</version>
```
spring-series项目中创建一个子模块lesson-001。
<a name="EZysI"></a>
#### spring-series/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.javacode2018</groupId>
  <artifactId>spring-series</artifactId>
  <packaging>pom</packaging>
  <version>1.0-SNAPSHOT</version>
  <modules>
    <module>lesson-001</module>
  </modules>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <!-- 配置maven编译的时候采用的编译器版本 -->
    <maven.compiler.compilerVersion>1.8</maven.compiler.compilerVersion>
    <!-- 指定源代码是什么版本的，如果源码和这个版本不符将报错，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-source参数 -->
    <maven.compiler.source>1.8</maven.compiler.source>
    <!-- 该命令用于指定生成的class文件将保证和哪个版本的虚拟机进行兼容，maven中执行编译的时候会用到这个配置，默认是1.5，这个相当于javac命令后面的-target参数 -->
    <maven.compiler.target>1.8</maven.compiler.target>
    <spring.version>5.2.3.RELEASE</spring.version>
  </properties>

  <dependencyManagement>
    <dependencies>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-core</artifactId>
        <version>${spring.version}</version>
      </dependency>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>${spring.version}</version>
      </dependency>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-beans</artifactId>
        <version>${spring.version}</version>
      </dependency>
    </dependencies>
  </dependencyManagement>
</project>
```
目前使用Spring最新的版本5.2.3.RELEASE，需要引入Spring提供的3个构件

1. spring-core
2. spring-context
3. spring-beans
<a name="sAqVY"></a>
#### lesson-001/pom.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <parent>
    <artifactId>spring-series</artifactId>
    <groupId>com.javacode2018</groupId>
    <version>1.0-SNAPSHOT</version>
  </parent>
  <modelVersion>4.0.0</modelVersion>

  <artifactId>lesson-001</artifactId>

  <dependencies>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-core</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-context</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-beans</artifactId>
    </dependency>
  </dependencies>

</project>
```
<a name="y2m6u"></a>
### lesson-001中创建HelloWord类
```java
package com.javacode2018.lesson001.demo1;

public class HelloWorld {
    public void say() {
        System.out.println("hello,spring!");
    }
}
```
HelloWord中创建了一个say方法，里面会输一段文字。
<a name="gw4xl"></a>
### 使用Spring容器
下面通过Spring容器来创建HelloWord对象，并从容器中获取这个对象，然后调用其say方法输出文字。
<a name="Fjg5r"></a>
### 创建bean xml配置文件
新建一个文件，文件路径如下：
```
spring-series\lesson-001\src\main\resources\com\javacode2018\lesson001\demo1\bean.xml
```
bean.xml内容如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans
  http://www.springframework.org/schema/beans/spring-beans-4.3.xsd">

  <!--
  定义一个bean
  id：bean的唯一标识，可以通过这个标识从容器中获取这个bean对象
  clss：这个bean的类型，完整类名称
  -->
  <bean id="helloWorld" class="com.javacode2018.lesson001.demo1.HelloWorld"/>

</beans>
```
上面就是bean的定义文件，每个xml中可以定义多个bean元素，通过bean元素定义需要Spring容器管理的对象，bean元素需指定id和class属性

- id表示这个bean的标识，在容器中需要唯一，可以通过这个id从容器中获取这个对象；
- class用来指定这个bean的完整类名

上面的配置文件中定义了一个helloWorld标识的HellWorld类型的bean对象。
<a name="btBVD"></a>
### 创建测试类
创建一个Client类，如下：
```java
package com.javacode2018.lesson001.demo1;

import org.springframework.context.support.ClassPathXmlApplicationContext;

public class Client {

    public static void main(String[] args) {
        //1.bean配置文件位置
        String beanXml = "classpath:/com/javacode2018/lesson001/demo1/beans.xml";

        //2.创建ClassPathXmlApplicationContext容器，给容器指定需要加载的bean配置文件
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(beanXml);

        //3.从容器中获取需要的bean
        HelloWorld helloWorld = context.getBean("helloWorld", HelloWorld.class);

        //4.使用对象
        helloWorld.say();

    }
}
```
上面main方法中有容器的详细使用步骤，需要先创建容器对象，创建容器的对象的时候需要指定bean xml文件的位置，容器启动之后会加载这些配文件，然后将这些对象构建好。<br />代码中通过容器提供的`getBean`方法从容器中获取了HellWorld对象，第一个参数就是xml中bean的id，第二个参数为bean对应的Class对象。
<a name="Ll3Iv"></a>
### 运行输出
```
hello,spring!
```
