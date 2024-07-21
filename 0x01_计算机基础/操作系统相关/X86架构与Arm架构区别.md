![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680673466469-3dd881c9-40df-4720-a621-f7fa90cdd70c.jpeg#averageHue=%2318a6fc&clientId=u2d2d8cfc-e882-4&from=paste&id=u608db9a1&originHeight=540&originWidth=1079&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue0d3aa82-9aa8-475f-9a05-fbab21e1008&title=)<br />X86架构和ARM架构是主流的两种CPU架构，X86架构的CPU是PC服务器行业的老大，ARM架构的CPU则是移动端的老大。X86架构和arm架构实际上就是CISC与RISC之间的区别，很多用户不理解它们两个之间到底有哪些区别，实际就是它们的领域不太相同，然后追求也不相同。
<a name="tBuuw"></a>
## X86架构和Arm架构区别
<a name="XcLKN"></a>
### 1、追求不同
X86主要追求性能，但会导致功耗大，不节能，而ARM则是追求节能，低功耗，但和X86相比性能较差。
<a name="HIdJj"></a>
### 2、领域不同
ARM主要应用于移动终端之中，类如手机，平板等，而X86则是主要应用于Intel，AMD等PC机，X86服务器中。
<a name="lzWFU"></a>
### 3、本质不同
X86采用CISC复杂指令集计算机，而ARM采用的是RISC精简指令集计算机。
<a name="qVVWU"></a>
### 4、CISC与RISC的不同
CISC是复杂指令集CPU，指令较多，因此使得CPU电路设计复杂，功耗大，但是对应编译器的设计简单。<br />RISC的精简指令集CPU，指令较少，功耗比较小，但编译器设计很复杂，它的关键在与流水线操作能在一个时钟周期完成多条指令。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680673466427-fcab75b9-aa4e-45f9-a9b2-70d34afe0a06.jpeg#averageHue=%23314249&clientId=u2d2d8cfc-e882-4&from=paste&id=u782a6dd4&originHeight=677&originWidth=894&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9bfcfd5f-8331-48da-96b6-e58573b3fe9&title=)
<a name="N6Dn9"></a>
## X86 VS ARM
X86架构（The X86 architecture）是微处理器执行的计算机语言指令集，指一个intel通用计算机系列的标准编号缩写，也标识一套通用的计算机指令集合。x86泛指一系列基于Intel 8086且向后兼容的中央处理器指令集架构。<br />Intel在早期以80x86这样的数字格式来命名处理器，包括Intel 8086、80186、80286、80386以及80486，由于以“86”作为结尾，因此其架构被称为“x86”。由于数字并不能作为注册商标，因此Intel及其竞争者均在新一代处理器使用可注册的名称，如奔腾（Pentium）、酷睿（Core）、锐龙（Ryzen，AMD推出）。<br />x86的32位架构一般又被称作IA-32，全名为“Intel Architecture, 32-bit”。其64位架构由AMD率先推出，并被称为“AMD64”。之后也被Intel采用，被其称为“Intel 64”。一般也被称作“x86-64”、“x64”。<br />值得注意的是，Intel也推出过IA-64架构，虽然名字上与“IA-32”相似，但两者完全不兼容，并不属于x86指令集架构家族。广义的x86架构, 泛指支持x86和x64架构的intel, amd的cpu, 但不包含ia64(安腾）<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1680673466509-7fac201f-60a1-4614-8258-f3f59df537df.jpeg#averageHue=%23e6e6d9&clientId=u2d2d8cfc-e882-4&from=paste&id=ucbd0acbd&originHeight=331&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u41fd4a94-19f6-42eb-bfdf-791e945f989&title=)<br />ARM架构，曾称进阶精简指令集机器（Advanced RISC Machine）更早称作Acorn RISC Machine，是一个32位精简指令集（RISC）处理器架构。还有基于ARM设计的派生产品，重要产品包括Marvell的XScale架构和德州仪器的OMAP系列。<br />ARM家族占比所有32位嵌入式处理器的75%，成为占全世界最多数的32位架构。<br />ARM处理器广泛使用在嵌入式系统设计，低耗电节能，非常适用移动通讯领域。消费性电子产品，例如可携式装置（PDA、移动电话、多媒体播放器、掌上型电子游戏，和计算机），电脑外设（硬盘、桌上型路由器），甚至导弹的弹载计算机等军用设施。<br />在数据中心需求增长的趋势下，核心芯片的角逐越演越烈。ARM已经进军服务器市场，ARM单核的面积仅为 X86 核的 1/7，同样芯片尺寸下可以继承更多核心数。通过“堆核”的方式，使得ARM架构处理器在性能快速提升下，也能保持较低的功耗。根据Ampere给出的数据，其CPU的性能超越传统x86处理器3倍，性能功耗比领先近4倍。与 x86 服务器CPU相比，Ampere Altra 系列可用50%的能耗，提供200%的性能。<br />近年来，ARM架构的的兴起下，许多巨头纷纷开始自研ARM架构的服务器芯片，包括国外的亚马逊、谷歌甚至微软，国内的腾讯、阿里巴巴、华为等等都积极参与其中。<br />曾经失败的高通似乎也在尝试再次进军服务器芯片市场，去年收购了芯片创业公司Nuvia，而Nuvia创立的目标是打造高性能的ARM服务器芯片。<br />ARM的服务器芯片有三大目标市场即云计算、HPC和边缘计算。
