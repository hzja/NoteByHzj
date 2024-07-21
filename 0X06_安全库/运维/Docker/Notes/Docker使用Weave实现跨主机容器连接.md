Docker Weave
<a name="Im4wT"></a>
# 1、步骤分析

- **安装weave**
- **启动weave**

**  $ weave launch**

- **连接不同的主机**
- **通过weave启动容器**

<a name="RWkOr"></a>
# 2、实例
<a name="DWwnK"></a>
## A.安装weave

```
	[root@iZuligp6e1dyzfZ ~]# wget -O /usr/bin/weave https://raw.githubusercontent.com/zettio/weave/master/weave
	提供权限
	[root@iZuligp6e1dyzfZ ~]# chmod a+x /usr/bin/weave
```

<a name="78Dli"></a>
## B.启动weave

```
$ weave launch
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894817352-592a34d3-7773-4ab8-9b53-08167abfbb23.png#align=left&display=inline&height=504&originHeight=1387&originWidth=2587&size=964064&status=done&style=none&width=940.7272727272727)

<a name="tN8qz"></a>
# 3、连接不同的主机

```
[root@iZuligp6e1dyzfZ ~]# weave launch 192.168.59.103
weave is already running; you can stop it with 'weave stop'.
[root@iZuligp6e1dyzfZ ~]# c2=$(weave run 192.168.1.2/24 -it ubuntu /bin/bash)
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1566894961770-bb3bb181-8c2e-4f22-b056-9841589e4522.png#align=left&display=inline&height=60&originHeight=166&originWidth=1877&size=60831&status=done&style=none&width=682.5454545454545)

<a name="Wxigg"></a>
# 4、通过weave启动容器
