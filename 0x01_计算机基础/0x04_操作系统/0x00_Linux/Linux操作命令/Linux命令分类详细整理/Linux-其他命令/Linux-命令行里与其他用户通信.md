Linux 
<a name="ZBldz"></a>
#### wall
`wall` 命令（意为 **write all** ）允许向所有在系统已登录的用户发送信息。任何用户都可以使用 `wall` 命令，但 `wall` 命令通常是系统管理员用来向用户发送公告通知的（比如服务器将关闭维护）。<br />一个系统管理员可能会发送如下信息：
```bash
$ wall The system will be going down in 15 minutes to address a serious problem
```
然后，所有已登录的用户都能看到类似这样的通知：<br />
```
Broadcast message from admin@dragonfly (pts/0) (Thu Mar  5 08:56:42 2020):
The system is going down in 15 minutes to address a serious problem
```
如果要发送的信息里有单引号，就需要用双引号将要发送的信息括起来，如下所示：
```bash
$ wall "Don't forget to save your work before logging off"
```
上例所示的命令里，最外层的引号在信息传送时不会显示出来，但如果没有最外层的引号，这条命令执行时会一直等待一个和句中单引号配对的引号以结束句子。
<a name="1Bktg"></a>
#### mesg
如果因为某些原因想拒绝某位用户发送来的信息，就可以使用 `mesg` 命令。该命令使用时应携带参数 `n` 或者 `y`，分别表示拒绝或接受某用户来信，其用例如下（ fcant 为用户名）：
```bash
$ mesg n fcant
$ mesg y fcant
```
值得注意的是，使用 `mesg` 命令屏蔽用户后，被屏蔽的用户并不会收到自己已被屏蔽的通知，也就是说，被屏蔽的用户并不会知道自己已经被屏蔽了。<br />也可以使用这个命令来接受或屏蔽所有用户的通信，只需把用户名参数省略掉即可：
```bash
$ mesg y
$ mesg n
```
<a name="uIbWM"></a>
#### write
另一个可以直接发送文本信息而不需要转换成电邮的命令是 `write`。这个命令能被用于和特定用户的通信中，比如要向用户名为 `fcant` 的用户发送信息，可以用如下命令：
```bash
$ write fcant
Are you still at your desk?
I need to talk with you right away.
^C
```
写完发送信息后按 `ctrl+C` 退出。这个命令允许传送信息，但不是开启一个双工通道，它仅仅只能用于送出信息。如果接收用户在不止一个终端上登录，能选择一个想传输的终端或者依靠系统选择一个耗时最少的终端传输，如下所示：
```bash
$ write fcant#1
```
如果被用户 `fcant` 屏蔽了，会看到如下提示：
```bash
$ write fcant
write: fcant has messages disabled
```
<a name="jfn40"></a>
#### talk / ytalk
`talk` 和 `ytalk` 命令可以和一个或多个用户建立交互通信，其中 `talk` 命令只允许和一个用户建立交互通信。`talk` 与 `ytalk` 命令都会启动一个双版面的窗口，通信的每一方都可以在窗口的上部版面输入信息并在下部版面看到回应信息。<br />被请求加入通信的一方可以通过输入 `talk` 并在其后跟上通信发起方的用户名即可回应通信，如下所示：
```bash
Message from Talk_Daemon@dragonfly at 10:10 ...
talk: connection requested by fcant@127.0.0.1.
talk: respond with:  talk fcant@127.0.0.1
$ talk fcant
```
值得注意的是，当系统既安装了 `talk` 也安装了 `ytalk` 时， `talk` 命令默认会与 `ytalk` 命令相关联，所以当输入 `talk fcant` 后，系统实际上执行的是 `ytalk fcant` 。由于系统两个命令都安装了，所以执行以上命令后看到的输出实际上是 `ytalk fcant` 的：
```
----------------------------= YTalk version 3.3.0 =--------------------------
Is the report ready?
-------------------------------= root@iZuligp6e1dyzfZ =----------------------------
Just finished it
```
以上为用户 `fcant` 看到的窗口信息，而通信另一方的 `root` 看到的窗口内容则是反过来的：
```
----------------------------= YTalk version 3.3.0 =--------------------------
Just finished it
-------------------------------= fcant@iZuligp6e1dyzfZ =----------------------------
Is the report ready?
```
同样，这个命令也用 `ctrl+C` 来终止。<br />要用此命令和其他系统的用户通信则需要加上 `-h` 的参数，参数值为主机名或IP地址。其用例如下：
```bash
$ talk -h 192.168.0.11 fcant
```

