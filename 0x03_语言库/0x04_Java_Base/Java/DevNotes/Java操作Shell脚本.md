Java Shell<br />在工作过程中，大部分是Java开发，但只会Java肯定远远不够。 在文本处理过程中，主要是用脚本进行开发。脚本开发的特点就是在进行批处理的时候非常方便。比如这么一个需求场景：<br />有一些特殊文件需要特殊环境才能操作（比如：IOS 奔溃文件还原，Android 奔溃混淆文件还原），而部署上线后必须依赖Linux环境执行，所以需要借助Java的Process类调用shell脚本处理一些任务！<br />下面主要介绍两点：

1. Java的Process类是什么
2. 如何使用Process类操作Shell脚本指令
3. Process 类操作shell脚本文件
4. Process类和ProcessBuilder源码
<a name="drI6z"></a>
## 一、Java中的 Process类
为了执行调用操作，JVM会启一个Process，Process类主要有以下几个方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621846134714-5e12f2b0-6858-4559-bb7e-a2764adb303f.png#clientId=u8ad04552-77f2-4&from=paste&height=413&id=uedd2cb83&originHeight=550&originWidth=718&originalType=binary&ratio=1&size=55383&status=done&style=none&taskId=ufc45348b-88ae-4373-8541-cfe3c9aa821&width=539)<br />为了执行调用操作，JVM会启一个Process，所以可以通过调用Process类的以下方法，得知调用操作是否正确执行：
<a name="SBBmP"></a>
### 1.1 `waitFor()` 
```java
//导致当前线程等待
//返回值是进程的出口值。0 表示正常终止；否则，就表示异常失败。
abstract int waitFor()
```
<a name="y6X71"></a>
### 1.2 `isAlive()` 
```java
// 检测子进程是否存活，存活则返回 true 
public boolean isAlive()
```
另外，调用某些Shell命令或脚本时，会有返回值，那么如果捕获这些返回值或输出呢？为了解决这个问题，Process类提供了：
<a name="AqQvr"></a>
### 1.3 `getInputStream()` 
```java
//获取子进程的输入流。 最好对输入流进行缓冲。
abstract InputStream getInputStream() 
```
<a name="rilIO"></a>
## 二、如何使用`Process`类操作Shell脚本指令
主要实现方式有两种：

- 直接使用 `Process`类处理进程
- 使用`ProcessBuilder`构造一个进程生成器
<a name="b9SIh"></a>
### 2.1 直接使用 Process类操作
在Linux系统查看系统当前目录<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621846244748-bf81206b-0d51-4854-a33f-4e2c825c2105.png#clientId=u8ad04552-77f2-4&from=paste&height=74&id=u332467df&originHeight=221&originWidth=3323&originalType=binary&ratio=1&size=179019&status=done&style=none&taskId=u4e12f071-19fc-4f8c-92b6-fa3e5e98c6a&width=1107.6666666666667)<br />使用Java调用实现同样的功能
```java
private static void execDemo() throws IOException, InterruptedException {
    Process process = Runtime.getRuntime().exec("pwd");
    // 检查执行状态
    int exitValue = process.waitFor();
    if (0 != exitValue) {
        log.error("call shell failed. error code is :" + exitValue);
    }
    BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
    String line = null;
    while ((line = reader.readLine()) != null) {
        System.out.println("line = " + line);
    }
}
```
<a name="oGEQi"></a>
### 2.2 使用`ProcessBuilder`构造一个进程生成器
使用Java的`ProcessBuilder`实现同样的功能
```java
private static void ProcessBuilderDemo() throws IOException, InterruptedException {
    ProcessBuilder processBuilder = new ProcessBuilder("pwd");
    // 将错误流重定向到输出流
    processBuilder.redirectErrorStream(true);
    Process start = processBuilder.start();
    int exitValue = start.waitFor();
    if (0 != exitValue) {
        log.error("call shell failed. error code is :" + exitValue);
    }
    BufferedReader reader = new BufferedReader(new InputStreamReader(start.getInputStream()));
    String line = null;
    while ((line = reader.readLine()) != null) {
        System.out.println("line = " + line);
    }
}
```
<a name="H8GX1"></a>
### 2.3 `Runtime`方式和`ProcessBuilder`方式对比

