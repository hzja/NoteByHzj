Git<br />推荐个学习git命令即好玩又有趣的神器网站 **Learning Git Branching**，它让学习git不再那么枯燥乏味，话不多说直接上动图。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1684990375920-44763821-9752-44db-a219-ec04be16bcf8.gif#averageHue=%235cbcfb&clientId=u6d7498ec-5864-4&from=paste&id=u49e8935d&originHeight=627&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8050df2e-8943-403b-b429-52aa1b17937&title=)<br />在这里你的每一个操作命令都会实时的以动画形式演示出来，这对于初学者理解分支等概念非常的友好。<br />接下来一起体验下。<br />这个网站可以说是目前为止见过学习git最好的教程了<br />**怕记不住命令? 没关系！**<br />它提供了完善的沙盒命令，在沙盒里你可以实际执行相应的命令，有很好的命令联想功能，不仅如此还能看到每个命令的执行动作情况。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1684990375888-550d5f49-3a82-46b1-a5c9-7cfcaf8f7563.gif#averageHue=%235bbaf9&clientId=u6d7498ec-5864-4&from=paste&id=u483eea47&originHeight=605&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u59b61fe5-d692-45ae-b919-d2a66e6002c&title=)<br />对于每一个git命令都有非常详细的原理解析和使用说明，当点击模拟执行命的时候，还会以动画的形式演示，非常生动。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1684990376182-2044ca2a-61ba-4dc1-8f8a-140078fb3311.gif#averageHue=%234ad34a&clientId=u6d7498ec-5864-4&from=paste&id=u7f3ee688&originHeight=576&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u06e31398-717e-4dd9-aed2-1007fedc5b1&title=)<br />经常有小伙伴说我想学XXX，但是网上的内容都不系统，东偏西凑学着学着就坚持不下去了。<br />这个网站最大的亮点是它通过一系列刺激的关卡挑战，用游戏的形式来学习，给定任务目标，通过提示执行git命令达到预期效果才算过关。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1684990375931-de2f55b4-c5e2-4413-85e8-1a2d107baf2e.gif#averageHue=%235bbafa&clientId=u6d7498ec-5864-4&from=paste&id=ub2888a4c&originHeight=494&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u161b0ffc-c533-448c-af52-15e2825dc2c&title=)<br />从Git命令的基础篇高级篇再到史诗篇，由浅入深逐步深入的学习Git的操作，非常贴合我们实际工作中的场景，在这个过程中可能还会发现一些有意思的事情。<br />下边分享几个高级篇的通关答案，敲了挺长时间才过关，自我提升了不少，哈哈~
<a name="Bu5id"></a>
### 撤销变更
```bash
git reset HEAD^
git checkout pushed
git revert HEAD
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990376475-378c4a04-5100-48df-a584-e15ae50d7ba0.png#averageHue=%23bc9bed&clientId=u6d7498ec-5864-4&from=paste&id=u3c5e122d&originHeight=432&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub4057113-3fc2-4314-aef9-a02edac332a&title=)
<a name="lcXhS"></a>
### 交互式Rebase
```bash
git rebase -i HEAD~4
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990376528-5ad1b623-3a20-4d1e-9c86-2f390edf81cb.png#averageHue=%23bc91ec&clientId=u6d7498ec-5864-4&from=paste&id=ue68013bc&originHeight=466&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc9775464-cce2-4536-bf94-71c77103c86&title=)
<a name="n9pNr"></a>
### 只取一个提交记录
```bash
git rebase -i HEAD~3/git cherry-pick bugFix
git branch -f master bugFix
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990376630-14d4e1b6-30c2-42e5-be73-5c3590bbe151.png#averageHue=%23a492e9&clientId=u6d7498ec-5864-4&from=paste&id=u9deee68c&originHeight=447&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc7c46f41-5eb5-466b-8a76-70cbb34ddd4&title=)
<a name="Kn7Ms"></a>
### Git Tag
```bash
git tag v0 c1
git tag v1 c2
git checkout c2
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990377078-9e7c9942-bbb3-491f-9d13-214aa08cb339.png#averageHue=%23918fe8&clientId=u6d7498ec-5864-4&from=paste&id=u31df8182&originHeight=417&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u653bf55c-8c0d-496a-9997-abcba78cadc&title=)
<a name="Ok6u5"></a>
### 多次Rebase
```bash
git rebase master bugFix
git rebase bugFix side
git rebase side another
git branch -f master another
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990377650-49c2bf2b-9c5c-4b44-9d59-89e1ef57a10d.png#averageHue=%23ae83e3&clientId=u6d7498ec-5864-4&from=paste&id=u49416df3&originHeight=363&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udf45b05b-005e-41e1-adb3-994ab866e1f&title=)
<a name="XYHUy"></a>
### 两个父节点
```bash
git branch bugWork HEAD~^2~
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990377722-039254e8-c97e-44df-a61e-7cf1fc659752.png#averageHue=%23d4d3d4&clientId=u6d7498ec-5864-4&from=paste&id=uf0eb1faa&originHeight=435&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u71888c7f-2f4b-42aa-b32b-e55fd87d3cd&title=)
<a name="KUf3R"></a>
### 纠缠不清的分支
```bash
git checkout one
git cherry-pick c4 c3 c2
git checkout two
git cherry-pick c5 c4 c3 c2
git branch -f three c2
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990378143-5af3813a-200a-442f-a428-76d4675bbd9c.png#averageHue=%23d291f0&clientId=u6d7498ec-5864-4&from=paste&id=ufd1b11e6&originHeight=469&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf89011bc-82a9-4c92-964f-68f62d1799d&title=)
<a name="aCitI"></a>
### 模拟团队合作
```bash
git clone
git fakeTeamwork 2
git commit
git pull
```
<a name="QGWWJ"></a>
### 推送主分支
```bash
git fetch
git rebase o/master side1
git rebase side1 side2
git rebase side2 side3
git rebase side3 master
git push
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990378279-51cd4ac0-493c-495d-a7a4-bbe6fc639e0f.png#averageHue=%2377a7e7&clientId=u6d7498ec-5864-4&from=paste&id=u6e43e294&originHeight=389&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u11b18503-2b91-418c-9af3-fbf712e61fc&title=)
<a name="qgSXp"></a>
### 合并远程仓库
```bash
git checkout master
git pull origin master
git merge side1
git merge side2
git merge side3
git push origin master
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990378712-9b7f542c-a04b-4189-b072-e56c7c5c175a.png#averageHue=%23b3a1e9&clientId=u6d7498ec-5864-4&from=paste&id=ub10c4760&originHeight=389&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u841113c6-69a0-4abf-baa8-78003be30d3&title=)
<a name="qMZM3"></a>
### Git Push的参数
```bash
git push origin foo:master
git push origin master^:foo
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990378958-60f8e395-46bd-498e-b693-f505d13e9c67.png#averageHue=%23b570f3&clientId=u6d7498ec-5864-4&from=paste&id=u35c78ab8&originHeight=403&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9324f0e7-a236-4ecc-9924-3bcf7002ea7&title=)
<a name="oKVvf"></a>
### Git Fetch的参数
```bash
git fetch origin master^:foo
git fetch origin foo:master
git checkout foo
git merge master
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990379115-5b89bd2b-dca4-4795-b5a7-ca5c7cd85b74.png#averageHue=%236a79ef&clientId=u6d7498ec-5864-4&from=paste&id=u135ffdc9&originHeight=410&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u85990604-ca40-477f-80df-3f2b8c260da&title=)
<a name="nOWQK"></a>
### 没有Source的Source
```bash
git pull origin :bar
git push origin :foo
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990379412-531778e0-46c8-499a-b8f1-fdcc6e5c6182.png#averageHue=%23cf7df5&clientId=u6d7498ec-5864-4&from=paste&id=ubff5b94e&originHeight=288&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u308653c5-3291-45f6-ac0c-15a68f6a16e&title=)
<a name="fm7fy"></a>
### Git Pull的参数
```bash
git pull origin bar:foo
git pull origin master:side
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1684990379803-b6be647b-fd57-449f-94cd-4e3cacd3d019.png#averageHue=%23b988ed&clientId=u6d7498ec-5864-4&from=paste&id=uc0d07d69&originHeight=394&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uea9f21f8-cc0f-43cc-af36-59d133a292a&title=)
