Java SpringBoot Mybatis
<a name="oslAD"></a>
## 1、引入依赖
只引入PageHelper不会自动适配SpringBoot失效，还需要整合依赖pagehelper-spring-boot-autoconfigure
```xml
<!--pagehelper分页插件 -->
<dependency>
   <groupId>com.github.pagehelper</groupId>
   <artifactId>pagehelper-spring-boot-starter</artifactId>
   <version>1.3.0</version>
</dependency>
<!-- Mybatis-plus -->
<dependency>
    <groupId>com.baomidou</groupId>
    <artifactId>mybatis-plus-boot-starter</artifactId>
    <version>3.2.0</version>
</dependency>	
```
<a name="7mvIN"></a>
## 2、配置文件配置相应的参数
<a name="4rkLv"></a>
### application.yml
```yaml
pagehelper:
  auto-dialect: on
  reasonable: true
  support-methods-arguments: true
  page-size-zero: true
  params: count=countSql
```
<a name="w6etT"></a>
### application.properties
```yaml
#分页插件会自动检测当前的数据库链接，自动选择合适的分页方式。 你可以配置helperDialect 属性来指定分页插件是否开启断言。
pagehelper.helper-dialect=on
#分页合理化参数，默认值为 false 。当该参数设置为 true 时， pageNum<=0 时会查询第一页， pageNum>pages （超过总数时），会查询最后一页。
pagehelper.reasonable=true
#支持通过Mapper接口参数传递page参数,默认值为falset
pagehelper.support-methods-arguments=true
#默认值为 false ，当该参数设置为 true 时，如果 pageSize=0 或者 RowBounds.limit =0 就会查询出全部的结果（相当于没有执行分页查询，但是返回结果仍然是 Page 类型）。
pagehelper.pageSizeZero=true
#为了支持 startPage(Object params) 方法，增加了该参数来配置参数映射，用于从对象中根据属性名取值
pagehelper.params=count=countSql
```
<a name="a55303c9"></a>
## 3、使用
```java
package com.fcant.competition.service.impl;

import com.fcant.competition.bean.CompetitionType;
import com.fcant.competition.mapper.CompetitionTypeMapper;
import com.fcant.competition.service.ICompetitionTypeService;
import com.fcant.competition.utils.MsgUtil;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageInfo;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * CompetitionTypeServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:26:05 2020/4/17/0017
 */
@Service
public class CompetitionTypeServiceImpl implements ICompetitionTypeService {

    @Autowired
    CompetitionTypeMapper competitionTypeMapper;

    @Override
    public MsgUtil getAllCompetitionType(Integer page, Integer pageSize) {
        PageHelper.startPage(page, pageSize);
        List<CompetitionType> competitionTypes = competitionTypeMapper.selectAll();
        PageInfo<CompetitionType> pageInfo = new PageInfo<>(competitionTypes);
        return MsgUtil.success().addData("pageInfo", pageInfo);
    }
}
```
<a name="xfYui"></a>
## 4、使用提示

- 只有紧跟在`PageHelper.startPage()`方法后的第一个Mybatis的查询（Select）方法会被分页。
- 请不要在系统中配置多个分页插件(使用Spring时,mybatis-config.xml和Spring配置方式，请选择其中一种，不要同时配置多个分页插件)！
- 对于带有for update的sql，会抛出运行时异常，对于这样的sql建议手动分页，毕竟这样的sql需要重视。
- 由于嵌套结果方式会导致结果集被折叠，因此分页查询的结果在折叠后总数会减少，所以无法保证分页结果数量正确。
