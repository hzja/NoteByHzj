Docker Docker-compose
<a name="ZGkLE"></a>
## Docker-compose简介
Docker-Compose项目是[Docker官方的开源项目](https://docs.docker.com/compose/)，负责实现对Docker容器集群的快速编排。<br />Docker-Compose将所管理的容器分为三层，分别是工程（project），服务（service）以及容器（container）。Docker-Compose运行目录下的所有文件（docker-compose.yml，extends文件或环境变量文件等）组成一个工程，若无特殊指定工程名即为当前目录名。一个工程当中可包含多个服务，每个服务中定义了容器运行的镜像，参数，依赖。一个服务当中可包括多个容器实例，Docker-Compose并没有解决负载均衡的问题，因此需要借助其它工具实现服务发现及负载均衡。<br />Docker-Compose的工程配置文件默认为docker-compose.yml，可通过环境变量COMPOSE_FILE或-f参数自定义配置文件，其定义了多个有依赖关系的服务及每个服务运行的容器。<br />使用一个Dockerfile模板文件，可以让用户很方便的定义一个单独的应用容器。在工作中，经常会碰到需要多个容器相互配合来完成某项任务的情况。例如要实现一个Web项目，除了Web服务容器本身，往往还需要再加上后端的数据库服务容器，甚至还包括负载均衡容器等。<br />Compose允许用户通过一个单独的docker-compose.yml模板文件（YAML 格式）来定义一组相关联的应用容器为一个项目（project）。<br />Docker-Compose项目由Python编写，调用Docker服务提供的API来对容器进行管理。因此，只要所操作的平台支持Docker API，就可以在其上利用Compose来进行编排管理。
<a name="JSlY6"></a>
## Docker-compose的安装与卸载
<a name="HsYse"></a>
### Docker-compose的安装
<a name="IR2yD"></a>
#### ①进入Docker-compose的Github发布页面选择合适的版本
[Docker-compose的Github发布页](https://github.com/docker/compose/releases/)<br />[https://github.com/docker/compose/releases/](https://github.com/docker/compose/releases/)

---

[docker-compose.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1663856217062-9f55e501-6065-4ab1-8985-458dfce95229.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2022%2Fzip%2F396745%2F1663856217062-9f55e501-6065-4ab1-8985-458dfce95229.zip%22%2C%22name%22%3A%22docker-compose.zip%22%2C%22size%22%3A14041362%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22uf6480f9d-ad7a-431f-8389-d467831b656%22%2C%22taskType%22%3A%22upload%22%2C%22__spacing%22%3A%22both%22%2C%22id%22%3A%22u2402b163%22%2C%22margin%22%3A%7B%22top%22%3Atrue%2C%22bottom%22%3Atrue%7D%2C%22card%22%3A%22file%22%7D)linux-x86_64   v2.11.0 稳定版<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598760244197-663c2593-b200-458c-880e-f241326bf0e9.png#height=1030&id=BFEvF&originHeight=2060&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=717073&status=done&style=shadow&title=&width=1899)
<a name="hX2gD"></a>
#### ②下载Docker-compose
```bash
curl -L https://github.com/docker/compose/archive/1.26.2.tar.gz -o /usr/local/bin/docker-compose
```
<a name="GSBqp"></a>
#### ③为Docker-compose添加可执行权限
```bash
chmod +x /usr/local/bin/docker-compose
```
<a name="gyKhh"></a>
#### ④查看安装的版本
```bash
docker-compose --version
```
<a name="n1Jv8"></a>
## Docker-compose常用命令
<a name="wpqYq"></a>
### Docker-compose命令格式
```bash
docker-compose [-f <arg>...] [options] [COMMAND] [ARGS...]

命令选项如下:
-f, --file FILE指定Compose模板文件，默认为docker-compose.yml
-p, --project-name NAME 指定项目名称，默认使用当前所在目录为项目名
--verbose  输出更多调试信息
-v, --version 打印版本并退出
--log-level LEVEL 定义日志等级(DEBUG, INFO, WARNING, ERROR, CRITICAL)
```
<a name="ditGQ"></a>
#### `docker-compose up`-启动服务容器
```bash
docker-compose up [options] [--scale SERVICE=NUM...] [SERVICE...]

命令选项包括：
-d 在后台运行服务容器
--no-color 不是有颜色来区分不同的服务的控制输出
--no-deps 不启动服务所链接的容器
--force-recreate 强制重新创建容器，不能与-no-recreate同时使用
--no-recreate 如果容器已经存在，则不重新创建，不能与–force-recreate同时使用
--no-build 不自动构建缺失的服务镜像
--build 在启动容器前构建服务镜像
--abort-on-container-exit 停止所有容器，如果任何一个容器被停止，不能与-d同时使用
-t, --timeout TIMEOUT 停止容器时候的超时（默认为10秒）
--remove-orphans 删除服务中没有在compose文件中定义的容器
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663443673-e912ba16-b71c-4501-9091-2c8d6fb0a1bd.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=629&id=ubf3a3143&originHeight=1572&originWidth=1968&originalType=binary&ratio=1&rotation=0&showTitle=false&size=236099&status=error&style=shadow&taskId=uadf1ee1d-79fa-4522-ae39-16c7521e469&title=&width=787.2)
<a name="OZ574"></a>
#### `docker-compose ps`-列出项目中所有的容器
```bash
docker-compose ps [options] [SERVICE...]
列出项目中所有的容器
```
<a name="cTSgQ"></a>
#### `docker-compose stop`-停止正在运行的容器
```bash
docker-compose stop [options] [SERVICE...]
选项包括
-t, --timeout TIMEOUT 停止容器时候的超时（默认为10秒）
docker-compose stop
停止正在运行的容器，可以通过docker-compose start 再次启动
```
<a name="NHmd0"></a>
#### `docker-compose -h`-查看帮助
```bash
docker-compose -h
查看帮助
```
<a name="I3BzR"></a>
#### `docker-compose down`-停止和删除容器、网络、卷、镜像
```bash
docker-compose down [options]
停止和删除容器、网络、卷、镜像。
选项包括：
--rmi type，删除镜像，类型必须是：all，删除compose文件中定义的所有镜像；local，删除镜像名为空的镜像
-v, --volumes，删除已经在compose文件中定义的和匿名的附在容器上的数据卷
--remove-orphans，删除服务中没有在compose中定义的容器
docker-compose down
停用移除所有容器以及网络相关
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663470657-0da7bdb6-1f1f-4f4d-97b6-bb2bb33c2ed9.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=548&id=u51258a66&originHeight=1370&originWidth=1903&originalType=binary&ratio=1&rotation=0&showTitle=false&size=150070&status=error&style=shadow&taskId=ufd31e086-042e-4021-beb2-dde1d924ae7&title=&width=761.2)
<a name="TRYRE"></a>
#### `docker-compose logs`-查看服务容器的输出
```bash
docker-compose logs [options] [SERVICE...]
查看服务容器的输出。默认情况下，docker-compose将对不同的服务输出使用不同的颜色来区分。可以通过–no-color来关闭颜色。
docker-compose logs
查看服务容器的输出
-f 跟踪日志输出
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663532012-c42f158a-ffd7-4659-bf0d-e6b74f58eabb.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=231&id=u56597b44&originHeight=577&originWidth=1856&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60531&status=error&style=shadow&taskId=ud47e98b9-7e16-4072-be92-6a0d6c67e67&title=&width=742.4)
<a name="FXEHH"></a>
#### `docker-compose bulid`-构建（重新构建）项目中的服务容器
```bash
docker-compose build [options] [--build-arg key=val...] [SERVICE...]
构建（重新构建）项目中的服务容器。
选项包括：
--compress 通过gzip压缩构建上下环境
--force-rm 删除构建过程中的临时容器
--no-cache 构建镜像过程中不使用缓存
--pull 始终尝试通过拉取操作来获取更新版本的镜像
-m, --memory MEM为构建的容器设置内存大小
--build-arg key=val为服务设置build-time变量
服务容器一旦构建后，将会带上一个标记名。可以随时在项目目录下运行docker-compose build来重新构建服务
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663568380-669a4a28-fff8-4ddc-8aea-c6c6146bfb40.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=464&id=u16cdd7c6&originHeight=1159&originWidth=2136&originalType=binary&ratio=1&rotation=0&showTitle=false&size=169184&status=error&style=shadow&taskId=uc34bc455-f0fc-4bc6-98b0-d548562566f&title=&width=854.4)
<a name="VF3pw"></a>
#### `docker-compose pull`-拉取服务依赖的镜像
```bash
docker-compose pull [options] [SERVICE...]
拉取服务依赖的镜像。
选项包括：
--ignore-pull-failures，忽略拉取镜像过程中的错误
--parallel，多个镜像同时拉取
-q, --quiet，拉取镜像过程中不打印进度信息
docker-compose pull
拉取服务依赖的镜像
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663617897-2fe65f1b-2c3b-4f9a-b0d7-e8eb82355d49.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=252&id=ub3e1003e&originHeight=630&originWidth=2143&originalType=binary&ratio=1&rotation=0&showTitle=false&size=86390&status=error&style=shadow&taskId=u148bf5fe-becb-450c-9196-a23cf692304&title=&width=857.2)
<a name="OiROS"></a>
#### `docker-compose restart`-重启项目中的服务
```bash
docker-compose restart [options] [SERVICE...]
重启项目中的服务。
选项包括：
-t, --timeout TIMEOUT，指定重启前停止容器的超时（默认为10秒）
docker-compose restart
重启项目中的服务
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663669542-a41431fa-26c3-43cd-b925-a44fe5f90522.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=168&id=u92cb1399&originHeight=420&originWidth=1870&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43344&status=error&style=shadow&taskId=ucd760254-0390-4124-b2f6-55ff3ca9bb8&title=&width=748)
<a name="zXhMj"></a>
#### `docker-compose rm`-删除所有（停止状态的）服务容器
```bash
docker-compose rm [options] [SERVICE...]
删除所有（停止状态的）服务容器。
选项包括：
-f, --force，删除时不需要确认，但不能删除非停止状态的容器
-v，删除容器所挂载的数据卷
docker-compose rm
删除所有（停止状态的）服务容器。推荐先执行docker-compose stop命令来停止容器。
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663694828-ebc658e2-972c-4b8a-85f8-9ae93e393ed9.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=313&id=ufaae3463&originHeight=783&originWidth=1892&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91264&status=error&style=shadow&taskId=u2e931907-cd18-49ee-9bcb-1b512e6b29c&title=&width=756.8)
<a name="1pCH1"></a>
#### `docker-compose start`-启动已经存在的服务容器
```bash
docker-compose start [SERVICE...]
docker-compose start
启动已经存在的服务容器。
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663727765-05a94fa3-dee7-475c-a3eb-e33ea42ad1eb.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=83&id=u62b19cb0&originHeight=208&originWidth=1938&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25027&status=error&style=shadow&taskId=uaa208b98-20de-4d8f-9485-c8bddf66c59&title=&width=775.2)
<a name="k4JIN"></a>
#### `docker-compose run`-在指定服务上执行一个命令
```bash
docker-compose run [options] [-v VOLUME...] [-p PORT...] [-e KEY=VAL...] SERVICE [COMMAND] [ARGS...]
在指定服务上执行一个命令。
docker-compose run ubuntu ping www.baidu.com
在指定容器上执行一个ping命令。
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663761480-09e86f2e-2066-44a7-8702-7efadd50b7e1.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=606&id=ub73a52d3&originHeight=1515&originWidth=2111&originalType=binary&ratio=1&rotation=0&showTitle=false&size=208748&status=error&style=shadow&taskId=ud88fe928-a7e6-48c2-b536-1bb20a823cf&title=&width=844.4)
<a name="JcWxZ"></a>
#### `docker-compose scale`-设置指定服务运行的容器个数
```bash
docker-compose scale web=3 db=2
设置指定服务运行的容器个数。通过service=num的参数来设置数量
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663790176-16ad0488-f2a8-40de-b625-900f44d6c353.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=336&id=u1abd64f3&originHeight=841&originWidth=1955&originalType=binary&ratio=1&rotation=0&showTitle=false&size=79147&status=error&style=shadow&taskId=ud4ccf888-6e81-4001-a239-c4d698969bf&title=&width=782)
<a name="ysUbx"></a>
#### `docker-compose pause`-暂停一个服务容器
```bash
docker-compose pause [SERVICE...]
暂停一个服务容器
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663837011-f2c3a2d4-c05c-4117-b89e-83a3695f6481.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=89&id=u06ace88e&originHeight=222&originWidth=1699&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21823&status=error&style=shadow&taskId=u3d682b12-7e3f-4506-84b3-2d1b573a5a9&title=&width=679.6)
<a name="ZAJqa"></a>
#### `docker-compose kill`-通过发送SIGKILL信号来强制停止服务容器
```bash
docker-compose kill [options] [SERVICE...]
通过发送SIGKILL信号来强制停止服务容器。
支持通过-s参数来指定发送的信号，例如通过如下指令发送SIGINT信号：
docker-compose kill -s SIGINT
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663860026-acfc4feb-3a73-451a-971a-549c6b35fe10.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=166&id=ua91d56a2&originHeight=416&originWidth=1746&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37107&status=error&style=shadow&taskId=u7ce789f5-22e1-47ca-8c14-f56624d13fb&title=&width=698.4)
<a name="ufktZ"></a>
#### `docker-compose config`-验证并查看compose文件配置
```bash
docker-compose config [options]
验证并查看compose文件配置。
选项包括：
--resolve-image-digests 将镜像标签标记为摘要
-q, --quiet 只验证配置，不输出。 当配置正确时，不输出任何内容，当文件配置错误，输出错误信息
--services 打印服务名，一行一个
--volumes 打印数据卷名，一行一个
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663883555-3e487150-0b1a-42c5-9994-5f3114677fd2.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=313&id=u6e450d94&originHeight=783&originWidth=2038&originalType=binary&ratio=1&rotation=0&showTitle=false&size=98438&status=error&style=shadow&taskId=udf011000-70d0-4b80-972b-544113246ff&title=&width=815.2)
<a name="enPAr"></a>
#### `docker-compose create`-为服务创建容器
```bash
docker-compose create [options] [SERVICE...]
为服务创建容器。
选项包括：
--force-recreate：重新创建容器，即使配置和镜像没有改变，不兼容–no-recreate参数
--no-recreate：如果容器已经存在，不需要重新创建，不兼容–force-recreate参数
--no-build：不创建镜像，即使缺失
--build：创建容器前，生成镜像
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663920499-fdd14db0-7193-4580-98f9-08f7b341f1b9.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=272&id=ud21d6832&originHeight=681&originWidth=1969&originalType=binary&ratio=1&rotation=0&showTitle=false&size=90891&status=error&style=shadow&taskId=ubadad388-6f16-49f8-8038-6a22f44bf39&title=&width=787.6)
<a name="52rHC"></a>
#### `docker-compose exec`-进入容器
```bash
docker-compose exec [options] SERVICE COMMAND [ARGS...]
选项包括：
-d 分离模式，后台运行命令。
--privileged 获取特权。
-u, --user USER 指定运行的用户。
-T 禁用分配TTY，默认docker-compose exec分配TTY。
--index=index，当一个服务拥有多个容器时，可通过该参数登陆到该服务下的任何服务，例如：
docker-compose exec –index=1 web /bin/bash ，web服务中包含多个容器
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663954685-1535b1c9-fd85-4f3e-82f8-fc98ba67625d.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=333&id=u69be0633&originHeight=833&originWidth=2051&originalType=binary&ratio=1&rotation=0&showTitle=false&size=113361&status=error&style=shadow&taskId=u3be450df-2d19-44cb-bd20-b3faf493cd0&title=&width=820.4)
<a name="kWLvy"></a>
#### `docker-compose port`-显示某个容器端口所映射的公共端口
```bash
docker-compose port [options] SERVICE PRIVATE_PORT
显示某个容器端口所映射的公共端口。
选项包括：
--protocol=proto，指定端口协议，TCP（默认值）或者UDP
--index=index，如果同意服务存在多个容器，指定命令对象容器的序号（默认为1）
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656663999721-b7956198-54c0-4e00-8b0b-8a13501c85ec.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=186&id=uf60d2239&originHeight=466&originWidth=1752&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50853&status=error&style=shadow&taskId=u0e3c979c-c997-4c0d-9d02-5d5e27a6f71&title=&width=700.8)
<a name="HrxPw"></a>
#### `docker-compose push`-推送服务依赖的镜像
```bash
docker-compose push [options] [SERVICE...]
推送服务依赖的镜像。
选项包括：
--ignore-push-failures 忽略推送镜像过程中的错误
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656664078603-d26b4226-be89-4d70-93f9-c464af1701f4.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=146&id=u2a774646&originHeight=365&originWidth=1981&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40125&status=error&style=shadow&taskId=ua9b5e037-2daf-4b5c-b13f-fbd4dac5f2d&title=&width=792.4)
<a name="f5guD"></a>
#### `docker-compose stop`-停止运行的容器
```bash
docker-compose stop [options] [SERVICE...]
停止运行的容器
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656664145973-64fbd7db-28e6-41d1-83b3-1d9f4bd8ccc0.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=210&id=uf1a1c67e&originHeight=526&originWidth=1818&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52446&status=error&style=shadow&taskId=uc72ed698-82aa-4456-a012-ad4102f408b&title=&width=727.2)
<a name="X4PL9"></a>
#### `docker-compose unppause`-恢复处于暂停状态中的服务
```bash
docker-compose unpause [SERVICE...]
恢复处于暂停状态中的服务。
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1656664219499-acab727e-c809-49f3-baed-dcea021f2237.png#clientId=u4896b9da-52ad-4&errorMessage=unknown%20error&from=paste&height=82&id=u7088b018&originHeight=206&originWidth=1547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21245&status=error&style=shadow&taskId=ue7477d3a-48fb-4077-ac83-9a4882a9187&title=&width=618.8)
<a name="pohV0"></a>
## Docker-compose模板文件
<a name="DE2S8"></a>
### Docker-compose模板文件简介
Compose允许用户通过一个docker-compose.yml模板文件（YAML 格式）来定义一组相关联的应用容器为一个项目（project）。<br />Compose模板文件是一个定义服务、网络和卷的YAML文件。Compose模板文件默认路径是当前目录下的docker-compose.yml，可以使用.yml或.yaml作为文件扩展名。<br />Docker-Compose标准模板文件应该包含version、services、networks 三大部分，最关键的是services和networks两个部分。
```yaml
version: '2'
services:
  web:
    image: dockercloud/hello-world
    ports:
      - 8080
    networks:
      - front-tier
      - back-tier
 
  redis:
    image: redis
    links:
      - web
    networks:
      - back-tier
 
  lb:
    image: dockercloud/haproxy
    ports:
      - 80:80
    links:
      - web
    networks:
      - front-tier
      - back-tier
    volumes:
      - /var/run/docker.sock:/var/run/docker.sock
 
