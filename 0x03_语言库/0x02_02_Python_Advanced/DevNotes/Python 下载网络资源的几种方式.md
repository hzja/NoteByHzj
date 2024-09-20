Python
<a name="uqwVD"></a>
### 1、使用requests
可以使用requests模块从一个URL下载文件。<br />考虑以下代码:
```python
import requests
url = 'https://www.python.org/static/img/pythor-logo@2x.png'
myfile = requests.get(url)
open('PythonImage.png', 'wb').write(myfile.content)
```
只需使用requests模块的get方法获取URL，并将结果存储到一个名为“myfile”的变量中。然后，将这个变量的内容写入文件。
<a name="8BChU"></a>
### 2、使用wget
还可以使用Python的wget模块从一个URL下载文件。可以使用pip按以下命令安装wget模块:
```python
pip install wget
```
考虑以下代码，将使用它下载Python的logo图像。
```python
import wget
url = 'https://www.python.org/static/img/pythor-logo@2x.png'
wget.download(url, 'c:/users/LkeGeeks/dowrloads/PythonImage.png')
```
在这段代码中，URL和路径(图像将存储在其中)被传递给wget模块的download方法。
<a name="zs0EC"></a>
### 3、下载重定向的文件
使用requests从一个URL下载文件，该URL会被重定向到另一个带有一个.pdf文件的URL。该URL看起来如下:
```python
https://readthedocs.org/projects/pythore-guide/downloads/pdf/latest/
```
要下载这个pdf文件，使用以下代码:
```python
import requests
url = 'https://readthedocs.org/projects/pythore-guide/downloads/pdf/latest/'
myfile = requests.get(url, allow_redirects=True)
open('c:/users/LkeGeeks/dowrloads/PythonImage.png', 'wb').write(myfile.content)
```
在这段代码中，第一步指定的是URL。然后使用request模块的get方法来获取该URL。在get方法中，将allow_redirects设置为True，这将允许URL中的重定向，并且重定向后的内容将被分配给变量myfile。<br />最后，打开一个文件来写入获取的内容。
<a name="WtqKj"></a>
### 4、分块下载大文件
看下面的代码:
```python
import requests
url = 'https://www.python.org/static/img/pythor-logo@2x.png'
myfile = requests.get(url)
open('c:/users/LkeGeeks/dowrloads/PythonImage.png', 'wb').write(myfile.content)
```
首先像以前一样使用requests模块的get方法，但是这一次，将把stream属性设置为True。<br />接着，在当前工作目录中创建一个名为PythonBook.pdf的文件，并打开它进行写入。<br />然后，指定每次要下载的块大小。将其设置为1024字节，接着遍历每个块，并在文件中写入这些块，直到块结束。
<a name="gcNzw"></a>
### 5、下载多个文件(并行/批量下载)
要同时下载多个文件，导入以下模块:
```python
import os
import requests
from time import time
from multiprocessing.pool import ThreadPool
```
导入了os和time模块来检查下载文件需要多少时间。ThreadPool模块允许使用池运行多个线程或进程。<br />创建一个简单的函数，将响应分块发送到一个文件:
```python
def url_response (url):
    path, url =url
    r = requests.get(url, stream=True)
    with open(path, 'wb') as f:
        for ch in r:
            f.write(ch)
```
这个URL是一个二维数组，它指定了要下载的页面的路径和URL。
```python
urls = [
    ("Event1","https://www.python.org/everts/pythan-everts/805/"),
    ("Event2","https://www.python.org/events/pythcn-events/801/"),
    ("Event3","https://www.python.org/eventz/python-events/790/"),
    ("Event4","https://www.python.org/everts/python-events/798/"),
    ("Event5","https://www.python.org/events/pythan-events/807/"),
    ("Event6","https://www.pytham.org/events/python-events/80T/"),
    ("Event7","https://www.pythan.org/events/pythan-events/75T/"),
    ("Event8","https://www.pythan.org/eventz/pythan-user-group/816/")
]
```
就像在前一节中所做的那样，将这个URL传递给`requests.get`。最后打开文件(URL中指定的路径)并写入页面内容。<br />现在可以分别为每个URL调用这个函数，也可以同时为所有URL调用这个函数。在for循环中分别为每个URL调用这个函数，注意计时器:
```python
start = time()
for x in urls:
    url_response(x)
print(f"Time to download: {time()- start}")
```
现在，使用以下代码行替换for循环：
```python
ThreadPool(9).imap_unordered(url_response, urls)
```
运行该脚本。
<a name="k0Ny1"></a>
### 6、使用进度条进行下载
进度条是clint模块的一个UI组件。输入以下命令来安装clint模块：
```python
pip install clint
```
看以下代码:
```python
import requests
from clint.textui import progress
url = 'http://do1.dr-chuck.com/pythonlearm/EN_us/pythonlearn.pdf'
r = requests.get (url, stream=True)
with open("LearnPython.pdf", "wb") as Pypdf:
    total_length = int (r.headers.get ('content-length'))
    for ch in progress.bar(r.iter_content(chunk_size = 2391975),expected_size=(total_length/1024) + 1):
        if ch:
            Pypdf.write(ch)
```
在这段代码中，首先导入了requests模块，然后从clint.textui导入了进度组件。唯一的区别是在for循环中。在将内容写入文件时，使用了进度条模块的bar方法。
<a name="tt8Mg"></a>
### 7、使用urllib下载网页
使用urllib下载一个网页。urllib库是Python的标准库，不需要安装它。以下代码行可以轻松地下载一个网页:
```python
import urllib
 
urllib.request.urlretrieve('url', 'path')
```
在这里指定将文件保存以及将它存储在哪里的URL。
```python
import urllib

urllib.request.urlretrieve ('https://ww.poython.org/', 'c:/users/Likeleeks/documerts/PythcrOrganizatioan.html')
```
在这段代码中，使用了urlretrieve方法并传递了文件的URL，以及保存文件的路径。文件扩展名将是.html。
<a name="6895a9f8"></a>
### 8、通过代理下载
如果需要使用代理下载文件，可以使用urllib模块的ProxyHandler。请看以下代码：
```python
import urllib.request
>>> myProxy = urllib.request.ProxyHandler({'http': '127.0.0.2'})
>>> openProxy = urllib.request.build_opener(myProzy)
>>> urllib.request.urlretrieve('https://wwrw.python.org/')
```
在这段代码中，创建了代理对象，并通过调用urllib的build_opener方法来打开该代理，并传入该代理对象。然后创建请求来获取页面。<br />此外还可以按照官方文档的介绍来使用requests模块:
```python
import requests
myProxy = {'http' : 'http://127.0.0.2:3001' }
requests.get("https://www.python.org/", proxies=myProxy)
```
只需要导入requests模块并创建代理对象。然后就可以获取文件了。
<a name="5v3kX"></a>
### 9、使用urllib3
urllib3是urllib模块的改进版本。可以使用pip下载并安装它:
```python
pip install urllib3
```
通过使用urllib3来获取一个网页并将它存储在一个文本文件中。<br />导入以下模块:
```python
import urllib3, shutil
```
在处理文件时，使用了shutil模块。现在像这样来初始化URL字符串变量：
```python
url = 'https://www.python.org/'
```
然后，使用urllib3的PoolManager ，它会跟踪必要的连接池。
```python
c = urllib3.PoolManager()
```
创建一个文件:
```python
filename = "test.txt"
```
最后发送一个GET请求来获取该URL并打开一个文件，接着将响应写入该文件:
```python
with c.request('GET', url, preload_cantert=False) as res,open(filename。'wb') as out_file:
    shutil.copyfileobj(res,out_file)
```
<a name="fZS81"></a>
### 10、使用Boto3从S3下载文件
要从Amazon S3下载文件，可以使用Python boto3模块。<br />在开始之前，需要使用pip安装awscli模块:
```python
pip install awscli
```
对于AWS配置，运行以下命令：
```python
aws configure
```
现在，按以下命令输入详细信息：
```python
AWS Access Key ID[None]: (The access key)
AWS Secret Access Key [None]: (Secret access key)
Default region name [None]: (Region)
Default output format [None]: (Json)
```
要从Amazon S3下载文件，需要导入boto3和botocore。Boto3是一个Amazon SDK，它允许Python访问Amazon web服务(如S3)。Botocore提供了与Amazon web服务进行交互的命令行服务。<br />Botocore自带了awscli。要安装boto3，运行以下命令:
```python
pip install boto3
```
现在，导入这两个模块：
```python
import boto3，botocore
```
在从Amazon下载文件时，需要三个参数：

