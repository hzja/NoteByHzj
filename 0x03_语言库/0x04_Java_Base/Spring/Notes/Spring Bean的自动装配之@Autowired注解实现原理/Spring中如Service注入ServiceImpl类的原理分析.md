Java Spring

- `@Service`注解，其实做了两件事情：
- `@Autowired`注解的意思就是:
- `@Autowired`和`@Resource`两个注解的区别：

方法一：Controller中注入service的时候使用`@Autowired`自动注入，`@Qualifier("beanId")`来指定注入哪一个。<br />方法二：Controller中注入service的时候使用`@Resource(type = 类名.class)`来指定注入哪一个。<br />方法三：

1. 每个service的impl都可以指定名称（使用`@Service（"名称"）`）
2. Controller中注入service的时候使用名称来指定注入哪一个（使用`@Resource(name="名称")`）。
<a name="IlEhV"></a>
## `@Service`注解，其实做了两件事情
1、声明TeacherServiceImpl.java是一个bean。因为TeacherServiceImpl .java是一个bean，其他的类才可以使用`@Autowired`将TeacherServiceImpl 作为一个成员变量自动注入。<br />2、TeacherServiceImpl.java在bean中的id是"teacherServiceImpl"，即类名且首字母小写。
> 注意：不能有同名的，不然要报错。

<a name="Zds6w"></a>
## `@Autowired`注解的意思
当Spring发现`@Autowired`注解时，将自动在代码上下文中找到和其匹配（默认是类型匹配）的Bean，并自动注入到相应的地方去。<br />`@Resource`的作用相当于`@Autowired`。
<a name="ZOXh5"></a>
## `@Autowired`和`@Resource`两个注解的区别

1. `@Autowired`是Spring的注解，`@Resource`是J2EE的注解，这个看一下导入注解的时候这两个注解的包名就一清二楚了。
2. `@Autowired`默认按照byType方式进行bean匹配，`@Resource`默认按照byName方式进行bean匹配。
3. `@Autowired`默认情况下必须要求依赖对象必须存在，如果要允许null值，可以设置它的`required`属性为false，如：`@Autowired(required=false)`。
<a name="H3xu0"></a>
### 方法一、二代码如下：
接口
```java
public interface HumanService {
    public String name();
}
```
接口实现类
```java
@Service
public class TeacherServiceImpl implements HumanService {
    @Override
    public String name() {
        System.out.println("teacher");
        return "teacher";
    }
}

@Service
public class DoctorServiceImpl implements HumanService {
    @Override
    public String name() {
        System.out.println("doctor");
        return "doctor";
    }
}
```
控制器
```java
@RestController
public class HumanController {
    //    @Resource(type = DoctorServiceImpl.class) //方法二
    @Autowired
    @Qualifier("teacherServiceImpl")
    private HumanService humanService;

    @RequestMapping("/name")
    public String name(){
        return humanService.name();
    }
}
```
<a name="PPd37"></a>
### 方法三代码如下：
接口
```java
public interface HumanService {
    public String name();
}
```
接口实现类
```java
@Service("teacherService")
public class TeacherServiceImpl implements HumanService {
    @Override
    public String name() {
        System.out.println("teacher");
        return "teacher";
    }
}
@Service("doctorService")
public class DoctorServiceImpl implements HumanService {
    @Override
    public String name() {
        System.out.println("doctor");
        return "doctor";
    }
}
```
控制器
```java
@RestController
public class HumanController {

    @Resource(name="doctorService")
    private HumanService humanService;

    @RequestMapping("/name")
    public String name(){
        return humanService.name();
    }
}
```
