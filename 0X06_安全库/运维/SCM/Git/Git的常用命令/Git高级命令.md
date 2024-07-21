Git
<a name="h9aGW"></a>
## 高级命令
<a name="SMOIH"></a>
### git submodule 子模块
有种情况经常会遇到：某个工作中的项目需要包含并使用另一个项目。也许是第三方库，或者独立开发的，用于多个父项目的库。现在问题来了：想要把它们当做两个独立的项目，同时又想在一个项目中使用另一个。如果将另外一个项目中的代码复制到自己的项目中，那么做的任何自定义修改都会使合并上游的改动变得困难。**「Git 通过子模块来解决这个问题，允许将一个 Git 仓库作为另一个 Git 仓库的子目录。它能将另一个仓库克隆到自己的项目中，同时还保持提交的独立。」**
```bash
# 在主项目中添加子项目，URL 为子模块的路径，path 为该子模块存储的目录路径
git submodule add [URL] [Path]

# 克隆含有子项目的主项目
git clone [URL]
# 当在克隆这样的项目时，默认会包含该子项目的目录，但该目录中还没有任何文件
# 初始化本地配置文件
git submodule init
# 从当前项目中抓取所有数据并检出父项目中列出的合适的提交-更新子模块
git submodule update
# 等价于 git submodule init && git submodule update
git submodule update --init

# 自动初始化并更新仓库中的每一个子模块， 包括可能存在的嵌套子模块
git clone --recurse-submodules [URL]
```
<a name="B0hGM"></a>
### git gc-运行Git的垃圾回收功能，清理冗余的历史快照
运行Git的垃圾回收功能，清理冗余的历史快照
<a name="qcUyu"></a>
### git archive-将加了tag的某个版本打包提取
将加了tag的某个版本打包提取
```bash
git archive -v --format=zip v0.1 > v0.1.zip
```
`--format` 表示打包的格式，如 `zip`，`-v` 表示对应的tag名，后面跟的是tag名，如 `v0.1`。
<a name="lE9AX"></a>
### git fetch-将远程版本库的更新取回到本地版本库
```bash
# 默认情况下，git fetch 取回所有分支的更新。如果只想取回特定分支的更新，可以指定分支名。
$ git fetch origin daily/0.0.1
```
<a name="OuexG"></a>
## 问题排查
<a name="baCX3"></a>
### git blame-查看文件每行代码块的历史信息
```bash
# 查看文件每行代码块的历史信息
$ 截取 demo.html 文件1-10行历史信息
```
<a name="xR4ja"></a>
### git bisect-二分查找历史记录，排查BUG
```bash
# 开始二分查找
$ git bisect start
# 标记当前二分提交ID为有问题的点
$ git bisect bad
# 标记当前二分提交ID为没问题的点
$ git bisect good
# 查到有问题的提交ID后回到原分支
$ git bisect reset
```
