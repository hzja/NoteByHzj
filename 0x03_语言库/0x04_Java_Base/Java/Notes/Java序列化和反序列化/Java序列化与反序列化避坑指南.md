Java 序列化和反序列化
<a name="bGUSw"></a>
#### 1、序列化与反序列化的概念
先说说序列化和反序列化的概念

- 序列化：将对象写入到IO流中
- 反序列化：从IO流中恢复对象

`Serializable`接口是一个标记接口，不用实现任何方法，标记当前类对象是可以序列化的，是给JVM看的。<br />序列化机制允许将这些实现序列化接口的对象转化为字节序列，这些字节序列可以保证在磁盘上或者网络传输后恢复成原来的对象。序列化就是把对象存储在JVM以外的地方，序列化机制可以让对象脱离程序的运行而独立存在。<br />序列化在业务代码也许用的不多，但是在框架层面用的是很多的。<br />相关技术：Session的序列化或者反序列化<br />先给出序列化的例子，请记住这个People类，后面会根据这个类来改造讲解。
```java
public class People {  

    private Long id;  

    public People(Long id) {  
        this.id = id;  
    }  

    public Long getId() {  
        return id;  
    }  

    public void setId(Long id) {  
        this.id = id;  
    }  

    @Override  
    public String toString() {  
        return "People{" +  
            "id=" + id +  
            '}';  
    }  
}
```
```java
import java.io.*;  
  
// 屏蔽编译器的警告  
@SuppressWarnings("all")  
public class Main {  
  
    /**  
     * <h1>序列化和反序列化 People 对象</h1>  
     */  
    private static void testSerializablePeople() throws Exception {  
  
        // 序列化的步骤  
  
        // 用于存储序列化的文件，这里的java_下划线仅仅为了说明是java序列化对象，没有任何其他含义  
        File file = new File("/tmp/people_10.java_");  
        if (!file.exists()) {  
            // 1，先得到文件的上级目录，并创建上级目录  
            file.getParentFile().mkdirs();  
            try {  
                // 2，再创建文件  
                file.createNewFile();  
            } catch (IOException e) {  
                e.printStackTrace();  
            }  
        }  
        People p = new People(10L);  
  
        // 创建一个输出流  
        ObjectOutputStream oos = new ObjectOutputStream(  
                new FileOutputStream(file)  
        );  
        // 输出可序列化对象  
        oos.writeObject(p);  
        // 关闭输出流  
        oos.close();  
  
        // 反序列化的步骤  
  
        // 创建一个输入流  
        ObjectInputStream ois = new ObjectInputStream(  
                new FileInputStream(file)  
        );  
        // 得到反序列化的对象，这里可以强转为People类型  
        Object newPerson = ois.readObject();  
        // 关闭输入流  
        ois.close();  
  
        System.out.println(newPerson);  
    }  
  
    public static void main(String[] args) throws Exception {  
        testSerializablePeople();  
    }  
}  
```
运行之后，看到磁盘文件因为序列化而多了一个文件<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836450-81d4db68-bc8d-4c4e-8228-0b16bd99290b.webp#clientId=u1c316b14-d5fc-4&from=paste&id=udf9dab27&originHeight=124&originWidth=371&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u384bbcb4-2959-4478-94a3-f18f7e94fe7&title=)<br />控制台中因反序列化输出的对象信息打印如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836501-9782ad28-5d66-47d4-a0d0-b3caa797043d.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u374caf0e&originHeight=112&originWidth=407&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4b95c1e-1217-49e0-82b2-fb3c83e139e&title=)
<a name="NHfk9"></a>
#### 2、子类实现`Serializable`接口，父类没有实现，子类可以序列化吗？
去掉父类People的`implements Serializable`，让父类不实现序列化接口，子类Worker实现序列化接口
```java
public class Worker extends People implements Serializable {  

    private String name;  
    private Integer age;  

    public Worker(Long id, String name, Integer age) {  
        super(id);  
        this.name = name;  
        this.age = age;  
    }  

}  
```
```java
public static void main(String[] args) throws Exception {  
    testSerizableWorker();  
}  

/**  
     * <h2>子类实现序列化, 父类不实现序列化</h2>  
     * */  
private static void testSerizableWorker() throws Exception {  

    File file = new File("/tmp/worker_10.java_");  
    if (!file.exists()) {  
        // 1，先得到文件的上级目录，并创建上级目录  
        file.getParentFile().mkdirs();  
        try {  
            // 2，再创建文件  
            file.createNewFile();  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
    }  
    Worker p = new Worker(10L, "lcy", 18);  

    // 创建一个输出流  
    ObjectOutputStream oos = new ObjectOutputStream(  
        new FileOutputStream(file)  
    );  
    // 输出可序列化对象  
    oos.writeObject(p);  
    // 关闭输出流  
    oos.close();  

    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file));  
    Object newWorker = ois.readObject(); // 父类没有序列化的时候，需要调用父类的无参数构造方法  
    ois.close();  
    System.out.println(newWorker);  
}  
```
再次测试运行<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836507-e55b0ed0-2322-4c09-9431-3873a2ff526b.webp#clientId=u1c316b14-d5fc-4&from=paste&id=ue2e57259&originHeight=301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud4da788a-cf35-4637-9f72-6c1e11069dd&title=)<br />结果显示没有有效地构造器，原来是因为父类没有序列化的时候，`Object newWorker = ois.readObject()`需要直接调用父类的无参数构造方法，不经过子类的无参构造方法。<br />在父类People中加上空的构造方法之后再次执行<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836422-c1cad59c-d7f1-4362-ba60-6312c9bfaded.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u08bd75d8&originHeight=121&originWidth=416&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u87c184bc-87d4-465d-b5c0-6853f191a82&title=)<br />结果却发现打印的不是Worker，而是父类People，因为子类没有实现`toString`而调用父类的`toString`，所以打印了People对象，至于父类成员变量id为什么是null，原因如下：<br />1、一个子类实现了 `Serializable` 接口，它的父类都没有实现 `Serializable`接口，序列化该子类对象。要想反序列化后输出父类定义的某变量的数值，就需要让父类也实现`Serializable`接口或者父类有默认的无参的构造函数。<br />2、在父类没有实现`Serializable` 接口时，虚拟机是不会序列化父对象的，而一个 Java对象的构造必须先有父对象，才有子对象，反序列化也不例外。所以反序列化时，为了构造父对象，只能调用父类的无参构造函数作为默认的父对象。因此当取父对象的变量值时，它的值是调用父类无参构造函数后的值，如果在父类无参构造函数中没有对变量赋值，那么父类成员变量值都是默认值，如这里的Long型就是null。<br />3、根据以上特性，可以将不需要被序列化的字段抽取出来放到父类中，子类实现 `Serializable`接口，父类不实现`Serializable`接口但提供一个空构造方法，则父类的字段数据将不被序列化。<br />最后加上子类Worker的`toString`方法，打印结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836507-8f42c3e2-4373-44e1-91cc-ffbbc6032a01.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u6c595425&originHeight=117&originWidth=402&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u155c3698-9e0a-46c6-a446-f3cc54886f2&title=)<br />总结：

