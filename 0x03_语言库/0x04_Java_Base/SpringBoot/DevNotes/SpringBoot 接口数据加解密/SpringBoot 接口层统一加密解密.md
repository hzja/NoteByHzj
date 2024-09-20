JavaSpringBoot
<a name="m5CJr"></a>
## 1、介绍
在日常的Java开发中，免不了和其他系统的业务交互，或者微服务之间的接口调用<br />如果想保证数据传输的安全，对接口出参加密，入参解密。<br />但是不想写重复代码，可以提供一个通用starter，提供通用加密解密功能
<a name="u9r1y"></a>
## 2、前置知识
<a name="hYBu6"></a>
### 2.1 hutool-crypto加密解密工具
hutool-crypto提供了很多加密解密工具，包括对称加密，非对称加密，摘要加密等等，这不做详细介绍。
<a name="ydEoc"></a>
### 2.2 request流只能读取一次的问题
<a name="u9aNL"></a>
##### 2.2.1 问题
在接口调用链中，request的请求流只能调用一次，处理之后，如果之后还需要用到请求流获取数据，就会发现数据为空。<br />比如使用了filter或者aop在接口处理之前，获取了`request`中的数据，对参数进行了校验，那么之后就不能在获取request请求流了
<a name="TAsta"></a>
##### 2.2.2 解决办法
继承`HttpServletRequestWrapper`，将请求中的流copy一份，复写`getInputStream`和`getReader`方法供外部使用。每次调用后的`getInputStream`方法都是从复制出来的二进制数组中进行获取，这个二进制数组在对象存在期间一致存在。<br />使用Filter过滤器，在一开始，替换request为自己定义的可以多次读取流的request。<br />这样就实现了流的重复获取<br />`InputStreamHttpServletRequestWrapper`
```java
package xyz.hlh.cryptotest.utils;

import org.apache.commons.io.IOUtils;

import javax.servlet.ReadListener;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * 请求流支持多次获取
 */
public class InputStreamHttpServletRequestWrapper extends HttpServletRequestWrapper {

    /**
     * 用于缓存输入流
     */
    private ByteArrayOutputStream cachedBytes;

    public InputStreamHttpServletRequestWrapper(HttpServletRequest request) {
        super(request);
    }

    @Override
    public ServletInputStream getInputStream() throws IOException {
        if (cachedBytes == null) {
            // 首次获取流时，将流放入 缓存输入流 中
            cacheInputStream();
        }

        // 从 缓存输入流 中获取流并返回
        return new CachedServletInputStream(cachedBytes.toByteArray());
    }

    @Override
    public BufferedReader getReader() throws IOException {
        return new BufferedReader(new InputStreamReader(getInputStream()));
    }

    /**
     * 首次获取流时，将流放入 缓存输入流 中
     */
    private void cacheInputStream() throws IOException {
        // 缓存输入流以便多次读取。为了方便, 我使用 org.apache.commons IOUtils
        cachedBytes = new ByteArrayOutputStream();
        IOUtils.copy(super.getInputStream(), cachedBytes);
    }

    /**
     * 读取缓存的请求正文的输入流
     * <p>
     * 用于根据 缓存输入流 创建一个可返回的
     */
    public static class CachedServletInputStream extends ServletInputStream {

        private final ByteArrayInputStream input;

        public CachedServletInputStream(byte[] buf) {
            // 从缓存的请求正文创建一个新的输入流
            input = new ByteArrayInputStream(buf);
        }

        @Override
        public boolean isFinished() {
            return false;
        }

        @Override
        public boolean isReady() {
            return false;
        }

        @Override
        public void setReadListener(ReadListener listener) {

        }

        @Override
        public int read() throws IOException {
            return input.read();
        }
    }

}
```
`HttpServletRequestInputStreamFilter`
```java
package xyz.hlh.cryptotest.filter;

import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;
import xyz.hlh.cryptotest.utils.InputStreamHttpServletRequestWrapper;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

import static org.springframework.core.Ordered.HIGHEST_PRECEDENCE;

/**
 * @description:
 *      请求流转换为多次读取的请求流 过滤器
 */
@Component
@Order(HIGHEST_PRECEDENCE + 1)  // 优先级最高
public class HttpServletRequestInputStreamFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {

        // 转换为可以多次获取流的request
        HttpServletRequest httpServletRequest = (HttpServletRequest) request;
        InputStreamHttpServletRequestWrapper inputStreamHttpServletRequestWrapper = new InputStreamHttpServletRequestWrapper(httpServletRequest);

        // 放行
        chain.doFilter(inputStreamHttpServletRequestWrapper, response);
    }
}
```
<a name="C2gRC"></a>
### 2.3 SpringBoot的参数校验`validation`
为了减少接口中，业务代码之前的大量冗余的参数校验代码<br />SpringBoot-validation提供了优雅的参数校验，入参都是实体类，在实体类字段上加上对应注解，就可以在进入方法之前，进行参数校验，如果参数错误，会抛出错误`BindException`，是不会进入方法的。<br />这种方法，必须要求在接口参数上加注解`@Validated`或者是`@Valid`<br />但是很多清空下，希望在代码中调用某个实体类的校验功能，所以需要如下工具类<br />`ParamException`
```java
package xyz.hlh.cryptotest.exception;

import lombok.Getter;

import java.util.List;

/**
 * @description 自定义参数异常
 */
@Getter
public class ParamException extends Exception {

    private final List<String> fieldList;
    private final List<String> msgList;

    public ParamException(List<String> fieldList, List<String> msgList) {
        this.fieldList = fieldList;
        this.msgList = msgList;
    }
}
```
ValidationUtils
```java
package xyz.hlh.cryptotest.utils;

import xyz.hlh.cryptotest.exception.CustomizeException;
import xyz.hlh.cryptotest.exception.ParamException;

import javax.validation.ConstraintViolation;
import javax.validation.Validation;
import javax.validation.Validator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

/**
 * @description 验证工具类
 */
public class ValidationUtils {

    private static final Validator VALIDATOR = Validation.buildDefaultValidatorFactory().getValidator();

    /**
     * 验证数据
     * @param object 数据
     */
    public static void validate(Object object) throws CustomizeException {

        Set<ConstraintViolation<Object>> validate = VALIDATOR.validate(object);

        // 验证结果异常
        throwParamException(validate);
    }

    /**
     * 验证数据(分组)
     * @param object 数据
     * @param groups 所在组
     */
    public static void validate(Object object, Class<?> ... groups) throws CustomizeException {

        Set<ConstraintViolation<Object>> validate = VALIDATOR.validate(object, groups);

        // 验证结果异常
        throwParamException(validate);
    }

    /**
     * 验证数据中的某个字段(分组)
     * @param object 数据
     * @param propertyName 字段名称
     */
    public static void validate(Object object, String propertyName) throws CustomizeException {
        Set<ConstraintViolation<Object>> validate = VALIDATOR.validateProperty(object, propertyName);

        // 验证结果异常
        throwParamException(validate);

    }

    /**
     * 验证数据中的某个字段(分组)
     * @param object 数据
     * @param propertyName 字段名称
     * @param groups 所在组
     */
    public static void validate(Object object, String propertyName, Class<?> ... groups) throws CustomizeException {

        Set<ConstraintViolation<Object>> validate = VALIDATOR.validateProperty(object, propertyName, groups);

        // 验证结果异常
        throwParamException(validate);

    }

    /**
     * 验证结果异常
     * @param validate 验证结果
     */
    private static void throwParamException(Set<ConstraintViolation<Object>> validate) throws CustomizeException {
        if (validate.size() > 0) {
            List<String> fieldList = new LinkedList<>();
            List<String> msgList = new LinkedList<>();
            for (ConstraintViolation<Object> next : validate) {
                fieldList.add(next.getPropertyPath().toString());
                msgList.add(next.getMessage());
            }

            throw new ParamException(fieldList, msgList);
        }
    }

}
```
<a name="KR3dU"></a>
### 2.5 自定义starter
自定义starter步骤

