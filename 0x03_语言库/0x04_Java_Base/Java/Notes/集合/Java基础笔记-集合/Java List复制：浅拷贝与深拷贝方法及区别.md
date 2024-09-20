Java List
<a name="nHYLm"></a>
### List浅拷贝
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1639645747978-bab41ca9-c897-4356-ad6c-aad0c8c2a87a.jpeg)<br />众所周知，list本质上是数组，而数组的是以地址的形式进行存储。<br />如上图将list A浅拷贝给list B，由于进行的是浅拷贝，所以直接将A的内容复制给了B，java中相同内容的数组指向同一地址，即进行浅拷贝后A与B指向同一地址。造成的后果就是，改变B的同时也会改变A，因为改变B就是改变B所指向地址的内容，由于A也指向同一地址，所以A与B一起改变。
<a name="mFibK"></a>
### 几种浅拷贝
<a name="RZYa7"></a>
#### 1、遍历循环复制
```java
List<Person> destList=new ArrayList<Person>(srcList.size());  
for(Person p : srcList){  
    destList.add(p);  
}
```
<a name="r8uQK"></a>
#### 2、使用List实现类的构造方法
```java
List<Person> destList=new ArrayList<Person>(srcList);
```
<a name="AHaqn"></a>
#### 3、使用`list.addAll()`方法
```java
List<Person> destList=new ArrayList<Person>();  
destList.addAll(srcList);
```
<a name="Cr5xl"></a>
#### 4、使用`System.arraycopy()`方法
```java
Person[] srcPersons=srcList.toArray(new Person[0]);  
Person[] destPersons=new Person[srcPersons.length];  
System.arraycopy(srcPersons, 0, destPersons, 0, srcPersons.length);
```
测试及结果
```java
printList(destList); //打印未改变B之前的A 
srcList.get(0).setAge(100);//改变B  
printList(destList); //打印改变B后的A
```
打印结果
```

123-->20  
ABC-->21  
abc-->22  
123-->100  
ABC-->21  
abc-->22  
```
<a name="jt2J2"></a>
### List深拷贝
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1639645748994-814ae47f-b44d-49f0-941c-ecf14912c411.jpeg)<br />如图，深拷贝就是将A复制给B的同时，给B创建新的地址，再将地址A的内容传递到地址B。ListA与ListB内容一致，但是由于所指向的地址不同，所以改变相互不受影响。
<a name="Qm9bJ"></a>
### 深拷贝的方法
<a name="DX412"></a>
#### 1.使用序列化方法
```java
public static <T> List<T> deepCopy(List<T> src) throws IOException, ClassNotFoundException {  
    ByteArrayOutputStream byteOut = new ByteArrayOutputStream();  
    ObjectOutputStream out = new ObjectOutputStream(byteOut);  
    out.writeObject(src);  

    ByteArrayInputStream byteIn = new ByteArrayInputStream(byteOut.toByteArray());  
    ObjectInputStream in = new ObjectInputStream(byteIn);  
    @SuppressWarnings("unchecked")  
    List<T> dest = (List<T>) in.readObject();  
    return dest;  
}  

List<Person> destList=deepCopy(srcList);  //调用该方法
```
<a name="oE8tc"></a>
#### 2.`clone`方法
```java
public class A implements Cloneable {   
    public String name[];   

    public A(){   
        name=new String[2];   
    }   

    public Object clone() {   
        A o = null;   
        try {   
            o = (A) super.clone();   
        } catch (CloneNotSupportedException e) {   
            e.printStackTrace();   
        }   
        return o;   
    }   
}  
for(int i=0;i<n;i+=){
    copy.add((A)src.get(i).clone());
}
```
Java对对象和基本的数据类型的处理是不一样的。在Java中用对象的作为入口参数的传递则缺省为”引用传递”，也就是说仅仅传递了对象的一个”引用”，这个”引用”的概念同C语言中的指针引用是一样的。当函数体内部对输入变量改变时，实质上就是在对这个对象的直接操作。<br />除了在函数传值的时候是”引用传递”，在任何用”＝”向对象变量赋值的时候都是”引用传递”。<br />测试及结果
```java
printList(destList); //打印未改变B之前的A 
srcList.get(0).setAge(100);//改变B  
printList(destList); //打印改变B后的A
```
```
123-->20  
ABC-->21  
abc-->22  
123-->20  
ABC-->21  
abc-->22  
```
在浅复制的情况下，源数据被修改破坏之后，使用相同引用指向该数据的目标集合中的对应元素也就发生了相同的变化。因此，在需求要求必须深复制的情况下，要是使用上面提到的方法，请确保List中的T类对象是不易被外部修改和破坏的。
