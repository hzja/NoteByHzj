<a name="unTqW"></a>
## 1、引言
HashMap相信所有学Java的都一定不会感到陌生，作为一个非常重用且非常实用的Java提供的容器，它在代码里面随处可见。因此遍历操作也是经常会使用到的。HashMap的遍历方式现如今有非常多种：

1. 使用迭代器（Iterator)。
2. 使用 `keySet()` 获取键的集合，然后通过增强的 for 循环遍历键。
3. 使用 `entrySet()` 获取键值对的集合，然后通过增强的 for 循环遍历键值对。
4. 使用 Java 8+ 的 Lambda 表达式和流。

以上遍历方式的孰优孰劣，在《阿里巴巴开发手册》中写道：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699411325123-5d67a56e-f404-4adf-b24c-e77a48ad0e6b.png#averageHue=%23f4f3f1&clientId=udb15dccd-91f7-4&from=paste&height=187&id=u004af23b&originHeight=467&originWidth=1794&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=178782&status=done&style=none&taskId=ucbcda501-17de-4d39-9554-963533a9ada&title=&width=717.6)<br />这里推荐使用的是`entrySet`进行遍历，在Java8中推荐使用`Map.forEach()`。给出的理由是**遍历次数**上的不同。

1. keySet遍历，需要经过**两次**遍历。
2. entrySet遍历，只需要**一次**遍历。

其中keySet遍历了两次，一次是转为Iterator对象，另一次是从hashMap中取出key所对应的value。<br />其中后面一段话很好理解，但是前面这句话却有点绕，为什么转换成了Iterator遍历了一次？
<a name="zKaSq"></a>
## 2、keySet如何遍历了两次
首先写一段代码，使用keySet遍历Map。
```java
public class Test {
    public static void main(String[] args) {
        Map<String, String> map = new HashMap<>();
        map.put("k1", "v1");
        map.put("k2", "v2");
        map.put("k3", "v3");
        for (String key : map.keySet()) {
            String value = map.get(key);
            System.out.println(key + ":" + value);
        }
    }

}
```
运行结果显而易见的是
```
k1:v1
k2:v2
k3:v3
```
两次遍历，第一次遍历所描述的是转为Iterator对象好像没有从代码中看见，看到的后面所描述的遍历，也就是遍历`map,keySet()`所返回的Set集合中的key，然后去HashMap中拿取value的。<br />Iterator对象呢？如何遍历转换为Iterator对象的呢？<br />首先这种遍历方式大家都应该知道是叫：增强for循环，for-each<br />这是一种Java的语法糖~。<br />可以通过反编译，或者直接通过Idea在class文件中查看对应的Class文件<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412032731-93374a19-72e2-4b68-b576-12e2ff9007bf.png#averageHue=%23fbfbfb&clientId=u063b4a97-a546-4&from=paste&height=338&id=u1b581d7c&originHeight=845&originWidth=1769&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=145220&status=done&style=none&taskId=u77086a9a-1cc6-4ebb-9878-ab7d2360821&title=&width=707.6)
```java
public class Test {
    public Test() {
    }

    public static void main(String[] args) {
        Map<String, String> map = new HashMap();
        map.put("k1", "v1");
        map.put("k2", "v2");
        map.put("k3", "v3");
        Iterator var2 = map.keySet().iterator();

        while(var2.hasNext()) {
            String key = (String)var2.next();
            String value = (String)map.get(key);
            System.out.println(key + ":" + value);
        }

    }
}
```
和编写的是存在差异的，其中可以看到其中通过`map.keySet().iterator()`获取到了我们所需要看见的Iterator对象。<br />那么它又是怎么转换成的呢？为什么需要遍历呢？查看`iterator()`方法
<a name="A9KcF"></a>
### `iterator()`
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412129566-e8257429-bdad-4c4f-ad16-5901f8d0d8d3.png#averageHue=%23f7f7f7&clientId=u063b4a97-a546-4&from=paste&height=171&id=u1724beac&originHeight=427&originWidth=2358&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=75039&status=done&style=none&taskId=u2c5ba841-2f2b-4295-be04-0423b26441b&title=&width=943.2)<br />发现是Set定义的一个接口。返回此集合中元素的迭代器
<a name="O22zU"></a>
### `HashMap.KeySet#iterator()`
查看HashMap中keySet类对该方法的实现。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412332601-c862a23b-104a-4870-8913-b3a7f4b4c559.png#averageHue=%23fdfde2&clientId=u063b4a97-a546-4&from=paste&height=559&id=uf11c1037&originHeight=1397&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=511647&status=done&style=none&taskId=u25f9e766-5433-4000-b259-f5d768686ed&title=&width=1536)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412491203-27e6aebf-c170-4e1f-af61-3c5d6a1802eb.png#averageHue=%23f9f8f7&clientId=u063b4a97-a546-4&from=paste&height=816&id=u13346888&originHeight=2040&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=704348&status=done&style=none&taskId=u9d183011-2269-4fd1-b1f0-2d6c10def43&title=&width=1536)
```java
final class KeySet extends AbstractSet<K> {
    public final int size()                 { return size; }
    public final void clear()               { HashMap.this.clear(); }
    public final Iterator<K> iterator()     { return new KeyIterator(); }
    public final boolean contains(Object o) { return containsKey(o); }
    public final boolean remove(Object key) {
        return removeNode(hash(key), key, null, false, true) != null;
    }
    public final Spliterator<K> spliterator() {
        return new KeySpliterator<>(HashMap.this, 0, -1, 0, 0);
    }
    public final void forEach(Consumer<? super K> action) {
        Node<K,V>[] tab;
        if (action == null)
            throw new NullPointerException();
        if (size > 0 && (tab = table) != null) {
            int mc = modCount;
            for (int i = 0; i < tab.length; ++i) {
                for (Node<K,V> e = tab[i]; e != null; e = e.next)
                action.accept(e.key);
            }
            if (modCount != mc)
                throw new ConcurrentModificationException();
        }
    }
}
```
其中的`iterator()`方法返回的是一个`KeyIterator`对象，那么究竟是在哪里进行了遍历呢？接着往下看去。
<a name="jFlyl"></a>
### `HashMap.KeyIterator`
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412553753-2b3b3f7f-f014-4d1c-b974-b8eea4b12c21.png#averageHue=%23f9f8f7&clientId=u063b4a97-a546-4&from=paste&height=816&id=u9e64c847&originHeight=2040&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=707261&status=done&style=none&taskId=u062befd0-91a9-4480-845d-f6fbee07e4e&title=&width=1536)
```java
final class KeyIterator extends HashIterator
    implements Iterator<K> {
    public final K next() { return nextNode().key; }
}
```
这个类也很简单：

