```go
func main() {
	var wg sync.WaitGroup
	var domains []string

	//file, err := os.Open("D:\\vscode\\python\\杂技\\webalive\\WebAliveScan-master\\target.txt")
	file, err := os.Open(file)
	if err != nil {
		log.Fatal(err)
	}
	reader := bufio.NewReader(file)
	for {
		line, err := reader.ReadBytes('\n')
		if err != nil {
			if err == io.EOF {
				break
			}
			log.Fatal(err)
		}
		domains = append(domains, strings.TrimSpace(string(line)))
	}
	fmt.Println("已经获取以下域名: ")
	fmt.Println("任务量长度为: ", len(domains))
	fmt.Println(domains)
	fmt.Println("                      --create by aufeng")
	fmt.Println("开始探测: ")
    
    //=====================下面是channel配合多线程
	a := make(chan string, len(domains))
	for l := 0; l < t; l++ {
		go func() {
			for i := range a {
				get_hash2(i, &wg)
				//wg.Done()
			}
		}()
	}
	for _, domain := range domains {
		wg.Add(1)
		a <- domain
	}
	wg.Wait()
	close(a)
    //=========================================
}

func get_hash2(i string,wg *sync.WaitGroup) {
	defer wg.Done()
    // cookie有csrf反爬，所以设置的cookie比较复杂
	cookie := make(map[string]string)
	cookie["Hm_lpvt_c001bd3b5a4d3a12e3ae1a5373826c40"] = "1649219678"
	cookie["csrftoken"] = "0bhPvisEeD98oQ4EJeCjC0YU8m0iODCo15ZS6HHQyuEwUQNEzEpHgrnpdhC6FhOx"

    //body--post请求的参数
	text :="host="+i+"&node=4&csrfmiddlewaretoken=X2g2EyrLIbrh6TRqjBv6EYAXf7LynbXPYWY5fXGX22WFCTAq91iuipZsk2nmeP9Y"
	payloadx := strings.NewReader(text)
	request,err := http.NewRequest("POST", "https://www.wepcc.com", payloadx)
	if err != nil {
		fmt.Println(err)
		return
	}
	request.Header.Add("Referer", "https://www.wepcc.com")
	request.Header.Add("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36")
	request.Header.Add("Content-type", "application/x-www-form-urlencoded; charset=UTF-8")
	
    //cookie设置的循环套入请求包的cookie值
    for key, value := range cookie {
		cookie := &http.Cookie{
			Name:  key,
			Value: value,
		}
		request.AddCookie(cookie)
	}
    
    
	client:=&http.Client{}
	response,_ := client.Do(request)
	defer response.Body.Close()
	body,_ := ioutil.ReadAll(response.Body)
	//fmt.Printf(string(body))

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
	check_cdn(i,a)
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1661529969776-b04c99b0-5c0f-4b17-ab6a-b9260d277c94.png#clientId=u22428e23-661d-4&from=paste&height=514&id=u041adb15&originHeight=643&originWidth=763&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29161&status=done&style=none&taskId=udc6a00fd-cb91-4f93-8429-01c6650649b&title=&width=610.4)
