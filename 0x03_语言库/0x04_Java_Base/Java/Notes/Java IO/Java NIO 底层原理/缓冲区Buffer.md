Java Buffer
<a name="DNsi4"></a>
## 1、缓冲区(Buffer)的数据存取
一个用于特定基本数据类型的容器。由java.nio包定义，所有缓冲区都是Buffer抽象类的子类。<br />Java NIO中的Buffer主要用于与NIO通道进行交互，数据是从通道读入缓冲区，从缓冲区写入通道中的。
<a name="ReOUm"></a>
### 缓冲区（Buffer）
在Java NIO中负责数据的存取。缓冲区就是数组。用于存储不同数据类型的数据
<a name="GgRri"></a>
### 缓冲区的类型
根据数据类型不同（boolean 除外），提供了相应类型的缓冲区：

- ByteBuffer
- CharBuffer
- ShortBuffer
- IntBuffer
- LongBuffer
- FloatBuffer
- DoubleBuffer

上述缓冲区的管理方式几乎一致，通过allocate()获取缓冲区
<a name="23WiK"></a>
### 缓冲区存取数据的两个核心方法

- `put()`：存入数据到缓冲区中
- `get()`：获取缓冲区中的数据
<a name="Ag0SQ"></a>
### 缓冲区的四个核心属性

- capacity：容量，表示缓冲区中最大存储数据的容量。一旦声明不能改变
- limit：界限，表示缓冲区中可以操作数据的大小。（limit后数据不能进行读写）
- position：位置，表示缓冲区正在操作数据的位置
- mark：标记，表示当前position的位置。可以通过reset() 恢复到mark的位置
- 0 <= mark <= position <= limit <= capacity
<a name="1AIUl"></a>
## 2、直接缓冲区和非直接缓冲区

- 非直接缓冲区：通过`allocate()`方法分配缓冲区，将缓冲区建立在JVM的内存中
- 直接缓冲区：通过`allocateDirect()`方法分配直接缓冲区，将缓冲区建立在物理内存中。可以提高效率
