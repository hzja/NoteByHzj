<a name="qD8WU"></a>
# java反射和cc链的关系
- java反射就是运行时检查、获取和操作类的信息，包括类名、字段、方法、构造函数
- commons-collections(cc链) 只是Apache 中的一个开发库
- Java反射和Commons Collections 是两个不同的概念，但它们可以在某些情况下一起使用，比如Commons Collections在特定版本下存在漏洞，攻击者可以会通过恶意构造的序列化数据触发远程代码执行漏洞
<a name="rlzoF"></a>
# Java反射的学习笔记
什么是java反射<br />大白话讲就是反向获取class中各种对象的信息、例如成员变量、方法、构造方法、包等等信息<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188607987-cb2abf60-5a75-403a-bea1-20550fc18d73.png#averageHue=%23faf8f8&height=295&id=UZwgj&originHeight=566&originWidth=1433&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=746)<br />图片来自：<br />[https://blog.csdn.net/sinat_38259539/article/details/71799078?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param](https://blog.csdn.net/sinat_38259539/article/details/71799078?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param)
<a name="mk32z"></a>
## 获得到类的字节码有3种方式
要想反射，首先第一步就是得到类的字节码，获得到类的字节码有3种方式<br />1、<br />Student stu1 = **new** Student();//这一new 产生一个Student对象，一个Class对象。<br />Class stuClass = stu1.getClass();//获取Class对象<br /> <br />2、<br />Class stuClass2 = Student.**class**;<br /> <br />3、<br />Class stuClass3 = Class._forName_("first.Student");//注意此字符串必须是真实路径，就是带包名的类路径，包名.类名<br /> <br />例子如下，先创建一个Student类
> **package** first;
>  
> **public** **class** Student {
>    
>     //---------------构造方法-------------------
>     //（默认的构造方法）
>     Student(**char** str){
>        System.**_out_**.println("(默认)的构造方法 s = " + str);
>     }
>    
>     //无参构造方法
>     **public** Student(){
>        System.**_out_**.println("调用了公有、无参构造方法执行了。。。");
>     }
>    
>     //有一个参数的构造方法
>     **public** Student(String name){
>        System.**_out_**.println("姓名：" + name);
>     }
>    
>     //有多个参数的构造方法
>     **public** Student(String name ,**int** age){
>        System.**_out_**.println("姓名："+name+"年龄："+ age);//这的执行效率有问题，以后解决。
>     }
>    
>     //受保护的构造方法
>     **protected** Student(**boolean** n){
>        System.**_out_**.println("受保护的构造方法 n = " + n);
>     }
>    
>     //私有构造方法
>     **private** Student(**int** age){
>        System.**_out_**.println("私有的构造方法   年龄："+ age);
>     }
>  }


 <br /> <br />构造反射
> **package** first;
>  
> **public** **class** Fanshe {
>     **public** **static** **void** main(String[] args) {
>        //第一种方式获取Class对象 
>        Student stu1 = **new** Student();//这一new 产生一个Student对象，一个Class对象。
>        Class stuClass = stu1.getClass();//获取Class对象
>        System.**_out_**.println(stuClass.getName());
>       
>        //第二种方式获取Class对象
>        Class stuClass2 = Student.**class**;
>        System.**_out_**.println(stuClass == stuClass2);//判断第一种方式获取的Class对象和第二种方式获取的是否是同一个
>       
>        //第三种方式获取Class对象
>        **try** {
>            Class stuClass3 = Class._forName_("first.Student");//注意此字符串必须是真实路径，就是带包名的类路径，包名.类名
>            System.**_out_**.println(stuClass3 == stuClass2);//判断三种方式是否获取的是同一个Class对象
>        } **catch** (ClassNotFoundException e) {
>            e.printStackTrace();
>        }
>       
>     }
> }

