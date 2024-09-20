Java Aviator
<a name="fX4Ha"></a>
## 1、简介
Aviator是一个高性能、轻量级的 java 语言实现的表达式求值引擎， 主要用于各种表达式的动态求值。现在已经有很多开源可用的 java 表达式求值引擎，为什么还需要 Avaitor 呢?<br />Aviator的设计目标是轻量级和高性能，相比于Groovy、JRuby的笨重， Aviator非常小， 加上依赖包也才 537K，不算依赖包的话只有 70K; 当然， Aviator的语法是受限的， 它不是一门完整的语言， 而只是语言的一小部分集合。<br />其次， Aviator的实现思路与其他轻量级的求值器很不相同， 其他求值器一般都是通过解释的方式运行， 而Aviator则是直接将表达式编译成 JVM 字节码， 交给 JVM 去执行。简单来说， Aviator的定位是介于 Groovy 这样的重量级脚本语言和 IKExpression 这样的轻量级表达式引擎之间。
<a name="nfovo"></a>
## 2、特性
Aviator的特性：

- 支持绝大多数运算操作符，包括算术操作符、关系运算符、逻辑操作符、位运算符、正则匹配操作符(=~)、三元表达式(`? :`)；
- 支持操作符优先级和括号强制设定优先级；
- 逻辑运算符支持短路运算；
- 支持丰富类型，例如nil、整数和浮点数、字符串、正则表达式、日期、变量等，支持自动类型转换；
- 支持传入变量，支持类似a.b.c的嵌套变量访问；
- 内置一套强大的常用函数库；
- 可自定义函数，易于扩展；
- 可重载操作符；
- 支持大数运算(BigInteger)和高精度运算(BigDecimal)；
- 性能优秀。

---

Aviator的限制：

