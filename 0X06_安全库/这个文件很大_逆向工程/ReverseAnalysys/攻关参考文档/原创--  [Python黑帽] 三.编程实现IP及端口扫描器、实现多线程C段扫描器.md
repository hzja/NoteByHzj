# 原创
：  [Python黑帽] 三.编程实现IP及端口扫描器、实现多线程C段扫描器

# [Python黑帽] 三.编程实现IP及端口扫描器、实现多线程C段扫描器

Python黑帽第三篇文章将分享网络扫描基础知识，编程实现IP及端口扫描器、实现多线程C段扫描器。本文参考了《Python绝技》书籍和i春秋ADO老师的课程内容，这里真心推荐大家去学习ichunqiu课程，同时也结合作者的编程经验进行讲解。希望这篇基础文章对您有所帮助，更希望大家提高安全意识，也欢迎大家讨论。

娜璋AI安全之家于2020年8月18日开通，将专注于Python和安全技术，主要分享Web渗透、系统安全、CVE复现、威胁情报分析、人工智能、大数据分析、恶意代码检测等文章。真心想把自己近十年的所学所做所感分享出来，与大家一起进步。

> 
声明：本人坚决反对利用教学方法进行恶意攻击的行为，一切错误的行为必将受到严惩，绿色网络需要我们共同维护，更推荐大家了解技术背后的原理，更好地进行安全防护。虽然作者是一名安全小白，但会保证每一篇文章都会很用心地撰写，希望这些基础性文章对你有所帮助，在安全路上一起前行。


**文章目录**

---


## 一.Python多线程

### 1.进程和线程

**进程**：是程序的一次执行，每个进程都有自己的地址空间、内存、数据栈及其他记录运行轨迹的辅助数据。<br/> **线程**：所有的线程都运行在同一个进程当中，共享相同的运行环境。线程有开始、顺序执行和结束三个部分。

由于单线程效率低，程序中往往要引入多线程编程。计算机的核心是CPU，它承担了所有的计算任务，它就像一座工厂，时刻运行着。假定工厂的电力有限，一次只能供给一个车间使用。也就是说，一个车间开工的时候，其他车间都必须停工。背后的含义就是，单个CPU一次只能运行一个任务。

进程就好比工厂的车间，它代表CPU所能处理的单个任务。任一时刻，CPU总是运行一个进程，其他进程处于非运行状态。一个车间里，可以有很多工人。他们协同完成一个任务。线程就好比车间里的工人，一个进程可以包括多个线程。

---


### 2.thread模块

Python thread模块可以调用下述函数实现多线程开启。它将产生一个新线程，在新的线程中用指定的参数和可选的kwargs来调用这个函数。

注意：使用这个方法时，一定要加time.sleep()函数，否则每个线程都可能不执行。此方法还有一个缺点，遇到较复杂的问题时，线程数不易控制。

```
# -*- coding: utf-8 -*-
import _thread
import time

def fun1():
    print("hello world %s" % time.ctime())
    
#多线程
def main():
    _thread.start_new_thread(fun1, ()) #无参数
    _thread.start_new_thread(fun1, ())
    time.sleep(2)
    print("over")

#程序成功在同一时刻运行两个函数
if __name__ == '__main__':
    main()

```

输出结果如下图所示：

---


### 3.threading模块

thread模块存在一些缺点，尤其是线程数不能被控制。下面使用threading解决线程数可控制的问题。

```
# -*- coding: utf-8 -*-
import threading
import time
import sys

def fun1(key):
    sys.stdout.write('hello %s:%s \n'%(key, time.ctime()))

def main():
    threads = []
    keys = ['a', 'b', 'c']

    #线程数
    threads_count = len(keys)

    for i in range(threads_count):
        t = threading.Thread(target=fun1,  args=(keys[i],))
        threads.append(t)

    for i in range(threads_count):
        threads[i].start()

    for i in range(threads_count):
        threads[i].join()

if __name__ == '__main__':
    main()

```

输出结果如下图所示，三个线程同时发生。

多线程threading方法能够控制线程数，例如想写成requests模块，获取网站的status_code状态码。

