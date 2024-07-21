Nginx <br />在企业实际生产环境中经常遇到的一个情况，升级Nginx到新的版本和如何回滚至旧版本。
<a name="boIv4"></a>
## 1、环境介绍
准备两个Nginx版本如下：
```bash
[root@nginx ~]# cd /download/nginx/
[root@nginx nginx]# ll
total 1952
-rw-r--r-- 1 root root  981687 Oct 17  2017 nginx-1.12.2.tar.gz
-rw-r--r-- 1 root root 1015384 Dec  4 09:58 nginx-1.14.2.tar.gz
```
<a name="kXxaD"></a>
## 2、编译安装新旧版本
<a name="rj2NP"></a>
### 编译安装nginx-1.12.2
```bash
[root@nginx nginx]# tar zxf nginx-1.12.2.tar.gz 
[root@nginx nginx]# cd nginx-1.12.2
[root@nginx nginx-1.12.2]# ./configure --prefix=/usr/local/nginx-1.12.2
[root@nginx nginx-1.12.2]# echo $?
0
[root@nginx nginx-1.12.2]# make && make install
[root@nginx nginx-1.12.2]# echo $?
0
[root@nginx nginx-1.12.2]# ll /usr/local/nginx-1.12.2/
total 0
drwxr-xr-x 2 root root 333 Mar  1 09:01 conf
drwxr-xr-x 2 root root  40 Mar  1 09:01 html
drwxr-xr-x 2 root root   6 Mar  1 09:01 logs
drwxr-xr-x 2 root root  19 Mar  1 09:01 sbin
```
<a name="sQC3B"></a>
### 编译安装nginx-1.14.2
```bash
[root@nginx ~]# cd /download/nginx/
[root@nginx nginx]# tar zxf nginx-1.14.2.tar.gz 
[root@nginx nginx]# cd nginx-1.14.2
[root@nginx nginx-1.14.2]# ./configure --prefix=/usr/local/nginx-1.14.2
[root@nginx nginx-1.14.2]# echo $?
0
[root@nginx nginx-1.14.2]# make && make install
[root@nginx nginx-1.14.2]# echo $?
0
[root@nginx nginx-1.14.2]# ls -l /usr/local/nginx-1.14.2/
total 0
drwxr-xr-x 2 root root 333 Mar  1 09:03 conf
drwxr-xr-x 2 root root  40 Mar  1 09:03 html
drwxr-xr-x 2 root root   6 Mar  1 09:03 logs
drwxr-xr-x 2 root root  19 Mar  1 09:03 sbin
```
到这里，两个版本的Nginx软件已经部署完成。
<a name="QPr3K"></a>
## 3、启动旧版本Nginx
```bash
[root@nginx ~]# /usr/local/nginx-1.12.2/sbin/nginx -t
nginx: the configuration file /usr/local/nginx-1.12.2/conf/nginx.conf syntax is ok
nginx: configuration file /usr/local/nginx-1.12.2/conf/nginx.conf test is successful
[root@nginx ~]# /usr/local/nginx-1.12.2/sbin/nginx
[root@nginx ~]# ps -ef|grep nginx
root       6324      1  0 09:06 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
nobody     6325   6324  0 09:06 ?        00:00:00 nginx: worker process
root       6327   1244  0 09:06 pts/0    00:00:00 grep --color=auto nginx
[root@nginx ~]# lsof -i :80
COMMAND  PID   USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
nginx   6324   root    6u  IPv4  26324      0t0  TCP *:http (LISTEN)
nginx   6325 nobody    6u  IPv4  26324      0t0  TCP *:http (LISTEN)
```
<a name="tTMjR"></a>
## 4、升级到新版本
版本升级其实就是针对二进制文件的升级，过程如下：
```bash
[root@nginx ~]# /usr/local/nginx-1.12.2/sbin/nginx -v
nginx version: nginx/1.12.2
[root@nginx ~]# cd /usr/local/nginx-1.12.2/sbin/
[root@nginx sbin]# mv nginx nginx-1.12.2
#首先备份原来的旧版本nginx二进制文件
[root@nginx sbin]# cp /usr/local/nginx-1.14.2/sbin/nginx ./
#拷贝新版本的二进制文件到当前目录
```
接下来进行平滑升级操作
```bash
[root@nginx ~]# ps -ef|grep nginx
root       6324      1  0 09:06 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
nobody     6325   6324  0 09:06 ?        00:00:00 nginx: worker process
root       6338   1244  0 09:11 pts/0    00:00:00 grep --color=auto nginx
[root@nginx ~]# kill -USR2 6324
[root@nginx ~]# ps -ef|grep nginx
root       6324      1  0 09:06 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
nobody     6325   6324  0 09:06 ?        00:00:00 nginx: worker process
root       6340   6324  0 09:12 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
nobody     6341   6340  0 09:12 ?        00:00:00 nginx: worker process
root       6343   1244  0 09:12 pts/0    00:00:00 grep --color=auto nginx
```
这时新的master进程已经正常开启，但老的work进程也存在，所以使用下面的命令，将老的work进程发出平滑停止的信号，如下：
```bash
[root@nginx ~]# kill -WINCH 6324
[root@nginx ~]# ps -ef|grep nginx
root       6324      1  0 09:06 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
root       6340   6324  0 09:12 ?        00:00:00 nginx: master process /usr/local/nginx-1.12.2/sbin/nginx
nobody     6341   6340  0 09:12 ?        00:00:00 nginx: worker process
root       6346   1244  0 09:14 pts/0    00:00:00 grep --color=auto nginx
```
从上面的结果发现，已经平滑的回滚的上一个版本，接下来测试是否能正常访问：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597893420085-1b81bda9-175d-499d-86e4-bcf26669934b.png#height=324&id=qeMma&originHeight=972&originWidth=3837&originalType=binary&size=264918&status=done&style=shadow&width=1279)<br />一样可以正常访问，所以，这个回滚的操作对用户来说也是不可感知的。
