<a name="m5yEm"></a>
## 1、软件管理工具
| **系统** | **管理机制** | **使用指令** | **管理命令** |
| --- | --- | --- | --- |
| Red Hat/Fedora/CentOS | RPM | rpm/rpmbuild | YUM (yum) |
| Debian/Ubuntu | DPKG | dpkg | APT (apt/apt-get) |

![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1699406622389-58f6a45c-2c21-4d32-ab49-453b086bfcc1.jpeg#averageHue=%2396c494&clientId=u803d7149-d6e3-4&from=paste&id=u6f5be249&originHeight=436&originWidth=1024&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6b431996-12f3-4419-8644-fb327be9c8f&title=)
<a name="n4lz1"></a>
## 2、yum和apt对比
(apt = apt-get、apt-cache 和 apt-config 中最常用命令选项的集合)

| **名称** | **CentOS为例** | **Ubuntu为例** | **备注** |
| --- | --- | --- | --- |
| 配置文件 | /etc/yum.repos.d/*.repo | /etc/apt/sources.list<br />/etc/apt/sources.list.d/*.list | 源配置文件 |
| 安装 | yum install software | apt install software | 安装 |
| 更新 | yum update | apt upgrade | 更新（ 更新已经安装的包） |
| 卸载 | yum remove software | apt remove software | 卸载，apt purge software 卸载软件并清除配置文件 |
| 查询 | yum search software | apt search software | 查询 |
| 下载 | yumdownloader software | apt download software | 只下载不安装 |
| 更新列表 | yum repolist | apt update | 获取软件包列表 |
| 列出包信息 | yum list | apt list | 列出包信息 |
| 清除缓存 | yum clean all | apt clean | 清除缓存 |
| 查看软件 | yum info software | apt show software | 查看软件信息 |

<a name="UWTu5"></a>
## 3、rpm和dpkg对比
rpm安装*.rpm的软件包，dpkg安装*.deb的软件包

| **名称** | **rpm** | **dpkg** | **备注** |
| --- | --- | --- | --- |
| 安装 | rpm -i software | dpkg -i software | 安装（i即install） |
| 查询 | rpm -q software | dpkg -l software | 查询软件是否安装(q即query，l即list) |
| 卸载 | rpm -e software | dpkg -r software | 删除软件（几乎不用e即erase,r即remove） |
| 配置文件 | rpm -qc software | dpkg -S software | 查看软件对应的配置文件 |
| 查找 | rpm -qa &#124; wc -l | dpkg -l &#124; wc -l | 查看安装软件数量 |
| 相关文件 | rpm -ql software | dpkg -L software | 查看软件所有相关文件信息（l即list，L即listfiles） |
| 文件属于包 | rpm -qf 文件 | <br /> | 查看文件是由那个软件生成的 |

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699406686486-7de02d71-9302-420e-b45d-84e87ee6daa5.png#averageHue=%23f4ece8&clientId=u803d7149-d6e3-4&from=paste&id=u6046e451&originHeight=540&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1d498f1-a053-4f06-9893-abc4a647f7b&title=)
