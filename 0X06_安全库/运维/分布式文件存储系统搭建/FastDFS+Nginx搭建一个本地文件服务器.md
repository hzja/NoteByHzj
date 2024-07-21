<a name="ATPRw"></a>
## **前言**
FastDFS是一个开源的轻量级分布式文件系统，它对文件进行管理，功能包括：文件存储、文件同步、文件访问（文件上传、文件下载）等，解决了大容量存储和负载均衡的问题。特别适合以文件为载体的在线服务，如相册网站、视频网站等等。<br />FastDFS为互联网量身定制，充分考虑了冗余备份、负载均衡、线性扩容等机制，并注重高可用、高性能等指标，使用FastDFS很容易搭建一套高性能的文件服务器集群提供文件上传、下载等服务。<br />配备一台公网服务器作为文件上传下载，花费资源和开销比较大，如果在本地搭建FastDFS服务，只把上传后的文件通过cpolar内网端口转发工具将文件配置可通过互联网访问，极大了减少服务器费用及开销，本文介绍详细搭建FastDFS教程，并结合Nginx+cpolar工具实现远程访问文件。
<a name="ZSdjt"></a>
## **1、本地搭建FastDFS文件系统**
<a name="RBs5l"></a>
### 1.1 环境安装
由于fastDFS是由c语言编写，所以需要安装gcc
```bash
yum -y install gcc-c++
```
FastDFS依赖libevent库，需要安装。执行如下命令：
```bash
yum -y install libevent
```
<a name="JyRpW"></a>
### 1.2 安装libfastcommon
进入local目录
```bash
cd /usr/local
```
libfastcommon是FastDFS官方提供的，libfastcommon包含了FastDFS运行所需要的一些基础库。点击下面链接即可获取：[https://github.com/happyfish100/libfastcommon/releases/tag/V1.0.7](https://github.com/happyfish100/libfastcommon/releases/tag/V1.0.7)<br />获取后，上传到local目录下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174493513-d9c3283f-e2c5-4a67-8fbd-5397ad61f287.png#averageHue=%23fcf4e0&clientId=u2f1e547b-1d96-4&from=paste&id=ue50cd90b&originHeight=511&originWidth=832&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u44e27c30-3209-4871-8dc3-f4edd93f83f&title=)<br />解压libfastcommon
```bash
tar -zxvf libfastcommonV1.0.7.tar.gz
```
进入解压文件夹
```bash
cd libfastcommon-1.0.7/
```
编译
```bash
./make.sh
```
安装
```bash
./make.sh install
```
libfastcommon安装好后会自动将库文件拷贝至/usr/lib64下，由于FastDFS程序引用usr/lib目录所以需要将/usr/lib64下的库文件拷贝至/usr/lib下。
```bash
cp /usr/lib64/libfastcommon.so /usr/lib/
```
<a name="ouQaS"></a>
### 1.3 安装FastDFS
进入local目录
```bash
cd /usr/local
```
点击下列地址，可获取下载FastDFS_v5.05.tar.gz文件：[https://sourceforge.net/projects/fastdfs/](https://sourceforge.net/projects/fastdfs/)<br />下载文件后，上传FastDFS_v5.05.tar.gz到/usr/local/<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174493536-1c8932e0-36f3-4686-b279-83cfe7db38d8.png#averageHue=%23fcf4e0&clientId=u2f1e547b-1d96-4&from=paste&id=u2f7c89c1&originHeight=395&originWidth=735&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueee44e3a-7acd-4cef-917a-096d1771f6a&title=)<br />解压文件
```bash
tar -zxvf FastDFS_v5.05.tar.gz
```
进入解压文件夹
```bash
cd FastDFS
```
编译
```bash
./make.sh
```
安装
```bash
./make.sh install
```
安装成功后，将安装目录下的conf下的文件拷贝到/etc/fdfs/下
```bash
cd conf/
```
拷贝
```bash
cp * /etc/fdfs/
```
<a name="zDXMn"></a>
### 1.4 配置Tracker
接下来进行配置，安装成功后进入/etc/fdfs目录
```bash
cd /etc/fdfs/
```
修改tracker.conf
```bash
vim tracker.conf
```
把base_path=/home/yuqing/fastdfs改为base_path=/home/fastdfs<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174493533-e0080536-c690-4d76-9866-9d0d994263e1.png#averageHue=%23fcf4e1&clientId=u2f1e547b-1d96-4&from=paste&id=uc7bab05e&originHeight=511&originWidth=832&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9df4c03a-554e-4a74-b85d-94b53e6089f&title=)<br />编辑后保存退出，然后创建一个对应的fastdfs文件夹
```bash
mkdir /home/fastdfs
```
然后启动
```bash
/usr/bin/fdfs_trackerd /etc/fdfs/tracker.conf restart
```
设置开机启动
```bash
vim /etc/rc.d/rc.local
```
将运行命令行添加进文件
```bash
/usr/bin/fdfs_trackerd /etc/fdfs/tracker.conf restart
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174493513-3d3dd9de-062e-44a2-b330-7f8be6a186a2.png#averageHue=%23fcf4e1&clientId=u2f1e547b-1d96-4&from=paste&id=ua1a4cd4e&originHeight=511&originWidth=832&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubfd26efe-09d6-467a-b876-ee8a02a4909&title=)
<a name="M96kN"></a>
### 1.5 配置Storage
进入/etc/fdfs目录
```bash
cd /etc/fdfs
```
修改storage.conf
```bash
vim storage.conf
```
把base_path=/home/yuqing/fastdfs改为base_path=/home/fastdfs<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174493537-32a4434b-541e-40a6-8d13-0c398e79888f.png#averageHue=%23fcf5e2&clientId=u2f1e547b-1d96-4&from=paste&id=ub151bd5c&originHeight=60&originWidth=380&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub50cf0af-3fce-4d9d-a822-95d24de99e8&title=)<br />把 store_path0=/home/yuqing/fastdfs 改为store_path0=/home/fastdfs/fdfs_storage<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494086-fa6ff82d-1ca4-4f37-919f-81abfce62ee8.png#averageHue=%23fcf5e2&clientId=u2f1e547b-1d96-4&from=paste&id=u677741f9&originHeight=78&originWidth=627&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8e46180f-4e5c-46cb-ae28-47a0c16c2f5&title=)<br />继续修改，设置tracker服务器：IP，tracker_server=Linux局域网ip:22122，设置好后保存退出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494137-631cef8e-7a3c-43fa-b488-deab7d204e55.png#averageHue=%23fcf5e2&clientId=u2f1e547b-1d96-4&from=paste&id=u24db0cba&originHeight=86&originWidth=467&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue94eebc3-2f5a-456f-b7eb-74c63a20219&title=)<br />创建对应fdfs_storage文件夹
```bash
mkdir /home/fastdfs/fdfs_storage
```
然后启动
```bash
/usr/bin/fdfs_storaged /etc/fdfs/storage.conf restart
```
设置开机自动启动
```bash
vim /etc/rc.d/rc.local
```
将运行命令行添加到文件，然后保存
```bash
/usr/bin/fdfs_storaged /etc/fdfs/storage.conf restart
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494148-d57da84e-8913-474b-a7a9-9b532e420f9b.png#averageHue=%23fcf4e1&clientId=u2f1e547b-1d96-4&from=paste&id=u970eb55b&originHeight=511&originWidth=832&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucd901bb3-3430-4f0f-9569-e23af2a7897&title=)
<a name="G1m22"></a>
### 1.6 测试上传下载
FastDFS安装成功可通过/usr/bin/fdfs_test命令测试上传、下载等操作。<br />修改/etc/fdfs/client.conf
```bash
vim /etc/fdfs/client.conf
```
修改如下内容，然后保存：
```bash
base_path=/home/fastdfs

tracker_server=Linux局域网ip:22122
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494124-38afb8ad-9634-417a-9ea2-9039ff20ffb5.png#averageHue=%23fcf5e2&clientId=u2f1e547b-1d96-4&from=paste&id=u3873e96b&originHeight=102&originWidth=558&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u75bcb1ee-4458-4fa3-b291-b8a195e064f&title=)<br />接下来测试上传文件，这里上传了一张图片到/usr/local目录下，作为测试使用<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494220-9e92a202-14ed-4e64-a0a9-9dc582507968.png#averageHue=%23fcf4e0&clientId=u2f1e547b-1d96-4&from=paste&id=u8e9cc722&originHeight=399&originWidth=722&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua340e4b9-054d-46cc-9a34-4aafc6447ca&title=)<br />FastDFS上传文件使用格式：<br />/usr/bin/fdfs_test /etc/fdfs/client.conf upload+ 上传文件的路径<br />然后按照格式，测试将test23.png图片上传到FastDFS中<br />/usr/bin/fdfs_test /etc/fdfs/client.conf upload /usr/local/test23.png<br />执行后，可以看到返回地址信息，表示成功，但是目前还访问不了，需要结合Nginx做访问下载<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494468-da38bce8-ba91-498d-bc28-29df7ed76f78.png#averageHue=%23fcf4e0&clientId=u2f1e547b-1d96-4&from=paste&id=u5d683567&originHeight=435&originWidth=903&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a5fc85b-fe53-4897-958a-cc0907f8014&title=)
<a name="BzOaU"></a>
### 1.7 与Nginx整合
点击下载下面链接下载fastdfs-nginx-module_v1.16：<br />[https://sourceforge.net/projects/fastdfs/files/FastDFS Nginx Module Source Code/fastdfs-nginx-module_v1.16.tar.gz/download](https://sourceforge.net/projects/fastdfs/files/FastDFS Nginx Module Source Code/fastdfs-nginx-module_v1.16.tar.gz/download)<br />进入 /usr/local/文件夹下
```bash
cd /usr/local/
```
上传fastdfs-nginx-module_v1.16.tar.gz<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494497-b4d8ce65-781d-4483-8344-d5566a84a09d.png#averageHue=%23fcf4e0&clientId=u2f1e547b-1d96-4&from=paste&id=u97cbffe5&originHeight=389&originWidth=751&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93194211-21c9-4cdc-936a-39d136350ae&title=)<br />解压fastdfs-nginx-module_v1.16.tar.gz
```bash
tar -zxvf fastdfs-nginx-module_v1.16.tar.gz
```
进入src，然后编辑配置文件
```bash
cd fastdfs-nginx-module/src/
```
修改config文件
```bash
vim config
```
修改这三个位置，将里面原本是/usr/local/的路径改为/usr/，然后保存退出<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494553-7f1fdc36-12b8-405a-9b86-2731c1e0650d.png#averageHue=%23fbf3e0&clientId=u2f1e547b-1d96-4&from=paste&id=uac2d1bec&originHeight=129&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubccf3e34-7668-44fc-9153-f71c3e10d3d&title=)<br />然后将FastDFS-nginx-module/src下的mod_FastDFS.conf拷贝至/etc/fdfs/下
```bash
cp mod_fastdfs.conf /etc/fdfs/
```
修改mod_FastDFS.conf的内容：
```bash
vim /etc/fdfs/mod_fastdfs.conf
```
修改内容如下：
```
base_path=/home/fastdfs

tracker_server=Linux局域网ip地址:22122

url_have_group_name=true #url中包含group名称

store_path0=/home/fastdfs/fdfs_storage #指定文件存储路径
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494620-b3a6da23-1206-4031-9473-bc3129ea4605.png#averageHue=%23fcf5e2&clientId=u2f1e547b-1d96-4&from=paste&id=u04ed2965&originHeight=945&originWidth=691&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u46983325-b141-4041-a1e9-4495ce9b6be&title=)<br />接下来将libfdfsclient.so拷贝至/usr/lib下
```bash
cp /usr/lib64/libfdfsclient.so /usr/lib/
```
创建nginx/client目录即可
```bash
mkdir -p /var/temp/nginx/client
```
<a name="uVFte"></a>
### 1.8 安装Nginx
首先安装系列环境
```bash
yum -y install pcre pcre-devel  

yum -y install zlib zlib-devel  

yum -y install openssl openssl-devel
```
然后点击下载Nginx包：[http://nginx.org/en/download.html](http://nginx.org/en/download.html)<br />上传到root目录下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174494744-c24f05c0-9321-4ba9-b0e2-602a031e7da6.png#averageHue=%23fcf3df&clientId=u2f1e547b-1d96-4&from=paste&id=u4434add7&originHeight=115&originWidth=586&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud77b77cd-5361-4818-9fea-e4452f5213f&title=)<br />解压，并进入解压文件夹
```bash
tar -zxvf nginx-1.16.1.tar.gz && cd nginx-1.16.
```
添加FastDFS-nginx-module模块，执行以下命令:
```bash
./configure \
--prefix=/usr/local/nginx \
--pid-path=/var/run/nginx/nginx.pid \
--lock-path=/var/lock/nginx.lock \
--error-log-path=/var/log/nginx/error.log \
--http-log-path=/var/log/nginx/access.log \
--with-http_gzip_static_module \
--http-client-body-temp-path=/var/temp/nginx/client \
--http-proxy-temp-path=/var/temp/nginx/proxy \
--http-fastcgi-temp-path=/var/temp/nginx/fastcgi \
--http-uwsgi-temp-path=/var/temp/nginx/uwsgi \
--http-scgi-temp-path=/var/temp/nginx/scgi \
--add-module=/usr/local/fastdfs-nginx-module/src
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516512-1bd4c1c5-88cf-45ec-8498-dc161485e513.png#averageHue=%23fbf3df&clientId=u2f1e547b-1d96-4&from=paste&id=u99935eb9&originHeight=177&originWidth=539&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8352fb79-4d62-4921-958c-d491f7dfd1a&title=)<br />然后编译
```bash
make
```
安装
```bash
make install
```
<a name="ZE7yc"></a>
### 1.9 配置Nginx
修改Nginx配置文件
```bash
vim /usr/local/nginx/conf/nginx.conf
```
添加server，添加后保存退出：
```nginx
server {
  listen       8089;#为了防止80端口,改成8089
  server_name  192.168.59.133;#这里需要使用你本机的局域网ip

  location /group1/M00/{
    ngx_fastdfs_module;
  }
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516571-35be4a26-3235-446b-ae47-5f127a36eb3b.png#averageHue=%23fcf4e1&clientId=u2f1e547b-1d96-4&from=paste&id=u744218ee&originHeight=815&originWidth=742&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10af294b-9d5f-4586-8a5b-34ff99216e2&title=)<br />启动Nginx
```bash
/usr/local/nginx/sbin/nginx
```
如果启动nginx出现以下错误：<br />nginx: [emerg] open() “/var/run/nginx/nginx.pid” failed (2: No such file or directory)<br />可以执行如下命令，然后再次启动：
```bash
mkdir /var/run/nginx
```
设置开机自动启动。
```bash
vim /etc/rc.d/rc.local
```
将运行命令行添加进文件，然后保存退出：
```bash
/usr/local/nginx/sbin/nginx
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516620-41cf610c-04d4-414a-9c32-793c1fd382dd.png#averageHue=%23fcf4e1&clientId=u2f1e547b-1d96-4&from=paste&id=ufc4de654&originHeight=516&originWidth=808&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud18efec1-a854-40c8-bfff-bd457818d27&title=)<br />关闭防火墙
```bash
service iptables stop
```
设置防火墙不自动启动
```bash
chkconfig iptables off
```
<a name="YBrNO"></a>
## **2、局域网测试访问FastDFS**
然后按照格式，测试将上面上传的test23.png图片上传到FastDFS中
```bash
/usr/bin/fdfs_test /etc/fdfs/client.conf upload /usr/local/test23.png
```
执行后，可以看到返回地址信息，表示成功<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516535-04a97544-a5a4-4bed-8013-65f8c826a4a8.png#averageHue=%23fbf3e0&clientId=u2f1e547b-1d96-4&from=paste&id=u5b417d10&originHeight=441&originWidth=787&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub8fa6543-02fa-40e2-89c7-b8dd78a8c5f&title=)<br />然后使用上面的地址，在浏览器访问，即可看到上传的图片，fastDFS搭建完成<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516528-47d1b220-d8c2-4685-b75a-838e38de5e8f.png#averageHue=%23484746&clientId=u2f1e547b-1d96-4&from=paste&id=uf82e2b7f&originHeight=709&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5fe3ab3e-490c-4e34-82f5-4388dcfff6d&title=)
<a name="g9Ayk"></a>
## **3、安装cpolar内网穿透**
上面在本地Linux虚拟机安装了fastDFS分布式文件服务，下面安装cpolar内网穿透工具，通过cpolar的http公网地址，可以很容易远程也可以访问fastDFS里面上传的文件，而无需自己注册域名购买云服务器。下面是安装cpolar内网穿透步骤<br />cpolar官网：[https://www.cpolar.com/](https://www.cpolar.com/)
<a name="h4MpY"></a>
### 使用一键脚本安装命令
```bash
curl -L https://www.cpolar.com/static/downloads/install-release-cpolar.sh | sudo bash
```
<a name="uvipQ"></a>
### token认证
登录cpolar官网后台，点击左侧的验证，查看自己的认证token，之后将token贴在命令行里:
```
cpolar authtoken xxxxxxx
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516880-de0d8d24-7a75-4484-b4f2-073a3c155c06.png#averageHue=%23f4f2f1&clientId=u2f1e547b-1d96-4&from=paste&id=u840563f5&originHeight=588&originWidth=1016&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubc86f22a-3c70-475f-85c7-1c8654e2c65&title=)
<a name="yNW4t"></a>
### 向系统添加服务
```bash
sudo systemctl enable cpolar
```
<a name="bCf7X"></a>
### 启动cpolar服务
```bash
sudo systemctl start cpolar
```
<a name="NBMyh"></a>
## **4、配置公网访问地址**
成功启动cpolar服务后，在浏览器上访问Linux局域网ip地址+9200端口，登录cpolar web UI管理界面。<br />登录成功后，点击左侧仪表盘的隧道管理——创建隧道，创建一个http协议的隧道指向上面设置的Nginx8089端口：

- 隧道名称：可自定义，注意不要与已有的隧道名称重复
- 协议：http
- 本地地址：8089
- 域名类型：免费选择随机域名
- 地区：选择China VIP

点击创建<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174516959-432e3680-5e7f-45bb-bc9e-8e435dc52565.png#averageHue=%23dbe5b4&clientId=u2f1e547b-1d96-4&from=paste&id=u8cae0c83&originHeight=633&originWidth=860&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u74c54ba1-f571-4775-b382-8849ef35f2e&title=)<br />然后打开在线隧道列表，查看并且复制公网地址<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174517052-a05a2c77-8a40-4d64-a5b2-2b365952af87.png#averageHue=%23e4e7c9&clientId=u2f1e547b-1d96-4&from=paste&id=u424b6036&originHeight=448&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u40802fea-681a-4072-8f94-2f3d6e57681&title=)<br />然后打开浏览器，输入公网地址，访问上传在fastDFS里的文件，即可远程访问成功<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174517172-6ffb70d8-6560-44f4-aa0a-bdcd28110faa.png#averageHue=%234a4948&clientId=u2f1e547b-1d96-4&from=paste&id=u8ed40559&originHeight=679&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub6e87e1d-d713-4576-84eb-870dd54aee8&title=)
<a name="WuAJe"></a>
## **5、固定公网地址**
由于刚刚创建隧道使用的是随机临时地址，该地址会在24小时内发生变化，为了长期和更稳定远程访问，接下来将这个公网地址配置为固定的。
<a name="yEjlE"></a>
### 5.1 保留二级子域名
需升级至基础套餐或以上才支持配置二级子域名，如有下载文件的需求，可以升级更高套餐以便获取更大的带宽，提高下载速度。<br />登录cpolar官网后台，点击左侧仪表盘的预留，找到保留二级子域名，为http隧道保留一个二级子域名。

- 地区：选择服务器地区
- 名称：填写您想要保留的二级子域名（可自定义）
- 描述：即备注，可自定义填写

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174517172-b5c054a4-8807-4fd1-98fb-465c07b7a978.png#averageHue=%23fcf6f4&clientId=u2f1e547b-1d96-4&from=paste&id=u417a0de3&originHeight=377&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u05753dc4-9118-4d3d-8a3a-0709c8e4c51&title=)<br />本例保留一个名称为fasttest的二级子域名。子域名保留成功后，将子域名复制下来，接下来需要将其配置到隧道中去。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174517908-20a403e7-e108-41f3-b1fe-de885501547b.png#averageHue=%23fcf7f6&clientId=u2f1e547b-1d96-4&from=paste&id=u540df849&originHeight=461&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uea7507a1-8ce2-416a-945c-427dbf55602&title=)
<a name="MG7z2"></a>
### 5.2 配置二级子域名
登录cpolar web ui管理界面。点击左侧仪表盘的隧道管理——隧道列表，找到需要配置二级子域名的隧道，点击右侧的编辑<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174518051-538e0ff4-d586-43e1-9867-e79dc726174f.png#averageHue=%23dbe6bd&clientId=u2f1e547b-1d96-4&from=paste&id=u4d7704d2&originHeight=351&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u67a3ccc7-4a2b-48e7-8865-5d1607097c9&title=)<br />修改隧道信息，将二级子域名配置到隧道中：

- 域名类型：改为选择二级子域名
- Sub Domain：填写刚刚所保留的二级子域名（本例为fasttest）

修改完成后，点击更新<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174518363-aaf2045a-713f-4a8d-9191-507d2477275c.png#averageHue=%23dbe4b6&clientId=u2f1e547b-1d96-4&from=paste&id=uec987879&originHeight=661&originWidth=985&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20226e2e-da09-4a03-afa4-f1ce1042bed&title=)<br />隧道更新成功后，点击左侧仪表盘的状态——在线隧道列表，可以看到隧道的公网地址，已经更新为二级子域名了，将公网地址复制下来。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174518490-698ae979-3a6a-4a93-96cb-1af5e77b16d9.png#averageHue=%23fefdfc&clientId=u2f1e547b-1d96-4&from=paste&id=uee0838d3&originHeight=353&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0210ed08-6121-4ab2-b532-270ea2a7991&title=)
<a name="MMfGT"></a>
## **6、测试访问固定二级子域名**
来测试一下访问配置成功的二级子域名，打开浏览器，输入公网地址加资源路径，访问上传在fastDFS里的文件，出现图片即可远程访问成功。现在，全网唯一的私有二级子域名，就创建好了。<br />而且这个地址也不会再随机变化，他是固定不变的，只要保持隧道在线，在外就可以通过这个公网地址，随时随地远程访问，无需公网IP，也不用设置路由器。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695174519023-7407778d-df50-40ff-8cfe-6f628b7a16fc.png#averageHue=%23464644&clientId=u2f1e547b-1d96-4&from=paste&id=uf3873f83&originHeight=662&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u98810f46-14fa-4f2f-bf5a-b1e2e34dc2d&title=)
