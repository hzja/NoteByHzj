Kubernetes
<a name="qRVJk"></a>
## 一、概述
Helm 针对 Kubernetes 的 Helm 包管理器。Helm 的一般操作：

- `helm search`：搜索 chart
- `helm pull`：下载 chart 到本地目录查看
- `helm install`：上传 chart 到 Kubernetes
- `helm list`：列出已发布的 chart
```bash
# 查看帮助
helm --help
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714090-9eefbb3b-a69d-4f79-a356-aec16b65bbf5.png#clientId=u5ceff77e-7d99-4&from=paste&id=u41eadbde&originHeight=1452&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabd26cad-fc3f-42e0-a40d-56ba579743d&title=)<br />官方文档：[https://helm.sh/zh/docs/helm/helm/](https://helm.sh/zh/docs/helm/helm/)
<a name="WHw8D"></a>
## 二、Helm 仓库（`helm repo`）
添加、列出、删除、更新和索引 chart 仓库。
<a name="xE501"></a>
### 1）添加 chart 仓库
```bash
helm repo add bitnami https://charts.bitnami.com/bitnami
```
<a name="EbGBv"></a>
### 2）列出已添加的仓库
```bash
helm repo list
```
<a name="Y8ke5"></a>
### 3）从 chart 仓库中更新本地可用 chart 的信息
```bash
helm repo update bitnami
```
<a name="KCJhT"></a>
### 4）删除一个或多个仓库
```bash
helm repo remove bitnami
```
<a name="re0RP"></a>
## 三、创建 chart（`helm create`）
使用给定名称创建新的 chart，该命令创建 chart 目录和 chart 用到的公共文件目录。<br />比如'`helm create foo`'会创建一个目录结构看起来像这样：
```bash
$ helm create foo
foo/
├── .helmignore   # Contains patterns to ignore when packaging Helm charts.
├── Chart.yaml    # Information about your chart
├── values.yaml   # The default values for your templates
├── charts/       # Charts that this chart depends on
└── templates/    # The template files
    └── tests/    # The test files
```
<a name="Zc6ja"></a>
## 四、chart 包安装（`helm install`）
该命令用于**安装 chart 包**。安装参数必须是 chart 的引用，一个**打包后的 chart 路径**，**未打包的 chart 目录或者是一个 URL**。<br />要重写 chart 中的值，使用`--values`参数传递一个文件或者使用`--set`参数在命令行传递配置，强制使用字符串要用`--set-string`。当值本身对于命令行太长或者是动态生成的时候，可以使用`--set-file`设置独立的值。
```bash
helm install -f myvalues.yaml myredis ./redis

helm install --set name=prod myredis ./redis

helm install --set-string long_int=1234567890 myredis ./redis

helm install --set-file my_script=dothings.sh myredis ./redis
```
<a name="XReGB"></a>
## 五、管理 chart 依赖（`helm dependency`）

- Helm chart 将依赖存储在'charts/'。对于 chart 开发者，管理依赖比声明了所有依赖的'Chart.yaml'文件更容易。
- 依赖命令对该文件进行操作，使得存储在'charts/'目录的需要的依赖和实际依赖之间同步变得很容易。

比如 Chart.yaml 声明了两个依赖：
```yaml
# Chart.yaml
dependencies:
- name: nginx
    version: "1.2.3"
    repository: "https://example.com/charts"
- name: memcached
    version: "3.2.1"
    repository: "https://another.example.com/charts"
```

- name 是 chart 名称，**必须匹配Chart.yaml文件中名称**。
- version 字段应该包含一个语义化的版本或版本范围。

从 2.2.0 开始，仓库可以被定义为本地存储的依赖 chart 的目录路径。路径应该以"file://"前缀开头，比如：
```yaml
# Chart.yaml
dependencies:
- name: nginx
    version: "1.2.3"
    repository: "file://../dependency_chart/nginx"
