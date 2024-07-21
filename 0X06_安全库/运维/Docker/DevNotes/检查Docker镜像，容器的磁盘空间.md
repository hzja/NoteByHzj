Docker<br />想知道 Docker 在你的 Linux 系统上到底占用了多少空间？<br />首先，所有Docker 镜像、容器和其他相关程序都位于/var/lib/docker。可以检查此目录的大小并获取 Docker 使用的总磁盘空间：
```bash
>  sudo du -sh /var/lib/docker
5G /var/lib/docker
```
怎么查看详细的占用情况呢?
<a name="HYRVN"></a>
### 检查 Docker 磁盘空间使用情况
了解镜像、容器、本地volumes占用了多少空间的基本的Docker方法：
```bash
docker system df
```
运行此命令获得按 Docker 组件分组的所有磁盘使用信息。
```bash
Emulate Docker CLI using podman. Create /etc/containers/nodocker to quiet msg.
TYPE           TOTAL       ACTIVE      SIZE        RECLAIMABLE
Images         1           0           146MB       146MB (100%)
Containers     0           0           0B          0B (0%)
Local Volumes  0           0           0B          0B (0%)
```
那怎么查看每个镜像磁盘空间的占用情况呢?可以使用下面的命令。
```bash
docker system df -v
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663043709115-6c74e083-a43d-4579-b089-fef89e99031c.png#clientId=u89ff8d1d-bc42-4&from=paste&height=655&id=uf34cd479&originHeight=1638&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1236080&status=done&style=none&taskId=u14f2b220-dfea-4045-a4b8-8836d8b51a9&title=&width=1329.2)
<a name="BZpTQ"></a>
### 检查 docker 镜像大小
查看 Docker 镜像及其大小：
```bash
docker image ls
```
它列出了系统上的所有 Docker 镜像，展示了一些详细信息，包括大小：
```bash
>  docker image ls
Emulate Docker CLI using podman. Create /etc/containers/nodocker to quiet msg.
REPOSITORY               TAG         IMAGE ID      CREATED     SIZE
docker.io/library/nginx  latest      2b7d6430f78d  7 days ago  146 MB
```
<a name="SdJKX"></a>
### 检查正在运行的容器大小
想知道正在运行的 Docker 容器的大小，可以使用 `docker ps` 命令：
```bash
docker ps --size
```
查看 SIZE 列：
```bash
>  docker ps --size
Emulate Docker CLI using podman. Create /etc/containers/nodocker to quiet msg.
CONTAINER ID  IMAGE                           COMMAND               CREATED         STATUS             PORTS                 NAMES       SIZE
8900fc2086b3  docker.io/library/nginx:latest  nginx -g daemon o...  14 seconds ago  Up 14 seconds ago  0.0.0.0:8099->80/tcp  nginx-test  1.11kB (virtual 142MB)
```
虚拟大小包括共享的底层镜像。
<a name="BLtb0"></a>
### 特定镜像磁盘使用情况
**安装一个nginx镜像**
```bash
> docker pull docker.io/library/nginx
```
**获取容器/镜像的元数据**
```bash
> docker inspect nginx
```
运行命令后，会注意到以下Data小节内的三个字段GraphDriver：
```json
...
        "GraphDriver": {
            "Data": {
                "MergedDir": "/var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e/merged",
                "UpperDir": "/var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e/diff",
                "WorkDir": "/var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e/work"
            },
...
```
可以运行以下命令来显示nginx镜像使用的磁盘空间：
```bash
>  sudo du -sh /var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e
0M /var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e
```
<a name="XwYK2"></a>
### 特定容器磁盘使用情况
运行容器占用的磁盘空间怎么查看呢?
```bash
> docker run -itd --name nginx-test -p 8099:80 docker.io/library/nginx
```
运行`docker ps`，可以看到它正在运行：
```bash
> docker ps
Emulate Docker CLI using podman. Create /etc/containers/nodocker to quiet msg.
CONTAINER ID  IMAGE                           COMMAND               CREATED        STATUS            PORTS                 NAMES
8900fc2086b3  docker.io/library/nginx:latest  nginx -g daemon o...  4 minutes ago  Up 4 minutes ago  0.0.0.0:8099->80/tcp  nginx-test
```
容器名为nginx-test
```bash
$ docker inspect nginx-test
```
查看GraphDriver字段。这些位置是容器数据物理存储在主机系统上的位置。
```json
...
        "GraphDriver": {
            "Data": {
                "LowerDir": "/var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3-init/diff:/var/lib/docker/overlay2/64c9c0cf8c9cfb0e2168071df0652a317d49f58a68fe86e4a9a9a525ab9e365e/diff",
                "MergedDir": "/var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3/merged",
                "UpperDir": "/var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3/diff",
                "WorkDir": "/var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3/work"
            },
            "Name": "overlay2"
        },
...
```
可以`du`查看大小
```bash
>  sudo du -sh /var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3
32K /var/lib/docker/overlay2/d734685e284c92bdcb6063ac292a48813f30f4b0b2dd6fa2882279c569e506a3
```
与 Docker 镜像和容器不同，volumes的物理位置非常简单。位于：
```bash
/var/lib/docker/volumes/
```
因此，可以查看整个 Docker volumes的磁盘空间使用情况：
```bash
>  sudo du -sh /var/lib/docker/volumes/
60K /var/lib/docker/volumes/
```
<a name="IsKtt"></a>
### 特定volumes磁盘使用情况
在这种情况下，主要有两种类型。一个是常规 Docker Volumes，另一个是bind mount。
<a name="Pc8DI"></a>
#### Docker Volumes
如果要查找特定volumes的位置，可以用`docker volume ls`检查volumes名或 ID。例如，使用以下命令运行 nginx 容器，其中包含一个volumes:
```bash
docker run -itd --name nginx-test -p 8099:80 docker.io/library/nginx
```
自动创建一个名为nginx-test的volumes。创建一个test.md文件
```bash
$ docker exec nginx-test sh -c "touch /tmp/test.md"

$ docker exec -ti nginx-test sh
/ # ls tmp
test.md
/ # exit
```
`docker volume ls`将列出名为test-data的volumes:
```bash
$ docker volume ls
DRIVER    VOLUME NAME
local     d502589845f7ae7775474bc01d8295d9492a6c26db2ee2c941c27f3cac4449d1
local     e71ee3960cfef0a133d323d146a1382f3e25856480a727c037b5c81b5022cb1b
local     test-data
```
test-datavolumes的物理位置
```bash
$ sudo ls -l /var/lib/docker/volumes/test-data/_data
total 0
-rw-r--r-- 1 root root 0 Oct  6 23:20 test.md
```
可以在du此处再次对特定volumes使用该命令！：
```bash
>  sudo du -sh /var/lib/docker/volumes/test-data/_data
0K /var/lib/docker/volumes/test-data/_data
```
<a name="SeEze"></a>
#### Bind Mounts
这个比较特殊，必须使用 Linux 方法来查看磁盘空间使用情况。同时，最好先停止正在运行的容器。
```bash
$ mkdir /home/rumenz/test-data
$ docker run -ti -d --name nginx-test -v /home/rumenz/test-data:/var/lib/app/content docker.io/library/nginx
```
test-data将映射到容器内/var/lib/app/content上，操作test-data目录等价于操作/var/lib/app/content。
```bash
>  sudo du -sh /home/rumenz/test-data
0K /home/rumenz/test-data
```
进入容器查看目录大小
```bash
>  sudo docker exec -ti nginx-test sh
/ # du -sh /var/lib/app/content
0K /var/lib/app/content
```
两个大小是相同的。
