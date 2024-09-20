JavaMyBatis
<a name="mE5Dq"></a>
## 一、MyBatis动态 sql 是什么
动态 SQL 是 MyBatis 的强大特性之一。在 JDBC 或其它类似的框架中，开发人员通常需要手动拼接 SQL 语句。根据不同的条件拼接 SQL 语句是一件极其痛苦的工作。例如，拼接时要确保添加了必要的空格，还要注意去掉列表最后一个列名的逗号。而动态 SQL 恰好解决了这一问题，可以根据场景动态的构建查询。<br />动态SQL（code that is executed dynamically），它一般是根据用户输入或外部条件动态组合的SQL语句块。 动态SQL能灵活的发挥SQL强大的功能、方便的解决一些其它方法难以解决的问题。 相信使用过动态SQL的人都能体会到它带来的便利，然而动态SQL有时候在执行性能 (效率)上面不如静态SQL，而且使用不恰当，往往会在安全方面存在隐患 (SQL 注入式攻击)。
<a name="RNHEK"></a>
### 1、Mybatis 动态 sql 是做什么的?
Mybatis 动态 sql 可以在 Xml 映射文件内，以标签的形式编写动态 sql，完成逻辑判断和动态拼接 sql 的功能。
<a name="VtQCl"></a>
### 2、Mybatis 的 9 种 动 态 sql 标 签有哪些？
| 元素 | 作用 | 备注 |
| --- | --- | --- |
| if | 判断语句 | 单条件分支判断  |
| choose (when、otherwise) | 相当于Java 中的 switch case 语句 | 多条件分支判断  |
| trim,where | 辅助元素 | 用于处理一些SQL拼装问题 |
| foreach | 循环语句 | 在in语句等列举条件常用 |
| bind | 辅助元素 | 拼接参数 |

<a name="Sg3AX"></a>
### 3、动态 sql 的执行原理？
原理为：使用 OGNL 从 sql 参数对象中计算表达式的值，根据表达式的值动态拼接 sql，以此来完成动态 sql 的功能。
<a name="HsI4H"></a>
## 二、MyBatis标签
<a name="sFxZm"></a>
### 1、if标签：条件判断
MyBatis if 类似于 Java 中的 if 语句，是 MyBatis 中最常用的判断语句。使用 if 标签可以节省许多拼接 SQL 的工作，把精力集中在 XML 的维护上。
<a name="m5ipZ"></a>
#### 1）不使用动态sql
```xml
<select id="selectUserByUsernameAndSex"
  resultType="user" parameterType="com.ys.po.User">
  <!-- 这里和普通的sql 查询语句差不多，对于只有一个参数，后面的 #{id}表示占位符，里面          不一定要写id,
  写啥都可以，但是不要空着，如果有多个参数则必须写pojo类里面的属性 -->
  select * from user where username=#{username} and sex=#{sex}
</select>
```
if 语句使用方法简单，常常与 test 属性联合使用。语法如下：
```xml
<if test="判断条件">
  SQL语句
</if>
```
<a name="MKzwf"></a>
#### 2）使用动态sql
上面的查询语句，可以发现，如果 #{username} 为空，那么查询结果也是空，如何解决这个问题呢？使用 if 来判断，可多个 if 语句同时使用。<br />以下语句表示为可以按照网站名称（name）或者网址（url）进行模糊查询。如果您不输入名称或网址，则返回所有的网站记录。但是，如果你传递了任意一个参数，它就会返回与给定参数相匹配的记录。
```xml
<select id="selectAllWebsite" resultMap="myResult">  
    select id,name,url from website 
    where 1=1    
   <if test="name != null">        
       AND name like #{name}   
   </if>    
   <if test="url!= null">        
       AND url like #{url}    
   </if>
</select>
```
<a name="Sx76i"></a>
### 2、where+if标签
where、if同时使用可以进行查询、模糊查询<br />注意，`<if>`失败后， `<where>` 关键字只会去掉库表字段赋值前面的and，不会去掉语句后面的and关键字，即注意，`<where>` 只会去掉`<if>` 语句中的最开始的and关键字。所以下面的形式是不可取的
```xml
<select id="findQuery" resultType="Student">
    <include refid="selectvp"/>
    <where>
        <if test="sacc != null">
            sacc like concat('%' #{sacc} '%')
        </if>
        <if test="sname != null">
            AND sname like concat('%' #{sname} '%')
        </if>
        <if test="sex != null">
            AND sex=#{sex}
        </if>
        <if test="phone != null">
            AND phone=#{phone}
        </if>
    </where>
</select>
```
这个“where”标签会知道如果它包含的标签中有返回值的话，它就插入一个‘where’。此外，如果标签返回的内容是以AND 或OR 开头的，则它会剔除掉。
<a name="Zi6Wc"></a>
### 3、set标签
set可以用来修改
```xml
<update id="upd">
    update student
    <set>
        <if test="sname != null">sname=#{sname},</if>
        <if test="spwd != null">spwd=#{spwd},</if>
        <if test="sex != null">sex=#{sex},</if>
        <if test="phone != null">phone=#{phone}</if>
    sid=#{sid}
    </set>
    where sid=#{sid}
</update>
```
<a name="wjtPm"></a>
### 4、choose(when,otherwise) 语句
有时候，不想用到所有的查询条件，只想选择其中的一个，查询条件有一个满足即可，使用 choose 标签可以解决此类问题，类似于 Java 的 switch 语句
```xml
<select id="selectUserByChoose" resultType="com.ys.po.User" parameterType="com.ys.po.User">
      select * from user
      <where>
          <choose>
              <when test="id !='' and id != null">
                  id=#{id}
              </when>
              <when test="username !='' and username != null">
                  and username=#{username}
              </when>
              <otherwise>
                  and sex=#{sex}
              </otherwise>
          </choose>
      </where>
  </select>
```
也就是说，这里有三个条件，id、username、sex，只能选择一个作为查询条件

