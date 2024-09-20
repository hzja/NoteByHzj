Java Spring<br />在实际工作中，重处理是一个非常常见的场景，比如:

- 发送消息失败。
- 调用远程服务失败。
- 争抢锁失败。

这些错误可能是因为网络波动造成的，等待过后重处理就能成功。通常来说，会用try/catch，while循环之类的语法来进行重处理，但是这样的做法缺乏统一性，并且不是很方便，要多写很多代码。然而spring-retry却可以通过注解，在不入侵原有业务逻辑代码的方式下，优雅的实现重处理功能。
<a name="AUI34"></a>
## `@Retryable`是什么？
Spring系列的spring-retry是另一个实用程序模块，可以以标准方式处理任何特定操作的重试。在spring-retry中，所有配置都是基于简单注释的。
<a name="trhA2"></a>
## 使用步骤
<a name="y4byT"></a>
### 1、POM依赖
```xml
<dependency>
	<groupId>org.springframework.retry</groupId>
	<artifactId>spring-retry</artifactId>
 </dependency>
```
<a name="iPdHP"></a>
### 2、启用`@Retryable`
```java
@EnableRetry
@SpringBootApplication
public class HelloApplication {

    public static void main(String[] args) {
        SpringApplication.run(HelloApplication.class, args);
    }

}
```
<a name="p14QY"></a>
### 3、在方法上添加`@Retryable`
```java
import com.mail.elegant.service.TestRetryService;
import org.springframework.retry.annotation.Backoff;
import org.springframework.retry.annotation.Retryable;
import org.springframework.stereotype.Service;
import java.time.LocalTime;
 
@Service
public class TestRetryServiceImpl implements TestRetryService {
 
    @Override
    @Retryable(value = Exception.class,maxAttempts = 3,backoff = @Backoff(delay = 2000,multiplier = 1.5))
    public int test(int code) throws Exception{
        System.out.println("test被调用,时间："+LocalTime.now());
          if (code==0){
              throw new Exception("情况不对头！");
          }
        System.out.println("test被调用,情况对头了！");
 
        return 200;
    }
}
```
来简单解释一下注解中几个参数的含义：

- `value`：抛出指定异常才会重试
- `include`：和value一样，默认为空，当exclude也为空时，默认所有异常
- `exclude`：指定不处理的异常
- `maxAttempts`：最大重试次数，默认3次
- `backoff`：重试等待策略，默认使用`@Backoff`，`@Backoff`的value默认为1000L，设置为2000L；`multiplier`（指定延迟倍数）默认为0，表示固定暂停1秒后进行重试，如果把`multiplier`设置为1.5，则第一次重试为2秒，第二次为3秒，第三次为4.5秒。

**当重试耗尽时还是失败，会出现什么情况呢？**<br />当重试耗尽时，`RetryOperations`可以将控制传递给另一个回调，即`RecoveryCallback`。Spring-Retry还提供了`@Recover`注解，用于`@Retryable`重试失败后处理方法。如果不需要回调方法，可以直接不写回调方法，那么实现的效果是，重试次数完了后，如果还是没成功没符合业务判断，就抛出异常。
<a name="M19X2"></a>
### 4、`@Recover`
```java
@Recover
public int recover(Exception e, int code){
	System.out.println("回调方法执行！！！！");
	//记日志到数据库 或者调用其余的方法
	return 400;
}
```
可以看到传参里面写的是 `Exception e`，这个是作为回调的接头暗号（重试次数用完了，还是失败，抛出这个`Exception e`通知触发这个回调方法）。对于`@Recover`注解的方法，需要特别注意的是：

- 方法的返回值必须与`@Retryable`方法一致
- 方法的第一个参数，必须是Throwable类型的，建议是与`@Retryable`配置的异常一致，其他的参数，需要哪个参数，写进去就可以了（`@Recover`方法中有的）
- 该回调方法与重试方法写在同一个实现类里面
<a name="LZhGo"></a>
### 5. 注意事项

- 由于是基于AOP实现，所以不支持类里自调用方法
- 如果重试失败需要给`@Recover`注解的方法做后续处理，那这个重试的方法不能有返回值，只能是void
- 方法内不能使用try catch，只能往外抛异常
- `@Recover`注解来开启重试失败后调用的方法(注意,需跟重处理方法在同一个类中)，此注解注释的方法参数一定要是`@Retryable`抛出的异常，否则无法识别，可以在该方法中进行日志处理。
