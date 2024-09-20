JavaSpring
<a name="HeUrg"></a>
## Spel概述
Spring表达式语言全称为“Spring Expression Language”，缩写为“SpEL”，类似于Struts2x中使用的OGNL表达式语言，能在运行时构建复杂表达式、存取对象图属性、对象方法调用等等，并且能与Spring功能完美整合，如能用来配置Bean定义。<br />**表达式语言给静态Java语言增加了动态功能。**<br />SpEL是单独模块，只依赖于core模块，不依赖于其他模块，可以单独使用。
<a name="bykDu"></a>
## Spel能干什么?
表达式语言一般是用最简单的形式完成最主要的工作，减少工作量。<br />SpEL支持如下表达式：<br />**一、基本表达式：** 字面量表达式、关系，逻辑与算数运算表达式、字符串连接及截取表达式、三目运算及Elivis表达式、正则表达式、括号优先级表达式；<br />**二、类相关表达式：** 类类型表达式、类实例化、instanceof表达式、变量定义及引用、赋值表达式、自定义函数、对象属性存取及安全导航表达式、对象方法调用、Bean引用；<br />**三、集合相关表达式：** 内联List、内联数组、集合，字典访问、列表，字典，数组修改、集合投影、集合选择；不支持多维内联数组初始化；不支持内联字典定义；<br />**四、其他表达式**：模板表达式。<br />**注：SpEL表达式中的关键字是不区分大小写的。**
<a name="COoRH"></a>
## SpEL基础
<a name="ch2Zp"></a>
### HelloWorld
首先准备支持SpEL的Jar包：“org.springframework.expression-3.0.5.RELEASE.jar”将其添加到类路径中。<br />SpEL在求表达式值时一般分为四步，其中第三步可选：首先构造一个解析器，其次解析器解析字符串表达式，在此构造上下文，最后根据上下文得到表达式运算后的值。<br />看下代码片段：
```java
import org.junit.Test;
import org.springframework.expression.EvaluationContext;
import org.springframework.expression.Expression;
import org.springframework.expression.ExpressionParser;
import org.springframework.expression.spel.standard.SpelExpressionParser;
import org.springframework.expression.spel.support.StandardEvaluationContext;

public class SpelTest {
    @Test
    public void test1() {
        ExpressionParser parser = new SpelExpressionParser();
        Expression expression = parser.parseExpression("('Hello' + ' World').concat(#end)");
        EvaluationContext context = new StandardEvaluationContext();
        context.setVariable("end", "!");
        System.out.println(expression.getValue(context));
    }
}
```
输出
```
Hello World!
```
接下来分析下代码：<br />1）创建解析器：**SpEL使用**`**ExpressionParser**`**接口表示解析器，提供**`**SpelExpressionParser**`**默认实现；**<br />2）解析表达式：使用`ExpressionParser`的`parseExpression`来解析相应的表达式为`Expression`对象。<br />3）构造上下文：准备比如变量定义等等表达式需要的上下文数据。<br />4）求值：通过`Expression`接口的`getValue`方法根据上下文获得表达式值。<br />是不是很简单，接下来看下其具体实现及原理吧。
<a name="HQlSK"></a>
### SpEL原理及接口
SpEL提供简单的接口从而简化用户使用，在介绍原理前学习下几个概念：<br />**一、表达式：** 表达式是表达式语言的核心，所以表达式语言都是围绕表达式进行的，从我们角度来看是“干什么”；<br />**二、解析器：** 用于将字符串表达式解析为表达式对象，从我们角度来看是“谁来干”；<br />**三、上下文：** 表达式对象执行的环境，该环境可能定义变量、定义自定义函数、提供类型转换等等，从我们角度看是“在哪干”；<br />**四、根对象及活动上下文对象：** 根对象是默认的活动上下文对象，活动上下文对象表示了当前表达式操作的对象，从我们角度看是“对谁干”。<br />理解了这些概念后，看下SpEL如何工作的呢，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1684163004287-42c217ae-3fa1-4792-ae13-6a19a074318d.jpeg#averageHue=%23e9e9e9&clientId=ucbc39e59-3d01-4&from=paste&id=uf13bcab2&originHeight=470&originWidth=436&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7759c571-2176-484c-a469-9f8cbfa2a46&title=)
<a name="klddU"></a>
#### 工作原理

