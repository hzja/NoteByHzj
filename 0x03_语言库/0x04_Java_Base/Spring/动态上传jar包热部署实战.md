Java<br />近期开发系统过程中遇到的一个需求，系统给定一个接口，用户可以自定义开发该接口的实现，并将实现打成jar包，上传到系统中。系统完成热部署，并切换该接口的实现。
<a name="r8p3K"></a>
### 定义简单的接口
这里以一个简单的计算器功能为例，接口定义比较简单，直接上代码。
```java
public interface Calculator {
    int calculate(int a, int b);
    int add(int a, int b);
}
```
<a name="WOcYN"></a>
### 该接口的一个简单的实现
考虑到用户实现接口的两种方式，使用Spring上下文管理的方式，或者不依赖Spring管理的方式，这里称它们为注解方式和反射方式。`calculate`方法对应注解方式，add方法对应反射方式。计算器接口实现类的代码如下：
```java
@Service
public class CalculatorImpl implements Calculator {
    @Autowired
    CalculatorCore calculatorCore;
    /**
     * 注解方式
     */
    @Override
    public int calculate(int a, int b) {
        int c = calculatorCore.add(a, b);
        return c;
    }
    /**
     * 反射方式
     */
    @Override
    public int add(int a, int b) {
        return new CalculatorCore().add(a, b);
    }
}
```
这里注入`CalculatorCore`的目的是为了验证在注解模式下，系统可以完整的构造出bean的依赖体系，并注册到当前Spring容器中。`CalculatorCore`的代码如下：
```java
@Service
public class CalculatorCore {
    public int add(int a, int b) {
        return a+b;
    }
}
```
<a name="lmaFY"></a>
### 反射方式热部署
用户把jar包上传到系统的指定目录下，这里定义上传jar文件路径为jarAddress，jar的Url路径为jarPath。
```java
private static String jarAddress = "E:/zzq/IDEA_WS/CalculatorTest/lib/Calculator.jar";
private static String jarPath = "file:/" + jarAddress;
```
并且可以要求用户填写jar包中接口实现类的完整类名。接下来系统要把上传的jar包加载到当前线程的类加载器中，然后通过完整类名，加载得到该实现的Class对象。然后反射调用即可，完整代码：
```java
/**
 * 热加载Calculator接口的实现 反射方式
 */
public static void hotDeployWithReflect() throws Exception {
    URLClassLoader urlClassLoader = new URLClassLoader(new URL[]{new URL(jarPath)}, Thread.currentThread().getContextClassLoader());
    Class clazz = urlClassLoader.loadClass("com.nci.cetc15.calculator.impl.CalculatorImpl");
    Calculator calculator = (Calculator) clazz.newInstance();
    int result = calculator.add(1, 2);
    System.out.println(result);
}
```
<a name="KQaLw"></a>
### 注解方式热部署
如果用户上传的jar包含了Spring的上下文，那么就需要扫描jar包里的所有需要注入Spring容器的bean，注册到当前系统的Spring容器中。其实，这就是一个类的热加载+动态注册的过程。<br />直接上代码：
```java
/**
 * 加入jar包后 动态注册bean到spring容器，包括bean的依赖
 */
public static void hotDeployWithSpring() throws Exception {
	Set<String> classNameSet = DeployUtils.readJarFile(jarAddress);
	URLClassLoader urlClassLoader = new URLClassLoader(new URL[]{new URL(jarPath)}, Thread.currentThread().getContextClassLoader());
	for (String className : classNameSet) {
		Class clazz = urlClassLoader.loadClass(className);
		if (DeployUtils.isSpringBeanClass(clazz)) {
			BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder.genericBeanDefinition(clazz);
			defaultListableBeanFactory.registerBeanDefinition(DeployUtils.transformName(className), beanDefinitionBuilder.getBeanDefinition());
		}
	}
}
```
在这个过程中，将jar加载到当前线程类加载器的过程和之前反射方式是一样的。然后扫描jar包下所有的类文件，获取到完整类名，并使用当前线程类加载器加载出该类名对应的class对象。判断该class对象是否带有Spring的注解，如果包含，则将该对象注册到系统的Spring容器中。<br />DeployUtils包含读取jar包所有类文件的方法、判断class对象是否包含sping注解的方法、获取注册对象对象名的方法。代码如下：
```java
/**
 * 读取jar包中所有类文件
 */
public static Set<String> readJarFile(String jarAddress) throws IOException {
    Set<String> classNameSet = new HashSet<>();
    JarFile jarFile = new JarFile(jarAddress);
    Enumeration<JarEntry> entries = jarFile.entries();//遍历整个jar文件
    while (entries.hasMoreElements()) {
        JarEntry jarEntry = entries.nextElement();
        String name = jarEntry.getName();
        if (name.endsWith(".class")) {
            String className = name.replace(".class", "").replaceAll("/", ".");
            classNameSet.add(className);
        }
    }
    return classNameSet;
}
```
```java
/**
 * 方法描述 判断class对象是否带有spring的注解
 */
public static boolean isSpringBeanClass(Class<?> cla) {
    if (cla == null) {
        return false;
    }
    //是否是接口
    if (cla.isInterface()) {
        return false;
    }
    //是否是抽象类
    if (Modifier.isAbstract(cla.getModifiers())) {
        return false;
    }
    if (cla.getAnnotation(Component.class) != null) {
        return true;
    }
    if (cla.getAnnotation(Repository.class) != null) {
        return true;
    }
    if (cla.getAnnotation(Service.class) != null) {
        return true;
    }
    return false;
}
```
```java
/**
 * 类名首字母小写 作为spring容器beanMap的key
 */
public static String transformName(String className) {
    String tmpstr = className.substring(className.lastIndexOf(".") + 1);
    return tmpstr.substring(0, 1).toLowerCase() + tmpstr.substring(1);
}
```
<a name="tbfoM"></a>
### 删除jar时，需要同时删除Spring容器中注册的bean
在jar包切换或删除时，需要将之前注册到Spring容器的bean删除。Spring容器的bean的删除操作和注册操作是相逆的过程，这里要注意使用同一个Spring上下文。<br />代码如下：
```java
/**
 * 删除jar包时 需要在spring容器删除注入
 */
public static void delete() throws Exception {
	Set<String> classNameSet = DeployUtils.readJarFile(jarAddress);
	URLClassLoader urlClassLoader = new URLClassLoader(new URL[]{new URL(jarPath)}, Thread.currentThread().getContextClassLoader());
	for (String className : classNameSet) {
		Class clazz = urlClassLoader.loadClass(className);
		if (DeployUtils.isSpringBeanClass(clazz)) {
			defaultListableBeanFactory.removeBeanDefinition(DeployUtils.transformName(className));
		}
	}
}
```
<a name="ayI6d"></a>
### 测试
测试类手动模拟用户上传jar的功能。测试函数写了个死循环，一开始没有找到jar会抛出异常，捕获该异常并睡眠10秒。这时候可以把jar手动放到指定的目录下。<br />代码如下：
```java
ApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
DefaultListableBeanFactory defaultListableBeanFactory = (DefaultListableBeanFactory) applicationContext.getAutowireCapableBeanFactory();
while (true) {
	try {
		hotDeployWithReflect();
		//            hotDeployWithSpring();
		//            delete();
	} catch (Exception e) {
		e.printStackTrace();
		Thread.sleep(1000 * 10);
	}
}
```
