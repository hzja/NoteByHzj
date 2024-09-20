Linux rpm <br />rpm命令是Red-Hat Package Manager（RPM软件包管理器）的缩写， 该命令用于管理Linux 下软件包的软件。在 Linux 操作系统下，几乎所有的软件均可以通过RPM 进行安装、卸载及管理等操作。<br />概括的说，rpm命令包含了五种基本功能：安装、卸载、升级、查询和验证。<br />语法格式：rpm [参数] [软件包]<br />常用参数：

| -a | 查询所有的软件包 |
| --- | --- |
| -b或-t | 设置包装套件的完成阶段，并指定套件档的文件名称； |
| -c | 只列出组态配置文件，本参数需配合”-l”参数使用 |
| -d | 只列出文本文件，本参数需配合”-l”参数使用 |

<a name="cQjlt"></a>
## rpm的作用

- 查询已安装的软件
- 安装软件
- 卸载软件
<a name="0dfbe902"></a>
## 常用命令
| **rpm [args] [包名]** | **类似软件管家{添加,卸载,查询}** |
| --- | --- |
| -v | 显示安装的详细过程 |
| -q | 查询某个软件是否安装 |
| -a | 查询所有软件 |
| -h | 列出软件安装时的标记 |
| -i | install 安装指定的软件 |
| --nodeps | 不验证软件相关关联性,常于-e一起使用,表示强制卸载 |

```bash
# 查询所有已安装软件
$ rpm -qa
# 查询某个包的安装信息
$ rpm -ql <包名>
# 安装指定的软件包
$ rpm -ivh <包名>
# 强制卸载指定软件包
$ rpm -e --nodeps <包名>
```
<a name="9b93f6c0"></a>
## 更换YUM源
```bash
# 安装wget
$ yum install -y wget
# 下载CentOS 7的repo文件
$ wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
# 清除缓存
$ yum clean all
# 生成缓存
$ yum makecache
# 备份CentOS 7系统自带yum源配置文件
$ mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
# 打开CentOS-Base.repo文件,将文件中的所有http开头的地址更改为https
$ vim /etc/yum.repos.d/CentOS-Base.repo
# 更新YUM
$ yum update
```
