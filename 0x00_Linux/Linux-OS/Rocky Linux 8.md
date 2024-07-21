Linux<br />对 CentOS 的支持于 2021 年结束，但该项目以 Rocky Linux 的形式继续存在。以下是如何安装和配置 Rocky Linux。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249911716-df3718dd-cc45-4482-acfb-6e7b2c2e9e56.webp#clientId=udc239c8b-bfd7-4&from=paste&id=u362bdd17&originHeight=375&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1fe69687-d82c-44d3-a9e6-6daa07eba37&title=)<br />Rocky Linux 是 CentOS 的创始人 Gregory Kurtzer 创建的 CentOS 克隆。在 RHEL 开发团队宣布对 CentOS 8 的支持于 2021 年 12 月 31 日结束数小时后，Kurtzer 宣布了 Rocky Linux 项目。基于 RHEL 的下游二进制兼容版本于 2021 年 6 月 21 日发布。Kurtzer 以他已故的 CentOS 合作伙伴的名字命名了该发行版-创始人，发推文，“回想早期的 CentOS，我的联合创始人 Rocky McGaugh。他不再和我们在一起，所以作为他的 H/T，他从未见过 CentOS 的成功，我向你介绍 Rocky Linux。”
<a name="JrpFp"></a>
## Rocky Linux 最低要求
以下是安装 Rocky Linux 8.5 的最低要求。

- 2 GB RAM 或更大
- 20 GB 硬盘或更大
- 2 个 CPU / vCPU（1.1 GHz 处理器）
- 可以上 Internet（可选）
- 可引导的媒体（USB 或 DVD）
<a name="eCc1G"></a>
## 下载 Rocky Linux 并创建 USB 驱动器
按照这些说明下载 Rocky Linux 并创建一个可引导的 USB 驱动器。

