Java SpringCloud Docker Jenkins GitLab Maven
<a name="zfGcE"></a>
## 1、前言与初衷
本文章会涉及Docker常见命令基础知识点结合不同场景实操一起使用。<br />本文章会涉及结合工作过程中部署不同环境服务器的项目案例场景为初心进行实际细讲。<br />本文章主要讲述Docker、Jenkins、GitLab、Git、JDK、SpringBoot、Maven等技术结合实现自动化运维部署（DevOps）应用工程，适合SpringCloud部署。<br />初衷想法：在学习过程中遇到比较有趣的问题、然而花了点心血和时间去整理，然而进行梳理出来一份文章比较完整有知识体系的DevOps自动化构建与部署工程文章，技术知识内容比较多，而且文章内容较长，然而分了几个章程来讲述
<a name="lAsLw"></a>
## 2、什么是DevOps？
DevOps（Development和Operations的组合词）是一组过程、方法与系统的统称，用于促进开发（应用程序/软件工程）、技术运营和质量保障（QA）部门之间的沟通、协作与整合，它是一种重视“软件开发人员（Dev）”和“IT运维技术人员（Ops）”之间沟通合作的文化、运动或惯例。透过自动化“软件交付”和“架构变更”的流程，来使得构建、测试、发布软件能够更加地快捷、频繁和可靠。<br />它的出现是由于软件行业日益清晰地认识到：为了按时交付软件产品和服务，开发和运营工作必须紧密合作。<br />特别说明<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184944919-43ddf787-1aa5-428e-ad57-0f6765e9da4c.webp#clientId=u91814063-8273-4&from=paste&id=uc5b572e4&originHeight=125&originWidth=752&originalType=url&ratio=3&status=done&style=shadow&taskId=u74b0c031-d79a-4d7f-a5c3-84705065541)<br />1、如何使用Maven结合Docker把SpringBoot应用编译成可用的镜像进行部署。<br />2、其中JDK和Maven是传统方式进行安装，由于本人Centos操作系统是有其他软件依赖它们，有时候传统方式安装软件会更好，这里不过多的阐述。有些软件在Docker安装过程与使用过程并没传统方式的简单，比如：Jenkins。
<a name="vJV2u"></a>
## 3、需要准备的工作有哪些
<a name="nZZdR"></a>
### 3.1 工程结构

- 打开IDEA或Eclipse新建一个SpringBoot的应用.

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184944801-fdca1ffb-26a8-418c-9802-c546c0bc2436.webp#clientId=u91814063-8273-4&from=paste&id=u61a15c20&originHeight=171&originWidth=749&originalType=url&ratio=3&status=done&style=shadow&taskId=u5cf7e492-d12c-47c4-bc7a-eae9f78cebf)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184945129-43a91bb1-7ec5-4ff5-8a31-be6b7081a772.webp#clientId=u91814063-8273-4&from=paste&id=udc76af5b&originHeight=592&originWidth=645&originalType=url&ratio=3&status=done&style=shadow&taskId=u2360415c-d432-4664-9221-ddb1ce1d58c)<br />环境配置特别说明
:::tips
注意事项：其中Gitlab、Registry、Jenkins都安装在node1机器上面，也就是node1作为主机(master),node2作为slave（从机或副机），机器名起有意义或能区分即可，推荐起master和slave，这里就不作过多的阐述，为了避免看文章有疑问，请看清单列表.
:::
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184944952-900e942c-c934-4833-a010-2f35ea6fe512.webp#clientId=u91814063-8273-4&from=paste&id=u52b5f7ea&originHeight=126&originWidth=748&originalType=url&ratio=3&status=done&style=shadow&taskId=ue6a9c6e0-fce5-4491-befb-47af66efe66)
<a name="ex9ww"></a>
### 3.2、SpringBoot配置和代码详解
<a name="MDqCg"></a>
#### 3.2.1 工程的pom.xml配置

