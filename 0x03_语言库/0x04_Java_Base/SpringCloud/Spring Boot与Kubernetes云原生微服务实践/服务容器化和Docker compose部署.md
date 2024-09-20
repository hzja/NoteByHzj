微服务容器化 Docker
<a name="YNv9G"></a>
## Staffjoy本地部署架构
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1661525535075-3fb16ad4-4d86-42a7-80a8-ec60c3284d6f.jpeg)
<a name="dI5w5"></a>
### 相关软件
<a name="vEMY4"></a>
#### Skywalking
[https://github.com/apache/skywalking](https://github.com/apache/skywalking)
<a name="OVsG6"></a>
## 本地部署启动
<a name="XWqOG"></a>
### 部署步骤
<a name="wHkKd"></a>
#### 1、清理释放内存
活动监控器<br />关闭Docker
<a name="ZmP4D"></a>
#### 2、部署MySQL数据库
staffjoy_account<br />staffjoy_company
<a name="n4MQ4"></a>
##### 创建数据库
```sql
create database staffjoy_account;
create database staffjoy_company;
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631628416785-b2136bf5-d0d8-437d-b749-3eaf703288e7.png#averageHue=%23595959&clientId=ua67ee839-8668-4&from=paste&height=135&id=uc57fe3a6&originHeight=404&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=429428&status=done&style=none&taskId=u98618381-9f1c-4768-a5b3-3448ca55bae&title=&width=1107.6666666666667)
<a name="rgjAf"></a>
##### 创建表-对应的SQL脚本文件
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631628637645-2e875955-3bd0-447e-9e01-acb74d4e71ff.png#averageHue=%23f8f8f7&clientId=ua67ee839-8668-4&from=paste&height=600&id=u93969f01&originHeight=1799&originWidth=900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=130410&status=done&style=shadow&taskId=ue5f7623d-ada5-4b4a-969c-c5b003d3323&title=&width=300)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631628709442-f286ac5d-424b-4347-963a-b7209f4b8ed2.png#averageHue=%23444444&clientId=ua67ee839-8668-4&from=paste&height=372&id=u2b3eb441&originHeight=1117&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1340869&status=done&style=none&taskId=u4d049f5f-401c-47cb-bd68-3b80adfe7e1&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631628772827-05186793-e361-459f-9d9c-c7412cfb7831.png#averageHue=%233e3e3e&clientId=ua67ee839-8668-4&from=paste&height=583&id=u2506efcf&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2109791&status=done&style=none&taskId=u487fc4e5-7c55-41cb-9f69-6354dea0b5a&title=&width=1107.6666666666667)
<a name="AQQ6K"></a>
##### 检查表是否创建完成
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631628838383-aa883a92-4837-4a6b-922c-83404f50a2d5.png#averageHue=%233c3c3c&clientId=ua67ee839-8668-4&from=paste&height=457&id=ub5f8dbcb&originHeight=1371&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1565365&status=done&style=none&taskId=ua74028be-9c0a-4b67-aade-d8e8328dd89&title=&width=1107.6666666666667)
<a name="Nxcvf"></a>
#### 3、部署Skywalking
[https://archive.apache.org/dist/incubator/skywalking/](https://archive.apache.org/dist/incubator/skywalking/)<br />[apache-skywalking-apm-incubating-6.0.0-beta.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1631635132065-1c87ce48-d35f-47e5-a0a0-8d78dba8a009.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1631635132065-1c87ce48-d35f-47e5-a0a0-8d78dba8a009.zip%22%2C%22name%22%3A%22apache-skywalking-apm-incubating-6.0.0-beta.zip%22%2C%22size%22%3A127954103%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22mode%22%3A%22title%22%2C%22taskId%22%3A%22uf14406af-246c-4df6-a941-9ad53af205d%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u6a9c1b04%22%2C%22card%22%3A%22file%22%7D)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631631826324-b4cbefee-7541-4afd-8afb-6a497af311bd.png#averageHue=%23faf9f9&clientId=u6da2e035-f402-4&from=paste&height=341&id=u1dc86f63&originHeight=1023&originWidth=2069&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154351&status=done&style=shadow&taskId=u6f19d33b-e6e2-434c-a258-ba2b7c28eb1&title=&width=689.6666666666666)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631631895376-a19d5729-0711-4347-85ee-9cd730189ed4.png#averageHue=%23f8f8f8&clientId=u6da2e035-f402-4&from=paste&height=327&id=u5bf0141c&originHeight=980&originWidth=2248&originalType=binary&ratio=1&rotation=0&showTitle=false&size=261237&status=done&style=shadow&taskId=ua3f50153-8867-48c6-8c24-88bfb6574c0&title=&width=749.3333333333334)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631631929751-e1e8c895-fb46-482a-92be-f23f10f5acaa.png#averageHue=%23faf9f8&clientId=u6da2e035-f402-4&from=paste&height=256&id=u2e96ea15&originHeight=768&originWidth=700&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45720&status=done&style=shadow&taskId=uf54c8d81-77fe-42d9-a9d2-735f4c15b5b&title=&width=233.33333333333334)
```bash
bin/startup.sh
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631632052019-9786d7d7-6cf2-4b9a-a342-876fbb02b641.png#averageHue=%23272626&clientId=u6da2e035-f402-4&from=paste&height=583&id=uac57e733&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=778453&status=done&style=none&taskId=u95d0cd0d-daf9-4c53-9435-ffd1c44216c&title=&width=1107.6666666666667)
<a name="SG9Kl"></a>
##### 访问Skywalking Web界面
默认的用户名和密码都是：admin<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631634685091-e8f1b1d8-86e2-4e41-89f9-244e16dafdda.png#averageHue=%23ebeff4&clientId=u9f48719d-559a-4&from=paste&height=601&id=u7b93c612&originHeight=1803&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=273552&status=done&style=none&taskId=u991d7349-5c2b-4b28-bf1f-d3d85dc8268&title=&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631634840403-6e3c0a72-9dd5-48f0-bbef-30dff4b016f7.png#averageHue=%23fefdfd&clientId=uaf65874b-746a-4&from=paste&height=601&id=u32b507d1&originHeight=1803&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=228804&status=done&style=shadow&taskId=u8c17a2d8-352a-4445-b1ce-3ad27ddf19c&title=&width=1266)
<a name="O2WMM"></a>
##### IDE设置Skywalking-agent
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631632098418-b9452774-cdf7-4981-a6d2-d54d58c1d7c8.png#averageHue=%23efefef&clientId=u6da2e035-f402-4&from=paste&height=502&id=ub1de33d1&originHeight=1506&originWidth=2770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=277736&status=done&style=shadow&taskId=ue6eda581-50e9-41b0-aa68-a737f8b8ff7&title=&width=923.3333333333334)
<a name="jpMMw"></a>
#### 4、部署服务
<a name="aKpjH"></a>
##### 本地机密数据配置
<a name="AeeJ4"></a>
##### 依次启动项目
mail->bot->account->company->www->whoami->faraday<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631633463797-1ef9164a-ede8-4cbc-ab60-7ff60697326e.png#averageHue=%23f0f0ef&clientId=u6da2e035-f402-4&from=paste&height=291&id=u71bb43e9&originHeight=873&originWidth=834&originalType=binary&ratio=1&rotation=0&showTitle=false&size=84838&status=done&style=shadow&taskId=u0f7ee603-33dd-4cbc-8f9c-5745d9d257a&title=&width=278)
<a name="m8GI7"></a>
##### 查看Skywalking监控到的服务
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631635372013-4bb05a41-a45b-4c33-b375-61fb38c25998.png#averageHue=%23fefdfd&clientId=uca2103af-f12d-4&from=paste&height=601&id=u7afffd59&originHeight=1803&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=233977&status=done&style=shadow&taskId=ude712dd0-3f72-4b5a-9fdf-b2c5456549e&title=&width=1266)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631635489145-a0126132-ea56-4d41-91fd-cecfd7ddfafb.png#averageHue=%23fdfdfd&clientId=uca2103af-f12d-4&from=paste&height=600&id=u4875943b&originHeight=1800&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=407177&status=done&style=none&taskId=uebd13c03-27db-4678-afde-fd995209b63&title=&width=1266)
<a name="Flc7D"></a>
#### 5、部署前端应用
```bash
npm install
npm start
```
<a name="M9QiW"></a>
#### 6、启动SwitchHosts
```
127.0.0.1 account.staffjoy-v2.local
127.0.0.1 faraday.staffjoy-v2.local
127.0.0.1 myaccount.staffjoy-v2.local
127.0.0.1 whoomi.staffjoy-v2.local
127.0.0.1 www.staffjoy-v2.local
127.0.0.1 ical.staffjoy-v2.local
127.0.0.1 staffjoy-v2.local
127.0.0.1 app.staffjoy-v2.local
127.0.0.1 company.staffjoy-v2.local
```
<a name="f7FxD"></a>
## Docker和Docker Compose
<a name="XjHWb"></a>
### 容器用途

1. 标准化打包
2. 隔离
3. 标准化部署
<a name="vJtYK"></a>
### OS & Kernel
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631721292590-cd7ec33c-8a2f-4610-91ba-9b1da7de13f3.jpeg)<br />操作系统内核主要负责管理底层操作系统系统硬件，用户空间调用内核空间的内容。
<a name="fS42G"></a>
### 虚拟机和容器
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631721695484-263cb462-b3d4-4c45-af05-290730ecd719.jpeg)<br />虚拟机主要在宿主机上通过Hypervisor创建虚拟化的硬件，可以虚拟出CPU、磁盘、网卡等等。在Hypervisor上面运行操作系统。容器是构建在宿主机之上。虚拟机的隔离性比较好，是一种强隔离机制，但他比较重量，启动的时候慢，消耗资源很多。容器的隔离是基于软件的，比较轻量，启动速度比较快，一般是秒级的，消耗的资源也比较少。在同一个主机上启动容器的数量要远远多于虚拟机的启动数量。
<a name="IkAhk"></a>
### Docker容器核心技术
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1662687724970-a435c8d7-c0f3-48b8-b772-728a96d140c7.jpeg)<br />CGroups：控制CPU和内存资源的使用量。负责资源的隔离<br />Namespaces：控制每个容器都有相互隔离的进程，文件系统，网络等空间。<br />Networking：实现容器网络底层技术，包括虚拟网卡，虚拟网桥，容器网络组网等等。<br />Storage（存储）：Docker容器支持一些存储驱动。
<a name="bX9T3"></a>
### 容器镜像
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631756257460-180d49b7-c5e5-4d4a-9432-08edf75c267d.jpeg)
<a name="VERQ4"></a>
### Docker架构
[https://docs.docker.com/get-started/overview/](https://docs.docker.com/get-started/overview/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631757589959-283f0506-53c2-4eda-9fb5-5d555657250f.png#averageHue=%2375b62b&clientId=u6de851db-b635-4&from=paste&height=404&id=ud36bb3cc&originHeight=1213&originWidth=2318&originalType=binary&ratio=1&rotation=0&showTitle=false&size=246152&status=done&style=shadow&taskId=ueee1683f-c65f-40ed-9954-71212dba96c&title=&width=772.6666666666666)
<a name="jW5tY"></a>
### Docker Compose
Docker Compose主要用于定义和运行多容器应用的工具。<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631806074924-5e67a928-cf9e-4d29-aeb9-af7466a9c810.jpeg)
<a name="mDn2Z"></a>
## 将微服务部署至Docker Compose环境
<a name="RobPq"></a>
### 1、镜像构建
```bash
mvn clean package -DskipTests
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631807415379-cc229048-64e2-471e-bdbf-a3a64558b99a.png#averageHue=%233f3f3e&clientId=u1b9132be-76b8-4&from=paste&height=583&id=ufef4cea6&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2246605&status=done&style=none&taskId=ua82913f1-a21c-407c-9975-f1257b82d22&title=&width=1107.6666666666667)
```bash
docker-compose build
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631844661209-d18db6ff-6cd0-4549-9048-779c043677eb.png#averageHue=%233e3e3e&clientId=ued80350c-5065-4&from=paste&height=593&id=ufbf713aa&originHeight=1780&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2147160&status=done&style=none&taskId=ufe83d8e5-54ab-407a-b878-317855ba781&title=&width=1107.6666666666667)
```bash
docker images
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632379350639-5179a3f1-3df2-4d56-9892-44da705b109a.png#averageHue=%23535353&clientId=u2bad1d93-088d-4&from=paste&height=181&id=u42fa19f4&originHeight=542&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=605674&status=done&style=none&taskId=u55b90045-c5e9-45e7-8f3e-7cf48a9acfd&title=&width=1107.6666666666667)
<a name="COkp4"></a>
### 2、部署MySQL数据库
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1631807458904-37bde253-5385-4256-9fa6-3ac6cf2aa463.png#averageHue=%23f7f6f6&clientId=u1b9132be-76b8-4&from=paste&height=202&id=ucb784c22&originHeight=606&originWidth=455&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29342&status=done&style=shadow&taskId=u99a940b7-e4fa-40e3-aedd-68ec080213a&title=&width=151.66666666666666)
<a name="BWdhu"></a>
### 3、部署Staffjoy
```bash
docker-compose up
docker-compose ps
```
<a name="IXGXX"></a>
### 4、启用SwitchHosts
```
127.0.0.1 account.staffjoy-v2.local
127.0.0.1 faraday.staffjoy-v2.local
127.0.0.1 myaccount.staffjoy-v2.local
127.0.0.1 whoomi.staffjoy-v2.local
127.0.0.1 www.staffjoy-v2.local
127.0.0.1 ical.staffjoy-v2.local
127.0.0.1 staffjoy-v2.local
127.0.0.1 app.staffjoy-v2.local
127.0.0.1 company.staffjoy-v2.local
```
<a name="mADDu"></a>
### 5、校验Staffjoy
<a name="bVvpB"></a>
### 6、清理
```bash
docker-compose down
```
