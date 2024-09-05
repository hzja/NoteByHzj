tcpdump<br />tcpdump采用命令行方式对接口的数据包进行筛选抓取，其丰富特性表现在灵活的表达式上。<br />使用tcpdump的最基本方法是简单地发出以下命令：
```bash
tcpdump
```
可以使用-v选项获得更多详细信息，而-vv可以获取更多信息。<br />不带任何选项的tcpdump，默认会抓取第一个网络接口，且只有将tcpdump进程终止才会停止抓包。<br />例如：
```bash
shell> tcpdump -nn -i eth0 icmp
```
<a name="1jFMJ"></a>
## 1、命令参数选项
```bash
tcpdump [ -DenNqvX ] [ -c count ] [ -F file ] [ -i interface ] [ -r file ]
        [ -s snaplen ] [ -w file ] [ expression ]

抓包选项：
-c：指定要抓取的包数量。注意，是最终要获取这么多个包。例如，指定"-c 10"将获取10个包，但可能已经处理了100个包，只不过只有10个包是满足条件的包。
-i interface：指定tcpdump需要监听的接口。若未指定该选项，将从系统接口列表中搜寻编号最小的已配置好的接口(不包括loopback接口，要抓取loopback接口使用tcpdump -i lo)，
            ：一旦找到第一个符合条件的接口，搜寻马上结束。可以使用'any'关键字表示所有网络接口。
-n：对地址以数字方式显式，否则显式为主机名，也就是说-n选项不做主机名解析。
-nn：除了-n的作用外，还把端口显示为数值，否则显示端口服务名。
-N：不打印出host的域名部分。例如tcpdump将会打印'nic'而不是'nic.ddn.mil'。
-P：指定要抓取的包是流入还是流出的包。可以给定的值为"in"、"out"和"inout"，默认为"inout"。
-s len：设置tcpdump的数据包抓取长度为len，如果不设置默认将会是65535字节。对于要抓取的数据包较大时，长度设置不够可能会产生包截断，若出现包截断，
      ：输出行中会出现"[|proto]"的标志(proto实际会显示为协议名)。但是抓取len越长，包的处理时间越长，并且会减少tcpdump可缓存的数据包的数量，
      ：从而会导致数据包的丢失，所以在能抓取想要的包的前提下，抓取长度越小越好。

输出选项：
-e：输出的每行中都将包括数据链路层头部信息，例如源MAC和目标MAC。
-q：快速打印输出。即打印很少的协议相关信息，从而输出行都比较简短。
-X：输出包的头部数据，会以16进制和ASCII两种方式同时输出。
-XX：输出包的头部数据，会以16进制和ASCII两种方式同时输出，更详细。
-v：当分析和打印的时候，产生详细的输出。
-vv：产生比-v更详细的输出。
-vvv：产生比-vv更详细的输出。

其他功能性选项：
-D：列出可用于抓包的接口。将会列出接口的数值编号和接口名，它们都可以用于"-i"后。
-F：从文件中读取抓包的表达式。若使用该选项，则命令行中给定的其他表达式都将失效。
-w：将抓包数据输出到文件中而不是标准输出。可以同时配合"-G time"选项使得输出文件每time秒就自动切换到另一个文件。可通过"-r"选项载入这些文件以进行分析和打印。
-r：从给定的数据包文件中读取数据。使用"-"表示从标准输入中读取。
```
常用参数选项说明：

| 参数 | 含义 |
| --- | --- |
| -a | 将网络地址和广播地址转变成名字 |
| -c | 在收到指定的包的数目后，tcpdump就会停止； |
| -d | 将匹配信息包的代码以人们能够理解的汇编格式给出；以可阅读的格式输出。 |
| -dd | 将匹配信息包的代码以c语言程序段的格式给出； |
| -ddd | 将匹配信息包的代码以十进制的形式给出； |
| -e | 在输出行打印出数据链路层的头部信息； |
| -f | 将外部的Internet地址以数字的形式打印出来； |
| -l | 使标准输出变为缓冲行形式； |
| -n | 直接显示IP地址，不显示名称； |
| -nn | 端口名称显示为数字形式，不显示名称； |
| -t | 在输出的每一行不打印时间戳； |
| -v | 输出一个稍微详细的信息，例如在ip包中可以包括ttl和服务类型的信息； |
| -vv | 输出详细的报文信息； |
| -F | 从指定的文件中读取表达式,忽略其它的表达式； |
| -i | 指定监听的网络接口； |
| -r | 从指定的文件中读取包(这些包一般通过-w选项产生)； |
| -w | 直接将抓取的数据包写入文件中，并不分析和打印出来； |
| -T | 将监听到的包直接解释为指定的类型的报文，常见的类型有rpc （远程过程调用）和snmp（简单 网络管理协议；） |

常用的选项也就这几个：

- tcpdump -D<br />
- tcpdump -c num -i int -nn -XX -vvv<br />

