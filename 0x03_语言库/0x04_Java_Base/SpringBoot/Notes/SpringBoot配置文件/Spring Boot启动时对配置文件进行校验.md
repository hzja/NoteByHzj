Java SpringBoot<br />在项目开发过程中，某个功能需要依赖在配置文件中配置的参数。这时候就可能出现下面这种现象：<br />有时候经常出现项目启动了，等到使用某个功能组件的时候出现异常，提示参数未配置或者bean注入失败。<br />那有没有一种方法在项目启动时就对参数进行校验而不是在实际使用的时候再抛出提示呢？<br />当然可以，答案就是使用Spring提供的Java Validation功能，简单实用。<br />Validation不是做参数校验的吗？居然还可以干这种事！
<a name="swIrS"></a>
## 开启启动时参数校验
只需要在创建的配置Properties类增加`Validation`相关配置即可
```java
@Validated
@Data
@ConfigurationProperties(prefix = "app")
@Component
public class AppConfigProperties {
    @NotEmpty(message = "配置文件配置必须要配置[app.id]属性")
    private String id;
}
```
上面的配置就会校验在 application.yml中有没有配置 app.id参数。如果在配置文件中没有该配置，项目启动就会失败，并抛出校验异常。<br />在使用配置文件校验时，必须使用`@configurationproperties`注解，`@value`注解不支持该功能。<br />在需要使用app.id的时候注入配置类即可：
```java
@Autowired
private AppConfigProperties appConfigProperties;
```
这样就可以实现需要的效果，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641428695849-32adeb4f-ba04-4e3a-8a76-3700dbb4f7d7.webp#clientId=u801b83dd-878d-4&from=paste&id=u2ea59950&originHeight=508&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf0ccc9c6-ce44-4815-a1c8-1e167d43639&title=)
<a name="lst9b"></a>
### 支持校验类型
| 校验规则 | 规则说明 |
| --- | --- |
| `@Null` | 限制只能为null |
| `@NotNull` | 限制必须不为null |
| `@AssertFalse` | 限制必须为false |
| `@AssertTrue` | 限制必须为true |
| `@DecimalMax(value)` | 限制必须为一个不大于指定值的数字 |
| `@DecimalMin(value)` | 限制必须为一个不小于指定值的数字 |
| `@Digits(integer,fraction)` | 限制必须为一个小数，且整数部分的位数不能超过integer，小数部分的位数不能超过fraction |
| `@Future` | 限制必须是一个将来的日期 |
| `@Max(value)` | 限制必须为一个不大于指定值的数字 |
| `@Min(value)` | 限制必须为一个不小于指定值的数字 |
| `@Past` | 验证注解的元素值（日期类型）比当前时间早 |
| `@Pattern(value)` | 限制必须符合指定的正则表达式 |
| `@Size(max,min)` | 限制字符长度必须在min到max之间 |
| `@NotEmpty` | 验证注解的元素值不为null且不为空（字符串长度不为0、集合大小不为0） |
| `@NotBlank` | 验证注解的元素值不为空（不为null、去除首位空格后长度为0），不同于`@NotEmpty`，`@NotBlank`只应用于字符串且在比较时会去除字符串的空格 |
| `@Email` | 验证注解的元素值是Email，也可以通过正则表达式和flag指定自定义的email格式 |

Validation 支持如下几种校验，可以满足基本的业务逻辑，当然如果还是满足不了业务逻辑，可以选择定制校验规则。
<a name="W4ZQE"></a>
### 定制校验逻辑

1. 定义校验逻辑规则，实现`org.springframework.validation.Validator`
```java
public class ConfigPropertiesValidator implements Validator {
    @Override
    public boolean supports(Class<?> aClass) {
        return AppConfigProperties.class.isAssignableFrom(aClass);
    }

    @Override
    public void validate(Object o, Errors errors) {
        AppConfigProperties config = (AppConfigProperties) o;
        if(StringUtils.isEmpty(config.getId())){
            errors.rejectValue("id", "app.id.empty", "[app.id] 属性必须要在配置文件配置");
        }else if (config.getId().length() < 5) {
            errors.rejectValue("id", "app.id.short", "[app.id] 属性的长度必须不能小于5");
        }
    }
}
```

1. 使用自定义校验规则就不需要再使用原生的`@NotEmpty`了，将其删除
```java
@Validated
@Data
@ConfigurationProperties(prefix = "app")
@Component
public class AppConfigProperties {
    //    @NotEmpty(message = "配置文件配置必须要配置[app.id]属性")
    private String id;
}
```

1. 注入自定义校验规则
```java
@Bean
public  ConfigPropertiesValidator configurationPropertiesValidator(){
    return new ConfigPropertiesValidator();
}
```
:::danger
注意：这里bean的方法名必须要使用 `configurationPropertiesValidator`，否则启动的时候不会执行该校验
:::

1. 修改app.id配置，观察启动情况

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641428695824-6c6f9aba-d7e1-4d41-83cd-960bc7278e7c.webp#clientId=u801b83dd-878d-4&from=paste&id=ucd25a736&originHeight=650&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7d6a4519-865d-4626-9866-c83a92c64ae&title=)<br />错误信息即为自定义校验的结果。
<a name="A99Wq"></a>
## 小结
通过配置Spring Boot启动校验功能，可以快速的识别参数配置的错误，避免在使用组件的时候才发现问题，可以减少排查问题的工作量，并且在封装自定义starter时可以有更好的体验。很多项目共用部门一个Redis集群，那不同项目之间肯定要进行key隔离，这时候提供一个公共的redis starter，强制必须在使用时指定app.id作为redis的自定义key前缀，提高了程序的健壮性。
