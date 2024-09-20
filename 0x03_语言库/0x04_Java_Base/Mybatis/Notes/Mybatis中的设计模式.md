Mybatis 设计模式<br />Mybatis至少遇到了以下的设计模式的使用：

1. Builder模式，例如`SqlSessionFactoryBuilder`、`XMLConfigBuilder`、`XMLMapperBuilder`、`XMLStatementBuilder`、`CacheBuilder`；
2. 工厂模式，例如`SqlSessionFactory`、`ObjectFactory`、`MapperProxyFactory`；
3. 单例模式，例如`ErrorContext`和`LogFactory`；
4. 代理模式，Mybatis实现的核心，比如`MapperProxy`、`ConnectionLogger`，用的jdk的动态代理；还有executor.loader包使用了cglib或者javassist达到延迟加载的效果；
5. 组合模式，例如`SqlNode`和各个子类`ChooseSqlNode`等；
6. 模板方法模式，例如`BaseExecutor`和`SimpleExecutor`，还有`BaseTypeHandler`和所有的子类例如`IntegerTypeHandler`；
7. 适配器模式，例如Log的Mybatis接口和它对jdbc、log4j等各种日志框架的适配实现；
8. 装饰者模式，例如Cache包中的cache.decorators子包中等各个装饰者的实现；
9. 迭代器模式，例如迭代器模式`PropertyTokenizer`；

