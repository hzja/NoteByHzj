Linux <br />在Linux中使用颜色代码来区分文件类型，通常情况下目录、链接、文件的颜色将不同。在终端中使用ls命令时，会发现一些带有颜色的文件。<br />`ls`命令使用环境变量`LS_COLORS`来确定文件名的显示颜色。可以通过调用`LS_COLORS`变量来查看文件类型及其颜色代码的列表。
```bash
[root@localhost ~]# echo $LS_COLORS
rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=01;05;37;41:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=01;36:*.au=01;36:*.flac=01;36:*.m4a=01;36:*.mid=01;36:*.midi=01;36:*.mka=01;36:*.mp3=01;36:*.mpc=01;36:*.ogg=01;36:*.ra=01;36:*.wav=01;36:*.oga=01;36:*.opus=01;36:*.spx=01;36:*.xspf=01;36:
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615772463401-0e3bf47a-b50b-48eb-82c3-66eafbcbc5af.png#align=left&display=inline&height=323&originHeight=970&originWidth=3323&size=1289753&status=done&style=none&width=1107.6666666666667)<br />默认的颜色代码在`/etc/DIR_COLORS`配置文件中。
```bash
cat /etc/DIR_COLORS
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615772488075-27d89038-c875-47da-97d1-1b7081b047f5.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2068529&status=done&style=none&width=1107.6666666666667)<br />它为文件使用三种类型的颜色代码：

- 属性代码：代码范围00-08
- 文字颜色代码：代码范围30-37,90-97
- 背景颜色代码：代码范围40-47,100-107
<a name="s0qGd"></a>
## 文件类型代码列表
下面是常用文件类型代码的列表：

| Code | File Types |
| :--- | :--- |
| di | 目录 |
| fi | 文件 |
| ex | 可执行文件 |
| ln | 符号链接文件 |
| so | 套接字 |
| bd | 块设备 |
| cd | 字符设备 |
| mi | 丢失文件 |
| *.extension | 例如：*.mp3,*.jpeg<br /> |

<a name="Am10d"></a>
## 属性代码列表
下面表格是属性代码：
```
+--------------+--------------------+
|    Code      |      Attributes    |
+--------------+--------------------+
|      00      | None               |
|      01      | Bold               |
|      04      | Underscore         |
|      05      | Blink              |
|      07      | Reverse            |
|      08      | Concealed          |
+--------------+--------------------+
```
<a name="us8hN"></a>
## 文本颜色代码
下面表格是字体颜色的代码：
```
+--------------+--------------------+     +--------------+--------------------+ 
|    Code      |      Text Color    |     |    Code      |      Text Color    |
+--------------+--------------------+     +--------------+--------------------+
|      30      | Black              |     |      90      | dark grey          |
|      31      | Red                |     |      91      | light red          |
|      32      | Green              |     |      92      | light green        |
|      33      | Yellow             |     |      93      | yellow             |
|      34      | Blue               |     |      94      | light blue         |
|      35      | Magenta            |     |      95      | light purple       |
|      36      | Cyan               |     |      96      | turquoise          |
|      37      | White              |     |      97      | white              |
+--------------+--------------------+     +--------------+--------------------+
```
<a name="0rYve"></a>
## 背景颜色代码
下面表格是背景颜色的代码：
```
+--------------+--------------------+     +--------------+--------------------+ 
|    Code      | Background Color   |     |    Code      | Background Color   |
+--------------+--------------------+     +--------------+--------------------+ 
|      40      | Black              |     |     100      | dark grey          |
|      41      | Red                |     |     101      | light red          |
|      42      | Green              |     |     102      | light green        |
|      43      | Yellow             |     |     103      | yellow             |
|      44      | Blue               |     |     104      | light blue         |
|      45      | Magenta            |     |     105      | light purple       |
|      46      | Cyan               |     |     106      | turquoise          |
|      47      | White              |     |     107      | white              |
+--------------+--------------------+     +--------------+--------------------+
```
<a name="AFwqY"></a>
## 在Linux中为文件设置自定义颜色
默认的文件夹颜色为“蓝色”，在这里将文件夹配色方案给为黄色93和04下换线，这种组合代码是`LS_COLORS="di=4;93"`<br />如果使其生效，可在`~/.bashrc`中添加上面代码。
```bash
[root@localhost ~]# echo "LS_COLORS=\"di=4;93\"" >> ~/.bashrc 
[root@localhost ~]# source ~/.bashrc
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1615772567638-1b7993cc-fc64-437b-83fe-10f00f61a84e.png#align=left&display=inline&height=504&originHeight=1511&originWidth=3323&size=1820049&status=done&style=none&width=1107.6666666666667)<br />可以看到文件夹都添加下划线了。
