CodeFever Community 由 蒲公英开发者服务平台 团队完整自主研发并基于 MIT 协议进行完整开源。CodeFever Community 保留了 CodeFever 的大部分功能, 并且进行了更适合单机部署的优化，支持一行命令安装到自己服务器！
<a name="fg72F"></a>
## 功能特性

- **「永久免费：」**永久免费提供给大家使用，您无需担心付费
- **「完整开源：」**毫无保留地完整开源，无任何编译或加密代码
- **「轻量安装：」**极简设计风格，排除重度冗余的多余功能
- **「性能高效：」**速度极快，对服务器资源要求极低，1核CPU/1G内存即可运行
- **「无限仓库：」**没有任何仓库数量、使用数量的限制
- **「代码对比：」**支持提交代码的不同版本支持高亮显示对比
- **「分支管理：」**完整支持 Git 原生分支和 Tag，帮助更清晰的管理代码
- **「合并请求（Merge Request）：」**支持多人协作，让代码版本控制更顺畅
- **「分支保护：」**分支保护功能让代码提交安全可控，代码 Review 更容易清晰
- **「多人协作：」**支持多人团队协作，并可以设置每个成员的角色和权限
- **「Webhook：」**支持Webhook功能，可轻松和其他系统进行集成
- **「管理后台：」**支持超级管理员后台，可以让团队leader管理所有项目和用户信息
- **「中英双语：」**原生支持中文和英文两种语言
<a name="PpJtW"></a>
## 如何安装
CodeFever 提供 从零开始安装 和 Docker 镜像安装 两种安装方式，可以根据自己的实际需要选择安装方式。
<a name="s57He"></a>
### Docker 镜像安装
最简单的安装方式是使用 Docker, 只需要一行命令即可完成：
```bash
docker run -d --privileged=true --name codefever -p 80:80 -p 22:22 -it pgyer/codefever-community:latest /usr/sbin/init
```

1. 服务启动后尝试访问 http://127.0.0.1 或 ` 登录
2. 如果希望使用 22 端口作为 Git 的 SSH 协议端口，需要在启动镜像前将宿主系统的 SSH 服务 端口 先修改成其他端口
3. 如果服务异常你可以登录 Shell 去人工维护, 也可以直接重启容器重启服务。
4. 默认管理员用户：root@codefever.cn，密码：123456。登录后请修改密码并绑定 MFA 设备。
<a name="gWV9X"></a>
### 从零开始安装
如果出于学习、研究或定制化需求, 也可以手动方式安装, 具体可见：[https://github.com/PGYER/codefever/blob/master/doc/zh-cn/installation/install_from_scratch.md](https://github.com/PGYER/codefever/blob/master/doc/zh-cn/installation/install_from_scratch.md)
<a name="Z8mxQ"></a>
## 运行截图
<a name="rugmr"></a>
### 完善的代码管理
内建完善的代码管理功能，帮助您的团队高效管理代码版本，让每次 Git 代码提交、推送、拉取、合并、查阅都变的更加轻松。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673673701539-345841ce-d219-4194-8813-f1b74e9de6bb.png#averageHue=%23b9996d&clientId=ue099d41e-4f53-4&from=paste&id=uf5ff0f97&originHeight=704&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub34755be-fc44-487d-8d60-8d55e054ff0&title=)
<a name="il11n"></a>
### 可靠的权限管理
清晰的分支权限管理，设有保护分支，提高整体安全性，团队协作规范有序且流畅。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673673701546-5f6f03d1-ac6e-41d9-afe4-215e428b8fa5.png#averageHue=%23a08e72&clientId=ue099d41e-4f53-4&from=paste&id=ue14c5daf&originHeight=704&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u779a5715-0398-42ba-baf6-9925451de41&title=)
<a name="J8lvV"></a>
### 完备的安全保障
增量代码自动扫描，提高 codereview 效率，合并前进行组员评审、审批等流程，提高代码质量，使用更安全。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673673701556-fa50f361-6e25-48da-84a1-95295f6ab2f4.png#averageHue=%23ddf9e1&clientId=ue099d41e-4f53-4&from=paste&id=u19aaf381&originHeight=721&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufa93c8af-c43b-4a50-89b5-53d2fe9bf3a&title=)
<a name="n277a"></a>
### 轻松的运维环境
系统支持采用多副本高可用架构，支持自动备份功能，保障代码安全，并具备完善的日志审计、通知机制，可有效追溯。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673673701571-eac1d2c7-aa3a-42b5-8bbe-1b7fa9ad3b45.png#averageHue=%23726350&clientId=ue099d41e-4f53-4&from=paste&id=uf28ec540&originHeight=704&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u31b48543-e04b-47dc-8176-639bb306a04&title=)
<a name="hrTb7"></a>
## 传送门

- 开源地址：[https://github.com/PGYER/codefever](https://github.com/PGYER/codefever)
