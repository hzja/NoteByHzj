Mybatis <br />Mybatis通过Mapper接口就能操作数据库的主要实现思想是：使用动态代理生成实现类，然后配合xml的映射文件中的SQL语句来实现对数据库的访问。
<a name="Tpnyc"></a>
## Mybatis编程模型
Mybatis是在iBatis上演变而来ORM框架，所以Mybatis最终会将代码转换成iBatis编程模型，而 Mybatis 代理阶段主要是将面向接口编程模型，通过动态代理转换成ibatis编程模型。
> 不直接使用iBatis编程模型的原因是为了解耦，从下面的两种示例可以看出，iBatis编程模型和配置文件耦合很严重。

<a name="5a9da895"></a>
### 面向接口编程模型
```java
@Test
// 面向接口编程模型
public void quickStart() throws Exception {
    // 2.获取sqlSession
    try (SqlSession sqlSession = sqlSessionFactory.openSession()) {
        initH2dbMybatis(sqlSession);
        // 3.获取对应mapper
        PersonMapper mapper = sqlSession.getMapper(PersonMapper.class);
        JdkProxySourceClassUtil.writeClassToDisk(mapper.getClass().getSimpleName(), mapper.getClass());
        // 4.执行查询语句并返回结果
        Person person = mapper.selectByPrimaryKey(1L);
        System.out.println(person.toString());
    }
}
```
<a name="l6iJD"></a>
### ibatis编程模型
```java
@Test
// ibatis编程模型
public void quickStartIBatis() throws Exception {
    // 2.获取sqlSession
    try (SqlSession sqlSession = sqlSessionFactory.openSession()) {
        initH2dbMybatis(sqlSession);
        // ibatis编程模型(与配置文件耦合严重)
        Person person = sqlSession.selectOne("com.fcant.domain.mapper.PersonMapper.selectByPrimaryKey", 1L);
        System.out.println(person.toString());
    }
}
```
<a name="ZrZgI"></a>
## 代理模块核心类

- MapperRegistry：Mapper接口动态代理工厂（MapperProxyFactory）的注册中心
- MapperProxyFactory：Mapper接口对应的动态代理工厂类。Mapper接口和MapperProxyFactory工厂类是一一对应关系
- MapperProxy：Mapper接口的增强器，它实现了`InvocationHandler`接口，通过该增强的invoke方法实现了对数据库的访问
- MapperMethod：对insert, update, delete, select, flush节点方法的包装类，它通过`sqlSession`来完成了对数据库的操作

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615770854934-f30560d5-9bd4-4df6-b917-fed21a09ded6.webp#align=left&display=inline&height=487&originHeight=487&originWidth=526&size=0&status=done&style=none&width=526)
<a name="ud4bB"></a>
## 代理初始化
<a name="6bc1a7c2"></a>
### 加载Mapper接口到内存
在Mybatis 源码可以发现当配置文件解析完成的最后一步是调用`org.apache.ibatis.builder.xml.XMLMapperBuilder#bindMapperForNamespace`方法。该方法的主要作用是：根据 namespace 属性将Mapper接口的动态代理工厂（MapperProxyFactory）注册到 MapperRegistry 中。源码如下：
```java
private void bindMapperForNamespace() {
    // 获取namespace属性（对应Mapper接口的全类名）
    String namespace = builderAssistant.getCurrentNamespace();
    if (namespace != null) {
        Class<?> boundType = null;
        try {
            boundType = Resources.classForName(namespace);
        } catch (ClassNotFoundException e) {
            //ignore, bound type is not required
        }
        if (boundType != null) {
            // 防止重复加载
            if (!configuration.hasMapper(boundType)) {
                // Spring may not know the real resource name so we set a flag
                // to prevent loading again this resource from the mapper interface
                // look at MapperAnnotationBuilder#loadXmlResource
                configuration.addLoadedResource("namespace:" + namespace);
                // 将Mapper接口的动态代理工厂注册到 MapperRegistry 中
                configuration.addMapper(boundType);
            }
        }
    }
}
```

