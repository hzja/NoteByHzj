Java Dubbo<br />目前架构是网关直接通过泛化调用Dubbo服务，不同于web Controller使用SpringMVC模块来做到参数注解校验。不过不用担心Dubbo也考虑到了这一点，基于SPI机制提供了`ValidationFilter`。来看看他是如何实现的。
<a name="xOypt"></a>
## Dubbo 源码实现
<a name="BHpou"></a>
### Dubbo SPI定义
什么是Dubbo SPI？简单说是通过文件配置对应class路径后会被执行class里的`invoke`函数。其中的实现原理大家顺着Dubbo的`ExtensionLoader`去看下源码就能知道。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638537871740-b6223426-76a6-481d-9cb2-61b519443135.webp#clientId=u6fdac9db-540c-4&from=paste&id=ue17fb742&originHeight=572&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uea22a879-2381-4cb7-883e-e5f26ad4ef3&title=)
<a name="RGH4j"></a>
### ValidationFilter说明
```java
//在哪种服务类型激活
//这里的VALIDATION_KEY=“validation” 也就是我们在SPI中需要把key按这个规定定义
@Activate(group = {CONSUMER, PROVIDER}, value = VALIDATION_KEY, order = 10000)
public class ValidationFilter implements Filter {
    private Validation validation;

    public void setValidation(Validation validation) {
        this.validation = validation;
    }

    @Override
    public Result invoke(Invoker<?> invoker, Invocation invocation) throws RpcException {
        //如果SPI中定义了validation 那么就进行校验
        if (validation != null && !invocation.getMethodName().startsWith("$")
            && ConfigUtils.isNotEmpty(invoker.getUrl().getMethodParameter(invocation.getMethodName(), VALIDATION_KEY))) {
            try {
                //执行参数校验
                Validator validator = validation.getValidator(invoker.getUrl());
                if (validator != null) {
                    validator.validate(invocation.getMethodName(), invocation.getParameterTypes(), invocation.getArguments());
                }
            } catch (RpcException e) {
                throw e;
            } catch (ValidationException e) {
                //抛出异常 这里的ValidationException需要深挖一下，后面会说
                // only use exception's message to avoid potential serialization issue
                return AsyncRpcResult.newDefaultAsyncResult(new ValidationException(e.getMessage()), invocation);
            } catch (Throwable t) {
                return AsyncRpcResult.newDefaultAsyncResult(t, invocation);
            }
        }
        return invoker.invoke(invocation);
    }

}
```
<a name="Bv3PI"></a>
## 基础使用
<a name="ItWoQ"></a>
### Maven依赖
<a name="pNdMn"></a>
#### SpringBoot项目推荐使用
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```
手动依赖
```xml
<dependency>
  <groupId>javax.el</groupId>
  <artifactId>javax.el-api</artifactId>
  <version>3.0.0</version>
</dependency>
<dependency>
  <groupId>org.glassfish</groupId>
  <artifactId>javax.el</artifactId>
</dependency>
<dependency>
  <groupId>javax.validation</groupId>
  <artifactId>validation-api</artifactId>
</dependency>
<dependency>
  <groupId>org.hibernate</groupId>
  <artifactId>hibernate-validator</artifactId>
</dependency>
```
<a name="wOjSK"></a>
### DTO添加validation定义
```java
import lombok.Data;

import javax.validation.constraints.NotBlank;
import javax.validation.constraints.NotNull;
import java.io.Serializable;

@Data
public class PracticeParam implements Serializable {
    @NotNull(message = "periodId不能为空")
    private Long periodId;
}
```
<a name="uUpNo"></a>
### 服务者interface
```java
public interface IPracticeService {
    boolean practiceAdd(PracticeParam practiceParam);
}
```
<a name="IdL69"></a>
### Dubbo RPC单元测试
```java
@SpringBootTest(classes = ClientApplication.class)
@RunWith(SpringRunner.class)
@Slf4j
public class PrecticeTest {
    @DubboReference(group = "user")
    private IPracticeService practiceLogicService;

