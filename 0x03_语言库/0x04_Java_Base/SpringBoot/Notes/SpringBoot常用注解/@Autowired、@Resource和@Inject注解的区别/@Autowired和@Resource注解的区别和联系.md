JavaSpringBoot
<a name="qfCo3"></a>
## 联系

- `@Autowired`和`@Resource`注解都是作为bean对象注入的时候使用的
- 两者都可以声明在字段和setter方法上
:::danger
注意：如果声明在字段上，那么就不需要再写setter方法。但是本质上，该对象还是作为set方法的实参，通过执行set方法注入，只是省略了setter方法罢了
:::
<a name="jRQex"></a>
## 区别

- `@Autowired`注解是Spring提供的，而`@Resource`注解是J2EE本身提供的
- `@Autowird`注解默认通过byType方式注入，而`@Resource`
- `@Autowired`注解注入的对象需要在IOC容器中存在，否则需要加上属性`required=false`，表示忽略当前要注入的bean，如果有直接注入，没有跳过，不会报错

简单来说，**byName就是变量名去匹配bean的id属性，而byType则是变量类型去匹配bean的class属性**
```xml
<bean id="userService" class="com.test.UserServiceImpl">
</bean>
```
```java
@Autowired
private UserService userService;
```
此处byName就是拿变量名`userService`去匹配IOC容器的`iduserService`，匹配成功；而byType就是拿变量类型UserService去匹配IOC容器的`idcom.test.UserService.UserServiceImpl`，因为`UserServiceImpl`是UserService实现，所以也匹配成功<br />接下来再分别讲讲`@Autowired`注解和`@Resource`注解的使用
<a name="O0nH7"></a>
## `@Autowird`注解的使用
<a name="qGOUL"></a>
### 步骤：
`@Autowird`默认的注入方式为byType，也就是根据类型匹配，当有多个实现时，则通过byName注入，也可以通过配合`@Qualifier`注解来显式指定name值，指明要使用哪个具体的实现类
<a name="Xqw8A"></a>
### 举例：
首先有一个接口UserService和两个实现类UserServiceImpl1和UserServiceImpl2，并且这两个实现类已经加入到Spring的IOC容器中了
```java
@Service
public class UserServiceImpl1 implements UserService

@Service
public class UserServiceImpl2 implements UserService
```
通过`@Autowired`注入使用
```java
@Autowired
private UserService userService;
```
根据上面的步骤，可以很容易判断出，直接这么使用是会报错的<br />原因：首先通过byType注入，判断UserService类型有两个实现，无法确定具体是哪一个，于是通过byName方式，这里的变量名userService也无法匹配IOC容器中id（此处指的userServiceImpl1和userServiceImpl2），于是报错。<br />注意：通过注解注入到IOC容器的id值默认是其类名首字母小写
<a name="GTXBA"></a>
### 解决方案
方式一：
```java
// 方式一：改变变量名
@Autowired
private UserService userServiceImpl1;
```
方式二：
```java
// 方式二：配合@Qualifier注解来显式指定name值
@Autowired
@Qualifier(value = "userServiceImpl1")
private UserService userService;
```
<a name="zygdj"></a>
## `@Resource`注解的使用
步骤：`@Resource`默认通过byName注入，如果没有匹配则通过byType注入
<a name="fRnnm"></a>
### 举例：
```java
@Service
public class UserServiceImpl1 implements UserService

@Service
public class UserServiceImpl2 implements UserService
```
```java
@Resource
private UserService userService;
```
首先通过byName匹配，变量名userService无法匹配IOC容器中任何一个id（这里指的userServiceImpl1和userServiceImpl2），于是通过byType匹配，发现类型UserService的实现类有两个，仍然无法确定，于是报错。<br />同时`@Resource`还有两个重要的属性：**name和type，用来显式指定byName和byType方式注入**<br />使用：对应4种情况
```java
// 1. 默认方式：byName
@Resource  
private UserService userDao; 

// 2. 指定byName
@Resource(name="userService")  
private UserService userService; 

// 3. 指定byType
@Resource(type=UserService.class)  
private UserService userService; 

// 4. 指定byName和byType
@Resource(name="userService",type=UserService.class)  
private UserService userService;
```

- **既没指定name属性，也没指定type属性：** 默认通过byName方式注入，如果byName匹配失败，则使用byType方式注入（也就是上面的那个例子）
- **指定name属性：** 通过byName方式注入，把变量名和IOC容器中的id去匹配，匹配失败则报错
- **指定type属性：** 通过byType方式注入，在IOC容器中匹配对应的类型，如果匹配不到或者匹配到多个则报错
- **同时指定name属性和type属性：** 在IOC容器中匹配，名字和类型同时匹配则成功，否则失败