- SpringBoot和Docker依赖的jar配置
```xml
<dependencies>
  <!-- Springboot依赖的Jar包 -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter</artifactId>
  </dependency>
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
  </dependency>

  <!-- Springboot热部署jar-->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-devtools</artifactId>
    <scope>runtime</scope>
    <optional>true</optional>
  </dependency>

  <!--yml配置文件提示插件-->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <optional>true</optional>
  </dependency>

  <!-- spring-boot测试jar -->
  <dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-test</artifactId>
    <scope>test</scope>
  </dependency>

</dependencies>
<build>
  <finalName>springboot</finalName>
  <!-- 一定要声明如下配置 打包xml 到Jar包 -->
  <!-- <resources>
            <resource>
                <directory>src/main/java</directory>
                是否替换资源中的属性
                <filtering>false</filtering>
            </resource>
        </resources>
        <sourceDirectory>${project.basedir}/src/main/java</sourceDirectory>
         -->
  <plugins>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-compiler-plugin</artifactId>
      <!-- 默认支持jdk1.8编译 -->
      <configuration>
        <source>1.8</source>
        <target>1.8</target>
      </configuration>
    </plugin>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <executions>
        <execution>
          <goals>
            <goal>repackage</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
    <!--docke rmaven编译插件-->
    <plugin>
      <groupId>com.spotify</groupId>
      <artifactId>docker-maven-plugin</artifactId>
      <version>0.4.12</version>
      <configuration>
        <dockerDirectory>${project.basedir}</dockerDirectory>
        <resources>
          <resource>
            <targetPath>/</targetPath>
            <directory>${project.build.directory}</directory>
            <include>${project.build.finalName}.jar</include>
          </resource>
        </resources>
      </configuration>
    </plugin>
    <plugin>
      <groupId>org.apache.maven.plugins</groupId>
      <artifactId>maven-jar-plugin</artifactId>
      <configuration>
        <archive>
          <manifest>
            <mainClass>com.flong.SpringbootApplication</mainClass>
          </manifest>
        </archive>
      </configuration>
    </plugin> 
  </plugins>
</build>
```
<a name="JAmgN"></a>
#### 3.2.2 no main manifest attribute错误解决

- 配置工程主入口
```xml
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-jar-plugin</artifactId>
    <configuration>
        <archive>
            <manifest>
          <mainClass>com.flong.SpringbootApplication</mainClass>
            </manifest>
        </archive>
    </configuration>
</plugin> 
```
<a name="N28Cw"></a>
#### 3.2.3 env环境变量文件

- 用于设置环境动态参数,文件是以.env为格式

`JAVA_OPTS_DEFAULT=-Xmx512m`
<a name="HZbOr"></a>
#### 3.2.4 Dockerfile打包工程镜像细讲

- 以开发环境的Dockerfile为例,如果是测试环境则,把所有路径包含springboot_dev改成springboot_test
```dockerfile
FROM frolvlad/alpine-oraclejdk8:slim
MAINTAINER jilongliang@sina.com
RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
RUN mkdir -p /home/devsoft/springboot_dev
WORKDIR /home/devsoft/springboot_dev
EXPOSE 7011
ADD ./target/springboot.jar ./
CMD java ${JAVA_OPTS_DEFAULT} -Djava.security.egd=file:/dev/./urandom -jar springboot.jar
```

- 参数说明

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184945090-ab92ac44-02fd-44df-a635-e10f5a5c6ddc.webp#clientId=u91814063-8273-4&from=paste&id=u3d6280ec&originHeight=227&originWidth=746&originalType=url&ratio=3&status=done&style=none&taskId=ua3f4a022-6520-4ffc-90db-f726a6cc8fb)

