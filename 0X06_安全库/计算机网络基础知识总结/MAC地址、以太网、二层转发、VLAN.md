MAC地址 <br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891829-872df648-8de2-45f4-9a2b-5a55416e09d5.webp#height=589&id=p7RI9&originHeight=589&originWidth=531&originalType=binary&size=0&status=done&style=shadow&width=531)
<a name="1b82aec7f5d67d1440647334aaf6170f"></a>
## MAC 地址
每个网卡或三层网口都有一个 **MAC 地址**， MAC 地址是烧录到硬件上，因此也称为**硬件地址**。MAC 地址作为数据链路设备的地址标识符，需要保证网络中的每个 MAC 地址都是**唯一**的，才能正确识别到数据链路上的设备。<br />MAC 地址由 **6** 个字节组成。前 3 个字节表示**厂商识别码**，每个网卡厂商都有特定唯一的识别数字。后 3 个字节由厂商给每个网卡进行分配。厂商可以保证生产出来的网卡不会有相同 MAC 地址的网卡。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891905-194788da-557d-45d7-839a-86dbdbdc3b64.webp#height=193&id=FJAN0&originHeight=193&originWidth=484&originalType=binary&size=0&status=done&style=shadow&width=484)MAC地址<br />现在可以通过软件修改 MAC 地址，虚拟机使用物理机网卡的 MAC 地址，并不能保证 MAC 地址是唯一的。但是只要 MAC 地址相同的设备不在同一个数据链路上就没问题。<br />为了查看方便， 6 个字节的 MAC 地址使用**十六进制**来表示。每个字节的 8 位二进制数分别用 2 个十六进制数来表示，例如我的网卡 MAC 地址是 `E0-06-E6-39-86-31`。
<a name="2a55ddb88bb27f2f4ff683ddd7815b5e"></a>
### 什么是字节？什么是比特？
**比特**，英文名 _bit_ ，也叫_位_。二进制中最小单位，一个比特的值要么是 `0` 要么是 `1` 。<br />**字节**，英文名 _Byte_ 。一个字节由八个比特构成。
<a name="b8f5ad85ed35c0d74c55d8aa70a48134"></a>
### MAC 地址怎么使用？
最常用的以太网和无线局域网，都是使用 MAC 地址作为地址标识符进行通信的。
<a name="6880a9ec5f945dbb1c7a825543e1406a"></a>
## 以太网
有线局域网中普遍使用**以太网**，以太网标准简单，传输速率高。常见的网络拓扑结构如下图。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617077891886-455d0fa7-6d29-4231-ade6-f2a62ae3e85a.png#height=269&id=fvsw3&originHeight=269&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)以太网
<a name="69179bc85d40e5b550da7da3bd9a0cd8"></a>
### 什么是网络拓扑？
网络的连接和构成的形态称为**网络拓扑**。它不仅可以直观的看到网络物理连接方式，还可以表示网络的逻辑结构。
<a name="343be8f51dc4ec00a1981c2360e85679"></a>
### 以太网数据格式
当今最常用的以太网协议标准是 `ETHERNET II` 标准。`ETHERNET II` 标准定义的数据帧格式如下图。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891854-815da039-a10e-4fcd-aaad-7f10fbb8b65d.webp#height=88&id=lqExa&originHeight=88&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)
<a name="0d6fa50108aac78afd07cb9572843611"></a>
#### 以太网帧格式

- 前导码（ Preamble ）

前导码由 7 个字节组成，每个字节固定为 `10101010` 。之后的 1 个字节称为**帧起始定界符**，这个字节固定为 `10101011` 。这 8 个字节表示以太网帧的开始，也是对端网卡能够确保与其同步的标志。帧起始定界符的最后两位比特被定义为 11 ，之后就是以太网数据帧的本体。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891876-6cd7fc2f-9f5c-42b7-b915-d8b6c357a9ec.webp#height=218&id=PvT7u&originHeight=218&originWidth=1021&originalType=binary&size=0&status=done&style=shadow&width=1021)前导码

