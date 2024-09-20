Java MapStruct<br />官网地址：[https://mapstruct.org/](https://mapstruct.org/)<br />pom 配置：
```xml
<properties>
  <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  <maven.compiler.source>1.8</maven.compiler.source>
  <maven.compiler.target>1.8</maven.compiler.target>
  <org.mapstruct.version>1.4.1.Final</org.mapstruct.version>
  <org.projectlombok.version>1.18.12</org.projectlombok.version>
</properties>

<dependencies>
  <dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct</artifactId>
    <version>${org.mapstruct.version}</version>
  </dependency>

  <!-- lombok dependencies should not end up on classpath -->
  <dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>${org.projectlombok.version}</version>
    <scope>provided</scope>
  </dependency>

  <!-- idea 2018.1.1 之前的版本需要添加下面的配置，后期的版本就不需要了，可以注释掉，
这里用的2019.3 -->
  <dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct-processor</artifactId>
    <version>${org.mapstruct.version}</version>
    <scope>provided</scope>
  </dependency>

</dependencies>

<build>
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-compiler-plugin</artifactId>
      <version>3.8.1</version>
      <configuration>
        <source>1.8</source>
        <target>1.8</target>
        <annotationProcessorPaths>
          <path>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>${org.projectlombok.version}</version>
          </path>
          <path>
            <groupId>org.mapstruct</groupId>
            <artifactId>mapstruct-processor</artifactId>
            <version>${org.mapstruct.version}</version>
          </path>
        </annotationProcessorPaths>
      </configuration>
    </plugin>
  </plugins>
</build>
```
关于lombok和mapstruct的版本兼容问题，maven插件要使用3.6.0版本以上、lombok使用1.16.16版本以上，另外编译的lombok mapstruct的插件不要忘了。否则会出现下面的错误：No property named "aaa" exists in source parameter(s). Did you mean "null"?<br />这种异常就是lombok编译异常导致缺少get setter方法造成的。还有就是缺少构造函数也会抛异常。
```java
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    private String name;
    private int age;
    private GenderEnum gender;
    private Double height;
    private Date birthday;

}
public enum GenderEnum {
    Male("1", "男"),
    Female("0", "女");

    private String code;
    private String name;

    public String getCode() {
        return this.code;
    }

    public String getName() {
        return this.name;
    }

    GenderEnum(String code, String name) {
        this.code = code;
        this.name = name;
    }
}
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class StudentVO {
    private String name;
    private int age;
    private String gender;
    private Double height;
    private String birthday;
}
@Mapper
public interface StudentMapper {

    StudentMapper INSTANCE = Mappers.getMapper(StudentMapper.class);

    @Mapping(source = "gender.name", target = "gender")
    @Mapping(source = "birthday", target = "birthday", dateFormat = "yyyy-MM-dd HH:mm:ss")
    StudentVO student2StudentVO(Student student);

}
```
实体类是开发过程少不了的，就算是用工具生成肯定也是要有的，需要手写的部分就是这个Mapper的接口，编译完成后会自动生成相应的实现类<br />![2021-09-18-12-19-39-501373.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631938795348-67402d71-8fd4-4f6b-b887-a9b0520e16c8.png#clientId=ueb59868c-4588-4&from=ui&id=ubf52659a&originHeight=407&originWidth=1073&originalType=binary&ratio=1&rotation=0&showTitle=false&size=222120&status=done&style=none&taskId=u9e56cc7e-8a82-4b59-9e8f-72d9ead612a&title=)<br />然后就可以直接用mapper进行实体的转换了
```java
public class Test {

    public static void main(String[] args) {

        Student student = Student.builder().name("小明").age(6).gender(GenderEnum.Male).height(121.1).birthday(new Date()).build();
        System.out.println(student);
        //这行代码便是实际要用的代码
        StudentVO studentVO = StudentMapper.INSTANCE.student2StudentVO(student);
        System.out.println(studentVO);

    }

}
```
![2021-09-18-12-19-44-402370.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631938795356-c716475a-7e3d-4c4b-8b00-6e10c2d01124.png#clientId=ueb59868c-4588-4&from=ui&id=uGV4L&originHeight=407&originWidth=1073&originalType=binary&ratio=1&rotation=0&showTitle=false&size=222120&status=done&style=none&taskId=ubf1ae702-6f8b-4a48-9883-8f128f2847e&title=)<br />mapper可以进行字段映射，改变字段类型，指定格式化的方式，包括一些日期的默认处理。<br />可以手动指定格式化的方法：
```java
@Mapper
public interface StudentMapper {

    StudentMapper INSTANCE = Mappers.getMapper(StudentMapper.class);

    @Mapping(source = "gender", target = "gender")
    @Mapping(source = "birthday", target = "birthday", dateFormat = "yyyy-MM-dd HH:mm:ss")
    StudentVO student2StudentVO(Student student);

    default String getGenderName(GenderEnum gender) {
        return gender.getName();
    }

}
```
上面只是最简单的实体映射处理，下面介绍一些高级用法
<a name="bCi4J"></a>
### 1、List 转换
属性映射基于上面的mapping配置
```java
@Mapper
public interface StudentMapper {

    StudentMapper INSTANCE = Mappers.getMapper(StudentMapper.class);

    @Mapping(source = "gender.name", target = "gender")
    @Mapping(source = "birthday", target = "birthday", dateFormat = "yyyy-MM-dd HH:mm:ss")
    StudentVO student2StudentVO(Student student);


    List<StudentVO> students2StudentVOs(List<Student> studentList);

}

public static void main(String[] args) {

    Student student = Student.builder().name("小明").age(6).gender(GenderEnum.Male).height(121.1).birthday(new Date()).build();

    List<Student> list = new ArrayList<>();
    list.add(student);
    List<StudentVO> result = StudentMapper.INSTANCE.students2StudentVOs(list);
    System.out.println(result);
}
```
![2021-09-18-12-19-49-492167.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631938795296-7a4ad7bd-7b2e-4b2b-90df-6e995f39c851.png#clientId=ueb59868c-4588-4&from=ui&id=gTLmj&originHeight=133&originWidth=788&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59400&status=done&style=none&taskId=ua73e7d4b-cdef-4b23-8875-fd0dff0712c&title=)
<a name="fKjLE"></a>
### 2、多对象转换到一个对象
```java
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class Student {

    private String name;
    private int age;
    private GenderEnum gender;
    private Double height;
    private Date birthday;

}

@Data
@AllArgsConstructor
@Builder
@NoArgsConstructor
public class Course {

    private String courseName;
    private int sortNo;
    private long id;

}

@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
public class StudentVO {
    private String name;
    private int age;
    private String gender;
    private Double height;
    private String birthday;
    private String course;
}

@Mapper
public interface StudentMapper {

    StudentMapper INSTANCE = Mappers.getMapper(StudentMapper.class);

    @Mapping(source = "student.gender.name", target = "gender")
    @Mapping(source = "student.birthday", target = "birthday", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Mapping(source = "course.courseName", target = "course")
    StudentVO studentAndCourse2StudentVO(Student student, Course course);

}

public class Test {

    public static void main(String[] args) {

        Student student = Student.builder().name("小明").age(6).gender(GenderEnum.Male).height(121.1).birthday(new Date()).build();
        Course course = Course.builder().id(1L).courseName("语文").build();

        StudentVO studentVO = StudentMapper.INSTANCE.studentAndCourse2StudentVO(student, course);
        System.out.println(studentVO);
    }

}
```
![2021-09-18-12-20-59-717165.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631938879243-7628f29b-fbda-441f-a4bb-47637d3fbfe7.png#clientId=ueb59868c-4588-4&from=ui&id=u22bda608&originHeight=153&originWidth=972&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59893&status=done&style=none&taskId=uc06465f9-16f4-4bd2-9697-e6580ca6b55&title=)
<a name="DZs6v"></a>
### 3、默认值
```java
@Mapper
public interface StudentMapper {

    StudentMapper INSTANCE = Mappers.getMapper(StudentMapper.class);

    @Mapping(source = "student.gender.name", target = "gender")
    @Mapping(source = "student.birthday", target = "birthday", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Mapping(source = "course.courseName", target = "course")
    @Mapping(target = "name", source = "student.name", defaultValue = "张三")
    StudentVO studentAndCourse2StudentVO(Student student, Course course);

}
```
![2021-09-18-12-20-59-886146.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631938879258-f4a01d32-1a38-4d3f-8cc5-e9681c5b065a.png#clientId=ueb59868c-4588-4&from=ui&id=NXES8&originHeight=117&originWidth=963&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61628&status=done&style=none&taskId=uf753a1b9-fbf1-46aa-ae17-8d08e14fb12&title=)
<a name="TO60q"></a>
### 4、转换基类
<a name="QbcdB"></a>
#### 准备工作
使用常见的 User 类以及对应 UserDto 对象来演示。
```java
@Data
@Accessors(chain = true)
public class User {
    private Long id;
    private String username;
    private String password;
    private Integer sex;
    private LocalDate birthday;
    private LocalDateTime createTime;
    private String config;
    private String test;
}

@Data
@Accessors(chain = true)
public class UserVo {
    private Long id;
    private String username;
    private String password;
    private Integer gender;
    private LocalDate birthday;
    private String createTime;
    private List<UserConfig> config;
    private String test;
    @Data
    public static class UserConfig {
        private String field1;
        private Integer field2;
    }
}
```
注意观察这两个类的区别。<br />因为项目中的对象转换操作基本都一样，因此抽取除了一个转换基类，不同对象如果只是简单转换可以直接继承该基类，而无需覆写基类任何方法，即只需要一个空类即可。如果子类覆写了基类的方法，则基类上的 `@Mapping` 会失效。
```java
@MapperConfig
public interface BaseMapping<SOURCE, TARGET> {

    /**
     * 映射同名属性
     */
    @Mapping(target = "createTime", dateFormat = "yyyy-MM-dd HH:mm:ss")
    TARGET sourceToTarget(SOURCE var1);

    /**
     * 反向，映射同名属性
     */
    @InheritInverseConfiguration(name = "sourceToTarget")
    SOURCE targetToSource(TARGET var1);

    /**
     * 映射同名属性，集合形式
     */
    @InheritConfiguration(name = "sourceToTarget")
    List<TARGET> sourceToTarget(List<SOURCE> var1);

    /**
     * 反向，映射同名属性，集合形式
     */
    @InheritConfiguration(name = "targetToSource")
    List<SOURCE> targetToSource(List<TARGET> var1);

    /**
     * 映射同名属性，集合流形式
     */
    List<TARGET> sourceToTarget(Stream<SOURCE> stream);

    /**
     * 反向，映射同名属性，集合流形式
     */
    List<SOURCE> targetToSource(Stream<TARGET> stream);
}
```
实现 User 与 UserVo 对象的转换器
```java
import org.mapstruct.Mapper;
import org.mapstruct.Mapping;

@Mapper(componentModel = "spring")
public interface UserMapping extends BaseMapping<User, UserVo> {

    @Mapping(target = "gender", source = "sex")
    @Mapping(target = "createTime", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Override
    UserVo sourceToTarget(User var1);

    @Mapping(target = "sex", source = "gender")
    @Mapping(target = "password", ignore = true)
    @Mapping(target = "createTime", dateFormat = "yyyy-MM-dd HH:mm:ss")
    @Override
    User targetToSource(UserVo var1);

    default List<UserConfig> strConfigToListUserConfig(String config) {
        return JSON.parseArray(config, UserConfig.class);
    }

    default String listUserConfigToStrConfig(List<UserConfig> list) {
        return JSON.toJSONString(list);
    }
}
```
这里使用的是 Spring 的方式，`@Mapper` 注解的 componentModel 属性值为 spring，不过应该大多数都用的此模式进行开发。<br />`@Mapping`用于配置对象的映射关系，示例中 User 对象性别属性名为 sex，而UserVo对象性别属性名为gender，因此需要配置 target 与 source 属性。<br />password 字段不应该返回到前台，可以采取两种方式不进行转换，第一种就是在vo对象中不出现password字段，第二种就是在`@Mapping`中设置该字段 `ignore = true`。<br />MapStruct 提供了时间格式化的属性 `dataFormat`，支持`Date`、`LocalDate`、`LocalDateTime`等时间类型与String的转换。示例中birthday 属性为 LocalDate 类型，可以无需指定dataFormat自动完成转换，而`LocalDateTime`类型默认使用的是ISO格式时间，在国内往往不符合需求，因此需要手动指定一下 dataFormat。
<a name="AlJ6g"></a>
### 5、自定义属性类型转换方法
一般常用的类型字段转换 MapStruct都能完成，但是有一些是自定义的对象类型，MapStruct就不能进行字段转换，这就需要编写对应的类型转换方法，这里使用的是JDK8，支持接口中的默认方法，可以直接在转换器中添加自定义类型转换方法。<br />示例中User对象的config属性是一个JSON字符串，UserVo对象中是List类型的，这需要实现JSON字符串与对象的互转。
```java
default List<UserConfig> strConfigToListUserConfig(String config) {
  return JSON.parseArray(config, UserConfig.class);
}

default String listUserConfigToStrConfig(List<UserConfig> list) {
  return JSON.toJSONString(list);
}
```
如果是 JDK8以下的，不支持默认方法，可以另外定义一个 转换器，然后再当前转换器的 `@Mapper` 中通过 `uses = XXX.class` 进行引用。<br />定义好方法之后，MapStruct当匹配到合适类型的字段时，会调用自定义的转换方法进行转换。
<a name="LMyl1"></a>
#### 单元测试
```java
@Slf4j
@RunWith(SpringRunner.class)
@SpringBootTest
public class MapStructTest {

    @Resource
    private UserMapping userMapping;

    @Test
    public void tetDomain2DTO() {
        User user = new User()
            .setId(1L)
            .setUsername("zhangsan")
            .setSex(1)
            .setPassword("abc123")
            .setCreateTime(LocalDateTime.now())
            .setBirthday(LocalDate.of(1999, 9, 27))
            .setConfig("[{\"field1\":\"Test Field1\",\"field2\":500}]");
        UserVo userVo = userMapping.sourceToTarget(user);
        log.info("User: {}", user);

        log.info("UserVo: {}", userVo);

    }

    @Test
    public void testDTO2Domain() {
        UserConfig userConfig = new UserConfig();
        userConfig.setField1("Test Field1");
        userConfig.setField2(500);

        UserVo userVo = new UserVo()
            .setId(1L)
            .setUsername("zhangsan")
            .setGender(2)
            .setCreateTime("2020-01-18 15:32:54")
            .setBirthday(LocalDate.of(1999, 9, 27))
            .setConfig(Collections.singletonList(userConfig));
        User user = userMapping.targetToSource(userVo);
        log.info("UserVo: {}", userVo);
        log.info("User: {}", user);
    }
}
```
<a name="oT7tL"></a>
### 6、常见问题
当两个对象属性不一致时，比如User对象中某个字段不存在与UserVo当中时，在编译时会有警告提示，可以在`@Mapping`中配置 `ignore = true`，当字段较多时，可以直接在`@Mapper`中设置`unmappedTargetPolicy`属性或者`unmappedSourcePolicy`属性为 `ReportingPolicy.IGNORE`即可。<br />如果项目中也同时使用到了 Lombok，一定要注意 Lombok的版本要等于或者高于1.18.10，否则会有编译不通过的情况发生。