- WORKDIR 工作目录说明进入容器此时会有一个.jar是在Dockerfile的ADD添加进去
```bash
docker exec -it 容器名称或容器id /bin/sh 
```
或要使用sh和bash要看COMMAND，-it
```bash
docker exec -it 容器名称或容器id/bin/bash
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184947480-24881ad6-a7d5-4618-bd25-0827a466ecfc.webp#clientId=u91814063-8273-4&from=paste&id=u3118a5c7&originHeight=151&originWidth=728&originalType=url&ratio=3&status=done&style=none&taskId=u1c580de0-46d5-466b-bf42-3ee5681796d)
<a name="K3ybe"></a>
#### 3.2.5 build.sh文件shell脚本详解
注意点1：经过测试动态变量的【等号】不能有空格和tab键置位,否则获取不了值，而且在shell脚本代码里面不支持空格格式化，支持tab置位格式化。在终端(ssh软件端)或Jenkins客户端shell命令,『位置变量』的参数以空格隔开。<br />如：`sh build.sh 192.168.1.235 springboot 0.0.1 7011 /home/jenkins/workspace/springboot_dev`<br />`$IMG_NAME:$IMG_VERSION这个IMG_VERSION`版本(tag)参数不指定默认latest<br />注意点2：通常情况下Docker是默认执行Dockerfile，但是可以自定义后缀文件进行编译，前提必须要-f（force）强制指定文件进行运行
```bash
#!/usr/bin/env bash
# 动态变量的【等号】不能有空格和tab键置位,否则获取不了值，而且在shell脚本代码里面不支持空格格式化，支持tab置位格式化。
# 在终端(ssh软件端)或Jenkins客户端shell命令,参数以空格隔开。如：sh build.sh 192.168.1.235 springboot 0.0.1 7011 /home/jenkins/workspace/springboot_dev 
IMG_SERVER="$1"
IMG_NAME="$2"
IMG_VERSION="$3"
IMG_PORT="$4"
RUN_EVN="$5"
IMG_PATH="$6"

echo "服务地址：$IMG_SERVER"
echo "工程镜像名称：$IMG_NAME"
echo "工程版本号：$IMG_VERSION"
echo "工程端口：$IMG_PORT"
echo "服务环境：$RUN_EVN"

#私服访问url路径和编译之后镜像文件存放到指定路径固定,不动态参数进行处理传值.
REGISTRY_URL="192.168.1.235:5000"
IMG_TAR_GZ_PATH="/home/img_tar_gz_path/"

# 判断动态参数不为空字符串的时候才执行下面操作
if [ "$IMG_SERVER" != "" ] && [ "$IMG_NAME" != "" ] && [ "$IMG_VERSION" != "" ] && [ "$IMG_PORT" != "" ]; then
    
   echo " .......进入删除  Container & Images 操作 ......."
    # 清理虚悬镜像,释放磁盘空间
    #docker images|grep none|awk '{print $3 }'|xargs docker rmi

    # 获取容器ID
    CONTAINER_ID=`docker ps -a | grep $IMG_NAME | awk '{ print $1 }'`

    # 获取镜像ID
    IMAGE_ID=`docker images | grep $IMG_NAME | awk '{ print $3 }'`

    # 判断是否存在删除开发容器
    if [[ "$CONTAINER_ID" != "" ]]; then
        docker rm -f $CONTAINER_ID
    fi

    # 判断是否存在删除开发镜像
    if [[ "$IMAGE_ID" != "" ]]; then
        docker rmi -f $IMAGE_ID
    fi
    # $IMG_NAME:$IMG_VERSION 这个IMG_VERSION版本(tag)参数不指定默认latest,通过不同参数执行不同环境文件
    # -f 表示强制指定Dockerfile文件进行编译
    
    echo " .......进入Building & Images 操作 ....... "
    
    #方法1、指定不同文件存放默认的Dockerfile，使用-f进行强制编译
    #docker build -t $IMG_NAME:$IMG_VERSION -f $IMG_PATH"env/"$RUN_EVN/Dockerfile $IMG_PATH
    
    #方法2、跟据不同Dockerfile文件的后缀进行编译不同环境的文件
    docker build -t $IMG_NAME:$IMG_VERSION -f $IMG_PATH"env/"Dockerfile_$RUN_EVN $IMG_PATH
    
    
    # 将镜像打一下标签，然后安照标签进行推送到私服里面，标签名就以服务名即可
    docker tag $IMG_NAME:$IMG_VERSION $REGISTRY_URL/$IMG_NAME:$IMG_VERSION
    
    # 推镜像到私服里面
    docker push $REGISTRY_URL/$IMG_NAME:$IMG_VERSION
    
    # 判断是否存在文件夹
    if [ -d "$IMG_PATH" ];then
         echo "已经存在:"$IMG_PATH
    else
        mkdir -p $IMG_PATH
    fi

    # 保存编译之后镜像文件存放到指定路径
    docker save $IMG_NAME -o $IMG_TAR_GZ_PATH/$IMG_NAME.tar.gz
    
    echo " .......进入Runing操作 ....."
    docker run -d --network default_network --restart=always --env-file=./.env  -e spring.profiles.active=$RUN_EVN --expose=$IMG_PORT --name=$IMG_NAME  -p $IMG_PORT:$IMG_PORT $IMG_NAME:$IMG_VERSION

    echo " .......Build & Run Finish Success~...."
