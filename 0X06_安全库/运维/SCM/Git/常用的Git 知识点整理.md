Git
<a name="AJRzm"></a>
## 1、Git基本概念

- repository
- config
- init
- clone
- fetch
- pull
- commit
- push
- branch
- head
- tag
- merge
- conflict
- diff
- log
- show
- status
<a name="moANP"></a>
## 2、Git工作空间和文件状态
<a name="LOqN1"></a>
### (1).工作空间
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642658392994-5fd832d9-6e7d-49bc-977b-a300f2f37b31.webp#clientId=u2eb0fc30-c29f-4&from=paste&id=u648e7533&originHeight=438&originWidth=917&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4a010d11-8f23-40ee-8d31-7c241fa37a2&title=)<br />左侧为工作区，右侧为版本库。

- 工作区（Working Directory） 就是在电脑里能看到的目录，比如learngit文件夹就是一个工作区。
- 版本库（Repository）工作区有一个隐藏目录.git，是Git的版本库。

在版本库中标记为index的区域为暂存区，标记为master的是Git自动创建的第一个分支，代表的是目录树。此时HEAD实际是指向master分支的一个“游标”，所以图示的命令中出现HEAD的地方可以用master来替换。图中的objects标识的区域为git的对象库，实际位于.git/objects目录下。

- 当对工作区修改（或新增）的文件执行`git add`命令时，暂存区的目录树会被更新，同时工作区修改（或新增）的文件内容会被写入到对象库中的一个新的对象中，而该对象的id被记录在暂存区的文件索引中。
- 当执行提交操作`git commit`时，暂存区的目录树会写到版本库（对象库）中，master分支会做相应的更新，即master最新指向的目录树就是提交时原暂存区的目录树。
- 当执行`git reset HEAD`命令时，暂存区的目录树会被重写，会被master分支指向的目录树所替换，但是工作区不受影响。
- 当执行`git rm --cached`命令时，会直接从暂存区删除文件，工作区则不做出改变。
- 当执行`git checkout .`或`git checkout --`命令时，会用暂存区全部的文件或指定的文件替换工作区的文件。这个操作很危险，会清楚工作区中未添加到暂存区的改动。
- 当执行`git checkout HEAD .`或`git checkout HEAD`命令时，会用HEAD指向的master分支中的全部或部分文件替换暂存区和工作区中的文件。这个命令也是极度危险的。因为不但会清楚工作区中未提交的改动，也会清楚暂存区中未提交的改动。
<a name="SSfrZ"></a>
### (2).文件状态
Git 有三种状态，文件可能处于其中之一：已提交(`committed`)、已修改(`modified`)和已暂存(`staged`)。
<a name="e43Ve"></a>
## 3、Git配置系统级、全局、当前仓库用户名、邮箱的命令
系统级、全局、当前仓库选项分别是：仓库`-system`、`-global`、`-local`(或默认不填)
```bash
git config --global user.name "Fcant"
git config --global user.email "fcscanf@outlook.com"
```
列出Git设置
```bash
git config --list
git config -l
```
<a name="q2UXN"></a>
## 4、Git fetch和pull的区别

- `git fetch`：相当于是从远程获取最新版本到本地，不会自动merge.
- `git pull`：相当于是从远程获取最新版本并merge到本地.
<a name="SAdTi"></a>
### (1). `git fetch`示例：
Git fetch origin master
```bash
git log -p master..origin/master
git merge origin/master
```
以上命令的含义：

- 首先从远程的`origin`的`master`主分支下载最新的版本到origin/master分支上
- 然后比较本地的`master`分支和origin/master分支的差别
- 最后进行合并
- 上述过程其实可以用以下更清晰的方式来进行：
<a name="CxFLn"></a>
### (2). `git pull`示例：
```bash
git pull origin master
```
上述命令其实相当于`git fetch`和`git merge`。在实际使用中，`git fetch`更安全一些，因为在`merge`前，可以查看更新情况，然后再决定是否合并。
<a name="VHG4B"></a>
## 5、Git reset和`revert`的区别

