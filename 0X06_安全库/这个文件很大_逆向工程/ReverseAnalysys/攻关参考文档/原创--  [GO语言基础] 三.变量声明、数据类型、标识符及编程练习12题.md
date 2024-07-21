# 原创
：  [GO语言基础] 三.变量声明、数据类型、标识符及编程练习12题

# [GO语言基础] 三.变量声明、数据类型、标识符及编程练习12题

作为网络安全初学者，会遇到采用Go语言开发的恶意样本。因此从今天开始从零讲解Golang编程语言，一方面是督促自己不断前行且学习新知识；另一方面是分享与读者，希望大家一起进步。**前文介绍了Go的编译运行、语法规范、注释转义及API标准库知识；这篇文章将介绍Golang的变量、数据类型和标识符知识，并通过12道编程练习进行提升。** 这系列文章入门部分将参考“尚硅谷”韩顺平老师的视频和书籍《GO高级编程》，详见参考文献，并结合作者多年的编程经验进行学习和丰富，且看且珍惜吧！后续会结合网络安全进行GO语言实战深入，加油~

这些年我学过各种编程语言，从最早的C语言到C++，再到C#、PHP、JAVA，再到IOS开发、Python，到最新的GO语言，学得是真的杂。有时候觉得编程语言恰恰是最简单的，而通过一门编程语言能够解决实际问题或深入底层才是其价值所在，并且当我们学好一门编程语言后，其他编程语言都非常类似，殊途同归，学起来也很迅速。

**源码下载地址：**

前文参考：

#### 文章目录

---


## 一.变量

### 1.什么是变量

**为什么需要变量呢？**<br/> 一个程序就是一个世界，不论使用哪种高级程序语言编写代码，<mark>变量都是其程序的基本组成单位</mark>。如下图所示的sum和sub都是变量。

**变量的定义：**<br/> 变量相当于内存中一个数据存储空间的表示，可以将变量看作是一个房间的门牌号，通过门牌号能找到房间；<mark>通过变量名可以访问到变量的值</mark>。变量使用的常见三个步骤：

**变量入门示例：**<br/> 变量表示内存中的一个存储区域，该区域有自己的变量名和数据类型。下面是一个简单案例：

```
package main
import "fmt"

func main() {
	//定义变量
	var n int
	//赋值变量
	n = 10
	//使用变量
	fmt.Println("n =", n)
}

```

---


### 2.变量的声明

**Go语言变量使用的三种方式：**

```
package main
import "fmt"

func main() {
	//方法一：指定变量类型 int默认值为0
	var i int
	fmt.Println("i =", i)

	//方法二：根据值自行判定变量类型
	var num = 3.14
	fmt.Println("num =", num)

	//方法三：省略var  :=声明变量并赋值
	name := "eastmount"
	fmt.Println("name =", name)
}

```

输出结果如下图所示：

**多变量声明：**<br/> 在编程中，通常会遇到一次性声明多个变量的情况。Golang同样提供了对应的功能，如下所示：

代码如下，注意不要重复定义变量，否则会报错。

```
package main
import "fmt"

func main() {
	//方法一：指定变量类型 int默认值为0
	var n1, n2, n3 int
	fmt.Println("n1 =", n1, "n2 =", n2, "n3 =", n3)

	//方法二：根据值自行判定变量类型
	var m1, name, m3 = 100, "csdn", 3.14
	fmt.Println("m1 =", m1, "name =", name, "m3 =", m3)

	//方法三：省略var  :=声明变量并赋值
	k1, k2, k3 := 100, "yxz", 3.14
	fmt.Println("k1 =", k1, "k2 =", k2, "k3 =", k3)
}

```

输出结果如下图所示：

那么，如何一次性声明多个全局变量呢？

```
package main
import "fmt"

//定义全局变量
var n = 100
var m = 200
var k = "eastmount"

//等价于一次性声明
var (
	o = 300
	p = 400
	q = "yxz"
)

func main() {
	//输出全局变量
	fmt.Println("n =", n, "m =", m, "k =", k)
}

```

---


### 3.变量的注意事项

**注意事项：**

**变量知识总结：**

> 
**加号用法：**



```
package main
import "fmt"

func main() {
	//加法运算
	var i, j = 2, 3
	var res = i + j
	fmt.Println("res =", res)

	//字符串拼接
	var str1 = "hello"
	var str2 = "world"
	var str3 = str1 + str2
	fmt.Println("str3 =", str3)
}

```

---


## 二.数据类型

每一种数据都定义了明确的数据类型，并在内存中分配不同大小的内存空间。常用的数据类型分为基本数据类型和派生/复杂数据类型。

