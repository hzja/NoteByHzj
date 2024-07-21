## kali安装vulhub

### 切换root环境

~~~ tex 
sudo -i
~~~

切换root环境

![切换root环境](.\图片\切换root环境.PNG)



### 一键安装Docker

<b>未安装过Docker</b>的机器上，<b>root权限</b>执行如下命令即可一键安装最新版Docker：

~~~ tex
curl -s https://get.docker.com/ | sh 
~~~



<b>已安装过老版本Docker</b>（且不是用这个一键安装脚本安装的），先卸载Docker:

~~~ tex
sudo apt purge --autoremove docker.io
~~~

再一键安装。



### 安装docker-compose

Docker-compose用于组合服务与内网，有部分环境涉及到多个容器且不同环境开启的端口目录也不相同，所以Vulhub选择使用docker-compose做环境一键化管理工具。用户不需学习docker命令的各种参数与用法，只需简单执行`docker-compose up -d`即可启动容器；

Docker-compose基于Python开发，所以可用pip安装：

~~~ tex
pip install docker-compose
~~~



pip安装：

~~~ tex
curl -s https://bootstrap.pypa.io/get-pip.py | python3
~~~



### 下载vulhub

~~~ tex
git clone https://github.com/vulhub/vulhub.git
~~~



### 启动漏洞环境

docker-compose会自动查找当前目录下的配置文件（默认文件名为`docker-compose.yml`），并根据其内容编译镜像和启动容器。所以，要运行某个漏洞靶场，需要先进入该漏洞所在的目录。



在Vulhub中选择某个环境，进入对应目录。如Flask服务端模板注入漏洞，我们进入`flask/ssti`目录：

~~~ tex
cd flask/ssti
~~~



直接执行如下命令，进行漏洞靶场的编译和运行：

~~~ tex
# 可选
docker-compose build

docker-compose up -d
~~~

![启动环境一](.\图片\启动环境一.PNG)

`docker-compose up -d`运行后，会自动查找当前目录下的配置文件。如果配置文件中包含的环境均已经存在，则不会再次编译；如果配置文件中包含的环境不存在，则会自动进行编译。所以，其实`docker-compose up -d`命令是包含了`docker-compose build`的。

如果更新了配置文件，可手工执行`docker-compose build`重新编译靶场环境



#### 启动成功

![启动成功](.\图片\启动成功.PNG)



### 自身IP地址网址

~~~ tex
https://www.ipaddress.com/what-is-my-ip-address/
~~~



### 报错

#### 报错一

![报错一](.\图片\报错一.PNG)

~~~ tex
docker.errors.DockerException: Error while fetching server API version: ('Connection aborted.', PermissionError(13, 'Permission denied'))
docker.errors.DockerException:: command not found
~~~



##### 原因

~~~ tex
docker未成功启动
~~~



##### 办法

1. 开启docker

~~~ tex
systemctl start docker
~~~



2. 查看docker进程

~~~ tex
ps -ef | grep docker
~~~



3. 执行

~~~ tex
docker-compose up -d
~~~



#### 报错二

![报错二](.\图片\报错二.PNG)

~~~ tex
usr/lib/python3/dist-packages/paramiko/transport.py:236: CryptographyDeprecationWarning: Blowfish has been deprecated
  "class": algorithms.Blowfish,
Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/urllib3/connectionpool.py", line 704, in urlopen
    httplib_response = self._make_request(
  File "/usr/lib/python3/dist-packages/urllib3/connectionpool.py", line 399, in _make_request
    conn.request(method, url, **httplib_request_kw)
  File "/usr/lib/python3.10/http/client.py", line 1282, in request
    self._send_request(method, url, body, headers, encode_chunked)
  File "/usr/lib/python3.10/http/client.py", line 1328, in _send_request
    self.endheaders(body, encode_chunked=encode_chunked)
  File "/usr/lib/python3.10/http/client.py", line 1277, in endheaders
    self._send_output(message_body, encode_chunked=encode_chunked)
  File "/usr/lib/python3.10/http/client.py", line 1037, in _send_output
    self.send(msg)
  File "/usr/lib/python3.10/http/client.py", line 975, in send
    self.connect()
  File "/usr/lib/python3/dist-packages/docker/transport/unixconn.py", line 30, in connect
    sock.connect(self.unix_socket)
PermissionError: [Errno 13] Permission denied
~~~

##### 解决办法

1. 切换超级用户下执行命令:

~~~ tex
sudo -i
~~~



2. 重新执行命令即可



#### 报错三

![报错三](.\图片\报错三.PNG)

~~~ tex
ERROR: for web  Cannot start service web: driver failed programming external connectivity on endpoint cve-2022-34265_web_1 (053da23b75918440c53007ad4ee28ffab01542a6f49218299b62da71c0351e59): Bind for 0.0.0.0:8000 failed: port is already allocated
ERROR: Encountered errors while bringing up the project.
~~~

一句话就是容器占用的port还没有完全释放



##### 方法一

###### 查看进程

~~~ tex
$ docker ps
检查docker镜像
~~~

~~~ tex
$ ps -aux | grep -v grep | grep docker-proxy
查看被docker-proxy占用的端口
~~~



###### 停docker进程

删除所有容器，然后删除 local-kv.db 这个文件，再启动 docker 就可

~~~ tex
$ sudo service docker stop
$ docker rm $(docker ps -aq)
$ sudo rm /var/lib/docker/network/files/local-kv.db
$ sudo service docker start
~~~



##### 方法二(推荐)

看端口是否映射

~~~ tex
docker ps
~~~

发现ports列表有：

![报错三解决方法二-1](.\图片\报错三解决方法二-1.PNG)

停止该容器：

~~~ tex
docker kill 685d872d0d6a
~~~

重新启动发现已经解决问题:

![报错三解决方法二-2](.\图片\报错三解决方法二-2.PNG)
