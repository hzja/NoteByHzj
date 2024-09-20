JavaSpringCloud AlibabaNacos<br />介绍下 Nacos 作为配置中心的基本用法，首先了解下为什么需要使用配置中心。
<a name="DtuEb"></a>
## 一、为什么需要配置中心
**在没有配置中心之前，传统应用配置的存在以下痛点：**<br />（1）采用本地静态配置，无法保证实时性：修改配置不灵活且需要经过较长的测试发布周期，无法尽快通知到客户端，还有些配置对实时性要求很高，比方说主备切换配置或者碰上故障需要修改配置，这时通过传统的静态配置或者重新发布的方式去配置，那么响应速度是非常慢的，业务风险非常大<br />（2）易引发生产事故：比如在发布的时候，容易将测试环境的配置带到生产上，引发生产事故。<br />（3）配置散乱且格式不标准：有的用properties格式，有的用xml格式，还有的存DB，团队倾向自造轮子，做法五花八门。<br />（4）配置缺乏安全审计、版本控制、配置权限控制功能：谁？在什么时间？修改了什么配置？无从追溯，出了问题也无法及时回滚到上一个版本；无法对配置的变更发布进行认证授权，所有人都能修改和发布配置。<br />**而配置中心区别于传统的配置信息分散到系统各个角落的方式，对系统中的配置文件进行集中统一管理，而不需要逐一对单个的服务器进行管理。那这样做有什么好处呢？**<br />（1）通过配置中心，可以使得配置标准化、格式统一化<br />（2）当配置信息发生变动时，修改实时生效，无需要重新重启服务器，就能够自动感知相应的变化，并将新的变化统一发送到相应程序上，快速响应变化。比方说某个功能只是针对某个地区用户，还有某个功能只在大促的时段开放，使用配置中心后只需要相关人员在配置中心动态去调整参数，就基本上可以实时或准实时去调整相关对应的业务。<br />（3）通过审计功能还可以追溯问题
<a name="vV6aN"></a>
## 二、Nacos配置中心的使用
微服务中配置中心的主流解决方案主要有三种：Nacos、Apollo、Config+Bus，不过这里主要介绍 Nacos 作为配置中心的用法，对其他两种方式感兴趣的请自行上网查阅
<a name="vPH0G"></a>
### 1、Springboot 整合 Nacos 配置中心
（1）首先声明项目的版本信息：
```xml
<properties>
    <spring-boot.version>2.3.2.RELEASE</spring-boot.version>
    <spring-cloud.version>Hoxton.SR9</spring-cloud.version>
    <spring-cloud-alibaba.version>2.2.6.RELEASE</spring-cloud-alibaba.version>
</properties>
 
<!--  只声明依赖，不引入依赖 -->
<dependencyManagement>
    <dependencies>
        <!-- 声明springBoot版本 -->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-dependencies</artifactId>
            <version>${spring-boot.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <!-- 声明springCloud版本 -->
        <dependency>
            <groupId>org.springframework.cloud</groupId>
            <artifactId>spring-cloud-dependencies</artifactId>
            <version>${spring-cloud.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
        <!-- 声明 springCloud Alibaba 版本 -->
        <dependency>
            <groupId>com.alibaba.cloud</groupId>
            <artifactId>spring-cloud-alibaba-dependencies</artifactId>
            <version>${spring-cloud-alibaba.version}</version>
            <type>pom</type>
            <scope>import</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```
