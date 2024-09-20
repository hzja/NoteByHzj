Mybatis
<a name="huPpt"></a>
## 用来循环容器的标签forEach,查看例子
foreach元素的属性主要有item，index，collection，open，separator，close。

- item：集合中元素迭代时的别名，
- index：集合中元素迭代时的索引
- open：常用语where语句中，表示以什么开始，比如以'('开始
- separator：表示在每次进行迭代时的分隔符，
- close 常用语where语句中，表示以什么结束，

在使用foreach的时候最关键的也是最容易出错的就是collection属性，该属性是必须指定的，但是在不同情况下，该属性的值是不一样的，主要有一下3种情况：

- 如果传入的是单参数且参数类型是一个List的时候，collection属性值为list .
- 如果传入的是单参数且参数类型是一个array数组的时候，collection的属性值为array .
- 如果传入的参数是多个的时候，就需要把它们封装成一个Map了，当然单参数也可以封装成map，实际上如果在传入参数的时候，在MyBatis里面也是会把它封装成一个Map的，map的key就是参数名，所以这个时候collection属性值就是传入的List或array对象在自己封装的map里面的key.

针对最后一条，来看一下官方说法：
> 注意：可以将一个 List 实例或者数组作为参数对象传给 MyBatis，当这么做的时候，MyBatis 会自动将它包装在一个 Map 中并以名称为键。List 实例将会以“list”作为键，而数组实例的键将是“array”。

