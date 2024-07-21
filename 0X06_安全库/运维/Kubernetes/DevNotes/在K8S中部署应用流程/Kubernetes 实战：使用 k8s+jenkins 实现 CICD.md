<a name="Gsstu"></a>
## 1、CICD
CICD（Continuous Integration，Continuous Delivery & Deployment），持续集成，持续部署，持续发布。<br />也就是说，在本地开发完代码之后，push到远程仓库，然后代码打包、部署的这个过程完全是自动化完成的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194247-9397b885-9e1b-468d-b544-61d799a0d799.png#averageHue=%23fafafa&clientId=u37757ad8-1d8d-4&from=paste&id=ud5d735c0&originHeight=511&originWidth=1069&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue599eb04-ac5b-4be4-a5c6-fc80434d57b&title=)<br />但是不要进入一个误区，CICD并不意味着一定就是这一套流程，只要实现了代码提交后自动部署即可，不管是部署在k8s还是Tomcat中，这是由应用规模和使用的工具来决定的。
<a name="nIm4D"></a>
## 2、准备k8s环境
<a name="a1vi1"></a>
## 3、Jenkins环境准备（选择一台服务器）
<a name="lGh3Z"></a>
### 1、安装Java（最新版Jenkins只支持jdk11以上）
<a name="c0cZF"></a>
#### 1）找到jdk资源上传到指定机器
```bash
# 解压
tar -zxvf jdk-17_linux-x64_bin.tar.gz
```
<a name="tK3Gc"></a>
#### 2）配置环境变量
```bash
vi /etc/profile
# 在最后添加上
export JAVA_HOME=/root/jenkins/jdk-17.0.7
export CLASSPATH=.:${JAVA_HOME}/jre/lib/rt.jar:${JAVA_HOME}/lib/dt.jar:${JAVA_HOME}/lib/tools.jar
export PATH=$PATH:${JAVA_HOME}/bin
# 保存后执行
source /etc/profile 
# 确保安装完成
java -version
```
<a name="awIYm"></a>
### 2、安装Maven
<a name="mVIRe"></a>
#### 1）找到Maven资源上传到指定机器
```bash
# 解压
tar -zxvf apache-maven-3.6.2-bin.tar.gz
```
<a name="GQxO8"></a>
#### 2）配置环境变量
```bash
vi /etc/profile
# 在最后添加
export MAVEN_HOME=/root/jenkins/apache-maven-3.6.2
export PATH=$PATH:$JAVA_HOME/bin:$MAVEN_HOME/bin
# 保存后执行
source /etc/profile 
# 确保安装完成
mvn -version
```
<a name="vlVk1"></a>
#### 3）配置Maven的阿里云镜像
apache-maven-3.6.2/conf/settings.xml中，修改镜像
```xml
<mirror>
  <id>alimaven</id>
  <name>aliyun maven</name>
  <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
  <mirrorOf>central</mirrorOf>        
</mirror>
```
<a name="uMnk2"></a>
### 3、安装和配置git（远程仓库使用gitee）
<a name="FWvz5"></a>
#### 1）安装git
```bash
# 下载安装
yum install git 
# 配置git
git config --global user.name "cxf"
git config --global user.email "cxf@qq.com"
```
<a name="LgfJ2"></a>
#### 2）在服务器上生成公钥
```bash
# -t key 类型 -C 注释 中间三次enter即可
ssh-keygen -t ed25519 -C "Gitee SSH Key"
# 查看公钥 私钥文件 id_ed25519 公钥文件 id_ed25519.pub
ls ~/.ssh/
```
复制生成后的 ssh key，通过仓库主页 「管理」->「部署公钥管理」->「添加部署公钥」，将生成的公钥添加到仓库中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194288-3c433622-720f-442f-9b4f-4677eb45c284.png#averageHue=%23fbf7ee&clientId=u37757ad8-1d8d-4&from=paste&id=uab031785&originHeight=286&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uafc5a647-3c54-43b3-93cf-f6d75d775e4&title=)
<a name="cTaQs"></a>
#### 3）验证
```bash
# 通过 ssh -T 测试时，输出 Anonymous：添加成功后，就可以使用 SSH 协议对仓库进行拉取。
$ ssh -T git@gitee.com
Hi Anonymous! You've successfully authenticated, but GITEE.COM does not provide shell access.
```
<a name="yXUn1"></a>
### 4、准备SpringBoot项目
<a name="Bu3qU"></a>
#### 1）编写SpringBoot项目
```java
@RestController
public class K8SController {
    @RequestMapping("/k8s")
    public String k8s(){
        return "hello K8s <br/>111222 ";
    }
}
```
<a name="WA5rm"></a>
#### 2）将SpringBoot项目提交到gitee
<a name="zTfLF"></a>
#### 3）git clone测试
```bash
git clone git@gitee.com:xiangno1/springboot-demo.git
```
拉下来代码，说明服务器配置没问题。
<a name="xb3zp"></a>
### 5、安装Jenkins
**Jenkins官网：**[https://jenkins.io/](https://jenkins.io/)<br />**入门指南：**[https://www.jenkins.io/zh/doc/pipeline/tour/getting-started/](https://www.jenkins.io/zh/doc/pipeline/tour/getting-started/)<br />必须在k8s集群中，因为后面需要在Jenkins的目录下创建文件执行，比如这里选用m1节点。
<a name="enpJ0"></a>
#### 1）下载Jenkins
```bash
wget http://mirrors.jenkins.io/war-stable/latest/jenkins.war
```
<a name="aIZ3C"></a>
#### 2）启动Jenkins（需要Java环境）
```bash
# 启动报错，需要先安装字体
# java.lang.NullPointerException: Cannot load from short array because "sun.awt.FontConfiguration.head" is null
yum install fontconfig
fc-cache --force

# 后台启动
nohup java -jar jenkins.war --httpPort=8080 & 
tail -f nohup.out 

# 启动日志会提示密码
#4c7341496d044ef7849576ccddc20d11
#This may also be found at: /root/.jenkins/secrets/initialAdminPassword
```
<a name="MEn0w"></a>
#### 3）Windows访问192.168.56.101:8080
Jenkins启动时会打印一串密码，需要填上<br />点击【安装推荐的插件】，需要等一段时间自动完成。<br />下载完插件之后，需要创建第一个 管理员用户 设置为admin/admin。
<a name="Uzhdd"></a>
#### 4）配置Jenkins的Git、Maven、Java
在全局配置，新版的在tools中，配置Maven。默认是使用系统环境变量中的。
<a name="mRvPT"></a>
#### 5）新建任务测试
新建一个任务（item），输入名称（springboot-demo），选择流水线<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194312-6c78e9b9-56b0-4e6f-8f0a-c6a00cc93fd8.png#averageHue=%23d1bea4&clientId=u37757ad8-1d8d-4&from=paste&id=udd00c251&originHeight=705&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub7a40ad4-a135-4452-bda0-b121896901c&title=)<br />配置springboot-demo的task，修改pipeline内容 ，增加Maven构建，然后“保存和立即构建“，同时可以查看“Console Output”<br />因为之前安装git的时候已经配置了公钥，所以拉取的时候并不需要用户密码了！
```
node {
   def mvnHome
   stage('Preparation') {
      git 'git@gitee.com:xiangno1/springboot-demo.git'
   }
   
   stage('Maven Build') { 
      sh "mvn clean package"
   }
}
```
编写完之后保存，然后点击【立即构建】。<br />在/root/.jenkins/workspace 中，会下载springboot-demo，并使用Maven打成一个jar包。
<a name="v2Ku2"></a>
#### 6）设置push之后自动构建
当用户进行git commit/push提交代码到Gitee时，能够通知Jenkins自动构建
:::danger
注意：Jenkins的ip一定要是Gitee能够访问到的地址
:::
在Gitee上配置Jenkins的webhook地址：http://192.168.56.101:8080/springboot-demo<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194296-090a8559-9cee-4e62-8c8a-9e978079425f.png#averageHue=%23fdfcf7&clientId=u37757ad8-1d8d-4&from=paste&id=u4fe9e776&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub76a9d5f-248f-45cf-bed3-3a23313ca81&title=)<br />**生成Personal access tokens**<br />Jenkins访问Gitee需要授权，所以在Gitee上生成token交给Jenkins使用，即Personal access tokens<br />Gitee的Settings[个人信息右上角]–>Developer settings–>Personal access tokens–>Generate new token<br />最后保存好该token，比如：72f048b514e95d6fe36f86d84374f2dcce402b43<br />**Jenkins安装插件**

- 安装github plugin插件：[系统管理]->[插件管理]->[可选插件]
- 安装gitlab插件和gitlab hook插件：[系统管理]->[插件管理]->[可选插件]

**配置GitHub Server**<br />[系统管理]->[系统配置]->[找到github服务器]->[添加github服务器]<br />在流水线触发器勾选GitHub hook trigger for GITScm polling
<a name="IiAJa"></a>
### 6、DockerHub准备（阿里云）
<a name="M1KMq"></a>
#### 1）创建一个springboot-demo 的镜像仓库
```bash
# 登录 ，需要输入密码
docker login --username=沙里沟第二小学扛把子 registry.cn-hangzhou.aliyuncs.com
# 私有云的话，需要在k8s添加凭证
kubectl create secret docker-registry <secret-name> \
     --docker-username=<username> \
     --docker-password=<password> \
     --docker-email=<email> \
     --docker-server=<registry>
<secret-name> 是您为 secret 指定的名称，例如 my-registry-secret。
<username> 是您在镜像仓库的用户名。
<password> 是您在镜像仓库的密码。
<email> 是与您在镜像仓库的账号相关联的电子邮件。
<registry> 是您的镜像仓库地址。
接下来，您需要在使用该凭证的部署（deployment）或其他相关资源中引用该 secret。可以在相关资源的定义文件中添加以下部分
spec:
     template:
       spec:
         imagePullSecrets:
         - name: <secret-name>
```
<a name="N4LLk"></a>
#### 2）准备shell脚本实现docker push
```bash
# mkdir /root/.jenkins/workspace/scripts/
# vi /root/.jenkins/workspace/scripts/springboot-demo-build-image.sh
# 记得chmod +x /root/.jenkins/workspace/scripts/springboot-demo-build-image.sh
# 脚本开始
# 进入到springboot-demo目录
cd ../springboot-demo

# 编写Dockerfile文件

cat <<EOF > Dockerfile
FROM openjdk:8-jre-alpine
COPY target/springboot-demo-0.0.1-SNAPSHOT.jar /springboot-demo.jar
ENTRYPOINT ["java","-jar","/springboot-demo.jar"]
EOF

echo "Dockerfile created successfully!"

# 基于指定目录下的Dockerfile构建镜像
docker build -t registry.cn-hangzhou.aliyuncs.com/cxf01/springboot-demo:v1.0 .

# push镜像，这边需要阿里云镜像仓库登录，在w1上登录
docker push registry.cn-hangzhou.aliyuncs.com/cxf01/springboot-demo:v1.0
```
<a name="A4kFm"></a>
#### 3）Jenkins的pipeline增加步骤
```
node {
   def mvnHome
   stage('Preparation') {
      git 'git@gitee.com:xiangno1/springboot-demo.git'
   }
   
   stage('Maven Build') { 
      sh "mvn clean package"
   }
   
   stage('Build Image') { 
      sh "/root/.jenkins/workspace/scripts/springboot-demo-build-image.sh"
   }
}
```
<a name="GVZlI"></a>
#### 4）再次执行Jenkins的构建
构建成功！并且上传阿里云Docker镜像仓库成功！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194321-e802ab1c-d363-45d7-bdfc-7411788e25b4.png#averageHue=%23f2f1f1&clientId=u37757ad8-1d8d-4&from=paste&id=ucd7296ed&originHeight=854&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u51a50dc1-1198-42c3-b65b-7f0a8e4be70&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1697172194716-34b11416-91e3-4cba-b795-fe28dc6b7e8e.png#averageHue=%23fcfcfc&clientId=u37757ad8-1d8d-4&from=paste&id=u542b2e23&originHeight=138&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5cdc9ba-906f-46f2-b223-402b73dbff7&title=)
<a name="zl4Ne"></a>
## 4、Kubernetes拉取镜像运行
<a name="gSfur"></a>
### 1、准备springboot-demo.yaml文件
在/root/.jenkins/workspace/scripts/目录下创建springboot-demo.yaml
```yaml
# 以Deployment部署Pod
apiVersion: apps/v1
kind: Deployment
metadata: 
  name: springboot-demo
spec: 
  selector: 
    matchLabels: 
      app: springboot-demo
  replicas: 1
  template: 
    metadata:
      labels: 
        app: springboot-demo
    spec: 
      imagePullSecrets: # 私有云凭证
      - name: springboot-demo
      containers: 
      - name: springboot-demo
        image: registry.cn-hangzhou.aliyuncs.com/cxf01/springboot-demo:v1.0
        ports: 
        - containerPort: 8080
---
# 创建Pod的Service
apiVersion: v1
kind: Service
metadata: 
  name: springboot-demo
spec: 
  ports: 
  - port: 80
    protocol: TCP
    targetPort: 8080
  selector: 
    app: springboot-demo
---
# 创建Ingress，定义访问规则
apiVersion: extensions/v1beta1
kind: Ingress
metadata: 
  name: springboot-demo
spec: 
  rules: 
  - host: springboot.cxf.com
    http: 
      paths: 
      - path: /
        backend: 
          serviceName: springboot-demo
          servicePort: 80
```
<a name="L1T2K"></a>
### 2、准备shell文件，用于k8s自动拉取镜像部署
编写k8s-deploy-springboot-demo.sh文件
```bash
# vi /root/.jenkins/workspace/scripts/k8s-deploy-springboot-demo.sh
# chmod +x /root/.jenkins/workspace/scripts/k8s-deploy-springboot-demo.sh
kubectl delete -f springboot-demo.yaml
kubectl apply -f /root/.jenkins/workspace/scripts/springboot-demo.yaml
echo "k8s deploy success!"
```
<a name="OhIs8"></a>
### 3、修改Jenkins的pipeline
```
node {
   def mvnHome
   stage('Preparation') {
      git 'git@gitee.com:xiangno1/springboot-demo.git'
   }
   
   stage('Maven Build') { 
      sh "mvn clean package"
   }
   
   stage('Build Image') { 
      sh "/root/.jenkins/workspace/scripts/springboot-demo-build-image.sh"
   }

   stage('K8S Deploy') { 
      sh "/root/.jenkins/workspace/scripts/k8s-deploy-springboot-demo.sh"
   }
}
```
<a name="h46gx"></a>
### 4、w1执行不了kubectl 命令
```
切换到master上，cd ~  --->   cat ~/.kube/config  --->复制内容
切换到worker02上   cd ~  ---> mkdir ~/.kube   vi ~/.kube/config   --->粘贴内容
```
