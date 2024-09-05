Linux ccat <br />`cat` 命令是一个 Unix 或类 Unix 下文本的查看命令，同时也可以配合重定向实现合并与复制功能。它是 GNU Linux 和类 Linux 操作系统中最常用的命令之一，使用频率非常高。<br />但是对于编程人员来讲的话，cat 命令有个问题，就是它的输出不带有任何颜色标记。如果用 cat 命令去查看代码文件的话，关键字没有颜色标记真的非常不方便阅读。<br />为了解决这一个痛点，有大佬就开发出了 `ccat` 这个命令。`ccat` 与 `cat` 非常相似，但是使用ccat命令，显示的代码内容会带有颜色，阅读起来非常舒服。目前，`ccat` 支持 JavaScript，Java，Ruby，Python，Go，C 和 JSON 语言。
<a name="KNRqM"></a>
# 1、安装ccat
1）`ccat` 命令在 AUR 里直接可用，所以如果系统里有 AUR 助手的话，可以直接安装。比如 Arch 系统，直接使用 `yay` 命令就可以安装。
> Arch 用户软件仓库 通常称为 AUR，AUR 助手是一个包装程序，允许用户从 AUR 存储库安装软件包，而无需手动干预。

```bash
$ yay -S ccat
```
2）在其他 Linux 发行版中，需要编译并安装ccat，步骤如下所示：

- 使用如下命令下载源码归档压缩包
```bash
$ wget https://github.com/jingweno/ccat/releases/download/v1.1.0/linux-amd64-1.1.0.tar.gz
```

- 解压缩下载的归档文件
```bash
$ tar xfz linux-amd64-1.1.0.tar.gz
```

- 将ccat可执行文件复制到系统 `$PATH` 中（例如，/usr/local/bin/）
```bash
$ sudo cp linux-amd64-1.1.0/ccat /usr/local/bin/
```

- 最后，使用 chmod 为 ccat 命令添加可执行权限
```bash
$ sudo chmod +x /usr/local/bin/ccat
```
<a name="03NmI"></a>
# 2、ccat的对比及使用
ccat 使用方法和 cat 命令非常相似，先用 cat 命令查看文本文件，例如 hello.c 。
```bash
$ cat hello.c
```
效果如下图<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1595386816556-a9f56fce-78e4-407b-8c7c-cce29a73d402.png#align=left&display=inline&height=211&originHeight=211&originWidth=321&size=0&status=done&style=none&width=321)<br />cat<br />可以看出来代码没有任何颜色标记。现在，再使用 ccat 查看文本文件 hello.c ，并查看其如何显示输出。
```bash
$ ccat hello.c
```
效果如下图：<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1595386816612-8300972a-cfdc-4087-9f81-508efcc8ed2e.png#align=left&display=inline&height=213&originHeight=213&originWidth=343&size=0&status=done&style=none&width=343)<br />ccat<br />ccat 使用语法高亮显示输出，而 cat 命令仅以系统的默认主题颜色显示输出。<br />还可以一次显示多个文件的输出，如下所示。
```bash
$ ccat test.txt example.txt
```
以HTML格式显示输出。为此，只需在末尾添加“ –html”选项。
```bash
$ ccat test.txt --html
```
不仅是本地文件，还可以像下面这样直接在网络上显示文件的内容。
```bash
$ curl https://raw.githubusercontent.com/jingweno/ccat/master/main.go | ccat
```
要查看默认的代码颜色，运行如下命令。
```bash
$ ccat --palette
```
可以设置自己的颜色代码。
```bash
$ ccat -G String="_fuchsia_" -G Plaintext="darkteal" hello.c
```
ps:在下载步骤中可能会遇到如下错误<br />![](https://cdn.nlark.com/yuque/0/2020/png/396745/1595386816573-373ee808-0566-468f-8dee-7601efe071eb.png#align=left&display=inline&height=96&originHeight=96&originWidth=724&size=0&status=done&style=none&width=724)ccat_error<br />这是由于 github 升级了 openssl，停用了 tlsv1.1，只需要更新本机的 openssl 以及 git 工具即可解决该问题。
<a name="0KzD2"></a>
# 3. 替换系统中的cat
如果喜欢 ccat 并想用 ccat 替换默认的 cat 命令，则只需创建一个别名即可。<br />如果是从 Arch Linux 中的 AUR 安装的，请在 `~/.bashrc` 文件中添加以下行。
```bash
alias cat=ccat
```
如果是从编译的二进制文件安装的，则在 `~/.bashrc` 文件中添加以下行。
```bash
alias cat=/usr/local/bin/ccat
```
最后，运行以下命令以使更改生效。
```bash
$ source ~/.bashrc
```
要在整个系统范围内添加它（所有用户都可以使用这个命令），请在 `/etc/bashrc` （在基于RPM的系统上）或 `/etc/profile`（在Debian的系统上）文件中添加以上条目。

