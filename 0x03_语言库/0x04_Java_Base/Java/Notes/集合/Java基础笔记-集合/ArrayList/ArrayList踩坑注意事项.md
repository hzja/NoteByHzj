<a name="Lyrex"></a>
## 1、使用`Arrays.asList`的注意事项
<a name="fOxsS"></a>
### 1.1 可能会踩的坑
先来看下`Arrays.asList`的使用：
```java
List<Integer> statusList = Arrays.asList(1, 2);
System.out.println(statusList);
System.out.println(statusList.contains(1));
System.out.println(statusList.contains(3));
```
输出结果如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702347505047-02a50016-75c2-437a-ab05-11c8a50b2230.png#averageHue=%23fbfbfb&clientId=uec3f69cf-22be-4&from=paste&height=158&id=u390dacef&originHeight=395&originWidth=1789&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=51003&status=done&style=shadow&taskId=u5ff69488-ad90-4408-b79f-e9b66c6c77c&title=&width=715.6)<br />然后，往`statusList`中添加元素3，如下所示：
```java
statusList.add(3);
System.out.println(statusList.contains(3));
```
预期的结果，应该是输出true，但是实际却是抛出了java.lang.UnsupportedOperationException异常：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702347563834-cf51161a-441a-47de-b156-fc6d6c726f9b.png#averageHue=%23f9f7f7&clientId=uec3f69cf-22be-4&from=paste&height=168&id=u7ca51ee8&originHeight=420&originWidth=2360&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=109500&status=done&style=shadow&taskId=ua4b2fcbf-f272-4468-86c7-9fdd3d320a9&title=&width=944)<br />不禁疑问，只是简单添加个元素，为啥会抛这么个异常呢，不科学啊。
<a name="NNWhj"></a>
### 1.2 原因分析
带着这个疑问，看下`Arrays`类提供的静态方法`asList`的源码：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702347791986-2a5ef9a9-fee9-4a39-8955-463fe8b4a499.png#averageHue=%23faf9f9&clientId=uec3f69cf-22be-4&from=paste&height=685&id=u7047b34d&originHeight=1712&originWidth=2042&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=314854&status=done&style=shadow&taskId=ud34902a2-c6f3-4e1f-859a-deda15c99ae&title=&width=816.8)<br />返回的是`ArrayList`，很熟悉，有木有，但是再细心一看，就会发现此`ArrayList`并不是经常使用的`ArrayList`，因为平时经常使用的`ArrayList`是位于java.util包下的：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702347871694-9d061629-f035-44ce-b718-dd006c336129.png#averageHue=%23f9f8f8&clientId=uec3f69cf-22be-4&from=paste&height=175&id=ufe0926bc&originHeight=437&originWidth=2601&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=100641&status=done&style=shadow&taskId=u9e178dc2-6dfc-478b-bcd7-820d31b1cd5&title=&width=1040.4)<br />但是此处的`ArrayList`却是`Arrays`类的内部类：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702347912359-43625ca8-d539-4799-bb13-944ee0bee669.png#averageHue=%23fcfbfb&clientId=uec3f69cf-22be-4&from=paste&height=508&id=ub92da53b&originHeight=1270&originWidth=2506&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=291692&status=done&style=shadow&taskId=ua057c1ff-a0d2-421b-a7a7-b2664f90687&title=&width=1002.4)<br />它也继承了`AbstractList`类，重写了很多方法，比如上面使用的`contains`方法，但是却没有重写`add`方法，所以在调用`add`方法时才会抛出java.lang.UnsupportedOperationException异常。<br />关于这一点，在《阿里巴巴Java开发手册》泰山版中，也有提及：<br />使用工具类 `Arrays.asList()`把数组转换成集合时，不能使用其修改集合相关的方法，它的 add/remove/clear 方法会抛出 UnsupportedOperationException 异常。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702349316585-fef2cf9d-930e-46ab-bb6b-516989fc0e3f.png#averageHue=%23f5f4f3&clientId=u93813566-aac9-4&from=paste&height=235&id=u4232e445&originHeight=587&originWidth=1830&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=219695&status=done&style=shadow&taskId=uedd68f02-0261-40a6-b161-68f86fd3edb&title=&width=732)<br />所以大家在使用`Arrays.asList`时还是要注意下，避免踩坑。
<a name="LqkQc"></a>
### 1.3 总结
`Arrays.asList`方法可以在一些简单的场合使用，比如快速声明一个集合，判断某个值是否在允许的范围内。<br />但声明后不要再调用`add`等方法修改集合，否则会报java.lang.UnsupportedOperationException异常。
<a name="iZBCk"></a>
## 2、使用`ArrayList`的`subList`的注意事项
先来看下`subList`的简单使用：
```java
List<String> bookList = new ArrayList<>();
bookList.add("遥远的救世主");
bookList.add("背叛");
bookList.add("天幕红尘");
bookList.add("人生");
bookList.add("平凡的世界");

List<String> luyaoBookList = bookList.subList(3, 5);

System.out.println(bookList);
System.out.println(luyaoBookList);
```
运行结果如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348065055-b4662b14-f5f4-4c41-9d8e-59c4c668deb9.png#averageHue=%23f7f6f6&clientId=uec3f69cf-22be-4&from=paste&height=131&id=ub28a607a&originHeight=328&originWidth=1812&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=79379&status=done&style=shadow&taskId=u7a994b9c-a1fe-4774-a417-9d6685f9615&title=&width=724.8)<br />从运行结果可以看出，`subList`返回的是`bookList`中索引从`fromIndex`（包含）到`toIndex`（不包含）的元素集合。<br />使用起来很简单，也很好理解，不过还是有以下几点要注意，否则会造成程序错误或者异常：<br />**1、** 修改原集合元素的值，会影响子集合；<br />**2、** 修改原集合的结构，会引起ConcurrentModificationException异常；<br />**3、** 修改子集合元素的值，会影响原集合；<br />**4、** 修改子集合的结构，会影响原集合；<br />以上几点在《阿里巴巴Java开发手册》泰山版中是这样描述的：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702349351644-445efcb2-6a2b-4ed8-a895-97b19595c548.png#averageHue=%23f4f2f1&clientId=u93813566-aac9-4&from=paste&height=128&id=u5e270a01&originHeight=321&originWidth=1826&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=119833&status=done&style=shadow&taskId=u25ebb014-55dc-4b67-891a-669f7f79812&title=&width=730.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702349419790-47fe3ddf-cc10-4850-bd94-843dc6ebc036.png#averageHue=%23f4f1f0&clientId=u93813566-aac9-4&from=paste&height=62&id=u2de899e9&originHeight=156&originWidth=1826&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=71856&status=done&style=shadow&taskId=ud15f9923-8d48-41ed-935b-b278a6255c2&title=&width=730.4)
<a name="O8nxo"></a>
### 2.1 修改原集合的值，会影响子集合
比如，修改下原集合bookList中某一元素的值（**非结构性修改**）：
```java
List<String> bookList = new ArrayList<>();
bookList.add("遥远的救世主");
bookList.add("背叛");
bookList.add("天幕红尘");
bookList.add("人生");
bookList.add("平凡的世界");

List<String> luyaoBookList = bookList.subList(3, 5);

System.out.println(bookList);
System.out.println(luyaoBookList);

// 修改原集合的值
bookList.set(3,"路遥-人生");

System.out.println(bookList);
System.out.println(luyaoBookList);
```
运行结果如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348280459-744cdc19-dd55-4b7d-a9c9-5d8e16dff777.png#averageHue=%23f9f8f7&clientId=uec3f69cf-22be-4&from=paste&height=533&id=u4b16b903&originHeight=1332&originWidth=2112&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=330750&status=done&style=shadow&taskId=u4377bef5-f09d-46c8-81c9-8a6d9ca6d40&title=&width=844.8)<br />可以看出，虽然只是修改了原集合bookList的值，但是影响到了子集合luyaoBookList。
<a name="y4Mi6"></a>
### 2.2 修改原集合的结构，会引起ConcurrentModificationException异常
比如，往原集合bookList中添加一个元素（**结构性修改**）：
```java
List<String> bookList = new ArrayList<>();
bookList.add("遥远的救世主");
bookList.add("背叛");
bookList.add("天幕红尘");
bookList.add("人生");
bookList.add("平凡的世界");

List<String> luyaoBookList = bookList.subList(3, 5);

System.out.println(bookList);
System.out.println(luyaoBookList);

// 往原集合中添加元素
bookList.add("早晨从中午开始");

System.out.println(bookList);
System.out.println(luyaoBookList);
```
运行结果如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348428612-1d1f4984-3325-4183-ae8c-712efb584b6a.png#averageHue=%23f8f6f4&clientId=uec3f69cf-22be-4&from=paste&height=564&id=u29f29c74&originHeight=1409&originWidth=2412&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=503961&status=done&style=shadow&taskId=ue4552ca7-05e5-4b2c-a1f8-7b6d98251df&title=&width=964.8)<br />可以看出，当往原集合中添加了元素（结构性修改）后，在遍历子集合时，发生了ConcurrentModificationException异常。<br />注意事项：以上异常并不是在添加元素时发生的，而是在添加元素后，遍历子集合时发生的。<br />关于这一点，在《阿里巴巴Java开发手册》泰山版中是这样描述的：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702349449783-0fefa060-c0b5-4965-996d-1c4ab7b5e812.png#averageHue=%23f4f1f0&clientId=u93813566-aac9-4&from=paste&height=62&id=ue2e222fb&originHeight=156&originWidth=1826&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=71856&status=done&style=shadow&taskId=ue32efeec-4722-4104-91cc-4c77b461fd0&title=&width=730.4)
<a name="ltuA4"></a>
### 2.3 修改子集合的值，会影响原集合
比如，修改下子集合luyaoBookList中某一元素的值（**非结构性修改**）：
```java
List<String> bookList = new ArrayList<>();
bookList.add("遥远的救世主");
bookList.add("背叛");
bookList.add("天幕红尘");
bookList.add("人生");
bookList.add("平凡的世界");

List<String> luyaoBookList = bookList.subList(3, 5);

System.out.println(bookList);
System.out.println(luyaoBookList);

// 修改子集合的值
luyaoBookList.set(1,"路遥-平凡的世界");

System.out.println(bookList);
System.out.println(luyaoBookList);
```
运行结果如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348554330-e1a6fe4e-d881-457c-936f-04ea5114f796.png#averageHue=%23f7f6f4&clientId=uec3f69cf-22be-4&from=paste&height=426&id=u3fcd1630&originHeight=1064&originWidth=2080&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=316350&status=done&style=shadow&taskId=u3c88d24f-1000-497b-9bc8-f44afb1969f&title=&width=832)<br />可以看出，虽然只是修改了子集合luyaoBookList的值，但是影响到了原集合bookList。
<a name="hDqmQ"></a>
### 2.4 修改子集合的结构，会影响原集合
比如，往子集合luyaoBookList中添加一个元素（**结构性修改**）：
```java
List<String> bookList = new ArrayList<>();
bookList.add("遥远的救世主");
bookList.add("背叛");
bookList.add("天幕红尘");
bookList.add("人生");
bookList.add("平凡的世界");

List<String> luyaoBookList = bookList.subList(3, 5);

System.out.println(bookList);
System.out.println(luyaoBookList);

// 往子集合中添加元素
luyaoBookList.add("早晨从中午开始");

System.out.println(bookList);
System.out.println(luyaoBookList);
```
运行结果如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348680521-6723b904-b328-4047-a9e2-7719f6b8de10.png#averageHue=%23f9f8f6&clientId=uec3f69cf-22be-4&from=paste&height=499&id=u42858cee&originHeight=1248&originWidth=2308&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=356079&status=done&style=shadow&taskId=ue3d0b5f9-cd51-452d-a0f7-487e1301342&title=&width=923.2)<br />可以看出，当往子集合中添加了元素（结构性修改）后，影响到了原集合bookList。
<a name="XUJlj"></a>
### 2.5 原因分析
首先，看下`subList`方法的注释，了解下它的用途：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348764846-37a14642-e28d-4740-88aa-ed7c71385a1f.png#averageHue=%23faf9f8&clientId=uec3f69cf-22be-4&from=paste&height=161&id=u1562923d&originHeight=403&originWidth=2485&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=132206&status=done&style=shadow&taskId=u5d7b2e63-e22e-4c3a-86fc-cf1a7a8a873&title=&width=994)
> Returns a view of the portion of this list between the specified {@code fromIndex}, inclusive, and {@code toIndex}, exclusive.

