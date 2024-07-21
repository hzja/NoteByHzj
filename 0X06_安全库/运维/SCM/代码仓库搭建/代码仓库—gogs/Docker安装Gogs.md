Docker Gogs<br />在选择本地 Git 服务时，通常会选择直接安装 Gitlab，但是其安装复杂，占用资源多等缺点。Gogs是一款轻量级的、易于设置、跨平台的 Git 托管服务，不逊色于 Gitlab 和 GitHub。<br />Gogs 是一款类似GitHub的开源文件/代码管理系统（基于Git），Gogs 的目标是打造一个最简单、最快速和最轻松的方式搭建自助 Git 服务。使用 Go 语言开发使得 Gogs 能够通过独立的二进制分发，并且支持 Go 语言支持的 所有平台，包括 Linux、Mac OS X、Windows 以及 ARM 平台。
<a name="EwAh8"></a>
### 拉取Gogs镜像
```bash
docker pull gogs/gogs
```
<a name="hKXzL"></a>
### 启动Gogs容器
```bash
docker run --name=gogs -d  -p 10022:22 -p 10080:3000 -v /var/gogs:/data gogs/gogs
```
参数说明:

- -d: 后台方式运行容器<br />
- –name: 指定容器名称<br />
- -v: 数据卷挂载, 用于将容器和数据分离<br />
- -p: 端口映射, 将容器的22端口映射到宿主机的10022端口, 将容器的3000端口映射到宿主机的10080端口，22和3000端口映射在后面启动配置时需要用到，所以需要映射两个端口<br />

打开浏览器，进行 gogs 配置，建议使用 MySQL 数据库，http://宿主机ip:10080<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1611715521657-ae445b1f-906f-4f43-aef8-28ea03c03afc.png#align=left&display=inline&height=602&originHeight=1805&originWidth=3798&size=324628&status=done&style=shadow&width=1266)
<a name="4jPAq"></a>
### 模板修改
如果觉得原生的首页模板不够漂亮，也可以自定义模板，结构如下：
```
/data
|-- git
|   |-- gogs-repositories
|-- ssh
|   |-- # ssh public/private keys for Gogs
|-- gogs
    |-- conf
    |-- data
    |-- log
```
创建/data/gogs/templates文件夹,在里面创建home.tmpl并将github项目中gogs/templates/home.tmpl的内容拷贝进去。根据需求修改这个home.tmpl，然后重启容器(docker restart 容器名称)。创建/data/gogs/public文件夹,在里面创建css,javascript或者图片等，不需要重启容器。可以通过保存自定义图标到 /data/gogs/public/img/favicon.png 实现站点图标的重载。静态文件的路径要在/data/gogs/templates中的模板中有定义, 其中{{AppSubURL}}/img/my-hero.png对应的就是/data/gogs/public/img/my-hero.png。
