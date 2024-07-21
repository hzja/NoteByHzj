<a name="zvIAV"></a>
## 流程
controller→service→impl→dao
<a name="JEOJH"></a>
## 常见java中间件服务器
Tomcat 、 Weblogic、Jetty、JBoss、GlassFish等

<a name="0t6tV"></a>
## 项目管理和构建工具Maven
Maven是一种自动构建项目的方式，可以帮助我们自动从本地和远程仓库拉取关联的jar包

<a name="DlJKi"></a>
## jdk和jre
1、java运行环境位于jre子目录中，window安装的时候会提示安装jre其实没必要，因为jdk包括了。<br />2、JRE(Java Runtime Environment) 是运行java程序所必须的环境集合，包含JVM标准、及java核心类库。<br />如果我们只是要运行java程序的话其实没必要安装jdk，只需要安装JRE就可以了。

<a name="mUsJ2"></a>
## jar如何生成和如何运行
jar包也是一样可以运行程序的。<br />以下是一段windows反射的命令执行，会弹出计算器弹框

> package test;
> 
> import java.lang.reflect.InvocationTargetException;
> public class RCE4 {
> 	public static void main(String[] args) throws IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException, ClassNotFoundException {
>       Object runtime=Class.forName("java.lang.Runtime").getMethod("getRuntime",new Class[]{}).invoke(null); //得到Runtime.getRuntime()函数
>       Class.forName("java.lang.Runtime").getMethod("exec", String.class).invoke(runtime,"calc.exe");  //执行函数
> 	}
> }

对test包右击

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612106683517-38faffc4-cca1-4b16-b0a9-056b87262320.png#align=left&display=inline&height=136&originHeight=272&originWidth=1284&size=29022&status=done&style=none&width=642)<br />然后点击Export文件，选择java中的JAR file<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612106778879-0cb117c3-4d3c-46e0-a961-45bba51a3687.png#align=left&display=inline&height=413&originHeight=550&originWidth=613&size=24432&status=done&style=none&width=460)

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612106812471-3acab25d-9d4f-4aad-8032-85e09529481c.png#align=left&display=inline&height=385&originHeight=770&originWidth=613&size=40396&status=done&style=none&width=306.5)<br />一路next之后，如果有很多java文件，需要导入入口函数main，我这里就一个java文件就不用瞎操心了。finish完成就生成jar包了。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612106863461-91443a00-755d-431f-a9e7-984bbfbf2bf9.png#align=left&display=inline&height=385&originHeight=770&originWidth=613&size=35602&status=done&style=none&width=306.5)<br />运行jar包也很简单。命令行有两种方式<br />方式一：有绑定主类（有main函数入口）<br />java -jar xxx.jar<br />方式二：没有绑定主类，可自行指定<br />java -cp xxx.jar xxx.main.Class

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612107043892-0fddbf46-c1b3-462f-93e4-949aee2c3f4e.png#align=left&display=inline&height=228&originHeight=455&originWidth=995&size=98720&status=done&style=none&width=497.5)<br />还有一种打包方式<br />jar -cvf helloworld.jar .<br />-c 表时要创建文件<br />-v 在控制台打印压缩详情<br />-f 指定压缩文件名<br />helloworld.jar 文件名可以自定义<br />. 表示helloworld目录下的所有文件，这里一定要写“.”，其他可能出错。<br />至此打包成功，但不能运行。想要可以运行还要修改helloword.jar中的MANIFEST.MF文件<br /> （3）使用解压缩工具打开helloword.jar并编辑META-INF/MANIFEST.MF添加属性<br />MANIFEST.MF初始状态
```
Manifest-Version: 1.0
Created-By: 1.8.0_161 (Oracle Corporation)
```
添加属性：（注意，冒号后有空格）<br />**Main-Class:** 包含main方法的类<br />**Class-Path** 依赖的jar包的路径，如果依赖多个jar包，使用空格隔开<br /> 路径：相对路径，jar包相对于helloworld.jar文件的路径<br />            绝对路径，jar包在操作系统中的路径<br /> 常用相对路径，将依赖的jar包和自己的jar包放在同一级目录下，这样Class-Path直接写依赖jar包的名字即可。<br />添加属性后；
```
Manifest-Version: 1.0
Created-By: 1.8.0_161 (Oracle Corporation)
Class-Path: mysql-connector-java-5.1.18.jar
Main-Class: com.gzn.demo.App
```
（4）运行测试<br /> 将依赖复制到helloworld.jar 同级目录下，使用 java -jar helloworld.jar 运行程序。

