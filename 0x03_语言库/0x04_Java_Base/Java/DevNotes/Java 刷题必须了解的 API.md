Java
<a name="gkUjt"></a>
## 1、排序
<a name="sOTeN"></a>
### 1.1 数组排序（``java.util.Arrays``）
<a name="asdqG"></a>
#### 1.1.1 基本数据类型排序

- 对整个数组排序
```java
public static void sort(int[] a);
```

- 对部分数组 `[fromIndex, toIndex)` 排序
```java
public static void sort(int[] a, int fromIndex, int toIndex);
```
七种基本类型 int、long、short、char、byte、float、double（除了 boolean），都支持上述格式的排序 API。
<a name="S2Pmg"></a>
#### 1.1.2 对象排序

- 实现了 `java.lang.Comparable` 接口的对象。
```java
// 对整个数组排序
public static void sort(Object[] a);
// 对部分数组 [fromIndex, toIndex) 排序
public static void sort(Object[] a, int fromIndex, int toIndex);
```

- 通过 `java.util.Comparator` 排序：
```java
// 对整个数组排序
public static <T> void sort(T[] a, Comparator<? super T> c);
// 对部分数组 [fromIndex, toIndex) 排序
public static <T> void sort(T[] a, int fromIndex, int toIndex,
                                Comparator<? super T> c);

public interface Comparator<T> {
    // result < 0：o1 排在 o2 前面
    // result == 0：o1 和 o2 的值一样
    // result > 0：o1 排在 o2 后面
    int compare(T o1, T o2);
}
```
案例：
```java
public class Solution {
    public static void main(String[] args) {
        Person[] persons = new Person[2];
        persons[0] = new Person(2);
        persons[1] = new Person(1);

        Arrays.sort(persons, new Comparator<Person>() {
            @Override
            public int compare(Person o1, Person o2) {
                return o1.id - o2.id;
            }
        });

        // 输出：
        // Solution.Person(id=1)
        // Solution.Person(id=2)
        Arrays.stream(persons).forEach(System.out::println);
    }

    @AllArgsConstructor
    @ToString
    public static class Person {
        private int id;
    }
}
```
或者使用 lambda 表达式替换 `Comparator` 匿名类。
```java
Arrays.sort(persons, (o1, o2) -> {
    return o1.id - o2.id;
});
```
<a name="Ozbc1"></a>
### 1.2 列表排序（``java.util.Collections``）
<a name="K8IEV"></a>
#### 排序
```java
public static <T extends Comparable<? super T>> void sort(List<T> list);

public static <T> void sort(List<T> list, Comparator<? super T> c);

public interface Comparator<T> {
    // result < 0：o1 排在 o2 前面
    // result == 0：o1 和 o2 的值一样
    // result > 0：o1 排在 o2 后面
    int compare(T o1, T o2);
}
```
<a name="UmUWF"></a>
#### 反转列表元素
```java
public static void reverse(List<?> list);
```
<a name="VQjEH"></a>
### 1.3 二维数组排序（``java.util.Arrays``）
提示：Java 数组也是一种对象
```java
// api
public static <T> void sort(T[] a, Comparator<? super T> c);

// 案例
Arrays.sort(nums, (int[]a, int[]b) -> a[0] - b[0]);
```
<a name="k4t3a"></a>
## 2、二分查找
<a name="HufUH"></a>
### 数组（`java.util.Arrays`）
```java
public static int binarySearch(int[] a, int key);
public static int binarySearch(int[] a, int fromIndex, int toIndex, int key);

public static int binarySearch(Object[] a, Object key);
public static int binarySearch(Object[] a, int fromIndex, int toIndex, Object key);

public static <T> int binarySearch(T[] a, T key, Comparator<? super T> c);
public static <T> int binarySearch(T[] a, int fromIndex, int toIndex, T key, Comparator<? super T> c);
```
<a name="dJl8O"></a>
### 列表（`java.util.Collections`）
```java
public static <T> int binarySearch(List<? extends Comparable<? super T>> list, T key);

public static <T> int binarySearch(List<? extends T> list, T key, Comparator<? super T> c);
```
<a name="jnXFf"></a>
## 3、栈（``java.util.Stack``）
<a name="AXW03"></a>
### 创建
```java
Stack<Integer> stack = new Stack<>();
```
<a name="oanlC"></a>
### 数据操作
```java
// 往【栈】里面添加一个元素
public E push(E item)

// 往【栈】里面弹出一个元素
public synchronized E pop();
```
<a name="BHfwc"></a>
### 条件判断
```java
public synchronized boolean isEmpty();
```
<a name="lVJsU"></a>
## 4、队列（``java.util.Queue``）
<a name="OoETj"></a>
### 创建（`java.util.LinkedList`）
```java
Queue<Integer> queue = new LinkedList<>();
```
<a name="UEnea"></a>
### 数据操作
```java
// 往【队列】里面添加一个元素
boolean add(E e);

// 往【队列】里面弹出一个元素
E poll();
```
<a name="OHSwp"></a>
### 条件判断
```java
boolean isEmpty();
```
<a name="Jpf34"></a>
## 5、堆（``java.util.PriorityQueue``）
提示：Java 里面的优先队列
<a name="O8ndu"></a>
### 创建
```java
// 创建一个最小堆
PriorityQueue<Integer> minHeap = new PriorityQueue<>();

// 创建一个最大堆
PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Comparator.reverseOrder());
```
<a name="RwJ4G"></a>
### 数据操作
```java
// 往【堆】里面添加一个元素
public boolean add(E e);

// 从【堆】里面弹出一个元素
public E poll();
```
<a name="d0VMc"></a>
## 其他工具
<a name="LKlkN"></a>
### 降序排序（`java.util.Comparator`）
```java
// 反转一个 Comparator 的排序规则
// 比如从【升序】反转为【降序】
default Comparator<T> reversed();

// 反转一个 Comparable 的排序规则
// 比如从【升序】反转为【降序】
public static <T extends Comparable<? super T>> Comparator<T> reverseOrder();
```
<a name="oOq3h"></a>
### 大数（`java.math.BigInteger`）
```java
// 创建一个大数
public static BigInteger valueOf(long val);

// 数据操作
public BigInteger add(BigInteger val);
public BigInteger subtract(BigInteger val);
public BigInteger multiply(BigInteger val);
public BigInteger divide(BigInteger val);
```
<a name="ZNv6x"></a>
### 集合（`java.util.Collections`）
```java
// 初始化一个具有 n 个相同元素 o 的 list
public static <T> List<T> nCopies(int n, T o);

// 反转一个 list 的顺序
public static void reverse(List<?> list);
```
