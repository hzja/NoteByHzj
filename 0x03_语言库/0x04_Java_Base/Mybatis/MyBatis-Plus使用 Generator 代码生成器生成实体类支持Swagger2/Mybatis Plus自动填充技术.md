JavaMybatis Plus<br />在开发过程中，经常会遇到一些需要在数据库表中自动写入特定字段值的情况，例如创建时间、更新时间等，这时候MybatisPlus的自动填充功能就派上用场了。在本文中，将介绍MybatisPlus自动填充的技术原理以及如何在项目中使用。<br />首先需要了解自动填充是如何工作的。MybatisPlus通过`MetaObject`对象来实现对数据库表的字段进行自动填充。当插入数据时，MybatisPlus会使用`MetaObject`对象检测实体中的特定字段，如果检测到有需要填充的字段，它就会根据特定的逻辑自动填充这些字段。
<a name="w16yy"></a>
## 改改能抄的代码
下面是一个示例代码，演示了如何使用MybatisPlus进行自动填充。在这个示例中，将创建一个Hero实体，包括创建者、创建时间、更新者、更新时间等属性，然后使用MybatisPlus的自动填充功能为这些属性赋值。
<a name="WBO2Y"></a>
### 实体类
```java
@Data
@TableName("Hero")
public class Hero {
    @TableId(type=IdType.AUTO)
    private Long id;

    private String name;

    @TableField(fill = FieldFill.INSERT)
    private Long createBy;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createTime;

    @TableField(fill = FieldFill.UPDATE)
    private Long updateBy;

    @TableField(fill = FieldFill.UPDATE)
    private LocalDateTime updateTime;
}
```
<a name="PHoGX"></a>
### Handler
```java
public class MyMetaObjectHandler implements MetaObjectHandler {

    @Override
    public void insertFill(MetaObject metaObject) {
        this.strictInsertFill(metaObject, "createBy", Long::valueOf, "Fcant");
        this.strictInsertFill(metaObject, "createTime", LocalDateTime::now, LocalDateTime.class);
    }

    @Override
    public void updateFill(MetaObject metaObject) {
        this.strictUpdateFill(metaObject, "updateBy", Long::valueOf, "Fcant");
        this.strictUpdateFill(metaObject, "updateTime", LocalDateTime::now, LocalDateTime.class);
    }
}
```
<a name="vskTp"></a>
### Config
```java
@Configuration
@MapperScan("com.fcant.mapper")
public class MybatisPlusConfig {
    @Bean
    public MybatisPlusInterceptor mybatisPlusInterceptor() {
        MybatisPlusInterceptor interceptor = new MybatisPlusInterceptor();
        // add PageInterceptor
        PaginationInnerInterceptor paginationInnerInterceptor = new PaginationInnerInterceptor();
        paginationInnerInterceptor.setDbType(DbType.MYSQL);
        interceptor.addInnerInterceptor(paginationInnerInterceptor);
        return interceptor;
    }

    @Bean
    public MyMetaObjectHandler myMetaObjectHandler() {
        return new MyMetaObjectHandler();
    }

}
```
在这个示例中，创建了一个User实体，其中包含了创建者、创建时间、更新者、更新时间等属性。使用`@TableField(fill = FieldFill.INSERT)` 和`@TableField(fill = FieldFill.UPDATE)`注解来标记需要自动填充的属性。<br />接下来，创建了一个名为`MyMetaObjectHandler`的对象，并实现了`MetaObjectHandler`接口，该对象将根据自定义逻辑为实体属性自动填充值。在这个示例中，为`createBy`属性的初始值提供了一个默认值“Fcant”，并使用`LocalDateTime::now`方法为createTime和updateTime属性提供当前时间。<br />最后，在配置类中注册了这个`MyMetaObjectHandler`对象。在进行数据库操作时，MybatisPlus会生成一个`MetaObject`对象并将这个对象传递到`MyMetaObjectHandler`中，`MyMetaObjectHandler`将根据实体属性需要自动填充的规则来自动填充这些属性。
<a name="FhC6X"></a>
### `@TableField`用法
| 注解名称 | 解释 |
| --- | --- |
| `@TableField(exist = false)` | 标记这个属性并非数据库表中的字段，但实体类需要这个属性，例如在一些特殊逻辑中需要用到。 |
| `@TableField(value = "column_name")` | 映射实体属性和数据库表中的字段名，value参数即为数据库表中的字段名。 |
| `@TableField(value = "column_name", el = "role.id")` | 映射实体属性和一个 SQL 片段，用法类似于 XML 中的 {} 或 #{}，这里的示例中将在 SQL 语句中使用 role.id 的值作为 column_name 的值。 |
| `@TableField(value = "column_name", condition = SqlCondition.LIKE)` | 指定实体类基本的 SQL 操作中 WHERE 条件的运算符，默认情况下为 =，例如这个示例中是用 LIKE 运算符进行模糊查询。 |
| `@TableField(select = false)` | 当进行查询操作时，标记这个字段不出现在 SQL 语句的 SELECT 字段中，同时 EntityWrapper 和 LambdaWrapper 会忽略这个字段。 |
| `@TableField(fill = FieldFill.INSERT)` | 指定需要自动填充的字段，在 INSERT 操作时自动设置默认值或者填充当前时间。 |
| `@TableField(fill = FieldFill.UPDATE)` | 指定需要自动填充的字段，在 UPDATE 操作时自动设置默认值或者填充当前时间。 |
| `@TableField(fill = FieldFill.INSERT_UPDATE)` | 指定需要自动填充的字段，在 INSERT 或者 UPDATE 操作时自动设置默认值或者填充当前时间。 |

<a name="XDSKZ"></a>
## 参考文献
Mybatis Plus 官方文档 [https://baomidou.com/](https://baomidou.com/)