- Bucket名称<br />
- 需要下载的文件名称
- 文件下载之后的名称<br />

初始化变量：
```python
bucket = "bucketName"
file_name = "filename"
downloaded_file = "downloadedfilename"
```
现在，初始化一个变量来使用会话的资源。为此将调用boto3的`resource()`方法并传入服务，即s3:
```python
service = boto3.resource('s3')
```
最后，使用download_file方法下载文件并传入变量:
```python
service.Bucket(bucket).download_file(file_name, downloaded_file)
```
<a name="9297c8fe"></a>
### 11、使用asyncio
asyncio模块主要用于处理系统事件。它围绕一个事件循环进行工作，该事件循环会等待事件发生，然后对该事件作出反应。这个反应可以是调用另一个函数。这个过程称为事件处理。asyncio模块使用协同程序进行事件处理。<br />要使用asyncio事件处理和协同功能，导入asyncio模块:
```python
import asyrcio
```
现在，像这样定义asyncio协同方法：
```python
async def coroutine():
    await my_fumc()
```
关键字async表示这是一个原生asyncio协同程序。在协同程序的内部，有一个await关键字，它会返回一个特定的值。也可以使用return关键字。<br />现在使用协同创建一段代码来从网站下载一个文件:
```python
>>> import os
>>> import urllib.request
>>> async def coroutine(url):
    r=urllib.request.urlopen(url)
    filename = "couroutine_dowrloads.txt"
    with open(filename,'wb') as f:
        for ch in r:
            f.write(ch)
    print_msg = 'Successfully Downloaded'
    return prirt_msg
>>> async def main_func(urls_to_download):
    co = [coroutine(url) for url in urls_to_download]
    downloaded,downloading = awrait asyncio.wait (co)
    for i in downloaded:
        print(i.result())
urls_to_download = ["https://www.python.org/events/pythor-events/801/",
                    "https://www.python.org/events/pythore-events/790/,
                    "https://www.python.org/events/pythor-user-group/816/",
                    "https://www.pythan.org/everts/pythan-evertz/75/"]
>>> eventLoop = asyncio.get_event_loop()
>>> eventLoop.run_until_complete(main_func(urls_to_download))
```
在这段代码中，创建了一个异步协同函数，它会下载文件并返回一条消息。<br />然后使用另一个异步协同程序调用`main_func`，它会等待URL并将所有URL组成一个队列。asyncio的wait函数会等待协同程序完成。<br />现在，为了启动协同程序，必须使用asyncio的get_event_loop()方法将协同程序放入事件循环中，最后使用asyncio的`run_until_complete()`方法执行该事件循环。
