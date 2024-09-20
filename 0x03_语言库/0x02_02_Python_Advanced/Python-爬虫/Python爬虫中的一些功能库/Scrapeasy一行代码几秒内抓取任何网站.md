Python爬虫
<a name="s93VJ"></a>
## Scrapeasy
Scrapeasy 是一个 Python 库，可以轻松抓取网页并从中提取数据。它可用于从单个页面抓取数据或从多个页面抓取数据。它还可用于从 PDF 和 HTML 表格中提取数据。<br />Scrapeasy 让你只用一行代码就可以用 python 抓取网站，它非常便于使用并为你处理一切。只需指定要抓取的网站以及想要接收什么样的数据，其余的交给 Scrapeasy。<br />Scrapeasy Python 爬虫在编写时考虑到了快速使用。它提供以下主要功能：

- 一键抓取网站——不仅仅是单个页面。
- 最常见的抓取活动（接收链接、图像或视频）已经实现。
- 从抓取的网站接收特殊文件类型，如 .php 或 .pdf 数据。
<a name="h8WSY"></a>
## **如何使用 Scrapeasy**
<a name="T6yI0"></a>
### **通过 pip 下载**
```bash
$ pip install scrapeasy
```
<a name="ggIGW"></a>
### **使用它**
Scraeasy 考虑到了易用性。首先，从 Scrapeasy 导入网站和页面
```python
from scrapeasy import Website, Page
```
<a name="n7neN"></a>
### **初始化网站**
首先，创建一个新的网站对象。对于这种方式，只需提供主页的 URL。将使用多年前创建的网站的 URL：
```python
web =Website("https://tikocash.com/solange/index.php/2022/04/13/how-do-you-control-irrational-fear-and-overthinking/")
```
<a name="R99R5"></a>
### **获取所有子站点的链接**
现在网站已经初始化，对 tikocash.com 上存在的所有子网站感兴趣，要找出这一点，请让 Web 对象接收所有子页面的链接。
```python
links = web.getSubpagesLinks()
```
根据本地互联网连接和正在抓取的网站的服务器速度，此请求可能需要一段时间，确保不要使用这种非常庞大的方法抓取整个网页。<br />但回到链接获取：通过调用 `.getSubpagesLinks()`，用请求所有子页面作为链接，并将收到一个 URL 列表。
```python
links2 = web.getSubpagesLinks()
```
可能已经注意到缺少典型的 http://www.-stuff。这是没有目的的，并且使你的生活更容易进一步使用链接。但请确保——当真正想在浏览器中或通过请求调用它们时——请在每个链接前面添加 http://www. 。
<a name="LSIL4"></a>
### **查找媒体**
尝试找到指向 fahrschule-liechti.com 放置在其网站上的所有图像的链接。<br />通过调用 `.getImages()` 方法来做到这一点。
```python
images = web.getImages()
```
响应将包括指向所有可用图像的链接。
<a name="pjcys"></a>
### **下载媒体**
现在做一些更高级的事情。喜欢 tikocash.com 在其网站上的图片，所以将它们全部下载到本地磁盘。听起来工作量是不是很大？其实很简单！
```python
web.download("img", "fahrschule/images")
```
首先，定义通过关键字 img 下载所有图像媒体。接下来，定义输出文件夹，图像应保存到的位置。就是这样！运行代码，看看发生了什么。几秒钟之内，就收到了 Tikocash.com 上的所有图片。
<a name="T1aJ7"></a>
### **获取链接**
接下来，找出 tikocash.com 链接到哪些页面。为了获得总体概述，找出它链接到的其他网站，出于这个原因，指定只获取域链接。
```python
domains = web.getLinks(intern=False, extern=False, domain=True)
```
因此，得到了在 tikocash.com 上链接的所有链接的列表。<br />好的，但现在想进一步了解这些链接，如何做到这一点？
<a name="W2JTg"></a>
### **获取链接域**
好吧，更详细的链接只不过是外部链接，所以，做了同样的请求，但这次包括外部，但不包括域。
```python
domains = web.getLinks(intern=False, extern=True, domain=False)
```
在这里，将详细了解所有外部链接。
<a name="HogI7"></a>
### **初始化页面**
好的，到目前为止，已经看到了很多关于网站的东西，但是还没有发现 Page 是做什么的。<br />好吧，如前所述，该页面只是网站中的一个站点，通过初始化W3schools页面，来尝试不同的示例。
```python
w3 = Page("https://www.w3schools.com/html/html5_video.asp")
```
如果还没有猜到，很快就会明白为什么选择了这个页面。
<a name="DD9C6"></a>
### **下载视频**
Scrapeasy 可以在几秒钟内从网页下载视频，来看看如何。
```python
w3.download("video", "w3/videos")
```
是的，仅此而已。只需指定要将所有视频媒体下载到输出文件夹 w3/videos 中，就可以开始了。当然，也可以只收到视频的链接，然后再下载，但这会不太酷。
```python
video_links = w3.getVideos()
```
<a name="cdOcq"></a>
### **下载其他文件类型（如 pdf 或图片）**
现在更笼统地说，下载特殊文件类型，如 .pdf、.php 或 .ico 怎么样？使用通用的 `.get()` 方法接收链接，或使用文件类型作为参数的 `.download()` 方法。
```python
calendar_links = Page("https://tikocash.com").get("php")
```
到这里就完毕。<br />现在下载一些 PDF。
```python
Page("http://mathcourses.ch/mat182.html").download("pdf", "mathcourses/pdf-files")
```
总之，Python 是一种通用语言，只需一行代码即可在几秒钟内抓取任何网站上的内容。<br />因此，这使其成为网络抓取和数据挖掘的强大工具。<br />因此，如果需要从网站中提取数据，Python 是适合你的工具。
