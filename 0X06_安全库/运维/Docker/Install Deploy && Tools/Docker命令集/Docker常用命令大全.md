Docker
<a name="Sy3Pu"></a>
## 容器生命周期管理命令
<a name="t39mA"></a>
### `run`
创建一个新的容器。
```bash
# 使用docker镜像nginx:latest以后台模式启动一个容器,并将容器命名为mynginx。  
docker run --name mynginx -d nginx:latest  
  
# 使用镜像 nginx:latest，以后台模式启动一个容器,将容器的 80 端口映射到主机的 80 端口,主机的目录 /data 映射到容器的 /data。  
docker run -p 80:80 -v /data:/data -d nginx:latest  
  
# 使用镜像nginx:latest以交互模式启动一个容器,在容器内执行/bin/bash命令。  
docker run -it nginx:latest /bin/bash
```
<a name="E33dX"></a>
### `start`/`stop`/`restart`

- `docker start`：启动一个或多个已经被停止的容器。
- `docker stop`：停止一个运行中的容器。
- `docker restart`：重启容器。
```bash
# 启动已被停止的容器mynginx  
docker start mynginx  
  
# 停止运行中的容器mynginx  
docker stop mynginx  
  
# 重启容器mynginx  
docker restart mynginx
```
<a name="llxF3"></a>
### `kill`
杀掉一个运行中的容器。可选参数：

- -s：发送什么信号到容器，默认 KILL
```bash
# 根据容器名字杀掉容器  
docker kill tomcat7  
  
# 根据容器ID杀掉容器  
docker kill 65d4a94f7a39
```
<a name="IpCyo"></a>
### `rm`
删除一个或多个容器。
```bash
# 强制删除容器 db01、db02：  
docker rm -f db01 db02  
  
# 删除容器 nginx01, 并删除容器挂载的数据卷：  
docker rm -v nginx01  
  
# 删除所有已经停止的容器：  
docker rm $(docker ps -a -q)
```
<a name="X7Erz"></a>
### `create`
创建一个新的容器但不启动它。
```bash
# 使用docker镜像nginx:latest创建一个容器,并将容器命名为mynginx  
docker create --name mynginx nginx:latest
```
<a name="S3vNe"></a>
### `exec`
在运行的容器中执行命令。可选参数：

- -d：分离模式：在后台运行
- -i：即使没有附加也保持STDIN 打开
- -t：分配一个伪终端
```bash
# 在容器 mynginx 中以交互模式执行容器内 /root/nginx.sh 脚本  
docker exec -it mynginx /bin/sh /root/nginx.sh  
  
# 在容器 mynginx 中开启一个交互模式的终端  
docker exec -i -t  mynginx /bin/bash  
  
# 也可以通过 docker ps -a 命令查看已经在运行的容器，然后使用容器 ID 进入容器。  
docker ps -a   
docker exec -it 9df70f9a0714 /bin/bash
```
<a name="Jv3qV"></a>
### `pause`/`unpause`

- `docker pause`：暂停容器中所有的进程。
- `docker unpause`：恢复容器中所有的进程。
```bash
# 暂停数据库容器db01提供服务。  
docker pause db01  
  
# 恢复数据库容器 db01 提供服务  
docker unpause db0
```
<a name="Brqa0"></a>
## 容器操作命令
<a name="Q46qf"></a>
### `ps`
列出容器。可选参数：

- -a：显示所有的容器，包括未运行的。
- -f：根据条件过滤显示的内容。
- –format：指定返回值的模板文件。
- -l：显示最近创建的容器。
- -n：列出最近创建的n个容器。
- –no-trunc：不截断输出。
- -q：静默模式，只显示容器编号。
- -s：显示总的文件大小。
```bash
# 列出所有在运行的容器信息。  
docker ps  
  
# 列出最近创建的5个容器信息。  
docker ps -n 5  
  
# 列出所有创建的容器ID。  
docker ps -a -q
```
补充说明：<br />容器的7种状态：`created`（已创建）、`restarting`（重启中）、`running`（运行中）、`removing`（迁移中）、`paused`（暂停）、`exited`（停止）、`dead`（死亡）。
<a name="KQUoA"></a>
### `inspect`
获取容器/镜像的元数据。可选参数：

- -f：指定返回值的模板文件。
- -s：显示总的文件大小。
- –type：为指定类型返回JSON。
```bash
# 获取镜像mysql:5.7的元信息。  
docker inspect mysql:5.7  
  
# 获取正在运行的容器mymysql的 IP。  
docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mymysql
```
<a name="sKeiK"></a>
### `top`
查看容器中运行的进程信息，支持 ps 命令参数。
```bash
# 查看容器mymysql的进程信息。  
docker top mymysql  
  
# 查看所有运行容器的进程信息。  
for i in  `docker ps |grep Up|awk '{print $1}'`;do echo \ &&docker top $i; done
```
<a name="jObX0"></a>
### `events`
获取实时事件。参数说明：