<a name="8tK2T"></a>
## jar包的源码
jar其实就是特殊的压缩包，直接解压也可以，或者用jd-gui直接脱源码<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612108934237-d29df64e-c5ae-4fc0-8762-3a7c5e86ec07.png#align=left&display=inline&height=400&originHeight=400&originWidth=855&size=25461&status=done&style=none&width=855)



<a name="jy3Jg"></a>
## WAR包如何生成和运行
war和jar的区别在于，war包能直接部署在tomcat的webapp目录下就能解析成web页面，jar需要解压。<br />

使用bin目录下的jar.exe文件<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612109421183-0080bb49-7454-480b-b175-4aa889d2b0fc.png#align=left&display=inline&height=210&originHeight=280&originWidth=682&size=20051&status=done&style=none&width=512)<br />jar -cvf aufeng.war shell.jsp<br />jar -cvf comment.war . <br />打包目录下所有文件

又或者是eclipse中的<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612111332699-d74fdd63-dedb-4c83-8688-397f9b5b87c5.png#align=left&display=inline&height=413&originHeight=550&originWidth=613&size=24574&status=done&style=none&width=460)<br />保存的路径名字<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612111385947-7ed9be99-7c4d-4d65-a582-768684053440.png#align=left&display=inline&height=413&originHeight=550&originWidth=613&size=24288&status=done&style=none&width=460)

也是解压就有源码的，运行也是一样java<br />java -jar *.war就可以了

<a name="0dL9e"></a>
## 关于java和jsp
其实两者差别不大，jsp是嵌套在<%里的java，主要区别还是在于参数传参的问题。<br />对比<br />jsp<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612439330599-116ee890-1685-4220-adb4-cb780f2badf3.png#align=left&display=inline&height=324&originHeight=432&originWidth=871&size=18963&status=done&style=none&width=653)<br />java<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612439343540-716945f0-5172-46dd-925d-5592cd769549.png#align=left&display=inline&height=298&originHeight=397&originWidth=621&size=16530&status=done&style=none&width=466)

