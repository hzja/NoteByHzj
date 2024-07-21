Docker
<a name="zAriG"></a>
## 1、前置知识
因为交换机的能力有限制，以及网线的连接不可能无限长， 所以不可能把所有的主机都连到同一个交换机上，然后处于同一个二层网络中。<br />就算能，主机间的 ARP 广播也会让这个网络瞬间瘫痪。<br />所以得把主机拆分到一个个的小的子网里，然后通过路由器再并成三层网络。<br />俗称的 IP 地址其实由两个部分组成，网络地址和主机地址。比如 10.0.0.1/8 中，第一个 10 是网络地址，后面的 0.0.1 是主机地址。那怎么知道怎么去拆分这两段呢？靠的就是子网掩码，也就是那个 /8。<br />IP 其实是由 32 位的二进制组成的，x.x.x.x 只是为了方便人类阅读将其转成了十进制。有个简单粗暴的办法就是可以认为每一段都是 8 位，所以 /8 就代表第一段是网络地址。<br />同理，/16 就代表前两段都是网络地址，10.0.0.0/16 中， 网络地址是 10.0，后两位 0.0 代表每个子网中的主机地址。<br />比如 10.0.0.1/16 和 10.0.0.2/16 是同一个子网的两个主机。10.0.0.1/16 和 10.1.0.1/16 是不同的子网的两个主机。<br />这里只是简单的介绍下解决方案。
<a name="ldy9D"></a>
## 2、问题表现

- docker 进程无法启动
- 容器端口无法访问，抓包显示为有入站但是没有出站
<a name="kn3Y0"></a>
## 3、排查
此处针对的是 dockerd 无法启动的情况，如果 dockerd 能启动，可以直接跳到解决一节<br />首先是看下日志
```bash
$ systemctl status docker
$ journalctl -u docker
$ dmesg | grep docker
```
一般能看到如下日志：
```bash
docker0: link is not ready
docker_gwbridge: link is not ready
```
或者更简单的排查方法，直接手动启动 dockerd 看看。启动方法可以通过 `grep ExecStart /usr/lib/systemd/system/docker.service` 查看。<br />一般来说按如下执行就行：
```bash
$ /usr/bin/dockerd --debug
```
然后能看到最后输出：
```bash
INFO[2021-07-29T02:25:55.811673622Z] stopping event stream following graceful shutdown  error="<nil>" module=libcontainerd namespace=moby
failed to start daemon: Error initializing network controller: list bridge addresses failed: PredefinedLocalScopeDefaultNetworks List: [10.252.0.0/24 10.252.1.0/24 10.252.2.0/24]: no available network
```
这时候可以看下 `ip addr`，是否有 docker0 和 docker_gwbridge，<br />如果发现没有 docker0，那基本可以肯定是 docker0 无法创建导致 dockerd 启动失败了。
<a name="BDgal"></a>
## 4、解决
<a name="SWaNk"></a>
### 找到占用的网段
默认的 docker0 网段是 172.17.0.0/16，docker_gwbridge 网段是 172.18.0.0/24，需要确认下这两个网段是否被占用了。<br />最简单的方法就是 ping 一下，如果无响应的话，那么就说明没有被占用。<br />其次就是看一下本机的路由表 `route -n`，确认一下有没有冲突的段。一般来说，会发现相关网段要么已经被占用，要么是路由表里存在冲突。
<a name="BhMQB"></a>
## 5、案例
比如在自己的环境里，给 docker0 配置为 10.252.0/24，然后 dockerd 起不来。<br />排查后发现 `route -n` 里有这么一条：
```bash
10.0.0.0        172.21.0.1      255.0.0.0       UG    0      0        0 eth0
```
也就是说 10/8 被占用了，导致和 10.252.0/24 冲突。后来联系网管删除了这条 10/8 的路由后解决。
<a name="RvqSx"></a>
### 修改 docker 占用的网段
以下操作需要先停止 docker 进程
```bash
$ systemctl stop docker
```
如果用 `docker service`，那么 docker 会占用四个网段：

1. docker0
2. docker_gwbridge
3. ucp（docker engine 占用，不常见）
4. ingress

其中前三个的网段可以通过配置文件配置，第四个只能手动创建。<br />前三个网段会读取 /etc/docker/daemon.json 这个配置文件， 这个文件默认是没有的，需要手动创建。
```json
{
    "bip": "",
    "default-address-pools": [
        {"base": "10.252.0.0/24", "size": 24},
        {"base": "10.252.1.0/24", "size": 24},
        {"base": "10.252.2.0/24", "size": 24}
    ]
}
```
注意这个 `default-address-pools` 至少要有两项，按顺序依次为：

1. docker0
2. docker_gwbridge
3. ucp

以防万一的话，配置三个是最好的😄。<br />接下来是修改 ingress，这个比较麻烦，需要手动创建。<br />解决办法是在初始化 docker swarm 以后，启动 docker stack/service 以前，执行以下脚本：

1. 删除 ingress
2. 删除 my-ingress（不一定存在，不存在就忽略）
3. 新建 my-ingress
```bash
$ yes 'y' | docker network rm ingress
$ yes 'y' | docker network rm my-ingress 2>&1 | true
$ docker network create \
  --driver overlay \
  --ingress \
  --subnet=10.252.3.0/24 \  # 指定 ingress 的网段，不要和上面 daemon.json 的网段冲突
  --gateway=10.252.3.2 \
  --opt com.docker.network.driver.mtu=1200 \
  my-ingress
```
ingress 是默认名，之所以新建一个名字不一样的，是因为删除 ingress 是异步的。如果删除 ingress 后立刻新建一个也叫 ingress 的网络，很可能会报一个重名的错误。<br />重启 docker 前，最好先清理一下 docker 的缓存：
```bash
$ ip link del dev docker0
$ ip link del dev docker_gwbridge
$ rm -rf /var/lib/docker/network
```
<a name="rHSwN"></a>
### 另一种粗糙简单的解决办法
另一种粗糙简单的解决办法就是干脆直接手动创建一个 docker0。<br />这种解决方法最简单，但是机器 reboot 后 docker0 会自动被删掉， 所以这方法并不持久。<br />而且，实际上也不一定能解决网段冲突的问题，只是说 docker 能启动了而已。
```bash
ip link add name docker0 type bridge
ip addr add dev docker0 10.252.0.1/24
```
