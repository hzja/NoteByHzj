Linux<br />用ps和grep命令寻找僵尸进
```bash
ps -A -ostat,ppid,pid,cmd | grep -e '^[Zz]'
```
命令注解：

- `-A` 参数列出所有进程
- `-o` 自定义输出字段 设定显示字段为 stat（状态），ppid（进程父id），pid(进程id)，cmd（命令）这四个参数 因为状态为 z或者Z的进程为僵尸进程。

所以使用grep抓取stat状态为zZ进程，运行结果参考如下：
```bash
Z 12334 12339 /path/cmd
```
这时，可以使用 `kill -HUP 12339`来杀掉这个僵尸进程<br />运行后，可以再次运行
```bash
ps -A -ostat,ppid,pid,cmd | grep -e '^[Zz]'
```
来确认是否已经将僵尸进程杀死 如果kill 子进程的无效，可以尝试kill 其父进程来解决问题，例如上面例子父进程pid是 12334，那么就运行
```bash
kill -HUP 12334
```
来解决问题<br />一般可以用top命令发现动态进程表<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658727669109-1580bb59-30a7-49c7-b27e-fd8407abad86.png#clientId=ua1bfa51e-1df7-4&from=paste&height=704&id=ue0008487&originHeight=1760&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1473934&status=done&style=none&taskId=ud1f656fb-2717-42b1-afbc-cc8c61caf23&title=&width=1329.2)<br />其中zombie是僵尸进程。
