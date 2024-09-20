Java Mybatis SQL<br />后端开发，在安全性方面最容易出问题的地方就在于SQL注入。通过 `where 1=1`这种魔幻的SQL写法，就可以很容易的对一个存在问题的系统进行攻击，以至于最终演进出sqlmap这样的神器存在。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1618995784448-c9aec840-df89-46b7-8136-320691865157.webp#clientId=u6c27244c-d468-4&from=paste&id=uc9e61781&originHeight=140&originWidth=140&originalType=url&status=done&style=none&taskId=uc72676e3-0156-48b3-88b0-56cd2d346a6)<br />SQL注入流行的原因，就是开发人员对自己太自信了，或者使用的工具太原始了，没有经过框架层进行一次过滤。如果用了Java界的MyBatis或者JPA，发生SQL注入的可能性就变的非常的低。现在PHP也有了类似于thinkphp一样的框架，代表着能搞的SQL注入漏洞已经越来越少了。<br />但不代表着没有，只是门槛提高了。以MyBatis为例，看一下到底还能不能发生SQL注入。
<a name="fOtih"></a>
## MyBatis依然存在SQL注入
使用Mybatis时第一个接触的概念，就是`#`和`$`的区别。这两个符号非常的像Shell中的魔幻符号，但好在只有两种情况。

- `#`  代表的是使用sql预编译方式，安全可靠
- `$` 代表着使用的是拼接方式，有SQL注入的风险

比如下面这个xml配置，就是一个绝对安全的写法。因为整个`#{id}`会被替换成`?`。
```xml
<select id="queryAll"  resultMap="resultMap">
  SELECT * FROM order WHERE id = #{id}
</select>
```
但可惜的是，有些场景，并不能使用预编译方式（或者仅仅是不知道）。像一些代码重构，把表名/列名/排序等字段，动态传入的时候，不可避免的就需要SQL拼接的方式，SQL注入依然有搞头。<br />但更容易发生问题的，还是`LIKE`和`IN`等类似的语句。<br />下面是两句Like模糊查询的写法，实际测试会发现，使用`#`竟然不好使了，会报错，需要使用sql拼接的`$`。问题由此发生。
```sql
SELECT * FROM order WHERE name like '%#{name}%'  //会报语法错
SELECT * FROM order WHERE name like '%${name}%'  //可以运行
```
而正确的写法，应该使用函数拼接。但是工期压死人，在不知不觉间，大多数人就选择了简单的写法。毕竟功能第一，也是体现工作量的最主要方式。
```sql
SELECT * FROM order WHERE  name like concat(‘%’,#{name}, ‘%’) //正确的写法
```
同样的问题，存在于`IN`语句。
```sql
in (#{tag}) //报错
in (${tag}) //可以运行
```
既然几个字符就可以运行，当然没人选择下面复杂的写法。
```xml
tag in
<foreach collection="tag" item="item" open="("separatosr="," close=")">
	#{tag} 
</foreach>
```
还有order by，也千万不要掉以轻心，一不小心就会万劫不复。
```sql
SELECT * FROM order order by createDate #{sortType} //报错
SELECT * FROM order order by createDate ${sortType} //正常
```
这种情况下，就需要把sortType搞成白名单了。不就一个ASC和DESC了，结果传一个长长的串，是怎么回事?
