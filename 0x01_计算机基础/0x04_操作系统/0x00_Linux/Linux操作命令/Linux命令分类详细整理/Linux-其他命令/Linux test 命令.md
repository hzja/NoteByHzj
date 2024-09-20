Linux<br />Linux test 命令是 Shell 内置命令，用来检测某个条件是否成立。test 通常和 if 语句一起使用，并且大部分 if 语句都依赖 test。可以将一个元素与另一个元素进行比较，但它更常用于BASH shell 脚本中，作为控制逻辑和程序流程 的条件语句的一部分。<br />test 命令有很多选项，可以进行数值、字符串和文件三个方面的检测。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999281-8efb1483-8c1c-4fbf-b6be-97f7bdead408.png#clientId=u8bbfe895-3fb9-4&from=paste&id=ub10a488d&originHeight=699&originWidth=1025&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue2ef21f6-c578-4d47-a620-ee8c9dc58d1&title=)
<a name="YvLih"></a>
### 一个基本的例子
在终端窗口中尝试这些命令。
```bash
linuxmi@linuxmi /home/linuxmi/www.linuxmi.com                                   
⚡ test 1 -eq 2 && echo "yes" || echo "no"
no

linuxmi@linuxmi /home/linuxmi/www.linuxmi.com                                   
⚡ test 1 -eq 1 && echo "yes" || echo "no"
yes
```
上面的命令分解如下：

- test：执行比较的命令
- 1：要比较的第一个元素。在此示例中，它是数字1，但它可以是任何数字，也可以是引号内的字符串。
- eq：比较方法。在本例中，正在测试一个值是否等于另一个值。
- 2：要比较第一个元素的元素。在这个例子中，它是数字2。
- &&：按顺序将命令链接在一起的 Linux 快捷方式。测试链的输出到随后的命令。当前面的命令的退出状态为 0 时，将执行双与号，这是表示命令没有失败的一种奇特方式。
- echo "yes"：比较成功时运行的命令。在这种情况下，我们所做的只是要求echo命令将单词“yes”打印到标准输出，但是如果测试结果证明为真，可以在此处运行任何将执行的命令。
- || : 在某种程度上，与 &&; 正好相反 只有在它前面的命令失败（退出状态不是 0）时，双管道才会执行。<br />echo "no"：比较不符时运行的命令。

小贴士：从 shell 提示符运行，test不会将值返回到标准输出，它仅返回退出状态代码。这就是为什么需要链接 echo 命令的原因。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999253-0c0e653f-30c9-4c2f-a73f-3fa74e5d86b9.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u6592fcb4&originHeight=465&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubeca5abc-ea44-4d2c-b3ec-6faf248cf1f&title=)<br />本质上一样的，该命令将 1 与 2 进行比较，如果它们匹配，则执行 echo“yes”语句并显示“yes”，如果它们不匹配，则执行 echo“no”语句，显示“no”。
<a name="c5Bvh"></a>
### 比较数字
如果要比较解析为数字的元素，可以使用以下比较运算符：

- -eq：值 1 等于值 2
- -ge : 值 1 大于或等于值 2
- -gt : 值 1 大于值 2
- -le : 值 1 小于或等于值 2
- -lt : 值 1 小于值 2
- -ne : 值 1 不等于值 2

