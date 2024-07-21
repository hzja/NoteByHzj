Docker
<a name="LNyht"></a>
## Docker 启动容器
Docker 启动容器有两种方式：

1. **基于镜像新建一个容器并启动；**
2. **将处于终止状态 （exited）的容器重新启动；**
<a name="wWi27"></a>
### 一、基于镜像新建容器并启动
通过如下命令来启动容器：
```bash
docker run IMAGE:TAG
```
参数说明：

- **IMAGE：镜像名称；**
- **TAG：标签，镜像版本号；**

举个例子🌰，比如想启动一个 ubuntu 容器，并通过终端输出 Hello world：<br />![docker 启动容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470293489-7631842b-f30d-4a71-b876-2a4bc58a1db1.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u999b3a73&originHeight=230&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u7817742d-b605-47de-bdc3-857f4eb2ff6&title=docker%20%E5%90%AF%E5%8A%A8%E5%AE%B9%E5%99%A8 "docker 启动容器")<br />执行命令如下：
```bash
docker run ubuntu:latest /bin/echo 'Hello world'
```
![docker 启动 ubuntu 容器并输出 hello world](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470293553-d98f7001-737d-41e7-97ed-a5ff70d16994.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u10ba51d4&originHeight=113&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=udce4dc7f-2c74-4ecf-8093-7b59545fef9&title=docker%20%E5%90%AF%E5%8A%A8%20ubuntu%20%E5%AE%B9%E5%99%A8%E5%B9%B6%E8%BE%93%E5%87%BA%20hello%20world "docker 启动 ubuntu 容器并输出 hello world")<br />注意，上面这行命令运行容器，在输出 Hello world 后就会终止运行。如果想以交互的方式运行容器，执行如下命令：
```bash
docker run -t -i ubuntu:latest /bin/bash
```
![以交互的方式运行 docker 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470293556-13d8a060-1a2c-4b22-9dc8-6c1f5a9113d4.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=ub79ffb91&originHeight=105&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u90d3fa7f-20d8-462b-979f-32280fb8bc2&title=%E4%BB%A5%E4%BA%A4%E4%BA%92%E7%9A%84%E6%96%B9%E5%BC%8F%E8%BF%90%E8%A1%8C%20docker%20%E5%AE%B9%E5%99%A8 "以交互的方式运行 docker 容器")<br />参数说明：

- `**-t**`**：让 Docker 分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上;**
- `**-i**`**：让容器的标准输入保持打开;**

如此，就可以直接在终端执行命令了：
```bash
root@c355025f2241:/# ls
bin  boot  dev  etc  home  lib  lib32  lib64  libx32  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
root@c355025f2241:/# date
Sun Jul  3 03:47:41 UTC 2022
```
<a name="a9fWC"></a>
#### 1.1 拓展知识：执行 `docker run` 后，Docker 都干了些啥？

