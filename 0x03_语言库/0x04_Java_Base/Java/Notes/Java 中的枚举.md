Java 枚举
<a name="Ha00h"></a>
## 1、概览
enum关键字在 java5 中引入，表示一种特殊类型的类，其总是继承java.lang.Enum类，更多内容可以自行查看其官方文档。<br />枚举在很多时候会和常量拿来对比，可能因为本身大量实际使用枚举的地方就是为了替代常量。那么这种方式由什么优势呢？<br />以这种方式定义的常量使代码更具可读性，允许进行编译时检查，预先记录可接受值的列表，并避免由于传入无效值而引起的意外行为。<br />下面示例定义一个简单的枚举类型 pizza 订单的状态，共有三种 `ORDERED`, `READY`, `DELIVERED`状态:
```java
public enum PizzaStatus {
    ORDERED,
    READY,
    DELIVERED;
}
```
简单来说，通过上面的代码避免了定义常量，将所有和 pizza 订单的状态的常量都统一放到了一个枚举类型里面。
```java
System.out.println(PizzaStatus.ORDERED.name());//ORDERED
System.out.println(PizzaStatus.ORDERED);//ORDERED
System.out.println(PizzaStatus.ORDERED.name().getClass());//class java.lang.String
System.out.println(PizzaStatus.ORDERED.getClass());//class shuang.kou.enumdemo.enumtest.PizzaStatus
```
<a name="AYivB"></a>
## 2、自定义枚举方法
现在对枚举是什么以及如何使用它们有了基本的了解，通过在枚举上定义一些额外的API方法，将上一个示例提升到一个新的水平：
```java
public class Pizza {
    private PizzaStatus status;
    publicenum PizzaStatus {
        ORDERED,
        READY,
        DELIVERED;
    }

    public boolean isDeliverable() {
        if (getStatus() == PizzaStatus.READY) {
            returntrue;
        }
        returnfalse;
    }

    // Methods that set and get the status variable.
}
```
<a name="xR1pN"></a>
## 3、使用 `==` 比较枚举类型
由于枚举类型确保JVM中仅存在一个常量实例，因此可以安全地使用“ ==”运算符比较两个变量，如上例所示；此外，“ ==”运算符可提供编译时和运行时的安全性。<br />首先看一下以下代码段中的运行时安全性，其中“ ==”运算符用于比较状态，并且如果两个值均为null 都不会引发 NullPointerException。相反，如果使用`equals`方法，将抛出 NullPointerException：
```java
if(testPz.getStatus().equals(Pizza.PizzaStatus.DELIVERED));
if(testPz.getStatus() == Pizza.PizzaStatus.DELIVERED);
```
对于编译时安全性，看另一个示例，两个不同枚举类型进行比较，使用`equal`方法比较结果确定为`true`，因为`getStatus`方法的枚举值与另一个类型枚举值一致，但逻辑上应该为`false`。这个问题可以使用`==`操作符避免。因为编译器会表示类型不兼容错误：
```java
if(testPz.getStatus().equals(TestColor.GREEN));
if(testPz.getStatus() == TestColor.GREEN);
```
<a name="Vo41a"></a>
## 4、在 switch 语句中使用枚举类型
```java
public int getDeliveryTimeInDays() {
    switch (status) {
        case ORDERED: return5;
        case READY: return2;
        case DELIVERED: return0;
    }
    return0;
}
```
<a name="ZPOUS"></a>
## 5、枚举类型的属性，方法和构造函数
可以通过在枚举类型中定义属性，方法和构造函数让它变得更加强大。<br />下面，扩展上面的示例，实现从比萨的一个阶段到另一个阶段的过渡，并了解如何摆脱之前使用的if语句和switch语句：
```java
public class Pizza {

    private PizzaStatus status;
    publicenum PizzaStatus {
        ORDERED (5){
            @Override
            public boolean isOrdered() {
                returntrue;
            }
        },
        READY (2){
            @Override
            public boolean isReady() {
                returntrue;
            }
        },
        DELIVERED (0){
            @Override
            public boolean isDelivered() {
                returntrue;
            }
        };

        privateint timeToDelivery;

        public boolean isOrdered() {returnfalse;}

        public boolean isReady() {returnfalse;}

        public boolean isDelivered(){returnfalse;}

        public int getTimeToDelivery() {
            return timeToDelivery;
        }

        PizzaStatus (int timeToDelivery) {
            this.timeToDelivery = timeToDelivery;
        }
    }

    public boolean isDeliverable() {
        returnthis.status.isReady();
    }

    public void printTimeToDeliver() {
        System.out.println("Time to delivery is " +
          this.getStatus().getTimeToDelivery());
    }

    // Methods that set and get the status variable.
}
```
下面这段代码展示它是如何 work 的：
```java
@Test
public void givenPizaOrder_whenReady_thenDeliverable() {
    Pizza testPz = new Pizza();
    testPz.setStatus(Pizza.PizzaStatus.READY);
    assertTrue(testPz.isDeliverable());
}
```
<a name="coQPi"></a>
## 6、`EnumSet` and `EnumMap`
<a name="F3sXC"></a>
### 6.1. `EnumSet`
`EnumSet` 是一种专门为枚举类型所设计的 `Set` 类型。<br />与`HashSet`相比，由于使用了内部位向量表示，因此它是特定 `Enum` 常量集的非常有效且紧凑的表示形式。<br />它提供了类型安全的替代方法，以替代传统的基于int的“位标志”，编写更易读和易于维护的简洁代码。<br />`EnumSet` 是抽象类，其有两个实现：`RegularEnumSet `、`JumboEnumSet`，选择哪一个取决于实例化时枚举中常量的数量。<br />在很多场景中的枚举常量集合操作（如：取子集、增加、删除、`containsAll`和`removeAll`批操作）使用`EnumSet`非常合适；如果需要迭代所有可能的常量则使用`Enum.values()`。
```java
public class Pizza {

    privatestatic EnumSet<PizzaStatus> undeliveredPizzaStatuses =
      EnumSet.of(PizzaStatus.ORDERED, PizzaStatus.READY);

    private PizzaStatus status;

    publicenum PizzaStatus {
        ...
    }

    public boolean isDeliverable() {
        returnthis.status.isReady();
    }

    public void printTimeToDeliver() {
        System.out.println("Time to delivery is " +
          this.getStatus().getTimeToDelivery() + " days");
    }

    public static List<Pizza> getAllUndeliveredPizzas(List<Pizza> input) {
        return input.stream().filter(
          (s) -> undeliveredPizzaStatuses.contains(s.getStatus()))
            .collect(Collectors.toList());
    }

    public void deliver() {
        if (isDeliverable()) {
            PizzaDeliverySystemConfiguration.getInstance().getDeliveryStrategy()
              .deliver(this);
            this.setStatus(PizzaStatus.DELIVERED);
        }
    }

    // Methods that set and get the status variable.
}
```
下面的测试演示了展示了 `EnumSet` 在某些场景下的强大功能：
```java
@Test
public void givenPizaOrders_whenRetrievingUnDeliveredPzs_thenCorrectlyRetrieved() {
    List<Pizza> pzList = new ArrayList<>();
    Pizza pz1 = new Pizza();
    pz1.setStatus(Pizza.PizzaStatus.DELIVERED);

    Pizza pz2 = new Pizza();
    pz2.setStatus(Pizza.PizzaStatus.ORDERED);

    Pizza pz3 = new Pizza();
    pz3.setStatus(Pizza.PizzaStatus.ORDERED);

    Pizza pz4 = new Pizza();
    pz4.setStatus(Pizza.PizzaStatus.READY);

    pzList.add(pz1);
    pzList.add(pz2);
    pzList.add(pz3);
    pzList.add(pz4);

    List<Pizza> undeliveredPzs = Pizza.getAllUndeliveredPizzas(pzList);
    assertTrue(undeliveredPzs.size() == 3);
}
```
<a name="SvJAf"></a>
### 6.2. `EnumMap`
`EnumMap`是一个专门化的映射实现，用于将枚举常量用作键。与对应的 `HashMap` 相比，它是一个高效紧凑的实现，并且在内部表示为一个数组:
```java
EnumMap<Pizza.PizzaStatus, Pizza> map;
```
快速看一个真实的示例，该示例演示如何在实践中使用它：
```java
public static EnumMap<PizzaStatus, List<Pizza>>
  groupPizzaByStatus(List<Pizza> pizzaList) {
    EnumMap<PizzaStatus, List<Pizza>> pzByStatus =
      new EnumMap<PizzaStatus, List<Pizza>>(PizzaStatus.class);

    for (Pizza pz : pizzaList) {
        PizzaStatus status = pz.getStatus();
        if (pzByStatus.containsKey(status)) {
            pzByStatus.get(status).add(pz);
        } else {
            List<Pizza> newPzList = new ArrayList<Pizza>();
            newPzList.add(pz);
            pzByStatus.put(status, newPzList);
        }
    }
    return pzByStatus;
}
```
下面的测试演示了展示了 `EnumMap` 在某些场景下的强大功能：
```java
@Test
public void givenPizaOrders_whenGroupByStatusCalled_thenCorrectlyGrouped() {
    List<Pizza> pzList = new ArrayList<>();
    Pizza pz1 = new Pizza();
    pz1.setStatus(Pizza.PizzaStatus.DELIVERED);

    Pizza pz2 = new Pizza();
    pz2.setStatus(Pizza.PizzaStatus.ORDERED);

    Pizza pz3 = new Pizza();
    pz3.setStatus(Pizza.PizzaStatus.ORDERED);

    Pizza pz4 = new Pizza();
    pz4.setStatus(Pizza.PizzaStatus.READY);

    pzList.add(pz1);
    pzList.add(pz2);
    pzList.add(pz3);
    pzList.add(pz4);

    EnumMap<Pizza.PizzaStatus,List<Pizza>> map = Pizza.groupPizzaByStatus(pzList);
    assertTrue(map.get(Pizza.PizzaStatus.DELIVERED).size() == 1);
    assertTrue(map.get(Pizza.PizzaStatus.ORDERED).size() == 2);
    assertTrue(map.get(Pizza.PizzaStatus.READY).size() == 1);
}
```
<a name="kvlFW"></a>
## 7、通过枚举实现一些设计模式
<a name="RCrtg"></a>
### 7.1 单例模式
通常，使用类实现 Singleton 模式并非易事，枚举提供了一种实现单例的简便方法。<br />《Effective Java 》和《Java与模式》都非常推荐这种方式，使用这种方式方式实现枚举可以有什么好处呢？
> 《Effective Java》
> 这种方法在功能上与公有域方法相近，但是它更加简洁，无偿提供了序列化机制，绝对防止多次实例化，即使是在面对复杂序列化或者反射攻击的时候。虽然这种方法还没有广泛采用，但是单元素的枚举类型已经成为实现 Singleton的最佳方法。—-《Effective Java 中文版 第二版》

