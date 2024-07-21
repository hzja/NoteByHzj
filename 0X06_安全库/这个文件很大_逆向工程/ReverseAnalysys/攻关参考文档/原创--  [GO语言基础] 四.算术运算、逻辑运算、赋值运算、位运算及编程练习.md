# 原创
：  [GO语言基础] 四.算术运算、逻辑运算、赋值运算、位运算及编程练习

# [GO语言基础] 四.算术运算、逻辑运算、赋值运算、位运算及编程练习

作为网络安全初学者，会遇到采用Go语言开发的恶意样本。因此从今天开始从零讲解Golang编程语言，一方面是督促自己不断前行且学习新知识；另一方面是分享与读者，希望大家一起进步。**前文介绍了Golang的变量、数据类型和标识符知识，并通过12道编程练习进行提升。这篇文章将介绍运算，包括算术运算、逻辑运算、赋值运算、位运算及编程练习。** 这系列文章入门部分将参考“尚硅谷”韩顺平老师的视频和书籍《GO高级编程》，详见参考文献，并结合作者多年的编程经验进行学习和丰富，且看且珍惜吧！后续会结合网络安全进行GO语言实战深入，加油~

这些年我学过各种编程语言，从最早的C语言到C++，再到C#、PHP、JAVA，再到IOS开发、Python，到最新的GO语言，学得是真的杂。有时候觉得编程语言恰恰是最简单的，而通过一门编程语言能够解决实际问题或深入底层才是其价值所在，并且当我们学好一门编程语言后，其他编程语言都非常类似，殊途同归，学起来也很迅速。

**源码下载地址：**

前文参考：

#### 文章目录

---


运算符是一种特殊的符号，用以表示数据的运算、赋值和比较等。常见的运算包括算术运算符、赋值运算符、比较运算符（关系运算符）、逻辑运算符、位运算符和其他运算符。

## 一.算术运算

算术运算符是对数值类型的变量进行运算，比如：加减乘除。在Go语言中也使用非常多，常用算术运算符如下表所示。

<th align="center">运算符</th><th align="center">运算</th><th align="center">示例</th><th align="center">结果</th>
|------
<td align="center">+</td><td align="center">正号</td><td align="center">+3</td><td align="center">3</td>
<td align="center">-</td><td align="center">负号</td><td align="center">-4</td><td align="center">-4</td>
<td align="center">+</td><td align="center">加</td><td align="center">5+5</td><td align="center">10</td>
<td align="center">-</td><td align="center">减</td><td align="center">6-4</td><td align="center">2</td>
<td align="center">*</td><td align="center">乘</td><td align="center">3*4</td><td align="center">12</td>
<td align="center">/</td><td align="center">除</td><td align="center">5 / 5</td><td align="center">1</td>
<td align="center">%</td><td align="center">取模(取余)</td><td align="center">7 % 5</td><td align="center">2</td>
<td align="center">++</td><td align="center">自增</td><td align="center">a=1 a++</td><td align="center">a=2</td>
<td align="center">- -</td><td align="center">自减</td><td align="center">a=1 a- -</td><td align="center">a=0</td>
<td align="center">+</td><td align="center">字符串相加</td><td align="center">“East” + “mount”</td><td align="center">“Eastmount”</td>

具体案例如下，重点介绍除法、取余、自增和自减。

```
package main
import "fmt"

func main() {
	//加法 减法 乘法
	var a int = 10
	var b int = 4
	var n1, n2, n3 int
	n1 = a + b
	n2 = a - b
	n3 = a * b
	fmt.Println("加法:", n1)
	fmt.Println("减法:", n2)
	fmt.Println("乘法:", n3)

	//除法：如果运算数都是整数，那么除后去掉小数部分，保留整数部分
	var n4 float32 = 10 / 4
	fmt.Println("除法:", n4)
	
	//除法：如果需要保留小数部分，则需要有浮点数参与运算
	var n5 float32 = 10.0 / 4
	fmt.Println("除法:", n5)

	//余数(%)：a % b = a - a / b * b
	fmt.Println("10 % 3 =", 10 % 3)      //=10-10/3*3=1
	fmt.Println("-10 % 3 =", -10 % 3)    //=-10-(-10)/3*3=-1
	fmt.Println("10 % -3 =", 10 % -3)    //=10-10/(-3)*3=1
	fmt.Println("-10 % -3 =", -10 % -3)  //=-10-(-10)/(-3)*(-3)=-1

	//++和--使用
	var i int = 10
	i++               //i = i + 1
	fmt.Println("自增:", i)
	i--               //i = i - 1
	fmt.Println("自减:", i)
}

```

