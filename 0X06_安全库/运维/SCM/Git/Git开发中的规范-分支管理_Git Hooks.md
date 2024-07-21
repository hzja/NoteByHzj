Git
<a name="yRpmf"></a>
# Git** 分支管理规范**

- 实际开发的时候，一人一条分支。除此之外还要有一条 develop 开发分支，一条 test 测试分支，一条 release 预发布分支。
   - **「develop」**：**「开发分支」**，开发人员每天都需要拉取/提交最新代码的分支；
   - **「test」**：**「测试分支」**，开发人员开发完并自测通过后，发布到测试环境的分支；
   - **「release」**：**「预发布分支」**，测试环境测试通过后，将测试分支的代码发布到预发环境的分支（**「这个得看公司支不支持预发环境，没有的话就可以不采用这条分支」**）；
   - **「master」**：**「线上分支」**，预发环境测试通过后，运营/测试会将此分支代码发布到线上环境；
- 大致流程：
   - 开发人员每天都需要拉取/提交最新的代码到 **「develop 分支」**；
   - 开发人员开发完毕，开始 **「集成测试」**，测试无误后提交到 **「test 分支」**并发布到测试环境，交由测试人员测试；
   - 测试环境通过后，发布到 **「release 分支」** 上，进行预发环境测试；
   - 预发环境通过后，发布到 **「master 分支」**上并打上标签（tag）；
   - 如果线上分支出了 bug ，这时候相关开发者应该基于预发布分支（**「没有预发环境，就使用 master 分支」**），新建一个 **「bug 分支」**用来临时解决 bug ，处理完后申请合并到 预发布 分支。这样做的好处就是：不会影响正在开发中的功能。

**「预发布环境的作用：」** 预发布环境是正式发布前最后一次测试。因为在少数情况下即使预发布通过了，都不能保证正式生产环境可以100%不出问题；预发布环境的配置，数据库等都是跟线上一样；有些公司的预发布环境数据库是连接线上环境，有些公司预发布环境是单独的数据库；如果不设预发布环境，如果开发合并代码有问题，会直接将问题发布到线上，增加维护的成本。
<a name="pjiqk"></a>
# Git 钩子

- 在使用 Git 的项目中，可以为项目设置 Git Hooks 在提交代码的各个阶段做一些代码检查等工作
- 钩子（Hooks） 都被存储在 Git 目录下的 hooks 子目录中。也就是绝大部分项目中的 .git/hook 目录
- 钩子<br />分为两大类，客户端的和服务器端的
   - 客户端钩子主要被提交和合并这样的操作所调用
   - 而服务器端钩子作用于接收被推送的提交这样的联网操作，这里主要介绍客户端钩子
