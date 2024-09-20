SpringBoot MongoDB
<a name="CrXAP"></a>
## 一、介绍
MongoDB 中有三个比较重要的名词：**数据库、集合、文档**！

- **数据库（Database）**：和关系型数据库一样，每个数据库中有自己的用户权限，不同的项目组可以使用不同的数据库
- **集合（Collection）**：集合指的是文档组（类似于 MySQL 中的表的概念），里面可以存储许多文档
- **文档（Document）**：文档是 MongoDB 中最基本的数据单元，由键值对组成，类似于 JSON 格式，可以存储不同字段，字段的值可以包括其他文档、数组和文档数组

明白这三个名词，基本就可以上手了，以 Java 语言为例，详细的介绍 Java 客户端改如何操作 MongoDB ！
<a name="ajQZV"></a>
## 二、Java 客户端
<a name="4yiiF"></a>
### 2.1、MongoDB 连接
Java 连接 MongoDB 服务器，与常用的连接关系型数据库方式类似！<br />标准 URI 连接语法：
```
mongodb://username:password@host1:port1,host2:port2,...,hostN:portN/database?options
```
参数说明：

- `mongodb://`：这是固定的格式，必须要指定
- `username:password@`：可选项，如果设置，在连接数据库服务器之后，驱动都会尝试登录这个数据库
- `host1:port1`：主机IP和端口号，必须指定至少一个host。如果要连接复制集，请指定多个主机地址
- `/database`：如果指定了`username:password@`，连接时会验证并登录指定数据库。若不指定，默认打开 test 数据库
- `?options`：连接可选项，例如`connectTimeoutMS=5000ms`，表示连接等待时间最长 5 秒

