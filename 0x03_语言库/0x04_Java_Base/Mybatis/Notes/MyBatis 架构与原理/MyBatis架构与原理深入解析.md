Java Mybatis
<a name="QxtPF"></a>
## 1、引言
主要讲解JDBC怎么演变到Mybatis的渐变过程，重点讲解了为什么要将JDBC封装成Mybaits这样一个持久层框架。再而论述Mybatis作为一个数据持久层框架本身有待改进之处。
<a name="fmGeO"></a>
## 2、JDBC实现查询分析
先看看最熟悉也是最基础的通过JDBC查询数据库数据，一般需要以下七个步骤：

1. 加载JDBC驱动；
2. 建立并获取数据库连接；
3. 创建 JDBC Statements 对象；
4. 设置SQL语句的传入参数；
5. 执行SQL语句并获得查询结果；
6. 对查询结果进行转换处理并将处理结果返回；
7. 释放相关资源（关闭`Connection`，关闭`Statement`，关闭`ResultSet`）；

以下是具体的实现代码：
```java
public static List<Map<String,Object>> queryForList(){  
    Connection connection = null;  
    ResultSet rs = null;  
    PreparedStatement stmt = null;  
    List<Map<String,Object>> resultList = new ArrayList<Map<String,Object>>();  
          
    try {  
        // 加载JDBC驱动  
        Class.forName("oracle.jdbc.driver.OracleDriver").newInstance();  
        String url = "jdbc:oracle:thin:@localhost:1521:ORACLEDB";  
              
        String user = "trainer";   
        String password = "trainer";   
              
        // 获取数据库连接  
        connection = DriverManager.getConnection(url,user,password);   
              
        String sql = "select * from userinfo where user_id = ? ";  
        // 创建Statement对象（每一个Statement为一次数据库执行请求）  
        stmt = connection.prepareStatement(sql);  
              
        // 设置传入参数  
        stmt.setString(1, "zhangsan");  
              
        // 执行SQL语句  
        rs = stmt.executeQuery();  
              
        // 处理查询结果（将查询结果转换成List<Map>格式）  
        ResultSetMetaData rsmd = rs.getMetaData();  
        int num = rsmd.getColumnCount();  
              
        while(rs.next()){  
            Map map = new HashMap();  
            for(int i = 0;i < num;i++){  
                String columnName = rsmd.getColumnName(i+1);  
                map.put(columnName,rs.getString(columnName));  
            }  
            resultList.add(map);  
        }  
              
    } catch (Exception e) {  
        e.printStackTrace();  
    } finally {  
        try {  
            // 关闭结果集  
            if (rs != null) {  
                rs.close();  
                rs = null;  
            }  
            // 关闭执行  
            if (stmt != null) {  
                stmt.close();  
                stmt = null;  
            }  
            if (connection != null) {  
                connection.close();  
                connection = null;  
            }  
        } catch (SQLException e) {  
            e.printStackTrace();  
        }  
    }        
    return resultList;  
}
```
<a name="VEAF4"></a>
## 3、JDBC演变到Mybatis过程
上面看到了实现JDBC有七个步骤，哪些步骤是可以进一步封装的，减少了开发的代码量。

<a name="kHehw"></a>
### 3.1 第一步优化：连接获取和释放

1. **问题描述：**

数据库连接频繁的开启和关闭本身就造成了**资源的浪费，影响系统的性能**。<br />**解决问题：**<br />数据库连接的获取和关闭**可以使用数据库连接池来解决资源浪费的问题**。通过连接池就可以反复利用已经建立的连接去访问数据库了。减少连接的开启和关闭的时间。

1. **问题描述：**

但是现在**连接池多种多样，可能存在变化**，有可能采用DBCP的连接池，也有可能采用容器本身的JNDI数据库连接池。<br />**解决问题：**<br />可以**通过DataSource进行隔离解耦**，统一从DataSource里面获取数据库连接，**DataSource具体由DBCP实现还是由容器的JNDI实现都可以**，所以将DataSource的具体实现通过让用户配置来应对变化。
<a name="iclhh"></a>
### 3.2 第二步优化：SQL统一存取

1. **问题描述：**

使用JDBC进行操作数据库时，**SQL语句基本都散落在各个JAVA类中**，这样有三个不足之处：<br />第一，可读性很差，不利于维护以及做性能调优。<br />第二，改动Java代码需要重新编译、打包部署。<br />第三，不利于取出SQL在数据库客户端执行（取出后还得删掉中间的Java代码，编写好的SQL语句写好后还得通过＋号在Java进行拼凑）。<br />**解决问题：**<br />可以考虑不把SQL语句写到Java代码中，那么把SQL语句放到哪里呢？首先需要有一个统一存放的地方，可以将这些**SQL语句统一集中放到配置文件或者数据库里面（以key-value的格式存放）**。然后通过SQL语句的key值去获取对应的SQL语句。<br />既然将SQL语句都统一放在配置文件或者数据库中，**那么这里就涉及一个SQL语句的加载问题**。
<a name="hp4y9"></a>
### 3.3 第三步优化：传入参数映射和动态SQL

1. **问题描述：**

很多情况下，都可以通过在SQL语句中设置占位符来达到使用传入参数的目的，这种方式本身就有一定局限性，它是按照一定顺序传入参数的，要与占位符一一匹配。但是，如果**传入的参数是不确定的**（比如列表查询，根据用户填写的查询条件不同，传入查询的参数也是不同的，有时是一个参数、有时可能是三个参数），那么就得**在后台代码中自己根据请求的传入参数去拼凑相应的SQL语句**，这样的话还是**避免不了在Java代码里面写SQL语句的命运**。既然已经把SQL语句统一存放在配置文件或者数据库中了，**怎么做到能够根据前台传入参数的不同，动态生成对应的SQL语句呢？**<br />**解决问题：**<br />第一，先解决这个动态问题，**按照正常的程序员思维是，通过if和else这类的判断来进行是最直观的**，这个时候想到了JSTL中的`<if test=""></if>`这样的标签，那么，能不能将这类的标签引入到SQL语句中呢？假设可以，那么这里就需要一个专门的SQL解析器来解析这样的SQL语句，但是，if判断的变量来自于哪里呢？传入的值本身是可变的，那么得为这个值定义一个不变的变量名称，而且这个变量名称必须和对应的值要有对应关系，可以通过这个变量名称找到对应的值，这个时候想到了key-value的Map。解析的时候根据变量名的具体值来判断。<br />假如前面可以判断没有问题，那么假如判断的结果是true，那么就需要输出的标签里面的SQL片段，但是怎么解决在标签里面使用变量名称的问题呢？这里需要**使用一种有别于SQL的语法来嵌入变量（比如使用＃变量名＃）**。这样，SQL语句经过解析后就可以动态的生成符合上下文的SQL语句。<br />还有，**怎么区分开占位符变量和非占位变量？**有时候单单使用占位符是满足不了的，占位符只能为查询条件占位，SQL语句其他地方使用不了。**这里可以使用**`**#变量名#**`**表示占位符变量，使用变量名表示非占位符变量**。
<a name="bbffw"></a>
### 3.4 第四步优化：结果映射和结果缓存

1. **问题描述：**

