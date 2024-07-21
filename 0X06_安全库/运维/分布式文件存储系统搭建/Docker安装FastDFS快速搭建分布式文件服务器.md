Docker FastDFS
<a name="GnP7z"></a>
### 搜索FastDFS的Docker镜像
```bash
docker search fastdfs
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1614322863900-0524e993-b805-4909-8cde-ff4f9a0d6325.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2103707&status=done&style=none&width=1107.6666666666667)
<a name="ZDH3m"></a>
### 拉取FastDFS镜像
```bash
docker pull qbanxiaoli/fastdfs
```
<a name="nnMzT"></a>
### 启动FastDFS容器
```bash
docker run -d --restart=always --privileged=true --net=host --name=fastdfs -e IP=192.168.127.131 -e WEB_PORT=80 -v ${HOME}/fastdfs:/var/local/fdfs qbanxiaoli/fastdfs
```
IP 后面是服务器公网ip或者虚拟机的IP，`-e WEB_PORT=80` 指定nginx端口
<a name="oFuwx"></a>
### 进入容器测试文件上传
```bash
docker exec -it fastdfs /bin/bash

echo "Hello FastDFS!">index.html

fdfs_test /etc/fdfs/client.conf upload index.html
```
<a name="SfEj2"></a>
### 一键式安装Docker中的FastDFS脚本
```bash
docker pull qbanxiaoli/fastdfs
docker run -d --restart=always --privileged=true --net=host --name=fastdfs -e IP=192.168.127.131 -e WEB_PORT=80 -v ${HOME}/fastdfs:/var/local/fdfs qbanxiaoli/fastdfs
```
