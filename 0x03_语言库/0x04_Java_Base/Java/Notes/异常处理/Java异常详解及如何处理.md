Java异常
<a name="vra5P"></a>
### 简介
程序运行时，发生的不被期望的事件，它阻止了程序按照程序员的预期正常执行，这就是异常。异常发生时，是任程序自生自灭，立刻退出终止，还是输出错误给用户？或者用C语言风格：用函数返回值作为执行状态？。<br />Java提供了更加优秀的解决办法：异常处理机制。<br />异常处理机制能让程序在异常发生时，按照代码的预先设定的异常处理逻辑，针对性地处理异常，让程序尽最大可能恢复正常并继续执行，且保持代码的清晰。<br />Java中的异常可以是函数中的语句执行时引发的，也可以是程序员通过throw 语句手动抛出的，只要在Java程序中产生了异常，就会用一个对应类型的异常对象来封装异常，JRE就会试图寻找异常处理程序来处理异常。<br />Throwable类是Java异常类型的顶层父类，一个对象只有是 Throwable 类的（直接或者间接）实例，他才是一个异常对象，才能被异常处理机制识别。JDK中内建了一些常用的异常类，也可以自定义异常。
<a name="C4xup"></a>
### Java异常的分类和类结构图
Java标准裤内建了一些通用的异常，这些类以Throwable为顶层父类。<br />Throwable又派生出Error类和Exception类。<br />错误：Error类以及他的子类的实例，代表了JVM本身的错误。错误不能被程序员通过代码处理，Error很少出现。因此，程序员应该关注Exception为父类的分支下的各种异常类。<br />异常：Exception以及他的子类，代表程序运行时发送的各种不期望发生的事件。可以被Java异常处理机制使用，是异常处理的核心。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657167532576-a165d241-1bec-4aef-bdc2-e49a5142212e.jpeg)

