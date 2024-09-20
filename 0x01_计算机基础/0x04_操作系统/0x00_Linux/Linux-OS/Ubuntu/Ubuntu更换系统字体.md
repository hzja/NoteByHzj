Ubuntu 字体
<a name="fh0vV"></a>
### 1、下载字体
```bash
$ wget https://github.com/source-foundry/Hack/releases/download/v3.003/Hack-v3.003-ttf.tar.xz
```
<a name="PcTKj"></a>
### 2、解压字体
```bash
$ tar -xvf Hack-v3.003-ttf.tar.xz
```
<a name="jiGz8"></a>
### 3、安装字体
字体通常安装在`/usr/share/fonts/`字体目录中，或者安装在以下目录中：`~/.local/share/fonts/`或`/usr/local/share/fonts`。在本例中，将使用`~/.local/share/fonts/path/`。如果目录不存在，请使用以下命令创建它：
```bash
$ mkdir -p ~/.local/share/fonts
```
将下载的字体移动到安装目录下
```bash
$ mv Hack-*.ttf ~/.local/share/fonts
```
重新生成字体缓存
```bash
$ fc-cache -f -v
```
<a name="vU3Qd"></a>
### 4、检查字体是否安装
```bash
$ fc-list |grep "Hack"
```
<a name="GWX4n"></a>
### 5、卸载安装的字体
```bash
$ rm -rf ~/.local/share/fonts/Hack-*.ttf
$ fc-cache
```
