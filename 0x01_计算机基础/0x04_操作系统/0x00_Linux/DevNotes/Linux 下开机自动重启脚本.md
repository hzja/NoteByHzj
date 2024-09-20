Linux
<a name="H0Gwr"></a>
## 1、简介
经常碰到机器断电之后需要重启一大堆服务，为了防止这种事情发生，设置开机自启的脚本十分的重要，习惯性的做法就是编写一个重启脚本，然后在 `/etc/rc.local` 中去完成开机执行。例如下面这样：
```bash
$ cat /etc/rc.local
bash /root/script/restart.sh
```
这样的方法虽然可行，但并不优雅。介绍两种更好的实现方式：
<a name="hsadP"></a>
## 2、通过 Crontab 实现
Crontab 可以使用 `@reboot` 来执行主机启动之后的命令。首先在命令行输入：
```bash
$ crontab -e
```
然后添加以下内容：
```bash
@reboot /root/script/restart.sh
```
完成后，这个脚本就可以在重启的时候自动执行了。<br />其它的一些进阶玩法：

- 在启动完成后的指定时间内运行脚本
```bash
# 在启动 5 分钟后运行指定脚本
@reboot sleep 300 && /home/wwwjobs/clean-static-cache.sh
```
<a name="z76fv"></a>
## 3、通过 Systemd 实现
首先编写一个名为 restart 的 Systemd 服务：
```bash
$ vim /lib/systemd/system/restart.service

[Unit]
Description=restart
After=default.target

[Service]
ExecStart=/root/script/restart.sh

[Install]
WantedBy=default.target
```
然后启用这个 Systemd 服务：
```bash
$ systemctl daemon-reload
$ systemctl enable restart.service
```
完成后，这个服务对应的脚本就可以自动开机自启了。