总体上根据Javac对异常的处理要求，将异常类分为2类。
<a name="yNevT"></a>
#### 非检查异常：
Error 和 RuntimeException 以及他们的子类。javac在编译时，不会提示和发现这样的异常，不要求在程序处理这些异常。所以如果愿意，可以编写代码处理（使用try…catch…finally）这样的异常，也可以不处理。对于这些异常，应该修正代码，而不是去通过异常处理器处理 。这样的异常发生的原因多半是代码写的有问题。如除0错误`ArithmeticException`，错误的强制类型转换错误`ClassCastException`，数组索引越界`ArrayIndexOutOfBoundsException`，使用了空对象`NullPointerException`等等。
<a name="IwS3v"></a>
#### 检查异常：
除了Error 和 RuntimeException的其它异常。javac强制要求程序员为这样的异常做预备处理工作（使用try…catch…finally或者throws）。在方法中要么用try-catch语句捕获它并处理，要么用throws子句声明抛出它，否则编译不会通过。这样的异常一般是由程序的运行环境导致的。因为程序可能被运行在各种未知的环境下，而程序员无法干预用户如何使用他编写的程序，于是程序员就应该为这样的异常时刻准备着。如SQLException , IOException,ClassNotFoundException 等。<br />需要明确的是：检查和非检查是对于javac来说的，这样就很好理解和区分了。
<a name="KB9E4"></a>
### 初识异常
下面的代码会演示2个异常类型：`ArithmeticException` 和 `InputMismatchException`。前者由于整数除0引发，后者是输入的数据不能被转换为int类型引发。
```java
package com.example;
import java. util .Scanner ;
public class AllDemo{
	public static void main (String [] args ){
		System . out. println( "----欢迎使用命令行除法计算器----" ) ;
		CMDCalculate ();
	}
	public static void CMDCalculate (){
		Scanner scan = new Scanner ( System. in );
		int num1 = scan .nextInt () ;
		int num2 = scan .nextInt () ;
		int result = devide (num1 , num2 ) ;
		System . out. println( "result:" + result) ;
		scan .close () ;
	}
	public static int devide (int num1, int num2 ){
		return num1 / num2 ;
	}
}
/*****************************************
----欢迎使用命令行除法计算器----
0
Exception in thread "main" java.lang.ArithmeticException : / by zero
     at com.example.AllDemo.devide( AllDemo.java:30 )
     at com.example.AllDemo.CMDCalculate( AllDemo.java:22 )
     at com.example.AllDemo.main( AllDemo.java:12 )
----欢迎使用命令行除法计算器----
r
Exception in thread "main" java.util.InputMismatchException
     at java.util.Scanner.throwFor( Scanner.java:864 )
     at java.util.Scanner.next( Scanner.java:1485 )
     at java.util.Scanner.nextInt( Scanner.java:2117 )
     at java.util.Scanner.nextInt( Scanner.java:2076 )
     at com.example.AllDemo.CMDCalculate( AllDemo.java:20 )
     at com.example.AllDemo.main( AllDemo.java:12 )
*****************************************/
```
异常是在执行某个函数时引发的，而函数又是层级调用，形成调用栈的，因为，只要一个函数发生了异常，那么他的所有的caller都会被异常影响。当这些被影响的函数以异常信息输出时，就形成的了异常追踪栈。<br />异常最先发生的地方，叫做异常抛出点。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657166937071-cd28a5b3-7e27-4cf3-bc6d-32b68e252dd1.png#clientId=u2d8243d4-2f4c-4&from=paste&id=uc6b264c8&originHeight=209&originWidth=845&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucf4da0a0-60d6-4e0e-aa34-02b23bd41c0&title=)<br />从上面的例子可以看出，当devide函数发生除0异常时，devide函数将抛出`ArithmeticException`异常，因此调用他的`CMDCalculate`函数也无法正常完成，因此也发送异常，而CMDCalculate的caller——main 因为`CMDCalculate`抛出异常，也发生了异常，这样一直向调用栈的栈底回溯。这种行为叫做异常的冒泡，异常的冒泡是为了在当前发生异常的函数或者这个函数的caller中找到最近的异常处理程序。由于这个例子中没有使用任何异常处理机制，因此异常最终由main函数抛给JRE，导致程序终止。<br />上面的代码不使用异常处理机制，也可以顺利编译，因为2个异常都是非检查异常。但是下面的例子就必须使用异常处理机制，因为异常是检查异常。<br />代码中选择使用throws声明异常，让函数的调用者去处理可能发生的异常。但是为什么只throws了IOException呢？因为`FileNotFoundException`是IOException的子类，在处理范围内。
```java
@Test
public void testException() throws IOException
{
    //FileInputStream的构造函数会抛出FileNotFoundException
    FileInputStream fileIn = new FileInputStream("E:\\a.txt");
    int word;
    //read方法会抛出IOException
    while((word =  fileIn.read())!=-1) 
    {
        System.out.print((char)word);
    }
    //close方法会抛出IOException
    fileIn.close()
}
```
<a name="iwwYb"></a>
### 异常处理的基本语法
在编写代码处理异常时，对于检查异常，有2种不同的处理方式：使用try…catch…finally语句块处理它。或者，在函数签名中使用throws 声明交给函数调用者caller去解决。<br />try…catch…finally语句块
```java
try{
     //try块中放可能发生异常的代码。
     //如果执行完try且不发生异常，则接着去执行finally块和finally后面的代码（如果有的话）。
     //如果发生异常，则尝试去匹配catch块。
}catch(SQLException SQLexception){
    //每一个catch块用于捕获并处理一个特定的异常，或者这异常类型的子类。Java7中可以将多个异常声明在一个catch中。
    //catch后面的括号定义了异常类型和异常参数。如果异常与之匹配且是最先匹配到的，则虚拟机将使用这个catch块来处理异常。
    //在catch块中可以使用这个块的异常参数来获取异常的相关信息。异常参数是这个catch块中的局部变量，其它块不能访问。
    //如果当前try块中发生的异常在后续的所有catch中都没捕获到，则先去执行finally，然后到这个函数的外部caller中去匹配异常处理器。
    //如果try中没有发生异常，则所有的catch块将被忽略。
}catch(Exception exception){
    //...
}finally{
    //finally块通常是可选的。
   //无论异常是否发生，异常是否匹配被处理，finally都会执行。
   //一个try至少要有一个catch块，否则， 至少要有1个finally块。但是finally不是用来处理异常的，finally不会捕获异常。
  //finally主要做一些清理工作，如流的关闭，数据库连接的关闭等。 
}
```
<a name="Htm8c"></a>
#### 需要注意的地方
1、try块中的局部变量和catch块中的局部变量（包括异常变量），以及finally中的局部变量，他们之间不可共享使用。<br />2、每一个catch块用于处理一个异常。异常匹配是按照catch块的顺序从上往下寻找的，只有第一个匹配的catch会得到执行。匹配时，不仅运行精确匹配，也支持父类匹配，因此，如果同一个try块下的多个catch异常类型有父子关系，应该将子类异常放在前面，父类异常放在后面，这样保证每个catch块都有存在的意义。<br />3、java中，异常处理的任务就是将执行控制流从异常发生的地方转移到能够处理这种异常的地方去。也就是说：当一个函数的某条语句发生异常时，这条语句的后面的语句不会再执行，它失去了焦点。执行流跳转到最近的匹配的异常处理catch代码块去执行，异常被处理完后，执行流会接着在“处理了这个异常的catch代码块”后面接着执行。<br />有的编程语言当异常被处理后，控制流会恢复到异常抛出点接着执行，这种策略叫做：resumption model of exception handling（恢复式异常处理模式 ）<br />而Java则是让执行流恢复到处理了异常的catch块后接着执行，这种策略叫做：termination model of exception handling（终结式异常处理模式）
```java
public static void main(String[] args){
	try {
		foo();
	}catch(ArithmeticException ae) {
		System.out.println("处理异常");
	}
}
public static void foo(){
	int a = 5/0;  //异常抛出点
	System.out.println("为什么还不给我涨工资!!!");  //////////////////////不会执行
}
```
<a name="AA7OX"></a>
#### `throws` 函数声明
`throws`声明：如果一个方法内部的代码会抛出检查异常（checked exception），而方法自己又没有完全处理掉，则javac保证必须在方法的签名上使用`throws`关键字声明这些可能抛出的异常，否则编译不通过。<br />`throws`是另一种处理异常的方式，它不同于try…catch…finally，`throws`仅仅是将函数中可能出现的异常向调用者声明，而自己则不具体处理。<br />采取这种异常处理的原因可能是：方法本身不知道如何处理这样的异常，或者说让调用者处理更好，调用者需要为可能发生的异常负责。
```java
public void foo() throws ExceptionType1 , ExceptionType2 ,ExceptionTypeN
{ 
     //foo内部可以抛出 ExceptionType1 , ExceptionType2 ,ExceptionTypeN 类的异常，或者他们的子类的异常对象。
}
```
<a name="MBhkA"></a>
### `finally`块
`finally`块不管异常是否发生，只要对应的try执行了，则它一定也执行。只有一种方法让`finally`块不执行：`System.exit()`。因此`finally`块通常用来做资源释放操作：关闭文件，关闭数据库连接等等。<br />良好的编程习惯是：在try块中打开资源，在`finally`块中清理释放这些资源。<br />**需要注意的地方:**<br />1、`finally`块没有处理异常的能力。处理异常的只能是catch块。<br />2、在同一try…catch…finally块中 ，如果try中抛出异常，且有匹配的catch块，则先执行catch块，再执行finally块。如果没有catch块匹配，则先执行finally，然后去外面的调用者中寻找合适的catch块。<br />3、在同一try…catch…finally块中 ，try发生异常，且匹配的catch块中处理异常时也抛出异常，那么后面的finally也会执行：首先执行finally块，然后去外围调用者中寻找合适的catch块。
<a name="oYf6K"></a>
### `throw` 异常抛出语句
`**throw exceptionObject**`<br />程序员也可以通过throw语句手动显式的抛出一个异常。`throw`语句的后面必须是一个异常对象。<br />`throw` 语句必须写在函数中，执行`throw` 语句的地方就是一个异常抛出点，它和由JRE自动形成的异常抛出点没有任何差别。
```java
public void save(User user)
{
      if(user  == null) 
          throw new IllegalArgumentException("User对象为空");
      //......
}
```
<a name="jdxEW"></a>
### 异常的链化
在一些大型的，模块化的软件开发中，一旦一个地方发生异常，则如骨牌效应一样，将导致一连串的异常。假设B模块完成自己的逻辑需要调用A模块的方法，如果A模块发生异常，则B也将不能完成而发生异常，但是B在抛出异常时，会将A的异常信息掩盖掉，这将使得异常的根源信息丢失。异常的链化可以将多个模块的异常串联起来，使得异常信息不会丢失。<br />异常链化:以一个异常对象为参数构造新的异常对象。新的异对象将包含先前异常的信息。这项技术主要是异常类的一个带Throwable参数的函数来实现的。这个当做参数的异常，叫他根源异常（cause）。<br />查看Throwable类源码，可以发现里面有一个Throwable字段cause，就是它保存了构造时传递的根源异常参数。这种设计和链表的结点类设计如出一辙，因此形成链也是自然的了。
```java
public class Throwable implements Serializable {
    private Throwable cause = this;
    public Throwable(String message, Throwable cause) {
        fillInStackTrace();
        detailMessage = message;
        this.cause = cause;
    }
     public Throwable(Throwable cause) {
        fillInStackTrace();
        detailMessage = (cause==null ? null : cause.toString());
        this.cause = cause;
    }
    //........
}
```
下面是一个例子，演示了异常的链化：从命令行输入2个int，将他们相加，输出。输入的数不是int，则导致`getInputNumbers`异常，从而导致add函数异常，则可以在add函数中抛出<br />一个链化的异常。
```java
public static void main(String[] args)
{
    System.out.println("请输入2个加数");
    int result;
    try
    {
        result = add();
        System.out.println("结果:"+result);
    } catch (Exception e){
        e.printStackTrace();
    }
}
//获取输入的2个整数返回
private static List<Integer> getInputNumbers()
{
    List<Integer> nums = new ArrayList<>();
    Scanner scan = new Scanner(System.in);
    try {
        int num1 = scan.nextInt();
        int num2 = scan.nextInt();
        nums.add(new Integer(num1));
        nums.add(new Integer(num2));
    }catch(InputMismatchException immExp){
        throw immExp;
    }finally {
        scan.close();
    }
    return nums;
}
//执行加法计算
private static int add() throws Exception
{
    int result;
    try {
        List<Integer> nums =getInputNumbers();
        result = nums.get(0)  + nums.get(1);
    }catch(InputMismatchException immExp){
        throw new Exception("计算失败",immExp);  /////////////////////////////链化:以一个异常对象为参数构造新的异常对象。
    }
    return  result;
}
/*
请输入2个加数
r 1
java.lang.Exception: 计算失败
    at practise.ExceptionTest.add(ExceptionTest.java:53)
    at practise.ExceptionTest.main(ExceptionTest.java:18)
Caused by: java.util.InputMismatchException
    at java.util.Scanner.throwFor(Scanner.java:864)
    at java.util.Scanner.next(Scanner.java:1485)
    at java.util.Scanner.nextInt(Scanner.java:2117)
    at java.util.Scanner.nextInt(Scanner.java:2076)
    at practise.ExceptionTest.getInputNumbers(ExceptionTest.java:30)
    at practise.ExceptionTest.add(ExceptionTest.java:48)
    ... 1 more
*/
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657166937036-3f8efb1d-c083-482f-b2f0-398fe860fa3a.png#clientId=u2d8243d4-2f4c-4&from=paste&id=u6905e207&originHeight=135&originWidth=361&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u624eb1d5-7696-47a6-bd8d-3052a52aa78&title=)
<a name="o8Hp0"></a>
### 自定义异常
如果要自定义异常类，则扩展Exception类即可，因此这样的自定义异常都属于检查异常（checked exception）。如果要自定义非检查异常，则扩展自RuntimeException。<br />按照国际惯例，自定义的异常应该总是包含如下的构造函数：

- 一个无参构造函数
- 一个带有String参数的构造函数，并传递给父类的构造函数。
- 一个带有String参数和Throwable参数，并都传递给父类构造函数
- 一个带有Throwable 参数的构造函数，并传递给父类的构造函数。

下面是IOException类的完整源代码，可以借鉴。
```java
public class IOException extends Exception{
    static final long serialVersionUID = 7818375828146090155L;
    public IOException()
    {
        super();
    }
    public IOException(String message)
    {
        super(message);
    }
    public IOException(String message, Throwable cause)
    {
        super(message, cause);
    }
    public IOException(Throwable cause)
    {
        super(cause);
    }
}
```
<a name="FZ1Hi"></a>
### 异常的注意事项
1、当子类重写父类的带有 throws声明的函数时，其throws声明的异常必须在父类异常的可控范围内——用于处理父类的throws方法的异常处理器，必须也适用于子类的这个带throws方法 。这是为了支持多态。<br />例如，父类方法throws 的是2个异常，子类就不能throws 3个及以上的异常。父类throws IOException，子类就必须`throws IOException`或者`IOException`的子类。<br />至于为什么？也许下面的例子可以说明。
```java
class Father{
    public void start() throws IOException
    {
        throw new IOException();
    }
}
class Son extends Father{
    public void start() throws Exception
    {
        throw new SQLException();
    }
}
/**********************假设上面的代码是允许的（实质是错误的）***********************/
class Test{
    public static void main(String[] args){
        Father[] objs = new Father[2];
        objs[0] = new Father();
        objs[1] = new Son();
        for(Father obj:objs)
        {
        //因为Son类抛出的实质是SQLException，而IOException无法处理它。
        //那么这里的try。。catch就不能处理Son中的异常。
        //多态就不能实现了。
            try {
                 obj.start();
            }catch(IOException)
            {
                 //处理IOException
            }
         }
   }
}
```
2、Java程序可以是多线程的。每一个线程都是一个独立的执行流，独立的函数调用栈。如果程序只有一个线程，那么没有被任何代码处理的异常 会导致程序终止。如果是多线程的，那么没有被任何代码处理的异常仅仅会导致异常所在的线程结束。<br />也就是说，Java中的异常是线程独立的，线程的问题应该由线程自己来解决，而不要委托到外部，也不会直接影响到其它线程的执行。
<a name="Wq1fl"></a>
### `finally`块和`return`
```java
public static void main(String[] args){
    int re = bar();
    System.out.println(re);
}
private static int bar() {
    try{
        return 5;
    } finally{
        System.out.println("finally");
    }
}
/*输出：
finally
*/
```
很多人面对这个问题时，总是在归纳执行的顺序和规律，不过还是很难理解。总结了一个方法。用如下GIF图说明。<br />![](https://cdn.nlark.com/yuque/0/2022/gif/396745/1657166937213-e677cd10-09cb-4e17-9f6d-6b4ec1f9d613.gif#clientId=u2d8243d4-2f4c-4&from=paste&id=uf2a8b6db&originHeight=455&originWidth=550&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u67c4d1e0-dace-449d-84d4-c7982999c6d&title=)<br />也就是说：try…catch…finally中的`return` 只要能执行，就都执行了，他们共同向同一个内存地址（假设地址是0x80）写入返回值，后执行的将覆盖先执行的数据，而真正被调用者取的返回值就是最后一次写入的。那么，按照这个思想，下面的这个例子也就不难理解了。
<a name="Ixpws"></a>
#### `finally`中的`return`会覆盖 try 或者catch中的返回值。
```java
public static void main(String[] args){
	int result;
	result  =  foo();
	System.out.println(result);     /////////2
	result = bar();
	System.out.println(result);    /////////2
}

