JavaSpringBoot
<a name="drWEV"></a>
## 一、策略模式代码实践
在介绍 SpringBoot 中如何实现策略设计模式之前，先简单的回顾一下策略模式的设计思路。<br />以编写一个简单的程序计算器，代码如下！

- **首先，定义一个**`**Operation**`**接口，用于逻辑的计算**
```java
public interface Operation {

    /**
     * 执行计算
     * @param a
     * @param b
     * @return
     */
    int execute(int a, int b);
}
```

- **接着，分别将四个if判断逻辑独立成一个模块，来单独处理**
```java
public class AddOperation implements Operation {

    @Override
    public int execute(int a, int b) {
        return a + b;
    }
}

public class SubOperation implements Operation {

    @Override
    public int execute(int a, int b) {
        return a - b;
    }
}

public class MultiOperation implements Operation {

    @Override
    public int execute(int a, int b) {
        return a * b;
    }
}

public class DivOperation implements Operation {

    @Override
    public int execute(int a, int b) {
        return a / b;
    }
}
```

- **然后，创建一个工厂类，用于处理客户端传入的参数**
```java
public class OperatorFactory {

    private static Map<String, Operation> operationMap = new HashMap<>();

    static {
        //初始化实现类
        operationMap.put("add", new AddOperation());
        operationMap.put("sub", new SubOperation());
        operationMap.put("multi", new MultiOperation());
        operationMap.put("div", new DivOperation());
        // more operators
    }

    /**
     * 获取对应的目标实现类
     * @param operator
     * @return
     */
    public static Optional<Operation> getOperation(String operator){
        return Optional.ofNullable(operationMap.get(operator));
    }

}
```

- **最后，在需要的地方引入方法即可！**
```java
public class OperatorTestMain {

    public static void main(String[] args) {
        //获取计算的目标实现类
        Operation targetOperation = OperatorFactory
                .getOperation("add")
                .orElseThrow(() -> new IllegalArgumentException("Invalid Operator"));
        int result = targetOperation.execute(1, 2);
        System.out.println("result:" +  result);
    }
}
```
以上就是一个典型的策略模式的实践思路，从代码阅读性、扩展性角度看，还是非常干净利落的。<br />那么，在SpringBoot项目中，应该如何使用呢？
<a name="svMhY"></a>
## 二、SpringBoot 实践应用
<a name="PuQDb"></a>
### 3.1、方案一

- **首先，还是定义一个**`**Command**`**接口，用于方法的抽象和统一**
```java
public interface Command {

    /**
     * 命令类型
     * @return
     */
    String operateType();

    /**
     * 执行
     * @param a
     * @param b
     * @return
     */
    Integer execute(int a, int b);

}
```

- **接着，编写四套不同的计算处理逻辑**
```java
@Component
public class AddCommand implements Command {

    @Override
    public String operateType() {
        return "add";
    }

    @Override
    public Integer execute(int a, int b) {
        return a + b;
    }
}

@Component
public class SubCommand implements Command {

    @Override
    public String operateType() {
        return "subtract";
    }

    @Override
    public Integer execute(int a, int b) {
        return a - b;
    }
}

@Component
public class MultiCommand implements Command {

    @Override
    public String operateType() {
        return "multiply";
    }

    @Override
    public Integer execute(int a, int b) {
        return a * b;
    }
}

@Component
public class DivCommand implements Command {

    @Override
    public String operateType() {
        return "divide";
    }

    @Override
    public Integer execute(int a, int b) {
        return a / b;
    }
}
```

- **然后，编写一个类似于上文的策略处理类**
```java
@Component
public class CalculatorService implements ApplicationContextAware {

    private Map<String, Command> commandMap = new ConcurrentHashMap<>();


    /**
     * 执行计算
     * @param operateType
     * @param a
     * @param b
     * @return
     */
    public int calculate(String operateType,int a, int b){
        Command targetCommand = Optional.ofNullable(commandMap.get(operateType))
                .orElseThrow(() -> new IllegalArgumentException("Invalid Operator"));
        return targetCommand.execute(a,b);
    }


    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        Map<String, Command> tempMap = applicationContext.getBeansOfType(Command.class);
        tempMap.values().forEach(source -> commandMap.put(source.operateType(), source));
    }

}
```

- **最后，只需要在适当的位置应用即可！**
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class CalculatorServiceTest {

    @Autowired
    private CalculatorService calculatorService;

    @Test
    public void test(){
        int result = calculatorService.calculate("add", 1,2);
        System.out.println("result:" +  result);
    }
}
```
总结：这种方案的实践，和上面介绍的思路基本上一致，不同的地方在于，当 springboot 启动时，会将对象注入到IOC容器。
<a name="ApgDl"></a>
### 3.2、方案二（推荐）
翻查Spring的ioc容器，会发现一个秘密，当一个接口有多个实现类时，Spring会自动将`Strategy`接口的实现类注入到这个Map中，key为bean id，value值则为对应的策略实现类。<br />简单的说，只需要通过`@Autowired`注入对象，不需要通过`CalculatorService`这个类进行单独配置，操作方式如下！

- **首先，编写一个CommandFactory工厂类，用于逻辑的处理**
```java
@Component
public class CommandFactory {

    /**
     * Spring会自动将Strategy接口的实现类注入到这个Map中，key为bean id，value值则为对应的策略实现类
     */
    @Autowired
    private Map<String, Command> commandMap;


    /**
     * 执行计算
     * @param operateType
     * @param a
     * @param b
     * @return
     */
    public int calculate(String operateType,int a, int b){
        Command targetCommand = Optional.ofNullable(commandMap.get(operateType))
                .orElseThrow(() -> new IllegalArgumentException("Invalid Operator"));
        return targetCommand.execute(a,b);
    }
}
```

- **最后，直接在合适的地方使用**`**CommandFactory**`**即可！**
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class CalculatorServiceTest {
	
	@Autowired
	private CommandFactory commandFactory;
	
	@Test
	public void test(){
		int result = commandFactory.calculate("addCommand", 1,2);
		System.out.println("result:" +  result);
	}
}
```
总结：方案二和方案一的不同点在于，不需要显式的编写`CalculatorService`策略处理类来初始化对象，Spring在初始化对象的时候，可以实现对象的注入！
