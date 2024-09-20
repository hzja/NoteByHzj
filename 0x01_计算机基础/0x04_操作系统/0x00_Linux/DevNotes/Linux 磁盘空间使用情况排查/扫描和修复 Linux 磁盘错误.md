Linux<br />学习如何使用 fsck 和 xfs_repair 命令修复 Linux 磁盘错误。
<a name="o4w5d"></a>
## 什么是FSCK？
fsck（文件系统一致性检查）是用于检查Linux操作系统文件系统一致性的工具。
<a name="vm8NL"></a>
## 列出 Linux 磁盘分区和类型
首先，需要确定 Linux 系统的磁盘分区、它们各自的文件系统以及挂载点。<br />下面使用lsblk命令列出磁盘明、文件系统类型、挂载点：
```bash
[root@localhost ~]# lsblk -o NAME,FSTYPE,MOUNTPOINT
NAME        FSTYPE      MOUNTPOINT
sda                     
sdb                     
sdc                     
sdd                     
sr0         iso9660     
nvme0n1                 
├─nvme0n1p1 ext4        /boot
└─nvme0n1p2 LVM2_member 
  ├─cl-root xfs         /
  └─cl-swap swap        [SWAP]
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668574715-522634a2-e6ff-4e65-ba74-9ae2457934c8.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=u1c00acad&originHeight=213&originWidth=534&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud889a15e-9c69-4c10-9f13-98468e2cf5b&title=)
<a name="gXx5j"></a>
## 获取 Linux 磁盘的上次扫描时间
可以使用下命令找到 Ext2,Ext3,Ext4 类型分区的最后一次扫描时间。
```bash
[root@localhost ~]# tune2fs -l /dev/nvme0n1p1 |grep checked
Last checked:             Wed Sep  1 15:42:11 2021
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668574967-cc553699-1a02-493d-a2ab-b5ef919b730f.png#averageHue=%23010000&clientId=u4f058211-788e-4&from=paste&id=u7e37d65d&originHeight=60&originWidth=571&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud04d6327-cc96-45f3-b2c2-2d68cfce06f&title=)
<a name="JTayW"></a>
## 扫描和修复 Ext4 类型的磁盘分区
扫描 Linux 磁盘分区，可以使用 fsck（文件系统一致性检查）命令。但是在检查和修复它之前，需要卸载该分区。下面实例中，卸载的是/boot分区。
```bash
umount /dev/nvme0n1p1
```
卸载成功后，使用fsck.ext4检查/dev/nvme0n1p1分区
```bash
[root@localhost ~]# fsck.ext4 /dev/nvme0n1p1 
e2fsck 1.44.3 (10-July-2018)
/dev/nvme0n1p1: clean, 311/65536 files, 55546/262144 blocks
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668597985-c4bdab36-7712-46e9-a85e-a3360dbe0be2.png#averageHue=%23010000&clientId=u4f058211-788e-4&from=paste&id=u3e998519&originHeight=244&originWidth=678&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u638061e7-3de6-46ec-b195-703025cebbe&title=)<br />检查并修复您的 Linux 磁盘后，在其各自的挂载点再次挂载分区。执行以下命令用来挂载 /etc/fstab 配置文件中列出的所有磁盘分区。
```bash
mount -a
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668598001-3add7f17-e279-4799-82b3-45dee30ed832.png#averageHue=%23030000&clientId=u4f058211-788e-4&from=paste&id=u0a53564c&originHeight=204&originWidth=636&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u41f8a772-4f87-477f-886b-3eef7ed59ae&title=)
<a name="bh4x7"></a>
## 在 Linux 启动时启用 Ext4 磁盘分区扫描
在系统启动时启用磁盘检查。必须修改该磁盘分区的 Mount Count 参数。
```bash
[root@localhost ~]# tune2fs -c 1 /dev/nvme0n1p1 
tune2fs 1.44.3 (10-July-2018)
Setting maximal mount count to 1
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668598410-471ae01a-64f7-4423-a39b-942842599d08.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=u81401988&originHeight=94&originWidth=455&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue5c7dbcb-fa2c-40ed-8cab-f195db55f45&title=)<br />重启系统之后，fsck命令现在会在启动时检查 Ext4 磁盘分区。
```bash
reboot
```
重新启动后，获取磁盘分区的Last Checked值，现在它会显示 Linux 上次启动的时间。
```bash
[root@localhost ~]# tune2fs -l /dev/nvme0n1p1 |grep checked
Last checked:             Tue Sep 14 11:08:11 2021
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668632805-1c6e0d8f-81b8-4041-ab5f-a26e9211ca26.png#averageHue=%23010000&clientId=u4f058211-788e-4&from=paste&id=u5132e5e0&originHeight=58&originWidth=560&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10296e0b-b3d8-47f9-a461-bd60eb309bc&title=)<br />恢复Mount Count 参数的默认值，否则它会在每次系统启动时继续执行磁盘扫描。
```bash
[root@localhost ~]# tune2fs -c -1 /dev/nvme0n1p1 
tune2fs 1.44.3 (10-July-2018)
Setting maximal mount count to -1
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668633205-d62319e0-45ac-4a18-84e7-fc550677ef49.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=u2aebce80&originHeight=70&originWidth=555&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2df9b0ec-0e1b-46aa-9f7f-8879da60dd9&title=)
<a name="dGMvJ"></a>
## 什么是 XFS_REPAIR？
xfs_repair 实用程序具有高度可扩展性，旨在有效修复具有许多 inode 且非常大的文件系统。与其他 Linux 文件系统不同，xfs_repair 不会在启动时运行，即使 XFS 文件系统没有完全卸载。如果发生不干净的卸载，xfs_repair 只会在挂载时重放日志，确保文件系统一致。
<a name="NmLFF"></a>
## 扫描和修复 XFS 类型的磁盘分区
XFS 类型的磁盘分区有自己的一组命令，与 Ext4 有点不同。在检查其一致性之前，必须卸载 XFS 磁盘分区。
```bash
umount /dev/sda1
```
有 xfs_repair 命令用于检查和修复磁盘错误。使用xfs_repair 命令的-n选项来检查磁盘错误。
```
[root@localhost ~]# xfs_repair -n /dev/sda1 
Phase 1 - find and verify superblock...
Phase 2 - using internal log
        - zero log...
        - scan filesystem freespace and inode maps...
        - found root inode chunk
