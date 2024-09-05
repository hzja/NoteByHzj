autossh<br />比如在 host2 机器上面部署了很多服务，但是由于某种原因导致无法直接 ssh 访问该服务器已经其上服务的对应端口，但是 host3 服务器，既能够访问 host2 服务器，又可以访问 host1 服务器。这时，可以通过设置 ssh 来进行服务的访问。但是还有两个问题：需要保持终端开启防止 SSH 进程被关闭；由于网络故障/波动导致 SSH 终断时无法自动重连。<br />前者可以使用 tmux 等第三方工具来解决，后者需要 supervisor 等第三方监控工具，不过还有一个更常用的选择方案，那就是使用 autossh 工具。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619139423648-9e54e088-dd27-4be9-a53a-b2bb96d8e9e0.png#align=left&display=inline&height=723&id=u4d4d511f&originHeight=723&originWidth=1080&status=done&style=shadow&width=1080)<br />用autossh工具进行内网穿透
<a name="yHjiy"></a>
### 1、工具介绍
该工具的起初灵感和机制来自于 rstunnel。<br />autossh 工具是一个用来启动 ssh 服务并进行监控的命令行应用程序，可以在程序问题或者是网络问题的时候，重启 ssh 服务。

- 在 autossh 1.2 的时候，其方法已经改变了：autossh 使用 ssh 来构造一个 ssh 重定向循环(本地到远程和远程到本地)，然后发送测试数据并获得返回结果。
- 在 autossh 1.3 增加了一个新的方法：可指定远程 echo 服务的端口用于返回测试数据发送结果。这个避免握手阶段以及所有远程机器端口的冲突问题，而老的 loop-of-forwardings 方法依然可用。

内网主机主动连接到外网主机，又被称作反向连接(Reverse Connection)，这样 NAT 路由/防火墙就会在内网主机和外网主机之间建立映射即可相互通信了。但这种映射是路由网关自动维持的，不会持续下去，如果连接断开或者网络不稳定都会导致通信失败，这时内网主机需要自动重连机制了。
```bash
# 安装方式
$ yum install autossh
$ apt install autossh
```
<a name="GPXeX"></a>
### 2、使用方式
autossh 使用了系统原生的 SSH 端口映射功能，性能开销非常小。
<a name="WG9Y4"></a>
#### 命令使用方式
```bash
# autossh usage
autossh [-V] [-M port[:echo_port]] [-f] [SSH_OPTIONS]
```
<a name="r8sC1"></a>
#### 命令使用参数
| 编号 | 参数 | 含义说明 |
| --- | --- | --- |
| 1 | -M | 用于有问题时就会自动重连；服务器 echo 机制使用的端口 |
| 2 | -D | 本地机器动态的应用程序端口转发 |
| 3 | -R | 将远程主机(服务器)的某个端口转发到本地端指定机器的指定端口 |
| 4 | -L | 将本地机(客户机)的某个端口转发到远端指定机器的指定端口 |
| 5 | -f | 后台运行 |
| 6 | -T | 不占用 shell |
| 7 | -n | 配合 -f 参数使用 |
| 8 | -N | 不执行远程命令 |
| 9 | -q | 安静模式运行；忽略提示和错误 |

<a name="ZYx7x"></a>
#### 命令使用演示
```bash
# 本地端口绑定(在host1服务器上面运行)
# 将所有发送到本机的8527端口的所有数据转发到远程主机的8000端口
$ ssh -vv -N -D localhost:8527 [email protected] -p 8000

# 主要是为了更新安全的运行ssh服务
$ autossh -M 5678 -vv -N -D localhost:8527 [email protected] -p 8000
```
<a name="suygo"></a>
### 3. 示例演示
SSH 仅支持 TCP 端口映射!<br />如果需要映射的端口不多，只有几个的话，autossh 确实是最佳选择。但是如果需要映射大量端口，建议还是使用类似 Ngrok 的端口映射工具，毕竟这类工具拥有比较完善的管理功能。
<a name="ruCyd"></a>
#### [1] 本地端口绑定和转发 (-L) => 在 host1 上面设置
```bash
# 将在host1主机上开启一个本地侦听的5900端口
# 这样之后，访问本地5900端口的流量将转发至host2的8000端口
# 其中-M参数负责通过5678端口监视连接状态，连接有问题时就会自动重连
$ autossh -M 5678 -fCN -L 5900:localhost:8000 [email protected]
$ autossh -M 5678 -fCN -L 5900:[email protected]:8000 [email protected]
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619139421474-17d598de-2205-42b8-81a3-e52137a44841.png#align=left&display=inline&height=498&id=u2e8e2b0d&originHeight=498&originWidth=1050&status=done&style=shadow&width=1050)<br />用autossh工具进行内网穿透
<a name="LAMcK"></a>
#### [2] 远程端口转发功能 (-R) => 在 host3 上面设置
```bash
# 将在host1主机上开启一个本地侦听的5900端口
# 这样之后，访问本地5900端口的流量将转发至host2的8080端口
# 其中-M参数负责通过5678端口监视连接状态，连接有问题时就会自动重连
$ autossh -M 5678 -fCN -R 5900:localhost:8000 [email protected]
$ autossh -M 5678 -fCN -R 5900:[email protected]:8000 [email protected]
```
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1619139420939-efad65bb-45c9-49bc-9b67-a3ef694b248f.png#align=left&display=inline&height=493&id=u039b7841&originHeight=493&originWidth=1041&status=done&style=shadow&width=1041)<br />用autossh工具进行内网穿透
<a name="YSc22"></a>
#### [3] 动态端口转发功能 (-D) => 在 host1 上面设置
```bash
# 动态端口转发功能
$ autossh -M 5678 -vv -D 1080 [email protected]
```
<a name="oAcfR"></a>
### 4、开机自启动
系统开机，服务自启动！<br />在 Ubuntu 或 CentOS 系统中，使用 `systemd` 来管理 autossh 的开机启动问题。配置很简单，只需要创建一个如下服务启动配置文件，即可。旧版本中可以使用 init.d 来管理方式，只需要添加对应命令到 rc.local 文件，即可。
```bash
# Ubuntu
# 配置文件地址
# /etc/systemd/system/remote-autossh.service
[Unit]
Description=AutoSSH service for remote tunnel
After=network-online.target

[Service]
User=root
ExecStart=/usr/bin/autossh -M 5678 -fCNR 18081:host2:8080 [email protected]

[Install]
WantedBy=multi-user.target

# CentOS
# 配置文件地址
# /etc/rc.d/rc.local
$ /usr/bin/autossh -M 5678 -fCNR 18081:host2:8080 [email protected]
```
<a name="upeUH"></a>
### 5、编写启停脚本
可以随时随地的启动、停止到服务端口映射！
```bash
PASS="escapelife"

doexit(){
    expect -c "
    set timeout -1
    spawn $1 -t ps aux |grep escape |grep sshd |awk '{print $2}' |xargs kill -9
    expect {
        \"*?assword:*\" {
            send \"$PASS\r\"
        }
    }
    expect eof
    "
}

dossh(){
    nohup expect -c "
    set timeout -1
    spawn $1
    expect {
        \"*?assword:*\" {
            send \"$PASS\r\";
            exp_continue
        }
    }
    " &
}

# 先关掉每台服务器的sshd连接
doexit "ssh -o -p 6622 [email protected]"

# 启动ssh端口映射
dossh "autossh -o -M 5678 -N -L 5900:127.0.0.1:8000 [email protected] -p 6622"
```
