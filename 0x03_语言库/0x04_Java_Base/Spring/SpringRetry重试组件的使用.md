Java Spring SpringRetry
<a name="PoEss"></a>
## 1、使用背景
在实际项目中其中一部分逻辑可能会因为调用了外部服务或者等待锁等情况下出现不可预料的异常，在这个时候可能需要对调用这部分逻辑进行重试，代码里面主要就是使用for循环写一大坨重试的逻辑，各种硬编码，各种补丁。<br />特别是针对重试的逻辑，到处都有。所以用一个重试组件spring-retry优化一波。解决掉这部分丑陋的代码！<br />这个组件的源码地址如下：[https://github.com/spring-projects/spring-retry](https://github.com/spring-projects/spring-retry)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648646789263-0d5714a5-76b4-45d3-9a69-7ecb9f58d72e.png#clientId=u362b5b93-6fb6-4&from=paste&id=u46a3f31b&originHeight=83&originWidth=464&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ff54ae1-45d7-4006-bf50-e3204c749c6&title=)
<a name="BhlXP"></a>
## 2、开始上代码
<a name="QSZKf"></a>
### 首先引入依赖：
```xml
<dependency>
    <groupId>org.springframework.retry</groupId>
    <artifactId>spring-retry</artifactId>
    <version>1.3.2</version>
</dependency>
```
由于该组件是依赖于 AOP ，所以还需要引入这个依赖（如果其他 jar 包中引用过了，也就不需要再次引用了）：
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-aop</artifactId>
	<version>2.6.1</version>
</dependency>
```
<a name="rdusk"></a>
### 开启重试：
```java
@SpringBootApplication
@EnableRetry
public class ApplicationStarter {
	public static void main(String[] args) {
		SpringApplication.run(ApplicationStarter.class);
	}
}
```
<a name="DuEXb"></a>
### Controller层
```java
@RestController
public class TestController {
@Autowired
private IRecursiveCallService recursiveCallService;

    @GetMapping("test2")
    public Object test2() {
        return recursiveCallService.testService();
    }
}
```
<a name="dpXDb"></a>
### Service层
```java
public interface IRecursiveCallService {

    /**
     * 测试service
     *
     * @return
     */
    List<Integer> testService();
}
```
<a name="rsjvJ"></a>
### Service层具体实现
```java
@Service
public class RecursiveCallServiceImpl implements IRecursiveCallService {
    
    @Override
    @Retryable(recover = "testService3")
    public List<Integer> testService() {
        System.out.println("到此一游！");
        System.out.println(1 / 0);
        return null;
    }
    
    @Recover
    public List<String> testService1() {
        System.out.println("错误的返回");
        return Collections.singletonList("S");
    }
    
    @Recover
    public List<Integer> testService2(String i) {
        System.out.println("正确的返回");
        return Collections.singletonList(1);
    }
    
    @Recover
    public List<Integer> testService3() {
        System.out.println("正确的返回2");
        return Collections.singletonList(2);
    }
}
```
<a name="MAMb1"></a>
## 3、`@Retryable`注解重要属性解析

- `recover`: 此类中用于恢复的方法的名称。方法必须用 `{@link Recover}` 注释标记。
- `value`: 可重试的异常类型。包括（）的同义词。默认为空（如果 excludes 也为空，则重试所有异常）。
- `exclude`: 不可重试的异常类型。默认为空（如果包含也为空，则重试所有异常）。如果 include 为空但 excludes 不是，则重试所有未排除的异常
- `maxAttempts`: 方法重试调用次数，默认3次
- `backoff`: 指定用于重试此操作的其他属性
<a name="jQAgd"></a>
## 4、`@backoff`注解

- `value`：重试之间间隔时间
- `delay`：重试之间的等待时间(以毫秒为单位)
- `maxDelay`：重试之间的最大等待时间(以毫秒为单位)
- `multiplier`：指定延迟的倍数
- `delayExpression`：重试之间的等待时间表达式
- `maxDelayExpression`：重试之间的最大等待时间表达式
- `multiplierExpression`：指定延迟的倍数表达式
- `random`：随机指定延迟时间
<a name="NycYN"></a>
## 5、`@Recover`注解
主要作用是标记方法为一个重试方法的补偿方法！！！
<a name="e5puJ"></a>
## 6、注意事项

- 方法重试依赖于 Spring 注入，所以调用的方法的类必须是被Spring管理的，然后通过 `@Autowired` 或 `@Resource` 引入使用，不然不会生效
- 方法重试的前提是方法抛出了异常，在方法执行出现了异常且没有被捕获的情况下重试
- 方法重试需要在方法上面加上注解 `@Retryable`
- 方法重试的补偿方法上面必须携带`@Recover`注解
- `@Recover`方法需要和`@Retryable`方法在同一个类中才能生效`@Recover`方法(`@Recover`方法在父类中也可以生效)
- 使用`@Retryable`注解，如果类中没有被`@Recover`标示的方法，无论是否使用 recover 属性都抛出原有异常
- 使用`@Retryable`注解同时 recover 属性不是空，如果类中有`@Recover`标示的方法，但是标示的方法不是 recover 指定的方法，抛出`ExhaustedRetryException`异常
- 使用`@Retryable`注解同时 recover 属性不是空，同时方法有注解`@Recover`，但是补偿方法的参数不是当前异常或者异常的父类，抛出`ExhaustedRetryException `异常
- 使用`@Retryable`注解不使用 recover 属性，如果类中被`@Recover`标示的方法有和原方法返回值一样的，使用当前被`@Recover`标示的方法（此时方法参数可随意，但是不能是除开当前异常的类及父类的异常类）
