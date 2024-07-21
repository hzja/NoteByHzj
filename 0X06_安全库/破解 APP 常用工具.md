黑客 
<a name="fCZ7n"></a>
## 抓包
<a name="lPwxO"></a>
### HTTP请求类

- Fiddler/Charles**（必备常用工具之一）**
- ProxyDroid**（必备常用工具之一）**
- PacketCapture/HTTPCanary**（必备常用工具之一）**
- JustTrustMe**（必备常用工具之一）**
- ObjectionUnpinningPlus**（必备常用工具之一）**

功能与JustTrustMe相同，但因为Frida的特殊性，可以随时修改内容，面对一些特殊情况时会很方便。

- WireShark

也是一个很常见的抓包工具，但由于工作方式过于底层，对HTTPS请求的处理比较麻烦，一般不建议对使用HTTP协议的APP使用。
<a name="W9kia"></a>
### 非HTTP请求类
<a name="PzCxk"></a>
#### WireShark
非HTTP的还是使用WireShark这类工具方便些，通常需要配合反编译找到协议的组成方式。<br />建议使用方式：电脑端开热点，然后指定用于创建热点的虚拟网卡，再把手机连上热点开始抓包。
<a name="pWCco"></a>
#### Tcpdump
在使用没有无线网卡的电脑或无法开热点的情况下可以直接在手机上运行Tcpdump然后导出文件在电脑端WireShark中打开，与直接使用WireShark抓包效果相同。
<a name="gPFgr"></a>
## 破解加密参数
<a name="hDrlZ"></a>
### Java层
<a name="RJhrn"></a>
#### dex-tools
将脱壳后的dex文件转jar文件，配合jd-gui查看源代码。<br />开源地址：[https://github.com/pxb1988/dex2jar](https://github.com/pxb1988/dex2jar)<br />[dex-tools-2.2-SNAPSHOT-2021-10-31.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1653665497789-1166955a-d3a3-4149-a544-9208b662af92.zip)<br />用法（脱壳后生成的多个dex文件转成jar后，可能每个都要看一下能不能找到APP对应的包路径）：
```bash
.\d2j-dex2jar.bat cookie5.dex
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1653665925325-84c49908-3475-4027-8c8e-9bfa3f089696.png#clientId=u98bc60ba-d475-4&from=paste&height=128&id=udff32d93&originHeight=319&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=289041&status=done&style=shadow&taskId=ubc4d95aa-8d56-4d48-99ac-05475d70ea7&title=&width=1329.2)
<a name="jiX3w"></a>
#### jd-gui
查看jar包的代码内容。<br />开源地址：[https://github.com/java-decompiler/jd-gui](https://github.com/java-decompiler/jd-gui)<br />[jd-gui-windows-1.6.6.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1653665496723-8f2368bf-e707-4cb1-8e11-52d6384ef3f5.zip)
<a name="kppX4"></a>
#### Jadx（必备常用工具之一）
一个非常方便的Java反编译工具，一般用到的功能主要是搜索、反混淆、查找方法调用这几样，性能和反编译出来的代码效果都比使用dex2jar+jd-gui之类的方式好。<br />[https://github.com/skylot/jadx](https://github.com/skylot/jadx)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651679682858-e8cd5cf0-1094-4c17-b993-747be37b2773.png#clientId=u45f8a90c-7371-4&from=paste&height=632&id=u93895534&originHeight=1581&originWidth=2746&originalType=binary&ratio=1&rotation=0&showTitle=false&size=74801&status=done&style=shadow&taskId=uff6a48b9-87c5-4f29-88c3-b4566a7affc&title=&width=1098.4)
<a name="F6CoJ"></a>
#### Xposed（必备常用工具之一）
Xposed框架大家应该都知道吧？这是一个功能十分强大的Hook框架，很多逆向工具都是基于它来写的，有特殊需求时也可以自己写一个模块使用。
<a name="XgfXO"></a>
#### Frida（必备常用工具之一）
相对于Xposed而言，Frida算是一个在安全圈外没有那么高知名度的Hook工具了，但它的功能在某些方面要比Xposed强得多（当然也有缺点），举个常用到的例子：用它来Hook So库中的函数~。
<a name="IMHxv"></a>
#### inspeckage（必备常用工具之一）
这是一个基于Xposed写的动态分析工具，Hook了大量逆向时常见的方法，下面是它的GitHub中给出的列表：
```
Shared Preferences (log and file);
Serialization;
Crypto;
Hashes;
SQLite;
HTTP (an HTTP proxy tool is still the best alternative);
File System;
Miscellaneous (Clipboard, URL.Parse());
WebView;
IPC;
Hooks (add new hooks dynamically)
```
注意它Hook列表中有Crypto和Hash，这两个类型在破解大部分APP的加密参数时可以说是降维打击，因为大部分APP的加密参数都逃不过MD5、SHA1、AES、DES这四种，而它们都被Hook了（不仅仅只有这四种）。基本上就是打开Inspeckage再打开它的Web端，然后打开指定的APP操作一下，一个搜索，加密参数就原形毕露了。
<a name="UN4al"></a>
#### DeveloperHelper
一个基于Xposed写的辅助工具，通常会用到的功能是查看Activity名、查看加固类型、查看Activity结构、自动脱壳这几个。
<a name="lhrRT"></a>
#### UCrack
也是一个基于Xposed写的辅助工具，集成了自动网络抓包、网络堆栈爆破、文件日志、WebView调试环境、自动脱壳、Native函数注册监控、记录程序自杀堆栈等功能，这个工具是之前偶然发现的，还没有使用过，有兴趣的同学可以用用看。
<a name="jEyGG"></a>
### C/C++层（So库）
<a name="fV6qB"></a>
#### IDA（必备常用工具之一）
[https://hex-rays.com/ida-free/#download](https://hex-rays.com/ida-free/#download)<br />非常强大的反汇编和动态调试工具，强烈不推荐使用NSA开源的Ghidra，效果跟IDA比起来差太多了。IDA可以在反汇编之后将汇编代码转成伪C代码，并且能在手机端启动了服务端之后注入APP进程使用动态调试功能。
<a name="wvZaN"></a>
#### Frida（必备常用工具之一）

- 有壳（加固）的
- DeveloperHelper
- UCrack
- FDex2

其实就是把几行代码包了一层而已，原理就是Hook `ClassLoader`的`loadClass`方法，反射调用`getDex`方法取得`Dex(com.android.dex.Dex类对象)`，再将里面的dex写出。 
<a name="P6ZqZ"></a>
## 其他工具
[https://crifan.github.io/android_app_security_crack/website/](https://crifan.github.io/android_app_security_crack/website/)
