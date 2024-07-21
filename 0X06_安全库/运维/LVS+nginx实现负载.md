<a name="e9e9d469"></a>
### 实验背景说明

某公司有两个站点www.jw.com   bbs.jw.com经过调查nginx服务器做七层负载均衡无法实验流量的承担，但是又要去使用域名进行访问。公司有六台服务器搭建站点。

<a name="dfbc392b"></a>
#### LVS+nginx实现多级负载均衡配置

环境准备6台centos7服务器，提前在**所有**的节点关闭防火墙和selinux命令如下。

```bash
systemctl stop firewalld.service
setenforce 0
```

<a name="9e07dbbe"></a>
#### 环境配置说明
| 服务器名称 | IP地址 | 安装软件 | 作用 |
| --- | --- | --- | --- |
| lvs-master | 192.168.40.10 | ipvsadm | 四层负载 |
| ngx-1 | 192.168.40.11 | nginx | 七层负载 |
| ngx-2 | 192.168.40.12 | nginx | 七层负载 |
| apache-1 | 192.168.40.23 | apache | 网站服务 |
| apache-2 | 192.168.40.24 | apache | 网站服务 |
| apache-3 | 192.168.40.25 | apache | 网站服务 |


<a name="863a8583"></a>
#### 第一步

**安装软件**

```bash
[root@lvs-master ~]# yum install -y ipvsadm
[root@lvs-master ~]# modprobe ip_vs
[root@ngx-1 ~]# yum install -y nginx
[root@ngx-2 ~]# yum install -y nginx
[root@apache-1 ~]# yum install -y httpd
[root@apache-2 ~]# yum install -y httpd
[root@apache-3 ~]# yum install -y httpd
```

**配置网站信息**

```bash
[root@apache-1 ~]# echo "www.jw.com-1" > /var/www/html/index.html
[root@apache-2 ~]# echo "www.jw.com-2" > /var/www/html/index.html
[root@apache-3 ~]# echo "bbs.jw.com-1" > /var/www/html/index.html
[root@apache-1 ~]# curl 192.168.40.25
bbs.jw.com-1
[root@apache-1 ~]# curl 192.168.40.23
www.jw.com-1
[root@apache-1 ~]# curl 192.168.40.24
www.jw.com-2
[root@apache-1 ~]# curl 192.168.40.25
bbs.jw.com-1
[root@apache-1 ~]#
```

<a name="9757f2c5"></a>
#### 第二步

**ngx-1配置nginx负载均衡**

```bash
[root@ngx-1 ~]# cat /etc/nginx/conf.d/www.conf
upstream www.jw.com {
        server 192.168.40.23:80;
        server 192.168.40.24:80;
    }
    upstream bbs.jw.com {
        server 192.168.40.25:80;
    }
    server {
        listen       80;
        server_name  www.jw.com;
        location / {
          proxy_pass http://www.jw.com;
        }
    }
    server {
        listen       80;
        server_name  bbs.jw.com;
        location / {
          proxy_pass http://bbs.jw.com;
    }
}
[root@ngx-1 ~]# systemctl restart nginx
[root@ngx-1 ~]# cat /etc/hosts|grep 11
192.168.40.11    www.jw.com bbs.jw.com
[root@ngx-1 ~]# curl www.jw.com
www.jw.com-2
[root@ngx-1 ~]# curl www.jw.com
www.jw.com-1
[root@ngx-1 ~]# curl bbs.jw.com
bbs.jw.com-1
[root@ngx-1 ~]#
```

**ngx-2配置负载均衡**

```bash
[root@ngx-2 ~]# cat /etc/nginx/conf.d/www.conf
upstream www.jw.com {
        server 192.168.40.23:80;
        server 192.168.40.24:80;
    }
    upstream bbs.jw.com {
        server 192.168.40.25:80;
    }
    server {
        listen       80;
        server_name  www.jw.com;
        location / {
          proxy_pass http://www.jw.com;
        }
    }
    server {
        listen       80;
        server_name  bbs.jw.com;
        location / {
          proxy_pass http://bbs.jw.com;
    }
}
[root@ngx-2 ~]# cat /etc/hosts|grep 11
192.168.40.11   www.jw.com bbs.jw.com
[root@ngx-2 ~]# systemctl restart nginx
[root@ngx-2 ~]# curl bbs.jw.com
bbs.jw.com-1
[root@ngx-2 ~]# curl www.jw.com
www.jw.com-2
[root@ngx-2 ~]# curl www.jw.com
www.jw.com-1
[root@ngx-2 ~]#
```

