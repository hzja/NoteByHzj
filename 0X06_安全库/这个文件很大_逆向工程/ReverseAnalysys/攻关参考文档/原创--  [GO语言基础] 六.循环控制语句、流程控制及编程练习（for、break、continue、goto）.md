# 原创
：  [GO语言基础] 六.循环控制语句、流程控制及编程练习（for、break、continue、goto）

# [GO语言基础] 六.循环控制语句、流程控制及编程练习（for、break、continue、goto）

作为网络安全初学者，会遇到采用Go语言开发的恶意样本。因此从今天开始从零讲解Golang编程语言，一方面是督促自己不断前行且学习新知识；另一方面是分享与读者，希望大家一起进步。**前文介绍了Golang的顺序控制语句和条件控制语句。这篇文章将详细讲解循环控制语句和流程控制，包括for、break、continue、goto及相关编程练习。** 这系列文章入门部分将参考“尚硅谷”韩顺平老师的视频和书籍《GO高级编程》，详见参考文献，并结合作者多年的编程经验进行学习和丰富，且看且珍惜吧！后续会结合网络安全进行GO语言实战深入，加油~

这些年我学过各种编程语言，从最早的C语言到C++，再到C#、PHP、JAVA，再到IOS开发、Python，到最新的GO语言，学得是真的杂。有时候觉得编程语言恰恰是最简单的，而通过一门编程语言能够解决实际问题或深入底层才是其价值所在，并且当我们学好一门编程语言后，其他编程语言都非常类似，殊途同归，学起来也很迅速。

**源码下载地址：**

前文参考：

#### 文章目录

---


## 一.for循环控制

### 1.基本语法

for循环的语法格式：

```
for 循环变量初始化; 循环条件; 循环变量迭代 {
	循环操作语句
}

```

for循环主要有四要素，包括：

for循环语句基本流程如下：

for循环的执行顺序说明如下：

下面举个简单的例子：

```
package main
import "fmt"

func main() {
	//for循环
	for n := 1; n &lt;= 10; n++ {
		fmt.Println("Eastmount", n)
	}
}

```

输出结果如下图所示：

<mark>**for的第二种语法格式：**</mark><br/> for循环条件是返回一个**布尔值**的表达式，第二种方式是将变量初始化和变量迭代写到其他位置。

```
for 循环判断条件 {
	//循环执行语句
}

```

案例如下所示：

<mark>**for的第三种语法格式：**</mark><br/> 下面的写法等价于 **for; ; {}** ，它是一个无限循环，通常需要配合break语句使用。

```
for {
	//循环执行语句
}

```

案例如下所示：

---


### 2.for-range循环

<mark>Go语言中range关键字用于for循环中迭代数组(array)、切片(slice)、通道(channel)或集合(map)的元素。在数组和切片中它返回元素的索引和索引对应的值，在集合中返回key-value对。数组后续文章介绍，这里主要介绍遍历字符串。</mark>

具体案例如下所示：

```
package main
import "fmt"

func main() {
	//1.for-range遍历字符串
	str := "Eastmount CSDN"
	for index, val := range str {  
		fmt.Printf("index=%d, value=%c \n", index, val)
	}

	//2.for-range遍历数组
	nums := []int{2, 3, 4}
	for i, num := range nums {
		fmt.Printf("index=%d, value=%d \n", i, num)
	}
}

```

输出结果如下图所示：

如果我们的字符串含有中文，那么传统的遍历字符串方式就会错误，会出现乱码。**原因是传统对字符串的遍历是按照字节来遍历，而一个汉字在utf8编码对应3个字节。** 如何解决呢？需要将str转换成[]rune切片即可。

> 
**rune**<br/> golang中string底层是通过byte数组实现的。中文字符在unicode下占2个字节，在utf-8编码下占3个字节，而golang默认编码正好是utf-8。rune 等同于int32，常用来处理unicode或utf-8字符。<br/><br/> <img alt="在这里插入图片描述" height="200" src="https://img-blog.csdnimg.cn/20210218213225279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="600"/>


