JavaHessian
<a name="YoNlm"></a>
## 问题和思考

1. **序列化参数有枚举属性，序列化端增加一个枚举，能否正常反序列化？**
2. **序列化子类，它和父类有同名参数，反序列化时，同名参数能否能正常赋值？**
3. **序列化对象增加参数，反序列化类不增加参数，能否正常反序列化？**
4. **用于序列化传输的属性，用包装器比较好，还是基本类型比较好？**
<a name="pQxgQ"></a>
## 为什么要使用序列化和反序列化

1. **程序在运行过程中，产生的数据，不能一直保存在内存中，需要暂时或永久存储到介质（如磁盘、数据库、文件）里进行保存，也可能通过网络发送给协作者。程序获取原数据，需要从介质，或网络传输获得。传输的过程中，只能使用二进制流进行传输。**
2. **简单的场景，基本类型数据传输。通过双方约定好参数类型，数据接收方按照既定规则对二进制流进行反序列化。**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178872793-946191d8-cc3c-43a8-883a-3712af31bc3b.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=ua23e34b6&originHeight=239&originWidth=715&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub82863b7-f7d2-482b-b7e2-9e4087485e7&title=)

1. **复杂的场景，传输数据的参数类型可能包括：基本类型、包装器类型、自定义类、枚举、时间类型、字符串、容器等。很难简单通过约定来反序列化二进制流。需要一个通用的协议，共双方使用，进行序列化和反序列化。**
<a name="uPGZ3"></a>
## 三种序列化协议及对比
| **序列化协议** | **特点** |
| --- | --- |
| jdk<br />（jdk 自带） | 1. 序列化：除了 static、transient类型<br />2. 特点：强类型，安全性高，序列化结果携带类型信息<br />3. 反序列化：基于 Field 机制<br />4. 应用场景：深拷贝 |
| fastjson<br />（第三方实现） | 1. 可读性好，空间占用小<br />2. 特点：弱类型，序列化结果不携带类型信息，可读性强。有一些安全性问题<br />3. 反序列化：基于 Field 机制，兼容 Bean 机制<br />4. 应用场景：消息、透传对象 |
| hessian<br />（第三方实现） | 1. 序列化：除了 static、transient 类型<br />2. 特点：强类型，体积小，可跨语言，序列化结果携带类型信息<br />3. 反序列化：基于 Field 机制，兼容 Bean 机制<br />4. 应用场景：RPC |

<a name="PAPIw"></a>
### 对比
```java
Father father = new Father();
father.name = "厨师";
father.comment = "川菜馆";
father.simpleInt = 1;
father.boxInt = new Integer(10);
father.simpleDouble = 1;
father.boxDouble = new Double(10);
father.bigDecimal = new BigDecimal(11.5);
```
<a name="Ix3Ek"></a>
#### 运行结果：
```java
jdk序列化结果长度：626，耗时：55
jdk反序列化结果：Father{version=0, name='厨师', comment='川菜馆', boxInt=10, simpleInt=1, boxDouble=10.0, simpleDouble=1.0, bigDecimal=11.5}耗时：87

hessian序列化结果长度：182，耗时：56
hessian反序列化结果：Father{version=0, name='厨师', comment='川菜馆', boxInt=10, simpleInt=1, boxDouble=10.0, simpleDouble=1.0, bigDecimal=11.5}耗时：7

Fastjson序列化结果长度：119，耗时：225
Fastjson反序列化结果：Father{version=0, name='厨师', comment='川菜馆', boxInt=10, simpleInt=1, boxDouble=10.0, simpleDouble=1.0, bigDecimal=11.5}耗时：69
```
<a name="qeL0X"></a>
#### 分析：

