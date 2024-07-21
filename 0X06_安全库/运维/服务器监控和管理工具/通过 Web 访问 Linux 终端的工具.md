rtty 由客户端和服务端组成。客户端采用纯C实现，服务端采用 GO 语言实现，前端界面采用 vue 实现。使用 rtty 可以在任何地方通过 Web 访问设备的终端，通过 设备ID 来区分不同的设备。rtty 非常适合远程维护 Linux设备。
<a name="t9wk0"></a>
## 特性

- 客户端 C 语言实现，非常小，适合嵌入式 Linux
   - **不支持 SSL**：rtty(32K) + libev(56K)
   - **支持 SSL**：+ libmbedtls(88K) + libmbedcrypto(241K) + libmbedx509(48k)
- 远程批量执行命令
- 支持SSL: openssl、mbedtls、CyaSSl(wolfssl)
- SSL 双向认证(mTLS)
- 非常方便的上传和下载文件
- 根据 设备ID 访问不同的设备
- 支持 HTTP 代理 访问设备的 Web
- 基于 Xterm.js 的全功能终端
- 部署简单，使用方便
<a name="ZlfzF"></a>
## 演示
![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1689003557880-176bc1a1-e034-482a-bdfa-09744b63caaf.gif#averageHue=%23dfd7cc&clientId=u5f1e3720-dae0-4&from=paste&id=u416666ab&originHeight=666&originWidth=1078&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6a58f246-e736-437d-b98a-0c04d79d749&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1689003557904-30c386b9-c44d-4022-a3c6-825d99347661.gif#averageHue=%23070707&clientId=u5f1e3720-dae0-4&from=paste&id=ubee538fc&originHeight=666&originWidth=1078&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u57c0c1d7-a3ac-4618-bba0-65502ae27be&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1689003557817-03690619-ca5a-4e6a-9c96-8746264abd8f.gif#averageHue=%237d7d7d&clientId=u5f1e3720-dae0-4&from=paste&id=u7ee13403&originHeight=666&originWidth=1078&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ude679583-0105-44b7-bcd0-f595c48ae8c&title=)
<a name="l6HpX"></a>
## 部署服务端
<a name="dEyED"></a>
### 安装依赖
```bash
sudo apt install -y libev-dev libssl-dev      # Ubuntu, Debian
sudo pacman -S --noconfirm libev openssl      # ArchLinux
sudo yum install -y libev-devel openssl-devel # Centos
```
<a name="lr3qC"></a>
### 克隆 rtty 代码
```bash
git clone --recursive https://github.com/zhaojh329/rtty.git
```
<a name="GuTFN"></a>
### 编译
```bash
cd rtty && mkdir build && cd build
cmake .. && make install
```
<a name="Wnc7V"></a>
### 将下面的参数替换为自己的参数
```bash
sudo rtty -I 'My-device-ID' -h 'your-server' -p 5912 -a -v -d 'My Device Description'
```
<a name="NXTGJ"></a>
### 生成一个 token
```bash
$ rttys token
Please set a password:******
Your token is: 34762d07637276694b938d23f10d7164
```
<a name="HeMox"></a>
### 使用 token
```bash
$rttys -t 34762d07637276694b938d23f10d7164
```
<a name="X4sFb"></a>
## 通过浏览器访问
使用 Web 浏览器访问服务器：http://your-server-host:5913，然后点击连接按钮。<br />或者直接连接设备，无需 Web 登录(需要在服务端配置设备白名单)
```
http://your-server-host:5913/connect/devid1

http://your-server-host:5913/connect/devid2
```
<a name="blHJo"></a>
### 从本地传输文件到远程设备
```bash
rtty -R
```
<a name="vFx1b"></a>
### 从远程设备传输文件到本地
```bash
rtty -S test.txt
```
<a name="K2hIm"></a>
## 传送门
开源协议：MIT<br />开源地址：[https://github.com/zhaojh329/rtty](https://github.com/zhaojh329/rtty)