1. 读取namespace属性，获取Mapper接口的全类名
2. 根据全类名将Mapper接口加载到内存
3. 判断是否重复加载Mapper接口
4. 调用Mybatis 配置类（configuration）的addMapper方法，完成后续步骤
<a name="YJXHL"></a>
### 注册代理工厂类
`org.apache.ibatis.session.Configuration#addMapper`该方法直接回去调用`org.apache.ibatis.binding.MapperRegistry#addMapper`方法完成注册。
```java
public <T> void addMapper(Class<T> type) {
    // 必须是接口
    if (type.isInterface()) {
        if (hasMapper(type)) {
            // 防止重复注册
            throw new BindingException("Type " + type + " is already known to the MapperRegistry.");
        }
        boolean loadCompleted = false;
        try {
            // 根据接口类，创建MapperProxyFactory代理工厂类
            knownMappers.put(type, new MapperProxyFactory<>(type));
            // It's important that the type is added before the parser is run
            // otherwise the binding may automatically be attempted by the
            // mapper parser. If the type is already known, it won't try.
            MapperAnnotationBuilder parser = new MapperAnnotationBuilder(config, type);
            parser.parse();
            loadCompleted = true;
        } finally {
            // 如果加载出现异常需要移除对应Mapper
            if (!loadCompleted) {
                knownMappers.remove(type);
            }
        }
    }
}
```

1. 判断加载类型是否是接口
2. 重复注册校验，如果校验不通抛出`BindingException`异常
3. 根据接口类，创建MapperProxyFactory代理工厂类
4. 如果加载出现异常需要移除对应Mapper
<a name="TsZbQ"></a>
## 获取代理对象
在Mybatis 源码中有如下代码，通过 sqlSession获取Mapper的代理对象：
```java
PersonMapper mapper = sqlSession.getMapper(PersonMapper.class);
```
<a name="FQXy1"></a>
### getMapper 获取代理对象
`sqlSession.getMapper(PersonMapper.class)`最终调用的是`org.apache.ibatis.binding.MapperRegistry#getMapper`方法，最后返回的是`PersonMapper`接口的代理对象，源码如下：
```java
public <T> T getMapper(Class<T> type, SqlSession sqlSession) {
    // 根据类型获取对应的代理工厂
    final MapperProxyFactory<T> mapperProxyFactory = (MapperProxyFactory<T>) knownMappers.get(type);
    if (mapperProxyFactory == null) {
        throw new BindingException("Type " + type + " is not known to the MapperRegistry.");
    }
    try {
        // 根据工厂类新建一个代理对象，并返回
        return mapperProxyFactory.newInstance(sqlSession);
    } catch (Exception e) {
        throw new BindingException("Error getting mapper instance. Cause: " + e, e);
    }
}
```

1. 根据类型获取对应的代理工厂
2. 根据工厂类新建一个代理对象，并返回
<a name="vFanP"></a>
### newInstance 创建代理对象
每一个Mapper接口对应一个MapperProxyFactory工厂类。MapperProxyFactory通过JDK动态代理创建代理对象，Mapper接口的代理对象是方法级别，所以每次访问数据库都需要新创建代理对象。源码如下：
```java
protected T newInstance(MapperProxy<T> mapperProxy) {
    // 使用JDK动态代理生成代理实例
    return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[]{mapperInterface}, mapperProxy);
}
public T newInstance(SqlSession sqlSession) {
    // Mapper的增强器
    final MapperProxy<T> mapperProxy = new MapperProxy<>(sqlSession, mapperInterface, methodCache);
    return newInstance(mapperProxy);
}
```

