Java
<a name="BSgIf"></a>
## 遍历元素
首先，以一段代码为例
```java
String[] array = {"1", "2", "3"};
for (String i : array) {
    System.out.println(i);
}
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
for (String i : list) {
    System.out.println(i);
}
```
结果
```
1
2
3
111
222
333
```
结果毫无疑问。<br />再来看看编译后的源码（idea自带，在target包里打开类源码文件即可）
```java
String[] array = new String[]{"1", "2", "3"};
String[] var2 = array;
int var3 = array.length;
for(int var4 = 0; var4 < var3; ++var4) {
    String i = var2[var4];
    System.out.println(i);
}
ArrayList<String> list = new ArrayList();
list.add("111");
list.add("222");
list.add("333");
Iterator var7 = list.iterator();
while(var7.hasNext()) {
    String i = (String)var7.next();
    System.out.println(i);
}
```
可见，遍历数组使用的是原始for循环，集合的话使用的是Iterator迭代器。
<a name="l9rir"></a>
## 删除元素
接下来删除元素：
<a name="c69U9"></a>
### 使用for循环
```java
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
log.info(list.toString());
for (int i = 0; i <list.size(); i++) {
    list.remove("222");
}
log.info(list.toString());
```
**结果**
```
11:11:52.532 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
11:11:52.539 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 333]
```
显然成功！
<a name="IZ4ws"></a>
### 使用foreach
```java
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
log.info(list.toString());
for (String i : list) {
    list.remove("222");
}
log.info(list.toString());
```
**结果**
```
11:50:48.333 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
Exception in thread "main" java.util.ConcurrentModificationException
at java.util.ArrayList$Itr.checkForComodification(ArrayList.java:909)
at java.util.ArrayList$Itr.next(ArrayList.java:859)
at com.xiaolinge.com.hello.HelloWord.main(HelloWord.java:30)
```
显然没有成功！<br />**原因**<br />迭代器内部的每次遍历都会记录List内部的modcount当做预期值，然后在每次循环中用预期值与List的成员变量modCount作比较，但是普通list.remove调用的是List的remove，这时modcount++，但是iterator内记录的预期值并没有变化，所以会报错。<br />如果想要删除元素的话需要使用迭代器内部的`remove`方法
```java
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
log.info(list.toString());
Iterator<String> it = list.iterator();
while (it.hasNext()){
    String next = it.next();
    //if外使用list的remove方法还是会报错的
    if(next.equals("222")){
        it.remove();//这里使用的是迭代器里面的remove（）方法，
        // 当然如果使用list的remove方法在此删除质地感元素的话是成功的，比如：list.remove("222")
    }
}
log.info(list.toString());
```
**结果**
```
12:06:14.042 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
12:06:14.046 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 333]
```
<a name="ErMx6"></a>
## 修改元素
<a name="TMiHQ"></a>
### 使用原始for
```java
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
log.info(list.toString());
for (int i = 0; i <list.size(); i++) {
    list.set(i,"444");
}
log.info(list.toString());
```
**结果**
```
12:12:56.910 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
12:12:56.915 [main] INFO com.xiaolinge.com.hello.HelloWord - [444, 444, 444]
```
可以修改元素；
<a name="MnGR7"></a>
### 使用foreach
```java
ArrayList<String> list = new ArrayList<>();
list.add("111");
list.add("222");
list.add("333");
log.info(list.toString());
for (String i : list) {
    i="444";
}
log.info(list.toString());
```
**结果**
```
12:34:47.207 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
12:34:47.211 [main] INFO com.xiaolinge.com.hello.HelloWord - [111, 222, 333]
```
看到咯，不行的哦。<br />那么，修改元素不行，修改元素的属性可不可以呢？来看下吧。
<a name="aVfHK"></a>
### foreach修改元素属性
（for就不测试了）<br />创建一个学生类
```java
public class Student {
    private int age;
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    private String name;
    public Student(){};
    public Student(int age,String name){
        this.age=age;
        this.name=name;
    }
}
```
接下来测试代码
```java
Student student=new Student(1,"huge");
Student student1=new Student(1,"xiaoyao");
List<Student> studentList=new ArrayList<Student>();
studentList.add(student);
studentList.add(student1);
System.out.println(student.getName());
System.out.println(student1.getName());
for(Student stu:studentList)
{
    stu.setName("jingtian");
}
System.out.println(student.getName());
System.out.println(student1.getName());
```
**结果**
```
huge
xiaoyao
jingtian
jingtian
```
是不是很神奇！修改不了对象，却可以修改对象的属性。
<a name="o9h2O"></a>
## 总结
1、for与foreach都可以遍历数组/集合，不过for则在较复杂的循环中效率更高。<br />2、foreach不可以删除/修改集合元素，而for可以<br />3、foreach和for都可以修改元素里面的属性<br />所以相比较下来for循环更为灵活。
