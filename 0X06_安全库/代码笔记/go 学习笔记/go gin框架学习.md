go get  github.com/gin-gonic/gin

下载的时候，protobuf会有问题，需要单独的git clone一份<br />git clone -- [https://go.googlesource.com/protobuf](https://go.googlesource.com/protobuf)
<a name="X530B"></a>
## 启动端口页面
启动一个get请求的8089端口页面

[http://127.0.0.1:8089/hello](http://127.0.0.1:8089/hello?username=aufeng)
```go
package main

import (
    "github.com/gin-gonic/gin"
)

func main() {
    // 创建一个默认的路由引擎
    r := gin.Default()
    // GET：请求方式；/hello：请求的路径
    // 当客户端以GET方法请求/hello路径时，会执行后面的匿名函数
    r.GET("/hello", func(c *gin.Context) {
        // c.JSON：返回JSON格式的数据
        c.JSON(200, gin.H{
            "message": "Hello world!",
        })
    })
    // 启动HTTP服务，默认在0.0.0.0:8080启动服务
    r.Run(":8089")  
}
```

启动带参数<br />[http://127.0.0.1:8089/hello?username=aufeng](http://127.0.0.1:8089/hello?username=aufeng)
```go
package main

import (
	"github.com/gin-gonic/gin"
)

func main() {
	// 创建一个默认的路由引擎
	r := gin.Default()
	r.GET("/hello", func(c *gin.Context) {
		// c.JSON：返回JSON格式的数据
		username :=c.Query("username")
		c.JSON(200, gin.H{
			"message": "Hello world!",
			"username": username,
		})
	})
	// 启动HTTP服务，默认在0.0.0.0:8080启动服务
	r.Run(":8089")
}
```

<a name="DbQs5"></a>
## 关于响应包报错
c.JSON(http.StatusOK, "ok")是不能放在循环体里面的，否则会报错，而且这个代表最终返回的状态，不能多，只能唯一，例如这样子是不行的<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1681656746550-948baf42-06fa-4f48-a339-1646e6739cbd.png#averageHue=%23fbf9f6&clientId=u6fd01ac2-02ba-4&from=paste&height=192&id=ue61b5772&originHeight=240&originWidth=481&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13576&status=done&style=none&taskId=u4669417c-2cde-4cc0-8854-ed24f971129&title=&width=384.8)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1681656765787-14c1ff5e-c9ff-4421-a536-ad5bd2b0bc4e.png#averageHue=%23aad9b5&clientId=u6fd01ac2-02ba-4&from=paste&height=132&id=u6e17ee5c&originHeight=165&originWidth=783&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=15813&status=done&style=none&taskId=ucb9124a8-09dc-496f-89d3-86a50560cea&title=&width=626.4)

解决办法是放到外层<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1681656799883-6858dfed-4a7c-4a37-afa2-14f98182f6a5.png#averageHue=%23fbfaf7&clientId=u6fd01ac2-02ba-4&from=paste&height=234&id=u2fdcad96&originHeight=293&originWidth=609&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=16447&status=done&style=none&taskId=u495515f4-8a8b-49e8-9606-5ec3ae81796&title=&width=487.2)

<a name="DWV5B"></a>
## 关于空数据返回
结构体定义的字段需要大写，否则无法返回数据<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1681743175991-99aa3e62-e5f3-4d75-90f2-10dcd71e40ca.png#averageHue=%23fbfaf9&clientId=u6fd01ac2-02ba-4&from=paste&height=142&id=ub3c09353&originHeight=177&originWidth=318&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=4392&status=done&style=none&taskId=u3149feb2-56b2-4000-9b99-f0328aa8645&title=&width=254.4)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1681743549196-9d51c4df-a01d-4872-a617-5e250670c784.png#averageHue=%23fcfbfa&clientId=u6fd01ac2-02ba-4&from=paste&height=208&id=uc3e0ebe4&originHeight=260&originWidth=580&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=15268&status=done&style=none&taskId=u6005e5cf-d2c0-491e-be1a-762db8b1f5b&title=&width=464)

c.JSON()返回的数据类型要[]interface {}

<a name="vsGwM"></a>
## 关于接口返回数据
```
package main

import (
	"encoding/json"
	"fmt"
	"github.com/gin-gonic/gin"
	"io/ioutil"
	"net/http"
	"reflect"
)
type need struct {
	Title interface{}
	Tag interface{}
	Method interface{}
	Path interface{}
}

var dataArray []interface {}

func main() {
	router := gin.Default()
	router.GET("readfile", func(c *gin.Context){ //任意文件读取漏洞
		content, err := ioutil.ReadFile("D:\\go1.20.2.windows-amd64\\go\\project\\src\\gotest\\gotest\\相应包.txt")
		if err != nil {
			fmt.Println("Error reading file:", err)
			return
		}
		//fmt.Println(string(content))
		jsonString := string(content)

		var data map[string]interface{}
		err = json.Unmarshal([]byte(jsonString), &data)
		if err != nil {
			fmt.Println("Error parsing JSON:", err)
			return
		}
		dataArray = data["data"].([]interface{})
		//n :=1
		//for _, item := range dataArray {
		//	fmt.Println(n)
		//	title := item.(map[string]interface{})["title"]
		//	tag := item.(map[string]interface{})["tag"]
		//	method := item.(map[string]interface{})["method"]
		//	path := item.(map[string]interface{})["path"]
		//	fmt.Println("title:", title)
		//	fmt.Println("tag:", tag)
		//	fmt.Println("method:", method)
		//
		//	fmt.Println("path:", path)
		//	n = n+1
		//	fmt.Println("")
		//}
		//c.JSON(http.StatusOK, gin.H{
		//	//"message": fmt.Sprintf("'%s'",string(content)),
		//	"message": fmt.Sprintf("'%v'",dataArray),
		//})
		c.JSON(http.StatusOK, dataArray)
	})
	router.GET("/search", func(c *gin.Context) {
		//c.JSON：返回JSON格式的数据
		tag1 :=c.Query("tag")
		fmt.Println("asdasd")
		var needs []need
		var result []need
		if len(tag1) >0 {
			fmt.Println("qwe")
			for _, item := range dataArray {
				//fmt.Println(n)
				title := item.(map[string]interface{})["title"]
				tag := item.(map[string]interface{})["tag"]
				method := item.(map[string]interface{})["method"]
				path := item.(map[string]interface{})["path"]

				needs = append(needs, need{title, tag, method, path})
				//fmt.Println(reflect.TypeOf(title))
				//fmt.Println("title:", title)
				//fmt.Println("tag:", tag)
				//fmt.Println("method:", method)
				//fmt.Println("path:", path)
			}
			for i := 0; i < len(needs); i++ {
				if (needs[i].Tag == tag1) {
					fmt.Println("yes")
					result = append(result, need{needs[i].Title, needs[i].Tag, needs[i].Method, needs[i].Path})

					//fmt.Println(needs[i].title)
					//fmt.Println(needs[i].tag)
					//fmt.Println(needs[i].method)
					//fmt.Println(needs[i].path)
					//fmt.Println(result)
				}
			}
			fmt.Println(result)
			fmt.Println(reflect.TypeOf(result)) //[]main.need形式
			fmt.Println(reflect.TypeOf(dataArray))//[]interface {}才能被c.JSON()解析返回

			//[]main.need形式转换成[]interface {}
			converted := make([]interface{}, len(result))
			for i, v := range result {
				converted[i] = v
			}
			fmt.Println(reflect.TypeOf(converted))
			fmt.Println(converted)
			//fmt.Println(needs)
			c.JSON(http.StatusOK, converted)
		}
	})
	router.GET("/hello", helloHandler)  //路由注册，调用了helloHandler函数
	if err := router.Run(":8800"); err != nil {
		fmt.Println("startup service failed, err:%v\n", err)
	}
	//router.Run(":8800")
}

func helloHandler(c *gin.Context) {  //被hello路径请求然后调用
	a :="hello \n aufeng"
	c.JSON(http.StatusOK, gin.H{
		"message": a,
	})
}

```
<a name="lsOf9"></a>
## 基本路由注册
```go
	router.GET("/hello", helloHandler)  //路由注册，调用了helloHandler函数
	if err := router.Run(); err != nil {
		fmt.Println("startup service failed, err:%v\n", err)
	}

func helloHandler(c *gin.Context) {  //被hello路径请求然后调用
	c.JSON(http.StatusOK, gin.H{
		"message": "Hello aufeng!",
	})
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650506253084-3a5f7092-605a-4293-86f3-c14a063cf7c8.png#averageHue=%23c8f3f1&clientId=ua198427e-503c-4&from=paste&height=230&id=u9e6bc9a3&originHeight=188&originWidth=602&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10907&status=done&style=none&taskId=u36a8428a-009a-4e6c-9cd3-47d5a0257e8&title=&width=737.6000061035156)
<a name="N6xLo"></a>
## 路由拆分成多个文件
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1650528426989-dedacd7c-7029-49a9-ad5f-e5b9112a3c45.png#averageHue=%23fbf9f8&clientId=ua198427e-503c-4&from=paste&height=207&id=bBysF&originHeight=259&originWidth=595&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19980&status=done&style=none&taskId=ub21d6501-1352-4f3a-8bdf-36c157e048d&title=&width=476)<br />login.go
```go
package routers

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

func LoadLogin(e *gin.Engine) {
	e.GET("/login", login)
}

func login(e *gin.Context) {
	e.String(http.StatusOK, "login-登录接口")
}
```

user.go
```go
package routers

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

func LoadUser(e *gin.Engine) {
	e.GET("/userinfo", userinfo)
}

func userinfo(e *gin.Context) {
	e.String(http.StatusOK, "userinfo-用户详情接口")
}
```

main.go
```go
package main

import (
	"fmt"
	"routers"
	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	routers.LoadUser(r)
	routers.LoadLogin(r)
	if err := r.Run(":8089"); err != nil {
		//  输出错误
		fmt.Println("startup service failed, err:%v\n", err)
	}
}
```


<a name="BxSFe"></a>
## 文件上传
```go
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <title>上传文件示例</title>
</head>
<body>
<form action="/upload" method="post" enctype="multipart/form-data">
    <input type="file" name="f1">
    <input type="submit" value="上传">
</form>
</body>
</html>
```
```go
package main

import (
	"fmt"
	"log"
	"net/http"
	"github.com/gin-gonic/gin"
)

func main() {
	router := gin.Default()
	router.LoadHTMLFiles("D:\\vscode\\go\\go-web\\upload\\upload.html")
	router.GET("test", func(c *gin.Context){  //解析html模板
		c.HTML(http.StatusOK,"upload.html",nil)
	})
	router.POST("/upload", func(c *gin.Context) {
		// 单个文件
		file, err := c.FormFile("f1")
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{
				"message": err.Error(),
			})
			return
		}

		log.Println(file.Filename)
		dst := fmt.Sprintf("D:\\vscode\\go\\go-web\\upload\\up-thing\\%s", file.Filename)
		// 上传文件到指定的目录
		c.SaveUploadedFile(file, dst) //保存到
		c.JSON(http.StatusOK, gin.H{
			"message": fmt.Sprintf("'%s' uploaded!",dst),
		})
	})
	router.Run(":8800")
}
```

<a name="pOybp"></a>
## 基本路由写的漏洞代码
```go
package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"log"
	"net/http"
	"os"
)

