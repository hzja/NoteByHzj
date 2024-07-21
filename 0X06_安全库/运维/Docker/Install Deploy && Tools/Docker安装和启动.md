Linux CentOS Docker
<a name="iITTa"></a>
## 1、检查系统内核版本
```bash
[root@runoob ~]# uname -r
```
<a name="BJkkE"></a>
## 2、下载Docker
<a name="nk8Ub"></a>
### 使用安装脚本下载Docker可以下载到最新版本的Docker
```bash
wget -qO- https://get.docker.com/ | sh
# 或
curl -fsSL https://get.docker.com/ | sh
```
<a name="mhmkK"></a>
### CentOS使用yum下载安装Docker不推荐：可能会安装到低版本的Docker
```bash
# yum install docker
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567156158304-da402fa5-0603-4f75-bffa-d36e847ba615.png#averageHue=%230c0c0b&height=562&id=F8ADI&originHeight=1546&originWidth=3327&originalType=binary&ratio=1&rotation=0&showTitle=false&size=355878&status=done&style=none&title=&width=1209.8181818181818)
<a name="ukNH0"></a>
### Ubuntu使用apt-get安装Docker不推荐：可能会安装到低版本的Docker
```bash
sudo apt-get update
sudo apt-get install -y docker.io
```
<a name="DcFuC"></a>
## 3、启动Docker服务
<a name="PEjAT"></a>
### A、Ubuntu添加Docker服务并启动Docker
```bash
sudo systemctl start docker
```
<a name="CxtOt"></a>
### B、CentOS添加Docker服务并启动Docker
```bash
sudo service docker start
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1567156377223-c868dd97-fd4c-4635-a7fe-ef33654a86c0.png#averageHue=%230a0a09&height=196&id=bBAT4&originHeight=538&originWidth=2966&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103950&status=done&style=none&title=&width=1078.5454545454545)
<a name="DvaNj"></a>
## 4、添加docker-proxy到用户bin目录下防止docker run时报错
[关于Docker run报exec: "docker-proxy": executable file not found in $PATH.错误的解决](https://www.yuque.com/fcant/operation/kcsig2?view=doc_embed)
<a name="xNMAU"></a>
## 5、添加docker-runc软链接防止容器运行报错
[Docker运行容器报错：Error response from daemon: shim error: docker-runc not installed on system.](https://www.yuque.com/fcant/operation/aq0v5d?view=doc_embed)
<a name="rGLlJ"></a>
## 6、修改使用国内的Docker镜像仓库
**国内官方源下载速度会比较慢，所以跟 npm 一样切换个国内源地址是不错的选择，例如 中国科学技术大学（中科大）、阿里云、网易、Docker 官方中国区等**

| 镜像仓库名称 | 链接 |
| --- | --- |
| Docker 官方中国区 | https://registry.docker-cn.com |
| 网易 163 | http://hub-mirror.c.163.com |
| 中国科学技术大学 | https://docker.mirrors.ustc.edu.cn |
| 阿里云 | https://<你的ID>.mirror.aliyuncs.com |

<a name="tssnd"></a>
### Mac 修改方式：

1. 修改 Docker 镜像仓库配置：`sudo vim /etc/docker/daemon.json`
2. 修改配置文件：

daemon.json
```json
{
	"registry-mirrors": ["https://docker.mirrors.ustc.edu.cn"],
}
```

1. 使配置文件生效：`sudo systemctl daemon-reload`
2. 重启 Docker：`sudo service docker restart`
<a name="sMK0x"></a>
### Windows 修改方式：
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646267725761-aa4bb6d4-c67b-4c38-a816-a1b80a57e61f.png#averageHue=%23e1eaf3&clientId=u62066b9c-3f76-4&errorMessage=unknown%20error&from=paste&id=u45e52e46&originHeight=622&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=ubb19063d-7d4f-4f06-9b2e-87a4208e1d8&title=)<br />修改完毕后，通过指令 `docker info` 查看：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1646267725669-bd21144c-858b-4b9a-abd2-9c1cea6ce3b8.png#averageHue=%23292423&clientId=u62066b9c-3f76-4&errorMessage=unknown%20error&from=paste&id=uf0f95d4b&originHeight=231&originWidth=809&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=ua0c4bafc-14c9-4ce5-93ab-cb7cbad7c84&title=)<br />当然，需要注意的是：切换源重启后，会导致当前的容器暂停掉，重启回来就行了。