- 子类实现`Serializable`接口，父类没有实现，子类可以序列化！！
- 这种情况父类一定要提供空构造方法，不要忘了子类的toString方法！
<a name="RSwwM"></a>
#### 3、类中存在引用对象，这个类对象在什么情况下可以实现序列化？
来一个组合对象，里面引用People对象，此时People对象没有实现`Serializable`接口，能否序列化呢？代码给出来，可以自行复制测试一下。
```java
public class Combo implements Serializable {  
  
    private int id;  
    private People people;  
  
    public Combo(int id, People people) {  
        this.id = id;  
        this.people = people;  
    }  
  
    public int getId() {  
        return id;  
    }  
  
    public void setId(int id) {  
        this.id = id;  
    }  
  
    public People getPeople() {  
        return people;  
    }  
  
    public void setPeople(People people) {  
        this.people = people;  
    }  
      
    @Override  
    public String toString() {  
        return "Combo{" +  
                "id=" + id +  
                ", people=" + people +  
                '}';  
    }  
}  
```
```java
public class People {  

    private Long id;  

    public People() {  
    }  

    public People(Long id) {  
        this.id = id;  
    }  

    public Long getId() {  
        return id;  
    }  

    public void setId(Long id) {  
        this.id = id;  
    }  

    @Override  
    public String toString() {  
        return "People{" +  
            "id=" + id +  
            '}';  
    }  
}  
```
```java
private static void testSerializableCombo() throws Exception {  

    File file = new File("/tmp/combo_10.java_");  
    if (!file.exists()) {  
        // 1，先得到文件的上级目录，并创建上级目录  
        file.getParentFile().mkdirs();  
        try {  
            // 2，再创建文件  
            file.createNewFile();  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
    }  
    Combo p = new Combo(1, new People(10L));  

    // 创建一个输出流  
    ObjectOutputStream oos = new ObjectOutputStream(  
        new FileOutputStream(file)  
    );  
    // 输出可序列化对象  
    oos.writeObject(p);  
    // 关闭输出流  
    oos.close();  

    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file));  
    Object newCombo = ois.readObject();  
    ois.close();  
    System.out.println(newCombo);  
}  

public static void main(String[] args) throws Exception {  
    testSerializableCombo();  
}  
```
运行结果如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546837155-6e904b57-96ac-4843-b132-83246c483ff6.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u923c6170&originHeight=354&originWidth=954&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc6748397-9188-4131-9c37-ef3502d519e&title=)<br />直接爆出异常，说明People类没有序列化。<br />当People加上`implements Serializable`实现序列化接口后，再次执行如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836985-58ef3cfa-c57d-4cb6-8fdc-6998f9e1983a.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u8c8e7ff6&originHeight=130&originWidth=408&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud703c6cc-64a4-47b1-8b51-8d5f9e09926&title=)<br />总结：

