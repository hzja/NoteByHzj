JavaSpring
<a name="ez8xz"></a>
## 什么是循环依赖？
先说一下什么是循环依赖，Spring在初始化A的时候需要注入B，而初始化B的时候需要注入A，在Spring启动后这2个Bean都要被初始化完成<br />Spring的循环依赖有4种场景

1. 构造器的循环依赖（singleton，prototype）
2. 属性的循环依赖（singleton，prototype）

**「Spring目前只支持singleton类型的属性循环依赖」**
<a name="lashQ"></a>
## 构造器的循环依赖
```java
@Component
public class ConstructorA {

    private ConstructorB constructorB;

    @Autowired
    public ConstructorA(ConstructorB constructorB) {
        this.constructorB = constructorB;
    }
}
```
```java
@Component
public class ConstructorB {

    private ConstructorA constructorA;

    @Autowired
    public ConstructorB(ConstructorA constructorA) {
        this.constructorA = constructorA;
    }
}
```
```java
@Configuration
@ComponentScan("com.javashitang.dependency.constructor")
public class ConstructorConfig {
}
```
```java
public class ConstructorMain {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context =
            new AnnotationConfigApplicationContext(ConstructorConfig.class);
        System.out.println(context.getBean(ConstructorA.class));
        System.out.println(context.getBean(ConstructorB.class));
    }
}
```
运行`ConstructorMain`的main方法的时候会在第一行就报异常，说明Spring没办法初始化所有的Bean，即上面这种形式的循环依赖Spring无法解决。<br />**「构造器的循环依赖，可以在构造函数中使用**`**@Lazy**`**注解延迟加载。在注入依赖时，先注入代理对象，当首次使用时再创建对象完成注入」**
```java
@Autowired
public ConstructorB(@Lazy ConstructorA constructorA) {
    this.constructorA = constructorA;
}
```
因为主要关注属性的循环依赖，构造器的循环依赖就不做过多分析了
<a name="IlDGY"></a>
## 属性的循环依赖
先演示一下什么是属性的循环依赖
```java
@Data
@Component
public class A {

    @Autowired
    private B b;
}
```
```java
@Data
@Component
public class B {

    @Autowired
    private A a;
}
```
```java
@Configuration
@EnableAspectJAutoProxy
@ComponentScan("com.javashitang.dependency")
public class Config {
}
```
```java
public class Main {

    public static void main(String[] args) {
        AnnotationConfigApplicationContext context =
                new AnnotationConfigApplicationContext(Config.class);
        System.out.println(context.getBean(A.class).getB() == context.getBean(B.class));
        System.out.println(context.getBean(B.class).getA() == context.getBean(A.class));
    }
}
```
Spring容器正常启动，运行结果为true，想实现类似的功能并不难，写个demo演示一下
```java
public class DependencyDemoV1 {

    private static final Map<String, Object> singletonObjects =
            new HashMap<>(256);

    @SneakyThrows
    public static <T> T getBean(Class<T> beanClass) {
        String beanName = beanClass.getSimpleName();
        if (singletonObjects.containsKey(beanName)) {
            return (T) singletonObjects.get(beanName);
        }
        // 实例化bean
        Object object = beanClass.getDeclaredConstructor().newInstance();
        singletonObjects.put(beanName, object);
        // 开始初始化bean，即填充属性
        Field[] fields = object.getClass().getDeclaredFields();
        for (Field field : fields) {
            field.setAccessible(true);
            // 获取需要注入字段的class
            Class<?> fieldClass = field.getType();
            field.set(object, getBean(fieldClass));
        }
        return (T) object;
    }

    public static void main(String[] args) {
        // 假装扫描出来的类
        Class[] classes = {A.class, B.class};
        for (Class aClass : classes) {
            getBean(aClass);
        }
        System.out.println(getBean(A.class).getB() == getBean(B.class));
        System.out.println(getBean(B.class).getA() == getBean(A.class));
    }

}
```
**「在开始后面的内容的时候，先明确2个概念」**<br />实例化：调用构造函数将对象创建出来 初始化：调用构造函数将对象创建出来后，给对象的属性也被赋值<br />可以看到只用了一个map就实现了循环依赖的实现，但这种实现有个小缺陷，`singletonObjects`中的类有可能只是完成了实例化，并没有完成初始化<br />而在spring中`singletonObjects`中的类都完成了初始化，因为取单例Bean的时候都是从`singletonObjects`中取的，不可能获取到没有初始化完成的对象。<br />所以来写第二个实现，**「用**`**singletonObjects**`**存初始化完成的对象，而用**`**earlySingletonObjects**`**暂存实例化完成的对象，等对象初始化完毕再将对象放入**`**singletonObjects**`**，并从**`**earlySingletonObjects**`**删除」**
```java
public class DependencyDemoV2 {

    private static final Map<String, Object> singletonObjects =
            new HashMap<>(256);

    private static final Map<String, Object> earlySingletonObjects =
            new HashMap<>(256);

    @SneakyThrows
    public static <T> T getBean(Class<T> beanClass) {
        String beanName = beanClass.getSimpleName();
        if (singletonObjects.containsKey(beanName)) {
            return (T) singletonObjects.get(beanName);
        }
        if (earlySingletonObjects.containsKey(beanName)) {
            return (T) earlySingletonObjects.get(beanName);
        }
        // 实例化bean
        Object object = beanClass.getDeclaredConstructor().newInstance();
        earlySingletonObjects.put(beanName, object);
        // 开始初始化bean，即填充属性
        Field[] fields = object.getClass().getDeclaredFields();
        for (Field field : fields) {
            field.setAccessible(true);
            // 获取需要注入字段的class
            Class<?> fieldClass = field.getType();
            field.set(object, getBean(fieldClass));
        }
        singletonObjects.put(beanName, object);
        earlySingletonObjects.remove(beanName);
        return (T) object;
    }

    public static void main(String[] args) {
        // 假装扫描出来的类
        Class[] classes = {A.class, B.class};
        for (Class aClass : classes) {
            getBean(aClass);
        }
        System.out.println(getBean(A.class).getB() == getBean(B.class));
        System.out.println(getBean(B.class).getA() == getBean(A.class));
    }

}
```
现在的实现和spring保持一致了，并且只用了2级缓存。spring为什么搞第三个缓存呢？**「第三个缓存主要和代理对象相关」**<br />还是把上面的例子改进一下，改成用3级缓存的实现
```java
public interface ObjectFactory<T> {
    T getObject();
}
```
```java
public class DependencyDemoV3 {

    private static final Map<String, Object> singletonObjects =
            new HashMap<>(256);

    private static final Map<String, Object> earlySingletonObjects =
            new HashMap<>(256);

    private static final Map<String, ObjectFactory<?>> singletonFactories =
            new HashMap<>(256);

    @SneakyThrows
    public static <T> T getBean(Class<T> beanClass) {
        String beanName = beanClass.getSimpleName();
        if (singletonObjects.containsKey(beanName)) {
            return (T) singletonObjects.get(beanName);
        }
        if (earlySingletonObjects.containsKey(beanName)) {
            return (T) earlySingletonObjects.get(beanName);
        }
        ObjectFactory<?> singletonFactory = singletonFactories.get(beanName);
        if (singletonFactory != null) {
            return (T) singletonFactory.getObject();
        }
        // 实例化bean
        Object object = beanClass.getDeclaredConstructor().newInstance();
        singletonFactories.put(beanName, () -> {
            Object proxy = createProxy(object);
            singletonFactories.remove(beanName);
            earlySingletonObjects.put(beanName, proxy);
            return proxy;
        });
        // 开始初始化bean，即填充属性
        Field[] fields = object.getClass().getDeclaredFields();
        for (Field field : fields) {
            field.setAccessible(true);
            // 获取需要注入字段的class
            Class<?> fieldClass = field.getType();
            field.set(object, getBean(fieldClass));
        }
        createProxy(object);
        singletonObjects.put(beanName, object);
        singletonFactories.remove(beanName);
        earlySingletonObjects.remove(beanName);
        return (T) object;
    }

    public static Object createProxy(Object object) {
        // 因为这个方法有可能被执行2次，所以这里应该有个判断
        // 如果之前提前进行过aop操作则直接返回，知道意思就行，不写了哈
        // 需要aop的话则返回代理对象，否则返回传入的对象
        return object;
    }

    public static void main(String[] args) {
        // 假装扫描出来的类
        Class[] classes = {A.class, B.class};
        for (Class aClass : classes) {
            getBean(aClass);
        }
        System.out.println(getBean(A.class).getB() == getBean(B.class));
        System.out.println(getBean(B.class).getA() == getBean(A.class));
    }

}
```
**「为什么要包装一个ObjectFactory对象？」**<br />如果创建的Bean有对应的aop代理，那其他对象注入时，注入的应该是对应的代理对象；**「但是Spring无法提前知道这个对象是不是有循环依赖的情况」**，而正常情况下（没有循环依赖情况），Spring都是在对象初始化后才创建对应的代理。这时候Spring有两个选择：

