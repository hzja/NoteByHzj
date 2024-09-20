Window Gradle
<a name="GH0Fa"></a>
## 1、Gradle官网
[Gradle官网](https://gradle.org/)<br />[https://gradle.org/](https://gradle.org/)
<a name="OExIr"></a>
## 2、Gradle下载
<a name="VfE5U"></a>
### 从官网下载
[Gradle各版本发布下载地址](https://gradle.org/releases/)<br />[https://gradle.org/releases/](https://gradle.org/releases/)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593217917863-994bfe02-0e1c-45b5-864b-77ab03a80c6c.png#averageHue=%23fefefe&height=561&id=V3fxj&originHeight=1683&originWidth=1726&originalType=binary&ratio=1&rotation=0&showTitle=false&size=276161&status=done&style=shadow&title=&width=575.3333333333334)
<a name="oRpoG"></a>
### Gradle安装包
<a name="Ku7AQ"></a>
#### V 6.5
[gradle-6.5-all.zip](https://www.yuque.com/attachments/yuque/0/2020/zip/396745/1595936522570-b839155c-c97c-4ec9-a622-6936a4585a2e.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fzip%2F396745%2F1595936522570-b839155c-c97c-4ec9-a622-6936a4585a2e.zip%22%2C%22name%22%3A%22gradle-6.5-all.zip%22%2C%22size%22%3A145767155%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22mode%22%3A%22title%22%2C%22uid%22%3A%221593217994880-0%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22id%22%3A%22bWVmt%22%2C%22card%22%3A%22file%22%7D)
<a name="g5aPe"></a>
## 3、Window环境变量的配置
<a name="X32yh"></a>
### 检查Java的版本，Gradle需要的JAVA版本要高于Java8以上
打开cmd命令窗口输入Java版本查询命令
```bash
$ java -version
java version "1.8.0_121"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593218206386-8d93c0bb-0329-4770-a964-9016ac62aa33.png#averageHue=%23353535&height=206&id=SpuLQ&originHeight=618&originWidth=1527&originalType=binary&ratio=1&rotation=0&showTitle=false&size=90589&status=done&style=none&title=&width=509)
<a name="k0jLa"></a>
### 解压Gradle压缩包，并进入解压目录下一级bin目录，复制路径
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593218760471-fb288277-075d-4a6a-acf0-2eabb8c033d7.png#averageHue=%23e7c687&height=513&id=Z2N7X&originHeight=1540&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=296403&status=done&style=shadow&title=&width=731)
<a name="CCkml"></a>
### 配置环境变量
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593218698932-dec09c70-0212-4efb-9509-c01318313029.png#averageHue=%238b8249&height=687&id=ECMCu&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3533645&status=done&style=none&title=&width=1280)
<a name="rcdaZ"></a>
### 检查安装完成
关闭刚才的cmd命令窗口，重新打开以便于读取最新的配置，然后输入命令
```bash
$ gradle -v
```
成功输出以下信息即表示安装完成<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593218858134-94b21f7a-641d-4868-adf5-700dd82c309f.png#averageHue=%23202020&height=513&id=d6HW4&originHeight=1540&originWidth=1733&originalType=binary&ratio=1&rotation=0&showTitle=false&size=180912&status=done&style=none&title=&width=577.6666666666666)
<a name="wTr6K"></a>
## 4、Gradle本地仓库配置
Gradle并没有提供像Maven那样的在配置文件配置本地仓库目录，需要在环境变量进行配置
<a name="xVdLj"></a>
### Window下配置Gradle的本地仓库变量
<a name="obC6g"></a>
#### 通过`gradle -g`命令配置
`gradle -g D:\LinkSpace\Download\DevelopPackage\Gradle\Repo`（指定的本地仓库的目录）
```bash
PS C:\Users\fcsca> gradle -g D:\LinkSpace\Download\DevelopPackage\Gradle\Repo
Starting a Gradle Daemon (subsequent builds will be faster)

> Task :help

Welcome to Gradle 6.5.

To run a build, run gradle <task> ...

To see a list of available tasks, run gradle tasks

To see a list of command-line options, run gradle --help

To see more detail about a task, run gradle help --task <task>

For troubleshooting, visit https://help.gradle.org

Deprecated Gradle features were used in this build, making it incompatible with Gradle 7.0.
Use '--warning-mode all' to show the individual deprecation warnings.
See https://docs.gradle.org/6.5/userguide/command_line_interface.html#sec:command_line_warnings

BUILD SUCCESSFUL in 5s
1 actionable task: 1 executed
PS C:\Users\fcsca>
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593704654700-fa49e64b-c4b7-4bd7-b3f9-00e8fa15046e.png#averageHue=%23212121&height=440&id=whzM6&originHeight=1321&originWidth=2320&originalType=binary&ratio=1&rotation=0&showTitle=false&size=198219&status=done&style=none&title=&width=773.3333333333334)
<a name="EmrEj"></a>
#### 通过环境变量配置
打开环境变量新建变量名为GRADLE_USER_HOME，值为本地仓库文件夹的目录路径（**注意不能指向和Maven相同的仓库，他们并不兼容**）<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593702692918-ed5eb78b-b2ff-456c-89cd-42df5a5eeeb8.png#averageHue=%23f6f6f6&height=502&id=VvCXT&originHeight=1507&originWidth=2314&originalType=binary&ratio=1&rotation=0&showTitle=false&size=311848&status=done&style=shadow&title=&width=771.3333333333334)![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593702923164-38ae064c-5a12-4b69-b2fb-44e486bd50a1.png#averageHue=%23f3f2f2&height=535&id=g3q9A&originHeight=1606&originWidth=3141&originalType=binary&ratio=1&rotation=0&showTitle=false&size=664239&status=done&style=none&title=&width=1047)
<a name="naStS"></a>
### Linux在/etc/profile文件进行配置环境变量
```bash
export GRADLE_HOME=/usr/local/gradle
export PATH=$GRADLE_HOME/bin:$PATH
export GRADLE_USER_HOME=$GRADLE_HOME/.gradle
```
<a name="67Emr"></a>
## 5、配置Gradle的全局初始化配置
正如Maven的个人setting.xml文件和全局setting.xml文件一样，Gradle也有定义自己的初始化全局配置文件的操作。
<a name="Bb0O1"></a>
### Gradle的配置文件的启动
Gradle的全局配置文件可以配置在以下四个位置：<br />**1、在命令行指定文件，例如：**`**gradle –init-script yourdir/init.gradle -q taskName**`**。可以多次输入此命令来指定多个init文件 。**<br />**2、把init.gradle文件放到USER_HOME/.gradle/ 目录下。**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593701318020-9581e9fa-9d97-4c67-8b2f-a95369845947.png#averageHue=%23e2c182&height=503&id=PbNTX&originHeight=1510&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=419658&status=done&style=none&title=&width=731)<br />**3、把以.gradle结尾的文件放到USER_HOME/.gradle/init.d/ 目录下。**<br />**4、把以.gradle结尾的文件放到GRADLE_HOME/init.d/ 目录下。**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593274513805-47d8654f-fe40-4335-b2a9-bd9730c7c839.png#averageHue=%23e9c884&height=467&id=iKPzw&originHeight=1401&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=444462&status=done&style=none&title=&width=731)<br />**如果存在上面的4种方式的2种以上，gradle会按上面的1-4序号依次执行这些文件，如果给定目录下存在多个init脚本，会按拼音a-z顺序执行这些脚本 。以上是Gradle初始化脚本（init.gradle）启动的配置，Gradle启动后会自动按照顺序读取。**
<a name="0siRj"></a>
### 配置Gradle全局初始化文件init.gradle
在习惯的目录下新建一个init.gradle初始化脚本，将下面的配置内容写入init.gradle文件
<a name="tqBY0"></a>
### 私服仓库的配置信息
```json
allprojects {
   repositories {
       maven {
           url "https://maven.aliyun.com/repository/public"
       }
       maven {
           url "https://maven.aliyun.com/repository/jcenter"
       }
       maven {
           url "https://maven.aliyun.com/repository/spring"
       }
       maven {
           url "https://maven.aliyun.com/repository/spring-plugin"
       }
       maven {
           url "https://maven.aliyun.com/repository/gradle-plugin"
       }
       maven {
           url "https://maven.aliyun.com/repository/google"
       }
       maven {
           url "https://maven.aliyun.com/repository/grails-core"
       }
       maven {
           url "https://maven.aliyun.com/repository/apache-snapshots"
       }
   }
}
```
<a name="XShUY"></a>
### Gradle全局代理设置
在配置目录下新建gradle.properties文件，添加如下配置
```json
org.gradle.jvmargs=-Xmx4g -XX:MaxPermSize=512m -XX:+HeapDumpOnOutOfMemoryError -Dfile.encoding=UTF-8
systemProp.http.proxyHost=127.0.0.1
systemProp.http.proxyPort=10800
systemProp.https.proxyHost=127.0.0.1
systemProp.https.proxyPort=10800
systemProp.file.encoding=UTF-8
org.gradle.warning.mode=all
```
<a name="fhNrM"></a>
## 6、在IDEA中的配置Gradle
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593220043438-2a5e428d-c2a7-4d28-9bd4-3a03ebc8a8fb.png#averageHue=%23f1f1f0&height=687&id=okYtA&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=869187&status=done&style=shadow&title=&width=1280)
