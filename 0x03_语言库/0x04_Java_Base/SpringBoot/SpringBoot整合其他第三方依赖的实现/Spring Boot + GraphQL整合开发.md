SpringBoot GraphQL<br />基于spring boot的web项目快速应用。<br />graphql-java的官方文档：Getting started with GraphQL Java and Spring Boot [https://www.graphql-java.com/tutorials/getting-started-with-spring-boot/](https://www.graphql-java.com/tutorials/getting-started-with-spring-boot/)，提供了相关依赖用以快速配置，不建议使用这个库及相关配置方式来搭建脚手架，在实际开发中，业务比较复杂的时候，会导致需要配置的业务代码比较多也比较繁琐，相对下面这种方式，代码复杂性比较高。<br />这里提供一种更灵活快捷的方式，在spring boot项目中快速应用开发。使用的依赖也和上面官方提供的都不一样，请注意区分。
<a name="ag5QN"></a>
## 快速开始
<a name="ExLrq"></a>
### 创建spring boot工程
通过Spring Initializr [https://start.spring.io/](https://start.spring.io/)快速搭建，选的jdk版本及spring boot版本，如下所示，其它版本未做兼容性测试。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297277649-935a40da-5266-474c-bddd-6cd9644ea9b5.webp#clientId=uea71b17b-3ee5-4&from=paste&id=uba98d4c5&originHeight=1033&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ub8f31078-498c-4269-a108-3ec56dc6c17)<br />点击下方的Generate按钮：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297277583-43be5c73-e49b-4b10-a471-1819228e47af.webp#clientId=uea71b17b-3ee5-4&from=paste&id=u6e198017&originHeight=198&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uc81f441b-4f03-4dfd-8b43-64ef4f32873)<br />打开工程结构如下，将application.properties删除了替换成applicaiton.yml：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297277663-5f487e54-790a-4135-bb45-97afcc513fcd.webp#clientId=uea71b17b-3ee5-4&from=paste&id=uad2b587e&originHeight=738&originWidth=924&originalType=url&ratio=1&status=done&style=none&taskId=u88105775-d57c-4663-a751-9bc1c8e25c1)
<a name="jljnx"></a>
### 引入相关依赖
pom.xml配置如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.4.6</version>
    <relativePath/> <!-- lookup parent from repository -->
  </parent>
  <groupId>com.xuxd</groupId>
  <artifactId>graphql.demo</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <name>graphql.demo</name>
  <description>GraphQL Demo project for Spring Boot</description>
  <properties>
    <java.version>1.8</java.version>
    <maven.compiler.source>1.8</maven.compiler.source>
    <maven.compiler.target>1.8</maven.compiler.target>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
    <lombok.version>1.18.20</lombok.version>
    <graphql-java-tools.version>11.0.1</graphql-java-tools.version>
    <gson.version>2.8.7</gson.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter</artifactId>
    </dependency>

    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
      <version>${lombok.version}</version>
      <scope>provided</scope>
    </dependency>

    <dependency>
      <groupId>com.graphql-java-kickstart</groupId>
      <artifactId>graphql-java-tools</artifactId>
      <version>${graphql-java-tools.version}</version>
    </dependency>

    <dependency>
      <groupId>com.google.code.gson</groupId>
      <artifactId>gson</artifactId>
      <version>${gson.version}</version>
    </dependency>
  </dependencies>

  <build>
    <plugins>
      <plugin>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-maven-plugin</artifactId>
      </plugin>
    </plugins>
  </build>

</project>
```
<a name="FWO9w"></a>
### 初始化GraphQL实例
创建一个GraphQL实例并将其注册到spring容器中，代码如下：<br />创建一个GraphQLProvider类：
```java
@Component
public class GraphQLProvider {

    private GraphQL graphQL;

    @Autowired
    private IItemService itemService;

    @Bean
    public GraphQL graphQL() {
        return graphQL;
    }

    @PostConstruct
    public void init() throws IOException {
        GraphQLSchema graphQLSchema = SchemaParser.newParser()
            .file("graphql/base.graphqls")
            .resolvers(new Query(), new Mutation())
            .file("graphql/item.graphqls")
            .resolvers(new ItemResolver(itemService))
//            .file("book.graphqls")
//            .resolvers(new BookResolver())  //其它定义照上面的示例，继续增加
            .build().makeExecutableSchema();

        this.graphQL = graphQL.newGraphQL(graphQLSchema).build();
    }

}
```
关于*.graphqls或者对应的Resolver如ItemResolver，这里只是作了微调整，相关代码如下：<br />base.grqphqls
```java
schema {
    # 查询
    query: Query
    # 更新
    mutation: Mutation
}

type Query {
    version: String
}

type Mutation {
    version: String
}
```
item.graphqls
```java
# 定义一个查询类型
extend type Query {
    queryItemList: ItemList  # 定义查询项目列表
    queryById(id: ID): Item
}

extend type Mutation {
    updateName(param: Param): Item
}

# 定义项目字段
type Item {
    id: ID!
    code: String!
    name: String!
}

type ItemList {
    itemList: [Item!]!  #获取项目列表
    total: Int!      # 获取项目总数
}

input Param {
    id: ID!
    name: String!
}
```
ItemResolver
```java
public class ItemResolver implements GraphQLQueryResolver, GraphQLMutationResolver {

    private IItemService itemService;

    public ItemResolver(IItemService itemService) {
        this.itemService = itemService;
    }

    // 对应item.graphqls里的queryItemList
    public ItemList queryItemList() {
        return itemService.queryItemList();
    }

    public Item queryById(Long id) {
        return itemService.queryById(id);
    }

    public Item updateName(Param param) {
        return itemService.updateName(param);
    }
}
```
相关业务代码比较多，就不一一贴了。
<a name="GyyIV"></a>
### 提供API
需要暴露一个接口来接收请求，并作相关处理，也只需提供一个接口即可。因此创建一个Controller：GraphqlController.
```java
@RestController
@RequestMapping("/graphql")
@Log
public class GraphqlController {

    @Autowired
    private GraphQL graphQL;

    @PostMapping
    public Object execute(@RequestBody GraphqlRequest request) {
        ExecutionInput executionInput = ExecutionInput.newExecutionInput()
            .query(request.getQuery())
            .variables(request.getVariables())
            .build();
        Map<String, Object> result = new HashMap<>();

        ExecutionResult executionResult = graphQL.execute(executionInput);
        List<GraphQLError> errors = executionResult.getErrors();

        if (errors != null && !errors.isEmpty()) {
            result.put("errors", errors);
            return result;
        }
        return executionResult.getData();
    }
}
```
到这一步，其实基本功能都已配置完成，可以启动项目进行相关测试了。<br />整个项目的代码结构如下，尽量用了一个比较常规的web项目结构（controller,service,dao等）：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297277842-d150494d-8eda-46f4-9d42-3188e28a447e.webp#clientId=uea71b17b-3ee5-4&from=paste&id=ue5361587&originHeight=1358&originWidth=824&originalType=url&ratio=1&status=done&style=none&taskId=uf5b9df6d-c0a6-49dd-8397-6854a8e9c83)
<a name="eSbaY"></a>
## 测试
示例中总共提供了3个接口，两个查询一个更新，分别进行测试：
```java
ItemList queryItemList();

Item queryById(Long id);

Item updateName(Param param);
```
<a name="Rv78i"></a>
### 查询所有项目列表（只获取每个项目的编码和名称，以及列表总数）：
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297277726-9a0d56aa-4333-41e7-a751-739bb8e91db5.webp#clientId=uea71b17b-3ee5-4&from=paste&id=u4216cac7&originHeight=985&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u4627eff5-191f-4055-969b-53ac8aabe1c)
<a name="v8NTB"></a>
### 根据ID查询，获取项目的id和名称
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297278198-bbaff0ff-dbbe-4460-baa5-cbbbe2132c59.webp#clientId=uea71b17b-3ee5-4&from=paste&id=u14a09c6b&originHeight=479&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u3ce24861-9fa5-427e-8476-d6be9132207)
<a name="DXoHu"></a>
### 更新指定ID的项目名称
项目Id为1编码为test的项目修改为“java项目”<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297278366-0576d248-da63-47b9-9c40-3bf828a58da2.webp#clientId=uea71b17b-3ee5-4&from=paste&id=ua40f4cd7&originHeight=494&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u15469f4c-a61e-4e4a-bb82-4535fee5b81)<br />再查询一下，可以看到结果更新了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635297278477-670c51e3-7369-4a66-9b6f-3433160465ce.webp#clientId=uea71b17b-3ee5-4&from=paste&id=uf0fc2175&originHeight=483&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u489ac802-fea6-4349-9237-1e720a78a4e)
