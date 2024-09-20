JavaSpringBoot
<a name="xU0hw"></a>
## 配置脱敏
实现配置的脱敏使用了Java的一个加解密工具Jasypt，它提供了单密钥对称加密和非对称加密两种脱敏方式。<br />单密钥对称加密：一个密钥加盐，可以同时用作内容的加密和解密依据；<br />非对称加密：使用公钥和私钥两个密钥，才可以对内容加密和解密；<br />以上两种加密方式使用都非常简单，咱们以springboot集成单密钥对称加密方式做示例。<br />首先引入jasypt-spring-boot-starter jar
```xml
<!--配置文件加密-->
<dependency>
	<groupId>com.github.ulisesbocchio</groupId>
	<artifactId>jasypt-spring-boot-starter</artifactId>
	<version>2.1.0</version>
</dependency>
```
配置文件加入秘钥配置项`jasypt.encryptor.password`，并将需要脱敏的value值替换成预先经过加密的内容`ENC(mVTvp4IddqdaYGqPl9lCQbzM3H/b0B6l)`。<br />这个格式是可以随意定义的，比如想要`abc[mVTvp4IddqdaYGqPl9lCQbzM3H/b0B6l]`格式，只要配置前缀和后缀即可。
```yaml
jasypt:
  encryptor:
    property:
      prefix: "abc["
      suffix: "]"
```
ENC(XXX）格式主要为了便于识别该值是否需要解密，如不按照该格式配置，在加载配置项的时候jasypt将保持原值，不进行解密。
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
java -jar -Djasypt.encryptor.password=1123  springboot-jasypt-2.3.3.RELEASE.jar
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
或者通过如下Java命令生成，几个参数D:\maven_lib\org\jasypt\jasypt\1.9.3\jasypt-1.9.3.jar为jasypt核心jar包，input待加密文本，password秘钥，algorithm为使用的加密算法。
```bash
java -cp  D:\maven_lib\org\jasypt\jasypt\1.9.3\jasypt-1.9.3.jar org.jasypt.intf.cli.JasyptPBEStringEncryptionCLI input="root" password=xiaofu  algorithm=PBEWithMD5AndDES
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775393-68fd561f-8c86-42c4-bbed-81f8fed635df.png#clientId=u9f1648a6-63ca-4&from=paste&id=u22d894c2&originHeight=357&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud201a23c-4bee-4e9a-babb-e4e553b26f3&title=)<br />一顿操作后如果还能正常启动，说明配置文件脱敏就没问题了。
<a name="S2qAw"></a>
## 敏感字段脱敏
生产环境用户的隐私数据，比如手机号、身份证或者一些账号配置等信息，入库时都要进行不落地脱敏，也就是在进入系统时就要实时的脱敏处理。<br />用户数据进入系统，脱敏处理后持久化到数据库，用户查询数据时还要进行反向解密。这种场景一般需要全局处理，那么用AOP切面来实现在适合不过了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775599-3df6e3a4-9276-41b8-9fda-94798e885f5f.png#clientId=u9f1648a6-63ca-4&from=paste&id=u5b572558&originHeight=347&originWidth=801&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5da8ebc6-b274-45e7-858c-fabdce48987&title=)<br />首先自定义两个注解`@EncryptField`、`@EncryptMethod`分别用在字段属性和方法上，实现思路很简单，只要方法上应用到`@EncryptMethod`注解，则检查入参字段是否标注`@EncryptField`注解，有则将对应字段内容加密。
```java
@Documented
@Target({ElementType.FIELD,ElementType.PARAMETER})
@Retention(RetentionPolicy.RUNTIME)
public @interface EncryptField {

    String[] value() default "";
}
```
```java
@Documented
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface EncryptMethod {

    String type() default ENCRYPT;
}
```
切面的实现也比较简单，对入参加密，返回结果解密。为了方便阅读这里就只贴出部分代码，完整案例Github地址：[https://github.com/chengxy-nds/Springboot-Notebook/tree/master/springboot-jasypt](https://github.com/chengxy-nds/Springboot-Notebook/tree/master/springboot-jasypt)
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
    //......
}
```
紧接着测试一下切面注解的效果，对字段mobile、address加上注解`@EncryptField`做脱敏处理。
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
```
```java
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
请求这个接口，看到参数被成功加密，而返回给用户的数据依然是脱敏前的数据，符合预期，那到这简单的脱敏实现就完事了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775574-741baabd-9341-4597-aef8-82f5b078bd1e.png#clientId=u9f1648a6-63ca-4&from=paste&id=uf34c3c09&originHeight=80&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u259093a9-8f99-45be-b403-6c9db92ec21&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775590-ce05e2bd-8012-41e0-b0ba-b9481e51709d.png#clientId=u9f1648a6-63ca-4&from=paste&id=u2bb4511b&originHeight=458&originWidth=831&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u122559b0-c8cb-441a-8230-882421347fb&title=)
<a name="Va3UN"></a>
## 知其然知其所以然
Jasypt工具虽然简单好用，但作为程序员不能仅满足于熟练使用，底层实现原理还是有必要了解下的，这对后续调试bug、二次开发扩展功能很重要。<br />个人认为Jasypt配置文件脱敏的原理很简单，无非就是在具体使用配置信息之前，先拦截获取配置的操作，将对应的加密配置解密后再使用。<br />具体是不是如此简单看下源码的实现，既然是以SpringBoot方式集成，那么就先从jasypt-spring-boot-starter源码开始入手。<br />starter代码很少，主要的工作就是通过SPI机制注册服务和`@Import`注解来注入需前置处理的类`JasyptSpringBootAutoConfiguration`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775609-b8067b02-93cb-4146-a001-ed1350fd3116.png#clientId=u9f1648a6-63ca-4&from=paste&id=ucdbd3ec7&originHeight=365&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5438cfb8-5123-48e2-80bf-2510f3823d0&title=)<br />在前置加载类`EnableEncryptablePropertiesConfiguration`中注册了一个核心处理类`EnableEncryptablePropertiesBeanFactoryPostProcessor`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368775683-b7733b90-aab3-42be-b425-1fcde918a238.png#clientId=u9f1648a6-63ca-4&from=paste&id=u7645cfde&originHeight=420&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u76e0ea39-cdf0-4520-9b4e-f96a52e8a90&title=)<br />它的构造器有两个参数，`ConfigurableEnvironment`用来获取所有配属信息，`EncryptablePropertySourceConverter`对配置信息做解析处理。<br />顺藤摸瓜发现具体负责解密的处理类`EncryptablePropertySourceWrapper`，它通过对Spring属性管理类`PropertySource<T>`做拓展，重写了`getProperty(String name)`方法，在获取配置时，凡是指定格式如`**ENC(x)**` 包裹的值全部解密处理。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368776028-8bb55ed2-0b7c-4039-8f5d-cc6cefbb6d1f.png#clientId=u9f1648a6-63ca-4&from=paste&id=u84b4d45d&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u710b30af-cd67-4ad1-b906-3d928be23f1&title=)<br />既然知道了原理那么后续二次开发，比如：切换加密算法或者实现自己的脱敏工具就容易的多了。<br />案例Github地址：[https://github.com/chengxy-nds/Springboot-Notebook/tree/master/springboot-jasypt](https://github.com/chengxy-nds/Springboot-Notebook/tree/master/springboot-jasypt)
<a name="Rkhlm"></a>
### PBE算法
再来聊一下Jasypt中用的加密算法，其实它是在JDK的JCE.jar包基础上做了封装，本质上还是用的JDK提供的算法，默认使用的是PBE算法PBEWITHMD5ANDDES，看到这个算法命名很有意思，段个句看看，PBE、WITH、MD5、AND、DES 好像有点故事，继续看。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368776056-2d7155b5-7775-4704-8b16-113b78f8a1cb.png#clientId=u9f1648a6-63ca-4&from=paste&id=ucfa08c6e&originHeight=279&originWidth=526&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub4c6ac09-8589-4710-bafe-97d3e616bac&title=)<br />PBE算法（Password Based Encryption，基于口令（密码）的加密）是一种基于口令的加密算法，其特点在于口令是由用户自己掌握，在加上随机数多重加密等方法保证数据的安全性。<br />PBE算法本质上并没有真正构建新的加密、解密算法，而是对已知的算法做了包装。比如：常用的消息摘要算法MD5和SHA算法，对称加密算法DES、RC2等，而PBE算法就是将这些算法进行合理组合，这也呼应上前边算法的名字。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368776049-d5f65c24-b0fd-463b-afef-84ccd9205aa6.png#clientId=u9f1648a6-63ca-4&from=paste&id=u996dfeaf&originHeight=341&originWidth=632&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u271dc519-c6a1-4609-8671-d7374f2d409&title=)<br />既然PBE算法使用较为常用的对称加密算法，那就会涉及密钥的问题。但它本身又没有钥的概念，只有口令密码，密钥则是口令经过加密算法计算得来的。<br />口令本身并不会很长，所以不能用来替代密钥，只用口令很容易通过穷举攻击方式破译，这时候就得加点**盐**了。<br />盐通常会是一些随机信息，比如随机数、时间戳，将盐附加在口令上，通过算法计算加大破译的难度。
<a name="vNbFA"></a>
### 源码里的猫腻
简单了解PBE算法，回过头看看Jasypt源码是如何实现加解密的。<br />在加密的时候首先实例化秘钥工厂`SecretKeyFactory`，生成八位盐值，默认使用的`jasypt.encryptor.RandomSaltGenerator`生成器。
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
由于默认使用的是随机盐值生成器，导致相同**内容每次加密后的内容都是不同的**。<br />那么解密时该怎么对应上呢？<br />看上边的源码发现，最终的加密文本是由两部分组成的，`params`消息头里边包含口令和随机生成的盐值，`encryptedMessage`密文。<br />![加密](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368776061-fec28703-c87d-4498-8d9c-1154ea82d707.png#clientId=u9f1648a6-63ca-4&from=paste&id=u1fe782c7&originHeight=331&originWidth=645&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u46548fc2-1ce8-4fa6-92e7-7217af44189&title=%E5%8A%A0%E5%AF%86 "加密")<br />而在解密时会根据密文encryptedMessage的内容拆解出params内容解析出盐值和口令，在调用JDK底层算法解密出实际内容。
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
![解密](https://cdn.nlark.com/yuque/0/2022/png/396745/1660368776103-20c81c0e-e36a-4c27-ab93-cce349c68188.png#clientId=u9f1648a6-63ca-4&from=paste&id=uaa1bd66b&originHeight=356&originWidth=649&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u6c3a3859-7afb-4a3b-8bfa-d41419b52a9&title=%E8%A7%A3%E5%AF%86 "解密")
