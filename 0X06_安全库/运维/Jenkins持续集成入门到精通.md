<a name="5d5f27c1"></a>
# 1、gitlab安装和部署
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603351982979-18d8b618-2010-4c05-b501-9a4ae1dcd8f9.png#align=left&display=inline&height=194&originHeight=387&originWidth=1305&size=35482&status=done&style=none&width=652.5)

1）首先，开发人员每天进行代码提交，提交到Git仓库<br />2）然后，Jenkins作为持续集成工具，使用Git工具到Git仓库拉取代码到集成服务器，再配合JDK，Maven等软件完成代码编译，代码测试与审查，测试，打包等工作，在这个过程中每一步出错，都重新再执行一次整个流程。<br />3）最后，Jenkins把生成的jar或war包分发到测试服务器或者生产服务器，测试人员或用户就可以访问应用。
<a name="7z8Ca"></a>
## a、服务器列表
| **名称** | **IP地址** | **安装的软件** |
| --- | --- | --- |
| 代码托管服务器 | 10.1.1.11 | Gitlab-12.4.2 |
| 持续集成服务器 | 10.1.1.12 | Jenkins-2.190.3，JDK1.8，Maven3.6.2，Git，SonarQube |
| 应用测试服务器 | 10.1.1.13 | JDK1.8，Tomcat8.5 |

<a name="i6TKY"></a>
## b、Gitlab代码托管服务器安装
**Gitlab简介**<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603352045422-9a34320d-7539-4901-be6f-a4e896298e64.png#align=left&display=inline&height=124&originHeight=247&originWidth=873&size=77816&status=done&style=none&width=436.5)<br />官网：[https://about.gitlab.com/](https://about.gitlab.com/)<br />GitLab 是一个用于仓库管理系统的开源项目，使用Git作为代码管理工具，并在此基础上搭建起来的web服务。<br />GitLab和GitHub一样属于第三方基于Git开发的作品，免费且开源（基于MIT协议），与Github类似，可以注册用户，任意提交你的代码，添加SSHKey等等。不同的是，**GitLab是可以部署到自己的服务器上，数据库等一切信息都掌握在自己手上，适合团队内部协作开发，**你总不可能把团队内部的智慧总放在别人的服务器上吧？简单来说可把GitLab看作个人版的GitHub。

**安装**

1. 安装相关依赖
```c
[root@gitlab ~]# yum -y install policycoreutils openssh-server openssh-clients postfix
```

2. 启动ssh服务&设置为开机启动
```c
[root@gitlab ~]# systemctl enable sshd && sudo systemctl start sshd
```

3. 设置postfix开机自启，并启动，postfix支持gitlab发信功能systemctl enable postfix && systemctl start postfix
```c
[root@gitlab ~]# systemctl enable postfix && systemctl start postfix
```

4. 开放ssh以及http服务，然后重新加载防火墙列表
```c
[root@gitlab ~]# firewall-cmd --add-service=ssh --permanent
[root@gitlab ~]# firewall-cmd --add-service=http --permanent
[root@gitlab ~]# firewall-cmd --reload
```

5. 下载gitlab包，并且安装

在线下载安装包：
```c
[root@gitlab ~]# wget https://mirrors.tuna.tsinghua.edu.cn/gitlab-ce/yum/el6/gitlab-ce-12.4.2-ce.0.el6.x86_64.rpm
[root@gitlab ~]# rpm -i gitlab-ce-12.4.2-ce.0.el6.x86_64.rpm
```

6. 修改gitlab配置
```c
[root@gitlab ~]#  vi /etc/gitlab/gitlab.rb
修改gitlab访问地址和端口，默认为80，我们改为82
external_url 'http://192.168.66.100:82'
nginx['listen_port'] = 82
```

