Java<br />**在 Java 语言中，本质只有值传递，而无引用传递**，解释和证明详见正文。<br />说到值传递和引用传递我们不得不提到两个概念：值类型和引用类型。
<a name="VKknM"></a>
## 1、值类型
通俗意义上来说，所谓的值类型指的就是 Java 中的 8 大基础数据类型：

- 整数型：byte、int、short、long
- 浮点型：float、double
- 字符类型：char
- 布尔类型：boolean

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238729585-4d8bc185-a2ea-4df4-985e-938252551d41.png#clientId=u7640ead2-cc3b-4&from=paste&id=ue385e723&originHeight=715&originWidth=912&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue2b411e8-8198-42b4-9263-134ba98f53d&title=)<br />**从 JVM 层面来讲：所谓的值类型指的是在赋值时，直接在栈中（Java 虚拟机栈）生成值的类型**，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238729572-0d1d011f-98e0-4649-910f-508bba3cdf87.png#clientId=u7640ead2-cc3b-4&from=paste&id=u2a268d0c&originHeight=554&originWidth=479&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8f70eeaf-db82-4a60-a2a5-c9c5383622b&title=)
<a name="LsNNU"></a>
## 2、引用类型
引用类型是指除值类型之外的数据类型，比如：

- 类
- 接口
- 数组
- 字符串
- 包装类（Integer、Double...）

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238729568-1b6f28e9-3e66-4037-bec8-b6a31e0d756c.png#clientId=u7640ead2-cc3b-4&from=paste&id=u1bdf86b1&originHeight=577&originWidth=971&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u32196982-a9df-4c58-ba0c-d6b760c8905&title=)**<br />从 JVM 的层面来讲，所谓的引用类型是指，在初始化时将引用生成栈上，而值生成在堆上的这些数据类型**，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238729597-e11db415-d603-4e98-998d-8aef34046317.png#clientId=u7640ead2-cc3b-4&from=paste&id=u33c5dc04&originHeight=656&originWidth=985&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa91a614-16a0-40ea-9a8b-ac15ea8c523&title=)
<a name="c60Yi"></a>
## 3、值传递
**值传递（Pass By Value）指的是方法传参时，传递的是原内容的副本，因此对副本进行如何修改都不会影响原内容。**<br />实现代码如下：
```java
public class PassTest {
    public static void main(String[] args) {
        int age = 18;
        System.out.println("调用方法前：" + age);
        intTest(age);
        System.out.println("调用方法后：" + age);
    }

    private static void intTest(int age) {
        age = 30;
        System.out.println("方法中修改为：" + age);
    }
}
```
程序的执行结果为：
```
调用方法前：18
方法中修改为：30
调用方法后：18
```
从上述结果可以看出，在方法中修改参数并未影响原内容，我们把这种传参方式称之为值传递。
<a name="c0wfS"></a>
## 4、引用传递
**引用传递（Pass By Reference）指的是方法传参时，传递的是参数本身，因此对参数进行任意修改都会影响原内容。**<br />模拟“引用传递”的实现代码如下：
```java
public class PassTest {
    public static void main(String[] args) {
        char[] name = {'石', '哥'};
        System.out.println("调用方法前：" + new String(name));
        paramTest(name);
        System.out.println("调用方法后：" + new String(name));
    }
    private static void paramTest(char[] n) {
        n[1] = '神';
        System.out.println("方法中修改为：" + new String(n));
    }
}
```
程序的执行结果为：
```java
调用方法前：石哥
方法中修改为：石神
调用方法后：石神
```
从上述的结果可以看出在 paramTest 方法中修改了参数之后，在 main 方法中再打印参数时，发现参数的值也跟着发生了改变，那么似乎可以得出结论，Java 中貌似也有“引用传递”，然而实事并如此，接着看。
<a name="niQTF"></a>
## 5、真假“引用传递”
给上面的代码添加一行，如下所示：
```java
public class PassByValue {
    public static void main(String[] args) {
        char[] name = {'石', '哥'};
        System.out.println("调用方法前：" + new String(name));
        paramTest(name);
        System.out.println("调用方法后：" + new String(name));
    }
    private static void paramTest(char[] n) {
        n = new char[2]; // 添加此行代码
        n[1] = '神';
        System.out.println("方法中修改为：" + new String(n));
    }
}
```
程序的执行结果为：
```java
调用方法前：石哥
方法中修改为：神
调用方法后：石哥
```
从上述结果可以看出，当在 paramTest 方法中添加 `new char[]` 之后，“引用传递”就突然变值传递了？为什么？<br />这是因为，**在 Java 语言中本质上只有值传递，也就说 Java 的传参只会传递它的副本，并不会传递参数本身。**<br />前面那个带引号的“引用传递”其实只是传递了它的引用副本，如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238729580-576d5f49-2af5-4f4b-ab12-37fa7e1c5e5e.png#clientId=u7640ead2-cc3b-4&from=paste&id=uc50f33fb&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9590d605-6cf5-4efd-bd10-ca9b9b198c9&title=)<br />PS：《Java虚拟机规范》中对  Java 堆的描述是：“所有的对象实例以及**数组**都应当在堆上分配”。<br />所以**在调用 **`**new char[]**`** 之后，可以看出 n 对象有了新地址，而原内容并未被修改，如果按照引用传递的思路来看的话，不管执行任何方式的修改都会改变原内容，因此可以更加确认 Java 语言中只有值传递**，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238730264-a6dba0c7-37a3-45b1-9679-881e1e155de5.png#clientId=u7640ead2-cc3b-4&from=paste&id=u80fa90db&originHeight=421&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9eefd2cf-146f-421d-9753-9b82773cf35&title=)
<a name="U7ZVE"></a>
## 总结
通过本文的内容，可以得出：**在 Java 语言中只有值传递，方法传参时只会传递副本信息而非原内容**。还知道了基础数据类型会直接生成到栈上，而对象或数组则会在栈和堆上都生成信息，并将栈上生成的引用，直接指向堆中生成的数据，如下图所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1664238730275-d076acc7-0636-46aa-b25a-3d00faa0a9e5.png#clientId=u7640ead2-cc3b-4&from=paste&id=CCpHZ&originHeight=489&originWidth=901&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub4b8c75d-2374-4640-8291-dfa9a4cb869&title=)