1. 打开浏览器并导航到[**rockylinux.org/download**](https://rockylinux.org/download)。
2. 单击**保存文件**。
3. 下载文件后，从机器创建 Rocky Linux 可引导 USB。如果不确定如何执行此操作，请参见下文：
   1. [**创建 Windows 11 可启动 USB 驱动器的方法**](https://www.makeuseof.com/windows-11-create-bootable-usb-drive/)
   2. [**如何在 Linux 中使用 Etcher 制作可启动的 USB 驱动器**](https://www.makeuseof.com/create-bootable-usb-drive-with-etcher-linux/)
   3. [**如何在 Mac 上创建和引导 Linux USB 驱动器**](https://www.makeuseof.com/tag/how-to-boot-a-linux-live-usb-stick-on-your-mac/)

创建安装媒体后，就可以继续了。
<a name="LMZvz"></a>
## Rocky Linux 安装说明
按照这些说明成功安装 Rocky Linux 8.5

1. 将创建的 Rocky Linux 可启动 USB 插入目标 PC/笔记本电脑。
2. 启动或重新启动计算机以从 USB 驱动器启动。看到 Rocky Linux 启动屏幕后，选择**Install Rocky Linux**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249911766-368c5d21-3392-412d-99c7-c2e9bf914ee0.webp#clientId=udc239c8b-bfd7-4&from=paste&id=ua4088458&originHeight=480&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6e65dec9-59ce-4efc-a37b-4df0610db6c&title=)

1. 出现提示时单击首选语言，然后单击**继续**。
2. 在 Installation Summary 窗口中，选择**Installation Destination**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249911758-fa04c3c0-2da7-4edc-88cf-8e8dda529fdc.webp#clientId=udc239c8b-bfd7-4&from=paste&id=u696ad7f2&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u29473e95-74b1-4b5f-b5ca-b6e19d492b0&title=)选择目标硬盘并单击**完成**。

1. 选择**网络**和**主机名**并连接到网络连接。单击**完成**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249911795-a0eda263-d005-44a3-a687-9da726783c1b.webp#clientId=udc239c8b-bfd7-4&from=paste&id=uc95e9a5d&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u53bc345a-7552-4019-b683-fdcfde7dfac&title=)

1. 选择**安装源**并选择 Rocky Linux 可引导 USB（如果尚未选择）。
2. 选择**软件选择**并选择所需的基本环境和附加软件。对选择感到满意时单击**完成。**

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249911774-edb66dc0-33cb-44df-9c4b-f7e2b25ad660.webp#clientId=udc239c8b-bfd7-4&from=paste&id=ue0e47642&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u42eb4b98-4531-4c52-9f8d-58d8e1acaaa&title=)

1. 从“**安装摘要**”窗口中选择**Root 密码。输入并验证想要的密码。单击完成**。
2. 从“**安装摘要**”窗口中选择**创建用户。输入想要的响应。单击完成**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912115-e5fc249d-f020-4270-8492-50c665b04806.webp#clientId=udc239c8b-bfd7-4&from=paste&id=ucc8a6967&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud20308e5-999e-40c1-ba89-452ff4a98d9&title=)

1. 在笔记本电脑/PC 上安装 Rocky Linux 时，请耐心等待。
2. 安装完成后，点击**重启系统**。出现提示时移除 Rocky Linux 可启动 USB 驱动器。按 **<Enter>**
3. 系统重新启动后，单击**Initial Setup**窗口中的**Licensing Information**
4. 阅读**许可协议**，滚动到底部，然后选中**我接受许可协议**。单击**完成。**

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912115-f32247a6-9450-408c-8da6-4524c4383220.webp#clientId=udc239c8b-bfd7-4&from=paste&id=u8056b79c&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0ffde865-21cd-483b-9058-1f1c627ecc2&title=)

1. 单击**完成配置**。
<a name="HPxbf"></a>
## Rocky Linux 安装后程序
在 Rocky Linux 首次启动后，需要完成一些配置步骤。

1. 使用之前创建的用户（和密码）登录到 Rocky Linux 安装。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912163-8472fd0d-bfbc-4f1b-a5e0-1827064c6367.webp#clientId=udc239c8b-bfd7-4&from=paste&id=u3c891d4c&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a966276-f4a2-4d18-878a-02bf322c76c&title=)

1. 系统重新启动后，单击**Initial Setup**窗口中的**Licensing Information**
2. 阅读**许可协议**，滚动到底部，然后选中**我接受许可协议**。单击**完成**。
3. 单击**完成配置**。
4. 登录到 Rocky Linux 安装。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912225-89bc4909-520f-4fb8-a5bf-2d6bf48b648c.webp#clientId=udc239c8b-bfd7-4&from=paste&id=u3e351904&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8bd0e536-c742-46a4-8c91-bf9d51b5cda&title=)

1. **在欢迎**屏幕上选择想要的语言。单击**下一步**。
2. 重复**键入**屏幕。单击**下一步**。
3. 在“**隐私**”屏幕上，选择是否要打开或关闭定位服务。单击**下一步**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912222-a8da731c-1f9f-4b95-84ca-963e62b5a635.webp#clientId=udc239c8b-bfd7-4&from=paste&id=ud68f7290&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubfdd16be-531a-43e0-bdb6-0b243a22219&title=)

1. 选择要连接的在线帐户并输入凭据，或单击**跳过**。
2. 单击**开始使用 Rocky Linux**。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645249912482-e0347ddf-b666-43e2-b6b2-86f02ad023aa.webp#clientId=udc239c8b-bfd7-4&from=paste&id=ufdf9e08f&originHeight=563&originWidth=750&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc5b380cd-1a51-47b2-91d6-8995e93bd46&title=)

1. 在笔记本电脑/PC 上成功安装 Rocky Linux 8！

每当安装新发行版时，首要任务是更新系统以确保应用任何安全更新以及操作系统更新上的软件。启动终端并使用以下命令更新存储库（如果出现提示，请输入 root 密码）：
```bash
# sudo yum check-update
```
`**yum check-update**`命令完成后，输入以下命令来更新操作系统和所有已安装的软件：
```bash
# sudo yum update
```
`**sudo yum update**`命令完成后，使用以下命令退出终端：
```bash
# exit
```
<a name="H3uzT"></a>
## 提高 Rocky Linux 8.5 的 DNF 速度
Dandified YUM，更广为人知的 DNF，是一个软件包管理器，用于安装、更新和删除软件包的基于 RPM 的 Linux 发行版。它最初是在 Fedora 18 中以可测试状态（即技术预览）引入的，但自 Fedora 22 以来它一直是 Fedora 的默认包管理器。Rocky Linux 中的包管理器还有 RPM（Red Hat 包管理器）。包管理器允许 Linux 用户安装、更新和删除软件。<br />安装并更新和升级新的 Rocky 8.5 安装后，建议提高 DNF 速度。首先备份**dnf.conf**文件。
```bash
# sudo cp /etc/dnf/dnf.conf /etc/dnf/dnf.bak
```
接下来，编辑**dnf.conf**文件：
```bash
# sudo nano /etc/dnf/dnf.conf
```
**现在，可以通过将以下内容添加到 dnf.conf**文件的底部来添加并行下载，从而提高 DNF 速度：
```bash
max_parallel_downloads=10
```
请注意，建议用户从 10 开始设置，但可以将其增加到不同的数量，例如 15、20 等。建议谨慎执行此操作。**要启用最快的镜像，请在 **`**max_parallel_downloads=10**`下方添加此行。
```bash
fastestmirror=True
```
保存更新的/etc/dnf.conf文件，然后退出 nano。<br />需要重新启动或重新启动任何服务，因为这些更改会立即生效。
<a name="kVGZx"></a>
## 迁移到 Rocky Linux 8.5
现有 CentOS 8.5、Alma Linux 8.5、RHEL 8.5 或 Oracle Linux 8.5 用户可以使用以下过程轻松迁移到 Rocky Linux。<br />首先，下载 Rocky Linux 迁移脚本。最安全的方法是通过 git 下载迁移脚本。<br />输入终端。输入以下命令安装 git（如果提示输入 root 密码）：
```bash
# sudo dnf install git
```
接下来，使用以下命令克隆 rocky-tools 存储库：
```bash
# sudo git clone https://github.com/rocky-linux/rocky-tools.git
```
还可以通过 curl 命令下载迁移脚本（尽管这是一种不太安全的方法）：
```bash
# sudo curl https://raw.githubusercontent.com/rocky-linux/rocky-tools/main/migrate2rocky/migrate2rocky.sh -o migrate2rocky.sh
```
现在有了迁移脚本，可以执行它并开始迁移。但是，首先，必须通过以下命令授予脚本文件的所有者执行权限：
```bash
# sudo chmod u+x migrate2rocky.sh
```
最后，将执行脚本：
```bash
# sudo ./migrate2rocky.sh -r
```
（请注意，`'-r'`选项指示脚本安装所有内容。请耐心等待迁移脚本完成。完成后，退出终端。
```bash
# exit
```
<a name="MOtr3"></a>
## Rocky Linux 是一个坚如磐石的发行版
Rocky Linux 对 Linux 爱好者来说是天赐之物，他们为 CentOS 8.0 的消失而悲痛，却又不太喜欢 Fedora。该发行版是最可靠的介绍性发行版。切换到 Rocky Linux 的 CentOS 用户会对发行版熟悉的外观和感觉感到惊喜。新的 Linux 用户在导航和遍历 Rocky Linux 时也不会遇到什么问题。<br />Linux 社区的反应非常积极并支持该发行版。Rocky Linux 发布后不久，创建了一个 subreddit，[**/r/RockyLinux**](https://www.reddit.com/r/RockyLinux)，现在拥有近 6000 名用户。
<a name="W4XjF"></a>
## 参考资料
**什么是 Rocky Linux: **[https://www.makeuseof.com/what-is-rocky-linux/](https://www.makeuseof.com/what-is-rocky-linux/)<br />**rocky download: **[https://rockylinux.org/download](https://rockylinux.org/download)<br />**创建 Win11 可启动 USB 驱动器的方法: **[https://www.makeuseof.com/windows-11-create-bootable-usb-drive/](https://www.makeuseof.com/windows-11-create-bootable-usb-drive/)<br />**Linux 中使用 Etcher 制作可启动的 USB 驱动器: **[https://www.makeuseof.com/create-bootable-usb-drive-with-etcher-linux/](https://www.makeuseof.com/create-bootable-usb-drive-with-etcher-linux/)<br />**Mac 上创建和引导 Linux USB 驱动器: **[https://www.makeuseof.com/tag/how-to-boot-a-linux-live-usb-stick-on-your-mac/](https://www.makeuseof.com/tag/how-to-boot-a-linux-live-usb-stick-on-your-mac/)<br />**/r/RockyLinux: **[https://www.reddit.com/r/RockyLinux](https://www.reddit.com/r/RockyLinux)