- 一个类里面所有的属性必须是可序列化的，这个类才能顺利的序列化。

比如，类中存在引用对象，那么这个引用对象必须是可序列化的，这个类才能序列化。
<a name="bax8S"></a>
#### 4、同一个对象多次序列化之间有属性更新，前后的序列化有什么区别？
下面例子中People是可序列化的，每次序列化之前都会把People的id值修改了，用来观察看看，多次序列化期间，如果对象属性更新，是否会影响序列化，反序列化有什么区别。
```java
/**  
 * <h2>同一个对象多次序列化的问题, 坑</h2>  
 * */  
private static void sameObjectRepeatedSerialization() throws Exception {  
  
    File file = new File("/tmp/peopele_more.java_");  
    if (!file.exists()) {  
        // 1，先得到文件的上级目录，并创建上级目录  
        file.getParentFile().mkdirs();  
        try {  
            // 2，再创建文件  
            file.createNewFile();  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
    }  
    People p = new People(10L);  
    ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(file));  
    // 未序列化，先修改属性  
    p.setId(11L);  
    oos.writeObject(p);  
    // 序列化一次后，再次修改属性  
    p.setId(15L);  
    oos.writeObject(p);  
    // 序列化两次后，再次修改属性  
    p.setId(20L);  
    oos.writeObject(p);  
    oos.close();  
  
    ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file));  
    Object people1 = ois.readObject();  
    Object people2 = ois.readObject();  
    Object people3 = ois.readObject();  
    ois.close();  
  
    System.out.println(((People) people1).getId());  
    System.out.println(((People) people2).getId());  
    System.out.println(((People) people3).getId());  
}  
  
  
public static void main(String[] args) throws Exception {  
    sameObjectRepeatedSerialization();  
}  
```
运行结果如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546836986-e1cee1cd-c4cf-4b8c-8eb1-1c64c0f7799a.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u71c361e5&originHeight=172&originWidth=401&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2f0a8188-c052-474f-afa7-a2206663b20&title=)<br />结果发现反序列化读出的值都是一样的。说明当对象第一次序列化成功后，后续这个对象属性即使有修改，也不会对后面的序列化造成成影响。<br />这其实是序列化算法的原因，所有要序列化的对象都有一个序列化的编码号，当试图序列化一个对象，会检查这个对象是否已经序列化过，若从未序列化过，才会序列化为字节序列去输出。若已经序列化过，则会输出一个编码符号，不会重复序列化一个对象。如下<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638546837131-36c9bcaa-a727-4ca4-ac29-7db7902a3adc.webp#clientId=u1c316b14-d5fc-4&from=paste&id=u54d34aa9&originHeight=182&originWidth=736&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u27c83349-da83-4e4b-a228-1854e928164&title=)<br />序列化一次后，后续继续序列化并未重复转换为字节序列，而是输出字符q~<br />总结：

- 当第一次序列化之后，不管如何修改这个对象的属性，都不会对后续的序列化产生影响，反序列化的结果都和第一次相同。
