JavaSpringBoot
<a name="mHh4E"></a>
## 报错
> Invalid character found in the request target. The valid characters are defined in RFC 7230 and RFC 3986

<a name="FoyVJ"></a>
## 原因
SpringBoot 2.0.0 以上都采用内置Tomcat8.0以上版本，而tomcat8.0以上版本遵从RFC规范添加了对Url的特殊字符的限制，url中只允许包含英文字母(a-zA-Z)、数字(0-9)、-_.~四个特殊字符以及保留字符( ! * ’ ( ) ; : @ & = + $ , / ? # [ ] ) (262+10+4+18=84)这84个字符，请求中出现了{}大括号或者[]，所以Tomcat报错。设置`RelaxedQueryChars`允许此字符(建议)，设置`requestTargetAllows`选项(Tomcat 8.5中不推荐)。根据Tomcat文档，下面提供一种方法来设置松弛的`QueryChars`属性。
<a name="zeIF7"></a>
## 解决方案
在启动类中添加`ConfigurableServletWebServerFactory` Bean对象。
```java
@SpringBootApplication
@EnableScheduling
@EnableFeignClients
public class ZhAlarmApplication {
    public static void main(String[] args) {
        ApplicationContext context = SpringApplication.run(ZhAlarmApplication.class, args);
        SpringContextUtil.setApplicationContext(context);
    }

    @Bean
    public ConfigurableServletWebServerFactory webServerFactory() {
        TomcatServletWebServerFactory factory = new TomcatServletWebServerFactory();
        factory.addConnectorCustomizers((TomcatConnectorCustomizer) connector -> connector.setProperty("relaxedQueryChars", "|{}[]\\"));
        return factory;
    }
}
```
