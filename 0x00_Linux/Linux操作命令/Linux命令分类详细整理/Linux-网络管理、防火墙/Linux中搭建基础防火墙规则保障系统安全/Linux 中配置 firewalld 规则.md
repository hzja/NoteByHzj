Linux<br />防火墙对于控制进出 Linux 服务器的网络流量至关重要。它能够定义一组防火墙规则来控制主机上的传入流量。这里介绍如何添加、删除、启用和禁用防火墙规则和区域。
<a name="yX2Xi"></a>
## 什么是 FirewallD
“firewalld”是firewall daemon。它提供了一个动态管理的防火墙，带有一个非常强大的过滤系统，称为 Netfilter，由 Linux 内核提供。<br />FirewallD 使用zones和services的概念，而 iptables 使用chain和rules。与 iptables 相比，“FirewallD”提供了一种非常灵活的方式来处理防火墙管理。<br />每个zones都可以按照指定的标准进行配置，以根据要求接受或拒绝某些服务或端口，并且它可以与一个或多个网络接口相关联。默认区域为public区域。<br />[yijiFirewalld zones[/yiji]<br />以下命令列出 FirewallD 提供的zones。运行以下命令以列出zones：
```bash
[root@server1 ~]# firewall-cmd --get-zones
block dmz drop external home internal public trusted work
```
**block：**对于 IPv4，任何传入连接都会被 icmp-host-prohibited 消息拒绝，对于 IPv6 则是 icmp6-adm-prohibited。<br />**dmz：**应用于DMZ区域的计算机，这些计算机可公开访问，但对内部网络的访问受到限制。仅接受选定的传入连接。<br />**drop：**任何传入连接都将在没有任何通知的情况下被丢弃。只允许传出连接。<br />**external：**用于在系统中充当路由器时启用 NAT 伪装的外部网络。只允许选定的传入连接。<br />**home：**用于家庭网络。仅接受选定的传入连接。<br />**internal：**用于内部网络，网络上的其他系统通常是可信任的。仅接受选定的传入连接。<br />**public：**用于公共区域，仅接受选定的传入连接。<br />**trusted：**接受所有网络连接。<br />**work：**用于工作区域，同一网络上的其他计算机大多受信任。仅接受选定的传入连接。
<a name="ruL4v"></a>
## Firewalld services
Firewalld 的service配置是预定义的服务。要列出可用的服务模块，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --get-services
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1654063928927-660273d1-c3be-4665-b5cf-d9035b302905.png#clientId=u53d21c4f-aa1f-4&from=paste&height=398&id=u752da9d9&originHeight=995&originWidth=2147&originalType=binary&ratio=1&rotation=0&showTitle=false&size=194451&status=done&style=none&taskId=u4692a49c-1b86-46b5-b6f3-0ce0e8995db&title=&width=858.8)
<a name="IiRIK"></a>
## Firewalld的临时设置和永久设置
Firewalld 使用两个独立的配置，即临时设置和永久设置：<br />**临时设置：**临时设置不会在系统重启时保持不变。这意味着临时设置不会自动保存到永久设置中。<br />**永久设置：**永久设置会存储在配置文件中，将在每次重新启动时加载并成为新的临时设置。
<a name="xavE5"></a>
## 启用、禁用Firewalld
Firewalld默认安装在Centos7/8中，下面命令时如何启用或者停用firewalld:
```bash
# 启用Firewalld
[root@server1 ~]# systemctl start firewalld
# 禁用Firewalld
[root@server1 ~]# systemctl stop firewalld
# 开机启动
[root@server1 ~]# systemctl enable firewlald
# 禁止开机启动
[root@server1 ~]# systemctl disable firewalld
```
查看firewlald的状态：

```bash
[root@server1 ~]# systemctl status firewalld
# 或者
[root@server1 ~]# firewall-cmd --state
running
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457115-ad07efc6-ab4d-44ac-bfaf-78c492c65f1a.png#clientId=u75d6007d-0a77-4&from=paste&id=uac2eeb1d&originHeight=307&originWidth=801&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua0433ee6-350f-4bba-9457-1357e56f467&title=)
<a name="EOkwB"></a>
## zone管理
Firewalld 为每个区域提供不同级别的安全性，公共区域设置为默认区域。下面命令查看默认区域：
```bash
[root@server1 ~]# firewall-cmd --get-default-zone 
public
```
下面命令查看默认区域的配置：
```bash
[root@server1 ~]# firewall-cmd --list-all
public (active)
  target: default
  icmp-block-inversion: no
  interfaces: ens160
  sources: 
  services: cockpit dhcpv6-client ntp ssh
  ports: 2222/tcp
  protocols: 
  masquerade: no
  forward-ports: 
  source-ports: 
  icmp-blocks: 
  rich rules:
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457178-a2b07217-c024-482a-a897-6bcbe708ff78.png#clientId=u75d6007d-0a77-4&from=paste&id=uf822c50c&originHeight=299&originWidth=512&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1939c764-cfac-41f4-b83f-4d8a12a1e61&title=)<br />通过使用选项"--zone”和“--change-interface”的组合，可以轻松更改zone中的接口。例如，要将“ens33”接口分配给“home”区域，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --zone=home --change-interface=ens33
success
[root@server1 ~]# firewall-cmd --zone=home --list-all
home (active)
  target: default
  icmp-block-inversion: no
  interfaces: ens33
  sources: 
  services: cockpit dhcpv6-client mdns samba-client ssh
  ports: 
  protocols: 
  masquerade: no
  forward-ports: 
  source-ports: 
  icmp-blocks: 
  rich rules:
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457191-23887ba5-00de-4356-9d4c-045daa31aae3.png#clientId=u75d6007d-0a77-4&from=paste&id=u81eb7e90&originHeight=311&originWidth=667&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7c125ef7-f341-4276-bcbe-a8f686f02dc&title=)<br />要查看所有活动的zone，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --get-active-zones 
home
  interfaces: ens33
public
  interfaces: ens160
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457197-5b7a8718-fbce-4aef-b37b-d5f32ff53459.png#clientId=u75d6007d-0a77-4&from=paste&id=u165f232f&originHeight=88&originWidth=574&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9b6d7c97-1aff-4687-87e9-441538a24ce&title=)<br />要更改默认zone，请使用以下命令。例如，要将默认区域更改为 home，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --set-default-zone=home
```
要找出与 ens160 接口关联的区域，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --get-zone-of-interface=ens160
public
```
要创建新zone，请使用以下命令。例如，要创建一个名为“test”的新区域，并永久生效，请运行：
```bash
[root@server1 ~]# firewall-cmd --permanent --new-zone=test
success
[root@server1 ~]# firewall-cmd --reload
success
```
<a name="OqIQt"></a>
## 开放和关闭端口
打开特定端口允许用户从外部访问系统，这代表了安全风险。因此，仅在必要时为某些服务打开所需的端口。<br />要获取当前区域中开放的端口列表，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --list-ports 
2222/tcp
```
下面实例将特定端口永久添加到列表中：
```bash
[root@server1 ~]# firewall-cmd --permanent --add-port=8080/tcp
success
[root@server1 ~]# firewall-cmd --reload
success
```
同样，要删除特定端口，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --remove-port=8080/tcp
success
```
可以使用以下命令每次确认端口是否已添加或删除：
```bash
[root@server1 ~]# firewall-cmd --list-ports
```
如果要为特定区域开放端口，例如，以下命令将为 home 区域打开端口 80：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=home --add-port=80/tcp
success
[root@server1 ~]# firewall-cmd --reload
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457481-6c2ade92-b1e7-4eac-b189-15b3a8e4d689.png#clientId=u75d6007d-0a77-4&from=paste&id=u2722b529&originHeight=118&originWidth=611&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udb44a73c-9373-4455-9b4e-c742f0571dd&title=)<br />同样，要从开放的端口中删除特定区域的特定端口，请运行：
```bash
[root@server1 ~]# firewall-cmd --zone=home --remove-port=80/tcp
success
```
<a name="JX1Rh"></a>
## 添加和移除服务类型
Firewalld 服务配置是预定义的服务，如果启用了服务，则会自动加载。使用预定义服务使用户可以更轻松地启用和禁用对服务的访问。<br />预定义的服务配置文件位于/usr/lib/firewalld/services目录中。<br />Firewalld的服务，不需要记住任何端口，并且可以一次性允许所有端口。<br />例如，执行以下命令允许 samba 服务。samba 服务需要启用以下一组端口：“139/tcp 和 445/tcp”以及“137/udp 和 138/udp”。<br />添加'samba'服务后，所有端口都会同时激活，因为所有端口信息都在samba服务配置中。下面是Firewalld中预定义的samba的服务配置文件：
```bash
[root@server1 ~]# cat /usr/lib/firewalld/services/samba.xml
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457546-776dbfa1-8220-4fff-8bed-60c5164dd96a.png#clientId=u75d6007d-0a77-4&from=paste&id=u897c8f9d&originHeight=224&originWidth=858&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u808f1c9a-da5a-41ba-aff3-5f88482d6e6&title=)<br />下面是在home区域放行samba服务：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=home --add-service=samba
success
[root@server1 ~]# firewall-cmd --reload
success
```
要获取有关 samba 服务的更多信息，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --info-service=samba
samba
  ports: 137/udp 138/udp 139/tcp 445/tcp
  protocols: 
  source-ports: 
  modules: netbios-ns
  destination:
```
要一次添加多个服务，请执行以下命令。例如，要添加 http 和 https 服务，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=home --add-service={http,https}
success
[root@server1 ~]# firewall-cmd --reload
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457558-776fe2ed-bb86-4e56-9d06-aae4db7707c3.png#clientId=u75d6007d-0a77-4&from=paste&id=u82e2e2ae&originHeight=344&originWidth=704&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u06630ada-6dde-4004-9cd9-f7ed181098d&title=)
<a name="oEv1k"></a>
## 设置端口转发
端口转发是一种将任何传入网络流量从一个端口转发到另一个内部端口或另一台机器上的外部端口的方法。<br />注意：端口转发必须开启IP伪装。使用下面显示的命令为`external`区域启用伪装。
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=external --add-masquerade
```
要检查是否为区域启用了 IP 伪装，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --zone=external --query-masquerade 
yes
```
显示yes，表示已经开启伪装。<br />要将端口重定向到同一系统上的另一个端口，例如：将80端口的所有数据包重定向到8080端口：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=external --add-forward-port=port=80:proto=tcp:toport=8080
success
```
如果要将流量转发到另一台服务器，例如：将所有 80 端口的数据包重定向到 IP 为 10.0.0.75 的服务器上的 8080 端口：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=external --add-forward-port=port=80:proto=tcp:toport=8080:toaddr=10.0.0.75
success
```
例如，要允许来自特定源地址的流量，仅允许从特定子网连接到服务器，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --permanent --zone=home --add-source=192.168.1.0/24
success
```
<a name="FqNtC"></a>
## 富规则设置
富规则允许使用易于理解的命令创建更复杂的防火墙规则，但丰富的规则很难记住，可以查看手册man firewalld.richlanguage并找到示例。<br />富规则的一般规则结构如下：
```
rule
  [source]
  [destination]
  service|port|protocol|icmp-block|icmp-type|masquerade|forward-port|source-port
  [log]
  [audit]
  [accept|reject|drop|mark]
```
要允许来自地址 192.168.0.0/24 的访问，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --zone=public --add-rich-rule='rule family="ipv4" source address="192.168.0.0/24" accept'
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457818-e2f7efcc-e52a-4dad-ab2a-c19db33e7056.png#clientId=u75d6007d-0a77-4&from=paste&id=ucf877935&originHeight=298&originWidth=654&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ude207a10-6fe2-4be3-95fb-a7121a825d5&title=)<br />要允许来自地址 192.168.0.0/24 的连接访问 ssh 服务，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --zone=public --add-rich-rule='rule family="ipv4" source address="192.168.0.0/24" service name="ssh" log prefix="ssh" level="info" accept'
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457897-3555fc11-1620-488d-ad11-20f9ba515bee.png#clientId=u75d6007d-0a77-4&from=paste&id=u4731dff2&originHeight=295&originWidth=923&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0e5de121-66b3-447b-afb3-6c59e9f1675&title=)<br />要拒绝来自192.168.10.0/24的流量访问ssh服务，请运行以下命令：
```bash
[root@server1 ~]# firewall-cmd --zone=public --add-rich-rule='rule family="ipv4" source address="192.168.10.0/24" port port=22 protocol=tcp reject'
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264457894-7affd576-a915-45fc-856a-0453e7c59183.png#clientId=u75d6007d-0a77-4&from=paste&id=u3710782e&originHeight=111&originWidth=917&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc2713cf0-92de-46dd-a8b3-aaa506058a8&title=)<br />要删除任何富规则，请使用`--remove-rich-rule`选项，下面使用`--list-rich-rules`列出富规则，然后删除掉富规则：
```bash
[root@server1 ~]# firewall-cmd --zone=public --list-rich-rules 
rule family="ipv4" source address="192.168.0.0/24" accept
rule family="ipv4" source address="192.168.0.0/24" service name="ssh" log prefix="ssh" level="info" accept
rule family="ipv4" source address="192.168.10.0/24" port port="22" protocol="tcp" reject
[root@server1 ~]# firewall-cmd --zone=public --remove-rich-rule='rule family="ipv4" source address="192.168.0.0/24" accept'
success
[root@server1 ~]# firewall-cmd --zone=public --remove-rich-rule='rule family="ipv4" source address="192.168.0.0/24" service name="ssh" log prefix="ssh" level="info" accept'
success
[root@server1 ~]# firewall-cmd --zone=public --remove-rich-rule='rule family="ipv4" source address="192.168.10.0/24" port port="22" protocol="tcp" reject'
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264458121-92ee44bf-91e6-49ff-b6aa-d5c7f0c79dfd.png#clientId=u75d6007d-0a77-4&from=paste&id=uefa23ead&originHeight=225&originWidth=916&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue57c4312-4e63-4fc0-8041-ca2ef09c274&title=)
<a name="fGhEp"></a>
## Firewalld 的 Direct 规则
Direct规则类似于 iptables 命令，对于熟悉 iptables 命令的用户很有用。或者，可以编辑/etc/firewalld/direct.xml文件中的规则并重新加载防火墙以激活这些规则。Direct规则主要由服务或应用程序用来添加特定的防火墙规则。<br />以下Direct规则将在服务器上打开端口 8080：
```bash
[root@server1 ~]# firewall-cmd --permanent --direct --add-rule ipv4 filter INPUT 0 -p tcp --dport 8081 -j ACCEPT
success
[root@server1 ~]# firewall-cmd --reload
success
```
要列出当前区域中的Direct规则，请运行：
```bash
[root@server1 ~]# firewall-cmd --direct --get-all-rules 
ipv4 filter INPUT 0 -p tcp --dport 8080 -j ACCEPT
ipv4 filter INPUT 0 -p tcp --dport 8081 -j ACCEPT
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264458171-9fa76df3-aaf7-47a0-8ee4-18bad05f6dfe.png#clientId=u75d6007d-0a77-4&from=paste&id=u14bbfe86&originHeight=69&originWidth=570&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6b8afe20-e66b-4d02-aecb-8475dba2e56&title=)<br />使用下面命令删除Direct规则：
```bash
[root@server1 ~]# firewall-cmd --direct --get-all-rules 
ipv4 filter INPUT 0 -p tcp --dport 8080 -j ACCEPT
ipv4 filter INPUT 0 -p tcp --dport 8081 -j ACCEPT
[root@server1 ~]# firewall-cmd --permanent --direct --remove-rule ipv4 filter INPUT 0 -p tcp --dport 8080 -j ACCEPT
success
[root@server1 ~]# firewall-cmd --reload
success
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264458186-b9b8e4e0-4857-44bd-b17e-baa4a38b8b80.png#clientId=u75d6007d-0a77-4&from=paste&id=u95d73149&originHeight=216&originWidth=911&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubfda9c87-b5b2-4bb3-9c73-9599916deb7&title=)<br />如何清空一个表的链？下面是语法和实例：
```bash
firewall-cmd --direct --remove-rules ipv4 [table] [chain]
```
```bash
[root@server1 ~]# firewall-cmd --permanent --direct --remove-rules ipv4 filter INPUT
success
[root@server1 ~]# firewall-cmd --reload
success
[root@server1 ~]# firewall-cmd --direct --get-all-rules
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653264458296-58b22ff2-6a75-439c-be54-3d1d5ffb2fc8.png#clientId=u75d6007d-0a77-4&from=paste&id=u2037ca54&originHeight=109&originWidth=789&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69fa46eb-a3c0-4438-8df7-20b1ba49502&title=)
<a name="biEVU"></a>
## 总    结
了解了 firewalld 的完整用法，例如zones、允许/拒绝服务和端口、端口转发、Rich Rules、Direct规则等。
