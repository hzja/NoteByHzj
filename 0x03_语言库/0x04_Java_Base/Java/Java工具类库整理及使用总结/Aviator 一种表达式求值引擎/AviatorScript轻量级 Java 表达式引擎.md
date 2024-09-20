- aviator基本使用
   - 基本表达式
   - 表达式变量
   - 使用函数
   - aviatorScript脚本

---

aviator本来是一个轻量级、高性能的基于JVM的表达式引擎。不过从5.0.0版本开始，aviator升级成为了aviatorScript，成为一个高性能、轻量级寄宿于 JVM （包括 Android 平台）之上的脚本语言。<br />根据官网的介绍，aviator支持的主要特性：

1. 支持数字、字符串、正则表达式、布尔值、正则表达式等基本类型，完整支持所有 Java 运算符及优先级等。
2. 函数是一等公民，支持闭包和函数式编程
3. 内置 bigint/decimal类型用于大整数和高精度运算，支持运算符重载得以让这些类型使用普通的算术运算符 +-*/ 参与运算。
4. 完整的脚本语法支持，包括多行数据、条件语句、循环语句、词法作用域和异常处理等。
5. 函数式编程结合 Sequence 抽象，便捷处理任何集合。
6. 轻量化的模块系统。
7. 多种方式，方便地调用 Java 方法，完整支持 Java 脚本 API（方便从 Java 调用脚本）。
8. 丰富的定制选项，可作为安全的语言沙箱和全功能语言使用。
9. 轻量化，高性能，ASM 模式下通过直接将脚本翻译成 JVM 字节码，解释模式可运行于 Android 等非标 Java 平台。

使用场景包括：

1. 规则判断及规则引擎
2. 公式计算
3. 动态脚本控制
4. 集合数据 ELT 等
<a name="nfm7c"></a>
## **aviator基本使用**
<a name="JrTLJ"></a>
### 基本表达式
要使用aviator，只需要添加相应依赖：
```xml
<dependency>
  <groupId>com.googlecode.aviator</groupId>
  <artifactId>aviator</artifactId>
  <version>5.3.3</version>
</dependency>
```
然后就可以进行表达式求值了：
```java
// 返回值为16
Long r = (Long) AviatorEvaluator.execute("2 * (3 + 5)");
```
为了提升性能，往往先编译表达式，然后可以反复执行，进行表达式求值：
```java
Expression expression = AviatorEvaluator.compile("2 * (3 + 5)");
Long r = (Long) expression.execute();
```
aviator支持数字、字符串、布尔值等基本数据类型，数字类型值都当作long或double类型处理。所以上面例子的求值结果是Long。<br />aviator表达式支持大部分的运算操作符，如常用的算术运算操作符(+、-、*、/、%)、逻辑运算操作符（&&、||、！）、比较运算操作符（>、>=、==、!=、<、<=）、位运算操作符（&、|、^、<<、>>）和优先级操作符，还支持三元操作表达(?:)、正则表达式(=~)。<br />一些例子：
```java
// 返回 hello world
String r = (String) AviatorEvaluator.execute("'hello' + ' world'");

// 返回 true
Boolean r = (Boolean) AviatorEvaluator.execute("100 > 80 && 30 < 40");

// 三元表达式，返回 30
Long r = (Long) AviatorEvaluator.execute("100 > 80 ? 30 : 40");

// 正则表达式，正则表达式放在//之间，返回 true
Boolean r = (Boolean) AviatorEvaluator.execute("'hello' =~ /[\\w]+/");
```
<a name="a1pmm"></a>
### 表达式变量
跟其他表达式引擎一样，aviator也是支持表达式求值时传入参数的：
```java
Long a = 12L;
Boolean r = (Boolean) AviatorEvaluator.exec("a > 10", a);
```
参数也可以是一个列表，如下：
```java
List<Long> a = new ArrayList<>();
a.add(12L);
a.add(20L);
Boolean r = (Boolean) AviatorEvaluator.exec("a[0] > 10", a);
```
也可以是一个对象：
```java
public static class Person {
    private String name;
    private Integer age;
}

Person a = new Person("movee", 25);
Boolean r = (Boolean) AviatorEvaluator.exec("a.age > 10", a);
```
跟一般地，aviator会将参数放到一个map中
```java
Map<String, Object> env = new HashMap<>();
env.put("person", new Person("movee", 25));
env.put("a", 20L);
Object result = AviatorEvaluator.execute("person.name", env);
```
这样一来，aviator可以非常方便的从json字符串中提取子json字符串。
```java
String jsonStr = """
    {
        "a": {
            "b": [
                    {
                        "x": 3
                    },
                    {
                        "x": 4
                    }
                ]
        }
    }
    """;


JSONObject jsonObj = new JSONObject(jsonStr);
// 结果返回 3
Object value = AviatorEvaluator.execute("a.b[0]['x']", jsonObj.toMap());
```
<a name="ttHSH"></a>
### 使用函数
aviator已经提供了很多开箱即用的函数了：
```java
// 返回4
Long r = (Long) AviatorEvaluator.execute("math.round(4.3)");

// 返回5
Long r = (Long) AviatorEvaluator.execute("string.length('hello')");

// 返回一个ArrayList：[1,2,3]
Object r = AviatorEvaluator.execute("seq.list(1,2,3)");
```
更详细的内置函数列表请参考：aviator函数库列表<br />也可以自定义一个java函数，自己编写一个类，继承aviator的`AbstractFunction`类，然后实现相应的方法即可：
```java
public class AddFunction extends AbstractFunction {

    /**
     * 函数实现的功能
     * @param env 参数
     * @param arg1 函数的第一个参数
     * @param arg2 函数的第二个参数
     * @return 返回值
     */
    @Override
    public AviatorObject call(Map<String, Object> env, AviatorObject arg1, AviatorObject arg2) {
        long num1 = FunctionUtils.getNumberValue(arg1, env).longValue();
        long num2 = FunctionUtils.getNumberValue(arg2, env).longValue();
        return AviatorLong.valueOf(num1+num2);
    }

    /**
     * 注册到aviator的名字
     * @return 函数名字
     */
    @Override
    public String getName() {
        return "add";
    }
}
```
然后就可以注册到aviator中，像使用内置函数一样使用自定义函数：
```java
// 注册
AviatorEvaluator.addFunction(new AddFunction());

// 使用
long sum = (Long) AviatorEvaluator.getInstance().execute("add(3,4)");
```
<a name="DC2ps"></a>
### aviatorScript脚本
aviator已经升级为一个脚本语言，所以不仅仅能进行表达式求值，还可以执行脚本程序。
```java
// 返回1
Object r = AviatorEvaluator.execute("if (true) { return 1; } else { return 2; }");
```
aviatorScript脚本一般放到独立的脚本文件中，文件名后缀一般为.av<br />例如，编写一个hello.av脚本文件，内容为：
```
if (a > 10) {
    return 10;
} else {
    return a;
}
```
然后就可以执行该脚本文件，并传入参数a的值：
```java
Map<String, Object> env = new HashMap<>();
env.put("a", 30);
Expression exp = AviatorEvaluator.getInstance().compileScript("./hello.av", true);
Object result = exp.execute(env);
```
官方文档：[https://github.com/killme2008/aviatorscript](https://github.com/killme2008/aviatorscript)
