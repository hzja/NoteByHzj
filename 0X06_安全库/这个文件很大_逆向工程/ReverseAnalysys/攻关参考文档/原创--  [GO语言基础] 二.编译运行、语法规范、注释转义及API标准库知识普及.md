# 原创
：  [GO语言基础] 二.编译运行、语法规范、注释转义及API标准库知识普及

# [GO语言基础] 二.编译运行、语法规范、注释转义及API标准库知识普及

作为网络安全初学者，会遇到采用Go语言开发的恶意样本。因此从今天开始从零讲解Golang编程语言，一方面是督促自己不断前行且学习新知识；另一方面是分享与读者，希望大家一起进步。**前文介绍了什么是GO语言及为什么我要学习Golang；这篇文章将介绍Go的编译运行、语法规范、注释转义及API标准库知识。** 这系列文章入门部分将参考“尚硅谷”韩顺平老师的视频和书籍《GO高级编程》，详见参考文献，并结合作者多年的编程经验进行学习和丰富，且看且珍惜吧！后续会结合网络安全进行GO语言实战深入，加油~

这些年我学过各种编程语言，从最早的C语言到C++，再到C#、PHP、JAVA，再到IOS开发、Python，到最新的GO语言，学得是真的杂。有时候觉得编程语言恰恰是最简单的，而通过一门编程语言能够解决实际问题或深入底层才是其价值所在，并且当我们学好一门编程语言后（尤其C和C++），其他编程语言都非常类似，殊途同归，学起来也很迅速。

**源码下载地址：**

前文参考：

#### 文章目录

---


## 一.Go语言执行流程和编译运行

### 1.回顾前文

前面我们通过VSCode已经快速编写了第一个Go程序，具体步骤如下：

其运行结果如下图所示：

核心代码如下：

```
package main    //把test.go文件归属到main

import "fmt"    //引入包fmt
func main() {
	//输出内容
	fmt.Println("hello, My name is Eastmount!")
}

```

代码解释：

---


### 2.执行流程

如果是对源码编译后再执行，Go的执行流程如下图所示：

如果是对源码直接执行，通过“go run”运行程序，Go的执行流程如下图所示：

注意，在真实环境中是需要先编译后运行的。所以“go run”命令的运行时间更长，因为编译过程中隐藏在命令中，而上面的步骤是运行编译后的可执行文件。

“尚硅谷”韩顺平老师简单总结了两种执行流程的区别：

---


### 3.编译运行

**(1) 什么是编译呢？**<br/> 编译(compilation , compile) 指利用编译程序从源语言编写的源程序产生目标程序的过程，Go程序的编译就是把高级语言（源码）变成计算机可以识别的二进制语言，计算机只认识1和0，编译程序把人们熟悉的语言换成二进制的。

当有了Go源文件后，通过编译器将其编译成机器可以识别的二进制文件，如下图所示，在该源文件目录下，通过“go build”对test01.go文件进行编译。

注意，“go build”编译可以指定生成的可执行文件名，但在Windows下必须是“.exe”后缀。下面简单演示下。

运行结果如下图所示：

> 
**小技巧：在CMD中如何快速进入指定目录**<br/> <mark>打开源文件所在目录，在路径栏输入cmd+回车，即可快速进入CMD。</mark><br/> <img alt="在这里插入图片描述" height="100" src="https://img-blog.csdnimg.cn/20210126003554304.png#pic_center" width="600"/><br/> <img alt="在这里插入图片描述" height="100" src="https://img-blog.csdnimg.cn/20210126003540969.png#pic_center" width="600"/><br/> 显示结果如下图所示：<br/> <img alt="在这里插入图片描述" height="350" src="https://img-blog.csdnimg.cn/20210126003733602.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/><br/> <mark>另一种方法是在该目录下按住“Shift+鼠标右键”，点击“在此处打开PowerShell”即可。</mark><br/> <img alt="在这里插入图片描述" height="350" src="https://img-blog.csdnimg.cn/20210126003838105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/>


**(2) 错误提升**<br/> 如果程序没有错误，没有任何提示，会在当前目录下出现一个可执行文件（Windows下是.exe文件，Linux下是一个可执行文件），该文件是二进制文件，也是可以执行的文件。如果程序有错误，编译时会在错误的那行提示，这有助于程序员调试代码。

**(3) 什么是运行**<br/> 运行有两种形式：

---


## 二.语法要求及常见问题

### 1.语法注意事项

---


### 2.常见问题

**(1) 如何修改字体大小？**<br/> 如果我们想在VSCode中修改Go程序字体大小，则需要在设置中修改代码选项。

性能-&gt;设置-&gt;字符大小。

