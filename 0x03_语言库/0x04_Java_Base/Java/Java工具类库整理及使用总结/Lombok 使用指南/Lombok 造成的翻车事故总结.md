JavaLombok
<a name="sV3bx"></a>
## Setter-Getter方法的坑
<a name="nPuHw"></a>
### 问题发现
在项目当中主要使用Lombok的Setter-Getter方法的注解，也就是组合注解`@Data`，但是在一次使用Mybatis插入数据的过程当中，出现了一个问题，问题描述如下：<br />有个实体类：
```java
@Data
public class NMetaVerify{
    private NMetaType nMetaType;
    private Long id;
    ....其他属性
}
```
当使用Mybatis插入数据的时候，发现，其他属性都能正常的插入，但是就是nMetaType属性在数据库一直是null。
<a name="sJX2g"></a>
### 解决
当debug项目代码到调用Mybatis的插入SQL对应的方法的时候，看到NMetaVerify对象的nMetaType属性还是有数据的，但是执行插入之后，数据库的nMetaType字段就是一直是null，原先以为是我的枚举类型写法不正确，看了下别的同样具有枚举类型的字段，也是正常能插入到数据库当中的，这更让我感觉到疑惑了。<br />于是，就跟踪Mybatis的源码，发现Mybatis在获取这个nMetaType属性的时候使用了反射，使用的是getxxxx方法来获取的，但是发现nMetaType的get方法好像有点和Mybatis需要的getxxxx方法长的好像不一样，问题找到了！
<a name="wojZh"></a>
### 原因
Lombok对于第一个字母小写，第二个字母大写的属性生成的get-set方法和Mybatis以及idea或者说是Java官方认可的get-set方法生成的不一样:<br />Lombok生成的Get-Set方法
```java
@Data
public class NMetaVerify {
    private Long id;
    private NMetaType nMetaType;
    private Date createTime;

    public void lombokFound(){
        NMetaVerify nMetaVerify = new NMetaVerify();
        nMetaVerify.setNMetaType(NMetaType.TWO); //注意：nMetaType的set方法为setNMetaType，第一个n字母大写了，
        nMetaVerify.getNMetaType();                                  //getxxxx方法也是大写
    }
}
```
IDEA，Mybatis，Java官方默认的行为为：
```java
public class NMetaVerify {
    private Long id;
    private NMetaType nMetaType;
    private Date createTime;

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public NMetaType getnMetaType() {//注意：nMetaType属性的第一个字母小写
        return nMetaType;
    }

    public void setnMetaType(NMetaType nMetaType) {//注意：nMetaType属性的第一个字母小写
        this.nMetaType = nMetaType;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }
}
```
<a name="dhruo"></a>
#### Mybatis(3.4.6版本)解析get-set方法获取属性名字的源码：
```java
package org.apache.ibatis.reflection.property;

import java.util.Locale;

import org.apache.ibatis.reflection.ReflectionException;


public final class PropertyNamer {

    private PropertyNamer() {
        // Prevent Instantiation of Static Class
    }

    public static String methodToProperty(String name) {
        if (name.startsWith("is")) {//is开头的一般是bool类型，直接从第二个(索引)开始截取(简单粗暴)
            name = name.substring(2);
        } else if (name.startsWith("get") || name.startsWith("set")) {//set-get的就从第三个(索引)开始截取
            name = name.substring(3);
        } else {
            throw new ReflectionException("Error parsing property name '" + name + "'.  Didn't start with 'is', 'get' or 'set'.");
        }
        //下面这个判断很重要，可以分成两句话开始解释，解释如下
        //第一句话：name.length()==1
        //       对于属性只有一个字母的，例如private int x;
        //          对应的get-set方法是getX();setX(int x);
        //第二句话：name.length() > 1 && !Character.isUpperCase(name.charAt(1)))
        //      属性名字长度大于1，并且第二个(代码中的charAt(1)，这个1是数组下标)字母是小写的
        //      如果第二个char是大写的，那就直接返回name
        if (name.length() == 1 || (name.length() > 1 && !Character.isUpperCase(name.charAt(1)))) {
            name = name.substring(0, 1).toLowerCase(Locale.ENGLISH) + name.substring(1);//让属性名第一个字母小写，然后加上后面的内容
        }

        return name;
    }

    public static boolean isProperty(String name) {
        return name.startsWith("get") || name.startsWith("set") || name.startsWith("is");
    }

    public static boolean isGetter(String name) {
        return name.startsWith("get") || name.startsWith("is");
    }

    public static boolean isSetter(String name) {
        return name.startsWith("set");
    }

}
```
<a name="MxzTF"></a>
#### Mybatis解析get-set方法为属性名字测试
```java
@Test
public void foundPropertyNamer() {
    String isName = "isName";
    String getName = "getName";
    String getnMetaType = "getnMetaType";
    String getNMetaType = "getNMetaType";

    Stream.of(isName,getName,getnMetaType,getNMetaType)
    .forEach(methodName->System.out.println("方法名字是:"+methodName+" 属性名字:"+ PropertyNamer.methodToProperty(methodName)));
}
```
输出结果如下:
```
方法名字是:isName 属性名字:name 
    方法名字是:getName 属性名字:name 
    方法名字是:getnMetaType 属性名字:nMetaType //这个以及下面的属性第二个字母都是大写，所以直接返回name
    方法名字是:getNMetaType 属性名字:NMetaType
```
<a name="g6t0I"></a>
### 解决方案

1. 修改属性名字，让第二个字母小写，或者说是规定所有的属性的前两个字母必须小写
2. 如果数据库已经设计好，并且前后端接口对接好了，不想修改，那就专门为这种特殊的属性使用idea生成get-set方法复制代码
<a name="TSm7y"></a>
## `@Accessor(chain = true)`注解的问题
<a name="z9qOc"></a>
### 问题发现
在使用 EasyExcel 导出的时候，发现以前的实体类导出都很正常，但是现在新加的实体类不正常了，比对了发现，新加的实体类增加了`@Accessor(chain = true)`注解，目的主要是方便链式调用set方法:
```java
new UserDto()
.setUserName("")
.setAge(10)
........
.setBirthday(new Date());
```
<a name="yrKiv"></a>
### 原因
EasyExcel底层使用的是cglib来做反射工具包的：
```java
com.alibaba.excel.read.listener.ModelBuildEventListener 类的第130行
BeanMap.create(resultModel).putAll(map);

// 最底层的是cglib的BeanMap的这个方法调用

abstract public Object put(Object bean, Object key, Object value);
```
但是cglib使用的是Java的rt.jar里面的一个`Introspector`这个类的方法：<br />Introspector.java 第520行
```java
if (int.class.equals(argTypes[0]) && name.startsWith(GET_PREFIX)) {
    pd = new IndexedPropertyDescriptor(this.beanClass, name.substring(3), null, null, method, null);
    //下面这行判断，只获取返回值是void类型的setxxxx方法
} else if (void.class.equals(resultType) && name.startsWith(SET_PREFIX)) {
    // Simple setter
    pd = new PropertyDescriptor(this.beanClass, name.substring(3), null, method);
    if (throwsException(method, PropertyVetoException.class)) {
        pd.setConstrained(true);
    }
}
```
<a name="BuYPS"></a>
### 解决方案

1. 去掉`Accessor`注解。
2. 要么就等待EasyExcel的作者替换掉底层的cglib或者是其他，反正是支持获取返回值不是void的setxxx方法就行复制代码。
