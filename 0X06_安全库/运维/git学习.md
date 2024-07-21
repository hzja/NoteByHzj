<a name="b5fSJ"></a>
## 初始化本地库
初始化本地库其实也就是创建本地库<br />初始化命令`git init`
```bash
root@ubuntu:/opt/workspace/wechat# pwd
/opt/workspace/wechat
root@ubuntu:/opt/workspace/wechat# git init
Initialized empty Git repository in /opt/workspace/wechat/.git/
```
初始化完成之后`.git`中会出现以下的文件
```python
root@ubuntu:/opt/workspace/wechat# ls -l .git/
total 32
drwxr-xr-x 2 root root 4096  5月 26 16:49 branches
-rw-r--r-- 1 root root   92  5月 26 16:49 config
-rw-r--r-- 1 root root   73  5月 26 16:49 description
-rw-r--r-- 1 root root   23  5月 26 16:49 HEAD
drwxr-xr-x 2 root root 4096  5月 26 16:49 hooks
drwxr-xr-x 2 root root 4096  5月 26 16:49 info
drwxr-xr-x 4 root root 4096  5月 26 16:49 objects
drwxr-xr-x 4 root root 4096  5月 26 16:49 refs
root@ubuntu:/opt/workspace/wechat#
```
**注意：**不要删除`.git`中的文件和文件夹，会导致git不能正常使用<br />**设置签名**<br />形式：<br />用户名：tom<br />Email地址：xxx@xxx.com<br />**作用：**区分开发人员的身份<br />辨析：设置的签名和登录远程代码仓库的账号没有任何关系<br />命令：

- 仓库级别`git config`：仅在当前本地库中有效
   - `git config user.name tom_pro`
   - `git config email goodMorning@qq.com`
- 系统用户级别`git config --global`：登录当前操作系统的用户范围
   - `git config --global user.name tom_pro`
   - `git config --global email goodMorning@qq.com`
- 优先级：就近原则项目级别优先于系统用户级别。二者都有时， 采用项目级别。二者都没有的话会报错
- 信息保存的位置是当前项目的`./git/config`

**签名保存的位置**
```bash
仓库级别保存的位置
root@ubuntu:/opt/workspace/wechat# git config user.name tom_pro
root@ubuntu:/opt/workspace/wechat# git config user.email goodtom@qq.com
root@ubuntu:/opt/workspace/wechat# cat .git/config
[core]
        repositoryformatversion = 0
        filemode = true
        bare = false
        logallrefupdates = true
[user]
        name = tom_pro
        email = goodtom@qq.com
root@ubuntu:/opt/workspace/wechat#


全局保存的位置
root@ubuntu:/opt/workspace/wechat# git config --global user.user tom_plus
root@ubuntu:/opt/workspace/wechat# git config --global user.email tom_plus@qq.com
root@ubuntu:/opt/workspace/wechat# cat /root/.gitconfig
```
<a name="Y21uw"></a>
## 添加提交查看状态
```bash
root@ubuntu:/opt/workspace/wechat# git status
On branch master

No commits yet

nothing to commit (create/copy files and use "git add" to track)

```
注：默认没有添加任何的文件<br />添加一个`good.txt`文件查看状态
```bash
root@ubuntu:/opt/workspace/wechat# cat good.txt
aaaaaaaaaa
bbbbbbbbbb
ccccccccc
dddddddd
root@ubuntu:/opt/workspace/wechat# git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        good.txt

nothing added to commit but untracked files present (use "git add" to track)
```
注意：`git status`发现了没有提交的代码`good.txt`
```bash
root@ubuntu:/opt/workspace/wechat# git add good.txt
root@ubuntu:/opt/workspace/wechat# git status
On branch master

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   good.txt

root@ubuntu:/opt/workspace/wechat#

```
`git add`将发现的代码提交到暂存区，默认是在工作区

将暂存区的文件删除`git rm --cached`
```bash
root@ubuntu:/opt/workspace/wechat# git rm --cached good.txt
rm 'good.txt'
root@ubuntu:/opt/workspace/wechat# git status
On branch master

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        good.txt

nothing added to commit but untracked files present (use "git add" to track)

```
将代码提交到本地库`git commit 文件名`，
```bash
root@ubuntu:/opt/workspace/wechat# git commit good.txt
[master (root-commit) 9591448] first commit code by oldjiang
 1 file changed, 4 insertions(+)
 create mode 100644 good.txt
root@ubuntu:/opt/workspace/wechat# git status
On branch master
nothing to commit, working tree clean

```
发现的是将本地的代码都提交上去了，现在本地没有上传的是空的

修改代码测试，将good.txt新增一行文件,发现的是`modified`是已经被修改了
```bash
root@ubuntu:/opt/workspace/wechat# git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   good.txt

no changes added to commit (use "git add" and/or "git commit -a")
root@ubuntu:/opt/workspace/wechat#

```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1653566308488-a89d9bfc-4053-4100-9255-3e45dc949c3c.png#clientId=u2bda250a-865f-4&from=paste&height=410&id=u1385766a&originHeight=564&originWidth=1003&originalType=binary&ratio=1&rotation=0&showTitle=false&size=210196&status=done&style=none&taskId=u7323207d-1c29-427e-bbb5-ad7e466896e&title=&width=729.4545454545455)<br />命令小结
```bash
git status 查看工作区/暂存区的状态
git add 将工作区的代码提交到暂存区
git commit -m "提交代码的描述信息" 将暂存区的代码提交至本地库
```
<a name="MOF5Y"></a>
## 版本的穿梭
优雅的方式查看日志信息
```bash
root@ubuntu:/opt/workspace/wechat# git reflog
f40c6b8 (HEAD -> master) HEAD@{0}: commit: third commit good.txt
5077ca0 HEAD@{1}: commit: second modified good.txt
9591448 HEAD@{2}: commit (initial): first commit code by oldjiang

```
HEAD@{移动到当前版本的步数}<br />基于索引值操作[`推荐`] 查看到的文件就是最初的文件信息，<br />要重置到什么版本就是要那个版本的索引号。
```bash
root@ubuntu:/opt/workspace/wechat# git reflog
8c8d88a (HEAD -> master) HEAD@{0}: commit: the forth commit good code
f40c6b8 HEAD@{1}: commit: third commit good.txt
5077ca0 HEAD@{2}: commit: second modified good.txt
9591448 HEAD@{3}: commit (initial): first commit code by oldjiang
root@ubuntu:/opt/workspace/wechat# git reset --hard 9591448
HEAD is now at 9591448 first commit code by oldjiang
root@ubuntu:/opt/workspace/wechat# cat good.txt
aaaaaaaaaa
bbbbbbbbbb
ccccccccc
dddddddd

root@ubuntu:/opt/workspace/wechat# git reset --hard 8c8d88a
HEAD is now at 8c8d88a the forth commit good code
root@ubuntu:/opt/workspace/wechat# cat good.txt
aaaaaaaaaa
bbbbbbbbbb
ccccccccc
dddddddd
UUUUUUUUUUUU
ZZZZZZZZZZZ
XXXXXXXXXXXXX
CCCCCCCCCCC
VVVVVVVVVVV

```
使用^符号进行  只能后退 `^`代表后退一个版本
```bash
root@ubuntu:/opt/workspace/wechat# git reset --hard HEAD^^^^
HEAD is now at 9591448 first commit code by oldjiang
root@ubuntu:/opt/workspace/wechat# cat good.txt
aaaaaaaaaa
bbbbbbbbbb
ccccccccc
dddddddd
root@ubuntu:/opt/workspace/wechat#

```
使用~符号 `~数字`代表后退的次数