输出结果如下图所示：

算术运算符使用的注意事项如下：

**简单案例分析：**

代码如下所示：

```
package main
import "fmt"

func main() {
	//假如还有97天放假，求包括多少个星期零多少天
	var num int = 97
	var week int
	var  day int
	week = num / 7
	day = num / 7
	fmt.Printf("%d个星期,零%d天 \n", week, day)

	//定义一个变量保存华氏度
	//摄氏度=5/9*(华氏度-32)
	var huashi float32 = 134.2
	var sheshi float32
	sheshi = 5.0 / 9 * (huashi - 32)
	fmt.Printf("%v 对应的摄氏温度 =%v \n", huashi ,sheshi)
}

```

输出结果如下图所示：

---


## 二.关系运算

关系运算符又称为比较运算符，其运算结果都是bool型，要么是true，要么是false。关系表达式经常出现在if结构的条件中，或循环结构for的条件中。

<th align="center">运算符</th><th align="center">运算</th><th align="center">示例</th><th align="center">结果</th>
|------
<td align="center">==</td><td align="center">相等于</td><td align="center">4==3</td><td align="center">false</td>
<td align="center">!=</td><td align="center">不等于</td><td align="center">4!=3</td><td align="center">true</td>
<td align="center">&lt;</td><td align="center">小于</td><td align="center">4&lt;3</td><td align="center">false</td>
<td align="center">&gt;</td><td align="center">大于</td><td align="center">4&gt;3</td><td align="center">true</td>
<td align="center">&lt;=</td><td align="center">小于等于</td><td align="center">4&lt;=3</td><td align="center">false</td>
<td align="center">&gt;=</td><td align="center">大于等于</td><td align="center">4&gt;=3</td><td align="center">true</td>

下面展示一个简单的代码：

```
package main
import "fmt"

func main() {
	var n1 int = 9
	var n2 int = 8

	//关系运算符使用
	fmt.Println(n1 == n2)    //false
	fmt.Println(n1 != n2)    //true
	fmt.Println(n1 &gt; n2)     //true
	fmt.Println(n1 &gt;= n2)    //true
	fmt.Println(n1 &lt; n2)     //false
	fmt.Println(n1 &lt;= n2)    //false
	flag := n1 &gt; n2
	fmt.Println("flag =", flag)
}

```

输出结果如下图所示：

关系运算符的注意事项：

---


## 三.逻辑运算

逻辑运算用于连接多个条件，通常是关系表达式，最终结果是一个bool值。假定A值为True，B值为False，则逻辑运算的结果描述如下：

<th align="center">运算符</th><th align="center">运算</th><th align="center">描述</th><th align="center">结果</th>
|------
<td align="center">&amp;&amp;</td><td align="center">**逻辑与**运算符</td><td align="center">如果两边的操作数都是True，则为True，否则为False</td><td align="center">(A&amp;&amp;B)为False</td>
<td align="center">||</td><td align="center">**逻辑或**运算符</td><td align="center">如果两边的操作数有一个True，则为True，否则为False</td><td align="center">(A || B)为True</td>
<td align="center">!</td><td align="center">**逻辑非**运算符</td><td align="center">如果条件为True，则逻辑为False，否则为True</td><td align="center">!(A&amp;&amp;B)为True</td>

下面展示一个简单的代码：

```
package main
import "fmt"

func main() {
	var age int = 28

	//逻辑与运算符 &amp;&amp;
	if age &gt; 20 &amp;&amp; age &lt; 30 {
		fmt.Println("age&gt;20 &amp;&amp; age&lt;30")
	}

	if age &gt; 30 &amp;&amp; age &lt; 40 {
		fmt.Println("age&gt;30 &amp;&amp; age&lt;40")
	}

	//逻辑或运算符 ||
	if age &gt; 20 || age &lt; 30 {
		fmt.Println("age&gt;20 || age&lt;30")
	}

	if age &gt; 30 || age &lt; 40 {
		fmt.Println("age&gt;30 || age&lt;40")
	}

	//逻辑非运算符 !
	if age &gt; 30 {
		fmt.Println("age&gt;30")
	}

	if !(age &gt; 30) {
		fmt.Println("!(age&gt;30)")
	}
}

```