1. 不管有没有循环依赖，实例化后就直接创建好代理对象，并将代理对象放入缓存，出现循环依赖时，其他对象直接就可以取到代理对象并注入（只需要2级缓存，`singletonObjects`和`earlySingletonObjects`即可）
2. **「不提前创建好代理对象，在出现循环依赖被其他对象注入时，才提前生成代理对象（此时只完成了实例化）。这样在没有循环依赖的情况下，Bean还是在初始化完成才生成代理对象」**（需要3级缓存）

**「所以到现在为止知道3级缓存的作用了把，主要是为了正常情况下，代理对象能在初始化完成后生成，而不用提前生成」**

| 缓存 | 说明 |
| --- | --- |
| `singletonObjects` | 第一级缓存，存放初始化完成的Bean |
| `earlySingletonObjects` | 第二级缓存，存放实例化完成的Bean，有可能被进行了代理 |
| `singletonFactories` | 延迟生成代理对象 |

<a name="lrDKZ"></a>
## 源码解析
获取Bean的时候先尝试从3级缓存中获取，和上面的Demo差不多<br />`DefaultSingletonBeanRegistry#getSingleton`![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725582542-8f81f251-091a-4839-9a54-065a2c3e8936.png#averageHue=%232e2b2a&clientId=u678ae756-d95e-4&from=paste&id=u0ae23f24&originHeight=295&originWidth=748&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u68e8c9c0-c0cb-4fad-9e6b-d20d8899164&title=)当从缓存中获取不到时，会进行创建 `AbstractAutowireCapableBeanFactory#doCreateBean`（删除了部分代码）![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725582670-e6c25cfc-c585-4944-9f63-94378248f720.png#averageHue=%232f2c2b&clientId=u678ae756-d95e-4&from=paste&id=udbfab71d&originHeight=644&originWidth=937&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u660f8220-22c9-4c37-a050-4b8a8701d86&title=)发生循环依赖时，会从工厂里获取代理对象![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725582488-6c6d2f18-81e4-48b6-bf35-b2d338932478.png#averageHue=%232e2c2b&clientId=u678ae756-d95e-4&from=paste&id=u72e94858&originHeight=265&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u21251483-b4b8-4f23-94cf-5b181fa6a58&title=)当开启aop代理时，`SmartInstantiationAwareBeanPostProcessor`的一个实现类有`AbstractAutoProxyCreator`<br />`AbstractAutoProxyCreator#getEarlyBeanReference`![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725582548-e9e883a3-5c85-4746-bc32-e20b256c2491.png#averageHue=%23302e2b&clientId=u678ae756-d95e-4&from=paste&id=ud39d707b&originHeight=111&originWidth=567&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a3394ff-1316-4c63-a26a-625b133eaa2&title=)<br />`getEarlyBeanReference`方法提前进行代理，为了防止后面再次进行代理，需要用`earlyProxyReferences`记录一下，这个Bean已经被代理过了，不用再代理了<br />`AbstractAutoProxyCreator#postProcessAfterInitialization`![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725582608-84a6a5ee-a6d5-4426-b16b-e62447aec569.png#averageHue=%232e2d2b&clientId=u678ae756-d95e-4&from=paste&id=ubeac49d6&originHeight=167&originWidth=704&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud2bf641c-8689-4d72-8452-48ea9c3a120&title=)<br />这个方法是进行aop代理的地方，因为有可能提前代理了，所以先根据`earlyProxyReferences`判断一下，是否提前代理了，提前代理过就不用代理了<br />当bean初始化完毕，会放入一级缓存，并从二三级缓存删除<br />`DefaultSingletonBeanRegistry#addSingleton`![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725583083-15453be1-8fe7-4385-be3e-80633fa66759.png#averageHue=%232f2d2b&clientId=u678ae756-d95e-4&from=paste&id=ua07a3401&originHeight=153&originWidth=573&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7e77e67d-cfaa-493c-84c1-86820030a8f&title=)<br />发生循环依赖时，整体的执行流程如下![](https://cdn.nlark.com/yuque/0/2023/png/396745/1672725583212-1cccfad0-301b-41cb-ac0c-f2bbc1110713.png#averageHue=%23f7f6f5&clientId=u678ae756-d95e-4&from=paste&id=u19ac8587&originHeight=405&originWidth=752&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5d85306d-ccae-4df7-8842-93e0a0ffe5e&title=)
