IDEA Plugins
<a name="DcfOO"></a>
## 版本工具说明

- JDK1.8
- IDEA2020.1
- 插件项目基于gradle构建。
- 知识背景：swing
<a name="Xs6Ae"></a>
## 目标
本实例实现一个Idea的插件，弹出一个表单Dialog，然后点击按钮，获取表单里输入的内容，然后将内容打印在表单的上方。<br />成品图展示：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114292-d0dab384-182f-4a9b-abe3-55cc5a7179bd.webp#clientId=uc932fb85-3596-4&from=paste&id=u6938745e&originHeight=500&originWidth=424&originalType=url&ratio=1&status=done&style=none&taskId=u16b44bd7-3190-4b66-8243-1fd29245693)
<a name="Wg9LO"></a>
## 项目初始化
新建一个gradle项目，修改其build.gradle文件：
```groovy
plugins {
    id 'java'
    id 'org.jetbrains.intellij' version '0.4.14' //引入intellij的gradle插件
}
 
group 'org.example'
version '1.0' //定义jar包/zip包的版本号
 
sourceCompatibility = 1.8 //限制jdk的使用版本号，这里限制到8，表示生成的idea插件只能运行在jdk8以上的环境中
 
repositories {
    mavenCentral() //远程仓库
}
 
dependencies {
    //这里引别的依赖包
    testCompile group: 'junit', name: 'junit', version: '4.12'
}
 
// See https://github.com/JetBrains/gradle-intellij-plugin/
intellij {
    // 这里是指打插件包的时候用idea什么版本的依赖包打
    // 比如这里用2019.3打包，如果你的插件实现源码里用了2019.3不存在的依赖包或类，就会报错
    // 一般就填当前IDEA的版本号即可
    version "2019.3"
}
 
patchPluginXml {
 
    //changeNotes里的内容展示位置参考图14
    changeNotes """
      1.0版本.

      第1.0版本：初始化这个测试插件项目"""
 
    // 这个意思是说当前定义的这个插件最早支持到什么版本的IDEA
    // 这里配置sinceBuild=191，表示插件只能被版本号大于等于2019.1版本的IDEA安装，低于这个版本的将抛无法兼容的错误
    // ↑上方参考这篇问答：https://intellij-support.jetbrains.com/hc/en-us/community/posts/360003338799-Build-compatible-plugin
    sinceBuild "191"
}
```
然后Idea的右边栏gradle将会多出intellij选项：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114078-09190434-5eed-4fa1-a74e-51a31c25b140.webp#clientId=uc932fb85-3596-4&from=paste&id=ua6666b5b&originHeight=419&originWidth=361&originalType=url&ratio=1&status=done&style=none&taskId=u5c009b30-5713-40a8-a925-f44787204d1)<br />这里说下runIde，它用来调试插件，运行它会再次启动一个Idea，这个Idea会自动安装上当前定义的插件包，用来调试。
<a name="zlqdS"></a>
## 新增plugin.xml
这个文件非常重要，它可以指定定义的插件出现在IDEA的哪个位置，可以指定具体的处理逻辑，还可以定义插件名称、子名称等等。<br />这个文件位于MATE-INF下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114058-c06dfaed-39b6-44d9-bd0e-a4d169f20abb.webp#clientId=uc932fb85-3596-4&from=paste&id=ufb408ce8&originHeight=365&originWidth=332&originalType=url&ratio=1&status=done&style=none&taskId=uc10997dc-a5c5-4751-bf87-5fbced0337b)<br />配置内容为：
```xml
<idea-plugin>
    <!--插件的id，注意不要跟其他插件重复，这个id全局唯一，尽可能复杂些-->
    <id>plugin.test</id>
    <!--插件的名称-->
    <name>PluginTest</name>
    <vendor email="xxxx@qq.com" url="http://www.bilibili.com">你公司的名字</vendor>

    <!--插件的描述信息，支持html，展示的位置参考图14-->
    <description><![CDATA[
    Plugin Test<br>
    第一行：单纯只是个测试<br>
    第二行：都说了只是个测试(●￣(ｴ)￣●)<br>
    <a href='https://www.bilibili.com'>你猜猜这是哪个网站？</a>
    <em>v1.0</em>
    ]]></description>
    
    <extensions defaultExtensionNs="com.intellij">
        <!-- Add your extensions here -->
    </extensions>

    <!--跟build.gradle里的sinceBuild一致即可，意义相同，必须配置-->
    <idea-version since-build="191"/>

    <actions>
        <!--下面的group是分组，分组需要有一个唯一的id标识，text用来控制分组出现在IDEA时呈现的文案，description是描述，不会展现出来，简单描述下分组就行-->
        <group id="PluginTest" text="插件测试组" description="插件测试描述">
            <!--add-to-group控制把该分组加到IDEA里，group-id用来描述加在哪个位置，MainMenu表示加在IDEA上方的主菜单栏里，
            anchor表示顺序，last表示最后一个，所以下面的配置可以描述为：将该插件加到IDEA上方主菜单栏的最后一位-->
            <add-to-group group-id="MainMenu" anchor="last"/>
            <!--这个用来指定一个分组下的触发动作，同样的需要一个id，自定义；class就是用来处理这个动作的逻辑类，具体的插件逻辑都会写到对应的action类里，text用来控制文案，description为描述-->
            <action id="Plugin.Test.Action"
                    class="plugin.test.FromAction"
                    text="表单测试" description="表单测试描述"/>
        </group>
    </actions>
</idea-plugin>
```
然后定义一个`Action`类，记为`FormAction`，继承`AnAction`，实现其抽象方法`actionPerformed`即可：
```java
public class FromAction extends AnAction {
    @Override
    public void actionPerformed(@NotNull AnActionEvent e) {
        //TODO 这里放插件逻辑
    }
}
```
<a name="PQZ40"></a>
## 启动
现在双击`runIde`即可调出另外一个安装了这个插件的IDEA界面，然后可以看运行结果进行调试。runIde还支持debug模式，不过运行时要右击选择：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114148-e1b95add-27f4-4fa0-8bab-3e3028c4769a.webp#clientId=uc932fb85-3596-4&from=paste&id=uf1e2e64a&originHeight=523&originWidth=517&originalType=url&ratio=1&status=done&style=none&taskId=uac1a6c09-f92a-4f66-9751-c1c99ae14cf)<br />来看下调试IDEA的界面运行效果：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114105-ff211bc7-645f-48c0-aa12-5728350ec5d9.webp#clientId=uc932fb85-3596-4&from=paste&id=u8ba92c00&originHeight=178&originWidth=960&originalType=url&ratio=1&status=done&style=shadow&taskId=u240a3aa3-ef78-4c1e-bfb1-6ac97cf25fb)
<a name="mX50e"></a>
## 定义Action
<a name="wh6O1"></a>
### 1. 定义会话框类
经过上面三步的配置，插件的基本样式已经展示出来，但是点击下方“表单测试”的action，并没有什么用，因为其绑定的FormAction类里没有任何有意义的实现。现在来实现开始的目标，点击“表单测试”后，弹出一个自定义的表单会话框，然后点击按钮，获取表单内容后打印在会话框内。<br />会话框（Dialog）需要定义一个继承了IDEA的DialogWrapper抽象类的子类，这个子类就是自定义的会话框实现，所有的样式定义、功能触发都是放到这个子类里的，现定于如下子类：
```java
public class FormTestDialog extends DialogWrapper {
 
    private String projectName; //假如需要获取到项目名，作为该类的属性放进来
 
    // DialogWrapper没有默认的无参构造方法，所以需要重写构造方法，它提供了很多重载构造方法，
    // 这里使用传project类型参数的那个，通过Project对象可以获取当前IDEA内打开的项目的一些属性，
    // 比如项目名，项目路径等
    public FormTestDialog(@Nullable Project project) {
        super(project);
        setTitle("表单测试~~"); // 设置会话框标题
        this.projectName = project.getName();
    }
 
    // 重写下面的方法，返回一个自定义的swing样式，该样式会展示在会话框的最上方的位置
    @Override
    protected JComponent createNorthPanel() {
        return null;
    }
 
    // 重写下面的方法，返回一个自定义的swing样式，该样式会展示在会话框的最下方的位置
    @Override
    protected JComponent createSouthPanel() {
        return null;
    }
 
    // 重写下面的方法，返回一个自定义的swing样式，该样式会展示在会话框的中央位置
    @Override
    protected JComponent createCenterPanel() {
        return null;
    }
}
```
<a name="Wb1G8"></a>
### 2. 会话框模块&类元素对照
找个实际的会话框为例，针对上述中几个方法所控制的会话框里的元素如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114683-901810e8-6b01-4813-8bfd-1bf290029cf0.webp#clientId=uc932fb85-3596-4&from=paste&id=u49c772c0&originHeight=513&originWidth=1024&originalType=url&ratio=1&status=done&style=none&taskId=u60c18cde-0233-424c-80b3-6db1f4c6330)
<a name="LSm4r"></a>
### 3. 会话框方法重定义
按照本文的实现目标，自定义的表单主体部分可以位于`createCenterPanel`里，然后表单的大标题可以放到`createNorthPanel`里，提交按钮可以放到`createSouthPanel`里，现在改写如下：
```java
public class FormTestDialog extends DialogWrapper {
 
    private String projectName;
     
    //swing样式类，定义在4.3.2
    private FormTestSwing formTestSwing = new FormTestSwing();
 
    public FormTestDialog(@Nullable Project project) {
        super(true);
        setTitle("表单测试~~"); //设置会话框标题
        this.projectName = project.getName(); //获取到当前项目的名称
        init(); //触发一下init方法，否则swing样式将无法展示在会话框
    }
 
    @Override
    protected JComponent createNorthPanel() {
        return formTestSwing.initNorth(); //返回位于会话框north位置的swing样式
    }
 
    // 特别说明：不需要展示SouthPanel要重写返回null，否则IDEA将展示默认的"Cancel"和"OK"按钮
    @Override
    protected JComponent createSouthPanel() {
        return formTestSwing.initSouth();
    }
 
    @Override
    protected JComponent createCenterPanel() {
        //定义表单的主题，放置到IDEA会话框的中央位置
        return formTestSwing.initCenter();
    }
}
```
<a name="ef21O"></a>
### 4. 自定义swing样式
下面是放置swing样式的类：
```java
public class FormTestSwing {
 
    private JPanel north = new JPanel();
 
    private JPanel center = new JPanel();
 
    private JPanel south = new JPanel();
 
    //为了让位于底部的按钮可以拿到组件内容，这里把表单组件做成类属性
    private JLabel r1 = new JLabel("输出：");
    private JLabel r2 = new JLabel("NULL");
 
    private JLabel name = new JLabel("姓名：");
    private JTextField nameContent = new JTextField();
 
    private JLabel age = new JLabel("年龄：");
    private JTextField ageContent = new JTextField();
 
    public JPanel initNorth() {
 
        //定义表单的标题部分，放置到IDEA会话框的顶部位置
 
        JLabel title = new JLabel("表单标题");
        title.setFont(new Font("微软雅黑", Font.PLAIN, 26)); //字体样式
        title.setHorizontalAlignment(SwingConstants.CENTER); //水平居中
        title.setVerticalAlignment(SwingConstants.CENTER); //垂直居中
        north.add(title);
 
        return north;
    }
 
    public JPanel initCenter() {
 
        //定义表单的主体部分，放置到IDEA会话框的中央位置
 
        //一个简单的3行2列的表格布局
        center.setLayout(new GridLayout(3, 2));
 
        //row1：按钮事件触发后将结果打印在这里
        r1.setForeground(new Color(255, 47, 93)); //设置字体颜色
        center.add(r1);
        r2.setForeground(new Color(139, 181, 20)); //设置字体颜色
        center.add(r2);
 
        //row2：姓名+文本框
        center.add(name);
        center.add(nameContent);
 
        //row3：年龄+文本框
        center.add(age);
        center.add(ageContent);
 
        return center;
    }
 
    public JPanel initSouth() {
 
        //定义表单的提交按钮，放置到IDEA会话框的底部位置
 
        JButton submit = new JButton("提交");
        submit.setHorizontalAlignment(SwingConstants.CENTER); //水平居中
        submit.setVerticalAlignment(SwingConstants.CENTER); //垂直居中
        south.add(submit);
 
        return south;
    }
}
```
现在点击下runIde按钮，同样的，在调试IDE里点击“表单测试”，然后就会弹出如下表单框：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114437-450dcf4d-0ff8-4b49-beeb-02750f045598.webp#clientId=uc932fb85-3596-4&from=paste&id=ua223c23e&originHeight=550&originWidth=433&originalType=url&ratio=1&status=done&style=none&taskId=ub7571baa-b0ec-41f1-b1d2-4a5eb440c97)<br />除非有特殊情况需要自定义swing样式，否则建议不加任何swing样式，这样自定义的swing界面是会随着IDEA的主题改变而去自适应的，比如将图7中的调试IDE的主题设置成Darcula，自定义的表单也会自适应的变成黑色背景：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114923-1399a014-0812-4247-9631-7813d474ac95.webp#clientId=uc932fb85-3596-4&from=paste&id=u9479f308&originHeight=290&originWidth=497&originalType=url&ratio=1&status=done&style=none&taskId=u7aa821b2-a55d-49d9-bf20-a83f38037c3)
<a name="fQ1tu"></a>
### 5. 事件绑定
定义好了样式，现在给“提交”按钮绑定一个事件，现在改写下`FormTestSwing.initSouth`方法：
```java
public JPanel initSouth() {
 
    //定义表单的提交按钮，放置到IDEA会话框的底部位置
 
    JButton submit = new JButton("提交");
    submit.setHorizontalAlignment(SwingConstants.CENTER); //水平居中
    submit.setVerticalAlignment(SwingConstants.CENTER); //垂直居中
    south.add(submit);
 
    //按钮事件绑定
    submit.addActionListener(e -> {
        //获取到name和age
        String name = nameContent.getText();
        String age = ageContent.getText();
        //刷新r2标签里的内容，替换为name和age
        r2.setText(String.format("name:%s, age:%s", name, age));
    });
 
    return south;
}
```
现在再来点击下“提交”按钮，就可以输出表单内容了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114593-cca81b87-d988-4ca4-80e2-a7f3bc233634.webp#clientId=uc932fb85-3596-4&from=paste&id=ub30403f5&originHeight=360&originWidth=482&originalType=url&ratio=1&status=done&style=none&taskId=uc334ffdb-1641-4bb8-a563-cd43ab6affd)
<a name="bQfNF"></a>
### 6. 插件绑定类：FormAction
之前讲过，这个类是插件的入口，结合上面定义好的表单Dialog，来看下它是怎么写的：
```java
public class FromAction extends AnAction {
    @Override
    public void actionPerformed(@NotNull AnActionEvent e) {
        FormTestDialog formTestDialog = new FormTestDialog(e.getProject());
        formTestDialog.setResizable(true); //是否允许用户通过拖拽的方式扩大或缩小你的表单框，我这里定义为true，表示允许
        formTestDialog.show();
    }
}
```
<a name="vO2mP"></a>
### 7. 插件的打包&安装
截止到第四步，都只是在调试IDE里查看效果，如果一个插件开发完成后，需要被实际的IDEA安装，这个时候就需要借助打包选项来打包插件，点击下面的选项构建插件：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965115356-e79082ef-f323-415a-95f6-851efb7d995d.webp#clientId=uc932fb85-3596-4&from=paste&id=u2af9f299&originHeight=443&originWidth=458&originalType=url&ratio=1&status=done&style=none&taskId=udee50566-f0c3-44fe-8c5f-794f28277c9)<br />构建完成后，查看build包下的distributions目录，里面的zip包就可以直接安装进IDEA：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114827-84e8cd89-4e42-4f39-b4e5-abe84003acce.webp#clientId=uc932fb85-3596-4&from=paste&id=u591bfea7&originHeight=550&originWidth=360&originalType=url&ratio=1&status=done&style=none&taskId=uc51d551b-1270-4bd9-bfd9-9de3c375500)<br />然后选择IDEA的Preferences下的plugins选项，弹出如下框，按照图里的指示选择zip包安装即可：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965114880-6d224295-87d0-4ce9-961c-04039302abce.webp#clientId=uc932fb85-3596-4&from=paste&id=u066961fb&originHeight=439&originWidth=1021&originalType=url&ratio=1&status=done&style=none&taskId=u8ac1d9d3-7111-4912-9918-6408b4ababb)<br />然后安装完成，重启IDEA即可：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965115133-01250251-378a-44b9-b458-0de3f4d0e8bc.webp#clientId=uc932fb85-3596-4&from=paste&id=u58f72093&originHeight=312&originWidth=1024&originalType=url&ratio=1&status=done&style=none&taskId=ud2fc8d25-5851-4469-aa25-aadda072d46)<br />各个展示模块对应插件项目里配置的来源参考下图：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965115073-9c20c8db-5ba5-4137-96f5-3fde76407c3b.webp#clientId=uc932fb85-3596-4&from=paste&id=u56e8ab03&originHeight=606&originWidth=983&originalType=url&ratio=1&status=done&style=none&taskId=u4433c492-4107-40a5-879d-b5c8098a50d)<br />重启后出现了跟调试IDEA里一样的菜单栏，选中后运行成功：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636965115296-9bd3514f-5f6c-4f76-b2c2-da1a56deb6fa.webp#clientId=uc932fb85-3596-4&from=paste&id=u306ac56e&originHeight=550&originWidth=466&originalType=url&ratio=1&status=done&style=none&taskId=uae2762f4-e267-4557-b3d8-2b924d75fef)<br />更多细节请看IDEA官方文档：[https://plugins.jetbrains.com/docs/intellij/welcome.html?from=jetbrains.org](https://plugins.jetbrains.com/docs/intellij/welcome.html?from=jetbrains.org)
<a name="oGT2Z"></a>
## 参考文档

- http://www.jetbrains.org/intellij/sdk/docs/tutorials/build_system/prerequisites.html
- http://www.jetbrains.org/intellij/sdk/docs/user_interface_components/tool_windows.html
- http://www.jetbrains.org/intellij/sdk/docs/user_interface_components/dialog_wrapper.html
- https://intellij-support.jetbrains.com/hc/en-us/community/posts/360003338799-Build-compatible-plugin
