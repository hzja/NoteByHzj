分以下两种情况来深入学习这个知识点。
<a name="t3A8f"></a>
## 容器未启动？
如果容器还没有构建时，想添加端口映射时，只需要在创建容器的时候添加 `-p` 参数，想添加几个端口映射就追加几个 `-p` 参数。类似于如下示例：
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
<a name="nZsh7"></a>
### Docker容器暴露端口的形式有四种
```
-p    
#将指定的容器端口映射到宿主机所有地址的一个随机端口

-p : 
#将容器端口映射到指定的主机端口

-p ::
#将容器端口映射到主机指定ip的随机端口

-p ::
#将容器端口映射到指定主机ip的指定端口
```
<a name="Os5ZW"></a>
## 容器已启动？
<a name="nZVNn"></a>
### 方法一：删除原有容器，重新建新容器
这个解决方案最为简单，把原来的容器删掉，重新建一个。当然这次不要忘记加上端口映射。
```bash
docker run -d -uroot -p 8989:8080 --name jenkins1 -v /home/wcy/jenkins_node:/var/jenkins_home jenkins/jenkins:2.289.3-centos
```
**优缺点**：优点是简单快捷，在测试环境使用较多。缺点是如果是数据库镜像，那重新建一个又要重新配置一次，就会比较麻烦。
<a name="mkJfE"></a>
### 方法二：修改容器配置文件，重启docker服务
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
如果不想先查看完整的容器 hash_of_the_container 数值，也可以直接先切换到当前容器相关目录中 `cd /var/lib/docker/containers/{hash_of_the_container}*` ，然后再去编辑 hostconfig.json 配置文件。
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
<a name="cOACt"></a>
#### 修改 hostconfig.json 配置文件
在 hostconfig.json 配置文件中，找到 `"PortBindings":{}` 这个配置项，然后进行修改。这里添加了两个端口映射，分别将宿主机的 8502 端口以及 8505 端口映射到容器的 8502 端口和 8505 端口。<br />HostPort 对应的端口代表 **宿主机** 的端口。<br />建议容器使用什么端口，宿主机就映射什么端口，方便以后管理。当然，具体情况，具体分析。
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
如果 config.v2.json 配置文件或者 config.json 配置文件中也记录了端口，也需要进行修改，如果没有，就不需要改。<br />只需要修改 `"ExposedPorts": {}` 相关之处。
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
**优缺点**：这个方法的优点是没有副作用，操作简单。缺点是需要重启整个docker服务，如果在同一个宿主机上运行着多个容器服务的话，就会影响其他容器服务。
<a name="vKTzD"></a>
### 方法三：利用`docker commit`新构镜像
`docker commit`：把一个容器的文件改动和配置信息commit到一个新的镜像。这个在测试的时候会非常有用，把容器所有的文件改动和配置信息导入成一个新的docker镜像，然后用这个新的镜像重起一个容器，这对之前的容器不会有任何影响。<br />停止docker容器
```bash
docker stop container01
```
commit该docker容器
```bash
docker commit container01 new_image:tag
```
用前一步新生成的镜像重新起一个容器
```bash
docker run --name container02 -p 80:80 new_image:tag
```
**优缺点**：这种方式的优点是不会影响统一宿主机上的其他容器，缺点是管理起来显得比较乱，没有第二种方法那么直观。
<a name="Qijfl"></a>
## Docker Desktop for Mac
How to login the VM of Docker Desktop for Mac<br />因为在 Docker for MacOS 中，容器的宿主机并不是 MacOS 本身，而是在 MacOS 中运行的一个 VM 虚拟机。虚拟机的路径可以通过查看 Docker Desktop 的配置界面 Disk image location 配置获得。<br />**那么如何进入这个虚拟机呢？**<br />最简单的方式是采用 justincormack/nsenter1 进入，这个镜像只有 101KB，已经非常小了。
```bash
docker run -it --rm --privileged --pid=host justincormack/nsenter1
```

- –rm 表示在退出的时候就自动删除该容器；
- –privileged 表示允许该容器访问宿主机（也就是要登录的 VM ）中的各种设备；
- –pid=host 表示允许容器共享宿主机的进程命名空间（namespace），或者通俗点儿解释就是允许容器看到宿主机中的各种进程；

然后再进入 /var/lib/docker/containers 目录修改 config.v2.json 配置文件和 hostconfig.json 配置文件即可。整体来说，在 MacOS 上除了进入 /var/lib/docker/containers 目录时，进入方式有所不同以外，修改配置文件方式和上文一样。需要注意的是，修改的时候请使用 vi 编辑器，因为这个镜像没有安装 vim 编辑器的。<br />比如：
```bash
vi /var/lib/docker/containers/a7377587b9f08cfe87af9a8ffa4da0f90bf07fb0a1cd6833a5ffcd9c37b842d0/config.v2.json  
  
vi /var/lib/docker/containers/a7377587b9f08cfe87af9a8ffa4da0f90bf07fb0a1cd6833a5ffcd9c37b842d0/hostconfig.json
```
