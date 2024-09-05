Linux
<a name="MfF2i"></a>
## 1、查看文件校验值
在文件进行拷贝或者进行传输的时候，可能有损坏或者被修改的可能，这时候可以查看校验值来确认一下。<br />比如平时工作需要用到其它组提供的一些对接的程序，每次程序运行不符合他们的预期的时候，都会对一下两边的md5校验值。<br />生成文件的校验值的方法有很多种，常用的有md5sum校验、crc校验、sum校验等。<br />命令分别为：
```bash
md5sum file_name
cksum file_name
sum 算法参数 file_name
```
例如：<br />以一个test.txt文件为例：

- md5sum校验
```bash
md5sum test.txt
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294224-7f493121-f031-4089-b7a5-44ece17fa06b.png#averageHue=%231b0b0a&clientId=u226e303e-1a56-4&from=paste&id=u6c273849&originHeight=229&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3140969f-a050-410e-9ef1-8f244edeba4&title=)

- crc校验
```bash
cksum test.txt
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294250-b2386f27-fa96-465f-830e-41eb7914c838.png#averageHue=%231a0b0a&clientId=u226e303e-1a56-4&from=paste&id=uc085c18e&originHeight=231&originWidth=662&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ff2d934-bbae-4892-9476-ed108b5253d&title=)

- sum校验

sum校验有两种算法，可以通过参数进行配置：<br />`-r`：表示使用system v算法。<br />`-s`：表示使用BSD算法。<br />不进行配置时，默认用的是system v算法。
```bash
sum -r test.txt
sum -s test.txt
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294336-c8e1fe0d-caad-4ba0-aa6a-1b6af14060e5.png#averageHue=%231b0c0a&clientId=u226e303e-1a56-4&from=paste&id=ue748dee6&originHeight=232&originWidth=663&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9f7f5334-ccb4-46f6-a822-0004a86767c&title=)
<a name="udDdA"></a>
## 2、查找文件位置
<a name="YMay6"></a>
### （1）locate
查找文件大家一般都习惯用find吧，但有时候locate更快一些，所以一般都会先使用locate。<br />locate 与 find 不同：find 是去硬盘找，locate 只在 /var/lib/slocate 资料库中找。locate 的速度比 find 快，它并不是真的查找，而是查数据库。<br />有些系统可能不带有locate，需要自己安装。比如，Ubuntu可以输入如下命令进行安装：
```bash
apt-get update
apt-get install mlocate
```
locate查找文件的命令很简单：
```bash
lcoate file_name
```
比如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294335-215a1df3-7d64-4f80-9a68-e772d811ac70.png#averageHue=%23100604&clientId=u226e303e-1a56-4&from=paste&id=u05624bff&originHeight=327&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1f2b35a2-a8a4-49c9-a635-0198dd4a121&title=)
<a name="lY1BT"></a>
### （2）find
find命令可以用名字、类型、所属人、大小等来进行搜索。<br />搜索文件基本语法：
```bash
find path -option file_name
```
如使用名字来搜索stdio.h文件：
```bash
find / -name stdio.h
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294209-09ebb318-a0ff-4d8e-ae4c-b51fbb0970a0.png#averageHue=%23190a08&clientId=u226e303e-1a56-4&from=paste&id=u535f0fda&originHeight=211&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u08170b07-4cfc-4632-b534-eeb3afedb37&title=)
<a name="qNg9E"></a>
## 3、命令行编辑技巧
在终端里误输入了一些比较长的内容：
```bash
LinuxZn@LinuxZn:~$ dsfdsfdddddddddddddddddddddddddddddddddddfsgadgdsgasdgsdhfdkshfkjdshflksdhfkldshfkj
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294669-eb8aa893-9692-444f-a9a9-f87ac89812c8.png#averageHue=%23180b0a&clientId=u226e303e-1a56-4&from=paste&id=u953fe83c&originHeight=269&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69f337eb-e1bc-480f-ab66-892bc2e6754&title=)<br />怎么比较快的删除掉呢？疯狂地按退格键当然可以达到目的。但是有更快速的方法：<br />输入快捷键 **ctrl+u** 即可把光标前面的内容全删掉。除此之外，还有如下几个实用且常用的快捷键：

- ctrl+k：把光标后面的内容全删掉。
- ctrl+a：光标移到开头处。
- ctrl+e：光标移动到末尾处。

除此之外，命令行还有很多实用常用、实用不常用的快捷方式，感兴趣的小伙伴可以自己去学习。
<a name="gJAL6"></a>
## 4、查看某个进程的pid
命令：
```bash
pidof process_name
```
如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294916-62774be9-13bc-46ab-abc7-3216fb00df7f.png#averageHue=%231f0504&clientId=u226e303e-1a56-4&from=paste&id=u8bb4a2b7&originHeight=144&originWidth=344&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u57824203-1311-4bbb-af03-84e1115bd36&title=)
<a name="gRTB5"></a>
## 5、查看某些进程的一些运行情况
top命令可以查看进程的一些信息，但是系统运行的进程过多，不利于查看某些进程的运行情况，如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294801-4390c3eb-783a-41f1-a300-5795478031fd.png#averageHue=%230b0705&clientId=u226e303e-1a56-4&from=paste&id=u83a5e512&originHeight=1055&originWidth=1049&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uacaae85f-f8c9-4ed3-86b5-1d279edaf6a&title=)<br />这时候可以通过如下命令查看指定进程的运行情况，例如：<br />查看kcalc进程的情况，命令：
```bash
top -p `pidof kcalc`
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646294831-79b48418-f53e-4f79-82cf-cdd64e9e9775.png#averageHue=%23170c0b&clientId=u226e303e-1a56-4&from=paste&id=u312b1c11&originHeight=330&originWidth=883&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u18e0322e-f63f-4c57-8bee-4674df9e6c6&title=)<br />这就简洁多了。
> **注意：**
> 这里的"`号"并不是单引号！！！
> 这里的"`号"并不是单引号！！！
> 这里的"`号"并不是单引号！！！