所以，不管是多参数还是单参数的list,array类型，都可以封装为map进行传递。如果传递的是一个List，则mybatis会封装为一个list为key，list值为object的map，如果是array，则封装成一个array为key，array的值为object的map，如果自己封装呢，则colloection里放的是自己封装的map里的key值
```xml
//mapper中要为这个方法传递的是一个容器,将容器中的元素一个一个的
//拼接到xml的方法中就要使用这个forEach这个标签了
public List<Entity> queryById(List<String> userids);

//对应的xml中如下
<select id="queryById" resultMap="BaseReslutMap" >
    select * FROM entity
    where id in 
    <foreach collection="userids" item="userid" index="index" open="(" separator="," close=")">
            #{userid}
    </foreach>
</select>
```
<a name="GMOi5"></a>
## `concat`模糊查询
```xml
//比如说想要进行条件查询,但是几个条件不是每次都要使用,那么就可以
//通过判断是否拼接到sql中
<select id="queryById" resultMap="BascResultMap" parameterType="entity">
  SELECT *  from entity
  <where>
      <if test="name!=null">
          name like concat('%',concat(#{name},'%'))
      </if>
  </where>
</select>
```
<a name="PuTEj"></a>
## `choose (when, otherwise)`标签
choose标签是按顺序判断其内部when标签中的test条件出否成立，如果有一个成立，则 choose 结束。当 choose 中所有 when 的条件都不满则时，则执行 otherwise 中的sql。类似于Java 的 switch 语句，choose 为 switch，when 为 case，otherwise 则为 default。<br />例如下面例子，同样把所有可以限制的条件都写上，方面使用。choose会从上到下选择一个when标签的test为true的sql执行。安全考虑，使用where将choose包起来，放置关键字多于错误。
```xml
<!--  choose(判断参数) - 按顺序将实体类 User 第一个不为空的属性作为：where条件 -->  
<select id="getUserList_choose" resultMap="resultMap_user" parameterType="com.yiibai.pojo.User">  
    SELECT *  
      FROM User u   
    <where>  
        <choose>  
            <when test="username !=null ">  
                u.username LIKE CONCAT(CONCAT('%', #{username, jdbcType=VARCHAR}),'%')  
            </when >  
            <when test="sex != null and sex != '' ">  
                AND u.sex = #{sex, jdbcType=INTEGER}  
            </when >  
            <when test="birthday != null ">  
                AND u.birthday = #{birthday, jdbcType=DATE}  
            </when >  
            <otherwise>  
            </otherwise>  
        </choose>  
    </where>    
</select>
```
<a name="vN7Oo"></a>
## selectKey 标签
在insert语句中，在Oracle经常使用序列、在MySQL中使用函数来自动生成插入表的主键，而且需要方法能返回这个生成主键。使用myBatis的selectKey标签可以实现这个效果。<br />下面例子，使用mysql数据库自定义函数`nextval('student')`，用来生成一个key，并把他设置到传入的实体类中的studentId属性上。所以在执行完此方法后，边可以通过这个实体类获取生成的key。
```xml
<!-- 插入学生 自动主键-->  
<insert id="createStudentAutoKey" parameterType="liming.student.manager.data.model.StudentEntity" keyProperty="studentId">  
    <selectKey keyProperty="studentId" resultType="String" order="BEFORE">  
        select nextval('student')  
    </selectKey>  
    INSERT INTO STUDENT_TBL(STUDENT_ID,  
                            STUDENT_NAME,  
                            STUDENT_SEX,  
                            STUDENT_BIRTHDAY,  
                            STUDENT_PHOTO,  
                            CLASS_ID,  
                            PLACE_ID)  
    VALUES (#{studentId},  
            #{studentName},  
            #{studentSex},  
            #{studentBirthday},  
            #{studentPhoto, javaType=byte[], jdbcType=BLOB, typeHandler=org.apache.ibatis.type.BlobTypeHandler},  
            #{classId},  
            #{placeId})  
</insert>
```
调用接口方法，和获取自动生成key
```java
StudentEntity entity = new StudentEntity();  
entity.setStudentName("黎明你好");  
entity.setStudentSex(1);  
entity.setStudentBirthday(DateUtil.parse("1985-05-28"));  
entity.setClassId("20000001");  
entity.setPlaceId("70000001");  
this.dynamicSqlMapper.createStudentAutoKey(entity);  
System.out.println("新增学生ID: " + entity.getStudentId());
```
<a name="qKH8z"></a>
## if标签
if标签可用在许多类型的sql语句中，以查询为例。首先看一个很普通的查询：
```xml
<!-- 查询学生list，like姓名 -->  
<select id="getStudentListLikeName" parameterType="StudentEntity" resultMap="studentResultMap">  
    SELECT * from STUDENT_TBL ST   
		WHERE ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName}),'%')  
</select>
```
但是此时如果studentName为null，此语句很可能报错或查询结果为空。此时使用if动态sql语句先进行判断，如果值为null或等于空字符串，就不进行此条件的判断，增加灵活性。<br />参数为实体类StudentEntity。将实体类中所有的属性均进行判断，如果不为空则执行判断条件。
```xml
<!-- 2 if(判断参数) - 将实体类不为空的属性作为where条件 -->  
<select id="getStudentList_if" resultMap="resultMap_studentEntity" parameterType="liming.student.manager.data.model.StudentEntity">  
    SELECT ST.STUDENT_ID,  
           ST.STUDENT_NAME,  
           ST.STUDENT_SEX,  
           ST.STUDENT_BIRTHDAY,  
           ST.STUDENT_PHOTO,  
           ST.CLASS_ID,  
           ST.PLACE_ID  
      FROM STUDENT_TBL ST   
     WHERE  
    <if test="studentName !=null ">  
        ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName, jdbcType=VARCHAR}),'%')  
    </if>  
    <if test="studentSex != null and studentSex != '' ">  
        AND ST.STUDENT_SEX = #{studentSex, jdbcType=INTEGER}  
    </if>  
    <if test="studentBirthday != null ">  
        AND ST.STUDENT_BIRTHDAY = #{studentBirthday, jdbcType=DATE}  
    </if>  
    <if test="classId != null and classId!= '' ">  
        AND ST.CLASS_ID = #{classId, jdbcType=VARCHAR}  
    </if>  
    <if test="classEntity != null and classEntity.classId !=null and classEntity.classId !=' ' ">  
        AND ST.CLASS_ID = #{classEntity.classId, jdbcType=VARCHAR}  
    </if>  
    <if test="placeId != null and placeId != '' ">  
        AND ST.PLACE_ID = #{placeId, jdbcType=VARCHAR}  
    </if>  
    <if test="placeEntity != null and placeEntity.placeId != null and placeEntity.placeId != '' ">  
        AND ST.PLACE_ID = #{placeEntity.placeId, jdbcType=VARCHAR}  
    </if>  
    <if test="studentId != null and studentId != '' ">  
        AND ST.STUDENT_ID = #{studentId, jdbcType=VARCHAR}  
    </if>   
</select>
```
使用时比较灵活， new一个这样的实体类，需要限制那个条件，只需要附上相应的值就会where这个条件，相反不去赋值就可以不在where中判断。
```java
public void select_test_2_1() {  
    StudentEntity entity = new StudentEntity();  
    entity.setStudentName("");  
    entity.setStudentSex(1);  
    entity.setStudentBirthday(DateUtil.parse("1985-05-28"));  
    entity.setClassId("20000001");  
    //entity.setPlaceId("70000001");  
    List<StudentEntity> list = this.dynamicSqlMapper.getStudentList_if(entity);  
    for (StudentEntity e : list) {  
        System.out.println(e.toString());  
    }  
}
```
<a name="ZmARj"></a>
## if + where 的条件判断
当where中的条件使用的if标签较多时，这样的组合可能会导致错误。以在3.1中的查询语句为例子，当java代码按如下方法调用时：
```java
@Test  
public void select_test_2_1() {  
    StudentEntity entity = new StudentEntity();  
    entity.setStudentName(null);  
    entity.setStudentSex(1);  
    List<StudentEntity> list = this.dynamicSqlMapper.getStudentList_if(entity);  
    for (StudentEntity e : list) {  
        System.out.println(e.toString());  
    }  
}
```
如果上面例子，参数studentName为null，将不会进行STUDENT_NAME列的判断，则会直接导“WHERE AND”关键字多余的错误SQL。<br />这时可以使用where动态语句来解决。这个“where”标签会知道如果它包含的标签中有返回值的话，它就插入一个‘where’。此外，如果标签返回的内容是以AND 或OR 开头的，则它会剔除掉。<br />上面例子修改为：
```xml
<!-- 3 select - where/if(判断参数) - 将实体类不为空的属性作为where条件 -->  
<select id="getStudentList_whereIf" resultMap="resultMap_studentEntity" parameterType="liming.student.manager.data.model.StudentEntity">  
    SELECT ST.STUDENT_ID,  
           ST.STUDENT_NAME,  
           ST.STUDENT_SEX,  
           ST.STUDENT_BIRTHDAY,  
           ST.STUDENT_PHOTO,  
           ST.CLASS_ID,  
           ST.PLACE_ID  
      FROM STUDENT_TBL ST   
    <where>  
        <if test="studentName !=null ">  
            ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName, jdbcType=VARCHAR}),'%')  
        </if>  
        <if test="studentSex != null and studentSex != '' ">  
            AND ST.STUDENT_SEX = #{studentSex, jdbcType=INTEGER}  
        </if>  
        <if test="studentBirthday != null ">  
            AND ST.STUDENT_BIRTHDAY = #{studentBirthday, jdbcType=DATE}  
        </if>  
        <if test="classId != null and classId!= '' ">  
            AND ST.CLASS_ID = #{classId, jdbcType=VARCHAR}  
        </if>  
        <if test="classEntity != null and classEntity.classId !=null and classEntity.classId !=' ' ">  
            AND ST.CLASS_ID = #{classEntity.classId, jdbcType=VARCHAR}  
        </if>  
        <if test="placeId != null and placeId != '' ">  
            AND ST.PLACE_ID = #{placeId, jdbcType=VARCHAR}  
        </if>  
        <if test="placeEntity != null and placeEntity.placeId != null and placeEntity.placeId != '' ">  
            AND ST.PLACE_ID = #{placeEntity.placeId, jdbcType=VARCHAR}  
        </if>  
        <if test="studentId != null and studentId != '' ">  
            AND ST.STUDENT_ID = #{studentId, jdbcType=VARCHAR}  
        </if>  
    </where>    
</select>
```
<a name="KStOH"></a>
## if + set实现修改语句
当update语句中没有使用if标签时，如果有一个参数为null，都会导致错误。<br />当在update语句中使用if标签时，如果前面的if没有执行，则或导致逗号多余错误。使用set标签可以将动态的配置SET 关键字，和剔除追加到条件末尾的任何不相关的逗号。使用if+set标签修改后，如果某项为null则不进行更新，而是保持数据库原值。<br />如下示例：
```xml
<!-- 4 if/set(判断参数) - 将实体类不为空的属性更新 -->  
<update id="updateStudent_if_set" parameterType="liming.student.manager.data.model.StudentEntity">  
    UPDATE STUDENT_TBL  
    <set>  
        <if test="studentName != null and studentName != '' ">  
            STUDENT_TBL.STUDENT_NAME = #{studentName},  
        </if>  
        <if test="studentSex != null and studentSex != '' ">  
            STUDENT_TBL.STUDENT_SEX = #{studentSex},  
        </if>  
        <if test="studentBirthday != null ">  
            STUDENT_TBL.STUDENT_BIRTHDAY = #{studentBirthday},  
        </if>  
        <if test="studentPhoto != null ">  
            STUDENT_TBL.STUDENT_PHOTO = #{studentPhoto, javaType=byte[], jdbcType=BLOB, typeHandler=org.apache.ibatis.type.BlobTypeHandler},  
        </if>  
        <if test="classId != '' ">  
            STUDENT_TBL.CLASS_ID = #{classId}  
        </if>  
        <if test="placeId != '' ">  
            STUDENT_TBL.PLACE_ID = #{placeId}  
        </if>  
    </set>  
    WHERE STUDENT_TBL.STUDENT_ID = #{studentId};      
</update>
```
<a name="ebzHR"></a>
## if + trim代替where/set标签
trim是更灵活的去处多余关键字的标签，他可以实践where和set的效果。
<a name="DS5w5"></a>
### trim代替where
```xml
<!-- 5.1if/trim代替where(判断参数) -将实体类不为空的属性作为where条件-->  
<select id="getStudentList_if_trim" resultMap="resultMap_studentEntity">  
    SELECT ST.STUDENT_ID,  
           ST.STUDENT_NAME,  
           ST.STUDENT_SEX,  
           ST.STUDENT_BIRTHDAY,  
           ST.STUDENT_PHOTO,  
           ST.CLASS_ID,  
           ST.PLACE_ID  
      FROM STUDENT_TBL ST   
    <trim prefix="WHERE" prefixOverrides="AND|OR">  
        <if test="studentName !=null ">  
            ST.STUDENT_NAME LIKE CONCAT(CONCAT('%', #{studentName, jdbcType=VARCHAR}),'%')  
        </if>  
        <if test="studentSex != null and studentSex != '' ">  
            AND ST.STUDENT_SEX = #{studentSex, jdbcType=INTEGER}  
        </if>  
        <if test="studentBirthday != null ">  
            AND ST.STUDENT_BIRTHDAY = #{studentBirthday, jdbcType=DATE}  
        </if>  
        <if test="classId != null and classId!= '' ">  
            AND ST.CLASS_ID = #{classId, jdbcType=VARCHAR}  
        </if>  
        <if test="classEntity != null and classEntity.classId !=null and classEntity.classId !=' ' ">  
            AND ST.CLASS_ID = #{classEntity.classId, jdbcType=VARCHAR}  
        </if>  
        <if test="placeId != null and placeId != '' ">  
            AND ST.PLACE_ID = #{placeId, jdbcType=VARCHAR}  
        </if>  
        <if test="placeEntity != null and placeEntity.placeId != null and placeEntity.placeId != '' ">  
            AND ST.PLACE_ID = #{placeEntity.placeId, jdbcType=VARCHAR}  
        </if>  
        <if test="studentId != null and studentId != '' ">  
            AND ST.STUDENT_ID = #{studentId, jdbcType=VARCHAR}  
        </if>  
    </trim>     
</select>
```
<a name="mIyp0"></a>
### trim代替set
```xml
<!-- 5.2 if/trim代替set(判断参数) - 将实体类不为空的属性更新 -->  
<update id="updateStudent_if_trim" parameterType="liming.student.manager.data.model.StudentEntity">  
    UPDATE STUDENT_TBL  
    <trim prefix="SET" suffixOverrides=",">  
        <if test="studentName != null and studentName != '' ">  
            STUDENT_TBL.STUDENT_NAME = #{studentName},  
        </if>  
        <if test="studentSex != null and studentSex != '' ">  
            STUDENT_TBL.STUDENT_SEX = #{studentSex},  
        </if>  
        <if test="studentBirthday != null ">  
            STUDENT_TBL.STUDENT_BIRTHDAY = #{studentBirthday},  
        </if>  
        <if test="studentPhoto != null ">  
            STUDENT_TBL.STUDENT_PHOTO = #{studentPhoto, javaType=byte[], jdbcType=BLOB, typeHandler=org.apache.ibatis.type.BlobTypeHandler},  
        </if>  
        <if test="classId != '' ">  
            STUDENT_TBL.CLASS_ID = #{classId},  
        </if>  
        <if test="placeId != '' ">  
            STUDENT_TBL.PLACE_ID = #{placeId}  
        </if>  
    </trim>  
    WHERE STUDENT_TBL.STUDENT_ID = #{studentId}  
</update>
```
<a name="gy0eE"></a>
## foreach
对于动态SQL 非常必须的，主是要迭代一个集合，通常是用于IN 条件。List 实例将使用“list”做为键，数组实例以“array” 做为键。<br />foreach元素是非常强大的，它允许你指定一个集合，声明集合项和索引变量，它们可以用在元素体内。它也允许你指定开放和关闭的字符串，在迭代之间放置分隔符。这个元素是很智能的，它不会偶然地附加多余的分隔符。
> 注意：可以传递一个List实例或者数组作为参数对象传给MyBatis。当你这么做的时候，MyBatis会自动将它包装在一个Map中，用名称在作为键。List实例将会以“list”作为键，而数组实例将会以“array”作为键。

