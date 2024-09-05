Linux 
<a name="43vQK"></a>
### 系统信息
```bash
arch      #显示机器的处理器架构(1)
uname -m  #显示机器的处理器架构(2)
uname -r  #显示正在使用的内核版本
dmidecode -q          #显示硬件系统部件 - (SMBIOS / DMI)
hdparm -i /dev/hda    #罗列一个磁盘的架构特性
hdparm -tT /dev/sda   #在磁盘上执行测试性读取操作
cat /proc/cpuinfo     #显示CPU info的信息
cat /proc/interrupts  #显示中断
cat /proc/meminfo     #校验内存使用
cat /proc/swaps       #显示哪些swap被使用
cat /proc/version     #显示内核的版本
cat /proc/net/dev     #显示网络适配器及统计
cat /proc/mounts      #显示已加载的文件系统
lspci -tv   #罗列PCI设备
lsusb -tv   #显示USB设备
Ispci：查看PCI设备  -v：查看详细信息
Isusb：查看USB设备 -v：查看详细信息
Ismod：查看加载的模块(驱动)
```
<a name="thlhd"></a>
### date 显示系统日期
```bash
cal 2007              #显示2007年的日历表
date 041217002007.00   #设置日期和时间 - 月日时分年.秒
clock -w              #将时间修改保存到 BIOS
date:查看或设置当前系统的时间：格式化显示时间：+%Y--%m--%d；
date -s:设置当前系统的时间
hwclock(clock)：显示硬件时钟时间(需要管理员权限)；
cal：查看日历
格式cal [参数] 月年
cal：显示当月的日历   cal4 2004 ：显示2004年4月的日历
cal- y 2003：显示2003年的日历
uptime：查看系统运行时间
```
<a name="BZOfP"></a>
### 关机 (系统的关机、重启以及登出 )
```bash
shutdown -h now    #关闭系统(1)
init 0            #关闭系统(2)
telinit 0         #关闭系统(3)
shutdown -h hours:minutes &   #按预定时间关闭系统
shutdown -c       #取消按预定时间关闭系统
shutdown -r now   #重启(1)
reboot   #重启(2)
logout   #注销
```
shutdown关闭、重启计算机<br />shutdown[关机、重启]时间  -h关闭计算机   -r：重启计算机<br />如：立即关机：shutdown -h now<br />10分钟后关机：shutdown -h +10<br />23:30分关机：shutdown -h 23:30<br />立即重启：shutdown -r now<br />poweroff：立即关闭计算机<br />reboot：立即重启计算机
<a name="XESpX"></a>
### 文件和目录
```bash
cd /home    #进入 '/ home' 目录'
cd ..       #返回上一级目录
cd ../..    #返回上两级目录
cd          #进入个人的主目录
cd ~user1   #进入个人的主目录
cd -       #返回上次所在的目录
pwd        #显示工作路径
ls      #查看目录中的文件
ls -F   #查看目录中的文件
ls -l   #显示文件和目录的详细资料
ls -a   #显示隐藏文件
ls *[0-9]*   #显示包含数字的文件名和目录名
tree         #显示文件和目录由根目录开始的树形结构(1)
lstree       #显示文件和目录由根目录开始的树形结构(2)
mkdir dir1         #创建一个叫做 'dir1' 的目录'
mkdir dir1 dir2    #同时创建两个目录
mkdir -p /tmp/dir1/dir2   #创建一个目录树
rm -f file1    #删除一个叫做 'file1' 的文件'
rmdir dir1     #删除一个叫做 'dir1' 的目录'
rm -rf dir1    #删除一个叫做 'dir1' 的目录并同时删除其内容
rm -rf dir1 dir2    #同时删除两个目录及它们的内容
mv dir1 new_dir     #重命名/移动 一个目录
cp file1 file2     #复制一个文件
cp dir/* .         #复制一个目录下的所有文件到当前工作目录
cp -a /tmp/dir1 .   #复制一个目录到当前工作目录
cp -a dir1 dir2     #复制一个目录
ln -s file1 lnk1  #创建一个指向文件或目录的软链接
ln file1 lnk1     #创建一个指向文件或目录的物理链接
touch -t 0712250000 file1   #修改一个文件或目录的时间戳 - (YYMMDDhhmm)
file file1 outputs the mime type of the file as text
iconv -l   #列出已知的编码
iconv -f fromEncoding -t toEncoding inputFile > outputFile creates a new from the given input file by assuming it is encoded in fromEncoding and converting it to toEncoding.
find . -maxdepth 1 -name *.jpg -print -exec convert "{}" -resize 80x60 "thumbs/{}" \; batch resize files in the current directory and send them to a thumbnails directory (requires convert from Imagemagick)
```
ls命令，查看文件与目录的命令，list之意
```bash
ls 查看目录中的文件 
ls -l 显示文件和目录的详细资料 
ls -a 列出全部文件，包含隐藏文件
ls -R 连同子目录的内容一起列出（递归列出），等于该目录下的所有文件都会显示出来  
ls [0-9] 显示包含数字的文件名和目录名
ls -ld：显示目录和链接信息；
# ctrl+r：历史记录中所搜命令（输入命令中的任意一个字符）；
# Linux中以.开头的文件是隐藏文件；
```
cp命令，用于复制文件，copy之意，它还可以把多个文件一次性地复制到一个目录下
```bash
-a ：将文件的特性一起复制
-p ：连同文件的属性一起复制，而非使用默认方式，与-a相似，常用于备份
-i ：若目标文件已经存在时，在覆盖时会先询问操作的进行
-r ：递归持续复制，用于目录的复制行为 //经常使用递归复制
-u ：目标文件与源文件有差异时才会复制
```
mv命令，用于移动文件、目录或更名，move之意
```bash
-f ：force强制的意思，如果目标文件已经存在，不会询问而直接覆盖
-i ：若目标文件已经存在，就会询问是否覆盖
-u ：若目标文件已经存在，且比目标文件新，才会更新
```
rm命令，用于删除文件或目录，remove之意
```bash
-f ：就是force的意思，忽略不存在的文件，不会出现警告消息
-i ：互动模式，在删除前会询问用户是否操作
-r ：递归删除，最常用于目录删除，它是一个非常危险的参数
```
1、cp：复制文件和目录 cp源文件（文件夹）目标文件（文件夹）<br />常用参数：-r:递归复制整个目录树；-v：显示详细信息；<br />复制文件夹时要在cp命令后面加一个-r参数：<br />如：cp -r 源文件夹  目标文件夹<br />2、touch+文件名：当文件不存在的时候，创建相应的文件；当文件存在的时候，修改文件的创建时间。<br />功能：生成一个空文件或修改文件的存取/修改的时间记录值。<br />touch * ：将当前下的文件时间修改为系统的当前时间<br />touch –d 20040210 test：将test文件的日期改为20040210<br />touch abc ：若abc文件存在，则修改为系统的当前时间；若不存在，则生成一个为当前时间的空文件<br />3、mv 文件 目标目录：移动或重命名文件或目录（如果指定文件名，则可以重命名文件）。可以将文件及目录移到另一目录下，或更改文件及目录的名称。<br />格式为：mv [参数]<源文件或目录> <目标文件或目录><br />mva.txt ../：将a.txt文件移动上层目录<br />mv a.txt b.txt：将a.txt改名为b.txt<br />mvdir2 ../：将dir2目录上移一层<br />4、rm：删除文件；<br />常用参数：-i：交互式   -r：递归的删除包括目录中的所有内容<br />5、mkdir +文件夹名称：创建文件夹；<br />6、rm -r +文件夹名称：删除文件夹（空文件夹和非空文件夹都可删除）<br />rmdir 文件夹名称：删除文件夹（只能删除空文件夹）<br />7、mkdir  -p dir1/dir2 ：在当前目录下创建dir1目录，并在dir1目录下创建dir2目录， 也就是连续创建两个目录（dir1/和dir1/dir2）<br />8、rmdir –p dir1/dir2：删除dir1下的dir2目录，若dir1目录为空也删除它<br />9、rm * ：删除当前目录下的所有文件<br />10、-f参数：强迫删除文件 rm –f  *.txt：强迫删除所有以后缀名为txt文件<br />11、-i参数：删除文件时询问<br />rm –i  * ：删除当前目录下的所有文件会有如下提示：<br />rm:backup:is a directory 遇到目录会略过<br />rm: remove ‘myfiles.txt’ ? Y<br />删除文件时会询问,可按Y或N键表示允许或拒绝删除文件 <br />12、-r参数：递归删除（连子目录一同删除，这是一个相当常用的参数）<br />rm  -r test ：删除test目录（含test目录下所有文件和子目录）<br />rm  -r  *：删除所有文件（含当前目录所有文件、所有子目录和子目录下的文件） 一般在删除目录时r和f一起用，避免麻烦<br />rm  -rf test ：强行删除、不加询问<br />13、grep：功能：在文件中搜索匹配的字符并进行输出<br />格式：grep[参数] <要找的字串> <要寻找字 串的源文件><br />greplinux test.txt：搜索test.txt文件中字符串linux并输出<br />14、ln命令<br />功能：在文件和目录之间建立链接<br />格式：ln [参数] <源文件或目录> <目标文件或目录><br />链接分“软链接”和“硬链接”<br />1.软链接:<br />ln–s /usr/share/do  doc ：创建一个链接文件doc,并指向目录/usr/share/do<br />2.硬链接:<br />ln  /usr/share/test  hard：创建一个硬链接文件hard，这时对于test文件对应 的存储区域来说，又多了一个文件指向它
<a name="l52Kf"></a>
### 文件搜索
```bash
find / -name file1     #从 '/' 开始进入根文件系统搜索文件和目录
find / -user user1     #搜索属于用户 'user1' 的文件和目录
find /home/user1 -name \*.bin        #在目录 '/ home/user1' 中搜索带有'.bin' 结尾的文件
find /usr/bin -type f -atime +100    #搜索在过去100天内未被使用过的执行文件
find /usr/bin -type f -mtime -10     #搜索在10天内被创建或者修改过的文件
find / -name \*.rpm -exec chmod 755 '{}' \;      #搜索以 '.rpm' 结尾的文件并定义其权限
find / -xdev -name \*.rpm        #搜索以 '.rpm' 结尾的文件，忽略光驱、捷盘等可移动设备
find /var/mail/ -size +50M -exec rm {} ＼;   #删除大于50M的文件
locate \*.ps       #寻找以 '.ps' 结尾的文件 - 先运行 'updatedb' 命令
whereis halt       #显示一个二进制文件、源码或man的位置
which halt         #显示一个二进制文件或可执行文件的完整路径
```
locate：快速查找文件、文件夹：locate keyword<br />此命令需要预先建立数据库，数据库默认每天更新一次，可用updatedb命令手工建立、更新数据库。<br />find查找位置查找参数<br />如：<br />find . -name *liuyazhuang* 查找当前目录下名称中含有"liuyazhuang"的文件<br />find / -name *.conf  查找根目录下（整个硬盘）下后缀为.conf的文件<br />find / -perm 777 查找所有权限是777的文件<br />find / -type d 返回根目录下所有的目录<br />find . -name "a*"-exec ls -l {} \;<br />find功能：用来寻找文件或目录。<br />格式：find [<路径>] [匹配条件]<br />find / -name httpd.conf  搜索系统根目录下名为httpd.conf的文件
<a name="FODQ8"></a>
### 挂载一个文件系统
```bash
mount /dev/hda2 /mnt/hda2    #挂载一个叫做hda2的盘 - 确定目录 '/ mnt/hda2' 已经存在
umount /dev/hda2            #卸载一个叫做hda2的盘 - 先从挂载点 '/ mnt/hda2' 退出
fuser -km /mnt/hda2         #当设备繁忙时强制卸载
umount -n /mnt/hda2         #运行卸载操作而不写入 /etc/mtab 文件- 当文件为只读或当磁盘写满时非常有用
mount /dev/fd0 /mnt/floppy        #挂载一个软盘
mount /dev/cdrom /mnt/cdrom       #挂载一个cdrom或dvdrom
mount /dev/hdc /mnt/cdrecorder    #挂载一个cdrw或dvdrom
mount /dev/hdb /mnt/cdrecorder    #挂载一个cdrw或dvdrom
mount -o loop file.iso /mnt/cdrom    #挂载一个文件或ISO镜像文件
mount -t vfat /dev/hda5 /mnt/hda5    #挂载一个Windows FAT32文件系统
mount /dev/sda1 /mnt/usbdisk         #挂载一个usb 捷盘或闪存设备
mount -t smbfs -o username=user,password=pass //WinClient/share /mnt/share      #挂载一个windows网络共享
```
<a name="jkocj"></a>
### 磁盘空间
```bash
df -h           #显示已经挂载的分区列表
ls -lSr |more    #以尺寸大小排列文件和目录
du -sh dir1      #估算目录 'dir1' 已经使用的磁盘空间'
du -sk * | sort -rn     #以容量大小为依据依次显示文件和目录的大小
rpm -q -a --qf '%10{SIZE}t%{NAME}n' | sort -k1,1n 
#以大小为依据依次显示已安装的rpm包所使用的空间 (fedora, redhat类系统)
dpkg-query -W -f='${Installed-Size;10}t${Package}n' | sort -k1,1n 
#以大小为依据显示已安装的deb包所使用的空间 (ubuntu, debian类系统)
```
<a name="uNo1T"></a>
### 用户和群组
```bash
groupadd group_name   #创建一个新用户组
groupdel group_name   #删除一个用户组
groupmod -n new_group_name old_group_name   #重命名一个用户组
useradd -c "Name Surname " -g admin -d /home/user1 -s /bin/bash user1     #创建一个属于 "admin" 用户组的用户
useradd user1      #创建一个新用户
userdel -r user1   #删除一个用户 ( '-r' 排除主目录)
usermod -c "User FTP" -g system -d /ftp/user1 -s /bin/nologin user1   #修改用户属性
passwd         #修改口令
passwd user1   #修改一个用户的口令 (只允许root执行)
chage -E 2005-12-31 user1    #设置用户口令的失效期限
pwck     #检查 '/etc/passwd' 的文件格式和语法修正以及存在的用户
grpck    #检查 '/etc/passwd' 的文件格式和语法修正以及存在的群组
newgrp group_name     #登陆进一个新的群组以改变新创建文件的预设群组
```
<a name="ruxVz"></a>
### 文件的权限
使用 "+" 设置权限，使用 "-" 用于取消
```bash
ls -lh    #显示权限
ls /tmp | pr -T5 -W$COLUMNS   #将终端划分成5栏显示
chmod ugo+rwx directory1      #设置目录的所有人(u)、群组(g)以及其他人(o)以读（r ）、写(w)和执行(x)的权限
chmod go-rwx directory1      #删除群组(g)与其他人(o)对目录的读写执行权限
chown user1 file1            #改变一个文件的所有人属性
chown -R user1 directory1    #改变一个目录的所有人属性并同时改变改目录下所有文件的属性
chgrp group1 file1          #改变文件的群组
chown user1:group1 file1     #改变一个文件的所有人和群组属性
find / -perm -u+s           #罗列一个系统中所有使用了SUID控制的文件
chmod u+s /bin/file1        #设置一个二进制文件的 SUID 位 - 运行该文件的用户也被赋予和所有者同样的权限
chmod u-s /bin/file1        #禁用一个二进制文件的 SUID位
chmod g+s /home/public      #设置一个目录的SGID 位 - 类似SUID ，不过这是针对目录的
chmod g-s /home/public      #禁用一个目录的 SGID 位
chmod o+t /home/public      #设置一个文件的 STIKY 位 - 只允许合法所有人删除文件
chmod o-t /home/public      #禁用一个目录的 STIKY 位
```
<a name="xf1a2"></a>
### 文件的特殊属性
- 使用 "+" 设置权限，使用 "-" 用于取消
```bash
chattr +a file1   #只允许以追加方式读写文件
chattr +c file1   #允许这个文件能被内核自动压缩/解压
chattr +d file1   #在进行文件系统备份时，dump程序将忽略这个文件
chattr +i file1   #设置成不可变的文件，不能被删除、修改、重命名或者链接
chattr +s file1   #允许一个文件被安全地删除
chattr +S file1   #一旦应用程序对这个文件执行了写操作，使系统立刻把修改的结果写到磁盘
chattr +u file1   #若文件被删除，系统会允许你在以后恢复这个被删除的文件
lsattr           #显示特殊的属性
```
<a name="xVn3m"></a>
### 打包和压缩文件
```bash
bunzip2 file1.bz2   #解压一个叫做 'file1.bz2'的文件
bzip2 file1         #压缩一个叫做 'file1' 的文件
gunzip file1.gz     #解压一个叫做 'file1.gz'的文件
gzip file1          #压缩一个叫做 'file1'的文件
gzip -9 file1       #最大程度压缩
rar a file1.rar test_file          #创建一个叫做 'file1.rar' 的包
rar a file1.rar file1 file2 dir1   #同时压缩 'file1', 'file2' 以及目录 'dir1'
rar x file1.rar     #解压rar包
unrar x file1.rar   #解压rar包
tar -cvf archive.tar file1   #创建一个非压缩的 tarball
tar -cvf archive.tar file1 file2 dir1  #创建一个包含了 'file1', 'file2' 以及 'dir1'的档案文件
tar -tf archive.tar    #显示一个包中的内容
tar -xvf archive.tar   #释放一个包
tar -xvf archive.tar -C /tmp     #将压缩包释放到 /tmp目录下
tar -cvfj archive.tar.bz2 dir1   #创建一个bzip2格式的压缩包
tar -jxvf archive.tar.bz2        #解压一个bzip2格式的压缩包
tar -cvfz archive.tar.gz dir1    #创建一个gzip格式的压缩包
tar -zxvf archive.tar.gz         #解压一个gzip格式的压缩包
zip file1.zip file1    #创建一个zip格式的压缩包
zip -r file1.zip file1 file2 dir1    #将几个文件和目录同时压缩成一个zip格式的压缩包
unzip file1.zip    #解压一个zip格式压缩包
```
tar命令，对文件进行打包，默认情况并不会压缩，如果指定了相应的参数，它还会调用相应的压缩程序（如gzip和bzip等）进行压缩和解压
```
-c ：新建打包文件
-t ：查看打包文件的内容含有哪些文件名
-x ：解打包或解压缩的功能，可以搭配-C（大写）指定解压的目录，注意-c,-t,-x不能同时出现在同一条命令中
-j ：通过bzip2的支持进行压缩/解压缩
-z ：通过gzip的支持进行压缩/解压缩
-v ：在压缩/解压缩过程中，将正在处理的文件名显示出来
-f filename ：filename为要处理的文件
-C dir ：指定压缩/解压缩的目录dir
```
归档、压缩<br />zip:压缩文件  zip liuyazhuang.zip myfile  格式为：“zip 压缩后的zip文件文件名”<br />unzip：解压文件  unzip liuyazhuang.zip<br />gzip：压缩文件 gzip 文件名<br />tar：归档文件<br />tar -cvf out.tar liuyazhuang  打包一个归档（将文件"liuyazhuang"打包成一个归档）<br />tar -xvf liuyazhuang.tar     释放一个归档（释放liuyazhuang.tar归档）<br />tar -cvzf backup.tar.gz/etc  <br />-z参数将归档后的归档文件进行gzip压缩以减少大小。<br />-c：创建一个新tar文件<br />-v：显示运行过程的信息<br />-f：指定文件名<br />-z：调用gzip压缩命令进行压缩<br />-t：查看压缩文件的内容<br />-x：解开tar文件<br />tar  -cvf test.tar  *：将所有文件打包成test.tar,扩展名.tar需自行加上<br />tar  -zcvf test.tar.gz  *：将所有文件打包成test.tar,再用gzip命令压缩<br />tar -tf   test.tar ：查看test.tar文件中包括了哪些文件<br />tar -xvf test.tar       将test.tar解开<br />tar -zxvf foo.tar.gz   解压缩<br />gzip各gunzip命令<br />gziptest.txt ：压缩文件时，不需要任何参数<br />gizp–l test.txt.gz：显示压缩率
<a name="Apj3C"></a>
### RPM 包 - （Fedora, Redhat及类似系统）
```bash
rpm -ivh package.rpm    #安装一个rpm包
rpm -ivh --nodeeps package.rpm   #安装一个rpm包而忽略依赖关系警告
rpm -U package.rpm        #更新一个rpm包但不改变其配置文件
rpm -F package.rpm        #更新一个确定已经安装的rpm包
rpm -e package_name.rpm   #删除一个rpm包
rpm -qa      #显示系统中所有已经安装的rpm包
rpm -qa | grep httpd    #显示所有名称中包含 "httpd" 字样的rpm包
rpm -qi package_name    #获取一个已安装包的特殊信息
rpm -qg "System Environment/Daemons"     #显示一个组件的rpm包
rpm -ql package_name       #显示一个已经安装的rpm包提供的文件列表
rpm -qc package_name       #显示一个已经安装的rpm包提供的配置文件列表
rpm -q package_name --whatrequires     #显示与一个rpm包存在依赖关系的列表
rpm -q package_name --whatprovides    #显示一个rpm包所占的体积
rpm -q package_name --scripts         #显示在安装/删除期间所执行的脚本l
rpm -q package_name --changelog       #显示一个rpm包的修改历史
rpm -qf /etc/httpd/conf/httpd.conf    #确认所给的文件由哪个rpm包所提供
rpm -qp package.rpm -l    #显示由一个尚未安装的rpm包提供的文件列表
rpm --import /media/cdrom/RPM-GPG-KEY    #导入公钥数字证书
rpm --checksig package.rpm      #确认一个rpm包的完整性
rpm -qa gpg-pubkey      #确认已安装的所有rpm包的完整性
rpm -V package_name     #检查文件尺寸、 许可、类型、所有者、群组、MD5检查以及最后修改时间
rpm -Va                 #检查系统中所有已安装的rpm包- 小心使用
rpm -Vp package.rpm     #确认一个rpm包还未安装
rpm2cpio package.rpm | cpio --extract --make-directories *bin*   #从一个rpm包运行可执行文件
rpm -ivh /usr/src/redhat/RPMS/`arch`/package.rpm    #从一个rpm源码安装一个构建好的包
rpmbuild --rebuild package_name.src.rpm       #从一个rpm源码构建一个 rpm 包
```
<a name="S2KZn"></a>
#### 1、软件包的安装
使用RPM命令的安装模式可以将软件包内所有的组件放到系统中的正确路径，安装软件包的命令是：`rpm –ivh wu-ftpd-2.6.2-8.i386.rpm`<br />i：作用rpm的安装模式 v: 校验文件信息h: 以＃号显示安装进度
<a name="unfSB"></a>
#### 2、软件包的删除
删除模式会将指定软件包的内容全部删除，但并不包括已更改过的配置文件，删除RPM软件包的命令如下：`rpm –e  wu-ftpd`<br />注意：这里必须使用软件名“wu-ftpd”或”wu-ftpd-2.6.2-8而不是使用当初安装时的软件包名.wu-ftpd-2.6.2-8.i386.rpm
<a name="Bhjcf"></a>
#### 3、软件包升级
升级模式会安装用户所指定的更新版本，并删除已安装在系统中的相同软件包，升级软件包命令如下：`rpm –Uvh wu-ftpd-2.6.2-8.i386.rpm`  –Uvh：升级参数
<a name="Iphg1"></a>
#### 4、软件包更新
更新模式下，rpm命令会检查在命令行中所指定的软件包是否比系统中原有的软件 包更新。如果情况属实，rpm命令会自动更新指定的软件包；反之，若系统中并没有指定软件包的较旧版本，rpm命令并不会安装此软件包。而在升级模式下，不管系统中是否有较旧的版本，rpm命令都会安装指定的软件包。<br />`rpm –Fvhwu-ftpd-2.6.2-8.i386.rpm`   -Fvh：更新参数
<a name="qKRf9"></a>
#### 5、软件包查询
若要获取RPM软件包的相关信息，可以使用查询模式。使用-q参数可查询一个已   安装的软件包的内容<br />`rpm  –q wu-ftpd`<br />查询软件包所安装的位置：rpm –ql package-name<br />`rpm –ql xv` (l参数：显示文件列表)
<a name="CmGSR"></a>
### YUM 软件包升级器 - （Fedora, RedHat及类似系统）
```bash
yum install package_name             #下载并安装一个rpm包
yum localinstall package_name.rpm    #将安装一个rpm包，使用你自己的软件仓库为你解决所有依赖关系
yum update package_name.rpm    #更新当前系统中所有安装的rpm包
yum update package_name        #更新一个rpm包
yum remove package_name        #删除一个rpm包
yum list                   #列出当前系统中安装的所有包
yum search package_name     #在rpm仓库中搜寻软件包
yum clean packages          #清理rpm缓存删除下载的包
yum clean headers           #删除所有头文件
yum clean all                #删除所有缓存的包和头文件
```
<a name="tl3A6"></a>
### DEB 包 (Debian, Ubuntu 以及类似系统)
```bash
dpkg -i package.deb     #安装/更新一个 deb 包
dpkg -r package_name    #从系统删除一个 deb 包
dpkg -l                 #显示系统中所有已经安装的 deb 包
dpkg -l | grep httpd    #显示所有名称中包含 "httpd" 字样的deb包
dpkg -s package_name    #获得已经安装在系统中一个特殊包的信息
dpkg -L package_name    #显示系统中已经安装的一个deb包所提供的文件列表
dpkg --contents package.deb    #显示尚未安装的一个包所提供的文件列表
dpkg -S /bin/ping              #确认所给的文件由哪个deb包提供
```
<a name="4YTn6"></a>
### APT 软件工具 (Debian, Ubuntu 以及类似系统)
```bash
apt-get install package_name      #安装/更新一个 deb 包
apt-cdrom install package_name    #从光盘安装/更新一个 deb 包
apt-get update      #升级列表中的软件包
apt-get upgrade     #升级所有已安装的软件
apt-get remove package_name     #从系统删除一个deb包
apt-get check     #确认依赖的软件仓库正确
apt-get clean     #从下载的软件包中清理缓存
apt-cache search searched-package    #返回包含所要搜索字符串的软件包名称
```
<a name="kY8Ml"></a>
### 查看文件内容
```bash
cat file1      #从第一个字节开始正向查看文件的内容
tac file1      #从最后一行开始反向查看一个文件的内容
more file1     #查看一个长文件的内容
less file1     #类似于 'more' 命令，但是它允许在文件中和正向操作一样的反向操作
head -2 file1    #查看一个文件的前两行
tail -2 file1    #查看一个文件的最后两行
tail -f /var/log/messages     #实时查看被添加到一个文件中的内容
```
echo：显示输入的内容  追加文件echo "liuyazhuang" >> liuyazhuang.txt<br />cat：显示文件内容,也可以将数个文件合并成一个文件。<br />格式：格式：cat[参数]<文件名><br />cat  test.txt：显示test.txt文件内容<br />cat  test.txt | more  ：逐页显示test.txt文件中的内容<br />cat  test.txt >> test1.txt ：将test.txt的内容附加到test1.txt文件之后<br />cat  test.txt test2.txt >readme.txt : 将test.txt和test2.txt文件合并成readme.txt 文件<br />head:显示文件的头几行（默认10行） -n:指定显示的行数格式：head -n 文件名<br />tail：显示文件的末尾几行（默认10行）-n：指定显示的行数   -f：追踪显示文件更新 （一般用于查看日志，命令不会退出，而是持续显示新加入的内容）<br />格式：格式：tail[参数]<文件名><br />tail-10 /etc/passwd ：显示/etc/passwd/文件的倒数10行内容<br />tail+10 /etc/passwd ：显示/etc/passwd/文件从第10行开始到末尾的内容<br />more：用于翻页显示文件内容（只能向下翻页）<br />more命令是一般用于要显示的内容会超过一个画面长度的情况。为了避免画  面显示时瞬间就闪过去，用户可以使用more命令，让画面在显示满一页时暂停，此时可按空格健继续显示下一个画面，或按Q键停止显示。<br />ls  -al  |more：以长格形式显示etc目录下的文件列表，显示满一个画面便暂停，可 按空格键继续显示下一画面，或按Q键跳离<br />less：翻页显示文件内容（带上下翻页）按下上键分页，按q退出、‘<br />less命令的用法与more命令类似，也可以用来浏览超过一页的文件。所不同  的是less 命令除了可以按空格键向下显示文件外，还可以利用上下键来卷动文件。当要结束浏览时，只要在less命令的提示符“：”下按Q键即可。<br />ls  -al | less：以长格形式列出/etc目录中所有的内容。用户可按上下键浏览或按Q键跳离
<a name="BWgV1"></a>
### 文本处理
```bash
cat file1 file2 ... | command <> file1_in.txt_or_file1_out.txt general syntax for text manipulation using PIPE, STDIN and STDOUT
cat file1 | command( sed, grep, awk, grep, etc...) > result.txt 
#合并一个文件的详细说明文本，并将简介写入一个新文件中
cat file1 | command( sed, grep, awk, grep, etc...) >> result.txt 
#合并一个文件的详细说明文本，并将简介写入一个已有的文件中
grep Aug /var/log/messages     #在文件 '/var/log/messages'中查找关键词"Aug"
grep ^Aug /var/log/messages    #在文件 '/var/log/messages'中查找以"Aug"开始的词汇
grep [0-9] /var/log/messages   #选择 '/var/log/messages' 文件中所有包含数字的行
grep Aug -R /var/log/*         #在目录 '/var/log' 及随后的目录中搜索字符串"Aug"
sed 's/stringa1/stringa2/g' example.txt 
#将example.txt文件中的 "string1" 替换成 "string2"
sed '/^$/d' example.txt           #从example.txt文件中删除所有空白行
sed '/ *#/d; /^$/d' example.txt   #从example.txt文件中删除所有注释和空白行
echo 'esempio' | tr '[:lower:]' '[:upper:]'    #合并上下单元格内容
sed -e '1d' result.txt          #从文件example.txt 中排除第一行
sed -n '/stringa1/p'            #查看只包含词汇 "string1"的行
sed -e 's/ *$//' example.txt    #删除每一行最后的空白字符
sed -e 's/stringa1//g' example.txt  
#从文档中只删除词汇 "string1" 并保留剩余全部
sed -n '1,5p;5q' example.txt     #查看从第一行到第5行内容
sed -n '5p;5q' example.txt       #查看第5行
sed -e 's/00*/0/g' example.txt   #用单个零替换多个零
cat -n file1       #标示文件的行数
cat example.txt | awk 'NR%2==1'      #删除example.txt文件中的所有偶数行
echo a b c | awk '{print $1}'        #查看一行第一栏
echo a b c | awk '{print $1,$3}'     #查看一行的第一和第三栏
paste file1 file2           #合并两个文件或两栏的内容
paste -d '+' file1 file2    #合并两个文件或两栏的内容，中间用"+"区分
sort file1 file2              #排序两个文件的内容
sort file1 file2 | uniq       #取出两个文件的并集(重复的行只保留一份)
sort file1 file2 | uniq -u    #删除交集，留下其他的行
sort file1 file2 | uniq -d    #取出两个文件的交集(只留下同时存在于两个文件中的文件)
comm -1 file1 file2    #比较两个文件的内容只删除 'file1' 所包含的内容
comm -2 file1 file2    #比较两个文件的内容只删除 'file2' 所包含的内容
comm -3 file1 file2    #比较两个文件的内容只删除两个文件共有的部分
```
<a name="NReUT"></a>
### 字符设置和文件格式转换
```bash
dos2unix filedos.txt fileunix.txt      #将一个文本文件的格式从MSDOS转换成UNIX
unix2dos fileunix.txt filedos.txt      #将一个文本文件的格式从UNIX转换成MSDOS
recode ..HTML < page.txt > page.html   #将一个文本文件转换成html
recode -l | more                       #显示所有允许的转换格式
```
<a name="zu0oq"></a>
### 文件系统分析
```bash
badblocks -v /dev/hda1    #检查磁盘hda1上的坏磁块
fsck /dev/hda1            #修复/检查hda1磁盘上linux文件系统的完整性
fsck.ext2 /dev/hda1       #修复/检查hda1磁盘上ext2文件系统的完整性
e2fsck /dev/hda1          #修复/检查hda1磁盘上ext2文件系统的完整性
e2fsck -j /dev/hda1       #修复/检查hda1磁盘上ext3文件系统的完整性
fsck.ext3 /dev/hda1       #修复/检查hda1磁盘上ext3文件系统的完整性
fsck.vfat /dev/hda1       #修复/检查hda1磁盘上fat文件系统的完整性
fsck.msdos /dev/hda1      #修复/检查hda1磁盘上dos文件系统的完整性
dosfsck /dev/hda1         #修复/检查hda1磁盘上dos文件系统的完整性
```
<a name="W28t0"></a>
### 初始化一个文件系统
```bash
mkfs /dev/hda1        #在hda1分区创建一个文件系统
mke2fs /dev/hda1      #在hda1分区创建一个linux ext2的文件系统
mke2fs -j /dev/hda1   #在hda1分区创建一个linux ext3(日志型)的文件系统
mkfs -t vfat 32 -F /dev/hda1   #创建一个 FAT32 文件系统
fdformat -n /dev/fd0           #格式化一个软盘
mkswap /dev/hda3               #创建一个swap文件系统
```
<a name="So1on"></a>
### SWAP文件系统
```bash
mkswap /dev/hda3             #创建一个swap文件系统
swapon /dev/hda3             #启用一个新的swap文件系统
swapon /dev/hda2 /dev/hdb3   #启用两个swap分区
```
<a name="XcmcJ"></a>
### 备份
```bash
dump -0aj -f /tmp/home0.bak /home    #制作一个 '/home' 目录的完整备份
dump -1aj -f /tmp/home0.bak /home    #制作一个 '/home' 目录的交互式备份
restore -if /tmp/home0.bak          #还原一个交互式备份
rsync -rogpav --delete /home /tmp    #同步两边的目录
rsync -rogpav -e ssh --delete /home ip_address:/tmp           #通过SSH通道rsync
rsync -az -e ssh --delete ip_addr:/home/public /home/local    #通过ssh和压缩将一个远程目录同步到本地目录
rsync -az -e ssh --delete /home/local ip_addr:/home/public    #通过ssh和压缩将本地目录同步到远程目录
dd bs=1M if=/dev/hda | gzip | ssh user@ip_addr 'dd of=hda.gz'  
#通过ssh在远程主机上执行一次备份本地磁盘的操作
dd if=/dev/sda of=/tmp/file1 
#备份磁盘内容到一个文件
tar -Puf backup.tar /home/user 执行一次对 '/home/user' 
#目录的交互式备份操作
( cd /tmp/local/ && tar c . ) | ssh -C user@ip_addr 'cd /home/share/ && tar x -p' 
#通过ssh在远程目录中复制一个目录内容
( tar c /home ) | ssh -C user@ip_addr 'cd /home/backup-home && tar x -p' 
#通过ssh在远程目录中复制一个本地目录
tar cf - . | (cd /tmp/backup ; tar xf - ) 
#本地将一个目录复制到另一个地方，保留原有权限及链接
find /home/user1 -name '*.txt' | xargs cp -av --target-directory=/home/backup/ --parents 
#从一个目录查找并复制所有以 '.txt' 结尾的文件到另一个目录
find /var/log -name '*.log' | tar cv --files-from=- | bzip2 > log.tar.bz2 
#查找所有以 '.log' 结尾的文件并做成一个bzip包
dd if=/dev/hda of=/dev/fd0 bs=512 count=1 
#做一个将 MBR (Master Boot Record)内容复制到软盘的动作
dd if=/dev/fd0 of=/dev/hda bs=512 count=1 
#从已经保存到软盘的备份中恢复MBR内容
```
<a name="WYglK"></a>
### 光盘
```bash
cdrecord -v gracetime=2 dev=/dev/cdrom -eject blank=fast -force 
#清空一个可复写的光盘内容
mkisofs /dev/cdrom > cd.iso             #在磁盘上创建一个光盘的iso镜像文件
mkisofs /dev/cdrom | gzip > cd_iso.gz    #在磁盘上创建一个压缩了的光盘iso镜像文件
mkisofs -J -allow-leading-dots -R -V "Label CD" -iso-level 4 -o ./cd.iso data_cd 
#创建一个目录的iso镜像文件
cdrecord -v dev=/dev/cdrom cd.iso               #刻录一个ISO镜像文件
gzip -dc cd_iso.gz | cdrecord dev=/dev/cdrom -  #刻录一个压缩了的ISO镜像文件
mount -o loop cd.iso /mnt/iso                  #挂载一个ISO镜像文件
cd-paranoia -B             #从一个CD光盘转录音轨到 wav 文件中
cd-paranoia -- "-3"        #从一个CD光盘转录音轨到 wav 文件中（参数-3）
cdrecord --scanbus         #扫描总线以识别scsi通道
dd if=/dev/hdc | md5sum    #校验一个设备的md5sum编码，例如一张 CD
```
<a name="609fd1d1"></a>
### 网络 - （以太网和WIFI无线）
```bash
ifconfig eth0    #显示一个以太网卡的配置
ifup eth0        #启用一个 'eth0' 网络设备
ifdown eth0      #禁用一个 'eth0' 网络设备
ifconfig eth0 192.168.1.1 netmask 255.255.255.0     #控制IP地址
ifconfig eth0 promisc     #设置 'eth0' 成混杂模式以嗅探数据包 (sniffing)
dhclient eth0            #以dhcp模式启用 'eth0'
route -n    #查看路由表
route add -net 0/0 gw IP_Gateway    #配置默认网关
route add -net 192.168.0.0 netmask 255.255.0.0 gw 192.168.1.1 
#配置静态路由到达网络'192.168.0.0/16'
route del 0/0 gw IP_gateway        #删除静态路由
hostname #查看机器名
host www.example.com       #把一个主机名解析到一个网际地址或把一个网际地址解析到一个主机名。
nslookup www.example.com   #用于查询DNS的记录，查看域名解析是否正常，在网络故障的时候用来诊断网络问题。
ip link show            #查看网卡信息
mii-tool                #用于查看、管理介质的网络接口的状态
ethtool                 #用于查询和设置网卡配置
netstat -tupl           #用于显示TCP/UDP的状态信息
tcpdump tcp port 80     #显示所有http协议的流量
```
<a name="m9rw0"></a>
### 进程相关的命令
<a name="uuz7F"></a>
#### ps命令
ps命令，用于将某个时间点的进程运行情况选取下来并输出，process之意
```bash
-A ：所有的进程均显示出来
-a ：不与terminal有关的所有进程
-u ：有效用户的相关进程
-x ：一般与a参数一起使用，可列出较完整的信息
-l ：较长，较详细地将PID的信息列出
ps aux # 查看系统所有的进程数据
ps ax # 查看不与terminal有关的所有进程
ps -lA # 查看系统所有的进程数据
ps axjf # 查看连同一部分进程树状态
```
<a name="971oa"></a>
#### kill命令
kill命令,用于向某个工作（%jobnumber）或者是某个PID（数字）传送一个信号，它通常与ps和jobs命令一起使用<br />命令格式 : kill[命令参数][进程id]<br />命令参数:
```bash
-l  信号，若果不加信号的编号参数，则使用“-l”参数会列出全部的信号名称
-a  当处理当前进程时，不限制命令名和进程号的对应关系
-p  指定kill 命令只打印相关进程的进程号，而不发送任何信号
-s  指定发送信号
-u  指定用户
```
实例1：列出所有信号名称 命令：kill -l 输出：
```bash
[root@localhost test6]# kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
13) SIGPIPE     14) SIGALRM     15) SIGTERM     16) SIGSTKFLT
17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU
25) SIGXFSZ     26) SIGVTALRM   27) SIGPROF     28) SIGWINCH
29) SIGIO       30) SIGPWR      31) SIGSYS      34) SIGRTMIN
35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4
39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12
47) SIGRTMIN+13 48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14
51) SIGRTMAX-13 52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10
55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6
59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX
```
说明：<br />只有第9种信号(SIGKILL)才可以无条件终止进程，其他信号进程都有权利忽略。   下面是常用的信号：
```
HUP    1    终端断线
INT     2    中断（同 Ctrl + C）
QUIT    3    退出（同 Ctrl + \）
TERM   15    终止
KILL    9    强制终止
CONT   18    继续（与STOP相反， fg/bg命令）
STOP    19    暂停（同 Ctrl + Z）
```
实例2：得到指定信号的数值
```bash
[root@localhost test6]# kill -l KILL
[root@localhost test6]# kill -l SIGKILL
[root@localhost test6]# kill -l TERM
[root@localhost test6]# kill -l SIGTERM
[root@localhost test6]#
```
实例3：先用ps查找进程，然后用kill杀掉
```bash
# 命令：kill 3268
[root@localhost test6]# ps -ef|grep vim 
root      3268  2884  0 16:21 pts/1    00:00:00 vim install.log
root      3370  2822  0 16:21 pts/0    00:00:00 grep vim
[root@localhost test6]# kill 3268
```
实例4：彻底杀死进程
```
命令：kill –9 3268   // -9 强制杀掉进程
```
<a name="NhuIl"></a>
#### killall命令
killall命令，向一个命令启动的进程发送一个信号，用于杀死指定名字的进程<br />命令格式 : killall[命令参数][进程名]
```bash
命令参数：
-Z 只杀死拥有scontext 的进程
-e 要求匹配进程名称
-I 忽略小写
-g 杀死进程组而不是进程
-i 交互模式，杀死进程前先询问用户
-l 列出所有的已知信号名称
-q 不输出警告信息
-s 发送指定的信号
-v 报告信号是否成功发送
-w 等待进程死亡
--help 显示帮助信息
--version 显示版本显示
```
示例
```bash
1：杀死所有同名进程
    killall nginx
    killall -9 bash
2.向进程发送指定信号
    killall -TERM ngixn  或者  killall -KILL nginx
```
<a name="QpFuK"></a>
#### top命令
top命令，是Linux下常用的性能分析工具，能够实时显示系统中各个进程的资源占用状况，类似于Windows的任务管理器。<br />杀死进程：
```bash
# 图形化界面的方式
kill -9 pid  （-9表示强制关闭）
killall -9 程序的名字
pkill 程序的名字
```
查看进程端口号：
```bash
netstat -tunlp | grep 端口号
```
<a name="hfltS"></a>
#### JPS工具
jps(Java Virtual Machine Process Status Tool)是JDK 1.5提供的一个显示当前所有java进程pid的命令，简单实用，非常适合在linux/unix平台上简单察看当前java进程的一些简单情况。<br />很多人用过unix系统里的ps命令，这个命令主要是用来显示当前系统的进程情况，有哪些进程，及其 id。jps 也是一样，它的作用是显示当前系统的java进程情况，及其id号。可以通过它来查看启动了几个java进程（因为每一个java程序都会独占一个java虚拟机实例），和他们的进程号（为下面几个程序做准备），并可通过opt来查看这些进程的详细启动参数。<br />使用方法：在当前命令行下打 jps(需要JAVA_HOME，没有的话，到改程序的目录下打) 。
```bash
jps存放在JAVA_HOME/bin/jps，使用时为了方便请将JAVA_HOME/bin/加入到Path.
$> jps
23991 Jps
23789 BossMain
23651 Resin
比较常用的参数：
#-q 只显示pid，不显示class名称,jar文件名和传递给main 方法的参数
$> jps -q
28680
23789
23651
#-m 输出传递给main 方法的参数，在嵌入式jvm上可能是null
$> jps -m
28715 Jps -m
23789 BossMain
23651 Resin -socketwait 32768 -stdout /data/aoxj/resin/log/stdout.log -stderr /data/aoxj/resin/log/stderr.log
#-l 输出应用程序main class的完整package名 或者 应用程序的jar文件完整路径名
$> jps -l
28729 sun.tools.jps.Jps
23789 com.asiainfo.aimc.bossbi.BossMain
23651 com.caucho.server.resin.Resin
#-v 输出传递给JVM的参数
$> jps -v
23789 BossMain
28802 Jps -Denv.class.path=/data/aoxj/bossbi/twsecurity/java/trustwork140.jar:/data/aoxj/bossbi/twsecurity/java/:/data/aoxj/bossbi/twsecurity/java/twcmcc.jar:/data/aoxj/jdk15/lib/rt.jar:/data/aoxj/jd
k15/lib/tools.jar -Dapplication.home=/data/aoxj/jdk15 -Xms8m
23651 Resin -Xss1m -Dresin.home=/data/aoxj/resin -Dserver.root=/data/aoxj/resin -Djava.util.logging.manager=com.caucho.log.LogManagerImpl -
Djavax.management.builder.initial=com.caucho.jmx.MBeanServerBuilderImpl
jps 192.168.0.77
#列出远程服务器192.168.0.77机器所有的jvm实例，采用rmi协议，默认连接端口为1099（前提是远程服务器提供jstatd服务）
#注：jps命令有个地方很不好，似乎只能显示当前用户的java进程，要显示其他用户的还是只能用unix/linux的ps命令。
```
<a name="JF8Gu"></a>
### VIM
VIM是一款功能强大的命令行文本编辑器，在Linux中通过vim命令可以启动vim编辑器。<br />一般使用vim + 目标文件路径 的形式使用vim<br />如果目标文件存在，则vim打开目标文件，如果目标文件不存在，则vim新建并打开该文件<br />:q：退出vim编辑器
<a name="xm2Gj"></a>
#### VIM模式
vim拥有三种模式：
<a name="yRMvW"></a>
#### （1）命令模式（常规模式）
vim启动后，默认进入命令模式，任何模式都可以通过esc键回到命令模式（可以多按几次），命令模式下可以键入不同的命令完成选择、复制、粘贴、撤销等操作。<br />命名模式常用命令如下：<br />i : 在光标前插入文本；<br />o:在当前行的下面插入新行；<br />dd:删除整行；<br />yy：将当前行的内容放入缓冲区（复制当前行）<br />n+yy :将n行的内容放入缓冲区（复制n行）<br />p:将缓冲区中的文本放入光标后（粘贴）<br />u：撤销上一个操作<br />r:替换当前字符<br />/ 查找关键字
<a name="KT7Bj"></a>
#### （2）插入模式
在命令模式下按 " i "键，即可进入插入模式，在插入模式可以输入编辑文本内容，使用esc键可以返回命令模式。
<a name="fdsa3"></a>
#### （3）ex模式
在命令模式中按" : "键可以进入ex模式，光标会移动到底部，在这里可以保存修改或退出vim.<br />ext模式常用命令如下：<br />:w ：保存当前的修改<br />:q ：退出<br />:q! ：强制退出，保存修改<br />:x  :保存并退出，相当于:wq<br />:set number 显示行号<br />:! 系统命令  执行一个系统命令并显示结果<br />:sh ：切换到命令行，使用ctrl+d切换回vim
