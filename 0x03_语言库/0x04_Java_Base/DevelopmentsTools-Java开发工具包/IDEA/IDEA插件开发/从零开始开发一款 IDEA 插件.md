<a name="sfbKL"></a>
## IDEA 插件介绍
平时最常用的ide就是IntelliJ IDEA。平时会用到各种各样的插件，通过插件的使用，提高自己的开发效率。

- IDEA具有全局性，安装好插件后，对idea生效，所有的工程均能找到。
- IDEA使用的便捷性，在固定的菜单中，或者右键即可看到该插件的使用入口
- 专注度高，展示出来的内容均是自己需要关注的，不需要关注的就不会展示。可以专注思考自己要怎么用。
- 和系统解耦，插件可以给给开发提效，却不会去占用项目的空间位置。
<a name="mChnx"></a>
## 为什么开发IDEA插件？
目前已有插件并不能真的完全满足自己的需求。举个简单的栗子，比如，最近在用mybatis plus的generator，配置完成之后，运行main方法，就可以生成ORM代码。新建一个工程，就需要把代码给拷贝过去，然后再重新配置一下，再生成。<br />缺点：

- 过于繁琐，每次都需要去把代码给拷贝过去，然后运行。
- 冗余，这个代码对于实际的业务来说，没有任何意义。
- 不便捷，代码只能生成在固定地方，否则就得自己去更改配置。然后自己再重复性的去复制过去。
- 不便捷，代码放在固定地方，在比较大的工程结构中，自己需要去找。如果新加入的开发不熟悉，并不知道这个在哪，低效。

基于以上思考，就需要开发插件来和系统解耦，达到提效的效果。
<a name="CrA6n"></a>
## 开发插件的场景需要准备些什么

