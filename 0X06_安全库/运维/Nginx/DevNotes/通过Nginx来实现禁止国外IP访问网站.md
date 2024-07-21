Nginx <br />基于 NGINX 的 `ngx_http_geoip2` 模块 来禁止国外 IP 访问网站
<a name="UE784"></a>
## 一、安装 `geoip2` 扩展依赖
```bash
[root@fxkj ~]# yum install libmaxminddb-devel -y
```
<a name="gHAPV"></a>
## 二、下载 `ngx_http_geoip2_module` 模块
```bash
[root@fxkj tmp]#  git clone https://github.com/leev/ngx_http_geoip2_module.git
[ro tmp]#
```
<a name="ruBbH"></a>
## 三、解压模块到指定路径
这里解压到/usr/local 目录下
```bash
[root@fxkj tmp]# mv ngx_http_geoip2_module/ /usr/local/
[root@fxkj local]# ll ngx_http_geoip2_module/
total 60
-rw-r--r-- 1 root root  1199 Aug 13 17:20 config
-rw-r--r-- 1 root root  1311 Aug 13 17:20 LICENSE
-rw-r--r-- 1 root root 23525 Aug 13 17:20 ngx_http_geoip2_module.c
-rw-r--r-- 1 root root 21029 Aug 13 17:20 ngx_stream_geoip2_module.c
-rw-r--r-- 1 root root  3640 Aug 13 17:20 README.md
```
<a name="MBfV9"></a>
## 四、安装 nginx 模块
首先说明下环境， 这里的Nginx版本是 1.16 , 在网上查了下 安装 ngx_http_geoip2 模块至少需要 1.18 版本及以上，因此此次安装 升级 nginx1.18，添加 ngx_http_geoip2 模块。

- 下载 nginx 1.18 版本
```bash
[root@fxkj ~]# yum install libmaxminddb-devel -y
```

- 解压 nginx1.18 软件包并 升级为 nginx1.18 ，添加 ngx_http_geoip2 模块

