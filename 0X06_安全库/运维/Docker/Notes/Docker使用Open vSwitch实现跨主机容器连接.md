Docker Open vSwitch
<a name="hhMGU"></a>
# 1、背景及条件
1.两台Ubuntu14.04或CentOS虚拟机模拟服务器<br />2.双网卡Host-Only&NAT<br />3.安装Open vSwitch<br />Ubuntu安装：
```bash
$ apt-get install openvswitch-switch
```
CentOS安装：
```bash
$ yum install openvswitch-switch
```
4.安装网桥管理工具<br />Ubuntu安装：
```bash
$ apt-get install bridge-utils
```
CentOS安装：
```bash
$ yum install bridge-utils
```
5.IP地址：<br />Host1：192.168.59.103<br />Host1：192.168.59.104<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894567360-20c24363-eefa-4135-94e5-3d0bb5080b25.png#align=left&display=inline&height=503&originHeight=1382&originWidth=2568&size=591515&status=done&style=none&width=933.8181818181819)

<a name="CXGxs"></a>
# 2、步骤分析

- **建立ovs网桥**
- **添加gre连接**
- **配置Docker容器虚拟网桥**
- **为虚拟网桥添加ovs接口**
- **添加不同Docker容器网段路由**