下面展示具体的代码。

```
package main
import "fmt"

func main() {
	//1.字符串遍历-传统方法
	str := "CSDN!秀璋"
	str2 := []rune(str)           //将str转换成[]rune
	for i := 0; i &lt; len(str2); i++  {  
		fmt.Printf("index=%d, value=%c \n", i, str2[i])
	}
	fmt.Println("")

	//2.字符串遍历-for range
	for index, val := range str {
		fmt.Printf("index=%d, value=%c \n", index, val)
	}
	fmt.Println("")

	//3.如果不转rune直接遍历(乱码)
	for i := 0; i &lt; len(str); i++  {  
		fmt.Printf("index=%d, value=%c \n", i, str[i])
	}
}

```

输出结果如下图所示，前面两种方式正确（按字节遍历），第三种方式会出现乱码。

---


### 3.for编程经典案例

下面介绍各编程语言中循环都会出现的案例，计算1到100的个数及总和。

```
package main
import "fmt"

func main() {
	var result = 0
	var num = 0
	for i := 1; i &lt;= 100; i++ {
		result += i
		num++
	}
	fmt.Println("1+2+...+100 =", result)
	fmt.Println("个数 =", num)
}

```

输出结果如下图所示：

流程图如下图所示：

---


### 4.类似while和do-while循环

由于Golang只有for循环，没有while关键字和do-while语法，所以只能通过for循环来模拟while和do-while循环，即使用for+break实现。

<mark>**(1) while循环**</mark><br/> for循环模拟while循环的核心代码如下图所示，需要注意：

类似于Java或C语言的while循环语句：

```
int i = 0;
while(i&lt;10){    // notice there is only &lt;
    do something
}

```

案例如下，使用while输出10句“hello world”。

```
package main
import "fmt"

func main() {
	//类似while循环
	var i int = 1
	for {
		//循环条件
		if i &gt; 10 {
			break           //结束循环
		}
		fmt.Println("hello world", i)
		i++
	}
}

```

输出结果如下图所示：

<mark>**(2) do-while循环**</mark><br/> 由于do-while是先执行后判断，所以for循环模拟do-while循环的核心代码如下图所示：

需要注意：

```
package main
import "fmt"

func main() {
	//使用do-while实现输出10句"hello world"
	var j int = 1
	for {
		//先执行后判断
		fmt.Println("hello world", j)
		j++
		if j &gt; 10 {
			break           //结束循环
		}
	}
}

```

---


## 二.多重循环控制

具体含义为：

下面通过案例进行多重循环理解，这也是循环的一个难点和重点。正如韩老师所说，编程过程中遇到困难是很常见的，我们需要注意：

---


### 案例1：循环计算平均成绩

题目：统计3个班成绩情况，每个班有4名同学，求出各个班的平均分和所有班级的平均分。学生的成绩从键盘输入。

```
package main
import "fmt"

func main() {
	//求出各个班的平均分和所有班级的平均分(3个班 每个班有4名同学)
	//解题:外层循环班级数 内存循环计算每个班级平均成绩
	var totalsum float64 = 0.0
	for j := 1; j &lt;= 3; j++ { 
		var sum float64 = 0.0
		for i := 1; i &lt;= 4; i++ {
			var score float64
			fmt.Printf("请输入第%d班 第%d个学生的成绩 \n", j, i)
			fmt.Scanln(&amp;score)
			//累计总分
			sum += score
		}
		fmt.Printf("第%d个班级的平均分是%v \n", j, sum / 4)
		//计算各个班的总结成绩
		totalsum += sum
	}
	fmt.Printf("各个班级的总成绩是%v 平均分是%v \n", totalsum, totalsum / (4* 3))
}

```

输出结果如下图所示：

