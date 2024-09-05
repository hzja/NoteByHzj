Linux bash 
<a name="NE1nG"></a>
# 1、下载编译最新版的bash
```bash
get http://ftp.gnu.org/gnu/bash/bash-4.4.tar.gz
tar zxvf bash-4.4.tar.gz
cd bash-4.4
./configure
make && make install
```
<a name="pSnoh"></a>
# 2、配置Bash为安装的最新版本的Bash
```bash
vim /etc/profile.d/bash.sh         #编辑用户自定义配置，输入alias参数
alias python='/local/tools/bash/bash-4.4/bash'　　#这里写bash的安装路径以及bash的可执行文件名
source /etc/profile.d/bash.sh     #重启会话使配置生效
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596989644723-f3a23cb3-8eaf-42ab-b229-c634ae6793f0.png#align=left&display=inline&height=199&originHeight=597&originWidth=3323&size=681929&status=done&style=none&width=1107.6666666666667)
<a name="yEFYq"></a>
# 3、bash命令的语法格式
语法格式：bash [参数] [执行的bash文件]
<a name="sLNNE"></a>
#  4、bash命令的常用参数
| -n | 检测脚本是否正确，并不执行脚本 |
| --- | --- |
| -x | 执行脚本，输出执行过程 |
| -c | bash从字符串中读入命令，如果字符串后还有变量就被设定为从$0开始的位置参数 |