假设已登录到要管理的远程计算机。如果不带任何选项运行“ tcpdump”，则输出将被来自SSH连接的数据包淹没。为避免这种情况，只需从输出中消除端口22：
```bash
tcpdump not port 22
```
可以使用许多不同的端口来执行此操作：
```bash
tcpdump not port 143 and not port 25 and not port 22
```
如果要进行相反的操作，即仅监视某个端口（这对于调试网络应用程序非常有用），则可以执行以下操作：
```bash
tcpdump port 143
```
还可以从网络上的特定主机获取数据：
```bash
tcpdump host hal9000
```
如果计算机具有多个网络接口，则还可以指定要收听的网络接口：
```bash
tcpdump -i eth1
```
还可以指定协议：
```bash
tcpdump udp
```
可以在/ etc / protocols中找到协议列表。
<a name="ZazQX"></a>
## 2、tcpdump表达式
表达式用于筛选输出哪些类型的数据包，如果没有给定表达式，所有的数据包都将输出，否则只输出表达式为true的包。在表达式中出现的shell元字符建议使用单引号包围。<br />tcpdump的表达式由一个或多个"单元"组成，每个单元一般包含ID的修饰符和一个ID(数字或名称)。有三种修饰符：
<a name="dF3Io"></a>
### (1) type：指定ID的类型。
可以给定的值有host/net/port/portrange。例如"host foo"，"net 128.3"，"port 20"，"portrange 6000-6008"。默认的type为host。
<a name="0gQnU"></a>
### (2) dir：指定ID的方向。
可以给定的值包括src/dst/src or dst/src and dst，默认为src or dst。例如，"src foo"表示源主机为foo的数据包，"dst net 128.3"表示目标网络为128.3的数据包，"src or dst port 22"表示源或目的端口为22的数据包。
<a name="KyXvf"></a>
### (3) proto：通过给定协议限定匹配的数据包类型。
常用的协议有tcp/udp/arp/ip/ether/icmp等，若未给定协议类型，则匹配所有可能的类型。例如"tcp port 21"，"udp portrange 7000-7009"。<br />所以，一个基本的表达式单元格式为"proto dir type ID"<br />![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1605771918575-9b8b15df-b95f-4034-8f45-d84e2da7d6ab.webp#align=left&display=inline&height=250&originHeight=250&originWidth=904&size=0&status=done&style=shadow&width=904)<br />除了使用修饰符和ID组成的表达式单元，还有关键字表达式单元：gateway，broadcast，less，greater以及算术表达式。<br />表达式单元之间可以使用操作符" and / && / or / || / not / ! "进行连接，从而组成复杂的条件表达式。如"host foo and not port ftp and not port ftp-data"，这表示筛选的数据包要满足"主机为foo且端口不是ftp(端口21)和ftp-data(端口20)的包"，常用端口和名字的对应关系可在linux系统中的/etc/service文件中找到。<br />另外，同样的修饰符可省略，如"tcp dst port ftp or ftp-data or domain"与"tcp dst port ftp or tcp dst port ftp-data or tcp dst port domain"意义相同，都表示包的协议为tcp且目的端口为ftp或ftp-data或domain(端口53)。<br />使用括号"()"可以改变表达式的优先级，但需要注意的是括号会被shell解释，所以应该使用反斜线"\"转义为"\(\)"，在需要的时候，还需要包围在引号中。
<a name="XrYzB"></a>
## 3、理解 tcpdump 的输出
<a name="QBI1H"></a>
### 3.1 输出内容结构
tcpdump 输出的内容虽然多，却很规律。<br />这里以随便抓取的一个 tcp 包为例来看一下
```
21:26:49.013621 IP 172.20.20.1.15605 > 172.20.20.2.5920: Flags [P.], seq 49:97, ack 106048, win 4723, length 48
```
从上面的输出来看，可以总结出：

1. 第一列：时分秒毫秒 21:26:49.013621<br />
2. 第二列：网络协议 IP<br />
3. 第三列：发送方的ip地址+端口号，其中172.20.20.1是 ip，而15605 是端口号<br />
4. 第四列：箭头 >， 表示数据流向<br />
5. 第五列：接收方的ip地址+端口号，其中 172.20.20.2 是 ip，而5920 是端口号<br />
6. 第六列：冒号<br />
7. 第七列：数据包内容，包括Flags 标识符，seq 号，ack 号，win 窗口，数据长度 length，其中 [P.] 表示 PUSH 标志位为 1，更多标识符见下面<br />
<a name="sTwsC"></a>
### 3.2 Flags 标识符
使用 tcpdump 抓包后，会遇到的 TCP 报文 Flags，有以下几种：

- `[S]` : SYN（开始连接）<br />
- `[P]` : PSH（推送数据）<br />
- `[F]` : FIN （结束连接）<br />
- `[R]` : RST（重置连接）<br />
- `[.]` : 没有 Flag，由于除了 SYN 包外所有的数据包都有ACK，所以一般这个标志也可表示 ACK<br />
<a name="uyGt9"></a>
## 4. 常规过滤规则
<a name="PB0HC"></a>
### 4.1 基于IP地址过滤：host
使用 `host` 就可以指定 host ip 进行过滤
```bash
$ tcpdump host 192.168.10.100
```
数据包的 ip 可以再细分为源ip和目标ip两种
```bash
# 根据源ip进行过滤
$ tcpdump -i eth2 src 192.168.10.100
# 根据目标ip进行过滤
$ tcpdump -i eth2 dst 192.168.10.200
```
<a name="39qAf"></a>
### 4.2 基于网段进行过滤：net
若ip范围是一个网段，可以直接这样指定
```bash
$ tcpdump net 192.168.10.0/24
```
网段同样可以再细分为源网段和目标网段
```bash
# 根据源网段进行过滤
$ tcpdump src net 192.168
# 根据目标网段进行过滤
$ tcpdump dst net 192.168
```
<a name="qGPva"></a>
### 3.3 基于端口进行过滤：port
使用 `port` 就可以指定特定端口进行过滤
```bash
$ tcpdump port 8088
```
端口同样可以再细分为源端口，目标端口
```bash
# 根据源端口进行过滤
$ tcpdump src port 8088
# 根据目标端口进行过滤
$ tcpdump dst port 8088
```
如果想要同时指定两个端口可以这样写
```bash
$ tcpdump port 80 or port 8088
```
但也可以简写成这样
```bash
$ tcpdump port 80 or 8088
```
如果想抓取的不再是一两个端口，而是一个范围，一个一个指定就非常麻烦了，此时可以这样指定一个端口段。
```
$ tcpdump portrange 8000-8080
$ tcpdump src portrange 8000-8080
$ tcpdump dst portrange 8000-8080
```
对于一些常见协议的默认端口，还可以直接使用协议名，而不用具体的端口号<br />比如 http  == 80，https == 443 等
```bash
$ tcpdump tcp port http
```
<a name="eWeOk"></a>
### 4.4 基于协议进行过滤：proto
常见的网络协议有：tcp, udp, icmp, http, ip,ipv6 等<br />若只想查看 icmp 的包，可以直接这样写
```bash
$ tcpdump icmp
```
protocol 可选值：ip, ip6, arp, rarp, atalk, aarp, decnet, sca, lat, mopdl,  moprc,  iso,  stp, ipx,  or  netbeui
<a name="q6GvG"></a>
### 4.5 基本IP协议的版本进行过滤
当想查看 tcp 的包，也许会这样写
```bash
$ tcpdump tcp
```
这样子写也没问题，就是不够精准，为什么这么说呢？<br />ip 根据版本的不同，可以再细分为 IPv4 和 IPv6 两种，如果只指定了 tcp，这两种其实都会包含在内。<br />那有什么办法，能够将 IPv4 和 IPv6 区分开来呢？<br />很简单，如果是 IPv4 的 tcp 包 ，就这样写（友情提示：数字 6 表示的是 tcp 在ip报文中的编号。）
```bash
$ tcpdump 'ip proto tcp'
# or
$ tcpdump ip proto 6
# or
$ tcpdump 'ip protochain tcp'
# or 
$ tcpdump ip protochain 6
```
而如果是 IPv6 的 tcp 包 ，就这样写
```bash
$ tcpdump 'ip6 proto tcp'
# or
$ tcpdump ip6 proto 6
# or
$ tcpdump 'ip6 protochain tcp'
# or 
$ tcpdump ip6 protochain 6
```
关于上面这几个命令示例，有两点需要注意：

1. 跟在 proto 和 protochain 后面的如果是 tcp, udp, icmp ，那么过滤器需要用引号包含，这是因为 tcp,udp, icmp 是 tcpdump 的关键字。<br />
2. 跟在ip 和 ip6 关键字后面的 proto 和 protochain 是两个新面孔，看起来用法类似，它们是否等价，又有什么区别呢？<br />

关于第二点，网络上没有找到很具体的答案，只能通过 `man tcpdump` 的提示。<br />proto 后面跟的 `<protocol>` 的关键词是固定的，只能是 ip, ip6, arp, rarp, atalk, aarp, decnet, sca, lat, mopdl,  moprc,  iso,  stp, ipx,  or  netbeui 这里面的其中一个。<br />而 protochain 后面跟的 protocol 要求就没有那么严格，它可以是任意词，只要 tcpdump 的 IP 报文头部里的 protocol 字段为 `<protocol>` 就能匹配上。<br />理论上来讲，下面两种写法效果是一样的
```bash
$ tcpdump 'ip && tcp'
$ tcpdump 'ip proto tcp'
```
同样的，这两种写法也是一样的
```bash
$ tcpdump 'ip6 && tcp'
$ tcpdump 'ip6 proto tcp'
```
<a name="glkyK"></a>
## 5. 可选参数解析
<a name="XMZ3D"></a>
### 5.1 设置不解析域名提升速度

- `-n`：不把ip转化成域名，直接显示  ip，避免执行 DNS lookups 的过程，速度会快很多<br />
- `-nn`：不把协议和端口号转化成名字，速度也会快很多。<br />
- `-N`：不打印出host 的域名部分.。比如,，如果设置了此选现，tcpdump 将会打印'nic' 而不是 'nic.ddn.mil'.<br />
<a name="tPgfD"></a>
### 5.2 过滤结果输出到文件
使用 tcpdump 工具抓到包后，往往需要再借助其他的工具进行分析，比如常见的 wireshark 。<br />而要使用wireshark ，得将 tcpdump 抓到的包数据生成到文件中，最后再使用 wireshark 打开它即可。<br />使用 `-w` 参数后接一个以 `.pcap` 后缀命令的文件名，就可以将 tcpdump 抓到的数据保存到文件中。
```bash
$ tcpdump icmp -w icmp.pcap
```
<a name="nb7Bm"></a>
### 5.3 从文件中读取包数据
使用 `-w` 是写入数据到文件，而使用 `-r` 是从文件中读取数据。<br />读取后，照样可以使用上述的过滤器语法进行过滤分析。
```bash
$ tcpdump icmp -r all.pcap
```
<a name="UJ48O"></a>
### 5.4 控制详细内容的输出

- `-v`：产生详细的输出. 比如包的TTL，id标识，数据包长度，以及IP包的一些选项。同时它还会打开一些附加的包完整性检测，比如对IP或ICMP包头部的校验和。<br />
- `-vv`：产生比-v更详细的输出. 比如NFS回应包中的附加域将会被打印, SMB数据包也会被完全解码。<br />
- `-vvv`：产生比-vv更详细的输出。比如 telent 时所使用的SB, SE 选项将会被打印, 如果telnet同时使用的是图形界面，其相应的图形选项将会以16进制的方式打印出来。<br />
<a name="FQBSw"></a>
### 5.5 控制时间的显示

- `-t`：在每行的输出中不输出时间<br />
- `-tt`：在每行的输出中会输出时间戳<br />
- `-ttt`：输出每两行打印的时间间隔(以毫秒为单位)<br />
- `-tttt`：在每行打印的时间戳之前添加日期的打印（此种选项，输出的时间最直观）<br />
<a name="96D1V"></a>
### 5.6 显示数据包的头部

- `-x`：以16进制的形式打印每个包的头部数据（但不包括数据链路层的头部）<br />
- `-xx`：以16进制的形式打印每个包的头部数据（包括数据链路层的头部）<br />
- `-X`：以16进制和 ASCII码形式打印出每个包的数据(但不包括连接层的头部)，这在分析一些新协议的数据包很方便。<br />
- `-XX`：以16进制和 ASCII码形式打印出每个包的数据(包括连接层的头部)，这在分析一些新协议的数据包很方便。<br />
<a name="banyD"></a>
### 5.7 过滤指定网卡的数据包

- `-i`：指定要过滤的网卡接口，如果要查看所有网卡，可以 `-i any`<br />
<a name="unsys"></a>
### 5.8 过滤特定流向的数据包

- `-Q`：选择是入方向还是出方向的数据包，可选项有：in, out, inout，也可以使用  --direction=[direction] 这种写法<br />
<a name="qsxq0"></a>
### 5.9 其他常用的一些参数

- `-A`：以ASCII码方式显示每一个数据包(不显示链路层头部信息). 在抓取包含网页数据的数据包时, 可方便查看数据<br />
- `-l` : 基于行的输出，便于保存查看，或者交给其它工具分析<br />
- `-q` : 简洁地打印输出。即打印很少的协议相关信息, 从而输出行都比较简短.<br />
- `-c` : 捕获 count 个包 tcpdump 就退出<br />
- `-s` :  tcpdump 默认只会截取前 `96` 字节的内容，要想截取所有的报文内容，可以使用 `-s number`， `number` 就是要截取的报文字节数，如果是 0 的话，表示截取报文全部内容。<br />
- `-S` : 使用绝对序列号，而不是相对序列号<br />
- `-C`：file-size，tcpdump 在把原始数据包直接保存到文件中之前, 检查此文件大小是否超过file-size. 如果超过了, 将关闭此文件,另创一个文件继续用于原始数据包的记录. 新创建的文件名与-w 选项指定的文件名一致, 但文件名后多了一个数字.该数字会从1开始随着新创建文件的增多而增加. file-size的单位是百万字节(nt: 这里指1,000,000个字节,并非1,048,576个字节, 后者是以1024字节为1k, 1024k字节为1M计算所得, 即1M=1024 ＊ 1024 ＝ 1,048,576)<br />
- `-F`：使用file 文件作为过滤条件表达式的输入, 此时命令行上的输入将被忽略.<br />
<a name="JL6FZ"></a>
### 5.10 对输出内容进行控制的参数

- `-D` : 显示所有可用网络接口的列表<br />
- `-e` : 每行的打印输出中将包括数据包的数据链路层头部信息<br />
- `-E` : 揭秘IPSEC数据<br />
- `-L` ：列出指定网络接口所支持的数据链路层的类型后退出<br />
- `-Z`：后接用户名，在抓包时会受到权限的限制。如果以root用户启动tcpdump，tcpdump将会有超级用户权限。<br />
- `-d`：打印出易读的包匹配码<br />
- `-dd`：以C语言的形式打印出包匹配码.<br />
- `-ddd`：以十进制数的形式打印出包匹配码<br />
<a name="Nk1KB"></a>
## 5. 过滤规则组合

- and：所有的条件都需要满足，也可以表示为 &&<br />
- or：只要有一个条件满足就可以，也可以表示为 `||`<br />
- not：取反，也可以使用 `!`<br />

举个例子，想需要抓一个来自`10.5.2.3`，发往任意主机的3389端口的包
```bash
$ tcpdump src 10.5.2.3 and dst port 3389
```
当在使用多个过滤器进行组合时，有可能需要用到括号，而括号在 shell 中是特殊符号，需要使用引号将其包含。例子如下：
```bash
$ tcpdump 'src 10.0.2.4 and (dst port 3389 or 22)'
```
而在单个过滤器里，常常会判断一条件是否成立，这时候，就要使用下面两个符号

- `=`：判断二者相等<br />
- `==`：判断二者相等<br />
- `!=`：判断二者不相等<br />

当使用这两个符号时，tcpdump 还提供了一些关键字的接口来方便进行判断，比如

- if：表示网卡接口名、<br />
- proc：表示进程名<br />
- pid：表示进程 id<br />
- svc：表示 service class<br />
- dir：表示方向，in 和 out<br />
- eproc：表示 effective process name<br />
- epid：表示 effective process ID<br />

比如现在要过滤来自进程名为 `nc` 发出的流经 en0 网卡的数据包，或者不流经 en0 的入方向数据包，可以这样子写
```bash
$ tcpdump "( if=en0 and proc =nc ) || (if != en0 and dir=in)"
```
<a name="SP0H5"></a>
## 6. 特殊过滤规则
<a name="r27VB"></a>
### 6.1 根据 tcpflags 进行过滤
tcpdump 支持根据数据包的标志位进行过滤
```
proto [ expr:size ]
```

- `proto`：可以是熟知的协议之一（如ip，arp，tcp，udp，icmp，ipv6）<br />
- `expr`：可以是数值，也可以是一个表达式，表示与指定的协议头开始处的字节偏移量。<br />
- `size`：是可选的，表示从字节偏移量开始取的字节数量。<br />

接下来，举几个例子，让人明白它的写法，不过在那之前，有几个点需要明白，这在后面的例子中会用到：<br />**1、**tcpflags 可以理解为是一个别名常量，相当于 13，它代表着与指定的协议头开头相关的字节偏移量，也就是标志位，所以 tcp[tcpflags] 等价于 tcp[13] ，对应下图中的报文位置。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1610608942158-cf342482-dce3-4d4a-b086-2a1d7880fc6d.webp#align=left&display=inline&height=86&originHeight=86&originWidth=496&size=0&status=done&style=none&width=496)<br />**2、**tcp-fin, tcp-syn, tcp-rst, tcp-push, tcp-ack, tcp-urg 这些同样可以理解为别名常量，分别代表 1，2，4，8，16，32，64。这些数字是如何计算出来的呢？<br />以 tcp-syn 为例，可以参照下面这张图，计算出来的值 是就是 2<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1610608942151-151cde0b-c44e-4637-81ee-612eb3cd98ef.webp#align=left&display=inline&height=265&originHeight=265&originWidth=814&size=0&status=done&style=none&width=814)<br />由于数字不好记忆，所以一般使用这样的“别名常量”表示。<br />因此当下面这个表达式成立时，就代表这个包是一个 syn 包。
```bash
tcp[tcpflags] == tcp-syn
```
要抓取特定数据包，方法有很多种。<br />下面以最常见的 syn包为例，演示一下如何用 tcpdump 抓取到 syn 包，而其他的类型的包也是同样的道理。<br />主要有三种写法：<br />1、第一种写法：使用数字表示偏移量
```bash
$ tcpdump -i eth0 "tcp[13] & 2 != 0"
```
2、第二种写法：使用别名常量表示偏移量
```bash
$ tcpdump -i eth0 "tcp[tcpflags] & tcp-syn != 0"
```
3、第三种写法：使用混合写法
```bash
$ tcpdump -i eth0 "tcp[tcpflags] & 2 != 0" 
# or
$ tcpdump -i eth0 "tcp[13] & tcp-syn != 0"
```
如果想同时捕获多种类型的包呢，比如 syn + ack 包<br />1、第一种写法
```bash
$ tcpdump -i eth0 'tcp[13] == 2 or tcp[13] == 16'
```
2、第二种写法
```bash
$ tcpdump -i eth0 'tcp[tcpflags] == tcp-syn or tcp[tcpflags] == tcp-ack'
```
3、第三种写法
```bash
$ tcpdump -i eth0 "tcp[tcpflags] & (tcp-syn|tcp-ack) != 0"
```
4、第四种写法：注意这里是 单个等号，而不是像上面一样两个等号，18（syn+ack） = 2（syn） + 16（ack）
```bash
$ tcpdump -i eth0 'tcp[13] = 18'
# or
$ tcpdump -i eth0 'tcp[tcpflags] = 18'
```
tcp 中有 类似 tcp-syn 的别名常量，其他协议也是有的，比如 icmp 协议，可以使用的别名常量有
```bash
icmp-echoreply, icmp-unreach, icmp-sourcequench, 
icmp-redirect, icmp-echo, icmp-routeradvert,
icmp-routersolicit, icmp-timx-ceed, icmp-paramprob, 
icmp-tstamp, icmp-tstampreply,icmp-ireq, 
icmp-ireqreply, icmp-maskreq, icmp-maskreply
```
<a name="DbGFl"></a>
### 6.2  基于包大小进行过滤
若想查看指定大小的数据包，也是可以的
```bash
$ tcpdump less 32 
$ tcpdump greater 64 
$ tcpdump <= 128
```
<a name="3EQ7B"></a>
### 6.3 根据 mac 地址进行过滤
例子如下，其中 ehost 是记录在 /etc/ethers 里的 name
```bash
$ tcpdump ether host [ehost]
$ tcpdump ether dst    [ehost]
$ tcpdump ether src    [ehost]
```
<a name="cW6F9"></a>
### 6.4 过滤通过指定网关的数据包
```bash
$ tcpdump gateway [host]
```
<a name="rEdJk"></a>
### 6.5 过滤广播/多播数据包
```bash
$ tcpdump ether broadcast
$ tcpdump ether multicast
$ tcpdump ip broadcast
$ tcpdump ip multicast
$ tcpdump ip6 multicast
```
<a name="YWrHv"></a>
## 7. 如何抓取到更精准的包？
如果只想抓取 HTTP 的 POST 请求该如何写呢？<br />如果只学习了上面的内容，还是无法写法满足这个抓取需求的过滤器。<br />先给出答案，然后再剖析一下，这个过滤器是如何生效的，居然能对包内的内容进行判断。
```bash
$ tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4]'
```
命令里的可选参数，在前面的内容里已经详细讲过了。这里不再细讲。<br />本节的重点是引号里的内容，看起来很复杂的样子。<br />将它逐一分解，只要先理解了下面几种用法，就能明白

- `tcp[n]`：表示 tcp 报文里 第 n 个字节<br />
- `tcp[n:c]`：表示 tcp 报文里从第n个字节开始取 c 个字节，tcp[12:1] 表示从报文的第12个字节（因为有第0个字节，所以这里的12其实表示的是13）开始算起取一个字节，也就是 8 个bit。查看 tcp 的报文首部结构，可以得知这 8 个bit 其实就是下图中的红框圈起来的位置，而在这里只要前面 4个bit，也就是实际数据在整个报文首部中的偏移量。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1610608942145-3d3e096d-c0eb-4853-b24d-9e7337f846fd.webp#align=left&display=inline&height=328&originHeight=328&originWidth=1080&size=0&status=done&style=none&width=1080)
- `&`：是位运算里的 and 操作符，比如 `0011 & 0010 = 0010`<br />
- `>>`：是位运算里的右移操作，比如 `0111 >> 2 = 0011`<br />
- `0xf0`：是 10 进制的 240 的 16 进制表示，但对于位操作来说，10进制和16进制都将毫无意义，需要的是二进制，将其转换成二进制后是：11110000，这个数有什么特点呢？前面个 4bit 全部是 1，后面4个bit全部是0。<br />

分解完后，再慢慢合并起来看<br />1、`tcp[12:1] & 0xf0` 其实并不直观，但是将它换一种写法，就好看多了，假设 tcp 报文中的 第12 个字节是这样组成的 `10110000`，那么这个表达式就可以变成 10110110 && 11110000 = 10110000，得到了 10110000 后，再进入下一步。<br />2、`tcp[12:1] & 0xf0) >> 2` 这个表达式实际是 `(tcp[12:1] & 0xf0) >> 4 ) << 2` 的简写形式。所以要搞懂 `tcp[12:1] & 0xf0) >> 2` 只要理解了`(tcp[12:1] & 0xf0) >> 4 ) << 2`  就行了 。<br />从上一步算出了 `tcp[12:1] & 0xf0`  的值其实是一个字节，也就是 8 个bit，但是再回去看下上面的 tcp 报文首部结构图，表示数据偏移量的只有 4个bit，也就是说 上面得到的值 10110000，前面 4 位（1011）才是正确的偏移量，那么为了得到 1011，只需要将 10110000 右移4位即可，也就是 `tcp[12:1] & 0xf0) >> 4`，至此是否已经得出了实际数据的正确位置呢，很遗憾还没有，Data Offset 的单位是 4个字节，因为要将 1011 乘以 4才可以，除以4在位运算中相当于左移2位，也就是 `<<2`，与前面的 `>>4` 结合起来一起算的话，最终的运算可以简化为 `>>2`<br />至此，终于得出了实际数据开始的位置是 `tcp[12:1] & 0xf0) >> 2` （单位是字节）。<br />找到了数据的起点后，目的是从数据中打到 HTTP 请求的方法，是 GET 呢 还是 POST ，或者是其他的？<br />有了上面的经验，就可以使用 `tcp[((tcp[12:1] & 0xf0) >> 2):4]` 从数据开始的位置再取出四个字节，然后将结果与 `GET` （注意 GET最后还有个空格）的 16进制写法（也就是 `0x47455420`）进行比对。
```
0x47   -->   71    -->  G
0x45   -->   69    -->  E
0x54   -->   84    -->  T
0x20   -->   32    -->  空格
```

如果相等，则该表达式为True，tcpdump 认为这就是所需要抓的数据包，将其输出到终端屏幕上。
<a name="5go9p"></a>
## 8、tcpdump示例
:::tips
注意：tcpdump只能抓取流经本机的数据包。
:::
<a name="ZJByx"></a>
### (1)默认启动
```bash
tcpdump
```
默认情况下，直接启动tcpdump将监视第一个网络接口(非lo口)上所有流通的数据包。这样抓取的结果会非常多，滚动非常快。
<a name="E8Kbw"></a>
### (2)监视指定网络接口的数据包
```bash
tcpdump -i eth1
```
如果不指定网卡，默认tcpdump只会监视第一个网络接口，如eth0。
<a name="idlxT"></a>
### (3)监视指定主机的数据包
例如所有进入或离开longshuai的数据包
```bash
tcpdump host longshuai
```
<a name="AA6db"></a>
### (4)打印helios<-->hot或helios<-->ace之间通信的数据包
```bash
tcpdump host helios and \( hot or ace \)
```
<a name="zjBEJ"></a>
### (5)打印ace与任何其他主机之间通信的IP数据包,但不包括与helios之间的数据包
```bash
tcpdump ip host ace and not helios
```
<a name="eMCFt"></a>
### (6)截获主机hostname发送的所有数据
```bash
tcpdump src host hostname
```
<a name="a4fe9c02"></a>
### (7)监视所有发送到主机hostname的数据包
```bash
tcpdump dst host hostname
```
<a name="zVYEa"></a>
### (8)监视指定主机和端口的数据包
```bash
tcpdump tcp port 22 and host hostname
```
<a name="1Aw6P"></a>
### (9)对本机的udp 123端口进行监视(123为ntp的服务端口)
```bash
tcpdump udp port 123
```
<a name="Aj8Ss"></a>
### (10)监视指定网络的数据包
如本机与192.168网段通信的数据包，"-c 10"表示只抓取10个包
```bash
tcpdump -c 10 net 192.168
```
<a name="hcDKr"></a>
### (11)打印所有通过网关snup的ftp数据包
(注意：表达式被单引号括起来了,这可以防止shell对其中的括号进行错误解析)
```bash
shell> tcpdump 'gateway snup and (port ftp or ftp-data)'
```
<a name="9c4cb393"></a>
### (12)抓取ping包
```bash
[root@server2 ~]# tcpdump -c 5 -nn -i eth0 icmp 
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
12:11:23.273638 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16422, seq 10, length 64
12:11:23.273666 IP 192.168.100.62 > 192.168.100.70: ICMP echo reply, id 16422, seq 10, length 64
12:11:24.356915 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16422, seq 11, length 64
12:11:24.356936 IP 192.168.100.62 > 192.168.100.70: ICMP echo reply, id 16422, seq 11, length 64
12:11:25.440887 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16422, seq 12, length 64
packets captured
packets received by filter
packets dropped by kernel
```
如果明确要抓取主机为192.168.100.70对本机的ping，则使用and操作符。
```bash
[root@server2 ~]# tcpdump -c 5 -nn -i eth0 icmp and src 192.168.100.62
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
12:09:29.957132 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16166, seq 1, length 64
12:09:31.041035 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16166, seq 2, length 64
12:09:32.124562 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16166, seq 3, length 64
12:09:33.208514 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16166, seq 4, length 64
12:09:34.292222 IP 192.168.100.70 > 192.168.100.62: ICMP echo request, id 16166, seq 5, length 64
packets captured
packets received by filter
packets dropped by kernel
```
:::tips
注意：不能直接写icmp src 192.168.100.70，因为icmp协议不支持直接应用host这个type。
:::
<a name="wkbD6"></a>
### (13)抓取到本机22端口包
```bash
[root@server2 ~]# tcpdump -c 10 -nn -i eth0 tcp dst port 22  
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
12:06:57.574293 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 535528834, win 2053, length 0
12:06:57.629125 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 193, win 2052, length 0
12:06:57.684688 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 385, win 2051, length 0
12:06:57.738977 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 577, win 2050, length 0
12:06:57.794305 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 769, win 2050, length 0
12:06:57.848720 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 961, win 2049, length 0
12:06:57.904057 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 1153, win 2048, length 0
12:06:57.958477 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 1345, win 2047, length 0
12:06:58.014338 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 1537, win 2053, length 0
12:06:58.069361 IP 192.168.100.1.5788 > 192.168.100.62.22: Flags [.], ack 1729, win 2052, length 0
packets captured
packets received by filter
packets dropped by kernel
```
<a name="UT1cS"></a>
### (14)解析包数据
```bash
[root@server2 ~]# tcpdump -c 2 -q -XX -vvv -nn -i eth0 tcp dst port 22
tcpdump: listening on eth0, link-type EN10MB (Ethernet), capture size 65535 bytes
12:15:54.788812 IP (tos 0x0, ttl 64, id 19303, offset 0, flags [DF], proto TCP (6), length 40)
    192.168.100.1.5788 > 192.168.100.62.22: tcp 0
        0x0000:  000c 2908 9234 0050 56c0 0008 0800 4500  ..)..4.PV.....E.
        0x0010:  0028 4b67 4000 4006 a5d8 c0a8 6401 c0a8  .(Kg@.@.....d...
        0x0020:  643e 169c 0016 2426 5fd6 1fec 2b62 5010  d>....$&_...+bP.
        0x0030:  0803 7844 0000 0000 0000 0000            ..xD........
12:15:54.842641 IP (tos 0x0, ttl 64, id 19304, offset 0, flags [DF], proto TCP (6), length 40)
    192.168.100.1.5788 > 192.168.100.62.22: tcp 0
        0x0000:  000c 2908 9234 0050 56c0 0008 0800 4500  ..)..4.PV.....E.
        0x0010:  0028 4b68 4000 4006 a5d7 c0a8 6401 c0a8  .(Kh@.@.....d...
        0x0020:  643e 169c 0016 2426 5fd6 1fec 2d62 5010  d>....$&_...-bP.
        0x0030:  0801 7646 0000 0000 0000 0000            ..vF........
packets captured
packets received by filter
packets dropped by kernel
```
<a name="fJ7zt"></a>
### (15)提取 HTTP 的 User-Agent
从 HTTP 请求头中提取 HTTP 用户代理：
```bash
$ tcpdump -nn -A -s1500 -l | grep "User-Agent:"
```
通过 `egrep` 可以同时提取用户代理和主机名（或其他头文件）：
```bash
$ tcpdump -nn -A -s1500 -l | egrep -i 'User-Agent:|Host:'
```
<a name="h1edZ"></a>
### (16)抓取 HTTP GET 和 POST 请求
抓取 HTTP GET 请求包：
```bash
$ tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420'
# or
$ tcpdump -vvAls0 | grep 'GET'
```
可以抓取 HTTP POST 请求包：
```bash
$ tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354'
# or 
$ tcpdump -vvAls0 | grep 'POST'
```
:::tips
注意：该方法不能保证抓取到 HTTP POST 有效数据流量，因为一个 POST 请求会被分割为多个 TCP 数据包。
:::
<a name="OOuDi"></a>
### (17)找出发包数最多的 IP
找出一段时间内发包最多的 IP，或者从一堆报文中找出发包最多的 IP，可以使用下面的命令：
```bash
$ tcpdump -nnn -t -c 200 | cut -f 1,2,3,4 -d '.' | sort | uniq -c | sort -nr | head -n 20
```

- `**cut -f 1,2,3,4 -d '.'**` : 以 `.` 为分隔符，打印出每行的前四列。即 IP 地址。<br />
- `**sort | uniq -c**` : 排序并计数<br />
- `**sort -nr**` : 按照数值大小逆向排序<br />
<a name="ALoa9"></a>
### (18)抓取 DNS 请求和响应
DNS 的默认端口是 53，因此可以通过端口进行过滤
```bash
$ tcpdump -i any -s0 port 53
```
<a name="1RwlY"></a>
### (19)切割 pcap 文件
当抓取大量数据并写入文件时，可以自动切割为多个大小相同的文件。例如，下面的命令表示每 3600 秒创建一个新文件 `capture-(hour).pcap`，每个文件大小不超过 `200*1000000` 字节：
```bash
$ tcpdump  -w /tmp/capture-%H.pcap -G 3600 -C 200
```
这些文件的命名为 `capture-{1-24}.pcap`，24 小时之后，之前的文件就会被覆盖。
<a name="8rjga"></a>
### (20)提取 HTTP POST 请求中的密码
从 HTTP POST 请求中提取密码和主机名：
```bash
$ tcpdump -s 0 -A -n -l | egrep -i "POST /|pwd=|passwd=|password=|Host:"
```
<a name="Mn1fe"></a>
### (21)提取 HTTP 请求的 URL
提取 HTTP 请求的主机名和路径：
```bash
$ tcpdump -s 0 -v -n -l | egrep -i "POST /|GET /|Host:"
```
<a name="B3PmQ"></a>
### (22)抓取 HTTP 有效数据包
抓取 80 端口的 HTTP 有效数据包，排除 TCP 连接建立过程的数据包（SYN / FIN / ACK）：
```bash
$ tcpdump 'tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)'
```
<a name="TsQ7N"></a>
### (23)结合 Wireshark 进行分析
通常 `Wireshark`（或 tshark）比 tcpdump 更容易分析应用层协议。一般的做法是在远程服务器上先使用 `tcpdump` 抓取数据并写入文件，然后再将文件拷贝到本地工作站上用 `Wireshark` 分析。<br />还有一种更高效的方法，可以通过 ssh 连接将抓取到的数据实时发送给 Wireshark 进行分析。以 MacOS 系统为例，可以通过 `brew cask install wireshark` 来安装，然后通过下面的命令来分析：
```bash
$ ssh root@remotesystem 'tcpdump -s0 -c 1000 -nn -w - not port 22' | /Applications/Wireshark.app/Contents/MacOS/Wireshark -k -i -
```
例如，如果想分析 DNS 协议，可以使用下面的命令：
```bash
$ ssh root@remotesystem 'tcpdump -s0 -c 1000 -nn -w - port 53' | /Applications/Wireshark.app/Contents/MacOS/Wireshark -k -i -
```
`-c` 选项用来限制抓取数据的大小。如果不限制大小，就只能通过 `ctrl-c` 来停止抓取，这样一来不仅关闭了 tcpdump，也关闭了 wireshark。
<a name="acf24fcd"></a>
## 9、保存数据包结果
在某些情况下，可能希望将输出重定向到文件，以便以后可以详细研究它或使用其他程序来解析输出。在以下示例中，仍然可以在将输出保存到文件时观看输出：
```bash
tcpdump -l | tee tcpdump_`date +%Y%m%e-%k.%M`
```
在上面的示例中，可以使用日期和时间来标识每个转储。在处理一天中特定时间出现的问题时，这可能会派上用场。<br />tcpdump还可以选择将其输出转储为二进制格式，以便以后读取。要创建一个二进制文件：
```bash
tcpdump -w tcpdump_raw_`date +%Y%m%e-%k.%M`
```
稍后，可以让tcpdump使用以下命令读取文件
```bash
tcpdump -r tcpdump_raw_YYYMMDD-H.M
```
也可以使用ethereal程序打开原始转储并解释它。<br />tcpdump提供了有关往返于网络的所有数据包的信息。
<a name="3bta3"></a>
## 10、将Ethereal与tcpdump一起使用
Ethereal是一种也可以用来捕获网络数据包的工具。安装后可以打开原始转储文件。<br />查看原始转储文件变得相当容易。可以看到源IP和目标IP以及它是什么类型的数据包。可以对遇到的网络问题进行故障排除并分析可疑行为。为端口32772运行了一个特定的转储，如下所示：
```bash
tcpdump port 32772 -w dump_32772
```
<a name="7s9pT"></a>
## 11、读取原始输出
即使从tcpdump读取所谓的“人类可读”输出也可能有点神秘。看下面的示例，只是从转储中选出一个随机数据包：
```bash
17:26:22.924493 IP www.linux.org.www > test.linux.org.34365: P 2845:3739(894) ack 1624 win 9648 <nop,nop,timestamp 326501459 24374272>
```
可以看到的是对[https://www.linux.org/](https://www.linux.org/)的Web服务器请求。时间戳记过后，会在主机名（即端口80）的末尾注意到.www。这将被发送到发出请求的主机test.linux.org的端口34365。“ P”代表TCP“ oush”功能。这意味着应该立即发送数据。在2845：3739（894）之后的数字中，2845标记第一个数据包的八位位组的编号。数字3739是数据包发送的最后一个字节的数字加1。数字894是发送的数据包的长度。表示“ ack 1624”的部分是“ acknowledge”的TCP术语-表示该数据包已被接受，下一个预期的数据包编号为1624。之后，看到“ win 9648”发送主机正在等待窗口大小为9648个八位位组的数据包。这之后是时间戳。<br />现在，如果认为难以解释，则使用-x选项，它将在十六进制输出中包含数据包内容。
```bash
18:12:45.149977 IP www.linux.org.www > test.linux.org.34536: . 1:1449(1448) 
ack 487 win 6432 <nop,nop,timestamp 329284215 27156244>
        0x0000:  4500 05dc 6a81 4000 4006 493b c0a8 0006  E...j.@.@.I;....
        0x0010:  c0a8 0009 0050 86e8 8fa4 1d47 1c33 e3af  .....P.....G.3..
        0x0020:  8010 1920 b4d9 0000 0101 080a 13a0 7a77  ..............zw
        0x0030:  019e 5f14 4854 5450 2f31 2e31 2032 3030  .._.HTTP/1.1.200
        0x0040:  204f 4b0d 0a44 6174 653a 2054 6875 2c20  .OK..Date:.Thu,.
        0x0050:  3135
```
可以从输出中得知这是一个HTTP请求。至于其余的，它不是人类直接可读的，但是很容易知道这是一个合法的数据包。使用这种格式的另一个好处是，即使无法完全解释此数据包的状况，也可以将其发送给可能的人。最终，这是未经任何过滤通过网络传输的原始数据。
