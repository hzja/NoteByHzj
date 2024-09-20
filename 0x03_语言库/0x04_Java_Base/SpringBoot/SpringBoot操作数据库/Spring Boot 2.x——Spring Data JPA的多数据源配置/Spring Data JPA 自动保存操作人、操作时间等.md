Java SpringBoot JPA<br />有些业务数据需要对数据的创建人、创建时间、最后更新人和最后更新时间进行记录。如果使用Spring Data Jpa做数据新增或更新，可实现自动保存这些信息而不需要显式设置对应字段的值。实现自动记录上述信息主要有5个注解：

- `@EnableJpaAuditing`：审计功能开关
- `@CreatedBy`：标记数据创建者属性
- `@LastModifiedBy`：标记数据最近一次修改者属性
- `@CreatedDate`：标记数据创建日期属性
- `@LastModifiedDate`：标记数据最近一次修改日期属性
<a name="XmZrT"></a>
## 依赖引用
使用Spring Data JPA要引用依赖spring-boot-starter-data-jpa
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
```
<a name="vSKVK"></a>
## 实体类标记审计属性
案例使用User实体演示过程，需要在实体对应的字段上添加对应的注解表示是审计属性，另外需要在实体类上开启审计监听，如下：
```java
@Entity
@Table(name = "t_user")
@EntityListeners({AuditingEntityListener.class})//开启审计监听
public class UserDO implement Serializable {
        @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    //保存创建人的字段
    @CreatedBy
    @Column(name = "created_by")
    private String createdBy;

    //保存最近修改人的字段
    @LastModifiedBy
    @Column(name = "last_modified_by")
    private String lastModifiedBy;

    //保存创建时间的字段
    @CreatedDate
    @Column(name = "created_date")

    //保存最近修改日期的字段
    private Date createdDate;
    @LastModifiedDate
    @Column(name = "last_modified_date")
    private Date lastModifiedDate;

    @Column(name = "real_name")
    private String realName;

    @Column(name = "username")    
    private String username;

    @Column(name = "password")
    private String password;

    //TODO 省略 getter setter
}
```
User实体对应数据表定义如下：
```sql
create table t_user (
    id int auto_increment primary key,
    username varchar(30) default '' not null comment '登录用户名',
    password varchar(100) default '' null comment '加密密码',
    real_name varchar(30) default '' null comment '真实姓名',
    created_by varchar(50) default 'HSystem' null comment '创建人',
    created_date datetime default CURRENT_TIMESTAMP not null,
    last_modified_by varchar(30) default 'HSystem' null comment '修改人',
    last_modified_date datetime default CURRENT_TIMESTAMP not null,
    constraint user_username_uindex unique (username)
);
```
<a name="PmJVL"></a>
## 审计自定义操作
当对实体有新增或保存操作时，系统会自动获取操作时的系统时间作为创建时间和修改时间。对于创建人或最后修改人，审计过程会获取当前登录系统的用户信息，当未登录的情况下，需要指定默认操作，可通过实现`AuditorAware`类来实现。下面代码在未获取到用户信息时返回`HSystem`表示默认为系统操作。
```java
@Configuration
public class SpringSecurityAuditorAware implements AuditorAware<String> {

    final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Override
    public Optional<String> getCurrentAuditor() {
        try {
            //TODO 这里根据具体情况获取用户信息
            Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
            if (authentication instanceof AnonymousAuthenticationToken) {
                return Optional.of("HSystem");
            } else {
                if (authentication == null) {
                    return Optional.of("HSystem");
                }
                User user = (User) authentication.getPrincipal();
                return Optional.of(user.getUsername());
            }
        } catch (Exception ex) {
            logger.error("get user Authentication failed: " + ex.getMessage(), ex);
            return Optional.of("HSystem");
        }
    }
}
```
<a name="bSUTS"></a>
## 应用开启审计功能
在应用程序入口类添加注解`@EnableJpaAuditing`开启审计功能，如下
```java
@SpringBootApplication
//启用JPA审计功能，自动填充@CreateDate、@CreatedBy、@LastModifiedDate、@LastModifiedBy注解的字段
@EnableJpaAuditing(auditorAwareRef = "springSecurityAuditorAware")
public class JapAuditApplication {

    public static void main(String[] args) {
        SpringApplication.run(HBackendApplication.class, args);
    }
}
```
注意：如果系统中有多个审计实现，需要指定Bean的名称，上面案例中使用名称为`springSecurityAuditorAware`的bean。
<a name="ShHjB"></a>
## 实体操作
定义User实体类的JPA操作接口UserRepository如下
```java
@Repository
public interface UserRepository extends PagingAndSortingRepository<User, Integer>, JpaRepository<User, Integer> {

}
```
经过以上步骤再使用UserRepository保存User信息时，就会自动更新创建人，创建时间，更新人和更新时间者四个字段。
