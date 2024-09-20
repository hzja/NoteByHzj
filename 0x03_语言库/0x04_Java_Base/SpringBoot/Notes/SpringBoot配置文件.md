Java SpringBoot
<a name="qlaA2"></a>
## 1、SpringBoot加载配置文件的值
<a name="hG7xq"></a>
### A、application.yml
```yaml
fcant:
  check:
    code: 123456
```
<a name="wguit"></a>
### B、取配置文件值的类

1. 在类上添加`@Component`组件注解
2. 在属性上添加`@Value`注解用于读取配置文件的值并赋值给对应的属性
```java
package com.fcant.springbootexpand.property;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

/**
 * ValueProperty
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 9:23 2019/12/5
 */
@Component
@Data
public class ValueProperty {
    @Value("${fcant.check.code}")
    private String check_code;
}

```
<a name="dSBay"></a>
## 2、读取配置文件的值赋给JavaBean属性
<a name="ujFju"></a>
### A、赋值的JavaBean
在类上添加组件注解`@Component`以及配置属性注解`@ConfigurationProperties(prefix = "fcant")`用于读取配置文件，并指定前缀
```java
package com.fcant.springbootexpand.property;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

/**
 * BeanProperty
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 9:45 2019/12/5
 */
@Data
@Component
@ConfigurationProperties(prefix = "fcant")
public class BeanProperty {

    private String tel;

    private boolean exist;
}

```
<a name="CPCM8"></a>
### B、application.yml
```yaml
fcant:
  tel: 17826260016
  exist: True
```
<a name="soKqe"></a>
## 3、加载外部配置文件读取值
<a name="gcX1z"></a>
### A、新建自定义的配置文件fcant.properties(不能为YMAL文件)
```
fcant.property=Fcant
```
<a name="X8PlO"></a>
### B、读取外部配置文件的Java类-PropertySourceProperty.java
为该类添加`@Component`组件注解，以及加载读取外部配置文件的`@PropertySource("classpath:fcant.properties")`注解，并为之指定类路径的配置文件名称；<br />并在对应的属性上面添加`@Value`注解
```java
package com.fcant.springbootexpand.property;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

/**
 * PropertySourceProperty
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 9:53 2019/12/5
 */
@Data
@Component
@PropertySource("classpath:fcant.properties")
public class PropertySourceProperty {

    @Value("${fcant.property}")
    private String property;
}

```
<a name="cAfoK"></a>
### C、配置文件没有给变量赋值的在Value注解中定义默认值
给配置文件的KEY定义默认值的在KEY后面加英文冒号和默认值
```java
package com.fcant.springbootexpand.property;

import lombok.Data;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

/**
 * PropertySourceProperty
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant
 * @date 9:53 2019/12/5
 */
@Data
@Component
@PropertySource("classpath:fcant.properties")
public class PropertySourceProperty {

    @Value("${fcant.property:default}")
    private String property;
}

```
<a name="SVCYg"></a>
## 4、多环境配置文件以及其加载使用
<a name="gqRRG"></a>
### A、建立多环境配置文件，命名为application-{dev}.yml
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575708120720-14df0830-fe2a-4fac-9534-3804b22c684e.png#height=122&id=YrGca&originHeight=122&originWidth=192&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5174&status=done&style=none&title=&width=192)
<a name="mOKgd"></a>
### B、在主配置文件中指定加载的文件
```yaml
spring:
  profiles:
    active: dev
```
<a name="FMKrV"></a>
### C、激活使用的配置文件会打印日志
```
2019-12-07 16:37:36.170  INFO 15804 --- [  restartedMain] c.f.s.SpringBootExpandApplication        : The following profiles are active: dev
```
<a name="howDy"></a>
### D、可以通过命令行参数指定加载的配置文件
<a name="ZZbTL"></a>
## 5、配置文件加载顺序

- 命令行参数
- 当前环境变量或者系统变量中的“`SPRING_APPLICATION_JSON`”属性
- `ServletConfig` 初始化参数
- `ServletContext` 初始化参数
- “`java:comp/env`”中的JNDI属性
- Java系统变量 “`System.getProperties()`”
- 操作系统环境变量
- Jar包外指定的 application-{profile}.properties
- Jar包内指定的 application-{profile}.properties
- Jar包外的 application.properties
- Jar包内的application.properties
- `@PropertySource`引入的配置
- 默认配置属性 “`SpringApplication.setDefaultProperties`”
