Java IO<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1634894309861-dda3f816-2786-4b95-9e5b-075d5f6de8ba.jpeg)
<a name="Ru8JK"></a>
## IO流的分类
根据处理数据类型的不同分为：字符流和字节流<br />根据数据流向不同分为：输入流和输出流
<a name="v1fQ8"></a>
### 字符流和字节流
字符流的由来：因为数据编码的不同，而有了对字符进行高效操作的流对象。本质其实就是基于字节流读取时，去查了指定的码表。字节流和字符流的区别：

- 读写单位不同：字节流以字节（8bit）为单位，字符流以字符为单位，根据码表映射字符，一次可能读多个字节。
- 处理对象不同：字节流能处理所有类型的数据（如图片、avi等），而字符流只能处理字符类型的数据。

结论：只要是处理纯文本数据，就优先考虑使用字符流。除此之外都使用字节流。
<a name="kKSia"></a>
### 输入流和输出流
对输入流只能进行读操作，对输出流只能进行写操作，程序中需要根据待传输数据的不同特性而使用不同的流。
<a name="JRENR"></a>
## 字节流
<a name="sPteT"></a>
### 输入

- `InputStream` 是所有的输入字节流的父类，它是一个抽象类。
- `ByteArrayInputStream`、`StringBufferInputStream`、`FileInputStream` 是三种基本的介质流，它们分别从Byte 数组、StringBuffer、和本地文件中读取数据。`PipedInputStream` 是从与其它线程共用的管道中读取数据
- `ObjectInputStream` 和所有`FilterInputStream` 的子类都是装饰流（装饰器模式的主角）
<a name="CdOEZ"></a>
### 输出

- `OutputStream` 是所有的输出字节流的父类，它是一个抽象类。
- `ByteArrayOutputStream`、`FileOutputStream` 是两种基本的介质流，它们分别向Byte 数组、和本地文件中写入数据。`PipedOutputStream` 是向与其它线程共用的管道中写入数据，
- `ObjectOutputStream` 和所有`FilterOutputStream` 的子类都是装饰流。
<a name="zW4Ee"></a>
### 配对
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633869051533-08fccd9a-4c73-405f-8a13-1549efad6ae7.webp#clientId=ucaf0b7af-385a-4&from=paste&id=u2cd104a0&originHeight=411&originWidth=436&originalType=url&ratio=1&status=done&style=shadow&taskId=u88194401-fbb3-4c9f-b023-e7cb8eeae12)<br />图中蓝色的为主要的对应部分，红色的部分就是不对应部分。紫色的虚线部分代表这些流一般要搭配使用。

- `LineNumberInputStream` 主要完成从流中读取数据时，会得到相应的行号，至于什么时候分行、在哪里分行是由该类主动确定的，并不是在原始中有这样一个行号。在输出部分没有对应的部分，完全可以自己建立一个`LineNumberOutputStream`，在最初写入时会有一个基准的行号，以后每次遇到换行时会在下一行添加一个行号，看起来也是可以的。好像更不入流了。
- `PushbackInputStream` 的功能是查看最后一个字节，不满意就放回流中。主要用在编译器的语法、词法分析部分。输出部分的`BufferedOutputStream` 几乎实现相近的功能。（预读，可以将读取内容的一部分放回流的开头，下回重读）
- `StringBufferInputStream` 已经被`Deprecated`，本身就不应该出现在`InputStream` 部分，主要因为String 应该属于字符流的范围。已经被废弃了，当然输出部分也没有必要需要它了！还允许它存在只是为了保持版本的向下兼容而已。
- `SequenceInputStream` 可以认为是一个工具类，将两个或者多个输入流当成一个输入流依次读取。完全可以从IO 包中去除，还完全不影响IO 包的结构，却让其更“纯洁”――纯洁的`Decorator` 模式。
- `PrintStream` 也可以认为是一个辅助工具。主要可以向其他输出流，或者`FileInputStream` 写入数据，本身内部实现还是带缓冲的。本质上是对其它流的综合运用的一个工具而已。一样可以踢出IO 包！`System.out` 和`System.out` 就是`PrintStream` 的实例！
<a name="RH28V"></a>
## 字符流
<a name="x2ti9"></a>
### 输入

- `Reader` 是所有的输入字符流的父类，它是一个抽象类。
- `CharReader`、`StringReader` 是两种基本的介质流，它们分别将Char 数组、String中读取数据。`PipedReader`是从与其它线程共用的管道中读取数据。
- `BufferedReader` 很明显就是一个装饰器，它和其子类负责装饰其它Reader 对象。
- `FilterReader` 是所有自定义具体装饰流的父类，其子类`PushbackReader` 对Reader 对象进行装饰，会增加一个行号。
- `InputStreamReader` 是一个连接字节流和字符流的桥梁，它将字节流转变为字符流（转换流）。`FileReader` 可以说是一个达到此功能、常用的工具类，在其源代码中明显使用了将FileInputStream`转变为Reader 的方法。
<a name="qq2Fi"></a>
### 输出

- `Writer` 是所有的输出字符流的父类，它是一个抽象类。
- `CharArrayWriter`、`StringWriter` 是两种基本的介质流，它们分别向Char 数组、String 中写入数据。`PipedWriter` 是向与其它线程共用的管道中写入数据，
- `BufferedWriter` 是一个装饰器为Writer 提供缓冲功能。
- `PrintWriter` 和`PrintStream` 极其类似，功能和使用也非常相似。
- `OutputStreamWriter` 是`OutputStream` 到`Writer` 转换的桥梁（转换流），它的子类FileWriter 其实就是一个实现此功能的具体类。
<a name="R7AZ3"></a>
### 配对
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633869051376-c2ba7e7e-af95-4f37-bfde-a01cb40cf703.webp#clientId=ucaf0b7af-385a-4&from=paste&id=ua2432f24&originHeight=491&originWidth=431&originalType=url&ratio=1&status=done&style=shadow&taskId=u25e8d1fe-af64-4f55-9c02-2afe36b3750)
<a name="kRIoR"></a>
## `RandomAccessFile`
该类并不是流体系中的一员，其封装了字节流，同时还封装了一个缓冲区（字符数组），通过内部的指针来操作字符数组中的数据。该对象特点：

- 该对象只能操作文件，所以构造函数接收两种类型的参数：a.字符串文件路径；b.File对象。
- 该对象既可以对文件进行读操作，也能进行写操作，在进行对象实例化时可指定操作模式(r,rw)

注意：该对象在实例化时，如果要操作的文件不存在，会自动创建；如果文件存在，写数据未指定位置，会从头开始写，即覆盖原有的内容。可以用于多线程下载或多个线程同时写数据到文件。
<a name="GRbID"></a>
## 管道
Java的I/O库提供了一个称做链接（Chaining）的机制，可以将一个流处理器跟另一个流处理器首尾相接，以其中之一的输出为输入，形成一个 流管道的链接。<br />例如，`DataInputStream`流处理器可以把`FileInputStream`流对象的输出当作输入，将Byte类型的数据转换成Java的原始类型和String类型的数据。
<a name="afbJ4"></a>
## 设计模式
Java I/O库的两个设计模式：

- 装饰者模式：也叫包装器模式，在由`InputStream`，`OutputStream`，`Reader`和`Writer`代表的等级结构内部，有一些流处理器可以 对另一些流处理器起到装饰作用，形成新的，具有改善了的功能的流处理器。装饰者模式是Java I/O库的整体设计模式。
- 适配器模式：在由`InputStream`，`OutputStream`，`Reader`和`Writer`代表的等级结构内部，有一些流处理器是对其它类型的流源的适配。
