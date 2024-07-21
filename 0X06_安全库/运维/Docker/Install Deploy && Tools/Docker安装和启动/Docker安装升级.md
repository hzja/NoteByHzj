Docker
<a name="Hx4Zg"></a>
### 1、查找主机上关于Docker的软件包
```bash
# 列出包含docker字段的软件的信息
rpm -qa | grep docker
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639119276611-5816c144-e783-4f09-ad23-95c9607ece6e.png#clientId=u1f01f98d-ef8c-4&from=paste&height=59&id=u0eea32b8&originHeight=176&originWidth=1513&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41522&status=done&style=none&taskId=ufa7a054d-9da4-4c24-8442-78e09e91364&title=&width=504.3333333333333)
<a name="YaTN9"></a>
### 2、使用`yum remove`卸载软件
```bash
yum remove docker-1.13.1-53.git774336d.el7.centos.x86_64
yum remove docker-client-1.13.1-53.git774336d.el7.centos.x86_64
yum remove docker-common-1.13.1-53.git774336d.el7.centos.x86_64
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639119301878-9f13973c-655c-402a-bfce-6341f94a7ef1.png#clientId=u1f01f98d-ef8c-4&from=paste&height=102&id=u72e59e46&originHeight=307&originWidth=2310&originalType=binary&ratio=1&rotation=0&showTitle=false&size=87958&status=done&style=none&taskId=u5ac983fb-2234-4ba6-8991-5290a335e5d&title=&width=770)<br />再使用docker命令会提示docker不存在<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639119319809-2abb0c24-127a-4ed1-bcc7-51442eca35c6.png#clientId=u1f01f98d-ef8c-4&from=paste&height=149&id=u76832c3f&originHeight=447&originWidth=2336&originalType=binary&ratio=1&rotation=0&showTitle=false&size=119211&status=done&style=none&taskId=uba22ed90-359b-4f63-8c16-9f46ff5a7f0&title=&width=778.6666666666666)
<a name="Bg9OD"></a>
### 3、使用curl升级到最新版
```bash
curl -fsSL https://get.docker.com/ | sh
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639121823332-05d1a129-e370-4e55-868b-421f0e241dab.png#clientId=u1f01f98d-ef8c-4&from=paste&height=543&id=u6c4099d7&originHeight=1630&originWidth=2521&originalType=binary&ratio=1&rotation=0&showTitle=false&size=344149&status=done&style=none&taskId=u1cb70521-0215-4cfe-8b41-ff9a8b2f921&title=&width=840.3333333333334)
<a name="CdLqn"></a>
### 4、重启Docker并查看版本信息
```bash
systemctl restart docker
docker version
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639121856412-58cf7cb9-abc1-4713-8a23-07b92acdb2ff.png#clientId=u1f01f98d-ef8c-4&from=paste&height=546&id=uccb56b46&originHeight=1637&originWidth=2518&originalType=binary&ratio=1&rotation=0&showTitle=false&size=305023&status=done&style=none&taskId=u491a6f4f-5cc6-44c2-bc83-98e3ba8de05&title=&width=839.3333333333334)
<a name="du1qo"></a>
### 5、设置Docker开机自启
```bash
systemctl enable docker
```
<a name="q7AD6"></a>
### 6、查看Docker系统信息，包括镜像和容器数…
```bash
docker info
```
<a name="wkQWk"></a>
### 8、使用`docker images`命令查看自己之前的镜像是否存在
[<br />](https://blog.csdn.net/qq_39629343/article/details/80168084)
