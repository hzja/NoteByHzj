<a name="ubJ7C"></a>
## 1、报错详情
```bash
[root@iZuligp6e1dyzfZ /]# docker run --name zk01 -p 2181:2181 --restart always -d 7341c5373a13
59d47e925c0d6f0815cbbab62d349b364248caebed53eb522413d7f59d27665a
/usr/bin/docker-current: Error response from daemon: driver failed programming external connectivity on endpoint zk01 (929602510b5a61eb8d0162c275f80d7d0797bf2ed68b2642c11bcc189db9ea89): exec: "docker-proxy": executable file not found in $PATH.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596511890712-b828e1ee-187d-4db1-82bc-161d11b219d3.png#height=125&id=caXKd&originHeight=376&originWidth=3323&originalType=binary&size=424335&status=done&style=none&width=1107.6666666666667)
<a name="hxUD7"></a>
## 2、解决步骤
容器的启动需要执行 /usr/bin/docker-proxy 
<a name="iT0ua"></a>
### ①查看docker-proxy的位置
```bash
[root@iZuligp6e1dyzfZ /]# cat /usr/lib/systemd/system/docker.service | grep prox
          --userland-proxy-path=/usr/libexec/docker/docker-proxy-current \
```
<a name="wbAa3"></a>
### ②创建一条docker-proxy的软连接到 /usr/bin/ 下
```bash
[root@iZuligp6e1dyzfZ /]# ln -s /usr/libexec/docker/docker-proxy-current /usr/bin/docker-proxy
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596510507839-355d4077-5459-47e9-aaff-24b28222d2ac.png#height=92&id=MX251&originHeight=275&originWidth=3323&originalType=binary&size=272063&status=done&style=none&width=1107.6666666666667)