示例测试
```bash
test 1 -eq 2 && echo "yes" || echo "no"
```
（在屏幕上显示“no”，因为 1 不等于 2）
```bash
test 1 -ge 2 && echo "yes" || echo "no"
```
（在屏幕上显示“no”，因为 1 不大于或等于 2）
```bash
test 1 -gt 2 && echo "yes" || echo "no"
```
（在屏幕上显示“no”，因为 1 不大于 2）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999279-36049fd2-b12d-4788-be7f-85c241db0d42.png#clientId=u8bbfe895-3fb9-4&from=paste&id=uf51f2fc7&originHeight=488&originWidth=947&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u30d7dc11-f6c6-4142-8798-39dd638fbe1&title=)
```bash
test 1 -le 2 && echo "yes" || echo "no"
```
（在屏幕上显示“yes”，因为 1 小于或等于 2）
```bash
test 1 -lt 2 && echo "yes" || echo "no"
```
（在屏幕上显示“yes”，因为 1 小于或等于 2）
```bash
test 1 -ne 2 && echo "yes" || echo "no"
```
（在屏幕上显示“yes”，因为 1 不等于 2）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999226-51e797a2-63c8-4ffe-a8b7-325de8e9adec.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u1239dc45&originHeight=418&originWidth=935&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9535d6a5-c670-46ed-b632-2e574e3a158&title=)
<a name="O4c0I"></a>
### 比较文本
比较解析为字符串的元素时，请使用以下比较运算符：

- =：字符串 1 匹配字符串 2
- != : 字符串 1 与字符串 2 不匹配
- -n：字符串长度大于0
- -z：字符串长度等于 0

例子
```bash
test "string1" = "string2" && echo "yes" || echo "no"
```
（在屏幕上显示“no”，因为“string1”不等于“string2”）
```bash
test "string1" != "string2" && echo "yes" || echo "no"
```
（在屏幕上显示“yes”，因为“string1”不等于“string2”）
```bash
test -n "string1" && echo "yes" || echo "no"
```
（在屏幕上显示“yes”，因为“string1”的字符串长度大于零）
```bash
test -z "string1" && echo "yes" || echo "no"
```
（在屏幕上显示“no”，因为“string1”的字符串长度大于零）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999220-d01021a6-5449-435e-96fd-76a6dbdd0c22.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u49f3b2db&originHeight=429&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5734483f-040d-4ef1-871c-6e8f1a72297&title=)
<a name="BJ75q"></a>
### 比较文件
比较文件时，请使用以下比较运算符：

- -ef：文件具有相同的设备和 inode 编号（它们是同一个文件）
- -nt : 第一个文件比第二个文件新
- -ot：第一个文件比第二个文件旧
- -b：文件存在并且是块特殊的
- -c：文件存在并且是字符特殊的
- -d：文件存在并且是目录
- -e : 文件存在
- -f : 文件存在并且是普通文件
- -g：文件存在并具有指定的组号
- -G : 文件存在且属于用户组
- -h或-L：文件存在并且是符号链接
- -k：文件存在并且设置了粘性位
- -O : 文件存在你是所有者
- -p：文件存在并且是命名管道
- -r：文件存在且可读
- -s：文件存在且大小大于零
- -S : 文件存在并且是一个socket
- -t ：在终端上打开文件描述符
- -u：文件存在并且设置了 set-user-id 位
- -w：文件存在且可写
- -x：文件存在且可执行

