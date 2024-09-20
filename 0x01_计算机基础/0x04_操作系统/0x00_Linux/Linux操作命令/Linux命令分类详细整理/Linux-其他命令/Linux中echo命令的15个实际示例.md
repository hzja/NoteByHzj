Linux<br />echo是Linux bash和C shell中最常用和最广泛使用的内置命令之一。<br />echo的语法：
```bash
echo [option(s)] [string(s)]
```
<a name="BFH51"></a>
### 1、输入一行文本并显示在标准输出上
```bash
$ echo rumenz is a community of Linux Nerds
```
输出以下文本：
```
rumenz is a community of Linux Nerds
```
<a name="qTNof"></a>
### 2、声明一个变量并打印它的值。例如：声明一个变量x并分配其值 =10。
```bash
$ x=10
```
打印：
```bash
$ echo The value of variable x = $x 

The value of variable x = 10
```
Note：这 `-e` Linux 中的选项充当反斜杠转义字符的解释。
<a name="tWPhU"></a>
### 3、使用选项`\b` 退格符和反斜杠解释器`-e`，删除中间的所有空格。
```bash
$ echo -e "rumenz \bis \ba \bcommunity \bof \bLinux \bNerds" 

rumenzisacommunityofLinuxNerds
```
<a name="dVgqd"></a>
### 4、使用选项`\n`和`-e`换行输出。
```bash
$ echo -e "rumenz \nis \na \ncommunity \nof \nLinux \nNerds" 

rumenz 
is 
a 
community 
of 
Linux 
Nerds
```
<a name="pWvsg"></a>
### 5、使用选项 `\t`水平制表符和`-e`。
```bash
$ echo -e "rumenz \tis \ta \tcommunity \tof \tLinux \tNerds" 

rumenz  is  a  community  of  Linux  Nerds
```
<a name="ydIhl"></a>
### 6、同时使用选项`\n`和水平制表符`\t`
```bash
$ echo -e "\n\trumenz \n\tis \n\ta \n\tcommunity \n\tof \n\tLinux \n\tNerds" 


        rumenz 
        is 
        a 
        community 
        of 
        Linux 
        Nerds
```
<a name="mZru7"></a>
### 7、使用选项 `\v`垂直制表符和`-e`
```bash
$ echo -e "\vrumenz \vis \va \vcommunity \vof \vLinux \vNerds" 

rumenz 
        is 
           a 
             community 
                       of 
                          Linux 
                                Nerds
```
<a name="bdVLY"></a>
### 8、同时使用选项`\n`和垂直制表符`\v`。
```bash
$ echo -e "\n\vrumenz \n\vis \n\va \n\vcommunity \n\vof \n\vLinux \n\vNerds" 


rumenz 

is 

a 

community 

of 

Linux 

Nerds
```
Note：可以使用该选项两次或根据需要多次将垂直制表符、水平制表符和新行间距加倍。
<a name="qABos"></a>
### 9、使用选项 `\r`和`-e`光标移至行首，但不换行。
```bash
$ echo -e "rumenz \ris a community of Linux Nerds" 

is a community of Linux Nerds
```
<a name="gpAiJ"></a>
### 10、使用选项 `\c`和`-e`不换行输出，`\c`后面的字符不显示。
```bash
$ echo -e "rumenz is a community \cof Linux Nerds" 

rumenz is a community rumenz@local:~$
```
<a name="nhKOz"></a>
### 11、使用`-n`不输出末尾的换行符。
```bash
$ echo -n "rumenz is a community of Linux Nerds" 
rumenz is a community of Linux Nerdsrumenz@local:~/Documents$
```
<a name="xR4WF"></a>
### 12、使用选项`\a`带有退格解释器的警报返回`-e`要有声音警报。
```bash
$ echo -e "rumenz is a community of \aLinux Nerds" 
rumenz is a community of Linux Nerds
```
Note：确保在执行前检查音量键。
<a name="LCTdQ"></a>
### 13、使用echo命令打印所有文件。
```bash
$ echo * 

103.odt 103.pdf 104.odt 104.pdf 105.odt 105.pdf 106.odt 106.pdf 107.odt 107.pdf 108a.odt 108.odt 108.pdf 109.odt 109.pdf 110b.odt 110.odt 110.pdf 111.odt 111.pdf 112.odt 112.pdf 113.odt linux-headers-3.16.0-customkernel_1_amd64.deb linux-image-3.16.0-customkernel_1_amd64.deb network.jpeg
```
<a name="K5pAg"></a>
### 14、打印所有.jpeg文件，请使用以下命令。
```bash
$ echo *.jpeg 

network.jpeg
```
<a name="tIW6W"></a>
### 15、echo可以与重定向运算符一起使用以输出到文件而不是标准输出。
```bash
$ echo "rumenz.com" > 1.txt 


rumenz@local:~$ cat 1.txt 
rumenz.com
```
<a name="NX13u"></a>
### echo选项
| Options | Description |
| --- | --- |
| -n | do not print the trailing newline. |
| -e | enable interpretation of backslash escapes. |
| \\b | backspace |
| \\ | backslash |
| \\n | new line |
| \\r | carriage return |
| \\t | horizontal tab |
| \\v | vertical tab |

