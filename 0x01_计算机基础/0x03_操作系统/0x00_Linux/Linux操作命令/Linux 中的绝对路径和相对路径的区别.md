Linux<br />路径是Linux中最基本的概念之一，这是每个Linux用户都必须知道的。<br />路径是指文件和目录的引用方式。它给出了文件或目录在 Linux 目录结构中的位置，由名称和斜杠组成。
```bash
/home/abhishek/scripts
```
作为一个系统用户，当要访问某个文件或目录，或者必须为命令或脚本指定文件或目录的位置时，就会使用路径。
```bash
cat /home/abhishek/scripts/my_script.sh
```
请记住，如果路径以斜杠“/”开头，则第一个斜杠表示根，路径中的其余斜线只是分隔符。初学者经常混淆根斜杠和分隔符斜杠。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661906408830-99d597fe-c8bd-4153-981a-0dbdddb6bd67.png#clientId=u74163d9e-db7d-4&from=paste&id=u881d8229&originHeight=236&originWidth=562&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u649b76ec-768f-4b03-979e-91a9e9a1ebc&title=)<br />在上图中，第一个路径是以根路径（`/`）开始的，第二个路径不是以 `/` 开始的。这两种写法都是正确的，第一个是绝对路径，第二个是相对路径。
<a name="jkKnl"></a>
### Linux中的绝对路径和相对路径
绝对路径始终从根目录（`/`）开始，比如：
```bash
/home/abhishek/scripts/my_scripts.sh
```
相对路径从当前目录开始。比如，如果在 /home 目录中，希望访问 my_script.sh 文件，那么访问的路径应该是：
```bash
abhishek/scripts/my_scripts.sh
```
<a name="SLhba"></a>
### 理解绝对路径和相对路径之间的差异
都知道Linux中目录结构是一个树，从根（`/`）开始，然后产生分支。<br />假设现在目录abhishek中，并且希望访问文件my_scripts.sh，如下图所示两种路径的表示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661906408890-507b0742-fa76-4870-8cc7-8e8297566ef3.png#clientId=u74163d9e-db7d-4&from=paste&id=u0d2e1815&originHeight=467&originWidth=684&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5a3f37b9-2842-4058-831c-8d64530eb69&title=)<br />假设使用 ls 命令查看 my_script.sh 的文件信息，那么使用绝对路径：
```bash
ls -l /home/abhishek/scripts/my_script.sh
```
然后，相对路径：
```bash
ls -l scripts/my_script.sh
```
那么，以上两种方式，除了文件路径不一样以外，其他的完全一样：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661906408833-bff35da6-cbcc-4fa5-9182-a606de3452d1.png#clientId=u74163d9e-db7d-4&from=paste&id=uf4a3436d&originHeight=185&originWidth=988&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua951845d-108c-489c-ae00-7f62b7fdb1e&title=)<br />注：在Linux中，文件名区分大小写。可以有大写、小写、数字、点、中划线、下划线以及除了斜杠（`/`）之外的大多数字符。斜杠（`/`）作为保留字符，用于根目录和用于分割路径中的目录。
<a name="AlcF1"></a>
### 使用带有 `.` 和 `..` 目录的相对路径
有两种特殊的相对路径：

- `.`（一个点）：表示路径中的当前路径；
- `..`（两个点）：表示父目录，即上一级目录。

如下图，要从 abhishek 目录中，到 prakash 目录中去。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661906408845-2af310f0-0ff9-4524-b9a1-54ba3d0fd218.png#clientId=u74163d9e-db7d-4&from=paste&id=uceb1eb72&originHeight=459&originWidth=674&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7a59df1a-fccb-419a-a7a2-88c5d8ec778&title=)<br />切换目录，使用  cd 命令。如果使用绝对路径，可以这样：
```bash
cd /home/prakash
```
使用相对路径的话 ，需要用到特殊的相对路径 .. :
```bash
cd ../prakash
```
为什么要使用 .. 呢？因为相对路径要从当前目录开始，需要告诉 cd 命令向上一级，带到 /home 目录，然后从那里在进入到 prakash 目录。<br />绝对路径始终以 / 开始，并且与当前位置无关；相对路径取决于当前的位置。
<a name="AbKDO"></a>
### 绝对路径和相对路径，应该使用哪个呢？
老实说，这个问题没有直截了当的答案。这要视情况而定。<br />如果当前所在的目录层次结构比较深，并且需要向上或者向下一级移动，那么使用相对路径会更简单。<br />假设位于<br />/home/username/programming/project/interface/src/header 目录中，并且需要访问 /home/username/programming/project/interface/bin 目录中的某些内容。使用相对路径可以避免键入所有冗长的目录名，只需在此处使用.././bin即可。<br />但是，如果要从 <br />/home/username/programming/project/interface/src/header目录访问 /usr/bin 目录中的某些内容，那么使用 ../../../../../../usr/bin 会比较麻烦，这个时候使用绝对路径更方便。<br />另一种情况，是在脚本或者程序中使用路径。如果位置是确定的，那么就使用绝对路径；如果项目中有多个文件夹，并且需要在其中之间切换，那么此处需要使用相对路径，因为不确定最终用户会将程序放在什么地方。
