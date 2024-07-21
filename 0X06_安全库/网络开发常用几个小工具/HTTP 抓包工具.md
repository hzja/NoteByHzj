抓包工具将自身当作代理，能够抓取浏览器到服务器之间的通讯，并提供修改、重放、批量执行的功能。是发现问题，分析协议，攻击站点的利器。常用的有以下三款：
<a name="dc1ef4a7"></a>
## Burpsuite（跨平台)
<a name="njw7d"></a>
### 官网
[https://portswigger.net/burp](https://portswigger.net/burp)
<a name="OUgQp"></a>
## Fiddle2 (Win)
<a name="Mbk9e"></a>
### Filddler Everywhere（推荐）
<a name="hknDN"></a>
#### 官网
[https://www.telerik.com/fiddler](https://www.telerik.com/fiddler)
<a name="OQwwG"></a>
#### 历史版本下载
[https://www.telerik.com/support/whats-new/fiddler-everywhere/release-history](https://www.telerik.com/support/whats-new/fiddler-everywhere/release-history)
<a name="weDh7"></a>
### Fiddler（不适应高分辨率显示）
[FiddlerSetup.zip](https://www.yuque.com/attachments/yuque/0/2020/zip/396745/1602841871613-ebfb7fe8-3ed8-45ef-95b1-c03b46ed27e5.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fzip%2F396745%2F1602841871613-ebfb7fe8-3ed8-45ef-95b1-c03b46ed27e5.zip%22%2C%22name%22%3A%22FiddlerSetup.zip%22%2C%22size%22%3A6533838%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22uid%22%3A%221602841838590-1%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22mode%22%3A%22title%22%2C%22id%22%3A%2270qvr%22%2C%22card%22%3A%22file%22%7D)
<a name="edqvR"></a>
## Charles
Charles3免费，Charles4收费💳
<a name="WqczL"></a>
### 官网
[https://www.charlesproxy.com/](https://www.charlesproxy.com/download/)
<a name="SWsbI"></a>
### Charles手机抓包步骤
<a name="kHSKS"></a>
#### 1、安装
<a name="AGIxJ"></a>
#### 2、设置代理
打开Charles，菜单栏Proxy->Proxy setting，设置端口号，勾上Enable transparent HTTP proxying<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633786948717-8737ff97-9736-416f-ad6e-5099b3852e62.png#averageHue=%23f0f0ef&clientId=u956a095e-a53c-4&from=paste&height=580&id=ucd5f2f06&originHeight=1739&originWidth=2218&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226690&status=done&style=shadow&taskId=u8fe55e5c-dd9b-4c01-89f4-3bd5c1e0fde&title=&width=739.3333333333334)
<a name="pDR4J"></a>
#### 3、配置https
打开Charles，菜单栏Help->SSL proxying，选择Install Charles Root Certificate<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695045212326-6fe7c66e-d594-4fb4-9d96-b9f6acb1c9ab.png#averageHue=%23f2f2f1&clientId=u9738eb38-03ad-4&from=paste&height=526&id=ufcf030c4&originHeight=1314&originWidth=2454&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=180027&status=done&style=none&taskId=u9a81dc82-d7ce-4e6f-82b2-a69c9b0f642&title=&width=981.6)<br />下载证书，存放路径选择：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633787152975-5eb59d2a-9763-4510-8b71-3567d0813cf6.png#averageHue=%23f8f8f7&clientId=u956a095e-a53c-4&from=paste&height=527&id=uc4111244&originHeight=1580&originWidth=1530&originalType=binary&ratio=1&rotation=0&showTitle=false&size=394274&status=done&style=shadow&taskId=ue3c2bd23-d9a6-4e81-8892-703825ba26b&title=&width=510)<br />下载手机证书，菜单栏Help->SSL proxying，选择Install Charles Root Certificate on a Mobile .....<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695045232858-f8766fcb-b3b6-47ed-817b-f51b11d2753a.png#averageHue=%23f2f2f1&clientId=u9738eb38-03ad-4&from=paste&height=483&id=u4403464b&originHeight=1207&originWidth=2400&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=171474&status=done&style=none&taskId=ue6a85cfc-4f12-461e-a2fa-c1b80e30bd3&title=&width=960)
<a name="yiQCB"></a>
#### 4、手机设置
手机设置代理：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633787206752-686edd71-a318-48ca-9d6b-e80efbdfee21.png#averageHue=%23afd2f2&clientId=u956a095e-a53c-4&from=paste&id=udfbf7dca&originHeight=221&originWidth=287&originalType=url&ratio=1&rotation=0&showTitle=false&size=35364&status=done&style=none&taskId=u4b648bf6-0142-4e2f-9524-f804cf38c69&title=)<br />手机打开浏览器，输入chls.pro/ssl，下载证书，安装，如果是iphone，打开设置-》通用-》描述文件-》安装，安装完打开设置-》通用-》关于本机-》证书信任设置-》打开
