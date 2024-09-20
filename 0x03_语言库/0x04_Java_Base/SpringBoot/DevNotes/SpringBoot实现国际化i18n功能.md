Java SpringBoot i18n 国际化
<a name="wdTVC"></a>
## 依赖引入
在Spring Boot的web项目中无需引入其他特殊的配置，默认的web starter中便已经涵盖了所需的基础组件。对应的依赖pom配置如下：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>2.2.2.RELEASE</version>
</dependency>
```
<a name="DDnWu"></a>
## 国际化项目结构
这里的项目主要是对外部的api接口，也就是前后端分离的项目。重点介绍后台的国际化配置，以及在接口使用中怎么根据key获取到对应的国际化文案内容。<br />首先通过整体看一下项目的目录结构：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624009500507-9555e6f2-9462-45ed-94d7-75aa95eb410a.webp#clientId=u432cf929-5994-4&from=paste&id=u8071a216&originHeight=915&originWidth=1080&originalType=url&ratio=3&status=done&style=shadow&taskId=ud9b965c8-db76-462f-92df-9959be7cbbb)
<a name="ZeUb3"></a>
## 配置解析器
在上面引入了对应的依赖之后，首先用来初始化`LocaleResolver`类，该类为默认的解析器，用于设置当前会话的默认国际化语言。
```java
/**
 * 配置国际化语言
 *
 **/
@Configuration
public class LocaleConfig {

    /**
   * 默认解析器 其中locale表示默认语言
   */
    @Bean
    public LocaleResolver localeResolver() {
        SessionLocaleResolver localeResolver = new SessionLocaleResolver();
        localeResolver.setDefaultLocale(Locale.CHINA);
        return localeResolver;
    }
}
```
这里是通过`@Configuration`注解来实例化配置类，在该配置类中通过`@Bean`注解的方法注入了创建的`SessionLocaleResolver`。
<a name="HmmnE"></a>
## 配置拦截器
有了解析器，还需要拦截器来对请求的语言参数进行获取，采用默认的`LocaleChangeInterceptor`作为拦截器来指定切换国际化语言的参数名。比如当请求的url中包含`?lang=zh_CN`表示读取国际化文件`messages_zh_CN.properties`。
```java
/**
 * Web相关配置
 **/
@Configuration
public class WebConfig implements WebMvcConfigurer {

    /**
   * 默认拦截器 其中lang表示切换语言的参数名
   *
   */
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        LocaleChangeInterceptor localeInterceptor = new LocaleChangeInterceptor();
        localeInterceptor.setParamName("lang");
        registry.addInterceptor(localeInterceptor);
    }
}
```
这里通过实现接口`WebMvcConfigurer`的`addInterceptors`方法来完成拦截器的初始化和对应参数的设置。
<a name="yHhuT"></a>
## 国际化文件
在完成了上面的基础配置之后，就需要定义具体的国际化文件，在文件中定义具体要进行国际化的参数项。<br />在Spring Boot中国际化文件的名称默认为messages，我们这里就以messages来定义国际化文件。<br />依次定义`messages.properties`、`messages_en_US.properties`、`messages_zh_CN.properties`、`messages_zh_TW.properties`。<br />其中`messages.properties`表示默认的，里面可以没有值，但必须有这样的一个文件，其他三个分别对应英文、中文、中文繁体。<br />三个文件的内容依次为：
```java
username=zhangsan

username=张三

username=張三
```
这里针对国际化文件的放置位置还是有讲究的，如果采用Spring Boot默认的查找路径，那么直接放在resources顶级目录下即可。<br />但如果想放到其他目录下，比如`statistics/i18n/`目录下，则需要在application.properties中配置如下：
```java
spring.messages.basename=statistics/i18n/messages
```
注意前面是没有斜杠的，表示相对路径。而文件名为messages，也不需要添加properties后缀。用过Spring Boot的都知道它会为咱们处理的。
<a name="zEOcW"></a>
## 工具类
如果是页面获取国际化内容，则此时后台的操作已经算是完事了，就差前端进行对应的js处理和展示了。我们这里如果只提供api的话，针对api内部分内容的国际化，还是需要根据key来获取对应的值的。<br />这里就需要一个工具类，来进行处理：
```java

/**
 * 国际化工具类
 **/
@Component
public class MessageUtils {

    private static MessageSource messageSource;

    public MessageUtils(MessageSource messageSource) {
        MessageUtils.messageSource = messageSource;
    }

    /**
   * 获取单个国际化翻译值
   */
    public static String get(String msgKey) {
        try {
            return messageSource.getMessage(msgKey, null, LocaleContextHolder.getLocale());
        } catch (Exception e) {
            return msgKey;
        }
    }
}
```
主要为了注入`MessageSource`，该工具上通过`@Component`进行了实例化。
<a name="WKOhv"></a>
## 具体使用
准备好以上内容之后，就可以在`Controller`层进行使用了，下面直接看代码。
```java
@RestController
@RequestMapping("/i18n")
public class I18nController {

  @RequestMapping("/user")
  public String getUserName() {
    return MessageUtils.get("username");
  }
}
```
直接定义了请求处理方法，在方法内通过key获得对应的国际化值。
