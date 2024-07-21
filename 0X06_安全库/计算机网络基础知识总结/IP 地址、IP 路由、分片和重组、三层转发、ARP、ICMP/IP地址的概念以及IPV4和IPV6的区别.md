IP IPV4 IPV6
<a name="xyygP"></a>
## IP地址（Internet Protocol Address）
IP地址（Internet Protocol Address）是指互联网协议地址，又译为网际协议地址。IP地址是一种统一的地址格式，它为互联网上的每一个网络和每一台主机分配一个逻辑地址，以此来屏蔽物理地址的差异。<br />IP地址分为IPV4和IPV6，一般所说的的IP地址指的是IPV4的地址。<br />IPV4互联协议版本4，有版本V4之前就有IPV1 IPV2IPV3，同样有IPV5 IPV6，IPV5在实验中已经夭折，替代IPV4的将是IPV6。(IPV4地址比如：192.168.1.1、114.114.114.114，IPV6地址比如：2001::1）<br />![2021-05-23-09-34-55-639251.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621733750470-8e27f06f-dbfe-4d5a-a0f6-b4a41ebd9a27.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=uff420f6e&originHeight=301&originWidth=548&originalType=binary&size=16806&status=done&style=shadow&taskId=u39784fcf-94fa-4eae-bd16-c2f4f9b82d2)<br />注：接下来说的IP指的是IPV4地址
<a name="fETYf"></a>
### 1、IP是电子设备间的名字。<br />![2021-05-23-09-34-55-741977.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621733844436-16bfd179-0ec0-436a-b2d4-11c112585b5e.png#clientId=u97efa6b3-7361-4&from=ui&id=u3ab2406c&originHeight=231&originWidth=565&originalType=binary&size=23091&status=done&style=shadow&taskId=ubf554572-8031-4b70-93b2-a4704e94c52)
常见的是IP地址是十进制的，在小学学到的十进制逢十进一，说的就是十进制。真正的IP地址到底怎样呢？IP地址是由二进制组成的，但为了方便人的记忆转换为十进制。
<a name="iaD8s"></a>
#### 1.十进制的计算方式
(1010)10 ＝1× 103＋0×102＋1× 101＋0× 100
<a name="VSaZP"></a>
#### 2.二进制的计算方式
(1010)2 ＝1× 23＋0 × 22＋1× 21＋0 ×20
<a name="tuxBI"></a>
#### 3.二进制转换十进制
IP地址由32位二进制组成，转换为十进制的方式让人更容易记忆。<br />![2021-05-23-09-34-56-063117.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621733876613-6d47d4e0-1b1c-408c-a032-612ccdf438d0.png#clientId=u97efa6b3-7361-4&from=ui&id=uc6df1b67&originHeight=183&originWidth=554&originalType=binary&size=25007&status=done&style=shadow&taskId=uff9777d6-e5a9-4b44-b826-ccd09eadc00)
<a name="j1IZb"></a>
#### 4.IP地址的组成
IP地址是由两部分组成的，网络部分和主机部分，比如：<br />![2021-05-23-09-34-56-252610.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621733891220-9dfee3ce-7374-42fe-acbf-30cf4eedbfdb.png#clientId=u97efa6b3-7361-4&from=ui&id=u74db8faa&originHeight=149&originWidth=368&originalType=binary&size=13189&status=done&style=shadow&taskId=u6fb21bcd-dd70-4d84-b535-743698491fd)<br />网络部分如果一样代表在同一个网段（主机部分可以不一样）。什么叫同一个网段（同一个广播域）呢？好比在同一房间的人一样，他们之间通讯可以基本靠吼，也就是所说的广播。不同网段的好比不同房间的他们之间正常情况下不能通讯。
<a name="rms8H"></a>
### 2、二进制转十进制
IP地址分为4组，8bit（8个二进制）一组，4个组组成了32个二进制。<br />二进制1111 1111转换为十进制为255 <br />![2021-05-23-09-34-56-388248.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621733912133-74674d1b-98da-45cc-ba4e-aa973132662a.png#clientId=u97efa6b3-7361-4&from=ui&id=u519bc588&originHeight=245&originWidth=484&originalType=binary&size=26293&status=done&style=shadow&taskId=u579cef4e-fb35-490a-aae7-c7f8e3f7f75)

| 二进制 | 1111 1111 |
| --- | --- |
| 十进制 | 255 |

二进制1110 1001转换为十进制为233

| 二进制 | 1110 1001 |
| --- | --- |
| 十进制 | 233 |

<a name="ah5Bh"></a>
### 3、十进制转二进制
将128除以2得出余数，然后一个个往下除，然后将余数倒叙进行排列<br />![2021-05-23-09-34-56-500946.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621733936384-b9dc2781-c99c-4c64-9d29-bc600e3cf0de.png#clientId=u97efa6b3-7361-4&from=ui&id=u0ef49284&originHeight=362&originWidth=520&originalType=binary&size=22059&status=done&style=shadow&taskId=u0b29658b-6586-4f06-b013-4ecfdbdbf41)
<a name="hNLdO"></a>
#### 进制转换计算器方式
计算器→查看→科学型<br />![2021-05-23-09-34-56-808123.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621733953017-0d216d33-6b38-4576-8931-0395ad683cf9.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=u5297659d&originHeight=321&originWidth=574&originalType=binary&size=37877&status=done&style=none&taskId=u5398e441-e9bf-4622-ae97-91e622838db)<br />选择十进制，输入255<br />点击二进制，这时候就将十进制转换为二进制。二进制转十进制一样的道理。
<a name="A0wEz"></a>
## IP地址的类型
IP地址类型分为A、B、C、D、E。<br />A、B、C类：生活中常用的类型的IP地址。<br />D类：组播地址在路由协议的时候会讲到几个常用的几个，用户组播地址在CCIE中学习。<br />E类：仅供Internet实验和开发<br />注：<br />A类前一位为0，B类前两位为10（其他位任意）；C类前三位为110（其他位任意）；D类前四位为1110（其他位任意）；E类前五位为11110（其他位任意），其中127和0开头的为特殊地址；<br />![2021-05-23-09-34-57-105329.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621733986468-b2721c07-4970-41c9-a0ef-cd996a2797c1.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=u12f12ec7&originHeight=388&originWidth=655&originalType=binary&size=40320&status=done&style=shadow&taskId=u3d456253-a75e-4b72-8593-a1792ec05a6)
<a name="FdMIR"></a>
### 1、A、B、C三类IP地址的组成
网络部分（NETWORK）<br />主机部分（HOST） <br />![2021-05-23-09-34-57-219025.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621734002926-cb860616-6160-42f3-a17a-b07de968076b.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=uf9129196&originHeight=243&originWidth=657&originalType=binary&size=33340&status=done&style=shadow&taskId=u1b7b3019-bfff-4f46-b664-fb3bab02759)
<a name="Jd2tR"></a>
### 2、IP地址的范围和作用思维导图如下<br />![2021-05-23-09-34-57-326739.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621734021503-09efadef-def5-4352-9492-106ab5fe6f0b.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=u87dc1b8c&originHeight=485&originWidth=624&originalType=binary&size=38555&status=done&style=shadow&taskId=u3af15d96-41e8-4409-adb9-0ab8a126ba0)
<a name="rMcom"></a>
### 3、IP地址中的特殊地址
![2021-05-23-09-34-57-436442.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621734039700-83e3e8fa-d0cf-43aa-8fd6-b8e1dff43c08.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=u44227454&originHeight=349&originWidth=550&originalType=binary&size=31971&status=done&style=shadow&taskId=u758c3806-9b29-4a4e-b4ea-d722fbe725f)
<a name="JhPrL"></a>
#### 4、公网地址和私网地址
![2021-05-23-09-34-57-540166.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1621734054323-844a0e1e-48cb-4516-a1db-eafe954b626f.jpeg#clientId=u97efa6b3-7361-4&from=ui&id=uf0ae0526&originHeight=250&originWidth=566&originalType=binary&size=18586&status=done&style=shadow&taskId=u9cc9b136-a6e0-4d82-a6b2-f91e28eea51)
