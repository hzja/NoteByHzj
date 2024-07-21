<a name="jU1mT"></a>
## 单ip自定义端口扫描
思路：<br />先把所有端口都存进一个数组里边<br />↓<br />数组启动协程进行单ip，端口扫描
```go
package main
 
import (
	"fmt"
	"strings"
	"sync"
	"strconv"
	"sort"
	"runtime"
	"time"
	"net"
)
var wg sync.WaitGroup
var str,ip string


func main() {
	fmt.Println("请输入要扫描的ip和端口,中间带空格,例如:192.168.1.1 80,8080,8000-9000: ")
	fmt.Scanf("%s %s",&ip,&str)
	fmt.Scanf("\r")
	parsePort(str)
	a :=parsePort(str)
	run(a)
	//fmt.Println(a)
	//fmt.Println(reflect.TypeOf(a))
}

func parsePort(ports string) []int{
	var scanPorts []int
    countSplit := strings.Split(ports, ",")
	for _,port := range countSplit{
		port = strings.Trim(port, " ")
		upper := port
		if strings.Contains(port,"-"){
			ranges := strings.Split(port,"-")
			if len(ranges) < 2 {
				continue
			}
			sort.Strings(ranges)
			port = ranges[0]
			upper = ranges[1]
		}
		start, _ := strconv.Atoi(port)
		end, _ := strconv.Atoi(upper)  //字符型转换整型
		for i := start; i <= end; i++ {
			scanPorts = append(scanPorts, i)
		}
	}
	//fmt.Println(scanPorts)
	return scanPorts
}

func run(a []int){
	//fmt.Println(a)
	runtime.GOMAXPROCS(1)
	var begin =time.Now()	
	for i := 0; i < len(a); i++ {
		wg.Add(1) // 启动一个goroutine就登记+1
		go hello(ip,a[i])
	}
	wg.Wait() // 等待所有登记的goroutine都结束
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime)
}


func hello(ip string,i int) {
	defer wg.Done() // goroutine结束就登记-1
	//var ip string = "127.0.0.1"
	//fmt.Scanf("请输入要扫描的ip: %s",&ip)
	//ip :="127.0.0.1"

	var address = fmt.Sprintf("%s:%d", ip, i)
	conn, err := net.Dial("tcp", address)
	if err != nil {
		//fmt.Println(address, "是关闭的")
		return
	}
	conn.Close()
	fmt.Println(address," 打开")
	//fmt.Println("Hello Goroutine!", i)

}


```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631786437304-02f668c5-03ac-4261-81cf-54e09205d5a2.png#clientId=ub951001e-25be-4&from=paste&height=185&id=u98ac0545&originHeight=247&originWidth=702&originalType=binary&ratio=1&size=37302&status=done&style=none&taskId=ude50bdfe-4b20-4618-94ea-8f14929eb74&width=527)
<a name="xP5Wi"></a>
### 缺陷：
这样子启动6万多个线程，cpu会爆炸，要控制cpu和过多的线程，需要写一个channel线程池控制<br />实现出来的代码

<a name="PUVfJ"></a>
### 改进
线程池控制cpu，实现队列的效果代码：

```go
package main

//线程池方式
import (
	"fmt"
	"gohive"
	"net"
	"sync"
	"time"
	"strings"
	"strconv"
	"sort"
)

//wg
var wg sync.WaitGroup

//地址管道,100容量
var addressChan = make(chan string,65535) //注意这个channel定义的长度，会导致deadlock!

var str,ip string

func main() {
	//ip
	//var ip = "10.82.200.91"
	//线程池大小
	fmt.Println("请输入要扫描的ip和端口,中间带空格,例如:192.168.1.1 80,8080,8000-9000: ")
	fmt.Scanf("%s %s",&ip,&str)
	fmt.Scanf("\r")
	a := parsePort(str)      //分割端口
	//fmt.Println(a)
	run(a)       //分割好的端口进行处理，传入队列中
}


//worker
func worker() {
	//函数结束释放连接
	defer wg.Done()
	for {
		address, ok := <-addressChan
		if !ok {
			break
		}
		//fmt.Println("address:", address)
		conn, err := net.Dial("tcp", address)
		//conn, err := net.DialTimeout("tcp", address, 10)
		if err != nil {
			//fmt.Println("close:", address, err)
			continue
		}
		conn.Close()
		fmt.Println("open:", address)
	}
}

func parsePort(ports string) []int{
	var scanPorts []int
    countSplit := strings.Split(ports, ",")
	for _,port := range countSplit{
		port = strings.Trim(port, " ")
		upper := port
		if strings.Contains(port,"-"){
			ranges := strings.Split(port,"-")
			if len(ranges) < 2 {
				continue
			}
			sort.Strings(ranges)
			port = ranges[0]
			upper = ranges[1]
		}
		start, _ := strconv.Atoi(port)
		end, _ := strconv.Atoi(upper)
		for i := start; i <= end; i++ {
			scanPorts = append(scanPorts, i)
		}
	}
	//fmt.Println(scanPorts)
	return scanPorts
}

func run(a []int){
	//fmt.Println(a)

	//用于生成ip:port,并且存放到地址管道种
	for i := 0; i < len(a); i++ {
		var address = fmt.Sprintf("%s:%d", ip, a[i])
		addressChan <- address
	}
	close(addressChan)
	//var elapseTime = time.Now().Sub(begin)
	fmt.Println("通道获取长度:", len(addressChan))
	run_pool()   //启动线程池
}

func run_pool()  {
	var begin = time.Now()
	var pool_size = 10000
	var pool = gohive.NewFixedSizePool(pool_size)

	//启动pool_size工人,处理addressChan种的每个地址
	for work := 0; work < pool_size; work++ {
		wg.Add(1)
		pool.Submit(worker)
	}
	//等待结束
	wg.Wait()
	//计算时间
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime)
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632302408081-78dd7b4b-9f58-4001-b115-a9ea489b60e6.png#clientId=u421ab73e-b03c-4&from=paste&height=354&id=u434d47bc&originHeight=472&originWidth=642&originalType=binary&ratio=1&size=35616&status=done&style=none&taskId=u3a89da3d-27cc-4a9b-bae9-646c9613bcf&width=482)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632312495509-723b346f-a83d-4704-893d-da1e51c478ad.png#clientId=u1a37fca9-4972-4&from=paste&height=224&id=udc89009e&originHeight=298&originWidth=588&originalType=binary&ratio=1&size=30785&status=done&style=none&taskId=u0b8213a1-fb34-4580-a9c4-17379860b7b&width=441)
<a name="OPQy3"></a>
## 多ip自定义端口扫描
 单ip扫描端口比较简单 、 如果多ip、多端口进行扫描 、 难度就上来了， (完成<br />1、需要组合，要怎么把ip和端口组合加入协程比较合理，也是一个难度   、 (完成<br />2、还要写chain  就是控制线程池  控制cpu、 (完成<br />3、消费队列，如何一进一存一消费（先进先出） （完成

先判断ip存活→进行自定义端口扫描

拉跨之作(后续需要修改和完善：
```go
package main

