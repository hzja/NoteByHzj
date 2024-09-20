Java SpringBoot @Import<br />`@Import`注解算是SpringBoot自动配置原理中一个很重要的注解
<a name="pdCNx"></a>
## 认识`@Import`注解
👀先看一下源码
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface Import {

 /**
  * {@link Configuration}, {@link ImportSelector}, {@link ImportBeanDefinitionRegistrar}
  * or regular component classes to import.
  */
 Class<?>[] value();

}
```
`@Import`表示要导入的一个或多个`@Configuration`类<br />看注释value通常是一个普通的组件，`Configuration`，`ImportSelector`，`ImportBeanDefinitionRegistrar`。
<a name="t54Oz"></a>
## 搭建项目结构用于测试
<a name="ObaPo"></a>
### 1、先创建一个maven工程
<a name="IuCQg"></a>
### 2、加入依赖
```xml
<dependencies>
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>1.18.18</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>4.3.24.RELEASE</version>
    </dependency>
</dependencies>
```
<a name="kMHvc"></a>
### 3、编写测试代码
Tom.java
```java
@Data
public class Tom {
    private String color;
    private Integer age;
}
```
AppConfig.java
```java
@Configuration
public class AppConfig {
}
```
App.java
```java
public class App {
    public static void main(String[] args) {
        // 创建容器
        AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext(AppConfig.class);
        // 获取bean
        Tom tom = context.getBean(Tom.class);
        System.out.println(tom);
    }
}
```
<a name="jjAwn"></a>
## `@Import`用法
<a name="bET3v"></a>
### 第一种普通组件
`@Import({要导入的容器中的组件})`<br />CustomConfiguration.java
```java
@Configuration
public class CustomConfiguration {
    @Bean
    public Tom tom() {
        Tom tom = new Tom();
        tom.setAge(11);
        tom.setColor("红色");
        return tom;
    }
}
```
将AppConfig.java加上`@Import({ CustomConfiguration.class })`
```java
@Import({ CustomConfiguration.class })
@Configuration
public class AppConfig {
}
```
这个时候运行App已经可以获取到Tom
<a name="siSp8"></a>
### 第二种`ImportSelector`
CustomSelector.java
```java
public class CustomSelector implements ImportSelector {

    @Override
    public String[] selectImports(AnnotationMetadata importingClassMetadata) {
        return new String[] { "com.moli.pojo.Tom" };
    }
}
```
将AppConfig.java加上`@Import({ CustomSelector.class })`
```java
@Import({ CustomSelector.class })
@Configuration
public class AppConfig {
}
```
这个时候运行App已经可以获取到Tom
<a name="eys2k"></a>
### 第三种`ImportBeanDefinitionRegistrar`
CustomRegistrar.java
```java
public class CustomRegistrar implements ImportBeanDefinitionRegistrar {
    @Override
    public void registerBeanDefinitions(AnnotationMetadata importingClassMetadata, BeanDefinitionRegistry registry) {
        BeanDefinitionBuilder builder = BeanDefinitionBuilder.genericBeanDefinition(Tom.class);
        builder.setScope(BeanDefinition.SCOPE_SINGLETON);
        builder.addPropertyValue("color", "灰色");
        builder.addPropertyValue("age", "12");
        registry.registerBeanDefinition("tom", builder.getBeanDefinition());
    }
}
```
将AppConfig.java加上`@Import({ CustomRegistrar.class })`
```java
@Import({ CustomRegistrar.class })
@Configuration
public class AppConfig {
}
```
这个时候运行App已经可以获取到Tom
<a name="lrXVM"></a>
## 最佳搭档 - `@Import`通用形式
在开源软件中大部分`@Import`中是伴随着一个`@EnableXXX`使用的
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Import({ CustomRegistrar.class })
public @interface EnableTom {
}
```
将AppConfig.java加上`@EnableTom`
```java
@EnableTom
@Configuration
public class AppConfig {
}
```
其实SpringBoot的自动配置就是`@Import`的一个应用
```java
@SuppressWarnings("deprecation")
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@AutoConfigurationPackage
@Import(EnableAutoConfigurationImportSelector.class)
public @interface EnableAutoConfiguration {
    ...
}
```
<a name="qhSSU"></a>
## 总结
直白一点使用`@Import`注解可以快速把实例加入Spring的IOC容器中<br />其实这三种用法也算是一种用法，就是导入一个类到容器，容器根据类的注解类型等信息对不同类进行不同处理；<br />方法一：导入一个`Configuration`组件，组件创建了一个Tom类的Bean；<br />方法二：`CustomSelector`返回的类的全限定类名，实际上要导入到容器中的组件全类名；<br />方法三：`ImportBeanDefinitionRegistrar`类似于第二种`ImportSelector`用法，只不过这种用法能自定义化注册，往容器内注入一个`BeanDefinition`，然后`BeanDeiniton`在容器内转为一个实例bean。
