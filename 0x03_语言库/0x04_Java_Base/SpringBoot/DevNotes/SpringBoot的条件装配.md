Java SpringBoot
<a name="xkVJF"></a>
### 一、介绍
在实际的项目开发中，往往需要根据不同的环境做出不同的配置，例如：在开发环境下，会使用内存数据库以便快速启动服务并进行开发调试，在test环境、生产环境，会使用对应环境的数据库。<br />如果应用程序可以根据自身的环境做一些这样的适配，那么程序开发无疑将更加灵活、高效。<br />在过去的应用程序开发中，常常会将这些环境变量写在某个指定的配置文件中，每次服务器启动的时候，会读取服务器中指定的配置文件，从而实现根据不同的环境，应用程序能做出对应的适配。<br />但是这样的工作，对于运维来说，非常苦逼，尤其是应用程序到达50个以上的时候，会非常不好维护，每次上线改配置，全靠人肉，想想都觉得反人类～<br />在使用SpringBoot来开发应用程序的时候，这些工作量将大大简化。<br />SpringBoot为开发者提供了三种可选的条件装配方式。

- Profile
- Conditional
- ConditionalOnProperty

下面了解一下具体的应用实践。
<a name="N4wkD"></a>
### 二、程序实践
<a name="grj7x"></a>
#### 2.1、Profile
SpringBoot 为应用程序提供了Profile这一概念，用来表示不同的环境。例如，分别定义开发、测试和生产这3个环境

- dev：开发环境
- test：测试环境
- production：生产环境

以上传文件为例，在开发环境下，将文件上传到本地，而在测试环境、生产环境，将文件上传到云端服务商。<br />1、首先编写两套上传服务
```java
/**
 * 上传文件到本地
 * @since 2021-06-13
 */
public class FileUploader implements Uploader {

    @Override
    public String upload(File file) {
        //上传文件到本地,并返回绝对路径
        return null;
    }
}

/**
 * 上传文件到OSS
 * @since 2021-06-13
 */
public class OSSUploader implements Uploader {

    @Override
    public String upload(File file) {
        //上传文件到云端,并返回绝对路径
        return null;
    }
}
```
2、然后编写一个服务配置类，根据不同的环境，创建不同的实现类
```java
@Configuration
public class AppConfig {

    @Bean
    @Profile("dev")
    public Uploader initFileUploader() {
        System.out.println("初始化一个上传到本地的bean");
        return new FileUploader();
    }

    @Bean
    @Profile("!dev")
    public Uploader initOSSUploader() {
        System.out.println("初始化一个上传到云端的bean");
        return new OSSUploader();
    }

}
```
3、最后，运行程序<br />在运行程序时，加上JVM参数`-Dspring.profiles.active=dev`就可以指定以`dev`环境启动。<br />如果当前的`Profile`设置为`dev`，则`Spring`容器会调用`initFileUploader()`创建`FileUploader`，否则，调用`initOSSUploader()`创建`OSSUploader`。<br />注意：`@Profile("!dev")`表示非`dev`环境。<br />当然，还可以在`application.properties`文件中加上如下配置，一样可以指定环境进行运行。
```java
spring.profiles.active=dev
```
<a name="U2gRA"></a>
#### 2.2、Conditional
除了可以根据`@Profile`条件来决定是否创建某个`Bean`外，Spring还可以根据`@Conditional`决定是否创建某个`Bean`。<br />以发短信为例，在生产环境，会提供发短信服务，而在其他环境，不会向运营商发短信。<br />1、创建一个条件配置类`SMSEnvCondition`
```java
public class SMSEnvCondition implements Condition {

    @Override
    public boolean matches(ConditionContext context, AnnotatedTypeMetadata metadata) {
        return "true".equalsIgnoreCase(context.getEnvironment().getProperty("enable.sms"));
    }
}
```
2、创建一个发短信的服务
```java
@Component
@Conditional(SMSEnvCondition.class)
public class SendMessageService {

    //...
}
```
3、在`application.properties`文件中，添加配置变量`enable.sms`
```java
enable.sms=true
```
当`enable.sms`为`true`的时候，会创建`SendMessageService`对象，否则不创建。
<a name="fiXpc"></a>
#### 2.3、ConditionalOnProperty
Spring提供的条件装配`@Conditional`，灵活性非常强，但是具体判断逻辑还需要自己实现，比较麻烦。<br />实际上，Spring Boot为开发者提供了很多使用起来更简单的条件注解，例如：

- `ConditionalOnProperty`：如果有指定的配置，条件生效
- `ConditionalOnBean`：如果有指定的Bean，条件生效
- `ConditionalOnMissingBean`：如果没有指定的Bean，条件生效
- `ConditionalOnMissingClass`：如果没有指定的Class，条件生效
- `ConditionalOnWebApplication`：在Web环境中条件生效
- `ConditionalOnExpression`：根据表达式判断条件是否生效

以最常用的`@ConditionalOnProperty`注解为例，将上面的代码改成如下方式即可实现按照条件进行加载。
```java
@Component
@ConditionalOnProperty(name="enable.sms", havingValue="true")
public class SendMessageService {

    //...
}
```
当`enable.sms`的值等于`true`时，会实例化`SendMessageService`对象；反之，不会创建对象。<br />是不是超级简单～～～<br />当然`@ConditionalOnProperty`的参数还不仅仅限于此，以上面上传文件为例，在开发环境，总是上传到本地；在测试环境、生产环境，将文件上传到云端，改造过程如下：
```java
@Component
@ConditionalOnProperty(name = "file.storage", havingValue = "file", matchIfMissing = true)
public class FileUploader implements Uploader {

    @Override
    public String upload(File file) {
        //上传文件到本地,并返回绝对路径
        return null;
    }
}

@Component
@ConditionalOnProperty(name = "file.storage", havingValue = "oss")
public class OSSUploader implements Uploader {


    @Override
    public String upload(File file) {
        //上传文件到云端,并返回绝对路径
        return null;
    }
}
```
当`file.storage`配置值为`file`，会加载`FileUploader`类；当`file.storage`配置值为`oss`，会加载`OSSUploader`类。<br />其中`@ConditionalOnProperty`中的`matchIfMissing`参数表示，当没有找到对应配置参数时，会默认加载当前类，也就是`FileUploader`类。
<a name="ziByN"></a>
### 三、小结
虽然，`@Profile`、`@Conditional`、`@ConditionalOnProperty`三个注解都能实现按照条件进行适配，但是`@Profile`注解控制比较粗糙，很难实现精细化控制。<br />在实际的使用过程中，使用最多的是`@Conditional`、`@ConditionalOnProperty`，可以很灵活的实现条件装配。<br />其中，`@ConditionalOnProperty`是`@Conditional`的一种具体扩展实现，提供了很多非常实用的操作，在使用中，推荐使用`@ConditionalOnProperty`。<br />如果不够，可以根据`@Conditional`条件装配，编写一套控制开关实现类。
