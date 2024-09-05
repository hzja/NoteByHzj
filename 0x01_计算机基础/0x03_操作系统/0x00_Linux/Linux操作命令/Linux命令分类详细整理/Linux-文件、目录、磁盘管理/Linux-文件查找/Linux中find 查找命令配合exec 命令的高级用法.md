Linux<br />Linux中的find 命令是根据给定条件查找文件和目录的常用命令、有很多的技巧。下面看一下使用exec配合使用的一些查找技巧、保证查找效率更上一层楼。
<a name="U0Cvf"></a>
## 将 `exec` 命令与文件命令的输出一起使用
用 `exec` 执行 `find` 的基本语法如下：
```bash
find [path] [arguments] -exec [command] {} \;
```
参数讲解:

- `[command]`是要对 find 命令给出的结果执行的操作命令。
- `{}`是一个占位符，用于保存 find 命令给出的结果。
- `\;` 表示对于每个找到的结果，都会执行[command]。需要转义成`\;`

注意: `{}` 和`\` 之间必须有空格；<br />还有另一种语法与上述略有不同，如下所示：
```bash
find [path] [arguments] -exec [command] {} +
```
在这里`+`表示对于 find 命令的每个结果，`[command]` 只执行一次。所有结果都作为参数一起传递给 `[command]`。`+` 不需要用`\+`转义。<br />`{} \;`表示为每个找到的结果执行该命令:
```bash
ls filetxt
ls filetxt
ls filetxt
```
`{} +`表示为所有结果参数执行一次命令
```bash
ls filetxt filetxt filetxt
```
虽然在这里使用`{} +`似乎是更好的选择，但如果 find 命令抛出 50 个结果，会导致参数过长出现错误。
<a name="Q8cuo"></a>
## find 和 exec 命令的实战案例
<a name="DG9De"></a>
### 1、查找并显示文件属性
显示 /tmp 目录下的所有lock文件并显示它们的属性。
```bash
sudo find /tmp/ -type f -name *lock -exec ls -l {} \;
```
**结果如下**
```bash
root@rumenz:~$ sudo find /tmp/ -type f -name *lock -exec ls -l {} \; 
-r--r--r-- 1 gdm gdm 11 Jul 17 08:01 /tmp/.X1024-lock
-r--r--r-- 1 gdm gdm 11 Jul 17 08:01 /tmp/.X1025-lock
```
<a name="JEJLE"></a>
### 2、查找和重命名文件
使用 find 和 `exec` 可以轻松重命名文件。
```bash
sudo find /home/sagar/Downloads/ -type f -name 'ubuntu*' -exec mv {} {}_renamed \;
```
上面的命令查找以名称 ubuntu 开头的文件并将它们存储在占位符`{}`中。一旦将结果存储在占位符中，它将在占位符中存储的每个文件的末尾添加_renamed。
<a name="Osu0m"></a>
### 3、收集和存储文件大小
将 /tmp 目录下每个文件的大小，并将输出保存在 /root 目录下，文件名为 rumenz.out
```bash
sudo find /tmp/ -type f -exec du -sh {} \; > /root/rumenz.out
```
<a name="ktlPa"></a>
### 4、删除带有特定参数的文件
自动删除文件时请格外小心。`rm -i`要么先查看 find 命令的结果。<br />find exec 命令组合的另一个常见示例是查找大于特定大小的文件并将其删除。比如：清理旧日志。<br />**删除大于 100 MB 的文件**
```bash
find ~/Desktop -size +100M -exec rm {} \;
```
同样，也可以根据文件的创建修改时间来删除文件。例如，删除超过 10 天的文件。
```bash
sudo find /tmp/ -type f -mtime +10 -exec rm {} \;
```
在这使用了 `-mtime` 来识别过去 24 小时内修改的数据，当与它配对 +10 时，它会查找并删除超过 10 天的文件。
<a name="S9akn"></a>
### 5、执行指定的命令
有些情况下，希望对查找到的文件执行某些命令。<br />例如当搜索任何 mp3 文件时，想运行 id3v2 它将带来有关找到的 mp3 文件的详细信息。
```bash
find . -name "*.mp3" -exec id3v2 -l {} \;
```
id3v2 是显示 mp3 文件详细信息的包，-l 用于显示与找到的 mp3 关联的每个 mp3 标签。
<a name="HMWQT"></a>
### 6、更改文件和目录的所有权
更改文件和目录的所有权，将用户deploy拥有的文件，改成rumenz
```bash
sudo find /home/sagar/disk/Downloads -user deploy -type f -exec chown rumenz {} \;
```
<a name="STxcT"></a>
### 7、更改文件的权限
那么如何使用 find 和 exec 更改文件的权限呢？
```bash
sudo find /home/sagar/disk/Downloads -type f -exec chmod 644 {} \;
```
在上面的命令中，使用了`-type f` 所以该命令只会应用于给定目录下的文件。
<a name="PuSac"></a>
### 8、查看每个文件的md5sum
如何查看/tmp目录下每个文件的md5值。
```bash
sudo find /tmp/ -type f -exec md5sum {} \;
```
可以将结果保存到指定文件中。
```bash
sudo find /tmp/ -type f -exec md5sum {} \; > /tmp/rumenz.out
```
<a name="jYPDl"></a>
### 9、结合`exec`和`grep`命令
下面的命令搜索所有具有.txt扩展名的文件。使用 grep它会在这些 .txt 文件的内容中搜索字符串excerpt。
```bash
find . -type f -name "*.txt" -exec grep -iH excerpt {} \;
```
使用该`-H`选项，grep 命令将显示每个匹配项的文件名。
```bash
root@rumenz# find . -type f -name "*.txt" -exec grep -iH excerpt {} \;
./author.txt:                <div class="post-card-excerpt">{{bio}}</div>
./partials/post-card.txt:            {{#if excerpt}}
./partials/post-card.txt:                <div class="post-card-excerpt">{{excerpt}}</div>
./post.txt:        {{#if custom_excerpt}}
./post.txt:            <p class="article-excerpt">{{custom_excerpt}}</p>
./tag.txt:                <div class="post-card-excerpt">
```
<a name="yIfAN"></a>
## 使用多个 `exec` 命令查找
如果想多次处理查找出来的文件，可以使用多个`exec`联合处理。
```bash
find . -type f -name "*.txt" -exec echo {} \; -exec grep excerpt {} \;
```
它将首先搜索 .txt 文件，然后使用第一个 `exec` 命令回显它们的名称。然后，将在这些文件中搜索excerpt字符串。
```bash
root@rumenz# find . -type f -name "*.txt" -exec echo {} \; -exec grep excerpt {} \;
./index.txt
./page.txt
./default.txt
./author.txt
```
