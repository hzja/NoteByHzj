Docker
<a name="YqH4h"></a>
## 1、通过容器构建
```bash
$ docker commit
```
**参数说明：**
```bash
-a --author=""指定作者
-m --message=""构建说明
-p --pause=true
```
<a name="HX4fP"></a>
## 2、通过Dockerfile构建
<a name="kxkwo"></a>
### A、命令
```bash
$ docker build [OPTIONS] PATH | URL | -
```
**参数说明：**
```bash
--force-rm=false
--no-cache=false
--pull=false
-q --quiet=false
--rm=true
-t --tag="指定构建镜像Dockerfile文件的路径"
```
<a name="GlUDE"></a>
### B、Dockerfile指令格式
```dockerfile
#First Dockerfile
FROM ubuntu:14.04
MAINTAINER dormancypress "dormancypress@outlook.com"
RUN apt-get update
RUN apt-get install -y nginx
EXPOSE 80
```
<a name="FHsmV"></a>
### C、Dockerfile参数介绍
| **参数** |   |   |
| --- | --- | --- |
| `**FROM <image>**`<br />`**FROM <image>:<tag>**` | **构建镜像依赖的基础镜像** | **该镜像必须是基础镜像** |
| `**MAINTAINER**` | **表示镜像的所有者以及作者** |   |
| `**RUN <command>**`<br />`**RUN   ["executable","param1","param2"]**` | **指定当前镜像运行的命令** | **shell模式**<br />**     /bin/sh -c command**<br />**exec模式**<br />**    **`**RUN ["/bin/bash","-c","echo hello"]**` |
| `**EXPOSE <port> [<port>…]**` | **指定运行该镜像容器使用的端口** | **指定一个或多个端口** |
| `**CMD command param1 param2**`<br />`**CMD ["executable","param1","param2"]**`<br />`**CMD ["param1","param2"]**` | **指定容器运行时的运行命令**<br />**等级:RUN>CMD** | **shell模式**<br />**exec模式**<br />**作为ENTRYPOINT指令的默认参数** |
| `**ENTERYPOINT   ["executable","param1","param2"]**`<br />`**ENTERYPOINT command param1 param2**` | **指定容器运行时的运行命令**<br />**不会被覆盖,除非使用**`**docker run --entrypoint**`**命令特殊指定** | `**exec**`**模式**<br />**shell模式** |
| `**ADD <src>…<dest>**`<br />`**ADD ["<src>"…"<dest>"]**` | **设置镜像的目录文件**<br />**ADD包含类似tar的解压功能** | **目标文件的来源地址(本地地址,构建目录的相对地址&#124;远程地址,不建议使用)和目标地址(镜像路径的绝对路径)**<br />**适用于文件路径中有空格的情况** |
| `**COPY ["<src>"…"<dest>"]**`<br />`**COPY <src>…<dest>**` | **设置镜像的目录文件**<br />**单纯复制文件,Docker推荐使用COPY** | **适用于文件路径中有空格的情况**<br />**目标文件的来源地址(本地地址,构建目录的相对地址&#124;远程地址,不建议使用)和目标地址(镜像路径的绝对路径)**<br />  |
| `**VOLUME ["/data"]**` |   |   |
| `**WORKDIR /path/to/workdir**` | **镜像构建及运行时的环境设置**<br />**镜像创建容器时设置工作目录**<br />**通常使用绝对路径** |   |
| `**ENV <key> <value>**`<br />`**ENV <key>=<value>**`**…** | **设置环境变量** |   |
| `**USER daemon**` | **设置基于那种用户的形式运行** | **USER nginx 以nginx的身份形式运行** |
| `**ONBUILD [INSTRUCTION]**` | **镜像触发器**<br />**当一个镜像被其他镜像作为基础镜像时执行**<br />**该命令在构建过程插入** |   |
| `**$ docker build --no-cache**` | **构建过程不使用缓存** |   |
| `**$ docker history <image>**` | **查看构建历史** |   |


