Docker<br />容器中的数据可以存储在容器层。但是将数据存放在容器层存在以下问题：

- 数据不是持久化。意思是如果容器删除了，这些数据也就没了
- 主机上的其它进程不方便访问这些数据
- 对这些数据的I/O会经过存储驱动，然后到达主机，引入了一层间接层，因此性能会有所下降

Docker 提供了3种持久化数据的方式：

1. `volumes`：存于主机文件系统中的某个区域，由Docker管理（/var/lib/docker/volumes/ on linux）。非Docker进程不应该修改这些数据。卷是Docker中持久化数据的最好方式
2. `bind mount`：存于主机文件系统中的任意位置。非Docker进程可以修改这些数据
3. `tmpfs mount`（Linux中）：存于内存中（注意，并不是持久化到磁盘）。在容器的生命周期中，它能被容器用来存放非持久化的状态或敏感信息

![2021-05-30-10-08-12-521060.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622340862324-742b6da1-8bc3-43c9-9ccf-b02407c64b72.png#clientId=u145aeff6-5799-4&from=ui&id=ud09f8d50&originHeight=255&originWidth=501&originalType=binary&size=21874&status=done&style=shadow&taskId=u9894f469-53b7-4f4d-a983-e3ae27a3fff)
<a name="TvTaA"></a>
## volumes
如果没有显式创建，一个卷会在最开始挂载时被创建。当容器停止时，卷仍然存在。多个容器可以通过read-write或read-only的方式使用同一个卷。<br />只有在显式删除时，卷才会被删除。如果将一个空卷挂载到容器中一个存有文件或目录的目录中，这些文件或目录会被拷贝到空卷中；如果将一个非空卷挂载到容器中一个存有文件或目录的目录中，这些文件或目录会被隐藏。
<a name="qII8U"></a>
### 使用

- 创建：`docker volume create`
- 删除某个卷：`docker volume rm 卷名`
- 删除所有未使用的卷：`docker volume prune`
- 列出所有卷：`docker volume ls`
- 查看某个卷的信息：`docker volume inspect 卷名`
- 挂载到容器：`-v`或`--volume`。如果是Docker17.06或更高：推荐使用`--mount`。（同 bind mount）
   - 挂载类型：`key`为`type`，`value`为`bind`、`volume`或`tmpfs`
   - 挂载源：`key`为`source`或`src`，对于命名卷，value`为`卷名，对于匿名卷，则忽略
   - 容器中的挂载点：`key`为`destination`、`dst`或`target`，`value`为容器中的路径
   - 读写类型：`value`为`readonly`，没有`key`
   - `volume-opt`选项，可以出现多次。比如`volume-driver=local`，`volume-opt=type=nfs`，…
   - 第一个域：对于命名卷，为卷名；匿名卷，则忽略，此时会创建匿名卷
   - 第二个域：容器中的挂载点
   - 第三个域：可选参数，由`','`隔开，如`ro`
   - `-v`或`--volume`：由3个域组成，`':'`分隔
   - `--mount`：由多个`','`隔开的键值对=组成：
:::tips
当使用`docker service create` 启动Docker服务时，只支持`--mount`，不支持`-v`和`--volume`。并且每个服务容器使用它们各自的本地卷，因此如果使用本地(local)卷驱动，容器无法通过卷共享数据，但是一些卷驱动支持共享存储。Docker for AWS和Doocker for Azure都使用Cloundstor plugin支持持久存储
:::
<a name="UG9V2"></a>
### 场景

- 多个运行容器间共享数据
- 当Docker主机不确保具有给定的目录或文件时。卷可以将容器运行时与Docker主机的配置解耦合
- 备份、恢复、或将数据从一个Docker主机迁移到另一个Docker主机时
<a name="NcRoA"></a>
## bind mount
主机中的文件或目录通过全路径被引用。在使用绑定挂载时，这些目录或文件不一定要已经存在。<br />如果使用这种方式将一个目录挂载到容器中一个存有文件或目录的目录中，这些文件或目录会被隐藏；如果主机中的文件或目录不存在，当使用`--mount`挂载时，Docker会报错，当使用`-v`或`--volume`时，会在主机上创建目录
<a name="qefa4"></a>
### 使用
挂载到容器：`-v`或`--volume`。如果是Docker17.06或更高：推荐使用`--mount`。（同 `volumes`）

- `-v`或`--volume`：由3个域组成，`':'`分隔
   - 第一个域：对于命名卷，为卷名；匿名卷，则忽略，此时会创建匿名卷
   - 第二个域：容器中的挂载点
   - 第三个域：可选参数，由`','`隔开，如`ro`
- `--mount`：由多个','隔开的键值对`<key>=<value>`组成：
   - 挂载类型：`key`为`type`，`value`为`bind`、`volume`或`tmpfs`
   - 挂载源：`key`为`source`或`src`，`value`为主机中文件或目录的路径
   - 容器中的挂载点：`key`为`destination`、`dst`或`target`，`value`为容器中的路径
   - 读写类型：`value`为`readonly`，没有`key`
   - `bind-propagation`选项：`key`为`bind-propagation`，`value`为`rprivate`、`private`、`rshared`、`shared`、`rslave`或`slave`
   - 一致性选项：`value`为`consistent`、`delegated`、`cached`。这个选项仅仅适用于Docker for Mac
   - `--mount`不支持z和Z（这个不同于`-v`和`--volume`）
<a name="ZYc0l"></a>
### 场景
大体上来说，只要可能，最好使用volumes

- 主机与容器共享配置文件（Docker默认情况下通过这种方式为容器提供DNS解析，通过将/etc/resolv.conf挂载到容器中）
- 共享源代码或build artifacts（比如将Maven的target/目录挂载到容器中，每次在Docker主机中build Maven工程时，容器能够访问到那些rebuilt artifacts）
- 当 docker主机中的文件或目录结构和容器需要的一致时
<a name="LDuxL"></a>
### bind propagation
:::info
对于bind mount和volumes，默认都是`rprivate`。只有在使用bind mount时可配置，且必须在linux下。bind propagation是个超前主题，对于大多数用户来说，并不需要配置
:::
对于一个挂载点/mnt，假设它同时也被挂载到/tmp。bind propagation控制 whether a mount on /tmp/a would also be available on /mnt/a<br />![2021-05-30-10-08-12-664203.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622340887312-650b37d8-266f-460e-a805-6cc75ef46266.png#clientId=u145aeff6-5799-4&from=ui&id=u1bbb0513&originHeight=318&originWidth=1001&originalType=binary&size=58161&status=done&style=shadow&taskId=u5b4f6700-9a5f-46c6-a9ce-2ceb4a92df6)<br />在设置bind propagation之前，主机文件系统需要支持bind propagation<br />下面的例子将主机中的target/挂载到容器中2次：
```bash
docker run -d \
  -it \
  --name devtest \
  --mount type=bind,source="$(pwd)"/target,target=/app \
  --mount type=bind,source="$(pwd)"/target,target=/app2,readonly,bind-propagation=rslave \
  nginx:latest
```
此时如果创建/app/foo/，/app2/foo也会存在
<a name="WOYuI"></a>
### selinux label
可以添加`z`或`Z`选项来修改挂载到容器中的主机文件或目录的selinux label：

- `z`选项指明bind mount的内容在多个容器间是共享的
- `Z`选项指明bind mount的内容是私有不共享的

要特别小心的使用这两个选项。“Bind-mounting a system directory such as /home or /usr with the Z option renders your host machine inoperable and you may need to relabel the host machine files by hand”
<a name="h6Tz0"></a>
## tmpfs mount
:::tips
只在linux中支持
:::
相对于volumes和bind mount，tmpfs mount是临时的，只在主机内存中持久化。当容器停止，tmpfs mount会被移除。对于临时存放敏感文件很有用<br />不同于volumes和bind mount，多个容器无法共享tmpfs mount
<a name="UJX71"></a>
### 使用

- 挂载到容器：`--tmpfs`。如果是Docker17.06或更高：推荐使用`--mount`
   - 挂载类型：`key`为`type`，`value`为`bind`、`volume`或`tmpfs`
   - 容器中的挂载点：`key`为`destination`、`dst`或`target`，`value`为容器中的路径
   - `tmpfs-size`和`tmpfs-mode`选项
   - `--tmpfs`：直接指定容器中的挂载点。不允许指定任何配置选项
   - `--mount`：由多个`','`隔开的键值对=组成：

![2021-05-30-10-08-12-786015.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622340898082-58cf63cf-1b30-42c3-b5bc-fca09420b0d0.png#clientId=u145aeff6-5799-4&from=ui&id=u94650fa0&originHeight=119&originWidth=999&originalType=binary&size=26613&status=done&style=shadow&taskId=u6ad1bc7c-49c2-465f-b063-86badefae1b)
<a name="qOIbO"></a>
### 场景

- 最好的使用场景既不想将数据存于主机，又不想存于容器中时。这可以是出于安全的考虑，或当应用需要写大量非持久性的状态数据时为了保护容器的性能
<a name="IPErG"></a>
## volume drivers
机器间共享数据<br />当构建错误容忍应用时，可能需要配置同一个服务的多个副本来访问相同的文件：<br />![2021-05-30-10-08-12-904880.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622340908931-e9f4b1ca-67c1-4a1f-9f83-5c1972bd1d0b.jpeg#clientId=u145aeff6-5799-4&from=ui&id=u05c1204a&originHeight=321&originWidth=1080&originalType=binary&size=24896&status=done&style=shadow&taskId=ua8c7e121-a319-453c-bddd-ef7da2c5576)<br />有多种方法来实现这个目的：

- 为应用添加逻辑，将文件存储到一个云对象存储系统（如Amazon S3）中
- 使用一个支持将文件写入外部存储系统（如NFS或Amazon S3）的driver来创建卷

volume drivers可以将底层存储系统从应用逻辑中抽象出来。比如，如果服务使用一个具有NFS driver的卷，能更新服务使用不同的driver，作为在云中存储数据的示例，而不更改应用程序逻辑
<a name="xnvsZ"></a>
### 使用
在使用`docker volume create`或驱动容器创建匿名卷时，可以指定一个`volume drivers`。下面的例子使用vieux/sshfs作为volume drivers<br />假设有2个节点，第一个节点是Docker主机，它能SSH到第二个节点
<a name="bMn97"></a>
#### 1、在Docker主机中，安装vieux/sshfs插件
```bash
docker plugin install --grant-all-permissions vieux/sshfs
```
<a name="erBnn"></a>
#### 2、使用卷驱动创建卷
<a name="LJCg2"></a>
##### 1）创建命名卷
```bash
docker volume create --driver vieux/sshfs \
  -o sshcmd=test@node2:/home/test \
  -o password=testpassword \
  sshvolume
```
<a name="pgwB0"></a>
##### 2）启动容器时使用卷驱动创建匿名卷
```bash
docker run -d \
  --name sshfs-container \
  --volume-driver vieux/sshfs \
  --mount src=sshvolume,target=/app,volume-opt=sshcmd=test@node2:/home/test,volume-opt=password=testpassword \
  nginx:latest
```
<a name="TRpgr"></a>
#### 3、备份、恢复、迁移数据卷
<a name="DpAWv"></a>
##### 1）备份一个容器
```bash
docker run --rm --volumes-from dbstore -v $(pwd):/backup ubuntu tar cvf /backup/backup.tar /dbdata
```

- 启动一个新容器，挂载dbstore容器中的卷
- 挂载一个本地主机目录到容器/backup
- 使用tar将dbdata卷中的数据打包成backup.tar
<a name="B0Hof"></a>
##### 2）用备份恢复容器
使用刚刚创建的备份来恢复容器：
```bash
docker run -v /dbdata --name dbstore2 ubuntu /bin/bash
```
然后，在新创建的容器的卷中使用tar解包备份的数据：
```bash
docker run --rm --volumes-from dbstore2 -v $(pwd):/backup ubuntu bash -c "cd /dbdata && tar xvf /backup/backup.tar --strip 1"
```
