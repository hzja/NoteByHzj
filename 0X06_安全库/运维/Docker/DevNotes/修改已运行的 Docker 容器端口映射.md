Docker
<a name="AhiR6"></a>
## 容器未启动？
如果容器还没有构建时，想添加端口映射时，只需要在创建容器的时候添加 -p 参数，想添加几个端口映射就追加几个 -p 参数。类似于如下示例：
```bash
docker run --name api_dfo_hyperf_ws \
-v /Users/pudongping/glory/codes/dfo/api_dfo_hyperf:/api_dfo_hyperf \
-p 9502:9502 \
-p 9503:9503 \
-p 9504:9504 \
-p 9505:9505 -it \
--entrypoint /bin/sh \
alex/alex_api_dfo:v1.0
```
<a name="UhMb0"></a>
## 容器已启动？
但是想修改或者添加端口时，先停止掉正在运行的容器。<br />以下内容都是以容器 id 为 cbe26510c276 进行操作的，请务必将容器 id 换成自己需要修改的容器 id。
```bash
docker stop {容器的名称或者 id }
```
比如：
```bash
docker stop cbe26510c276
```
查看容器完整的 hash_of_the_container 数值：
```bash
docker inspect {容器的名称或者 id } | grep Id

# 比如：
docker inspect cbe26510c276 | grep Id
# 会得到如下结果：
# "Id": "cbe26510c276fa9a4487a8c2af8cbb49410f2a5305149d2b26eb8ce37c777d00"
```
打开 hostconfig.json 配置文件
```bash
vim /var/lib/docker/containers/{hash_of_the_container}/hostconfig.json
```
比如：
```bash
vim /var/lib/docker/containers/cbe26510c276fa9a4487a8c2af8cbb49410f2a5305149d2b26eb8ce37c777d00/hostconfig.json
```
如果不想先查看完整的容器 hash_of_the_container 数值，也可以直接先切换到当前容器相关目录中 cd /var/lib/docker/containers/{hash_of_the_container}* ， 然后再去编辑 hostconfig.json 配置文件。
```bash
cd /var/lib/docker/containers/{hash_of_the_container}*
```
比如
```bash
cd /var/lib/docker/containers/cbe26510c276*
```
然后再去编辑 hostconfig.json 配置文件：
```bash
vim hostconfig.json
```
<a name="BLul8"></a>
### 修改 hostconfig.json 配置文件
在 hostconfig.json 配置文件中，找到 "PortBindings":{} 这个配置项，然后进行修改。这里添加了两个端口映射，分别将宿主机的 8502 端口以及 8505 端口映射到容器的 8502 端口和 8505 端口。<br />HostPort 对应的端口代表 宿主机 的端口。<br />建议容器使用什么端口，宿主机就映射什么端口，方便以后管理。当然，具体情况，具体分析。
```json
{
    "PortBindings": {
        "8502/tcp": [
            {
                "HostIp": "",
                "HostPort": "8502"
            }
        ],
        "8505/tcp": [
            {
                "HostIp": "",
                "HostPort": "8505"
            }
        ]
    }
}
```
如果 config.v2.json 配置文件或者 config.json 配置文件中也记录了端口，也需要进行修改，如果没有，就不需要改。<br />只需要修改 "ExposedPorts": {} 相关之处。
```json
{
    "Args": [],
    "Config": {
        "ExposedPorts": {
            "8502/tcp": {},
            "8505/tcp": {}
        },
        "Entrypoint": [
            "/bin/sh"
        ]
    }
}
```
最后重启 docker，然后查看容器相关配置信息是否已经修改完毕
```bash
# 重启 docker
service docker restart
# 或者
systemctl restart docker
```
查看容器相关配置信息：
```bash
docker inspect {容器的名称或者 id }
# 比如：
docker inspect cbe26510c276
```
配置符合要求后，再次启动容器：
```bash
docker start {容器的名称或者 id }
# 比如：
docker start cbe26510c276
```
<a name="GeOuW"></a>
## Docker Desktop for Mac
How to login the VM of Docker Desktop for Mac<br />因为在 Docker for MacOS 中，容器的宿主机并不是 MacOS 本身，而是在 MacOS 中运行的一个 VM 虚拟机 。虚拟机的路径可以通过查看 Docker Desktop 的配置界面 Disk image location 配置获得。
<a name="zG6MJ"></a>
### 那么如何进入这个虚拟机呢？
最简单的方式是采用 justincormack/nsenter1 进入，这个镜像只有 101KB，已经非常小了。
```bash
docker run -it --rm --privileged --pid=host justincormack/nsenter1
```

- `-rm` 表示在退出的时候就自动删除该容器；
- `-privileged` 表示允许该容器访问宿主机（也就是想要登录的 VM ）中的各种设备；
- `-pid=host` 表示允许容器共享宿主机的进程命名空间（namespace），或者通俗点儿解释就是允许容器看到宿主机中的各种进程；

然后再进入 /var/lib/docker/containers 目录修改 config.v2.json 配置文件和 hostconfig.json 配置文件即可。整体来说，在 MacOS 上除了进入 /var/lib/docker/containers 目录时，进入方式有所不同以外，修改配置文件方式和上文一样。需要注意的是，修改的时候请使用 vi 编辑器，因为这个镜像没有安装 vim 编辑器的。<br />比如：
```bash
vi /var/lib/docker/containers/a7377587b9f08cfe87af9a8ffa4da0f90bf07fb0a1cd6833a5ffcd9c37b842d0/config.v2.json

vi /var/lib/docker/containers/a7377587b9f08cfe87af9a8ffa4da0f90bf07fb0a1cd6833a5ffcd9c37b842d0/ho
```
