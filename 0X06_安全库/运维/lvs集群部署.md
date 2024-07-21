<a name="0d01cd4c"></a>
### LVS集群配置信息DR模式

节点关闭Networkmanager**所有的节点**

```bash
[root@zabbix-server ~]# systemctl stop NetworkManager.service
[root@zabbix-server ~]# systemctl disable NetworkManager.service
```

<a name="99f6537f"></a>
#### 创建一个子接口 lvs机器

```bash
[root@centos network-scripts]# cat ifcfg-eno16777728:0
TYPE=Ethernet
BOOTPROTO=static
DEFROUTE=yes
DEVICE=eno16777728:0
ONBOOT=yes
DNS1=192.168.40.2
IPADDR=192.168.40.100
PREFIX=24
GATEWAY=192.168.40.2
[root@centos network-scripts]# systemctl restart network
[root@centos network-scripts]# ifconfig
eno16777728: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.40.25  netmask 255.255.255.0  broadcast 192.168.40.255
        inet6 fe80::20c:29ff:feda:abcb  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:da:ab:cb  txqueuelen 1000  (Ethernet)
        RX packets 50417  bytes 71989175 (68.6 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 14742  bytes 1029561 (1005.4 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

eno16777728:0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.40.100  netmask 255.255.255.0  broadcast 192.168.40.255
        ether 00:0c:29:da:ab:cb  txqueuelen 1000  (Ethernet)
```

<a name="93d99293"></a>
#### 关闭广播功能

```bash
[root@centos ~]# grep -v "#" /etc/sysctl.conf
net.ipv4.conf.all.send_redirects=0
net.ipv4.conf.default.send_redirects=0
net.ipv4.conf.eno16777728.send_redirects=0
[root@centos ~]# sysctl -p
```

<a name="a5e92570"></a>
#### 安装软件

```bash
[root@centos ~]# yum install -y ipvsadm
[root@centos ~]# modprobe ip_vs
[root@centos ~]# ipvsadm -Ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
[root@centos ~]#
```

<a name="42009172"></a>
#### 配置服务器1和2完全一致

```bash
[root@zabbix-server network-scripts]# cat ifcfg-lo:0
DEVICE=lo:0
IPADDR=192.168.40.100
NETMASK=255.255.255.255
NETWORK=127.0.0.0
# If you're having problems with gated making 127.0.0.0/8 a martian,
# you can change this to something else (255.255.255.255, for example)
BROADCAST=127.255.255.255
ONBOOT=yes
NAME=loopback
[root@zabbix-server ~]# grep -v "#" /etc/sysctl.conf
net.ipv4.conf.all.arp_ignore=1
net.ipv4.conf.all.arp_announce=2
net.ipv4.conf.default.arp_ignore=1
net.ipv4.conf.default.arp_announce=2
net.ipv4.conf.lo.arp_ignore=1
net.ipv4.conf.lo.arp_announce=2
[root@zabbix-server ~]# sysctl -p
net.ipv4.conf.all.arp_ignore = 1
net.ipv4.conf.all.arp_announce = 2
net.ipv4.conf.default.arp_ignore = 1
net.ipv4.conf.default.arp_announce = 2
net.ipv4.conf.lo.arp_ignore = 1
net.ipv4.conf.lo.arp_announce = 2
[root@zabbix-server ~]# ifup lo:0
[root@zabbix-server ~]# ifconfig
lo:0: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 192.168.40.100  netmask 255.255.255.255
        loop  txqueuelen 0  (Local Loopback)
[root@zabbix-server ~]# route add -host 192.168.40.100 dev lo:0
[root@zabbix-server ~]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.40.2    0.0.0.0         UG    100    0        0 eno16777                                                                                                         736
192.168.40.0    0.0.0.0         255.255.255.0   U     100    0        0 eno16777                                                                                                         736
192.168.40.100  0.0.0.0         255.255.255.255 UH    0      0        0 lo
[root@zabbix-server ~]#
```

