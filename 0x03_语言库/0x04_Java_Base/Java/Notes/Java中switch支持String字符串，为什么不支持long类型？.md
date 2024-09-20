Java<br />Java Switch 支持byte、short、int类型，在JDK 1.5 时，支持了枚举类型，在 JDK1.7时，又支持了String类型。那么它为什么就不能支持long类型呢，明明它跟 byte、short、int 一样都是数值型，它又是咋支持 String 类型的呢？
<a name="KyLa0"></a>
## 一、结论
先说结论：switch 底层是使用 int 型 来进行判断的，即使是枚举、String类型，最终也是转变成 int 型。由于 long 型表示范围大于 int 型，因此不支持 long 类型。<br />下面详细介绍下各个类型是如何被转变成 int 类型的，使用的编译命令为 javac，反编译网站为：[http://javare.cn](http://javare.cn)
<a name="HwsBU"></a>
## 二、枚举类型是怎么变成 int 类型的？
在没有实验之前，想当然的认为它是不是根据枚举的 int 型字段来计算的（因为一般枚举都是一个int型，一个string型），但是转念一想，万一枚举没有 int 型字段呢，万一有多个 int 型字段呢，所以肯定不是这样的，下面看实验吧。<br />定义两个枚举类，一个枚举类有一个int型属性，一个string型属性，另外一个枚举类只有一个string属性：
```java
public enum SexEnum {
    MALE(1, "男"),
    FEMALE(0, "女");

    private int type;

    private String name;

    SexEnum(int type, String name) {
        this.type = type;
        this.name = name;
    }
}
public enum Sex1Enum {
    MALE("男"),
    FEMALE("女");
    private String name;

    Sex1Enum(String name) {
        this.name = name;
    }
}
```
然后编写一个测试类，并且让两个枚举 switch 的 FEMALE 和 MALE 对应的返回值不同：
```java
public class SwitchTest {
    public int enumSwitch(SexEnum sex) {
        switch (sex) {
            case MALE:
                return 1;
            case FEMALE:
                return 2;
            default:
                return 3;
        }
    }

    public int enum1Switch(Sex1Enum sex) {
        switch (sex) {
            case FEMALE:
                return 1;
            case MALE:
                return 2;
            default:
                return 3;
        }
    }
}
```
将这几个类反编译下：
```java
// SexEnum.class
public enum SexEnum {

   MALE(1, "男"),
   FEMALE(0, "女");
   private int type;
   private String name;
   // $FF: synthetic field
   private static final SexEnum[] $VALUES = new SexEnum[]{MALE, FEMALE};


   private SexEnum(int var3, String var4) {
      this.type = var3;
      this.name = var4;
   }

}

// Sex1Enum.class
public enum Sex1Enum {

   MALE("男"),
   FEMALE("女");
   private String name;
   // $FF: synthetic field
   private static final Sex1Enum[] $VALUES = new Sex1Enum[]{MALE, FEMALE};


   private Sex1Enum(String var3) {
      this.name = var3;
   }

}
```
反编译这两个枚举类，发现其中多了一个 $VALUES 数组，内部包含了所有的枚举值。继续反编译测试类：
```java
// SwitchTest$1.class
import com.example.express.test.Sex1Enum;
import com.example.express.test.SexEnum;

// $FF: synthetic class
class SwitchTest$1 {

   // $FF: synthetic field
   static final int[] $SwitchMap$com$example$express$test$SexEnum;
   // $FF: synthetic field
   static final int[] $SwitchMap$com$example$express$test$Sex1Enum = new int[Sex1Enum.values().length];


   static {
      try {
         $SwitchMap$com$example$express$test$Sex1Enum[Sex1Enum.FEMALE.ordinal()] = 1;
      } catch (NoSuchFieldError var4) {
         ;
      }

      try {
         $SwitchMap$com$example$express$test$Sex1Enum[Sex1Enum.MALE.ordinal()] = 2;
      } catch (NoSuchFieldError var3) {
         ;
      }

      $SwitchMap$com$example$express$test$SexEnum = new int[SexEnum.values().length];

      try {
         $SwitchMap$com$example$express$test$SexEnum[SexEnum.MALE.ordinal()] = 1;
      } catch (NoSuchFieldError var2) {
         ;
      }

      try {
         $SwitchMap$com$example$express$test$SexEnum[SexEnum.FEMALE.ordinal()] = 2;
      } catch (NoSuchFieldError var1) {
         ;
      }

   }
}
```
首先生成了一个名为 SwitchTest$1.java 的链接类，里面定义了两个枚举数组，这两个数组元素添加的顺序完全和测试类中 switch 类调用的顺序一致。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673238571088-4d5d6a88-ea03-4b6e-bd48-56d7e6e608c7.png#averageHue=%23c1c0c0&clientId=u7dfe8d38-7ade-4&from=paste&id=u81122ba1&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0b67f108-e388-4164-81f3-d32b4c69bf0&title=)<br />枚举元素在数组中的下标由 `ordinal()` 函数决定，该方法就是返回枚举元素在枚举类中的序号。<br />这里其实就已经知道了，在 switch 语句中，是根据枚举元素在枚举中的序号来转变成 int 型的。最后再看下测试类的反编译结果验证下：
```java
// SwitchTest.class
import com.example.express.test.Sex1Enum;
import com.example.express.test.SexEnum;
import com.example.express.test.SwitchTest.1;

public class SwitchTest {
   public int enumSwitch(SexEnum var1) {
      switch(1.$SwitchMap$com$example$express$test$SexEnum[var1.ordinal()]) {
      case 1:
         return 1;
      case 2:
         return 2;
      default:
         return 3;
      }
   }

   public int enum1Switch(Sex1Enum var1) {
      switch(1.$SwitchMap$com$example$express$test$Sex1Enum[var1.ordinal()]) {
      case 1:
         return 1;
      case 2:
         return 2;
      default:
         return 3;
      }
   }
}
```
<a name="ex6OI"></a>
## 三、String 类型是怎么变成 int 类型的？
首先知道 char 类型是如何变成 int 类型的，很简单，是 ASCII 码，例如存在 switch 语句：
```java
public int charSwitch(char c) {
    switch (c) {
        case 'a':
            return 1;
        case 'b':
            return 2;
        default:
            return Integer.MAX_VALUE;
    }
}
```
反编译结果为：
```java
public int charSwitch(char var1) {
    switch(var1) {
        case 97:
            return 1;
        case 98:
            return 2;
        default:
            return Integer.MAX_VALUE;
    }
}
```
那么对于 String 来说，利用的就是 `hashCode()` 函数了，但是 两个不同的字符串 `hashCode()` 是有可能相等的，这时候就得靠 `equals()` 函数了，例如存在 switch 语句：
```java
public int stringSwitch(String ss) {
    switch (ss) {
        case "ABCDEa123abc":
            return 1;
        case "ABCDFB123abc":
            return 2;
        case "helloWorld":
            return 3;
        default:
            return Integer.MAX_VALUE;
    }
}
```
其中字符串 ABCDEa123abc 和 ABCDFB123abc 的 hashCode 是相等的，反编译结果为：
```java
public int stringSwitch(String var1) {
   byte var3 = -1;
   switch(var1.hashCode()) {
       case -1554135584:
          if(var1.equals("helloWorld")) {
             var3 = 2;
          }
          break;
       case 165374702:
          if(var1.equals("ABCDFB123abc")) {
             var3 = 1;
          } else if(var1.equals("ABCDEa123abc")) {
             var3 = 0;
          }
   }

   switch(var3) {
       case 0:
          return 1;
       case 1:
          return 2;
       case 2:
          return 3;
       default:
          return Integer.MAX_VALUE;
   }
}
```
可以看到它引入了局部变量 var3，对于 hashCode 相等情况通过 equals() 方法判断，最后再判断 var3 的值。
<a name="BAVaZ"></a>
## 四、它们的包装类型支持吗？
这里以 Integer 类型为例，Character 和 Byte 同理，例如存在 switch 语句：
```java
public int integerSwitch(Integer c) {
    switch (c) {
        case 1:
            return 1;
        case 2:
            return 2;
    }
    return -1;
}
```
反编译结果为：
```java
public int integerSwitch(Integer var1) {
    switch(var1.intValue()) {
        case 1:
            return 1;
        case 2:
            return 2;
        default:
            return -1;
    }
}
```
可以看到，是支持包装类型的，通过自动拆箱解决。<br />那万一包装类型是 NULL 咋办，首先都知道 swtich 的 case 是不给加 null 的，编译都通不过，那如果传 null 呢？<br />答案是 NPE，毕竟实际还是包装类型的拆箱，自然就报空指针了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673238571097-f89542e6-6ef6-4e79-a378-d7ecb9c1f87d.png#averageHue=%23302e2e&clientId=u7dfe8d38-7ade-4&from=paste&id=ufb814696&originHeight=114&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3060b0e3-a105-4534-b51e-e50096ae511&title=)
