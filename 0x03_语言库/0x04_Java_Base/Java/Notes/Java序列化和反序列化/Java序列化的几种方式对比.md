Java 序列化
<a name="AMyjZ"></a>
# what
<a name="An4cI"></a>
## 什么是序列化
首先要明白，序列化它是一个过程，什么过程呢？<br />把一个java对象转化成字节序列的过程<br />java对象都知道，那什么是字节序列呢？<br />字节，也就是byte，1byte = 8bit，也就是一个字节等于8位，每一位都是用0或者1来表示，在内存中，数据就是以二进制的形式存储的<br />那序列呢？简单看来说就好比排队，一列一列的，至此，字节序列，是不是就是像字节在排队一样，而字节又是一个个的8bit，理解了吧！<br />所以，Java中的序列化就是把Java对象变成二进制内容的一个过程，也就是从内存中把数据存储下来，而数据在内存中都是二进制的形式！<br />记住了，Java序列化出来的东西就是一个二进制内容，就是对象在内存中的存储形式！
<a name="SdunL"></a>
## 变量存储角度理解序列化
接下来再以变量在内存中的存储为例去理解什么是序列化！<br />举一个例子，比如写一个变量：
```java
String name = "张三";
```
也就是刚开始，把name的值设置成为”张三“，在后面的程序当中，可以修改这个name，比如又把name的值修改成为”李四“！<br />程序的运行，需要把数据加载进内存才可以，也就是说，无论是最开始的”张三“还是后来的”李四“都会被加载进内存运行，那内存都会为这些变量分配内存！<br />可是，一旦程序执行完毕，变量所分配或者说所占用的内存就会被回收， 程序也就结束了，不过在这个过程中，可以从内存中把这些变量存储下来，那这个过程就叫做序列化！
<a name="H0KjU"></a>
## 反序列化
当理解了什么是序列化之后，那反序列化也就不是问题了，自然而然的就懂了，所谓的反序列化用稍微专业点的话说就是：<br />把字节序列还原成对象的过程<br />由此可见，无论序列化还是反序列化，都是对象和字节序列之间的互相转换！
<a name="JtIaI"></a>
## 序列化的多样性
以上得知，序列化是一个对象和字节序列互相转换的过程，那随之而来的一个问题就是，该怎么转换？<br />也就是该如何实现把对象序列化成字节序列，然后再把字节序列反序列化成对象，这其中必然存在一种规则，序列化和反序列化都必须按照这个规则来！<br />那这个规则就是序列化协议，那由此基本可以得出，可能存在不同的序列化协议，然后有不同的方式去实现序列化。<br />也就是不管怎么处理，最终实现的目的是对象和字节序列的互相转换即可，比如Java就有其自己实现的一套序列化机制，可以把Java对象序列化成字节序列，还可以把自己序列再通过反序列化还原成原来的对象！<br />除了Java，像熟知的json也有其自己的序列化技术，加入用Java的序列化技术把一个对象序列化成了字节序列，那用json的反序列化技术是无法将其还原成原本的Java对象的！
<a name="JXOJc"></a>
# how
<a name="klSVt"></a>
## Java序列化demo演示
比如定义一个简单的Person类，包含以下简单属性：
```java
private String name;
private int age;
```
接着就可以将其序列化，具体操作如下：
```java
FileOutputStream outputStream = new FileOutputStream("C:\\Users\\ithuangqing\\desktop\\person.txt");
ObjectOutputStream objectOutputStream = new ObjectOutputStream(outputStream);

Person person = new Person();
person.setName("张三");
person.setAge(25);
objectOutputStream.writeObject(person);

objectOutputStream.flush();
objectOutputStream.close();
```
以上就是在Java中的序列化过程，执行该程序 ，会在桌面生成一个person.txt的文本文件，查看下内容，发现是乱码：<br />![2021-08-30-22-35-52-466443.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334444744-fa9914de-dbf2-47bd-a275-e056b7aeaa96.png#clientId=u2722760f-56bf-4&from=ui&id=u0fe1004c&originHeight=273&originWidth=1080&originalType=binary&ratio=1&size=83419&status=done&style=shadow&taskId=u6ca8985a-b35a-4e56-b364-e3b554aca51)<br />可以使用专业的十六进制编辑器查看，可以看到相关的十六进制内容以及二进制内容，这个就是Java对象序列化后的字节序列了。<br />接着看看如何将其反序列化，也就是将其还原成原来的Java对象！
```java
//反序列化
Person person = null;
File file;
FileInputStream fileInputStream = new FileInputStream("C:\\Users\\ithuangqing\\desktop\\person.txt");
ObjectInputStream objectInputStream = new ObjectInputStream(fileInputStream);
person = (Person) objectInputStream.readObject();

objectInputStream.close();
fileInputStream.close();

System.out.println(person.getName());
```
可以得到相应的结果，执行上述反序列化程序，可以得出：<br />![2021-08-30-22-35-52-584433.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334455902-d66ede2b-8425-4013-bc62-41c861e2ac29.png#clientId=u2722760f-56bf-4&from=ui&id=ud8802240&originHeight=185&originWidth=701&originalType=binary&ratio=1&size=18049&status=done&style=none&taskId=u0e8510e2-2262-4e5e-9114-2f51d5a4517)<br />正确得到之前设置的name，说明反序列化成功，以上就是在Java中的序列化和反序列化操作！
<a name="FSCVA"></a>
### 查看序列化出来的内容
可以查看序列化出来的字节序，会发现以txt文本形式查看是一些乱码，乱七八糟的，也看不懂，可以以二进制内容查看！<br />![2021-08-30-22-35-52-733432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334455932-7adbd568-f9d6-4980-9510-4fe973fff678.png#clientId=u2722760f-56bf-4&from=ui&id=bYZ6m&originHeight=387&originWidth=1010&originalType=binary&ratio=1&size=63866&status=done&style=none&taskId=u98f7f5ae-1260-4212-a935-fb7e591b98d)<br />这些二进制内容，就是该Peroson对象在内存中的存储形式！这也是Java把对象序列化出来的二进制内容!
<a name="FXlfX"></a>
### 序列化条件Serializable
一个Java对象要想实现序列化的功能，那就必须实现一个接口，如下：<br />![2021-08-30-22-35-52-924432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334476721-4bbf5e76-55df-4dcd-b103-50fea6d3054c.png#clientId=u2722760f-56bf-4&from=ui&id=u8f782144&originHeight=240&originWidth=863&originalType=binary&ratio=1&size=25021&status=done&style=none&taskId=u63bee161-01fa-4936-b7a4-432fcaf230e)<br />可以查看该接口的定义：<br />![2021-08-30-22-35-53-057431.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334476798-3b255e17-a4e1-446c-a030-c2f3abccac7e.png#clientId=u2722760f-56bf-4&from=ui&id=qu4Wk&originHeight=399&originWidth=828&originalType=binary&ratio=1&size=71723&status=done&style=none&taskId=u4833c258-5e31-47f2-acd3-f0d829c535b)<br />可以发现，这就是一个空接口，在Java中这样的接口叫做**“标记接口”英文叫做Marker Interface**，一般来说，一个类如果实现了一个标记接口，起到的作用仅仅是给自己增加一个标签，比如上述这个，如果一个类实现了这个接口，那就会给自己身增加一个“序列化”的标签，说明这个类可以实现序列化相关功能!
<a name="nabop"></a>
### 为什么要实现Serializable接口
那为什么要实现Serializable接口呢难道就是为了让其实现序列化？说起来好像是这么回事，但是还是有一些深入的内容需要了解的！<br />首先来看两个类：<br />1、`ObjectOutputStream `<br />2、`ObjectInputStream`<br />两个类：经过之前的代码演示清楚了，对于Java的序列化而言就是通过上述两个数据流类来完成的，也就是说他们包含了序列化和反序列化对象的方法，分别如下：<br />![2021-08-30-22-35-53-254434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334476926-38c16bc7-a1a1-46c6-933b-386208e482e3.png#clientId=u2722760f-56bf-4&from=ui&id=vYMHF&originHeight=495&originWidth=1080&originalType=binary&ratio=1&size=155470&status=done&style=none&taskId=u2032f5d9-3639-4bc3-8c24-fa09f47710f)![2021-08-30-22-35-53-457429.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334498716-84189f43-b19f-46d3-bd0d-7a106bbeebe1.png#clientId=u2722760f-56bf-4&from=ui&id=u965e29c4&originHeight=519&originWidth=1080&originalType=binary&ratio=1&size=144195&status=done&style=none&taskId=u23b8a1c2-0535-4433-8f6e-599c5ac2643)<br />先拿`ObjectOutputStream`来说，这个类包含很多写方法来写各种的数据类型，举一个例子来看：![2021-08-30-22-35-53-596434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334498679-5a640d86-c27c-4008-bace-c056e8af5132.png#clientId=u2722760f-56bf-4&from=ui&id=Ls30g&originHeight=235&originWidth=1080&originalType=binary&ratio=1&size=79353&status=done&style=none&taskId=u17cbf132-42e0-4b5f-93e6-264c2e07f22)<br />可以看到其中包含一个`writeInt`方法可以将基本数据类型写到外部文件中，同样的则可以使用`ObjectInputStream` 再将外部的数据读取进来：<br />![2021-08-30-22-35-53-782439.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334498687-fce76060-38c5-4d06-95d2-7aac42241ddc.png#clientId=u2722760f-56bf-4&from=ui&id=Msvx4&originHeight=238&originWidth=1080&originalType=binary&ratio=1&size=66170&status=done&style=none&taskId=uaae0a262-e65e-48af-a90c-667d002d1bb)<br />查看打印输出：<br />![2021-08-30-22-35-53-894433.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334529995-593724fe-7d3c-4aa9-855c-363f03be2d24.png#clientId=u2722760f-56bf-4&from=ui&id=u6de0df15&originHeight=201&originWidth=939&originalType=binary&ratio=1&size=21986&status=done&style=none&taskId=ube917058-6697-431f-b044-fb625845733)<br />所以对于 `ObjectInputStream` 和 `ObjectOutputStream`这种高层次的数据流来说，它可以完成这样的一些操作，就是可以通过写操作将数据类型转换为字节流，而读操作又可以将字节流转换成特定的数据类型！<br />那对于Java这种高级面向对象编程语言而言，对象则是Java中所有数据的一个类型载体，因此也要可以对对象进行相应的读写，说白了就是需要让Java虚拟机知道在进行IO操作的时候，如何进行对象和字节流之间的转换，而Serializable接口就起到了这样的作用！<br />所以对于`ObjectInputStream` 和 `ObjectOutputStream`来说最主要的作用是进行Java序列化的操作，主要是因为他们提供了如下的两个方法：<br />1、序列化一个对象
```java
public final void writeObject(Object x) throws IOException
```
2、反序列化对象
```java
public final Object readObject() throws IOException, ClassNotFoundException
```
<a name="kP7F2"></a>
### .ser文件
可以看这里的一步操作：<br />![2021-08-30-22-35-54-150430.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334557591-971e372f-c64a-47b8-a0ca-19cee5fab45d.png#clientId=u2722760f-56bf-4&from=ui&id=u99c5f20b&originHeight=395&originWidth=1080&originalType=binary&ratio=1&size=102631&status=done&style=none&taskId=ud3cab35e-968d-480c-9cda-e8d217371d8)<br />也就是把Java对象序列化出来的字节序列内容存储到了一个txt文本文件里面，不过，就好比写的与i写纯文本文件的格式txt，写的Java文件格式是java，也就是后缀名是.java，所以这里序列化出来的文件也有一个标准的格式叫做.ser<br />那为什么叫做这个呢？因为Java中要把一个对象序列化需要实现`Serializable`，看这个单词的开头，操作一下：<br />![2021-08-30-22-35-54-380437.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334557599-67548369-7948-4c7d-8bbc-b0180b9a5c18.png#clientId=u2722760f-56bf-4&from=ui&id=LfVCz&originHeight=383&originWidth=1080&originalType=binary&ratio=1&size=113957&status=done&style=none&taskId=ud7d3da70-9563-4c26-bda5-3a45cc7ded0)<br />执行该序列化程序，会得到该文件：<br />![2021-08-30-22-35-54-525432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334557664-04ddee58-dc9e-4739-8e76-57232a3540fe.png#clientId=u2722760f-56bf-4&from=ui&id=WoK48&originHeight=117&originWidth=546&originalType=binary&ratio=1&size=70777&status=done&style=none&taskId=u35274f64-c3a8-4c75-9f1e-abe95553a8e)<br />可以打开看下：<br />![2021-08-30-22-35-54-739434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334577925-eadfe7ab-33a3-4e26-aa90-f643cb561f4d.png#clientId=u2722760f-56bf-4&from=ui&id=u45435a65&originHeight=393&originWidth=1080&originalType=binary&ratio=1&size=162032&status=done&style=shadow&taskId=u379e4c96-b529-4890-8ca8-c99edf697e1)<br />以二进制内容查看：<br />![2021-08-30-22-35-54-920434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334577863-12ae8d8e-f26a-42f6-ada4-769db0a9f14e.png#clientId=u2722760f-56bf-4&from=ui&id=u0b4e&originHeight=374&originWidth=966&originalType=binary&ratio=1&size=65466&status=done&style=none&taskId=u502086d2-a71f-4642-8cb1-6508e6340ce)<br />和之前的txt的内容是一致的！说这个只是希望大家后续在见到.ser文件知道这是一个序列化文件！
<a name="EZ1TC"></a>
## 再看序列化是什么
到了这里再来看看，什么是Java的序列化：<br />把Java对象在内存中的状态给存储下来的一个过程，会得到一个该Java对象的字节序列，可以说是一个二进制内容，本质上是一个`byte[]`数组！<br />为什么说是`byte[]`数组呢？`1byte = 8bit`，也就是8位一组，也就是一个字节，而二进制内存都是0和1这种8位8位的，看下：<br />![2021-08-30-22-35-55-081431.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334577815-ff3a85e3-2b8c-47ab-9af1-8b0ba5bc58e1.png#clientId=u2722760f-56bf-4&from=ui&id=ad8Xr&originHeight=369&originWidth=847&originalType=binary&ratio=1&size=58953&status=done&style=none&taskId=u7590f9d1-6a00-4e55-ae43-4007378717f)<br />看着这张图，可以理解为什么是byte[]数组了吧！
<a name="ACjwO"></a>
# why
<a name="oEwSx"></a>
## 序列化可以用在哪些地方
经过之前的描述知道了，通过序列化，可以把Java对象转换成字节序列，也就是二进制内容，这个内容中就包含了对象的相关数据，以及对象的类型信息和存储在对象中的数据的类型！<br />也就是说，通过反序列化是可以还原这个Java对象的，而且这个过程是Java虚拟机来操作的，是直接由Java虚拟机来构建这个对象，所以这个对象中的构造函数是不会得到执行的，因为根本不会经过这一步，虚拟机直接把对象给整出来了！<br />那就知道了，反序列化是由虚拟机来搞定的，那么也就是实现了，在一个平台上序列化出来的对象，完全可以放到另一个平台上去反序列化该对象！<br />因此，这就造成了，序列化可以用于一些特定的数据传输：<br />1、把内存中的对象保存起来，比如保存到数据库中或者保存到一个文件中，以便长期保存<br />2、使用socket进行网络数据传输<br />3、RMI(即远程调用Remote Method Invocation)的使用，也就是要利用对象序列化去运行远程主机上的服务，以达到就像在本地机上运行对象时一样。
<a name="IznaV"></a>
## 序列化有哪些好处
那序列化有什么好处呢？或者说为什么要用序列化呢？<br />首先，数据其实是比较复杂的，比如对象啊，文件啊，等等都有各自不同的数据格式，怎么能把它们统一保存呢？这不，序列化就可以啊，经过序列化之后，别管是什么，都保存在一块了，都是字节序列，这就方便数据之间的传输了，因为格式统一！<br />另外，序列化保存的是对象在内存中的保存状态，反序列化的时候是由Java虚拟机直接将该类还原在内存中，简洁快速而高效!<br />还有就是有的时候需要把Java对象从内存中保存下来，以便脱离内存，存储在磁盘上，达到长期保存的目的，那这个序列化就很合适了！
<a name="OXmSW"></a>
## 序列化注意事项
<a name="qAydb"></a>
### 异常
这里主要有两个异常：<br />1、`ClassNotFoundException`：没有找到对应的Class<br />2、`InvalidClassException`：Class不匹配<br />首先说一下第一个异常，也就是`ClassNotFoundException`，这个其实是比较简单的，也就是类没找到，什么类没找到呢？代码复现一下：<br />![2021-08-30-22-35-55-328432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334630155-439cc695-40e2-43f4-b1fd-774069058831.png#clientId=u2722760f-56bf-4&from=ui&id=udc22b0e5&originHeight=305&originWidth=1080&originalType=binary&ratio=1&size=107983&status=done&style=none&taskId=u0729008b-6188-44f8-b1a4-8b335a7ba4f)<br />执行该代码，就会发生问题：<br />![2021-08-30-22-35-55-553430.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334630197-2963d047-5250-4dc0-b559-74b3968645ca.png#clientId=u2722760f-56bf-4&from=ui&id=lBJHO&originHeight=236&originWidth=1080&originalType=binary&ratio=1&size=117408&status=done&style=none&taskId=u2bb6b891-33ed-44a3-8019-ca376fec875)<br />这个错误其实已经说的很明显了，没有找到Person这个类了，那是因为把之前的Person改成Person1了，所以反序列化就找不到对应的类，这就是无法实现反序列化的：<br />![2021-08-30-22-35-55-718433.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334629902-3ab291e2-3b6b-4acf-b0ee-b0bb26f25c49.png#clientId=u2722760f-56bf-4&from=ui&id=m2eE4&originHeight=281&originWidth=1008&originalType=binary&ratio=1&size=32794&status=done&style=none&taskId=u0921492b-1fd2-4bc1-bcb9-a1c329a32d3)<br />再把Person1改回Person试一下：<br />![2021-08-30-22-35-55-932435.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334655504-d35b8173-d779-4d16-b0fd-f5ca504d4025.png#clientId=u2722760f-56bf-4&from=ui&id=uf1c7db70&originHeight=448&originWidth=1080&originalType=binary&ratio=1&size=151385&status=done&style=none&taskId=ucd9c0ae7-9a91-4f48-9b83-afdd2d61848)<br />也就是反序列化的时候，得有个对应的类，而且这个类的路径位置啥的也是不能改变的：<br />![2021-08-30-22-35-56-149431.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334655563-8bf3424a-572d-4475-b91b-bed9a6d28e7d.png#clientId=u2722760f-56bf-4&from=ui&id=ccHHJ&originHeight=479&originWidth=1080&originalType=binary&ratio=1&size=192129&status=done&style=none&taskId=u10806c42-3434-4e3d-b90c-6ead9c83229)<br />接下来看下InvalidClassException这个也就是Class不匹配的问题。<br />同样，通过代码来演示：<br />![2021-08-30-22-35-56-345435.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334655399-d3f0e9e5-f890-402e-8bdd-0fc26b2ad634.png#clientId=u2722760f-56bf-4&from=ui&id=RDW3M&originHeight=198&originWidth=1080&originalType=binary&ratio=1&size=99689&status=done&style=none&taskId=u0c3eddda-12bb-4dae-b0b5-1fd86bf0a2b)<br />为什么会出现这个错误呢？看这里：<br />![2021-08-30-22-35-56-447434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334701336-2628963c-50f9-4f3b-896a-0880334ddc9d.png#clientId=u2722760f-56bf-4&from=ui&id=u2e44fc71&originHeight=286&originWidth=850&originalType=binary&ratio=1&size=36762&status=done&style=none&taskId=u82282c91-8ab7-4b28-b27e-297d6162b0e)<br />之前是int，反序列化的时候这成了long，不就是不匹配嘛，所以这样也是会出错的！<br />所以啊，总的来说，就是，序列化的时候是什么样子的，反序列化个一模一样的，但是如果就没有定义这样的一个类，那反序列化肯定就出错了！
<a name="IMdSR"></a>
### 如何解决：serialVersionUID
为了解决InvalidClassException的问题，就出现了这么一个东西，先看代码：<br />![2021-08-30-22-35-56-820475.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334701496-03e3e9c1-0da1-4c1b-a440-f77450cb4066.png#clientId=u2722760f-56bf-4&from=ui&id=WA1na&originHeight=248&originWidth=955&originalType=binary&ratio=1&size=34074&status=done&style=none&taskId=u327dff52-a577-4c04-9878-001ed975c3a)<br />这个操作大家应该不陌生吧，应该都见过的，它的出现主要就是为了解决Class类型不匹配的问题，它作为标识Java类的序列化版本，可以看作是一个标记，一般来说，可以由IDE自动生成，如果修改了类中的字段什么的，那这个`serialVersionUID`就会发生改变，通过这样的一种机制来自动阻止不匹配的class版本！<br />在之前进行序列化操作的时候并没有创建这个`serialVersionUID`，但是，没有显式创建就并不代表它没有，而是会生成默认的`serialVersionUID`，但是，在实际当中，Java官方建议还是要显式的创建`serialVersionUID`！<br />为啥，因为如果不显式创建，那么默认的生成时高度依赖于JVM的，但是如果序列化和反序列化是跨平台操作的化，就有可能会发生前后创建的`serialVersionUID`不一致从而导致反序列化出现异常，所以还是要显式创建`serialVersionUID`，保证即使跨平台这个`serialVersionUID`也是一致的！<br />那`serialVersionUID`是如何起作用的呢？<br />说的简单点就是，序列化的时候这个`serialVersionUID`也被序列化进去了，那在反序列化的时候，JVM就会把传进来的字节流中的`serialVersionUID`与本地对应的类中的`serialVersionUID`进行比较，看是否一致，一致就可以反序列化，不一致就不能反序列化，看代码演示：<br />![2021-08-30-22-35-56-965474.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334701514-aeaba474-c956-4220-9618-eed0f04fad83.png#clientId=u2722760f-56bf-4&from=ui&id=IVrGg&originHeight=263&originWidth=1001&originalType=binary&ratio=1&size=34053&status=done&style=none&taskId=ue985bd5d-a15d-466d-b658-5d4cefd533a)<br />这里显式创建`serialVersionUID`，然后进行序列化操作生成新的字节序列内容，在反序列化之前，把这个`serialVersionUID`给修改下：<br />![2021-08-30-22-35-57-068432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334754129-3ad695f0-acb5-4fce-9695-ac023de42e6a.png#clientId=u2722760f-56bf-4&from=ui&id=u1069ed36&originHeight=257&originWidth=887&originalType=binary&ratio=1&size=34357&status=done&style=none&taskId=u551b70c8-9d3f-45ed-b31c-2be51e1acc4)<br />然后进行反序列化操作：<br />![2021-08-30-22-35-57-263438.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334754276-487f8ca9-0057-4392-a611-fab0deca92f6.png#clientId=u2722760f-56bf-4&from=ui&id=XPQoD&originHeight=325&originWidth=1080&originalType=binary&ratio=1&size=156630&status=done&style=none&taskId=ufaa0fb70-a689-48ac-8ce1-4fc1b7e28cf)<br />产生异常了，看来是类不匹配，看下这个异常描述：<br />![2021-08-30-22-35-57-484433.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334754129-4d9b5d25-0ec5-4e12-a832-d174f6aab83f.png#clientId=u2722760f-56bf-4&from=ui&id=dPRvY&originHeight=161&originWidth=1080&originalType=binary&ratio=1&size=22869&status=done&style=none&taskId=u3d388cfe-c431-4962-956f-e7613dfd9cc)<br />说的是不是很清楚了！<br />那这个时候，再次将起修改成1L，然后做如下操作：<br />![2021-08-30-22-35-57-670434.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334794132-32f40bcc-e211-4eac-bb7c-e00c1fd8018d.png#clientId=u2722760f-56bf-4&from=ui&id=ued7a7a4e&originHeight=283&originWidth=1033&originalType=binary&ratio=1&size=65475&status=done&style=none&taskId=u6125ddc1-368e-4fcc-ba63-4be9c726843)<br />那根据之前说的，这里修改了类，那反序列化的化就会出现InvalidClassException 问题，那执行反序列化看下：<br />![2021-08-30-22-35-57-842473.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334794058-724895a6-0b66-4cf1-b09b-3bb76266a5cc.png#clientId=u2722760f-56bf-4&from=ui&id=Ft1la&originHeight=135&originWidth=1080&originalType=binary&ratio=1&size=61159&status=done&style=none&taskId=ucd46d93d-4ed2-46c0-ac5e-a0a68f78402)<br />果然报错，类型问题，不过再来看一个情况：<br />![2021-08-30-22-35-58-072432.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334794237-495ce9b5-2d9c-4aae-8eb2-088235ddb1ca.png#clientId=u2722760f-56bf-4&from=ui&id=imVY2&originHeight=301&originWidth=1080&originalType=binary&ratio=1&size=106685&status=done&style=none&taskId=u1f4e01cf-8273-4c82-a1fc-228ea0b7fa0)<br />再反序列化试下：<br />![2021-08-30-22-35-58-165436.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630334822789-5e1b2e1f-f9ed-42f0-b1ae-e637a4083009.png#clientId=u2722760f-56bf-4&from=ui&id=ua56e806a&originHeight=210&originWidth=789&originalType=binary&ratio=1&size=19552&status=done&style=none&taskId=u4bb7003c-32e2-4e4a-be83-b084493971d)<br />正常输出，没有报错，但是如果没有显式自定义`serialVersionUID`的话，那就会由系统自定义生成，那就会报错了！<br />所以一定要注意`serialVersionUID`的加与不加的一些区别和可能会产生的问题！也就是说序列化完成之后，如果原类型字段增加或者减少，不指定`serialVersionUID`的情况下，也是会报不一致的错误。指定了则不报错！
<a name="ozqo8"></a>
## Java的序列化安全嘛
以上较为详细的介绍了Java的序列化操作，那么现在来思考这样的一个问题，Java的序列化安全嘛？也就是使用Java序列化的操作会不会产生什么安全隐患？<br />想象一下这个，就是，序列化生成的字节序列可以通过反序列化直接将其在内存中还原成原状态，那如果某个字节序列是特意设置好的，含有一些不安全代码，那直接给还原到内存中了，是不是会产生一些安全问题！<br />所以Java中提供的序列化机制，本身是存在一些安全性问题的，那更好的办法是啥呢？可以通过使用json来实现，这样输出的数据都是一些基本类型的内容，不像Java序列化那样，序列化输出的包含了很多对象相关信息！
