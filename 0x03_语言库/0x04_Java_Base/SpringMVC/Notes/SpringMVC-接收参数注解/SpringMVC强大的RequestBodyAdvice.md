JavaSpringMVC
<a name="F0Qnl"></a>
## 1、前言
在实际项目中，有时候需要在请求之前或之后做一些操作，比如：对参数进行解密，对所有的返回值进行加密等。这些与业务无关的操作，没有必要在每个 Controller 方法中都写一遍，这里就可以使用 SpringMVC 中的`@ControllerAdvice` 和 `RequestBodyAdvice`、`ResponseBodyAdvice` 来对请求前后进行处理，本质上就是 aop 的思想。<br />`RequestBodyAdvice`：对`@RquestBody` 进行增强处理，比如所有请求的数据都加密之后放在 body 中，在到达 Controller 的方法之前，需要先进行解密，那么就可以通过 `RequestBodyAdvice` 来进行统一的解密处理，无需在 Controller 方法中去做这些通用的操作。<br />`ResponseBodyAdvice`：通过名称就可以知道，这玩意是对`@ResponseBody` 进行增强处理的，可以对 Controller 中`@ResponseBody` 类型返回值进行增强处理，也就是说可以拦截`@ResponseBody` 类型的返回值，进行再次处理，比如加密、包装等操作。
<a name="VRrIB"></a>
## 2、这个需求如何实现？
比如咱们的项目中对数据的安全性要求比较高，那么可以对所有请求的数据进行加密，后端需要解密之后再进行处理。<br />怎么实现呢？可以在Controller中的每个方法中先进行解密，然后在进行处理，这也太low了吧，需要修改的代码太多了。<br />这个需求可以通过`@ControllerAdvice` 和 `RequestBodyAdvice` 来实现，特别的简单，两三下的功夫就搞定了，下面上代码。
<a name="VrVCU"></a>
## 3、案例
<a name="tf8m2"></a>
### 3.1、自定义一个 `RequestBodyAdvice`
```java
import com.javacode2018.springmvc.chat13.util.EncryptionUtils;
import org.apache.commons.io.IOUtils;
import org.springframework.core.MethodParameter;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpInputMessage;
import org.springframework.http.converter.HttpMessageConverter;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.RequestBodyAdviceAdapter;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Type;

@ControllerAdvice
public class DecryptRequestBodyAdvice extends RequestBodyAdviceAdapter {
    @Override
    public boolean supports(MethodParameter methodParameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) {
        return true;
    }

    @Override
    public HttpInputMessage beforeBodyRead(HttpInputMessage inputMessage, MethodParameter parameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) throws IOException {
        String encoding = "UTF-8";
        //①：获取http请求中原始的body
        String body = IOUtils.toString(inputMessage.getBody(), encoding);
        //②：解密body，EncryptionUtils源码在后面
        String decryptBody = EncryptionUtils.desEncrypt(body);
        //将解密之后的body数据重新封装为HttpInputMessage作为当前方法的返回值
        InputStream inputStream = IOUtils.toInputStream(decryptBody, encoding);
        return new HttpInputMessage() {
            @Override
            public InputStream getBody() throws IOException {
                return inputStream;
            }

            @Override
            public HttpHeaders getHeaders() {
                return inputMessage.getHeaders();
            }
        };
    }

}
```