networks:
  front-tier:
    driver: bridge
  back-tier:
    driver: bridge
```
Compose目前有三个版本分别为Version 1，Version 2，Version 3，Compose区分Version 1和Version 2（Compose 1.6.0+，Docker Engine 1.10.0+）。Version 2支持更多的指令。Version 1将来会被弃用。
<a name="XABtN"></a>
#### image
image是指定服务的镜像名称或镜像ID。如果镜像在本地不存在，Compose将会尝试拉取镜像。
```yaml
services:
    web:
        image: hello-world
```
<a name="BkFYO"></a>
#### build
服务除了可以基于指定的镜像，还可以基于一份Dockerfile，在使用up启动时执行构建任务，构建标签是build，可以指定Dockerfile所在文件夹的路径。Compose将会利用Dockerfile自动构建镜像，然后使用镜像启动服务容器。
```yaml
build: /path/to/build/dir
```
也可以是相对路径，只要上下文确定就可以读取到Dockerfile。
```yaml
build: ./dir
```
设定上下文根目录，然后以该目录为准指定Dockerfile。
```yaml
build:
  context: ../
  dockerfile: path/of/Dockerfile
```
build都是一个目录，如果要指定Dockerfile文件需要在build标签的子级标签中使用dockerfile标签指定。<br />如果同时指定image和build两个标签，那么Compose会构建镜像并且把镜像命名为image值指定的名字。
<a name="7OAZR"></a>
#### context
context选项可以是Dockerfile的文件路径，也可以是到链接到git仓库的url，当提供的值是相对路径时，被解析为相对于撰写文件的路径，此目录也是发送到Docker守护进程的context
```yaml
build:
  context: ./dir
