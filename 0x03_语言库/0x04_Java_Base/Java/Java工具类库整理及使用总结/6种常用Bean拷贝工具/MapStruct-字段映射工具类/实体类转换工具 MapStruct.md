Java MapStruct
<a name="Rsv4A"></a>
## 1、什么是MapStruct
<a name="yrBbX"></a>
### 1.1 JavaBean 的困扰
在开发的时候业务代码之间有很多的 JavaBean 之间的相互转化， 非常的影响观感，却又不得不存在。后来想的一个办法就是通过反射，或者自己写很多的转换器。<br />第一种通过反射的方法确实比较方便，但是现在无论是 `BeanUtils`, `BeanCopier` 等在使用反射的时候都会影响到性能。虽然可以进行反射信息的缓存来提高性能。但是像这种的话，需要类型和名称都一样才会进行映射，有很多时候，由于不同的团队之间使用的名词不一样，还是需要很多的手动 set/get 等功能。<br />第二种的话就是会很浪费时间，而且在添加新的字段的时候也要进行方法的修改。不过，由于不需要进行反射，其性能是很高的。
<a name="BPs2q"></a>
### 1.2 MapStruct 带来的改变
MapSturct 是一个生成类型安全，高性能且无依赖的 JavaBean 映射代码的注解处理器（annotation processor）。

