Java<br />在日常开发中如何优雅的实现对象复制。<br />首先看看为什么需要对象复制？
<a name="BiRSh"></a>
## 为什么需要对象复制
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631502641368-a3f92317-d8d7-40cb-a1c2-98ec2489694b.webp#clientId=ud7d0187e-a3a1-4&from=paste&id=uc3969fad&originHeight=1272&originWidth=766&originalType=url&ratio=1&status=done&style=shadow&taskId=ub2bda28a-c8b0-445b-b4ad-bfec3b53dc3)<br />如上，是平时开发中最常见的三层MVC架构模型，编辑操作时Controller层接收到前端传来的DTO对象，在Service层需要将DTO转换成DO，然后在数据库中保存。查询操作时Service层查询到DO对象后需要将DO对象转换成VO对象，然后通过Controller层返回给前端进行渲染。<br />这中间会涉及到大量的对象转换，很明显不能直接使用getter/setter复制对象属性，这看上去太low了。<br />所以必须要找一个第三方工具来实现对象转换。<br />看到这里有同学可能会问，为什么不能前后端都统一使用DO对象呢？这样就不存在对象转换呀？<br />设想一下如果不想定义 DTO 和 VO，直接将 DO 用到数据访问层、服务层、控制层和外部访问接口上。此时该表删除或则修改一个字段，DO 必须同步修改，这种修改将会影响到各层，这并不符合高内聚低耦合的原则。通过定义不同的 DTO 可以控制对不同系统暴露不同的属性，通过属性映射还可以实现具体的字段名称的隐藏。不同业务使用不同的模型，当一个业务发生变更需要修改字段时，不需要考虑对其它业务的影响，如果使用同一个对象则可能因为 “不敢乱改” 而产生很多不优雅的兼容性行为。
<a name="gFpNf"></a>
## 对象复制工具类推荐
对象复制的类库工具有很多，除了常见的Apache的BeanUtils，Spring的BeanUtils，Cglib BeanCopier，还有重量级组件MapStruct，Orika，Dozer，ModelMapper等。<br />如果没有特殊要求，这些工具类都可以直接使用，除了Apache的BeanUtils。原因在于Apache BeanUtils底层源码为了追求完美，加了过多的包装，使用了很多反射，做了很多校验，所以导致性能较差，并在阿里巴巴开发手册上强制规定避免使用 Apache BeanUtils。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631502641377-37efacd7-7bef-434f-81b7-10f356c38372.webp#clientId=ud7d0187e-a3a1-4&from=paste&id=u55154641&originHeight=148&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u11a01965-abf5-4a83-af7b-c243045c349)<br />强制规定避免使用 Apache BeanUtils<br />至于剩下的重量级组件，综合考虑其性能还有使用的易用性，这里更推荐使用Orika。Orika底层采用了javassist类库生成Bean映射的字节码，之后直接加载执行生成的字节码文件，在速度上比使用反射进行赋值会快很多。<br />国外大神 baeldung 已经对常见的组件性能进行过详细测试，大家可以通过 [https://www.baeldung.com/java-performance-mapping-frameworks](https://www.baeldung.com/java-performance-mapping-frameworks) 查看。
<a name="pNAQM"></a>
## Orika基本使用
要使用Orika很简单，只需要简单四步：
<a name="EFhvy"></a>
### 引入依赖
```xml
<dependency>
  <groupId>ma.glasnost.orika</groupId>
  <artifactId>orika-core</artifactId>
  <version>1.5.4</version>
</dependency>
```
<a name="sb1dj"></a>
### 构造一个MapperFactory
```java
MapperFactory mapperFactory = new DefaultMapperFactory.Builder().build();  
```
<a name="TiTMf"></a>
### 注册字段映射
```java
mapperFactory.classMap(SourceClass.class, TargetClass.class)  
   .field("firstName", "givenName")
   .field("lastName", "sirName")
   .byDefault()
   .register();
```
当字段名在两个实体不一致时可以通过`.field()`方法进行映射，如果字段名都一样则可省略，`byDefault()`方法用于注册名称相同的属性，如果不希望某个字段参与映射，可以使用`exclude`方法。
<a name="HYMah"></a>
### 进行映射
```java
MapperFacade mapper = mapperFactory.getMapperFacade();

SourceClass source = new SourceClass();  
// set some field values
...
// map the fields of 'source' onto a new instance of PersonDest
TargetClass target = mapper.map(source, TargetClass.class);  
```
经过上面四步就完成了SourceClass到TargetClass的转换。至于Orika的其他使用方法大家可以参考 [http://orika-mapper.github.io/orika-docs/index.html](http://orika-mapper.github.io/orika-docs/index.html)<br />每次都要这先创建`MapperFactory`，建立字段映射关系，才能进行映射转换。<br />别急，这里准备了一个工具类OrikaUtils。<br />提供了五个公共方法：
```java
import ma.glasnost.orika.MapperFacade;
import ma.glasnost.orika.MapperFactory;
import ma.glasnost.orika.impl.DefaultMapperFactory;
import ma.glasnost.orika.metadata.ClassMapBuilder;
import org.springframework.util.CollectionUtils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Description:
 * Orika封装的工具类
 * @date 2021/9/2 14:01
 */
public class OrikaUtils {

    private static final MapperFactory FACTORY = new DefaultMapperFactory.Builder().build();

    /**
     * 缓存实例集合
     */
    private static final Map<String, MapperFacade> CACHE_MAPPER = new ConcurrentHashMap<>();

    private final MapperFacade mapper;

    public OrikaUtils(MapperFacade mapper) {
        this.mapper = mapper;
    }

    /**
     * 转换实体函数
     * @param sourceEntity 源实体
     * @param targetClass  目标类对象
     * @param refMap       配置源类与目标类不同字段名映射
     * @param <S>          源泛型
     * @param <T>          目标泛型
     * @return 目标实体
     */
    public static <S, T> T convert(S sourceEntity, Class<T> targetClass, Map<String, String> refMap) {
        if (sourceEntity == null) {
            return null;
        }
        return classMap(sourceEntity.getClass(), targetClass, refMap).map(sourceEntity, targetClass);
    }

    /**
     * 转换实体函数
     *
     * @param sourceEntity 源实体
     * @param targetClass  目标类对象
     * @param <S>          源泛型
     * @param <T>          目标泛型
     * @return 目标实体
     */
    public static <S, T> T convert(S sourceEntity, Class<T> targetClass) {
        return convert(sourceEntity, targetClass, null);
    }

    /**
     * 转换实体集合函数
     *
     * @param sourceEntityList 源实体集合
     * @param targetClass      目标类对象
     * @param refMap           配置源类与目标类不同字段名映射
     * @param <S>              源泛型
     * @param <T>              目标泛型
     * @return 目标实体集合
     */
    public static <S, T> List<T> convertList(List<S> sourceEntityList, Class<T> targetClass, Map<String, String> refMap) {
        if (sourceEntityList == null) {
            return null;
        }
        if (sourceEntityList.size() == 0) {
            return new ArrayList<>(0);
        }
        return classMap(sourceEntityList.get(0).getClass(), targetClass, refMap).mapAsList(sourceEntityList, targetClass);
    }

    /**
     * 转换实体集合函数
     *
     * @param sourceEntityList 源实体集合
     * @param targetClass      目标类对象
     * @param <S>              源泛型
     * @param <T>              目标泛型
     * @return 目标实体集合
     */
    public static <S, T> List<T> convertList(List<S> sourceEntityList, Class<T> targetClass) {
        return convertList(sourceEntityList, targetClass, null);
    }



    /**
     * 注册属性
     * @param source 源类
     * @param target 目标类
     * @param refMap 属性转换
     */
    public static <V, P> void register(Class<V> source, Class<P> target,Map<String, String> refMap){
        if (CollectionUtils.isEmpty(refMap)) {
            FACTORY.classMap(source, target).byDefault().register();
        } else {
            ClassMapBuilder<V, P> classMapBuilder = FACTORY.classMap(source, target);
            refMap.forEach(classMapBuilder::field);
            classMapBuilder.byDefault().register();
        }
    }

    /**
     * 属性名称一致可用
     * @param source 源数据
     * @param target 目标对象
     * @return OrikaUtils
     */
    private static <V, P> OrikaUtils classMap(Class<V> source, Class<P> target) {
        return classMap(source, target, null);
    }

    /**
     * 属性名称不一致可用
     *
     * @param source 原对象
     * @param target 目标对象
     * @return OrikaUtils
     */
    private static synchronized <V, P> OrikaUtils classMap(Class<V> source, Class<P> target, Map<String, String> refMap) {
        String key = source.getCanonicalName() + ":" + target.getCanonicalName();
        if (CACHE_MAPPER.containsKey(key)) {
            return new OrikaUtils(CACHE_MAPPER.get(key));
        }
        register(source,target,refMap);
        MapperFacade mapperFacade = FACTORY.getMapperFacade();
        CACHE_MAPPER.put(key, mapperFacade);

        return new OrikaUtils(mapperFacade);
    }



    /**
     * Orika复制对象
     * @param source 源数据
     * @param target 目标对象
     * @return target
     */
    private <V, P> P map(V source, Class<P> target) {
        return mapper.map(source, target);
    }

    /**
     * 复制List
     * @param source 源对象
     * @param target 目标对象
     * @return P
     */
    private <V, P> List<P> mapAsList(List<V> source, Class<P> target) {
        return CollectionUtils.isEmpty(source) ? Collections.emptyList() : mapper.mapAsList(source, target);
    }

}
```
分别对应：

1. 字段一致实体转换
2. 字段不一致实体转换（需要字段映射）
3. 字段一致集合转换
4. 字段不一致集合转换（需要字段映射）
5. 字段属性转换注册

接下来通过单元测试案例重点介绍此工具类的使用。
<a name="jSKzC"></a>
## Orika工具类使用文档
先准备两个基础实体类，Student，Teacher。
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class Student {
    private String id;
    private String name;
    private String email;
}


@Data
@AllArgsConstructor
@NoArgsConstructor
public class Teacher {
    private String id;
    private String name;
    private String emailAddress;
}
```
<a name="rnt3V"></a>
### TC1，基础实体映射
```java
/**
 * 只拷贝相同的属性
 */
@Test
public void convertObject(){
    Student student = new Student("1","java","fcscanf@outlook.com");
    Teacher teacher = OrikaUtils.convert(student, Teacher.class);
    System.out.println(teacher);
}
```
输出结果：
```
Teacher(id=1, name=java, emailAddress=null)
```
此时由于属性名不一致，无法映射字段email。
<a name="x21El"></a>
### TC2，实体映射 - 字段转换
```java
/**
 * 拷贝不同属性
 */
@Test
public void convertRefObject(){
    Student student = new Student("1","java","fcscanf@outlook.com");

    Map<String,String> refMap = new HashMap<>(1);
    //map key 放置 源属性，value 放置 目标属性
    refMap.put("email","emailAddress");
    Teacher teacher = OrikaUtils.convert(student, Teacher.class, refMap);
    System.out.println(teacher);
}
```
输出结果：
```
Teacher(id=1, name=java, emailAddress=fcscanf@outlook.com)
```
此时由于对字段做了映射，可以将email映射到emailAddress。**注意这里的refMap中key放置的是源实体的属性，而value放置的是目标实体的属性，不要弄反了。**
<a name="Cc0Ts"></a>
### TC3，基础集合映射
```java
/**
  * 只拷贝相同的属性集合
  */
@Test
public void convertList(){
    Student student1 = new Student("1","java","fcscanf@outlook.com");
    Student student2 = new Student("2","Fcant","fcscanf@outlook.com");
    List<Student> studentList = Lists.newArrayList(student1,student2);

    List<Teacher> teacherList = OrikaUtils.convertList(studentList, Teacher.class);

    System.out.println(teacherList);
}
```
输出结果：
```
[Teacher(id=1, name=java, emailAddress=null), Teacher(id=2, name=Fcant, emailAddress=null)]
```
此时由于属性名不一致，集合中无法映射字段email。
<a name="I3zE6"></a>
### TC4，集合映射 - 字段映射
```java
/**
 * 映射不同属性的集合
 */
@Test
public void convertRefList(){
    Student student1 = new Student("1","java","fcscanf@outlook.com");
    Student student2 = new Student("2","Fcant","fcscanf@outlook.com");
    List<Student> studentList = Lists.newArrayList(student1,student2);

    Map<String,String> refMap = new HashMap<>(2);
    //map key 放置 源属性，value 放置 目标属性
    refMap.put("email","emailAddress");

    List<Teacher> teacherList = OrikaUtils.convertList(studentList, Teacher.class,refMap);

    System.out.println(teacherList);
}
```
输出结果：
```
[Teacher(id=1, name=java, emailAddress=fcscanf@outlook.com), Teacher(id=2, name=Fcant, emailAddress=fcscanf@outlook.com)]
```
也可以通过这样映射：
```java
Map<String,String> refMap = new HashMap<>(2);
refMap.put("email","emailAddress");
List<Teacher> teacherList = OrikaUtils.classMap(Student.class,Teacher.class,refMap)
        .mapAsList(studentList,Teacher.class);
```
<a name="WIKf5"></a>
### TC5，集合与实体映射
有时候需要将集合数据映射到实体中，如Person类
```java
@Data
public class Person {
    private List<String> nameParts;
}
```
现在需要将Person类nameParts的值映射到Student中，可以这样做
```java
/**
 * 数组和List的映射
 */
@Test
public void convertListObject(){
    Person person = new Person();
    person.setNameParts(Lists.newArrayList("1","java","fcscanf@outlook.com"));

    Map<String,String> refMap = new HashMap<>(2);
    //map key 放置 源属性，value 放置 目标属性
    refMap.put("nameParts[0]","id");
    refMap.put("nameParts[1]","name");
    refMap.put("nameParts[2]","email");

    Student student = OrikaUtils.convert(person, Student.class,refMap);
    System.out.println(student);
}
```
输出结果：
```
Student(id=1, name=java, email=fcscanf@outlook.com)
```
<a name="qjGT5"></a>
### TC6，类类型映射
有时候需要类类型对象映射，如BasicPerson类
```java
@Data
public class BasicPerson {
    private Student student;
}
```
现在需要将BasicPerson映射到Teacher
```java
/**
 * 类类型映射
 */
@Test
public void convertClassObject(){
    BasicPerson basicPerson = new BasicPerson();
    Student student = new Student("1","java","fcscanf@outlook.com");
    basicPerson.setStudent(student);

    Map<String,String> refMap = new HashMap<>(2);
    //map key 放置 源属性，value 放置 目标属性
    refMap.put("student.id","id");
    refMap.put("student.name","name");
    refMap.put("student.email","emailAddress");

    Teacher teacher = OrikaUtils.convert(basicPerson, Teacher.class,refMap);
    System.out.println(teacher);
}

```
输出结果：
```
Teacher(id=1, name=java, emailAddress=fcscanf@outlook.com)
```
<a name="pWR48"></a>
### TC7，多重映射
有时候会遇到多重映射，如将`StudentGrade`映射到`TeacherGrade`
```java
@Data
public class StudentGrade {
    private String studentGradeName;
    private List<Student> studentList;
}

@Data
public class TeacherGrade {
    private String teacherGradeName;
    private List<Teacher> teacherList;
}
```
这种场景稍微复杂，Student与Teacher的属性有email字段不相同，需要做转换映射；StudentGrade与TeacherGrade中的属性也需要映射。
```java
/**
 * 一对多映射
 */
@Test
public void convertComplexObject(){
  Student student1 = new Student("1","java","fcscanf@outlook.com");
  Student student2 = new Student("2","Fcant","fcscanf@outlook.com");
  List<Student> studentList = Lists.newArrayList(student1,student2);

  StudentGrade studentGrade = new StudentGrade();
  studentGrade.setStudentGradeName("硕士");
  studentGrade.setStudentList(studentList);

  Map<String,String> refMap1 = new HashMap<>(1);
  //map key 放置 源属性，value 放置 目标属性
  refMap1.put("email","emailAddress");
  OrikaUtils.register(Student.class,Teacher.class,refMap1);


  Map<String,String> refMap2 = new HashMap<>(2);
  //map key 放置 源属性，value 放置 目标属性
  refMap2.put("studentGradeName", "teacherGradeName");
  refMap2.put("studentList", "teacherList");


  TeacherGrade teacherGrade = OrikaUtils.convert(studentGrade,TeacherGrade.class,refMap2);
  System.out.println(teacherGrade);
}
```
多重映射的场景需要根据情况调用`OrikaUtils.register()`注册字段映射。<br />输出结果：
```java
TeacherGrade(teacherGradeName=硕士, teacherList=[Teacher(id=1, name=java, emailAddress=fcscanf@outlook.com), Teacher(id=2, name=Fcant, emailAddress=fcscanf@outlook.com)])
```
<a name="PR9Fx"></a>
### TC8，MyBaits plus分页映射
如果使用的是mybatis的分页组件，可以这样转换
```java
public IPage<UserDTO> selectPage(UserDTO userDTO, Integer pageNo, Integer pageSize) {
    Page page = new Page<>(pageNo, pageSize);
    LambdaQueryWrapper<User> query = new LambdaQueryWrapper();
    if (StringUtils.isNotBlank(userDTO.getName())) {
        query.like(User::getKindName,userDTO.getName());
    }
    IPage<User> pageList = page(page,query);
    // 实体转换 SysKind转化为SysKindDto
    Map<String,String> refMap = new HashMap<>(3);
    refMap.put("kindName","name");
    refMap.put("createBy","createUserName");
    refMap.put("createTime","createDate");
    return pageList.convert(item -> OrikaUtils.convert(item, UserDTO.class, refMap));
}
```
<a name="gjPIc"></a>
## 小结
在MVC架构中肯定少不了需要用到对象复制，属性转换的功能，借用Orika组件，可以很简单实现这些功能。 
