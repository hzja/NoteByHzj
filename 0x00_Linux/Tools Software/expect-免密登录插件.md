Linux expect <br />很多时候进行linux的操作交互需要人工干预，有时候可能需要一种工具可以不需要人工干预的和linux交互，这里引入了一个工具expect。<br />
<a name="GYhgZ"></a>
## expect介绍
TCL(tool command language)是一种类似shell脚本的语言,可以使用他来完成许多操作，expect是在它的基础上发展而来的，expect还提供了一些tcl没有的命令。如果要写一个自动处理输入输出的脚本，建议选择expect。
<a name="YToWX"></a>
## expect安装
```bash
yum install -y expect
```
<a name="vBoag"></a>
## expect使用
```bash
#!/usr/bin/expect
2.设置超时时间，单位为秒，timeout -1 为永不超时。
set timeout 2
3.壳子
spawn需要在expect环境下运行，用来传递交互指令。
 4.expect命令
判断输出结果是否包含某字符串，没有立即返回，否则就等一段时间后返回，用timeout设置等待时间。
5.send
执行交互动作，将交互要执行的动作进行输入给交互指令。命令字符串结尾要加上"\r"，如果出现异常等待的状态可以进行核查。
6.exp_continue
继续执行接下来的交互操作。
7.interact
执行后保持交互状态，把控制权交给控制台，如不加interact，交互完成会自动退出。
```
<a name="YY0Mq"></a>
### 免密码通过ssh登陆服务器
操作脚本
```bash
➜  batch cat sf
#!/usr/bin/expect -f
spawn ssh  root@ip
expect {
"yes/no" { send "yes\r";exp_continue}
"*password:" { send "passwordstr\r" }
}
interact
```
执行查看结果，登陆成功
```bash
➜  batch ./35
spawn ssh root@ip
root@ip's password:
Last login: Sat Jan  5 10:15:56 2019 from 113.46.187.213

Welcome to Alibaba Cloud Elastic Compute Service !
```
