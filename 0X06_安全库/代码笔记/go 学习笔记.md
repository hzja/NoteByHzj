<a name="ArzrQ"></a>
## go学习文章
[http://c.biancheng.net/view/](http://c.biancheng.net/view/)

安装好go的环境变量后，直接在vscode安装<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629083569111-95054e9f-e704-4904-98fb-2cb3a218dc60.png#height=347&id=TQMCM&originHeight=340&originWidth=346&originalType=binary&ratio=1&size=25607&status=done&style=none&width=353)<br />重启vsc后，运行第一个hello word<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1629083611111-2abb3b04-1d90-435a-87e8-57da1cba55a0.png#height=291&id=rKrYe&originHeight=388&originWidth=512&originalType=binary&ratio=1&size=20462&status=done&style=none&width=384)


<a name="POry1"></a>
## 定义变量
go语言定义一个变量，如果不定义值，就默认为0或者false<br />var a string<br />var a string = "abc"<br />var a int

也可以这样子（简短变量声明)两个是一样的<br />**:= **(定义且赋值)                **= **(只是赋值)<br />c := 1  →  var c int = 1<br />d :="abc"  →  var d string ="abc"
<a name="zBRc5"></a>
### 匿名变量

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630854975984-e0dfbd2e-54ec-466a-a6ed-cd5b7923f627.png#clientId=u0a67ffef-dc08-4&from=paste&height=328&id=uf393015e&originHeight=437&originWidth=458&originalType=binary&ratio=1&size=26670&status=done&style=none&taskId=u9d56e50e-6acf-4876-bfbf-fe15742633e&width=344)
<a name="F0Ufc"></a>
## 定义数组
var n [10]int<br />balance := [...]float32{1000.0, 2.0, 3.4, 7.0, 50.0}
<a name="FZo4y"></a>
## 循环
```go
	sum :=0
	for i :=0;i < 10;i++ {
		sum +=i
		fmt.Println(sum)
	}
```

```go
	sum1 :=0
	for{
		sum1++
		if sum1 > 100{
			
			fmt.Println(sum1)
			break
		}
    }
```
break的意思：结束内层循环，外层继续。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630940257640-847eb642-9962-4e65-adc9-31c3dacfb5dc.png#clientId=u49421ba8-3e11-4&from=paste&height=435&id=u9bae686b&originHeight=580&originWidth=479&originalType=binary&ratio=1&size=122630&status=done&style=none&taskId=u711ea149-eac9-4128-9ac1-b63d1617a8b&width=359)

goto：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630940964205-fa613c21-5ba2-4d86-8b66-0ef7cd93a241.png#clientId=u49421ba8-3e11-4&from=paste&height=423&id=u7feea30a&originHeight=563&originWidth=477&originalType=binary&ratio=1&size=30005&status=done&style=none&taskId=u707a0e17-b85f-4328-9453-1530bd31483&width=358)

<a name="BfCue"></a>
### _和for range循环
```go
package main
import "fmt"

func main() {
	a := [...]int{1,2,3,4}
	fmt.Println(a)                  // [0 1 0 5]
	//fmt.Printf("type of a:%T\n", a) //type of a:[4]int

	var b = [...]string{"北京","上海","中山"}
	fmt.Println(b)
	for index,c := range b{     //其实这里就是把数组的序号输出一下，可以用_去掉输出的序号
		fmt.Println(index,c)
	}

}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631110679132-f9008473-2995-48a8-8e65-f5d239dabe3a.png#clientId=u16d36f9b-53e7-4&from=paste&height=572&id=u999ad1ce&originHeight=762&originWidth=622&originalType=binary&ratio=1&size=39507&status=done&style=none&taskId=ua710d930-b9b9-4a2e-9346-85c2d8243a8&width=467)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631110867774-a9e8a677-adb0-4adc-8216-09a3418b2606.png#clientId=u16d36f9b-53e7-4&from=paste&height=588&id=u7c73131a&originHeight=784&originWidth=623&originalType=binary&ratio=1&size=44372&status=done&style=none&taskId=u23abee63-e789-44ec-9d0e-6178d9decee&width=467)

结束当前循环，进入下一个循环
```go
package main
import (
	"fmt"

)

