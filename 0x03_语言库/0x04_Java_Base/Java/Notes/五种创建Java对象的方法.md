Java<br />作为Java开发人员，通常每天都会创建许多对象，但是总是使用新的或依赖管理系统(例如Spring)来创建这些对象。但是还有更多方法可以创建对象。<br />用Java创建对象的共有5种核心方式，下面将以其示例加以说明，然后介绍创建对象的行的字节码。但是，有很多API可以创建对象，但是这些API还将间接使用这5种核心方式之一，例如Spring BeanFactory。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1648647142434-485ac253-3412-47b9-89fe-8377edc8b5be.jpeg#clientId=u023c38b5-b200-4&from=paste&id=uf73c2571&originHeight=411&originWidth=945&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u955a959f-76c0-4993-b580-431c2f869ea&title=)<br />如果将执行最后给出的程序，则会看到方法1、2、3使用构造函数创建对象，而4、5没有调用构造函数创建对象。
<a name="VqQB4"></a>
## 1、使用新关键字
这是创建对象的最常见和常规的方法，也是一种非常简单的方法。通过使用此方法，可以调用要调用的任何构造函数(无参数的构造函数和参数化的)。
```java
Employee emp1 = new Employee();
```
<a name="ni7AS"></a>
## 2、使用Class类的`newInstance()`方法
还可以使用`Class`类的`newInstance()`方法创建一个对象。此`newInstance()`方法调用no-arg构造函数来创建对象。<br />可以通过`newInstance()`通过以下方式创建对象：
```java
Employee emp2 = (Employee) Class.forName("org.programming.mitra.exercises.Employee").newInstance();
```
Or
```java
Employee emp2 = Employee.class.newInstance();
```
<a name="SdJM8"></a>
## 3、使用构造方法类的`newInstance()`方法
与`Class`类的`newInstance()`方法类似，`java.lang.reflect.Constructor`类中有一个`newInstance()`方法可用于创建对象。还可以使用此`newInstance()`方法来调用参数化构造函数和私有构造函数。
```java
Constructor<Employee> constructor = Employee.class.getConstructor();

Employee emp3 = constructor.newInstance();
```
这两种`newInstance()`方法都被称为创建对象的反射方式。实际上，`Class`类的`newInstance()`方法在内部使用了`Constructor`类的`newInstance()`方法。这就是为什么最好使用后一种方法，并且也将其用于诸如Spring，Hibernate，Struts等不同框架的原因。要了解这两种`newInstance()`方法之间的区别，请阅读使用Example在Java中通过`Reflection`创建对象。
<a name="XqXoo"></a>
## 4、使用`clone()`方法：
每当在任何对象上调用`clone()`时，JVM实际上都会创建一个新对象，并将先前对象的所有内容复制到其中。使用`clone`方法创建对象不会调用任何构造函数。<br />要在对象上使用`clone()`方法，需要实现`Cloneable`并在其中定义`clone()`方法。
```java
Employee emp4 = (Employee) emp3.clone();
```
Java克隆是Java社区中最值得探讨的话题，它的确有其缺点，但是在对象完全满足Java克隆的强制条件之前，它仍然是创建任何对象的副本的最流行和最简单的方法。
<a name="a2cxe"></a>
## 5、使用反序列化
每当序列化然后反序列化一个对象时， JVM就会创建一个单独的对象。在反序列化中，JVM不使用任何构造函数来创建对象。要反序列化对象，需要在类中实现`Serializable`接口。
```java
ObjectInputStream in = new ObjectInputStream(new FileInputStream("data.obj"));
Employee emp5 = (Employee) in.readObject();
```
正如在上面的字节码中看到的那样，所有四个方法调用都转换为`invokevirtual`(这些方法直接处理对象的创建)，除了第一个转换为两个调用的方法是一个是新的，另一个是调用特殊的(对构造函数的调用)。<br />例：考虑一个要为其创建对象的Employee类：
```java
class Employee implements Cloneable, Serializable {

    private static final long serialVersionUID = 1L;

    private String name;

    public Employee() { System.out.println("Employee Constructor Called..."); }

    public String getName() { return name; }

    public void setName(String name) { this.name = name; }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Employee employee = (Employee) o;
        return Objects.equals(name, employee.name);
    }

    @Override
    public int hashCode() { return Objects.hash(name); }

    @Override
    public String toString() { return String.format("Employee{name='%s'}", name); }

    @Override
    public Object clone() {
        Object obj = null;
        try {
            obj = super.clone();
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        return obj;
    }
}
```
在下面的Java程序中，将以所有5种方式创建Employee对象。
```java
public class ObjectCreation {
	public static void main(String... args) throws Exception {
		
		// 1. Using new keyword
		Employee emp1 = new Employee();
		emp1.setName("emp1");
		
		
		// 2. Using Class class's newInstance() method
		Employee emp2 = Employee.class.newInstance();
		emp2.setName("emp2");
		
		
		// 3. Using Constructor class's newInstance() method
		Constructor<Employee> constructor = Employee.class.getConstructor();
		Employee emp3 = constructor.newInstance();
		emp3.setName("emp3");
		
		// 4. Using clone() method
		Employee emp4 = (Employee) emp3.clone();
		emp4.setName("emp4");
		
		
		// Serialization
		try (ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream("data.obj"))) {
			out.writeObject(emp4);
		}
		
		// 5. Using Deserialization
		Employee emp5;
		try (ObjectInputStream in = new ObjectInputStream(new FileInputStream("data.obj"))) {
			emp5 = (Employee) in.readObject();
			emp5.setName("emp5");
		}
		
		System.out.println(emp1 + ", hashcode : " + emp1.hashCode());
		System.out.println(emp2 + ", hashcode : " + emp2.hashCode());
		System.out.println(emp3 + ", hashcode : " + emp3.hashCode());
		System.out.println(emp4 + ", hashcode : " + emp4.hashCode());
		System.out.println(emp5 + ", hashcode : " + emp5.hashCode());
	}
}
```
该程序将给出以下输出：
```java
Employee Constructor Called...
Employee Constructor Called...
Employee Constructor Called...
Employee{name='emp1'}, hashcode : 3117192
Employee{name='emp2'}, hashcode : 3117193
Employee{name='emp3'}, hashcode : 3117194
Employee{name='emp4'}, hashcode : 3117195
Employee{name='emp5'}, hashcode : 3117196
```
