![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1698975292939-3e9fba4b-6fa2-4684-8d1f-0afff1d5f7ff.jpeg)
<a name="kx3ee"></a>
## 方法一：提前 return
假如有如下代码：
```java
if (condition){
    doSomething;
} else {
    return;
}
```
这种代码一般采用提前 return 的方式，去掉不必要的 else。
```java
if (!condition){
    return
}

doSomething;
```
这种方法一般只适合分支结构很简单的 if...else，可以提前 return ，把一些不必要的 if...else 去掉。
<a name="r9AJv"></a>
## 方法二：枚举
枚举其实也是可以去掉 if...else 的，如下：
```java
String orderStatusDes;
if ("1".equals(orderStatus)) {
    orderStatusDes = "订单未支付";
} else if ("2".equals(orderStatus)) {
    orderStatusDes = "订单已支付";
} else if ("3".equals(orderStatus)) {
    orderStatusDes = "订单已发货";
} else if ("4".equals(orderStatus)) {
    orderStatusDes = "订单已签收";
} else if ("5".equals(orderStatus)) {
    orderStatusDes = "订单已评价";
}
```
这种类型的代码非常适合枚举来解决。<br />先定义一个枚举类：
```java
@Getter
@AllArgsConstructor
public enum OrderStatusEnum {
    UN_PAID("1","订单未支付"),
    PAIDED("2","订单已支付"),
    SENDED("3","订单已发货"),
    SINGED("4","订单已签收"),
    EVALUATED("5","订单已评价");

    private String status;

    private String statusDes;

    static OrderStatusEnum of(String status) {
        for (OrderStatusEnum statusEnum : OrderStatusEnum.values()) {
            if (statusEnum.getStatus().equals(status)) {
                return statusEnum;
            }
        }
        return null;
    }
}
```
有了这个枚举，上面代码直接可以优化为一行代码：
```java
String orderStatusDes = OrderStatusEnum.of(orderStatus).getStatusDes();
```
当然一般在实际项目中，这种处理方式也不是最佳的，最佳的方式应该是在数据库里面有一个码值配置表，然后加载到系统缓存中来，在通过 code 去取值。当然枚举也是一种很好的解决方案。
<a name="J0fKO"></a>
## 方案三：Optional 判空
相信各位小伙伴的项目里面一定存在非空判断，如果为空，则抛出异常或者 return。
```java
Order order = getOrderById(id);
if (order == null) {
    return "-1";
} else {
    return order.getOrderStatus();
}
```
对于这种代码利用 Optional 可以非常优雅地解决。
```java
return Optional.ofNullable(order).map(o -> o.getOrderStatus()).orElse("-1");
```
这种方式是不是非常优雅，有格调。最后补充一句：**防止 NPE，是程序员的基本修养**
<a name="PwBl9"></a>
## 方案四：表驱动法
表驱动法，是一种可以在表中查找信息，而不必用过多的 if...else 来把他们找出来的方法。如下：
```java
if ("code1".equals(action)) {
    doAction1();
} else if ("code2".equals(action)) {
    doAction2();
} else if ("code3".equals(action)) {
    doAction3();
} else if ("code4".equals(action)) {
    doAction4();
} else if ("code5".equals(action)) {
    doAction5();
}
```
优化方法如下：
```java
Map<String, Function<?> action> actionMap = new HashMap<>();
action.put("code1",() -> {doAction1()});
action.put("code2",() -> {doAction2()});
action.put("code3",() -> {doAction3()});
action.put("code4",() -> {doAction4()});
action.put("code5",() -> {doAction5()});

// 使用
actionMap.get(action).apply();
```
其实这种方式也不是很好，因为它会显得代码非常臃肿。一种变形方案是将 `doAction()` 抽象成类。如下：
```java
//1. 先定义一个 ActionService 接口
public interface ActionService {
    void doAction();
}

//2. 然后定义 5 个实现类
public class ActionService1 implements ActionService{
    public void doAction() {
        //do something
    }
}

//3. 加入表中
Map<String, ActionService> actionMap = new HashMap<>();
action.put("code1",new ActionService1());
action.put("code2",new ActionService2());
action.put("code3",new ActionService3());
action.put("code4",new ActionService4());
action.put("code5",new ActionService5());

//4. 调用
actionMap.get(action).doAction();
```
这种方式是不是比较优雅些！
<a name="LrSdA"></a>
## 方案五：策略模式 + 工厂方法
策略模式 + 工厂方法是解决 if...else 用得非常多的方案，它和上面的表驱动法有点儿类似。使用策略模式 + 工厂方法分为几个步骤，以上面例子为例：
<a name="wYB0I"></a>
### 把条件模块抽象为一个公共的接口，策略接口
```java
public interface ActionService {
    void doAction();
}
```
<a name="oNfuq"></a>
### 根据每个逻辑，定义出自己具体的策略实现类
如下：
```java
public class ActionService1 implements ActionService{
    public void doAction() {
        //do something
    }
}

public class ActionService2 implements ActionService{
    public void doAction() {
        //do something
    }
}

// 省略其他策略
```
<a name="PE9fj"></a>
### 工厂类，统一调度，用来管理这些策略
如下：
```java
public class ActionServiceFactory {
    private ActionServiceFactory(){

    }

    private static class SingletonHolder{
        private static ActionServiceFactory instance=new ActionServiceFactory();
    }

    public static ActionServiceFactory getInstance(){
        return SingletonHolder.instance;
    }

    private static final Map<String,ActionService> ACTION_SERVICE_MAP = new HashMap<String, ActionService>();

    static {
        ACTION_SERVICE_MAP.put("action1",new ActionService1());
        ACTION_SERVICE_MAP.put("action2",new ActionService2());
        ACTION_SERVICE_MAP.put("action3",new ActionService3());
        ACTION_SERVICE_MAP.put("action4",new ActionService4());
        ACTION_SERVICE_MAP.put("action5",new ActionService5());
    }

    public static ActionService getActionService(String actionCode) {
        ActionService actionService = ACTION_SERVICE_MAP.get(actionCode);
        if (actionService == null) {
            throw new RuntimeException("非法 actionCode");
        }
        return actionService;
    }

    public void doAction(String actionCode) {
        getActionService(actionCode).doAction();
    }
}
```
单例模式实现工厂类。
<a name="Z5UzB"></a>
### 使用
```java
ActionServiceFactory.getInstance().doAction("action1");
```
这种优化方式也是很优雅的，特别适合分支较多，逻辑较为复杂的代码块，这种方式将分支逻辑与业务代码解耦了，是一种很不错的方案。
<a name="SMweq"></a>
## 方案六：责任链模式
想不到责任链模式也能优化 if...else 吧。责任链可以看做是一个单链表的数据结构，一个对象一个对象地过滤条件，符合的就执行，然后结束，不符合的就传递到下一个节点，如果每个对象都无法处理，一般都有一个最终的节点来统一处理。<br />依然以上面那个例子为例。
<a name="uYJg6"></a>
### 定义责任链处理请求节点
```java
public abstract class ActionHandler {

    // 后继节点
    protected ActionHandler successor;

    /**
     * 处理请求
     * @param actionCode
     */
    public void handler(String actionCode) {
        doHandler(actionCode);
    }

    // 设置后继节点
    protected ActionHandler setSuccessor(ActionHandler successor) {
        this.successor = successor;
        return this;
    }

    // 处理请求
    public abstract void doHandler(String actionCode);
}
```
<a name="vcbL9"></a>
### 定义首尾节点，用于一些异常情况的处理
```java
// 首节点，判断 actionCode 是否为空
public class HeadHandler extends ActionHandler{

    @Override
    public void doHandler(String actionCode) {
        if (StringUtils.isBlank(actionCode)) {
            throw new RuntimeException("actionCode 不能为空");
        }

        successor.doHandler(actionCode);
    }
}

// 尾节点，直接抛出异常，因为到了尾节点说明当前 code 没有处理
public class TailHandler extends ActionHandler{

    @Override
    public void doHandler(String actionCode) {
        throw new RuntimeException("当前 code[" + actionCode + "] 没有具体的 Handler 处理");
    }
}
```
<a name="oLI9P"></a>
### 定义各个节点具体的实现节点
```java
public class ActionHandler1 extends ActionHandler{

    @Override
    public void doHandler(String actionCode) {
        if ("action1".equals(actionCode)) {
            doAction1();
        } else {
            // 传递到下一个节点
            successor.doHandler(actionCode);
        }
    }
}

public class ActionHandler2 extends ActionHandler{

    @Override
    public void doHandler(String actionCode) {
        if ("action2".equals(actionCode)) {
            doAction2();
        } else {
            // 传递到下一个节点
            successor.doHandler(actionCode);
        }
    }
}

// 省略其他节点
```
<a name="FMHN2"></a>
### 定义工厂，来构建一条完整的责任链，并负责调度
```java
public class ActionHandlerFactory {

    private ActionHandler headHandler;

    private ActionHandlerFactory(){
        headHandler = new HeadHandler();
        ActionHandler actionHandler1 = new ActionHandler1();
        ActionHandler actionHandler2 = new ActionHandler2();
        ActionHandler actionHandler3 = new ActionHandler3();
        ActionHandler actionHandler4 = new ActionHandler4();
        ActionHandler actionHandler5 = new ActionHandler5();

        ActionHandler tailHandler = new TailHandler();

        // 构建一条完整的责任链
        headHandler.setSuccessor(actionHandler1).setSuccessor(actionHandler2).setSuccessor(actionHandler3).
        setSuccessor(actionHandler4).setSuccessor(actionHandler5).setSuccessor(tailHandler);
    }

    private static class SingletonHolder{
        private static ActionHandlerFactory instance=new ActionHandlerFactory();
    }

    public static ActionHandlerFactory getInstance(){
        return SingletonHolder.instance;
    }

    public void doAction(String actionCode) {
        headHandler.doHandler(actionCode);
    }
}
```
<a name="Kp7jU"></a>
### 使用
```java
ActionHandlerFactory.getInstance().doAction("action1");
```
<a name="tDyGC"></a>
## 方案七：Function
Function 是 Java 8 中的函数式接口，利用好它可以极大地简化代码，例如利用它可以轻松去掉if...else。比如有下面一段代码：
```java
// 抛出异常
if (...) {
    throw new RuntimeException("有异常...")
}

// if...else 分支
if(...) {
    doSomething1();
} else {
    doSomething2();
}
```
现在利用 Function 来处理上面两段代码
<a name="hjeXc"></a>
### 处理抛出异常
<a name="Ka4d9"></a>
#### 定义抛出异常的形式的函数式接口
```java
@FunctionalInterface
public interface ThrowExceptionFunction {

    /**
     * 抛出异常
     * @param message
     */
    void throwMessage(String message);
}
```
这里只需要有一个这样的函数式接口就行，而且方法也没有返回值，是一个消费型接口。
<a name="v68Wp"></a>
#### 增加判断工具类
```java
public class ValidateUtils {

    /**
     * 抛出异常
     * @param flag
     * @return
     */
    public static ThrowExceptionFunction isTrue(Boolean flag) {
        return (errorMessage) -> {
            if (flag) {
                throw new RuntimeException(errorMessage);
            }
        };
    }
}
```
`ValidateUtils` 类也是非常简单的，如果传入的 flag 为 true，则抛出异常。`isTrue()` 返回值也是刚刚定义的 ThrowExceptionFunction。
<a name="qVILO"></a>
#### 使用
```java
ValidateUtils.isTrue(flag).throwMessage("有异常...");
```
使用方式是不是非常简单？
<a name="eofvn"></a>
### 处理 if...else 分支
其实使用 Function 来去掉 if...else 分支有点儿偏门，因为它非常依赖定义的 Function 函数，比如定义的方法只有两个参数，那它就只能处理处理两个分支的，对于三个分支的 if...else 则需要重新定义方法。下面以两个分支为例。
<a name="MvM8k"></a>
#### 定义函数式接口
```java
@FunctionalInterface
public interface ActionHandler {
    void doActionHandler(ActionService trueActionService,ActionService falseActionService);
}
```
函数式接口中定义了一个方法，`doActionHandler()`，它有两个参数，分别为：

1. `trueActionService`：为 true 时要进行的操作
2. `falseActionService`：为 false 时要进行的操作
<a name="uxFGY"></a>
#### 定义判断方法
增加一个工具类，用来判断为 true 时执行哪个方法，为 false 时执行哪个方法。
```java
public class ActionHandlerUtils {

    public static ActionHandler isTrue(Boolean flag) {
        return (trueActionService,falseActionService) -> {
            if (flag) {
                trueActionService.doAction();
            } else {
                trueActionService.doAction();
            }
        };
    }
}
```
<a name="tYsyS"></a>
#### 使用
```java
ActionHandlerUtils.isTrue(true)
        .doActionHandler(() -> {
            //do true Something
        },() ->{
            //do false Something
    });
```
<a name="pApXk"></a>
## 总结
这里总结了 7 中方式用来解决 if...else 的问题，相信里面总有一两种方案是适合你的，七种方案各有优劣，各自有各自的使用场景，需要在实践中不断领悟，在重构中不断进化，总结出适合自己最佳的重构方案。
