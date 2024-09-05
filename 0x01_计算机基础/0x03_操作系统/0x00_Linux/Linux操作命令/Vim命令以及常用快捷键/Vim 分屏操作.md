Linux Vim<br />Vim分屏功能是通过分割窗口来实现的，这是提高工作效率的一大利器。无论是想同时显示两个文件，或者同时显示一个文件的两个不同的位置，又或者并排比较两个文件，等等，这些都能通过分屏来实现，这样子很方便代码的比对和复制粘贴
<a name="Zl5mc"></a>
### 水平方向分屏打开新文件
```bash
:sp linuxmi.py
```
或者
```bash
:split linuxmi.py
```
这个命令把窗口横向切分为两个窗口，并把光标置于上面的窗口中。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639880714839-61812663-94af-40a3-8296-3fd80c0f4a60.webp#clientId=u78dd12fd-2247-4&from=paste&id=u9c33ec66&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u073fd73b-fe98-44b9-a6ed-5d8fc55bc10&title=)
<a name="ksP04"></a>
### 垂直方向分屏打开新文件
```bash
:vsp linux.py
:vsplit linux.py
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639880715037-d5b5c777-2654-493c-8723-2648ed363672.webp#clientId=u78dd12fd-2247-4&from=paste&id=u2019fe3f&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaf8dc179-cd2a-40d1-9b41-b74474557ec&title=)
```bash
:sview linux.py ->只读分屏打开文件
```
另外，要打开窗口编辑一个新的文件时，可以用以下命令：
```bash
:new
```
<a name="sS6Wx"></a>
### 从命令行直接打开多个文件且是分屏
```bash
vim -On file1, file2 ... ->垂直分屏
vim -on file1, file2 ... ->水平分屏
linuxmi@linuxmi:~/www.linuxmi.com$ vim -O3 linux.py linuxmi.py linuxmi.cpp
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639880714904-3544fcc4-40ca-483d-93d2-a9c4fe1aa87f.webp#clientId=u78dd12fd-2247-4&from=paste&id=u2542a814&originHeight=169&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uebb46ce5-cd8e-45e5-aecf-d305763a1fb&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639880715126-6856d0a9-c90f-4715-bf6a-821f349a72c1.webp#clientId=u78dd12fd-2247-4&from=paste&id=u280a8d7c&originHeight=702&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u025018e0-385a-4d42-9d27-bfd19ef440a&title=)
:::tips
注：-O垂直分屏，-o水平分屏，n表示分几个屏
:::
<a name="u9cuP"></a>
### 实时调整当前窗口的宽度

- ctrl-w > //向右加宽，默认值为1
- ctrl-w N > //向右加宽宽度N
- ctrl-w < // 同理
<a name="goGxg"></a>
### 横屏/竖屏分屏打开当前文件

- ctrl+w s
- ctrl+w v
<a name="jLg8q"></a>
### 切换分屏

- ctrl+w h,j,k,l
- ctrl+w 上下左右键
- crtl+w进行分屏窗口的切换 按完以后再按一个w
- crtl+w进行分屏窗口的切换 按完以后再按一个r 互换窗口
- crtl+w进行分屏窗口的切换 按完以后再按一个c 关闭窗口
<a name="e7JtS"></a>
### 关闭分屏
关闭窗口有以下几个个命令：

- ctrl+W c 关闭当前窗口
- ctrl+w q 关闭当前窗口，若只有一个分屏且退出vim
- :only 仅保留当前分屏
- :hide 关闭当前分屏
<a name="PU8ik"></a>
### 调整分屏的大小（宽度与高度）

- ctrl+w = 所有分屏都统一高度
- ctrl+w + 增加高度，默认值为1
- ctrl+w - 减少高度
- 10 ctrl+w + 增加10行高度
- ctrl-w N + //当前屏高度加N

使用指定当前屏的调整高度
```python
: res[ize] N
```
示例：
```python
:resize 30
```
**移动分屏**：ctrl+W H,J,K,L<br />**将屏幕移动到最顶端**：ctrl-w + K<br />**将屏幕移动到最低端**：ctrl-w + J<br />**将屏幕移动到最左边**：ctrl-w + H<br />**将屏幕移动到最右边**：ctrl-w + L
