Java
<a name="VT2x7"></a>
## 一、摘要
`RandomAccessFile` 类，也被称为**随机访问文件类**。<br />`RandomAccessFile` 可以说是 Java 体系中功能最为丰富的文件操作类，相比之前介绍的通过字节流或者字符流接口方式读写文件，`**RandomAccessFile**`** 类可以跳转到文件的任意位置处进行读写数据，而无需把文件从头读到尾**，但是该类仅限于操作文件，不能访问其他的 IO 设备，如网络、内存映像等。<br />**所以如果需要访问文件的部分内容，而不是把文件从头读到尾，使用 RandomAccessFile 将是更好的选择**。<br />实际上，虽然`RandomAccessFile`类具备随机读写数据的功能，但是它既不是`InputStream`的子类，也不是`OutputStream`的子类，绝大部分的方法都是从零开始写的，这可能是因为  `RandomAccessFile` 需要在文件里面前后移动，它的行为与其它的 I/O 类有着根本性的不同，所以相对比较独立。<br />`RandomAccessFile`对象类中内置了一个位置指示器，可以指向当前读写处的位置，当读写 n 个字节后，文件指示器将指向这 n 个字节后的下一个字节处。刚打开文件时，文件指示器指向文件的开头处，当移动文件指示器到新的位置时，随后的读写将从新的位置开始，这是它与其他的文件读写方式最大的不同。<br />基本上，`RandomAccessFile`的工作方式是，结合`DataInputStream`和`DataOutputStream`类完成数据的读写，再加上自己的一些方法，比如定位用的 `getFilePointer`方法，在文件里移动用的`seek`方法，以及判断文件大小length方法、跳过多少字节数的`skipBytes`方法等，来完成文件的随机访问和读写操作。
<a name="lAmEc"></a>
## 二、`RandomAccessFile` 类基本介绍
下面先来看看一个简单的例子。
```java
// 获取随机访问文件对象
RandomAccessFile raf = new RandomAccessFile(new File("randomFileDemo.txt"), "rw");
for (int i = 0; i < 10; i++) {
    // 写入数据，1个int占4个字节
    raf.writeInt(i);
}
raf.close();

System.out.println("================修改前的内容===============" );
// 重新获取随机访问文件对象
raf = new RandomAccessFile(new File("randomFileDemo.txt"), "rw");
for (int i = 0; i < 10; i++) {
    System.out.println("Value：" + i + ": " + raf.readInt());
}
raf.close();

// 重新获取随机访问文件对象
raf = new RandomAccessFile(new File("randomFileDemo.txt"), "rw");
// 设置文件指针偏移量，从0开始，直接将文件指针移到第6个int数据后面（1个int占4个字节）
raf.seek(5 * 4);
//覆盖第6个int数据
raf.writeInt(16);
raf.close();

System.out.println("================修改后的内容===============" );
// 重新获取随机访问文件对象
raf = new RandomAccessFile(new File("randomFileDemo.txt"), "rw");
for (int i = 0; i < 10; i++) {
    System.out.println("Value：" + i + ": " + raf.readInt());
}
raf.close();
```
输出结果：
```
================修改前的内容===============
Value：0: 0
Value：1: 1
Value：2: 2
Value：3: 3
Value：4: 4
Value：5: 5
Value：6: 6
Value：7: 7
Value：8: 8
Value：9: 9
================修改后的内容===============
Value：0: 0
Value：1: 1
Value：2: 2
Value：3: 3
Value：4: 4
Value：5: 16
Value：6: 6
Value：7: 7
Value：8: 8
Value：9: 9
```
`RandomAccessFile`类为用户提供了两种构造方法，具体操作方式如下：
```java
// 第一种构造方法：指定 file 对象和读写模式
RandomAccessFile raf = new RandomAccessFile(File file, String mode);

// 第二种构造方法：指定 filename 路径和读写模式
RandomAccessFile raf = new RandomAccessFile(String filename, String mode);
```
其实第二种构造方法也是`new File()`出来，再调用第一种构造方法，两者都可以获取随机访问文件对象。<br />至于`mode`，表示以何种方式打开文件，Java给开发者提供了四种mode值，具体解释如下！

