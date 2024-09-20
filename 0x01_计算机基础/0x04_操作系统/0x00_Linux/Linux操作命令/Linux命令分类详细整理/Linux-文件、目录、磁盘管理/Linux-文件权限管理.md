Linux chmod chgrp chown
<a name="2RAdI"></a>
## 文件权限
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596081001157-0059dac7-2550-4ab3-91b3-76a0bb4bdda0.png#averageHue=%23eeeeee&height=155&id=Pk7b6&originHeight=464&originWidth=1029&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57860&status=done&style=shadow&title=&width=343)

| **用户权限（user）** |  |  | **组权限（group）** |  |  | **其他用户** |  |  |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **r** | **w** | **x** | **r** | **w** | **x** | **r** | **w** | **x** |
| **4** | **2** | **1** | **4** | **2** | **1** | **4** | **2** | **1** |

- 每一类用户都有3个权限
   - r：读取权限， 只读
   - w: 写入权限 ， 可以编辑文件
   - x:  执行权限， 由于某些文件是可以执行的，如有有执行权限才可以执行
- 操作权限
   - 添加权限：chmod 用户或组+权限  资源文件或目录
   - 删除权限：chmod 用户或组-权限  资源文件或目录
   - 修改权限：chmod 用户或组=权限  资源文件或目录
<a name="JtrD8"></a>
## 权限操作命令
<a name="H4Qda"></a>
### chmod-更改文件或目录的用户权限
语法格式：chmod [参数] [文件]<br />常用参数：

| -c | 若该文件权限确实已经更改，才显示其更改动作 |
| --- | --- |
| -f | 若该文件权限无法被更改也不显示错误讯息 |
| -v | 显示权限变更的详细资料 |
| -R | 对目前目录下的所有文件与子目录进行相同的权限变更(即以递回的方式逐个变更) |

<a name="yMWCy"></a>
### chgrp-更改文件的用户组
语法格式: chgrp [参数] [目录]<br />常用参数：

| -c | 效果类似”-v”参数，但仅回报更改的部分 |
| --- | --- |
| -f | 不显示错误信息 |
| -h | 对符号连接的文件作修改，而不更动其他任何相关文件 |
| -R | 递归处理，将指定目录下的所有文件及子目录一并处理 |

<a name="gRDXE"></a>
### chown-更改文件或目录的用户和用户组
语法格式：chown [参数]<br />常用参数：

| -R | 对目前目录下的所有文件与子目录进行相同的拥有者变更 |
| --- | --- |
| -c | 若该文件拥有者确实已经更改，才显示其更改动作 |
| -f | 若该文件拥有者无法被更改也不要显示错误讯息 |
| -h | 只对于连结(link)进行变更，而非该 link 真正指向的文件 |
| -v | 显示拥有者变更的详细资料 |

<a name="XA2jT"></a>
## 常见的权限操作
<a name="98a315c0"></a>
### 授权
```bash
$ chmod 777 file/dir
```
<a name="zd0F6"></a>
### 添加可执行权限
```bash
$ chmod +x file
```
<a name="e4C2Z"></a>
## 导出以及恢复文件的权限
<a name="aDicm"></a>
### 导出文件的权限
```bash
$ getfacl -R ./ > chmod.txt
```
<a name="GIkWy"></a>
### 从导出的权限文件中恢复文件的权限
```bash
$ setfacl --restore=chmod.txt
```
<a name="TcPEY"></a>
## Linux下的s、t、i、a权限
<a name="zbpoe"></a>
### s：文件属主和组设置SUID和GUID
文件在被设置了`s`权限后将以root身份执行。在设置`s`权限时文件属主、属组必须先设置相应的`x`权限，否则`s`权限并不能正真生效（`chmod`命令不进行必要的完整性检查，即使不设置`x`权限就设置`s`权限，`chmod`也不会报错，当`ls -l`时看到`rwS`，大写`S`说明`s`权限未生效）。Linux修改密码的passwd便是一个设置了SUID的程序，普通用户无读写/etc/shadow文件的权限但是却可以修改自己的密码。
```bash
ls -al /usr/bin/passwd
-rwsr-xr-x 1 root root 32988 2008-12-08 17:17 /usr/bin/passwd
```
可以通过字符模式设置`s`权限：`chmod a+s filename`，也可以使用绝对模式进行设置：
> 设置suid：将相应的权限位之前的那一位设置为4；
> 设置guid：将相应的权限位之前的那一位设置为2；
> 两者都置位：将相应的权限位之前的那一位设置为4+2=6。

如：
```bash
chmod 4764 filename   //设置SUID
```
<a name="GUoW0"></a>
### t：设置粘着位
一个文件可读写的用户并一定想让他有删除此文件的权限，如果文件设置了`t`权限则只用属主和root有删除文件的权限，通过`chmod +t filename` 来设置`t`权限。
<a name="0kBOk"></a>
### i：不可修改权限
例：`chattr +i filename` 则filename文件就不可修改，无论任何人，如果需要修改需要先删除i权限，用`chattr -i filename`就可以了。查看文件是否设置了`i`权限用`lsattr filename`命令。
<a name="5imEo"></a>
### a：只追加权限
对于日志系统很好用，这个权限让目标文件只能追加，不能删除，而且不能通过编辑器追加。可以使用`chattr +a`设置追加权限。
<a name="obmnI"></a>
## 文件属性
chattr 命令用于改变文件属性<br />这项指令可改变存放在ext2文件系统上的文件或目录属性，这些属性共有以下8种模式：

- a：让文件或目录仅供附加用途。
- b：不更新文件或目录的最后存取时间。
- c：将文件或目录压缩后存放。
- d：将文件或目录排除在倾倒操作之外。
- i：不得任意更动文件或目录。
- s：保密性删除文件或目录。
- S：即时更新文件或目录。
- u：预防意外删除。
<a name="nEh6q"></a>
### 语法
```bash
chattr [-RV][-v<版本编号>][+/-/=<属性>][文件或目录...]
```
<a name="qLFgk"></a>
### 参数

- -R 递归处理，将指定目录下的所有文件及子目录一并处理。
- -v<版本编号> 设置文件或目录版本。
- -V 显示指令执行过程。
- +<属性> 开启文件或目录的该项属性。
- -<属性> 关闭文件或目录的该项属性。
- =<属性> 指定文件或目录的该项属性。
<a name="x67LR"></a>
### 实例
```bash
# 不能改动文件
chattr +i /etc/resolv.conf
# 查看权限
lsattr /etc/resolv.conf
----i-------- /etc/resolv.conf
# 只能追加不能删除
chattr +a /var/log/messages
```
