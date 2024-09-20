Java
<a name="RZKAi"></a>
### 前言
当指执行插入排序、希尔排序、归并排序等算法时，比较两个对象“大小”的比较操作。很容易理解整型的 i>j 这样的比较方式，但当对多个对象进行排序时，如何比较两个对象的“大小”呢？<br />这样的比较 stu1 > stu2 显然是不可能通过编译的。为了解决如何比较两个对象大小的问题，JDK提供了两个接口 `java.lang.Comparable` 和 `java.util.Comparator` 。
<a name="Dxvad"></a>
### 一、自然排序：`java.lang.Comparable`
Comparable 接口中只提供了一个方法：`compareTo(Object obj)` ，该方法的返回值是 int 。如果返回值为正数，则表示当前对象(调用该方法的对象)比 obj 对象“大”；反之“小”；如果为零的话，则表示两对象相等。<br />下面是一个实现了 Comparable 接口的 Student 类：
```java
public class Student implements Comparable { 

    private int id; 

    private String name; 

    public Student() { 
        super(); 
    } 

    @Override 
    public int compareTo(Object obj) { 
        if (obj instanceof Student) { 
            Student stu = (Student) obj; 
            return id - stu.id; 
        } 
        return 0; 
    } 

    @Override 
    public String toString() { 
        return "<" + id + ", " + name + ">"; 
    } 
} 
```
Student 实现了自然排序接口 Comparable ，那么怎么利用这个接口对一组 Student 对象进行排序的呢？在学习数组的时候，使用了一个类来给整型数组排序：`java.util.Arrays` 。使用 Arrays 的 sort 方法来给整型数组排序。<br />翻翻 API 文档就会发现， Arrays 里给出了 sort 方法很多重载形式，其中就包括 `sort(Object[] obj)` ，也就是说 Arryas 也能对对象数组进行排序，排序过程中比较两个对象“大小”时使用的就是 Comparable 接口的 `compareTo` 方法。
```java
public class CompareTest { 

    public static void main(String[] args) { 
        Student stu1 = new Student(1, "Little"); 
        Student stu2 = new Student(2, "Cyntin"); 
        Student stu3 = new Student(3, "Tony"); 
        Student stu4 = new Student(4, "Gemini"); 

        Student[] stus = new Student[4]; 
        stus[0] = stu1; 
        stus[1] = stu4; 
        stus[2] = stu3; 
        stus[3] = stu2; 
        System.out.println(“Array: ” + Arrays.toString(stus)); 
        Arrays.sort(stus); 
        System.out.println(“Sort: ” + Arrays.toString(stus)); 
    } 
}
```
Student 数组里添加元素的顺序并不是按学号 id 来添加的。调用了 Arrays.sort(stus) 之后，对 Student 数组进行排序，不管 sort 是使用哪种排序算法来实现的，比较两个对象“大小”这个操作，它是肯定要做的。那么如何比较两个对象的“大小”？Student 实现的 Comparable 接口就发挥作用了。<br />sort 方法会将待比较的那个对象强制类型转换成 Comparable ，并调用 compareTo 方法，根据其返回值来判断这两个对象的“大小”。所以，在这个例子中排序后的原 Student 乱序数组就变成了按学号排序的 Student 数组。<br />但是注意到，排序算法和 Student 类绑定了， Student 只有一种排序算法。但现实社会不是这样的，如果不想按学号排序怎么办？假如，想按姓名来给学生排序怎么办？<br />只能修改 Student 类的 Comparable 接口的 compareTo 方法，改成按姓名排序。如果在同一个系统里有两个操作，一个是按学号排序，另外一个是按姓名排序，这怎么办？不可能在 Student 类体中写两个 compareTo 方法的实现。这么看来Comparable就有局限性了。为了弥补这个不足，JDK 还提供了另外一个排序方式，也就是下面要说的比较器排序。
<a name="IH6eb"></a>
### 二、比较器排序：`java.util.Comparator`
之所以提供比较器排序接口，是因为有时需要对同一对象进行多种不同方式的排序，这点自然排序 Comparable 不能实现。另外， Comparator 接口的一个好处是将比较排序算法和具体的实体类分离了。<br />翻翻 API 会发现， `Arrays.sort` 还有种重载形式：`sort(T[] a, Comparator<? super T> c)`，这个方法参数的写法用到了泛型，可以把它理解成这样的形式：`sort(Object[] a, Comparator c)`，这个方法的意思是按照比较器 c 给出的比较排序算法，对 Object 数组进行排序。<br />Comparator 接口中定义了两个方法：`compare(Object o1, Object o2)` 和 equals 方法，由于 equals 方法所有对象都有的方法，因此当实现 Comparator 接口时，只需重写 compare 方法，而不需重写 equals 方法。Comparator 接口中对重写 equals 方法的描述是：“注意，不重写 `Object.equals(Object)` 方法总是安全的。<br />然而，在某些情况下，重写此方法可以允许程序确定两个不同的 Comparator 是否强行实施了相同的排序，从而提高性能。”。只需知道第一句话就OK了，也就是说，可以不用去想应该怎么实现 equals 方法，因为即使不显示实现 equals 方法，而是使用Object类的 equals 方法，代码依然是安全的。<br />那么来写个代码，来用一用比较器排序。还是用 Student 类来做，只是没有实现 Comparable 接口。由于比较器的实现类只用显示实现一个方法，因此，可以不用专门写一个类来实现它，当需要用到比较器时，可以写个匿名内部类来实现 Comparator 。<br />下面是按姓名排序的方法：
```java
public void sortByName () { 
    Student stu1 = new Student(1, "Little"); 
    Student stu2 = new Student(2, "Cyntin"); 
    Student stu3 = new Student(3, "Tony"); 
    Student stu4 = new Student(4, "Gemini"); 

    Student[] stus = new Student[4]; 
    stus[0] = stu1; 
    stus[1] = stu4; 
    stus[2] = stu3; 
    stus[3] = stu2; 
    System.out.println("Array: " + Arrays.toString(stus)); 

    Arrays.sort(stus, new Comparator() { 

        @Override 
        public int compare(Object o1, Object o2) { 
            if (o1 instanceof Student && o2 instanceof Student) { 
                Student s1 = (Student) o1; 
                Student s2 = (Student) o2; 
                //return s1.getId() - s2.getId(); // 按Id排 
                return s1.getName().compareTo(s2.getName()); // 按姓名排 
            } 
            return 0; 
        } 

    }); 

    System.out.println("Sorted: " + Arrays.toString(stus)); 
}
```
当需要对Student按学号排序时，只需修改排序方法中实现Comparator的内部类中的代码，而不用修改 Student 类。<br />注意：当然，也可以用 Student 类实现 Comparator 接口，这样Student就是（is a）比较器了（Comparator）。当需要使用这种排序的时候，将 Student 看作 Comparator 来使用就可以了，可以将 Student 作为参数传入 sort 方法，因为 Student is a Comparator 。<br />但这样的代码不是个优秀的代码，因为之所以使用比较器（Comparator），其中有个重要的原因就是，这样可以把比较算法和具体类分离，降低类之间的耦合。<br />TreeSet对这两种比较方式都提供了支持，分别对应着TreeSet的两个构造方法：

