Nginx keepalived
<a name="Acv3d"></a>
### 什么是高可用？
高可用HA（High Availability）是分布式系统架构设计中必须考虑的因素之一，它通常是指，通过设计减少系统不能提供服务的时间。<br />如果一个系统能够一直提供服务，那么这个可用性则是百分之百，但是天有不测风云。所以只能尽可能的去减少服务的故障。
<a name="itFAB"></a>
### 解决的问题？
在生产环境上很多时候是以Nginx做反向代理对外提供服务，但是一天Nginx难免遇见故障，如：服务器宕机。当Nginx宕机那么所有对外提供的接口都将导致无法访问。<br />虽然无法保证服务器百分之百可用，但是也得想办法避免这种悲剧，今天使用keepalived来实现Nginx的高可用。
<a name="y7xqP"></a>
### 双机热备方案
这种方案是国内企业中最为普遍的一种高可用方案，双机热备其实就是指一台服务器在提供服务，另一台为某服务的备用状态，当一台服务器不可用另外一台就会顶替上去。
<a name="GGQsw"></a>
#### keepalived是什么？
Keepalived软件起初是专为LVS负载均衡软件设计的，用来管理并监控LVS集群系统中各个服务节点的状态，后来又加入了可以实现高可用的VRRP (Virtual Router Redundancy Protocol ，虚拟路由器冗余协议）功能。<br />因此，Keepalived除了能够管理LVS软件外，还可以作为其他服务（例如：Nginx、Haproxy、MySQL等）的高可用解决方案软件
<a name="iCoLG"></a>
#### 故障转移机制
Keepalived高可用服务之间的故障切换转移，是通过VRRP 来实现的。<br />在 Keepalived服务正常工作时，主 Master节点会不断地向备节点发送（多播的方式）心跳消息，用以告诉备Backup节点自己还活着，当主 Master节点发生故障时，就无法发送心跳消息，备节点也就因此无法继续检测到来自主 Master节点的心跳了，于是调用自身的接管程序，接管主Master节点的 IP资源及服务。<br />而当主 Master节点恢复时，备Backup节点又会释放主节点故障时自身接管的IP资源及服务，恢复到原来的备用角色。
<a name="tONLV"></a>
### 实现过程
<a name="mKJQw"></a>
#### 准备工作
192.168.16.128<br />192.168.16.129<br />两台虚拟机。安装好Nginx
<a name="GOhnJ"></a>
#### 安装Nginx
更新`yum`源文件：
```bash
rpm -ivh http://nginx.org/packages/centos/7/noarch/RPMS/nginx-release-centos-7-0.el7.ngx.noarch.rpm
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
```
安装Nginx：
```bash
yum -y install  nginx
```
操作命令：
```bash
systemctl start nginx; #启动Nginx
systemctl stop nginx; #停止Nginx
```
<a name="rYQ4b"></a>
#### 安装keepalived
yum方式直接安装即可，该方式会自动安装依赖：
```bash
yum -y install keepalived
```
<a name="jxEMv"></a>
#### 修改主机（192.168.16.128）keepalived配置文件
yum方式安装的会生产配置文件在/etc/keepalived下：
```bash
vim keepalived.conf
keepalived.conf:
#检测脚本
vrrp_script chk_http_port {
    script "/usr/local/src/check_nginx_pid.sh" #心跳执行的脚本，检测nginx是否启动
    interval 2                          #（检测脚本执行的间隔，单位是秒）
    weight 2                            #权重
}
#vrrp 实例定义部分
vrrp_instance VI_1 {
    state MASTER            # 指定keepalived的角色，MASTER为主，BACKUP为备
    interface ens33         # 当前进行vrrp通讯的网络接口卡(当前centos的网卡) 用ifconfig查看你具体的网卡
    virtual_router_id 66    # 虚拟路由编号，主从要一直
    priority 100            # 优先级，数值越大，获取处理请求的优先级越高
    advert_int 1            # 检查间隔，默认为1s(vrrp组播周期秒数)
    #授权访问
    authentication {
        auth_type PASS #设置验证类型和密码，MASTER和BACKUP必须使用相同的密码才能正常通信
        auth_pass 1111
    }
    track_script {
        chk_http_port            #（调用检测脚本）
    }
    virtual_ipaddress {
        192.168.16.130            # 定义虚拟ip(VIP)，可多设，每行一个
    }
}
```
`virtual_ipaddress` 里面可以配置vip，在线上通过vip来访问服务。<br />interface` 需要根据服务器网卡进行设置通常查看方式 `ip addr<br />`authentication`配置授权访问后备机也需要相同配置
<a name="tUY3p"></a>
#### 修改备机（192.168.16.129）keepalived配置文件
keepalived.conf：
```
#检测脚本
vrrp_script chk_http_port {
    script "/usr/local/src/check_nginx_pid.sh" #心跳执行的脚本，检测nginx是否启动
    interval 2                          #（检测脚本执行的间隔）
    weight 2                            #权重
}
#vrrp 实例定义部分
vrrp_instance VI_1 {
    state BACKUP                        # 指定keepalived的角色，MASTER为主，BACKUP为备
    interface ens33                      # 当前进行vrrp通讯的网络接口卡(当前centos的网卡) 用ifconfig查看你具体的网卡
    virtual_router_id 66                # 虚拟路由编号，主从要一直
    priority 99                         # 优先级，数值越大，获取处理请求的优先级越高
    advert_int 1                        # 检查间隔，默认为1s(vrrp组播周期秒数)
    #授权访问
    authentication {
        auth_type PASS #设置验证类型和密码，MASTER和BACKUP必须使用相同的密码才能正常通信
        auth_pass 1111
    }
    track_script {
        chk_http_port                   #（调用检测脚本）
    }
    virtual_ipaddress {
        192.168.16.130                   # 定义虚拟ip(VIP)，可多设，每行一个
    }
}
```
<a name="dmGxM"></a>
#### 检测脚本：
```bash
#!/bin/bash
#检测nginx是否启动了
A=`ps -C nginx --no-header |wc -l`        
if [ $A -eq 0 ];then    #如果nginx没有启动就启动nginx                        
      systemctl start nginx                #重启nginx
      if [ `ps -C nginx --no-header |wc -l` -eq 0 ];then    #nginx重启失败，则停掉keepalived服务，进行VIP转移
              killall keepalived                    
      fi
fi
```
脚本授权：`chmod 775 check_nginx_pid.sh`<br />说明：脚本必须通过授权，不然没权限访问，在这里两条服务器执行、VIP(virtual_ipaddress:192.168.16.130)，在生产环境是直接通过vip来访问服务。<br />模拟nginx故障：<br />修改两个服务器默认访问的Nginx的html页面作为区别。<br />首先访问192.168.16.130，通过vip进行访问，页面显示192.168.16.128；说明当前是主服务器提供的服务。<br />这个时候192.168.16.128主服务器执行命令：
```bash
systemctl stop nginx; #停止nginx
```
再次访问vip(192.168.16.130)发现这个时候页面显示的还是：192.168.16.128，这是脚本里面自动重启。<br />现在直接将192.168.16.128服务器关闭，在此访问vip(192.168.16.130)现在发现页面显示192.168.16.129，这个时候keepalived就自动故障转移了，一套企业级生产环境的高可用方案就搭建好了。<br />keepalived中还有许多功能比如：邮箱提醒等等，可以去官网看看文档。
