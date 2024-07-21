Nginx Shell 日志
<a name="b1nAr"></a>
### 1、根据访问IP统计UV

- UV（Unique Visitor）独立访客，统计访问某站点的用户数；<br />
- IP（Internet Protocol）独立IP数，是指独立的浏览了页面的不同IP，即统计不同的IP浏览用户数量。同一IP不管访问了几个页面，独立IP数均为1；不同的IP浏览页面，计数会加1。IP是基于用户广域网IP地址来区分不同的访问者的，所以多个用户（多个局域网IP）在同一个路由器（同一个广域网IP）内上网，可能被记录为一个独立IP访问者。另外如果用户不断更换IP，则有可能被多次统计。<br />
```bash
awk '{print $1}' access.log | sort | uniq -c | wc -l
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611488558-2f46cc39-0471-46d0-a3ee-3658265b5e0d.png#align=left&display=inline&height=180&originHeight=539&originWidth=3323&size=602195&status=done&style=none&width=1107.6666666666667)
<a name="rnIh3"></a>
### 2、根据访问URL统计PV

- PV（Page View）访问量, 即页面浏览量或点击量，衡量网站用户访问的网页数量；在一定统计周期内用户每打开或刷新一个页面就记录1次，多次打开或刷新同一页面则浏览量累计。<br />
```bash
awk '{print $7}' access.log | wc -l
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611510021-6a37b334-ca78-4a63-a01a-ca230593a307.png#align=left&display=inline&height=72&originHeight=215&originWidth=3323&size=174949&status=done&style=none&width=1107.6666666666667)
<a name="W2Zci"></a>
### 3、统计访问最多的URL
根据访问最多的URL来判断哪些业务比较繁忙；也跟根据情况判断是否该业务是否有黑客攻击。
```bash
awk '{print $7}' access.log | sort | uniq -c | sort -n -k 1 -r | more
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611532781-59bbc816-e1a2-415c-b9a4-31516f9d8c80.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2075789&status=done&style=none&width=1107.6666666666667)<br />加一个more命令主要防止URL一屏幕显示不下的情况。
<a name="svnDz"></a>
### 4、统计访问最频繁的IP
根据访问IP来判断哪些IP操作最多；也可以根据情况判断该IP是不是疑似攻击IP。
```bash
awk '{print $1}' access.log | sort | uniq -c | sort -n -k 1 -r | more
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611563152-d9b24970-d0b9-4cc4-b303-36e9f83b08e5.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2014302&status=done&style=none&width=1107.6666666666667)<br />加一个more命令主要防止IP一屏幕显示不下的情况。
<a name="52z3c"></a>
### 5、根据时间段统计查看日志
具体使用sed或者grep都可以，主要是编写正则表达式<br />sed：
```bash
cat access.log | sed -n '/29\/Aug\/2020:[01-23]/p' | more
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611633083-404a7b6e-2dab-4266-8f77-c9a6bc9b65ac.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2321591&status=done&style=none&width=1107.6666666666667)<br />grep：
```bash
grep '29\/Aug\/2020:[01-23]' access.log | more
```
<a name="ctnKn"></a>
### 6、统计当日的的pv和uv
因为是统计当日的pv和uv，所以在编写命令时之前了解一下nginx的日志格式。日志的默认格式如下：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611726452-4a60162b-181c-432b-a8ec-ba5c0c41c6e8.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2341853&status=done&style=none&width=1107.6666666666667)<br />默认输出的月份使用英文简写。<br />pv：
```bash
cat access.log | sed -n /`date "+%d\/%b\/%Y"`/p | awk '{print $7}' | sort | wc -l
```
uv：
```bash
cat access.log | sed -n /`date "+%d\/%b\/%Y"`/p |awk '{print $1}' | sort|uniq -c | wc -l
```
如果使用以上这两个命令发现统计不出来任何数据时，就要检查一下，系统的语言变量LANG是否配置成中文，如果是需要配置成英文<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600611780190-e78fb60c-c7fd-44c1-b64e-e03c2eccb2c6.png#align=left&display=inline&height=70&originHeight=209&originWidth=3323&size=166030&status=done&style=none&width=1107.6666666666667)

使用以下命令配置成英文<br />
```bash
export LANG="en_US.UTF-8"
```
这时在执行pv和uv命令应该就可以统计出来了
<a name="G5YHT"></a>
### 7、获取最耗时的请求时间、url、耗时
下面是获取最耗时的前十个请求，如果想获取全部去掉：head -10
```bash
cat access.log | awk '{print $4,$7,$NF}' \
| awk -F '"' '{print $1,$2,$3}' | sort -k3 -rn | head -10
```
<a name="czuCk"></a>
### 8、获取每分钟的请求数量
```bash
cat access.log  | awk '{print substr($4,14,5)}' | uniq -c | awk '{print $2","$1}'
```
可以输出到csv格式文件中
```bash
cat access.log  | awk '{print substr($4,14,5)}' \
| uniq -c | awk '{print $2","$1}' > access.csv
```
<a name="9Vgju"></a>
### 9、查看某个时间段之间的ip访问个数
例如查看10点到19点的访问ip
```bash
grep "2020:1[0-9]" access.log | awk '{ips[$1]+=1} END{for(ip in ips) print ips[ip],ip}' | sort -nr | wc -l
```
查看10点到19点之间的ip访问数，并且访问数>=200的ip
```bash
grep '2020:1[0-19]' access.log | \
awk '{ips[$1]+=1}END{for(ip in ips) if(ips[ip]>=200) print ips[ip],ip}' | sort -nr
```
