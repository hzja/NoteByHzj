Java
<a name="Wgufa"></a>
## 前言
最新工作中，遇到了通过反射调用get/set方法的地方，虽然反射的性能不是很好，但是相比较于硬编码的不易扩展，`getDeclareFields`可以拿到所有的成员变量，后续添加或删除成员变量时，不用修改代码，且应用次数只在修改数据时使用，故牺牲一些性能提高扩展性
<a name="LFYtM"></a>
## 传统的方式
见过很多人通过反射调用get/set方法都是通过获取属性的name，然后通过字符串截取将首字母大写，再拼上get/set来做
```java
String fieldName = field.getName();
String getMethodName = "get" + fieldName.substring(0, 1).toUpperCase() + fieldName.substring(1);
```
还有稍微好一点的同学，通过fieldName转成字符数组，首个字符-32来避免字符串截取的
```java
String fieldName = field.getName();
char[] chars = fieldName.toCharArray();
chars[0] = (char)(chars[0] - 32);
String getMethodName = "get" + new String(chars);
```
诚然，两种方式都可以，但是不知道有没有遇到过，生成的get/set方法并不是已get/set开头的，而是以is开头的，比如boolean类型的成员变量。这个时候就需要去判断属性的类型，然后用不同的前缀来拼接get/set方法名。其实，在jdk中已经包含了这样的工具类
<a name="PClvg"></a>
## `Introspector`和`PropertyDescriptor`
关于这两个类的详细介绍，这里就不说了，简单的理解就是对象信息的描述，里面提供了一些API方便拿到对象的信息
```java
BeanInfo beanInfo;
try {
    beanInfo = Introspector.getBeanInfo(template.getClass());
} catch (IntrospectionException e) {
    log.info("xxxxxxxxxxxxxxxx", e);
    return null;
}

List<PropertyDescriptor> descriptors = Arrays.stream(beanInfo.getPropertyDescriptors()).filter(p -> {
    String name = p.getName();
    //过滤掉不需要修改的属性
    return !"class".equals(name) && !"id".equals(name);
}).collect(Collectors.toList());

for (PropertyDescriptor descriptor : descriptors) {
    //descriptor.getWriteMethod()方法对应set方法
    Method readMethod = descriptor.getReadMethod();
    System.out.println(descriptor.getName());
    try {
        Object o = readMethod.invoke(template);
        System.out.println(o);
    } catch (IllegalAccessException | InvocationTargetException e) {
        log.info("xxxxxxxxxxxxxxxx", e);
        return null;
    }
}
```
`PropertyDescriptor`类提供了`getReadMethod`和`getWriteMethod`，其实就是对于get/set方法，至于方法名称不需要来关于，这样就可以避免方法名拼错的情况了。 另外`PropertyDescriptor`除了可以通过`Introspector`获取，也可以自己`new`来创建，其构造方法还是比较全的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671634139330-03b88c40-933c-4330-8607-0d48c6845016.png#averageHue=%23f6f2ee&clientId=u51aad8e4-a3ba-4&from=paste&id=u4e168af8&originHeight=128&originWidth=421&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue482d944-bf3c-4247-83f9-16a191256ef&title=)<br />通常传递一个属性的名称和类对象class就可以了
```java
List<Field> fields = Arrays.stream(template.getClass().getDeclaredFields()).filter(f -> {
    String name = f.getName();
    //过滤掉不需要修改的属性
    return !"id".equals(name) && !"serialVersionUID".equals(name);
}).collect(Collectors.toList());

for (Field field : fields) {
    try {
        PropertyDescriptor descriptor = new PropertyDescriptor(field.getName(), template.getClass());
        Method readMethod = descriptor.getReadMethod();
        Object o = readMethod.invoke(template);
        System.out.println(o);
    } catch (IntrospectionException | IllegalAccessException | InvocationTargetException e) {
        e.printStackTrace();
    }
}
```
通过上面两种不同的实现方式可以看到，`Introspector`会额外有一个class属性，但是类似`serialVersionUID`不会算在内；而自定义`PropertyDescriptor`需要通过反射拿到所有的属性，虽然不会有class属性，但是`serialVersionUID`会算在内，使用的时候需要注意一下。 `Introspector`不同于普通的反射，反射一次，一段时间内可重复使用，为什么不是永久呢，看下源码
```java
/**
 * Introspect on a Java Bean and learn about all its properties, exposed
 * methods, and events.
 * <p>
 * If the BeanInfo class for a Java Bean has been previously Introspected
 * then the BeanInfo class is retrieved from the BeanInfo cache.
 *
 * @param beanClass  The bean class to be analyzed.
 * @return  A BeanInfo object describing the target bean.
 * @exception IntrospectionException if an exception occurs during
 *              introspection.
 * @see #flushCaches
 * @see #flushFromCaches
 */
public static BeanInfo getBeanInfo(Class<?> beanClass)
    throws IntrospectionException
{
    if (!ReflectUtil.isPackageAccessible(beanClass)) {
        return (new Introspector(beanClass, null, USE_ALL_BEANINFO)).getBeanInfo();
    }
    ThreadGroupContext context = ThreadGroupContext.getContext();
    BeanInfo beanInfo;
    synchronized (declaredMethodCache) {
        beanInfo = context.getBeanInfo(beanClass);
    }
    if (beanInfo == null) {
        beanInfo = new Introspector(beanClass, null, USE_ALL_BEANINFO).getBeanInfo();
        synchronized (declaredMethodCache) {
            context.putBeanInfo(beanClass, beanInfo);
        }
    }
    return beanInfo;
}
```
注意中间加粗标红的代码，这里除了同步之外，还做了一个本地的缓存
```java
BeanInfo getBeanInfo(Class<?> type) {
    return (this.beanInfoCache != null)
        ? this.beanInfoCache.get(type)
        : null;
}
```
这个`beanInfoCache` 其实是一个`WeakHashMap`，每次gc被回收，所以上面说一段时间内可以重复使用而不是永久，也是为了避免OOM吧。
