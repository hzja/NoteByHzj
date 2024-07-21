DBA Linux
<a name="wXIPO"></a>
#### 1. 按oracleSID来查询连接数据库的客户端IP
```bash
netstat -anpT | grep oracleSID | awk '{print $5}' | grep -o -E  '1.*:' | awk -F ':' '{print $1}' | sort
```
<a name="KF8Ot"></a>
#### 2. 按监听端口1521来查询连接数据库的客户端IP
```bash
netstat -anpT | grep 1521 | awk '{print $5}' | grep -o -E  '1.*:' | awk -F ':' '{print $1}' | sort
```
<a name="sMeBh"></a>
#### 3. 杀掉数据库服务器上连接数据库oracleSID的客户端的进程
```bash
kill -9 `ps -ef | grep oracleSID | grep LOCAL=NO | grep -v grep | awk '{print $2}'`
```
<a name="5LPFU"></a>
#### 4. 删掉某个用户userA下面的所有进程
```bash
pkill -9 -u userA
```
<a name="cecb859b"></a>
#### 5. 查询连接1521端口的process进程数
```bash
netstat -pan |grep 1521 |wc -l
```
<a name="61760377"></a>
#### 6. 查询来自某台服务器192.168.21.15的连接的process进程数
```bash
netstat -pan |grep  192.168.21.15 |wc -l
```
<a name="5D9tV"></a>
#### 7. 汇总连接1521端口的每个客户端IP的连接数
```bash
netstat -apnT|grep 1521 |awk  '{print $5}'|sort -u |grep -v 1521 |grep -v '*' |awk -F ':'  '{print $4}'|uniq -c |sort -nr
netstat -anpT|grep 1521 |awk  '{print $5}'|grep -o -E  '1.*:' |awk -F ':' '{print $1}' | sort |uniq -c |sort -nr
```
<a name="2drKZ"></a>
#### 8. 显示前10个占用空间最大的文件或目录 
```bash
du -s * | sort -nr | head
```
<a name="7UGf3"></a>
#### 9. 统计某一天的所有文件的总量大小 
```bash
ls --full-time `find ./* -name "log_*.bak"` | grep '2016-05-09' | awk '{print $9}' | xargs du -ck
```
<a name="059e6220"></a>
#### 10. 删除几天前的文件 
```bash
find /mitac/mds/arch/ -ctime +150 -exec rm -rf {} \;--常用 
find /mitac/mds/arch/ -name '*836701255.dbf' -ctime +150 -exec rm -rf {} \;--删除以836701255.dbf结尾，150天前被修改过的文件
```
<a name="8QxA7"></a>
#### 11. CPU占用最多的前10个进程： 
```bash
ps auxw|head -1;ps auxw|sort -rn -k3|head -10
```
<a name="o63ky"></a>
#### 12. 内存消耗最多的前10个进程 
```bash
ps auxw|head -1;ps auxw|sort -rn -k4|head -10
```
<a name="RRac0"></a>
#### 13. 虚拟内存使用最多的前10个进程 
```bash
ps auxw|head -1;ps auxw|sort -rn -k5|head -10 
top后按1，可以看到多少个cpu，按shift+p则按cpu排序，按shift+m则按内存排序
```
<a name="f0wqH"></a>
#### 14. vi替换所有字符串，如下把1都换成2
```bash
:%s/1/2/g
```
<a name="EzqI7"></a>
#### 15. 查看IO信息，如下 每1秒显示一次，一共显示3次，显示为MB
```bash
iostat -d -x -m 1 3
```
<a name="3YfhQ"></a>
#### 16. 查看cp一个文件file1到/u01目录下需要多长时间
```bash
time cp file1 /u01/
```
<a name="uuWoG"></a>
#### 17. 查看CPU某段时间比如7:00到9:00期间的使用率
```bash
sar -s 07:00:00 -e 10:00:00
```
