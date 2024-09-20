Java
<a name="SksOv"></a>
## 1、使用`Objects.equals`比较对象
这种方法相信大家并不陌生，甚至很多人都经常使用。是JDK7提供的一种方法，可以快速实现对象的比较，有效避免烦人的空指针检查。但是这种方法很容易用错，例如：
```java
Long longValue = 123L;
System.out.println(longValue==123); //true
System.out.println(Objects.equals(longValue,123)); //false
```
为什么替换`==`为`Objects.equals()`会导致不同的结果？这是因为使用`==`编译器会得到封装类型对应的基本数据类型`longValue`，然后与这个基本数据类型进行比较，相当于编译器会自动将常量转换为比较基本数据类型，而不是包装类型。<br />使用该`Objects.equals()`方法后，编译器默认常量的基本数据类型为int。下面是源码`Objects.equals()`，其中`a.equals(b)`使用的是`Long.equals()`会判断对象类型，因为编译器已经认为常量是int类型，所以比较结果一定是false。
```java
public static boolean equals(Object a, Object b) {
    return (a == b) || (a != null && a.equals(b));
}
    
public boolean equals(Object obj) {
    if (obj instanceof Long) {
        return value == ((Long)obj).longValue();
    }
    return false;
}
```
知道了原因，解决方法就很简单了。直接声明常量的数据类型，如`Objects.equals(longValue,123L)`。其实如果逻辑严密，就不会出现上面的问题。需要做的是保持良好的编码习惯。
<a name="Y083q"></a>
## 2、日期格式错误
在日常的开发中，经常需要对日期进行格式化，但是很多人使用的格式不对，导致出现意想不到的情况。请看下面的例子。
```java
Instant instant = Instant.parse("2021-12-31T00:00:00.00Z");
DateTimeFormatter formatter = DateTimeFormatter.ofPattern("YYYY-MM-dd HH:mm:ss")
.withZone(ZoneId.systemDefault());
System.out.println(formatter.format(instant));//2022-12-31 08:00:00
```
以上用于YYYY-MM-dd格式化，年从2021 变成了 2022。为什么？这是因为 java 的DateTimeFormatter 模式YYYY和yyyy之间存在细微的差异。它们都代表一年，但是yyyy代表日历年，而YYYY代表星期。这是一个细微的差异，仅会导致一年左右的变更问题，因此代码本可以一直正常运行，而仅在新的一年中引发问题。12月31日按周计算的年份是2022年，正确的方式应该是使用yyyy-MM-dd格式化日期。<br />这个bug特别隐蔽。这在平时不会有问题。它只会在新的一年到来时触发。
<a name="JD5HY"></a>
## 3、在 ThreadPool 中使用 ThreadLocal
如果创建一个ThreadLocal 变量，访问该变量的线程将创建一个线程局部变量。合理使用ThreadLocal可以避免线程安全问题。<br />但是，如果在线程池中使用ThreadLocal ，就要小心了。代码可能会产生意想不到的结果。举个很简单的例子，假设有一个电商平台，用户购买商品后需要发邮件确认。
```java
private ThreadLocal<User> currentUser = ThreadLocal.withInitial(() -> null);

private ExecutorService executorService = Executors.newFixedThreadPool(4);

public void executor() {
    executorService.submit(()->{
        User user = currentUser.get();
        Integer userId = user.getId();
        sendEmail(userId);
    });
}
```
如果使用ThreadLocal来保存用户信息，这里就会有一个隐藏的bug。因为使用了线程池，线程是可以复用的，所以在使用ThreadLocal获取用户信息的时候，很可能会误获取到别人的信息。可以使用会话来解决这个问题。
<a name="M9HHP"></a>
## 4、使用HashSet去除重复数据
在编码的时候，经常会有去重的需求。一想到去重，很多人首先想到的就是用HashSet去重。但是，不小心使用 HashSet 可能会导致去重失败。
```java
User user1 = new User();
user1.setUsername("test");

User user2 = new User();
user2.setUsername("test");

List<User> users = Arrays.asList(user1, user2);
HashSet<User> sets = new HashSet<>(users);
System.out.println(sets.size());// the size is 2
```
细心的读者应该已经猜到失败的原因了。HashSet使用hashcode对哈希表进行寻址，使用equals方法判断对象是否相等。如果自定义对象没有重写hashcode方法和equals方法，则默认使用父对象的hashcode方法和equals方法。所以HashSet会认为这是两个不同的对象，所以导致去重失败。
<a name="b4LeP"></a>
## 5、线程池中的异常被吃掉
```java
ExecutorService executorService = Executors.newFixedThreadPool(1);
executorService.submit(()->{
    //do something
    double result = 10/0;
});
```
上面的代码模拟了一个线程池抛出异常的场景。真正的业务代码要处理各种可能出现的情况，所以很有可能因为某些特定的原因而触发RuntimeException 。<br />但是如果没有特殊处理，这个异常就会被线程池吃掉。这样就会导致出现问题你都不知道，这是很严重的后果。因此，最好在线程池中try catch捕获异常。