接下来挨个模式进行解读，先介绍模式自身的知识，然后解读在Mybatis中怎样应用了该模式。
<a name="nf0ig"></a>
### 1、Builder模式
Builder模式的定义是“将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。”，它属于创建类模式，一般来说，如果一个对象的构建比较复杂，超出了构造函数所能包含的范围，就可以使用工厂模式和Builder模式，相对于工厂模式会产出一个完整的产品，Builder应用于更加复杂的对象的构建，甚至只会构建产品的一个部分。
![](https://cdn.nlark.com/yuque/__puml/ba71d9050772179607585d282ce846b0.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuY2xhc3MgUGVyc29uRGlyZWN0b3Ige1xuICArQ3JlYXRlUGVyc29uKClcbn1cblxuY2xhc3MgUGVyc29uQnVpbGRlciB7XG4gICtCdWlsZEhlYWQoKVxuXHQrQnVpbGRCb2R5KClcblx0K0J1aWxkQXJtTGVmdCgpXG5cdCtCdWlsZEFybVJpZ2h0KClcblx0K0J1aWxkbGxlZ0xlZnQoKVxuXHQrQnVpbGRMZWdSaWdodCgpXG59XG5cbmNsYXNzIFBlcnNvblRoaW5CdWlsZGVyIHtcbiAgK0J1aWxkSGVhZCgpXG5cdCtCdWlsZEJvZHkoKVxuXHQrQnVpbGRBcm1MZWZ0KClcblx0K0J1aWxkQXJtUmlnaHQoKVxuXHQrQnVpbGRsbGVnTGVmdCgpXG5cdCtCdWlsZExlZ1JpZ2h0KClcbn1cblxuY2xhc3MgUGVyc29uRmF0QnVpbGRlciB7XG4gICtCdWlsZEhlYWQoKVxuXHQrQnVpbGRCb2R5KClcblx0K0J1aWxkQXJtTGVmdCgpXG5cdCtCdWlsZEFybVJpZ2h0KClcblx0K0J1aWxkbGxlZ0xlZnQoKVxuXHQrQnVpbGRMZWdSaWdodCgpXG59XG5cblBlcnNvbkRpcmVjdG9yIG8tZG93bi0-IFBlcnNvbkJ1aWxkZXJcblBlcnNvbkJ1aWxkZXIgPHwtZG93bi0gUGVyc29uVGhpbkJ1aWxkZXJcblBlcnNvbkJ1aWxkZXIgPHwtZG93bi0gUGVyc29uRmF0QnVpbGRlclxuXG5cbkBlbmR1bWwiLCJ1cmwiOiJodHRwczovL2Nkbi5ubGFyay5jb20veXVxdWUvX19wdW1sL2JhNzFkOTA1MDc3MjE3OTYwNzU4NWQyODJjZTg0NmIwLnN2ZyIsImlkIjoiVFRYVlgiLCJtYXJnaW4iOnsidG9wIjp0cnVlLCJib3R0b20iOnRydWV9LCJjYXJkIjoiZGlhZ3JhbSJ9)在Mybatis环境的初始化过程中，`SqlSessionFactoryBuilder`会调用`XMLConfigBuilder`读取所有的MybatisMapConfig.xml和所有的*Mapper.xml文件，构建Mybatis运行的核心对象`Configuration`对象，然后将该`Configuration`对象作为参数构建一个`SqlSessionFactory`对象。<br />其中`XMLConfigBuilder`在构建`Configuration`对象时，也会调用`XMLMapperBuilder`用于读取*Mapper文件，而`XMLMapperBuilder`会使用`XMLStatementBuilder`来读取和build所有的SQL语句。<br />在这个过程中，有一个相似的特点，就是这些Builder会读取文件或者配置，然后做大量的`XpathParser`解析、配置或语法的解析、反射生成对象、存入结果缓存等步骤，这么多的工作都不是一个构造函数所能包括的，因此大量采用了Builder模式来解决。<br />对于`builder`的具体类，方法都大都用`build*`开头，比如`SqlSessionFactoryBuilder`为例，它包含以下方法：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922793656-5322c03b-ea0d-4fb6-a29a-f3459dc8074c.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u1ca806c9&originHeight=394&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0118e734-3088-440d-b5fa-b4e0379ac66&title=)<br />即根据不同的输入参数来构建`SqlSessionFactory`这个工厂对象。
<a name="WZpZI"></a>
### 2、工厂模式
在Mybatis中比如`SqlSessionFactory`使用的是工厂模式，该工厂没有那么复杂的逻辑，是一个简单工厂模式。<br />简单工厂模式(Simple Factory Pattern)：又称为静态工厂方法(Static Factory Method)模式，它属于类创建型模式。在简单工厂模式中，可以根据参数的不同返回不同类的实例。简单工厂模式专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922813373-eb54c9b0-7607-4410-8372-78a0ef98f27f.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u3fe8d1d2&originHeight=422&originWidth=675&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u00591336-e44c-4024-8250-54d92606626&title=)<br />`SqlSession`可以认为是一个Mybatis工作的核心的接口，通过这个接口可以执行执行SQL语句、获取Mappers、管理事务。类似于连接MySQL的`Connection`对象。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922824105-3f53b256-33c2-4b33-a1ef-5f3df361e109.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u7e76201d&originHeight=396&originWidth=882&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u79e6b998-bbc0-4211-b41e-f70f2b0ba19&title=)<br />可以看到，该Factory的`openSession`方法重载了很多个，分别支持`autoCommit`、`Executor`、`Transaction`等参数的输入，来构建核心的`SqlSession`对象。<br />在`DefaultSqlSessionFactory`的默认工厂实现里，有一个方法可以看出工厂怎么产出一个产品：
```java
private SqlSession openSessionFromDataSource(ExecutorType execType, TransactionIsolationLevel level,
                                             boolean autoCommit) {
    Transaction tx = null;
    try {
        final Environment environment = configuration.getEnvironment();
        final TransactionFactory transactionFactory = getTransactionFactoryFromEnvironment(environment);
        tx = transactionFactory.newTransaction(environment.getDataSource(), level, autoCommit);
        final Executor executor = configuration.newExecutor(tx, execType);
        return new DefaultSqlSession(configuration, executor, autoCommit);
    } catch (Exception e) {
        closeTransaction(tx); // may have fetched a connection so lets call
        // close()
        throw ExceptionFactory.wrapException("Error opening session.  Cause: " + e, e);
    } finally {
        ErrorContext.instance().reset();
    }
}
```
这是一个`openSession`调用的底层方法，该方法先从`configuration`读取对应的环境配置，然后初始化`TransactionFactory`获得一个`Transaction`对象，然后通过`Transaction`获取一个`Executor`对象，最后通过`configuration`、`Executor`、是否`autoCommit`三个参数构建了`SqlSession`。<br />在这里其实也可以看到端倪，`SqlSession`的执行，其实是委托给对应的`Executor`来进行的。<br />而对于`LogFactory`，它的实现代码：
```java
public final class LogFactory {
    private static Constructor<? extends Log> logConstructor;
    private LogFactory() {
        // disable construction
    }
    public static Log getLog(Class<?> aClass) {
        return getLog(aClass.getName());
    }
}
```
这里有个特别的地方，是Log变量的的类型是`Constructor<?**「extends」**Log>`，也就是说该工厂生产的不只是一个产品，而是具有Log公共接口的一系列产品，比如`Log4jImpl`、`Slf4jImpl`等很多具体的Log。
<a name="b4296213"></a>
### 3、单例模式
单例模式(Singleton Pattern)：单例模式确保某一个类只有一个实例，而且自行实例化并向整个系统提供这个实例，这个类称为单例类，它提供全局访问的方法。<br />单例模式的要点有三个：一是某个类只能有一个实例；二是它必须自行创建这个实例；三是它必须自行向整个系统提供这个实例。单例模式是一种对象创建型模式。单例模式又名单件模式或单态模式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922838719-38f4f0d4-1623-4555-8e37-c0402d41eb11.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=uc1518dd3&originHeight=273&originWidth=550&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u73d62f9b-29e1-4205-b9de-0129f5375db&title=)<br />在Mybatis中有两个地方用到单例模式，`ErrorContext`和`LogFactory`，其中`ErrorContext`是用在每个线程范围内的单例，用于记录该线程的执行环境错误信息，而`LogFactory`则是提供给整个Mybatis使用的日志工厂，用于获得针对项目配置好的日志对象。<br />`ErrorContext`的单例实现代码：
```java
public class ErrorContext {
    private static final ThreadLocal<ErrorContext> LOCAL = new ThreadLocal<ErrorContext>();
    private ErrorContext() {
    }
    public static ErrorContext instance() {
        ErrorContext context = LOCAL.get();
        if (context == null) {
            context = new ErrorContext();
            LOCAL.set(context);
        }
        return context;
    }
}
```
构造函数是`private`修饰，具有一个`static`的局部`instance`变量和一个获取`instance`变量的方法，在获取实例的方法中，先判断是否为空如果是的话就先创建，然后返回构造好的对象。<br />只是这里有个有趣的地方是，LOCAL的静态实例变量使用了`ThreadLocal`修饰，也就是说它属于每个线程各自的数据，而在`instance()`方法中，先获取本线程的该实例，如果没有就创建该线程独有的`ErrorContext`。
<a name="S8FQO"></a>
### 4、代理模式
代理模式可以认为是Mybatis的核心使用的模式，正是由于这个模式，只需要编写Mapper.java接口，不需要实现，由Mybatis后台完成具体SQL的执行。<br />代理模式(Proxy Pattern) ：给某一个对象提供一个代 理，并由代理对象控制对原对象的引用。代理模式的英 文叫做Proxy或Surrogate，它是一种对象结构型模式。<br />代理模式包含如下角色：