### 1.整型

整数类型就是用于存放整数值，比如0、-2、23等。

**(1) 整形的各个类型**<br/> 注意，一个字节为8位（bit），计算机采用二进制（0或1），即2^8表数范围。

<th align="center">类型</th><th align="center">有无符号</th><th align="center">占用存储空间</th><th align="center">表数范围</th>
|------
<td align="center">int8</td><td align="center">有</td><td align="center">1字节</td><td align="center">                                                       −                                     128                                     ∼                                     127                                              -128 \sim 127                              −128∼127</td>
<td align="center">int16</td><td align="center">有</td><td align="center">2字节</td><td align="center">                                                       −                                                   2                                        15                                                  ∼                                                   2                                        15                                                  −                                     1                                              - 2^{15} \sim 2^{15}-1                              −215∼215−1</td>
<td align="center">int32</td><td align="center">有</td><td align="center">4字节</td><td align="center">                                                       −                                                   2                                        31                                                  ∼                                                   2                                        31                                                  −                                     1                                              - 2^{31} \sim 2^{31}-1                              −231∼231−1</td>
<td align="center">int64</td><td align="center">有</td><td align="center">8字节</td><td align="center">                                                       −                                                   2                                        63                                                  ∼                                                   2                                        63                                                  −                                     1                                              - 2^{63} \sim 2^{63}-1                              −263∼263−1</td>

案例如下：

```
package main
import "fmt"

func main() {
	var i int = 1
	fmt.Println("i =", i)

	var j int8 = 127
	fmt.Println("j =", j)
}

```

输出结果如下图所示：

**(2) 无符号整形**

<th align="center">类型</th><th align="center">有无符号</th><th align="center">占用存储空间</th><th align="center">表数范围</th>
|------
<td align="center">uint8</td><td align="center">无</td><td align="center">1字节</td><td align="center">                                                       0                                     ∼                                     255                                              0 \sim 255                              0∼255</td>
<td align="center">uint16</td><td align="center">无</td><td align="center">2字节</td><td align="center">                                                       0                                     ∼                                                   2                                        16                                                  −                                     1                                              0 \sim 2^{16}-1                              0∼216−1</td>
<td align="center">uint32</td><td align="center">无</td><td align="center">4字节</td><td align="center">                                                       0                                     ∼                                                   2                                        32                                                  −                                     1                                              0 \sim 2^{32}-1                              0∼232−1</td>
<td align="center">uint64</td><td align="center">无</td><td align="center">8字节</td><td align="center">                                                       0                                     ∼                                                   2                                        64                                                  −                                     1                                              0 \sim 2^{64}-1                              0∼264−1</td>

案例如下，如果uint8赋值为256则会提示边界溢出“.\type03_01.go:14:6: constant 256 overflows uint8”。

**(3) 其他int类型**

<th align="center">类型</th><th align="center">有无符号</th><th align="center">占用存储空间</th><th align="center">表数范围</th>
|------
<td align="center">int</td><td align="center">有</td><td align="center">32位系统4个字节<br/>64位系统8个字节</td><td align="center">                                                       −                                                   2                                        31                                                  ∼                                                   2                                        31                                                  −                                     1                                                 −                                                   2                                        63                                                  ∼                                                   2                                        63                                                  −                                     1                                              - 2^{31} \sim 2^{31}-1 \\ - 2^{63} \sim 2^{63}-1                              −231∼231−1−263∼263−1</td>
<td align="center">uint</td><td align="center">无</td><td align="center">32位系统4个字节<br/>64位系统8个字节</td><td align="center">                                                       0                                     ∼                                                   2                                        32                                                  −                                     1                                                 0                                     ∼                                                   2                                        64                                                  −                                     1                                              0 \sim 2^{32}-1 \\ 0 \sim 2^{64}-1                              0∼232−10∼264−1</td>
<td align="center">rune</td><td align="center">有</td><td align="center">与int32等价，表示一个unicode码，常用处理中文</td><td align="center">                                                       −                                                   2                                        31                                                  ∼                                                   2                                        31                                                  −                                     1                                              - 2^{31} \sim 2^{31}-1                              −231∼231−1</td>
<td align="center">byte</td><td align="center">无</td><td align="center">与uint8等价，存储字符选用</td><td align="center">                                                       0                                     ∼                                     255                                              0 \sim 255                              0∼255</td>

案例如下：

```
package main
import "fmt"

func main() {
	var i int = -20
	fmt.Println("i =", i)

	var j uint = 127
	fmt.Println("j =", j)

	var k rune = 1024
	fmt.Println("k =", k)

	var m byte = 255
	fmt.Println("m =", m)
}

```

