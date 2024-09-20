Java
<a name="HX7WJ"></a>
## **Java中的Properties类**
Properties对象包含键值对，都作为字符串。java.util.Properties类是Hashtable的子类。<br />它可以根据属性键获取属性值。Properties类提供了从属性文件获取数据和将数据存储到属性文件中的方法。此外，它还可以用于获取系统的属性。
<a name="oFbZU"></a>
### properties文件的优点
如果从properties文件中更改了信息，则无需重新编译：如果从properties文件中更改了任何信息，您无需重新编译Java类。它用于存储需要经常更改的信息。
<a name="uq2iv"></a>
### **Properties类的构造函数**
| 方法 | 描述 |
| --- | --- |
| `Properties()` | 创建一个空的属性列表，没有默认值。 |
| `Properties(Properties defaults)` | 创建一个带有指定默认值的空属性列表。 |

<a name="ceN0X"></a>
### 使用Properties类从属性文件中获取信息的示例
要从属性文件中获取信息，首先创建属性文件。<br />db.properties
```
user=system 
password=oracle
```
现在，创建用于从属性文件读取数据的Java类。<br />Test.java
```java
import java.util.*; 
import java.io.*; 
public class Test { 
    public static void main(String[] args)throws Exception{ 
        FileReader reader=new FileReader("db.properties"); 
        
        Properties p=new Properties(); 
        p.load(reader); 
        
        System.out.println(p.getProperty("user")); 
        System.out.println(p.getProperty("password")); 
    } 
}
```
输出：
```
system
oracle
```
现在，如果更改了属性文件的值，无需重新编译Java类。这意味着没有维护问题。
<a name="m9LwT"></a>
### 使用Properties类获取所有系统属性的示例
通过`System.getProperties()`方法，可以获取系统的所有属性。创建一个从系统属性中获取信息的类。<br />Test.java
```java
import java.util.*; 
import java.io.*; 
public class Test { 
    public static void main(String[] args)throws Exception{ 
      
        Properties p=System.getProperties(); 
        Set set=p.entrySet(); 
        
        Iterator itr=set.iterator(); 
        while(itr.hasNext()){ 
            Map.Entry entry=(Map.Entry)itr.next(); 
            System.out.println(entry.getKey()+" = "+entry.getValue()); 
        } 
      
    } 
}
```
输出：
```
java.runtime.name = Java(TM) SE Runtime Environment
sun.boot.library.path = C:\Program Files\Java\jdk1.7.0_01\jre\bin
java.vm.version = 21.1-b02
java.vm.vendor = Oracle Corporation
java.vendor.url = http://java.oracle.com/
path.separator = ;
java.vm.name = Java HotSpot(TM) Client VM
file.encoding.pkg = sun.io
user.country = US
user.script = 
sun.java.launcher = SUN_STANDARD
...........
```
<a name="crwb1"></a>
### **使用Properties类创建属性文件的示例**
现在编写代码来创建属性文件。<br />Test.java
```java
import java.util.*; 
import java.io.*; 
public class Test { 
    public static void main(String[] args)throws Exception{ 
      
        Properties p=new Properties(); 
        p.setProperty("name","Son"); 
        p.setProperty("email","son@javatpoint.com"); 
        
        p.store(new FileWriter("info.properties"),"Javatpoint Properties Example"); 
      
    } 
}
```
看一下生成的属性文件。<br />info.properties
```
#Javatpoint Properties Example 
#Thu Oct 03 22:35:53 IST 2013 
email=son@javatpoint.com 
name=Son
```
