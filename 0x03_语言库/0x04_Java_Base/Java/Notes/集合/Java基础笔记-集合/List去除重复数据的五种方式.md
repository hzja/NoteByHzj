Java List 
<a name="j7C2t"></a>
### 1、使用`LinkedHashSet`删除`arraylist`中的重复数据
`LinkedHashSet`是在一个ArrayList删除重复数据的最佳方法。`LinkedHashSet`在内部完成两件事：

- 删除重复数据
- 保持添加到其中的数据的顺序

Java示例使用`LinkedHashSet`删除`arraylist`中的重复项。在给定的示例中，`numbersList`是包含整数的`arraylist`，其中一些是重复的数字。<br />例如1,3和5.将列表添加到`LinkedHashSet`，然后将内容返回到列表中。结果`arraylist`没有重复的整数。
```java
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashSet;

public class ArrayListExample{
    public static void main(String[] args)
    {
        ArrayList<Integer> numbersList = new ArrayList<>(Arrays.asList(1, 1, 2, 3, 3, 3, 4, 5, 6, 6, 6, 7, 8));
        System.out.println(numbersList);
        LinkedHashSet<Integer> hashSet = new LinkedHashSet<>(numbersList);
        ArrayList<Integer> listWithoutDuplicates = new ArrayList<>(hashSet);
        System.out.println(listWithoutDuplicates);
    }
}
```
输出结果
```
[1, 1, 2, 3, 3, 3, 4, 5, 6, 6, 6, 7, 8]
 
[1, 2, 3, 4, 5, 6, 7, 8]
```
<a name="LfZYA"></a>
### 2、使用Java8新特性`stream`进行List去重
要从`arraylist`中删除重复项，也可以使用java 8 stream api。使用`steam`的`distinct()`方法返回一个由不同数据组成的流，通过对象的`equals()`方法进行比较。<br />收集所有区域数据List使用`Collectors.toList()`。<br />Java程序，用于在不使用Set的情况下从java中的`arraylist`中删除重复项。
```java
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

public class ArrayListExample{
    public static void main(String[] args){
        ArrayList<Integer> numbersList = new ArrayList<>(Arrays.asList(1, 1, 2, 3, 3, 3, 4, 5, 6, 6, 6, 7, 8));
        System.out.println(numbersList);
        List<Integer> listWithoutDuplicates = numbersList.stream().distinct().collect(Collectors.toList());
        System.out.println(listWithoutDuplicates);
    }
}
```
输出结果
```
[1, 1, 2, 3, 3, 3, 4, 5, 6, 6, 6, 7, 8]
 
[1, 2, 3, 4, 5, 6, 7, 8]
```
<a name="RSdot"></a>
### 3、利用`HashSet`不能添加重复数据的特性
由于`HashSet`不能保证添加顺序，所以只能作为判断条件保证顺序：
```java
private static void removeDuplicate(List<String> list) {
    HashSet<String> set = new HashSet<String>(list.size());
    List<String> result = new ArrayList<String>(list.size());
    for (String str : list) {
        if (set.add(str)) {
            result.add(str);
        }
    }
    list.clear();
    list.addAll(result);
}
```
<a name="Xfzph"></a>
### 4、利用`List`的`contains`方法循环遍历
重新排序，只添加一次数据,避免重复：
```java
private static void removeDuplicate(List<String> list) {
    List<String> result = new ArrayList<String>(list.size());
    for (String str : list) {
        if (!result.contains(str)) {
            result.add(str);
        }
    }
    list.clear();
    list.addAll(result);
}
```
<a name="hKI1L"></a>
### 5、双重`for`循环去重（不推荐在List元素都一样的情况下使用）
```java
for (int i = 0; i < list.size(); i++) { 
    for (int j = 0; j < list.size(); j++) { 
        if(i!=j&&list.get(i).equals(list.get(j))) { 
            list.remove(list.get(j)); 
        } 
    }
}
```
