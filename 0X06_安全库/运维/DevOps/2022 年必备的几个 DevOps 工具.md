DevOps<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429575-bfa652fd-75d5-45f2-8507-391c34c8a57c.webp#clientId=u27e64039-bfc7-4&from=paste&id=u92db5e3b&originHeight=155&originWidth=326&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6f31c1d1-4fb5-4edf-aed9-606d656ea4d&title=)<br />提到 DevOps 这个词，很多人一定不会陌生。作为一个热门的概念，DevOps近 年来频频出现在各大技术社区和媒体的文章中。到了 2020 年，DevOps 的革命也终于成为了一个主流，DevOps 相关工具的受欢迎程度也在激增。根据 Google 趋势，「DevOps 工具」的搜索量一直在稳定增长，并且这种趋势还在持续。<br />DevOps 工具越来越多，了解它们以及知道在什么时候使用他们越来越重要。由于 DevOps 涵盖了整个软件开发生命周期，因此有很多工具可供选择。将其大致分为以下几类：

- 开发和构建工具
- 自动化测试工具
- 部署工具
- 运行时 DevOps 工具
- 协作 DevOps 工具

成功且成熟地采用 DevOps 做法将始终拥有完整的渠道，其中包括适用于这五个类别的工具。确保评估当前的工具堆栈，以确保没有丢失 CI/CD 管道的关键部分。
<a name="l7TM3"></a>
## 开发和构建工具 
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429538-4a5b9fcf-2712-408a-9fed-6e87bd8993ec.webp#clientId=u27e64039-bfc7-4&from=paste&id=u73e47724&originHeight=409&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0ab06b63-ff01-4b7c-8efe-c51ce8b8965&title=)<br />这是 CI/CD 管道堆栈的基础。一切都从这里开始。该类别中最好的工具可以协调多个事件流，并可以轻松地与外部工具集成。<br />软件开发生命周期的这一部分中的工具分为三个子类别：

- 源代码控制管理（SCM）
- 持续集成（CI）
- 数据管理

2020年，推荐的SCM技术是GIT，因此SCM工具必须具有出色的GIT支持。对于CI，绝对需要在临时容器化环境中运行和执行构建的能力。对于数据管理，需要能够对数据库架构进行更改并使它与应用程序版本保持一致。
<a name="xOhzI"></a>
### 2020年排名第一的SCM + CI工具：Gitlab和Gitlab-CI
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429607-ae09f8b6-b1b8-4c99-9f28-678c99f55b16.webp#clientId=u27e64039-bfc7-4&from=paste&id=uf9f814ae&originHeight=400&originWidth=400&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub372ee09-a144-4292-82d0-db3fe0687d7&title=)<br />Gitlab无疑是2020年最伟大的DevOps Lifecycle工具，它将在可预见的未来成为创新的领导者。<br />Gitlab的核心功能提供了一个完美的GIT存储库管理工具。它基于Web的用户界面是最冗长且易于使用的。Gitlab的免费套餐可提供所需的一切，并且具有SaaS和On-Prem尺寸。<br />市场上有很多SCM工具，但是没有一种工具像Gitlab多年来所做的那样将“持续集成”直接集成到存储库中。称为Gitlab-CI，将.gitlab-ci.yml文件粘贴到代码库的根目录中，任何GIT事件都会根据在此处定义的内容触发操作。他们确实是按代码进行持续集成的领导者。
<a name="sV8qu"></a>
#### 主要优势：

- 成熟度 - 该产品自2013年以来一直投放市场，并且非常稳定并且得到了很好的支持。
- 开源 - Gitlab的免费版没有削减开发团队所需的核心功能。每个付费层都提供了附加功能，这些附加功能可根据组织的规模和需求带来极高的价值。
- 易用的 CI — 市场上没有其他工具可以像Gitlab-CI一样直接将持续集成直接嵌入到SCM中。使用Docker构建进行临时构建的能力提供了无忧的构建作业，并且内置的报告使调试构建失败变得容易。无需复杂的集成和业务流程就可以对多种工具进行编排。
- 无限集成 - Gitlab提供了每个核心DevOps类别中所需工具的轻松集成。这使开发人员和操作人员在任何环境中都可以使用一个真实的来源来获取与其应用程序相关的信息。
<a name="ba2no"></a>
#### 竞争对手：
还有其他工具在此领域也很流行，但是它们不如Gitlab。原因如下：