1. 先获取Mapper对应增强器(MapperProxy)
2. 根据增强器使用JDK动态代理产生代理对象
<a name="5uqkM"></a>
### 代理类的反编译结果
```java
import com.sun.proxy..Proxy8;
import com.fcant.domain.model.Person;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.UndeclaredThrowableException;
public final class $Proxy8 extends Proxy implements Proxy8 {
    private static Method m3;
   ...
    public $Proxy8(InvocationHandler var1) throws  {
        super(var1);
    }
    ...
    public final Person selectByPrimaryKey(Long var1) throws  {
        try {
            return (Person)super.h.invoke(this, m3, new Object[]{var1});
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }
    static {
        try {
            m3 = Class.forName("com.sun.proxy.$Proxy8").getMethod("selectByPrimaryKey", Class.forName("java.lang.Long")); 
        } catch (NoSuchMethodException var2) {
            throw new NoSuchMethodError(var2.getMessage());
        } catch (ClassNotFoundException var3) {
            throw new NoClassDefFoundError(var3.getMessage());
        }
    }
}
```
从代理类的反编译结果来看，都是直接调用增强器的`invoke`方法，进而实现对数据库的访问。
<a name="Td7gl"></a>
## 执行代理
通过上诉反编译代理对象，可以发现所有对数据库的访问都是在增强器`org.apache.ibatis.binding.MapperProxy#invoke`中实现的。
<a name="1g2o6"></a>
### 执行增强器 MapperProxy
```java
@Override
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    try {
        // 如果是Object本身的方法不增强
        if (Object.class.equals(method.getDeclaringClass())) {
            return method.invoke(this, args);
        }
        // 判断是否是默认方法
        else if (method.isDefault()) {
            if (privateLookupInMethod == null) {
                return invokeDefaultMethodJava8(proxy, method, args);
            } else {
                return invokeDefaultMethodJava9(proxy, method, args);
            }
        }
    } catch (Throwable t) {
        throw ExceptionUtil.unwrapThrowable(t);
    }
    // 从缓存中获取MapperMethod对象
    final MapperMethod mapperMethod = cachedMapperMethod(method);
    // 执行MapperMethod
    return mapperMethod.execute(sqlSession, args);
}
```

1. 如果是Object本身的方法不增强
2. 判断是否是默认方法
3. 从缓存中获取MapperMethod对象
4. 执行MapperMethod
<a name="fT1IM"></a>
### 模型转换 MapperMethod
`MapperMethod`封装了Mapper接口中对应方法的信息(MethodSignature)，以及对应的sql语句的信息(SqlCommand)；它是mapper接口与映射配置文件中sql语句的桥梁；MapperMethod对象不记录任何状态信息，所以它可以在多个代理对象之间共享；

