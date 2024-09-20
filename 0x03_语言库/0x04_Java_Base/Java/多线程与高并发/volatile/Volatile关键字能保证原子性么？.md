JavaVolatile<br />做Java开发的，在面试的时候，如果涉及到多线程，那么面试官有不少人会询问关于 volatile 这个关键字的使用，以及他的作用，来看看这个 volatile 关键的的作用，以及他的一些特性。
<a name="jdY4M"></a>
## volatile
volatile 是 Java 中的一个相对来说比较重要的关键字，主要就是用来修饰会被不同线程访问和修改的变量。<br />而这个变量只能保证两个特性，一个是保证有序性，另外一个则是保证可见性。<br />那么什么是有序性，什么又是可见性呢？
<a name="Ayu2f"></a>
### 有序性
那么什么是有序性呢？<br />其实程序执行的顺序按照代码的先后顺序执行，禁止进行指令重排序。<br />看似理所当然，其实并不是这样，指令重排序是JVM为了优化指令，提高程序运行效率，在不影响单线程程序执行结果的前提下，尽可能地提高并行度。<br />但是在多线程环境下，有些代码的顺序改变，有可能引发逻辑上的不正确。<br />而 volatile 就是因为有这个特性，所以才被大家熟知的。<br />volatile 又是如何保证有序性的呢？<br />有很多小伙伴就说，网上说的是 volatile 可以禁止指令指令重排序，这就保证了代码的程序会严格按照代码的先后顺序执行。这就保证了有序性。被 volatile 修饰的变量的操作，会严格按照代码顺序执行，就是说当代码执行到 volatile 修饰的变量时，其前面的代码一定执行完毕，后面的代码一定没有执行。<br />如果这时候，面试官不再继续深挖下去的话，那么可能这个问题已经回答完了，但是如果面试官继续往下深挖，为什么会禁止指令重排，什么又是指令重排呢？<br />在从源码到指令的执行，一般是分成了三种重排，如图所示<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660697305445-0337ffb0-515d-4fef-8d3a-74903bb70d2b.png#averageHue=%23f8f8f8&clientId=ue6e1e1b8-b406-4&from=paste&height=671&id=u359a6cee&originHeight=1342&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u62bbdc1c-d0ab-44be-983a-245d223308f&title=&width=236)<br />接下来就得看看 volatile 是如何禁止指令重排的。<br />直接用代码来进行验证。
```java
public class ReSortDemo {

    int a = 0;
    boolean flag = false;

    public void mehtod1(){
        a = 1;
        flag = true;
    }

    public void method2(){
        if(flag){
            a = a +1;
            System.out.println("最后的值: "+a);
        }
    }
}
```
如果有人看到这段代码，肯定会说，那这段代码出来的结果会是什么呢？<br />有些人说是 2，是的， 如果只是单线程调用，那结果就是 2，但是如果是多线程调用的时候，最后的输出结果不一定是想象到的 2，这时就要把两个变量都设置为 volatile。<br />如果大家对单例模式了解比较多的话，肯定也是关注过这个 volatile，为什么呢？<br />大家看看如下代码，
```java
class Singleton {
    // 不是一个原子性操作
    //private static Singleton instance;
    //改进，Volatile 可以保持可见性，不能保证原子性，由于内存屏障，可以保证避免指令重排的现象产生！
    private static volatile Singleton instance;

    // 构造器私有化
    private Singleton() {
    }

    // 提供一个静态的公有方法，加入双重检查代码，解决线程安全问题, 同时解决懒加载问题，同时保证了效率, 推荐使用
    public static Singleton getInstance() {
        if (instance == null) {
            synchronized (Singleton.class) {
                if (instance == null) {
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```
上面的单例模式大家熟悉么？<br />是的，这就是 **双重检查(DCL 懒汉式) **<br />有人会说，因为有指令重排序的存在，双端检索机制也也不一定是线程安全的呀，对呀，所以这里用到了 synchronized 关键字，让他变成了线程安全的了。
<a name="sr1fp"></a>
### 可见性
其实可见性就是，在多线程环境中，对共享变量的修改对于其他线程是否立即可见的问题。<br />那么他的可见性一般都会表现在什么地方呢？用在什么地方呢？<br />一般用这个变量，很多都是为了保证他的可见性，就比如定义的一个全局变量，在其中有个循环来判断这个变量的值，有一个线程修改了这个参数的时候，这个循环会停止，跳转到之后去执行。<br />来看看没有使用volatile修饰代码实现：
```java
public class Test {

    private static boolean flag = false;

    public static void main(String[] args) throws Exception{
        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("线程A开始执行:");
                for (;;){
                    if (flag){
                        System.out.println("跳出循环");
                        break;
                    }
                }
            }
        }).start();
        Thread.sleep(100);

        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("线程B开始执行");
                flag = true;
                System.out.println("标识已经变更");
            }
        }).start();
    }

}
```
结果大家肯定是可想而知，运行结果肯定是
```java
线程A开始执行:
线程B开始执行
标识已经变更
```
确实，就是这样的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660697305448-aa8af2fd-2783-4850-87aa-0d16c63e0a5e.png#averageHue=%232f2f2f&clientId=ue6e1e1b8-b406-4&from=paste&id=u9e7384ae&originHeight=135&originWidth=299&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud6852095-1c6d-4483-8704-53d14298c43&title=)<br />如果用 volatile 呢，那么这个代码的执行结果就会不一样呢？<br />来试一下：
```java
public class Test {

    private static volatile boolean flag = false;

    public static void main(String[] args) throws Exception{
        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("线程A开始执行:");
                for (;;){
                    if (flag){
                        System.out.println("跳出循环");
                        break;
                    }
                }
            }
        }).start();
        Thread.sleep(100);

        new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("线程B开始执行");
                flag = true;
                System.out.println("标识已经变更");
            }
        }).start();
    }
```
这样就能看到另外一个执行结果，在循环当中的输出语句是可以被执行的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660697305503-1fe0d178-7d3d-4403-91da-992af2dc6a22.png#averageHue=%232e2e2e&clientId=ue6e1e1b8-b406-4&from=paste&id=u005457a6&originHeight=132&originWidth=475&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u38ff11f6-08f5-4f05-be28-c239620ec74&title=)<br />也就是说，在线程B 中，去修改这个被修饰的变量，那么最终，在线程A中，就能顺利读取到数据信息了。
<a name="nKktt"></a>
## 是否能够保证原子性
不能，来看一点代码，被volatile修饰的变量，
```java
public class Test {

    // volatile不保证原子性
    // 原子性：保证数据一致性、完整性
    volatile int number = 0;

    public void addPlusPlus() {
        number++;
    }

    public static void main(String[] args) {
        Test volatileAtomDemo = new Test();
        for (int j = 0; j < 20; j++) {
            new Thread(() -> {
                for (int i = 0; i < 1000; i++) {
                    volatileAtomDemo.addPlusPlus();
                }
            }, String.valueOf(j)).start();
        }// 后台默认两个线程：一个是main线程，一个是gc线程
        while (Thread.activeCount() > 2) {
            Thread.yield();
        }
        // 如果volatile保证原子性的话，最终的结果应该是20000 // 但是每次程序执行结果都不等于20000
        System.out.println(Thread.currentThread().getName() +
                " final number result = " + volatileAtomDemo.number);
    }

}
```
如果能够保原子性，那么最终的结果应该是20000，但是每次的最终结果并不能保证就是20000，比如：
```java
main final number result = 17114
main final number result = 20000
main final number result = 19317
```
三次执行，都是不同的结果，<br />为什么会出现这种呢？这就和`number++`有关系了<br />`number++`被拆分成3个指令

- **执行**`**GETFIELD**`**拿到主内存中的原始值number**
- **执行**`**IADD**`**进行加1操作**
- **执行**`**PUTFIELD**`**把工作内存中的值写回主内存中**

当多个线程并发执行`PUTFIELD`指令的时候，会出现写回主内存覆盖问题，所以才会导致最终结果不为 20000，所以 `volatile` 不能保证原子性。
