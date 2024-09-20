Java17
<a name="mCjo2"></a>
## 项目快速集成Java 17
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632102725687-bfd91d43-87cf-4116-8047-f41fd3b6c6d1.png#clientId=uec2b82e5-3674-4&from=paste&height=552&id=ubb208ce9&originHeight=1656&originWidth=2003&originalType=binary&ratio=1&size=230807&status=done&style=shadow&taskId=u1995c21f-1ce7-4e97-bd93-ae6da4a8843&width=667.6666666666666)<br />IDEA 集成 Java 17<br />亚马逊的Corretto JDK 17 、Zulu JDK 17 都已经加入了豪华午餐。<br />IDEA默认解压到Windows当前用户文件夹路径下（这里的是C:\Users\fcsca\.jdks），之所以解压到.jdks下是因为IDEA的下载目标文件夹就是这个文件夹，方便IDEA自动检出。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632102808927-0348aab5-a96e-4782-b267-2c2afc3b1cb2.png#clientId=uec2b82e5-3674-4&from=paste&height=552&id=u3520ed48&originHeight=1656&originWidth=2003&originalType=binary&ratio=1&size=202103&status=done&style=none&taskId=ud58d46c6-efa3-429b-868c-c7996c8fed5&width=667.6666666666666)<br />解压完成的效果<br />这里不需要重新配置Java环境变量，都是项目级别的Java版本控制，不会对其它项目造成影响。<br />然后新建一个Maven项目（也可以是普通项目或者Gradle项目），这个时候还需要配置项目的语言级别等。
<a name="WM339"></a>
### 语言级别
调整JDK的语言级别为Java 17 ,在IDEA下按快捷键 Ctrl+Alt+Shift+S 呼出下面的对话框并将Language Level修改为17。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632103069467-d2c63ebe-9365-454a-ae4a-4b4a83c9edf0.png#clientId=uec2b82e5-3674-4&from=paste&height=651&id=uf60bfdef&originHeight=1954&originWidth=2558&originalType=binary&ratio=1&size=259484&status=done&style=none&taskId=ud52134f5-5625-4c67-9097-0cfd082fcd1&width=852.6666666666666)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632103087344-a3e5afbb-e0d9-43fc-b9f5-3c166ff4a0b1.png#clientId=uec2b82e5-3674-4&from=paste&height=651&id=u0d111e42&originHeight=1954&originWidth=2558&originalType=binary&ratio=1&size=238089&status=done&style=shadow&taskId=u07522261-9cf4-4980-9263-e4fa7b95ae4&width=852.6666666666666)<br />修改项目 JDK Level
<a name="iw0UQ"></a>
### 字节码版本
编译器的字节码版本也需要调整为17。在IDEA中按下快捷键 Ctrl+Alt+S 在图示中的位置进行修改。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632103188473-ec5d37ff-306b-455e-98f2-f0cac1049bdf.png#clientId=uec2b82e5-3674-4&from=paste&height=651&id=u08978bef&originHeight=1954&originWidth=2558&originalType=binary&ratio=1&size=306282&status=done&style=shadow&taskId=ua4c42322-565f-4f61-8eec-3dd8419cea1&width=852.6666666666666)<br />修改编译器的字节码版本
<a name="UjYJX"></a>
## Record Class
搞定了环境配置后，开始试一试一个最直观的、也相当有用的语法糖Record。<br />准确地说这不属于Java 17的新特性，最早在Java 14 中出现，在Java 16中转为正式特性。不过作为LTS版本，这依然是很重要的一个概念。<br />直观一些，一个数据类传统的写法是：
```java
public class MyRecord {
    private final String username;
    private final Integer age;

    public MyRecord(String username, Integer age) {
        this.username = username;
        this.age = age;
    }

    public String username() {
        return username;
    }

    public Integer age() {
        return age;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        MyRecord oldRecord = (MyRecord) o;
        return Objects.equals(username, oldRecord.username)
                && Objects.equals(age, oldRecord.age);
    }

    @Override
    public int hashCode() {
        return Objects.hash(username, age);
    }

    @Override
    public String toString() {
        return "MyRecord[" +
                "username='" + username + '\'' +
                ", age=" + age +
                ']';
    }
}
```
用`Record`就可以简化为：
```java
public record MyRecord(String username,Integer age) {
}
```
这样大大减少了一些模板代码，让逻辑更加清晰简单。一定意义上代替了Lombok。
<a name="ttLbo"></a>
### Record 是不可变的
`Record`被用来设计传输不可变的数据。从上面的例子可以看到，一个`Record`类被初始化后里面的属性是不能改变的，没有`Setter`方法而是通过全参数构造来初始化数据，天然线程安全。
<a name="qsGa8"></a>
### Record的超类
所有用`Record`关键字声明的类都是`java.lang.Record`的子类，这一点有点像枚举。
```java
public abstract class Record {
 
    protected Record() {}
 
    @Override
    public abstract boolean equals(Object obj);
 
    @Override
    public abstract int hashCode();
 
    @Override
    public abstract String toString();
}
```
从这里也可以看出所有`Record`的实现都覆写了`equals`、`hashCode`、`toString`三个方法。<br />如何判断一个类是`Record`类?<br />传统方法：
```java
Record.class.isAssignableFrom(MyRecord.class)
```
JDK提供了一个新的方法来解决这个问题：
```java
MyRecord.class.isRecord()
```
值得一提的是`Class`类还提供了`getRecordComponents`来获取`Record`类的成员属性信息。
```java
RecordComponent[] recordComponents = MyRecord.class.getRecordComponents();
```
<a name="jlyMw"></a>
### Record无法使用`extends`关键字
由于`Record`类的唯一的隐式超类是`java.lang.Record`，Java不支持多继承，使用 `extends` 显式定义会导致编译错误。
<a name="on38Q"></a>
### 无法定义额外的成员变量
Record类的成员变量只能通过构造声明。所以下面这种写法是错误的：
```java
public record MyRecord(String username,Integer age) {
    privite String gender;
}
```
但是可以在Record类中定义静态变量。
<a name="SmYDU"></a>
### 定义方法时需要小心
定义方法比较开放，但是请确保定义的方法不会破坏`Record`不可变的含义。不推荐定义`Setter`方法。<br />另外注意`Record`类的`Getter`方法不是`setXXXX`格式的。
<a name="jC7aF"></a>
### 使用注解
唯一需要注意的是，在`Record`类的成员变量上使用注解可能会作用的`Getter`方法上。就像这样：
```java
public record MyRecord(@Deprecated String username,Integer age) {
}
```
编译后：
```java
public record MyRecord(String username, Integer age) {
    public MyRecord(@Deprecated String username, Integer age) {
        this.username = username;
        this.age = age;
    }

    public String getUsername() {
        return this.username;
    }

    /** @deprecated */
    @Deprecated
    public String username() {
        return this.username;
    }

    public Integer age() {
        return this.age;
    }
}
```
具体的作用域需要根据注解上的`@Target`元注解的定义域来判定。