**(2) 系统找不到指定文件。**<br/> 解决方法：源文件名不存在或错误，或者路径写错误。

**(3) 每次编译和运行都需要在CMD中执行吗？**<br/> 作为Go初学者，我建议大家先结合CMD来执行程序，这既能了解程序的基本执行流程；又能了解CMD的常用操作，如果你是想用GO做安全或后台，这些都是非常基础的命令。当然，随着这些列文章深入，后面会分享更好的集成开发工具，那些工具中可以直接执行程序，这也是为什么Python我首先推荐大家使用官网的IDLE。

**(4) 编程语法错误。**<br/> 作为初学者，在编程过程中最容易犯的错误就是语法错误，GO要求必须按照语法规则编写代码。如果你的程序违反了语法规则，如忘记大括号、引号、拼写错误、变量定义未使用等，Go编译器都会提示相应的语法错误，记住：

如下图所示，在第6行没有定义“fmt.Printin”，这就是一个语法错误，正确的应该是“fmt.Println”。

---


## 三.Golang转义字符、注释及编程风格

### 1.转义字符

Go语言常用的转义字符（escape char）如下：

具体实例如下所示，新建文件夹“chapter02”和“escaptechar”，接着新建“main.go”文件，并添加如下代码：

接着我们在CMD中去到指定目录，编译运行程序输出如下图所示效果：

完整代码如下：

```
package main

import "fmt"    //引入包fmt 提供格式化、输出、输入函数
func main() {
	//转义字符
	fmt.Println("Eastmount\tCSDN\tyangxiuzhang")

	fmt.Println("Hello\nWorld!!!")

	fmt.Println("C:\\Users\\xiuzhang\\Desktop\\data")

	fmt.Println("秀璋说\"我爱娜娜\"")

	//注意:回车会替换前面的内容
	fmt.Println("秀璋爱娜娜\r珞珞")

	fmt.Println("\a")
}

```

输出结果如下所示：

其他转义字符如下所示：

```
\a             响铃符       （相当于 \x07）
\b             退格键(BS)   （相当于 \x08）
\f             换页符       （相当于 \x0C）
\t             横向制表符    （相当于 \x09）
\n             换行符       （相当于 \x0A）
\r             回车符       （相当于 \x0D）
\v             纵向制表符    （相当于 \x0B）
\123           匹配 8  進制编码所代表的字符（必须是 3 位数字）
\x7F           匹配 16 進制编码所代表的字符（必须是 3 位数字）
\x{10FFFF}     匹配 16 進制编码所代表的字符（最大值 10FFFF  ） 
\\             匹配字符 \
\"             匹配字符 "

```

---


### 2.注释

注释（Comment）用于解释程序的文字，注释提高了代码的阅读性。一个程序员一定要养成良好的编程习惯，注释就是将自己的思想整理出来，再用代码去实现。注释的好处及应用场景如下：

**即使是Go语言的源代码，也包括了丰富的注释，所以养成良好的注释习惯真的很重要，尤其是初学者。通常三行代码一行注释，根据个人习惯而定。**

Go语言支持C语言风格的 /* */ 块注释，也支持C++风格的 // 行注释。行注释更通用，块注释主要用于针对包的详细说明、函数功能说明或屏蔽大块的代码。

Golang两种注释如下：

注释的使用细节：

---


### 3.编程风格

**(1) 正确的注释和注释风格**

**(2) 正确的缩进和空白**

使用gofmt后自动对齐，如下图所示：

<img alt="在这里插入图片描述" src="https://img-blog.csdnimg.cn/20210128165634460.png#pic_center"/><br/> <img alt="在这里插入图片描述" height="350" src="https://img-blog.csdnimg.cn/20210128165747600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="550"/>

**(3) Go语言的代码风格**<br/> 正确的代码风格：

```
package main

import "fmt" //引入包fmt 提供格式化、输出、输入函数
func main() {
	//行注释：转义字符
	fmt.Println("Eastmount\tCSDN\tyangxiuzhang")
}

```

错误的代码风格：

```
package main

import "fmt" //引入包fmt 提供格式化、输出、输入函数
func main()
{
	//行注释：转义字符
	fmt.Println("Eastmount\tCSDN\tyangxiuzhang")
}

```

**(4) 一行代码最长不超过80个字符，超过请使用换行展示，尽量保持格式优雅。**

---


## 四.Golang编程指南

<mark>Golang 官方网站如下，记住官方文档和源码永远是最好的学习场所。</mark>

当我们点击“Run”按钮即可运行程序，我们可以通过官网进行简单的代码学习。

点击“Tour”可进入编程指南，点击“packages”可查看官方标准包说明文档。

