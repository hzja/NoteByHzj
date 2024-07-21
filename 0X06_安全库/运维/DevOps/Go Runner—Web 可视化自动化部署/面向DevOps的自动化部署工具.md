DevOps<br />推荐一款好用的CI/CD工具。<br />建木是一个面向DevOps领域的极易扩展的图形化工具，帮助用户轻松编排各种DevOps流程并分发到不同平台执行。
<a name="ijfLy"></a>
## 什么是建木
“建木”是上古先民崇拜的一种圣树，传说建木是沟通天地人神的桥梁。伏羲、黄帝等众帝都是通过这一神圣的梯子上下往来于人间天庭。《淮南子·墬形训》亦曰：“建木在都广，众帝所自上下。日中无景，呼而无响，盖天地之中也。”<br />为此项目命名为“建木”，希望项目也可以成为不同业务场景下系统间相互沟通的桥梁。<br />建木是一个面向DevOps领域的极易扩展的开源无代码(图形化)/低代码(GitOps)工具。可以帮助用户轻松编排各种DevOps流程并分发到不同平台执行。
<a name="HynwB"></a>
## 在线体验
**地址：** [https://www.gitlink.org.cn/jianmu/demo/devops](https://www.gitlink.org.cn/jianmu/demo/devops)<br />**账号：** jianmu<br />**密码：** jianmu.dev<br />点击登录，输入账号密码即可在线体验<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671067876316-d7d8e96e-b895-42d7-868c-b50704ec3203.png#averageHue=%23dadfe0&clientId=ub697fe52-ba10-4&from=paste&id=u11146c4b&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc4529cd9-5f0b-43ab-a8f4-9261490dd68&title=)<br />**体验说明**<br />建木作为第三方工具与代码库GitLink集成，为GitLink提供DevOps引擎。提前为大家准备了GitLink体验账号和一些流程示例，无须将建木安装部署在本地，登录账号即可快速体验流程编排。
<a name="IkDUm"></a>
## 项目特点
<a name="krkx6"></a>
### 图形化编排
自由拖拽节点，快速配置参数，高效构建项目。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670651346584-e4881d33-8328-4bf2-889b-b1aa7df26c6d.png#averageHue=%239bc69c&clientId=u1e84c407-1d28-4&from=paste&id=u98865968&originHeight=584&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa721187-4860-450c-9e78-609143db90e&title=)
<a name="ZFYmU"></a>
### 环境隔离
每个环境都在各自的容器内运行，运行后自动销毁回收。
<a name="zXMGH"></a>
### 流程可视化
可视化配置流程，任务编排与执行状态一目了然。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670651346648-592dc5f8-6541-43eb-9892-5391fca25f6e.png#averageHue=%23fefefe&clientId=u1e84c407-1d28-4&from=paste&id=u3a682925&originHeight=396&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaa81633c-8010-462a-83d6-0f9a2145e5b&title=)
<a name="rfFb6"></a>
### 节点生态
官方和社区提供多种节点，各种节点可以自由组合，满足各种场景。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670651346670-eb4d21c4-7b2a-477e-8209-a1d146d722bf.png#averageHue=%2392d2d5&clientId=u1e84c407-1d28-4&from=paste&id=ucd8cb72a&originHeight=479&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u154bb569-4f4c-4e32-9646-aaed41ead61&title=)
<a name="CRrGz"></a>
### 平台多样
支持多种平台，用户自行选择。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670651346589-de723f51-55ea-44b1-a2d9-9bb1e32d6bde.png#averageHue=%23fbf9f8&clientId=u1e84c407-1d28-4&from=paste&id=u32868229&originHeight=183&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2f254047-fde6-4a45-8818-df01deb6e92&title=)
<a name="uCDvX"></a>
## 项目使用
目前支持两种方式安装部署建木，大家根据需要任选其一即可
<a name="QuSVa"></a>
### 资源和系统要求
<a name="Teipc"></a>
#### 操作系统
Ubuntu 21.04 (推荐) 或 macOS Monterey
<a name="EzEnb"></a>
#### 建议配置

- CPU：2C
- 内存：8GB
- 磁盘：100GB
<a name="jA94k"></a>
### 部署方式一：docker-compose部署
<a name="oiN5z"></a>
#### 软件版本要求

- 推荐使用Ubuntu 21.04系统安装
- Docker 19.30以上
- Docker-Compose 1.29.2以上
<a name="hrLPJ"></a>
#### 部署方式
**下载docker-compose.yml**
```bash
wget https://gitee.com/jianmu-dev/jianmu-deploy/raw/master/docker-compose.yml
```
启动
```bash
docker-compose up -d
```
访问http://127.0.0.1，默认**用户名密码**为admin/123456。
<a name="Kao1W"></a>
### 部署方式二：k8s部署
<a name="M7YMJ"></a>
#### 软件版本要求

- Kubernetes 1.18以上
<a name="m91b1"></a>
#### 部署方式

- 下载kubernetes.yaml
```bash
wget https://gitee.com/jianmu-dev/jianmu-deploy/raw/master/kubernetes.yaml
```

- 启动
```bash
kubectl apply -f kubernetes.yaml
```
访问node节点ip:30180，默认**用户名密码**为admin/123456，使用详见hello项目使用。
<a name="GHPjx"></a>
### 编辑流程
使用建木CI快速构建maven项目

- 使用git clone节点从git仓库中将项目clone下来
- 使用maven 构建节点将项目打成docker镜像，并且将其push到自己的docker hub仓库
- 使用`docker pull`，`docker run`命令把项目启动起来
- 使用企业微信通知节点通知项目部署完成

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1670651346658-ee3f75ba-57bf-42aa-995a-ed6ee01df797.png#averageHue=%23f0f0f0&clientId=u1e84c407-1d28-4&from=paste&id=u3544fac5&originHeight=496&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udf512bbc-56aa-454d-b9a3-ae8ce96b2e4&title=)
<a name="zot3R"></a>
## 第一个项目
<a name="zByxl"></a>
### 前提条件说明

- 有企业微信账号
- 新建一个测试群
- 测试群添加一个机器人，并获取机器人的webhook，形如：https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=06de918a-fd35-45b7-b2e6-cff28371fd5d

添加流程可参考：[https://gitee.com/help/articles/4296#article-header0](https://gitee.com/help/articles/4296#article-header0)
<a name="KAD2y"></a>
### 设置企业微信空间

1. 创建企业微信用户空间，点击密钥管理

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671068022114-1b94a2af-d7eb-4f13-8996-6c3c4404dc5f.png#averageHue=%23f0bda6&clientId=ub697fe52-ba10-4&from=paste&id=u4c43ffe6&originHeight=583&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua17032d5-10f5-46c4-bca9-794b69500f9&title=)

2. 点击新增命名空间

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671068022181-23e9a276-2e87-4b7f-aa34-b5bfbfbd8bc9.png#averageHue=%23e56953&clientId=ub697fe52-ba10-4&from=paste&id=ud7f117f6&originHeight=585&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9992c1ed-6fd0-42f7-9e52-92bd401b278&title=)

3. 填写命名空间名称

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671068022129-2f25da85-3ebb-4b32-8a62-4bc963bd2239.png#averageHue=%23716960&clientId=ub697fe52-ba10-4&from=paste&id=ue7ebbda1&originHeight=590&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u30f00c1d-5cd9-4dff-ada9-58f24c8b100&title=)

4. 进入命名空间，创建密钥，填写名称和值，参考如下

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671068022176-1b448f7e-01a3-4976-b3de-e5fc2c103447.png#averageHue=%23a7a7a7&clientId=ub697fe52-ba10-4&from=paste&id=u5bea24dd&originHeight=427&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2218f23b-938a-46c1-a59b-fe31e5a4f45&title=)
<a name="VpLPn"></a>
### 创建并执行第一个项目

1. 创建第一个hello项目
2. 点击导入项目，URL输入https://gitee.com/jianmu-dev/jianmu-docs.git

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671068022147-2d92eda6-24f6-413a-b0e3-293be04a39da.png#averageHue=%23f0bea8&clientId=ub697fe52-ba10-4&from=paste&id=ub997200e&originHeight=583&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1299afab-d9c1-497d-bbf9-f2459bba4ed&title=)

1. 填写分支为master
2. 关闭下方的认证开关，点击下一步
3. 选择hello-world.yml，点击保存
<a name="uY8QB"></a>
### 实现效果

1. 可以点击项目立即触发执行，届时可以看到测试企业群的Bot就会发一个问候。
2. 系统将会根据导入的yaml文件生成名为hello_world的流程并在每小时的0/30分时触发执行。
<a name="hWomR"></a>
## 项目地址
开源地址：[https://gitee.com/jianmu-dev/jianmu](https://gitee.com/jianmu-dev/jianmu)<br />官方文档：[https://docs.jianmu.dev/](https://docs.jianmu.dev/)
