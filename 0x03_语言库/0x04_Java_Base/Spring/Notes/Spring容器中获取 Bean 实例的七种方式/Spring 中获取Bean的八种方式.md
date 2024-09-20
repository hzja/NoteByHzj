JavaSpringBoot<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1669340361567-cb5e3c7d-9575-407a-b9e1-2c4d2e4b4052.png#averageHue=%23f9f2dc&clientId=u249e9f8d-022b-4&from=paste&id=ud78207b8&originHeight=488&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u103581c2-38e0-45c0-8a39-a8bdbcc0ea0&title=)
<a name="NaUOW"></a>
### 1、在初始化时保存`ApplicationContext`对象
适用于Spring框架的独立应用程序，须要程序通过配置文件初始化Spring。<br />applicationContext.xml配置：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:p="http://www.springframework.org/schema/p"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
  <bean id="test" class="com.sxtx.bean.Test">
  </bean>

</beans>
```
代码：
```java
@Test
public void test() {
    ApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
    //ApplicationContext applicationContext = new FileSystemXmlApplicationContext("applicationContext.xml"); 
    Test test= (Test) applicationContext.getBean("test");
    System.out.println(test);
}
```
<a name="CpuPd"></a>
### 2、通过Spring提供的工具类获取`ApplicationContext`对象
适合于Spring框架的B/S系统，通过`ServletContext`对象获取`ApplicationContext`对象。然后在通过它获取须要的类实例。以下两个工具方式的差别是，前者在获取失败时抛出异常。后者返回null。
```java
ApplicationContext ac1 = WebApplicationContextUtils.getRequiredWebApplicationContext(ServletContext sc); 
ApplicationContext ac2 = WebApplicationContextUtils.getWebApplicationContext(ServletContext sc); 
ac1.getBean("beanId"); 
ac2.getBean("beanId");
```
<a name="BPU8n"></a>
### 3、实现接口`ApplicationContextAware`（推荐）
实现该接口的`setApplicationContext(ApplicationContext context)`方法，并保存`ApplicationContext` 对象。Spring初始化时，扫描到该类，就会通过该方法将`ApplicationContext`对象注入。然后在代码中就可以获取spring容器bean了。<br />例如：
```java
User bean = SpringUtils.getBean("user");
```
```java
@Component
public class SpringUtils implements ApplicationContextAware {
 
    private static ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        SpringUtils.applicationContext = applicationContext;
    }
 
    public static <T> T getBean(String beanName) {
        if(applicationContext.containsBean(beanName)){
            return (T) applicationContext.getBean(beanName);
        }else{
            return null;
        }
    }
 
    public static <T> Map<String, T> getBeansOfType(Class<T> baseType){
        return applicationContext.getBeansOfType(baseType);
    }
}
```
<a name="Xk7MI"></a>
### 4、继承自抽象类`ApplicationObjectSupport`
调用父类的`getApplicationContext()`方法，获取Spring容器对象。
```java
@Service
public class SpringContextHelper extends ApplicationObjectSupport {

    public Object getBean(String beanName) {
        return getApplicationContext().getBean(beanName);
    }
}
```
<a name="S1JOD"></a>
### 5、继承自抽象类`WebApplicationObjectSupport`
调用`getWebApplicationContext()`获取`WebApplicationContext`
```java
@Service
public class SpringContextHelper extends WebApplicationObjectSupport {

    public Object getBean(String beanName) {
        return getApplicationContext().getBean(beanName);
    }
}
```
<a name="sBVXt"></a>
### 6、使用`BeanFactory`直接获取（不推荐）
使用`BeanFactory`从工厂中直接获取Bean实例，但是`XmlBeanFactory`类已经废弃，因此不建议使用。
```java
@Test
public void test() {
    BeanFactory beanFactory = new XmlBeanFactory(new ClassPathResource("applicationContext.xml"));
    Test test= (Test) beanFactory.getBean("test");
    System.out.println(test);
}
```
<a name="UXsgV"></a>
### 7、使用`ContextLoader`提供的`getCurrentWebApplicationContext`方法
```java
@Test
public void test() {
    MockServletContext sc = new MockServletContext("");
    sc.addInitParameter(ContextLoader.CONFIG_LOCATION_PARAM, "/applicationContext.xml");
    ServletContextListener listener = new ContextLoaderListener();
    ServletContextEvent event = new ServletContextEvent(sc);
    listener.contextInitialized(event);
    
    WebApplicationContext wac = ContextLoader.getCurrentWebApplicationContext();
    Test test= (Test) wac.getBean("test");
    System.out.println(test);
}
```
<a name="Ipeiq"></a>
### 8、实现接口`BeanFactoryPostProcessor`
Spring工具类 方便在非Spring管理环境中获取bean
```java
@Component
public final class SpringUtilsS implements BeanFactoryPostProcessor
{
    /** Spring应用上下文环境 */
    private static ConfigurableListableBeanFactory beanFactory;

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException
    {
        SpringUtilsS.beanFactory = beanFactory;
    }

