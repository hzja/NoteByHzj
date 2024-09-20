Java Arthas
<a name="vpC1z"></a>
## 1、Arthas 介绍
**Arthas** 是 Alibaba 在 2018 年 9 月开源的 **Java 诊断**工具。支持 `JDK6+`， 采用命令行交互模式，提供 `Tab` 自动补全，可以方便的定位和诊断线上程序运行问题。<br />开源地址：[_https://github.com/alibaba/arthas_](https://github.com/alibaba/arthas)<br />官方文档：[https://arthas.aliyun.com/doc/](https://arthas.aliyun.com/doc/)
<a name="q6MoE"></a>
## 2、Arthas 使用场景
仅列举几项常见的使用情况

1. 是否有一个全局视角来查看系统的运行状况？
2. 为什么 CPU 又升高了，到底是哪里占用了 CPU ？
3. 运行的多线程有死锁吗？有阻塞吗？
4. 程序运行耗时很长，是哪里耗时比较长呢？如何监测呢？
5. 这个类从哪个 jar 包加载的？为什么会报各种类相关的 Exception？
6. 修改的代码为什么没有执行到？难道是没 commit？分支搞错了？
7. 遇到问题无法在线上 debug，难道只能通过加日志再重新发布吗？
8. 有什么办法可以监控到 JVM 的实时运行状态？
<a name="ELESw"></a>
## 3、Arthas下载安装
<a name="910e6224"></a>
### A、下载-在Github发布页
[Github下载页地址](https://github.com/alibaba/arthas/releases)<br />[https://github.com/alibaba/arthas/releases](https://github.com/alibaba/arthas/releases)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594876607356-74ab1efb-4c65-4ab3-b10a-6cce3cc1bf6d.png#averageHue=%23e6cb9a&height=687&id=pGlPf&originHeight=2060&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=500764&status=done&style=shadow&title=&width=1266)
<a name="dzj4G"></a>
### B、下载程序包
[arthas-bin.zip](https://www.yuque.com/attachments/yuque/0/2020/zip/396745/1595930855072-5d58cb97-c3b0-4fca-858d-006082a83447.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fzip%2F396745%2F1595930855072-5d58cb97-c3b0-4fca-858d-006082a83447.zip%22%2C%22name%22%3A%22arthas-bin.zip%22%2C%22size%22%3A11844306%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22uid%22%3A%221594876505331-0%22%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22dcPtp%22%2C%22card%22%3A%22file%22%7D)
<a name="TyMA9"></a>
### C、使用下载工具下载
```bash
# github下载
wget https://alibaba.github.io/arthas/arthas-boot.jar
# 或者 Gitee 下载
wget https://arthas.gitee.io/arthas-boot.jar
curl -O https://arthas.aliyun.com/arthas-boot.jar
```
<a name="M3e58"></a>
### D、在线安装
:::info
此方式只适应于 Linux/Unix/Mac 等平台，命令如下
:::
```bash
curl -L https://arthas.aliyun.com/install.sh | sh
```
启动arthas
```bash

./as.sh PID #进程id 指定JAVA进程id
./as.sh -h #h来获取更多参数信
```
<a name="NF36G"></a>
### E、在Docker中安装
```bash
#运行arthas-demo
docker run --name arthas-demo -it hengyunabc/arthas:latest /bin/sh -c "java -jar /opt/arthas/arthas-demo.jar"
#安装arthas
docker exec -it  ${containerId} /bin/bash -c "wget https://arthas.aliyun.com/arthas-boot.jar && java -jar arthas-boot.jar"
```
也可以很简单把Arthas安装到Docker镜像里，如下：
```dockerfile
FROM openjdk:8-jdk-alpine

# copy arthas
COPY --from=hengyunabc/arthas:latest /opt/arthas /opt/arthas
```
<a name="kSHmY"></a>
## 4、Arthas 使用
**Arthas** 是一款命令行交互模式的 Java 诊断工具，由于是 Java 编写，所以可以直接下载相应 的 jar 包运行。通过 `java -jar` 命令执行
<a name="idtVz"></a>
### A、使用`java -jar`启动后选择检测的Java进程
```bash
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas                                                   [13:18]
❯ cd .\arthas-bin\
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                       [13:18]
❯ java -jar arthas-boot.jar
[INFO] arthas-boot version: 3.3.6
[INFO] Found existing java process, please choose one and input the serial number of the process, eg : 1. Then hit ENTER.
* [1]: 3984
  [2]: 22068
  [3]: 20552 org.jetbrains.idea.maven.server.RemoteMavenServer36
  [4]: 27784 arthas-boot.jar
  [5]: 10492 org.jetbrains.jps.cmdline.Launcher
  [6]: 9244 com.smart.Application
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594876797130-cde51418-87b9-4bb9-88f5-732f231dc76b.png#averageHue=%233e3e3e&height=583&id=Nlmwp&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2180705&status=done&style=none&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594877335975-93cc6eaf-56a8-428f-b974-b6bbe919f32d.png#averageHue=%233c3c3c&height=583&id=JOrEc&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2075388&status=done&style=none&title=&width=1107.6666666666667)
<a name="PWTox"></a>
### B、在启动时传入需要监测的进程PID
<a name="cKZfW"></a>
#### ①使用jps查看当前的启动的Java进程
```bash
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                       [13:22]
❯ jps
3984
22068
20552 RemoteMavenServer36
26696 Jps
27784 jar
10492 Launcher
9244 Application
```
<a name="jcYwI"></a>
#### ②启动arthas.jar并选择对应的PID
```bash
⨯ fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                       [13:22]
❯ jps
3984
22068
20552 RemoteMavenServer36
26696 Jps
27784 jar
10492 Launcher
9244 Application
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                         [13:23]
❯ java -jar arthas-boot.jar 9244
[INFO] arthas-boot version: 3.3.6
[INFO] arthas home: D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin
[INFO] Try to attach process 9244
[INFO] Attach process 9244 success.
[INFO] arthas-client connect 127.0.0.1 3658
  ,---.  ,------. ,--------.,--.  ,--.  ,---.   ,---.
 /  O  \ |  .--. ''--.  .--'|  '--'  | /  O  \ '   .-'
|  .-.  ||  '--'.'   |  |   |  .--.  ||  .-.  |`.  `-.
|  | |  ||  |\  \    |  |   |  |  |  ||  | |  |.-'    |
`--' `--'`--' '--'   `--'   `--'  `--'`--' `--'`-----'


wiki      https://alibaba.github.io/arthas
tutorials https://alibaba.github.io/arthas/arthas-tutorials
version   3.3.6
pid       9244
time      2020-07-16 13:24:43

[arthas@9244]$
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594877183456-85fae485-2232-4bdd-b1a9-88550e79e616.png#averageHue=%233c3c3c&height=583&id=L51Lx&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2184921&status=done&style=none&title=&width=1107.6666666666667)
<a name="659c798d"></a>
### C、更多启动命令可参考help
```bash
# 其他用法
EXAMPLES:
  java -jar arthas-boot.jar <pid>
  java -jar arthas-boot.jar --target-ip 0.0.0.0
  java -jar arthas-boot.jar --telnet-port 9999 --http-port -1
  java -jar arthas-boot.jar --tunnel-server 'ws://192.168.10.11:7777/ws'
  java -jar arthas-boot.jar --tunnel-server 'ws://192.168.10.11:7777/ws'
--agent-id bvDOe8XbTM2pQWjF4cfw
  java -jar arthas-boot.jar --stat-url 'http://192.168.10.11:8080/api/stat'
  java -jar arthas-boot.jar -c 'sysprop; thread' <pid>
  java -jar arthas-boot.jar -f batch.as <pid>
  java -jar arthas-boot.jar --use-version 3.1.4
  java -jar arthas-boot.jar --versions
  java -jar arthas-boot.jar --session-timeout 3600
  java -jar arthas-boot.jar --attach-only
  java -jar arthas-boot.jar --repo-mirror aliyun --use-http
```
<a name="p0j5T"></a>
## 5、在项目中使用Arthas attach自身进程
<a name="7Vz7z"></a>
### 1）Arthas Spring Boot Starter
目前只支持springboot 2，添加maven：
```xml
<dependency>
    <groupId>com.taobao.arthas</groupId>
    <artifactId>arthas-spring-boot-starter</artifactId>
    <version>${arthas.version}</version>
</dependency>
```
配置属性：
```
arthas.agent-id=hsehdfsfghhwertyfad
arthas.tunnel-server=ws://47.75.156.201:7777/ws
```
通过访问http://localhost:8080/actuator/arthas，来查看agent-id，如下：
```json
{
    "arthasConfigMap": {
        "agent-id": "hsehdfsfghhwertyfad",
        "tunnel-server": "ws://47.75.156.201:7777/ws",
    }
}
```
<a name="ZFEhJ"></a>
### 2）非SpringBoot应用
添加maven依赖：
```xml
<dependency>
  <groupId>com.taobao.arthas</groupId>
  <artifactId>arthas-agent-attach</artifactId>
  <version>${arthas.version}</version>
</dependency>
<dependency>
  <groupId>com.taobao.arthas</groupId>
  <artifactId>arthas-packaging</artifactId>
  <version>${arthas.version}</version>
</dependency>
```
添加启动代码：
```java
import com.taobao.arthas.agent.attach.ArthasAgent;
public class ArthasAttachExample {
    public static void main(String[] args) {
        ArthasAgent.attach();
    }
}
```
也可以配置属性：
```java
HashMap<String, String> configMap = new HashMap<String, String>();
configMap.put("arthas.appName", "demo");
configMap.put("arthas.tunnelServer", "ws://127.0.0.1:7777/ws");
ArthasAgent.attach(configMap);
```
:::danger
**注意**：配置必须是**驼峰**的，和 SpringBoot 的**-**风格不一样。SpringBoot 应用才同时支持**驼峰** 和 **- **风格的配置。<br />其他的配置参数参考：[https://arthas.aliyun.com/doc/arthas-properties.html](https://arthas.aliyun.com/doc/arthas-properties.html)
:::
<a name="BPSvR"></a>
## 6、远程连接监控
<a name="AJf8o"></a>
### A、WebConsole
通过浏览器http访问8563端口，注意页面红色框内的IP也必须指定服务端的IP，不能用默认的127.0.0.1，如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599566414316-0cdc1c91-9102-44c4-80ac-f504b13b8443.png#averageHue=%23070706&height=902&id=ysg7H&originHeight=1804&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=412946&status=done&style=shadow&title=&width=1899)
<a name="NMqDY"></a>
### B、telnet
Windows默认telnet客服端没有开启，需要手动开启一下。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681049527207-a9f8fb0a-b4a7-427c-a32e-feb9ea93f0eb.png#averageHue=%23eceae8&clientId=ud84eb76c-e0a8-4&from=paste&height=385&id=uf4bf8c6e&originHeight=962&originWidth=974&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=125003&status=done&style=none&taskId=u2640ba5e-dd29-434a-9313-ea190dd4670&title=&width=389.6)<br />然后使用 `telnet ip:port` 连接到远程调试的Arthas进程进行调试。
<a name="AdHWx"></a>
### C、Arthas Tunnel Server/Client方式
具体文档：[https://arthas.aliyun.com/doc/tunnel.html](https://arthas.aliyun.com/doc/tunnel.html)<br />通过 Arthas Tunnel Server/Client 来远程管理/连接多个 Agent。<br />比如，在流式计算里，Java 进程可以是在不同的机器启动的，想要使用 Arthas 去诊断会比较麻烦，因为用户通常没有机器的权限，即使登陆机器也分不清是哪个 Java 进程。<br />在这种情况下，可以使用 Arthas Tunnel Server/Client。<br />下载Arthas-tunnel-Server程序包：[https://github.com/alibaba/arthas/releases](https://github.com/alibaba/arthas/releases)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599566683125-c38a0120-2343-4915-aeae-93a592df9506.png#averageHue=%23fefefe&height=904&id=Y6ZKL&originHeight=1807&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=372270&status=done&style=shadow&title=&width=1899)<br />在服务端启动该程序包
```bash
java -jar arthas-tunnel-server.jar
```
默认情况下，arthas tunnel server的web端口是8080，arthas agent连接的端口是7777。<br />启动arthas，注册到tunnel server，并指定一个agent-id：mytest123（默认是随机产生）：
```bash
java -jar arthas-boot.jar --tunnel-server 'ws://192.168.1.28:7777/ws' --agent-id mytest123
```
通过 Spring Boot 的 Endpoint，可以查看到具体的连接信息： http://127.0.0.1:8080/actuator/arthas，登陆用户名是arthas，密码在 arthas tunnel server 的日志里可以找到，比如：
```
32851 [main] INFO  o.s.b.a.s.s.UserDetailsServiceAutoConfiguration

Using generated security password: f1dca050-3777-48f4-a577-6367e55a78a2
```
然后可以得到已经注册的应用的agentId<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681781269389-3da75904-df2f-48b3-920e-7dafde159d7e.png#averageHue=%23fafaf9&clientId=u74ab33c3-b583-4&from=paste&height=450&id=uea535c3f&originHeight=1124&originWidth=1019&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=116913&status=done&style=none&taskId=u042eb3f0-1523-4a1f-9a7d-9430d3033bf&title=&width=407.6)<br />然后访问 tunnel-server，可以看到所有连接的应用列表：http://localhost:8080/apps.html不可用，后台会报错<br />在新窗口打开<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681723199498-c973e39a-6876-4b17-a90a-2ba3748ea765.png#averageHue=%23fefefe&clientId=u9d1469c8-fdfa-4&from=paste&id=u046f2d34&originHeight=378&originWidth=1070&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u90baea07-e805-4e1d-a1be-e6a760922ac&title=)<br />再打开详情，则可以看到连接的所有 agent 列表：http://localhost:8080/agents.html?app=test<br />在新窗口打开<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1681723199441-64d75a03-e858-4038-830d-63bc98eb9314.png#averageHue=%23fefefe&clientId=u9d1469c8-fdfa-4&from=paste&id=ue0f63f88&originHeight=334&originWidth=1072&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7e52c2c5-16d3-4efc-945e-145a27ff8e7&title=)
<a name="jTVhJ"></a>
## 7、Arthas的Web控制台窗口
**Arthas** 支持 Web Console，在成功启动连接进程之后就已经自动启动，可以直接访问 [http://127.0.0.1:8563/](http://127.0.0.1:8563/) 访问，页面上的操作模式和控制台完全一样。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594877600698-6b2514c1-8752-43d6-8ce1-2c41a90aec3a.png#averageHue=%231a1a19&height=687&id=zDRK6&originHeight=2060&originWidth=3832&originalType=binary&ratio=1&rotation=0&showTitle=false&size=378701&status=done&style=none&title=&width=1277.3333333333333)