- GitHub - GitHub一直是小型和早期开发商店的出色SaaS源代码管理系统。但是，对于需要在网络中保留其IP的大型企业，GitHub的唯一选择是.OVA不支持高可用性的虚拟机。这使其难以维护on-prem，并且只能在中型组织中运行，然后服务器本身才开始崩溃。它缺少GitHub Actions（直到最近，但仍不在本地版本中）或CI-as-Code，这意味着始终需要带上自己的CI工具并管理该集成。最后，它比任何Gitlab定价都昂贵。
- Jenkins — 尽管Jenkins已经成为持续集成工具的默认标准，但它始终缺少源代码控制元素。意味着，将始终使用Jenkins 和 SCM工具。当像GitLab这样的工具同时提供这两种功能时，这简直是不必要的复杂。它可怕的UX使得现代Web应用程序有很多不足之处。
- BitBucket/Bamboo — 不得不说，这是一个自动失败者，考虑到需要两种工具来完成Gitlab的一项工作。尽管BitBucket云支持Gitlab-CI / GitHub Action功能，但没有一家公司（规模超过一家初创公司）可以轻易采用它。用于本地的 BitBucket服务器甚至不支持BitBucket管道！
<a name="pRuaO"></a>
### 2020年排名第一的数据管理工具：FlywayDB
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429654-7e7460a6-97b3-49f3-87ef-32eb1465c735.webp#clientId=u27e64039-bfc7-4&from=paste&id=ua4824b12&originHeight=430&originWidth=593&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u169398c1-c3fe-4f14-9ba3-c558274c3e2&title=)<br />Web应用程序开发中最容易被忽视的方面是数据库的自动化需求。在应用程序的新版本中部署数据库架构更改通常是事后的想法。模式更改通常会添加或重命名列或表。如果应用程序版本与架构版本不匹配，则该应用程序可能会完全损坏。由于存在两个不同的系统，因此通过应用程序升级来协调数据库更改也可能很困难。FlyWayDB解决了所有这些问题。
<a name="fDgEL"></a>
#### 主要优势：

- 数据库版本控制 - FlyWay允许简单地创建数据库版本，跟踪数据库迁移以及轻松地前滚或后退架构更改，而无需某些定制解决方案。
- 二进制或内置 - 可以选择在应用程序启动时或作为二进制可执行文件运行Flyway。在代码中使用此工具，以便它在启动时检查版本功能并运行适当的迁移，从而使数据库和应用程序版本保持同步。也可以临时运行cmd行，从而在不重建整个应用程序的情况下为现有数据库提供了灵活性。
<a name="UMpJM"></a>
#### 竞争对手：
这个空间中没有太多工具。但是，来看几个：

- LiquiBase — Liquibase是相似的。
- Flocker - 这可能仅适用于容器化的应用程序-在容器中运行数据库非常困难，必须精心计划才能成功执行。建议将RDS之类的服务用于数据库，而不要尝试运行存储在容器中的关键数据。
<a name="BzA4T"></a>
## 自动化测试工具
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429621-164dea03-b762-4508-8d4a-dcfb41e06a93.webp#clientId=u27e64039-bfc7-4&from=paste&id=u33dc0c3e&originHeight=239&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufe8a08ba-ef6f-4dab-9180-93e324bc577&title=)<br />必须首先将自动化工具安装到测试金字塔中，从而开始对自动测试工具进行评估。测试金字塔有4层：