这个部分是对关于XML配置文件和XML映射文件的而讨论的。下一部分将详细讨论Java API，所以你可以得到你已经创建的最有效的映射。
<a name="UsRuA"></a>
### 1、参数为array示例的写法
接口的方法声明：
```java
public List<StudentEntity> getStudentListByClassIds_foreach_array(String[] classIds);
```
动态SQL语句：
```xml
<!-- 7.1 foreach(循环array参数) - 作为where中in的条件 -->  
<select id="getStudentListByClassIds_foreach_array" resultMap="resultMap_studentEntity">  
    SELECT ST.STUDENT_ID,  
           ST.STUDENT_NAME,  
           ST.STUDENT_SEX,  
           ST.STUDENT_BIRTHDAY,  
           ST.STUDENT_PHOTO,  
           ST.CLASS_ID,  
           ST.PLACE_ID  
      FROM STUDENT_TBL ST  
      WHERE ST.CLASS_ID IN   
     <foreach collection="array" item="classIds"  open="(" separator="," close=")">  
        #{classIds}  
     </foreach>  
</select>
```
测试代码，查询学生中，在20000001、20000002这两个班级的学生：
```java
@Test  
public void test7_foreach() {  
    String[] classIds = { "20000001", "20000002" };  
    List<StudentEntity> list = this.dynamicSqlMapper.getStudentListByClassIds_foreach_array(classIds);  
    for (StudentEntity e : list) {  
        System.out.println(e.toString());  
    }  
}
```
<a name="oHZa1"></a>
### 2、参数为list示例的写法
接口的方法声明：
```java
public List<StudentEntity> getStudentListByClassIds_foreach_list(List<String> classIdList);
```
动态SQL语句：
```xml
<!-- 7.2 foreach(循环List<String>参数) - 作为where中in的条件 -->  
<select id="getStudentListByClassIds_foreach_list" resultMap="resultMap_studentEntity">  
    SELECT ST.STUDENT_ID,  
           ST.STUDENT_NAME,  
           ST.STUDENT_SEX,  
           ST.STUDENT_BIRTHDAY,  
           ST.STUDENT_PHOTO,  
           ST.CLASS_ID,  
           ST.PLACE_ID  
      FROM STUDENT_TBL ST  
      WHERE ST.CLASS_ID IN   
     <foreach collection="list" item="classIdList"  open="(" separator="," close=")">  
        #{classIdList}  
     </foreach>  
</select>
```
测试代码，查询学生中，在20000001、20000002这两个班级的学生：
```java
@Test  
public void test7_2_foreach() {  
    ArrayList<String> classIdList = new ArrayList<String>();  
    classIdList.add("20000001");  
    classIdList.add("20000002");  
    List<StudentEntity> list = this.dynamicSqlMapper.getStudentListByClassIds_foreach_list(classIdList);  
    for (StudentEntity e : list) {  
        System.out.println(e.toString());  
    }  
}
```
sql片段标签`<sql>`：通过该标签可定义能复用的sql语句片段，在执行sql语句标签中直接引用即可。这样既可以提高编码效率，还能有效简化代码，提高可读性<br />需要配置的属性：`id="key"` >>>表示需要改sql语句片段的唯一标识<br />引用：通过`<include refid="" />`标签引用，`refid=""` 中的值指向需要引用的`<sql>`中的`id="key"`属性
```xml
<!--定义sql片段-->  
<sql id="orderAndItem">  
  o.order_id,o.cid,o.address,o.create_date,o.orderitem_id,i.orderitem_id,i.product_id,i.count  
</sql>  

<select id="findOrderAndItemsByOid" parameterType="java.lang.String" resultMap="BaseResultMap">  
  select  
	<!--引用sql片段-->  
  <include refid="orderAndItem" />  
  from ordertable o  
  join orderitem i on o.orderitem_id = i.orderitem_id  
  where o.order_id = #{orderId}  
</select>
```
