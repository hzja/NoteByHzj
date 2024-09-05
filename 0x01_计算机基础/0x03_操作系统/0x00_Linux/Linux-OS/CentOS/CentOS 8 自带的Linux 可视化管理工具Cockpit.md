Linux CentOS8
<a name="zwl3y"></a>
## Cockpit简介
Cockpit是CentOS 8内置的一款基于Web的可视化管理工具，对一些常见的命令行管理操作都有界面支持，比如用户管理、防火墙管理、服务器资源监控等，使用非常方便，号称人人可用的Linux管理工具。<br />下面是Cockpit的管理界面，看起来还是挺炫酷的！<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644975102013-f9debc55-068e-41c1-8178-eae0108633f6.png#clientId=ub7084dcc-94bd-4&from=paste&height=713&id=ud1f35877&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=230632&status=done&style=none&taskId=uc38b0079-129a-49d1-9a79-7af0fdac1f7&title=&width=1536)
<a name="avkwo"></a>
## CentOS 8安装
如果想体验最新版本的Cockpit，需要安装CentOS 8，下面先简单看看CentOS 8的安装。

- CentOS 8的安装与CentOS 7基本相同，镜像下载地址：[https://vault.centos.org/8.5.2111/isos/x86_64/](https://vault.centos.org/8.5.2111/isos/x86_64/)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644974568908-85b081bf-4b21-4d6b-97d5-1390a36ad2b4.png#clientId=ub7084dcc-94bd-4&from=paste&height=713&id=ucdf154c0&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=241242&status=done&style=none&taskId=ua09199b8-db7e-4969-b62b-c1c4398501d&title=&width=1536)

- 使用CentOS 8的yum命令安装软件时经常会遇到无法下载的问题，切换到阿里云的镜像源即可解决，这里使用Centos-vault-8.5.2111.repo仓库配置；
```bash
# 先将原BaseOS配置进行备份
mv /etc/yum.repos.d/CentOS-Linux-BaseOS.repo /etc/yum.repos.d/CentOS-Linux-BaseOS.repo.bak
# 再下载新配置
sudo wget -O /etc/yum.repos.d/CentOS-Linux-BaseOS.repo http://mirrors.aliyun.com/repo/Centos-vault-8.5.2111.repo
```

- yum仓库配置文件在/etc/yum.repos.d目录下，再修改CentOS-Linux-AppStream.repo文件，直接拷贝CentOS-Linux-BaseOS中的appstream部分即可；
```bash
[appstream]
name=CentOS-8.5.2111 - AppStream - mirrors.aliyun.com
baseurl=http://mirrors.aliyun.com/centos-vault/8.5.2111/AppStream/$basearch/os/
        http://mirrors.aliyuncs.com/centos-vault/8.5.2111/AppStream/$basearch/os/
        http://mirrors.cloud.aliyuncs.com/centos-vault/8.5.2111/AppStream/$basearch/os/
gpgcheck=0
gpgkey=http://mirrors.aliyun.com/centos/RPM-GPG-KEY-CentOS-Official
```

- 运行如下命令清空缓存并生效；
```bash
sudo yum clean all
sudo yum makecache
```

- 接下来查询一个安装包信息测试下，发现已经可以正常使用了。

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644975819777-cd83ded3-44fa-488c-8a37-7fc3d8245a96.png#clientId=uccb91113-f531-4&from=paste&height=650&id=uaf46aa76&originHeight=1625&originWidth=2175&originalType=binary&ratio=1&rotation=0&showTitle=false&size=222558&status=done&style=none&taskId=u370e5399-2bf4-45fa-8363-043333cb2e6&title=&width=870)
<a name="SJDLC"></a>
## Cockpit安装启动
下面介绍下Cockpit的安装和启动，非常简单。

- CentOS 8默认已安装Cockpit，直接启动服务即可；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644974537309-9bfc863e-e504-456f-828b-e47ebb2830ed.png#clientId=ub7084dcc-94bd-4&from=paste&height=198&id=u8a95d7a4&originHeight=495&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80280&status=done&style=none&taskId=ub5cb6b01-c745-42a5-8b6c-489d2bab9e1&title=&width=877.2)
```bash
# 配置cockpit服务开机自启
systemctl enable --now cockpit.socket
# 启动cockpit服务
systemctl start cockpit
```

- CentOS 7上如果要使用Cockpit的话，需要自行安装，并开放对应服务；
```bash
# 安装
yum install cockpit
# 开放服务
firewall-cmd --permanent --zone=public --add-service=cockpit
# 重新加载防护墙
firewall-cmd --reload
```

- 安装完成后即可通过浏览器访问Cockpit，使用Linux用户即可登录（比如root用户），访问地址：http://192.168.3.106:9090/

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644974424017-908cfd80-c69b-4957-8649-7ef08693d358.png#clientId=ub7084dcc-94bd-4&from=paste&height=713&id=ud3ebd7f8&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=704531&status=done&style=shadow&taskId=u7f06b607-4f76-49c2-9cbc-1a457e004f4&title=&width=1536)
<a name="G1xqB"></a>
## Cockpit使用
之前经常使用命令行来管理Linux服务器，有了Cockpit就可以愉快地使用图形化界面了，下面来体验下Cockpit的功能。

- 通过概览查看服务器的基本信息，包括CPU内存使用情况、系统信息、服务器配置等；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978201561-db633fe9-f9cf-4fb9-96c1-4ec6d8ba1bd0.png#clientId=ua3ed0890-98e1-4&from=paste&height=713&id=u64e147c4&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=342130&status=done&style=none&taskId=ue19deac3-07b0-4f63-9800-71aaca0afcc&title=&width=1536)

- 点击查看图表可以查看到更详细的CPU、内存、磁盘、网络等监控信息，基本上就是个界面版的top命令；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978712553-73ed3157-f01c-457d-b978-6ad4f1656806.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=u38db382c&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=323628&status=done&style=none&taskId=u98fb1f99-bdd7-43a9-b94c-331def1e999&title=&width=1536)