<a name="e6efac8c"></a>
#### 访问测试

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599831288873-230683f3-fab5-49b3-8897-100bf6c7bc58.png#height=179&id=dPM56&originHeight=358&originWidth=978&originalType=binary&ratio=1&rotation=0&showTitle=false&size=209098&status=done&style=none&title=&width=489)

<a name="e85d481b"></a>
#### LVS配置规则测试

```bash
[root@centos ~]# ipvsadm -A -t 192.168.40.100:80 -s rr
[root@centos ~]# ipvsadm -a -t 192.168.40.100:80 -r 192.168.40.23:80 -g
[root@centos ~]# ipvsadm -a -t 192.168.40.100:80 -r 192.168.40.24:80 -g
[root@centos ~]# ipvsadm -Ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
TCP  192.168.40.100:80 rr
  -> 192.168.40.23:80             Route   1      0          0
  -> 192.168.40.24:80             Route   1      0          0
[root@centos ~]#
```

<a name="90f84f8b"></a>
### LVS集群配置信息NAT模式

<a name="cbb060fd"></a>
#### 环境准备

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599831300587-47e459d1-7fb6-484b-940c-486d9e242950.png#height=161&id=jGM89&originHeight=321&originWidth=995&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48827&status=done&style=none&title=&width=497.5)

| 角色 | IP地址 |
| --- | --- |
| lvs负载均衡 | 192.168.40.25/24 10.10.10.11/24 |
| web服务器1 | 192.168.40.23/24 |
| web服务器2 | 192.168.40.24/24 |


所有的节点关闭NetworkManager防火墙和selinux

```bash
[root@zabbix-server ~]# systemctl stop NetworkManager.service
[root@zabbix-server ~]# systemctl disable NetworkManager.service
[root@zabbix-server ~]# systemctl stop firewalld.service
[root@zabbix-server ~]# setenforce 0
```

<a name="43f373ff"></a>
#### lvs负载均衡

```bash
[root@centos ~]# echo "1" > /proc/sys/net/ipv4/ip_forward
[root@centos ~]# sysctl -p
[root@centos ~]# yum install -y ipvsadm
[root@centos ~]# systemctl start ipvsadm
[root@centos ~]# iptables -t nat -A POSTROUTING -s 192.168.40.0/24 -o eno16777728 -j SNAT --to-source 10.10.10.11
[root@centos ~]# ipvsadm -A -t 10.10.10.11:80 -s rr
[root@centos ~]# ipvsadm -a -t 10.10.10.11:80 -r 192.168.40.23:80 -m
[root@centos ~]# ipvsadm -a -t 10.10.10.11:80 -r 192.168.40.24:80 -m
[root@centos ~]# ipvsadm -Ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
TCP  10.10.10.11:80 rr
  -> 192.168.40.23:80             Masq    1      0          0
  -> 192.168.40.24:80             Masq    1      0          0
[root@centos ~]#
```

<a name="0294367b"></a>
#### 所有真实服务器端

将网关换成为lvs另外一张网卡的ip

```bash
[root@zabbix-server ~]# systemctl stop NetworkManager.service
[root@zabbix-server ~]# systemctl disable NetworkManager.service
[root@zabbix-server ~]# systemctl stop firewalld.service
[root@zabbix-server ~]# setenforce 0
[root@zabbix-server network-scripts]# grep -i "gateway" ifcfg-eno16777736
GATEWAY=192.168.40.25
[root@zabbix-server network-scripts]# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.40.25   0.0.0.0         UG    100    0        0 eno16777736
192.168.40.0    0.0.0.0         255.255.255.0   U     100    0        0 eno16777736
[root@zabbix-server network-scripts]#
```

<a name="e6efac8c-1"></a>
#### 访问测试

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599831318282-a4176760-0962-4582-87e9-b28ef53c20ff.png#height=311&id=s6m9h&originHeight=622&originWidth=1002&originalType=binary&ratio=1&rotation=0&showTitle=false&size=288331&status=done&style=none&title=&width=501)



<a name="8b3329ba"></a>
#### lvs测试状态监测

