Linux 时钟同步<br />Ubuntu系统默认的时钟同步服务器是ntp.ubuntu.com，Debian则是0.debian.pool.ntp.org等， 各Linux发行版都有自己的NTP官方服务器。身在中国，使用这些都会有高延迟，但对时钟同步这件事来说影响不大。<br />在某些环境下，比如公司内网、云上子网等，是与互联网隔绝的。这时要想做时钟同步，就只能自己配置了。<br />这里介绍如何自己配置时钟同步，不介绍如何自建NTP服务器。
<a name="Q8865"></a>
### 安装
一般timesync是预装的。如果没有，可以使用以下命令手动安装。
```bash
sudo apt install systemd-timesyncd
```
:::danger
它和ntp是冲突的，二者只能安装一个。
:::
<a name="QLWUk"></a>
### 配置
修改/etc/systemd/timesyncd.conf，把NTP设为华为内绿区可达的NTP服务器。
```
[Time]
NTP=ntp7.aliyun.com
NTP=ntp6.aliyun.com
```
修改完成后，需要restart后这个配置才生效。
```bash
sudo systemctl restart systemd-timesyncd.service
```
如果以上systemd-timesyncd.service因为什么原因而不存在，则可通过以下命令修复：
```bash
sudo dpkg-reconfigure systemd-timesyncd
```
<a name="eQiQF"></a>
### 查看效果
一般查看日期与时间是使用date。使用timedatectl可以查看到更多时钟同步相关信息。
```bash
$ timedatectl status
               Local time: 二 2020-09-22 20:06:05 CST
           Universal time: 二 2020-09-22 12:06:05 UTC
                 RTC time: 二 2020-09-22 12:06:05
                Time zone: Asia/Shanghai (CST, +0800)
System clock synchronized: yes
              NTP service: active
          RTC in local TZ: no
```
<a name="7ZGBM"></a>
### 国内NTP服务器列表
以下给出一些阿里云的NTP列表，可以通过ping择优使用。
```
ntp1.aliyun.com
ntp2.aliyun.com
ntp3.aliyun.com
ntp4.aliyun.com
ntp5.aliyun.com
ntp6.aliyun.com
ntp7.aliyun.com
```

