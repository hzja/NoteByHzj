JavaSpringBoot<br />再编写 starter 之前先了解一下什么是 starter，一个 starter 其实就是对一个功能的集成封装，然后对外提供一个依赖，让业务去使用，像熟悉的 Redis，mongo，mybatis 等。另外由于任何人都可以编写自己的 starter，那么为了区分官方的 starter 和个人的 starter，通常在命名上面会有一个规范。所以 SpringBoot 官方提出，第三方在建立自己的 Starter 的时候命名规则统一用xxx-spring-boot-starter，而官方提供的 Starter 统一命名方式为spring-boot-starter-xxx。<br />知道自动装配首先要有一个配置类，其次还要有 spring.factories 文件，所以这两步是必不可少的。接下来就实际操作一下。
<a name="ngEp3"></a>
## 编写配置类
编写配置类首先要添加一个自动装配的依赖，然后再编写对应的配置类和业务实现类，在 pom 中添加如下
<a name="WbmZ6"></a>
### 依赖
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-autoconfigure</artifactId>
	<version>2.7.1</version>
</dependency>
```
<a name="hroKs"></a>
### 装配类
```java
package com.example.hash.starter.config;

import com.example.hash.starter.service.MyHashTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.autoconfigure.condition.ConditionalOnClass;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@ConditionalOnClass({MyHashTemplate.class})
@EnableConfigurationProperties(MyHashProperties.class)
public class MyHashAutoConfiguration {

	@Autowired
	MyHashProperties myHashProperties;

	@Bean
	@ConditionalOnMissingBean(MyHashTemplate.class)
	public MyHashTemplate myJsonService() {
		MyHashTemplate myHashTemplate = new MyHashTemplate();
		myHashTemplate.setPrefix(myHashProperties.getHashPre());
		return myHashTemplate;
	}
}
```
<a name="eXPGA"></a>
### 属性类
```java
package com.example.hash.starter.config;

import org.springframework.boot.context.properties.ConfigurationProperties;

@ConfigurationProperties(prefix = "ziyou.hash")
public class MyHashProperties {
	private String hashPre;

	public String getHashPre() {
		return hashPre;
	}

	public void setHashPre(String hashPre) {
		this.hashPre = hashPre;
	}
}
```
<a name="ueu1s"></a>
### 业务实现类
```java
package com.example.hash.starter.service;

import javax.xml.bind.DatatypeConverter;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class MyHashTemplate {

	private String prefix;

	public String myHash(String origin) {
		if (null == origin || origin.length() == 0) {
			return null;
		}
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(origin.getBytes());
			byte[] digest = md.digest();
			return this.prefix + ":" + DatatypeConverter.printHexBinary(digest).toUpperCase();
		} catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}

	public void setPrefix(String prefix) {
		this.prefix = prefix;
	}
}
```
简单说明一下上面三个类都是干什么的，MyHashTemplate 该类是实际业务需要注入的类，用来对入参进行 MD5 摘要，然后返回一个拼接了前缀的字符串给业务。这个前缀是通过 application.properties 中配置 `ziyou.hash.hashPre=JavaGeekTech666` 配置后传递过来的。MyHashProperties 是接受属性值的类，MyHashAutoConfiguration 则是自动装配类，这个类会根据条件进行 MyHashTemplate  Bean 的初始化，并将前缀进行赋值。
<a name="WTdj7"></a>
## 增加配置文件
最后还需要在 resource 文件中编写一个 META-INF/spring.factories 文件，内容如下
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.example.hash.starter.config.MyHashAutoConfiguration
```
前面的 Key 是固定写法，后面的 value 就是配置类的全路径引用地址。
<a name="I840c"></a>
## 在项目中使用
编写完了 starter 过后，再创建一个新的 web 应用，在其中增加编写的 starter 来验证是否成功。第一步在 pom 文件中增加依赖
```xml
<dependency>
	<groupId>com.starter.example</groupId>
	<artifactId>myhash-spring-boot-starter</artifactId>
	<version>0.0.1-SNAPSHOT</version>
</dependency>
```
```java
package com.example.demo.controller;

import com.example.demo.service.HelloService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {
	
	@Autowired
	private HelloService helloService;
	
	@GetMapping(value = "/hello")
	public String hello(@RequestParam("name") String name) {
		return helloService.sayHello(name);
	}
}
```
```java
package com.example.demo.service;

import com.example.hash.starter.service.MyHashTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class HelloService {
	@Autowired
	private MyHashTemplate myHashTemplate;
	
	public String sayHello(String name) {
		return myHashTemplate.myHash(name);
	}
}
```
在 application.properties 文件中增加如下配置
```
ziyou.hash.hashPre=JavaGeekTech
```
启动项目，访问地址 http://127.0.0.1:8080/hello?name=ziyou 可以看到效果如下。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660178475526-401e2d1b-ce5b-4d44-ba40-62fdc420e444.jpeg#clientId=uca3349e7-76db-4&from=paste&id=u828c8fe8&originHeight=277&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc22dd2ee-b2b1-4ebf-9480-b991ed8a44e&title=)<br />至此可以看到，自己编写的 starter 已经成功生效了，只不过功能很简单而已，完全可以根据自己需要的实际功能来实现一个复杂一点的 starter 来提供开箱即用的效果。<br />在一些大公司特别是一些中间件或者中台团队，很多时候都是提供各种工具的 starter 来给业务团队使用的，毕竟很多重复的功能如果每个业务团队都自己开发的话，完全是浪费资源，通过中台团队统一封装 starter，让各个业务团队拿来就用快速搭建业务即可。