例子
```bash
⚡ test linuxmi -nt linux && echo "yes"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999687-508627d8-a717-4594-81da-ad603f70a960.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u2bdde722&originHeight=223&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u956292ba-b8e1-4027-9502-584d3301cbb&title=)<br />（如果 linuxmi 比 linux 新，则显示“yes”字样，如上图）
```bash
⚡ test -e /home/linuxmi/linuxmi && echo "yes"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653011000643-c88d6d08-518c-4992-ab68-9738e75ba1f3.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u61a1584d&originHeight=223&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69335db9-a1df-4ea4-a7d7-e297dd292ac&title=)<br />（如果 linuxmi 存在，将显示“yes”）
```bash
test -O /home/linuxmi/linuxmi && echo "yes"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999662-080a2371-e7ee-4504-bda9-727ead4fa677.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u3b1ba9f7&originHeight=223&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u49df1808-e6f5-4fc3-b884-7d6c923daee&title=)<br />（如果拥有 file1，则显示“yes”字样”）<br />块特殊：文件是块设备，这意味着数据以字节块的形式读取。这些通常是设备文件，例如硬盘驱动器。<br />特殊字符：文件在写入时立即执行，通常是串行端口等设备
<a name="PWw1l"></a>
### 比较多个条件
到目前为止，一切都在将一件事与另一件事进行比较，但是如果想比较两个条件怎么办？<br />例如，如果一只动物有四条腿并且会发出“哞哞”的声音，它可能是一头奶牛。简单地检查四只腿并不能保证有一头牛，但检查它发出的声音肯定可以。<br />要同时测试这两个条件，请使用以下语句：
```bash
test 4 -eq 4 -a "moo" = "moo" && echo "it is a cow" || echo "it is not a cow"
```
这里的关键部分是-a标志，它代表and。<br />有一种更好和更常用的方法来执行相同的测试，如下所示：
```bash
test 4 -eq 4 && test "moo" = "moo" && echo "it is a cow" || echo "it is not a cow"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999664-e4569d16-4e03-458b-9219-e6017da9da72.png#clientId=u8bbfe895-3fb9-4&from=paste&id=ud457ee81&originHeight=355&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u20bc8e8c-5bdd-4620-aecb-e01c7dfd546&title=)<br />test 命令的分支很重要。如果第一个测试 (4 = 4) 失败，则 test 命令以非零退出代码终止。因此，我们跳转到双管道符号并且“it is not a cow”打印到标准输出。但是，如果第一个测试成功并因此 test 导致退出代码0，那么我们跳转到第一个双与号(&&)。下一条语句是另一个测试条件！<br />如果第二次 test 失败，我们再次跳到双管并从那里继续。然而，如果第二个 test 成功，我们跳转到第二个双& 语句，在这个例子中，它只是将“it is a cow”回显到标准输出，然后终止返回到 shell 提示符。<br />另一个测试比较两个语句，如果其中一个为真，则输出一个字符串。例如，要检查是否存在名为“linuxmi.txt”的文件或名为“linuxmi.py”的文件，可以使用以下命令：<br />这里的关键部分是-o代表or。
```bash
test -e linuxmi.txt -o -e linuxmi.py && echo "linuxmi exists" || echo "linuxmi does not exist"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999708-f322e99b-5d6d-410f-a836-f05c26b8d4b5.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u12d0e1c2&originHeight=245&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9713bffa-153f-4d88-ba73-32b7327f91d&title=)<br />有一种更好和更常用的方法来执行相同的测试，如下所示：
```bash
test -e linuxmi.txt || test -e linuxmi.py && echo "linuxmi exists" || echo "linuxmi does not exist"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653011000023-8d7cab43-4f52-4a68-b490-f2bb6ee55889.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u55c1c577&originHeight=245&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3a193ef2-7dfd-4c77-bea3-18bd309ef91&title=)<br />排除 test 关键字<br />实际上不需要使用单词test来执行比较。所要做的就是将语句括在方括号中，如下所示：
```bash
⚡ [ -e linux.py ] && echo "linux.py exists" || echo "file1 does not exist"
linux.py exists
```
[and]基本上与test含义 相同。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653010999932-87b35763-0f14-4a65-bf14-f0cd12667347.png#clientId=u8bbfe895-3fb9-4&from=paste&id=uea4eaf1f&originHeight=311&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uae3c2b41-8af7-4e10-8757-30ac5845e65&title=)<br />现在知道这一点，可以改进比较多个条件，如下所示：
```bash
[ 4 -eq 4 ] && [ "moo" = "moo" ] && echo "it is a cow" || echo "it is not a cow"

[ -e linuxmi.py ] || [ -e linuxmi.txt ] && echo "linuxmi exists" || echo "linuxmi does not exist"
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1653011000113-208ee598-4101-4447-a8e9-9b441d736177.png#clientId=u8bbfe895-3fb9-4&from=paste&id=u676e3fa5&originHeight=355&originWidth=928&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3726ca86-8512-40e9-ba1a-f16954322d5&title=)
<a name="zZq6u"></a>
### 总结
test 命令在脚本中更有用，因为可以对照另一个变量测试一个变量的值并控制程序流程。在命令行上，使用它来测试文件是否存在。
