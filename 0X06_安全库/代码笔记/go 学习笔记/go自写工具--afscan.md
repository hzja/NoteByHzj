<a name="R6Jl3"></a>
## github地址
[https://github.com/AuFeng111/afscan](https://github.com/AuFeng111/afscan)

自己写工具。中间走了很多弯路，踩了很多坑。但是最后还是写了个简单的小工具，暂时只有2个功能。<br />自己写，好处就是不用担心免杀，也不用担心流量特征。代码想怎么改就怎么改。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1633941571317-e8416690-8961-4d2a-883c-13ec38775709.png#clientId=u60bf839b-ceb3-4&from=paste&height=510&id=ue544871c&originHeight=680&originWidth=1358&originalType=binary&ratio=1&rotation=0&showTitle=false&size=107918&status=done&style=none&taskId=uc2b25323-a3fd-4c68-89af-b741d5068a4&title=&width=1019)

<a name="LYkSH"></a>
## 页面功能点
```go
package main

import (
    //"pac"
    "fmt"
    "flag"
    //"log"
    //"icmp"
    "afscan/icmpalive"
    "afscan/portscan"
)
var model,ip,port string
var t int
var y int

func init(){
	flag.StringVar(&model,"model","", "icmpalive\nportscan")
    flag.StringVar(&ip,"ip","127.0.0.1", "192.168.1-255.1-10\n192.168.1.1/24\n192.168.1.*")
	flag.IntVar(&t,"t",500, "thread") 
    flag.StringVar(&port,"port","22,80,445,3389,8000-9000","22,80,445,3389,8000-9000")
    
    flag.Usage = func() {
        fmt.Printf("\nUsage: \n./afscan.exe -model icmpalive -ip 121.4.236.95/24\n./afscan.exe -model icmpalive -ip 121.4.236.1/24 -t 500\n./afscan.exe -model portscan -ip 192.168.201.1/24 -port 22,445,1-10000 \n\n                       --by au7eng\n\nOptions:\n")
        flag.PrintDefaults()  //输出flag
    }
    flag.Parse()  //解析flag
}

func main() {
    //基本用法
    switch model {
    case "icmpalive":
        icmpalive.Main(t,ip)
    case "portscan":
        portscan.Main(port,t,ip)
    default:
        flag.Usage()

    }
}
```

<a name="I07Cw"></a>
## 端口扫描功能点
```go
package portscan

import (
	"fmt"
	//"gohive"
	"net"
	"sync"
	"time"
	"strings"
	"strconv"
	"sort"
	"afscan/icmpalive"
	//"github.com/gpool"
	//"reflect"
)

var wg sync.WaitGroup

func Main(ports string,t int,ip string)  {
	ipalive := icmpalive.Main(t,ip)
	fmt.Println("ipalive  end")
	var begin = time.Now()

	//ipalive :=[...]string{"10.82.200.91","10.82.200.86","10.82.200.1","10.82.200.98","10.82.200.68","10.82.200.81","10.82.200.75","10.82.200.79","10.82.200.54","10.82.200.11","10.82.200.48","10.82.200.20","10.82.200.61","10.82.200.90","10.82.200.15","10.82.200.40","10.82.200.8","10.82.200.18","10.82.200.103","10.82.200.26","10.82.200.64","10.82.200.89","10.82.200.87","10.82.200.27","10.82.200.100","10.82.200.47","10.82.200.43","10.82.200.2","10.82.200.109","10.82.200.49","10.82.200.51","10.82.200.131"}
	//ipalive :=[...]string{"121.4.236.99","121.4.236.5","121.4.236.9","121.4.236.14","121.4.236.6","121.4.236.11","121.4.236.8","121.4.236.59","121.4.236.96","121.4.236.84","121.4.236.2","121.4.236.10","121.4.236.53","121.4.236.39","121.4.236.62","121.4.236.63"}
	//ipalive :=[...]string{"10.82.10.30","10.82.10.33","10.82.10.103","10.82.10.106","10.82.10.40","10.82.10.21","10.82.10.104","10.82.10.111","10.82.10.110","10.82.10.109","10.82.10.55","10.82.10.108","10.82.10.66","10.82.10.150","10.82.10.102","10.82.10.101","10.82.10.151","10.82.10.100","10.82.10.152","10.82.10.170","10.82.10.160"}
	//ipalive :=[...]string{"192.168.0.3","192.168.0.112","192.168.0.117","192.168.0.1"}
	var scanport = make(chan string,1000)
	//pool := gpool.New(1000)
	//results := make(chan string, 100)
	//fmt.Println(reflect.TypeOf(ipalive))
	//fmt.Scanf("%s",&str)
	a := parsePort(ports)      //分割端口
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

	for i := 0; i < 5000; i++ {
		wg.Add(1)
		//pool.Add(1)
		go worker(scanport)
		//go worker(scanport,pool)

	}
	/*
	for work := 0; work < pool_size; work++ {
		wg.Add(1)
		pool.Submit(worker)
	}*/
	//等待结束
	wg.Wait()
	//pool.Wait()
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
		//conn, err := net.Dial("tcp", address)
		conn, err := net.DialTimeout("tcp", address, 3 *time.Second)
		if err != nil {
			//fmt.Println("close:", address, err)
			continue
		}
		conn.Close()
		fmt.Println("open:", address)
	}
	//pool.Wait()
}
```
<a name="VuZwP"></a>
## 主机探测功能点
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
	//"github.com/gpool"
	//"reflect"
)

