Java 字符串<br />毫无疑问 `String` 是作为一个 Java 开发工程师天天都需要打交道的类，那么如果问 `String` 字符串的最大长度是多少知道吗？<br />可能想都没想，就直接回答 `65535`，那么问题来了，真的吗？<br />首先对于 `String` 可以有下面几种用法：

1. 定义一个 `String` 类型的变量：`private static final String STRING_TEST = "xxxxxxxxxxx";` 或者 `String newString = "newString";`
2. 通过在方法中定义 `String` 类型的变量，通过字节流创建字符串：`byte[] bytes = new byte[length];String s = new String(bytes);`；

有朋友可能会说，这两种不都是定义一个字符串变量，有什么区别吗？表面上看是没什么区别，但是实际上区别还是蛮大的。<br />首先第一种方式定一个静态类变量，或者普通的字符串变量，这种形式字符串是存放在栈中的；而第二种方式字符串是存放在堆中的。这个时候有的小伙伴又要问了，这存在不同的地方有什么关系呢？首先这关系可大了！<br />当字符串存放在栈中的时候，根据 `class` 文件的结果规范，可以看到所采用的的存储格式是这样的：
```c
CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1 bytes[length];
}
```
其中 `u2` 是一种类似于Java 中 `int` 一样的数据类型，只是表示的是一个 2 个字节的数据类型，只不过 `int` 是 4 个字节，这也就意味着允许的最大长度为 `65535` 个字符。所以可以得出一个结果，当字符串存放在栈内存中的时候，字符串的长度可以达到 `65535`。<br />再看看第二种方式。很显然第二种方式不管是通过字节流的方式，还是 `new` 一个对象，存放的位置都是早 Java 的堆内存中，而且通过 `String` 的源码，可以看到了，底层是通过一个 `char[]` 数组来存放的。
```java
private final char value[];
```
那么就知道了，字符传的大小就跟数组的长度有直接关系了，另外在定义数组长度的时候，最多只能定义 `int` 类型的最大值，也就是`Integer.MAX_VALUE = 0x7fffffff;` 而且 `String` 类的 `length()` 方法的返回值也可以看出来，返回的类型是 `int` ，数值最大也是`Integer.MAX_VALUE = 0x7fffffff;`
```java
/**
     * Returns the length of this string.
     * The length is equal to the number of <a href="Character.html#unicode">Unicode
     * code units</a> in the string.
     *
     * @return  the length of the sequence of characters represented by this
     *          object.
     */
public int length() {
    return value.length;
}
```
所以看到这里，又得出了一个结果，**当字符串存放在堆内存的时候，最大的长度为 `Integer.MAX_VALUE = 0x7fffffff;` **。不过需要注意的是，这个数值是理论上的，其实很多虚拟机会在数组中加入一些字符，所以实际的数值是达不到这么多，另外在 `ArrayList` 中也可以看到这个验证，这里定义的最大值就是`Integer.MAX_VALUE - 8`; 而不直接采用最大值。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636255404497-3e18d3a1-7af2-4f0a-97af-17cca0356b7a.webp#clientId=u2c2b7b55-f3d6-4&from=paste&id=u3d249621&originHeight=338&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u9d37a634-c326-4b7d-960c-7c604801d27)<br />此外上面说的最大值是在虚拟机有这么大的内存的前提下，如果说虚拟机配置的内存比这个要小，那也是达不到这么大。可以通过 JVM 参数来配置虚拟机的内存大小，`-Xms512m` 设置堆内存初始值大小。`-Xmx1024m` 设置堆内存最大值。下面是在自己的电脑上测试的效果，可以看到，当开始提示Requested array size exceeds VM limit，后面因为电脑内存不够了，所以一直分配失败，达不到最大值，只能降低长度了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636255404596-037c1d31-d435-4f1d-abff-6beb41917108.webp#clientId=u2c2b7b55-f3d6-4&from=paste&id=u590eff83&originHeight=610&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u6ff24edf-d2ce-4c80-a969-6fd69416a1e)<br />另外还要注意一个点，那就是在这里说的长度针对的都是英文字符。
