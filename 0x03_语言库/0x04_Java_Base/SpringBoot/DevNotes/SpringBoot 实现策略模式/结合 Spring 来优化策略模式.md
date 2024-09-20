JavaSpring<br />**策略模式（Strategy Pattern）：**定义不同的策略算法，以达到新增算法、移除算法、修改算法的便利性和调用无感知，并且不同的算法区分开之后也更加方便阅读策略算法。
<a name="LMn6Z"></a>
## 一、在什么场景下使用策略模式
<a name="qQ6dE"></a>
### 1、业务场景
在项目开发中有业务需求要对不同类型的加工单做不同的复杂业务校验。
<a name="NrjUE"></a>
### 2、为什么要使用策略模式
该加工单大概率不会有新增，但是可能后期会有状态的同步以及加工单数据同步，但是目前只需要对业务进行校验。<br />最开始的方案为通过判断来执行不同的业务处理，但是复杂的业务校验导致Service类急剧膨胀，三个不同类型的加工单业务校验达到四百行代码，此时在实际开发时候自己都难以快速定位到方法。<br />考虑该Service类还有其他业务处理和后期可能的变化，以及代码的可阅读性，所以使用策略类来进行封装。
<a name="pg5aS"></a>
## 二、Java中使用策略模式
<a name="QgNgA"></a>
## 三、原始策略模式的缺点

- 如果是单一的调用路线，依旧需要判断来执行不同的算法
- 当新增策略类时依旧需要修改原有代码
- 每次的调用都需要创建对象，不断创建对象导致内存的占用与代码执行时间的增加。

对于缺点的三条，观点是即使影响很小很小，但是对于一个系统来说没有一处代码优化是能对系统提升有很大帮助，只有所有的细节加起来才能让一个系统变的更好，所以在编写代码会时刻关注这些细节
<a name="q3g8d"></a>
## 四、如何结合Spring来优化策略模式
<a name="wHJyP"></a>
### 1、思路

- 原始的策略类需要通过判断来执行具体的策略，通过一个Map来解决该问题，通过不同的类型来获取不同的策略类。
- 如何做到只新增策略类就可以不需要修改代码呢？通过自定义注解配合Spring来解决。
- 如何避免多次创建对象呢，很显然可以利用Spring来管理所有的策略类，并动态添加到Map中。
<a name="qhQts"></a>
### 2、实现
自定义注解
```java
import java.lang.annotation.*;

/**
 * 业务策略注解
 */
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE})
@Documented
@Inherited
public @interface BusinessStrategy {
}
```
<a name="CuuOS"></a>
#### 自定义枚举
```java
public enum StrategyEnum {

    /**
     * 策略A枚举
     */
    STRATEGY_A(1, "策略A"),

    /**
     * 策略B枚举
     */
    STRATEGY_B(2, "策略B"),

    /**
     * 策略C枚举
     */
    STRATEGY_C(3, "策略C");

    private int strategyType;

    private String typeDescription;

    StrategyEnum(int strategyType, String typeDescription) {
        this.strategyType = strategyType;
        this.typeDescription = typeDescription;
    }

    public int getStrategyType() {
        return strategyType;
    }

    public String getTypeDescription() {
        return typeDescription;
    }
}
```
<a name="mKUFQ"></a>
#### BizResult（业务处理结果返回实体）
```java
/**
 * 业务处理返回对象
 */
public class BizResult<T> {

    /**
     * 是否执行成功标记，默认为成功
     */
    private boolean isSuccess = true;

    /**
     * 执行后返回结果
     */
    private T result;

    public BizResult(){

    }

    public BizResult(boolean isSuccess){

        this.isSuccess = isSuccess;
    }

    public BizResult(T result){

        this.result = result;
    }

    public BizResult(boolean isSuccess, T result){

        this.isSuccess = isSuccess;
        this.result = result;
    }

    public boolean isSuccess() {
        return isSuccess;
    }

    public void setSuccess(boolean success) {
        isSuccess = success;
    }

    public T getResult() {
        return result;
    }

    public void setResult(T result) {
        this.result = result;
    }

    @Override
    public String toString() {
        return "BizResult{" +
                "isSuccess=" + isSuccess +
                ", result=" + result +
                '}';
    }
}
```
<a name="faCcd"></a>
#### Strategy（策略接口）
```java
import com.dakai.strategypattern.annotation.BusinessStrategy;
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.domain.BizResult;
import com.dakai.strategypattern.strategy.domain.StrategyParam;

public interface Strategy {


    /**
     * 调用策略方法
     * @param strategyParam 执行策略方案参数
     * @return 业务处理返回结果封装实体
     */
    BizResult invokeStrategy(StrategyParam strategyParam);

    /**
     * 获取当前对象对应枚举
     * @return 策略枚举
     */
    StrategyEnum getStrategyEnum();
}
```
<a name="G6rC9"></a>
#### 策略实现类

- StrategyA
```java
import com.dakai.strategypattern.annotation.BusinessStrategy;
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.Strategy;
import com.dakai.strategypattern.strategy.domain.BizResult;
import com.dakai.strategypattern.strategy.domain.StrategyParam;
import org.springframework.stereotype.Component;

@Component
@BusinessStrategy
public class StrategyA implements Strategy {
    @Override
    public BizResult invokeStrategy(StrategyParam strategyParam) {

        System.out.println("正在执行StrategyA......");
        return null;
    }

    @Override
    public StrategyEnum getStrategyEnum() {
        return StrategyEnum.STRATEGY_A;
    }
}
```

