IDEA Plugins
<a name="vId9X"></a>
## 开发环境
开发IDEA插件有以下这些依赖：

- IntelliJ IDEA Community Edition
- IntelliJ IDEA Community Edition 源码
- Plugin DevKit 插件
- IntelliJ Platform SDK

本地可能已经安装了Ultimate版本，但是还是需要安装IDEA的社区版本。因为商业版是闭源的，所以在调试时无法调试核心代码。<br />社区版的安装包里是不包含源码的，所以需要手动从github上clone一份：
```bash
git clone --depth 1 git://git.jetbrains.org/idea/community.git idea
```
关于从源码运行IDEA的方法参考：Check Out And Build Community Edition
<a name="F3pHJ"></a>
### 添加 IDEA jdk
虽然不知道原因，但是根据Check Out And Build Community Edition，需要建立一个IDEA jdk 来运行插件：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076249-be22f9b7-b95d-4862-8ed3-0b4959e09572.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u625499ef&originHeight=665&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6279506c-5cb3-4b3e-b3f8-b4847a6256d&title=)<br />除非在Mac上使用官方JDK，否则需要手动添加 /lib/tools.jar 到classpath中。
<a name="b4Did"></a>
### 配置 IntelliJ Platform SDK
打开 File | Project Structure 新建一个 IntelliJ Platform SDK：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076146-b7fa42a4-eda9-4fa6-8a09-68d15877e749.webp#clientId=u35b8c255-f5fb-4&from=paste&id=ub1339db4&originHeight=593&originWidth=959&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u317d22c4-1159-4f0c-abf2-86fa7f8c663&title=)<br />Java SDK 选择刚刚建立的 IDEA jdk：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076269-4539bd90-ca8c-4df3-8320-681cfb7108c4.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u0fd2ec4c&originHeight=140&originWidth=431&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u48fb4954-36f9-4edb-af54-997872427be&title=)<br />然后可以把下载的 IDEA 社区版源码添加到源码路径中，这样在调试时，就可以调试 IDEA 自身的代码了：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076134-48fb5fcc-0657-45bb-9104-bb101e950a89.webp#clientId=u35b8c255-f5fb-4&from=paste&id=ue8ce855f&originHeight=665&originWidth=960&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9a2fcf98-8bbc-448f-8b10-d7f07e12c7b&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076061-221cd133-ccea-4bb8-94e8-e4ec8749441f.webp#clientId=u35b8c255-f5fb-4&from=paste&id=ue8432461&originHeight=551&originWidth=490&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7db6914d-171d-4596-a8d8-0ba2741b090&title=)
<a name="Vf9MK"></a>
## 第一个插件
来编写一个最简单的插件来学习编写一个插件的完整步骤。
<a name="nAf9F"></a>
### 新建工程
选择IntellJ Platform Plugin，然后Project SDK指定刚刚新建的plugin sdk：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076774-c34d17b1-5b2f-4fb2-8187-29d14a213283.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u8955a07e&originHeight=536&originWidth=943&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u42c98c81-f4b8-46da-bb86-d89c545c736&title=)<br />新建的插件项目：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076674-ddc1c016-e3bc-45fe-b9a3-0dea3afe85bd.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u5dd1342c&originHeight=262&originWidth=560&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua6ed22a6-fc61-4a9a-9eb1-866d7db6c02&title=)<br />插件根目录下有两个目录src和resources。src是插件代码目录，resource是插件资源目录，其中META-INF/plugin.xml是插件的描述文件，就像Java web项目的web.xml一样。<br />plugin.xml 默认的内容如下：
```xml
<idea-plugin>
  <id>com.your.company.unique.plugin.id</id>
  <name>Plugin display name here</name>
  <version>1.0</version>
  <vendor email="support@yourcompany.com" url="http://www.yourcompany.com">YourCompany</vendor>

  <description><![CDATA[
      Enter short description for your plugin here.<br>
      <em>most HTML tags may be used</em>
    ]]></description>

  <change-notes><![CDATA[
      Add change notes here.<br>
      <em>most HTML tags may be used</em>
    ]]>
  </change-notes>

  <!-- please see http://www.jetbrains.org/intellij/sdk/docs/basics/getting_started/build_number_ranges.html for description -->
  <idea-version since-build="145.0"/>

  <!-- please see http://www.jetbrains.org/intellij/sdk/docs/basics/getting_started/plugin_compatibility.html
       on how to target different products -->
  <!-- uncomment to enable plugin in all products
  <depends>com.intellij.modules.lang</depends>
  -->

  <extensions defaultExtensionNs="com.intellij">
    <!-- Add your extensions here -->
  </extensions>

  <actions>
    <!-- Add your actions here -->
  </actions>

</idea-plugin>
```
<a name="dDs2b"></a>
### 新建一个 Action
插件扩展 IDEA 最常见的方式就是在菜单栏或者工具栏中添加菜单项，用户通过点击菜单项来触发插件功能。IDEA 提供了 AnAction 类，这个类有一个虚方法 actionPerformed，这个方法会在每次菜单被点击时调用。<br />新建一个自定义的 Action 有两个步骤：

