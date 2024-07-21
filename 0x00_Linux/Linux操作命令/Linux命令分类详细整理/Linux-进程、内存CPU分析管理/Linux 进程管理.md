Linux
<a name="PUN02"></a>
## 什么是 Linux 进程
理论上，一个进程被称为一个正在执行的程序，就是一个系统目前正在进行的任务。<br />在系统上的每一个动作都会导致一个新的进程。例如，打开一个浏览器就会启动一个进程。<br />简单地说，一个进程是一个程序的实例。用户的操作被转化为一个命令，在执行该命令时将会创建一个新的进程。<br />进程按照父-子层次结构工作。正如层次结构的名称所表示的那样，由命令/程序启动的进程被称为父进程，父进程产生的进程被称为子进程。
<a name="B2gaK"></a>
## Linux 进程的类型
在 Linux 发行版中，进程被分为两种类型：

- 前台进程
- 后台进程
<a name="J3nYq"></a>
### 前台进程
一个需要用户使用终端命令或程序来启动的进程被称为前台进程。这意味着前台进程需要一个来自用户的输入触发。所以每个前台进程都是手动触发的。<br />每当一个进程在前台运行时，其他进程应该等待，直到当前进程完成。<br />证明这一点的最好例子是通过 sleep 命令。sleep 命令不允许用户与终端互动，直到达到给定的时间。
```bash
sleep 10
```
终端命令在前台延迟 10 秒<br />![sleep 终端命令在前台运行并暂停用户输入](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458465-333601c0-7fab-40b1-963a-68f7cc3bf14f.png#averageHue=%231e2529&clientId=u17686ce1-b001-4&from=paste&id=uf6b217b4&originHeight=186&originWidth=507&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ubb391be5-4c09-45ac-b40f-5ed527b70f3&title=sleep%20%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4%E5%9C%A8%E5%89%8D%E5%8F%B0%E8%BF%90%E8%A1%8C%E5%B9%B6%E6%9A%82%E5%81%9C%E7%94%A8%E6%88%B7%E8%BE%93%E5%85%A5 "sleep 终端命令在前台运行并暂停用户输入")<br />应该等待 10 秒钟，以访问终端运行另一个命令。
<a name="Sk4Xb"></a>
### 后台进程
一个独立于用户输入运行的进程被称为后台进程。与前台进程不同，可以在一个后台进程中同时运行多个进程。<br />要在后台运行一个进程，在用来启动该进程的命令的结尾处放一个安培号（&）。<br />这里有一个例子：<br />在一个后台进程中执行 sleep 命令。它将在后台运行，并将终端还给用户以运行其他命令。<br />![后台进程的终端命令示例](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458490-b6d8c92e-f65a-4031-a57c-c9dacca7d5c9.png#averageHue=%231f262a&clientId=u17686ce1-b001-4&from=paste&id=u36d29cf1&originHeight=288&originWidth=584&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u48e8048d-bd2b-4d6e-a495-07713552c38&title=%E5%90%8E%E5%8F%B0%E8%BF%9B%E7%A8%8B%E7%9A%84%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4%E7%A4%BA%E4%BE%8B "后台进程的终端命令示例")
```bash
sleep 10 &
```
终端命令在后台延迟 10 秒<br />现在可以看到，上述命令在后台运行。它创建了一个 PID 19003 的进程。所以可以同时运行另一条命令（pwd 命令）。
<a name="XbsIk"></a>
## 如何将一个前台进程改为后台进程
如果在前台启动一个进程并想把它放到后台，可以用 bg 命令来实现。看看如何将前台进程改为后台。<br />如果一个进程正在运行，按 CTRL+Z 键。这个命令将暂停当前进程。<br />![前台进程输出](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458564-933bd7c7-fa0e-4fd1-a5d8-5839ea97cdc3.png#averageHue=%23262f33&clientId=u17686ce1-b001-4&from=paste&id=u9dc820f8&originHeight=319&originWidth=1048&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u15f9236f-6d36-4805-ac0f-356dbac4d69&title=%E5%89%8D%E5%8F%B0%E8%BF%9B%E7%A8%8B%E8%BE%93%E5%87%BA "前台进程输出")<br />然后运行 bg 命令。它接受一个进程 ID 作为参数，并将该进程放入后台。如果参数为空，它将把当前暂停的进程放入后台。
```bash
bg <process_id>
```
将进程移至后台的命令
```bash
bg
```
将上一个进程移到后台的命令<br />![前台进程向后台进程输出](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458554-29a6102a-ae5f-45cb-b921-cb4a5bb2bcdf.png#averageHue=%232b353a&clientId=u17686ce1-b001-4&from=paste&id=u92b7b6c5&originHeight=224&originWidth=649&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u451af8ed-4faf-453e-8d15-3292957f823&title=%E5%89%8D%E5%8F%B0%E8%BF%9B%E7%A8%8B%E5%90%91%E5%90%8E%E5%8F%B0%E8%BF%9B%E7%A8%8B%E8%BE%93%E5%87%BA "前台进程向后台进程输出")<br />可以看到暂停的命令（sudo apt update）在后台恢复了。
<a name="B66wQ"></a>
## 如何列出 Linux 进程
在介绍如何做到这一点之前，应该知道为什么可能需要知道一个进程的列表。这里有几个原因：

- 要知道哪个进程消耗的时间更多
- 要知道哪个进程占用更多的内存和 CPU
- 要知道一个正在运行的进程所触发的命令

要查看当前正在运行的进程，可以使用 ps（Process Status 进程状态）命令：
```bash
ps
```
列出正在运行的进程的终端命令<br />![ps 命令展示正在运行的进程的列表](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459286-1eb7bf2c-2b5d-4403-b2b1-31805d14a54b.png#averageHue=%23222a2e&clientId=u17686ce1-b001-4&from=paste&id=ua7d2879b&originHeight=133&originWidth=447&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua38ed4e3-062d-416a-93d2-bf20fd810c8&title=ps%20%E5%91%BD%E4%BB%A4%E5%B1%95%E7%A4%BA%E6%AD%A3%E5%9C%A8%E8%BF%90%E8%A1%8C%E7%9A%84%E8%BF%9B%E7%A8%8B%E7%9A%84%E5%88%97%E8%A1%A8 "ps 命令展示正在运行的进程的列表")<br />要列出每个登录用户的所有进程的摘要，可以使用 w 命令。这个命令是 Linux 中 who、uptime 和 ps -a 命令的结合。
```bash
w
```
列出所有登录用户的进程的终端命令<br />![w 命令显示所有用户的进程列表](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458859-5949cd10-9d8c-4513-b1bf-21dd0aef2f50.png#averageHue=%23222a2e&clientId=u17686ce1-b001-4&from=paste&id=uc289fe8d&originHeight=91&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua577f1e7-cb1d-4f41-8b2a-271d1b88468&title=w%20%E5%91%BD%E4%BB%A4%E6%98%BE%E7%A4%BA%E6%89%80%E6%9C%89%E7%94%A8%E6%88%B7%E7%9A%84%E8%BF%9B%E7%A8%8B%E5%88%97%E8%A1%A8 "w 命令显示所有用户的进程列表")
<a name="UxbjZ"></a>
## 如何以树状视图列出进程
当一个程序/命令运行时，它启动了一个被称为“父进程”的主进程。父进程可能依赖于其他一些命令/程序，这将创建一个子进程。<br />下面的截图是一个例子。<br />![父进程（firefox）的子进程](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035458973-7e740b2a-fe34-4505-921c-2f85ea237157.png#averageHue=%232c363c&clientId=u17686ce1-b001-4&from=paste&id=ud70c22ef&originHeight=258&originWidth=478&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u8e0e4b45-057f-43e2-ac4e-896e744f0b4&title=%E7%88%B6%E8%BF%9B%E7%A8%8B%EF%BC%88firefox%EF%BC%89%E7%9A%84%E5%AD%90%E8%BF%9B%E7%A8%8B "父进程（firefox）的子进程")<br />在上面的截图中，Firefox 是父进程，其他进程是其子进程。<br />来探讨一下如何以树状结构列出进程。<br />pstree 是一个 Linux 命令，用于以树状结构列出所有用户的当前运行进程。它被用作 ps 命令的一个更直观的替代。
```bash
pstree
```
将进程以树状结构列出的终端命令<br />![pstree 命令以树状结构列出所有进程](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459071-83bb9842-fb7b-4868-a817-51c1134cc7d3.png#averageHue=%23232b30&clientId=u17686ce1-b001-4&from=paste&id=uf7bd53bd&originHeight=412&originWidth=865&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ub52e3ed9-3248-4769-9d95-362d499f476&title=pstree%20%E5%91%BD%E4%BB%A4%E4%BB%A5%E6%A0%91%E7%8A%B6%E7%BB%93%E6%9E%84%E5%88%97%E5%87%BA%E6%89%80%E6%9C%89%E8%BF%9B%E7%A8%8B "pstree 命令以树状结构列出所有进程")<br />可以看到，正在运行的进程是以树状形式显示的。这对于可视化进程是很有用的。<br />在命令中添加 -p 标志将显示每个分支和其进程 ID。
```bash
pstree -p
```
以树状结构显示一个父进程的子进程的终端命令<br />![终端命令以树状视图显示带有 PID 的进程列表](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459192-adcdef49-8dd5-44d2-a881-ba135cb94852.png#averageHue=%23262f34&clientId=u17686ce1-b001-4&from=paste&id=u6d950069&originHeight=226&originWidth=641&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u1bba3e7e-a35e-4b83-9ded-74c047d09e5&title=%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4%E4%BB%A5%E6%A0%91%E7%8A%B6%E8%A7%86%E5%9B%BE%E6%98%BE%E7%A4%BA%E5%B8%A6%E6%9C%89%20PID%20%E7%9A%84%E8%BF%9B%E7%A8%8B%E5%88%97%E8%A1%A8 "终端命令以树状视图显示带有 PID 的进程列表")<br />要列出一个特定进程的子进程，请将进程 ID 作为参数传给 pstree 命令。
```bash
pstree 3149
```
列出进程 id 3149 的进程的终端命令<br />![将某一进程罗列为树状视图](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459413-d3aad859-85a8-407c-93cd-b0bf85a0c5c1.png#averageHue=%23283136&clientId=u17686ce1-b001-4&from=paste&id=u01833a98&originHeight=295&originWidth=509&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9c233a9e-e30c-437b-bdf0-cef166dd141&title=%E5%B0%86%E6%9F%90%E4%B8%80%E8%BF%9B%E7%A8%8B%E7%BD%97%E5%88%97%E4%B8%BA%E6%A0%91%E7%8A%B6%E8%A7%86%E5%9B%BE "将某一进程罗列为树状视图")<br />在上文中，提到 pstree 命令列出了所有用户的进程。在 pstree 命令中传递用户名，只列出该用户运行的进程。
```bash
pstree root
```
![以树状视图列出特定用户的进程](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459504-bc84cd44-44ec-41f8-90e4-080ee837f3cd.png#averageHue=%23232b30&clientId=u17686ce1-b001-4&from=paste&id=u5392e2e5&originHeight=428&originWidth=823&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ua1c94371-e2b4-4c39-aa8c-bc704266163&title=%E4%BB%A5%E6%A0%91%E7%8A%B6%E8%A7%86%E5%9B%BE%E5%88%97%E5%87%BA%E7%89%B9%E5%AE%9A%E7%94%A8%E6%88%B7%E7%9A%84%E8%BF%9B%E7%A8%8B "以树状视图列出特定用户的进程")<br />上面的截图显示了由 root 用户运行的进程。
<a name="pfQFe"></a>
## 如何查看某个程序的进程
很多开发者可能都遇到过以下情况：<br />在进行 web 开发项目时，使用 Chrome、Firefox 等不同的浏览器来验证输出。<br />有些开发者会不断地打开标签，而从不关闭已打开的标签。由于负载过重（如果打开了 150 多个标签），浏览器有时会没有反应😣导致系统挂起。最糟糕的是，将无法关闭浏览器😂。<br />与 Windows 不同，在 Linux 中没有任务管理器来杀死浏览器。<br />这个问题在 Linux 中也可以轻松解决。看看一个 Linux 专家是如何处理这种情况的。<br />每个程序（包括浏览器）都是作为一个进程运行的。所以，只需要找到进程的 ID，然后关掉它。<br />看看如何找到需要的一个命令/程序的进程 ID。<br />在系统中，Chrome 正在运行，现在可以通过运行以下命令得到 Chrome 的 PID：
```bash
pidof chrome
```
查找进程 ID 的终端命令<br />![查找 Chrome 进程 ID 的终端命令](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459493-7ab4c42d-d651-4b3a-b802-ca9326eb1d66.png#averageHue=%23242c31&clientId=u17686ce1-b001-4&from=paste&id=uebd5e8f2&originHeight=87&originWidth=489&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=ud66f0880-822a-4531-9e89-c5145981dfa&title=%E6%9F%A5%E6%89%BE%20Chrome%20%E8%BF%9B%E7%A8%8B%20ID%20%E7%9A%84%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4 "查找 Chrome 进程 ID 的终端命令")
<a name="Xef9a"></a>
## 如何杀死一个进程
在 Linux 中，有一个叫作 kill 的命令，通过传递 PID（进程 ID）或进程名称来杀死任何进程。<br />下面是 kill 命令的语法：
```bash
kill <pid/processname>
```
kill 命令的语法<br />存储 Chrome 的 PID，并使用 kill 命令杀死它：
```bash
a=$(pidof chrome)
kill $a
```
杀死一个进程的命令<br />![杀死一个进程的终端命令](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459691-7d294b12-a1b1-4628-9c08-0bc811054cef.png#averageHue=%2320282c&clientId=u17686ce1-b001-4&from=paste&id=ue5eaaa91&originHeight=108&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uc916bc4d-89a5-4181-9560-cad70969546&title=%E6%9D%80%E6%AD%BB%E4%B8%80%E4%B8%AA%E8%BF%9B%E7%A8%8B%E7%9A%84%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4 "杀死一个进程的终端命令")<br />上述命令将杀死 Chrome web 浏览器。
<a name="SuaBK"></a>
## 如何列出所有进程
可以使用 top 命令查看所有的 Linux 进程。它为所有用户显示每个进程的实时更新。
```
top
```
实时列出所有进程的命令<br />![实时显示所有进程的终端命令](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459900-8995254c-a691-4508-a079-d734f5571361.png#averageHue=%23313c42&clientId=u17686ce1-b001-4&from=paste&id=ud0042e25&originHeight=330&originWidth=805&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9721cb59-db61-4e53-a3c8-05bcdb23ed3&title=%E5%AE%9E%E6%97%B6%E6%98%BE%E7%A4%BA%E6%89%80%E6%9C%89%E8%BF%9B%E7%A8%8B%E7%9A%84%E7%BB%88%E7%AB%AF%E5%91%BD%E4%BB%A4 "实时显示所有进程的终端命令")<br />了解一下标题，以理解下方的数据。

- PID 代表一个唯一的进程 ID
- USER 代表任务所有者的用户名
- PR 代表该进程的优先级，数字越小，优先级越高
- NI 代表任务的优先级，一个负的 Nice 值意味着更高的优先级，一个正的 Nice 值意味着更低的优先级
- VIRT 代表任务使用的总虚拟内存
- RES 代表一个进程的 RAM 使用量，以千字节为单位
- SHR 代表一个进程使用的共享内存大小（Kb）
- S 代表进程的状态：
   - D：不间断的睡眠
   - R：运行
   - S：睡眠
   - T：追踪（已停止）
   - Z：僵尸
- CPU 代表 CPU 使用率
- MEM 代表任务的内存使用量
- TIME 代表 CPU 时间
- COMMAND 代表用于启动进程的命令

要显示特定的用户进程，应该使用标志 `-u`：
```bash
top -u <username>
```
列出特定用户进程的终端命令语法<br />要查看由用户 gogosoon 运行的进程，请运行以下命令：
```bash
top -u gogosoon
```
列出由用户 gogosoon 启动的进程的终端命令<br />![由用户 gogosoon 启动的进程的终端输出](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459955-840f9c76-2f4d-4b16-ac99-734934b90439.png#averageHue=%23303b41&clientId=u17686ce1-b001-4&from=paste&id=u759246de&originHeight=521&originWidth=817&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u9677c62c-42e5-42a9-a386-fa7c622e242&title=%E7%94%B1%E7%94%A8%E6%88%B7%20gogosoon%20%E5%90%AF%E5%8A%A8%E7%9A%84%E8%BF%9B%E7%A8%8B%E7%9A%84%E7%BB%88%E7%AB%AF%E8%BE%93%E5%87%BA "由用户 gogosoon 启动的进程的终端输出")<br />这对实时调试进程来说有点困难。<br />这里有一个方便的 GUI 工具来处理 Linux 中的进程。但必须手动安装这个工具。这将更像 Windows 中的任务管理器。
```bash
sudo apt install gnome-system-monitor
```
安装系统监控应用程序的终端命令<br />安装后，只需在终端输入软件的名称：
```
gnome-system-monitor
```
在 GUI 中打开进程列表的命令<br />这将在一个新的窗口中打开所有的进程，并有一个不错的 GUI：<br />![Gnome 系统监控](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035459938-06cada32-b00b-4e42-99e5-a1595746d608.png#averageHue=%23fcb962&clientId=u17686ce1-b001-4&from=paste&id=u1fb2e967&originHeight=206&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=uf908023a-c47f-4c09-b58e-b28c40676eb&title=Gnome%20%E7%B3%BB%E7%BB%9F%E7%9B%91%E6%8E%A7 "Gnome 系统监控")<br />当右键点击任何进程时，它将显示诸如杀死、停止、结束等操作。<br />资源选项卡显示以下工具：

- CPU 历史
- 内存和交换历史
- 网络历史

![CPU 历史记录](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035460090-eb91b6e7-4616-4564-b6dd-d786130f9e1e.png#averageHue=%23f3f5f9&clientId=u17686ce1-b001-4&from=paste&id=u7c42a715&originHeight=219&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u3c24761f-43e7-4008-a0f6-aed58d18a6e&title=CPU%20%E5%8E%86%E5%8F%B2%E8%AE%B0%E5%BD%95 "CPU 历史记录")<br />![内存和交换历史记录](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035460235-521d92bd-f12d-4e25-bc8b-d3d2a09214d5.png#averageHue=%23f5f7f9&clientId=u17686ce1-b001-4&from=paste&id=u1e50acf9&originHeight=183&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u2a877c8a-7752-43cf-bad7-1f62f3781db&title=%E5%86%85%E5%AD%98%E5%92%8C%E4%BA%A4%E6%8D%A2%E5%8E%86%E5%8F%B2%E8%AE%B0%E5%BD%95 "内存和交换历史记录")<br />![网络历史记录](https://cdn.nlark.com/yuque/0/2023/png/396745/1676035460445-592d861d-9b22-4d6f-a4de-9456ffedbacf.png#averageHue=%23f4f6f9&clientId=u17686ce1-b001-4&from=paste&id=u8d62426c&originHeight=186&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=true&status=done&style=none&taskId=u28c2532f-dc69-41e0-9696-44ba9bbea18&title=%E7%BD%91%E7%BB%9C%E5%8E%86%E5%8F%B2%E8%AE%B0%E5%BD%95 "网络历史记录")<br />这些图表对确定系统中的负载很有用。
<a name="G21U2"></a>
## 总结
在这里已经了解了 Linux 中进程的基本知识。能更好地理解它们是如何工作的。
