JavaXStream
<a name="IuhU0"></a>
## 1、Xstream介绍
<a name="wREZp"></a>
### (1)Xstream介绍
Xstream是一种OXMapping 技术，是用来处理XML文件序列化的框架，在将JavaBean序列化，或将XML文件反序列化的时候，不需要其它辅助类和映射文件，使得XML序列化不再繁索。Xstream也可以将JavaBean序列化成Json或反序列化，使用非常方便。
<a name="Fv2NI"></a>
### (2)Xstream的简单例子
```java
class Person//JavaBean实体类
{
    private String name;
    private int age;
    public Person(String name,int age)
    {
        this.name=name;
        this.age=age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class Test{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream();
        //XML序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //XML反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);

        xstream = new XStream(new JettisonMappedXmlDriver());
        xstream.setMode(XStream.NO_REFERENCES);
        //Json序列化
        String json=xstream.toXML(bean);
        System.out.println(json);
        //Json反序列
        bean=(Person)xstream.fromXML(json);
        System.out.println(bean);
    }
}
```
程序运行结果：
```java
<test.Person>
  <name>张三</name>
  <age>19</age>
</test.Person>
Person [name=张三, age=19]
{"test.Person":{"name":"张三","age":19}}
Person [name=张三, age=19
```
**注意：**Xstream序列化XML时需要引用的jar包：xstream-[version].jar、xpp3-[version].jar、xmlpull-[version].jar。Xstream序列化Json需要引用的jar包：jettison-[version].jar。<br />使用Xstream序列化时，对JavaBean没有任何限制。JavaBean的字段可以是私有的，也可以没有getter或setter方法，还可以没有默认的构造函数。
<a name="z6jpx"></a>
## 2、Xstream的基本使用
<a name="hJ7OC"></a>
### (1)Xstream序列化XML
Xstream序列化XML时可以允许用户使用不同的XML解析器，用户可以使用一个标准的JAXP DOM解析器或自Java6集成StAX解析器。这样用户就不需要依赖xpp3-[version].jar。<br />Xstream序列化XML时，也可以对XML节点重命名。
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        //XStream xstream = new XStream();//需要XPP3库
        //XStream xstream = new XStream(new DomDriver());//不需要XPP3库
        XStream xstream = new XStream(new StaxDriver());//不需要XPP3库开始使用Java6 
        xstream.alias("人",Person.class);//为类名节点重命名
        //XML序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //XML反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<?xml version="1.0" ?><人><name>张三</name><age>19</age></人>
Person [name=张三, age=19]
```
<a name="MKybM"></a>
### (2)Xstream序列化Json
Xstream序列化Json与序列化XML类似，例如：
```java
public class Test
{
  public static void main(String[] args)
  {
    Person bean=new Person("张三",19);
    XStream xstream = new XStream(new JettisonMappedXmlDriver());//设置Json解析器
        xstream.setMode(XStream.NO_REFERENCES);//设置reference模型,不引用
    xstream.alias("人",Person.class);//为类名节点重命名
    //Json序列化
    String xml = xstream.toXML(bean);
    System.out.println(xml);
    //Json反序列化
    bean=(Person)xstream.fromXML(xml);
    System.out.println(bean);
  }
}
```
程序运行结果：
```
{"人":{"name":"张三","age":19}}
Person [name=张三, age=19]
```
<a name="FFtKX"></a>
## 3、Xstream序列化重命名
<a name="TqVwx"></a>
### (1)为包重命名：`Xstream.aliasPackage()`方法
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream();
        xstream.aliasPackage("com.lzw", "test");//为包名称重命名
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<com.lzw.Person>
  <name>张三</name>
  <age>19</age>
</com.lzw.Person>
Person [name=张三, age=19]
```
<a name="tp5Rh"></a>
### (2)为类重命名：`Xstream.alias()`方法
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream();
        xstream.alias("人", Person.class);//为类名节点重命名
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<人>
  <name>张三</name>
  <age>19</age>
