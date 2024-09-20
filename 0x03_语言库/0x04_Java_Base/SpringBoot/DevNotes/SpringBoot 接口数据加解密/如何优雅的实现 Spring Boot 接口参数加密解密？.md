JavaSpringBoot<br />加密解密本身并不是难事，问题是在何时去处理？定义一个过滤器，将请求和响应分别拦截下来进行处理也是一个办法，这种方式虽然粗暴，但是灵活，因为可以拿到一手的请求参数和响应数据。不过 SpringMVC 中提供了 `ResponseBodyAdvice` 和 `RequestBodyAdvice`，利用这两个工具可以对请求和响应进行预处理，非常方便。<br />本文的两个目的：

- 分享参数/响应加解密的思路。
- 分享 `ResponseBodyAdvice` 和 `RequestBodyAdvice` 的用法。

好了，那么接下来就不废话了，一起来看下。
<a name="E3aTA"></a>
## 1、开发加解密 starter
为了让开发的这个工具更加通用，也为了复习一下自定义 Spring Boot Starter，这里就将这个工具做成一个 stater，以后在 Spring Boot 项目中直接引用就可以。<br />首先创建一个 Spring Boot 项目，引入 spring-boot-starter-web 依赖：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <scope>provided</scope>
    <version>2.4.3</version>
</dependency>
```
因为这个工具是为 Web 项目开发的，以后必然使用在 Web 环境中，所以这里添加依赖时 scope 设置为 provided。<br />依赖添加完成后，先来定义一个加密工具类备用，加密这块有多种方案可以选择，对称加密、非对称加密，其中对称加密又可以使用 AES、DES、3DES 等不同算法，这里使用 Java 自带的 Cipher 来实现对称加密，使用 AES 算法：
```java
public class AESUtils {

    private static final String AES_ALGORITHM = "AES/ECB/PKCS5Padding";

    // 获取 cipher
    private static Cipher getCipher(byte[] key, int model) throws Exception {
        SecretKeySpec secretKeySpec = new SecretKeySpec(key, "AES");
        Cipher cipher = Cipher.getInstance(AES_ALGORITHM);
        cipher.init(model, secretKeySpec);
        return cipher;
    }

    // AES加密
    public static String encrypt(byte[] data, byte[] key) throws Exception {
        Cipher cipher = getCipher(key, Cipher.ENCRYPT_MODE);
        return Base64.getEncoder().encodeToString(cipher.doFinal(data));
    }

    // AES解密
    public static byte[] decrypt(byte[] data, byte[] key) throws Exception {
        Cipher cipher = getCipher(key, Cipher.DECRYPT_MODE);
        return cipher.doFinal(Base64.getDecoder().decode(data));
    }
}
```
这个工具类比较简单，不需要多解释。需要说明的是，加密后的数据可能不具备可读性，因此一般需要对加密后的数据再使用 Base64 算法进行编码，获取可读字符串。换言之，上面的 AES 加密方法的返回值是一个 Base64 编码之后的字符串，AES 解密方法的参数也是一个 Base64 编码之后的字符串，先对该字符串进行解码，然后再解密。<br />接下来封装一个响应工具类备用：
```java
public class RespBean {
    private Integer status;
    private String msg;
    private Object obj;

    public static RespBean build() {
        return new RespBean();
    }

    public static RespBean ok(String msg) {
        return new RespBean(200, msg, null);
    }

    public static RespBean ok(String msg, Object obj) {
        return new RespBean(200, msg, obj);
    }

    public static RespBean error(String msg) {
        return new RespBean(500, msg, null);
    }

    public static RespBean error(String msg, Object obj) {
        return new RespBean(500, msg, obj);
    }

    private RespBean() {
    }

    private RespBean(Integer status, String msg, Object obj) {
        this.status = status;
        this.msg = msg;
        this.obj = obj;
    }

    public Integer getStatus() {
        return status;
    }

    public RespBean setStatus(Integer status) {
        this.status = status;
        return this;
    }

    public String getMsg() {
        return msg;
    }