- 没有`if else`、`do while`等语句，没有赋值语句，仅支持逻辑表达式、算术表达式、三元表达式和正则匹配。
- 无法自定义运算符，自定义函数有局限性
<a name="Lx9qv"></a>
## 3、依赖
Aviator依赖了commons-beanutils，使用Aviator可以添加下面的maven依赖：
```xml
<dependency>
  <groupId>com.googlecode.aviator</groupId>
  <artifactId>aviator</artifactId>
  <version>{version}</version>
</dependency>
```
从 3.2.0 版本开始， Aviator 仅支持 JDK 7 及其以上版本。JDK 6 请使用 3.1.1 这个稳定版本。
<a name="F0XzM"></a>
## 4、使用手册
<a name="xTvnO"></a>
### 执行表达式
Aviator的使用都是集中通过`com.googlecode.aviator.AviatorEvaluator`这个入口类来处理，最简单的例子，执行一个计算1+2+3的表达式：
```java
val result = AviatorEvaluator.execute("1+2+3")
println(result)     //6
```
如果开启了 `ALWAYS_PARSE_FLOATING_POINT_NUMBER_INTO_DECIMAL` 选项，那么在表达式中出现的浮点数都将解析为 `BigDecimal`，这是为了方便一些用户要求高精度的计算，又不想额外地给浮点数加上 M 后缀标记为 `BigDecimal`：
```java
AviatorEvaluator.setOption(Options.ALWAYS_PARSE_FLOATING_POINT_NUMBER_INTO_DECIMAL, true)
```
<a name="XxKH3"></a>
### 多行表达式
从 4.0.0 开始， aviator支持以分号 ; 隔开的多行表达式，对于多行表达式求值的结果将是最后一个表达式的结果，例如
```java
val result = AviatorEvaluator.execute("println('hello world'); 1+2+3 ; 100-1")
println(result) 
//输出结果：
//hello world
//99
```
表达式的结果将是最后一个表达式 100-1，也就是 99，但是中间的表达式也将执行，包括打印 hello world。通过在表达式求值过程中加入 println打印，可以方便调试，也可以通过 `Options.TRACE_EVAL`来跟踪执行过程，参见后续章节。
<a name="er9ps"></a>
### 求值器多实例
`AviatorEvaluator`是一个全局静态实例，但是很多场景下，可能想为不同的场景提供一个不同的求值器实例，包括不同的选项配置和自定义函数列表等，那么从 4.0.0开始， Aviator提供了多实例的求值器支持：
```java
val instance = AviatorEvaluator.newInstance
//接下来使用 instance，几乎跟 AviatorEvaluator 没有不同，只是换成了实例方法
```
<a name="Qwr5e"></a>
### 使用变量
下面的例子演示了怎么向表达式传入变量值，表达式中的world是一个变量，默认为null，通过传入`Map<String,Object>`的变量绑定环境，将world设置为“world”。env 的key是变量名，value是变量的值。
```java
val world = "world"
val env = new util.HashMap[String,Object]()
env.put("world",world)
val result =AviatorEvaluator.execute(" 'hello ' + world ",env)
println(result)        //  hello world
```
'hello'是一个Aviator的String， Aviator的String是任何用单引号或者双引号括起来的字符序列， String可以比较大小(基于unicode顺序)， 可以参与正则匹配， 可以与任何对象相加， 任何对象与String相加结果为String。String中也可以有转义字符，如\n、\、' 等。
```java
AviatorEvaluator.execute(" 'a\"b' ")                  // 字符串 a"b
AviatorEvaluator.execute(" \"a\'b\" ")                // 字符串 a'b
AviatorEvaluator.execute(" 'hello ' + 3 ")              // 字符串 hello 3
AviatorEvaluator.execute(" 'hello '+ unknow ")         // 字符串 hello null
```
<a name="qGYEQ"></a>
### exec 方法
Aviator 2.2 开始新增加一个exec方法，可以更方便地传入变量并执行，而不需要构造env这个map了：
```java
val value = "world"
AviatorEvaluator.exec("'hello' + value",value)        //  hello world
```
<a name="CGXUm"></a>
### 调用函数
Aviator 支持函数调用，函数调用的风格类似 lua，下面的例子获取字符串的长度：
```java
AviatorEvaluator.execute("string.length('hello')") 
```
string.length(‘hello’)是一个函数调用，string.length是一个函数，'hello'是调用的参数。<br />再用string.substring来截取字符串：
```java
AviatorEvaluator.execute("string.contains(\"test\", string.substring('hello', 1, 2))") // true
```
通过`string.substring('hello', 1, 2)`获取字符串'e'，然后通过函数`string.contains`判断e是否在'test'中。可以看到，函数可以嵌套调用。
<a name="bWVAe"></a>
### 自定义函数
Aviator除了内置的函数之外，还允许用户自定义函数，只要实现`com.googlecode.aviator.runtime.type.AviatorFunction`接口，并注册到`AviatorEvaluator`即可使用。`AviatorFunction`接口十分庞大，通常来说并不需要实现所有的方法，只要根据方法的参数个数，继承`AbstractFunction`类并`override`相应方法即可。
```java
object test {
  def main(args: Array[String]): Unit = {
    AviatorEvaluator.addFunction(new AddFunction)
    System.out.println(AviatorEvaluator.execute("add(1, 2)")) // 3.0
    System.out.println(AviatorEvaluator.execute("add(add(1, 2), 100)")) // 103.0
  }
}
class AddFunction extends AbstractFunction{
  override def call(env: java.util.Map[String, Object], arg1: AviatorObject, arg2: AviatorObject): AviatorObject = {
    val left = FunctionUtils.getNumberValue(arg1, env)
    val right = FunctionUtils.getNumberValue(arg2, env)
    new AviatorDouble(left.doubleValue + right.doubleValue)
  }

  def getName = "add"
}
```
注册函数通过`AviatorEvaluator.addFunction`方法，移除可以通过`removeFunction`。另外， `FunctionUtils` 提供了一些方便参数类型转换的方法。<br />如果参数个数不确定，可以继承 `AbstractVariadicFunction` 类，只要实现其中的 `variadicCall` 方法即可，比如实现一个找到第一个参数不为 null 的函数：
```java
object test {
  def main(args: Array[String]): Unit = {
    AviatorEvaluator.addFunction(new GetFirstNonNullFunction)
    System.out.println(AviatorEvaluator.execute("getFirstNonNull(1)")) // 1
    System.out.println(AviatorEvaluator.execute("getFirstNonNull(1,2,3,4,nil,5)")) // 1
    System.out.println(AviatorEvaluator.execute("getFirstNonNull(a,b,c,d)")) // null
  }
}
class GetFirstNonNullFunction extends AbstractVariadicFunction {
  override def variadicCall(env: util.Map[String, AnyRef], args: AviatorObject*): AviatorObject = {
    if (args != null) for (arg <- args) {
      if (arg.getValue(env) != null) return arg
    }
    new AviatorString(null)
  }

  override def getName = "getFirstNonNull"

}
```
当然，同时仍然覆写特定的 call 方法来自定义实现。<br />自定义函数在 4.0.0 之后也可以通过 lambda 来定义：
```java
AviatorEvaluator.defineFunction("add", "lambda (x,y) -> x + y end")
AviatorEvaluator.exec("add(1,2)")    //3
```
<a name="rJl5T"></a>
### 函数加载器
从 4.0.0 开始，Aviator 还支持 `FunctionLoader`接口，可以用于自定义函数加载器：
```java
class function extends FunctionLoader{
  /**
    * Invoked when function not found
    *
    * @param name function name
    */
  override def onFunctionNotFound(name: String): AviatorFunction = ???
}
```
用户可以自主实现函数加载器，当函数不能从当前求值器中找到的时候，将调用 loader 的`onFunctionNotFound` 方法进行查找。自定义的加载器，通过 `AviatorEvaluator.addFunctionLoader(loader)`注册，可以注册多个加载器，加载顺序将按照添加顺序进行查找，其中任何一个找到，都将中断查找过程。
<a name="yfiYX"></a>
### 重载运算符
Aviator 支持的运算符参见操作符一节。部分用户可能有重载这些内置运算符的需求，例如在 Excel 里， & 不是位运算，而是字符串连接符，那么可以通过 3.3.0 版本支持的运算符重载来实现：
```java
AviatorEvaluator.addOpFunction(OperatorType.BIT_AND, new AbstractFunction() {
      override def call(env: util.Map[String, Object], arg1: AviatorObject, arg2: AviatorObject) = new AviatorString(arg1.getValue(env).toString + arg2.getValue(env).toString)

      override def getName = "&"
    })
```
`AviatorEvaluator.addOpFunction(opType, func)` 就可以重载指定的运算符，重载后运行即可看到：
```java
val map = new util.HashMap[String,Object]()
map.put("a","4")
val result = AviatorEvaluator.execute("a&3", map)
println(result)      //43
val map1 = new util.HashMap[String,Object]()
map1.put("a","hello")
val result1 =  AviatorEvaluator.execute("a&' world'", map1)
println(result1)    //hello world
```
请注意，运算符重载使用不当，一定程度上会带来混乱，并且有一定的性能损失，请慎重使用。
<a name="tChqD"></a>
### 编译表达式
上面提到的例子都是直接执行表达式，事实上 Aviator 背后做了编译并执行的工作。可以自己先编译表达式，返回一个编译的结果，然后传入不同的`env`来复用编译结果，提高性能，这是更推荐的使用方式：
```java
val expression = "a-(b-c)>100"
// 编译表达式
val compiledExp = AviatorEvaluator.compile(expression)
val env = new util.HashMap[String,Object]()
env.put("a", 100.3.asInstanceOf[Object])
env.put("b", new Integer(45))
env.put("c", (-199.100).asInstanceOf[Object])
// 执行表达式
val result = compiledExp.execute(env).asInstanceOf[Boolean]
println(result) // false
```
通过compile方法可以将表达式编译成Expression的中间对象，当要执行表达式的时候传入env并调用Expression的execute方法即可。表达式中使用了括号来强制优先级，这个例子还使用了`>`用于比较数值大小，比较运算符!=、==、>、>=、<、<=不仅可以用于数值，也可以用于String、Pattern、Boolean等等，甚至是任何用户传入的两个都实现了`java.lang.Comparable`接口的对象之间。<br />编译后的结果可以自己缓存，也可以交给 Aviator 缓存，`AviatorEvaluator`内部有一个全局的缓存池，如果决定缓存编译结果，可以通过：
```java
public static Expression compile(final String expression, final boolean cached)
```
将cached设置为true即可，那么下次编译同一个表达式的时候将直接返回上一次编译的结果。<br />使缓存失效通过：
```java
public static void invalidateCache(String expression)
```
<a name="vKmWn"></a>
### 访问数组和集合
可以通过中括号去访问数组和java.util.List对象，可以通过map.key访问java.util.Map中key对应的value，一个例子:
```java
val list = new util.ArrayList[String]
list.add("hello")
list.add(" world")
val array = new Array[Int](3)
array(0) = 0
array(1) = 1
array(2) = 3
val map = new util.HashMap[String,Object]()
map.put("date", new Date())
val env = new util.HashMap[String,Object]()
env.put("list", list)
env.put("array", array)
env.put("mmap", map)
println(AviatorEvaluator.execute("list[0]+list[1]", env)) // hello world

println(AviatorEvaluator.execute("'array[0]+array[1]+array[2]=' + (array[0]+array[1]+array[2])", env)) // array[0]+array[1]+array[2]=4

println(AviatorEvaluator.execute("'today is ' + mmap.date ", env)) // today is Tue Mar 19 11:42:51 CST 2019
```
如果函数调用或者括号表达式结果是一个数组或者List，同样可以可以通过 `[index]` 访问：
```java
println(AviatorEvaluator.exec("string.split(s,',')[0]", "a,b,c,d"))    //a
```
<a name="XYmUa"></a>
### 三元操作符
Aviator 不提供if else语句，但是提供了三元操作符`?:`用于条件判断，使用上与 java 没有什么不同:
```java
println(AviatorEvaluator.exec("a>0? 'yes':'no'", new Integer(1))) // yes
```
Aviator 的三元表达式对于两个分支的结果类型并不要求一致，可以是任何类型，这一点与 java 不同。
<a name="QJEOE"></a>
### 正则表达式匹配
Aviator 支持类 Ruby 和 Perl 风格的表达式匹配运算，通过=~操作符，如下面这个例子匹配 email 并提取用户名返回：
```java
val email = "killme2008@gmail.com"
val env = new util.HashMap[String,Object]()
env.put("email", email)
val username = AviatorEvaluator.execute("email=~/([\\w0-8]+)@\\w+[\\.\\w+]+/ ? $1 : 'unknow' ", env).asInstanceOf[String]
println(username) // killme2008
```
email与正则表达式`/([\w0-8]+@\w+[\.\w+]+)/`通过=~操作符来匹配，结果为一个 Boolean 类 型， 因此可以用于三元表达式判断，匹配成功的时候返回$1，指代正则表达式的分组 1，也就是用户名，否则返回unknown。<br />Aviator 在表达式级别支持正则表达式，通过//括起来的字符序列构成一个正则表达式，正则表达式可以用于匹配(作为=~的右操作数)、比较大小。但是匹配仅能与字符串进行匹配。匹配成功后， Aviator 会自动将匹配成功的捕获分组(capturing groups) 放入 `env ${num}`的变量中，其中`$0` 指代整个匹配的字符串，而`$1`表示第一个分组，`$2`表示第二个分组以此类推。<br />请注意，分组捕获放入 env 是默认开启的，因此如果传入的 env 不是线程安全并且被并发使用，可能存在线程安全的隐患。关闭分组匹配，可以通过 `AviatorEvaluator.setOption(Options.PUT_CAPTURING_GROUPS_INTO_ENV, false);` 来关闭，对性能有稍许好处。<br />Aviator 的正则表达式规则跟 Java 完全一样，因为内部其实就是使用java.util.regex.Pattern做编译的。
<a name="golmw"></a>
### 变量的语法糖
Aviator 有个方便用户使用变量的语法糖，当要访问变量a中的某个属性b，那么可以通过a.b访问到，更进一步，a.b.c将访问变量a的b属性中的c属性值，推广开来也就是说 Aviator 可以将变量声明为嵌套访问的形式。<br />TestAviator类符合JavaBean规范，并且是 `public` 的，就可以使用语法糖：
```java
object test { 

  def main(args: Array[String]): Unit = {
    val foo = new Foo(100, 3.14f, new Date())
    val env = new util.HashMap[String,Object]()
    env.put("foo", foo)
    println(AviatorEvaluator.execute("'foo.i = '+foo.i", env)) // foo.i = 100

    println(AviatorEvaluator.execute("'foo.f = '+foo.f", env)) // foo.f = 3.14

    println(AviatorEvaluator.execute("'foo.date.year = '+(foo.date.year+1900)", env)) // foo.date.year = 2019

  }
}

class Foo(var i: Int, var f: Float, var date: Date) {

  def getI: Int = i

  def setI(i: Int): Unit = {
    this.i = i
  }

  def getF: Float = f

  def setF(f: Float): Unit = {
    this.f = f
  }

  def getDate: Date = date

  def setDate(date: Date): Unit = {
    this.date = date
  }
}
```
<a name="nkwLE"></a>
### nil 对象
nil是 Aviator 内置的常量，类似 java 中的null，表示空的值。nil跟null不同的在于，在 java 中null只能使用在==、!=的比较运算符，而nil还可以使用>、>=、<、<=等比较运算符。Aviator 规定，任何对象都比nil大除了nil本身。用户传入的变量如果为null，将自动以nil替代。
```java
AviatorEvaluator.execute("nil == nil");   //true
AviatorEvaluator.execute(" 3> nil");      //true
AviatorEvaluator.execute(" true!= nil");  //true
AviatorEvaluator.execute(" ' '>nil ");    //true
AviatorEvaluator.execute(" a==nil ");     //true, a 是 null
```
nil与String相加的时候，跟 java 一样显示为 null
<a name="g3RUZ"></a>
### 日期比较
Aviator 并不支持日期类型，如果要比较日期，需要将日期写字符串的形式，并且要求是形如 “yyyy-MM-dd HH:mm:ss:SS”的字符串，否则都将报错。字符串跟java.util.Date比较的时候将自动转换为Date对象进行比较：
```java
val env = new util.HashMap[String,Object]()
val date = new Date()
val dateStr = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SS").format(date)
env.put("date", date)
env.put("dateStr", dateStr)
var result = AviatorEvaluator.execute("date==dateStr", env).asInstanceOf[Boolean]
println(result) // true
result = AviatorEvaluator.execute("date > '2010-12-20 00:00:00:00' ", env).asInstanceOf[Boolean]
println(result)// true
result = AviatorEvaluator.execute("date < '2200-12-20 00:00:00:00' ", env).asInstanceOf[Boolean]
println(result)// true
result = AviatorEvaluator.execute("date==date ", env).asInstanceOf[Boolean]
println(result)// true
```
也就是说String除了能跟String比较之外，还能跟nil和java.util.Date对象比较。
<a name="hTXeY"></a>
### 大数计算和精度
从 2.3.0 版本开始，aviator 开始支持大数字计算和特定精度的计算， 本质上就是支持java.math.BigInteger和java.math.BigDecimal两种类型， 这两种类型在 aviator 中简称为big int和decimal类型。类似99999999999999999999999999999999这样的数字在 Java 语言里是没办法编译通过 的， 因为它超过了Long类型的范围， 只能用BigInteger来封装。但是 aviator 通过包装，可以直接支持这种大整数的计算，例如：
```java
println(AviatorEvaluator.execute("99999999999999999999999999999999 + 99999999999999999999999999999999"))
//结果：199999999999999999999999999999998
```
<a name="reULy"></a>
### 字面量表示
big int和decimal的表示与其他数字不同，两条规则：<br />以大写字母N为后缀的整数都被认为是big int，如1N，2N，9999999999999999999999N等， 都是big int类型。<br />超过long范围的整数字面量都将自动转换为big int类型。<br />以大写字母M为后缀的数字都被认为是decimal， 如1M，2.222M， 100000.9999M等， 都是decimal类型。<br />用户也可以通过变量传入这两种类型来参与计算。<br />如果用户觉的给浮点数添加 M 后缀比较繁琐，也可以强制所有浮点数解析为 BigDecimal，通过代码开启下列选项即可：
```java
AviatorEvaluator.setOption(Options.ALWAYS_PARSE_FLOATING_POINT_NUMBER_INTO_DECIMAL, true)
```
<a name="zb2j3"></a>
### 运算
big int和decimal的运算，跟其他数字类型long，double没有什么区别，操作符仍然是一样的。aviator重载了基本算术操作符来支持这两种新类型：
```java
var rt = AviatorEvaluator.exec("9223372036854775807100.356M * 2")
println(rt + " " + rt.getClass) // 18446744073709551614200.712 class java.math.BigDecimal
rt = AviatorEvaluator.exec("92233720368547758074+1000")
println(rt + " " + rt.getClass) // 92233720368547759074 class java.math.BigInteger
val a = new BigInteger(String.valueOf(Long.MaxValue) + String.valueOf(Long.MaxValue))
val b = new java.math.BigDecimal("3.2")
val c = new java.math.BigDecimal("9999.99999")
rt = AviatorEvaluator.exec("a+10000000000000000000", a)
println(rt + " " + rt.getClass) // 92233720368547758089223372036854775807 class java.math.BigInteger
rt = AviatorEvaluator.exec("b+c*2", b, c)
println(rt + " " + rt.getClass) // 20003.19998 class java.math.BigDecimal
rt = AviatorEvaluator.exec("a*b/c", a, b, c)
println(rt + " " + rt.getClass) // 2.951479054745007313280155218459508E+34 class java.math.BigDecimal
```
<a name="sQwKq"></a>
### 类型转换和提升
当big int或者decimal和其他类型的数字做运算的时候，按照long < big int < decimal < double的规则做提升， 也就是说运算的数字如果类型不一致， 结果的类型为两者之间更“高”的类型。例如：<br />1 + 3N， 结果为big int的4N<br />1 + 3.1M，结果为decimal的4.1M<br />1N + 3.1M，结果为decimal的 4.1M<br />1.0 + 3N，结果为double的4.0<br />1.0 + 3.1M，结果为double的4.1<br />decimal 的计算精度<br />Java 的java.math.BigDecimal通过java.math.MathContext支持特定精度的计算，任何涉及到金额的计算都应该使用decimal类型。<br />默认 Aviator 的计算精度为`MathContext.DECIMAL128`，可以自定义精度，通过：
```java
AviatorEvaluator.setOption(Options.MATH_CONTEXT, MathContext.DECIMAL64)
```
即可设置，更多关于decimal的精度问题请看java.math.BigDecimal的 javadoc 文档。
<a name="rUQ8j"></a>
### 强大的 seq 库
aviator 拥有强大的操作集合和数组的 seq 库。整个库风格类似函数式编程中的高阶函数。在 aviator 中，数组以及java.util.Collection下的子类都称为seq，可以直接利用 seq 库进行遍历、过滤和聚合等操作。<br />例如，假设有个 list:
```java
val env = new util.HashMap[String,Object]()
val list = new util.ArrayList[Integer]
list.add(3)
list.add(20)
list.add(10)
env.put("list", list)
var result = AviatorEvaluator.execute("count(list)", env)
println(result) // 3
result = AviatorEvaluator.execute("reduce(list,+,0)", env)
println(result) // 33
result = AviatorEvaluator.execute("filter(list,seq.gt(9))", env)
println(result) // [20, 10]
result = AviatorEvaluator.execute("include(list,10)", env)
println(result) // true
result = AviatorEvaluator.execute("sort(list)", env)
println(result) // [3, 10, 20]
AviatorEvaluator.execute("map(list,println)", env)
```
可以：<br />求长度: `count(list)`<br />求和: `reduce(list,+,0)`, `reduce`函数接收三个参数，第一个是seq，第二个是聚合的函数，如+等，第三个是聚合的初始值<br />过滤: `filter(list,seq.gt(9))`，过滤出list中所有大于9的元素并返回集合; `seq.gt`函数用于生成一个谓词，表示大于某个值<br />判断元素在不在集合里: `include(list,10)`<br />排序: `sort(list)`<br />遍历整个集合: `map(list,println)`，map接受的第二个函数将作用于集合中的每个元素，这里简单地调用`println`打印每个元素<br />其他还有：<br />`seq.some(list, pred)` 当集合中只要有一个元素满足谓词函数 pred 返回 true，立即返回 true，否则为 false。<br />`seq.every(list, pred)` 当集合里的每个元素都满足谓词函数 pred 返回 true，则结果为 true，否则返回 false。<br />`seq.not_any(list, pred)`，当集合里的每个元素都满足谓词函数 pred 返回 false，则结果为 true，否则返回 false。<br />以及 `seq.or(p1, p2, …)` 和 `seq.and(p1, p2, …)` 用于组合 `seq.gt`、`seq.lt` 等谓词函数。
<a name="LbHKV"></a>
### 两种运行模式
默认 `AviatorEvaluator` 以执行速度优先：
```java
AviatorEvaluator.setOption(Options.OPTIMIZE_LEVEL, AviatorEvaluator.EVAL)
```
可以修改为编译速度优先，这样不会做编译优化：
```java
AviatorEvaluator.setOption(Options.OPTIMIZE_LEVEL, AviatorEvaluator.COMPILE)
```
<a name="Pf9wj"></a>
### 调试信息
如果想查看每个表达式生成的字节码，可以通过打开 Trace 选项：
```java
AviatorEvaluator.setOption(Options.TRACE, true)
```
默认是打印到标准输出，可以改变输出指向：
```java
AviatorEvaluator.setTraceOutputStream(new FileOutputStream(new File("aviator.log")))
```
<a name="qot1g"></a>
## 5、语法手册
下面是 Aviator 详细的语法规则定义。
<a name="pWnbZ"></a>
### 数据类型