- 首先你需要有一定的开发经验，新手的话，还是需要先学习语言基础，idea的使用等。
- 有需求场景，知道自己接下来要开发的是一个什么东西，围绕着需求不断地去完善插件
- 大致看下idea plugin sdk的开发手册，对它能干什么有一个大致的了解。
- 如果要使用gradle开发，需要对gradle有一定的使用能力。
<a name="CWgQo"></a>
## 开发插件流程（最简单的使用场景）
<a name="LuuAe"></a>
### 新建项目（使用的最新的idea，创建流程稍有不一样）
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793340-1c7fe9b4-3c76-49e4-9d29-c0f8666a1da8.png#averageHue=%233f4144&clientId=ua550ef7a-3b30-4&from=paste&id=u06198273&originHeight=749&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua231ba62-d0bf-4060-8eb4-104c4f5b1d5&title=)
<a name="YR8vs"></a>
### 配置gradle
最新版的插件开发使用的是kts的gradle插件。基本用法是一样的，不同的是写法有稍微一点不一样。
<a name="fyC3K"></a>
#### 一些常用的配置
依赖
```groovy
//顶层结构
dependencies {
    implementation("org.springframework.boot:spring-boot-starter:2.7.0")
}

// 一般的gradle是 
compile group: 'org.springframework.boot', name: 'spring-boot-starter', version: '2.7.0'
```
将依赖打进jar包中（不打进去，插件如果有用到依赖，就会无法使用，找不到类）
```
// 顶层结构
tasks.jar.configure {
    duplicatesStrategy = org.gradle.api.file.DuplicatesStrategy.INCLUDE
    from(configurations.runtimeClasspath.get().filter { it.name.endsWith("jar")}.map { zipTree(it) })
}
```
将仓库改为阿里云仓库
```
// 顶层结构
repositories {
    mavenLocal()
    maven("https://maven.aliyun.com/nexus/content/repositories/central/")
    mavenCentral()
}
```
<a name="BcQWN"></a>
### 创建第一个action
```java
import com.intellij.openapi.actionSystem.AnAction;
import com.intellij.openapi.actionSystem.AnActionEvent;
import com.intellij.openapi.project.Project;
import com.intellij.openapi.ui.Messages;

public class MyAction extends AnAction {
    @Override
    public void actionPerformed(@NotNull AnActionEvent e) {
        Project project = e.getProject();
        Messages.showMessageDialog(project, "this is message content", "this message title", Messages.getInformationIcon());
    }
}
```
<a name="rkrdq"></a>
### 将action配置到plugin中
src -> main -> resource -> META-INF -> plugin.xml
```xml
<idea-plugin>
  <!-- 插件的id，该id是唯一的，插件的坐标是一维的，只有一个id。这点和maven的不太一样，maven的是二维坐标 -->
  <id>com.geek45.plugin-demo</id>
  <!-- 插件名字-->
  <name>plugin demo</name>
  <!-- 插件的描述，有固定格式，这里可以写html代码 -->
  <description><![CDATA[
    <h1> Demo plugin </h1>
    测试插件 <br>
    点击插件可以弹出一个弹窗
    <p>
    plugin demo <br />
    <b>
    <a href="https://www.geek45.com">Website</a> |
    <a href="https://github.com/xiaopangzhi795/plugin-demo">GitHub</a> |
  </b>
  </p>
    ]]>
  </description>
  <!-- 这里是插件依赖管理，和gradle的依赖不一回事，可以依赖其他的插件 -->
  <depends>com.intellij.modules.platform</depends>
  <!-- 拓展管理，后续的ui页面会在这里配置，暂时用不到 -->
  <extensions defaultExtensionNs="com.intellij">

  </extensions>
  <!-- action配置，按钮展示在哪里需要在这配置 -->
  <actions>
    <action class="com.geek45.action.MyAction" id="myAction" text="action" description="this is action demo" >
      <!-- 将插件放在Tools菜单中的first位置。 -->
      <add-to-group group-id="ToolsMenu" anchor="first" />
    </action>
  </actions>

</idea-plugin>
```
<a name="mGcn9"></a>
### 打包
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793375-c95b9f1a-1aaf-4874-a08b-9edcbd6ae315.png#averageHue=%233f4248&clientId=ua550ef7a-3b30-4&from=paste&id=u2e4ec321&originHeight=646&originWidth=432&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u669ba316-3c87-42ce-a490-e4db3fb73cd&title=)<br />gradle中，运行jar命令，就可以把插件打成jar包，在项目根目录中，有build目录，libs中会有打好的jar包。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793321-adc444ed-c18c-42f9-8318-298fbb85a132.png#averageHue=%233b3e44&clientId=ua550ef7a-3b30-4&from=paste&id=u6a441eb8&originHeight=503&originWidth=973&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd1e71a8-900b-44be-949a-8a5db7b2b46&title=)<br />plugin中，选择本地安装，然后选择刚才打好的jar包，就可以把插件安装到自己的idea中。
<a name="nBJO7"></a>
### 本地运行
打包安装太麻烦了，可以选择本地运行的方式进行调试。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793324-4fe5724a-cbfb-48a8-abfa-36f07bdee2cf.png#averageHue=%23404349&clientId=ua550ef7a-3b30-4&from=paste&id=u5c11caa4&originHeight=598&originWidth=437&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud8eee5f8-19f0-4e57-8b13-b921e0ead37&title=)<br />选择runIde，就可以本地运行。该命令支持debug运行，debug可以打断点。<br />运行该命令，会启动一个新的IDEA，该IDEA中已经安装了开发的插件。在该idea中可以直接使用插件，使用过程中，可以在项目里面断点跟进。<br />新的IDEA和正常IDEA操作是一样的，可以打开一个测试code，来测试你的插件是否正常运行。
<a name="TPqEQ"></a>
### 运行效果
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793335-8002d991-febf-4831-9e1c-a1c7e12e8035.png#averageHue=%2331392d&clientId=ua550ef7a-3b30-4&from=paste&id=u6708735c&originHeight=953&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7369e84-9192-481b-9475-437211c5fdf&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1693877793861-1ceec1ec-040d-4624-817c-c6d95bef7080.png#averageHue=%23303436&clientId=ua550ef7a-3b30-4&from=paste&id=u729a68a7&originHeight=414&originWidth=1060&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3d153d02-aaf0-40c3-8cfa-520e0ced503&title=)
