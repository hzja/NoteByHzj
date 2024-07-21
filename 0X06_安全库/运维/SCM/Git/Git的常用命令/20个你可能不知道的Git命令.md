Git<br />如果曾经浏览过git手册（或运行man git），那么会注意到git的内容比大多数人日常使用的多得多。这些命令中有很多是非常强大的，可以让你的生活变得更轻松（其他的则有点小众，但还是要知道的）。
<a name="H0zYs"></a>
## Git Web
运行 git instaweb 可以立即在 gitweb 中浏览你的工作存储库。<br />Git有一个内置的web-based visualiser的可视化工具，用于浏览本地仓库，通过浏览器的GUI来查看和管理仓库。它有很多有用的功能，包括。

- 浏览和浏览修订版，检查差异、文件内容和元数据
- 直观地查看提交日志、分支、目录、文件历史和附件数据
- 生成提交和版本库活动日志的RSS或Atom feeds
- 搜索提交、文件、更改和差异

要打开它，只需在版本库中运行`git instaweb`。浏览器应该弹出，并加载http://localhost:1234。如果没有安装Lighttpd，可以用`-d`标志指定一个替代的网络服务器。其他选项可以通过标记（比如`-p`表示端口，`-b`表示浏览器打开，等等），或者在git config中的[instaweb]块下配置。<br />还有`git gui`命令，它可以打开一个基于GUI的git应用<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673832391549-d9dae85d-1960-411c-ab75-61d848593342.png#averageHue=%231d2338&clientId=ua90aada2-9bc0-4&from=paste&id=u84d3033f&originHeight=636&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud5daf139-d940-4e8b-ba78-767acaf5935&title=)
<a name="bTyOx"></a>
## Git Notes
使用`git notes`为提交添加额外信息<br />有时您需要为一个 git 提交附加额外的数据（不仅仅是更改、消息、日期时间和作者信息）。<br />这些注释存储在 .git/refs/notes 中，由于它与提交对象数据分开，您可以随时修改与提交相关的注释，而不会改变 SHA-1 哈希值。<br />你可以用git log、大多数git GUI应用程序或`git notes show`命令来查看注释。一些git主机也在提交视图中显示注释（尽管GH不再显示注释）。
<a name="mgFI6"></a>
## Git Bisect
使用`git bisect`，可以通过二进制搜索找到引入错误的提交。<br />这是最强大但又最容易使用的git命令之一--当涉及到调试时，bisect绝对是个救星。启动bisect后，它为你检查提交，你告诉它该提交是好的（没有bug），还是坏的（引入了bug），这可以让你缩小出现bug的最早的提交。<br />要开始工作，先运行`git bisect start`，然后用`git bisect good <commit-hash>`传递一个已知的好的提交，用`git bisect bad <optional-hash>`传递一个已知的坏的提交（默认为当前）。然后它将检查好的和坏的提交之间的提交，然后用`git bisect good`或`git bisect bad`来指定错误是否存在。然后它将重复这个过程，在坏和好的中间检查出一个提交，一直到你找到引入该错误的确切提交。用`git bisect reset`随时取消。<br />bisect命令还有很多内容，包括重放、查看提交、跳过，所以下次调试的时候值得看看文档。
<a name="IPdtg"></a>
## Git Grep
使用`git grep`来搜索代码、文件、提交或其他任何东西，跨越你的 repo<br />有没有发现自己需要在git项目的任何地方搜索一个字符串？使用`git grep`，可以轻松地在整个项目中搜索任何字符串或RegEx，也可以跨分支搜索（就像一个更强大的Ctrl + F！）。
```
git grep <regexp><ref></p><p>
```
它包括很多选项来缩小搜索范围，或指定结果格式。例如，使用 `-l` 只返回文件名，`-c` 指定每个文件的匹配数量，`-e` 排除符合条件的结果，`--and` 指定多个条件，`-n` 用行号搜索。<br />由于`git grep`与regex兼容，可以对搜索的字符串进行更高级的处理。 也可以用它来指定文件的扩展名，比如`git grep 'console.log' *.js`会显示所有JavaScript文件中的console.logs。<br />第二个参数是ref，可以是分支名、提交、提交范围，或者其他任何东西。例如，`git grep "foo" HEAD~1` 会搜索前一个提交。
<a name="jvLm6"></a>
## Git Archive
使用git archive把整个版本库合并成一个文件<br />当分享或备份一个版本库时，通常倾向于将其存储为一个单一的文件。使用 `git archive` 将包括所有的 repo 历史，所以它可以很容易地被提取回它的原始形式。该命令还包括很多额外的选项，所以你可以准确地定制哪些文件包括在归档中，哪些不包括。
```bash
git archive --format=tar --output=./my-archive HEAD
```
<a name="w6RLS"></a>
## Git Submodules
使用`git submodule`将任何其他仓库拉入你的仓库<br />在git中，submodules 让你把一个版本库装入另一个版本库，通常用于核心依赖关系或把组件分割成独立的版本库。更多信息，请看这个帖子。<br />运行下面的命令将把一个模块拉到指定的位置，同时创建一个.gitmodules文件，这样当 repo 被克隆时就会一直下载它。使用 `--recursive` 标志，在克隆 repo 时包括子模块。
<a name="QnR6D"></a>
## Git Bug Report
使用`git bugreport`来编写一份bug ticket，包括git和系统信息<br />这个命令将捕获系统信息，然后打开一个标准的bug模板（重现步骤，实际+预期输出，等等）。完成的文件应该是一个非常完整的bug报告，包括所有必要的信息。<br />如果你是一个开源包的维护者，并要求用户（开发者）提出一个bug报告，这就非常方便了，因为它可以确保包括所有必要的数据。<br />如果你要向核心git系统提出一个bug报告，也可以运行`git diagnose`命令，然后在这里提出你的问题。
<a name="G9BLt"></a>
## Git Fsck
使用 `git fsck` 检查所有对象，或恢复无法到达的对象<br />虽然不是经常需要，但有时你可能需要验证git存储的对象。这就是fsck（或称文件系统检查）的作用，它测试对象数据库，验证所有对象的SHA-1 ID以及它们的连接。<br />它也可以和`--unreachable`标志一起使用，以找到不再能从任何命名的引用中到达的对象（因为与其他命令不同，它包括.git/objects中的所有内容）。
<a name="uIw4w"></a>
## Git Stripspace
使用`git stripspace`来格式化给定文件中的空白处<br />最好的做法是避免在行尾留白，避免出现多个连续的空行，避免在输入的开头和结尾出现空行，并以新行结束每个文件。有很多特定语言的工具可以自动做到这一点（比如prettier），但Git也有这个内置功能。<br />它主要用于元数据（提交信息、标签、分支描述等），但如果你用管道将文件输送给它，然后再将响应输送回文件，它也能发挥作用。例如，`cat ./path-to-file.txt | git stripspace` 或 `git stripspace < dirty-file.txt > clean-file.txt`<br />也可以用它来删除注释（用`--strip-comments`），甚至是注释行（用`--comment-lines`）。
<a name="RaZdc"></a>
## Git Diff
用 `git diff` 可以比较两组代码之间的差异<br />可以运行 `git diff` 来显示自上次提交以来的所有更改，或者使用 `git diff <commit-sha>` 来比较 2 个提交，或 1 个提交到 HEAD。但你可以用diff命令做的事情还有很多。<br />也可以用它来比较任意两个文件，比如 `diff file-1.txt file-2.txt` （不用再访问 diffchecker.com 了！）。<br />或者用`git diff branch1...branch2`来比较两个分支，或者相互参照。<br />注意，双点（...）与空格相同，表示diff输入应该是分支的顶端，但你也可以用三点（...）将第一个参数转换成两个diff输入之间共享的共同祖先提交的ref--非常有用 如果只想在不同分支间比较一个文件，只需将文件名作为第三个参数传入。<br />你可能想看某个日期范围内的所有改动，为此使用`git diff HEAD@{7.day.agree} HEAD@{0}`（上周），这也可以与文件名、分支名、特定提交或任何其他参数配对。<br />还有`git range-diff`命令，它提供了一个比较提交范围的简单接口。<br />`git diff`工具还有很多功能（以及使用自己的diff检查器的选项），所以建议去看看文档。
<a name="nmLNl"></a>
## Git Hooks
当一个给定的获取动作发生时，使用 hooks 来执行命令或运行脚本。<br />Hooks 可以让你实现几乎任何事情的自动化。例如：确保符合标准（提交消息、分支名称、补丁大小），代码质量（测试、lint），为提交附加额外信息（用户、设备、ticket  ID），调用webhook记录事件或运行管道，等等。<br />大多数git事件都有前钩和后钩，比如提交、重定位、合并、推送、更新、applypatch等。<br />钩子存储在.git/hooks中（除非用`git config core.hooksPath`在其他地方配置它们），并且可以用`git hook`命令来测试。由于它们只是shell文件，它们可以用来运行任何命令。<br />Hooks  不会被推送到远程仓库，所以要在团队中分享和管理它们，需要使用一个钩子管理器，比如lefthook或husky。也有一些第三方工具，使管理 hooks 更容易，推荐overcommit。<br />记住，hooks 总是可以被跳过的（用--no-verify标志），所以永远不要纯粹依赖钩子，特别是与安全有关的东西。
<a name="w3IJ0"></a>
## Git Blame
使用`git blame`来显示特定修订和行的作者信息<br />一个经典的方法，快速找出谁写了一行特定的代码（也就是你的同事要为这个错误负责！）。但它也可以用来确定在哪个时间点改变了什么，并检查该提交和相关元数据。<br />例如，要查看index.rs第400至420行的作者和提交信息，需要运行。
```bash
git blame -L 400,420 index.rs
```
<a name="ywOmA"></a>
## Git LFS
使用`git lfs`存储大文件，不拖累你的 repo<br />通常项目会包含较大的文件（如数据库、二进制资产、档案或媒体文件），这将拖慢git的工作流程并超出使用限制。这就是大文件存储的作用——它使你能够将这些大的资产存储在其他地方，同时保持它们在git中的可追踪性，并保持相同的访问控制/权限。LFS的工作原理是将这些大文件替换成文本指针，在git中进行跟踪。<br />要使用它，只需运行`git lfs track <file glob>`，它将更新你的.gitattributes文件。可以通过文件的扩展名（比如*.psd）、目录或单独指定文件。运行`git lfs ls-files`可以查看被追踪的LFS文件的列表。
<a name="izCCJ"></a>
## Git GC
使用 `git gc` 来优化你的版本库<br />随着时间的推移，git 仓库会积累各种类型的垃圾，这些垃圾会占用磁盘空间，并拖慢行动。这就是内置垃圾收集器的作用。运行git gc将删除无主的和不可访问的提交（用`git prune`），压缩文件修订和存储的git对象，以及其他一些一般的内务工作，如打包Refs、修剪reflog、revere metadata或陈旧的工作树和更新索引。<br />添加 `--aggressive` 标志将积极地优化版本库，丢弃任何现有的deltas并重新计算，这需要更长的运行时间，但如果有一个大的版本库，可能会需要。
<a name="T1uMm"></a>
## Git Show
使用 `git show` 可以轻松检查任何 git 对象。<br />输出对象（blob、树、标签或提交）以易于阅读的形式。要使用，只需运行 `git show <object>`。还可能希望附加 `--pretty` 标志，以获得更清晰的输出，但还有许多其他选项可以自定义输出（使用 `--format`），因此此命令对于显示您需要的内容非常有用。<br />一个很有用的例子是，在另一个分支中预览文件，而无需切换分支。只需运行 `git show branch:file`。
<a name="aL9Iu"></a>
## Git Describe
使用 `git describe` 找到一个提交中可触及的最新标签，并给它一个人类可读的名字<br />运行`git describe`，你会看到一个人类可读的字符串，它是由最后一个标签的名字和当前提交的内容组合而成的，生成一个字符串。你也可以向它传递一个特定的标签。<br />请注意，必须已经创建了标签才行，除非你附加了 `--all` 标志。Git describe 默认情况下只使用带注释的标签，所以你必须指定 `--tags` 标志，让它也使用轻量级标签。
<a name="zJt0u"></a>
## Git Tag
使用`git tag`标记你的版本库历史中的特定点<br />能够标记版本库历史上特定的、重要的点通常很有用，最常用来表示版本。创建一个标签就像`git tag <tagname>`一样简单，或者你可以用`git tag -a v4.2.0 <commit sha>`标记一个历史提交。和提交一样，您可以用 -m 在标签旁边加上一条信息。<br />别忘了用 `git push origin <tagname>` 把你的标签推送到远程。 要列出所有标签，只需运行`git tag`，并可选择使用-l进行通配符搜索。 然后你就可以用`git checkout <tagname>` 签出一个特定的标签。
<a name="ZARm4"></a>
## Git Reflog
使用`git reflog`列出你的 repo上的所有更新<br />Git 使用一种叫做参考日志，或 "reflogs "的机制来跟踪分支顶端的更新。各种事件被追踪，包括：克隆、拉、推、提交、签出和合并。能够找到一个事件的参考往往很有用，因为许多命令都接受参考作为参数。只要运行git reflog来查看HEAD上最近的事件。<br />reflog 非常有用的一件事是恢复丢失的提交。Git 从来不会丢失任何东西，即使是在重写历史的时候（比如重写或修正提交）。Reflog 允许你回到提交，即使它们没有被任何分支或标签所引用。<br />默认情况下 reflog 使用 HEAD（你当前的分支），但可以在任何 ref 上运行 reflog。例如 `git reflog show <branch name>`，或者用 `git reflog stash` 来查看隐藏的修改。或者用`git reflog show --all`来显示所有的引用。
<a name="Wc7DH"></a>
## Git Log
使用 `git log` 来查看提交列表<br />你可能已经很熟悉运行 `git log` 来查看当前分支上最近的提交列表了。但你还可以用 `git log` 做一些别的事情。<br />使用 `git log --graph --decorate --oneline` 会显示一个漂亮的、整齐的提交图以及 ref pointers。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673832391568-3de2eb68-49f6-46d0-9853-68b3888199c8.png#averageHue=%231f222f&clientId=ua90aada2-9bc0-4&from=paste&id=u19a82901&originHeight=281&originWidth=628&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7bbe5a61-b443-4586-8580-5497678a78c&title=)<br />你还经常需要能够根据各种参数来过滤日志，其中最有用的是。

