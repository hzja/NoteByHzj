Java

- 定义
- 构造器
- 枚举类成员
- `values()`、`valueOf(String name)`方法
- 枚举的用法
- 总结

---

不知道从什么时候开始，Java 程序员总想着消除 if else。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688191201245-208a97fd-6701-44b4-857c-226675b79d9b.png#averageHue=%23f8f8f8&clientId=ud1dd16e4-6ada-4&from=paste&id=fxmH1&originHeight=461&originWidth=701&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc715f6bd-e85d-4190-a224-e00f393d250&title=)<br />基于此，来重新学习一下 Java 中的枚举类型。用它来消除部分 if else 吧。
<a name="elbvP"></a>
## **定义**
枚举是什么意思呐？百度百科的说法是这样的：
> 在数学和计算机科学理论中，一个集的**「枚举」** 是列出某些有穷序列集的所有成员的程序，或者是一种特定类型对象的计数。这两种类型经常（但不总是）重叠。是一个被命名的整型常数的集合。

枚举在日常生活中很常见，例如表示星期的 SUNDAY、MONDAY、TUESDAY、WEDNESDAY、THURSDAY、FRIDAY、SATURDAY 就是一个枚举。<br />由此映射到 Java 语言中，即可定义一个表示星期的枚举类：
```java
public enum Week {
    SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
}
```
定义枚举类的关键字是 enum，枚举类对象不能通过 new 出来，里面的 SUNDAY、MONDAY...这些其实就相当于是枚举类 Week 的实例。固定的就这几个，不能在外部创建新的实例。引用的时候直接`类.实例名`
```java
Week w = Week.MONDAY;
```
<a name="LSjHh"></a>
## **构造器**
枚举类也有构造器，默认是 `private` 修饰的，并且也只能是 `private`。观察这段代码：
```java
public enum Week {
    SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY;

    Week(){
        System.out.println("hello");
    }

}

public class Test {
    public static void main(String[] args) {
        Week w = Week.FRIDAY;
    }
}
```
会发现这样的结果：
> hello hello hello hello hello hello hello

构造函数共执行了 7 次，正好对应类中枚举项的数量。其实此类的枚举项的创建，就相当于其他类调用无参构造器 new 出来的对象，也就是这个枚举类创建了7次实例，所以输出了7个 hello。<br />除了无参构造器，枚举类也有有参构造器。
```java
public enum Week {
    SUNDAY(7), MONDAY(1), TUESDAY(2), WEDNESDAY(3), THURSDAY(4), FRIDAY(5), SATURDAY(6);

    Week(int weekNum){
        System.out.println(weekNum);
    }
}
```
这次将会输出：
> 7 1 2 3 4 5 6

<a name="gVI2g"></a>
## **枚举类成员**
枚举类和正常类一样，也可以有成员变量、实例方法、静态方法等。
```java
public enum Week {
    SUNDAY(7), MONDAY(1), TUESDAY(2), WEDNESDAY(3), THURSDAY(4), FRIDAY(5), SATURDAY(6);

    private int weekNum;

    Week(int weekNum){
        this.weekNum = weekNum;
    }

    public int getWeekNum() {
        return weekNum;
    }

}
```
使用：
```java
public class Test {
    public static void main(String[] args) {
        Week w = Week.FRIDAY;
        System.out.println(w.getWeekNum());
    }
}
```
输出：
> 5

枚举类中还可以有抽象方法。
```java
public enum Week {
    SUNDAY(){
        @Override
        public void getWeekNum() {
            System.out.println(7);
        }
    },
    MONDAY() {
        @Override
        public void getWeekNum() {
            System.out.println("星期一");
        }
    },

    TUESDAY(){
        @Override
        public void getWeekNum() {
            System.out.println("礼拜二");
        }
    },
    WEDNESDAY(){
        @Override
        public void getWeekNum() {
            System.out.println("周三");
        }
    };

    public abstract void getWeekNum();
}

public class Test {
    public static void main(String[] args) {
        Week w = Week.TUESDAY;
        w.getWeekNum();
    }
}
```
输出：
> 礼拜二

<a name="rq5eX"></a>
## `**values()**`**、**`**valueOf(String name)**`** 方法**
每个枚举类都有两个 `static` 方法：