![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188608310-77d9958c-09cf-4d25-ba40-aaa0afdd3b5c.png#averageHue=%23f9f4f3&height=598&id=Zs5CA&originHeight=797&originWidth=905&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=679)<br /> <br /> <br /> <br /> 
<a name="hIpCH"></a>
## 反射有参构造函数
Class stuClass3 = Class._forName_("first.Student");// 获取类的 Class 对象实例<br />Constructor c = stuClass3.getConstructor(String.**class**);// 根据 Class 对象实例获取 Constructor 对象<br />c.newInstance("abc");<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188608628-2c371c7a-9b13-458b-8843-2c4907364998.png#averageHue=%23f7f2f1&height=334&id=gSSz3&originHeight=445&originWidth=866&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=650)
<a name="d411R"></a>
## 反射“私有”的构造函数
Class stuClass3 = Class._forName_("first.Student");<br />Constructor c = stuClass3.getDeclaredConstructor(**int**.**class**);<br />c.setAccessible(**true**); (暴力反射)<br />c.newInstance(20);<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188608864-a6257e5e-9ae1-48cb-bcd3-a66fe741f6f3.png#averageHue=%23f6f4f2&height=343&id=yNgTS&originHeight=457&originWidth=846&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=635)<br /> 
<a name="rZMrz"></a>
## 反射得到类中所有的构造函数 getDeclaredConstructors

**但是反射出来的顺序是倒序的。**<br />[https://blog.csdn.net/qq_39005688/article/details/102536921](https://blog.csdn.net/qq_39005688/article/details/102536921)

Class stuClass3 = Class._forName_("first.Student");<br />Constructor[] cs=stuClass3.getDeclaredConstructors();<br />**for**(Constructor c:cs) {<br />    System.**_out_**.println(c);<br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188609219-c7bc8f94-b671-43ce-98f1-9d5dba56970f.png#averageHue=%23f7f6f4&height=406&id=XEQjg&originHeight=541&originWidth=868&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=651)<br />[https://blog.csdn.net/qq_39005688/article/details/102536921](https://blog.csdn.net/qq_39005688/article/details/102536921)
<a name="d3MKm"></a>
## 反射类中的方法(重要)
Class stuClass3 = Class._forName_("first.Student");<br />Student p = (Student)stuClass3.newInstance();<br />Method m = stuClass3.getMethod("Student1",String.**class**,**int**.**class**);// 获取方法的 Method 对象<br />m.invoke(p,"aufeng",20);// 利用 invoke 方法调用方法<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188609464-7ace14af-57fc-43fb-b624-a8cd72b26fad.png#averageHue=%23f7f4f2&height=373&id=DheHR&originHeight=497&originWidth=872&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=654)

getMethod("Student1",String.**class**,**int**.**class**) 这里的Student1，String.class，int.class分别指<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612283279031-fc10eaea-451b-46dc-a9df-5b4270455990.png#averageHue=%23fcf9f8&height=427&id=zzWT2&originHeight=569&originWidth=685&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33269&status=done&style=none&title=&width=514)<br />直接写成这样子也可以<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1613711318382-4e53c21e-4f16-490b-a21f-c5d901186d59.png#averageHue=%23f7f6f5&height=371&id=KpANS&originHeight=494&originWidth=1012&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34577&status=done&style=none&title=&width=759)<br />stuClass3.getMethod("Student1",String.class,int.class).invoke((Student)stuClass3.newInstance(),"aufeng",20);<br />与反射的命令对比<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612194480430-c52608f8-df98-4fd6-9da2-89563303832b.png#averageHue=%23f9f8f7&height=240&id=frLmL&originHeight=240&originWidth=1629&originalType=binary&ratio=1&rotation=0&showTitle=false&size=168704&status=done&style=none&title=&width=1629)<br />获取类的⽅法： forName <br />实例化类对象的⽅法： newInstance <br />获取函数的⽅法： getMethod <br />执⾏函数的⽅法： invoke

例如runtime命令执行的反射
```java
package Test;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class test {
    public static void main(String[] args) throws IOException, ClassNotFoundException, NoSuchMethodException, InvocationTargetException, IllegalAccessException, InstantiationException {
        //反射命令执行
        Class clazz = Class.forName("java.lang.Runtime");
        Method getRuntimeMethod = clazz.getMethod("getRuntime");
        Method execMethod = clazz.getMethod("exec", String.class);
        Object runtime = getRuntimeMethod.invoke(clazz);
        execMethod.invoke(runtime, "calc.exe");


        //直接命令执行的方法
        String [] cmd = {"cmd","/C","calc"};
        Process proc = Runtime.getRuntime().exec(cmd);
    }
}
```

<a name="5f8nx"></a>
## 反射类中的属性字段
Class stuClass3 = Class._forName_("first.Student");<br />           Student p =(Student)stuClass3.newInstance();<br />           Field f =stuClass3.getField("name");//反射类中的属性字段<br />           String s=(String)f.get(p);<br />           System.**_out_**.println(s);<br />          <br />           f.set(p, "aaaffff");//修改strings值<br />           System.**_out_**.println(p.name);<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612188609757-905a8e33-1ee4-4956-9e4f-cb82b4ca6ad7.png#averageHue=%23f8f3f1&height=435&id=OgybP&originHeight=580&originWidth=867&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=650)<br /> 



参考文章<br />[https://blog.csdn.net/ju_362204801/article/details/90578678](https://blog.csdn.net/ju_362204801/article/details/90578678)