- 目的地址（ Destination Address ）<br />目的地址由 6 个字节组成，用来标识数据帧的目的设备，类似于快递的收件人地址。
- 源地址（ Source Address ）<br />源地址由 6 个字节组成，用来标识数据帧的始发设备，类似于快递的发件人地址。
- 类型（ Type ）<br />类型字段由 2 个字节组成。类型字段是表明上一层（即网络层）的协议类型，可以让接收方使用相同的协议进行数据帧的解封装。
- 数据（ Data ）<br />帧头后就是数据。一个数据帧所能容纳的最大数据范围是 46 ～ 1500 个字节。如果数据部分不足 46 个字节，则填充这个数据帧，让它的长度可以满足最小长度的要求。
- FCS（ Frame Check Sequence ）<br />FCS 由 4 个字节组成，位于数据帧的尾部，用来检查帧是否有所损坏。通过检查 FCS 字段的值将受到噪声干扰的错误帧丢弃。
<a name="cc06664bdadae61d0fb64eba92771a15"></a>
#### 最小的数据帧是多少字节？
数据帧的各字段加起来一共是 `64` 字节，其中数据是 `46` 字节。再加上前导码就是 `72` 字节。因此最小的数据帧是 `72` 字节。在传输过程中，每个数据帧还有 `12` 字节的数据帧间隙，所以最小的可传输数据帧长度是 `84` 字节，即 `672` 比特。
<a name="3f88283bfed727937dea4bff9410e2dd"></a>
### 交换机二层转发原理
交换机有多个网络端口，它通过识别数据帧的**目标 MAC 地址**，根据 **MAC 地址表**决定从哪个端口发送数据。MAC 地址表不需要在交换机上手工设置，而是可以自动生成的。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617077891930-4c67d111-819d-418e-a18d-d0894c12f78d.png#height=363&id=MvKZd&originHeight=363&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)交换机二层转发
<a name="d9e937753ce488ee64d14a0a02976afc"></a>
#### 交换机是如何添加、更新、删除 MAC 地址表条目的？
在初始状态下，交换机的 MAC 地址表是空的，不包含任何条目。当交换机的某个端口接收到一个数据帧时，它就会将这个数据帧的源 MAC 地址、接收数据帧的端口号作为一个条目保存在自己的 MAC 地址表中，同时在接收到这个数据帧时重置这个条目的老化计时器时间。这就是交换机自动添加 MAC 地址表条目的方式。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891916-69277f18-261e-49de-9a3a-ad974b53024d.webp#height=337&id=YXREy&originHeight=337&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)自动添加MAC地址表条目<br />在新增这一条 MAC 地址条目后，如果交换机再次从同一个端口收到相同 MAC 地址为源 MAC 地址的数据帧时，交换机就会**更新**这个条目的老化计时器，确保活跃的的条目不会老化。但是如果在老化时间内都没收到匹配这个条目的数据帧，交换机就会将这个老化的条目从自己的 MAC 地址表中**删除**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891908-478e8ae5-3c9e-47d9-acae-fd1bedb3945b.webp#height=337&id=RBX4h&originHeight=337&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)老化条目被删除<br />还可以手动在交换机的 MAC 地址表中添加**静态条目**。静态添加的 MAC 地址条目优先动态学习的条目进行转发，而且静态条目没有老化时间，会一直保存在交换机的 MAC 地址表中。
<a name="fc7c56462ee1f9f16cf1e15319e35739"></a>
#### 如何使用 MAC 地址表条目进行转发？
当交换机的某个端口收到一个单播数据帧时，它会查看这个数据帧的二层头部信息，并进行两个操作。一个操作是根据源 MAC 地址和端口信息添加或更新 MAC 地址表。另一个操作是查看数据帧的目的 MAC 地址，并根据数据帧的目的 MAC 地址查找自己的 MAC 地址表。在查找 MAC 地址表后，交换机会根据查找结果对数据帧进行处理，这里有 3 中情况：

1. 交换机没有在 MAC 地址表中找到这个数据帧的目的 MAC 地址，因此交换机不知道自己的端口是否有连接这个 MAC 地址的设备。于是，交换机将这个数据帧从除了接收端口之外的所有端口**泛洪**出去。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891950-f0329f92-d64f-4cf2-8071-54899e6c46ed.webp#height=748&id=SZxmf&originHeight=748&originWidth=906&originalType=binary&size=0&status=done&style=shadow&width=906)泛洪

2. 交换机的 MAC 地址表中有这个数据帧的目的 MAC 地址，且对应端口不是接收到这个数据帧的端口，交换机知道目的设备连接在哪个端口上，因此交换机会根据 MAC 地址表中的条目将数据帧从对应端口单播**转发**出去，而其它与交换机相连的设备则不会收到这个数据帧。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892028-23223451-6969-4c38-993a-e4f991b39851.webp#height=748&id=a45cL&originHeight=748&originWidth=911&originalType=binary&size=0&status=done&style=shadow&width=911)转发

