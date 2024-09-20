Java Jackson
<a name="OIKoB"></a>
### 一、背景
实际的业务开发过程中，经常需要**对用户的隐私数据进行脱敏处理，所谓脱敏处理其实就是将数据进行混淆隐藏**，例如下图，将用户的手机号、地址等数据信息，采用*进行隐藏，以免泄露个人隐私信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634859983042-746ec68e-1558-4d47-87c6-3e7604534a44.webp#clientId=u76735bd5-65ec-4&from=paste&id=u83f5a1d9&originHeight=314&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u5cbd798b-0f37-464f-92ca-cb3864bb9e2)<br />如果需要脱敏的数据范围很小很小，甚至就是指定的字段，一般的处理方式也很简单，就是写一个隐藏方法即可实现数据脱敏。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634859983068-ecc967ee-8bdd-48bb-a619-29f439aeaa1d.webp#clientId=u76735bd5-65ec-4&from=paste&id=uc5e196e6&originHeight=184&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u4670b5dd-1640-4ca2-a969-dcb46c074bc)<br />如果是需求很少的情况下，采用这种方式实现没太大问题，好维护！<br />但如果是类似上面那种很多位置的数据，需要分门别类的进行脱敏处理，通过这种简单粗暴的处理，代码似乎就显得不太优雅了。<br />思考一下，可不可以在数据输出的阶段，进行统一数据脱敏处理，这样就可以省下不少体力活。<br />说到数据输出，很多同学可能会想到 JSON 序列化。熟悉的 web 系统，就是将数据通过 json 序列化之后展示给前端。<br />那么问题来了，如何在序列化的时候，进行数据脱敏处理呢？
<a name="VRDPC"></a>
### 二、程序实践
<a name="KEn7o"></a>
#### 2.1、首先添加依赖包
默认的情况下，如果当前项目已经添加了spring-web包或者spring-boot-starter-web包，因为这些jar包已经集成了jackson相关包，因此无需重复依赖。<br />如果当前项目没有jackson包，可以通过如下方式进行添加相关依赖包。
```xml
<!--jackson依赖-->
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-core</artifactId>
    <version>2.9.8</version>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-annotations</artifactId>
    <version>2.9.8</version>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-databind</artifactId>
    <version>2.9.8</version>
</dependency>
```
<a name="CiDAF"></a>
#### 2.2、编写脱敏类型枚举类，满足不同场景的处理
```java
public enum SensitiveEnum {

    /**
     * 中文名
     */
    CHINESE_NAME,

    /**
     * 身份证号
     */
    ID_CARD,

    /**
     * 座机号
     */
    FIXED_PHONE,

    /**
     * 手机号
     */
    MOBILE_PHONE,

    /**
     * 地址
     */
    ADDRESS,

    /**
     * 电子邮件
     */
    EMAIL,

    /**
     * 银行卡
     */
    BANK_CARD,

    /**
     * 公司开户银行联号
     */
    CNAPS_CODE
}
```
<a name="YOYiP"></a>
#### 2.3、编写脱敏注解类
```java
import com.fasterxml.jackson.annotation.JacksonAnnotationsInside;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@Retention(RetentionPolicy.RUNTIME)
@JacksonAnnotationsInside
@JsonSerialize(using = SensitiveSerialize.class)
public @interface SensitiveWrapped {

    /**
     * 脱敏类型
     * @return
     */
    SensitiveEnum value();
}
```
<a name="LaYId"></a>
#### 2.4、编写脱敏序列化类
```java
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.BeanProperty;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.ser.ContextualSerializer;
import java.io.IOException;
import java.util.Objects;

public class SensitiveSerialize extends JsonSerializer<String> implements ContextualSerializer {

    /**
     * 脱敏类型
     */
    private SensitiveEnum type;

    
    @Override
    public void serialize(String s, JsonGenerator jsonGenerator, SerializerProvider serializerProvider) throws IOException {
        switch (this.type) {
            case CHINESE_NAME: {
                jsonGenerator.writeString(SensitiveInfoUtils.chineseName(s));
                break;
            }
            case ID_CARD: {
                jsonGenerator.writeString(SensitiveInfoUtils.idCardNum(s));
                break;
            }
            case FIXED_PHONE: {
                jsonGenerator.writeString(SensitiveInfoUtils.fixedPhone(s));
                break;
            }
            case MOBILE_PHONE: {
                jsonGenerator.writeString(SensitiveInfoUtils.mobilePhone(s));
                break;
            }
            case ADDRESS: {
                jsonGenerator.writeString(SensitiveInfoUtils.address(s, 4));
                break;
            }
            case EMAIL: {
                jsonGenerator.writeString(SensitiveInfoUtils.email(s));
                break;
            }
            case BANK_CARD: {
                jsonGenerator.writeString(SensitiveInfoUtils.bankCard(s));
                break;
            }
            case CNAPS_CODE: {
                jsonGenerator.writeString(SensitiveInfoUtils.cnapsCode(s));
                break;
            }
        }
    }

    @Override
    public JsonSerializer<?> createContextual(SerializerProvider serializerProvider, BeanProperty beanProperty) throws JsonMappingException {
        // 为空直接跳过
        if (beanProperty != null) {
            // 非 String 类直接跳过
            if (Objects.equals(beanProperty.getType().getRawClass(), String.class)) {
                SensitiveWrapped sensitiveWrapped = beanProperty.getAnnotation(SensitiveWrapped.class);
                if (sensitiveWrapped == null) {
                    sensitiveWrapped = beanProperty.getContextAnnotation(SensitiveWrapped.class);
                }
                if (sensitiveWrapped != null) {
                    // 如果能得到注解，就将注解的 value 传入 SensitiveSerialize
                    return new SensitiveSerialize(sensitiveWrapped.value());
                }
            }
            return serializerProvider.findValueSerializer(beanProperty.getType(), beanProperty);
        }
        return serializerProvider.findNullValueSerializer(beanProperty);
    }

    public SensitiveSerialize() {}

    public SensitiveSerialize(final SensitiveEnum type) {
        this.type = type;
    }
}
```
其中`createContextual`的作用是通过字段已知的上下文信息定制`JsonSerializer`对象。
<a name="BwQDY"></a>
#### 2.4、编写脱敏工具类
```java
import org.apache.commons.lang3.StringUtils;

public class SensitiveInfoUtils {

    /**
     * [中文姓名] 只显示第一个汉字，其他隐藏为2个星号<例子：李**>
     */
    public static String chineseName(final String fullName) {
        if (StringUtils.isBlank(fullName)) {
            return "";
        }
        final String name = StringUtils.left(fullName, 1);
        return StringUtils.rightPad(name, StringUtils.length(fullName), "*");
    }

    /**
     * [中文姓名] 只显示第一个汉字，其他隐藏为2个星号<例子：李**>
     */
    public static String chineseName(final String familyName, final String givenName) {
        if (StringUtils.isBlank(familyName) || StringUtils.isBlank(givenName)) {
            return "";
        }
        return chineseName(familyName + givenName);
    }

    /**
     * [身份证号] 显示最后四位，其他隐藏。共计18位或者15位。<例子：420**********5762>
     */
    public static String idCardNum(final String id) {
        if (StringUtils.isBlank(id)) {
            return "";
        }

        return StringUtils.left(id, 3).concat(StringUtils
                .removeStart(StringUtils.leftPad(StringUtils.right(id, 4), StringUtils.length(id), "*"),
                        "***"));
    }

    /**
     * [固定电话] 后四位，其他隐藏<例子：****1234>
     */
    public static String fixedPhone(final String num) {
        if (StringUtils.isBlank(num)) {
            return "";
        }
        return StringUtils.leftPad(StringUtils.right(num, 4), StringUtils.length(num), "*");
    }

    /**
     * [手机号码] 前三位，后四位，其他隐藏<例子:138******1234>
     */
    public static String mobilePhone(final String num) {
        if (StringUtils.isBlank(num)) {
            return "";
        }
        return StringUtils.left(num, 3).concat(StringUtils
                .removeStart(StringUtils.leftPad(StringUtils.right(num, 4), StringUtils.length(num), "*"),
                        "***"));

    }

    /**
     * [地址] 只显示到地区，不显示详细地址；要对个人信息增强保护<例子：北京市海淀区****>
     *
     * @param sensitiveSize 敏感信息长度
     */
    public static String address(final String address, final int sensitiveSize) {
        if (StringUtils.isBlank(address)) {
            return "";
        }
        final int length = StringUtils.length(address);
        return StringUtils.rightPad(StringUtils.left(address, length - sensitiveSize), length, "*");
    }

    /**
     * [电子邮箱] 邮箱前缀仅显示第一个字母，前缀其他隐藏，用星号代替，@及后面的地址显示<例子:g**@163.com>
     */
    public static String email(final String email) {
        if (StringUtils.isBlank(email)) {
            return "";
        }
        final int index = StringUtils.indexOf(email, "@");
        if (index <= 1) {
            return email;
        } else {
            return StringUtils.rightPad(StringUtils.left(email, 1), index, "*")
                    .concat(StringUtils.mid(email, index, StringUtils.length(email)));
        }
    }

    /**
     * [银行卡号] 前六位，后四位，其他用星号隐藏每位1个星号<例子:6222600**********1234>
     */
    public static String bankCard(final String cardNum) {
        if (StringUtils.isBlank(cardNum)) {
            return "";
        }
        return StringUtils.left(cardNum, 6).concat(StringUtils.removeStart(
                StringUtils.leftPad(StringUtils.right(cardNum, 4), StringUtils.length(cardNum), "*"),
                "******"));
    }

    /**
     * [公司开户银行联号] 公司开户银行联行号,显示前两位，其他用星号隐藏，每位1个星号<例子:12********>
     */
    public static String cnapsCode(final String code) {
        if (StringUtils.isBlank(code)) {
            return "";
        }
        return StringUtils.rightPad(StringUtils.left(code, 2), StringUtils.length(code), "*");
    }

}
```
<a name="bE7VM"></a>
#### 2.5、编写测试实体类
最后，编写一个实体类`UserEntity`，看看转换后的效果如何？
```java
public class UserEntity {

    /**
     * 用户ID
     */
    private Long userId;

    /**
     * 用户姓名
     */
    private String name;

    /**
     * 手机号
     */
    @SensitiveWrapped(SensitiveEnum.MOBILE_PHONE)
    private String mobile;

    /**
     * 身份证号码
     */
    @SensitiveWrapped(SensitiveEnum.ID_CARD)
    private String idCard;

    /**
     * 年龄
     */
    private String sex;

    /**
     * 性别
     */
    private int age;

    //省略get、set...
}
```
测试程序如下：
```java
public class SensitiveDemo {

    public static void main(String[] args) throws JsonProcessingException {
        UserEntity userEntity = new UserEntity();
        userEntity.setUserId(1l);
        userEntity.setName("张三");
        userEntity.setMobile("18000000001");
        userEntity.setIdCard("420117200001011000008888");
        userEntity.setAge(20);
        userEntity.setSex("男");

        //通过jackson方式，将对象序列化成json字符串
        ObjectMapper objectMapper = new ObjectMapper();
        System.out.println(objectMapper.writeValueAsString(userEntity));
    }
}
```
结果如下：
```json
{"userId":1,"name":"张三","mobile":"180****0001","idCard":"420*****************8888","sex":"男","age":20}
```
很清晰的看到，转换结果成功！<br />如果当前的项目是基于SpringMVC框架进行开发的，那么在对象返回的时候，框架会自动采用jackson框架进行序列化。
```java
@RequestMapping("/hello")
public UserEntity hello() {
    UserEntity userEntity = new UserEntity();
    userEntity.setUserId(1l);
    userEntity.setName("张三");
    userEntity.setMobile("18000000001");
    userEntity.setIdCard("420117200001011000008888");
    userEntity.setAge(20);
    userEntity.setSex("男");
    return userEntity;
}
```
请求网页http://127.0.0.1:8080/hello，结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1634859982797-36083ef9-91c3-4394-9590-75a3ca111363.webp#clientId=u76735bd5-65ec-4&from=paste&id=ucba9e9ef&originHeight=129&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ub63f879e-a112-4f54-a641-8abc118e2b6)
<a name="tbBDe"></a>
### 三、小结
在实际的业务场景开发中，采用注解方式进行全局数据脱敏处理，可以有效的解决敏感数据隐私泄露的问题。
