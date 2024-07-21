```go
package main

import (
	"log"
	"iprange-master"
	"github.com/gpool"
	"time"
	"icmp"
	"flag"
)
var ipalive []string
var ip string
var t int

func RunTest(i string,pool *gpool.Pool)  {
	a := icmp.Main(i)
	if a !=""{
		ipalive = append(ipalive, a)
	}
	pool.Done()
}

func main()  {

	log.Println("基于icmp探测主机存活     --by aufeng\n\t\texample  可以使用-h查看参数: \n\t\ticmp-alive.exe -ip 192.168.1.1\n\t\ticmp-alive.exe -ip 192.168.1-255.1-10 -t 1000\n\t\t192.168.1.*\n\t\t192.168.1.0/24\n")
	flag.StringVar(&ip,"ip","127.0.0.1", "需要探测的ip范围")
	flag.IntVar(&t,"t",100, "线程控制") 

	flag.Parse() //必须要添加这一行把命令行带进去解析，不然会直接执行

	size:=t
	pool := gpool.New(size) //设置线程池大小
	list, err := iprange.ParseList(ip)
	if err != nil {
		log.Printf("error: %s", err)
	}
	log.Printf("%+v", list)
	rng := list.Expand()

	start := time.Now()  //开始计时
	for _, host := range rng {
		pool.Add(1)
		go RunTest(host.String(),pool)
	}
	pool.Wait()
	elapsed := time.Since(start) //结束计时
	log.Println("该函数执行完成耗时: ", elapsed)
	log.Println("探测存活的主机: ",len(ipalive),ipalive)
}
```
```
package icmp

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"log"
	"net"
	"time"
)

// https://my.oschina.net/wgc2010/blog/2960776
var icmp ICMP

type ICMP struct {
	Type        uint8
	Code        uint8
	Checksum    uint16
	Identifier  uint16
	SequenceNum uint16
}

func Main() {
	bool := Ping("192.168.137.1")
	fmt.Println("结果为:", bool)
}

func Main2(ip string) string {
	return ip
}
func Ping(ip string) bool {
	//开始填充数据包
	icmp.Type = 8 //8->echo message  0->reply message
	icmp.Code = 0
	icmp.Checksum = 0
	icmp.Identifier = 0
	icmp.SequenceNum = 0

	recvBuf := make([]byte, 32)
	var buffer bytes.Buffer

	//先在buffer中写入icmp数据报求去校验和
	binary.Write(&buffer, binary.BigEndian, icmp)
	icmp.Checksum = CheckSum(buffer.Bytes())
	//然后清空buffer并把求完校验和的icmp数据报写入其中准备发送
	buffer.Reset()
	binary.Write(&buffer, binary.BigEndian, icmp)

	Time, _ := time.ParseDuration("1s")
	conn, err := net.DialTimeout("ip4:icmp", ip, Time)
	if err != nil {
		return false
	}
	_, err = conn.Write(buffer.Bytes())
	if err != nil {

		log.Println("conn.Write error:", err)
		return false
	}
	conn.SetReadDeadline(time.Now().Add(time.Second * 1))
	num, err := conn.Read(recvBuf)
	if err != nil {
		//log.Println("conn.Read error:", err)
		return false
	}

	conn.SetReadDeadline(time.Time{})

	if string(recvBuf[0:num]) != "" {
		return true
	}
	return false
}

func CheckSum(data []byte) uint16 {
	var (
		sum    uint32
		length int = len(data)
		index  int
	)
	for length > 1 {
		sum += uint32(data[index])<<8 + uint32(data[index+1])
		index += 2
		length -= 2
	}
	if length > 0 {
		sum += uint32(data[index])
	}
	sum += (sum >> 16)

	return uint16(^sum)
}

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632561297428-279558b2-a677-4e4d-9c6e-ce7809c695e0.png#averageHue=%23282626&clientId=u3c9ff508-afbc-4&from=paste&height=299&id=ue310990e&originHeight=398&originWidth=624&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50884&status=done&style=none&taskId=ub623a67f-ab20-4e0b-a379-aa1d4c52d20&title=&width=468)