3. 交换机的 MAC 地址表中有这个数据帧的目的 MAC 地址，且对应端口就是接收到这个数据帧的端口。这种情况下，交换机会认为数据帧的目的地址就在这个端口所连接的范围内，因此目的设备应该已经收到数据帧。这个数据帧与其它端口的设备无关，不会将数据帧从其它端口转发出去。于是，交换机会**丢弃**数据帧。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892020-55fbd159-5dda-4870-8284-aeed6d7fb91c.webp#height=748&id=SiKvP&originHeight=748&originWidth=911&originalType=binary&size=0&status=done&style=shadow&width=911)丢弃<br />**单播**：主机一对一的发送数据。单播地址是主机的 MAC 地址。<br />**广播**：向局域网内所有设备发送数据。只有全 1 的 MAC 地址为广播 MAC 地址，即 `FF-FF-FF-FF-FF-FF` 。<br />**泛洪**：将某个端口收到的数据从除该端口之外的所有端口发送出去。泛洪操作广播的是普通数据帧而不是广播帧。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077891972-6b301198-6c0d-434b-a485-6ad2cd52a851.webp#height=345&id=i896M&originHeight=345&originWidth=444&originalType=binary&size=0&status=done&style=shadow&width=444)单播![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892010-97367062-4c90-4c50-9fcb-771d589ee676.webp#height=339&id=UBSTn&originHeight=339&originWidth=442&originalType=binary&size=0&status=done&style=shadow&width=442)广播
<a name="979756b47aedf3800cd5c6ec180d923e"></a>
## VLAN
**广播域**<br />网络规模越大，广播域就越大，泛洪流量也越来越大，降低通信效率。在一个广播域内的任意两台主机之间可以任意通信，通信数据有被窃取的风险。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617077892048-a5003046-5816-497f-bdcd-63afb6314486.png#height=516&id=HJBD1&originHeight=516&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)泛洪攻击<br />为了解决广播域扩大带来的性能问题和安全性降低问题， VLAN 技术应运而生。VLAN 技术能够在逻辑上把一个物理局域网**分隔**为多个广播域，每个广播域称为一个**虚拟局域网**（即 VLAN ）。每台主机只能属于一个 VLAN ，同属一个 VLAN 的主机通过**二层**直接通信，属于不同 VLAN 的主机只能通过 **IP 路由**功能才能实现通信。通过划分多个 VLAN ，从而减小广播域传播的范围，过滤多余的包，提高网络的传输效率，同时提高了网络的安全性。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892043-fc2f0c7b-75c2-40e1-82d8-d4cd95d2abc4.webp#height=304&id=MCmP7&originHeight=304&originWidth=686&originalType=binary&size=0&status=done&style=shadow&width=686)VLAN
<a name="b06c42b71225daecbfaa1cf8d2495544"></a>
### VLAN 原理
VLAN 技术通过给数据帧插入 **VLAN 标签**（又叫 `VLAN TAG`）的方式，让交换机能够分辨出各个数据帧所属的 VLAN 。<br />VLAN 标签是用来区分数据帧所属 VLAN 的，是 4 个字节长度的字段，插入到以太网帧头部上。VLAN 标签会插入到源 MAC 地址后面， **IEEE 802.1Q** 标准有这个格式定义和字段构成说明。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892033-7b037700-590d-41e1-8bf1-e22c72461033.webp#height=593&id=Na7jj&originHeight=593&originWidth=1080&originalType=binary&size=0&status=done&style=shadow&width=1080)VLAN TAG

