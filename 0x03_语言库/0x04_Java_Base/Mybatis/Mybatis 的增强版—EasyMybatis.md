Mybatis<br />easy-mybatis是一个对Mybatis的增强框架（插件）。在Spring集成Mybatis的基础上，将项目开发中对数据库的常用操作统一化。使用本框架可以很便捷的对数据库进行操作，提高开发效率，减少机械行为。<br />Maven依赖引入
```xml
<!-- https://mvnrepository.com/artifact/top.zuoyu.mybatis/easy-mybatis-spring-boot-starter -->
<dependency>
	<groupId>top.zuoyu.mybatis</groupId>
	<artifactId>easy-mybatis-spring-boot-starter</artifactId>
	<version>1.0.0</version>
</dependency>
```
Gradle依赖引入
```groovy
// https://mvnrepository.com/artifact/top.zuoyu.mybatis/easy-mybatis-spring-boot-starter
implementation 'top.zuoyu.mybatis:easy-mybatis-spring-boot-starter:1.0.0'
```
<a name="Z9pcL"></a>
## 框架初衷
这个框架的初衷是，减少Java程序员千篇一律的数据库操作。<br />对于开发人员来说：

- 精力应该花费在业务逻辑上，而非机械式的“技术”上。
- 项目中减少无关痛痒的代码，从抽象的角度看实现。
- 各司其职，各劳其力，追求项目角度的服务流水线。
<a name="ndJJw"></a>
### 服务分离的时代
如今已很难看到单体架构的项目，目前的项目大都是通过RESTful、MQ、Socket的方式（协议）进行数据传输。<br />质疑传统JavaWeb项目中的数据库操作模式——即Model(DTO)存在的意义。理论上，数据库设计是不可能完全遵循视图模型的，这就导致“正确”的做法是在项目中引入VO，由多个DTO来组装。<br />那么，为什么不能用灵活的Map来替代呢？<br />对一个Map的方法进行拓展，增加其对Json的解析能力，那么是不是就可以摆脱POJO的各种麻烦组装。
<a name="tYgz1"></a>
### 思考框架设计
思考如何设计这个框架的时候，被需要考虑的方方面面给阻挡住了。<br />因为一个数据库框架需要考虑的东西实在太多了，比如：

- 事务机制
- 类型转换
- 会话管理

···<br />思来想去，发现自己方向跑偏了，只是希望统一数据库操作的接口 + 摆脱Model，没必要重新平地起墙，完全可以在一个现有的框架基础上进行封装。那么，对这个现有框架的选择就尤为重要了。
<a name="CxIT6"></a>
### 现有框架的选择
目前Java中主流的数据库操作框架：

- Spring JDBC
- Spring Data JPA
- Mybatis
- Hibernate

