# 原创
：  [GO语言基础] 五.顺序控制语句和条件控制语句（if、else、switch）

# [GO语言基础] 五.顺序控制语句和条件控制语句（if、else、switch）

作为网络安全初学者，会遇到采用Go语言开发的恶意样本。因此从今天开始从零讲解Golang编程语言，一方面是督促自己不断前行且学习新知识；另一方面是分享与读者，希望大家一起进步。**前文介绍了Golang的运算，包括算术运算、逻辑运算、赋值运算、位运算及编程练习。这篇文章将详细讲解顺序控制语句和条件控制语句。** 这系列文章入门部分将参考“尚硅谷”韩顺平老师的视频和书籍《GO高级编程》，详见参考文献，并结合作者多年的编程经验进行学习和丰富，且看且珍惜吧！后续会结合网络安全进行GO语言实战深入，加油~

这些年我学过各种编程语言，从最早的C语言到C++，再到C#、PHP、JAVA，再到IOS开发、Python，到最新的GO语言，学得是真的杂。有时候觉得编程语言恰恰是最简单的，而通过一门编程语言能够解决实际问题或深入底层才是其价值所在，并且当我们学好一门编程语言后，其他编程语言都非常类似，殊途同归，学起来也很迅速。

**源码下载地址：**

前文参考：

#### 文章目录

---


在程序中，程序运行的流程控制决定程序是如何执行的，也是大家必须要掌握的，主要有三大流程控制语句。这篇文章先介绍前两个流程控制。

## 一.顺序控制语句

**顺序控制是指程序从上到下逐行地执行，中间没有任何判断和跳转。其流程图如下所示：**

下面举个简单的案例说明。代码中没有判断，程序按照默认的流程执行，即顺序控制。

```
package main
import "fmt"

func main() {
	//假如还有97天放假 问多少个星期多少天
	var num int = 97
	var week int
	var day int
	week = num / 7
	day = num % 7
	fmt.Printf("%d个星期零%d天 \n", week, day)

	//定义一个变量保存华氏温度 转换成摄氏温度
	//摄氏温度=5/9*(摄氏温度-32)
	var huashi float32 = 134.2
	var sheshi float32
	sheshi = 5.0 / 9 * (huashi - 32)
	fmt.Printf("%v 对应的摄氏温度=%v \n", huashi, sheshi)
}

```

输出结果如下图所示：

注意，Golang中定义变量时采用合法的前向引用，比如正确形式：

```
func main() {
	var num1 int = 10          //声明num1
	var num2 int = num1 + 20   //使用num1
	fmt.Println(num2)
}

```

错误形式：

```
func main() {
	var num2 int = num1 + 20   //使用num1
	var num1 int = 10          //声明num1(x)
	fmt.Println(num2)
}

```

---


## 二.条件控制语句

分支控制是让程序有选择执行，包括三种常见形式：

### 1.单分支控制

基本语法如下：

```
if 条件表达式 {
	执行代码块
}

```

**说明：当条件表达式为True时，就会执行括号中的代码块。** 注意{}是必须有的，即使只写一行代码。对应的流程图如下所示：

案例分析：编写代码实现输入人的年龄，如果年龄大于18岁，则输出“你的年龄大于18岁”。

```
package main
import "fmt"

func main() {
	//输入年龄
	var age int
	fmt.Println("请输入年龄:")
	fmt.Scanln(&amp;age)

	//单分支判断
	if age &gt; 18 {
		fmt.Println("你的年龄大于18岁")
	}
}

```

输出结果如下图所示：

注意事项：

```
if n := 20; n &gt; 5 {
	fmt.Println("定义变量的值数字大于5")
}

```

---


### 2.双分支控制

基本语法如下：

```
if 条件表达式 {
	执行代码块1
} else {
	执行代码块2
}

```

**说明：当条件表达式为True时，否则执行代码块2。注意{}是必须有的，即使只写一行代码。** 对应的流程图如下所示：

案例分析：编写代码实现输入人的年龄，如果年龄大于18岁，则输出“你的年龄大于18岁”；否则输出“你的年龄小于等于18岁”。

```
package main
import "fmt"

func main() {
	//输入年龄
	var age int
	fmt.Println("请输入年龄:")
	fmt.Scanln(&amp;age)

	//单分支判断
	if age &gt; 18 {
		fmt.Println("你的年龄大于18岁")
	} else {
		fmt.Println("你的年龄小于等于18岁")
	}
}

```

