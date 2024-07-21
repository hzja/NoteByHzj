Linux<br />Linux 中的sleep和wait命令允许以选定的速度运行命令或在等待任务完成后捕获并显示任务的退出状态。sleep只是在命令之间插入一个定时暂停。wait命令在完成后才通知它已完成。
<a name="E4t9Y"></a>
## Sleep命令
sleep 命令会暂停指定的时间。它通常用于脚本中，但也适用于命令行。在下面的示例中， sleep 在两个 date 命令之间暂停30秒。
```bash
[root@localhost ~]# date ; sleep 30 ; date
Thu Nov 11 05:06:44 EST 2021
Thu Nov 11 05:07:14 EST 2021
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651019239137-1dbebd5f-1af7-477b-b625-cc9f1f9edee8.png#clientId=u525cfdb0-d006-4&from=paste&height=72&id=u84a09033&originHeight=179&originWidth=1517&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26215&status=done&style=none&taskId=ud4df3a67-5026-4a1b-bb2d-0c8e93e88d4&title=&width=606.8)<br />sleep 命令将数字参数作为秒数。但也可以通过在参数中添加一个字符来让它休眠不同的时间：

- 10s = 10 seconds
- 1m = 1 minute
- 2h = 2 hours
- 3d = 3 days
```bash
[root@localhost ~]# date ; sleep 10s ; date
Thu Nov 11 05:11:09 EST 2021
Thu Nov 11 05:11:19 EST 2021
```
睡眠时间也可以小于1秒钟，就是在数字前面加一个`.`.

- .1 = 十分之一秒
- .01 = 百分之一秒
- .001 = 千分之一秒
```bash
[root@localhost ~]# date ; sleep .1 ; date
Thu Nov 11 05:14:03 EST 2021
Thu Nov 11 05:14:03 EST 2021
```
sleep 命令通常用于定期运行命令，例如每 10 秒或每分钟一次，当要监视某些活动时。例如，可能正在等待同事登录文件服务器。可能会运行这样的命令：
```bash
[root@localhost ~]# while true ; do who ; sleep 1 ; done
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651019358343-8479bf73-db00-4dcf-b5d0-1f49f7f0c8b2.png#clientId=u525cfdb0-d006-4&from=paste&height=257&id=u24e4bdf8&originHeight=643&originWidth=1898&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112455&status=done&style=none&taskId=u1dfc8d05-1684-4c5f-8cd4-8714424579f&title=&width=759.2)<br />可以观察内存使用情况的变化。
```bash
[root@localhost ~]# while true ; do free -h ; sleep 5 ; done
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651019298294-eaea4764-dd72-4757-a694-dd06ff5232d8.png#clientId=u525cfdb0-d006-4&from=paste&height=352&id=ue6ba4586&originHeight=879&originWidth=2041&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94369&status=done&style=none&taskId=u7786db4d-06e9-49fa-9f7f-0e46eaf3ad9&title=&width=816.4)
<a name="GEsr1"></a>
## Wait命令
wait 命令在等待后台进程完成后捕获后台进程的退出状态。您也可以在脚本中使用它。将 sleep 命令替换为要等待的进程。此脚本还显示退出状态。
```bash
#!/bin/bash

sleep 5 &
process_id=$!
echo "PID: $process_id"
wait $process_id
echo "Exit status: $?"
```
第一行在后台启动 sleep 命令。第二行获取最近执行的后台进程的进程 ID`$!`。然后脚本显示该信息，等待该进程完成并显示退出状态。<br />如果使用 `wait -n`（不需要额外的参数），`wait` 将等待任何后台任务完成。在下面的示例中， sleep 6 命令首先完成，脚本结束，但其他两个 sleep 进程将继续在后台运行几秒钟。
```bash
#!/bin/bash

sleep 15 &
sleep 9 &
sleep 6 &
wait -n
echo “First job has been completed.”
```
如果改成以下脚本，它会在每个睡眠过程完成时告诉你。由于时间关系，这将按照与睡眠过程开始时相反的顺序发生。
```bash
#!/bin/bash

sleep 15 &
sleep 9 &
sleep 6 &
wait -n
echo "First job has been completed."
wait -n
echo "Next job has been completed."
wait
echo "All jobs have been completed."
```
注意：等待的进程必须是“子进程”（在当前 shell 中启动的进程）。
<a name="nz6as"></a>
## 总结
两者都可以在脚本或命令行中使用。但是，`wait` 命令仅适用于在后台运行的进程。
