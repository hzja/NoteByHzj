Docker
<a name="7J6Ox"></a>
### 容器周期管理
<a name="G2XYP"></a>
#### `run`
`docker run`：创建一个新的容器并运行一个命令
```bash
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```
OPTIONS 说明：

- `-a stdin`：指定标准输入输出内容类型，可选 `STDIN`/`STDOUT`/`STDERR `三项；
- `-d`：后台运行容器，并返回容器 ID；
- `-i`：以交互模式运行容器，通常与 -t 同时使用；
- `-P`：随机端口映射，容器内部端口随机映射到主机的端口
- `-p`：指定端口映射，格式为：`主机 (宿主) 端口:容器端口`
- `-t`：为容器重新分配一个伪输入终端，通常与 -i 同时使用；
- `--name="nginx-lb"`：为容器指定一个名称；
- `--dns 8.8.8.8`：指定容器使用的 DNS 服务器，默认和宿主一致；
- `--dns-search example.com`：指定容器 DNS 搜索域名，默认和宿主一致；
- `-h "mars"`：指定容器的 hostname；
- `-e username="ritchie"`：设置环境变量；
- `--env-file=[]`：从指定文件读入环境变量；
- `--cpuset="0-2" or --cpuset="0,1,2"`：绑定容器到指定 CPU 运行；
- `-m`：设置容器使用内存最大值；
- `--net="bridge"`：指定容器的网络连接类型，支持 bridge/host/none/container：四种类型；
- `--link=[]`：添加链接到另一个容器；
- `--expose=[]`：开放一个端口或一组端口；
- `--volume` , `-v`：绑定一个卷

