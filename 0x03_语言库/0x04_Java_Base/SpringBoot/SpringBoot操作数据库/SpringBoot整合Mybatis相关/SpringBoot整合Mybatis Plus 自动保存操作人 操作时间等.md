Java SpringBoot Mybatis<br />使用Mybatis的时候，Mybatis提倡的是简单，官方也没有类似的功能，这个时候如果业务需要审计数据的时候需要手工维护，这时候可以模仿JPA实现一个类似的功能，如下：
<a name="ofaKZ"></a>
## 使用Mybatis Plus插件
```xml
<dependency>
    <groupId>com.baomidou</groupId>
    <artifactId>mybatis-plus-boot-starter</artifactId>
    <version>${version}</version>
</dependency>
```
<a name="PATky"></a>
## 自定义实现自动填充Handler
```java
public class AuditMetaObjectHandler extends MetaObjectHandler {

    @Override
    public void insertFill(MetaObject metaObject) {
        boolean createDate = metaObject.hasSetter("createDate");
        boolean updateDate = metaObject.hasSetter("updateDate");
        if (createDate || updateDate) {
            Date now = new Date();
            if (createDate) {
                this.setFieldValByName("createDate", now, metaObject);
            }
            if (updateDate) {
                this.setFieldValByName("updateDate", now, metaObject);
            }
        }
        String username = ...;//获取用户，如果使用SpringSecurity的话可以从SecurityContext中获取
        if (metaObject.hasSetter("createUser")) {
            this.setFieldValByName("createUser", username, metaObject);
        }
        if (metaObject.hasSetter("updateUser")) {
            this.setFieldValByName("updateUser", username, metaObject);
        }
    }

    @Override
    public void updateFill(MetaObject metaObject) {
        if (metaObject.hasSetter("updateDate")) {
            this.setFieldValByName("updateDate", new Date(), metaObject);
        }
        if (metaObject.hasSetter("updateUser")) {
            // TODO 获取用户，如果使用SpringSecurity的话可以从SecurityContext中获取
            String username = ...;
            this.setFieldValByName("updateUser", username, metaObject);
        }
    }
}
```
<a name="Vx4uQ"></a>
## 启用配置
在`MybatisPlusConfigurer`配置中新增配置
```java
/**
* 审计数据插件
*
* @return AuditMetaObjectHandler
*/
@Bean
@ConditionalOnMissingBean(name = "auditMetaObjectHandler")
public AuditMetaObjectHandler auditMetaObjectHandler() {
    return new AuditMetaObjectHandler();
}
```
<a name="kw5KF"></a>
## 配置实体类
```java
public class UserDO implement Serializable{

     //TODO 省略

    /**
      * 创建时间
      */
    @TableField(value = "create_date", fill = FieldFill.INSERT)
    private Date createDate;

    /**
      * 创建用户
      */
    @TableField(value = "create_user", fill = FieldFill.INSERT)
    private String createUser;

    /**
     * 更新时间
     */
    @TableField("update_date", fill = FieldFill.INSERT_UPDATE)
    private Date updateDate;

    /**
    * 更新用户
    */
   @TableField("update_user", fill = FieldFill.INSERT_UPDATE)
   private String updateUser;

}
```
<a name="tSmYg"></a>
## 总结
这样就可以实现类似 Spring Data JPA 的审计功能
