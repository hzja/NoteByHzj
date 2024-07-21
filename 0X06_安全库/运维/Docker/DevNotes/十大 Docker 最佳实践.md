Docker<br />展示了 10 种强化 Docker 基础架构并保护容器和数据免受恶意攻击的方法。
<a name="Jq0dl"></a>
## 介绍
随着许多公司在其基础设施中采用 Docker，威胁参与者的攻击面也增加了。这就需要保护 Docker 基础设施。在本文中，提到了一些可以加强 Docker 容器安全性的要点。<br />要充分利用本文，必须具备以下条件：

- **熟悉 Linux 命令行**
- **关于容器化和 Docker 的基本概念**
<a name="RHb9o"></a>
## 什么是 Docker？
Docker 是一个开源容器化平台。它允许开发人员将应用程序打包到容器中：标准化的可执行组件将应用程序源代码与在运行该代码所需的操作系统 (OS) 库和依赖项相结合。
<a name="QCYGi"></a>
## 十大最佳实践
Docker 文档概述了在保护 Docker 容器时要考虑的四个主要方面：<br />[https://docs.docker.com/engine/security/](https://docs.docker.com/engine/security/)

- **内核对命名空间和 cgroup 的支持**
- **Docker 守护进程的攻击面**
- **容器配置错误**
- **使用 AppArmor、SELinux 等 Linux 内核安全模块等**

将这些分解为可以遵循的 10 大实践来强化 Docker 环境。
<a name="UaJWy"></a>
## 1、经常更新主机和 Docker 守护进程
容器与主机系统共享内核。在容器上下文中执行的任何内核漏洞都会直接影响主机内核。内核提权漏洞 Dirty Cow 在容器中执行时会导致对主机的 root 访问。因此，保持主机和 Docker 引擎最新很重要。
<a name="IJuLb"></a>
## 2、不要暴露 Docker daemon socket
Docker 客户端和 Docker 守护程序之间发生的所有通信都通过 Docker 守护程序套接字进行，这是一个 UNIX 套接字，通常位于/var/run/docker.sock，这允许访问 Docker API。传统的 UNIX 文件权限用于限制对该套接字的访问。在默认配置中，该套接字由 root 用户拥有。如果其他人获得了对套接字的访问权，将拥有对主机的 root 访问权。

- **设置权限，以便只有 root 用户和 docker 组可以访问 Docker 守护进程套接字**
- **使用 SSH 保护 Docker 守护进程套接字**
- **使用 TLS (HTTPS) 保护 Docker 守护程序套接字。这允许通过 HTTP 以安全的方式访问 Docker**
- **不要让守护程序套接字可用于远程连接，除非您使用 Docker 的加密 HTTPS 套接字，它支持身份验证**
- **不要使用类似的选项运行 Docker 镜像**`**-v /var/run/docker.sock:/var/run/docker.sock**`**，这会在生成的容器中公开套接字。请记住，以只读方式安装套接字不是解决方案，只会使其更难被破坏。docker compose 文件中的一个例子是**
```yaml
volumes:
	- "/var/run/docker.sock:/var/run/docker.sock"
```
要检查是否已经有一个在这种配置中运行的容器：
```bash
docker inspect --format='{{.HostConfig.Binds}}' [container id]
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502558-9f021e29-9a00-45c0-b61f-d5030e0e096a.png#clientId=ub5bddca0-8b83-4&from=paste&id=u9386a408&originHeight=349&originWidth=1039&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3cb0aef3-4b75-469d-83aa-46d39f809a4&title=)
<a name="ZTH5z"></a>
## 3、以无 root 模式运行 Docker
可以以非 root 用户身份运行 Docker 守护程序，以防止 Docker 中的潜在漏洞。这称为“无 root 模式”。无 root 模式不需要 root 权限安装 Docker 与 Docker API 通信。<br />在无 root 模式下，Docker 守护进程和容器在用户命名空间中运行，默认情况下没有 root 权限。
<a name="svSP2"></a>
### 在无 root 模式下运行 Docker

- **使用 sudo 权限安装uidmap软件包：**
```bash
apt-get install -y uidmap
```

- **从 Docker 的网站获取安装脚本并运行：**
```bash
curl -fSsL https://get.docker.com/rootless | sh
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502567-a731dd8d-2ad8-4c72-a081-0413a556cc4c.png#clientId=ub5bddca0-8b83-4&from=paste&id=u7056db25&originHeight=454&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufcfc46ef-0cff-42b6-a280-b22ef383b11&title=)<br />获取无 root 安装脚本<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502688-b47a3886-a1d7-4ba9-9881-b1d47637459c.png#clientId=ub5bddca0-8b83-4&from=paste&id=u3238166a&originHeight=568&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9edac25e-6d91-4721-9239-bd210070408&title=)

- **复制以开头的最后两行**`**export**`**并将它们粘贴到**`**~/.bashrc**`**文件的末尾。这样可以确保每次打开 Bash shell 时，都会设置这两个变量：PATH 和DOCKER_HOST。**

![.bashrc](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502626-010e8ceb-625d-49d2-a03b-7d0b79ab6c04.png#clientId=ub5bddca0-8b83-4&from=paste&id=u8a031e59&originHeight=90&originWidth=491&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2dbd1c8e-c351-4fc5-91b8-dcbcd1729f8&title=.bashrc ".bashrc")

- **运行**`**source ~/.bashrc**`**以在当前的 shell 会话中设置这些变量。**
- **运行**`**systemctl --user start docker**`**以启动 Docker 引擎。**
- **可以通过运行来检查 docker 是否正在运行**`**docker version**`

![版本输出](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502580-c241ada0-afab-4655-b631-e96129d19b34.png#clientId=ub5bddca0-8b83-4&from=paste&id=ue9f4cb55&originHeight=523&originWidth=597&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u09e69237-e380-4c1a-891c-ef6400c0e66&title=%E7%89%88%E6%9C%AC%E8%BE%93%E5%87%BA "版本输出")
<a name="PRBMU"></a>
## 4、容器资源配置
控制组或 cgroups 是 Linux 内核功能，在实现容器的资源分配和限制方面起着关键作用。他们的工作不仅是确保每个容器获得其公平份额的资源，如内存和 CPU，而且还要确保单个容器不会因耗尽其中一个资源而导致系统崩溃。<br />限制资源可防止拒绝服务攻击。以下是一些可用于限制容器资源的 CLI 标志：

- `**--memory=<memory size>**`**— 最大内存量**
- `**--restart=on-failure:<number_of_restarts>**`** — 重启次数**
- `**--memory-swap <value>**`**— 交换内存量**
- `**--cpus=<number>**`**— 容器可用的最大 CPU 资源**
- `**--ulimit nofile=<number>**`**— 文件描述符的最大数量**
- `**--ulimit nproc=<number>**`**— 最大进程数**

默认情况下，Docker 允许容器使用主机内核允许的尽可能多的 RAM 和 CPU 资源。因此有必要设置资源约束以防止容器和主机中的安全问题。
<a name="WtYBp"></a>
## 5、避免使用特权容器
<a name="KfXRO"></a>
### 避免使用 `--privileged` 标志
Docker 具有允许容器在主机上以 root 权限运行的功能。这是通过`--privileged`标志完成的。以特权模式运行的容器对主机上的所有设备都具有 root 权限。<br />如果攻击者要破坏特权容器，他们就有可能轻松访问主机上的资源。篡改系统中的安全模块（如 SELinux）也很容易。因此，不建议在开发生命周期的任何阶段以特权模式运行容器。<br />特权容器是主要的安全风险。滥用的可能性是无穷无尽的。攻击者可以识别主机上运行的服务来发现和利用漏洞。他们还可以利用容器错误配置，例如具有弱凭据或没有身份验证的容器。特权容器为攻击者提供 root 访问权限，从而导致执行恶意代码。避免在任何环境中使用它们。<br />要检查容器是否在特权模式下运行，请使用以下命令：
```bash
docker inspect --format='{{.HostConfig.Privileged}}' [container_id]
```

- **true意味着容器是特权的**
- **false表示容器没有特权**
<a name="C58tT"></a>
### 使用 no-new-privileges 选项
在创建容器时添加no-new-privileges安全选项，以禁止容器进程使用setuid或setgid二进制文件提升其权限。这可以防止容器内的进程在执行期间获得新的权限。因此，如果有一个设置了 setuid 或 setgid 位的程序，任何试图通过该程序获得特权的操作都将被拒绝。
<a name="xE5f4"></a>
## 6、将文件系统和卷设置为只读
Docker 中一个具有安全意识的有用功能是使用只读文件系统运行容器。这减少了攻击向量，因为容器的文件系统不能被篡改或写入，除非它对其文件系统文件和目录具有明确的读写权限。<br />以下代码将 Docker 容器设置为只读：
```bash
docker run --read-only alpine sh -c 'echo "read only" > /tmp'
```
<a name="YX32s"></a>
## 7、Drop capabilities
Linux 内核能够将 root 用户的权限分解为不同的单元，称为 capabilities。几乎所有与 Linux root 用户相关的特殊权限都分解为单独的 capabilities。<br />![capsh 显示的特权容器的capabilities](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052502912-29ed989c-532c-499b-87ab-e45270662e17.png#clientId=ub5bddca0-8b83-4&from=paste&id=u5defbaa6&originHeight=176&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u9102068d-3556-45c6-ba2f-73fd58ccf89&title=capsh%20%E6%98%BE%E7%A4%BA%E7%9A%84%E7%89%B9%E6%9D%83%E5%AE%B9%E5%99%A8%E7%9A%84capabilities "capsh 显示的特权容器的capabilities")<br />Docker 施加了某些限制，使得使用功能变得更加简单。文件功能存储在文件的扩展属性中，并且在构建 Docker 镜像时会去除扩展属性。这意味着您通常不必过多关注容器中的文件功能。<br />正如之前提到的，记住不要运行带有`--privileged`标志的容器，因为这会将所有 Linux 内核功能添加到容器中。<br />最安全的设置是使用`--cap-drop all` 删除所有功能，然后仅添加所需的功能。例如：
```bash
docker run --cap-drop all --cap-add CHOWN alpine
```
<a name="gyrVt"></a>
## 8、使用 Linux 安全模块
考虑使用像 seccomp 或 AppArmor 这样的安全模块。以下是一些众所周知的模块：

- **Seccomp：用于允许/禁止在容器中运行的系统调用**
- **AppArmor：使用程序配置文件来限制单个程序的功能**
- **SELinux：使用安全策略，这是一组规则，告诉 SELinux 什么可以访问或不能访问，以强制执行策略允许的访问。**

这些安全模块可用于为进程和用户的访问权限提供另一个级别的安全检查，超出标准文件级访问控制所提供的安全检查。
<a name="al572"></a>
### seccomp
默认情况下，容器获取默认的 seccomp 配置文件。<br />[https://github.com/moby/moby/blob/master/profiles/seccomp/default.json](https://github.com/moby/moby/blob/master/profiles/seccomp/default.json)<br />这可以用以下命令覆盖：
```bash
docker run --rm -it --security-opt seccomp=./seccomp/profile.json hello-world
```
![使用 Seccomp 配置文件运行容器](https://cdn.nlark.com/yuque/0/2022/png/396745/1658052503102-fe438537-4b9f-464f-8de4-f18f37e4e3ca.png#clientId=ub5bddca0-8b83-4&from=paste&id=ue259fca0&originHeight=274&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u7d5f8d8d-45f7-44bc-b4e9-4af2bc9beca&title=%E4%BD%BF%E7%94%A8%20Seccomp%20%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%BF%90%E8%A1%8C%E5%AE%B9%E5%99%A8 "使用 Seccomp 配置文件运行容器")<br />使用 Seccomp 配置文件，可以选择容器中允许哪些系统调用以及拒绝哪些系统调用，因为在生产环境中并非全部都需要。您可以从 Docker 文档中了解有关编写 seccomp 配置文件的更多信息。
<a name="nRxRH"></a>
### AppArmor
默认情况下，容器使用docker-default AppArmor 模板。可以使用`--security-opt`自定义配置文件覆盖默认设置。<br />为此，必须首先将新配置文件加载到 AppArmor 中以与容器一起使用
```bash
apparmor_parser -r -W /path/to/custom_profile
```
现在使用自定义配置文件运行容器
```bash
docker run --rm -it --security-opt apparmor=custom_profile hello-world
```
请参阅此 wiki 以了解如何创建 AppArmor 配置文件。<br />[https://gitlab.com/apparmor/apparmor/-/wikis/QuickProfileLanguage](https://gitlab.com/apparmor/apparmor/-/wikis/QuickProfileLanguage)
<a name="CV64D"></a>
## 9、设置容器的用户
防止提权攻击的一种简单方法是将容器的用户设置为非特权用户。如果容器被入侵，攻击者将没有足够的权限对容器发起攻击。有多种方法可以为容器设置用户：

- **运行容器时使用**`**-u**`**标志：**
```bash
docker run -u 1001 nobody
```

- **在 Docker 守护程序中启用用户命名空间支持 ( **`**--userns-remap=default**`**)**
- **在构建镜像时 Dockerfile 中使用 USER指令：**
```dockerfile
FROM ubuntu:latest
RUN apt-get -y update
RUN groupadd -r john && useradd -r -g john john
USER john
```
但是，重要的是要注意，如果容器内存在本地权限升级，则此假设不成立。
<a name="e9aHU"></a>
## 10、将日志记录级别至少设置为 INFO
维护日志信息很重要，因为它有助于在运行时解决任何潜在问题。默认情况下，Docker 守护程序配置为具有日志记录级别`info`，如果不是这种情况，可以使用该`--log-level info`选项进行设置。这是一个基本日志级别，捕获除调试日志之外的所有日志。除非需要，否则不建议使用`debug`日志级别。<br />在 docker-compose 中配置日志级别：
```bash
docker-compose --log-level info up
```
<a name="gcA48"></a>
## 参考

- [https://cheatsheetseries.owasp.org/cheatsheets/Docker_Security_Cheat_Sheet.html](https://cheatsheetseries.owasp.org/cheatsheets/Docker_Security_Cheat_Sheet.html)
- [https://blog.aquasec.com/docker-security-best-practices](https://blog.aquasec.com/docker-security-best-practices)
- [https://www.techtarget.com/searchitoperations/feature/How-to-harden-Docker-images-to-enhance-security](https://www.techtarget.com/searchitoperations/feature/How-to-harden-Docker-images-to-enhance-security)
- [**https://docs.docker.com/engine/security/**](https://docs.docker.com/engine/security/)

_原文：_[_https://blog.appsecco.com/top-10-docker-hardening-best-practices-f16c090e4d59_](https://blog.appsecco.com/top-10-docker-hardening-best-practices-f16c090e4d59)