- **jdk 序列化耗时最短，但是序列化结果长度最长，是其它两种的 3 ～ 5 倍。**
- **fastjson 序列化结果长度最短，但是耗时是其它两种的 4 倍左右。**
- **hessian 序列化耗时与 jdk 差别不大，远小于 fastjson 序列化耗时。且与 jdk 相比，序列化结果占用空间非常有优势。另外，hessian 的反序列化速度最快，耗时是其它两种的 1/10。**
- **综上比较，hessian 在序列化和反序列化表现中，性能最优。**
<a name="Yw5cp"></a>
## Hessian 序列化实战
<a name="jLYZf"></a>
### 实验准备
<a name="p1GkH"></a>
#### 父类
```java
public class Father implements Serializable {

    /**
     * 静态类型不会被序列化
     */
    private static final long serialVersionUID = 1L;

    /**
     * transient 不会被序列化
     */
    transient int version = 0;

    /**
     * 名称
     */
    public String name;

    /**
     * 备注
     */
    public String comment;

    /**
     * 包装器类型1
     */
    public Integer boxInt;

    /**
     * 基本类型1
     */
    public int simpleInt;

    /**
     * 包装器类型2
     */
    public Double boxDouble;

    /**
     * 基本类型2
     */
    public double simpleDouble;

    /**
     * BigDecimal
     */
    public BigDecimal bigDecimal;

    public Father() {
    }

    @Override
    public String toString() {
        return "Father{" +
                "version=" + version +
                ", name='" + name + '\'' +
                ", comment='" + comment + '\'' +
                ", boxInt=" + boxInt +
                ", simpleInt=" + simpleInt +
                ", boxDouble=" + boxDouble +
                ", simpleDouble=" + simpleDouble +
                ", bigDecimal=" + bigDecimal +
                '}';
    }
}
```
<a name="dAQKS"></a>
#### 子类
```java
public class Son extends Father {

    /**
     * 名称，与father同名属性
     */
    public String name;

    /**
     * 自定义类
     */
    public Attributes attributes;

    /**
     * 枚举
     */
    public Color color;

    public Son() {
    }

}
```
<a name="v2NOY"></a>
#### 属性-自定义类
```java
public class Attributes implements Serializable {

    private static final long serialVersionUID = 1L;

    public int value;

    public String msg;

    public Attributes() {
    }

    public Attributes(int value, String msg) {
        this.value = value;
        this.msg = msg;
    }

}
```
<a name="K8ygU"></a>
#### 枚举
```java
public enum Color {

    RED(1, "red"),
    YELLOW(2, "yellow")
    ;

    public int value;

    public String msg;

    Color() {
    }

    Color(int value, String msg) {
        this.value = value;
        this.msg = msg;
    }
}
```
**使用到的对象及属性设置**
```java
Son son = new Son();
son.name = "厨师";    // 父子类同名字段，只给子类属性赋值
son.comment = "川菜馆";
son.simpleInt = 1;
son.boxInt = new Integer(10);
son.simpleDouble = 1;
son.boxDouble = new Double(10);
son.bigDecimal = new BigDecimal(11.5);
son.color = Color.RED;
son.attributes = new Attributes(11, "hello");
```
<a name="lQDuj"></a>
### 运行结果分析
使用 Hessian 序列化，结果写入文件，使用 vim 打开。使用 16 进制方式查看，查看命令：`%!xxd`
```java
00000000: 4307 6474 6f2e 536f 6e9a 046e 616d 6504  C.dto.Son..name.
00000010: 6e61 6d65 0763 6f6d 6d65 6e74 0662 6f78  name.comment.box
00000020: 496e 7409 7369 6d70 6c65 496e 7409 626f  Int.simpleInt.bo
00000030: 7844 6f75 626c 650c 7369 6d70 6c65 446f  xDouble.simpleDo
00000040: 7562 6c65 0a61 7474 7269 6275 7465 7305  uble.attributes.
00000050: 636f 6c6f 720a 6269 6744 6563 696d 616c  color.bigDecimal
00000060: 6002 e58e a8e5 b888 4e03 e5b7 9de8 8f9c  `.......N.......
00000070: e9a6 869a 915d 0a5c 430e 6474 6f2e 4174  .....].\C.dto.At
00000080: 7472 6962 7574 6573 9205 7661 6c75 6503  tributes..value.
00000090: 6d73 6761 9b05 6865 6c6c 6f43 0964 746f  msga..helloC.dto
000000a0: 2e43 6f6c 6f72 9104 6e61 6d65 6203 5245  .Color..nameb.RE
000000b0: 4443 146a 6176 612e 6d61 7468 2e42 6967  DC.java.math.Big
000000c0: 4465 6369 6d61 6c91 0576 616c 7565 6304  Decimal..valuec.
000000d0: 3131 2e35 0a                             11.5.
```
对其中的十六进制数逐个分析，可以拆解为一下结构：参考 hessian 官方文档，链接：[http://hessian.caucho.com/doc/hessian-serialization.html](http://hessian.caucho.com/doc/hessian-serialization.html)
<a name="ZJ8nd"></a>
### 序列化原理
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178872741-2bac38a9-8b21-4a8f-9d44-b3bec369565d.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u4bf9ccd1&originHeight=483&originWidth=781&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufbf80042-d39d-4e08-a0d4-e3bfe47751b&title=)
<a name="vDYBe"></a>
#### 序列化规则：

1. **被序列化的类必须实现了 Serializable 接口**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178872786-0d67115e-0d66-45e4-ba7f-2475712e9259.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=ub94ee070&originHeight=317&originWidth=995&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u78ce0502-a512-4427-a5c5-06d61b8630c&title=)

1. **静态属性和 transient 变量，不会被序列化。**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178872778-13f00b85-a1b3-48b4-8a6d-c1a038100cd4.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=uaea1a970&originHeight=818&originWidth=850&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u19814ff4-647a-47d2-a5b4-efbebeeb4eb&title=)

1. **枚举类型在序列化后，存储的是枚举变量的名字**
2. **序列化结果的结构：类定义开始标识 C -> 类名长度+类名 -> 属性数量 -> （逐个）属性名长度+属性名 -> 开始实例化标识 -> （按照属性名顺序，逐个设置）属性值（发现某个属性是一个对象，循环这个过程）**

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178872742-8b8fa467-82e8-4df3-9672-aa194c3cb587.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=uc879762b&originHeight=645&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8960efae-b123-4727-ac37-23bb372c9e6&title=)
<a name="JYEXI"></a>
### 反序列化
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873197-ec90c662-f0e1-4bd3-8d68-4d1125dace82.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u7fc88f94&originHeight=662&originWidth=841&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u319831ce-8c32-447a-a7da-2c98227a558&title=)<br />**通俗原理图：**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873243-03560072-d8db-46ef-9777-d1db581abf8b.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=ua37d59c1&originHeight=399&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u793c78b0-3d44-49c8-a921-14600ec8ade&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873301-b9820b72-7897-4ac9-b2aa-7fceea84896b.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=ua2d6a5dd&originHeight=638&originWidth=1004&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3dd3da4e-355b-4a0f-ad68-09827463f06&title=)<br />解释：这是前边的序列化文件，可以对着这个结构理解反序列化的过程。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873200-e44358e2-e19c-4a81-ba67-1c7654ed22ad.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u291cd846&originHeight=463&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua6e51a63-3cd7-42f6-b5c8-5a63c4e311b&title=)<br />解释：读取到“C”之后，它就知道接下来是一个类的定义，接着就开始读取类名，属性个数和每个属性的名称。并把这个类的定义缓存到一个_classDefs 的 list 里。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873322-f92c1687-1685-41b2-ad96-869a0712f73c.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u296fee18&originHeight=197&originWidth=635&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0fcc1bd2-7ce7-4e72-a7ec-4536962ef4a&title=)<br />解释：通过读取序列化文件，获得类名后，会加载这个类，并生成这个类的反序列化器。这里会生成一个_fieldMap，key 为反序列化端这个类所有属性的名称，value 为属性对应的反序列化器。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873529-6a32b7f2-4b87-420a-8e05-0df3ec7d40ed.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u7f8a7900&originHeight=284&originWidth=788&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua98f4184-7134-4c7c-9a2f-cc72b9b5908&title=)<br />解释：读到 6 打头的 2 位十六进制数时，开始类的实例化和赋值。
<a name="V769g"></a>
#### 遗留问题解答：
<a name="VeEHo"></a>
##### 增加枚举类型，反序列化不能正常读取。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873636-0c887c55-c2b6-4a29-a92c-79845ef81c40.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u195c0235&originHeight=557&originWidth=916&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud173a314-6432-47a8-923b-37a1e64fec0&title=)<br />**原因：枚举类型序列化结果中，枚举属性对应的值是枚举名。反序列化时，通过枚举类类名+枚举名反射生成枚举对象。枚举名找不到就会报错。**
<a name="WnCBg"></a>
##### 反序列化为子类型，同名属性值无法正常赋值。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873728-e19e3201-d6e2-46a2-bb59-3bf700373c4d.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=uead6c043&originHeight=608&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u360b9d31-917d-4d8f-83d9-0b806ae96ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873733-e810a25e-ad88-41f3-8ba9-080c8c12b785.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=udb769ddb&originHeight=92&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u28bcdde6-c11d-4b0c-a86f-9127c7091ce&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873804-8d9f9f8d-01c8-4ecd-8621-c7b8e883b38e.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u43050640&originHeight=282&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u268ebf6f-d945-49c2-a353-d0f92637bec&title=)
<a name="knOM5"></a>
##### 序列化对象增加参数，反序列化可以正常运行。
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661178873912-dcbceabe-7438-4301-868b-5b47f95daabb.png#clientId=u1ae5bbd9-bd28-4&from=paste&id=u8b6f0831&originHeight=176&originWidth=507&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u18d1e50f-a800-46d5-be90-361290820c1&title=)<br />**原因：反序列化时，是先通过类名加载同名类，并生成同名类的反序列化器，同名类每个属性对应的反序列化器存储在一个 map 中。在反序列化二进制文件时，通过读取到的属性名，到 map 中获取对应的反序列化器。若获取不到，默认是 **`**NullFieldDeserializer.DESER**`**。待到读值的时候，仅读值，不作 set 操作**<br />**序列化和反序列化双方都使用对象类型时，更改属性类型，若序列化方不传输数据，序列化结果是‘N’，能正常反序列化。但是对于一方是基本类型，更改属性类型后，因为 hessian 对于基本类型使用不同范围的值域，所以无法正常序列化。**<br />**参考文档：**

- [**https://paper.seebug.org/1131/**](https://paper.seebug.org/1131/)
- **hessian 官方文档：序列化规则**[**http://hessian.caucho.com/doc/hessian-serialization.html#anchor10**](http://hessian.caucho.com/doc/hessian-serialization.html#anchor10)