输出结果如下图所示：

注意事项：

<mark>**条件语句嵌套**</mark><br/> 案例：对下列代码，若有输出，指出输出结果。

```
package main
import "fmt"

func main() {
	var x int = 4
	var y int = 1
	
	//双分支嵌套判断
	if (x &gt; 2) {
		if (y &gt; 2) {
			fmt.Println(x + y)
			fmt.Println("执行语句:----a----")
		}
		fmt.Println("执行语句:----b----")
	} else {
		fmt.Println("x is", x, "y is", y)
		fmt.Println("执行语句:----c----")
	}
}

```

输出结果如下图所示：

<mark>**条件判断**</mark>

```
package main
import "fmt"

func main() {
	//判断两个数之和大于等于50
	var n1 int32 = 10
	var n2 int32 = 50
	if n1 + n2 &gt;= 50 {
		fmt.Println("相加结果大于等于50")
	}

	//判断第一个数大于10.0，并且第二个数小于20.0
	var n3 float64 = 11.0
	var n4 float64 = 17.0
	if n3 &gt; 10.0 &amp;&amp; n4 &lt; 20.0 {
		fmt.Println("输出两数之和 =", (n3+n4))
	}
	
	//判断两者之和是否能被3又能被5整除
	var n5 int32 = 10
	var n6 int32 = 5
	if (n5 + n6) % 3 == 0 &amp;&amp; (n5 + n6) % 5 ==0 {
		fmt.Println("能被3和5整除")
	}
}

```

输出结果如下图所示：

<mark>错误案例1：编译错误，缺少括号{}</mark>

<mark>错误案例2：编译错误，else不能换行</mark>

---


### 3.多分支控制

基本语法如下：

```
if 条件表达式1 {
	执行代码块1
} else if 条件表达式2 {
	执行代码块2
}
...
 else {
 	执行代码块n
}

```

说明：

对应的流程图如下所示：

案例分析：编写代码实现考试成绩判断，如果成绩大于等于90则为A，如果成绩[80,90)则为B，依次类推不及格为E。

```
package main
import "fmt"

func main() {
	//输入变量
	var score int
	fmt.Println("请输入成绩:")
	fmt.Scanln(&amp;score)

	//多分支判断
	if score &gt;= 90 {
		fmt.Println("该同学成绩为A")
	} else if score &gt;=80 &amp;&amp; score &lt; 90 {
		fmt.Println("该同学成绩为B")
	} else if score &gt;=70 &amp;&amp; score &lt; 80 {
		fmt.Println("该同学成绩为C")
	} else if score &gt;=60 &amp;&amp; score &lt; 70 {
		fmt.Println("该同学成绩为D")
	} else {
		fmt.Println("该同学成绩为E")
	}
	fmt.Printf("成绩为:%d", score)
}

```

输出结果如下图所示：

注意事项：

**案例1：融合bool类型多分支判断**

```
package main
import "fmt"

func main() {
	//多分支判断
	var b bool = true
	if b == false {
		fmt.Println("a")
	} else if b {
		fmt.Println("b")
	} else if !b {
		fmt.Println("c")
	} else {
		fmt.Println("d")
	}
}

```

输出结果为“b”，如果将第一个判断修改成“b=false”是否能编译通过呢？答案是否，它会提示编译错误，因为if条件表达式不能是赋值语句。

**案例2：输入一个字符，判断：**

```
package main
import "fmt"

func main() {
	var ch byte
	var res byte
	fmt.Println("请输入字母:")
	fmt.Scanf("%c", &amp;ch)

	//大小写转换
	if ch &gt;= 'A' &amp;&amp; ch &lt;= 'Z' {
		res = ch + 32
	} else if ch &gt;= 'a' &amp;&amp; ch &lt;= 'z' {
		res = ch - 32
	} else {
		res = ch
	}
	fmt.Printf("输入字母:%c %v \n", ch, ch)
	fmt.Printf("输出结果:%c %v \n", res, res)
}

```

输出结果如下图所示，这道题目非常棒，希望大家熟练掌握。知识点包括：

---


### 4.嵌套分支

