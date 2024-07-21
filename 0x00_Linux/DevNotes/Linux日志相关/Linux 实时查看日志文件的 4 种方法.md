Linux<br />在 Linux 下如何才能实时查看日志内容呢？有很多工具可以帮助在文件持续修改的同时输出文件内容，最常用的莫过于 `tail` 命令了。
<a name="TK0wd"></a>
## 1、tail Command – Monitor Logs in Real Time
如前所述，tail 命令是实时显示日志的最常用的方法。不过该命令有两个版本，如下所示。<br />第一个例子是为 tail 命令加上 `-f` 参数。
```bash
$ sudo tail -f /var/log/apache2/access.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679604570-98538663-d9db-4f88-b6ec-f52de177b630.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=u473d4392&originHeight=281&originWidth=788&originalType=url&ratio=1&status=done&style=none&taskId=u8eed6b8f-7696-4cd2-bdd3-02fb9a6dba7)<br />第二个例子就是 `tailf` 命令。它本身内建了 `-f` 参数，因此无需再为它指定 `-f` 参数。
```bash
$ sudo tailf /var/log/apache2/access.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679604404-3a22bfd2-cace-4872-b452-293afd09e7a4.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=uf3d73870&originHeight=386&originWidth=870&originalType=url&ratio=1&status=done&style=none&taskId=uabd8b8b1-63e5-4542-9600-fd6952e2405)<br />通常 Linux 服务器上的日志都是轮转日志。这种情况下，需要用 `-F` 参数。<br />`tail -F` 会监控是否创建了新日志（所谓新日志指的是同一个名字，但是 fd 不一样的日志文件），并且会转而显示新日志的内容，而不是老文件的内容。
```bash
$ sudo tail -F /var/log/apache2/access.log
```
然而，默认情况下 `tail` 命令只会显示文件最后 10 行的内容。如果只想在实时模式下查看最后两行的内容，那么可以连用 `-n` 和 `-f` 参数，如下这样：
```bash
$ sudo tail -n2 -f /var/log/apache2/access.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679604425-07987a43-9331-49e7-b50b-41fda3876c14.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=ubc6119af&originHeight=106&originWidth=776&originalType=url&ratio=1&status=done&style=none&taskId=u2b44b6be-03f9-48dc-94e8-1daef65be4e)
<a name="Xo1T7"></a>
## 2、Multitail Command – Monitor Multiple Log Files in Real Time
另一个好玩的命令是 Multitail Command。从名字中就能看出它可以实时监控多个日志，Multitail 还能前后翻阅被监控的文件。<br />使用下面命令可以在基于 Debian 或 RedHat 的系统中安装 Mulitail。
```bash
$ sudo apt install multitail   [On Debian & Ubuntu]
$ sudo yum install multitail   [On RedHat & CentOS]
$ sudo dnf install multitail   [On Fedora 22+ version]
```
下面例子演示了如何同时显示两个日志文件。
```bash
$ sudo multitail /var/log/apache2/access.log /var/log/apache2/error.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679604350-67aa1991-b9c2-4d97-a3d8-1154691f3efc.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=u89a88279&originHeight=637&originWidth=830&originalType=url&ratio=1&status=done&style=none&taskId=u4ff2da22-1e18-4192-b126-d5107a456db)
<a name="u3tFg"></a>
## 3、lnav Command – Monitor Multiple Log Files in Real Time
另一个类似 Multitail 的命令是 Lnav，Lnav 也能实时监控多个文件。<br />使用下面命令可以在基于 Debian 或 RedHat 的系统中安装 Lnav。
```bash
$ sudo apt install lnav   [On Debian & Ubuntu]
$ sudo yum install lnav   [On RedHat & CentOS]
$ sudo dnf install lnav   [On Fedora 22+ version]
```
使用 Lnav 同时查看两个日志的方法如下：
```bash
$ sudo lnav /var/log/apache2/access.log /var/log/apache2/error.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679604367-18c7a0a4-ec37-4093-ad99-b06d3213803b.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=u6e41647d&originHeight=280&originWidth=990&originalType=url&ratio=1&status=done&style=none&taskId=ua91a5a74-14b5-4293-a754-5b6a75831f7)
<a name="Ew15k"></a>
## 4、less Command – Display Real Time Output of Log Files
最后可以用 less 命令 查看日志文件，然后按下 `Shift+F` 也能实时查看日志内容。<br />跟 `tail` 一样, 在 less 中按下 `Shift+F` 会追踪文件末尾的内容。也可以在调用 `less` 命令时就加上 `+F` 参数。
```bash
sudo less + F /var/log/apache2/access.log
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636679605042-67580a2c-3d9f-48a8-a28f-b4f9f34972be.webp#clientId=uea4b7b31-9bf9-4&from=paste&id=u51941d34&originHeight=485&originWidth=940&originalType=url&ratio=1&status=done&style=none&taskId=u3673b093-d7c7-40e1-8480-938cdbf1d88)

