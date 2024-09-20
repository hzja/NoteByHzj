Maven
<a name="Y8svl"></a>
## Maven项目目录结构
```
--P（父工程pom）
	-- A （子工程 - common）
	-- B （子工程 - biz 依赖common）
	-- C （子工程 - Web 依赖B、间接依赖A）
```
<a name="WgSou"></a>
## 一、Maven 命令参数
`-D` 传入属性参数 <br />`-P` 使用pom中指定的配置 <br />`-e` 显示maven运行出错的信息 <br />`-o` 离线执行命令，即不去远程仓库更新包 <br />`-X` 显示maven允许的debug信息 <br />`-U` 强制去远程参考更新snapshot包 <br />例如 `mvn install -Dmaven.test.skip=true -Poracle` <br />其他参数可以通过`mvn help` 获取
<a name="wPoEd"></a>
## 二、Maven常用命令
<a name="ZD1yc"></a>
### 1、`mvn clean` (清理项目生产的临时文件，一般是模块下的target目录)
清理项目生产的临时文件，一般是模块下的target目录
<a name="I4a6r"></a>
### 2、`mvn package` (项目打包工具，会在模块下的target目录生成jar或war等文件)
项目打包工具，会在模块下的target目录生成jar或war等文件
<a name="EcoIA"></a>
### 3、`mvn test` (测试命令，或执行src/test/java/下junit的测试用例)
测试命令，或执行src/test/java/下junit的测试用例
<a name="LpphX"></a>
### 4、`mvn install` （安装到本地Maven仓库）
模块安装命令 将打包的的jar/war文件复制到本地仓库中，供其他模块使用 `-Dmaven.test.skip=true` 跳过测试(同时会跳过test compile)
<a name="pIMes"></a>
### 5、`mvn install -X` (追踪依赖的完整轨迹)
追踪依赖的完整轨迹
<a name="WH4jk"></a>
### 6、`mvn deploy` (发布命令 将打包的文件发布到远程仓库)
发布命令 将打包的文件发布到远程仓库
<a name="ae0er"></a>
### 7、`mvn dependency:analyze` (分析依赖关系，用来分析出无用，重复依赖)
分析依赖关系，用来分析出无用，重复依赖
<a name="XHr77"></a>
### 8、`mvn dependency:tree` (打印出项目的整个依赖树)
打印出项目的整个依赖树 
<a name="AKf3P"></a>
### 9、`mvn dependency:resolve` (查看依赖)
查看依赖
<a name="ts767"></a>
### 10、`mvn project-info-reports:dependencies` (生成项目依赖的报表)
生成项目依赖的报表
<a name="O5daV"></a>
## 三、其他命令
<a name="V6W8R"></a>
### 下载源码
```bash
mvn dependency:sources
```
<a name="FQWUZ"></a>
### 下载源码及文档
```bash
mvn dependency:sources -DdownloadSources=true -DdownloadJavadocs=true
```
<a name="EGyle"></a>
### 安装本地jar
```bash
mvn install:install-file -Dfile=you.jar -DgroupId=org.csource.fastdfs -DartifactId=fastdfs -Dversion=1.25 -Dpackaging=jar
```
`-Dfile`：本地jar包位置（未安装至本地仓库前的jar包文件位置路径）<br />`-DgroupId`：项目名 对应 `<groupId>com.jacob</groupId>`<br />`-DartifactId`：文件名 对应 `<artifactId>jacob</artifactId>`<br />`-Dversion`：版本号 对应 `<version>1.18</version>`
:::danger
注意：执行该命令时需要将Maven的bin目录配置在环境变量。<br />在PowerShell执行会报错；<br />在CMD命令窗口执行时使用jar包文件的绝对路径，在任何位置执行都可以，不会有报错。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615529470451-87dad6fb-e39c-4c86-b1bf-4b3bbfc3650b.png#averageHue=%23474747&height=379&id=lfNC8&originHeight=1137&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1367369&status=done&style=none&title=&width=1107.6666666666667)![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615529521302-89e54e80-79c7-41bf-85bd-a5b591f8ab73.png#averageHue=%230c0c0c&height=473&id=aeqKQ&originHeight=1419&originWidth=2760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=307763&status=done&style=none&title=&width=920)
:::
<a name="sN4Yi"></a>
### 安装到仓库，跳过测试，跳过文档
```bash
mvn install -Dmaven.test.skip=true -Dmaven.javadoc.skip=true
```
<a name="GYXzw"></a>
### 把依赖的jar copy到lib目录
```bash
mvn dependency:copy-dependencies -DoutputDirectory=lib
```
<a name="GSLUW"></a>
### 打包时跳过测试
```bash
mvn package -DskipTests
```
<a name="BqdlJ"></a>
### SpringBoot项目打包命令
```bash
mvn clean package -Dmaven.test.skip=true
```
<a name="Xwoal"></a>
## 四、Maven命令分类整理
<a name="JZhqA"></a>
### 构建打包类命令
<a name="Px8Uv"></a>
#### 编译源代码
```bash
mvn compile
```
<a name="GVhXn"></a>
#### 编译测试代码
```bash
mvn test-compile
```
<a name="Wbuw3"></a>
#### 编译测试代码
```bash
mvn test
```
<a name="tiKrp"></a>
#### 打包项目
```bash
mvn package
```
<a name="jqMBu"></a>
#### 清除打包的项目
```bash
mvn clean
```
package、install、deploy等命令常与该命令配合使用。
<a name="Pafwk"></a>
#### 清除历史打包并重新打包
```bash
mvn clean package
```
最常用的命令之一。
<a name="GZmXF"></a>
#### 清除并`install`到本地
```bash
mvn clean install
```
<a name="paq8h"></a>
#### 查看当前项目已被解析的依赖
```bash
mvn dependency:list
```
常用于Jar包依赖冲突排查等。
<a name="D1xkR"></a>
#### 上传到私服
```bash
mvn deploy
```
<a name="kfZ4L"></a>
### 复杂组合命令
<a name="ZOoTf"></a>
#### 强制检查更新后打包
```bash
mvn clean package -U
```
`-U`参数，强制检查更新，防止缓存等原因使用历史依赖，在自动化发布脚本中会经常用到。
<a name="gupEp"></a>
#### 打包排除单元测试
```bash
mvn clean package -Dmaven.test.skip=true
```
<a name="msnVj"></a>
#### 打包并指定环境
```bash
mvn clean package -P test
```
test为maven中配置的不同打包环境名称。
<a name="JGB1n"></a>
#### 打包指定子项目和父项目
```bash
mvn clean install -pl C -am -Dmaven.test.skip=true
```
说明：对父工程P、子模块C、以及子模块C依赖的B、A模块执行`mvn clean install`操作，涉及到的P、A、B、C模块全部install到本地。<br />在自动化构建时，通常会用到该组命令，对于基础板块可将install命令换成deploy命令，发布到私服中。
<a name="WgTEw"></a>
#### 只打包父工程
```bash
mvn clean install -N
```
只打包父工程P，子模块不会执行相关操作。有父子关系的项目构建上传到私服时，往往需要先构建上传父项目（pom项目）。
<a name="BVSWJ"></a>
#### 只打包A工程
```bash
mvn clean install -pl A -am
```
由于A工程不依赖其他模块，因此仅对P和A进行`mvn clean install`操作。
<a name="UE5GO"></a>
#### 被依赖项目打包
```bash
mvn clean install -pl A -amd
```
对父工程P、子模块A以及依赖于A模块的B、C模块执行`mvn clean install`操作。使用场景：基础的common模块变更了，通过该命令将依赖common的其他模块也进行重新`mvn clean install`操作。
<a name="CZoXs"></a>
#### Jar包发布
```bash
mvn clean deploy -Dmaven.test.skip=true
```
上述命令可以对jar包进行打包发布到仓库，同时需要在pom.xml中通过`distributionManagement`元素配置要发布的私有仓库地址信息。私有仓库的密码等信息在setting.xml文件中配置。
<a name="c6XdR"></a>
#### 打包指定setting.xml文件
```bash
mvn clean package -s settings.xml
```
<a name="A4gdv"></a>
#### 万能的命令
最后，还有一个万能的命令，help命令：<br />`mvn -help` 或 `mvn -h`<br />通过此命令可以查看更多Maven支持的命令操作。
<a name="E8SwH"></a>
## 五、IDEA中Maven操作
在IDEA中Maven的一些操作技巧与功能。
<a name="ycWpJ"></a>
### 1、通过Terminal执行命令
打开IDEA的Terminal命令窗口，默认是打开当前项目的根目录，可以直接执行`mvn`命令。也可通过cd等命令切换目录进行执行。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1655697539187-eb499866-4a17-4f0b-80a3-7263a45d1fd8.jpeg#averageHue=%23f8f8f8&clientId=uf06d6907-c418-4&from=paste&id=u17112a5d&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9cb10d6c-1c23-4477-a19b-2ff745d5806&title=)<br />该方式的优势是比IDE自动解析、下载jar包依赖要快非常多，而且不容易出现下载一半卡死的状况。
<a name="MjHFl"></a>
### 2、Maven图形界面执行
可通过IDEA提供的Maven图形界面执行：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1655697539320-265b765b-9f51-4a46-a945-f4a703f533b2.jpeg#averageHue=%23f5f3f2&clientId=uf06d6907-c418-4&from=paste&id=udc98e887&originHeight=1020&originWidth=1026&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub84105b9-0647-4b68-b10e-bcc4bd0fa4c&title=)<br />双击对应的命令即可执行。左上角的两个圆圈可以进行jar包依赖的刷新。
<a name="K8zy3"></a>
#### 禁用单元测试
在执行打包构建时如果不需要执行单元测试，可选择闪电符号。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1655697539341-9086aa4c-f1fe-4ee8-abdc-a5fe70f37e69.jpeg#averageHue=%23f1efee&clientId=uf06d6907-c418-4&from=paste&id=u813c1202&originHeight=534&originWidth=892&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub83654f3-0240-4143-a1f1-779116a2188&title=)<br />alt或command选择多个命令同时执行：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1655697539340-00da59ff-d329-497b-b4eb-405edea9eee8.jpeg#averageHue=%23f5ebe0&clientId=uf06d6907-c418-4&from=paste&id=u2ac5f743&originHeight=672&originWidth=952&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc76627a6-0638-43d0-a858-2d984b284f5&title=)