在一个分支结果中又完整嵌套另一个完整的分支结构，里面的分支结构称为内层分支，外面的分支结构称为外层分支。基本语法如下：

```
if 条件表达式 {
	if 条件表达式 {
		语句块
	} else {
		语句块
	}
	....
}

```

注意事项：

案例分析：

```
package main
import "fmt"

func main() {
	var sex string
	var second float64

	fmt.Println("请输入秒数:")
	fmt.Scanln(&amp;second)

	//嵌套分支
	if second &lt;=8 {
		fmt.Println("请输入性别:")
		fmt.Scanln(&amp;sex)
		if sex == "男" {
			fmt.Println("进入决赛的男子组")
		} else if sex == "女" {
			fmt.Println("进入决赛的女子组")
		} else {
			fmt.Println("性别输入错误")
		}
	} else {
		fmt.Println("淘汰未进入决赛")
	}
}

```

输出结果如图所示：

---


## 三.switch分支控制

switch语句用于基于不同条件执行不同动作，每个case分支都是唯一的，从上往下逐一测试，直到匹配为止。<mark>注意，匹配项后面不需要再加break</mark>。

基本语法如下：

```
switch 表达式 {

case 表达式1, 表达式2, ...:
	语句块1
case 表达式3, 表达式4, ...:
	语句块2
	....
default:
    语句块n
}

```

对应的流程图如下所示：

下面简单实现一个switch语句案例。请编写程序实现输入某个字符，然后a表示周一，b表示周二，c表示周三，根据用户的输入依次显示信息。

```
package main
import "fmt"

func main() {
	var key byte
	fmt.Println("请输入a到g的一个字符")
	fmt.Scanf("%c", &amp;key)

	//switch语句
	switch key {
		case 'a':
			fmt.Println("今天星期一")
		case 'b':
			fmt.Println("今天星期二")
		case 'c':
			fmt.Println("今天星期三")
		case 'd':
			fmt.Println("今天星期四")
		case 'e':
			fmt.Println("今天星期五")
		case 'f':
			fmt.Println("今天星期六")
		case 'g':
			fmt.Println("今天星期天")
		default:
			fmt.Println("输入有误....")
	}
}

```

输出结果如下图所示：

注意事项：<br/> <mark>(1) case和switch后是一个表达式，即常量值、变量、一个有返回值的函数等</mark>

(2) case后的各个表达式值的数据类型，必须和switch的表达式数据类型一致

(3) case后面可以带多个表达式，使用逗号间隔，如下图所示：

(4) case后的表达式如果是常量值，则要求不能重复

(5) case后面不需要带break，程序匹配到一个case后就会执行对应的代码块，然后退出switch，如果一个都匹配不到，则执行default

(6) default语句不是必须的

(7) switch后也可以不带表达式，类似于if-else分支来使用；同时case中也可以对范围进行判断

```
package main
import "fmt"

func main() {
	//switch后不接表达式 类似于if-else分支来使用
	var age int = 10

	switch {
		case age == 10:
			fmt.Println("年龄为10")
		case age == 20:
			fmt.Println("年龄为20")
		default:
			fmt.Println("没有匹配到")
	}

	//case中也可以对范围进行判断
	var score int = 90
	switch {
		case score &gt; 90:
			fmt.Println("成绩优秀...")
		case score &gt;= 70 &amp;&amp; score &lt;= 90:
			fmt.Println("成绩良好...")
		case score &gt;=60 &amp;&amp; score &lt;70:
			fmt.Println("成绩及格...")
		default:
			fmt.Println("不及格")
	}
}

```

(8) switch后也可以直接声明定义变量，分好结束，但不推荐使用。

<mark>(9) switch穿透fallthrough。如果在case语句块后增加fallthrough，则会继续执行下一个case，也叫switch穿透。</mark>

```
package main
import "fmt"

func main() {
	//switch穿透fallthrough
	var age int = 10

	switch age {
		case 10:
			fmt.Println("年龄为10")
			fallthrough   //默认只能穿透一层
		case 20:
			fmt.Println("年龄为20")
			fallthrough
		case 30:
			fmt.Println("年龄为30")
		default:
			fmt.Println("没有匹配到")
	}

}

```

输出结果如下图所示：

