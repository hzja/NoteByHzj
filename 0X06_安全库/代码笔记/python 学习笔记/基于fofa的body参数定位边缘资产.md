<a name="ihqB5"></a>
### 漏洞已修复，请勿模仿，后果自负。仅作为笔记学习。
<a name="rSIar"></a>
### 关于边缘资产搜集
内蒙古电力集团的。。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624640097416-9a5c585f-1bad-417b-875c-bd9dd28fa374.png#align=left&display=inline&height=101&originHeight=134&originWidth=1021&size=29511&status=done&style=none&width=766)<br />按照常规的，百度、bind，根本找不到下面的资产。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624642841666-ca112588-d96f-4e0d-8f74-a3e71bdcfa73.png#align=left&display=inline&height=319&originHeight=425&originWidth=887&size=273374&status=done&style=none&width=665)<br />用fofa的body搜，甚至全是一些奇奇怪怪的东西<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624640166563-2e4be55b-22cf-43a4-89c2-7bdcbc967e7a.png#align=left&display=inline&height=562&originHeight=750&originWidth=1159&size=109582&status=done&style=none&width=869)<br />但是这个时候不能放弃，把title再筛一下关键词，可能就出来了。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624640287174-6be10674-38f9-4c78-aa4f-8e4272187c22.png#align=left&display=inline&height=296&originHeight=395&originWidth=1139&size=71714&status=done&style=none&width=854)
```python
import requests
import json
import base64
import re

def main(targetsrting):
    email=""    #email
    key=""  #key
    #targetsrting='ip="202.107.117.5/24"' #搜索关键字
    target=base64.b64encode(targetsrting.encode('utf-8')).decode("utf-8")
    url="https://fofa.so/api/v1/search/all?email={}&key={}&qbase64={}&fields=host,server,title&size=1000".format(email,key,target)
    resp = requests.get(url)
    try:
        resp = requests.get(url)
        data_model = json.loads(resp.text) #字符串转换为字典
        #print(data_model)
        num = 0
        for i in data_model["results"]:
            num = num +1
            if (len(i[2]) > 0) and ('Not Found' not in i[2])&('ERROR' not in i[2])&('Unavailable' not in i[2]):
                print('{:<30}{:<30}{:<20}'.format(i[0],i[1],i[2]))
        a = input('是否要用body进行边缘资产的title筛查(确定的话请摁1,不需要则摁2):')
        if(a == '1'):
            body(targetsrting,data_model)
        print("fofa查询总共",num,"条数据,以上数据均通过title筛查不输出空值。")
    except:
        print("'\n',出现问题了,账号密码、网络、其他原因，无法fofa查询")

def body(targetsrting,data_model):
    print('/n','body筛查的结果')
    num = 0
    inputString = '{}'.format(targetsrting)
    f2 = re.findall(r'"([^"]*)"', inputString)
    for i in data_model["results"]:
        num = num +1
        if (f2[0] in i[2]):
            print('{:<30}{:<30}{:<20}'.format(i[0],i[1],i[2]))

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