- `git revert`是用一次新的`commit`来回滚之前的`commit`，`git reset`是直接删除指定的`commit`。
- 在回滚这一操作上看，效果差不多。但是在日后继续`merge`以前的老版本时有区别。因为`git revert`是用一次逆向的`commit`“中和”之前的提交，因此日后合并老的branch时，导致这部分改变不会再次出现，但是`git reset`是之间把某些`commit`在某个branch上删除，因而和老的branch再次`merge`时，这些被回滚的`commit`应该还会被引入。
- `git reset`是把HEAD向后移动了一下，而`git revert`是HEAD继续前进，只是新的`commit`的内容和要`revert`的内容正好相反，能够抵消要被`revert`的内容。
- `git revert`与`git reset`最大的不同是，`git revert` 仅仅是撤销某次提交。

另外，说一下`git revert`， `git reset –hard`和 `–soft`的区别

- `git reset –mixed id`：是将git的HEAD变了（也就是提交记录变了），但文件并没有改变，（也就是working tree并没有改变）。
- `git reset –soft id`：实际上，是`git reset –mixed id`后，又做了一次`git add`。
- `git reset –herd id`：是将git的HEAD变了，文件也变了。
<a name="M6Olo"></a>
## 6、Git merge和reabse的相同点和不同点
`merge`是合并的意思，`rebase`是复位基底的意思，相同点都是用来合并分支的。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642658393145-bff9ba95-3fe9-4e93-a8f4-a2cfb47d7ea7.webp#clientId=u2eb0fc30-c29f-4&from=paste&id=u89ba375c&originHeight=368&originWidth=895&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u23d5208c-4f63-4da2-90a6-8dfa5abea2d&title=)<br />不同点：

- `merge`操作会生成一个新的节点，之前的提交分开显示。而`rebase`操作不会生成新的节点，是将两个分支融合成一个线性的提交。
- 解决冲突时。merge操作遇到冲突的时候，当前merge不能继续进行下去。手动修改冲突内容后，add 修改，commit就可以了。而`rebase`操作的话，会中断rebase,同时会提示去解决冲突。解决冲突后,将修改add后执行`git rebase –continue`继续操作，或者`git rebase –skip`忽略冲突。
- `git pull`和`git pull --rebase`区别：`git pull`做了两个操作分别是”获取”和”合并”。所以加了rebase就是以rebase的方式进行合并分支，默认为merge。

总结：选择 merge 还是 rebase？

- merge 是一个合并操作，会将两个分支的修改合并在一起，默认操作的情况下会提交合并中修改的内容
- merge 的提交历史忠实地记录了实际发生过什么，关注点在真实的提交历史上面
- rebase 并没有进行合并操作，只是提取了当前分支的修改，将其复制在了目标分支的最新提交后面
- rebase 的提交历史反映了项目过程中发生了什么，关注点在开发过程上面
- merge 与 rebase 都是非常强大的分支整合命令，没有优劣之分，使用哪一个应由项目和团队的开发需求决定
- merge 和 rebase 还有很多强大的选项，可以使用 git help 查看
<a name="Ovtil"></a>
## 7、Git stash是什么？它的相关使用方式命令

- `git stash`：备份当前的工作区的内容，从最近的一次提交中读取相关内容，让工作区保证和上次提交的内容一致。同时，将当前的工作区内容保存到Git栈中。
- `git stash pop`：从Git栈中读取最近一次保存的内容，恢复工作区的相关内容。由于可能存在多个Stash的内容，所以用栈来管理，pop会从最近的一个stash中读取内容并恢复。
- `git stash pop –index stash@{0}`：恢复编号为0的进度的工作区和暂存区。
- `git stash apply stash@{1}` 以将指定版本号为`stash@{1}`的工作取出来
- `git stash drop[]` 删除某一个进度，默认删除最新进度
- `git stash list`：显示Git栈内的所有备份，可以利用这个列表来决定从那个地方恢复。
- `git stash clear`：清空Git栈。此时使用gitg等图形化工具会发现，原来stash的哪些节点都消失了
```bash
# 恢复工作进度
git stash pop [--index] [<stash>]
--index 参数：不仅恢复工作区，还恢复暂存区
<stash> 指定恢复某一个具体进度。如果没有这个参数，默认恢复最新进度

# 这是git stash保存进度的完整命令形式
git stash [save message] [-k|--no-keep-index] [--patch]
-k和--no-keep-index指定保存进度后，是否重置暂存区
--patch 会显示工作区和HEAD的差异,通过编辑差异文件，排除不需要保存的内容。和git add -p命令类似
```
使用save可以对进度添加备注
```bash
# git stash save "这是保存的进度"
```
<a name="HH1f5"></a>
## 8、Git只从暂存区删除，从工作空间删除的命令分别是什么?
```bash
git rm --cached

git rm
git commit
```
<a name="D0wjJ"></a>
## 9、Git标签的使用
```bash
# 列出现有的标签
git tag

# 打标签
git tag -a v1.01 -m "Relase version 1.01"

# 查看相应标签的版本信息
git show v1.4
```

