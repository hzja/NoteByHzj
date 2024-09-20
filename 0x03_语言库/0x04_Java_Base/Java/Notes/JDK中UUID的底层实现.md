Java UUID
<a name="FCgCZ"></a>
## 前提
UUID是Universally Unique IDentifier的缩写，翻译为通用唯一标识符或者全局唯一标识符。对于UUID的描述，下面摘录一下规范文件A Universally Unique IDentifier (UUID) URN Namespace中的一些描述：
:::info
UUID（也称为GUID）定义了统一资源名称命名空间。UUID的长度为128比特，可以保证在空间和时间上的唯一性。
:::
「动机：」
:::info
使用UUID的主要原因之一是不需要集中式管理，其中一种格式限定了IEEE 802节点标识符，其他格式无此限制。可以自动化按需生成UUID，应用于多重不同的场景。UUID算法支持极高的分配速率，每台机器每秒钟可以生成超过1000万个UUID，因此它们可以作为事务ID使用。UUID具有固定大小128比特，与其他替代方案相比，它具有体积小的优势，非常适用于各种排序、散列和存储在数据库中，具有编程易用性的特点。
:::
这里只需要记住UUID几个核心特定：

- 全局时空唯一性
- 固定长度128比特，也就是16字节(1 byte = 8 bit)
- 分配速率极高，单机每秒可以生成超过1000万个UUID（实际上更高）

下面就JDK中的UUID实现详细分析一下UUID生成算法。编写本文的时候选用的JDK为JDK11。
<a name="Q63rT"></a>
## 再聊UUID
前面为了编写简单的摘要，所以只粗略摘录了规范文件里面的一些章节，这里再详细聊聊UUID的一些定义、碰撞概率等等。
<a name="oMpXf"></a>
### UUID定义
UUID是一种软件构建的标准，也是开放软件基金会组织在分布式计算环境领域的一部分。提出此标准的目的是：让分布式系统中的所有元素或者组件都有唯一的可辨别的信息，因为极低冲突频率和高效算法的基础，它不需要集中式控制和管理唯一可辨别信息的生成，由此，每个使用者都可以自由地创建与其他人不冲突的UUID。<br />「UUID本质是一个128比特的数字」，这是一个位长巨大的数值，理论上来说，UUID的总数量为2^128个。这个数字大概可以这样估算：如果「每纳秒」产生「1兆」个不相同的UUID，需要花费超过100亿年才会用完所有的UUID。
<a name="ohAcV"></a>
### UUID的变体与版本
UUID标准和算法定义的时候，为了考虑历史兼容性和未来的扩展，提供了多种变体和版本。接下来的变体和版本描述来源于维基百科中的Versions章节和RFC 4122中的Variant章节。<br />目前已知的变体如下：

- 变体0xx：Reserved, NCS backward compatibility，为向后兼容做预留的变体
- 变体10x：The IETF aka Leach-Salz variant (used by this class)，称为Leach–Salz UUID或者IETF UUID，JDK中UUID目前正在使用的变体
- 变体110：Reserved, Microsoft Corporation backward compatibility，微软早期GUID预留变体
- 变体111：Reserved for future definition，将来扩展预留，目前还没被使用的变体

目前已知的版本如下：

- 空UUID（特殊版本0），用00000000-0000-0000-0000-000000000000表示，也就是所有的比特都是0
- date-time and MAC address（版本1）：基于时间和MAC地址的版本，通过计算当前时间戳、随机数和机器MAC地址得到。由于有MAC地址，这个可以保证其在全球的唯一性。但是使用了MAC地址，就会有MAC地址暴露问题。若是局域网，可以用IP地址代替
- date-time and MAC address, DCE security version（版本2）：分布式计算环境安全的UUID，算法和版本1基本一致，但会把时间戳的前4位置换为POSIX的UID或GID
- namespace name-based MD5（版本3）：通过计算名字和命名空间的MD5散列值得到。这个版本的UUID保证了：相同命名空间中不同名字生成的UUID的唯一性；不同命名空间中的UUID的唯一性；相同命名空间中相同名字的UUID重复生成是相同的
- random（版本4）：根据随机数，或者伪随机数生成UUID。这种UUID产生重复的概率是可以计算出来的，还有一个特点就是预留了6比特存放变体和版本属性，所以随机生成的位一共有122个，总量为2^122，比其他变体的总量要偏少
- namespace name-based SHA-1（版本5）：和版本3类似，散列算法换成了SHA-1

