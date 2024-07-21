<a name="UNDqA"></a>
## 随多少个任务就多少个协程(cpu要求很高
随多少个任务就多少个协程，适合一定中小范围，不适合百万级别的并发<br />参考代码：<br />[https://github.com/vus520/go-scan/blob/master/main.go](https://github.com/vus520/go-scan/blob/master/main.go)
```go
// 用于协程任务控制
    wg := sync.WaitGroup{}
    parallelChan := make(chan int, parallelCounts)
    for i := 0; i < len(ips); i++ {
        ip := net.ParseIP(ips[i])
        wg.Add(1)
        parallelChan <- 1
        go checkPort(ip, port, &wg, &parallelChan)
    }
    wg.Wait()

}
```
```go
func checkPort(ip net.IP, port int, wg *sync.WaitGroup, parallelChan *chan int) {
    defer wg.Done()

    target := ip.String() + ":" + strconv.Itoa(port)

    if verbose > 0 {
        fmt.Println("checking " + target)
    }

    conn, err := net.DialTimeout(protocol, target, time.Millisecond*time.Duration(timeOut))
    if err == nil {
        printOpeningPort(ip, port)
        conn.Close()
    }
    <-*parallelChan
}
```

<a name="Xfou9"></a>
## (先生产，后消费）型channel 
（先生产，后消费）型channel 存在最大的问题是，先生产后消费，所以先定义好的channel容易超出界限，需要定义很大的一个值
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
	for j := 1; j <= 10; j++ { //如果j界限大于1000，就会报错。
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

<a name="sD0hX"></a>
## 一边生产一边消耗类型

```go
func Main(ports string, t int, ip string) []string { //先进行icmp探测
	ipalive := icmpalive.Main(t, ip)

	var begin = time.Now()

	var scanport = make(chan string, 1000)

	a := parsePort(ports) //分割端口
	//fmt.Println(a)
	//results = squarer(scanport,results)
	go func() {
		//fmt.Println(a)
		//用于生成ip:port,并且存放到地址管道种
		for _, ip := range ipalive {
			for i := 0; i < len(a); i++ {
				var address = fmt.Sprintf("%s:%d", ip, a[i])
				scanport <- address
				//fmt.Println(scanport,' ',i)
			}
		}
		close(scanport)
		//var elapseTime = time.Now().Sub(begin)
	}()
	//fmt.Println("通道获取长度:", len(scanport))

	for i := 0; i < 5000; i++ {
		wg.Add(1)
		//pool.Add(1)
		go worker(scanport)
		//go worker(scanport,pool)

	}
	wg.Wait()
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime, "目标端口开放数量 :", len(addresses))
	return addresses
}


func worker(scanport chan string) {
	//函数结束释放连接
	defer wg.Done()
	for {
		address, ok := <-scanport
		if !ok {
			break
		}

		conn, err := net.DialTimeout("tcp", address, 1*time.Second)
		if err != nil {
			//fmt.Println("close:", address, err)
			continue
		}
		conn.Close()
		fmt.Println("open:", address)
		addresses = append(addresses, address)
	}

}

```
<a name="eQfJY"></a>
## 无限大缓存通道(好用
无限大缓存通道，先计算好要执行的任务数量，直接算进缓存通道，然后再开始跑多线程
```html
package portscan2

import (
	"afscan/icmpalive"
	"fmt"
	"net"
	"sort"
	"strconv"
	"strings"
	"sync"
	"time"
)

type Addr struct {
	ip   string
	port int
}

//var addresses []string

func PortScan(ip string, ports string, t int, timeout int64) []string {
	var addresses []string

	var begin = time.Now()
	hostslist := icmpalive.Main(t, ip) //先写死500线程
	probePorts := parsePort(ports)
	workers := 600
	Addrs := make(chan Addr, len(hostslist)*len(probePorts))
	//results := make(chan string, len(hostslist)*len(probePorts))
	var wg sync.WaitGroup

	for i := 0; i < workers; i++ {
		go func() {
			for addr := range Addrs {
				a := PortConnect(addr, timeout, &wg)
				//PortConnect(addr, results, timeout, &wg)
				wg.Done()
				if a != "" {
					addresses = append(addresses, a)
				}
			}
		}()
	}

	//添加扫描目标
	for _, port := range probePorts {
		for _, host := range hostslist {
			wg.Add(1)
			Addrs <- Addr{host, port}
		}
	}
	wg.Wait()
	close(Addrs)
	//close(results)
	var elapseTime = time.Now().Sub(begin)
	fmt.Println("耗时:", elapseTime, "目标端口开放数量 :", len(addresses))
	return addresses
}

func PortConnect(addr Addr, Timeout int64, wg *sync.WaitGroup) string {
	host, port := addr.ip, addr.port
	conn, err := net.DialTimeout("tcp4", fmt.Sprintf("%s:%v", host, port), time.Duration(Timeout)*time.Second)
	defer func() {
		if conn != nil {
			conn.Close()
		}
	}()
	if err != nil {
		return ""
	}
	address := host + ":" + strconv.Itoa(port)
	//addresses = append(addresses, address)
	fmt.Println("open:", address)
	return address
}

func parsePort(ports string) []int {
	var scanPorts []int
	countSplit := strings.Split(ports, ",")
	for _, port := range countSplit {
		port = strings.Trim(port, " ")
		upper := port
		if strings.Contains(port, "-") {
			ranges := strings.Split(port, "-")
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

```

<a name="AMXfy"></a>
## 我用习惯的并发伪代码
```go
func main() {
	var wg sync.WaitGroup
	var domains []string
	//....已经实现了计算domains的长度
	a := make(chan string, len(domains))
	for l := 0; l < t; l++ {
		go func() {
			for i := range a {  //channel中取出来进行下一个方法
				get_hash2(i, &wg)
				//wg.Done()
			}
		}()
	}
	for _, domain := range domains {
		wg.Add(1)
		a <- domain //存入channel池
	}
	wg.Wait()
	close(a)
}

func get_hash2(i string,wg *sync.WaitGroup) {
	defer wg.Done()  //运行一次，channel池丢一个
	//忽略中间一堆实现的代码
	check_cdn(i,a)
}

func check_cdn(i string, a []string) {
	//实现代码...
}
```