- `-a` 选项,创建一个含附注类型的标签
- `-m` 选项,指定了对应的标签说明
<a name="ExJOK"></a>
## 9、Git分支的使用
```bash
# 查看本地分支
git branch

# 查看远程分支
git branch -r

# 创建本地分支(注意新分支创建后不会自动切换为当前分支)
git branch [name]

# 切换分支
git checkout [name]

# 创建新分支并立即切换到新分支
git checkout -b [name]

# 强制删除一个分支
git branch -D [name]

# 合并分支(将名称为[name]的分支与当前分支合并)
git merge [name]

# 查看各个分支最后提交信息
git br -v

# 查看已经被合并到当前分支的分支
git br --merged

# 查看尚未被合并到当前分支的分支
git br --no-merged
```
<a name="yAJUQ"></a>
## 10、介绍Git冲突处理经验，以及`merge`和`rebase`中的ours和theirs的差别。
merge和rebase对于ours和theirs的定义是完全相反的。在merge时，ours指代的是当前分支，theirs代表需要被合并的分支。而在rebase过程中，ours指向了修改参考分支，theirs却是当前分支。因为rebase 隐含了一个`git checkout upstream`的过程，将HEAD从local分支变成了upstream分支。git会在rebase结束后撤销这个改变，但它已经不可避免地影响了冲突的状态，使rebase中ours和theirs的定义与merge 截然相反。因此，在使用ours与theirs时请格外小心。
<a name="vdrQr"></a>
## 11. Git远程操作相关
<a name="sGMki"></a>
### (1). clone
```bash
git clone <版本库的网址>
git clone <版本库的网址> <本地目录名>
# 克隆jQuery的版本库
git clone https://github.com/jquery/jquery.git

git clone -o jQuery https://github.com/jquery/jquery.git
```
<a name="f112F"></a>
### (2). remote
```bash
# 列出所有远程主机
git remote

# 使用-v选项，可以参看远程主机的网址
git remote -v

# 可以查看该主机的详细信息
git remote show <主机名>

# 添加远程主机
git remote add <主机名> <网址>

# 删除远程主机
git remote rm <主机名>

# 修改远程主机名称
git remote rename <原主机名> <新主机名>
```
<a name="zYLaZ"></a>
### (3). fetch
```bash
# 取回所有分支(branch)的更新到本地
git fetch <远程主机名>

# 取回某的特定分支的更新
git fetch <远程主机名> <分支名>

# 取回origin主机的master分支的更新
git fetch origin master

# 所取回的更新，在本地主机上要用”远程主机名/分支名”的形式读取。比如origin主机的master，就要用origin/master读取。可以使用git merge命令或者git rebase命令，在本地分支上合并远程分支
git merge origin/master
git rebase origin/master
```
<a name="Iq3B4"></a>
### (4). pull
```bash
git pull <远程主机名> <远程分支名>:<本地分支名>
# 取回origin主机的next分支，与本地的master分支合并
git pull origin next:master

# 如果远程分支是与当前分支合并，则冒号后面的部分可以省略。
git pull origin next

# 上面的命令实质上等同于先做git fetch，再做git merge。
git fetch origin
git merge origin/next

# 合并需要采用rebase模式
git pull --rebase <远程主机名> <远程分支名>:<本地分支名>
```
<a name="MWW3w"></a>
### (5). push
```bash
git push <远程主机名> <本地分支名>:<远程分支名>
```
注意：分支推送顺序的写法是`“<来源地>:<目的地>”`，所以`git pull是“<远程分支>:<本地分支>”`，而`git push是“<本地分支>:<远程分支>”`。