使用 docker 镜像 nginx:latest 以后台模式启动一个容器，并将容器命名为 mynginx。
```bash
docker run --name mynginx -d nginx:latest
```
使用镜像 nginx:latest 以后台模式启动一个容器，并将容器的 80 端口映射到主机随机端口。
```bash
docker run -P -d nginx:latest
```
使用镜像 nginx:latest，以后台模式启动一个容器，将容器的 80 端口映射到主机的 80 端口，主机的目录 /data 映射到容器的 /data。
```bash
docker run -p 80:80 -v /data:/data -d nginx:latest
```
绑定容器的 8080 端口，并将其映射到本地主机 127.0.0.1 的 80 端口上。
```bash
docker run -p 127.0.0.1:80:8080/tcp ubuntu bash
```
使用镜像 nginx:latest 以交互模式启动一个容器，在容器内执行 /bin/bash 命令。
```bash
docker run -it nginx:latest /bin/bash
```
<a name="7FDns"></a>
#### `start`/`stop`/`restart`
```bash
docker start [OPTIONS] CONTAINER [CONTAINER...]
docker stop [OPTIONS] CONTAINER [CONTAINER...]
docker restart [OPTIONS] CONTAINER [CONTAINER...]
```
启动已被停止的容器 myrunoob
```bash
docker start myrunoob
```
停止运行中的容器 myrunoob
```bash
docker stop myrunoob
```
重启容器 myrunoob
```bash
docker restart myrunoob
```
<a name="PmTNW"></a>
#### `kill`
`docker kill` : 杀掉一个运行中的容器。
```bash
docker kill [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS 说明：

- `-s`：向容器发送一个信号

杀掉运行中的容器 mynginx
```bash
docker kill -s KILL mynginx
```
<a name="Pb2WB"></a>
#### `rm`
`docker rm`：删除一个或多个容器。
```bash
docker rm [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS 说明：

- `-f`：通过 SIGKILL 信号强制删除一个运行中的容器。
- `-l`：移除容器间的网络连接，而非容器本身。
- `-v`：删除与容器关联的卷。

强制删除容器 db01、db02：
```bash
docker rm -f db01 db02
```
移除容器 nginx01 对容器 db01 的连接，连接名 db：
```bash
docker rm -l db
```
删除容器 nginx01, 并删除容器挂载的数据卷：
```bash
docker rm -v nginx01
```
删除所有已经停止的容器：
```bash
docker rm $(docker ps -a -q)
```
<a name="BvzfV"></a>
#### `pause`/`unpause`
`docker pause`：暂停容器中所有的进程。<br />`docker unpause`：恢复容器中所有的进程。
```bash
docker pause [OPTIONS] CONTAINER [CONTAINER...]
docker unpause [OPTIONS] CONTAINER [CONTAINER...]
```
暂停数据库容器 db01 提供服务。
```bash
docker pause db01
```
恢复数据库容器 db01 提供服务。
```bash
docker unpause db01
```
删除停止的 Docker 容器
```bash
docker container prune
```
<a name="NarYJ"></a>
#### `create`
`docker create`：创建一个新的容器但不启动它 用法同 docker run
```bash
docker create [OPTIONS] IMAGE [COMMAND] [ARG...]
```
语法同 `docker run`<br />使用 docker 镜像 nginx:latest 创建一个容器，并将容器命名为 myrunoob
```bash
docker create --name myrunoob  nginx:latest     
# 输出
09b93464c2f75b7b69f83d56a9cfc23ceb50a48a9db7652ee4c27e3e2cb1961f
```
<a name="gCGvO"></a>
#### `exec`
`docker exec` ：在运行的容器中执行命令
```bash
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```
`OPTIONS` 说明：

- `-d`：分离模式：在后台运行
- `-i`：即使没有附加也保持 STDIN 打开
- `-t`：分配一个伪终端

在容器 mynginx 中以交互模式执行容器内 /root/runoob.sh 脚本：
```bash
docker exec -it mynginx /bin/sh /root/runoob.sh
```
在容器 mynginx 中开启一个交互模式的终端：
```
docker exec -i -t  mynginx /bin/bash
root@b1a0703e41e7:/#
```
也可以通过 `docker ps -a` 命令查看已经在运行的容器，然后使用容器 ID 进入容器。 查看已经在运行的容器 ID：
```bash
docker ps -a 
# 输出
...
9df70f9a0714        openjdk             "/usercode/script.sh…" 
...
```
第一列的 9df70f9a0714 就是容器 ID。 通过 exec 命令对指定的容器执行 bash：
```bash
docker exec -it 9df70f9a0714 /bin/bash
```
<a name="H6sY6"></a>
#### 启动容器
`-d` 后台运行 `-i` 交互 `-t` 终端
```bash
docker run -d -i -t ubuntu:18.04 /bin/bash
docker run -dit ubuntu:18.04 /bin/bash
```
<a name="vD5UO"></a>
#### 进入后台容器
```bash
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```
<a name="oSqJS"></a>
#### 删除 / 清除容器
```bash
docker container rm [OPTIONS] CONTAINER [CONTAINER...] # 删除容器
docker container prune # 清理掉停止的容器
```
<a name="VfEd1"></a>
#### 容器管理常用命令
```bash
docker ps -a # 查看所有的容器
docker ps # 查看运行着的容器
docker container ls -a # 查看终止状态的容器
docker container start <容器 ID>/<NAMES> # 启动一个容器
docker container stop <容器 ID>/<NAMES> # 停止一个运行着的容器
docker search python # 从 Docker Hub 查找 python 镜像 
docker pull python # 从镜像仓库中拉取或者更新指定镜像 
docker container prune # 清理掉停止的容器
docker rmi <容器 ID> # 删除本地容器 
docker tag [OPTIONS] IMAGE[:TAG] [REGISTRYHOST/][USERNAME/]NAME[:TAG]  # 标记本地镜像，将其归入某一仓库
```
<a name="IKiNc"></a>
### 容器操作
<a name="BzSUp"></a>
#### ps
`docker ps`：列出容器
```bash
docker ps [OPTIONS]
```
OPTIONS 说明：

- `-a`：显示所有的容器，包括未运行的。
- `-f`：根据条件过滤显示的内容。
- `--format`：指定返回值的模板文件。
- `-l`：显示最近创建的容器。
- `-n`：列出最近创建的 n 个容器。
- `--no-trunc`：不截断输出。
- `-q`：静默模式，只显示容器编号。
- `-s`：显示总的文件大小。

列出所有在运行的容器信息。
```bash
docker ps
# 输出
CONTAINER ID   IMAGE          COMMAND                ...  PORTS                    NAMES
09b93464c2f7   nginx:latest   "nginx -g 'daemon off" ...  80/tcp, 443/tcp          myrunoob
96f7f14e99ab   mysql:5.6      "docker-entrypoint.sh" ...  0.0.0.0:3306->3306/tcp   mymysql
```
| **名称及说明** | **属性** |
| --- | --- |
| `CONTAINERID`：容器ID。 |  |
| `IMAGE`：使用的镜像。 |  |
| `COMMAND`：启动容器时运行的命令。 |  |
| `CREATED`：容器的创建时间。 |  |
| `STATUS`：容器状态。 | `created`(已创建)<br />`restarting`(重启中)<br />`running`(运行中)<br />`removing`(迁移中)<br />`paused`(暂停)<br />`exited`(停止)<br />`dead`（死亡) |
| `PORTS`：容器的端口信息和使用的连接类型 |  |
| (tcp\\udp) |  |
| `NAMES`：自动分配的容器名称。 |  |

