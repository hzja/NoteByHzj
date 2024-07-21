Git<br />来看 git 的一些「高级」用法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662425766637-7a3d97da-32b4-4119-8eb2-ee5648f90cb6.png#clientId=u4c034602-3be0-4&from=paste&id=u45615198&originHeight=567&originWidth=779&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9b280745-90b7-414d-a494-fe4ff54244e&title=)

---

<a name="IWDj6"></a>
### 导航 —— 跳到之前的分支
```bash
git checkout -
```
<a name="zAeyM"></a>
### 查看历史
```bash
# 每个提交在一行内显示
git log --oneline

# 在所有提交日志中搜索包含「homepage」的提交
git log --all --grep='homepage'

# 获取某人的提交日志
git log --author="Maxence"
```
<a name="OM0PA"></a>
### 之前重置了一个不想保留的提交，但是现在又想要回滚？
```bash
# 获取所有操作历史
git reflog

# 重置到相应提交
git reset HEAD@{4}
# ……或者……
git reset --hard <提交的哈希值>
```
<a name="thd1s"></a>
### 本地仓库搞得一团糟，应该怎么清理
```bash
git fetch origin
git checkout master
git reset --hard origin/master
```
<a name="zSumv"></a>
### 查看当前分支和 master 的不同
```bash
git diff master..my-branch
```
<a name="yOTua"></a>
### 定制提交
```bash
# 编辑上次提交
git commit --amend -m "更好的提交日志"

# 在上次提交中附加一些内容，保持提交日志不变git add . && git commit --amend --no-edit

# 空提交 —— 可以用来重新触发 CI 构建
git commit --allow-empty -m "chore: re-trigger build"
```
<a name="twSQy"></a>
### squash 提交
比方说想要 rebase 最近 3 个提交：
```bash
- git rebase -i HEAD~3
- 保留第一行的 pick，剩余提交替换为 squash 或 s
- 清理提交日志并保存（vi 编辑器中键入 :wq 即可保存）
pick 64d26a1 feat: add index.js
s 45f0259 fix: update index.js
s 8b15b0a fix: typo in index.js
```
<a name="lhBzH"></a>
### 修正
比方说想在提交 fed14a4c 加上一些内容。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662425766631-027aed9b-ff92-4dca-aef8-eaa1ba9cc804.png#clientId=u4c034602-3be0-4&from=paste&id=u374cc6eb&originHeight=248&originWidth=532&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda60eb4f-2573-43a2-ad29-983edf12cfa&title=)
<a name="iFKLP"></a>
### git 提交分支
```bash
git add .
git commit --fixup HEAD~1
# 或者也可以用提交的哈希值（fed14a4c）替换 HEAD~1

git rebase -i HEAD~3 --autosquash
# 保存并退出文件（VI 中输入 `:wq`）
```
<a name="vCRXL"></a>
### rebase 的时候在每个提交上执行命令
如果特性很多，一个分支里可能有多个提交。如果测试失败了，希望能找到导致测试失败的提交。这时候可以使用 `rebase --exec` 命令在每个提交上执行命令。
```bash
# 在最近 3 个提交上运行 `npm test` 命令
git rebase HEAD~3 --exec "npm test"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662425766747-f63aaed7-48c0-4c88-9970-4ac0eaeda8c2.png#clientId=u4c034602-3be0-4&from=paste&id=u393d5f58&originHeight=284&originWidth=532&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u635bbc5d-2169-4523-86a5-e50138c2137&title=)
<a name="jWKob"></a>
### 暂存
暂存不止是 `git stash` 和 `git stash pop` ;)
```bash
# 保存所有正在追踪的文件
git stash save "日志信息"

# 列出所有的暂存项
git stash list

# 获取并删除暂存项
git stash apply stash@{1}
git stash drop stash@{1}
# ……或使用一条命令……
git stash pop stash@{1}
```
<a name="bTTUU"></a>
### 清理
```bash
# 移除远程仓库上不存在的分支
git fetch -p

# 移除所有包含 `greenkeeper` 的分支
git fetch -p && git branch --remote | fgrep greenkeeper | sed 's/^.\{9\}//' | xargs git push origin --delete
```
<a name="GI2Zq"></a>
### GitHub = Git + Hub
把 Hub 当成 git 的一个封装来用。如果也想这么做，可以设置一个别名：`alias git='hub'`<br />**打开浏览器访问仓库 url（仅限 GitHub 仓库）**
```bash
git browse
```
<a name="ENfB3"></a>
### 最常用的 git 别名
```bash
alias g='git'
alias glog='git log --oneline --decorate --graph'
alias gst='git status'
alias gp='git push'
alias ga='git add'alias gc='git commit -v'

# 🤘
alias yolo='git push --force'
```
```bash
# 每周站会汇报工作时用
git-standup() {
    AUTHOR=${AUTHOR:="`git config user.name`"}

    since=yesterday
    if [[ $(date +%u) == 1 ]] ; then
        since="2 days ago"
    fi

    git log --all --since "$since" --oneline --author="$AUTHOR"
}
```
