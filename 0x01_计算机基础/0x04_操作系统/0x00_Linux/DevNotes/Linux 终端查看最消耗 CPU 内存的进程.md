Linux 
<a name="rU74U"></a>
### 1.CPU占用最多的前10个进程
```bash
ps auxw|head -1;ps auxw|sort -rn -k3|head -10
```
<a name="YwftZ"></a>
### 2.内存消耗最多的前10个进程
```bash
ps auxw|head -1;ps auxw|sort -rn -k4 | head -10
```
<a name="F5yrT"></a>
### 3.虚拟内存使用最多的前10个进程
```bash
ps auxw|head -1;ps auxw|sort -rn -k5|head -10
```
<a name="NPL5J"></a>
### 4.其他查询命令
```bash
ps auxw --sort=rss
ps auxw --sort=%cpu
```
<a name="4cCHV"></a>
### 查询结果参数的含义

1. %MEM 进程的内存占用率；<br />
2. MAJFL is the major page fault count；<br />
3. VSZ 进程所使用的虚存的大小；<br />
4. RSS 进程使用的驻留集大小或者是实际内存的大小(RSS is the "resident 5. set size" meaning physical memory used)；<br />
5. TTY 与进程关联的终端（tty）。<br />