- TPID （标签协议标识符）：<br />长度 2 个字节，值为 `0x8100` ，用来表示这个数据帧携带了 802.1Q 标签。不支持 802.1Q 标准的设备收到这类数据帧，会把它丢弃。
- TCI （标签控制信息）：<br />长度 2 个字节，又分为三个子字段，用来表示数据帧的控制信息：
- 优先级（ Priority ）：长度为 3 比特，取值范围 `0 ~ 7` ，用来表示数据帧的优先级。取值越大，优先级越高。当交换机发送拥塞是，优先转发优先级高的数据帧。
- CFI （规范格式指示器）：长度为 1 比特，取值非 0 即 1 。
- **VLAN ID** （ VLAN 标识符）：长度为 12 比特，用来表示 VLAN 标签的数值。取值范围是 `1 ~ 4094` 。
<a name="d5cf29fc3ba945d09d05ab31ff8df42d"></a>
#### 划分 VLAN 后，交换机如何处理广播报文？
交换机上划分了多个 VLAN 时，在交换机接收到广播数据帧时，只会将这个数据帧在相同 VLAN 的端口进行**广播**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892033-c74d6e53-9e22-4a02-bd34-f4f3c7d803b3.webp#height=621&id=vYZUU&originHeight=621&originWidth=921&originalType=binary&size=0&status=done&style=shadow&width=921)广播帧在相同VLAN内广播
<a name="44abf37a780566099b200c3c54ad71c3"></a>
#### 划分 VLAN 后，交换机如何处理目的 MAC 地址不在 MAC 地址表中的单播数据帧？
交换机上划分了多个 VLAN 时，当交换机接收到一个目的 MAC 地址不存在于自己 MAC 地址表中的单播数据帧时，只会将这个数据帧在相同 VLAN 的端口进行**泛洪**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892039-9d597103-8d7d-4dfa-9b29-a93b41c72c86.webp#height=621&id=ANgXO&originHeight=621&originWidth=921&originalType=binary&size=0&status=done&style=shadow&width=921)单播帧在相同VLAN内泛洪
<a name="79bfa72b93742508a0624d5ee171e63a"></a>
#### 划分 VLAN 后，不同 VLAN 的主机能否通信？
划分多 VLAN 的环境中，即使交换机 MAC 地址表里保存了某个数据帧的目的 MAC 地址条目，若这个目的 MAC 地址所对应的端口与数据帧的入端口在不同的 VLAN 中，交换机也不会通过 MAC 地址表中的端口发送数据帧。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892108-acfa2906-fc06-480c-9015-0396df1201ff.webp#height=621&id=kfuEx&originHeight=621&originWidth=921&originalType=binary&size=0&status=done&style=shadow&width=921)不同VLAN的主机不能通信<br />**小结**：在不使用路由转发的前提下，交换机不会从一个 VLAN 的端口中接收到的数据帧，转发给其它 VLAN 的端口。
<a name="c2a48a21d3d66858750ab2246403393f"></a>
#### 怎么区分不同的 VLAN ？
通过 VLAN ID 进行区分，例如 VLAN 10 和 VLAN 20 就是不同的 VLAN 。
<a name="4462ddd7947238ee7b20ae45b6de1bbb"></a>
#### VLAN 技术有哪些好处？

- 增加了广播域的数量，减小了每个广播域的规模，也减少了每个广播域中终端设备的数量；
- 增强了网络安全性，保障网络安全的方法增加了；
- 提高了网络设计的逻辑性，可以规避地理、物理等因素对于网络设计的限制。
<a name="0e208bd44ff175003f9511d1079eea77"></a>
### 划分 VLAN
可以使用不同的方法，把交换机上的每个端口划分到某个 VLAN 中，以此在逻辑上分隔广播域。<br />交换机通常会使用**基于端口划分 VLAN** 的方法。在交换机上手动配置，绑定交换机端口和 VLAN ID 的关系。<br />优点：配置简单。想要把某个端口划分到某个 VLAN 中，只需要把端口的 **PVID** （端口 VLAN ID ）配置到相应的 VLAN ID 即可。<br />缺点：当终端设备移动位置是，可能需要为终端设备连接的新端口重新划分 VLAN 。<br />除了这种方法外，还可以使用基于 **MAC 地址划分 VLAN** 、**基于 IP 地址划分 VLAN** 、**基于协议划分 VLAN** 、**基于策略划分 VLAN** 等方法来划分 VLAN。<br />_PVID ：接口默认 VLAN ID ，是交换机端口配置的参数，默认值是 1 。_
<a name="cabbbdab8abf25a7396e4b1deec78282"></a>
### 跨交换机 VLAN 原理
终端设备不会生成带 VLAN 标签的数据帧，它们发出的数据帧叫做**无标记帧**（ Untagged ）。它们连接的交换机会给无标记帧打上 VLAN 标签。交换机通过每个端口的 PVID ，判断从这个接口收到的无标记帧属于哪个 VLAN ，并在转发时，插入相应的 VLAN 标签，从而将无标记帧变为**标记帧**（ Tagged ）。<br />当两台交换机通过端口连接时，收到的数据帧是标记帧还是无标记帧？交换机端口会如何处理呢？<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892107-4f2143ce-a274-4a22-995e-327737395ea5.webp#height=1045&id=rezt5&originHeight=1045&originWidth=711&originalType=binary&size=0&status=done&style=shadow&width=711)<br />跨交换机的VLAN<br />交换机根据连接的设备类型，判断各个接口收到的数据帧是否打标，来配置交换机接口的类型。