例如，无密码连接 MongoDB
```
mongodb://127.0.0.1:27017
```
使用用户名`test`，密码`test`登录 MongoDB 的`test_db`数据库
```
mongodb://test:test@127.0.0.1:27017/test_db
```
无密码连接指定三台服务器 (端口 27017, 27018, 和27019)
```
mongodb://127.0.0.1:27017,127.0.0.1:27018,127.0.0.1:27019
```
<a name="I212h"></a>
### 2.2、添加依赖包
和关系型数据库操作一样，在 Java 程序中如果要使用 MongoDB，先要添加对应的 MongoDB JDBC 驱动器，然后才能继续操作！<br />在选择的版本号的时候，注意需要和服务器中的安装版本匹配，具体的版本号选择可以从如下地址中获取到最新的版本：mongo-java-driver
```xml
<dependency>
    <groupId>org.mongodb</groupId>
    <artifactId>mongo-java-driver</artifactId>
    <version>3.12.7</version>
</dependency>
```
<a name="p64jO"></a>
### 2.3、连接数据库
连接数据库的 Java 代码如下：
```java
private static MongoClient mongoClient;
static {
    try {
        //连接到MongoDB服务 如果是远程连接可以替换“localhost”为服务器所在IP地址
        mongoClient = MongoClients.create("mongodb://test:test@127.0.0.1:27017/test_db");
        System.out.println("Connect to database successfully");
    } catch (Exception e) {
        System.err.println( e.getClass().getName() + ": " + e.getMessage() );
    }
}
```
上文中采用的是用户名和密码方式登录数据库，因为数据库已经开启了权限验证，因此需要通过用户名和密码才能登录。
<a name="P1MBe"></a>
### 2.4、创建集合
`com.mongodb.client`包里面都集成好了大部分常用的 api，可以通过`MongoDatabase`类中的`createCollection()`方法来创建集合。<br />创建一个`tb_role`集合，代码片段如下：
```java
/**
 * 创建集合
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    mongoDatabase.createCollection("tb_role");
    System.out.println("集合创建成功");
}
```
如果想删除集合，操作也很简单！
```java
/**
 * 删除集合
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    mongoDatabase.getCollection("tb_role").drop();
    System.out.println("集合 tb_role 删除成功");
}
```
<a name="sCTjE"></a>
### 2.5、创建文档
可以通过`MongoCollection`类中的`insertMany()`方法来插入一个文档。<br />向`tb_role`集合中插入文档，代码片段如下：
```java
/**
 * 插入文档
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    //获取集合
    MongoCollection<Document> collection = mongoDatabase.getCollection("tb_role");
    //向集合中插入文档
    Document document = new Document("name", "管理员").
            append("code", "manager").
            append("sort", 100);
    List<Document> documents = new ArrayList<>();
    documents.add(document);
    collection.insertMany(documents);
    System.out.println("文档插入成功");
}
```
<a name="Ctvov"></a>
### 2.6、更新文档
可以通过`MongoCollection`类中的`updateMany()`方法来更新集合中的文档。<br />更新`tb_role`集合中的指定文档信息，代码片段如下：
```java
/**
 * 更新文档
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    //获取集合
    MongoCollection<Document> collection = mongoDatabase.getCollection("tb_role");
    //更新文档
    collection.updateMany(Filters.eq("code", "manager"), new Document("$set",new Document("name","经理")));
    //遍历所有文档
    FindIterable<Document> findIterable = collection.find();
    MongoCursor<Document> mongoCursor = findIterable.iterator();
    while(mongoCursor.hasNext()){
        System.out.println(mongoCursor.next());
    }
}
```
上文操作类似于 SQL语句中的`update tb_role set name = '经理' where code = 'manager'`！
<a name="ca556e8a"></a>
### 2.7、删除文档
可以通过`MongoCollection`类中的`deleteOne()`或者`deleteMany`方法来删除集合中的文档。<br />删除`tb_role`集合中的指定文档信息，代码片段如下：
```java
/**
 * 删除文档
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    //获取集合
    MongoCollection<Document> collection = mongoDatabase.getCollection("tb_role");
    //删除符合条件的第一个文档
    collection.deleteOne(Filters.eq("code", "manager"));
    //删除所有符合条件的文档
    collection.deleteMany (Filters.eq("code", "manager"));
    //遍历所有文档
    FindIterable<Document> findIterable = collection.find();
    MongoCursor<Document> mongoCursor = findIterable.iterator();
    while(mongoCursor.hasNext()){
        System.out.println(mongoCursor.next());
    }
}
```
在这里需要注意一下，在删除的时候，官方推荐尽可能先查询一下这个数据是否存在，如果存在，然后在执行删除操作，可以调用`findOneAndDelete()`方法进行删除！
<a name="Lm6jH"></a>
### 2.8、查询文档
`MongoCollection`类中提供了非常丰富的检索文档的方法，例如，可以通过`find()`方法来获取集合中的所有文档。<br />检索`tb_role`集合中的文档信息，代码片段如下：
```java
/**
 * 查询文档
 * @param args
 */
public static void main(String[] args) {
    // 连接到数据库
    MongoDatabase mongoDatabase = mongoClient.getDatabase("test_db");
    System.out.println("Connect to database successfully");
    //获取集合
    MongoCollection<Document> collection = mongoDatabase.getCollection("tb_role");
    //遍历所有文档
    FindIterable<Document> findIterable = collection.find();
    MongoCursor<Document> mongoCursor = findIterable.iterator();
    while(mongoCursor.hasNext()){
        System.out.println(mongoCursor.next());
    }
    //查询当前集合所有文档数量
    long count = collection.countDocuments();
    System.out.println("当前文档数量:" + count);
    //带条件遍历文档
    FindIterable<Document> documentFindIterable = collection.find(Filters.eq("code", "manager"));
    MongoCursor<Document> documentMongoCursor = documentFindIterable.iterator();
    while(documentMongoCursor.hasNext()){
        System.out.println(documentMongoCursor.next());
    }
}
```
<a name="QfNRR"></a>
## 三、SpringBoot 整合 MongoDB
采用 SpringBoot 来对接 MongoDB，下面就以常用的几个操作为例来介绍，过程如下！
<a name="ba4409d2"></a>
### 3.1、创建 SpringBoot 工程，添加 MongoDB 依赖包
```xml
<!-- 引入springboot -->
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.1.0.RELEASE</version>
</parent>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-mongodb</artifactId>
</dependency>
```
<a name="vNWBN"></a>
### 3.2、添加配置文件
在`application.properties`文件中添加`mongodb`配置
```
#配置数据库连接地址
spring.data.mongodb.uri=mongodb://test123:test123@127.0.0.1:27017/test_db
```
相关参数说明：

