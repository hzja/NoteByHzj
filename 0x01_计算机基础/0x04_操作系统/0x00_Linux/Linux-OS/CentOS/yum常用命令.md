Linux CentOS yum
<a name="rVt09"></a>
### 常用命令
<a name="Y5CLc"></a>
#### 1.安装软件
_yum install 软件名称_
<a name="DN4pl"></a>
#### 2.卸载软件
_yum remove 软件名称_
<a name="ocFZZ"></a>
#### 3.更新软件
_yum update 软件名称_
<a name="IGEW4"></a>
#### 4.列出所有可安装的软件包
yum list
<a name="OxDfG"></a>
#### 5.列出所有可更新的软件包
_yum list updates_
<a name="05rpt"></a>
#### 6.列出所有已安装的软件包
_yum list installed_
<a name="Y9CaB"></a>
#### 7.列出所有可更新的软件包信息
_yum info updates_
<a name="xG4fK"></a>
#### 8.列出所有已安装的软件包信息
_yum info installed<br />_
<a name="scOXw"></a>
### 其它命令
check Check for problems in the rpmdb<br />check-update 检查是否有软件包更新<br />clean 删除缓存的数据<br />deplist 列出软件包的依赖关系<br />distribution-synchronization Synchronize installed packages to the latest available versions<br />downgrade downgrade a package<br />erase 从系统中移除一个或多个软件包<br />groupinfo 显示组的详细信息<br />groupinstall 向系统中安装一组软件包<br />grouplist 列出可安装的组<br />groupremove 从系统中移除一组软件包<br />help 显示用法信息<br />history Display, or use, the transaction history<br />info 显示关于软件包或组的详细信息<br />install 向系统中安装一个或多个软件包<br />list 列出一个或一组软件包<br />load-transaction load a saved transaction from filename<br />makecache 创建元数据缓存<br />provides 查找提供指定内容的软件包<br />reinstall 覆盖安装一个包<br />repolist 显示已配置的仓库<br />resolvedep 判断哪个包提供了指定的依赖<br />search 在软件包详细信息中搜索指定字符串<br />shell 运行交互式的 yum 外壳<br />update 更新系统中的一个或多个软件包<br />update-minimal Works like update, but goes to the 'newest' package match which fixes a problem that affects your system<br />updateinfo Acts on repository update information<br />upgrade 更新软件包同时考虑软件包取代关系<br />version yum版本信息
