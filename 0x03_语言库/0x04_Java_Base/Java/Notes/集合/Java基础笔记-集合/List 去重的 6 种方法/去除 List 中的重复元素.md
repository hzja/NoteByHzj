JavaList
<a name="pWB4x"></a>
## List 去重方案
**假设有以下数据：**
```java
/**
 * 3 个张三，2 个李强
 */
public List<String> initList = Arrays.asList(
    "张三", 
    "李四", 
    "张三", 
    "周一", 
    "刘四", 
    "李强", 
    "李白", 
    "张三", 
    "李强", 
    "王五"
);
```
<a name="kD5Nv"></a>
### 1、for 循环添加去重
```java
/**
 * for 循环添加去重
 */
@Test
public void remove1() {
    List<String> list = new ArrayList(initList);
    List<String> list2 = new ArrayList<>();
    for (String element : list) {
        if (!list2.contains(element)) {
            list2.add(element);
        }
    }
    System.out.println(list2);
}
```
这个是最基本的实现了，创建一个空的 List，添加前判断一下存在不存在，不存在才添加，这样就保证了元素不重复。<br />输出结果：
```
[张三, 李四, 周一, 刘四, 李强, 李白, 王五]
```
<a name="ZPrw5"></a>
### 2、for 双循环去重
```java
/**
 * for 双循环去重
 */
@Test
public void remove2() {
    List<String> list = new ArrayList(initList);
    for (int i = 0; i < list.size() - 1; i++) {
        for (int j = list.size() - 1; j > i; j--) {
            if (list.get(j).equals(list.get(i))) {
                list.remove(j);
            }
        }
    }
    System.out.println(list);
}
```
利用双循环，判断是否有相等的，再进行移除。<br />输出结果：
```
[张三, 李四, 周一, 刘四, 李强, 李白, 王五]
```
<a name="FhehT"></a>
### 3、for 循环重复坐标去重
```java
/**
 * for 循环重复坐标去重
 */
@Test
public void remove3() {
    List<String> list = new ArrayList(initList);
    List<String> list2 = new ArrayList(initList);
    for (String element : list2) {
        if (list.indexOf(element) != list.lastIndexOf(element)) {
            list.remove(list.lastIndexOf(element));
        }
    }
    System.out.println(list);
}
```
这种方式很新颖吧？<br />复制一个 list2，再循环 List2，判断 list 中的元素的首尾出现的坐标位置是否一致，如果一致，则说明没有重复的，否则重复，再删除重复的位置的元素。<br />输出结果：
```
[张三, 李四, 周一, 刘四, 李强, 李白, 王五]
```
<a name="LNSyM"></a>
### 4、Set 去重
```java
/**
 * Set 去重
 */
@Test
public void remove4() {
    List<String> list = new ArrayList(initList);
    List<String> list2 = new ArrayList(new HashSet(list));
    System.out.println(list2);
}
```
这招也太简单了吧，一行代码搞定！<br />Set 是不包含重复元素的，把 List 先装进 HashSet，然后再装回来，这样就保证了元素的不重复。<br />输出结果：
```
[李强, 李四, 张三, 周一, 李白, 王五, 刘四]
```
结果虽然正确，但元素顺序和原始 List 不一致，如果要保证顺序性，可以把 HashSet 换成 `LinkedHashSet`：
```java
/**
 * Set 去重
 */
@Test
public void remove4() {
    List<String> list = new ArrayList(initList);
    List<String> list2 = new ArrayList(new LinkedHashSet(list));
    System.out.println(list2);
}
```
输出结果：
```
[张三, 李四, 周一, 刘四, 李强, 李白, 王五]
```
这下顺序对了吧，`LinkedHashSet` 可以保证元素的顺序性！
<a name="KF3Cg"></a>
### 5、Stream 去重
```java
/**
 * Stream 去重
 */
@Test
public void remove5() {
    List<String> list = new ArrayList(initList);
    list = list.stream().distinct().collect(Collectors.toList());
    System.out.println(list);
}
```
利用 Stream 的 `distinct` 方法去重，这个方法也十分简单，一行代码搞定！<br />输出结果：
```
[张三, 李四, 周一, 刘四, 李强, 李白, 王五]
```
<a name="ahkEv"></a>
## 总结
本文总结了 5 种去除 List 重复元素的方法：

- for 循环添加去重
- for 双循环去重
- for 循环重复坐标去重
- Set 去重
- Stream 去重

最后两种方案最简单，都是一行代码就能搞定的，推荐使用！