- 创建工厂，编写功能代码
- 声明自动配置类，把需要对外提供的对象创建好，通过配置类统一向外暴露
- 在resource目录下准备一个名为spring/spring.factories的文件，以org.springframework.boot.autoconfigure.EnableAutoConfiguration为key，自动配置类为value列表，进行注册
<a name="RCdCJ"></a>
### 2.6 `RequestBodyAdvice`和`ResponseBodyAdvice`

- `RequestBodyAdvice`是对请求的json串进行处理， 一般使用环境是处理接口参数的自动解密
- `ResponseBodyAdvice`是对请求相应的jsoin传进行处理，一般用于相应结果的加密
<a name="wJPs9"></a>
## 3、功能介绍
接口相应数据的时候，返回的是加密之后的数据 接口入参的时候，接收的是解密之后的数据，但是在进入接口之前，会自动解密，取得对应的数据
<a name="vwd3B"></a>
## 4、功能细节
加密解密使用对称加密的AES算法，使用hutool-crypto模块进行实现<br />所有的实体类提取一个公共父类，包含属性时间戳，用于加密数据返回之后的实效性，如果超过60分钟，那么其他接口将不进行处理。<br />如果接口加了加密注解`EncryptionAnnotation`，并且返回统一的json数据Result类，则自动对数据进行加密。如果是继承了统一父类`RequestBase`的数据，自动注入时间戳，确保数据的时效性<br />如果接口加了解密注解`DecryptionAnnotation`，并且参数使用RequestBody注解标注，传入json使用统一格式RequestData类，并且内容是继承了包含时间长的父类`RequestBase`，则自动解密，并且转为对应的数据类型<br />功能提供Springboot的starter，实现开箱即用
<a name="Vodcw"></a>
## 5、代码实现
[https://gitee.com/springboot-hlh/spring-boot-csdn/tree/master/09-spring-boot-interface-crypto](https://gitee.com/springboot-hlh/spring-boot-csdn/tree/master/09-spring-boot-interface-crypto)
<a name="EWUri"></a>
### 5.1 项目结构
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668750052864-798664a6-5d1b-490a-8ac1-a7fd1444b914.png#averageHue=%23f3f1f0&clientId=u3d415c21-0295-4&from=paste&id=udade560d&originHeight=148&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua8ffb086-edd5-4d99-84c2-d30979b44aa&title=)
<a name="A9rBu"></a>
### 5.2 crypto-common
<a name="hkYal"></a>
##### 5.2.1 结构
![2022-11-18-13-39-16.736530300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668750371792-afa0a5b8-bb7f-4d10-af58-5c63aeef411f.png#averageHue=%23f9f7f5&clientId=u3d415c21-0295-4&from=ui&id=u94727cd7&originHeight=464&originWidth=500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69902&status=done&style=none&taskId=ucf91d03e-4dc7-4379-b2c3-8c553d28ff9&title=)
<a name="R7vUY"></a>
### 5.3 crypto-spring-boot-starter
<a name="xEcXJ"></a>
##### 5.3.1 接口
![2022-11-18-13-39-16.834521300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668750371788-c2c34ee1-2599-4e21-828e-65fa5c65c7b3.png#averageHue=%23fbf9f7&clientId=u3d415c21-0295-4&from=ui&id=Kd5Ls&originHeight=521&originWidth=449&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62256&status=done&style=none&taskId=uea66c97d-e368-4c66-9558-5bb979eac8b&title=)
<a name="NCLqY"></a>
##### 5.3.2 重要代码
crypto.properties AES需要的参数配置
```
# 模式    cn.hutool.crypto.Mode
crypto.mode=CTS
# 补码方式 cn.hutool.crypto.Mode
crypto.padding=PKCS5Padding
# 秘钥
crypto.key=testkey123456789
# 盐
crypto.iv=testiv1234567890
```
spring.factories 自动配置文件
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
xyz.hlh.crypto.config.AppConfig
```
CryptConfig AES需要的配置参数
```java
package xyz.hlh.crypto.config;

import cn.hutool.crypto.Mode;
import cn.hutool.crypto.Padding;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;

import java.io.Serializable;

/**
 * @description: AES需要的配置参数
 */
@Configuration
@ConfigurationProperties(prefix = "crypto")
@PropertySource("classpath:crypto.properties")
@Data
@EqualsAndHashCode
@Getter
public class CryptConfig implements Serializable {

    private Mode mode;
    private Padding padding;
    private String key;
    private String iv;

}
```
AppConfig 自动配置类
```java
package xyz.hlh.crypto.config;

import cn.hutool.crypto.symmetric.AES;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.annotation.Resource;
import java.nio.charset.StandardCharsets;

/**
 * @description: 自动配置类
 */
@Configuration
public class AppConfig {

    @Resource
    private CryptConfig cryptConfig;

    @Bean
    public AES aes() {
        return new AES(cryptConfig.getMode(), cryptConfig.getPadding(), cryptConfig.getKey().getBytes(StandardCharsets.UTF_8), cryptConfig.getIv().getBytes(StandardCharsets.UTF_8));
    }

}
```
`DecryptRequestBodyAdvice` 请求自动解密
```java
package xyz.hlh.crypto.advice;

import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.SneakyThrows;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.MethodParameter;
import org.springframework.http.HttpInputMessage;
import org.springframework.http.converter.HttpMessageConverter;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.context.request.RequestAttributes;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import org.springframework.web.servlet.mvc.method.annotation.RequestBodyAdvice;
import xyz.hlh.crypto.annotation.DecryptionAnnotation;
import xyz.hlh.crypto.common.exception.ParamException;
import xyz.hlh.crypto.constant.CryptoConstant;
import xyz.hlh.crypto.entity.RequestBase;
import xyz.hlh.crypto.entity.RequestData;
import xyz.hlh.crypto.util.AESUtil;

import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.lang.reflect.Type;

/**
 * @description: requestBody 自动解密
 */
@ControllerAdvice
public class DecryptRequestBodyAdvice implements RequestBodyAdvice {

    @Autowired
    private ObjectMapper objectMapper;

    /**
     * 方法上有DecryptionAnnotation注解的，进入此拦截器
     * @param methodParameter 方法参数对象
     * @param targetType 参数的类型
     * @param converterType 消息转换器
     * @return true，进入，false，跳过
     */
    @Override
    public boolean supports(MethodParameter methodParameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) {
        return methodParameter.hasMethodAnnotation(DecryptionAnnotation.class);
    }

    @Override
    public HttpInputMessage beforeBodyRead(HttpInputMessage inputMessage, MethodParameter parameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) throws IOException {
        return inputMessage;
    }

    /**
     * 转换之后，执行此方法，解密，赋值
     * @param body spring解析完的参数
     * @param inputMessage 输入参数
     * @param parameter 参数对象
     * @param targetType 参数类型
     * @param converterType 消息转换类型
     * @return 真实的参数
     */
    @SneakyThrows
    @Override
    public Object afterBodyRead(Object body, HttpInputMessage inputMessage, MethodParameter parameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) {

        // 获取request
        RequestAttributes requestAttributes = RequestContextHolder.getRequestAttributes();
        ServletRequestAttributes servletRequestAttributes = (ServletRequestAttributes) requestAttributes;
        if (servletRequestAttributes == null) {
            throw new ParamException("request错误");
        }

        HttpServletRequest request = servletRequestAttributes.getRequest();

        // 获取数据
        ServletInputStream inputStream = request.getInputStream();
        RequestData requestData = objectMapper.readValue(inputStream, RequestData.class);

        if (requestData == null || StringUtils.isBlank(requestData.getText())) {
            throw new ParamException("参数错误");
        }

        // 获取加密的数据
        String text = requestData.getText();

        // 放入解密之前的数据
        request.setAttribute(CryptoConstant.INPUT_ORIGINAL_DATA, text);

        // 解密
        String decryptText = null;
        try {
            decryptText = AESUtil.decrypt(text);
        } catch (Exception e) {
            throw new ParamException("解密失败");
        }

        if (StringUtils.isBlank(decryptText)) {
            throw new ParamException("解密失败");
        }

        // 放入解密之后的数据
        request.setAttribute(CryptoConstant.INPUT_DECRYPT_DATA, decryptText);

        // 获取结果
        Object result = objectMapper.readValue(decryptText, body.getClass());

        // 强制所有实体类必须继承RequestBase类，设置时间戳
        if (result instanceof RequestBase) {
            // 获取时间戳
            Long currentTimeMillis = ((RequestBase) result).getCurrentTimeMillis();
            // 有效期 60秒
            long effective = 60*1000;

            // 时间差
            long expire = System.currentTimeMillis() - currentTimeMillis;

            // 是否在有效期内
            if (Math.abs(expire) > effective) {
                throw new ParamException("时间戳不合法");
            }

            // 返回解密之后的数据
            return result;
        } else {
            throw new ParamException(String.format("请求参数类型：%s 未继承：%s", result.getClass().getName(), RequestBase.class.getName()));
        }
    }

    /**
     * 如果body为空，转为空对象
     * @param body spring解析完的参数
     * @param inputMessage 输入参数
     * @param parameter 参数对象
     * @param targetType 参数类型
     * @param converterType 消息转换类型
     * @return 真实的参数
     */
    @SneakyThrows
    @Override
    public Object handleEmptyBody(Object body, HttpInputMessage inputMessage, MethodParameter parameter, Type targetType, Class<? extends HttpMessageConverter<?>> converterType) {
        String typeName = targetType.getTypeName();
        Class<?> bodyClass = Class.forName(typeName);
        return bodyClass.newInstance();
    }
}
```
`EncryptResponseBodyAdvice` 相应自动加密
```java
package xyz.hlh.crypto.advice;

import cn.hutool.json.JSONUtil;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.SneakyThrows;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.MethodParameter;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.HttpMessageConverter;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.ResponseBodyAdvice;
import sun.reflect.generics.reflectiveObjects.ParameterizedTypeImpl;
import xyz.hlh.crypto.annotation.EncryptionAnnotation;
import xyz.hlh.crypto.common.entity.Result;
import xyz.hlh.crypto.common.exception.CryptoException;
import xyz.hlh.crypto.entity.RequestBase;
import xyz.hlh.crypto.util.AESUtil;

import java.lang.reflect.Type;

@ControllerAdvice
public class EncryptResponseBodyAdvice implements ResponseBodyAdvice<Result<?>> {

    @Autowired
    private ObjectMapper objectMapper;

    @Override
    public boolean supports(MethodParameter returnType, Class<? extends HttpMessageConverter<?>> converterType) {
        ParameterizedTypeImpl genericParameterType = (ParameterizedTypeImpl)returnType.getGenericParameterType();

        // 如果直接是Result，则返回
        if (genericParameterType.getRawType() == Result.class && returnType.hasMethodAnnotation(EncryptionAnnotation.class)) {
            return true;
        }

        if (genericParameterType.getRawType() != ResponseEntity.class) {
            return false;
        }

        // 如果是ResponseEntity<Result>
        for (Type type : genericParameterType.getActualTypeArguments()) {
            if (((ParameterizedTypeImpl) type).getRawType() == Result.class && returnType.hasMethodAnnotation(EncryptionAnnotation.class)) {
                return true;
            }
        }

        return false;
    }

    @SneakyThrows
    @Override
    public Result<?> beforeBodyWrite(Result<?> body, MethodParameter returnType, MediaType selectedContentType, Class<? extends HttpMessageConverter<?>> selectedConverterType, ServerHttpRequest request, ServerHttpResponse response) {

        // 加密
        Object data = body.getData();

        // 如果data为空，直接返回
        if (data == null) {
            return body;
        }

        // 如果是实体，并且继承了Request，则放入时间戳
        if (data instanceof RequestBase) {
            ((RequestBase)data).setCurrentTimeMillis(System.currentTimeMillis());
        }

        String dataText = JSONUtil.toJsonStr(data);

        // 如果data为空，直接返回
        if (StringUtils.isBlank(dataText)) {
            return body;
        }

        // 如果位数小于16，报错
        if (dataText.length() < 16) {
            throw new CryptoException("加密失败，数据小于16位");
        }

        String encryptText = AESUtil.encryptHex(dataText);

        return Result.builder()
                .status(body.getStatus())
                .data(encryptText)
                .message(body.getMessage())
                .build();
    }
}
```
<a name="HjYUV"></a>
### 5.4 crypto-test
<a name="wyvUe"></a>
##### 5.4.1 结构
![2022-11-18-13-39-17.044675000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668750371887-3b412ea1-385b-4fc4-ba2c-30f29e870edf.png#averageHue=%23fcfbf9&clientId=u3d415c21-0295-4&from=ui&id=dvFHs&originHeight=301&originWidth=410&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20341&status=done&style=none&taskId=u3bc2ad76-b069-4512-a516-c8f875972f6&title=)
<a name="bvySE"></a>
##### 5.4.2 重要代码
application.yml 配置文件
```yaml
spring:
  mvc:
    format:
      date-time: yyyy-MM-dd HH:mm:ss
      date: yyyy-MM-dd
  # 日期格式化
  jackson:
    date-format: yyyy-MM-dd HH:mm:ss
```
Teacher 实体类
```java
package xyz.hlh.crypto.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import org.hibernate.validator.constraints.Range;

import javax.validation.constraints.NotBlank;
import javax.validation.constraints.NotNull;
import java.io.Serializable;
import java.util.Date;

/**
 * @description: Teacher实体类，使用SpringBoot的validation校验
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class Teacher extends RequestBase implements Serializable {

    @NotBlank(message = "姓名不能为空")
    private String name;
    @NotNull(message = "年龄不能为空")
    @Range(min = 0, max = 150, message = "年龄不合法")
    private Integer age;
    @NotNull(message = "生日不能为空")
    private Date birthday;

}
```
TestController 测试Controller
```java
package xyz.hlh.crypto.controller;

import org.springframework.http.ResponseEntity;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import xyz.hlh.crypto.annotation.DecryptionAnnotation;
import xyz.hlh.crypto.annotation.EncryptionAnnotation;
import xyz.hlh.crypto.common.entity.Result;
import xyz.hlh.crypto.common.entity.ResultBuilder;
import xyz.hlh.crypto.entity.Teacher;

/**
 * @description: 测试Controller
 */
@RestController
public class TestController implements ResultBuilder {

    /**
     * 直接返回对象，不加密
     * @param teacher Teacher对象
     * @return 不加密的对象
     */
    @PostMapping("/get")
    public ResponseEntity<Result<?>> get(@Validated @RequestBody Teacher teacher) {
        return success(teacher);
    }

    /**
     * 返回加密后的数据
     * @param teacher Teacher对象
     * @return 返回加密后的数据 ResponseBody<Result>格式
     */
    @PostMapping("/encrypt")
    @EncryptionAnnotation
    public ResponseEntity<Result<?>> encrypt(@Validated @RequestBody Teacher teacher) {
        return success(teacher);
    }

    /**
     * 返回加密后的数据
     * @param teacher Teacher对象
     * @return 返回加密后的数据 Result格式
     */
    @PostMapping("/encrypt1")
    @EncryptionAnnotation
    public Result<?> encrypt1(@Validated @RequestBody Teacher teacher) {
        return success(teacher).getBody();
    }

    /**
     * 返回解密后的数据
     * @param teacher Teacher对象
     * @return 返回解密后的数据
     */
    @PostMapping("/decrypt")
    @DecryptionAnnotation
    public ResponseEntity<Result<?>> decrypt(@Validated @RequestBody Teacher teacher) {
        return success(teacher);
    }

}
```