@SuppressWarnings("finally")
public static int foo(){
	trz{
		int a = 5 / 0;
	} catch (Exception e){
		return 1;
	} finally{
		return 2;
	}
}
@SuppressWarnings("finally")
public static int bar()
{
	try {
		return 1;
	}finally {
		return 2;
	}
}
```
<a name="p0JEL"></a>
#### `finally`中的`return`会抑制（消灭）前面try或者catch块中的异常
```java
class TestException{
    public static void main(String[] args){
        int result;
        try{
            result = foo();
            System.out.println(result);           //输出100
        } catch (Exception e){
            System.out.println(e.getMessage());    //没有捕获到异常
        }
        try{
            result  = bar();
            System.out.println(result);           //输出100
        } catch (Exception e){
            System.out.println(e.getMessage());    //没有捕获到异常
        }
    }
    //catch中的异常被抑制
    @SuppressWarnings("finally")
    public static int foo() throws Exception
    {
        try {
            int a = 5/0;
            return 1;
        }catch(ArithmeticException amExp) {
            throw new Exception("我将被忽略，因为下面的finally中使用了return");
        }finally {
            return 100;
        }
    }
    //try中的异常被抑制
    @SuppressWarnings("finally")
    public static int bar() throws Exception
    {
        try {
            int a = 5/0;
            return 1;
        }finally {
            return 100;
        }
    }
}
```
<a name="YCqX5"></a>
#### `finally`中的异常会覆盖（消灭）前面try或者catch中的异常
```java
class TestException{
	public static void main(String[] args){
		int result;
		try{
			result = foo();
		} catch (Exception e){
			System.out.println(e.getMessage());    //输出：我是finaly中的Exception
		}
		try{
			result  = bar();
		} catch (Exception e){
			System.out.println(e.getMessage());    //输出：我是finaly中的Exception
		}
	}
	//catch中的异常被抑制
	@SuppressWarnings("finally")
	public static int foo() throws Exception
	{
		try {
			int a = 5/0;
			return 1;
		}catch(ArithmeticException amExp) {
			throw new Exception("我将被忽略，因为下面的finally中抛出了新的异常");
		}finally {
			throw new Exception("我是finaly中的Exception");
		}
	}
	//try中的异常被抑制
	@SuppressWarnings("finally")
	public static int bar() throws Exception
	{
		try {
			int a = 5/0;
			return 1;
		}finally {
			throw new Exception("我是finaly中的Exception");
		}
	}
}
```
上面的3个例子都异于常人的编码思维，因此建议：

- 不要在`fianlly`中使用`return`。
- 不要在`finally`中抛出异常。
- 减轻`finally`的任务，不要在`finally`中做一些其它的事情，`finally`块仅仅用来释放资源是最合适的。
- 将尽量将所有的`return`写在函数的最后面，而不是`try … catch … finally`中。