- 单元测试 - 这是所有自动化测试的基础。就数量而言，与其他类型相比，应该拥有最多的单元测试。这些测试应由软件开发人员编写和运行，以确保应用程序的一部分（称为“单元”）符合其设计并按预期运行。
- 组件测试 — 组件测试的主要目的是验证测试对象的输入/输出行为。这样可以确保测试对象的功能按照所需规范正确运行。
- 集成测试 — 这是测试阶段，在此阶段中，各个软件模块被组合在一起并作为一个整体进行测试。
- 端到端测试 - 此层是不言自明的。

由于单元和组件层测试仅由应用程序开发人员驱动，并且通常是特定于编程语言的，因此不会在DevOps空间中评估这些工具。
<a name="OF4xR"></a>
### 2020年排名第一的集成测试工具：Cucumber
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429968-c3adf9b5-e195-4e5e-9a26-5cb2e027b556.webp#clientId=u27e64039-bfc7-4&from=paste&id=u8110a922&originHeight=400&originWidth=700&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=udfbca5dd-0257-48d7-9c21-e9f03111822&title=)<br />Cucumber将规范和测试文档合并为一个有凝聚力的有效文档。由于它们是由Cucumber自动测试的，因此规格始终是最新的。如果想开始构建Web自动化测试框架并在Web应用程序上模拟用户行为，那么带有Java和Cucumber BDD的Selenium WebDriver是在项目中学习和实现Cucumber的好方法。
<a name="fWuYL"></a>
#### 主要优势：

- 行为驱动的开发 — Cucumber用于BDD测试，它已成为一种入门测试框架（与传统的测试驱动开发相比）。
- 动态文档 - 记录所做的事情总是很痛苦。由于测试被定义为代码，因此Cucumber测试会自动生成文档以进行匹配以确保它们始终保持同步。
- 支持 - 这里有很多工具可供选择，但是当情况变得严重时，需要工具维护者的认真支持。黄瓜拥有足够的资金和支持结构来维持该工具的未来几年。
<a name="UIWe7"></a>
#### 竞争对手：
在这个领域中有许多框架和特定于技术的工具，但是只有Cucumber接近于“一刀切”的解决方案。
<a name="LWDr6"></a>
## 端到端测试工具
进行端到端测试时，有两个重点领域需要关注：

- 功能测试
- 负载测试

功能测试显然是在测试想要发生的事情是否实际发生。当单击SPA上的某些页面，填写表格并单击Submit时，数据将显示在数据库中，并且屏幕显示成功！<br />还需要能够测试在相同场景下工作的x数量的用户是否可以正确处理。<br />如果在这两个方面都没有进行测试，则CI / CD管道中的差距将很大。
<a name="t2tC5"></a>
### 2020年排名第一的端到端测试工具 — 功能：SoapUI Pro
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429878-83a455e0-9914-4966-8c42-a2266f4fe130.webp#clientId=u27e64039-bfc7-4&from=paste&id=u263d6bef&originHeight=400&originWidth=400&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e47b896-9cb9-40ec-ab15-6c832018038&title=)<br />由于SOAP Web服务是默认的，因此SoapUI进入API测试领域已有很长时间了。尽管不再构建新的SOAP服务，并且该工具的名称没有更改，但这并不意味着它没有发展。SoapUI为构建后端Web服务的自动化功能测试提供了一种出色的结构。这些可以轻松地与持续集成工具集成，并且可以作为CI / CD管道的一部分运行。
<a name="FJluT"></a>
#### 主要优势：

- 广泛的文档 - 此工具已经存在了一段时间，因此有许多在线资源可确定如何配置负载测试。
- 易于使用 — 尽管有多种API测试工具可用，但拥有一个用于多种服务的接口可以使构建测试变得简单。
<a name="j3E9r"></a>
#### 竞争对手：

