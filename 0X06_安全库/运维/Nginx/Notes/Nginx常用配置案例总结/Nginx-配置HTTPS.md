Nginx HTTPS
<a name="7hyti"></a>
## HTTPS
HTTP数据传输时没有对数据进行加密，所以导致数据不安全。而HTTPS在HTTP上加了一层，对数据进行加密，这样就保证了数据的安全性。防止传输的数据过程中被不法分子盗用、劫持、篡改，而导致数据信息的泄露。
<a name="NEIVq"></a>
### HTTPS协议的实现
对传输内容进行加密以及身份验证
<a name="Nst3t"></a>
#### 对称加密：加密秘钥和解密秘钥是一样的
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600603865618-ba80f884-8ebf-4269-84bb-f3fb8660cc87.png#height=276&id=PCYmz&originHeight=827&originWidth=2146&originalType=binary&ratio=1&rotation=0&showTitle=false&size=96753&status=done&style=shadow&title=&width=715.3333333333334)
<a name="LFqDf"></a>
#### 非对称加密：加密密钥与解密密钥是不一样的，但是是成对的。
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600604625983-73553f55-a7da-43b0-af4e-6370d832f935.png#height=228&id=JK7TW&originHeight=683&originWidth=1940&originalType=binary&ratio=1&rotation=0&showTitle=false&size=102868&status=done&style=shadow&title=&width=646.6666666666666)
<a name="QWZgJ"></a>
### HTTPS加密协议原理
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600604925453-462f15ba-2496-48ff-ac6f-2728a118c0ca.png#height=268&id=ogRzj&originHeight=805&originWidth=1801&originalType=binary&ratio=1&rotation=0&showTitle=false&size=102415&status=done&style=shadow&title=&width=600.3333333333334)<br />中间人伪造客户端和服务端：（中间人可以伪装成客户端和服务端，中间人可以对数据进行劫持，不安全）<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600605040182-b894a379-0890-4e40-b648-52956d09c022.png#height=275&id=b1d91&originHeight=826&originWidth=1881&originalType=binary&ratio=1&rotation=0&showTitle=false&size=111528&status=done&style=shadow&title=&width=627) <br />HTTPS的CA签名证书：（服务端和客户端通过实现约定好的证书进行认证，都会对证书进行校验，所以中间人没法劫持数据，故安全）<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600605184322-48e23e45-d02a-4bfc-bbbb-c75e01ae2771.png#height=268&id=FGM8L&originHeight=804&originWidth=2345&originalType=binary&ratio=1&rotation=0&showTitle=false&size=159505&status=done&style=shadow&title=&width=781.6666666666666)
<a name="mA4Kx"></a>
## 验证Nginx SSL模块的安装情况
安装Nginx时，必须首先安装openssl openssl-devel依赖 模块，而这两个模块就是HTTPS需要用户的类库；所以毫无疑问安装Nginx时，Linux系统肯定安装了这两个模块；<br />Nginx的https需要安装with-stream_ssl_preread_module目录，在`nginx的源码目录`执行以下命令，可以看出nginx默认并不会安装stream_ssl_preread_module模块的，需要重新编译安装一下
```bash
cat auto/options | grep YES  | grep ssl
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600605318974-a2cb458c-f198-4bc9-8cd8-aa981d148be8.png#height=488&id=AqMKj&originHeight=1463&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1791618&status=done&style=none&title=&width=1107.6666666666667)<br />安装一下stream_ssl_preread_module模块
```bash
./configure --prefix=/usr/local/nginx --user=nginx \
 --group=nginx --with-http_secure_link_module \
--with-http_stub_status_module --with-stream_ssl_preread_module
--with-http_ssl_modulemake
cd objs/
mv nginx /usr/local/nginx/sbin/
```
<a name="nN0I7"></a>
## 生成秘钥和CA证书
<a name="sih7q"></a>
### 生成key秘钥
先进入Nginx的安装目录，然后生成一个目录，存放密钥和CA证书
```bash
cd /usr/local/nginx/conf/
mkdir httpsKeys
cd httpsKeys
```
<a name="nBNJX"></a>
#### openssl命令的基本用法
```bash
基本语法：
openssl genrsa [args] [numbits]说明：
args1 对生成的私钥文件是否要使用加密算法进行对称加密: 
    -des : CBC模式的DES加密 
    -des3 : CBC模式的3DES加密 
    -aes128 : CBC模式的AES128加密 
    -aes192 : CBC模式的AES192加密 
    -aes256 : CBC模式的AES256加密 
