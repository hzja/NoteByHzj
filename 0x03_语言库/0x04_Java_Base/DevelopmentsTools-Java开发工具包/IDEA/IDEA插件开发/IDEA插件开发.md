IDEA IDEA Plugins
<a name="mfRXk"></a>
### 准备工作
这里使用 idea 社区版进行开发，社区版的源码都是开放的，方便开发调试。如果安装了付费版，开发插件当然也没问题，但是idea 插件开发过程中，还是会自动下载 idea 社区版的源码到本地的。<br />社区版下载地址 [https://www.jetbrains.com/idea/download/#section=windows](https://www.jetbrains.com/idea/download/#section=windows)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185971196-fedb4edd-c3e8-4a79-b7b8-7f743a6c0968.png#clientId=u708e9fd2-4386-4&from=paste&height=713&id=u4a32b724&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=253906&status=done&style=shadow&taskId=ub0c4e961-97d0-4a98-99fc-89bb3bbc67d&title=&width=1536)

idea 官方建议使用最新版本的社区
> You can use either IntelliJ IDEA Community Edition or IntelliJ IDEA Ultimate as your IDE (it is highly recommended to use the latest available version). Both include the complete set of plugin development tools.

所以自己在开发的过程中，尽量使用最新的版本去进行开发。
<a name="SMfQz"></a>
### 创建工程
这里简单说一下， 目前创建 idea 插件工程有三种方式， 分别是 Github Template， Gradle ，DevKit 。DevKit 是最早期的开发方式，官方比较推荐 Github Template 和 Gradle。<br />这里通过创建 Gradle 项目的方式来开发插件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185769741-4a031b8d-5859-4bb4-9217-186610513bbd.png#clientId=u708e9fd2-4386-4&from=paste&id=u076cdba4&originHeight=398&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u44311c3c-b3d3-4fd0-8e3f-a409879d60b&title=)<br />新建项目 -> 选中 Gradle -> 选择项目 SDK 。注意这里需要选择 JDK 11 版本以上的 JDK 。在开发过程中使用 Microsoft 编译的 JDK11 的版本JDK，最后编译生成项目的时候编译报错了。换了 JDK 版本之后就好了，这个问题比较诡异。<br />如果想使用kotlin 进行开发的话，记得选中画圈中间的 Kotlin/JVM 。如果想让Gradle  也使用 kotlin ， 那可以选择 kotlin DSL 构建脚本。<br />选择完毕之后，进入下一步。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185769750-ec74b4de-5b74-4964-a7bf-5a74560c9fb2.png#clientId=u708e9fd2-4386-4&from=paste&id=uc689de89&originHeight=355&originWidth=416&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4a6117c4-8c34-4abf-9763-31c50747416&title=)<br />输入项目名称，包名之后，这个和创建Java项目一致的。<br />点击完成，等待 idea 下载开发插件所需的依赖。需要等待几分钟，具体视网络情况而定。
<a name="DA2l9"></a>
### 项目结构
项目结构大致如下
```
plugindemo
├── build.gradle
├── gradle
│   └── wrapper
│       ├── gradle-wrapper.jar
│       └── gradle-wrapper.properties
├── gradlew
├── gradlew.bat
├── settings.gradle
└── src
    ├── main
    │   ├── java
    │   └── resources
    │       └── META-INF
    │           └── plugin.xml
    └── test
        ├── java
        └── resources
```
plugin.xml 是需要重点关注的文件，插件的所有配置项都在这里面。
```xml
<idea-plugin>
	<id>org.example.plugindemo</id>
	<name>pplugin Demo</name>
	<vendor  url="http://www.xx.com">seiko</vendor>
	
	<description><![CDATA[
		test ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest ittest it

		]]></description>
	
	<!-- please see https://plugins.jetbrains.com/docs/intellij/plugin-compatibility.html
				 on how to target different products -->
	<depends>com.intellij.modules.platform</depends>
	
	<extensions defaultExtensionNs="com.intellij">
		<!-- Add your extensions here -->
	</extensions>
	
	<actions>
		<!-- Add your actions here -->
		<group id="MyPlugin.GreetingMenu" text="Greeting" description="xxx">
			<add-to-group group-id="MainMenu" anchor="last"/>
			<action class="org.example.plugindemo.HelloAction" id="MyPlugin.Hello" text="hello" description="hello"/>
		</group>
	</actions>
</idea-plugin>
```
plugin.xml 中的 actions 节点，就是开发插件所有的操作入口。这里把插件配置到主菜单上面。主要操作就是一个 helloAction。
<a name="S5sz7"></a>
### 创建 Action
新建一个 class 继承 AnAction就行。这里开发的过程中继承了 `DumbAwareAction`， `DumbAwareAction`本身继承了 AnAction。
```kotlin
class HelloAction : DumbAwareAction() {
    
    override fun actionPerformed(e: AnActionEvent) {
        val project = e.getData(PlatformDataKeys.PROJECT);
        Messages.showMessageDialog(project, "Hello Kotlin", "Greeting", Messages.getInformationIcon());
    }
}
```
继承实现了 actionPerformed 方法。弹出一个 Hello Kotlin的提示框。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185769738-2707aec6-aa5e-4f12-88e3-6f1018a7cf90.png#clientId=u708e9fd2-4386-4&from=paste&id=ud5f2e6ce&originHeight=300&originWidth=579&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud23bec97-af33-4755-a2de-cbb62a28f03&title=)<br />需要对 Action 注册到 plugin.xml ， 可以在 class 上进行操作，进行“注册操作”。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185769752-3ae1e57b-a285-4f71-a454-faaee9888149.png#clientId=u708e9fd2-4386-4&from=paste&id=u4ec4025a&originHeight=610&originWidth=818&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5e468345-738f-4eb0-aca5-cb7f4a886ad&title=)<br />这种方式比较友好， 如果直接在plugin.xml 编辑的话，容易出错。<br />注册完了之后，就可以通过 Gradle 的 runIde 进行运行。注意这里会独立启动一个IDE来运行开发的插件， 需要注意下电脑的内存情况。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185770237-716d4d5a-c99b-4e2a-b4e1-c36c467c83ef.png#clientId=u708e9fd2-4386-4&from=paste&id=uddcd71b2&originHeight=635&originWidth=380&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ucba18914-cda7-4f5a-b78d-2fb22bfea04&title=)<br />运行启动之后，能够看到主菜单上 有个 Greeting 的菜单栏，这里和在 plugin.xml 中的actions 配置项是一致的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185770313-9ebe9f15-1f51-4f71-bb60-45c01e46f9cd.png#clientId=u708e9fd2-4386-4&from=paste&id=u793eef26&originHeight=253&originWidth=406&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue365ae15-1ec1-447e-8ff8-db309cc6cb7&title=)<br />点击 Hello ， 弹出一个Hello Kotlin 的消息框， 这里就是 HelloAction 的输出结果。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646185770201-c66cda59-d3d3-48ee-9dab-684a0c1dd28d.png#clientId=u708e9fd2-4386-4&from=paste&id=u76d1fe7b&originHeight=400&originWidth=515&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u57e8c1ff-b888-47a0-8032-29abdc518f0&title=)<br />到这里可以算是开发完一个简单的插件了。