- userName：表示用户名，根据实际情况填写即可
- password：表示用户密码，根据实际情况填写即可
- dbName：表示数据库，可以自定义，初始化数据的时候，会自动创建
<a name="SwCvC"></a>
### 3.3、创建实体类
创建用于示例中测试的实体类`Person`。<br />其中注解`@Document(collection="persons")`用于指定当前文档属于集合`persons`。<br />注解`@Id`表示当前`id`字段属于主键类型。
```java
/**
 * 使用@Document注解指定集合名称
 */
@Document(collection="persons")
public class Person implements Serializable {
    private static final long serialVersionUID = -3258839839160856613L;
 /**
     * 使用@Id注解指定MongoDB中的 _id 主键
     */
    @Id
    private Long id;
    private String userName;
    private String passWord;
    private Integer age;
    private Date createTime;
    //...get/set
    @Override
    public String toString() {
        return "Person{" +
                "id=" + id +
                ", userName='" + userName + '\'' +
                ", passWord='" + passWord + '\'' +
                ", age=" + age +
                ", createTime=" + createTime +
                '}';
    }
}
```
<a name="s7Kz7"></a>
### 3.4、操作 MongoDB
Springboot 操作 MongoDB 有两种方式。

- 第一种方式是采用 Springboot 官方推荐的 JPA 方式，这种操作方式，使用简单但是灵活性比较差。
- 第二种方式是采用 Spring Data MongoDB 封装的 MongoDB 官方 Java 驱动 MongoTemplate 对 MongoDB 进行操作，这种方式非常灵活，能满足绝大部分需求。

这里将采用第二种方式进行介绍！
<a name="f51c40b0"></a>
### 3.5、插入文档
`MongoTemplate`提供了`insert()`方法，用于插入文档，示例代码如下：
<a name="fnChw"></a>
#### 插入文档
> 没指定集合名称时，会取`@Document`注解中的集合名称

