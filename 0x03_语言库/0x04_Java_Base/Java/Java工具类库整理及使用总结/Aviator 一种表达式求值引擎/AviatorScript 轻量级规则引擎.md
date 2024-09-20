Java
<a name="LRFlQ"></a>
# 简介
AviatorScript 是一门高性能、轻量级寄宿于 JVM （包括 Android 平台）之上的脚本语言。<br />它起源于2010年，作者对当时已有的一些产品不是很满意，所以自己撸了一个，它是Groovy的一个定制化的子集。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691112402019-72dcd194-f265-4a76-852a-7fe3f786ce90.png#averageHue=%23b4cfda&clientId=uc2e474ce-320a-4&from=paste&id=u042eb2d0&originHeight=171&originWidth=477&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u263875c0-1e8e-47f0-b0d0-ac0c2fd933b&title=)<br />相比较一些传统的规则引擎，比如Drools、Jess、JRules，它更加轻量级，而且性能更好，同时能力开放，扩展很方便。<br />来看（吹）看（吹）AviatorScript的特点：

1. 它支持数字、字符串、正则表达式、布尔值等基本类型，并且可以使用所有 Java 运算符进行运算。
2. 还有一个内置的东西叫做 bigint 和 decimal，可以处理超大整数和高精度运算。而且还可以通过运算符重载让它们使用普通的算术运算符 +-*/。
3. 语法非常齐全，可以用它来写多行数据、条件语句、循环语句，还能处理词法作用域和异常处理等等。
4. 如果喜欢函数式编程，还有一个叫做 Sequence 抽象的东西，可以更方便地处理集合。
5. 还有一个轻量化的模块系统，方便组织代码。
6. 如果需要调用 Java 方法，也没问题，可以用多种方式方便地调用 Java 方法，还有一个完整的脚本 API可以从 Java 调用脚本。
7. 性能也是超出想象的好，如果使用 ASM 模式，它会直接将脚本翻译成 JVM 字节码，解释模式还可以在 Android 等非标准 Java 平台上运行。

