Github<br />**前言**<br />在使用GitHub时，看到好的项目或想给某个项目做贡献，此时通常会将代码仓库fork到自己的账号下。如果在此期间，如果源仓库的代码发生了变动，就需要与源仓库代码进行同步。这里实操一下，如何实现这一操作。
<a name="wgmaV"></a>
### **配置项目的上游仓库**
首先需要大家将fork的仓库代码clone到本地，后面的所有操作都是基于本地代码库来进行操作的。比如，可以通过git clone先将fork的代码下载到本地：
```bash
git clone git@github.com:secbr/nacos.git
```
后续的一步步操作，都是基于本地仓库来进行操作。
<a name="uT5tD"></a>
### **进入到本地仓库目录**
通过cd操作，进入到clone下来的本地仓库根目录：
```bash
cd /Users/apple/develop/nacos-request/nacos
```
后续的操作无特殊说明，都是在这个本地仓库的目录下进行操作。
<a name="eZo8z"></a>
### **查看远程仓库路径**
执行命令 `git remote -v` 查看远程仓库的路径：
```bash
appledeMacBook-Pro-2:nacos apple$ git remote -v
origin  https://github.com/secbr/nacos.git (fetch)
origin  https://github.com/secbr/nacos.git (push)
```
如果只显示2行内容，说明该项目还未设置upstream（中文叫：上游代码库），一般情况下，设置好一次upstream后就无需重复设置。通过显示远程仓库的路径和clone时的路径对照，会发现，此时远程仓库的路径还是fork项目的路径。
<a name="zWobK"></a>
### **添加upstream路径**
执行命令 `git remote add upstream https://xxx.git`，把fork的源仓库设置为 upstream 。这里项目是从alibaba的nacos仓库fork过来的，因此对应的upstream就是alibaba的源仓库地址。<br />执行上述命令，在此执行`git remote -v` 检查是否成功。
```bash
appledeMacBook-Pro-2:nacos apple$ git remote add upstream https://github.com/alibaba/nacos.git
appledeMacBook-Pro-2:nacos apple$ git remote -v
origin  https://github.com/secbr/nacos.git (fetch)
origin  https://github.com/secbr/nacos.git (push)
upstream        https://github.com/alibaba/nacos.git (fetch)
upstream        https://github.com/alibaba/nacos.git (push)
```
通过上面的输出可以看出，多了两项upstream的地址，说明添加upstream成功。
<a name="yXwfb"></a>
### **检查本地代码状态**
由于实例是直接从仓库clone下来的，本地还没有修改代码。如果本地项目已经修改了一些代码，不确定是否提交了代码，就需要执行git status来检查一下。
```bash
appledeMacBook-Pro-2:nacos apple$ git status
On branch develop
Your branch is up to date with 'origin/develop'.

nothing to commit, working tree clean
```
上面显示，本地没有需要提交的（commit）的代码。如果本地有修改，需要先从本地仓库推送到GitHub仓库。然后，再执行一次 `git status` 检查。<br />对应推送到GitHub仓库的基本操作步骤如下：
```bash
git add -A 或者 git add filename
git commit -m "your note"
git push origin master
git status
```
完成上面的基本操作之后，确认代码都已经提交，便可以开始执行源仓库与本地仓库的merge操作了。
<a name="syicO"></a>
### **抓取源仓库的更新**
经过上面步骤的准备之后，可以进行源仓库的代码更新了。执行命令 `git fetch upstream` 抓取原仓库的更新：
```bash
appledeMacBook-Pro-2:nacos apple$ git fetch upstream
remote: Enumerating objects: 2646, done.
remote: Counting objects: 100% (2593/2593), done.
remote: Compressing objects: 100% (1157/1157), done.
remote: Total 2646 (delta 731), reused 2404 (delta 682), pack-reused 53
Receiving objects: 100% (2646/2646), 1.67 MiB | 1.47 MiB/s, done.
Resolving deltas: 100% (734/734), completed with 37 local objects.
From https://github.com/alibaba/nacos
 * [new branch]          0.2.1                               -> upstream/0.2.1
 * [new branch]          0.2.2                               -> upstream/0.2.2
 * [new branch]          0.3.0                               -> upstream/0.3.0
//...省略一部分
```
执行上述命令之后，上游仓库的更新（commit）会本存储为本地的分支，通常名称为：upstream/BRANCHNAME。比如上面的upstream/0.3.0。
<a name="HlkpH"></a>
### **切换分支**
完成了上游仓库分支的拉取之后，先来核查一下本地仓库当前处于哪个分支，也就是需要更新合并的分支。比如，这里需要将develop分支的内容更新到与上游仓库代码一致。则先切换到develop分支：
```bash
appledeMacBook-Pro-2:nacos apple$ git checkout develop
Already on 'develop'
Your branch is up to date with 'origin/develop'.
```
上面提示已经是develop分支了。
<a name="xbkLx"></a>
### **执行合并**
执行命令 `git merge upstream/develop` 合并远程的develop分支。比如合并的可能是master，可根据需要将develop的名称替换成对应的master。
```bash
appledeMacBook-Pro-2:nacos apple$ git merge upstream/develop
Removing test/src/test/java/com/alibaba/nacos/test/naming/DeregisterInstance_ITCase.java
// ...省略一部分
Removing naming/src/test/java/com/alibaba/nacos/naming/core/PushServiceTest.java
Auto-merging client/src/main/java/com/alibaba/nacos/client/naming/remote/http/NamingHttpClientProxy.java
CONFLICT (content): Merge conflict in client/src/main/java/com/alibaba/nacos/client/naming/remote/http/NamingHttpClientProxy.java
Removing client/src/main/java/com/alibaba/nacos/client/naming/core/HostReactor.java
Removing .editorconfig
Automatic merge failed; fix conflicts and then commit the result.
```
执行完上述命令之后，会发现上游代码指定分支的修改内容已经反映到本地代码了。
<a name="Le0fH"></a>
### **上传代码到fork分支**
执行完上一步的合并操作之后，往后还有一些后续处理，比如代码冲突。如果本地修改了内容，上游仓库也修改了对应的代码，则可能会出现冲突。这时就需要对比代码进行修改。本人更习惯使用IDEA中可视化的插件进行代码冲突解决，也可以选择喜欢的方式进行解决。<br />解决完冲突之后，就可以执行正常的代码add、commit和push操作了。这里的一系列操作都是针对自己fork的仓库的，对应操作实例如下：
```bash
appledeMacBook-Pro-2:nacos apple$ git add .
appledeMacBook-Pro-2:nacos apple$ git commit -m 'merge from nacos'
[develop 8601c1791] merge from nacos 
appledeMacBook-Pro-2:nacos apple$ git push
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Delta compression using up to 12 threads
Compressing objects: 100% (2/2), done.
Writing objects: 100% (2/2), 281 bytes | 281.00 KiB/s, done.
Total 2 (delta 1), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (1/1), completed with 1 local object.
To https://github.com/secbr/nacos.git
   76a4dcbb1..8601c1791  develop -> develop
```
上述操作，通过add、commit、push一系列操作，将源仓库中的修改内容，提交到自己fork的分支当中了。此时再查看自己fork的GitHub仓库，可以发现代码已经更新。
