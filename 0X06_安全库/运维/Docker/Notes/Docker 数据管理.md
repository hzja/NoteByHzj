Docker<br />默认容器的数据是保存在容器的可读写层，当容器被删除时其上的数据也会丢失，所以为了实现数据的持久性则需要选择一种数据持久技术来保存数据。官方提供了三种存储方式：Volumes、Bind mounts和tmpfs。
<a name="ZDNFX"></a>
## 数据存储方式
Bind mount 会覆盖容器中的文件，而 volume mount 则不会。即如果容器中已有文件，则会将文件同步到主机的目录上。此方式与 Linux 系统的 mount 方式很相似，即是会覆盖容器内已存在的目录或文件，但并不会改变容器内原有的文件，当 umount 后容器内原有的文件就会还原。<br />![2021-05-29-20-58-08-841413.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622293821712-b72de6de-bcdc-4f94-b609-bb7cdf24aca2.png#clientId=uefeaac26-6191-4&from=ui&id=u1be483b3&originHeight=366&originWidth=720&originalType=binary&size=792282&status=done&style=shadow&taskId=uf8a89015-93dc-4d15-bd0f-6d83370789d)
<a name="zbxAW"></a>
#### 数据卷(Volumes)

- 由docker创建和管理，且与主机的核心功能隔离
- 无论是命名还是匿名数据卷，都存储在/var/lib/docker/volumes/下面
- 定义的数据卷可以在多个容器中同时使用，且不会自动删除
- 允许容器将内容保存到远端、云服务提供商、加密内容等等
<a name="ndzy4"></a>
#### 挂在主机目录(Bind mounts)

- 与数据卷相比，挂在主机目录具有有限的功能
- 应用的文件或者目录事先不需要存在，用时会自动创建
- 该方式允许访问容器的敏感文件，可能会产生安全隐患
<a name="DeNxk"></a>
#### 内存映射(tmpfs)

- 仅存储在容器的内存中，永远不会写入文件系统
- swarm服务使用tmpfs挂载将敏感信息挂载到容器中
<a name="mXpV3"></a>
## 数据卷 - volumes
数据卷是存储在 Docker 容器的特定目录下面
<a name="A9fuD"></a>
#### 优势说明
Docker Volumes 机制通常用来给 Docker 容器保存持久化数据，使用 Volumes 有很多优势：

- 更容易进行备份和数据迁移
- 使用 Docker CLI 命令或者 Docker API 来管理
- 可以在 Linux 和 Windows 操作系统上使用
- 可以更安全得在多个容器中共享
- Volume drivers 允许容器将内容保存到远端、云服务提供商、加密 volume 内容
- 新 Volume 的内容可以被容器预先填充

Volumes 通常也优于容器的可写层，使用 Volumes 不会增加容器的体积，并且 Volumes 的内容存储在外部独立于容器的生命周期。如果容器不产生持久化数据，可以考虑使用 tmpfs 内存映射(只保存在容器的内存中)的方式来避免数据存储在其他可能的地方，避免增加容器的体积。
<a name="wmXqr"></a>
#### 使用说明
最开始的时候 `-v` 或者 `--volume` 选项是给单独容器使用，而 `--mount` 选项是给集群服务使用。但是从 Docker 17.06 开始，也可以在单独容器上使用 `--mount`。通常来讲 `--mount` 选项也更加具体和详细。`-v` 选项将所有选项集中到一个值，而 `--mount` 选项将可选项分开。如果需要指定 volume driver 选项，那么必须使用 `--mount` 选项。
```bash
# 创建一个数据卷
$ docker volume create my-vol

# 查看所有的数据卷
$ docker volume ls

# 查看指定数据卷的信息
$ docker volume inspect my-vol
[
    {
        "Driver": "local",
        "Labels": {},
        "Mountpoint": "/var/lib/docker/volumes/my-vol/_data",
        "Name": "my-vol",
        "Options": {},
        "Scope": "local"
    }
]

# 移除指定数据卷的
$ docker volume rm my-vol

# 清除无主的数据卷
$ docker volume prune
```
```bash
# 启动一个挂载数据卷的容器
$ docker run -d -P --name web \
    -v my-vol:/wepapp \
    training/webapp python app.py

$ docker run -d -P --name web \
    --mount source=my-vol,target=/webapp \
    training/webapp python app.py

# 启动一个挂载数据卷的服务
$ docker service create -d --name devtest-service \
    --mount source=myvol2,target=/app \
    nginx:latest
```
```bash
# 挂载为只读模式
$ docker run -d --name=nginxtest \
    -v nginx-vol:/usr/share/nginx/html:ro \
    nginx:latest

# type可以分为bind、volume、tmpfs, 默认为volume
# source用于设置数据卷的名称，匿名数据卷可以省略
# target表示需要挂载到容器里面的地方
# readonly表示挂载的内容为只读模式，可选
# volume-opt表示可以使用多次，可选
$ docker run -d --name=nginxtest \
    --mount source=nginx-vol,destination=/usr/share/nginx/html,readonly \
    nginx:latest
[3] 挂载远程数据卷
```
```bash
# 插件sshfs允许您轻松地在容器中挂载远程文件夹

# 下载该插件
$ docker plugin install --grant-all-permissions vieux/sshfs

# 使用该驱动创建ssh数据卷
$ docker volume create --driver vieux/sshfs \
    -o sshcmd=test@node2:/home/test \
    -o password=testpassword \
    -o port=3336 \
    sshvolume

# 启动该驱动程序创建卷创建容器
# 如果两个容器配置了可信关系，就不需要设置volume-opt密码了
$ docker run -d \
    --name sshfs-container \
    --volume-driver vieux/sshfs \
    --mount src=sshvolume,target=/app, \
    volume-opt=sshcmd=test@node2:/home/test,volume-opt=password=testpassword \
    nginx:latest
```
![2021-05-29-20-58-09-092231.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622293467200-6ac94471-4844-4e45-960e-0f4d8840a494.png#clientId=uefeaac26-6191-4&from=ui&id=u0099c9af&originHeight=220&originWidth=740&originalType=binary&size=489487&status=done&style=shadow&taskId=u8a082743-d6ca-4180-900a-53deeed00eb)
<a name="fgKMk"></a>
## 挂载主机目录 - bind mounts
挂载主机目录是将主机中的特定目录直接挂在到容器内部使用
<a name="hkKrt"></a>
#### 使用说明
```bash
# 使用bind模式启动容器
$ docker run -d -it --name devtest \
    -v "$(pwd)"/target:/app \
    nginx:latest

$ docker run -d -it --name devtest \
    --mount type=bind,source="$(pwd)"/target,target=/app \
    nginx:latest

# 看下对应的信息
$ docker inspect devtest
"Mounts": [
    {
        "Type": "bind",
        "Source": "/tmp/source/target",
        "Destination": "/app",
        "Mode": "",
        "RW": true,
        "Propagation": "rprivate"
    }
],

# 挂载为只读模式
$ docker run -d -it --name devtest \
    -v "$(pwd)"/target:/app:ro \
    nginx:latest

$ docker run -d -it --name devtest \
    --mount type=bind,source="$(pwd)"/target,target=/app,readonly \
    nginx:latest
```
<a name="eJDRC"></a>
#### 特殊属性
| 编号 | 属性值 | 描述 |
| --- | --- | --- |
| 1 | `shared` | 原始mount的次级mount会显示在重复mount中,且重复mount的次级mount的内容也会在原始mount中显示 |
| 2 | `slave` | 与shared mount相似，只是内容单方向可见，重复 mount的内容不会在原始mount中显示 |
| 3 | `private` | 次级mount在原始mount和重复mount之间互不可见 |
| 4 | `rshared` | 与slave mount一样,只是传播范围扩展至嵌套的重复mount和原始mount |
| 5 | `rslave` | 与slave mount一样,只是传播范围扩展至嵌套的重复mount和原始mount |
| 6 | `rprivate` | 默认值，与private mount一样，即原始mount和重复mount之间都不会传播内容 |

```bash
$ docker run -d -it --name devtest \
    -v "$(pwd)"/target:/app \
    -v "$(pwd)"/target:/app2:ro,rslave \
    nginx:latest

$ docker run -d -it --name devtest \
    --mount type=bind,source="$(pwd)"/target,target=/app \
    --mount type=bind,source="$(pwd)"/target,target=/app2,readonly,bind-propagation=rslave \
    nginx:latest
```
<a name="UxHZT"></a>
## 内存映射 - `tmpfs`
内存映射是将内存映射到容器内供容器内部使用
<a name="qRIrz"></a>
#### 优势说明
最开始 `--tmpfs` 是给单独容器使用，而 `--mount` 选项是给 `swarm` 集群服务使用的。但是，从 Docker 17.06 开始，也可以在单独容器上使用 `--mount` 了。通常说来，`--mount` 更明确，更冗长。最大的区别是 `--tmpfs` 标志不支持任何可配置选项。其中 `--tmpfs` 只能在容器中使用，而 `swarm` 集群则必须使用 `--mount` 来使用 `tmpfs` 内存映射。
<a name="zywNd"></a>
#### 使用说明
```bash
# 容器上使用
$ docker run -d -it --name tmptest \
    --tmpfs /app \
    nginx:latest

$ docker run -d -it --name tmptest \
    --mount type=tmpfs,destination=/app \
    nginx:latest
```
<a name="iOHJW"></a>
## 日志驱动 - logs
在容器外部查看容器内部的日志输出情况，便于排除和监控问题<br />可以利用 `docker logs` 命令，查看 Docker 容器内部应用程序运行时所产生的日志。可以免除首先进入 Docker 容器，再打开应用程序的日志文件的过程。docker logs 会监控容器中操作系统的标准输出设备(STDOUT)，一旦 STDOUT 有数据产生，就会将这些数据传输到另一个设备中，则被称为日志驱动(Logging Driver)。
```bash
# 动态查看日志内容
$ docker logs -f netdata
```
Docker 是怎样做到的呢？使用 `docker info` 命令，可以看到 Docker 容器的相关信息，其中有一项 Logging Driver 的字段。
```bash
# 当前所设置的日志驱动类型
$ docker info | grep 'Logging Driver'
Logging Driver: json-file
```
| 编号 | 驱动类型 | 解释说明 |
| --- | --- | --- |
| 1 | `none` | 容器不输出任何日志 |
| 2 | `json-file` | 日志以JSON格式写入文件中(默认) |
| 3 | `syslog` | 日志写入宿主机的Syslog 中 |
| 4 | `journald` | 日志写入宿主机的Journald中 |
| 5 | `gelf` | 日志以GELF格式写入Graylog 中 |
| 6 | `fluentd` | 日志写入宿主机的Fluented中 |
| 7 | `splunk` | 日志写入splunk 中 |
| 8 | `etwlogs` | 日志写入ETW中 |
| 9 | `mats` | 日志写入NATS服务中 |

可以在 `docker run` 命令中通过 `--log-driver` 参数来设置具体的 Docker 日志驱动，也可以通过 `--log-opt` 参数来指定对应日志驱动的相关选项。
```bash
docker run -d -p 80:80 --name nginx \
    --log-driver json-file \ # 设置日志驱动
    --log-opt max-size=10m \ # 表示JSON文件最大为10MB，超过则生成新的文件
    --log-opt max-file=3 \   # 表示JSON文件最多保存3个，超过则删除多余文件
    nginx

# 当然，可以在配置文件中添加，全局生效
$ cat /etc/docker/daemon.json
{
  "log-driver": "syslog"
}

# 修改配置之后重启服务
$ sudo systemctl restart docker
```
额外，需要注意的是，默认情况下，Docker 将日志存储到一个日志文件。
```bash
# 检查日志文件路径
$ docker inspect --format='{{.LogPath}}' netdata
/var/lib/docker/containers/556553bcb5xxx13cbc588a4-json.log

# 查看实时日志信息
$ tail -f `docker inspect --format='{{.LogPath}}' netdata`
```
