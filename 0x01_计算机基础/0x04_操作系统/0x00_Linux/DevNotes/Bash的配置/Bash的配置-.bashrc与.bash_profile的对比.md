Linux Bash <br />**Bash** 启动文件以及 **.bashrc** 和 **.bash_ profile** 之间的区别：
<a name="MgiuV"></a>
### Interactive Login 和 Non-Login Shell
**Bash** 被调用时，会从一组启动文件中读取并执行命令，至于读取哪些文件取决于 **Shell** 是作为交互式登录调用还是作为非登录 **Shell** 调用。<br />其实，**Shell** 可以是交互式的，也可以是非交互式的。简单一点地说，交互式 **Shell** 是一种读写用户终端的 **Shell**，而非交互式 **Shell** 完全是与终端无关的 **Shell**。值得注意的是，交互式 **Shell** 可以是登录 **Shell** ，也可以是非登录 **Shell**。<br />有几种情况可以调用登录 **Shell**，一种是当用户通过 `ssh` 或本地方式远程登录到终端，还有就是使用 `--login` 选项启动 **Bash** 。比如在 **Shell** 提示符中键入 `bash` 或打开新的 **Gnome** 终端选项卡时。交互式非登录 **Shell** 从登录 **Shell** 调用。
<a name="xy8UQ"></a>
### Bash启动文件
当 **Bash** 作为交互式登录 **Shell** 被调用时，它将查找 **/etc/profile** 文件，如果该文件存在，它将运行文件中列出的命令。然后 **Bash** 按以下的顺序搜索 **~/.bash_profile** 、**~/.bash_login** 和 **~/.profile** 文件，并在第一个找到的文件中执行命令。<br />当 **Bash** 作为交互式非登录 **Shell** 被调用时，它从 **~/.bashrc** 文件读取并执行命令，需要指出的是该文件必须存在并且它是可读的才行。
<a name="4DUZ4"></a>
### .bashrc 和 .bash_配置文件之间的差异
在 **Bash** 作为交互式登录 **Shell** 被调用的情况下，**.bash_profile** 才会被读取、执行，而 **.bashrc** 是启动交互式非登录 **Shell** 时才被执行的。一般使用 **.bash_profile** 来运行只运行一次的命令，比如运行自定义环境变量 `PATH` 的命令。<br />每次启动新 **Shell** 时应该将运行的命令放在 **.bashrc** 文件中，这些命令包括别名和函数、自定义提示、历史自定义等。<br />通常，**~/.bash_profile** 包含的命令来源于 **.bashrc** 文件中，示例如下：
```bash
[root@iZuligp6e1dyzfZ ~]# cat ~/.bashrc
# .bashrc

# User specific aliases and functions

alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
```
这意味着每次登录到终端时，上面两个文件都会被读取并执行其中的命令。<br />大多数 **Linux** 发行版都是使用 **~/.profile** 而不是 **~/.bash_profile**。因为 **~/.profile** 文件可以被所有 **Shell** 读取，而 **~/.bash_ profile** 仅能被 **Bash Shell** 读取。<br />如果系统上没有这些启动文件，那么可以自行创建它们。
<a name="Rqrza"></a>
### 结论
**.bash_profile** 和 **.bashrc** 是在调用 **Bash** 时运行的包含 **Shell** 命令的文件。**.bash_profile** 可以在交互式登录 **Shell** 上被读取和执行，而 **.bashrc** 可以在非登录 **Shell** 上被读取和执行。