执行SQL语句、获取执行结果、对执行结果进行转换处理、释放相关资源是一整套下来的。假如是执行查询语句，那么执行SQL语句后，返回的是一个ResultSet结果集，**这个时候就需要将ResultSet对象的数据取出来，不然等到释放资源时就取不到这些结果信息了**。从前面的优化来看，以及将获取连接、设置传入参数、执行SQL语句、释放资源这些都封装起来了，只剩下结果处理这块还没有进行封装，如果能封装起来，每个数据库操作都不用自己写那么一大堆Java代码，直接调用一个封装的方法就可以搞定了。<br />**解决问题：**<br />分析一下，一般对执行结果的有哪些处理，**有可能将结果不做任何处理就直接返回，也有可能将结果转换成一个JavaBean对象返回、一个Map返回、一个List返回等**`，结果处理可能是多种多样的。从这里看，必须告诉SQL处理器两点：**第一，需要返回什么类型的对象；第二，需要返回的对象的数据结构怎么跟执行的结果映射**，这样才能将具体的值copy到对应的数据结构上。<br />接下来，**可以进而考虑对SQL执行结果的缓存来提升性能**。缓存数据都是key-value的格式，那么这个key怎么来呢？怎么保证唯一呢？即使同一条SQL语句几次访问的过程中由于传入参数的不同，得到的执行SQL语句也是不同的。那么缓存起来的时候是多对。**但是SQL语句和传入参数两部分合起来可以作为数据缓存的key值**。
<a name="NyXlB"></a>
### 3.5 第五步优化：解决重复SQL语句问题

1. **问题描述：**

由于将所有SQL语句都放到配置文件中，**这个时候会遇到一个SQL重复的问题**，几个功能的SQL语句其实都差不多，有些可能是SELECT后面那段不同、有些可能是WHERE语句不同。有时候表结构改了，那么就需要改多个地方，不利于维护。<br />**解决问题：**<br />当代码程序出现重复代码时怎么办？**将重复的代码抽离出来成为独立的一个类，然后在各个需要使用的地方进行引用**。对于SQL重复的问题，也可以采用这种方式，通过将SQL片段模块化，**将重复的SQL片段独立成一个SQL块，然后在各个SQL语句引用重复的SQL块**，这样需要修改时只需要修改一处即可。
<a name="lBFEC"></a>
## 4、Mybaits有待改进之处

1. **问题描述：**

Mybaits所有的数据库操作都是基于SQL语句，**导致什么样的数据库操作都要写SQL语句**。一个应用系统要写的SQL语句实在太多了。<br />**改进方法：**<br />对数据库进行的操作大部分都是对表数据的增删改查，很多都是对单表的数据进行操作，由这点可以想到一个问题：**单表操作可不可以不写SQL语句，通过JavaBean的默认映射器生成对应的SQL语句**，比如：一个类UserInfo对应于USER_INFO表， userId属性对应于USER_ID字段。**这样就可以通过反射可以获取到对应的表结构了，拼凑成对应的SQL语句显然不是问题**。
<a name="jjPJd"></a>
## 5、MyBatis框架整体设计
![MyBatis框架整体设计](https://cdn.nlark.com/yuque/0/2021/png/396745/1623847802510-310573a2-13a3-4554-acee-e85e42c0c08c.png#clientId=ua7292053-2022-4&errorMessage=unknown%20error&from=paste&id=u0c031fc0&originHeight=791&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=true&status=error&style=none&taskId=uaf24a04f-c300-444d-a0f4-591e65f5d8b&title=MyBatis%E6%A1%86%E6%9E%B6%E6%95%B4%E4%BD%93%E8%AE%BE%E8%AE%A1 "MyBatis框架整体设计")
<a name="doPqY"></a>
### 5.1 接口层-和数据库交互的方式
MyBatis和数据库的交互有两种方式：

1. 使用传统的MyBatis提供的API；
2. 使用Mapper接口；
<a name="GPNGT"></a>
#### 5.1.1 使用传统的MyBatis提供的API
**这是传统的传递Statement Id 和查询参数给 SqlSession 对象，使用 SqlSession对象完成和数据库的交互**；MyBatis提供了非常方便和简单的API，供用户实现对数据库的增删改查数据操作，以及对数据库连接信息和MyBatis 自身配置信息的维护操作。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651936664566-b52d0b4f-b1b2-44e7-9c71-aaad145e9e8c.jpeg)<br />传统的MyBatis工作模式<br />上述使用MyBatis 的方法，是**创建一个和数据库打交道的SqlSession对象，然后根据Statement Id 和参数来操作数据库**，这种方式固然很简单和实用，但是**它不符合面向对象语言的概念和面向接口编程的编程习惯**。由于面向接口的编程是面向对象的大趋势，MyBatis 为了适应这一趋势，增加了第二种使用MyBatis 支持接口（Interface）调用方式。
<a name="umjFw"></a>
#### 5.1.2 使用Mapper接口
**MyBatis 将配置文件中的每一个<mapper> 节点抽象为一个 Mapper 接口：**<br />**这个接口中声明的方法和<mapper> 节点中的<select|update|delete|insert> 节点项对应**，即<select|update|delete|insert> 节点的id值为Mapper 接口中的方法名称，**parameterType 值表示Mapper 对应方法的入参类型**，而**resultMap 值则对应了Mapper 接口表示的返回值类型或者返回结果集的元素类型**。<br />![Mapper接口和Mapper.xml配置文件之间的对应关系](https://cdn.nlark.com/yuque/0/2022/png/396745/1651936228025-d4d10600-0feb-431c-944b-0085611fb9b5.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=557&id=u640d1804&originHeight=1392&originWidth=3839&originalType=binary&ratio=1&rotation=0&showTitle=true&size=821629&status=error&style=none&taskId=u27a9aa8f-76bf-44bd-90b7-2529fbee1ee&title=Mapper%E6%8E%A5%E5%8F%A3%E5%92%8CMapper.xml%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E4%B9%8B%E9%97%B4%E7%9A%84%E5%AF%B9%E5%BA%94%E5%85%B3%E7%B3%BB&width=1535.6 "Mapper接口和Mapper.xml配置文件之间的对应关系")<br />**根据MyBatis 的配置规范配置好后，通过**`**SqlSession.getMapper(XXXMapper.class)**`**方法，MyBatis 会根据相应的接口声明的方法信息，通过动态代理机制生成一个Mapper 实例**，使用Mapper接口的某一个方法时，MyBatis会根据这个方法的方法名和参数类型，确定Statement Id，底层还是通过`SqlSession.select("statementId",parameterObject);`或者`SqlSession.update("statementId",parameterObject);` 等等来实现对数据库的操作，**MyBatis引用Mapper 接口这种调用方式，纯粹是为了满足面向接口编程的需要**。（其实还有一个原因是在于，面向接口的编程，使得用户在接口上可以使用注解来配置SQL语句，这样就可以脱离XML配置文件，实现“0配置”）。
<a name="HM1XP"></a>
### 5.2 数据处理层
**数据处理层可以说是MyBatis的核心**，从大的方面上讲，它要完成两个功能：

1. 通过传入参数构建动态SQL语句；
2. SQL语句的执行以及封装查询结果集成`List<E>`；
<a name="Zq5MO"></a>
#### 5.2.1 参数映射和动态SQL语句生成
动态语句生成可以说是MyBatis框架非常优雅的一个设计，**MyBatis 通过传入的参数值，使用 Ognl 来动态地构造SQL语句**，使得MyBatis 有很强的灵活性和扩展性。<br />**参数映射指的是对于java 数据类型和jdbc数据类型之间的转换：**这里有包括两个过程：**查询阶段**，要将java类型的数据，转换成jdbc类型的数据，通过 `preparedStatement.setXXX()` 来设值；另一个就是**对resultset查询结果集的jdbcType 数据转换成java 数据类型**。
<a name="nI4uJ"></a>
#### 5.2.2 SQL语句的执行以及封装查询结果集成`List<E>###`
动态SQL语句生成之后，MyBatis 将执行SQL语句，并将可能返回的结果集转换成`List<E>` 列表。**MyBatis 在对结果集的处理中，支持结果集关系一对多和多对一的转换**，并且有两种支持方式，**一种为嵌套查询语句的查询，还有一种是嵌套结果集的查询**。
<a name="j98hX"></a>
### 5.3 框架支撑层

