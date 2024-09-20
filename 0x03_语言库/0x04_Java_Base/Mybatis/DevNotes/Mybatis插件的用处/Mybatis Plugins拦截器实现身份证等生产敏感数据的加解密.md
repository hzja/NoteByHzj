Java Mybatis 
<a name="TDVqv"></a>
## 1、背景
在实际生产项目中，经常需要对如身份证信息、手机号、真实姓名等的敏感数据进行加密数据库存储，但在业务代码中对敏感信息进行手动加解密则十分不优雅，甚至会存在错加密、漏加密、业务人员需要知道实际的加密规则等的情况。介绍使用SpringBoot+Mybatis拦截器+自定义注解的形式对敏感数据进行存储前拦截加密的详细过程。
<a name="SXfge"></a>
## 2、什么是Mybatis Plugin
在Mybatis官方文档中，对于Mybatis plugin的的介绍是这样的：<br />MyBatis 允许在已映射语句执行过程中的某一点进行拦截调用。默认情况下，MyBatis 允许使用插件来拦截的方法调用包括：
```java
//语句执行拦截
Executor (update, query, flushStatements, commit, rollback, getTransaction, close, isClosed)
// 参数获取、设置时进行拦截
ParameterHandler (getParameterObject, setParameters)
// 对返回结果进行拦截
ResultSetHandler (handleResultSets, handleOutputParameters)
//sql语句拦截
StatementHandler (prepare, parameterize, batch, update, query)
```
简而言之，即在执行sql的整个周期中，可以任意切入到某一点对sql的参数、sql执行结果集、sql语句本身等进行切面处理。基于这个特性，便可以使用其需要进行加密的数据进行切面统一加密处理了（分页插件 pageHelper 就是这样实现数据库分页查询的）。
<a name="O0DJi"></a>
## 3、实现基于注解的敏感信息加解密拦截器
<a name="Ons26"></a>
### 3.1 实现思路
对于数据的加密与解密，应当存在两个拦截器对数据进行拦截操作<br />参照官方文档，因此此处应当使用`ParameterHandler`拦截器对入参进行加密<br />使用`ResultSetHandler`拦截器对出参进行解密操作。<br />![2021-06-19-20-20-08-703234.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624105454369-373d0309-8056-4cca-bfe5-13ef52f59427.png#clientId=u78ce95a8-0f8e-4&from=ui&id=ucaf1a127&originHeight=352&originWidth=502&originalType=binary&ratio=3&size=531401&status=done&style=shadow&taskId=u994f8515-0cb7-4e51-b090-59e521078a2)<br />目标需要加密、解密的字段可能需要灵活变更，此时定义一个注解，对需要加密的字段进行注解，那么便可以配合拦截器对需要的数据进行加密与解密操作了。<br />Mybatis的`interceptor`接口有以下方法需要实现。
```java
public interface Interceptor {

    //主要参数拦截方法
    Object intercept(Invocation invocation) throws Throwable;

    //mybatis插件链
    default Object plugin(Object target) {return Plugin.wrap(target, this);}

    //自定义插件配置文件方法
    default void setProperties(Properties properties) {}
}
```
<a name="aso47"></a>
### 3.2 定义需要加密解密的敏感信息注解
定义注解敏感信息类（如实体类POJO\PO）的注解
```java
/**
 * 注解敏感信息类的注解
 */
@Inherited
@Target({ ElementType.TYPE })
@Retention(RetentionPolicy.RUNTIME)
public @interface SensitiveData {
}
```
定义注解敏感信息类中敏感字段的注解
```java
/**
 * 注解敏感信息类中敏感字段的注解
 */
@Inherited
@Target({ ElementType.Field })
@Retention(RetentionPolicy.RUNTIME)
public @interface SensitiveField {
}
```
<a name="D8DwW"></a>
### 3.3 定义加密接口及其实现类
定义加密接口，方便以后拓展加密方法（如AES加密算法拓展支持PBE算法，只需要注入时指定一下便可）
```java
public interface EncryptUtil {
    /**
     * 加密
     *
     * @param declaredFields paramsObject所声明的字段
     * @param paramsObject   mapper中paramsType的实例
     * @return T
     * @throws IllegalAccessException 字段不可访问异常
     */
    <T> T encrypt(Field[] declaredFields, T paramsObject) throws IllegalAccessException;
}
```
EncryptUtil 的AES加密实现类，此处AESUtil为自封装的AES加密工具，需要的小伙伴可以自行封装，本文不提供。
```java
@Component
public class AESEncrypt implements EncryptUtil {
    @Autowired
    AESUtil aesUtil;
    /**
     * 加密
     *
     * @param declaredFields paramsObject所声明的字段
     * @param paramsObject   mapper中paramsType的实例
     * @return T
     * @throws IllegalAccessException 字段不可访问异常
     */
    @Override
    public <T> T encrypt(Field[] declaredFields, T paramsObject) throws IllegalAccessException {
        for (Field field : declaredFields) {
            //取出所有被EncryptDecryptField注解的字段
            SensitiveField sensitiveField = field.getAnnotation(SensitiveField.class);
            if (!Objects.isNull(sensitiveField)) {
                field.setAccessible(true);
                Object object = field.get(paramsObject);
                //暂时只实现String类型的加密
                if (object instanceof String) {
                    String value = (String) object;
                    //加密  这里我使用自定义的AES加密工具
                    field.set(paramsObject, aesUtil.encrypt(value));
                }
            }
        }
        return paramsObject;
    }
}
```
<a name="y0y62"></a>
### 3.4 实现入参加密拦截器
Myabtis包中的`org.apache.ibatis.plugin.Interceptor`拦截器接口要求实现以下三个方法
```java
public interface Interceptor {

    //核心拦截逻辑
    Object intercept(Invocation invocation) throws Throwable;

    //拦截器链
    default Object plugin(Object target) {return Plugin.wrap(target, this);}

    //自定义配置文件操作
    default void setProperties(Properties properties) { }
}
```
因此，参考官方文档的示例，自定义一个入参加密拦截器。<br />`@Intercepts` 注解开启拦截器，`@Signature` 注解定义拦截器的实际类型。<br />`@Signature`中

- type 属性指定当前拦截器使用`StatementHandler`、`ResultSetHandler`、`ParameterHandler`，`Executor`的一种
- `method` 属性指定使用以上四种类型的具体方法（可进入class内部查看其方法）。
- args 属性指定预编译语句

此处使用了 `ParameterHandler.setParamters()`方法，拦截mapper.xml中`paramsType`的实例（即在每个含有`paramsType`属性mapper语句中，都执行该拦截器，对`paramsType`的实例进行拦截处理）
```java
/**
* 加密拦截器
* 注意@Component注解一定要加上
* 
* @author : tanzj
* @date : 2020/1/19.
  */
@Slf4j
@Component
@Intercepts({
    @Signature(type = ParameterHandler.class, method = "setParameters", args = PreparedStatement.class),
})
public class EncryptInterceptor implements Interceptor {

    private final EncryptDecryptUtil encryptUtil;

    @Autowired
    public EncryptInterceptor(EncryptDecryptUtil encryptUtil) {
        this.encryptUtil = encryptUtil;
    }

    @Override

    @Override
    public Object intercept(Invocation invocation) throws Throwable {
        //@Signature 指定了 type= parameterHandler 后，这里的 invocation.getTarget() 便是parameterHandler
        //若指定ResultSetHandler ，这里则能强转为ResultSetHandler
        ParameterHandler parameterHandler = (ParameterHandler) invocation.getTarget();
        // 获取参数对像，即 mapper 中 paramsType 的实例
        Field parameterField = parameterHandler.getClass().getDeclaredField("parameterObject");
        parameterField.setAccessible(true);
        //取出实例
        Object parameterObject = parameterField.get(parameterHandler);
        if (parameterObject != null) {
            Class<?> parameterObjectClass = parameterObject.getClass();
            //校验该实例的类是否被@SensitiveData所注解
            SensitiveData sensitiveData = AnnotationUtils.findAnnotation(parameterObjectClass, SensitiveData.class);
            if (Objects.nonNull(sensitiveData)) {
                //取出当前当前类所有字段，传入加密方法
                Field[] declaredFields = parameterObjectClass.getDeclaredFields();
                encryptUtil.encrypt(declaredFields, parameterObject);
            }
        }
        return invocation.proceed();
    }

   /**
 	* 切记配置，否则当前拦截器不会加入拦截器链
    */
    @Override
    public Object plugin(Object o) {
        return Plugin.wrap(o, this);
    }

    //自定义配置写入，没有自定义配置的可以直接置空此方法
    @Override
    public void setProperties(Properties properties) {
    }
}
```
至此完成自定义加密拦截加密。
<a name="Yc24d"></a>
### 3.5 定义解密接口及其实现类
解密接口，其中result为mapper.xml中`resultType`的实例。
```java
public interface DecryptUtil {
    /**
 * 解密
 *
 * @param result resultType的实例
 * @return T
 * @throws IllegalAccessException 字段不可访问异常
 */
    <T> T decrypt(T result) throws IllegalAccessException;
}
```
解密接口AES工具解密实现类
```java
public class AESDecrypt implements DecryptUtil {
    @Autowired
    AESUtil aesUtil;

    /**
     * 解密
     *
     * @param result resultType的实例
     * @return T
     * @throws IllegalAccessException 字段不可访问异常
     */
    @Override
    public <T> T decrypt(T result) throws IllegalAccessException {
        //取出resultType的类
        Class<?> resultClass = result.getClass();
        Field[] declaredFields = resultClass.getDeclaredFields();
        for (Field field : declaredFields) {
            //取出所有被EncryptDecryptField注解的字段
            SensitiveField sensitiveField = field.getAnnotation(SensitiveField.class);
            if (!Objects.isNull(sensitiveField)) {
                field.setAccessible(true);
                Object object = field.get(result);
                //只支持String的解密
                if (object instanceof String) {
                    String value = (String) object;
                    //对注解的字段进行逐一解密
                    field.set(result, aesUtil.decrypt(value));
                }
            }
        }
        return result;
    }
}
```
<a name="uVRob"></a>
### 3.6 定义出参解密拦截器
```java
@Slf4j
@Component
@Intercepts({
    @Signature(type = ResultSetHandler.class, method = "handleResultSets", args = {Statement.class})
})
public class DecryptInterceptor implements Interceptor {
    @Autowired
    DecryptUtil aesDecrypt;

    @Override
    public Object intercept(Invocation invocation) throws Throwable {
        //取出查询的结果
        Object resultObject = invocation.proceed();
        if (Objects.isNull(resultObject)) {
            return null;
        }
        //基于selectList
        if (resultObject instanceof ArrayList) {
            ArrayList resultList = (ArrayList) resultObject;
            if (!CollectionUtils.isEmpty(resultList) && needToDecrypt(resultList.get(0))) {
                for (Object result : resultList) {
                    //逐一解密
                    aesDecrypt.decrypt(result);
                }
            }
            //基于selectOne
        } else {
            if (needToDecrypt(resultObject)) {
                aesDecrypt.decrypt(resultObject);
            }
        }
        return resultObject;
    }

    private boolean needToDecrypt(Object object) {
        Class<?> objectClass = object.getClass();
        SensitiveData sensitiveData = AnnotationUtils.findAnnotation(objectClass, SensitiveData.class);
        return Objects.nonNull(sensitiveData);
    }


    @Override
    public Object plugin(Object target) {
        return Plugin.wrap(target, this);
    }

    @Override
    public void setProperties(Properties properties) {

    }
}
```
至此完成解密拦截器的配置工作。
<a name="bmwvp"></a>
### 3.7 注解实体类中需要加解密的字段
![2021-06-19-20-20-08-848844.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1624105263259-6d57dcde-e5ba-49f4-8591-089b5e6dbb42.png#clientId=u78ce95a8-0f8e-4&from=ui&id=ud0bf06b2&originHeight=515&originWidth=327&originalType=binary&ratio=3&size=506626&status=done&style=shadow&taskId=u7b7ddd3b-c121-4950-816c-84dcb13b824)<br />此时在mapper中，指定`paramType=User resultType=User` 便可实现脱离业务层，基于Mybatis拦截器的加解密操作。
