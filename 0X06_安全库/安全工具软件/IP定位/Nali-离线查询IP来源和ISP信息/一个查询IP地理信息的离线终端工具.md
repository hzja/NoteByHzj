<a name="EF6Y7"></a>
## 功能
- 支持多种数据库
   - 纯真 IPv4 离线数据库
   - ZX IPv6 离线数据库
   - Geoip2 城市数据库 (可选)
   - IPIP 数据库 (可选)
   - ip2region 数据库 (可选)
   - DB-IP 数据库 (可选)
   - IP2Location DB3 LITE 数据库 (可选)
- CDN 服务提供商查询
- 支持管道处理
- 支持交互式查询
- 同时支持IPv4和IPv6
- 支持多语言
- 查询完全离线
- 全平台支持
- 支持彩色输出
<a name="h4yoe"></a>
## 安装
<a name="t0g2b"></a>
### 从源码安装
Nali 需要预先安装 Go >= 1.19. 安装后可以从源码安装软件：
```bash
$ go install github.com/zu1k/nali@latest
```
<a name="teTPY"></a>
### 下载预编译的可执行程序
可以从Release页面下载预编译好的可执行程序：[https://github.com/zu1k/nali/releases](https://github.com/zu1k/nali/releases)<br />需要选择适合系统和硬件架构的版本下载，解压后可直接运行
<a name="KlWu8"></a>
### Arch 系 Linux
在 Aur 中发布了 3 个相关的包：

- nali-go：Release 版本，安装时编译
- nali-go-bin：Release 版本，预编译的二进制文件
- nali-go-git：最新的 master 分支版本，安装时编译
<a name="el5OE"></a>
## 使用说明
<a name="UU9we"></a>
### 查询一个IP的地理信息
```bash
$ nali 1.2.3.4
1.2.3.4 [澳大利亚 APNIC Debogon-prefix网络]
```
<a name="fbIX6"></a>
#### 或者 使用 管道
```bash
$ echo IP 6.6.6.6 | nali
IP 6.6.6.6 [美国 亚利桑那州华楚卡堡市美国国防部网络中心]
```
<a name="v8yDJ"></a>
### 同时查询多个IP的地理信息
```bash
$ nali 1.2.3.4 4.3.2.1 123.23.3.0
1.2.3.4 [澳大利亚 APNIC Debogon-prefix网络]
4.3.2.1 [美国 新泽西州纽瓦克市Level3Communications]
123.23.3.0 [越南 越南邮电集团公司]
```
<a name="rEYQA"></a>
### 交互式查询
使用 exit 或  quit 退出查询
```bash
$ nali
123.23.23.23
123.23.23.23 [越南 越南邮电集团公司]
1.0.0.1
1.0.0.1 [美国 APNIC&CloudFlare公共DNS服务器]
8.8.8.8
8.8.8.8 [美国 加利福尼亚州圣克拉拉县山景市谷歌公司DNS服务器]
quit
```
<a name="t7uP8"></a>
### 与 dig 命令配合使用
需要系统中已经安装好 dig 程序
```bash
$ dig nali.zu1k.com +short | nali
104.28.2.115 [美国 CloudFlare公司CDN节点]
104.28.3.115 [美国 CloudFlare公司CDN节点]
172.67.135.48 [美国 CloudFlare节点]
```
<a name="R12D0"></a>
### 与 nslookup 命令配合使用
需要系统中已经安装好 nslookup 程序
```bash
$ nslookup nali.zu1k.com 8.8.8.8 | nali
Server:         8.8.8.8 [美国 加利福尼亚州圣克拉拉县山景市谷歌公司DNS服务器]
Address:        8.8.8.8 [美国 加利福尼亚州圣克拉拉县山景市谷歌公司DNS服务器]#53
Non-authoritative answer:
Name:   nali.zu1k.com
Address: 104.28.3.115 [美国 CloudFlare公司CDN节点]
Name:   nali.zu1k.com
Address: 104.28.2.115 [美国 CloudFlare公司CDN节点]
Name:   nali.zu1k.com
Address: 172.67.135.48 [美国 CloudFlare节点]
```
<a name="afsfd"></a>
### 与任意程序配合使用
因为 nali 支持管道处理，所以可以和任意程序配合使用
```bash
bash abc.sh | nali
```
Nali 将在 IP后面插入IP地理信息，CDN域名后面插入CDN服务提供商信息
<a name="CtA2a"></a>
### 支持IPv6
和 IPv4 用法完全相同
```bash
$ nslookup google.com | nali
Server:         127.0.0.53 [局域网 IP]
Address:        127.0.0.53 [局域网 IP]#53
Non-authoritative answer:
Name:   google.com
Address: 216.58.211.110 [美国 Google全球边缘网络]
Name:   google.com
Address: 2a00:1450:400e:809::200e [荷兰Amsterdam Google Inc. 服务器网段]
```
<a name="lA2jt"></a>
### 查询 CDN 服务提供商
因为 CDN 服务通常使用 CNAME 的域名解析方式，所以推荐与 nslookup 或者 dig 配合使用，在已经知道 CNAME 后可单独使用
```bash
$ nslookup www.gov.cn | nali
Server:         127.0.0.53 [局域网 IP]
Address:        127.0.0.53 [局域网 IP]#53
Non-authoritative answer:
www.gov.cn      canonical name = www.gov.cn.bsgslb.cn [白山云 CDN].
www.gov.cn.bsgslb.cn [白山云 CDN]       canonical name = zgovweb.v.bsgslb.cn [白山云 CDN].
Name:   zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 103.104.170.25 [新加坡 ]
Name:   zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 2001:428:6402:21b::5 [美国Louisiana州Monroe Qwest Communications Company, LLC (CenturyLink)]
Name:   zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 2001:428:6402:21b::6 [美国Louisiana州Monroe Qwest Communications Company, LLC (CenturyLink)]
```
<a name="UrS2I"></a>
## 用户交互
程序第一次运行后，会在工作目录生成配置文件 config.yaml (默认~/.nali/config.yaml)，配置文件定义了数据库信息，默认用户无需进行修改<br />数据库格式默认如下：
```yaml
- name: geoip
  name-alias:
  - geolite
  - geolite2
  format: mmdb
  file: GeoLite2-City.mmdb
  languages:
  - ALL
  types:
  - IPv4
  - IPv6
```
其中，languages 和 types 表示该数据库支持的语言和查询类型。如果需要增加数据库，需小心修改配置文件，如果有任何问题，欢迎提 issue 询问。
<a name="rUtsu"></a>
### 查看帮助
```bash
$ nali --help
Usage:
  nali [flags]
  nali [command]
Available Commands:
  help        Help about any command
  update      update qqwry, zxipv6wry, ip2region ip database and cdn
Flags:
  -h, --help     help for nali
  -t, --toggle   Help message for toggle
Use "nali [command] --help" for more information about a command.
```
<a name="c03Vu"></a>
### 更新数据库
更新所有可以自动更新的数据库
```bash
$ nali update
2020/07/17 12:53:46 正在下载最新纯真 IP 库...
2020/07/17 12:54:05 已将最新的纯真 IP 库保存到本地 /root/.nali/qqwry.dat
```
或者指定数据库
```bash
$ nali update --db qqwry,cdn
2020/07/17 12:53:46 正在下载最新纯真 IP 库...
2020/07/17 12:54:05 已将最新的纯真 IP 库保存到本地 /root/.nali/qqwry.dat
```
<a name="l2FaO"></a>
### 自选数据库
用户可以指定使用哪个数据库，需要设置环境变量：NALI_DB_IP4、NALI_DB_IP6 或者两个同时设置<br />支持的变量内容：

- Geoip2 ['geoip', 'geoip2']
- Chunzhen ['chunzhen', 'qqwry']
- IPIP ['ipip']
- Ip2Region ['ip2region', 'i2r']
- DBIP ['dbip', 'db-ip']
- IP2Location ['ip2location']
<a name="dfHbW"></a>
#### Windows平台
<a name="XBVu1"></a>
##### 使用geoip数据库
```bash
set NALI_DB_IP4=geoip
或者使用 powershell
$env:NALI_DB_IP4="geoip"
```
<a name="rwTOW"></a>
##### 使用ipip数据库
```bash
set NALI_DB_IP6=ipip
或者使用 powershell
$env:NALI_DB_IP6="ipip"
```
<a name="eDMxV"></a>
#### Linux平台
<a name="jada5"></a>
##### 使用geoip数据库
```bash
export NALI_DB_IP4=geoip
```
<a name="ClsQv"></a>
##### 使用ipip数据库
```bash
export NALI_DB_IP4=ipip
```
<a name="V2yFN"></a>
### 多语言支持
通过修改环境变量 NALI_LANG 来指定使用的语言，当使用非中文语言时仅支持GeoIP2这个数据库<br />该参数可设置的值见 GeoIP2 这个数据库的支持列表
```
# NALI_LANG=en nali 1.1.1.1
1.1.1.1 [Australia]
```
<a name="qF2TR"></a>
### 工作目录
设置环境变量 NALI_HOME 来指定工作目录，配置文件和数据库存放在工作目录下。也可在配置文件中使用绝对路径指定其他数据库路径。<br />设置环境变量 NALI_CONFIG_HOME 来指定配置文件目录，NALI_DB_HOME 来执行数据库文件目录<br />如果未指定相关环境变量，将使用 XDG 规范，配置文件目录在 $XDG_CONFIG_HOME/nali，数据库文件目录在 $XDG_DATA_HOME/nali
```bash
set NALI_HOME=D:\nali
# or
export NALI_HOME=/var/nali
```
<a name="k766y"></a>
### 开源地址
[https://github.com/zu1k/nali](https://github.com/zu1k/nali)
