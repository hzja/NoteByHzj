Java
<a name="AkH5d"></a>
## 一、问题回溯
在项目的开发过程中，当对文件进行读写操作时，不知道大家有没有碰到这样的问题。<br />有的同学在做一个读取临时文件数据的工作，当读完文件内容，准备将其删除的时候，有时候会正常，但有时候会提示：**操作无法完成，因为文件已在 Java™ Platform SE binary 中打开**，编译器也会提示：**Resource leak: 'xxxx' is never closed**。<br />样例代码如下：
```java
File file = new File("xxx.txt");
// 实例化输入流
FileReader reader = new FileReader(file);
// 缓冲区
char[] buffer = new char[1024];

// 分次读取数据，每次最多读取1024个字符，将数据读取到缓冲区之中，同时返回读取的字节个数
int len;
while ((len = reader.read(buffer)) > -1) {
    // 字符转为字符串
    String msg = new String(buffer, 0, len);
    System.out.println(msg);
}

// 删除文件
file.delete();
```
经过排查，发现出现该问题的原因是：**读取文件的 IO 流没有正常的关闭，导致文件一直被流持有，删除文件不成功**！<br />那这么解决这个问题呢？答案其实也很简单，当读完 IO 流的数据或者写完数据，手动调用一下关闭流的方法，最后再进行删除文件。
```java
// 删除文件之前，先将 IO 流关闭
reader.close();

// 删除文件
file.delete();
```
可能有的同学会发出疑问，**为什么 IO 流必须手动关闭，不能像其他的方法一样坐等 GC 回收**？<br />一起来聊聊这个话题，以及如何正确的关闭 IO 流操作。
<a name="ryQDs"></a>
## 二、为什么 IO 流需要手动关闭？
熟悉编程语言的同学，可能知道，无论是 C 语言还是 C++，都需要手动释放内存，但是 Java 不需要。<br />这主要得益于 Java 的虚拟机垃圾回收机制，它可以帮助开发者自动回收内存中的对象，不需要手动释放内存，**但是有些东西它是无法回收的，例如端口、显存、文件等，超出了虚拟机能够释放资源的界限**。<br />如果对未关闭流的文件进行读写操作，可能就会报错，告诉你这个文件被某个进程占用。如果不手动释放资源，随着资源占有量逐渐增多，垃圾会越来越多，最终可能导致系统无法存储其他的资源，甚至会出现系统崩溃。<br />一般来说，只要存在 IO 流读写操作，无论使用到的是网络 IO 或者文件 IO，都是需要和计算机内的资源打交道的，清理计算机上面的垃圾，Java 的虚拟机垃圾回收机制没有这个能力。<br />熟悉 Java 虚拟机垃圾回收机制的同学，可能知道 gc 有两个显著的特点：

- gc 只能释放内存资源，而不能释放与内存无关的资源
- gc 回收具有不确定性，也就是说你根本不知道它什么时候会回收