输出结果如下图所示：

逻辑运算符的注意事项：

案例如下：

```
package main
import "fmt"

//测试函数
func test() bool {
	fmt.Println("CSDN Eastmount...")
	return true
}

func main() {
	var age int = 28

	//短路与
	//由于 age&lt;30 为false 因此后面test()函数不执行
	if age &lt; 20 &amp;&amp; test() {
		fmt.Println("短路与判断")
	}

	//短路或
	//由于 age&gt;20 为true 因此后面test()函数不执行
	if age &gt; 20 &amp;&amp; test() {
		fmt.Println("短路或判断")
	}
}

```

输出结果如下图所示，与运算中的条件输出未执行，或运算的条件输出。

---


## 四.赋值运算

赋值运算就是将某个运算后的值，赋值给指定的变量。赋值运算符如下表所示：

<th align="center">运算符</th><th align="center">描述</th><th align="center">结果</th>
|------
<td align="center">=</td><td align="center">赋值运算符，将一个表达式的值赋给左边</td><td align="center">C=A+B 赋值给 C</td>
<td align="center">+=</td><td align="center">相加后再赋值</td><td align="center">C+=A 等于 C=C+A</td>
<td align="center">-=</td><td align="center">相减后再赋值</td><td align="center">C-=A 等于 C=C-A</td>
<td align="center">*=</td><td align="center">相乘后再赋值</td><td align="center">C*=A 等于 C=C*A</td>
<td align="center">/=</td><td align="center">相除后再赋值</td><td align="center">C/=A 等于 C=C/A</td>
<td align="center">%=</td><td align="center">求余后再赋值</td><td align="center">C%=A 等于 C=C%A</td>
<td align="center">&lt;&lt;=</td><td align="center">左移后再赋值</td><td align="center">C&lt;&lt;=2 等于 C=C&lt;&lt;2</td>
<td align="center">&gt;&gt;=</td><td align="center">右移后再赋值</td><td align="center">C&gt;&gt;=2 等于 C=C&gt;&gt;2</td>
<td align="center">&amp;=</td><td align="center">按位与后再赋值</td><td align="center">C&amp;=2 等于 C=C&amp;2</td>
<td align="center">^=</td><td align="center">按位异或再赋值</td><td align="center">C ^ =2 等于 C=C ^ 2</td>
<td align="center">|=</td><td align="center">按位或后再赋值</td><td align="center">C|=2 等于 C=C|2</td>

> 
注意，这部分的赋值运算涉及到了二进制相关之后，后面会详细介绍。


下面介绍赋值运算的案例知识，基本使用包括：

```
package main
import "fmt"

//测试函数
func test() int {
	var num int = 10
	return num
}

func main() {
	var a int = 3
	var b int = 4
	var c int
	var d int

	//赋值运算执行顺序是从右到左
	c = a + 3
	fmt.Println("a=", a, "c=", c)

	//赋值运算左边只能是变量 右边可以使变量、表达式、常量
	//表达式：任何有值都可以看做表达式
	d = a
	d = 8 + 2 * 5
	d = test() + 90
	fmt.Println(d)

	//运算后赋值
	fmt.Println("b =", b)
	b += 2
	fmt.Println(b)
	b -= 3
	fmt.Println(b)
	b *= 2
	fmt.Println(b)
	b /= 3
	fmt.Println(b)
	b %= 2
	fmt.Println(b)
}

```

输出结果如下图所示：

**案例1**：变量a和b的值进行交换，并输出结果。

```
package main
import "fmt"

func main() {
	//赋值运算符
	a := 9
	b := 2

	//有两个变量a和b 要求将其进行交换并输出结果
	fmt.Printf("交换前的结果 a = %v , b = %v \n", a, b)

	//定义一个临时变量
	t := a
	a = b
	b = t

	//输出交换后的结果
	fmt.Printf("交换后的结果 a = %v , b = %v \n", a, b)
}

```

输出结果如下：

**案例2**：求两个数10和20的最大值。

