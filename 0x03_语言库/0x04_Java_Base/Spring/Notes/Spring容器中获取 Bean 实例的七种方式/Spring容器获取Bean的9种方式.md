JavaSpring
<a name="KKiU4"></a>
## 前言
随着SpringBoot的普及，Spring的使用也越来越广，在某些场景下，无法通过注解或配置的形式直接获取到某个Bean。比如，在某一些工具类、设计模式实现中需要使用到Spring容器管理的Bean，此时就需要直接获取到对应的Bean。<br />这里整理汇总了常见的获取Bean的方式，并提供一些优劣分析，方便大家在使用到时有更好的选择。同时，也会为大家适当的普及和拓展一些相关知识。
<a name="HVRHF"></a>
## Spring的IoC容器
在Spring中，Bean的实例化、定位、配置应用程序中的对象及建立对象间的依赖关系，都是在IoC容器中进行的。因此，要在Spring中获取Bean，本质上就是从IoC容器当中获取Bean。<br />在Spring中，BeanFactory是IoC容器的实际代表者，该接口提供了IoC容器最基本功能。同时，Spring还提供了另外一种类型的容器：`ApplicationContext`容器。<br />`ApplicationContext`容器包括`BeanFactory`容器的所有功能（BeanFactory的子接口），提供了更多面向应用的功能，它提供了国际化支持和框架事件体系，更易于创建实际应用。<br />一般情况，称`BeanFactory`为IoC容器，称`ApplicationContext`为应用上下文。但有时为了方便，也将`ApplicationContext`称为Spring容器。<br />通常不建议使用`BeanFactory`，但`BeanFactory` 仍然可以用于轻量级的应用程序，如移动设备或基于applet的应用程序，其中它的数据量和速度是显著。
<a name="M4RI1"></a>
### `BeanFactory`与`ApplicationContext`的区别
BeanFactory是Spring框架的基础设施，面向Spring本身。`ApplicationContext`则面向使用Spring框架的开发者，几乎所有的应用场合都可以直接使用`ApplicationContext`，而非底层的BeanFactory。<br />另外，`ApplicationContext`的初始化和`BeanFactory`有一个重大的区别：<br />`BeanFactory`在初始化容器时，并未实例化Bean，直到第一次访问某个Bean时才实例目标Bean。这样，就不能发现一些存在的Spring的配置问题。如果Bean的某一个属性没有注入，BeanFacotry加载后，直至第一次使用调用getBean方法才会抛出异常。<br />而`ApplicationContext`则在初始化应用上下文时就实例化所有单实例的Bean，相对应的，`ApplicationContext`的初始化时间会比`BeanFactory`长一些。<br />了解了上述的基本理论知识之后，就可以尝试从IoC容器当中获取Bean对象了。
<a name="NPLct"></a>
## 方式一：通过`BeanFactory`获取
通过`BeanFactory`来获取Bean。基于xml配置文件的时代，可以通过如下方式获得`BeanFactory`，再通过`BeanFactory`来获得对应的Bean。
```java
BeanFactory beanFactory = new XmlBeanFactory(new ClassPathResource("applicationContext.xml"));
UserInfo userInfo = (UserInfo) beanFactory.getBean("userInfo");
```
有一定编程年龄的程序员，应该对此还有一些印象。这种写法估计也只会出现在古老的项目当中。鉴于xml形式配置文件已经被基于注解形式所替代，同时`XmlBeanFactory`也被标注为废弃。此种方式不推荐使用。<br />其实，不推荐的理由还有一个，在上面已经提到，尽量不要使用`BeanFactory`，而应该使用`ApplicationContext`。
<a name="Yyjnb"></a>
## 方式二：通过`BeanFactoryAware`获取
在上面的方式中，`XmlBeanFactory`已经被废弃，但可以通过其他方式来获得`BeanFactory`，然后再从`BeanFactory`中获得指定的Bean。获取`BeanFactory`实例最简单的方式就是实现`BeanFactoryAware`接口。<br />`BeanFactoryAware`接口源码：
```java
public interface BeanFactoryAware extends Aware {

    /**
     * 初始化回调方法，Spring会自动将BeanFactory注入进去，接收之后即可使用BeanFactory
     */
    void setBeanFactory(BeanFactory beanFactory) throws BeansException;
}
```
`BeanFactoryAware`属于`org.springframework.beans.factory.Aware`根标记接口，使用setter注入来在应用程序上下文启动期间获取对象。Aware接口是回调，监听器和观察者设计模式的混合，它表示Bean有资格通过回调方式被Spring容器通知。<br />这里提供一个完整的工具类：
```java
@Component
public class BeanFactoryHelper implements BeanFactoryAware {

    private static BeanFactory beanFactory;

    /**
    * 重写 BeanFactoryAware 接口的方法
    * @param beanFactory ：参数赋值给本地属性之后即可使用 BeanFactory
    * @throws BeansException BeansException
    */
    @Override
    public void setBeanFactory(BeanFactory beanFactory) throws BeansException {
        BeanFactoryHelper.beanFactory = beanFactory;
    }
    /**
    * 根据名称获取容器中的对象实例
    * @param beanName ：注入的实例必须已经存在容器中，否则抛异常：NoSuchBeanDefinitionException
    * @return Object
    */
    public static Object getBean(String beanName) {
        return beanFactory.getBean(beanName);
    }
    /**
    * 根据 class 获取容器中的对象实例
    * @param requiredType ：被注入的必须已经存在容器中，否则抛异常：NoSuchBeanDefinitionException
    * @param <T> Class
    * @return 对象
    */
    public static <T> T getBean(Class<T> requiredType) {
        return beanFactory.getBean(requiredType);
    }
    /**
    * 判断 spring 容器中是否包含指定名称的对象
    * @param beanName bean名称
    * @return 是否存在
    */
    public static boolean containsBean(String beanName) {
        return beanFactory.containsBean(beanName);
    }
    //其它需求皆可参考 BeanFactory 接口和它的实现类
}
```
在上述工具类中，便是基于`BeanFactoryAware`的特性，获得了`BeanFactory`，然后再通过`BeanFactory`来获得指定的Bean。<br />该方案满足了获取Bean的基本需求，但同时具有使用`BeanFactory`的缺点。根据前文介绍的BeanFactory特性，可酌情使用。<br />上面提供了两种基于`BeanFactory`容器获得Bean的方式，下面则通过`ApplicationContext`来获取容器中的Bean，不同的是获取`ApplicationContext`的方式的区别。
<a name="GNnhQ"></a>
## 方式三：启动获取`ApplicationContext`
在项目启动时先获取`ApplicationContext`对象，然后将其存储在一个地方，以便后续用到时进行使用。<br />这里提供两种场景的获取：