func main() {
	router := gin.Default()
	router.LoadHTMLFiles("D:\\vscode\\go\\go-web\\upload\\upload.html")
	router.GET("test", func(c *gin.Context){
		c.HTML(http.StatusOK,"upload.html",nil)
	})
	router.GET("urlredirect", func(c *gin.Context){ //url跳转漏洞
		url :=c.Query("url")
		c.Redirect(http.StatusMovedPermanently, url)
	})
	router.GET("readfile", func(c *gin.Context){ //任意文件读取漏洞
		read :=c.Query("read")
		content, err := os.ReadFile(read)
		if err != nil {
			panic(err)
		}
		c.JSON(http.StatusOK, gin.H{
			"message": fmt.Sprintf("'%s'",string(content)),
		})
	})

	router.GET("/hello", helloHandler)  //路由注册，调用了helloHandler函数
	if err := router.Run(); err != nil {
		fmt.Println("startup service failed, err:%v\n", err)
	}

	router.POST("/upload", func(c *gin.Context) { //任意文件上传漏洞
		// 单个文件
		file, err := c.FormFile("f1")
		if err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{
				"message": err.Error(),
			})
			return
		}

		log.Println(file.Filename)
		dst := fmt.Sprintf("D:\\vscode\\go\\go-web\\upload\\up-thing\\%s", file.Filename)
		// 上传文件到指定的目录
		c.SaveUploadedFile(file, dst)
		c.JSON(http.StatusOK, gin.H{
			"message": fmt.Sprintf("'%s' uploaded!",dst),
		})
	})
	router.Run(":8800")
}

