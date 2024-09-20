Java8 Stream<br />接口文档：<br />`filter`是Java8 Stream的方法：
```java
Stream filter(Predicate<? super T> predicate)
```
返回由与此给定谓词匹配的此流的元素组成的流。<br />`removeIf`是Java8 Collecttion的一个默认方法。
```java
default boolean removeIf(Predicate<? super E> filter)
```
删除满足给定谓词的这个集合的所有元素。<br />`removeIf`和`filter`方法都能达到过滤/删除元素的作用。<br />从功能是实现上，`removeIf`是条件为`true`则过滤此元素，`false`则保留。而`filter`则是条件为`false`过滤此元素，而`true`则保留。<br />从时间上，要看场景，因为`removeIf`返回的是`boolean`，而`filter`是个intermediate operation，需要terminal operation最终来处理流，如果只是单单只做删除/过滤操作， `removeIf`会快些。<br />示例：
```java
public static void main(String[] args) {
    List list = new ArrayList(Arrays.asList(1,2,3,4,5));
    long last = System.currentTimeMillis();
    list.removeIf(a -> a.equals(2));
    System.out.println(System.currentTimeMillis() - last);//37~38
}

public static void main(String[] args) {
    List list = new ArrayList(Arrays.asList(1,2,3,4,5));
    long last = System.currentTimeMillis();
    list.stream().filter(a -> !a.equals(2)).collect(Collectors.toList());
    System.out.println(System.currentTimeMillis() - last);//41~44
}
```
来看看源码实现：<br />`removeIf`是`Collection`接口的默认方法（Java8新增），底层实现是通过获得迭代器迭代每一个元素，满足条件的通过`remove()`方法删除，直到迭代完返回`true`，迭代完都没有满足条件的元素则返回`false`。如果是多个中间流参与，那么还是推荐使用`filter`方便。
```java
default boolean removeIf(Predicate<? super E> filter) {
    //判断是否为null
    Objects.requireNonNull(filter);
    boolean removed = false;
    final Iterator<E> each = iterator();
    while (each.hasNext()) {
        //迭代出现运行时异常或者错误由由Predicate被转发给调用者
        if (filter.test(each.next())) {
            //remove底层调用的是System.arraycopy方法，是个C++编写的native方法，操作的是指针，所有比较快
            each.remove();
            removed = true;
        }
    }
    return removed;
}
```
每次`filter`都产生一个新的`StatelessOp`，也就新的流，通过`opWrapSink`不断匹配条件，当为`false`时则标记此流元素可过滤
```java
@Override
public final Stream<P_OUT> filter(Predicate<? super P_OUT> predicate) {
    //判断是否为null
    Objects.requireNonNull(predicate);
    //将无状态的中间操作附加到现有流中来构造新流。StreamOpFlag.NOT_SIZED表示要清除的位值
    return new StatelessOp<P_OUT, P_OUT>(this, StreamShape.REFERENCE,
                                         StreamOpFlag.NOT_SIZED) {
        @Override
        Sink<P_OUT> opWrapSink(int flags, Sink<P_OUT> sink) {
            return new Sink.ChainedReference<P_OUT, P_OUT>(sink) {
                @Override
                public void begin(long size) {
                    downstream.begin(-1);
                }

                @Override
                public void accept(P_OUT u) {
                    if (predicate.test(u))
                        downstream.accept(u);
                }
            };
        }
    };
}
//要清除的位值
static final int NOT_SIZED = SIZED.clear;

SIZED(3, set(Type.SPLITERATOR).set(Type.STREAM).clear(Type.OP)),
```
