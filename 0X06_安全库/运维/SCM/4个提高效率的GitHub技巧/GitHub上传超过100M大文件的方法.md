Github<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1646893162265-64905cb1-84db-4842-bd1b-0faab110a3a0.png#clientId=u21156753-23f7-4&from=paste&height=713&id=udf11121d&originHeight=1783&originWidth=3802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=392677&status=done&style=none&taskId=ubb056238-f692-4fc8-8272-25f89da445f&title=&width=1520.8)<br />[Git Large File Storage (LFS)](https://git-lfs.github.com/?utm_source=github_site&utm_medium=billing_upgrade_link&utm_campaign=gitlfs) 在 Git 中用文本指针替换音频、视频、数据集和图形等大文件，同时将文件内容存储在像 GitHub. com 或 GitHub Enterprise 这样的远程服务器上。
<a name="c5bvE"></a>
## 1、初始化操作

1. 创建Github账号
2. 下载本地Git
3. 初始化本地Git仓库
4. 将本地仓库推送至远程仓库进行关联
<a name="EpGnz"></a>
## 2、大文件上传操作
这里就要用到Git Large File Storage了，初始化操作成功之后，输入
```bash
git lfs install
```
然后输入 `git lfs track "file.zip"` (这里的文件是要上传的文件)<br />然后输入 `git add .gitattributes`<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1646275551047-863694d8-a4e9-46f6-b000-02ce961f361f.png#clientId=u1a4f645e-02a7-4&from=paste&height=302&id=u7acea738&originHeight=755&originWidth=3135&originalType=binary&ratio=1&rotation=0&showTitle=false&size=239995&status=done&style=shadow&taskId=u90c8bed5-f5ed-4a8d-a458-382cf4807e1&title=&width=1254)
<a name="VwmPE"></a>
## 3、上传文件至Github仓库

1. 上传文件到GitHub的仓库：`git add file.zip`
2. 提交项目并备注信息：`git commit -m "提交信息"`。
3. 推送文件至远程仓库：`git push -u origin master`