输出结果如下：

**(4) 整形的注意事项**

```
package main
import "fmt"
import "unsafe"

func main() {
	//查看变量的数据类型
	//fmt.Printf() 用于格式化输出
	var num int = 1024
	fmt.Println("num =", num)
	fmt.Printf("num 的数据类型是 %T \n", num)

	//查看变量占用的空间大小
	var n int64 = 10
	var name = "eastmount"
	fmt.Printf("n 的数据类型是 %T; 占用字节数是 %d \n", n, unsafe.Sizeof(n))
	fmt.Printf("name 的数据类型是 %T; 占用字节数是 %d \n", name, unsafe.Sizeof(name))

	//Golang程序中整型变量使用尽量遵守保小原则
	//在保证程序正确运行下，尽量使用空间小的数据类型
	var age byte = 28
	fmt.Printf("n 的数据类型是 %T; 占用字节数是 %d \n", age, unsafe.Sizeof(age))
}

```

输出结果如下图所示：

---


### 2.浮点型

浮点型用于存放小数，比如3.14、-1.9等，两种类型如下（没有double类型）。

浮点数都是有符号的，浮点数在机器中存放形式为：

浮点数的尾数部分可能丢失，造成精度损失。

```
package main
import "fmt"

func main() {
	//浮点数定义
	var price float32 = 89.12
	fmt.Println("price =", price)

	var num1 float32 = -0.00081
	var num2 float64 = -78942.00912
	fmt.Println("num1 =", num1, "num2 =", num2)
	
	//精度损失
	var num3 float32 = -123.0000203
	var num4 float64 = -123.0000203
	fmt.Println("\n精度损失")
	fmt.Println("num3 =", num3, "num4 =", num4)
}

```

输出结果如下图所示，可以看到float32精度损失。

**浮点数的注意事项**

```
package main
import "fmt"
import "unsafe"

func main() {
	//浮点数默认声明为 float64类型
	var num = 89.12
	fmt.Printf("num的数据类型是 %T; 占用字节数是 %d \n", num, unsafe.Sizeof(num))

	//十进制数形式
	num1 := 3.14
	num2 := .123
	fmt.Println("num1 =", num1, "num2 =", num2)

	//科学计数法形式
	num3 := 1.234e2      //1.234 * 10的2次方
	num4 := 1.234E2      //1.234 * 10的2次方
	num5 := 1.234E-2     //1.234 / 10的2次方
	fmt.Println("num3 =", num3, "num4 =", num4, "num5 =", num5)
}

```

输出结果如下图所示：

---


### 3.字符类型

Golang中没有专门的字符类型，如果要存储单个字符（字母），一般使用 <mark>byte</mark> 来保存（ASCII码表）。注意，是单个字母，而汉字是3个字节。

字符串是一串固定长度的字符连接起来的字符序列。Go的字符串是由单个字节连接起来的，也就是说传统的字符串是由字符组成的，<mark>**而Go的字符串不同，它是由字节组成的**</mark>。

举个简单示例：

```
package main
import "fmt"

func main() {
	//定义字符类型
	var c1 byte = 'a'
	var c2 byte = '6'

	//当我们直接输出byte值 即输出对应字符的ASCII码值
	fmt.Println("c1 =", c1)
	fmt.Println("c2 =", c2)

	//如果需要输出对应字符 需要使用格式化输出
	fmt.Printf("c1=%c c2=%c\n", c1, c2)

	//var c3 byte = '杨'
	//overflow溢出
	var c3 int = '杨'
	fmt.Printf("c3=%c c3对应码值=%d\n", c3, c3)
}

```

输出结果如下图所示，比如“杨”对应Unicode编码10进制为“26472”。由于ASCII码只能存储127个字符，英文字母和字符可以，但是中文字符或其他国家字符很多，故衍生出UTF-8其他编码方式。

**字符型的注意事项**

```
package main
import "fmt"

func main() {
	//直接赋值数字 格式化输出字符
	var c1 int = 22269   // 22269-&gt;'国'  120-&gt;'x'
	fmt.Printf("c1=%c\n", c1)

	//字符类型可以运算 相当于一个整数 运算时按码值运行
	var c2  = 10 + 'a'
	fmt.Printf("c3=%c c3对应码值=%d\n", c2, c2)
}

```

输出结果如下图所示：

**字符类型本质探讨**

---


### 4.布尔型

布尔（bool）类型数据只允许取值true和false，用于表示真和假。它仅占1个字节，常用于逻辑运算，适用于程序流程控制（后续详细介绍）。

下面举个简单的案例：