- 注解处理器
- 可以生成 JavaBean 之间那的映射代码
- 类型安全，高性能，无依赖性
<a name="Q6YSs"></a>
## 2、MapStruct 入门
<a name="lm7wu"></a>
### 2.1 添加依赖
```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.16.20</version>
    <scope>provided</scope>
</dependency>
<dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct-jdk8</artifactId>
    <version>${org.mapstruct.version}</version>
</dependency>
<dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct-processor</artifactId>
    <version>${org.mapstruct.version}</version>
</dependency>
<dependency>
    <groupId>cn.hutool</groupId>
    <artifactId>hutool-all</artifactId>
    <version>5.1.0</version>
</dependency>
<dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <version>4.12</version>
    <scope>test</scope>
</dependency>
```
<a name="kl6yh"></a>
### 2.2 po类
```java
@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
}
```
<a name="qlh6g"></a>
### 2.3 dto类
```java
@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
}
```
<a name="tKDQF"></a>
### 2.4 创建转换接口
```java
//可以使用abstract class代替接口
@Mapper
public interface UserMapper {
    
    UserDto userToUserDto(User user);
    //集合
    List<UserDto> userToUserDto(List<User> users);
}
```
<a name="bK5q6"></a>
### 2.5 测试方法
```java
@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    UserDto userDto = mapper.userToUserDto(user);
    System.out.println(userDto);
}
```
<a name="I6JVp"></a>
### 2.6 运行效果
![2021-09-04-13-56-49-437039.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735105616-b1f9e846-49a0-4892-95ce-eabeaa3a585b.png#clientId=u00ec64f5-dd4a-4&from=ui&id=u57becff9&originHeight=161&originWidth=1080&originalType=binary&ratio=1&size=46909&status=done&style=none&taskId=u8dd1282d-89ff-4504-9023-e082dfec1c7)
<a name="gl93X"></a>
### 2.7 查看编译的class
底层通过自动取值赋值操作完成<br />![2021-09-04-13-56-49-702036.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735106877-893f13d0-ccb4-4d7c-bf4f-387f837ce8b7.png#clientId=u00ec64f5-dd4a-4&from=ui&id=cKxVW&originHeight=639&originWidth=1080&originalType=binary&ratio=1&size=2074422&status=done&style=none&taskId=u96cc16d0-88c5-4009-894e-a6671c06dbd)
<a name="Yr761"></a>
## 3、MapStruct优点分析
<a name="H5Cos"></a>
### 3.1 性能高
这是相对反射来说的，反射需要去读取字节码的内容，花销会比较大。而通过 MapStruct 来生成的代码，其类似于人手写。速度上可以得到保证。
<a name="ruRRg"></a>
### 3.2 使用简单
如果是完全映射的，使用起来肯定没有反射简单。用类似 BeanUtils 这些工具一条语句就搞定了。但是，如果需要进行特殊的匹配（特殊类型转换，多对一转换等），其相对来说也是比较简单的。<br />基本上，使用的时候，只需要声明一个接口，接口下写对应的方法，就可以使用了。当然，如果有特殊情况，是需要额外处理的。
<a name="M5LJ3"></a>
### 3.3 代码独立
生成的代码是对立的，没有运行时的依赖。
<a name="eqYS5"></a>
### 3.4 易于 debug
在生成的代码中，可以轻易的进行 debug。
<a name="KM9Yg"></a>
## 4、MapStruct使用案例
<a name="Z9idu"></a>
### 4.1 属性名称相同
在实现类的时候，如果属性名称相同，则会进行对应的转化。通过此种方式，可以快速的编写出转换的方法。（入门案例）
<a name="xoNSG"></a>
### 4.2 属性名不相同
属性名不相同，在需要进行互相转化的时候，则可以通过`@Mapping` 注解来进行转化。
```java
@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String password;
}

@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String pwd;
}

@Mapper
public interface UserMapper {
    //单个属性
    //@Mapping(source = "pwd",target = "password")
    //多个属性
    @Mappings({
            @Mapping(source = "pwd",target = "password")
    })
    UserDto userToUserDto(User user);
}
```

- source 需要转换的对接，通常是入参
- target 转换的对接，通常是出参
- ignore 忽略，默认false不忽略，需要忽略设置为true
- defaultValue 默认值
- expressions 可以通过表达式来构造一些简单的转化关系。虽然设计的时候想兼容很多语言，不过目前只能写Java代码。
```java
@Mappings({
            @Mapping(source = "birthdate", target = "birth"),//属性名不一致映射
            @Mapping(target = "birthformat", expression = "java(org.apache.commons.lang3.time.DateFormatUtils.format(person.getBirthdate(),\"yyyy-MM-dd HH:mm:ss\"))"),//自定义属性通过java代码映射
    })
public PersonVo PersonToPersonVo(Person person);
```
这里用到演示了如何使用`TimeAndFormat`对`time`和`format`操作，这里必须要指定需要使用的Java类的完整包名，不然编译的时候不知道使用哪个Java类，会报错。
```java
@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    user.setPwd("123456");
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    UserDto userDto = mapper.userToUserDto(user);
    System.out.println(userDto);
}
```
![2021-09-04-13-56-49-854037.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735126589-24fa4e4c-b8f5-4589-8da4-8bc8f1348e84.png#clientId=u00ec64f5-dd4a-4&from=ui&id=u9ddc2cb5&originHeight=433&originWidth=583&originalType=binary&ratio=1&size=759053&status=done&style=none&taskId=ufdb907b6-1421-4801-b571-a34870bb528)
<a name="m6F03"></a>
### 4.3 转换非基础类型属性
如果subUser与subUserDto字段名称相同直接配置即可完成(对象类型，包括list)
```java
@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String password;
    private List<SubUserDto> subUserDto;
}

@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String pwd;
    private List<SubUser> subUser;
}

@Mappings({
        @Mapping(source = "pwd",target = "password"),
        @Mapping(source = "subUser", target = "subUserDto")
})
UserDto userToUserDto(User user);
```
<a name="cv5Lk"></a>
### 4.4 Mapper 中使用自定义的转换
有时候，对于某些类型，无法通过代码生成器的形式来进行处理。那么， 就需要自定义的方法来进行转换。这时候，可以在接口（同一个接口，后续还有调用别的 Mapper 的方法）中定义默认方法（Java8及之后）。
```java
@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String password;
    private SubUserDto subUserDto;
}

@Data
public class SubUserDto {
    private Boolean result;
    private String name;
}

@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String pwd;
    private SubUser subUser;
}

@Data
public class SubUser {
    private Integer deleted;
    private String name;
}

@Mapper
public interface UserMapper {
    @Mappings({
            @Mapping(source = "pwd",target = "password"),
            @Mapping(source = "subUser", target = "subUserDto")
    })
    UserDto userToUserDto(User user);

    default SubUserDto subSource2subTarget(SubUser subUser) {
        if (subUser == null) {
            return null;
        }
        SubUserDto subUserDto = new SubUserDto();
        subUserDto.setResult(!subUser.getDeleted().equals(0));
        subUserDto.setName(subUser.getName()==null?"":subUser.getName());
        return subUserDto;
    }
}
```
只能存在一个`default`修饰的方法
```java
@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    user.setPwd("123456");
    SubUser subUser =new SubUser();
    subUser.setDeleted(0);
    subUser.setName("rkw");
    user.setSubUser(subUser);
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    UserDto userDto = mapper.userToUserDto(user);
    System.out.println(userDto);
}
```
![2021-09-04-13-56-49-993083.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735150581-4d9741fe-9d82-418f-9fb3-d6760b716845.png#clientId=u00ec64f5-dd4a-4&from=ui&id=ueeec245e&originHeight=462&originWidth=711&originalType=binary&ratio=1&size=987582&status=done&style=none&taskId=u951950dc-abe5-4743-a08e-7ed41f7856e)
<a name="HE19Y"></a>
### 4.5 多转一
在实际的业务中少不了将多个对象转换成一个的场景。MapStruct 当然也支持多转一的操作。
```java
@Data
public class SubUser {
    private Integer deleted;
    private String name;
}

@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String pwd;
}

@Mapper
public interface UserMapper {
    @Mappings({
            @Mapping(source = "user.pwd",target = "password"),
            @Mapping(source = "subUser.name", target = "name")
    })
    NewUserDto userToUserDto(User user,SubUser subUser);
}

@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    user.setPwd("123456");
    SubUser subUser =new SubUser();
    subUser.setDeleted(0);
    subUser.setName("rkw");
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    NewUserDto userDto = mapper.userToUserDto(user,subUser);
    System.out.println(userDto);
}
```
![2021-09-04-13-56-50-169309.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735168163-d16848d9-66d2-4cf0-8f04-c4d340b779ff.png#clientId=u00ec64f5-dd4a-4&from=ui&id=u56bc4377&originHeight=691&originWidth=1080&originalType=binary&ratio=1&size=2243231&status=done&style=none&taskId=u0975cf32-9f54-46e6-b8b9-22ab7e7626c)
<a name="mT2ff"></a>
#### 4.5.1 遵循原则

- 当多个对象中， 有其中一个为 null， 则会直接返回 null
- 如一对一转换一样， 属性通过名字来自动匹配。因此， 名称和类型相同的不需要进行特殊处理
- 当多个原对象中，有相同名字的属性时，需要通过 `@Mapping` 注解来具体的指定， 以免出现歧义（不指定会报错）。如上面的 name

属性也可以直接从传入的参数来赋值
```java
@Mapping(source = "person.description", target = "description")
@Mapping(source = "name", target = "name")
DeliveryAddress personAndAddressToDeliveryAddressDto(Person person, String name);
```
<a name="ZU5TM"></a>
### 4.6 更新 Bean 对象
有时候，不是想返回一个新的 Bean 对象，而是希望更新传入对象的一些属性。这个在实际的时候也会经常使用到。
```java
@Mapper
public interface UserMapper {

    NewUserDto userToNewUserDto(User user);

    /**
     * 更新， 注意注解 @MappingTarget
     * 注解 @MappingTarget后面跟的对象会被更新。
     */
    void updateDeliveryAddressFromAddress(SubUser subUser,@MappingTarget NewUserDto newUserDto);
}

@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    SubUser subUser =new SubUser();
    subUser.setDeleted(0);
    subUser.setName("rkw");
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    NewUserDto userDto = mapper.userToNewUserDto(user);
    mapper.updateDeliveryAddressFromAddress(subUser,userDto);
    System.out.println(userDto);
}
```
<a name="y7X4P"></a>
### 4.7 map映射
```java
@MapMapping(valueDateFormat ="yyyy-MM-dd HH:mm:ss")
public Map<String ,String> DateMapToStringMap(Map<String,Date> sourceMap);

@Test
public void mapMappingTest(){
    Map<String,Date> map=new HashMap<>();
    map.put("key1",new Date());
    map.put("key2",new Date(new Date().getTime()+9800000));
    Map<String, String> stringObjectMap = TestMapper.MAPPER.DateMapToStringMap(map);
}
```
<a name="yWZPS"></a>
### 4.8 多级嵌套
只需要在mapper接口中定义相关的类型转换方法即可，list类型也适用
<a name="EvOrR"></a>
#### 4.8.1 方式1
```java
@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private Boolean isDisable;
    private List<SubUser> user;
}

@Data
public class SubUser {
    private Integer deleted;
    private String name;
    private List<SubSubUser> subUser;
}
@Data
public class SubSubUser {
    private String aaa;
    private String ccc;
}

@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String isDisable;
    private List<SubUserDto> user;
}
@Data
public class SubUserDto {
    private Integer deleted;
    private String name;
    private List<SubSubUserDto> subUser;
}
@Data
public class SubSubUserDto {
    private String aaa;
    private  String bbb;
}

@Mapper
public interface UserMapper {

    UserDto userToNewUserDto(User user);
    
    //子集字段相同方法不用编写会自动生成
    
    //孙子集字段不相同(list会自动读取此方法生成list)
    @Mapping(source = "ccc",target = "bbb")
    SubSubUserDto bbb(SubSubUser subSubUser);

}
```
<a name="qPAlr"></a>
#### 4.8.2 方式2
通过uses配置类型转换
```java
@Mapper(uses = {TestMapper.class})
public interface UserMapper {
    UserDto userToNewUserDto(User user);
}

@Mapper
public interface TestMapper {
    @Mapping(source = "ccc",target = "bbb")
    SubSubUserDto bbb(SubSubUser subSubUser);
}
```
<a name="bmkL1"></a>
## 5、获取 mapper
<a name="gibxx"></a>
### 5.1 通过 Mapper 工厂获取
都是通过 `Mappers.getMapper(xxx.class)` 的方式来进行对应 Mapper 的获取。此种方法为通过 Mapper 工厂获取。<br />如果是此种方法，约定俗成的是在接口内定义一个接口本身的实例 `INSTANCE`， 以方便获取对应的实例。
```java
@Mapper
public interface SourceMapper {

    SourceMapper INSTANCE = Mappers.getMapper(SourceMapper.class);

    // ......
}
```
这样在调用的时候，就不需要在重复的去实例化对象了。类似下面
```java
Target target = SourceMapper.INSTANCE.source2target(source);
```
<a name="OwP8U"></a>
### 5.2 使用依赖注入
对于 Web 开发，依赖注入应该很熟悉。MapSturct 也支持使用依赖注入，同时也推荐使用依赖注入。<br />![2021-09-04-13-56-50-326334.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630735202680-a37476f7-4e83-4697-bdd1-aabbef7f8670.png#clientId=u00ec64f5-dd4a-4&from=ui&id=u64435d46&originHeight=157&originWidth=763&originalType=binary&ratio=1&size=8060&status=done&style=shadow&taskId=u320283f4-ed1a-4936-93ce-110c0089a4d)
```java
@Mapper(componentModel = "spring")
```
<a name="eAWGF"></a>
### 5.3 依赖注入策略
可以选择是通过构造方法或者属性注入，默认是属性注入。
```java
public enum InjectionStrategy {

    /** Annotations are written on the field **/
    FIELD,

    /** Annotations are written on the constructor **/
    CONSTRUCTOR
}
```
类似如此使用
```java
@Mapper(componentModel = "cdi" injectionStrategy = InjectionStrategy.CONSTRUCTOR)
```
<a name="Xchpv"></a>
### 5.4 自定义类型转换
有时候，在对象转换的时候可能会出现这样一个问题，就是源对象中的类型是Boolean类型，而目标对象类型是String类型，这种情况可以通过`@Mapper`的uses属性来实现：
```java
@Data
public class User {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private Boolean isDisable;
}

@Data
public class UserDto implements Serializable {
    private Integer id;
    private String name;
    private String address;
    private Date birth;
    private String isDisable;
}

@Mapper(uses = {BooleanStrFormat.class})
public interface UserMapper {
    UserDto userToNewUserDto(User user);
}

public class BooleanStrFormat {
    public String toStr(Boolean isDisable) {
        if (isDisable) {
            return "Y";
        } else {
            return "N";
        }
    }
    public Boolean toBoolean(String str) {
        if (str.equals("Y")) {
            return true;
        } else {
            return false;
        }
    }
}
```
要注意的是，如果使用了例如像spring这样的环境，Mapper引入uses类实例的方式将是自动注入，那么这个类也应该纳入Spring容器
```java
@Test
public void userPoToUserDto() {
    User user =new User();
    user.setId(1);
    user.setName("myx");
    user.setAddress("河北沧州");
    user.setBirth(new Date());
    user.setIsDisable(true);
    SubUser subUser =new SubUser();
    subUser.setDeleted(0);
    subUser.setName("rkw");
    UserMapper mapper = Mappers.getMapper(UserMapper.class);
    UserDto userDto = mapper.userToNewUserDto(user);
    System.out.println(userDto);
}
```