### 1.编程指南

点击“Tour”编程指南显示如下图所示：

下列代码输出结果如下：

```
package main

import "fmt"

func main() {
	fmt.Println("Hello, eastmount")
	fmt.Println("CSDN")
	var num = 1 + 2
	fmt.Println("num=", num)
}

```

**那么，官方文档怎么使用呢？**<br/> 我们点击“简体中文”，然后看到如下图所示的Go指南。

比如我们想了解“包”的基本概念，可以点击显示如下图所示，并且有对应的示例代码。

```
package main

import (
	"fmt"
	"math/rand"
)

func main() {
	fmt.Println("My favorite number is", rand.Intn(10))
}

```

虽然这个手册对我们编程有帮助，但是其解释过于简单，对零基础的同学不太友好。但也建议大家去了解学习。

---


### 2.官方API标准库

Go语言是通过包（packages）的形式来管理它的函数的，点击“packages”可查看官方标准包说明文档。

比如我们想了解import "fmt"中对应的函数及功能。我们直接在网站中查询“fmt”，点击“index”显示如下图所示：

我们想查看函数Println原型和案例，显示如下图所示：

```
package main

import (
	"fmt"
)

func main() {
	const name, age = "Kim", 22
	fmt.Println(name, "is", age, "years old.")

	// It is conventional not to worry about any
	// error returned by Println.
}

```

输出结果为：

**Go标准库API文档**

Go的包和源文件及函数的关系简图如下所示：

比如作者的GO源码如下图所示：

---


## 五.Dos常用指令

DOS常用指令这里仅简单介绍，这是为了让大家更好地了解Windows基础操作，同时对Linux、MacOS和安全领域的命令操作也有帮助。当然，后续也会详细介绍在工具中编译运行GO程序的方法。

### 1.目录操作指令

常见命令如下：

---


### 2.文件操作指令

---


### 3.其它指令

---


## 六.Golang编程练习

### 1.题目

虽然GO语言的基本语法下一篇文章再叙述，但作者希望你们能通过搜索引擎独立解决这六个基础练习，加油！

---


### 2.解答

```
package main

import "fmt"
func main() {
	fmt.Println("Hi, My name is Eastmount!")
}

```

```
package main

import "fmt"    //引入包fmt 提供格式化、输出、输入函数
func main() {
	fmt.Println("姓名\t年龄\t籍贯\t行业\nyxz\t28\t贵州\t程序员")
}

```

```
package main

import "fmt"    //引入包fmt 提供格式化、输出、输入函数
func main() {
	var a = 2
	var b = 3
	fmt.Println("a + b = ", a + b)
	fmt.Println("a - b = ", a - b)
	fmt.Println("a * b = ", a * b)
	fmt.Println("a / b = ", a / b)
}

```

```
package main

import (
   "fmt"
   "time"
)

func main() {
	//获取当前日期
    today := time.Now()
	fmt.Println("当前日期为:", today)
	//分别获取年月日
	year := today.Year()
    month := today.Month()
	day := today.Day()
	fmt.Println("年:", year)
	fmt.Println("月:", month)
	fmt.Println("日:", day)
	fmt.Printf("%d-%02d-%02d", year, month, day)
}

```

> 
**补充知识：= 是赋值， := 是声明变量并赋值**
<pre><code>// = 使用必须使用先var声明
var a
a=100
var b = 100
var c int = 100
// := 是声明并赋值，并且系统自动推断类型，不需要var关键字
d := 100
</code></pre>


```
package main

import "fmt" 
func main() {
	var result = 0
	for i := 0; i &lt;= 100; i++ {
		result += i 
	}
	fmt.Println("1+2+...+100=", result)
}

```

```
package main

import "fmt" 
func main() {
	var len = 5
	for i := 0; i &lt; len; i++ {
		for j := len - i; j &gt; 0; j-- {
			fmt.Print("*")
		}
		fmt.Print("\n")
	}
}

```

---


## 七.总结

写到这里，这篇基础性Golang文章介绍完毕，希望您喜欢！

编程习惯和Go基本概念了解后，后面的文章将详细介绍Go语言的基础语法知识，并结合案例进行普及。希望这篇基础性文章对您有帮助，写得不好的地方还请海涵。同时非常感谢参考文献中的大佬们的文章分享，深知自己很菜，得努力前行。也希望自己能深入下去，未来四年好好研究Go编程语言，做更多实际工程，写更好的文章，共勉！

源代码下载地址：

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:娜璋AI之家 Eastmount 2021-01-29 星期五 夜于贵阳 https://blog.csdn.net/Eastmount)

---


**参考文献：**