```
package main
import "fmt"

func main() {
	//求两个数的最大值
	var a int = 10
	var b int = 20
	var max int
	
	if a &gt; b {
		max = a
	} else {
		max = b
	}
	fmt.Println("max =", max)
}

```

输出结果如下图所示：

<mark>注意，Go语言明确不支持三元运算符，官方说明如下：</mark>

---


## 五.位运算

位运算如下表所示，后面讲解二进制时再进行举例说明。

<th align="center">运算符</th><th align="center">描述</th>
|------
<td align="center">&amp;</td><td align="center">按位与运算符“&amp;”是双目运算符，其功能是参与运算的两数各对应的二进制相与。<br/> 运算规则是：<mark>同时为1，结果为1，否则为0</mark></td>
<td align="center">|</td><td align="center">按位或运算符“|”是双目运算符，其功能是参与运算的两数各对应的二进制相或。<br/> 运算规则是：<mark>有一个为1，结果为1，否则为0</mark></td>
<td align="center">^</td><td align="center">按位异或运算符“^”是双目运算符，其功能是参与运算的两数各对应的二进制相异或。<br/> 运算规则是：<mark>当二进制位不同时，结果为1，否则为0</mark></td>
<td align="center">&lt;&lt;</td><td align="center">左移运算符“&lt;&lt;”是双目运算符，其功能是把“&lt;&lt;”左边的运算数的各二进制位全部左移若干位，<br/> 高位丢弃，低位补0。<mark>左移n位就是乘以2的n次方</mark></td>
<td align="center">&gt;&gt;</td><td align="center">右移运算符“&gt;&gt;”是双目运算符，其功能是把“&gt;&gt;”左边的运算数的各二进制位全部右移若干位，<br/> <mark>右移n位就是除以2的n次方</mark></td>

---


## 六.运算优先级

**1.其他运算符**

<th align="center">运算符</th><th align="center">描述</th><th align="center">示例</th>
|------
<td align="center">&amp;</td><td align="center">返回变量存储地址</td><td align="center">&amp;a；将给出变量的实际地址</td>
<td align="center">*</td><td align="center">指针变量</td><td align="center">*a；是一个指针变量</td>

下面补充个简单的示例：

```
package main
import "fmt"

func main() {
	//地址
	a := 100
	fmt.Println("a的地址=", &amp;a)
	fmt.Println("a的值=", a)

	//指针变量
	var ptr *int = &amp;a
	fmt.Println("ptr指向的值是=", *ptr)
	fmt.Println("ptr的地址是=", ptr)
}

```

输出结果如下图所示：

**2.运算符的优先级**<br/> 运算符有不同的优先级，所谓优先级就是表达式运算中的运算顺序。如下图所示，上一行运算符中优先于下一行。只有单目运算符、赋值运算符是从右向左运算的。优先级大概如下：

---


## 七.输入语句

在编程中，需要接收用户输入的数据，可以使用键盘输入语句来获取。

**func Scanln**<br/> Scanln类似Scan，但会在换行时才停止扫描。最后一个条目后必须有换行或者到达结束位置。

**func Scanf**<br/> Scanf从标准输入扫描文本，根据format参数指定的格式将成功读取的空白分隔的值保存进成功传送给本函数的参数。返回成功扫描的条目个数和遇到的任何错误。

下面通过案例进行演示输入语句，从控制台接收用户信息，包括：

**第一种方法是使用 fmt.Scanln() 获取内容**<br/> 当程序执行到 fmt.Scanln(&amp;name)，会停止在这里等待用户输入信息并回车

```
package main
import "fmt"

func main() {
	//从控制台接收用户信息，包括：姓名、年龄、薪水、是否通过GO课程考试
	var name string
	var age byte
	var money float32
	var isPass bool
	
	//输入
	fmt.Println("请输入姓名:")
	fmt.Scanln(&amp;name)
	fmt.Println("请输入年龄:")
	fmt.Scanln(&amp;age)
	fmt.Println("请输入薪水:")
	fmt.Scanln(&amp;money)
	fmt.Println("请输入是否通过GO语言课程:")
	fmt.Scanln(&amp;isPass)

	fmt.Printf("名字是 %v \n 年龄是 %v \n 薪水是 %v \n 考试 %v \n", name, age, money, isPass)
}

```