其中，JDK中应用的变体是Leach-Salz，提供了namespace name-based MD5（版本3）和random（版本4）两个版本的UUID生成实现。
<a name="H0xJA"></a>
### UUID的格式
在规范文件描述中，UUID是由16个8比特数字，或者说32个16进制表示形式下的字符组成，一般表示形式为8-4-4-4-12，加上连接字符-一共有36个字符，例如：
```java
## 例子
123e4567-e89b-12d3-a456-426614174000
## 通用格式
xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
```
其中4比特长度的M和1到3比特长度的N分别代表版本号和变体标识。UUID的具体布局如下：

| 属性 | 属性名 | 长度（bytes） | 长度（16进制符） | 内容 |
| --- | --- | --- | --- | --- |
| time_low | 时间戳低位 | 4 | 8 | 代表时间戳的低32<br />比特的整数表示 |
| time_mid | 时间戳中位 | 2 | 4 | 代表时间戳的中间16<br />比特的整数表示 |
| time_hi_and_version | 时间戳高位和版本号 | 2 | 4 | 高位4比特是版本号表示，剩余是时间戳的高12比特的整数表示 |
| clock_seq_hi_and_res clock_seq_low | 时钟序列与变体编号 | 2 | 4 | 最高位1到3比特表示变体编号，剩下的13到15比特表示时钟序列 |
| node | 节点ID | 6 | 12 | 48比特表示的节点ID |