<mark>(10) Type Switch。switch语句还可以被用于type-switch来判断某个interface（接口）变量中实际指向的变量类型。后续文章会补充interface。</mark>

```
package main
import "fmt"

func main() {
	//type-switch
	var x interface{}     //定义空接口
	var y = 10.0
	x = y

	switch i := x.(type) {
		case nil:
			fmt.Printf("x的类型是:%T", i)
		case int:
			fmt.Printf("x是int型")
		case float64:
			fmt.Printf("x是float64型")   //输出结果
		case func(int) float64:
			fmt.Printf("x是func(int)型")
		case bool, string:
			fmt.Printf("x是bool或string型")
		default:
			fmt.Println("未知类型")
	}
}

```

<mark>**最后简单总结switch和if的区别：**</mark>

---


## 四.编程练习

### 1.题目

**(1) 判断一个年份是否是闰年，闰年的条件是符合以下二者之一。**

**(2) 请分别使用if和switch语句实现成绩判断，输出分数对应的等级。**

**(3) 根据淡旺季的月份和年龄，实现一个票价系统。**

**(4) 使用switch语句实现：根据用户输入月份，输出对应春夏秋冬季节。**

**(5) 求ax^2+bx+c=0方程的根。a、b、c分别为函数的参数，如果b^2-4ac&gt;0，则有两个解；如果b^2-4ac=0，则有一个解；否则无解。**

---


### 2.答案

<mark>**(1) 判断一个年份是否是闰年，闰年的条件是符合以下二者之一。**</mark>

代码如下：

```
package main
import "fmt"

func main() {
	//判断闰年
	var year int
	fmt.Println("请输入年份:")
	fmt.Scanln(&amp;year)

	if (year % 4 == 0 &amp;&amp; year % 100 !=0) || year % 400 == 0 {
		fmt.Println(year, "是闰年~")
	} else {
		fmt.Println(year, "不是闰年~")
	}
}

```

输出结果如下图所示：

<mark>**(2) 请分别使用if和switch语句实现成绩判断，输出分数对应的等级。**</mark>

代码如下：

```
package main
import "fmt"

func main() {
	var score int
	fmt.Println("请输入分数:")
	fmt.Scanln(&amp;score)

	//if判断
	if score &gt;= 90 {
		fmt.Println("分数等级为A")
	} else if score &gt;= 80 &amp;&amp; score &lt; 90 {
		fmt.Println("分数等级为B")
	} else if score &gt;= 70 &amp;&amp; score &lt; 80 {
		fmt.Println("分数等级为C")
	} else if score &gt;= 60 &amp;&amp; score &lt; 70 {
		fmt.Println("分数等级为D")
	} else {
		fmt.Println("不及格等级为E")
	}

	//switch判断
	switch {
		case score &gt; 90:
			fmt.Println("分数等级为A")
		case score &gt;= 80 &amp;&amp; score &lt; 90:
			fmt.Println("分数等级为B")
		case score &gt;=70 &amp;&amp; score &lt; 80:
			fmt.Println("分数等级为C")
		case score &gt;=60 &amp;&amp; score &lt; 70:
			fmt.Println("分数等级为D")
		default:
			fmt.Println("不及格等级为E")
	}
}

```

输出结果如下图所示：<br/> <img alt="在这里插入图片描述" height="220" src="https://img-blog.csdnimg.cn/20210216002337511.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70#pic_center" width="600"/>

<mark>**(3) 根据淡旺季的月份和年龄，实现一个票价系统。**</mark>

代码如下：

```
package main
import "fmt"

func main() {
	//出票系统
	var month byte
	var age byte
	var price float64 = 60.0

	fmt.Println("请输入游玩月份:")
	fmt.Scanln(&amp;month)
	fmt.Println("请输入游客年龄:")
	fmt.Scanln(&amp;age)

	if month &gt;= 4 &amp;&amp; month &lt;= 10 {
		//旺季
		if age &gt; 60 {
			fmt.Printf("%v月 票价 %v 年龄 %v \n", month, price / 3, age)
		} else if age &gt;= 18 {
			fmt.Printf("%v月 票价 %v 年龄 %v \n", month, price, age)
		} else {
			fmt.Printf("%v月 票价 %v 年龄 %v \n", month, price / 2, age)
		}
	} else {
		//淡季
		if age &gt;= 18 &amp;&amp; age &lt; 60 {
			fmt.Println("淡季成人票价 40")
		} else {
			fmt.Println("淡季儿童和老人票价 20")
		}
	}
}

```

