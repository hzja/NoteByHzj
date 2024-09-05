Linux CentOS Python 
<a name="jv7kX"></a>
# 安装Python之前需要安装（否则有的地方还需要安装，会报错，需要重新编译）
```bash
yum -y install libffi-devel
```
<a name="MJ90I"></a>
# 1、安装Python3.8
如果通过curl下载比较慢可以下载下面的Python安装包使用FTP工具上传至Linux系统下<br />[Python-3.8.0.tgz](https://www.yuque.com/attachments/yuque/0/2020/tgz/396745/1596864974055-77390aaf-0d20-4198-8e07-17cff3c1b2f5.tgz?_lake_card=%7B%22uid%22%3A%221596864966691-0%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Ftgz%2F396745%2F1596864974055-77390aaf-0d20-4198-8e07-17cff3c1b2f5.tgz%22%2C%22name%22%3A%22Python-3.8.0.tgz%22%2C%22size%22%3A23949883%2C%22type%22%3A%22application%2Fx-compressed%22%2C%22ext%22%3A%22tgz%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22khsCw%22%2C%22card%22%3A%22file%22%7D)<br />通过curl以及Window的xsync上传对比<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596865098661-b490c947-1de7-48bf-9d4f-c68a8f7919dd.png#align=left&display=inline&height=74&originHeight=223&originWidth=3323&size=310289&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596865131230-9c09ab5f-7bcc-47a6-b465-8514d09f5092.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2142996&status=done&style=none&width=1107.6666666666667)
```bash
yum -y install yum-utils
yum-builddep python
curl -O https://www.python.org/ftp/python/3.8.0/Python-3.8.0.tgz
tar xf Python-3.8.0.tgz
cd Python-3.8.0
./configure --prefix=/local/tools/Python-3.8.0
make && make install
```
<a name="dRW1e"></a>
# 2、设置Python3.8为默认版本
```bash
vim /etc/profile.d/python.sh         #编辑用户自定义配置，输入alias参数
alias python='/usr/local/bin/python3.8/python'　　#这里写python的安装路径以及Python的可执行文件名
source /etc/profile.d/python.sh     #重启会话使配置生效
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596866093201-6fb26334-431e-4edc-8332-c275c6dce6c6.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2250014&status=done&style=none&width=1107.6666666666667)
<a name="GAmtw"></a>
# 3、Python3.8编译安装后bin目录有pip3，只需要创建软链接即可
```bash
[root@iZuligp6e1dyzfZ bin]# ln -s /local/tools/Python-3.8.0/bin/pip3 /usr/local/bin/pip3
[root@iZuligp6e1dyzfZ bin]# pip3 -V
pip 19.2.3 from /local/tools/Python-3.8.0/lib/python3.8/site-packages/pip (python 3.8)
```
