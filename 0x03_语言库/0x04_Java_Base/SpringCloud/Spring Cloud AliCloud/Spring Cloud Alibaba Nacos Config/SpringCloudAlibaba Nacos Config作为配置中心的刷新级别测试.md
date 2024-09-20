Java SpringCloud Nacos
<a name="LMAX9"></a>
## 1、配置对应的dataId的refresh为True，并且使用`@RefreshScope`注解进行刷新
<a name="cJoHc"></a>
### 配置
```yaml
spring:
  cloud:
    nacos:
      username: nacos
      password: nacos
      config:
        server-addr: 127.0.0.1:8848
        namespace: public
        file-extension: yml
        refresh-enabled: true
        shared-configs:
          - dataId: ${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}
            refresh: true
          - dataId: common.${spring.cloud.nacos.config.file-extension}
            refresh: true
```
```yaml
info:
  name: 123456
  age: 18
fcant:
  name: fc
  age: 18
```
<a name="JrkKi"></a>
### 使用
```java
import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

/**
 * ConfigValue
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 16:47 2021/11/10/0010
 */
@Data
@Component
@ConfigurationProperties(prefix = "fcant")
public class ConfigValue {
    @Value("${fcant.name}")
    private String name;
    @Value("${fcant.age}")
    private String age;
}
```
```java
@RestController
@RequestMapping("/version")
@RefreshScope
public class VersionController {

    @Autowired
    ConfigValue configValue;

    @Value("${info.name}")
    private String name;
    
    @GetMapping("/name")
    public String getName() {
        return name + configValue.toString();
    }
}
```
<a name="D9Bhg"></a>
### 结果
```java
前：123456ConfigValue(name=fc, age=18)
后：wdConfigValue(name=fc, age=19)
```
<a name="uTfRq"></a>
### 结论
配置对应的dataId的refresh为True，并且使用`@RefreshScope`注解，对应的配置值和配置类的值及时的刷新了。
<a name="u3zdY"></a>
## 2、配置对应的dataId的refresh为True，但不使用`@RefreshScope`注解进行刷新
<a name="cGIm2"></a>
### 配置中心修改前的值
```yaml
info:
  name: wd
  age: 18
fcant:
  name: fc
  age: 17
```
<a name="Loef0"></a>
### 配置中心修改后的值
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636536050408-48960692-2597-4780-8b8f-3dbbdd23a681.png#clientId=u3b0fd36b-c840-4&from=paste&height=478&id=ub671748b&originHeight=1434&originWidth=1994&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138836&status=done&style=shadow&taskId=u523dc056-02dd-4189-a845-ec72321c44c&title=&width=664.6666666666666)
<a name="wD7xL"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636535767101-4c28d9f2-f105-42b4-a981-1a64b7102bd9.png#clientId=u3b0fd36b-c840-4&from=paste&height=262&id=u5bf7dc0c&originHeight=787&originWidth=1975&originalType=binary&ratio=1&rotation=0&showTitle=false&size=135434&status=done&style=shadow&taskId=u373122b9-9a75-42ae-b32a-6be755bc2f4&title=&width=658.3333333333334)
```java
前：wdConfigValue(name=fc, age=19)
后：wdConfigValue(name=fc name, age=17)
```
<a name="Uu0I7"></a>
### 结论
不使用`@RefreshScope`注解，不能刷新`@Value`注解引用的值，但是配置类的值被更新了。
<a name="DYLIe"></a>
## 3、配置对应的dataId的refresh为False，使用`@RefreshScope`注解进行刷新
<a name="ME6O2"></a>
### 修改前的配置文件
```yaml
info:
  name: wd name
  age: 18
fcant:
  name: fc name
  age: 17
```
<a name="G9HQP"></a>
### 返回值
```yaml
wd nameConfigValue(name=fc name, age=17)
```
<a name="rJX9M"></a>
### 修改后的配置文件
```yaml
info:
  name: wd hello
  age: 18
fcant:
  name: fc hello
  age: 11
```
<a name="Jv1M4"></a>
### 返回值
```yaml
wd nameConfigValue(name=fc name, age=17)
```
<a name="ua3rk"></a>
### 结论
不设置dataId的refresh属性为True，仅使用`@RefreshScope`注解无法拿到最新值。
<a name="gjAyd"></a>
## 实验表格
| dataId的`refresh`属性值 | 是否使用`@RefreshScope`注解 | 配置类是否更新 | `@Value`注解的值是否更新 |
| --- | --- | --- | --- |
| True✅ | ✅ | ✅ | ✅ |
| True✅ | ❌ | ✅ | ❌ |
| False❌ | ✅ | ❌ | ❌ |

<a name="IZdJj"></a>
## 结论

- 当dataId的`refresh`属性值为True时，会自动更新配置类的值。
- 当dataId的`refresh`属性值为True时，使用`@Value`注解引用的值必须使用`@RefreshScope`注解才会自动更新。
- 当dataId的`refresh`属性值为False时，任何值都不会刷新。
