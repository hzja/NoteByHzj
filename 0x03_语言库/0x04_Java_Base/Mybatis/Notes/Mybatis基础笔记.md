Mybatis  
<a name="wyZlo"></a>
# Mybatis学习思维导图
![Mybatis笔记.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565020296655-0c60dab6-ccc6-47b4-8a30-2a9fd197e62b.png#averageHue=%23f2f2f7&height=10630&id=yHdMW&originHeight=10630&originWidth=5318&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4790988&status=done&style=none&title=&width=5318)

附件下载<br />[Mybatis笔记.png](https://www.yuque.com/attachments/yuque/0/2019/png/396745/1565020307357-3803dd45-bfee-4b4a-b5bd-583d1d07ab10.png?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fpng%2F396745%2F1565020307357-3803dd45-bfee-4b4a-b5bd-583d1d07ab10.png%22%2C%22name%22%3A%22Mybatis%E7%AC%94%E8%AE%B0.png%22%2C%22size%22%3A4790988%2C%22ext%22%3A%22png%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1565020278039-12%22%2C%22type%22%3A%22image%2Fpng%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22gpQqQ%22%2C%22card%22%3A%22file%22%7D)
<a name="Xkx7w"></a>
# 1、Mybatis-config配置文件
<a name="VR67g"></a>
## properties标签-引入配置文件
MyBatis可以使用properties标签来引入外部properties配置文件的内容<br />resource：引入类路径下的资源<br />url：引入网络路径或磁盘路径下的资源
```xml
<properties resource="db.properties"></properties>
```
<a name="Wgqhn"></a>
## settings标签
MyBatis中极为重要的调整设置，他们会改变MyBatis的运行时行为<br />setting：为settings的子标签，用来设置每一个设置项<br />name：设置项名<br />value：设置项有效值

| 设置参数 | 描述 | 有效值 | 默认值 |
| --- | --- | --- | --- |
| cacheEnabled | 该配置影响的所有映射器中配置的缓存的全局开关 | true<br />false | TRUE |
| lazyLoadingEnabled | 延迟加载的全局开关。当开启时，所有关联对象都会延迟加载。特定关联关系中可以通过设置fetchType属性来覆盖该项的开关状态 | true<br />false | FALSE |
| useColumnLabel | 使用列标签代替列名。不同的驱动在这方面会有不同的表现，具体可以参考相关驱动文档或通过测试这两种不同的模式来观察所用驱动的结果 | true<br />false | TRUE |
| defaultStatementTimeout | 设置超时时间。它决定驱动等待数据库响应的秒数 | Any<br />positive<br />integer | Not <br />Set(null) |
| mapUnderscoreToCamelCase | 是否开启自动驼峰命名规则（camel case）映射。即从经典数据库列名A_COLUMN到经典Java属性名aColumn的类似映射 | true<br />false | FALSE |

<a name="p55TP"></a>
## `typeAliases`标签**-配置Mapper映射别名**
`typeAliases`：别名处理器，可以为的java类型起别名<br />`typeAlias`：`typeAliases`的子标签，为每一个java类型起别名<br />`type`：指定要起别名的类型的全类名，默认别名为类名小写<br />`alias`：指定该全类名对应的新的别名<br />`package`：`typeAliases`的子标签，为某个包下的所有类批量起别名<br />`name`：指定包名，然后会自动为当前包以及下面所有的后代包的每一  个类都起一个默认别名（类名小写）<br />在使用package批量起别名时可能会在不同包内出现相同的类名，为避免重复别名，可以在其中一个类前使用`@Alias("xxx")`注解为该类取一个新的别名<br />别名不区分大小写，但是最好 按照大小写区分填写，提高代码可读性<br />MyBatis为Java的一些类指定了别名，在为自己的类指定别名时不要与这些别名冲突<br />Alias Mapped Type<br />_byte byte<br />_long long<br />_short short<br />_int int<br />_integer int<br />_double double<br />_float float<br />_boolean boolean<br />string String<br />byte Byte<br />long Long<br />short Short<br />int Integer<br />integer Integer<br />double Double<br />float Float<br />boolean Boolean<br />date Date<br />decimal BigDecimal<br />bigdecimal BigDecimal<br />object Object<br />map Map<br />hashmap HashMap<br />list List<br />arraylist ArrayList<br />collection Collection<br />iterator Iterator
<a name="Ooa3G"></a>
## `typeHandlers`标签
类型处理器，负责进行Java中的类型与数据库存储的数据类型的转换等，如把Java的String转成数据库的varchar类型等
<a name="uKN3a"></a>
## `environments`标签：配置环境-配置多环境并在不同情况使用(比如测试和开发使用不同)
MyBatis可以配置多种环境，environments标签的default属性用于指定某种环境，可以达到快速切换环境的目的，用处在于可以方便的切换开发环境与测试环境。<br />`environment`：environments的子标签，用于配置一个具体的环境信息，id代表当前环境的唯一标识符，必须有以下两个标签：<br />`transactionManager`：事务管理器<br />type：事务管理器的类型有JDBC | MANAGED两种取值<br />自定义事务管理器：实现`TransactionFactory`接口，type为全类名<br />dataSource：数据源；<br />type：数据源类型；UNPOOLED | POOLED | JNDI<br />自定义数据源：实现`DataSourceFactory`接口，type就是全类名
```xml
<environments default="development">
    <environment id="development">
        <transactionManager type="JDBC"/>
        <!-- 配置数据库连接信息 -->
        <dataSource type="POOLED">
            <!-- value属性值引用db.properties配置文件中配置的值 -->
            <property name="driver" value="${driver}"/>
            <property name="url" value="${url}"/>
            <property name="username" value="${name}"/>
            <property name="password" value="${password}"/>
        </dataSource>
    </environment>
</environments>
```
<a name="6fn9D"></a>
## `databaseProvider`标签
支持多数据库厂商的查询，即同时支持MySQL，SQL Server，Oracle等数据库
```xml
type="DB_VENDOR"：VendorDatabaseIdProvider
```
作用就是得到数据库厂商的标识（驱动：通过Connection的`getMetaData()`方法获取`DatabaseMetaData`对象，再通过该对象调用`getDatabaseProductName()`方法就可以返回数据库厂商标识了），MyBatis就能根据数据库厂商标识来执行不同的sql<br />在全局配置文件中这样配置
```xml
<databaseIdProvider type="DB_VENDOR">
    <!-- 为不同的数据库厂商起别名 -->
    <property name="MySQL" value="mysql"/>
    <property name="Oracle" value="oracle"/>
    <property name="SQL Server" value="sqlserver"/>
</databaseIdProvider>
```
在Mapper中的select等sql标签中加入databaseId属性，属性值写给该SQL语句对应的数据库厂商名起的别名即可
```xml
<select id="getEmpById" resultType="bean.Employee" databaseId="mysql">
		select * from tbl_employee where id = #{id}
</select>
<select id="getEmpById" resultType="bean.Employee" databaseId="oracle">
		select EMPLOYEE_ID id,LAST_NAME lastName,EMAIL email,GENDER gender from employees where id = #{id}
</select> 
```
<a name="xZDVV"></a>
## Mappers标签
将SQL映射注册到全局配置中<br />mapper：mappers的子标签，用于注册一个sql映射<br />注册配置文件：<br />resource：引用类路径下的sql映射文件<br />url：引用网络路径或者磁盘路径下的sql映射文件<br />注册接口<br />class：引用（注册）接口<br />两种方法：

1. 有sql映射文件(Mapper.xml)，映射文件名必须和接口同名，并且放在与接口同一目录下
2. 没有sql映射文件(Mapper.xml)，所有的SQL都是利用注解写在接口上的，用法如下：

在全局配置文件中直接将接口`EmployeeMapperAnnotation`配置为mapper
<a name="yZ0xb"></a>
## Mybatis中configuration属性配置顺序
在全局配置文件中的configuration标签中配置属性时需要遵循以下顺序进行配置：<br />properties?, settings?, typeAliases?, typeHandlers?, objectFactory?, objectWrapperFactory?, reflectorFactory?, plugins?, environments?, databaseIdProvider?, mappers?
<a name="lZgzW"></a>
# 2、Mapper配置文件详解
<a name="rLnxE"></a>
## 常用标签
| 标签名 | 标签作用 |
| --- | --- |
| cache | 命名空间的二级缓存配置 |
| cache-ref | 其他命名空间缓存配置的引用 |
| resultMap | 自定义结果集映射 |
| parameterMap | （已废弃）老式风格的参数映射 |
| sql | 抽取可重用语句块 |
| insert | 映射插入语句 |
| update | 映射更新语句 |
| delete | 映射删除语句 |
| select | 映射查询语句 |

MyBatis允许增删改直接定义以下类型的返回值：<br />Integer、Long、boolean、void<br />MyBatis会自动进行封装<br />当使用`sqlSessionFactory.openSession()`方式获取`openSession`时数据不会自动提交，需要自行调用`openSession.commit()`方法进行手动提交；若使用`sqlSessionFactory.openSession(true)`方式获取`openSession`时，会自动提交数据，就不用手动提交了
<a name="46bjd"></a>
## 获取自增主键
MySQL支持自增主键，自增主键值的获取，MyBatis也是利用`statement.getGenreatedKeys()`<br />在insert标签中有一个`useGeneratedKeys`属性，将其设置为"true"则使用自增主键获取主键值的策略；还有一个keyProperty属性，指定对应的主键属性，也就是MyBatis获取到主键值以后，将这个值封装给javaBean的哪个属性，例子如下，这样在添加操作完成后，MyBatis会将自增主键的值赋给employee的id属性
```xml
<insert id="addEmp" parameterType="bean.Employee" databaseId="mysql" useGeneratedKeys="true" keyProperty="id">
		insert into tbl_employee(last_name,email,gender) values(#{lastName},#{email},#{gender})
</insert>
```
Oracle不支持自增主键：Oracle使用序列来模拟自增；<br />每次插入的数据的主键是从序列中拿到的值，如何获取到这个值？<br />用selectKey标签
<a name="CNuP1"></a>
## 参数处理
当Mapper接口中方法传入参数时，针对不同情况，MyBatis会进行不同处理。
<a name="YVOEU"></a>
### 单个参数
MyBatis不会做特殊处理，使用`#{参数名}`就可以取出参数值，但是即使`#{}`中写的不是参数名也能正常运行，也能达到与`#{参数名}`相同的效果，但是为了规范以及提升可读性，要使用`#{参数名}`
<a name="fTEaJ"></a>
### 多个参数
<a name="uvRmW"></a>
#### 命名参数
传入多个参数时，在每个参数前加一个@Param标签
```java
public Employee getEmpByIdAndLastName(@Param("id")Integer id, @Param("lastName")String lastName);
```
这样就能在写SQL语句时使用`#{@Param中的参数}`获取参数值了，此时key为在`@Param`中写的字符串
<a name="YRTqm"></a>
#### POJO
如果多个参数正好是业务逻辑的数据模型，就可以直接传入POJO<br />`#{属性名}`：取出传入的POJO的属性值
<a name="LC5VD"></a>
#### Map
如果多个参数不是业务模型中的数据，没有对应的POJO，为了方便，也可以传入map<br />`#{key}`：取出map中对应的值<br />如果多个参数不是业务模型中的数据，但是经常要使用，推荐来编写一个TO（Transfer Object）数据传输对象
```java
Page{
    int index;
    int size;
}
```
PS：全局配置`useActualParamName`（JDK1.8）时name=参数名
<a name="dPx50"></a>
#### `#{}`与`${}`
`#{}`与`${}`都可以用于获取map中的值或者POJO对象属性的值<br />区别：

1. `#{}`：是以预编译的形式，将参数设置到SQL语句中，相当于`PreparedStatement`，防止SQL注入
2. `${}`：取出的值直接拼装在SQL语句中，会存在安全问题

大多数情况下，取参数的值都是用#{}的，在原生JDBC不支持使用占位符的地方就可以使用${}进行取值<br />比如分表：按照年份表拆分用`select * from ${year}_salary`<br />因为`${year}`所在的位置不能用?，因为`${year}_salary`才是一个完整的表名<br />`#{}`在进行取值的时候可以规定参数的一些规则：<br />参数位置支持的属性：javaType、jdbcType、mode、numericScale、resultMap、typeHandler、jdbcTypeName、expression<br />jdbcType通常需要在某种特定的条件下被设置：<br />在数据为null的时候，有些数据库可能不能识别MyBatis对null的默认处理，比如Oracle就会报错JdbcType OTHER：无效的类型。因为MyBatis对所有的null都映射的是Jdbc的OTHER类型，Oracle不认识，需要加一个jdbcType=NULL，如`#{email,jdbcType=NULL}`，这样当传入参数是null时就是jdbcType里的null类型，可以传入Oracle<br />或者在全局配置里配置jdbcTypeForNull=NULL，这样配置
<a name="Ji7Pq"></a>
## select标签
Select元素来定义查询操作。<br />id：唯一标识符：用来引用这条语句，需要和接口的方法名一致<br />parameterType：参数类型：可以不传，MyBatis会根据TypeHandler自动判断<br />resultType：返回值类型：别名或者全类名，如果返回的是集合，定义集合中元素的类型。不能和resultMap同时使用<br />resultMap：外部resultMap的命名引用，和resultType属性不能同时使用
<a name="kkycS"></a>
### 一对一查询使用association标签
<a name="lVXk0"></a>
### 一对多查询使用collection标签
<a name="5GADG"></a>
## 动态SQL
<a name="ZZ5az"></a>
### if标签
查询的时候因为后面的if判断后拼接的判断条件都要带and连接，所以必须在where后首先接上1=1，防止出错，或者使用where标签
<a name="poGBr"></a>
### where标签
MyBatis会将where标签中拼装的SQL语句中多余出来的and和or去掉，但是where只会去掉第一个多出来的and或者or，所以要把and或者or写在前面
<a name="csFd0"></a>
### trim标签
prefix：前缀，给拼串后的整个字符串加一个前缀<br />prefixOverrides：前缀覆盖，去掉整个字符串前面多余的字符<br />suffix：后缀，给拼串后的整个字符串加一个后缀<br />suffixOverrides：后缀覆盖，去掉整个字符串后面多余的字符
<a name="ZzGpW"></a>
### choose标签
分支选择：相当于switch-case，含子标签when, otherwise
<a name="PAXhI"></a>
### set标签-UPDATE子标签
更新指定项
```xml
<!-- public void updateEmp(Employee employee); -->
<update id="updateEmp">
  	update tbl_employee
<set>
    <if test="lastName!=null">
    	last_name=#{lastName},
    </if>
    <if test="email!=null">
    	email=#{email},
    </if>
    <if test="gender!=null">
      gender=#{gender}
  </if>
</set>
	where id=#{id}
</update>
```

1. 注意：set中的if中写的设置语句后的逗号","不能省略，不然就无法一次更新多个信息了

想达到此效果也可用trim标签完成，只需要将原来的set处的标签改为
```xml
<trim prefix="set" suffixOverrides=","></trim>
```

<a name="Xogk6"></a>
### foreach标签

1. collection：指定要遍历的集合
2. list类型的参数会特殊处理封装在map中，map的key就叫list
3. item：将当前遍历出的元素赋值给指定的变量
4. separator：表示每个元素与每个元素之间的分隔符
5. open：为遍历出的结果拼接一个开始的字符
6. close：为遍历出的结果拼接一个结束的字符
7. index：索引。遍历list的时候index表示的就是索引，item就是map的值；遍历map的时候index表示的就是map的key，item就是map的值
<a name="eZH7L"></a>
### bind标签
增删改查标签内还有一个bind子标签，是绑定的意思<br />作用：可以将OGNL表达式绑定到一个变量中，方便后来引用这个变量的值
```xml
<bind name="_lastName" value="'%'+lastName+'%'">
```

这样就相当于在原先的lastName的值的两端加上了%，方便使用like查询，这样在接下来写就可以写like #{_lastName}了，在Java的方法中也不用再每次都手动加%了<br />不推荐使用这种方式，要是模糊查询还是每次在Java方法中自己加%、_等符号比较好，这样查询的操作自由度要更高，规则有变的情况下不需要更改的配置值
<a name="enpFl"></a>
### sql标签与include标签
与增删改查标签同级，可以将使用频率比较高的sql语句部分写入sql标签中
```xml
<sql id="insertColumn">
		employee_id,last_name,email
</sql>

insert into employees(
		<include refid="insertColumn"></include>
)
```
当数据库类型不同，导致需要保存的东西不同时，可以在sql标签内写<if>子标签判断_databaseId即可<br />在<include>标签内还可以用`<property name="xxx" value="aaa">`标签自定义一些属性，在<sql>标签中可以用${xxx(name属性值)}来调用，注意是$不是#
<a name="SJDx7"></a>
## Mybatis内置参数
<a name="ge6Mw"></a>
### _parameter
代表整个参数

1. 单个参数：_parameter就是这个参数
2. 多个参数：参数会被封装为一个map：_parameter就是代表这个map
<a name="CZcrM"></a>
### _databaseId
如果在全局配置文件mybatis-config.xml中配置了databaseIdProvider标签，那么_databaseId就是代表当前数据库的别名<br />在不同的数据库会有不同的操作时，可以用if标签区分，<br /><if test="_databaseId=='mysql'">......</if><br /><if test="_databaseId=='oracle'">......</if>
<a name="j9uHe"></a>
## Mybatis缓存
MyBatis系统中默认定义了两级缓存<br />一级缓存和二级缓存

1. 默认情况下，只有一级缓存（SqlSession级别的缓存，也称为本地缓存）开启
2. 二级缓存需要手动开启和配置，他是基于namespace级别的缓存
3. 为了提高扩展性。MyBatis定义了缓存接口Cache。可以通过实现Cache接口来自定义二级缓存
<a name="aJutG"></a>
### 一级缓存
一级缓存也称为本地缓存，它是一直开启的<br />与数据库同一次会话期间查询到的数据会放在本地缓存中。以后如果需要获取相同的数据，直接从缓存中拿，没必要再去查询数据库<br />从缓存中获取的对象地址是相等的<br />一级缓存失效的情况<br />SqlSession不同，如果同一个查询用的是两个不同的SqlSession对象，那么就会向数据库重新发送SQL语句进行查询<br />SqlSession相同，但是查询条件不同（一级缓存中没有这个数据）<br />SqlSession相同，但是两次查询之间执行了增删改操作（可能使数据库的数据产生变化，需要重新查询）<br />SqlSession相同，手动清除了缓存（调用了openSession.clearCache();方法将缓存清空）
<a name="rmf6N"></a>
### 二级缓存
二级缓存也称为全局缓存，是基于namespace级别的缓存，一个namespace对应一个二级缓存<br />工作机制

1. 一个会话，查询一条数据，这个数据就会被放在当前会话的一级缓存中
2. 如果会话关闭，一级缓存中的数据才会被保存到二级缓存中；新的会话查询信息就可以参照二级缓存中的内容
3. sqlSession用EmployeeMapper查询的Employee和用DepartmentMapper查询的Department放在不同的namespace中，不同namespace查出的数据放在自己对应的缓存（map）中
<a name="9Pm02"></a>
### 使用步骤

1. 开启全局二级缓存配置（此设置不影响一级缓存，设为false也不会关闭一级缓存）

在settings中配置
```xml
<setting name="cacheEnabled" value="true"/>
```
即可开启全局二级缓存配置（默认开启，保险起见再配置下）<br />去mapper.xml中配置使用二级缓存<br />配置`<cache></cache>`标签即可<br />cache中的属性含义

1. eviction：缓存的回收策略（默认是LRU）
   1. LRU：最近最少使用的，即移除最长时间不被使用的对象
   2. FIFO：先进先出，即按照对象进入缓存的顺序来移除它们
   3. SOFT：软引用，移除基于垃圾回收器状态和软引用规则的对象
   4. WEAK：弱引用，更积极地移除基于垃圾回收器状态和弱引用规则的对象
2. flushInterval：缓存刷新间隔，即缓存多长时间清空一次，可以自行设置，单位是毫秒（默认不清空）
3. readOnly：是否只读（默认false）

true：只读，这样MyBatis认为所有从缓存中获取数据的操作都是只读操作，不会修改数据，MyBatis为了加快获取速度，会直接将数据在缓存中的引用交给用户。不安全，速度快<br />false：非只读，MyBatis认为获取的数据可能会被修改，MyBatis会利用序列化&反序列化技术克隆一份相同的数据。安全，速度慢

4. size：指定缓存中存放多少元素
5. type：指定自定义缓存的全类名，实现Cache接口就可以使用POJO需要实现序列化接口Serializable
<a name="kwx7w"></a>
### 缓存相关设置/属性

1. `<setting name="cacheEnabled" value="true"/>`设为"false"时二级缓存无法使用，但是不会关闭一级缓存
2. 每个select标签都有`useCache`属性，一般来说默认为"true"，改为"false"时二级缓存不使用，一级缓存依旧可以使用
3. 每个增删改标签都有`flushCache`属性，默认为"true"，所以增删改执行完成之后就会清除缓存，一级二级都会清除；查询标签其实也有，只是默认值是"false"，可以自行修改为"true"，这样每次查询之前就都会清除缓存了，相当于不使用缓存
4. `sqlSession.clearCache()`方法只是清除当前session的一级缓存，不影响二级缓存
5. `localCacheScope`：本地缓存作用域（一级缓存SESSION），当前会话的所有数据保存在会话缓存中

STATEMENT：可以禁用一级缓存<br />缓存的查询顺序：

1. 二级缓存
2. 一级缓存
3. 数据库
