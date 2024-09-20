Java
<a name="L0R3A"></a>
## 使用 `new` 来创建对象
使用 `new` 来创建对象是最简单的一种方式了，`new` 是 Java 中的关键字，`new` 通过为新对象分配内存并返回对该内存的引用来实例化一个类，这个实例化一个类其实就相当于创建了一个对象，因为类也是一种对象；`new` 也负责调用对象的构造函数，下面是使用 `new` 来创建对象的代码
```java
Object obj = new Object();
```
这段代码中，在堆区域中分配了一块内存，然后把 obj 对象指向了这块内存区域。<br />下面是这段代码的字节码<br />![2021-06-25-21-12-34-546921.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624626956888-e25694d4-876f-4186-a653-1a5cc8a3e061.png#clientId=ub8f9af12-97fd-4&from=ui&id=u41cb8c97&originHeight=273&originWidth=739&originalType=binary&ratio=3&size=9147&status=done&style=shadow&taskId=u048a625e-c737-4bc0-9593-53d4da188d9)<br />在 Java 中，创建一个对象就是调用其构造方法，所以使用 `new Object()` 构造的对象，其实是调用了 Object 类的`无参数` 的构造方法。但是通过字节码发现，对象的创建和调用其构造方法是分开的。<br />字节码的 `new` 表示在堆中创建一个对象，并把对象的引用推入栈中。`invokespecial` 表示调用对象无参数的构造方法。其实，JVM 提供了五种方法调用指令，分别是

- `invokestatic`：该指令用于调用静态方法，即使用 `static` 关键字修饰的方法；
- `invokespecial`：该指令用于三种场景：调用实例构造方法，调用私有方法（即 `private` 关键字修饰的方法）和父类方法（即 super 关键字调用的方法）；
- `invokeinterface`：该指令用于调用接口方法，在运行时再确定一个实现此接口的对象；
- `invokevirtual`：该指令用于调用虚方法（就是除了上述三种情况之外的方法）；
- `invokedynamic`：在运行时动态解析出调用点限定符所引用的方法之后，调用该方法；在 JDK 1.7 中提出，主要用于支持 JVM 上的动态脚本语言（如 Groovy，Jython 等）

好了，现在知道了 `new` 和 `invokespecial` 是干啥用的，那么 `dup` 指令呢？<br />dup 会复制栈上的最后一个元素，然后再次将其推入栈；因此，如果在栈上有一个对象引用，并且调用了 dup，则现在在栈上有对该对象的两个引用。看起来有点不知其所以然，所以在求助网上的时候，又发现了 R 大的解释<br />![2021-06-25-21-12-34-772292.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627018756-0e3bea65-8ada-4397-a1af-4f62a728f25d.png#clientId=ub8f9af12-97fd-4&from=ui&id=u7f2da40f&originHeight=79&originWidth=680&originalType=binary&ratio=3&size=14157&status=done&style=shadow&taskId=u3f2aa00f-fe29-44d3-8360-39cac7945ee)<br />后面的 astore 就会把操作数栈顶的那个引用消耗掉，保存到指定的局部变量去。<br />如果直接使用 `new Object()` 没有创建局部变量的话，请注意一下它的字节码。<br />![2021-06-25-21-12-34-914935.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627037490-4d95b3ad-87b4-4ec6-9402-2117342aa5f7.png#clientId=ub8f9af12-97fd-4&from=ui&id=u6cdabb1e&originHeight=269&originWidth=745&originalType=binary&ratio=3&size=9025&status=done&style=shadow&taskId=u0f16ebce-ebc8-4f00-a74c-dbd5a2dcf48)<br />看出来细微的差别了吗？上图中的 `astore_1` 竟然变成了 `pop`，这也就是说，`new Object()` 没有保存对象的局部变量，而是直接把它给消耗掉了。符合预期。<br />所以这是第一种创建的方式，也就是使用 new 来创建。
<a name="ccrYm"></a>
## 使用 `newInstance` 方法来创建
这个`newInstance` 方法指的是 `class` 类中的方法，`newInstance` 方法会调用无参的构造方法创建对象。<br />可以使用 `newInstance` 方法创建对象，下面是使用示例代码
```java
User user = (User)Class.forName("com.test.User").newInstance();

// 或者使用

User user = User.class.newInstance();
```
下面分析一下这个字节码，其实使用第一种方式和第二种方式就差了一个 `Class.forName` 的字节码，这是一个静态方法，应该用的是 `invokestatic`，下面验证一下。<br />第一种方式的字节码<br />![2021-06-25-21-12-35-174208.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627070119-2d757ffe-ddc5-4eb5-9172-9fc09b7951d5.png#clientId=ub8f9af12-97fd-4&from=ui&id=u91272fce&originHeight=149&originWidth=832&originalType=binary&ratio=3&size=51571&status=done&style=none&taskId=u3be481ac-7a73-4421-bfde-da8744beab5)<br />第二种方式的字节码<br />![2021-06-25-21-12-35-389639.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627080967-3d8ead15-2025-4771-8300-fd69f08401db.png#clientId=ub8f9af12-97fd-4&from=ui&id=uc78bfdaf&originHeight=152&originWidth=839&originalType=binary&ratio=3&size=43932&status=done&style=shadow&taskId=ud6746987-6eda-49e1-bfb6-e3eee2d8bac)<br />可以看到验证的是正确的。<br />那么这段字节码是什么意思呢？<br />`ldc` 的意思是把常量池中的引用推入到当前堆栈中，`invokestatic` 和 `invokevirtual` 上面解释过了，然后就是 `checkcast`， 这个字节码的含义就是进行类型转换，因为 `newInstance` 生成的是一个 Object 的对象，所以需要把它转换为需要的 User 类型，这个字节码就是干这个活的。
<a name="yha3W"></a>
## 使用反射来创建对象
使用反射来创建对象其实也是使用了 `newInstance` 方法，只不过这个方法是 `Constructor` ，Java 反射中构造器的方法，可以通过这种方式来创建一个新的对象。如下代码所示
```java
Constructor<User> constructor = User.class.getConstructor();
User user = constructor.newInstance();
```
下面是它的字节码<br />![2021-06-25-21-12-35-654872.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627110932-33f1e269-f822-4bcd-9d6d-a23f607df560.png#clientId=ub8f9af12-97fd-4&from=ui&id=u7125f43d&originHeight=210&originWidth=823&originalType=binary&ratio=3&size=62088&status=done&style=shadow&taskId=u1e7b23d6-da6f-43a9-87c8-ddc069bbb63)<br />这里解释下 `iconst_0` ，它的意思就是将 int 值 0 加载到堆栈上，这个相当于是为 `getConstructor` 方法准备参数分配的字节码。<br />为了验证这个结论，从简优化，看一下其他方法的字节码
```java
User.class.getDeclaredField("id");
```
它的字节码如下：<br />![2021-06-25-21-12-35-847355.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627135930-dbaeae0b-d74e-4a31-a686-ea90cb708b14.png#clientId=ub8f9af12-97fd-4&from=ui&id=ue446266a&originHeight=117&originWidth=813&originalType=binary&ratio=3&size=31137&status=done&style=shadow&taskId=ue0baaa92-dcdf-4993-a527-018f55388f2)<br />可以看到，第二个 ldc 其实就是 `getDeclaredField` 中的参数，为 String 类型，所以是用的 ldc，它是将引用推入堆栈。
<a name="dFIHw"></a>
## 使用对象克隆来创建对象
这是第四种创建方式，使用 `Cloneable` 类中的 `clone()` 方法来创建，它的前提是需要实现 `Cloneable` 接口并实现其定义的 `clone` 方法。用 `clone` 方法创建对象并不会调用任何构造函数。<br />如下代码所示
```java
Constructor<User> constructor = User.class.getConstructor();
User user = constructor.newInstance();
user.setName("Hello");

User user2 = (User)user.clone();
System.out.println(user2.getName());
```
输出 Hello<br />它的字节码如下<br />![2021-06-25-21-12-36-123183.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624627165232-6993c609-5ea5-49d7-b1a3-5c3f77b41b03.png#clientId=ub8f9af12-97fd-4&from=ui&id=u61be3708&originHeight=355&originWidth=815&originalType=binary&ratio=3&size=117073&status=done&style=shadow&taskId=ua06ac6a5-2dd8-4412-b472-52e37680600)<br />这个字节码有些长，但是字节码的概念和含义上面已经介绍过了，最主要的就是推入堆栈，调用对应的实例方法。<br />对象克隆这块是面试官非常喜欢考的一个点，后面会解析一下浅拷贝和深拷贝的区别。
<a name="XQLjm"></a>
## 使用反序列化创建对象
当使用序列化和反序列化时，JVM 会创建一个单独的对象。在反序列化时，JVM 创建对象不会调用任何构造函数，如下代码所示
```java
ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("xxx"));
out.writeObject(user2);
out.close();
//Deserialization
ObjectInputStream in = new ObjectInputStream(new FileInputStream("xxx"));
User user3 = (User) in.readObject();
in.close();
user3.setName("cxuan003");
System.out.println(user3 + ", hashcode : " + user3.hashCode());
```
这段反编译过后的字节码文件比较长，这里就先不贴出来了，读可以自己编译看一下，其实并没有特别的字节码指令，大部分上面已经提到过了。
