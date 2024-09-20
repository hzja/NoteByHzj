Java Synchronized
<a name="yB95L"></a>
## 1、前言
`synchronized`锁是jvm内置的锁，不同于`ReentrantLock`锁。`synchronized`关键字可以修饰方法，也可以修饰代码块。`synchronized`关键字修饰方法时可以修饰静态方法，也可以修饰非静态方法；同样，`synchronized`关键字修饰代码块时可以修饰对象，也可以修饰类。当然，`synchronized`修饰静态方法/类和非静态方法/对象时的作用范围是不同的。下面通过各种demo来详解`synchronized`的各种用法及注意事项。
<a name="jaAOy"></a>
## 2、synchronized类锁
这里所说的`synchronized`类锁的作用范围是类级别的，不会因为同一个类的不同对象执行而失效。
<a name="ZiyLp"></a>
### 2.1 `synchronized`修饰同一个类的两个静态方法时互斥
```java
public class SynchronizeAndClassLock {
    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            // new了一个ClassLock对象
            new ClassLock().test1();
        }).start();

        new Thread(() -> {
            // new了另一个ClassLock对象
            new ClassLock().test2();
        }).start();
    }

}
class ClassLock {
    public synchronized static void test1(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }
 // 【注意】public static void test2(){ 不会互斥，因为此时test2没有使用类锁。
    public synchronized static void test2(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635939958731-42416e08-0a03-4880-b8e7-fbd9c1ff8e52.png#clientId=u477d03c9-7aa8-4&from=paste&height=243&id=ub1400a5e&originHeight=728&originWidth=2002&originalType=binary&ratio=1&size=144412&status=done&style=shadow&taskId=u5dfee492-7192-4eb0-b095-6b97e7bf7f1&width=667.3333333333334)<br />【结论】两个线程分别同时执行同一个类产生的不同对象的两个不同 `synchronized static`方法，类锁生效，虽然是不同对象，因为两个线程使用的是同一个类锁。反过来，假如`test2`方法没有`synchronized`修饰的话，只有test1方法有被`synchronized`修饰，此时两个方法也不会互斥，一个有锁，一个没有锁，自然不会互斥。
<a name="r9Iqc"></a>
### 2.2 `synchronized`分别修饰同一个类的静态方法和当前类时互斥
```java
public class SynchronizeAndClassLock2 {
    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            // new了一个ClassLock2对象
            new ClassLock2().test1();
            // ClassLock2.test1();
        }).start();

        new Thread(() -> {
            // new了另一个ClassLock2对象
            new ClassLock2().test2();
            // ClassLock2.test2();
        }).start();
    }

}
class ClassLock2 {
    public synchronized static void test1(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }

    public static void test2(){
     // 【注意】synchronized (SynchronizeAndClassLock2.class)不会互斥
        synchronized (ClassLock2.class) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940052566-199a9e0a-ed28-41cb-9b02-ae976134ef41.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=239&id=u68dbbe0a&originHeight=718&originWidth=2259&originalType=binary&ratio=1&size=148458&status=done&style=shadow&taskId=ud68fe8b3-c766-4f9d-81b8-d95574bf240&width=753)<br />【结论】两个线程同时分别执行一个被`synchronized`修饰`static`方法，一个有`synchnized`(该类)代码块的`static`方法，锁生效，虽然是不同对象，因为两个线程使用的同一个类锁。反过来，如果是修饰的不同类，因为类锁不同，肯定不会互斥，比如将`test2`方法的`synchronized (ClassLock2.class)`这句代码改成`synchronized (SynchronizeAndClassLock2.class)`，此时不会互斥。
<a name="idtI6"></a>
### 2.3 `synchronized`分别修饰同一个静态对象时互斥
```java
public class SynchronizeAndClassLock10 {

    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            new RunObject1().test1();
        }).start();

        new Thread(() -> {
            new RunObject2().test2();
        }).start();
    }
}

class RunObject1 {
    public static void test1(){
     // 【1】synchronized (StaticLock2.staticLock1) {
        synchronized (StaticLock2.staticLock) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }
}


class RunObject2 {
    public static void test2() {
     // 【2】synchronized (StaticLock2.staticLock2) {
        synchronized (StaticLock2.staticLock) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }
}

class StaticLock2 {
    public static Object staticLock = new Object();
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940249208-19a2a4b2-a600-4bc2-ba91-43567b3c92e3.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=231&id=u8a95aecb&originHeight=692&originWidth=2101&originalType=binary&ratio=1&size=147244&status=done&style=shadow&taskId=u0300502f-a95c-4471-93f8-68130b0b0a1&width=700.3333333333334)<br />【结论】`synchronized`分别修饰同一个类的静态对象时互斥，反过来，如果是修饰不同的静态对象，肯定不会互斥，比如将上面代码中标【1】和【2】的`synchronized`代码结合使用。
<a name="MlJFd"></a>
## 3、`synchronized`对象锁
这里说的`synchronized`对象锁的作用范围是对象级别的即仅仅作用于同一个对象，如果是同一个类的两个不同的对象是不会互斥的，即没有效果的。
<a name="R0KWI"></a>
### 3.1 `synchronized`修饰同一个类对象的两个非静态方法时互斥
```java
public class SynchronizeAndObjectLock2 {
    public static void main(String[] args) throws Exception {
        // 【注意】当且仅当是同一个SynchronizeAndObjectLock2对象
        SynchronizeAndObjectLock2 synchronizeAndObjectLock2 = new SynchronizeAndObjectLock2();
        new Thread(() -> {
            synchronizeAndObjectLock2.test1();
        }).start();

        new Thread(() -> {
            synchronizeAndObjectLock2.test2();
        }).start();
    }
    public synchronized void test1(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }

    public synchronized void test2(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940389008-fb64bd2b-e550-4155-ab96-0105911aba46.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=237&id=uaa32c31e&originHeight=710&originWidth=2063&originalType=binary&ratio=1&size=147255&status=done&style=shadow&taskId=u43540b63-cbf0-402d-a5a7-eab4ea13346&width=687.6666666666666)<br />【结论】两个线程同时执行被`synchronized`修饰的相同对象的不同（相同）方法，锁生效，因为两个线程使用的是相同的对象锁
<a name="OcFzC"></a>
### 3.2 `synchronized`分别修饰同一个类对象的非静态方法和当前对象时互斥
```java
public class SynchronizeAndObjectLock3 {
    public static void main(String[] args) throws Exception {
        // 【注意】当且仅当是同一个SynchronizeAndObjectLock3对象
        SynchronizeAndObjectLock3 synchronizeAndObjectLock3 = new SynchronizeAndObjectLock3();
        new Thread(() -> {
            synchronizeAndObjectLock3.test1();
        }).start();

        new Thread(() -> {
            synchronizeAndObjectLock3.test2();
        }).start();
    }
    public void test1(){
        synchronized(this) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }

    }

    public synchronized void test2(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940472658-ac85395c-925a-491e-9590-7505b2145fdc.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=229&id=u1332b51c&originHeight=686&originWidth=2078&originalType=binary&ratio=1&size=146493&status=done&style=shadow&taskId=u2afd38ac-912f-41d3-9388-ca76e66f45d&width=692.6666666666666)<br />【结论】`snchronized`修饰非静态方法与`synchronized(this)`互斥，可见，`snchronized`修饰非静态方法实质锁的是当前对象。
<a name="PJgC3"></a>
### 3.3 `synchronized`修饰不同对象的两个非静态方法时不会互斥
```java
public class SynchronizeAndObjectLock {
    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            // 这里new 了一个SynchronizeAndObjectLock对象
            new SynchronizeAndObjectLock().test1();
        }).start();

        new Thread(() -> {
            // 这里new 了另一个SynchronizeAndObjectLock对象
            new SynchronizeAndObjectLock().test2();
        }).start();
    }
    public synchronized void test1(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }

    public synchronized void test2(){
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (Exception e) {}
        System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940670120-f6bbcf3b-2036-464c-b919-fdbc6a16fb5e.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=240&id=u60d1cf75&originHeight=721&originWidth=2092&originalType=binary&ratio=1&size=149189&status=done&style=shadow&taskId=ua71f5af7-eb2a-468d-8e0b-ac4f41e5c35&width=697.3333333333334)<br />【结论】两个线程同时执行被`synchronized`修饰的不同对象的不同（相同）方法，锁未生效，因为两个线程使用的是不同的对象锁。