- 如果 id 不为空，那么查询语句为：`select * from user where id=?`
- 如果 id 为空，那么看username 是否为空，如果不为空，那么语句为 `select * from user where username=?;`
- 如果 username 为空，那么查询语句为 `select * from user where sex=?`
<a name="UEf9u"></a>
### 5、trim
trim标记是一个格式化的标记，可以完成set或者是where标记的功能<br />**①、用 trim 改写上面第二点的 if+where 语句**

- prefix：前缀
- prefixoverride：去掉第一个and或者是or
```xml
<select id="selectUserByUsernameAndSex" resultType="user" parameterType="com.ys.po.User">
    select * from user
    <!-- <where>
        <if test="username != null">
           username=#{username}
        </if>
         
        <if test="username != null">
           and sex=#{sex}
        </if>
    </where>  -->
    <trim prefix="where" prefixOverrides="and | or">
        <if test="username != null">
           and username=#{username}
        </if>
        <if test="sex != null">
           and sex=#{sex}
        </if>
    </trim>
</select>
```
**②、用 trim 改写上面第三点的 if+set 语句**

- suffix：后缀
- suffixoverride：去掉最后一个逗号（也可以是其他的标记，就像是上面前缀中的and一样）
```xml
<!-- 根据 id 更新 user 表的数据 -->
<update id="updateUserById" parameterType="com.ys.po.User">
    update user u
        <!-- <set>
            <if test="username != null and username != ''">
                u.username = #{username},
            </if>
            <if test="sex != null and sex != ''">
                u.sex = #{sex}
            </if>
        </set> -->
        <trim prefix="set" suffixOverrides=",">
            <if test="username != null and username != ''">
                u.username = #{username},
            </if>
            <if test="sex != null and sex != ''">
                u.sex = #{sex},
            </if>
        </trim>
     
     where id=#{id}
</update>
```
**③、trim+if同时使用可以添加**
```xml
<insert id="add">
    insert  into student
    <trim prefix="(" suffix=")" suffixOverrides=",">
        <if test="sname != null">sname,</if>
        <if test="spwd != null">spwd,</if>
        <if test="sex != null">sex,</if>
        <if test="phone != null">phone,</if>
    </trim>

    <trim prefix="values (" suffix=")"  suffixOverrides=",">
        <if test="sname != null">#{sname},</if>
        <if test="spwd != null">#{spwd},</if>
        <if test="sex != null">#{sex},</if>
        <if test="phone != null">#{phone}</if>
    </trim>

</insert>
```
<a name="a1Hhv"></a>
### 6、MyBatis foreach标签
foreach是用来对集合的遍历，这个和Java中的功能很类似。通常处理SQL中的in语句。<br />foreach 元素的功能非常强大，它允许你指定一个集合，声明可以在元素体内使用的集合项（item）和索引（index）变量。它也允许你指定开头与结尾的字符串以及集合项迭代之间的分隔符。这个元素也不会错误地添加多余的分隔符<br />你可以将任何可迭代对象（如 List、Set 等）、Map 对象或者数组对象作为集合参数传递给 foreach。当使用可迭代对象或者数组时，index 是当前迭代的序号，item 的值是本次迭代获取到的元素。当使用 Map 对象（或者 Map.Entry 对象的集合）时，index 是键，item 是值。
```xml
//批量查询
<select id="findAll" resultType="Student" parameterType="Integer">
    <include refid="selectvp"/> WHERE sid in
    <foreach item="ids" collection="array"  open="(" separator="," close=")">
        #{ids}
    </foreach>
</select>
//批量删除
<delete id="del"  parameterType="Integer">
    delete  from  student  where  sid in
    <foreach item="ids" collection="array"  open="(" separator="," close=")">
        #{ids}
    </foreach>
</delete>
```
<a name="OoEbB"></a>
#### 整合案例
<a name="aOYQW"></a>
##### xml
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.yzx.mapper.StuMapper">
    <sql id="selectvp">
        select  *  from  student
    </sql>
    
    <select id="find" resultType="Student">
        <include refid="selectvp"/>
    </select>

    <select id="findbyid"  resultType="student">
        <include refid="selectvp"/>
        WHERE 1=1
        <if test="sid != null">
            AND sid like #{sid}
        </if>
    </select>

        <select id="findQuery" resultType="Student">
            <include refid="selectvp"/>
            <where>
                <if test="sacc != null">
                    sacc like concat('%' #{sacc} '%')
                </if>
                <if test="sname != null">
                    AND sname like concat('%' #{sname} '%')
                </if>
                <if test="sex != null">
                    AND sex=#{sex}
                </if>
                <if test="phone != null">
                    AND phone=#{phone}
                </if>
            </where>
        </select>

    <update id="upd">
        update student
        <set>
            <if test="sname != null">sname=#{sname},</if>
            <if test="spwd != null">spwd=#{spwd},</if>
            <if test="sex != null">sex=#{sex},</if>
            <if test="phone != null">phone=#{phone}</if>
        sid=#{sid}
        </set>
        where sid=#{sid}
    </update>

    <insert id="add">
        insert  into student
        <trim prefix="(" suffix=")" suffixOverrides=",">
            <if test="sname != null">sname,</if>
            <if test="spwd != null">spwd,</if>
            <if test="sex != null">sex,</if>
            <if test="phone != null">phone,</if>
        </trim>

        <trim prefix="values (" suffix=")"  suffixOverrides=",">
            <if test="sname != null">#{sname},</if>
            <if test="spwd != null">#{spwd},</if>
            <if test="sex != null">#{sex},</if>
            <if test="phone != null">#{phone}</if>
        </trim>

    </insert>
    <select id="findAll" resultType="Student" parameterType="Integer">
        <include refid="selectvp"/> WHERE sid in
        <foreach item="ids" collection="array"  open="(" separator="," close=")">
            #{ids}
        </foreach>
    </select>

    <delete id="del"  parameterType="Integer">
        delete  from  student  where  sid in
        <foreach item="ids" collection="array"  open="(" separator="," close=")">
            #{ids}
        </foreach>
    </delete>
</mapper>
```
<a name="HRKQ8"></a>
##### 测试类
```java
package com.yzx.test;

import com.yzx.entity.Student;
import com.yzx.mapper.StuMapper;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

public class StuTest {
    SqlSession sqlSession=null;
    InputStream is=null;

    @Before
    public   void  before() throws IOException {
        //1.读取核心配置文件
        is= Resources.getResourceAsStream("sqlMapperConfig.xml");
        //2.拿到工厂构建类
        SqlSessionFactoryBuilder sqlSessionFactoryBuilder=new SqlSessionFactoryBuilder();
        //3.拿到具体工厂
        SqlSessionFactory build=sqlSessionFactoryBuilder.build(is);
        //4.拿到session
        sqlSession = build.openSession();
    }

    @After
    public  void  after(){
        //7，提交事务
        sqlSession.commit();
        //8.关闭资源
        sqlSession.close();
        if(is!=null){
            try {
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        };
    }

    //查询所有
    @Test
    public  void  find(){
        //5.获取具体的mapper接口
        StuMapper mapper=sqlSession.getMapper(StuMapper.class);
        //6.调用执行
        List<Student> list=mapper.find();
        list.forEach(a-> System.out.println(a));
    }
    //查询单个
    @Test
    public  void  findbyid(){

        StuMapper mapper=sqlSession.getMapper(StuMapper.class);
        List<Student> list=mapper.findbyid(2);
        list.forEach(a-> System.out.println(a));
    }
    //模糊查询
    @Test
    public  void  findQuery(){

        StuMapper mapper=sqlSession.getMapper(StuMapper.class);

        Student  stu=new Student();
        stu.setSname("小");
        stu.setSex("男");
        List<Student> list=mapper.findQuery(stu);
        list.forEach(a-> System.out.println(a));
    }
    //修改
    @Test
    public  void  upd(){

        StuMapper mapper=sqlSession.getMapper(StuMapper.class);

        Student  stu=new Student();
        stu.setSid(3);
        stu.setSname("小若");
        stu.setSex("人妖");
        int i=mapper.upd(stu);
        System.out.println("修改了"+i+"条数据"+"  "+stu.toString());

    }
    //添加
    @Test
    public  void  add(){

        StuMapper mapper=sqlSession.getMapper(StuMapper.class);

        Student  stu=new Student();
        stu.setSname("小贺");
        stu.setSex("男");
        stu.setPhone("99999999");
        int i=mapper.add(stu);
        System.out.println("添加了"+i+"条数据"+"  "+stu.toString());

    }

    //批量操作
    @Test
    public  void  findAll(){

        StuMapper mapper=sqlSession.getMapper(StuMapper.class);
        Integer[] i={1,2,3,4};
        List<Student> list=mapper.findAll(i);
        list.forEach(a-> System.out.println(a));
    }
    //批量操作

    //批量删除
    @Test
    public  void  del(){
        StuMapper mapper=sqlSession.getMapper(StuMapper.class);
        Integer[] i={1,2,3,4};
        int i1=mapper.del(i);
        System.out.println("删除了"+i1+"条数据");
    }
}
```
<a name="G1pLq"></a>
### 7、sql
在实际开发中会遇到许多相同的SQL，比如根据某个条件筛选，这个筛选很多地方都能用到，可以将其抽取出来成为一个公用的部分，这样修改也方便，一旦出现了错误，只需要改这一处便能处处生效了，此时就用到了`<sql>`这个标签了。<br />当多种类型的查询语句的查询字段或者查询条件相同时，可以将其定义为常量，方便调用。为求`<select>`结构清晰也可将 sql 语句分解。
```xml
<sql id="selectvp">
    select  *  from  student
</sql>
```
<a name="mI0Ed"></a>
### 8、`include`
这个标签和`<sql>`是天仙配，是共生的，include用于引用sql标签定义的常量。比如引用上面sql标签定义的常量<br />refid这个属性就是指定`<sql>`标签中的id值（唯一标识）
```xml
<select id="findbyid"  resultType="student">
    <include refid="selectvp"/>
    WHERE 1=1
    <if test="sid != null">
        AND sid like #{sid}
    </if>
</select>
```
<a name="yXukc"></a>
### 9、如何引用其他XML中的SQL片段
比如在com.xxx.dao.xxMapper这个Mapper的XML中定义了一个SQL片段如下：
```xml
<sql id="Base_Column_List"> ID,MAJOR,BIRTHDAY,AGE,NAME,HOBBY</sql>
```
此时在com.xxx.dao.PatinetMapper中的XML文件中需要引用，如下：
```xml
<include refid="com.xxx.dao.xxMapper.Base_Column_List"></include>
```
<a name="FogJN"></a>
## 三、MyBatis关联查询
<a name="gQAPG"></a>
### 1、MyBatis一对多关联查询
```xml
<!--一对多-->
<resultMap id="myStudent1" type="student1">
    <id property="sid" column="sid"/>
    <result property="sname" column="sname"/>
    <result property="sex" column="sex"/>
    <result property="sage" column="sage"/>
    <collection property="list" ofType="teacher">
        <id property="tid" column="tid"/>
        <result property="tname" column="tname"/>
        <result property="tage" column="tage"/>
    </collection>
</resultMap>

<!--一对多-->
<select id="find1" resultMap="myStudent1">
    select  *  from  student1  s  left  join  teacher  t  on s.sid=t.sid
</select>
```
<a name="vtljv"></a>
### 2、MyBatis多对一关联查询
```xml
<!--多对一-->
<resultMap id="myTeacher" type="teacher">
    <id property="tid" column="tid"/>
    <result property="tname" column="tname"/>
    <result property="tage" column="tage"/>
    <association property="student1" javaType="Student1">
        <id property="sid" column="sid"/>
        <result property="sname" column="sname"/>
        <result property="sex" column="sex"/>
        <result property="sage" column="sage"/>
    </association>
</resultMap>


<!--多对一-->
<select id="find2" resultMap="myTeacher">
	select  *  from  teacher  t right join student1 s on  t.sid=s.sid
</select>
```
<a name="L8WCD"></a>
### 3、MyBatis多对多关联查询
```xml
<!--多对多 以谁为主表查询的时候，主表约等于1的一方,另一方相当于多的一方-->
<select id="find3" resultMap="myStudent1">
    select  *  from  student1 s  left join relevance r on  s.sid=r.sid  left join teacher t on  r.tid=t.tid
</select>
```


