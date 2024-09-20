Docker IDEA Plugins
<a name="TBPVf"></a>
# 1、修改docker.service配置文件开放Docker服务端口
```bash
[root@iZuligp6e1dyzfZ ~]# vim /usr/lib/systemd/system/docker.service
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596467735885-a2402aa1-f668-4e59-9681-57e869afa076.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2102848&status=done&style=none&width=1107.6666666666667)
<a name="uf3A7"></a>
# 2、系统服务重载配置文件以及测试开放的端口
```bash
[root@iZuligp6e1dyzfZ ~]# systemctl daemon-reload
[root@iZuligp6e1dyzfZ ~]# systemctl restart docker
[root@iZuligp6e1dyzfZ ~]# ps -ef|grep docker
root      7591     1  0 23:01 ?        00:00:00 /usr/bin/dockerd-current -H tcp://0.0.0.0:2375 -H unix://var/run/docker.sock
root      7596  7591  0 23:01 ?        00:00:00 /usr/bin/docker-containerd-current -l unix:///var/run/docker/libcontainerd/docker-containerd.sock --metrics-interval=0 --start-timeout 2m --state-dir /var/run/docker/libcontainerd/containerd --shim docker-containerd-shim --runtime docker-runc
root      7705  4965  0 23:02 pts/4    00:00:00 grep --color=auto docker
[root@iZuligp6e1dyzfZ ~]# curl http://localhost:2375/version
{"Version":"1.13.1","ApiVersion":"1.26","MinAPIVersion":"1.12","GitCommit":"64e9980/1.13.1","GoVersion":"go1.10.3","Os":"linux","Arch":"amd64","KernelVersion":"3.10.0-514.26.2.el7.x86_64","BuildTime":"2020-07-01T14:56:42.708555659+00:00","PkgVersion":"docker-1.13.1-162.git64e9980.el7.centos.x86_64"}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596467081509-2b0ec053-ffbc-4c0f-b7fe-03e6eed58588.png#align=left&display=inline&height=272&originHeight=815&originWidth=3323&size=1075053&status=done&style=none&width=1107.6666666666667)
<a name="dJQbq"></a>
# 3、在系统环境变量配置Docker主机的连接信息
新建DOCKER_HOST变量，值为tcp://120.79.178.68:2375主机和Docker配置映射的端口<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596467238101-955bc91f-2e4c-4747-9864-9e23ff5d6fe8.png#align=left&display=inline&height=139&originHeight=416&originWidth=1674&size=50371&status=done&style=none&width=558)
<a name="IMUMi"></a>
# 4、在IDEA的Docker进行配置连接
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596467602429-739677a7-6b8e-4898-961a-e771bb0da138.png#align=left&display=inline&height=585&originHeight=1756&originWidth=2368&size=352921&status=done&style=none&width=789.3333333333334)
<a name="EPbzF"></a>
# 5、进入插件页面进行镜像和容器的管理操作
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596467661650-9e493fcd-71bd-4481-9204-a5c8a2c8b68e.png#align=left&display=inline&height=324&originHeight=972&originWidth=3840&size=239464&status=done&style=none&width=1280)

