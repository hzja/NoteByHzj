JavaSpringBoot
<a name="yfcEJ"></a>
## 场景
假设现在场景如下：写了一个加密组件 daniel-encrypt，需要写一个starter，别人引用starter就直接可以开始使用组件了，配置文件中添加 `config.Daniel.enable=true` 即可生效。<br />加密项目 daniel-encrypt 中的加密类如下。
```java
package com.daniel.encrypt;
public class DanielEncrypt {
    public static String encrypt(String plainText) {
        // 实现加密逻辑
        return "encrypted " + plainText;
    }
}
```
<a name="KPF27"></a>
## 自定义 starter
<a name="Xedyk"></a>
### 1、添加依赖
首先，创建一个名为 daniel-encrypt-spring-boot-starter 的 Spring Boot starter 项目，然后在pom.xml文件中添加以下依赖项：
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
        <version>2.6.3</version>
    </dependency>

    <dependency>
        <groupId>com.daniel.encrypt</groupId>
        <artifactId>daniel-encrypt</artifactId>
        <version>1.0.0</version>
    </dependency>
</dependencies>
```
<a name="MFVLQ"></a>
### 2、创建自动配置类
在 autoconfigure目录下创建一个名为 `DanielEncryptAutoConfiguration` 的自动配置类。这个类的作用是在自动装配后，自动注册我们需要的功能类。
```java
@Configuration
@EnableConfigurationProperties(DanielEncryptProperties.class)
@ConditionalOnProperty(prefix = "config.Daniel", name = "enable", havingValue = "true")
public class DanielEncryptAutoConfiguration {

    private final DanielEncryptProperties properties;

    public DanielEncryptAutoConfiguration(DanielEncryptProperties properties) {
        this.properties = properties;
    }

    @Bean
    public DanielEncrypt danielEncrypt() {
        return new DanielEncrypt();
    }
}
```
<a name="DIGBh"></a>
### 3、创建属性类
在 autoconfigure 目录下创建一个名为 DanielEncryptProperties 的属性类。这个类的作用是让 Spring Boot 可以解析配置文件中的配置，给属性类赋值，以实现配置控制组件是否生效。
```java
package com.daniel.encrypt.autoconfigure;

import org.springframework.boot.context.properties.ConfigurationProperties;

@ConfigurationProperties("config.Daniel")
public class DanielEncryptProperties {

    private boolean enable;

    public boolean isEnable() {
        return enable;
    }

    public void setEnable(boolean enable) {
        this.enable = enable;
    }
}
```
<a name="HJ9nL"></a>
### 4、配置 spring.factories
在src/main/resources/META-INF目录下创建一个名为spring.factories的文件，并添加以下内容。
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
com.daniel.encrypt.autoconfigure.DanielEncryptAutoConfiguration
```
这样，Spring 在自动装配的时候才会自动去加载配置类。
<a name="gcRje"></a>
### 5、引入自定义 starter
最后，可以在另一个Spring Boot项目中使用自定义的starter，只需要在pom.xml文件中添加以下依赖项：
```xml
<dependency>
  <groupId>com.daniel.encrypt</groupId>
  <artifactId>daniel-encrypt-spring-boot-starter</artifactId>
  <version>1.0.0</version>
</dependency>
```
配置文件中记得添加 config.Daniel.enable=true，这样自定义的starter就可以开始使用了！
