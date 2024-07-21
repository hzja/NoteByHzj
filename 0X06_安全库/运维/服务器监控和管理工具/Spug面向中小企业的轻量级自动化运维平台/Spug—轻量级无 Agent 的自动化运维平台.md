运维
<a name="fTtpt"></a>
## Spug简介
Spug面向中小型企业设计的轻量级无 Agent 的自动化运维平台，整合了主机管理、主机批量执行、主机在线终端、文件在线上传下载、应用发布部署、在线任务计划、配置中心、监控、报警等一系列功能。

- 代码仓库地址：[https://github.com/openspug/spug.dev](https://github.com/openspug/spug.dev)
- 官网地址：[https://www.spug.dev](https://www.spug.dev)
- 使用文档：[https://www.spug.dev/docs/about-spug/](https://www.spug.dev/docs/about-spug/)
- 更新日志：[https://www.spug.dev/docs/change-log/](https://www.spug.dev/docs/change-log/)
- 常见问题：[https://www.spug.dev/docs/faq/](https://www.spug.dev/docs/faq/)
<a name="cCfSz"></a>
## Spug的功能

- 批量执行: 主机命令在线批量执行
- 在线终端: 主机支持浏览器在线终端登录
- 文件管理: 主机文件在线上传下载
- 任务计划: 灵活的在线任务计划
- 发布部署: 支持自定义发布部署流程
- 配置中心: 支持 KV、文本、json 等格式的配置
- 监控中心: 支持站点、端口、进程、自定义等监控
- 报警中心: 支持短信、邮件、钉钉、微信等报警方式
- 优雅美观: 基于 Ant Design 的 UI 界面
- 开源免费: 前后端代码完全开源
<a name="z67UD"></a>
## 安装环境要求

- Python 3.6+
- Django 2.2
- Node 12.14
- React 16.11
<a name="r2V8b"></a>
## 安装Spug
简化一切安装操作步骤，官方也建议使用docker进行安装，那么，接下来就使用docker来安装这款工具平台。操作环境基于Centos7.x操作系统。
<a name="tkJBX"></a>
### 1、安装docker并启动
```bash
yum install docker -y
systemctl start docker
```
<a name="FeIiL"></a>
### 2、拉取镜像
阿里云的镜像与 Docker hub 同步更新，国内用户建议使用阿里云的镜像。
```bash
$ docker pull registry.aliyuncs.com/openspug/spug
```
<a name="jOgbW"></a>
### 3、启动容器
Docker镜像内部使用的 Mysql 数据库。如果需要持久化存储代码和数据，可以添加：-v 映射容器内/data路径
```bash
$ docker run -d --name=spug -p 80:80 registry.aliyuncs.com/openspug/spug
# 持久化存储启动命令：
# mydata是本地磁盘路径，/data是容器内代码和数据初始化存储的路径
$ docker run -d --name=spug -p 80:80 -v /mydata/:/data registry.aliyuncs.com/openspug/spug
```
<a name="zzpVo"></a>
### 4、初始化
以下操作会创建一个用户名为 admin 密码为 spug.dev 的管理员账户，可自行替换管理员账户。
```bash
$ docker exec spug init_spug admin spug.dev
# 执行完毕后需要重启容器
$ docker restart spug
```
<a name="itdtF"></a>
### 5、访问测试
在浏览器中输入 [http://localhost:80 访问,用户名：admin  密码：spug.dev](http://localhost:80%20%E8%AE%BF%E9%97%AE,%E7%94%A8%E6%88%B7%E5%90%8D%EF%BC%9Aadmin%20%C2%A0%E5%AF%86%E7%A0%81%EF%BC%9Aspug.dev)<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571666-d64051be-e567-43a3-81a3-5710f53a45d8.webp#averageHue=%23e9edf7&height=800&id=GMSCp&originHeight=800&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="OEzQN"></a>
## 使用介绍
登录完成后，就可以看到主界面，如下<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571695-f17eb049-de6f-40c7-ac2d-d3a35bf0a82b.webp#averageHue=%23fdfefc&height=490&id=kvVvW&originHeight=490&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="27jeQ"></a>
### 主机管理
管理维护平台可操作的主机，首次添加主机时需要输入 ssh 指定用户的密码。![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571682-0c9c280c-2ec7-49ac-82a8-efe5548051bd.webp#averageHue=%23ecf5f4&height=271&id=hiU2l&originHeight=271&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="V5lrr"></a>
### 批量执行
包含维护命令模版和批量远程执行命令两部分功能，常用来执行一些临时的任务例如，批量安装/卸载某个依赖包等。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571753-8ecc3d9f-81db-467a-b015-6b39b012140a.webp#averageHue=%23767a80&height=612&id=AQHe8&originHeight=612&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)

- 执行任务

可以选择一到多个在主机管理中添加的主机作为执行的目标主机，命令内容可以直接写也支持从模板中读取已保存的命令。

- 模板管理

用于存储复杂、常用的命令集合，以便后期可随时使用。
<a name="JLR1P"></a>
### 应用发布

- 应用管理

管理和维护可发布的应用。每个应用又可以创建对应环境的发布配置，发布配置请查看发布配置文档。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571688-9c4c2741-0bb2-4c18-83a0-3bfc2e720537.webp#averageHue=%2396a1ad&height=613&id=oIMQo&originHeight=613&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)

- 发布配置

配置指定应用在某环境下如何执行发布，发布支持两种方式 常规发布 和 自定义发布。

- 发布申请

创建和执行发布。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571770-501d6178-46ed-4296-87ec-0d71217d1ac9.webp#averageHue=%239ea7b1&height=614&id=FjnpS&originHeight=614&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="MD7hU"></a>
### 配置中心

- 环境管理

管理应用的运行环境，一般包含开发环境、测试环境和生产环境，应用发布和配置管理需要用它来区分不同的环境。<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604381571810-04f743fa-4c79-459f-8b91-9445c4453380.png#averageHue=%23dcddd5&height=488&id=mPlK8&originHeight=488&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)

- 服务管理

管理和维护应用依赖的服务配置。例如有两个应用 A 和应用 B，它们共同使用一个数据库，那么就可以把这个数据库提取出来作为单独的服务来管理。这样带来的好处是如果这个数据库配置变更了，那么只需要在服务管理里把这个数据库的配置更新即可，不必在多个应用之间切换查找更新。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571806-979f1f36-de14-4992-a222-0dd140aa6222.webp#averageHue=%23f9fbf8&height=612&id=IH6FM&originHeight=612&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)

- 应用管理

用于维护应用的配置，应用配置包含 公共 和 私有 两种类型的配置。

- 配置管理

用户维护服务和应用在不同环境下的具体配置。<br />**任务调度**<br />维护一些周期性的任务<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571806-5a8e13f7-cc49-4c32-ada6-5f8c43675cc0.webp#averageHue=%238895a2&height=616&id=YgX1Y&originHeight=616&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=1080)
<a name="qCWmf"></a>
### 监控中心
该模块提供了以下几种常用的监控模式

- 站点检测

通过 GET 请求指定的 url 匹配返回的状态码来确定站点是否异常

- 端口检测

检测指定目标主机的端口是否可以正常建立接连

- 进程检测

检测指定目标主机的某个进程是否存活

- 自定义脚本检测

在指定主机上运行自定义的脚本，通过判断返回的退出状态码来确定是否有异常<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1604381571827-d82942a1-6ab3-41d4-bf48-93faa9db3c75.png#averageHue=%23e7f7d7&height=603&id=w0FcE&originHeight=603&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="C2JAW"></a>
### 报警中心
配置与维护日常报警相关，如:报警记录、报警联系人与组<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1604381571832-58f13ba8-2a42-473b-b346-c6f5db4a1ba9.webp#averageHue=%23fafcfa&height=499&id=fWcwV&originHeight=499&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="0b1o8"></a>
### 系统管理
除了页面上对普通用的管理，Spug 还提供了 manage.py user 命令可用于管理员账户的管理操作。

- 创建账户

创建账户使用 manage.py user add 命令，用法示例如下
```bash
$ cd spug/spug_api
$ source venv/bin/activate
$ python manage.py user add -u admin -p 123 -n Fcant -s
```
Docker 安装的可以执行如下命令
```bash
$ docker exec spug python3 /data/spug/spug_api/manage.py user add -u admin -p 123 -n Fcant -s
#上面的命令会创建个登录名为 admin 密码为 123 昵称为 Fcant 的管理员账户，注意最后的 -s 参数，如果携带了这个参数意味着该账户为管理员账户， 管理员账户可以不受任何限制的访问所有功能模块。
```

- 重置密码

使用 manage.py user reset 命令来重置账户密码，用法示例如下
```bash
$ cd spug/spug_api
$ source venv/bin/activate
$ python manage.py user reset -u admin -p abc
```
Docker 安装的可以执行如下命令
```bash
$ docker exec spug python3 /data/spug/spug_api/manage.py user reset -u admin -p abc
#上述操作会重置登录名为 admin 的账户的密码为 abc。
```

- 启用账户

当页面上登录连续错误数次超过3次后账户自动转为禁用状态，普通用户可以通过 系统管理 / 账户管理 在页面是启用账户即可，但管理员账户需要使用如下命令来启用
```bash
$ cd spug/spug_api
$ source venv/bin/activate
$ python manage.py user enable -u admin
```
Docker 安装的可以执行如下命令
```
$ docker exec spug python3 /data/spug/spug_api/manage.py user enable -u admin
```
