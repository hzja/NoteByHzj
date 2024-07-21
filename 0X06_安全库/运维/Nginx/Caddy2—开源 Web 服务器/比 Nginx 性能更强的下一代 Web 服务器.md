<a name="HBVPw"></a>
## 简介
Caddy 2 是一个强大的、企业级的、开源的 Web 服务器。是一个 Go 编写的 Web 服务器，类似于 Nginx，Caddy 提供了更加强大的功能。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1668731300031-662abf9a-62b9-4b74-9cce-de4f8f667b97.png#averageHue=%23f0f7f9&clientId=u9599d605-9b24-4&from=paste&height=713&id=u8ade6744&originHeight=1783&originWidth=3794&originalType=binary&ratio=1&rotation=0&showTitle=false&size=578922&status=done&style=none&taskId=u4086c100-fb56-4625-9031-2a9616d2a43&title=&width=1517.6)<br />支持 HTTP/2 的 Web 服务端。它使用 Golang 标准库提供 HTTP 功能。Caddy 一个显著的特性是默认启用HTTPS。它是第一个无需额外配置即可提供HTTPS 特性的Web 服务器。

- 官网：[https://caddyserver.com](https://caddyserver.com)
- 文档：[https://caddyserver.com/docs/](https://caddyserver.com/docs/)
<a name="LRlbq"></a>
## 特性

- 全自动支持 HTTP/2 协议，无需配置。
- 全自动变成 HTTPS 站点，无需任何配置。
- 合理使用多核
- 支持 IPv6 环境
- 对 WebSockets 有很好的支持
- 可以自动把 Markdown 转成 HTML
- Caddy 对 log 格式的定义很容易
- 易于部署 ,没有依赖
- 作为反向代理支持主动和被动健康检查、负载均衡、断路、缓存等功能。
- 支持 Windows、 Linux、Mac 三大主流系统
<a name="w7LbM"></a>
### 相较于 Nginx 来说使用 Caddy 的优势如下

- 自动的 HTTPS 证书申请(ACME HTTP/DNS 挑战)
- 自动证书续期以及 OCSP stapling 等
- 更高的安全性包括但不限于 TLS 配置以及内存安全等
- 友好且强大的配置文件支持
- 支持 API 动态调整配置(有木有人可以搞个 Dashboard)
- 支持 HTTP3(QUIC)
- 支持动态后端，例如连接 Consul、作为 k8s ingress 等
- 后端多种负载策略以及健康检测等
- 本身 Go 编写，高度模块化的系统方便扩展(CoreDNS 基于 Caddy1 开发)
<a name="SEB6X"></a>
## 安装
下载地址：[https://caddyserver.com/download](https://caddyserver.com/download)
<a name="lFTrV"></a>
### Fedora/RHEL/CentOS 8
```bash
$ dnf install 'dnf-command(copr)'
$ dnf copr enable @caddy/caddy
$ dnf install caddy
```
<a name="Et3lm"></a>
### MacOS
```bash
brew install caddy
```
<a name="Rkmua"></a>
### RHEL/CentOS 7
```bash
$ yum install yum-plugin-copr
$ yum copr enable @caddy/caddy
$ yum install caddy
```
<a name="s2keX"></a>
### Debian/Ubuntu/Raspbian
```bash
$ sudo apt install -y debian-keyring debian-archive-keyring apt-transport-https
$ curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/gpg.key' | sudo tee /etc/apt/trusted.gpg.d/caddy-stable.asc
$ curl -1sLf 'https://dl.cloudsmith.io/public/caddy/stable/debian.deb.txt' | sudo tee /etc/apt/sources.list.d/caddy-stable.list
$ sudo apt update
$ sudo apt install caddy
```
安装完成后，在终端命令行下输入 caddy ：
```bash
$ caddy

Caddy is an extensible server platform.

usage:
  caddy <command> [<args...>]

commands:
  adapt           Adapts a configuration to Caddy's native JSON
  build-info      Prints information about this build
  environ         Prints the environment
  file-server     Spins up a production-ready file server
  fmt             Formats a Caddyfile
  hash-password   Hashes a password and writes base64
  help            Shows help for a Caddy subcommand
  list-modules    Lists the installed Caddy modules
  reload          Changes the config of the running Caddy instance
  reverse-proxy   A quick and production-ready reverse proxy
  run             Starts the Caddy process and blocks indefinitely
  start           Starts the Caddy process in the background and then returns
  stop            Gracefully stops a started Caddy process
  trust           Installs a CA certificate into local trust stores
  untrust         Untrusts a locally-trusted CA certificate
  upgrade         Upgrade Caddy (EXPERIMENTAL)
  validate        Tests whether a configuration file is valid
  version         Prints the version

Use 'caddy help <command>' for more information about a command.

Full documentation is available at:
```
<a name="NXW1w"></a>
### 常用命令
```bash
$ systemctl start[stop|restart] caddy  //启动服务
$ systemctl reload caddy //重新加载配置文件
$ systemctl status caddy //查看运行状态
```
<a name="TduFn"></a>
## 配置
Caddy2 的配置文件核心采用 json，但是 json 可读性不强，所以官方维护了一个转换器，抽象出称之为 Caddyfile 的新配置格式；关于 Caddyfile 的完整语法请查看官方文档。文档地址：[https://caddyserver.com/docs/caddyfile](https://caddyserver.com/docs/caddyfile)
<a name="YKcKu"></a>
### 常用配置
<a name="qOCJb"></a>
#### 站点配置
```
(site_option) {
  encode zstd gzip
  file_server
  handle_errors {
    rewrite * /{http.error.status_code}.html
    file_server
  }
  import acme_https
  import log_file
  root * /www/{host}
}
```
<a name="mdtZW"></a>
#### 多域名配置
```
地址 {
    ........
}
```
这个地址支持以下几种格式
```
localhost
example.com
:443
http://example.com
localhost:8080
127.0.0.1
[::1]:2015
example.com/foo/*
*.example.com
http://
```
<a name="UH2iG"></a>
#### 全局配置选项
```
{
    # 开启所有的调试输出
    debug    
    # 设定默认的sni
    default_sni domain.com
    # 打开或关闭管理端口
    admin off
}
```
<a name="Oyl8Z"></a>
#### 错误页面跳转
```
www.mingongge.com {
    root * /web/mingongge.com/
    file_server
    handle_errors {
        rewrite * /{http.error.status_code}.html
        file_server
    }
}
```
<a name="UVp4X"></a>
#### Log 日志
```
(log_file) {
  log {
    format logfmt
    output file /var/log/caddy/{host}.access.log {
      roll_keep 7
    }
  }
}
```
<a name="S0pu3"></a>
#### Header
```
www.mingongge.com {
  root * /web/mingongge.com/
  file_server
  header Access-Control-Allow-Origin *
  header Cache-Control max-age=3600
  header /css/* Cache-Control max-age=604800
}
```
<a name="Ht0vs"></a>
#### Cache-Control
```
(cachecontrol) {
  header /css/* Cache-Control max-age=3600
  header /img/* Cache-Control max-age=3600
  header /js/* Cache-Control max-age=3600
}
```
<a name="KtFd1"></a>
### 反向代理
```
https://www.mingongge.com {
gzip
tls admin@mingongge.com
proxy / https://welcome.mingongge.com
}
```
<a name="IkV9K"></a>
### 快速创建一个站点
```bash
caddy file-server --website ./index --listen :8088 --domain www.mingongge.com
```
参数说明
```
file-server #这是一个文件服务器
--website   #文件服务的根目录
./index     #首页文件的存储目录（如存放 index.html）
--listen :8088  #定义监听端口
--domain www.mingongge.com  #绑定域名
```
<a name="Wb8Yz"></a>
### 配置片段
Caddyfile 支持类似代码中 function 一样的配置片段，这些配置片段可以在任意位置被 `import`，同时可以接受参数，以下为配置片断示例:
```
# 括号内为片段名称，可以自行定义
(TLS) {
    protocols tls1.2 tls1.3
    ciphers TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
}

# 在任意位置可以引用此片段从而达到配置复用
import TLS
```
<a name="VRadG"></a>
### 配置模块化
import 指令除了支持引用配置片段以外，还支持引用外部文件，同时支持通配符，有了这个命令以后就可以方便的将配置文件进行模块化处理:
```
# 引用外部的 /etc/caddy/*.caddy
import /etc/caddy/*.caddy
```
<a name="CHSYi"></a>
## 模块化配置实例
<a name="BDSmI"></a>
### Caddyfile
Caddyfile 类似于 nginx 的 nginx.conf 主配置。
```
(LOG) {
    log {       
        format formatted "[{ts}] {request>remote_addr} {request>proto} {request>method} <- {status} -> {request>host} {request>uri} {request>headers>User-Agent>[0]}"  {
            time_format "iso8601"
        }
        output file "{args.0}" {
            roll_size 100mb
            roll_keep 3
            roll_keep_for 7d
        }
    }
}

(TLS) {
    protocols tls1.2 tls1.3
    ciphers TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256 TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384 TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256
}

(HSTS) {
    header / Strict-Transport-Security "max-age=63072000"
}

(ACME_GANDI) {
    # 从环境变量获取 GANDI_API_TOKEN
    dns gandi {$GANDI_API_TOKEN}
}

# 聚合上面的配置片段为新的片段
(COMMON_CONFIG) {
    # 压缩支持
    encode zstd gzip

    # TLS 配置
    tls {
        import TLS
        import ACME_GANDI
    }

    # HSTS
    import HSTS
}

# 开启 HTTP3 实验性支持
{
    servers :443 {
        protocol {
            experimental_http3
        }
    }
}

# 引入其他具体的站点配置
import /etc/caddy/*.caddy
```
<a name="PZDoK"></a>
### 站点配置
```
www.mingongge.com {
    # 重定向到 mingongge.com(默认 302)
    redir https://mingongge.com{uri}

    # 日志
    import LOG "/data/logs/mingongge.com.log"

    # TLS、HSTS、ACME 等通用配置
    import COMMON_CONFIG
}

mingongge.com {
    # 路由
    route /* {
        reverse_proxy mingongge_com:80
    }

    # 日志
    import LOG "/data/logs/mingongge.com.log"

    # TLS、HSTS、ACME 等通用配置
    import COMMON_CONFIG
}
```
配置完成后，通过 `systemctl start caddy` 去启动 caddy 服务器。每次配置修改后可以通过 `systemctl reload caddy` 进行配置重载。这些命令的功能与 Nginx 的命令功能基本一样。
<a name="a0JGL"></a>
## 总结
Caddy 内置了丰富的插件，例如 “file_server”、内置各种负载均衡策略等，这些插件组合在一起可以实现一些复杂的功能。Caddy 采用 Go 语言编写，相比较而言（Nginx 通过 Lua 或者 C 开发插件），对于后期的开发更容易上手。
