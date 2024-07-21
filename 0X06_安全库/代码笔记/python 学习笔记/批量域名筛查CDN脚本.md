经常打攻防或者红蓝对抗的时候发现资产下来都是一堆url。正面刚不进去，需要快速找到软骨头。思路一般如下： <br /> 1、域名→子域名→真实ip→全端口<br /> 2、 url→域名→真实ip→全端口<br />虽然现在的CDN很难绕，但是可以迅速找软骨头，找到没有部署CDN的资产，一顿猛揍。某种意义上来说，没部署CDN也可以是防御意识没到位。所以需要批量域名筛查CDN。

<a name="AcJoN"></a>
## 思路
有cdn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620277521439-c06cc278-e80d-4581-af49-88dfe6e7f507.png#height=326&id=NSb6d&originHeight=435&originWidth=445&originalType=binary&ratio=1&rotation=0&showTitle=false&size=33977&status=done&style=none&title=&width=334)<br />无cdn，关键的区别在于Addresses关键字，如果没有基本上都是无cdn的，当然也不一定，但是基本上是。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620277653646-04ae9dac-ef29-4b5d-ba22-48a83c185719.png#height=170&id=nctMI&originHeight=170&originWidth=400&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12435&status=done&style=none&title=&width=400)<br />www.neusoft.com
<a name="a4ONf"></a>
## 实现
可以使用os模块中的popen，把结果存入一个变量中，然后通过判断是否存在Addresses来判断是否存在cdn。<br />a = os.popen(command).read()但是这个结果输出有点问题，<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620298878772-3350ac0c-ab06-4c3d-a946-d9ce31637ed9.png#height=87&id=erKEa&originHeight=87&originWidth=277&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3361&status=done&style=none&title=&width=277)<br />后面换了个模块subprocess.getoutput(command)那一个字就是真香，能直接执行命令， 并且能把命令存入到变量中。通过Addresses匹配内容来判断是否存在CDN。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299327476-5464056d-e801-4cd1-9d9c-f63502264a22.png#height=130&id=zwbGb&originHeight=173&originWidth=467&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13095&status=done&style=none&title=&width=350)<br />通过命令执行ping无CDN的域名，然后通过正则匹配出来ip<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299374061-9924d83b-d896-47a7-8a71-322bb5024872.png#height=78&id=I5ATo&originHeight=104&originWidth=1184&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19274&status=done&style=none&title=&width=888)<br />把无CDN的站点存入文本中，不重合并且换行存入。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620299415037-207d0565-3e27-4266-aca8-ef59307aa9a2.png#height=43&id=zswB8&originHeight=57&originWidth=746&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8542&status=done&style=none&title=&width=560)<br />然后把代码逻辑和运行的稳健性完善了一下。总体代码主要如下
```python
import os
import re
import subprocess

def cdntest():
    sum = 0
    with open("D:\\vscode\\python\\基础\\domain.txt","r") as f:
        for i in f.readlines():
            try:
                i = i.strip()
                command =('nslookup {}'.format(i))
                a = subprocess.getoutput(command)
                sum = sum+1          
                #print('---------------------')
                if 'Addresses' not in a:
                    command1 =('ping -n 1 {}'.format(i))
                    b = os.popen(command1).read()
                    c = re.findall(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b", b)
                    print('{}==================>>{}该站点不存在CDN,真实ip为{}'.format(sum,i,c[0]))
                    with open("D:\\vscode\\python\\基础\\result.txt","a",encoding='utf-8') as W:
                        W.write('{}:{}'.format(i,c[0])+'\n')
                else:
                    print('{}==================>>{}该站点有问题'.format(sum,i))
            except:
                print('{}==================>>{}该站点有问题'.format(sum,i))
    f.close()
    W.close()    
if __name__ == '__main__':
    cdntest()
```
<a name="f0v57"></a>
## 需要的改进
1、有很高的准确性，但不是百分百准确，最好还是多地ping去识别、提取ip<br />2、没有线程，运行只能单线程，面对大量的域名可能需要等比较长的时间（但肯定比手工快）

<a name="lMyti"></a>
## 改进之后
为了防止多线程的有锁无锁问题，用了队列配合多线程去提高速度。