```
package main
import "fmt"
import "unsafe"

func main() {
	//定义数据类型
	var num = false
	fmt.Println("num =", num)

	//注意bool类型占用1个字节 只能取true或false
	fmt.Println("占用空间 =", unsafe.Sizeof(num))
}

```

输出结果如下：

**注意：Go语言中bool类型不可以使用0或非0的整数替代false或true，这和C语言不同。**

---


### 5.字符串类型

字符串是一串固定长度的字符连接起来的字符序列。Go字符串是由单个字节连接起来的。Go语言的字符串的字节使用UTF-8编码标识Unicode文本。

```
package main
import "fmt"

func main() {
	//定义字符串类型
	var name string = "我的名字叫Eastmount!"
	fmt.Println(name)
}

```

输出结果如下图所示：

**字符串的注意事项**

**字符串两种表示形式**

```
package main
import "fmt"

func main() {
	//双引号字符串
	str1 := "Eastmount\nCSDN"
	fmt.Println(str1)

	//反引号字符串 ``
	str2 := `
	package main
	import "fmt"

	func main() {
		//定义字符串类型
		var name string = "我的名字叫Eastmount!"
		fmt.Println(name)
	}
	`
	fmt.Println(str2)
}

```

输出结果如下图所示：

**字符串拼接**

当一行字符串太长时，需要使用到多行字符串，可以进行如下处理：

---


### 6.基本数据类型的默认值

在Golang中，数据类型都有一个默认值，当程序员没有赋初值时，它就会保留默认值（或零值）。常见默认值如下图所示：

举例如下：

---


### 7.基本数据类型转换

Golang和Java、C不同，Go在不同类型的变量之间赋值时需要**显示转换**。换句话说，Golang中数据类型不能自动转换。

**强制转换基本语法**

```
package main
import "fmt"

func main() {
	//数据类型转换
	var a int32 = 100

	//整型-&gt;浮点型
	var b float32 = float32(a)
	var c int8 = int8(a)

	//低精度-&gt;高精度
	var d int64 = int64(a)
	fmt.Printf("a=%v b=%v c=%v d=%v\n", a, b, c, d)
	fmt.Printf("a=%T b=%T c=%T d=%T\n", a, b, c, d)

	//浮点型-&gt;整型
	var e float32 = 3.14
	var f int32 = int32(e)
	fmt.Printf("e=%v f=%v\n", e, f)
	fmt.Printf("e=%T f=%T\n", e, f)
}

```

输出结果如下图所示：

**注意事项**

由于n3是int8类型，赋值语句含128所以编译不通过；而n4也是int8类型，但编译能通过，结果溢出处理。

---


### 8.基本数据类型和string转换

在程序开发中，经常将基本数据类型转换成string或将string转换成基本数据类型。

**(1) 基本数据类型转换成string**

代码如下：

```
package main
import "fmt"

func main() {
	//变量定义
	var num1 int = 99
	var num2 float64 = 3.14
	var b bool = true
	var char byte = 'h'
	var str string
	
	//fmt.Sprintf转换
	str = fmt.Sprintf("%d", num1)
	fmt.Printf("str type %T str=%q\n", str, str)
	
	str = fmt.Sprintf("%f", num2)
	fmt.Printf("str type %T str=%q\n", str, str)

	str = fmt.Sprintf("%t", b)
	fmt.Printf("str type %T str=%q\n", str, str)

	str = fmt.Sprintf("%c", char)
	fmt.Printf("str type %T str=%q\n", str, str)
}

```

输出结果如下图所示：

```
package main
import "fmt"
import "strconv"

func main() {
	//变量定义
	var num1 int = 99
	var num2 float64 = 3.14
	var b bool = true
	var num3 int64 = 4567
	var str string
	
	//strconv.FormatInt 10进制
	str = strconv.FormatInt(int64(num1), 10)
	fmt.Printf("str type %T str=%q\n", str, str)

	//strconv.FormatFloat(num2, 'f', 10, 64)
	//说明：'f'格式 10表示小数位保留10位 64表示小数float64
	str = strconv.FormatFloat(num2, 'f', 10, 64)
	fmt.Printf("str type %T str=%q\n", str, str)

	str = strconv.FormatBool(b)
	fmt.Printf("str type %T str=%q\n", str, str)

	str = strconv.Itoa(int(num3))
	fmt.Printf("str type %T str=%q\n", str, str)
}

```

输出结果如下图所示：

**(2) string类型转基本数据类型**<br/> 使用strconv包的函数：

需要说明，因为返回的是int64或float64，如希望得到int32、float32，则需要调用 int32(num)处理。