- Number类型：数字类型，支持四种类型，分别是long,double,java.math.BigInteger(简称 big int)和java.math.BigDecimal(简 称 decimal)，规则如下：
   - 任何以大写字母 N 结尾的整数都被认为是 big int
   - 任何以大写字母 M 结尾的数字都被认为是 decimal
   - 其他的任何整数都将被转换为 Long
   - 其他任何浮点数都将被转换为 Double
   - 超过 long 范围的整数字面量都将自动转换为 big int 类型、

其中 big int 和 decimal 是 2.3.0 版本开始引入的。数字还支持十六进制(以0x或者0X开头的数字)， 以及科学计数法，如1e-3等。不支持其他进制。<br />String类型: 字符串类型，单引号或者双引号括起来的文本串，如'hello world'， 变量如果传入的是String或者Character也将转为String类型<br />Bool类型: 常量true和false，表示真值和假值，与 java 的Boolean.TRUE和Boolean.False对应<br />Pattern类型: 正则表达式， 以//括起来的字符串，如/\d+/，内部 实现为java.util.Pattern<br />变量类型: 与 Java 的变量命名规则相同，变量的值由用户传入<br />nil类型: 常量nil，类似 java 中的null，但是nil比较特殊，nil不仅可以参与==、!=的比较， 也可以参与>、>=、<、<=的比较，Aviator 规定任何类型都大于nil除了nil本身，nil==nil返回true。用户传入的变量值如果为null，那么也将作为nil处理，nil打印为null
<a name="UTq5O"></a>
### 操作符
<a name="v5KcE"></a>
#### 算术运算符
Aviator 支持常见的算术运算符，包括+ - * / %五个二元运算符，和一元运算符-(负)。其中- * / %和一元的-仅能作用于Number类型。<br />+不仅能用于Number类型，还可以用于String的相加，或者字符串与其他对象的相加。<br />Aviator 规定，任何类型与String相加，结果为String。
<a name="sKU7T"></a>
#### 逻辑运算符
Avaitor 的支持的逻辑运算符包括，一元否定运算符!，以及逻辑与的&&，逻辑或的||。逻辑运算符的操作数只能为Boolean。<br />&&和||都执行短路规则。
<a name="iV7Ca"></a>
#### 关系运算符
Aviator 支持的关系运算符包括<， <=， >， >=以及==和!= 。<br />关系运算符可以作用于Number之间、String之间、Pattern之间、Boolean之间、变量之间以及其他类型与nil之间的关系比较， 不同类型除了nil之外不能相互比较。
<a name="SNStf"></a>
#### 位运算符
Aviator 支持所有的 Java 位运算符，包括`&`, `|`, `^`, `~`, `>>`, `<<`, `>>>`。
<a name="y4Lmg"></a>
#### 匹配运算符
匹配运算符=~用于String和Pattern的匹配，它的左操作数必须为String，右操作数必须为Pattern。匹配成功后，Pattern的分组将存于变量$num，num为分组索引。
<a name="qDARN"></a>
#### 三元运算符
Aviator 没有提供if else语句，但是提供了三元运算符`?:`，形式为`bool ? exp1: exp2`。其中bool必须为Boolean类型的表达式， 而exp1和exp2可以为任何合法的 Aviator 表达式，并且不要求exp1和exp2返回的结果类型一致。
<a name="nsZPp"></a>
## 6、内置函数
完整的内置函数列表参见内置函数。<br />[https://github.com/killme2008/aviatorscript/wiki/%E5%86%85%E7%BD%AE%E5%87%BD%E6%95%B0](https://github.com/killme2008/aviatorscript/wiki/%E5%86%85%E7%BD%AE%E5%87%BD%E6%95%B0)
<a name="hdw6w"></a>
## 7、选项列表
`AviatorEvaluator.setOption(opt, val)` 支持定义求值器的行为，完整的 Options 枚举选项参见完整选项说明<br />[https://github.com/killme2008/aviatorscript/wiki/%E5%AE%8C%E6%95%B4%E9%80%89%E9%A1%B9%E5%88%97%E8%A1%A8%E8%AF%B4%E6%98%8E](https://github.com/killme2008/aviatorscript/wiki/%E5%AE%8C%E6%95%B4%E9%80%89%E9%A1%B9%E5%88%97%E8%A1%A8%E8%AF%B4%E6%98%8E)
<a name="UFHcy"></a>
## 8、JavaDoc
[http://fnil.net/aviator/apidocs/](http://fnil.net/aviator/apidocs/)
<a name="Znjl4"></a>
## 9、4.0功能详解
[https://github.com/killme2008/aviatorscript/wiki/](https://github.com/killme2008/aviatorscript/wiki/)
<a name="s0ZdO"></a>
## 10、参考链接
主页：[http://fnil.net/aviator/](http://fnil.net/aviator/)<br />示例：<br />[https://github.com/killme2008/aviatorscript/tree/master/src/test/java/com/googlecode/aviator/example](https://github.com/killme2008/aviatorscript/tree/master/src/test/java/com/googlecode/aviator/example)<br />官方指南：[https://github.com/killme2008/aviatorscript/wiki](https://github.com/killme2008/aviatorscript/wiki)