    @Test
    public void add(){
        PracticeParam practiceParam=new PracticeParam();
        log.info(String.valueOf(practiceLogicService.practiceAdd(practiceParam)));
    }
}
```
测试结果
```java
javax.validation.ValidationException: Failed to validate service: com.xx.contract.IPracticeService, method: practiceAdd, cause: [ConstraintViolationImpl{interpolatedMessage='periodId不能为空', propertyPath=periodId, rootBeanClass=class com.xx.request.PracticeParam, messageTemplate='periodId不能为空'}]
```
看结果是生效了，不过这和实际项目中还有些距离。不能把这个异常抛给网关，根据项目需要适配上全局异常试试。
<a name="vU3uA"></a>
## 源码分析与扩展
<a name="LfWXi"></a>
### Dubbo异常处理
再回到前面SPI文件<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638537872143-6fedff6c-b956-4a29-8820-e6b1fcb8d1ee.webp#clientId=u6fdac9db-540c-4&from=paste&id=uffaebd58&originHeight=737&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3c8bb6cb-e857-4636-b0f3-497a5064468&title=)
<a name="fFhoL"></a>
## ExceotionFilter源码分析
```java
//在服务提供者端生效
@Activate(group = CommonConstants.PROVIDER)
public class ExceptionFilter implements Filter, Filter.Listener {
    private Logger logger = LoggerFactory.getLogger(ExceptionFilter.class);
    @Override
    public Result invoke(Invoker<?> invoker, Invocation invocation) throws RpcException {
        return invoker.invoke(invocation);
    }
    @Override
    public void onResponse(Result appResponse, Invoker<?> invoker, Invocation invocation) {
        //异常处理逻辑
        if (appResponse.hasException() && GenericService.class != invoker.getInterface()) {
            try {
                Throwable exception = appResponse.getException();

                // directly throw if it's checked exception
                if (!(exception instanceof RuntimeException) && (exception instanceof Exception)) {
                    return;
                }
                // directly throw if the exception appears in the signature
                try {
                    Method method = invoker.getInterface().getMethod(invocation.getMethodName(), invocation.getParameterTypes());
                    Class<?>[] exceptionClassses = method.getExceptionTypes();
                    for (Class<?> exceptionClass : exceptionClassses) {
                        if (exception.getClass().equals(exceptionClass)) {
                            return;
                        }
                    }
                } catch (NoSuchMethodException e) {
                    return;
                }

                // for the exception not found in method's signature, print ERROR message in server's log.
                logger.error("Got unchecked and undeclared exception which called by " + RpcContext.getContext().getRemoteHost() + ". service: " + invoker.getInterface().getName() + ", method: " + invocation.getMethodName() + ", exception: " + exception.getClass().getName() + ": " + exception.getMessage(), exception);

                // directly throw if exception class and interface class are in the same jar file.
                String serviceFile = ReflectUtils.getCodeBase(invoker.getInterface());
                String exceptionFile = ReflectUtils.getCodeBase(exception.getClass());
                if (serviceFile == null || exceptionFile == null || serviceFile.equals(exceptionFile)) {
                    return;
                }
                // directly throw if it's JDK exception
                String className = exception.getClass().getName();
                if (className.startsWith("java.") || className.startsWith("javax.")) {
                    return;
                }
                // directly throw if it's dubbo exception
                if (exception instanceof RpcException) {
                    return;
                }
                // otherwise, wrap with RuntimeException and throw back to the client
                //重点时这句，替换异常信息
                appResponse.setException(new RuntimeException(StringUtils.toString(exception)));
            } catch (Throwable e) {
                logger.warn("Fail to ExceptionFilter when called by " + RpcContext.getContext().getRemoteHost() + ". service: " + invoker.getInterface().getName() + ", method: " + invocation.getMethodName() + ", exception: " + e.getClass().getName() + ": " + e.getMessage(), e);
            }
        }
    }
    @Override
    public void onError(Throwable e, Invoker<?> invoker, Invocation invocation) {
        logger.error("Got unchecked and undeclared exception which called by " + RpcContext.getContext().getRemoteHost() + ". service: " + invoker.getInterface().getName() + ", method: " + invocation.getMethodName() + ", exception: " + e.getClass().getName() + ": " + e.getMessage(), e);
    }
    // For test purpose
    public void setLogger(Logger logger) {
        this.logger = logger;
    }
}
```
<a name="ii8Zu"></a>
## 基础使用
<a name="rCLce"></a>
### 定义SPI
项目结构说明：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638537872500-40e7472d-ea2f-44ab-aece-37f3429cb132.webp#clientId=u6fdac9db-540c-4&from=paste&id=u7c3afa96&originHeight=227&originWidth=420&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc83c639-324a-40a8-ad54-e5168c49f7b&title=)<br />文件定义（Dubbo SPI需要严格按如下路径和文件名）
```java
src/main/resources/META-INF/dubbo/org.apache.dubbo.rpc.Filter
```
文件内容
```java
validation=com.xx.xx.config.DubboValidationFilter
exception=com.xx.xx.config.DubboExceptionFilter
```
<a name="rUDe1"></a>
### 自定义`DubboValidationFilter`
```java
import com.xx.exception.ParamException;
import org.apache.dubbo.common.extension.Activate;
import org.apache.dubbo.common.utils.ConfigUtils;
import org.apache.dubbo.rpc.*;
import org.apache.dubbo.validation.Validation;
import org.apache.dubbo.validation.Validator;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import java.util.Set;