注意：在将String类型转换成基本数据类型时，**要确保String类型能够转成有效的数据**。比如把“123”转成一个整数，但不能把“hello”转成一个整数；如果这样Go直接将其转成0，其他类型也是同样的道理，float转成0、bool转成false。

---


## 三.指针

### 1.基本介绍

由于后续很多内容（如引用）都会涉及到指针，C语言中它也是一个难点，因此这里我们先介绍指针的基础知识，更好地帮助我们学习Golang。

对于基本数据类型来说，变量存的就是值，也叫值类型。获取变量的地址使用“&amp;”，比如：

```
package main
import "fmt"

func main() {
	//基本数据类型在内存布局
	var i int = 10
	//i的地址 &amp;i
	fmt.Println("i的地址=", &amp;i)
	fmt.Println("i的值=", i)
}

```

输出结果：

```
i的地址= 0xc0000100a0
i的值= 10

```

---


### 2.指针类型

指针变量存的是一个地址，这个地址指向的空间存的才是值，比如：

举例说明指针在内存的布局。

```
package main
import "fmt"

func main() {
	//基本数据类型在内存布局
	var i int = 10
	//i的地址 &amp;i
	fmt.Println("i的地址=", &amp;i)
	fmt.Println("i的值=", i)

	/*
	 var ptr *int = &amp;i
	   1.ptr是一个指针变量
	   2.ptr的类型是*int
	   3.ptr本身的值是&amp;i
	*/
	var ptr *int = &amp;i
	fmt.Printf("ptr=%v\n", ptr)
}

```

输出结果如下图所示：

---


### 3.获取指针类型所指向的值

使用 * 实现，比如 var ptr **int，使用**ptr获取ptr指向的值。

```
package main
import "fmt"

func main() {
	//基本数据类型在内存布局
	var i int = 10
	//i的地址 &amp;i
	fmt.Println("i的地址=", &amp;i)
	fmt.Println("i的值=", i)

	/*
	 var ptr *int = &amp;i
	   1.ptr是一个指针变量
	   2.ptr的类型是*int
	   3.ptr本身的值是&amp;i
	*/
	var ptr *int = &amp;i
	fmt.Printf("ptr=%v\n", ptr)

	//获取指针类型指向的值
	fmt.Printf("ptr的地址=%v\n", &amp;ptr)
	fmt.Printf("ptr指向的值=%v\n", *ptr)
}

```

输出结果如下所示：

举例说明：

---


### 4.指针修改值

<mark>编写一个程序，获取一个int变量num的地址并显示终端；再将num的地址赋值给指针ptr，通过ptr去修改num的值。</mark>

```
package main
import "fmt"

func main() {
	//基本数据类型
	var num int = 10
	fmt.Printf("num的地址=%v 原始值=%v\n", &amp;num, num)

	//指针
	var ptr *int
	ptr = &amp;num
	fmt.Printf("ptr的地址=%v 指向的值为=%v 自身=%v\n", &amp;ptr, *ptr, ptr)

	//修改值
	*ptr = 666
	fmt.Printf("num修改后的值=%v\n", num)
}

```

输出结果如下图所示：

下面这三个练习也推荐大家尝试。

---


### 5.值类型和引用类型

<mark>(1) 值类型</mark>

<mark>(2) 引用类型</mark>

内存的栈区和堆区示意图如下：

---


## 四.标识符和关键字

### 1.标识符

Golang对各种变量、方法和函数等命名时使用的字符序列称为标识符。凡是自己可以起名字的地方也都叫标识符。其命名规则如下：

标识符命名注意事项：

举例说明，首先在utils.go中定义一个变量；然后在main.go中使用该变量。

untils.go

```
package model

//定义一个变量
var HeroName string = "武松"

```

main.go

```
package main
import (
	"fmt"
	//导入utils.go文件的变量或函数 引入该model包
	"go_code/chapter03/datatype/model"
)

func main() {
	//该区域的数据可以在同一类型范围内变化
	var n int = 10
	n = 30
	n = 50
	fmt.Println("n=", n)

	//使用utils.go的HeroName变量 包名.标志符
	fmt.Println(model.HeroName)
}

```

输出结果如下图所示：

如果变量名定义时是小写“heroNam”则会报错

---


### 2.关键字

在Go中，为简化代码编译过程中对代码的解析，系统仅保留25个关键字，如下表所示：

除了保留关键字外，Go还提供了36个预定的标志符，包括基础数据类型和系统内嵌函数。

---


## 五.GO编程练习

### 1.题目

**(1) 分别定义常见的数据类型（整型、浮点型、字符型、布尔型、字符串型）变量，输出对应结果并查看变量的空间大小、数据类型。**