选择现有框架有一个原则——“统一数据库操作的接口 + 摆脱Model”是对该框架的加强，而非变异；不能因为“统一数据库操作的接口 + 摆脱Model”而无法使用原框架的部分功能。<br />“摆脱Model”这个特点，首先就要排除重度ORM框架，也就是支持JPA操作的数据库——Spring Data JPA、Hibernate；原因很简单，这两个框架的强大之处恰恰就在它完全面向Model操作。<br />剩下的就只有两个框架了，Spring JDBC和Mybatis。其中，Spring JDBC留给了开发人员大量的可操作空间，更加自由，但恰恰是这种自由使得它更加繁琐。<br />而Mybatis是一个轻量ORM框架，准确来说Mybatis不能称为ORM框架，因为它并不是面向Model操作数据库，仅仅是将数据库字段与Model字段互相赋值，并没有做到ORM定义的关系映射。
<a name="oEJT0"></a>
### 抉择
由以上各框架的特点，结合国内Java语言中数据库操作框架的热度，毫无疑问的选择了Mybatis。<br />考虑到SpringBoot对Mybatis优秀的支持级别，作者决定基于mybatis-spring-boot-starter开发这款框架，准备来说应该称其为“插件”。
<a name="xWhub"></a>
## 框架特性
<a name="npkZy"></a>
### 更便捷
摒弃传统mybatis的model、xml、dao数据库操作模式，避繁就简，快速开发。
<a name="MnkZw"></a>
### 更高效
采用预编译SQL，拒绝运行期间反射生成SQL，性能更高效。
<a name="wMYQZ"></a>
### 无侵入
只是对Mybatis-Spring的增强插件，对已有工程不做任何修改，仍可使用原生框架的功能，仅仅是简化了开发阶段对数据库的操作。
<a name="t8Y9c"></a>
### 统一操作接口
对数据库的所有操作共用一个接口，降低使用门槛，轻松操作数据库。
<a name="h7iHI"></a>
### 统一操作对象
使用JsonObject为数据对象，提供一系列操作方法，方便从持久化对象组装为视图对象。
<a name="XNdLK"></a>
### 易上手
整个框架只提供了一个接口、一个注解、两个对象，仅仅一行配置便可完成对数据库进行常用操作。<br />...
<a name="sR2zF"></a>
### 安利
在mybatis-spring-boot环境下，使用该框架（插件），可以减少传统Mybatis使用中对model、xml、dao的机械式开发。<br />所有的数据库操作均使用`MapperRepository`接口，通过注解`@Magic("xxx")`标记接口的数据表归属，即可直接使用。<br />该框架（插件）不妨碍同时使用传统Mybatis中model、xml、dao的数据库开发方式。
<a name="yp3dt"></a>
## 快速上手
<a name="VTtAs"></a>
### 安装
<a name="P9zVR"></a>
#### 安装mybatis-spring-boot环境
mybatis-spring-boot的Maven依赖
```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>${spring-boot.version}</version>
    <relativePath/>
</parent>

<dependencies>
    <dependency>
        <groupId>org.mybatis.spring.boot</groupId>
        <artifactId>mybatis-spring-boot-starter</artifactId>
        <version>${mybatis-spring-boot.version}</version>
    </dependency>
</dependencies>
```
mybatis-spring-boot的Gradle依赖
```groovy
plugins {
	id 'org.springframework.boot' version '${springBootVersion}'
	id 'io.spring.dependency-management' version '${springManagementVersion}'
	id 'java'
}

dependencies {
	implementation 'org.mybatis.spring.boot:mybatis-spring-boot-starter:${mybatisSpringVersion}'
}
```
<a name="wt8oQ"></a>
#### 安装本框架（插件）
Maven依赖引入
```xml
<!-- https://mvnrepository.com/artifact/top.zuoyu.mybatis/easy-mybatis-spring-boot-starter -->
<dependency>
	<groupId>top.zuoyu.mybatis</groupId>
	<artifactId>easy-mybatis-spring-boot-starter</artifactId>
	<version>1.0.0</version>
</dependency>
```
Gradle依赖引入
```groovy
// https://mvnrepository.com/artifact/top.zuoyu.mybatis/easy-mybatis-spring-boot-starter
implementation 'top.zuoyu.mybatis:easy-mybatis-spring-boot-starter:1.0.0'
```
<a name="a6zV1"></a>
### 配置
这里以MySQL数据库为例，Oracle数据库配置请参考配置说明
<a name="t48Fh"></a>
##### 配置spring-boot-jdbc数据库
```yaml
spring:
datasource:
type: com.zaxxer.hikari.HikariDataSource
driver-class-name: com.mysql.cj.jdbc.Driver
url: jdbc:mysql://172.0.0.1:3306/xxxx
username: xxxx
    password: xxxx
```
关于springBoot的配置，这里不多赘述，更多移步[https://spring.io/projects/spring-boot](https://spring.io/projects/spring-boot)
<a name="bIiyE"></a>
#### 配置easy-mybatis支持的表名（例子）
```yaml
easy-mybatis:
  table-names: teacher, student
```
这里的table-names配置，表示需要easy-mybatis框架支持的数据表名，多个表名使用逗号隔开。<br />即可使用easy-mybatis框架操作teacher和student两个数据表，如果需要支持其他数据表，需要在此配置。
<a name="vOx6H"></a>
### 操作数据库（例子）
```java
@SpringBootTest
class DemoApplicationTests {
    
    // 表示该接口用来操作名称为'teacher'的数据表
    @Magic("teacher")
    private MapperRepository teacherRepository;
    
    // 表示该接口用来操作名称为'student'的数据表
    @Magic("student")
    private MapperRepository studentRepository;
    
    
    // 查询teacher表下所有数据
    @Test
    void teacherTest() {
        teachertRepository.selectList().forEach(System.out::println);
    }
    
    // 查询student表下符合特定条件的数据
    @Test
    void studentTest() {
        studentRepository.selectListByExample(
            new JsonObject().put("birthday", "2009/12/12 12:12:12")
        ).forEach(System.out::println);
    }
    
}
```
使用`MapperRepository`接口对数据库进行操作，需要使用`@Magic("表名称")`标记该接口的数据表归属。<br />在本例中，`@Magic("teacher")`表示该`MapperRepository`为"teacher"数据表的操作接口，可以通过`teacherRepository`调用一系列方法完成对"teacher"数据表的操作。
<a name="vi7uo"></a>
## 配置说明
该框架（插件）的全部相关配置
```yaml
#easy-mybatis的相关配置
easy-mybatis:
  #表示需要载入的数据库表
  table-names: SYS_USER, SYS_DEPT
  #默认的日期字符串格式
  data-format: yyyy-MM-dd HH:mm:ss
  #当数据库为Oracle时，数据表对应的主键SEQ名称
  sequences:
    SYS_USER: SEQ_SYS_USER,
    SYS_DEPT: SEQ_SYS_DEPT
  #当数据库为Oracle时，数据表的日期类型字段格式
  oracle-date-format: yyyy-mm-dd hh24:mi:ss
```
<a name="Rol9C"></a>
### 详细说明
<a name="JdVA4"></a>
#### table-names
此配置表示需要载入的数据表，多个表名由逗号隔开，只有在此处配置了的数据表，才能在项目中使用`@Magic("表名称")`标记`MapperRepository`接口的数据表归属。
<a name="aYyyR"></a>
#### data-format
此配置表示在JsonObject中的日期存在格式，JsonObject中对日期的存储类型为String类型，可通过此配置自定义存储格式，默认格式为yyyy-MM-dd HH:mm:ss。
:::tips
注意：当data-format的格式修改，请务必将oracle-date-format的格式与之匹配。
:::
<a name="AUPSE"></a>
#### sequences
此配置表示当数据库为Oracle时，每个数据表的主键生成器名称，以键值对的格式进行参数配置。若此处没有对数据表的主键生成器进行配置，那么在做该数据表的数据插入行为时需自定义填充主键数值。
<a name="iN7OI"></a>
#### oracle-date-format
此配置表示当数据库为Oracle时，传入日期格式字段时的参数格式，即`TO_DATE(field, '$dateFormat')`。
:::tips
注意：当oracle-date-format的格式修改，请务必将data-format的格式与之匹配。
:::
<a name="eoHO7"></a>
## 操作接口约定
本框架（插件）的数据库操作唯一接口——top.zuoyu.mybatis.service.MapperRepository。
<a name="M9oXn"></a>
### 以ByExample为结尾的方法
此类是根据JsonObject类型的参数example中键值对，对符合要求的数据进行操作，例如：
```java
// 构建example，这里指字段"birthday"为"2009/12/12 12:12:12"
JsonObject example = new JsonObject().put("birthday", "2009/12/12 12:12:12");
// 查询所有字段"birthday"为"2009/12/12 12:12:12"的学生数据
List<JsonObject> studentList = studentRepository.selectListByExample(example);
```
这里的`selectListByExample`方法就是根据参数example中键值对查找符合要求的数据。<br />同理， `countByExample`方法就是获取符合参数example中键值对的数据总量...
<a name="mQaWb"></a>
### 以By为结尾的方法
此类是根据String类型的参数`suffixSql`对方法追加参数中SQL语句的内容，例如：
```java
// 查找"is_delete"字段为0的数据，并以年龄进行排序
List<JsonObject> teachers = teacherRepository.selectListBy("WHERE is_delete = 0 ORDER BY age");

// 将"birthday"字段大于"1990/1/1 00:00:00"的数据，"salary"字段改为20000。 （也就是出生日期在1990年之前的老师，工资改为2W）
JsonObject example = new JsonObject().put("salary", 20000);
int count = teacherRepository.updateBy(example, "WHERE birthday < '1990/1/1 00:00:00'");
```
这里的`selectListBy`方法就是查找出符合参数中SQL语句要求的内容，updateBy方法就是将符合参数中SQL语句要求的数据修改为example定义的数值。<br />同理，`countBy`方法就是获取符合参数中SQL语句要求的数据总量...
<a name="Ek47f"></a>
### `insert`、`insertBatch`方法

- 当数据库为MySQL时，如果数据表主键自增，则无需定义主键数值；若主键为非自增，则需定义主键数值。
- 当数据库为Oracle时，如果在配置中定义了数据表的主键生成器，则无需定义主键数值；反之，则需定义主键数值。

`MapperRepository`提供了丰富的方法，以方便对数据库的CURD操作，更多关于`MapperRepository`的使用说明请移步`MapperRepository`说明：<br />[https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/service/MapperRepository.html](https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/service/MapperRepository.html)
<a name="v89Bq"></a>
## 操作对象约定
数据库接口的操作对象主要为JsonObject类型和JsonArray类型。<br />其中top.zuoyu.mybatis.json.JsonObject为键值对容器，以键值的形式装载，数据结构为：
```json
{
  'field1' : 'value1',
  'field2' : 'value2',
  'field3' : 'value3',
  ...
}
```
键的类型为java.lang.String类型，值的类型可以为任何类型。<br />JsonObject提供了丰富的方法，以方便操作键值对和类型转换，更多关于JsonObject的使用说明请移步JsonObject说明、JsonObject的API文档。<br />[https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonObject.html](https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonObject.html)<br />其中top.zuoyu.mybatis.json.JsonArray为数组容器，以多个单位的形式装载，数据结构为：
```json
[
    object1,
    object2,
    object3,
    ...
]
```
每个单位可以为任何类型，且各单位类型允许与其他单位类型不同。<br />JsonArray提供了丰富的方法，以方便操作键值对和类型转换，更多关于JsonArray的使用说明请移步JsonArray说明、JsonArray的API文档。<br />[https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonArray.html](https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonArray.html)
<a name="KQWea"></a>
### MapperRepository说明
查询所有selectList
```java
/**
 * 查询所有
 *
 * @return 所有数据
 */
List<JsonObject> selectList();
```
查询符合样例的数据`selectListByExample`
```java
/**
 * 根据已有键值查询
 *
 * @param example - 已有键值
 * @return 符合要求的数据集合
 */
List<JsonObject> selectListByExample(JsonObject example);
```
根据主键查询唯一对象`selectByPrimaryKey`
```java
/**
 * 根据主键查询唯一对象
 *
 * @param primaryKey - 主键
 * @return 唯一对象
 */
JsonObject selectByPrimaryKey(Serializable primaryKey);
```
查询符合条件的数据`selectListBy`
```java
/**
 * 查询符合条件的数据
 *
 * @param suffixSql - 条件语句（例如：where field = xxx）
 * @return 数据数量
 */
List<JsonObject> selectListBy(String suffixSql);
```
查询特定的字段或结果`selectFields`
```java
/**
 * 查询特定的字段或结果
 * （例如："fieldA, fieldB, fieldC"）
 * （例如："COUNT(field)"）
 *
 * @param fields - 特定的字段或结果
 * @return 特定的字段或结果
 */
JsonArray selectFields(String fields);
```
根据样例查询特定的字段或结果`selectFieldsByExample`
```java
/**
 * 根据已有键值查询特定的字段或结果
 * （例如："fieldA, fieldB, fieldC"）
 * （例如："COUNT(field)"）
 *
 * @param fields  - 特定的字段或结果
 * @param example - 已有键值
 * @return 特定的字段或结果
 */
JsonArray selectFieldsByExample(String fields, JsonObject example);
```
根据主键查询特定的字段或结果`selectFieldsByPrimaryKey`
```java
/**
 * 根据主键查询特定的字段或结果
 *
 * @param fields     - 特定的字段或结果（例如："fieldA, fieldB, fieldC"）
 * @param primaryKey - 主键
 * @return 特定的字段或结果
 */
JsonArray selectFieldsByPrimaryKey(String fields, Serializable primaryKey);
```
根据主键集合查询特定的字段或结果`selectFieldsByPrimaryKeys`
```java
/**
 * 根据主键集合查询特定的字段或结果
 *
 * @param fields      - 特定的字段或结果（例如："fieldA, fieldB, fieldC"）
 * @param primaryKeys - 主键集合
 * @return 特定的字段或结果
 */
JsonArray selectFieldsByPrimaryKeys(String fields, Serializable[] primaryKeys);
```
查询符合条件的数据`selectFieldsBy`
```java
/**
 * 查询符合条件的数据
 *
 * @param fields    - 特定的字段或结果（例如："fieldA, fieldB, fieldC"）
 * @param suffixSql - 条件语句（例如：where field = xxx）
 * @return 特定的字段或结果
 */
JsonArray selectFieldsBy(String fields, String suffixSql);
```
查询符合条件的数据数量`countBy`
```java
/**
 * 查询符合条件的数据数量
 *
 * @param suffixSql - 条件语句（例如：where field = xxx）
 * @return 数据数量
 */
long countBy(String suffixSql);
```
查询是否存在符合样例的数据数量`countByExample`
```java
/**
 * 根据已有键值查询是否存在符合条件的数据数量
 *
 * @param example - 已有键值
 * @return 数据数量
 */
long countByExample(JsonObject example);
```
是否存在符合条件的数据`existsBy`
```java
/**
 * 是否存在符合条件的数据
 *
 * @param suffixSql - 条件语句（例如：where field = xxx）
 * @return 是否存在
 */
boolean existsBy(String suffixSql);
```
是否存在符合样例的数据`existsByExample`
```java
/**
 * 根据已有键值查询是否存在符合条件的数据
 *
 * @param example - 已有键值
 * @return 是否存在
 */
boolean existsByExample(JsonObject example);
```
新增对象`insert`
```java
/**
 * 新增对象
 *
 * @param jsonObject - 对象键值
 * @return 变动数据的数量
 */
int insert(JsonObject jsonObject);
```
批量新增对象`insertBatch`
```java
/**
 * 批量新增对象
 *
 * @param jsonObjects - 对象键值集合
 * @return 变动数据的数量
 */
int insertBatch(List<JsonObject> jsonObjects);
```
根据主键修改对象属性`updateByPrimaryKey`
```java
/**
 * 根据主键修改对象属性
 *
 * @param jsonObject - 包含主键对象键值
 * @return 变动数据的数量
 */
int updateByPrimaryKey(JsonObject jsonObject);
```
修改特定条件的对象属性`updateBy`
```java
/**
 * 修改特定条件的对象属性
 *
 * @param jsonObject - 要修改的键值
 * @param suffixSql  - 条件语句（例如：where field = xxx）
 * @return 变动数据的数量
 */
int updateBy(JsonObject jsonObject, String suffixSql);
```
批量根据主键修改对象属性`updateByPrimaryKeyBatch`
```java
/**
 * 批量根据主键修改对象属性
 *
 * @param jsonObjects - 对象键值集合
 * @return 变动数据的数量
 */
int updateByPrimaryKeyBatch(List<JsonObject> jsonObjects);
```
根据主键删除对象`deleteByPrimaryKey`
```java
/**
 * 根据主键删除对象
 *
 * @param primaryKey - 主键
 * @return 变动数据的数量
 */
int deleteByPrimaryKey(Serializable primaryKey);
```
删除符合条件的数据`deleteBy`
```java
/**
 * 删除符合条件的数据
 *
 * @param suffixSql - 条件语句（例如：where field = xxx）
 * @return 变动数据的数量
 */
int deleteBy(String suffixSql);
```
批量根据主键删除对象`deleteByPrimaryKeys`
```java
/**
 * 批量根据主键删除对象
 *
 * @param primaryKeys - 主键组
 * @return 变动数据的数量
 */
int deleteByPrimaryKeys(Serializable[] primaryKeys);
```
根据样例删除对象`deleteByExample`
```java
/**
 * 根据已有键值删除对象
 *
 * @param example - 已有键值
 * @return 变动数据的数量
 */
int deleteByExample(JsonObject example);
```
<a name="c1CaF"></a>
### JsonObject说明
top.zuoyu.mybatis.json.JsonObject为键值对容器，以键值的形式装载，数据结构为：
```json
{
    'field1' : 'value1',
    'field2' : 'value2',
    'field3' : 'value3',
    ...
}
```
键的类型为java.lang.String类型，值的类型可以为任何类型。
<a name="WEv1v"></a>
#### 类型转换
内部默认已包含常规的类型转换，默认支持以下类型：

- java.math.BigInteger
- java.math.BigDecimal
- java.lang.Boolean
- java.lang.Byte
- java.lang.Character
- java.util.Date
- java.lang.Double
- java.lang.Float
- java.lang.Integer
- java.lang.Long
- java.lang.Short
- java.lang.String

如果默认的转换器无法满足需求，或者需要转换为其他类型，可通过自定义转换器`ConvertClass<T>`进行实现，接口方法：
```java
public interface ConvertClass<T> {

    /**
     * 将给定的 value 对象转换为 T 类型
     *
     * @param value - 给定对象
     * @return 目标转换类型的对象
     */
    T convert(Object value);

    /**
     * 将给定的 value 对象转换为 T 类型， 否则返回  defaultValue
     *
     * @param value        - 给定对象
     * @param defaultValue - 默认值
     * @return 目标转换类型的对象
     */
    T convert(Object value, T defaultValue);
}
```
<a name="zUvkq"></a>
#### 示例
```java
@Test
void convertTest() {
    // 使用JSON字符串初始化对象
    JsonObject student = new JsonObject(studentJSON);

    // 获取学生的性别，如果无法转换则返回"未知"
    String sex = student.getValue("sex", new SexConvert());

    // 获取学生的性别，如果无法转换则返回"未录入"
    String sexOrDefault = student.getValue("sex", new SexConvert(), "未录入");
}

static class SexConvert implements ConvertClass<String> {

    @Override
    public String convert(Object value) {
        if (value instanceof Integer) {
            if ((Integer) value == 1) {
                return "男";
            } else if ((Integer)value == 0) {
                return "女";
            }
        }
        return "未知";
    }

    @Override
    public String convert(Object value, String defaultValue) {
        if (value instanceof Integer) {
            if ((Integer) value == 1) {
                return "男";
            } else if ((Integer)value == 0) {
                return "女";
            }
        }
        return defaultValue;
    }
}
```
本示例也可使用`StringConvert`转换器，`StringConvert`为`ConvertClass<T>`的子接口。<br />也可以通过`toClass`方法，将`JsonObject`对象转换为指定类型的实体类。<br />子接口：

- BigDecimalConvert
- BigIntegerConvert
- BooleanConvert
- ByteConvert
- CharacterConvert
- DateConvert
- DoubleConvert
- FloatConvert
- IntegerConvert
- LongConvert
- ShortConvert
- StringConvert

更多方法请移步JsonObject的API文档。<br />[https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonObject.html](https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonObject.html)
<a name="r5SoG"></a>
### JsonArray说明
top.zuoyu.mybatis.json.JsonArray为数组容器，以多个单位的形式装载，数据结构为：
```json
[
    object1,
    object2,
    object3,
    ...
]
```
每个单位可以为任何类型，且各单位类型允许与其他单位类型不同。
<a name="xhL1S"></a>
#### 类型转换
具体使用方法与JsonObject在类型转换操作一致，参考JsonObject的类型转换操作<br />更多方法请移步JsonArray的API文档。<br />[https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonArray.html](https://mybatis.zuoyu.top/doc/index.html?top/zuoyu/mybatis/json/JsonArray.html)
<a name="J0BCa"></a>
## 开源地址

- 项目主页：[https://mybatis.zuoyu.top](https://mybatis.zuoyu.top)
- API文档地址：[https://mybatis.zuoyu.top/doc/index.html](https://mybatis.zuoyu.top/doc/index.html)
- GitHub地址：[https://github.com/zuoyuip/easy-mybatis](https://github.com/zuoyuip/easy-mybatis)
- Gitee地址：[https://gitee.com/zuoyuip/easy-mybatis](https://gitee.com/zuoyuip/easy-mybatis)
