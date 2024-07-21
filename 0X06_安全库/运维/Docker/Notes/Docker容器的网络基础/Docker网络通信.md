Docker
<a name="gy52L"></a>
# 网络
部署的容器中很多应用都是需要让外部通过网络端口来进行访问的，比如比如mysql的3306端口，mongodb的27017端口和redis的6379端口等等；不仅是外部访问，不同的容器之间可能还需要进行通信，比如web应用容器需要来连接mysql或者mongodb容器，都涉及到了网络通信。
<a name="CBnlb"></a>
## 端口映射
容器要想让外部访问应用，可以通过`-P`或者`-p`参数来指定需要对外暴露的端口：
```bash
$ docker run -d -P nginx
9226326c42067d282f80dbc18a8a36bf54335b61a84b191a29a5f59d25c9fbc3
```
使用`-P`会在主机绑定一个随机端口，映射到容器内部的端口；查看刚刚创建的容器，可以看到随机端口49154映射到了容器内部的80端口：
```bash
$ docker ps -l
CONTAINER ID   IMAGE    CREATED              STATUS              PORTS                                    
9226326c4206   nginx    About a minute ago   Up About a minute   0.0.0.0:49154->80/tcp, :::49154->80/tcp
```
使用logs命令可以看到nginx的访问日志：
```bash
$ docker logs 9226326c4206
10.197.92.41 - - [16/Mar/2022:01:40:32 +0000] "GET / HTTP/1.1" 304 0 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36" "-"
```
`docker port`可以快捷地查看容器端口的绑定情况：
```bash
$ docker port 9226326c4206
80/tcp -> 0.0.0.0:49154
```
使用`-p`参数可以指定一个端口进行映射：
```bash
$ docker run -d -p 3000:80 nginx
```
也可以使用`ip:hostPort:containerPort`格式指定映射一个特定的ip：
```bash
$ docker run -d -p 127.0.0.1:3000:80 nginx
```
省略hostPort参数本地主机会自动分配一个端口，类似`-P`参数的作用：
```bash
$ docker run -d -p 127.0.0.1::80 nginx
```
还可以使用udp来指定映射到udp端口：
```bash
$ docker run -d -p 3000:80/udp nginx
```
有时候想要映射容器的多个端口，可以使用多个`-p`参数：
```bash
$ docker run -d 
            -p 8000:8000 \
            -p 8010:8010\
            nginx
```
或者映射某个范围内的端口列表：
```bash
$ docker run -d -p 8080-8090:8080-8090 nginx
```
<a name="fYktb"></a>
## docker网络模式
想要将多个容器进行互联互通，为了避免不同容器之间互相干扰，可以给多个容器建立不同的局域网，让局域网络内的网络彼此联通。<br />要理解docker的网络模式，首先来看下docker有哪些网络；安装docker后，它会自动创建三个网络none、host和brdge，使用`network ls`命令查看：
```bash
$ docker network ls
NETWORK ID     NAME      DRIVER    SCOPE
c64d7d519c22   bridge    bridge    local
6306a0b1d150   host      host      local
d058571d4197   none      null      local
```
分别来看下这几个网络的用途；首先是none，none顾名思义，就是什么都没有，该网络关闭了容器的网络功能，使用`--network=none`指定使用none网络：
```bash
$ docker run -itd --name=busybox-none --network=none busybox
49f88dd75ae774bea817b27c647506eda5ad581403bfbad0877e8333376ae3b0

docker exec 49f88dd75ae7  ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: tunl0@NONE: <NOARP> mtu 1480 qdisc noop qlen 1000
    link/ipip 0.0.0.0 brd 0.0.0.0
3: ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop qlen 1000
    link/tunnel6 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00 brd 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00
```
这里使用了busybox镜像，可能有的童鞋对它不了解，这是一个集成压缩了三百多个常用linux命令和工具的软件，它被称为Linux工具里的瑞士军刀，这里主要用它的ip命令查看容器的网络详细情况。<br />可以看到这个容器除了lo本地环回网卡，没有其他的网卡信息；不仅不能接收信息，也不能对外发送信息，用ping命令测试网络情况：
```bash
$ docker exec 49f88dd75ae7 ping xieyufei.com
ping: bad address 'xieyufei.com'
```
这个网络相当于一个封闭的孤岛，那我们不禁会想，这样“自闭”的网络有什么用呢？<br />封闭意味着隔离，一些对安全性要求高并且不需要联网的应用可以使用none网络。比如某个容器的唯一用途是生成随机密码，就可以放到none网络中避免密码被窃取。<br />其次是bridge网络模式，docker安装时会在宿主机上虚拟一个名为docker0的网桥，如果不指定`--network`，创建的容器默认都会挂载到docker0上，通过命令查看宿主机下所有的网桥：
```bash
$ brctl show
bridge name     bridge id               STP enabled     interfaces
docker0         8000.02426b8674a4       no
```
这里的网桥可以把它理解为一个路由器，它把两个相似的网络连接起来，并对网络中的数据进行管理，同时也隔离外界对网桥内部的访问；同一个网桥下的容器之间可以相互通信；还是通过busybox查看容器的网络情况
```bash
$ docker run -itd --name=busybox-bridge --network=bridge busybox
f45e26e5bb6f94f50061f22937abb132fb9de968fdd59fe7ad524bd81eb2f1b0

$ docker exec f45e26e5bb6f ip a
181: eth0@if182: <BROADCAST,MULTICAST,UP,LOWER_UP,M-DOWN> mtu 1500 qdisc noqueue 
    link/ether 02:42:ac:11:00:06 brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.6/16 brd 172.17.255.255 scope global eth0
       valid_lft forever preferred_lft forever
```
可以看到这里容器多了一个eth0的网卡，它的ip是172.17.0.6。<br />最后是host网络模式，这种模式禁用了Docker的网络隔离，容器共享了宿主机的网络，还是通过busybox来查看容器的网络情况：
```bash
$ docker run -itd --name=busybox-host --network=host busybox
2d1f6d7a01f1afe1e725cf53423de1d79d261a3b775f6f97f9e2a62de8f6bb74

$ docker exec 2d1f6d7a01f1 ip a
2: enp4s0f2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast qlen 1000
    link/ether 74:d0:2b:ec:96:8a brd ff:ff:ff:ff:ff:ff
    inet 192.168.0.100/24 brd 192.168.0.255 scope global dynamic noprefixroute enp4s0f2
       valid_lft 37533sec preferred_lft 37533sec
```
可以发现这里容器的ip地址是192.168.0.100，和宿主机的ip地址是一样的；host模式其实类似于Vmware的桥接模式，容器没有独立的ip、端口，而是使用宿主机的ip、端口。<br />需要注意的是host模式下，不需要添加`-p`参数，因为它使用的就是主机的IP和端口，添加`-p`参数后，反而会出现以下警告：
```
WARNING: Published ports are discarded when using host network mode
```
host模式由于和宿主机共享网络，因此它的网络模型是最简单最低延迟的模式，容器进程直接与主机网络接口通信，与物理机性能一致。不过host模式也不利于网络自定义配置和管理，所有容器使用相同的ip，不利于主机资源的利用，一些对网络性能要求比较高的容器，可以使用该模式。
<a name="peGr6"></a>
## 容器互联
通过容器互联来测试两个容器在同一个网桥下面是如何进行连接互通的；首先自定义一个网桥：
```bash
$ docker network create -d bridge my-net
```
如果对网桥不满意，可以通过rm命令删除它：
```bash
$ docker network rm my-net
```
新建两个容器，并且把它们连接到my-net的网络中：
```bash
$ docker run -itd --name busybox1 --network my-net busybox
$ docker run -itd --name busybox2 --network my-net busybox
```
让两个容器之间互相ping，发现他们之间能够ping通：
```bash
$ docker exec  busybox1 ping busybox2
PING busybox2 (172.23.0.3): 56 data bytes
64 bytes from 172.23.0.3: seq=0 ttl=64 time=0.139 ms
64 bytes from 172.23.0.3: seq=1 ttl=64 time=0.215 ms

$ docker exec  busybox2 ping busybox1
PING busybox1 (172.23.0.2): 56 data bytes
64 bytes from 172.23.0.2: seq=0 ttl=64 time=0.090 ms
64 bytes from 172.23.0.2: seq=1 ttl=64 time=0.224 ms
```
