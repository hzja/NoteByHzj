Maven
<a name="OQc07"></a>
## 一、摘要
在项目开发过程中，经常需要将一些公共方法提取出来，然后单独封装成一个第三方公共jar包，采用普通的方式打包后的jar，**依赖的工程执行编译时，却提示找不到对应的依赖包**，那么如何将工程打包为可执行jar包呢？<br />下面介绍三种通过maven将工程打包成可执行的打包方式。
<a name="ObFPU"></a>
## 二、方法实践
<a name="KK093"></a>
### 2.1、assembly插件
<a name="auTYj"></a>
#### 2.1.1、pom.xml的相关配置文件如下
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example.java</groupId>
  <artifactId>example-frame-fatJar</artifactId>
  <version>1.0.0</version>

  .....
  <build>
    <finalName>sso-api</finalName>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.2</version>
        <configuration>
          <source>1.7</source>
          <target>1.7</target>
          <encoding>UTF-8</encoding>
        </configuration>
      </plugin>
      <!-- maven-assembly-plugin -->
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-assembly-plugin</artifactId>
        <version>2.6</version>
        <configuration>
          <descriptorRefs>
            <descriptorRef>jar-with-dependencies</descriptorRef>
          </descriptorRefs>
          <encoding>UTF-8</encoding>
        </configuration>
        <executions>
          <execution>
            <id>make-assembly</id>
            <phase>package</phase>
            <goals>
              <goal>single</goal>
            </goals>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
</project>
```
<a name="EnA1Q"></a>
#### 2.1.2、运行如下命令，进行打包
```bash
mvn clean package
```
会在target文件夹里生成一个jar-with-dependencies的jar是可执行的。
<a name="FFU7r"></a>
#### 2.1.3、验证jar是否可执行
在带有jar-with-dependencies的jar文件下，打开终端，输入如下命令
```bash
#验证jar是否可执行，如果没有报错，说明没有问题
java -jar xxx-jar-with-dependencies.jar
```
<a name="BUcLJ"></a>
### 2.2、shade插件
<a name="c9Y62"></a>
#### 2.2.1、pom.xml的相关配置文件如下
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example.java</groupId>
  <artifactId>example-frame-fatJar</artifactId>
  <version>1.0.0</version>

  ......
  <build>
    <finalName>sso-api</finalName>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>3.2</version>
        <configuration>
          <source>1.7</source>
          <target>1.7</target>
          <encoding>UTF-8</encoding>
        </configuration>
      </plugin>
      <!-- maven-shade-plugin，不同的是shade可以将多个相同的配置文件追加合并 -->
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-shade-plugin</artifactId>
        <version>2.4.1</version>
        <executions>
          <execution>
            <phase>package</phase>
            <goals>
              <goal>shade</goal>
            </goals>
            <configuration>
              <transformers>
                <transformer
                  implementation="org.apache.maven.plugins.shade.resource.ManifestResourceTransformer">
                  <mainClass>com.xxg.Main</mainClass>
                </transformer>
                <transformer
                  implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">
                  <resource>META-INF/spring.handlers</resource>
                </transformer>
                <transformer
                  implementation="org.apache.maven.plugins.shade.resource.AppendingTransformer">
                  <resource>META-INF/spring.schemas</resource>
                </transformer>
              </transformers>
            </configuration>
          </execution>
        </executions>
      </plugin>
    </plugins>
  </build>
</project>
```
<a name="EyhoQ"></a>
#### 2.2.2、运行如下命令，进行打包
```bash
mvn clean package
```
发现生成了2个jar包，一个为：original-XXX.jar，另一个为：XXX.jar，其中original...jar里只包含了工程自己的class文件，而另外的一个jar包则包含了工程本身以及所有依赖的jar包的class文件。只需要使用第二个jar包就可以了。
<a name="XFemw"></a>
#### 2.2.3、验证jar是否可执行
在XXX.jar文件下，打开终端，输入如下命令
```bash
#验证jar是否可执行，如果没有报错，说明没有问题
java -jar XXX.jar
```
**不同点**：shade可以将多个相同的配置文件追加合并，比如，多个子项目下有相同的配置文件，shade在打包的时候，会将相同的配置文件合并。
<a name="eTZei"></a>
### 2.3、Fatjar打包工具(eclipse插件)
<a name="ZcxsD"></a>
#### 2.3.1、eclipse在线安装插件
1、打开eclipse，打开菜单help > Install New Sofware > Add...<br />**name**：FatJar<br />**URL**：[http://kurucz-grafika.de/fatjar](http://kurucz-grafika.de/fatjar)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666571366664-7c99f34b-9ceb-4389-bd69-09f105cf1415.png#clientId=u7aef9a9e-fd1c-4&from=paste&id=ud085b493&originHeight=365&originWidth=1068&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4ee47f28-46fb-4aaa-92ae-6953ef99677&title=)<br />根据提示下载安装并重启eclipse！
<a name="Kdjgk"></a>
#### 2.3.2、FatJar使用
在使用Eclipse进行导出时，**点击项目右键**，在弹出的右键菜单中选择**Build Fat Jar**, 打开配置Fat Jar弹出框；或者，**项目右键，点击Export**，然后在打开的Export选择框中选择**Other**下面的**Fat Jar Exporter**, 选择需要导出的项目，点击下一步打开配置Fat Jar弹出框。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666571366691-ac97664a-e2bf-4012-9af6-d29c23fa45cd.png#clientId=u7aef9a9e-fd1c-4&from=paste&id=u93a59ebd&originHeight=607&originWidth=627&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua00e14f1-abb9-4d25-a11c-0a69b2345f5&title=)
<a name="KLQUn"></a>
#### 2.3.3、验证jar是否可执行
在XXX.jar文件下，打开终端，输入如下命令
```bash
#验证jar是否可执行，如果没有报错，说明没有问题
java -jar XXX.jar
```