1. 首先定义表达式：“1+2”；
2. 定义解析器`ExpressionParser`实现，SpEL提供默认实现`SpelExpressionParser`；

  2.1.`SpelExpressionParser`解析器内部使用Tokenizer类进行词法分析，即把字符串流分析为记号流，记号在SpEL使用Token类来表示；<br />  2.2.有了记号流后，解析器便可根据记号流生成内部抽象语法树；在SpEL中语法树节点由`SpelNode`接口实现代表：如OpPlus表示加操作节点、`IntLiteral`表示int型字面量节点；使用`SpelNodel`实现组成了抽象语法树；<br />  2.3.对外提供`Expression`接口来简化表示抽象语法树，从而隐藏内部实现细节，并提供getValue简单方法用于获取表达式值；SpEL提供默认实现为`SpelExpression`；

3. 定义表达式上下文对象（可选），SpEL使用`EvaluationContext`接口表示上下文对象，用于设置根对象、自定义变量、自定义函数、类型转换器等，SpEL提供默认实现`StandardEvaluationContext`；
4. 使用表达式对象根据上下文对象（可选）求值（调用表达式对象的`getValue`方法）获得结果。

接下来让看下SpEL的主要接口吧：
<a name="trlOZ"></a>
#### `ExpressionParser`接口
表示解析器，默认实现是org.springframework.expression.spel.standard包中的`SpelExpressionParser`类，使用`parseExpression`方法将字符串表达式转换为`Expression`对象，对于`ParserContext`接口用于定义字符串表达式是不是模板，及模板开始与结束字符：
```java
public interface ExpressionParser {
    Expression parseExpression(String expressionString) throws ParseException;
    Expression parseExpression(String expressionString, ParserContext context) throws ParseException;
}
```
来看下示例：
```java
@Test
public void testParserContext() {
    ExpressionParser parser = new SpelExpressionParser();
    ParserContext parserContext = new ParserContext() {
        @Override
        public boolean isTemplate() {
            return true;
        }

        @Override
        public String getExpressionPrefix() {
            return "#{";
        }

        @Override
        public String getExpressionSuffix() {
            return "}";
        }
    };
    String template = "#{'Hello '}#{'World!'}";
    Expression expression = parser.parseExpression(template, parserContext);
    System.out.println(expression.getValue());
}
```
在此演示的是使用`ParserContext`的情况，此处定义了`ParserContext`实现：定义表达式是模块，表达式前缀为“#{”，后缀为“}”；使用`parseExpression`解析时传入的模板必须以“#{”开头，以“}”结尾，如"#{'Hello '}#{'World!'}"。<br />默认传入的字符串表达式不是模板形式，如之前演示的Hello World。
<a name="skf8g"></a>
#### `EvaluationContext`接口
表示上下文环境，默认实现是org.springframework.expression.spel.support包中的`StandardEvaluationContext`类，使用`setRootObject`方法来设置根对象，使用`setVariable`方法来注册自定义变量，使用`registerFunction`来注册自定义函数等等。
<a name="pVdnF"></a>
#### `Expression`接口
表示表达式对象，默认实现是org.springframework.expression.spel.standard包中的`SpelExpression`，提供`getValue`方法用于获取表达式值，提供`setValue`方法用于设置对象值。<br />了解了SpEL原理及接口，接下来的事情就是SpEL语法了。
<a name="MANT4"></a>
## SpEL语法
<a name="r8Q0n"></a>
### 基本表达式
<a name="DbAja"></a>
#### 字面量表达式
SpEL支持的字面量包括：字符串、数字类型（int、long、float、double）、布尔类型、null类型。

| 类型 | 示例 |
| --- | --- |
| 字符串 | `String str1 = parser.parseExpression("'Hello World!'").getValue(String.class);` |
| 数字类型 | `int int1 = parser.parseExpression("1").getValue(Integer.class);`<br />`long long1 = parser.parseExpression("-1L").getValue(long.class);`<br />`float float1 = parser.parseExpression("1.1").getValue(Float.class);`<br />`double double1 = parser.parseExpression("1.1E+2").getValue(double.class);`<br />`int hex1 = parser.parseExpression("0xa").getValue(Integer.class);`<br />`long hex2 = parser.parseExpression("0xaL").getValue(long.class);` |
| 布尔类型 | `boolean true1 = parser.parseExpression("true").getValue(boolean.class);`<br />`boolean false1 = parser.parseExpression("false").getValue(boolean.class);` |
| null类型 | `Object null1 = parser.parseExpression("null").getValue(Object.class);` |