- Selenium - Selenium是该领域的另一个出色工具。如果正在构建和运行基于Java的应用程序，则建议使用它。但是，如果要使用多种技术来处理一个完整的Web应用程序，那么对于非Java语言的用户来说可能会有些笨拙。
<a name="ZMF8X"></a>
### 2020年排名第一的端到端测试工具 — 负载测试：LoadRunner
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430001-1084cb4c-c109-4a73-9368-a805c7f0c230.webp#clientId=u27e64039-bfc7-4&from=paste&id=u70c383da&originHeight=225&originWidth=300&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub83a2586-c51c-468a-ba4b-4e089f5151f&title=)<br />说明：在对应用程序的各个方面进行负载测试时，只有LoadRunner才能完成。是的，这很昂贵而且入门有点困难，但是它是唯一可以执行测试的工具，可以作为技术架构师相信新代码将在极端压力下执行。另外，现在是时候让负载运行技巧从SQA资源转移到开发团队了。
<a name="UKaHU"></a>
#### 主要优势：

- 广泛的文档 - 该工具已经存在了一段时间，因此有许多在线资源可以确定如何配置负载测试。
- 协议支持 - 从ODBC到AJAX，再到HTTPS以及应用程序可能在某处使用的其他晦涩协议，LoadRunner支持该协议。要避免串接多个负载测试工具-这只会增加复杂性。
<a name="MuheQ"></a>
#### 击败竞争对手：
再说一次，在这个领域中没有太多的“一刀切”的工具，因此简单的解决方案是可以在任何环境中使用任何技术将其丢弃。
<a name="KAGSj"></a>
## 部署工具
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275429964-a0a91b01-9d7d-4c90-8a86-529cf41d2c3f.webp#clientId=u27e64039-bfc7-4&from=paste&id=u7ee50a90&originHeight=450&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ea43eb4-3aeb-4a34-9728-d9184ec96a9&title=)<br />部署工具可能是应用程序开发中鲜为人知的方面。对于操作人员来说，如果不深入了解应用程序代码和功能，就很难使用部署工具。对于开发人员来说，管理代码部署是一项新的职责，因此他们对许多部署工具的经验很少。<br />首先将部署工具分为三个子类别：

- 构件管理
- 配置管理
- 部署方式
<a name="Lk0WR"></a>
### 2020年排名第一的工件管理工具：Nexus
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430166-c6b9fa4a-f25a-4395-9aa8-bcd386483f92.webp#clientId=u27e64039-bfc7-4&from=paste&id=ua854c483&originHeight=241&originWidth=282&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u90b8f609-eae2-4120-9135-cce4d931bdd&title=)<br />Nexus工件存储库支持几乎所有主要技术，从Java到NPM再到Docker。可以使用这一工具来存储所有可部署的工件。通过使软件包更接近构建过程，代理远程软件包管理器的能力还大大提高了CI配置的速度。这样做的另一个好处是，可以全局查看跨多个软件项目使用的所有软件包，从而锁定不安全的开源软件包，这些软件包可能是代码中的攻击媒介。
<a name="WRVMK"></a>
#### 主要优势：

- 技术支持 - 该产品自2013年以来一直投放市场，并且非常稳定且得到了很好的支持。
- 开源 - Gitlab的免费版本没有削减开发团队需要的核心功能。每个付费层均提供附加功能，这些附加功能可带来最大价值，具体取决于组织的规模和需求。
<a name="WhEGN"></a>
### 2020年排名第一的配置管理工具：Ansible
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430370-b5a49341-5326-40f5-a581-ec94fc256490.webp#clientId=u27e64039-bfc7-4&from=paste&id=uc5b29b27&originHeight=394&originWidth=475&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u405d6429-7be4-4c0a-81bb-6f9273de315&title=)<br />Ansible是这个领域的领导者，原因很简单：无国籍。较早的现场配置管理工具着重于管理配置状态。如果它与所需的配置脱离同步，它将自行修复。在新的应用程序中，只有无状态组件。新版本的代码是新的构件，并已部署以替换现有的构件。拥有短暂的短暂环境。
<a name="ruQAK"></a>
#### 主要优势：

