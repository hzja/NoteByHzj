Java
<a name="Ia2aS"></a>
### 一、为什么需要重写`hashCode()`方法和`equals()`方法?（Why）
有时在业务系统中判断对象时有时候需要的不是一种严格意义上的相等，而是一种业务上的对象相等。在这种情况下，原生的`equals`方法就不能满足需求了。<br />JavaBean的超类(父类)是Object类，JavaBean中的`equals`方法是继承自Object中的方法。Object类中定义的`equals()`方法是用来比较两个引用所指向的对象的内存地址是否一致。并不是比较两个对象的属性值是否一致，所以这时需要重写`equals()`方法。<br />Object类中`equals()`方法的源码
```java
public boolean equals(Object obj) {
    return (this == obj);
}

public class Demo {
    public static void main(String[] args) {
        Student stu1 = new Student("awu",22);
        Student stu2 = new Student("awu",22);
        System.out.println(stu1.equals(stu2));
        /*因为Student这个JavaBean没有重写关于属性值相等的equals()方法
                  ,所以默认比较的是地址值,从而输出结果为false*/    
    }
}
```
那么为什么在重写equals方法的时候需要重写`hashCode`方法呢？<br />主要是`Object.hashCode`的通用约定：

- 在java应用程序运行时，无论何时多次调用同一个对象时的`hsahCode()`方法，这个对象的`hashCode()`方法的返回值必须是相同的一个`int`值。
- 如果两个对象`equals()`返回值为true，则他们的`hashCode()`也必须返回相同的int值。
- 如果两个对象`equals()`返回值为false，则他们的`hashCode()`返回值也必须不同。

以`HashSet`来说明为什么要这么约定：`HashSet`存放元素时，根据元素的`hashCode`值快速找到要存储的位置，如果这个位置有元素，两个对象通过`equals()`比较，如果返回值为true，则不放入；如果返回值为false，则这个时候会以链表的形式在同一个位置上存放两个元素，这会使得HashSet的性能降低，因为不能快速定位了。<br />还有一种情况就是两个对象的`hashCode()`返回值不同，但是`equals()`返回`true`，这个时候`HashSet`会把这两个对象都存进去，这就和Set集合不重复的规则相悖了;所以，重写了`equals()`方法时，要按照b，c规则重写`hashCode()`方法！（其实就是如果只重写了 equals 方法，两个对象 equals 返回了true，但是如果没有重写 `hashCode` 方法，集合还是会插入元素。这样集合中就出现了重复元素了。）
<a name="XftwI"></a>
### 二、在什么情况下需要重写`hashCode()`方法和`equals()`方法? (When)
当自定义的一个类，想要把它的实例保存在以Hash散列查找的集合中时，就需要重写这两个方法；
```java
public class Student {
    private String name;

    private Integer age;

    public Student(){

    }

    public Student(String name,Integer age){
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    @Override  
    public int hashCode(){  
        final int prime = 31;  
        int result = 17;  
        result = prime * result + name.hashCode();  
        result = prime * result + age;  
        return result;  
    }  

    @Override  
    public boolean equals(Object obj){  
        if(this == obj)  
            return true;  
        if(obj == null)  
            return false;  
        if(getClass() != obj.getClass())  
            return false;  
        final Student other = (Student)obj;  
        return Objects.equals(name, other.name) && Objects.equals(age, other.age);
    }  

}

public class Demo {
    public static void main(String[] args) {
        Student stu1 = new Student("awu",22);
        Student stu3 = new Student("awu",33);
        Student stu2 = new Student("awu",22);

        Set set = new HashSet();
        set.add(stu1);
        set.add(stu2);
        set.add(stu3);

        System.out.println(set.size());
        /*输出结果为2*/

    }
}
```
如果不是以Hash散列查找的集合，即使重写`HashCode`也没多大实际用处。比如如下栗子:
```java
public class Demo {
    public static void main(String[] args) {
        Student stu1 = new Student("awu",22);
        Student stu3 = new Student("awu",33);
        Student stu2 = new Student("awu",22);

        ArrayList list = new ArrayList();
        list.add(stu1);
        list.add(stu2);
        list.add(stu3);

        System.out.println(list .size());
        /*输出结果为3*/

    }
}
```
<a name="HnBn9"></a>
### 三、如何重写这两个方法？(How)
```java
public class Student {
    private String name;

    private Integer age;

    public Student(){

    }

    public Student(String name,Integer age){
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    @Override  
    public int hashCode(){  
        final int prime = 31;  
        int result = 17;  
        result = prime * result + name.hashCode();  
        result = prime * result + age;  
        return result;  
    }  

    @Override  
    public boolean equals(Object obj){  
        if(this == obj)  
            return true;  
        if(obj == null)  
            return false;  
        if(getClass() != obj.getClass())  
            return false;  
        final Student other = (Student)obj;  
        return Objects.equals(name, other.name) && Objects.equals(age, other.age);  
    }

}
```