```
变量名称        数据类型        占用空间        对应的值
num1     int     8       -12
num2     uint8   1       127
num3     int64   8       12345
num4     float32         4       3.14
num5     float64         8       314.15
num6     int     8       26472
num7     bool    1       false
num8     string          16      Eastmount

```

**(2) 判断数字9的奇偶性输出它是奇数或偶数。**

```
num = 9
这是一个奇数

```

**(3) 有人用温度计测量出华氏法表示的温度（如69°F），先要求把它转换为以摄氏法表示的温度（如20°C），输入值为69。**

```
华氏度 f= 69
摄氏度 c= 20.555555555555557
摄氏度取整 c= 20

```

**(4) 通过两种方法（调用函数和循环）实现计算字符串“Eastmount”长度。**

```
The length of "Eastmount" is 9.
E a s t m o u n t
The length of "Eastmount" is 9.

```

**(5) 循环依次输出“East 秀璋”字符串的所有字符。**

```
Unicode遍历字符串
Unicode: E  69
Unicode: a  97
Unicode: s  115
Unicode: t  116
Unicode:    32
Unicode: 秀  31168
Unicode: 璋  29835

utf-8遍历字符串
Unicode: E  69
Unicode: a  97
Unicode: s  115
Unicode: t  116
Unicode:    32
Unicode: ç  231
Unicode: §  167
Unicode: €  128
Unicode: ç  231
Unicode: ’  146
Unicode: ‹  139

```

**(6) 实现字符串循环拼接，将变量str拼接成“a”到“z”并输出。**

```
abcdefghijklmnopqrstuvwxyz

```

**(7) 从键盘上输入整数、浮点数和字符，然后赋值给变量并输出结果。**

```
Eastmount
28
60.2
我的名字是: Eastmount
我的年龄是: 28
我的体重是: 60.2

```

**(8) 任意输入一个字母，实现大小写自动转换输出。**

```
请输入任意字母:
A
对应的ASCII码值: 65
A =&gt; a

请输入任意字母:
h
对应的ASCII码值: 104
h =&gt; H

```

**(9) 实现多种数据类型转换（int和float转换、float和string转换）。**

```
a=100 int32, b=100 float32
c=3.14 float32, d=3 int32
e=3.14 float64, f=3.140000 string
e=123.456 string, f=123.456 float64

```

**(10) 指针基本概念，定义变量i，然后指针ptr指向该值，输出对应值及地址。**

```
i的地址= 0xc0000100a0
i的值= 10
ptr=0xc0000100a0
ptr的地址=0xc000006030
ptr指向的值=10

```

**(11) 编写一个程序，获取一个int变量num的地址并显示终端；再将num的地址赋值给指针ptr，通过ptr去修改num的值。**

```
666
num的地址= 0xc0000100a0
num的值= 54
ptr的地址=0xc000006030
ptr指向的值=54

修改值后: num=512 0xc0000100a0
修改值后: ptr=512 0xc000006030

```

**(12) 输入a和b两个整数，调用指针按从大到小的顺序输出a和b。**

```
20 88
a=88, b=20
max=88, min=20    //p1和p2

```

---


### 2.解答

> 
注意：程序实现的方法由千万种，作者更多是提供一个场景让你独立思考，独立解决问题。希望你喜欢这些题目，不要觉得枯燥，很多题目后续都会结合实际项目及经验进行介绍。


<mark>**(1) 分别定义常见的数据类型（整型、浮点型、字符型、布尔型、字符串型）变量，输出对应结果并查看变量的空间大小、数据类型。**</mark>

```
package main
import "fmt"
import "unsafe"

func main() {
	var num1 int = -12
	fmt.Println("变量名称\t数据类型\t占用空间\t对应的值")
	fmt.Printf("num1 \t %T \t %d \t %v \n", num1, unsafe.Sizeof(num1), num1)

	var num2 byte = 127
	fmt.Printf("num2 \t %T \t %d \t %v \n", num2, unsafe.Sizeof(num2), num2)

	var num3 int64 = 12345
	fmt.Printf("num3 \t %T \t %d \t %v \n", num3, unsafe.Sizeof(num3), num3)

	var num4 float32 = 3.14
	fmt.Printf("num4 \t %T \t %d \t %v \n", num4, unsafe.Sizeof(num4), num4)

	var num5 float64 = 3.1415e2
	fmt.Printf("num5 \t %T \t %d \t %v \n", num5, unsafe.Sizeof(num5), num5)

	var num6 int = '杨'
	fmt.Printf("num6 \t %T \t %d \t %v \n", num6, unsafe.Sizeof(num6), num6)

	var num7 bool = false
	fmt.Printf("num7 \t %T \t %d \t %v \n", num7, unsafe.Sizeof(num7), num7)

	var num8 string = "Eastmount"
	fmt.Printf("num8 \t %T \t %d \t %v \n", num8, unsafe.Sizeof(num8), num8)
}

```