<mark>同时，为了更灵活的编写代码，我们可以尝试将循环控制常量3个班和4位同学修改为变量，具体如下：</mark>

```
package main
import "fmt"

func main() {
	//求出各个班的平均分和所有班级的平均分(3个班 每个班有4名同学)
	//解题:外层循环班级数 内存循环计算每个班级平均成绩
	var classNum int = 3
	var stuNum int = 4
	var totalsum float64 = 0.0

	for j := 1; j &lt;= classNum; j++ { 
		var sum float64 = 0.0
		for i := 1; i &lt;= stuNum; i++ {
			var score float64
			fmt.Printf("请输入第%d班 第%d个学生的成绩 \n", j, i)
			fmt.Scanln(&amp;score)
			//累计总分
			sum += score
		}
		//注意golang数据类型需要转换一致才能运算 int-&gt;float64
		fmt.Printf("第%d个班级的平均分是%v \n", j, sum / float64(stuNum))
		//计算各个班的总结成绩
		totalsum += sum
	}
	result := totalsum / float64(stuNum * classNum)
	fmt.Printf("各个班级的总成绩是%v 平均分是%v \n", totalsum, result)
}

```

如果需要继续统计各班及格人数，怎么实现呢？

---


### 案例2：循环打印金字塔和倒三角

打印金字塔是经典的案例，在前面的第二篇文章也布置过。下面我们通过for循环打印各种金字塔，思路为：

首先介绍打印矩形和半个三角形金字塔代码。

```
package main
import "fmt"

func main() {
	var totalLevel int = 5

	//1.打印矩形
	for i := 1; i &lt;= totalLevel; i++ {     //i表示层数
		for j := 1; j &lt;= totalLevel; j++ { //每层输出
			fmt.Print("*")
		}
		fmt.Println()  //换行
	}
	fmt.Println()

	//2.打印正三角半个金字塔
	for i := 1; i &lt;= totalLevel; i++ {   //i表示层数
		for j := 1; j &lt;= i; j++ {        //每层输出
			fmt.Print("*")
		}
		fmt.Println()  //换行
	}
	fmt.Println()

	//3.打印倒三角半个金字塔
	for i := 1; i &lt;= totalLevel; i++ {   //i表示层数
		for j := (totalLevel - i); j &gt;= 0; j-- {       //每层输出
			fmt.Print("*")
		}
		fmt.Println()  //换行
	}
}

```

输出结果如下图所示：

理解上述代码之后，我们再打印金字塔，其规律为：

```
空格 + * + 空格

    *           1层 1个* 规律: 2*层数-1   空格 4个 规律:总层数-当前层数
   ***          2层 3个* 规律: 2*层数-1   空格 3个 规律:总层数-当前层数
  *****
 *******
*********

```

完整代码如下，包括实心金字塔和空心金字塔。

```
package main
import "fmt"

func main() {
	/* 打印金字塔
			*           1层 1个* 规律: 2*层数-1   空格 4个 规律:总层数-当前层数
		   ***          2层 3个* 规律: 2*层数-1   空格 3个 规律:总层数-当前层数
		  *****
		 *******
		*********
	*/

	var totalLevel int = 5

	//方法一 打印实心金字塔
	for i := 1; i &lt;= totalLevel; i++ {               //i表示层数
		//先打印空格
		for j := (totalLevel - i); j &gt;= 0; j-- {
			fmt.Print(" ")
		}
		//打印星号
		for k := (2 * i - 1); k &gt; 0; k-- {     
			fmt.Print("*")
		}
		fmt.Println()  //换行
	}
	fmt.Println()

	//方法二 打印空心金字塔
	for i := 1; i &lt;= totalLevel; i++ {               //i表示层数
		//先打印空格
		for j := 1; j &lt;= (totalLevel - i); j++ {
			fmt.Print(" ")
		}
		//打印星号
		for k := 1; k &lt;= (2 * i - 1); k++ {
			//每层第一个和最后一个打印* 最底层全部打印*
			if k == 1 || k == (2 * i - 1) || i == totalLevel {
				fmt.Print("*")
			} else {
				fmt.Print(" ")
			}
		}
		fmt.Println()  //换行
	}
}

```