7. 重载配置及启动gitlab
```c
[root@gitlab ~]#  gitlab-ctl reconfigure
[root@gitlab ~]#  gitlab-ctl restart
```
[ gitlab修改用户密码     ](https://www.cnblogs.com/xiaobaoTribe/p/10708871.html)<br />首先ssh登录你的gitlab服务器，并执行下面命令
```
gitlab-rails console production
```
直到重新加载好控制台，这里有多种方法来查找你的root用户
```
user = User.where(id: 1).first
id在用户资料中能看到具体的
```
或者执行下面都行
```
user = User.find_by(email: 'admin@local.host')
```
```
user = User.find_by(name: 'admin')
```
执行完上面的命令后，现在可以修改密码了，如下命令
```
user.password = 'secret_pass'
user.password_confirmation = 'secret_pass'
```
修改完后要记得保存，请执行下面命令
```
user.save!
```


启动成功后，看到以下修改管理员root密码的页面，修改密码后，然后登录即可<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603352640067-e4f608af-ecc6-48f8-9f3b-229222cf0103.png#align=left&display=inline&height=426&originHeight=852&originWidth=1748&size=91173&status=done&style=none&width=874)
<a name="vdopv"></a>
## c、创建组、用户、项目

创建组<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603371796931-05f99a98-20f5-483a-b7c4-448ea76f3d66.png#align=left&display=inline&height=419&originHeight=838&originWidth=850&size=50782&status=done&style=none&width=425)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603371863145-03d106f3-fa70-4503-8063-65c771a52666.png#align=left&display=inline&height=352&originHeight=703&originWidth=1072&size=54996&status=done&style=none&width=536)<br />创建用户<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603371931255-51ceefcd-3492-4607-9012-c9db16fee8f4.png#align=left&display=inline&height=230&originHeight=460&originWidth=1920&size=73123&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372286640-8666d3a4-4340-4e17-acea-8da1da7cb522.png#align=left&display=inline&height=159&originHeight=318&originWidth=1433&size=18712&status=done&style=none&width=716.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372299552-600e6bff-4fc9-45fa-bd41-025239b41e5e.png#align=left&display=inline&height=270&originHeight=540&originWidth=1243&size=26133&status=done&style=none&width=621.5)<br />创建密码<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372335751-3353cca4-9eb2-43de-9d72-b4b5e966cf57.png#align=left&display=inline&height=143&originHeight=286&originWidth=1600&size=24775&status=done&style=none&width=800)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372397331-98dbd9e1-3721-42cf-87df-da97b743992b.png#align=left&display=inline&height=282&originHeight=564&originWidth=1623&size=36908&status=done&style=none&width=811.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372437363-f45dc8cb-f876-4e72-a6c4-a3677002a4e2.png#align=left&display=inline&height=80&originHeight=159&originWidth=1628&size=13159&status=done&style=none&width=814)<br />查看组<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372503108-753b6d87-387a-4c61-9232-e3a7e3d91dd0.png#align=left&display=inline&height=138&originHeight=275&originWidth=1920&size=39040&status=done&style=none&width=960)<br />组里面添加新的成员<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372641040-ceb01603-0da9-4892-8d84-5ac3052589d0.png#align=left&display=inline&height=230&originHeight=459&originWidth=1918&size=54453&status=done&style=none&width=959)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372672136-93702a10-628b-4f4a-a27b-a896afa649b7.png#align=left&display=inline&height=208&originHeight=416&originWidth=1920&size=57261&status=done&style=none&width=960)<br />Gitlab用户在组里面有5种不同权限：<br />Guest：可以创建issue、发表评论，不能读写版本库<br />Reporter：可以克隆代码，不能提交，QA、PM可以赋予这个权限<br />Developer：可以克隆代码、开发、提交、push，普通开发可以赋予这个权限<br />Maintainer：可以创建项目、添加tag、保护分支、添加项目成员、编辑项目，核心开发可以赋予这个权限<br />Owner：可以设置项目访问权限 - Visibility Level、删除项目、迁移项目、管理组成员，开发组组长可以赋予这个权限

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372849951-b49fa24f-ba55-4a50-acd1-8c756294c759.png#align=left&display=inline&height=158&originHeight=315&originWidth=1617&size=43037&status=done&style=none&width=808.5)<br />新账户登录<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603372987812-6701f502-9bbf-4264-a3cb-8aeb3b056083.png#align=left&display=inline&height=300&originHeight=599&originWidth=1361&size=65717&status=done&style=none&width=680.5)
<a name="pGXuT"></a>
## d、创建一个项目发布到gitlab
视频中使用的是ideaIC-2020.2.3由于不会使用Java代码所使用的是pycharm的开发环境<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603376850958-7d2eed46-bbfa-4904-874f-60fcddfbbac7.png#align=left&display=inline&height=233&originHeight=466&originWidth=1920&size=80775&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603378923027-7913fd93-d094-431e-b3db-72f53c420924.png#align=left&display=inline&height=206&originHeight=412&originWidth=1920&size=59484&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603376476100-06aa38ed-477a-4fcd-954b-517cef16e6c5.png#align=left&display=inline&height=288&originHeight=576&originWidth=1001&size=32066&status=done&style=none&width=500.5)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603376521547-78a36818-b235-47c0-bac7-18ccce4c27dc.png#align=left&display=inline&height=195&originHeight=316&originWidth=411&size=13146&status=done&style=none&width=254)<br />账户密码是root和12345678<br />备注：使用的是初始化的账户和密码要是后面创建的用户登录不上、我也不知道这个是为啥<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603381506285-044d7775-647f-4391-9c72-c184673932bb.png#align=left&display=inline&height=125&originHeight=249&originWidth=1920&size=37261&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603381594427-0c4a0476-8415-47a3-8dbb-a38c5f612f4a.png#align=left&display=inline&height=150&originHeight=300&originWidth=1809&size=45197&status=done&style=none&width=904.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603381630123-0cd47d55-d058-4181-944e-79cfaf006f39.png#align=left&display=inline&height=357&originHeight=713&originWidth=1920&size=115493&status=done&style=none&width=960)
<a name="7lJtd"></a>
# 2、安装部署jenkins
<a name="575255f0"></a>
## a、安装Java环境
```c
[root@jenkins ~]# yum install java-1.8.0-openjdk* -y
[root@jenkins ~]# cd /usr/lib/j
java/        java-1.5.0/  java-1.6.0/  java-1.7.0/  java-1.8.0/  java-ext/    jvm/         jvm-commmon/ jvm-exports/ jvm-private/
[root@jenkins ~]# cd /usr/lib/jvm
[root@jenkins jvm]# ll
total 0
lrwxrwxrwx. 1 root root  26 Oct 22 11:59 java -> /etc/alternatives/java_sdk
drwxr-xr-x. 4 root root  95 Oct  8 10:46 java-1.7.0-openjdk-1.7.0.75-2.5.4.2.el7_0.x86_64
lrwxrwxrwx. 1 root root  32 Oct 22 11:59 java-1.8.0 -> /etc/alternatives/java_sdk_1.8.0
lrwxrwxrwx. 1 root root  40 Oct 22 11:59 java-1.8.0-openjdk -> /etc/alternatives/java_sdk_1.8.0_openjdk
drwxr-xr-x. 9 root root 101 Oct 22 11:59 java-1.8.0-openjdk-1.8.0.262.b10-0.el7_8.x86_64
lrwxrwxrwx. 1 root root  34 Oct 22 11:59 java-openjdk -> /etc/alternatives/java_sdk_openjdk
lrwxrwxrwx. 1 root root  21 Oct 22 11:59 jre -> /etc/alternatives/jre
lrwxrwxrwx. 1 root root  27 Oct  8 10:47 jre-1.7.0 -> /etc/alternatives/jre_1.7.0
lrwxrwxrwx. 1 root root  35 Oct  8 10:47 jre-1.7.0-openjdk -> /etc/alternatives/jre_1.7.0_openjdk
lrwxrwxrwx. 1 root root  52 Oct  8 10:47 jre-1.7.0-openjdk-1.7.0.75-2.5.4.2.el7_0.x86_64 -> java-1.7.0-openjdk-1.7.0.75-2.5.4.2.el7_0.x86_64/jre
lrwxrwxrwx. 1 root root  27 Oct 22 11:59 jre-1.8.0 -> /etc/alternatives/jre_1.8.0
lrwxrwxrwx. 1 root root  35 Oct 22 11:59 jre-1.8.0-openjdk -> /etc/alternatives/jre_1.8.0_openjdk
lrwxrwxrwx. 1 root root  51 Oct 22 11:59 jre-1.8.0-openjdk-1.8.0.262.b10-0.el7_8.x86_64 -> java-1.8.0-openjdk-1.8.0.262.b10-0.el7_8.x86_64/jre
lrwxrwxrwx. 1 root root  29 Oct 22 11:59 jre-openjdk -> /etc/alternatives/jre_openjdk
[root@jenkins jvm]# cd java-1.8.0-openjdk
[root@jenkins java-1.8.0-openjdk]# ll
total 51432
drwxr-xr-x.  2 root root     4096 Oct 22 11:59 bin
drwxr-xr-x. 10 root root     4096 Oct 22 11:59 demo
drwxr-xr-x.  3 root root     4096 Oct 22 11:59 include
drwxr-xr-x.  4 root root       26 Oct 22 11:58 jre
drwxr-xr-x.  3 root root     4096 Oct 22 11:59 lib
drwxr-xr-x. 11 root root     4096 Oct 22 11:59 sample
-rw-r--r--.  1 root root 52641577 Aug  6 12:28 src.zip
drwxr-xr-x.  2 root root     4096 Oct 22 11:59 tapset
[root@jenkins java-1.8.0-openjdk]#

```
<a name="iIi9j"></a>
##  b、下载要安装的RPM软件包
```c
[root@jenkins ~]# wget https://mirrors.tuna.tsinghua.edu.cn/jenkins/redhat/jenkins-2.219-1.1.noarch.rpm
--2020-10-22 12:04:05--  https://mirrors.tuna.tsinghua.edu.cn/jenkins/redhat/jenkins-2.219-1.1.noarch.rpm
Resolving mirrors.tuna.tsinghua.edu.cn (mirrors.tuna.tsinghua.edu.cn)... 101.6.8.193, 2402:f000:1:408:8100::1
Connecting to mirrors.tuna.tsinghua.edu.cn (mirrors.tuna.tsinghua.edu.cn)|101.6.8.193|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 66185373 (63M) [application/x-redhat-package-manager]
Saving to: ‘jenkins-2.219-1.1.noarch.rpm’

100%[============================================================================================================================>] 66,185,373  1.03MB/s   in 63s

2020-10-22 12:05:08 (1.00 MB/s) - ‘jenkins-2.219-1.1.noarch.rpm’ saved [66185373/66185373]

[root@jenkins ~]# rpm -ivh jenkins-2.219-1.1.noarch.rpm
warning: jenkins-2.219-1.1.noarch.rpm: Header V4 DSA/SHA1 Signature, key ID d50582e6: NOKEY
Preparing...                          ################################# [100%]
Updating / installing...
   1:jenkins-2.219-1.1                ################################# [100%]
[root@jenkins ~]#

```
<a name="89nW1"></a>
## c、修改配置文件并且重启
```c
[root@jenkins ~]# cd /etc/sysconfig/
[root@jenkins sysconfig]# egrep "root|8088" jenkins
JENKINS_USER="root"
JENKINS_PORT="8088"
[root@jenkins sysconfig]# systemctl restart jenkins.service
[root@jenkins sysconfig]#

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603383207993-e82bfaac-e22f-4471-a58c-9eacf1c0da27.png#align=left&display=inline&height=487&originHeight=974&originWidth=1715&size=115469&status=done&style=none&width=857.5)
<a name="r8VTu"></a>
## d、查看密码
```c
[root@jenkins sysconfig]# cat /var/lib/jenkins/secrets/initialAdminPassword
ffcaa34c7a804ddc82878430a0ab52d3
[root@jenkins sysconfig]#
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603383419281-95abc794-a7cc-403b-b468-82efcff61d19.png#align=left&display=inline&height=427&originHeight=855&originWidth=1246&size=58138&status=done&style=none&width=623)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603383470249-89b7f158-3eb5-4aa8-a1e3-e14895ce8f74.png#align=left&display=inline&height=294&originHeight=588&originWidth=1916&size=77491&status=done&style=none&width=958)
<a name="M7IVg"></a>
# 3、jenkins插件的安装


Jenkins本身不提供很多功能，我们可以通过使用插件来满足我们的使用。例如从Gitlab拉取代码，使用Maven构建项目等功能需要依靠插件完成。接下来演示如何下载插件。

<a name="WRq7w"></a>
## a、修改Jenkins插件下载地址
Jenkins国外官方插件地址下载速度非常慢，所以可以修改为国内插件地址：Jenkins->Manage Jenkins->Manage Plugins，点击Available<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603383726796-1f63cefa-2b8c-4074-af37-65f4d6d8dca7.png#align=left&display=inline&height=378&originHeight=755&originWidth=1918&size=167056&status=done&style=none&width=959)<br />备注要是进入之后发现的是<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603413465599-81d8cc9b-f38d-4ccb-8735-9fa161558ed8.png#align=left&display=inline&height=208&originHeight=416&originWidth=1237&size=86945&status=done&style=none&width=618.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603413521836-e6cb032b-6c51-4660-b040-fbb0a86633dd.png#align=left&display=inline&height=266&originHeight=532&originWidth=964&size=97597&status=done&style=none&width=482)将https改成http然后重启jenkins服务<br />在页面当中加载所有的插件<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603413616053-97e229af-c470-4ce6-bb8e-30244c69086f.png#align=left&display=inline&height=344&originHeight=688&originWidth=1920&size=102897&status=done&style=none&width=960)
<a name="XrJdC"></a>
## b、修改国内的下载地址
```c
[root@jenkins ~]# cd /var/lib/jenkins/
[root@jenkins jenkins]# ls -l
total 44
-rw-r--r--. 1 root root 1642 Oct 22 20:34 config.xml
-rw-r--r--. 1 root root  155 Oct 22 20:34 hudson.model.UpdateCenter.xml
-rw-------. 1 root root 1712 Oct 22 12:12 identity.key.enc
-rw-r--r--. 1 root root    5 Oct 22 12:17 jenkins.install.InstallUtil.lastExecVersion
-rw-r--r--. 1 root root    5 Oct 22 12:17 jenkins.install.UpgradeWizard.state
-rw-r--r--. 1 root root  179 Oct 22 12:17 jenkins.model.JenkinsLocationConfiguration.xml
-rw-r--r--. 1 root root  171 Oct 22 12:12 jenkins.telemetry.Correlator.xml
drwxr-xr-x. 2 root root    6 Oct 22 12:12 jobs
drwxr-xr-x. 3 root root   18 Oct 22 12:12 logs
-rw-r--r--. 1 root root  907 Oct 22 20:34 nodeMonitors.xml
drwxr-xr-x. 2 root root    6 Oct 22 12:12 nodes
drwxr-xr-x. 2 root root    6 Oct 22 12:12 plugins
-rw-r--r--. 1 root root  129 Oct 22 20:34 queue.xml.bak
-rw-r--r--. 1 root root   64 Oct 22 12:12 secret.key
-rw-r--r--. 1 root root    0 Oct 22 12:12 secret.key.not-so-secret
drwx------. 4 root root 4096 Oct 22 20:30 secrets
drwxr-xr-x. 2 root root   65 Oct 22 20:34 updates
drwxr-xr-x. 2 root root   23 Oct 22 12:12 userContent
drwxr-xr-x. 3 root root   55 Oct 22 12:17 users
[root@jenkins jenkins]# cd updates/
[root@jenkins updates]# sed -i 's/http:\/\/updates.jenkins-ci.org\/download/https:\/\/mirrors.tuna.tsinghua.edu.cn\/jenkins/g' default.json && sed -i 's/http:\/\/www.google.com/https:\/\/www.baidu.com/g' default.json

```
最后，Manage Plugins点击Advanced，把Update Site改为国内插件下载地址<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414064898-57c08633-5906-4f74-85f5-937bf208b870.png#align=left&display=inline&height=273&originHeight=545&originWidth=1473&size=40533&status=done&style=none&width=736.5)<br />[https://mirrors.tuna.tsinghua.edu.cn/jenkins/updates/update-center.json](https://mirrors.tuna.tsinghua.edu.cn/jenkins/updates/update-center.json)

重启jenkins可以直接在url地址后面写restart也可以直接使用命令行重启例如[http://10.1.1.12:8088/](http://10.1.1.12:8088/)restart

<a name="gNz2T"></a>
## c、汉化界面测试替换的下载地址
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414353080-80cb3d81-9489-403d-86c8-6bc73fdf89cc.png#align=left&display=inline&height=364&originHeight=727&originWidth=1920&size=164964&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414584746-6b073d1b-4ddc-42e2-95cc-c71f06806127.png#align=left&display=inline&height=167&originHeight=237&originWidth=1345&size=54915&status=done&style=none&width=949)<br />安装完成之后再重启一下服务<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414603082-a63b3b7e-8581-4608-b26b-ac89aa442525.png#align=left&display=inline&height=260&originHeight=519&originWidth=1920&size=58448&status=done&style=none&width=960)

<a name="gfbIT"></a>
## d、jenkins权限管理
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414811647-03390b79-6db2-409d-b86e-3044d0e382d9.png#align=left&display=inline&height=243&originHeight=486&originWidth=1920&size=62075&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414868809-722cf1b7-d503-465a-a424-ef381661132e.png#align=left&display=inline&height=240&originHeight=480&originWidth=1920&size=65994&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603414997103-9a5f6e99-ca67-4091-8ae7-81c71b59e6e9.png#align=left&display=inline&height=258&originHeight=516&originWidth=1920&size=111983&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415071397-3d684fec-ecf5-4b5c-b175-5c769c2c646a.png#align=left&display=inline&height=415&originHeight=829&originWidth=1920&size=86065&status=done&style=none&width=960)<br />管理角色<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415143560-aa1a68d5-6025-4657-a2c3-96882c6bc430.png#align=left&display=inline&height=219&originHeight=438&originWidth=1920&size=77617&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415170308-07782bac-1f4e-4b48-bfa4-b427f73c72fa.png#align=left&display=inline&height=193&originHeight=385&originWidth=1920&size=73059&status=done&style=none&width=960)<br />创建角色<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415490398-e3dbd7a8-de61-4999-81d9-c2954617e961.png#align=left&display=inline&height=388&originHeight=775&originWidth=1920&size=108071&status=done&style=none&width=960)<br />创建用户<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415708122-c2599dd5-75af-4baf-ad44-fba61ba7bce0.png#align=left&display=inline&height=129&originHeight=258&originWidth=1918&size=45475&status=done&style=none&width=959)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415689733-185f4798-6e9d-43f1-8783-417a94c21461.png#align=left&display=inline&height=178&originHeight=355&originWidth=1920&size=59504&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415759886-ce235698-b48f-429f-88ae-971bf9478cf2.png#align=left&display=inline&height=172&originHeight=343&originWidth=1920&size=34244&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415788624-f3be732f-42f2-4dd5-b836-7f072ddb0de5.png#align=left&display=inline&height=173&originHeight=346&originWidth=1920&size=34520&status=done&style=none&width=960)<br />备注：要是不授予角色的话登录上去是什么都没有的<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416210745-774bca26-4ceb-42e2-8d87-6969c0792d2a.png#align=left&display=inline&height=283&originHeight=565&originWidth=1832&size=46260&status=done&style=none&width=916)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416220451-dac6d920-1004-4cf0-9001-ce8b1f7c4e82.png#align=left&display=inline&height=107&originHeight=214&originWidth=1920&size=16570&status=done&style=none&width=960)<br />授予角色<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603415895057-51d627b4-af42-4c2f-8175-fe84f35df471.png#align=left&display=inline&height=232&originHeight=463&originWidth=1920&size=83836&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416088983-191b2436-f5b4-41ad-b32d-3b86d1789d7e.png#align=left&display=inline&height=186&originHeight=371&originWidth=1920&size=62118&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416341250-46f260e7-753b-4fba-8f50-7c00275a563d.png#align=left&display=inline&height=310&originHeight=620&originWidth=1920&size=47765&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416328191-a797b3ac-11f2-49dd-b114-c2478e847d7b.png#align=left&display=inline&height=200&originHeight=399&originWidth=1920&size=38991&status=done&style=none&width=960)<br />给其他的用户赋予角色<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416493463-4a6eb66f-0f5c-46f5-a4c2-ac30c82649d4.png#align=left&display=inline&height=315&originHeight=629&originWidth=1920&size=74055&status=done&style=none&width=960)<br />创建两个项目给分别给两个不同的用户<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416606282-fc5adf21-a03e-4c93-a889-d9930f9addf5.png#align=left&display=inline&height=64&originHeight=127&originWidth=1920&size=21710&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416569175-9064a714-4092-49d3-bbfa-74d291baa280.png#align=left&display=inline&height=213&originHeight=426&originWidth=1920&size=49514&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416689740-bdd9b02d-3712-4aa2-94ba-06e02ffcfc62.png#align=left&display=inline&height=130&originHeight=259&originWidth=1920&size=53511&status=done&style=none&width=960)<br />换账户登录查看项目<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416776947-ec5fbf8c-2621-4878-a5c7-124acb7f48e5.png#align=left&display=inline&height=126&originHeight=251&originWidth=1920&size=51241&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603416801187-c3edb345-8e2b-4ce4-adbe-404649d4ac85.png#align=left&display=inline&height=142&originHeight=284&originWidth=1920&size=60016&status=done&style=none&width=960)
<a name="h0tD1"></a>
# 4、jenkins凭证管理
凭据可以用来存储需要密文保护的数据库密码、Gitlab密码信息、Docker私有仓库密码等，以便Jenkins可以和这些第三方的应用进行交互。

<a name="OaqG7"></a>
## 一、使用http方式连接jenkins获取代码

<a name="65U6Q"></a>
### a、安装插件Credentials Binding
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603417230072-af058a3a-cccc-40d6-9067-22fac296e700.png#align=left&display=inline&height=184&originHeight=367&originWidth=1920&size=52616&status=done&style=none&width=960)<br />安装了之后可以在系统管理里面找到第一点的版本可以直接在左侧找到<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603418859392-59332b85-2268-4e4b-86c2-24a1226a080f.png#align=left&display=inline&height=217&originHeight=433&originWidth=1920&size=80854&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603419062092-6b18dcb4-a13b-4e65-aabf-6fd349187c75.png#align=left&display=inline&height=216&originHeight=431&originWidth=1920&size=80837&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603419074309-e24899a3-569b-492a-ac03-dbddb0c73373.png#align=left&display=inline&height=143&originHeight=286&originWidth=1920&size=42676&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603418958177-1f32d97f-9778-4166-be13-5d15865a393b.png#align=left&display=inline&height=229&originHeight=458&originWidth=1920&size=50280&status=done&style=none&width=960)<br />Username with password：用户名和密码<br />SSH Username with private key：使用SSH用户和密钥<br />Secret file：需要保密的文本文件使用时Jenkins会将文件复制到一个临时目录中，再将文件路径设置到一个变量中等构建结束后，所复制的Secret file就会被删除。<br />Secret text：需要保存的一个加密的文本串，如钉钉机器人或Github的api token<br />Certificate：通过上传证书文件的方式
<a name="bimuQ"></a>
### b、在centos7和jenkins上安装Git插件
```c
[root@jenkins updates]# yum install -y git
Loaded plugins: fastestmirror, langpacks
base                                                                       | 3.6 kB  00:00:00
extras                                                                     | 2.9 kB  00:00:00
updates                                                                    | 2.9 kB  00:00:00
Loading mirror speeds from cached hostfile
 * base: mirrors.huaweicloud.com
 * extras: mirrors.huaweicloud.com
 * updates: mirrors.huaweicloud.com
Resolving Dependencies
--> Running transaction check
---> Package git.x86_64 0:1.8.3.1-23.el7_8 will be installed
[root@jenkins updates]# git --version
git version 1.8.3.1
[root@jenkins updates]#

```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603419927593-a1b91476-bff6-470f-afdc-c954977c3f67.png#align=left&display=inline&height=128&originHeight=256&originWidth=1590&size=137900&status=done&style=none&width=795)<br />创建一个项目测试是否有Git选项<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603419977084-d4b978e2-32e8-4555-855e-a2e0c04def9d.png#align=left&display=inline&height=288&originHeight=588&originWidth=1920&size=71388&status=done&style=none&width=940)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603420328100-1ba46894-f8ec-4f56-bb98-1279dbae8e72.png#align=left&display=inline&height=236&originHeight=479&originWidth=1891&size=54588&status=done&style=none&width=930)

<a name="BCTue"></a>
### c、添加一个用户凭证

备注：经过做实验的时候发现了一个问题我用zhangsan的这个用户没得办法正常的使用所以选择的是root账户，不知道这个原因是啥<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603421053965-f449aa13-9af5-425a-bef6-d1df04957e0a.png#align=left&display=inline&height=232&originHeight=464&originWidth=1920&size=56736&status=done&style=none&width=960)<br />gitlab上获取用户登录http连接<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603420849450-8178202d-ac8d-4399-b5cc-35538f283483.png#align=left&display=inline&height=142&originHeight=284&originWidth=1920&size=60549&status=done&style=none&width=960)<br />jenkins<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603421206273-9e9c0d48-9116-4b97-9895-34787be5e9e5.png#align=left&display=inline&height=139&originHeight=277&originWidth=1920&size=63652&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603421144659-ad3adb79-132c-4655-89df-e174f7e489f0.png#align=left&display=inline&height=200&originHeight=400&originWidth=1920&size=62077&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603421240586-ee8bedf4-1620-4b13-88ee-77d2bd33559e.png#align=left&display=inline&height=334&originHeight=667&originWidth=1920&size=77059&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603421300601-401327c7-1e33-4eef-813e-f0e98924548a.png#align=left&display=inline&height=205&originHeight=410&originWidth=1920&size=55157&status=done&style=none&width=960)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603422017356-b6237579-1163-442b-a5cf-0aaabd8e5fd2.png#align=left&display=inline&height=328&originHeight=655&originWidth=1920&size=101080&status=done&style=none&width=960)<br />命令行查看从gitlab上面获取的代码
```c
[root@jenkins itcast01]# pwd
/var/lib/jenkins/workspace/itcast01
[root@jenkins itcast01]# ls
code.py  README.md
[root@jenkins itcast01]# cat code.py
print("代码测试")[root@jenkins itcast01]#
[root@jenkins itcast01]# cat README.md
这是测试平台用于验证是否能连接上pycharm[root@jenkins itcast01]#

```
<a name="fpsSO"></a>
## 二、采用ssh私钥认证获取代码

<a name="v7zEh"></a>
### a、jenkins服务器生成ssh秘钥
```c
[root@jenkins ~]# ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/root/.ssh/id_rsa):
Created directory '/root/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /root/.ssh/id_rsa.
Your public key has been saved in /root/.ssh/id_rsa.pub.
The key fingerprint is:
3b:d3:c4:21:9f:59:8c:62:b3:c9:ee:41:b6:71:5c:12 root@jenkins
The key's randomart image is:
+--[ RSA 2048]----+
|          E      |
|           +     |
|        = + +    |
|       o X B     |
|        S O      |
|       + B       |
|        B .      |
|       . +       |
|        .        |
+-----------------+
[root@jenkins ~]# ll /root/.ssh/
total 8
-rw-------. 1 root root 1679 Oct 23 01:24 id_rsa
-rw-r--r--. 1 root root  394 Oct 23 01:24 id_rsa.pub
[root@jenkins ~]# cat  /root/.ssh/id_rsa.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC11hopu851+lmKs1vZr23bK3JUtxMbJQBwI5aJb6gZvFnIz6HsT0QCt+7nqM0hautAzY5jS76mUJt/g0eHFV2bOIa2h2h2aikQezYVpyY+A5NADP8GCQHKmozWWP5LhKKDU1V4Ryu8wJy98EauYw8Zk11jgGIfO142mK6V0upzfi2qq7jl8pNF2cK4G9SqlCjG/ien3YFYNLMNLROa/TygnvNGxGDawtpDZ1IdsXcmH1TZXzv7B85TmvHVvT1/3YT22W+ICx5lr+PjmfkUkqOVl9Y/0QOa+48x92IlP0sM/rD3SIfIQuK7gXXN5viPXXLrK/OLnT+A2ElbcIoI0EzH root@jenkins
[root@jenkins ~]# cat  /root/.ssh/id_rsa
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAtdYaKbvOdfpZirNb2a9t2ytyVLcTGyUAcCOWiW+oGbxZyM+h
7E9EArfu56jNIWrrQM2OY0u+plCbf4NHhxVdmziGtododmopEHs2FacmPgOTQAz/
BgkBypqM1lj+S4Sig1NVeEcrvMCcvfBGrmMPGZNdY4BiHzteNpiuldLqc34tqqu4
5fKTRdnCuBvUqpQoxv4np92BWDSzDS0Tmv08oJ7zRsRg2sLaQ2dSHbF3Jh9U2V87
+wfOU5rx1b09f92E9tlviAseZa/j45n5FJKjlZfWP9EDmvuPMfdiJT9LDP6w90iH
yELiu4F1zeb4j11y6yvzi50/gNhJW3CKCNBMxwIDAQABAoIBAEiS/Hw9rgiedBCM
x5MZloIMVCSFsUV7PjKDsbiG7RPgfgFNBbaYaG+hU+P4Fm9JCj4iDLuK5rvgnj+F
eQ3sNCEYrYO6hbIFawBd/NclkHjd2m6NAVg6ThG8H4wJlu0zMd2ybAGGfmlmgui4
Nq2xcbxmebaV7hPDETY/OCcqOFMBk75R6fEs6+q7umB3LvuJgDIUonfBHpOO+IH3
0W3Dyq5WftXqjw+LQfSiUWkW+ssFecjSGkylIN76h5rgZS5NtG8gQKkCY9zIzmm2
WrFFyE/ONhBR7ZEEZ5AeSBS4/cvkeow3w5MG4OmH0IksGmgPA+kh0ECn59F0NbOO
qCP6cekCgYEA5vZu6+MUdutnw0Y5ahp9o2tn7i2xmypcnQuwbPP2YvLJUtw+jB5b
3kL3PXArUl2QL4oJb+lsizKcdhlmi2HEsrIG/bvjKdHkzPiNqqnOcd8/esnQ2/Sb
Ld11J+5g36IVUU+2hdPuuVWVHJAL5dFH4Gw80o+l1YPmIfmJP6uBNPUCgYEAyYxW
yuz5rhN+HPXyCrONMQvfdylGlc+ZY7IwYukY1aU/34nga5KyT3oACcLipG0bNc7X
jYCqclmyRA2cQHakPZNlRaK/HiGmb2DWajJFVqSTlv2KpxFv9x2p33B5h5D7BnLT
KX8ERUSFzxjzyiZ9CpJftyH1UKBUqzdij7gZBUsCgYEArMdgFWEMFyZlumSXYPOu
V4lq93gBbKWVIPr0oZ0kDcO4rh8LP4Eakrwf/zshfX129slZ4/mIdN2IxKSd3Nln
75JUvo5HEyrs8hVizXA4e2FaOL4vW8yyJe6MzTF8EPBM6EYsshLofIc0hGiSLfuQ
vKfcHFzP31nL2FC3xwTDJN0CgYA0usyyRpGEI4FpHkd22UblQA/04w2egDPHS4Ux
O37N1nABkdbvDfAsruR3qU8WaZ0m1Li8Qp9VT3Y+K7lnfCK4IWnrcArEXVVn6lPu
cLyHUIN0i1zNHNjINDCPTqPc/edO5pesUZjSbnz7k7Gkf7FTJLx58x51O50rNlBI
/Yc+RQKBgQDUMnoVW6FMtAnSCtBPEi1JPis/DlxOQIJd9+UB3zw3UR3HG6V184AQ
3MWB59m6rYcj0Gn0z+DqmtstzHPV7JF/AXcAqIsuLmUzl0SVsmjlH+pFvHlID6ez
oKBOoU/CDbPeSX5e5AC9AsCXdl3LdpbPzZHhEV/dATzdgSVNooHFqg==
-----END RSA PRIVATE KEY-----
[root@jenkins ~]#

```
<a name="SRyyu"></a>
### b、在gitlab服务器上面添加秘钥
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603430960421-9a8bafaa-7d2b-43bd-9d64-a3be5892a373.png#align=left&display=inline&height=396&originHeight=791&originWidth=1919&size=138000&status=done&style=none&width=959.5)
<a name="A69HS"></a>
### c、jenkins凭证管理
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603431288367-0a5b9c07-5790-4c12-98d5-f16559693ceb.png#align=left&display=inline&height=334&originHeight=667&originWidth=1920&size=65566&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603431522305-f502dc4d-5602-4a3e-b99b-a667e33bf5be.png#align=left&display=inline&height=168&originHeight=336&originWidth=1920&size=64200&status=done&style=none&width=960)
<a name="Ro67r"></a>
### d、创建一个项目
![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603431584691-201742bd-ae93-4dac-a96a-0754f060a821.png#align=left&display=inline&height=310&originHeight=620&originWidth=1912&size=75493&status=done&style=none&width=956)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603432007755-a757d63d-de07-432d-9ded-e76e37d28aca.png#align=left&display=inline&height=350&originHeight=699&originWidth=1920&size=80424&status=done&style=none&width=960)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603431896832-41b7efdd-06db-4c93-a437-3b626ec92412.png#align=left&display=inline&height=297&originHeight=593&originWidth=1920&size=83504&status=done&style=none&width=960)

![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603431946258-87bd3d5c-57fc-432c-9ee3-0a432694adbb.png#align=left&display=inline&height=327&originHeight=654&originWidth=1920&size=99206&status=done&style=none&width=960)<br />终端查看验证
```c
[root@jenkins ~]# cd /var/lib/jenkins/workspace/&&ls -l
total 0
drwxr-xr-x. 3 root root 47 Oct 22 22:48 itcast01
drwxr-xr-x. 2 root root  6 Oct 22 22:48 itcast01@tmp
drwxr-xr-x. 2 root root  6 Oct 22 21:30 itheima
drwxr-xr-x. 3 root root 47 Oct 23 01:44 test02
drwxr-xr-x. 2 root root  6 Oct 23 01:44 test02@tmp
[root@jenkins workspace]# cd test02
[root@jenkins test02]# ls -l
total 8
-rw-r--r--. 1 root root 21 Oct 23 01:44 code.py
-rw-r--r--. 1 root root 55 Oct 23 01:44 README.md
[root@jenkins test02]#

```
<a name="RRICL"></a>
# 三、Maven安装和配置

<a name="QcP6H"></a>
## a、下载安装包apache-maven-3.6.2-bin.tar.gz
```c
[root@jenkins opt]# tar -xf apache-maven-3.6.2-bin.tar.gz
[root@jenkins opt]# ls
apache-maven-3.6.2  apache-maven-3.6.2-bin.tar.gz  rh
[root@jenkins opt]# mkdir maven
[root@jenkins opt]# mv apache-maven-3.6.2/* maven/
[root@jenkins opt]# cd maven/
[root@jenkins maven]# ls
bin  boot  conf  lib  LICENSE  NOTICE  README.txt
[root@jenkins maven]# tail -3 /etc/profile
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk
export MAVEN_HOME=/opt/maven
export PATH=$PATH:$JAVA_HOME/bin:$MAVEN_HOME/bin
[root@jenkins maven]# source /etc/profile
[root@jenkins maven]# mvn -v
Apache Maven 3.6.2 (40f52333136460af0dc0d7232c0dc0bcf0d9e117; 2019-08-27T11:06:16-04:00)
Maven home: /opt/maven
Java version: 1.8.0_262, vendor: Oracle Corporation, runtime: /usr/lib/jvm/java-1.8.0-openjdk-1.8.0.262.b10-0.el7_8.x86_64/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.10.0-229.el7.x86_64", arch: "amd64", family: "unix"
[root@jenkins maven]#

```
<a name="zIbkE"></a>
## b、jenkins部署maven
全局设置<br />**![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603440626506-ffa08884-24db-4268-8a58-bed826cd96e7.png#align=left&display=inline&height=321&originHeight=642&originWidth=1920&size=63345&status=done&style=none&width=960)![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603440636166-5b2b1aff-814f-4cc3-a690-d85dd3c39067.png#align=left&display=inline&height=158&originHeight=316&originWidth=1920&size=24958&status=done&style=none&width=960)**<br />添加Jenkins全局变量<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603441268161-16697ee5-1802-430a-9e65-d5d370a302d9.png#align=left&display=inline&height=59&originHeight=118&originWidth=1919&size=17647&status=done&style=none&width=959.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/2476579/1603441233199-39e2f227-01f9-4aa0-aceb-1d684734817b.png#align=left&display=inline&height=311&originHeight=622&originWidth=1920&size=42378&status=done&style=none&width=960)**<br />环境变量<br />JAVA_HOM    /usr/lib/jvm/java-1.8.0-openjdk<br />M2_HOME    /opt/maven<br />PATH+EXTRA $M2_HOME/bin

<a name="0jxcO"></a>
## c、修改配置文件

**待修改为完善**
```c
[root@jenkins ~]# vim /opt/maven/conf/settings.xml
53   -->
54   <localRepository>/root/repo</localRepository>

161   <mirror>
162   <id>alimaven</id>
163   <name>aliyun</name>
164   <url>http://maven.aliyun.com/nexus/groups/public</url>
165   <mirrorOf>central</mirrorOf>
166   </mirror>
```


















