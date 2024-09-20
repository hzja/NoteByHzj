Mybatis OGNL<br />在Structs流行的时代OGNL可是必会的数据渲染技术。它全称Object Graph Navigation Language，作用是降低对数据层访问的难度，它拥有类型转换、访问对象方法、操作集合对象等功能。目前已经很少通过OGNL来访问数据层了，目前国内大部分的ORM框架是Mybatis，而Mybatis中的动态SQL技术运用了OGNL。
<a name="QgojZ"></a>
## Mybatis中的OGNL
不少人在Mybatis的Mapper文件中写过这样的判断：
```xml
<if test="field!='' and field!= null">
  and some_col = #{field}
</if>
```
当field不为空字符并且不为null的情况下增加一个查询条件。其中 test就是一个OGNL表达式。Mybatis中的OGNL表达式主要有两种用途。
<a name="XOgZx"></a>
### 条件断言
这种是最常用的。执行动态SQL的条件断言，常用的有这些表达式：

- `b1 or b2`  条件 或
- `b1 and b2`  条件 与
- `!b1` 取反，也可以写作not b1
- `b1 == b2,b1 eq b2`  判断两个值相等
- `b1 != b2,b1 neq b2` 判断两个值不想等
- `b1 lt b2`  判断b1小于（less than）b2
- `b1 gt b2`  判断b1小于（greater than）b2
- `b1 lte b2`：判断b1小于等于b2
- `b1 gte b2`：判断b1大于等于b2
- `b1 in b2` 判断b2包含b1
- `b1 not in b2`  判断b2不包含b1

这些表达式经常和test配合。
<a name="iyPkb"></a>
### 四则运算赋值
还有一些表达式用来赋值或者增强属性。经常用来做模糊搜索的 `bind`标签：
```xml
<bind name="nameLike" value="'%'+ name + '%'"/>
```
这里的`value`也属于OGNL表达式 `e1+e2`，字符串是拼接，数字的话就是加法运算，可以引申出肯定还有：

- `e1*e2` 乘法
- `e1/e2` 除法
- `e1-e2` 减法
- `e1%e2` 取模
<a name="pNBVv"></a>
### 类的内置方法
其实Mybatis的Mapper.xml中还可以使用对象的内置方法，比如需要判断一个`java.util.Collection`集合是否为空，可以这么写：
```xml
<if test="collection!=null and collection.size()> 0">
  and some_col = #{some_val}
</if>
```
这里就使用了对象的内置方法`Collection.size()`。<br />还可以调用自定义对象`CollectionUtils`的静态方法来判断集合是否为空：
```java
public final class CollectionUtils {
    public static boolean isNotEmpty( Collection<?> collection) {
        return (collection != null && !collection.isEmpty());
    }    
}
```
那么上面的`<if>`判断改为:
```xml
<if test="@cn.fcant.util.CollectionUtils@isNotEmpty(collection)">
  and some_col = #{some_val}
</if>
```
这里要带上类的全限定名。
<a name="GNLW9"></a>
### 取值操作
取值操作的话，如果是对象直接`e.property`，如果是集合或者`Map`可以`e[index|key]`，通过索引或者键名来取值。分别举个例子：
```xml
# 对象取属性
user.username
# 集合取元素
array[1] 
# map 取值
map['username']
```
其实静态属性也能取值调用，跟上面的静态方法类似：
```java
@cn.fcant.Cache@user
```
对应Java代码：
```java
public final class Cache {
    public static User user = new User ("Fcant") ;
}
```
<a name="wkUuH"></a>
### 赋值操作
上面的取值除了可以做判断还可以用来SQL参数赋值：
```xml
<where>
  <!-- 常用的赋值方式 -->
  username = #{username}
  <!-- $ 也可以赋值 -->
  and user_id =${userId}
  <!-- 对象取属性 -->
  and id = ${user.id}
  <!-- Math.abs  双@简写 -->
  and age = ${@@abs(-12345678)}
  <!-- 调用枚举 -->
  and gender =${@cn.fcant.GenderEnum@MALE.ordinal()}
  and id=${@cn.fcant.Cache@user.userId}
</where>

```
通过`${}`符号可以用OGNL表达式给SQL参数赋值。