Phase 3 - for each AG...
        - scan (but don't clear) agi unlinked lists...
        - process known inodes and perform inode discovery...
        - agno = 0
        - agno = 1
        - agno = 2
        - agno = 3
        - process newly discovered inodes...
Phase 4 - check for duplicate blocks...
        - setting up duplicate extent list...
        - check for inodes claiming duplicate blocks...
        - agno = 0
        - agno = 1
        - agno = 2
        - agno = 3
No modify flag set, skipping phase 5
Phase 6 - check inode connectivity...
        - traversing filesystem ...
        - traversal finished ...
        - moving disconnected inodes to lost+found ...
Phase 7 - verify link counts...
No modify flag set, skipping filesystem flush and exiting.
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668633131-6eedd49c-26b5-488b-89ab-3ec4c7bc6a9a.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=u9ef59ee8&originHeight=481&originWidth=567&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8a06dcd6-5333-4ba3-93a0-f7841bb7f55&title=)<br />上述命令只执行磁盘检查，不尝试修复任何错误。现在，执行不带 `-n` 选项的 xfs_repair 命令，它将执行 Linux 磁盘分区的扫描和修复。
```
[root@localhost ~]# xfs_repair /dev/sda1
Phase 1 - find and verify superblock...
Phase 2 - using internal log
        - zero log...
        - scan filesystem freespace and inode maps...
        - found root inode chunk
Phase 3 - for each AG...
        - scan and clear agi unlinked lists...
        - process known inodes and perform inode discovery...
        - agno = 0
        - agno = 1
        - agno = 2
        - agno = 3
        - process newly discovered inodes...
Phase 4 - check for duplicate blocks...
        - setting up duplicate extent list...
        - check for inodes claiming duplicate blocks...
        - agno = 0
        - agno = 1
        - agno = 2
        - agno = 3
Phase 5 - rebuild AG headers and trees...
        - reset superblock...
Phase 6 - check inode connectivity...
        - resetting contents of realtime bitmap and summary inodes
        - traversing filesystem ...
        - traversal finished ...
        - moving disconnected inodes to lost+found ...
Phase 7 - verify and correct link counts...
done
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668672469-9337a195-8b6e-4dfd-8bb9-9e9c4495b2e3.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=ua867bc1c&originHeight=511&originWidth=637&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue6a46725-f7f7-465a-bfc5-c74c1853c3f&title=)<br />下面诚信挂载分区
```bash
mount /dev/sda1 /data/
```
<a name="E2EHk"></a>
## 在 Linux 启动时启用 XFS 磁盘分区扫描
在某些情况下，如果 Linux 操作系统正在使用磁盘分区，则无法卸载磁盘分区。出于这个原因，您可能不得不将磁盘检查推迟到下一次系统启动。<br />要使 xfs_repair 命令能够在 Linux 启动时运行，请在 GRUB 菜单内核命令的末尾添加“`fsck.mode=force fsck.repair=yes`”。<br />查看系统日志以验证磁盘修复命令的执行情况。
```bash
[root@localhost ~]# journalctl |grep systemd-fsck
```
要在启动时永久启用磁盘检查，必须在 GRUB 配置文件中添加“`fsck.mode=force fsck.repair=yes`”。<br />下面编辑grub配置文件，找到 GRUB_CMDLINE_LINUX 参数并在行尾附加`fsck.mode=force fsck.repair=yes`。
```
[root@localhost ~]# cat /etc/default/grub 
GRUB_TIMEOUT=5
GRUB_DISTRIBUTOR="$(sed 's, release .*$,,g' /etc/system-release)"
GRUB_DEFAULT=saved
GRUB_DISABLE_SUBMENU=true
GRUB_TERMINAL_OUTPUT="console"
GRUB_CMDLINE_LINUX="crashkernel=auto resume=/dev/mapper/cl-swap rd.lvm.lv=cl/root rd.lvm.lv=cl/swap fsck.mode=force fsck.repair=yes"
GRUB_DISABLE_RECOVERY="true"
GRUB_ENABLE_BLSCFG=true
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668672842-bcf35b14-bc69-42b2-8de5-19c67f603dc9.png#averageHue=%23010000&clientId=u4f058211-788e-4&from=paste&id=u45d673d6&originHeight=216&originWidth=862&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9c6cb71b-3d56-4b6d-ac86-5a6133761c4&title=)<br />根据新参数重新生成 GRUB 菜单配置。
```bash
grub2-mkconfig
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1678668672882-d964ce42-b7a9-4760-b4b6-bb6f12d5eae5.png#averageHue=%23000000&clientId=u4f058211-788e-4&from=paste&id=ufea67a32&originHeight=451&originWidth=586&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubb630b2f-bdc9-4176-93a8-a613ac4c135&title=)<br />然后重启系统：
```bash
reboot
```
<a name="DuID7"></a>
## 总结
到目前为止已经成功扫描和修复XFS、Ext4文件系统了。