else
    echo " .......Illegal Command Operation ......."
fi
```
<a name="TfuDv"></a>
#### 3.2.6 Docker (save、load、tag、push，pull)命令使用

- 其中push，pull一个是推，一个是拉，在某种程度下，都是对在私服上面的镜像进行操作
- docker save命令是保存编译的tar.gz或tar压缩文件，语法如：
```bash
docker save 镜像名 -o 路径/镜像名.tar.gz
```
或
```bash
docker save 镜像名 -o 路径/镜像名.tar
```
`docker load` 命令是用于导入使用 `docker save` 命令导出的镜像，此命令非常重要，由于有些客户要求项目工程要求部署在内网，此时这个命令在无网络的内网情况下部署项目的时候就体现它重要的地位了.语法 `docker load [OPTIONS]`，在加载的过程有点慢，因为文件有点大，其中显示`Loady Layer [======]`输出信息,证实镜像是分层关系。
```bash
docker load -i /home/img_tar_gz_path/springboot.tar.gz
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184947536-63b1f39f-2887-44c3-a0ee-12cab5196216.webp#clientId=u91814063-8273-4&from=paste&id=ub0a56d24&originHeight=533&originWidth=716&originalType=url&ratio=3&status=done&style=none&taskId=u717ef826-f748-4a25-a559-5f7c29616c7)

- 参数说明

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184947806-98ad9686-3dcc-4ff0-99b9-e2d91003c0f5.webp#clientId=u91814063-8273-4&from=paste&id=u572d9247&originHeight=116&originWidth=1025&originalType=url&ratio=3&status=done&style=shadow&taskId=u80ffa40e-38d7-4cfb-a831-ed3f58b30a6)

- `docker tag`和`docker push`命令是一起结合使用，先tag后push，每个镜像名和版本是以冒号区分，而docker pull根据情况使用。
```bash
# 将镜像打一下标签，然后安照标签进行推送到私服里面，标签名就以服务名即可
docker tag 镜像名:版本号 私服路径/镜像名:版本号
# 推镜像到私服里面
docker push私服路径/镜像名:版本号
```

- 查看镜像

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184947630-2ae9ae2f-23d6-4bc9-a61f-b4a8793337e9.webp#clientId=u91814063-8273-4&from=paste&id=ued9dbac4&originHeight=254&originWidth=710&originalType=url&ratio=3&status=done&style=none&taskId=uf32f11e0-0db6-4765-9b48-01c0bf403a6)<br />浏览器验证docker push推送上私服的镜像<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184947667-fc720978-b1bc-4d3f-9e45-738d0a8445bd.webp#clientId=u91814063-8273-4&from=paste&id=u9d8cd02b&originHeight=385&originWidth=554&originalType=url&ratio=3&status=done&style=shadow&taskId=u6411a69d-de67-4013-960a-22f9e3e3d7e)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948208-4eb88a96-b30f-4fb2-a823-722cb0ab4e92.webp#clientId=u91814063-8273-4&from=paste&id=ua0985f15&originHeight=199&originWidth=554&originalType=url&ratio=3&status=done&style=shadow&taskId=u17a1cea4-37ac-4b65-b81f-0a6ef29d824)
<a name="l80Zr"></a>
#### 3.2.7 虚悬镜像