```
<a name="KoLsP"></a>
### 1）列举指定 chart 的依赖
```bash
# helm dependency list CHART
helm dependency list wordpress
```
<a name="T8XPF"></a>
### 2）依赖升级
基于 Chart.yaml 内容升级 charts/
```bash
# helm dependency update CHART [flags]
helm dependency update wordpress
```
<a name="Bld7h"></a>
## 六、Helm 列表（`helm list`）

- **列举发布版本**，该命令会列举出指定命名空间的所有发布版本，(如果没有指定命名空间，会使用当前命名空间)。
- **默认**情况下，只会**列举出部署的或者失败的发布**，像`--uninstalled`或者`--all`会修改默认行为。这些参数可以组合使用：`--uninstalled --failed`。
- 默认情况下，最多返回256项，使用`--max`限制数量，`--max`设置为**0 不会返回所有结果**，而是返回服务器默认值，可能要比 256 更多。同时使用`--max`和`--offset`参数可以**翻页**显示。
```bash
# -A显示所有
helm list --max=10 --offset=2 -A
```
<a name="GkKoy"></a>
## 七、升级版本（`helm upgrade`）
该命令将发布升级到新版的 chart。升级参数必须是发布和 chart。chart 参数可以是：chart引用('example/mariadb')，chart目录路径，打包的 chart 或者完整 URL。对于 chart 引用，除非使用'`--version`'参数指定，否则会使用最新版本。
```bash
helm upgrade --set foo=bar --set foo=newbar redis ./redis
```
<a name="wlrW6"></a>
## 八、发布历史（`helm history`）
检索发布历史，打印给定版本的历史修订。**默认**会返回最大的256个历史版本。设置`--max`配置返回历史列表的最大长度。<br />历史发布集合会被打印成格式化的表格，例如：
```bash
$ helm history angry-bird
REVISION    UPDATED                     STATUS          CHART             APP VERSION     DESCRIPTION
1           Mon Oct 3 10:15:13 2016     superseded      alpine-0.1.0      1.0             Initial install
2           Mon Oct 3 10:15:13 2016     superseded      alpine-0.1.0      1.0             Upgraded successfully
3           Mon Oct 3 10:15:13 2016     superseded      alpine-0.1.0      1.0             Rolled back to 2
4           Mon Oct 3 10:15:13 2016     deployed        alpine-0.1.0      1.0             Upgraded successfully
```
<a name="fsSWD"></a>
## 九、回滚版本（`helm rollback`）
回滚发布到上一个版本，回滚命令的第一个参数是发布的名称，第二是修订（版本）号，如果省略此参数，会回滚到上一个版本。
```bash
# helm rollback <RELEASE> [REVISION] [flags]
# 先查看历史版本
helm history myharbor -n harbor
# 不指定版本就回退上个版本
helm rollback myharbor 1 -n harbor
helm rollback myharbor -n harbor
```
<a name="VEvi5"></a>
## 十、展示 chart（`helm show`）
```bash
# helm show all [CHART] [flags]
# 该命令检查chart(目录、文件或URL)并显示所有的内容（values.yaml, Chart.yaml, README）
helm show all mysql

