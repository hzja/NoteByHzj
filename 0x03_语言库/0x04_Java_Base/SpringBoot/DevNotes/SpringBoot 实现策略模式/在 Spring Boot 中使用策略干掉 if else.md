JavaSpringBoot
<a name="bL0lZ"></a>
## 需求
这里虚拟一个业务需求，让大家容易理解。假设有一个订单系统，里面的一个功能是根据订单的不同类型作出不同的处理。<br />订单实体：
```java
import java.math.BigDecimal;

/**
 * @Description:
 * @Date: Created in 9:48 2019/2/2
 */
public class OrderDTO {

    private String code;

    private BigDecimal price;

    /**
     * 订单类型
     * 1：普通订单；
     * 2：团购订单；
     * 3：促销订单；
     */
    private String type;
}
```
service接口：
```java
public interface IOrderService {

    /**
     * 根据订单的不同类型作出不同的处理
     *
     * @param dto 订单实体
     * @return 为了简单，返回字符串
     */
    String handle(OrderDTO dto);

}
```
<a name="SJrvH"></a>
### 传统实现
根据订单类型写一堆的if else：
```java
@Service
public class OrderServiceImpl implements IOrderService {

    @Override
    public String handle(OrderDTO dto) {
        String type = dto.getType();
        if ("1".equals(type)) {
            return "处理普通订单";
        } else if ("2".equals(type)) {
            return "处理团购订单";
        } else if ("3".equals(type)) {
            return "处理促销订单";
        }
        return null;
    }

}
```
<a name="e15I3"></a>
### 策略模式实现
利用策略模式，只需要两行即可实现业务逻辑：
```java
@Service
public class OrderServiceV2Impl implements IOrderService {

    @Autowired
    private HandlerContext handlerContext;

    @Override
    public String handle(OrderDTO dto) {
        AbstractHandler handler = handlerContext.getInstance(dto.getType());
        return handler.handle(dto);
    }

}
```
可以看到上面的方法中注入了`HandlerContext`，这是一个处理器上下文，用来保存不同的业务处理器，具体在下文会讲解。从中获取一个抽象的处理器`AbstractHandler`，调用其方法实现业务逻辑。<br />现在可以了解到，主要的业务逻辑是在处理器中实现的，因此有多少个订单类型，就对应有多少个处理器。以后需求变化，增加了订单类型，只需要添加相应的处理器就可以，上述`OrderServiceV2Impl`完全不需改动。<br />先看看业务处理器的写法：
```java
@Component
@HandlerType("1")
public class NormalHandler extends AbstractHandler {

    @Override
    public String handle(OrderDTO dto) {
        return "处理普通订单";
    }

}
```
```java
@Component
@HandlerType("2")
public class GroupHandler extends AbstractHandler {

    @Override
    public String handle(OrderDTO dto) {
        return "处理团购订单";
    }

}
```
```java
@Component
@HandlerType("3")
public class PromotionHandler extends AbstractHandler {

    @Override
    public String handle(OrderDTO dto) {
        return "处理促销订单";
    }

}
```
首先每个处理器都必须添加到Spring容器中，因此需要加上`@Component`注解，其次需要加上一个自定义注解`@HandlerType`，用于标识该处理器对应哪个订单类型，最后就是继承`AbstractHandler`，实现自己的业务逻辑。<br />自定义注解 `@HandlerType`：
```java
@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
public @interface HandlerType {

    String value();

}
```
抽象处理器 `AbstractHandler`：
```java
public abstract class AbstractHandler {

    abstract public String handle(OrderDTO dto);

}
```
自定义注解和抽象处理器都很简单，那么如何将处理器注册到Spring容器中呢？<br />具体思路是：<br />1、扫描指定包中标有`@HandlerType`的类；<br />2、将注解中的类型值作为key，对应的类作为value，保存在Map中；<br />3、以上面的map作为构造函数参数，初始化`HandlerContext`，将其注册到Spring容器中；<br />将核心的功能封装在`HandlerProcessor`类中，完成上面的功能。<br />`HandlerProcessor`：
```java
@Component
@SuppressWarnings("unchecked")
public class HandlerProcessor implements BeanFactoryPostProcessor {

    private static final String HANDLER_PACKAGE = "com.cipher.handler_demo.handler.biz";

    /**
     * 扫描@HandlerType，初始化HandlerContext，将其注册到spring容器
     *
     * @param beanFactory bean工厂
     * @see HandlerType
     * @see HandlerContext
     */
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        Map<String, Class> handlerMap = Maps.newHashMapWithExpectedSize(3);
        ClassScaner.scan(HANDLER_PACKAGE, HandlerType.class).forEach(clazz -> {
            String type = clazz.getAnnotation(HandlerType.class).value();
            handlerMap.put(type, clazz);
        });
        HandlerContext context = new HandlerContext(handlerMap);
        beanFactory.registerSingleton(HandlerContext.class.getName(), context);
    }

}
```
<a name="mWHWG"></a>
## `ClassScanner`：扫描工具类源码
`HandlerProcessor`需要实现`BeanFactoryPostProcessor`，在Spring处理bean前，将自定义的bean注册到容器中。<br />核心工作已经完成，现在看看`HandlerContext`如何获取对应的处理器：<br />`HandlerContext`：
```java
@SuppressWarnings("unchecked")
public class HandlerContext {

    private Map<String, Class> handlerMap;

    public HandlerContext(Map<String, Class> handlerMap) {
        this.handlerMap = handlerMap;
    }

    public AbstractHandler getInstance(String type) {
        Class clazz = handlerMap.get(type);
        if (clazz == null) {
            throw new IllegalArgumentException("not found handler for type: " + type);
        }
        return (AbstractHandler) BeanTool.getBean(clazz);
    }

}
```
`BeanTool`：获取bean工具类<br />`#getInstance` 方法根据类型获取对应的class，然后根据class类型获取注册到Spring中的bean。<br />最后请注意一点，`HandlerProcessor`和`BeanTool`必须能被扫描到，或者通过`@Bean`的方式显式的注册，才能在项目启动时发挥作用。
<a name="GFSTk"></a>
## 总结
利用策略模式可以简化繁杂的if else代码，方便维护，而利用自定义注解和自注册的方式，可以方便应对需求的变更。本文只是提供一个大致的思路，还有很多细节可以灵活变化，例如使用枚举类型、或者静态常量，作为订单的类型，相信你能想到更多更好的方法。<br />示例代码：[https://github.com/ciphermagic/java-learn/tree/master/sandbox/src/main/java/com/cipher/handler_demo](https://github.com/ciphermagic/java-learn/tree/master/sandbox/src/main/java/com/cipher/handler_demo)