输出结果如下图所示：

---


<mark>**(2) 判断数字9的奇偶性输出它是奇数或偶数。**</mark>

```
package main
import "fmt"

func main() {
	var num int = 9
	fmt.Println("num =", num)

	//判断奇偶性
	if num % 2 == 0 {
		fmt.Println("这是一个偶数")
	} else {
		fmt.Println("这是一个奇数")
	}
}

```

输出结果如下图所示：

---


<mark>**(3) 有人用温度计测量出华氏法表示的温度（如69°F），先要求把它转换为以摄氏法表示的温度（如20°C），输入值为69。**</mark>

```
package main
import "fmt"

func main() {
	var f float64
	var c float64

	//温度转换
	f = 69.0
	c = (5.0 / 9) * (f - 32)
	fmt.Println("华氏度 f=", f)
	fmt.Println("摄氏度 c=", c)
	fmt.Println("摄氏度整数 c=", int64(c))
}

```

输出结果如下图所示：

---


<mark>**(4) 通过两种方法（调用函数和循环）实现计算字符串“Eastmount”长度。**</mark>

```
package main
import "fmt"
 
func main() {
    var str string
	str = "Eastmount"

	//计算字符串长度
	fmt.Printf("The length of \"%s\" is %d. \n", str, len(str))
	
	//循环计算字符串长度
	var num int = 0
	for _, s := range str {
		fmt.Printf("%c ", s)
		num += 1
	}
	fmt.Printf("\nThe length of \"%s\" is %d. \n", str, num)
}

```

输出结果如下图所示：

注意，当字符串中包含多字节字符时，要用到标准库utf8中的RuneCountInString函数来获取字符串的长度。代码如下：

```
package main
import (
    "fmt"
    "unicode/utf8"
)
 
func main() {
	//多字节字符
	test := "Hello, 世界"
	fmt.Println("bytes =", len(test))                      //bytes = 13
    fmt.Println("runes =", utf8.RuneCountInString(test))   //runes = 9
}

```

---


<mark>**(5) 循环依次输出“East 秀璋”字符串的所有字符。**</mark>

```
package main
import "fmt"
 
func main() {
	str := "East 秀璋"

	//方法1：Unicode遍历字符串
	fmt.Printf("Unicode遍历字符串\n")
	for _, s := range str {
		fmt.Printf("Unicode: %c  %d\n", s, s)
	}
	

	//方法2：utf-8遍历字符串
	fmt.Printf("utf-8遍历字符串\n")
	for i := 0; i &lt; len(str); i++ {
		ch := str[i]
		fmt.Printf("Unicode: %c  %d\n", ch, ch)
	}
}

```

输出结果如下图所示：

---


<mark>**(6) 实现字符串循环拼接，将变量str拼接成“a”到“z”并输出。**</mark>

```
package main
import "fmt"
 
func main() {
	var str string
	var tt string
	var ch byte = 'a'

	for i := 0; i &lt; 26; i++ {
		tt = fmt.Sprintf("%c", ch)
		str += tt
		ch += 1
	}
	fmt.Println(str)
}

```

上述代码注意类型转换，输出结果如下图所示：

---


<mark>**(7) 从键盘上输入整数、浮点数和字符，然后赋值给变量并输出结果。**</mark>

```
package main
import "fmt"

func main() {
	var (
		name string
		age int
		weight float32
	)

	//从终端获取用户的输入内容
	fmt.Scan(&amp;name, &amp;age, &amp;weight)
	fmt.Println("我的名字是:", name)
	fmt.Println("我的年龄是:", age)
	fmt.Println("我的体重是:", weight)
}

```

输出结果如下图所示：

---


<mark>**(8) 任意输入一个字母，实现大小写自动转换输出。**</mark>

```
package main
import "fmt"

func main() {
	var ch byte
	var re byte

	//从终端获取用户的输入内容
	fmt.Println("请输入任意字母:")
	fmt.Scanf("%c", &amp;ch)
	fmt.Println("对应的ASCII码值:", ch)

	//大小写转换
	if ch &gt;= 'A' &amp;&amp; ch &lt;= 'Z' {
		re = ch + 32
	} else if ch &gt;= 'a' &amp;&amp; ch &lt;= 'z' {
		re = ch - 32
	}
	fmt.Printf("%c =&gt; %c", ch, re)
}

```

