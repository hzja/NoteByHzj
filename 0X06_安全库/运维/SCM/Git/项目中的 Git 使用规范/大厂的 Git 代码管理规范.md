<a name="pT1Pr"></a>
## 分支命名
<a name="BYJOY"></a>
### master 分支
master 为主分支，也是用于部署生产环境的分支，需要确保 master 分支稳定性。master 分支一般由 release 以及 hotfix 分支合并，任何时间都不能直接修改代码。
<a name="a5N3c"></a>
### develop 分支
develop 为开发环境分支，始终保持最新完成以及 bug 修复后的代码，用于前后端联调。一般开发新功能时，feature 分支都是基于 develop 分支创建的。
<a name="a6Yo7"></a>
### feature 分支
开发新功能时，以 develop 为基础创建 feature 分支。<br />分支命名时以 feature/ 开头，后面可以加上开发的功能模块， 命名示例：feature/user_module、feature/cart_module。
<a name="hnmOW"></a>
### test 分支
test 为测试环境分支，外部用户无法访问，专门给测试人员使用，版本相对稳定。
<a name="A8W2u"></a>
### release 分支
release 为预上线分支（预发布分支），UAT 测试阶段使用。一般由 test 或 hotfix 分支合并，不建议直接在 release 分支上直接修改代码。
<a name="UefJg"></a>
### hotfix 分支
线上出现紧急问题时，需要及时修复，以 master 分支为基线，创建 hotfix 分支。修复完成后，需要合并到 master 分支和 develop 分支。<br />分支命名以hotfix/ 开头的为修复分支，它的命名规则与 feature 分支类似。
<a name="oYpGi"></a>
## 分支与环境对应关系
在系统开发过程中常用的环境：

- DEV 环境（Development environment）：用于开发者调试使用。
- FAT 环境（Feature Acceptance Test environment）：功能验收测试环境，用于测试环境下的软件测试者测试使用。
- UAT 环境 （User Acceptance Test environment）：用户验收测试环境，用于生产环境下的软件测试者测试使用。
- PRO 环境（Production environment）：生产环境。

对应关系：

| 分支 | 功能 | 环境 | 可访问 |
| --- | --- | --- | --- |
| master | 主分支，稳定版本 | PRO | 是 |
| develop | 开发分支，最新版本 | DEV | 是 |
| feature | 开发分支，实现新特性 | <br /> | 否 |
| test | 测试分支，功能测试 | FAT | 是 |
| release | 预上线分支，发布新版本 | UAT | 是 |
| hotfix | 紧急修复分支，修复线上bug | <br /> | 否 |

<a name="K7O2Y"></a>
### 分支合并流程规范
业界常见的两大主分支（master、develop）、三个辅助分支（feature、release、hotfix）的生命周期：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695349227366-95566484-646a-48b9-afdc-5c932d83ea7c.png#averageHue=%23efefee&clientId=u189d8af5-4215-4&from=paste&id=u8b9e592e&originHeight=795&originWidth=600&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a28cdb0-8933-40de-b974-75e9d89dc8c&title=)<br />以上生命周期仅作参考，不同开发团队可能有不同的规范，可自行灵活定义。<br />例如团队在开发时，至少需要保证以下流程：

- develop 分支和 hotfix 分支，必须从 master 分支检出。
- 由 develop 分支合并到 test 分支。
- 功能测试无误后，由 test 分支合并到 release 分支。
- UAT 测试通过后，由 release 分支合并到 master 分支。
- 对于工作量小的功能开发（工时小于 1 天），可以直接在 devolop 分支进行开发，否则由 develop 分支检出 feature 分支进行开发，开发完后合并到develop 分支。
<a name="jx3vF"></a>
## Git Commit Message 规范
Git Commit Message 规范指提交代码时编写的规范注释，编写良好的 Commit Message 可以达到 3 个重要的目的：

- 加快代码 review 的流程。
- 编写良好的版本发布日志。
- 让之后的维护者了解代码里出现特定变化和 feature 被添加的原因。
<a name="nA5C5"></a>
### Angular Git Commit Guidelines
业界应用的比较广泛的是 Angular Git Commit Guidelines：
```
<type>(<scope>): <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

- type：提交类型。
- scope：可选项，本次 commit 波及的范围。
- subject：简明扼要地阐述下本次 commit 的主旨，在Angular Git Commit Guidelines中强调了三点。使用祈使句，首字母不要大写，结尾无需添加标点。
- body：同样使用祈使句，在主体内容中需要把本次 commit 详细地描述一下，比如此次变更的动机。
- footer：描述下与之关联的 issue 或 break change。
<a name="jVp2V"></a>
### 简易版
项目中实际可以采用简易版规范：
```
<type>(<scope>):<subject>
```
<a name="Lui1w"></a>
### type 规范
Angular Git Commit Guidelines中推荐的 type 类型如下：

- feat：新增功能。
- fix：修复 bug。
- docs：仅文档更改。
- style：不影响代码含义的更改（空白、格式设置、缺失分号等）。
- refactor：既不修复 bug 也不添加特性的代码更改。
- perf：改进性能的代码更改。
- test：添加缺少的测试或更正现有测试。
- chore：对构建过程或辅助工具和库（如文档）的更改。

除此之外，还有一些常用的类型：

- delete：删除功能或文件。
- modify：修改功能。
- build：改变构建流程，新增依赖库、工具等（例如 webpack、gulp、npm 修改）。
- test：测试用例的新增、修改。
- ci：自动化流程配置修改。
- revert：回滚到上一个版本。
<a name="hbhoV"></a>
### 单次提交注意事项

- 提交问题必须为同一类别。
- 提交问题不要超过 3 个。
- 提交的 commit 发现不符合规范，`git commit --amend -m "新的提交信息"`或 `git reset --hard HEAD` 重新提交一次。
<a name="sxaiO"></a>
## 配置 .gitignore 文件
.gitignore是一份用于忽略不必提交的文件的列表，项目中可以根据实际需求统一.gitignore文件，减少不必要的文件提交和冲突，净化代码库环境。<br />通用文件示例：
```
HELP.md
target/
!.mvn/wrapper/maven-wrapper.jar
!**/src/main/**/target/
!**/src/test/**/target/

### STS ###
.apt_generated
.classpath
.factorypath
.project
.settings
.springBeans
.sts4-cache

### IntelliJ IDEA ###
.idea
*.iws
*.iml
*.ipr

### NetBeans ###
/nbproject/private/
/nbbuild/
/dist/
/nbdist/
/.nb-gradle/
build/
!**/src/main/**/build/
!**/src/test/**/build/

### VS Code ###
.vscode/

# Log file
*.log
/logs*

# BlueJ files
*.ctxt

# Mobile Tools for Java (J2ME)
.mtj.tmp/

# Package Files #
*.jar
*.war
*.ear
*.zip
*.tar.gz
*.rar
*.cmd
```
<a name="APJpp"></a>
## 其他
此外，还有一些其他建议：

- master 分支的每一次更新，都建议打 tag 添加标签，通常为对应版本号，便于管理。
- feature 分支、hotfix 分支在合并后可以删除，避免分支过多，管理混乱。
- 每次 pull 代码前，提交本地代码到本地库中，否则可能会出现合并代码出错，导致代码丢失。