| 模式 | 作用 |
| --- | --- |
| r | 表示以只读方式打开，调用结果对象的任何`write`方法都将导致抛出IOException |
| rw | 打开以便读取和写入，如果该文件尚不存在，则尝试创建该文件 |
| rws | 打开以便读取和写入，相对于"rw"，还要求对文件内容或元数据的每个更新都同步写入到底层存储设备 |
| rwd | 打开以便读取和写入，相对于"rw"，还要求对文件内容的每个更新都同步写入到底层存储设备 |

值得注意的地方是，**"rw"模式下，Java 并不强求指定的路径下一定存在某个文件，假如文件不存在，会自动创建**。<br />`RandomAccessFile`类为用户提供的方法比较多，可以关注下几个重要的方法即可，详细方法如下图！

| 方法 | 作用 |
| --- | --- |
| `void close()` | 关闭此随机访问文件流并释放与该流关联的所有系统资源（重要) |
| `FileChannel getChannel()` | 返回与此文件关联的唯一FileChannel对象，NIO场景会用到 |
| `long getFilePointer()` | 返回此文件中的当前偏移量 |
| `long length()` | 返回此文件的长度 |
| `void seek(long pos)` | 设置文件的指针偏移量，在该位置开始下一个读取或写入操作（重要） |
| `int skipBytes(int n)` | 跳过给定数量的字节，返回跳过的字节数（重要) |
| `void setLength(Long len)` | 设置文件的长度 |
| `int read()` | 返回读取的字节数 |
| `int read(byte[] b)` | 将读取到的字节数据，输出到byte数组中，并返回读入的总字节数，如果由于已经达到文件末尾而不再有数据，则返回-1。(重要) |
| `int read(byte[] b, int off, int len)` | 从此文件的指定初始偏移量off开始，读取最多leng个字节数据，输出到byte数组中，并返回读入的总字节数 |
| `boolean readBoolean()` | 从此文件读取一个boolean值，其余`readByte()`、`readChar()`、`readDouble()`等操作类似 |
| `String readLine()` | 从此文件读取一行文本数据，如果已经达到文件末尾而不再有数据，则返回空。 |
| `void write(byte[] b)` | 将给定的字节数组byte，写入文件 |
| `void write(byte[] b, int off, int len)` | 从偏移量off处开始，将给定的字节数组数据，最多leng个字节数据，写入文件 |
| `void writeBoolean(boolean<br />v)` | 按单字节值将boolean写入该文件，其余`writeByte(int v)`、`writeBytes(String s)`、`writeChar(int v)`等都类似，唯一不同的就是写入的字节总数不同  |