- 在docker编译不成功会或者是新版本覆盖旧版本归类为虚悬镜像,生成这个个镜像既没有仓库名，也没有标签，均为 <none>。一般来说，虚悬镜像已经失去了存在的价值，是可以随意删除的。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948319-63bcb7d4-f8be-436d-b30c-f4858b6d0f1a.webp#clientId=u91814063-8273-4&from=paste&id=u812a9674&originHeight=213&originWidth=715&originalType=url&ratio=3&status=done&style=none&taskId=ucc2ba994-1471-476e-9578-e4cb2528846)
<a name="d3QFT"></a>
#### 3.2.8 不同环境的配置文件
配置参数<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948229-73d91549-e393-4c1f-8597-16fc8c5935db.webp#clientId=u91814063-8273-4&from=paste&id=ue0a8bb7d&originHeight=168&originWidth=750&originalType=url&ratio=3&status=done&style=shadow&taskId=ub3b91ee3-6000-4755-ad3b-8f508dc2ecf)<br />不同环境配置参数内容
```yaml
server:
  port: 7011
runEvn: '开发环境'

server:
  port: 7011
runEvn: '测试环境'
```
<a name="VY5DN"></a>
#### 3.2.9 Controller测试代码
```java
@RestController
public class SimpleController {
   //读取配置动态参数
    @Value("${runEvn}")
    private String runEvn;
    
    @GetMapping("/test")
    public String test() {
        return "this spring boot " + runEvn +" date long " 
       + System.currentTimeMillis();
    }
}
```
<a name="TYM0s"></a>
## 4、非多台机器免密远程登录&Jenkins部署详解
<a name="uGfbZ"></a>
### 4.1 特别说明

- 以开发环境为例子进行说明
- 开发环境部署目标机器是与Jenkins机器同一台机器，一般情况，Jenkins是单独一台机器，这里为了节省自身电脑内存，故放在同一台机器进行演示与学习。
<a name="Qs3Pe"></a>
### 4.2 新建maven工程

- 点击Jenkins的新建任务菜单

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948371-6a8154bc-256f-4047-b4b0-1256c46825fd.webp#clientId=u91814063-8273-4&from=paste&id=ue59eb8b0&originHeight=453&originWidth=695&originalType=url&ratio=3&status=done&style=shadow&taskId=u6b55364b-4b1d-4032-83f6-7395285e617)
<a name="Q51J2"></a>
### 4.3 参数化构建过程说明
<a name="p9Hzg"></a>
#### 4.3.1 添加参数
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948697-c9b275c3-1f72-412e-adae-733b021daeff.webp#clientId=u91814063-8273-4&from=paste&id=ud55c1190&originHeight=457&originWidth=576&originalType=url&ratio=3&status=done&style=shadow&taskId=u2ae9bfa9-0398-41b1-b046-9c4f710aa05)
<a name="zZKVm"></a>
#### 4.3.2 参数说明以开发环境为案例
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948853-5ac06cd9-de69-4287-8697-f5bb22404ece.webp#clientId=u91814063-8273-4&from=paste&id=u3d0888fb&originHeight=315&originWidth=712&originalType=url&ratio=3&status=done&style=shadow&taskId=u026d3054-8c54-42e4-85b5-30242b93171)
<a name="vICKs"></a>
#### 4.3.3 源码管理
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948935-9738bf3f-83ae-44c6-a3f6-d0b9e127c335.webp#clientId=u91814063-8273-4&from=paste&id=u68925a0a&originHeight=366&originWidth=713&originalType=url&ratio=3&status=done&style=shadow&taskId=u94e37baf-8f77-4297-a438-93cc2765091)
<a name="H0GFn"></a>
#### 4.3.4 Build编译设置
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184948950-7d3b7f06-1ac0-4e9b-ac32-d4387ec00070.webp#clientId=u91814063-8273-4&from=paste&id=uc088418d&originHeight=190&originWidth=683&originalType=url&ratio=3&status=done&style=shadow&taskId=u1896db0e-4384-49aa-87e1-dae48647c71)<br />表示忽略测试单元类进行编译
```bash
clean install -U -Dmaven.test.skip=true
```
<a name="TOCR5"></a>
### 4.4 SSH Publishers设置

