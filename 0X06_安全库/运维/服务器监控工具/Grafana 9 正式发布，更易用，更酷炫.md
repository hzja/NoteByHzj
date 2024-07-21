Grafana 9.0 的主要重点是改善 Grafana 的用户体验，使可观察性和数据可视化更易用也更容易获得。无论是通过 Prometheus 和 Loki 可视化查询生成器还是面板和仪表板搜索功能，Grafana 9.0 都引入了更新的工作流程，使发现和调查数据变得更加容易和直观。<br />要深入了解所有最新功能，可以加入在 GrafanaCONline 举行的 Grafana 9.0 会议：[https://grafana.com/go/grafanaconline/2022/grafana-9-deep-dive/?pg=blog&plcmt=body-txt](https://grafana.com/go/grafanaconline/2022/grafana-9-deep-dive/?pg=blog&plcmt=body-txt)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125343-de84f4f9-6d77-4219-94f6-749c0c6f450e.png#averageHue=%23f8eee0&clientId=u08dd10a9-43ce-4&from=paste&id=u6dfacf7f&originHeight=608&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u89202661-a59e-47ef-8358-48377d4a327&title=)
<a name="OiRNk"></a>
## Visual Prometheus 查询生成器
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125368-f6bb69f3-cad3-4ce1-901a-b6181a5bfc4a.png#averageHue=%23e6e7e5&clientId=u08dd10a9-43ce-4&from=paste&id=u6f9c6a80&originHeight=601&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u965ea076-43ab-43a0-8cc0-12df2865a7a&title=)<br />尽管 PromQL 是一种强大的查询语言，但当涉及到编写查询时，它并不是最简单的，也不容易理解它们。对于 Prometheus 新手来说，开始编写查询可能是令人生畏的。<br />Prometheus 的新查询生成器正是为了解决这个问题而建立的。在 Grafana 9.0 中，你将在 Explore 中看到一个全新的可视化查询生成器界面，允许任何人编写、编辑和理解一个查询的作用。
<a name="psqIp"></a>
## 多种方式来编写查询
你已经熟悉的 Explore 界面现在增加了切换字段，可以选择在文本编辑模式（Code）或可视化生成器模式（Builder）中编写 PromQL 查询。当选择 Builder 模式时，一个新的可视化界面允许你通过多词搜索下拉菜单选择感兴趣的指标来制作你的查询。可以在这些模式之间进行切换，同时保留你的文字修改。<br />这个新的查询生成器允许你通过多词搜索来搜索和选择一个指标。可以从选择一个指标或一个标签过滤器开始。
<a name="TdwFF"></a>
## 对指标进行数学运算
Operations 字段用于通过各种函数、聚合和二进制操作对感兴趣的指标进行数学操作。可以通过 + Operation 按钮将这些操作分层。由于 Operations 是按照执行的顺序呈现的，而不是按照文本查询中的倒序排列，这使得阅读和编辑查询变得更加容易。
<a name="vgETE"></a>
## 通过应用内指南持续学习
如果你是 PromQL 的新手，可以使用第三种模式，即 Explain，通过应用内指南了解已经写好的查询。可以在 Builder 模式和 Explain 模式之间切换，同时保留查询，以了解更多关于被查询的指标和执行的操作。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125375-51413cff-a52f-4418-a30f-d051909b647d.png#averageHue=%23202228&clientId=u08dd10a9-43ce-4&from=paste&id=u969fce7a&originHeight=309&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubb963b50-88c7-42cd-a8d9-72869aded20&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125358-213ee3b5-2d2e-499b-b34a-38846d9372c6.png#averageHue=%23212329&clientId=u08dd10a9-43ce-4&from=paste&id=u51b4e7d4&originHeight=271&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb49e65f-eaa4-4ab3-9003-4b5a95c6342&title=)<br />新的可视化生成器也有被称为 "hints" 的建议，会适时提供正确操作的建议<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125374-3a56e612-6c6c-411d-8265-0dae072899c2.png#averageHue=%23202229&clientId=u08dd10a9-43ce-4&from=paste&id=ud67dbe44&originHeight=311&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u563415cc-a4d9-4b56-897b-2e70e31b24f&title=)
<a name="EKZS5"></a>
## Visual Grafana Loki 查询生成器
在 Grafana 9.0 中，编写 LogQL 查询也得到了一个可视化查询生成界面的帮助。在许多方面 LogQL 比 PromQL 更复杂，有更多的语法需要记忆。新的查询生成器将帮助你编写和理解 Loki 查询，而不需要记住任何语法。<br />在下图所示的 Loki 查询生成器中，可以添加和编辑标签过滤器、解析器和函数等。Loki 查询生成器支持上面列出的 Prometheus 查询生成器的所有功能，包括 Explain 模式，以及在 Code 模式和 Builder 模式之间切换的功能。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125662-32bd1e24-a7e6-44da-8339-7a3fe0301dfa.png#averageHue=%23202329&clientId=u08dd10a9-43ce-4&from=paste&id=udd8d6036&originHeight=315&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubcc98d64-46c5-4112-8aa3-87aab9531db&title=)
<a name="bezBp"></a>
## Explore-to-dashboard 工作流程
虽然 Grafana 一直支持从仪表盘移动到 Explore 而不丢失上下文的功能，但反过来却不行。<br />Grafana 9.0 公布了一个新的 Explore 到仪表盘的工作流程，允许你直接从 Explore 模式中创建面板或仪表盘。当一个复杂的查询起作用时，不再需要费力地复制它或重写它到一个新的仪表板。相反只需指示 Grafana，通过点击一个按钮（见下图），就可以直接从 Explore 创建一个新的面板 / 仪表盘或添加到一个现有的面板中。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125786-f55269d6-2c55-43b4-968f-e63e81cdf32b.png#averageHue=%2327292c&clientId=u08dd10a9-43ce-4&from=paste&id=u82712773&originHeight=561&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u45068efd-443c-4777-8677-ab9f25b8d34&title=)
<a name="Np7sq"></a>
## 新的 heatmap 面板
新的和修订的 heatmap 面板经历了架构上的变化，使其具有更高的性能（能够在超过 20 万个数据点上呈现时间序列），速度也提高了几个数量级。除了性能，heatmap 面板上的分辨率也更高，现在可以对色谱进行自定义和精细控制。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125847-44a72964-6bb8-4b43-a77c-143ad732e708.png#averageHue=%23daeba3&clientId=u08dd10a9-43ce-4&from=paste&id=ua2f12e83&originHeight=722&originWidth=1058&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub47175fb-cce7-4d6b-9eee-14d38c62e54&title=)
<a name="Bb2MG"></a>
## 命令面板
命令面板对于那些常用键盘快捷键的用户来说是一次很大的生产力提升。使用 cmd+K（macOS）或 ctrl+K（Linux/Windows），可以调出一个命令面板，使导航和仪表盘搜索更加容易。根据你在 Grafana 用户界面中的位置，可以快速运行一个查询、切换到分割视图、在仪表盘之间导航，或改变主题偏好。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125845-f756f582-2279-4850-80e0-e26610f63c46.png#averageHue=%23292a2e&clientId=u08dd10a9-43ce-4&from=paste&id=u9332d165&originHeight=671&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub2cfef35-c962-4ac4-99c4-c236b210aea&title=)
<a name="I7AUe"></a>
## 面板搜索
如果管理多个仪表盘和每个仪表盘下的许多不同的面板，搜索面板标题可以节省滚动仪表盘或在仪表盘之间切换以找到正确面板的时间。随着搜索功能的最新更新，现在可以按标题搜索面板。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705125897-2669513e-17e9-46c0-9296-6ac3419f17b7.png#averageHue=%2315161b&clientId=u08dd10a9-43ce-4&from=paste&id=u3bfbb7a6&originHeight=764&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u20385d10-3230-4c33-9409-dd33aed3a3c&title=)
<a name="ARPhF"></a>
## 仪表盘中的 Trace 面板
在 Grafana 9.0 中，现在可以在仪表盘中添加 Trace 面板，通过 Trace 视图来可视化，而不是在 Explore 模式中查看它们。这项功能目前在 Grafana 9.0 中处于测试阶段。
<a name="jL5XR"></a>
## 仪表盘预览
这个测试版功能提供了所有可用仪表盘的摘要概述，当名称不足时，可以快速找到需要的仪表盘。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1680705126206-84e98fe5-8bc5-4672-974c-8478533f6b2b.png#averageHue=%23212328&clientId=u08dd10a9-43ce-4&from=paste&id=u8ed127ad&originHeight=676&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc955426b-b74e-4d77-a9f6-3fc0a5efd13&title=)
<a name="ANO3Q"></a>
## 新的导航
扩大导航栏，以便更好地了解 Grafana 的功能和安装的集成。Grafana 9 还将引入一种方法，让你把你的仪表盘加注星标，并从导航菜单中轻松访问它们。可以通过打开 savedItems 功能来选择访问被标记的仪表盘。
<a name="MeGyt"></a>
## Grafana Alerting 的改进
在 v8.0 中，Grafana 引入了一种新的告警用户体验，以简化跨多个数据源和 Grafana 部署的告警创建和管理。在 Grafana 9.0 中，这是现在的默认设置，随着这一变化，Grafana 还进一步改善了告警体验 —— 特别是 UI 和文档。