方法的使用，可以参考如下样例：
```java
RandomAccessFile file = new RandomAccessFile("file.txt", "rw");
// 以下向file文件中写数据
file.writeInt(20);// 占4个字节
file.writeDouble(8.236598);// 占8个字节
file.writeUTF("这是一个UTF字符串");// 这个长度写在当前文件指针的前两个字节处，可用readShort()读取
file.writeBoolean(true);// 占1个字节
file.writeShort(395);// 占2个字节
file.writeLong(2325451L);// 占8个字节
file.writeUTF("又是一个UTF字符串");
file.writeFloat(35.5f);// 占4个字节
file.writeChar('a');// 占2个字节

file.seek(0);// 把文件指针位置设置到文件起始处

// 以下从file文件中读数据，要注意文件指针的位置
System.out.println("——————从file文件指定位置读数据——————");
System.out.println(file.readInt());
System.out.println(file.readDouble());
System.out.println(file.readUTF());

file.skipBytes(3);// 将文件指针跳过3个字节，本例中即跳过了一个boolean值和short值。
System.out.println(file.readLong());

file.skipBytes(file.readShort()); // 跳过文件中“又是一个UTF字符串”所占字节，注意readShort()方法会移动文件指针，所以不用加2。
System.out.println(file.readFloat());

//以下演示文件复制操作
System.out.println("——————文件复制（从file到fileCopy）——————");
file.seek(0);
RandomAccessFile fileCopy=new RandomAccessFile("fileCopy.txt","rw");
int len=(int)file.length();//取得文件长度（字节数）
byte[] b=new byte[len];
file.readFully(b);//读取全部内容
fileCopy.write(b);//全部写入目标文件
System.out.println("复制完成！");
```
根据以上的方法介绍，可以利用`RandomAccessFile`实现一个在任意位置插入数据的操作，具体实例如下：
```java
public class RandomAccessFileTest1 {

    /**
     * 插入数据
     * @param skip 跳过多少过字节进行插入数据
     * @param str 要插入的字符串
     * @param fileName 文件路径
     */
    public static void insert(long skip, String str, String fileName){
        try {
            RandomAccessFile raf = new RandomAccessFile(fileName,"rw");
            if(skip <  0 || skip > raf.length()){
                System.out.println("跳过字节数无效");
                return;
            }
            byte[] b = str.getBytes();
            raf.setLength(raf.length() + b.length);
            // 将尾部数据进行迁移
            for(long i = raf.length() - 1; i > b.length + skip - 1; i--){
                raf.seek(i - b.length);
                byte temp = raf.readByte();
                raf.seek(i);
                raf.writeByte(temp);
            }
            // 从指定的位置，开始覆写数据
            raf.seek(skip);
            raf.write(b);
            raf.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 方法测试
     * @param args
     * @throws Exception
     */
    public static void main(String[] args) {
        insert(0, "一起学习Java", "test.txt");
        insert(0, "Hello，", "test.txt");
    }
}
```
文件内容结果如下：
```
Hello，一起学习Java
```
<a name="iANSu"></a>
## 三、`RandomAccessFile` 类的应用
在实际的开发过程中，`RandomAccessFile` 的一个重要应用场景就是网络请求中的文件多线程下载及断点续传。<br />首先将文件分成几块，然后每块用不同的线程进行下载，下面是一个利用多线程在写文件时的例子：
```java
public class RandomAccessFileTest2 {

    public static void main(String[] args) throws Exception {
        // 预分配文件所占的磁盘空间，磁盘中会创建一个指定大小的文件
        RandomAccessFile raf = new RandomAccessFile("test.txt", "rw");
        raf.setLength(1024*1024); // 预分配 1M 的文件空间
        raf.close();

        // 所要写入的文件内容
        String s1 = "第一个字符串";
        String s2 = "第二个字符串";
        String s3 = "第三个字符串";
        String s4 = "第四个字符串";
        String s5 = "第五个字符串";

        // 利用多线程同时写入一个文件
        new FileWriteThread(1024*1,s1.getBytes()).start(); // 从文件的1024字节之后开始写入数据
        new FileWriteThread(1024*2,s2.getBytes()).start(); // 从文件的2048字节之后开始写入数据
        new FileWriteThread(1024*3,s3.getBytes()).start(); // 从文件的3072字节之后开始写入数据
        new FileWriteThread(1024*4,s4.getBytes()).start(); // 从文件的4096字节之后开始写入数据
        new FileWriteThread(1024*5,s5.getBytes()).start(); // 从文件的5120字节之后开始写入数据
    }
}

class FileWriteThread extends Thread{
    private int skip;
    private byte[] content;

    public FileWriteThread(int skip,byte[] content){
        this.skip = skip;
        this.content = content;
    }

    @Override
    public void run(){
        RandomAccessFile raf = null;
        try {
            // 利用线程在文件的指定位置写入指定数据
            raf = new RandomAccessFile("test.txt", "rw");
            raf.seek(skip);
            raf.write(content);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                raf.close();
            } catch (Exception e) {
            }
        }
    }
}
```
<a name="KOtry"></a>
## 四、小结
主要围绕 `RandomAccessFile` 类的基本概念和常用方法，做了一次简单的知识总结，该类是 IO 流体系中功能最丰富的文件内容访问类，既可以读取文件中任意位置的内容，也可以向文件任意位置写入数据。<br />当然 `RandomAccessFile` 当读写大文件的时候，会出现内存溢出问题，此时可以采用**内存映射文件**方式进行读写数据。