AviatorScript可以用在各种场景，比如规则判断和规则引擎、公式计算、动态脚本控制，甚至集合数据 ELT 等等。可以说相当全能了。
<a name="NFpIu"></a>
# 快速开始
AviatorScript 是一门寄生在 JVM (Hosted on the JVM）上的语言，类似 clojure/scala/kotlin 等等，从写个Hello World开始。

- 创建一个SpringBoot项目，引入依赖，这里选择的是最新版本
```xml
<dependency>
  <groupId>com.googlecode.aviator</groupId>
  <artifactId>aviator</artifactId>
  <version>5.3.3</version>
</dependency>
```
PS:可以看到aviator的groupId有一个googlecode，但是它和Google可没什么关系，这是因为早期aviator托管在Google的一个开源项目托管平台Google Code。

- 在项目的resource目录下创建一个目录script，在script目录下创建脚本hello.av
```
println("Hello, AviatorScript!");
```

- 编写一个单元测试，运行脚本
```java
@Test
void testHello() throws Exception {
    //获取路径
    ClassPathResource resource = new ClassPathResource("script/hello.av");
    String scriptPath = resource.getPath();
    //编译
    Expression exp = AviatorEvaluator.getInstance().compileScript(scriptPath);
    //执行
    exp.execute();
}
```
最后执行一下，就可以看到输出：
```
Hello, AviatorScript!
```

- 也可以直接把脚本定义成字符串，用`compile()`来进行编译
```java
@Test
void testHelloStr() throws Exception {
    //定义脚本
    String script="println(\"Hello, AviatorScript!\");";
    //编译
    Expression exp = AviatorEvaluator.getInstance().compile(script);
    //执行
    exp.execute();
}
```
AviatorScript有一个Idea插件，支持直接编译运行Aviator脚本，比较方便。<br />![Aviator插件](https://cdn.nlark.com/yuque/0/2023/png/396745/1691112401974-367ef3fb-5699-48ec-8ebf-d1c8212c31c1.png#averageHue=%233b4453&clientId=uc2e474ce-320a-4&from=paste&id=u06d6ca8b&originHeight=549&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u71144c08-5df8-4334-8708-406fc7cd0fb&title=Aviator%E6%8F%92%E4%BB%B6 "Aviator插件")<br />但不足之处，这个插件已经不怎么维护了，只兼容到了Idea2021版本。<br />![Idea插件](https://cdn.nlark.com/yuque/0/2023/png/396745/1691112402026-e72276a7-1961-424e-ba35-9a33471c0bb2.png#averageHue=%23fefdfd&clientId=uc2e474ce-320a-4&from=paste&id=ua0a6e3c9&originHeight=205&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ue0df334f-4c9d-4db1-8e04-c4fc2e47846&title=Idea%E6%8F%92%E4%BB%B6 "Idea插件")<br />AviatorScript脚本的运行，分为两步，编译和执行。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1691112681513-21f686dc-e729-447d-b98a-79e68b9637a1.jpeg)<br />编译支持编译脚本文件和脚本文本，分别使用`compileScript`和`compile`方法。<br />编译产生的  Expression 对象，最终都是调用 `execute()` 方法执行。<br />这里有个重要能力，`execute` 方法可以接受一个变量列表组成的 map，来注入执行的上下文：
```java
String expression = "a-(b-c) > 100";
Expression compiledExp = AviatorEvaluator.compile(expression);
//上下文
double a=100.3,b=45,c= -199.100;
Map<String, Object> context=new HashMap<>();
context.put("a",a);
context.put("b",b);
context.put("c",c);
//通过注入的上下文执行
Boolean result = (Boolean) compiledExp.execute(context);
System.out.println(result);
```
实现一些规则的判断就是基于这个能力，把一些参数下上下文传进去，然后进行逻辑判断。
<a name="arWTP"></a>
# 基本语法
在来看看AviatorScript的基本语法，它的语法相当简洁，比较接近于数学表达式的形式。
<a name="oubLI"></a>
## 基本类型及运算
AviatorScript 支持常见的类型，如数字、布尔值、字符串等等，同时将大整数、BigDecimal、正则表达式也作为一种基本类型来支持。
<a name="lFyPI"></a>
### 数字
AviatorScript 支持数字类型，包括整数和浮点数，以及高精度计算（BigDecimal）。数字类型可以进行各种算术运算。
<a name="AJYlA"></a>
#### 整数和算术运算
整数类型，对应Java中的long类型，可以表示范围为 -9223372036854774808 ~ 9223372036854774807 的整数。整数可以使用十进制或十六进制表示。
```
let a = 99;
let b = 0xFF;
let c = -99;

println(a + b); // 270
println(a / b); // 0
println(a - b + c); // -156
println(a + b * c); // -9801
println(a - (b - c)); // 198
println(a / b * b + a % b); // 99
```
整数可以进行加减乘除和取模运算。需要注意的是，整数相除的结果仍然是整数，遵循整数运算规则。可以使用括号来指定运算的优先级。
<a name="RwkXE"></a>
#### 浮点数
浮点数类型对应Java中的double类型，表示双精度 64 位浮点数。浮点数可以使用十进制或科学计数法表示。
```
let a = 1.34159265;
let b = 0.33333;
let c = 1e-2;

println(a + b); // 1.67492265
println(a - b); // 1.00826265
println(a * b); // 0.4471865500145
println(a / b); // 4.0257402772554
println(a + c); // 1.35159265
```
浮点数可以进行加减乘除运算，结果仍然为浮点数。
<a name="znqXv"></a>
#### 高精度计算(Decimal)
高精度计算使用 BigDecimal 类型，可以进行精确的数值计算，适用于货币运算或者物理公式运算的场景。可以通过在数字后面添加 "M" 后缀来表示 BigDecimal 类型。
```
let a = 1.34M;
let b = 0.333M;
let c = 2e-3M;

println(a + b); // 1.673M
println(a - b); // 1.007M
println(a * b); // 0.44622M
println(a / b); // 4.022022022M
println(a + c); // 1.342M
```
BigDecimal 类型可以进行加减乘除运算，结果仍然为 BigDecimal 类型。默认的运算精度是 MathContext.DECIMAL128，可以通过修改引擎配置项 `Options.MATH_CONTEXT` 来改变。
<a name="IsLht"></a>
#### 数字类型转换
数字类型在运算时会自动进行类型转换：

- 单一类型参与的运算，结果仍然为该类型。
- 多种类型参与的运算，按照 long -> bigint -> decimal -> double 的顺序自动提升，结果为提升后的类型。

可以使用 long(x) 函数将数字强制转换为 long 类型，使用 double(x) 函数将数字强制转换为 double 类型。
```
let a = 1;
let b = 2;

println("a/b is " + a/b); // 0
println("a/double(b) is " + a/double(b)); // 0.5
```
a 和 b 都是 long 类型，它们相除的结果仍然是整数。使用 double(b) 将 b 转换为 double 类型后，相除的结果为浮点数。
<a name="zyPRj"></a>
### 字符串
字符串类型由单引号或双引号括起来的连续字符组成。可以使用 println 函数来打印字符串。
```
let a = "hello world";
println(a); // hello world
```
字符串的长度可以通过 string.length 函数获取。
```
let a = "hello world";
println(string.length(a)); // 11
```
字符串可以通过 + 运算符进行拼接。
```
let a = "hello world";
let b = "AviatorScript";
println(a + ", " + b + "!" + 5); // hello world, AviatorScript!5
```
字符串还包括其他函数，如截取字符串 substring，都在 string 这个 namespace 下，具体见函数库列表。
<a name="MwnWH"></a>
### 布尔类型和逻辑运算
布尔类型用于表示真和假，它只有两个值 true 和 false 分别表示真值和假值。<br />比较运算如大于、小于可以产生布尔值：
```
println("3 > 1 is " + (3 > 1));  // 3 > 1 is true
println("3 >= 1 is " + (3 >= 1));  // 3 >= 1 is true
println("3 >= 3 is " + (3 >= 3));  // 3 >= 3 is true
println("3 < 1 is " + (3 < 1));  // 3 < 1 is false
println("3 <= 1 is " + (3 <= 1));  // 3 <= 1 is false
println("3 <= 3 is " + (3 <= 3));  // 3 <= 3 is true
println("3 == 1 is " + (3 == 1));  // 3 == 1 is false
println("3 != 1 is " + (3 != 1));  // 3 != 1 is true
```
上面演示了所有的逻辑运算符：

- > 大于
- >= 大于等于
- < 小于
- <= 小于等于
- == 等于
- != 不等于
<a name="iVXE1"></a>
## 基本语法
AviatorScript也支持条件语句和循环语句。
<a name="GYTxI"></a>
### 条件语句
AviatorScript 中的条件语句和其他语言没有太大区别：

- if
```
if(true) {
   println("in if body");
}
```

- if-else
```
if(false){
   println("in if body");
} else {
   println("in else body");
}
```

- if-elsif-else
```
let a = rand(1100);

if(a > 1000) {
  println("a is greater than 1000.");
} elsif (a > 100) {
  println("a is greater than 100.");
} elsif (a > 10) {
   println("a is greater than 10.");
} else {
   println("a is less than 10 ");
}
```
<a name="zLwS2"></a>
### 循环语句
AviatorScript提供了两种循环语句：for和while。
<a name="EkxrN"></a>
#### for循环：遍历集合
for ... in 语句通常用于遍历一个集合，例如下面是遍历 0 到 9 的数字
```
for i in range(0, 10) {
  println(i);
}
```
在这里，`range(start, end)` 函数用于创建一个整数集合，包括起始值 start，但不包括结束值 end。在循环迭代过程中，变量 i 绑定到集合中的每个元素，并执行大括号 {...} 中的代码块。<br />range 函数还可以接受第三个参数，表示递增的步长大小（默认步长为 1）。例如，可以打印出0到9之间的偶数：
```
for i in range(0, 10, 2) {
  println(i);
}
```
for .. in 可以用于任何集合结构，比如数组、 java.util.List 、 java.util.Map 等等。
<a name="lbxYZ"></a>
#### while循环
while 循环本质上是将条件语句与循环结合在一起。当条件为真时，不断执行一段代码块，直到条件变为假。<br />例如，下面的示例中，变量 sum 从 1 开始，不断累加自身，直到超过 1000 才停止，然后进行打印输出：
```
let sum = 1;

while sum < 1000 {
  sum = sum + sum;
}

println(sum);
```
循环可以用这三个关键字结束——**continue/break/return**：

- continue用于跳过当前迭代，继续下一次迭代。
- break用于跳出整个循环。
- return用于中断整个脚本（或函数）的执行并返回。
<a name="KQ8CU"></a>
## 函数
再来看看AviatorScript一个非常重要的特性——函数。
<a name="rL2Fp"></a>
### 函数
<a name="Xef93"></a>
#### 函数定义和调用
AviatorScript中使用fn语法来定义函数：
```
fn add(x, y) {
  return x + y;
}

three = add(1, 2);
println(three);  // 输出：3

s = add('hello', ' world');
println(s);  // 输出：hello world
```
这里通过fn关键字来定义了一个函数，函数名为add，它接受两个参数x和y，并返回它们的和。<br />需要注意的是，AviatorScript是动态类型系统，不需要定义参数和返回值的类型，它会根据实际传入和返回的值进行自动类型转换。因此，可以使用字符串来调用add函数。<br />函数的返回值可以通过return语句来指定，也可以省略不写。在函数体内，如果没有明确的return语句，最后一个表达式的值将被作为返回值。
<a name="wBDCL"></a>
#### 自定义函数
再来给大家介绍一个AviatorScript里非常好的特性，支持自定义函数，这给AviatorScript带来了非常强的扩展性。<br />可以通过 java 代码实现并往引擎中注入自定义函数，在 AviatorScript 中就可以使用，事实上所有的内置函数也是通过同样的方式实现的：
```java
public class TestAviator {

    public static void main(String[] args) {
        //通通创建一个AviatorEvaluator的实例
        AviatorEvaluatorInstance instance = AviatorEvaluator.getInstance();
        //注册函数
        instance.addFunction(new AddFunction());
        //执行ab脚本,脚本里调用自定义函数
        Double result= (Double) instance.execute("add(1, 2)");
        //输出结果
        System.out.println(result);
    }
}

/**
 * 实现AbstractFunction接口，就可以自定义函数
 */
class AddFunction extends AbstractFunction {

    /**
     * 函数调用
     * @param env 当前执行的上下文
     * @param arg1 第一个参数
     * @param arg2 第二个参数
     * @return 函数返回值
     */
    @Override
    public AviatorObject call(Map<String, Object> env,
                              AviatorObject arg1, AviatorObject arg2) {
        Number left = FunctionUtils.getNumberValue(arg1, env);
        Number right = FunctionUtils.getNumberValue(arg2, env);
        //将两个参数进行相加
        return new AviatorDouble(left.doubleValue() + right.doubleValue());
    }

    /**
     * 函数的名称
     * @return 函数名
     */
    public String getName() {
        return "add";
    }
}
```
可以看到：

- 继承`AbstractFunction`类，就可以自定义一个函数
- 重写`call`方法，就可以定义函数的逻辑，可以通过`FunctionUtils`获取脚本传递的参数
- 通过`getName`可以设置函数的名称
- 通过`addFunction`添加一个自定义函数类的实例，就可以注册函数
- 最后就可以在`Aviator`的脚本里编译执行自定义的函数

好了，关于AviatorScript的语法就不过多介绍了，可以直接查看[官方文档](https://www.yuque.com/boyan-avfmj/aviatorscript/cpow90)，可读性相当不错。<br />接下来就来看看AviatorScript的实际应用，看看它到底怎么提升项目的灵活性。
<a name="Yxx5e"></a>
# 实战案例
标题带了规则引擎，在项目里也主要是拿AviatorScript作为规则引擎使用——可以把AviatorScript的脚本维护在配置中心或者数据库，进行动态地维护，这样一来，一些规则的修改，就不用大动干戈地去修改代码，这样就更加方便和灵活了。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1691112992477-3ff51e0c-c1b9-4282-898e-6cd62a02cca0.jpeg)
<a name="E3Ql2"></a>
## 客户端版本控制
在日常的开发中，很多时候可能面临这样的情况，兼容客户端的版本，尤其是Android和iPhone，有些功能是低版本不支持的，或者说有些功能到了高版本就废弃掉，这时候如果硬编码去兼容就很麻烦，那么就可以考虑使用规则脚本的方式。

- 自定义版本比较函数：AviatorScript没有内置版本比较函数，但是可以利用它的自定义函数特性，自己定义一个版本比较函数
```java
class VersionFunction extends AbstractFunction {

    @Override
    public String getName() {
        return "compareVersion";
    }

    @Override
    public AviatorObject call(Map<String, Object> env, AviatorObject arg1, AviatorObject arg2) {
        // 获取版本
        String version1 = FunctionUtils.getStringValue(arg1, env);
        String version2 = FunctionUtils.getStringValue(arg2, env);
        int n = version1.length(), m = version2.length();
        int i = 0, j = 0;
        while (i < n || j < m) {
            int x = 0;
            for (; i < n && version1.charAt(i) != '.'; ++i) {
                x = x * 10 + version1.charAt(i) - '0';
            }
            ++i; // 跳过点号
            int y = 0;
            for (; j < m && version2.charAt(j) != '.'; ++j) {
                y = y * 10 + version2.charAt(j) - '0';
            }
            ++j; // 跳过点号
            if (x != y) {
                return x > y ? new AviatorBigInt(1) : new AviatorBigInt(-1);
            }
        }
        return new AviatorBigInt(0);
    }
}
```

- 注册自定义函数：为了方便使用各种自定义函数，一般定义一个单例的`AviatorEvaluatorInstance`，把它注册成Bean
```java
@Bean
public AviatorEvaluatorInstance aviatorEvaluatorInstance() {
    AviatorEvaluatorInstance instance = AviatorEvaluator.getInstance();
    // 默认开启缓存
    instance.setCachedExpressionByDefault(true);
    // 使用LRU缓存，最大值为100个。
    instance.useLRUExpressionCache(100);
    // 注册内置函数，版本比较函数。
    instance.addFunction(new VersionFunction());
}
```

- 在代码里传递上下文：接下来，就可以在业务代码里将一些参数放进执行上下文，然然后进行编译执行，注意编译的时候最好要开启缓存，这样效率会高很多
```java
/**
 * 
 * @param device 设备
 * @param version 版本
 * @param rule 规则脚本
 * @return
 */
public boolean filter(String device,String version,String rule){
    // 执行参数
    Map<String, Object> env = new HashMap<>();
    env.put("device", device);
    //编译脚本
    Expression expression = aviatorEvaluatorInstance.compile(DigestUtils.md5DigestAsHex(rule.getBytes()), rule, true);
    //执行脚本
    boolean isMatch = (boolean) expression.execute(env);
    return isMatch;
}
```

- 编写脚本：接下来就可以编写和维护对应的规则脚本，这些规则脚本通常放在在配置中心或者数据库，方便进行动态变更
```
if(device==bil){
    return false;
}

## 控制android的版本
if (device=="Android" && compareVersion(version,"1.38.1")<0){
    return false;
}

return true;
```
这样一来，假如某天，客户端Bug或者产品原因，需要修改客户端和客户端的版本控制，直接修改脚本就好了。<br />甚至可以在env里放进更多参数，比如uid，可以实现简单的黑白名单。<br />自定义函数除了这种简单的比较版本，还可以放一些复杂的逻辑，比如判断是否新用户等等。
<a name="XTw5J"></a>
## 营销活动规则
假如现在运营希望进行一场营销活动，对用户进行一定的支付优惠，最开始的一版活动规则：

- 满1000减200，满500减100

这个好写，一顿if-else就完事了。<br />但是没过几天，又改了活动规则：

- 首单用户统一减20

好，啪啪改代码。<br />又过去几天，活动规则又改了：

- 随机优惠不同金额

为了一些多变的营销规则，大动干戈，不停修改代码，耗时费力，那么不如用规则脚本实现：

- 定义脚本
```
if (amount>=100){
    return 200;
}elsif(amount>=500){
    return 100;
}else{
    return 0;
}
```

- 业务代码调用
```java
public BigDecimal getDiscount(BigDecimal amount,String rule){
    // 执行规则并计算最终价格
    Map<String, Object> env = new HashMap<>();
    env.put("amount", amount);
    Expression expression = aviatorEvaluatorInstance.compile(DigestUtils.md5DigestAsHex(rule.getBytes()), rule, true);
    return  (BigDecimal) expression.execute();
}
```
接下来，再发生营销规则变更，就可以少量开发（自定义函数，比如判断首单用户），并且可以组件化地维护营销规则。
<a name="rLWZ1"></a>
## 订单风控规则
Aviator在订单风控里应用也很香，风控的规则调整是相当频繁的，比如一个电商网站，常常要根据交易的争议率、交易表现等等，来收紧和放松风控规则，这就要求能对一风控规则进行快速地配置变更。<br />例如，根据订单金额、客户评级、收货地址等属性，自动判断是否有风险并触发相应的风控操作。

- 规则脚本
```
if (amount>=1000 || rating <= 2){
    return "High";
}elsif(amount >= 500 || rating<=4){
    return "Mid";
}else{
    return "Low";
}
```

- 代码调用：这里只是简单返回了一个风控等级，其实可以通过Map的方式返回多个参数。
```java
public String riskLevel(BigDecimal amount,String rating,String rule){
    // 执行规则并计算最终价格
    Map<String, Object> env = new HashMap<>();
    env.put("amount", amount);
    env.put("rating", rating);
    Expression expression = aviatorEvaluatorInstance.compile(DigestUtils.md5DigestAsHex(rule.getBytes()), rule, true);
    return  (String) expression.execute();
}
```
上面随手列出了几个简单的例子，AviatorScript 还可以用在一些审批流程、事件处理、数据质量管理等等场景……<br />在一些轻量级的需要规则引擎的场景下，AviatorScript 真的太香了，尤其是它的扩展性，支持通过Java自定义函数，甚至可以在脚本里查询数据库、查询Redis、调用外部接口……这样就可以像搭积木一样搭建想要的功能。
<a name="tkUXE"></a>
# 总结
分享了一款轻量级的规则脚本语言AviatorScript，它的语法丰富，但是很轻量，并且支持非常灵活的扩展，在项目中使用可以有效提高业务的灵活性，降低开发的工作量。
<a name="l3gQe"></a>
## 参考
[https://www.yuque.com/boyan-avfmj/aviatorscript/cpow90](https://www.yuque.com/boyan-avfmj/aviatorscript/cpow90)<br />[https://github.com/killme2008/aviatorscript](https://github.com/killme2008/aviatorscript)