- 无状态 - Ansible剧本是从操作员机器上运行的，并命中服务器目标。不在乎远程对象的状态，这使得使用Packer之类的工具来构建可部署对象变得更加容易。
- 开源 - 和CentOS一样，Ansible也由RedHat维护。该企业及其高级支持人员可以帮助维护社区，并确保高质量，易于使用的模块。
- 分子测试 — 因为配置管理和其他任何东西一样都是代码，所以如果不对其进行测试，将无所不能。用于测试Ansible角色的分子框架可以无缝地工作，以确保按代码配置质量一样高，并遵循与应用程序代码相同的CI / CD管道。
- YAML — 与其他工具相比，YAML更加清晰。由于配置管理对于采用DevOps的任何人来说通常都是新事物，因此这使其成为关键卖点。
<a name="EUI42"></a>
#### 竞争对手：

- OpsCode Chef - 以厨师食谱开发人员的身份开始了DevOps生涯。但是，它们根本无法解决当今无状态，云原生应用程序的问题。对于更传统的遗留应用程序来说，这是一个很好的工具，但是本文将重点放在未来。
- Puppet — Puppet从未成长为一个庞大的社区，特别是与Chef and Ansible相比。它非常适合配置和裸机，但不支持Web应用程序类型的配置管理。
<a name="STJYd"></a>
### 2020年排名第一的部署工具：Terraform
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430302-d5e80a67-6b26-4f04-9cd8-1c694e87c6c1.webp#clientId=u27e64039-bfc7-4&from=paste&id=ucebe3174&originHeight=512&originWidth=546&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8a25ec66-f16d-4443-961a-d13ceee541e&title=)<br />Terraform解决了从网络组件到实际服务器映像定义基础架构即代码的问题。自最初发布以来，它已经走了很长一段路，并建立了庞大的插件社区和支持社区，以解决可能遇到的几乎所有部署场景。支持本地，云中或其他任何类型的环境的能力是首屈一指的。最后，最新版本在HCL中提供了许多与其他任何传统编程语言相同的逻辑功能和类，从而使开发人员可以轻松学习和学习。
<a name="bL3H5"></a>
#### 主要优势：
不受云/环境影响 - Terraform利用提供的资源作为Terraform代码与与基础架构提供商进行通信所需的所有API和后端逻辑之间的接口。这意味着可以学习一种工具，并且能够在任何地方工作。

- 开源 — 同样，很难敲响免费工具。社区支持是一流的。
<a name="nA72V"></a>
#### 竞争对手：

- AWS CloudFormation — 即使仅在AWS云环境中工作，也可能会在职业生涯中继续前进，而不是去那里。将技能和知识整合到一个平台中可能会有风险。此外，许多新的AWS服务通常在CloudFormation中可用之前作为Terraform模块提供。
<a name="YLDlo"></a>
## 运行时DevOps工具
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430296-bb98d9f0-77fb-4403-be7b-03ca529eed4e.webp#clientId=u27e64039-bfc7-4&from=paste&id=u9122f0bc&originHeight=174&originWidth=512&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub0600943-281f-4c96-9f8a-6eba9d6f54e&title=)<br />任何开发项目的最终目标都是在生产中运行应用程序。在DevOps世界中，希望确保对环境中的任何潜在问题具有可见性，并且还希望将人工干预降至最低。选择正确的运行时工具集对于实现开发必不可少的条件至关重要。<br />运行时工具子类别为：

