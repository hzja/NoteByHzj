JavaSpringBoot
<a name="OdhlG"></a>
## 第一种Jasypt加密
<a name="hYgLu"></a>
### Maven依赖
不同的SpringBoot版本引入的jasypt版本不同<br />SpringBoot版本号1依赖
```xml
<!-- Jasypt加密 -->
<dependency>
    <groupId>com.github.ulisesbocchio</groupId>
    <artifactId>jasypt-spring-boot-starter</artifactId>
    <version>2.0.0</version>
</dependency>
```
SpringBoot版本号2依赖
```xml
<dependency>
	<groupId>com.github.ulisesbocchio</groupId>
	<artifactId>jasypt-spring-boot-starter</artifactId>
	<version>3.0.3</version>
</dependency>
```
<a name="Mr8dA"></a>
### 在配置中心增加相关配置
```
jasypt.encryptor.password=henghe #加密密钥 
```
或者<br />为了防止salt(盐)泄露，反解出密码。可以在项目部署的时候使用命令传入salt(盐)值:
```bash
java -jar xxx.jar -Djasypt.encryptor.password=henghe
```
<a name="nsNiW"></a>
### 将application.yml或配置中心中重要的配置改为加密格式
```yaml
 password: ENC(tnJpIM6iACWO/wRI//7XsbBqy/Mpx6MG1hXe4S7U4cNWmGhajnUSeXmBmQiniKEU)
```
SpringBoot程序在启动会自动检测appliction.yml或者配置中心中有ENC(xx)的配置并做解密
<a name="vOsI5"></a>
### 加解密的测试类
```java
import org.jasypt.encryption.pbe.PooledPBEStringEncryptor;
import org.jasypt.encryption.pbe.StandardPBEByteEncryptor;
import org.jasypt.encryption.pbe.config.SimpleStringPBEConfig;

/**
 * @Copyright (C), 2018-2018
 * @Descripition : Jasypt安全框架加密类工具包
 */
public class JasyptUtils {

    /**
     * Jasypt生成加密结果
     *
     * @param password 配置文件中设定的加密密码 jasypt.encryptor.password
     * @param value    待加密值
     * @return
     */
    public static String encryptPwd(String password, String value) {
        PooledPBEStringEncryptor encryptOr = new PooledPBEStringEncryptor();
        encryptOr.setConfig(cryptOr(password));
        String result = encryptOr.encrypt(value);
        return result;
    }

    /**
     * 解密
     *
     * @param password 配置文件中设定的加密密码 jasypt.encryptor.password
     * @param value    待解密密文
     * @return
     */
    public static String decyptPwd(String password, String value) {
        PooledPBEStringEncryptor encryptOr = new PooledPBEStringEncryptor();
        encryptOr.setConfig(cryptOr(password));
        String result = encryptOr.decrypt(value);
        return result;
    }

    /**
     * @param password salt
     * @return
     */
    public static SimpleStringPBEConfig cryptOr(String password) {
        SimpleStringPBEConfig config = new SimpleStringPBEConfig();
        config.setPassword(password);
        config.setAlgorithm(StandardPBEByteEncryptor.DEFAULT_ALGORITHM);
        config.setKeyObtentionIterations("1000");
        config.setPoolSize("1");
        config.setProviderName(null);
        config.setSaltGeneratorClassName("org.jasypt.salt.RandomSaltGenerator");
        config.setStringOutputType("base64");
        return config;
    }

    public static void main(String[] args) {
        // 加密
        System.out.println(encryptPwd("EbfYkitulv73I2p0mXI50JMXoaxZTKJ7", "root@1234"));
        // 解密
//        mysql@1234
//        System.out.println(decyptPwd("EbfYkitulv73I2p0mXI50JMXoaxZTKJ7", "bgWQ4OfVCUJ1ExsqNhGV+KKBgpx8alv+"));

//        root@1234
//        System.out.println(decyptPwd("EbfYkitulv73I2p0mXI50JMXoaxZTKJ7", "tdHzge8YvviOJaiV/+P6uQ9wgB44D1aH"));
    }

}
```
<a name="Z2xUe"></a>
## 第二种，自定义class
<a name="nPggu"></a>
### 实现`EnvironmentPostProcessor`
从Spring Boot 1.3开始，可以在应用程序上下文刷新之前使用`EnvironmentPostProcessor`来自定义应用程序的`Environment`。`Environment`表示当前应用程序运行的环境，它可以统一访问各种属性源中的属性，如属性文件、JVM系统属性、系统环境变量和Servlet上下文参数。使用`EnvironmentPostProcessor`可以在bean初始化之前对`Environment`进行修改<br />`EnvironmentPostProcessor`可以在创建应用程序上下文之前，添加或者修改环境配置。<br />`EnvironmentPostProcessor`接口实现代表：`ConfigFileApplicationListener`
<a name="u46LG"></a>
### 加密解密工具类
```java
package com.yss.henghe.iris.configuration;

/**
 * @Description 
 * @Date 2021/12/1 16:36
 */

import java.util.Base64;
import javax.crypto.Cipher;
import java.security.KeyFactory;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
public class RSAEncryptDecrypt
{
    // 公钥
    private static String publicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqfkYJn8BHttyGrQYcoXTZK8Za" +
            "MVUkqhEdFIyWZhnUQz6Jr299IkBCrbTnKcQTn3ekyQl5/F+j2p8YuNQDBJiq46T/srI+zh1XIaTIKJoOI8M4ploKOztJ8IP" +
            "L9ucdnv/tEdGDD9kY5JILa5DQnem9HkaS55kGJX6Oet6CRiNekwiG4+K61SjyfqxuLcqm7v2gH2nvTnkci9FLwtErpdF4uT" +
            "Mv6LB46Z9Hpg5iVsHDbwnwqOCfirgwdalJQTy+jcn9UqqNTsneREOxTtt9JQEnsDE/4UIPAYiU6cQDRJ5YrXR56LYC/0g55" +
            "KpMVnMIJgS5Y/YVt66QHtUTCVmmSgxAQIDAQAB";

    // 私钥
    private static String privateKey = "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCp+" +
            "RgmfwEe23IatBhyhdNkrxloxVSSqER0UjJZmGdRDPomvb30iQEKttOcpxBOfd6TJCXn8X6Panxi41AMEmKrjpP+" +
            "ysj7OHVchpMgomg4jwzimWgo7O0nwg8v25x2e/+0R0YMP2RjkkgtrkNCd6b0eRpLnmQYlfo563oJGI16TCIbj4rrVKPJ+" +
            "rG4tyqbu/aAfae9OeRyL0UvC0Sul0Xi5My/osHjpn0emDmJWwcNvCfCo4J+" +
            "KuDB1qUlBPL6Nyf1Sqo1Oyd5EQ7FO230lASewMT/hQg8BiJTpxANEnlitdHnotgL/SDnkqkxWcwgmBLlj" +
            "9hW3rpAe1RMJWaZKDEBAgMBAAECggEBAIHEloaVglJ/sf7nLp8IwxrkgB64QVhytUillKFItOBxau53A" +
            "HaYvr3iVW8NMWrruClYeMQ7YKe34d1RtMRyqPhXs2/cfFMoiJmqeNt6gt1jga/i9V4BfRJUm2mrXiorg" +
            "06s97LUFx3aCdcua1Vsqn+NkeDXvY3zuwXLXPFi2GjcRBnwZL9S2i8mxCSChWy8ePjYW34bJ7viTQ7hf" +
            "Hz5VNA0hqMQrmgzUADq5xelnhQKsEWBaHHhLTKGYF0yNmRJ7zQJkiSncxIrzD5dvRH88Ms2nJ7lJlb1F" +
            "YEpZ8URlK63ofOskvcpvXFH3g7yvi6j9rCRFAL/i2UnP+lFimQ+bs0CgYEA7ZdVrSGMsKRwaX7+lmCTu" +
            "5cR6V74ohZKyDyzp8PcOBxvPmjY5tIULK++12xyOpsMHAwAoKfRedRrvS9vm525g5A+fioALXE74/d2A7" +
            "E/psO6hO89W5AcA91wMTZXLLGZ8JD0LBQp3u4dNGyyZLpDMTgM1WUsted1mOn2msRkK68CgYEAtySKlvaL" +
            "8Rw70LMMYZUUJ6BbUsR85aJHZ3JPsisM01PgEgcJiZHfThVZHqUgEU0noqRDzIVVKk9C1Gqx+t6F3JqU8" +
            "Cewop+nIKboLH5vMr+7OqM5hXLSx9g8Mm779k9P4+u7Kvq4/fY38d7kVayr3TVb5g0jEo1ugsHonwMsKk8Cg" +
            "YBa791/EqRCx+2us0jGTdi9qCjW5d7MSzP8SB+LSs/zOg7qGD9MuYO3Rt0Inx1piQathXqIAzOOKdvC4XEaYtgqnv8" +
            "MUw8WVYzSyFiHOURfk/LEBr25WgMfB5Z1f5MGLEP7a7/JTz5ncUQEWMY+/3vQTt+6narrRNgh2wrkWd7tSQKBgGIs" +
            "YWpZUVz3UI0oXbu1iW9Qg4PTtkv2eKZYXaZZc2+ZJ6UiRpeLLZQS14oY5B7CKDwEKB/rXWLnyCBL7YpYXJOL/cjazd" +
            "HvGUzki9LGF9+xbbEaLEx/58OfA23ZlpFLpLy98cAxVJc2tHigje/rNtnGr7ObWTCpxhKr1YHf1n37AoGBAI64k/lV4" +
            "FJn57cJnIZPp1jjapxSouxO063Z5BTYVsA6/cqXgbNPt1S8TnZRaRB2qtVrN5otRuueO6GIRJf6XTXvwxLV4xQAA/Z5" +
            "g2q2gelK7/ChYRdVn6Fwtnwx4iPBkH6FK+eQ0E8BfcJ5f6XPWp8N7ZpaZrd+dyc3r7uW78gG";

    //加密
    public static byte[] encrypt(byte[] str, String publicKey) throws Exception
    {
        // base64编码的公钥
        byte[] decoded = Base64.getDecoder().decode(publicKey);
        RSAPublicKey pubKey = (RSAPublicKey) KeyFactory.getInstance("RSA")
                .generatePublic(new X509EncodedKeySpec(decoded));
        // RSA加密
        Cipher cipher = Cipher.getInstance("RSA");
        cipher.init(Cipher.ENCRYPT_MODE, pubKey);
        return cipher.doFinal(str);
    }

    //解密
    public static byte[] decrypt(byte[] str) throws Exception
    {
        // base64编码的私钥
        byte[] decoded = Base64.getDecoder().decode(privateKey);
        RSAPrivateKey priKey = (RSAPrivateKey) KeyFactory.getInstance("RSA")
                .generatePrivate(new PKCS8EncodedKeySpec(decoded));
        // RSA解密
        Cipher cipher = Cipher.getInstance("RSA");
        cipher.init(Cipher.DECRYPT_MODE, priKey);
        return cipher.doFinal(str);
    }

    public static void main(String[] args) throws Exception
    {
        byte[] message = "1q2w3eROOT!".getBytes();
        System.out.println("原始字符串为:" + new String(message));
        byte[] messageEn = encrypt(message, publicKey);
        String encryPassword = new String(Base64.getEncoder().encode(messageEn));
        System.out.println("加密后的Base64字符串为:" + encryPassword);

        String str = new String(Base64.getEncoder().encode(messageEn));
        byte[] decode = Base64.getDecoder().decode(str);
        byte[] messageDe = RSAEncryptDecrypt.decrypt(decode);
        System.out.println("还原后的字符串为:" + new String(messageDe));
    }
}
```
<a name="QvRDJ"></a>
### 创建自定义类，实现`EnvironmentPostProcessor`
```java
package com.yss.henghe.iris.configuration;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.env.EnvironmentPostProcessor;
import org.springframework.core.env.*;

import java.util.*;

/**
* @Description
* @Date 2021/12/1 14:05
*/
public class SafetyEncryptProcessor implements EnvironmentPostProcessor{
	
	public static final String SPRING_DATASOURCE_PASSWORD = "spring.datasource.password";
	public static final String PASSWORD_ENABLE_ENCRYPT = "password.enable.encrypt";
	public static final String TRUE = "true";
	
	@Override
	public void postProcessEnvironment(ConfigurableEnvironment environment, SpringApplication application) {
		String enableEncrypt = environment.getProperty(PASSWORD_ENABLE_ENCRYPT);
		if (TRUE.equals(enableEncrypt)) {
			String password = environment.getProperty(SPRING_DATASOURCE_PASSWORD);
			Properties properties = new Properties();
			byte[] decode = Base64.getDecoder().decode(password);
			byte[] messageDe = new byte[0];
			try {
				messageDe = RSAEncryptDecrypt.decrypt(decode);
			} catch (Exception e) {
				
			}
			
			properties.setProperty(SPRING_DATASOURCE_PASSWORD, new String(messageDe));
			PropertiesPropertySource propertiesPropertySource = new PropertiesPropertySource(SPRING_DATASOURCE_PASSWORD,
																							 properties);
			environment.getPropertySources().addFirst(propertiesPropertySource);
		}
		
	}
}
```
<a name="ZDGGy"></a>
### 在META-INF下创建spring.factories，并且引入`CustomEnvironmentPostProcessor` 类
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1655391534959-ecac4016-8f8a-4cc9-b450-66de368d87fc.png#clientId=ue919bf0f-dc14-4&from=paste&id=u4527ec30&originHeight=121&originWidth=332&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u228821b6-19f5-41b9-9a80-cb8bc926079&title=)<br />spring.factories:
```
org.springframework.boot.env.EnvironmentPostProcessor=\
com.yss.henghe.iris.configuration.SafetyEncryptProcessor
```
<a name="QLbVt"></a>
### 将加密后的密码写入到配置文件中
```
spring.datasource.password
```
<a name="PzeZu"></a>
### 公钥私钥生成方法，利用datax工具，可以生成1024,2048等制定要求的公钥秘钥
Maven依赖
```xml
<dependency> 
	<groupId>com.alibaba.datax</groupId> 
	<artifactId>datax-core</artifactId> 
	<version>0.0.1-SNAPSHOT</version> 
</dependency>
```
```java
public static void main(String[] args) throws Exception{
	String[] initKey = SecretUtil.initKey(); 
	System.out.printf("%s: %s\n","publicKey",initKey[0]); 
	System.out.printf("%s: %s\n","privateKey",initKey[1]);
}
```