func helloHandler(c *gin.Context) {  //被hello路径请求然后调用
	c.JSON(http.StatusOK, gin.H{
		"message": "Hello aufeng!",
	})
}

```
<a name="JqtRJ"></a>
## 

<a name="rKS1T"></a>
## 接收数组
```
//-------- main.go ---------------
package main
import (
	"github.com/gin-gonic/gin"
	"go-use/practise"
)
func main() {
	// 创建一个默认的路由引擎
	engine := gin.Default()
	// 注册路由
	practise.TestRecPostArrValue(engine)
	_ = engine.Run()
}
//------ go-use/practise/param_receice.go -------
// 接收POST提交的数组
func TestRecPostArrValue(engine *gin.Engine) {
	engine.POST("/postArr", func(context *gin.Context) {
		arr := context.PostFormArray("name")
		context.JSON(200, gin.H{
			"postArr": arr,
		})
	})
}


➜ curl -X POST http://127.0.0.1:8080/postArr -d "name[]=张三&name[]=李东"
{"postArr":["张三","李东"]}
```
[http://liuqh.icu/2021/04/29/go/gin/5-param-receive/](http://liuqh.icu/2021/04/29/go/gin/5-param-receive/)

<a name="XcdXr"></a>
## 接收json
```
//--- main.go ---------------
package main
import (
	"github.com/gin-gonic/gin"
	"go-use/practise"
)
func main() {
	// 创建一个默认的路由引擎
	engine := gin.Default()
	// 注册路由
	practise.TestRecPostJson(engine)
	_ = engine.Run()
}
//--- go-use/practise/param_receice.go -------
// 方式一: 将接收POST提交的JSON，直接赋值给变量
func TestRecPostJson(engine *gin.Engine) {
	engine.POST("/postJson", func(context *gin.Context) {
		param := make(map[string]interface{})
		err := context.BindJSON(&param)
		if err != nil {
			context.JSON(500, gin.H{"err": err})
      return
		}
		context.JSON(200, gin.H{"return": param})
	})
}


# 请求
➜ curl -X POST http://127.0.0.1:8080/postJson -d '{
   "name":"张三",
   "age":20,
   "likes":["打游戏","旅游"]
}'
# 返回
{"return":{"age":20,"likes":["打游戏","旅游"],"name":"张三"}}
```