- X 即服务
- 编排
- 监控方式
- 日志记录
<a name="FqZnT"></a>
### 2020年排名第一的X-as-a-Service工具：Amazon Web Services
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430595-61bc1584-dd6a-430f-a7a0-e66c45039553.webp#clientId=u27e64039-bfc7-4&from=paste&id=u35fd7497&originHeight=408&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uac10e065-83c0-47db-a137-d2aa45a0583&title=)<br />亚马逊一直是云计算领域的领导者。他们也不仅止步于此-他们为开发人员提供了许多新服务，以利用它可以旋转。将任何技术和任何模式带到AWS上，就可以构建和运行它。与在自己的数据中心中构建，管理和维护传统硬件相比，它们的成本极其合理。免费服务层使任何人都有机会在必须做出购买决定之前进行尝试，这对于尝试以正确的方式构建应用程序而不必因成本而造成损害非常有用。<br />主要好处：

- 行业标准 - 如果有在AWS中构建应用程序的经验，那么基本上可以在任何地方找到工作。企业喜欢AWS，而创业公司喜欢AWS的低成本。
- Free-Tier — 与其他所有功能相比，AWS的业务确实如此。使用该服务并查看其工作原理，然后再决定将数千美元投入可能有巨大陷阱的事物中。

竞争对手：

- Azure – 自最初发布以来，Azure已经走了很长一段路，值得称赞。但是，区分自身的需求已导致其对服务的名称进行了怪异的命名，而这些服务要难一点了-到底什么是“ blob存储”？尽管.NET代码在Microsoft生态系统中效果更好，但不太可能仅将.NET用于应用程序的各个方面。
- Heroku — 简而言之，除了在Heroku上的个人项目外。透明度不高，企业没有理由将其用作平台。这对于在博客中演示某些内容非常有用，但对于实际应用程序来说，非常感谢！
<a name="KxkM5"></a>
### 2020年排名第一的编排工具：OpenShift
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430637-ab15177b-476b-4f4c-8f19-d85653b018db.webp#clientId=u27e64039-bfc7-4&from=paste&id=u866fdd8f&originHeight=192&originWidth=192&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2cf43cb6-0cfc-4f0d-90c8-7a082a2d3d0&title=)<br />在应用程序堆栈中的某处使用了Docker或容器。无服务器应用程序很棒，但它们不能适合所有的架构模式。在没有业务流程平台的情况下运行容器根本行不通。从安全性和工具角度来看，Core Kubernetes带来了很多需求。OpenShift是唯一拥有Kubernetes平台的平台，它具有Source2Image构建，pod中的部署自动化以及甚至可追溯性和监视功能。它可以在本地，云中或同时在两者中运行。
<a name="ofjYF"></a>
#### 主要优势：

- 内置的安全性 - 管理K8安全性几乎需要博士学位。必须仔细考虑并考虑所有细节。默认情况下，OpenShift所采用的安全机制减少了开发人员的工作量，并为他们的应用程序提供了更安全的平台。
- 多合一解决方案 – 与默认不包含负载平衡工具的香草K8不同，OpenShift拥有一切。可以使用它来托管容器，构建容器，运行CI / CD工具，协调外部流程，管理机密等等。尽管GUI仍然需要做更多的工作，但API优先的方法意味着一切都可以编写脚本，并且与K8的其他GUI不同，它使学习Kubernetes的基础知识变得更加简单，而无需首先获得该学位！
<a name="pVfwg"></a>
#### 竞争对手：

- Docker Swarm - Docker swarm尝试通过删除大量内容来简化K8。这对于较小的应用程序非常有用，但对于企业应用程序则根本不起作用。此外，AWS ECS之类的服务采用了类似的方法，但是正在与之交互的其他服务（Lambda，IAM等）的使用变得更容易。
<a name="p3BV8"></a>
### 2020年排名第一的监控工具：New Relic
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430734-3cb8193f-4737-4ca8-a6d3-c0fc05409d57.webp#clientId=u27e64039-bfc7-4&from=paste&id=ue3f4fd5c&originHeight=366&originWidth=451&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5e79d9a-9446-43c8-89e5-11fd80e5c11&title=)<br />New Relic的早期发行版确实做得非常好-APM监视。现在，它是一套完整的监视工具，可以监视服务器性能，容器性能，数据库性能，最终用户体验监视，当然还有APM监视。
<a name="wCV5q"></a>
#### 主要优势：