<a name="h0Gnj"></a>
## map
[https://blog.csdn.net/taraex/article/details/90243965](https://blog.csdn.net/taraex/article/details/90243965)

<a name="KZO1X"></a>
## ysoseral、jrmp、rmi之类
这里边的ysoseral、JRMP、RMI，我也需要去学习

ysoserial是一款反序列化利用工具，里面集合了各种java反序列化。包含 CommonsCollections1-6、CommonsBeanutils1、FileUpload1 。它可以让⽤户根据⾃⼰选择的利⽤链，⽣成反序列化利⽤数据，通过将这些数据发送给⽬标，从⽽执⾏⽤户预先定义的命令。 gadget就是所谓的"利用链"<br />**<br />例如：<br />**java -jar ysoserial.jar CommonsCollections1 "id"**

查看ysoserial文档，都是各种利用链的代码<br />[https://github.com/frohoff/ysoserial/tree/master/src/main/java/ysoserial/payloads](https://github.com/frohoff/ysoserial/tree/master/src/main/java/ysoserial/payloads)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612926613881-b9b4679d-451f-4cc6-894a-b70ec3c37793.png#align=left&display=inline&height=431&originHeight=861&originWidth=731&size=42776&status=done&style=none&width=366)<br />JRMP:java remote method protocol，Java远程方法协议，一个对象的Java到Java的远程调用<br />RMI 指的是远程bai方法调用 (Remote Method Invocation)。它是du一种机制zhi，能够让在某个 Java 虚拟机dao上的对象调用另zhuan一个 Java 虚拟机中的对象上的方法<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612860770306-1f0104de-ea79-4eff-986e-50cdbd12ccfd.png#align=left&display=inline&height=188&originHeight=376&originWidth=936&size=32851&status=done&style=none&width=468)<br />JNDI是一个接口，在这个接口下会有多种目录系统服务的实现，通过名称等去找到相关的对象，并把它下载到客户端中来。 <br />ldap指轻量级目录访问协议<br />**使用RMI还是LDAP调用远程class恶意文件取决于目标机器上的JDK版本**

[https://www.freebuf.com/vuls/178014.html](https://www.freebuf.com/vuls/178014.html)<br />[https://www.freebuf.com/articles/web/252539.html](https://www.freebuf.com/articles/web/252539.html)<br />[https://www.freebuf.com/articles/web/220958.html](https://www.freebuf.com/articles/web/220958.html)

<a name="fVxMy"></a>
## object
Object类是所有类的父类<br />以下两种类的定义的最终效果是完全相同的
> class Person { }
> 
> class Person extends Object { }


- **Object类的hashcode方法**

不同对象的 hashCode() 的值一般是不相同。但是，同一个对象的hashCode() 值肯定相同<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612933900300-e0d6892c-5906-4177-a1cd-66135fb7373e.png#align=left&display=inline&height=299&originHeight=399&originWidth=527&size=16721&status=done&style=none&width=395)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612934480556-b5a9e5f6-ec35-4ba7-ad96-96e18b48cb71.png#align=left&display=inline&height=305&originHeight=407&originWidth=527&size=19701&status=done&style=none&width=395)

- **Object类的getClass()方法**

可以通过 Class 类中的一个方法，获取对象的真实类的全名称。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612934740871-82b76593-2ccb-4697-af0d-5df99683e03f.png#align=left&display=inline&height=277&originHeight=369&originWidth=520&size=16162&status=done&style=none&width=390)
<a name="cz3Z1"></a>
## 动态代理
[https://www.imooc.com/article/details/id/21339](https://www.imooc.com/article/details/id/21339)<br />[https://www.zhihu.com/question/20794107](https://www.zhihu.com/question/20794107)<br />[https://www.jianshu.com/p/9bcac608c714](https://www.jianshu.com/p/9bcac608c714)

实现动态代理需要做两方面的工作<br />必须新建一个类，并且这个类必须实现 InvocationHandler 接口。<br />在调用的时候使用 Proxy.newProxyInstance() 方法生成代理类。<br />最后直接使用生成的代理类调用相关的方法即可。

所有的handler都需要实现InvocationHandler这个接口，并实现其invoke方法来实现对接口的调用。<br />Java 动态代理的入口是从 Proxy.newInstance() 方法中开始的

参考[https://www.imooc.com/article/details/id/21339](https://www.imooc.com/article/details/id/21339)中的代码
```java
//做蛋糕的机器
public interface CakeMachine{
    void makeCake();
}

//专门做水果蛋糕的机器
class FruitCakeMachine implements CakeMachine{
    public void makeCake() {
        System.out.println("Making a fruit cake...");
    }
}

//专门做巧克力蛋糕的机器
public class ChocolateCakeMachine implements CakeMachine{
    public void makeCake() {
        System.out.printf("making a Chocolate Cake...");
    }
}

//做面包的机器
public interface BreadMachine {
    void makeBread();
}

//专门做红豆面包的机器
public class RedBeanBreadMachine implements BreadMachine {
    public void makeBread() {
        System.out.println("making red bean bread....");
    }
}

//专门做葡萄干面包的机器
public class CurrantBreadMachine implements BreadMachine{
    public void makeBread() {
        System.out.println("making currant bread...");
    }
}

//蛋糕店
public class CakeShop {
    public static void main(String[] args) {
        new FruitCakeMachine().makeCake();
        new ChocolateCakeMachine().makeCake();
        new RedBeanBreadMachine().makeBread();
        new CurrantBreadMachine().makeBread();
    }
}

--------------------------------------------------------------------------------------------------------------


//杏仁动态代理
public class ApricotHandler implements InvocationHandler{

    private Object object;

    public ApricotHandler(Object object) {
        this.object = object;
    }

    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        Object result = method.invoke(object, args);    //调用真正的蛋糕机做蛋糕
        System.out.println("adding apricot...");
        return result;
    }
}

//动态代理
newInstance、InvocationHandler

public class CakeShop {
    public static void main(String[] args) {
        //给蛋糕加上杏仁
        FruitCakeMachine fruitCakeMachine = new FruitCakeMachine();
        ApricotHandler apricotHandler = new ApricotHandler(fruitCakeMachine);
        CakeMachine cakeMachine = (CakeMachine) Proxy.newProxyInstance(fruitCakeMachine.getClass().getClassLoader(),
                fruitCakeMachine.getClass().getInterfaces(),
                apricotHandler);
        cakeMachine.makeCake();
}

```
<a name="C079i"></a>
## 不错的文章
[https://zhishihezi.net/b/98ae566719b21536dff0c4febaa697d2#open](https://zhishihezi.net/b/98ae566719b21536dff0c4febaa697d2#open) CC链的分析

<a name="QVVJd"></a>
## Java语法

**FileInputStream**<br />该流用于从文件读取数据

**FileOutputStream**<br />该类用来创建一个文件并向文件中写数据。
<a name="h25Lr"></a>
### 文件写入创建io
File f = new File      实例化文件的位置<br />FileOutputStream fop = new FileOutputStream(f);  文件输出流，可以理解为打开这个文本，然后用write实现写入。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623817851629-417a7ada-ebb9-4f79-b13f-0f9b26c2075b.png#align=left&display=inline&height=180&originHeight=240&originWidth=804&size=24384&status=done&style=none&width=603)<br />FileOutputStream不能直接写入字符串，只支持byte或者int，所以需要把字符串转化为字节传入。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623818941506-767d3737-a898-4f2e-91f9-cf149fe73093.png#align=left&display=inline&height=156&originHeight=208&originWidth=620&size=22661&status=done&style=none&width=465)
```java
public class Day4 {
	public static void main(String[] args) throws IOException {
        System.out.println("正在运行1");
        String str = "Hello World!!!" ;
        byte b[] = str.getBytes();
		File f = new File("C:\\Users\\19307\\eclipse-workspace\\first\\test2.txt");
		FileOutputStream fop = new FileOutputStream(f);
		fop.write(b);
		fop.close();
		System.out.println("end...");
}
}
```

如果简单方便的话，可以用FileWriter直接实现文件写入
```java
package News;
import java.io.*;

public class Day4 {
	public static void main(String[] args) throws IOException {
        System.out.println("正在运行1");
		File f = new File("C:\\Users\\19307\\eclipse-workspace\\first\\test2.txt");
		FileWriter writer=new FileWriter(f,true);
		writer.write("  My name is aufeng!\n 1111111111111111111111");
		writer.close();

		System.out.println("end...");
}
}
```
<a name="wiw4p"></a>
### 文件读取
FileReader 这个实现文件读取，直接fr.read()是没输出的<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623820430320-1ac5ced2-3124-4033-8762-85ba4c365f57.png#align=left&display=inline&height=515&originHeight=686&originWidth=933&size=54398&status=done&style=none&width=700)<br />需要把内容存进数组里，然后循环输出<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623820490569-9a8df596-1681-4297-81f0-54e7b3b455e4.png#align=left&display=inline&height=452&originHeight=602&originWidth=837&size=50747&status=done&style=none&width=628)
```java
package News;
import java.io.*;

public class Day4 {
	public static void main(String[] args) throws IOException {
        System.out.println("正在运行1");
        String str = "Hello World!!!" ;
        byte b[] = str.getBytes();
		File f = new File("C:\\Users\\19307\\eclipse-workspace\\first\\test2.txt");
		FileOutputStream fop = new FileOutputStream(f);
		fop.write(b);
		fop.close();
		System.out.println("end...");
        FileReader fr = new FileReader(f);
        char[] a = new char[50];
        fr.read(a);
        for (char c : a)
            System.out.print(c); // 一个一个打印字符
        fr.close();
        
        }
}
```

BufferedReader+FileReader 读取文件内容
```java
package News;
import java.io.*;

public class Day4 {
	public static void main(String[] args)  {
        try {
            BufferedReader in = new BufferedReader(new FileReader("C:\\Users\\\\19307\\\\eclipse-workspace\\\\first\\\\test2.txt"));
            String str;
            while ((str = in.readLine()) != null) {
                System.out.println(str);
            }
            System.out.println(str);
        } catch (IOException e) {
        }
    }
}
```

