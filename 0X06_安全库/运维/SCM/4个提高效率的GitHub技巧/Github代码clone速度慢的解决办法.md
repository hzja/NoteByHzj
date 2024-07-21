Git
<a name="gkl4n"></a>
## 1、有代理
<a name="J4tBY"></a>
### 通过命令修改
只需要针对github设置代理即可，这里以ss为例子：
```bash
#只对github.com
git config --global http.https://github.com.proxy socks5://127.0.0.1:1080
#取消代理
git config --global --unset http.https://github.com.proxy#只对github.com
git config --global http.https://github.com.proxy socks5://127.0.0.1:1080
#取消代理
git config --global --unset http.https://github.com.proxy
```
<a name="a8cKU"></a>
### 直接修改~/.gitconfig文件
在~/.gitconfig中添加如下配置
```python
[http "https://github.com"]
	proxy = socks5://127.0.0.1:10808
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1662639938393-e240048f-a029-43da-b3aa-fd84eb19aa1b.png#clientId=u4f28688a-64fb-4&from=paste&height=383&id=u58d10f99&originHeight=957&originWidth=1806&originalType=binary&ratio=1&rotation=0&showTitle=false&size=49963&status=done&style=none&taskId=ua6f36cfa-55c2-4bc4-93f4-9bafee6be65&title=&width=722.4)
<a name="ps9xo"></a>
## 2、没代理
<a name="LZG1y"></a>
### 2.1 换URL
可以使用github的镜像网站：
```
github.com.cnpmjs.org
```
github.com替换为上述URL即可，然后去clone，速度飞快！
<a name="WPniG"></a>
### 2.2 配置Hosts
在ipaddress中查找github.com与github.global.ssl.fastly.net对应的ip，配置到hosts即可。
```
https://www.ipaddress.com/
```
```
xxxx  github.global.ssl.fastly.net
xxxx  github.com
```
例如：Ubuntu上在`/etc/hosts`中添加：
```
151.101.72.249 github.http://global.ssl.fastly.net
140.82.113.4 github.com
```
