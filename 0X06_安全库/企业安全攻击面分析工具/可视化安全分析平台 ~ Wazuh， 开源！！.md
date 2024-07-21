网络安全<br />Wazuh 是一个用于威胁预防、检测和响应的开源平台，它能够跨场所、虚拟化、容器化和基于云的环境保护工作负载。解决方案包括一个部署到被监控系统的终端安全代理和一个收集和分析代理收集的数据的管理服务器。此外，Wazuh 已经与 Elastic Stack 完全集成，提供了一个搜索引擎和数据可视化工具，允许用户通过他们的安全警报进行导航。
<a name="H7Lcl"></a>
## 使用场景
<a name="y7bMo"></a>
### 入侵检测
Wazuh 代理扫描被监控的系统，寻找恶意软件，rootkit 和可疑的异常。它们可以检测隐藏文件、隐藏进程或未注册的网络侦听器，以及系统调用响应中的不一致。除了代理功能之外，服务器组件还使用基于特征的入侵检测方法，使用其正则表达式引擎来分析收集的日志数据并寻找危害的指标。
<a name="ebTMM"></a>
### 日志数据分析
Wazuh 代理读取操作系统和应用程序日志，并安全地将它们转发给中央管理器，以便进行基于规则的分析和存储。当没有部署代理时，服务器还可以通过 syslog 从网络设备或应用程序接收数据。Wazuh 规则帮助您了解应用程序或系统错误、错误配置、企图和/或成功的恶意活动、违反政策以及各种其他安全和操作问题。
<a name="jAUxv"></a>
### 完整性检查
Wazuh 监视文件系统，识别需要密切关注的文件的内容、权限、所有权和属性的更改。此外，它本机识别用于创建或修改文件的用户和应用程序。完整性检查能力可以与威胁情报结合使用来识别威胁或被入侵的主机。此外，一些美国守规标准，如 PCI DSS，要求它。
<a name="LbfCA"></a>
### 漏洞检测
Wazuh 代理提取软件清单数据，并将这些信息发送到服务器，在服务器上与不断更新的 CVE 数据库相关联，以识别众所周知的脆弱软件。自动的漏洞评估可以帮助您找到关键资产中的弱点，并在攻击者利用它们破坏您的业务或窃取机密数据之前采取纠正措施。
<a name="DZYi8"></a>
### 配置评估
Wazuh 监视系统和应用程序配置设置，以确保它们符合您的安全策略、标准和/或加强指南。代理执行定期扫描，以检测已知存在漏洞、未打补丁或配置不安全的应用程序。此外，可以自定义配置检查，对其进行裁剪以适当地与组织保持一致。警报包括更好的配置建议，参考信息和与守规地图的映射。
<a name="GRwFc"></a>
### 事故应变
Wazuh 提供开箱即用的主动响应，以执行各种应对主动威胁的对策，例如在满足某些标准时阻止从威胁来源访问系统。此外，Wazuh 还可用于远程运行命令或系统查询，识别危险指标(IOCs) ，并帮助执行其他实时取证或事件响应任务。
<a name="Gn1LQ"></a>
### 合规
Wazuh 提供了一些必要的安全控制，以符合行业标准和规定。这些特性，结合其可伸缩性和多平台支持，可以帮助组织满足技术遵从性需求。Wazuh 广泛应用于支付处理公司和金融机构，以满足 PCI DSS (支付卡行业数据安全标准)的要求。它的 web 用户界面提供了报告和仪表板，可以帮助解决这个和其他规则(例如 GPG13 或 GDPR)。
<a name="F54Oj"></a>
### 云安全
通过使用集成模块从著名的云服务提供商那里获取安全数据，例如 Amazon AWS、 Azure 或 Google Cloud，Wazuh 帮助在 API 级别上监控云基础设施。此外，Wazuh 还提供了评估云环境配置的规则，可以很容易地发现弱点。此外，Wazuh 轻量级和多平台代理通常用于实例级监视云环境。
<a name="wHG3b"></a>
### 容器安全
Wazuh 提供对 Docker 主机和容器的安全可见性，监视它们的行为并检测威胁、漏洞和异常。Wazuh 代理与 Docker 引擎本地集成，允许用户监视图像、卷、网络设置和运行中的容器。不断收集和分析详细的运行时信息。例如，为以特权模式运行的容器、易受攻击的应用程序、在容器中运行的 shell、对持久卷或映像的更改以及其他可能的威胁发出警报。
<a name="xkqjv"></a>
## 快速安装

1. 下载并运行 Wazuh 安装助手。
```bash
curl -sO https://packages.wazuh.com/4.3/wazuh-install.sh && sudo bash ./wazuh-install.sh -a
```
助手完成安装后，输出会显示访问凭据和确认安装成功的消息。
```bash
INFO: --- Summary ---
INFO: You can access the web interface https://<wazuh-dashboard-ip>
    User: admin
    Password: <ADMIN_PASSWORD>
INFO: Installation finished.
```
到这里现在已经安装并配置了 Wazuh。

2. 使用 https://<wazuh-dashboard-ip> 和凭据访问 Wazuh Web 界面，用户名: admin、密码：<ADMIN_PASSWORD>。当第一次访问 Wazuh 仪表板时，浏览器会显示一条警告消息，指出证书不是由受信任的机构颁发的。这是意料之中的，用户可以选择接受证书作为，或者将系统配置为使用来自受信任机构的证书。

如果要卸载 Wazuh central 组件，请使用选项 `-u` 或 `--uninstall` 运行 Wazuh 安装助手。<br />现在 Wazuh 安装已准备就绪，可以开始部署 Wazuh 代理了，这可用于保护笔记本电脑、台式机、服务器、云实例、容器或虚拟机。该代理可以提供多种安全功能。<br />代理需要根据自己的系统选择合适的安装包，可以从官方文档 [https://documentation.wazuh.com/current/installation-guide/wazuh-agent/index.html](https://documentation.wazuh.com/current/installation-guide/wazuh-agent/index.html) 获取安装。<br />Wazuh WUI  为数据可视化和分析提供了强大的用户界面，此界面还可用于管理 Wazuh 配置并监控其状态。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1655772429623-0e0f7394-1175-4382-ab3f-ddde226aaf49.jpeg#clientId=u3eac0bd6-16a0-4&from=paste&id=u2a475660&originHeight=966&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8e1cc1fc-034f-4bc0-867b-3fe66f701b8&title=)<br />关于Wazuh 的更多使用方式请查看官方文档了解更多相关信息。<br />[https://documentation.wazuh.com/current/index.html ](https://documentation.wazuh.com/current/index.html )<br />Git 仓库：[https://github.com/wazuh/wazuh](https://github.com/wazuh/wazuh)