1. **事务管理机制**

**事务管理机制对于ORM框架而言是不可缺少的一部分**，事务管理机制的质量也是考量一个ORM框架是否优秀的一个标准。

1. **连接池管理机制**

由于创建一个数据库连接所占用的资源比较大，**对于数据吞吐量大和访问量非常大的应用而言，连接池的设计就显得非常重要**。

1. **缓存机制**

为了提高数据利用率和减小服务器和数据库的压力，**MyBatis 会对于一些查询提供会话级别的数据缓存**，会将对某一次查询，放置到SqlSession 中，在允许的时间间隔内，对于完全相同的查询，MyBatis会直接将缓存结果返回给用户，而不用再到数据库中查找。

1. **SQL语句的配置方式**

传统的MyBatis 配置SQL语句方式就是使用XML文件进行配置的，但是这种方式不能很好地支持面向接口编程的理念，**为了支持面向接口的编程，MyBatis 引入了Mapper接口的概念，面向接口的引入，对使用注解来配置SQL语句成为可能，用户只需要在接口上添加必要的注解即可，不用再去配置XML文件了**，但是，目前的MyBatis 只是对注解配置SQL语句提供了有限的支持，某些高级功能还是要依赖XML配置文件配置SQL 语句。
<a name="zK36o"></a>
### 5.4 引导层
**引导层是配置和启动MyBatis配置信息的方式**。MyBatis 提供两种方式来引导MyBatis ：**基于XML配置文件的方式和基于Java API 的方式**。
<a name="SivNb"></a>
### 5.5 主要构件及其相互关系
从MyBatis代码实现的角度来看，MyBatis的主要的核心部件有以下几个：<br />`**SqlSession**`**：**作为MyBatis工作的主要顶层API，表示和数据库交互的会话，完成必要数据库增删改查功能；<br />`**Executor**`**：**MyBatis执行器，是MyBatis 调度的核心，负责SQL语句的生成和查询缓存的维护；<br />`**StatementHandler**`**：**封装了JDBC Statement操作，负责对JDBC statement 的操作，如设置参数、将Statement结果集转换成List集合。<br />`**ParameterHandler**`**：**负责对用户传递的参数转换成JDBC Statement 所需要的参数；<br />`**ResultSetHandler**`**：**负责将JDBC返回的ResultSet结果集对象转换成List类型的集合；<br />`**TypeHandler**`**：**负责java数据类型和jdbc数据类型之间的映射和转换；<br />`**MappedStatement**`**：**`MappedStatement`维护了一条<select|update|delete|insert>节点的封装；<br />`**SqlSource**`**：**负责根据用户传递的parameterObject，动态地生成SQL语句，将信息封装到`BoundSql`对象中，并返回；<br />`**BoundSql**`**：**表示动态生成的SQL语句以及相应的参数信息；<br />`**Configuration**`**：**MyBatis所有的配置信息都维持在`Configuration`对象之中；<br />**它们的关系如下图所示：**<br />![MyBatis主要构件关系如图](https://cdn.nlark.com/yuque/0/2021/png/396745/1623847802597-3589a805-e662-4598-8e1f-0d7d689f3a8c.png#clientId=ua7292053-2022-4&errorMessage=unknown%20error&from=paste&id=ufa5c66c4&originHeight=1326&originWidth=1064&originalType=url&ratio=3&rotation=0&showTitle=true&status=error&style=shadow&taskId=uac6e2de5-a713-4b9d-86e3-031cdf217b5&title=MyBatis%E4%B8%BB%E8%A6%81%E6%9E%84%E4%BB%B6%E5%85%B3%E7%B3%BB%E5%A6%82%E5%9B%BE "MyBatis主要构件关系如图")
<a name="m0FgP"></a>
## 6、SqlSession工作过程分析

1. **开启一个数据库访问会话---创建SqlSession对象**
```java
SqlSession sqlSession = factory.openSession();
```
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651936662702-db821804-c049-4fdd-8c87-5e0adc168a23.jpeg)<br />MyBatis封装了对数据库的访问，把对数据库的会话和事务控制放到了`SqlSession`对象中

1. **为SqlSession传递一个配置的Sql语句的Statement Id和参数，然后返回结果：**
```java
List<Employee> result = sqlSession.selectList("com.louis.mybatis.dao.EmployeesMapper.selectByMinSalary",params);
```
上述的"com.louis.mybatis.dao.EmployeesMapper.selectByMinSalary"，是配置在EmployeesMapper.xml 的Statement ID，params是传递的查询参数。<br />来看一下`sqlSession.selectList()`方法的定义：
```java
public <E> List<E> selectList(String statement, Object parameter) {  
	return this.selectList(statement, parameter, RowBounds.DEFAULT);  
}  

public <E> List<E> selectList(String statement, Object parameter, RowBounds rowBounds) {  
	try {  
		//1.根据Statement Id，在mybatis 配置对象Configuration中查找和配置文件相对应的MappedStatement      
		MappedStatement ms = configuration.getMappedStatement(statement);  
		//2. 将查询任务委托给MyBatis 的执行器 Executor  
		List<E> result = executor.query(ms, wrapCollection(parameter), rowBounds, Executor.NO_RESULT_HANDLER);  
		return result;  
	} catch (Exception e) {  
		throw ExceptionFactory.wrapException("Error querying database.  Cause: " + e, e);  
	} finally {  
		ErrorContext.instance().reset();  
	}  
}
```
MyBatis在初始化的时候，会将MyBatis的配置信息全部加载到内存中，**使用org.apache.ibatis.session.Configuration实例来维护**。使用者可以使用`sqlSession.getConfiguration()`方法来获取。**MyBatis的配置文件中配置信息的组织格式和内存中对象的组织格式几乎完全对应的**。<br />上述例子中的：
```xml
<select id="selectByMinSalary" resultMap="BaseResultMap" parameterType="java.util.Map" >  
   select   
       EMPLOYEE_ID, FIRST_NAME, LAST_NAME, EMAIL, SALARY  
   from LOUIS.EMPLOYEES  
   <if test="min_salary != null">  
       where SALARY < #{min_salary,jdbcType=DECIMAL}  
   </if>  
</select>
```
**加载到内存中会生成一个对应的**`**MappedStatement**`**对象，然后会以**`**key="com.louis.mybatis.dao.EmployeesMapper.selectByMinSalary"**`** ，value为**`**MappedStatement**`**对象的形式维护到**`**Configuration**`**的一个Map中**。当以后需要使用的时候，只需要通过Id值来获取就可以了。<br />从上述的代码中可以看到`SqlSession`的职能是：`**SqlSession**`**根据Statement ID，在Mybatis配置对象**`**Configuration**`**中获取到对应的**`**MappedStatement**`**对象，然后调用mybatis执行器来执行具体的操作**。