- **检查本地是否存在指定的镜像，不存在就从 registry 下载**
- **利用镜像创建并启动一个容器**
- **分配一个文件系统，并在只读的镜像层外面挂载一层可读写层**
- **从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去**
- **从地址池配置一个 ip 地址给容器**
- **执行用户指定的应用程序**
- **执行完毕后容器被终止**
<a name="Z4cX9"></a>
#### 1.2 以 daemon 守护态方式运行容器
实际应用中，需要让容器以后台的方式运行，以此保证一个稳定的服务。<br />运行容器时添加 `-d`  参数可以让容器以后台的方式运行，例如启动一个 Ubuntu 后台容器，并每隔一秒打印 Hello world，命令如下：
```bash
docker run -d ubuntu:latest /bin/sh -c "while true; do echo hello world; sleep 1; done"
```
执行成功后，会返回一个容器 ID。<br />![docker 以后台的方式运行容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470293578-08dc1cdb-2c9e-4fd4-accd-3227e1a35ae9.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u31a689c5&originHeight=57&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u8b2a01e3-1cd4-4205-85b0-10b5291efdc&title=docker%20%E4%BB%A5%E5%90%8E%E5%8F%B0%E7%9A%84%E6%96%B9%E5%BC%8F%E8%BF%90%E8%A1%8C%E5%AE%B9%E5%99%A8 "docker 以后台的方式运行容器")<br />后台运行的容器可以通过 `docker logs` 命令来查看日志：
```bash
docker container logs [container ID or NAMES]
```
<a name="tp5nf"></a>
### 二、启动终止状态的容器
执行如下命令，可以将已经终止 （exited）运行的容器重启。
```bash
docker container start [container ID or NAMES]
```
测试一波好不好使，先通过 `docker ps` 命令获取正在运行的容器：<br />![获取 docker 容器 id](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470293562-4cd59c87-b765-4c0d-9b12-4f5bcdb11e33.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u43d6abd1&originHeight=86&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ucc4465c5-4043-4e0c-bf5d-10114abe0e8&title=%E8%8E%B7%E5%8F%96%20docker%20%E5%AE%B9%E5%99%A8%20id "获取 docker 容器 id")<br />本地有个 Redis 容器正在运行，先拿到容器 ID 或者容器名称，执行 `docker stop` 停止它后，再执行 `docker container start` 命令重启这个 Redis 容器：<br />![重启 redis 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294037-bf19b334-4586-4395-a515-f5ed23f1febe.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=ubc4c7a20&originHeight=166&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ubef206d2-ac95-4769-a28e-7c6da072c60&title=%E9%87%8D%E5%90%AF%20redis%20%E5%AE%B9%E5%99%A8 "重启 redis 容器")<br />可以看到，成功重启了一个终止状态的容器。
<a name="gfTSP"></a>
## Docker 查看容器
<a name="WPKab"></a>
### 查看正在运行中、停止运行的容器

- `**docker ps**`**：查看正在运行中的容器；**
- `**docker ps -a**`**：查看所有容器，包括运行中的、已经停止运行的容器。**

![docker 查看容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294071-a7bf5e67-47c0-4f2f-ada5-1e100418f284.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=ua9dcaa35&originHeight=244&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u30e384db-9f92-48ea-8101-16a2f2bca07&title=docker%20%E6%9F%A5%E7%9C%8B%E5%AE%B9%E5%99%A8 "docker 查看容器")<br />返回字段说明：

- **CONTAINER ID：容器 ID；**
- **IMAGE：创建容器时使用的镜像；**
- **COMMAND：容器最后运行的命令；**
- **CREATED：容器创建时间；**
- **STATUS：容器状态；**
- **PORTS：端口信息；**
- **NAMES ：容器名：和容器 ID 一样，可以标识容器的唯一性，同一台宿主机上不允许有同名容器存在，否则会冲突；**
<a name="TqrVh"></a>
### **拓展命令**

- `**docker ps -l**`**：查看最新创建的容器，注意，只列出最后创建的容器。**
- `**docker ps -n=2: -n=2**`** 指定列出最新创建的 2 个容器。**
<a name="J3t2A"></a>
## Docker 进入容器
要想进入到已运行的 Docker 容器，主要有如下两种方式：

