<a name="nxUaR"></a>
## 1、序
Git的rebase命令是一个非常强大的命令，关于这个命令的介绍文章网上很多，但是大多理论性比较强，所以这里结合几个非常典型的应用场景来谈一下rebase命令的使用。<br />说实话，rebase这个操作极少用到，大部分时候老老实实merge就行了。说到rebase这个单词，有些文章翻译成“变基”，实在有些生硬，但是从字面意思上看base有着基础的含义，比如每次修改代码的时候是基于上一次的基础，这是一个线性的过程，而rebase则是打破这个线性过程，改变变更的基础，达到一些特殊的目的。
<a name="S9A8S"></a>
## 2、合并提交记录
通常情况下，每次对代码进行一些修改都会commit一下，时间久了，提交的记录就非常多，比如下面图中：<br />![提交记录](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592246151-c8d788e4-d977-4fb7-87b9-b51969a83f0f.png#averageHue=%23414b54&clientId=u928695ef-98d5-4&from=paste&id=uf41446d4&originHeight=324&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9f056d1f-3a32-472c-8da0-7f690a4b16e&title=%E6%8F%90%E4%BA%A4%E8%AE%B0%E5%BD%95 "提交记录")<br />个人对此并不反感，很多时候提交记录详细一点方便以后排查bug、甚至甩锅，谁的提交记录就是谁写的代码、做的修改，一目了然，这玩意也不占多少空间，但是有些特殊情况，比如对外开源的项目在发布的时候希望有一个干净的commit记录。<br />或者，纯粹是你的leader搞事情，觉得提交记录这么多看着不舒服。。。anyway，这时候使用rebase命令就可以轻松搞定。<br />整体过程可以分为几步：
<a name="aog0o"></a>
### 第一步，获取commit id
比如上图里面这些提交记录，想把前面4次记录合并到“重构”这次，那么就需要先获取“重构”这个提交记录 **前面** 那一次提交的commit id，就是那一串hash值，使用`git log`命令：
```
commit 01da1d7351dfea156eabc7ddf36a9e8eba227645 (tag: v1.1)
Author: wangbenjun <wangbenjun@gmail.com>
Date:   Fri Nov 19 22:00:16 2021 +0800

    重构

commit a0645baeb9938f34658f2766a2d4ec2855791da1 (tag: v1.0)
Author: wangbenjun <wangbenjun@gmail.com>
Date:   Wed Aug 18 20:47:34 2021 +0800

    优化目录结构
```
<a name="CT30i"></a>
### 第二步，`git rebase -i a0645baeb`
git rebase这个命令参数很多，这里就不多介绍，不然又有点overwhelming了，感兴趣的童鞋自行研究，这里只说-i，参数是commit id，填一部分就行。<br />然后会自动打开一个编辑页面，可能是nano，也可能是vi，取决于系统的配置。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592246304-f2983c63-6921-48ab-9078-bb52b7c13f63.png#averageHue=%232e2d2d&clientId=u928695ef-98d5-4&from=paste&id=u4400b7f9&originHeight=562&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4829d746-0474-40ed-a37c-6f1cb07b6ab&title=)<br />很多人一看到这个多内容可能就有点慌了，不知所措，其实大部分都是注释，只有最上面那几行是有用的，可以看到默认是 pick xxx，这个pick啥意思？<br />其实下面就有介绍，p表示use commit，意思就是采用这个提交，然后还有r、e、s、f等命令，这里咱也不多说，只看s，这个s表示采用提交并合并到上一次提交里面。<br />然后需要把重构后面的记录前面的pick全部改成s，意思是把后面4个提交合并到前面那一次提交里面。
<a name="U0O4M"></a>
### 第三步，注释并保存
把pick改成s之后保存，又会自动弹出一个新的编辑页面，内容大概如下：
```
# This is a combination of 5 commits.
# This is the 1st commit message:

重构

# This is the commit message #2:

fix

# This is the commit message #3:

update

# This is the commit message #4:

fix
```
其大意是这是这5次提交信息的合并，然后它会自动把每次提交的信息都给你列出来，这里可以选择直接保存，也可以顺便修改一下，改成你想要的内容，然后保存。
<a name="cHPmi"></a>
### 第四步，推送到远程
完成上面的操作之后，再次使用git log则会发现提交记录已经变了，但是目前还只是在本地，还需要推送到远程，而且推送的时候必须使用--force，如：
```bash
git push origin --force
```
完成上述操作之后，整个git提交记录就会完全改变了，而且是不可逆操作，当然这也意味着rebase操作是有一定风险的，如果不太清楚需要做什么的话就不要做。

---

结合以上例子，大家可能会发现rebase是可以拥有修改提交记录的能力，所以基于此，可以衍生出以下应用场景：
<a name="pHXhJ"></a>
## 3、删除敏感信息
假如公司有人把一些敏感信息，比如账号密码上传到git上面去了，第一反应可能是删掉密码再提交一次。。。然而这有点掩耳盗铃的意思，历史提交记录里面还有呢<br />![历史提交记录](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592246290-f54a5fcc-492d-4d94-bad7-4c9b8ea7fa64.png#averageHue=%23404e60&clientId=u928695ef-98d5-4&from=paste&id=uf7756063&originHeight=223&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u5c65a39e-d46e-443d-90b4-e9359b88c64&title=%E5%8E%86%E5%8F%B2%E6%8F%90%E4%BA%A4%E8%AE%B0%E5%BD%95 "历史提交记录")<br />这时候就可以使用rebase修改提交记录，把密码出现过的地方全部抹除掉，相当于没有发生过，过程和上述一样。
<a name="PjMt4"></a>
## 4、偷别人的代码
假如你们公司按代码提交量算绩效，你这周摸鱼没有写代码，但是你同事写了很多代码，可以使用rebase把别人的提交记录修整一番，然后你同事的提交记录就没了，变成你的了...<br />哈哈，此招太贱，可能会挨打，注意使用场景！
<a name="yU3uV"></a>
## 5、清理提交记录
有些同学在使用git的时候不知道有stash这个功能，所以每次切换分支的时候都喜欢commit，导致这个commit记录超级多，而且很多无意义的信息，虽然无大雅，但是有些追求极致的leader看不下去， 比如：
```
fix bug
fix bug
fix bug
fix bug
...
```
或者有时候在调试代码的时候，必须提交到代码库才能部署，也会导致出现很多重复无意义的提交，比较难看。<br />这时候也可以使用reabse来合并这部分提交，值得一说的是rebase -i支持区间，从某一个commit到另一个commit。
<a name="RrMR8"></a>
## 6、合并代码
rebase本身也可以用于合并代码，比如说你在一个开发分支dev上面开发，啪啪啪一堆提交记录，开发完成之后，你想往master合并，如果直接merge过去会导致主分支看上去也很杂乱，碰到一个事多的leader可能又不爽你了，这时候你咋办呢？<br />这时候你也可以使用rebase，只不过这时候的base是主分支，不是commit id了，比如：
```bash
git rebase -i master
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683592246234-77608176-e58e-4c49-9a05-283232c9645a.png#averageHue=%23767979&clientId=u928695ef-98d5-4&from=paste&id=u593dffb3&originHeight=485&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucaac583e-dd39-42b5-a64a-49f2c3c2188&title=)<br />顺便说一下，`git pull`的时候默认自动merge，其实还可以选择rebase，这时候rebase的作用就是类似合并，只不过在时间线上面有差异。<br />最后，总结一下，那就是rebase很强很厉害，可以随意操纵历史提交记录，实现特殊的需求，不知道上面举的这些例子有没有你刚好用到的。