这个符号在键盘上感叹号!键的左边。<br />查看多个进程，如：
```bash
top -p `pidof kcalc` -p `pidof test_x86`
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295013-8b44c273-f162-4f45-953c-cf4357b12be2.png#averageHue=%231c0f0d&clientId=u226e303e-1a56-4&from=paste&id=u58a1189e&originHeight=286&originWidth=826&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u19648739-c70f-4402-95ac-e5ba2e165de&title=)
<a name="GijgX"></a>
## 6、杀死进程
<a name="kufAY"></a>
### （1）使用kill
先使用pidof查看进程的pid，然后再使用kill命令：
```bash
kill -9 process_pid
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295386-93903fa5-a482-42d5-88bd-af51dd65499c.png#averageHue=%23170404&clientId=u226e303e-1a56-4&from=paste&id=u6989848c&originHeight=195&originWidth=383&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub59d3fbe-6070-4d58-9cfa-cd7c6e3abc5&title=)
<a name="YbRQ5"></a>
### （2）使用killall
使用killall，命令：
```bash
killall process_name
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295462-5d8c8980-7984-4231-a5cb-559b1331993c.png#averageHue=%23190303&clientId=u226e303e-1a56-4&from=paste&id=u752d02b0&originHeight=170&originWidth=437&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud783dec6-4dab-4a69-a526-ad25456cced&title=)
<a name="P4tSf"></a>
## 7、终端输出的log同时保存到文件
有时候需要把终端实时输出的log信息保存到文件中，有如下两种方法：
<a name="Ila3m"></a>
### （1）tee
**tee工具** 用于将数据重定向到文件，另一方面还可以提供一份重定向数据的副本作为后续命令的stdin。简单的说就是 **把数据重定向到给定文件和屏幕上** 。<br />命令：
```bash
executable_file | tee log_file
```
演示如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295515-b6679a04-6cae-4c8d-8d67-9caadd6ffdbb.png#averageHue=%2388b86a&clientId=u226e303e-1a56-4&from=paste&id=ued9288b7&originHeight=296&originWidth=1014&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6604e8ed-d906-4e6f-8fbc-e57e1693309&title=)
<a name="kZB0Q"></a>
### （2）script
**script工具** 是一个非常使用的工具，可以把输出到终端的信息记录下来。使用步骤如：

- 输入  **script log.txt** 命令开始保存终端输出的信息  ，其中log.txt为需要写入的log文件，可随意命名。
- 输入 **exit** 退出保存。

演示如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295576-43978142-560f-474d-bb9d-3cbbe41e5278.png#averageHue=%2388af65&clientId=u226e303e-1a56-4&from=paste&id=u9aa2782f&originHeight=368&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubcb5c21a-9968-4129-b669-b18139ce79e&title=)
<a name="WsUnl"></a>
### （3）使用一些可以保存log的终端工具
一些终端工具具有记录日志的功能，比如T **Terminator终端** ：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646295635-d73f64c3-3592-4bf0-8843-9cf4afe3282a.png#averageHue=%230e0c0b&clientId=u226e303e-1a56-4&from=paste&id=u8d1e1242&originHeight=636&originWidth=762&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6ee20caf-e771-4bb9-9cf1-e2386c3d653&title=)
<a name="r9MKF"></a>
## 8、查看程序依赖的动态库
有些程序依赖一些动态库，可以通过ldd命令查看依赖的动态库。命令：
```bash
ldd executable_file
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296033-1bebd111-723c-4420-b733-a8fef1080cc1.png#averageHue=%23525f96&clientId=u226e303e-1a56-4&from=paste&id=ub5ffdb07&originHeight=311&originWidth=793&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69399635-f3aa-4736-bc4d-8336960e768&title=)
<a name="L78sC"></a>
## 9、查看ELF文件头
ELF文件有几种，之前刚来的一位应届生，编译了一份程序，编译没报错，但是一直运行不起来。然后在PC上运行有问题，报错如：
```
无法执行二进制文件: 可执行文件格式错误
```
原因是他那份工程里设置了使用交叉编译器进行编译，但是他却在PC运行，所以就报错了。<br />可以可执行文件的ELF头，ELF头包含了很多信息，其中就包括有系统架构这一项。命令如：
```bash
readelf -h elf_file
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296033-165233c2-b56d-49bb-ab01-881c60c1fef0.png#averageHue=%230d0605&clientId=u226e303e-1a56-4&from=paste&id=u0b44cba7&originHeight=637&originWidth=767&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc47d8358-03c3-47e7-9316-ebaa2212e8a&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296009-9a9d1089-ec57-4598-873d-cc7e0805593c.png#averageHue=%230c0605&clientId=u226e303e-1a56-4&from=paste&id=ub26e7934&originHeight=634&originWidth=766&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf3a72550-02b0-4f0d-bb3d-ca254713d5a&title=)<br />除此之外，通过file命令也可以查看到文件的一些信息：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296198-b951fdda-c594-4777-badd-2cab15055df4.png#averageHue=%23200f0d&clientId=u226e303e-1a56-4&from=paste&id=uc24d8680&originHeight=215&originWidth=854&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue7b63f91-3741-42c5-8025-9cf9cd9a732&title=)
<a name="RPnNJ"></a>
## 10、文本文件查看
文本文件查看一般使用cat命令，但除了cat命令之外，还有其它几个实用的命令，下面依次来介绍：
<a name="iLPyh"></a>
### （1）cat命令
cat命令常常用于查看内容较少的文件。很多人把这个命令叫做小猫咪命令，但cat其实是 concatenate（连续） 的缩写，即连续显示文本内容。命令格式为：
```bash
cat [参数选项] [文件]
```
如：
```bash
cat -n /etc/profile
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296282-a5975a90-e26a-406a-a397-5c0955e822f7.png#averageHue=%23fafafa&clientId=u226e303e-1a56-4&from=paste&id=u366c2ef2&originHeight=274&originWidth=737&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6ad546c4-9787-4c52-93fa-6e2f28247dd&title=)<br />其中，加上参数  `**-n**`  用可以显示行数。cat的更多的参数选项可以输入 `**man cat**` 进行查看。以下列举的其他命令的详细介绍也可以输入 **man 命令** 进行查看。
<a name="yyJy5"></a>
### （2）tac命令
tac命令的正好与cat命令相反，是从文件末尾开始显示。
<a name="OVgdg"></a>
### （3）more命令
more命令适用于查看内容较多的文件。因为他可以实时显示百分比以提示现在已经阅读了多少内容。<br />命令格式为：
```bash
more [参数选项] [文件]
```
如：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675646296464-5c1946a0-d4b8-47d7-bc2b-9183d17f73aa.jpeg#averageHue=%23f8f8f8&clientId=u226e303e-1a56-4&from=paste&id=uf8fb14f6&originHeight=492&originWidth=734&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uaeeab6ef-70ab-4704-9a4a-e40f4c041b7&title=)<br />可以使用 **空格键** 或者 **回车键** 往下翻页查看后面的内容。
<a name="RQOJv"></a>
### （4）less命令
less也适用于查看内容较多的文件。less比more更为灵活，因为less可以往上、往下翻页。按下键盘上  **PgUp**  键可以往上翻页，按下  **PgDn**  可以往下翻页。但是less命令不会实时显示当前阅读的百分比。<br />命令格式为：
```bash
less [参数选项] [文件]
```
<a name="nefIl"></a>
### （5）head命令
head命令用于查看文件的前n行。如使用命令
```bash
head -n 20 /etc/profile
```
查看/etc目录下profile文件的前20行内容：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675646296669-f13bcdeb-050a-45ac-aa82-cd2b3986a8a6.jpeg#averageHue=%23f7f7f7&clientId=u226e303e-1a56-4&from=paste&id=u5e3f4677&originHeight=492&originWidth=739&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc7fa0a23-727b-43a3-93b5-5508d1244bc&title=)
<a name="iVnWA"></a>
### （6）tail命令
tail命令与head命令相反，tail命令用于查看文件后n行内容。如：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1675646296581-4431dbac-c227-4f4d-b1c3-46a288f1d816.png#averageHue=%23fcfcfc&clientId=u226e303e-1a56-4&from=paste&id=uc22babe6&originHeight=489&originWidth=733&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9bdb4fc-0e17-497c-85bc-99c392f3587&title=)
<a name="Ldujt"></a>
### （7）nl命令
nl命令可以显示内容的同时显示行号，与 `cat -n` 命令的作用差不多：![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1675646297051-fdaa6432-db4c-4cb9-9347-f7f6fd95c698.jpeg#averageHue=%23f7f7f7&clientId=u226e303e-1a56-4&from=paste&id=u7a866c3b&originHeight=483&originWidth=733&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufc018180-e092-49b8-af61-fb706d4ccb4&title=)
<a name="R4Tl5"></a>
## 11、设置LD_LIBRARY_PATH
**LD_LIBRARY_PATH** 是Linux / Unix中预定义的环境变量，它设置链接器在链接动态库/共享库时应该查看的路径。有时候需要把当前路径加到LD_LIBRARY_PATH中，如：
```bash
export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
```
