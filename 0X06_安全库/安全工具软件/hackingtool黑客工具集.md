hackingtool
<a name="W6SLN"></a>
# 1、项目地址
[hackingtool仓库地址](https://github.com/Z4nzu/hackingtool)<br />[https://github.com/Z4nzu/hackingtool](https://github.com/Z4nzu/hackingtool)
<a name="Mnrad"></a>
# 2、工具包含
Hackingtool是一款针对渗透测试人员的多合一测试套件，目前该套件可以给广大安全研究人员提供下列菜单内的工具：
> -AnonSurf匿名保护工具
> -信息收集工具
> -密码攻击工具
> -无线攻击工具
> -SQL注入工具
> -网络钓鱼攻击工具
> -Web攻击工具
> -后渗透工具
> -信息取证工具
> -Payload创建工具
> -路由漏洞利用工具
> -WiFi干扰工具
> -XSS攻击工具
> -逆向工程工具
> -社交媒体查询工具
> -DDoS攻击工具
> -隐写工具
> -IDN Homograph攻击工具
> -哈希破解工具
> -社交媒体攻击工具
> -Android攻击工具
> -RAT工具
> -Web爬虫
> -Payload注入工具
> -系统更新工具

<a name="h2-1"></a>
## AnonSurf匿名保护工具
Anonmously Surf<br />Multitor
<a name="h2-2"></a>
## 信息收集工具
Nmap<br />Dracnmap<br />Port Scanning<br />Host To IP<br />Xerosploit<br />Infoga - Email OSINT<br />ReconSpider<br />RED HAWK (All In One Scanning)<br />ReconDog<br />Striker<br />SecretFinder<br />Port Scanne<br />Breacher
<a name="h2-3"></a>
## 密码攻击工具
Cupp<br />WordlistCreator<br />Goblin WordGenerator<br />Credential reuse attacks<br />Wordlist (Contain 1.4 Billion Pass)
<a name="h2-4"></a>
## 无线攻击工具
WiFi-Pumpkin<br />pixiewps<br />Bluetooth Honeypot GUI Framework<br />Fluxion<br />Wifiphisher<br />Wifite<br />EvilTwin
<a name="h2-5"></a>
## SQL注入工具
sqlmap tool<br />NoSqlMap<br />Damn Small SQLi Scanner<br />Explo<br />Blisqy - Exploit Time-based blind-SQL injection<br />Leviathan - Wide Range Mass Audit Toolkit<br />SQLScan
<a name="h2-6"></a>
## 社交媒体攻击工具
Instagram Attack<br />AllinOne SocialMedia Attack<br />Facebook Attack<br />Application Checker
<a name="h2-7"></a>
## Android攻击工具
Keydroid<br />MySMS<br />Lockphish (Grab target LOCK PIN)<br />DroidCam (Capture Image)<br />EvilApp (Hijack Session)
<a name="h2-8"></a>
## 网络钓鱼攻击工具
Setoolkit<br />SocialFish<br />HiddenEye<br />Evilginx2<br />Shellphish<br />BlackEye<br />I-See-You(Get Location using phishing attack)<br />SayCheese (Grab target's Webcam Shots)<br />QR Code Jacking
<a name="h2-9"></a>
## Web攻击工具
SlowLoris<br />Skipfish<br />SubDomain Finder<br />CheckURL<br />Blazy<br />Sub-Domain TakeOver
<a name="h2-10"></a>
## 后渗透工具
Vegile - Ghost In The Shell<br />Chrome Keylogger
<a name="h2-11"></a>
## 信息取证工具
Bulk_extractor<br />Disk Clone and ISO Image Aquire<br />AutoSpy<br />Toolsley<br />Wireshark
<a name="h2-12"></a>
## Payload生成器
The FatRat*<br />Brutal<br />Stitch<br />MSFvenom Payload Creator<br />Venom Shellcode Generator<br />Spycam<br />Mob-Droid
<a name="h2-13"></a>
## 漏洞利用框架
RouterSploit<br />WebSploit<br />Commix<br />Web2Attack<br />Fastssh
<a name="h2-14"></a>
## 社交媒体查询工具
Find SocialMedia By Facial Recognation System<br />Find SocialMedia By UserName<br />Sherlock<br />SocialScan
<a name="h2-15"></a>
## 信息隐藏工具
SteganoHide<br />StegnoCracker<br />Whitespace
<a name="h2-16"></a>
## DDoS攻击工具
SlowLoris<br />SYN Flood DDoS Weapon<br />UFOnet<br />GoldenEye
<a name="h2-17"></a>
## XSS攻击工具
DalFox(Finder of XSS)<br />XSS Payload Generator<br />Advanced XSS Detection Suite<br />Extended XSS Searcher and Finder<br />XSS-Freak<br />XSpear<br />XSSCon<br />XanXSS
<a name="h2-18"></a>
## IDN Homograph工具
EvilURL
<a name="h2-19"></a>
## 电子邮件验证工具
KnockMail
<a name="h2-20"></a>
## 哈希破解工具
Hash Buster
<a name="vbsXX"></a>
# 3、hackingtool的安装
需要提前安装好Git（也可以在github下载zip包后使用FTP工具上传）以及pip。<br />pip的安装参考<br />[语雀内容](https://www.yuque.com/fcant/linux/kggxu1?view=doc_embed)
<a name="AqAUX"></a>
## A.安装操作过程如下
以下操作需要以root用户进行
```bash
git clone https://github.com/Z4nzu/hackingtool.git
chmod -R 755 hackingtool  
cd hackingtool
sudo pip3 install -r requirement.txt
bash install.sh
sudo hackingtool
```
<a name="o5YpK"></a>
## B.执行pip3安装时报错
<a name="drGQK"></a>
### ①.报错内容
```bash
ModuleNotFoundError: No module named '_ctypes'
```
<a name="Oa7A2"></a>
### ②.解决办法
**回到Python安装处，安装缺少的模块，再执行Python的编译**<br />[语雀内容](https://www.yuque.com/fcant/linux/kggxu1?inner=P7cOO&view=doc_embed)
<a name="9WmPN"></a>
### ③.再次执行pip3进行安装成功执行
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596869224205-c82880fc-99c3-46ea-935b-bb35c140e2bc.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2166611&status=done&style=none&width=1107.6666666666667)
<a name="MQt6w"></a>
## C.修改install.sh中google.com为baidu.com，因为Google访问不了
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596869418454-bd0eaafe-af87-423c-a4e6-60f2ac966d2e.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2102632&status=done&style=none&width=1107.6666666666667)
<a name="o1mlL"></a>
## D.启动成功
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596869632383-cb393e44-99d8-4b47-9957-7d3a3000d80b.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=1769351&status=done&style=none&width=1107.6666666666667)