- 自定义的类需要实现 `RequestBodyAdvice` 接口，这个接口有个默认的实现类 `RequestBodyAdviceAdapter`，相当于一个适配器，方法体都是空的，所以自定义的类可以直接继承这个类，更方便一些
- 这个类上面一定要加上`@ControllerAdvice`注解，有了这个注解，SpringMVC 才能够识别这个类是对 Controller 的增强类
- `supports` 方法：返回一个 boolean 值，若为 true，则表示参数需要这个类处理，否则，跳过这个类的处理
- `beforeBodyRead`：在 body 中的数据读取之前可以做一些处理，在这个方法中来做解密的操作。
<a name="aM83D"></a>
### 3.2、来个 Controller 测试效果
下面这个 Controller 中搞了 2 个方法，稍后传递密文进来，最后这两个方法会将结果返回，返回的结果是经过`DecryptRequestBodyAdvice`类处理之后的明文，稍后验证。
```java
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class UserController {
    @RequestMapping("/user/add")
    public User add(@RequestBody User user) {
        System.out.println("user:" + user);
        return user;
    }

    @RequestMapping("/user/adds")
    public List<User> adds(@RequestBody List<User> userList) {
        System.out.println("userList:" + userList);
        return userList;
    }

    public static class User {
        private String name;
        private Integer age;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public Integer getAge() {
            return age;
        }

        public void setAge(Integer age) {
            this.age = age;
        }

        @Override
        public String toString() {
            return "User{" +
            "name='" + name + '\'' +
            ", age=" + age +
            '}';
        }
    }
}
```
<a name="VDTvr"></a>
### 3.3、加密工具类 `EncryptionUtils`
可以运行 main 方法，得到 2 个测试的密文。
```java
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;

/**
 * 加密工具类
 */
public class EncryptionUtils {

    private static String key = "abcdef0123456789";

    public static void main(String[] args) throws Exception {
        m1();
        m2();
    }

    private static void m1(){
        String body = "{\"name\":\"路人\",\"age\":30}";
        String encryptBody = EncryptionUtils.encrypt(body);
        System.out.println(encryptBody);
        String desEncryptBody = EncryptionUtils.desEncrypt(encryptBody);
        System.out.println(desEncryptBody);
    }
    private static void m2(){
        String body = "[{\"name\":\"路人\",\"age\":30},{\"name\":\"springmvc高手系列\",\"age\":30}]";
        String encryptBody = EncryptionUtils.encrypt(body);
        System.out.println(encryptBody);
        String desEncryptBody = EncryptionUtils.desEncrypt(encryptBody);
        System.out.println(desEncryptBody);
    }

    private static String AESTYPE = "AES/CBC/PKCS5Padding";

    /**
     * 加密明文
     *
     * @param plainText 明文
     * @return
     * @throws Exception
     */
    public static String encrypt(String plainText) {
        try {
            Cipher cipher = Cipher.getInstance(AESTYPE);
            byte[] dataBytes = plainText.getBytes("utf-8");
            byte[] plaintext = new byte[dataBytes.length];
            System.arraycopy(dataBytes, 0, plaintext, 0, dataBytes.length);
            SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");
            IvParameterSpec ivspec = new IvParameterSpec(key.getBytes());
            cipher.init(Cipher.ENCRYPT_MODE, keyspec, ivspec);
            byte[] encrypted = cipher.doFinal(plaintext);
            return new String(Base64.getEncoder().encode(encrypted), "UTF-8");
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * 解密密文
     *
     * @param encryptData 密文
     * @return
     * @throws Exception
     */
    public static String desEncrypt(String encryptData) {
        try {
            Cipher cipher = Cipher.getInstance(AESTYPE);
            SecretKeySpec keyspec = new SecretKeySpec(key.getBytes(), "AES");
            IvParameterSpec ivspec = new IvParameterSpec(key.getBytes());
            cipher.init(Cipher.DECRYPT_MODE, keyspec, ivspec);
            byte[] original = cipher.doFinal(Base64.getDecoder().decode(encryptData.getBytes("UTF-8")));
            return new String(original, "utf-8");
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }

}
```
<a name="OmnLv"></a>
### 3.4、验证效果
验证效果

| 接口 | 明文 | 密文 |
| --- | --- | --- |
| /user/add | {"name":"路人","age":30} | 0A10mig46aZI76jwpgmeeuqDHc7h4Zq/adoY6d5r2mY= |
| /user/adds | [{"name":"路人","age":30},{"name":"springmvc 高手系列","age":30}] | UzWvCsrqt7ljXVI18XBXU3B9S4P2bMB72vH0HNst1GhMt5HTAiodbJwr7r8PuWWs1gM5iAYY4DZWfLgsTbizAEwEtqw8VuCuk2hYBjoCtCc= |

将项目发布到 Tomcat，然后使用 idea 中的 HTTP client 跑下这 2 个测试用例
```http
POST http://localhost:8080/chat13/user/add
Content-Type: application/json

0A10mig46aZI76jwpgmeeuqDHc7h4Zq/adoY6d5r2mY=

###
POST http://localhost:8080/chat13/user/adds
Content-Type: application/json

UzWvCsrqt7ljXVI18XBXU3B9S4P2bMB72vH0HNst1GhMt5HTAiodbJwr7r8PuWWs1gM5iAYY4DZWfLgsTbizAEwEtqw8VuCuk2hYBjoCtCc=
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684056137371-cd11d76b-e194-42da-9bad-9282eb2e6bcf.png#averageHue=%23f9f7f5&clientId=ucd0ec76c-dcfb-4&from=paste&id=uda657f06&originHeight=379&originWidth=997&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u375b64f5-01a6-4262-9d79-34caa7aaf15&title=)<br />输出如下，变成明文了
```
用例1输出
{
  "name": "路人",
  "age": 30
}