<a name="8BQeA"></a>
## Hook{钩子}官网文档
[Hook{钩子}官网文档](https://git-scm.com/book/zh/v2)<br />[https://git-scm.com/book/zh/v2](https://git-scm.com/book/zh/v2)
<a name="6b7Uj"></a>
## pre-commit

- `pre-commit` 就是在代码提交之前做些东西，比如代码打包，代码检测，称之为钩子（hook）
- 在 commit 之前执行一个函数（callback）。这个函数成功执行完之后，再继续 commit，但是失败之后就阻止 commit
- 在 .git->hooks->下面有个 pre-commit.sample* ，这个里面就是默认的函数(脚本)样本
<a name="cuNRM"></a>
### 安装 pre-commit
```bash
npm install pre-commit --save-dev
```
<a name="BiT8y"></a>
### 配置脚本
**「如果没有在 **`**.git->hooks**`** 目录下生成 **`**pre-commit**`** 文件的话，则要手工创建 **`**node ./node_modules/pre-commit/install.js**`**」**
```javascript
"scripts": {
    "build": "tsc",
    "eslint": "eslint src --ext .ts",
    "eslint:fix": "eslint src --ext .ts --fix"
  },
//在提交代码之前，先执行 scripts 中的 eslint 命令
"pre-commit": [
   "eslint"
]
```
<a name="574Ds"></a>
# Git在项目开发中常见问题
<a name="qztgz"></a>
## 1、拉取别人的远程分支合并后，git 会存取这个拉取的记录，如果不小心删了别人的上传的文件，这时候想要再拉取别人的分支是没用的，会显示 already-up
这时候可以回滚代码，重新拉取。
<a name="go0UD"></a>
## 2、以前有过这样的经历：前后端、客户端的代码都存放在一个 git 仓库中，在根目录下各自新建项目目录。那么可以直接在自己的项目目录下使用 git 提交代码并且在各自的项目目录下配置 .gitignore 文件，不用在根目录下配置 .gitignore 文件，这样就互不影响了
<a name="gO755"></a>
## 3、fatal：refusing to merge unrelated histories 拒绝合并不相关的历史
git 2.9.2 之后，不可以合并没有相同结点的分支（分支之间自仓库建立后，从来没有过互相拉取合并）。如果需要合并两个不同结点的分支，如下：
```bash
$ git pull origin branchName --allow-unrelated-histories
$ git merge branchName --allow-unrelated-histories
```
这个功能是可以让大家不要把仓库上传错了，如果会加上这个代码，那么就是自己确定了上传。旧版本的 Git 很容易就把代码传错了，现在可以看到，如果上传的不是之前的，那么就需要加代码上传。正常情况下，都是先建立仓库，然后切多个分支，分支先去拉取合并主分支的内容，然后再各自开发， 如果建立仓库后，各个分支没有去拉取主分支的代码，之后各个分支之间想要合并时就会报错。
<a name="BEcJq"></a>
## 4、合并分支时出现问题，想要解除合并状态
```bash
error: merge is not possible because you have unmerged files.
hint: Fix them up in the work tree, and then use 'git add/rm <file>'
hint: as appropriate to mark resolution and make a commit.
fatal: Exiting because of an unresolved conflict.
```
当远程分支和本地分支发生冲突后，git 保持合并状态，如果没有去解决完所有的冲突，那么 git 会一直保持这个状态，就无法再提交代码。只有先解除合并状态后，才能继续提交。执行命令前最好先备份一下，有可能本地做的修改会被远程分支覆盖掉。
```bash
# 解除合并状态
$ git merge --abort
```
<a name="lwmsX"></a>
## 5、不小心把某些文件上传到远程 git 仓库/想要删除远程仓库中的文件
```bash
# 删除暂存区和工作区的文件
$ git rm filename
# 只删除暂存区的文件，不会删除工作区的文件
$ git rm --cached filename
```
如果在配置 .gitignore 文件之前就把某个文件上传到远程仓库了，这时候想把远程仓库中的该文件删除，此时配置 .gitignore 文件也没有用，因为该文件已经被追踪了，但又不想在本地删除该文件后再重新提交到远程仓库，这时候可以使用 `git rm --cached filename` 命令取消该文件的追踪，这样下次提交的时候，git 就不会再提交这个文件，从而远程仓库的该文件也会被删除
<a name="GMxBk"></a>
## 6、将本地新建的项目上传到新建的远程仓库上
之前没有进行过关联，即没有通过 clone 远程项目到本地再开始做项目，而是先本地新建了一个项目，然后想传到远程仓库上。
```bash
# 将本地仓库和远程仓库关联起来
$ git remote add origin 远程仓库地址
# 将本地的 master 分支推送到 origin 主机，同时指定 origin 为默认主机
$ git push -u origin master
# 上面的命名执行后，下次再从本地库上传内容的时候只需下面这样就可以了
$ git push
```
<a name="S4ANy"></a>
## 7、HEAD 指针既可以指向分支（默认指向当前分支），也可以指向快照，当指向的是快照时，这个时候指针的状态称之为游离状态（detached）
<a name="3og3q"></a>
## 8、创建与合并分支的原理
<a name="M7R7W"></a>
## 9、每次 `git push` 都要输入用户名、密码

- step 1：生成公钥
```bash
ssh-keygen -t rsa -C "fcscanf@outlook.com"
# Generating public/private rsa key pair...
# 三次回车即可生成 ssh key
```

- step 2：查看已生成的公钥
```bash
cat ~/.ssh/id_rsa.pub
```

- step3：复制已生成的公钥添加到 git 服务器

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593326592348-fd42f5f1-5162-4e62-9059-2643fa2a3086.png#height=687&id=yAHDi&originHeight=2060&originWidth=3798&originalType=binary&ratio=1&size=691725&status=done&style=shadow&width=1266)

- step4：测试 ssh 是否能够连接成功

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1593326688928-035ac06c-c3ff-473a-83f4-2db4673c6ff1.png#height=175&id=bmvVF&originHeight=526&originWidth=2143&originalType=binary&ratio=1&size=83653&status=done&style=none&width=714.3333333333334)

- step5：使用 ssh 协议 clone 远程仓库 或者 如果已经用 https 协议 clone 到本地了，那么就重新设置远程仓库

**「使用 ssh 协议」**
```bash
$ git remote set-url origin git@xxx.com:xxx/xxx.git
```

- step5：创建文件存储用户名和密码

一般为 C:\users\Administrator，也可以是自己创建的系统用户名目录，文件名为 .git-credentials。由于在 Windows 中不允许直接创建以 "." 开头的文件，所以用命令行创建该文件。
```bash
$ touch .git-credentials
$ echo "http://{username}:{password}@github.com" >> ~/.git-credentials
$ git config --global credential.helper store
```
<a name="PDpjO"></a>
## 10、git 不允许提交空文件夹
可以在当前目录下，添加一个 .gitkeep 文件
<a name="clQkw"></a>
## 11、有时候复制过来的文件，使用 `git add .` 无法进行文件追踪，可以使用 `git add -A` ，这也就是每次提交前都要 `git status` 的原因
<a name="sBLdU"></a>
## 12、同一台电脑配置多个 git 账号
<a name="deoQY"></a>
## 13、Another git process seems to be running in this repository, e.g.
原因在于 Git 在使用过程中遭遇了奔溃，部分被上锁资源没有被释放导致的。<br />**「解决方案：」** 进入项目文件夹下的 .git 文件中（显示隐藏文件夹或 `rm .git/index.lock`）删除 index.lock 文件即可。
<a name="wCjwg"></a>
## 14、`git commit -am "xxx"` 有时候会失效，无法提交所有的修改
`git commit -am "xxx"` 只会将被 **「tracked」** 的文件添加到暂存区并提交，而将文件添加到 git 管理是要使用 git add 命令，将新的文件 **「tracked」** 。（新建了文件之后，idea 会提示是否需要加到 git 管理中。选择记住后，之后 idea 默认都会把新建的文件 **「tracked」** 化）
<a name="U1cF9"></a>
## 15、`git merge --no-ff` 的作用
**「禁止快进式（fast-forward）合并，会生成一个新的提交」**<br />从合并后的代码来看，结果都是一样的，区别就在于 `--no-ff` 会让 git 生成一个新的提交对象。为什么要这样？通常把 master 作为主分支，上面存放的都是比较稳定的代码，提交频率也很低，而 feature 是用来开发特性的，上面会存在许多零碎的提交，快进式合并会把 feature 的提交历史混入到 master 中，搅乱 master 的提交历史。所以如果根本不在意提交历史，也不爱管 master 干不干净，那么 `--no-ff` 其实没什么用。
<a name="TjEZW"></a>
## 16、`git merge` 与 `git rebase` 的区别
<a name="k7ZUp"></a>
## 17、`git log` 无法正常显示中文
```bash
# 试试
$ git -c core.pager=more log
# 如果可以显示中文的话，把 pager 设置为 more
$ git config --global core.pager more
```
<a name="qOUYt"></a>
## 18、`git merge -m "xxx"` 的时候可以附加信息
默认是 Merge branch branchName
<a name="9LFPw"></a>
## 19、`git pull` 会拉取所有远程分支的代码到本地镜像仓库中
想要 merge 别人的分支时：

- 如果本地仓库中已经有了他人的分支（直接切换到他人分支，就会在本地生成一条他人的分支），就可以使用 merge branchname；
- 如果本地仓库没有他人的分支，那么就得使用 merge origin/branchname 来合并
<a name="w9DWs"></a>
## 20、`git branch -r/-a/-l` 查看的都是本地镜像仓库中的分支，如果本地镜像仓库没有拉取远程仓库的代码，此时别人新推了一个分支到远程仓库，这时候是查看不到这条新推的分支的
<a name="3djFM"></a>
## 21、`git stash` 存储未追踪的文件
如果新建了文件，但是没有用 `git add .` 追踪文件，那么 `git stash` 是无法存储的
```bash
$ git stash -u
```
<a name="rAgZQ"></a>
## 22、如何在 github 上 pr 项目
<a name="rqT6O"></a>
## 23、`git push` 无法提交代码
**「可能出现的报错：」**

- **「remote:」** Permission to xxxxx.git denied to xxx. fatal: unable to access 'github.com/ xxxxx.git/': The requested URL returned error: 403
- **「remote:」** You do not have permission to push to the repository via HTTPS<br />**「fatal:」** Authentication failed for 'gitee.com/xxx.git/'
```bash
# 查看当前项目的 git 配置
$ cat .git/config
```
查看本地项目的 .git/config 设置的仓库 url 地址和 github 使用的链接地址是否一致。`git push` 的数据协议有两种方式：`ssh` 和 `https`。如果不一致就需要切换 `url` 地址。
<a name="Ny7Gy"></a>
## 24、git 输错用户名和密码，后续的 git 操作一直报错
```bash
remote: Coding 提示: Authentication failed.
remote: 认证失败，请确认您输入了正确的账号密码。
fatal: Authentication failed for 'https://e.coding.net/xxx.git/'
```
在控制面板里找到凭据管理器，选中 Windows 凭据，找到 git 的凭据，点击编辑，输入所用 github 的正确用户名和密码。
<a name="X6HoB"></a>
## 25、lint-staged 失败
可能项目名路径中包含了中文名，需要替换成英文名
<a name="KZnAf"></a>
## 26、查看 git 安装目录

- **「Mac：」** 在命令行中输入 `which git`，就会显示 git 的安装位置了
- **「Windows：」** 打开cmd，输入 `where git`，就会显示 git 的安装路径了
<a name="aUExU"></a>
## 27、Git中用vim打开、修改、保存文件
<a name="I6QoI"></a>
## 28、windows10 无法编辑 vimrc
<a name="k5MGx"></a>
## 29、Windows下Git Bash中VIM打开文件中文乱码
<a name="VirkP"></a>
## 30、如何修改旧的 commit 的 message/如何将多个 commit 合成一个 commit/如何将多个间隔的 commit 合成一个 commit/
```bash
git rebase -i
```
<a name="InEtx"></a>
## 31、如果两个人都对某个文件进行了修改，一个是重命名文件，一个是修改文件内容，那么会起冲突吗？git 很智能，会自动合并这些修改
如果两个人都对同一个文件重命名，此时会起冲突，git 不会自动处理，需要开发者自身去解决冲突
<a name="BsShy"></a>
## 32、`git revert` 失败：error: Commit faulty merge is a merge but no -m option was given、error: option `mainline' expects a number greater than zero
```bash
git revert -m 1
```
<a name="za3Kv"></a>
## 33、git 创建一个空的分支
在 Git 中创建分支，是必须有一个父节点的，也就是说必须在已有的分支上来创建新的分支，如果工程已经进行了一段时间，这个时候是无法创建空分支的。但是有时候就是需要创建一个空白的分支。
```bash
$ git checkout --orphan emptyBranchName
```
该命令会生成一个叫 `emptybranch` 的分支，该分支会包含父分支的所有文件。但新的分支不会指向任何以前的提交，就是它没有历史，如果提交当前内容，那么这次提交就是这个分支的首次提交。<br />想要空分支，所以需要把当前内容全部删除，用 git 命令
```bash
$ git rm -rf . // 注意：最后的‘.’不能少。
```
<a name="UdkMA"></a>
## 34、如何清空一个分支的所有提交
先删除该分支，然后再新建一个空的分支（分支名就是删除的分支名）~~
