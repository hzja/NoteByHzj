Docker<br />Docker 网络 IP 地址冲突通常发生在跨主机部署的多个 Docker 容器使用了同一个 IP 地址的情况下。导致这种情况的原因可能是由于不同的容器映射到同一个主机端口，或由于在 Docker 网络之间启用了容器之间的互相访问。<br />要处理 Docker 网络 IP 地址冲突，可以采取以下步骤：
<a name="X2UlW"></a>
### 1、查找重复 IP 地址
_运行以下命令查找重复的 IP 地址：_
```bash
docker network ls
docker inspect <network_name>
```
_执行结果：_这将列出网络列表和相应的详细信息，包括与网络连接的容器的详细信息。在详细信息记录中查找重复的 IP 地址。
<a name="UEWyt"></a>
### 2、停用容器
_确定使用相同 IP 地址的容器后，可以首先尝试通过停用其中一个容器来解决问题。_
```bash
docker stop <container_name>
```
_执行结果：_这会停止容器并释放用于其 IP 地址的资源。如果容器被重新启动，则可能会重新获得不同的 IP 地址。
<a name="HrnTX"></a>
### 3、更改重复 IP 地址
_如果需要继续使用两个容器，则可以手动更改其 IP 地址。_首先，通过 shell 进入到容器中，然后更改容器的 IP 地址并退出容器。
```bash
docker exec -it <container_name> /bin/bash
ip addr show
ip addr add <new_ip_address>/<subnet_mask> dev eth0
exit
```
_执行结果：_查询 - 查找容器的当前 IP 地址和子网掩码。更改 - 将容器的 IP 地址更改为新的IP地址。退出 - 退出容器并返回主机命令行。
<a name="yDl7M"></a>
### 4、清理 Docker 环境
_如果以上步骤都不能解决问题，则可能需要清理 Docker 环境并重新启动所有容器。确保备份任何重要数据和配置文件。_

1. 停止所有容器。
```bash
docker stop $(docker ps -aq)
```

2. 移除所有容器
```bash
docker rm $(docker ps -aq)
```

3. 移除所有镜像
```bash
docker rmi -f $(docker images -q)
```

4. 删除所有 Docker 网络
```bash
docker network prune
```

5. 重新启动 Docker 服务
```bash
systemctl restart docker
```
_执行结果：_这会移除所有容器和图像以及 Docker 网络，并重新启动 Docker 服务。这将彻底解决 Docker 网络 IP 地址冲突的问题。<br />总之，建议开发人员在启动容器时要避免使用相同的 IP 地址，使用唯一的 IP 地址，并通过标签将容器映射到不同的端口，以避免可能导致 Docker 网络 IP 地址冲突的问题。<br />另外，可以考虑在 Docker 网络之间启用容器访问控制来限制容器之间的通信，并避免由于配置错误导致的 IP 地址冲突问题。通过限制容器之间的访问，可以确保容器只能与其所需的特定容器通信，并且可以提高容器之间的安全性。<br />综上所述，处理 Docker 网络 IP 地址冲突可能需要多种方法，包括手动更改容器的 IP 地址，停用不需要的容器或清理整个 Docker 环境。最好在启动容器时，避免使用相同的 IP 地址，可以提高容器环境的稳定性和安全性。