1. **MyBatis执行器Executor根据SqlSession传递的参数执行**`**query()**`**方法（由于代码过长，只需阅读注释的地方即可）：**
```java
/** 
   * BaseExecutor 类部分代码 
   * 
   */  
public <E> List<E> query(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler) throws SQLException {
	// 1. 根据具体传入的参数，动态地生成需要执行的SQL语句，用BoundSql对象表示    
	BoundSql boundSql = ms.getBoundSql(parameter);  
	// 2. 为当前的查询创建一个缓存Key  
	CacheKey key = createCacheKey(ms, parameter, rowBounds, boundSql);  
	return query(ms, parameter, rowBounds, resultHandler, key, boundSql);  
}  

@SuppressWarnings("unchecked")  
public <E> List<E> query(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql) throws SQLException {  
	ErrorContext.instance().resource(ms.getResource()).activity("executing a query").object(ms.getId());  
	if (closed) throw new ExecutorException("Executor was closed.");  
	if (queryStack == 0 && ms.isFlushCacheRequired()) {  
		clearLocalCache();  
	}  
	List<E> list;  
	try {  
		queryStack++;  
		list = resultHandler == null ? (List<E>) localCache.getObject(key) : null;  
		if (list != null) {  
			handleLocallyCachedOutputParameters(ms, key, parameter, boundSql);  
		} else {  
			// 3.缓存中没有值，直接从数据库中读取数据    
			list = queryFromDatabase(ms, parameter, rowBounds, resultHandler, key, boundSql);  
		}  
	} finally {  
		queryStack--;  
	}  
	if (queryStack == 0) {  
		for (DeferredLoad deferredLoad : deferredLoads) {  
			deferredLoad.load();  
		}  
		deferredLoads.clear(); // issue #601  
		if (configuration.getLocalCacheScope() == LocalCacheScope.STATEMENT) {  
			clearLocalCache(); // issue #482  
		}  
	}  
	return list;  
}

private <E> List<E> queryFromDatabase(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql) throws SQLException {  
	List<E> list;  
	localCache.putObject(key, EXECUTION_PLACEHOLDER);  
	try {  

		//4. 执行查询，返回List 结果，然后    将查询的结果放入缓存之中  
		list = doQuery(ms, parameter, rowBounds, resultHandler, boundSql);  
	} finally {  
		localCache.removeObject(key);  
	}  
	localCache.putObject(key, list);  
	if (ms.getStatementType() == StatementType.CALLABLE) {  
		localOutputParameterCache.putObject(key, parameter);  
	}  
	return list;  
}
```
```java
/** 
 * 
 * SimpleExecutor类的doQuery()方法实现 
 * 
 */  
public <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException {  
	Statement stmt = null;  
	try {  
		Configuration configuration = ms.getConfiguration();  
		//5. 根据既有的参数，创建StatementHandler对象来执行查询操作  
		StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql);  
		//6. 创建java.Sql.Statement对象，传递给StatementHandler对象  
		stmt = prepareStatement(handler, ms.getStatementLog());  
		//7. 调用StatementHandler.query()方法，返回List结果集  
		return handler.<E>query(stmt, resultHandler);  
	} finally {  
		closeStatement(stmt);  
	}  
}
```
上述的`Executor.query()`方法几经转折，**最后会创建一个**`**StatementHandler**`**对象，然后将必要的参数传递给**`**StatementHandler**`，使用`StatementHandler`来完成对数据库的查询，最终返回List结果集。<br />**从上面的代码中可以看出，**`**Executor**`**的功能和作用是：**

1. 根据传递的参数，完成SQL语句的动态解析，生成`BoundSql`对象，供`StatementHandler`使用；
2. 为查询创建缓存，以提高性能；
3. 创建JDBC的`Statement`连接对象，传递给`StatementHandler`对象，返回List查询结果；
4. `**StatementHandler**`**对象负责设置**`**Statement**`**对象中的查询参数、处理JDBC返回的**`**resultSet**`**，将**`**resultSet**`**加工为List 集合返回：**

接着上面的`Executor`第六步，看一下：`prepareStatement()` 方法的实现：
```java
/** 
 * 
 * SimpleExecutor类的doQuery()方法实现 
 * 
 */  
public <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException { 
	Statement stmt = null; 
	try { 
		Configuration configuration = ms.getConfiguration(); 
		StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql); 
		// 1.准备Statement对象，并设置Statement对象的参数 
		stmt = prepareStatement(handler, ms.getStatementLog()); 
		// 2. StatementHandler执行query()方法，返回List结果 
		return handler.<E>query(stmt, resultHandler); 
	} finally {
		closeStatement(stmt); 
	} 
}  

private Statement prepareStatement(StatementHandler handler, Log statementLog) throws SQLException {
	Statement stmt;  
	Connection connection = getConnection(statementLog);  
	stmt = handler.prepare(connection);  
	//对创建的Statement对象设置参数，即设置SQL 语句中 ? 设置为指定的参数  
	handler.parameterize(stmt);  
	return stmt;  
}
```
以上可以总结`StatementHandler`对象主要完成两个工作：

1. 对于JDBC的`PreparedStatement`类型的对象，创建的过程中，使用的是SQL语句字符串会包含 若干个`?` 占位符，其后再对占位符进行设值。<br />`**StatementHandler**`**通过**`**parameterize(statement)**`**方法对**`**Statement**`**进行设值；**
2. `StatementHandler`通过`List<E> query(Statement statement, ResultHandler resultHandler)`方法来完成执行`Statement`，和将`Statement`对象返回的`resultSet`封装成List；
3. `**StatementHandler**`** 的**`**parameterize(statement)**`** 方法的实现：**
```java
/** 
 * StatementHandler 类的parameterize(statement) 方法实现  
 */  
public void parameterize(Statement statement) throws SQLException {  
	//使用ParameterHandler对象来完成对Statement的设值    
	parameterHandler.setParameters((PreparedStatement) statement);  
}
```
```java
/** 
 *  
 * ParameterHandler类的setParameters(PreparedStatement ps) 实现 
 * 对某一个Statement进行设置参数 
 */  
public void setParameters(PreparedStatement ps) throws SQLException {  
	ErrorContext.instance().activity("setting parameters").object(mappedStatement.getParameterMap().getId());  
	List<ParameterMapping> parameterMappings = boundSql.getParameterMappings();  
	if (parameterMappings != null) {  
		for (int i = 0; i < parameterMappings.size(); i++) {  
			ParameterMapping parameterMapping = parameterMappings.get(i);  
			if (parameterMapping.getMode() != ParameterMode.OUT) {  
				Object value;  
				String propertyName = parameterMapping.getProperty();  
				if (boundSql.hasAdditionalParameter(propertyName)) { // issue #448 ask first for additional params  
					value = boundSql.getAdditionalParameter(propertyName);  
				} else if (parameterObject == null) {  
					value = null;  
				} else if (typeHandlerRegistry.hasTypeHandler(parameterObject.getClass())) {  
					value = parameterObject;  
				} else {  
					MetaObject metaObject = configuration.newMetaObject(parameterObject);  
					value = metaObject.getValue(propertyName);  
				}  

				// 每一个Mapping都有一个TypeHandler，根据TypeHandler来对preparedStatement进行设置参数  
				TypeHandler typeHandler = parameterMapping.getTypeHandler();  
				JdbcType jdbcType = parameterMapping.getJdbcType();  
				if (value == null && jdbcType == null) jdbcType = configuration.getJdbcTypeForNull();  
				// 设置参数  
				typeHandler.setParameter(ps, i + 1, value, jdbcType);  
			}  
		}  
	}  
}
```
从上述的代码可以看到，`StatementHandler`的`parameterize(Statement)` 方法调用了 `ParameterHandler`的`setParameters(statement)` 方法，<br />`**ParameterHandler**`**的**`**setParameters(Statement)**`**方法负责 根据输入的参数，对statement对象的 ? 占位符处进行赋值。**

