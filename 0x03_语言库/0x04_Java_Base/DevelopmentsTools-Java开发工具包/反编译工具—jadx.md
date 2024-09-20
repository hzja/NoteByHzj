反编译<br />jadx 是一款功能强大的反编译工具，使用起来简单方便（拖拽式操作），不光提供了命令行程序，还提供了 GUI 程序。一般情况下，直接使用 GUI 程序就可以了。<br />jadx 支持 Windows、Linux、 macOS，能打开.apk, .dex, .jar,.zip等格式的文件<br />就比如说需要反编译一个 jar 包查看其源码的话，直接将 jar 包拖入到 jadx 中就可以了。效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696465-9ba01551-86a1-45aa-8973-dcf22a815ec4.webp#averageHue=%23e0e4eb&clientId=u36f5bb46-be06-4&from=paste&id=u213ef400&originHeight=646&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29a137cb-2573-4069-8c32-66d5e477006&title=)<br />再比如说想看看某个 apk 的源码，拿到 apk 之后直接拖入进 jadx 中就可以了。效果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696342-886bb3c7-62bd-4955-9122-9ab68814130b.webp#averageHue=%23edf1f2&clientId=u36f5bb46-be06-4&from=paste&id=u4e2b0a2c&originHeight=469&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0b7b17d2-80d0-499d-a9f5-645c84a8aaf&title=)
<a name="Dde5H"></a>
## jadx 安装
jadx 是一款开源软件，是可以免费使用的。可以在 jadx 的项目主页下载 jadx 最新版。

- 项目地址：[https://github.com/skylot/jadx](https://github.com/skylot/jadx)
- 下载地址：[https://github.com/skylot/jadx/releases/tag/v1.3.1](https://github.com/skylot/jadx/releases/tag/v1.3.1)

直接下载第一个即可。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696495-c7bce5c1-435f-498a-a495-a9d96e03b5b7.webp#averageHue=%23959b96&clientId=u36f5bb46-be06-4&from=paste&id=ub2605aa1&originHeight=1026&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u547a6f6f-1704-40c7-990f-08bc1c671a9&title=)<br />下载之后，解压下载好的 jadx 压缩文件后进入 bin 目录即可找到可执。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696562-b562d6e0-77fa-4341-b98b-53ec793d6691.webp#averageHue=%23e7efee&clientId=u36f5bb46-be06-4&from=paste&id=ua1a86e72&originHeight=386&originWidth=826&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf91b61ba-fd64-4aef-a9ef-95dd6f45437&title=)

- jadx：命令行版本
- jadx-gui：图形操作界面版本

也可以自己克隆源码，本地编译，这也是比较推荐的方式。
```bash
git clone https://github.com/skylot/jadx.git
```
jadx 由 Java 语言编写，使用 Gradle 进行构建。克隆到本地之后，可以直接使用 Gradle 命令进行构建：
```bash
cd jadx
# Windows 平台使用 gradlew.bat 而不是 ./gradlew
./gradlew dist
```
也可以直接使用 IDE 打开，然后像运行普通 Java 程序那样使用它：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696591-357a46ac-d7e4-4059-b421-05a7f85a3150.webp#averageHue=%232e333d&clientId=u36f5bb46-be06-4&from=paste&id=u41604108&originHeight=575&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2679033c-ff41-4ac2-bbc0-b6cb45b3129&title=)
<a name="JMfpB"></a>
## jadx 使用
<a name="VzrY2"></a>
### 反编译文件
通过 File -> Open files... 打开需要反编译的文件或者直接将文件拖拽进 jadx 中就可以了。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696824-e3e01677-9915-4aaa-87f3-efc2e0426ea6.webp#averageHue=%23e2e5e7&clientId=u36f5bb46-be06-4&from=paste&id=u7ae1d917&originHeight=480&originWidth=1024&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6ec176b9-9879-4a9e-aa8a-7dfd2398375&title=)<br />从上图可以看出，jadx 支持.apk, .dex, .jar,.zip,.class等格式的文件。
<a name="tuzfn"></a>
### 搜索功能
jadx 自带强大的搜索功能，支持多种匹配模式。<br />通过 Navigation 即可打开搜索功能，可以选择搜索指定的类，方法，属性，代码，文件，甚至是注释。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696806-fe0c7292-17de-4be8-8c1b-4a82a7bcf072.webp#averageHue=%23e4edec&clientId=u36f5bb46-be06-4&from=paste&id=ub847a8b5&originHeight=722&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7116b780-fea3-436c-831d-0cf09ea23a9&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946696948-1c71e630-7405-4d47-aa0a-4e0b2c79ae4c.webp#averageHue=%23f0f1f4&clientId=u36f5bb46-be06-4&from=paste&id=uca8010bb&originHeight=679&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa006a99-34f0-43a9-80c4-3f509cf31e8&title=)
<a name="EtRDf"></a>
### 查看类，变量或者方法使用情况
对于某个类、变量或者方法，还可以查看哪些地方使用了它。<br />直接选中对应的类、变量或者方法，然后点击右键选择 Find Usage 即可。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946697048-84aaa2e5-188d-4b63-bc4c-ae35823215c0.webp#averageHue=%2399a9dc&clientId=u36f5bb46-be06-4&from=paste&id=u0ba8ec4f&originHeight=393&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61f26878-bb01-4e71-8465-b565958158c&title=)<br />很快，jadx 就会找出整个项目有哪些地方使用了它。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946697167-434e2802-7d6f-40b4-b844-a4ed7b91df71.webp#averageHue=%23d3e0d1&clientId=u36f5bb46-be06-4&from=paste&id=uc164e731&originHeight=720&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaeb7ecfe-b4fa-4d33-bfe8-6a3029d5504&title=)
<a name="tordE"></a>
### 添加注释
还可以自定义注释到源代码中。<br />选中对应的位置之后，点击右键选择 Comment 即可。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946697181-27c9c0cf-3ac8-4c5b-a059-9f2a9b69f3df.webp#averageHue=%23d7e0ea&clientId=u36f5bb46-be06-4&from=paste&id=u1918a76d&originHeight=468&originWidth=792&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u15f19581-b0e9-44d3-bbe1-0a45654a970&title=)
<a name="SvSfI"></a>
### 反混淆
一般情况下，为了项目的安全，在打包发布一个 apk 之前都会对其代码进行混淆加密比如用无意义的短变量去重命名类、变量、方法，以免代码被轻易破解泄露。<br />经过混淆的代码在功能上是没有变化的，但是去掉了部分名称中的语义信息。<br />为了代码的易读性，可以对代码进行反混淆。<br />在 jadx 中，通过 Tools -> Deobfuscation 即可开启反混淆功能。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641946697513-4db4b07c-0bcb-47b9-822c-c95452ed31de.webp#averageHue=%23e0e7e1&clientId=u36f5bb46-be06-4&from=paste&id=u5cacbd02&originHeight=389&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2a5cec59-f189-4a73-ab55-41879c7f850&title=)