    public RespBean setMsg(String msg) {
        this.msg = msg;
        return this;
    }

    public Object getObj() {
        return obj;
    }

    public RespBean setObj(Object obj) {
        this.obj = obj;
        return this;
    }
}
```
接下来定义两个注解 `@Decrypt` 和 `@Encrypt`：
```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD,ElementType.PARAMETER})
public @interface Decrypt {
}
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Encrypt {
}
```
这两个注解就是两个标记，在以后使用的过程中，哪个接口方法添加了 `@Encrypt` 注解就对哪个接口的数据加密返回，哪个接口/参数添加了 `@Decrypt` 注解就对哪个接口/参数进行解密。这个定义也比较简单，没啥好说的，需要注意的是 `@Decrypt` 比 `@Encrypt` 多了一个使用场景就是 `@Decrypt` 可以用在参数上。<br />考虑到用户可能会自己配置加密的 key，因此再来定义一个 `EncryptProperties` 类来读取用户配置的 key：
```java
@ConfigurationProperties(prefix = "spring.encrypt")
public class EncryptProperties {
    private final static String DEFAULT_KEY = "www.itboyhub.com";
    private String key = DEFAULT_KEY;

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }
}
```
这里设置了默认的 key 是 www.itboyhub.com，key 是 16 位字符串。以后如果用户想自己配置 key，只需要在 application.properties 中配置 `spring.encrypt.key=xxx` 即可。<br />所有准备工作做完了，接下来就该正式加解密了。<br />这里主要分享 `ResponseBodyAdvice` 和 `RequestBodyAdvice` 的用法，`RequestBodyAdvice` 在做解密的时候倒是没啥问题，而 `ResponseBodyAdvice` 在做加密的时候则会有一些局限，不过影响不大，还是前面说的，如果想非常灵活的掌控一切，那还是自定义过滤器吧。这里就先用这两个工具来实现了。<br />另外还有一点需要注意，`ResponseBodyAdvice` 在使用了 `@ResponseBody` 注解的时候才会生效，`RequestBodyAdvice` 在使用了 `@RequestBody` 注解的时候才会生效，换言之，前后端都是 JSON 交互的时候，这两个才有用。不过一般来说接口加解密的场景也都是前后端分离的时候才可能有的事。<br />先来看接口加密：
```java
@EnableConfigurationProperties(EncryptProperties.class)
@ControllerAdvice
public class EncryptResponse implements ResponseBodyAdvice<RespBean> {
    private ObjectMapper om = new ObjectMapper();
    @Autowired
    EncryptProperties encryptProperties;
    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        return returnType.hasMethodAnnotation(Encrypt.class);
    }

    @Override
    public RespBean beforeBodyWrite(RespBean body, MethodParameter returnType, MediaType selectedContentType, Class<? extends HttpMessageConverter<?>> selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {
        byte[] keyBytes = encryptProperties.getKey().getBytes();
        try {
            if (body.getMsg()!=null) {
                body.setMsg(AESUtils.encrypt(body.getMsg().getBytes(),keyBytes));
            }
            if (body.getObj() != null) {
                body.setObj(AESUtils.encrypt(om.writeValueAsBytes(body.getObj()), keyBytes));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return body;
    }
}
```
自定义 `EncryptResponse` 类实现 `ResponseBodyAdvice`接口，泛型表示接口的返回类型，这里一共要实现两个方法：

1. `supports`：这个方法用来判断什么样的接口需要加密，参数 `returnType` 表示返回类型，这里的判断逻辑就是方法是否含有 `@Encrypt` 注解，如果有，表示该接口需要加密处理，如果没有，表示该接口不需要加密处理。
2. `beforeBodyWrite`：这个方法会在数据响应之前执行，也就是先对响应数据进行二次处理，处理完成后，才会转成 json 返回。这里的处理方式很简单，RespBean 中的 status 是状态码就不用加密了，另外两个字段重新加密后重新设置值即可。
3. 另外需要注意，自定义的 `ResponseBodyAdvice` 需要用 `@ControllerAdvice` 注解来标记。

再来看接口解密：
```java
@EnableConfigurationProperties(EncryptProperties.class)
@ControllerAdvice
public class DecryptRequest extends RequestBodyAdviceAdapter {
    @Autowired
    EncryptProperties encryptProperties;
    @Override
    public boolean supports(MethodParameter methodParameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) {
        return methodParameter.hasMethodAnnotation(Decrypt.class) || methodParameter.hasParameterAnnotation(Decrypt.class);
    }

    @Override
    public HttpInputMessage beforeBodyRead(final HttpInputMessage inputMessage, MethodParameter parameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) throws IOException {
        byte[] body = new byte[inputMessage.getBody().available()];
        inputMessage.getBody().read(body);
        try {
            byte[] decrypt = AESUtils.decrypt(body, encryptProperties.getKey().getBytes());
            final ByteArrayInputStream bais = new ByteArrayInputStream(decrypt);
            return new HttpInputMessage() {
                @Override
                public InputStream getBody() throws IOException {
                    return bais;
                }

                @Override
                public HttpHeaders getHeaders() {
                    return inputMessage.getHeaders();
                }
            };
        } catch (Exception e) {
            e.printStackTrace();
        }
        return super.beforeBodyRead(inputMessage, parameter, targetType, converterType);
    }
}
```

1. 首先大家注意，`DecryptRequest` 类没有直接实现 `RequestBodyAdvice` 接口，而是继承自 `RequestBodyAdviceAdapter` 类，该类是 `RequestBodyAdvice` 接口的子类，并且实现了接口中的一些方法，这样当继承自 `RequestBodyAdviceAdapter` 时，就只需要根据自己实际需求实现某几个方法即可。
2. `supports`：该方法用来判断哪些接口需要处理接口解密，这里的判断逻辑是方法上或者参数上含有 `@Decrypt` 注解的接口，处理解密问题。
3. `beforeBodyRead`：这个方法会在参数转换成具体的对象之前执行，先从流中加载到数据，然后对数据进行解密，解密完成后再重新构造 `HttpInputMessage` 对象返回。

接下来，再来定义一个自动化配置类，如下：
```java
@Configuration
@ComponentScan("org.javaboy.encrypt.starter")
public class EncryptAutoConfiguration {

}
```
这个也没啥好说的，比较简单。<br />最后，resources 目录下定义 META-INF，然后再定义 spring.factories 文件，内容如下：
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=org.javaboy.encrypt.starter.autoconfig.EncryptAutoConfiguration
```
这样当项目启动时，就会自动加载该配置类。<br />至此， starter 就开发完成了。
<a name="K61vf"></a>
## 2、打包发布
可以将项目安装到本地仓库，也可以发布到线上供他人使用。
<a name="s9cI9"></a>
### 2.1 安装到本地仓库
安装到本地仓库比较简单，直接 `mvn install`，或者在 IDEA 中，点击右边的 Maven，然后双击 install，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852609015-94cb4501-b5c5-4beb-9bcd-4bc63d31fe23.png#averageHue=%23fbf9f9&clientId=u5451ae6f-2b85-4&from=paste&id=uf413099c&originHeight=250&originWidth=270&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u771208cc-0328-4b69-9bed-d65fa986b20&title=)
<a name="VGPQA"></a>
### 2.2 发布到线上
发布到线上可以使用 JitPack 来做。<br />首先在 GitHub 上创建一个仓库，将代码上传上去。<br />上传成功后，点击右边的 Create a new release 按钮，发布一个正式版，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852609046-117b6ce6-3db5-47a7-8102-fff23c05872e.png#averageHue=%2390b589&clientId=u5451ae6f-2b85-4&from=paste&id=ub2b3c425&originHeight=356&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u09914480-03e9-4604-972a-8dba86a8d62&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852608927-0c9ca851-8d82-4870-a192-28d25554db15.png#averageHue=%23fefefe&clientId=u5451ae6f-2b85-4&from=paste&id=uc3d7c789&originHeight=812&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc277cad0-6bed-4b31-a23e-3d6fbbd793f&title=)<br />发布成功后，打开 jitpack，输入仓库的完整路径，点击 lookup 按钮，查找到之后，再点击 Get it 按钮完成构建，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852608897-78a982e4-32ed-4be8-a8f0-54d581784f79.png#averageHue=%23a6ca98&clientId=u5451ae6f-2b85-4&from=paste&id=u51eea6bc&originHeight=382&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7092ec02-9931-4c44-a1e7-641ab0454b8&title=)<br />构建成功后，JitPack 上会给出项目引用方式：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852609060-e2a444a1-610c-41b7-a664-2fb49bca622f.png#averageHue=%237e7e7d&clientId=u5451ae6f-2b85-4&from=paste&id=uc535c302&originHeight=893&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u683237e7-1eb5-4295-a3e4-5714d457b7f&title=)<br />注意引用时将 tag 改成具体的版本号。<br />至此，工具就已经成功发布了！小伙伴们可以通过如下方式引用这个 starter：
```xml
<dependencies>
    <dependency>
        <groupId>com.github.lenve</groupId>
        <artifactId>encrypt-spring-boot-starter</artifactId>
        <version>0.0.3</version>
    </dependency>
</dependencies>
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>
```
<a name="inyq4"></a>
## 3、应用
创建一个普通的 Spring Boot 项目，引入 web 依赖，再引入刚刚的 starter 依赖，如下：
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <dependency>
        <groupId>com.github.lenve</groupId>
        <artifactId>encrypt-spring-boot-starter</artifactId>
        <version>0.0.3</version>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
</dependencies>
<repositories>
    <repository>
        <id>jitpack.io</id>
        <url>https://jitpack.io</url>
    </repository>
</repositories>
```
然后再创建一个实体类备用：
```java
public class User {
    private Long id;
    private String username;
    //省略 getter/setter
}
```
创建两个测试接口：
```java
@RestController
public class HelloController {
    @GetMapping("/user")
    @Encrypt
    public RespBean getUser() {
        User user = new User();
        user.setId((long) 99);
        user.setUsername("javaboy");
        return RespBean.ok("ok", user);
    }

    @PostMapping("/user")
    public RespBean addUser(@RequestBody @Decrypt User user) {
        System.out.println("user = " + user);
        return RespBean.ok("ok", user);
    }
}
```
第一个接口使用了 `@Encrypt` 注解，所以会对该接口的数据进行加密（如果不使用该注解就不加密），第二个接口使用了 `@Decrypt` 所以会对上传的参数进行解密，注意 `@Decrypt` 注解既可以放在方法上也可以放在参数上。<br />接下来启动项目进行测试。<br />首先测试 get 请求接口：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852609353-8eef4733-6ef9-44c2-a7b5-7d4656e9d737.png#averageHue=%23fbfbfb&clientId=u5451ae6f-2b85-4&from=paste&id=u64670d97&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub15d343f-d6fe-40c4-8fd4-10d81df1318&title=)<br />可以看到，返回的数据已经加密。<br />再来测试 post 请求：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670852609210-5b0f7221-e29c-4c4f-9ac2-51a6930947b8.png#averageHue=%23fafafa&clientId=u5451ae6f-2b85-4&from=paste&id=uf1d6e28c&originHeight=830&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8c29a83a-ae71-4edc-9906-ac6a14ef1e3&title=)<br />可以看到，参数中的加密数据已经被还原了。<br />如果用户想要修改加密密钥，可以在 application.properties 中添加如下配置：
```
spring.encrypt.key=1234567890123456
```
加密数据到了前端，前端也有一些 js 工具来处理加密数据。
<a name="iggSD"></a>
## 4、小结
主要聊聊 `ResponseBodyAdvice` 和 `RequestBodyAdvice` 的用法，一些加密思路，当然 `ResponseBodyAdvice` 和 `RequestBodyAdvice` 还有很多其他的使用场景可以自行探索～本文使用了对称加密中的 AES 算法，大家也可以尝试改成非对称加密。
