Linux<br />先来看看 bpytop 资源监视器主界面。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943690560-4d5778e1-0bd9-4ccb-8400-16476868ae2e.webp#clientId=u65317e18-70ea-4&from=paste&id=ub5ad6e35&originHeight=669&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf9487949-50ba-4753-bbea-22d91015210&title=)
<a name="LFAg4"></a>
#### 特征

- 易使用，带有游戏启发的菜单系统；
- 完全支持鼠标，所有带有突出显示键的按钮均可单击，并且鼠标滚动可在进程列表和菜单框中使用；
- 快速反应灵敏的 UI，带有 UP，DOWN 键可自行过程选择；
- 用于显示所选进程的详细统计信息的功能；
- 具有过滤过程的能力，可输入多个过滤器；
- 在排序选项之间轻松切换；
- 将 SIGTERM，SIGKILL，SIGINT 发送到选定的进程；
- 用于更改所有配置文件选项的 UI 菜单；
- 网络使用情况的自动缩放图；
- 如有新版本可用，可在菜单选项中进行显示；
- 显示磁盘的当前读写速度；

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943690723-f3fb7d89-6d4d-4887-ac4c-bbebde557536.webp#clientId=u65317e18-70ea-4&from=paste&id=ud5c43af9&originHeight=669&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udeb05ffb-320a-46dc-8fbd-a82fef3bdf9&title=)<br />Bpytop 使用与 bashtop 相同的主题文件，因此为 bashtop 制作的任何主题都可以使用。<br />可根据自己的需求修改及设置 Main 配色，在自定义设置主题时，重新生成一个名为`.theme`的主题文件并设置默认变量赋值。
```bash
#Bashtop theme with default colors and black background
......
# Main background, empty for terminal default, need to be empty if you want transparent background
theme[main_bg]="#00"

# Main text color
theme[main_fg]="#cc"

# Title color for boxes
theme[title]="#ee"

# Higlight color for keyboard shortcuts
theme[hi_fg]="#90"

# Background color of selected item in processes box
theme[selected_bg]="#7e2626" 

# Foreground color of selected item in processes box
theme[selected_fg]="#ee"

# Color of inactive/disabled text
theme[inactive_fg]="#40"

# Misc colors for processes box including mini cpu graphs, details memory graph and details status text
theme[proc_misc]="#0de756"

# Cpu box outline color
theme[cpu_box]="#3d7b46"

# Memory/disks box outline color
theme[mem_box]="#8a882e"

# Net up/down box outline color
theme[net_box]="#423ba5"

# Processes box outline color
theme[proc_box]="#923535"

# Box divider line and small boxes line color
theme[div_line]="#30"

# Temperature graph colors
theme[temp_start]="#4897d4"
theme[temp_mid]="#5474e8"
theme[temp_end]="#ff40b6"

# CPU graph colors
theme[cpu_start]="#50f095"
theme[cpu_mid]="#f2e266"
theme[cpu_end]="#fa1e1e"

# Mem/Disk free meter
theme[free_start]="#223014"
theme[free_mid]="#b5e685"
theme[free_end]="#dcff85"

# Mem/Disk cached meter
theme[cached_start]="#0b1a29"
theme[cached_mid]="#74e6fc"
theme[cached_end]="#26c5ff"

# Mem/Disk available meter
theme[available_start]="#292107"
theme[available_mid]="#ffd77a"
theme[available_end]="#ffb814"

# Mem/Disk used meter
theme[used_start]="#3b1f1c"
theme[used_mid]="#d9626d"
theme[used_end]="#ff4769"

# Download graph colors
theme[download_start]="#231a63"
theme[download_mid]="#4f43a3"
theme[download_end]="#b0a9de"

# Upload graph colors
theme[upload_start]="#510554"
theme[upload_mid]="#7d4180"
theme[upload_end]="#dcafde"
```
更多的主题配色，可通过 Bpytop 项目中的 themes 文件夹查看。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943690559-132ff090-6a3a-49eb-b264-c872a6200e93.webp#clientId=u65317e18-70ea-4&from=paste&id=ub2a7406e&originHeight=472&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5a4c6ff1-7a65-44e0-b802-7cd0afb6c3d&title=)
<a name="UYX4U"></a>
#### 安装
依赖项安装 Linux，使用 python3 和 git 安装<br />安装 psutil python 模块（可能需要sudo）
```bash
# python3 -m pip install psutil
```
依赖项安装 OSX，用于显示 CPU 温度。
```bash
# brew install osx-cpu-temp
```
手动安装 Linux，OSX 和 FreeBSD
```bash
# git clone https://github.com/aristocratos/bpytop.git
# cd bpytop
# sudo make install
```
<a name="b9oIU"></a>
#### 命令行选项
```bash
USAGE: bpytop [argument]

Arguments:
    -m, --mini            Start in minimal mode without memory and net boxes  // 最小模式下启动
    -v, --version         Show version info and exit  // 显示版本信息并退出
    -h, --help            Show this help message and exit  // 显示此帮助信息并退出
    --debug               Start with loglevel set to DEBUG overriding value set in config
```
由此看来目前 bpytop 还处于开发阶段，接下来，还有很多要去做新功能：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1643943690694-92830650-c3a1-4be1-ad2c-24625b870f2f.webp#clientId=u65317e18-70ea-4&from=paste&id=uc48d0a27&originHeight=853&originWidth=632&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue4f5cc3d-0ec6-4014-9dbd-12abb5ff761&title=)<br />该项目 GitHub 地址：[https://github.com/aristocratos/bpytop](https://github.com/aristocratos/bpytop)
