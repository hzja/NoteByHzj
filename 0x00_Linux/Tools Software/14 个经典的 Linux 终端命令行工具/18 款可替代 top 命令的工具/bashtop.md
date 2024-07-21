Linux<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943895457-6266306d-3ccd-4ddd-af07-e3485628e405.webp#clientId=uc0250edc-9c1a-4&from=paste&id=uad09b292&originHeight=694&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u334e5902-d0f1-4e3b-bc89-94e7ff243f7&title=)<br />在 Linux 系统中的两个查看进程的命令也用的不计其数，`ps`命令用于显示当前进程  process 的状态、`top`命令用于实时显示 process 的动态。<br />推荐一款 GitHub 热榜的 Linux 资源监视器：bashtop，其主要作用：用于展示当前 Linux 操作系统的显示处理器、内存、硬盘、网络和进程等各项资源的使用情况与状态。<br />该工具是由瑞典的一名开发者 aristocratos 开发而成，现累积近 4000+ Star。<br />首先先来看看bashtop资源监视器主界面，看似很高大上吧。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943895179-e90cae4c-58b7-4fbc-9e8c-c4e4730245e9.webp#clientId=uc0250edc-9c1a-4&from=paste&id=u3cb843dc&originHeight=694&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u96b2abdb-c8d5-4616-a98a-1edf6a341ac&title=)
<a name="VIvhp"></a>
#### 特征

- 易使用，快速响应的操作界面，可进行过程选择；
- 用于显示所选进程的详细统计信息的功能；
- 将 SIGTERM，SIGKILL，SIGINT 发送到选定的进程；
- 能够过滤流程；
- 排序选项之间可轻松切换；
- 网络使用情况的自动缩放图；
- 支持多个不同主题风格的配置；

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943895101-13222de3-772b-438e-af9a-b14f4b3b4343.webp#clientId=uc0250edc-9c1a-4&from=paste&id=ue8320d98&originHeight=694&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4b9b23f6-c34e-4a7e-a706-342ea88937d&title=)<br />可根据自己的需求修改及设置 Main 配色，在自定义设置主题时，重新生成一个名为`.theme`的主题文件并设置默认变量赋值。
```bash
#Bashtop theme with flat-remix colors
......

# Main background, empty for terminal default, need to be empty if you want transparent background
theme[main_bg]=""

# Main text color
theme[main_fg]="#737680"

# Title color for boxes
theme[title]="#272a34"

# Higlight color for keyboard shortcuts
theme[hi_fg]="#90"

# Background color of selected item in processes box
theme[selected_bg]="#b8174c"

# Foreground color of selected item in processes box
theme[selected_fg]="#ff"

# Color of inactive/disabled text
theme[inactive_fg]="#40"

# Misc colors for processes box including mini cpu graphs, details memory graph and details status text
theme[proc_misc]="#367bf0"

# Cpu box outline color
theme[cpu_box]="#367bf0"

# Memory/disks box outline color
theme[mem_box]="#19a187"

# Net up/down box outline color
theme[net_box]="#fd3535"

# Processes box outline color
theme[proc_box]="#4aaee6"

# Box divider line and small boxes line color
theme[div_line]="#50"

# Temperature graph colors
theme[temp_start]="#367bf0"
theme[temp_mid]="#b8174c"
theme[temp_end]="#d41919"

# CPU graph colors
theme[cpu_start]="#367bf0"
theme[cpu_mid]="#4aaee6"
theme[cpu_end]="#54bd8e"

# Mem/Disk free meter
theme[free_start]="#811035"
theme[free_mid]="#b8174c"
theme[free_end]="#d41919"

# Mem/Disk cached meter
theme[cached_start]="#2656a8"
theme[cached_mid]="#4aaee6"
theme[cached_end]="#23bac2"

# Mem/Disk available meter
theme[available_start]="#fea44c"
theme[available_mid]="#fd7d00"
theme[available_end]="#fe7171"

# Mem/Disk used meter
theme[used_start]="#12715f"
theme[used_mid]="#19a187"
theme[used_end]="#23bac2"

# Download graph colors
theme[download_start]="#367bf0"
theme[download_mid]="#19a187"
theme[download_end]="#4aaee6"

# Upload graph colors
theme[upload_start]="#8c42ab"
theme[upload_mid]="#b8174c"
theme[upload_end]="#d41919"
```
更多的主题配色，可通过`bashtop`项目中的`themes`文件夹查看。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1643944044542-36d3583b-9c39-45ec-bbef-f488eae17f20.png#clientId=uc0250edc-9c1a-4&from=paste&height=713&id=u6600640d&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=348424&status=done&style=none&taskId=u27fcb2a5-70f4-4450-a2a4-ae7e7a05a43&title=&width=1536)
<a name="H48SY"></a>
#### 手动安装
下载`bashtop`到`PATH`中，从克隆目录中运行。<br />Debian 快速安装
```bash
$ sudo ./build
```
卸载
```bash
$ sudo ./build --remove
```
目前bashtop还处于开发阶段，接下来，还有很多要去做新功能：

- 添加选项以及更改文本，图形和仪表的颜色。
- 添加并支持显示 AMD CPU 温度；
- 显示进程的树状视图；
- 重置网络下载/上传总数；
- 添加 gpu temp 的用法；
- 磁盘 io 统计信息；
- 调整所有框大小的选项；
- 添加命令行参数解析；
- 其他优化和代码清除；
- 稀疏之处添加评论；
- 修复跨平台的可兼容性；

GitHub 地址：[https://github.com/aristocratos/bashtop](https://github.com/aristocratos/bashtop)
