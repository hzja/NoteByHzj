Java ReadWriteLock<br />开发中遇到并发的问题一般会用到锁，`Synchronized`存在明显的一个性能问题就是读与读之间互斥；<br />`ReadWriteLock`是JDK5中提供的读写分离锁。读写分离锁可以有效地帮助减少锁竞争，以提升系统的性能。<br />`**ReadWriteLock**`**管理一组锁，一个是只读的锁，一个是写锁。**<br />Java并发库中`ReetrantReadWriteLock`实现了`ReadWriteLock`接口并添加了可重入的特性。<br />而**读写锁**`**ReentrantReadWriteLock**`**：读读共享，读写互斥，写写互斥；** 读写锁维护了一对锁，一个读锁，一个写锁，通过分离读锁和写锁，使得并发性相比一般的排他锁有了很大提升。在读多写少的情况下，读写锁能够提供比排他锁更好的并发性和吞吐量。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634443237843-fb96601f-6dcc-4976-9311-fa08c3263de5.png#clientId=ua732dd6f-3fa8-4&from=paste&height=293&id=ucb8ec13c&originHeight=880&originWidth=2818&originalType=binary&ratio=1&size=247041&status=done&style=shadow&taskId=ufddaa6bf-567e-4062-875a-afb98ad3410&width=939.3333333333334)<br />从源码中可以看出，读写锁中同样依赖队列同步器Sync(AQS)实现同步功能，而读写状态就是其同步器的同步状态。<br />下面从例子中来说明：**读读共享，读写互斥，写写互斥**。<br />代码如下：
```java
public class ReentrantWriteReadLockTest {
    ReentrantReadWriteLock lock = new ReentrantReadWriteLock();
    ReadLock readLock = lock.readLock();
    WriteLock writeLock = lock.writeLock();
    
    public void read(){
        try {
            readLock.lock();
            System.out.println("线程"+Thread.currentThread().getName()+"进入。。。");
            Thread.sleep(3000);
            System.out.println("线程"+Thread.currentThread().getName()+"退出。。。");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally{
            readLock.unlock();
        }
    }
    
    public void write(){
        try {
            writeLock.lock();
            System.out.println("线程"+Thread.currentThread().getName()+"进入。。。");
            Thread.sleep(3000);
            System.out.println("线程"+Thread.currentThread().getName()+"退出。。。");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }finally{
            writeLock.unlock();
        }
    }
    
 
    public static void main(String[] args) {
        final ReentrantWriteReadLockTest wr = new ReentrantWriteReadLockTest();
        Thread t1 = new Thread(new Runnable() {
            public void run() {
                wr.read();
            }
        }, "t1");
        Thread t2 = new Thread(new Runnable() {
            public void run() {
                wr.read();
            }
        }, "t2");
        Thread t3 = new Thread(new Runnable() {
            public void run() {
                wr.write();
            }
        }, "t3");
        Thread t4 = new Thread(new Runnable() {
            public void run() {
                wr.write();
            }
        }, "t4");
        
        t1.start();
        t2.start();
        //t3.start();
        //t4.start();
    }
}
```
当启动线程t1和t2时，结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634449639986-d9b7b4da-f6c9-4df3-a5c1-dcbf40e0035d.png#clientId=u29f47825-d529-4&from=paste&height=241&id=u25c2df2f&originHeight=724&originWidth=1946&originalType=binary&ratio=1&size=111366&status=done&style=shadow&taskId=ub7d652f0-0a4c-49b8-b03c-8dd1d8761d0&width=648.6666666666666)<br />**线程t1和t2可以同时进入，说明了读读共享！**<br />当启动线程t2和t3时，结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634449692504-200c7f4e-16e7-4b13-aed6-abdd127394d8.png#clientId=u29f47825-d529-4&from=paste&height=239&id=ude66052c&originHeight=717&originWidth=2027&originalType=binary&ratio=1&size=113843&status=done&style=shadow&taskId=u83c6be7d-c94f-457a-9968-cdf9bb9da85&width=675.6666666666666)<br />**一个线程必须等待另一个线程退出，才能进入，说明了读写互斥！**<br />当启动线程t3和t4时，结果如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634449729499-870a7c99-3d2b-4392-8f90-1f3b2eeca1e7.png#clientId=u29f47825-d529-4&from=paste&height=252&id=u6ee5d256&originHeight=755&originWidth=2023&originalType=binary&ratio=1&size=114346&status=done&style=shadow&taskId=u6533b277-3799-4bb1-9f8c-69d6693de6f&width=674.3333333333334)<br />**一个线程必须等待另一个线程退出，才能进入，说明了写写互斥！**