需要注意：<br />**1、升级 nginx, 添加 nginx 模块 只需要 编译 然后 **`**make**`** 不需要 **`**make instll**`** 不然线上的 nginx 会被新版本 nginx 完完整整的替换掉**<br />**2、编译前 需要看下 nginx 当前安装了哪些模块**
```bash
[root@fxkj tmp]# /usr/local/nginx/sbin/nginx -V

nginx version: nginx/1.16.0

built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)

built with OpenSSL 1.0.2k-fips 26 Jan 2017

TLS SNI support enabled

configure arguments: –with-http_stub_status_module –prefix=/usr/local/nginx –user=nginx –group=nginx –with-http_ssl_module –with-stream
```
编译安装
```bash
[root@fxkj tmp]# tar -xf nginx-1.18.0.tar.gz
[root@fxkj tmp]# cd nginx-1.18.0/
[root@fxkj nginx-1.18.0]# ./configure --with-http_stub_status_module \
 --prefix=/usr/local/nginx \
 --user=nginx --group=nginx --with-http_ssl_module --with-stream \
 --add-module=/usr/local/ngx_http_geoip2_module
[root@fxkj nginx-1.18.0]# make
[root@fxkj nginx-1.18.0]# cp /usr/loca/nginx/sbin/nginx /usr/loca/nginx/sbin/nginx1.16    #备份
[root@fxkj nginx-1.18.0]# cp objs/nginx /usr/local/nginx/sbin/    #用新的去覆盖旧的
[root@fxkj nginx-1.18.0]# pkill nginx     #杀死nginx
[root@fxkj nginx-1.18.0]# /usr/local/nginx/sbin/nginx    #再次启动Nginx
```
查看 nginx 版本 以及安装的模块
```bash
[root@fxkj nginx-1.18.0]# /usr/local/nginx/sbin/nginx -V

nginx version: nginx/1.18.0

built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)

built with OpenSSL 1.0.2k-fips 26 Jan 2017

TLS SNI support enabled

configure arguments: –with-http_stub_status_module –prefix=/usr/local/nginx –user=nginx –group=nginx –with-http_ssl_module –with-stream –add-module=/usr/local/ngx_http_geoip2_module
```
<a name="yJlVq"></a>
## 五、下载最新的 IP 地址数据库文件
模块安装成功后，还要在 Nginx 里指定数据库，在安装运行库时默认安装了两个，位于 /usr/share/GeoIP/ 目录下，一个只有 IPv4，一个包含 IPv4 和 IPv6：<br />登录[ www.maxmind.com]( www.maxmind.com) 网址，创建账户 下载最新的库文件。<br />点击左侧 ，Download Files<br />![2021-05-09-23-36-24-498030.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620574778202-4951008c-65a3-49d1-841c-ac7cc564b53b.png#clientId=u4695bf99-7976-4&from=ui&id=ud92b175a&originHeight=427&originWidth=1080&originalType=binary&size=33081&status=done&style=shadow&taskId=u8be306a9-d387-4389-99ff-821502c3f4b)<br />选择 GeoLite2 Country ，点击 Download GZIP 下载即可<br />![2021-05-09-23-36-24-586791.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620574789969-fa4d4792-24d7-4b49-9d64-dec55d2edb39.png#clientId=u4695bf99-7976-4&from=ui&id=u59f484e0&originHeight=697&originWidth=968&originalType=binary&size=63512&status=done&style=shadow&taskId=uf5d5e201-fcc9-4f84-9635-9aa1a471a18)<br />上传到 /usr/share/GeoIP/ 下并解压
```bash
[root@fxkj local]# cd /usr/share/GeoIP/
[root@fxkj GeoIP]# ll
total 69612
lrwxrwxrwx. 1 root root       17 Mar  7  2019 GeoIP.dat -> GeoIP-initial.dat
-rw-r--r--. 1 root root  1242574 Oct 30  2018 GeoIP-initial.dat
lrwxrwxrwx. 1 root root       19 Mar  7  2019 GeoIPv6.dat -> GeoIPv6-initial.dat
-rw-r--r--. 1 root root  2322773 Oct 30  2018 GeoIPv6-initial.dat
-rw-r--r--  1 root root  3981623 Aug 12 02:37 GeoLite2-Country.mmdb
```
<a name="PfOsL"></a>
## 六、配置 nginx 配置文件
修改前 先备份配置文件
```bash
[root@fxkj ~]# cp /usr/local/nginx/conf/nginx.conf /usr/local/nginx/conf/nginx.conf-bak
[root@fxkj ~]# vim /usr/local/nginx/conf/nginx.conf
```
在 http 中添加 几行，定义数据库文件位置
```bash
geoip2 /usr/share/GeoIP/GeoLite2-City.mmdb {
auto_reload 5m;
$geoip2_data_country_code country iso_code;
}
  map $geoip2_data_country_code $allowed_country {
                default yes;
                CN no;
}
```
![2021-05-09-23-36-24-662566.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620574802468-e861ae8c-2ef4-4ca5-90b3-38f9bfc25725.png#clientId=u4695bf99-7976-4&from=ui&id=uad94b7d4&originHeight=168&originWidth=464&originalType=binary&size=4299&status=done&style=none&taskId=u06485a79-f73f-4c7f-b8d9-eab4dc0e816)<br />在 server 中的 location 下 添加 条件<br />如果满足 IP 是国外 IP 就 执行下面的 return 动作，这里定义了 3 种, 注释了其中两个。<br />当访问 IP 是国外 IP ，直接返回 404
```bash
if ($allowed_country = yes) {
       # return https://www.baidu.com;
       # return /home/japan;
        return 404;
 }
```
修改完毕后， 检测下配置文件，重新加载下 nginx
```bash
[root@fxkj ~]# /usr/local/nginx/sbin/nginx -t
nginx: the configuration file /usr/local/nginx/conf/nginx.conf syntax is ok
nginx: configuration file /usr/local/nginx/conf/nginx.conf test is successful
[roo@fxkj ~]# /usr/local/nginx/sbin/nginx -s reload
```
<a name="e6bB3"></a>
## 7、模拟测试验证
使用海外节点的服务器去访问网站<br />这里 IP 是 来自于韩国<br />![2021-05-09-23-36-24-742413.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620574819744-98f22c6f-59a7-4521-b25f-dd8363c2b28e.png#clientId=u4695bf99-7976-4&from=ui&id=u0fbd9718&originHeight=248&originWidth=773&originalType=binary&size=19656&status=done&style=shadow&taskId=u8581e2ac-6395-4872-9899-964443f5cad)<br />可以看到访问网站报错 404 Not Found<br />![2021-05-09-23-36-24-811381.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1620574837612-d6b8dd2f-b6c9-4444-aac1-9d2ccf428b2c.jpeg#clientId=u4695bf99-7976-4&from=ui&id=u5eb706ba&originHeight=259&originWidth=1080&originalType=binary&size=4471&status=done&style=shadow&taskId=u2068aca9-9880-410e-94ba-319beb7c6f7)<br />再来看下 nginx 的访问日志
```bash
“13.125.1.194 – – [14/Aug/2020:16:15:51 +0800] “GET /favicon.ico HTTP/1.1” 404 548 “https://www.fxkjnj.com/” “Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36”
```
![2021-05-09-23-36-24-927174.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620574846432-460fef66-f487-4013-9704-87c6ba10a8ef.png#clientId=u4695bf99-7976-4&from=ui&id=ud7ff623d&originHeight=47&originWidth=1080&originalType=binary&size=25026&status=done&style=none&taskId=u000db7f2-0987-4253-abfb-ee3f3d08a28)<br />至此，通过 Nginx 来实现禁止国外 IP 访问网站。
