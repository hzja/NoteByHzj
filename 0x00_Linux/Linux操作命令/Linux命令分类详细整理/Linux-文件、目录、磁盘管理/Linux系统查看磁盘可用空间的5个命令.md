Linux
<a name="okmnF"></a>
## `df` 命令
`df` 命令是英文单词 `disk-free` 的缩写，用于查看 Linux 系统中的可用的和已经使用的磁盘空间。这个命令一般有以下几个常用选项：

- `df -h` ：以人类可读的格式显示磁盘空间（否则默认显示单位是字节，不直观）
- `df -a` ：包含全部的文件系统
```bash
df -ha
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558228151-2cbff4c2-f1db-4ff0-8b68-7e3d3385060f.png#clientId=ueafdf579-3a26-4&from=paste&height=426&id=u83491942&originHeight=1277&originWidth=1917&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226711&status=done&style=none&taskId=u1065cebf-6cc5-4953-8375-ed6cfe3c9e2&title=&width=639)

- `df -T` ：显示磁盘使用情况以及每个块的文件系统类型（例如，xfs、ext2、ext3、btrfs 等）
- `df -i` ：显示已使用和空闲的 inode
```bash
df -T
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558283215-a437d977-8b84-46c4-8557-742aabd865c6.png#clientId=ueafdf579-3a26-4&from=paste&height=487&id=ub225b99d&originHeight=1460&originWidth=2526&originalType=binary&ratio=1&rotation=0&showTitle=false&size=390766&status=done&style=none&taskId=u946ee62a-8113-412c-9b21-af22659392e&title=&width=842)<br />如果不喜欢敲代码，更喜欢使用图形界面，那么在 GNOME 桌面中可以使用一个叫 Disks 的软件（gnome-disk-utility）来获取这些信息。<br />Disks 启动之后可以查看计算机检测到的所有磁盘，然后单击分区以查看有关它的详细信息，包括已用空间和剩余空间。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636804769732-bb379c93-a2cb-4810-83a7-c7b7a8d349c6.webp#clientId=ua253cf84-63e5-4&from=paste&id=u7f8d23f4&originHeight=406&originWidth=676&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u242a744e-b022-4627-b171-88eb8c1ad5b&title=)
<a name="DXjwG"></a>
## `du` 命令
`du` 命令是英语单词 `disk useage` 的缩写，它是以默认千字节大小显示文件、文件夹等磁盘使用情况。常用的选项有以下几个：

- `du -h` ：以人类可读的格式显示所有目录和子目录的磁盘使用情况
- `du -a` ：显示所有文件的磁盘使用情况
- `du -s` ：仅显示总计，只列出最后加总的值
```bash
du -h
du -a
du -s
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558753608-a119495d-1458-40bb-9b17-5d57bf45866f.png#clientId=ueafdf579-3a26-4&from=paste&height=396&id=u6fd2cc4e&originHeight=1188&originWidth=2152&originalType=binary&ratio=1&rotation=0&showTitle=false&size=261359&status=done&style=none&taskId=u2033718c-5f2a-4499-9d9f-820e24b27df&title=&width=717.3333333333334)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558787581-cdd3f151-d52e-41a3-9fe6-bbbcbc0638e3.png#clientId=ueafdf579-3a26-4&from=paste&height=438&id=u9ce5c38e&originHeight=1313&originWidth=2458&originalType=binary&ratio=1&rotation=0&showTitle=false&size=383609&status=done&style=none&taskId=ud4538eb5-72fc-4dbc-aa24-5b8cb7a4d32&title=&width=819.3333333333334)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558808171-246319d4-bb4d-4407-b7ad-86ad240689a6.png#clientId=ueafdf579-3a26-4&from=paste&height=74&id=u015906bd&originHeight=223&originWidth=1349&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47936&status=done&style=none&taskId=u38b25548-5047-4c19-94c6-18ff1bcc22f&title=&width=449.6666666666667)<br />同样地，在 GNOME 桌面中，也有一个叫 Disk Usage 的软件，可以很直观查看磁盘的使用情况。而在 KDE 桌面中，对应的软件是 Filelight 软件。<br />在这两个软件中，磁盘使用情况被映射到一系列的同心圆里，中间是基本文件夹（通常是 /home 目录，但是可以自行设定），每个外环代表一个更深的目录级别。将鼠标悬停在任意位置上，就可以获取这部分磁盘占用空间的详细信息。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636804769761-8882b93b-6b1e-441a-9acc-eb448d069ed7.webp#clientId=ua253cf84-63e5-4&from=paste&id=uc42fbb9b&originHeight=646&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u320695a8-8a00-4bff-a5ba-f462d5e4d3e&title=)
<a name="LozRj"></a>
## `ls -al` 命令
`ls` 命令大家再熟悉不过了，使用 `ls -al` 命令可以列出特定目录的全部内容及其大小。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558835259-0fdec2e4-4602-480a-b9a7-d6f506d5d0d8.png#clientId=ueafdf579-3a26-4&from=paste&height=516&id=uc395eeeb&originHeight=1547&originWidth=2253&originalType=binary&ratio=1&rotation=0&showTitle=false&size=466863&status=done&style=none&taskId=ua88f5e24-be00-420c-b168-6ef9de2cf38&title=&width=751)
<a name="R0kEZ"></a>
## `stat` 命令
`stat` 命令后面可以直接跟上文件或目录，用于显示文件/目录或文件系统的大小和其他统计信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558887250-b0cf2933-4351-44ca-87c3-29e84cf79f73.png#clientId=ueafdf579-3a26-4&from=paste&height=148&id=u71d5dc97&originHeight=444&originWidth=1921&originalType=binary&ratio=1&rotation=0&showTitle=false&size=108555&status=done&style=none&taskId=uf2ae26f7-8660-4448-85f0-d935830bce4&title=&width=640.3333333333334)
<a name="ZCYiN"></a>
## Linux `fdisk -l` 命令
`fdisk -l` 可以显示磁盘大小以及磁盘分区信息。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1639558907721-74c88251-3352-431c-a8ed-c7954a473061.png#clientId=ueafdf579-3a26-4&from=paste&height=177&id=ufc6bfa7c&originHeight=530&originWidth=1737&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114754&status=done&style=none&taskId=u9889399a-6b3a-4e63-ac4c-4000807f078&title=&width=579)<br />以上这些命令是查看磁盘可用空间时非常常用的几个命令，而且都是 Linux 系统内置命令，无需额外安装。也有一些功能类似的第三方工具，比如 Disks 、Ncdu 等工具，可以直观显示磁盘空间利用率。