import (
	"fmt"
	//"gohive"
	"net"
	"sync"
	"time"
	"strings"
	"strconv"
	"sort"
	//"reflect"
)

var wg sync.WaitGroup

func main()  {
	var begin = time.Now()

	//ipalive :=[...]string{"10.82.200.91","10.82.200.86","10.82.200.1","10.82.200.98","10.82.200.68","10.82.200.81","10.82.200.75","10.82.200.79","10.82.200.54","10.82.200.11","10.82.200.48","10.82.200.20","10.82.200.61","10.82.200.90","10.82.200.15","10.82.200.40","10.82.200.8","10.82.200.18","10.82.200.103","10.82.200.26","10.82.200.64","10.82.200.89","10.82.200.87","10.82.200.27","10.82.200.100","10.82.200.47","10.82.200.43","10.82.200.2","10.82.200.109","10.82.200.49","10.82.200.51","10.82.200.131"}
	//ipalive :=[...]string{"192.168.1.103","192.168.1.101","192.168.1.1","127.0.0.1"}
	ipalive :=[...]string{"127.0.0.1"}

	var scanport = make(chan string,10000)
	//results := make(chan string, 100)
	//fmt.Println(reflect.TypeOf(ipalive))
	//fmt.Scanf("%s",&str)
	a := parsePort("1-65535")      //分割端口
	//fmt.Println(a)
	//results = squarer(scanport,results)
	go func (){
		//fmt.Println(a)
		//用于生成ip:port,并且存放到地址管道种
		for _,ip :=range ipalive{
			for i := 0; i < len(a); i++ {
				var address = fmt.Sprintf("%s:%d", ip, a[i])
				scanport <- address
				//fmt.Println("通道获取长度:", len(scanport))
			}
		}
		close(scanport)
		//var elapseTime = time.Now().Sub(begin)
	}()
	//fmt.Println("通道获取长度:", len(scanport))
	/*
	var pool_size = 10000
	var pool = gohive.NewFixedSizePool(pool_size)
	*/
	//启动pool_size工人,处理scanport种的每个地址
	for i := 0; i < 5000; i++ {
		wg.Add(1)
		//go worker(ports, results)
		go worker(scanport)

	}
	/*
	for work := 0; work < pool_size; work++ {
		wg.Add(1)
		pool.Submit(worker)
	}*/
	//等待结束
	wg.Wait()
	//计算时间
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime)
}

func parsePort(ports string) []int{
	var scanPorts []int
    countSplit := strings.Split(ports, ",")
	for _,port := range countSplit{
		port = strings.Trim(port, " ")
		upper := port
		if strings.Contains(port,"-"){
			ranges := strings.Split(port,"-")
			if len(ranges) < 2 {
				continue
			}
			sort.Strings(ranges)
			port = ranges[0]
			upper = ranges[1]
		}
		start, _ := strconv.Atoi(port)
		end, _ := strconv.Atoi(upper)
		for i := start; i <= end; i++ {
			scanPorts = append(scanPorts, i)
		}
	}
	//fmt.Println(scanPorts)
	return scanPorts
}

/*
func squarer(results chan string, scanport chan string) chan string{   //存入的值转换为取出的值
	for i := range scanport {
		results <- i
	}
	close(results)
	return results
}*/
//工人
func worker(scanport chan string) {
	//函数结束释放连接
	defer wg.Done()
	for {
		address, ok := <-scanport
		if !ok {
			break
		}
		//fmt.Println("address:", address)
		conn, err := net.Dial("tcp", address)
		//conn, err := net.DialTimeout("tcp", address, 10)
		if err != nil {
			//fmt.Println("close:", address, err)
			continue
		}
		conn.Close()
		fmt.Println("open:", address)
	}
}
```
