Docker<br />Docker作为轻量级的、高性能的沙箱容器，使用频率极高，功能非常强大。<br />强大的功能需要繁杂的命令来支撑。这里汇总了docker常用的命令，并对每个命令进行说明和举例。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623857575894-7da2f678-9f20-4d62-b375-2ab1510a63ce.png#averageHue=%23f1efec&height=782&id=u45727a1c&originHeight=782&originWidth=911&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&title=&width=911)<br />镜像仓库用来保存镜像，可分为远程镜像仓库和本地镜像仓库。<br />通过pull命令可以把远程仓库的镜像下载到本地，通过push命令可以把本地仓库的镜像推送到远程<br />本地仓库中的镜像可以用来创建容器，一个镜像可以创建多个容器<br />容器也可以通过commit命令打包成镜像，提交到本地仓库
<a name="BbbLo"></a>
## 操作远程仓库的命令
<a name="HLDdt"></a>
### login：登录到远程仓库
login命令可以登录到远程仓库，登录到远程仓库后可可以拉取仓库的镜像了
<a name="lCGVU"></a>
#### login语法
```bash
docker login [OPTIONS] [SERVER]
```
OPTIONS：可选参数<br />SERVER：远程仓库的地址，默认为docker官方仓库，也就是 [https://hub.docker.com/](https://hub.docker.com/)<br />OPTIONS的常用值<br />-u string：用户名<br />-p string：密码
<a name="ea9Ro"></a>
#### login常用写法
使用helianxiaowu用户登录远程仓库，密码为123456
```bash
docker login -u helianxiaowu -p 123456 192.168.10.10/docker-lib
```
不指定用户登录到远程仓库，这时会提示输入用户名或密码
```bash
docker login 192.168.10.10/docker-lib
```
不指定用户登录到默认的远程仓库，也会提示输入用户名或密码
```bash
docker login
```
<a name="ZpGbQ"></a>
### search：从远程仓库搜索镜像
search命令可以从远程仓库搜索镜像

- 列含义如下
| NAME | DESCRIPTION | STARS | OFFICIAL | AUTOMATED |
| --- | --- | --- | --- | --- |
| 镜像名称 | 镜像描述 | 镜像热度，类似于github的starts | 是否是官方发布 | 是否自动构建 |

<a name="zneR2"></a>
#### search语法
```bash
docker search [OPTIONS] TERM
```
OPTIONS：可选参数<br />TERM：镜像的关键词<br />OPTIONS的常用值<br />-ffilter：根据条件过滤镜像，过滤条件详见下文<br />no-trunc：显示完整的镜像描述。默认情况下，搜索出来的镜像的描述太长的话会隐藏，no-trunc参数会让镜像信息完整的展示出来<br />--limitint：限制搜索出来的镜像的个数，最大不能超过100个，默认25个<br />--formatstring：指定镜像显示的格式，格式详见下文

- `-f`参数表示根据条件过滤搜索出来的镜像，语法如下
```bash
docker search -f KEY=VALUE TERM
```

- KEY的可选值如下

starsint：根据热度过滤，如：stars=10表示过滤热度大于10的镜像<br />is-automatedboolean：根据是否自动构建过滤，如：is-automated=false表示过滤非自动构建的镜像<br />is-officialboolean：根据是否官方发布过滤，如：is-official=false表示过滤非官方发布的镜像

- --format参数用来指定搜索出来的镜像的显示的格式，语法如下。table表示使用表格的方式显示，支持\t格式

docker search --format "[table] {{COLUMN}}[{{COLUMN}}...]" TERM

- COLUMN的可选值如下：

.Name：显示镜像的名称列<br />.Description：显示镜像的描述列<br />.StarCount：显示镜像的热度一列<br />.IsOfficial：显示镜像是否是官方发布一列<br />.IsAutomated：显示镜像是否是自动构建一列
<a name="v0fab"></a>
#### search常用写法
搜索centos镜像
```bash
docker search centos
```
搜索centos镜像，只展示5个
```bash
docker search --limit 5 centos
```
搜索热度大于100并且不是自动构建的centos镜像
```bash
docker search -f stars=100 -f is-automated=true centos
```
搜索非官方发布的centos镜像，搜索结果只展示名称和热度，列之间用TAB键隔开
```bash
docker search -f is-official=false --format "table{{.Name}}\t{{.StarCount}}" centos
```
<a name="WYHDt"></a>
### push：把本地镜像推送到远程仓库
push可以把本地仓库中的镜像推送到远程仓库，不过需要先登录远程仓库
<a name="fAUq7"></a>
#### push用法
```bash
docker push [OPTIONS] NAME[:TAG]
```
OPTIONS：可选参数<br />NAME：镜像名称<br />TAG：镜像版本号，可省略，默认为latest<br />OPTIONS的常用值<br />--disable-content-trust：推送时远程仓库不校验签名，默认为true
<a name="McUrd"></a>
#### push常用写法
将my-image镜像的1.1.0版本推送到远程仓库
```bash
docker push my-image:1.1.0
```
将my-image镜像推送到远程仓库，不指定版本时默认为latest版本
```bash
docker push my-image
```
<a name="tdT3U"></a>
### pull：从远程仓库拉取或更新镜像
pull命令可以从远程仓库拉取镜像，如果本地仓库已经存在该镜像，则会更新
<a name="Xz4zn"></a>
#### pull语法
```bash
docker pull [OPTIONS] NAME[:TAG|@DIGEST]
```
OPTIONS：可选参数<br />NAME：镜像名称<br />TAG：镜像版本号，可省略，默认为latest<br />DIGEST：镜像的摘要，每个镜像都有对应的名称、id、摘要信息，每个摘要信息能唯一代表一个镜像。<br />OPTIONS的常用值<br />-a：拉取镜像的所有版本号<br />--disable-content-trust：推送时远程仓库不校验签名，默认为true<br />-q：安静模式，推送过程中不展示详细信息
<a name="MAHLr"></a>
#### pull常用写法
从远程仓库拉取centos镜像，不指定版本时默认为latest版本
```bash
docker pull centos
```
使用安静模式从远程仓库拉取版本号为5.11的centos镜像
```bash
docker pull -q centos:5.11
```
使用安静模式从远程仓库拉取所有版本号的centos镜像
```bash
docker pull -a -q centos
```
<a name="RAaxB"></a>
## 操作本地镜像的命令
<a name="m4FMX"></a>
### images：显示所有镜像
images命令可以显示本地存在的所有镜像

- 列含义如下
| REPOSITORY | TAG | IMAGE ID | CREATED | SIZE |
| --- | --- | --- | --- | --- |
| 仓库路径 | 镜像版本 | 镜像id | 创建时间 | 镜像大小 |

<a name="Lc9pO"></a>
#### images语法
```bash
docker images [OPTIONS] [REPOSITORY[:TAG]]
```
OPTIONS：可选参数<br />REPOSITORY：镜像路径<br />TAG：镜像版本<br />OPTIONS的常用值<br />-a：显示所有镜像，包含中间映像（默认情况下中间映像是隐藏的）<br />-ffilter：根据条件过滤镜像，过滤条件详见下文<br />-q：只显示镜像id<br />no-trunc：显示完整的镜像id。默认情况下，镜像的id只显示前12位，no-trunc参数会将镜像id完整的显示出来<br />--digests：显示镜像的摘要信息<br />--format string: 指定镜像显示的格式，格式详见下文

- -f参数表示根据条件过滤要显示的镜像，语法如下
```bash
docker images -f KEY=VALUE [REPOSITORY[:TAG]]
```

- KEY的可选值如下

dangling boolean：过滤悬挂的镜像，如：dangling=true表示只显示悬挂的镜像<br />label string：根据标签过滤，如：label=version表示显示有version标签的镜像，label=version=1.0表示显示version=1.0的镜像<br />before image：显示在某个镜像之前创建的镜像，如：before=centos:5.8表示显示在centos:5.8这个镜像之前创建的镜像<br />since image：显示在某个存在之后创建的镜像，如：since=centos:5.8表示显示在centos:5.8这个镜像存在之后的镜像<br />reference string：模糊匹配，如：reference=cent*:5*, 显示名称已cent开头版本号已5开头的镜像

- --format参数用来指定镜像显示格式，语法如下。table表示使用表格的方式显示，支持\t格式
```bash
docker images --format "[table] {{COLUMN}}[{{COLUMN}}...]" [REPOSITORY[:TAG]]
```

- COLUMN的可选值如下：

.ID：显示镜像的名称列<br />.Repository：显示镜像的描述列<br />.Tag：显示镜像的热度一列<br />.Digest：显示镜像是否是官方发布一列<br />.CreatedSince：显示镜像是否是自动构建一列<br />.CreatedAt：显示镜像是否是自动构建一列<br />.Size：显示镜像是否是自动构建一列
<a name="hS7gs"></a>
#### images常用写法
显示本地所有镜像
```bash
docker images 
```
显示本地所有镜像，只显示id列并且不截断
```bash
docker images -q --no-trunc
```
显示centos镜像信息
```bash
docker images centos
```
显示列中包含cent关键字的所有镜像
```bash
docker images | grep cent
```
显示本地所有镜像，并显示摘要列
```bash
docker images --digests
```
显示在cengos:latest镜像之后创建的latest版本的所有镜像
```bash
docker images -f since=centos:latest -f reference=*:latest
```
显示所有镜像信息，只显示镜像id、摘要、创建时间3列，列之间用TAB键隔开
```bash
docker images --format "table {{.ID}}\t{{.Digest}}\t{{.CreatedAt}}"
```
显示在centos:5.11镜像之前创建的镜像，只显示镜像仓库路径、版本号、创建时间3列，列之间用TAB键隔开
```bash
docker images -f before=centos:5.11 --format "table {{.Repository}}\t{{.Tag}}\t{{.CreatedAt}}"
```
<a name="Dv2L4"></a>
### rmi：删除本地镜像
rmi命令可以删除一个或多个本地镜像，通常情况应该用rm表示删除命令，但是在doker命令中rm表示删除容器，所以用rmi表示删除镜像，其中的i是image的首字母
<a name="hLWyb"></a>
#### rmi语法
```bash
docker rmi [OPTIONS] IMAGE [IMAGE...]
```
OPTIONS：可选参数<br />IMAGE：镜像id或仓库路径名称<br />OPTIONS的常用值<br />-f：强制删除，如果镜像有对应的容器正在运行，则不允许直接删除镜像，需要强制删除<br />--no-prune：不删除该镜像的过程镜像，默认是删除的
<a name="g71CA"></a>
#### rmi常用写法
删除centos镜像
```bash
docker rmi tomcat
```
删除centos:5.11镜像
```bash
docker rmi centos:5.11
```
删除id为621ceef7494a的镜像
```bash
docker rmi 621ceef7494a
```
同时删除tomcat、centos和redis镜像
```bash
docker rmi tomcat centos redis
```
强制删除tomcat镜像，就算此时有tomcat容器正在运行，镜像也会被删除
```bash
docker rmi -f tomcat
```
<a name="ETGje"></a>
### tag：标记镜像，将其归入仓库
tag命令可以基于一个镜像，创建一个新版本的镜像并归入本地仓库，此时该镜像在仓库中存在两个版本，可以根据这两个镜像创建不同的容器
<a name="MRy1X"></a>
#### tag语法
```bash
docker tag SOURCE_IMAGE[:TAG] TARGET_IMAGE[:TAG]
```
SOURCE_IMAGE：原镜像<br />TARGET_IMAGE：新镜像<br />TAG：镜像的版本号
<a name="o1JKn"></a>
#### tag的常用写法
基于redis:latest镜像创建my-redis1.0镜像，并把新镜像归入redis-lib仓库
```bash
docker tag redis:latest redis-lib/my-redis:1.0
```
基于621ceef7494a镜像创建my-redis:test-100m镜像，并把新镜像归入redis-lib仓库
```bash
docker tag 621ceef7494a redis-lib/my-redis:test-100m
```
<a name="ucz9f"></a>
### history：查看镜像的创建历史
history命令用来查看某一个镜像的创建历史，也就是镜像的提交记录
<a name="tE44L"></a>
#### history语法
```bash
docker history [OPTIONS] IMAGE
```
OPTIONS：可选参数<br />IMAGE：镜像<br />OPTIONS的常用值

- -Hboolean：已可读的格式打印日期和大小，默认为true
- -q：只显示镜像id<br />no-trunc：输出结果不截取，正常情况下查看到的结果如果某一列太长会被截取<br />--format string：指定镜像显示的格式，格式详见下文
- --format用来指定镜像的显示的格式，语法如下。table表示使用表格的方式显示，支持\t格式
```bash
docker history --format "[table] {{COLUMN}}[{{COLUMN}}...]" IMAGE
```

- COLUMN的可选值如下：

.ID：镜像的ID<br />.CreatedSince：镜像创建的时长<br />.CreatedAt：镜像创建的时间戳<br />.CreatedBy：镜像创建使用的命令<br />.Size：镜像的大小<br />.Comment：镜像的评论
<a name="iDGGb"></a>
#### history常用写法
显示centos镜像的创建历史
```bash
docker history centos
```
显示centos镜像的创建历史，时间和大小转换为人类可读的格式
```bash
docker history -H=true centos
```
显示centos镜像的创建历史，只显示ID、创建时间戳和创建时的命令3列，列之间使用TAB键隔开
```bash
docker history --format "table {{.ID}}\t{{.CreatedAt}}\t{{.CreatedBy}}" centos
```
<a name="jOwU4"></a>
### save：将镜像打包成文件
save命令可以把一个镜像或多个镜像打包到一个文件中，需要特别注意和export命令的区分<br />save命令打包的是镜像，包含镜像的所有信息<br />exprot命令打包的是容器，只是保存容器当时的快照，历史记录和元数据信息将会丢失，详见exprot命令介绍
<a name="VeprI"></a>
#### save语法
```bash
docker save [OPTIONS] IMAGE [IMAGE...]
```
OPTIONS：可选参数<br />IMAGE：镜像<br />OPTIONS的常用值<br />-o string：指定目标文件，和linux原生命令>有相同作用
<a name="vb5LV"></a>
#### save常用写法
将centos镜像打包成my-images.tar
```bash
docker save centos > /home/my-images.tar
```
将centos镜像和redis镜像打包到my-images.tar
```bash
docker save centos redis > /home/my-images.tar
```
将centos镜像和redis镜像打包到my-images.tar
```bash
docker save -o /home/my-images.tar centos redis
```
<a name="StZaR"></a>
### load：从指定文件中加载镜像
load命令可以从指定文件中加载镜像，该文件需要是save命令保存的文件
<a name="NghRc"></a>
#### load语法
```bash
docker load [OPTIONS]
```
OPTIONS：可选参数<br />OPTIONS的常用值<br />-i string：指定文件的路径<br />-q：安静模式输出
<a name="MMSfe"></a>
#### load常用写法
从my-images.tar文件中加载镜像
```bash
docker load < /home/my-images.tar
```
从my-images.tar文件中加载镜像
```bash
docker load -i /home/my-images.tar
```
使用安静模式从my-images.tar文件中加载镜像
```bash
docker load -i /home/my-images.tar -q
```
<a name="HWplr"></a>
## 操作容器的命令
<a name="ChK0N"></a>
### run：创建一个容器并运行
run命令可以创建一个容器并运行，如果创建容器的镜像不存在则会从远程镜像仓库下载<br />运行容器的同时还能给容器发送一个命令
<a name="oCenK"></a>
#### run语法
```bash
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```
OPTIONS：可选参数<br />IMAGE：镜像<br />COMMAND：需要运行的命令<br />ARG：命令的参数<br />OPTIONS的常用值<br />由于run命令的OPTIONS的可选值比较多，这里只列出使用频率最高的一些可选值。使用`docker run --help`可以查看run命令的所有可用参数<br />-i：以交互模式运行，通常与`-t`一起使用<br />-t：为容器分配一个伪终端，通常与`-i`一起使用<br />-d：后台模式运行容器，并返回容器id<br />-plist：指定端口映射，格式为宿主机端口:容器端口<br />-P：随机分配端口映射<br />--namestring：给容器指定一个名称<br />-mbytes：限制容器可以使用的内存大小，单位可选b、k、m、g<br />-vlist：把宿主机的磁盘路径挂载到容器的某个路径<br />--volumes-fromlist：绑定别的容器某个路径到此容器的某个路径<br />-w：指定容器的工作目录，默认是根目录<br />--rm：当容器停止运行时自动删除<br />--hostnamestring：指定容器的主机名<br />run常用写法创建一个centos容器，并运行
```bash
docker run centos
```
创建一个centos容器，并以交互模式运行
```bash
docker run -it centos
```
创建一个centos容器，并后台模式运行
```bash
docker run -d centos
```
创建一个centos容器，重命名为my-centos，并以交互模式运行，并在容器中运行bash命令
```bash
docker run -it --name my-centos centos /bin/bash
```
创建一个spring-boot容器并以交互模式运行，容器重命名为my-boot，并把主机的80端口映射到容器的8080端口，此时访问主机ip+80端口即可访问容器中的sping-boot项目
```bash
docker run -it --name my-boot -p 80:8080 spring-boot
```
创建一个spring-boot容器并以交互模式运行，容器重命名为my-boot，并把主机/logs/my-boot/的目录绑定到容器的/logs目录，此时my-boot项目的日志可以在主机的/logs/my-boot目录中查看
```bash
docker run -it --name my-boot -v /logs/my-boot/:/logs/ spring-boot
```
创建一个spring-boot容器并以交互模式运行，容器重命名为my-boot；把主机的80端口映射到容器的8080端口；把主机/logs/my-boot/的路径绑定到容器的/logs目录；给容器分配最大500M的内存；指定spring-boot的配置文件为test
```bash
docker run -it --name my-boot -p 80:8080 -v /logs/my-boot/:/logs/ -m 500M spring-boot --spring.profiles.active=test
```
<a name="NahQk"></a>
### start：启动容器
start命令可以启动一个或多个已经停止的容器
<a name="gp5iF"></a>
#### start语法
```bash
docker start [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -a：将容器的标准输出或标准错误附加到终端
- -i：为容器附加一个标准输入终端
<a name="EfUYr"></a>
#### start常用写法
启动已经停止的tomcat容器
```bash
docker start tomcat
```
启动已经停止的tomcat和centos容器
```bash
docker start tomcat centos
```
启动已经停止的my-spring-boot容器，并输出日志
```bash
docker start -a my-spring-boot
```
启动已经停止centos容器，并附加一个输入终端
```bash
docker start -i centos
```
<a name="dw46z"></a>
### restart：重启容器
restart可以对一个或多个容器进行重启。如果容器是未启动的则会启动，如果是正在运行中的，则会重启
<a name="KUPUO"></a>
#### restart语法
```bash
docker restart [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值<br />-t int：在重启之前等待几秒，默认10秒
<a name="vXo1M"></a>
#### restart常用写法
重启centos容器
```bash
docker restart centos
```
20秒之后重启centos和tomcat容器
```bash
docker restart -t 20 centos tomcat
```
<a name="e8mGI"></a>
### stop：停止容器
stop命令可以停止一个或多个正在运行的容器<br />kill命令也可以用来停止容器<br />不同的是stop命令允许容器在停止之前有一定的时间来进行额外操作，如释放链接、关闭请求等<br />kill命令则会直接强制杀死容器
<a name="rvi0H"></a>
#### stop语法
```bash
docker stop [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -t int：等待n秒后如果还没停止，直接杀死，默认10秒
<a name="EtiZd"></a>
#### stop常用写法
停止tomcat容器
```bash
docker stop tomcat
```
停止tomcat和centos容器
```bash
docker stop tomcat centos
```
停止tomcat容器，如果5秒内还未停止则直接杀死
```bash
docker stop -t 5 tomcat
```
<a name="XoRrW"></a>
### restart：重启容器
restart命令可以重启一个或多个容器，不管容器是运行或停止
<a name="SzQo0"></a>
#### restart语法
```bash
docker restart [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值<br />-t int：如果重启的容器正在运行，等待n秒还没停止，直接杀死然后重启，默认10秒
<a name="hMEUr"></a>
#### restart常用写法
重启tomcat容器
```bash
docker restart tomcat
```
重启tomcat和centos容器
```bash
docker restart tomcat centos
```
重启正在运行的tomcat容器，如果5秒内还未停止则直接杀死然后重启
```bash
docker restart -t 5 tomcat
```
<a name="ceVEk"></a>
### kill：杀死容器
kill命令可以杀死一个或多个正在运行的容器
<a name="IZZYv"></a>
#### kill语法
```bash
docker kill [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值<br />-s string：给容器发送一个信号，信号编号和linux原生命令kill的信号编号一致，默认值9，下文列出一些常用值

- -s参数信号编号常用值

1：杀死并重新加载，也可用HUP表示<br />9：强制杀死，也可用KILL表示，默认值<br />15：正常停止，也可用TERM表示
<a name="IquOL"></a>
#### kill常用写法
杀死tomcat容器
```bash
docker kill tomcat
```
强制杀死tomcat容器
```bash
docker kill -s 9 tomcat
```
强制杀死tomcat容器
```bash
docker kill -s KILL tomcat
```
杀死tomcat和centos容器
```bash
docker kill tomcat centos
```
<a name="fTRex"></a>
### rm：删除容器
rm命令可以删除一个或多个容器<br />如果容器正在运行，则需要通过-f参数强制删除
<a name="DKXdh"></a>
#### rm语法
```bash
docker rm [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -f：强制删除，即使容器正在运行也可以删除
- -l：删除容器之间的网络关系，而不是容器本身
- -v：删除容器和它挂载的卷
<a name="PHx1C"></a>
#### rm常用写法
删除centos容器
```bash
docker rm centos
```
强制删除centos容器，即使容器正在运行也会被删除
```bash
docker rm -f centos
```
删除centos容器，并删除它挂载的卷
```bash
docker rm -f centos
```
删除所有已经停止的容器
```bash
docker rm $(docker ps -a -q)
```
移除容器my-nginx对容器my-db的连接，连接名db
```bash
docker rm -l db 
```
<a name="pl2ze"></a>
### pause：暂停容器
pause命令可以暂停一个或多个正在运行的容器
<a name="bZc5M"></a>
#### pause语法
```bash
docker pause CONTAINER [CONTAINER...]
```
CONTAINER：容器
<a name="dXSXd"></a>
#### pause常用写法
暂停正在运行的centos容器
```bash
docker pause centos
```
暂停正在运行的centos和tomcat容器
```bash
docker pause centos tomcat
```
<a name="sBQEA"></a>
### unpause：取消暂停容器
unpause命令可以对一个或多个暂停的容器取消暂停
<a name="rkFnN"></a>
#### pause语法
```bash
docker unpause CONTAINER [CONTAINER...]
```
CONTAINER：容器
<a name="K0gr6"></a>
#### unpause常用写法
取消暂停的centos容器
```bash
docker unpause centos
```
取消暂停centos和tomcat容器
```bash
docker unpause centos tomcat
```
<a name="GLGpu"></a>
### create：创建一个容器
create命令可以创建一个容器，但不运行它，在需要的时候可以使用start命令启动<br />和run命令的用法几乎一致，都会创建一个容器，如果容器依赖的镜像不存在都会从远程仓库拉取<br />run命令创建容器后会运行容器<br />create命令只是创建容器，不运行
<a name="b14ND"></a>
#### create语法
```bash
docker create [OPTIONS] IMAGE [COMMAND] [ARG...]
```
OPTIONS：可选参数<br />IMAGE：镜像<br />COMMAND：需要运行的命令<br />ARG：命令的参数<br />OPTIONS的常用值<br />create命令和run命令的可选参数一样<br />由于可选参数比较多，这里只列出使用频率最高的一些可选值。使用docker create --help可以查看create命令的所有可用参数

- -i：以交互模式运行，通常与-t一起使用
- -t：为容器分配一个伪终端，通常与-i一起使用
- -d：后台模式运行容器，并返回容器id
- -plist：指定端口映射，格式为宿主机端口:容器端口<br />-P：随机分配端口映射
- --namestring：给容器指定一个名称
- -mbytes：限制容器可以使用的内存大小
- -vlist：把宿主机的磁盘路径挂载到容器的某个路径
- --volumes-fromlist：绑定别的容器某个路径到此容器的某个路径
- -w：指定容器的工作目录，默认是根目录
- --rm：当容器停止运行时自动删除
- --hostnamestring：指定容器的主机名
<a name="WE8ju"></a>
#### create常用写法
创建一个centos容器
```bash
docker create centos
```
创建一个centos容器，start启动时以交互模式运行
```bash
docker create -it centos
```
创建一个centos容器，start启动时后台模式运行
```bash
docker create -d centos
```
创建一个centos容器，重命名为my-centos，start时以交互模式运行，并在容器中运行bash命令
```bash
docker create -it --name my-centos centos /bin/bash
```
创建一个spring-boot容器，重命名为my-boot，并把主机的80端口映射到容器的8080端口，start时以交互模式运行，此时访问主机ip+80端口即可访问容器中的sping-boot项目
```bash
docker create -it --name my-boot -p 80:8080 spring-boot
```
创建一个spring-boot容器，容器重命名为my-boot，并把主机/logs/my-boot/的目录绑定到容器的/logs目录，start时以交互模式运行，此时my-boot项目的日志可以在主机的/logs/my-boot目录中查看
```bash
docker create -it --name my-boot -v /logs/my-boot/:/logs/ spring-boot
```
创建一个spring-boot容器，容器重命名为my-boot；把主机的80端口映射到容器的8080端口；把主机/logs/my-boot/的路径绑定到容器的/logs目录；给容器分配最大500M的内存；指定spring-boot的配置文件为test；start时以交互模式运行
```bash
docker create -it --name my-boot -p 80:8080 -v /logs/my-boot/:/logs/ -m 500M spring-boot --spring.profiles.active=test
```
<a name="x6nFu"></a>
### exec：在容器中执行命令
exce命令可以在一个运行中的容器中执行一个命令
<a name="Cp2DQ"></a>
#### exec语法
```bash
 docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />COMMAND：要执行的命令<br />ARG：命令的参数<br />OPTIONS的常用值

- -d：命令在后台运行
- -i：保持标准输入，通常和-t一起使用
- -t：分配一个伪终端，通常和-i一起使用
- -wstring：指定容器的路径
<a name="SqmVH"></a>
#### exec常用写法
在centos容器中运行pwd命令
```bash
docker exec centos pwd
```
为centos容器分配一个输入终端
```bash
docker exec -it centos /bin/bash
```
在centos镜像中的bin目录执行ls命令
```bash
docker exec -w /bin centos ls
```
<a name="TPZ1r"></a>
### ps：查看容器列表
ps命令可以列出所有容器的列表，查看容器的基本信息。不加任何参数的情况下，默认只展示正在运行的容器<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623857575860-ea307662-d829-490d-9456-2ee59c15791f.webp#averageHue=%23143462&height=119&id=u54e2639d&originHeight=119&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1080)

- 列含义如下
| CONTAINER ID | IMAGE | COMMAND | CREATED | PORTS | NAMES |
| --- | --- | --- | --- | --- | --- |
| 容器id | 对应的镜像 | 容器启动时运行的命令 | 创建时间 | 绑定的的端口 | 容器名称 |

<a name="GMOOC"></a>
#### ps语法
```bash
docker ps [OPTIONS]
```
OPTIONS：可选参数<br />OPTIONS的常用值

- -a：显示所有容器，默认只显示正在运行的
- -ffilter：根据条件过滤容器，过滤条件详见下文
- -nint：显示最后创建的n个容器，包含所有状态
- -l：显示最新创建的容器，包含所有状态
- -q：只显示容器id
- -s：显示容器大小，默认不显示该列
- --no-trunc：显示内容不截断，默认情况下显示的容器是截断后的信息
- -f参数表示根据条件过滤搜索出来的镜像，语法如下
```bash
docker ps -f KEY=VALUE
```

- KEY的可选值如下

id：根据容器id过滤<br />name：查看容器名称中包含给定字段的容器<br />exited：根据容器退出的错误码进行过滤<br />status：根据容器的状态进行过滤，状态可选值有：created、paused、exited、dead、running、restarting、removing<br />before：只显示在某个容器之前创建的容器<br />since：只显示在某个容器之后创建的容器<br />volume：过滤绑定了某个目录的容器，只针对运行中的容器<br />publish：根据宿主机端口过滤，只针对运行中的容器<br />expose：根据容器端口过滤，只针对运行中的容器
<a name="t7S3o"></a>
#### ps常用写法
查看运行中的容器
```bash
docker ps
```
查看所有容器
```bash
docker ps -a
```
查看所有容器，并显示容器大小
```bash
docker ps -a -s
```
查看所有容器，显示内容不截断
```bash
docker ps -a --no-trunc
```
查看容器名称中包含cent的容器
```bash
docker ps -f name=cent
```
查看状态是created的容器
```bash
docker ps -f status=created
```
查看在centos之前创建的容器
```bash
docker ps -f before=centos
```
查看绑定了宿主机80端口并且正在运行的容器
```bash
docker ps -f publish=80
```
<a name="AGU94"></a>
### inspect：获取容器或镜像的元数据
inspect命令可以获取一个或多个容器或者镜像的元数据信息<br />元数据信息可以理解为容器或者镜像的详情，它比ps命令显示的内容要详细的多。比如说端口映射、挂载目录等，显示格式为json类型
<a name="gwTlX"></a>
#### inspect语法
```bash
docker inspect [OPTIONS] CONTAINER|IMAGE [CONTAINER|IMAGE...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />IMAGE：镜像<br />OPTIONS的常用值

- -fstring：格式化输出结果，inspect默认显示整个文件的详情，-f参数可以指定只显示某些属性
- --s：只对容器有效，显示容器的配置文件行数和大小，显示的结果中会多出SizeRw、SizeRootFs两个参数
- --typestring：指定要inspect的类型，container表示容器，image表示镜像，默认是容器。比如我有一个tomcat镜像，同时有一个名称为tomcat的容器，就可以用--type参数来指定要inspect是tomcat容器还是tomcat镜像
<a name="nUwKX"></a>
#### inspect常用写法
查看tomcat容器的元数据信息
```bash
docker inspect tomcat
```
查看tomcat镜像的元数据信息
```bash
docker inspect --type=image tomcat
```
查看tomcat容器的ip地址
```bash
docker inspect --format='{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' tomcat
```
查看tomcat容器的ip地址
```bash
docker inspect tomcat | grep IPAddress
```
查看tomcat容器的端口映射
```bash
docker inspect -f "{{.HostConfig.PortBindings}}" tomcat
```
查看tomcat容器的挂载目录
```bash
docker inspect -f "{{.HostConfig.Binds}}" tomcat
```
<a name="OD6Vt"></a>
### stats：监控容器的资源使用情况
stats命令可以可以监控容器的资源使用情况，如cpu使用情况、内存使用情况等。每秒刷新一次，直到使用ctrl+c退出<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1623857576874-ba2e1851-0c90-493e-84fa-e1a220dd3faa.webp#averageHue=%23143563&height=72&id=uc080ddf2&originHeight=72&originWidth=1056&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=1056)

- 列含义如下
| CONTAINER ID | NAME | CPU % | MEM USAGE/LIMIT | MEM % | NET I/O | BLOCK I/O | PIDS |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 容器id | 容器名称 | cpu使用百分比 | 使用内容大小/最大可用内存 | 内存使用百分比 | 网络IO | 磁盘IO | 容器内线程或进程的数量 |

<a name="V9b9s"></a>
#### stats语法
```bash
docker stats [OPTIONS] [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -afilter：显示所有容器的资源使用情况，默认只显示正在运行的
- --formatstring：格式化输出结果
- --no-stream：不间隔刷新，只显示第一次统计结果
- --no-trunc：不截断显示信息，默认情况下有些字段只显示简略信息，如容器id
<a name="drUmh"></a>
#### stats常用写法
监控所有正在运行的容器的资源使用情况
```bash
docker stats
```
监控所有容器的资源使用情况，包含未启动的容器
```bash
docker stats -a
```
只监控centos容器的资源使用情况
```bash
docker stats centos
```
监控centos容器的资源使用情况，显示结果不刷新
```bash
docker stats --no-stream centos
```
<a name="IlwA2"></a>
### top：查看容器中运行的进程信息
top可以查看容器的进程信息，docker exec CONTAINER ps也可以查看容器的进程。<br />不同的是，前者查看的是容器运行在宿主机的进程id。后者查看的是容器内的进程id
<a name="sXyLZ"></a>
#### top语法
```bash
docker top CONTAINER [ps OPTIONS]
```
CONTAINER：容器<br />OPTIONS：ps命令的可选参数
<a name="KQkXA"></a>
#### top常用写法
查看centos镜像的宿主机进程id
```bash
docker top centos
```
<a name="dWyCE"></a>
### rename：重命名容器
rename可以对容器进行重命名，在容器run时如果没有使用--name参数指定容器名称，可以使用rename进行命名
<a name="GYPag"></a>
#### rename语法
```bash
docker rename CONTAINER NEW_NAME
```
<a name="efOAg"></a>
#### rename常用写法
将centos容器重命名为my-centos
```bash
docker rename centos my-centos
```
<a name="EV5VK"></a>
### attach：连接到容器内
attach可以连接到容器内，这个容器必须是正在运行的容器，不是运行状态时，会报错<br />当使用ctrl+c或exit等命令退出容器时，会导致容器停止运行。所以，不建议在生产环境使用该命令。生产环境可以使用exec命令进入容器
<a name="Vdfho"></a>
#### attach语法
```bash
docker attach [OPTIONS] CONTAINER
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- --sig-proxy=false boolean：默认true，为false时可以防止容器遇到ctrl+c退出信号时停止运行
<a name="MB87X"></a>
#### attach常用写法
进入正在运行的centos镜像内
```bash
docker attach centos
```
<a name="RjAj3"></a>
### update：更新一个或多个容器的配置
update可以对容器的配置进行更新
<a name="mSsmh"></a>
#### update语法
```bash
docker update [OPTIONS] CONTAINER [CONTAINER...]
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -m bytes：指定容器的内存大小，单位可选b、k、m、g
- --memory-swap bytes：--cpu demecial：cpu资源，如1.5表示可以使用宿主机的1.5个cpu资源
- --cpuset-cpus string：容器可以使用宿主机的cpu内核编号，0-3表示4个内核都可以使用，1,3表示只能使用1和3号内核
- --restart string：指定容器的退出的重启策略。no：不重启；on-failure：容器非正常退出时重启；on-failure:3：非正常退出时重启3次；alaways：总是重启；unless-stopped：在容器退出时总是重启容器，但是不考虑在Docker守护进程启动时就已经停止了的容器
- --pids-limit int：限制容器进程或线程的数量，默认-1表示不限制
<a name="QixFj"></a>
#### update常用写法
更新centos镜像的内存为2G
```bash
docker update --memory-swap -1 -m 2g centos
```
更新容器的重启策略
```bash
docker update --restart on-failure:3 centos
```
更新tomcat容器的最大线程数为2000
```bash
docker update --pids-limit 2000 tomcat
```
<a name="JA2oN"></a>
### logs：查看容器的日志
<a name="fRV2c"></a>
#### logs语法
```bash
docker logs [OPTIONS] CONTAINER
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -f：跟踪日志的实时输出
- --until string：查看某个时间点之前的日志，格式：2021-06-03T19:30:20Z。或使用相对时间10m，表示10分钟之前
- --since string：查看某个时间点之后的日志，格式：2021-06-03T19:30:20Z。使用相对时间10m，表示10分钟之内
- -n int：查看最后几行日志，默认显示全部
- -t：日志中显示时间戳
<a name="eIWnl"></a>
#### logs常用写法
查看tomcat最后10行日志
```bash
docker logs -n 10 tomcat
```
查看tomcat最后10行日志，并实时监控日志输出
```bash
docker logs -n 10 -f  tomcat
```
查看最近10分钟的日志
```bash
docker logs --since 10m tomcat
```
查看6月3号9点到10点之间的日志
```bash
docker logs --since 2021-06-03T9:00:00  --until 2021-06-03T10:00:00 tomcat
```
<a name="gAY1q"></a>
### wait：阻塞容器，直到容器退出并打印它的退出代码
wait命令可以阻塞一个或多个容器直到容器退出并打印出他们的退出代码
<a name="AAi68"></a>
#### wait语法
```bash
docker wait CONTAINER [CONTAINER...]
```
CONTAINER：容器
<a name="WdKIF"></a>
#### wait常用写法
阻塞centos容器，直到它退出并打印退出状态码
```bash
docker wait centos
```
此时新打开一个终端，将centos容器stop掉，切换到wait的终端就可以看到打出一个状态码
<a name="uqxbe"></a>
### port：列出端口的映射关系
<a name="BZ4kM"></a>
#### port语法
```bash
docker port CONTAINER [PRIVATE_PORT[/PROTO]]
```
CONTAINER：容器<br />PRIVATE_PORT：容器端口<br />PROTO：端口使用的协议
<a name="ELpjk"></a>
#### port常用写法
查看my-boot容器的端口映射
```bash
docker port my-boot
```
查看my-boot容器的8080端口映射的宿主机端口
```bash
docker port my-boot 8080
```
查看my-boot容器使用tcp协议的8080端口映射的宿主机端口
```bash
docker port my-boot 8080/tcp
```
<a name="kLnHA"></a>
### export：将容器打包成一个文件
export命令可以将容器打包到一个文件中，它和save命令比较容易混淆<br />export和save的不同之处在于：export打包的是容器，save打包的是镜像<br />export打包的是容器当时的快照，至于容器的历史记录和元数据信息都会丢失。还有，export的文件在被import成一个镜像时，可以重新指定镜像的名称和版本号
<a name="fpkfw"></a>
#### export语法
```bash
docker export [OPTIONS] CONTAINER
```
OPTIONS：可选参数<br />CONTAINER：容器<br />OPTIONS的常用值

- -o string：指定打包文件
<a name="rpcQ4"></a>
#### export常用写法
将my-boot容器打包到my-boot.tar文件
```bash
docker export -o /tmp/my-boot.tar my-boot
```
<a name="fdC5K"></a>
### import：从本地文件或远程文件导入镜像到本地仓库
import可以从本地文件或远程文件中导入镜像到本地仓库<br />如果是从文件中导入，这个文件需要是export命令导出的文件
<a name="tS3fa"></a>
#### import语法
```bash
docker import [OPTIONS] file|URL|- [REPOSITORY[:TAG]]
```
OPTIONS：可选参数<br />file：文件地址<br />URL：URL地址<br />-：从标准输入终端导入，通常和linux中的cat命令一起使用<br />REPOSITORY：本地镜像仓库地址<br />TAG：镜像版本号<br />OPTIONS的常用值

- -m string：添加描述信息
- -c list：对创建的容器使用dokerfile指令
<a name="YIAm9"></a>
#### import常用写法
从my-boot.tar文件创建镜像
```bash
cat /tmp/my-boot.tar | docker import -
```
从my-boot.tar文件创建镜像
```bash
docker import /tmp/my-boot.tar
```
从my-boot.tar文件创建镜像，并指定镜像名称为my-boot-test、版本号为1.0
```bash
docker import /tmp/my-boot.tar my-boot-test:1.0
```
从my-boot.tar文件创建镜像，备注信息为测试，并指定镜像名称为my-boot-test、版本号为1.0
```bash
docker import --message '测试' /tmp/my-boot.tar my-boot-test:1.0
```
从远程服务器的my-boot.tar文件创建镜像
```bash
docker import http://192.168.100.1:8080/images/my-boot.tar
```