- 如果交换机接口收到**无标记帧**，由交换机根据这个接口所在 VLAN 为数据帧打上 VLAN 标签；同时接口发送数据帧时，也不携带 VLAN 标签。应该把这类接口配置为 **Access （接入）接口**， Access 接口连接的链路称为 Access 链路。
- 如果交换机接口收到多个 VLAN 的流量，也就是收到了**标记帧**；同时为了让对端设备能够区分不同 VLAN 的流量，通过接口发出的流量会打上 VLAN 标签。应该把这类接口配置为 **Trunk （干道）接口**，相应的链路称为 Trunk 链路。
<a name="604f42616a5f47327961b3196ca606c8"></a>
#### 跨交换机发送数据
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892116-1ffad11b-ef7e-4c24-9bdf-58e04409a5fb.webp#height=1085&id=kx2sH&originHeight=1085&originWidth=711&originalType=binary&size=0&status=done&style=shadow&width=711)<br />跨交换机VLAN实例<br />**主机 A** 以主机 F 的 MAC 地址作为目的 MAC 地址封装了一个数据帧，从网卡发送出去。<br />**交换机 A** 在 Access 接口收到数据帧。查询 MAC 地址表，发现数据帧的目的地址是与交换机 B 相连的 Trunk 接口。于是交换机给数据帧打上 Access 接口的 PVID 配置，即给数据帧打上 VLAN 10 的标签，并从 Trunk 接口转发给交换机 B 。<br />**交换机 B** 在 trunk 接口收到数据帧。查看 MAC 地址表，发现是 VLAN 10 的数据帧，目的地址设备是连接在 VLAN 10 的一个 Access 接口上。于是去掉数据帧的 VLAN 标签，并从这个 Access 接口转发给主机 F 。
<a name="609c4d17fe40691c7cda60ab4c3dd6db"></a>
### 模拟实验
<a name="4ef0c6ecbe1b9b16948467421c8018fe"></a>
#### Access 接口和 Trunk 接口的配置
<a name="612cd008c2bb9c956ed7572de535a0e3"></a>
##### 实验拓扑图
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892029-e8f51013-1c24-4e67-b629-146f8ee67c6e.webp#height=313&id=nTLVW&originHeight=313&originWidth=681&originalType=binary&size=0&status=done&style=shadow&width=681)<br />Access接口和Trunk接口配置的拓扑图
<a name="a83545879f435b18f08ab0f670b7e383"></a>
##### 实验要求

- 将 SW 1 （即交换机 1）和 SW 2 （即交换机 2）相连的接口配置为 Trunk 接口，允许传输 VLAN 5 的数据；
- 将 PC （即主机） 与 SW 相连接口配置为 Access 接口，接口的 PVID 配置为 VLAN 5 。
<a name="9fde9b19c563116491ee9255ed6f7c0d"></a>
##### 实验步骤

1. SW 1 上的配置如下：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892109-ce93de8e-7526-4eb5-90ac-c8830670cb6c.webp#height=454&id=kXkUr&originHeight=454&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)SW1配置

2. 检查 SW 1 的接口配置，使用命令 display vlan 查看接口 VLAN 情况。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892158-e85efb81-26b4-42b3-a082-9911a17407c4.webp#height=1506&id=mGcA1&originHeight=1506&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)检查VLAN配置
<a name="de21f9c195a653df47722f300c820807"></a>
#### Hybrid 接口的配置
三种接口类型特点：

- **Access 接口**：这种接口只能属于一个 VLAN，只能接收和发送一个 VLAN 的数据。通常用于连接终端设备，比如主机或服务器等。
- **Trunk 接口**：这种接口能够接收和发送多个 VLAN 的数据，通常用于连接交换机。
- **Hybrid 接口**：这种接口能够接收和发送多个 VLAN 的数据，可用于交换机的链路，也可用于终端设备。与 Trunk 接口的区别是，发送数据时 Trunk 接口只会摘掉 PVID 标签，而 Hybrid 接口能够不携带 VLAN 标签发送多个 VLAN 数据。