- 如果省略远程分支名，则表示将本地分支推送与之存在”追踪关系”的远程分支(通常两者同名)，如果该远程分支不存在，则会被新建。
- 如果省略本地分支名，则表示删除指定的远程分支，因为这等同于推送一个空的本地分支到远程分支。
```bash
# 将本地的master分支推送到origin主机的master分支。如果后者不存在，则会被新建
git push origin master

# 省略了本地分支，以下等同，删除origin主机的master分支
git push origin :master
git push origin --delete master

# 如果当前分支与远程分支之间存在追踪关系，则本地分支和远程分支都可以省略
git push origin

# 如果当前分支只有一个追踪分支，那么主机名都可以省略。
git push

# 如果当前分支与多个主机存在追踪关系，则可以使用-u选项指定一个默认主机，这样后面就可以不加任何参数使用git push
git push -u origin master

# 不管是否存在对应的远程分支，将本地的所有分支都推送到远程主机
git push --all origin

# 强制推送
git push --force origin

# git push不会推送标签(tag)，除非使用–tags选项
git push origin --tags
```
<a name="hhquO"></a>
## 12、Git Flow使用简介
就像代码需要代码规范一样，代码管理同样需要一个清晰的流程和规范。三种广泛使用的工作流程：

- Git flow
- Github flow
- Gitlab flow

三种工作流程，有一个共同点：都采用”功能驱动式开发”（Feature-driven development，简称FDD）。它指的是，需求是开发的起点，先有需求再有功能分支（feature branch）或者补丁分支（hotfix branch）。完成开发后，该分支就合并到主分支，然后被删除。最早诞生、并得到广泛采用的一种工作流程，就是Git flow。<br />它最主要的特点有两个。首先，项目存在两个长期分支，分别是：主分支master、开发分支develop。其次，项目存在三种短期分支，分别是：功能分支（feature branch）、补丁分支（hotfix branch）、预发分支（release branch），一旦完成开发，它们就会被合并进develop或master，然后被删除。
<a name="jn7fk"></a>
### (1). Git Flow流程图
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642658395201-c2f7d7f4-7dba-42a9-a1a0-e79102fb075f.webp#clientId=u2eb0fc30-c29f-4&from=paste&id=u4818f789&originHeight=678&originWidth=450&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u73dbc7e9-1b80-41cb-aafe-eacade21c19&title=)
<a name="iCrUs"></a>
### (2). Git Flow常用的分支

- Production分支。也就是经常使用的Master分支，这个分支最近发布到生产环境的代码，最近发布的Release， 这个分支只能从其他分支合并，不能在这个分支直接修改。
- Develop分支。这个分支是主开发分支，包含所有要发布到下一个Release的代码，这个主要合并与其他分支，比如Feature分支。
- Feature分支。这个分支主要是用来开发一个新的功能，一旦开发完成，合并回Develop分支进入下一个Release。
- Release分支。当需要一个发布一个新Release的时候，基于Develop分支创建一个Release分支，完成Release后，合并到Master和Develop分支。
- Hotfix分支。当在Production发现新的Bug时候，需要创建一个Hotfix, 完成Hotfix后，合并回Master和Develop分支，所以Hotfix的改动会进入下一个Release。
<a name="jJgHs"></a>
### (3). Git Flow代码示例
<a name="j4v4Z"></a>
#### a. 创建develop分支
```bash
git branch develop
git push -u origin develop
```
<a name="EI98q"></a>
#### b. 开始新Feature开发
```bash
git checkout -b some-feature develop
# Optionally, push branch to origin:
git push -u origin some-feature

# 做一些改动
git status
git add some-file
git commit
```
<a name="JA0SL"></a>
#### c. 完成Feature
```bash
git pull origin develop
git checkout develop
git merge --no-ff some-feature
git push origin develop

git branch -d some-feature

# If you pushed branch to origin:
git push origin --delete some-feature
```
<a name="ytlcr"></a>
#### d. 开始Relase
```bash
git checkout -b release-0.1.0 develop

# Optional: Bump version number, commit
# Prepare release, commit
```
<a name="pGDHE"></a>
#### e. 完成Release
```bash
git checkout master
git merge --no-ff release-0.1.0
git push

git checkout develop
git merge --no-ff release-0.1.0
git push

git branch -d release-0.1.0

# If you pushed branch to origin:
git push origin --delete release-0.1.0   

git tag -a v0.1.0 master
git push --tags
```
<a name="RrZP1"></a>
#### f. 开始Hotfix
```bash
git checkout -b hotfix-0.1.1 master
```
<a name="QEIie"></a>
#### g. 完成Hotfix
```bash
git checkout master
git merge --no-ff hotfix-0.1.1
git push

git checkout develop
git merge --no-ff hotfix-0.1.1
git push

git branch -d hotfix-0.1.1

git tag -a v0.1.1 master
git push --tags
```