输出结果如下图所示：

---


<mark>**(9) 实现多种数据类型转换（int和float转换、float和string转换）。**</mark>

```
package main
import "fmt"
import "strconv"

func main() {
	//整型-&gt;浮点型
	var a int32 = 100
	var b float32 = float32(a)
	fmt.Printf("a=%v %T, b=%v %T\n", a, a, b, b)

	//浮点型-&gt;整型
	var c float32 = 3.14
	var d int32 = int32(c)
	fmt.Printf("c=%v %T, d=%v %T\n", c, c, d, d)

	//其他类型-&gt;string
	var e float64 = 3.14
	var f string
	f = fmt.Sprintf("%f", e)
	fmt.Printf("e=%v %T, f=%v %T\n", e, e, f, f)

	//string-&gt;其他类型
	var g string = "123.456"
	var h float64
	h, _ = strconv.ParseFloat(g, 64)
	fmt.Printf("e=%v %T, f=%v %T\n", g, g, h, h)
}

```

输出结果如下图所示：

---


<mark>**(10) 指针基本概念，定义变量i，然后指针ptr指向该值，输出对应值及地址。**</mark>

```
package main
import "fmt"

func main() {
	//基本数据类型在内存布局
	var i int = 10
	//i的地址 &amp;i
	fmt.Println("i的地址=", &amp;i)
	fmt.Println("i的值=", i)

	/*
	 var ptr *int = &amp;i
	   1.ptr是一个指针变量
	   2.ptr的类型是*int
	   3.ptr本身的值是&amp;i
	*/
	var ptr *int = &amp;i
	fmt.Printf("ptr=%v\n", ptr)

	//获取指针类型指向的值
	fmt.Printf("ptr的地址=%v\n", &amp;ptr)
	fmt.Printf("ptr指向的值=%v\n", *ptr)
}

```

输出结果如下图所示：

---


<mark>**(11) 编写一个程序，获取一个int变量num的地址并显示终端；再将num的地址赋值给指针ptr，通过ptr去修改num的值。**</mark>

```
package main
import "fmt"

func main() {
	//获取一个int变量num的地址并显示终端
	var num int
	fmt.Scanf("%c", &amp;num)
	fmt.Println("num的地址=", &amp;num)
	fmt.Println("num的值=", num)

	//定义ptr指针变量
	var ptr *int = &amp;num
	fmt.Printf("ptr的地址=%v\n", &amp;ptr)
	fmt.Printf("ptr指向的值=%v\n", *ptr)

	//通过ptr去修改num的值
	*ptr = 512
	fmt.Printf("修改值后: num=%d %v\n", num, &amp;num)
	fmt.Printf("修改值后: ptr=%d %v\n", *ptr, &amp;ptr)
}

```

输出结果如下图所示：

---


<mark>**(12) 输入a和b两个整数，调用指针按从大到小的顺序输出a和b。**</mark>

```
package main
import "fmt"

func main() {
	//变量定义
	var a int
	var b int
	
	//输入两个整数
	fmt.Println("请输入两个整数:")
	fmt.Scanf("%d %d", &amp;a, &amp;b)

	//p1指向a p2指向b
	var p1 *int = &amp;a
	var p2 *int = &amp;b
	var p *int

	//如果a&lt;b则交换p1与p2的值 a存储大值
	if a &lt; b {
		p = p1
		p1 = p2
		p2 = p
	}

	//从大到小输出a、b结果
	fmt.Printf("a=%v, b=%v\n", a, b)

	//输出p1和p2所指向变量的值
	fmt.Printf("max=%v, min=%v\n", *p1, *p2)
}

```

输出结果如下图所示，但遗憾的是a和b值并没有交换，而p1确实指向较大值，p2指向较小值。

这与C语言也存在一些差异，C语言代码如下，请大家下来思考具体原因。

---


## 六.总结

写到这里，这篇基础性Golang文章介绍完毕，希望您喜欢！

Go基本概念和数据类型了解后，后面的文章将详细介绍Go语言的运算、条件语句和循环语句知识，并结合案例进行普及。希望这篇基础性文章对您有帮助，写得不好的地方还请海涵。同时非常感谢参考文献中的大佬们的文章分享，尤其是韩顺平老师，深知自己很菜，得努力前行。也希望自己能深入下去，未来四年好好研究Go编程语言，做更多实际工程，写更好的文章，共勉！

源代码下载地址：

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:娜璋AI之家 Eastmount 2021-01-31 星期天 夜于贵阳 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount))

---


**参考文献：**

---

