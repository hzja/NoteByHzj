JavaSpringBoot<br />SpringBoot项目的依赖，一般都会采用Maven管理，整个项目，一般都分为以下几部分：

- **三方依赖通过pom.xml文件配置，添加到项目中来特点：变化小，占用空间大**
- **业务代码特点：变化大，占用空间小**
- **静态资源特点：变化适中，占用空间大；不过一般的静态资源都另外管理，很少会直接放在项目里面；**

而整个项目通常会被构建成一个Jar，上传到服务器运行；整个Jar包中，三方依赖会被一并打包进去，占用空间最大的，也就是这部分依赖包；<br />比如下面这个最基本的测试 SpringBoot 项目，就一个简单的hello world接口，但是打包出来的jar就有20多M；<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477201345-bfd23525-9d50-4956-b77b-bba8d9e93964.png#clientId=u0168a634-410c-4&from=paste&id=uc3db38c0&originHeight=303&originWidth=778&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e9cec36-981d-45cb-9b28-145f139e0d7&title=)<br />把Jar包解压之后，发现三方依赖竟然比整个Jar包都大（可能压缩的原因），自己的代码只有100多K；<br />这还只是一个最基础的项目，如果业务复杂，依赖多，光是三方包可能就占用几十、几百M之多；<br />由于依赖包**变化小，占用空间大**的特点，大部分情况是第一次添加之后，后面很少会去调整；但每次修改哪怕是一行代码，都需要重新把他们构建Jar中去，往服务器上传、发布，白白消耗了大量的资源、带宽以及时间；<br />那能否将三方依赖和自己的业务代码分开打包呢？答案是：**可以的**；
<a name="K0y7N"></a>
## 1、依赖拆分配置
只需要在项目pom.xml文件中添加下面的配置：
```xml
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-compiler-plugin</artifactId>
            <configuration>
                <source>1.8</source>
                <target>1.8</target>
            </configuration>
        </plugin>
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
            <configuration>
                <fork>true</fork>
                <finalName>${project.build.finalName}</finalName>
                <!--解决windows命令行窗口中文乱码-->
                <jvmArguments>-Dfile.encoding=UTF-8</jvmArguments>
                <layout>ZIP</layout>
                <includes>
                    <!--这里是填写需要包含进去的jar，
                        必须项目中的某些模块，会经常变动，那么就应该将其坐标写进来
                        如果没有则non-exists ，表示不打包依赖
                    -->
                    <include>
                        <groupId>non-exists</groupId>
                        <artifactId>non-exists</artifactId>
                    </include>
                </includes>
            </configuration>
            <executions>
                <execution>
                    <goals>
                        <goal>repackage</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
        <!--此插件用于将依赖包抽出-->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-dependency-plugin</artifactId>
            <executions>
                <execution>
                    <id>copy-dependencies</id>
                    <phase>package</phase>
                    <goals>
                        <goal>copy-dependencies</goal>
                    </goals>
                    <configuration>
                        <outputDirectory>${project.build.directory}/lib</outputDirectory>
                        <excludeTransitive>false</excludeTransitive>
                        <stripVersion>false</stripVersion>
                        <includeScope>runtime</includeScope>
                    </configuration>
                </execution>
            </executions>
        </plugin>
        <plugin>
            <artifactId>maven-surefire-plugin</artifactId>
            <configuration>
                <skip>true</skip>
            </configuration>
        </plugin>
    </plugins>
</build>
```
再次构建
```bash
mvn clean package -DskipTests=true
```
发现target目录中多了个lib文件夹，里面保存了所有的依赖jar，自己业务相关的jar也只有小小的157kb，相比之前21M，**足足小了100多倍**；<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477217673-94f86a57-2bca-4150-a1c4-3d17764bdc0f.png#clientId=u0168a634-410c-4&from=paste&id=u9046c6a3&originHeight=566&originWidth=788&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u536ae52f-613c-4425-a1b6-d44a8c6d933&title=)<br />这种方式打的包，在项目启动时，需要通过`-Dloader.path`指定lib的路径：
```bash
java -Dloader.path=./lib -jar xxx.jar
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477232646-082a6a66-95a3-4fc0-9032-5b8a5bf9fda1.png#clientId=u0168a634-410c-4&from=paste&id=u5a38f364&originHeight=355&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9b810383-7f4e-4228-b5cb-6d824f74e0b&title=)<br />虽然这样打包，三方依赖的大小并没有任何的改变，但有个很大的不同就是自己的业务包和依赖包分开了；在不改变依赖的情况下，也就只需要第一次上传lib目录到服务器，后续业务的调整、bug修复，在没调整依赖的情况下，就只需要上传更新小小的业务包即可；既省资源又省时间；就算是依赖变化了，也只需要更新调整的依赖，没变的依赖包咱也就不管了。<br />有朋友可能会说：**这业务包确实小了，但是无形中增加了对依赖包的管理，提高了管理成本**。<br />没错，这种方式，确实增加了Jar包的管理成本，多人协调开发，构建的时候，还需要专门去关注是否有人更新依赖；<br />不过这并不是啥大事儿，Jenkins自动化工具，就能自动维护这个lib目录，减少人工核对，避免维护成本；

---

<a name="U7Yer"></a>
## 2、Jenkins 管理依赖拆分的Jar
<a name="DpJ4B"></a>
### SSH的方式
SSH的方式相比于之前的方式，只是多了管理lib中jar的过程，未调整的依赖Jar包，不上传到服务器；所以相比之前的方案，多了一个检测脚本jenkins_jar_and_lib_check.sh；他的作用就是在SSH上传之前，检测那些依赖更新了，然后只要留已更新的依赖上传到服务器；
<a name="j7XBW"></a>
#### Jenkins构建的过程

- **拉取最新代码**
- **Maven打包**
- **Jenkins本地执行jenkins_jar_and_lib_check.sh检测依赖Jar和App jar是否更新**
- **上传已经更新的Jar/脚本**
- **远程执行jenkins_restart_mini.sh判断是否更新并重启，为了不影响之前的教程，这里新加了一个脚本jenkins_restart_mini.sh，和前面几篇文章中提到的jenkins_restart.sh作用是一样的**
   - **判断依赖jar/业务jar是否更新（任意一个更新都需要重启）**
   - **不需要更新的前提下，判断进程是否存在**
   - **重启服务**
<a name="GlLWm"></a>
#### jenkins_jar_and_lib_check.sh脚本
Jenkins本地校验踢出未更新的依赖和业务Jar的脚本；需要在Maven构建完，ssh传输之前使用；<br />完整脚本地址：[https://github.com/vehang/ehang-spring-boot/blob/main/script/jenkins/jenkins_jar_and_lib_check.sh](https://github.com/vehang/ehang-spring-boot/blob/main/script/jenkins/jenkins_jar_and_lib_check.sh)<br />校验步骤：

- **模块下创建一个tmp的临时目录**
- **将业务jar和依赖jar拷贝的tmp临时目录下**
- **分别对业务jar和依赖jar进行MD5校验**
   - **更新的留下**
   - **未更新的删除掉只有Jenkins本地的校验才删除，减少不必要的传输；服务端的检验不要删了，每一个都需要使用的；**

脚本部分细节说明：

- **Jar包MD5校验方法：**`**jar_check_md5()**`**公共方法！直接通过Jar包的MD5校验是否更新**
```shell
# 直接通过jar校验
jar_check_md5() {
  # jar 包的路径
  JAR_FILE=$1
  if [ ! -f $JAR_FILE ]; then
    # 如果校验的jar不存在 返回失败
    return 1
  fi

  JAR_MD5_FILE=${JAR_FILE}.md5
  echo "jenkins校验 JAR的MD5文件："$JAR_MD5_FILE
  if [ -f $JAR_MD5_FILE ]; then
    cat $JAR_MD5_FILE
    md5sum $JAR_FILE
    md5sum --status -c $JAR_MD5_FILE
    RE=$?
    md5sum $JAR_FILE > $JAR_MD5_FILE
    return $RE
  else
    md5sum $JAR_FILE > $JAR_MD5_FILE
  fi

  return 1
}
```

- **Jar 解压校验文件详情**`**jar_unzip_check_md5()**`**公共方法！如果前面直接校验Jar的方式没有成功，就需要再通过解压的方式校验**
```shell
# 将Jar解压之后校验
jar_unzip_check_md5() {
  # jar 包的路径
  UNZIP_JAR_FILE=$1
  if [ ! -f $UNZIP_JAR_FILE ]; then
    # 如果校验的jar不存在 返回失败
    return 1
  fi

  # jar的名称
  UNZIP_JAR_FILE_NAME=`basename -s .jar $UNZIP_JAR_FILE`
  echo "jenkins校验 JAR包名称："$UNZIP_JAR_FILE_NAME
  # jar所在的路径
  UNZIP_JAR_FILE_BASE_PATH=${UNZIP_JAR_FILE%/${UNZIP_JAR_FILE_NAME}*}
  echo "jenkins校验 JAR包路径："$UNZIP_JAR_FILE_BASE_PATH
  # 解压的临时目录
  JAR_FILE_UNZIP_PATH=${UNZIP_JAR_FILE_BASE_PATH}/jar_unzip_tmp
  echo "jenkins校验 解压路径："$JAR_FILE_UNZIP_PATH

  # 用于缓存解压后文件详情的目录
  UNZIP_JAR_FILE_LIST=${UNZIP_JAR_FILE_BASE_PATH}/${UNZIP_JAR_FILE_NAME}.files
  echo "jenkins校验 jar文件详情路径："$UNZIP_JAR_FILE_LIST
  # 缓存解压后文件详情的MD5
  UNZIP_JAR_FILE_LIST_MD5=${UNZIP_JAR_FILE_BASE_PATH}/${UNZIP_JAR_FILE_NAME}.files.md5
  echo "jenkins校验 jar文件详情MD5校验路径："$UNZIP_JAR_FILE_LIST

  rm -rf $JAR_FILE_UNZIP_PATH
  mkdir -p $JAR_FILE_UNZIP_PATH
  # 解压文件到临时目录
  unzip $UNZIP_JAR_FILE -d $JAR_FILE_UNZIP_PATH
  # 遍历解压目录，计算每个文件的MD5值及路径 输出到详情列表文件中
  find $JAR_FILE_UNZIP_PATH -type f -print | xargs md5sum > $UNZIP_JAR_FILE_LIST
  rm -rf $JAR_FILE_UNZIP_PATH

  if [ ! -f $UNZIP_JAR_FILE_LIST_MD5 ]; then
    # 如果校验文件不存在 直接返回校验失败
    md5sum $UNZIP_JAR_FILE_LIST > $UNZIP_JAR_FILE_LIST_MD5
    return 1
  fi

  cat $UNZIP_JAR_FILE_LIST_MD5
  md5sum $UNZIP_JAR_FILE_LIST
  md5sum --status -c $UNZIP_JAR_FILE_LIST_MD5
  RE=$?
  md5sum $UNZIP_JAR_FILE_LIST > $UNZIP_JAR_FILE_LIST_MD5
  # 返回校验结果
  return $RE
}
```

- **汇总判断公共方法！这里汇总了jar_check_md5和jar_unzip_check_md5两个方法调用；注意：由于这是上传前Jenkins调用的本地校验，一旦校验发现没有改变，会执行**`**rm -f $JAR_FILE**`**删除命令；**
```shell
check_md5() {
  # jar 包的路径
  JAR_FILE=$1
  if [ -f $JAR_FILE ]; then
    # 直接通过jar校验
    jar_check_md5 $JAR_FILE
    if [ $? = 0 ];then
      echo "jenkins校验 通过Jar的MD5校验成功"
      rm -f $JAR_FILE
      return 0
    else
      echo "jenkins校验 通过Jar的MD5校验失败"
    fi

    # 通过解压jar 校验是否更新
    jar_unzip_check_md5 $JAR_FILE
    if [ $? = 0 ];then
      echo "jenkins校验 通过解压的MD5校验成功"
      rm -f $JAR_FILE
      return 0
    else
      echo "jenkins校验 通过解压的MD5校验失败"
    fi
  fi

  return 1
}
```

- **判断依赖包**
```shell
# lib目录的路径
MODULE_LIB_PATH=${MODULE_PATH}/target/lib
echo "jenkins校验 lib目录："$MODULE_LIB_PATH
if [ -d $MODULE_LIB_PATH ]; then
  # 将打包后的lib下的依赖全部拷贝到临时的lib文件夹下
  \cp -r ${MODULE_LIB_PATH}/* ${MODULE_TMP_LIB_PATH}
  for LIB_JAR_FILE in ${MODULE_TMP_LIB_PATH}/*.jar
  do
    echo $LIB_JAR_FILE
    if [ -f $LIB_JAR_FILE ];then
      echo "jenkins校验依赖Jar："$LIB_JAR_FILE
      check_md5 $LIB_JAR_FILE
      if [ $? = 0 ];then
        echo "jenkins依赖lib校验！成功，没有发生变化"$LIB_JAR_FILE
      else
        echo "jenkins依赖lib校验！失败，已经更新"$LIB_JAR_FILE
      fi
    fi
  done
fi
```

- **判断业务包**
```shell
MODULE_JAR=${MODULE_TMP_PATH}/${JAR_NAME}.jar
echo "jenkins校验项目Jar："$MODULE_JAR
check_md5 $MODULE_JAR
if [ $? = 0 ];then
   echo "jenkins校验成功，没有发生变化"
else
   echo "jenkins校验失败，已经更新"
fi
```
<a name="GAdyS"></a>
#### SSH传输说明
前面的教程，SSH传输的都是各个项目target目录下的jar，由于这里，本地要做校验，需要缓存历史的MD5值等消息，就创建了临时文件tmp，不再使用target（每次编译都会被清空，无法缓存）；<br />因此这里关于Jenkins的SSH传输配置就需要传输tmp目录了，包括下面脚本中使用的项目路径，也是tmp目录，不再使用target了
<a name="qF7TM"></a>
#### jenkins_restart_mini.sh
服务端检测更新，重启服务的脚本；<br />地址：[https://github.com/vehang/ehang-spring-boot/blob/main/script/jenkins/jenkins_restart_mini.sh](https://github.com/vehang/ehang-spring-boot/blob/main/script/jenkins/jenkins_restart_mini.sh)<br />校验步骤：

- **遍历所有模块的tmp目录**
- **校验依赖Jar、业务Jar是否更新**
- **判断进程是否存在**
   - **已更新 / 进程不存在；重启**
   - **未更新跳过**

构建测试：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477261144-8326e67a-858f-4d2c-a52c-ceaa1cb57c5b.png#clientId=u0168a634-410c-4&from=paste&id=u1c3cf75e&originHeight=182&originWidth=743&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf1379265-59d3-42b3-8727-e7f7b284504&title=)
<a name="q3C5Z"></a>
### Docker的方式
Docker方式和SSH的方式有比较大的差异，采用SSH的方式，一般是明确知道那些服务器，然后直接上传；但采用Docker，最终服务在那些机器上运行，就不一定了，比如使用了K8S；<br />那就意味着，服务所需的包、依赖，都必须打到Docker镜像中，以方便容器启动时使用；但这似乎又违背了本文的意图，哪怕是只有业务更新，也需要把所有的依赖添加到镜像中去；<br />既然业务包也依赖包能拆分，业务（app）镜像和依赖（lib）镜像分开也就能解决这个问题了，如下图：

- **基础依赖镜像避免干扰，每个模块都有自己独立的依赖镜像；只有在依赖变更的情况下构建更新；是否需要推送的镜像仓库**
- **业务镜像模块依赖更新/业务更新的时候，重新构建更新**

示例项目：[https://github.com/vehang/ehang-spring-boot/tree/main/spring-boot-012-tools-jenkins-mini-build](https://github.com/vehang/ehang-spring-boot/tree/main/spring-boot-012-tools-jenkins-mini-build)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477295273-0721bf56-f6d0-43a4-a0b4-d2d7d9cd9821.png#clientId=u0168a634-410c-4&from=paste&id=u56370ffa&originHeight=249&originWidth=414&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1501ffb0-dc12-49a4-8622-717f361ed46&title=)<br /> <br />目录说明：

- **app保存业务包以及构建镜像用的Dockerfile**
- **lib保存业务所需的依赖包以及构建依赖镜像的Dockerfile**
- **docker-image-build.shJenkins 构建依赖镜像、业务镜像的脚本**
- **docker-image-pull.sh服务器获取最新镜像的脚本**
- **docker-compose.yaml启动容器的基础配置文件**
<a name="OVj4K"></a>
#### Jenkins构建Docker镜像并启动的过程

- **拉取最新代码**
- **Maven打包**
- **执行script/jenkins目录下的jenkins_docker_build.sh作用是遍历所有模块中的docker-image-build.sh并逐一执行**
- **执行模块下的docker-image-build.sh**
   - **校验依赖jar是否更新a. 更新构建依赖基础镜像b. 未更新跳过**
   - **检验业务并构建业务镜像**
   - **将镜像推送到镜像仓库**
- **将script/jenkins脚本以及模块下docker目录的脚本上传到服务器**
- **执行jenkins_restart_docker.sh**
   - **执行docker-image-pull.sh下载最新的镜像**
   - **执行docker-compose.yaml启动服务**
<a name="e03mr"></a>
#### 依赖镜像的Dockerfile
目录在/docker/lib/Dockerfile
```dockerfile
FROM openjdk:8

# 同步时区
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 将lib目录下的所有jar全部拷贝到镜像里面
ADD ./*.jar /lib/
```
就是将所有的目录包拷贝到镜像中去；<br />构建镜像：
```bash
docker build -t lib-jenjins-mini-build:latest ./docker/lib/.
```
最终会构建出一个名为lib-jenjins-mini-build:latest的镜像
<a name="yAqxj"></a>
#### 业务镜像的Dockerfile
目录在/docker/app/Dockerfile
```dockerfile
# 集成自依赖基础镜像
FROM lib-jenjins-mini-build:latest

# 将当前目录下的jar拷贝到容器类
ADD ./*.jar /app.jar

# 监听端口
EXPOSE 18092

# 启动
ENTRYPOINT ["java","-Dloader.path=/lib","-Djava.security.egd=file:/dev/./urandom" \
,"-XX:+UnlockExperimentalVMOptions","-XX:+UseCGroupMemoryLimitForHeap" \
,"-jar", "/app.jar" ]
```

- `**FROM**`**指明基础镜像为前面构建的依赖镜像**
- `**ADD**`**将业务jar包拷贝到容器中去**
- `**EXPOSE**`**设置监听端口**
- `**ENTRYPOINT**`**启动服务，这里务必要指定**`**-Dloader.path=/lib**`**，其中**`**/lib**`**就是前面基础镜像依赖保存的地址，如果有调整，这里也需要跟着调整**
<a name="WCSbL"></a>
#### docker-image-build.sh
用于构建基础镜像和业务镜像的脚本；<br />脚本地址：[https://github.com/vehang/ehang-spring-boot/blob/main/spring-boot-012-tools-jenkins-mini-build/docker/docker-image-build.sh](https://github.com/vehang/ehang-spring-boot/blob/main/spring-boot-012-tools-jenkins-mini-build/docker/docker-image-build.sh)

- **公共方法以下的三个校验方法属于公共方法，和前面介绍的一样jar_check_md5 通过jar的md5值直接检测jar_unzip_check_md5 通过对jar包解压 校验文件详情的MD5check_md5 汇总上面两个方法的校验**
- **准备jar将业务jar包拷贝到docker/app目录将依赖Jar拷贝到docker/lib目录**
```shell
MODULE_LIB_PATH=${MODULE_BATH_PATH}/docker/lib
MODULE_APP_PATH=${MODULE_BATH_PATH}/docker/app