<a name="lqzx3"></a>
#### `inspect`
`docker inspect`：获取容器 / 镜像的元数据。
```bash
docker inspect [OPTIONS] NAME|ID [NAME|ID...]
```
OPTIONS 说明：

- `-f`：指定返回值的模板文件。
- `-s`：显示总的文件大小。
- `--type`：为指定类型返回 JSON。
<a name="8bp7F"></a>
#### `top`
`docker top`：查看容器中运行的进程信息，支持 `ps` 命令参数。<br />只有在运行中的容器才能使用这个命令
```bash
docker top [OPTIONS] CONTAINER [ps OPTIONS]
```
容器运行时不一定有 /bin/bash 终端来交互执行 `top` 命令，而且容器还不一定有 `top` 命令，可以使用 `docker top` 来实现查看 container 中正在运行的进程。
<a name="ttlg1"></a>
#### `attach`
`docker attach`：连接到正在运行中的容器。
<a name="pSNPB"></a>
#### `events`
`docker events`：从服务器获取实时事件
```bash
docker events [OPTIONS]
```
OPTIONS 说明：

- `-f`：根据条件过滤事件；
- `--since`：从指定的时间戳后显示所有事件；
- `--until`：流水时间显示到指定的时间为止；

显示 docker 镜像为 python 2016 年 7 月 1 日后的相关事件。
```bash
docker events -f "image"="python" --since="1467302400"
```
1467302400 代表时间戳的意思 在线时间戳转换工具：[https://tool.lu/timestamp/](https://tool.lu/timestamp/)
<a name="tIg61"></a>
#### `logs`
`docker logs`：获取容器的日志
```bash
docker logs [OPTIONS] CONTAINER
```
OPTIONS 说明：

- `-f`：跟踪日志输出
- `--since`：显示某个开始时间的所有日志
- `-t`：显示时间戳
- `--tail`：仅列出最新 N 条容器日志
<a name="az4hj"></a>
#### `wait`
`docker wait`：阻塞运行直到容器停止，然后打印出它的退出代码。
```bash
docker wait [OPTIONS] CONTAINER [CONTAINER...]
```
<a name="YmD8F"></a>
#### `export`
`docker export`：将文件系统作为一个 tar 归档文件导出到 STDOUT。
```bash
docker export [OPTIONS] CONTAINER
```
OPTIONS 说明：

- `-o`：将输入内容写到文件。

将 id 为 a404c6c174a2 的容器按日期保存为 tar 文件。
```bash
docker export -o mysql-`date +%Y%m%d`.tar a404c6c174a2
```
<a name="pNXq1"></a>
#### `port`
`docker port`：列出指定的容器的端口映射，或者查找将 PRIVATE_PORT NAT 到面向公众的端口。
```bash
docker port [OPTIONS] CONTAINER [PRIVATE_PORT[/PROTO]]
```
<a name="I3Dpi"></a>
#### `stats`
这是 docker 内置的监控命令，当要查看当前主机下所有容器占用内存和 cpu 的情况的时候就可以使用这个命令。
```bash
docker stats
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1604989138724-45e35ed4-7cf8-405e-9dcc-a009fae9cc20.png#averageHue=%23383838&height=583&id=MASKK&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2046572&status=done&style=none&title=&width=1107.6666666666667)
<a name="xzdl4"></a>
### 容器 rootfs 命令
<a name="jE65Z"></a>
#### `commit`
`docker commit`：从容器创建一个新的镜像。
```bash
docker commit [OPTIONS] CONTAINER [REPOSITORY[:TAG]]
```
OPTIONS 说明：

- `-a`：提交的镜像作者；
- `-c`：使用 Dockerfile 指令来创建镜像；
- `-m`：提交时的说明文字；
- `-p`：在 commit 时，将容器暂停。

将容器 d1891e47245f 保存为新的镜像，并添加提交人信息和说明信息。
```bash
docker commit -a "Leong" -m "python opencv" d1891e47245f  leong/python_opencv:v1
```
<a name="2ZG64"></a>
#### `cp`
`docker cp`：用于容器与主机之间的数据拷贝。
```bash
docker cp [OPTIONS] CONTAINER:SRC_PATH DEST_PATH|-
docker cp [OPTIONS] SRC_PATH|- CONTAINER:DEST_PATH
```
OPTIONS 说明：

- `-L`：保持源目标中的链接
```bash
docker cp /www/runoob 96f7f14e99ab:/www/  # 将主机 /www/runoob 目录拷贝到容器 96f7f14e99ab 的 /www 目录下。
docker cp /www/runoob 96f7f14e99ab:/www   # 将主机 /www/runoob 目录拷贝到容器 96f7f14e99ab 中，目录重命名为 www。
docker cp  96f7f14e99ab:/www /tmp/        # 将容器 96f7f14e99ab 的 /www 目录拷贝到主机的 /tmp 目录中。
```
<a name="Zcbjg"></a>
#### `diff`
`docker diff`：检查容器里文件结构的更改。
```bash
docker diff [OPTIONS] CONTAINER
```
查看容器 id 前缀为 d189 的容器的文件结构更改。
```bash
docker diff d189
```
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1601207863160-d78725aa-e166-45b8-bcb2-259a177e8550.png#averageHue=%23140f0a&height=383&id=y2EWW&originHeight=383&originWidth=937&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=937)<br />最前面的一个字符含义如下：

1. A - Add
2. D - Delete
3. C - Change
<a name="S5pqr"></a>
### 镜像仓库
<a name="4nhdl"></a>
#### `login`/`logout`
`docker login`：登陆到一个 Docker 镜像仓库，如果未指定镜像仓库地址，默认为官方仓库 Docker Hubdocker `logout`：登出一个 Docker 镜像仓库，如果未指定镜像仓库地址，默认为官方仓库 Docker Hub
```bash
docker login [OPTIONS] [SERVER]
docker logout [OPTIONS] [SERVER]
```
OPTIONS 说明：

- `-u`：登陆的用户名
- `-p`：登陆的密码
<a name="OPRyR"></a>
#### `pull`
`docker pull`：从镜像仓库中拉取或者更新指定镜像
```bash
docker pull [OPTIONS] NAME[:TAG|@DIGEST]
```
OPTIONS 说明：

- `-a`：拉取所有 tagged 镜像
- `--disable-content-trust`：忽略镜像的校验，默认开启

从 Docker Hub 下载 REPOSITORY 为 java 的所有镜像。
```bash
docker pull -a java
```
<a name="aW1W3"></a>
#### `push`
`docker push`：将本地的镜像上传到镜像仓库，要先登陆到镜像仓库
```bash
docker push [OPTIONS] NAME[:TAG]
```
OPTIONS 说明：

- `--disable-content-trust`：忽略镜像的校验，默认开启

上传本地镜像 myapache:v1 到镜像仓库中。
```bash
docker push myapache:v1
```
<a name="oIPBX"></a>
#### `search`
`docker search`：从 Docker Hub 查找镜像
```bash
docker search [OPTIONS] TERM
```
OPTIONS 说明：

- `--automated` 只列出 automated build 类型的镜像；
- `--no-trunc` 显示完整的镜像描述；
- `-s` 列出收藏数不小于指定值的镜像。

从 Docker Hub 查找所有镜像名包含 anaconda，并且收藏数大于 10 的镜像
```bash
docker search -s 10 anaconda
```

- NAME 镜像仓库源的名称
- DESCRIPTION 镜像的描述
- OFFICIAL 是否 docker 官方发布
- stars 类似 Github 里面的 star，表示点赞、喜欢的意思。
- AUTOMATED 自动构建。
<a name="pz4vq"></a>
### 本地镜像管理
<a name="fVlhw"></a>
#### `images`
```bash
docker images [OPTIONS] [REPOSITORY[:TAG]]
```
OPTIONS 说明：

- `-a`：列出本地所有的镜像（含中间映像层，默认情况下，过滤掉中间映像层）；
- `--digests`：显示镜像的摘要信息；
- `-f`：显示满足条件的镜像；
- `--format`：指定返回值的模板文件；
- `--no-trunc`：显示完整的镜像信息；
- `-q`：只显示镜像 ID。
<a name="pP2zr"></a>
#### `rmi`/`prune`
`docker rmi`：删除本地一个或多少镜像。
```bash
docker rmi [OPTIONS] IMAGE [IMAGE...]
```
OPTIONS 说明：

- `-f`：强制删除；
- `--no-prune`：不移除该镜像的过程镜像，默认移除；

`prune` 命令用来删除不再使用的 docker 对象。 删除所有未被 tag 标记和未被容器使用的镜像：
```bash
docker image prune
# 输出
WARNING! This will remove all dangling images.
Are you sure you want to continue? [y/N] y
```
删除所有未被容器使用的镜像：
```bash
docker image prune -a
```
删除所有停止运行的容器：
```bash
docker container prune
```
删除所有未被挂载的卷：
```bash
docker volume prune
```
删除所有网络：
```bash
docker network prune
```
删除 docker 所有资源：
```bash
docker system prune
```
<a name="8biYw"></a>
#### `tag`
`docker tag`：标记本地镜像，将其归入某一仓库。
```bash
docker tag [OPTIONS] IMAGE[:TAG] [REGISTRYHOST/][USERNAME/]NAME[:TAG]
```
将镜像 ubuntu:15.10 标记为 runoob/ubuntu:v3 镜像。
```bash
docker tag ubuntu:15.10 runoob/ubuntu:v3
docker images   runoob/ubuntu:v3
# 输出
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
runoob/ubuntu       v3                  4e3b13c8a266        3 months ago        136.3 MB
```
<a name="onZSj"></a>
#### `build`
`docker build`： 命令用于使用 Dockerfile 创建镜像。
```bash
docker build [OPTIONS] PATH | URL | -
```
OPTIONS 说明：

- `--build-arg=[]`：设置镜像创建时的变量；
- `--cpu-shares`：设置 cpu 使用权重；
- `--cpu-period`：限制 CPU CFS 周期；
- `--cpu-quota`：限制 CPU CFS 配额；
- `--cpuset-cpus`：指定使用的 CPU id；
- `--cpuset-mems`：指定使用的内存 id；
- `--disable-content-trust`：忽略校验，默认开启；
- `-f`：指定要使用的 Dockerfile 路径；
- `--force-rm`：设置镜像过程中删除中间容器；
- `--isolation`：使用容器隔离技术；
- `--label=[]`：设置镜像使用的元数据；
- `-m`：设置内存最大值；
- `--memory-swap`：设置 Swap 的最大值为内存 + swap，"-1" 表示不限 swap；
- `--no-cache`：创建镜像的过程不使用缓存；
- `--pull`：尝试去更新镜像的新版本；
- `--quiet`, `-q`：安静模式，成功后只输出镜像 ID；
- `--rm`：设置镜像成功后删除中间容器；
- `--shm-size`：设置 /dev/shm 的大小，默认值是 64M；
- `--ulimit`：Ulimit 配置。
- `--tag`, `-t`：镜像的名字及标签，通常 name:tag 或者 name 格式；可以在一次构建中为一个镜像设置多个标签。
- `--network`：默认 default。在构建期间设置 RUN 指令的网络模式

使用当前目录的 Dockerfile 创建镜像，标签为 runoob/ubuntu:v1。
```bash
docker build -t runoob/ubuntu:v1 .
```
使用 URL github.com/creack/docker-firefox 的 Dockerfile 创建镜像。
```bash
docker build github.com/creack/docker-firefox
```
也可以通过 `-f Dockerfile` 文件的位置：
```bash
docker build -f /path/to/a/Dockerfile .
```
在 Docker 守护进程执行 Dockerfile 中的指令前，首先会对 Dockerfile 进行语法检查，有语法错误时会返回：
```bash
docker build -t test/myapp .
# 输出
Sending build context to Docker daemon 2.048 kB
Error response from daemon: Unknown instruction: RUNCMD
```
<a name="S6Uu2"></a>
#### `history`
`docker history`：查看指定镜像的创建历史。
```bash
docker history [OPTIONS] IMAGE
```
OPTIONS 说明：

- `-H`：以可读的格式打印镜像大小和日期，默认为 true；
- `--no-trunc`：显示完整的提交记录；
- `-q`：仅列出提交记录 ID。

查看本地镜像 runoob/ubuntu:v3 的创建历史。
```bash
docker history runoob/ubuntu:v3
# 输出
IMAGE             CREATED           CREATED BY                                      SIZE      COMMENT
4e3b13c8a266      3 months ago      /bin/sh -c #(nop) CMD ["/bin/bash"]             0 B                 
<missing>         3 months ago      /bin/sh -c sed -i 's/^#\s*\(deb.*universe\)$/   1.863 kB            
<missing>         3 months ago      /bin/sh -c set -xe   && echo '#!/bin/sh' > /u   701 B               
<missing>         3 months ago      /bin/sh -c #(nop) ADD file:43cb048516c6b80f22   136.3 MB
```
<a name="ArkcE"></a>
#### `save`
`docker save`：将指定镜像保存成 tar 归档文件。
```bash
docker save [OPTIONS] IMAGE [IMAGE...]
```
OPTIONS 说明：

- `-o`：输出到的文件。

将镜像 runoob/ubuntu:v3 生成 my_ubuntu_v3.tar 文档
```bash
docker save -o my_ubuntu_v3.tar runoob/ubuntu:v3
ll my_ubuntu_v3.tar
# 输出
-rw------- 1 runoob runoob 142102016 Jul 11 01:37 my_ubuntu_v3.tar
```
<a name="ex9YV"></a>
#### `load`
`docker load`：导入使用 `docker save` 命令导出的镜像。
```bash
docker load [OPTIONS]
```
OPTIONS 说明：

- `--input` , `-i`：指定导入的文件，代替 `STDIN`。
- `--quiet` , `-q`：精简输出信息。

导入镜像：
```bash
docker image ls
# 输出
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
docker load < busybox.tar.gz
# 输出
Loaded image: busybox:latest
docker images
# 输出
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
busybox             latest              769b9341d937        7 weeks ago         2.489 MB
docker load --input fedora.tar
# 输出
Loaded image: fedora:rawhide
Loaded image: fedora:20
docker images
# 输出
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
busybox             latest              769b9341d937        7 weeks ago         2.489 MB
fedora              rawhide             0d20aec6529d        7 weeks ago         387 MB
fedora              20                  58394af37342        7 weeks ago         385.5 MB
fedora              heisenbug           58394af37342        7 weeks ago         385.5 MB
fedora              latest              58394af37342        7 weeks ago         385.5 MB
```
<a name="Q4lRC"></a>
#### `import`
`docker import`：从归档文件中创建镜像。
```bash
docker import [OPTIONS] file|URL|- [REPOSITORY[:TAG]]
```
OPTIONS 说明：

- `-c`：应用 docker 指令创建镜像；
- `-m`：提交时的说明文字；

从镜像归档文件 my_ubuntu_v3.tar 创建镜像，命名为 runoob/ubuntu:v4
```bash
docker import  my_ubuntu_v3.tar runoob/ubuntu:v4  
# 输出
sha256:63ce4a6d6bc3fabb95dbd6c561404a309b7bdfc4e21c1d59fe9fe4299cbfea39
docker images runoob/ubuntu:v4
# 输出
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
runoob/ubuntu       v4                  63ce4a6d6bc3        20 seconds ago      142.1 MB
```
```bash
docker images ls # 列出本地镜像
```
<a name="5buJW"></a>
### Docker 数据卷命令
创建 Docker 数据卷
```bash
$ docker volume create {数据卷名}
```
列出所有 Docker 数据卷
```bash
$ docker volume ls
```
删除指定 Docker 数据卷
```bash
$ docker volume rm {数据卷名}
```
删除未关联（失效） Docker 数据卷
```bash
$ docker volume prune
$ docker volume rm $(docker volume ls -qf dangling=true)
```
<a name="gxUSa"></a>
### Docker 文件操作命令
从主机复制文件到 Docker 容器中
```bash
$ sudo docker cp {主机内文件路径} {容器ID}:{容器内文件存储路径}
```
从 Docker 容器中复制文件到主机中
```bash
$ sudo docker cp {容器ID}:{容器内文件路径} {主机内文件存储路径}
```
<a name="gpyi0"></a>
### docker 命令的几对关系
<a name="L5tDu"></a>
#### `save` 和 `export`
save 保存的是镜像，export 保存的是容器
<a name="Eovmw"></a>
#### `load` 和 `import`
`load` 载入镜像包，`import` 载入容器包，但两者都会恢复为镜像<br />`load` 不能对载入的进行进行命名，`import` 可以 
<a name="f9559aed"></a>
### Docker命令集
| **容器生命周期管理** | **容器操作** | **容器rootfs命令** | **镜像仓库** | **本地镜像管理** | **info&#124;version** |
| --- | --- | --- | --- | --- | --- |
| `**run -d -p**` | `**ps**` | `**commit**` | `**login**` | `**images**` | `**info**` |
| `**start**`**/**`**stop**`**/**`**restart**` | `**inspect**` | `**cp**` | `**pull**` | `**rmi**` | `**version**` |
| `**kill**` | `**top**` | `**diff**` | `**push**` | `**tag**` | ** ** |
| `**rm**` | `**attach**` | ** ** | `**search**` | `**build**` | ** ** |
| `**pause**`**/**`**unpause**` | `**events**` | ** ** | ** ** | `**history**` | ** ** |
| `**create**` | `**logs**` | ** ** | ** ** | `**save**` | ** ** |
| `**exec**` | `**wait**` | ** ** | ** ** | `**import**` | ** ** |
| ** ** | `**export**` | ** ** | ** ** | ** ** | ** ** |
| ** ** | `**port**` | ** ** | ** ** | ** ** | **                ** |


---

| **命令** | **作用功能** | **参数作用** |
| --- | --- | --- |
| **$ **`**ps -ef &#124; grep docker**`<br />**$ **`**sudo status docker**` | **查看守护进程** |   |
|   |   |   |
| **$ **`**docker ps [-a][-l]**` | **列出运行的应用信息** | **列出所有应用&#124;列出最新启动的应用** |
| **$ **`**docker inspect 容器名或应用ID**` | **容器或应用的配置信息** | **参数为容器的名称或者应用的ID** |
| **$ **`**docker start [-i] 容器名**` | **重新启动停止的容器** | **交互方式启动** |
| **$ **`**docker rm 容器名**` | **删除已经停止的容器** |   |
| **$ **`**docker run [-i] [-t] [-d] [-p] 9200:9200<br />  --volumes-from [CONTAINER NAME] -v$(pwd):/backup tar cvf&#124;xvf /backup/backup.tar<br />  [container data volume] [-v]  ~/container_data:/data:ro --name [name] 容器名**` | **运行一个容器** | **交互式运行&#124;&#124;后台运行&#124;定义端口映射&#124;自定义运行的容器名**<br />`**-D**`** **`**--debug=false**`<br />`**-e**`** **`**--exec-driver="native"**`<br />`**-g**`** **`**--graph="/var/lib/docker"**`<br />`**--icc=true**`** 容器镜像互联**<br />`**-l**`** **`**--log-level="info"**`<br />`**--label=[]**`<br />`**-p**`** **`**--pidfile="/var/run/docker.pid"**`<br /> <br />`**--volumes-from [CONTAINER NAME]**`**指定挂载容器卷**<br />** **`**-v$(pwd):/backup**`** 指定数据备份&#124;还原的目录**<br />`**tar cvf&#124;xvf /backup/backup.tar**`**将数据压缩&#124;解压至目录**<br />** **`**[container data<br />  volume]**`**指定备份的数据卷**<br /> <br />`**-v**`** 为容器添加数据卷~/container_data(本地数据卷的位置):/data(容器访问数据卷的位置):ro(指定访问权限)** |
| **$ **`**docker attach 容器名或应用ID**` | **进入守护进程，进程是通过Ctrl+P、Ctrl+Q退出的** |   |
| **$ **`**docker logs [-f] [-t] [--tail]**` | **查看容器已经应用运行日志** | `**-f**`** **`**--follows=true&#124;false**`**默认**`**false**`<br />`**-t**`** **`**--timestamps=true&#124;false**`**默认**`**false**`<br />`**--tail=all**`** 可以带参数0表示最新** |
| **$ **`**docker top 容器名**` | **查看运行容器的进程** |   |
| **$ **`**docker exec [-i] [-t] [-d] 容器名**` | **在运行的容器启动新的进程** |   |
| **$ **`**docker kill 容器名**`<br />**$ **`**docker stop 容器名**` | **停止守护式容器** | **直接停止进行**<br />**发送信号停止进程** |
| **$ **`**docker run [-P] [-p] **` | **容器运行配置端口映射** | **映射所有端口&#124;映射自定义端口** |
| **$ **`**docker images <br />  [-a] [-f] [--no-trunc] [-q]**` |   | `**-a**`** <br />  **`**--all=false**`**查看所有镜像**<br />`**-f**`** **`**--filter=[]**`**过滤镜像**<br />`**--no-trunc=false**`**截断镜像的ID名称**<br />`**-q**`** **`**--quiet=false**`**只显示镜像的ID** |
| **$ **`**docker inspect [-f]**` | **查看镜像名称** | **定义显示格式** |
| **$ **`**docker rmi 镜像名**` | **删除镜像** | `**-f**`**  **`**--f=false**`<br />`**--no-prune=false**` |
| **$ **`**docker search 镜像名**` | **搜索镜像** | `**--automated=false**`**自动化构建**<br />`**--no-trunc=false**`<br />`**-s**`** **`**--stars=0**`**显示结果的最低星级评分**<br />**最多返回25个结果** |
| **$ **`**docker pull 镜像名**` | **拉取镜像** | `**-a**`** **`**--all-tags=false**`** 拉取所有符合tags的镜像**<br />`**--registry-mirror**`**选择镜像仓库** |
| **$ **`**docker push 镜像名：tag**` | **推送镜像** |   |
| **$ **`**docker run --link=[CONTAINER_NAME]:[ALIAS] [IMAGE]<br />  [COMMOND]**` |   | `**CONTAINER_NAME**`** 要关联的镜像名称**<br />`**ALIAS**`** 关联镜像的代号** |


