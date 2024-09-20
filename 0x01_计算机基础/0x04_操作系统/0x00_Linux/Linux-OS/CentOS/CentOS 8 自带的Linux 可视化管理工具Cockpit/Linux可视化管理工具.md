Linux<br />如何更好的管理和监控一台 Linux 服务器呢？介绍一个简单易用基于 web 的 Linux 服务管理工具——Cockpit。
<a name="SL3mD"></a>
## 项目简介
Cockpit 可以帮助用户通过一个易于使用的 Web 界面来配置和管理 Linux 服务器。它适用于各种 Linux 发行版，包括 Red Hat、Fedora、CentOS、Ubuntu 等。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738253-18a66df5-552e-4261-860e-09fd97da6fee.png#averageHue=%23f7f7f7&clientId=u591aaf8d-61f4-4&from=paste&id=u7c450bd3&originHeight=1112&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u51a88aac-1c40-47cb-a80f-4cad416b238&title=)<br />Cockpit 是一个轻量级的应用，安装包只有 4MB，而且安装和使用都非常的简单易上手。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738240-b4df4bcb-d356-421c-b20e-6e6ad8adf97d.png#averageHue=%233c4249&clientId=u591aaf8d-61f4-4&from=paste&id=ucea52e74&originHeight=257&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub1792563-b76e-4d10-8c07-24c1aacc6fb&title=)<br />Cockpit 提供了许多常用的系统管理功能，例如用户和组管理、网络设置、防火墙配置、服务管理、日志查看等。它还可以用于监视系统资源使用情况，例如 CPU、内存、磁盘空间等。<br />而且，Cockpit 还支持安装各种插件，方便扩展其他功能。
<a name="nCrnx"></a>
## 项目安装
Cockpit 为不同的 Linux 版本提供了不同的安装方式。全部的方法可以参考这篇文档：[https://cockpit-project.org/running.html](https://cockpit-project.org/running.html)<br />在这里用最常用的 centOS 来举例。
```bash
# 安装
sudo yum install cockpit
# 启动
sudo systemctl enable --now cockpit.socket
# 防火墙配置（也可以不配置）
sudo firewall-cmd --permanent --zone=public --add-service=cockpit
sudo firewall-cmd --reload
```
<a name="fzikw"></a>
## 项目使用
安装完成后，访问 https://ip:9090，输入服务器的用户名密码，即可进入 web 端的控制界面。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738248-97c883ee-5440-4b57-b704-e08a9e3d94c6.png#averageHue=%23eaeaea&clientId=u591aaf8d-61f4-4&from=paste&id=ud39f3e55&originHeight=402&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u57b9cbd7-47de-4e47-8753-db404861b63&title=)<br />可以看到，首页是性能的总览图，可以直观的查看各种系统参数和资源使用情况，如 CPU、内存和硬盘使用情况，以及网络流量、进程和负载、硬件信息等。点击每一项都可以进入详情页，进一步查看详细的状况。<br />左侧的菜单栏，可以看到 Cockpit 有如下功能。
<a name="BEE6S"></a>
### 查看日志
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738233-63a9cfa8-6b26-446f-bba0-a981b7be3aeb.png#averageHue=%23cdcdcd&clientId=u591aaf8d-61f4-4&from=paste&id=u26c48eab&originHeight=478&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uba3c9768-9832-42ab-9b70-feb2848270d&title=)
<a name="uFISZ"></a>
### 网络管理
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738242-2637b33c-8f42-4d84-acb8-09c1d636f392.png#averageHue=%23e7e7e7&clientId=u591aaf8d-61f4-4&from=paste&id=u802550aa&originHeight=331&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub2ed7ac5-6e28-4369-a86b-fc6a86557d7&title=)
<a name="mG22K"></a>
### 账户管理
在这里可以创建账户，以及查看已有账户信息和密码维护。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576738982-ad201bbe-6145-46f0-aa80-0c33e4b5b2e6.png#averageHue=%23e8e8e8&clientId=u591aaf8d-61f4-4&from=paste&id=u8a46fb8f&originHeight=436&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue6cda9f0-0e8e-427a-b797-72eed038fd6&title=)
<a name="GRTej"></a>
### 服务管理
在这里可以查看服务列表，以及管理服务的启动和关闭。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680576739063-f8a91de1-dae1-41ba-a1b3-ecc358be9cf8.png#averageHue=%23d6d6d6&clientId=u591aaf8d-61f4-4&from=paste&id=uda61b3af&originHeight=524&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0247a4d7-8eb0-42db-b89e-bc60d4b4f34&title=)
<a name="vU6yo"></a>
## 总结
Cockpit 是一个非常出色的开源项目，它提供了一个易于使用的 Linux 服务管理工具，可以轻松配置和管理各种 Linux 服务和设置。它具有易于安装和使用的优点，以及可扩展性和可定制性的优点。如果感兴趣就赶快来试试吧~<br />项目地址：[https://github.com/cockpit-project/cockpit](https://github.com/cockpit-project/cockpit)
