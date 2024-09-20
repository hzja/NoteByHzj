Java SpringBoot<br />很多时候，在做公司系统或产品时，都需要自己创建用户管理体系，这对于开发人员来说并不是什么难事，但是当需要维护多个不同系统并且相同用户跨系统使用的情况下，如果每个系统维护自己的用户信息，那么此时用户信息的同步就会变的比较麻烦，对于用户自身来说也会非常困扰，很容易出现不同系统密码不一致等情况出现。<br />如果此时引入LDAP来集中存储用户的基本信息并提供统一的读写接口和校验机制，那么这样的问题就比较容易解决了。尤其在一些内部管理系统的开发和搭建时，往往内部系统一开始并不全是自己开发的，还有很多第三方产品支持，比如：OA系统、财务系统等，如果自己开发一套用户管理系统，那么这些系统对接还得二次开发，成本很大。由于LDAP并不是什么新技术，大部分成熟软件都支持用LDAP来管理用户，所以时至今日，LDAP的应用依然可以经常看到。<br />下面就具体来看看，当使用Spring Boot开发的时候，如何来访问LDAP服务端。
<a name="p14a3"></a>
### LDAP简介
LDAP（轻量级目录访问协议，Lightweight Directory Access Protocol)是实现提供被称为目录服务的信息服务。目录服务是一种特殊的数据库系统，其专门针对读取，浏览和搜索操作进行了特定的优化。目录一般用来包含描述性的，基于属性的信息并支持精细复杂的过滤能力。目录一般不支持通用数据库针对大量更新操作操作需要的复杂的事务管理或回卷策略。而目录服务的更新则一般都非常简单。这种目录可以存储包括个人信息、web链结、jpeg图像等各种信息。为了访问存储在目录中的信息，就需要使用运行在TCP/IP 之上的访问协议—LDAP。<br />LDAP目录中的信息是是按照树型结构组织，具体信息存储在条目(entry)的数据结构中。条目相当于关系数据库中表的记录；条目是具有区别名DN （Distinguished Name）的属性（Attribute），DN是用来引用条目的，DN相当于关系数据库表中的关键字（Primary Key）。属性由类型（Type）和一个或多个值（Values）组成，相当于关系数据库中的字段（Field）由字段名和数据类型组成，只是为了方便检索的需要，LDAP中的Type可以有多个Value，而不是关系数据库中为降低数据的冗余性要求实现的各个域必须是不相关的。LDAP中条目的组织一般按照地理位置和组织关系进行组织，非常的直观。LDAP把数据存放在文件中，为提高效率可以使用基于索引的文件数据库，而不是关系数据库。类型的一个例子就是mail，其值将是一个电子邮件地址。<br />LDAP的信息是以树型结构存储的，在树根一般定义国家(c=CN)或域名(dc=com)，在其下则往往定义一个或多个组织 (organization)(o=Acme)或组织单元(organizational units) (ou=People)。一个组织单元可能包含诸如所有雇员、大楼内的所有打印机等信息。此外，LDAP支持对条目能够和必须支持哪些属性进行控制，这是有一个特殊的称为对象类别(objectClass)的属性来实现的。该属性的值决定了该条目必须遵循的一些规则，其规定了该条目能够及至少应该包含哪些属性。例如：inetorgPerson对象类需要支持sn(surname)和cn(common name)属性，但也可以包含可选的如邮件，电话号码等属性。<br />LDAP简称对应

- o：organization（组织-公司）
- ou：organization unit（组织单元-部门）
- c：countryName（国家）
- dc：domainComponent（域名）
- sn：surname（姓氏）
- cn：common name（常用名称）
<a name="b1vLM"></a>
### 入门示例
在了解了LDAP的基础概念之后，通过一个简单例子进一步理解！

- 创建一个基础的Spring Boot项目
- 在pom.xml中引入两个重要依赖
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-ldap</artifactId>
</dependency>

<dependency>
    <groupId>com.unboundid</groupId>
    <artifactId>unboundid-ldapsdk</artifactId>
    <scope>test</scope>
