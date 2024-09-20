Linuxcrontab<br />crontab-ui 是一个用 Node.js 编写的 Web 前端，帮助管理 crontab 文件。<br />自动化是目前的一个热门话题。作为网站可靠性工程师（SRE）的日常工作中，部分职责是将尽可能多的重复性任务自动化。但是，有多少人在日常非工作生活中这样做呢？专注于将劳作自动化，以便可以专注于重要的事情。<br />作为一个初出茅庐的系统管理员，最早了解的东西之一是 “cron”。cron 被广泛用于做一些事情，如轮换日志、启动和停止服务、运行程序作业等等。它在几乎所有的 Unix 和 Linux 系统中都可用，cron 可以自动运行任何控制台应用或脚本，这使得它非常、非常灵活。
> 译注：CRON 是 “Command Run On” 的缩写，即在某个时间运行命令。

![Image of a Crontab](https://cdn.nlark.com/yuque/0/2022/png/396745/1652698582076-89ff8017-0687-4798-83be-de3c39fbcefa.png#clientId=u5e9a7425-e0e6-4&from=paste&id=ud9998af7&originHeight=635&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uc6594522-0824-4129-b1fd-af6c6d2cc89&title=Image%20of%20a%20Crontab "Image of a Crontab")<br />用 cron 来获取电子邮件，运行过滤程序，确保服务正在运行，与 Habitica 等在线游戏互动等。
<a name="n6yJ6"></a>
## 以传统方式使用 cron
要开始使用 cron，可以简单地在命令行输入 `crontab -e`，启动一个打开了当前 crontab（“cron table” 的缩写）文件的编辑器（如果以 root 身份这样做，访问的是系统 crontab）。这是保存作业计划的地方，记录了何时运行。
<a name="V3eHb"></a>
## crontab-ui简介
有一些奇妙的工具可以帮助解决这个问题crontab-ui是一个用 Node.js 编写的 Web 前端，可以帮助管理 crontab 文件。为了安装和启动 crontab-ui 供个人使用，使用了以下命令。
```bash
# 做个备份
crontab -l > $HOME/crontab-backup
# 安装 Crontab UI
npm install -g crontab-ui
# 创建本地数据库目录
mkdir $HOME/crontab-ui
# 启动 crontab-ui
CRON_DB_PATH=$HOME/crontab-ui crontab-ui
```
完成这些后，只需将网页浏览器指向 http://localhost:8000，就会看到 crontab-ui 的网页界面。要做的第一件事是点击 “从 Crontab 获取(Get from Crontab)”，加载可能有的任何现有作业。然后点击“备份(Backup)”，这样就可以回滚所做的任何修改。<br />![Image of Crontab-UI](https://cdn.nlark.com/yuque/0/2022/png/396745/1652698582066-90f0e1e5-cb64-4c8f-9031-527ed800a441.png#clientId=u5e9a7425-e0e6-4&from=paste&id=u61889d40&originHeight=1085&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf95a94e5-5655-41e0-832d-17f5b4957a4&title=Image%20of%20Crontab-UI "Image of Crontab-UI")<br />添加和编辑 cron 作业是非常简单的。添加一个名称，想运行的完整命令，以及时间（使用 cron 语法），然后保存。另外，还可以捕获日志，并设置将工作状态邮寄到选择的电子邮箱。<br />完成后，点击 “保存到 Crontab(Save to Crontab)”。<br />它的日志记录功能非常棒。有了 crontab-ui，可以通过点击一个按钮来查看日志，这在排除故障时非常有用。<br />建议不要一直运行 crontab-ui，至少不要公开运行。虽然它确实具有一些基本的身份验证功能，但它不应该暴露在本地机器之外。不需要经常编辑 cron 作业，所以可以按需启动和停止它。<br />下次需要编辑 crontab 时，可以试试 crontab-ui！
