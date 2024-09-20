Linux
<a name="Dg6vy"></a>
## w 命令
w 命令是 Linux 中的一个工具，它显示当前登录到系统中的用户及其进程的信息。它显示谁已登录，以及他们正在做什么活动。这意味着它可以显示他们在系统中运行什么进程。
<a name="wBxKf"></a>
### 语法
下面是 w 命令的基本语法：
```bash
w [options] [username]
```
w 命令接受一个可选的选项列表，然后是一个可选的用户名。如果指定了用户名，w 将只显示该用户拥有的进程信息。
<a name="CFPqc"></a>
## w 命令的例子及其用法
下面是一些使用 w 命令的例子。<br />当只用 w 运行它时，它显示以下输出：
```bash
$ w

 21:45:07 up 1 day, 12:48,  1 user,  load average: 1.05, 0.85, 0.56

USER     TTY        LOGIN@   IDLE   JCPU   PCPU WHAT

debugpoi tty2       Thu08   36:48m  0.03s  0.03s /usr/libexec/gnome-session-binary
```
![Linux 中 w 命令的基本输出](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1673521386261-a6684469-e578-4610-8de1-b44dc56c9693.jpeg#averageHue=%23142b14&clientId=u36069334-9e05-4&from=paste&id=ue9aecc34&originHeight=218&originWidth=974&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u72b711d4-ad46-4b4e-8742-d824de5dd92&title=Linux%20%E4%B8%AD%20w%20%E5%91%BD%E4%BB%A4%E7%9A%84%E5%9F%BA%E6%9C%AC%E8%BE%93%E5%87%BA "Linux 中 w 命令的基本输出")<br />解释：USER 列给出了用户名，然后是终端号、登录日期时间、空闲时间、CPU 使用率，以及用户正在执行的进程。<br />◈ USER - 在 Linux 或 BSD 系统中登录的用户名称。<br />◈ TTY - 当前会话的终端标识符号。<br />◈ FROM - 用户的主机名或 IP 地址。<br />◈ LOGIN@ - 用户登录的时间。它有时会根据你的系统设置显示日期。<br />◈ IDLE - 用户与终端交互后的空闲时间。<br />◈ JCPU - 该会话的所有用户进程使用的 CPU 时间。<br />◈ PCPU - 该用户的进程（在 WHAT 字段中提到）使用的时间。<br />◈ WHAT - 当前带参数的进程。<br />下面是 w 命令的另一个例子，有两个用户在虚拟机环境中登录。正如你所看到的，显示了两个用户名与当前运行的带有进程参数的独立进程。<br />![多用户环境的 w 命令输出](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1673521386276-963ab909-8890-40c0-b48f-372ab5701c90.jpeg#averageHue=%233d4048&clientId=u36069334-9e05-4&from=paste&id=ubf67fe74&originHeight=172&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u5dcfa4ad-bd16-4287-a75a-2ef8ebd7c44&title=%E5%A4%9A%E7%94%A8%E6%88%B7%E7%8E%AF%E5%A2%83%E7%9A%84%20w%20%E5%91%BD%E4%BB%A4%E8%BE%93%E5%87%BA "多用户环境的 w 命令输出")<br />看一下这个命令的一些选项。<br />要停止显示标题，使用 `-h` 选项。它与 `--no-header` 开关相同。
```bash
$ w -h
```
`-f` 选项可以在输出中切换 FROM 字段的可见性。
```bash
$ w -f
```
使用 `-s` 选项打印一个简短的输出，不包括 JCPU、PCPU 和 LOGIN@ 信息。
```bash
$ w -s
```
要显示一个特定用户（例如，debugpoint）拥有的所有进程的列表：
```bash
$ w debugpoint
```
