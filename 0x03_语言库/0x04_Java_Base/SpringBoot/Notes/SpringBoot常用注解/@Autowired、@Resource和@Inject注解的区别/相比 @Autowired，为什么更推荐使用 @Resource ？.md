Java
<a name="EabCm"></a>
## @Autowired 和 @Resource 的有什么区别？
`@Autowired` 和 `@Resource` 注解都是做bean注入时使用的！<br />其中`@Autowired`时Spring提供的注解；`@Resource`并不是Spring提供的，而是JDK提供的，但是Spring支持该注解的注入，使用的时候不需要导入而外的架包。
<a name="H6f0f"></a>
## 相同点
两者都可以写在 属性 或者 setter方法 上。<br />如果两者都写在字段上，那么就不需要再写setter方法了！
<a name="HunJ4"></a>
## 不同点
<a name="A7Rod"></a>
### `@Autowired`
默认的情况下是按照byType的方式注入！<br />可以举一个例子
```java
public class TestServiceImpl() {

    @Autowired
    private UserDao userDao;

    ... 
}
```
上述代码，会先去容器中查找一下，有哪些对象的类型是UserDao，找到之后把具体的值赋值到userDao中去（但是如果找到多个的话，会报错！）<br />`@Autowired`注解是按照类型（byType）装配依赖对象的，默认情况下它要求依赖对象必须存在；如果允许null值，可以设置required属性为false。<br />如果想使用按照名称（byName）来装配，可以结合`@Qualifier`注解一起使用<br />问：假如UserDao有多个实现类，比如UserDaoImpl1、UserDaoImpl2，代码要怎么写？<br />如果不用 `@Qualifier` 那默认是使用byType，会找到多个UserDao类型的，会报错！！！所以要像下面的代码这样写！！！
<a name="q40HK"></a>
#### 方案一（@Qualifier）
```java
public class TestServiceImpl() {

    @Autowired
    @Qualifier("userDaoImpl1")  // 指定哪一个实现类
    private UserDao userDao;

    ...   
}
```
<a name="sZvOt"></a>
#### 方案二（`@Primary`）
在 `@Autowired` 中，如果有多个Bean，但是不想使用的`@Qualifier`时候，可以这样做：<br />使用`@Primary`注解指定一个进行注入！！！
```java
@Primary
@Mapper
public class UserDaoImpl01 implements UserDao {
    ...
}
```
```java
@Mapper
public class UserDaoImpl02 implements UserDao {
    ...
}
```
<a name="YnFLu"></a>
### `@Resource`
默认按照是byName的方式注入，如果名称找不到，则按照类型注入。<br />`@Resource` 中有两个重要的属性 name 和 type<br />在Spring中，将`@Resource`注解的name属性解析为bean的名称，type为bean的类型。

- 如果使用name属性，则使用byName的自动注入策略
- 如果使用type属性，则使用byType的自动注入策略
- 如果不指定，这是将会通过反射机制使用byName自动注入策略
<a name="adL8c"></a>
### `@Resource`的装配顺序
1）如果同时指定了name和type，则从Spring上下文中找到唯一匹配的bean进行装配，找不到则抛出异常<br />2）如果指定了name，则从上下文中查找名称(id)匹配的bean进行装配，找不到则抛出异常<br />3）如果指定了type，则从上下文中找到类型匹配的唯一bean进行装配，找不到或者找到多个，都会抛出异常<br />4）如果既没有指定name，又没有指定type，则自动按照byName方式进行装配。如果没有匹配，则回退为一个原始类型(UserDao)进行匹配，如果匹配则自动装配。（先Name后type）
<a name="ylVJu"></a>
## 为什么更推荐使用 `@Resource` ？
Resource注解在字段上，这个注解是属于J2EE的，减少了与spring的耦合。<br />但是其实啊，这个问题，很多人可能对它的理解有误！<br />更推荐使用 `@Resource` ，不是因为`@Resource`注解性能更好之类的。<br />而是因为其可以指定是通过 name 还是 type 的注入方式，而`@Autowired`注解本身自己是不能实现这个效果的，要和`@Qualifier`一起用才可以！