翻译过来意思就是：<br />返回指定的{@code fromIndex}(包含)和{@code toIndex}(排除)之间的列表部分的视图。<br />然后，看下它的源码：
```java
public List<E> subList(int fromIndex, int toIndex) {

    subListRangeCheck(fromIndex, toIndex, size);
    return new SubList(this, 0, fromIndex, toIndex);
}
```
可以看到，它调用了`SubList`类的构造函数，该构造函数的源码如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1702348899128-75a67739-4253-4486-80f2-8c65c9fd55b5.png#averageHue=%23fcfcfc&clientId=uec3f69cf-22be-4&from=paste&height=528&id=uca2a59f2&originHeight=1321&originWidth=2705&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=308324&status=done&style=shadow&taskId=uc00ca2d2-8fb7-4a9b-a908-42ef82f5e87&title=&width=1082)<br />可以看出，`SubList`类是`ArrayList`的内部类，该构造函数中也并没有重新创建一个新的`ArrayList`，所以修改原集合或者子集合的元素的值，是会相互影响的。
<a name="WDned"></a>
### 2.6 总结
`ArrayList`的`subList`方法，返回的是原集合的一个子集合（视图），非结构性修改任意一个集合的元素的值，都会彼此影响，结构性修改原集合时，会报ConcurrentModificationException异常，结构性修改子集合时，会影响原集合，所以使用时要注意，避免程序错误或者异常