- 易用性 - 在担任系统工程师时曾使用过许多监视工具，但从未遇到过像New Relic这样易于使用的监视工具。这是一个SaaS，因此不必设置服务器组件也很不错。
- 端到端可见性 - 其他工具尝试监视应用程序的一个特定方面。无论是CPU利用率还是网络流量，所有这些层都可以协同工作，以使应用正常运行。New Relic能够组合所有数据以真实了解正在发生的事情。
<a name="nIobI"></a>
#### 竞争对手：

- Zabbix — Zabbix由于缺乏向云原生环境和APM空间发展的能力，因此使其滞后。它仍然可以很好地监视传统的服务器基础结构，仅此而已。
- DataDog - 此工具过于侧重于管理生产应用程序的过程视角，而对代码本身的关注不足。在真正的DevOps团队中，有开发人员参与生产，无需依靠繁琐的工具来提供世界一流的支持。
<a name="lCxG7"></a>
### 2020年排名第一的测井工具：Splunk
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430933-b4356870-23c8-4290-a770-5ffabea1de32.webp#clientId=u27e64039-bfc7-4&from=paste&id=u7844c4a1&originHeight=184&originWidth=363&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u105ce2af-db9b-4742-a5af-f80919ed9bc&title=)<br />很难反对Splunk。他们很久以来一直是日志聚合的领导者，并且他们继续做得最好。借助本地和SaaS产品，可以在任何地方使用它。主要的缺点是，它仍然很难运行！
<a name="vHiyu"></a>
#### 主要优势：

- 行业标准 —企业喜欢Splunk，他们也有钱为此付出代价。虽然初创企业可能难以证明其成本合理，但许多概念和技能可以转移到开源替代方案中。
- 可支持性 -简单地说，它可以正常工作。它具有许多默认值和即用型功能，因此不必花费大量时间阅读文档并尝试使一些没有明确说明的内容能够正常工作。
<a name="uzi4A"></a>
#### 竞争对手：

- ELK Stack - ElasticSearch，LogStash和Kibana，虽然似乎总是很酷，因为它们不收取使用费用，但随着日志集的增长和机上越来越多的应用程序的维护，它的确变得更加困难。与使用Splunk相比，在构建任何类型的仪表板之前花了更多的时间来设置工具。
<a name="hdjkN"></a>
## 协作 DevOps工具
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275430937-9450c95e-e26f-4324-9b64-ce6157c07b69.webp#clientId=u27e64039-bfc7-4&from=paste&id=ue8da2f62&originHeight=565&originWidth=800&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u98d8b693-e59d-418f-91e9-34e76963405&title=)<br />DevOps首先是组织内部的文化变革。虽然购买工具不会一夜之间改变文化，但无疑可以帮助培养与同事合作的新方法。<br />协作工具子类别为：

- 问题跟踪
- 聊天操作
- 文献资料
<a name="tTThy"></a>
### 2020年排名第一的问题跟踪工具：Jira
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275431068-5054ad0c-ae77-4e5f-8993-9704e4882574.webp#clientId=u27e64039-bfc7-4&from=paste&id=u166c1347&originHeight=388&originWidth=580&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufdd700bd-6131-4bf9-b3e7-cdfc0ad58de&title=)<br />吉拉（Jira）继续保持头把交椅，尽管在这个领域竞争日益激烈。但是，Jira内置的强大灵活性使开发团队和运营团队可以管理其项目工作和冲刺任务。使用敏捷术语的内置标准有助于缓解从传统工作方法到更精益流程的文化转变。
<a name="wLmYm"></a>
#### 主要优势：

- 行业标准 — 同样，就像许多工具一样，Jira到处都有使用。小型团队可以使用便宜的许可证并获得所需的一切，而企业可以为任何人负担得起许可证。
- 集成 - 在这个领域处于领先地位并且快速增长意味着第三方工具会选择首先构建本机集成，而它们只会增加工具的价值，而Jira就是这种情况。可以与现成的列表中的所有其他工具集成，而无需进行任何定制。
<a name="h31c8"></a>
#### 竞争对手：