- `static Direction[] values()`：返回本类所有枚举常量；
- `static Direction valueOf(String name)`：通过枚举常量的名字返回`Direction`常量，注意，这个方法与`Enum`类中的`valueOf()`方法的参数个数不同。
```java
public class Test {
    public static void main(String[] args) {
        for (Week w : Week.values()) {
            System.out.println(w);
        }
        System.out.println("星期天：" + Week.valueOf("SUNDAY"));
    }
}
```
结果如下：
> SUNDAY MONDAY TUESDAY WEDNESDAY THURSDAY FRIDAY SATURDAY 星期天：SUNDAY

<a name="lLKUy"></a>
## **枚举的用法**
<a name="TCY5R"></a>
### 「类型约束」
相信大家平时开发过程中，肯定这样定义过常量来使用：
```java
public class Discount {
    public static final double EIGHT_DISCOUNT = 0.8;

    public static final double SIX_DISCOUNT = 0.6;

    public static final double FIVE_DISCOUNT = 0.5;
}
```
这样定义其实也没有什么问题，但是如果有一个方法是这样的：
```java
BigDecimal calculatePrice(double discount){
    //...
}
```
需要传入商品折扣计算价格，使用上面的常量定义就没有类型上的约束，传入任何 double 类型的值都可以，编译器不会发出警告。单如果你使用枚举来定义这种情况，就会有更强的类型约束:
```java
public enum Discount {
    EIGHT_DISCOUNT(0.8), SIX_DISCOUNT(0.6), FIVE_DISCOUNT(0.5);

    private double discountNum;

    Discount(double discountNum) {
        this.discountNum = discountNum;
    }

    double getDiscountNum(){
        return discountNum;
    }
}
```
使用：
```java
public class Test {
    public static void main(String[] args) {
        calculatePrice(Discount.EIGHT_DISCOUNT);
    }

    static BigDecimal calculatePrice(Discount discount){
        System.out.println(discount.getDiscountNum());
        //...
        return null;
    }
}
```
> 0.8

<a name="ubg0I"></a>
### 「switch 中使用」
```java
public class Test {
    public static void main(String[] args) {
        Week w = Week.MONDAY;
        switch (w) {
            case MONDAY:
                System.out.println("周一"); break;
            case TUESDAY:
                System.out.println("周二"); break;
        }
    }
}
```
> 周一

<a name="tDwam"></a>
### 「实现接口，消除 if/else」
创建的枚举类默认是被final修饰，并且默认继承了Enum类。因此不能再继承其他的类。但是可以去实现接口。<br />有这样一个判断场景。
```java
if ("dog".equals(animalType)){
    System.out.println("吃骨头");
} else if ("cat".equals(animalType)) {
    System.out.println("吃鱼干");
} else if ("sheep") {
    System.out.println("吃草");
}
```
怎样用枚举来消除掉 if/else ，看下面的代码：<br />先定义一个接口，里面有一个通用方法 `eat()`
```java
public interface Eat {
    //吃
    String eat();
}
```
然后创建枚举类实现这个接口
```java
public enum AnimalEnum implements Eat {
    Dog(){
        @Override
        public void eat() {
            System.out.println("吃骨头");
        }
    },

    Cat() {
        @Override
        public void eat() {
            System.out.println("吃鱼干");
        }
    },

    Sheep() {
        @Override
        public void eat() {
            System.out.println("吃草");
        }
    }
}
```
调用的时候只需要一行代码：
```java
public class Test {
    public static void main(String[] args) {
        AnimalEnum.valueOf("Cat").eat();
    }
}
```
> 吃鱼干

而且这样一来，以后假如想扩充新的动物，只需要去枚举类中加代码即可，而不用改任何老代码，符合开闭原则！
<a name="c2ZQl"></a>
### 「单例模式中应用」
枚举在单例模式的一种实现方式中也可以用到。
```java
/**
 * @Description: 枚举  线程安全
 */
public class SingletonExample {

    /**
     * 构造函数私有化，避免外部创建实例
     */
    private SingletonExample(){}

    private static SingletonExample getInstance() {
        return Singleton.INSTANCE.getInstance();
    }

    private enum Singleton {
        INSTANCE;
        private SingletonExample instance;

        // JVM 保证这个方法绝对只调用一次
        Singleton() {
            instance = new SingletonExample();
        }

        public SingletonExample getInstance() {
            return instance;
        }
    }
}
```
<a name="lBB25"></a>
## **总结**
Java 中其实还有专门用于枚举的集合类，`EnumSet` 和 `EnumMap`。
