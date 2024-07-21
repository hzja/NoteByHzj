Docker Image Container Volume<br />已经部署的容器化服务，也不是不需要维护的。而且，由于生产环境往往有这样那样的严格要求，往往需要些非常规操作。Image（镜像）、Container（容器）和Volume（数据卷）的迁移，就是一类有用的非常规操作。<br />以下镜像，均以最简单的Alpine为例。
<a name="Ti6Bz"></a>
### Image
镜像的迁移，适用于离线环境。<br />一般离线环境，都会自建Docker Registry。无论官方的，还是最近流行的Harbor，都是不错的选择。但是，这个世界上就是有些环境，或者说一些环境在某些时期，没有外网，也没有内部的Registry。这个时候要部署Docker的服务，怎么办？<br />只能通过镜像的迁移。实际上，Harbor的offline installer，就是采用这种形式。
<a name="ww3Od"></a>
#### Save
```bash
# use stdout
docker save alpine > /tmp/alpine.tar
# or write to a file directly
docker save alpine -o /tmp/alpine.tar
```
推荐使用-o的形式，因为利用stdout的做法虽然直观，但在某些场景下无效，比如利用ssh远程执行命令。
<a name="vuD2A"></a>
#### Load
```bash
# use stdout
docker load < /tmp/wekan.tar
# or read from a file directly
docker load -i /tmp/wekan.tar
```
<a name="cifoB"></a>
### Container
容器的迁移，适用于已经上线，且状态复杂、从零开始启动不能正常工作的服务。容器迁移的包，包含了镜像。
<a name="aKFSV"></a>
#### Export
先准备一个正在运行的服务，并且弄脏环境。
```bash
$ docker run --rm -d --name test alpine tail -f /dev/null
9232f0c1dafe0f29918f281ca37bb41914677e818cb6f252abf3dab3be04fbb2
$ docker exec test touch proof
$ docker exec test ls -hl proof
-rw-r--r-- 1 root root 0 Nov 20 14:33 proof
#执行导出操作：docker export test -o test.tar
```
<a name="TkQaR"></a>
#### Import
首先，关闭刚才运行的服务。
```bash
$ docker kill test
test

#执行导入操作：
$ docker import test.tar test-img
sha256:e03727eeba7e16dd3acfcc7536f1244762508f9b6b9856e49cc837c1b7ffa444
```
要注意的是，import后得到的是一个镜像，相当于是执行了docker commit后的内容。当然，docker commit不是一个推荐的操作，所以容器的导入、导出，就显得不是那么的顺眼。<br />最后，检查之前创建的文件。
```bash
$ docker run --rm -d --name test test-img tail -f /dev/null
ee29cb63bb2d3ed8ac890789ba80c4fe4078b9d5343a8952b6217d64b4dcbe23

$ docker exec test ls -hl proof
-rw-r--r-- 1 root root 0 Nov 20 14:33 proof
```
可以看到，前面创建的文件是存在的，并且时间戳完全一致。
<a name="RZo59"></a>
### Volume
![](https://cdn.nlark.com/yuque/0/2020/png/396745/1598196172700-16cd331f-d9b7-4d49-b93e-db4a885d1413.png#align=left&display=inline&height=255&originHeight=255&originWidth=502&size=0&status=done&style=none&width=502)<br />数据卷的迁移，比较麻烦。Docker并未提供官方的简单方案。<br />当然，直接用root用户访问文件系统的Docker数据，比如默认的/var/lib/docker/volumes/下的文件夹，直接进行打包操作，也不是不行。但这毫无疑问是最糟糕的方案。<br />目前参考《Use volumes | Docker Documentation》，找到的最佳方案是，用另一个容器，把数据卷内容打包，并且通过挂载的形式传递到宿主机。
<a name="usED4"></a>
#### Backup
首先，准备一个Volume。
```bash
$ docker run --rm -d --name test -v test-vol:/data test-img tail -f /dev/null
f4ff81f4c31025ff476fbebc2c779a915b43ba5940b5bcc42e3ef9b1379eaeab

$ docker exec test touch /data/proof
$ docker exec test ls -hl proof
-rw-r--r-- 1 root root 0 Nov 20 14:40 proof
```
执行备份操作：
```bash
$ docker run --rm -v test-vol:/volume -v 
$PWD:/backup alpine tar cvf /backup/backup.tar volume
volume/
volume/proof
```
直接在已运行容器中打包，然后通过docker cp复制出来，也是一个方案。但这会对正在运行的容器有影响，不建议在真正重要的容器中使用。<br />这里利用了一个Alpine镜像来执行操作。实际上，任何一个自带tar的镜像都是可以的。
<a name="mJ3Qy"></a>
#### Restore
首先，清理刚才的容器和数据卷。
```bash
$ docker kill testtest
$ docker volume rm test-voltest-vol
```
执行还原操作：
```bash
docker run --rm -v test-vol:/volume -v 
$PWD:/backup alpine tar xf /backup/backup.tar
```
最后，检查还原后的结果。
```bash
$ docker run --rm -v test-vol:/data alpine ls -ahl /data
total 8
drwxr-xr-x 2 root root 4.0K Nov 20 14:48 .
drwxr-xr-x 1 root root 4.0K Nov 20 14:50 ..
-rw-r--r-- 1 root root 0 Nov 20 14:40 proof
```
<a name="Axce6"></a>
### 结论
以上其实都不是常规手段。<br />Image的传递，更应该依赖于内部Docker Registry而非tar。（当然，也有例外，比如集群部署大镜像的P2P方案，也许可以借鉴这个手段。）<br />Container的状态，应该是可弃的。一个运行了很长时间的Container，应该是可以restart、甚至kill后再重新run也不影响既有功能的。任何有依赖的状态，都应该考虑持久化、网络化，而不能单纯地保存在本地文件系统中。<br />Volume的手动迁移，的确可以采用上述方式。但是，Volume需要手动迁移、备份吗？这需要专业而完善的插件来实现。
