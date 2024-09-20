Docker Nexus
<a name="kCRy9"></a>
### 1、查找镜像
```bash
docker search nexus
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610611475165-7ff965db-d02a-4db1-9c97-1ce6012f7d1d.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2249805&status=done&style=none&width=1107.6666666666667)
<a name="k6Vqd"></a>
### 2、拉取镜像
```bash
docker pull sonatype/nexus3
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610611511562-a897bfbe-19bb-48be-9d36-16ee29e2f2f5.png#align=left&display=inline&height=215&originHeight=646&originWidth=3323&size=775001&status=done&style=none&width=1107.6666666666667)
<a name="d7eMp"></a>
### 3、启动容器
```bash
docker run -d -p 8081:8081 -p 8082:8082 -p 8083:8083 -v /etc/localtime:/etc/localtime --name nexus3 sonatype/nexus3
```
# -v /home/nexus/nexus-data:/nexus-data 将数据挂载到宿主机<br />## 8081：nexus3网页端<br />## 8082：docker(hosted)私有仓库，可以pull和push<br />## 8083：docker(proxy)代理远程仓库，只能pull
<a name="Bk8a6"></a>
#### 查看启动日志
```bash
docker logs nexus3
```
<a name="274FA"></a>
### 4、开放端口
```bash
firewall-cmd --zone=public  --permanent --add-port=8081/tcp
firewall-cmd --zone=public  --permanent --add-port=8082/tcp
firewall-cmd --zone=public  --permanent --add-port=8083/tcp
重新启动防火墙
systemctl restart firewalld
```
<a name="UEpci"></a>
### 5、访问Nexus服务
[http://localhost:8081/](http://localhost:8081/)<br />点击右上角登录 账号密码：admin/admin123<br />也可以通过`docker exec`命令进入容器中查看admin帐号的密码
```bash
docker exec -it nexus3 /bin/bash
vi /nexus-data/admin.password
exit
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610763093831-bd0876be-1f30-4365-ae38-77ad90a407c9.png#align=left&display=inline&height=602&originHeight=1805&originWidth=3840&size=2053902&status=done&style=none&width=1280)
<a name="zwDXm"></a>
### 6、配置Nginx端口转发时的注意事项
:::tips
下面这两处需要配置为服务器的外网IP，否则会在加载静态资源时从该配置的主机名进行加载，如果配置为其他名称则会加载不到静态资源，会出现无法访问的问题
:::
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1610763681725-51cca37f-2095-46dd-ad13-fd74221f3619.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2115980&status=done&style=none&width=1107.6666666666667)
<a name="NYtPq"></a>
### 7、设置开机启动
```bash
docker update --restart=always nexus3
```
<a name="8FMUw"></a>
### 8、在项目中配置使用新的仓库地址
```xml
<repositories>
    <repository>
        <id>maven-nexus</id>
        <name>maven-nexus</name>
        <url>http://192.168.0.129:8081/repository/maven-public/</url>
        <releases>
            <enabled>true</enabled>
        </releases>
        <snapshots>
            <enabled>true</enabled>
        </snapshots>
    </repository>
</repositories>
```
<a name="zyy9g"></a>
### 9、Maven 配置使用私服（下载插件）
```xml
<pluginRepositories>
    <pluginRepository>
        <id>maven-nexus</id>
        <name>maven-nexus</name>
        <url>http://192.168.0.129:8081/nexus/repository/maven-public/</url>
        <releases>
            <enabled>true</enabled>
        </releases>
        <snapshots>
            <enabled>true</enabled>
        </snapshots>
    </pluginRepository>
</pluginRepositories>
```
<a name="HdudB"></a>
### 10、Maven 配置使用私服（发布依赖）
<a name="VFTXb"></a>
#### （1）首先修改 setting.xml 文件
指定 releases 和 snapshots server 的用户名和密码：
```xml
<servers>
    <server>
        <id>releases</id>
        <username>admin</username>
        <password>123</password>
    </server>
    <server>
        <id>snapshots</id>
        <username>admin</username>
        <password>123</password>
    </server>
</servers>
```
<a name="EKBMu"></a>
#### （2）配置项目的 pom.xml文件
在项目的 pom.xml 文件中加入 distributionManagement 节点：
:::tips
注意：repository 里的 id 需要和上一步里的 server id 名称保持一致。
:::
```xml
<distributionManagement>
    <repository>
        <id>releases</id>
        <name>Releases</name>
        <url>http://192.168.0.129:8081/repository/maven-releases/</url>
    </repository>
    <snapshotRepository>
        <id>snapshots</id>
        <name>Snapshot</name>
        <url>http://192.168.0.129:8081/repository/maven-snapshots/</url>
    </snapshotRepository>
</distributionManagement>
```
