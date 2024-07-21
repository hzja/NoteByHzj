# NMAP

## 1. 简介

Nmap（the Network Mapper）是一款免费开源的网络发现和安全审计工具，能够运行在所有主流的操作系统上，包括Windows、Linux和macOS。



> 官网地址：https://nmap.org/
>
> GitHub地址：https://github.com/nmap/nmap



Nmap的主要功能有：

+ 主机发现（指定的主机是否存活）
+ 端口扫描（存活的主机中，指定的端口是否开启）
+ 应用和操作系统版本检测（存活的主机操作系统版本、开启的端口运行的服务版本）
+ 扫描脚本的编写



## 2. 基本用法

~~~ shell
nmap {target_hosts} [-p {target_ports}]
~~~

`target_hosts`指定需要扫描的主机，`target_ports`指定需要扫描的端口，按下`Enter`，Nmap就会开始扫描并输出扫描结果。



### 2.1. <code>target_hosts</code>语法

#### 2.1.1. 随机选择目标

~~~ shell
-iR <hostnum>
~~~

`hostnum`是目标主机的数量。



#### 2.1.2. 单个地址

~~~ shell
# IP地址
192.168.85.1

# 主机名
example.com
~~~



#### 2.1.3. 多个地址

~~~ shell
# 一段连续的地址
192.168.85.1-5
192.168.0-255.1-254 
0-255.0-255.13.37

# 一个网段
192.168.85.1/24
example.com/24

# 从文件中读取
-iL targets.txt
~~~



#### 2.1.4.从多个地址中排除一些地址

~~~ shell
192.168.85.1/24 --exclude <host1[,host2][,host3],...>
192.168.85.1/24 --excludefile <exclude_file>
~~~



### 2.2. <code>target_ports</code>语法

~~~ shell
# 用逗号分隔多个端口号
21,22,23,80

# 连续的端口号
1-100

# 上面两种结合
21,22,1-100
~~~



## 3. 选项 

在基本用法的基础上，还可以通过各种选项控制Nmap的各种行为，例如使用的扫描技术、是否探测版本等，下面是一些常用的选项：

+ -O：启动操作系统指纹扫描。
+ -sV：service Version，检测目标地址开放的端口对应的服务器版本信息。
+ -sS：SYN scans，使用TCP SYN扫描。
+ -A：启用操作系统扫描、版本扫描、脚本扫描和路由跟踪。
+ -v：verbose，提高输出结果的详细程度，使用-vv进一步提高输出结果的详细程度。
+ -T<0-5>: Time，扫描速度，数字越大扫描速度越快。
  



继续学习网址：[Nmap学习1 - 基础知识_hydap-CSDN博客](https://blog.csdn.net/weixin_43838889/article/details/122549402)



## 问题

### 问题一

~~~ tex
Note: Host seems down. If it is really up, but blocking our ping probes, try -Pn
Nmap done: 1 IP address (0 hosts up) scanned in 2.42 seconds
~~~



#### 原因

权限问题，尝试关闭防火墙 



#### 解决办法

~~~ shell
nmap -PN -sT 目标ip
~~~

ps:

~~~ tex
-Pn：将所有主机视为联机-跳过主机发现
-sS/sT/sA/sW/sM：TCP SYN/Connect()/ACK/Window/Maimon扫描
~~~



##### Kali_Linux关闭防火墙 

~~~ shell
apt-get install ufw #安装防火墙
ufw diable #关闭防火墙
ufw enable #开启防火墙
~~~