<a name="NaG7k"></a>
### 3.4 `synchronized`代码块修饰同一个对象时互斥
```java
public class SynchronizeAndObjectLock5 {
    private Object objectLock = new Object();

    public static void main(String[] args) throws Exception {
        
        SynchronizeAndObjectLock5 synchronizeAndObjectLock5 = new SynchronizeAndObjectLock5();
        new Thread(() -> {
            synchronizeAndObjectLock5.test1();
        }).start();

        new Thread(() -> {
            synchronizeAndObjectLock5.test2();
        }).start();
    }
    public void test1(){
        synchronized(objectLock) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }

    }

    public void test2(){
        synchronized(objectLock) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940799036-0190dfd4-cfd6-4f1e-9cc3-88f92122579b.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=219&id=uc751d9a3&originHeight=657&originWidth=2085&originalType=binary&ratio=1&size=143944&status=done&style=shadow&taskId=udec773b7-d3c2-4579-b387-457f1ab41e3&width=695)<br />【结论】`synchronized`代码块修饰同一个对象时互斥，若`synchronized`代码块修饰的是不同对象，那么不会互斥。
<a name="irphP"></a>
## 4、`synchronized`修饰当前类和当前对象时不会互斥
```java
public class ClassAndObjectLock {
    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            ClassAndObjectLock.test1();
        }).start();

        new Thread(() -> {
            new ClassAndObjectLock().test2();
        }).start();
    }
    public static void test1(){
        synchronized (ClassAndObjectLock.class) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }

    public void test2(){
        synchronized (this) {
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " begin...");
            try {
                TimeUnit.SECONDS.sleep(1);
            } catch (Exception e) {}
            System.out.println(new Date() + " " + Thread.currentThread().getName() + " end...");
        }
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635940884806-2a4bb8ca-5075-47de-99e8-426b9b805355.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=237&id=ub1a859be&originHeight=712&originWidth=2006&originalType=binary&ratio=1&size=137890&status=done&style=shadow&taskId=u2066ec7f-2ea1-4ac4-9430-5b6cbe2a1fd&width=668.6666666666666)<br />【结论】可见，类锁和对象锁是相互独立的，互不相斥。
<a name="MRgX9"></a>
## 5、`synchronized`锁注意事项
<a name="QT7wA"></a>
### 5.1 `synchronized`锁不能被中断
为了模拟`synchronized`锁不可中断，下面先让两个线程进入死锁，然后再用main线程去中断其中一个线程，看被中断的线程能否释放锁并被唤醒。
```java
public class DeadLockCannotInterruptDemo {
    private static Object lock1 = new Object();
    private static Object lock2 = new Object();

    public static void main(String[] args) throws Exception {
        Thread threadA = new Thread(new Runnable() {
            @Override
            public void run() {
                synchronized (lock1) {
                    System.out.println(Thread.currentThread().getName() + " get lock1");
                    try {
                        Thread.sleep(10);
                        synchronized (lock2) {
                            System.out.println(Thread.currentThread().getName() + " get lock2");
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        Thread threadB = new Thread(new Runnable() {
            @Override
            public void run() {
                synchronized (lock2) {
                    System.out.println(Thread.currentThread().getName() + " get lock2");
                    try {
                        Thread.sleep(10);
                        synchronized (lock1) {
                            System.out.println(Thread.currentThread().getName() + " get lock1");
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });

        threadA.start();
        threadB.start();

        TimeUnit.SECONDS.sleep(3);
        System.out.println("main thread begin to interrupt " + threadA.getName() + " and " + threadA.getName() + " will release lock1...");
        threadA.interrupt();
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635941004509-95c507a7-494d-4f8e-978a-6d1b7af1633a.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=230&id=u9dfb3e05&originHeight=689&originWidth=2734&originalType=binary&ratio=1&size=109073&status=done&style=shadow&taskId=ud34a05fe-d3c1-405b-86f6-fd4543c645b&width=911.3333333333334)<br />【结论】如上图，`main`线程中断Thread-0后，Thread-0并不会释放锁并醒过来。同样的，`ReentrantLock`的`tryLock`或`lockInterruptibly`是可以被中断的。
<a name="aHeTQ"></a>
### 5.2 `synchronized`锁可重入
<a name="pzxXN"></a>
#### 5.2.1 不同方法，`synchronized`是可重入的
```java
public class SynchronizeAndReentrant {
    public static void main(String[] args) throws Exception {
        SynchronizeAndReentrant synchronizeAndReentrant = new SynchronizeAndReentrant();
        synchronizeAndReentrant.test1();
    }
    public synchronized void test1(){
        System.out.println(" test1 method is called...");
        test2();
    }