**添加本机解析记录**


**访问测试**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830611728-6dd583f3-2f86-4047-a481-c35151d08bf0.png#align=left&display=inline&height=382&originHeight=763&originWidth=678&size=142764&status=done&style=none&width=339)

将IP地址换为192.168.40.12测试最后显示的效果和上面一模一样

<a name="207e30c0"></a>
#### 第三步

**LVS配置**

lvs-master ngx-1 ngx-2执行

```bash
[root@lvs-master ~]# systemctl stop NetworkManager
[root@lvs-master ~]# grep -v "#" /etc/sysctl.conf
net.ipv4.conf.all.arp_ignore=1
net.ipv4.conf.all.arp_announce=2
net.ipv4.conf.default.arp_ignore=1
net.ipv4.conf.default.arp_announce=2
net.ipv4.conf.lo.arp_ignore=1
net.ipv4.conf.lo.arp_announce=2
[root@lvs-master ~]# sysctl -p
net.ipv4.conf.all.arp_ignore = 1
net.ipv4.conf.all.arp_announce = 2
net.ipv4.conf.default.arp_ignore = 1
net.ipv4.conf.default.arp_announce = 2
net.ipv4.conf.lo.arp_ignore = 1
net.ipv4.conf.lo.arp_announce = 2
[root@lvs-master ~]#
```

lvs-master节点执行

```bash
[root@lvs-master ~]# vim /etc/sysconfig/network-scripts/ifcfg-eno16777736:0
TYPE=Ethernet
DEVICE=eno16777736:0
ONBOOT=yes
DNS1=192.168.40.2
IPADDR=192.168.40.100
GATEWAY=192.168.40.2
NETMASK=255.255.255.0
[root@lvs-master ~]# ifup eno16777736:0
[root@lvs-master ~]# ipvsadm -A -t 192.168.40.100:80 -s rr
[root@lvs-master ~]# ipvsadm -a -t 192.168.40.100:80 -r 192.168.40.11:80 -g
[root@lvs-master ~]# ipvsadm -a -t 192.168.40.100:80 -r 192.168.40.12:80 -g
```

ngx-1 ngx-2

```bash
[root@ngx-1 ~]# vim /etc/sysconfig/network-scripts/ifcfg-lo:0
DEVICE=lo:0
IPADDR=192.168.40.100
NETMASK=255.255.255.255
NETWORK=127.0.0.0
BROADCAST=127.255.255.255
ONBOOT=yes
NAME=loopback
[root@ngx-1 ~]# ifup lo:0
[root@ngx-1 ~]# route add -host 192.168.40.100 dev lo:0
```

修改hosts主机解析记录

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830550151-05a8d9d5-7ad8-4fa6-aae6-4d2f88d2ae69.png#align=left&display=inline&height=344&originHeight=688&originWidth=641&size=104648&status=done&style=none&width=320.5)

**总结：**实验完成了LVS+nginx实现了四层+七层的负载均衡，在生产的环境中可以在lvs加上keepalived实现高可用，防止单点故障的发生。由于nginx是只带了高可用的特性在后端服务器发生故障nginx是能自动剔除故障节点保证服务的不中断。

**补充：**

```bash
[root@apache-2 ~]# systemctl stop httpd
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830572367-f0da650f-cce0-4072-aeda-6b5f64df49ec.png#align=left&display=inline&height=91&originHeight=181&originWidth=555&size=28314&status=done&style=none&width=277.5)

我们仍然能够很好的访问到后端的节点

```bash
[root@apache-2 ~]# systemctl start httpd
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1599830583168-09037ea4-b0b6-4004-9e1a-9f9ea1506016.png#align=left&display=inline&height=77&originHeight=153&originWidth=470&size=26523&status=done&style=none&width=235)
