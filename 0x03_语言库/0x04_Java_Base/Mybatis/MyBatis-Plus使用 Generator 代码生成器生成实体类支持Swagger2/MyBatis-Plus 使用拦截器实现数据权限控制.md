JavaMyBatis-Plus
<a name="A0MOW"></a>
## **前言背景**
平时开发中遇到根据当前用户的角色，只能查看数据权限范围的数据需求。列表实现方案有两种，一是在开发初期就做好判断赛选，但如果这个需求是中途加的，或不希望每个接口都加一遍，就可以方案二加拦截器的方式。在mybatis执行sql前修改语句，限定where范围。<br />当然拦截器生效后是全局性的，如何保证只对需要的接口进行拦截和转化，就可以应用注解进行识别<br />因此具体需要哪些步骤就明确了

- 创建注解类
- 创建拦截器实现`InnerInterceptor`接口，重写查询方法
- 创建处理类，获取数据权限 SQL 片段，设置where
- 将拦截器加到MyBatis-Plus插件中
<a name="yftGf"></a>
## **基础版**
<a name="nyUnz"></a>
### 自定义注解
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Target({ElementType.METHOD, ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface UserDataPermission {
}
```
<a name="uyslN"></a>
### 拦截器
```java
import com.baomidou.mybatisplus.core.plugins.InterceptorIgnoreHelper;
import com.baomidou.mybatisplus.core.toolkit.PluginUtils;
import com.baomidou.mybatisplus.extension.parser.JsqlParserSupport;
import com.baomidou.mybatisplus.extension.plugins.inner.InnerInterceptor;
import lombok.*;
import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.statement.select.PlainSelect;
import net.sf.jsqlparser.statement.select.Select;
import net.sf.jsqlparser.statement.select.SelectBody;
import net.sf.jsqlparser.statement.select.SetOperationList;
import org.apache.ibatis.executor.Executor;
import org.apache.ibatis.mapping.BoundSql;
import org.apache.ibatis.mapping.MappedStatement;
import org.apache.ibatis.session.ResultHandler;
import org.apache.ibatis.session.RowBounds;

import java.sql.SQLException;
import java.util.List;

@Data
@NoArgsConstructor
@AllArgsConstructor
@ToString(callSuper = true)
@EqualsAndHashCode(callSuper = true)
public class MyDataPermissionInterceptor extends JsqlParserSupport implements InnerInterceptor {

    /**
     * 数据权限处理器
     */
    private MyDataPermissionHandler dataPermissionHandler;

    @Override
    public void beforeQuery(Executor executor, MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException {
        if (InterceptorIgnoreHelper.willIgnoreDataPermission(ms.getId())) {
            return;
        }
        PluginUtils.MPBoundSql mpBs = PluginUtils.mpBoundSql(boundSql);
        mpBs.sql(this.parserSingle(mpBs.sql(), ms.getId()));
    }

    @Override
    protected void processSelect(Select select, int index, String sql, Object obj) {
        SelectBody selectBody = select.getSelectBody();
        if (selectBody instanceof PlainSelect) {
            this.setWhere((PlainSelect) selectBody, (String) obj);
        } else if (selectBody instanceof SetOperationList) {
            SetOperationList setOperationList = (SetOperationList) selectBody;
            List<SelectBody> selectBodyList = setOperationList.getSelects();
            selectBodyList.forEach(s -> this.setWhere((PlainSelect) s, (String) obj));
        }
    }

    /**
     * 设置 where 条件
     *
     * @param plainSelect  查询对象
     * @param whereSegment 查询条件片段
     */
    private void setWhere(PlainSelect plainSelect, String whereSegment) {

        Expression sqlSegment = this.dataPermissionHandler.getSqlSegment(plainSelect, whereSegment);
        if (null != sqlSegment) {
            plainSelect.setWhere(sqlSegment);
        }
    }
}
```
<a name="frg7R"></a>
### 拦截器处理器
基础只涉及 = 表达式，要查询集合范围 in 看进阶版用例
```java
import cn.hutool.core.collection.CollectionUtil;
import lombok.SneakyThrows;
import lombok.extern.slf4j.Slf4j;
import net.sf.jsqlparser.expression.Alias;
import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.expression.HexValue;
import net.sf.jsqlparser.expression.StringValue;
import net.sf.jsqlparser.expression.operators.conditional.AndExpression;
import net.sf.jsqlparser.expression.operators.relational.EqualsTo;
import net.sf.jsqlparser.expression.operators.relational.ExpressionList;
import net.sf.jsqlparser.expression.operators.relational.InExpression;
import net.sf.jsqlparser.expression.operators.relational.ItemsList;
import net.sf.jsqlparser.schema.Column;
import net.sf.jsqlparser.schema.Table;
import net.sf.jsqlparser.statement.select.PlainSelect;

import java.lang.reflect.Method;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

@Slf4j
public class MyDataPermissionHandler {

    /**
     * 获取数据权限 SQL 片段
     *
     * @param plainSelect  查询对象
     * @param whereSegment 查询条件片段
     * @return JSqlParser 条件表达式
     */
    @SneakyThrows(Exception.class)
    public Expression getSqlSegment(PlainSelect plainSelect, String whereSegment) {
        // 待执行 SQL Where 条件表达式
        Expression where = plainSelect.getWhere();
        if (where == null) {
            where = new HexValue(" 1 = 1 ");
        }
        log.info("开始进行权限过滤,where: {},mappedStatementId: {}", where, whereSegment);
        //获取mapper名称
        String className = whereSegment.substring(0, whereSegment.lastIndexOf("."));
        //获取方法名
        String methodName = whereSegment.substring(whereSegment.lastIndexOf(".") + 1);
        Table fromItem = (Table) plainSelect.getFromItem();
        // 有别名用别名，无别名用表名，防止字段冲突报错
        Alias fromItemAlias = fromItem.getAlias();
        String mainTableName = fromItemAlias == null ? fromItem.getName() : fromItemAlias.getName();
        //获取当前mapper 的方法
        Method[] methods = Class.forName(className).getMethods();
        //遍历判断mapper 的所以方法，判断方法上是否有 UserDataPermission
        for (Method m : methods) {
            if (Objects.equals(m.getName(), methodName)) {
                UserDataPermission annotation = m.getAnnotation(UserDataPermission.class);
                if (annotation == null) {
                    return where;
                }
                // 1、当前用户Code
                User user = SecurityUtils.getUser();
                // 查看自己的数据
                //  = 表达式
                EqualsTo usesEqualsTo = new EqualsTo();
                usesEqualsTo.setLeftExpression(new Column(mainTableName + ".creator_code"));
                usesEqualsTo.setRightExpression(new StringValue(user.getUserCode()));
                return new AndExpression(where, usesEqualsTo);
            }
        }
        //说明无权查看，
        where = new HexValue(" 1 = 2 ");
        return where;
    }

}
```
<a name="Rku7Q"></a>
### 将拦截器加到MyBatis-Plus插件中
如果之前项目配插件 ，直接用下面方式就行
```java
@Bean
public MybatisPlusInterceptor mybatisPlusInterceptor() {
    MybatisPlusInterceptor interceptor = new MybatisPlusInterceptor();
    // 添加数据权限插件
    MyDataPermissionInterceptor dataPermissionInterceptor = new MyDataPermissionInterceptor();
    // 添加自定义的数据权限处理器
    dataPermissionInterceptor.setDataPermissionHandler(new MyDataPermissionHandler());
    interceptor.addInnerInterceptor(dataPermissionInterceptor);
    interceptor.addInnerInterceptor(new PaginationInnerInterceptor(DbType.MYSQL));
    return interceptor;
}
```
但如果项目之前是依赖包依赖，或有公司内部统一拦截设置好，也可以往`MybatisPlusInterceptor`进行插入，避免影响原有项目配置
```java
@Bean
public MyDataPermissionInterceptor myInterceptor(MybatisPlusInterceptor mybatisPlusInterceptor) {
    MyDataPermissionInterceptor sql = new MyDataPermissionInterceptor();
    sql.setDataPermissionHandler(new MyDataPermissionHandler());
    List<InnerInterceptor> list = new ArrayList<>();
    // 添加数据权限插件
    list.add(sql);
    // 分页插件
    mybatisPlusInterceptor.setInterceptors(list);
    list.add(new PaginationInnerInterceptor(DbType.MYSQL));
    return sql;
}
```
以上就是简单版的是拦截器修改语句使用
<a name="t9R3H"></a>
### 使用方式
在mapper层添加注解即可
```java
@UserDataPermission
List<CustomerAllVO> selectAllCustomerPage(IPage<CustomerAllVO> page, @Param("customerName")String customerName);
```
<a name="kjA8m"></a>
## **进阶版**
基础版只是能用，业务功能没有特别约束，先保证能跑起来<br />进阶版 解决两个问题：

- 加了角色，用角色决定范围
- 解决不是mapper层自定义sql查询问题。

两个是完全独立的问题 ，可根据情况分开解决
<a name="wTQV6"></a>
### 解决不是mapper层自定义sql查询问题。
例如名称简单的sql语句，直接在Service层用mybatisPluse自带的方法
```java
xxxxService.list(Wrapper<T> queryWrapper)
xxxxService.page(new Page<>(),Wrapper<T> queryWrapper)
```
以上这种应该把注解加哪里呢<br />因为service层，本质上还是调mapper层， 所以还是在mapper层做文章，原来的mapper实现了`extends BaseMapper` 接口，所以能够查询，要做的就是在 mapper层中间套一个中间接口，来方便加注解<br />`xxxxxMapper` ——》`DataPermissionMapper`（中间） ——》`BaseMapper`<br />根据自身需要，在重写的接口方法上加注解即可，这样就影响原先的代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1690765410534-59e21ec4-e13a-4e8f-a544-28ccbefa1e03.png#averageHue=%232f2e2c&clientId=u5012a95d-526e-4&from=paste&id=u6f913621&originHeight=123&originWidth=840&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucb48ebf9-7241-4b2a-9ae0-8fa01cef16d&title=)
```java
import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.Constants;
import org.apache.ibatis.annotations.Param;

import java.io.Serializable;
import java.util.Collection;
import java.util.List;
import java.util.Map;

public interface DataPermissionMapper<T> extends BaseMapper<T> {

    /**
     * 根据 ID 查询
     *
     * @param id 主键ID
     */
    @Override
    @UserDataPermission
    T selectById(Serializable id);

    /**
     * 查询（根据ID 批量查询）
     *
     * @param idList 主键ID列表(不能为 null 以及 empty)
     */
    @Override
    @UserDataPermission
    List<T> selectBatchIds(@Param(Constants.COLLECTION) Collection<? extends Serializable> idList);

    /**
     * 查询（根据 columnMap 条件）
     *
     * @param columnMap 表字段 map 对象
     */
    @Override
    @UserDataPermission
    List<T> selectByMap(@Param(Constants.COLUMN_MAP) Map<String, Object> columnMap);

    /**
     * 根据 entity 条件，查询一条记录
     *
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    T selectOne(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 Wrapper 条件，查询总记录数
     *
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    Integer selectCount(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 entity 条件，查询全部记录
     *
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    List<T> selectList(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 Wrapper 条件，查询全部记录
     *
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    List<Map<String, Object>> selectMaps(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 Wrapper 条件，查询全部记录
     * <p>注意： 只返回第一个字段的值</p>
     *
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    List<Object> selectObjs(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 entity 条件，查询全部记录（并翻页）
     *
     * @param page         分页查询条件（可以为 RowBounds.DEFAULT）
     * @param queryWrapper 实体对象封装操作类（可以为 null）
     */
    @Override
    @UserDataPermission
    <E extends IPage<T>> E selectPage(E page, @Param(Constants.WRAPPER) Wrapper<T> queryWrapper);

    /**
     * 根据 Wrapper 条件，查询全部记录（并翻页）
     *
     * @param page         分页查询条件
     * @param queryWrapper 实体对象封装操作类
     */
    @Override
    @UserDataPermission
    <E extends IPage<Map<String, Object>>> E selectMapsPage(E page, @Param(Constants.WRAPPER) Wrapper<T> queryWrapper);
}
```
<a name="RnyPk"></a>
### 解决角色控制查询范围
引入角色，先假设有三种角色，按照常规的业务需求，一种是管理员查看全部、一种是部门管理查看本部门、一种是仅查看自己。<br />有了以上假设，就可以设置枚举类编写业务逻辑， 对是业务逻辑，所以只需要更改”拦截器处理器类“

- 建立范围枚举
- 建立角色枚举以及范围关联关系
- 重写拦截器处理方法

范围枚举
```java
@AllArgsConstructor
@Getter
public enum DataScope {
    // Scope 数据权限范围 ：ALL（全部）、DEPT（部门）、MYSELF（自己）
    ALL("ALL"),
    DEPT("DEPT"),
    MYSELF("MYSELF");
    private String name;
}
```
角色枚举
```java
@AllArgsConstructor
@Getter
public enum DataPermission {

    // 枚举类型根据范围从前往后排列，避免影响getScope
    // Scope 数据权限范围 ：ALL（全部）、DEPT（部门）、MYSELF（自己）
    DATA_MANAGER("数据管理员", "DATA_MANAGER",DataScope.ALL),
    DATA_AUDITOR("数据审核员", "DATA_AUDITOR",DataScope.DEPT),
    DATA_OPERATOR("数据业务员", "DATA_OPERATOR",DataScope.MYSELF);

    private String name;
    private String code;
    private DataScope scope;


    public static String getName(String code) {
        for (DataPermission type : DataPermission.values()) {
            if (type.getCode().equals(code)) {
                return type.getName();
            }
        }
        return null;
    }

    public static String getCode(String name) {
        for (DataPermission type : DataPermission.values()) {
            if (type.getName().equals(name)) {
                return type.getCode();
            }
        }
        return null;
    }

    public static DataScope getScope(Collection<String> code) {
        for (DataPermission type : DataPermission.values()) {
            for (String v : code) {
                if (type.getCode().equals(v)) {
                    return type.getScope();
                }
            }
        }
        return DataScope.MYSELF;
    }
}
```
重写拦截器处理类 `MyDataPermissionHandler`
```java
import lombok.SneakyThrows;
import lombok.extern.slf4j.Slf4j;
import net.sf.jsqlparser.expression.Alias;
import net.sf.jsqlparser.expression.Expression;
import net.sf.jsqlparser.expression.HexValue;
import net.sf.jsqlparser.expression.StringValue;
import net.sf.jsqlparser.expression.operators.conditional.AndExpression;
import net.sf.jsqlparser.expression.operators.relational.EqualsTo;
import net.sf.jsqlparser.expression.operators.relational.ExpressionList;
import net.sf.jsqlparser.expression.operators.relational.InExpression;
import net.sf.jsqlparser.expression.operators.relational.ItemsList;
import net.sf.jsqlparser.schema.Column;
import net.sf.jsqlparser.schema.Table;
import net.sf.jsqlparser.statement.select.PlainSelect;

import java.lang.reflect.Method;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

@Slf4j
public class MyDataPermissionHandler {

    private RemoteRoleService remoteRoleService;
    private RemoteUserService remoteUserService;


    /**
     * 获取数据权限 SQL 片段
     *
     * @param plainSelect  查询对象
     * @param whereSegment 查询条件片段
     * @return JSqlParser 条件表达式
     */
    @SneakyThrows(Exception.class)
    public Expression getSqlSegment(PlainSelect plainSelect, String whereSegment) {
        remoteRoleService = SpringUtil.getBean(RemoteRoleService.class);
        remoteUserService = SpringUtil.getBean(RemoteUserService.class);

        // 待执行 SQL Where 条件表达式
        Expression where = plainSelect.getWhere();
        if (where == null) {
            where = new HexValue(" 1 = 1 ");
        }
        log.info("开始进行权限过滤,where: {},mappedStatementId: {}", where, whereSegment);
        //获取mapper名称
        String className = whereSegment.substring(0, whereSegment.lastIndexOf("."));
        //获取方法名
        String methodName = whereSegment.substring(whereSegment.lastIndexOf(".") + 1);
        Table fromItem = (Table) plainSelect.getFromItem();
        // 有别名用别名，无别名用表名，防止字段冲突报错
        Alias fromItemAlias = fromItem.getAlias();
        String mainTableName = fromItemAlias == null ? fromItem.getName() : fromItemAlias.getName();
        //获取当前mapper 的方法
        Method[] methods = Class.forName(className).getMethods();
        //遍历判断mapper 的所以方法，判断方法上是否有 UserDataPermission
        for (Method m : methods) {
            if (Objects.equals(m.getName(), methodName)) {
                UserDataPermission annotation = m.getAnnotation(UserDataPermission.class);
                if (annotation == null) {
                    return where;
                }
                // 1、当前用户Code
                User user = SecurityUtils.getUser();
                // 2、当前角色即角色或角色类型（可能多种角色）
                Set<String> roleTypeSet = remoteRoleService.currentUserRoleType();
                
                DataScope scopeType = DataPermission.getScope(roleTypeSet);
                switch (scopeType) {
                    // 查看全部
                    case ALL:
                        return where;
                    case DEPT:
                        // 查看本部门用户数据
                        // 创建IN 表达式
                        // 创建IN范围的元素集合
                        List<String> deptUserList = remoteUserService.listUserCodesByDeptCodes(user.getDeptCode());
                        // 把集合转变为JSQLParser需要的元素列表
                        ItemsList deptList = new ExpressionList(deptUserList.stream().map(StringValue::new).collect(Collectors.toList()));
                        InExpression inExpressiondept = new InExpression(new Column(mainTableName + ".creator_code"), deptList);
                        return new AndExpression(where, inExpressiondept);
                    case MYSELF:
                        // 查看自己的数据
                        //  = 表达式
                        EqualsTo usesEqualsTo = new EqualsTo();
                        usesEqualsTo.setLeftExpression(new Column(mainTableName + ".creator_code"));
                        usesEqualsTo.setRightExpression(new StringValue(user.getUserCode()));
                        return new AndExpression(where, usesEqualsTo);
                    default:
                        break;
                }
            }

        }
        //说明无权查看，
        where = new HexValue(" 1 = 2 ");
        return where;
    }
}
```
 需要注意的点可能有：

- 记得把拦截器加到MyBatis-Plus的插件中，确保生效
- 要有一个业务赛选标识字段， 这里用的创建人 creator_code，也可以用dept_code 等等
