判断CDN的方法有很多，有根据socket探测返回ip数量进行判断、nslooup进行判断。<br />但是我比较喜欢用多地ping来判断。用国内、海外的服务器去ping域名，看返回的ip是否相同，如果不相同那么就是存在CDN了。

<a name="ET2Br"></a>
## github地址
[https://github.com/AuFeng111/go_cdn](https://github.com/AuFeng111/go_cdn)
<a name="p96gI"></a>
## 思路导图
例如：<br />www.kingsoft.com  有cdn<br />ffbebbs.kingsoft.com 无cdn

![](https://cdn.nlark.com/yuque/0/2021/jpeg/1345801/1637851313221-854f4762-b434-4823-af5d-2e46b7ea30fb.jpeg)

[https://www.wepcc.com/](https://www.wepcc.com/) 这个网站速度应该会快很多，该网站的规律是如下图所示，先获取所有节点的hash<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1637509421920-fc161010-c271-4bc5-9eb3-ecfed2a7d633.png#clientId=u8d3aca82-4d18-4&from=paste&height=376&id=ua8cfcaa8&originHeight=752&originWidth=1556&originalType=binary&ratio=1&size=91661&status=done&style=none&taskId=ua25b2284-35d0-4b9d-9ae0-4d2934fabc3&width=778)<br />然后通过hash去对应服务器进行域名的ping获得ip值<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1637509449076-baaa839e-f986-4c78-a55e-66f6676232ed.png#clientId=u8d3aca82-4d18-4&from=paste&height=327&id=u85408e56&originHeight=653&originWidth=1552&originalType=binary&ratio=1&size=71838&status=done&style=none&taskId=uf0cea7d3-2beb-4ada-a4f7-4b63005ea22&width=776)

<a name="CtHKg"></a>
## 代码实现
获取节点的所有hash
```go
func get_hash(i string, wg *sync.WaitGroup) { //获取服务器的hash
	defer wg.Done()
	//i := "ffbebbs.kingsoft.com"
	resp, err := http.PostForm("https://www.wepcc.com/", url.Values{"host": {i}, "node": {"2,3,6"}})
	if err != nil {
		fmt.Printf("get failed, err:%v\n", err)
		return
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Printf("read from resp.Body failed, err:%v\n", err)
		return
	}
	//fmt.Print(string(body))
	reg := regexp.MustCompile(`<tr class="item" data-id="(?s:(.*?))">`)
	if reg == nil {
		fmt.Println("MustCompile err")
		return
	}
	//提取关键信息
	result := reg.FindAllStringSubmatch(string(body), -1)
	//fmt.Println(result)
	//过滤<></>
	var a []string
	for _, text := range result {
		a = append(a, text[1])
		//fmt.Println(text[1])
	}
	//fmt.Println(a)
	check_ping(i, a)
}
```

利用上一步中的hash进行cdn的探测，有时候服务器不一定能ping出来ip，还会无响应等状态，所以比较的时候要判断好
```go
func check_ping(i string, a []string) { //利用各地的服务器ip节点去进行ping，进行比较
	var ip []string
	for _, b := range a {
		resp, err := http.PostForm("https://www.wepcc.com/check-ping.html", url.Values{"host": {i}, "node": {b}})
		if err != nil {
			fmt.Printf("get failed, err:%v\n", err)
			return
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			fmt.Printf("read from resp.Body failed, err:%v\n", err)
			return
		}
		//var data[] string
		//fmt.Println(string(body))
		//c, err := regexp.MatchString("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}", string(body))
		if strings.Index(string(body), "ipAddress") > 0 { //如果服务器报错，就不会存在ipAddress的字段
			c := regexp.MustCompile("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}").FindAllStringSubmatch(string(body), -1)
			ip = append(ip, c[0][0])
		}
	}
	b := 1
	if len(ip) > 0 { //ip长度大于0再进行比较
		//fmt.Print(len(ip), "   ", i)
		for a := 1; a < len(ip); a++ {
			if ip[0] != ip[a] {
				b = 0
				break
			}
		}
	}
	if b == 1 {
		fmt.Printf("[+]no cdn: %-30s %10s\n", i, ip[0])
	}
	// } else {
	// 	fmt.Printf("[-]have cdn: %-20s\n", i)
	// }
}
```

<a name="RXas1"></a>
## 效果
准确率是百分百的，就是速度慢了一点，但是其实也还行。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1637851604272-4c204129-a2ef-43ad-83ed-3b318c2d1005.png#clientId=u8a96a34c-d7d9-4&from=paste&height=259&id=ue7c00c33&originHeight=346&originWidth=975&originalType=binary&ratio=1&size=71250&status=done&style=none&taskId=uc4c9e772-b909-41f5-9ad4-9e577664b33&width=731)
