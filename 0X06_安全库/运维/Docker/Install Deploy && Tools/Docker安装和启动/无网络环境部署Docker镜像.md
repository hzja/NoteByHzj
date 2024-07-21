Docker
<a name="URNr2"></a>
## 一、简介
无网络环境，部署 Docker 镜像，这通常适用于一些部署环境是脱离网络的公司，或者公司内部有着严格的网络安全要求。且还是 Docker 部署的程序，这个时候怎么办，来看看无网络环境如何部署 Docker 镜像。
<a name="rbi7z"></a>
## 二、准备
这里准备两台服务器，Server A 是原镜像服务器，Server B 需要部署 Applicatio 的服务器<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262357921-f9e40d98-71bd-4efc-8b1e-92f769d5023c.png#averageHue=%23e2c08f&clientId=u53b15118-0797-4&from=paste&id=u2ee6954a&originHeight=120&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u324fa8f1-1985-425f-9351-91624b60994&title=)
<a name="UzNbB"></a>
## 三、Server A
这里使用 Nginx 模拟应用程序，在工作场景中也是对镜像进行操作，所以无差异
<a name="GlJdE"></a>
### 3.1 原镜像
在 Server A 使用 `docker pull nginx:1.21.6` 拉取镜像到本地，然后可以使用 `docker images` 进行查看<br />![2022-08-23-21-47-35.909575500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262550313-2d1db0d1-76fa-4e99-8ce0-16370f568803.png#averageHue=%23143a5e&clientId=u53b15118-0797-4&from=ui&id=ucdf9bc63&originHeight=285&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=925253&status=done&style=shadow&taskId=u1a082e6c-c8a1-496d-9c6e-337634255d7&title=)
<a name="pDX1q"></a>
### 3.2 导出镜像
导出镜像，用到的是 `docker save` 命令，将指定镜像保存成 tar 文件，语法如下
```bash
docker save [OPTIONS] IMAGE [IMAGE...]
```
案例：这里以 nginx:1.21.6 镜像为例，将镜像 nginx:1.21.6 导出为 nginx-1.21.6.tar 文件
```bash
docker save -o  nginx-1.21.6.tar nginx:1.21.6
```
![2022-08-23-21-47-35.932138100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262550175-44dd9517-82be-49d5-8bac-c59db0b19881.png#averageHue=%2387bf99&clientId=u53b15118-0797-4&from=ui&id=C7ioJ&originHeight=163&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=529215&status=done&style=shadow&taskId=uf0067eb2-853e-45ba-9190-20a44c712a9&title=)
<a name="IWDi0"></a>
## 四、Server B
<a name="Y4S1H"></a>
### 4.1 tar 镜像文件
将 Server A 上的 nginx-1.21.6.tar 放到 Server B 上（方式根据实际来），这里放置的是 /var/local/ 目录下。如下：<br />![2022-08-23-21-47-35.947962300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262550189-6ff94f5c-9d3b-4258-829e-4c01ee18e1bb.png#averageHue=%23204467&clientId=u53b15118-0797-4&from=ui&id=CQ38R&originHeight=169&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=548685&status=done&style=shadow&taskId=ufa3910c7-db80-444f-8189-aa74858fb1d&title=)
<a name="l8geR"></a>
### 4.2 导入镜像
导入镜像，用到的是 `docker load` 命令，将指定镜像的 tar 文件，导入到 docker 中，语法如下
```bash
docker load [OPTIONS]
```
案例：这里导入上面生成的 nginx-1.21.6.tar 镜像文件，然后可以使用 docker images 看到导入的 nginx:1.21.6 镜像
```bash
docker load -i  nginx-1.21.6.tar
```
![2022-08-23-21-47-35.960663500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262575523-7df5d597-d7b3-49a6-89ec-0e996ab7258e.png#averageHue=%23173c60&clientId=u53b15118-0797-4&from=ui&id=uff56ae7b&originHeight=296&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=960969&status=done&style=shadow&taskId=u9353496c-7bb0-47f6-9b26-aabd79dfe9a&title=)<br />在导入之前可以，使用 `docker images` 命令，看下现有镜像<br />![2022-08-23-21-47-35.976375000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262575355-880766b0-26a0-4986-81dd-b2be2123dada.png#averageHue=%232a4c6d&clientId=u53b15118-0797-4&from=ui&id=UlkT4&originHeight=143&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=464289&status=done&style=shadow&taskId=uabb6a1f4-29c3-4c44-97c3-a09009862be&title=)
<a name="Ay6vJ"></a>
### 4.3 启动容器
导入镜像后，可以启动一下，看下是否是成功的，执行下面命令即可启动 nginx
```bash
docker run --name nginx-test -p 10000:80 -d nginx:1.21.6
```
![2022-08-23-21-47-35.988378400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262575273-723d50a1-91af-4726-bdf0-f888cd22824e.png#averageHue=%23052e55&clientId=u53b15118-0797-4&from=ui&id=IZTeb&originHeight=63&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=204585&status=done&style=shadow&taskId=u55aa6f98-3037-4436-a6a7-5038af48603&title=)访问，http://192.168.88.131:10000/<br />![2022-08-23-21-47-35.999568400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661262607212-2159107f-c076-46f7-b088-f69923cce2bd.png#averageHue=%23ebebea&clientId=u53b15118-0797-4&from=ui&id=ub04d3ce0&originHeight=172&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=558437&status=done&style=shadow&taskId=uc673f07d-b5e4-4486-8bbc-596af80f4be&title=)