```java
@Test
public void test2() {
    ExpressionParser parser = new SpelExpressionParser();

    String str1 = parser.parseExpression("'Hello World!'").getValue(String.class);
    int int1 = parser.parseExpression("1").getValue(Integer.class);
    long long1 = parser.parseExpression("-1L").getValue(long.class);
    float float1 = parser.parseExpression("1.1").getValue(Float.class);
    double double1 = parser.parseExpression("1.1E+2").getValue(double.class);
    int hex1 = parser.parseExpression("0xa").getValue(Integer.class);
    long hex2 = parser.parseExpression("0xaL").getValue(long.class);
    boolean true1 = parser.parseExpression("true").getValue(boolean.class);
    boolean false1 = parser.parseExpression("false").getValue(boolean.class);
    Object null1 = parser.parseExpression("null").getValue(Object.class);

    System.out.println("str1=" + str1);
    System.out.println("int1=" + int1);
    System.out.println("long1=" + long1);
    System.out.println("float1=" + float1);
    System.out.println("double1=" + double1);
    System.out.println("hex1=" + hex1);
    System.out.println("hex2=" + hex2);
    System.out.println("true1=" + true1);
    System.out.println("false1=" + false1);
    System.out.println("null1=" + null1);
}
```
输出
```
str1=Hello World!
int1=1
long1=-1
float1=1.1
double1=110.0
hex1=10
hex2=10
true1=true
false1=false
null1=null
```
<a name="AbdZT"></a>
#### 算数运算表达式
SpEL支持加(+)、减(-)、乘(*)、除(/)、求余（%）、幂（^）运算。

| 类型 | 示例 |
| --- | --- |
| 加减乘除 | `int result1 = parser.parseExpression("1+2-3*4/2").getValue(Integer.class);//-3` |
| 求余 | `int result2 = parser.parseExpression("4%3").getValue(Integer.class);//1` |
| 幂运算 | `int result3 = parser.parseExpression("2^3").getValue(Integer.class);//8` |