# helm show values [CHART] [flags]
# 该命令检查chart(目录、文件或URL)并显示values.yaml文件的内容
helm show values mysql
```
<a name="TR4lB"></a>
## 十一、拉取 chart（`helm pull`）
从仓库下载并（可选）在本地目录解压。
```bash
# helm pull [chart URL | repo/chartname] [...] [flags]
# 仅下载
helm pull bitnami/redis
# 下载并解压到当前目录
helm pull bitnami/redis --untar
```
<a name="nTh8b"></a>
## 十二、Helm 打包（`helm package`）

- **将 chart 目录打包到 chart 归档中**，该命令将 chart 打包成一个 chart 版本包文件。如果给定路径，就会在该路径中查找 chart（必须包含 Chart.yaml 文件）然后将目录打包。
- 要签名一个 chart，使用`--sign`参数，在大多数场景中，也要提供`--keyring path/to/secret/keys`和`--key keyname`。
```bash
helm package mysql/
# Successfully packaged chart and saved it to: /opt/k8s/helm/mysql-9.3.1.tgz
```
如果**想忽略 helm 中的文件**，可以在.helmignore进行匹配，该.helmignore 文件支持 Unix shell 全局匹配，相对路径匹配和否定（以！前缀反向匹配）。每行仅考虑一种模式。示例如下：
```bash
# .helmignore
.git
*/temp*
*/*/temp*
temp?
```
<a name="Aqtk2"></a>
## 十三、推送 chart 到 harbor（helm cm-push）
将 chart 推送到远程。
<a name="IqPil"></a>
### 1）在线安装
```bash
helm plugin install https://github.com/chartmuseum/helm-push
```
<a name="mDOyy"></a>
### 2）离线安装
下载地址：[https://github.com/chartmuseum/helm-push/tags](https://github.com/chartmuseum/helm-push/tags)
```bash
# 1、下载安装包
wget https://github.com/chartmuseum/helm-push/releases/tag/v0.10.3

# 2、查看helm的plugin路径：helm env
helm env

# 3、在该路径下创建helm-push文件夹，并将安装包拷贝到该文件夹下解压即可
mkdir /root/.local/share/helm/plugins/helm-push

wget https://github.com/chartmuseum/helm-push/releases/download/v0.10.3/helm-push_0.10.3_linux_amd64.tar.gz

tar zxvf /root/.local/share/helm/plugins/helm-push/helm-push_0.10.3_linux_amd64.tar.gz -C /root/.local/share/helm/plugins/helm-push
```
查看插件
```bash
helm plugin list
```
<a name="POlIp"></a>
### 3）helm 增加 harbor repo
```bash
# chartrepo，固定参数，bigdata自定义项目
helm repo add local-harbor --username=admin --password=Harbor12345 https://myharbor.com/chartrepo/bigdata/ --ca-file /opt/k8s/helm/ca.crt
```
证书直接在 harbor 上下载<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714169-13d7a1e5-29f6-44ad-8845-d83f49e2bc7e.png#clientId=u5ceff77e-7d99-4&from=paste&id=ubb0c5841&originHeight=441&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u53113d30-4066-4484-8b58-4060c9b01b5&title=)
<a name="WTcyA"></a>
### 4）示例演示
```bash
# 查看帮助
helm cm-push --help

# 推送，接【目录】
helm cm-push mysql/ local-harbor --ca-file /opt/k8s/helm/ca.crt

# 推送，接【压缩包】
helm cm-push wordpress-15.1.5.tgz local-harbor --ca-file /opt/k8s/helm/ca.crt

# 推送，指定版本，--version
helm cm-push mychart/ --version="1.2.3" local-harbor --ca-file /opt/k8s/helm/ca.crt

# 强制推送，--force
helm cm-push --force mychart-0.3.2.tgz local-harbor
```
查看<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714158-7330b25b-111a-47b0-b4c6-0f6bad0c8005.png#clientId=u5ceff77e-7d99-4&from=paste&id=ubc387d1d&originHeight=406&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubbfa95a7-454b-4a6a-8b65-855c16f4e17&title=)
<a name="bu11m"></a>
## 十四、搜索 hub（helm search hub）
在Artifact Hub或自己的 hub 实例中搜索 chart。<br />Artifact Hub 是**基于 web 页面的应用**，支持 CNCF 项目的查找、安装和发布包及配置项，包括了公开发布的 Helm chart。它是 CNCF 的沙盒项目。可以访问[https://artifacthub.io/](https://artifacthub.io/)
```bash
# 不带参数，列出所有
helm search hub

# 指定chart
helm search hub mysql
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714247-5a45452d-3476-4452-91d8-11a873e7b44f.png#clientId=u5ceff77e-7d99-4&from=paste&id=ueb2d3b95&originHeight=409&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6b825a9c-f717-4839-b474-6d202dcc87a&title=)
<a name="FHsov"></a>
## 十五、搜索仓库（`helm search repo`）
用 chart 中关键字搜索仓库，搜索会读取系统上配置的所有仓库，并查找匹配。搜索这些仓库会使用存储在系统中的元数据。它会展示找到**最新稳定版本的 chart**。如果指定了--devel参数，输出会包括预发布版本。
```bash
# Search for stable release versions matching the keyword "nginx"
$ helm search repo nginx

# Search for release versions matching the keyword "nginx", including pre-release versions
$ helm search repo nginx --devel

# Search for the latest stable release for nginx-ingress with a major version of 1
$ helm search repo nginx-ingress --version ^1.0.0
```
<a name="aW6cz"></a>
## 十六、验证 chart（`helm lint`）
该命令使用一个 chart 路径并运行一系列的**测试来验证 chart 的格式是否正确**。如果遇到引起 chart 安装失败的情况，会触发[ERROR]信息，如果遇到违反惯例或建议的问题，会触发[WARNING]。
```bash
# helm lint PATH [flags]
helm lint ./mysql
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714189-d54ae9d5-67f3-493d-9483-432584b0e0ff.png#clientId=u5ceff77e-7d99-4&from=paste&id=u20963584&originHeight=231&originWidth=831&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u45c36987-79a2-41e4-9dfa-ab0bc5810c3&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662912714417-ce5e42cc-d14b-4f4c-86c1-d1ef760240db.png#clientId=u5ceff77e-7d99-4&from=paste&id=u6a723efc&originHeight=231&originWidth=831&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uebe03ea9-5455-4ee3-8a1b-1469955386f&title=)
<a name="QyA7o"></a>
## 十七、常用命令总结
```bash
helm version                            // 查看helm版本
helm create xxx                         // 创建一个xxx charts
helm lint ./xxx                         // 检查包的格式或信息是否有问题
helm install xxx1 ./xxx                 // 部署安装xxx，设置名称为xxx1
helm list                               // 列出已经部署的charts
helm history                            // 发布历史
helm upgrade                            // 更新版本
helm rollback                           // 回滚版本
helm package ./xxx                      // 打包charts
helm repo add --username admin --password password myharbor xxx  // 增加repo
helm uninstall xxx1                     // 卸载删除xxx1
helm pull                                // 拉取chart包
helm cm-push                            // 推送chart包
helm repo update                        // 更新仓库资源
helm search hub                         // 从 Artifact Hub 中查找并列出 helm charts。Artifact Hub中存放了大量不同的仓库
helm search repo                        // 从你添加（使用 helm repo add）到本地 helm 客户端中的仓库中进行查找。该命令基于本地数据进行搜索，无需连接互联网
```
Helm 常用命令（chart 安装、升级、回滚、卸载等操作）就先到这里。
