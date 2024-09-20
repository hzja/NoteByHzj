Java jar**
<a name="HWQ1A"></a>
## 调试模式的参数
启动中需要加入特定参数才能使用debug模式，并且需要开放调试端口
<a name="D5BNf"></a>
### JDK5-8
```bash
-agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=5005
```
<a name="Uhbq0"></a>
### JDK9+
```bash
-agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=*:5005
```
`suspend=n`表示的是启动Java应用时是否立即进入调试模式，`suspend=y`表示启动即暂停，`suspend=n`则表示启动时不需要暂停。`address=*:5005`表示的是`Debug`监听的服务地址和端口，根据需求修改，上述配置会监听到`0.0.0.0`。
<a name="pL5X5"></a>
### 参数说明
<a name="32VB8"></a>
#### (1) transport
指定运行的被调试应用和调试者之间的通信协议，它由几个可选值：<br />dt_socket：主要的方式，采用 socket 方式连接<br />dt_shmem：采用共享内存方式连接，仅支持 Windows 平台（暂未验证）
<a name="Npug6"></a>
#### (2) server
当前应用作为调试服务端还是客户端，默认为 n。<br />如果想将当前应用作为被调试应用，设置该值为 y；如果想将当前应用作为客户端，作为调试的发起者，设置该值为 n。
<a name="NuDwQ"></a>
#### (3) suspend
当前应用启动后，是否阻塞应用直到被连接，默认值为 y。<br />在大部分的应用场景，这个值为 n，即不需要应用阻塞等待连接。一个可能为 y 的应用场景是，程序在启动时出现了一个故障，为了调试，必须等到调试方连接上来后程序再启动。
<a name="dd952250"></a>
#### (4) address
暴露的调试连接端口，默认值为 8000。
<a name="9cebb1b6"></a>
#### (5) onthrow
当程序抛出设定异常时，中断调试。
<a name="2Mhby"></a>
#### (6) onuncaught
当程序抛出未捕获异常时，是否中断调试，默认值为 n。
<a name="zfsTK"></a>
#### (7) launch
当调试中断时，执行的程序。
<a name="bTBqe"></a>
#### (8) timeout
该参数限定为 `java -agentlib:jdwp=…` 可用，单位为毫秒ms。<br />当 `suspend = y` 时，该值表示等待连接的超时；当 `suspend = n` 时，该值表示连接后的使用超时。
<a name="uLeZs"></a>
## IDEA配置远程调试
<a name="YWGar"></a>
## 编译后的class文件夹调试
只需要将编译后的class字节码文件打包成一个jar包或者是war包，然后执行如上操作就好了。
```bash
cd src
jar -cvf test.jar *
```
<a name="hlRYg"></a>
## 总结
总的来说调试还是比较重要，其实不仅仅是jar包的调试，比如还有Spring Boot（Spring Boot以jar包启动调试与上面类似）、Tomcat、weblogic这些。假设条件允许情况下，做一些审计时候，比如war包，也可以采用远程调试的方式进行审计。会更简单快捷定位一些。
