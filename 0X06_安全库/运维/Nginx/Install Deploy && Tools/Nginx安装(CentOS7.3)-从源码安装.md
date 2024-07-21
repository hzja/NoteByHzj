Linux CentOS Nginx<br />Nginx是使用C语言开发的，建议部署在Linux操作系统上，当然也可以安装在Windows操作系统安装Windows版本的Nginx。<br />Nginx官方下载地址：[https://nginx.org/en/download.html](https://nginx.org/en/download.html)
<a name="zq4UW"></a>
## 1、安装依赖库
```bash
yum -y install gcc gcc-c++ pcre pcre-devel \
 zlib zlib-devel openssl openssl-devel
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597890454695-f68f868e-32f8-448f-a535-d4bd1c2eb9a1.png#averageHue=%233a3a3a&height=351&id=zjHYn&originHeight=1052&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1469167&status=done&style=none&title=&width=1107.6666666666667)
<a name="Riqla"></a>
### gcc安装
源码安装 Nginx需要对在官网下载的Nginx源码进行编译，编译依赖 gcc 环境，如果没有 gcc 环境，则需要先安装<br />[CentOS7.3安装升级gcc](https://www.yuque.com/fcant/linux/vv50z7?view=doc_embed)
<a name="DVpSg"></a>
### PCRE pcre-devel 安装
PCRE(Perl Compatible Regular Expressions) 是一个Perl库，包括 perl 兼容的正则表达式库。nginx 的 http 模块使用 pcre 来解析正则表达式，所以需要在 linux 上安装 pcre 库，pcre-devel 是使用 pcre 开发的一个二次开发库。nginx也需要此库。
```bash
yum install -y pcre pcre-devel
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597890588914-e78ac376-f0bd-4740-908d-94dc3eda1874.png#averageHue=%234c4c4c&height=234&id=vykxX&originHeight=702&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=852541&status=done&style=none&title=&width=1107.6666666666667)
<a name="V7iy4"></a>
### zlib 安装
zlib 库提供了很多种压缩和解压缩的方式， Nginx使用 zlib 对 http 包的内容进行 gzip ，所以需要在 CentOS上安装 zlib 库。
```
yum install -y zlib zlib-devel
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597890634213-b138f3c8-5a32-40fe-ae40-2d98d9efa2d2.png#averageHue=%234b4b4b&height=234&id=sJYLh&originHeight=703&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=853492&status=done&style=none&title=&width=1107.6666666666667)
<a name="1tIY5"></a>
### OpenSSL 安装
OpenSSL 是一个强大的安全套接字层密码库，囊括主要的密码算法、常用的密钥和证书封装管理功能及 SSL 协议，并提供丰富的应用程序供测试或其它目的使用。Nginx不仅支持 http 协议，而且支持 https（即在ssl协议上传输http），所以需要在 CentOS安装 OpenSSL 库。
```
yum install -y openssl openssl-devel
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597890710084-dcce8762-2ece-41ca-8240-0abf60d3165a.png#averageHue=%234d4d4d&height=236&id=vfc9l&originHeight=708&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=864084&status=done&style=none&title=&width=1107.6666666666667)
<a name="B5Xbo"></a>
## 2、创建Nginx用户及Nginx用户组
先创建一个名为nginx且没有登录权限的用户和一个名为nginx的用户组
<a name="YCFvT"></a>
### 创建Nginx用户组
创建`nginx`用户组(`-r`选项表示创建一个系统用户组)
```bash
groupadd -r nginx
```
创建用户并加入到nginx系统用户组
<a name="q5AKh"></a>
### 创建Nginx用户
```bash
useradd -r -g nginx -s /sbin/nologin -d /usr/local/nginx -M nginx
```
-r：添加系统用户( 这里指将要被创建的系统用户`nginx`)<br />-g：指定要创建的用户所属组( 这里指添加新系统用户`nginx`到`nginx`系统用户组 )<br />-s：新帐户的登录`shell`( `/sbin/nologin` 这里设置为将要被创建系统用户`nginx`不能用来登录系统 )<br />-d：新帐户的主目录( 这里指定将要被创建的系统用户`nginx`的家目录为 `/usr/local/nginx` )<br />-M：不要创建用户的主目录( 也就是说将要被创建的系统用户`nginx`不会在 `/home` 目录下创建 `nginx` 家目录 )
<a name="WIakP"></a>
## 3、下载Nginx源码并安装
<a name="e0b291aa"></a>
### 下载Nginx
```bash
wget http://nginx.org/download/nginx-1.16.1.tar.gz
```
<a name="2b9563e8"></a>
### 解压
```bash
tar zxf nginx-1.16.1.tar.gz -C ./nginx
```
<a name="6opX9"></a>
### 编译安装Nginx
```bash
cd ./nginx/nginx-1.16.1/
./configure --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-file-aio --with-http_realip_module
make && make install
```
安装成功后，查看Nginx内置模块或者查看自定义模块<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695261241109-296a0389-46cf-4f5d-977d-3035c552bce7.png#averageHue=%233a3a3a&clientId=u9f4fa0a0-4a7e-4&from=paste&height=182&id=u0570ca6e&originHeight=454&originWidth=3296&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=552214&status=done&style=none&taskId=u7e044020-422c-4c56-af6f-786ec6a5b33&title=&width=1318.4)<br />如果自带的模块不能满足需求可以自定义添加，以下是Windows版的Nginx内置模块可参考按需添加：
```
--with-http_v2_module --with-http_realip_module --with-http_addition_module --with-http_sub_module --with-http_dav_module --with-http_stub_status_module --with-http_flv_module --with-http_mp4_module --with-http_gunzip_module --with-http_gzip_static_module --with-http_auth_request_module --with-http_random_index_module --with-http_secure_link_module --with-http_slice_module --with-mail --with-stream --with-openssl=objs.msvc8/lib/openssl-1.1.1q --with-openssl-opt='no-asm no-tests -D_WIN32_WINNT=0x0501' --with-http_ssl_module --with-mail_ssl_module --with-stream_ssl_module
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695262787085-dfe8217d-4146-4e3b-8bed-68203e91a0c4.png#averageHue=%23404040&clientId=u9f4fa0a0-4a7e-4&from=paste&height=328&id=u442f0cd9&originHeight=820&originWidth=3297&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1127378&status=done&style=none&taskId=u935c0661-896c-4611-92dd-bd529a9f36d&title=&width=1318.8)
<a name="hEdJa"></a>
## 4、Nginx模块
<a name="hH8jq"></a>
### 查看Nginx内置模块
```bash
cat ./auto/options | grep YES
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597891264899-e42421e1-6c3b-48ec-a1d0-411da0729877.png#averageHue=%233c3c3c&height=583&id=GqbOb&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2134719&status=done&style=none&title=&width=1107.6666666666667)
<a name="hjah0"></a>
### 查看Nginx自定义模块
需要切换至/usr/local/nginx/sbin目录下
```bash
[root@iZuligp6e1dyzfZ nginx-1.16.1 10:48]# cd /usr/local/nginx/sbin/
[root@iZuligp6e1dyzfZ sbin 10:49]# pwd
/usr/local/nginx/sbin
[root@iZuligp6e1dyzfZ sbin 10:49]# ll
total 3736
-rwxr-xr-x 1 root root 3825520 Aug 20 10:40 nginx
[root@iZuligp6e1dyzfZ sbin 10:49]# ./nginx -V
nginx version: nginx/1.16.1
built by gcc 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)
configure arguments: --prefix=/usr/local/nginx --user=nginx --group=nginx
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597891963190-af9f5ec1-0b48-4878-ae1e-7a05f52e75f6.png#averageHue=%234b4b4b&height=254&id=Bii2u&originHeight=761&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=973131&status=done&style=none&title=&width=1107.6666666666667)
<a name="SXMCk"></a>
### 添加第三方模块
<a name="5qN9b"></a>
## 5、验证Nginx的安装
在/usr/local/nginx/sbin目录下执行Nginx启动命令后，访问80端口出现页面即可说明安装启动成功
<a name="XmNtX"></a>
### 使用curl访问
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597893300822-d8bd7357-b324-478e-a8b3-afa84e0f9201.png#averageHue=%233c3c3c&height=583&id=CCElR&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2188987&status=done&style=none&title=&width=1107.6666666666667)
<a name="Rj47J"></a>
### 使用浏览器访问
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597893420085-1b81bda9-175d-499d-86e4-bcf26669934b.png#averageHue=%23fbfafa&height=324&id=CEVfI&originHeight=972&originWidth=3837&originalType=binary&ratio=1&rotation=0&showTitle=false&size=264918&status=done&style=shadow&title=&width=1279)
<a name="40s8M"></a>
## 6、Nginx的常用命令
需要切换至nginx的可执行程序的目录下进行操作<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597893270602-74f70252-bbce-4849-99b3-631fde1b6505.png#averageHue=%23464646&height=337&id=F7FYy&originHeight=1011&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1320807&status=done&style=none&title=&width=1107.6666666666667)
```bash
cd /usr/local/nginx/sbin/
./nginx       # 启动
./nginx -s stop  #关闭
./nginx -s quit
./nginx -s reload  #重新加载配置
./nginx -s quit:此方式停止步骤是待nginx进程处理任务完毕进行停止。
./nginx -s stop:此方式相当于先查出nginx进程id再使用kill命令强制杀掉进程。
```
<a name="ArpoN"></a>
## 7、生产环境Nginx的使用
<a name="M3RFl"></a>
### 先停止再启动（推荐）
对 nginx 进行重启相当于先停止再启动，即先执行停止命令再执行启动命令。
```bash
./nginx -s quit
./nginx
```
<a name="7mVz1"></a>
### 修改nginx.conf后，重新加载配置文件
当nginx的配置文件nginx.conf修改后，要想让配置生效需要重启nginx，使用`nginx -s reload`不用先停止nginx，再启动nginx。即可将配置信息在 nginx中生效。
```bash
./nginx -s reload
```
<a name="wEm8j"></a>
### 开机自启动Nginx
在rc.local增加启动代码
```bash
vim /etc/rc.local
```
即增加一行 
```
/usr/local/nginx/sbin/nginx
```
<a name="AzJBG"></a>
### 设置rc.local文件执行权限
```
chmod 755 rc.local
```
<a name="pqoOa"></a>
## 8、Nginx一键式安装脚本
```bash
yum -y install gcc gcc-c++ pcre pcre-devel \
 zlib zlib-devel openssl openssl-devel
yum install -y pcre pcre-devel
yum install -y zlib zlib-devel
yum install -y openssl openssl-devel
groupadd -r nginx
useradd -r -g nginx -s /sbin/nologin -d /usr/local/nginx -M nginx
wget http://nginx.org/download/nginx-1.16.1.tar.gz
tar zxf nginx-1.16.1.tar.gz
cd nginx-1.16.1/
./configure --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-file-aio --with-http_realip_module
make && make install
/usr/local/nginx/sbin/nginx -V
```