- `git log --search="<anything>"` - 搜索特定代码修改的日志
- `git log --author="<pattern>"` - 只显示特定作者的日志
- `git log --grep="<pattern>"` - 使用搜索词或重组词过滤日志
- `git log <since>...<until>` - 显示两个引用之间的所有提交内容
- `git log -- <file>` -- 显示所有只对某一特定文件做出的提交

或者，只需运行`git shortlog`就可以得到一个夏季的提交列表。
<a name="kBBKl"></a>
## Git Cherry Pick
使用 `git cherry-pick` 来通过引用挑选指定的提交，并将其追加到工作的 HEAD 中。<br />有时你需要从其他地方拉出一个特定的提交，到你当前的分支。这在应用热修复、撤销修改、恢复丢失的提交以及某些团队协作中非常有用。需要注意的是，传统的合并通常是更好的做法，因为挑选提交会导致日志中出现重复的提交。<br />使用方法很简单，只需运行`git cherry-pick <commit-hash>`。这将把指定的提交拉到你的当前分支。
<a name="ySwYr"></a>
## Git Switch
使用`git switch`<br />在分支间移动是经常做的事情，switch 命令就像是 `git checkout` 的简化版，它可以用来创建和浏览不同的分支，但与 checkout 不同的是，在分支间移动时不会复制修改过的文件。<br />与 `checkout -b` 类似，switch 命令可以附加 `-c` 标志来创建一个新的分支，并直接跳入其中，例如 `git switch -c <新分支>`。而运行`git switch` - 会丢弃你所做的任何实验性修改，并返回到你之前的分支。
<a name="Z1NHN"></a>
## Git Standup
使用`git standup`来回忆在上一个工作日所做的事情，基于git提交的内容<br />把这个放在最后，因为它不包括在大多数git客户端中，但你可以用你的系统包管理器，用一个单行的curl脚本，或者从源码构建来轻松安装它。<br />如果老板要求每天做一个总结，对昨天的工作进行更新，但总是记不住到底做了什么——这个将显示一个格式良好的列表，列出在给定时间范围内所做的一切。使用方法很简单，只要运行`git standup`，或者使用这些选项来指定应该显示的数据（作者、时间范围、分支等。<br />原文：[https://leerob.substack.com/p/why-im-optimistic-about-javascripts](https://leerob.substack.com/p/why-im-optimistic-about-javascripts)
