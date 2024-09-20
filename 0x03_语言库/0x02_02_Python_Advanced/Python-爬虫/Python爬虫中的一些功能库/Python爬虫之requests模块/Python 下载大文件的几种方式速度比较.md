Python<br />通常都会用 requests 库去下载，这个库用起来太方便了。
<a name="qM3LI"></a>
## 方法一
使用以下流式代码，无论下载文件的大小如何，Python 内存占用都不会增加：
```python
def download_file(url):
    local_filename = url.split('/')[-1]
    # 注意传入参数 stream=True
    with requests.get(url, stream=True) as r:
        r.raise_for_status()
        with open(local_filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=8192): 
                f.write(chunk)
    return local_filename
```
如果有对 chunk 编码的需求，那就不该传入 chunk_size 参数，且应该有 if 判断。
```python
def download_file(url):
    local_filename = url.split('/')[-1]
    # 注意传入参数 stream=True
    with requests.get(url, stream=True) as r:
        r.raise_for_status()
        with open(local_filename, 'w') as f:
            for chunk in r.iter_content(): 
                if chunk:
                    f.write(chunk.decode("utf-8"))
    return local_filename
```
[iter_content](https://requests.readthedocs.io/en/latest/api/#requests.Response.iter_content) 函数本身也可以解码，只需要传入参数 `decode_unicode = True` 即可。<br />请注意，使用 iter_content 返回的字节数并不完全是 chunk_size，它是一个通常更大的随机数，并且预计在每次迭代中都会有所不同。
<a name="rwbkL"></a>
## 方法二
使用 [Response.raw](https://requests.readthedocs.io/en/latest/api/#requests.Response.raw) 和 [shutil.copyfileobj](https://docs.python.org/3/library/shutil.html#shutil.copyfileobj)
```python
import requests
import shutil

def download_file(url):
    local_filename = url.split('/')[-1]
    with requests.get(url, stream=True) as r:
        with open(local_filename, 'wb') as f:
            shutil.copyfileobj(r.raw, f)

    return local_filename
```
这将文件流式传输到磁盘而不使用过多的内存，并且代码更简单。<br />注意：根据文档，`Response.raw` 不会解码，因此如果需要可以手动替换 `r.raw.read` 方法
```python
response.raw.read = functools.partial(response.raw.read, decode_content=True)
```
<a name="UB24m"></a>
## 速度
方法二更快。方法一如果 2-3 MB/s 的话，方法二可以达到近 40 MB/s。
<a name="veHks"></a>
## 参考资料
iter_content: [https://requests.readthedocs.io/en/latest/api/#requests.Response.iter_content](https://requests.readthedocs.io/en/latest/api/#requests.Response.iter_content)<br />Response.raw: [https://requests.readthedocs.io/en/latest/api/#requests.Response.raw](https://requests.readthedocs.io/en/latest/api/#requests.Response.raw)<br />shutil.copyfileobj: [https://docs.python.org/3/library/shutil.html#shutil.copyfileobj](https://docs.python.org/3/library/shutil.html#shutil.copyfileobj)
