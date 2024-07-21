Linux
<a name="pIc75"></a>
### scp 命令的基本语法
下面的命令将读作 copy source_file_name进入destination_folder在destination_host使用username account。
```bash
> scp source_file_name username@destination_host:destination_folder
```
里面有很多参数scp可以使用的命令。以下是可能在日常使用中使用的参数。
<a name="b0Kqg"></a>
### 使用`-v`参数提供 scp 进程的详细信息
基础的scp不带参数的命令将在后台复制文件。除非该过程完成或出现某些错误，否则用户将看不到任何内容。<br />可以使用 `-v` 参数将调试信息打印到屏幕中。它可以帮助你调试连接、身份验证和配置问题。
```bash
$ scp -v Label.pdf rumenz@192.168.1.110:.
```
输出复制文件时 scp 显示进度
```bash
Executing: program /usr/bin/ssh host 202.x.x.x, user mrarianto, command scp -v -t .
OpenSSH_6.0p1 Debian-3, OpenSSL 1.0.1c 10 May 2012
debug1: Reading configuration data /etc/ssh/ssh_config
debug1: /etc/ssh/ssh_config line 19: Applying options for *
debug1: Connecting to 202.x.x.x [202.x.x.x] port 22.
debug1: Connection established.
debug1: Host '202.x.x.x' is known and matches the RSA host key.
debug1: Found key in /home/pungki/.ssh/known_hosts:1
debug1: ssh_rsa_verify: signature correct
debug1: Next authentication method: password
fcant@202.x.x.x's password:
debug1: Authentication succeeded (password).
Authenticated to 202.x.x.x ([202.x.x.x]:22).
Sending file modes: C0770 3760348 Label.pdf
Sink: C0770 3760348 Label.pdf
Label.pdf 100% 3672KB 136.0KB/s 00:27
Transferred: sent 3766304, received 3000 bytes, in 65.2 seconds
Bytes per second: sent 57766.4, received 46.0
debug1: Exit status 0
```
<a name="OYngs"></a>
### 提供复制大文件所需的时间和连接速度
`-p` 参数将复制大文件所需的时间和连接速度将出现在屏幕上。
```bash
$ scp -p Label.pdf fcant@192.168.1.110:.
```
输出 scp 估计复制大文件所需的时间
```
> fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 126.6KB/s 00:29
```
<a name="wyOAW"></a>
### 使用 `-C` 参数加快文件传输速度
可以加快文件传输速度的参数之一是 -C 范围。这 -C 参数将随时随地压缩你的文件，独特的是压缩只发生在网络中。当文件到达目标服务器时，它将恢复到压缩前的原始大小。
```bash
$ scp -pv messages.log fcant@192.168.1.110:.
```
scp 在不压缩的情况下传输文件
```
Executing: program /usr/bin/ssh host 202.x.x.x, user mrarianto, command scp -v -p -t .
OpenSSH_6.0p1 Debian-3, OpenSSL 1.0.1c 10 May 2012
debug1: Reading configuration data /etc/ssh/ssh_config
debug1: /etc/ssh/ssh_config line 19: Applying options for *
debug1: Connecting to 202.x.x.x [202.x.x.x] port 22.
debug1: Connection established.
debug1: identity file /home/pungki/.ssh/id_rsa type -1
debug1: Found key in /home/pungki/.ssh/known_hosts:1
debug1: ssh_rsa_verify: signature correct
debug1: Trying private key: /home/pungki/.ssh/id_rsa
debug1: Next authentication method: password
rumenz@202.x.x.x's password:
debug1: Authentication succeeded (password).
Authenticated to 202.x.x.x ([202.x.x.x]:22).
debug1: Sending command: scp -v -p -t .
File mtime 1323853868 atime 1380425711
Sending file timestamps: T1323853868 0 1380425711 0
messages.log 100% 93MB 58.6KB/s 27:05
Transferred: sent 97614832, received 25976 bytes, in 1661.3 seconds
Bytes per second: sent 58758.4, received 15.6
debug1: Exit status 0
```
`-C` 允许压缩
```bash
$ scp -Cpv messages.log rumenz@192.168.1.110:.
```
scp 使用压缩更快地传输文件
```
Executing: program /usr/bin/ssh host 202.x.x.x, user mrarianto, command scp -v -p -t .
OpenSSH_6.0p1 Debian-3, OpenSSL 1.0.1c 10 May 2012
debug1: Reading configuration data /etc/ssh/ssh_config
debug1: /etc/ssh/ssh_config line 19: Applying options for *
debug1: Connecting to 202.x.x.x [202.x.x.x] port 22.
debug1: Connection established.
debug1: identity file /home/pungki/.ssh/id_rsa type -1
debug1: Host '202.x.x.x' is known and matches the RSA host key.
debug1: Found key in /home/pungki/.ssh/known_hosts:1
debug1: ssh_rsa_verify: signature correct
debug1: Next authentication method: publickey
debug1: Trying private key: /home/pungki/.ssh/id_rsa
debug1: Next authentication method: password
fcant@202.x.x.x's password:
debug1: Enabling compression at level 6.
debug1: Authentication succeeded (password).
Authenticated to 202.x.x.x ([202.x.x.x]:22).
debug1: channel 0: new [client-session]
debug1: Sending command: scp -v -p -t .
File mtime 1323853868 atime 1380428748
Sending file timestamps: T1323853868 0 1380428748 0
Sink: T1323853868 0 1380428748 0
Sending file modes: C0600 97517300 messages.log
messages.log 100% 93MB 602.7KB/s 02:38
Transferred: sent 8905840, received 15768 bytes, in 162.5 seconds
Bytes per second: sent 54813.9, received 97.0
debug1: Exit status 0
debug1: compress outgoing: raw data 97571111, compressed 8806191, factor 0.09
debug1: compress incoming: raw data 7885, compressed 3821, factor 0.48
```
如果要通过网络复制大量文件，-C 参数将减少所需的总时间。<br />压缩方法不适用于所有文件。当源文件已经被压缩时，就没有什么效果了。文件如.zip,.rar,pictures， 和.iso
<a name="kLbwr"></a>
### 更改 scp 密码以加密文件
默认情况下scp使用 AES-128 来加密文件。如果想更改为其他密码对其进行加密，可以使用 `-c` 范围。看看这个命令。
```bash
$ scp -c 3des Label.pdf fcant@192.168.1.110:.

fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 282.5KB/s 00:13
```
上面的命令告诉scp使用3des algorithm来加密文件。请注意此参数使用 `-c`不是`-C`。
<a name="UKxnX"></a>
### 使用 scp 命令限制带宽使用
另一个可能有用的参数是 `-l` 范围。这 `-l` 参数将限制使用的带宽制 。如果执行自动化脚本来复制大量文件，这将很有用
```bash
$ scp -l 400 Label.pdf fcant@192.168.1.110:.

fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 50.3KB/s 01:13
```
scp 默认带宽的单位是Kilobyte/sec(KB/s）。所以如果想限制带宽scp最多只有50 KB/s，需要将其设置为50 x 8=400。
<a name="fuyPO"></a>
### 指定要与 scp 一起使用的特定端口
scp正在使用端口22作为默认端口。但出于安全原因，可以将端口更改为另一个端口。例如，这里使用端口2249。
```bash
$ scp -P 2249 Label.pdf fcant@192.168.1.110:.

fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 262.3KB/s 00:14
```
<a name="PDyZV"></a>
### 递归复制目录中的文件
有时需要复制目录和其中的所有文件，目录。
```bash
$ scp -r documents fcant@192.168.1.110:.

fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 282.5KB/s 00:13
scp.txt 100% 10KB 9.8KB/s 00:00
```
复> 制过程完成后，将在目标服务器上找到一个名为documents及其所有文件。文件夹documents是自动创建的。
<a name="oYTBa"></a>
### 禁用进度表和警告 / 诊断消息
```bash
$ scp -q Label.pdf fcant@192.168.1.110:.

fcant@202.x.x.x's password:
fcant@local $
```
可以看到，输入密码后，没有关于 scp 进程的信息。该过程完成后，将再次看到提示。
<a name="GNIeF"></a>
### 通过代理使用 scp 复制文件
代理服务器通常用于办公环境。scp 本身没有配置代理。当环境使用代理时，必须告诉scp 与代理进行通信。<br />例如代理地址是10.0.96.6代理端口是8080。代理还实现了用户身份验证。首先，需要创建 ~/.ssh/config文件
```
ProxyCommand /usr/bin/corkscrew 10.0.96.6 8080 %h %p ~/.ssh/proxyauth
```
然后需要创建文件 ~/.ssh/proxyauth 里面输入。
```
myusername:mypassword
```
前提是需要安装corkscrew
```bash
$ apt-get install corkscrew
```
Centos系统可以用yum安装corkscrew
```bash
# yum install corkscrew
```
由于 ~/.ssh/proxyauth 文件包含你的 username和password 以明文格式，请确保该文件只能自己访问。
<a name="Q1Gmh"></a>
### 选择不同的ssh_config 文件
对于经常在公司网络和公共网络之间切换的移动用户来说，总是在scp中更改设置会很痛苦。
<a name="MVmVT"></a>
#### 场景示例
代理在公司网络中使用，但不在公共网络中使用，并且定期切换网络。
```bash
$ scp -F /home/pungki/proxy_ssh_config Label.pdf

fcant@192.168.1.110:.
fcant@202.x.x.x's password:
Label.pdf 100% 3672KB 282.5KB/s 00:13
```
默认情况下 ssh_config 每个用户的文件将被放置在 ~/.ssh/config。创建一个特定的 ssh_config 具有代理兼容性的文件将更容易在网络之间切换。<br />当在公司网络，可以使用 `-F` 范围。当在公共网络上时，可以不用`-F`参数。