SpEL还提供求余（MOD）和除（DIV）而外两个运算符，与“%”和“/”等价，不区分大小写。
<a name="kEsIY"></a>
#### 关系表达式
等于（==）、不等于(!=)、大于(>)、大于等于(>=)、小于(<)、小于等于(<=)，区间（between）运算。<br />如`parser.parseExpression("1>2").getValue(boolean.class);`将返回false；<br />而`parser.parseExpression("1 between {1, 2}").getValue(boolean.class);`将返回true。<br />between运算符右边操作数必须是列表类型，且只能包含2个元素。第一个元素为开始，第二个元素为结束，区间运算是包含边界值的，即 `xxx>=list.get(0) && xxx<=list.get(1)`。<br />SpEL同样提供了等价的“EQ” 、“NE”、 “GT”、“GE”、 “LT” 、“LE”来表示等于、不等于、大于、大于等于、小于、小于等于，不区分大小写。
```java
@Test
public void test3() {
    ExpressionParser parser = new SpelExpressionParser();
    boolean v1 = parser.parseExpression("1>2").getValue(boolean.class);
    boolean between1 = parser.parseExpression("1 between {1,2}").getValue(boolean.class);
    System.out.println("v1=" + v1);
    System.out.println("between1=" + between1);
}
```
输出
```
v1=false
between1=true
```
<a name="XPVBJ"></a>
#### 逻辑表达式
且（and或者&&）、或(or或者||)、非(!或NOT)。
```java
@Test
public void test4() {
    ExpressionParser parser = new SpelExpressionParser();

    boolean result1 = parser.parseExpression("2>1 and (!true or !false)").getValue(boolean.class);
    boolean result2 = parser.parseExpression("2>1 && (!true || !false)").getValue(boolean.class);

    boolean result3 = parser.parseExpression("2>1 and (NOT true or NOT false)").getValue(boolean.class);
    boolean result4 = parser.parseExpression("2>1 && (NOT true || NOT false)").getValue(boolean.class);

    System.out.println("result1=" + result1);
    System.out.println("result2=" + result2);
    System.out.println("result3=" + result3);
    System.out.println("result4=" + result4);
}
```
输出
```
result1=true
result2=true
result3=true
result4=false
```
<a name="u0zkX"></a>
#### 字符串连接及截取表达式
使用“+”进行字符串连接，使用“'String'[0] [index]”来截取一个字符，目前只支持截取一个，如“'Hello ' + 'World!'”得到“Hello World!”；而“'Hello World!'[0]”将返回“H”。
<a name="eeE9r"></a>
#### 三目运算
三目运算符 **“表达式1?表达式2:表达式3”**用于构造三目运算表达式，如“2>1?true:false”将返回true；
<a name="JDKzU"></a>
#### Elivis运算符
Elivis运算符**“表达式1?:表达式2”**从Groovy语言引入用于简化三目运算符的，当表达式1为非null时则返回表达式1，当表达式1为null时则返回表达式2，简化了三目运算符方式“表达式1? 表达式1:表达式2”，如“null?:false”将返回false，而“true?:false”将返回true；
<a name="UI4J4"></a>
#### 正则表达式
使用“str matches regex，如“'123' matches '\d{3}'”将返回true；
<a name="gNqou"></a>
#### 括号优先级表达式
使用“(表达式)”构造，括号里的具有高优先级。
<a name="YuMxe"></a>
### 类相关表达式
<a name="YeOxr"></a>
#### 类类型表达式
使用“T(Type)”来表示java.lang.Class实例，“Type”必须是类全限定名，“java.lang”包除外，即该包下的类可以不指定包名；使用类类型表达式还可以进行访问类静态方法及类静态字段。<br />具体使用方法如下：
```java
@Test
public void testClassTypeExpression() {
    ExpressionParser parser = new SpelExpressionParser();
    //java.lang包类访问
    Class<String> result1 = parser.parseExpression("T(String)").getValue(Class.class);
    System.out.println(result1);

    //其他包类访问
    String expression2 = "T(com.javacode2018.spel.SpelTest)";
    Class<SpelTest> value = parser.parseExpression(expression2).getValue(Class.class);
    System.out.println(value == SpelTest.class);

    //类静态字段访问
    int result3 = parser.parseExpression("T(Integer).MAX_VALUE").getValue(int.class);
    System.out.println(result3 == Integer.MAX_VALUE);

    //类静态方法调用
    int result4 = parser.parseExpression("T(Integer).parseInt('1')").getValue(int.class);
    System.out.println(result4);
}
```
输出
```
class java.lang.String
true
true
1
```
对于java.lang包里的可以直接使用“T(String)”访问；其他包必须是类全限定名；可以进行静态字段访问如“`T(Integer).MAX_VALUE`”；也可以进行静态方法访问如“`T(Integer).parseInt('1')`”。
<a name="Akr4n"></a>
#### 类实例化
类实例化同样使用java关键字“new”，类名必须是全限定名，但java.lang包内的类型除外，如String、Integer。
```java
@Test
public void testConstructorExpression() {
    ExpressionParser parser = new SpelExpressionParser();
    String result1 = parser.parseExpression("new String('java')").getValue(String.class);
    System.out.println(result1);

    Date result2 = parser.parseExpression("new java.util.Date()").getValue(Date.class);
    System.out.println(result2);
}
```
实例化完全跟Java内方式一样，运行输出
```
java
Tue Aug 03 20:22:43 CST 2020
```
<a name="dWhBt"></a>
#### `instanceof`表达式
SpEL支持`instanceof`运算符，跟Java内使用同义；如“`'haha' instanceof T(String)`”将返回true。
```java
@Test
public void testInstanceOfExpression() {
    ExpressionParser parser = new SpelExpressionParser();
    Boolean value = parser.parseExpression("'Java' instanceof T(String)").getValue(Boolean.class);
    System.out.println(value);
}
```
输出
```
true
```
<a name="rsksx"></a>
#### 变量定义及引用
变量定义通过`EvaluationContext`接口的`setVariable(variableName, value)`方法定义；在表达式中使用`"#variableName"`引用；除了引用自定义变量，SpE还允许引用根对象及当前上下文对象，使用`"#root"`引用根对象，使用`"#this"`引用当前上下文对象；
```java
@Test
public void testVariableExpression() {
    ExpressionParser parser = new SpelExpressionParser();
    EvaluationContext context = new StandardEvaluationContext();
    context.setVariable("name", "java");
    context.setVariable("lesson", "Spring系列");

    //获取name变量，lesson变量
    String name = parser.parseExpression("#name").getValue(context, String.class);
    System.out.println(name);
    String lesson = parser.parseExpression("#lesson").getValue(context, String.class);
    System.out.println(lesson);

    //StandardEvaluationContext构造器传入root对象，可以通过#root来访问root对象
    context = new StandardEvaluationContext("我是root对象");
    String rootObj = parser.parseExpression("#root").getValue(context, String.class);
    System.out.println(rootObj);

    //#this用来访问当前上线文中的对象
    String thisObj = parser.parseExpression("#this").getValue(context, String.class);
    System.out.println(thisObj);
}
```
输出
```
java
Spring系列
我是root对象
我是root对象
```
使用“`#variable`”来引用在`EvaluationContext`定义的变量；除了可以引用自定义变量，还可以使用“#root”引用根对象，“#this”引用当前上下文对象，此处“#this”即根对象。
<a name="WpNgH"></a>
#### 自定义函数
目前只支持类静态方法注册为自定义函数；SpEL使用`StandardEvaluationContext`的`registerFunction`方法进行注册自定义函数，其实完全可以使用`setVariable`代替，两者其实本质是一样的；
```java
@Test
public void testFunctionExpression() throws SecurityException, NoSuchMethodException {
    //定义2个函数,registerFunction和setVariable都可以，不过从语义上面来看用registerFunction更恰当
    StandardEvaluationContext context = new StandardEvaluationContext();
    Method parseInt = Integer.class.getDeclaredMethod("parseInt", String.class);
    context.registerFunction("parseInt1", parseInt);
    context.setVariable("parseInt2", parseInt);

    ExpressionParser parser = new SpelExpressionParser();
    System.out.println(parser.parseExpression("#parseInt1('3')").getValue(context, int.class));
    System.out.println(parser.parseExpression("#parseInt2('3')").getValue(context, int.class));

    String expression1 = "#parseInt1('3') == #parseInt2('3')";
    boolean result1 = parser.parseExpression(expression1).getValue(context, boolean.class);
    System.out.println(result1);
}
```
此处可以看出“registerFunction”和“setVariable”都可以注册自定义函数，但是两个方法的含义不一样，推荐使用“registerFunction”方法注册自定义函数。<br />运行输出
```
3
3
true
```
<a name="CZwwd"></a>
#### 表达式赋值
使用`Expression#setValue`方法可以给表达式赋值
```java
@Test
public void testAssignExpression1() {
    Object user = new Object() {
        private String name;

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        @Override
        public String toString() {
            return "$classname{" +
                    "name='" + name + '\'' +
                    '}';
        }
    };
    {
        //user为root对象
        ExpressionParser parser = new SpelExpressionParser();
        EvaluationContext context = new StandardEvaluationContext(user);
        parser.parseExpression("#root.name").setValue(context, "java");
        System.out.println(parser.parseExpression("#root").getValue(context, user.getClass()));
    }
    {
        //user为变量
        ExpressionParser parser = new SpelExpressionParser();
        EvaluationContext context = new StandardEvaluationContext();
        context.setVariable("user", user);
        parser.parseExpression("#user.name").setValue(context, "java");
        System.out.println(parser.parseExpression("#user").getValue(context, user.getClass()));
    }
}
```
运行输出
```
$classname{name='java'}
$classname{name='java'}
```
<a name="KZmpH"></a>
#### 对象属性存取及安全导航表达式
对象属性获取非常简单，即使用如“`a.property.property`”这种点缀式获取，SpEL对于属性名首字母是不区分大小写的；SpEL还引入了Groovy语言中的安全导航运算符“**(对象|属性)?.属性**”，用来避免“?.”前边的表达式为null时抛出空指针异常，而是返回null；修改对象属性值则可以通过赋值表达式或`Expression`接口的`setValue`方法修改。
```java
public static class Car {
    private String name;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Car{" +
        "name='" + name + '\'' +
        '}';
    }
}

public static class User {
    private Car car;

    public Car getCar() {
        return car;
    }

    public void setCar(Car car) {
        this.car = car;
    }

    @Override
    public String toString() {
        return "User{" +
        "car=" + car +
        '}';
    }
}

@Test
public void test5() {
    User user = new User();
    EvaluationContext context = new StandardEvaluationContext();
    context.setVariable("user", user);

    ExpressionParser parser = new SpelExpressionParser();
    //使用.符号，访问user.car.name会报错，原因：user.car为空
    try {
        System.out.println(parser.parseExpression("#user.car.name").getValue(context, String.class));
    } catch (EvaluationException | ParseException e) {
        System.out.println("出错了：" + e.getMessage());
    }
    //使用安全访问符号?.，可以规避null错误
    System.out.println(parser.parseExpression("#user?.car?.name").getValue(context, String.class));

    Car car = new Car();
    car.setName("保时捷");
    user.setCar(car);

    System.out.println(parser.parseExpression("#user?.car?.toString()").getValue(context, String.class));
}
```
运行输出
```
出错了：EL1007E: Property or field 'name' cannot be found on null
null
Car{name='保时捷'}
```
<a name="LpDSr"></a>
#### 对象方法调用
对象方法调用更简单，跟Java语法一样；如“`'haha'.substring(2,4)`”将返回“ha”；而对于根对象可以直接调用方法；
<a name="IZoWT"></a>
#### Bean引用
SpEL支持使用“@”符号来引用Bean，在引用Bean时需要使用`BeanResolver`接口实现来查找Bean，Spring提供`BeanFactoryResolver`实现。
```java
@Test
public void test6() {
    DefaultListableBeanFactory factory = new DefaultListableBeanFactory();
    User user = new User();
    Car car = new Car();
    car.setName("保时捷");
    user.setCar(car);
    factory.registerSingleton("user", user);

    StandardEvaluationContext context = new StandardEvaluationContext();
    context.setBeanResolver(new BeanFactoryResolver(factory));

    ExpressionParser parser = new SpelExpressionParser();
    User userBean = parser.parseExpression("@user").getValue(context, User.class);
    System.out.println(userBean);
    System.out.println(userBean == factory.getBean("user"));
}
```
运行输出
```
User{car=Car{name='保时捷'}}
true
```
<a name="GONLP"></a>
### 集合相关表达式
<a name="YArKh"></a>
#### 内联List
从Spring3.0.4开始支持内联List，使用{表达式，……}定义内联List，如“{1,2,3}”将返回一个整型的ArrayList，而“{}”将返回空的List，对于字面量表达式列表，SpEL会使用`java.util.Collections.unmodifiableList`方法将列表设置为不可修改。
```java
@Test
public void test7() {
    ExpressionParser parser = new SpelExpressionParser();
    //将返回不可修改的空List
    List<Integer> result2 = parser.parseExpression("{}").getValue(List.class);
    //对于字面量列表也将返回不可修改的List
    List<Integer> result1 = parser.parseExpression("{1,2,3}").getValue(List.class);
    Assert.assertEquals(new Integer(1), result1.get(0));
    try {
        result1.set(0, 2);
    } catch (Exception e) {
        e.printStackTrace();
    }
    //对于列表中只要有一个不是字面量表达式，将只返回原始List，
    //不会进行不可修改处理
    String expression3 = "{{1+2,2+4},{3,4+4}}";
    List<List<Integer>> result3 = parser.parseExpression(expression3).getValue(List.class);
    result3.get(0).set(0, 1);
    System.out.println(result3);
    //声明二维数组并初始化
    int[] result4 = parser.parseExpression("new int[2]{1,2}").getValue(int[].class);
    System.out.println(result4[1]);
    //定义一维数组并初始化
    int[] result5 = parser.parseExpression("new int[1]").getValue(int[].class);
    System.out.println(result5[0]);
}
```
输出
```
java.lang.UnsupportedOperationException
 at java.util.Collections$UnmodifiableList.set(Collections.java:1311)
 at com.javacode2018.spel.SpelTest.test7(SpelTest.java:315)
[[1, 6], [3, 8]]
2
0
```
<a name="bNaHR"></a>
#### 内联数组
和Java 数组定义类似，只是在定义时进行多维数组初始化。
```java
int[][][] result4 = parser.parseExpression("new int[1][2][3]{{1}{2}{3}}").getValue(int[][][].class);
```
<a name="aimDK"></a>
#### 集合，字典元素访问
SpEL目前支持所有集合类型和字典类型的元素访问，使用“集合[索引]”访问集合元素，使用“map[key]”访问字典元素；
```java
//SpEL内联List访问  
int result1 = parser.parseExpression("{1,2,3}[0]").getValue(int.class);  

//SpEL目前支持所有集合类型的访问  
Collection<Integer> collection = new HashSet<Integer>();  
collection.add(1);  
collection.add(2);  

EvaluationContext context2 = new StandardEvaluationContext();  
context2.setVariable("collection", collection);  
int result2 = parser.parseExpression("#collection[1]").getValue(context2, int.class);  


//SpEL对Map字典元素访问的支持  
Map<String, Integer> map = new HashMap<String, Integer>();  
map.put("a", 1);  

EvaluationContext context3 = new StandardEvaluationContext();  
context3.setVariable("map", map);  
int result3 = parser.parseExpression("#map['a']").getValue(context3, int.class);
```
<a name="lHVWm"></a>
#### 列表，字典，数组元素修改
可以使用赋值表达式或`Expression`接口的`setValue`方法修改；
```java
@Test
public void test8() {
    ExpressionParser parser = new SpelExpressionParser();

    //修改list元素值
    List<Integer> list = new ArrayList<Integer>();
    list.add(1);
    list.add(2);

    EvaluationContext context1 = new StandardEvaluationContext();
    context1.setVariable("collection", list);
    parser.parseExpression("#collection[1]").setValue(context1, 4);
    int result1 = parser.parseExpression("#collection[1]").getValue(context1, int.class);
    System.out.println(result1);

    //修改map元素值
    Map<String, Integer> map = new HashMap<String, Integer>();
    map.put("a", 1);
    EvaluationContext context2 = new StandardEvaluationContext();
    context2.setVariable("map", map);
    parser.parseExpression("#map['a']").setValue(context2, 4);
    Integer result2 = parser.parseExpression("#map['a']").getValue(context2, int.class);
    System.out.println(result2);
}
```
输出
```
4
4
```
<a name="QK840"></a>
#### 集合投影
在SQL中投影指从表中选择出列，而在SpEL指根据集合中的元素中通过选择来构造另一个集合，该集合和原集合具有相同数量的元素；SpEL使用“（list|map）.![投影表达式]”来进行投影运算：
```java
@Test
public void test9() {
    ExpressionParser parser = new SpelExpressionParser();

    //1.测试集合或数组
    List<Integer> list = new ArrayList<Integer>();
    list.add(4);
    list.add(5);
    EvaluationContext context1 = new StandardEvaluationContext();
    context1.setVariable("list", list);
    Collection<Integer> result1 = parser.parseExpression("#list.![#this+1]").getValue(context1, Collection.class);
    result1.forEach(System.out::println);

    System.out.println("------------");
    //2.测试字典
    Map<String, Integer> map = new HashMap<String, Integer>();
    map.put("a", 1);
    map.put("b", 2);
    EvaluationContext context2 = new StandardEvaluationContext();
    context2.setVariable("map", map);
    List<Integer> result2 = parser.parseExpression("#map.![value+1]").getValue(context2, List.class);
    result2.forEach(System.out::println);
}
```
对于集合或数组使用如上表达式进行投影运算，其中投影表达式中“#this”代表每个集合或数组元素，可以使用比如“`#this.property`”来获取集合元素的属性，其中“#this”可以省略。<br />Map投影最终只能得到List结果，如上所示，对于投影表达式中的“#this”将是`Map.Entry`，所以可以使用“value”来获取值，使用“key”来获取键。
<a name="favcy"></a>
#### 集合选择
在SQL中指使用select进行选择行数据，而在SpEL指根据原集合通过条件表达式选择出满足条件的元素并构造为新的集合，SpEL使用“(list|map).?[选择表达式]”，其中选择表达式结果必须是boolean类型，如果true则选择的元素将添加到新集合中，false将不添加到新集合中。
```java
@Test
public void test10() {
    ExpressionParser parser = new SpelExpressionParser();

    //1.测试集合或数组
    List<Integer> list = new ArrayList<Integer>();
    list.add(1);
    list.add(4);
    list.add(5);
    list.add(7);
    EvaluationContext context1 = new StandardEvaluationContext();
    context1.setVariable("list", list);
    Collection<Integer> result1 = parser.parseExpression("#list.?[#this>4]").getValue(context1, Collection.class);
    result1.forEach(System.out::println);

    System.out.println("------------");

}
```
输出
```
5
7
```
对于集合或数组选择，如“`#collection.?[#this>4]`”将选择出集合元素值大于4的所有元素。选择表达式必须返回布尔类型，使用“`#this`”表示当前元素。
```java
//2.测试字典
Map<String, Integer> map = new HashMap<String, Integer>();
map.put("a", 1);
map.put("b", 2);
map.put("c", 3);
EvaluationContext context2 = new StandardEvaluationContext();
context2.setVariable("map", map);
Map<String, Integer> result2 = parser.parseExpression("#map.?[key!='a']").getValue(context2, Map.class);
result2.forEach((key, value) -> {
    System.out.println(key + ":" + value);
});
System.out.println("------------");
List<Integer> result3 = parser.parseExpression("#map.?[key!='a'].![value+1]").getValue(context2, List.class);
result3.forEach(System.out::println);
```
输出
```
b:2
c:3
------------
3
4
```
对于字典选择，如“`#map.?[#this.key != 'a']`”将选择键值不等于”a”的，其中选择表达式中“#this”是`Map.Entry`类型，而最终结果还是Map，这点和投影不同；集合选择和投影可以一起使用，如“`#map.?[key != 'a'].![value+1]`”将首先选择键值不等于”a”的，然后在选出的Map中再进行“value+1”的投影。
<a name="QwgRc"></a>
### 表达式模板
模板表达式就是由字面量与一个或多个表达式块组成。每个表达式块由“前缀+表达式+后缀”形式组成，如“${1+2}”即表达式块。在前边已经介绍了使用`ParserContext`接口实现来定义表达式是否是模板及前缀和后缀定义。在此就不多介绍了，如“Error ${#v0} ${#v1}”表达式表示由字面量“Error ”、模板表达式“#v0”、模板表达式“#v1”组成，其中v0和v1表示自定义变量，需要在上下文定义。<br />解析表达式的时候需要指定模板，模板通过`ParserContext`接口来定义
```java
public interface ParserContext {
    //是否是模板
    boolean isTemplate();
    //模板表达式前缀
    String getExpressionPrefix();
    //模板表达式后缀
    String getExpressionSuffix();
}
```
有个子类，直接可以拿来用：`TemplateParserContext`。
```java
@Test
public void test11() {
    //创建解析器
    SpelExpressionParser parser = new SpelExpressionParser();
    //创建解析器上下文
    ParserContext context = new TemplateParserContext("%{", "}");
    Expression expression = parser.parseExpression("你好:%{#name},我们正在学习:%{#lesson}", context);

    //创建表达式计算上下文
    EvaluationContext evaluationContext = new StandardEvaluationContext();
    evaluationContext.setVariable("name", "java");
    evaluationContext.setVariable("lesson", "spring高手!");
    //获取值
    String value = expression.getValue(evaluationContext, String.class);
    System.out.println(value);
}
```
运行输出
```
你好:java,我们正在学习:spring高手!
```
<a name="cjGwh"></a>
## 在Bean定义中使用spel表达式
<a name="ZsWLg"></a>
### xml风格的配置
SpEL支持在Bean定义时注入，默认使用“`#{SpEL表达式}`”表示，其中“`#root`”根对象默认可以认为是`ApplicationContext`，只有`ApplicationContext`实现默认支持SpEL，获取根对象属性其实是获取容器中的Bean。<br />如：
```xml
<bean id="world" class="java.lang.String">  
  <constructor-arg value="#{' World!'}"/>  
</bean>  

<bean id="hello1" class="java.lang.String">  
  <constructor-arg value="#{'Hello'}#{world}"/>  
</bean>    

<bean id="hello2" class="java.lang.String">  
  <constructor-arg value="#{'Hello' + world}"/>
</bean>  

<bean id="hello3" class="java.lang.String">  
  <constructor-arg value="#{'Hello' + @world}"/>  
</bean>
```
模板默认以前缀“`#{`”开头，以后缀“`}`”结尾，且不允许嵌套，如“`#{'Hello'#{world}}`”错误，如`“#{'Hello' + world}`”中“world”默认解析为Bean。当然可以使用“@bean”引用了。<br />是不是很简单，除了XML配置方式，Spring还提供一种注解方式`@Value`，接着往下看吧。
<a name="Qr0vq"></a>
### 注解风格的配置
基于注解风格的SpEL配置也非常简单，使用`@Value`注解来指定SpEL表达式，该注解可以放到字段、方法及方法参数上。<br />测试Bean类如下，使用`@Value`来指定SpEL表达式：
```java
public class SpELBean {  
    @Value("#{'Hello' + world}")  
    private String value;  
}
```
<a name="Nlk3S"></a>
### 在Bean定义中SpEL的问题
如果有同学问“#{我不是SpEL表达式}”不是SpEL表达式，而是公司内部的模板，想换个前缀和后缀该如何实现呢？<br />使用`BeanFactoryPostProcessor`接口提供`postProcessBeanFactory`回调方法，它是在IoC容器创建好但还未进行任何Bean初始化时被`ApplicationContext`实现调用，因此在这个阶段把SpEL前缀及后缀修改掉是安全的，具体代码如下：
```java
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.BeanExpressionResolver;
import org.springframework.beans.factory.config.BeanFactoryPostProcessor;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.context.expression.StandardBeanExpressionResolver;
import org.springframework.stereotype.Component;

@Component
public class SpelBeanFactoryPostProcessor implements BeanFactoryPostProcessor {
    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {
        BeanExpressionResolver beanExpressionResolver = beanFactory.getBeanExpressionResolver();
        if (beanExpressionResolver instanceof StandardBeanExpressionResolver) {
            StandardBeanExpressionResolver resolver = (StandardBeanExpressionResolver) beanExpressionResolver;
            resolver.setExpressionPrefix("%{");
            resolver.setExpressionSuffix("}");
        }
    }
}
```
上测试代码
```java
package com.javacode2018.spel.test1;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

@Component
public class LessonModel {
    @Value("你好,%{@name},%{@msg}")
    private String desc;

    @Override
    public String toString() {
        return "LessonModel{" +
        "desc='" + desc + '\'' +
        '}';
    }
}
```
@name：容器中name的bean<br />@msg：容器中msg的bean<br />下面来个配置类，顺便定义name和msg这2个bean，顺便扫描上面2个配置类
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

@ComponentScan
@Configuration
public class MainConfig {
    @Bean
    public String name() {
        return "Fcant";
    }

    @Bean
    public String msg() {
        return "学习java！";
    }
}
```
测试用例
```java
@Test
public void test12() {
    AnnotationConfigApplicationContext context = new AnnotationConfigApplicationContext();
    context.register(MainConfig.class);
    context.refresh();
    LessonModel lessonModel = context.getBean(LessonModel.class);
    System.out.println(lessonModel);
}
```
运行输出
```
LessonModel{desc='你好,Fcant,学习java！'}
```
<a name="CSE8E"></a>
## 总结

1. Spel功能还是比较强大的，可以脱离Spring环境独立运行
2. Spel可以用在一些动态规则的匹配方面，比如监控系统中监控规则的动态匹配；其他的一些条件动态判断等等
