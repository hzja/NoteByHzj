Java SpringBoot MongoTemplate
<a name="wH6OC"></a>
## 1、项目引入依赖操作MongoDB的依赖
```xml
<!--集成mongodb-->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-data-mongodb</artifactId>
  <version>2.2.6.RELEASE</version>
</dependency>
```
<a name="PcINc"></a>
## 2、然后配置文件加上连接MongoDB的配置
```yaml
spring: 
  data:
    mongodb:
      host: mongodb-server
      port: 27017
      database: content
```
<a name="ad4CC"></a>
## 3、在具体项目的Service类中注入MongoTemplate 
```java
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
 
 
    @Autowired
    private MongoTemplate mongoTemplate;
```
<a name="1815434e"></a>
## 4、封装实体类
其中Document的注解至关重要。这个影响查询时能不能查询到数据
```java
import io.swagger.annotations.ApiModelProperty;
import org.springframework.data.mongodb.core.mapping.Document;
 
import java.io.Serializable;
import java.util.Date;
@Document(collection="homework_multimedia_detail")
public class HomeworkMultimediaDetailDocument implements Serializable {
    private static final long serialVersionUID = 8653911691313041614L;
    @ApiModelProperty(value = "课堂标识", required = true)
    private Long virtualClassId;
    ......
}
```
<a name="99beZ"></a>
## 5、具体的service操作mongodb库
<a name="mNvyx"></a>
### 5.1 保存数据到mongodb
```java
int num = Math.abs(studentUserId.intValue()) % 10;
String tableName = String.format("%s_%d", "homework_multimedia_detail", num);
LOGGER.info("tableName:{}",tableName);

HomeworkMultimediaDetailDocument homeworkMultimediaDetailDocument = new HomeworkMultimediaDetailDocument();
BeanUtils.copyProperties(multimediaHomeworkParam,homeworkMultimediaDetailDocument);
homeworkMultimediaDetailDocument.setVirtualClassId(virtualClassId);
homeworkMultimediaDetailDocument.setClassTypeId(classTypeId);
homeworkMultimediaDetailDocument.setStudentUserId(studentUserId);
homeworkMultimediaDetailDocument.setVoiceUrl(voiceS3Url);
homeworkMultimediaDetailDocument.setCreateTime(DateUtil.localTime());
//分库保存基础数据到mongodb,确保在用户状态先入库在入库mongodb!
mongoTemplate.insert(homeworkMultimediaDetailDocument, TABLE_NAME);
```
<a name="KnPFh"></a>
### 5.2 mongodb的简单查询
```java
Query query = new Query();
query.addCriteria(Criteria.where("virtualClassId").is(homeworkMultimedia.getVirtualClassId())
                  .and("studentUserId").is(homeworkMultimedia.getStudentUserId())
                  .and("classTypeId").is(homeworkMultimedia.getClassTypeId())
                  .and("lessonId").is(homeworkMultimedia.getLessonId())
                  .and("createTime").gte(homeworkMultimedia.getCreateTime()).lte(nowAfterSeconds)
                 );
List<HomeworkMultimediaDetailDocument> homeworkMultimediaDetailDocumentList = mongoTemplate.find(query, HomeworkMultimediaDetailDocument.class);
```
<a name="Cx2Jv"></a>
### 5.3 复杂的查询：多条件，分页，排序查询
```java
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;

public Map getStudentMultimediaHomeworkByConditions(List<Long> studentUserIdList, Integer lessonNo, Date startTime, Date endTime,
                                                    Integer pageNum, Integer pageSize, String sortField, String sortValue) {
    Map map = new HashMap<>();
    List<MultimediaResources> multimediaResourcesList = new ArrayList<>();
    Sort sort = null;
    if (StringUtils.isEmpty(sortField)) {
        sortField = "createTime";
    }
    if (StringUtils.isEmpty(sortValue)) {
        sortValue = Constants.SORT_DESC;
    }
    if (sortValue.toLowerCase().equals(Constants.SORT_DESC)) {
        sort = Sort.by(Sort.Direction.DESC, sortField);
    } else {
        sort = Sort.by(Sort.Direction.ASC, sortField);
    }
    Criteria criteria = Criteria.where("version").gte(1);
    if (lessonNo != null && lessonNo > 0) {
        criteria.and("lessonNo").is(lessonNo);
    }
    if (studentUserIdList != null && studentUserIdList.size() > 0) {
        criteria.and("studentUserId").in(studentUserIdList);
    }
    if (startTime != null) {
        if (endTime != null) {
            criteria.and("createTime").gte(startTime).lte(endTime);
        } else {
            criteria.and("createTime").gte(startTime);
        }
    } else {
        if (endTime != null) {
            criteria.and("createTime").lte(endTime);
        }
    }
    Query query = Query.query(criteria);
    //首先查询部分也的总条数
    Long count = mongoTemplate.count(query, HomeworkMultimediaDetailDocument.class);
    //导出功能没有分页
    if (pageNum != null && pageSize != null) {
        PageRequest pageRequest = PageRequest.of(pageNum - 1, pageSize);
        query.with(pageRequest);
    }
    query.with(sort);
    //然后分页,排序查询具体的list,获取到本节的作业内容具体数据。
    List<HomeworkMultimediaDetailDocument> homeworkMultimediaDetailDocumentList = mongoTemplate.find(query, HomeworkMultimediaDetailDocument.class);
    if (homeworkMultimediaDetailDocumentList != null && homeworkMultimediaDetailDocumentList.size() > 0) {
        homeworkMultimediaDetailDocumentList.forEach(homeworkMultimediaDetailDocument -> {
            MultimediaResources multimediaResources = new MultimediaResources();
            BeanUtils.copyProperties(homeworkMultimediaDetailDocument, multimediaResources);
            multimediaResourcesList.add(multimediaResources);
        });
    }
    map.put("total", count);
    map.put("list", multimediaResourcesList);
    return map;
}
```
<a name="6sLik"></a>
## 6、使用过程中需要注意的问题
<a name="9RpLI"></a>
### 6.1 mongodbTemplate分页
mongodbTemplate的分页第一页不是从1开始，而是从0开始。并且新的方法已经不是通过new创建对象。而是这样：
```java
PageRequest pageRequest = PageRequest.of(pageNum - 1, pageSize);
Sort sort = Sort.by(Sort.Direction.DESC, sortField);
```
<a name="MO0AU"></a>
### 6.2 经验1
```java
Query query = Query.query(criteria);
```
与
```java
Query query = new Query();query.addCriteria(criteria);
```
效果一样。
```java
mongoTemplate.find(query, HomeworkMultimediaDetailDocument.class,collectionName)
```
与
```java
mongoTemplate.find(query, HomeworkMultimediaDetailDocument.class)+HomeworkMultimediaDetailDocument
```
注解上映射关系效果一样。
<a name="bb2ece7c"></a>
### 6.3 经验2
聚合操作Aggregation完全可以使用Criteria+Query来代替。最多通过java代码做些处理。当然，如果Aggregation能正确查询也很厉害。Query支持单个字段多种条件，多个字段排序，分页功能。
<a name="a512ac0a"></a>
### 6.4 经验3
Query支持的条件查询一个字段只能出现一次。像这样：
```java
criteria.and("createTime").gte(startTime).lte(endTime);
```
如果这样写就会运行报错，正确的写法如下：
```java
criteria.and("createTime").gte(startTime); 
criteria.and("createTime").lte(endTime);
```
<br />
