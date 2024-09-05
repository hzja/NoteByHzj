在Linux系统中，搭建强固的防火墙是确保系统安全的重要步骤。通过合理配置防火墙规则，可以有效防范网络攻击、限制不必要的访问，并保障系统的稳定性。本文将详细介绍基础防火墙配置的步骤和示例代码，以帮助管理员提升系统的安全性。
<a name="T35jA"></a>
## 使用firewall-cmd
firewall-cmd 是基于 firewalld 的命令行工具，用于配置 iptables 防火墙规则。
```bash
# 示例代码：启用 firewalld 服务
sudo systemctl start firewalld
sudo systemctl enable firewalld
```
<a name="dtgl6"></a>
## 查看防火墙状态和已开放端口
```bash
# 示例代码：查看防火墙状态
sudo firewall-cmd --state
```
```bash
# 示例代码：查看已开放端口
sudo firewall-cmd --list-ports
```
<a name="U85t3"></a>
## 开放和关闭端口
```bash
# 示例代码：开放端口
sudo firewall-cmd --add-port=80/tcp --permanent
sudo firewall-cmd --reload
```
```bash
# 示例代码：关闭端口
sudo firewall-cmd --remove-port=80/tcp --permanent
sudo firewall-cmd --reload
```
<a name="BxYuO"></a>
## 允许和拒绝特定IP地址
```bash
# 示例代码：允许特定IP访问
sudo firewall-cmd --add-source=192.168.1.2 --permanent
sudo firewall-cmd --reload
```
```bash
# 示例代码：拒绝特定IP访问
sudo firewall-cmd --add-source=192.168.1.3 --permanent --zone=drop
sudo firewall-cmd --reload
```
<a name="Xsy0c"></a>
## 配置端口转发
```bash
# 示例代码：配置端口转发
sudo firewall-cmd --add-forward-port=80:internal_ip:80 --permanent
sudo firewall-cmd --reload
```
<a name="W5puU"></a>
## 使用服务定义规则
```bash
# 示例代码：开放SSH服务
sudo firewall-cmd --add-service=ssh --permanent
sudo firewall-cmd --reload
```
<a name="QtKss"></a>
## 配置防火墙日志
```bash
# 示例代码：开启防火墙日志
sudo firewall-cmd --set-log-denied=all
sudo firewall-cmd --reload
```
<a name="kg3uX"></a>
## 定制富规则
```bash
# 示例代码：定制富规则
sudo firewall-cmd --add-rich-rule='rule family="ipv4" source address="192.168.1.2" drop' --permanent
sudo firewall-cmd --reload
```
<a name="u8FPB"></a>
## 使用iptables命令
对于熟悉传统 iptables 的管理员，也可以使用 iptables 命令进行防火墙配置。
```bash
# 示例代码：开放端口
sudo iptables -A INPUT -p tcp --dport 80 -j ACCEPT
sudo service iptables save
sudo service iptables restart
```
<a name="QnGZq"></a>
## 防火墙配置备份和恢复
```bash
# 示例代码：备份防火墙规则
sudo firewall-cmd --zone=public --list-all > firewall-backup.xml
```
```bash
# 示例代码：恢复防火墙规则
sudo firewall-cmd --restore < firewall-backup.xml
sudo firewall-cmd --reload
```
<a name="T1bAS"></a>
## 使用fail2ban加固防火墙
fail2ban 是一个基于日志文件的入侵防御工具，它可以监控系统日志，自动封锁恶意的IP地址。
```bash
# 示例代码：安装fail2ban
sudo yum install epel-release
sudo yum install fail2ban
```
```bash
# 示例代码：启动并配置fail2ban
sudo systemctl start fail2ban
sudo systemctl enable fail2ban
```
<a name="XqjCC"></a>
## 配置端口转发和NAT
```bash
# 示例代码：配置端口转发
sudo iptables -t nat -A PREROUTING -p tcp --dport 80 -j DNAT --to-destination internal_ip:80
sudo iptables -t nat -A POSTROUTING -j MASQUERADE
sudo service iptables save
sudo service iptables restart
```
<a name="sOoBA"></a>
## 使用ipset管理IP集合
```bash
# 示例代码：创建ipset集合
sudo ipset create blocked_ips hash:ip
```
```bash
# 示例代码：添加IP到集合
sudo ipset add blocked_ips 192.168.1.2
```
```bash
# 示例代码：使用ipset阻止IP
sudo iptables -A INPUT -m set --match-set blocked_ips src -j DROP
sudo service iptables save
sudo service iptables restart
```
<a name="O9qj5"></a>
## 配置IPv6防火墙规则
```bash
# 示例代码：开放IPv6端口
sudo firewall-cmd --add-port=80/tcp --permanent --zone=public
sudo firewall-cmd --reload
```
<a name="Zgn1f"></a>
## 使用SELinux增强安全性
SELinux 是Linux内核的安全模块，可以提供更强大的安全策略。
```bash
# 示例代码：查看SELinux状态
sudo sestatus
```
```bash
# 示例代码：禁用SELinux
sudo setenforce 0
```
<a name="zt9BW"></a>
## 配置TCP Wrapper限制访问
/etc/hosts.allow 和 /etc/hosts.deny 可以用于配置TCP Wrapper规则，限制允许和拒绝的访问。
```bash
# 示例代码：仅允许特定IP访问SSH
echo "sshd: 192.168.1.2" | sudo tee -a /etc/hosts.allow
echo "sshd: ALL" | sudo tee -a /etc/hosts.deny
```
<a name="XDRZS"></a>
## 定期审查防火墙规则
```bash
# 示例代码：查看防火墙规则
sudo iptables -L -n
```
```bash
# 示例代码：查看firewalld规则
sudo firewall-cmd --list-all
```
<a name="kjHrw"></a>
## 设置连接跟踪规则
```bash
# 示例代码：开启连接跟踪
sudo modprobe nf_conntrack
```
```bash
# 示例代码：查看连接跟踪表
sudo cat /proc/net/nf_conntrack
```
<a name="IS3IF"></a>
## 配置限制连接速率
```bash
# 示例代码：使用iptables限制连接速率
sudo iptables -A INPUT -p tcp --syn --dport 80 -m connlimit --connlimit-above 20 --connlimit-mask 32 -j DROP
sudo service iptables save
sudo service iptables restart
```
<a name="AUpkB"></a>
## 总结
在搭建基础防火墙规则以保障Linux系统安全方面，本文提供了丰富而详细的示例代码，助力管理员深入了解和配置系统防火墙。从基础的端口管理、IP地址允许和拒绝，到更高级的防火墙配置、IPv6规则、SELinux和TCP Wrapper，本文深入介绍了多种工具和技术，使得管理员能够全面地提升系统的安全性。<br />通过 firewall-cmd、iptables 以及其他工具的灵活应用，管理员可以定制防火墙规则，限制不必要的访问，防范网络攻击。fail2ban 的引入增加了对恶意IP的自动封锁，提高了系统的主动防御能力。此外，学习如何配置端口转发、使用ipset管理IP集合、设置IPv6规则，以及定期审查和优化防火墙规则，都是提高系统安全性的关键步骤。<br />对于更高级的安全需求，本文还介绍了使用 SELinux 增强系统安全性、配置TCP Wrapper限制访问、设置连接跟踪规则以及限制连接速率等技术。这些工具和技术的综合使用，为管理员提供了多层次、全方位的系统安全保障。总体而言，通过不断学习和实践这些防火墙配置技巧，管理员将更有信心、更灵活地维护和提升Linux系统的安全性。
