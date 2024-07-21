<a name="47fd5753"></a>
# squid代理

**优势：**

1.提高客户端访问速度；

2.隐蔽内部主机的ip地址；

3.部署简单，可以实现访问控制；

**作用**

——代理上网(正向代理，透明代理）

——网站静态页面缓存加速（反向代理)


| 主机名称 | IP地址 | 角色 |
| --- | --- | --- |
| client | 192.168.40.24 | 客户端 |
| squid | 192.168.40.23 10.10.10.11 | squid服务器 |
| web | 10.10.10.12 | web服务器 |


<a name="5ce824e8"></a>
### 一、传统模式

客户端必须在自己的浏览器里指定代理服务器的ip地址和代理服务监听的端口号，才能通过squid代理服务器上网。

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830752964-df6040fc-65d5-4103-80c4-3726f2a2f343.png#height=124&id=GDu8o&originHeight=247&originWidth=993&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60470&status=done&style=none&title=&width=496.5)

<a name="0da66a22"></a>
#### 前提条件两张网卡能够两两ping通

```bash
[root@client ~]# ping 192.168.40.23
PING 192.168.40.23 (192.168.40.23) 56(84) bytes of data.
64 bytes from 192.168.40.23: icmp_seq=1 ttl=64 time=0.260 ms
64 bytes from 192.168.40.23: icmp_seq=2 ttl=64 time=0.335 ms
^C
--- 192.168.40.23 ping statistics ---
2 packets transmitted, 2 received, 0% packet loss, time 1000ms
rtt min/avg/max/mdev = 0.260/0.297/0.335/0.041 ms
[root@squid ~]# ping 10.10.10.12
PING 10.10.10.12 (10.10.10.12) 56(84) bytes of data.
64 bytes from 10.10.10.12: icmp_seq=1 ttl=64 time=0.163 ms
64 bytes from 10.10.10.12: icmp_seq=2 ttl=64 time=0.860 ms
64 bytes from 10.10.10.12: icmp_seq=3 ttl=64 time=0.339 ms
^C
--- 10.10.10.12 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2001ms
rtt min/avg/max/mdev = 0.163/0.454/0.860/0.295 ms
[root@squid ~]#
```

<a name="24066d41"></a>
#### 将客户端的网关指向squid服务器

```bash
[root@client ~]# cat /etc/sysconfig/network-scripts/ifcfg-eno16777736
TYPE=Ethernet
ONBOOT=yes
DNS1=192.168.40.2
IPADDR=192.168.40.24
PREFIX=24
GATEWAY=192.168.40.23
IPV6_PEERDNS=yes
IPV6_PEERROUTES=yes
[root@client ~]#
```

<a name="0fc32480"></a>
#### 开启路由转发功能

```bash
[root@squid ~]# echo "1" >/proc/sys/net/ipv4/ip_forward
[root@squid ~]# sysctl -p
[root@squid ~]# yum install -y squid
[root@squid ~]# systemctl start httpd
[root@squid ~]# systemctl enable httpd
```

<a name="bc7edfef"></a>
#### 测试访问

```bash
[root@client ~]# curl 10.10.10.12
^C
[root@client ~]# curl -x sockers5://192.168.40.23:3128 10.10.10.12
bbs.jw.com-1
```

<a name="aaabf8c0"></a>
#### 关闭web服务器测试

```bash
[root@web ~]# systemctl stop httpd
[root@web ~]#
```

```bash
[root@client ~]# curl -x sockers5://192.168.40.23:3128 10.10.10.12
bbs.jw.com-1
```

结论：客户端访问得到的结果是使用的squid的缓存的数据并非web服务器的数据，在大型的网站当中可以使用这种缓存的方式减少web服务器的压力

<a name="65e434d9"></a>
### 二、透明模式

透明代理，本质作用是和普通代理是一样的，但是透明代理，客户端不需要在自己的浏览器里指定代理服务器的ip地址和代理服务监听的端口号，给客户端的感觉自己是直接和公网通信，但是必须把网关地址指向代理服务器的内网接口ip地址。

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830772080-668bac14-7784-468c-b36c-87f26a5271d2.png#height=136&id=TiQI0&originHeight=271&originWidth=1017&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71106&status=done&style=none&title=&width=508.5)

<a name="4307cd7f"></a>
#### 修改配置信息

```bash
[root@squid ~]# vim /etc/squid/squid.conf
# Squid normally listens to port 3128
http_port 192.168.40.23:3128 transparent
[root@squid ~]# systemctl reload squid
[root@squid ~]# iptables -t nat -A PREROUTING -i eno16777736 -s 192.168.40.0/24 -p tcp --dport 80 -j REDIRECT --to-ports 3128
```

<a name="e6efac8c"></a>
#### 访问测试

```bash
[root@client ~]# curl  10.10.10.12
bbs.jw.com-1
[root@client ~]#
```

<a name="b5934921"></a>
#### 限制访问文件的大小

正常情况下