输出结果如下图所示：

---


## 三.跳转控制语句

### 1.break

break语句用于终止某个语句块的执行，用于中断当前for循环或跳出switch语句。基本语法如下：

```
{
	....
	break
	....
}

```

其示意图如下图所示（引用至韩老师）。

首先结合之前1+2+…+100介绍一个简单案例，求出当和第一次大于20的当前数字。

```
package main
import "fmt"

func main() {
	//求出当和第一次大于20的当前数字
	sum := 0
	for i := 1; i &lt;= 100; i++ {
		sum += i   //求和
		if sum &gt; 20 {
			fmt.Println("sum大于20时当前数字是", i)
			break  //跳出循环
		} 
	}
}

```

输出结果如下图所示：

break语句的注意事项：

通过标签跳出for循环指定层的代码如下所示：

```
package main
import "fmt"

func main() {
	//通过标签跳出for循环指定层
	label1:
	for i := 0; i &lt; 4; i++ {
		//label2:
		for j := 0; j &lt; 10; j++ {
			if j ==2 {
				//默认跳出最近的循环 这里尝试跳出外层循环
				break label1
			}
			fmt.Println("j =", j, "i =", i)
		}
	} 
}

```

运行结果如下，当j等于2时跳出所有循环，即调转至label1。：

---


### 2.continue

<mark>continue语句用于结束本次循环，它将继续执行下一次循环。其基本语法如下：</mark>

```
{
	...
	continue
	...
}

```

流程图如下所示，它将结束本次循环，继续迭代执行下一次循环。

同样，continue语句出现在多层嵌套的循环语句体中时，可以通过标签指明要跳过的是哪一层循环，这个和前面的break标签使用规则一样。

> 
个人不太喜欢这个跳出指定循环层的功能，感觉代码不是很好控制。尤其是大型项目时，更建议结合实际判断条件进行跳出，当然作者理解能力尚浅，如果读者有好的应用场景可以分享与我，谢谢。


举个简单案例：

```
package main
import "fmt"

func main() {
	//continue跳出当前循环
	for i := 0; i &lt; 4; i++ {
		for j := 0; j &lt; 10; j++ {
			if j ==2 {
				continue
			}
			fmt.Println("j =", j, "i =", i)
		}
	} 
}

```

输出结果如下图所示，每次都没有输出j=2。

如果我们需要使用continue打印100以内的奇数，则可以编写如下代码：

---


## 四.goto语句

<mark>Golang中的goto语句可以无条件转移到程序中指定的行，goto常与条件语句配合使用，用以实现条件转移或跳出循环体。注意，在GO程序设计中一般不主张使用goto语句，以避免造成流程的混乱，使理解和调试程序都产生困难，同样C语言也不主张使用。</mark>

```
goto label
....
label: statement

```

其执行流程如下图所示：

下面介绍一个简单的案例：

```
package main
import "fmt"

func main() {
	//goto语句
	fmt.Println("aaaaa")
	var n int = 30
	if n &gt; 20 {
		goto label
	}
	fmt.Println("bbbbb")
	fmt.Println("ccccc")
	label:
	fmt.Println("ddddd")
	fmt.Println("eeeee")
	fmt.Println("fffff")
}

```

输出结果如下图所示：

---


## 五.跳转控制语句return

return使用在方法或函数中，表示跳出所在的方法或函数。在分享函数的时候，我会详细介绍。

return使用说明：

```
package main
import "fmt"

func main() {
	//return语句
	for i := 1; i&lt;=10; i++ {
		if i==3 {
			return
		}
		fmt.Println("输出结果", i)
	}
	fmt.Println("over")
}

```

输出结果如下图所示：

---