- 其中SSH Server Name就是在http://jenkins地址:端口/jenkins/configure设置好进行选择
- Transfer Set Source file传输文件的路径,可以使用参数构建的占位符${serverPath}获取
- Remote directory远程文件目录，同理也参数构建的占位符${serverPath}获取

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949057-628824e4-4769-4485-980f-db3d19e7cda1.webp#clientId=u91814063-8273-4&from=paste&id=ud6c9e0df&originHeight=424&originWidth=711&originalType=url&ratio=3&status=done&style=shadow&taskId=udd792348-55fd-44b3-9293-2c854faa842)

- SSH Publishers shell脚本`#!/bin/bash`表示告诉终端使用bash解析器进行执行，而且只有第一行bash才有效。
```bash
#!/bin/bash
# 创建目录
mkdir -p ${serverPath}
# 切换目录
cd ${serverPath}
# 运行脚本
sh build.sh  $server  ${appName}  ${version}  ${port} ${env} ${serverPath}
```
<a name="ThEcL"></a>
### 4.5 构建与编译部署项目

- 截图的构建参数都是在参数化构建过程配置的参数

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949261-4ec3b86f-83b1-44ee-88dc-abed07d0d0af.webp#clientId=u91814063-8273-4&from=paste&id=u01c486ae&originHeight=540&originWidth=767&originalType=url&ratio=3&status=done&style=shadow&taskId=ue2fba9f5-6216-416f-a2db-c0ebe5df2a5)
<a name="D5gQx"></a>
### 4.6 构建&运行&&部署结果

- 构建过程

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949639-78149c5b-864d-48fc-b889-45bb9798f389.webp#clientId=u91814063-8273-4&from=paste&id=ufc408e44&originHeight=594&originWidth=554&originalType=url&ratio=3&status=done&style=shadow&taskId=u80a70a11-55e2-4f44-8ee2-363c448128e)

- 部署结果

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949585-90ce1e72-de6f-4311-bf70-b329c12c2ed0.webp#clientId=u91814063-8273-4&from=paste&id=u3ede4729&originHeight=180&originWidth=554&originalType=url&ratio=3&status=done&style=shadow&taskId=u086bbe1a-5e11-43b9-95b7-f296455a0bb)
<a name="pPlLE"></a>
## 5、多台机器免密远程登录&Jenkins部署流程详解
<a name="hrFpC"></a>
### 5.1 特别说明

- 以测试环境为例子进行说明
- 步骤流程几乎一样，唯一是在SSH Publishers 和源码存放路径不一样，测试环境部署目标机器是与Jenkins机器不同一台机器
<a name="rVJ5d"></a>
### 5.2 新建maven工程

- 点击Jenkins的新建任务菜单

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949693-ddad7c4f-c890-4403-95f7-365d93876580.webp#clientId=u91814063-8273-4&from=paste&id=ue21fac2f&originHeight=453&originWidth=695&originalType=url&ratio=3&status=done&style=shadow&taskId=u2f73a79b-470f-4646-8395-8590407e3a0)
<a name="jQhOj"></a>
### 5.3 参数化构建过程说明
<a name="X7n2i"></a>
#### 5.3.1 添加参数
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949741-721941b8-d6bc-457b-a2be-a4d4e5475fae.webp#clientId=u91814063-8273-4&from=paste&id=u9fc5d437&originHeight=457&originWidth=576&originalType=url&ratio=3&status=done&style=shadow&taskId=u46c78ef2-d28f-4295-94bd-7df3fcaeae4)
<a name="SdJMw"></a>
#### 5.3.2 参数说明以测试环境为案例
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184949926-8cf0ae8f-959a-4c02-90c5-ed3ddcc11150.webp#clientId=u91814063-8273-4&from=paste&id=u26099c06&originHeight=367&originWidth=760&originalType=url&ratio=3&status=done&style=shadow&taskId=uef0b334a-a99c-44f0-95f5-974bcdb9919)
<a name="xh6GF"></a>
#### 5.3.3 源码管理
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950159-de1989b5-413b-4f6e-9130-b715e742c843.webp#clientId=u91814063-8273-4&from=paste&id=u23114260&originHeight=366&originWidth=713&originalType=url&ratio=3&status=done&style=shadow&taskId=u54662e08-d6e6-4227-aa6f-a8012e63cdb)
<a name="i645H"></a>
#### 5.3.4 Build编译设置
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950205-9c5c2787-4e10-4ad0-a7d3-6e0ee00cb0e4.webp#clientId=u91814063-8273-4&from=paste&id=ue5e29ed9&originHeight=190&originWidth=683&originalType=url&ratio=3&status=done&style=shadow&taskId=u69c56241-3206-4ba1-9884-6e309c5df6f)
<a name="rycxY"></a>
### 5.4 SSH Publishers设置