- `Subject`: 抽象主题角色
- `Proxy`: 代理主题角色
- `RealSubject`: 真实主题角色

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922868130-266d09ab-c086-4833-9d78-f9489da53799.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u6b87b1ca&originHeight=437&originWidth=658&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u97770df1-e9bf-4eb9-a7c7-47cc7049447&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922868212-b76db39d-c324-4d24-9bee-2379d189c23b.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u59447e78&originHeight=354&originWidth=613&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub27b3e7f-91c6-46a7-9f36-de1c8b07925&title=)<br />这里有两个步骤，第一个是提前创建一个`Proxy`，第二个是使用的时候会自动请求`Proxy`，然后由`Proxy`来执行具体事务；<br />当使用`Configuration`的`getMapper`方法时，会调用`mapperRegistry.getMapper`方法，而该方法又会调用`mapperProxyFactory.newInstance(sqlSession)`来生成一个具体的代理：
```java
public class MapperProxyFactory<T> {
    private final Class<T> mapperInterface;
    private final Map<Method, MapperMethod> methodCache = new ConcurrentHashMap<Method, MapperMethod>();
    public MapperProxyFactory(Class<T> mapperInterface) {
        this.mapperInterface = mapperInterface;
    }
    public Class<T> getMapperInterface() {
        return mapperInterface;
    }
    public Map<Method, MapperMethod> getMethodCache() {
        return methodCache;
    }
    @SuppressWarnings("unchecked")
    protected T newInstance(MapperProxy<T> mapperProxy) {
        return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[] { mapperInterface },
                                          mapperProxy);
    }
    public T newInstance(SqlSession sqlSession) {
        final MapperProxy<T> mapperProxy = new MapperProxy<T>(sqlSession, mapperInterface, methodCache);
        return newInstance(mapperProxy);
    }
}
```
在这里，先通过`T newInstance(SqlSession sqlSession)`方法会得到一个`MapperProxy`对象，然后调用T `newInstance(MapperProxymapperProxy)`生成代理对象然后返回。<br />而查看`MapperProxy`的代码，可以看到如下内容：
```java
public class MapperProxy<T> implements InvocationHandler, Serializable {
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        try {
            if (Object.class.equals(method.getDeclaringClass())) {
                return method.invoke(this, args);
            } else if (isDefaultMethod(method)) {
                return invokeDefaultMethod(proxy, method, args);
            }
        } catch (Throwable t) {
            throw ExceptionUtil.unwrapThrowable(t);
        }
        final MapperMethod mapperMethod = cachedMapperMethod(method);
        return mapperMethod.execute(sqlSession, args);
    }
}
```
非常典型的，该`MapperProxy`类实现了`InvocationHandler`接口，并且实现了该接口的`invoke`方法。<br />通过这种方式，只需要编写Mapper.java接口类，当真正执行一个Mapper接口的时候，就会转发给`MapperProxy.invoke`方法，而该方法则会调用后续的`sqlSession.cud>executor.execute>prepareStatement`等一系列方法，完成SQL的执行和返回。
<a name="aYkjg"></a>
### 5、组合模式
组合模式组合多个对象形成树形结构以表示“整体-部分”的结构层次。<br />组合模式对单个对象(叶子对象)和组合对象(组合对象)具有一致性，它将对象组织到树结构中，可以用来描述整体与部分的关系。同时它也模糊了简单元素(叶子对象)和复杂元素(容器对象)的概念，使得客户能够像处理简单元素一样来处理复杂元素，从而使客户程序能够与复杂元素的内部结构解耦。<br />在使用组合模式中需要注意一点也是组合模式最关键的地方：叶子对象和组合对象实现相同的接口。这就是组合模式能够将叶子节点和对象节点进行一致处理的原因。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922907979-7f1a290c-66c2-4b98-adfe-06ebf12f7e65.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=uec03eb10&originHeight=454&originWidth=631&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf9452663-a386-4471-af6d-5619dc71e5c&title=)<br />Mybatis支持动态SQL的强大功能，比如下面的这个SQL：
```xml
<update id="update" parameterType="org.format.dynamicproxy.mybatis.bean.User">
    UPDATE users
    <trim prefix="SET" prefixOverrides=",">
        <if test="name != null and name != ''">
            name = #{name}
        </if>
        <if test="age != null and age != ''">
            , age = #{age}
        </if>
        <if test="birthday != null and birthday != ''">
            , birthday = #{birthday}
        </if>
    </trim>
    where id = ${id}
</update>
```
在这里面使用到了`trim`、if等动态元素，可以根据条件来生成不同情况下的SQL；<br />在`DynamicSqlSource.getBoundSql`方法里，调用了`rootSqlNode.apply(context)`方法，`apply`方法是所有的动态节点都实现的接口：
```java
public interface SqlNode {
    boolean apply(DynamicContext context);
}
```
对于实现该`SqlSource`接口的所有节点，就是整个组合模式树的各个节点：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652922937045-01b3c4ed-d443-4e01-8ada-d5b6a7ff9141.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u146c567a&originHeight=800&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u37036458-028f-4908-852b-88f0a4759d3&title=)<br />组合模式的简单之处在于，所有的子节点都是同一类节点，可以递归的向下执行，比如对于`TextSqlNode`，因为它是最底层的叶子节点，所以直接将对应的内容`append`到SQL语句中：
```java
@Override
public boolean apply(DynamicContext context) {
    GenericTokenParser parser = createParser(new BindingTokenParser(context, injectionFilter));
    context.appendSql(parser.parse(text));
    return true;
}
```
但是对于IfSqlNode，就需要先做判断，如果判断通过，仍然会调用子元素的`SqlNode`，即`contents.apply`方法，实现递归的解析。
```java
@Override
public boolean apply(DynamicContext context) {
    if (evaluator.evaluateBoolean(test, context.getBindings())) {
        contents.apply(context);
        return true;
    }
    return false;
}
```
<a name="7YrjI"></a>
### 6、模板方法模式
模板方法模式是所有模式中最为常见的几个模式之一，是基于继承的代码复用的基本技术。<br />模板方法模式需要开发抽象类和具体子类的设计师之间的协作。一个设计师负责给出一个算法的轮廓和骨架，另一些设计师则负责给出这个算法的各个逻辑步骤。代表这些具体逻辑步骤的方法称做基本方法(primitive method)；而将这些基本方法汇总起来的方法叫做模板方法(template method)，这个设计模式的名字就是从此而来。<br />模板类定义一个操作中的算法的骨架，而将一些步骤延迟到子类中。使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。
![](https://cdn.nlark.com/yuque/__puml/afedf0e2c5fbd4db68201e777913d535.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuY2xhc3MgQWJzdHJhY3RUZW1wbGF0ZSB7XG4gICt0ZW1wbGF0ZU1ldGhvZCgpOiB2b2lkXG5cdCNhYnN0cmFjdE1ldGhvZCgpOiB2b2lkXG5cdCNob29rTWV0aG9kKCk6IHZvaWRcblx0LWNvbmNyZXRlTWV0aG9kKCk6IHZvaWRcbn1cblxuY2xhc3MgQ29uY3JldGVUZW1wbGF0ZSB7XG5cdCthYnN0cmFjdE1ldGhvZCgpOiB2b2lkXG5cdCtob29rTWV0aG9kKCk6IHZvaWRcbn1cblxuQWJzdHJhY3RUZW1wbGF0ZSA8fC1kb3duLSBDb25jcmV0ZVRlbXBsYXRlXG5cblxuQGVuZHVtbCIsInVybCI6Imh0dHBzOi8vY2RuLm5sYXJrLmNvbS95dXF1ZS9fX3B1bWwvYWZlZGYwZTJjNWZiZDRkYjY4MjAxZTc3NzkxM2Q1MzUuc3ZnIiwiaWQiOiJjdDUzVyIsIm1hcmdpbiI6eyJ0b3AiOnRydWUsImJvdHRvbSI6dHJ1ZX0sImNhcmQiOiJkaWFncmFtIn0=)在Mybatis中，`sqlSession`的SQL执行，都是委托给`Executor`实现的，`Executor`包含以下结构：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652923777938-af91d0c3-50a1-40e7-a424-990b03fa745e.png#clientId=u4a9e2f8e-0f07-4&from=paste&height=326&id=u21e09846&originHeight=816&originWidth=1532&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39659&status=done&style=shadow&taskId=ud3c46e5a-5ac5-4c1b-b5d3-32d44fa6e20&title=&width=612.8)<br />其中的`BaseExecutor`就采用了模板方法模式，它实现了大部分的SQL执行逻辑，然后把以下几个方法交给子类定制化完成：
```java
protected abstract int doUpdate(MappedStatement ms, Object parameter) throws SQLException;
protected abstract List<BatchResult> doFlushStatements(boolean isRollback) throws SQLException;
protected abstract <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException;
```
该模板方法类有几个子类的具体实现，使用了不同的策略：

- 简单`SimpleExecutor`：每执行一次update或select，就开启一个`Statement`对象，用完立刻关闭`Statement`对象。（可以是`Statement`或`PrepareStatement`对象）
- 重用`ReuseExecutor`：执行update或select，以sql作为key查找`Statement`对象，存在就使用，不存在就创建，用完后，不关闭`Statement`对象，而是放置于`Map<String, Statement>`内，供下一次使用。（可以是`Statement`或`PrepareStatement`对象）
- 批量`BatchExecutor`：执行update（没有select，JDBC批处理不支持select），将所有sql都添加到批处理中（`addBatch()`），等待统一执行（`executeBatch()`），它缓存了多个`Statement`对象，每个`Statement`对象都是`addBatch()`完毕后，等待逐一执行`executeBatch()`批处理的；`BatchExecutor`相当于维护了多个桶，每个桶里都装了很多属于自己的SQL，就像苹果蓝里装了很多苹果，番茄蓝里装了很多番茄，最后，再统一倒进仓库。（可以是`Statement`或`PrepareStatement`对象）

比如在`SimpleExecutor`中这样实现update方法：
```java
@Override
public int doUpdate(MappedStatement ms, Object parameter) throws SQLException {
    Statement stmt = null;
    try {
        Configuration configuration = ms.getConfiguration();
        StatementHandler handler = configuration.newStatementHandler(this, ms, parameter, RowBounds.DEFAULT, null,
                                                                     null);
        stmt = prepareStatement(handler, ms.getStatementLog());
        return handler.update(stmt);
    } finally {
        closeStatement(stmt);
    }
}
```
<a name="cePDY"></a>
### 7、适配器模式
适配器模式(Adapter Pattern) ：将一个接口转换成客户希望的另一个接口，适配器模式使接口不兼容的那些类可以一起工作，其别名为包装器(Wrapper)。适配器模式既可以作为类结构型模式，也可以作为对象结构型模式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652924610807-35f30e2a-ec97-412b-9618-8fad5b51482c.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=uca02aaae&originHeight=313&originWidth=724&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ude9897a2-320b-4be6-8787-0ec82e9b24a&title=)<br />在Mybatsi的logging包中，有一个Log接口：
```java
public interface Log {
    boolean isDebugEnabled();
    boolean isTraceEnabled();
    void error(String s, Throwable e);
    void error(String s);
    void debug(String s);
    void trace(String s);
    void warn(String s);
}
```
该接口定义了Mybatis直接使用的日志方法，而Log接口具体由谁来实现呢？Mybatis提供了多种日志框架的实现，这些实现都匹配这个Log接口所定义的接口方法，最终实现了所有外部日志框架到Mybatis日志包的适配：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652924703880-1e645797-4754-434a-9784-97449649d4e2.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u04e4b427&originHeight=518&originWidth=888&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8abfff99-10e5-4027-912d-2a242042ff3&title=)<br />比如对于Log4jImpl的实现来说，该实现持有了org.apache.log4j.Logger的实例，然后所有的日志方法，均委托该实例来实现。
```java
public class Log4jImpl implements Log {
    private static final String FQCN = Log4jImpl.class.getName();
    private Logger log;
    public Log4jImpl(String clazz) {
        log = Logger.getLogger(clazz);
    }
    @Override
    public boolean isDebugEnabled() {
        return log.isDebugEnabled();
    }
    @Override
    public boolean isTraceEnabled() {
        return log.isTraceEnabled();
    }
    @Override
    public void error(String s, Throwable e) {
        log.log(FQCN, Level.ERROR, s, e);
    }
    @Override
    public void error(String s) {
        log.log(FQCN, Level.ERROR, s, null);
    }
    @Override
    public void debug(String s) {
        log.log(FQCN, Level.DEBUG, s, null);
    }
    @Override
    public void trace(String s) {
        log.log(FQCN, Level.TRACE, s, null);
    }
    @Override
    public void warn(String s) {
        log.log(FQCN, Level.WARN, s, null);
    }
}
```
<a name="96f28ffb"></a>
### 8、装饰者模式
装饰模式(Decorator Pattern) ：动态地给一个对象增加一些额外的职责(Responsibility)，就增加对象功能来说，装饰模式比生成子类实现更为灵活。其别名也可以称为包装器(Wrapper)，与适配器模式的别名相同，但它们适用于不同的场合。根据翻译的不同，装饰模式也有人称之为“油漆工模式”，它是一种对象结构型模式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652924763983-1492d626-c5e2-41a5-a463-e61d3971ac84.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u2df22dd4&originHeight=546&originWidth=643&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29d5825c-4aa8-4502-9874-40adfa03f7c&title=)<br />在Mybatis中，缓存的功能由根接口Cache（org.apache.ibatis.cache.Cache）定义。整个体系采用装饰器设计模式，数据存储和缓存的基本功能由PerpetualCache（org.apache.ibatis.cache.impl.PerpetualCache）永久缓存实现，然后通过一系列的装饰器来对PerpetualCache永久缓存进行缓存策略等方便的控制。如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652924771571-4fa99e14-06e7-42ba-83f8-51694fa1f62f.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=uf1f00a0a&originHeight=477&originWidth=956&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u39f3da38-79bc-458f-b2a0-96973219538&title=)<br />用于装饰`PerpetualCache`的标准装饰器共有8个（全部在org.apache.ibatis.cache.decorators包中）：

