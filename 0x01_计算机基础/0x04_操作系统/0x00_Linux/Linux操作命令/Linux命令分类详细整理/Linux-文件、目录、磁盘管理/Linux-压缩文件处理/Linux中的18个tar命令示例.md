Linux
<a name="cTxfH"></a>
### 1、在 Linux 中创建 tar 存档文件
下面的示例命令将创建一个tar存档文件rumenz-14-09-12.tar对于目录/home/rumenz在当前工作目录中。请参阅示例命令的操作。
```bash
# tar -cvf rumenz-14-09-12.tar /home/rumenz/

/home/rumenz/
/home/rumenz/cleanfiles.sh
/home/rumenz/openvpn-2.1.4.tar.gz
/home/rumenz/rumenz-14-09-12.tar
/home/rumenz/phpmyadmin-2.11.11.3-1.el5.rf.noarch.rpm
/home/rumenz/rpmforge-release-0.5.2-2.el5.rf.i386.rpm
```
创建 tar 存档文件的选项。

1. c– 创建一个新的 .tar 存档文件。
2. v– 详细显示 .tar 文件进度。
3. f– 存档文件的文件名类型。
<a name="VeteB"></a>
### 2、在 Linux 中创建 tar.gz 存档文件
创建一个压缩gzip使用该选项作为存档文件z.例如，下面的命令将创建一个压缩MyImages-14-09-12.tar.gz目录的文件/home/MyImages.(Note: tar.gz and tgz both are similar）。
```bash
# tar cvzf MyImages-14-09-12.tar.gz /home/MyImages
OR
# tar cvzf MyImages-14-09-12.tgz /home/MyImages

/home/MyImages/
/home/MyImages/Sara-Khan-and-model-Priyanka-Shah.jpg
/home/MyImages/RobertKristenviolent101201.jpg
/home/MyImages/Justintimerlake101125.jpg
/home/MyImages/Mileyphoto101203.jpg
/home/MyImages/JenniferRobert101130.jpg
/home/MyImages/katrinabarbiedoll231110.jpg
/home/MyImages/the-japanese-wife-press-conference.jpg
/home/MyImages/ReesewitherspoonCIA101202.jpg
/home/MyImages/yanaguptabaresf231110.jpg
```
<a name="kkBvF"></a>
### 3、在 Linux 中创建 tar.bz2 存档文件
bz2功能压缩并创建一个小于gzip，这bz2压缩比压缩和解压缩文件需要更多的时间gzip<br />要创建压缩的tar文件，使用选项j。以下示例命令将创建一个Phpfiles-org.tar.bz2一个目录的文件/home/php
```bash
# tar cvfj Phpfiles-org.tar.bz2 /home/php
OR
# tar cvfj Phpfiles-org.tar.tbz /home/php
OR 
# tar cvfj Phpfiles-org.tar.tb2 /home/php

/home/php/
/home/php/iframe_ew.php
/home/php/videos_all.php
/home/php/rss.php
/home/php/index.php
/home/php/vendor.php
/home/php/video_title.php
/home/php/report.php
/home/php/object.html
/home/php/video.php
```
<a name="JPApw"></a>
### 4、在 Linux 中解压 tar 存档文件
要解压缩或提取 tar 文件，只需使用选项发出以下命令x(extract）。例如，下面的命令将解压文件public_html-14-09-12.tar在当前工作目录中。<br />如果要在不同的目录中解压，请使用选项作为-C(specified directory）。
```bash
## Untar files in Current Directory ##
# tar -xvf public_html-14-09-12.tar

## Untar files in specified Directory ##
# tar -xvf public_html-14-09-12.tar -C /home/public_html/videos/

/home/public_html/videos/
/home/public_html/videos/views.php
/home/public_html/videos/index.php
/home/public_html/videos/logout.php
/home/public_html/videos/all_categories.php
/home/public_html/videos/feeds.xml
```
<a name="hpK5B"></a>
### 5、在 Linux 中解压 tar.gz 存档文件
解压tar.gz存档文件，只需运行以下命令。如果想在不同的目录中解压，只需使用选项-C和目录路径，如上例所示。
```bash
# tar -xvf thumbnails-14-09-12.tar.gz

/home/public_html/videos/thumbnails/
/home/public_html/videos/thumbnails/katdeepika231110.jpg
/home/public_html/videos/thumbnails/katrinabarbiedoll231110.jpg
/home/public_html/videos/thumbnails/onceuponatime101125.jpg
/home/public_html/videos/thumbnails/playbutton.png
/home/public_html/videos/thumbnails/ReesewitherspoonCIA101202.jpg
/home/public_html/videos/thumbnails/snagItNarration.jpg
/home/public_html/videos/thumbnails/Minissha-Lamba.jpg
/home/public_html/videos/thumbnails/Lindsaydance101201.jpg
/home/public_html/videos/thumbnails/Mileyphoto101203.jpg
```
<a name="TEyF7"></a>
### 6、在 Linux 中解压 tar.bz2 存档文件
解压缩高度压缩的tar.bz2文件，只需使用以下命令。下面的示例命令将解压所有.flv存档文件中的文件。
```bash
# tar -xvf videos-14-09-12.tar.bz2

/home/public_html/videos/flv/katrinabarbiedoll231110.flv
/home/public_html/videos/flv/BrookmuellerCIA101125.flv
/home/public_html/videos/flv/dollybackinbb4101125.flv
/home/public_html/videos/flv/JenniferRobert101130.flv
/home/public_html/videos/flv/JustinAwardmovie101125.flv
/home/public_html/videos/flv/Lakme-Fashion-Week.flv
/home/public_html/videos/flv/Mileyphoto101203.flv
/home/public_html/videos/flv/Minissha-Lamba.flv
```
<a name="q15hl"></a>
### 7、列出 Linux 中 tar 存档文件的内容
要列出tar存档文件的内容，只需运行以下带有选项的命令t(list content）。下面的命令将列出内容uploadprogress.tar文件。
```bash
# tar -tvf uploadprogress.tar

-rw-r--r-- chregu/staff   2276 2011-08-15 18:51:10 package2.xml
-rw-r--r-- chregu/staff   7877 2011-08-15 18:51:10 uploadprogress/examples/index.php
-rw-r--r-- chregu/staff   1685 2011-08-15 18:51:10 uploadprogress/examples/server.php
-rw-r--r-- chregu/staff   1697 2011-08-15 18:51:10 uploadprogress/examples/info.php
-rw-r--r-- chregu/staff    367 2011-08-15 18:51:10 uploadprogress/config.m4
-rw-r--r-- chregu/staff    303 2011-08-15 18:51:10 uploadprogress/config.w32
-rw-r--r-- chregu/staff   3563 2011-08-15 18:51:10 uploadprogress/php_uploadprogress.h
-rw-r--r-- chregu/staff  15433 2011-08-15 18:51:10 uploadprogress/uploadprogress.c
-rw-r--r-- chregu/staff   1433 2011-08-15 18:51:10 package.xml
```
<a name="oyuul"></a>
### 8、在 Linux 中列出内容 tar.gz 存档文件
使用以下命令列出内容tar.gz文件。
```bash
# tar -tvf staging.rumenz.com.tar.gz

-rw-r--r-- root/root         0 2012-08-30 04:03:57 staging.rumenz.com-access_log
-rw-r--r-- root/root       587 2012-08-29 18:35:12 staging.rumenz.com-access_log.1
-rw-r--r-- root/root       156 2012-01-21 07:17:56 staging.rumenz.com-access_log.2
-rw-r--r-- root/root       156 2011-12-21 11:30:56 staging.rumenz.com-access_log.3
-rw-r--r-- root/root       156 2011-11-20 17:28:24 staging.rumenz.com-access_log.4
-rw-r--r-- root/root         0 2012-08-30 04:03:57 staging.rumenz.com-error_log
-rw-r--r-- root/root      3981 2012-08-29 18:35:12 staging.rumenz.com-error_log.1
-rw-r--r-- root/root       211 2012-01-21 07:17:56 staging.rumenz.com-error_log.2
-rw-r--r-- root/root       211 2011-12-21 11:30:56 staging.rumenz.com-error_log.3
-rw-r--r-- root/root       211 2011-11-20 17:28:24 staging.rumenz.com-error_log.4
```
<a name="Ul511"></a>
### 9、Linux 中列出内容 tar.bz2 存档文件
列出内容tar.bz2文件，发出以下命令。
```bash
# tar -tvf Phpfiles-org.tar.bz2

drwxr-xr-x root/root         0 2012-09-15 03:06:08 /home/php/
-rw-r--r-- root/root      1751 2012-09-15 03:06:08 /home/php/iframe_ew.php
-rw-r--r-- root/root     11220 2012-09-15 03:06:08 /home/php/videos_all.php
-rw-r--r-- root/root      2152 2012-09-15 03:06:08 /home/php/rss.php
-rw-r--r-- root/root      3021 2012-09-15 03:06:08 /home/php/index.php
-rw-r--r-- root/root      2554 2012-09-15 03:06:08 /home/php/vendor.php
-rw-r--r-- root/root       406 2012-09-15 03:06:08 /home/php/video_title.php
-rw-r--r-- root/root      4116 2012-09-15 03:06:08 /home/php/report.php
-rw-r--r-- root/root      1273 2012-09-15 03:06:08 /home/php/object.html
```
<a name="neLRl"></a>
### 10、在 Linux 中从 tar 文件解压单个文件
提取名为的单个文件cleanfiles.sh从cleanfiles.sh.tar使用以下命令。
```bash
# tar -xvf cleanfiles.sh.tar cleanfiles.sh
OR
# tar --extract --file=cleanfiles.sh.tar cleanfiles.sh

cleanfiles.sh
```
<a name="zrHH2"></a>
### 11、在 Linux 中从 tar.gz 文件解压单个文件
提取单个文件rumenzbackup.xml来自rumenzbackup.tar.gz归档文件，使用命令如下。
```bash
# tar -zxvf rumenzbackup.tar.gz rumenzbackup.xml
OR
# tar --extract --file=rumenzbackup.tar.gz rumenzbackup.xml

rumenzbackup.xml
```
<a name="ApvXy"></a>
### 12、Linux 下 tar.bz2 文件解压单个文件
提取名为的单个文件index.php从文件Phpfiles-org.tar.bz2使用以下选项。
```bash
# tar -jxvf Phpfiles-org.tar.bz2 home/php/index.php
OR
# tar --extract --file=Phpfiles-org.tar.bz2 /home/php/index.php

/home/php/index.php
```
<a name="QVKym"></a>
### 13、从 tar、tar.gz 和 tar.bz2 文件中解压多个文件
从文件夹中提取或解压多个文件tar,tar.gz,和tar.bz2存档文件。例如，下面的命令将提取 file 1 file 2 来自存档文件。
```bash
# tar -xvf rumenz-14-09-12.tar "file1" "file2" 

# tar -zxvf MyImages-14-09-12.tar.gz "file1" "file2" 

# tar -jxvf Phpfiles-org.tar.bz2 "file1" "file2"
```
<a name="KacrJ"></a>
### 14、在 Linux 中使用通配符提取文件组
为了提取一组文件，使用基于通配符的提取。例如，提取一组模式以.php从一个tar, tar.gz, and tar.bz2存档文件。
```bash
# tar -xvf Phpfiles-org.tar --wildcards '*.php'

# tar -zxvf Phpfiles-org.tar.gz --wildcards '*.php'

# tar -jxvf Phpfiles-org.tar.bz2 --wildcards '*.php'

/home/php/iframe_ew.php
/home/php/videos_all.php
/home/php/rss.php
/home/php/index.php
/home/php/vendor.php
/home/php/video_title.php
/home/php/report.php
/home/php/video.php
```
<a name="NH6F4"></a>
### 15、在 Linux 中将文件或目录添加到 tar 归档文件
要将文件或目录添加到现有的 tar 存档文件，使用选项r(append）。比如添加文件xyz.txt和目录php对现有的rumenz-14-09-12.tar存档文件。
```bash
# tar -rvf rumenz-14-09-12.tar xyz.txt

# tar -rvf rumenz-14-09-12.tar php

drwxr-xr-x root/root         0 2012-09-15 02:24:21 home/rumenz/
-rw-r--r-- root/root  15740615 2012-09-15 02:23:42 home/rumenz/cleanfiles.sh
-rw-r--r-- root/root    863726 2012-09-15 02:23:41 home/rumenz/openvpn-2.1.4.tar.gz
-rw-r--r-- root/root  21063680 2012-09-15 02:24:21 home/rumenz/rumenz-14-09-12.tar
-rw-r--r-- root/root   4437600 2012-09-15 02:23:41 home/rumenz/phpmyadmin-2.11.11.3-1.el5.rf.noarch.rpm
-rw-r--r-- root/root     12680 2012-09-15 02:23:41 home/rumenz/rpmforge-release-0.5.2-2.el5.rf.i386.rpm
-rw-r--r-- root/root 0 2012-08-18 19:11:04 xyz.txt
drwxr-xr-x root/root 0 2012-09-15 03:06:08 php/ 
-rw-r--r-- root/root 1751 2012-09-15 03:06:08 php/iframe_ew.php 
-rw-r--r-- root/root 11220 2012-09-15 03:06:08 php/videos_all.php 
-rw-r--r-- root/root 2152 2012-09-15 03:06:08 php/rss.php 
-rw-r--r-- root/root 3021 2012-09-15 03:06:08 php/index.php 
-rw-r--r-- root/root 2554 2012-09-15 03:06:08 php/vendor.php 
-rw-r--r-- root/root 406 2012-09-15 03:06:08 php/video_title.php
```
<a name="d6klu"></a>
### 16、将文件或目录添加到 tar.gz 和 tar.bz2 文件
tar 命令没有将文件或目录添加到现有压缩文件的选项tar.gz和tar.bz2存档文件。如果尝试会得到以下错误。
```bash
# tar -rvf MyImages-14-09-12.tar.gz xyz.txt

# tar -rvf Phpfiles-org.tar.bz2 xyz.txt

tar: This does not look like a tar archive
tar: Skipping to next header
xyz.txt
tar: Error exit delayed from previous errors
```
<a name="DRaAT"></a>
### 17、如何验证 tar、tar.gz 和 tar.bz2 存档文件
要验证任何 tar 或压缩存档文件，使用该选项W(verify）。为此，只需使用以下命令示例。(Note：不能对压缩的 (*.tar.gz, *.tar.bz2) 存档文件)。
```bash
# tar tvfW rumenz-14-09-12.tar

tar: This does not look like a tar archive
tar: Skipping to next header
tar: Archive contains obsolescent base-64 headers
tar: VERIFY FAILURE: 30740 invalid headers detected
Verify -rw-r--r-- root/root    863726 2012-09-15 02:23:41 /home/rumenz/openvpn-2.1.4.tar.gz
Verify -rw-r--r-- root/root  21063680 2012-09-15 02:24:21 /home/rumenz/rumenz-14-09-12.tar
tar: /home/rumenz/rumenz-14-09-12.tar: Warning: Cannot stat: No such file or directory
Verify -rw-r--r-- root/root   4437600 2012-09-15 02:23:41 home/rumenz/phpmyadmin-2.11.11.3-1.el5.rf.noarch.rpm
tar: /home/rumenz/phpmyadmin-2.11.11.3-1.el5.rf.noarch.rpm: Warning: Cannot stat: No such file or directory
Verify -rw-r--r-- root/root     12680 2012-09-15 02:23:41 home/rumenz/rpmforge-release-0.5.2-2.el5.rf.i386.rpm
tar: /home/rumenz/rpmforge-release-0.5.2-2.el5.rf.i386.rpm: Warning: Cannot stat: No such file or directory
Verify -rw-r--r-- root/root         0 2012-08-18 19:11:04 xyz.txt
Verify drwxr-xr-x root/root         0 2012-09-15 03:06:08 php/
```
<a name="k2caQ"></a>
### 18、检查 tar、tar.gz 和 tar.bz2 存档文件的大小
tar,tar.gz,和tar.bz2存档文件，使用以下命令。例如，下面的命令将以千字节（KB）。
```bash
# tar -czf - rumenz-14-09-12.tar | wc -c
12820480

# tar -czf - MyImages-14-09-12.tar.gz | wc -c
112640

# tar -czf - Phpfiles-org.tar.bz2 | wc -c
20480
```
