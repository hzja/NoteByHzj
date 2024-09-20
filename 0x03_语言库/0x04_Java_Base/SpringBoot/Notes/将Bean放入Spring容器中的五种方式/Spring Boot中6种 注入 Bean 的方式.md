Java SpringBoot
<a name="arL1y"></a>
## 背景
谈到Spring的时候一定会提到IOC容器、DI依赖注入，Spring通过将一个个类标注为Bean的方法注入到IOC容器中，达到了控制反转的效果。那么刚开始接触Bean的时候，一定是使用xml文件，一个一个的注入，就例如下面这样。
```xml
<bean id="bean" class="beandemo.Bean" />
```
项目一般很大的话，就需要成千上百个Bean去使用，这样写起来就很繁琐。那么Spring实现了一种通过注解来实现注入的方法。只需要在你需要注入的类前面加上相应的注解，Spring就会扫描到他们去实现注入。<br />xml扫描包的方式
```xml
<context:component-scan base-package="com.company.beandemo"/>
```
<a name="eQjxc"></a>
## 通过注解注入的一般形式
一般情况下，注入Bean有一个最直白，最易懂的方式去实现注入，下面废话先不多说，先贴代码。

---

- Bean类
```java
public class MyBean{
}
```

- Configuration类
```java
//创建一个class配置文件
@Configuration
public class MyConfiguration{
    //将一个Bean交由Spring进行管理
    @Bean
    public MyBean myBean(){
        return new MyBean();
    }
}
```

- Test类

与xml有一点不同，这里在Test中，实例化的不再是`ClassPathXmlApplicationContext`，而是获取的`AnnotationConfigApplicationContext`实例。
```java
ApplicationContext context = new AnnotationConfigApplicationContext(MyConfiguration.class);
MyBean myBean = cotext.getBean("myBean",MyBean.class);
System.out.println("myBean = " + myBean);
```

---

上面的代码中MyBean也就是需要Spring去管理的一个Bean，他只是一个简单的类。而MyConfiguration中，首先用`@Configuration`注解去标记了该类，这样标明该类是一个Spring的一个配置类，在加载配置的时候会去加载他。<br />在MyConfiguration中可以看到有一个方法返回的是一个MyBean的实例，并且该方法上标注着`@Bean`的注解，标明这是一个注入Bean的方法，会将下面的返回的Bean注入IOC。
<a name="RZ9o2"></a>
## 通过构造方法注入Bean
在生成一个Bean实例的时候，可以使用Bean的构造方法将Bean实现注入。直接看代码

---

- Bean类
```java
@Component
public class MyBeanConstructor {

    private AnotherBean anotherBeanConstructor;

    @Autowired
    public MyBeanConstructor(AnotherBean anotherBeanConstructor){
        this.anotherBeanConstructor = anotherBeanConstructor;
    }

    @Override
    public String toString() {
        return "MyBean{" +
            "anotherBeanConstructor=" + anotherBeanConstructor +
            '}';
    }
}
```

- AnotherBean类
```java
@Component(value="Bean的id，默认为类名小驼峰")
public class AnotherBean {
}
```

- Configuration类
```java
@Configuration
@ComponentScan("com.company.annotationbean")
public class MyConfiguration{
}
```

---

这里可以发现，和一般方式注入的代码不一样了，来看看新的注解都是什么意思：

- `@AutoWired`

简单粗暴，直接翻译过来的意思就是自动装配，还不理解为什么叫自动装配？看了下一个注解的解释你就知道了。若是在这里注入的时候指定一个Bean的id就要使用`@Qualifier`注解

- `@Component`（默认单例模式）

翻译过来是零件，感觉像是修汽车？是的，Spring管理Bean的方法就是修汽车的方式。在需要将一个类变成一个Bean被Spring可以注入的时候加上注解零件`@Conmonent`，那么就可以在加载Bean的时候把他像零件一样装配到这个IOC汽车上了<br />在这里还有几个其他的注解也可以实现这个功能，也就是细化的`@Component`：

- `@Controller` 标注在Controller层
- `@Service` 标注在Service层
- `@Repository` 标注在dao层
- `@ComponentScan("")`

