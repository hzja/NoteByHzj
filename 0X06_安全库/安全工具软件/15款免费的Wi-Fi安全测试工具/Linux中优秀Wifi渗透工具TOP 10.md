网络安全
<a name="HBvQ0"></a>
## 1、Aircrack-ng
Aircrack是目前WEP/WPA/WPA2破解领域中最热门的工具，Aircrack-ng套件包含的工具能够捕捉数据包和握手包，生成通信数据，或进行暴力破解攻击以及字典攻击。Aircrack-ng是一款多合一整合套件，该套件大致包含下列几种工具：

- Aircrack-ng：无线密码破解
- Aireplay：生成网络数据，去客户端验证
- Airodump-ng：数据包捕捉
- Airbase-ng：配置伪造的接入点

Aircrack-ng可以在Linux、Windows和macOS上运行，并且KaliLinux默认自带了该套件。如果准备使用这款工具，那么还需要确保自己的无线网卡具备数据包注入功能。
<a name="BYkdF"></a>
## 2、Reaver
排名第二的是Reaver，Reaver也是目前流行的无线网络攻击工具，它主要针对的是WPS漏洞。Reaver会对WiFi保护设置（WPS）的注册PIN码进行暴力破解攻击，并尝试恢复出WPA/WPA2密码。由于很多路由器制造商和ISP会默认开启WPS功能，因此市面上的很多路由器都无法抵御这种攻击。<br />在使用Reaver时，无线路由器的信号一定要足够强。平均来说，Reaver可以在4-10个小时之内破解目标路由器的密码，具体破解时长还要根据接入点类型、信号强度和PIN码本身来判断。从概率论和统计学的角度来看，有50%的机会只需要花一半时间就能够破解出目标路由器的PIN码。
<a name="SuqZF"></a>
## 3、Pixiewps
PixieWPS是Kali Linux新加入的一款专门针对WPS漏洞的渗透工具。PixieWPS使用C语言开发，可以用来离线爆破WPS PIN码。它所使用的技术名叫pixie dust攻击，需要注意的是，PixieWPS需要一个修改版的Reaver或Wifite才能正常运行。由于这款工具在短时间内就变得非常受欢迎了，因此它在排行榜上排到了第三名。
<a name="otweJ"></a>
## 4、Wifite
Wifite是一款能够攻击多种无线加密方式（WEP/WPA/WPA2和WPS）的自动化工具， Wifite在运行之前需要提供几个参数，而Wifite将会自动完成所有的任务。它可以捕获WPA握手包，自动化去客户端验证，进行MAC地址欺骗，以及破解WiFi密码。
<a name="BleJW"></a>
## 5、Wireshark
Wireshark算得上是目前最好的网络协议分析工具了，在Wireshark的帮助下，能够深入地分析一个网络，并获取到尽可能多的信息。Wireshark能够实时捕获网络数据包、对上百种协议进行深入分析、以及浏览和过滤数据包。需要注意的是，Wireshark是一款跨平台工具。<br />KaliLinux默认自带了Wireshark，不过Windows和macOS用户都可以使用它。如果想使用Wireshark的某些特殊功能，那么需要一款支持监听模式的WiFi适配器。
<a name="lwwYr"></a>
## 6、oclHashcat
oclHashcat并不是一款专用的WiFi入侵工具，而且它也不是Kali Linux的自带工具，但它能够对捕捉到的握手包进行高速爆破攻击和字典攻击（使用GPU跑字典）。在使用了Aircrack-ng或其他工具捕捉到了握手包之后，就可以使用GPU和oclHashcat来破解WiFi密码了，而且GPU+oclHashcat的破解速度要比CPU+Aircrack-ng的速度快得多。<br />oclHashcat适用于Windows和Linux，并且还有专门针对AMD和Nvidia显卡的版本。
<a name="uIFJy"></a>
## 7、Fern Wifi Cracker
Fern Wifi Cracker是一款针对无线网络的安全审计工具和攻击工具，该工具采用Python编写，它也是这十大无线攻击工具中唯一一款具有图形用户界面的工具。Fern Wifi Cracker能够破解WEP、WPA和WPS密钥，并且还包含有能够执行MiTM（中间人攻击）攻击的组件。
<a name="IggSx"></a>
## 8、Wash
当攻击者在对WPS PIN码进行暴力破解攻击时，很多接入点会将自己锁定以保证路由器的安全性。而Wash就是一款能够确定目标接入点是否开启了WPS的工具，在使用Reaver进行了多次尝试之后，可以用Wash来检查接入点是否锁定了WPS。
<a name="l4SE8"></a>
## 9、Crunch
Crunch可以用来生成自定义字典，而生成的字典可以用于字典攻击。由于字典攻击的成功率依赖于所使用字典的质量，因此总是不可避免地要根据自己的需要来生成字典。
<a name="vGaHH"></a>
## 10、Macchanger
实际上，Macchanger是一款能够将设备的MAC地址改成随机值的小工具，当然了，也可以将其修改为想要的数据。为了避免目标设备采用了MAC地址过滤，所以对于WiFi攻击来说，MAC地址修改就显得非常重要了。
