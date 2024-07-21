Docker Docker-ce
<a name="YJ8if"></a>
### 1、拉取Portainer镜像
```bash
docker pull portainer/portainer
```
<a name="C5Oxm"></a>
### 2、创建Portainer数据卷
```bash
docker volume create portainer_data
```
<a name="xy7mq"></a>
### 3、运行Portainer
```bash
docker run -d -p 9000:9000 \
            --name portainer \
            --restart always \
            -v /var/run/docker.sock:/var/run/docker.sock \
            -v portainer_data:/data \
            portainer/portainer
```
参数说明：`-v /var/run/docker.sock:/var/run/docker.sock` ：把宿主机的Docker守护进程(Docker daemon)默认监听的Unix域套接字挂载到容器中；`-v portainer_data:/data` ：把宿主机portainer_data数据卷挂载到容器/data目录；
<a name="uLJg1"></a>
### 4、浏览器访问9000端口
1、登陆 http://x.x.x.x:9000，设置管理员账号和密码。<br />2、单机版在新页面选择 Local 即可完成安装，集群选择Remote然后输入SWARM的IP地址，点击Connect完成安装。<br />3、浏览器访问 http://192.168.2.119:9000 , 设置一个密码即可，点击创建用户<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982686989-83be0323-55e4-4491-8812-485fffcd7701.png#clientId=u9658ca7b-6a90-4&from=paste&height=462&id=u208e6a74&originHeight=1387&originWidth=2132&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84468&status=done&style=none&taskId=u8f60fae1-35c4-4b84-899a-7c59df1ddc2&title=&width=710.6666666666666)<br />这里搭建的是单机版，直接选择Local ，点击连接<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982742273-1596b8f7-e154-4433-a2d1-9e4bb32e0c7a.png#clientId=u9658ca7b-6a90-4&from=paste&height=495&id=ue63f8a7d&originHeight=1485&originWidth=2957&originalType=binary&ratio=1&rotation=0&showTitle=false&size=185108&status=done&style=none&taskId=ubbc9a78a-9d35-4181-9775-31a61054229&title=&width=985.6666666666666)<br />进入仪表盘主页面<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982768039-45a58833-4137-4bf2-b2b0-6003453212f7.png#clientId=u9658ca7b-6a90-4&from=paste&height=594&id=u8f054073&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=514198&status=done&style=none&taskId=u0aa6846e-1216-40a8-862c-ca9cfdc1a40&title=&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982811329-2876a9c2-8c95-4f54-92fc-2876d1948957.png#clientId=u9658ca7b-6a90-4&from=paste&height=594&id=u84133964&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=262501&status=done&style=none&taskId=u1a50a848-eed0-401b-bd58-3e1063b7861&title=&width=1280)<br />容器页面<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639982825062-83adbf0a-51cf-4d14-8b35-322f4105b8e7.png#clientId=u9658ca7b-6a90-4&from=paste&height=594&id=ub0e31d33&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=508683&status=done&style=none&taskId=ud43dd195-3cf5-444e-be6f-db5bd5ed750&title=&width=1280)
<a name="QQgPE"></a>
### 5、一键式安装启动脚本
```bash
docker pull portainer/portainer
docker volume create portainer_data
docker run -d -p 9000:9000 \
            --name portainer \
            --restart always \
            -v /var/run/docker.sock:/var/run/docker.sock \
            -v portainer_data:/data \
            portainer/portainer
```
<a name="tY5sb"></a>
### 其他安装方式
<a name="kTHgg"></a>
#### 单机版安装
```bash
# 创建存储卷
$ docker volume create portainer_data

# 启动服务
$ docker run -d -p 80:9000 \
    -v /var/run/docker.sock:/var/run/docker.sock \
    -v portainer_data:/data \
    portainer/portainer
```
<a name="PWcsE"></a>
#### 集群版安装
```bash
# 创建存储卷
$ docker volume create portainer_data

# 启动服务
$ docker service create \
    --name portainer \
    --publish 80:9000 \
    --replicas=1 \
    --constraint 'node.role == manager' \
    --mount type=bind,src=//var/run/docker.sock,dst=/var/run/docker.sock \
    --mount type=volume,src=portainer_data,dst=/data \
    portainer/portainer \
    -H unix:///var/run/docker.sock
```
<a name="r7Xrq"></a>
#### 自定义 docker-compose 安装
```yaml
version: "3.2"
services:
  portainer:
    image: portainer/portainer
    command: -H unix:///var/run/docker.sock
    restart: always
    ports:
      - 80:9000
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
      - portainer_data:/data
volumes:
  portainer_data:
```
<a name="KhAMX"></a>
#### 官网 docker-compose 安装
```yaml
version: "2"

services:
  nginx-proxy:
    image: jwilder/nginx-proxy
    restart: always
    networks:
      - proxy
    ports:
      - "80:80"
    volumes:
      - "/var/run/docker.sock:/tmp/docker.sock:ro"
      - "./vhost.d:/etc/nginx/vhost.d:ro"

  portainer:
    image: portainer/portainer
    restart: always
    environment:
      - VIRTUAL_HOST=dev.portainer
    networks:
      - proxy
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
      - portainer_data:/data

networks:
  proxy:

volumes:
  portainer_data:
```
<a name="YmTTr"></a>
#### 官网部署集群方式
```bash
# 集群也可以使用如下正统方式安装部署
$ docker stack deploy --compose-file=docker-stack.yml portainer
bash
version: '3.2'

services:
  agent:
    image: portainer/agent
    environment:
      # REQUIRED: Should be equal to the service name prefixed by "tasks." when
      # deployed inside an overlay network
      AGENT_CLUSTER_ADDR: tasks.agent
      # AGENT_PORT: 9001
      # LOG_LEVEL: debug
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
      - /var/lib/docker/volumes:/var/lib/docker/volumes
    networks:
      - agent_network
    deploy:
      mode: global
      placement:
        constraints: [node.platform.os == linux]

  portainer:
    image: portainer/portainer
    command: -H tcp://tasks.agent:9001 --tlsskipverify
    ports:
      - "9000:9000"
      - "8000:8000"
    volumes:
      - portainer_data:/data
    networks:
      - agent_network
    deploy:
      mode: replicated
      replicas: 1
      placement:
        constraints: [node.role == manager]

networks:
  agent_network:
    driver: overlay

volumes:
  portainer_data:
```