```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 插入文档
     * @throws Exception
     */
    @Test
    public void insert() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        mongoTemplate.insert(person);
    }
}
```
<a name="mY7Vq"></a>
#### 自定义集合名称，插入文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 自定义集合，插入文档
     * @throws Exception
     */
    @Test
    public void insertCustomCollection() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        mongoTemplate.insert(person, "custom_person");
    }
}
```
<a name="9VXgy"></a>
#### 自定义集合，批量插入文档
> 如果采用批量插入文档，必须指定集合名称

```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 自定义集合，批量插入文档
     * @throws Exception
     */
    @Test
    public void insertBatch() throws Exception {
        List<Person> personList = new ArrayList<>();
        Person person1 =new Person();
        person1.setId(10l);
        person1.setUserName("张三");
        person1.setPassWord("123456");
        person1.setCreateTime(new Date());
        personList.add(person1);
        Person person2 =new Person();
        person2.setId(11l);
        person2.setUserName("李四");
        person2.setPassWord("123456");
        person2.setCreateTime(new Date());
        personList.add(person2);
        mongoTemplate.insert(personList, "custom_person");
    }
}
```
<a name="Tx8Nc"></a>
### 3.6、存储文档
`MongoTemplate`提供了`save()`方法，用于存储文档。<br />在存储文档的时候会通过主键ID进行判断，如果存在就更新，否则就插入，示例代码如下：
<a name="cVNBn"></a>
#### 存储文档，如果没有插入，否则通过`主键ID`更新
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 存储文档，如果没有插入，否则更新
     * @throws Exception
     */
    @Test
    public void save() throws Exception {
        Person person =new Person();
        person.setId(13l);
        person.setUserName("八八");
        person.setPassWord("123456");
        person.setAge(40);
        person.setCreateTime(new Date());
        mongoTemplate.save(person);
    }
}
```
<a name="qdAzQ"></a>
#### 自定义集合，存储文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 自定义集合，存储文档
     * @throws Exception
     */
    @Test
    public void saveCustomCollection() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        mongoTemplate.save(person, "custom_person");
    }
}
```
<a name="KBlfZ"></a>
### 3.7、更新文档
`MongoTemplate`提供了`updateFirst()`和`updateMulti()`方法，用于更新文档，示例代码如下：
<a name="Ji3fT"></a>
#### 更新文档，匹配查询到的文档数据中的第一条数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 更新文档，匹配查询到的文档数据中的第一条数据
     * @throws Exception
     */
    @Test
    public void updateFirst() throws Exception {
        //更新对象
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三123");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        //更新条件
        Query query= new Query(Criteria.where("id").is(person.getId()));
        //更新值
        Update update= new Update().set("userName", person.getUserName()).set("passWord", person.getPassWord());
        //更新查询满足条件的文档数据（第一条）
        UpdateResult result =mongoTemplate.updateFirst(query,update, Person.class);
        if(result!=null){
            System.out.println("更新条数：" + result.getMatchedCount());
        }
    }
}
```
<a name="bULsG"></a>
#### 更新文档，匹配查询到的文档数据中的所有数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 更新文档，匹配查询到的文档数据中的所有数据
     * @throws Exception
     */
    @Test
    public void updateMany() throws Exception {
        //更新对象
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        //更新条件
        Query query= new Query(Criteria.where("id").is(person.getId()));
        //更新值
        Update update= new Update().set("userName", person.getUserName()).set("passWord", person.getPassWord());
        //更新查询满足条件的文档数据（全部）
        UpdateResult result = mongoTemplate.updateMulti(query, update, Person.class);
        if(result!=null){
            System.out.println("更新条数：" + result.getMatchedCount());
        }
    }
}
```
<a name="eEWkO"></a>
### 3.8、删除文档
`MongoTemplate`提供了`remove()`、`findAndRemove()`和`findAllAndRemove()`方法，用于删除文档，示例代码如下：
<a name="O5HNk"></a>
#### 删除符合条件的所有文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 删除符合条件的所有文档
     * @throws Exception
     */
    @Test
    public void remove() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        Query query = new Query(Criteria.where("userName").is(person.getUserName()));
        DeleteResult result = mongoTemplate.remove(query, Person.class);
        System.out.println("删除条数：" + result.getDeletedCount());
    }
}
```
<a name="V9Ld2"></a>
#### 删除符合条件的单个文档，并返回删除的文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 删除符合条件的单个文档，并返回删除的文档
     * @throws Exception
     */
    @Test
    public void findAndRemove() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        Query query = new Query(Criteria.where("id").is(person.getId()));
        Person result = mongoTemplate.findAndRemove(query, Person.class);
        System.out.println("删除的文档数据：" + result.toString());
    }
}
```
<a name="xsPS9"></a>
#### 删除符合条件的所有文档，并返回删除的文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 删除符合条件的所有文档，并返回删除的文档
     * @throws Exception
     */
    @Test
    public void findAllAndRemove() throws Exception {
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        Query query = new Query(Criteria.where("id").is(person.getId()));
        List<Person> result = mongoTemplate.findAllAndRemove(query, Person.class);
        System.out.println("删除的文档数据：" + result.toString());
    }
}
```
<a name="gfbIU"></a>
### 3.9、查询文档
`MongoTemplate`提供了非常多的文档查询方法，日常开发中用的最多的就是`find()`方法，示例代码如下：
<a name="VFd8T"></a>
#### 查询集合中的全部文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 查询集合中的全部文档数据
     * @throws Exception
     */
    @Test
    public void findAll() throws Exception {
        List<Person> result = mongoTemplate.findAll(Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="XjuqP"></a>
#### 查询集合中指定的ID文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 查询集合中指定的ID文档数据
     * @throws Exception
     */
    @Test
    public void findById() {
        long id = 1l;
        Person result = mongoTemplate.findById(id, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="Apyj3"></a>
#### 根据条件查询集合中符合条件的文档，返回第一条数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据条件查询集合中符合条件的文档，返回第一条数据
     */
    @Test
    public void findOne() {
        String userName = "张三";
        Query query = new Query(Criteria.where("userName").is(userName));
        Person result = mongoTemplate.findOne(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="NuIII"></a>
#### 根据条件查询集合中符合条件的文档
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据条件查询集合中符合条件的文档
     */
    @Test
    public void findByCondition() {
        String userName = "张三";
        Query query = new Query(Criteria.where("userName").is(userName));
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="A6d5r"></a>
#### 根据【AND】关联多个查询条件，查询集合中的文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据【AND】关联多个查询条件，查询集合中的文档数据
     */
    @Test
    public void findByAndCondition() {
        // 创建条件
        Criteria criteriaUserName = Criteria.where("userName").is("张三");
        Criteria criteriaPassWord = Criteria.where("passWord").is("123456");
        // 创建条件对象，将上面条件进行 AND 关联
        Criteria criteria = new Criteria().andOperator(criteriaUserName, criteriaPassWord);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="yKECL"></a>
#### 根据【OR】关联多个查询条件，查询集合中的文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据【OR】关联多个查询条件，查询集合中的文档数据
     */
    @Test
    public void findByOrCondition() {
        // 创建条件
        Criteria criteriaUserName = Criteria.where("userName").is("张三");
        Criteria criteriaPassWord = Criteria.where("passWord").is("123456");
        // 创建条件对象，将上面条件进行 OR 关联
        Criteria criteria = new Criteria().orOperator(criteriaUserName, criteriaPassWord);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="dDAaE"></a>
#### 根据【IN】关联多个查询条件，查询集合中的文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据【IN】关联多个查询条件，查询集合中的文档数据
     */
    @Test
    public void findByInCondition() {
        // 设置查询条件参数
        List<Long> ids = Arrays.asList(1l, 10l, 11l);
        // 创建条件
        Criteria criteria = Criteria.where("id").in(ids);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="W8119"></a>
#### 根据【逻辑运算符】查询集合中的文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据【逻辑运算符】查询集合中的文档数据
     */
    @Test
    public void findByOperator() {
        // 设置查询条件参数
        int min = 20;
        int max = 35;
        Criteria criteria = Criteria.where("age").gt(min).lte(max);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="9iLU0"></a>
#### 根据【正则表达式】查询集合中的文档数据
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据【正则表达式】查询集合中的文档数据
     */
    @Test
    public void findByRegex() {
        // 设置查询条件参数
        String regex = "^张*";
        Criteria criteria = Criteria.where("userName").regex(regex);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="Fk3ve"></a>
#### 根据条件查询集合中符合条件的文档，获取其文档列表并排序
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 根据条件查询集合中符合条件的文档，获取其文档列表并排序
     */
    @Test
    public void findByConditionAndSort() {
        String userName = "张三";
        Query query = new Query(Criteria.where("userName").is(userName)).with(Sort.by("age"));
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="TOjVw"></a>
#### 根据单个条件查询集合中的文档数据，并按指定字段进行排序与限制指定数目
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
   /**
     * 根据单个条件查询集合中的文档数据，并按指定字段进行排序与限制指定数目
     */
    @Test
    public void findByConditionAndSortLimit() {
        String userName = "张三";
        //从第一行开始，查询2条数据返回
        Query query = new Query(Criteria.where("userName").is(userName)).with(Sort.by("createTime")).limit(2).skip(1);
        List<Person> result = mongoTemplate.find(query, Person.class);
        System.out.println("查询结果：" + result.toString());
    }
}
```
<a name="ir77Z"></a>
#### 统计集合中符合【查询条件】的文档【数量】
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
   /**
     * 统计集合中符合【查询条件】的文档【数量】
     */
    @Test
    public void countNumber() {
        // 设置查询条件参数
        String regex = "^张*";
        Criteria criteria = Criteria.where("userName").regex(regex);
        // 创建查询对象，然后将条件对象添加到其中
        Query query = new Query(criteria);
        long count = mongoTemplate.count(query, Person.class);
        System.out.println("统计结果：" + count);
    }
}
```
<a name="a9d916a3"></a>
### 4.0、创建索引
索引在所有的数据库中，暂居的位置非常重要，例如检索一张上百万的数据表的时候，如果没走索引，查询效率会极其缓慢，对于 MongoDB 来说，同样如此。<br />示例如下：
<a name="lKl14"></a>
#### 创建升序索引
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
    /**
     * 创建升序索引
     */
    @Test
    public void createAscendingIndex() {
        // 设置字段名称
        String field = "userName";
        // 创建索引
        mongoTemplate.getCollection("persons").createIndex(Indexes.ascending(field));
    }
}
```
<a name="TmJAV"></a>
#### 移除索引
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
   /**
     * 根据索引名称移除索引
     */
    @Test
    public void removeIndex() {
        // 设置字段名称
        String field = "userName";
        // 删除索引
        mongoTemplate.getCollection("persons").dropIndex(field);
    }
}
```
<a name="t9Mnm"></a>
#### 查询集合中所有的索引
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class PersonServiceTest {
    @Autowired
    private MongoTemplate mongoTemplate;
   /**
     * 查询集合中所有的索引
     */
    @Test
    public void getIndexAll() {
        // 获取集合中所有列表
        ListIndexesIterable<Document> indexList = mongoTemplate.getCollection("persons").listIndexes();
        // 获取集合中全部索引信息
        for (Document document : indexList) {
            System.out.println("索引列表：" + document);
        }
    }
}
```
<a name="eVtdt"></a>
#### 通过在实体类上加注解方式来创建索引
```java
/**
 * 使用@Document注解指定集合名称
 */
@Document(collection="persons")
public class Person implements Serializable {
    private static final long serialVersionUID = -3258839839160856613L;
 /**
     * 使用@Id注解指定MongoDB中的 _id 主键
     */
    @Id
    private Long id;
    private String userName;
    private String passWord;
    private Integer age;
 /**
     * 创建一个5秒之后文档自动删除的索引
     */
    @Indexed(expireAfterSeconds=5)
    private Date createTime;
    //...get/set
    @Override
    public String toString() {
        return "Person{" +
                "id=" + id +
                ", userName='" + userName + '\'' +
                ", passWord='" + passWord + '\'' +
                ", age=" + age +
                ", createTime=" + createTime +
                '}';
    }
}
```
<a name="tmSXF"></a>
### 4.1、引入 MongoDB 中的事务
单节点 mongodb 不支持事务，需要搭建 MongoDB 复制集。
```java
/**
 * 配置事务管理器
 *
 */
@Configuration
public class TransactionConfig {
    @Bean
    MongoTransactionManager transactionManager(MongoDatabaseFactory dbFactory) {
        return new MongoTransactionManager(dbFactory);
    }
}
```
事务服务测试！
```java
@Service
public class TransactionExample {
    @Autowired
    private MongoTemplate mongoTemplate;
    @Transactional(rollbackFor = Exception.class)
    public Object transactionTest(){
        Person person =new Person();
        person.setId(1l);
        person.setUserName("张三");
        person.setPassWord("123456");
        person.setCreateTime(new Date());
        Person newPerson = mongoTemplate.insert(person);
        // 抛出异常，观察数据是否进行回滚
        if(1 == 1){
            throw new RuntimeException("异常");
        }
        return newPerson;
    }
}
```