- `TreeSet()`：根据TreeSet中元素实现的 Comparable 接口的 compareTo 方法比较排序
- `TreeSet(Comparator comparator)`：根据给定的 comparator 比较器，对 TreeSet 中的元素比较排序

当向 TreeSet 中添加元素时，TreeSet 就会对元素进行排序。至于是用自然排序还是用比较器排序，就看 TreeSet 构造是怎么写的了。当然，添加第一个元素时不会进行任何比较， TreeSet 中都没有元素，就没办法比较。<br />下面，分别给出使用两种排序比较方式的 TreeSet 测试代码：
```java
/** 
 * 使用自然排序 
 * Student必须实现Comparable接口，否则会抛出ClassCastException 
 */ 
public void testSortedSet3() { 
  Student stu1 = new Student(1, "Little"); 
  Student stu2 = new Student(2, "Cyntin"); 
  Student stu3 = new Student(3, "Tony"); 
  Student stu4 = new Student(4, "Gemini"); 

  SortedSet set = new TreeSet(); 
  set.add(stu1); 
  set.add(stu3); // 若Student没有实现Comparable接口，抛出ClassCastException 
  set.add(stu4); 
  set.add(stu2); 
  set.add(stu4); 
  set.add(new Student(12, "Little")); 

  System.out.println(set); 
} 

/** 
 * 使用比较器排序 
 * Student可以只是个简单的Java类，不用实现Comparable接口 
 */ 
public void testSortedSet3() { 
  Student stu1 = new Student(1, "Little"); 
  Student stu2 = new Student(2, "Cyntin"); 
  Student stu3 = new Student(3, "Tony"); 
  Student stu4 = new Student(4, "Gemini"); 

  SortedSet set = new TreeSet(new Comparator() { 

    @Override 
    public int compare(Object o1, Object o2) { 
      if (o1 instanceof Student 
          && o2 instanceof Student) { 
        Student s1 = (Student) o1; 
        Student s2 = (Student) o2; 
        return s1.getName().compareTo(s2.getName()); 
      } 
      return 0; 
    } 

  }); 

  set.add(stu1); 
  set.add(stu3); 
  set.add(stu4); 
  set.add(stu2); 
  set.add(stu4); 
  set.add(new Student(12, "Little")); 

  System.out.println(set); 
}
```
另外，介绍个工具类，`java.util.Collections`。注意，这不是Collection接口。Collections很像Arrays类。Arrays提供了一系列用于对数组操作的静态方法，查找排序等等。<br />Collections也提供了一系列这样的方法，只是它是用于处理集合的，虽然Collections类和Collection接口很像，但是不要被Collections的名字给欺骗了，它不是只能处理Collection接口以及子接口的实现类，同样也可以处理Map接口的实现类。
