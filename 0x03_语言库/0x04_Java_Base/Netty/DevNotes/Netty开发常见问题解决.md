Netty
<a name="CWzZF"></a>
## 客户端发送数据服务器端接收不到的问题解决
客户端发送数据，服务器端始终收不到，结果发现，客户端发送时，最终发送出去的格式应该是ByteBuf格式才行，修改如下：
```java
channel.writeAndFlush(Unpooled.buffer().writeBytes(data.getBytes()));
```
<a name="pWWEV"></a>
## 客户端接收报文不完整的情况
<a name="q7cgZ"></a>
### 服务端发送的密文长度
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634914757007-cae37894-8815-4335-9def-0bf1e386b293.png#clientId=ue334068f-fc7d-4&from=paste&height=219&id=u705bc7fe&originHeight=438&originWidth=2501&originalType=binary&ratio=1&size=141651&status=done&style=shadow&taskId=u59c673fc-0b15-4f52-95d0-5cc1d8bfc14&width=1250.5)
<a name="KPWG7"></a>
### 客户端收到的密文被分了三个包
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634914826082-6fb0fc34-5ec1-4af1-af27-ecc05a8278b7.png#clientId=ue334068f-fc7d-4&from=paste&height=437&id=u847580d6&originHeight=874&originWidth=2397&originalType=binary&ratio=1&size=420529&status=done&style=shadow&taskId=u90495eb6-266b-473a-ace1-91c73c42fc8&width=1198.5)
<a name="Fpnj9"></a>
### 解决办法
客户端和服务端在Handler之前配置处理符
:::tips
注意：只配置一方不管用，还是会被分包，必须服务端和客户端都配置。
:::
<a name="bF3GI"></a>
#### 服务端配置
```java
ServerBootstrap b = new ServerBootstrap();
b.option(ChannelOption.RCVBUF_ALLOCATOR, new FixedRecvByteBufAllocator(1024*1024)); //这行配置比较重要;
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634914953383-83084ad2-23b2-4aec-8e15-dc18d8d8b235.png#clientId=ue334068f-fc7d-4&from=paste&height=749&id=u1ace843c&originHeight=1497&originWidth=3672&originalType=binary&ratio=1&size=495504&status=done&style=shadow&taskId=u0c2bb9d0-25b2-4919-aff9-c3b95ad4344&width=1836)
<a name="gCEJT"></a>
#### 客户端配置
```java
Bootstrap b = new Bootstrap();
b.option(ChannelOption.RCVBUF_ALLOCATOR, new FixedRecvByteBufAllocator(1024*1024)); //这行配置比较重要;
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634915052548-24c096df-506b-40dd-a663-1c702a92cd06.png#clientId=ue334068f-fc7d-4&from=paste&height=870&id=uade546a7&originHeight=1740&originWidth=3720&originalType=binary&ratio=1&size=613793&status=done&style=shadow&taskId=ua40fd9c9-41f8-49f1-a570-c70c200dc55&width=1860)
<a name="QgjXt"></a>
### 结果
客户端成功接收到完整的报文。
<a name="MZadX"></a>
#### Server端发出的密文长度
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634915177030-600cbaa6-5c2b-4599-a802-9d85f34a0041.png#clientId=ue334068f-fc7d-4&from=paste&height=205&id=ua2a7d7d7&originHeight=410&originWidth=1918&originalType=binary&ratio=1&size=139129&status=done&style=shadow&taskId=u7ee7f172-e29b-4874-893b-974af145726&width=959)
<a name="HBMxB"></a>
#### Client端接收到的密文长度
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634915120408-49c49f24-6338-4c10-804d-579c8c997cf8.png#clientId=ue334068f-fc7d-4&from=paste&height=280&id=ue12886fc&originHeight=559&originWidth=2629&originalType=binary&ratio=1&size=248060&status=done&style=shadow&taskId=uba657680-8693-4143-9396-ecabcca688f&width=1314.5)
