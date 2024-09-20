JavaQueue
<a name="jUmqf"></a>
# Queue 接口
_Queue_ 接口出现在 java.util 包中，它也继承自 _Collection_ 接口，用于保存要按**先进先出** FIFO（First In First Out）顺序处理的元素。它是一个有序的对象列表，其使用仅限于在列表末尾插入元素并从列表开头删除元素，即遵循队列先进先出的基本原则。<br />![Queue 接口的继承实现关系](https://cdn.nlark.com/yuque/0/2022/png/396745/1656635245822-8ecbff4d-92cc-4222-91de-ef995fd9954c.png#clientId=u9772c6a6-85c9-4&from=paste&id=u0d2a619a&originHeight=635&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u66748e39-0c0e-4868-b64d-1bad8adc553&title=Queue%20%E6%8E%A5%E5%8F%A3%E7%9A%84%E7%BB%A7%E6%89%BF%E5%AE%9E%E7%8E%B0%E5%85%B3%E7%B3%BB "Queue 接口的继承实现关系")<br />作为一个接口，_Queue_ 需要一个用于声明的具体类，最常见的类是 java 中的 `_PriorityQueue_` 和 `_LinkedList_`。不过，这两个实现类都不是线程安全的。如果需要线程安全的实现，就可以考虑 _java.util.concurrent_ 包下面的 `_PriorityBlockingQueue_` 类。<br />_Queue_ 接口的声明：
```java
public interface Queue extends Collection  
```
**创建 _Queue_ 对象**，因为 _Queue_ 是一个接口，所以只能使用 _Queue_ 接口的实现类来创建实例，比如：
```java
// Obj 是存储在 Queue 中的对象类型
Queue<Obj> queue = new PriorityQueue<Obj> ();
```
下面以 _LinkedList_ 为例实现一个普通队列，并进行简单的入队和出队操作：
```java
import java.util.LinkedList;
import java.util.Queue;

public class QueueInterfaceDemo {
    public static void main(String[] args) {
        Queue<Integer> q = new LinkedList<>();

        // 向队列中依次添加 {0, 1, 2, 3, 4} 
        for (int i = 0; i < 5; i++) {
            q.add(i);
        }

        // 打印队列中的元素
        System.out.println("Elements of queue " + q);

        // 移除对头的元素 0 
        int removedEle = q.remove();
        System.out.println("removed element-" + removedEle);

        System.out.println(q);

        // 取出对头元素，但不移除
        int head = q.peek();
        System.out.println("head of queue-" + head);

        // 计算当前的队列大小
        int size = q.size();
        System.out.println("Size of queue-" + size);
    }
}
```
<a name="IYWpx"></a>
## Queue 接口的操作
<a name="pctjK"></a>
### 添加元素
可以使用 `add()` 方法向队列中添加元素。此外 `_PriorityQueue_` 中不保留元素的插入顺序。元素默认按照升序的优先级顺序存储。关于优先队列 `_PriorityQueue_` 的具体实现细节之后分享。
```java
public class QueueAddDemo {

    public static void main(String[] args)
    {
        Queue<String> pq = new PriorityQueue<>();

        pq.add("Love");
        pq.add("For");
        pq.add("Passion");

        System.out.println(pq);
    }
}
```
<a name="fsIWP"></a>
### 删除元素
可以使用 `remove(`) 方法删除队列中的元素。如果要删除的对象在队列中存在多个，则删除第一个匹配到的项。除此之外，`poll()`方法可用于删除队头元素并返回它。
```java
public class QueueRemoveDemo {
    public static void main(String[] args) {
        Queue<String> pq = new PriorityQueue<>();

        pq.add("Love");
        pq.add("For");
        pq.add("Love");

        System.out.println("初始队列 " + pq);

        pq.remove("Love");

        System.out.println("删除 Love 之后 " + pq);

        System.out.println("Poll 方法 " + pq.poll());

        System.out.println("最终的队列 " + pq);
    }
}
```
<a name="XW8Yf"></a>
### 遍历队列
有多种方法可以遍历 `_Queue_`。常见的方法是将队列转换为数组并使用 for 循环遍历。但是，队列还有一个内置的迭代器，可以用来迭代队列。
```java
public class QueueIterator {
    public static void main(String[] args) {
        Queue<String> pq = new PriorityQueue<>();

        pq.add("Geeks");
        pq.add("For");
        pq.add("Geeks");

        Iterator iterator = pq.iterator();

        while (iterator.hasNext()) {
            System.out.print(iterator.next() + " ");
        }
    }
}
```
<a name="UvqIA"></a>
## 队列的常用实现类
<a name="yPmyS"></a>
### `PriorityQueue`
在集合框架中实现的 `_PriorityQueue_` 类是一种基于优先级的对象处理方法。众所周知，队列是遵循 First-In-First-Out 的算法，但有时需要根据优先级处理队列中的元素，这时就可以选择使用 `_PriorityQueue_`。简单的使用示例：
```java
class PriorityQueueTest {
    public static void main(String args[]) {
        // 创建优先级队列
        Queue<Integer> pQueue = new PriorityQueue<Integer>();

        // 队列中添加元素
        pQueue.add(10);
        pQueue.add(20);
        pQueue.add(15);

        // 打印堆顶元素
        System.out.println(pQueue.peek());

        // 打印并删除堆顶元素
        System.out.println(pQueue.poll());

        // 再次打印堆顶元素
        System.out.println(pQueue.peek());
    }
}
```
<a name="ATyP8"></a>
### LinkedList
LinkedList 是集合框架中实现的链表数据结构，同时由于实现了 _Queue_ 接口，也可以用于表示普通队列。链表是一种线性数据结构，其中元素存储在非连续的地址空间，每个元素是一个单独的对象，具有数据部分和地址部分。元素使用指针和地址进行链接。
```java
class LinkedListQueue {
    public static void main(String args[]) {
        // 创建一个空的链表
        Queue<Integer> ll = new LinkedList<Integer>();

        // 添加元素
        ll.add(10);
        ll.add(20);
        ll.add(15);

        // 取队头元素
        System.out.println(ll.peek());

        // 打印并删除队头元素
        System.out.println(ll.poll());

        // 打印队头元素
        System.out.println(ll.peek());
    }
}
```
<a name="YtrkG"></a>
### `PriorityBlockingQueue`
_PriorityQueue_ 和 _LinkedList_ 都是非线程安全的类，如果需要线程安全的实现，可以考虑 _PriorityBlockingQueue_ ，它使用与类 _PriorityQueue_ 相同的排序规则并提供阻塞检索操作的无界阻塞队列（[**unbounded blocking queue**](https://docs.oracle.com/javase/8/docs/api/index.html)）。虽然这个队列在逻辑上是无界的，但是在添加元素时，可能会由于资源耗尽而导致 OutOfMemoryError 。此类不允许空元素。依赖于自然排序的优先级队列也不允许插入不可比对象（否则会导致 ClassCastException）。
```java
class PriorityBlockingQueueDemo {
    public static void main(String[] args) {
        // 创建一个优先级阻塞队列
        Queue<Integer> pbq = new PriorityBlockingQueue<Integer>();
        
        // 向队列中添加元素
        pbq.add(10);
        pbq.add(20);
        pbq.add(15);
        
        // 取出堆顶元素
        System.out.println(pbq.peek());
        
        // 删除 PriorityBlockingQueue 的堆顶元素
        System.out.println(pbq.poll());
        
        // 打印堆顶元素
        System.out.println(pbq.peek());
    }
}
```
<a name="rSibA"></a>
## 总结
本文主要介绍了 _Queue_ 队列的基本概念及其主要的实现子类，现将 _Queue_ 的特征总结如下几点：

1. **Queue 用于在队列的末尾插入元素并从队列的头部删除。它遵循 FIFO 先进先出的基本原则。**
2. **Queue 接口支持 Collection 接口的所有方法，包括插入和删除等操作。**
3. **LinkedList、 **`**ArrayBlockingQueue**`** 和 **`**PriorityQueue**`** 是 Queue 接口最常用的实现类。**
4. **在 java.util 包中定义的队列是无界队列。在 java.util.concurrent 包中的队列是有界队列。**
<a name="SLlWw"></a>
## 参考资料
优先级阻塞队列: [_https://docs.oracle.com/javase/8/docs/api/index.html_](https://docs.oracle.com/javase/8/docs/api/index.html)
