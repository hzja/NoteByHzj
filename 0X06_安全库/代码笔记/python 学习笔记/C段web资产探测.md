有时候资产的C段会有一些边缘资产，主要快速识别关键字，定位边缘资产。可以通过title
<a name="eHFi1"></a>
## 普通python+多线程：
普通的C段443端口指纹识别。
```python
import re
import requests        
from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
import threading
import time

def work(i):   
    try:
        response = requests.get('https://'+i,verify=False)       
        # print(strhtml.text)
        a = re.findall(r"<title.*?>(.+?)</title>", response.text)
        print(i,"     ",a,"     ",response.headers['Server'])
    except:
        pass
        #print("请求失败")

def scan_thread(I):  # 线程函数
    print("开始运行**")
    threads = []    # 线程列表
    for i in I:
        #print(i)
        threads.append(   
                  # 添加线程
            threading.Thread(target=work, args=(i,))
        )

    for thread in threads:      # 遍历列表 启动线程
        thread.start()

    for thread in threads:      # 遍历列表 等待结束线程
        thread.join()

def shuzu(a):
    I = []
    for i in range(1,256):
        I.append(a+repr(i))
    scan_thread(I)


if __name__ == "__main__":
    ip = input("请输入要扫描C段的ip：")
    reg = r'\d{1,3}\.\d{1,3}\.\d{1,3}\.'
    a = re.findall(reg,ip)
    t1 = time.time()
    shuzu(a[0])
    print("运行完毕，所需时间",(time.time()-t1))
```

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622617612151-d3ff5bb3-f9c3-4658-a2cf-31c505ec7783.png#height=130&id=BvM5C&originHeight=260&originWidth=1334&originalType=binary&ratio=1&size=56619&status=done&style=none&width=667)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622618233775-189c2d36-d47d-4b99-ad63-41e68a23a6e6.png#height=215&id=W20UO&originHeight=430&originWidth=1436&originalType=binary&ratio=1&size=85626&status=done&style=none&width=718)

<a name="85qP7"></a>
## fofa接口
优点：写出来后，不需要翻fofa页面，直接快速看，就能定位C段相关资产。<br />https://fofa.so/api/v1/search/all?email=&key=&qbase64=<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622556297698-4bec2954-5388-40b3-85bc-11ec37c00ccd.png#height=316&id=T7b0W&originHeight=421&originWidth=964&originalType=binary&ratio=1&size=38845&status=done&style=none&width=723)<br />qbase64是需要查询的内容的base64加密<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622556207841-ab6edf9d-51f1-4c7f-aa62-4adbf4be6627.png#height=208&id=tXCpx&originHeight=415&originWidth=1432&originalType=binary&ratio=1&size=31634&status=done&style=none&width=716)<br />写出来后，就一个字真香啊。速度太快了，线程都不用起。<br />把title中的404、not found去掉，输出一些有质量的title，便于快速定位C段弱资产
```python
import requests
import json
import base64

def main(targetsrting):
    email=""    #email
    key=""  #key
    #targetsrting='ip="202.107.117.5/24"' #搜索关键字
    target=base64.b64encode(targetsrting.encode('utf-8')).decode("utf-8")
    url="https://fofa.so/api/v1/search/all?email={}&key={}&qbase64={}&fields=host,server,title&size=1000".format(email,key,target)
    try:
        resp = requests.get(url)
        data_model = json.loads(resp.text) #字符串转换为字典
        num = 0
        for i in data_model["results"]:
            num = num +1
            if (len(i[2]) > 0) and ('Not Found' not in i[2])&('ERROR' not in i[2])&('Unavailable' not in i[2]):
                print('{:<30}{:<30}{:<20}'.format(i[0],i[1],i[2]))
            
        print("fofa查询总共",num,"条数据,以上数据均通过title筛查不输出空值。")
    except:
        print("'\n',出现问题了,账号密码、网络、其他原因，无法fofa查询")

if __name__ == '__main__':
    print('''
    fofa语法
    host=".gov.cn"
    port="6379"
    ip="1.1.1.1"
    ip="220.181.111.1/24"
    
    该脚本主要用于快速C段寻找目标边缘资产。 --by aufeng
    ''')
    a = input("请输入需要查询的fofa语法：")
    main(a)
```

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622715808044-16c25f9e-38b5-44b4-a424-9843aabdf6d4.png#height=474&id=EicWV&originHeight=947&originWidth=1161&originalType=binary&ratio=1&size=210487&status=done&style=none&width=580.5)
