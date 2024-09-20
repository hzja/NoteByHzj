Java
<a name="UeCpb"></a>
## 一、基础概念
Java中，一般创建一个对象可能会选择new一下个实例。但是随着技术的不断提升，也学习到了，可以通过反射技术实现对象的创建。<br />有没有想一下，什么时候改用`new`创建对象，什么时候改用反射创建对象呢？<br />两者创建对象的效率又是如何呢？
```java
//new 方式创建对象
ReflectDemo reflectDemo = new ReflectDemo();
//反射创建对象  反射创建对象的三种方式
(1)Class<ReflectDemo> reflectDemoClass = ReflectDemo.class;
(2)Class<?> aClass = Class.forName ("com.whale.springtransaction.transactiondemo.reflectdemo.ReflectDemo");
(3)Class<? extends Class> aClass = reflectDemoClass.getClass ();
```
<a name="EVKal"></a>
## 二、new 对象和反射创建对象的效率对比
```java
//测试代码如下
public class ReflectDemo {

    public static void main (String[] args) throws IllegalAccessException, InstantiationException {
        proxyObject();
        newObject();
    }

    //new 创建对象
    //5
    public static void newObject(){
        long startTime = System.currentTimeMillis ();
        int i;
        for (i = 0; i < 100000000; i++) {
            ReflectDemo reflectDemo = new ReflectDemo ();
        }
        if (i == 100000000) {
            long endTime = System.currentTimeMillis ();
            System.out.println ("new耗时为:" + (endTime - startTime));
        }
    }

    //反射 创建对象
    //30
    public static void proxyObject() throws IllegalAccessException, InstantiationException {
        long startTime = System.currentTimeMillis ();
        Class<ReflectDemo> reflectDemoClass = ReflectDemo.class;
        int i;
        for (i = 0; i < 100000000; i++) {


            ReflectDemo reflectDemo = reflectDemoClass.newInstance ();
        }
        if (i == 100000000) {
            long endTime = System.currentTimeMillis ();
            System.out.println ("反射耗时为:" + (endTime - startTime));
        }
    }
}
```
![2022-12-01-21-28-39.201531600.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669901337033-dd8948a1-ea54-40b1-8ad8-b300e68f5a7d.png#averageHue=%239f8560&clientId=u3e699093-f904-4&from=ui&id=ued3f7526&originHeight=227&originWidth=775&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12706&status=done&style=none&taskId=u905f2980-52f5-45b5-bcb2-515bb5f5d5b&title=)<br />最终可以发现，new **100000000** 个对象和反射创建 **100000000** 个对象，效率相差了很多倍。<br />所以下面来探讨一下为什么这么大差别?<br />**首先第一点，一般的Java代码是需要编译后在虚拟机里面运行的。**<br />首先一般都是通过一个前端编辑器，比如javac，把java文件转为class文件。<br />接下来，程序运行期间，可能会通过一个JIT，即时编译器将字节码文件转换为计算机认识的机器码文件。<br />另外一种可能是通过一个AOT编译器，直接把java文件编译为本地机器码文件。其中JIT在程序运行期会对程序进行优化，但是反射是通过动态解析的方式，因此可能无法执行某些java虚拟机的优化。<br />总结起来有下面几个原因:

1. `Method#invoke` 方法会对参数做封装和解封操作
2. 需要检查方法可见；
3. 需要校验参数；
4. 反射方法难以内联；
5. JIT无法优化；
<a name="gVkE3"></a>
## 三、反射和new 的使用场景
<a name="gSTw4"></a>
### 反射的部分使用场景 

1. Spring通过反射来实例化对象，并放入到Ioc容器中
2. 使用JDBC链接数据库时加载数据库驱动`Class.forName()`
3. 逆向代码 例如反编译
4. 利用反射，在泛型为int的arryaList集合中存放一个String类型的对象
<a name="KzdVZ"></a>
### new 对象和反射的区别

1. new的对象无法访问其中的私有属性，反射出来的可以通过设置`setAccessible()`方法来省略访问权限符。
2. new必须要知道类名，而反射创建对象不需要知道类型也可以创建
