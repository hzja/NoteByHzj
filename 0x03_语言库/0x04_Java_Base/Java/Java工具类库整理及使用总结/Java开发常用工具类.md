Java Utils
<a name="bKrHg"></a>
### **一、字符串相关**
String 类在平常的java开发过程中用的最多的一个类，经常需要围绕String做一些处理，JDK自身也提供了很多String API，但是功能都比较基础。通常需要结合String 多个方法才能完成一个业务功能。<br />首先介绍的是Apache提供的**StringUtils**工具类<br />需要引入的pom依赖：
```xml
<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-lang3</artifactId>
  <version>3.10</version>
</dependency>
```
:::info
**说明：commons-lang3是一直在维护的新版本，建议使用这个，而不是commons-lang**
:::
<a name="rOjWe"></a>
#### 1、判断字符串是否为空
```java
String str = "hello world";
if (null == str || str.isEmpty()) {  
}
```
上面的写法想必大家应该都写过，这段代码虽然简单，但是一不小心很容易造成空指针异常<br />使用StringUtils写法如下：
```java
if(StringUtils.isEmpty(str)){
}
```
上面的代码判断为字符串为空是不是更简单呢，不过需要注意的是下面这种情况
```java
public class UtilsTest {
    public static void main(String[] args) {
        String str1 = "";
        String str2 = " ";
        System.out.println(StringUtils.isEmpty(str1));
        System.out.println(StringUtils.isEmpty(str2));
    }
}
```
<a name="8b9ab1c5"></a>
#### 2、字符串固定长度
```java
String result = StringUtils.leftPad("test",8,"0");
```
上面leftPad方法表示返回字符串固定长度8，如果不足，在左边补0
<a name="P9HNP"></a>
#### 3、关键字替换
```java
StringUtils.replace("abc","a","A"); 
StringUtils.replaceOnce("aba","a","A"); 
StringUtils.replacePattern("ABCabc123","[^A-Z0-9]+","");
```
<a name="h5wQI"></a>
#### 4、字符串拼接
```java
String[] array = new String[]{"abc","123","456"};
StringBuilder stringBuilder = new StringBuilder();
for(String s:array){
    stringBuilder.append(s).append(";");
} 
System.out.println(stringBuilder.toString());
```
StringUtils类使用
```java
String[] array = new String[]{"abc","123","456"};
StringUtils.join(array,";")
```
<a name="UvDEy"></a>
#### 5、字符串拆分
```java
StringUtils.split("a..b.c",",")
```
<a name="zT8S6"></a>
### **二、日期相关**
DateUtils和DateFormatUtils<br />在JDK8之前，Java只提供了一个Date类，当需要将Date按照一定格式转换成字符串的时候，这个时候需要使用到SimpleDateFormat类。
<a name="El2vD"></a>
#### 1、日期转指定格式的字符串
```java
public class UtilsTest {
    public static void main(String[] args) {
        Date date = new Date();
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        
        String dateString=simpleDateFormat.format(date);
        System.out.println(dateString);
    }
}
```
看起来好像也很简单，但是这里面用到的SimpleDateFormat不是线程安全的，这就导致在多线程环境下可能出现线程安全的问题，因此，可以使用common-lang3下的时间工具类DataUtils/DateFormateUtils，从而解决Date与字符串的转换问题。<br />方法很简单，上面代码的转换等价于如下代码：
```java
String dateString = DateFormatUtils.format(new Date(),"yyyy-MM-dd HH:mm:ss");
System.out.println(dateString);
```
<a name="powNA"></a>
#### 2、字符串转日期
```java
Date date = DateUtils.parseDate("2020-10-15 22:00:00","yyyy-MM-dd HH:mm:ss");System.out.println(date);
```
输出结果：
```
Thu Oct 15 22:00:00 CST 2020
```
<a name="SJRkT"></a>
#### 3、DateUtils时间计算
除开上面的日期转换之外，DateUtils还提供了很方便的时间计算的功能函数<br />直接贴代码：
```java
public class UtilsTest {
    public static void main(String[] args){
        Date now = new Date();
        //Date加1天
        Date addDays = DateUtils.addDays(now, 1);
        //Date加33分钟
        Date addMinutes = DateUtils.addMinutes(now, 33);
        //Date减去60秒
        Date addSeconds = DateUtils.addSeconds(now, -60);
        //过滤时分秒
        Date date = Dateutils.truncate(now, Calendar.DATE);
        System.out.println(date);
        System.out.println(addDays);
        System.out.println(addMinutes);
        System.out.println(addSeconds);
    }
}
```
输出结果：
```java
Thu 0ct 15 00:00:00 CST 2020
Fri 0ct 16 22:50:46 CST 2020
Thu 0ct 15 23:23:46 CST 2020
Thu 0ct 15 22:49:46 CST 2020
```
<a name="JRvHs"></a>
### **三、集合数组相关**
<a name="3G6B8"></a>
#### 1、判断是否为空
以前这样判断
```java
List<String> list = new ArrayList<String>();
if(null==list||list.isEmpty()){
}
```
上面写起来其实也不难，但是也比较容易抛出空指针异常，现在可以通过使用commons-collections类来判断是否为空<br />pom依赖：
```xml
<dependency>
    <groupId>org.apache.commons</groupId>
    <artifactId>commons-collections4</artifactId>
    <version>4.4</version>
</dependency>
```
使用CollectionUtils/MapUtils/ArrayUtils进行判空判断
```java
List<String> list = new ArrayList<String>();
Map map = new HashMap();
String[] strings = new String[];
map.put("name", "zxb");


if (CollectionUtils.isEmpty(list)) {
  
}

if (MapUtils.isEmpty(map)){
  
}


if(ArrayUtils.isEmpty(strings)){
  
}
```
:::tips
其中要注意的是ArrayUtils是commons-lang3包下的
:::
<a name="0c3d13c4"></a>
#### 2、将数组快速加入到list中
直接上代码：
```java
public class UtilsTest {
    public static void main(String[] args){
        List<String> list = new ArrayList<String>();
        list.add("A");
        list.add("B");
        list.add("C");
        String[] arrays = new String[]{ "D", "E", "H"};
        CollectionUtils.addAll(list, arrays);
        System.out.print1n(list);
    }
}
```
输出：
```java
[A, B, C，D, E, H]
```
其他的方法这里不做过多补充，Google的Guava工具类也有很多对于集合的操作增强类
<a name="dNt5o"></a>
### **四、计时相关**
以前计时：
```java
long start = System.currentTimeMillis();
long end = System.currentTimeMillis();
System.out.println("运行时间："+(end-start)+"ms");
```
上面想必大家都用过，确实很简单，但是这种计时是非常不灵活的，默认情况下只能取ms为单位，如果需要分钟呢？那就需要另外计算来转换，这里给大家介绍Guava stopwatch计时工具类来统计程序执行时间<br />pom依赖：
```xml
<dependency>
    <groupId>com.google.guava</groupId>
    <artifactId>guava</artifactId>
    <version>19.0</version>
</dependency>
```
使用Stopwatch工具类统计程序执行时间：
```java
public class UtilsTest{
    public static void main(String[] args) throws InterruptedException {
        Stopwatch stopwatch = Stopwatch.createStarted();
        //创建计时器，但是不是立即执行，需要主动调用start方法开始计时
        //Stopwatch stopwatch = Stopwatch.createUnstacted();
        //stopwatch.start();
        //这里当做其他代码花费时间
        TimeUnit.SECONDS.sleep( timeout: 2l);
        //当前消耗的时间
        System.out.println(stopwatch.elapsed(TimeUnit.SECONDS));
        TimeUnit.SECONDS.sleep( timeout: 2l);
        stopwatch.stop();
        System.out.println(stopwatch.elapsed(TimeUnit.SECONDS));
        //重新开始,从原来时间基础上计算，如果想要重新从0开始计算，需要调用stopwatch.reset()
        stopwatch.start();
        TimeUnit.SECONDS.sleep( timeout: 2l);
        system.out.println(stopwatch.elapsed(TimeUnit.SECONDS));
    }
}
```
输出结果：
```
2
4
6
```