输出结果如下图所示：

**第二种方法是使用 fmt.Scanf() 获取内容**<br/> 该方法可以按照指定的格式输入

```
package main
import "fmt"

func main() {
	//从控制台接收用户信息，包括：姓名、年龄、薪水、是否通过GO课程考试
	var name string
	var age byte
	var money float32
	var isPass bool
	
	//输入空格间隔
	fmt.Println("请输入姓名, 年龄, 薪水, 是否通过考试")
	fmt.Scanf("%s %d %f %t", &amp;name, &amp;age, &amp;money, &amp;isPass)
	fmt.Printf("名字是 %v \n 年龄是 %v \n 薪水是 %v \n 考试 %v \n", name, age, money, isPass)
}

```

输出结果如下图所示：

---


## 八.进制转换

### 1.进制

对于证书，常见的四种表示方式为：

下面举个示例说明。

```
package main
import "fmt"

func main() {
	//二进制输出
	var i int = 10
	fmt.Printf("%b \n", i)

	//八进制：0-7, 满8进1，以数字0开头
	var j int = 011      //对应十进制8+1=9
	fmt.Println("j =", j)

	//十六进制：0-9及A-F，满16进1，以0x或0X开头
	var k int = 0x11    //对应十进制16+1=17
	fmt.Println("k =", k)
}

```

输出结果如下图所示：

> 
注意：前面的文章我们普及过一个小技巧。在GO语言所在文件夹中输入CMD会自动打开CMD，然后输入go run命令即可执行代码。


---


### 2.进制转换介绍

进制的图示如下：

常见转换方法如下：

<mark>**第一组：其他进制转十进制**</mark><br/> 比如：

```
134 = 4*1 + 3*10 + 4*100 = 4 + 30 + 100 = 134

```

<mark>二进制转十进制</mark>

 
      
       
        
        
          1011 
         
        
          = 
         
        
          1 
         
        
          ∗ 
         
         
         
           2 
          
         
           0 
          
         
        
          + 
         
        
          1 
         
        
          ∗ 
         
         
         
           2 
          
         
           1 
          
         
        
          + 
         
        
          0 
         
        
          ∗ 
         
         
         
           2 
          
         
           2 
          
         
        
          + 
         
        
          1 
         
        
          ∗ 
         
         
         
           2 
          
         
           3 
          
         
        
          = 
         
        
          1 
         
        
          + 
         
        
          2 
         
        
          + 
         
        
          0 
         
        
          + 
         
        
          8 
         
        
          = 
         
        
          11 
         
        
       
         1011 = 1*2^0 + 1*2^1 + 0*2^2 + 1*2^3 = 1 + 2 + 0 + 8 = 11 
        
       
     1011=1∗20+1∗21+0∗22+1∗23=1+2+0+8=11

<mark>八进制转十进制</mark>

 
      
       
        
        
          0123 
         
        
          = 
         
        
          3 
         
        
          ∗ 
         
         
         
           8 
          
         
           0 
          
         
        
          + 
         
        
          2 
         
        
          ∗ 
         
         
         
           8 
          
         
           1 
          
         
        
          + 
         
        
          1 
         
        
          ∗ 
         
         
         
           8 
          
         
           2 
          
         
        
          = 
         
        
          3 
         
        
          + 
         
        
          16 
         
        
          + 
         
        
          64 
         
        
          = 
         
        
          83 
         
        
       
         0123 = 3*8^0 + 2*8^1 + 1*8^2=3+16+64=83 
        
       
     0123=3∗80+2∗81+1∗82=3+16+64=83

<mark>十六进制转十进制</mark>

 
      
       
        
        
          0 
         
        
          x 
         
        
          34 
         
        
          A 
         
        
          = 
         
        
          10 
         
        
          ∗ 
         
        
          1 
         
         
         
           6 
          
         
           0 
          
         
        
          + 
         
        
          4 
         
        
          ∗ 
         
        
          1 
         
         
         
           6 
          
         
           1 
          
         
        
          + 
         
        
          3 
         
        
          ∗ 
         
        
          1 
         
         
         
           6 
          
         
           2 
          
         
        
          = 
         
        
          10 
         
        
          + 
         
        
          64 
         
        
          + 
         
        
          768 
         
        
          = 
         
        
          842 
         
        
       
         0x34A = 10*16^0 + 4*16^1 + 3*16^2 = 10 + 64 + 768 = 842 
        
       
     0x34A=10∗160+4∗161+3∗162=10+64+768=842

