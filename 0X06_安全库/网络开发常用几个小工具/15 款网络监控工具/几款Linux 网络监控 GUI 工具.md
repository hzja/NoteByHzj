网络监控
<a name="Ug9zU"></a>
## 概述
这里分享几款Linux网络监控的GUI工具，GUI工具与CLI工具相比，GUI工具输出通常包括图形和可视化组织的统计数据，便于一目了然地监控所有网络带宽和性能。
<a name="yxpif"></a>
## Nagios Core - 监控系统、网络和基础设施
Nagios Core** **是一款开源、功能丰富且免费的带宽监控应用程序，它还提供具有官方支持的付费版本。该工具的界面是基于网络的，可以监控多种服务，例如 HTTP、POP3 和SMTP。另一个很棒的功能是出现问题时的自动警报，允许用户在问题出现时立即解决。Nagios 还通过 SNMP 在网络设备（例如交换机和路由器）中提供带宽监控，这有助于发现可能的带宽占用。<br />此外，Nagios 有助于监控每个端口的带宽利用率并快速检测网络中断和协议故障。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676379783419-c7f6963c-ea50-48b2-a018-e5ef972453d1.jpeg#averageHue=%23d0dec7&clientId=u95200cc4-fdb2-4&from=paste&id=u4ffaac64&originHeight=371&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uaab389e8-330a-410d-8c3e-1e0866ea229&title=)
<a name="w7Q26"></a>
## Zabbix - 应用程序和网络监控工具
Zabbix 是一款免费、开源、功能丰富的网络监控应用程序，采用服务器-客户端模型设计。其基于 Web 的界面提供实时网络、服务器、设备和应用程序监控。<br />它生成的数据日志直观地表示网络性能或设备负载指标，可用于跟踪和改善网络性能。该应用程序非常适合小型和大型商业应用程序。<br />Zabbix 支持大多数标准网络协议，例如 HTTP、FTP、SMTP、IMAP 等，无需在被监控设备上安装额外的软件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379783458-bb89ac80-08d1-4580-b862-81bd81618a56.png#averageHue=%23646540&clientId=u95200cc4-fdb2-4&from=paste&id=u6a7b380f&originHeight=448&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc8dab6c3-577b-4e72-a722-55409a6a3c1&title=)
<a name="Uioqy"></a>
## Cacti - 网络监控和绘图工具
Cacti 是一个基于 Web 的网络监控工具，用 PHP 编写，具有直观和用户友好的界面。它的主要优势是全面的数据收集和有关网络带宽和其他指标（例如用户访问和权限）的报告。<br />Cacti 使用脚本收集数据并输出清晰的图表和图形，使用户能够快速查明任何带宽使用高峰或网络问题。所有数据都使用 MySQL 数据库存储，并用于生成自定义图表。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379783453-91d44b57-3270-4e63-8b79-49346bd34ff1.png#averageHue=%23e6e150&clientId=u95200cc4-fdb2-4&from=paste&id=u27e617b9&originHeight=413&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucea82b85-038f-4d2e-a731-547013c752d&title=)
<a name="lRjKK"></a>
## Site 24x7 - NetFlow Analyzer
Site 24x7 的 NetFlow Analyzer是一种基于云的网络监控实用程序，可为网络中的多个设备提供广泛的功能。它实现了各种技术，例如 NetFlow、sFlow 和 J-Flow，允许用户收集不同应用程序和设备的统计信息。<br />该工具可以监控源和目标设备、它们的接口和流量。用户可以为所有受监控的指标配置阈值，并在超过阈值时收到警报。<br />NetFlow 不是免费的，尽管它确实提供功能齐全的 30 天免费试用期。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676379783509-77858f96-d8a5-424b-8dd7-bdfabf5ffa4b.jpeg#averageHue=%2318252e&clientId=u95200cc4-fdb2-4&from=paste&id=uceaa73d6&originHeight=419&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6147c0d7-ae2e-4446-8baa-d2feba59ff0&title=)
<a name="r689S"></a>
## ManageEngine - NetFlow 分析器
ManageEngine 的 NetFlow Analyzer是一种基于流量的网络监控工具，可提供流量模式分析和带宽峰值检测。用户可以通过分析流量模式并获取有关设备、接口或应用程序的详细信息来查明网络问题。<br />Analyzer 提供实时监控和故障排除，有助于在网络问题发生时第一时间发现它们。该实用程序还具有可自定义的报告，只需单击几下即可获得。支持的流技术有 NetFlow、sFlow、IPFIX、Netstream、J-Flow 和 AppFlow。<br />关于定价，该应用程序有两种版本——专业版和企业版。主要区别在于每秒的流量和支持的网络数。这两个版本还有 30 天的免费试用期。免费版仅限于监控最多两个接口。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379783435-288dd1bd-fcfe-49f8-80a0-0298b9dea675.png#averageHue=%23e9d676&clientId=u95200cc4-fdb2-4&from=paste&id=u4b00b8f6&originHeight=516&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua69f3261-94b1-408b-b414-80882d19e5f&title=)
<a name="av2st"></a>
## SolarWinds - NetFlow 流量分析器
SolarWinds 的NetFlow Traffic Analyzer是一种网络监控实用程序，具有广泛的监控功能和网络中可扩展的节点数量的能力。该实用程序提供交互式和可视化数据显示，并提供带有颜色编码和可搜索的可视化数据的能力。<br />除了网络监控之外，NTA 还允许用户监控带宽并在出现流量问题或设备出现故障时收到警报。其主要功能之一是监控端口 0 的异常流量、防止攻击和保护网络。<br />该应用程序具有 30 天的全功能免费试用期，之后用户可以在两种许可证类型之间进行选择 -订阅，从 875 欧元起，或永久，从 1585 欧元起。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676379783930-15803682-c912-446f-8cba-dfd46aa8e34b.jpeg#averageHue=%23dfdfd4&clientId=u95200cc4-fdb2-4&from=paste&id=u6c6f7676&originHeight=451&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua0155e89-48cb-4187-9d01-a55951a3fd4&title=)
<a name="dPwGL"></a>
## Darkstat - 捕获网络流量
Darkstat** **是一个轻量级且简单的基于 Web 的网络监控实用程序，也可以在命令行中运行。该工具实时工作，分析流量并通过HTTP或命令提示符以图形格式显示网络统计信息。<br />Darkstat 捕获系统的流量信息和计算机使用统计数据，并以图形报告的形式输出收集到的信息。通过图表可以很容易地注意到任何异常的使用高峰。<br />支持IPv4和IPv6，支持异步反向DNS解析。该实用程序是免费的。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676379783860-003c6047-ae74-4d9d-af15-15167e512a8d.jpeg#averageHue=%23cdd6e5&clientId=u95200cc4-fdb2-4&from=paste&id=u65c02e82&originHeight=535&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6a840240-1e52-46ef-b80e-801c603a83d&title=)
<a name="G8Tkc"></a>
## SARG – 鱿鱼分析报告生成器
SARG是一款开源网络和带宽监控应用程序，通过分析 squid 日志文件并生成有关网络使用情况的详细 HTML 报告。报告包括 IP 地址、总带宽使用情况、发送和接收的字节数、访问过的站点等。<br />该工具对于监控连接到同一网络的各台机器上的网络使用情况特别有用。SARG 自动生成报告，但它要求允许squid代理服务器写入日志文件以供其读取。<br />Squid是一个缓存代理，支持 HTTP、HTTPS、FTP 等。它优化了客户端和服务器之间的数据流，缓存经常使用的内容以节省带宽。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379783846-4691dbca-5685-4c3c-b1c8-b1d5f9268a91.png#averageHue=%23eee6d3&clientId=u95200cc4-fdb2-4&from=paste&id=uad2c3550&originHeight=378&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u655fbda4-ccde-428a-89ee-fbb4e7e7a17&title=)
<a name="ElXrX"></a>
## Monitorix - 系统和网络监控工具
Monitorix** **是一个免费的开源 Linux/Unix 服务器监控工具，用于系统资源和网络监控。它还支持服务器以外的嵌入式设备。<br />它支持 IPv4 和 IPv6，并允许用户监控其网络上所有设备的网络流量和带宽。该实用程序是轻量级的，包括一个用于查看统计数据和图形的内置 HTTP 服务器。这些图表采用颜色编码，以便进行更直接的分析。<br />除了网络统计数据，Monitorix 还可以监控几乎所有系统资源，包括磁盘驱动器温度、系统负载、内核使用情况、CPU 使用情况、环境传感器统计数据等等。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379783949-08de09fc-d40b-4139-b116-8eb27de3f9d3.png#averageHue=%231f1f1e&clientId=u95200cc4-fdb2-4&from=paste&id=u7101ee7c&originHeight=289&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u392f2812-7bf1-48e3-bec5-8182667b8e0&title=)
<a name="CZXIz"></a>
## Observium – 网络监控平台
Observium是一款网络监控工具，支持 Linux、Windows、FreeBSD、Cisco 等众多平台和操作系统。它配备了功能齐全且直观的界面和设备自动检测功能。该应用程序主要用于小型服务器。<br />它使用标准的 SNMP 来收集网络指标，然后创建直观的图表，让网络监控一目了然。<br />它有两种版本 -完全免费的Observium Community和Observium Professional。<br />建议将社区版用于个人服务器，而专业版则提供实时更新、网络活动通知和警报。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379784106-5fadb10e-e9d0-43f2-9b58-6f635cff88f3.png#averageHue=%23f3f2ec&clientId=u95200cc4-fdb2-4&from=paste&id=u2012a0a4&originHeight=458&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u56006ba5-419e-4047-9c70-063211068c8&title=)
<a name="VMwYO"></a>
## EtherApe - 网络监控
EtherApe是一款专为 Linux/Unix 设计的图形化网络监控工具。它支持多种设备，包括以太网、FDDI、令牌环、ISDN、PPP、SLIP 和 WLAN 设备，还支持多种封装格式。<br />该实用程序可以充当数据包嗅探器和带宽监控工具，并使用图形显示网络活动。它从文件中获取数据或从网络中实时获取数据，允许用户过滤他们想要查看的流量。<br />图中的每个节点都是一个特定的主机，而链接是到主机的连接。一切都用颜色编码来代表网络上不同的协议和各种类型的流量。<br />EtherApe 是完全免费和开源的。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676379784233-28e6ca96-6295-48e4-a910-c441f15deaca.png#averageHue=%23524343&clientId=u95200cc4-fdb2-4&from=paste&id=u668634f6&originHeight=487&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u44c82ffe-c0fa-4b95-b39a-38eee58a463&title=)
<a name="HRytB"></a>
## 小结
本文分享了几款Linux 系统监控和网络带宽GUI工具 。根据不同工具的特征，总有一款适合业务的选项。