```
<a name="saxVT"></a>
#### dockerfile
使用dockerfile文件来构建，必须指定构建路径
```yaml
build:
  context: .
  dockerfile: Dockerfile-alternate
```
<a name="a2hCG"></a>
#### commond
使用command可以覆盖容器启动后默认执行的命令。
```yaml
command: bundle exec thin -p 3000
```
<a name="gbveu"></a>
#### container_name
Compose的容器名称格式是：<项目名称><服务名称><序号><br />可以自定义项目名称、服务名称，但如果想完全控制容器的命名，可以使用标签指定：
```yaml
container_name: app
```
<a name="4yzum"></a>
#### depends_on
在使用Compose时，最大的好处就是少打启动命令，但一般项目容器启动的顺序是有要求的，如果直接从上到下启动容器，必然会因为容器依赖问题而启动失败。例如在没启动数据库容器的时候启动应用容器，应用容器会因为找不到数据库而退出。depends_on标签用于解决容器的依赖、启动先后的问题
```yaml
version: '2'
services:
  web:
    build: .
    depends_on:
      - db
      - redis
  redis:
    image: redis
  db:
    image: postgres
```
上述YAML文件定义的容器会先启动redis和db两个服务，最后才启动web 服务。
<a name="5taoW"></a>
#### PID
`pid: "host"`<br />将PID模式设置为主机PID模式，跟主机系统共享进程命名空间。容器使用pid标签将能够访问和操纵其他容器和宿主机的名称空间。
<a name="LNnjS"></a>
#### ports
ports用于映射端口的标签。<br />使用HOST:CONTAINER格式或者只是指定容器的端口，宿主机会随机映射端口。
```yaml
ports:
 - "3000"
 - "8000:8000"
 - "49100:22"
 - "127.0.0.1:8001:8001"