- StrategyB
```java
import com.dakai.strategypattern.annotation.BusinessStrategy;
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.Strategy;
import com.dakai.strategypattern.strategy.domain.BizResult;
import com.dakai.strategypattern.strategy.domain.StrategyParam;
import org.springframework.stereotype.Component;

@Component
@BusinessStrategy
public class StrategyB implements Strategy {

    @Override
    public BizResult invokeStrategy(StrategyParam strategyParam) {

        System.out.println("正在执行StrategyB......");
        return null;
    }

    @Override
    public StrategyEnum getStrategyEnum() {
        return StrategyEnum.STRATEGY_B;
    }
}
```

- StrategyC
```java
import com.dakai.strategypattern.annotation.BusinessStrategy;
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.Strategy;
import com.dakai.strategypattern.strategy.domain.BizResult;
import com.dakai.strategypattern.strategy.domain.StrategyParam;
import org.springframework.stereotype.Component;

@Component
@BusinessStrategy
public class StrategyC implements Strategy {

    @Override
    public BizResult invokeStrategy(StrategyParam strategyParam) {

        System.out.println("正在执行StrategyC......");
        return null;
    }

    @Override
    public StrategyEnum getStrategyEnum() {
        return StrategyEnum.STRATEGY_C;
    }
}
```
<a name="AADJ9"></a>
#### StrategyContext（策略模式上下文）
```java
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.domain.BizResult;
import com.dakai.strategypattern.strategy.domain.StrategyParam;
import org.springframework.stereotype.Component;

import java.util.HashMap;
import java.util.Map;

@Component
public class StrategyContext {

    public Map<StrategyEnum, Strategy> strategyMap = new HashMap<>(3);

    /**
     * 执行策略上下文
     * @param strategyParam 策略参数
     * @return 业务处理返回封装实体
     */
    public BizResult invokeContext(StrategyParam strategyParam){

        if (strategyParam == null){

            throw new IllegalArgumentException("执行策略参数不能为空！");
        }

        // 获取策略参数中枚举实例
        StrategyEnum strategyEnum = strategyParam.getStrategyEnum();

        // 通过枚举实例Get对应策略类
        Strategy strategy = strategyMap.get(strategyEnum);

        // 执行策略方案
        return strategy.invokeStrategy(strategyParam);
    }
}
```
StrategyManage（策略管理类）重点
```java
import com.dakai.strategypattern.annotation.BusinessStrategy;
import com.dakai.strategypattern.strategy.Strategy;
import com.dakai.strategypattern.strategy.StrategyContext;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.stereotype.Component;

/**
 * 策略类管理器
 */
@Component
public class StrategyManage implements BeanPostProcessor {

    @Autowired
    private StrategyContext strategyContext;

    @Override
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
      
       // 一定要将Bean实例返回
        return bean;
    }

    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {

        // 判断Bean是否被自定义注解标示
        if (bean instanceof Strategy && bean.getClass().isAnnotationPresent(BusinessStrategy.class)){

            // 将策略类强转为顶层策略接口
            Strategy strategy = (Strategy) bean;

            // 将策略类添加到策略Map中
            strategyContext.strategyMap.put(strategy.getStrategyEnum(), strategy);
        }
      
       // 一定要把Bean实例返回
        return bean;
    }
}
```
<a name="BRmrr"></a>
#### StrategyTest（测试类）
```java
import com.dakai.StrategyPatternApplication;
import com.dakai.strategypattern.enums.StrategyEnum;
import com.dakai.strategypattern.strategy.Strategy;
import com.dakai.strategypattern.strategy.StrategyContext;
import com.dakai.strategypattern.strategy.domain.StrategyParam;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit.jupiter.web.SpringJUnitWebConfig;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.context.web.WebAppConfiguration;

@RunWith(SpringRunner.class)
@SpringBootTest(classes = StrategyPatternApplication.class)
@WebAppConfiguration
public class StrategyTest {

    @Autowired
    private StrategyContext strategyContext;

    @Test
    public void test(){

        StrategyParam strategyParamOne = new StrategyParam();
        strategyParamOne.setStrategyEnum(StrategyEnum.STRATEGY_A);
        strategyContext.invokeContext(strategyParamOne);

        StrategyParam strategyParamTwo = new StrategyParam();
        strategyParamTwo.setStrategyEnum(StrategyEnum.STRATEGY_B);
        strategyContext.invokeContext(strategyParamTwo);

        StrategyParam strategyParamThree = new StrategyParam();
        strategyParamThree.setStrategyEnum(StrategyEnum.STRATEGY_C);
        strategyContext.invokeContext(strategyParamThree);
    }
}
```
执行结果
```java
正在执行StrategyA......
正在执行StrategyB......
正在执行StrategyC......
```
<a name="pqzML"></a>
### 3、如何新增策略
定义策略类实现策略接口，在枚举中新增对应枚举值就可以了，不需要其他修改。
<a name="fPTkb"></a>
## 五、总结
好处

- 更小的内存占用
- 更便捷添加策略类的方式
- 更灵活的执行策略，去除了if判断
