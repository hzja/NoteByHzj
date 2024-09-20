Linux
<a name="tbbIf"></a>
## 1、概述
Linux是一个多用户的操作系统。多用户意味着每个用户登录系统后，都有自己专用的运行环境。而这个环境是由一组变量所定义，这组变量被称为环境变量。用户可以对自己的环境变量进行修改以达到对环境的要求。环境变量是和Shell紧密相关的，它是通过Shell命令来设置的。环境变量又可以被所有当前用户所运行 的程序所使用。对于bash来说，可以通过变量名来访问相应的环境变量。
<a name="xFYbV"></a>
## 2、显示
使用`env`命令或者`printenv`命令可以察看所有环境变量。<br />使用`echo`显示特定环境变量值，eg：
```bash
$ echo $HOME
/home/admin
```
<a name="LTVL3"></a>
## 3、设置一个新的环境变量
```bash
$ export NAME= " Example "
$ echo $NAME
Example
```
<a name="iSsih"></a>
## 4、使用`unset`命令来清除环境变量
```bash
$ unset NAME
$ echo $NAME
```
<a name="mQRov"></a>
## 5、使用`readonly`命令设置只读变量（设置为只读属性后，其值将不可变，直到进程结束）
```bash
$ export NAME= " Example "
$ readonly NAME

$ unset NAME
-bash: unset: NAME: cannot unset: readonly variable
$ NAME= " New " #会发现此也变量不能被修改
-bash: TEST: readonly variable
```
<a name="q3HXi"></a>
## 6、在C程序中访问和设置环境变量
对于C程序的用户来说，可以使用下列三个函数来设置或访问一个环境变量：

- `getenv()`访问一个环境变量。输入参数是需要访问的变量名字，返回值是一个字符串。如果所访问的环境变量不存在，则会返回NULL
- `setenv()`在程序里面设置某个环境变量的函数
- `unsetenv()`清除某个特定的环境变量的函数

另外，还有一个全局的指针变量`environ`，它指向的是包含所有的环境变量的一个列表。下面的程序可以打印出当前运行环境里面的所有环境变量：
```c
#include
extern char**environ;
int main ()
{
    char**var;
    for (var =environ;*var !=NULL;++var)
    printf ( " %s \n " ,*var);
    return 0;
}
```
<a name="E1KBJ"></a>
## 7、修改系统配置文件
通过修改一些相关的环境定义文件来修改环境变量，与环境相关的文件有/etc/profile（root用户）和~/.bash_profile（普通用户，在自己的目录下，隐藏文件，如果没有可以新建）等。修改完毕后重新登录一次或运行命令`source xxx`就生效了。
<a name="r6EIx"></a>
## 8、常见的环境变量
PATH 决定了shell将到哪些目录中寻找命令或程序<br />HOME 当前用户主目录<br />TERM 终端的类型。<br />UID 当前用户的识别字，取值是由数位构成的字串。<br />PWD 当前工作目录的绝对路径名，该变量的取值随cd命令的使用而变化。<br />HISTSIZE 历史记录数<br />LOGNAME 当前用户的登录名<br />HOSTNAME 指主机的名称<br />SHELL 前用户Shell类型<br />LANGUGE 语言相关的环境变量，多语言可以修改此环境变量<br />MAIL 当前用户的邮件存放目录<br />PS1 shell命令行基本提示符，对于root用户是`#`，对于普通用户是`$`<br />PS2 附属提示符，默认是">"
