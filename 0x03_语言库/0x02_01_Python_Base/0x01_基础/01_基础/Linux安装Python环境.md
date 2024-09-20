## Linux安装Python环境

### 通过Python解释器安装

+ linux

下载源代码python-3.6.6版本，解压安装

~~~ shell
[root@localhost ~]# yum install openssl openssl-devel cmake gcc ncurses-devel gcc-c++ libffi-devel -y
[root@localhost ~]# wget https://www.python.org/ftp/python/3.7.2/Python-3.7.2.tgz
[root@localhost ~]# tar -zxf Python-3.7.2.tgz
[root@localhost ~]# cd Python-3.7.2/
[root@localhost Python-3.7.2]# ./configure --prefix=/usr/local/python3.7
[root@localhost Python-3.7.2]# make
[root@localhost Python-3.7.2]# make install
[root@localhost Python-3.7.2]# ln -s /usr/local/python3.7/bin/python3.7 /usr/bin/python3
[root@localhost Python-3.7.2]# vim /etc/profile
PATH=/usr/local/python3.7/bin/:$PATH
export PATH
[root@localhost Python-3.7.2]# source /etc/profile
[root@localhost Python-3.7.2]# python3 -V
Python 3.6.6
~~~

安装第三方工具ipython

~~~ shell
[root@localhost Python-3.6.6]# pip3 install ipython
~~~

+ windows
+ + 可下载安装python的msi包直接安装；

+ + 自带python的GUI开发环境；

+ + 开发工具很多；



### 通过anaconda安装python环境(推荐)

1.感觉安装脚本较大,可以安装miniconda(https://conda.io/miniconda.html)1；

2.Anaconda指的是一个开源的Python发行版本，其包含了conda、Python等180多个科学包及其依赖项。 [1]  因为包含了大量的科学包，Anaconda 的下载文件比较大（约 531 MB），如果只需要某些包，或者需要节省带宽或存储空间，也可以使用Miniconda这个较小的发行版（仅包含conda和 Python）；

3.Conda是一个开源的包、环境管理器，可以用于在同一个机器上安装不同版本的软件包及其依赖，并能够在不同的环境之间切换；

~~~ shell
wget https://repo.anaconda.com/archive/Anaconda3-2019.10-Linux-x86_64.sh
chmod +x Anaconda3-2019.10-Linux-x86_64.sh
# 不想污染环境可以输入no
./Anaconda3-2019.10-Linux-x86_64.sh   
echo 'export PATH=/root/anaconda3/bin:$PATH' >> /etc/profile
source /etc/profile
~~~

<b>创建环境:(默认有一个base环境)</b>

~~~ shell
conda create --name python3.5 python=3.5
~~~

<b>切换环境:</b>

~~~ shell
windows :activate python3.5
linux/macos : source activate python3.5
~~~

![img3](D:\Note\python\图片\img3.png)

### 添加频道（这里什么情况？不太懂)

安装conda相当于买了一台电视机，但是有电视了不意味着你就能看节目了，你要手动添加频道才能看你想看的电视节目；

~~~ shell
conda config --add channels bioconda
conda config --add channels conda-forge
conda config --add channels genomedk
~~~

安装完成后可用pip命令安装相应的包,可用conda安装软件,包括ipython

~~~ shell 
pip install ipython
conda install ipython
~~~

