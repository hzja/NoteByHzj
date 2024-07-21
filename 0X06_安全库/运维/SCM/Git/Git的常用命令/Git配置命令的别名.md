Git Alias
<a name="RTUMl"></a>
## 1、在 `.gitconfig` 文件进行别名配置
在用户目录下，有一个 `.gitconfig` 文件，通过配置 `[alias]` 组来配置命令的别名<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1601708494249-32e4ab2e-a145-4c2e-a837-ece084dd476d.png#align=left&display=inline&height=555&originHeight=1665&originWidth=2304&size=441397&status=done&style=none&width=768)
```bash
[alias]
  rhm = reset --hard origin/master
  st = status
  br = branch
```
<a name="a2kZZ"></a>
## 2、通过git命令行命令进行别名配置
Git中的别名(alias)配置规则是这样的。
```bash
git config --global alias.[new_alias] "[previous_git_command]"
# Example
git config --global alias.save commit
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602745050276-ca72a874-5d92-44ee-89c7-b6f74b7e7093.png#align=left&display=inline&height=73&originHeight=219&originWidth=3323&size=188769&status=done&style=none&width=1107.6666666666667)<br />配置完成后，可以看到在.gitconfig文件中出现了定义的内容<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1602746090395-b0d26f7d-5cc5-4c95-8d5a-bfc3ed77f992.png#align=left&display=inline&height=60&originHeight=180&originWidth=1197&size=12476&status=done&style=shadow&width=399)
<a name="ZFfJa"></a>
## 3、常用的别名命令配置
```bash
[alias]
    recommit = commit --amend -m
    commend = commit --amend --no-edit
    here = !git init && git add . && git commit -m \"Initialized a new repository\"
    search = grep
    who = blame
    zip = archive --format=tar.gz -o ../repo.tar.gz
    clonely = clone --single-branch --branch
    plg = log --graph --pretty=format:'%C(yellow)%h%Creset -%Cred%d%Creset %s %Cgreen| %cr %C(bold blue)| %an%Creset' --abbrev-commit --date=relative
    fresh = filter-branch --prune-empty --subdirectory-filter
```
<a name="eeb9e336"></a>
### `git recommit`
```bash
git config --global alias.recommit 'commit --amend -m'
```
`git commit --amend` 允许更改最后的提交信息(`message`)。`recommit`命令让提交变得更简单，更容易记住。
```bash
# Change the last commit message with recommit
git recommit "New commit message"
# [master 64175390] New commit message
#  Date: Tue Sep 22 15:09:11 2020 +0000
#  1 file changed, 2 insertions(+)
#  create mode 100644 vue.js
```
<a name="ENXcI"></a>
### `git commend`
```bash
git config --global alias.commend 'commit --amend --no-edit'
```
使用`--no-edit`标志进行修改，可以在最近一次提交时在仓库中提交新的更改，不需要再次重复提交消息。<br />**解释一下这个命令**，是否有这种经历，写完代码了 `git add .` 、 `git commit xxx` ，一顿操作，刚想`push` 的时候发现 有个文件漏改了，又是 `git add .` 、 `git commit xxx` 一顿操作，此时 `commit` 就会有两次记录，这对于项目来说是非常不好的，一次 `commit` 被分成了两次，如果遇到需要`revert` 就傻眼了。这个时候就可以用这个命令轻松解决。<br />**代码演示**
```bash
echo 'Hello world' > README.md
git add .
git commit -m "Hello Word"
git log --oneline
4b39c8a (HEAD -> master) Add README.md
```
```bash
echo 'Hello Fcant' >> README.md
git commend
git log --oneline 
60c5190 (HEAD -> master) Add README.md
```
此时`git log`依然只有一次记录。
<a name="bFDLn"></a>
### `git search`
```bash
git config --global alias.search 'grep'
# Example
git search [search_term]
```
`git grep`允许在存储库中搜索关键字(且支持正则)，并返回各种匹配项。但是 `grep` 一般在使用Linux的匹配搜索比较常见，中文环境下使用 `search`易于记住并且易于使用。
```bash
git search createHotContext
```
<a name="zjJtv"></a>
### `git here`
```bash
git config --global alias.here '!git init && git add . && git commit -m "init Pro"'
```
通常初始化一个新的仓库时，将暂存所有文件，并使用初始提交消息进行提交。使用`git here`一步就完成了（这对于开源工具重度爱好者，真的是福星）。只需在要创建新仓库的文件夹中运行它，就可以了。
> 小技巧: 在公司开发代码需要提交到公司的私有仓库，因此全局配置了公司的 username 和 email，当切换到开源项目的时候，是会忘记修改回来，因此会创建一个 `git config user.name xxx \n git config user.email xxx@xx.com` 的一个 sh文件。因此初始化的时候可以这样 。

```bash
git config --global alias.here '!git init && sh ~/my/git.sh && git add . && git commit -m "init Pro"'
```
这样子，既可以提交到私有仓库，创建开源项目的时候又不耽误。
<a name="69Yx9"></a>
### `git who`
```bash
git config --global alias.who 'blame'
# Example
git who index.ts
# 641753902 (Ephraim Atta-Duncan 2020-09-22 15:09:11 +0000 1)
# 641753902 (Ephraim Atta-Duncan 2020-09-22 15:09:11 +0000 2) console.log("who?")
```
`git blame` 用于逐行检查文件的内容，并查看每行的最后修改时间以及修改的作者。如果有错误，可以追溯到某一行的改动是谁修改的。vscode 插件 `GitLens`也是基于此原理。
> 总结: 追踪 bug 小能手，以后谁写出bug，轻松定位某一行是谁写的。

<a name="EkGnu"></a>
### `git zip`
```bash
git config --global alias.zip 'archive --format=tar.gz -o repo.tar.gz'
# Example
git zip [branch_name]
```
使用 `archive`命令可以创建整个或部分仓库的 `tarball` 和`zip`。`git zip` 更容易记住。只需添加分支名称。
```bash
➜  git-fcant git:(master) ls
README.md
➜  git-fcant git:(master) git zip master
➜  git-fcant git:(master) ✗ ls
README.md   repo.tar.gz
```
<a name="Tbj81"></a>
### `git newbie`
```bash
git config --global alias.newbie 'checkout --orphan'
# Example
git newbie [new_branch_name]
```
带有`--orphan` 标志的`git checkout`允许创建一个分支，而没有来自父分支的任何历史记录。
```bash
➜  git-fcant git:(master) git newbie pages
Switched to a new branch 'pages'
➜  git-fcant git:(pages) ✗ ls
README.md
➜  git-fcant git:(pages) ✗ git log
fatal: your current branch 'pages' does not have any commits yet
➜  git-fcant git:(pages) ✗
```
<a name="o3xPq"></a>
#### 实践
它的应用场景<br />还记得`github pages` 吗，利用他能快速创建站点，可以用某个分支来当做站点展示，但是如果把源码和打包后的文件都放在一个分支，就会显得累赘与混乱，这个时候就可以利用这个特性来创建一个全新无 `commit` 的分支。两个工程（一个源文件工程，一个打包后的工程）放到同一个仓库(repo)中。
<a name="XgGbA"></a>
#### 代码演示
```bash
➜  git-fcant git:(master) git newbie pages
Switched to a new branch 'pages'
➜  git-fcant git:(pages) ✗ ls
README.md
➜  git-fcant git:(pages) ✗ git log
fatal: your current branch 'pages' does not have any commits yet
➜  git-fcant git:(pages) ✗ git st
On branch pages
No commits yet
Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
 new file:   README.md
