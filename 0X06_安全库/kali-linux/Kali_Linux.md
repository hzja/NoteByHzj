# 基础命令

~~~shell
sudo
~~~

 Linux 和 Unix 系统上以超级用户（root）权限执行命令，允许授权用户临时提升权限执行需要特权的操作，如安装软件、修改系统配置等



~~~ shell
sudo -i 
~~~

切换root用户



~~~ shell
sudo -l
~~~

查看当前用户的sudo权限



~~~ shell
uname 
~~~

用于获取当前操作系统



~~~shell
用法：uname [选项]...
输出特定的系统信息。如果不带 <选项>，则视为使用了 -s 选项。

  -a, --all                按如下次序输出所有信息，其中若 -p 和 -i 的
                             探测结果为未知，则省略：
  -s, --kernel-name        输出内核名称
  -n, --nodename           输出网络节点的主机名
  -r, --kernel-release     输出内核发行号
  -v, --kernel-version     输出内核版本号
  -m, --machine            输出主机的硬件架构名称
  -p, --processor          输出处理器类型（不可移植）
  -i, --hardware-platform  输出硬件平台（不可移植）
  -o, --operating-system   输出操作系统名称
      --help        显示此帮助信息并退出
      --version     显示版本信息并退出
~~~



## tar.gz是什么？

以·tar.gz为后缀的文件是一种[压缩文件](https://so.csdn.net/so/search?q=压缩文件&spm=1001.2101.3001.7020)，在Linux和macOS下常见，Linux和macOS都可以直接解压使用这种压缩文件



~~~ shell
tar -zxvf data.tar.gz
运行命令后会将data.tar.gz解压到当前目录下

-z: 使用gzip
-c: 压缩
-x: 解压
-v: 显示解压压缩的文件列表
-f: 指定文件
~~~



~~~ shell
tar -zcvf data.tar.gz data
运行后会将指定文件或者文件夹data，压缩后保存成data.tar.gz
~~~



## zip格式压缩与解压

### 安装

~~~ shell
yum install -y unzip
~~~



### 使用

#### 压缩

常见用法有

~~~ shell
zip -q -r 1.zip 1.txt
~~~

将当前目录下的1.txt压缩为1.zip。选项-q表示忽略执行过程，-r表示递归处理，将指定目录下的所有文件和子目录一并处理



~~~ shell
zip -q -r 1.zip ../1.txt
~~~

将上级目录中的1.txt压缩为1.zip



#### 解压

假设当前目录有一个压缩包为1.zip

常见的用法有

~~~ shell
unzip 1.zip
~~~

直接解压到当前目录下



~~~ shell
unzip 1.zip -d folder
~~~

通过-d指定解压路径，即解压到当前目录下folder这个文件夹下，如果这个文件夹不存在，可以自动创建



查看压缩文件

~~~ shell
zip -sf 1.zip
~~~

列出压缩文件1.zip中的内容



## 查看IP地址

+ ifconfig

~~~ shell
──(root㉿kali)-[~]
└─# ifconfig
br-021434f21f4b: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.20.0.1  netmask 255.255.0.0  broadcast 172.20.255.255
        ether 02:42:1f:b3:9a:4d  txqueuelen 0  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

br-6cd91058bdf2: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.19.0.1  netmask 255.255.0.0  broadcast 172.19.255.255
        ether 02:42:e9:18:b0:4e  txqueuelen 0  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

br-7be9904d216e: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.18.0.1  netmask 255.255.0.0  broadcast 172.18.255.255
        ether 02:42:ce:47:c9:40  txqueuelen 0  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

docker0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 172.17.0.1  netmask 255.255.0.0  broadcast 172.17.255.255
        ether 02:42:46:53:fe:60  txqueuelen 0  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.164.129  netmask 255.255.255.0  broadcast 192.168.164.255
        inet6 fe80::20c:29ff:fe63:6bb5  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:63:6b:b5  txqueuelen 1000  (Ethernet)
        RX packets 44206  bytes 49030638 (46.7 MiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 12694  bytes 1366744 (1.3 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 108  bytes 6688 (6.5 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 108  bytes 6688 (6.5 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
~~~



+ ip add

~~~ shell
┌──(root㉿kali)-[~]
└─# ip add  
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 00:0c:29:63:6b:b5 brd ff:ff:ff:ff:ff:ff
    inet 192.168.164.129/24 brd 192.168.164.255 scope global dynamic noprefixroute eth0
       valid_lft 1585sec preferred_lft 1585sec
    inet6 fe80::20c:29ff:fe63:6bb5/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
3: br-021434f21f4b: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:1f:b3:9a:4d brd ff:ff:ff:ff:ff:ff
    inet 172.20.0.1/16 brd 172.20.255.255 scope global br-021434f21f4b
       valid_lft forever preferred_lft forever
4: br-6cd91058bdf2: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:e9:18:b0:4e brd ff:ff:ff:ff:ff:ff
    inet 172.19.0.1/16 brd 172.19.255.255 scope global br-6cd91058bdf2
       valid_lft forever preferred_lft forever
5: br-7be9904d216e: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:ce:47:c9:40 brd ff:ff:ff:ff:ff:ff
    inet 172.18.0.1/16 brd 172.18.255.255 scope global br-7be9904d216e
       valid_lft forever preferred_lft forever
6: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default 
    link/ether 02:42:46:53:fe:60 brd ff:ff:ff:ff:ff:ff
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
       valid_lft forever preferred_lft forever
                                                  
~~~