args2 对称加密密码
    -passout passwords
    其中passwords为对称加密(des、3des、aes)的密码(使用这个参数就省去了console交互提示输入密码的环节) 
args3 输出文件
    -out file : 输出证书私钥文件 
[numbits]: 密钥长度，理解为私钥长度
```
<a name="CkkAN"></a>
#### 使用如下命令生成key秘钥
```bash
openssl genrsa -idea -out jesonc.key 2048
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600605904409-3bf6a2aa-24ef-40ee-9392-3d281c7c7a13.png#height=236&id=LOpI7&originHeight=708&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=834316&status=done&style=none&title=&width=1107.6666666666667)<br />执行以上命令需要输入密钥key的密码，这里使用密码：admin，最终在当前目录生成一个jesonc.key的密钥文件。
<a name="b9ytP"></a>
#### 生成证书签名请求文件（csr文件）
根据密钥文件jesonc.key生成证书签名请求文件jesonc.csr
```bash
openssl req -new -key jesonc.key -out jesonc.csr
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600606094352-a4de7799-9757-469e-8549-417f5c68d2bd.png#height=487&id=l5Id0&originHeight=1462&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1774133&status=done&style=none&title=&width=1107.6666666666667)<br />执行命令的过程中需要根据提示输入一些相关的信息，最终会在当前目录生成jesonc.csr文件。
<a name="dNjCQ"></a>
#### 生成证书签名文件（CA文件）
生成证书签名文件（CA文件），有效期设置为10年，这个有效期根据自己的要求设置。
```bash
openssl x509 -req -days 3650 -in jesonc.csr \
-signkey jesonc.key -out jesonc.crt
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600606828647-f42efdee-5c38-4882-92c0-62eeb0da7614.png#height=237&id=Z9PVz&originHeight=712&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=824323&status=done&style=none&title=&width=1107.6666666666667)
<a name="thWwr"></a>
## nginx配置https
Nginx配置HTTPS的基本语法如下<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600606330978-09e3f51e-5142-4f8e-9770-5e756cb533cd.png#height=583&id=yVgKb&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2043140&status=done&style=none&title=&width=1107.6666666666667)
<a name="tup1M"></a>
### ssl开关
```nginx
配置语法：listen 443 ssl;  # 1.1版本后这样写
```
ssl证书文件
```nginx
配置语法：ssl_certificate file;
默认状态：-
配置方法：http、server
```
ssl密码文件
```nginx
配置语法：ssl_certificate_key file;
默认状态：ssl off;
配置方法：http、server
```
在server模块添加如下配置
```nginx
listen       443 ssl;
server_name  localhost;

ssl_session_cache   shared:SSL:10m;
ssl_session_timeout 10m;
ssl_certificate /usr/local/nginx/conf/httpsKeys/jesonc.crt; # 证书路径
ssl_certificate_key /usr/local/nginx/conf/httpsKeys/jesonc.key; # 请求认证 key 的路径
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600606989889-c5281392-70d2-492e-a11e-b8c7bf3ace8a.png#height=583&id=yDy1J&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2057489&status=done&style=none&title=&width=1107.6666666666667)
<a name="SH1tJ"></a>
## 启动Nginx
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600607069248-cccff4d9-b059-4b28-b820-c9c55231a443.png#height=92&id=XLRLD&originHeight=276&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=281499&status=done&style=none&title=&width=1107.6666666666667)<br />启动Nginx时需要输入密码，这是因为生成证书签名请求文件（csr文件）在 `A challenge password` 时输入了密码。这个密码可以不输入。<br />使用HTTPS访问配置的Nginx的服务器验证是否可以访问，虽然目前自签名的证书已经不能使用；但是nginx的HTTPS大致是这样配置的。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600607211949-cee51cd3-af43-4018-be12-1a2a4d355675.png#height=128&id=AFb3W&originHeight=383&originWidth=1102&originalType=binary&ratio=1&rotation=0&showTitle=false&size=252416&status=done&style=shadow&title=&width=367.3333333333333)