<mark>**第二组：十进制转其他进制**</mark>

<mark>十进制转二进制</mark>

<mark>十进制转八进制</mark>

<mark>十进制转十六进制</mark>

<mark>**第三组：二进制转其他进制**</mark>

<mark>二进制转八进制</mark>

 
      
       
        
        
          11 
         
        
          , 
         
        
          010 
         
        
          , 
         
        
          101 
         
        
          = 
         
        
          0325 
         
        
       
         11, 010, 101 = 0325 
        
       
     11,010,101=0325

<mark>二进制转十六进制</mark>

 
      
       
        
        
          1101 
         
        
          , 
         
        
          0101 
         
        
          = 
         
        
          0 
         
        
          x 
         
        
          D 
         
        
          5 
         
        
       
         1101, 0101 = 0xD5 
        
       
     1101,0101=0xD5

<mark>**第四组：其他进制转二进制**</mark>

<mark>八进制转二进制</mark>

 
      
       
        
        
          0237 
         
        
          = 
         
        
          10 
         
        
          , 
         
        
          011 
         
        
          , 
         
        
          111 
         
        
       
         0237 = 10, 011, 111 
        
       
     0237=10,011,111

<mark>十六进制转二进制</mark>

 
      
       
        
        
          0 
         
        
          x 
         
        
          237 
         
        
          = 
         
        
          10 
         
        
          , 
         
        
          0011 
         
        
          , 
         
        
          0111 
         
        
       
         0x237 = 10, 0011, 0111 
        
       
     0x237=10,0011,0111

---


### 3.位运算案例

接着回到之前的位运算，我们来理解下面这段代码。

```
package main
import "fmt"

func main() {
	var a int = 1 &gt;&gt; 2      //001  =&gt; 0
	var b int = -1 &gt;&gt; 2     
	var c int = 1 &lt;&lt; 2      //100  =&gt; 4
	var d int = -1 &lt;&lt; 2     //-100 =&gt; 4
	var e int = 7 &gt;&gt; 2      //111  =&gt; 1
	var f int = 7 &lt;&lt; 2      //111  =&gt; 11100=28

	fmt.Println("a=", a, "b=", b)
	fmt.Println("c=", c, "d=", d)
	fmt.Println("e=", e, "f=", f)
}

```

输出结果如下图所示：

我们接着再看一个案例。

```
package main
import "fmt"

func main() {
	fmt.Println(2&amp;3)
	fmt.Println(2|3)
	fmt.Println(13&amp;7)
	fmt.Println(5|4)
	fmt.Println(-3^3)
}

```

输出结果如下图所示，大家可以想想为什么？接下来作者即将介绍。

---


### 4.原码、反码、补码

二进制（Binary）是逢2进位的进位制，0、1是基本运算符。现代电子计算机技术采用的是二进制，因为它只使用0和1两个数字符号，非常简单方便，易于用电子方式实现。计算机内部处理的信息，都是采用二进制来表示的。

二进制数用0和1两个数字及其组合来表示任何数，进位规则是“逢2进1”，数字1在不同的位上代表不同的值，按从右至左的次序，这个值以二倍递增。在计算机内部，运行各种运算都是以二进制的方式来运行。

原码、反码和补码知识非常关键，韩顺平老师精简成了6条规则：

---


### 5.位运算符和移位运算符

Golang中3个位运算符规则如下：

举个案例：

对应的原理如下：

Golang中有2个移位运算符，规则如下：

案例：

---


## 九.编程练习

### 1.题目

**(1) 有两个变量，a和b，要求将其进行交换，但是不允许使用中间变量，最终打印结果。**

**(2) 求三个数中的最大值并输出结果。**

**(3) 给出三角形的三边长，求三角形面积。**

**(4) 输入两个整数，分别计算它们的与运算、或运算和异或运算结果。**

**(5) 输入3个数a、b、c，要求由小到大的顺序输出结果。**

**(6) 手动将下面的其他进制转换成十进制数。**

**(7) 手动将下面的十进制转换成其他进制数。**