还是翻译，零件扫描，去看看括号里的“零件仓库”里面，哪些“零件”（类）需要被装载，Spring就会去扫描这个包，将里面所有标注了`@Component`的类进行注入。<br />这里的通过构造方法进行注入就很好理解了，在装配MyBean这个零件的时候，突然发现他必须在AnotherBean的基础上才能安装到IOC里面，那么就在每次装配MyBean的时候自动装配一个AnotherBean进去。举个例子吧：<br />还是以汽车为例，在踩油门出发之前，是不是必须发车？？这里的AutoWired的内容就像发车，不发车，这个油门你踩断都没有用，他都不会走。
<a name="kJ61C"></a>
## 通过set方法注入Bean
可以在一个属性的set方法中去将Bean实现注入，看代码吧

---

- MyBean类
```java
@Component
public class MyBeanSet {

    private AnotherBean anotherBeanSet;

    @Autowired
    public void setAnotherBeanSet(AnotherBean anotherBeanSet) {
        this.anotherBeanSet = anotherBeanSet;
    }

    @Override
    public String toString() {
        return "MyBeanSet{" +
            "anotherBeanSet=" + anotherBeanSet +
            '}';
    }
}
```

- Configuration类 和 Test类

同上一个，就不贴了

---

这里可以发现在setter方法上有一个`@AutoWired`，与上面不同的是，不会在实例化该类时就自动装配这个对象，而是在显式调用setter的时候去装配。
<a name="cCqqE"></a>
## 通过属性去注入Bean
前面两种注入的方式诸如时间不同，并且代码较多，若是通过属性，即就是
```java
@Component
public class MyBeanProperty {

    @Autowired
    private AnotherBean anotherBeanProperty;

    @Override
    public String toString() {
        return "MyBeanProperty{" +
            "anotherBeanProperty=" + anotherBeanProperty +
            '}';
    }
}
```
这里可以看到这个类中需要使用AnotherBean这个实例对象，可以通过`@AutoWired`去自动装配它。<br />对于有些小伙伴问私有属性，Spring怎么去加载它到IOC的？那就是反射。
<a name="qOs2B"></a>
## 通过List注入Bean

- MyBeanList类
```java
@Component
public class MyBeanList {

    private List<String> stringList;

    @Autowired
    public void setStringList(List<String> stringList) {
        this.stringList = stringList;
    }

    public List<String> getStringList() {
        return stringList;
    }
}
```

- `MyConfiguration`类
```java
@Configuration
@ComponentScan("annoBean.annotationbean")
public class MyConfiguration {

    @Bean
    public List<String> stringList(){
        List<String> stringList = new ArrayList<String>();
        stringList.add("List-1");
        stringList.add("List-2");
        return stringList;
    }
}
```
这里将MyBeanList进行了注入，对List中的元素会逐一注入。下面介绍另一种方式注入List

- `MyConfiguration`类
```java
@Bean
//通过该注解设定Bean注入的优先级，不一定连续数字
@Order(34)
public String string1(){
    return "String-1";
}

@Bean
@Order(14)
public String string2(){
    return "String-2";
}
```
注入与List中泛型一样的类型，会自动去匹配类型，及时这里没有任何List的感觉，只是String的类型，但他会去通过List的Bean的方式去注入。<br />第二种方式的优先级高于第一种，当两个都存在的时候，若要强制去使用第一种方式，则要去指定Bean的id即可
<a name="jOg2A"></a>
## 通过Map去注入Bean
```java
@Component
public class MyBeanMap {

    private Map<String,Integer> integerMap;

    public Map<String, Integer> getIntegerMap() {
        return integerMap;
    }

    @Autowired
    public void setIntegerMap(Map<String, Integer> integerMap) {
        this.integerMap = integerMap;
    }
}

@Bean
public Map<String,Integer> integerMap(){
    Map<String,Integer> integerMap = new HashMap<String, Integer>();
    integerMap.put("map-1",1);
    integerMap.put("map-2",2);
    return integerMap;
}

@Bean
public Integer integer1(){
    return 1;
}

@Bean
public Integer integer2(){
    return 2;
}
```
同样这里也具有两种方式去注入Map类型Bean，且第二种的优先值高于第一种<br />以上就是Bean通过注解注入的几种方式，可以对比着xml注入的方式去看。
