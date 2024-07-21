网络抓包工具<br />Burp Suite，简称 BP 工具，一般作为渗透测试工具，是一款用于攻击 Web 应用程序的集成平台<br />主要功能模块包含：

- Proxy 模块抓包、数据拦截及数据包修改
- Target 模块Site map 站点记录及分析
- Intruder 模块暴力破解
- Repeater 模块重放工具
- Decoder 模块编/解码工具
- Comparer 模块内容比对

官网地址：[https://portswigger.net/burp](https://portswigger.net/burp)
<a name="iGIyP"></a>
## 1、破解
根据系统类型，下载对应版本的 BP 应用软件<br />PS：推荐下载 Professional 专业版<br />下载地址：[https://portswigger.net/burp/releases](https://portswigger.net/burp/releases)<br />破解 BP 工具有多种方式，这里推荐使用 BurpLoaderKeygen 来进行注册<br />破解补丁：[https://github.com/h3110w0r1d-y/BurpLoaderKeygen](https://github.com/h3110w0r1d-y/BurpLoaderKeygen)<br />注册方法可以查看项目 readme 来进行，需要注意的是，破解的前提是本机为 JDK9+<br />BP 软件主界面图如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472163-383adb38-f457-4f39-bdd0-6e46161fc823.png#averageHue=%23f9f8f8&clientId=ufcec889f-1ab0-4&from=paste&id=uc6998795&originHeight=680&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua08202d2-3db8-44df-87da-68850292865&title=)
<a name="hdOmA"></a>
## 2、安装配置证书
这里以 Mac、iPhone 为例进行讲解
<a name="EeSOk"></a>
### 2-1 Mac 安装证书
为了抓取 https 的数据包，需安装配置证书<br />首先，从 BP 工具中导出「 der 」格式的证书文件<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472086-41757de0-c561-43d7-8bb3-99b2b0b4de1b.png#averageHue=%23f3f2f2&clientId=ufcec889f-1ab0-4&from=paste&id=u2eb6d281&originHeight=695&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc772f877-0248-42aa-8bd0-09c0244b37a&title=)<br />然后安装证书<br />最后，并在钥匙串中配置证书的访问权限即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472209-e8b32057-67b0-47f1-a377-83f8a901534c.png#averageHue=%23f5f4f4&clientId=ufcec889f-1ab0-4&from=paste&id=ub6d3ac8f&originHeight=1181&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9e7b0596-04e1-4c4d-8ea2-e5cc7113e99&title=)
<a name="UfWG5"></a>
### 2-2  iphone 安装证书
首先，只需要将上面的证书文件导入到手机中（微信文件传输助手、隔空投送等）<br />然后，在「 通用 - 设备管理 - PortSwigger CA」安装证书<br />最后，在「 通用 - 关于本机 - 证书信任设置 - 选中信任 PortSwigger CA 证书 」
<a name="DroDO"></a>
## 3、抓包
下面聊聊几种常见的抓包场景
<a name="hRT0a"></a>
### 3-1 Chrome 浏览器
首先配置 BP 工具 Proxy Listeners 信息，指定监听地址及端口号<br />这里默认监听本机及 8080 端口号<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472143-fc03dc37-d556-45f8-97a6-2777210da337.png#averageHue=%23f3f1f0&clientId=ufcec889f-1ab0-4&from=paste&id=ua215efac&originHeight=701&originWidth=921&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7035ea33-a175-4c0b-86ac-4b54ce51a19&title=)<br />然后，在 Chrome 安装一款插件「 Proxy SwitchyOmega 」，并且新建一个情景模式，并启用它<br />该情景模式地址与端口号与 BP 工具中保持一致<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472136-e17482de-0724-4f6a-920f-00ca8709b1ef.png#averageHue=%23fbfbfb&clientId=ufcec889f-1ab0-4&from=paste&id=u5cd5aa82&originHeight=457&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec9c4786-8da6-4ea2-9161-9edf94f2aa0&title=)<br />接着，在 BP 工具 Proxy 模块下的 Intercept Tab 标签下，关闭数据拦截功能，即：Intercept is off<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472685-41af6508-f16b-4eaf-8b50-ebaa928d013d.png#averageHue=%23f6f5f5&clientId=ufcec889f-1ab0-4&from=paste&id=u275a59e8&originHeight=584&originWidth=783&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u339c389c-ca32-4f01-bfc8-ab74515c69c&title=)<br />最后，访问某一个网站，在 BP 工具 Proxy 模块下的 HTTP history Tab 标签下就展示了数据包列表<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472779-8ae26729-e861-4c51-8b09-306c6dd1e2af.png#averageHue=%23f1efee&clientId=ufcec889f-1ab0-4&from=paste&id=u85ed4707&originHeight=680&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u40a82b1b-9574-4ca5-b63d-4ce46ccc528&title=)
<a name="cISlv"></a>
### 3-2  PC 端小程序
首先，根据 BP 工具配置的 Proxy Listeners 信息，在网络管理中配置代理<br />即：网页代理（HTTP）、安全网页代理（HTTPS）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509472978-15ff32d3-0ce9-4cfc-9ee3-07f7993ed8aa.png#averageHue=%23f5f5f5&clientId=ufcec889f-1ab0-4&from=paste&id=uac0d1074&originHeight=838&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u41bc43ee-6cab-4af3-8103-23aa52a2315&title=)<br />然后，保证 BP 工具的数据拦截功能已经关闭<br />最后，在 PC 微信中打开小程序，数据包都会在 BP 工具中展示出来<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509474151-a10269ed-cc53-4101-81e2-1ef97ecde8c5.png#averageHue=%23efecea&clientId=ufcec889f-1ab0-4&from=paste&id=ue0153a72&originHeight=681&originWidth=1057&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufa761e81-a183-4d01-8cd4-09ff0fc510e&title=)
<a name="r31L2"></a>
### 3-3 App 应用
首先，查看 PC 端实际的 ip 地址，配置 BP 工具代理信息为该 ip 地址，端口号保持默认 8080<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509473247-d087a2f8-53f8-43c3-957a-45e5fcaad3d3.png#averageHue=%23f4f3f2&clientId=ufcec889f-1ab0-4&from=paste&id=ud3e1ec7f&originHeight=642&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9e95e4e-e8b3-4c27-a80b-0ebe931d1b0&title=)<br />然后，保证手机和 PC 在同一局域网，配置代理为手动模式<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654509473453-065b9324-0246-4b14-b35c-5a2c12f4d0d0.jpeg#averageHue=%23f3f9f3&clientId=ufcec889f-1ab0-4&from=paste&height=448&id=u4d505f6e&originHeight=927&originWidth=828&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uad8d254b-2910-4231-aea4-adf1fca961d&title=&width=400.3999938964844)<br />这样，App 端的数据包都会展示在 BP 工具 Proxy 列表中
<a name="N9oNI"></a>
### 3-4  补充一下
Proxy 模块提供了过滤功能，可以通过关键字、正则、响应类型、响应码等对数据进行过滤<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509473832-0c5632f9-8052-45ab-88a6-bca32a89c130.png#averageHue=%23efedec&clientId=ufcec889f-1ab0-4&from=paste&id=ub5d52a3e&originHeight=574&originWidth=935&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udade9297-65bc-4d57-8cb6-ee88165861b&title=)
<a name="mvmLK"></a>
## 4、拦截及数据修改
和 Charles、FD 类似，BP 工具同样也支持数据拦截及修改功能<br />操作步骤如下：<br />首先，在 Proxy 设置选项卡中开启请求拦截、响应拦截，并配置拦截条件<br />PS：通过 Domain、IP、URL 及布尔运算符自定义拦截规则<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654509473937-2a2bc30c-6f5f-459c-ad99-a0cde235b15f.png#averageHue=%23f4f4f3&clientId=ufcec889f-1ab0-4&from=paste&id=u1050a17c&originHeight=547&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u990b7a87-b421-4fb0-9a8d-f45a4dac5c4&title=)<br />然后，在 Intercept Tab 面板开启拦截功能，即：Intercept is on<br />最后访问目标地址，在 Intercept Tab 面板对请求参数、响应内容进行修改，结合 Forward（前进）、Drop（不处理）功能按钮，来完成对数据的拦截及修改的目的<br />除了抓包，BP 工具还内置了很多实用的功能。