</人>
Person [name=张三, age=19]
```
<a name="gGxoO"></a>
### (3)为字段重命名：`Xstream.aliasField()`方法
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream();
        xstream.aliasField("姓名", Person.class,"name");//为类的字段节点重命名
        xstream.aliasField("年龄", Person.class,"age");//为类的字段节点重命名
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<test.Person>
  <姓名>张三</姓名>
  <年龄>19</年龄>
</test.Person>
Person [name=张三, age=19]
```
<a name="XKEIK"></a>
### (4)省略集合根节点：`Xstream.addImplicitCollection()`方法
```java
class Person
{
    private String name;
    private int age;
    private List friends;
    public Person(String name, int age, String... friends)
    {
        this.name = name;
        this.age = age;
        this.friends = Arrays.asList(friends);
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", friends=" + friends + "]";
    }
}
public class Test
{
    public static void main(String[] args)
    {
        Person bean =new Person("张三",19,"李四","王五","赵六");
        XStream xstream = new XStream();
        xstream.addImplicitCollection(Person.class, "friends");//省略集合根节点
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<test.Person>
  <name>张三</name>
  <age>19</age>
  <string>李四</string>
  <string>王五</string>
  <string>赵六</string>
</test.Person>
Person [name=张三, age=19, friends=[李四, 王五, 赵六]]
```
<a name="JUeaj"></a>
### (5)把字段节点设置成属性：`Xstream.useAttributeFor()`方法
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean =new Person("张三",19,"李四","王五","赵六");
        XStream xstream = new XStream();
        xstream.useAttributeFor(Person.class, "name");//把字段节点设置成属性
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<test.Person name="张三">
  <age>19</age>
  <friends class="java.util.Arrays$ArrayList">
    <a class="string-array">
      <string>李四</string>
      <string>王五</string>
      <string>赵六</string>
    </a>
  </friends>
