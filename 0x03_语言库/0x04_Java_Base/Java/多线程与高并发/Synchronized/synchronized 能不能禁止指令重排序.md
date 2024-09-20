Java

- I）、指令重排序
   - 1、问题描述
   - 2、DCL代码字节码分析指令重排序问题
- II）、有序性
   - 1、代码示例

---

<a name="KotiM"></a>
## **指令重排序**
<a name="bIDRc"></a>
### 问题描述
首先一定要明确：指令重排序和有序性是不一样的。这一点非常重要。<br />经常都会这么说：

- `volatile`能保证内存可见性、禁止指令重排序但是不能保证原子性。
- `synchronized`能保证原子性、可见性和有序性。

注意：这里的有序性并不是代表能禁止指令重排序。<br />举个例子：<br />在双重检查的单例模式中，既然已经加了`synchronized`为什么还需要`volatile`去修饰变量呢？如果`synchronized`能禁止指令重排，那么完全可以不用要`volatile`。
<a name="nWDW0"></a>
### DCL代码字节码分析指令重排序问题
首先需要知道的知识点：`Object obj = new Object();`这句代码并不是一个原子操作，他分为三步：

- 在内存申请一片空间，new 出一个对象
- 调用new出的这个对象的构造方法
- 将这个对象的引用赋值给obj
<a name="G2bfO"></a>
#### a)、DCL双重检查代码
```java
public class MySingleton {

    private static MySingleton INSTANCE;

    private MySingleton() {
    }

    public static MySingleton getInstance() {
        if (INSTANCE == null) {
            synchronized (MySingleton.class) {
                if (INSTANCE == null) {
                    INSTANCE = new MySingleton();
                }
            }
        }
        return INSTANCE;
    }
}
```
<a name="NzTE7"></a>
#### b)、字节码如下
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1694568951887-21ff8195-1f07-401d-8beb-ad54a4c107b3.jpeg#averageHue=%23f0e9e0&clientId=uec6c4c82-9f6d-4&from=paste&id=c2IAm&originHeight=394&originWidth=1072&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc067199a-7c78-49e5-b4c5-8850e62e32a&title=)<br />从字节码中可以看到，`new MySingleton();`这句代码对应了17、20、21、24这四行字节码（20行是一个引用的拷贝，可以忽略）。

- 首先在17行在内存中开辟一块空间创建一个MySingleton对象。
- 然后在21行调用该对象的构造方法。
- 然后在24行将该对象的引用赋值给静态变量INSTANCE。

以上是期望的执行顺序，希望每个线程都按照该顺序去执行指令（这就是禁止指令重排序）。但是由于计算机为了提高运行效率，会将指令顺序进行优化重排（比如上面的顺序可能会优化重排为：17、24、21）<br />**指令重排序带来的问题**

- 计算机为了提升效率，会将代码顺序做一些优化，比如在t1线程中的执行顺序是 17、24、21，在t2线程中执行的顺序是17、21、24 （在单个线程中不管是那种执行顺序都不会有问题）。
- 当t1线程获取到锁执行对象创建的时候，先执行了24行，将该对象的引用赋值给了静态变量INSTANCE（此时对象还没调用构造方法，该对象还不是一个完整的对象）。
- 此时t2线程开始运行了，当t2线程执行到`if (INSTANCE == null)`(第16行代码)语句的时候，t2线程发现INSTANCE不为空，此时t2线程直接返回INSTANCE对象。但是此时该对象还是一个不完整的对象，在t2线程使用该对象的时候就会出现问题。

所以说指令重排序在单线程中是不会有任何问题的，但是一旦涉及到多线程的情况，那么指令重排序可能会带来意想不到的结果。
<a name="X1T7V"></a>
## **有序性**
那么既然`synchronized`不能禁止指令重排序，那么他保证的有序性是什么有序呢？<br />它的本质是让多个线程在调用`synchronized`修饰的方法时，由并行（并发）变成串行调用，谁获得锁谁执行。
<a name="zcDpy"></a>
### 代码示例
t1、t2两个线程都需要去获取单例对象，然后调用test方法，并且test方法是加了同步锁的方法。
```java
public class MySingleton {

    private static MySingleton INSTANCE;

    private MySingleton() {
    }

    public static MySingleton getInstance() {
        if (INSTANCE == null) {
            synchronized (MySingleton.class) {
                if (INSTANCE == null) {
                    INSTANCE = new MySingleton();
                }
            }
        }
        return INSTANCE;
    }

    public static void test(final MySingleton singleton) {
        synchronized (MySingleton.class) {
            System.out.println(singleton);
        }
    }
}
```
测试代码
```java
public class MySingletonTest {
    // 可以看到两个线程都需要去获取单例对象，然后调用test方法，并且test方法是加了同步锁的方法
    public static void main(final String[] args) {
        new Thread(() -> {
            MySingleton instance = MySingleton.getInstance();
            MySingleton.test(instance);
        }, "t1").start();
        new Thread(() -> {
            MySingleton instance = MySingleton.getInstance();
            MySingleton.test(instance);
        }, "t2").start();
    }
}
```
即使是t2线程获得了未调用构造函数的对象，那么在t2线程中再去调用`MySingleton.test(instance);`方法的时候，也并不会出现任何问题，因为使用了同步锁，每个一加锁执行的方法都变成了串行，将并发执行变成了串行，当t2线程获取到锁然后执行的时候，t1早已经释放了锁，此时instance也已经早就被实例化好了。所以不会出现问题。<br />**所以synchronized保证顺序性是指的将并发执行变成了串行，但并不能保证内部指令重排序问题。**
