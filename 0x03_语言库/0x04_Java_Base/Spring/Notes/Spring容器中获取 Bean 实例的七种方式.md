JavaSpring
<a name="yVObJ"></a>
## 实现方式
这里一共提供七种实现方式：
<a name="cnt86"></a>
### 一、使用`BeanFactory`直接获取（不推荐）
使用`BeanFactory`从工厂中直接获取Bean实例，但是`XmlBeanFactory`类已经废弃，因此不建议使用，测试代码如下：
```java
/**
 * 方式一：XmlBeanFactory已经废弃不建议使用
 */
@Test
public void getBeanTest1() {
    BeanFactory beanFactory = new XmlBeanFactory(new ClassPathResource("applicationContext.xml"));
    UserInfo userInfo = (UserInfo) beanFactory.getBean("userInfo");
    System.out.println(userInfo);
}
```
<a name="mkoTX"></a>
### 二、在初始化时保存`ApplicationContext`对象
可以在初始化的时候保存`ApplicationContext`对象，然后通过这个对象获取Bean，测试代码如下：
```java
/**
 * 方式二：使用ClassPathXmlApplicationContext获取ApplicationContext
 */
@Test
public void getBeanTest2() {
    ApplicationContext applicationContext = new ClassPathXmlApplicationContext("applicationContext.xml");
    UserInfo userInfo = (UserInfo) applicationContext.getBean("userInfo");
    System.out.println(userInfo);
}
```
<a name="R2d4z"></a>
### 三、继承自抽象类`ApplicationObjectSupport`
可以继承抽象类`ApplicationObjectSupport`并将自己继承的类注入到Spring容器中，示例代码如下：
```java
/**
 * 方法三：继承ApplicationObjectSupport来获取ApplicationContext，
 * 注意：需要把自己继承的类注入到Spring
 */
@Test
public void getBeanTest3() {
    ApplicationContextUtil2 applicationContextUtil2 = (ApplicationContextUtil2) ApplicationContextUtil.getBean("applicationContextUtil2");
    UserInfo userInfo = (UserInfo) applicationContextUtil2.getBean("userInfo");
    System.out.println(userInfo);
}
```
其中`ApplicationContextUtil2`的代码如下所示：
```java
public class ApplicationContextUtil2 extends ApplicationObjectSupport {

    /**
     * 通过bean的id获取bean对象
     * @param beanName
     * @return
     */
    public Object getBean(String beanName){
        return super.getApplicationContext().getBean(beanName);
    }

}
```
最后别忘了将Bean注入到Spring容器中，通过注解，或者配置均可，本示例通过配置实现
```xml
<!-- 测试获取bean的方式,继承ApplicationObjectSupport需要先注入这个类 -->
<bean id="applicationContextUtil2" class="com.leo.util.ApplicationContextUtil2"></bean>
```
<a name="TwCiM"></a>
### 四、继承自抽象类`WebApplicationObjectSupport`
可以继承抽象类`WebApplicationObjectSupport`并将自己继承的类注入到Spring容器中，示例代码如下：
```java
/**
 * 方法四：继承WebApplicationObjectSupport来获取ApplicationContext，
 * 注意：需要把自己继承的类注入到Spring，同时需要添加@WebAppConfiguration注解，否则会找不到web容器
 */
@Test
public void getBeanTest4() {
    ApplicationContextUtil3 applicationContextUtil3 = (ApplicationContextUtil3) ApplicationContextUtil.getBean("applicationContextUtil3");
    UserInfo userInfo = (UserInfo) applicationContextUtil3.getBean("userInfo");
    System.out.println(userInfo);
}
```
其中`ApplicationContextUtil3` 的示例代码如下：
```java
public class ApplicationContextUtil3 extends WebApplicationObjectSupport{

    /**
     * 通过bean的id获取bean对象
     * @param beanName
     * @return
     */
    public Object getBean(String beanName){
        return super.getWebApplicationContext().getBean(beanName);
    }

}
```
最后莫忘了将Bean注入到Spring容器中，通过注解，或者配置均可，本示例通过配置实现
```xml
<!-- 测试获取bean的方式,继承WebApplicationObjectSupport需要先注入这个类 -->
<bean id="applicationContextUtil3" class="com.leo.util.ApplicationContextUtil3"></bean>
```
<a name="oS2JI"></a>
### 五、使用Spring提供的工具类`WebApplicationContextUtils`
使用Spring提供的工具类`WebApplicationContextUtils`来获取`WebApplicationContext`对象，这个方法很常见于SpringMVC构建的web项目中，测试代码如下所示：
```java
/**
 * 方法五：使用WebApplicationContextUtils提供的方法获取ApplicationContext对象
 */
@Test
public void getBeanTest5(){
    //模拟ServletContext上下文，不然会出现空指针异常
    MockServletContext sc = new MockServletContext("");
    sc.addInitParameter(ContextLoader.CONFIG_LOCATION_PARAM, "/applicationContext.xml");
    ServletContextListener listener = new ContextLoaderListener();
    ServletContextEvent event = new ServletContextEvent(sc);
    listener.contextInitialized(event);

    //使用WebApplicationContextUtils的getRequiredWebApplicationContext方法
    WebApplicationContext webApplicationContext = WebApplicationContextUtils.getRequiredWebApplicationContext(sc);
    UserInfo userInfo = (UserInfo) webApplicationContext.getBean("userInfo");
    System.out.println(userInfo);
    //使用WebApplicationContextUtils的getWebApplicationContext方法
    WebApplicationContext webApplicationContext2 = WebApplicationContextUtils.getWebApplicationContext(sc);
    UserInfo userInfo2 = (UserInfo) webApplicationContext2.getBean("userInfo");
    System.out.println(userInfo2);
}
```
<a name="E1xR1"></a>
### 六、实现`ApplicationContextAware`接口
通过实现`ApplicationContextAware`接口，在Spring容器启动的时候将`ApplicationContext`注入进去，从而获取`ApplicationContext`对象，这种方法也是常见的获取Bean的一种方式，测试代码如下：
```java
/**
 *方法六：实现ApplicationContextAware接口获取ApplicationContext
 */
@Test
public void getBeanTest6(){
    UserInfo userInfo2 = (UserInfo) ApplicationContextUtil.getBean("userInfo");
    System.out.println(userInfo2);
}
```
其中`ApplicationContextUtil`的实现如下：
```java
public class ApplicationContextUtil implements ApplicationContextAware{
    private static ApplicationContext applicationContext;

    /**
     * 通过bean的id获取bean对象
     * @param beanName
     * @return
     */
    public static Object getBean(String beanName){
        return applicationContext.getBean(beanName);
    }

    /**
     * 根据bean的id和类型获取bean对象
     * @param beanName
     * @param clazz
     * @param <T>
     * @return
     */
    public static <T> T getBean(String beanName,Class<T> clazz){
        return clazz.cast(getBean(beanName));
    }


    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }
}
```
<a name="XV34T"></a>
### 七、使用`ContextLoader`提供的`getCurrentWebApplicationContext`方法
使用`ContextLoader`提供的`getCurrentWebApplicationContext`方法提供的方法也是常用的获取`WebApplicationContext`的一种方法，这个方法常见于SpringMVC实现的web项目中。<br />测试代码如下：
```java
/**
 * 方法七：使用ContextLoader的getCurrentWebApplicationContext方法获取WebApplicationContext
 */
@Test
public void getBeanTest7() {
    MockServletContext sc = new MockServletContext("");
    sc.addInitParameter(ContextLoader.CONFIG_LOCATION_PARAM, "/applicationContext.xml");
    ServletContextListener listener = new ContextLoaderListener();
    ServletContextEvent event = new ServletContextEvent(sc);
    listener.contextInitialized(event);
    //如果不加上面的模拟创建ServletContext对象，会报空指针异常
    WebApplicationContext wac = ContextLoader.getCurrentWebApplicationContext();
    UserInfo userInfo = (UserInfo) wac.getBean("userInfo");
    System.out.println(userInfo);
}
```
