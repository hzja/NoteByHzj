Java<br />在开发项目时，不希望自己的代码被别人抄袭，但是 Java 开发的项目可以被反编译，可以用代码混淆的方式来解决。Java 代码混淆就是为了保护 Java 源代码，对编译好的 class 文件进行混淆处理。<br />常见的加密方式：<br />对 class 文件的字节码完全加密<br />对 class 文件混淆：字节码混淆，比如对类名、变量名、方法名进行替换。<br />推荐一款好用的代码混淆工具 -- classfinal。
<a name="IdnhR"></a>
### 项目介绍
ClassFinal是一款java class文件安全加密工具，支持直接加密jar包或war包，无需修改任何项目代码，兼容spring-framework；可避免源码泄漏或字节码被反编译。<br />基于 `-agentJava:xxx.jar` 这一原理，加密时对 class 文件做了两次处理，一次是对 class 文件的字节码完全加密，一次是对 class 文件混淆，这个混淆是保留成员和方法，对方法内部实现进行隐藏；<br />解密时，判断如果该类是自己加密过的，找到完全加密的字节码进行解密，如果不是自己加密的就跳过。其对 class 文件混淆，就是方便类似 SpringBoot 等三方框架直接分析 class 文件。
<a name="SEvOs"></a>
### 功能特性

- 无需修改原项目代码，只要把编译好的jar/war包用本工具加密即可。
- 运行加密项目时，无需求修改tomcat，spring等源代码。
- 支持普通jar包、springboot jar包以及普通java web项目编译的war包。
- 支持spring framework、swagger等需要在启动过程中扫描注解或生成字节码的框架。
- 支持maven插件，添加插件后在打包过程中自动加密。
- 支持加密WEB-INF/lib或BOOT-INF/lib下的依赖jar包。
- 支持绑定机器，项目加密后只能在特定机器运行。
- 支持加密SpringBoot的配置文件。
<a name="CMrez"></a>
### 下载使用
[https://repo1.maven.org/maven2/net/roseboy/classfinal-fatjar/1.2.1/classfinal-fatjar-1.2.1.jar](https://repo1.maven.org/maven2/net/roseboy/classfinal-fatjar/1.2.1/classfinal-fatjar-1.2.1.jar)<br />参数说明：
```bash
参数说明
-file        加密的jar/war完整路径
-packages    加密的包名(可为空,多个用","分割)
-libjars     jar/war包lib下要加密jar文件名(可为空,多个用","分割)
-cfgfiles    需要加密的配置文件，一般是classes目录下的yml或properties文件(可为空,多个用","分割)
-exclude     排除的类名(可为空,多个用","分割)
-classpath   外部依赖的jar目录，例如/tomcat/lib(可为空,多个用","分割)
-pwd         加密密码，如果是#号，则使用无密码模式加密
-code        机器码，在绑定的机器生成，加密后只可在此机器上运行
-Y           无需确认，不加此参数会提示确认以上信息
```
<a name="fn3Rr"></a>
### Maven插件方式
在要加密的项目pom.xml中加入以下插件配置,目前最新版本是：1.2.1。
```xml
<plugin>
    <!-- https://gitee.com/roseboy/classfinal -->
    <groupId>net.roseboy</groupId>
    <artifactId>classfinal-maven-plugin</artifactId>
    <version>${classfinal.version}</version>
    <configuration>
        <password>000000</password><!--加密打包之后pom.xml会被删除，不用担心在jar包里找到此密码-->
        <packages>com.yourpackage,com.yourpackage2</packages>
        <cfgfiles>application.yml</cfgfiles>
        <excludes>org.spring</excludes>
        <libjars>a.jar,b.jar</libjars>
    </configuration>
    <executions>
        <execution>
            <phase>package</phase>
            <goals>
                <goal>classFinal</goal>
            </goals>
        </execution>
    </executions>
</plugin>
```
运行`mvn package`时会在target下自动加密生成yourpaoject-encrypted.jar。<br />maven插件的参数名称与直接运行的参数相同，请参考上节的参数说明。
<a name="HVdPl"></a>
### 加密文件
加密时`-pwd`参数设为`#`，启动时可不用输入密码；如果是war包，启动时指定参数 `-nopwd`，跳过输密码过程。
<a name="yldvQ"></a>
### 机器绑定
机器绑定只允许加密的项目在特定的机器上运行；<br />在需要绑定的机器上执行以下命令，生成机器码
```bash
java -jar classfinal-fatjar.jar -C
```
加密时用`-code`指定机器码。机器绑定可同时支持机器码+密码的方式加密。
