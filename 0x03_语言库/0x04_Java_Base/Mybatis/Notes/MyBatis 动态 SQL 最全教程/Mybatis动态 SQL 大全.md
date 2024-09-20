Java Mybatis
<a name="hhUHO"></a>
### 1、If 语句
需求：根据作者名字和博客名字来查询博客！如果作者名字为空，那么只根据博客名字查询，反之，则根据作者名来查询
```xml
<!--需求1：
根据作者名字和博客名字来查询博客！
如果作者名字为空，那么只根据博客名字查询，反之，则根据作者名来查询
select * from blog where title = #{title} and author = #{author}
-->
<select id="queryBlogIf" parameterType="map" resultType="blog">
 select * from blog where
 <if test="title != null">
  title = #{title}
 </if>
 <if test="author != null">
  and author = #{author}
 </if>
</select>
```
这样写可以看到，如果 author 等于 null，那么查询语句为 `select * from user where title=#{title}`，但是如果title为空呢？那么查询语句为 `select * from user where and author=#{author}`，这是错误的SQL 语句，如何解决呢？请看下面的 where 语句！
<a name="e5mTr"></a>
### 2、Where语句
修改上面的SQL语句：
```xml
<select id="queryBlogIf" parameterType="map" resultType="blog">
 select * from blog
 <where>
  <if test="title != null">
   title = #{title}
  </if>
  <if test="author != null">
   and author = #{author}
  </if>
 </where>
</select>
```
where 元素只会在子元素返回任何内容的情况下才插入 “WHERE” 子句。而且，若子句的开头为 “AND” 或 “OR”，where 元素也会将它们去除。<br />如果 where 元素与期望的不太一样，也可以通过自定义 trim 元素来定制 where 元素的功能。
<a name="K485T"></a>
##### 2.1 和 where 元素等价的自定义 trim 元素
```xml
<trim prefix="WHERE" prefixOverrides="AND |OR ">
  ...
</trim>
```
<a name="Hm0uz"></a>
### 3、Set语句
同理，上面的对于查询 SQL 语句包含 where 关键字，如果在进行更新操作的时候，含有 set 关键词，怎么处理呢？
```xml
<!--注意set是用的逗号隔开-->
<update id="updateBlog" parameterType="map">
 update blog
 <set>
  <if test="title != null">
   title = #{title},
  </if>
  <if test="author != null">
   author = #{author}
  </if>
 </set>
 where id = #{id};
</update>
```
这个例子中，set 元素会动态地在行首插入 SET 关键字，并会删掉额外的逗号（这些逗号是在使用条件语句给列赋值时引入的）
<a name="gSAts"></a>
##### 3.1 与 set 元素等价的自定义 trim 元素
```xml
<trim prefix="SET" suffixOverrides=",">
  ...
</trim>
```
<a name="j6rQT"></a>
### 4、Choose语句
有时候，不想用到所有的查询条件，只想选择其中的一个，查询条件有一个满足即可，使用 choose 标签可以解决此类问题，类似于 Java 的 switch 语句
```xml
<select id="queryBlogChoose" parameterType="map" resultType="blog">
 select * from blog
 <where>
  <choose>
   <when test="title != null">
    title = #{title}
   </when>
   <when test="author != null">
    and author = #{author}
   </when>
   <otherwise>
    and views = #{views}
   </otherwise>
  </choose>
 </where>
</select>
```
<a name="IY6ho"></a>
### 5、Foreach语句
将数据库中前三个数据的id修改为1,2,3；<br />需求：需要查询 blog 表中 id 分别为1,2,3的博客信息
```xml
<select id="queryBlogForeach" parameterType="map" resultType="blog">
 select * from blog
 <where>
  <!--
  collection:指定输入对象中的集合属性
  item:每次遍历生成的对象
  open:开始遍历时的拼接字符串
  close:结束时拼接的字符串
  separator:遍历对象之间需要拼接的字符串
  select * from blog where 1=1 and (id=1 or id=2 or id=3)
  -->
  <foreach collection="ids" item="id" open="and (" close=")"
  separator="or">
   id=#{id}
  </foreach>
 </where>
</select>
```
<a name="fnePM"></a>
### 6、SQL片段
有时候可能某个 sql 语句用的特别多，为了增加代码的重用性，简化代码，需要将这些代码抽取出来，然后使用时直接调用。<br />提取SQL片段：
```xml
<sql id="if-title-author">
 <if test="title != null">
  title = #{title}
 </if>
 <if test="author != null">
  and author = #{author}
 </if>
</sql>
```
引用SQL片段：
```xml
<select id="queryBlogIf" parameterType="map" resultType="blog">
 select * from blog
 <where>
  <!-- 引用 sql 片段，如果refid 指定的不在本文件中，那么需要在前面加上 namespace-->
  <include refid="if-title-author"></include>
  <!-- 在这里还可以引用其他的 sql 片段 -->
 </where>
</select>
```
注意：

- 最好基于 单表来定义 sql 片段，提高片段的可重用性
- 在 sql 片段中不要包括 where
<a name="V2Yc6"></a>
### 7、Bind元素
bind 元素允许在 OGNL 表达式以外创建一个变量，并将其绑定到当前的上下文。比如：
```xml
<select id="selectBlogsLike" resultType="Blog">
	<bind name="pattern" value="'%' + _parameter.getTitle() + '%'" />
	SELECT * FROM BLOG
	WHERE title LIKE #{pattern}
</select>
```
