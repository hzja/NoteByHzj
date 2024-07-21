<a name="iy0Ds"></a>
## Git撤销已合并提交的多种姿势
在Git中，合并分支是一个常见的操作，但有时候可能会意外地将错误的提交合并到了主分支。这时候需要撤销已合并的提交并恢复到正确的状态。本文将介绍的是如何在Git中撤销已合并的提交，无论这个提交记录是最新的还是中间的某一个。
<a name="JbpRZ"></a>
## 撤销最新合并的一次提交
如果要撤销最新的合并提交，可以使用`git revert`命令来创建一个新的提交，撤销错误的变更。

1. 首先使用`git log`命令查看提交历史，找到最新的合并提交。
```bash
$ git log
commit c3d2e9a4e2a1e285ff4d8f06e01d4e3f19b532ea (HEAD -> master)
Author: Hanmeimei <hanmeimei@example.com>
Date:   Fri Jun 30 15:26:43 2023 +0800

    Incorrect merge commit

commit 3e5fb8a7b631eb6492ef32e28a813084d4d3de2b
Author: Lilei <Lilei@example.com>
Date:   Thu Jun 29 18:20:56 2023 +0800

    Correct commit

...
```
在上面的示例中，大家可以看到最新的错误合并提交（Incorrect merge commit）。

1. 使用`git revert`命令撤销合并提交，并创建一个新的提交来还原到正确的状态。
```bash
$ git revert c3d2e9a4e2a1e285ff4d8f06e01d4e3f19b532ea
```
Git将自动创建一个新的提交，撤销错误的合并提交。

1. 使用`git log`或`git show`命令验证新的提交历史，确认错误的变更已经被撤销。
```bash
$ git log
commit b254d0f063b4ab4e7b78fb42015e0c55e0e98712 (HEAD -> master)
Author: Hanmeimei <hanmeimei@example.com>
Date:   Fri Jun 30 15:46:28 2023 +0800

    Revert "Incorrect merge commit"

    This reverts commit c3d2e9a4e2a1e285ff4d8f06e01d4e3f19b532ea.

commit 3e5fb8a7b631eb6492ef32e28a813084d4d3de2b
Author: Lilei <lilei@example.com>
Date:   Thu Jun 29 18:20:56 2023 +0800

    Correct commit

...
```
在这里可以看到一个新的提交（Revert "Incorrect merge commit"）被创建，它撤销了最新的错误合并提交。
<a name="Eximr"></a>
## 撤销最新合并的多次提交
如果要撤销最新合并的多次提交，可以使用`git reset`命令来回滚到某次提交。以下是步骤：

1. 首先使用`git log`命令查看提交历史，找到要回滚的哈希值。
```bash
$ git log
commit c5b890eee2edf9a353ec6bba0543e41d2529a8f8 (HEAD -> master)
Author: Hanmeimei <hanmeimei@example.com>
Date:   Mon Jul 3 15:12:10 2023 +0800

    Incorrect merge commit

commit 82bcf43083a4dc8c87091ebde4dd5374f0c6e274
Author: Hanmeimei <hanmeimei@example.com>
Date:   Mon Jul 3 15:11:54 2023 +0800

    Incorrect merge commit2
    
commit 60a52b00d0ee2703156231e209e8aad115919aee
Author: Hanmeimei <hanmeimei@example.com>
Date:   Mon Jun 26 06:32:35 2023 +0000

    Correct commit

...
```
在这里需要回滚到（Correct commit）。

1. 使用`git reset` 命令撤销合并提交，并创建一个新的提交来还原到正确的状态。
```bash
$ git reset --soft 60a52b00d0ee2703156231e209e8aad115919aee // 暂存区
$ git reset --hard 60a52b00d0ee2703156231e209e8aad115919aee // HEAD 指向此次提交记录
$ git push origin HEAD --force // 强制推送远端
```
注意：此次提交之后的修改不做任何保留，`git status`查看工作区是没有记录的。

1. 最后，使用`git log`或`git show`命令验证提交历史，确认错误的变更已经被撤销。
```bash
$ git log
commit 60a52b00d0ee2703156231e209e8aad115919aee (HEAD -> master)
Author: Hanmeimei <hanmeimei@example.com>
Date:   Mon Jun 26 06:32:35 2023 +0000

    Correct commit

...
```
如果出现了误删，可以用以下办法来恢复：
```bash
$ git reset --hard 82bcf43083a4dc8c87091ebde4dd5374f0c6e274 // 误删的哈希值

 HEAD is now at 82bcf4308 feat: Incorrect merge commit2
 
$ git push origin HEAD --force // 强制推送远端
```
<a name="MAswR"></a>
## 撤销中间合并某一个提交
如果要撤销中间的某一个合并提交，可以使用`git revert`命令，并指定要撤销的提交哈希值。

1. 首先使用`git log`命令查看提交历史，并找到要撤销的中间合并提交。
```bash
$ git log
commit 3e5fb8a7b631eb6492ef32e28a813084d4d3de2b (HEAD -> master)
Author: Lilei <lilei@example.com>
Date:   Wed Jun 21 12:00:00 2023 +0000

    Correct commit
    
commit a1b2c3d4e5f6a7b8c9d0e1f2g3h4i5j6k7l8m9n0
Author: Hanmeimei <hanmeimei@example.com>
Date:   Fri Jun 18 12:00:00 2023 +0000

    Incorrect merge commit
    
...
```
在这里可以看到一个中间的错误合并提交（Incorrect merge commit）。

1. 然后使用`git revert`命令撤销合并提交，并创建一个新的提交来还原到正确的状态。
```bash
$ git revert a1b2c3d4e5f6a7b8c9d0e1f2g3h4i5j6k7l8m9n0
```
Git将自动创建一个新的提交，撤销错误的合并提交。

1. 最后使用`git log`或`git show`命令验证新的提交历史，确认错误的变更已经被撤销。
```bash
$ git log
commit b254d0f063b4ab4e7b78fb42015e0c55e0e98712 (HEAD -> master)
Author: Hanmeimei <hanmeimei@example.com>
Date:   Mon Jun 28 12:10:00 2023 +0000

    Revert "Incorrect merge commit"

    This reverts commit a1b2c3d4e5f6a7b8c9d0e1f2g3h4i5j6k7l8m9n0.

commit 3e5fb8a7b631eb6492ef32e28a813084d4d3de2b
Author: Lilei <lilei@example.com>
Date:   Wed Jun 21 12:00:00 2023 +0000

    Correct commit
    
commit a1b2c3d4e5f6a7b8c9d0e1f2g3h4i5j6k7l8m9n0
Author: Hanmeimei <hanmeimei@example.com>
Date:   Fri Jun 18 12:00:00 2023 +0000

    Incorrect merge commit

...
```
可以看到之前的提交仍会保留在`git log`中，而此次撤销会做为一次新的提交（Revert "Incorrect merge commit"）。
<a name="zJj9x"></a>
## 注意
在执行撤销操作之前，请一定要确保你了解操作的后果，并在进行任何更改之前创建备份或保存重要的数据。
