Dubbo
<a name="4vO9m"></a>
# 1、在Github下载dubbo-admin项目
[dubbo-admin在Github官网的地址](https://github.com/apache/dubbo-admin)<br />[https://github.com/apache/dubbo-admin](https://github.com/apache/dubbo-admin)
<a name="ZCqk4"></a>
# 2、启动dubbo-admin项目
由于dubbo-admin项目版本更新迭代，此文所述使用的前后端分离版本
<a name="Yqr8H"></a>
## A.解压下载的项目
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583760489643-edda1733-5818-4ad0-888c-77a12ca425d3.png#align=left&display=inline&height=434&originHeight=434&originWidth=645&size=53522&status=done&style=none&width=645)
<a name="qSIwg"></a>
## B.启动服务端程序dubbo-admin-server
**_前提-本地配置好Maven的环境变量_**<br />**_未安装的可参考_[https://www.yuque.com/fcant/java/maven-install-package](https://www.yuque.com/fcant/java/maven-install-package)<br />[1.Maven的安装和环境变量配置及使用-Window](https://www.yuque.com/fcant/java/maven-install-package?view=doc_embed)

<a name="2SKbD"></a>
### ①、进入dubbo-admin-server\src\main\resources目录下，修改配置文件application.properties设置后端项目运行的端口号
因为Zookeeper服务端启动时默认绑定8080端口，如果不修改指定端口，会报端口冲突错误<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583764460166-58b23aa9-c9b5-4166-8745-44ccb84f1d4a.png#align=left&display=inline&height=629&originHeight=629&originWidth=802&size=71295&status=done&style=none&width=802)
<a name="Nym5G"></a>
### ②、进入dubbo-admin-server目录，在Path输入cmd弹出命令行程序窗口，输入如下命令进行项目打包运行

```powershell
mvn package -Dmaven.test.skip=true
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583760893890-bb0ce786-a234-4c89-9efa-b13db16b4e01.png#align=left&display=inline&height=540&originHeight=540&originWidth=1080&size=90135&status=done&style=none&width=1080)![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583760912283-190ef284-6779-47bc-a623-da79f4775c65.png#align=left&display=inline&height=540&originHeight=540&originWidth=1080&size=109008&status=done&style=none&width=1080)
<a name="djcCg"></a>
### ③、进入打成jar包的目录，通过 `java -jar` 命令启动

```powershell
D:\Dome\dubbo-admin-develop\dubbo-admin-server>cd target

D:\Dome\dubbo-admin-develop\dubbo-admin-server\target>java -jar dubbo-admin-server-0.1.jar
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583765342700-3895cd8d-3209-42f0-bd78-f9c80e4e2008.png#align=left&display=inline&height=705&originHeight=705&originWidth=1349&size=172088&status=done&style=none&width=1349)
<a name="1WSR1"></a>
### ④、在7001端口启动成功![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583765363607-e62267ea-08ba-4297-9b08-3e068dbe37ef.png#align=left&display=inline&height=705&originHeight=705&originWidth=1349&size=170771&status=done&style=none&width=1349)
<a name="GorAc"></a>
### ⑤、访问后端Swagger
[http://localhost:7001/swagger-ui.html](http://localhost:7001/swagger-ui.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583766242326-9c9f924d-8be5-4fa2-bf56-f869d1ba997c.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=121860&status=done&style=none&width=1366)
<a name="bNxHH"></a>
## C.启动前端项目dubbo-admin-ui
**_前提-本地配置好Node的环境变量_**<br />**_未安装可参考_**<br />[https://www.yuque.com/fcant/web/node-install-win](https://www.yuque.com/fcant/web/node-install-win)<br />[1.Node环境安装-Win](https://www.yuque.com/fcant/web/node-install-win?view=doc_embed)
<a name="N01Xz"></a>
### ①、因为前面修改了后台默认访问端口，这里也要修改前端配置的同源访问端口
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583766625943-e82e149f-bcb9-44d2-805f-cd89b8a8377a.png#align=left&display=inline&height=631&originHeight=631&originWidth=1341&size=92799&status=done&style=none&width=1341)
<a name="W7GSW"></a>
### ②、使用 `cnpm i` 命令下载需要的包
```powershell
cnpm i
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583761676849-ab2baefe-be0f-4769-86d5-c40a536f1918.png#align=left&display=inline&height=540&originHeight=540&originWidth=1080&size=117028&status=done&style=none&width=1080)
<a name="NskIg"></a>
### ③、使用 `cnpm run dev` 命令启动启动页面程序

```powershell
cnpm run dev
```

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583761830607-3c59b2d0-f842-414d-95cc-f892cdd7cd8f.png#align=left&display=inline&height=231&originHeight=231&originWidth=1080&size=29972&status=done&style=none&width=1080)
<a name="GAYQF"></a>
### ④、在浏览器输入URL访问项目成功启动
**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583761904912-96c9d809-2e97-49b5-9b55-e7c75334a993.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=97840&status=done&style=none&width=1366)**
<a name="gLp7V"></a>
### ⑤、点击服务查询进行登录，在后台默认配置文件可知用户名和密码皆为root
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1583766864003-76f921b1-821d-40a8-96e3-b112892dbc0b.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=108755&status=done&style=none&width=1366)