var wg sync.WaitGroup

func main()  {
	var begin = time.Now()

	//ipalive :=[...]string{"10.82.200.91","10.82.200.86","10.82.200.1","10.82.200.98","10.82.200.68","10.82.200.81","10.82.200.75","10.82.200.79","10.82.200.54","10.82.200.11","10.82.200.48","10.82.200.20","10.82.200.61","10.82.200.90","10.82.200.15","10.82.200.40","10.82.200.8","10.82.200.18","10.82.200.103","10.82.200.26","10.82.200.64","10.82.200.89","10.82.200.87","10.82.200.27","10.82.200.100","10.82.200.47","10.82.200.43","10.82.200.2","10.82.200.109","10.82.200.49","10.82.200.51","10.82.200.131"}
	//ipalive :=[...]string{"121.4.236.99","121.4.236.5","121.4.236.9","121.4.236.14","121.4.236.6","121.4.236.11","121.4.236.8","121.4.236.59","121.4.236.96","121.4.236.84","121.4.236.2","121.4.236.10","121.4.236.53","121.4.236.39","121.4.236.62","121.4.236.63"}
	//ipalive :=[...]string{"10.82.10.30","10.82.10.33","10.82.10.103","10.82.10.106","10.82.10.40","10.82.10.21","10.82.10.104","10.82.10.111","10.82.10.110","10.82.10.109","10.82.10.55","10.82.10.108","10.82.10.66","10.82.10.150","10.82.10.102","10.82.10.101","10.82.10.151","10.82.10.100","10.82.10.152","10.82.10.170","10.82.10.160"}
	ipalive :=[...]string{"192.168.1.7","192.168.1.1","192.168.1.4","192.168.1.202"}
	var scanport = make(chan string,1000)
	//pool := gpool.New(1000)
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

	for i := 0; i < 5000; i++ {
		wg.Add(1)
		//pool.Add(1)
		go worker(scanport)
		//go worker(scanport,pool)

	}
	/*
	for work := 0; work < pool_size; work++ {
		wg.Add(1)
		pool.Submit(worker)
	}*/
	//等待结束
	wg.Wait()
	//pool.Wait()
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
		//conn, err := net.Dial("tcp", address)
		conn, err := net.DialTimeout("tcp", address, 1 *time.Second)
		if err != nil {
			//fmt.Println("close:", address, err)
			continue
		}
		conn.Close()
		fmt.Println("open:", address)
	}
	//pool.Wait()
}
```

<a name="M81YI"></a>
## webtitle识别
```go
package webtitle

import (
	// "encoding/json"
	"afscan/portscan"
	"fmt"
	"sort"
	"strings"
	"sync"

	"github.com/goscraper/goscraper" //解决乱码的一个第三方title识别库
	//"reflect"
)

func main() {
	ports := "443"
	ip := "210.76.75.192/24"
	Webtitle(ports, ip)
}

var wg sync.WaitGroup

func Webtitle(ports string, ip string) {

	addr := portscan.Main(ports, 500, ip) //先icmp后端口扫描
	// addr := portscan.Second_Main(ports, ip) //直接端口判断，不进行icmp
	if len(addr) > 0 {
		for _, ips := range addr {
			wg.Add(1)
			//fmt.Println(ips)
			go title_scan(ips)
		}
		wg.Wait()
	}

}

func title_scan(ips string) {
	//var a [][]string
	//忽略https的校验
	/*
		var tr = &http.Transport{
			MaxIdleConns:      30,
			IdleConnTimeout:   time.Second,
			DisableKeepAlives: true,
			TLSClientConfig:   &tls.Config{InsecureSkipVerify: true},
			DialContext: (&net.Dialer{
				Timeout:   time.Second,
				KeepAlive: time.Second,
			}).DialContext,
		}*/

	defer wg.Done()
	var url string
	//client := &http.Client{Transport: tr, Timeout: time.Second}
	countSplit := strings.Split(ips, ":")
	HTTPS_PORT := []string{"443", "9443", "7443", "8443", "6443"}
	if in(countSplit[1], HTTPS_PORT) == true {
		url = "https://" + ips
	} else {
		url = "http://" + ips
	}
	s, err := goscraper.Scrape(url, 1)
	if err != nil {
		//fmt.Println(err)
		return
	}

	fmt.Printf("%s %-20s\n", url, strings.TrimSpace(s.Preview.Title))
}


func in(target string, str_array []string) bool {
	sort.Strings(str_array)
	index := sort.SearchStrings(str_array, target)
	if index < len(str_array) && str_array[index] == target {
		return true
	}
	return false
}

```
webtitle需要写一个带缓冲的channel解决一些高并发的问题
<a name="IEqcq"></a>
## 修改
icmp协议探测需要高权限才行<br />端口扫描需要摆脱ip存活探测。<br />协程池？<br />优秀的channel


<a name="NHbFA"></a>
## 需要读懂的源码
[https://github.com/MsloveDl/Finder](https://github.com/MsloveDl/Finder)<br />[https://github.com/Mitooooo/GoScanPortScript/blob/master/portScan.go](https://github.com/Mitooooo/GoScanPortScript/blob/master/portScan.go)<br />[https://github.com/FDlucifer/goportscan/blob/master/sourcecode/scanallports.go](https://github.com/FDlucifer/goportscan/blob/master/sourcecode/scanallports.go)<br />[https://github.com/vus520/go-scan/blob/master/main.go](https://github.com/vus520/go-scan/blob/master/main.go)