\cp -r ${MODULE_BATH_PATH}/target/*.jar ${MODULE_APP_PATH}
\cp -r ${MODULE_BATH_PATH}/target/lib/*.jar ${MODULE_LIB_PATH}
```

- **校验并构建依赖镜像检验依赖包是否更新，如果更新，构建基础的依赖镜像；**
```shell
LIB_UPDATE=false
for LIB_JAR_FILE in ${MODULE_LIB_PATH}/*.jar
do
  echo $LIB_JAR_FILE
  if [ -f $LIB_JAR_FILE ];then
    echo "Jenkins Docker镜像构建校验lib 依赖Jar："$LIB_JAR_FILE
    check_md5 $LIB_JAR_FILE
    if [ $? = 0 ];then
      echo "Jenkins Docker镜像构建校验lib！成功，没有发生变化"$LIB_JAR_FILE
    else
      LIB_UPDATE=true
      echo "Jenkins Docker镜像构建校验lib！失败，已经更新"$LIB_JAR_FILE
    fi
  fi
done
# 一旦发现lib有变化，就构建新的lib镜像
if [ $LIB_UPDATE = true ]; then
  docker build -t ${MODULE_DOCKER_LIB_IMAGE_NAME}:latest ${MODULE_LIB_PATH}/.
fi
```

- **校验业务jar并构建镜像校验业务包、依赖包是否更新**`**if [ $APP_UPDATE = true ] || [ $LIB_UPDATE = true ]**`**；更新就重新构建镜像；**
```shell
APP_UPDATE=false
for APP_JAR_FILE in ${MODULE_APP_PATH}/*.jar
do
  echo $APP_JAR_FILE
  if [ -f $APP_JAR_FILE ];then
    echo "Jenkins Docker镜像构建校验APP 依赖Jar："$APP_JAR_FILE
    check_md5 $APP_JAR_FILE
    if [ $? = 0 ];then
      echo "Jenkins Docker镜像构建校验APP！成功，没有发生变化"$APP_JAR_FILE
    else
      APP_UPDATE=true
      echo "Jenkins Docker镜像构建校验APP！失败，已经更新"$APP_JAR_FILE
    fi
  fi
done
# 一旦发现lib有变化，或者APP发生变化 都需要构建新的镜像
if [ $APP_UPDATE = true ] || [ $LIB_UPDATE = true ]; then
  # 构建镜像
  docker build -t registry.cn-guangzhou.aliyuncs.com/ehang_jenkins/${MODULE_DOCKER_IMAGE_NAME}:latest ${MODULE_APP_PATH}/.
  # 将镜像推送到阿里云
  docker push registry.cn-guangzhou.aliyuncs.com/ehang_jenkins/${MODULE_DOCKER_IMAGE_NAME}:latest
fi
```
构建测试：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477315015-5bc15818-2918-4e9f-acf3-335d7ea53238.png#clientId=u0168a634-410c-4&from=paste&id=u98bff57d&originHeight=437&originWidth=866&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dab1744-054f-4fee-96cc-373853415e6&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662477315101-5362a0a2-06ee-448d-b658-c7c2f75233a7.png#clientId=u0168a634-410c-4&from=paste&id=ubb770ac5&originHeight=180&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2af10e4c-fcf1-4c63-8a9c-4438fb1f3ae&title=)<br />再回看一下部署过程，发现曾经最耗时的部分，现在一下变得丝滑了好多...
