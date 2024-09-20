Java SpringBoot
<a name="qOIYS"></a>
## 配置脱敏
实现配置的脱敏使用Java的一个加解密工具`Jasypt`，它提供了单密钥对称加密和非对称加密两种脱敏方式。<br />单密钥对称加密：一个密钥加盐，可以同时用作内容的加密和解密依据；<br />非对称加密：使用公钥和私钥两个密钥，才可以对内容加密和解密；<br />以上两种加密方式使用都非常简单，以SpringBoot集成单密钥对称加密方式做示例。<br />首先引入jasypt-spring-boot-starter jar
```xml
 <!--配置文件加密-->
 <dependency>
     <groupId>com.github.ulisesbocchio</groupId>
     <artifactId>jasypt-spring-boot-starter</artifactId>
     <version>2.1.0</version>
 </dependency>
```
配置文件加入秘钥配置项`jasypt.encryptor.password`，并将需要脱敏的`value`值替换成预先经过加密的内容`ENC(mVTvp4IddqdaYGqPl9lCQbzM3H/b0B6l)`。<br />这个格式是可以随意定义的，比如想要`abc[mVTvp4IddqdaYGqPl9lCQbzM3H/b0B6l]`格式，只要配置前缀和后缀即可。
```yaml
jasypt:
  encryptor:
    property:
      prefix: "abc["
      suffix: "]"
```
`ENC(XXX)`格式主要为了便于识别该值是否需要解密，如不按照该格式配置，在加载配置项的时候jasypt将保持原值，不进行解密。
```yaml
spring:
  datasource:
    url: jdbc:mysql://1.2.3.4:3306/xiaofu?useSSL=false&useUnicode=true&characterEncoding=UTF-8&autoReconnect=true&ze oDateTimeBehavior=convertToNull&serverTimezone=Asia/Shanghai
    username: xiaofu
    password: ENC(mVTvp4IddqdaYGqPl9lCQbzM3H/b0B6l)

# 秘钥
jasypt:
  encryptor:
    password: 123456
```
秘钥是个安全性要求比较高的属性，所以一般不建议直接放在项目内，可以通过启动时`-D`参数注入，或者放在配置中心，避免泄露。
```bash
java -jar -Djasypt.encryptor.password=1123 springboot-jasypt-2.3.3.RELEASE.jar
```
预先生成的加密值，可以通过代码内调用API生成
```java
@Autowired
private StringEncryptor stringEncryptor;

public void encrypt(String content) {
    String encryptStr = stringEncryptor.encrypt(content);
    System.out.println("加密后的内容：" + encryptStr);
}
```
或者通过如下Java命令生成，几个参数D:\maven_lib\org\jasypt\jasypt\1.9.3\jasypt-1.9.3.jar为jasypt核心jar包，`input`待加密文本，`password`秘钥，`algorithm`为使用的加密算法。
```bash
java -cp  D:\maven_lib\org\jasypt\jasypt\1.9.3\jasypt-1.9.3.jar org.jasypt.intf.cli.JasyptPBEStringEncryptionCLI input="root" password=xiaofu algorithm=PBEWithMD5AndDES
```
![2021-09-07-12-10-17-752706.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630987837131-78a0996c-bfbb-4c42-ad1c-e0874cbf2060.png#clientId=uad12c5d4-0423-4&from=ui&id=uf0de2387&originHeight=357&originWidth=960&originalType=binary&ratio=1&size=12090&status=done&style=none&taskId=u9c9d7ea4-0964-47eb-918a-36028f2b274)<br />操作后如果还能正常启动，说明配置文件脱敏就没问题了。
<a name="Mqq6b"></a>
## 敏感字段脱敏
生产环境用户的隐私数据，比如手机号、身份证或者一些账号配置等信息，入库时都要进行不落地脱敏，也就是在进入系统时就要实时的脱敏处理。<br />用户数据进入系统，脱敏处理后持久化到数据库，用户查询数据时还要进行反向解密。这种场景一般需要全局处理，那么用AOP切面来实现在适合不过了。<br />![2021-09-07-12-10-17-900663.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630987837166-8982a7c5-218e-42f9-a5c4-c1b65363c369.png#clientId=uad12c5d4-0423-4&from=ui&id=Nftjd&originHeight=347&originWidth=801&originalType=binary&ratio=1&size=189104&status=done&style=shadow&taskId=ubde1cead-b0e6-4ca1-963f-c28e68d9de9)<br />首先自定义两个注解`@EncryptField`、`@EncryptMethod`分别用在字段属性和方法上，实现思路很简单，只要方法上应用到`@EncryptMethod`注解，则检查入参字段是否标注`@EncryptField`注解，有则将对应字段内容加密。
```java
@Documented
@Target({ElementType.FIELD,ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
public @interface EncryptField {

    String[] value() default "";
}

@Documented
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface EncryptMethod {

    String type() default ENCRYPT;
}
```
切面的实现也比较简单，对入参加密，返回结果解密。
```java
@Slf4j
@Aspect
@Component
public class EncryptHandler {

    @Autowired
    private StringEncryptor stringEncryptor;

    @Pointcut("@annotation(com.xiaofu.annotation.EncryptMethod)")
    public void pointCut() {
    }

    @Around("pointCut()")
    public Object around(ProceedingJoinPoint joinPoint) {
        /**
         * 加密
         */
        encrypt(joinPoint);
        /**
         * 解密
         */
        Object decrypt = decrypt(joinPoint);
        return decrypt;
    }

    public void encrypt(ProceedingJoinPoint joinPoint) {

        try {
            Object[] objects = joinPoint.getArgs();
            if (objects.length != 0) {
                for (Object o : objects) {
                    if (o instanceof String) {
                        encryptValue(o);
                    } else {
                        handler(o, ENCRYPT);
                    }
                    //TODO 其余类型自己看实际情况加
                }
            }
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
    }

    public Object decrypt(ProceedingJoinPoint joinPoint) {
        Object result = null;
        try {
            Object obj = joinPoint.proceed();
            if (obj != null) {
                if (obj instanceof String) {
                    decryptValue(obj);
                } else {
                    result = handler(obj, DECRYPT);
                }
                //TODO 其余类型自己看实际情况加
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return result;
    }
    // ......
}
```
紧接着测试一下切面注解的效果，对字段`mobile`、`address`加上注解`@EncryptField`做脱敏处理。
```java
@EncryptMethod
@PostMapping(value = "test")
@ResponseBody
public Object testEncrypt(@RequestBody UserVo user, @EncryptField String name) {

    return insertUser(user, name);
}

private UserVo insertUser(UserVo user, String name) {
    System.out.println("加密后的数据：user" + JSON.toJSONString(user));
    return user;
}

@Data
public class UserVo implements Serializable {

    private Long userId;

    @EncryptField
    private String mobile;

    @EncryptField
    private String address;

    private String age;
}
```
请求这个接口，看到参数被成功加密，而返回给用户的数据依然是脱敏前的数据，符合预期，那到这简单的脱敏实现就完事了。<br />![2021-09-07-12-10-18-052669.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630987862022-a0ce2626-e223-401f-907a-dd17fe106d92.png#clientId=uad12c5d4-0423-4&from=ui&id=u4918e262&originHeight=80&originWidth=1080&originalType=binary&ratio=1&size=53946&status=done&style=shadow&taskId=u92db1bcc-16b6-4813-b2bc-c4820bd57f3)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1630987495754-31da8129-b75e-4148-ac34-7ba4861e0ad9.webp#clientId=uad12c5d4-0423-4&from=paste&id=u1cbee0db&originHeight=458&originWidth=831&originalType=url&ratio=1&status=done&style=shadow&taskId=u7be24d41-ca17-479f-bebf-a91ae1394ae)
<a name="sdpe4"></a>
## 加密的原理实现
Jasypt工具虽然简单好用，但作为程序员不能仅满足于熟练使用，底层实现原理还是有必要了解下的，这对后续调试bug、二次开发扩展功能很重要。<br />个人认为Jasypt配置文件脱敏的原理很简单，无非就是在具体使用配置信息之前，先拦截获取配置的操作，将对应的加密配置解密后再使用。<br />具体是不是如此简单看下源码的实现，既然是以SpringBoot方式集成，那么就先从jasypt-spring-boot-starter源码开始入手。<br />starter代码很少，主要的工作就是通过SPI机制注册服务和`@Import`注解来注入需前置处理的类`JasyptSpringBootAutoConfiguration`。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636466157075-4f9f4f9a-b13c-4e0a-a47a-80359918ace7.png#clientId=u19fd0bd8-083a-4&from=paste&height=382&id=uc68c8a1a&originHeight=764&originWidth=3019&originalType=binary&ratio=1&size=258283&status=done&style=shadow&taskId=u075ad3d0-14df-46b1-81a1-849840c3858&width=1509.5)<br />在前置加载类`EnableEncryptablePropertiesConfiguration`中注册了一个核心处理类`EnableEncryptablePropertiesBeanFactoryPostProcessor`。<br />![2021-09-07-12-10-18-635664.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630987884746-dc1692c8-d21a-4485-b96c-d5194d708a5a.png#clientId=uad12c5d4-0423-4&from=ui&id=u915e62d7&originHeight=420&originWidth=1080&originalType=binary&ratio=1&size=139848&status=done&style=shadow&taskId=u217726cc-7eb9-407f-bcf1-3e15a9fb204)<br />它的构造器有两个参数，`ConfigurableEnvironment`用来获取所有配属信息，`EncryptablePropertySourceConverter`对配置信息做解析处理。<br />顺藤摸瓜发现具体负责解密的处理类`EncryptablePropertySourceWrapper`，它通过对Spring属性管理类`PropertySource<T>`做拓展，重写了`getProperty(String name)`方法，在获取配置时，凡是指定格式如ENC(x) 包裹的值全部解密处理。<br />![2021-09-07-12-10-18-851468.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630987884784-609dfa8a-3ace-451f-a56d-3f9af41e96b8.png#clientId=uad12c5d4-0423-4&from=ui&id=xZWOL&originHeight=425&originWidth=1080&originalType=binary&ratio=1&size=110766&status=done&style=shadow&taskId=u4d2868cc-4d89-493b-9d8d-37b5ef2c61e)<br />既然知道了原理那么后续二次开发，比如：切换加密算法或者实现自己的脱敏工具就容易的多了。
<a name="Ft0Lv"></a>
### PBE算法
再来聊一下`Jasypt`中用的加密算法，其实它是在JDK的JCE.jar包基础上做了封装，本质上还是用的JDK提供的算法，默认使用的是PBE算法PBEWITHMD5ANDDES，看到这个算法命名很有意思，段个句看看，PBE、WITH、MD5、AND、DES 好像有点故事，继续看。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630988012621-2a215513-1d1a-44aa-88d2-3b20766e315e.png#clientId=uad12c5d4-0423-4&from=paste&height=582&id=u13476fd6&originHeight=1745&originWidth=799&originalType=binary&ratio=1&size=190518&status=done&style=shadow&taskId=u7b77912d-dfd3-4dbc-9840-7f4b50ae281&width=266.3333333333333)<br />PBE算法（Password Based Encryption，基于口令（密码）的加密）是一种基于口令的加密算法，其特点在于口令是由用户自己掌握，在加上随机数多重加密等方法保证数据的安全性。<br />PBE算法本质上并没有真正构建新的加密、解密算法，而是对已知的算法做了包装。比如：常用的消息摘要算法`MD5`和SHA算法，对称加密算法DES、RC2等，而PBE算法就是将这些算法进行合理组合，这也呼应上前边算法的名字。<br />![2021-09-07-12-10-19-214287.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630988026676-9a81b9b1-cea9-479b-9fd3-e22a52768bb2.png#clientId=uad12c5d4-0423-4&from=ui&id=u2f1f36c9&originHeight=341&originWidth=632&originalType=binary&ratio=1&size=144395&status=done&style=shadow&taskId=u69be693e-0f6f-4b01-a341-eb3a9d53593)<br />既然PBE算法使用较为常用的对称加密算法，那就会涉及密钥的问题。但它本身又没有钥的概念，只有口令密码，密钥则是口令经过加密算法计算得来的。<br />口令本身并不会很长，所以不能用来替代密钥，只用口令很容易通过穷举攻击方式破译，这时候就得加点盐了。<br />盐通常会是一些随机信息，比如随机数、时间戳，将盐附加在口令上，通过算法计算加大破译的难度。
<a name="g4mf9"></a>
### 源码里的猫腻
简单了解PBE算法，回过头看看`Jasypt`源码是如何实现加解密的。<br />在加密的时候首先实例化秘钥工厂`SecretKeyFactory`，生成八位盐值，默认使用的`jasypt.encryptor.RandomSaltGenerator`生成器。
```java
public byte[] encrypt(byte[] message) {
    // 根据指定算法，初始化秘钥工厂
    final SecretKeyFactory factory = SecretKeyFactory.getInstance(algorithm1);
    // 盐值生成器，只选八位
    byte[] salt = saltGenerator.generateSalt(8);
    // 
    final PBEKeySpec keySpec = new PBEKeySpec(password.toCharArray(), salt, iterations);
    // 盐值、口令生成秘钥
    SecretKey key = factory.generateSecret(keySpec);

    // 构建加密器
    final Cipher cipherEncrypt = Cipher.getInstance(algorithm1);
    cipherEncrypt.init(Cipher.ENCRYPT_MODE, key);
    // 密文头部（盐值）
    byte[] params = cipherEncrypt.getParameters().getEncoded();

    // 调用底层实现加密
    byte[] encryptedMessage = cipherEncrypt.doFinal(message);

    // 组装最终密文内容并分配内存（盐值+密文）
    return ByteBuffer
            .allocate(1 + params.length + encryptedMessage.length)
            .put((byte) params.length)
            .put(params)
            .put(encryptedMessage)
            .array();
}
```
由于默认使用的是随机盐值生成器，导致相同内容每次加密后的内容都是不同的。<br />那么解密时该怎么对应上呢？<br />看上边的源码发现，最终的加密文本是由两部分组成的，`params`消息头里边包含口令和随机生成的盐值，`encryptedMessage`密文。<br />![2021-09-07-12-10-19-438298.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630988026666-3cc6c93a-ed5a-4ef3-b0db-ecfd1d258ec3.png#clientId=uad12c5d4-0423-4&from=ui&id=nyZet&originHeight=331&originWidth=645&originalType=binary&ratio=1&size=138993&status=done&style=shadow&taskId=u56fc2efc-813e-4ffd-8f0e-9f383ae5bcd)<br />加密<br />而在解密时会根据密文`encryptedMessage`的内容拆解出`params`内容解析出盐值和口令，在调用JDK底层算法解密出实际内容。
```java
@Override
@SneakyThrows
public byte[] decrypt(byte[] encryptedMessage) {
    // 获取密文头部内容
    int paramsLength = Byte.toUnsignedInt(encryptedMessage[0]);
    // 获取密文内容
    int messageLength = encryptedMessage.length - paramsLength - 1;
    byte[] params = new byte[paramsLength];
    byte[] message = new byte[messageLength];
    System.arraycopy(encryptedMessage, 1, params, 0, paramsLength);
    System.arraycopy(encryptedMessage, paramsLength + 1, message, 0, messageLength);

    // 初始化秘钥工厂
    final SecretKeyFactory factory = SecretKeyFactory.getInstance(algorithm1);
    final PBEKeySpec keySpec = new PBEKeySpec(password.toCharArray());
    SecretKey key = factory.generateSecret(keySpec);

    // 构建头部盐值口令参数
    AlgorithmParameters algorithmParameters = AlgorithmParameters.getInstance(algorithm1);
    algorithmParameters.init(params);

    // 构建加密器，调用底层算法
    final Cipher cipherDecrypt = Cipher.getInstance(algorithm1);
    cipherDecrypt.init(
            Cipher.DECRYPT_MODE,
            key,
            algorithmParameters
    );
    return cipherDecrypt.doFinal(message);
}
```
![2021-09-07-12-10-19-701344.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630988026685-18cc0dcf-bcfd-4476-b062-9ddace7a117e.png#clientId=uad12c5d4-0423-4&from=ui&id=UU1Wy&originHeight=356&originWidth=649&originalType=binary&ratio=1&size=149118&status=done&style=shadow&taskId=u4851d6ae-f3e2-4be5-bde6-39ea4395874)