➜  git-fcant git:(pages) ✗
```
<a name="Ba6sz"></a>
### `git clonely`
```bash
git config --global alias.clonely 'clone --single-branch --branch'
# Example
git clonely [branch_name] [remote_url]
git clonely v3 https://github.com/vuejs/vue-apollo
# Cloning into 'vue-apollo'...
# remote: Enumerating objects: 2841, done.
# remote: Total 2841 (delta 0), reused 0 (delta 0), pack-reused 2841
# Receiving objects: 100% (2841/2841), 1.92 MiB | 330.00 KiB/s, done.
# Resolving deltas: 100% (1743/1743), done.
```
带有`--single-branch --branch`标志的`git clone`允许从存储库中`clone`特定分支。
<a name="FG1Wz"></a>
#### 作用
当然是减少`clone`时间，这对大仓库而言简直是福星。
<a name="9Jn41"></a>
### `git plg`
```bash
git config --global alias.plg "log --graph --pretty=format:'%C(yellow)%h%Creset -%Cred%d%Creset %s %Cgreen| %cr %C(bold blue)| %an%Creset' --abbrev-commit --date=relative"
# Example
git plg # plg - Pretty Log
```
`git log`没什么问题，除了它有点丑陋，没有颜色差异，如果要自定义它，需要在 google 上查询相关的命令。幸运的是有别名(alias)。使用该命令的别名，将获得非常漂亮的日志。
<a name="45b665a2"></a>
### `git fresh`
```bash
git config --global alias.fresh "filter-branch --prune-empty --subdirectory-filter"
# Example
git fresh [subfolder] [branch_name]
git fresh src main # Don't do this unless you know what you are doing
```
通过一系列参数，使用`fresh`命令用于从子文件夹中创建新的存储库。带有多个参数的 `filter-branch`获取指定子文件夹的内容，并将其中的内容替换为该子文件夹的内容。
<a name="Rj6Nq"></a>
#### 实践
假设有这样一个项目，目录结构如下
```
.
├── script
│   └── index.js
├── README.md
```
如果需要改造项目，将 `script` 作为单独的项目， 这个时候需要将 `script` 拆出来，一般会通过拷贝来解决，这样做没有什么问题，但是将丢失`script`目录以及子文件所有历史修改记录。<br />现在成功将 `script` 目录拆成了单独的项目。
```bash
git fresh script master
```
再来看 `commit` 记录，依旧保留了`script` 的相关`commit`记录，对于管理项目来说非常有帮助。
```git
commit 8b311558195684d6420baedce74e0f9951208038 (HEAD -> master)
Author: qiufeng <qiufeng@163.com>
Date:   Thu Oct 1 22:37:21 2020 +0800
    feat: script
(END)
```
如果不小心拆分错了，还可以进行还原。
```bash
git reset --hard refs/original/refs/heads/{branch_name}
```
还可以继续拆分,这个时候拆分需要先清除一下备份
```bash
git update-ref -d refs/original/refs/heads/master
```