</dependency>
```
其中，spring-boot-starter-data-ldap是Spring Boot封装的对LDAP自动化配置的实现，它是基于spring-data-ldap来对LDAP服务端进行具体操作的。<br />而unboundid-ldapsdk主要是为了在这里使用嵌入式的LDAP服务端来进行测试操作，所以scope设置为了test，实际应用中，通常会连接真实的、独立部署的LDAP服务器，所以不需要此项依赖。

- 在src/test/resources目录下创建ldap-server.ldif文件，用来存储LDAP服务端的基础数据，以备后面的程序访问之用。
```
dn: dc=didispace,dc=com
objectClass: top
objectClass: domain
objectclass: extensibleObject
dc: didispace

dn: ou=people,dc=didispace,dc=com
objectclass: top
objectclass: organizationalUnit
ou: people

dn: uid=ben,ou=people,dc=didispace,dc=com
objectclass: top
objectclass: person
objectclass: organizationalPerson
objectclass: inetOrgPerson
cn: didi
sn: zhaiyongchao
uid: didi
userPassword: {SHA}nFCebWjxfaLbHHG1Qk5UU4trbvQ=
```
这里创建了一个基础用户，真实姓名为zhaiyongchao，常用名didi，在后面的程序中，会来读取这些信息。更多内容解释大家可以深入学习LDAP来理解，这里不做过多的讲解。

- 在application.properties中添加嵌入式LDAP的配置
```
spring.ldap.embedded.ldif=classpath:ldap-server.ldif
spring.ldap.embedded.base-dn=dc=didispace,dc=com
```

- 使用spring-data-ldap的基础用法，定义LDAP中属性与Java中定义实体的关系映射以及对应的Repository
```java
@Data
@Entry(base = "ou=people,dc=didispace,dc=com", objectClasses = "inetOrgPerson")
public class Person {

    @Id
    private Name id;
    @DnAttribute(value = "uid", index = 3)
    private String uid;
    @Attribute(name = "cn")
    private String commonName;
    @Attribute(name = "sn")
    private String userName;
    private String userPassword;

}

public interface PersonRepository extends CrudRepository<Person, Name> {

}
```
通过上面的定义之后，已经将Person对象与LDAP存储内容实现了映射，只需要使用`PersonRepository`就可以轻松的对LDAP内容实现读写。

- 创建单元测试用例读取所有用户信息：
```java
@Slf4j
@SpringBootTest
public class ApplicationTests {

    @Autowired
    private PersonRepository personRepository;

    @Test
    public void findAll() {
        personRepository.findAll().forEach(p -> {
            System.out.println(p);
        });
    }
}
```
启动该测试用例之后，可以看到控制台中输出了刚才维护在ldap-server.ldif中的用户信息：
```java
Person(id=uid=ben,ou=people,dc=didispace,dc=com, uid=ben, commonName=didi, userName=zhaiyongchao, userPassword=123,83,72,65,125,110,70,67,101,98,87,106,120,102,97,76,98,72,72,71,49,81,107,53,85,85,52,116,114,98,118,81,61)
```
<a name="z0hnQ"></a>
#### 添加用户
通过上面的入门示例，如果能够独立完成，那么在Spring Boot中操作LDAP的基础目标已经完成了。<br />如果足够了解Spring Data，其实不难想到，这个在其下的子项目必然也遵守Repsitory的抽象。所以，可以使用上面定义的`PersonRepository`来轻松实现操作，比如下面的代码就可以方便的往LDAP中添加用户：
```java
Person person = new Person();
person.setUid("uid:1");
person.setSuerName("AAA");
person.setCommonName("aaa");
person.setUserPassword("123456");
personRepository.save(person);
```
如果还想实现更多操作，可以参考spring-data-ldap的文档来进行使用。
<a name="P4vOi"></a>
#### 连接LDAP服务端
在本文的例子中都采用了嵌入式的LDAP服务器，事实上这种方式也仅限于本地测试开发使用，真实环境下LDAP服务端必然是独立部署的。<br />在Spring Boot的封装下，只需要配置下面这些参数就能将上面的例子连接到远端的LDAP而不是嵌入式的LDAP。
```
spring.ldap.urls=ldap://localhost:1235
spring.ldap.base=dc=didispace,dc=com
spring.ldap.username=didispace
spring.ldap.password=123456
```