func main()  {
	sshIP := []string{"127.0.0.1","192.168.1.1"}
	sshUser := []string{"root","admin","test","user","ubuntu"}
	sshPassword := []string{"123","123456", "admin", "admin123", "root", "", "pass123", "pass@123", "password", "123123", "654321", "111111", "123", "1", "admin@123", "Admin@123", "admin123!@#","P@ssw0rd!", "P@ssw0rd", "Passw0rd", "qwe123", "12345678", "test", "test123", "123qwe!@#", "123456789", "123321", "666666", "a123456.", "123456~a", "123456!a", "000000", "1234567890", "8888888", "!QAZ2wsx", "1qaz2wsx", "abc123", "abc123456", "1qaz@WSX", "a11111", "a12345", "Aa1234", "Aa1234.", "Aa12345", "a123456", "a123123", "Aa123123", "Aa123456", "Aa12345.", "sysadmin", "system", "1qaz!QAZ", "2wsx@WSX", "qwe123!@#", "Aa123456!", "A123456s!", "sa123456", "1q2w3e"}
	//fmt.Println(add)
	for _,ip := range sshIP{
		Loop: //如果达到目标，就跳回第一个循环
		for _,User := range sshUser{
			for _,Pass := range sshPassword{
				if ip == "127.0.0.1" && User == "test" && Pass == "123456"{
					fmt.Println(ip,User,Pass,"-------------------success")
					break Loop
				}
				if ip == "192.168.1.1" && User == "root" && Pass == "123456"{
					fmt.Println(ip,User,Pass,"-------------------success")
					break Loop
				}else{
					fmt.Println(ip,User,Pass)
				}

				}
		}
	}
}

```
<a name="rRYuK"></a>
## 切片
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631111744923-c3c806f6-ba30-4df7-a61b-08459448fac7.png#clientId=u16d36f9b-53e7-4&from=paste&height=523&id=uda8387ac&originHeight=697&originWidth=721&originalType=binary&ratio=1&size=36105&status=done&style=none&taskId=ub9ea329a-06d0-4a80-a3e3-2f965cf78db&width=541)<br />动态切片
```go
	var s []int
	s = append(s, 1)        // [1]
	s = append(s, 2, 3, 4)  // [1 2 3 4]
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631113798674-2ec2f856-6ee6-4a46-939b-fa74e724a2e4.png#clientId=u16d36f9b-53e7-4&from=paste&height=566&id=u7bcd4f59&originHeight=754&originWidth=1036&originalType=binary&ratio=1&size=73311&status=done&style=none&taskId=ubf21691b-9e49-46e3-9c94-9b63e4292e6&width=777)
```go
func main()  {
    var a []int
    for i:=0 ;i < 10;i ++{
        a = append(a,i)
    }
    fmt.Println(a)
    
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632576764026-100e9715-ee87-49c5-b53d-2d36a97f5b09.png#clientId=u50613c84-09e5-4&from=paste&id=u16c4fe0f&originHeight=78&originWidth=411&originalType=binary&ratio=1&size=11022&status=done&style=none&taskId=u18eb6747-c843-4713-b94e-38eb0efe427)
<a name="rhluH"></a>
## 函数调用
```go
func max(x int) int{
	return x;
}