把文本中的域名添加到队列中，相当于把要处理的问题都添加到一个队列中使用queue.put()，然后再使用多线程去处理队列<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620374002099-2eec270c-5ad9-42d5-984f-38701695b85a.png#height=130&id=vST2d&originHeight=173&originWidth=648&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18676&status=done&style=none&title=&width=486)
```python
import threading
import subprocess
import time
import queue
import re
import os

# 下面来通过多线程来处理Queue里面的任务：
def work(q):
    while True:
        if q.empty():
            return
        else:
            t = q.get()
            try:
                command =('nslookup {}'.format(t))
                a = subprocess.getoutput(command)        
                #print('---------------------')
                if 'Addresses' not in a:
                    command1 =('ping -n 1 {}'.format(t))
                    b = os.popen(command1).read()
                    c = re.findall(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b", b)
                    print('==================>>{}该站点不存在CDN,真实ip为{}'.format(t,c[0]))
                    '''
                    with open("D:\\vscode\\python\\基础\\result.txt","a",encoding='utf-8') as W: # 写入无CDN的文本，需要先创建。
                        W.write('{}:{}'.format(t,c[0])+'\n')
                    '''
                else:
                    pass
                    #print('==================>>{}该站点有问题'.format(t))
            except:
                pass
                #print('==================>>{}该站点有问题'.format(t))
          
def main():
    q = queue.Queue()
    filename = input('请输入域名所在的文本路径：')
    with open(filename,"r") as f:    #域名放置的目录
        for i in f.readlines():
            q.put(i.strip())
    print("获取了",q.qsize(),"个目标")
    thread_num = 10
    threads = []
    for i in range(thread_num):
        t = threading.Thread(target=work, args=(q,))
        threads.append(t)
    # 创建5个线程
    for i in range(thread_num):
        threads[i].start()
    for i in range(thread_num):
        threads[i].join()

if __name__ == "__main__":

    banner='''
    

               __                 
              / _|                
   __ _ _   _| |_ ___ _ __   __ _ 
  / _` | | | |  _/ _ \ '_ \ / _` |
 | (_| | |_| | ||  __/ | | | (_| |    by aufeng 
  \__,_|\__,_|_| \___|_| |_|\__, |    基于nslookup的批量域名CDN筛查脚本
                             __/ |    Tips ：cmd运行后，输入请输入域名所在的文本路径,txt即可
                            |___/ 


    '''
    print(banner)
    start = time.time()
    main()
    print('耗时：', time.time() - start)
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620374504786-e1dfdb57-2ac4-4a49-bf3c-5b5aedf22069.png#height=664&id=VkhSI&originHeight=885&originWidth=990&originalType=binary&ratio=1&rotation=0&showTitle=false&size=176247&status=done&style=none&title=&width=743)

<a name="R66Ob"></a>
## 实战后出现的问题(需要改进
Addresses 也不一定有cdn的，要看ipv4<br />zhejiang.gov.cn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1621921960779-35753003-0dc9-4217-9324-ecdc6870fbf5.png#height=372&id=izQ3U&originHeight=496&originWidth=1140&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68083&status=done&style=none&title=&width=855)
<a name="ToyP6"></a>
## 最终版(好用
后来发现规律，有cdn会有3个ip<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622266083636-9cf5ba29-8b27-4f3d-9512-4637c5b19e56.png#height=155&id=duiS6&originHeight=207&originWidth=561&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17441&status=done&style=none&title=&width=421)<br />无cdn只会有2个ip<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622266210623-eb056508-e240-4502-accb-96123d6123cb.png#height=160&id=u4hIo&originHeight=160&originWidth=537&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15855&status=done&style=none&title=&width=537)<br />所以直接匹配，如果多出2个ip就是存在cdn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622266593333-4b46703c-5073-411a-a16c-760b24302ca1.png#height=104&id=EvnND&originHeight=208&originWidth=1188&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22134&status=done&style=none&title=&width=594)
```python
import threading
import subprocess
import time
import queue
import re
import os


# 下面来通过多线程来处理Queue里面的任务：
def work(q):
    global num
    while True:
        if q.empty():
            return
        else:
            t = q.get()
            try:
                command =('nslookup {}'.format(t))
                a = subprocess.getoutput(command)
                c = re.findall(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b", a)
                #print(len(c))
                if len(c) < 3:
                    num = num + 1
                    command1 =('ping -n 1 {}'.format(t))
                    b = os.popen(command1).read()
                    c = re.findall(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b", b)
                    print('==================>>{}该站点不存在CDN,真实ip为{}'.format(t,c[0]))
                    '''
                    with open("D:\\vscode\\python\\基础\\result.txt","a",encoding='utf-8') as W: # 写入无CDN的文本，需要先创建。
                        W.write('{}:{}'.format(t,c[0])+'\n')
                    '''
                else:
                    pass
                    #print('==================>>{}该站点有问题'.format(t))
            except:
                pass
                #print('==================>>{}该站点有问题'.format(t))
          
def main():
    q = queue.Queue()
    filename = input('请输入域名所在的文本路径：')
    with open(filename,"r") as f:    #域名放置的目录
        for i in f.readlines():
            q.put(i.strip())
    print("获取了",q.qsize(),"个目标")
    thread_num = 10
    threads = []
    for i in range(thread_num):
        t = threading.Thread(target=work, args=(q,))
        threads.append(t)
    # 创建5个线程
    for i in range(thread_num):
        threads[i].start()
    for i in range(thread_num):
        threads[i].join()

if __name__ == "__main__":
    global num
    num = 0

    banner='''
    

               __                 
              / _|                
   __ _ _   _| |_ ___ _ __   __ _ 
  / _` | | | |  _/ _ \ '_ \ / _` |
 | (_| | |_| | ||  __/ | | | (_| |    by aufeng 
  \__,_|\__,_|_| \___|_| |_|\__, |    基于nslookup的批量域名CDN筛查脚本
                             __/ |    Tips ：cmd运行后，输入请输入域名所在的文本路径,txt即可
                            |___/ 


    '''
    print(banner)
    start = time.time()
    main()
    print('耗时：', time.time() - start)
    print("无cdn的数量：",num)
```

<a name="S3QJi"></a>
## 需要改进 (有点慢
需要去筛一些云服务器，比如百度直接搜ip，匹配是某云厂商<br />ip的C段归类

根据返回包的ip值数量来判断cdn<br />[https://myssl.com/cdn_check.html?domain=www.kingsoft.com](https://myssl.com/cdn_check.html?domain=www.kingsoft.com)

www.kingsoft.com  有cdn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1636733287263-73c727df-74ec-4b86-8487-a8ec6bf6aa5d.png#clientId=uee5e194c-cfe7-4&from=paste&height=346&id=u1481f242&originHeight=692&originWidth=1547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94910&status=done&style=none&taskId=u71270999-6758-4cb5-9dad-634882eb753&title=&width=773.5)

ffbebbs.kingsoft.com 无cdn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1636733255184-7af3d51e-0371-4068-adb6-7c0032fb99d8.png#clientId=uee5e194c-cfe7-4&from=paste&height=348&id=ud45390f9&originHeight=695&originWidth=1547&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91391&status=done&style=none&taskId=uf43b0b21-b330-454d-8475-9a18a91dc41&title=&width=773.5)


[https://www.wepcc.com/](https://www.wepcc.com/) 这个网站速度应该会快很多<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1637509421920-fc161010-c271-4bc5-9eb3-ecfed2a7d633.png#clientId=u8d3aca82-4d18-4&from=paste&height=376&id=ua8cfcaa8&originHeight=752&originWidth=1556&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91661&status=done&style=none&taskId=ua25b2284-35d0-4b9d-9ae0-4d2934fabc3&title=&width=778)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1637509449076-baaa839e-f986-4c78-a55e-66f6676232ed.png#clientId=u8d3aca82-4d18-4&from=paste&height=327&id=u85408e56&originHeight=653&originWidth=1552&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71838&status=done&style=none&taskId=uf0cea7d3-2beb-4ada-a4f7-4b63005ea22&title=&width=776)


获取节点的所有hash
```go
func get_hash(i string) {
	//i := "ffbebbs.kingsoft.com"
	resp, err := http.PostForm("https://www.wepcc.com/", url.Values{"host": {i}, "node": {"2,3,6"}})
	if err != nil {
		fmt.Printf("get failed, err:%v\n", err)
		return
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("read from resp.Body failed, err:%v\n", err)
		return
	}
	//fmt.Print(string(body))
	reg := regexp.MustCompile(`<tr class="item" data-id="(?s:(.*?))">`)
	if reg == nil {
		fmt.Println("MustCompile err")
		return
	}
	//提取关键信息
	result := reg.FindAllStringSubmatch(string(body), -1)
	//fmt.Println(result)
	//过滤<></>
	var a []string
	for _, text := range result {
		a = append(a, text[1])
		//fmt.Println(text[1])
	}
	//fmt.Println(a)
	check_ping(i, a)
}

```

利用上一步中的hash进行cdn的探测
```go
func check_ping(i string, a []string) {
	var ip []string
	for _, b := range a {
		resp, err := http.PostForm("https://www.wepcc.com/check-ping.html", url.Values{"host": {i}, "node": {b}})
		if err != nil {
			fmt.Printf("get failed, err:%v\n", err)
			return
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			fmt.Printf("read from resp.Body failed, err:%v\n", err)
			return
		}
		//var data[] string
		//fmt.Println(string(body))
		//matched, err := regexp.MatchString("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}", string(body))
		c := regexp.MustCompile("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}").FindAllStringSubmatch(string(body), -1)
		ip = append(ip, c[0][0])
		//fmt.Println(c[0][0])
	}
	if ip[0] == ip[1] && ip[0] == ip[2] && ip[0] == ip[3] && ip[0] == ip[4] {
		fmt.Println("[+]no cdn: ", i, " ", ip[0])
	}
}
```