- `**FifoCache**`：先进先出算法，缓存回收策略
- `**LoggingCache**`：输出缓存命中的日志信息
- `**LruCache**`：最近最少使用算法，缓存回收策略
- `**ScheduledCache**`：调度缓存，负责定时清空缓存
- `**SerializedCache**`：缓存序列化和反序列化存储
- `**SoftCache**`：基于软引用实现的缓存管理策略
- `**SynchronizedCache**`：同步的缓存装饰器，用于防止多线程并发访问
- `**WeakCache**`：基于弱引用实现的缓存管理策略

另外，还有一个特殊的装饰器`TransactionalCache`：事务性的缓存<br />正如大多数持久层框架一样，Mybatis缓存同样分为一级缓存和二级缓存

- **一级缓存**，又叫本地缓存，是`PerpetualCache`类型的永久缓存，保存在执行器中（`BaseExecutor`），而执行器又在`SqlSession`（`DefaultSqlSession`）中，所以一级缓存的生命周期与`SqlSession`是相同的。
- **二级缓存**，又叫自定义缓存，实现了Cache接口的类都可以作为二级缓存，所以可配置如`encache`等的第三方缓存。二级缓存以namespace名称空间为其唯一标识，被保存在`Configuration`核心配置对象中。

二级缓存对象的默认类型为`PerpetualCache`，如果配置的缓存是默认类型，则Mybatis会根据配置自动追加一系列装饰器。<br />Cache对象之间的引用顺序为：<br />`SynchronizedCache`–>`LoggingCache`–>`SerializedCache`–>`ScheduledCache`–>`LruCache`–>`PerpetualCache`
<a name="mFrJz"></a>
### 9、迭代器模式
迭代器（Iterator）模式，又叫做游标（Cursor）模式。GOF给出的定义为：提供一种方法访问一个容器（container）对象中各个元素，而又不需暴露该对象的内部细节。<br />Java的`Iterator`就是迭代器模式的接口，只要实现了该接口，就相当于应用了迭代器模式：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652924783573-c2b6340a-1a4f-42c4-8aac-5bb0c3cd740e.png#clientId=u4a9e2f8e-0f07-4&from=paste&id=u78fd509a&originHeight=276&originWidth=560&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc37ed02b-5c01-4bc9-90ec-a75681b5844&title=)<br />比如Mybatis的`PropertyTokenizer`是`property`包中的重量级类，该类会被`reflection`包中其他的类频繁的引用到。这个类实现了`Iterator`接口，在使用时经常被用到的是`Iterator`接口中的`hasNext`这个函数。
```java
{
    private String name;
    private String indexedName;
    private String index;
    private String children;
    public PropertyTokenizer(String fullname) {
        int delim = fullname.indexOf('.');
        if (delim > -1) {
            name = fullname.substring(0, delim);
            children = fullname.substring(delim + 1);
        } else {
            name = fullname;
            children = null;
        }
        indexedName = name;
        delim = name.indexOf('[');
        if (delim > -1) {
            index = name.substring(delim + 1, name.length() - 1);
            name = name.substring(0, delim);
        }
    }
    public String getName() {
        return name;
    }
    public String getIndex() {
        return index;
    }
    public String getIndexedName() {
        return indexedName;
    }
    public String getChildren() {
        return children;
    }
    @Override
    public boolean hasNext() {
        return children != null;
    }
    @Override
    public PropertyTokenizer next() {
        return new PropertyTokenizer(children);
    }
    @Override
    public void remove() {
        throw new UnsupportedOperationException(
            "Remove is not supported, as it has no meaning in the context of properties.");
    }
}
```
可以看到，这个类传入一个字符串到构造函数，然后提供了`iterator`方法对解析后的子串进行遍历，是一个很常用的方法类。
