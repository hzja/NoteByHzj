Git<br />内网提交需要校验企业邮箱，但有时邮箱设置错误导致 commit 的邮箱有问题，此时可以通过修改已提交记录中的邮箱来修复，无需重新提交。<br />经过检索，发现两种方法，分别适用于修改一次和修改多次，引文在最后都有注明。
<a name="FsGcr"></a>
## 修改最近一次提交的邮箱
```bash
$ git commit --amend --author="NewAuthor <NewEmail@address.com>"
```
<a name="JvkAc"></a>
## 批量修改邮箱
使用该脚本，替换其中 [Your Old Email] [Your New Author Name] [Your New Email] 之后在 git 目录中执行即可。
```bash
#!/bin/sh

git filter-branch --env-filter '

an="$GIT_AUTHOR_NAME"
am="$GIT_AUTHOR_EMAIL"
cn="$GIT_COMMITTER_NAME"
cm="$GIT_COMMITTER_EMAIL"

if [ "$GIT_COMMITTER_EMAIL" = "[Your Old Email]" ]
then
    cn="[Your New Author Name]"
    cm="[Your New Email]"
fi
if [ "$GIT_AUTHOR_EMAIL" = "[Your Old Email]" ]
then
    an="[Your New Author Name]"
    am="[Your New Email]"
fi

export GIT_AUTHOR_NAME="$an"
export GIT_AUTHOR_EMAIL="$am"
export GIT_COMMITTER_NAME="$cn"
export GIT_COMMITTER_EMAIL="$cm"
```
<a name="hMMDL"></a>
## Q&A

- A previous backup already exists in refs/original/
```
Cannot create a new backup.
A previous backup already exists in refs/original/
Force overwriting the backup with -f
```
出现这一句说明之前曾经执行过 git filter-branch ，在 refs/original/ 有一个备份，这个时候只要删掉那个备份即可，删除备份命令为:
```bash
$ git update-ref -d refs/original/refs/heads/master
# 或
$ git filter-branch -f --tree-filter -f 'rm -f test' -- --all
```
