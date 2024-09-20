简单的业务使用 if-else 或 switch case，确实直观明了，但是一旦功能复杂，业务流程长了之后，使用传统的 if-else 或 switch case 方式，就会让代码结构变的异常冗余、混乱，过一段时间之后，哪怕是自己写的代码，读起来也会忍不住想喷上几句，这时候，如果能合理的使用设计模式，就能使这种混乱的局面变的比较优雅；<br />物流行业中，通常会涉及到EDI报文(XML格式文件)传输和回执接收，每发送一份EDI报文，后续都会收到与之关联的回执（标识该数据在第三方系统中的流转状态）。<br />这里枚举几种回执类型：MT1101、MT2101、MT4101、MT8104、MT8105、MT9999，系统在收到不同的回执报文后，会执行对应的业务逻辑处理。当然，实际业务场景并没有那么笼统，这里以回执处理为演示案例。<br />模拟一个回执类
```java
@Data  
public class Receipt {  

    /**  
     * 回执信息  
     */  
    String message;  

    /**  
     * 回执类型(`MT1101、MT2101、MT4101、MT8104、MT8105、MT9999`)  
     */  
    String type;  

}
```
模拟一个回执生成器
```java
public class ReceiptBuilder {  

    public static List<Receipt> generateReceiptList(){  
        //直接模拟一堆回执对象  
        List<Receipt> receiptList = new ArrayList<>();  
        receiptList.add(new Receipt("我是MT2101回执喔","MT2101"));  
        receiptList.add(new Receipt("我是MT1101回执喔","MT1101"));  
        receiptList.add(new Receipt("我是MT8104回执喔","MT8104"));  
        receiptList.add(new Receipt("我是MT9999回执喔","MT9999"));  
        //......  
        return receiptList;  
    }  
}
```
<a name="DyupT"></a>
## 1、传统做法-if-else分支
```java
List<Receipt> receiptList = ReceiptBuilder.generateReceiptList();  
//循环处理  
for (Receipt receipt : receiptList) {  
    if (StringUtils.equals("MT2101",receipt.getType())) {  
        System.out.println("接收到MT2101回执");  
        System.out.println("解析回执内容");  
        System.out.println("执行业务逻辑");  
    } else if (StringUtils.equals("MT1101",receipt.getType())) {  
        System.out.println("接收到MT1101回执");  
        System.out.println("解析回执内容");  
        System.out.println("执行业务逻辑");  
    } else if (StringUtils.equals("MT8104",receipt.getType())) {  
        System.out.println("接收到MT8104回执");  
        System.out.println("解析回执内容");  
        System.out.println("执行业务逻辑");  
    } else if (StringUtils.equals("MT9999",receipt.getType())) {  
        System.out.println("接收到MT9999回执");  
        System.out.println("解析回执内容");  
        System.out.println("执行业务逻辑");  
        System.out.println("推送邮件");  
    }  
    // ......未来可能还有好多个else if  
}
```
在遇到if-else的分支业务逻辑比较复杂时，都习惯于将其抽出一个方法或者封装成一个对象去调用，这样整个if-else结构就不会显得太臃肿。<br />就上面例子，当回执的类型越来越多时，分支else if 就会越来越多，每增加一个回执类型，就需要修改或添加if-else分支，违反了开闭原则（对扩展开放，对修改关闭）。
<a name="aOMtY"></a>
## 2、策略模式+Map字典
策略模式的目的是封装一系列的算法，它们具有共性，可以相互替换，也就是说让算法独立于使用它的客户端而独立变化，客户端仅仅依赖于策略接口 。<br />在上述场景中，可以把if-else分支的业务逻辑抽取为各种策略，但是不可避免的是依然需要客户端写一些if-else进行策略选择的逻辑，可以将这段逻辑抽取到工厂类中去，这就是策略模式+简单工厂，代码如下。<br />策略接口
```java
/**  
 * @Description: 回执处理策略接口
 */  
public interface IReceiptHandleStrategy {  

    void handleReceipt(Receipt receipt);  

}
```
策略接口实现类，也就是具体的处理者
```java
public class Mt2101ReceiptHandleStrategy implements IReceiptHandleStrategy {  

    @Override  
    public void handleReceipt(Receipt receipt) {  
        System.out.println("解析报文MT2101:" + receipt.getMessage());  
    }  

}  

public class Mt1101ReceiptHandleStrategy implements IReceiptHandleStrategy {  

    @Override  
    public void handleReceipt(Receipt receipt) {  
        System.out.println("解析报文MT1101:" + receipt.getMessage());  
    }  

}  

public class Mt8104ReceiptHandleStrategy implements IReceiptHandleStrategy {  

    @Override  
    public void handleReceipt(Receipt receipt) {  
        System.out.println("解析报文MT8104:" + receipt.getMessage());  
    }  

}  

public class Mt9999ReceiptHandleStrategy implements IReceiptHandleStrategy {  

    @Override  
    public void handleReceipt(Receipt receipt) {  
        System.out.println("解析报文MT9999:" + receipt.getMessage());  
    }  

}
```
策略上下文类(策略接口的持有者)
```java
/**  
 * @Description: 上下文类，持有策略接口
 */  
public class ReceiptStrategyContext {  

    private IReceiptHandleStrategy receiptHandleStrategy;  

    /**  
     * 设置策略接口  
     * @param receiptHandleStrategy  
     */  
    public void setReceiptHandleStrategy(IReceiptHandleStrategy receiptHandleStrategy) {  
        this.receiptHandleStrategy = receiptHandleStrategy;  
    }  

    public void handleReceipt(Receipt receipt){  
        if (receiptHandleStrategy != null) {  
            receiptHandleStrategy.handleReceipt(receipt);  
        }  
    }  
}
```
策略工厂
```java
/**  
 * @Description: 策略工厂
 */  
public class ReceiptHandleStrategyFactory {  

    private ReceiptHandleStrategyFactory(){}  

    public static IReceiptHandleStrategy getReceiptHandleStrategy(String receiptType){  
        IReceiptHandleStrategy receiptHandleStrategy = null;  
        if (StringUtils.equals("MT2101",receiptType)) {  
            receiptHandleStrategy = new Mt2101ReceiptHandleStrategy();  
        } else if (StringUtils.equals("MT8104",receiptType)) {  
            receiptHandleStrategy = new Mt8104ReceiptHandleStrategy();  
        }  
        return receiptHandleStrategy;  
    }  
}
```
客户端
```java
public class Client {  

    public static void main(String[] args) {  
        //模拟回执  
        List<Receipt> receiptList = ReceiptBuilder.generateReceiptList();  
        //策略上下文  
        ReceiptStrategyContext receiptStrategyContext = new ReceiptStrategyContext();  
        for (Receipt receipt : receiptList) {  
            //获取并设置策略  
            IReceiptHandleStrategy receiptHandleStrategy = ReceiptHandleStrategyFactory.getReceiptHandleStrategy(receipt.getType());  
            receiptStrategyContext.setReceiptHandleStrategy(receiptHandleStrategy);  
            //执行策略  
            receiptStrategyContext.handleReceipt(receipt);  
        }  
    }  
}
```
解析报文MT2101:我是MT2101回执报文喔 <br />解析报文MT8104:我是MT8104回执报文喔。<br />由于目的是消除if-else，那么这里需要将`ReceiptHandleStrategyFactory`策略工厂进行改造下，采用字典的方式存放策略，而Map具备key-value结构，采用Map是个不错选择。<br />稍微改造下，代码如下
```java
/**  
 * @Description: 策略工厂 
 */  
public class ReceiptHandleStrategyFactory {  

    private static Map<String,IReceiptHandleStrategy> receiptHandleStrategyMap;  

    private ReceiptHandleStrategyFactory(){  
        this.receiptHandleStrategyMap = new HashMap<>();  
        this.receiptHandleStrategyMap.put("MT2101",new Mt2101ReceiptHandleStrategy());  
        this.receiptHandleStrategyMap.put("MT8104",new Mt8104ReceiptHandleStrategy());  
    }  

    public static IReceiptHandleStrategy getReceiptHandleStrategy(String receiptType){  
        return receiptHandleStrategyMap.get(receiptType);  
    }  
}
```
经过对策略模式+简单工厂方案的改造，已经消除了if-else的结构，每当新来了一种回执，只需要添加新的回执处理策略，并修改`ReceiptHandleStrategyFactory`中的Map集合。<br />如果要使得程序符合开闭原则，则需要调整`ReceiptHandleStrategyFactory`中处理策略的获取方式，通过反射的方式，获取指定包下的所有`IReceiptHandleStrategy`实现类，然后放到字典Map中去。
<a name="orE45"></a>
## 3、责任链模式
责任链模式是一种对象的行为模式。在责任链模式里，很多对象由每一个对象对其下家的引用而连接起来形成一条链。请求在这个链上传递，直到链上的某一个对象决定处理此请求。<br />发出这个请求的客户端并不知道链上的哪一个对象最终处理这个请求，这使得系统可以在不影响客户端的情况下动态地重新组织和分配责任<br />回执处理者接口
```java
/**  
 * @Description: 抽象回执处理者接口
 */  
public interface IReceiptHandler {  

    void handleReceipt(Receipt receipt,IReceiptHandleChain handleChain);  

}
```
责任链接口
```java
/**  
 * @Description: 责任链接口
 */  
public interface IReceiptHandleChain {  

    void handleReceipt(Receipt receipt);  
}
```
责任链接口实现类
```java
/**  
 * @Description: 责任链实现类
 */  
public class ReceiptHandleChain implements IReceiptHandleChain {  
    //记录当前处理者位置  
    private int index = 0;  
    //处理者集合  
    private static List<IReceiptHandler> receiptHandlerList;  

    static {  
        //从容器中获取处理器对象  
        receiptHandlerList = ReceiptHandlerContainer.getReceiptHandlerList();  
    }  

    @Override  
    public void handleReceipt(Receipt receipt) {  
        if (receiptHandlerList !=null && receiptHandlerList.size() > 0) {  
            if (index != receiptHandlerList.size()) {  
                IReceiptHandler receiptHandler = receiptHandlerList.get(index++);  
                receiptHandler.handleReceipt(receipt,this);  
            }  
        }  
    }  
}
```
具体回执处理者
```java
public class Mt2101ReceiptHandler implements IReceiptHandler {  

    @Override  
    public void handleReceipt(Receipt receipt, IReceiptHandleChain handleChain) {  
        if (StringUtils.equals("MT2101",receipt.getType())) {  
            System.out.println("解析报文MT2101:" + receipt.getMessage());  
        }  
            //处理不了该回执就往下传递  
        else {  
            handleChain.handleReceipt(receipt);  
        }  
    }  
}  

public class Mt8104ReceiptHandler implements IReceiptHandler {  

    @Override  
    public void handleReceipt(Receipt receipt, IReceiptHandleChain handleChain) {  
        if (StringUtils.equals("MT8104",receipt.getType())) {  
            System.out.println("解析报文MT8104:" + receipt.getMessage());  
        }  
            //处理不了该回执就往下传递  
        else {  
            handleChain.handleReceipt(receipt);  
        }  
    }  
}
```
责任链处理者容器(如果采用Spring，则可以通过依赖注入的方式获取到`IReceiptHandler`的子类对象)
```java
/**  
 * @Description: 处理者容器
 */  
public class ReceiptHandlerContainer {  

    private ReceiptHandlerContainer(){}  

    public static List<IReceiptHandler> getReceiptHandlerList(){  
        List<IReceiptHandler> receiptHandlerList = new ArrayList<>();  
        receiptHandlerList.add(new Mt2101ReceiptHandler());  
        receiptHandlerList.add(new Mt8104ReceiptHandler());  
        return receiptHandlerList;  
    }  

}
```
客户端
```java
public class Client {  

    public static void main(String[] args) {  
        //模拟回执  
        List<Receipt> receiptList = ReceiptBuilder.generateReceiptList();  
        for (Receipt receipt : receiptList) {  
            //回执处理链对象  
            ReceiptHandleChain receiptHandleChain = new ReceiptHandleChain();  
            receiptHandleChain.handleReceipt(receipt);  
        }  
    }  
}
```
解析报文MT2101:我是MT2101回执报文喔<br />解析报文MT8104:我是MT8104回执报文喔。<br />通过责任链的处理方式，if-else结构也被消除了，每当新来了一种回执，只需要添加`IReceiptHandler`实现类并修改`ReceiptHandlerContainer`处理者容器即可，如果要使得程序符合开闭原则，则需要调整`ReceiptHandlerContainer`中处理者的获取方式，通过反射的方式，获取指定包下的所有`IReceiptHandler`实现类。<br />这里使用到了一个反射工具类，用于获取指定接口的所有实现类
```java
/**  
 * @Description: 反射工具类
 */  
public class ReflectionUtil {  

    /**  
     * 定义类集合（用于存放所有加载的类）  
     */  
    private static final Set<Class<?>> CLASS_SET;  

    static {  
        //指定加载包路径  
        CLASS_SET = getClassSet("com.yaolong");  
    }  

    /**  
     * 获取类加载器  
     * @return  
     */  
    public static ClassLoader getClassLoader(){  
        return Thread.currentThread().getContextClassLoader();  
    }  

    /**  
     * 加载类  
     * @param className 类全限定名称  
     * @param isInitialized 是否在加载完成后执行静态代码块  
     * @return  
     */  
    public static Class<?> loadClass(String className,boolean isInitialized) {  
        Class<?> cls;  
        try {  
            cls = Class.forName(className,isInitialized,getClassLoader());  
        } catch (ClassNotFoundException e) {  
            throw new RuntimeException(e);  
        }  
        return cls;  
    }  

    public static Class<?> loadClass(String className) {  
        return loadClass(className,true);  
    }  

    /**  
     * 获取指定包下所有类  
     * @param packageName  
     * @return  
     */  
    public static Set<Class<?>> getClassSet(String packageName) {  
        Set<Class<?>> classSet = new HashSet<>();  
        try {  
            Enumeration<URL> urls = getClassLoader().getResources(packageName.replace(".","/"));  
            while (urls.hasMoreElements()) {  
                URL url = urls.nextElement();  
                if (url != null) {  
                    String protocol = url.getProtocol();  
                    if (protocol.equals("file")) {  
                        String packagePath = url.getPath().replace("%20","");  
                        addClass(classSet,packagePath,packageName);  
                    } else if (protocol.equals("jar")) {  
                        JarURLConnection jarURLConnection = (JarURLConnection) url.openConnection();  
                        if (jarURLConnection != null) {  
                            JarFile jarFile = jarURLConnection.getJarFile();  
                            if (jarFile != null) {  
                                Enumeration<JarEntry> jarEntries = jarFile.entries();  
                                while (jarEntries.hasMoreElements()) {  
                                    JarEntry jarEntry = jarEntries.nextElement();  
                                    String jarEntryName = jarEntry.getName();  
                                    if (jarEntryName.endsWith(".class")) {  
                                        String className = jarEntryName.substring(0, jarEntryName.lastIndexOf(".")).replaceAll("/", ".");  
                                        doAddClass(classSet,className);  
                                    }  
                                }  
                            }  
                        }  
                    }  
                }  
            }  


        } catch (IOException e) {  
            throw new RuntimeException(e);  
        }  
        return classSet;  
    }  
  
    private static void doAddClass(Set<Class<?>> classSet, String className) {  
        Class<?> cls = loadClass(className,false);  
        classSet.add(cls);  
    }  
  
    private static void addClass(Set<Class<?>> classSet, String packagePath, String packageName) {  
        final File[] files = new File(packagePath).listFiles(new FileFilter() {  
            @Override  
            public boolean accept(File file) {  
                return (file.isFile() && file.getName().endsWith(".class")) || file.isDirectory();  
            }  
        });  
        for (File file : files) {  
            String fileName = file.getName();  
            if (file.isFile()) {  
                String className = fileName.substring(0, fileName.lastIndexOf("."));  
                if (StringUtils.isNotEmpty(packageName)) {  
                    className = packageName + "." + className;  
                }  
                doAddClass(classSet,className);  
            } else {  
                String subPackagePath = fileName;  
                if (StringUtils.isNotEmpty(packagePath)) {  
                    subPackagePath = packagePath + "/" + subPackagePath;  
                }  
                String subPackageName = fileName;  
                if (StringUtils.isNotEmpty(packageName)) {  
                    subPackageName = packageName + "." + subPackageName;  
                }  
                addClass(classSet,subPackagePath,subPackageName);  
            }  
        }  
    }  
  
  
    public static Set<Class<?>> getClassSet() {  
        return CLASS_SET;  
    }  
  
    /**  
     * 获取应用包名下某父类（或接口）的所有子类（或实现类）  
     * @param superClass  
     * @return  
     */  
    public static Set<Class<?>> getClassSetBySuper(Class<?> superClass) {  
        Set<Class<?>> classSet = new HashSet<>();  
        for (Class<?> cls : CLASS_SET) {  
            if (superClass.isAssignableFrom(cls) && !superClass.equals(cls)) {  
                classSet.add(cls);  
            }  
        }  
        return classSet;  
    }  
  
    /**  
     * 获取应用包名下带有某注解的类  
     * @param annotationClass  
     * @return  
     */  
    public static Set<Class<?>> getClassSetByAnnotation(Class<? extends Annotation> annotationClass) {  
        Set<Class<?>> classSet = new HashSet<>();  
        for (Class<?> cls : CLASS_SET) {  
            if (cls.isAnnotationPresent(annotationClass)) {  
                classSet.add(cls);  
            }  
        }  
        return classSet;  
    }  
  
}
```
接下来改造`ReceiptHandlerContainer`
```java
public class ReceiptHandlerContainer {  

    private ReceiptHandlerContainer(){}  

    public static List<IReceiptHandler> getReceiptHandlerList(){  
        List<IReceiptHandler> receiptHandlerList = new ArrayList<>();  
        //获取IReceiptHandler接口的实现类  
        Set<Class<?>> classList = ReflectionUtil.getClassSetBySuper(IReceiptHandler.class);  
        if (classList != null && classList.size() > 0) {  
            for (Class<?> clazz : classList) {  
                try {  
                    receiptHandlerList.add((IReceiptHandler)clazz.newInstance());  
                } catch ( Exception e) {  
                    e.printStackTrace();  
                }  
            }  
        }  
        return receiptHandlerList;  
    }  

}
```
至此，该方案完美符合了开闭原则，如果新增一个回执类型，只需要添加一个新的回执处理器即可，无需做其它改动。如新加了MT6666的回执，代码如下
```
public class Mt6666ReceiptHandler implements IReceiptHandler {  
  
    @Override  
    public void handleReceipt(Receipt receipt, IReceiptHandleChain handleChain) {  
        if (StringUtils.equals("MT6666",receipt.getType())) {  
            System.out.println("解析报文MT6666:" + receipt.getMessage());  
        }  
        //处理不了该回执就往下传递  
        else {  
            handleChain.handleReceipt(receipt);  
        }  
    }  
}
```
<a name="r06H3"></a>
## 4、策略模式+注解
此方案其实和上述没有太大异同，为了能符合开闭原则，通过自定义注解的方式，标记处理者类，然后反射获取到该类集合，放到Map容器中，这里不再赘述。
<a name="wIFYR"></a>
## 5、小结
if-else 或 switch case 这种分支判断的方式对于分支逻辑不多的简单业务，还是直观高效的。对于业务复杂，分支逻辑多，采用适当的模式技巧，会让代码更加清晰，容易维护，但同时类或方法数量也是倍增的。**「需要对业务做好充分分析，避免一上来就设计模式，避免过度设计！」**