## 六.Golang编程练习

### 1.题目

**(1) 打印1到100之间所有是9倍数的整数的个数及总和。**

**(2) 循环依次输出“East 秀璋”字符串的所有字符。**

**(3) 打印9*9乘法表。**

**(4) 模拟网站登录验证机制.假设有3次机会，如果用户名为“Eastmount”，密码为“666666”提示登录成功，否则提示剩余机会；最终超过3次则提示“输入错误次数过多，无法登录”。**

**(5) 随机生成1到100的一个数，如果生成99这个数就停止，计算一共使用多少次。**

**(6) 输入如下4*5的矩阵。**

```
1       2       3       4       5
2       4       6       8       10
3       6       9       12      15
4       8       12      16      20

```

**(7) 利用循环求Fibonacci数列的前10个数。**

**(8) 求2到200间的全部素数。**

---


### 2.解答

<mark>**(1) 打印1到100之间所有是9倍数的整数的个数及总和。**</mark>

代码如下：

```
package main
import "fmt"

func main() {
	var max int = 100
	var count int = 0
	var sum int = 0
	for i := 1; i &lt;= max; i++ {
		if i % 9 == 0 {
			count++
			sum += i
		}
	}
	fmt.Printf("count=%v sum=%v \n", count, sum)
}

```

输出结果如下图所示：

<mark>**(2) 循环依次输出“East 秀璋”字符串的所有字符。**</mark><br/> 在数组上使用range将传入index和值两个变量。如果我们不需要使用该元素的序号，则使用空白符"_"省略。

```
package main
import "fmt"
 
func main() {
	str := "East=秀璋"

	//方法1
	fmt.Printf("utf-8遍历字符串\n")
	for _, s := range str {
		fmt.Printf("utf-8遍历: %c %v \n", s, s)
	}
	fmt.Println("")

	//方法2
	str2 := []rune(str)
	for i := 0; i &lt; len(str2); i++  {  
		fmt.Printf("utf-8遍历: index=%d, value=%c \n", i, str2[i])
	}
	fmt.Println("")

	//方法3 乱码
	fmt.Printf("unicode遍历字符串\n")
	for i := 0; i &lt; len(str); i++ {
		ch := str[i]
		fmt.Printf("unicode遍历: %c  %d\n", ch, ch)
	}
}

```

输出结果如下图所示：

<mark>**(3) 打印9*9乘法表。**</mark>

代码如下：

```
package main
import "fmt"
 
func main() {
	//9*9乘法表
	var num int = 9
	for i := 1; i &lt;= num; i++ {
		for j := 1; j &lt;= i; j++ {
			fmt.Printf("%v*%v=%v \t", j, i, j*i)
		}
		fmt.Println()
	}
}

```

输出结果如下图所示：

<mark>**(4) 模拟网站登录验证机制.假设有3次机会，如果用户名为“Eastmount”，密码为“666666”提示登录成功，否则提示剩余机会；最终超过3次则提示“输入错误次数过多，无法登录”。**</mark>

代码如下：

```
package main
import "fmt"
 
func main() {
	//模拟网站登录
	var name string
	var pwd string
	var loginChance = 3

	for i := 1; i &lt;= 3; i++ {
		//输入信息
		fmt.Println("请输入用户名")
		fmt.Scanln(&amp;name)
		fmt.Println("请输入密码")
		fmt.Scanln(&amp;pwd)
		
		if name == "Eastmount" &amp;&amp; pwd == "666666" {
			fmt.Println("恭喜你登录成功")
			break
		} else {
			loginChance--
			fmt.Printf("输入错误，你还有%v次机会\n", loginChance)
		}
	}
	if loginChance &lt;= 0 {
		fmt.Println("输入错误次数过多，无法登录")
	}
}

```

输出结果如下图所示：

<mark>**(5) 随机生成1到100的一个数，如果生成99这个数就停止，计算一共使用多少次。**</mark>