- -f：根据条件过滤事件；
- –since：从指定的时间戳后显示所有事件；
- –until：流水时间显示到指定的时间为止；
```bash
# 显示docker 2016年7月1日后的所有事件。  
docker events  --since="1467302400"  
  
# 显示docker 镜像为mysql:5.6 2016年7月1日后的相关事件。  
docker events -f "image"="mysql:5.6" --since="1467302400"
```
说明：如果指定的时间是到秒级的，需要将时间转成时间戳。如果时间为日期的话，可以直接使用，如–since=“2016-07-01”。
<a name="lRQ10"></a>
### `logs`
获取容器的日志。参数说明：

- -f：跟踪日志输出
- –since：显示某个开始时间的所有日志
- -t：显示时间戳
- –tail：仅列出最新N条容器日志
```bash
# 跟踪查看容器mynginx的日志输出。  
docker logs -f mynginx  
  
# 查看容器mynginx从2016年7月1日后的最新10条日志。  
docker logs --since="2016-07-01" --tail=10 mynginx
```
<a name="gqewP"></a>
### `export`
将文件系统作为一个tar归档文件导出到STDOUT。参数说明：

- -o：将输入内容写到文件。
```bash
# 将id为a404c6c174a2的容器按日期保存为tar文件。  
docker export -o mysql-`date +%Y%m%d`.tar a404c6c174a2  
  
ls mysql-`date +%Y%m%d`.tar
```
<a name="peKVt"></a>
### `port`
列出指定的容器的端口映射。
```bash
# 查看容器mynginx的端口映射情况。  
docker port mymysql
```
<a name="PHpG7"></a>
## 容器`rootfs`命令
<a name="LRTZ4"></a>
### `commit`
从容器创建一个新的镜像。参数说明：

- -a：提交的镜像作者；
- -c：使用Dockerfile指令来创建镜像；
- -m：提交时的说明文字；
- -p：在`commit`时，将容器暂停。
```bash
# 将容器a404c6c174a2 保存为新的镜像,并添加提交人信息和说明信息。  
docker commit -a "guodong" -m "my db" a404c6c174a2  mymysql:v1
```
<a name="Yxmr1"></a>
### `cp`
用于容器与主机之间的数据拷贝。参数说明：

- -L：保持源目标中的链接
```bash
# 将主机/www/runoob目录拷贝到容器96f7f14e99ab的/www目录下。  
docker cp /www/runoob 96f7f14e99ab:/www/  
  
# 将主机/www/runoob目录拷贝到容器96f7f14e99ab中，目录重命名为www。  
docker cp /www/runoob 96f7f14e99ab:/www  
  
# 将容器96f7f14e99ab的/www目录拷贝到主机的/tmp目录中。  
docker cp  96f7f14e99ab:/www /tmp/
```
<a name="mzEPT"></a>
### `diff`
检查容器里文件结构的更改。
```bash
# 查看容器mymysql的文件结构更改。  
docker diff mymysql
```
<a name="siHzF"></a>
## 镜像仓库命令
<a name="QIf4A"></a>
### `login`/`logout`
`docker login`：登陆到一个Docker镜像仓库，如果未指定镜像仓库地址，默认为官方仓库 Docker Hub<br />`docker logout`：登出一个Docker镜像仓库，如果未指定镜像仓库地址，默认为官方仓库 Docker Hub参数说明：

- -u：登陆的用户名
- -p：登陆的密码
```bash
# 登陆到Docker Hub  
docker login -u 用户名 -p 密码  
  
# 登出Docker Hub  
docker logout
```
<a name="psacF"></a>
### `pull`
从镜像仓库中拉取或者更新指定镜像。参数说明：

- -a：拉取所有 tagged 镜像
- –disable-content-trust：忽略镜像的校验，默认开启
```bash
# 从Docker Hub下载java最新版镜像。  
docker pull java  
  
# 从Docker Hub下载REPOSITORY为java的所有镜像。  
docker pull -a java  
```
<a name="nM0X2"></a>
### `push`
将本地的镜像上传到镜像仓库，要先登陆到镜像仓库。参数说明：

- –disable-content-trust：忽略镜像的校验，默认开启
```bash
# 上传本地镜像myapache:v1到镜像仓库中。  
docker push myapache:v1
```
<a name="bat0h"></a>
### `search`
从Docker Hub查找镜像。参数说明：

- –automated：只列出 automated build类型的镜像；
- –no-trunc：显示完整的镜像描述；
- -f <过滤条件>：列出指定条件的镜像。
```bash
# 从 Docker Hub 查找所有镜像名包含 java，并且收藏数大于 10 的镜像  
docker search -f stars=10 java  
  
NAME                  DESCRIPTION                           STARS   OFFICIAL   AUTOMATED  
java                  Java is a concurrent, class-based...   1037    [OK]         
anapsix/alpine-java   Oracle Java 8 (and 7) with GLIBC ...   115                [OK]  
develar/java                                                 46                 [OK]  
```
每列参数说明：

