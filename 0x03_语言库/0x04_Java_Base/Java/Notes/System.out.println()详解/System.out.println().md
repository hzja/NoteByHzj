Java<br />如何理解`System.out.println()` ?<br />学了这么久的面向对象编程，那如何用一行代码体现呢？<br />如果能自己读懂`System.out.println()`，就真正了解了Java面向对象编程的含义<br />面向对象编程即创建了对象，所有的事情让对象帮亲力亲为（即对象调用方法）
```java
System.out.println("hello world");
```
```
hello world

Process finished with exit code 0
```
<a name="bEnyL"></a>
### 首先分析System源码
System就是Java自定义的一个类<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267163-3448ad23-bcfe-43c4-8283-77442f5439a2.png#averageHue=%23fcf9f8&clientId=u02c5a9dd-8278-4&from=paste&id=u8fc70af7&originHeight=597&originWidth=818&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u109f67cf-f86e-42da-b6e4-4bd3d10284b&title=)
<a name="zMNsf"></a>
### out源码分析
①out是System里面的一个静态数据成员，而且这个成员是`java.io.PrintStream`类的引用<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267004-ca035fa0-6770-4efd-9603-b62f12a3da3d.png#averageHue=%23fcf2f1&clientId=u02c5a9dd-8278-4&from=paste&id=udb41fa3d&originHeight=83&originWidth=569&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3c895e4c-60ad-4e02-8422-c9af245e82a&title=)<br />②out已经存在了且用`Static`修饰了，所以可以直接使用类名+属性名的方式调用，也就是`System.out`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267117-65d202cf-a871-4e5c-95a4-a2449aa2c0cd.png#averageHue=%23faf3f2&clientId=u02c5a9dd-8278-4&from=paste&id=u826a8165&originHeight=396&originWidth=713&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc51c4fba-edfa-47a3-ba12-0e398000ac4&title=)
<a name="mv8rE"></a>
### println分析
①`println()`就是`java.io.PrintStream`类里的一个方法，它的作用是向控制台输出信息。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267381-9eda4388-4d40-4ad6-b184-895453f7e9ea.png#averageHue=%23fdf4f3&clientId=u02c5a9dd-8278-4&from=paste&id=u59716c53&originHeight=191&originWidth=633&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud01c5924-0902-45a3-9839-18c368423ec&title=)<br />②里面有很多重载的方法，这样就保证了任意的东西都可以输出<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267728-63fd20bf-4e53-43ab-87c2-1fe44e7a26e3.png#averageHue=%23fdfaf9&clientId=u02c5a9dd-8278-4&from=paste&id=u29c03044&originHeight=470&originWidth=810&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u77a837b3-5543-4c88-9fac-9badbf93a54&title=)<br />小结下来就是：类调用对象，对象调用方法
<a name="rXQC3"></a>
### 拓展知识点 :
<a name="xefMa"></a>
#### 1、`System.out.print();`与`System.out.println();`的区别
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267762-a9e89d83-af5f-487b-8249-01672d136b8a.png#averageHue=%23faf8f6&clientId=u02c5a9dd-8278-4&from=paste&id=u4d02f01d&originHeight=508&originWidth=678&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9d5ce3c-5efa-4d8f-bc67-5084fb35741&title=)
<a name="e4VRS"></a>
#### 2、字符数组输出面试案例
```java
public class Demo {

    public static void main(String[] args) {
        char[] ch=new char[]{'x','y'};
        System.out.println(ch);

        char[] ch1=new char[]{'x','y'};
        System.out.println("ch1="+ch1);
    }
}
```
```
xy
ch1=[C@74a14482
```
这是`println()`方法的重载，java打印输出`System.out.println`会自动调用输入参数的`toString`方法，输出内容时`toString`方法的返回值。<br />`**println**`**的参数分基本类型，一个是String 另一个是Object类型。**<br />`System.out.println(ch)`，`println()`自动调用`println(char[])` 也就是Object类型 所以输出xy<br />然而`System.out.println("ch="+ch) "+"`是字符串连接符，自动调用`println(String)`，也就是String类型 输出的是xxx@xxxx的形式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669726267612-92f1bffb-b5d7-476b-9037-5a667b290488.png#averageHue=%23fbfaf7&clientId=u02c5a9dd-8278-4&from=paste&id=u29c05b81&originHeight=107&originWidth=826&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u31ae1068-fc35-49dc-a63b-f4105f98a0d&title=)
