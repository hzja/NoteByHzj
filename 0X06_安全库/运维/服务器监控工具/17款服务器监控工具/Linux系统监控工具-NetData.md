Linux NetData 
<a name="BipdO"></a>
## 1、netdata官网
[netdata官网](https://learn.netdata.cloud/#installation)<br />[https://learn.netdata.cloud/#installation](https://learn.netdata.cloud/#installation)
<a name="rVH3y"></a>
## 2、安装netdata
<a name="dvWAf"></a>
### A.根据官网教程安装[一键安装]（会遇到下载访问不了的情况）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597984714756-20bc01a4-ddb9-42c8-b3ed-84905dc3799a.png#align=left&display=inline&height=686&originHeight=2059&originWidth=3798&size=699641&status=done&style=shadow&width=1266)
```bash
yum update -y
bash <(curl -Ss https://my-netdata.io/kickstart.sh)
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597847512385-9f694463-ff70-4fb1-acaf-01e141838533.png#align=left&display=inline&height=524&originHeight=1048&originWidth=1991&size=778082&status=done&style=none&width=995.5)<br />其中下载安装过程中需要输入回车以及y进行确定
<a name="EW0SZ"></a>
### B.从Github的发布页下载安装
<a name="wlh9j"></a>
#### ①安装必要的依赖
```bash
yum install zlib-devel gcc make git autoconf autogen automake pkgconfig
```
<a name="6RFvm"></a>
#### ②从Github发布页选择版本下载
[NetData的Github发布页](https://github.com/netdata/netdata/releases)<br />[https://github.com/netdata/netdata/releases](https://github.com/netdata/netdata/releases)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597984794686-fb0df346-0994-4255-b759-ff747042606e.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3798&size=555404&status=done&style=shadow&width=1266)
```bash
cd /local/tool/netdata
wget https://github.com/netdata/netdata/releases/download/v1.24.0/netdata-v1.24.0.tar.gz
tar -xf netdata-1.24.0.tar.gz
cd netdata-1.24.0
./netdata-installer.sh
# 接下来就是漫长的下载安装过程9（预计两个小时，建议在tmux或者Screen中执行）
```
如果使用wget访问Github下载比较慢也可以下载下面的压缩包使用FTP工具上传执行后面的命令<br />[netdata-v1.24.0.tar.gz](https://www.yuque.com/attachments/yuque/0/2020/gz/396745/1597987062288-d03cf03c-d49d-45c7-b466-811c2feba43e.gz?_lake_card=%7B%22uid%22%3A%221597987059784-0%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fgz%2F396745%2F1597987062288-d03cf03c-d49d-45c7-b466-811c2feba43e.gz%22%2C%22name%22%3A%22netdata-v1.24.0.tar.gz%22%2C%22size%22%3A4390275%2C%22type%22%3A%22application%2Fx-gzip%22%2C%22ext%22%3A%22gz%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22BUYYL%22%2C%22card%22%3A%22file%22%7D)
<a name="H9UPP"></a>
#### ③安装成功的显示
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597992754685-26627276-aed7-43ea-ace6-e3f6065116bc.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2055890&status=done&style=none&width=1107.6666666666667)
<a name="fb5zr"></a>
## 3、然后使用http://服务器主机IP:1999进行访问
<a name="8Cn0S"></a>
## 4、netdata的配置文件：/etc/netdata/netdata.conf
端口配置等在此配置文件进行更改
<a name="N8kVG"></a>
## 5、启动/关闭NetData
a、关闭（安装成功默认开启）killall netdata<br />b、开启 /user/sbin/netdata
<a name="mTh6V"></a>
## 6、NetData的效果图
可以看到系统整体情况、CPU、磁盘、网络等情况一目了然<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597993460391-59290d40-4f8f-460a-acff-a499e61ea39e.png#align=left&display=inline&height=684&originHeight=2051&originWidth=3834&size=848586&status=done&style=shadow&width=1278)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597993523198-97b19781-8a31-4869-8ca4-dd62abec57d7.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=846317&status=done&style=none&width=1280)
