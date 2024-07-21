IP
<a name="s6Tsy"></a>
## Fav-up
Fav-up是一款功能强大的IP查询工具，该工具可以通过Shodan和Favicon（网站图标）来帮助研究人员查询目标服务或设备的真实IP地址。
<a name="LICHZ"></a>
## 工具安装
首先，该工具需要本地设备安装并部署好Python 3环境。然后广大研究人员需要使用下列命令将该项目源码克隆至本地：
```bash
git clone https://github.com/pielco11/fav-up.git
```
接下来， 运行下列命令安装好Fav-up所需的依赖组件：
```bash
pip3 install -r requirements.txt
```
除此之外，还需要一个Shodan API密钥！
<a name="kqeL4"></a>
## 工具使用
<a name="qc6IE"></a>
### 命令行接口
首先，需要确定如何传递API密钥：<br />`-k`或—key：向stdin传递密钥；<br />`-kf`或—key-file：传递获取密钥的目标文件名；<br />`-sc`或—shodan-cli：从Shodan命令行接口获取密钥；<br />配置好密钥之后，就能够以下列几种不同方式使用Fav-up了：<br />`-f`或—favicon-file：在本地存储的需要查询的Favicon网站图标文件；<br />`-fu`或—favicon-url：无需在本地存储Favicon网站图标，但是需要知道目标图标的实际URL地址；<br />`-w`或—web：如果不知道Favicon网站图标的实际URL，可以直接传递目标站点地址；<br />`-fh`或—favicon-hash：在全网搜索Favicon网站图标哈希；<br />可以指定包含了Favicon网站图标的URL和域名的输入文件，或者直接提供Favicon网站图标的本地存储路径：<br />`-fl`或—favicon-list：文件包含所有待查询Favicon网站图标的完整路径；<br />`-ul`或—url-list：文件包含所有待查询Favicon网站图标的完整URL地址；<br />`-wl`或—web-list：<br />当然了，也可以将搜索结果存储至一个CSV/JSON文件中：<br />`-o`或—output：指定数据输出文件和格式，比如说csv，它会将存储结果存储至一个CSV文件中；
<a name="GWiFw"></a>
## 工具使用样例
<a name="X8e4m"></a>
### Favicon-file
```bash
python3 favUp.py --favicon-file favicon.ico -sc
```
<a name="cZjHO"></a>
### Favicon-url
```bash
python3 favUp.py --favicon-url https://domain.behind.cloudflare/assets/favicon.ico -sc
```
<a name="dyfbY"></a>
### Web
```bash
python3 favUp.py --web domain.behind.cloudflare -sc
```
<a name="ZfSzx"></a>
## 作为模块导入使用
```python
from favUp import FavUp
f = FavUp()          
f.shodanCLI = True
f.web = "domain.behind.cloudflare"
f.show = True
f.run()
for result in f.faviconsList:
    print(f"Real-IP: {result['found_ips']}")
print(f"Hash: {result['favhash']}")
```
<a name="i4vK7"></a>
## 所有属性
| 变量 | 类型 |
| --- | --- |
| `FavUp.show` | 布尔 |
| `FavUp.key` | 字符串 |
| `FavUp.keyFile` | 字符串 |
| `FavUp.shodanCLI` | 布尔 |
| `FavUp.faviconFile` | 字符串 |
| `FavUp.faviconURL` | 字符串 |
| `FavUp.web` | 字符串 |
| `FavUp.shodan` | Shodan 类 |
| `FavUp.faviconsList` | 列表[字典] |

<a name="ofkhb"></a>
## 工具使用截图
![2021-05-30-18-56-02-856956.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622372345013-1501eca5-a920-4a68-84b8-90d768a28611.jpeg#clientId=u6d480878-6e19-4&from=ui&id=u3a9d9c62&originHeight=341&originWidth=519&originalType=binary&size=23037&status=done&style=none&taskId=u595b7200-2735-42fc-a358-64a9bb3aa02)![2021-05-30-18-56-02-954438.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622372345036-409b2def-d95e-4b09-9e27-5d293401e03d.jpeg#clientId=u6d480878-6e19-4&from=ui&id=ucb117518&originHeight=200&originWidth=690&originalType=binary&size=15838&status=done&style=none&taskId=u636bd38c-43c9-4286-a2a6-af6b27e8ef2)
<a name="mKW46"></a>
## 项目地址
Fav-up：[https://github.com/pielco11/fav-up](https://github.com/pielco11/fav-up)
