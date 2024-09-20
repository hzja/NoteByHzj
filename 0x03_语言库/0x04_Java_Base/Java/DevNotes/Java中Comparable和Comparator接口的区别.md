Java Comparable Comparator
<a name="gzfWw"></a>
## `Comparable` 简介
`Comparable` 是排序接口。<br />若一个类实现了`Comparable`接口，就意味着“该类支持排序”。此外，“实现`Comparable`接口的类的对象”可以用作“有序映射(如`TreeMap`)”中的键或“有序集合(`TreeSet`)”中的元素，而不需要指定比较器。接口中通过`x.compareTo(y)`来比较x和y的大小。若返回负数，意味着x比y小；返回零，意味着x等于y；返回正数，意味着x大于y。
<a name="zuOCg"></a>
## `Comparator` 简介
`Comparator` 是比较器接口。若需要控制某个类的次序，而该类本身不支持排序(即没有实现`Comparable`接口)；那么，可以建立一个“该类的比较器”来进行排序。这个“比较器”只需要实现`Comparator`接口即可。也就是说，可以通过“实现`Comparator`类来新建一个比较器”，然后通过该比较器对类进行排序。<br />`int compare(T o1, T o2)`和上面的`x.compareTo(y)`类似，定义排序规则后返回正数，零和负数分别代表大于，等于和小于。
<a name="Z2q54"></a>
## 两者的联系
`Comparable`相当于“内部比较器”，而`Comparator`相当于“外部比较器”。<br />代码实现
```java
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * @                           _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
 *@DESCRIPTION Comparable是排序接口；若一个类实现了Comparable接口，就意味着“该类支持排序”。
 *             Comparable相当于“内部比较器”
 *@PACKAGE_NAME com.github.compare
 **/
public class ComparableAndCompartor
{
    public static void main(String[] args)
    {
        List<House> houses = new ArrayList();
        House h1 = new House(95.0, 12000);
        House h2 = new House(110.0, 12160);
        House h3 = new House(80.0, 16300);
        House h4 = new House(150.3, 10690);
        houses.add(h1);
        houses.add(h2);
        houses.add(h3);
        houses.add(h4);
        comparable(houses);
        comparator(houses);
    }

    /**
     *@DESCRIPTION House类实现类Comparable接口, 并重写了compareTo方法, 所以执行Collections.sort方法时会去调用我们重写的compareTo方法
     *@CLASS_NAME ComparableAndCompartor
     **/
    private static void comparable(List houses)
    {

        System.out.printf("未排序前的顺序,%s\n", houses);
        Collections.sort(houses);
        System.out.printf("按面积大小排序后的顺序,%s\n", houses);
    }

    private static void comparator(List houses)
    {

        System.out.printf("未排序前的顺序,%s\n", houses);
        Collections.sort(houses, new ComparatorDetail());
        System.out.printf("按单价大小排序后的顺序,%s\n", houses);
    }

    /**
     *@DESCRIPTION 实现Compatator接口, 并重写compare方法, 根据单价倒序排序
     *@CLASS_NAME ComparableAndCompartor
     **/
    static class ComparatorDetail implements Comparator<House>
    {

        @Override
        public int compare(House o1, House o2)
        {
            if (o1.price < o2.price)
                return 1;
            else if (o1.price > o2.price)
                return -1;
            return 0;
        }
    }
}
```
```java
package com.github.compare;

/**
 * @                           _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
 *@DESCRIPTION 一个房子对象, 有面积和单价两个属性
 *@PACKAGE_NAME com.github.compare
 **/
public class House implements Comparable<House>
{
    /*房子的面积*/
    protected double proportion;

    /*房子每平米的售价*/
    protected double price;

    public House(double proportion, double price)
    {
        this.proportion = proportion;
        this.price = price;
    }

    /**
     *@DESCRIPTION 重写compareTo方法, 利用房子的面积来进行大小比较
     *@CLASS_NAME House
     **/
    @Override
    public int compareTo(House o)
    {
        /*当前对象的面积大,返回正数*/
        if (this.proportion > o.proportion)
            return 1;
            /*当前面积小,返回负数*/
        else if (this.proportion < o.proportion)
            return -1;
        /*相等返回0*/
        return 0;
    }

    @Override
    public String toString()
    {
        return "面积为" + proportion + "\t价格为" + price;
    }
}
```
<a name="yL6ZY"></a>
## 附注
<a name="HMlxe"></a>
### `Collection`与`Collections`的区别
`Collection`是集合类的上级接口，继承与他有关的接口主要有`List`和`Set`<br />`Collections`是针对集合类的一个帮助类，他提供一系列静态方法实现对各种集合的搜索、排序、线程安全等操作
```java
public static void main(String args[]) {   
    //注意List是实现Collection接口的   
    List list = new ArrayList();   
    double array[] = { 112, 111, 23, 456, 231 };   
    for (int i = 0; i < array.length; i++) {   
        list.add(new Double(array[i]));   
    }   
    Collections.sort(list);   //把list按从小到大排序
    for (int i = 0; i < array.length; i++) {   
        System.out.println(list.get(i));   
    }   
    // 结果：23.0 111.0 112.0 231.0 456.0   
}   
```
<a name="ZAoR5"></a>
### `Collections`如何调用重写的`compareTo`方法的
集合框架中，`Collections`工具类支持两种排序方法：
```java
Collections.sort(List<T> list); 
Collections.sort(List<T> list, Comparator<? super T> c)
```
如果待排序的列表中是数字或者字符，可以直接使用`Collections.sort(list);`当需要排序的集合或数组不是单纯的数字型时，需要自己定义排序规则，实现一个`Comparator`比较器。<br />`Collections`调用`Collections.sort(list)`方法，方法传递一个List集合，这里要求，`List`泛型里面装的元素必须实现`Compareable`接口此外，列表中的所有元素都必须是可相互比较的（也就是说，对于列表中的任何 e1 和 e2 元素，`e1.compareTo(e2)` 不得抛出 `ClassCastException`）。<br />Java源码里是这样写的
```java
All elements in the list must implement the {@link Comparable}interface.Furthermore, all elements in the list must be <i>mutually comparable</i> (that is, {@code e1.compareTo(e2)} must not throw a {@code ClassCastException} for any elements
```
<a name="prhYr"></a>
#### `Collections.sort`源码
```java
public static <T extends Comparable<? super T>> void sort(List<T> list) {
    Object[] a = list.toArray();
    Arrays.sort(a);
    ListIterator<T> i = list.listIterator();
    for (int j=0; j<a.length; j++) {
        i.next();
        i.set((T)a[j]);
    }
}
```
由源码可以看出来，`sort`内部调用了`Arrays.sort`的方法，继续向下看
<a name="CAXbM"></a>
#### `Arrays.sort`源码
```java
public static void sort(Object[] a) {
    if (LegacyMergeSort.userRequested)
        legacyMergeSort(a);
    else
        ComparableTimSort.sort(a);
}
```
源码里首先判断是否采用传统的排序方法，`LegacyMergeSort.userRequested`属性默认为false，也就是说默认选中 `ComparableTimSort.sort(a)`方法(传统归并排序在1.5及之前是默认排序方法，1.5之后默认执行`ComparableTimSort.sort()`方法。除非程序中强制要求使用传统归并排序，语句如下：
```java
System.setProperty("java.util.Arrays.useLegacyMergeSort", "true"))
```
继续看 `ComparableTimSort.sort(a)`源码
<a name="cYEXu"></a>
#### `ComparableTimSort.sort(a)`源码
```java
static void sort(Object[] a) {
    sort(a, 0, a.length);
}

static void sort(Object[] a, int lo, int hi) {
    rangeCheck(a.length, lo, hi);
    int nRemaining  = hi - lo;
    if (nRemaining < 2)
        return;  // Arrays of size 0 and 1 are always sorted

    // If array is small, do a "mini-TimSort" with no merges
    if (nRemaining < MIN_MERGE) {
        int initRunLen = countRunAndMakeAscending(a, lo, hi);
        binarySort(a, lo, hi, lo + initRunLen);
        return;
    }

    /**
         * March over the array once, left to right, finding natural runs,
         * extending short natural runs to minRun elements, and merging runs
         * to maintain stack invariant.
         */
    ComparableTimSort ts = new ComparableTimSort(a);
    int minRun = minRunLength(nRemaining);
    do {
        // Identify next run
        int runLen = countRunAndMakeAscending(a, lo, hi);

        // If run is short, extend to min(minRun, nRemaining)
        if (runLen < minRun) {
            int force = nRemaining <= minRun ? nRemaining : minRun;
            binarySort(a, lo, lo + force, lo + runLen);
            runLen = force;
        }

        // Push run onto pending-run stack, and maybe merge
        ts.pushRun(lo, runLen);
        ts.mergeCollapse();

        // Advance to find next run
        lo += runLen;
        nRemaining -= runLen;
    } while (nRemaining != 0);

    // Merge all remaining runs to complete sort
    assert lo == hi;
    ts.mergeForceCollapse();
    assert ts.stackSize == 1;
}
```
nRemaining表示没有排序的对象个数，方法执行前，如果这个数小于2，就不需要排序了。<br />如果`2<= nRemaining <=32`，即`MIN_MERGE`的初始值，表示需要排序的数组是小数组，可以使用mini-TimSort方法进行排序，否则需要使用归并排序。<br />mini-TimSort排序方法：先找出数组中从下标为0开始的第一个升序序列，或者找出降序序列后转换为升序重新放入数组，将这段升序数组作为初始数组，将之后的每一个元素通过二分法排序插入到初始数组中。注意，这里就调用到了重写的`compareTo()`方法了。
```java
private static int countRunAndMakeAscending(Object[] a, int lo, int hi) {
    assert lo < hi;
    int runHi = lo + 1;
    if (runHi == hi)
        return 1;

    // Find end of run, and reverse range if descending
    if (((Comparable) a[runHi++]).compareTo(a[lo]) < 0) { // Descending
        while (runHi < hi && ((Comparable) a[runHi]).compareTo(a[runHi - 1]) < 0)
            runHi++;
        reverseRange(a, lo, runHi);
    } else {                              // Ascending
        while (runHi < hi && ((Comparable) a[runHi]).compareTo(a[runHi - 1]) >= 0)
            runHi++;
    }

    return runHi - lo;
}
```
