Java IO
<a name="XYtec"></a>
### 包装流的`close`方法是否会自动关闭被包装的流？
平时使用输入流和输出流一般都会使用buffer包装一下，直接看下面代码（这个代码运行正常，不会报错）
```java
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;


public class IOTest {

    public static void main(String[] args) throws IOException {

         FileOutputStream fileOutputStream = new FileOutputStream("c:\\a.txt");
         BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);

         bufferedOutputStream.write("test write something".getBytes());
         bufferedOutputStream.flush();

         //从包装流中关闭流
         bufferedOutputStream.close();
    }

}
```
下面来研究下这段代码的`bufferedOutputStream.close();`方法是否调用了`fileOutputStream.close();`<br />先看`BufferedOutputStream`源代码：
```java
public class BufferedOutputStream extends FilterOutputStream { ...
```
可以看到它继承`FilterOutputStream`，并且没有重写`close`方法，所以直接看`FilterOutputStream`的源代码：
```java
public void close() throws IOException {
	try {
	  flush();
	} catch (IOException ignored) {
	}
	out.close();
}
```
跟踪out（`FilterOutputStream`中）：
```java
protected OutputStream out;

public FilterOutputStream(OutputStream out) {
	this.out = out;
}
```
再看看`BufferedOutputStream`中：
```java
public BufferedOutputStream(OutputStream out) {
	this(out, 8192);
}

public BufferedOutputStream(OutputStream out, int size) {
	super(out);
	if (size <= 0) {
		throw new IllegalArgumentException("Buffer size <= 0");
	}
	buf = new byte[size];
}
```
可以看到`BufferedOutputStream`调用`super(out);`，也就是说，`out.close();`调用的是通过`BufferedOutputStream`传入的被包装的流，这里就是`FileOutputStream`。<br />在看看其他类似的，比如`BufferedWriter`的源代码：
```java
public void close() throws IOException {
	synchronized (lock) {
		if (out == null) {
			return;
		}
		try {
			flushBuffer();
		} finally {
			out.close();
			out = null;
			cb = null;
		}
	}
}
```
通过观察各种流的源代码，可得结论：包装的流都会自动调用被包装的流的关闭方法，无需自己调用。
<a name="K5tGX"></a>
### 关闭流方法是否有顺序？
由上面的结论，就会产生一个问题：如果手动关闭被包装流会怎么样，这个关闭流有顺序吗？而实际上习惯都是两个流都关闭的。<br />首先来做一个简单的实验，基于第一个问题的代码上增加手动增加关闭流的代码，那么就有两种顺序：
<a name="yk7Az"></a>
#### 1、先关闭被包装流（正常没异常抛出）
```java
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;


public class IOTest {

    public static void main(String[] args) throws IOException {

         FileOutputStream fileOutputStream = new FileOutputStream("c:\\a.txt");
         BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);

         bufferedOutputStream.write("test write something".getBytes());
         bufferedOutputStream.flush();

         fileOutputStream.close();//先关闭被包装流
         bufferedOutputStream.close();
    }

}
```
<a name="Flsri"></a>
#### 2、先关闭包装流（正常没异常抛出）
```java
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;


public class IOTest {

    public static void main(String[] args) throws IOException {

         FileOutputStream fileOutputStream = new FileOutputStream("c:\\a.txt");
         BufferedOutputStream bufferedOutputStream = new BufferedOutputStream(fileOutputStream);

         bufferedOutputStream.write("test write something".getBytes());
         bufferedOutputStream.flush();


         bufferedOutputStream.close();//先关闭包装流
         fileOutputStream.close();
    }
}
```
上述两种写法都没有问题，已经知道`bufferedOutputStream.close();`会自动调用`fileOutputStream.close();`方法，那么这个方法是怎么执行的呢？看看`FileOutputStream`的源码：
```java
public void close() throws IOException {
	synchronized (closeLock) {
		if (closed) {
			return;
		}
		closed = true;
	}

...
```
可以看出它采用同步锁，而且使用了关闭标记，如果已经关闭了则不会再次操作，所以多次调用不会出现问题。<br />如果没有看过参考文章，可能就会断下结论，关闭流不需要考虑顺序<br />看下下面的代码：
```java
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

public class IOTest {

    public static void main(String[] args) throws IOException {

        FileOutputStream fos = new FileOutputStream("c:\\a.txt");
        OutputStreamWriter osw = new OutputStreamWriter(fos, "UTF-8");
        BufferedWriter bw = new BufferedWriter(osw);
        bw.write("java IO close test");

        // 从内带外顺序顺序会报异常
        fos.close();
        osw.close();
        bw.close();

    }

}
```
会抛出Stream closed的IO异常：
```java
Exception in thread "main" java.io.IOException: Stream closed
    at sun.nio.cs.StreamEncoder.ensureOpen(StreamEncoder.java:45)
    at sun.nio.cs.StreamEncoder.write(StreamEncoder.java:118)
    at java.io.OutputStreamWriter.write(OutputStreamWriter.java:207)
    at java.io.BufferedWriter.flushBuffer(BufferedWriter.java:129)
    at java.io.BufferedWriter.close(BufferedWriter.java:264)
    at IOTest.main(IOTest.java:18)
```
而如果把`bw.close();`放在第一，其他顺序任意，即修改成下面两种：
```java
bw.close();
osw.close();
fos.close();

bw.close();
fos.close();
osw.close();
```
都不会报错，这是为什么呢，看看`BufferedWriter`的`close`源码：
```java
public void close() throws IOException {
	synchronized (lock) {
		if (out == null) {
			return;
		}
		try {
			flushBuffer();
		} finally {
			out.close();
			out = null;
			cb = null;
		}
	}
}
```
里面调用了`flushBuffer()`方法，也是抛异常中的错误方法：
```java
void flushBuffer() throws IOException {
	synchronized (lock) {
		ensureOpen();
		if (nextChar == 0)
			return;
		out.write(cb, 0, nextChar);
		nextChar = 0;
	}
}
```
可以看到很大的一行
```java
out.write(cb, 0, nextChar);
```
这行如果在流关闭后执行就会抛IO异常，有时候会写成：
```java
fos.close();
fos = null;
osw.close();
osw = null;
bw.close();
bw = null;
```
这样也会抛异常，不过是由于`flushBuffer()`中`ensureOpen()`抛的，可从源码中看出：
```java
private void ensureOpen() throws IOException {
	if (out == null)
		throw new IOException("Stream closed");
}


void flushBuffer() throws IOException {
	synchronized (lock) {
		ensureOpen();
		if (nextChar == 0)
			return;
		out.write(cb, 0, nextChar);
		nextChar = 0;
	}
}
```
如何防止这种情况？<br />直接写下面这种形式就可以：
```java
bw.close();
bw = null;
```
:::success
结论：一个流上的`close`方法可以多次调用，理论上关闭流不需要考虑顺序，但有时候关闭方法中调用了`write`等方法时会抛异常。
:::

---

由上述的两个结论可以得出下面的建议：<br />**关闭流只需要关闭最外层的包装流，其他流会自动调用关闭，这样可以保证不会抛异常。**如：
```java
bw.close();
//下面三个无顺序
osw = null;
fos = null;
bw = null;
```
注意的是，有些方法中`close`方法除了调用被包装流的`close`方法外还会把包装流置为null，方便JVM回收。`bw.close()`中的：
```java
public void close() throws IOException {
	synchronized (lock) {
		if (out == null) {
			return;
		}
		try {
			flushBuffer();
		} finally {
			out.close();
			out = null;
			cb = null;
		}
	}
}
```
`finally`中就有把out置为null的代码，所以有时候不需要自己手动置为null。（建议还是写一下）