- **使用 **`**docker exec**`** 命令;**
- **使用 **`**docker attach**`** 命令;**
<a name="hWJEu"></a>
### 一、exec 命令（推荐）
Docker 在 1.3.X 版本后开始支持 `exec` 命令进入容器，命令如下：
```bash
docker exec -it [container ID or NAMES] 
```
举个例子🌰，比如想进入到本地的 Redis 容器中：<br />![Docker 准备进入 redis 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294105-44dc22da-c066-47bc-becf-1fdf3c784684.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u711180be&originHeight=97&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=udadd2291-22f7-478c-895c-53a6ef5e1a8&title=Docker%20%E5%87%86%E5%A4%87%E8%BF%9B%E5%85%A5%20redis%20%E5%AE%B9%E5%99%A8 "Docker 准备进入 redis 容器")<br />可以通过容器 ID 或者名称进入容器：
```bash
docker exec -it redis /bin/bash
```
![docker 通过 exec 命令进入到容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294185-1f88376a-1c06-4c84-a159-a5bdbb46040b.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=uc5d0c68e&originHeight=111&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u9af7d72d-a817-4321-bcb1-1a931f70db6&title=docker%20%E9%80%9A%E8%BF%87%20exec%20%E5%91%BD%E4%BB%A4%E8%BF%9B%E5%85%A5%E5%88%B0%E5%AE%B9%E5%99%A8 "docker 通过 exec 命令进入到容器")<br />要想退出容器，只需键入 `exit` 命令回车即可：
```bash
(base) ➜  ~ docker exec -it redis /bin/bash
root@023d93457910:/data#
root@023d93457910:/data#
root@023d93457910:/data#
root@023d93457910:/data# exit
exit
```
**注意：**`**exec**`** 命令和下面 **`**attach**`** 命令的区别在于，当使用 exit 退出容器时，不会导致容器停止。**
<a name="OYXFs"></a>
### 二、attach 命令
执行命令格式如下：
```bash
docker attach [container ID or NAMES]
```
**注意：这种方式在执行 exit 命令退出容器时，会导致容器停止运行。**
<a name="Fr1gt"></a>
## Docker 停止&关闭容器
有以下两种方式关闭一个正在运行的 Docker 容器：
<a name="hoPA7"></a>
### 一、stop 优雅模式
```bash
docker container stop [container ID or NAMES]
# 简写模式（可省略关键字 container ）
docker stop [container ID or NAMES]
```
举个例子🌰，比如想要停止下图中的 Ubuntu 容器：<br />![docker 停止关闭 ubuntu 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294324-0b159116-0697-4eff-95f4-082360158500.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u4605045d&originHeight=104&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ubbc25618-474b-4f35-8eac-1538ced36a4&title=docker%20%E5%81%9C%E6%AD%A2%E5%85%B3%E9%97%AD%20ubuntu%20%E5%AE%B9%E5%99%A8 "docker 停止关闭 ubuntu 容器")<br />通过容器 ID 或者容器名称都可以停止容器，命令如下：
```bash
# 通过容器 ID 关闭容器
docker stop 4e6fc78cc0c4
```
<a name="uKB9k"></a>
### 二、kill 强制模式
`kill` 命令强制关闭容器：
```bash
docker container kill [container ID or NAMES]
# 简写模式（可省略关键字 container ）
docker kill [container ID or NAMES]
```
<a name="RMQnq"></a>
### 对于交互形式运行的容器如何关闭？
前面小节中说到的交互形式运行的容器，可以通过输入 exit 或者 Ctrl+d 退出容器。
<a name="ZG3SZ"></a>
### 查看已经停止运行的容器
执行如下命令，可以看到那些已经停止运行的容器：
```bash
docker container ls -a
```
![查看 docker 已经停止运行的容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294553-5b08e229-0e65-4c10-9975-1e1b57444e2e.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u580fcf11&originHeight=173&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u0ee70a86-66b5-4e2f-810b-bd8ba7f9398&title=%E6%9F%A5%E7%9C%8B%20docker%20%E5%B7%B2%E7%BB%8F%E5%81%9C%E6%AD%A2%E8%BF%90%E8%A1%8C%E7%9A%84%E5%AE%B9%E5%99%A8 "查看 docker 已经停止运行的容器")<br />想要重启已经停止运行的容器，命令如下：
```bash
docker container start [container ID or NAMES]
```
另外，`docker container restart` 命令会将一个已经运行中的容器重新启动。
<a name="PV7Th"></a>
## Docker 重启容器
执行如下命令重启 Docker 容器：
```bash
# 不管容器是否启动，直接重启
docker restart [container ID or NAMES]
```
举个例子🌰，比如想要重启下面的 Redis 容器：<br />![docker 重启 redis 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294592-3d351783-ca08-4259-979e-7780c80502b1.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u367a8bdb&originHeight=100&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub8b8c77d-6d4a-42eb-9920-709b0a6763a&title=docker%20%E9%87%8D%E5%90%AF%20redis%20%E5%AE%B9%E5%99%A8 "docker 重启 redis 容器")<br />执行如下命令即可：
```bash
# 通过容器 ID 或者名称都可以
docker restart redis
```
<a name="WSvPD"></a>
### `-t` 参数
`-t`：设置关闭容器的限制时间，若超时未能关闭，则使用 `kill` 命令强制关闭，默认值为 10s，这个时间用于容器保存自己的状态。
```bash
# 限时 5s 关闭 redis 容器，然后重启
docker restart -t=5 redis
```
![限时5秒重启redis容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294571-67de2d29-5c0d-4977-aeb5-449e0b4de8be.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u55f93199&originHeight=98&originWidth=1008&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ucebf2a6c-2893-465d-86f9-263f5e47ae6&title=%E9%99%90%E6%97%B65%E7%A7%92%E9%87%8D%E5%90%AFredis%E5%AE%B9%E5%99%A8 "限时5秒重启redis容器")
<a name="PFfta"></a>
## Docker 导出&导入容器
<a name="RHR7U"></a>
### 一、导出容器
使用 `docker export` 命令可以导出容器，比如想导出下面的 Redis 容器：<br />![查看本地的 docker 容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294714-5eeea581-0366-4dac-92d4-83f89fb732d9.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u5cd89665&originHeight=79&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u4d406e00-e45f-44de-b19e-fea4533c170&title=%E6%9F%A5%E7%9C%8B%E6%9C%AC%E5%9C%B0%E7%9A%84%20docker%20%E5%AE%B9%E5%99%A8 "查看本地的 docker 容器")
```bash
docker export 9e8d11aeef0c > redis.tar
```
导出的 tar 包快照存于当前目录下。
<a name="JSdib"></a>
### 二、导入容器快照
使用 `docker import` 命令可以将快照导入为镜像，例如：
```bash
cat redis.tar | docker import - test/redis:v1.0
```
导入成功后，就可以看到新生成的镜像了：<br />![docker 导出容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470294905-6b675bbc-7a55-47dd-8929-db9b790e5475.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u957b78b0&originHeight=109&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ubaa02e60-a3f0-453c-96c7-519255621af&title=docker%20%E5%AF%BC%E5%87%BA%E5%AE%B9%E5%99%A8 "docker 导出容器")<br />除了通过快照的方式导入容器，还可以通过指定 URL 或者某个目录来导入，例如：
```bash
docker import http://example.com/exampleimage.tgz example/imagerepo
```
<a name="j3Dfj"></a>
## Docker 删除容器
本小节中将学习如何删除 Docker 容器。
<a name="FADD9"></a>
### 一、删除容器
执行如下命令可以删除一个已经停止运行的容器：
```bash
docker container rm [container ID or NAMES]
# 简写模式（可省略关键字 container ）
docker rm [container ID or NAMES]
```
举个例子🌰，比如想要删除下面的 Redis 容器：<br />![docker 查看本地容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470295112-88dac1ae-73ad-4702-9be3-3160fefb9778.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=u87a3ad76&originHeight=102&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ub189beed-7c4a-406d-8aa5-407bd071a52&title=docker%20%E6%9F%A5%E7%9C%8B%E6%9C%AC%E5%9C%B0%E5%AE%B9%E5%99%A8 "docker 查看本地容器")<br />执行如下命令即可：
```bash
# 通过容器 ID 删除
docker rm 023d93457910
```
<a name="TfSgY"></a>
#### 强制删除容器
添加 `-f` 参数可强制删除一个正在运行的容器：<br />![docker 查看正在运行的容器](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658470295175-4f4557ab-4e58-4467-a64b-dcb32e93971e.jpeg#clientId=udeb70fc2-6322-4&from=paste&id=ud68570d6&originHeight=110&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u70f42dc5-e4ac-4ad6-b19c-9a8931bb02f&title=docker%20%E6%9F%A5%E7%9C%8B%E6%AD%A3%E5%9C%A8%E8%BF%90%E8%A1%8C%E7%9A%84%E5%AE%B9%E5%99%A8 "docker 查看正在运行的容器")<br />如图示，强制删除这个 Redis 容器：
```bash
# 通过容器 ID 强制删除容器
docker rm -f 207aaf2e3384
```
<a name="EZEQ4"></a>
### 二、删除所有已经停止运行的容器
如何一次性删除所有已经停止的容器呢？<br />用 `docker container ls -a` 命令可以查看所有已经创建的包括停止运行的容器，如果数量太多要一个个删除会很麻烦，执行如下命令即可删除掉所有已经停止运行的容器：
```bash
docker container prune
```