1. `**StatementHandler**`** 的**`**List<E> query(Statement statement, ResultHandler resultHandler)**`**方法的实现：**
```java
/** 
 * PreParedStatement类的query方法实现 
 */  
public <E> List<E> query(Statement statement, ResultHandler resultHandler) throws SQLException {  
	//1.调用preparedStatemnt。execute()方法，然后将resultSet交给ResultSetHandler处理    
	PreparedStatement ps = (PreparedStatement) statement;  
	ps.execute();  
	//2. 使用ResultHandler来处理ResultSet  
	return resultSetHandler.<E> handleResultSets(ps);  
}
```
从上述代码可以看出，`StatementHandler` 的`List<E> query(Statement statement, ResultHandler resultHandler)`方法的实现，是调用了`ResultSetHandler的handleResultSets(Statement)` 方法。`**ResultSetHandler**`**的**`**handleResultSets(Statement)**`** 方法会将Statement语句执行后生成的resultSet 结果集转换成List<E> 结果集**：
```java
/**   
   * ResultSetHandler类的handleResultSets()方法实现 
   *  
   */  
public List<Object> handleResultSets(Statement stmt) throws SQLException {  
	final List<Object> multipleResults = new ArrayList<Object>();  

	int resultSetCount = 0;  
	ResultSetWrapper rsw = getFirstResultSet(stmt);  

	List<ResultMap> resultMaps = mappedStatement.getResultMaps();  
	int resultMapCount = resultMaps.size();  
	validateResultMapsCount(rsw, resultMapCount);  

	while (rsw != null && resultMapCount > resultSetCount) {  
		ResultMap resultMap = resultMaps.get(resultSetCount);  

		//将resultSet  
		handleResultSet(rsw, resultMap, multipleResults, null);  
		rsw = getNextResultSet(stmt);  
		cleanUpAfterHandlingResultSet();  
		resultSetCount++;  
	}

	String[] resultSets = mappedStatement.getResulSets();  
	if (resultSets != null) {  
		while (rsw != null && resultSetCount < resultSets.length) {  
			ResultMapping parentMapping = nextResultMaps.get(resultSets[resultSetCount]);  
			if (parentMapping != null) {  
				String nestedResultMapId = parentMapping.getNestedResultMapId();  
				ResultMap resultMap = configuration.getResultMap(nestedResultMapId);  
				handleResultSet(rsw, resultMap, null, parentMapping);  
			}  
			rsw = getNextResultSet(stmt);  
			cleanUpAfterHandlingResultSet();  
			resultSetCount++;  
		}  
	}  

	return collapseSingleResultList(multipleResults);  
}
```
<a name="XcvXM"></a>
## 7、MyBatis初始化机制
<a name="cB9GH"></a>
### 7.1 MyBatis的初始化做了什么
**任何框架的初始化，无非是加载自己运行时所需要的配置信息。**MyBatis的配置信息，大概包含以下信息，其高层级结构如下：<br />[![https://mybatis.org/mybatis-3/configuration.html](https://cdn.nlark.com/yuque/0/2022/png/396745/1651935204533-61ead642-45ff-4055-a0eb-bdc5f134d806.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&id=uf846a4c8&originHeight=407&originWidth=509&originalType=binary&ratio=1&rotation=0&showTitle=true&size=33234&status=error&style=shadow&taskId=uf6ceb0b8-0e23-4daa-ab1d-266e42ec7ff&title=https%3A%2F%2Fmybatis.org%2Fmybatis-3%2Fconfiguration.html "https://mybatis.org/mybatis-3/configuration.html")](https://mybatis.org/mybatis-3/configuration.html)<br />![MyBatis配置信息结构图](https://cdn.nlark.com/yuque/0/2022/png/396745/1651975676218-ab4370ec-8710-4b23-93c6-9b096d390366.png#clientId=u72a61239-7f20-4&errorMessage=unknown%20error&from=paste&height=480&id=ue774b340&originHeight=1200&originWidth=1261&originalType=binary&ratio=1&rotation=0&showTitle=true&size=307421&status=error&style=shadow&taskId=ufb704645-c785-4b82-ac3e-6a7e78b46c9&title=MyBatis%E9%85%8D%E7%BD%AE%E4%BF%A1%E6%81%AF%E7%BB%93%E6%9E%84%E5%9B%BE&width=504.4 "MyBatis配置信息结构图")<br />**MyBatis的上述配置信息会配置在XML配置文件中，那么，这些信息被加载进入MyBatis内部，MyBatis是怎样维护的呢？**<br />MyBatis采用了一个非常直白和简单的方式---**使用 org.apache.ibatis.session.Configuration对象作为一个所有配置信息的容器，Configuration对象的组织结构和XML配置文件的组织结构几乎完全一样**（当然，Configuration对象的功能并不限于此，它还负责创建一些MyBatis内部使用的对象，如Executor等）。如下图所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651934241825-81036a2f-5eec-4896-a122-4a051591272d.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=673&id=ub84d8d84&originHeight=1683&originWidth=2190&originalType=binary&ratio=1&rotation=0&showTitle=false&size=426197&status=error&style=shadow&taskId=ua82688c7-1340-40b1-98d4-ce8b7d59373&title=&width=876)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651934869816-6e8a4221-2978-4949-b53d-5b1464f92be6.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=620&id=u14259959&originHeight=1549&originWidth=2304&originalType=binary&ratio=1&rotation=0&showTitle=false&size=137363&status=error&style=shadow&taskId=u562f7c69-dbbd-4932-a882-b7a878764ef&title=&width=921.6)<br />`Configuration`对象的组织结构和XML配置文件的组织结构几乎完全一样<br />MyBatis根据初始化好`Configuration`信息，这时候用户就可以使用MyBatis进行数据库操作了。**可以这么说，MyBatis初始化的过程，就是创建 **`**Configuration**`**对象的过程**。<br />**MyBatis的初始化可以有两种方式：**<br />基于XML配置文件：基于XML配置文件的方式是将MyBatis的所有配置信息放在XML文件中，MyBatis通过加载并XML配置文件，将配置文信息组装成内部的`Configuration`对象。<br />基于Java API：这种方式不使用XML配置文件，需要MyBatis使用者在Java代码中，手动创建`Configuration`对象，然后将配置参数set 进入`Configuration`对象中。<br />接下来将通过 基于XML配置文件方式的MyBatis初始化，深入探讨MyBatis是如何通过配置文件构建`Configuration`对象，并使用它。
<a name="vhtpM"></a>
### 7.2 基于XML配置文件创建`Configuration`对象
现在就从使用MyBatis的简单例子入手，深入分析一下MyBatis是怎样完成初始化的，都初始化了什么。看以下代码：
```java
String resource = "mybatis-config.xml";  
InputStream inputStream = Resources.getResourceAsStream(resource);  
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);  
SqlSession sqlSession = sqlSessionFactory.openSession();  
List list = sqlSession.selectList("com.foo.bean.BlogMapper.queryAllBlogInfo");
```
有MyBatis使用经验的都知道，上述语句的作用是执行com.foo.bean.BlogMapper.queryAllBlogInfo 定义的SQL语句，返回一个List结果集。总的来说，上述代码经历了**mybatis初始化 -->创建SqlSession -->执行SQL语句**返回结果三个过程。<br />上述代码的功能是根据配置文件mybatis-config.xml 配置文件，创建`SqlSessionFactory`对象，然后产生SqlSession，执行SQL语句。**而mybatis的初始化就发生在第三句：**`**SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);**` 现在看看第三句到底发生了什么。

1. **MyBatis初始化基本过程：**

`SqlSessionFactoryBuilder`根据传入的数据流生成`Configuration`对象，然后根据`Configuration`对象创建默认的`SqlSessionFactory`实例。<br />**初始化的基本过程如下序列图所示：**
![](https://cdn.nlark.com/yuque/__puml/65a52ba0c3f1848ae3f3c0ac9872a143.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuYXV0b251bWJlclxuXG5wYXJ0aWNpcGFudCBcIk15YmF0aXMtY29uZmlnLnhtbFwiIGFzIE15YmF0aXNDb25maWdcbnBhcnRpY2lwYW50IFwiQ2xpZW50XCIgYXMgQ2xpZW50XG5wYXJ0aWNpcGFudCBcIlNxbFNlc3Npb25GYWN0b3J5QnVpbGRlclwiIGFzIFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlciAjb3JhbmdlXG5wYXJ0aWNpcGFudCBcIlhNTENvbmZpZ0J1aWxkZXJcIiBhcyBYTUxDb25maWdCdWlsZGVyXG5cblxuYWN0aXZhdGUgTXliYXRpc0NvbmZpZ1xuXG5NeWJhdGlzQ29uZmlnIC0-IENsaWVudDogUmVhZGVy5oiW6ICFSW5wdXRTdHJlYW3mtYFcbmFjdGl2YXRlIENsaWVudFxuXG5DbGllbnQgLT4gU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyOiBidWlsZChpbnB1dFN0cmVhbSlcbmFjdGl2YXRlIFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlclxuU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyIC0-IFhNTENvbmZpZ0J1aWxkZXI6IG5ldyBYTUxDb25maWdCdWlsZGVyKGlucHV0U3RyZWFtLC4uKVxuYWN0aXZhdGUgWE1MQ29uZmlnQnVpbGRlclxuU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyIC0-IFhNTENvbmZpZ0J1aWxkZXI6IHBhcnNlKClcblhNTENvbmZpZ0J1aWxkZXIgLS0-IFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlcjog6L-U5ZueQ29uZmlndXJhdGlvbuWvueixoVxuZGVhY3RpdmF0ZSBYTUxDb25maWdCdWlsZGVyXG5TcWxTZXNzaW9uRmFjdG9yeUJ1aWxkZXIgLT4gU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyOiBidWlsZChjb25maWd1cmF0aW9uKei_lOWbnm5ldyBEZWZhdWx0U3FsU2Vzc2lvbkZhY3RvcnkoY29uZmlnKVxuU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyIC0tPiBDbGllbnQ6IOi_lOWbnlNxbFNlc3Npb25GYWN0b3J55a6e5L6LXG5kZWFjdGl2YXRlIFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlclxuXG5AZW5kdW1sIiwidXJsIjoiaHR0cHM6Ly9jZG4ubmxhcmsuY29tL3l1cXVlL19fcHVtbC82NWE1MmJhMGMzZjE4NDhhZTNmM2MwYWM5ODcyYTE0My5zdmciLCJpZCI6InVEWFh6IiwibWFyZ2luIjp7InRvcCI6dHJ1ZSwiYm90dG9tIjp0cnVlfSwiY2FyZCI6ImRpYWdyYW0ifQ==)MyBatis初始化序列图<br />由上图所示，Mybatis初始化要经过简单的以下几步：

1. 调用`SqlSessionFactoryBuilder`对象的`build(inputStream)`方法；
2. `SqlSessionFactoryBuilder`会根据输入流`inputStream`等信息创建`XMLConfigBuilder`对象;
3. `SqlSessionFactoryBuilder`调用`XMLConfigBuilder`对象的`parse()`方法；
4. `XMLConfigBuilder`对象返回`Configuration`对象；
5. `SqlSessionFactoryBuilder`根据`Configuration`对象创建一个`DefaultSqlSessionFactory`对象；
6. `SqlSessionFactoryBuilder`返回 `DefaultSqlSessionFactory`对象给Client，供Client使用。

`**SqlSessionFactoryBuilder**`**相关的代码如下所示：**
```java
public SqlSessionFactory build(InputStream inputStream)  {  
	return build(inputStream, null, null);  
}  

public SqlSessionFactory build(InputStream inputStream, String environment, Properties properties)  {  
	try  {  
		//2. 创建XMLConfigBuilder对象用来解析XML配置文件，生成Configuration对象  
		XMLConfigBuilder parser = new XMLConfigBuilder(inputStream, environment, properties);  
		//3. 将XML配置文件内的信息解析成Java对象Configuration对象  
		Configuration config = parser.parse();  
		//4. 根据Configuration对象创建出SqlSessionFactory对象  
		return build(config);  
	} catch (Exception e) {  
		throw ExceptionFactory.wrapException("Error building SqlSession.", e);  
	} finally {  
		ErrorContext.instance().reset();  
		try {  
			inputStream.close();  
		} catch (IOException e) {  
			// Intentionally ignore. Prefer previous error.  
		}  
	}
}

// 从此处可以看出，MyBatis内部通过Configuration对象来创建SqlSessionFactory,用户也可以自己通过API构造好Configuration对象，调用此方法创SqlSessionFactory  
public SqlSessionFactory build(Configuration config) {  
	return new DefaultSqlSessionFactory(config);  
}
```
上述的初始化过程中，涉及到了以下几个对象：<br />`SqlSessionFactoryBuilder` ：`SqlSessionFactory`的构造器，用于创建`SqlSessionFactory`，采用了Builder设计模式<br />`Configuration `：该对象是mybatis-config.xml文件中所有mybatis配置信息<br />`SqlSessionFactory`：`SqlSession`工厂类，以工厂形式创建`SqlSession`对象，采用了`Factory`工厂设计模式<br />`XMLConfigBuilder `：负责将mybatis-config.xml配置文件解析成`Configuration`对象，共`SqlSessonFactoryBuilder`使用，创建`SqlSessionFactory`

1. **创建**`**Configuration**`**对象的过程：**<br />接着上述的 MyBatis初始化基本过程讨论，**当**`**SqlSessionFactoryBuilder**`**执行**`**build()**`**方法，调用了**`**XMLConfigBuilder**`**的parse()方法，然后返回了**`**Configuration**`**对象**。那么`parse()`方法是如何处理XML文件，生成`Configuration`对象的呢？

（1）`XMLConfigBuilder`会**将XML配置文件的信息转换为**`**Document**`**对象**，而XML配置定义文件**DTD转换成**`**XMLMapperEntityResolver**`**对象**，然后**将二者封装到**`**XpathParser**`**对象中，**`**XpathParser**`**的作用是提供根据Xpath表达式获取基本的DOM节点Node信息的操作**。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1651933942220-d0c2387d-ece9-4d70-aa5a-674381747a4a.jpeg)<br />![XpathParser组成结构图和生成图](https://cdn.nlark.com/yuque/0/2022/png/396745/1651933048700-708b3195-ec2c-4d86-ab71-7717d4d86bd9.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=382&id=u79e314e0&originHeight=954&originWidth=2200&originalType=binary&ratio=1&rotation=0&showTitle=true&size=121862&status=error&style=shadow&taskId=ud2ca833e-2839-4efc-9a83-2968d844fce&title=XpathParser%E7%BB%84%E6%88%90%E7%BB%93%E6%9E%84%E5%9B%BE%E5%92%8C%E7%94%9F%E6%88%90%E5%9B%BE&width=880 "XpathParser组成结构图和生成图")<br />（2）之后`XMLConfigBuilder`调用`parse()`方法：**会从XPathParser中取出 **`**<configuration>**`**节点对应的Node对象，然后解析此Node节点的子Node**：properties, settings, typeAliases, typeHandlers, objectFactory, objectWrapperFactory, plugins, environments, databaseIdProvider, mappers：
```java
public Configuration parse() {  
	if (parsed) {  
		throw new BuilderException("Each XMLConfigBuilder can only be used once.");  
	}  
	parsed = true;  
	//源码中没有这一句，只有parseConfiguration(parser.evalNode("/configuration"));  
	//为了看得更明晰，源码拆分为以下两句  
	XNode configurationNode = parser.evalNode("/configuration");  
	parseConfiguration(configurationNode);  
	return configuration;
}
/** 
  * 解析 "/configuration"节点下的子节点信息，然后将解析的结果设置到Configuration对象中 
  */  
private void parseConfiguration(XNode root) {  
	try {  
		//1.首先处理properties 节点     
		propertiesElement(root.evalNode("properties")); //issue #117 read properties first  
		//2.处理typeAliases  
		typeAliasesElement(root.evalNode("typeAliases"));  
		//3.处理插件  
		pluginElement(root.evalNode("plugins"));  
		//4.处理objectFactory  
		objectFactoryElement(root.evalNode("objectFactory"));  
		//5.objectWrapperFactory  
		objectWrapperFactoryElement(root.evalNode("objectWrapperFactory"));  
		//6.settings  
		settingsElement(root.evalNode("settings"));  
		//7.处理environments  
		environmentsElement(root.evalNode("environments")); // read it after objectFactory and objectWrapperFactory issue #631  
		//8.database  
		databaseIdProviderElement(root.evalNode("databaseIdProvider"));  
		//9.typeHandlers  
		typeHandlerElement(root.evalNode("typeHandlers"));  
		//10.mappers  
		mapperElement(root.evalNode("mappers"));  
	} catch (Exception e) {  
		throw new BuilderException("Error parsing SQL Mapper Configuration. Cause: " + e, e);  
	}  
}
```
注意：在上述代码中，还有一个非常重要的地方，**就是解析XML配置文件子节点**`**<mappers>**`**的方法**`**mapperElements(root.evalNode("mappers"))**`**，它将解析配置的Mapper.xml配置文件，Mapper配置文件可以说是MyBatis的核心**，MyBatis的特性和理念都体现在此Mapper的配置和设计上。<br />（3）**然后将这些值解析出来设置到Configuration对象中：**<br />解析子节点的过程这里就不一一介绍了，用户可以参照MyBatis源码仔细揣摩，**就看上述的**`**environmentsElement(root.evalNode("environments"));**`** 方法是如何将**`**environments**`**的信息解析出来，设置到Configuration对象中的：**
```java
/** 
  * 解析environments节点，并将结果设置到Configuration对象中 
  * 注意：创建envronment时，如果SqlSessionFactoryBuilder指定了特定的环境（即数据源）；
  *      则返回指定环境（数据源）的Environment对象，否则返回默认的Environment对象；
  *      这种方式实现了MyBatis可以连接多数据源 
  */  
private void environmentsElement(XNode context) throws Exception {  
	if (context != null)  
	{  
		if (environment == null)  
		{  
			environment = context.getStringAttribute("default");  
		}  
		for (XNode child : context.getChildren())  
		{  
			String id = child.getStringAttribute("id");  
			if (isSpecifiedEnvironment(id))  
			{  
				//1.创建事务工厂 TransactionFactory  
				TransactionFactory txFactory = transactionManagerElement(child.evalNode("transactionManager"));  
				DataSourceFactory dsFactory = dataSourceElement(child.evalNode("dataSource"));  
				//2.创建数据源DataSource  
				DataSource dataSource = dsFactory.getDataSource();  
				//3. 构造Environment对象  
				Environment.Builder environmentBuilder = new Environment.Builder(id)  
					.transactionFactory(txFactory)  
					.dataSource(dataSource);  
				//4. 将创建的Envronment对象设置到configuration 对象中  
				configuration.setEnvironment(environmentBuilder.build());  
			}  
		}  
	}  
}
```
```java
private boolean isSpecifiedEnvironment(String id) {  
	if (environment == null)  
	{  
		throw new BuilderException("No environment specified.");  
	}  
	else if (id == null)  
	{  
		throw new BuilderException("Environment requires an id attribute.");  
	}  
	else if (environment.equals(id))  
	{  
		return true;  
	}  
	return false;  
}
```
（4）**返回Configuration对象：**<br />将上述的MyBatis初始化基本过程的序列图细化：
![](https://cdn.nlark.com/yuque/__puml/0eaf446c49863434f91f948514ba3aa1.svg#lake_card_v2=eyJ0eXBlIjoicHVtbCIsImNvZGUiOiJAc3RhcnR1bWxcblxuYXV0b251bWJlclxuXG5wYXJ0aWNpcGFudCBcIlNxbFNlc3Npb25GYWN0b3J5QnVpbGRlclwiIGFzIFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlciAjc2t5Ymx1ZVxucGFydGljaXBhbnQgXCJYTUxDb25maWdCdWlsZGVyXCIgYXMgWE1MQ29uZmlnQnVpbGRlciAjcmVkXG5wYXJ0aWNpcGFudCBcIkNvbmZpZ3VyYXRpb25cIiBhcyBDb25maWd1cmF0aW9uICNvcmFuZ2VcbnBhcnRpY2lwYW50IFwiWFBhdGhQYXJzZXJcIiBhcyBYUGF0aFBhcnNlclxucGFydGljaXBhbnQgXCJYTUxNYXBwZXJFbnRpdHlSZXNvbHZlclwiIGFzIFhNTE1hcHBlckVudGl0eVJlc29sdmVyXG5cbmFjdGl2YXRlIFNxbFNlc3Npb25GYWN0b3J5QnVpbGRlclxuU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyIC0-IFhNTENvbmZpZ0J1aWxkZXI6IG5ldyBYTUxDb25maWdCdWlsZGVyKGlucHV0U3RyZWFtLClcbmFjdGl2YXRlIFhNTENvbmZpZ0J1aWxkZXJcblhNTENvbmZpZ0J1aWxkZXIgLT4gWE1MTWFwcGVyRW50aXR5UmVzb2x2ZXI6IG5ldyBYTUxNYXBwZXJFbnRpdHlSZXNvbHZlcigpXG5hY3RpdmF0ZSBYTUxNYXBwZXJFbnRpdHlSZXNvbHZlclxuWE1MTWFwcGVyRW50aXR5UmVzb2x2ZXIgLS0-IFhNTENvbmZpZ0J1aWxkZXI66L-U5ZuecmVzb2x2ZXJcbmRlYWN0aXZhdGUgWE1MTWFwcGVyRW50aXR5UmVzb2x2ZXJcblhNTENvbmZpZ0J1aWxkZXIgLT4gWFBhdGhQYXJzZXI6IG5ldyBYUGF0aFBhcnNlcihpbnB1dFN0cmVhbSwgdHJ1ZSwgcHJvcHMsIHJlc29sdmVyKVxuYWN0aXZhdGUgWFBhdGhQYXJzZXJcblhQYXRoUGFyc2VyIC0tPiBYTUxDb25maWdCdWlsZGVyOui_lOWbnnBhcnNlclxuZGVhY3RpdmF0ZSBYUGF0aFBhcnNlclxuWE1MQ29uZmlnQnVpbGRlciAtPiBDb25maWd1cmF0aW9uOiBuZXcgQ29uZmlndXJhdGlvbigpXG5hY3RpdmF0ZSBDb25maWd1cmF0aW9uXG5Db25maWd1cmF0aW9uIC0tPiBYTUxDb25maWdCdWlsZGVyOui_lOWbnmNvbmZpZ3VyYXRpb25cbmRlYWN0aXZhdGUgQ29uZmlndXJhdGlvblxuZGVhY3RpdmF0ZSBYTUxDb25maWdCdWlsZGVyXG5TcWxTZXNzaW9uRmFjdG9yeUJ1aWxkZXIgLT4gWE1MQ29uZmlnQnVpbGRlcjogcGFyc2UoKVxuYWN0aXZhdGUgWE1MQ29uZmlnQnVpbGRlclxuWE1MQ29uZmlnQnVpbGRlciAtPiBYUGF0aFBhcnNlcjogZXZhbE5vZGUoXCIvY29uZmlndXJhdGlvblwiKVxuYWN0aXZhdGUgWFBhdGhQYXJzZXJcblhQYXRoUGFyc2VyIC0tPiBYTm9kZTpuZXcgWE5vZGUobm9kZSAuLi4pXG5hY3RpdmF0ZSBYTm9kZVxuWE5vZGUgLT4gWFBhdGhQYXJzZXI6IOi_lOWbnnhub2RlXG5kZWFjdGl2YXRlIFhOb2RlXG5YUGF0aFBhcnNlciAtPiBYTUxDb25maWdCdWlsZGVyOiDov5Tlm554bm9kZVxuZGVhY3RpdmF0ZSBYUGF0aFBhcnNlclxuWE1MQ29uZmlnQnVpbGRlciAtPiBYTm9kZTogZXZhbE5vZGUoXCJYWFhYXCIpXG5hY3RpdmF0ZSBYTm9kZVxuWE5vZGUgLT4gWE1MQ29uZmlnQnVpbGRlcjogWFhY5a2Q6IqC54K555qEWE5vZGXlr7nosaFcbmRlYWN0aXZhdGUgWE5vZGVcblhNTENvbmZpZ0J1aWxkZXIgLT4gWE1MQ29uZmlnQnVpbGRlcjog6Kej5p6QWFhY57G75Z6L55qEWE5vZGXoioLngrnvvIzojrflj5bnm7jlupTnmoTlgLxZWVlcblhNTENvbmZpZ0J1aWxkZXIgLT4gQ29uZmlndXJhdGlvbjogc2V0WFhYKFlZWSlcbmFjdGl2YXRlIENvbmZpZ3VyYXRpb25cbkNvbmZpZ3VyYXRpb24gLS0-IFhNTENvbmZpZ0J1aWxkZXJcbmRlYWN0aXZhdGUgQ29uZmlndXJhdGlvblxuWE1MQ29uZmlnQnVpbGRlciAtLT4gU3FsU2Vzc2lvbkZhY3RvcnlCdWlsZGVyOui_lOWbnuaehOmAoOWlveeahENvbmZpZ3VyYXRpb27lr7nosaFcbmRlYWN0aXZhdGUgWE1MQ29uZmlnQnVpbGRlclxuXG5AZW5kdW1sIiwidXJsIjoiaHR0cHM6Ly9jZG4ubmxhcmsuY29tL3l1cXVlL19fcHVtbC8wZWFmNDQ2YzQ5ODYzNDM0ZjkxZjk0ODUxNGJhM2FhMS5zdmciLCJpZCI6IlVhOWhYIiwibWFyZ2luIjp7InRvcCI6dHJ1ZSwiYm90dG9tIjp0cnVlfSwiY2FyZCI6ImRpYWdyYW0ifQ==)基于XML配置创建`Configuration`对象的过程
<a name="cceEX"></a>
### 7.3 基于Java API手动加载XML配置文件创建`Configuration`对象，并使用`SqlSessionFactory`对象
可以使用`XMLConfigBuilder`手动解析XML配置文件来创建`Configuration`对象，代码如下：
```java
String resource = "mybatis-config.xml";  

InputStream inputStream = Resources.getResourceAsStream(resource);  
// 手动创建XMLConfigBuilder，并解析创建Configuration对象  
XMLConfigBuilder parser = new XMLConfigBuilder(inputStream, null,null);  
Configuration configuration=parse();  
// 使用Configuration对象创建SqlSessionFactory  
SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(configuration);  
// 使用MyBatis  
SqlSession sqlSession = sqlSessionFactory.openSession();  
List list = sqlSession.selectList("com.foo.bean.BlogMapper.queryAllBlogInfo");
```
<a name="SwLa9"></a>
### 7.4 涉及到的设计模式
初始化的过程涉及到创建各种对象，所以会使用一些创建型的设计模式。**在初始化的过程中，**`**Builder**`**模式运用的比较多**。
<a name="y9IbQ"></a>
#### 7.4.1 Builder模式应用1：`SqlSessionFactory`的创建
对于创建`SqlSessionFactory`时，会**根据情况提供不同的参数，其参数组合可以有以下几种**：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651930397338-0c9f2feb-bca4-4d34-a720-0c925e6ff88f.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=220&id=uae312b05&originHeight=550&originWidth=1114&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80825&status=error&style=shadow&taskId=uec419709-e64b-4c80-83ec-c094cdc2661&title=&width=445.6)<br />根据情况提供不同的参数，创建`SqlSessionFactory`<br />由于构造时参数不定，可以为其创建一个构造器Builder，**将**`**SqlSessionFactory**`**的构建过程和表示分开**：<br />![MyBatis将SqlSessionFactoryBuilder和SqlSessionFactory相互独立](https://cdn.nlark.com/yuque/0/2022/png/396745/1651930274715-dba0e4c4-b885-4880-aac8-dc8ac65207a9.png#clientId=u0825a591-7b2d-4&errorMessage=unknown%20error&from=paste&height=454&id=uea271c4f&originHeight=1134&originWidth=2680&originalType=binary&ratio=1&rotation=0&showTitle=true&size=129349&status=error&style=shadow&taskId=ub208311f-efd7-46ef-b3cb-d45b02edece&title=MyBatis%E5%B0%86SqlSessionFactoryBuilder%E5%92%8CSqlSessionFactory%E7%9B%B8%E4%BA%92%E7%8B%AC%E7%AB%8B&width=1072 "MyBatis将SqlSessionFactoryBuilder和SqlSessionFactory相互独立")
<a name="sUij5"></a>
#### 7.4.2 Builder模式应用2：数据库连接环境`Environment`对象的创建
在构建`Configuration`对象的过程中，`XMLConfigBuilder`解析Mybatis XML配置文件节点`<environment>`节点时，会有以下相应的代码：
```java
private void environmentsElement(XNode context) throws Exception {  
	if (context != null) {  
		if (environment == null) {  
			environment = context.getStringAttribute("default");  
		}  
		for (XNode child : context.getChildren()) {  
			String id = child.getStringAttribute("id");  
			//是和默认的环境相同时，解析之  
			if (isSpecifiedEnvironment(id)) {  
				TransactionFactory txFactory = transactionManagerElement(child.evalNode("transactionManager"));  
				DataSourceFactory dsFactory = dataSourceElement(child.evalNode("dataSource"));  
				DataSource dataSource = dsFactory.getDataSource();  

				//使用了Environment内置的构造器Builder，传递id 事务工厂和数据源  
				Environment.Builder environmentBuilder = new Environment.Builder(id)  
					.transactionFactory(txFactory)  
					.dataSource(dataSource);  
				configuration.setEnvironment(environmentBuilder.build());  
			}  
		}  
	}  
}
```
**在**`**Environment**`**内部，定义了静态内部**`**Builder**`**类：**
```java
public final class Environment {
	private final String id;  
	private final TransactionFactory transactionFactory;  
	private final DataSource dataSource;  

	public Environment(String id, TransactionFactory transactionFactory, DataSource dataSource) {  
		if (id == null) {  
			throw new IllegalArgumentException("Parameter 'id' must not be null");  
		}  
		if (transactionFactory == null) {  
			throw new IllegalArgumentException("Parameter 'transactionFactory' must not be null");  
		}  
		this.id = id;  
		if (dataSource == null) {  
			throw new IllegalArgumentException("Parameter 'dataSource' must not be null");  
		}  
		this.transactionFactory = transactionFactory;  
		this.dataSource = dataSource;  
	}  

	public static class Builder {  
		private String id;  
		private TransactionFactory transactionFactory;  
		private DataSource dataSource;  

		public Builder(String id) {  
			this.id = id;  
		}  

		public Builder transactionFactory(TransactionFactory transactionFactory) {  
			this.transactionFactory = transactionFactory;  
			return this;  
		}  

		public Builder dataSource(DataSource dataSource) {  
			this.dataSource = dataSource;  
			return this;  
		}  

		public String id() {  
			return this.id;  
		}  

		public Environment build() {  
			return new Environment(this.id, this.transactionFactory, this.dataSource);  
		}  
	}  

	public String getId() {  
		return this.id;  
	}  

	public TransactionFactory getTransactionFactory() {  
		return this.transactionFactory;  
	}  

	public DataSource getDataSource() {  
		return this.dataSource;  
	}
}
```
