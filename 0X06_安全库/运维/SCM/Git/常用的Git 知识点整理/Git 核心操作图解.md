Git<br />Git有很多优点。其中很显著的一点，就是版本的分支（branch）和合并（merge）十分方便。<br />有些传统的版本管理软件，分支操作实际上会生成一份现有代码的物理拷贝，而Git只生成一个指向当前版本（又称"快照"）的指针，因此非常快捷易用。<br />但是，太方便了也会产生副作用。如果不加注意，很可能会留下一个枝节蔓生、四处开放的版本库，到处都是分支，完全看不出主干发展的脉络。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850314-4e9092b3-dbdb-42a9-8946-a3b5b3fe3911.png#clientId=u7be21998-25aa-4&from=paste&id=u9aea1286&originHeight=369&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u139c5041-0eb7-43ad-b34b-97078d9676b&title=)<br />那有没有一个好的分支策略呢？答案当然是有的。
<a name="WI6yG"></a>
### 一、主分支Master
首先，代码库应该有一个、且仅有一个主分支。所有提供给用户使用的正式版本，都在这个主分支上发布。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850230-3451dbc5-f35e-433a-9d9a-7e484f52ed0b.png#clientId=u7be21998-25aa-4&from=paste&id=u5c483bed&originHeight=600&originWidth=300&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5f95d9ad-da30-451c-869e-ee1c030c935&title=)<br />Git主分支的名字，默认叫做Master。它是自动建立的，版本库初始化以后，默认就是在主分支在进行开发。
<a name="SXy75"></a>
### 二、开发分支Develop
主分支只用来发布重大版本，日常开发应该在另一条分支上完成。把开发用的分支，叫做Develop。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850236-0b56dc63-0355-4c81-8bf1-02117c1b98f4.png#clientId=u7be21998-25aa-4&from=paste&id=udc40305a&originHeight=553&originWidth=500&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u350b5cc0-a6b2-49e6-bc72-cc9ea49b7ad&title=)<br />这个分支可以用来生成代码的最新隔夜版本（nightly）。如果想正式对外发布，就在Master分支上，对Develop分支进行"合并"（merge）。<br />Git创建Develop分支的命令：
```bash
git checkout -b develop master
```
将Develop分支发布到Master分支的命令：
```bash
# 切换到Master分支
git checkout master

# 对Develop分支进行合并
git merge --no-ff develop
```
这里稍微解释一下上一条命令的`--no-ff`参数是什么意思。默认情况下，Git执行"快进式合并"（fast-farward merge），会直接将Master分支指向Develop分支。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850255-6ea8af1a-08d3-46d0-88d2-620be80528d8.png#clientId=u7be21998-25aa-4&from=paste&id=uebb725a2&originHeight=606&originWidth=500&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u92d5f38d-bd00-4c43-872c-ac2df830763&title=)<br />使用`--no-ff`参数后，会执行正常合并，在Master分支上生成一个新节点。为了保证版本演进的清晰，希望采用这种做法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850305-1c972df7-f114-4fb9-a99c-2a4c143f9525.png#clientId=u7be21998-25aa-4&from=paste&id=u43e21858&originHeight=726&originWidth=250&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u72d0af0d-5aff-4389-a4ad-5e23381f719&title=)
<a name="DwWb9"></a>
### 三、临时性分支
前面讲到版本库的两条主要分支：Master和Develop。前者用于正式发布，后者用于日常开发。其实，常设分支只需要这两条就够了，不需要其他了。<br />但是，除了常设分支以外，还有一些临时性分支，用于应对一些特定目的的版本开发。临时性分支主要有三种：

- 功能（feature）分支
- 预发布（release）分支
- 修补bug（fixbug）分支

这三种分支都属于临时性需要，使用完以后，应该删除，使得代码库的常设分支始终只有Master和Develop。<br />接下来，一个个来看这三种"临时性分支"。<br />**第一种是功能分支**，它是为了开发某种特定功能，从Develop分支上面分出来的。开发完成后，要再并入Develop。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850568-c2c4bb12-20e8-48cd-b3f0-64c48cee37d6.png#clientId=u7be21998-25aa-4&from=paste&id=ua6ce6010&originHeight=727&originWidth=400&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub0506840-c96f-41c5-9c30-ae4dce951fe&title=)<br />功能分支的名字，可以采用`feature-*`的形式命名。<br />创建一个功能分支：
```bash
git checkout -b feature-x develop
```
开发完成后，将功能分支合并到develop分支：
```bash
git checkout develop

git merge --no-ff feature-x
```
删除feature分支：
```bash
git branch -d feature-x
```
**第二种是预发布分支**，它是指发布正式版本之前（即合并到Master分支之前），可能需要有一个预发布的版本进行测试。<br />预发布分支是从Develop分支上面分出来的，预发布结束以后，必须合并进Develop和Master分支。它的命名，可以采用`release-*`的形式。<br />创建一个预发布分支：
```bash
git checkout -b release-1.2 develop
```
确认没有问题后，合并到master分支：
```bash
git checkout master

git merge --no-ff release-1.2

# 对合并生成的新节点，做一个标签
git tag -a 1.2
```
再合并到develop分支：
```bash
git checkout develop

git merge --no-ff release-1.2
```
最后，删除预发布分支：
```bash
git branch -d release-1.2
```
**最后一种是修补bug分支**。软件正式发布以后，难免会出现bug。这时就需要创建一个分支，进行bug修补。<br />修补bug分支是从Master分支上面分出来的。修补结束以后，再合并进Master和Develop分支。它的命名，可以采用`fixbug-*`的形式。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654650850623-52f9bf85-e60e-469e-9c91-16e33cc3ade5.png#clientId=u7be21998-25aa-4&from=paste&id=udbefcafc&originHeight=427&originWidth=500&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubac14448-1456-4554-b84e-943ff751582&title=)<br />创建一个修补bug分支：
```bash
git checkout -b fixbug-0.1 master
```
修补结束后，合并到master分支：
```bash
git checkout master

git merge --no-ff fixbug-0.1

git tag -a 0.1.1
```
再合并到develop分支：
```bash
git checkout develop

git merge --no-ff fixbug-0.1
```
最后，删除"修补bug分支"：
```bash
git branch -d fixbug-0.1
```
