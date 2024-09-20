Linux htop glances 
<a name="IZJmf"></a>
### htop
[htop官网](https://hisham.hm/htop/)<br />[https://hisham.hm/htop/](https://hisham.hm/htop/)<br />htop 是一个类似于 top 的交互式流程查看器，但它提供了更好的开箱即用的用户体验。默认情况下，htop 会用令人愉快的彩色显示与 top 相同的指标。<br />默认情况下，htop 是这样的：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596442277232-447c5a16-d6ff-4951-b09f-6d5644406b07.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2183024&status=done&style=none&width=1107.6666666666667)<br />相比之下，top 默认是这样的：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596442299443-d064054a-d314-4f49-b2e0-c61e07a23ef6.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2098267&status=done&style=none&width=1107.6666666666667)<br />此外，htop 在顶部提供了系统概览信息，在底部提供了一个命令栏，可以使用功能键来触发命令，而且，还可以按 F2 进入设置界面对它进行定制。可以更改其颜色、添加或删除指标，或更改概览栏的显示选项。<br />虽然对于最新版本的 top，也可以通过配置实现类似的结果，但 htop 提供了更合理的默认配置，这使得它成为一个漂亮且易于使用的流程查看器。
<a name="NJXwa"></a>
### glances
<a name="hBrud"></a>
####  glances安装
```bash
[root@iZuligp6e1dyzfZ ~]# wget -O- https://bit.ly/glances | /bin/bash
```
glances 是`htop`的补充工具。除了列出所有进程及其 CPU 和内存使用情况之外，它还可以显示有关系统的其他信息，比如：

- 网络及磁盘使用情况<br />
- 文件系统已使用的空间和总空间<br />
- 来自不同传感器（例如电池）的数据<br />
- 以及最近消耗过多资源的进程列表
<a name="X9fOk"></a>
#### glances的使用
```bash
[root@iZuligp6e1dyzfZ /]# glances
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596442835310-81ac20b8-27b9-42ff-bfb1-9388c71d6a33.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2238223&status=done&style=none&width=1107.6666666666667)
