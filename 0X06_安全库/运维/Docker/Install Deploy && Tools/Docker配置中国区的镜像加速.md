Docker 镜像加速
<a name="TRJbg"></a>
## 1、镜像加速器的选择
<a name="yoD4R"></a>
### A.使用阿里云提供的镜像加速器
<a name="IDk9e"></a>
#### ①登录阿里官网
[阿里云官网](https://www.aliyun.com/?spm=5176.202918.fnqwg5agi.2.1bf54ef5MzjG4Z)<br />[https://www.aliyun.com/?spm=5176.202918.fnqwg5agi.2.1bf54ef5MzjG4Z](https://www.aliyun.com/?spm=5176.202918.fnqwg5agi.2.1bf54ef5MzjG4Z)
<a name="xKv7z"></a>
#### ②选择控制台
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596506845545-68ad0148-6825-40c8-ae87-a98e2d42a2a2.png#height=686&id=qooXE&originHeight=2058&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2057621&status=done&style=shadow&title=&width=1266)
<a name="ayFRz"></a>
#### ③选择产品与服务>容器镜像服务
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596506902921-2cb4789e-c0dd-461f-b1a7-ea2a5491c6f8.png#height=686&id=R1ETy&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=936384&status=done&style=shadow&title=&width=1280)
<a name="TIRYg"></a>
#### ④即可获得镜像加速的地址
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596507123532-5e90a0cb-4ef6-4730-b311-444466ecd5ac.png#height=686&id=hJ4Yu&originHeight=2058&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=681401&status=done&style=shadow&title=&width=1266)
<a name="yzCPE"></a>
### B.网易的镜像加速器
```
https://c.163.com/hub#/m/home/
```
<a name="ZrxAL"></a>
### C.镜像中国的地址
```
https://registry.docker-cn.com
```
<a name="sve6X"></a>
## 2、将镜像加速器地址进行配置
<a name="G3f2N"></a>
### A.拉取镜像时指定仓库地址（当次有效）
```bash
$ docker pull registry.docker-cn.com/library/ubuntu
```
<a name="Ka2fr"></a>
### B.将镜像仓库的地址配置到/etc/docker/daemon.json文件（永久有效）
<a name="w2ueB"></a>
#### ①编辑配置文件添加节点配置
```bash
[root@iZuligp6e1dyzfZ bin]# vim /etc/docker/daemon.json
```
daemon.json文件配置内容如下（**只能配置一个，不能在数组里配置多条，否则启动不了**）：
```json
{
  	"registry-mirrors": ["https://0wjc67q5.mirror.aliyuncs.com"]
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596507942447-ae0ff95c-037b-40e1-a748-ab91ef15b57b.png#height=110&id=GS8kP&originHeight=329&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=311253&status=done&style=none&title=&width=1107.6666666666667)
<a name="kn4bi"></a>
#### ②进行服务重载配置重启服务使其生效
```bash
[root@iZuligp6e1dyzfZ bin]# systemctl daemon-reload
[root@iZuligp6e1dyzfZ bin]# systemctl restart docker
```
<a name="IVtsM"></a>
### C.通过配置Docker的守护进程的启动参数（Docker进程关闭后就失效了）
```bash
$ dockerd --registry-mirror=https://registry.docker-cn.com
```

