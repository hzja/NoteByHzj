Atuin 🐢 使用 SQLite 数据库取代了现有的 shell 历史，并为命令记录了额外的内容，可以通过一个非常美观的 UI 界面进行操作。此外，它还通过 Atuin 服务器，在节点之间提供可选的、完全加密的历史记录同步功能。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1696987231662-6cfd8c9c-00dc-4e93-a20e-7b6cddf7d2bc.gif#averageHue=%23002b36&clientId=uc496db76-36ba-4&from=paste&id=u708c8ac4&originHeight=775&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u47924ba4-4791-4601-ac12-cb869b9e764&title=)<br />除了通过 UI 进行搜索之外，还可以通过命令来进行更细粒度的搜索控制，比如**搜索昨天下午3点之后记录的所有成功的 make 命令**，则可以使用下面的命令：
```bash
atuin search --exit 0 --after "yesterday 3pm" make
```
可以使用作者托管的服务器，也可以使用自己的服务器！或者干脆不使用 sync 功能也可以。所有的历史记录同步都是加密的，所以数据安全方面完全不用担心。
<a name="wocT0"></a>
## 特点

- 重新绑定 up 和 ctrl-r 的全屏历史记录搜索 UI 界面
- 使用 sqlite 数据库存储 shell 历史记录
- 备份以及同步已加密的 shell 历史记录
- 在不同的终端、不同的会话以及不同的机器上都有相同的历史记录
- 记录退出代码、cwd、主机名、会话、命令持续时间，等等。
- 计算统计数据，如 "最常用的命令"。
- 不替换旧的历史文件
- 通过Alt-<num>快捷键快速跳转到之前的记录
- 通过 ctrl-r 切换过滤模式;可以仅从当前会话、目录或全局来搜索历史记录
- 支持的 Shells：zsh、bash、fish
<a name="LoJ50"></a>
## 使用默认的同步服务器
这将为您注册由项目作者托管的默认同步服务器。一切都是端到端加密的，所以信息是安全的！
```bash
bash <(curl https://raw.githubusercontent.com/ellie/atuin/main/install.sh)

atuin register -u <USERNAME> -e <EMAIL> -p <PASSWORD>
atuin import auto
atuin sync
```
<a name="DNxkM"></a>
## 仅离线 (不同步)
```bash
bash <(curl https://raw.githubusercontent.com/ellie/atuin/main/install.sh)
            
atuin import auto
```
<a name="xbubX"></a>
## 安装
<a name="FsHSI"></a>
### 脚本方式 (推荐)
安装脚本将完成设置，确保 shell 正确配置。
```bash
# 不要以root身份运行，如果需要的话，会要求root。
bash <(curl https://raw.githubusercontent.com/ellie/atuin/main/install.sh)
```
<a name="rqVcd"></a>
### 使用cargo
最好使用 rustup 来设置 Rust 工具链，然后就可以运行下面的命令:
```bash
cargo install atuin
```
<a name="aaiJR"></a>
### Homebrew
```bash
brew install atuin
```
<a name="VeBlc"></a>
### MacPorts
Atuin 也可以在 MacPorts 中找到
```bash
sudo port install atuin
```
<a name="yPFdZ"></a>
### Pacman
Atuin 在 Arch Linux 的 社区存储库 中可用。
```bash
pacman -S atuin
```
<a name="EF3nu"></a>
### 从源码编译安装
```bash
git clone https://github.com/ellie/atuin.git
cd atuin
cargo install --path .
```
<a name="hTPtM"></a>
## Shell 插件
安装二进制文件后，需要安装 shell 插件。如果使用的是脚本安装，那么这一切应该都会自动完成！
<a name="hI596"></a>
### zsh
```bash
echo 'eval "$(atuin init zsh)"' >> ~/.zshrc
```
或使用插件管理器:
```bash
zinit load ellie/atuin
```
<a name="uE88d"></a>
### bash
需要设置一些钩子(hooks), 所以首先需要安装 bash-preexec :
```bash
curl https://raw.githubusercontent.com/rcaloras/bash-preexec/master/bash-preexec.sh -o ~/.bash-preexec.sh
echo '[[ -f ~/.bash-preexec.sh ]] && source ~/.bash-preexec.sh' >> ~/.bashrc
```
然后设置 Atuin
```bash
echo 'eval "$(atuin init bash)"' >> ~/.bashrc
```
<a name="lS2Aw"></a>
### fish
添加
```bash
atuin init fish | source
```
到 ~/.config/fish/config.fish 文件中的 is-interactive 块中
<a name="JDlhw"></a>
## 常用命令
<a name="trAvW"></a>
### `atuin search <query>`
Atuin 搜索支持带有 * 或 % 字符的通配符。默认情况下，会执行前缀搜索（即，所有查询都会自动附加通配符）。

| 参数 | 描述 |
| --- | --- |
| --cwd/-c | 列出历史记录的目录（默认：所有目录） |
| --exclude-cwd | 不包括在此目录中运行的命令（默认值：none） |
| --exit/-e | 按退出代码过滤（默认：none） |
| --exclude-exit | 不包括以该值退出的命令（默认值：none） |
| --before | 仅包括在此时间之前运行的命令（默认值：none） |
| --after | 仅包含在此时间之后运行的命令（默认值：none） |
| --interactive/-i | 打开交互式搜索 UI（默认值：false） |
| --human/-h | 对时间戳和持续时间使用人类可读的格式（默认值：false） |

**示例**
```bash
# 打开交互式搜索 TUI 
atuin search -i

# 打开预装了查询的交互式搜索 TUI
atuin search -i atuin

# 搜索所有以 cargo 开头且成功退出的命令。
atuin search --exit 0 cargo

# 从当前目录中搜索所有在2021年4月1日之前运行且失败的命令。
atuin search --exclude-exit 0 --before 01/04/2021 --cwd .

#搜索所有以 cargo 开头，成功退出且是在昨天下午3点之后运行的命令。
atuin search --exit 0 --after "yesterday 3pm" cargo
```
<a name="AR16h"></a>
### `atuin sync` 手动触发同步
Atuin 可以将历史记录备份到服务器，并使用它来确保多台机器具有相同的 shell 历史记录。这都是端到端加密的，因此服务器操作员_永远_看不到您的数据！<br />任何人都可以托管一个服务器（尝试 `atuin server start`，更多文档将在后面介绍），但作者在 https://api.atuin.sh 上托管了一个。这是默认的服务器地址，也可以进行更改。同样，我_不能_看到您的数据，也不想。
<a name="zNobo"></a>
#### 注册一个同步账号
```bash
atuin register -u <USERNAME> -e <EMAIL> -p <PASSWORD>
```
用户名（USERNAME）必须是唯一的，电子邮件（EMAIL）仅用于重要通知（安全漏洞、服务更改等），注册后，意味着已经登录了 :) 同步应该从这里自动发生！
<a name="XDFxv"></a>
#### 密钥
由于数据是加密的， Atuin 将生成一个密钥。它被存储在 Atuin 的数据目录里（ Linus 上为 ~/.local/share/atuin）<br />也可以通过以下方式获得它
```bash
atuin key
```
:::danger
千万不要跟任何人分享密钥！
:::
<a name="jAJ67"></a>
#### 登录
如果登录到一个新的机器上，需要加密密钥（atuin key）。
```bash
atuin login -u <USERNAME> -p <PASSWORD> -k <KEY>
```
<a name="kU7aO"></a>
#### 注销
```bash
atuin logout
```
GitHub 仓库：[https://github.com/ellie/atuin](https://github.com/ellie/atuin)
