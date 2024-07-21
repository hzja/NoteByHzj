Linux tmux<br />Tmux是一个优秀的终端复用软件，类似GNU Screen，但来自于OpenBSD，采用BSD授权。使用它最直观的好处就是，通过一个终端登录远程主机并运行tmux后，在其中可以开启多个控制台而无需再“浪费”多余的终端来连接这台远程主机。是BSD实现的Screen替代品，相对于Screen，它更加先进：支持屏幕切分，而且具备丰富的命令行参数，使其可以灵活、动态的进行各种布局和操作。<br />Tmux 可用于在一个终端窗口中运行多个终端会话。不仅如此，还可以通过 Tmux 使终端会话运行于后台或是按需接入、断开会话，这个功能非常实用。
<a name="DhqHc"></a>
## tmux的Github仓库地址
[tmux的Github](https://github.com/tmux/tmux)<br />[https://github.com/tmux/tmux](https://github.com/tmux/tmux)<br />
<a name="fk94x"></a>
## tmux的下载安装
```bash
yum install tmux 
```
<a name="qYZri"></a>
## tmux的常用命令
<a name="NKa9L"></a>
### 运行tmux
```bash
tmux
```
<a name="Fvh1a"></a>
### 新建tmux会话：tmux new -s SESSION-NAME
(注：SESSION-NAME表示会话名称，可以为字母或数字)<br />也可以不用指定会话名称 ( 不推荐): # tmux  new<br />tmux [new -s 会话名 -n 窗口名]
```bash
tmux new -s second-tmux
```
:::info
加上参数-d，表示在后台新建会话：`_tmux new -s shibo -d_`
:::
<a name="wyZSX"></a>
### 查看已创建的会话：tmux ls
```bash
tmux ls
```
<a name="E1o1X"></a>
### 进入一个已知会话：tmux a -t SESSION-NAME 或tmux attach -t SESSION-NAME
```bash
[root@iZuligp6e1dyzfZ ~ 15:32]# tmux ls
0: 1 windows (created Thu Aug 20 13:54:50 2020) [116x29] (attached)
[root@iZuligp6e1dyzfZ ~ 15:32]# tmux a -t 0
```
<a name="exHCO"></a>
### 暂时离开当前会话：tmux detach
(  该命令会从当前会话中退出去, 因此才会有稍后重新接入会话这么一说 )
```bash
tmux detach
```
<a name="xsQHb"></a>
### 恢复会话：tmux at [-t 会话名]
```bash
tmux at [-t 会话名]
```
<a name="rAq3Z"></a>
### 关闭会话：tmux kill-session -t SESSION-NAME
( 在会话内部或外部执行均可)
```bash
tmux kill-session -t 0
```
> 注： 1. 单独运行tmux命令，即开启一个tmux会话 ; 2. 不能在tmux会话里面再新建会话，会报错："sessions should be nested with care, unset $TMUX to force"

关闭所有会话
```bash
tmux ls | grep : | cut -d. -f1 | awk '{print substr($1, 0, length($1)-1)}' | xargs kill
```
<a name="KeVMw"></a>
### 分屏操作
很多情况下, 需要在一个会话中运行多个命令，执行多个任务,可以在一个会话的多个窗口里组织他们。<br />1. 分屏：分为水平分屏和垂直分屏
<a name="0d3l2"></a>
#### 水平分屏
快捷键：先按 Ctrl+b, 放开后再按%(Shift + 5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597912492209-be5490ed-4ad3-44de-b9d4-3a49d5cde806.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3218&size=1983682&status=done&style=none&width=1072.6666666666667)
<a name="9f34f36d"></a>
#### 垂直分屏
快捷键：先按 Ctrl+b, 放开后再按Shift + "<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597912569179-d5733979-5065-434c-9109-45bc4a11074d.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3218&size=1996386&status=done&style=none&width=1072.6666666666667)
<a name="02918fad"></a>
#### 分屏后的窗口中的光标互相切换
快捷键：先按Ctrl+b, 放开后再按下o<br />切换tmux会话终端<br />快捷键：先按Ctrl+b, 放开后再按s<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597912695277-28dbc0af-48bb-48dd-954a-e55d8eb66b68.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3218&size=2014131&status=done&style=none&width=1072.6666666666667)
<a name="Ceyrm"></a>
#### 终端内显示时间 
快捷键：先按Ctrl+b, 放开后再按t<br />退出时间界面：按q键<br />**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597912726285-1f2bccc3-2c02-42f0-919c-9916c9f6ec9f.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3218&size=1962207&status=done&style=none&width=1072.6666666666667)**
<a name="T9ycr"></a>
### 其他快捷键操作
<a name="DE6Ht"></a>
#### 终止一个终端窗口(需确认) 
快捷键：exit 或 先按Ctrl+b, 放开后再按 &<br />在当前窗口的基础上再打开一个新的窗口 <br />快捷键：先按Ctrl+b, 放开后再按c
<a name="M9HlJ"></a>
#### 暂时退出当前会话 
快捷键：先按Ctrl+b, 放开后再按 d<br />查看面板编号 <br />快捷键：先按Ctrl+b, 放开后再按q<br />关闭所有分屏后的窗口，即合并为一个窗口 <br />快捷键：先按Ctrl+b, 放开后再按！
<a name="IVeam"></a>
### 快捷命令介绍
tmux new -s 会话名  //新起一个会话<br />tmux ls //查看现有的会话<br />tmux a //进入最近的会话<br />tmux a -t  会话名//进入指定会话<br />tumx kill-session -t 会话名 //干掉指定会话
```bash
ctrl+b ?            显示快捷键帮助
ctrl+b 空格键       采用下一个内置布局，这个很有意思，在多屏时，用这个就会将多有屏幕竖着展示
ctrl+b !            把当前窗口变为新窗口
ctrl+b  "           模向分隔窗口
ctrl+b %            纵向分隔窗口
ctrl+b q            显示分隔窗口的编号
ctrl+b o            跳到下一个分隔窗口。多屏之间的切换
ctrl+b 上下键      上一个及下一个分隔窗口
ctrl+b C-方向键    调整分隔窗口大小
ctrl+b &           确认后退出当前tmux
ctrl+b [           复制模式，即将当前屏幕移到上一个的位置上，其他所有窗口都向前移动一个。
ctrl+b c           创建新窗口
ctrl+b n           选择下一个窗口
ctrl+b l           最后使用的窗口
ctrl+b p           选择前一个窗口
ctrl+b w           以菜单方式显示及选择窗口
ctrl+b s           以菜单方式显示和选择会话。这个常用到，可以选择进入哪个tmux
ctrl+b t           显示时钟。然后按enter键后就会恢复到shell终端状态
ctrl+b d           脱离当前会话；这样可以暂时返回Shell界面，输入tmux attach能够重新进入之前的会话
```
control+b //唤起tmux(命令前缀)<br />c  //创建新窗口<br />w //查看所有窗口<br />" //横分面板<br />% //竖分面板<br />, //修改窗口名<br />? //查看所有命令,q退出<br />q //显示面板号,快速输入面板号可以选择相应面板<br />d //挂起当前会话<br />s //查看所有会话,q退出<br />$ //修改会话名称<br />p //切换窗口<br />: //命令行模式
<a name="XXwBW"></a>
### tmux命令速查表
| Ctrl+b |  | 激活控制台；此时以下按键生效 |
| --- | --- | --- |
| 系统操作 | ? | 列出所有快捷键；按q返回 |
|  | d | 脱离当前会话；这样可以暂时返回Shell界面，输入tmux attach能够重新进入之前的会话 |
|  | D | 选择要脱离的会话；在同时开启了多个会话时使用 |
|  | Ctrl+z | 挂起当前会话 |
|  | r | 强制重绘未脱离的会话 |
|  | s | 选择并切换会话；在同时开启了多个会话时使用 |
|  | : | 进入命令行模式；此时可以输入支持的命令，例如kill-server可以关闭服务器 |
|  | [ | 进入复制模式；此时的操作与vi/emacs相同，按q/Esc退出 |
|  | ~ | 列出提示信息缓存；其中包含了之前tmux返回的各种提示信息 |
| 窗口操作 | c | 创建新窗口 |
|  | & | 关闭当前窗口 |
|  | 数字键 | 切换至指定窗口 |
|  | p | 切换至上一窗口 |
|  | n | 切换至下一窗口 |
|  | l | 在前后两个窗口间互相切换 |
|  | w | 通过窗口列表切换窗口 |
|  | , | 重命名当前窗口；这样便于识别 |
|  | . | 修改当前窗口编号；相当于窗口重新排序 |
|  | f | 在所有窗口中查找指定文本 |
| 面板操作 | ” | 将当前面板平分为上下两块 |
|  | % | 将当前面板平分为左右两块 |
|  | x | 关闭当前面板 |
|  | ! | 将当前面板置于新窗口；即新建一个窗口，其中仅包含当前面板 |
|  | Ctrl+方向键 | 以1个单元格为单位移动边缘以调整当前面板大小 |
|  | Alt+方向键 | 以5个单元格为单位移动边缘以调整当前面板大小 |
|  | Space | 在预置的面板布局中循环切换；依次包括even-horizontal、even-vertical、main-horizontal、main-vertical、tiled |
|  | q | 显示面板编号 |
|  | o | 在当前窗口中选择下一面板 |
|  | 方向键 | 移动光标以选择面板 |
|  | { | 向前置换当前面板 |
|  | } | 向后置换当前面板 |
|  | Alt+o | 逆时针旋转当前窗口的面板 |
|  | Ctrl+o | 顺时针旋转当前窗口的面板 |

<a name="h3QHg"></a>
### 在 Tmux 中，按下 Tmux 前缀 `ctrl+b`，然后可执行的操作
<a name="pT6U3"></a>
#### 会话
```
:new<回车>  启动新会话
s           列出所有会话
$           重命名当前会话
```
<a name="8y2Pm"></a>
#### 窗口 (标签页)
```
c  创建新窗口
w  列出所有窗口
n  后一个窗口
p  前一个窗口
f  查找窗口
,  重命名当前窗口
&  关闭当前窗口
```
<a name="lc5J7"></a>
#### 调整窗口排序
```
swap-window -s 3 -t 1  交换 3 号和 1 号窗口
swap-window -t 1       交换当前和 1 号窗口
move-window -t 1       移动当前窗口到 1 号
```
<a name="ebLqW"></a>
#### 窗格（分割窗口）
```
%  垂直分割
"  水平分割
o  交换窗格
x  关闭窗格
⍽  左边这个符号代表空格键 - 切换布局
q 显示每个窗格是第几个，当数字出现的时候按数字几就选中第几个窗格
{ 与上一个窗格交换位置
} 与下一个窗格交换位置
z 切换窗格最大化/最小化
```
<a name="AzgHv"></a>
#### 同步窗格
这么做可以切换到想要的窗口，输入 Tmux 前缀和一个冒号呼出命令提示行，然后输入：
```
:setw synchronize-panes
```
可以指定开或关，否则重复执行命令会在两者间切换。 这个选项值针对某个窗口有效，不会影响别的会话和窗口。 完事儿之后再次执行命令来关闭。
<a name="35970413"></a>
#### 调整窗格尺寸
如果不喜欢默认布局，可以重调窗格的尺寸。虽然这很容易实现，但一般不需要这么干。以下命令用来调整窗格：
```
PREFIX : resize-pane -D          当前窗格向下扩大 1 格
PREFIX : resize-pane -U          当前窗格向上扩大 1 格
PREFIX : resize-pane -L          当前窗格向左扩大 1 格
PREFIX : resize-pane -R          当前窗格向右扩大 1 格
PREFIX : resize-pane -D 20       当前窗格向下扩大 20 格
PREFIX : resize-pane -t 2 -L 20  编号为 2 的窗格向左扩大 20 格
```
<a name="f6c541fb"></a>
#### 文本复制模式
按下**前缀 [**进入文本复制模式。可以使用方向键在屏幕中移动光标。默认情况下，方向键是启用的。在配置文件中启用 Vim 键盘布局来切换窗口、调整窗格大小。Tmux 也支持 Vi 模式。要是想启用 Vi 模式，只需要把下面这一行添加到 .tmux.conf 中：
```
setw -g mode-keys vi
```
启用这条配置后，就可以使用 h、j、k、l 来移动光标了。<br />想要退出文本复制模式的话，按下回车键就可以了。一次移动一格效率低下，在 Vi 模式启用的情况下，可以辅助一些别的快捷键高效工作。<br />例如，可以使用 w 键逐词移动，使用 b 键逐词回退。使用 f 键加上任意字符跳转到当前行第一次出现该字符的位置，使用 F 键达到相反的效果。
```
vi             emacs        功能
^              M-m          反缩进
Escape         C-g          清除选定内容
Enter          M-w          复制选定内容
j              Down         光标下移
h              Left         光标左移
l              Right        光标右移
L                           光标移到尾行
M              M-r          光标移到中间行
H              M-R          光标移到首行
k              Up           光标上移
d              C-u          删除整行
D              C-k          删除到行末
$              C-e          移到行尾
:              g            前往指定行
C-d            M-Down       向下滚动半屏
C-u            M-Up         向上滚动半屏
C-f            Page down    下一页
w              M-f          下一个词
p              C-y          粘贴
C-b            Page up      上一页
b              M-b          上一个词
q              Escape       退出
C-Down or J    C-Down       向下翻
C-Up or K      C-Up         向下翻
n              n            继续搜索
?              C-r          向前搜索
/              C-s          向后搜索
0              C-a          移到行首
Space          C-Space      开始选中
               C-t          字符调序
```
<a name="TfTk7"></a>
### 其他命令
```
d  退出 tmux（tmux 仍在后台运行）
t  窗口中央显示一个数字时钟
?  列出所有快捷键
:  命令提示符
```
<a name="5ZDma"></a>
### 配置选项
```
# 鼠标支持 - 设置为 on 来启用鼠标
* setw -g mode-mouse off
* set -g mouse-select-pane off
* set -g mouse-resize-pane off
* set -g mouse-select-window off
# 设置默认终端模式为 256color
set -g default-terminal "screen-256color"
# 启用活动警告
setw -g monitor-activity on
set -g visual-activity on
# 居中窗口列表
set -g status-justify centre
# 最大化/恢复窗格
unbind Up bind Up new-window -d -n tmp \; swap-pane -s tmp.1 \; select-window -t tmp
unbind Down
bind Down last-window \; swap-pane -s tmp.1 \; kill-window -t tmp
```
<a name="fUDVk"></a>
### 配置文件（~/.tmux.conf）：
# 基础设置<br />set -g default-terminal "screen-256color"<br />set -g display-time 3000<br />set -g escape-time 0<br />set -g history-limit 65535<br />set -g base-index 1<br />set -g pane-base-index 1

# 前缀绑定 (Ctrl+a)<br />set -g prefix ^a<br />unbind ^b<br />bind a send-prefix

# 分割窗口<br />unbind '"'<br />bind - splitw -v<br />unbind %<br />bind | splitw -h

# 选中窗口<br />bind-key k select-pane -U<br />bind-key j select-pane -D<br />bind-key h select-pane -L<br />bind-key l select-pane -R

# copy-mode 将快捷键设置为 vi 模式<br />setw -g mode-keys vi

# 启用鼠标(Tmux v2.1)<br />set -g mouse on

# 更新配置文件<br />bind r source-file ~/.tmux.conf \; display "已更新"
