Maven<br />在项目中使用Maven管理jar包依赖，往往会出现以下状况：<br />1、国内访问maven默认远程中央镜像特别慢；<br />2、使用阿里的镜像替代远程中央镜像；<br />3、阿里云镜像中缺少部分jar包；<br />4、同时使用私有仓库和公有仓库；<br />针对以上情况，就需要让Maven支持多仓库配置。
<a name="U6Oex"></a>
### 单独仓库配置
当只配置一个仓库时，操作比较简单，直接在Maven的settings.xml文件中的`<mirrors></mirrors>`标签中进行全局配置即可，以阿里云的镜像为例：
```xml
<mirrors>
    <mirror>
        <id>alimaven</id>
        <name>aliyun maven</name>
        <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
        <mirrorOf>central</mirrorOf>
    </mirror>
</mirrors>
```
只用新增一个mirror配置即可。要做到单一仓库，设置mirrorOf到*。<br />mirrorOf中配置的星号，表示匹配所有的artifacts，也就是everything使用这里的代理地址。上面的mirrorOf配置了具体的名字，指的是repository的名字。<br />镜像配置说明：<br />1、id：镜像的唯一标识；<br />2、name：名称描述；<br />3、url：地址；<br />4、mirrorOf： 指定镜像规则，什么情况下从镜像仓库拉取。其中， *：匹配所有，所有内容都从镜像拉取；<br />external:*：除了本地缓存的所有从镜像仓库拉取；<br />repo,repo1：repo或者repo1，这里的repo指的仓库ID；<br />*,!repo1：除了repo1的所有仓库；
<a name="aHFX1"></a>
### 环境仓库配置激活
:::danger
针对多仓库的配置在`<mirrors></mirrors>`标签中多配置几个mirror并不会生效。
:::
正确的操作是在profiles节点下配置多个profile，而且配置之后要激活。
```xml
<profiles>
    <profile>
      <id>boundlessgeo</id> 
      <repositories>
        <repository>
          <id>boundlessgeo</id> 
          <url>https://repo.boundlessgeo.com/main/</url> 
          <releases>
            <enabled>true</enabled>
          </releases> 
          <snapshots>
            <enabled>true</enabled> 
            <updatePolicy>always</updatePolicy>
          </snapshots>
        </repository>
      </repositories>
    </profile>
    <profile>
      <id>aliyun</id> 
      <repositories>
        <repository>
          <id>aliyun</id> 
          <url>http://maven.aliyun.com/nexus/content/groups/public/</url> 
          <releases>
            <enabled>true</enabled>
          </releases> 
          <snapshots>
            <enabled>true</enabled> 
            <updatePolicy>always</updatePolicy>
          </snapshots>
        </repository>
      </repositories>
    </profile> 
    <profile>
      <id>maven-central</id> 
      <repositories>
        <repository>
          <id>maven-central</id> 
          <url>http://central.maven.org/maven2/</url> 
          <releases>
            <enabled>true</enabled>
          </releases> 
          <snapshots>
            <enabled>true</enabled> 
            <updatePolicy>always</updatePolicy>
          </snapshots>
        </repository>
      </repositories>
    </profile>
<profiles>
```
通过配置activeProfiles子节点激活：
```xml
<activeProfiles>
    <activeProfile>boundlessgeo</activeProfile>
    <activeProfile>aliyun</activeProfile>
    <activeProfile>maven-central</activeProfile>
</activeProfiles>
```
此时如果是在Idea中使用了本地的Maven配置，那么在项目的Maven管理中会看到类似如下图中的profile选项。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599036847192-0d30178e-ed55-47f7-a971-46286ccb951f.png#averageHue=%23d4a868&height=182&id=EJc9q&originHeight=547&originWidth=1115&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37009&status=done&style=shadow&title=&width=371.6666666666667)<br />打包时，勾选所使用的profile即可。如果使用Maven命令打包执行命令格式如下：
```bash
mvn -P aliyun ...
```
1.如果aliyun仓库的id设置为central，则会覆盖maven里默认的远程仓库。<br />2.aliyun的仓库也可以不用配置，直接在mirrors标签内配置一个镜像仓库，mirrors镜像仓库mirrorOf的值设置为central，则也可以实现覆盖默认的仓库。
<a name="qcoMN"></a>
### 项目仓库依赖配置
在项目中添加多个仓库，是通过修改项目中的pom文件实现的。<br />思路：在项目中pom文件的repositories节点（如果没有手动添加）下添加多个repository节点，每个repository节点是一个仓库。<br />配置效果如下：
```xml
<!-- 特殊maven仓库 -->
<repositories>
    <repository>
        <id>central-repo1</id>
        <name>Maven Repository Switchboard</name>
        <url>http://repo1.maven.org/maven2/</url>
        <layout>default</layout>
        <releases>
            <enabled>true</enabled>
        </releases>
    </repository>
</repositories>
```
这里的id就是mirrorOf要使用的ID。<br />在实践的过程中发现单单配置该仓库配置并不会生效，需要同时在setting.xml中定义一个mirrorOf为central-repo1的仓库配置，与该配置的id对照。<br />setting.xml中的对照配置如下：
```xml
<mirror>
    <id>central</id>
    <name>Maven Repository Switchboard</name>
    <url>https://repo1.maven.org/maven2/</url>
    <mirrorOf>central-repo1</mirrorOf>
</mirror>
```

