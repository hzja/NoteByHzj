Docker gitlab-ce
<a name="12Imy"></a>
### 1、使用Docker命令拉取gitlab-ce的镜像
```bash
docker pull gitlab/gitlab-ce
```
<a name="0SXuo"></a>
### 2、创建gitlab的数据库目录
```bash
mkdir -p /docker/gitlab/config
mkdir -p /docker/gitlab/logs
mkdir -p /docker/gitlab/data
```
<a name="hJ0ZJ"></a>
### 3、运行容器
```bash
docker run -p 8083:80 -p 8082:443 -p 8084:22 -v /docker/gitlab/config:/etc/gitlab -v /docker/gitlab/logs:/var/log/gitlab -v /docker/gitlab/data:/var/opt/gitlab --name=gitlab --privileged=true gitlab/gitlab-ce:latest
```
<a name="zwjEp"></a>
### 4、配置容器内的IP和端口-解决生成的clone的连接地址不正当的问题
```bash
docker exec -it gitlab /bin/bash //进入容器内部
cd opt/gitlab/embedded/service/gitlab-rails/config //找到gitlab.yml所在的路径
vi gitlab.yml  //修改此文件，并且保存设置
gitlab-ctl restart  //容器内部重启gitlab服务
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608193048371-25311332-81dd-499c-8495-cf465061ac5a.png#averageHue=%23414141&height=583&id=Wl96f&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2261400&status=done&style=none&title=&width=1107.6666666666667)
<a name="r5duP"></a>
### 5、Gitlab-ce的使用
<a name="E1L2m"></a>
#### 使用8083端口访问Gitlab Web服务
因为启动时使用的是外部的8083端口和Gitlab-ce的80端口进行映射。
<a name="CKaYn"></a>
#### 设置管理员密码
初次访问Gitlab Web页面时，会被要求设置密码，该密码是管理员帐号密码，管理员帐号登录名称默认是root
<a name="XtL3t"></a>
#### 使用root帐号进行权限管理
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608213744514-901faa9b-a250-43c8-be13-51786e5c3d28.png#averageHue=%23faf9f9&height=170&id=oPxsq&originHeight=509&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=122838&status=done&style=shadow&title=&width=1266)<br />普通用户无权限管理图标<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608213805986-ff2394ec-28ae-4834-a325-8f9dc5b650fb.png#averageHue=%23dda277&height=163&id=JFtPo&originHeight=490&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=105052&status=done&style=shadow&title=&width=1266)
<a name="bE3b4"></a>
### 6、一键式安装部署脚本
```bash
docker pull gitlab/gitlab-ce
mkdir -p /docker/gitlab/config
mkdir -p /docker/gitlab/logs
mkdir -p /docker/gitlab/data
docker run -p 8083:80 -p 8082:443 -p 8084:22 -v /docker/gitlab/config:/etc/gitlab -v /docker/gitlab/logs:/var/log/gitlab -v /docker/gitlab/data:/var/opt/gitlab --name=gitlab --privileged=true gitlab/gitlab-ce:latest
```
