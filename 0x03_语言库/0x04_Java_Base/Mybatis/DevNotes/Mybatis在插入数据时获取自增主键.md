Mybatis insert 自增主键
<a name="j3mYe"></a>
## 方式一：通过INSERT标签属性
<a name="RJGIB"></a>
### 1、Mapper映射文件中INSERT语句的写法
在INSERT标签的属性`keyProperty`、`useGenerateKey`、`parameterType`属性赋值

| **属性** | **值** |
| --- | --- |
| `keyProperty` | 将获取的自增主键的值赋予JavaBean属性 |
| `keyColumn` | 指定数据库主键的列名 |
| `useGenerateKey` | True |
| `parameterType` | 参数类型 |

```xml
<insert id="addUser" 
        keyProperty="userId"
        keyColumn="user_id"
        useGeneratedKeys="true" 
        parameterType="com.fcant.bean.UserInfo">
    INSERT INTO user_info(user_id, user_name, age, create_time)
    VALUES (#{userId}, #{userName}, #{age}, #{createTime})
</insert>
```
<a name="ROIW3"></a>
### 2、Mapper接口中写法
```java
/**
 * 添加用户
 *
 * @param userInfo
 * @return int
 * @author Fcant
 * @date 18:40 2019/12/3
 */
int addUser(UserInfo userInfo);
```
<a name="s3Kmh"></a>
### 3、Service层接口的写法
```java
/**
 * 添加用户
 *
 * @param userInfo
 * @return int
 * @author Fcant
 * @date 18:40 2019/12/3
*/
int addUser(UserInfo userInfo);
```
<a name="dzKHq"></a>
### 4、在Service实现类调用Mapper接口后，再获取对象的自增主键的属性值即可得到
```java
/**
 * 添加用户
 *
 * @param userInfo
 * @return int
 * @author Fcant
 * @date 18:40 2019/12/3
 */
@Override
public int addUser(UserInfo userInfo) {
    userMapper.addUser(userInfo);
    System.out.println(userInfo.getUserId());
    return userInfo.getUserId();
}
```
<a name="QMSLX"></a>
## 方式二：通过`<selectKey>`标签
MyBatis在执行插入操作时，可以通过配置`<selectKey>`元素来获取自增值。<br />`<selectKey>`元素可以嵌套在`<insert>`元素内，用于在插入数据之后，查询自增值并将其设置到参数对象中。例如：
```xml
<insert id="insertUser" parameterType="com.example.User">
  insert into user (name, age) values (#{name}, #{age})
  <selectKey keyProperty="id" order="AFTER" resultType="int">
    select last_insert_id()
  </selectKey>
</insert>
```
在上面的示例中，`<selectKey>`元素的`keyProperty`属性指定了自增值要设置到哪个属性中，order属性指定了在插入之后查询自增值的时机，`resultType`属性指定了自增值的类型。<br />在执行插入操作时，MyBatis会执行以下步骤：

1. 执行插入操作，将数据插入到数据库中。
2. 根据`<selectKey>`元素的配置，查询自增值。
3. 将查询到的自增值设置到参数对象中。
4. 返回执行结果。

需要注意的是，不同的数据库在获取自增值的方式上可能会有差异。例如，MySQL可以使用`select last_insert_id()`语句获取最后插入的自增值，而Oracle可以使用`returning`关键字返回自增值。因此，在配置`<selectKey>`元素时，需要根据具体的数据库类型和自增值的获取方式进行相应的配置。
