Java<br />下面的这断代码大家应该再熟悉不过了，线程休眠需要捕获或者抛出线程中断异常，也就是在睡觉的时候突然有个人冲进来把你吵醒了。
```java
try {
    Thread.sleep(3000);
} catch (InterruptedException e) {
    e.printStackTrace();
}
```
此时线程被打断后，代码会继续运行或者抛出异常结束运行，这并不是需要的中断线程的作用。
<a name="zVClU"></a>
## 到底是什么是线程中断？
线程中断即线程运行过程中被其他线程给打断了，它与 stop 最大的区别是：stop 是由系统强制终止线程，而线程中断则是给目标线程发送一个中断信号，如果目标线程没有接收线程中断的信号并结束线程，线程则不会终止，具体是否退出或者执行其他逻辑由目标线程决定。<br />来看下线程中断最重要的 3 个方法，它们都是来自 Thread 类！<br />1、`java.lang.Thread#interrupt`<br />中断目标线程，给目标线程发一个中断信号，线程被打上中断标记。<br />2、`java.lang.Thread#isInterrupted()`<br />判断目标线程是否被中断，不会清除中断标记。<br />3、`java.lang.Thread#interrupted`<br />判断目标线程是否被中断，会清除中断标记。
<a name="quPRW"></a>
## 线程中断实战
来实例演示下线程中断如何用！
<a name="ZCfro"></a>
### 示例1（中断失败）
```java
private static void test1() {
    Thread thread = new Thread(() -> {
        while (true) {
            Thread.yield();
        }
    });
    thread.start();
    thread.interrupt();
}
```
请问示例1中的线程会被中断吗？答案：不会，因为虽然给线程发出了中断信号，但程序中并没有响应中断信号的逻辑，所以程序不会有任何反应。
<a name="x2cpo"></a>
### 示例2（中断成功）
```java
private static void test2() {
    Thread thread = new Thread(() -> {
        while (true) {
            Thread.yield();

            // 响应中断
            if (Thread.currentThread().isInterrupted()) {
                System.out.println("线程被中断，程序退出。");
                return;
            }
        }
    });
    thread.start();
    thread.interrupt();
}
```
给示例2加上了响应中断的逻辑，程序接收到中断信号打印出信息后返回退出。
<a name="q3w1w"></a>
### 示例3（中断失败）
```java
private static void test3() throws InterruptedException {
    Thread thread = new Thread(() -> {
        while (true) {
            // 响应中断
            if (Thread.currentThread().isInterrupted()) {
                System.out.println("线程被中断，程序退出。");
                return;
            }

            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                System.out.println("线程休眠被中断，程序退出。");
            }
        }
    });
    thread.start();
    Thread.sleep(2000);
    thread.interrupt();
}
```
示例3 `sleep()` 方法被中断，并输出了 `线程休眠被中断，程序退出。` 程序继续运行……为什么呢？<br />来看 sleep 的源码：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654848363148-c5240ab5-7f8b-498f-9d48-8f6b357f2d26.png#clientId=uf15c1b1b-7b2f-4&from=paste&height=206&id=u87605fa1&originHeight=514&originWidth=1660&originalType=binary&ratio=1&rotation=0&showTitle=false&size=87313&status=done&style=shadow&taskId=ueae6fefe-4cc7-47c2-8619-4b511268568&title=&width=664)<br />可以看出 `sleep()` 方法被中断后会清除中断标记，所以循环会继续运行。。
<a name="foMqW"></a>
### 示例4（中断成功）
```java
private static void test4() throws InterruptedException {
    Thread thread = new Thread(() -> {
        while (true) {
            // 响应中断
            if (Thread.currentThread().isInterrupted()) {
                System.out.println("线程被中断，程序退出。");
                return;
            }

            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                System.out.println("线程休眠被中断，程序退出。");
                Thread.currentThread().interrupt();
            }
        }
    });
    thread.start();
    Thread.sleep(2000);
    thread.interrupt();
}
```
示例4全部信息输出并正常退出，只是在 `sleep()` 方法被中断并清除标记后手动重新中断当前线程，然后程序接收中断信号返回退出。