```
当使用HOST:CONTAINER格式来映射端口时，如果使用的容器端口小于60可能会得到错误得结果，因为YAML将会解析xx:yy这种数字格式为60进制。所以建议采用字符串格式。
<a name="GrXaa"></a>
#### extra_hosts
添加主机名的标签，会在/etc/hosts文件中添加一些记录。
```yaml
extra_hosts:
 - "somehost:162.242.195.82"
 - "otherhost:50.31.209.229"
```
启动后查看容器内部hosts：
```yaml
162.242.195.82  somehost
50.31.209.229   otherhost
```
<a name="dV1Pt"></a>
#### volumes
挂载一个目录或者一个已存在的数据卷容器，可以直接使用 [HOST:CONTAINER]格式，或者使用[HOST:CONTAINER:ro]格式，后者对于容器来说，数据卷是只读的，可以有效保护宿主机的文件系统。<br />Compose的数据卷指定路径可以是相对路径，使用 . 或者 .. 来指定相对目录。<br />数据卷的格式可以是下面多种形式
```yaml
volumes:
  // 只是指定一个路径，Docker 会自动在创建一个数据卷（这个路径是容器内部的）。
  - /var/lib/mysql
  // 使用绝对路径挂载数据卷
  - /opt/data:/var/lib/mysql
  // 以 Compose 配置文件为中心的相对路径作为数据卷挂载到容器。
  - ./cache:/tmp/cache
  // 使用用户的相对路径（~/ 表示的目录是 /home/<用户目录>/ 或者 /root/）。
  - ~/configs:/etc/configs/:ro
  // 已经存在的命名的数据卷。
  - datavolume:/var/lib/mysql