</test.Person>
Person [name=张三, age=19, friends=[李四, 王五, 赵六]]
```
<a name="Dh2xP"></a>
### (6)隐藏字段：`xstream.omitField()`方法
```java
public class Test
{
    public static void main(String[] args)
    {
        Person bean =new Person("张三",19,"李四","王五","赵六");
        XStream xstream = new XStream();
        xstream.omitField(Person.class, "friends");//把字段节点隐藏
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<test.Person>
  <name>张三</name>
  <age>19</age>
</test.Person>
Person [name=张三, age=19, friends=null]
```
<a name="De9YW"></a>
## 4、Xstream注解的使用
<a name="fSOpP"></a>
### (1)设置Xstream应用注解
使用Xstream注解前需要对Xstream进行配置，可以使用两种方式：应用某个JavaBean类的注解或自动使用JavaBean类的注解。代码如下：
```java
XStream xstream = new XStream();
xstream.processAnnotations(Person.class);//应用Person类的注解
xstream.autodetectAnnotations(true);//自动检测注解
```
<a name="TQkmx"></a>
### (2)重命名注解：`@XStreamAlias()`
```java
@XStreamAlias("人")
    class Person
{
    @XStreamAlias("姓名")
    private String name;
    @XStreamAlias("年龄")
    private int age;
    @XStreamAlias("朋友")
    private List friends;
    public Person(String name, int age, String... friends)
    {
        this.name = name;
        this.age = age;
        this.friends = Arrays.asList(friends);
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", friends=" + friends + "]";
    }
}
```
程序运行结果：
```
<人>
  <姓名>张三</姓名>
  <年龄>19</年龄>
  <朋友 class="java.util.Arrays$ArrayList">
    <a class="string-array">
      <string>李四</string>
      <string>王五</string>
      <string>赵六</string>
    </a>
  </朋友>
</人>
Person [name=张三, age=19, friends=[李四, 王五, 赵六]]
```
<a name="ptQyV"></a>
### (3)省略集合根节点：`@XStreamImplicit`
```java
class Person
{
    private String name;
    private int age;
    //@XStreamImplicit//只隐藏集合根节点
    @XStreamImplicit(itemFieldName="朋友")//设置重复的节点名，可能会导致无法反序列化
    private List<String> friends;
    public Person(String name, int age, String... friends)
    {
        this.name = name;
        this.age = age;
        this.friends = Arrays.asList(friends);
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", friends=" + friends + "]";
    }
}
```
程序运行结果：
```
<test.Person>
  <name>张三</name>
  <age>19</age>
  <朋友>李四</朋友>
  <朋友>王五</朋友>
  <朋友>赵六</朋友>
</test.Person>
Person [name=张三, age=19, friends=[李四, 王五, 赵六]]
```
<a name="ECuPM"></a>
### (4)把字段节点设置成属性：`@XStreamAsAttribute`
```java
class Person
{
    @XStreamAsAttribute
    private String name;
    @XStreamAsAttribute
    private int age;
    private List<String> friends;
    public Person(String name, int age, String... friends)
    {
        this.name = name;
        this.age = age;
        this.friends = Arrays.asList(friends);
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", friends=" + friends + "]";
    }
}
```
程序运行结果：
```java
<test.Person name="张三" age="19">
  <friends class="java.util.Arrays$ArrayList">
    <a class="string-array">
      <string>李四</string>
      <string>王五</string>
      <string>赵六</string>
    </a>
  </friends>
</test.Person>
Person [name=张三, age=19, friends=[李四, 王五, 赵六]]
```
<a name="g7j01"></a>
### (5)隐藏字段：`@XStreamOmitField`
```java
class Person
{
    private String name;
    private int age;
    @XStreamOmitField
    private List<String> friends;
    public Person(String name, int age, String... friends)
    {
        this.name = name;
        this.age = age;
        this.friends = Arrays.asList(friends);
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", friends=" + friends + "]";
    }
}
```
程序运行结果：
```
<test.Person>
  <name>张三</name>
  <age>19</age>
</test.Person>
Person [name=张三, age=19, friends=null]
```
<a name="GdrQM"></a>
### (6)设置转换器：`@XStreamConverter()`
```java
class Person
{
    private String name;
    private int age;
    @XStreamConverter(value=BooleanConverter.class,booleans={false},strings={"男","女"})
    private boolean sex;
    public Person(String name, int age, boolean sex)
    {
        this.name = name;
        this.age = age;
        this.sex=sex;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + ", sex=" + sex + "]";
    }
}
```
程序运行结果：
```
<test.Person>
  <name>张三</name>
  <age>19</age>
  <sex>男</sex>
</test.Person>
Person [name=张三, age=19, sex=true]
```
<a name="bkVn1"></a>
## 5、Xstream自定义的转换器
<a name="ONPnC"></a>
### (1)Xstream自带的转换器
Xstream内部有许多转换器，用于JavaBean对象到XML或Json之间的转换。这些转换器的详细信息网址：[http://xstream.codehaus.org/converters.html](http://xstream.codehaus.org/converters.html)
<a name="IhEZE"></a>
### (2)使用自定义的转换器
```java
class Person
{
    private String name;
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
    public String getName()
    {
        return name;
    }
    public void setName(String name)
    {
        this.name = name;
    }
    public int getAge()
    {
        return age;
    }
    public void setAge(int age)
    {
        this.age = age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class PersonConverter implements Converter 
{
    @Override//定义转换器能转换的JavaBean类型
    public boolean canConvert(Class type)
    {
        return type.equals(Person.class);
    }
    @Override//把对象序列化成XML或Json
    public void marshal(Object value, HierarchicalStreamWriter writer, 
                        MarshallingContext context)
    {
        Person person = (Person) value;
        writer.startNode("姓名");
        writer.setValue(person.getName());
        writer.endNode();
        writer.startNode("年龄");
        writer.setValue(person.getAge()+"");
        writer.endNode();
        writer.startNode("转换器");
        writer.setValue("自定义的转换器");
        writer.endNode();
    }
    @Override//把XML或Json反序列化成对象
    public Object unmarshal(HierarchicalStreamReader reader, 
                            UnmarshallingContext context)
    {
        Person person = new Person("",-1);
        reader.moveDown();
        person.setName(reader.getValue());
        reader.moveUp();
        reader.moveDown();
        person.setAge(Integer.parseInt(reader.getValue()));
        reader.moveUp();
        return person;
    }
}
public class Test
{
    public static void main(String[] args)
    {
        Person bean =new Person("张三",19);
        XStream xstream = new XStream();
        xstream.registerConverter(new PersonConverter());//注册转换器
        //序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
<test.Person>
  <姓名>张三</姓名>
  <年龄>19</年龄>
  <转换器>自定义的转换器</转换器>
</test.Person>
Person [name=张三, age=19]
```
<a name="Ipai3"></a>
### (3)常用的转换器接口与抽象类
`SingleValueConverter`：单值转换接口<br />`AbstractSingleValueConverter`：单值转换抽象类<br />`Converter`：常规转换器接口
<a name="b9gPU"></a>
## 6、Xstream对象流的使用
<a name="GJQT0"></a>
### (1)Xstream对象输出流
```java
class Person
{
    private String name;
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
}
public class Test
{
    public static void main(String[] args) throws IOException
    {    
        XStream xstream = new XStream();
        ObjectOutputStream out = xstream.createObjectOutputStream(System.out);
        out.writeObject(new Person("张三",12));
        out.writeObject(new Person("李四",19));
        out.writeObject("Hello");
        out.writeInt(12345);
        out.close();
    }
}
```
程序运行结果：
```
<object-stream>
  <test.Person>
    <name>张三</name>
    <age>12</age>
  </test.Person>
  <test.Person>
    <name>李四</name>
    <age>19</age>
  </test.Person>
  <string>Hello</string>
  <int>12345</int>
</object-stream>
```
**注意：**XStream对象流是通过标准java.io.ObjectOutputStream和java.io.ObjectInputStream对象。因为XML文档只能有一个根节点,必须包装在一个序列化的所有元素 额外的根节点。 这个根节点默认 < object-stream > 上面的例子所示。
<a name="nszDn"></a>
### (2)Xstream对象输出流
```java
class Person
{
    private String name;
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class Test
{
    public static void main(String[] args) throws IOException, ClassNotFoundException
    {
        String s="<object-stream><test.Person><name>张三</name><age>12</age></test.Person><int>12345</int></object-stream>";
        StringReader reader = new StringReader(s);
        XStream xstream = new XStream();
        ObjectInputStream in = xstream.createObjectInputStream(reader);
        System.out.println((Person) in.readObject());
        System.out.println(in.readInt());
    }
}
```
程序运行结果：
```
Person [name=张三, age=12]
12345
```
<a name="X6szv"></a>
## 7、Xstream持久化API
<a name="PigH8"></a>
### (1)保存JavaBean对象
```java
class Person
{
    private String name;
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class Test
{
    public static void main(String[] args) throws IOException, ClassNotFoundException
    {
        PersistenceStrategy strategy = new FilePersistenceStrategy(new File("D:\\tmp"));
        List list = new XmlArrayList(strategy);
        list.add(new Person("张三",13));//保存数据
        list.add(new Person("李四",21));
        list.add(new Person("王五",17));
    }
}
```
程序运行结果：如果检查D:\tmp目录,有三个文件:int@0.xml、int@1.xml、int@2.xml；每个对象都被序列化到XML文件里。
<a name="PQub4"></a>
### (2)读取并删除JavaBean对象
```java
public class Test
{
    public static void main(String[] args) throws IOException, ClassNotFoundException
    {
        PersistenceStrategy strategy = new FilePersistenceStrategy(new File("D:\\tmp"));
        List list = new XmlArrayList(strategy);
        for (Iterator it = list.iterator(); it.hasNext();)
            {
                System.out.println((Person) it.next());
                it.remove();//删除对象序列化文件
            }
    }
}
```
程序运行结果：
```
Person [name=张三, age=13]
Person [name=李四, age=21]
Person [name=王五, age=17]
```
<a name="hegeN"></a>
## 8、Xstream操作Json
<a name="zXLUz"></a>
### (1)Xstream序列化Json的重命名
```java

@XStreamAlias("人")
    class Person
{
    @XStreamAlias("姓名")
    private String name;
    @XStreamAlias("年龄")
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class Test
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream(new JettisonMappedXmlDriver());//设置Json解析器
        xstream.autodetectAnnotations(true);
        //Json序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
        //Json反序列化
        bean=(Person)xstream.fromXML(xml);
        System.out.println(bean);
    }
}
```
程序运行结果：
```
{"人":{"姓名":"张三","年龄":19}}
Person [name=张三, age=19]
```
**注意：**Xstream序列化Json的重命名的方式与其序列化成XML的方式一样！
<a name="oD3MV"></a>
### (2)去掉序列化Json的根节点
```java
class Person
{
    private String name;
    private int age;
    public Person(String name, int age)
    {
        this.name = name;
        this.age = age;
    }
    @Override
    public String toString()
    {
        return "Person [name=" + name + ", age=" + age + "]";
    }
}
public class Test00
{
    public static void main(String[] args)
    {
        Person bean=new Person("张三",19);
        XStream xstream = new XStream(new JsonHierarchicalStreamDriver()
                                      {
                                          public HierarchicalStreamWriter createWriter(Writer writer)
                                          {
                                              return new JsonWriter(writer, JsonWriter.DROP_ROOT_MODE);
                                          }
                                      });
        //Json序列化
        String xml = xstream.toXML(bean);
        System.out.println(xml);
    }
}
```
程序运行结果：
```
{ "name": "张三", "age": 19 }
```
**注意：**去掉根节点后的Json串是不能反序列化的，因为XStream 不知道它的类型。
<a name="TVk28"></a>
### (3)Json的解析器区别
前面两个例子使用了不同的Json解析器，这里说明他们的不同之处：

   1. `JettisonMappedXmlDriver`：是支持序列化和反序列化Json的。
   2. `JsonHierarchicalStreamDriver`：只支持序列化，不支持反序列化。