- NAME：镜像仓库源的名称
- DESCRIPTION：镜像的描述
- OFFICIAL：是否 docker 官方发布
- stars：类似 Github 里面的 star，表示点赞、喜欢的意思
- AUTOMATED：自动构建
<a name="o1ivs"></a>
## 本地镜像管理命令
<a name="UZRwZ"></a>
### `images`
列出本地镜像。参数说明：

- -a：列出本地所有的镜像（含中间映像层，默认情况下，过滤掉中间映像层）；
- –digests：显示镜像的摘要信息；
- -f：显示满足条件的镜像；
- –format：指定返回值的模板文件；
- –no-trunc：显示完整的镜像信息；
- -q：只显示镜像ID。
```bash
# 查看本地镜像列表。  
docker images  
  
# 列出本地镜像中REPOSITORY为ubuntu的镜像列表。  
docker images  ubuntu
```
<a name="aXghG"></a>
### `rmi`
删除本地一个或多个镜像。参数说明：

- -f：强制删除；
- –no-prune：不移除该镜像的过程镜像，默认移除；
```bash
# 强制删除本地镜像 guodong/ubuntu:v4。  
docker rmi -f guodong/ubuntu:v4
```
<a name="t4bXU"></a>
### `tag`
标记本地镜像，将其归入某一仓库。
```bash
# 将镜像ubuntu:15.10标记为 runoob/ubuntu:v3 镜像。  
docker tag ubuntu:15.10 runoob/ubuntu:v3
```
<a name="Z8YHC"></a>
### `build`
用于使用 Dockerfile 创建镜像。参数说明：

- –build-arg=[]：设置镜像创建时的变量；
- –cpu-shares：设置cpu使用权重；
- –cpu-period：限制CPUCFS周期；
- –cpu-quota：限制CPUCFS配额；
- –cpuset-cpus：指定使用的CPUid；
- –cpuset-mems：指定使用的内存id；
- –disable-content-trust：忽略校验，默认开启；
- -f：指定要使用的Dockerfile路径；
- –force-rm：设置镜像过程中删除中间容器；
- –isolation：使用容器隔离技术；
- –label=[]：设置镜像使用的元数据；
- -m：设置内存最大值；
- –memory-swap：设置Swap的最大值为内存+swap，"-1"表示不限swap；
- –no-cache：创建镜像的过程不使用缓存；
- –pull：尝试去更新镜像的新版本；
- –quiet，-q：安静模式，成功后只输出镜像ID；
- –rm：设置镜像成功后删除中间容器；
- –shm-size：设置/dev/shm的大小，默认值是64M；
- –ulimit：Ulimit配置。
- –squash：将Dockerfile中所有的操作压缩为一层。
- –tag，-t：镜像的名字及标签，通常 name:tag 或者 name 格式；可以在一次构建中为一个镜像设置多个标签。
- –network：默认 default。在构建期间设置RUN指令的网络模式
```bash
# 使用当前目录的 Dockerfile 创建镜像，标签为 runoob/ubuntu:v1  
docker build -t runoob/ubuntu:v1 .   
  
# 使用URL github.com/creack/docker-firefox 的 Dockerfile 创建镜像  
docker build github.com/creack/docker-firefox  
  
# 通过 -f Dockerfile文件的位置 创建镜像  
docker build -f /path/to/a/Dockerfile .
```
<a name="qV0M7"></a>
### `history`
查看指定镜像的创建历史。参数说明：

- -H：以可读的格式打印镜像大小和日期，默认为true；
- –no-trunc：显示完整的提交记录；
- -q：仅列出提交记录ID。
```bash
# 查看本地镜像 guodong/ubuntu:v3 的创建历史。  
docker history guodong/ubuntu:v3
```
<a name="sUB7m"></a>
### `save`
将指定镜像保存成 tar 归档文件。参数说明：

- -o：输出到的文件。
```bash
# 将镜像 runoob/ubuntu:v3 生成 my_ubuntu_v3.tar 文档  
docker save -o my_ubuntu_v3.tar runoob/ubuntu:v3
```
<a name="vD2N3"></a>
### `load`
导入使用 docker save 命令导出的镜像。参数说明：

- –input , -i：指定导入的文件，代替 STDIN。
- –quiet , -q：精简输出信息。
```bash
# 导入镜像  
docker load --input fedora.tar
```
<a name="Ekh8S"></a>
### `import`
从归档文件中创建镜像。参数说明：

- -c：应用docker 指令创建镜像；
- -m：提交时的说明文字；
```bash
# 从镜像归档文件my_ubuntu_v3.tar创建镜像，命名为runoob/ubuntu:v4  
docker import  my_ubuntu_v3.tar runoob/ubuntu:v4
```
<a name="IFQlV"></a>
## 基础版本信息命令
<a name="UQW2f"></a>
### `info`
显示 Docker 系统信息，包括镜像和容器数。
```bash
# 查看docker系统信息。  
docker info
```
<a name="BWmpW"></a>
### `version`
显示 Docker 版本信息。
```bash
docker version
```
