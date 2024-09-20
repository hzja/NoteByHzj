<a name="qwnJe"></a>
## 1、MapStruct官网
[MapStruct官网](https://mapstruct.org/)：[https://mapstruct.org/](https://mapstruct.org/)<br />[MapStruct官网示例](https://github.com/mapstruct/mapstruct-examples)：[https://github.com/mapstruct/mapstruct-examples](https://github.com/mapstruct/mapstruct-examples)<br />Mapstruct是一种代码生成器，它极大地简化了基于"约定优于配置"方法的Java bean类型之间映射的实现。生成的映射代码使用纯方法调用，因此快速、类型安全且易于理解。<br />约定优于配置，也称作按约定编程，是一种软件设计范式，旨在减少软件开发人员需做决定的数量，获得简单的好处，而又不失灵活性。
<a name="yFKmK"></a>
## 2、MapStruct的使用场景
主要用于处理不同业务分层中产生的实体类如在数据存储层，使用DO来抽象一个业务实体；在业务逻辑层，使用DTO来表示数据传输对象；到了展示层，又把对象封装成VO来与前端进行交互。字段相差不大而定义了多重类。通过使用get/set方法进行取值赋值。
```java
personDTO.setName(personDO.getName());
personDTO.setAge(personDO.getAge());
personDTO.setSex(personDO.getSex());
personDTO.setBirthday(personDO.getBirthday());
```
<a name="Tm2R6"></a>
## 3、MapStruct的使用
<a name="KMQMF"></a>
### A、需要进行转换的实体类
```java
public class PersonDO {
    private Integer id;
    private String name;
    private int age;
    private Date birthday;
    private String gender;
}
```
```java
public class PersonDTO {
    private String userName;
    private Integer age;
    private Date birthday;
    private Gender gender;
}
```
<a name="FzfVU"></a>
### B、引入依赖
```xml
...
<properties>
  <org.mapstruct.version>1.4.1.Final</org.mapstruct.version>
</properties>
...

<dependencies>
  <dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct</artifactId>
    <version>${org.mapstruct.version}</version>
  </dependency>
  <dependency>
    <groupId>org.mapstruct</groupId>
    <artifactId>mapstruct-processor</artifactId>
    <version>${org.mapstruct.version}</version>
    <scope>provided</scope>
  </dependency>
</dependencies>
```
因为MapStruct需要在编译器生成转换代码，所以需要在maven-compiler-plugin插件中配置上对mapstruct-processor的引用。
<a name="sOQKG"></a>
### C、定义一个做映射的接口
使用注解`@Mapper`定义一个`Converter`接口，在其中定义一个do2dto方法，方法的入参类型是PersonDO，出参类型是PersonDTO，这个方法就用于将PersonDO转成PersonDTO。
```java
@Mapper
interface PersonConverter {
    PersonConverter INSTANCE = Mappers.getMapper(PersonConverter.class);
    @Mappings(@Mapping(source = "name", target = "userName"))
    PersonDTO do2dto(PersonDO person);
}
```
<a name="z6gbs"></a>
### D、测试代码
```java
public static void main(String[] args) {
    PersonDO personDO = new PersonDO();
    personDO.setName("Fcant");
    personDO.setAge(18);
    personDO.setBirthday(new Date());
    personDO.setId(1);
    personDO.setGender(Gender.MALE.name());
    PersonDTO personDTO = PersonConverter.INSTANCE.do2dto(personDO);
    System.out.println(personDTO);
}
```
输出结果：
```
PersonDTO{userName='Fcant', age=18, birthday=Sat Aug 08 19:00:44 CST 2020, gender=MALE}
```
<a name="fz1Ez"></a>
## 4、MapStruct处理字段映射
要转换的两个类中源对象属性与目标对象属性的类型和名字一致的时候，会自动映射对应属性。
<a name="itjyP"></a>
### A、名字不一致时的映射
在PersonDO中用name表示用户名称，而在PersonDTO中使用userName表示用户名。<br />这时候就要使用`@Mapping`注解了，只需要在方法签名上，使用该注解，并指明需要转换的源对象的名字和目标对象的名字就可以了，如将name的值映射给userName，可以使用如下方式：
```java
@Mapping(source = "name", target = "userName")
```
<a name="JXpnn"></a>
### B、可以自动映射的类型
MapStruct会对部分类型自动做映射，不需要做额外配置。<br />一般情况下，对于以下情况可以做自动类型转换：

- 基本类型及其包装类型。
- 基本类型的包装类型和String类型之间
- String类型和枚举类型之间
<a name="SJgzc"></a>
### C、自定义常量
在转换映射过程中，给一些属性定义一个固定的值，这个时候可以使用 constant
```java
@Mapping(source = "name", constant = "Fcant")
```
<a name="uUcpP"></a>
### D、类型不匹配时的映射
如果需要在Person这个对象中增加家庭住址这个属性，那么一般在PersonoDTO中会单独定义一个HomeAddress类来表示家庭住址，而在Person类中，一般使用String类型表示家庭住址。这就需要在HomeAddress和String之间使用JSON进行互相转化，这种情况下，MapStruct也是可以支持的。
```java
public class PersonDO {
    private String name;
    private String address;
}
```
```java
public class PersonDTO {
    private String userName;
    private HomeAddress address;
}
```
```java
@Mapper
interface PersonConverter {
    PersonConverter INSTANCE = Mappers.getMapper(PersonConverter.class);

    @Mapping(source = "userName", target = "name")
    @Mapping(target = "address",expression = "java(homeAddressToString(dto2do.getAddress()))")
    PersonDO dto2do(PersonDTO dto2do);

    default String homeAddressToString(HomeAddress address){
        return JSON.toJSONString(address);
    }
}
```
只需要在`PersonConverter`中在定义一个方法（因为`PersonConverter`是一个接口，所以在JDK 1.8以后的版本中可以定义一个`default`方法），这个方法的作用就是将`HomeAddress`转换成String类型。
> `default`方法：Java 8 引入的新的语言特性，用关键字`default`来标注，被`default`所标注的方法，需要提供实现，而子类可以选择实现或者不实现该方法

然后在dto2do方法上，通过以下注解方式即可实现类型的转换：
```java
@Mapping(target = "address",expression = "java(homeAddressToString(dto2do.getAddress()))")
```
上面这种是自定义的类型转换，还有一些类型的转换是MapStruct本身就支持的，如String和Date之间的转换：
```java
@Mapping(target = "birthday",dateFormat = "yyyy-MM-dd HH:mm:ss")
```
<a name="YmKLb"></a>
## 5、MapStruct的性能
MapStruct在编译时生成bean映射，这确保了高性能，可以提前将问题反馈出来，也使得开发人员可以彻底的错误检查。<br />在代码中使用了很多MapStruct提供的注解，这使得在编译期，MapStruct就可以直接生成bean映射的代码，相当于MapStruct写了很多setter和getter。<br />如在代码中定义了以下一个Mapper
```java
@Mapper
interface PersonConverter {
    PersonConverter INSTANCE = Mappers.getMapper(PersonConverter.class);

    @Mapping(source = "userName", target = "name")
    @Mapping(target = "address",expression = "java(homeAddressToString(dto2do.getAddress()))")
    @Mapping(target = "birthday",dateFormat = "yyyy-MM-dd HH:mm:ss")
    PersonDO dto2do(PersonDTO dto2do);

    default String homeAddressToString(HomeAddress address){
        return JSON.toJSONString(address);
    }
}
```
经过代码编译后，会自动生成一个PersonConverterImpl：
```java
@Generated(
    value = "org.mapstruct.ap.MappingProcessor",
    date = "2020-08-09T12:58:41+0800",
    comments = "version: 1.3.1.Final, compiler: javac, environment: Java 1.8.0_181 (Oracle Corporation)"
)

class PersonConverterImpl implements PersonConverter {
    @Override
    public PersonDO dto2do(PersonDTO dto2do) {
        if ( dto2do == null ) {
            return null;
        }
        PersonDO personDO = new PersonDO();
        personDO.setName( dto2do.getUserName() );
        if ( dto2do.getAge() != null ) {
            personDO.setAge( dto2do.getAge() );
        }

        if ( dto2do.getGender() != null ) {
            personDO.setGender( dto2do.getGender().name() );
        }
        personDO.setAddress( homeAddressToString(dto2do.getAddress()) );
        return personDO;
    }
}
```
在运行期，对于bean进行映射的时候，就会直接调用PersonConverterImpl的dto2do方法，这样就没有什么特殊的事情要做了，只是在内存中进行set和get就可以了。因为在编译期做了很多事情，所以MapStruct在运行期的性能会很好，并且还有一个好处，那就是可以把问题的暴露提前到编译期。使得如果代码中字段映射有问题，那么应用就会无法编译，强制开发者要解决这个问题才行。
