Java Synchronize<br />介绍Synchronize的8种同步方法的访问场景，在这8种情况下，多线程访问同步方法是否还是线程安全的。这些场景是多线程编程中经常遇到的，而且也是面试时高频被问到的问题，所以不管是理论还是实践，这些都是多线程场景必须要掌握的场景。
<a name="9eyHn"></a>
## 八种使用场景：
接下来通过代码实现，分别判断以下场景是不是线程安全的，以及原因是什么。

1. 两个线程同时访问同一个对象的同步方法<br />
2. 两个线程同时访问两个对象的同步方法<br />
3. 两个线程同时访问（一个或两个）对象的静态同步方法<br />
4. 两个线程分别同时访问（一个或两个）对象的同步方法和非同步方法<br />
5. 两个线程访问同一个对象中的同步方法，同步方法又调用一个非同步方法<br />
6. 两个线程同时访问同一个对象的不同的同步方法<br />
7. 两个线程分别同时访问静态synchronized和非静态synchronized方法<br />
8. 同步方法抛出异常后，JVM会自动释放锁的情况<br />
<a name="9xPJv"></a>
### 场景一：两个线程同时访问同一个对象的同步方法
分析：这种情况是经典的对象锁中的方法锁，两个线程争夺同一个对象锁，所以会相互等待，是线程安全的。
:::info
**「两个线程同时访问同一个对象的同步方法，是线程安全的。」**
:::
<a name="cQ4Tp"></a>
### 场景二：两个线程同时访问两个对象的同步方法
这种场景就是对象锁失效的场景，原因出在访问的是两个对象的同步方法，那么这两个线程分别持有的两个线程的锁，所以是互相不会受限的。加锁的目的是为了让多个线程竞争同一把锁，而这种情况多个线程之间不再竞争同一把锁，而是分别持有一把锁，所以结论是：
:::info
**「两个线程同时访问两个对象的同步方法，是线程不安全的。」**
:::
<a name="t69Lc"></a>
#### 代码验证
```java
public class Condition2 implements Runnable {  
    // 创建两个不同的对象  
    static Condition2 instance1 = new Condition2();  
    static Condition2 instance2 = new Condition2();  

    @Override  
    public void run() {  
        method();  
    }  

    private synchronized void method() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，运行结束");  
    }  

    public static void main(String[] args) {  
        Thread thread1 = new Thread(instance1);  
        Thread thread2 = new Thread(instance2);  
        thread1.start();  
        thread2.start();  
        while (thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }  
}
```
<a name="4MSe1"></a>
#### 运行结果
两个线程是并行执行的，所以线程不安全。
```
线程名：Thread-0，运行开始  
线程名：Thread-1，运行开始  
线程：Thread-0，运行结束  
线程：Thread-1，运行结束  
测试结束
```
<a name="47dYV"></a>
#### 代码分析
**「问题在此：」**<br />两个线程（thread1、thread2），访问两个对象（instance1、instance2）的同步方法（method()）,两个线程都有各自的锁，不能形成两个线程竞争一把锁的局势，所以这时，synchronized修饰的方法method()和不用synchronized修饰的效果一样（不信去把synchronized关键字去掉，运行结果一样），所以此时的method()只是个普通方法。<br />**「如何解决这个问题：」**<br />若要使锁生效，只需将method()方法用static修饰，这样就形成了类锁，多个实例（instance1、instance2）共同竞争一把类锁，就可以使两个线程串行执行了。这也就是下一个场景要讲的内容。
<a name="NxzJi"></a>
### 场景三：两个线程同时访问（一个或两个）对象的静态同步方法
这个场景解决的是场景二中出现的线程不安全问题，即用类锁实现：<br />**「两个线程同时访问（一个或两个）对象的静态同步方法，是线程安全的。」**
<a name="y4Nrw"></a>
### 场景四：两个线程分别同时访问（一个或两个）对象的同步方法和非同步方法
这个场景是两个线程其中一个访问同步方法，另一个访问非同步方法，此时程序会不会串行执行呢，也就是说是不是线程安全的呢？<br />可以确定是线程不安全的，如果方法不加`synchronized`都是安全的，那就不需要同步方法了。验证下结论：
:::info
**「两个线程分别同时访问（一个或两个）对象的同步方法和非同步方法，是线程不安全的。」**
:::
```java
public class Condition4 implements Runnable {  

    static Condition4 instance = new Condition4();  

    @Override  
    public void run() {  
        //两个线程访问同步方法和非同步方法  
        if (Thread.currentThread().getName().equals("Thread-0")) {  
            //线程0,执行同步方法method0()  
            method0();  
        }  
        if (Thread.currentThread().getName().equals("Thread-1")) {  
            //线程1,执行非同步方法method1()  
            method1();  
        }  
    }  

    // 同步方法  
    private synchronized void method0() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，同步方法，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，同步方法，运行结束");  
    }  

    // 普通方法  
    private void method1() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，普通方法，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，普通方法，运行结束");  
    }  

    public static void main(String[] args) {  
        Thread thread1 = new Thread(instance);  
        Thread thread2 = new Thread(instance);  
        thread1.start();  
        thread2.start();  
        while (thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }  
}
```
<a name="XvJGL"></a>
#### 运行结果
两个线程是并行执行的，所以是线程不安全的。
```
线程名：Thread-0，同步方法，运行开始  
线程名：Thread-1，普通方法，运行开始  
线程：Thread-0，同步方法，运行结束  
线程：Thread-1，普通方法，运行结束  
测试结束
```
<a name="DI9Zi"></a>
#### 结果分析
问题在于此：method1没有被synchronized修饰，所以不会受到锁的影响。即便是在同一个对象中，当然在多个实例中，更不会被锁影响了。结论：
:::info
**「非同步方法不受其它由synchronized修饰的同步方法影响」**
:::
可能想到一个类似场景：多个线程访问同一个对象中的同步方法，同步方法又调用一个非同步方法，这个场景会是线程安全的吗？
<a name="pyd6S"></a>
### 场景五：两个线程访问同一个对象中的同步方法，同步方法又调用一个非同步方法
来实验下这个场景，用两个线程调用同步方法，在同步方法中调用普通方法；再用一个线程直接调用普通方法，看看是否是线程安全的？
```java
public class Condition8 implements Runnable {  

    static Condition8 instance = new Condition8();  

    @Override  
    public void run() {  
        if (Thread.currentThread().getName().equals("Thread-0")) {  
            //直接调用普通方法  
            method2();  
        } else {  
            // 先调用同步方法，在同步方法内调用普通方法  
            method1();  
        }  
    }  

    // 同步方法  
    private static synchronized void method1() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，同步方法，运行开始");  
        try {  
            Thread.sleep(2000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，同步方法，运行结束,开始调用普通方法");  
        method2();  
    }  

    // 普通方法  
    private static void method2() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，普通方法，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，普通方法，运行结束");  
    }  

    public static void main(String[] args) {  
        // 此线程直接调用普通方法  
        Thread thread0 = new Thread(instance);  
        // 这两个线程直接调用同步方法  
        Thread thread1 = new Thread(instance);  
        Thread thread2 = new Thread(instance);  
        thread0.start();  
        thread1.start();  
        thread2.start();  
        while (thread0.isAlive() || thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }  

}
```
<a name="pr5ns"></a>
#### 运行结果
```
线程名：Thread-0，普通方法，运行开始  
线程名：Thread-1，同步方法，运行开始  
线程：Thread-1，同步方法，运行结束,开始调用普通方法  
线程名：Thread-1，普通方法，运行开始  
线程：Thread-0，普通方法，运行结束  
线程：Thread-1，普通方法，运行结束  
线程名：Thread-2，同步方法，运行开始  
线程：Thread-2，同步方法，运行结束,开始调用普通方法  
线程名：Thread-2，普通方法，运行开始  
线程：Thread-2，普通方法，运行结束  
测试结束
```
<a name="vGDSe"></a>
#### 结果分析
可以看出，普通方法被两个线程并行执行，不是线程安全的。这是为什么呢？<br />因为如果非同步方法，有任何其他线程直接调用，而不是仅在调用同步方法时，才调用非同步方法，此时会出现多个线程并行执行非同步方法的情况，线程就不安全了。<br />对于同步方法中调用非同步方法时，要想保证线程安全，就必须保证非同步方法的入口，仅出现在同步方法中。但这种控制方式不够优雅，若被不明情况的人直接调用非同步方法，就会导致原有的线程同步不再安全。所以不推荐在项目中这样使用，但要理解这种情况，并且要用语义明确的、让人一看就知道这是同步方法的方式，来处理线程安全的问题。<br />所以，最简单的方式，是在非同步方法上，也加上`synchronized`关键字，使其变成一个同步方法，这样就变成了《场景五：两个线程同时访问同一个对象的不同的同步方法》，这种场景下，大家就很清楚的看到，同一个对象中的两个同步方法，不管哪个线程调用，都是线程安全的了。<br />所以结论是：
:::info
**「两个线程访问同一个对象中的同步方法，同步方法又调用一个非同步方法，仅在没有其他线程直接调用非同步方法的情况下，是线程安全的。若有其他线程直接调用非同步方法，则是线程不安全的。」**
:::
<a name="161154a7"></a>
### 场景六：两个线程同时访问同一个对象的不同的同步方法
这个场景也是在探讨对象锁的作用范围，对象锁的作用范围是对象中的所有同步方法。所以，当访问同一个对象中的多个同步方法时，结论是：
:::info
**「两个线程同时访问同一个对象的不同的同步方法时，是线程安全的。」**
:::
```java
public class Condition5 implements Runnable {  
    static Condition5 instance = new Condition5();  

    @Override  
    public void run() {  
        if (Thread.currentThread().getName().equals("Thread-0")) {  
            //线程0,执行同步方法method0()  
            method0();  
        }  
        if (Thread.currentThread().getName().equals("Thread-1")) {  
            //线程1,执行同步方法method1()  
            method1();  
        }  
    }  

    private synchronized void method0() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，同步方法0，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，同步方法0，运行结束");  
    }  

    private synchronized void method1() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，同步方法1，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，同步方法1，运行结束");  
    }  

    //运行结果:串行  
    public static void main(String[] args) {  
        Thread thread1 = new Thread(instance);  
        Thread thread2 = new Thread(instance);  
        thread1.start();  
        thread2.start();  
        while (thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }  
}
```
<a name="cMtth"></a>
#### 运行结果
是线程安全的。
```
线程名：Thread-1，同步方法1，运行开始  
线程：Thread-1，同步方法1，运行结束  
线程名：Thread-0，同步方法0，运行开始  
线程：Thread-0，同步方法0，运行结束  
测试结束
```
<a name="GzzIz"></a>
#### 结果分析
两个方法（method0()和method1()）的synchronized修饰符，虽没有指定锁对象，但默认锁对象为this对象为锁对象，<br />所以对于同一个实例（instance），两个线程拿到的锁是同一把锁，此时同步方法会串行执行。这也是`synchronized`关键字的可重入性的一种体现。
<a name="Knwb8"></a>
### 场景七：两个线程分别同时访问静态synchronized和非静态synchronized方法
这种场景的本质也是在探讨两个线程获取的是不是同一把锁的问题。静态`synchronized`方法属于类锁，锁对象是`（*.class）`对象，非静态`synchronized`方法属于对象锁中的方法锁，锁对象是`this`对象。两个线程拿到的是不同的锁，自然不会相互影响。结论：
:::info
**「两个线程分别同时访问静态synchronized和非静态synchronized方法，线程不安全。」**
:::
<a name="T0Fpd"></a>
#### 代码实现
```java
public class Condition6 implements Runnable {  
    static Condition6 instance = new Condition6();  

    @Override  
    public void run() {  
        if (Thread.currentThread().getName().equals("Thread-0")) {  
            //线程0,执行静态同步方法method0()  
            method0();  
        }  
        if (Thread.currentThread().getName().equals("Thread-1")) {  
            //线程1,执行非静态同步方法method1()  
            method1();  
        }  
    }  

    // 重点：用static synchronized 修饰的方法，属于类锁，锁对象为（*.class）对象。  
    private static synchronized void method0() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，静态同步方法0，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，静态同步方法0，运行结束");  
    }  

    // 重点：synchronized 修饰的方法，属于方法锁，锁对象为（this）对象。  
    private synchronized void method1() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，非静态同步方法1，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，非静态同步方法1，运行结束");  
    }  

    //运行结果:并行  
    public static void main(String[] args) {  
        //问题原因： 线程1的锁是类锁（*.class）对象，线程2的锁是方法锁（this）对象,两个线程的锁不一样，自然不会互相影响，所以会并行执行。  
        Thread thread1 = new Thread(instance);  
        Thread thread2 = new Thread(instance);  
        thread1.start();  
        thread2.start();  
        while (thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }
```
<a name="6QSxy"></a>
#### 运行结果
```
线程名：Thread-0，静态同步方法0，运行开始  
线程名：Thread-1，非静态同步方法1，运行开始  
线程：Thread-1，非静态同步方法1，运行结束  
线程：Thread-0，静态同步方法0，运行结束  
测试结束
```
<a name="d4a20dce"></a>
### 场景八：同步方法抛出异常后，JVM会自动释放锁的情况
本场景探讨的是`synchronized`释放锁的场景：
:::info
**「只有当同步方法执行完或执行时抛出异常这两种情况，才会释放锁。」**
:::
所以，在一个线程的同步方法中出现异常的时候，会释放锁，另一个线程得到锁，继续执行。而不会出现一个线程抛出异常后，另一个线程一直等待获取锁的情况。这是因为JVM在同步方法抛出异常的时候，会自动释放锁对象。
<a name="COSlt"></a>
#### 代码实现
```java
public class Condition7 implements Runnable {  

    private static Condition7 instance = new Condition7();  

    @Override  
    public void run() {  
        if (Thread.currentThread().getName().equals("Thread-0")) {  
            //线程0,执行抛异常方法method0()  
            method0();  
        }  
        if (Thread.currentThread().getName().equals("Thread-1")) {  
            //线程1,执行正常方法method1()  
            method1();  
        }  
    }  

    private synchronized void method0() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        //同步方法中，当抛出异常时，JVM会自动释放锁，不需要手动释放，其他线程即可获取到该锁  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，抛出异常，释放锁");  
        throw new RuntimeException();  

    }  

    private synchronized void method1() {  
        System.out.println("线程名：" + Thread.currentThread().getName() + "，运行开始");  
        try {  
            Thread.sleep(4000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("线程：" + Thread.currentThread().getName() + "，运行结束");  
    }  

    public static void main(String[] args) {  
        Thread thread1 = new Thread(instance);  
        Thread thread2 = new Thread(instance);  
        thread1.start();  
        thread2.start();  
        while (thread1.isAlive() || thread2.isAlive()) {  
        }  
        System.out.println("测试结束");  
    }  

}
```
<a name="DvYNC"></a>
#### 运行结果
```
线程名：Thread-0，运行开始  
线程名：Thread-0，抛出异常，释放锁  
线程名：Thread-1，运行开始  
Exception in thread "Thread-0" java.lang.RuntimeException  
 at com.study.synchronize.conditions.Condition7.method0(Condition7.java:34)  
 at com.study.synchronize.conditions.Condition7.run(Condition7.java:17)  
 at java.lang.Thread.run(Thread.java:748)  
线程：Thread-1，运行结束  
测试结束
```
<a name="Tswal"></a>
#### 结果分析
可以看出线程还是串行执行的，说明是线程安全的。而且出现异常后，不会造成死锁现象，JVM会自动释放出现异常线程的锁对象，其他线程获取锁继续执行。