1. 继承了`HashIterator`类。
2. 实现了`Iterator`接口。
3. 一个`next()`方法。

还是没有看见哪里进行了遍历，那么继续查看`HashIterator`类
<a name="kcS6t"></a>
### `HashMap.HashIterator`
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699412713498-cf99e001-31b1-465f-8b7b-b11c451710c0.png#averageHue=%23f9f8f8&clientId=u063b4a97-a546-4&from=paste&height=816&id=u2b107fd2&originHeight=2040&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=688227&status=done&style=none&taskId=ufb4e669b-6c4b-41bd-91a5-ccbcbde0c14&title=&width=1536)
```java
abstract class HashIterator {
    Node<K,V> next;        // next entry to return
    Node<K,V> current;     // current entry
    int expectedModCount;  // for fast-fail
    int index;             // current slot

    HashIterator() {
        expectedModCount = modCount;
        Node<K,V>[] t = table;
        current = next = null;
        index = 0;
        if (t != null && size > 0) { // advance to first entry
            do {} while (index < t.length && (next = t[index++]) == null);
        }
    }

    public final boolean hasNext() {
        return next != null;
    }

    final Node<K,V> nextNode() {
        Node<K,V>[] t;
        Node<K,V> e = next;
        if (modCount != expectedModCount)
            throw new ConcurrentModificationException();
        if (e == null)
            throw new NoSuchElementException();
        if ((next = (current = e).next) == null && (t = table) != null) {
            do {} while (index < t.length && (next = t[index++]) == null);
        }
        return e;
    }

    public final void remove() {
        Node<K,V> p = current;
        if (p == null)
            throw new IllegalStateException();
        if (modCount != expectedModCount)
            throw new ConcurrentModificationException();
        current = null;
        K key = p.key;
        removeNode(hash(key), key, null, false, false);
        expectedModCount = modCount;
    }
}
```
可以发现这个构造器中存在了一个do-while循环操作，目的是找到一个第一个不为空的entry。
```java
HashIterator() {
    expectedModCount = modCount;
    Node<K,V>[] t = table;
    current = next = null;
    index = 0;
    if (t != null && size > 0) { // advance to first entry
        do {} while (index < t.length && (next = t[index++]) == null);
    }
}
```
而`KeyIterator`是`extend HashIterator`对象的。这里涉及到了继承的相关概念。<br />例如两个类
```java
public class Father {

    public Father(){
        System.out.println("father");
    }
}
```
```java
public class Son extends Father{

    public static void main(String[] args) {
        Son son = new Son();
    }
}
```
创建Son对象的同时，会执行Father构造器。也就会打印出father这句话。<br />那么这个循环操作就是我们要找的循环操作了。
<a name="s5DAl"></a>
## 3、总结

1. 使用keySet遍历，其实内部是使用了对应的`iterator()`方法。
2. `iterator()`方法是创建了一个`KeyIterator`对象。
3. `KeyIterator`对象`extend HashIterator`对象。
4. `HashIterator`对象的构造方法中，会遍历找到第一个不为空的`entry`。

`keySet`->`iterator()`->`KeyIterator`->`HashIterator`