```bash
[root@centos ~]# ipvsadm -Ln
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port Scheduler Flags
  -> RemoteAddress:Port           Forward Weight ActiveConn InActConn
TCP  10.10.10.11:80 rr
  -> 192.168.40.23:80             Masq    1      0          3
  -> 192.168.40.24:80             Masq    1      0          3
[root@centos ~]#
```

<a name="6a3169d9"></a>
#### 持久化连接端口连接

```bash
[root@centos ~]# ipvsadm -D -t 10.10.10.11:80 	#删除一个集群信息
[root@centos ~]# ipvsadm -Ln --stats
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port               Conns   InPkts  OutPkts  InBytes OutBytes
  -> RemoteAddress:Port
[root@centos ~]# ipvsadm -A -t 10.10.10.11:80 -p 120 -s rr
[root@centos ~]# ipvsadm -a -t 10.10.10.11:80 -r 192.168.40.23:80 -m
[root@centos ~]# ipvsadm -a -t 10.10.10.11:80 -r 192.168.40.24:80 -m
[root@centos ~]# ipvsadm -Ln --persistent-conn
IP Virtual Server version 1.2.1 (size=4096)
Prot LocalAddress:Port            Weight    PersistConn ActiveConn InActConn
  -> RemoteAddress:Port
TCP  10.10.10.11:80 rr persistent 120
  -> 192.168.40.23:80             1         0           0          0
  -> 192.168.40.24:80             1         1           0          2
[root@centos ~]#
```

<a name="cd8992b6"></a>
#### 验证

多次刷新也是显示的是同一个页面

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599831338663-90a56ad7-ff38-4836-9b2e-37ae30f5ebcf.png#height=189&id=lL9Ak&originHeight=377&originWidth=982&originalType=binary&ratio=1&rotation=0&showTitle=false&size=227540&status=done&style=none&title=&width=491)

<a name="0e73ca60"></a>
#### LVS验证

```bash
[root@centos ~]# ipvsadm -Ln -c
IPVS connection entries
pro expire state       source             virtual            destination
TCP 01:46  NONE        10.10.10.1:0       10.10.10.11:80     192.168.40.24:80
TCP 01:51  FIN_WAIT    10.10.10.1:58274   10.10.10.11:80     192.168.40.24:80
TCP 01:52  FIN_WAIT    10.10.10.1:58280   10.10.10.11:80     192.168.40.24:80
TCP 01:52  FIN_WAIT    10.10.10.1:58275   10.10.10.11:80     192.168.40.24:80
[root@centos ~]#
```

发现始终访问的是192.168.40.24:80这台机器

<a name="bf5e0264"></a>
### 高可用

**keepalived+lvs**

在两台负载均衡服务器上面配置高可用是为了监听LVS的状态要是其中一台服务器宕机SLAVE节点可以顶替master节点继续工作，不影响用户的正常访问

**两台机器都安装keepalived**

```bash
[root@k8s-node2 ~]# yum -y install keepalived
[root@k8s-node2 ~]# cat /etc/keepalived/keepalived.conf
! Configuration File for keepalived

global_defs {
   router_id R1
}

vrrp_instance VI_1 {
    state MASTER
    interface eno16777728
    virtual_router_id 51
    priority 80
    advert_int 1
    authentication {
        auth_type PASS
        auth_pass 1111
    }
    virtual_ipaddress {
        10.10.10.11
    }
}

virtual_server 10.10.10.11 80 {
    delay_loop 2
    lb_algo rr
    lb_kind NAT
    protocol TCP

    real_server 192.168.40.23 80 {
        weight 1
        TCP_CHECK {
           connect_port 80
           connect_timeout 3
           nb_get_retry 3
           delay_before_retry 4
           }
        }
    }
}

    real_server 192.168.40.24 80 {
        weight 1
        TCP_CHECK {
           connect_port 80
           connect_timeout 3
           nb_get_retry 3
           delay_before_retry 4
           }
        }
    }
```

参考博客：[https://blog.51cto.com/14181896/2373270](https://blog.51cto.com/14181896/2373270)


