<a name="kVCtM"></a>
### 1、首先使用`ps -ef` 命令能够得到当前系统运行的所有进程信息
```bash
ps -ef
```
<a name="W9FKb"></a>
### 2、查找想要获取的进程信息，例zookeeper
```bash
ps -ef | grep zookeeper
```
<a name="TAFMS"></a>
### 3、忽略grep查询进程名
```bash
ps -ef | grep zookeeper | grep -v grep
```
<a name="BzA9U"></a>
### 4、查找该进程的PID，执行操作
```bash
ps -ef | grep zookeeper | grep -v grep | awk '{print $2}'
```
获取到pid后，可以进行后续处理。比如执行kill等操作。<br />例如：
```bash
ps -ef | grep zookeeper | grep -v grep | awk '{print $2}' | xargs kill -9
```
