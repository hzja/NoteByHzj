JavaSpringBootNeo4j<br />了解北欧神话的小伙伴们应该知道，它的神话体系可以用一个字来形容，那就是『**乱**』！就像是雷神3中下面这张错综复杂的关系网，也只能算是其中的一支半节。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961167-ab6bac61-730a-4cce-9f7d-824f8261248a.png#clientId=u8e06e699-2c91-4&from=paste&id=ucd48529a&originHeight=504&originWidth=748&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud059bed6-1827-47c4-8cc8-332fa957405&title=)<br />来看看，如何在SpringBoot项目中，实现并呈现这张雷神中复杂的人物关系图谱。<br />本文将通过下面几个主要模块，构建自然界中实体间的联系，实现知识图谱描述：

- 图数据库neo4j安装
- 简单CQL入门
- springboot整合neo4j
- 文本SPO抽取
- 动态构建知识图谱
<a name="skm3W"></a>
## Neo4j安装
知识图谱的底层依赖于关键的图数据库，在这里选择Neo4j，它是一款高性能的 nosql 图形数据库，能够将结构化的数据存储在**图**而不是**表**中。<br />首先进行安装，打开官网下载Neo4j的安装包，下载免费的community社区版就可以，地址放在下面：<br />[https://neo4j.com/download/other-releases/](https://neo4j.com/download/other-releases/)<br />需要注意的是，neo4j 4.x以上的版本都需要依赖 jdk11环境，所以如果运行环境是jdk8的话，那么还是老老实实下载3.x版本就行，下载解压完成后，在bin目录下通过命令启动：
```bash
neo4j console
```
启动后在浏览器访问安装服务器的7474端口，就可以打开neo4j的控制台页面：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961088-e7d82beb-d216-43fd-a932-0e24512d2991.png#clientId=u8e06e699-2c91-4&from=paste&id=ueca9ac0d&originHeight=581&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad75d735-0bf0-4207-a8a1-840552b8f4e&title=)<br />通过左侧的导航栏，依次可以查看存储的数据、一些基础查询的示例以及一些帮助说明。<br />而顶部带有`$`符号的输入框，可以用来输入neo4j特有的CQL查询语句并执行，具体的语法放在下面介绍。
<a name="XN611"></a>
## 简单CQL入门
就像平常使用关系型数据库中的SQL语句一样，neo4j中可以使用Cypher查询语言（CQL）进行图形数据库的查询，简单来看一下增删改查的用法。
<a name="gHMeZ"></a>
### 添加节点
在CQL中，可以通过CREATE命令去创建一个节点，创建不含有属性节点的语法如下：
```sql
CREATE (<node-name>:<label-name>)
```
在CREATE语句中，包含两个基础元素，节点名称node-name和标签名称lable-name。标签名称相当于关系型数据库中的表名，而节点名称则代指这一条数据。<br />以下面的CREATE语句为例，就相当于在Person这张表中创建一条没有属性的空数据。
```sql
CREATE (索尔:Person)
```
而创建包含属性的节点时，可以在标签名称后面追加一个描绘属性的json字符串：
```sql
CREATE (
   <node-name>:<label-name>
   {    
      <key1>:<value1>,
      …
      <keyN>:<valueN>
   }
)
```
用下面的语句创建一个包含属性的节点：
```sql
CREATE (洛基:Person {name:"洛基",title:"诡计之神"})
```
<a name="jeH0i"></a>
### 查询节点
在创建完节点后，就可以使用`MATCH`匹配命令查询已存在的节点及属性的数据，命令的格式如下：
```sql
MATCH (<node-name>:<label-name>)
```
通常，`MATCH`命令在后面配合`RETURN`、`DELETE`等命令使用，执行具体的返回或删除等操作。<br />执行下面的命令：
```sql
MATCH (p:Person) RETURN p
```
查看可视化的显示结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961082-8329e09b-2bd2-4aa2-8aa2-403c6b1b9630.png#clientId=u8e06e699-2c91-4&from=paste&id=u554033bf&originHeight=521&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u451a95ab-1761-4f2e-843e-25184585363&title=)<br />可以看到上面添加的两个节点，分别是不包含属性的空节点和包含属性的节点，并且所有节点会有一个默认生成的id作为唯一标识。
<a name="qsVQy"></a>
### 删除节点
接下来，删除之前创建的不包含属性的无用节点，上面提到过，需要使用`MATCH`配合`DELETE`进行删除。
```sql
MATCH (p:Person) WHERE id(p)=100 
DELETE p
```
在这条删除语句中，额外使用了`WHERE`过滤条件，它与SQL中的`WHERE`非常相似，命令中通过节点的id进行了过滤。<br />删除完成后，再次执行查询操作，可以看到只保留了洛基这一个节点：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961080-1f272fbf-0e3c-493b-a3d3-08fa238da92b.png#clientId=u8e06e699-2c91-4&from=paste&id=u7b74cf65&originHeight=405&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6c78b7a7-cad6-413f-80ac-4253f27afd7&title=)
<a name="g7DtB"></a>
### 添加关联
在neo4j图数据库中，遵循属性图模型来存储和管理数据，也就是说可以维护节点之间的关系。<br />在上面，创建过一个节点，所以还需要再创建一个节点作为关系的两端：
```sql
CREATE (p:Person {name:"索尔",title:"雷神"})
```
创建关系的基本语法如下：
```sql
CREATE (<node-name1>:<label-name1>) 
- [<relation-name>:<relation-label-name>]
-> (<node-name2>:<label-name2>)
```
当然，也可以利用已经存在的节点创建关系，下面借助`MATCH`先进行查询，再将结果进行关联，创建两个节点之间的关联关系：
```sql
MATCH (m:Person),(n:Person) 
WHERE m.name='索尔' and n.name='洛基' 
CREATE (m)-[r:BROTHER {relation:"无血缘兄弟"}]->(n)
RETURN r
```
添加完成后，可以通过关系查询符合条件的节点及关系：
```sql
MATCH (m:Person)-[re:BROTHER]->(n:Person) 
RETURN m,re,n
```
可以看到两者之间已经添加了关联：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961156-8acd5112-21c1-4fce-ad5b-6c69ff4a097e.png#clientId=u8e06e699-2c91-4&from=paste&id=u948534d7&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc284d657-07e5-4faf-a9ee-943dc06973d&title=)<br />需要注意的是，如果节点被添加了关联关系后，单纯删除节点的话会报错，：
```sql
Neo.ClientError.Schema.ConstraintValidationFailed
Cannot delete node<85>, because it still has relationships. To delete this node, you must first delete its relationships.
```
这时，需要在删除节点时同时删除关联关系：
```sql
MATCH (m:Person)-[r:BROTHER]->(n:Person) 
DELETE m,r
```
执行上面的语句，就会在删除节点的同时，删除它所包含的关联关系了。<br />那么，简单的cql语句入门到此为止，它已经基本能够满足简单业务场景了，下面开始在SpringBoot中整合neo4j。
<a name="zy9C1"></a>
## SpringBoot整合Neo4j
创建一个springboot项目，这里使用的是2.3.4版本，引入neo4j的依赖坐标：
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-neo4j</artifactId>
</dependency>
```
在application.yml中配置neo4j连接信息：
```yaml
spring:
  data:
    neo4j:
      uri: bolt://127.0.0.1:7687
      username: neo4j
      password: 123456
```
大家如果对jpa的应用非常熟练的话，那么接下来的过程可以说是轻车熟路，因为它们基本上是一个模式，同样是构建model层、repository层，然后在此基础上操作自定义或模板方法就可以了。
<a name="oTvvq"></a>
### 节点实体
可以使用基于注解的实体映射来描述图中的节点，通过在实体类上添加`@NodeEntity`表明它是图中的一个节点实体，在属性上添加`@Property`代表它是节点中的具体属性。
```java
@Data
@NodeEntity(label = "Person")
public class Node {
    @Id
    @GeneratedValue
    private Long id;

    @Property(name = "name")
    private String name;

    @Property(name = "title")
    private String title;
}
```
这样一个实体类，就代表它创建的实例节点的`<label-name>`为Person，并且每个节点拥有name和title两个属性。
<a name="WLx84"></a>
### Repository持久层
对上面的实体构建持久层接口，继承`Neo4jRepository`接口，并在接口上添加`@Repository`注解即可。
```java
@Repository
public interface NodeRepository extends Neo4jRepository<Node,Long> {
    @Query("MATCH p=(n:Person) RETURN p")
    List<Node> selectAll();

    @Query("MATCH(p:Person{name:{name}}) return p")
    Node findByName(String name);
}
```
在接口中添加了个两个方法，供后面测试使用，`selectAll()`用于返回全部数据，`findByName()`用于根据name查询特定的节点。<br />接下来，在service层中调用repository层的模板方法：
```java
@Service
@AllArgsConstructor
public class NodeServiceImpl implements NodeService {
    private final NodeRepository nodeRepository;
    
    @Override
    public Node save(Node node) {
        Node save = nodeRepository.save(node);
        return save;
    }
}
```
前端调用`save()`接口，添加一个节点后，再到控制台用查询语句进行查询，可以看到新的节点已经通过接口方式被添加到了图中：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961432-32aed6d6-5a88-4ee2-8282-a2b61806d57e.png#clientId=u8e06e699-2c91-4&from=paste&id=ubf299095&originHeight=467&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c84ba30-8e44-4a89-9608-f1bcf73c594&title=)<br />在service中再添加一个方法，用于查询全部节点，直接调用在`NodeRepository`中定义的`selectAll()`方法：
```java
@Override
public List<Node> getAll() {
    List<Node> nodes = nodeRepository.selectAll();
    nodes.forEach(System.out::println);
    return nodes;
}
```
在控制台打印了查询结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961423-0d26bfbd-18a7-45ef-b92f-64c27d4343f3.png#clientId=u8e06e699-2c91-4&from=paste&id=ufca24cc0&originHeight=200&originWidth=535&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u696cd257-4df9-428d-8d0f-8dd05bbf9d4&title=)<br />对节点的操作就介绍到这里，接下来开始构建节点间的关联关系。
<a name="UURPK"></a>
### 关联关系
在neo4j中，关联关系其实也可以看做一种特殊的实体，所以可以用实体类来对其进行描述。与节点不同，需要在类上添加`@RelationshipEntity`注解，并通过`@StartNode`和`@EndNode`指定关联关系的开始和结束节点。
```java
@Data
@RelationshipEntity(type = "Relation")
public class Relation {
    @Id
    @GeneratedValue
    private Long id;

    @StartNode
    private Node startNode;

    @EndNode
    private Node endNode;

    @Property
    private String relation;
}
```
同样，接下来也为它创建一个持久层的接口：
```java
@Repository
public interface RelationRepository extends Neo4jRepository<Relation,Long> {
    @Query("MATCH p=(n:Person)-[r:Relation]->(m:Person) " +
            "WHERE id(n)={startNode} and id(m)={endNode} and r.relation={relation}" +
            "RETURN p")
    List<Relation> findRelation(@Param("startNode") Node startNode,
                                @Param("endNode") Node endNode,
                                @Param("relation") String relation);
}
```
在接口中自定义了一个根据起始节点、结束节点以及关联内容查询关联关系的方法，会在后面用到。
<a name="xcbsH"></a>
### 创建关联
在service层中，创建提供一个根据节点名称构建关联关系的方法：
```java
@Override
public void bind(String name1, String name2, String relationName) {
    Node start = nodeRepository.findByName(name1);
    Node end = nodeRepository.findByName(name2);

    Relation relation =new Relation();
    relation.setStartNode(start);
    relation.setEndNode(end);
    relation.setRelation(relationName);

    relationRepository.save(relation);
}
```
通过接口调用这个方法，绑定海拉和索尔之间的关系后，查询结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961387-4742760e-c425-41a9-a97f-c7ae4e298e20.png#clientId=u8e06e699-2c91-4&from=paste&id=u0b322030&originHeight=500&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1f67667e-5a95-41d4-a364-dcd7af9d741&title=)
<a name="QlxOZ"></a>
## 文本SPO抽取
在项目中构建知识图谱时，很大一部分场景是基于非结构化的数据，而不是由手动输入确定图谱中的节点或关系。因此，需要基于文本进行知识抽取的能力，简单来说就是要在一段文本中抽取出SPO主谓宾三元组，来构成图谱中的点和边。<br />这里借助Git上一个现成的工具类，来进行文本的语义分析和SPO三元组的抽取工作，项目地址：<br />[https://github.com/hankcs/MainPartExtractor](https://github.com/hankcs/MainPartExtractor)<br />这个项目虽然比较简单一共就两个类两个资源文件，但其中的工具类却能够有效的完成句子中的主谓宾的提取，使用它前需要先引入依赖的坐标：
```xml
<dependency>
  <groupId>com.hankcs</groupId>
  <artifactId>hanlp</artifactId>
  <version>portable-1.2.4</version>
</dependency>
<dependency>
  <groupId>edu.stanford.nlp</groupId>
  <artifactId>stanford-parser</artifactId>
  <version>3.3.1</version>
</dependency>
```
然后把这个项目中com.hankcs.nlp.lex包下的两个类拷到项目中，把resources下的models目录拷贝到resources下。<br />完成上面的步骤后，调用`MainPartExtractor`工具类中的方法，进行一下简单的文本SPO抽取测试：
```java
public void mpTest(){
    String[] testCaseArray = {
        "我一直很喜欢你",
        "你被我喜欢",
        "美丽又善良的你被卑微的我深深的喜欢着……",
        "小米公司主要生产智能手机",
        "他送给了我一份礼物",
        "这类算法在有限的一段时间内终止",
        "如果大海能够带走我的哀愁",
        "天青色等烟雨，而我在等你",
        "我昨天看见了一个非常可爱的小孩"
    };
    for (String testCase : testCaseArray) {
        MainPart mp = MainPartExtractor.getMainPart(testCase);
        System.out.printf("%s   %s   %s \n",
                          GraphUtil.getNodeValue(mp.getSubject()),
                          GraphUtil.getNodeValue(mp.getPredicate()),
                          GraphUtil.getNodeValue(mp.getObject()));
    }
}
```
在处理结果`MainPart`中，比较重要的是其中的`subject`、`predicate`和`object`三个属性，它们的类型是`TreeGraphNode`，封装了句子的主谓宾语成分。下面看一下测试结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961570-ce74301c-ea07-41cd-9982-6ce1ac4e50f6.png#clientId=u8e06e699-2c91-4&from=paste&id=u719a920a&originHeight=283&originWidth=360&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u67d7bf5f-1725-4ead-9f40-82b266f2410&title=)<br />可以看到，如果句子中有明确的主谓宾语，那么则会进行抽取。如果某一项为空，则该项为null，其余句子结构也能够正常抽取。
<a name="Gy7lA"></a>
## 动态构建知识图谱
在上面的基础上，就可以在项目中动态构建知识图谱了，新建一个`TextAnalysisServiceImpl`，其中实现两个关键方法。<br />首先是根据句子中抽取的主语或宾语在neo4j中创建节点的方法，这里根据节点的name判断是否为已存在的节点，如果存在则直接返回，不存在则添加：
```java
private Node addNode(TreeGraphNode treeGraphNode){
    String nodeName = GraphUtil.getNodeValue(treeGraphNode);

    Node existNode = nodeRepository.findByName(nodeName);
    if (Objects.nonNull(existNode))
        return existNode;

    Node node =new Node();
    node.setName(nodeName);
    return nodeRepository.save(node);
}
```
然后是核心方法，说白了也很简单，参数传进来一个句子作为文本先进行spo的抽取，对实体进行Node的保存，再查看是否已经存在同名的关系，如果不存在则创建关联关系，存在的话则不重复创建。下面是关键代码：
```java
@Override
public List<Relation> parseAndBind(String sentence) {
    MainPart mp = MainPartExtractor.getMainPart(sentence);

    TreeGraphNode subject = mp.getSubject();    //主语
    TreeGraphNode predicate = mp.getPredicate();//谓语
    TreeGraphNode object = mp.getObject();      //宾语

    if (Objects.isNull(subject) || Objects.isNull(object))
        return null;

    Node startNode = addNode(subject);
    Node endNode = addNode(object);
    String relationName = GraphUtil.getNodeValue(predicate);//关系词

    List<Relation> oldRelation = relationRepository
            .findRelation(startNode, endNode,relationName);
    if (!oldRelation.isEmpty())
        return oldRelation;

    Relation botRelation=new Relation();
    botRelation.setStartNode(startNode);
    botRelation.setEndNode(endNode);
    botRelation.setRelation(relationName);
    Relation relation = relationRepository.save(botRelation);

    return Arrays.asList(relation);
}
```
创建一个简单的controller接口，用于接收文本：
```java
@GetMapping("parse")
public List<Relation> parse(String sentence) {
    return textAnalysisService.parseAndBind(sentence);
}
```
接下来，从前端传入下面几个句子文本进行测试：
```java
海拉又被称为死亡女神
死亡女神捏碎了雷神之锤
雷神之锤属于索尔
```
调用完成后，再来看看neo4j中的图形关系，可以看到**海拉**、**死亡女神**、**索尔**、**锤**这些实体被关联在了一起：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663393961571-5b47410a-8ffa-4e11-8fdd-dbee075c5fc7.png#clientId=u8e06e699-2c91-4&from=paste&id=ue96542c2&originHeight=392&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u52795a5e-61b4-4dc4-a0f5-2535c7bb09a&title=)<br />到这里，一个简单的文本处理和图谱创建的流程就被完整的串了起来，但是这个流程还是比较粗糙，之后还需要在下面几个方面继续优化：

- 当前使用的还是单一类型的节点和关联关系，后续可以在代码中丰富更多类型的节点和关联关系实体类
- 文中使用的文本spo抽取效果一般，如果应用于企业项目，那么建议基于更精确的nlp算法去做语义分析
- 当前抽取的节点只包含了实体的名称，不包含具体的属性，后续需要继续完善补充实体的属性
- 完善知识融合，主要是添加实体的指代消解以及属性的融合功能
