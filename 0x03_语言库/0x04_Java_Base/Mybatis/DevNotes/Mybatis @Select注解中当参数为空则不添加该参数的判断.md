JavaMybatis
```java
public interface OrderMapper extends SqlMapper{
	
	@Select("select * from tbl_order where room like #{room} and mydate like #{mydate}")
	public List<Order> getbyroom(OrderPara op);
	
}
```
这样整个语句是写死的，必须有2个参数，在这种模式下，如何能实现根据room和mydate是否为空来动态的拼写sql语句<br />比如当`mydate=""`
```java
Select("select * from tbl_order where room like #{room} ")
public List<Order> getbyroom(OrderPara op);
```
如果用xml来配置语句的话，可以用
```xml
<when test="title != null">
	and mydate= #{mydate}
</when>
```
如果是用`@Select`<br />这种 改如何做呢？<br />**解决方案：**<br />用script标签包围，然后像xml语法一样书写
```java
@Select({"<script>",
    "SELECT * FROM tbl_order",
    "WHERE 1=1",
    "<when test='title!=null'>",
    "AND mydate = #{mydate}",
    "</when>",
    "</script>"})
```