> 《Java与模式》
> 《Java与模式》中，作者这样写道，使用枚举来实现单实例控制会更加简洁，而且无偿地提供了序列化机制，并由JVM从根本上提供保障，绝对防止多次实例化，是更简洁、高效、安全的实现单例的方式。

下面的代码段显示了如何使用枚举实现单例模式：
```java
publicenum PizzaDeliverySystemConfiguration {
    INSTANCE;
    PizzaDeliverySystemConfiguration() {
        // Initialization configuration which involves
        // overriding defaults like delivery strategy
    }

    private PizzaDeliveryStrategy deliveryStrategy = PizzaDeliveryStrategy.NORMAL;

    public static PizzaDeliverySystemConfiguration getInstance() {
        return INSTANCE;
    }

    public PizzaDeliveryStrategy getDeliveryStrategy() {
        return deliveryStrategy;
    }
}
```
如何使用呢？请看下面的代码：
```java
PizzaDeliveryStrategy deliveryStrategy = PizzaDeliverySystemConfiguration.getInstance().getDeliveryStrategy();
```
通过 `PizzaDeliverySystemConfiguration.getInstance()` 获取的就是单例的 `PizzaDeliverySystemConfiguration`
<a name="hUfb2"></a>
### 7.2 策略模式
通常，策略模式由不同类实现同一个接口来实现的。<br />这也就意味着添加新策略意味着添加新的实现类。使用枚举，可以轻松完成此任务，添加新的实现意味着只定义具有某个实现的另一个实例。<br />下面的代码段显示了如何使用枚举实现策略模式：
```java
publicenum PizzaDeliveryStrategy {
    EXPRESS {
        @Override
        public void deliver(Pizza pz) {
            System.out.println("Pizza will be delivered in express mode");
        }
    },
    NORMAL {
        @Override
        public void deliver(Pizza pz) {
            System.out.println("Pizza will be delivered in normal mode");
        }
    };

    public abstract void deliver(Pizza pz);
}
```
给 Pizza增加下面的方法：
```java
public void deliver() {
    if (isDeliverable()) {
        PizzaDeliverySystemConfiguration.getInstance().getDeliveryStrategy()
          .deliver(this);
        this.setStatus(PizzaStatus.DELIVERED);
    }
}
```
如何使用呢？请看下面的代码：
```java
@Test
public void givenPizaOrder_whenDelivered_thenPizzaGetsDeliveredAndStatusChanges() {
    Pizza pz = new Pizza();
    pz.setStatus(Pizza.PizzaStatus.READY);
    pz.deliver();
    assertTrue(pz.getStatus() == Pizza.PizzaStatus.DELIVERED);
}
```
<a name="PgikX"></a>
## 8、Java 8 与枚举
Pizza 类可以用Java 8重写，可以看到方法 lambda 和Stream API如何使 `getAllUndeliveredPizzas()`和`groupPizzaByStatus()`方法变得如此简洁：
<a name="CMzvY"></a>
### `getAllUndeliveredPizzas()`
```java
public static List<Pizza> getAllUndeliveredPizzas(List<Pizza> input) {
    return input.stream().filter(
      (s) -> !deliveredPizzaStatuses.contains(s.getStatus()))
        .collect(Collectors.toList());
}
```
<a name="kQVwl"></a>
### `groupPizzaByStatus()`
```java
publicstatic EnumMap<PizzaStatus, List<Pizza>>
  groupPizzaByStatus(List<Pizza> pzList) {
    EnumMap<PizzaStatus, List<Pizza>> map = pzList.stream().collect(
      Collectors.groupingBy(Pizza::getStatus,
      () -> new EnumMap<>(PizzaStatus.class), Collectors.toList()));
    return map;
}
```
<a name="kWtfI"></a>
## 9、Enum 类型的 JSON 表现形式
使用Jackson库，可以将枚举类型的JSON表示为POJO。下面的代码段显示了可以用于同一目的的Jackson批注：
```java
@JsonFormat(shape = JsonFormat.Shape.OBJECT)
publicenum PizzaStatus {
    ORDERED (5){
        @Override
        public boolean isOrdered() {
            returntrue;
        }
    },
    READY (2){
        @Override
        public boolean isReady() {
            returntrue;
        }
    },
    DELIVERED (0){
        @Override
        public boolean isDelivered() {
            returntrue;
        }
    };

    privateint timeToDelivery;

    public boolean isOrdered() {returnfalse;}

    public boolean isReady() {returnfalse;}

    public boolean isDelivered(){returnfalse;}

    @JsonProperty("timeToDelivery")
    public int getTimeToDelivery() {
        return timeToDelivery;
    }

    private PizzaStatus (int timeToDelivery) {
        this.timeToDelivery = timeToDelivery;
    }
}
```
可以按如下方式使用 Pizza 和 PizzaStatus：
```java
Pizza pz = new Pizza();
pz.setStatus(Pizza.PizzaStatus.READY);
System.out.println(Pizza.getJsonString(pz));
```
生成 Pizza 状态以以下JSON展示：
```json
{
  "status" : {
    "timeToDelivery" : 2,
    "ready" : true,
    "ordered" : false,
    "delivered" : false
  },
  "deliverable" : true
}
```
有关枚举类型的JSON序列化/反序列化（包括自定义）的更多信息，请参阅Jackson-将枚举序列化为JSON对象。
<a name="wzPif"></a>
## 10、总结
讨论了Java枚举类型，从基础知识到高级应用以及实际应用场景，感受到枚举的强大功能。
<a name="ietzA"></a>
## 11、补充
在上面讲到了可以通过在枚举类型中定义属性，方法和构造函数让它变得更加强大。<br />下面通过一个实际的例子展示一下，当调用短信验证码的时候可能有几种不同的用途，在下面这样定义：
```java
publicenum PinType {

    REGISTER(100000, "注册使用"),
    FORGET_PASSWORD(100001, "忘记密码使用"),
    UPDATE_PHONE_NUMBER(100002, "更新手机号码使用");

    privatefinalint code;
    privatefinal String message;

    PinType(int code, String message) {
        this.code = code;
        this.message = message;
    }

    public int getCode() {
        return code;
    }

    public String getMessage() {
        return message;
    }

    @Override
    public String toString() {
        return"PinType{" +
                "code=" + code +
                ", message='" + message + '\'' +
                '}';
    }
}
```
实际使用：
```java
System.out.println(PinType.FORGET_PASSWORD.getCode());
System.out.println(PinType.FORGET_PASSWORD.getMessage());
System.out.println(PinType.FORGET_PASSWORD.toString());
```
Output:
```java
100001
忘记密码使用
PinType{code=100001, message='忘记密码使用'}
```
这样的话，在实际使用起来就会非常灵活方便！
