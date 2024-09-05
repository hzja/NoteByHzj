Linux SSH<br />如果希望在OpenSSH (SSHD服务器)身份验证之前显示消息或横幅?尝试使用这个简单的技巧在Linux系统上显示Banner或ASCII艺术字。OpenSSH有一个名为Banner的内置选项。在允许身份验证之前，将指定文件的内容发送给远程用户。如果Banner选项设置为none，那么在ssh登录时就不会显示任何Banner消息。默认情况下，不显示横幅，并且禁用该选项。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605577885562-ea90784e-4a18-49f5-ab9e-c639ef5e43ac.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2183544&status=done&style=none&width=1107.6666666666667)
<a name="aa66a59b"></a>
### 开启banner选项
1、登录到远程的Linux操作系统。<br />2、编辑`/etc/ssh/sshd_config`配置文件：
```bash
[root@localhost ~]# vim /etc/ssh/sshd_config
```
3、在配置文件末尾添加`Banner /etc/ssh/my_banner`这一行内容：
```bash
Banner /etc/ssh/my_banner
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605577933463-8929e77f-02a5-40db-97e5-1d9590fc4152.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2167167&status=done&style=none&width=1107.6666666666667)<br />4、保存配置文件，并退出编辑。<br />5、创建`/etc/ssh/my_banner`文件：
```bash
[root@localhost ~]# touch /etc/ssh/my_banner
```
6、重启sshd服务：
```bash
[root@localhost ~]# systemctl restart sshd
```
<a name="17dMb"></a>
### 生成ASCII Art图案
可以网上搜索ASCII art，会有许多生成ascii 艺术字的网站。还可以将图片转换成ASCII类型的。<br />另一种方式是使用`figlet`命令生成普通字符的放大版。在centos中需要安装epel源，然后安装figlet程序：
```bash
[root@localhost ~]# yum -y install figlet
[root@localhost ~]# figlet Fcant Welcome
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605578071345-4a33f9c3-2271-46c3-aaa6-6e9fb8ba8305.png#align=left&display=inline&height=153&originHeight=458&originWidth=3323&size=493791&status=done&style=none&width=1107.6666666666667)<br />可以看到生成的文字。可以将figlet生成的艺术字体写入`/etc/ssh/my_banner`文件中：
```bash
[root@localhost ~]# figlet Fcant Welcome > /etc/ssh/my_banner
```
现在可以ssh登录linux系统时就可以看到提示信息了：
```bash
[root@localhost ~]# ssh root@localhost
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1605578181763-a48ba34e-41f8-4722-b225-52b7755b1c13.png#align=left&display=inline&height=334&originHeight=1003&originWidth=3323&size=1232029&status=done&style=none&width=1107.6666666666667)
