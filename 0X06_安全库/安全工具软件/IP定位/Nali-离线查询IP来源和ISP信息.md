IP定位
<a name="5lCWB"></a>
## Nali介绍
Nali 是一个可以自动解析这些工具stdout中的IP并附上所属ISP和地理信息。<br />Nali取自中文「哪里」。Nali 一开始是数年前由surfchen用C和Perl写成的小工具（C 用于解析纯真 IP 数据库，Perl 用于解析 stdout），查询是在本地进行，并不会进行联网查询，所以效率方面不会有什么影响。<br />Nali CLI采用Nodejs开发，在安装之前，需要具备Nodejs环境。<br />**项目地址：**

- GitHub: [https://github.com/sukkaw/nali-cli](https://github.com/sukkaw/nali-cli)
- 官网：[https://nali.skk.moe/](https://nali.skk.moe/)
<a name="ghyUq"></a>
## Nali安装
准备好Nodejs环境之后，只需要执行下面的命令即可成功安装。
```bash
# NPM
$ npm i nali-cli -g --registry=http://registry.npm.taobao.org
# Yarn
$ yarn global add nali-cli
```
Nali CLI也有macOS x64和Linux x64编译好的二进制文件提供下载，可以在GitHub Repo中的bin目录中下载。

- nali-cli/blob/master/bin/nali-cli-linux
- nali-cli/blob/master/bin/nali-cli-macos
:::danger
注意：首次运行Nali CLI 时会自动下载所需的qqwry.dat到$HOME/.config/nali-cli/目录下。
:::
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600440403684-573f508e-f332-4b4d-9490-554b4acb623f.png#align=left&display=inline&height=110&originHeight=330&originWidth=3323&size=329332&status=done&style=none&width=1107.6666666666667)
<a name="VkVPS"></a>
## Nali使用
<a name="vRnej"></a>
### 1、帮助信息
```bash
$ nali --help
Usage: nali <command> [options]
Options:
  -v, --version  版本信息
  -h, --help     output usage information
Commands:
  parse          解析 stdin 或参数中的 IP 信息 (默认)
  update         更新 IP 库
  help [cmd]     display help for [cmd]
```
<a name="QX6Zm"></a>
### 2、自带工具命令

- nali-dig
- nali-nslookup
- nali-ping
- nali-tracepath
- nali-traceroute

Nali required related software installed. For example, in order to use nali-dig and nali-nslookup you need to have bind (dnsutils) installed.意思是需要安装应用程序才能使用。
<a name="EVcN2"></a>
### 3、使用举例
查询一个IP地址
```bash
$ nali 1.145.1.4
1.145.1.4 [澳大利亚 墨尔本 Telstra]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600440424697-7191557c-f144-4ce1-a0a9-63f845a63ebb.png#align=left&display=inline&height=144&originHeight=432&originWidth=3323&size=479837&status=done&style=none&width=1107.6666666666667)<br />查询一组IP地址
```bash
$ nali 114.5.1.4 191.919.8.10 1.0.0.1
114.5.1.4 [印度尼西亚] 191.919.8.10 1.0.0.1 [美国 APNIC&CloudFlare 公共 DNS 服务器]
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600440440017-6cf6904c-a28d-497f-aaab-c7cdd86e1b04.png#align=left&display=inline&height=180&originHeight=540&originWidth=3323&size=626046&status=done&style=none&width=1107.6666666666667)<br />Nali CLI 还支持从外部的stdin中解析和查询IP信息:
```bash
$ dig blog.skk.moe +short | nali
104.18.101.28 [美国 CloudFlare 公司 CDN 节点]
104.18.100.28 [美国 CloudFlare 公司 CDN 节点]
```
```bash
$ nslookup www.gov.cn 1.0.0.1 | nali
Server:        1.0.0.1 [美国 APNIC&CloudFlare 公共 DNS 服务器]
Address:    1.0.0.1 [美国 APNIC&CloudFlare 公共 DNS 服务器]#53
Non-authoritative answer:
www.gov.cn    canonical name = www.gov.cn.bsgslb.cn. [白山云 CDN]
www.gov.cn.bsgslb.cn [白山云 CDN]    canonical name = zgovweb.v.bsgslb.cn. [白山云 CDN]
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 107.155.25.117 [香港 Zenlayer]
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 107.155.25.118 [香港 Zenlayer]
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 107.155.25.116 [香港 Zenlayer]
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 107.155.25.120 [香港 Zenlayer]
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 2001:438:fffd:98::4
Name:    zgovweb.v.bsgslb.cn [白山云 CDN]
Address: 2001:438:fffd:98::5
```
使用自带的内置命令
```bash
$ nali-nslookup blog.skk.moe
Server:         1.0.0.1 [美国 APNIC&CloudFlare 公共 DNS 服务器]
Address:        1.0.0.1 [美国 APNIC&CloudFlare 公共 DNS 服务器]#53
Non-authoritative answer:
Name:   blog.skk.moe
Address: 104.18.101.28 [美国 CloudFlare 公司 CDN 节点]
Name:   blog.skk.moe
Address: 104.18.100.28 [美国 CloudFlare 公司 CDN 节点]
Name:   blog.skk.moe
Address: 2606:4700::6812:641c
Name:   blog.skk.moe
Address: 2606:4700::6812:651c
```
```bash
$ nali-traceroute 117.8.222.169
traceroute to 117.8.222.169[天津市 联通] (117.8.222.169[天津市 联通]), 30 hops max, 60 byte packets
 1  dns1.online.tj.cn (111.161.19.1[天津市 联通])  6.298 ms  6.767 ms  7.192 ms
 2  no-data (125.39.237.237[天津市 深圳市腾讯计算机系统有限公司IDC机房(联通)])  5.129 ms  5.509 ms  5.907 ms
 3  dns217.online.tj.cn (111.161.29.217[天津市 联通])  0.573 ms no-data (125.39.237.225[天津市 深圳市腾讯计算机系统有限公司IDC机房(联通)])  3.843 ms  3.148 ms
 4  dns169.online.tj.cn (117.8.222.169[天津市 联通])  10.547 ms  10.718 ms  10.771 ms
```
更新IP数据库（qqwry.dat）
```bash
nali update
```