1. 继承 AnAction 类，在 actionPerformed 方法中实现插件逻辑
2. 注册 action，有两种方式，通过代码注册和通过 plugin.xml 注册

先写一个简单的Action类：
```java
public class TextBoxes extends AnAction {
    // 如果通过Java代码来注册，这个构造函数会被调用，传给父类的字符串会被作为菜单项的名称
    // 如果你通过plugin.xml来注册，可以忽略这个构造函数
    public TextBoxes() {
        // 设置菜单项名称
        super("Text _Boxes");
        // 还可以设置菜单项名称，描述，图标
        // super("Text _Boxes","Item description",IconLoader.getIcon("/Mypackage/icon.png"));
    }
 
    public void actionPerformed(AnActionEvent event) {
        Project project = event.getData(PlatformDataKeys.PROJECT);
        String txt= Messages.showInputDialog(project, "What is your name?", "Input your name", Messages.getQuestionIcon());
        Messages.showMessageDialog(project, "Hello, " + txt + "!\n I am glad to see you.", "Information", Messages.getInformationIcon());
    }
}
```
然后在plugin.xml中注册这个Action：
```xml
<actions>
  <group id="MyPlugin.SampleMenu" text="_Sample Menu" description="Sample menu">
    <add-to-group group-id="MainMenu" anchor="last"  />
       <action id="Myplugin.Textboxes" class="Mypackage.TextBoxes" text="Text _Boxes" description="A test menu item" />
  </group>
</actions>
```
这里新建了一个菜单组，其中 text 字符串的下划线表示这个字母作为快捷键。这个菜单显示的效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908076950-41e1fbb9-58d3-465f-89ca-3485b9c1fae9.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u0b4395a0&originHeight=112&originWidth=336&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub5633324-1898-48ca-a7ea-50ab12f336f&title=)<br />除了手动新建Action，IDEA还提供了快速新建的方法，在代码目录上点击新建，可以看到Action：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908077019-2a02f3e5-1dab-472b-8c9c-41b7300520f2.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u5d6424cf&originHeight=800&originWidth=372&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua81c958e-569f-4eb3-bc64-0cc6ee893a8&title=)<br />可以在这个面板中填写要新建的Action信息，IDEA会新建类，还有在plugin.xml中注册：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644908077165-632ac3cf-247e-4e55-8fb0-651f1e73e018.webp#clientId=u35b8c255-f5fb-4&from=paste&id=u4e862600&originHeight=620&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa02dbf9-24f6-416b-aec1-8eccf171407&title=)
<a name="MWx7p"></a>
### 运行插件
运行插件特别简单，和运行普通Java代码一样，点击运行或者调试的按钮，就会启动一个新的IDEA实例，这个实例中插件是生效的。<br />点击 Text Boxes 就可以看到插件的效果了。