**(8) 手动将下面的二进制转换成其他进制。**

---


### 2.解答

<mark>**(1) 有两个变量，a和b，要求将其进行交换，但是不允许使用中间变量，最终打印结果。**</mark>

```
package main
import "fmt"

func main() {
	//不引用第三个变量交换a和b
	var a int = 10
	var b int = 20
	fmt.Printf("原始值：a=%v b=%v \n", a, b)

	a = a + b
	b = a - b   //b = a + b - b ==&gt; b=a
	a = a - b   //a = a + b - a ==&gt; a=b

	fmt.Printf("交换值：a=%v b=%v \n", a, b)
}

```

输出结果如下图所示：

<mark>**(2) 求三个数中的最大值并输出结果。**</mark>

```
package main
import "fmt"

func main() {
	//求三个数中的最大值并输出结果
	var a int = 10
	var b int = 60
	var c int = 30
	var max int
	fmt.Printf("a=%v b=%v c=%v \n", a, b, c)

	if a &gt; b {
		max = a
	} else {
		max = b
	}

	if c &gt; max {
		max = c
	}

	fmt.Println("三个数中最大值是=", max)
}

```

输出结果如下图所示：

<mark>**(3) 给出三角形的三边长，求三角形面积。**</mark>

```
package main
import(
	"fmt"
	"math"
)

func main() {
	//给出三角形的三边长，求三角形面积
	var a float64 = 3.67
	var b float64 = 5.43
	var c float64 = 6.21
	var s float64
	var area float64

	s = (a + b + c) / 2
	area = math.Sqrt(s*(s-a)*(s-b)*(s-c))
	fmt.Printf("输出三条边: a=%v b=%v c=%v \n", a, b, c)
	fmt.Printf("area=%v \n", area)
}

```

输出结果如下图所示：

<mark>**(4) 输入两个整数，分别计算它们的与运算、或运算和异或运算结果。**</mark>

```
package main
import "fmt"

func main() {
	var a int
	var b int
	
	//输入
	fmt.Println("请输入a:")
	fmt.Scanln(&amp;a)
	fmt.Println("请输入b:")
	fmt.Scanln(&amp;b)

	//计算
	c := a &amp; b
	d := a | b
	e := a ^ b
	fmt.Printf("与运算 a=%v b=%v c=%v \n", a, b, c)
	fmt.Printf("或运算 a=%v b=%v d=%v \n", a, b, d)
	fmt.Printf("异或运算 a=%v b=%v e=%v \n", a, b, e)
}

```

输出结果如下图所示：

<mark>**(5) 输入3个数a、b、c，要求由小到大的顺序输出结果。**</mark>

```
package main
import "fmt"

func main() {
	var a int
	var b int
	var c int
	var t int
	
	//输入
	fmt.Println("请输入a:")
	fmt.Scanln(&amp;a)
	fmt.Println("请输入b:")
	fmt.Scanln(&amp;b)
	fmt.Println("请输入c:")
	fmt.Scanln(&amp;c)

	//排序
	if a &gt; b {
		t = a
		a = b
		b = t   //互换后 a是小数
	}
	if a &gt; c {
		t = a
		a = c
		c = t   //互换后 a是三者小数
	}
	if b &gt; c {
		t = b
		b = c
		c = t   //互换后 b是次小数
	}
	fmt.Printf("从小到到排序：a=%v b=%v c=%v \n", a, b, c)
}

```

输出结果如下图所示：

---


## 十.总结

写到这里，这篇基础性Golang文章介绍完毕，希望您喜欢！今天是2021年2月11日，祝大家新年快乐，牛气冲天，也希望自己能分享更优秀、更深入的文章。

Go基本运算和进制转换了解后，后面的文章将详细介绍Go语言的条件语句和循环语句知识，并结合案例进行普及。希望这篇基础性文章对您有帮助，写得不好的地方还请海涵。同时非常感谢参考文献中的大佬们的文章分享，尤其是韩顺平老师，深知自己很菜，得努力前行。也希望自己能深入下去，未来四年好好研究Go编程语言，做更多实际工程，写更好的文章，共勉！

源代码下载地址：

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:娜璋AI之家 Eastmount 2021-02-11 星期四 夜于贵阳 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount))

---


**参考文献：**

---

