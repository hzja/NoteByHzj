<a name="NgHOk"></a>
## 1、下载 VirtualBox并安装
```
https://www.virtualbox.org/wiki/Downloads
```
<a name="RjpBf"></a>
## 2、下载Rocky Linux
选择 Rocky-9.1-x86_64-dvd.iso 镜像<br />官网：[https://rockylinux.org/download](https://rockylinux.org/download)<br />阿里云镜像服务：[https://mirrors.aliyun.com/rockylinux/9.1/isos/x86_64/?spm=a2c6h.25603864.0.0.70c556799nMQLA](https://mirrors.aliyun.com/rockylinux/9.1/isos/x86_64/?spm=a2c6h.25603864.0.0.70c556799nMQLA)
<a name="Sgat2"></a>
## 3、创建虚拟机镜像

- 选择Rocky Linux镜像文件

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896125715-f1753adb-352c-4487-8b78-7f2ba3e32d5d.png#averageHue=%23a8c2e0&clientId=ue4588d76-548e-4&from=paste&id=ucd0697df&originHeight=564&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3dfdee8e-e91a-4b12-87ad-47ec69c8409&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896125644-60ed356c-e78d-4fcb-a052-27d5c614b1f4.png#averageHue=%2387a5c1&clientId=ue4588d76-548e-4&from=paste&id=u6bd0e74a&originHeight=547&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud630d85d-63ca-4c14-940b-e2c9175e40f&title=)

- 设置用户/密码、Hostname、Domain Name

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896125703-dba46cf4-0522-4ff0-8a09-a4b0570e785f.png#averageHue=%238aa6c2&clientId=ue4588d76-548e-4&from=paste&id=uecb25501&originHeight=552&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8062129a-fee2-4ffb-b62f-17eb1cfc41a&title=)

- 设置内存、处理器个数

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896125610-a6d5ab9f-6654-45c3-a1c1-952a95dff728.png#averageHue=%23a7bfd6&clientId=ue4588d76-548e-4&from=paste&id=u025d103b&originHeight=555&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u50317864-b9eb-4d88-89ad-20ee7001f4a&title=)

- 设置磁盘空间

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896125682-ddccea1b-d095-4a3f-8cb5-bd252e3ce5f7.png#averageHue=%238aa9c6&clientId=ue4588d76-548e-4&from=paste&id=u1ce7183a&originHeight=550&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u75013620-b794-4eaf-813c-a4726642b69&title=)

- 挂载光驱文件

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896132568-ec90dd5a-9251-4239-8ef0-3e0d1a29ef3a.png#averageHue=%238cbce2&clientId=ue4588d76-548e-4&from=paste&id=u1ae56a98&originHeight=547&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udceddcb9-32a2-41d9-9f2e-f8951f0861a&title=)

- 启动虚拟机

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896133035-5d237da9-3355-40c3-804c-ef294c184d77.png#averageHue=%238dbee6&clientId=ue4588d76-548e-4&from=paste&id=u00009cc6&originHeight=458&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0fcc9749-12be-4d19-905e-4d7702ff5ca&title=)
<a name="WKgyI"></a>
## 4、安装RockyLinux

- 选择语言

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896133061-dc14df8e-f6ac-4e68-b08c-12f44bea1035.png#averageHue=%231d557b&clientId=ue4588d76-548e-4&from=paste&id=u2dcc6aa0&originHeight=773&originWidth=1012&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucd0e1284-95f5-4cce-a3f0-2310f62b195&title=)

- 选择安装的硬盘和设置root用户的密码

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896132929-7960b291-660a-4350-bb7e-067b47fe4ad5.png#averageHue=%23f3f2ef&clientId=ue4588d76-548e-4&from=paste&id=ubaecab78&originHeight=585&originWidth=824&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u022ac539-e310-4019-9b16-beabd9a58c5&title=)<br />选择硬盘<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896132965-5c65b6d6-992f-41cc-9400-9893e6bfc4a9.png#averageHue=%23f4f3f2&clientId=ue4588d76-548e-4&from=paste&id=u2ed6eb57&originHeight=598&originWidth=795&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u05ff0925-653e-44e6-b56d-64861538768&title=)<br />设置密码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896133354-5d6bb99c-023a-49dd-9013-3369c95e5a3d.png#averageHue=%23285b76&clientId=ue4588d76-548e-4&from=paste&id=u5dcb2757&originHeight=629&originWidth=829&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6897f15e-445c-467b-a299-ea4251f0e2e&title=)
<a name="z1YKA"></a>
## 5、VirtualBox安装增强功能
去VirtualBox官网下载文件：VBoxGuestAdditions_7.0.6.iso。文件名中的 **7.0.6** 是VirtuaBox的版本号，可以选择和原来的VirtuaBox的版本号保持一致的。<br />[https://download.virtualbox.org/virtualbox](https://download.virtualbox.org/virtualbox)<br />文件VBoxGuestAdditions_7.0.6.iso下载到本地以后，进入Virtual Box，进行如下图的操作：<br />设置->存储->控制器：IDE->蓝色齿轮->选择虚拟盘->选择刚刚下载好的VBoxGuestAdditions_7.0.6.iso<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896133673-71dd1663-6f54-40fc-b918-c79ec2590639.png#averageHue=%2384afd1&clientId=ue4588d76-548e-4&from=paste&id=u33dd8341&originHeight=654&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud5bdd034-2eee-4c1f-ae27-51965b6dbc0&title=)<br />启动Linux找到挂载的VBoxGuestAdditions_7.0.6.iso<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896133818-332e96d4-3fc6-4337-a97b-3fe457fefcfb.png#averageHue=%23a3b1a9&clientId=ue4588d76-548e-4&from=paste&id=u5a3fc311&originHeight=622&originWidth=814&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u16fc5388-cd3a-457c-bb7c-5b3ae6b2b08&title=)<br />在终端进入该目录，执行 VBoxLinuxAdditions.run，切换到root用户，执行命令如下：
```bash
cd /run/media/huangjinjin/VBox_GAs_7.0.6
sudo ./VBoxLinuxAdditions.run
```
<a name="bH4ee"></a>
## 6、网络配置

- VirtualBox设置网络为桥接 在VirtualBox选中安装的RockyLinux系统，点击设置，再选择网络，勾选“启用网络连接”，并在连接方式中选择“桥接网卡”

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896134043-bfc274bf-2c0d-4772-a124-abfd129d5c5b.png#averageHue=%2380b2d8&clientId=ue4588d76-548e-4&from=paste&id=ub4c7ffaf&originHeight=537&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uad6dc817-cd54-4969-aa3d-3758a76dee2&title=)

- linux中设置ip，子网掩码，网关

打开文件(如果ifcfg-enp0s3不存在直接创建)
```bash
vi /etc/sysconfig/network-scripts/ifcfg-enp0s3
```
```
DEVICE=enp0s3 #网卡名称，必须和ifcfg-eth0后面的eth0一样
HWADDR=08:00:27:77:AE:95 #网卡的MAC地址，默认的
TYPE=Ethernet #类型
UUID=c031fded-f139-4751-9357-d873107480ed #uuid，不重要
ONBOOT=yes #是否默认启动此接口的意思，填yes
NM_CONTROLLED=yes #是否接受其他软件的网络管理
BOOTPROTO=statics #ip获取的方式，填static时需要手动设置
IPADDR=192.168.10.108 #设置的ip地址
NETMASK=255.255.255.0 #设置的子网掩码
GATEWAY=192.168.10.1 #设置的默认网管
```
重点关注网关（GATEWAT），可以看到和主机网关一致（若不一致则修改为一致）：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676896134287-b4ea05c0-b867-4805-b115-d4dfeccb7b52.png#averageHue=%2356565a&clientId=ue4588d76-548e-4&from=paste&id=u9d416822&originHeight=560&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufdafcdaf-b51f-484a-9aad-bd937c91008&title=)<br />需要注意的地方，此处的IPADDR，NERTMASK, GATEWAY需要跟Windows系统设置的ip相对向，所以需要查看win的网络设置，进行设置。

- 重启网络
<a name="pwyQE"></a>
### 查看网络状态
```bash
systemctl status NetworkManager
```
<a name="iCyIR"></a>
### 开机启动网络
```bash
systemctl enable NetworkManager
```
<a name="xBGMR"></a>
### 取消开机启动网络
```bash
systemctl disable NetworkManager
```
<a name="VsnUj"></a>
### 开启网络
```bash
systemctl start NetworkManager
```
<a name="ByowR"></a>
### 重启网络
```bash
systemctl restart NetworkManager
```
<a name="VtAal"></a>
### 关闭网络
```bash
systemctl stop NetworkManager
```
<a name="R0oYA"></a>
## 7、遇到的问题

- 报cdrom被占用，这个时候需要将当期的虚拟光盘中的盘片清除，也就是取消勾选。
- 再次点击安装增强，如果提示无法打开virtualbox下面的一个xxx.iso的话，去网站上搜索对应virtualbox版本的缺失的这个xxx.iso，并放到提示的目录下。
- 再次点击安装增强，如不提示错误，证明安装成功了
- 以linux系统为例，需要把光盘中的内容mount到可以操作的文件夹下，比如在/tmp/下**以下命令都在root用户下操作**
```bash
cd /tmp
mkdir cdrom
```
创建一个cdrom的文件夹，然后使用命令：
```bash
mount /dev/cdrom cdrom
```
然后`cd cdrom`到 cdrom 文件下，执行
```bash
./VBoxLinuxAdditions.run
```

- 如果出现
```
kernel headers not found for target kernel
```
需要执行
```bash
yum update kernel -y
yum install kernel-headers kernel-devel gcc make -y
```
然后执行重启
```bash
reboot
```

- 再次执行1~4步骤，如果还有问题比如
```
“VirtualBox Guest Additions: Kernel headers not found for target kernel
4.19.0-6-amd64. Please install them and execute
/sbin/rcvboxadd setup”
```
改完之后日志里面没有错，输出的结果里只剩下一个挂载失败：
```
ValueError: File context for /opt/VBoxGuestAdditions-6.30.1/other/mount.vboxsf already defined
```
在root用户下执行：
```bash
semanage fcontext -d /opt/VBoxGuestAdditions-/other/mount.vboxsf
restorecon /opt/VBoxGuestAdditions-/other/mount.vboxsf
```
然后重启
```bash
reboot
```
再重复1~4的操作即可。