基于这个表格画一个图：<br />![2021-05-19-14-53-14-892857.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621407266076-fb0b197e-773f-4da3-a4cc-a91b216715e0.png#clientId=u4c65d6cf-58f5-4&from=ui&id=u16cbc052&originHeight=136&originWidth=1080&originalType=binary&size=41729&status=done&style=none&taskId=u7bb3b1e6-811c-464e-b7c9-55e5fa74e88)<br />「严重注意，重复三次」：

- 上面提到的UUID的具体布局只适用于date-time and MAC address（版本1）和date-time and MAC address, DCE security version（版本2），其他版本虽然采用了基本一样的字段分布，但是无法获取时间戳、时钟序列或者节点ID等信息
- 上面提到的UUID的具体布局只适用于date-time and MAC address（版本1）和date-time and MAC address, DCE security version（版本2），其他版本虽然采用了基本一样的字段分布，但是无法获取时间戳、时钟序列或者节点ID等信息
- 上面提到的UUID的具体布局只适用于date-time and MAC address（版本1）和date-time and MAC address, DCE security version（版本2），其他版本虽然采用了基本一样的字段分布，但是无法获取时间戳、时钟序列或者节点ID等信息
:::info
JDK中只提供了版本3和版本4的实现，但是java.util.UUID的布局采用了上面表格的字段
:::
<a name="xPOUE"></a>
### UUID的碰撞几率计算
UUID的总量虽然巨大，但是如果不停地使用，假设每纳秒生成超过1兆个UUID并且人类有幸能够繁衍到100亿年以后，总会有可能产生重复的UUID。那么，怎么计算UUID的碰撞几率呢？这是一个数学问题，可以使用比较著名的「生日悖论」解决：<br />![2021-05-19-14-53-15-131221.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621407275734-beeeff6a-14bb-4f5b-aaf0-b3088ef12f91.png#clientId=u4c65d6cf-58f5-4&from=ui&id=u353929f9&originHeight=779&originWidth=820&originalType=binary&size=65955&status=done&style=shadow&taskId=ub7f35ba3-38f2-4757-9353-486953d4980)<br />上图来源于某搜索引擎百科。刚好维基百科上给出了碰撞几率的计算过程，其实用的也是生日悖论的计算方法，这里贴一下：<br />![2021-05-19-14-53-15-413467.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621407285743-1a8ac448-aa07-4986-81e9-c3c85b8fe890.png#clientId=u4c65d6cf-58f5-4&from=ui&id=u4f68deea&originHeight=128&originWidth=1080&originalType=binary&size=47753&status=done&style=shadow&taskId=ua1d21cf0-c6a2-4786-b887-6a03c545721)<br />上面的碰撞几率计算是基于Leach–Salz变体和版本4进行，得到的结论是：

- 103万亿个UUID中找到重复项的概率是十亿分之一
- 要生成一个冲突率达到50%的UUID至少需要生成2.71 * 1_000_000^3个UUID

有生之年不需要担心UUID冲突，出现的可能性比大型陨石撞地球还低。
<a name="boXIW"></a>
### UUID的使用场景
基本所有需要使用全局唯一标识符的场景都可以使用UUID，除非对长度有明确的限制，常用的场景包括：

- 日志框架映射诊断上下文中的TRACE_ID
- APM工具或者说OpenTracing规范中的SPAN_ID
- 特殊场景下数据库主键或者虚拟外键
- 交易ID（订单ID）
- 等等......
<a name="ZjVl3"></a>
## JDK中UUID详细介绍和使用
这里先介绍使用方式。前面提到JDK中应用的变体是Leach-Salz（变体2），提供了namespace name-based MD5（版本3）和random（版本4）两个版本的UUID生成实现，实际上`java.util.UUID`提供了四种生成UUID实例的方式：

- 最常见的就是调用静态方法`UUID#randomUUID()`，这就是版本4的静态工厂方法
- 其次是调用静态方法`UUID#nameUUIDFromBytes(byte[] name)`，这就是版本3的静态工厂方法
- 另外有调用静态方法`UUID#fromString(String name)`，这是解析8-4-4-4-12格式字符串生成UUID实例的静态工厂方法
- 还有低层次的构造函数`UUID(long mostSigBits, long leastSigBits)`，这个对于使用者来说并不常见

最常用的方法有实例方法`toString()`，把UUID转化为16进制字符串拼接而成的8-4-4-4-12形式表示，例如：
```java
String uuid = UUID.randomUUID().toString();
```
其他Getter方法：
```java
UUID uuid = UUID.randomUUID();
// 返回版本号
int version = uuid.version();
// 返回变体号
int variant = uuid.variant();
// 返回时间戳 - 这个方法会报错，只有Time-based UUID也就是版本1或者2的UUID实现才能返回时间戳
long timestamp = uuid.timestamp();
// 返回时钟序列 - 这个方法会报错，只有Time-based UUID也就是版本1或者2的UUID实现才能返回时钟序列
long clockSequence = uuid.clockSequence();
// 返回节点ID - 这个方法会报错，只有Time-based UUID也就是版本1或者2的UUID实现才能返回节点ID
long nodeId = uuid.node();

可以验证一下不同静态工厂方法的版本和变体号：
UUID uuid = UUID.randomUUID();
int version = uuid.version();
int variant = uuid.variant();
System.out.println(String.format("version:%d,variant:%d", version, variant));
uuid = UUID.nameUUIDFromBytes(new byte[0]);
version = uuid.version();
variant = uuid.variant();
System.out.println(String.format("version:%d,variant:%d", version, variant));
// 输出结果
version:4,variant:2
version:3,variant:2
```
<a name="RRfZr"></a>
## 探究JDK中UUID源码实现
java.util.UUID被final修饰，实现了Serializable和Comparable接口，从一般理解上看，有下面的特定：

- 不可变，一般来说工具类都是这样定义的
- 可序列化和反序列化
- 不同的对象之间可以进行比较，比较方法后面会分析

下面会从不同的方面分析一下java.util.UUID的源码实现：

- 属性和构造函数
- 随机数版本实现
- namespace name-based MD5版本实现
- 其他实现
- 格式化输出
- 比较相关的方法
<a name="MiQZT"></a>
### 属性和构造函数
前面反复提到JDK中只提供了版本3和版本4的实现，但是java.util.UUID的布局采用了UUID规范中的字段定义，长度一共128比特，刚好可以存放在两个long类型的整数中，所以看到了UUID类中存在两个long类型的整型数值：
```java
public final class UUID implements java.io.Serializable, Comparable<UUID> {
   
    // 暂时省略其他代码

    /*
     * The most significant 64 bits of this UUID.
     * UUID中有效的高64比特
     *
     * @serial
     */
    private final long mostSigBits;

    /*
     * The least significant 64 bits of this UUID.
     *  UUID中有效的低64比特
     *
     * @serial
     */
    private final long leastSigBits;
    
    // 暂时省略其他代码
}
```
从UUID类注释中可以看到具体的字段布局如下：<br />「高64比特mostSigBits的布局」

| 字段 | bit长度 | 16进制字符长度 |
| --- | --- | --- |
| time_low | 32 | 8 |
| time_mid | 16 | 4 |
| version | 4 | 1 |
| time_hi | 12 | 3 |

「低64比特leastSigBits的布局」

| 字段 | bit长度 | 16进制字符长度 |
| --- | --- | --- |
| variant | 2 | 小于1 |
| clock_seq | 14 | variant和clock_seq加起来等于4 |
| node | 48 | 12 |

接着看UUID的其他成员属性和构造函数：
```java
public final class UUID implements java.io.Serializable, Comparable<UUID> {
   
    // 暂时省略其他代码
    
    // Java语言访问类，里面存放了很多底层相关的访问或者转换方法，在UUID中主要是toString()实例方法用来格式化成8-4-4-4-12的形式，委托到Long.fastUUID()方法
    private static final JavaLangAccess jla = SharedSecrets.getJavaLangAccess();

    // 静态内部类确保SecureRandom初始化，用于版本4的随机数UUID版本生成安全随机数
    private static class Holder {
        static final SecureRandom numberGenerator = new SecureRandom();
    }
    
    // 通过长度为16的字节数组，计算mostSigBits和leastSigBits的值初始化UUID实例
    private UUID(byte[] data) {
        long msb = 0;
        long lsb = 0;
        assert data.length == 16 : "data must be 16 bytes in length";
        for (int i=0; i<8; i++)
            msb = (msb << 8) | (data[i] & 0xff);
        for (int i=8; i<16; i++)
            lsb = (lsb << 8) | (data[i] & 0xff);
        this.mostSigBits = msb;
        this.leastSigBits = lsb;
    }
    
    // 直接指定mostSigBits和leastSigBits构造UUID实例
    public UUID(long mostSigBits, long leastSigBits) {
        this.mostSigBits = mostSigBits;
        this.leastSigBits = leastSigBits;
    }

    // 暂时省略其他代码
}
```
私有构造`private UUID(byte[] data)`中有一些位运算技巧：
```java
long msb = 0;
long lsb = 0;
assert data.length == 16 : "data must be 16 bytes in length";
for (int i=0; i<8; i++)
    msb = (msb << 8) | (data[i] & 0xff);
for (int i=8; i<16; i++)
    lsb = (lsb << 8) | (data[i] & 0xff);
this.mostSigBits = msb;
this.leastSigBits = lsb;
```
输入的字节数组长度为16，mostSigBits由字节数组的前8个字节转换而来，而leastSigBits由字节数组的后8个字节转换而来。中间变量msb或者lsb在提取字节位进行计算的时候：

- 先进行左移8位确保需要计算的位为0，已经计算好的位移动到左边
- 然后右边需要提取的字节data[i]的8位会先和0xff（补码1111 1111）进行或运算，确保不足8位的高位被补充为0，超过8位的高位会被截断为低8位，也就是data[i] & 0xff确保得到的补码为8位
- 前面两步的结果再进行或运算

一个模拟过程如下：
```java
（为了区分明显，每4位加了一个下划线）

（为了简答，只看字节数组的前4个字节，同时只看long类型的前4个字节）

0xff === 1111_1111

long msb = 0  => 0000_0000 0000_0000 0000_0000 0000_0000

byte[] data
0000_0001 0000_0010 0000_0100 0000_1000

i = 0（第一轮）
msb << 8 = 0000_0000 0000_0000 0000_0000 0000_0000
data[i] & 0xff = 0000_0001 & 1111_1111 = 0000_0001
(msb << 8) | (data[i] & 0xff) = 0000_0000 0000_0000 0000_0000 0000_0001

（第一轮 msb = 0000_0000 0000_0000 0000_0000 0000_0001）

i = 1（第二轮）
msb << 8 = 0000_0000 0000_0000 0000_0001 0000_0000
data[i] & 0xff = 0000_0010 & 1111_1111 = 0000_0010
(msb << 8) | (data[i] & 0xff) = 0000_0000 0000_0000 0000_0001 0000_0010

（第二轮 msb = 0000_0000 0000_0000 0000_0001 0000_0010）

i = 2（第三轮）
msb << 8 = 0000_0000 0000_0001 0000_0010 0000_0000
data[i] & 0xff = 0000_0100 & 1111_1111 = 0000_0100
(msb << 8) | (data[i] & 0xff) = 0000_0000 0000_0001 0000_0010 0000_0100

（第三轮 msb = 0000_0000 0000_0001 0000_0010 0000_0100）

i = 3（第四轮）
msb << 8 = 0000_0001 0000_0010 0000_0100 0000000
data[i] & 0xff = 0000_1000 & 1111_1111 = 0000_1000
(msb << 8) | (data[i] & 0xff) = 0000_0001 0000_0010 0000_0100 0000_1000

（第四轮 msb = 0000_0001 0000_0010 0000_0100 0000_1000）
```
以此类推，这个私有构造函数执行完毕后，长度为16的字节数组的所有位就会转移到mostSigBits和leastSigBits中。
<a name="zp9RH"></a>
### 随机数版本实现
构造函数分析完，接着分析重磅的静态工厂方法`UUID#randomUUID()`，这是使用频率最高的一个方法：
```java
public static UUID randomUUID() {
    // 静态内部类Holder持有的SecureRandom实例，确保提前初始化
    SecureRandom ng = Holder.numberGenerator;
    // 生成一个16字节的安全随机数，放在长度为16的字节数组中
    byte[] randomBytes = new byte[16];
    ng.nextBytes(randomBytes);
    // 清空版本号所在的位，重新设置为4
    randomBytes[6]  &= 0x0f;  /* clear version        */
    randomBytes[6]  |= 0x40;  /* set to version 4     */
    // 清空变体号所在的位，重新设置为
    randomBytes[8]  &= 0x3f;  /* clear variant        */
    randomBytes[8]  |= 0x80;  /* set to IETF variant  */
    return new UUID(randomBytes);
}
```
关于上面的位运算，这里可以使用极端的例子进行推演：
```java
假设randomBytes[6] = 1111_1111
// 清空version位
randomBytes[6] &= 0x0f => 1111_1111 & 0000_1111 = 0000_1111
得到randomBytes[6] = 0000_1111 （这里可见高4比特被清空为0）
// 设置version位为整数4 => 十六进制0x40 => 二级制补码0100_0000
randomBytes[6] |= 0x40 => 0000_1111 | 0100_0000 = 0100_1111
得到randomBytes[6] = 0100_1111

结果：version位 => 0100（4 bit）=> 对应十进制数4

同理

假设randomBytes[8] = 1111_1111
// 清空variant位
randomBytes[8] &= 0x3f => 1111_1111 & 0011_1111 = 0011_1111
// 设置variant位为整数128 => 十六进制0x80 => 二级制补码1000_0000 （这里取左边高位2位）
randomBytes[8] |= 0x80 => 0011_1111 | 1000_0000 = 1011_1111

结果：variant位 => 10（2 bit）=> 对应十进制数2
```
关于UUID里面的Getter方法例如`version()`、`variant()`其实就是找到对应的位，并且转换为十进制整数返回，如果熟练使用位运算，应该不难理解，后面不会分析这类的Getter方法。<br />「随机数版本实现强依赖于SecureRandom生成的随机数（字节数组）」。`SecureRandom`的引擎提供者可以从`sun.security.provider.SunEntries`中查看，对于不同系统版本的JDK实现会选用不同的引擎，常见的如NativePRNG。JDK11配置文件`$JAVA_HOME/conf/security/java.security`中的`securerandom.source`属性用于指定系统默认的随机源：<br />![2021-05-19-14-53-17-355317.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621407301249-e58a4aa7-3ab2-4ea4-9640-ee244cef6b32.png#clientId=u4c65d6cf-58f5-4&from=ui&id=u5c96b59c&originHeight=1211&originWidth=912&originalType=binary&size=144902&status=done&style=none&taskId=u8fe88f2a-34c1-4839-b788-74325c436fa)<br />这里要提一个小知识点，想要得到密码学意义上的安全随机数，可以直接使用真随机数产生器产生的随机数，或者使用真随机数产生器产生的随机数做种子。通过查找一些资料得知「非物理真随机数产生器」有：

- Linux操作系统的/dev/random设备接口
- Windows操作系统的`CryptGenRandom`接口

如果不修改java.security配置文件，默认随机数提供引擎会根据不同的操作系统选用不同的实现，这里不进行深究。在Linux环境下，`SecureRandom`实例化后，不通过`setSeed()`方法设置随机数作为种子，默认就是使用/dev/random提供的安全随机数接口获取种子，产生的随机数是密码学意义上的安全随机数。「一句话概括，UUID中的私有静态内部类Holder中的`SecureRandom`实例可以产生安全随机数，这个是JDK实现UUID版本4的一个重要前提」。这里总结一下随机数版本UUID的实现步骤：

- 通过`SecureRandom`依赖提供的安全随机数接口获取种子，生成一个16字节的随机数（字节数组）
- 对于生成的随机数，清空和重新设置`version`和`variant`对应的位
- 把重置完`version`和`variant`的随机数的所有位转移到`mostSigBits`和`leastSigBits`中
<a name="AuvKB"></a>
### namespace name-based MD5版本实现
接着分析版本3也就是namespace name-based MD5版本的实现，对应于静态工厂方法`UUID#nameUUIDFromBytes()`：
```java
public static UUID nameUUIDFromBytes(byte[] name) {
    MessageDigest md;
    try {
        md = MessageDigest.getInstance("MD5");
    } catch (NoSuchAlgorithmException nsae) {
        throw new InternalError("MD5 not supported", nsae);
    }
    byte[] md5Bytes = md.digest(name);
    md5Bytes[6]  &= 0x0f;  /* clear version        */
    md5Bytes[6]  |= 0x30;  /* set to version 3     */
    md5Bytes[8]  &= 0x3f;  /* clear variant        */
    md5Bytes[8]  |= 0x80;  /* set to IETF variant  */
    return new UUID(md5Bytes);
}
```
它的后续基本处理和随机数版本基本一致（清空版本位的时候，重新设置为3），唯一明显不同的地方就是生成原始随机数的时候，采用的方式是：基于输入的name字节数组，通过MD5摘要算法生成一个MD5摘要字节数组作为原始安全随机数，返回的这个随机数刚好也是16字节长度的。使用方式很简单：
```java
UUID uuid = UUID.nameUUIDFromBytes("throwable".getBytes());
```
namespace name-based MD5版本UUID的实现步骤如下：

- 通过输入的命名字节数组基于MD5算法生成一个16字节长度的随机数
- 对于生成的随机数，清空和重新设置version和variant对应的位
- 把重置完version和variant的随机数的所有位转移到mostSigBits和leastSigBits中

namespace name-based MD5版本的UUID强依赖于MD5算法，有个明显的特征是如果输入的byte[] name一致的情况下，会产生完全相同的UUID实例。
<a name="RvQ2t"></a>
### 其他实现
其他实现主要包括：
```java
// 完全定制mostSigBits和leastSigBits，可以参考UUID标准字段布局进行设置，也可以按照自行制定的标准
public UUID(long mostSigBits, long leastSigBits) {
    this.mostSigBits = mostSigBits;
    this.leastSigBits = leastSigBits;
}

// 基于字符串格式8-4-4-4-12的UUID输入，重新解析出mostSigBits和leastSigBits，这个静态工厂方法也不常用，里面的位运算也不进行详细探究
public static UUID fromString(String name) {
    int len = name.length();
    if (len > 36) {
        throw new IllegalArgumentException("UUID string too large");
    }
    int dash1 = name.indexOf('-', 0);
    int dash2 = name.indexOf('-', dash1 + 1);
    int dash3 = name.indexOf('-', dash2 + 1);
    int dash4 = name.indexOf('-', dash3 + 1);
    int dash5 = name.indexOf('-', dash4 + 1);
    if (dash4 < 0 || dash5 >= 0) {
        throw new IllegalArgumentException("Invalid UUID string: " + name);
    }
    long mostSigBits = Long.parseLong(name, 0, dash1, 16) & 0xffffffffL;
    mostSigBits <<= 16;
    mostSigBits |= Long.parseLong(name, dash1 + 1, dash2, 16) & 0xffffL;
    mostSigBits <<= 16;
    mostSigBits |= Long.parseLong(name, dash2 + 1, dash3, 16) & 0xffffL;
    long leastSigBits = Long.parseLong(name, dash3 + 1, dash4, 16) & 0xffffL;
    leastSigBits <<= 48;
    leastSigBits |= Long.parseLong(name, dash4 + 1, len, 16) & 0xffffffffffffL;
    return new UUID(mostSigBits, leastSigBits);
}
```
<a name="rXgAY"></a>
### 格式化输出
格式化输出体现在`UUID#toString()`方法，这个方法会把`mostSigBits`和`leastSigBits`格式化为8-4-4-4-12的形式，这里详细分析一下格式化的过程。首先从注释上看格式是：
```java
<time_low>-<time_mid>-<time_high_and_version>-<variant_and_sequence>-<node>

time_low = 4 * <hexOctet> => 4个16进制8位字符
time_mid = 2 * <hexOctet> => 2个16进制8位字符
time_high_and_version = 4 * <hexOctet> => 2个16进制8位字符
variant_and_sequence = 4 * <hexOctet> => 2个16进制8位字符
node = 4 * <hexOctet> => 6个16进制8位字符

hexOctet = <hexDigit><hexDigit>（2个hexDigit）
hexDigit = 0-9a-F（其实就是16进制的字符）
```
和前文布局分析时候的提到的内容一致。`UUID#toString()`方法源码如下：
```java
private static final JavaLangAccess jla = SharedSecrets.getJavaLangAccess();


public String toString() {
    return jla.fastUUID(leastSigBits, mostSigBits);
}

↓↓↓↓↓↓↓↓↓↓↓↓

// java.lang.System
private static void setJavaLangAccess() {

    SharedSecrets.setJavaLangAccess(new JavaLangAccess() {
    
        public String fastUUID(long lsb, long msb) {
            return Long.fastUUID(lsb, msb);
        }
}

↓↓↓↓↓↓↓↓↓↓↓↓
// java.lang.Long
static String fastUUID(long lsb, long msb) {
    // COMPACT_STRINGS在String类中默认为true，所以会命中if分支
    if (COMPACT_STRINGS) {
        // 初始化36长度的字节数组 
        byte[] buf = new byte[36];
        // lsb的低48位转换为16进制格式写入到buf中 - node => 位置[24,35]
        formatUnsignedLong0(lsb,        4, buf, 24, 12);
        // lsb的高16位转换为16进制格式写入到buf中 - variant_and_sequence  => 位置[19,22]
        formatUnsignedLong0(lsb >>> 48, 4, buf, 19, 4);
        // msb的低16位转换为16进制格式写入到buf中 - time_high_and_version => 位置[14,17]
        formatUnsignedLong0(msb,        4, buf, 14, 4); 
        // msb的中16位转换为16进制格式写入到buf中 - time_mid => 位置[9,12]
        formatUnsignedLong0(msb >>> 16, 4, buf, 9,  4);
        // msb的高32位转换为16进制格式写入到buf中 - time_low => 位置[0,7]
        formatUnsignedLong0(msb >>> 32, 4, buf, 0,  8);
        // 空余的字节槽位插入'-'，刚好占用了4个字节
        buf[23] = '-';
        buf[18] = '-';
        buf[13] = '-';
        buf[8]  = '-';
        // 基于处理好的字节数组，实例化String，并且编码指定为LATIN1
        return new String(buf, LATIN1);
    } else {
        byte[] buf = new byte[72];
        formatUnsignedLong0UTF16(lsb,        4, buf, 24, 12);
        formatUnsignedLong0UTF16(lsb >>> 48, 4, buf, 19, 4);
        formatUnsignedLong0UTF16(msb,        4, buf, 14, 4);
        formatUnsignedLong0UTF16(msb >>> 16, 4, buf, 9,  4);
        formatUnsignedLong0UTF16(msb >>> 32, 4, buf, 0,  8);
        StringUTF16.putChar(buf, 23, '-');
        StringUTF16.putChar(buf, 18, '-');
        StringUTF16.putChar(buf, 13, '-');
        StringUTF16.putChar(buf,  8, '-');
        return new String(buf, UTF16);
    }
}

/**
 * 格式化无符号的长整型，填充到字节缓冲区buf中，如果长度len超过了输入值的ASCII格式表示，则会使用0进行填充
 * 这个方法就是把输入长整型值val，对应一段长度的位，填充到字节数组buf中，len控制写入字符的长度，offset控制写入buf的起始位置
 * 而shift参数决定基础格式，4是16进制，1是2进制，3是8位
 */
static void formatUnsignedLong0(long val, int shift, byte[] buf, int offset, int len) {
    int charPos = offset + len;
    int radix = 1 << shift;
    int mask = radix - 1;
    do {
        buf[--charPos] = (byte)Integer.digits[((int) val) & mask];
        val >>>= shift;
    } while (charPos > offset);
}
```
<a name="UFbrU"></a>
### 比较相关的方法
比较相关方法如下：
```java
// hashCode方法基于mostSigBits和leastSigBits做异或得出一个中间变量hilo，再以32为因子进行计算
public int hashCode() {
    long hilo = mostSigBits ^ leastSigBits;
    return ((int)(hilo >> 32)) ^ (int) hilo;
}

// equals为实例对比方法，直接对比两个UUID的mostSigBits和leastSigBits值，完全相等的时候返回true
public boolean equals(Object obj) {
    if ((null == obj) || (obj.getClass() != UUID.class))
        return false;
    UUID id = (UUID)obj;
    return (mostSigBits == id.mostSigBits &&
            leastSigBits == id.leastSigBits);
}

// 比较规则是mostSigBits高位大者为大，高位相等的情况下，leastSigBits大者为大
public int compareTo(UUID val) {
    // The ordering is intentionally set up so that the UUIDs
    // can simply be numerically compared as two numbers
    return (this.mostSigBits < val.mostSigBits ? -1 :
            (this.mostSigBits > val.mostSigBits ? 1 :
                (this.leastSigBits < val.leastSigBits ? -1 :
                (this.leastSigBits > val.leastSigBits ? 1 :
                0))));
}
```
所有比较方法仅仅和`mostSigBits`和`leastSigBits`有关，毕竟这两个长整型就存储了UUID实例的所有信息。