- 通过存储可以查看更为详细的文件系统信息，还可以进行卷组的管理及NFS的挂载；
- 通过网络可以查看防火墙及网络监控信息，可以进行防火墙的开启关闭；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978726976-e9bd3b6b-9e87-48b1-a5e5-14ef41a1c78e.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=uf1b27677&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=248995&status=done&style=none&taskId=u79f69bb2-2848-4dc9-9234-12434dc4150&title=&width=1536)

- 点击防火墙可以查看开放的服务端口，通过添加服务可以直接开放端口，还在用firewalld命令？

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978783169-a2fcc0cd-612a-4f42-8a5e-ebc7970c99e4.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=u9cd99cfb&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191150&status=done&style=none&taskId=ucbb0c351-5784-4950-813e-54545ad5b5f&title=&width=1536)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978811650-5226e6ab-41a3-47fe-9550-a4ddf4f3b511.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=u8cb74a52&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=300718&status=done&style=none&taskId=u56100adb-d4a8-414f-856e-56f01eec717&title=&width=1536)

- 通过Podman容器管理可以像使用Docker一样下载镜像并创建运行容器；
- 比如说可以根据Nginx镜像来创建一个nginx容器，并运行在80端口上；
- 此时直接访问服务器的80端口就可以访问到Nginx的首页了；
- 通过账户可以方便地管理Linux中的用户，再也不用useradd命令了；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978832402-06b2da6b-8bdf-4b5e-af75-907ea3b6e159.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=u98d474cb&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=182867&status=done&style=none&taskId=ub2ae4d24-5830-486b-bc64-4065e4b350f&title=&width=1536)

- 另外Cockpit在服务器软件需要更新时也会提示通过软件更新可以进行更新操作；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978140174-0fb99a16-62ee-423c-a070-a8e0f54315d2.png#clientId=ua3ed0890-98e1-4&from=paste&height=713&id=ue98fadf1&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=302902&status=done&style=none&taskId=u9a9956a3-f661-4fa2-9079-7066842d551&title=&width=1536)

- 通过应用功能可以下载一些服务器应用，目前只有几个可以安装，期待以后能有更多选择；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644976942798-f0bda749-5d63-4d89-a748-360a9c9eaa28.png#clientId=ua3ed0890-98e1-4&from=paste&height=713&id=u6e8d80c3&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=150099&status=done&style=none&taskId=ud992325c-742e-498b-86ba-98f4e5bc76f&title=&width=1536)

- 如果以上功能满足不了需求，Cockpit也提供了命令行功能，打开终端即使用；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644975034816-c2f29662-3fea-46ec-bd73-1239242bf640.png#clientId=ub7084dcc-94bd-4&from=paste&height=713&id=ucd141b87&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=480107&status=done&style=none&taskId=u098db7b0-7f1c-44de-8ec0-70d06f471ce&title=&width=1536)

- Cockpit还提供了SELinux管理功能，可以控制其开启和关闭。

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644978850838-d54e085e-b3f6-40dd-8b6b-534d86ab1c67.png#clientId=ucb09f609-456c-4&from=paste&height=713&id=u753759e5&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=139734&status=done&style=shadow&taskId=u3a794afb-c812-4521-bbfd-02cd63d0b89&title=&width=1536)
<a name="bEefY"></a>
## 总结
作为CentOS 8官方内置的可视化管理工具，Cockpit确实涵盖了很多常用的服务器管理功能，界面炫酷且使用方便！升级到CentOS 8的朋友，不妨尝试下它！
<a name="NxRnq"></a>
## 参考资料
官网文档：[https://cockpit-project.org/documentation.html](https://cockpit-project.org/documentation.html)