- Trello — Trello成为免费使用的看板工具，因此迅速流行。但是，一旦事情开始扩展，并且从数十个问题扩展到数千个问题，Trello将变得难以导航，搜索和报告。
- Pivotal Tracker -他们更多地关注产品管理，而不是技术任务。尽管从Jira进行产品管理比较困难，但是仍然可以完成此过程，而不必获取完全独立的工具。
<a name="EQkqD"></a>
### 2020年排名第一的ChatOps工具：MatterMost
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275431152-450cefe5-c1a1-4eef-b488-d3c78b39044e.webp#clientId=u27e64039-bfc7-4&from=paste&id=u57f86471&originHeight=200&originWidth=200&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u75c3e0f7-1246-43db-8d61-8352c1ba82c&title=)<br />说明：这可能是2020年这份清单上最大的惊喜，这是一件好事！MatterMost通过使用以前最好的工具，但引入了本地部署而获得了普及。对于企业来说，这是巨大的，因为它可以控制数据，还可以帮助与本地工具集成-不再需要为了新的事物而走出防火墙。
<a name="yZczi"></a>
#### 主要优势：

- 开源 - MatterMost的开源版本非常适合小型或大型团队。与Slack的免费层会丢失历史记录不同，自己运行服务器意味着拥有数据。
- 集成 - 因为API几乎100％基于Slack API，所以几乎所有Slacks集成都可以直接与MatterMost一起使用。
<a name="wsS5J"></a>
#### 竞争对手：

- Slack - 松弛很棒，但是它们已经变得如此庞大，需要开始获利。他们业务的付费阶段即将到来，并且剥夺了Slack用来免费提供的许多价值，最关键的是聊天记录。
- Microsoft Teams - 尝试将Microsoft产品与非Microsoft本地产品集成！
<a name="Z6zLb"></a>
### 2020年排名第一的文档工具：Confluence
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1645275431427-fcaa49bf-1c7e-4203-91a1-9bc3f3a22d5e.webp#clientId=u27e64039-bfc7-4&from=paste&id=u3a048b76&originHeight=256&originWidth=256&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u56f86677-24ba-409d-a37e-2b221594a46&title=)<br />无论使用哪种工具，都很难创建和维护高质量的技术文档。尽管最近有许多SaaS文档工具进入市场，但很难接受将有关关键应用程序的敏感技术文档存储给第三方。需要将数据和文档保留在本地，这就是Confluence所做的。<br />主要优势：

- 易于管理 - 大多数自托管工具的启动和运行可能有些复杂，并且大规模维护它们需要一些特定知识。开箱即用的Confluence服务器非常适合10个用户或10,000个用户。
- 插件－ 尽管创建具有默认融合功能的漂亮且易于浏览的文档已经很不错了，但是拥有用于几乎所有内容的插件的能力释放了Wiki的潜力。

竞争对手：

- Read the docs — 非常适合开源公共代码，但永远不会考虑在这里存储关键的应用程序知识。
- MarkDown — 尽管非常适合于记录有关代码的内容，但很难将体系结构，过程或其他类型的文档直接放入MarkDown格式中。
- Jekyll — 在记录技术知识时，并不想简单地构建一个新的静态站点，以便在每次更改时进行部署。简单的Confluence版本管理系统使内部文档的处理变得更加容易。
<a name="h1Jve"></a>
## 总结 2020 年最佳
市场上实际上有数百种DevOps工具。试图浏览应使用哪些以及何时实施它们可能会令人不知所措。遵循此简单指南，为完整的CI / CD管道选择DevOps工具堆栈。<br />将工具分为以下五个关键领域：

- 开发和构建工具
- 自动化测试工具
- 部署工具
- 运行时工具
- 协作工具
