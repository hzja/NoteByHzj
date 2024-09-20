Linux Parallel SSH<br />parallel-ssh 是为小规模自动化而设计的异步并行的 SSH 库!<br />parallel-ssh 是为小规模自动化而设计的异步并行的 SSH 库，包括 pssh、pscp、prsync、pslurp 和 pnuke工具，其源代码使用 Python语言编写开发的。该项目最初位于Google Code上，是由Brent N.Chun编写和维护的，但是由于工作繁忙，Brent于2009年10月将维护工作移交给了Andrew McNabb管理。到了 2012年的时候，由于Google Code的已关闭，该项目一度被废弃，现在也只能在 Google Code 的归档中找到当时的版本了。<br />但是需要注意的是，之前的版本是不支持 Python3 的，但是 Github 上面有人 Fork 了一份，自己进行了改造使其支持 Python3 以上的版本了。与此同时，还有一个组织专门针对 parallel-ssh 进行了开发和维护，目前已经很久没有更新了。有需要的，自己可以自行查阅。

- [https://github.com/lilydjwg/pssh](https://github.com/lilydjwg/pssh)
- [https://github.com/ParallelSSH/parallel-ssh](https://github.com/ParallelSSH/parallel-ssh)

可扩展性

- 支持扩展到百台，甚至上千台主机使用

易于使用

- 只需两行代码，即可在任意数量的主机上运行命令

执行高效

- 号称是最快的 Python SSH 库可用

资源使用

- 相比于其他 Python SSH 库，其消耗资源最少
```bash
# Mac系统安装
$ brew install pssh

# CentOS系统安装
$ yum install pssh

# Ubuntu系统安装
$ apt install pssh

# PIP安装
$ pip insall pssh

# 源代码编译安装(2.3.1)
# 官方地址: https://code.google.com/archive/p/parallel-ssh/source/default/source
$ tar zxvf pssh-2.3.1.tar.gz
$ cd pssh-2.3.1
$ python setup.py install
```
![2021-05-08-23-22-45-580787.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487507232-709cde8b-181f-42b5-8ccf-babe74ccc47c.png#clientId=u060f98fc-6b3a-4&from=ui&id=uba4f0a0b&originHeight=390&originWidth=851&originalType=binary&size=69998&status=done&style=shadow&taskId=uf64f1c02-6b18-4759-bd77-1557a79b8a9)
```bash
# 工具对应的子命令子命令
$ ls -lh /usr/local/Cellar/pssh/2.3.1_1/bin/
pnuke -> ../libexec/bin/pnuke
prsync -> ../libexec/bin/prsync
pscp -> ../libexec/bin/pscp
pslurp -> ../libexec/bin/pslurp
pssh -> ../libexec/bin/pssh
pssh-askpass -> ../libexec/bin/pssh-askpass
```
![2021-05-08-23-22-45-753327.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487520371-facb854f-50e1-4718-a9f2-bd4fa6f5d6f3.png#clientId=u060f98fc-6b3a-4&from=ui&id=ua65058dd&originHeight=319&originWidth=870&originalType=binary&size=83123&status=done&style=shadow&taskId=u8ba6a666-74b8-4aef-85d6-f6bea83a66b)
<a name="vWJeZ"></a>
### pssh
通过 ssh 协议在多台主机上并行地运行命令
<a name="TNmsX"></a>
#### 命令参数使用
![2021-05-08-23-22-45-996201.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487533452-615eeb4e-ec44-4cd4-a9e4-0d00cecb92dc.png#clientId=u060f98fc-6b3a-4&from=ui&id=u9bb5c65c&originHeight=836&originWidth=891&originalType=binary&size=171941&status=done&style=none&taskId=uf7b91c37-1a6f-4d5b-b95f-c0247545a87)
<a name="Pp0Na"></a>
#### 适用范例
```bash
# Usage: pssh [OPTIONS] command [...]

# 在两个主机上运行命令并在每个服务器上打印其输出
$ pssh -i -H "host1 host2" hostname -i

# 运行命令并将输出保存到单独的文件中
$ pssh -H host1 -H host2 -o path/to/output_dir hostname -i

# 在多个主机上运行命令并在新行分隔的文件中指定
$ pssh -i -h path/to/hosts_file hostname -i

# 以root运行命令(要求输入root用户密码)
$ pssh -i -h path/to/hosts_file -A -l root_username hostname -i

# 运行带有额外SSH参数的命令
$ pssh -i -h path/to/hosts_file -x "-O VisualHostKey=yes" hostname -i

# 运行并行连接数量限制为10的命令
$ pssh -i -h path/to/hosts_file -p 10 'cd dir; ./script.sh; exit'
```
<a name="CEJBE"></a>
### pscp
通过 ssh 协议把文件并行地复制到多台主机上
<a name="OeptO"></a>
#### 命令参数使用
![2021-05-08-23-22-46-177713.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487555890-04d8520c-b9d1-4c01-8ae6-afc4d0106c2c.png#clientId=u060f98fc-6b3a-4&from=ui&id=ub4183f5e&originHeight=783&originWidth=890&originalType=binary&size=162037&status=done&style=none&taskId=uc89cf587-21e0-4229-bf16-d2f75c2b6c9)
<a name="f1xNZ"></a>
#### 适用范例
```bash
# Usage: pscp [OPTIONS] local remote

# 将本地文件复制到远程机器上
$ pscp -h hosts.txt -l root foo.txt /home/irb2/foo.txt
[1] 23:00:08 [SUCCESS] 172.18.10.25
[2] 09:52:28 [SUCCESS] 172.18.10.24
```
<a name="AGtQd"></a>
### prsync
通过 rsync 协议把文件高效地并行复制到多台主机上
<a name="trx8n"></a>
#### 命令参数使用
![2021-05-08-23-22-46-383681.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487698161-9972ca51-911d-43cb-b520-befef82a70be.png#clientId=u060f98fc-6b3a-4&from=ui&id=u4a147e39&originHeight=866&originWidth=803&originalType=binary&size=104440&status=done&style=shadow&taskId=u2eafa18d-a29b-4219-811d-cd0b1ebec38)
<a name="OJM0i"></a>
#### 适用范例
```bash
# Usage: prsync [OPTIONS] local remote

# 使用rsync协议进行本地文件复制操作
$ prsync -r -h hosts.txt -l root foo /home/irb2/foo
```
<a name="dP8N8"></a>
### pslurp
通过 ssh 协议把文件并行地从多个远程主机复制到中心主机上
<a name="jVuXb"></a>
#### 命令参数使用
![2021-05-08-23-22-48-023102.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487686813-13d328d0-b596-47c0-b886-f1930cb57c63.png#clientId=u060f98fc-6b3a-4&from=ui&id=u8422b0a9&originHeight=771&originWidth=801&originalType=binary&size=92674&status=done&style=shadow&taskId=u258313a3-1515-4031-9ea7-fa189c97b6f)
<a name="fXxRv"></a>
#### 适用范例
```bash
# Usage: pslurp [OPTIONS] remote local

# 将远程主机上面的文件复制到本地
$ pslurp -h hosts.txt -l root -L /tmp/outdir /home/irb2/foo.txt foo.txt
```
<a name="T6ijr"></a>
### pnuke
通过 ssh 协议并行地在多个远程主机上杀死进程
<a name="PZnvl"></a>
#### 命令参数使用
![2021-05-08-23-22-49-147255.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620487676518-0ff6ad5a-a526-49f9-bf4d-93b47e2db271.png#clientId=u060f98fc-6b3a-4&from=ui&id=u9eea8f21&originHeight=680&originWidth=807&originalType=binary&size=84276&status=done&style=shadow&taskId=u2f80ce6b-1161-4ec1-8bf6-89a848d4160)
<a name="F87ms"></a>
#### 适用范例
```bash
# Usage: pnuke [OPTIONS] pattern

# 结束远程主机上面的进程任务
$ pnuke -h hosts.txt -l root java
```
