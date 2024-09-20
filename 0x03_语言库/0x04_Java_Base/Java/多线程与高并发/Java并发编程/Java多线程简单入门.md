Java 多线程
<a name="FQ7tt"></a>
## 多线程创建的两种方式：
<a name="Nq31h"></a>
### ♠①：创建一个类继承Thread类，并重写run方法。
```java
public class MyThread extends Thread {
    @Override
    public void run() {
        for (int i = 0; i < 100; i++) {
            System.out.println(getName() + ":打了" + i + "个小兵");
        }
    }
}
```
来写个测试方法验证下：
```java
//创建MyThread对象
MyThread t1=new  MyThread();
MyThread t2=new  MyThread();
MyThread t3=new  MyThread();
//设置线程的名字
t1.setName("鲁班");
t2.setName("刘备");
t3.setName("亚瑟");
//启动线程
t1.start();
t2.start();
t3.start();
```
来看一下执行后的结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639462140148-4cb7a67a-9cbd-4632-96a2-0a8b49d36d9d.webp#clientId=u024b9fe7-7843-4&from=paste&id=a8fcG&originHeight=556&originWidth=832&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4cb01770-3f98-41f8-9dbf-bdba35c7918&title=)
<a name="k6WWo"></a>
### ♠②：创建一个类实现Runnable接口，并重写run方法。
```java
public class MyRunnable implements Runnable {
    @Override
    public void run() {
        for (int i = 0; i < 10; i++) {
            try {//sleep会发生异常要显示处理
                Thread.sleep(20);//暂停20毫秒
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName() + "打了:" + i + "个小兵");
        }
    }
}
```
写个测试方法验证下：
```java
//创建MyRunnable类
MyRunnable mr = new MyRunnable();
//创建Thread类的有参构造,并设置线程名
Thread t1 = new Thread(mr, "张飞");
Thread t2 = new Thread(mr, "貂蝉");
Thread t3 = new Thread(mr, "吕布");
//启动线程
t1.start();
t2.start();
t3.start();
```
来看一下执行后的结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639462140207-5e31deaa-c851-411b-b1e5-1bfee195056a.webp#clientId=u024b9fe7-7843-4&from=paste&id=u7cc434b7&originHeight=532&originWidth=768&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub6dbd80f-eecf-43ca-b8d5-af97d88cc98&title=)
<a name="XPy7t"></a>
## ❤1、为什么要重写`run`方法？
因为run方法是用来封装被线程执行的代码。
<a name="U429Y"></a>
## ❤2、`run()`方法和`start()`方法有什么区别？

- `run()`：封装线程执行的代码，直接调用相当于调用普通方法。
- `start()`：启动线程，然后由JVM 调用此线程的 `run()` 方法。
<a name="qhItS"></a>
## ❤3、通过继承 Thread 的方法和实现 Runnable 接口的方式创建多线程，哪个好？
实现Runable接口好，原因有两个：

- ♠①、避免了Java单继承的局限性
- ♠②、适合多个相同的程序代码去处理同一资源的情况，把线程、代码和数据有效的分离，更符合面向对象的设计思想。

针对线程控制，还会遇到 3 个常见的方法，来一一介绍下。<br />1）`sleep()`：使当前正在执行的线程暂停指定的毫秒数，也就是进入休眠的状态。<br />需要注意的是，sleep 的时候要对异常进行处理。
```java
try {//sleep会发生异常要显示处理
    Thread.sleep(20);//暂停20毫秒
} catch (InterruptedException e) {
    e.printStackTrace();
}
```
2）`join()`：等待这个线程执行完才会轮到后续线程得到cpu的执行权，使用这个也要抛出异常。
```java
//创建MyRunnable类
MyRunnable mr = new MyRunnable();
//创建Thread类的有参构造,并设置线程名
Thread t1 = new Thread(mr, "张飞");
Thread t2 = new Thread(mr, "貂蝉");
Thread t3 = new Thread(mr, "吕布");
//启动线程
t1.start();
try {
    t1.join(); //等待t1执行完才会轮到t2，t3抢
} catch (InterruptedException e) {
    e.printStackTrace();
}
t2.start();
t3.start();
```
来看一下执行后的结果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639462140107-21a8c203-b09e-4912-a6b9-104451ee9359.webp#clientId=u024b9fe7-7843-4&from=paste&id=ub8526467&originHeight=618&originWidth=652&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud8d90ce2-e6dc-488b-9408-6d63a0c45a2&title=)<br />3）`setDaemon()`：将此线程标记为守护线程，准确来说，就是服务其他的线程，像 Java 中的垃圾回收线程，就是典型的守护线程。
```java
//创建MyRunnable类
MyRunnable mr = new MyRunnable();
//创建Thread类的有参构造,并设置线程名
Thread t1 = new Thread(mr, "张飞");
Thread t2 = new Thread(mr, "貂蝉");
Thread t3 = new Thread(mr, "吕布");

t1.setDaemon(true);
t2.setDaemon(true);

//启动线程
t1.start();
t2.start();
t3.start();
```
如果其他线程都执行完毕，main 方法（主线程）也执行完毕，JVM 就会退出，也就是停止运行。如果 JVM 都停止运行了，守护线程自然也就停止了。<br />最后再来看一下线程的生命周期。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639462140176-eb550fb2-6d2d-4960-891a-7c941d762d01.webp#clientId=u024b9fe7-7843-4&from=paste&id=u8228912d&originHeight=555&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3f92080f-db4f-46bd-a732-9696a0018a4&title=)