（2）添加 nacos 配置中心的 maven 依赖：
```xml
<!-- SpringCloud Ailibaba Nacos Config -->
<dependency>
  <groupId>com.alibaba.cloud</groupId>
  <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
</dependency>
```
（3）在 application.properties 文件中添加 nacos 配置中心相关配置：
```
spring.profiles.active=dev
spring.application.name=cloud-producer-server
server.port=8080
 
# nacos 配置中心地址
spring.cloud.nacos.config.server-addr=localhost:8848
# 配置文件的类型
spring.cloud.nacos.config.file-extension=yaml
```
（4）在 nacos 控制台新建一个 DataID 为 cloud-producer-server-dev.yaml 的配置集：<br />为什么 DataID 的命名为 cloud-producer-server-dev.yaml 会在下文介绍<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814619-c5e77462-3ea2-4fee-a5c3-02cb45fd6f89.png#clientId=u9eea28ff-86fb-4&from=paste&id=u03de7930&originHeight=287&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9505e9ed-1ee4-4529-a418-69d2c73933c&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814431-6afe5c89-9679-4144-bf10-aa63af6eb675.png#clientId=u9eea28ff-86fb-4&from=paste&id=uc23a6b5b&originHeight=465&originWidth=862&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1045d668-1c72-441f-bdee-1cde86724df&title=)（5）编写测试类：
```java
//配置发布之后，动态刷新配置
@RefreshScope
@RestController
@RequestMapping("provider")
public class ProviderController
{
    // 使用原生注解@Value()导入配置
    @Value("${user.id}")
    private String id;
    @Value("${user.name}")
    private String name;
    @Value("${user.age}")
    private String age;
 
    @GetMapping("getNacosConfig")
    public String providerTest()
    {
        return "我是provider，已成功获取nacos配置中心的数据：(id:" + id + ",name:" + name + ",age:" + age +")";
    }
}
```
（6）启动服务验证：<br />启动项目，访问 http://localhost:8080/provider/getNacosConfig 接口，可以看到 nacos 配置中心的配置信息已经生效并被成功获取到了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814567-1d8e4d7f-a7c3-4678-b824-2dfd3d81c797.png#clientId=u9eea28ff-86fb-4&from=paste&id=u4663fab0&originHeight=163&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81db3a78-4db0-408e-be32-81c6245b961&title=)（7）验证动态刷新配置：<br />配置的动态刷新是配置中心最核心的功能之一，假设现在需要修改 user.name 的值，那么直接改变 Nacos 中的配置会生效吗？试下直接将 Nacos 中的配置修改成 “zhangsan”，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814451-5eb2fe46-e65f-4203-8e26-5cd88c96ed1b.png#clientId=u9eea28ff-86fb-4&from=paste&id=uf52f10f7&originHeight=425&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc0a00221-c4b5-47a4-8b47-67c77a0e6e0&title=)此时不重启项目并重新访问该接口，结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814486-0d44d0f1-2dd7-43a6-83f7-ec87bea6c47c.png#clientId=u9eea28ff-86fb-4&from=paste&id=uf69b8c95&originHeight=160&originWidth=715&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u945de63f-6759-43f4-b71a-26f0a83cdfd&title=)<br />可以发现配置已经动态刷新了，这是为什么呢？其实是由于在类上添加了 `@RefreshScope` 注解而产生的效果。
```java
//配置发布之后，动态刷新配置
@RefreshScope
@RestController
@RequestMapping("provider")
public class ProviderController
```
<a name="N4aT2"></a>
### 2、Nacos 的核心概念
<a name="dTZFi"></a>
#### 2.1、Data ID
（1）Data ID 的命名格式：<br />前面演示了在 nacos 控制台新建一个 DataID 为 cloud-producer-server-dev.yaml 的数据集，那么这个 Data ID 是什么呢？<br />Data ID 是配置集的唯一标识，一个应用可以包含多个配置集，每个配置集都需要被一个有意义的名称标识。那么 Data ID 怎么取值呢？格式通俗一点就是 “前缀-环境-扩展名”，如下所示：<br />`${spring.cloud.nacos.config.prefix}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}`<br />**① prefix：** 前缀，默认是 `spring.application.name` 的值，也可以通过配置项 `spring.cloud.nacos.config.prefix` 来配置。
```
# 若不指定，默认采用应用名的方案
spring.application.name=cloud-producer-server

# 手动指定配置的dataID前缀标识
# spring.cloud.nacos.config.prefix=cloud-producer-server-config
```
**② **`**active**`**：** 配置运行环境，即为当前环境对应的 profile。<br />注意：当 `spring.profiles.active` 为空时，对应的连接符 ”-“ 也将不存在，dataId 的拼接格式变成 `${prefix}.${file-extension}`
```
# dev表示开发环境
spring.profiles.active=dev
```
**③ **`**file-exetension**`**：** 配置文件的类型，默认是 properties，也可以通过配置项 `spring.cloud.nacos.config.file-extension` 来配置，目前支持的类型有 TEXT、JSON、XML、YAML、HTML、Properties
```
# 指定配置文件类型为yaml文件
spring.cloud.nacos.config.file-extension=yaml
```
**④ 最终配置：**<br />经过前面三个步骤，最终在nacos配置中心的控制台新增配置文件就是：cloud-producer-server.yaml
<a name="GzzZM"></a>
#### 2.2、环境隔离-命名空间Namespace
Nacos 引入命名空间 Namespace 的概念来进行多环境配置和服务的管理及隔离。<br />例如，可能存在本地开发环境dev、测试环境test、生产环境prod 三个不同的环境，那么可以创建三个不同的 Namespace 区分不同的环境。创建方式如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814876-eec7ec2a-46a7-46d8-9a35-48eb8bfc4973.png#clientId=u9eea28ff-86fb-4&from=paste&id=ued7d0e2f&originHeight=573&originWidth=947&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc6e48b09-037d-4749-b964-c3edb59c3c2&title=)创建完成后，就可以在Nacos 控制台的配置列表上面看到不同的命名空间了，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989814821-8a90d40b-4c83-44a6-8ade-395adc742080.png#clientId=u9eea28ff-86fb-4&from=paste&id=u376f3c3c&originHeight=522&originWidth=947&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6950eae3-c173-4135-83d5-a8fbd8257b7&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1660990725308-7fcb8f31-08b0-4b31-9bde-8a7f8e861e3c.jpeg)<br />成功创建新命名空间后，就可以在 SpringBoot 的配置文件配置命名空间的 id 切换到对应的命名空间，并获取对应空间下的配置文件，但在没有指定命名空间配置的情况下，默认的配置都是在 public 空间中，指定命名空间的方式如下：
```
# 对应创建的命名空间的ID，此处对应的是dev命名空间
cloud.nacos.config.namespace=483bb765-a42d-4112-90bc-50b8dff768b3
```
<a name="Wetka"></a>
#### 2.3、业务隔离-Group分组
Group 也可以实现环境隔离的功能，但 Group 设计的目的主要是做同一个环境中的不同服务分组，把不同的微服务的配置文件划分到同一个分组里面去，Nacos 如果不指定 Group，则默认的分组是 DEFAULT_GROUP。<br />如果没有 Group，试想一下这个场景：有两个微服务，一个是订单系统，一个是用户系统，但是他们有着相同的配置，比如 datasource-url，那么如何区分呢？这时候 Group 就派上用场了。<br />上述场景中订单系统、用户系统可以单独分为一个组，比如 ORDER_GROUP、USER_GROUP，当然这是比较细粒度的分组，根据企业的业务也可以多个微服务分为一组。<br />接下来演示一下创建配置集时以及集成时如何指定分组，还是前面的例子，新建配置集是在如下位置指定Group分组：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1660989815058-e0e87e3f-6dbb-484f-aa4f-c5c5a2d0df89.png#clientId=u9eea28ff-86fb-4&from=paste&id=u6637245d&originHeight=301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u426c07e4-ef36-4206-a118-966928861c5&title=)接下来在 application.properties 文件分组：
```
spring.cloud.nacos.config.namespace=483bb765-a42d-4112-90bc-50b8dff768b3
```
<a name="Ao4mv"></a>
### 3、小结
Nacos 实现配置管理和动态配置刷新很简单，总结如下步骤：