用例2输出
[
  {
    "name": "路人",
    "age": 30
  },
  {
    "name": "springmvc高手系列",
    "age": 30
  }
]
```
无需在 Controller 中进行解密，将解密统一放在`RequestBodyAdvice`中做了。
<a name="QeVud"></a>
## 4、多个 `RequestBodyAdvice` 指定顺序
当程序中定义了多个`RequestBodyAdvice`，可以通过下面 2 种方式来指定顺序。<br />方式 1：使用`@org.springframework.core.annotation.Order`注解指定顺序，顺序按照 value 的值从小到大，如：
```java
@Order(2)
@ControllerAdvice
public class RequestBodyAdvice1 extends RequestBodyAdviceAdapter{}

@Order(1)
@ControllerAdvice
public class RequestBodyAdvice2 extends RequestBodyAdviceAdapter{}
```
方式 1：实现`org.springframework.core.Ordered`接口，顺序从小到大，如：
```java
@ControllerAdvice
public class RequestBodyAdvice1 extends RequestBodyAdviceAdapter implements Ordered{
    int getOrder(){
        return 1;
    }
}

@Order(1)
@ControllerAdvice
public class RequestBodyAdvice2 extends RequestBodyAdviceAdapter implements Ordered{
    int getOrder(){
        return 2;
    }
}
```
<a name="RWIGf"></a>
## 5、`@ControllerAdvice` 指定增强的范围
`@ControllerAdvice` 注解相当于对 Controller 的功能进行了增强，目前来看，对所有的 Controller 方法都增强了。<br />那么，能否控制一下增强的范围呢？比如对某些包中的 Controller 进行增强，或者通过其他更细的条件来控制呢？<br />确实可以，可以通过`@ControllerAdvice` 中的属性来指定增强的范围，需要满足这些条件的才会被`@ControllerAdvice` 注解标注的 bean 增强，每个属性都是数组类型的，所有的条件是或者的关系，满足一个即可。
```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Component
public @interface ControllerAdvice {

    /**
     * 用来指定controller所在的包，满足一个就可以
     */
    @AliasFor("basePackages")
    String[] value() default {};

    /**
     * 用来指定controller所在的包，满足一个就可以
     */
    @AliasFor("value")
    String[] basePackages() default {};

    /**
     * controller所在的包必须为basePackageClasses中同等级或者子包中，满足一个就可以
     */
    Class<?>[] basePackageClasses() default {};

    /**
     * 用来指定Controller需要满足的类型，满足assignableTypes中指定的任意一个就可以
     */
    Class<?>[] assignableTypes() default {};

    /**
     * 用来指定Controller上需要有的注解，满足annotations中指定的任意一个就可以
     */
    Class<? extends Annotation>[] annotations() default {};

}
```
扩展知识：这块的判断对应的源码如下，有兴趣的可以看看。
```
org.springframework.web.method.HandlerTypePredicate#test
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684056137341-e3bd3c0c-3184-4c8f-bdf9-39e8443fabc6.png#averageHue=%23fdfcfb&clientId=ucd0ec76c-dcfb-4&from=paste&id=uda13e40b&originHeight=594&originWidth=924&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u965b124e-3179-4e52-841d-cf2072cb3de&title=)
<a name="Tzawg"></a>
## 6、`RequestBodyAdvice` 原理
有些朋友可能对`@ControllerAdvice`和`RequestBodyAdvice`的原理比较感兴趣，想研究一下他们的源码，关键代码在下面这个方法中，比较简单，有兴趣的可以去翻阅一下，这里就不展开说了。
```
org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter#afterPropertiesSet
org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter#initControllerAdviceCache
```
<a name="iu9t2"></a>
## 7、总结

- `@ControllerAdvice` 和 `RequestBodyAdvice` 一起使用可以拦截`@RequestBody` 标注的参数，对参数进行增强处理
- 建议：案例中 `RequestBodyAdvice#supports` 方法咱们直接返回的是 true，会对所有`@RequestBody` 标注的参数进行处理，有些参数可能不需要处理，对于这种情况的，`supports` 方法需要返回 false，这种问题留给大家自己试试了，挺简单的，比如可以自定义一个注解标注在无需处理的参数上，检测到参数上有这个注解的时候（`supprts` 方法中的 `methodParameter` 参数可以获取参数的所有信息），`supports` 返回 `false`。
