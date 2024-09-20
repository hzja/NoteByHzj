Java 序列化
<a name="ASfOb"></a>
### Java Object Serialization
Java对象序列化（Serialization）是指将Java中的对象转为字节流，从而可以方便的存储或在网络中传输，反序列化（Deserialization）是指将字节流转为Java对象<br />一般情况下，Java Object Serialization指的是利用JDK自带的功能对对象进行序列化/反序列化，而不是使用其他的序列化库进行（反）序列化<br />JDK 序列化中，要求对象必须实现java.io.Serializable接口，基本使用方式如下：
<a name="avVay"></a>
#### Serialization
```java
// Serialize today's date to a file.
FileOutputStream f = new FileOutputStream("tmp");
ObjectOutput s = new ObjectOutputStream(f);
s.writeObject("Today");
s.writeObject(new Date());
s.flush();
```
<a name="xcvD7"></a>
### Deserialization
```java
// Deserialize a string and date from a file.
FileInputStream in = new FileInputStream("tmp");
ObjectInputStream s = new ObjectInputStream(in);
String today = (String)s.readObject();
Date date = (Date)s.readObject();
```
<a name="lg8sk"></a>
### serialVersionUID
```java
private static final long serialVersionUID = 1L;
```
Java Object Serialization 会使用对象中的 serialVersionUID 常量属性作为该对象的版本号，进行反序列化时会校验该版本号是否一致，如果不一致会导致序列化失败，抛出`InvalidClassException`异常。<br />默认情况下，JVM 为每一个实现了 Serializable 的接口的类生成一个 `serialVersionUID`（long），这个 ID 的计算规则是通过当前类信息（类名、属性等）去生成的，所以当属性有变更时这个`serialVersionUID` 也一定会发生变更。<br />这个 `serialVersionUID` 的生成，和所使用的JDK有关，不同的JDK可能会生成不一样的版本号，所以最好是手动生成一个，大多数 JAVA IDE 都会提供这个生成的功能。<br />![2021-06-13-21-16-49-179056.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623590307993-25e3b254-c1c6-4359-9377-6b676f58e2eb.png#clientId=u84b05d72-2f98-4&from=ui&id=u5a1bd64f&originHeight=493&originWidth=709&originalType=binary&ratio=3&size=1050884&status=done&style=none&taskId=u8bf4338b-64af-4494-9da4-23b5576e6b2)<br />而且考虑到实际业务场景，变更属性是常有的事，如果使用自动生成的版本号很容易造成 `serialVersionUID` 不一致的问题，导致反序列化失败。
<a name="UGDLV"></a>
### `serialVersionUID` 不一致时的兼容处理
处理这个不一致也很简单，既然反序列化时使用 `ObjectInputStream` 来实现，那么这里自定义一个 `CompatibleInputStream` 继承 `ObjectInputStream`，然后重写 `readClassDescriptor` 方法即可<br />当遇到目标数据 Class 版本号和本地 Class 版本号不一致时，默认使用本地版本的 Class。
```java
public class CompatibleInputStream extends ObjectInputStream {
    private static Logger logger = LoggerFactory.getLogger(CompatibleInputStream.class);

    public CompatibleInputStream(InputStream in) throws IOException {
        super(in);
    }

    @Override
    protected ObjectStreamClass readClassDescriptor() throws IOException, ClassNotFoundException {
        ObjectStreamClass resultClassDescriptor = super.readClassDescriptor(); // initially streams descriptor
        Class localClass; // the class in the local JVM that this descriptor represents.
        try {
            localClass = Class.forName(resultClassDescriptor.getName()); 
        } catch (ClassNotFoundException e) {
            logger.error("No local class for " + resultClassDescriptor.getName(), e);
            return resultClassDescriptor;
        }
        ObjectStreamClass localClassDescriptor = ObjectStreamClass.lookup(localClass);
        if (localClassDescriptor != null) { // only if class implements serializable
            final long localSUID = localClassDescriptor.getSerialVersionUID();
            final long streamSUID = resultClassDescriptor.getSerialVersionUID();
            if (streamSUID != localSUID) { // check for serialVersionUID mismatch.
                final StringBuffer s = new StringBuffer("Overriding serialized class version mismatch: ");
                s.append("local serialVersionUID = ").append(localSUID);
                s.append(" stream serialVersionUID = ").append(streamSUID);
                Exception e = new InvalidClassException(s.toString());
                logger.error("Potentially Fatal Deserialization Operation.", e);
                resultClassDescriptor = localClassDescriptor; // Use local class descriptor for deserialization
            }
        }
        return resultClassDescriptor;
    }
}
```
![2021-06-13-21-16-49-339546.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1623590350649-04ceb456-81be-4601-a3a0-5eaa1752add5.png#clientId=u84b05d72-2f98-4&from=ui&id=u577913b6&originHeight=446&originWidth=1080&originalType=binary&ratio=3&size=1447902&status=done&style=none&taskId=u005d147e-b490-4fa1-a17f-73f19a0cfa6)<br />使用方式：
```java
// Deserialize a string and date from a file.
FileInputStream in = new FileInputStream("tmp");
//反序列化时使用上面的CompatibleInputStream即可
ObjectInputStream s = new CompatibleInputStream(in);
String today = (String)s.readObject();
Date date = (Date)s.readObject();
```