**实验拓扑图**<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892124-6f0927b4-a5cd-4fff-956a-1c6e5514360b.webp#height=353&id=EF4Vz&originHeight=353&originWidth=574&originalType=binary&size=0&status=done&style=shadow&width=574)<br />Hybrid接口配置的拓扑图<br />**实验要求**

- 新建 3 个 VLAN ，PC 1 属于 VLAN 2 ，PC 2 属于 VLAN 3 ，Server 1 （即服务器 1）属于 VLAN 10 ；
- 通过 Hybrid 接口实现 VLAN 2 和 VLAN 3 不能互通，但 VLAN 2 和 VLAN 3 都能与 VLAN 10 进行通信。

**实验步骤**

1. SW 1 的 E0/0/2 接口，只允许通过 VLAN 2 ， PC 1 又需要访问 VLAN 10 ，但是无法识别 VLAN 标签信息，因此配置 Hybrid 的 PVID 为 VLAN 2 ，同时放通 VLAN 2 和 VLAN 10 。E0/0/3 接口配置同理。E0/0/1 接口需要放通 VLAN 2 、 VLAN 3 和 VLAN 10 的流量，对端交换机又需要识别 VLAN 标签，因此以带 VLAN 标签的形式放通 VLAN 2 、 VLAN 3 和 VLAN 10 的流量。SW 1 上的配置如下：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892053-619ad817-65e4-41e9-b516-0ff946d88145.webp#height=623&id=ShZkz&originHeight=623&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)SW1配置

2. SW 2 的 E0/0/1 接口配置和 SW 1 的 E0/0/1 接口同理。SW 2 的 E0/0/10 接口，只允许通过 VLAN 10 ， Server 1 又需要放通 VLAN 2 和 VLAN 3 的流量，因此配置 Hybrid 的 PVID 为 VLAN 10 ，同时放通 VLAN 2 、 VLAN 3 和 VLAN 10 。SW2 上的配置如下：

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892146-cf291752-a8e5-419a-a103-2dfed9c12b2c.webp#height=454&id=RX5Ts&originHeight=454&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)SW2配置

3. 检查 VLAN 10 信息，分别在 SW 1 和 SW 2 上使用命令 display vlan 10 查看配置是否正确。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892118-0c9a3ce9-2863-4bed-bb6b-c7237296b796.webp#height=895&id=so4Rz&originHeight=895&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)查看SW1的VLAN 10信息![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892147-7eb31f9f-65b1-4588-9171-8f1f8e6b3585.webp#height=895&id=baXyL&originHeight=895&originWidth=1080&originalType=binary&size=0&status=done&style=none&width=1080)查看SW2的VLAN 10信息
<a name="4169bbe508e560a4020419f4111d59fb"></a>
## 结尾
<a name="e34669e8e2d4343d75e9f981ba3727ea"></a>
### Access 接口接收数据帧处理过程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892214-9a5836f1-bffc-413c-8ee4-25301d7ed0fb.webp#height=552&id=wqI0W&originHeight=552&originWidth=429&originalType=binary&size=0&status=done&style=shadow&width=429)<br />Access接口接收数据帧流程图
<a name="9b03c678437a201d0426a1457329c4ac"></a>
### Access 接口发送数据帧处理过程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892198-ff8f1298-1c69-4444-b3f0-f9fe4345f9d0.webp#height=551&id=MCn9h&originHeight=551&originWidth=341&originalType=binary&size=0&status=done&style=shadow&width=341)<br />Access接口发送数据帧流程图
<a name="e1c8a575e51a87c5ba71080c0f77bd38"></a>
### Trunk 接口接收数据帧处理过程
![](https://cdn.nlark.com/yuque/0/2021/png/396745/1617077892070-5114c8a4-9acc-4c1b-ab3c-d5f6b094c0c2.png#height=551&id=EamNF&originHeight=551&originWidth=489&originalType=binary&size=0&status=done&style=shadow&width=489)<br />Trunk接口接收数据帧流程图
<a name="d41db0332ed187c94f8a7d658875bb34"></a>
### Trunk 接口发送数据帧处理过程
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617077892212-efeb338e-7ac9-4ec6-8c68-9a0faf978127.webp#height=551&id=MGuuO&originHeight=551&originWidth=489&originalType=binary&size=0&status=done&style=shadow&width=489)<br />Trunk接口发送数据帧流程图
