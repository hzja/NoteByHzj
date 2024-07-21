# machines

网址：https://app.hackthebox.com/machines

1.这是没有登录的状态，没有数据

![连接靶机1](./img/连接靶机1.PNG)

2.查看Machines的状态，现在是离线状态

![连接靶机2](./img/连接靶机2.PNG)

3. 通过OpenVPN下载登录用VPN

![连接靶机3](./img/连接靶机3.PNG)

4.选择下载节点和服务器

![_](./img/连接靶机4.PNG)

5.下载文件后拉文件到kali_linux文件夹

![连接靶机5](./img/连接靶机5.PNG)

6.在vpn文件的文件夹运行terminal执行以下命令 

~~~ shell
openvpn vpn文件名
~~~

![连接靶机6](./img/连接靶机6.PNG)



7.执行后出现

~~~ shell
Initialization Sequence Completed
~~~

说明连接成功，重新打开https://app.hackthebox.com/machines发现已经登录成功。

![连接靶机7](./img/连接靶机7.PNG)