Linux<br />smem 是Linux系统上的一款可以生成多种内存耗用报告的命令行工具。与现有工具不一样的是 smem 可以报告 PSS【Proportional Set Size（按比例占用大小）】，这是一种更有意义的指标。可以衡量虚拟内存系统的库和应用程序所占用的内存数量。<br />由于大部分的物理内存通常在多个应用程序之间共享，名为驻留集大小（RSS）的这个标准的内存耗用衡量指标会大大高估内存耗用情况。PSS这个参数而是衡量了每个应用程序在每个共享内存区中的「公平分配」，给出了一个切合实际的衡量指标。
<a name="kOGKu"></a>
## 1、安装 smem 工具
如果使用的是 Fedora 19 以上系统， smem  默认在存储库中，因此可以使用 yum 来安装它：
```bash
$ sudo yum install smem
```
对于 Ubuntu 用户，可以使用 apt-get 命令来安装  smem ：
```bash
$ sudo apt-get install smem
```
如果无法正常安装的话，可以下载它的源码直接安装，地址为：[https://www.selenic.com/smem/download/](https://www.selenic.com/smem/download/)
<a name="elJM4"></a>
## 2、smem 工具常见用法
默认情况下， smem  将显示每个正在运行的进程及所使用的内存。在这里，可以留意 RSS 相对于 USS 和 PSS 的大小，可以看到它明显高于另外二者。
```bash
$ smem
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675051-f1973753-2b44-4d68-bafa-7703a9d736ee.webp#clientId=ue008d9b0-16f6-4&from=paste&id=u84e5effe&originHeight=717&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub22ef428-5f4e-46cd-99ab-ca2a7b12cea&title=)<br />此外， smem  还可显示每个库所使用的内存。这个结果比较长，可能需要耗费一些时间，取决于系统。
```bash
$ smem -m
```
这个命令产生的结果太多了，如果想要查看特定应用程序使用内存情况，例如 Firefox，那么可以配合 `grep` 命令使用，同时使用 `-p` 选项以百分比形式查看内存使用情况。
```bash
$ smem -m -p | grep firefox
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675038-5e1fc27e-29b7-4870-baab-6f89c50cd66e.webp#clientId=ue008d9b0-16f6-4&from=paste&id=u2d905e6d&originHeight=675&originWidth=929&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u47db596b-ac9d-4992-a4b5-fbf18fc821d&title=)<br />smem  命令还可以显示每个**用户**的内存使用情况，需要使用 `-u` 选项：
```bash
$ smem -u -p
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675058-4d248012-ed8f-4f02-840b-f5a30fdd898f.webp#clientId=ue008d9b0-16f6-4&from=paste&id=u8e38bc86&originHeight=90&originWidth=718&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub2444dfa-9cc8-4f7a-8a9f-cb2702e1056&title=)<br />还可以使用 `-w` 选项查看系统内存使用情况：
```bash
$ smem -w -p
```
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675063-fc2d2eda-9ba9-4433-b4ae-fe2fec47d310.webp#clientId=ue008d9b0-16f6-4&from=paste&id=uf54a3030&originHeight=228&originWidth=815&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub7656d53-ae95-458a-b11a-9ef067c13d3&title=)
<a name="Vlgf3"></a>
## 3、结果可视化输出
显示数字毕竟不直观，还可以使用 smem 生成图形图表来显示内存使用情况，一目了然。要达到这个目的，需要使用到Python 。但光有 Python 还不行，还需要安装用于生成图表的 matplotlib 库。<br />Fedora 用户可以通过运行以下命令来安装它：
```bash
$ sudo yum install python-matplotlib
```
Ubuntu 用户可以通过运行下面命令获得它：
```bash
$ sudo apt-get install python-matplotlib
```
库安装之后，现在就可以以条形图或饼图的形式将获得的内存使用情况以可视化表示。
<a name="c5pKk"></a>
### 3.1 饼形图
使用 smem 以饼图的形式查看内存使用情况，需要加上 `--pie` 选项，如下所示：
```bash
$ smem --pie name -s pss
```
命令运行之后将生成一个饼图。请注意，这条命令里还加了 `-s pss` ，意思是显示 PSS 的内存使用情况。要获取 USS 和 RSS 的饼图，只需将命令中的 pss 替换为 uss 或 rss 即可。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675103-394354d9-2079-40c8-ace1-37a54bb12b68.webp#clientId=ue008d9b0-16f6-4&from=paste&id=ua7b15690&originHeight=625&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6b32ac9c-7548-4db4-a673-41a1abce0bb&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675421-fac11c66-6621-48ce-b8f0-096cad0cc15d.webp#clientId=ue008d9b0-16f6-4&from=paste&id=ub150ccce&originHeight=572&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1cff057c-caf5-4711-b756-88a7b9c461c&title=)<br />从两个饼图中，可以看到 RSS 报告还有 41.74% 的未使用内存，而 PSS 报告有 53.02% 的未使用内存。可以看出来，RSS 显示的可用内存偏小，但实际还有很充足的内存空间。
<a name="KN2ub"></a>
### 3.2 柱状图
smem  的另一个很酷的特性是以柱状图的形式生成输出。有了这个功能，可以一次性查看 USS、PSS 和 RSS 报告的内存使用情况。<br />要实现这个功能，需要加上 `--bar` 选项：
```bash
$ smem --bar pid -c "pss uss rss"
```
上面的命令将给出带有进程 ID 号的内存使用情况的输出结果。如果想要查看进程的名称，可以将 pid 替换为 name 即可。<br />命令运行之后，可以生成如下图所示的条柱状图。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644539675478-5230328c-b2e3-41b9-adc5-f2fdec3bef4b.webp#clientId=ue008d9b0-16f6-4&from=paste&id=ue7d05368&originHeight=579&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u36d9497e-cd8b-4eca-937c-e23e1d87500&title=)
