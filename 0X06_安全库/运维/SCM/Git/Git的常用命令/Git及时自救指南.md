Git<br />Git 虽然因其分布式管理方式，不完全依赖网络，良好的分支策略，容易部署等优点，已经成为最受欢迎的源代码管理方式。但是一分耕耘一分收获，如果想更好地掌握 git，需要付出大量的学习成本。即使在各种 GUI 的加持下，也不得不说 git 真的很难，在 V2EX 上也常有如何正确使用 git 的讨论，同时在 Stackoverflow 上超过 10w+ 的 git 相关问题也证明了 git 的复杂性。再加上 git 的官方文档也一直存在着 “先有鸡还是先有蛋” 的问题，虽然文档非常全面，但如果不知道遇到的问题叫什么，那么根本就无从查起。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288660003-ffdcc0a4-3241-4624-92a9-ee103586bf75.webp#clientId=u5a5056ee-5915-4&from=paste&id=u1a08c072&originHeight=451&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u082041da-7174-4ce2-a2e6-31661f2f462)<br />作为国内领先的研发管理解决方案供应商，CODING 一直致力于在国内普及 git 的使用，为软件研发提供更高效率。本文节选自 Katie Sylor-Miller 在日常工作中所遇到过的让他很头疼的 git 相关问题，并整理了相应的应对措施，在这里分享给正在学习如何使用 git 的同学们。当然这些应对措施并不是唯一的，可能会有其他更好的应对方法，这也恰恰是 git 这套版本控制系统强大的地方。
<a name="VqGFR"></a>
## 1、搞错了一个很重要的东西，git 有个神奇的时间机器能复原！
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288660154-c55981a6-b955-4647-91dc-f60ae90d05e3.webp#clientId=u5a5056ee-5915-4&from=paste&id=u0bbecb4a&originHeight=426&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ua71b608b-96b4-4b18-973a-02a20a98666)<br />`reflog` 是一个非常实用的命令，可以使用这个命令去找回无意间删除的代码，或者去掉一些刚刚添加的却把仓库里的代码弄坏的内容。同时也可以拯救一下失败的 `merge`，或者仅仅是为了回退到之前的版本。
<a name="YAW4z"></a>
## 2、commit 完才想起来，还有一处小地方要修改！
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288659884-a5e1b3f3-6839-41ff-a0a7-cce08077bb73.webp#clientId=u5a5056ee-5915-4&from=paste&id=u6ab35e63&originHeight=375&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u65a225c8-a21e-4d9e-8259-9aec096426c)<br />当 `commit` 完然后跑测试的时候，经常突然发现忘了在等于号前面加空格。虽然可以把修改过的代码再重新 `commit` 一下，然后 `rebase -i` 将两次揉在一起，不过上面的方法会比较快。
<a name="Evc4y"></a>
## 3、要改一下上一个 commit message！
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288659894-8c37cbcb-4400-41b3-a315-890a1b2bbcc6.webp#clientId=u5a5056ee-5915-4&from=paste&id=uc9cd83c3&originHeight=353&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ub0cee5ab-9c6e-4f0c-97e5-721317f9c81)<br />当项目组对 commit message 有格式要求时，或者当忘了中英文间要加空格，这个命令能救命。
<a name="OEcRy"></a>
## 4、不小心把本应在新分支上的内容，commit 到 master 了！ ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288660039-4bc2a83f-bbd0-4c5a-9538-398d0b6d2c0a.webp#clientId=u5a5056ee-5915-4&from=paste&id=ud295bf3f&originHeight=400&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ud0e77268-a65e-464b-b7f9-1cc783f41d2)
注意：这个指令必须在错误的 `commit` 后直接执行，如果已经试了其他的方式，可能就需要用 `git reset HEAD@{number}` 来代替 `HEAD~` 了。
<a name="Oz6f3"></a>
## 5、不小心 commit 到错误的分支上了！ 
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288662308-cd7643be-58e7-42b4-86f2-7eb8118094c8.webp#clientId=u5a5056ee-5915-4&from=paste&id=ucad9e843&originHeight=478&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ud99b433a-cc18-4d54-89b7-8cb337aad42)<br />也有很多人推荐了 cherry-pick 的解决方案，所以选哪个就看心情了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288662624-bfd8a3bf-78bb-46be-938f-1f72b32471fe.webp#clientId=u5a5056ee-5915-4&from=paste&id=ua5706d14&originHeight=400&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=ube136aad-f42c-47d1-934b-9a32188e0fc)
<a name="zy8of"></a>
## 6、执行了 diff 但是啥也没出现 
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288662577-f5e3b6b0-61a1-485d-a84e-aa1290d8fed9.webp#clientId=u5a5056ee-5915-4&from=paste&id=u093c8bbb&originHeight=271&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u28e04fe9-eabe-4958-b72e-8a1d07d81a0)<br />Git 不会给通过 `add` 加入到 `staging` 区域里面的文件做 `diff` ，除非加了 `--staged` 的标签，别怀疑了这是一个 feature 不是一个 bug，当然对于第一次碰到这个问题的人来说还是有些不好理解的。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288663006-7e0c6031-8703-41fb-af77-e58e567e1266.webp#clientId=u5a5056ee-5915-4&from=paste&id=u17ad0969&originHeight=400&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u21491039-aa57-4c43-b9b5-6b79f4f45a1)
<a name="jBNJt"></a>
## 7、Git 从入门到放弃
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636288663025-3fd70429-0933-4eb5-ae82-75f94c0c9b95.webp#clientId=u5a5056ee-5915-4&from=paste&id=u24f9be2b&originHeight=348&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u7049053d-be76-4561-8c43-c8e48708de4)