```bash
[root@web html]# dd if=/dev/zero of=s count=1M bs=1024
[root@client ~]# wget  10.10.10.12/size.html
--2020-09-10 04:16:38--  http://10.10.10.12/s
Connecting to 10.10.10.12:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 1073741824 (1.0G) [text/html]
Saving to: ‘size.html’

30% [===========>                             ] 328,439,473 87.1MB/s  eta 10s    ^C
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830786383-f77f9e7c-3168-429e-a4bf-c02f50d5c0f3.png#height=244&id=XWa8h&originHeight=488&originWidth=820&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64439&status=done&style=none&title=&width=410)

**添加访问策略**

```bash
[root@squid ~]# grep "500" /etc/squid/squid.conf
reply_body_max_size 500 MB
[root@squid ~]# systemctl restart squid
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830831041-0ded0c8c-0d4a-49d7-b774-2f8de1801021.png#height=414&id=ArgNv&originHeight=827&originWidth=1053&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136020&status=done&style=none&title=&width=526.5)

**总结：**传统模式与透明模式的主要区别就是访问的时候不用手动添加代理和端口对用户的体验更加的友好

<a name="8f3f426b"></a>
### 三、反向代理

**实验拓扑图**

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830854630-3e4d3316-0e6e-4bd6-9806-3218f81f0209.png#height=165&id=rwh3u&originHeight=329&originWidth=972&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55427&status=done&style=none&title=&width=486)

| 主机名称 | IP地址 | 角色 |
| --- | --- | --- |
| client | 192.168.40.24 | 客户端 |
| squid | 192.168.40.23 10.10.10.11 | squid服务器 |
| web-1 | 10.10.10.12 | web服务器 |
| web-2 | 10.10.10.13 | web服务器 |


传统模式和透明是为客户端服务的，借助squid加快访问web服务的速度，或者是公司内部对员工上网行为做限制使用的，而反向代理模式下的squid的服务对象是web服务器，通过squid来隐藏真实web服务器IP，加快客户的访问速度，还有负载均衡的功能。

<a name="af7a4bae"></a>
#### **将两台web服务器的网关指向squid服务器**

```bash
[root@web-1 html]# grep -i "gateway" /etc/sysconfig/network-scripts/ifcfg-eno16777728
GATEWAY=10.10.10.11
[root@web2 ~]# grep -i "gateway" /etc/sysconfig/network-scripts/ifcfg-eno16777736
GATEWAY=10.10.10.11
```

<a name="90b0688a"></a>
#### **web服务端测试**

```bash
[root@squid ~]# curl 10.10.10.12
web-1
[root@squid ~]# curl 10.10.10.13
web-2
[root@squid ~]#
```

<a name="6aa8b62f"></a>
#### **squid配置**

```bash
[root@squid ~]# grep -A 2 "^http_port" /etc/squid/squid.conf
http_port 80 vhost
cache_peer 10.10.10.12 parent 80 0 originserver no-query originserver round-robin name=apache1
cache_peer 10.10.10.13 parent 80 0 originserver no-query originserver round-robin name=apache2
[root@squid ~]# systemctl reload squid
[root@squid ~]# echo "1" > /proc/sys/net/ipv4/ip_forward
[root@squid ~]# sysctl -p
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830875646-7c5123d0-0ef9-4559-a723-ccbd0b564b6f.png#height=264&id=GAB0Y&originHeight=528&originWidth=724&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45690&status=done&style=none&title=&width=362)

<a name="f8f68d5e"></a>
#### squid做反向代理缓存静态页面内容

```bash
[root@web-1 ~]# systemctl stop httpd
```

停止其中的一台机器无论怎么刷新访问都是web-2

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830887876-334395bf-c6e4-47bd-a548-7985a5d66fd8.png#height=95&id=a5jEh&originHeight=190&originWidth=495&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18713&status=done&style=none&title=&width=247.5)

要是两台机器都停止了就会出现访问最后访问的页面

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830896733-989fdb55-ba1c-4c03-8339-f7ca8ee03bcb.png#height=113&id=z3op1&originHeight=226&originWidth=529&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19995&status=done&style=none&title=&width=264.5)

<a name="18a949b3"></a>
### 四、ACL配置由两部分组成

之所以我们没有做ACL访问策略就能访问是因为在默认的配置文件当中就是包含了这些网段的内容

```bash
acl localnet src 10.0.0.0/8     # RFC1918 possible internal network
acl localnet src 172.16.0.0/12  # RFC1918 possible internal network
acl localnet src 192.168.0.0/16 # RFC1918 possible internal network
acl localnet src fc00::/7       # RFC 4193 local private network range
acl localnet src fe80::/10      # RFC 4291 link-local (directly plugged) machines
```

<a name="14d3838c"></a>
#### 初识ACL规则

**我们将其中的 acl localnet src 192.168.0.0/16给注释掉重启服务就会发现访问被拒绝**

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830908036-74e01642-9985-41f1-96bb-23c07eebd1d6.png#height=252&id=Mp4ju&originHeight=504&originWidth=748&originalType=binary&ratio=1&rotation=0&showTitle=false&size=83086&status=done&style=none&title=&width=374)

acl 列表名称 列表类型 列表内容

http access allow或者deny列表名称

| 内容 | 字段说明 |
| --- | --- |
| src | 源地址 |
| dst | 目标地址 |
| port | 目标端口 |
| dstdomain | 目标域 |
| time | 访问时间 |
| maxconn | 最大连接 |
| url_regex | 目标URL地址 |
| urlpath_regex | 整个目标URL地址 |


**备注**：要是只是写了acl没有写http access默认是拒绝。