1. 其中SSH Server Name就是在http://jenkins地址:端口/jenkins/configure设置好进行选择
2. Transfer Set Source file传输文件的路径,可以使用参数构建的占位符${serverPath}获取
3. Remote directory远程文件目录，同理也参数构建的占位符${serverPath}获取
4. docker_server1表示与Jenkins部署同一个宿主机，使用脚本有远程机器shell脚本操作免登陆操作。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950562-0905d8aa-743c-4ba0-9e52-9b3aa4c43f0b.webp#clientId=u91814063-8273-4&from=paste&id=udb79dc5c&originHeight=408&originWidth=765&originalType=url&ratio=3&status=done&style=shadow&taskId=u71919655-ed73-4d3b-b0c4-dffe27750ab)

- docker_server1 Shell脚本#!/bin/bash表示告诉终端使用bash解析器进行执行，而且只有第一行bash才有效。
```bash
#!/bin/bash
#  打印信息
echo "用户名${userName}"
echo "服务器${server}"
echo "服务器目录${serverPath}"
#  远程创建存放远程上传的代码目录路径
ssh $server  mkdir -p ${targetServerPath}
#  远程拷贝代码到目标机器指定路径
scp -r ${serverPath}/  ${userName}@${server}:${targetServerPath}
```

- docker_server2表示要部署那台目标机器,所以它的脚本跟docker_server1不一样.

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950465-3e064389-2a8d-4d66-ad11-78feec933c1d.webp#clientId=u91814063-8273-4&from=paste&id=ue5fc8309&originHeight=401&originWidth=772&originalType=url&ratio=3&status=done&style=none&taskId=u5f333da0-c33d-40f6-867b-a7e78ad78e0)

- docker_server2 Shell脚本
```bash
#!/bin/bash
# 切换文件目录
cd ${serverPath}
# 执行脚本
sh build.sh ${server}  ${appName}  ${version}  ${port}  ${env} ${serverPath}
```
<a name="PmeIL"></a>
### 5.5 构建与编译部署项目

- 截图的构建参数都是在参数化构建过程配置的参数

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950556-65be5b28-4ea1-45d2-b1d8-d5bed0e32547.webp#clientId=u91814063-8273-4&from=paste&id=udedbc670&originHeight=463&originWidth=553&originalType=url&ratio=3&status=done&style=shadow&taskId=u7447a79b-c964-4958-84ef-11f701d01db)
<a name="NbQHV"></a>
### 5.6 构建&运行&部署结果

- 构建过程

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950918-939e1ebe-3973-4627-a051-4ab3b95b5d4d.webp#clientId=u91814063-8273-4&from=paste&id=u97a137e5&originHeight=590&originWidth=554&originalType=url&ratio=3&status=done&style=shadow&taskId=udd2f65e6-7c64-40c6-acc0-611890374c8)

- 部署结果

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624184950750-9b6778b1-c276-46c3-8ab4-83f01fbd1771.webp#clientId=u91814063-8273-4&from=paste&id=udd208056&originHeight=165&originWidth=530&originalType=url&ratio=3&status=done&style=shadow&taskId=u47d83ba2-a5d2-4866-9f48-7069688f8c2)

 