    /**
     * 获取对象
     *
     * @param name
     * @return Object 一个以所给名字注册的bean的实例
     * @throws BeansException
     *
     */
    @SuppressWarnings("unchecked")
    public static <T> T getBean(String name) throws BeansException
    {
        return (T) beanFactory.getBean(name);
    }

    /**
     * 获取类型为requiredType的对象
     *
     * @param clz
     * @return
     * @throws BeansException
     *
     */
    public static <T> T getBean(Class<T> clz) throws BeansException
    {
        T result = (T) beanFactory.getBean(clz);
        return result;
    }

    /**
     * 如果BeanFactory包含一个与所给名称匹配的bean定义，则返回true
     *
     * @param name
     * @return boolean
     */
    public static boolean containsBean(String name)
    {
        return beanFactory.containsBean(name);
    }

    /**
     * 判断以给定名字注册的bean定义是一个singleton还是一个prototype。 如果与给定名字相应的bean定义没有被找到，将会抛出一个异常（NoSuchBeanDefinitionException）
     *
     * @param name
     * @return boolean
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static boolean isSingleton(String name) throws NoSuchBeanDefinitionException
    {
        return beanFactory.isSingleton(name);
    }

    /**
     * @param name
     * @return Class 注册对象的类型
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static Class<?> getType(String name) throws NoSuchBeanDefinitionException
    {
        return beanFactory.getType(name);
    }

    /**
     * 如果给定的bean名字在bean定义中有别名，则返回这些别名
     *
     * @param name
     * @return
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static String[] getAliases(String name) throws NoSuchBeanDefinitionException
    {
        return beanFactory.getAliases(name);
    }

    /**
     * 获取aop代理对象
     * 
     * @param invoker
     * @return
     */
    @SuppressWarnings("unchecked")
    public static <T> T getAopProxy(T invoker)
    {
        return (T) AopContext.currentProxy();
    }
}
```
<a name="jNbEe"></a>
### 扩展
`BeanFactory`和`ApplicationContext`是Spring的两大核心接口，都可以当做Spring的容器。其中`ApplicationContext`是`BeanFactory`的子接口。
<a name="opHUM"></a>
#### `BeanFactory`
（1）、是Spring里面最底层的接口（最原始的接口），包含了各种Bean的定义，读取bean配置文档，管理bean的加载、实例化，控制bean的生命周期，维护bean之间的依赖关系。<br />（2）、采用的是延迟加载形式来注入Bean的，即只有在使用到某个Bean时(调用`getBean()`)，才对该Bean进行加载实例化。这样，就不能发现一些存在的Spring的配置问题。如果Bean的某一个属性没有注入，`BeanFacotry`加载后，直至第一次使用调用getBean方法才会抛出异常。<br />（3）`BeanFactory`通常以编程的方式被创建。<br />（4）`BeanFactory`和`ApplicationContext`都支持`BeanPostProcessor`、`BeanFactoryPostProcessor`的使用，但两者之间的区别是：`BeanFactory`需要手动注册，而`ApplicationContext`则是自动注册。<br />（5） 占用内存小。
<a name="EvNSB"></a>
#### `ApplicationContext`
1、`ApplicationContext`接口作为`BeanFactory`的派生，除了提供`BeanFactory`所具有的功能外，还提供了更完整的框架功能：

- 继承`MessageSource`，因此支持国际化。
- 统一的资源文件访问方式。
- 提供在监听器中注册bean的事件。
- 同时加载多个配置文件。
- 载入多个（有继承关系）上下文 ，使得每一个上下文都专注于一个特定的层次，比如应用的web层。

2、`ApplicationContext`，它是在容器启动时，一次性创建了所有的Bean。这样，在容器启动时，就可以发现Spring中存在的配置错误，这样有利于检查所依赖属性是否注入。`ApplicationContext`启动后预载入所有的单实例Bean，通过预载入单实例bean ，确保当需要的时候，就不用等待，因为它们已经创建好了。<br />3、`ApplicationContext` 占用内存空间大，当程序的配置bean特别多时，程序启动慢。<br />4、`ApplicationContext` 能以编程式方式创建，还能能以声明的方式创建，如使用`ContextLoader`。