- `Runtime`方式：此为最常见的一种运行方式，历史最悠久，使应用程序能够与其运行的环境相连接，但是在读取上还存在一些不便性，正常的输出流与错误流得分开读取。其他功能基本相同。
- `ProcessBuilder`：此为jdk1.5加入的，它没有将应用程序与其运行的环境相连接。这个就需要自己设置其相关的信息。但它提供了将正常流与流程流合并在一起的解决办法，只需要设置`redirectErrorStream`（错误流重定向到标准数据流）属性即可，这样更有利于脚本执行过程中出现问题时排查。
<a name="sScb4"></a>
## 三、Process类操作Shell脚本文件
<a name="wwAeP"></a>
### 假如要调用的Shell脚本是文件：/root/experiment/test.sh
```bash
#!/usr/bin/env bash
args=1
if [ $# -eq 1 ];then
 args=$1
 echo "The argument is: $args"
fi
 
echo "This is a $call"
start=`date +%s`
sleep 3s
end=`date +%s`
cost=$((($end - $start) * $args * $val))
echo "Cost Time: $cost"
```
<a name="Hhtfh"></a>
### Java调用代码
```java
public static void callShellScript_test() {
    BufferedReader bufferedReader = null;
    try {
        String shellPath = CallShellScriptDemo01.class.getResource("/").getPath() + "shell/test.sh";
        ProcessBuilder builder = new ProcessBuilder("/bin/sh", shellPath);
        // 错误流重定向到标准输出流
        builder.redirectErrorStream(true);
        Process ps = builder.start();
        int exitValue = ps.waitFor();
        if (0 != exitValue) {
            log.error("call shell failed. error code is :" + exitValue);
        }
        bufferedReader = new BufferedReader(new InputStreamReader(ps.getInputStream()));
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            System.out.println("line = " + line.toString());
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
    log.info("数据刷新完成");
}
```
<a name="mzjE4"></a>
## 四、Process类和ProcessBuilder源码
<a name="CO4mZ"></a>
### 4.1 Process类
源代码虽是英文的，但是可以通过对原注释的阅读，直接体会jdk原创作者的真实想法，这样大家对process这个类会有更深一层次的理解。
```java

/**
 * The {@link ProcessBuilder#start()} and
 * {@link Runtime#exec(String[],String[],File) Runtime.exec}
 * methods create a native process and return an instance of a
 * subclass of {@code Process} that can be used to control the process
 * and obtain information about it.  The class {@code Process}
 * provides methods for performing input from the process, performing
 * output to the process, waiting for the process to complete,
 * checking the exit status of the process, and destroying (killing)
 * the process.
 *
 * <p>The methods that create processes may not work well for special
 * processes on certain native platforms, such as native windowing
 * processes, daemon processes, Win16/DOS processes on Microsoft
 * Windows, or shell scripts.
 *
 * <p>By default, the created subprocess does not have its own terminal
 * or console.  All its standard I/O (i.e. stdin, stdout, stderr)
 * operations will be redirected to the parent process, where they can
 * be accessed via the streams obtained using the methods
 * {@link #getOutputStream()},
 * {@link #getInputStream()}, and
 * {@link #getErrorStream()}.
 * The parent process uses these streams to feed input to and get output
 * from the subprocess.  Because some native platforms only provide
 * limited buffer size for standard input and output streams, failure
 * to promptly write the input stream or read the output stream of
 * the subprocess may cause the subprocess to block, or even deadlock.
 *
 * <p>Where desired, <a href="ProcessBuilder.html#redirect-input">
 * subprocess I/O can also be redirected</a>
 * using methods of the {@link ProcessBuilder} class.
 *
 * <p>The subprocess is not killed when there are no more references to
 * the {@code Process} object, but rather the subprocess
 * continues executing asynchronously.
 *
 * <p>There is no requirement that a process represented by a {@code
 * Process} object execute asynchronously or concurrently with respect
 * to the Java process that owns the {@code Process} object.
 *
 * <p>As of 1.5, {@link ProcessBuilder#start()} is the preferred way
 * to create a {@code Process}.
 *
 * @since   JDK1.0
 */
public abstract class Process {
/**
 * Returns the output stream connected to the normal input of the
 * subprocess.  Output to the stream is piped into the standard
 * input of the process represented by this {@code Process} object.
 *
 * <p>If the standard input of the subprocess has been redirected using
 * {@link ProcessBuilder#redirectInput(Redirect)
 * ProcessBuilder.redirectInput}
 * then this method will return a
 * <a href="ProcessBuilder.html#redirect-input">null output stream</a>.
 *
 * <p>Implementation note: It is a good idea for the returned
 * output stream to be buffered.
 *
 * @return the output stream connected to the normal input of the
 *         subprocess
 */
public abstract OutputStream getOutputStream();

/**
 * Returns the input stream connected to the normal output of the
 * subprocess.  The stream obtains data piped from the standard
 * output of the process represented by this {@code Process} object.
 *
 * <p>If the standard output of the subprocess has been redirected using
 * {@link ProcessBuilder#redirectOutput(Redirect)
 * ProcessBuilder.redirectOutput}
 * then this method will return a
 * <a href="ProcessBuilder.html#redirect-output">null input stream</a>.
 *
 * <p>Otherwise, if the standard error of the subprocess has been
 * redirected using
 * {@link ProcessBuilder#redirectErrorStream(boolean)
 * ProcessBuilder.redirectErrorStream}
 * then the input stream returned by this method will receive the
 * merged standard output and the standard error of the subprocess.
 *
 * <p>Implementation note: It is a good idea for the returned
 * input stream to be buffered.
 *
 * @return the input stream connected to the normal output of the
 *         subprocess
 */
public abstract InputStream getInputStream();

/**
 * Returns the input stream connected to the error output of the
 * subprocess.  The stream obtains data piped from the error output
 * of the process represented by this {@code Process} object.
 *
 * <p>If the standard error of the subprocess has been redirected using
 * {@link ProcessBuilder#redirectError(Redirect)
 * ProcessBuilder.redirectError} or
 * {@link ProcessBuilder#redirectErrorStream(boolean)
 * ProcessBuilder.redirectErrorStream}
 * then this method will return a
 * <a href="ProcessBuilder.html#redirect-output">null input stream</a>.
 *
 * <p>Implementation note: It is a good idea for the returned
 * input stream to be buffered.
 *
 * @return the input stream connected to the error output of
 *         the subprocess
 */
public abstract InputStream getErrorStream();

/**
 * Causes the current thread to wait, if necessary, until the
 * process represented by this {@code Process} object has
 * terminated.  This method returns immediately if the subprocess
 * has already terminated.  If the subprocess has not yet
 * terminated, the calling thread will be blocked until the
 * subprocess exits.
 *
 * @return the exit value of the subprocess represented by this
 *         {@code Process} object.  By convention, the value
 *         {@code 0} indicates normal termination.
 * @throws InterruptedException if the current thread is
 *         {@linkplain Thread#interrupt() interrupted} by another
 *         thread while it is waiting, then the wait is ended and
 *         an {@link InterruptedException} is thrown.
 */
public abstract int waitFor() throws InterruptedException;

/**
 * Causes the current thread to wait, if necessary, until the
 * subprocess represented by this {@code Process} object has
 * terminated, or the specified waiting time elapses.
 *
 * <p>If the subprocess has already terminated then this method returns
 * immediately with the value {@code true}.  If the process has not
 * terminated and the timeout value is less than, or equal to, zero, then
 * this method returns immediately with the value {@code false}.
 *
 * <p>The default implementation of this methods polls the {@code exitValue}
 * to check if the process has terminated. Concrete implementations of this
 * class are strongly encouraged to override this method with a more
 * efficient implementation.
 *
 * @param timeout the maximum time to wait
 * @param unit the time unit of the {@code timeout} argument
 * @return {@code true} if the subprocess has exited and {@code false} if
 *         the waiting time elapsed before the subprocess has exited.
 * @throws InterruptedException if the current thread is interrupted
 *         while waiting.
 * @throws NullPointerException if unit is null
 * @since 1.8
 */
    public boolean waitFor(long timeout, TimeUnit unit)（）
/**
 * Returns the exit value for the subprocess.
 *
 * @return the exit value of the subprocess represented by this
 *         {@code Process} object.  By convention, the value
 *         {@code 0} indicates normal termination.
 * @throws IllegalThreadStateException if the subprocess represented
 *         by this {@code Process} object has not yet terminated
 */
public abstract int exitValue();

/**
 * Kills the subprocess. Whether the subprocess represented by this
* {@codeProcess} object is forcibly terminated or not is
* implementation dependent.
*/
public abstract void destroy();

/**
 * Kills the subprocess. The subprocess represented by this
 * {@code Process} object is forcibly terminated.
 *
 * <p>The default implementation of this method invokes {@link #destroy}
 * and so may not forcibly terminate the process. Concrete implementations
 * of this class are strongly encouraged to override this method with a
 * compliant implementation.  Invoking this method on {@code Process}
 * objects returned by {@link ProcessBuilder#start} and
 * {@link Runtime#exec} will forcibly terminate the process.
 *
 * <p>Note: The subprocess may not terminate immediately.
 * i.e. {@code isAlive()} may return true for a brief period
 * after {@code destroyForcibly()} is called. This method
 * may be chained to {@code waitFor()} if needed.
 *
 * @return the {@code Process} object representing the
 *         subprocess to be forcibly destroyed.
 * @since 1.8
 */
    public Process destroyForcibly(){}


/**
 * Tests whether the subprocess represented by this {@code Process} is
 * alive.
 *
 * @return {@code true} if the subprocess represented by this
 *         {@code Process} object has not yet terminated.
 * @since 1.8
 */
public boolean isAlive(){}
}
```
