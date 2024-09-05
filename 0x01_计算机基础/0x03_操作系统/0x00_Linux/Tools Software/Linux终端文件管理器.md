Linux 终端文件管理
<a name="4fD5J"></a>
### 1、nnn
项目的Github仓库地址：[https://github.com/jarun/nnn](https://github.com/jarun/nnn)<br />**nnn **是一个功能齐全的文件管理器，用于低端设备和常规桌面，它非常轻快。<br />**nnn **也是一个磁盘使用分析器、一个模糊应用程序启动程序、一个批处理文件重命名器和一个文件选择器。许多插件都可以扩展其功能，自定义插件很容易添加，有一个独立的(neo)vim 选择插件项目。<br />它运行在 Linux、MacOS、Raspberry PI、BSD、Cygwin、Linux subsystem for Windows 和 Android 上的 Termux 上。<br />nnn 与 DES 和 GUI 实用程序无缝地工作。它几乎是零配置(有合理的默认值)，不到 5 分钟即可设置。<br />**特性**

- Frugal
   - 通常需要少于3.5MB的常驻内存
   - 适用于8-bit colors
   - 磁盘IO敏感（很少磁盘读写）
   - 没有FPU使用情况（所有整数运算，甚至文件大小）
   - 通过快速重画行来最大程度地减少屏幕刷新
   - 微小的二进制文件（通常小于100KB）
- Portable
   - 静态链接的二进制文件可用
   - 与语言无关的插件
   - 最小的库深度，易于编译
   - 使用make变量编译输入/输出功能
   - 没有配置文件，具有合理默认值的最小配置
   - 可在许多包装机上广泛使用
   - Unicode支持
- Quality
   - 隐私保护（不会收集未经确认的用户数据）
   - 兼容POSIX，遵循Linux内核编码样式
   - 高度优化的静态分析集成代码
- Modes
   - 浅（默认），细节
   - 磁盘使用情况分析器（块/视在）
   - 文件选择器，（neo）vim插件
- Navigation
   - 带目录自动选择的从_类型到导航_模式
   - 具有自定义颜色的上下文（也称为选项卡/工作区）
   - 会话，带有热键的书签；固定并访问目录
   - 远程挂载（需要sshfs，rclone）
   - 熟悉快捷键（箭头，~，-，@），快速参考
   - CD退出（_轻松_集成外壳）
   - 自动前进打开文件
- Search
   - 随需输入即时过滤
   - 正则表达式（POSIX / PCRE）和字符串（默认）过滤器
   - 子树搜索插件可打开或编辑文件
- Sort
   - 默认情况下订购纯数字名称（访问_/ proc_）
   - 不区分大小写的版本（又名自然）排序
   - 按文件名，访问/更改/修改（默认）时间，大小，扩展名
   - 反向排序
- Mimes
   - 使用桌面开启器打开或指定自定义开启器
   - 在基于FIFO的预览器中预览悬停的文件
   - 创建，列出，提取，装入（基于FUSE）档案
   - 选择在EDITOR中打开所有文本文件
- Information
   - 详细的文件信息
   - 媒体信息插件
- Convenience
   - 使用热键运行插件和自定义命令
   - 符合FreeDesktop的垃圾桶（需要trash-cli）
   - 跨目录文件/全部/范围选择
   - 用于选择或目录的批处理重命名器
   - 显示来自stdin的文件列表
   - 复制（as），移动（as），删除，存档，链接选择
   - 目录更新，有关cp，mv，rm完成的通知
   - 在选择时将文件路径复制到系统剪贴板
   - 创建（与父母一起），重命名，复制（任何地方）文件和目录
   - 启动GUI应用，运行命令，生成外壳，切换可执行文件
   - 悬停的文件设置为`$nnn`提示符并生成了shell
   - 可配置的空闲超时后锁定终端
<a name="kt99k"></a>
### 2、ranger
项目的Github仓库地址：[https://github.com/ranger/ranger](https://github.com/ranger/ranger)<br />Ranger 具有 VI 键绑定的控制台文件管理器，提供了简约而美观的 curses 界面，具有目录层次结构的视图，带有 rifle 的一个文件启动器，擅长自动找出要用于哪种文件类型的程序。<br />易于维护高级语言文件管理器、切换目录和浏览文件系统方便快捷、小巧、基于控制台可集成到 unix shell 中、多窗格视图模式。<br />ranger 分为三个部分：

- 左边：上级目录结构
- 中间：当前可操作的目录
- 右边：当前目录下所有的文件
<a name="4N7cX"></a>
### 3、cfiles
项目的Github仓库地址：[https://github.com/mananapr/cfiles](https://github.com/mananapr/cfiles)<br />cfiles 是一个终端文件管理器，具有像 VIM 一样的键绑定，使用 ncurses 库用 C 语言编写，提供轻巧，快速和最小化的界面。
<a name="Hfv23"></a>
### 4、dfshow
项目的Github仓库地址：[https://github.com/roberthawdon/dfshow](https://github.com/roberthawdon/dfshow)<br />dfshow（Directory File Show）：它是 Unix 操作系统的终端文件管理器，是Larry Kroeker的DF-EDIT（目录文件编辑器）针对MS-DOS和PC-DOS系统的某些应用程序的类Unix重写，基于 1986 年发布的 DF-EDIT 2.3d，DF-SHOW 完全是自由开源的，并在 GPLv3 下发布。<br />DF-SHOW 特征：

- 列出文件
- 查看文件
- 编辑文件
- 复制文件
- 创建链接
- 重命名文件
- 删除文件
- 创建目录
- 更新文件权限，所有者和组
- 在文件中搜索匹配的文件
- 启动可执行文件

DF-SHOW 实际上是两个应用程序的结合，分别是：show和sf。<br />show应用程序（类似于 ls 命令）：使用户可查看磁盘上文件和目录的名称以及有关文件的信息，用于显示目录的内容、创建新目录、重命名和删除文件/文件夹、更新权限、搜索文件等。<br />查看目录中的内容列表，请使用以下命令：
```bash
show <directory_names>
```
<directory_name> 参数：可以由当前用户在系统上访问的任何目录访问。<br />`sf` 应用程序（显示文件）：用于显示文件的内容。
```bash
sf <file>
```
`<file> 参数`：可以是当前用户在系统上可以访问的任何文件，如不指定文件的情况下调用了 show file 实用程序，则会提示用户在应用程序中加载文件。
