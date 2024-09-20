Ubuntu
<a name="yvl2M"></a>
## Ubuntu20.04.2配置静态固定IP地址
Ubuntu从17.10开始，放弃在/etc/network/interfaces里面配置IP，改为在/etc/netplan/XX-installer-config.yaml的yaml文件中配置IP地址。
<a name="jtcyO"></a>
### 查看网络配置信息
在Ubuntu20.04中，默认没有安装ifconfig，因此使用`ip addr`命令
```bash
ip addr
```
:::danger
拓展：可以使用 `ip route show` 命令查看网关
:::
或者使用`nmcli` 查看，需要执行安装`sudo apt install network-manager`
```bash
nmcli device show ens32
```
<a name="BYvwB"></a>
### 修改配置文件
使用熟悉的编辑器打开 /etc/netplan/下面的yaml配置文件，本人的是01-network-manager-all.yaml 文件名，看实际情况
```yaml
sudo vim /etc/netplan/01-network-manager-all.yaml
```
或者是
```yaml
sudo gedit /etc/netplan/01-network-manager-all.yaml
```
打开后可以是
```yaml
# Let NetworkManager manage all devices on this system
network:
  version: 2
  renderer: NetworkManager
```
根据自己的需要参考如下示例修改配置
```yaml
# Let NetworkManager manage all devices on this system
network:
  ethernets:
    enp0s8:   # 配置的网卡的名称
      addresses: [192.168.56.102/24]   # 配置的静态ip地址和掩码
      dhcp4: false   # 关闭dhcp4
      optional: true
      gateway4: 192.168.56.1 # 网关地址
      nameservers:
        addresses: [192.168.56.1,114.114.114.114]  # DNS服务器地址，多个DNS服务器地址需要用英文逗号分>隔开，可不配置
  version: 2
  renderer: NetworkManager
```
输入 :wq 保存退出后，执行 `sudo netplan apply` 使配置生效，之后每次启动虚拟机IP地址就不会再改变了。这样虚拟机内部可以正常上网，在物理机中也可以直接使用虚拟机的ip访问内部的服务。<br />使配置生效
```bash
sudo netplan apply
```
至此Ubuntu20.04的静态IP配置完成。经过测试，可以正常上网。
<a name="V9DBC"></a>
## Ubuntu17.1 以下版本配置方法
第一步：
```bash
sudo vim /etc/network/interfaces
```
（这里之所以加sudo是因为有些同学登陆用户非root，会导致权限不够）<br />具体编辑内容如下：
```bash
auto eno1  # en01 网卡名称
iface eno1 inet static
address 192.168.1.129   # IP地址， 要根据自己网段下IP的使用设置，不能和别的IP相冲突
netmask 255.255.255.0
gateway 192.168.1.2   #网关
```
查看网卡名称命令如下：
```bash
ifconfig
```
查看网关命令如下：
```bash
ip route show
```
![2022-09-05-23-14-42.850982200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662390902953-dbb1f769-ba25-4260-a8a0-a23723215869.png#clientId=ucc8c21f4-44f8-4&from=ui&id=u6dc70962&originHeight=751&originWidth=742&originalType=binary&ratio=1&rotation=0&showTitle=false&size=274569&status=done&style=none&taskId=u72e5a2b8-d006-47e3-8940-8776adea82e&title=)<br />第二步：设置dns
```bash
sudo vim /etc/resolvconf/resolv.conf.d/base
```
添加如下内容：
```bash
nameserver 8.8.8.8
nameserver 8.8.4.4
```
第三步：刷新配置文件
```bash
resolvconf -u
```
第四步：重启网卡
```bash
sudo ifdown 网卡名称
sudo ifup 网卡名称
```
第五步：输入`ifconfig`重新查看ip地址
