Git
<a name="Ygrin"></a>
## 一、本地初始化配置
<a name="OmlXF"></a>
### 配置全局用户名和邮箱
<a name="MZQVr"></a>
#### 命令
```bash
# 显示当前的Git配置
$ git config --list
# 配置全局用户
$ git config --global user.name "Fcant"
$ git config --global user.email fcscanf@outlook.com
arr2[2] #array(7,8,9)
# 配置别名
$ git config --global alias.co checkout
$ git config --global alias.ss status
$ git config --global alias.cm commit
$ git config --global alias.br branch
$ git config --global alias.rg reflog
# 这里只是美化 log 的输出，实际使用时可以在 git lg 后面加命令参数，如：git lg -10 显示最近10条提交
$ git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"
# 删除全局配置
$ git config --global --unset alias.xxx
$ git config --global --unset user.xxx
# 颜色设置
git config --global color.ui true                         # git status等命令自动着色
git config --global color.status auto
git config --global color.diff auto
git config --global color.branch auto
git config --global color.interactive auto
git config --global --unset http.proxy                    # remove  proxy configuration on git
```
<a name="4KTNa"></a>
#### 说明文档
[Git本地初始化配置](https://git-scm.com/book/zh/v2/%E8%B5%B7%E6%AD%A5-%E5%88%9D%E6%AC%A1%E8%BF%90%E8%A1%8C-Git-%E5%89%8D%E7%9A%84%E9%85%8D%E7%BD%AE)<br />[https://git-scm.com/book/zh/v2/%E8%B5%B7%E6%AD%A5-%E5%88%9D%E6%AC%A1%E8%BF%90%E8%A1%8C-Git-%E5%89%8D%E7%9A%84%E9%85%8D%E7%BD%AE](https://git-scm.com/book/zh/v2/%E8%B5%B7%E6%AD%A5-%E5%88%9D%E6%AC%A1%E8%BF%90%E8%A1%8C-Git-%E5%89%8D%E7%9A%84%E9%85%8D%E7%BD%AE)
<a name="UIfp6"></a>
### 配置本地ssh的密钥
<a name="lthl6"></a>
#### 命令
```bash
$ ssh-keygen -t rsa -f ~/.ssh/id_rsa -C "fcscanf@outlook.com"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1586000806779-1f50c823-9497-41cb-b5eb-bf175e3c45bd.png#averageHue=%23050302&height=338&id=Ka0ze&originHeight=338&originWidth=562&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29018&status=done&style=none&title=&width=562)<br />-t rsa：表示用rsa算法进行加密。若不指定，则默认生成用于SSH-2的RSA密钥。<br />    　　密钥类型："rsa1"(SSH-1) "rsa"(SSH-2) "dsa"(SSH-2)<br />    　　当前指定类型为rsa1时，提示 failed: unknown or unsupported key type.<br />-t dsa：同理，使用dsa算法进行加密。<br />-C ：提供一个新注释；默认初始化为user@host<br />-c ：要求修改私钥和公钥文件中的注释。本选项只支持 RSA1 密钥。　　//当前测试无效，提示非RSA1密钥。<br />-f ：指定生成密钥路径。  //此处指的是全路径，包含生成的key文件的名称，<br />     例如：<br />ssh-keygen -t rsa -f ~/.ssh/id_rsa    //其中id_rsa为生成文件名称.
<a name="9JEkk"></a>
## 二、Git常用操作命令
<a name="yxBS7"></a>
### 新建一个 Git 项目的两种方式
<a name="PZ9F6"></a>
#### 本地新建好 Git 项目，然后关联远程仓库
```bash
# 初始化一个Git仓库
$ git init
# 新建一个目录，将其初始化为Git代码库
$ git init [project-name]
# 关联远程仓库
$ git remote add <name> <git-repo-url>
# 例如
$ git remote add origin https://github.com/xxxxxx
```
<a name="xxG1x"></a>
#### clone 远程仓库
```bash
# 新建好远程仓库，然后 clone 到本地
$ git clone <git-repo-url>

# 将远程仓库下载到（当前 git bash 启动位置下面的）指定文件中，如果没有会自动生成
$ git clone <git-repo-url> <project-name>
```
<a name="HM6n2"></a>
### 查看 Git 信息
```bash
 # 查看系统配置
$ git config --list
# 查看用户配置
$ cat ~/.gitconfig
# 查看当前项目的 git 配置
$ cat .git/config
# 查看暂存区的文件
$ git ls-files
# 查看本地 git 命令历史
$ git reflog
# 查看所有 git 命令
$ git --help -a
# 查看当前 HEAD 指向
$ cat .git/HEAD

# git 中 D 向下翻一行  F 向下翻页  B 向上翻页  Q 退出
# 查看提交历史
$ git log --oneline
          --grep="关键字"
          --graph
          --all
          --author "username"
          --reverse
          -num
          -p
          --before=  1  day/1  week/1  "2019-06-06"
          --after= "2019-06-06"
          --stat
          --abbrev-commit
          --pretty=format:"xxx"
          
# oneline -> 将日志记录一行一行的显示
# grep="关键字" -> 查找日志记录中(commit提交时的注释)与关键字有关的记录
# graph -> 记录图形化显示 ！！！
# all -> 将所有记录都详细的显示出来
# author "username" -> 查找这个作者提交的记录
# reverse -> commit 提交记录顺序翻转
# before -> 查找规定的时间(如:1天/1周)之前的记录
# num -> git log -10 显示最近10次提交 ！！！
# stat -> 显示每次更新的文件修改统计信息，会列出具体文件列表 ！！！
# abbrev-commit -> 仅显示 SHA-1 的前几个字符，而非所有的 40 个字符 ！！！
# pretty=format:"xxx" ->  可以定制要显示的记录格式 ！！！
# p -> 显示每次提交所引入的差异（按 补丁 的格式输出）！！！
```
<a name="mz03D"></a>
### Git 常用命令
```bash
# 查看工作区和暂存区的状态
$ git status
# 将工作区的文件提交到暂存区
$ git add .
# 提交到本地仓库
$ git commit -m "本次提交说明"
# add和commit的合并，便捷写法（未追踪的文件无法直接提交到暂存区/本地仓库）
$ git commit -am "本次提交说明"
# 将本地分支和远程分支进行关联
$ git push -u origin branchName
# 将本地仓库的文件推送到远程分支
$ git push
# 拉取远程分支的代码
$ git pull origin branchName
# 合并分支
$ git merge branchName
# 查看本地拥有哪些分支
$ git branch
# 查看所有分支（包括远程分支和本地分支）
$ git branch -a
# 切换分支
$ git checkout branchName
# 临时将工作区文件的修改保存至堆栈中
$ git stash
# 将之前保存至堆栈中的文件取出来
$ git stash pop
```
<a name="R2aKV"></a>
### add-将工作区的文件添加到暂存区
将工作区的文件添加到暂存区
```bash
# 添加指定文件到暂存区（追踪新增的指定文件）
$ git add [file1] [file2] ...
# 添加指定目录到暂存区，包括子目录
$ git add [dir]
# 添加当前目录的所有文件到暂存区（追踪所有新增的文件）
$ git add .
# 添加每个变化前，都会要求确认
# 对于同一个文件的多处变化，可以实现分次提交
$ git add -p
# 删除工作区/暂存区的文件
$ git rm [file1] [file2] ...
# 停止追踪指定文件，但该文件会保留在工作区
$ git rm --cached [file]
# 改名工作区/暂存区的文件
$ git mv [file-original] [file-renamed]

# Git 2.0 以下版本
#只作用于文件的新增和修改
$ git add .
#只作用于文件的修改和删除
$ gti add -u
#作用于文件的增删改
$ git add -A

# Git 2.0 版本
$ git add . 等价于 $ git add -A
```

- **「**`**git add .**`**」** ：**「操作的对象是“当前目录”所有文件变更」**，"."  表示当前目录。会监控工作区的状态树，使用它会把工作区的**「所有变化提交」**到暂存区，包括文件内容修改（`modified`）以及新文件（`new`），但**「不包括被删除的文件」**。
- **「**`**git add -u**`**」** ：**「操作的对象是整个工作区已经跟踪的文件变更，无论当前位于哪个目录下」**。仅监控**「已经被 add 的文件」**（即 **「**`**tracked file**`**」**），它会将被修改的文件（包括文件删除）提交到暂存区。`git add -u` 不会提交新文件（**「**`**untracked file**`**」**）。（**「**`**git add --update**`**」** 的缩写）
- **「**`**git add -A**`**」** ：**「操作的对象是“整个工作区”所有文件的变更，无论当前位于哪个目录下」**。是上面两个功能的合集（`git add --all` 的缩写）。

通过 `git add -i` 进入Git命令提示
```bash
❯ git add -i
*** Commands ***
  1: status       2: update       3: revert       4: add untracked
  5: patch        6: diff         7: quit         8: help
What now>
```
通过输入序列号或首字母可以选择相应的功能，具体的功能解释如下：

- `status`：功能上和 `git add -i` 相似
- `update`：详见下方 `git add -u`
- `revert`：把已经添加到暂存区的文件从暂存区剔除，其操作方式和 `update` 类似
- `add untracked`：可以把新增的文件添加到暂存区，其操作方式和 `update` 类似
- `patch`：详见下方 `git add -p`
- `diff`：比较暂存区文件和本地版本库的差异，其操作方式和 `update` 类似
- `quit`：退出 `git add -i` 命令系统
- `help`：查看帮助信息

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594739248248-07a85767-5287-437c-975e-0e2f2269afd8.png#averageHue=%233e3e3c&height=583&id=DWkz7&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2039858&status=done&style=none&title=&width=1107.6666666666667)
<a name="SK5FA"></a>
#### `git add -p` - 交互命令中最有用的 `patch` 模式
这是交互命令中最有用的模式，其操作方式和 `update` 类似，选择后 `Git` 会显示这些文件的当前内容与本地版本库中的差异，然后可以自己决定是否添加这些修改到暂存区，在命令行 `Stage deletion [y,n,q,a,d,/,?]?` 后输入 `y,n,q,a,d,/,?` 其中一项选择操作方式，具体功能解释如下：

- y：接受修改
- n：忽略修改
- q：退出当前命令
- a：添加修改
- d：放弃修改
- /：通过正则表达式匹配修改内容
- ?：查看帮助信息

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594739485559-7d37f56e-c88b-4347-8285-fd0622a24375.png#averageHue=%23363634&height=386&id=FERGG&originHeight=1159&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1433870&status=done&style=none&title=&width=1107.6666666666667)
<a name="uklB0"></a>
#### `git add -u` -直接进入交互命令中的 `update` 模式
它会先列出工作区 `修改` 或 `删除` 的文件列表，`新增` 的文件不会被显示，在命令行 `Update>>` 后输入相应的列表序列号表示选中该项，回车继续选择，如果已选好，直接回车回到命令主界面
<a name="E1479"></a>
#### `git add --ignore-removal` -添加工作区 `修改` 或 `新增` 的文件列表， `删除` 的文件不会被添加
添加工作区 `修改` 或 `新增` 的文件列表， `删除` 的文件不会被添加。
<a name="clean-"></a>
### clean-删除本地工作目录中的未追踪文件
```bash
# 删除本地工作目录中的未追踪文件
$ git clean -n 
–n 表示试运行，在试运行中什么都不会删除。
-f 表示实际删除文件。
-d 表示删除未追踪的目录。
```
<a name="T52PL"></a>
### status-工作区和暂存区的工作状态查看
```bash
# 查看工作区和暂存区的状态
$ git status
# 以简短方式查看工作区和暂存区文件状态
$ git status -s
查看工作区和暂存区文件状态，包括被忽略的文件
$ git status --ignored
```
<a name="83rdJ"></a>
### commit-将暂存区的更改提交至本地仓库
```bash
# 将暂存区的文件提交到本地仓库并添加提交说明
$ git commit -m "本次提交的说明"
# 不打开编辑器，直接在命令行中输入多行提交原因
$ git commit -m '第一行提交原因'  -m '第二行提交原因'
# 提交暂存区的指定文件到仓库区
$ git commit [file1] [file2] ... -m [message]

# 提交工作区自上次commit之后的变化，直接到仓库区
$ git commit -a

# 提交时显示所有diff信息
$ git commit -v

# add 和 commit 的合并，便捷写法，将add和commit合为一步
# 和 git add -u 命令一样，未跟踪的文件是无法提交上去的，将工作区修改或删除的文件提交到本地版本库， 新增 的文件不会被提交
$ git commit -am "本次提交的说明"

# 跳过验证继续提交
$ git commit --no-verify
$ git commit -n

# 编辑器会弹出上一次提交的信息，可以在这里修改提交信息
$ git commit --amend
# 修复提交，同时修改提交信息，修改最新一条提交记录的提交原因
$ git commit --amend -m "本次提交的说明"
# 加入 --no-edit 标记会修复提交但不修改提交信息，编辑器不会弹出上一次提交的信息
$ git commit --amend --no-edit
# 重做上一次commit，并包括指定文件的新变化
$ git commit --amend [file1] [file2] ...
# 将当前文件改动提交到 HEAD 或当前分支的历史ID
$ git commit -C HEAD
```
`git commit --amend` 既可以修改上次提交的文件内容，也可以修改上次提交的说明。会用一个新的 `commit` 更新并替换最近一次提交的 `commit` 。如果暂存区有内容，这个新的 `commit` 会把任何修改内容和上一个 `commit` 的内容结合起来。如果暂存区没有内容，那么这个操作就只会把上次的 `commit` 消息重写一遍。**「永远不要修复一个已经推送到公共仓库中的提交，会拒绝推送到仓库」**
<a name="Z9bLj"></a>
### push & pull-和远程仓库的下拉以及推送

- 分支推送顺序的写法是 **「<来源地>:<目的地>」**
```bash
# 将本地仓库的文件推送到远程分支
# 如果远程仓库没有这个分支，会新建一个同名的远程分支
# 如果省略远程分支名，则表示两者同名
$ git push <远程主机名> <本地分支名>:<远程分支名>
$ git push origin branchname

# 如果省略本地分支名，则表示删除指定的远程分支
# 因为这等同于推送一个空的本地分支到远程分支。
$ git push origin :master
# 等同于
$ git push origin --delete master

# 建立当前分支和远程分支的追踪关系
$ git push -u origin master
# 如果当前分支与远程分支之间存在追踪关系
# 则可以省略分支和 -u
$ git push

# 不管是否存在对应的远程分支，将本地的所有分支都推送到远程主机
$ git push --all origin

# 拉取所有远程分支到本地镜像仓库中
$ git pull
# 拉取并合并项目其他人员的一个分支
$ git pull origin branchname
# 等同于 fetch + merge
$ git fetch origin branchName
$ git merge origin/branchName

# 如果远程主机的版本比本地版本更新，推送时 Git 会报错，要求先在本地做 git pull 合并差异，
# 然后再推送到远程主机。这时，如果你一定要推送，可以使用 –-force 选项
# （尽量避免使用）
$ git push --force origin | git push -f origin
```
<a name="XTC4W"></a>
### log-历史提交查看
<a name="cZ9GI"></a>
#### `git log`-点线图

- **「git 中一条分支就是一个指针，新建一条分支就是基于当前指针新建一个指针」**
- **「切换至某个分支 ，就是将 HEAD 指向某条分支（指针）」**
- **「切换至某个 commit ，就是将 HEAD 指向某个 commit」**

符号解释：
```bash
*	表示一个 commit
|	表示分支前进
/	表示分叉
\	表示合入
|/	表示新分支
```
```bash
# 显示当前分支的版本历史
$ git log

# 显示commit历史，以及每次commit发生变更的文件
$ git log --stat

# 搜索提交历史，根据关键词
$ git log -S [keyword]

# 显示某个commit之后的所有变动，每个commit占据一行
$ git log [tag] HEAD --pretty=format:%s

# 显示某个commit之后的所有变动，其"提交说明"必须符合搜索条件
$ git log [tag] HEAD --grep feature

# 显示某个文件的版本历史，包括文件改名
$ git log --follow [file]
$ git whatchanged [file]

# 显示带提交差异对比的历史记录
$ git log -p
# 显示 demo.html 文件的历史记录
$ git log demo.html
# 显示2周前开始到现在的历史记录，其它时间可以类推
$ git log --since="2 weeks ago"
# 显示截止到2周前的历史记录，其它时间可以类推
$ git log --before="2 weeks ago"
# 显示最近10条历史记录
$ git log -10
# 显示过去5次提交
$ git log -5 --pretty --oneline

# 显示所有提交过的用户，按提交次数排序
$ git shortlog -sn

# 显示指定文件是什么人在什么时间修改过
$ git blame [file]
# 显示从提交ID f5f630a 到 HEAD 之间的记录，HEAD 可以为空或其它提交ID
$ git log f5f630a..HEAD
# 在一行中输出简短的历史记录
$ git log --pretty=oneline
# 格式化输出历史记录
$ git log --pretty=format:"%h" 
```
格式化输出历史记录<br />`Git` 用各种 `placeholder` 来决定各种显示内容，常用的显示如下：

- %H: commit hash
- %h: 缩短的commit hash
- %T: tree hash
- %t: 缩短的 tree hash
- %P: parent hashes
- %p: 缩短的 parent hashes
- %an: 作者名字
- %aN: mailmap的作者名
- %ae: 作者邮箱
- %ad: 日期 (--date= 制定的格式)
- %ar: 日期, 相对格式(1 day ago)
- %cn: 提交者名字
- %ce: 提交者 email
- %cd: 提交日期 (--date= 制定的格式)
- %cr: 提交日期, 相对格式(1 day ago)
- %d: ref名称
- %s: commit信息标题
- %b: commit信息内容
- %n: 换行
<a name="uKI8U"></a>
#### reflog-可以查看所有分支的所有操作记录（包括commit和reset的操作、已经被删除的commit记录
`reflog` 可以查看所有分支的所有操作记录（包括commit和reset的操作、已经被删除的commit记录，跟 `git log` 的区别在于它不能查看已经删除了的commit记录
<a name="LA97f"></a>
#### git reflog-显示 `HEAD` 指向发生改变的时间列表

- **「显示的是一个 **`**HEAD**`** 指向发生改变的时间列表」**。在切换分支、用 `git commit` 进行提交、以及用 `git reset` 撤销 commit 时，`HEAD` 指向会改变，但当进行 `git checkout -- <filename>` 撤销或者 `git stash`  存储文件等操作时，`HEAD` 并不会改变，这些修改从来没有被提交过，因此 `reflog` 也无法恢复它们。
- `git reflog` 不会永远保持，Git 会定期清理那些 “用不到的” 对象，不要指望几个月前的提交还一直在那里。
```bash
# 显示当前分支的最近几次提交
$ git reflog
```
<a name="J9ZNi"></a>
### branch-分支管理
```bash
# 查看本地分支，列出所有本地分支
$ git branch | git branch -l
# 查看远程分支
$ git branch -r
# 查看所有分支（本地分支+远程分支）
$ git branch -a
# 查看所有分支并带上最新的提交信息
$ git branch -av
# 查看本地分支对应的远程分支
$ git branch -vv

# 新建分支
# 在别的分支下新建一个分支，新分支会复制当前分支的内容
# 注意：如果当前分支有修改，但是没有提交到仓库，此时修改的内容是不会被复制到新分支的
$ git branch branchname
# 切换分支(切换分支时，本地工作区，仓库都会相应切换到对应分支的内容)
$ git checkout branchname
# 切换到上一个分支
$ git checkout -
# 创建一个 aaa 分支，并切换到该分支 （新建分支和切换分支的简写）
$ git checkout -b aaa
# 可以看做是基于 master 分支创建一个 aaa 分支，并切换到该分支
$ git checkout -b aaa master

# 新建一个分支，指向指定commit
$ git branch [branch] [commit]

# 新建一个分支，与指定的远程分支建立追踪关系
$ git branch --track [branch] [remote-branch]

# 新建一条空分支（详情请看问题列表）
$ git checkout --orphan emptyBranchName
$ git rm -rf .

# 删除本地分支,会阻止删除包含未合并更改的分支
$ git brnach -d branchname
# 强制删除一个本地分支，即使包含未合并更改的分支
$ git branch -D branchname
# 删除远程分支
# 推送一个空分支到远程分支，其实就相当于删除远程分支
$ git push origin  :远程分支名
# 或者
$ git push origin --delete 远程分支名
$ git branch -dr [remote/branch]

# 建立追踪关系，在现有分支与指定的远程分支之间
$ git branch --set-upstream [branch] [remote-branch]

# 修改当前分支名
$ git branch -m branchname

# 选择一个commit，合并进当前分支
$ git cherry-pick [commit]

# 新建一个分支，指向某个tag
$ git checkout -b [branch] [tag]

# 检出head版本的README文件（可用于修改错误回退）
$ git checkout -- README  
```
<a name="HSx1r"></a>
### merge-三种常用合并分支的方法
```bash
# 默认 fast-forward ，HEAD 指针直接指向被合并的分支
$ git merge

# 禁止快进式合并
$ git merge --no-ff

$ git merge --squash
```

- `fast-forward`：会在当前分支的提交历史中添加进被合并分支的提交历史（**「得先理解什么时候会发生快速合并，并不是每次 merge 都会发生快速合并」**）；
- `--no-ff`：**「会生成一个新的提交」**，让当前分支的提交历史不会那么乱；
- `--squash`：**「不会生成新的提交」**，会将被合并分支多次提交的内容直接存到工作区和暂存区，由开发者手动去提交，这样当前分支最终只会多出一条提交记录，不会掺杂被合并分支的提交历史
<a name="IOzhq"></a>
### rebase-重新定义分支的版本库状态
```bash
# 合并分支，这跟 merge 很像,但还是有本质区别
$ git rebase branch_name
# 合并过程中可能需要先解决冲突，然后执行 git rebase --continue
$ git rebase -i HEAD~~
# 打开文本编辑器，将看到从 HEAD 到 HEAD~~ 的提交如下
pick 9a54fd4 添加commit的说明
pick 0d4a808 添加pull的说明
# Rebase 326fc9f..0d4a808 onto d286baa
#
# Commands:
#  p, pick = use commit
#  r, reword = use commit, but edit the commit message
#  e, edit = use commit, but stop for amending
#  s, squash = use commit, but meld into previous commit
#  f, fixup = like "squash", but discard this commit's log message
#  x, exec = run command (the rest of the line) using shell
将第一行的 pick 改成 Commands 中所列出来的命令，然后保存并退出，所对应的修改将会生效。
如果移动提交记录的顺序，将改变历史记录中的排序。
```
合并分支，这跟 `merge` 很像，但还是有本质区别，看下图<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594741539659-ddec0a44-11f8-48cc-a68d-2606f890f83f.png#averageHue=%23d6dfe3&height=309&id=BHZMf&originHeight=927&originWidth=1694&originalType=binary&ratio=1&rotation=0&showTitle=false&size=730632&status=done&style=none&title=&width=564.6666666666666)
<a name="8fxQb"></a>
### stash-将所有未提交的修改保存至堆栈中，用于后续恢复当前工作区内容

- 能够将所有未提交的修改保存至堆栈中，用于后续恢复当前工作区内容
- 如果文件没有提交到**「暂存区（使用 git add . 追踪新的文件）」**，使用该命令会提示 `No local changes to save` ，无法将修改保存到堆栈中

**「使用场景：」** 当接到一个修复紧急 bug 的任务时候，一般都是先创建一个新的 bug 分支来修复它，然后合并，最后删除。但是，如果当前正在开发功能中，短时间还无法完成，无法直接提交到仓库，这时候可以先把当前工作区的内容 `git stash` 一下，然后去修复 bug，修复后，再 `git stash pop`，恢复之前的工作内容。
```bash
# 将所有未提交的修改（提交到暂存区）保存至堆栈中
$ git stash
# 给本次存储加个备注，以防时间久了忘了
$ git stash save "存储"
# 存储未追踪的文件
$ git stash -u

# 查看栈中保存的列表
$ git stash list

在 Windows 上和 PowerShell 中，需要加双引号
# 恢复后，stash 记录并不删除
$ git stash apply "stash@{index}"
# 从Git栈中检出其中一条记录，但不从栈中移除
$ git stash apply stash@{0}
# 恢复的同时把 stash 记录也删了
$ git stash pop "stash@{index}"
# 把当前栈中最近一次记录检出并创建一个新分支
$ git stash branch new_banch
# 从Git栈中检出最新保存的一条记录，并将它从栈中移除
$ git stash pop
# 删除 stash 记录
$ git stash drop "stash@{index}"
# 移除栈中其中一条记录
$ git stash drop stash@{0}
# 删除所有存储的进度
$ git stash clear
# 为当前修改或删除的文件创建一个自定义的栈并返回一个ID，此时并未真正存储到栈里
$ git stash create 
# 将 create 方法里返回的ID放到 store 后面，此时在栈里真正创建了一个记录，但当前修改或删除的文件并未从工作区移除
$ git stash store xxxxxx
# 查看当前记录中修改了哪些文件
$ git stash show "stash@{index}"
# 显示栈中其中一条记录
$ git stash show stash@{0}
# 查看当前记录中修改了哪些文件的内容
$ git stash show -p "stash@{index}"
```
<a name="pjQs4"></a>
### diff-工作区和暂存区的文件对比
```bash
# 查看工作区和暂存区单个文件的对比
$ git diff filename
# 查看工作区和暂存区所有文件的对比
$ git diff
# 查看工作区和暂存区所有文件的对比，并显示出所有有差异的文件列表
$ git diff --stat
# 注意：
# 1.你修改了某个文件，但是没有提交到暂存区，这时候会有对比的内容
# 一旦提交到暂存区，就不会有对比的内容(因为暂存区已经更新)
# 2.如果你新建了一个文件，但是没有提交到暂存区，这时候 diff 是没有结果的	

# 查看暂存区与上次提交到本地仓库的快照（即最新提交到本地仓库的快照）的对比
$ git diff --cached/--staged
# 查看工作区与上次提交到本地仓库的快照（即最新提交到本地仓库的快照）的对比
$ git diff branchname
# 查看工作区与 HEAD 指向（默认当前分支最新的提交）的对比
$ git diff HEAD
# 显示两次提交之间的差异
$ git diff [first-branch]...[second-branch]

# 显示今天你写了多少行代码
$ git diff --shortstat "@{0 day ago}"

# 显示某次提交的元数据和内容变化
$ git show [commit]

# 显示某次提交发生变化的文件
$ git show --name-only [commit]

# 显示某次提交时，某个文件的内容
$ git show [commit]:[filename]
# 查看两个本地分支中某一个文件的对比
$ git diff branchname..branchname filename
# 查看两个本地分支所有的对比
$ git diff branchname..branchname
# 查看远程分支和本地分支的对比
$ git diff origin/branchname..branchname
# 查看远程分支和远程分支的对比
$ git diff origin/branchname..origin/branchname

# 查看两个 commit 的对比
$ git diff commit1..commit2
```
<a name="fd3ch"></a>
### remote-远程仓库相关操作
```bash
# 查看所有远程主机
$ git remote
# 查看关联的远程仓库的详细信息
$ git remote -v
# 关联远程仓库
$ git remote add <name> <git-repo-url>
# 例如
$ git remote add origin https://github.com/xxxxxx
# 删除远程仓库的 “关联”
$ git remote rm projectname
# 设置远程仓库的 “关联”
$ git remote set-url origin <newurl>
# 下载远程仓库的所有变动
$ git fetch [remote]
# 显示某个远程仓库的信息
$ git remote show [remote]
# 取回远程仓库的变化，并与本地分支合并
$ git pull [remote] [branch]

# 上传本地指定分支到远程仓库
$ git push [remote] [branch]

# 强行推送当前分支到远程仓库，即使有冲突
$ git push [remote] --force

# 推送所有分支到远程仓库
$ git push [remote] --all
```
<a name="dcmoK"></a>
### tag-版本发布
常用于发布版本
```bash
# 默认在 HEAD 上创建一个标签
$ git tag v1.0
# 指定一个 commit id 创建一个标签
$ git tag v0.9 f52c633
# 创建带有说明的标签，用 -a 指定标签名，-m 指定说明文字
$ git tag -a v0.1 -m "version 0.1 released"

# 查看所有标签
# 注意：标签不是按时间顺序列出，而是按字母排序的。
$ git tag

# 查看单个标签具体信息
$ git show <tagname>

# 推送一个本地标签
$ git push origin <tagname>
# 推送全部未推送过的本地标签
$ git push origin --tags

# 删除本地标签
# 因为创建的标签都只存储在本地，不会自动推送到远程。
# 所以，打错的标签可以在本地安全删除。
$ git tag -d v0.1
# 删除一个远程标签（先删除本地 tag ，然后再删除远程 tag）
$ git push origin :refs/tags/<tagname>

# 新建一个分支，指向某个tag
$ git checkout -b [branch] [tag]
```
<a name="DUhBK"></a>
### rm-从工作区和暂存区移除文件
```bash
# 删除暂存区和工作区的文件
$ git rm filename
# 只删除暂存区的文件，不会删除工作区的文件
$ git rm --cached filename
```
如果在配置 .gitignore 文件之前就把某个文件上传到远程仓库了，这时候想把远程仓库中的该文件删除，此时配置 .gitignore 文件也没有用，因为该文件已经被追踪了，但又不想在本地删除该文件后再重新提交到远程仓库，这时候可以使用 `git rm --cached filename` 命令取消该文件的追踪，这样下次提交的时候，git 就不会再提交这个文件，从而远程仓库的该文件也会被删除
<a name="uJg0A"></a>
### mv-移动或重命名文件、目录
```bash
# 将 a.md 重命名为 b.md ，同时添加变动到暂存区，加 -f 参数可以强制重命名
# 相比用 mv a.md b.md 命令省去了 git add 操作
$ git mv a.md b.md -f
```
<a name="oit1a"></a>
### 版本切换 & 重设 & 撤销

- **「checkout 可以撤销工作区的文件，reset 可以撤销工作区/暂存区的文件」**
- **「reset 和 checkout 可以作用于 commit 或者文件，revert 只能作用于 commit」**
<a name="C67j5"></a>
#### checkout 详解
```bash
# 恢复暂存区的指定文件到工作区
$ git checkout <filename>
# 恢复暂存区的所有文件到工作区
$ git checkout .

# 回滚到最近的一次提交
# 如果修改某些文件后，没有提交到暂存区，此时的回滚是回滚到上一次提交
# 如果是已经将修改的文件提交到仓库了，这时再用这个命令回滚无效
# 因为回滚到的是之前自己修改后提交的版本
$ git checkout HEAD
$ git checkout HEAD -- filename
# 回滚到最近一次提交的上一个版本
$ git checkout HEAD^
# 回滚到最近一次提交的上2个版本
$ git checkout HEAD^^

# 切换分支，在这里也可以看做是回到项目「当前」状态的方式
$ git checkout <当前正在使用的分支>
# 切换到某个指定的 commit 版本
$ git checkout <commit_id>
# 切换指定 tag
$ git checkout <tag>
```

- **「在开发的正常阶段，**`**HEAD**`** 一般指向 master 或是其他的本地分支，但当使用 **`**git checkout <commit id>**`** 切换到指定的某一次提交的时候，**`**HEAD**`** 就不再指向一个分支了——它直接指向一个提交，HEAD 就会处于 detached 状态（游离状态）」**。
- 切换到某一次提交后，可以查看文件，编译项目，运行测试，甚至编辑文件而不需要考虑是否会影响项目的当前状态，所做的一切都不会被保存到主栈的仓库中。当想要回到主线继续开发时，使用 `git checkout branchName` 回到项目初始的状态（**「这时候会提示是否需要新建一条分支用于保留刚才的修改」**）。
- **「哪怕切换到了某一版本的提交，并且对它做了修改后，不小心提交到了暂存区，只要切换回分支的时候，依然会回到项目的初始状态。（***注意：所做的修改，如果 commit 了，会被保存到那个版本中。切换完分支后，会提示是否要新建一个分支来保存刚才修改的内容。如果刚才解决了一个 bug ，这时候可以新建一个临时分支，然后本地自己的开发主分支去合并它，合并完后删除临时分支***）。」**
- **「一般都是用 checkout 回退版本，查看历史代码，测试 bug 在哪」**
<a name="GwjLf"></a>
#### reset 详解
`git reset [--hard|soft|mixed|merge|keep] [<commit>或HEAD]`：将当前的分支重设(`reset`)到指定的 `<commit>` 或者 `HEAD` (默认，如果不显示指定 `<commit>`，默认是 `HEAD` ，即最新的一次提交)，并且根据 `[mode]` 有可能更新索引和工作目录。`mode` 的取值可以是 `hard`、`soft`、`mixed`、`merged`、`keep` 。
```bash
# 从暂存区撤销特定文件，但不改变工作区。它会取消这个文件的暂存，而不覆盖任何更改
$ git reset <fileName>
# 重置暂存区最近的一次提交，但工作区的文件不变
$ git reset
# 等价于
$ git reset HEAD （默认）
# 重置暂存区与工作区，回退到最近一次提交的版本内容
$ git reset --hard
# 重置暂存区与工作区，回退到最近一次提交的上一个版本
$ git reset --hard HEAD^

# 将当前分支的指针指向为指定 commit（该提交之后的提交都会被移除），同时重置暂存区，但工作区不变
$ git reset <commit>
# 等价于
$ git reset --mixed  <commit>

# 将当前分支的指针指向为指定 commit（该提交之后的提交都会被移除），但保持暂存区和工作区不变
$ git reset --soft  <commit>
# 将当前分支的指针指向为指定 commit（该提交之后的提交都会被移除），同时重置暂存区、工作区
$ git reset --hard  <commit>
# 重置当前HEAD为指定commit，但保持暂存区和工作区不变
$ git reset --keep [commit]
```

- `git reset` 有很多种用法。它可以被用来移除提交快照，尽管它通常被用来撤销暂存区和工作区的修改。不管是哪种情况，它应该只被用于本地修改——永远不应该重设和其他开发者共享的快照。
- **「当用 reset 回滚到了某个版本后，那么在下一次 git 提交时，之前该版本后面的版本会被作为垃圾删掉。」**
- **「当回退到一个旧版本后，此时再用 git log 查看提交记录，会发现之前的新版本记录没有了。如果第二天，想恢复到新版本怎么办？找不到新版本的 commit_id 怎么办？」**

**「此时可以用 **`**git reflog**`** 查看历史命令，这样就可以看到之前新版本的 commit_id ，然后 **`**git reset --hard commit_id**`** 就可以回到之前的新版本代码」**

- 虽然可以用 git reflog 查看本地历史，然后回复到之前的新版本代码，但是在别的电脑上是无法获取当前电脑上的历史命令的，所以这种方法不安全。万一电脑突然坏了，这时候就无法回到未来的版本。
<a name="nZmek"></a>
#### revert 详解-**「撤销某个已经提交的快照（和 reset 重置到某个指定版本不一样）」**
```bash
# 生成一个撤销最近的一次提交的新提交
$ git revert HEAD
# 生成一个撤销最近一次提交的上一次提交的新提交
$ git revert HEAD^
# 生成一个撤销最近一次提交的上两次提交的新提交
$ git revert HEAD^^
# 生成一个撤销最近一次提交的上n次提交的新提交
$ git revert HEAD~num

# 生成一个撤销指定提交版本的新提交
$ git revert <commit_id>
# 生成一个撤销指定提交版本的新提交，执行时不打开默认编辑器，直接使用 Git 自动生成的提交信息
$ git revert <commit_id> --no-edit
```
`git revert`命令用来**「撤销某个已经提交的快照（和 reset 重置到某个指定版本不一样）」**。它是在提交记录最后面加上一个撤销了更改的新提交，而不是从项目历史中移除这个提交，这避免了 Git 丢失项目历史。<br />**「撤销（revert）应该用在想要在项目历史中移除某个提交的时候」**。比如说，在追踪一个 bug，然后发现它是由一个提交造成的，这时候撤销就很有用。<br />**「撤销（revert）被设计为撤销公共提交的安全方式，重设（reset）被设计为重设本地更改。」**<br />因为两个命令的目的不同，它们的实现也不一样：重设完全地移除了一堆更改，而撤销保留了原来的更改，用一个新的提交来实现撤销。**「千万不要用 **`**git reset**`** 回退已经被推送到公共仓库上的 提交，它只适用于回退本地修改（从未提交到公共仓库中）。如果需要修复一个公共提交，最好使用 git revert」**。<br />发布一个提交之后，必须假设其他开发者会依赖于它。移除一个其他团队成员在上面继续开发的提交在协作时会引发严重的问题。当他们试着和仓库同步时，他们会发现项目历史的一部分突然消失了。一旦在重设之后又增加了新的提交，Git 会认为本地历史已经和 origin/master 分叉了，同步仓库时的合并提交(merge commit)会使你的同事困惑。
<a name="UK6Tu"></a>
### cherry-pick-**将指定的提交 **commit **应用于当前分支**（可以用于恢复不小心撤销（revert/reset）的提交）
**「将指定的提交 commit 应用于当前分支」**（可以用于恢复不小心撤销（revert/reset）的提交）
```bash
$ git cherry-pick <commit_id>
$ git cherry-pick <commit_id> <commit_id>
$ git cherry-pick <commit_id>^..<commit_id>
```
<a name="KVOOE"></a>
### 打包部署
```bash
# 生成一个可供发布的压缩包
$ git archive
# 全量包
$ git archive --format tar.gz --output "./output.tar.gz" 分支
# 增量包
$ git archive -o update.zip 
$ git diff 新版本 旧版本 --name-only
```