```
如果不使用宿主机的路径，可以指定一个volume_driver。<br />`volume_driver: mydriver`
<a name="KAYC4"></a>
#### volumes_from
从另一个服务或容器挂载其数据卷：
```yaml
volumes_from:
   - service_name   
     - container_name
```
<a name="5UFU0"></a>
#### dns
自定义DNS服务器。可以是一个值，也可以是一个列表。
```yaml
dns: 8.8.8.8
dns: 
    - 8.8.8.8   
      - 9.9.9.9
```
<a name="6oQY9"></a>
#### expose
暴露端口，但不映射到宿主机，只允许能被连接的服务访问。仅可以指定内部端口为参数，如下所示：
```yaml
expose:
    - "3000"
    - "8000"
```
<a name="Alc2r"></a>
#### links
链接到其它服务中的容器。使用服务名称（同时作为别名），或者“服务名称:服务别名”（如 SERVICE:ALIAS），例如：
```yaml
links:
    - db
    - db:database
    - redis
```
<a name="BKJUx"></a>
#### net
设置网络模式。
```yaml
net: "bridge"
net: "none"
net: "host"
```
<a name="1hRv8"></a>
### Docker-compose模板文件示例
<a name="CYIPv"></a>
#### Docker-compose模板文件编写
<a name="wrqtz"></a>
##### docker-compose.yml
```yaml
version: '2'
services:
  web1:
    image: nginx
    ports:
      - "6061:80"
    container_name: "web1"
    networks:
      - dev
  web2:
    image: nginx
    ports:
      - "6062:80"
    container_name: "web2"
    networks:
      - dev
      - pro
  web3:
    image: nginx
    ports:
      - "6063:80"
    container_name: "web3"
    networks:
      - pro
 
networks:
  dev:
    driver: bridge
  pro:
    driver: bridge
```
docker-compose.yml文件指定了3个web服务
<a name="oZoMa"></a>
#### 启动应用
创建一个webapp目录，将docker-compose.yaml文件拷贝到webapp目录下，使用docker-compose启动应用。
```bash
docker-compose up -d
```
<a name="ngYX1"></a>
#### 服务访问
通过浏览器访问web1，web2，web3
```nginx
http://127.0.0.1:6061
http://127.0.0.1:6062
http://127.0.0.1:6063
```