import static org.apache.dubbo.common.constants.CommonConstants.PROVIDER;
import static org.apache.dubbo.common.constants.FilterConstants.VALIDATION_KEY;

@Activate(group = {PROVIDER}, value = VALIDATION_KEY, order = -1)
public class DubboValidationFilter implements Filter {

    private Validation validation;

    public void setValidation(Validation validation) {
        this.validation = validation;
    }

    @Override
    public Result invoke(Invoker<?> invoker, Invocation invocation) throws RpcException {
        if (validation != null && !invocation.getMethodName().startsWith("$")
            && ConfigUtils.isNotEmpty(invoker.getUrl().getMethodParameter(invocation.getMethodName(), VALIDATION_KEY))) {
            try {
                Validator validator = validation.getValidator(invoker.getUrl());
                if (validator != null) {
                    //挖掘点 validate函数的源码
                    validator.validate(invocation.getMethodName(), invocation.getParameterTypes(), invocation.getArguments());
                }
            }
            //Dubbo源码里捕获的是ValidationException这个异常,原始信息变成了字符串，所以接下来
            //通过JValidator源码分析进行如下扩展
            catch (ConstraintViolationException e) {
                //获取我们的异常，这里的异常时集合的，因为我们参数可能多个都不通过
                StringBuilder message = new StringBuilder();
                Set<ConstraintViolation<?>> violations = e.getConstraintViolations();
                for (ConstraintViolation<?> violation : violations) {
                    //这里只获取第一个不通过的原因
                    message.append(violation.getMessage().concat(";"));
                    break;
                }
                //项目自定义异常类型，网关可以捕获到该异常
                throw new ParamException(message.toString());
            } catch (RpcException e) {
                throw e;
            } catch (Throwable t) {
                return AsyncRpcResult.newDefaultAsyncResult(t, invocation);
            }
        }
        return invoker.invoke(invocation);
    }

}
```
<a name="Lbdln"></a>
### JValidator源码分析
```java
//Validated 校验过程
public void validate(String methodName, Class<?>[] parameterTypes, Object[] arguments) throws Exception {
  List<Class<?>> groups = new ArrayList<>();
  Class<?> methodClass = methodClass(methodName);
  if (methodClass != null) {
    groups.add(methodClass);
  }
  //异常返回信息 violations
  Set<ConstraintViolation<?>> violations = new HashSet<>();
  Method method = clazz.getMethod(methodName, parameterTypes);
  Class<?>[] methodClasses;
  if (method.isAnnotationPresent(MethodValidated.class)){
    methodClasses = method.getAnnotation(MethodValidated.class).value();
    groups.addAll(Arrays.asList(methodClasses));
  }
  // add into default group
  groups.add(0, Default.class);
  groups.add(1, clazz);

  // convert list to array
  Class<?>[] classgroups = groups.toArray(new Class[groups.size()]);

  Object parameterBean = getMethodParameterBean(clazz, method, arguments);
  if (parameterBean != null) {
    violations.addAll(validator.validate(parameterBean, classgroups ));
  }

  for (Object arg : arguments) {
    validate(violations, arg, classgroups);
  }

  if (!violations.isEmpty()) {
    logger.error("Failed to validate service: " + clazz.getName() + ", method: " + methodName + ", cause: " + violations);
    //这里原始异常是它，所以我们需要捕获它，得到violations
    throw new ConstraintViolationException("Failed to validate service: " + clazz.getName() + ", method: " + methodName + ", cause: " + violations, violations);
  }
}
```
<a name="I5lBZ"></a>
### 自定义`DubboExceptionFilter`
```java
@Slf4j
@Activate(group = CommonConstants.PROVIDER)
public class DubboExceptionFilter extends ExceptionFilter {
   @Override
   public Result invoke(Invoker<?> invoker, Invocation invocation) throws RpcException {
       return invoker.invoke(invocation);
  }
   @Override
   public void onResponse(Result appResponse, Invoker<?> invoker, Invocation invocation) {
       log.error("dubbo global exception ---------->{}", appResponse.getException());
       if (appResponse.hasException() && GenericService.class != invoker.getInterface()) {
           try {
               Throwable exception = appResponse.getException();

               // 自定义异常处理
               if (exception instanceof ParamException) {
              //按项目log收集规范输出
                   log.error("dubbo service exception ---------->{}", exception);
                   return;
              }
              ......
          } catch (Throwable e) {
               log.warn("Fail to ExceptionFilter when called by " + RpcContext.getContext().getRemoteHost() + ". service: " + invoker.getInterface().getName() + ", method: " + invocation.getMethodName() + ", exception: " + e.getClass().getName() + ": " + e.getMessage(), e);
          }
      }
  }
}
```
<a name="f8xPp"></a>
### PRC单元测试
```
org.apache.dubbo.remoting.RemotingException: com.xx.exception.ParamException: periodId不能为空;
com.xx.exception.ParamException: periodId不能为空;
```
<a name="IPdrz"></a>
## 高级进阶
在业务里一个DTO对象会用于新增或更新，在新增时不需要主键ID，在更新时需要主键ID。<br />就需要引入分组的概念了。
<a name="uBnvO"></a>
### 定义validation groups，在API模块中定义两个分组。
```java
//用于新增
public interface InsertValidation {
}

//用于更新
public interface UpdateValidation {
}
```
<a name="GfucO"></a>
### 定义 DTO
```java
@Data
public class PracticeParam implements Serializable {

    //只用于更新
    @NotNull(groups={UpdateValidation.class},message = "id不能为空")
    private Integer id;

    // 如果两组校验都需要可以省去group的定义，完整的如下
    // @NotBlank(groups={InsertValidation.class, UpdateValidation.class},message = "名称不能为空")
    @NotNull(message = "periodId不能为空")
    private Long periodId;
}
```
<a name="Kyb2V"></a>
### 服务提供者`interface`
```java
public interface IPracticeService {
    //因为periodId参数是默认不区分组的，所以这里省去了Validated注解
    boolean practiceAdd(PracticeParam practiceParam);

    boolean practiceEdit(@Validated(value = {UpdateValidation.class}) PracticeParam practiceParam);
}
```
看看真实运行情况。
<a name="XcfFN"></a>
### 生产环境验证
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638537872560-33fd760f-13a4-483c-87c5-7cc6935ca78c.webp#clientId=u6fdac9db-540c-4&from=paste&id=u0fffc66a&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50a24d4b-7f45-4a5b-8614-ecf9e2e3cac&title=)
