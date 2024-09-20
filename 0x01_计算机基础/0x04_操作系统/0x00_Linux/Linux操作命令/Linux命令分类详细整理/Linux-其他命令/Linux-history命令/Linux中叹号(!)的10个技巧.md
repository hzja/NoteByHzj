Linux<br />'!'Linux 中的符号或运算符可用作**逻辑否定**运算符，也可通过调整从历史记录中获取命令，或通过修改运行先前运行的命令。以下所有命令均已在 bash Shell 中明确检查。虽然没有检查过，但其中大部分不会在其他 shell 中运行。
<a name="MauKo"></a>
### 1、按命令编号运行历史记录中的命令。
可能不知道可以从**历史**命令（已经 / 较早执行的命令）运行命令这一事实。要开始，首先通过运行 “**history**” 命令找到命令编号。
```bash
$ history
```
现在，在**history**的输出中，仅按照它出现的编号运行来自**history**的命令。假设运行出现在'**history**' 命令输出中编号为**1551**的命令。
```bash
$ !1551
```
并且，它运行在编号**1551**中列出的命令。这种检索已执行命令的方法非常有用，特别是在那些很长的命令的情况下。只需要使用`!`来调用它。
<a name="ZDqRx"></a>
### 2、将先前执行的命令作为倒数第二个命令、倒数第 7 个命令等运行。
可以运行那些之前运行过的命令，它们的运行顺序是最后一个运行命令将表示为**-1**，倒数第二个为**-2**，倒数第七个为**-7**，......。<br />首先运行**history**命令以获取上次执行命令的列表。有必要运行**history**命令，这样就可以确保没有类似的命令rm command > file和其他命令，只是为了确保不会意外运行任何危险的命令。然后检查第六个最后一个命令，八个最后一个命令和第十个最后一个命令。
```bash
$ history
$ !-6
$ !-8
$ !-10
```
按数字运行上次执行的命令
<a name="nNRvv"></a>
### 3、将运行的最后一个命令的参数传递给新命令，无需重新输入
需要列出目录 “**/home/$USER/Binary/firefox**” 的内容。
```
$ ls /home/$USER/Binary/firefox
```
然后意识到应该触发 “**ls -l**” 以查看哪个文件在那里可执行？所以应该再次输入整个命令！不，不需要。只需要将这个新命令的最后一个参数作为：
```bash
$ ls -l !$
```
这里`!$`将携带在最后一个命令中传递给这个新命令的参数。<br />将上次执行命令的参数传递给新命令
<a name="jp5xT"></a>
### 4、使用 (!) 处理两个或多个参数
假设在桌面上创建了一个文本文件**1.txt**。
```bash
$ touch /home/avi/Desktop/1.txt
```
然后使用**cp**命令在任一侧使用完整路径将其复制到 “**/home/avi/Downloads**”。
```bash
$ cp /home/avi/Desktop/1.txt /home/avi/downloads
```
现在已经使用**cp**命令传递了两个参数。第一个是 “**/home/avi/Desktop/1.txt**”，第二个是 “**/home/avi/Downloads**”，以不同的方式处理它们，只需执行`echo [arguments]`以不同方式打印两个参数。
```bash
$ echo "1st Argument is : !^"
$ echo "2nd Argument is : !cp:2"
```
注意第一个参数可以打印为“!^”，其余参数可以通过执行“![Name_of_Command]:[Number_of_argument]”.<br />在上面的例子中，第一个命令是 “**cp**”，第二个参数需要打印。因此“!cp:2”，如果任何命令说**xyz**使用 5 个参数运行并且需要获得第 4 个参数，可以使用“!xyz:4”，并根据需要使用它。所有参数都可以通过 访问“!*”。<br />处理两个或更多参数
<a name="r3QPg"></a>
### 5、根据关键字执行最后一条命令
可以根据关键字执行上次执行的命令。可以这样理解：
```bash
$ ls /home > /dev/null      [Command 1]
$ ls -l /home/avi/Desktop > /dev/null    [Command 2] 
$ ls -la /home/avi/Downloads > /dev/null [Command 3]
$ ls -lA /usr/bin > /dev/null    [Command 4]
```
在这里，使用了相同的命令(ls)，**但使用了不同的开关和不同的文件夹。此外，已将每个命令的输出发送到 “**/dev/null”，因为不会处理命令的输出，控制台也保持干净。<br />现在根据关键字执行上次运行命令。
```bash
$ ! ls     [Command 1]
$ ! ls -l    [Command 2] 
$ ! ls -la    [Command 3]
$ ! ls -lA    [Command 4]
```
检查输出，可以发现只是通过ls关键字运行已执行的命令。<br />根据关键字运行命令
<a name="lpM1F"></a>
### 6、神奇的`!!`
可以使用(`!!`)它将在当前命令中使用 alter/tweak 调用最后一个运行命令。看以下场景<br />最后一天运行了一个单行脚本来获取私有 IP，所以运行：
```bash
$ ip addr show | grep inet | grep -v 'inet6'| grep -v '127.0.0.1' | awk '{print $2}' | cut -f1 -d/
```
然后突然发现需要将上面脚本的输出重定向到一个文件**ip.txt**，那该怎么办？应该再次重新输入整个命令并将输出重定向到文件吗？那么一个简单的解决方案是使用UP导航键并添加'> ip.txt'将输出重定向到文件。
```bash
$ ip addr show | grep inet | grep -v 'inet6'| grep -v '127.0.0.1' | awk '{print $2}' | cut -f1 -d/ > ip.txt
```
UP键可以快速找到历史命令。现在考虑以下条件，下次在单行脚本下运行时。
```bash
$ ifconfig | grep "inet addr:" | awk '{print $2}' | grep -v '127.0.0.1' | cut -f2 -d:
```
一旦运行脚本，bash 提示符就会返回一个错误消息“bash: ifconfig: command not found”，不难猜测以用户身份运行此命令，而它应该以 root 身份运行。<br />那么有什么解决办法呢？很难登录到 root 然后再次键入整个命令！此外，上一个示例中的（**向上导航键**）在这里也没有出现。所以？需要“!!”不带引号调用，这将调用该用户的最后一个命令。
```bash
$ su -c !! root
```
这里**su**是切换用户，也就是 root，`-c`以用户身份运行特定的命令，最重要的部分`!!`将被命令替换，最后运行的命令将在这里替换。是的！需要提供 root 密码。<br />`!!`主要在以下场景中使用，<br />当以普通用户身份运行**apt-get**命令时，通常会收到一个错误，说无权执行。
```bash
$ apt-get upgrade && apt-get dist-upgrade
```
Opps 错误... 不要担心执行以下命令以使其成功..
```bash
$ su -c !!
```
```bash
$ service apache2 start
or
$ /etc/init.d/apache2 start
or
$ systemctl start apache2
```
OOPS 用户无权执行此类任务，所以运行..
```bash
$ su -c 'service apache2 start'
or
$ su -c '/etc/init.d/apache2 start'
or
$ su -c 'systemctl start apache2'
```
<a name="tlNLR"></a>
### 7、运行影响除 ![FILE_NAME] 之外的所有文件的命令
该!（**逻辑 NOT**）可以用来运行所有命令的文件 / 文件扩展名，除了后面'!'。<br />**A.从目录中删除所有文件，但名称为2.txt**的文件除外。
```bash
$ rm !(2.txt)
```
B.从文件夹中删除所有文件类型，除了扩展名为 “**pdf**”的文件类型。
```bash
$ $ rm !(*.pdf)
```
<a name="wQz1L"></a>
### 8、检查一个目录（比如 / home/avi/Tecmint）是否存在？printf 如果所述目录存在与否。
在这里，将使用'! -d'来验证目录是否存在，然后使用逻辑 AND 运算符(&&)来打印该目录不存在，然后使用逻辑 OR 运算符(||)来打印目录是否存在。<br />逻辑是，当输出[ ! -d /home/rumenz ]为**0 时**，它将执行超出 Logical 的内容，**否则它将转到 Logical OR(||)并执行超出 Logical**OR 的内容。
```bash
$ [ ! -d /home/rumenz ] && printf '\nno such /home/rumenz directory exist\n' || printf '\n/home/rumenz directory exist\n'
```
<a name="PsppB"></a>
### 9、检查目录是否存在？如果没有退出命令。
与上述条件类似，但这里如果所需目录不存在，它将退出命令。
```bash
$ [ ! -d /home/rumenz ] && exit
```
<a name="ZVyL0"></a>
### 10、如果它不存在，则在主目录中创建一个目录（比如 rumenz）。
脚本语言中的一种通用实现，如果所需目录不存在，它将创建一个。
```bash
[ ! -d /home/rumenz ] && mkdir /home/rumenz
```
