commit message 的提交很多时候都只依赖开发人员的自我规范，而开发人员往往在需求紧急或者 bug 要及时修复的时候，根本不会花很多时间在写 git commit message 的信息。而且就算是写，每个人的风格也不一样，所以写出来的 message 也不完全相同。<br />这个时候就需要有一套规范了，现在业界比较常用的规范是的格式是这样的：type(scope):subject，下面详细来聊一下。
<a name="sIrT2"></a>
## Type
type 代表的是提交内容的一种类型，每一种类型都代表着不同的含义，具体的类型取值和含义如下：

1. feat：表示开发一个新的需求特性；
2. fix：表示修复一个 bug；
3. docs：表示是针对文档的修改，并没有修改代码；
4. style：格式修改，不影响代码功能；
5. refactor：不是进行 feat 和 fix 的代码修改，重构功能；
6. perf：提升性能的代码修改；
7. test：添加测试代码或者修正已经存在的测试功能代码；
8. build：修改会影响构建或者依赖的代码；
9. ci：修改集成配置的文件或者脚本；
10. chore：一些不够影响到源码和测试文件的修改；
11. revert：针对之前的一个提交的 revert 修改；

在写一个 git commit 的时候，要搞清楚当前提交的内容的真正含义是什么，从而选择正确的类型。此外还要求对于代码的修改需要尽量细粒度，话句话说就是尽量将一个大的改动进行拆分，根据适当的情况进行 git 提交，避免一次性提交太多的改动。
<a name="uTvAk"></a>
## Scope
scope 表示的当次 git 提交的内容影响的范围，这个范围比较宽泛，比如可以是 DAO 层，Controller 层，或者是具有特定功能的比如 utils 工具模块，权限模块，数据模块等等，只要能跟自己的项目挂上钩，表达出修改的范围就行，如果涉及到的范围比较多的话，可以用 * 表示，并不强制要求。
<a name="KLsww"></a>
## Subject
subject 部分是最重要的 git commit message 的部分，也就是经常要写提交信息的部分，这一部分通常会一个言简意赅的信息描述，需要写出改动代码的原因。<br />上面的 type，scope，subject 三个部分是常用的部分，不过有些规范将 git 的提交规范定义为 Header，Body 和 Footer 三个部分，而 type，scope，subject 三个属于 Header 的部分。
<a name="Vj2rl"></a>
## 扩展
Header 部分也就是上面提到的三个部分，是每个 git 提交的基础内容；Body 部分则是更加详细的描述信息，用于完整记录代码的修改地方和逻辑；Footer 部分则会将本次提交的内容与具体的需求或者缺陷相关联，比如对应的需求地址是什么，或者修复的 Bug 缺陷是什么等。
<a name="LFJ7M"></a>
## IDEA 插件
上面的内容不多，但是要记下来的还是很繁琐的，特别是有时候很难记住所有的 type 类型，好在 IDEA 现在有一个插件，就是用来规范 git 提交模板的。<br />在 IDEA 的插件市场中安装 git commit template，直接搜索安装，然后重启 IDEA 即可。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689493381637-29a7aae0-edeb-47c8-bf95-5657759f50e9.png#averageHue=%232a2d30&clientId=u5b684c76-9426-4&from=paste&id=u8dc4499a&originHeight=619&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua279a844-265d-4d58-b4de-b972d57e7f2&title=)<br />安装完成过后，在需求提交代码的时候，会出现这个按钮。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689493381614-7a2ab241-954d-4fc7-8e35-ebe706dd765c.png#averageHue=%232a402a&clientId=u5b684c76-9426-4&from=paste&id=u8cb442ae&originHeight=638&originWidth=1076&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u49f7cfad-c407-4fbb-9a6a-be96269fa21&title=)<br />点击一下就可以看到下面这个页面，其中 short description 就是上面提到的 subject，而 Long description 代表的就是 Body 部分，而下面的 Breaking changes 和 Closed issues 则代表的是 Footer 部分，在使用的过程中按需填入即可。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1689493381598-e2193196-956f-43b8-9185-fc0b56284193.png#averageHue=%23323537&clientId=u5b684c76-9426-4&from=paste&id=u63901ad2&originHeight=1018&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue3b2e2f9-2bce-487d-b3a5-899becb2d6f&title=)
