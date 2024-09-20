JavaList<br />都知道 List 中是不允许在循环的过程中去进行移除元素的，为什么呢？一般的新人可能会遇到这个问题，比如说会从 List 的遍历的过程中去进行 remove 数据，但是干过几年的开发的有经验的工作人员，是肯定不会这么干的，很简单，会报错。
<a name="wKCUm"></a>
### List 进行 remove
可以来看一段代码：
```java
public static void main(String[] args) {
    List<String> list= new ArrayList<>();
    list.add("1");
    list.add("2");
    list.add("3");

    for (String s: list) {
        if (s.equals("1")) {
            list.remove(s);
        }
    }

    System.out.println(list);
}
```
上面这段代码，一般都是初入开发行业的小伙伴可能会这么写，但是当运行的时候，就会发现会报一个错误。
```
Exception in thread "main" java.util.ConcurrentModificationException
 at java.util.ArrayList$Itr.checkForComodification(ArrayList.java:909)
 at java.util.ArrayList$Itr.next(ArrayList.java:859)
 at com.example.fastdfs.Test.main(Test.java:22)
```
但是把代码改成删除元素 2 的时候，发现又成功了！！！<br />是真的成功了，看代码和运行结果：
```java
public static void main(String[] args) {
    List<String> list= new ArrayList<>();
    list.add("1");
    list.add("2");
    list.add("3");

    for (String s: list) {
        if (s.equals("2")) {
            list.remove(s);
        }
    }

    System.out.println(list);
}
```
运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669595432079-d3badeba-fea1-4dbf-a3c3-0169f6d0df42.png#averageHue=%232f2e2e&clientId=ufaed2275-27dc-4&from=paste&id=u1fad5ddf&originHeight=137&originWidth=587&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u963f0b42-3fd9-4146-a596-a8207ed4205&title=)<br />为什么会出现这种情况，删除第一个元素不行，删除第二个元素好用，删除第三个元素又不行了，到底是什么原因导致的呢？<br />这时候就得去看看他的源码编译出来是什么样子的。源码如下：
```java
public static void main(String[] args) {
    List<String> list = new ArrayList();
    list.add("1");
    list.add("2");
    list.add("3");
    Iterator var2 = list.iterator();

    while(var2.hasNext()) {
        String s = (String)var2.next();
        if (s.equals("3")) {
            list.remove(s);
        }
    }

    System.out.println(list);
}
```
也就是说，foreach 的循环内部，就是采用的`iteratior`形式，使用的核心方法是`hasnext()`和`next()`。<br />既然都使用迭代器了，为啥还是不行呢？来看看迭代器的源码，然后分析一下为啥不行。<br />其实可以从报错都能看出点端倪，报错信息是ArrayList.java:909  而这个方法内部则是这样的：
```java
final void checkForComodification() {
    if (modCount != expectedModCount)
        throw new ConcurrentModificationException();
}
```
源代码在执行 remove 方法的时候后，也是调用的 list 当中的`remove` 方法，源代码中，就是这段：
```java
public boolean remove(Object o) {
    if (o == null) {
    for (int index = 0; index < size; index++)
        if (elementData[index] == null) {
            fastRemove(index);
            return true;
        }
    } else {
        for (int index = 0; index < size; index++)
            if (o.equals(elementData[index])) {
                fastRemove(index);
                return true;
            }
    }
    return false;
}
```
中间调用的 `fastRemove` 方法中，中间就看到了：
```java
private void fastRemove(int index) {
    modCount++;
    int numMoved = size - index - 1;
    if (numMoved > 0)
        System.arraycopy(elementData, index+1, elementData, index,
                         numMoved);
    elementData[--size] = null; // clear to let GC do its work
}
```
这里`modCount++`了<br />而当再一次循环的时候，调用的是list内部类itr的`next`方法，<br />在调用的list的`remove`的时候，`modCount++`了，而`expectedModCount`是等于最开始`modCount`值。<br />这时候二者的值不相等的时候，就出现异常了。<br />归根结底，虽然这个地方使用的是迭代器的遍历，但是remove 的方法可不是迭代器的方法呀。<br />那么使用迭代器遍历然后移除是什么样子的呢？
```java
public static void main(String[] args) {
    List<String> list= new ArrayList<>();
    list.add("1");
    list.add("2");
    list.add("3");

    Iterator<String> iterator = list.iterator();
    while (iterator.hasNext()){
        String next = iterator.next();
        if ("3".equals(next)){
            iterator.remove();
        }
    }

    System.out.println(list);
}
```
这么写是不是有点多，那么应该如何快速的写完这段代码呢？<br />其实一行代码就能很快解决这个事情，先来看代码怎么写的：
```java
list.removeIf(vo-> "3".equals(vo));
```
也不用管返回值了，只要是能满足这个条件的 ，那么就会从集合中给移除掉。<br />看结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669595432098-88391b93-4d32-440e-9f75-c022c7dca51d.png#averageHue=%232d2d2c&clientId=ufaed2275-27dc-4&from=paste&id=u0b84fc60&originHeight=225&originWidth=583&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ueb6ad998-0da6-4ad0-ab83-a6fbe1c63b6&title=)<br />这么一看，是不是发现非常简单方便，而且还快捷，而且如果要是对于代码量来说的话，那肯定是非常的少的，但凡满足条件的，肯定可以。
<a name="snur0"></a>
### removeIf 的进阶玩法
为什么称之为进阶玩法，实际上也并不是完整的进阶玩法，比如说如果有一个功能是这样的，要求做一个导入的功能，然后导入的数据只有一个车牌号是唯一值，之前导入的数据，不做处理，新增的文件中，可能会包含所有的数据，要求数据库中已经存在的数据，不处理，然后导入数据库中不存在的数据。<br />如果字段少的话，那么实现思路可能会有几种。<br />第一种：<br />mybatis 的 SelectKey 标签，判断是否存在，如果存在就不进行新增。<br />第二种：<br />导入之前，查询数据库数据，比对数据，然后直接进行remove，最后不存在的数据导入<br />这两种方法实际上都能实现，但是他们的适用情况就不太一样了，如果字段非常多呢？<br />自己写sql 的话，那么代价实在是有点大，如果使用的还是 Mybatis-plus 的话，那么肯定第一种方式好像就没办法使用了，只能使用第二种了。<br />那么 removeIf 应该怎么来写呢？
```java
//创建第一个UserList
List<User> userList = new ArrayList<>();
User user = new User();
user.setId(UUID.randomUUID().toString());
user.setName("张三");
user.setAge(20);
user.setDept("开发部");
userList.add(user);

User user1 = new User();
user1.setId(UUID.randomUUID().toString());
user1.setName("李四");
user1.setAge(22);
user1.setDept("测试部");
userList.add(user1);

User user2 = new User();
user2.setId(UUID.randomUUID().toString());
user2.setName("王五");
user2.setAge(27);
user2.setDept("财务部");
userList.add(user2);

//创建第二个UserList

List<User> userEnd= new ArrayList<>();
User user3= new User();
user3.setId(UUID.randomUUID().toString());
user3.setName("张三");
user3.setAge(20);
user3.setDept("开发部");
userEnd.add(user3);

User user4= new User();
user4.setId(UUID.randomUUID().toString());
user4.setName("李四");
user4.setAge(22);
user4.setDept("测试部");
userEnd.add(user4);
```
如果这时候要把第一个userList 中的数据导入数据库，而 userEnd 则是数据库中的数据，这时候，要根据姓名来区分的话，是不是应该之导入王五才对，这时候得筛选出王五的数据来，然后做导入，这个时候 removeIf 就派上用场了。
```java
userList.removeIf(us1-> userEnd.stream().anyMatch(u ->us1.getName().equals(u.getName())));

System.out.println(Arrays.toString(userList.toArray()));
```
最后来看看结果：
```
[User(id=029b0b0f-ad42-4c15-8341-a3bb401be6d6, name=王五, age=27, dept=财务部)]
```
是不是已经做到了呢？