```
# -*- coding: utf-8 -*-
import threading
import time
import requests
import sys

def fun1():
    time_start = time.time()
    r = requests.get(url="http://www.eastmountyxz.com/")
    times = time.time() - time_start
    #print('Status:%s--%s--%s'%(r.status_code, times, time.strftime('%H:%M:%S')))
    sys.stdout.write('Status:%s--%s--%s\n'%(r.status_code, times, time.strftime('%H:%M:%S')))
    
def main():
    threads = []
    #线程数 网页访问10次
    threads_count = 10

    for i in range(threads_count):
        t = threading.Thread(target=fun1,  args=())
        threads.append(t)

    for i in range(threads_count):
        threads[i].start()

    for i in range(threads_count):
        threads[i].join()

if __name__ == '__main__':
    main()

```

输出结果如下图所示：

---


## 二.编程实现ping扫描

上述代码简单讲解了thread和threading模块的多线程使用。但实际应用中有什么用呢？我们可以通过它对所有C段的地址进行扫描，ping探测其是否存活，代码如下。

```
# -*- coding: utf-8 -*-
import time
from subprocess import Popen, PIPE

def ping_check():
    ip = '127.0.0.1'
    #ping指定次数后停止ping 但报错访问被拒绝 选项-c需要具有管理权限
    #check = Popen("ping -c 3 {0} \n".format(ip), stdin=PIPE, stdout=PIPE,  shell=True)
    check = Popen("ping {0} \n".format(ip), stdin=PIPE, stdout=PIPE,  shell=True)
    data = check.stdout.read() #数据
    print(data.decode("gbk"))

#程序成功在同一时刻运行两个函数
if __name__ == '__main__':
    ping_check()

```

如果输入的ip地址为本机127.0.0.1，则输出正常连通结果，如下所示。

如果输入的ip地址为本机220.0.0.1，则提示超时，如下图所示。

**接着思考：如何对一个C段网址进行ping探测呢？**<br/> 基本思路是设计一个循环，如果主机不存在，返回的是timeout；如果主机存在，则包含TTL字样，这里以TTL为判断标准，从而判断存活的数据。

```
# -*- coding: utf-8 -*-
import time
from subprocess import Popen, PIPE

def ping_check():
    ip = '127.0.0.1'
    check = Popen("ping {0} \n".format(ip), stdin=PIPE, stdout=PIPE,  shell=True)
    data = check.stdout.read() #数据
    data = data.decode("gbk")  #编码转换:byte-&gt;str
    if 'TTL' in data: #存活
        print('UP')

#程序成功在同一时刻运行两个函数
if __name__ == '__main__':
    ping_check()

```

输出结果为“UP”。

---


## 三.检测IP存活地址

接下来我们尝试检测ichunqiu网站的ip地址存活情况。首先，调用ping命令检测该网站的ip地址，即：117.23.xx.xx。（该部分参考i春秋ADO老师，再次感谢）

这里将ping_check()函数设置一个传递参数，对应ip地址，对它进行探测；通过thread线程实现ip地址存活性探测，能探测到很多存活的主机。

```
# -*- coding: utf-8 -*-
import _thread
import time
from subprocess import Popen, PIPE

def ping_check(ip):
    check = Popen("ping {0} \n".format(ip), stdin=PIPE, stdout=PIPE, shell=True)
    data = check.stdout.read()  #数据
    data = data.decode("gbk")   #编码转换:byte-&gt;str
    if 'TTL' in data: #存活
        print('%s is UP' % ip)

#主函数
if __name__ == '__main__':
    #寻找目标 ichunqiu  117.23.xx.xx
    for i in range(1,255):
        ip = '117.23.xx.' + str(i)
        #多线程方法
        _thread.start_new_thread(ping_check, (ip, ))
        time.sleep(0.1)

```

输出结果如下图所示，其中IP值也被识别出来。

**问题：**<br/> 在多线程编程中，几个线程是同时启动，所以输出也是输出在一行，那怎么才能换行输出呢？这里使用系统输出解决。

