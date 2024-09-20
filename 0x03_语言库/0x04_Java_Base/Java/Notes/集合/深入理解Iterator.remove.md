<a name="orPZI"></a>
## 问题复现
```java
public void test1()  {
    List<Integer> arrayList = new ArrayList<>();
    for (int i = 0; i < 20; i++) {
        arrayList.add(Integer.valueOf(i));
    }

    // 复现方法一
    Iterator<Integer> iterator = arrayList.iterator();
    while (iterator.hasNext()) {
        Integer integer = iterator.next();
        if (integer.intValue() == 5) {
            arrayList.remove(integer);
        }
    }

    // 复现方法二
    iterator = arrayList.iterator();
    for (Integer value : arrayList) {
        Integer integer = iterator.next();
        if (integer.intValue() == 5) {
            arrayList.remove(integer);
        }
    }
}
```
异常如下：
```
Exception in thread "main" java.util.ConcurrentModificationException
    at java.util.ArrayList$Itr.checkForComodification(ArrayList.java:909)
    at java.util.ArrayList$Itr.next(ArrayList.java:859)
    at com.github.freeacs.base.RedisUtil.main(RedisUtil.java:54)
```
<a name="Mf6pn"></a>
### 问题分析
抛出异常的是`iterator.next`，先看看它的源码。
```java
public E next() {
    checkForComodification();
    int i = cursor;
    if (i >= size)
        throw new NoSuchElementException();
    Object[] elementData = ArrayList.this.elementData;
    if (i >= elementData.length)
        throw new ConcurrentModificationException();
    cursor = i + 1;
    return (E) elementData[lastRet = i];
}
final void checkForComodification() {
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```
在`next`方法中首先调用了`checkForComodification`方法，该方法会判断`modCount`是否等于`expectedModCount`，不等于就会抛出java.util.ConcurrentModificationExcepiton异常。<br />接下来跟踪看一下`modCount`和`expectedModCount`的赋值和修改。<br />`modCount`是ArrayList的一个属性，继承自抽象类AbstractList，用于表示ArrayList对象被修改次数。
```java
protected transient int modCount = 0;
```
整个ArrayList中修改`modCount`的方法比较多，有add、remove、clear、`ensureCapacityInternal`等，凡是设计到ArrayList对象修改的都会自增`modCount`属性。<br />在创建Iterator的时候会将`modCount`赋值给`expectedModCount`，在遍历ArrayList过程中，没有其他地方可以设置`expectedModCount`了，因此遍历过程中`expectedModCount`会一直保持初始值20（调用add方法添加了20个元素，修改了20次）。
```java
int expectedModCount = modCount; // 创建对象Itr时初始化
```
遍历的时候是不会触发`modCount`自增的，但是遍历到`integer.intValue() == 5`的时候，执行了一次`arrayList.remove(integer)`，这行代码执行后`modCount++`变为了21，但此时的`expectedModCount`仍然为20。
```java
final void checkForComodification() {
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```
在执行`next`方法时，遇到`modCount != expectedModCount`方法，导致抛出异常java.util.ConcurrentModificationException。<br />明白了抛出异常的过程，但是为什么要这么做呢？很明显这么做是为了阻止程序员在不允许修改的时候修改对象，起到保护作用，避免出现未知异常。<br />引用网上的一段解释，点击查看解释来源<br />Iterator 是工作在一个独立的线程中，并且拥有一个 mutex 锁。<br />Iterator 被创建之后会建立一个指向原来对象的单链索引表，当原来的对象数量发生变化时，这个索引表的内容不会同步改变。<br />当索引指针往后移动的时候就找不到要迭代的对象，所以按照 fail-fast 原则 Iterator 会马上抛出 java.util.ConcurrentModificationException 异常。<br />所以 Iterator 在工作的时候是不允许被迭代的对象被改变的。但可以使用 Iterator 本身的方法 `remove()` 来删除对象， `Iterator.remove()` 方法会在删除当前迭代对象的同时维护索引的一致性。<br />再来分析下第二种for循环抛异常的原因：
```java
public void forEach(Consumer<? super E> action) {
    Objects.requireNonNull(action);
    final int expectedModCount = modCount;
    @SuppressWarnings("unchecked")
    final E[] elementData = (E[]) this.elementData;
    final int size = this.size;
    for (int i=0; modCount == expectedModCount && i < size; i++) {
        action.accept(elementData[i]);
    }
    if (modCount != expectedModCount) {
        throw new ConcurrentModificationException();
    }
}
```
在for循环中一开始也是对`expectedModCount`采用`modCount`进行赋值。在进行for循环时每次都会有判定条件`modCount == expectedModCount`，当执行完`arrayList.remove(integer)`之后，该判定条件返回false退出循环，然后执行if语句，结果同样抛出java.util.ConcurrentModificationException异常。<br />这两种复现方法实际上都是同一个原因导致的。
<a name="pQ5Uk"></a>
### 问题解决-单线程
上述的两种复现方法都是在单线程运行的，先来说明单线程中的解决方案：
```java
public void test2() {
    List<Integer> arrayList = new ArrayList<>();
    for (int i = 0; i < 20; i++) {
        arrayList.add(Integer.valueOf(i));
    }

    Iterator<Integer> iterator = arrayList.iterator();
    while (iterator.hasNext()) {
        Integer integer = iterator.next();
        if (integer.intValue() == 5) {
            iterator.remove();
        }
    }
}
```
这种解决方案最核心的就是调用`iterator.remove()`方法。看看该方法源码为什么这个方法能避免抛出异常
```java
public void remove() {
    if (lastRet < 0)
        throw new IllegalStateException();
    checkForComodification();

    try {
        ArrayList.this.remove(lastRet);
        cursor = lastRet;
        lastRet = -1;
        expectedModCount = modCount;
    } catch (IndexOutOfBoundsException ex) {
        throw new ConcurrentModificationException();
    }
}
```
在`iterator.remove()`方法中，同样调用了ArrayList自身的`remove`方法，但是调用完之后并非就return了，而是`expectedModCount = modCount`重置了`expectedModCount`值，使二者的值继续保持相等。<br />针对forEach循环并没有修复方案，因此在遍历过程中同时需要修改ArrayList对象，则需要采用iterator遍历。<br />上面提出的解决方案调用的是`iterator.remove()`方法，如果不仅仅是想调用`remove`方法移除元素，还想增加元素，或者替换元素，是否可以呢？浏览Iterator源码可以发现这是不行的，Iterator只提供了`remove`方法。<br />但是ArrayList实现了`ListIterator`接口，`ListIterator`类继承了Iter，这些操作都是可以实现的，使用示例如下：
```java
public void test3() {
    ArrayList<Integer> arrayList = new ArrayList<>();
    for (int i = 0; i < 20; i++) {
        arrayList.add(Integer.valueOf(i));
    }

    ListIterator<Integer> iterator = arrayList.listIterator();
    while (iterator.hasNext()) {
        Integer integer = iterator.next();
        if (integer.intValue() == 5) {
            iterator.set(Integer.valueOf(6));
            iterator.remove();
            iterator.add(integer);
        }
    }
}
```
<a name="w4Xhw"></a>
## 多线程情况下分析及解决
单线程问题解决了，再来看看多线程情况。
<a name="Qvzjt"></a>
### 问题复现
```java
public void test4() {
    ArrayList<Integer> arrayList = new ArrayList<>();
    for (int i = 0; i < 20; i++) {
        arrayList.add(Integer.valueOf(i));
    }

    Thread thread1 = new Thread(new Runnable() {
        @Override
        public void run() {
            ListIterator<Integer> iterator = arrayList.listIterator();
            while (iterator.hasNext()) {
                System.out.println("thread1 " + iterator.next().intValue());
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });

    Thread thread2 = new Thread(new Runnable() {
        @Override
        public void run() {
            ListIterator<Integer> iterator = arrayList.listIterator();
            while (iterator.hasNext()) {
                System.out.println("thread2 " + iterator.next().intValue());
                iterator.remove();
            }
        }
    });
    thread1.start();
    thread2.start();
}
```
在个测试代码中，开启两个线程，一个线程遍历，另外一个线程遍历加修改。程序输出结果如下
```
thread1 0
thread2 0
thread2 1
thread2 2
thread2 3
thread2 4
thread2 5
thread2 6
thread2 7
thread2 8
thread2 9
thread2 10
thread2 11
thread2 12
thread2 13
thread2 14
thread2 15
thread2 16
thread2 17
thread2 18
thread2 19
Exception in thread "Thread-0" java.util.ConcurrentModificationException
    at java.util.ArrayList$Itr.checkForComodification(ArrayList.java:901)
    at java.util.ArrayList$Itr.next(ArrayList.java:851)
    at com.snow.ExceptionTest$1.run(ExceptionTest.java:74)
    at java.lang.Thread.run(Thread.java:745)

Process finished with exit code 0
```
<a name="wrapI"></a>
### 问题分析
从上面代码执行结果可以看出thread2 遍历结束后，thread1 sleep完1000ms准备遍历第二个元素，next的时候抛出异常了。从时间点分析一下抛异常的原因。<br />两个thread都是使用的同一个arrayList，thread2修改完后`modCount = 21`，此时thread2的`expectedModCount = 21` 可以一直遍历到结束；thread1的`expectedModCount`仍然为20，因为thread1的`expectedModCount`只是在初始化的时候赋值，其后并未被修改过。因此当arrayList的`modCount`被thread2修改为21之后，thread1想继续遍历必定会抛出异常了。<br />在这个示例代码里面，两个thread，每个thread都有自己的iterator，当thread2通过iterator方法修改`expectedModCount`必定不会被thread1感知到。这个跟ArrayList非线程安全是无关的，即使这里面的ArrayList换成Vector也是一样的结果，不信上测试代码：
```java
public void test5() {
    Vector<Integer> vector = new Vector<>();
    for (int i = 0; i < 20; i++) {
        vector.add(Integer.valueOf(i));
    }

    Thread thread1 = new Thread(new Runnable() {
        @Override
        public void run() {
            ListIterator<Integer> iterator = vector.listIterator();
            while (iterator.hasNext()) {
                System.out.println("thread1 " + iterator.next().intValue());
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });

    Thread thread2 = new Thread(new Runnable() {
        @Override
        public void run() {
            ListIterator<Integer> iterator = vector.listIterator();
            while (iterator.hasNext()) {
                Integer integer = iterator.next();
                System.out.println("thread2 " + integer.intValue());
                if (integer.intValue() == 5) {
                    iterator.remove();
                }
            }
        }
    });
    thread1.start();
    thread2.start();
}
```
执行结果为：
```
thread1 0
thread2 0
thread2 1
thread2 2
thread2 3
thread2 4
thread2 5
thread2 6
thread2 7
thread2 8
thread2 9
thread2 10
thread2 11
thread2 12
thread2 13
thread2 14
thread2 15
thread2 16
thread2 17
thread2 18
thread2 19
Exception in thread "Thread-0" java.util.ConcurrentModificationException
    at java.util.Vector$Itr.checkForComodification(Vector.java:1184)
    at java.util.Vector$Itr.next(Vector.java:1137)
    at com.snow.ExceptionTest$3.run(ExceptionTest.java:112)
    at java.lang.Thread.run(Thread.java:745)

Process finished with exit code 0
```
test5()方法执行结果和test4()是相同的，那如何解决这个问题呢？
<a name="wCfKj"></a>
### 问题解决-多线程
<a name="uhl8n"></a>
#### 方案一：iterator遍历过程加锁
```java
public static void test5() {
    ArrayList<Integer> arrayList = new ArrayList<>();
    for (int i = 0; i < 20; i++) {
        arrayList.add(Integer.valueOf(i));
    }

    Thread thread1 = new Thread(new Runnable() {
        @Override
        public void run() {
            synchronized (arrayList) {
                ListIterator<Integer> iterator = arrayList.listIterator();
                while (iterator.hasNext()) {
                    System.out.println("thread1 " + iterator.next().intValue());
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    });

    Thread thread2 = new Thread(new Runnable() {
        @Override
        public void run() {
            synchronized (arrayList) {
                ListIterator<Integer> iterator = arrayList.listIterator();
                while (iterator.hasNext()) {
                    Integer integer = iterator.next();
                    System.out.println("thread2 " + integer.intValue());
                    if (integer.intValue() == 5) {
                        iterator.remove();
                    }
                }
            }
        }
    });
    thread1.start();
    thread2.start();
}
```
这种方案本质上是将多线程通过加锁来转变为单线程操作，确保同一时间内只有一个线程去使用iterator遍历arrayList，其它线程等待，效率显然是只有单线程的效率。
<a name="bpH98"></a>
#### 方案二：使用`CopyOnWriteArrayList`
先来看代码，很有意思咯
```java
public void test6() {
    List<Integer> list = new CopyOnWriteArrayList<>();
    for (int i = 0; i < 20; i++) {
        list.add(Integer.valueOf(i));
    }

    Thread thread1 = new Thread(new Runnable() {
        @Override
        public void run() {
            ListIterator<Integer> iterator = list.listIterator();
            while (iterator.hasNext()) {
                System.out.println("thread1 " + iterator.next().intValue());
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });

    Thread thread2 = new Thread(new Runnable() {
        @Override
        public void run() {
            for (Integer integer : list) {
                System.out.println("thread2 " + integer.intValue());
                if (integer.intValue() == 5) {
                    list.remove(integer);
                }
            }
            for (Integer integer : list) {
                System.out.println("thread2 again " + integer.intValue());
            }
            //                ListIterator<Integer> iterator = list.listIterator();
            //                while (iterator.hasNext()) {
            //                    Integer integer = iterator.next();
            //                    System.out.println("thread2 " + integer.intValue());
            //                    if (integer.intValue() == 5) {
            //                        iterator.remove();
            //                    }
            //                }
        }
    });
    thread1.start();
    thread2.start();
}
```
先不分析，看执行结果，这个执行结果重点关注字体加粗部分。<br />thread1 0<br />thread2 0<br />thread2 1<br />thread2 2<br />thread2 3<br />**thread2 4**<br />**thread2 5**<br />**thread2 6**<br />thread2 7<br />thread2 8<br />thread2 9<br />thread2 10<br />thread2 11<br />thread2 12<br />thread2 13<br />thread2 14<br />thread2 15<br />thread2 16<br />thread2 17<br />thread2 18<br />thread2 19<br />thread2 again 0<br />thread2 again 1<br />thread2 again 2<br />thread2 again 3<br />**thread2 again 4**<br />**thread2 again 6**<br />thread2 again 7<br />thread2 again 8<br />thread2 again 9<br />thread2 again 10<br />thread2 again 11<br />thread2 again 12<br />thread2 again 13<br />thread2 again 14<br />thread2 again 15<br />thread2 again 16<br />thread2 again 17<br />thread2 again 18<br />thread2 again 19<br />thread1 1<br />thread1 2<br />thread1 3<br />**thread1 4**<br />**thread1 5**<br />**thread1 6**<br />thread1 7<br />thread1 8<br />thread1 9<br />thread1 10<br />thread1 11<br />thread1 12<br />thread1 13<br />thread1 14<br />thread1 15<br />thread1 16<br />thread1 17<br />thread1 18<br />thread1 19<br />先分析thread2的输出结果，第一次遍历将4 5 6都输出，情理之中；第一次遍历后删除掉了一个元素，第二次遍历输出4 6，符合预期。<br />再来看下thread1的输出结果，有意思的事情来了，thread1 仍然输出了4 5 6，什么鬼？thread1和thread2都是遍历list，list在thread1遍历第二个元素的时候就已经删除了一个元素了，为啥还能输出5？<br />为了了解这个问题，需要了解`CopyOnWriteArrayList`是如何做到一边遍历的同时还能一边修改并且还不抛异常的。<br />在这里不想再深入分析`CopyOnWriteArrayList`代码，后续会专门出一篇博客来解释这个类的源码的。<br />这里说一下`CopyOnWriteArrayList`的解决思路，其实很简单：
```java
private transient volatile Object[] array;
```
`CopyOnWriteArrayList`本质上是对array数组的一个封装，一旦`CopyOnWriteArrayList`对象发生任何的修改都会new一个新的Object[]数组`newElement`，在`newElement`数组上执行修改操作，修改完成后将`newElement`赋值给array数组（array=newElement）。<br />因为array是volatile的，因此它的修改对所有线程都可见。<br />了解了`CopyOnWriteArrayList`的实现思路之后，再来分析上面代码test6为什么会出现那样的输出结果。先来看下thread1和thread2中用到的两种遍历方式的源码：<br />forEach的方式如下：
```java
public void forEach(Consumer<? super E> action) {
    if (action == null) throw new NullPointerException();
    // 在遍历开始前获取当前数组
    Object[] elements = getArray();
    int len = elements.length;
    for (int i = 0; i < len; ++i) {
        @SuppressWarnings("unchecked") E e = (E) elements[i];
        action.accept(e);
    }
}
```
listIterator的方式如下：
```java
public ListIterator<E> listIterator() {
    return new COWIterator<E>(getArray(), 0);
}
static final class COWIterator<E> implements ListIterator<E> {
    /** Snapshot of the array */
    private final Object[] snapshot;
    /** Index of element to be returned by subsequent call to next.  */
    private int cursor;

    private COWIterator(Object[] elements, int initialCursor) {
        cursor = initialCursor;
        // 初始化为当前数组
        snapshot = elements;
    }

    public void remove() {
        // 已经不支持Iterator remove操作了！！
        throw new UnsupportedOperationException();
    }

    public boolean hasNext() {
        return cursor < snapshot.length;
    }

    @SuppressWarnings("unchecked")
    public E next() {
        if (! hasNext())
            throw new NoSuchElementException();
        return (E) snapshot[cursor++];
    }

    // 此处省略其他无关代码
}
```
这两种遍历方式有个共同的特点：都在初始化的时候将当前数组保存下来了，之后的遍历都将会遍历这个数组，而不管array如何变化。<br />有了这个时间节点表就很清楚了，thread1和thread2 start的时候都会将A数组初始化给自己的临时变量，之后遍历的也都是这个A数组，而不管`CopyOnWriteArrayList`中的array发生了什么变化。因此也就解释了thread1在thread2 remove掉一个元素之后为什么还会输出5了。在thread2中，第二次遍历初始化数组变成了当前的array，也就是修改后的B，因此不会有`Integer.valueOf(5)`这个元素了。<br />从test6执行结果来看，`CopyOnWriteArrayList`确实能解决一边遍历一边修改并且还不会抛异常，但是这也是有代价的：

1. thread2对array数组的修改thread1并不能被动感知到，只能通过`hashCode()`方法去主动感知，否则就会一直使用修改前的数据
2. 每次修改都需要重新new一个数组，并且将array数组数据拷贝到new出来的数组中，效率会大幅下降

此外`CopyOnWriteArrayList`中的`ListIterator`实现是不支持remove、add和set操作的，一旦调用就会抛出UnsupportedOperationException异常，因此test6注释代码34-41行中如果运行是会抛异常的。