```
package main
import (
	"fmt"
	"math/rand"
	"time"
)
 
func main() {
	//随机生成1到100的一个数，如果生成99这个数就停止
	var count int = 0
	for {
		//种子生成 使用系统时间的不确定性来进行初始化
		rand.Seed(time.Now().UnixNano())
		num := rand.Intn(100) + 1
		fmt.Println("num =", num)
		count++
		if (num == 99) {
			break
		}
	}
	fmt.Println("生成随机数99共使用次数 =", count)
}

```

输出结果如下图所示：

<mark>**(6) 输入如下4*5的矩阵。**</mark>

代码如下：

```
package main
import "fmt"
 
func main() {
	var n int = 4
	var m int = 5
	var res int = 0
	for i := 1; i &lt;= n; i++ {
		for j := 1; j &lt;= m; j++ {
			fmt.Printf("%v\t", i*j)
			res++
		}
		fmt.Println("")
	} 
}

```

输出结果如下图所示：

<mark>**(7) 利用循环求Fibonacci数列的前10个数。**</mark>

解题思路对应的流程如下图所示：

```
package main
import "fmt"
 
func main() {
	//Fibonacci数列
	var f1 int = 1
	var f2 int = 1
	var f3 int

	fmt.Printf("%v\n%v\n", f1, f2)
	for i := 1; i &lt;= 10; i++ {
		f3 = f1 + f2
		fmt.Printf("%v\n", f3)
		f1 = f2
		f2 = f3
	} 
}

```

输出结果如下图所示：

<mark>**(8) 求2到200间的全部素数。**</mark>

代码如下：

```
package main
import "fmt"
 
func main() {
	//求2到200间的全部素数
	var n int
	var k int
	var i int
	var m int = 0

	for n = 2; n &lt;= 200; n++ {
		k = n / 2
		//如果n被i整除终止内循环
		for i = 2; i &lt;= k; i++ {
			if n % i == 0 {
				break
			}
		}
		//如果i&gt;k+1表示n未被整除
		if i &gt;= k + 1 {
			fmt.Printf("%d\t", n)
			//m控制换行 输出10个素数换行
			m = m + 1
			if m % 10 == 0 {
				fmt.Println()
			}
		}
	}
}

```

运行结果如下图所示：

---


## 七.总结

写到这里，这篇基础性Golang文章介绍完毕，希望您喜欢！祝大家新年快乐，牛气冲天，也希望自己能分享更优秀、更深入的文章。

Go基本运算和进制转换了解后，后面的文章将详细介绍Go语言的条件语句和循环语句知识，并结合案例进行普及。希望这篇基础性文章对您有帮助，写得不好的地方还请海涵。同时非常感谢参考文献中的大佬们的文章分享，尤其是韩顺平老师，深知自己很菜，得努力前行。也希望自己能深入下去，未来四年好好研究Go编程语言，做更多实际工程，写更好的文章，共勉！

源代码下载地址：

2020年在github的绿瓷砖终于贴完了第一年提交2100余次，获得1500多+stars，开源93个仓库，300个粉丝。挺开心的，希望自己能坚持在github打卡五年，督促自己不断前行。简单总结下，最满意的资源是YQ爆发时，去年2月分享的舆情分析和情感分析，用这系列有温度的代码为武汉加油；最高赞的是Python图像识别系列，也获得了第一位来自国外开发者的贡献补充；最花时间的是Wannacry逆向系列，花了我两月逆向分析，几乎成为了全网最详细的该蠕虫分析；还有AI系列、知识图谱实战、CVE复现、APT报告等等。当然也存在很多不足之处，希望来年分享更高质量的资源，也希望能将安全和AI顶会论文系列总结进来，真诚的希望它们能帮助到大家，感恩有你，一起加油~

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:娜璋AI之家 Eastmount 2021-02-19 星期五 夜于贵阳 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount))

---


**参考文献：**

---