输出结果如下图所示：

<mark>**(4) 使用switch语句实现：根据用户输入月份，输出对应春夏秋冬季节。**</mark>

代码如下：

```
package main
import "fmt"

func main() {
	//季节判断
	var month byte
	fmt.Println("请输入月份:")
	fmt.Scanln(&amp;month)

	switch month {
		case 3, 4, 5:
			fmt.Println("Spring")
		case 6, 7, 8:
			fmt.Println("Summer")
		case 9, 10, 11:
			fmt.Println("Autumn")
		case 12, 1, 2:
			fmt.Println("Winter")
		default:
			fmt.Println("Error Input")
	}
}

```

输出结果如下图所示：

<mark>**(5) 求ax^2+bx+c=0方程的根。a、b、c分别为函数的参数，如果b^2-4ac&gt;0，则有两个解；如果b^2-4ac=0，则有一个解；否则无解。**</mark>

注意，需要引入math.Sqrt(num)即可求平方根

```
package main
import "fmt"
import "math"

func main() {
	/* 
	 分析思路
	   1) a,b,c是给出的变量
	   2) 使用给出的数学公式和多分支语句计算
	   3) 导入math包计算平方根
	*/
	var a float64
	var b float64
	var c float64
	fmt.Println("请输入abc三个变量")
	fmt.Scanf("%f %f %f", &amp;a, &amp;b, &amp;c)
	fmt.Printf("a=%v b=%v c=%v \n", a, b, c)

	m := b * b - 4 * a * c
	if m &gt; 0 {
		x1 := (-b + math.Sqrt(m)) / (2 * a)
		x2 := (-b - math.Sqrt(m)) / (2 * a)
		fmt.Printf("x1=%v x2=%v", x1, x2)
	} else if m == 0 {
		x1 := (-b + math.Sqrt(m)) / (2 * a)
		fmt.Printf("x1=%v", x1)
	} else {
		fmt.Printf("无解")
	}
}

```

输出结果如下图所示：

---


## 五.总结

写到这里，这篇基础性Golang文章介绍完毕，希望您喜欢！祝大家新年快乐，牛气冲天，也希望自己能分享更优秀、更深入的文章。

Go基本运算和进制转换了解后，后面的文章将详细介绍Go语言的条件语句和循环语句知识，并结合案例进行普及。希望这篇基础性文章对您有帮助，写得不好的地方还请海涵。同时非常感谢参考文献中的大佬们的文章分享，尤其是韩顺平老师，深知自己很菜，得努力前行。也希望自己能深入下去，未来四年好好研究Go编程语言，做更多实际工程，写更好的文章，共勉！

源代码下载地址：

2020年在github的绿瓷砖终于贴完了第一年提交2100余次，获得1500多+stars，开源93个仓库，300个粉丝。挺开心的，希望自己能坚持在github打卡五年，督促自己不断前行。简单总结下，最满意的资源是YQ爆发时，去年2月分享的舆情分析和情感分析，用这系列有温度的代码为武汉加油；最高赞的是Python图像识别系列，也获得了第一位来自国外开发者的贡献补充；最花时间的是Wannacry逆向系列，花了我两月逆向分析，几乎成为了全网最详细的该蠕虫分析；还有AI系列、知识图谱实战、CVE复现、APT报告等等。当然也存在很多不足之处，希望来年分享更高质量的资源，也希望能将安全和AI顶会论文系列总结进来，真诚的希望它们能帮助到大家，感恩有你，一起加油~

<mark>2020年8月18新开的“娜璋AI安全之家”，主要围绕Python大数据分析、网络空间安全、人工智能、Web渗透及攻防技术进行讲解，同时分享论文的算法实现。娜璋之家会更加系统，并重构作者的所有文章，从零讲解Python和安全，写了近十年文章，真心想把自己所学所感所做分享出来，还请各位多多指教，真诚邀请您的关注！谢谢。</mark>

(By:娜璋AI之家 Eastmount 2021-02-16 星期二 夜于贵阳 [https://blog.csdn.net/Eastmount](https://blog.csdn.net/Eastmount))

---


**参考文献：**

---

