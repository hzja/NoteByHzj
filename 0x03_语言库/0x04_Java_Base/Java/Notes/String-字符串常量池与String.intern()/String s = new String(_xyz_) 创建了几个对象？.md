Java
<a name="PLdAd"></a>
### 从方法区说起
常量池存在于方法区，而方法区在jdk1.7版本前后改变比较大，所以还是先来说说方法区的演变。<br />在jdk1.7版本之前，常量池存在于方法区，方法区是堆的一个逻辑部分，他有一个名字叫做非堆。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858845849-e57c924f-c17c-428e-aa07-80b8a748ade4.jpeg#averageHue=%23f0e7c8&clientId=u376cb4f0-b2d7-4&from=paste&id=u68649645&originHeight=314&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua4a136cf-aac0-4cfe-a327-acb2fc02ca9&title=)<br />1.7版本把字符串常量池放到了堆中。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858845904-a18bf6f8-b387-4945-99ba-18e6dc6b6927.jpeg#averageHue=%23ebe6c4&clientId=u376cb4f0-b2d7-4&from=paste&id=u06b60892&originHeight=306&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue2db5397-8317-4084-88e7-f49cab63baa&title=)<br />而在1.8以后，则是移除了永久代，方法区概念保留，方法区的实现改为了元空间，常量池还是在堆中。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858845910-825184f6-6288-479b-955f-6be6a702aa85.jpeg#averageHue=%23e9e4c5&clientId=u376cb4f0-b2d7-4&from=paste&id=u85faa487&originHeight=298&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u391962dc-56c8-47be-a6cb-6b4088679fb&title=)<br />为什么要说方法区的改变，只是为了文章接下来的内容不会由于JDK的版本而产生分歧，接下来内容都会以jdk1.8版本作为基础来讨论。
<a name="xKUv6"></a>
### `String s = new String("xyz");`
先来一段代码：
```java
public class Test {
    public static void main(String[] args) {
        String s = "xyz";
    }
}
```
接着javac编译代码，然后用javap来反编译，执行`javap -c Test`<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858845925-0c1d9531-fef3-4c92-882f-50bc8bb7cf8c.jpeg#averageHue=%232f2f2f&clientId=u376cb4f0-b2d7-4&from=paste&id=uc8c42b50&originHeight=490&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc0a6bfc8-ec0b-41a4-9342-bc132cb3ca2&title=)<br />从结果来看，ldc命令**在常量池中创建了一个"xyz"的对象，然后把他推至操作数栈顶**，然后astore保存到局部变量，return返回。<br />接着看第二段面试题中的代码
```java
public class Test {
    public static void main(String[] args) {
        String s = new String("xyz");
    }
}
```
同样反编译分析<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858845883-b3b91d7b-7cb1-43d6-b6ff-4f34349f862f.jpeg#averageHue=%232c3031&clientId=u376cb4f0-b2d7-4&from=paste&id=uef9e2634&originHeight=492&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uead1018c-76a8-4154-be5f-2de8a1b526d&title=)<br />很明显，可以看到new 创建了一个String对象，同时ldc在常量池中创建了"xyz"字符串对象，之后`invokespecial`执行构造函数，astore_1赋值，return返回。<br />通过以上两个例子，可以知道`String s = new String("xyz");` 创建了2个对象，而有些答案说的3个对象，则是把引用s也算作一个对象。<br />还有答案说xyz存在就创建了2个，不存在就创建了3个（包含引用s），再来测试一下。
```java
public class Test {
    public static void main(String[] args) {
        String s = "xyz";
        String s2 = new String("xyz");
    }
}
```
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680858846422-df8cddc5-8231-4ea1-93df-2969ed29d68e.jpeg#averageHue=%232c3031&clientId=u376cb4f0-b2d7-4&from=paste&id=u2db5ba2d&originHeight=485&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7cdfe54d-0212-478d-bb8a-3a77a0de1c9&title=)<br />从这里，很明显的发现这就是例子1和2的一个结合，但是注意两次ldc后面的#2，#号代表着索引，说明第二次`new String("xyz")`的时候并没有重新创建xyz对象。<br />**一些常见的指令助记符含义：**

1. **nop**， 什么都不做。
2. **aconst_null**，将 null 推送至栈顶。
3. **iconst_i**(变量数字)，将 int 型 i 推送至栈顶。同理有lconst_0，fconst_0这种你应该知道什么意思了
4. **ldc**，将 int，float 或 String 型常量值从常量池中推送至栈顶。
5. **iload**，将指定的 int 型局部变量推送至栈顶。
6. **istore**，将栈顶 int 型数值存入指定局部变量。同理astore_i代表将栈顶引用型数值存入第i个局部变量。
7. **dup**，复制栈顶数值并将复制值压入栈顶。
8. **invokevirtual**，调用实例方法。
9. **invokespecial**，调用超类构造方法，实例初始化方法，私有方法。
10. **invokestatic**，调用静态方法。
11. **invokeinterface**，调用接口方法。
12. **invokedynamic**，调用动态链接方法。
13. **new**，创建一个对象，并将其引用值压入栈顶。
<a name="ciyad"></a>
### 总结
**到底创建了几个对象呢？**

1. 如果xyz不存在，引用算对象的话，那就是3个
2. 如果xyz不存在，引用不算对象的话，那就是2个
3. 如果xyz存在，引用算对象的话，那就是2个
4. 如果xyz存在，引用不算对象的话，那就是1个

当然，引用肯定是不算对象的，最终答案应该是1或者2个，这个面试题说实话不应该出现在初级面试题里。
