在一次写代码实现http发包post请求的时候发现不知道怎么实现自动有这个csrftoken的cookie<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1661528640935-cd6f3cb8-f59c-4285-b650-81576c340f0a.png#clientId=u3249bc62-b68e-4&from=paste&height=151&id=uec7cb694&originHeight=189&originWidth=768&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9002&status=done&style=none&taskId=u7400acb3-f5eb-4a24-80f1-2ef3a6ff973&title=&width=614.4)
```go
Cookie: Hm_lpvt_c001bd3b5a4d3a12e3ae1a5373826c40=1649219678; csrftoken=0bhPvisEeD98oQ4EJeCjC0YU8m0iODCo15ZS6HHQyuEwUQNEzEpHgrnpdhC6FhOx
```
后来搞了很久，发现一篇文章解决了问题，伪代码如下，用了map去传值
```go
cookie := make(map[string]string)
cookie["Hm_lpvt_c001bd3b5a4d3a12e3ae1a5373826c40"] = "1649219678"
cookie["csrftoken"] = "0bhPvisEeD98oQ4EJeCjC0YU8m0iODCo15ZS6HHQyuEwUQNEzEpHgrnpdhC6FhOx"
......
......
for key, value := range cookie {
		cookie := &http.Cookie{
			Name:  key,
			Value: value,
		}
		request.AddCookie(cookie)
	}
```

最后逻辑代码
```go
func get_hash2(i string,wg *sync.WaitGroup) {
	defer wg.Done()
	cookie := make(map[string]string)
	cookie["Hm_lpvt_c001bd3b5a4d3a12e3ae1a5373826c40"] = "1649219678"
	cookie["csrftoken"] = "0bhPvisEeD98oQ4EJeCjC0YU8m0iODCo15ZS6HHQyuEwUQNEzEpHgrnpdhC6FhOx"

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
```
