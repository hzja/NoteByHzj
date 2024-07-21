# 交换机

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1642300390959-4872e172-a4dc-4121-9b91-d8a5e1d6ea2b.webp#clientId=u758ee7a9-ae62-4&from=paste&id=uc63a78cc&originHeight=297&originWidth=729&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua192a554-98ea-4059-8919-66df6f999ab&title=)



### S口
**Serial**接口的意思，也叫高速异步串口，主要是连接广域网的V.35线缆用的，说白了就是路由器和路由器连接时候用的，可以用命令设置带宽，一般也就在10M、8M左右。

### E口
是**Ethernet**接口，叫以太网接口，也是主要连接以太网（局域网）用的，也是用普通的双绞线就可以连接，速率默认是10Mbps，现在新型的设备上已经把这个接口淘汰了。另外，路由器上还有一个必不可少的接口是Console口，叫控制口，这个接口是用来调试路由器的。有的路由器还有AUX接口，也是控制接口；还有G口，是千兆以太网接口，是连接以太网用的。

### F口
是**FastEthernet**接口，快速以太网口，也叫百兆口。主要连接以太网（局域网）用的，说白了就是连接交换机或电脑用的，用普通的双绞线就可以连接，速率默认是100Mbps，可以用命令限速，但是不可能超过100Mbps。



### G口
G口是**Gigabitethernet**的意思，是千兆口。Gigabit只表示它是千兆口，不管是光口还是电口显示都是一样的，要详细看的话，可以`display int`查看。

GigabitEthernet可以是光口，也可以是电口，具体看设备参数。Ethernet是以太网端口，GigabitEthernet是千兆以太网端口，两者都是由IEEE 802.3-2005标准定义，可以采用网线也可以采用光纤。
