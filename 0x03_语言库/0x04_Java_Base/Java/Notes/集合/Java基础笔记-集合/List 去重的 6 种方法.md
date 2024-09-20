Java List
<a name="yuHQs"></a>
## 前置知识
正式开始之前，先来搞懂两组概念：无序集合和有序集合 & 无序和有序。因为接下来的方法实现中，会反复提及这两组概念，所以有必要在正式开始之前，先把它们搞清楚。
<a name="SP6hw"></a>
### 无序集合
无序集合是指，数据读取的顺序和数据插入的顺序是不一致的。<br />例如，插入集合的顺序是：1、5、3、7，而集合的读取顺序竟然是：1、3、5、7。
<a name="n27KH"></a>
### 有序集合
有序集合的概念和无序集合的概念正好相反，它是指集合的读取顺序和插入顺序是一致的。<br />例如，插入数据的顺序是：1、5、3、7，那么读取的顺序也是：1、5、3、7。
<a name="aau1Y"></a>
### 有序和无序
通过上面的无序集合和有序集合，可以得出有序和无序的概念。有序指的是数据的排列顺序和读取顺序符合预期就叫做有序。而无序指的是数据的排列顺序和读取顺序不符合预期就叫做无序。<br />PS：如果对于有序和无序的概念不是很清楚也没关系，通过下面的事例，可以进一步的理解它们的含义。
<a name="coS7P"></a>
## 方法1：contains判断去重(有序)
要进行数据去重，首先想到的是新建一个集合，然后循环原来的集合，每次循环判断原集合中的循环项，如果当前循环的数据，没有在新集合中存在就插入，已经存在了就舍弃，这样当循环执行完，就得到了一个没有重复元素的集合了，实现代码如下：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method(list);
    }

    /**
     * 自定义去重
     * @param list
     */
    public static void method(List<Integer> list) {
        // 新集合
        List<Integer> newList = new ArrayList<>(list.size());
        list.forEach(i -> {
            if (!newList.contains(i)) { // 如果新集合中不存在则插入
                newList.add(i);
            }
        });
        System.out.println("去重集合:" + newList);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752081-0bf7e358-5cea-4223-9531-18139a3c64d5.webp#averageHue=%23333436&clientId=ub761e523-e579-4&from=paste&id=u197afb04&originHeight=188&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5fc36fa8-9083-4141-a09e-869ada5f3e0&title=)此方法的优点的：理解起来比较简单，并且最终得到的集合也是有序的，这里的有序指的是新集合的排列顺序和原集合的顺序是一致的；但缺点是实现代码有点多，不够简洁优雅。
<a name="vabK0"></a>
## 方法2：迭代器去重(无序)
自定义 List 去重，除了上面的新建集合之外，也可以使用迭代器循环判断每一项数据，如果当前循环的数据，在集合中存在两份或两份以上，就将当前的元素删除掉，这样循环完之后，也可以得到一个没有重复数据的集合，实现代码如下：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method_1(list);
    }

    /**
     * 使用迭代器去重
     * @param list
     */
    public static void method_1(List<Integer> list) {
        Iterator<Integer> iterator = list.iterator();
        while (iterator.hasNext()) {
            // 获取循环的值
            Integer item = iterator.next();
            // 如果存在两个相同的值
            if (list.indexOf(item) != list.lastIndexOf(item)) {
                // 移除最后那个相同的值
                iterator.remove();
            }
        }
        System.out.println("去重集合:" + list);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752421-815451e0-e24f-4080-bc03-dd9d86743941.webp#averageHue=%23333336&clientId=ub761e523-e579-4&from=paste&id=u4e83fda3&originHeight=191&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u35587619-2e55-4f77-80c4-bdaf8123057&title=)此方法的实现比上一种方法的实现代码要少一些，并且不需要新建集合，但此方法得到的新集合是无序的，也就是新集合的排列顺序和原集合不一致，因此也不是最优的解决方案。
<a name="sPhnq"></a>
## 方法3：HashSet去重(无序)
HashSet 天生具备“去重”的特性，那只需要将 List 集合转换成 HashSet 集合就可以了，实现代码如下：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method_2(list);
    }

    /**
     * 使用 HashSet 去重
     * @param list
     */
    public static void method_2(List<Integer> list) {
        HashSet<Integer> set = new HashSet<>(list);
        System.out.println("去重集合:" + set);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752064-ea65614f-1345-40c3-8afa-97a930c61618.webp#averageHue=%23333436&clientId=ub761e523-e579-4&from=paste&id=u4e54b789&originHeight=190&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4fb4be4b-5f01-489c-bcc9-7225c95d43c&title=)此方法的实现代码较为简洁，但缺点是 HashSet 会自动排序，这样新集合的数据排序就和原集合不一致了，如果对集合的顺序有要求，那么此方法也不能满足当前需求。
<a name="EQfnS"></a>
## 方法4：LinkedHashSet去重(有序)
既然 HashSet 会自动排序不能满足需求，那就使用 LinkedHashSet，它既能去重又能保证集合的顺序，实现代码如下：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method_3(list);
    }

    /**
     * 使用 LinkedHashSet 去重
     * @param list
     */
    public static void method_3(List<Integer> list) {
        LinkedHashSet<Integer> set = new LinkedHashSet<>(list);
        System.out.println("去重集合:" + set);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752277-26cd131c-7187-4e44-9e72-4b5663cf839b.webp#averageHue=%23333336&clientId=ub761e523-e579-4&from=paste&id=u7ef8e502&originHeight=189&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1d5589d9-9b34-4d66-b41f-fb9c4c357ff&title=)从上述代码和执行结果可以看出，LinkedHashSet 是到目前为止，实现比较简单，且最终生成的新集合与原集合顺序保持一致的实现方法，是可以考虑使用的一种去重方法。
<a name="qZpzE"></a>
## 方法5：TreeSet去重(无序)
除了以上的 Set 集合之外，还可以使用 TreeSet 集合来实现去重功能，实现代码如下：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method_4(list);
    }

    /**
     * 使用 TreeSet 去重(无序)
     * @param list
     */
    public static void method_4(List<Integer> list) {
        TreeSet<Integer> set = new TreeSet<>(list);
        System.out.println("去重集合:" + set);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752118-03bb6329-440e-4a14-b0bb-73e613844957.webp#averageHue=%23333335&clientId=ub761e523-e579-4&from=paste&id=uc2bc37ab&originHeight=190&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufc1b50a7-7728-46da-a1f8-51a5df0e77c&title=)比较遗憾的是，TreeSet 虽然实现起来也比较简单，但它有着和 HashSet 一样的问题，会自动排序，因此也不能满足需求。
<a name="Kmy4i"></a>
## 方法6：Stream去重(有序)
JDK 8 带来了一个非常实用的方法 Stream，使用它可以实现很多功能，比如下面的去重功能：
```java
public class ListDistinctExample {
    public static void main(String[] args) {
        List<Integer> list = new ArrayList<Integer>() {{
            add(1);
            add(3);
            add(5);
            add(2);
            add(1);
            add(3);
            add(7);
            add(2);
        }};
        System.out.println("原集合:" + list);
        method_5(list);
    }

    /**
     * 使用 Stream 去重
     * @param list
     */
    public static void method_5(List<Integer> list) {
        list = list.stream().distinct().collect(Collectors.toList());
        System.out.println("去重集合:" + list);
    }
}
```
以上程序执行的结果，如下所示：![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639981752788-ea78c3f4-f627-4c12-ac96-aa88b5b9fbec.webp#averageHue=%23333436&clientId=ub761e523-e579-4&from=paste&id=uc9903e02&originHeight=187&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc2182c4e-a191-4fb6-bfed-eaaabbf5d2c&title=)Stream 实现去重功能和其他方法不同的是，它不用新创建集合，使用自身接收一个去重的结果就可以了，并且实现代码也很简洁，并且去重后的集合顺序也和原集合的顺序保持一致，是最优先考虑的去重方法。
<a name="ciqSC"></a>
## 总结
这里介绍了 6 种集合去重的方法，其中实现最简洁，且去重之后的顺序能和原集合保持一致的实现方法，只有两种：LinkedHashSet 去重和 Stream 去重，而后一种去重方法无需借助新集合，是优先考虑的去重方法。