- SqlCommand ：从configuration中获取方法的命名空间.方法名以及SQL语句的类型；
- MethodSignature：封装mapper接口方法的相关信息（入参，返回类型）；
- ParamNameResolver：解析mapper接口方法中的入参；
```java
public Object execute(SqlSession sqlSession, Object[] args) {
    Object result;
    // 根据SQL类型，调用不同方法。
    // 这里可以看出，操作数据库都是通过 sqlSession 来实现的
    switch (command.getType()) {
        case INSERT: {
            Object param = method.convertArgsToSqlCommandParam(args);
            result = rowCountResult(sqlSession.insert(command.getName(), param));
            break;
        }
        case UPDATE: {
            Object param = method.convertArgsToSqlCommandParam(args);
            result = rowCountResult(sqlSession.update(command.getName(), param));
            break;
        }
        case DELETE: {
            Object param = method.convertArgsToSqlCommandParam(args);
            result = rowCountResult(sqlSession.delete(command.getName(), param));
            break;
        }
        case SELECT:
            // 根据方法返回值类型来确认调用sqlSession的哪个方法
            // 无返回值或者有结果处理器
            if (method.returnsVoid() && method.hasResultHandler()) {
                executeWithResultHandler(sqlSession, args);
                result = null;
            }
            // 返回值是否为集合类型或数组
            else if (method.returnsMany()) {
                result = executeForMany(sqlSession, args);
            }
            // 返回值是否为Map
            else if (method.returnsMap()) {
                result = executeForMap(sqlSession, args);
            }
            // 返回值是否为游标类型
            else if (method.returnsCursor()) {
                result = executeForCursor(sqlSession, args);
            }
            // 查询单条记录
            else {
                // 参数解析
                Object param = method.convertArgsToSqlCommandParam(args);
                result = sqlSession.selectOne(command.getName(), param);
                if (method.returnsOptional()
                    && (result == null || !method.getReturnType().equals(result.getClass()))) {
                    result = Optional.ofNullable(result);
                }
            }
            break;
        case FLUSH:
            result = sqlSession.flushStatements();
            break;
        default:
            throw new BindingException("Unknown execution method for: " + command.getName());
    }
    if (result == null && method.getReturnType().isPrimitive() && !method.returnsVoid()) {
        throw new BindingException("Mapper method '" + command.getName()
                                   + " attempted to return null from a method with a primitive return type (" + method.getReturnType() + ").");
    }
    return result;
}
private <E> Object executeForMany(SqlSession sqlSession, Object[] args) {
    List<E> result;
    // 将方法参数转换成SqlCommand参数
    Object param = method.convertArgsToSqlCommandParam(args);
    if (method.hasRowBounds()) {
        // 获取分页参数
        RowBounds rowBounds = method.extractRowBounds(args);
        result = sqlSession.selectList(command.getName(), param, rowBounds);
    } else {
        result = sqlSession.selectList(command.getName(), param);
    }
    // issue #510 Collections & arrays support
    if (!method.getReturnType().isAssignableFrom(result.getClass())) {
        if (method.getReturnType().isArray()) {
            return convertToArray(result);
        } else {
            return convertToDeclaredCollection(sqlSession.getConfiguration(), result);
        }
    }
    return result;
}
```
在`execute`方法中完成了面向接口编程模型到iBatis编程模型的转换，转换过程如下：

1. 通过`MapperMethod.SqlCommand. type`+`MapperMethod.MethodSignature.returnType`来确定需要调用`SqlSession`中的那个方法
2. 通过`MapperMethod.SqlCommand. name`来找到需要执行方法的全类名
3. 通过`MapperMethod.MethodSignature.paramNameResolver`来转换需要传递的参数
<a name="TnMRB"></a>
## SqlSession
在Mybatis中SqlSession相当于一个门面，所有对数据库的操作都需要通过SqlSession接口，SqlSession中定义了所有对数据库的操作方法，如数据库读写命令、获取映射器、管理事务等，也是Mybatis中为数不多的有注释的类。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615770854947-bc423666-85a9-45cc-8716-6e727dee3483.webp#align=left&display=inline&height=950&originHeight=950&originWidth=572&size=0&status=done&style=none&width=572)
<a name="09b4cc07"></a>
## 流程图
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1615770854959-948c3299-07f3-4168-a869-820af672f78a.webp#align=left&display=inline&height=1530&originHeight=1530&originWidth=1080&size=0&status=done&style=shadow&width=1080)
<a name="dPMuI"></a>
## 总结
通过上面的源码解析，可以发现Mybatis面向接口编程是通过JDK动态代理模式来实现的。主要执行流程是：

1. 在映射文件初始化完成后，将对应的Mapper接口的代理工厂类`MapperProxyFactory`注册到`MapperRegistry`
2. 每次操作数据库时，`sqlSession`通过`MapperProxyFactory`获取Mapper接口的代理类
3. 代理类通过增强器`MapperProxy`调用XML映射文件中SQL节点的封装类`MapperMethod`
4. 通过`MapperMethod`将Mybatis 面向接口的编程模型转换成iBatis编程模型（SqlSession模型）
5. 通过`SqlSession`完成对数据库的操作