    public synchronized void test2(){
        System.out.println(" test2 method is called...");
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635941061791-ba75d93b-433c-4a58-9e8b-d43ba49041b5.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=232&id=u60d21a56&originHeight=695&originWidth=2105&originalType=binary&ratio=1&size=93023&status=done&style=shadow&taskId=ud0ec6f59-ee01-4c4e-ba25-acca17c7212&width=701.6666666666666)
<a name="mIzX1"></a>
#### 5.2.2 相同方法，`synchronized`是可重入的
```java
public class SynchronizeAndReentrant2 {
    int i = 1;
    public static void main(String[] args) throws Exception {
        SynchronizeAndReentrant2 synchronizeAndReentrant = new SynchronizeAndReentrant2();
        synchronizeAndReentrant.test1();
    }
    public synchronized void test1(){

        System.out.println(" test1 method is called " + i++ + "st time..." );
        while(i < 5) {
            test1();
        }
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635941117466-ea698e06-2403-4901-84a4-366335dcd4a1.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=237&id=u7078cf77&originHeight=712&originWidth=2123&originalType=binary&ratio=1&size=113772&status=done&style=shadow&taskId=u21a20aa9-b6d8-4be6-88b4-fbc833a51c7&width=707.6666666666666)
<a name="wooIk"></a>
### 5.3 `synchronized`锁不带超时功能
`synchronized`锁不带超时功能，而`ReentrantLock`的`tryLock`是具备带超时功能的，在指定时间没获取到锁，该线程会苏醒，有助于预防死锁的产生。
<a name="a1tWx"></a>
### 5.4 唤醒/等待需要`synchronized`锁
```java
public class NotifyNeedSynchronized {
    public static Object lock = new Object();
    public static void main(String[] args) throws Exception{
        // 抛出IllegalMonitorStateException
        //lock.notify();
        lock.wait();
    }
}
```
运行结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635941174035-a0ae6e07-997a-4ac4-b57d-b3b3e66bafd9.png#clientId=u24f7b2f3-f3f0-4&from=paste&height=232&id=u3f28d471&originHeight=695&originWidth=3777&originalType=binary&ratio=1&size=200405&status=done&style=shadow&taskId=u9ae1e875-3961-4a6d-8f8a-5699e4911ea&width=1259)<br />【结论】使用`Object`的`notify`和`wait`等方法时，必须要使用`synchronized`锁，否则会抛出`IllegalMonitorStateException`。
<a name="Ptn8T"></a>
### 5.5 使用`synchronized`锁时尽量缩小范围以保证性能
使用`synchronized`锁时，为了尽可能提高性能，应该尽量缩小锁的范围。能不锁方法就不锁方法，推荐尽量使用`synchronized`代码块来降低锁的范围。以下面的一段`netty`源码为例：
```java
// ServerBootstrap.java

public <T> ServerBootstrap childOption(ChannelOption<T> childOption, T value) {
    if (childOption == null) {
        throw new NullPointerException("childOption");
    }
    if (value == null) {
        synchronized (childOptions) {
            childOptions.remove(childOption);
        }
    } else {
        synchronized (childOptions) {
            childOptions.put(childOption, value);
        }
    }
    return this;
}
```
可见，找到并发访问代码的临界区，并不用`synchronized`锁全部代码，尽量避免使用`synchronized`来修饰方法。
