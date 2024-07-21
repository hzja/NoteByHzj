#  starting point

网址：https://app.hackthebox.com/starting-point

1. 注册用户
2. 登录用户
3. 进入网站首页

![login_html](.\img\login_html.PNG)



4.下载VPN

4.1. 选择第二个"Starting Point"

![select靶机](.\img\select靶机.PNG)



4.2.选择第一个"OpenVPN",这个不收费而第二个要收费

![select靶机3](.\img\select靶机3.PNG)

4.3.随便选一个VPN节点和服务器下载VPN文件

![select靶机4](.\img\select靶机4.PNG)

4.4.已经下载到靶机

![select靶机5](.\img\select靶机5.PNG)



5. 将文件从本地传入虚拟机中

5.1.安装VMWares Tools

![传输文件到虚拟机1](.\img\传输文件到虚拟机1.PNG)

虚拟机关机状态下发现 ”安装 VMware Tools“ 按钮是灰色无法点击



5.2.重新设置虚拟机的”CD/DVD(IDE)"选项选择“使用 ISO 映像文件”后点确定

![传输文件到虚拟机2](.\img\传输文件到虚拟机2.PNG)



5.3.登录到系统之前重新在第一步中的”安装 VMware Tools“上点击

![传输文件到虚拟机3](.\img\传输文件到虚拟机3.PNG)



5.4.登录到系统后发现第一步中相同的位置已经变成了“取消 VMware Tools 安装”,说实话这里我有点莫名其妙，看不到后台干了啥![传输文件到虚拟机4](.\img\传输文件到虚拟机4.PNG)



5.5将文件直接从本机中拉入虚拟机中，发现文件直接出现在桌面

![传输文件到虚拟机4](.\img\传输文件到虚拟机4.PNG)



5.6.打开终端，切换root权限，用openvpn打开文件：

![连接靶机1](.\img\连接靶机1.PNG)



6.连接靶机

6.1.打开终端，切换root权限，用openvpn命令打开文件

![连接靶机1](.\img\连接靶机1.PNG)



6.2.回车后执行到下面这句话就已经成功

![连接靶机2](./img/连接靶机2.PNG)



6.3.到hack the box网站上看已经登录成功了。

![连接靶机3](./img/连接靶机3.PNG)

7.过关，攻击第一个靶机

7.1.问题1

![攻击第一个靶机1](./img/攻击第一个靶机1.PNG)



7.2.问题2

![攻击第一个靶机2](./img/攻击第一个靶机2.PNG)



7.3.问题3

![攻击第一个靶机3](./img/攻击第一个靶机3.PNG)



7.4.问题4

![攻击第一个靶机4](./img/攻击第一个靶机4.PNG)



7.5.问题5

![攻击第一个靶机5](./img/攻击第一个靶机5.PNG)



7.6.问题6

![攻击第一个靶机6](./img/攻击第一个靶机6.PNG)



7.7.问题7

![攻击第一个靶机7](./img/攻击第一个靶机7.PNG)



7.8.问题8

![攻击第一个靶机8](./img/攻击第一个靶机8.PNG)

这里让提交一个flag，实际上是让用kali linux结合刚刚学习到的知识去攻击第一台靶机。



在kali linux上执行以下命令：

~~~ shell
telnet target ip
~~~

![攻击第一个靶机9](./img/攻击第一个靶机9.PNG)



~~~ shell
Meow login: root //root回车，免密码登录
~~~

![攻击第一个靶机10](./img/攻击第一个靶机10.PNG)



~~~ shell
root@Meow:~# ls
flag.txt  snap
root@Meow:~# cat flag.txt
b40abdfe23665f766f9c61ecba8a4c19
~~~

![攻击第一个靶机11](./img/攻击第一个靶机11.PNG)