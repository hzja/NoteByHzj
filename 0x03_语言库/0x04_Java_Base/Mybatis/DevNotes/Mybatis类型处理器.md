Java Mybatis
<a name="Z3n8t"></a>
## 1、明确需求
在设计之初，`sys_role`表的`enabled`字段有2个可选值，其中0 代表禁用，1代表启用，而且实体类中使用的是Interger类型：
```java
/**
 * 有效标志
 */
private Integer enabled;

public Integer getEnabled() {
    return enabled;
}

public void setEnabled(Integer enabled) {
    this.enabled = enabled;
}
```
如果要新增或者更新角色信息，肯定要校验enabled字段的值必须是0或者1，所以最初的部分代码可能是这样的：
```java
if (sysRole.getEnabled() == 0 || sysRole.getEnabled() == 1) {
     sysRoleMapper.updateById(sysRole);

     sysRole = sysRoleMapper.selectById(2L);
     Assert.assertEquals(0, sysRole.getEnabled());
} else {
     thrownew Exception("无效的enabled值");
}
```
这种硬编码的方式不仅看起来不友好，而且不利于后期维护。所以需求就是，拒绝硬编码，使用友好的编码方式来校验enabled字段的值是否有效。
<a name="NsKG8"></a>
## 2、使用MyBatis提供的枚举类型处理器
通常会使用枚举来解决这种场景。<br />首先新建com.fcant.mybatisaction.type包，然后在该包下新建枚举`Enabled`：
```java
package com.fcant.mybatisaction.type;

publicenum Enabled {
    /**
     * 禁用
     */
    disabled,
    
    /**
     * 启用
     */
    enabled;
}
```
其中，`disabled`对应的索引为0，`enabled`对应的索引为1。<br />然后将`SysRole`类中原来为`Integer`类型的`enabled`字段修改为：
```java
/**
 * 有效标志
 */
private Enabled enabled;

public Enabled getEnabled() {
    return enabled;
}

public void setEnabled(Enabled enabled) {
    this.enabled = enabled;
}
```
此时原本硬编码的代码就可以修改为：
```java
if (sysRole.getEnabled() == Enabled.disabled || sysRole.getEnabled() == Enabled.enabled) {
    sysRoleMapper.updateById(sysRole);

    sysRole = sysRoleMapper.selectById(2L);
    Assert.assertEquals(Enabled.disabled, sysRole.getEnabled());
} else {
    thrownew Exception("无效的enabled值");
}
```
虽然上面的代码很完美的解决了硬编码的问题，但此时又引出一个新的问题：<br />数据库并不能识别`Enabled`枚举类型，在新增，更新或者作为查询条件时，需要将枚举值转换为数据库中的int类型，在查询数据时，需要将数据库的int类型的值转换为`Enabled`枚举类型。<br />带着这个问题，在`SysRoleMapperTest`测试类中添加如下测试方法：
```java
@Test
public void testUpdateById() {
    SqlSession sqlSession = getSqlSession();

    try {
        SysRoleMapper sysRoleMapper = sqlSession.getMapper(SysRoleMapper.class);

        // 先查询出id=2的角色，然后修改角色的enabled值为disabled
        SysRole sysRole = sysRoleMapper.selectById(2L);
        Assert.assertEquals(Enabled.enabled, sysRole.getEnabled());

        // 修改角色的enabled为disabled
        sysRole.setEnabled(Enabled.disabled);

        if (sysRole.getEnabled() == Enabled.disabled || sysRole.getEnabled() == Enabled.enabled) {
            sysRoleMapper.updateById(sysRole);

            sysRole = sysRoleMapper.selectById(2L);
            Assert.assertEquals(Enabled.disabled, sysRole.getEnabled());
        } else {
            thrownew Exception("无效的enabled值");
        }
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        sqlSession.close();
    }
}
```
运行测试代码，发现抛出如下异常：
```java
Error querying database. Cause: org.apache.ibatis.executor.result.ResultMapException: Error attempting to get column 'enabled' from result set. Cause: java.lang.IllegalArgumentException: No enum constant com.fcant.mybatisaction.type.Enabled.1
```
这是因为MyBatis在处理Java类型和数据库类型时，使用`TypeHandler`（类型处理器）对这两者进行转换。<br />MyBatis为Java类型和数据库JDBC中的常用类型类型提供了`TypeHandler`接口的实现。<br />MyBatis在启动时会加载所有的JDBC对应的类型处理器，在处理枚举类型时默认使用`org.apache.ibatis.type.EnumTypeHandler`处理器，这个处理器会将枚举类型转换为字符串类型的字面值使用，对于`Enabled`枚举来说，就是“disabled"和"enabled"字符串。<br />而数据库中`enabled`字段的类型是int，所以在查询到角色信息将int类型的值1转换为`Enabled`类型报错。<br />那么如何解决这个问题呢？<br />MyBatis还提供了另一个枚举处理器：`org.apache.ibatis.type.EnumOrdinalTypeHandler`，这个处理器使用枚举的索引进行处理，可以解决此处转换报错的问题。<br />使用这个处理器，需要在之前的resources/mybatis-config.xml中添加如下配置：
```xml
<typeHandlers>
    <typeHandler handler="org.apache.ibatis.type.EnumOrdinalTypeHandler"
                 javaType="com.fcant.mybatisaction.type.Enabled"/>
</typeHandlers>
```
再次运行测试代码，测试通过，输出日志如下：
```java
DEBUG [main] - ==> Preparing: SELECT id,role_name,enabled,create_by,create_time FROM sys_role WHERE id = ?
DEBUG [main] - ==> Parameters: 2(Long)
TRACE [main] - <== Columns: id, role_name, enabled, create_by, create_time
TRACE [main] - <== Row: 2, 普通用户, 1, 1, 2019-06-27 18:21:12.0
DEBUG [main] - <== Total: 1
DEBUG [main] - ==> Preparing: UPDATE sys_role SET role_name = ?,enabled = ?,create_by=?, create_time=? WHERE id=?
DEBUG [main] - ==> Parameters: 普通用户(String), 0(Integer), 1(Long), 2019-06-27 18:21:12.0(Timestamp), 2(Long)
DEBUG [main] - <== Updates: 1
```
从日志中可以看出，在查询角色信息时，`MyBatis`将1转换为了`Enabled.enabled`，在更新角色信息时，MyBatis将`Enabled.disabled`转换为了0。
<a name="eUFx9"></a>
## 3、使用自定义的类型处理器
假设`enabled`字段的值既不是枚举的字面值，也不是枚举的索引值，此时`org.apache.ibatis.type.EnumTypeHandler`和`org.apache.ibatis.type.EnumOrdinalTypeHandler`都不能满足需求，这种情况下就需要自己来实现类型处理器了。<br />首先修改下枚举类`Enabled`代码：
```java
package com.fcant.mybatisaction.type;

publicenum Enabled {

    /**
     * 启用
     */
    enabled(1),

    /**
     * 禁用
     */
    disabled(0);

    privatefinalint value;

    private Enabled(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
```
然后在`com.fcant.mybatisaction.type`包下新建类型处理器`EnabledTypeHandler`：
```java
package com.fcant.mybatisaction.type;

import org.apache.ibatis.type.JdbcType;
import org.apache.ibatis.type.TypeHandler;

import java.sql.CallableStatement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

/**
 * Enabled类型处理器
 */
publicclass EnabledTypeHandler implements TypeHandler<Enabled> {
    privatefinal Map<Integer, Enabled> enabledMap = new HashMap<Integer, Enabled>();

    public EnabledTypeHandler() {
        for (Enabled enabled : Enabled.values()) {
            enabledMap.put(enabled.getValue(), enabled);
        }
    }

    @Override
    public void setParameter(PreparedStatement preparedStatement, int i, Enabled enabled, JdbcType jdbcType) throws SQLException {
        preparedStatement.setInt(i, enabled.getValue());
    }

    @Override
    public Enabled getResult(ResultSet resultSet, String s) throws SQLException {
        Integer value = resultSet.getInt(s);
        return enabledMap.get(value);
    }

    @Override
    public Enabled getResult(ResultSet resultSet, int i) throws SQLException {
        Integer value = resultSet.getInt(i);
        return enabledMap.get(value);
    }

    @Override
    public Enabled getResult(CallableStatement callableStatement, int i) throws SQLException {
        Integer value = callableStatement.getInt(i);
        return enabledMap.get(value);
    }
}
```
自定义类型处理器实现了`TypeHandler`接口，重写了接口中的4个方法，并且在无参构造函数中遍历了枚举类型`Enabled`并对字段`enabledMap`进行了赋值。<br />想要使用自定义的类型处理器，也需要在`resources/mybatis-config.xml`中添加如下配置：
```xml
<typeHandlers>
    <!--其他配置-->
    <typeHandler handler="com.fcant.mybatisaction.type.EnabledTypeHandler"
                 javaType="com.fcant.mybatisaction.type.Enabled"/>
</typeHandlers>
```
运行测试代码，输出日志和上面的输出日志一样。
