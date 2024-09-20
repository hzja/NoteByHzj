Linux Shell
<a name="kCpAZ"></a>
## 1、等号周围没有空格
对于许多语言，例如 Python，在等号周围添加空格是一个好习惯，因为它可以提高代码的可读性。<br />但是在 Bash 中，不应该在定义变量时添加额外的空格。<br />如果声明一个变量 name，如下：
```bash
name = "Yang"
```
这里会有一个错误：
```bash
-bash: name: command not found
```
如上所示，Bash 将 name 视为命令而不是变量，因为添加了不必要的空格。<br />因此，定义变量的正确方法如下：
```bash
name="Yang"
```
<a name="sbS75"></a>
## 2、正确地定义数组
在 Bash 中定义一个数组并不难。只需要使用一对括号来包含所有元素，如下例所示：
```bash
names=("Yang" "Elon" "Bill")
```
俗话说得好，“细节决定成败"。之前在编写 Bash 脚本时犯了一个严重的错误。习惯性地使用逗号（而非空格）来分隔数组中的每个元素。
```bash
names=("Yang","Elon","Bill")
```
Bash 不会将逗号视为分隔符，并且在 names 数组中只有一个元素 Yang,Elon,Bill 而不是预期的三个。
<a name="vGQX0"></a>
## 3、正确选择引号
当在 Bash 中声明一个变量时，关于引号的使用有 3 个可选的方案：

- **没有引号**
- **用单引号**
- **用双引号**

默认情况下，Bash 中的每个值都是一个字符串。因此，如果不需要空格，就不需要使用任何引号。（同样，它与其他语言有些不同，在其他语言中，您不能在没有引号的情况下定义字符串）<br />当需要使用引号时，请注意**单引号**和**双引号**之间的区别。<br />例如，声明 3 个变量，如下所示。那么，truth1 和 truth2 有什么区别呢？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1652017720359-13c2c3d2-14b2-4766-a38d-55dd043c244f.png#clientId=u7f16c730-187e-4&from=paste&id=u75972402&originHeight=90&originWidth=406&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02fd2bcf-6d7e-4ea5-8a24-e7e6caa200e&title=)<br />如果打印它们，会看到 truth1 是 ${name} is amazing.。而 truth2 是 Yang is amazing.。<br />总而言之，一对单引号内的字符串将始终被解释为它的本身。
<a name="v5Ki2"></a>
## 4、避免在命令中使用反引号
在 Bash 中，可以将命令的结果保存到变量中。有两种可能的方法来做到这一点：

- `variable=`command``
- `variable=$(command)`

例如，要保存 `ls` 命令的结果，可以这样写：
```bash
file_list=`ls`
```
或者
```bash
file_list=$(ls)
```
但是，这里的最佳实践始终是使用第二种方法，尤其是在编写较长的脚本时。因为**反引号**和**单引号**看起来很相似，有时可能会混淆它们。
<a name="N0rW9"></a>
## 5、避免使用特殊名称
一些名称在 Linux 中被预定义为环境变量，如 `USER`、`HOME`、`PATH` 等。可以使用 `printenv` 命令列出它们。<br />显然，不应该用与环境变量相同的名称来声明变量。<br />好消息是 Linux 中所有的环境变量都是用大写字母写的，所以可以用小写字母来自定义变量，以区别环境变量。这是一个好习惯。
