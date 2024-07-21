PodMan
<a name="SbQga"></a>
## 环境
Centos8
<a name="Z2nbl"></a>
## 安装podman
Podman 是一个容器环境，首先在主机上安装 Podman。执行下面命令来安装podman：
```bash
[root@localhost ~]# yum -y install podman
```
然后修改一下用户命名空间的大小：
```bash
[root@localhost ~]# echo "user.max_user_namespaces=28633" >> /etc/sysctl.d/userns.conf
[root@localhost ~]# sysctl -p /etc/sysctl.d/userns.conf
user.max_user_namespaces = 28633
```
下面创建一个podman容器来看一下吧，下面使用的是RHEL的UBI镜像：
```bash
[root@localhost ~]# podman run ubi8/ubi cat /etc/os-release
Resolved "ubi8/ubi" as an alias (/etc/containers/registries.conf.d/001-rhel-shortnames.conf)
Trying to pull registry.access.redhat.com/ubi8/ubi:latest...
Getting image source signatures
Checking if image destination supports signatures
Copying blob ce3c6836540f done  
Copying blob 63f9f4c31162 done  
Copying config cc06568478 done  
Writing manifest to image destination
Storing signatures
NAME="Red Hat Enterprise Linux"
VERSION="8.5 (Ootpa)"
ID="rhel"
ID_LIKE="fedora"
VERSION_ID="8.5"
PLATFORM_ID="platform:el8"
PRETTY_NAME="Red Hat Enterprise Linux 8.5 (Ootpa)"
ANSI_COLOR="0;31"
CPE_NAME="cpe:/o:redhat:enterprise_linux:8::baseos"
HOME_URL="https://www.redhat.com/"
DOCUMENTATION_URL="https://access.redhat.com/documentation/red_hat_enterprise_linux/8/"
BUG_REPORT_URL="https://bugzilla.redhat.com/"

REDHAT_BUGZILLA_PRODUCT="Red Hat Enterprise Linux 8"
REDHAT_BUGZILLA_PRODUCT_VERSION=8.5
REDHAT_SUPPORT_PRODUCT="Red Hat Enterprise Linux"
REDHAT_SUPPORT_PRODUCT_VERSION="8.5"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262272-20df9e65-400a-4f3f-82ab-e40d71c7217c.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=u859539d9&originHeight=434&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4805a59d-c7fe-462e-a5e2-f06f35f764c&title=)
<a name="qOytd"></a>
## 创建Dockerfile
现在可以创建一个 Dockerfile 来指定如何构建新的镜像。首先需要为 Dockerfile 文件创建一个目录：
```bash
[root@localhost ~]# mkdir ~/myc
[root@localhost ~]# cd ~/myc
```
创建一个文件Dockerfile文件，来建立一个新镜像：
```bash
[root@localhost myc]# vim Dockerfile

FROM ubi8/ubi:latest
RUN dnf install -y nano
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262589-c22140b1-e20a-467a-bae2-a6166b58a71d.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=ud9b28a61&originHeight=82&originWidth=358&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufdf8e305-a2fe-45d4-b576-cd83f1d2b2d&title=)<br />开始使用`podman build`来创建容器：
```bash
[root@localhost myc]# podman build -f Dockerfile -t ubi-with-nano
[root@localhost myc]# podman build -f Dockerfile -t ubi-with-nano
STEP 1/2: FROM ubi8/ubi:latest
STEP 2/2: RUN dnf install -y nano
Updating Subscription Management repositories.
Unable to read consumer identity
...
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262706-4c513dec-778c-4d34-a7e6-0828e6107904.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=u77f66504&originHeight=438&originWidth=850&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue6c44c0d-e150-4515-9346-3f95ac1e9fb&title=)<br />使用`podman images`来确认是否创建新镜像：
```bash
[root@localhost myc]# podman images
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262936-6f99b2e9-db99-4a85-ace3-ab3ca48b0009.png#averageHue=%23040101&clientId=u05115a38-7f85-4&from=paste&id=u9ff3e8d1&originHeight=115&originWidth=774&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u03bb5807-0a03-49e6-8d4e-6aa02eaae4b&title=)<br />现在可以运行容器了，检查nano编辑器是否可用：
```bash
[root@localhost myc]# podman run localhost/ubi-with-nano /usr/bin/which nano
/usr/bin/nano
```
通过查看nano执行文件的位置，来检查是否安装。<br />Nano 现在已安装在自定义容器中。还可以交互式的运行容器：
```
[root@localhost myc]# podman run -it localhost/ubi-with-nano /bin/bash
[root@d1f0e46f2b6d /]# ls
bin   dev  home  lib64      media  opt   root sbin  sys  usr
boot  etc  lib  lost+found  mnt    proc  run srv   tmp  var
[root@d1f0e46f2b6d /]#
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262967-ad24cee8-ba98-4be6-b4cb-b113ff88fd8d.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=u6ec102cc&originHeight=98&originWidth=666&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u43ab5085-cf09-4f15-9464-c5c26e2a832&title=)<br />在容器中运行exit来退出容器。<br />可以使用`podman ps`来查看运行的容器，如果需要查看以停止的容器，可以添加`-a`选项：
```bash
[root@localhost myc]# podman ps 
[root@localhost myc]# podman ps -a
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041262969-73412d6c-3625-4255-94f4-d3af8de39ff0.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=u4de8c487&originHeight=384&originWidth=896&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3f2f27ec-83cb-4081-98cd-62ac2db5023&title=)
<a name="hECVl"></a>
## 存储
经常让新用户感到困惑的一件事是它们的短暂性。例如进入容器里创建的文件，退出之后，再次进入发现文件没有了。下面将容器中需要存储永久性文件的文件夹挂载到系统的某个文件夹中。下面在本机创建一个存储位置：
```bash
[root@localhost ~]# mkdir /pod_data
```
然后使用您的存储目录作为一些相关的挂载点来启动容器。此示例将本地目录/pod_data绑定到容器中名为/storage 的位置，必须在目录位置附加 :Z，以便 SELinux 可以在主机和 Podman 之间转换上下文。
```bash
[root@localhost ~]# podman run -it --volume /pod_data:/storage:Z localhost/ubi-with-nano
[root@d590bc344b76 /]# echo "hello podman" >> /storage/msg.txt
[root@d590bc344b76 /]# exit
exit
[root@localhost ~]# cat /pod_data/msg.txt 
hello podman
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668041263466-30cfa8f7-f7f2-4862-ab1c-2f34c7889bfe.png#averageHue=%23000000&clientId=u05115a38-7f85-4&from=paste&id=u88d35f80&originHeight=148&originWidth=766&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4b8dcb51-989a-4a78-9acd-67b345c61b3&title=)<br />可以看到目录绑定之后，在容器中写入数据，退出容器，在本机的/pod_data可以看到写入的内容。