```
# -*- coding: utf-8 -*-
import _thread
import time
from subprocess import Popen, PIPE
import sys

def ping_check(ip):
    check = Popen("ping {0} \n".format(ip), stdin=PIPE, stdout=PIPE, shell=True)
    data = check.stdout.read()  #数据
    data = data.decode("gbk")   #编码转换:byte-&gt;str
    if 'TTL' in data: #存活
        sys.stdout.write('%s is UP \n' % ip)

#主函数
if __name__ == '__main__':
    #寻找目标 ichunqiu  1.31.128.240
    for i in range(1,255):
        ip = '1.31.xx.' + str(i)
        #多线程方法
        _thread.start_new_thread(ping_check, (ip, ))
        time.sleep(0.1)

```

按行输出结果，如下图所示：

---


## 四.编程实现端口扫描器

当我们拿到某个网站通过需要扫描它开放的端口，比如通过在线工具TScan扫描结果如下图所示，开放了80、443、8080、7777等端口。那我们是否能通过Python编写代码实现呢？类似于NMAP的功能。

### 1.socket方法

Python扫描端口可以通过socket通讯实现，通过建立TCP套接字，向端口发送TCP SYN数据包， 并等待TCP ACK相应，从而确定该端口是否开放。

完整代码：

```
# -*- coding: utf-8 -*-
import optparse
import socket
from socket import *

#Socket连接
def connScan(tgtHost, tgtPort):
    try:
        conn = socket(AF_INET, SOCK_STREAM)
        conn.connect((tgtHost, tgtPort))
        #conn.send('Violent Python\n')
        #results = conn.recv(100)
        print(' [+] %d/tcp open ' % tgtPort)
    except Exception as err:
        print(' [-] %d/tcp closed' % tgtPort)
        #print(err)
    finally:
        conn.close()
        
#扫描端口
def portScan(tgtHost, tgtPorts):
    try:
        tgtIP = gethostbyname(tgtHost)
        print(tgtIP)
    except:
        print("[-] Cannot resolve '%s': Unknown host" % tgtHost)
    try:
        tgtName = gethostbyaddr(tgtIP)
        print("[+] Scan Results for: " + tgtName[0])
    except:
        print("[+] Scan Results for: " + tgtIP)
    setdefaulttimeout(1)
    for tgtPort in tgtPorts:
        print(" Scanning port " + tgtPort)
        #连接端口
        connScan(tgtHost, int(tgtPort))

#主函数 optparse用于处理命令行参数
def main():
    parser = optparse.OptionParser("usage%prog "+ \
                "-H &lt;target host&gt; -p &lt;target port&gt;")
    parser.add_option('-H', dest='tgtHost', type='string', \
                help='specify target host')
    parser.add_option('-p', dest='tgtPort', type='string', \
                help='specify target port[s] separated by comma')

    #解析脚本输入的参数值
    (options, args) = parser.parse_args()
    tgtHost = options.tgtHost
    tgtPorts = str(options.tgtPort).split(',')
    if (tgtHost==None) or (tgtPorts[0]==None):
        print('[-] You must specify a target host and port[s].')

    #端口扫描
    portScan(tgtHost, tgtPorts)
    
if __name__ == '__main__':
    main()

```

该代码使用了optparse参数定义，包括需要扫描的host和需要扫描的端口，运行结果如下图所示，成功将开放的端口80、443、777、9080识别出来。

同样，我们也可以简单编写上述代码，自定义IP端口序列进行扫描。

```
# -*- coding: utf-8 -*-
import socket

#扫描端口
def get_ip_status(ip,port):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server.connect((ip,port))
        print('{0} port {1} is open'.format(ip, port))
    except Exception as err:
        print('{0} port {1} is not open'.format(ip,port))
    finally:
        server.close()
 
if __name__ == '__main__':
    host = '210.40.xx.xx'
    for port in range(20,100):
        get_ip_status(host,port)

```

输出结果如下：

---


### 2.telnetlib方法

接下来我们参考Dahlhin大神的文章，补充Python内置模块telnetlib，它同样可以完成端口检测任务。

```
# -*- coding: utf-8 -*-
import telnetlib

#端口扫描
def get_ip_status(ip,port):
    server = telnetlib.Telnet()      #创建一个Telnet对象
    try:
        server.open(ip,port)         #利用Telnet对象的open方法进行tcp链接
        print('{0} port {1} is open'.format(ip, port))
    except Exception as err:
        print('{0} port {1} is not open'.format(ip,port))
    finally:
        server.close()
 
if __name__ == '__main__':
    host = '210.40.xx.xx'
    for port in range(20,100):
        get_ip_status(host,port)

```