- **添加对应 spring-cloud-starter-alibaba-nacos-config 依赖**
- **使用原生注解 **`**@Value()**`** 导入配置**
- **使用原生注解 **`**@RefreshScope**`** 刷新配置**
- **根据自己业务场景做好多环境配置隔离(Namespace)、不同业务配置隔离(Group)**
<a name="iVxwh"></a>
### 4、共享配置
当微服务的数量越来越多，势必会有相同的配置，这时可以将相同的配置抽取出来作为项目中共有的配置，比如集群中的数据源信息、日志的配置信息，nacos 也是支持这种一个配置中心多个配置集这种写法的。<br />（1）在nacos中新建两个 Data ID 分别是 db.yaml 和 log.yaml 的文件。<br />（2）在配置文件中分别加入部分配置内容<br />（3）在 Springboot 项目中添加如下的 nacos 配置：
```yaml
spring:
  cloud:
    nacos:
      config:
        extension-configs[0]:
          data-id: db.yaml
          # 默认为DEFAULT_GROUP
          group: DEFAULT_GROUP
          # 是否动态刷新，默认为false
          refresh: true
        extension-configs[1]:
          data-id: log.yaml
          group: DEFAULT_GROUP
          refresh: true
```
为了更加清晰的在多个应用间配置共享的 Data Id，官方推荐使用 shared-configs，配置如下：
```yaml
spring:
  cloud:
    nacos:
      config:
        shared-configs[0]:
          data-id: db.yaml
          # 默认为DEFAULT_GROUP
          group: DEFAULT_GROUP   
          # 是否动态刷新，默认为false
          refresh: true   
        shared-configs[1]:
          data-id: log.yaml
          group: DEFAULT_GROUP
          refresh: true
```
（4）思考：在这2个文件中出现相同配置，nacos如何选取？<br />当多个 Data Id 同时出现相同的配置时，它的优先级关系是 `spring.cloud.nacos.config.extension-configs[n].data-id` 其中 n 的值越大，优先级越高。<br />注意：`spring.cloud.nacos.config.extension-configs[n].data-id` 的值必须带文件扩展名，文件扩展名既可支持 properties，又可以支持 yaml/yml。此时 `spring.cloud.nacos.config.file-extension` 的配置对自定义扩展配置的 Data Id 文件扩展名没有影响。<br />（5）不同方式配置加载优先级：<br />Nacos 配置中心目前提供以下三种配置能力从 Nacos 拉取相关的配置，当三种方式共同使用时，他们的一个优先级关系是:A < B < C：<br />A：通过 `spring.cloud.nacos.config.shared-configs[n].data-id` 支持多个共享 Data Id 的配置<br />B：通过 `spring.cloud.nacos.config.extension-configs[n].data-id` 的方式支持多个扩展 Data Id 的配置<br />C：通过内部相关规则(`spring.cloud.nacos.config.prefix`、`spring.cloud.nacos.config.file-extension`、`spring.cloud.nacos.config.group`)自动生成相关的 Data Id 配置