- 基于xml配置bean的形式，适用于比较古老的项目，已经很少使用了；
- 基于SpringBoot启动时获取`ApplicationContext`对象；

基于xml的形式实现：
```java
// 其中applicationContext.xml 为配置容器的xml，不过现在一般很少使用了
ApplicationContext ac = new FileSystemXmlApplicationContext("applicationContext.xml");
```
这里等于直接初始化容器，并且获得容器的引用。这种方式适用于采用Spring框架的独立应用程序，需要程序通过配置文件手工初始化Spring的情况。目前大多数Spring项目已经不再采用xml配置，很少使用了。<br />基于SpringBoot启动实现：
```java
@SpringBootApplication
public class ExampleApplication {

    public static void main(String[] args) {
        // 启动时，保存上下文，并保存为静态
        ConfigurableApplicationContext ac = SpringApplication.run(ExampleApplication.class, args);
        SpringContextUtil.setAc(ac);
    }
}
```
对应的`SpringContextUtil`类如下：
```java
public class SpringContextUtil1 {

    private static ApplicationContext ac;

    public static <T>  T getBean(String beanName, Class<T> clazz) {
        T bean = ac.getBean(beanName, clazz);
        return bean;
    }

    public static void setAc(ApplicationContext applicationContext){
        ac = applicationContext;
    }
}
```
两种方式都是在启动Spring项目时，直接获取到`ApplicationContext`的引用，然后将其存储到工具类当中。在使用时，则从工具类中获取`ApplicationContext`容器，进而从中获得Bean对象。
<a name="ZYZog"></a>
## 方式四：通过继承`ApplicationObjectSupport`
此种方式依旧是先获得`ApplicationContext`容器，然后从中获取Bean对象，只不过是基于继承`ApplicationObjectSupport`类实现的。<br />具体实现代码：
```java
@Component
public class SpringContextUtil extends ApplicationObjectSupport {
    public <T> T getBean(Class<T> clazz) {
        ApplicationContext ac = getApplicationContext();
        if(ac == null){
            return null;
        }
        return ac.getBean(clazz);
    }
}
```
注意，这里的`SpringContextUtil`类需要实例化。
<a name="jmXPb"></a>
## 方式五：通过继承`WebApplicationObjectSupport`
`WebApplicationObjectSupport`是`ApplicationObjectSupport`的一个实现类，提供了Web相关的支持。实现原理与`ApplicationObjectSupport`一样。<br />具体实现代码如下：
```java
@Component
public class SpringContextUtil extends WebApplicationObjectSupport {
    public <T> T getBean(Class<T> clazz) {
        ApplicationContext ac = getApplicationContext();
        if(ac == null){
            return null;
        }
        return ac.getBean(clazz);
    }
}
```
对照基于`ApplicationObjectSupport`的实现，除了继承对象不同外，没有其他区别，都是基于`getApplicationContext`方法来获取。
<a name="qZgD5"></a>
## 方式六：通过`WebApplicationContextUtils`
Spring提供了工具类`WebApplicationContextUtils`，通过该类可获取`WebApplicationContext`对象。<br />具体实现代码如下：
```java
public class SpringContextUtil2 {
    public static <T> T getBean(ServletContext request, String name, Class<T> clazz){
        WebApplicationContext webApplicationContext = WebApplicationContextUtils.getRequiredWebApplicationContext(request);
        // 或者
        WebApplicationContext webApplicationContext1 = WebApplicationContextUtils.getWebApplicationContext(request);
        //        webApplicationContext1.getBean(name, clazz)
        T bean = webApplicationContext.getBean(name, clazz);
        return bean;
    }
}
```
这个方法很常见于SpringMVC构建的Web项目中，适用于Web项目的B/S结构。
<a name="snWor"></a>
## 方式七：通过`ApplicationContextAware`
通过实现`ApplicationContextAware`接口，在Spring容器启动时将`ApplicationContext`注入进去，从而获取`ApplicationContext`对象，这种方法也是常见的获取Bean的一种方式，推荐使用。<br />具体实现代码如下：
```java
@Component
public class SpringContextUtil3 implements ApplicationContextAware {

    private static ApplicationContext ac;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        ac = applicationContext;
    }

    public static <T> T getBean(Class<T> clazz) {
        T bean = ac.getBean(clazz);
        return bean;
    }

}
```
这种方式与前面通过`BeanFactoryAware`获得`BeanFactory`的思路一致。
<a name="EzQYx"></a>
## 方式八：通过`ContextLoader`
使用`ContextLoader`提供的`getCurrentWebApplicationContext`方法，也是常用的获取`WebApplicationContext`的一种方法。<br />具体实现代码如下：
```java
WebApplicationContext wac = ContextLoader.getCurrentWebApplicationContext();
wac.getBean(beanID);
```
该方法常见于SpringMVC实现的Web项目中。该方式是一种不依赖于Servlet，不需要注入的方式。但是需要注意一点，在服务器启动时和Spring容器初始化时，不能通过该方法获取Spring容器。
<a name="OUzj7"></a>
## 方式九：通过`BeanFactoryPostProcessor`
Spring工具类，方便在非Spring管理环境中获取Bean。
```java
@Component
public final class SpringUtils implements BeanFactoryPostProcessor{
    
    /** Spring应用上下文环境 */
    private static ConfigurableListableBeanFactory beanFactory;

    @Override
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException{
        SpringUtilsS.beanFactory = beanFactory;
    }

    /**
     * 获取对象
     *
     * @param name
     * @return Object 一个以所给名字注册的bean的实例
     * @throws BeansException
     *
     */
    @SuppressWarnings("unchecked")
    public static <T> T getBean(String name) throws BeansException{
        return (T) beanFactory.getBean(name);
    }

    /**
     * 获取类型为requiredType的对象
     *
     * @param clz
     * @return
     * @throws BeansException
     *
     */
    public static <T> T getBean(Class<T> clz) throws BeansException{
        T result = (T) beanFactory.getBean(clz);
        return result;
    }

    /**
     * 如果BeanFactory包含一个与所给名称匹配的bean定义，则返回true
     *
     * @param name
     * @return boolean
     */
    public static boolean containsBean(String name){
        return beanFactory.containsBean(name);
    }

    /**
     * 判断以给定名字注册的bean定义是一个singleton还是一个prototype。 如果与给定名字相应的bean定义没有被找到，将会抛出一个异常（NoSuchBeanDefinitionException）
     *
     * @param name
     * @return boolean
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static boolean isSingleton(String name) throws NoSuchBeanDefinitionException{
        return beanFactory.isSingleton(name);
    }

    /**
     * @param name
     * @return Class 注册对象的类型
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static Class<?> getType(String name) throws NoSuchBeanDefinitionException{
        return beanFactory.getType(name);
    }

    /**
     * 如果给定的bean名字在bean定义中有别名，则返回这些别名
     *
     * @param name
     * @return
     * @throws NoSuchBeanDefinitionException
     *
     */
    public static String[] getAliases(String name) throws NoSuchBeanDefinitionException{
        return beanFactory.getAliases(name);
    }

    /**
     * 获取aop代理对象
     * 
     * @param invoker
     * @return
     */
    @SuppressWarnings("unchecked")
    public static <T> T getAopProxy(T invoker){
        return (T) AopContext.currentProxy();
    }
}
```
其中`ConfigurableListableBeanFactory`接口，也属于`BeanFactory`的子接口。
<a name="A5efp"></a>
## 小结
在本文中介绍了9种从Spring容器中获取Bean的方法，虽然每种方式实现各有不同，但从本质上来讲，无非就是通过`BeanFactory`或`ApplicationContext`获取Bean，只不过获取`BeanFactory`或`ApplicationContext`容器的方式不同而已。<br />那么，是否意识到，学习一项技术或一个实现方式，只要把握住它的根本，无论形式如何变化，都万变不离其宗。而这里“宗”就是IoC容器。
