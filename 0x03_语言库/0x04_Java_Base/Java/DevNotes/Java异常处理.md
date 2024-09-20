Java 异常处理
<a name="ea5lZ"></a>
### 线上代码不要使用`printStackTrace()`
写完代码后请一定要检查下，代码中千万不要有`printStackTrace()`。因为`printStackTrace()`只会在控制台上输出错误的堆栈信息，他只适合于用来代码调试。<br />真正需要记录异常，请使用日志记录。
<a name="X3xTp"></a>
### 永远不要在catch块中吞掉异常
```java
catch (NoSuchMethodException e) {
    return null;
}
```
永远不要不处理异常，而是返回null，这样异常就会被吞掉，无法获取到任何失败信息，会给日后的问题排查带来巨大困难。
<a name="g2MAx"></a>
### 在需要的地方声明特定的受检异常
```java
public void foo() throws Exception { 
    //错误做法
}
```
一定要尽量避免上面的代码，因为他的调用者完全不知道错误的原因到底是什么。<br />在方法声明中，可以由方法抛出一些特定受检异常。如果有多个，那就分别抛出多个，这样这个方法的使用者才会分别针对每个异常做特定的处理，从而避免发生故障。
```java
public void foo() throws SpecificException1, SpecificException2 { 
    //正确做法
}
### 始终只捕获那些可以处理的异常
catch (NoSuchMethodException e) {
    throw e; //这代码一点用都没有
}
```
这是一个基本概念，当能够处理他时再捕获这个异常，否则永远都不要捕获他。<br />如果不能在catch块中处理它，那么就不要先捕获再直接抛出，这样没有任何意义。
<a name="2gxjn"></a>
### 尽量捕获特定的子类，而不是直接捕获Exception类
```java
try {
    someMethod();
} catch (Exception e) {
    LOGGER.error("method has failed", e);
}
```
以上代码，最大的问题就是，如果someMethod()的开发者在里面新增了一个特定的异常，并且预期是调用方能够特殊的对他进行处理。<br />但是调用者直接catch了Exception类，就会导致永远无法知道someMethod的具体变化细节。这久可能导致在运行的过程中在某一个时间点程序崩溃。
<a name="vuJ6K"></a>
### 永远不要捕获Throwable类
这是一个严重的问题，因为Java中的Error也可以是Throwable的子类。但是Error是Java虚拟机本身无法控制的。Java虚拟机甚至可能不会在出现任何错误时请求用户的catch子句。
<a name="uBBIa"></a>
### 始终在自定义异常中覆盖原有的异常，这样堆栈跟踪就不会丢失
```java
catch (NoSuchMethodException e) {
    throw new MyServiceException("Some information: " + e.getMessage());  //错误做法
}
```
上面的命令可能会丢失掉主异常的堆栈跟踪。正确的方法是:
```java
catch (NoSuchMethodException e) {
     throw new MyServiceException("Some information: " , e);  //正确做法
}
```
<a name="4VHWB"></a>
### 可以记录异常或抛出异常，但不要同时做
```java
catch (NoSuchMethodException e) {
     LOGGER.error("Some information", e);
     throw e;
  }
```
如上面的代码所示，抛出和日志记录可能会在日志文件中产生多个日志消息。<br />这就会导致同一个问题，却在日志中有很多不同的错误信息，使得开发人员陷入混乱。
<a name="VuIxc"></a>
### 永远不要在finally中抛出异常
```java
try {
    someMethod();  //抛出 exceptionOne
} finally {
    cleanUp();    //如果在这里再抛出一个异常，那么try中的exceptionOne将会丢失forever
}
```
在上面的例子中，如果someMethod()抛出一个异常，并且在finally块中，`cleanUp()`也抛出一个异常，那么初始的exceptionOne(正确的错误异常)将永远丢失。
<a name="p6ida"></a>
### 如果不打算处理异常，请使用finally块而不是catch块
```java
try {
    someMethod(); 
} finally {
    cleanUp();    
}
```
这也是一个很好的实践。如果在方法中访问其他方法，而该方法抛出了异常不想处理，但是仍然需要做一些清理工作，那么在finally块中进行清理。不要使用catch块。