输出结果同样是80端口开放，我们可以扩大扫描的范围，但其速度非常慢。

---


### 3.多线程扫描

引入多线程threading模块实现多线程扫描，此时我们仅显示开放着的端口。

```
import telnetlib
import threading
import queue

#IP端口开发检测
def get_ip_status(ip):
    server = telnetlib.Telnet()
    for port in range(20,10000):
        try:
            server.open(ip,port)
            print('{0} port {1} is open'.format(ip, port))
        except Exception as err:
            #print('{0} port {1} is not open'.format(ip,port))
            pass
        finally:
            server.close()
 
def check_open(q):
    try:
        while True:
            ip = q.get_nowait()
            get_ip_status(ip)
    except queue.Empty as e:
        pass

#主函数
if __name__ == '__main__':
    host = ['210.40.xx.xx']     #模拟多IP地址
    q = queue.Queue()
    for ip in host:
        q.put(ip)
    threads = []
    for i in range(10):
        t = threading.Thread(target=check_open,args=(q,))
        t.start()
        threads.append(t)
 
    for t in threads:
        t.join()

```

---


## 五.多线程+Queue实现C段扫描

虽然threading解决了线程数可控问题，但是面对复杂问题的时候，比如生产者和消费者问题，仍然不能很好地解决。

生产者-消费者问题和Queue模块：

**生产者和消费者问题**<br/> 生产者生产货物，将货物放到队列数据中，生产者在生产这些货物时，它的时间是不确定的；当生存着将货物交给消费者，消耗的时间也是不确定的；由于两个时间都不确定，多线程编程存在一定问题。这里引入Queue模块解决该问题（Python3引入库为queue，Python2为Queue）。

```
import queue

queue = queue.Queue()

for i in range(10):
    queue.put(i)
print(queue.empty())
print(queue.qsize())

#取数据 get依次取出里面的数据
print(queue.get())
print(queue.get())

```

输出结果如下所示：

生产者利用Queue将所有数据货物按顺序放入Queue，接着消费者依次取出Queue中的数据。接着实现C段扫描。

```
# -*- coding: utf-8 -*-
# By:CSDN Eastmount 2019-10-05
import threading
import queue
import sys
from subprocess import Popen, PIPE

#定义一个类 传入参数queue
class DoRun(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self._queue = queue

    def run(self):
        #非空取数据
        while not self._queue.empty():
            ip = self._queue.get()
            #print ip
            check_ping = Popen("ping {0} \n".format(ip), stdin=PIPE, stdout=PIPE,  shell=True)
            data = check_ping.stdout.read()
            data = data.decode("gbk")
            if 'TTL' in data:
                sys.stdout.write(ip+' is UP.\n')
            
def main():
    threads = []
    threads_count = 100
    q = queue.Queue()
    
    #放入ip地址
    for i in range(1, 255):
        q.put('210.40.81.' + str(i))

    for i in range(threads_count):
        threads.append(DoRun(q))

    for i in threads:
        i.start()

    for i in threads:
        i.join()

if __name__ == '__main__':
    main() 

```

最终输出结果如下图所示，通过该代码可以实现检测某网站ip段的存活情况。

---


## 六.总结

希望这篇文章对你有所帮助，这是Python黑帽第三篇博客，编程实现了IP及端口扫描器、实现多线程C段扫描器。后续作者也将继续深入学习，制作一些常用的小工具供大家交流，当然你也可以和NMAP这类工具对比，但Python有时候能让你集成更强大且自由的工具。

最后，真诚地感谢您关注“娜璋之家”公众号，也希望我的文章能陪伴你成长，希望在技术路上不断前行。文章如果对你有帮助、有感悟，就是对我最好的回报，且看且珍惜！再次感谢您的关注，也请帮忙宣传下“娜璋之家”，哈哈~初来乍到，还请多多指教。

前文：

**想家喽 o(╥﹏╥)o**

(By:娜璋AI安全之家 Eastmount 2020-09-17 夜于武汉)

---


**参考文献：**