func main(){
    fmt.Println(max(e))
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630597743711-3936133d-8be6-43be-a582-f2f489c4d388.png#clientId=u8aba6734-6407-4&from=paste&height=516&id=ufae52c69&originHeight=688&originWidth=531&originalType=binary&ratio=1&size=39469&status=done&style=none&taskId=uca9f01dc-3d36-4ca8-b611-21c07515cc1&width=398)<br />这个博主讲的不错<br />[https://blog.csdn.net/weixin_41640994/article/details/109491591](https://blog.csdn.net/weixin_41640994/article/details/109491591)
<a name="O45SY"></a>
## 占位符
[https://studygolang.com/articles/2644](https://studygolang.com/articles/2644)
<a name="nyqlB"></a>
## 输入输出
go的输入赋值和python不一样，要注意&<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630592993913-265c0cb8-40db-48eb-8670-c7e913a4cbfc.png#clientId=u8aba6734-6407-4&from=paste&height=308&id=u4ba5050d&originHeight=410&originWidth=465&originalType=binary&ratio=1&size=22865&status=done&style=none&taskId=u489d4fbc-a0e0-4253-a28e-d7134125c18&width=349)
<a name="cRAij"></a>
## if ;
之前总是看不懂这种语法，其实就是变换一下，把if写成一段了。
```java
if err = msg.WriteMsg(conn, loginMsg); err != nil {return}
						↓
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648382597144-6936bc4c-c96c-47f0-acf1-e1cf8ba8f870.png#clientId=u966f0504-26eb-4&from=paste&id=u6afd14a4&originHeight=117&originWidth=544&originalType=binary&ratio=1&size=7170&status=done&style=none&taskId=ub4953b65-2329-47e6-a48e-46cc88a9c1e)
<a name="CBuyG"></a>
## 关于...的用法
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631116564846-42f0590d-9628-421b-8afd-1f36d9560998.png#clientId=u16d36f9b-53e7-4&from=paste&height=374&id=u16183e1d&originHeight=498&originWidth=714&originalType=binary&ratio=1&size=32095&status=done&style=none&taskId=u6bef703f-1285-403e-ae0c-fb9112a22d8&width=536)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631116751116-fb8f34cc-32f4-4e93-8ddd-63d72b5b9aa0.png#clientId=u16d36f9b-53e7-4&from=paste&height=412&id=u4d5bd757&originHeight=549&originWidth=1027&originalType=binary&ratio=1&size=41558&status=done&style=none&taskId=u2363e694-6d3f-4571-a58d-bd20575a307&width=770)

<a name="lLb8k"></a>
## go 接口
```go
package main

import (
	"fmt"
)

// Sayer 接口
type Sayer interface {
	say()
}

type dog struct{}

type cat struct{}

// dog实现了Sayer接口
func (d dog) say() {
	fmt.Println("汪汪汪")
}

// cat实现了Sayer接口
func (c cat) say() {
	fmt.Println("喵喵喵")
}

func main() {
	var x Sayer // 声明一个Sayer类型的变量x
	a := cat{}  // 实例化一个cat
	b := dog{}  // 实例化一个dog
	x = a       // 可以把cat实例直接赋值给x
	x.say()     // 喵喵喵
	x = b       // 可以把dog实例直接赋值给x
	x.say()     // 汪汪汪
}

```

```go
package main

import (
	"fmt"
)

// Sayer 接口
type Sayer interface {
	say()
}
type Mover interface {
	move()
}

type dog struct{}

type cat struct{}

// dog实现了Sayer接口
func (d dog) say() {
	fmt.Println("汪汪汪")
}

// cat实现了Sayer接口
func (c cat) say() {
	fmt.Println("喵喵喵")
}

func (d *dog) move() { //指针接收值
	fmt.Println("狗会动")
}
func main() {

	a := dog{}
	a.say()

	b := cat{}
	b.say()

	c := &dog{}
	c.move()
}

```
```go
package main

import (
	"fmt"
)

type Shape interface {
	Area() float64
	Perimeter() float64
}
type Rect struct {
	width  float64
	height float64
}

func (r Rect) Area() float64 {
	return r.width * r.height
}

func (r Rect) Perimeter() float64 {
	return 2 * (r.height + r.width)
}

func main() {
	var s Shape
	s = Rect{5.0, 3.0} //创建一个Shape结构体并使其实现Shape接口

	r := Rect{5.0, 3.0} // r == s
	fmt.Println(s.Area())
	fmt.Println(r.Perimeter())
}

```
<a name="q37sR"></a>
## go结构体
```go
package main
import "fmt"

type person struct {
	name string
	city string
	age int
}

func main(){
	var p1 person   //  p1 :=&person{} 等于 var p1 person
	p1.name = "aufeng"
	p1.city = "广州"
	p1.age = 18
	
	fmt.Printf("%v,%#v",p1,p1)
}
```
利用场景
```go
//定义结构体，并且存入数值
var tasks []Task
for _, user := range users {
   for _, password := range passwords {
      for _, ip := range aliveIps {
         tasks = append(tasks, Task{ip, user, password})
      }
   }
}

type Task struct {
   ip       string
   user     string
   password string
}

//结构体+高并发的的场景，可以替换多层循环

func runTask(tasks []Task) {
   var wg sync.WaitGroup
   for _, task := range tasks {
      wg.Add(1)
      go func(task Task) {
         defer wg.Done()
         success, _ := sshLogin(task.ip, task.user, task.password)
         if success {
            log.Printf("破解%v成功，用户名是%v,密码是%v\n", task.ip, task.user, task.password)
         }
      }(task)
   }
   wg.Wait()
}

```

```go
package main

import "fmt"

type person struct {
	name string
	city string
	age  int8
}

func main() {
	a := newPerson("tony", "zhongsan", 8)
	fmt.Printf("%v", a.age)
	//fmt.Printf("%v", *a)

}

func newPerson(name, city string, age int8) *person {
	return &person{
		name: name,
		city: city,
		age:  age,
	}
}

```

结构体+函数和方法
```go
//Person 结构体
type Person struct {
	name string
	age  int8
}

//NewPerson 构造函数
func NewPerson(name string, age int8) *Person { //函数
	return &Person{
		name: name,
		age:  age,
	}
}

//Dream Person做梦的方法
func (p Person) Dream() {  //方法也叫接口
	fmt.Printf("%s的梦想是学好Go语言！\n", p.name)
}

func main() {
	p1 := NewPerson("小王子", 25) //函数
	p1.Dream()   //方法 = 接口
}
```
<a name="KnT53"></a>
## go map
```go
func main() {
	scoreMap := make(map[string]int, 8)
	scoreMap["张三"] = 90
	scoreMap["小明"] = 100
	fmt.Println(scoreMap)
	fmt.Println(scoreMap["小明"])
	fmt.Printf("type of a:%T\n", scoreMap)
}


result：

map[小明:100 张三:90]
100
type of a:map[string]int
```
```go
package main

import "fmt"

func main(){
    //创建集合
    var map1 map[string]string
    map1 = make(map[string]string)

    map1["name"] = "小明"
    map1["account"] = "admin"
    map1["password"] = "123456"

    //使用键输出map值
    for i := range map1{
        fmt.Println(i,"是",map1[i])
    }

    //查看元素在集合中是否存在
    capital,ok := map1 ["status"]
    if(ok){
        fmt.Println("status 是",capital)
    }else{
        fmt.Println("status 不存在")
    }
}
```
<a name="zMJRd"></a>
## 异常处理
[https://blog.csdn.net/super_lixiang/article/details/82689474](https://blog.csdn.net/super_lixiang/article/details/82689474)
```go
package main
 
import (
    "errors"
    "fmt"
)
 
func main() {
    //获得调用的结果
    ret, err := getCircleAreaII(-5)
 
    //如果返回的错误不为空，则说明程序出现了异常
    if err!=nil{
        fmt.Println(err)
    }else {
        fmt.Println("ret=",ret)
    }
}
 
/*
如果发生异常时，以返回错误的方式代替恐慌
正确结果和错误，必有一个为空
这样相对温和，也不会造成程序崩溃
*/
func getCircleAreaII(radius float32)(ret float32,err error) {
    if radius < 0{
        err = errors.New("傻鸟，半径不能为负")
        return
    }
    ret = 3.14 * radius * radius
    return
}
```
<a name="iCImY"></a>
## go转义符
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630858058329-55b4446f-b446-4e73-8bbb-7784f036f541.png#clientId=u782b78b7-a600-4&from=paste&height=297&id=u4b4cf913&originHeight=396&originWidth=688&originalType=binary&ratio=1&size=25113&status=done&style=none&taskId=ufcff82f1-9a98-4840-b1cf-feed3d3d391&width=516)
<a name="NgImw"></a>
## 代码内容格式化
碰到其他人写的代码，可能代码内容格式是乱的，可以使用下面的方法直接自动化格式好。<br />go fmt xxx.go 

<a name="H7R8g"></a>
## go func
```go
func main() {
    runtime.GOMAXPROCS(1) //设置协程调度只有一个P
    wg := sync.WaitGroup{}
    wg.Add(20)
    for i := 0; i < 10; i++ {
        go func() {
            fmt.Println("A: ", i)
            wg.Done()
        }()
    }
    for i := 0; i < 10; i++ {
        go func(i int) {
            fmt.Println("B: ", i)
            wg.Done()
        }(i)
    }
    wg.Wait()
}

//https://studygolang.com/articles/21588?fr=sidebar
```
<a name="oX70I"></a>
## 并发代码
```go
package main
import (
	"fmt"
	"sync"
	"time"
)

var wg sync.WaitGroup // 申明一个信号量的全局变量

func hello(i int) {
	defer wg.Done() // goroutine结束就登记-1
	fmt.Println(i)
}

func main() {
	var begin =time.Now()
	for i := 1; i < 9000; i++ {
		wg.Add(1) // 启动一个goroutine就登记+1
		go hello(i)
	}
	wg.Wait() // 等待所有登记的goroutine都结束再执行下一步
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime)
}
```

WaitGroup对象不是一个引用类型，在通过函数传值的时候需要使用地址：<br />*sync.WaitGroup
```go
func main() {
    wg := sync.WaitGroup{}
    wg.Add(100)
    for i := 0; i < 100; i++ {
        go f(i, &wg)
    }
    wg.Wait()
}

// 一定要通过指针传值，不然进程会进入死锁状态
func f(i int, wg *sync.WaitGroup) { //这样子就不用定义全局变量
    fmt.Println(i)
    wg.Done()
}
```

控制线程池<br />[https://zhuanlan.zhihu.com/p/150694706](https://zhuanlan.zhihu.com/p/150694706)
<a name="Yby7e"></a>
## 指针
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631350892485-5a22bb80-d115-444c-9b17-40d285cb8917.png#clientId=uee424b2e-dc47-4&from=paste&height=299&id=uf2de4ded&originHeight=398&originWidth=454&originalType=binary&ratio=1&size=21062&status=done&style=none&taskId=u03ece6c8-e5b8-4b4f-800a-a04984335ae&width=341)<br />取地址操作符&和取值操作符*是一对互补操作符，&取出地址，*根据地址取出地址指向的值。  

声明了指针变量后，还需要使用new来初始化
```go
func main() {
	var a *int
	a = new(int)
	*a = 10
	fmt.Println(*a)
}
```

指针调用传值
```go
func modify1(x int) {
	x = 100
}

func modify2(x *int) {
	*x = 100
}

func main() {
	a := 10
	modify1(a)
	fmt.Println(a) // 10
	modify2(&a)
	fmt.Println(a) // 100
}
```
<a name="MjmSh"></a>
## 生成exe文件
go build -o bf.exe .\bingfa.go

<a name="lSYyo"></a>
## 类型强转换+切割
```go
package main
 
import (
	"fmt"
	"strings"
	"strconv"
)
 
func main() {
	//str := "80-90"
	var str string
	fmt.Scanf("%s",&str)
    //指定分隔符
    countSplit := strings.Split(str, "-")
    //fmt.Println(countSplit[0])
	//fmt.Println(reflect.TypeOf(countSplit[1])) //查看类型
	
	a,_:=strconv.Atoi(countSplit[0])//类型强转换
	b,_:=strconv.Atoi(countSplit[1]) 
	for i := a; i < b; i++ {
		c := i
		fmt.Println(c)
	}

}
```
<a name="mbar3"></a>
## 自定义包
这个自定义包，必须要再go的 $GOPATH/src  下面建立，例如我在src的目录下创建了pac<br />D:\go\src\pac<br />pac的内容：
```go
package pac

import "fmt"

func Printstr(x string) {
    fmt.Println(x + " is pac")
}

```
然后可以引用自定义的包，引用方式如下,记得，引用的**函数名开头要大写**
```go
package main

import "pac"

func main() {
    pac.Printstr("this")
}
```
这个调用自定义包的话，要注意全局变量的问题，之前写端口扫描的时候，总是会卡住，原因调用的时候，这个全局变量没被函数使用，需要去掉，然后像箭头那样调用才行<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1636560318377-7c080817-da33-4ec4-8496-35c2017751de.png#clientId=uacf72388-41d3-4&from=paste&height=351&id=ucbb35f14&originHeight=468&originWidth=736&originalType=binary&ratio=1&size=38699&status=done&style=none&taskId=u2a9fe714-faf8-4cf3-b8f3-889aed762f3&width=552)
<a name="kKOXa"></a>
## channel 通道

```go
var ch1 chan int   // 声明一个传递整型的通道
ch :=  make(chan 元素类型, [缓冲大小])  //定义一个缓冲通道
ch <- 10 // 把10发送到ch中
x := <- ch // 从ch中接收值并赋值给变量x
<-ch       // 从ch中接收值，忽略结果
close(ch) 关闭通道

单向通道的定义：
chan<- int是一个只能发送的通道，可以发送但是不能接收； 存入
<-chan int是一个只能接收的通道，可以接收但是不能发送。 取出
```
 无缓冲的通道只有在**有接收值**的时候才能**发送值 **<br />** **<br />**定义的通道是否缓冲，需要看缓冲大小的值有没有定义。**<br /> make(chan 元素类型, [缓冲大小])

```go
package main
import (
	"fmt"
)

func counter(save chan<- int) {
	for i := 0; i <= 1000; i++ {
		save <- i
	}
	close(save)
}

func squarer(ch2 chan<- int, ch1 <-chan int) {
	for i := range ch1 {
		ch2 <- i
	}
	close(ch2)
}
func printer(ch2 <-chan int) {
	for i := range ch2 {
		fmt.Println(i)
	}
}

func main() {
	ch1 := make(chan int,10)
	ch2 := make(chan int)
	go counter(ch1)
	go squarer(ch2, ch1)
	printer(ch2)
}
```

```go
package main
import (
	"fmt"
)

func main()  {
	b :=make(chan int, 1)
	b <- 10
	fmt.Println("ok")
	x := <-b
	fmt.Println("ok--",x)

}
```
```go
package main
import "fmt"

func main() {
	ch1 := make(chan int)
	//ch2 := make(chan int)
	// 开启goroutine将0~100的数发送到ch1中
	go func() {
		for i := 0; i < 100; i++ {
			ch1 <- i
		}
		close(ch1)
	}()

	for i := range ch1 { // 通道关闭后会退出for range循环  这里相当于把ch1通道的值取出来
		fmt.Println(i)
	}
	}
```
```go
通 jobs存进去之后，会被results取出来，然后jobs通道会对应的减少results通道所取的值

package main
import (
		"fmt"
		//"time"
		"sync"
)

var wg sync.WaitGroup

func worker(id int, jobs <-chan int, results chan<- int) {
	defer wg.Done()
	for j := range jobs {
		//fmt.Printf("worker:%d start job:%d\n", id, j)
		//time.Sleep(time.Second)
		results <- j * 2
	}
}
func main() {
	jobs := make(chan int, 100)
	results := make(chan int, 100)
	// 5个任务
	for j := 1; j <= 10; j++ {
		jobs <- j
	}
	close(jobs)

	// 开启3个goroutine
	for w := 1; w <= 3; w++ {
		wg.Add(1)
		go worker(w, jobs, results)
	}
	wg.Wait()
	close(results)
	// 输出结果	
	for x := range jobs{  //jobs的值被results取光了，所以输出是无
		fmt.Println(x)
	}
	//fmt.Println(jobs)
	
}

```

<a name="zYUhB"></a>
## 下载第三方依赖包的代理

go env -w GOPROXY=https://goproxy.cn,direct

<a name="qTh2E"></a>
## 关于引用第三方包的报错


![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632384820553-27a1a822-42b1-466a-babb-cbdb9ce19886.png#clientId=u20e2b2f0-c1c4-4&from=paste&height=87&id=u6bf231cd&originHeight=116&originWidth=960&originalType=binary&ratio=1&size=36087&status=done&style=none&taskId=u3a7dd3e7-13dc-46bc-b803-89dd80479f7&width=720)

每次go get都会下载到  C:\Users\19307\go\pkg\mod\github.com    需要把里边的包放到D:\go\src\github.com 然后还需要改一下errors的命名才能正常使用<br />go get -u <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632384799917-537e598a-7b53-4844-9178-27ca8b749839.png#clientId=u20e2b2f0-c1c4-4&from=paste&height=187&id=u66987348&originHeight=374&originWidth=1300&originalType=binary&ratio=1&size=61644&status=done&style=none&taskId=uae1e23a4-f8b4-4de3-a89b-16c72f0592e&width=650)<br />在这个路径里边还没办法直接运行main.go文件，需要把main.go文件移出来其他目录才行

<a name="NYJJn"></a>
## nil是什么
 当出现不等于nil的时候，说明出现某些错误了  ， nil的意思是无，或者是零值  

<a name="FdzRf"></a>
## 未知类型转为string
后面加个.String()<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632540193755-61eb0367-9340-45ec-b6c5-a31c1018659a.png#clientId=uc7042e01-a562-4&from=paste&height=107&id=u89ef7757&originHeight=143&originWidth=548&originalType=binary&ratio=1&size=10065&status=done&style=none&taskId=u6dd1c2b6-6ac7-43c0-94ad-fbca1d0decb&width=411)

<a name="dEv0M"></a>
## flag包

```go
package main
import(
	"flag"
	"fmt"
)


func main() { 
	// 第一个参数是命令行key，第二个参数是默认是，第三个参数是 .exe -h 提示 
	//var user = flag.String("user", "root", "用户名") 
	//var port = flag.Int("port", 3306, "端口") 
	var ip = flag.String("ip", " ", "需要探测的ip范围") 
 
	//必须使用flag.Parse()解析一下命令行参数 
	flag.Parse() 
	//flag.Type返回的是一个指针，必须通过 *变量取值
	fmt.Println(*ip) 

	//fmt.Println(*user,*port,*ip) 
} 
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632637385596-5a1df319-0ed8-48b2-bf20-d6bbf5cfea09.png#clientId=u05c42bcc-1106-4&from=paste&height=85&id=u756252c9&originHeight=114&originWidth=731&originalType=binary&ratio=1&size=22542&status=done&style=none&taskId=u7b93c0a7-eb45-464d-a5e6-9bd6fec70c7&width=548)<br />但是需要指针进行传值，所以可以用Var()的方法去解决问题
```go
package main
import(
	"flag"
	"fmt"
)

var ip string 
func main() { 
	// 第一个参数是命令行key，第二个参数是默认是，第三个参数是 .exe -h 提示 
	flag.StringVar(&ip,"ip", " ", "需要探测的ip范围") 
 
	//必须使用flag.Parse()解析一下命令行参数 
	flag.Parse()
	fmt.Println(ip) 
} 
```
<a name="Vpk98"></a>
## 正则匹配
```go
    package main
    import (
        "fmt"
        "regexp"
    )
    func main() {
        // 原生字符串
        buf := `
        
    <!DOCTYPE html>
    <html lang="zh-CN">
    <head>
        <title>C语言中文网 | Go语言入门教程</title>
    </head>
    <body>
        <div>Go语言简介</div>
        <div>Go语言基本语法
        Go语言变量的声明
        Go语言教程简明版
        </div>
        <div>Go语言容器</div>
        <div>Go语言函数</div>
    </body>
    </html>
        `
        //解释正则表达式
        reg := regexp.MustCompile(`<div>(?s:(.*?))</div>`)
        if reg == nil {
            fmt.Println("MustCompile err")
            return
        }
        //提取关键信息
        result := reg.FindAllStringSubmatch(buf, -1)
        //过滤<></>
        for _, text := range result {
            fmt.Println("text[1] = ", text[1])
        }
    }
```
<a name="naI0d"></a>
## go交叉编译

[https://studygolang.com/articles/14376](https://studygolang.com/articles/14376)

SET CGO_ENABLED=0<br />SET GOOS=linux<br />SET GOARCH=amd64<br />go build xxx.go

**GOOS**：目标平台的操作系统（darwin、freebsd、linux、windows）<br />**GOARCH**：目标平台的体系架构（386、amd64、arm）<br />交叉编译不支持 CGO 所以要禁用它


<a name="SV103"></a>
## go socket编程
```java
好文章
https://blog.csdn.net/tianlongtc/article/details/80163661#:~:text=Go%E6%98%AF%E8%87%AA%E5%B8%A6runtime%E7%9A%84%E8%B7%A8%E5%B9%B3%E5%8F%B0%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80%EF%BC%8CGo%E4%B8%AD%E6%9A%B4%E9%9C%B2%E7%BB%99%E8%AF%AD%E8%A8%80%E4%BD%BF%E7%94%A8%E8%80%85%E7%9A%84tcp%20socket%20api%E6%98%AF%E5%BB%BA%E7%AB%8BOS%E5%8E%9F%E7%94%9Ftcp%20socket%E6%8E%A5%E5%8F%A3%E4%B9%8B%E4%B8%8A%E7%9A%84%E3%80%82,%E7%94%B1%E4%BA%8EGo%20runtime%E8%B0%83%E5%BA%A6%E7%9A%84%E9%9C%80%E8%A6%81%EF%BC%8Cgolang%20tcp%20socket%E6%8E%A5%E5%8F%A3%E5%9C%A8%E8%A1%8C%E4%B8%BA%E7%89%B9%E7%82%B9%E4%B8%8E%E5%BC%82%E5%B8%B8%E5%A4%84%E7%90%86%E6%96%B9%E9%9D%A2%E4%B8%8EOS%E5%8E%9F%E7%94%9F%E6%8E%A5%E5%8F%A3%E6%9C%89%E7%9D%80%E4%B8%80%E4%BA%9B%E5%B7%AE%E5%88%AB%E3%80%82
```

server服务端
```java
//go-tcpsock/server.go
func handleConn(c net.Conn) {
    defer c.Close()
    for {
        // read from the connection
        // ... ...
        // write to the connection
        //... ...
    }
}
 
func main() {
    l, err := net.Listen("tcp", ":8888")
    if err != nil {
        fmt.Println("listen error:", err)
        return
    }
 
    for {
        c, err := l.Accept()
        if err != nil {
            fmt.Println("accept error:", err)
            break
        }
        // start a new goroutine to handle
        // the new connection.
        go handleConn(c)
    }
}
```

客户端
```java
conn, err := net.Dial("tcp", "ip:port")
if err != nil {
    //handle error
}
// read or write on conn
```
