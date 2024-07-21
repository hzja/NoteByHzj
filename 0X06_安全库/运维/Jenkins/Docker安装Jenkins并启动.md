Docker Jenkins
<a name="ace2bec1"></a>
## 1、Docker安装Jenkins
```bash
[root@iZuligp6e1dyzfZ /]# docker pull jenkins
[root@iZuligp6e1dyzfZ /]# docker images
REPOSITORY            TAG                 IMAGE ID            CREATED             SIZE
docker.io/zookeeper   latest              7341c5373a13        6 days ago          252 MB
docker.io/mysql       latest              e3fcc9e1cc04        12 days ago         544 MB
docker.io/jenkins     latest              cd14cecfdb3a        2 years ago         696 MB
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596512458026-85b2af9d-56ef-47e1-ba5e-df4654377f27.png#align=left&display=inline&height=611&originHeight=1834&originWidth=3323&size=2218246&status=done&style=none&width=1107.6666666666667)
<a name="rol7d"></a>
## 2、运行Jenkins容器
```bash
[root@iZuligp6e1dyzfZ /]# docker ps -a
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS
                                                  NAMES
6c06b127e03a        7341c5373a13        "/docker-entrypoin..."   46 minutes ago      Up 46 minutes       2181/tcp, 2888/tcp, 3888/tcp, 0.0.0.0:2180->2180/tcp, 8080/tcp   zk01
[root@iZuligp6e1dyzfZ /]# docker images
REPOSITORY            TAG                 IMAGE ID            CREATED             SIZE
docker.io/zookeeper   latest              7341c5373a13        6 days ago          252 MB
docker.io/mysql       latest              e3fcc9e1cc04        12 days ago         544 MB
docker.io/jenkins     latest              cd14cecfdb3a        2 years ago         696 MB
[root@iZuligp6e1dyzfZ /]# docker run -d --name jenkins01 -p 8081:8081 cd14cecfdb3a
ad0ed7c845f7e82f731bb21c46bfe331125f88af8e94cf21d0c9ba472a91632b
[root@iZuligp6e1dyzfZ /]# docker ps -a
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                                                            NAMES
ad0ed7c845f7        cd14cecfdb3a        "/bin/tini -- /usr..."   2 minutes ago       Up 2 minutes        8080/tcp, 50000/tcp, 0.0.0.0:8081->8081/tcp                      jenkins01
6c06b127e03a        7341c5373a13        "/docker-entrypoin..."   48 minutes ago      Up 48 minutes       2181/tcp, 2888/tcp, 3888/tcp, 0.0.0.0:2180->2180/tcp, 8080/tcp   zk01
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596514565878-060b52db-4e0c-4954-bb32-123a118a40d9.png#align=left&display=inline&height=271&originHeight=812&originWidth=3323&size=999598&status=done&style=none&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596514738060-e3a3e8d6-0fb2-45c3-9474-99b4a0eedde3.png#align=left&display=inline&height=160&originHeight=479&originWidth=3323&size=610299&status=done&style=none&width=1107.6666666666667)