所以进行流的操作时，凡是跨出虚拟机边界的资源都要求程序员自己手动关闭资源。<br />可能有的同学又发出疑问，平时本地测试的时候没有发现这个问题，为什么部署到线上就出这个提示的呢？<br />以读取文件的`FileInputStream`流为例，其实里面隐含了一个`finalize`方法，当虚拟机进行垃圾回收之前，会调用这个方法。<br />打开源码，会发现底层调用的其实是`close`释放资源的方法，可以看到 JDK 间接的帮助开发者进行最后一次的兜底。
```java
/**
 * Ensures that the <code>close</code> method of this file input stream is
 * called when there are no more references to it.
 *
 * @exception  IOException  if an I/O error occurs.
 * @see        java.io.FileInputStream#close()
 */
protected void finalize() throws IOException {
    if ((fd != null) &&  (fd != FileDescriptor.in)) {
        /* if fd is shared, the references in FileDescriptor
         * will ensure that finalizer is only called when
         * safe to do so. All references using the fd have
         * become unreachable. We can call close()
         */
        close();
    }
}
```
这就解释了，为什么只是时不时的会出现提示，并不是总是。这个方法什么时候被调用，这取决于虚拟机的垃圾回收频次。<br />**但是在实际的开发过程中，开发者不能完全依赖虚拟机回收这些系统资源，只要涉及到流的操作，强烈建议大家一定要手动关闭释放资源，避免出现一些不必要的bug**。<br />具体如何手动释放资源资源呢，接着看！
<a name="anNZO"></a>
## 三、正确的关闭流姿势介绍
在操作 Java 流对象后要将流进行关闭，但是现实的情况却往往不尽人意，原因是每个开发者的写法可能不尽相同，**不同的写法导致出现各种千奇百怪的问题**，下面一起来看看几种关闭流的代码案例！
<a name="khnhN"></a>
### 写法 1：在 try 中关流，而没在 `finally` 中关流
```java
try {
    OutputStream out = new FileOutputStream("file");
    // ...操作流代码
    out.close();
} catch (Exception e) {
    e.printStackTrace();
}
```
**当操作流代码报错的时候，这种写法会导致流无法正常的关闭，因此不推荐采用**！<br />正确的操作方式，应该在`finally`里面完成，实例代码如下：
```java
OutputStream out = null;
try {
    out = new FileOutputStream("file");
    // ...操作流代码
} catch (Exception e) {
    e.printStackTrace();
} finally {
    // 在 finally 中进行关闭，确保一定能被执行
    try {
        if (out != null) {
            out.close();
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
<a name="pmWJD"></a>
### 写法 2：在关闭多个流时，将其放在一个 try 中
在关闭多个流时，有的同学嫌弃麻烦，将其放在一个 try 中完成，实例代码如下：
```java
OutputStream out1 = null;
OutputStream out2 = null;
try {
    out1 = new FileOutputStream("file");
    out2 = new FileOutputStream("file");
    // ...操作流代码
} catch (Exception e) {
    e.printStackTrace();
} finally {
    try {
        if (out1 != null) {
            // 如果此处出现异常，则out2流没有被关闭
            out1.close();
        }
        if (out2 != null) {
            out2.close();
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
**这种写法下，当out1.close出异常的时候，out2.close是不会被正常关闭的，因此不推荐采用**！<br />正确的操作方式，应该是一个一个的close，别偷懒，实例代码如下：
```java
OutputStream out1 = null;
OutputStream out2 = null;
try {
    out1 = new FileOutputStream("file");
    out2 = new FileOutputStream("file");
    // ...操作流代码
} catch (Exception e) {
    e.printStackTrace();
} finally {
    try {
        if (out1 != null) {
            out1.close();
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
    try {
        if (out2 != null) {
            out2.close();
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
<a name="x99I9"></a>
### 写法 3：在循环中创建流，在循环外关闭
有的同学在循环操作多个文件时，在循环外关闭文件流，实例代码如下：
```java
OutputStream out = null;
try {
    for (int i = 0; i < 10; i++) {
        out = new FileOutputStream("file");
        // ...操作流代码
    }
} catch (Exception e) {
    e.printStackTrace();
} finally {
    try {
        if (out != null) {
            out.close();
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
表面看上去好像没有问题，但是实际上创建了 10 个 IO 流，try 里面的逻辑执行完成之后，只是把最后的一个 IO 流对象赋予给了out参数。**也就是当程序执行完毕之后，只关闭了最后一个 IO 流，其它 9 个 IO 流没用被手动关闭，因此不推荐采用**！<br />正确的操作方式，应该是在循环体内close，别偷懒，实例代码如下：
```java
for (int i = 0; i < 10; i++) {
    OutputStream out = null;
    try {
        out = new FileOutputStream("file");
        // ...操作流代码
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        try {
            if (out != null) {
                out.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="YM6W6"></a>
### 写法 4：关闭多个流时，没用遵循后定义先释放原则
有的同学在操作多个文件流时，操作完成之后，依照先后次序进行关闭文件流，实例代码如下：
```java
FileOutputStream fos = null;
BufferedOutputStream bos = null;
try {
    fos = new FileOutputStream("file");
    bos = new BufferedOutputStream(fos);
    // ...操作流代码
} catch (Exception e){

} finally {
    // 依次关闭流
    try {
        fos.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
    try {
        // 此处会报 java.io.IOException: Stream Closed 错误
        bos.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```
按照先后顺序关闭文件流，这种写法下，有可能会报java.io.IOException: Stream Closed错误。<br />原因是`BufferedOutputStream`依赖于`FileOutputStream`，如果直接关闭`FileOutputStream`流，再次关闭`BufferedOutputStream`，会提示源头已经被关闭，缓存区数据无法输出。<br />正确的操作方式，应该遵循**后定义先释放**的原则，实例代码如下：
```java
FileOutputStream fos = null;
BufferedOutputStream bos = null;
try {
    fos = new FileOutputStream("file");
    bos = new BufferedOutputStream(fos);
    // ...操作流代码
} catch (Exception e){

} finally {
    // 后定义先释放
    try {
        bos.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
    try {
        fos.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```
<a name="SW8rM"></a>
### 写法 5：jdk7 及以上版本，推荐采用 try-with-resources 写法
try-with-resources是 JDK 7 中引入的一个新的异常处理机制，它能让开发人员不用显式的释放try-catch语句块中使用的资源。<br />以上文为例，可以改成如下写法：
```java
try (FileOutputStream fos = new FileOutputStream("file");
     BufferedOutputStream bos = new BufferedOutputStream(fos)){
    // ...操作流代码
} catch (Exception e){
    e.printStackTrace();
}
```
**try-with-resources释放资源的操作，也是遵循的后定义先释放的原则**！
<a name="iCeDr"></a>
### 写法 6：使用包装流时，只需要关闭最后面的包装流即可
包装流是指通过装饰设计模式实现的 IO 流类，其目的是对底层流的功能进行扩展，在实际数据传输的时候，还是使用底层流进行传输。比如缓存字节输出流`BufferedOutputStream`就是一个包装流，目的是对字节输出流提供一个缓存区功能，让数据输出效率更高。<br />在使用到包装流的时候，只需要关闭最后面的包装流即可。<br />以上文为例，改写的实例代码如下：
```java
InputStream is = null;
InputStreamReader isr = null;
BufferedReader br = null;
try {
    is = new FileInputStream("file");
    isr = new InputStreamReader(is);
    br = new BufferedReader(isr);
    // ...操作流代码
} catch (Exception e){
    e.printStackTrace();
} finally {
    // 关闭包装流，也会自动关闭 InputStream 流
    try {
        br.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
}
```
这是因为，包装流关闭时会调用原生流的关闭方法，请看源码！
```java
public void close() throws IOException {
    synchronized (lock) {
        if (in == null)
            return;
        try {
            // 这里的in 指的是 InputStreamReader，最后会原生流的close方法
            in.close();
        } finally {
            in = null;
            cb = null;
        }
    }
}
```
<a name="nEWKC"></a>
## 四、内存流是否需要关闭？
在上文中提到只要是 IO 流都建议大家手动关闭资源，但是在 Java 中有一种流，它是不需要手动关闭的，比如内存读写流：`ByteArrayInputStream`、`ByteArrayOutputStream`。<br />不同于指向硬盘的流，`ByteArrayInputStream`和`ByteArrayOutputStream`其实是伪装成流的字节数组存储在内存中（把它们当成字节数据来看就好了），他们不会锁定任何文件句柄和端口，如果不再被使用，字节数组会被垃圾回收掉，所以不需要关闭。<br />当 IO 流是指向存储卡 / 硬盘 / 网络等外部资源的流，是一定要手动关闭的。
