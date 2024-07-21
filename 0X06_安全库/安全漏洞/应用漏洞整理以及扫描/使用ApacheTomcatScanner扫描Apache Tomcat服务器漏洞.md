Apache Tomcat
<a name="wgwt2"></a>
## 关于ApacheTomcatScanner 
ApacheTomcatScanner是一个功能强大的Python脚本，该脚本主要针对Apache Tomcat服务器安全而设计，可以帮助广大研究人员轻松扫描和检测Apache Tomcat服务器中的安全漏洞。
<a name="tr3Cv"></a>
## 功能介绍 
1、支持使用多线程Worker搜索Apache Tomcat服务器；<br />2、支持扫描多个目标：支持接收一个Windows域中的目标计算机列表，支持从文件按行读取目标，支持使用`--t`/`--target`选项读取单个目标（IP/DNS/CIDR）；<br />3、支持自定义即设置端口列表；<br />4、支持测试/manager/html访问和默认凭证；<br />5、支持使用`--list-cves`选项查看每个版本的CVE漏洞信息；
<a name="R8oLa"></a>
## 工具安装 
由于该工具基于Python开发，因此首先需要在本地设备上安装并配置好Python环境。接下来，使用下列命令将该项目源码克隆至本地：
```bash
git clone https://github.com/p0dalirius/ApacheTomcatScanner.git
```
除此之外，还可以通过PyPi下载最新版本的ApacheTomcatScanner：
```bash
sudo python3 -m pip install apachetomcatscanner
```
<a name="QFYkw"></a>
## 工具使用 
```bash

$ ./ApacheTomcatScanner.py -h
Apache Tomcat Scanner v2.3.2 - by @podalirius_
usage: ApacheTomcatScanner.py [-h] [-v] [--debug] [-C] [-T THREADS] [-s] [--only-http] [--only-https] [--no-check-certificate] [--xlsx XLSX] [--json JSON] [-PI PROXY_IP] [-PP PROXY_PORT] [-rt REQUEST_TIMEOUT] [-tf TARGETS_FILE]
                              [-tt TARGET] [-tp TARGET_PORTS] [-ad AUTH_DOMAIN] [-ai AUTH_DC_IP] [-au AUTH_USER] [-ap AUTH_PASSWORD] [-ah AUTH_HASH]
A python script to scan for Apache Tomcat server vulnerabilities.
optional arguments:
  -h, --help            show this help message and exit
  -v, --verbose         Verbose mode. (default: False)
  --debug               Debug mode, for huge verbosity. (default: False)
  -C, --list-cves       List CVE ids affecting each version found. (default: False)
  -T THREADS, --threads THREADS
                        Number of threads (default: 5)
  -s, --servers-only    If querying ActiveDirectory, only get servers and not all computer objects. (default: False)
  --only-http           Scan only with HTTP scheme. (default: False, scanning with both HTTP and HTTPs)
  --only-https          Scan only with HTTPs scheme. (default: False, scanning with both HTTP and HTTPs)
  --no-check-certificate
                        Do not check certificate. (default: False)
  --xlsx XLSX           Export results to XLSX
  --json JSON           Export results to JSON
  -PI PROXY_IP, --proxy-ip PROXY_IP
                        Proxy IP.
  -PP PROXY_PORT, --proxy-port PROXY_PORT
                        Proxy port
  -rt REQUEST_TIMEOUT, --request-timeout REQUEST_TIMEOUT
  -tf TARGETS_FILE, --targets-file TARGETS_FILE
                        Path to file containing a line by line list of targets.
  -tt TARGET, --target TARGET
                        Target IP, FQDN or CIDR
  -tp TARGET_PORTS, --target-ports TARGET_PORTS
                        Target ports to scan top search for Apache Tomcat servers.
  -ad AUTH_DOMAIN, --auth-domain AUTH_DOMAIN
                        Windows domain to authenticate to.
  -ai AUTH_DC_IP, --auth-dc-ip AUTH_DC_IP
                        IP of the domain controller.
  -au AUTH_USER, --auth-user AUTH_USER
                        Username of the domain account.
  -ap AUTH_PASSWORD, --auth-password AUTH_PASSWORD
                        Password of the domain account.
  -ah AUTH_HASH, --auth-hash AUTH_HASH
                        LM:NT hashes to pass the hash for this user.
```
<a name="Rnztz"></a>
## 参数解释 
```bash
-h, --help：显示帮助信息和退出
-v, --verbose：开启Verbose模式（默认：False）
--debug：开启调试模式（默认：False）
-C, --list-cves：显示每个受影响版本的CVE ID（默认：False）
-T THREADS, --threads THREADS：设置线程数量（默认：5）
-s, --servers-only：如果查询活动目录，只会获取服务器，而不是所有计算机（默认：False）
--only-http：仅扫描HTTP（默认：False，扫描HTTP和HTTPS）
--only-https：仅扫描HTTPS（默认：False，扫描HTTP和HTTPS）
--no-check-certificate：不检测证书（默认：False）
--xlsx XLSX：将结果导出为XLSX
--json JSON：将结果导出为JSON
-PI PROXY_IP, --proxy-ip PROXY_IP：代理IP
-PP PROXY_PORT, --proxy-port PROXY_PORT：代理端口
-rt REQUEST_TIMEOUT, --request-timeout REQUEST_TIMEOUT：请求超时
-tf TARGETS_FILE, --targets-file TARGETS_FILE：包含目标的文件路径
-tt TARGET, --target TARGET：目标IP、FQDN或CIDR
-tp TARGET_PORTS, --target-ports TARGET_PORTS：待扫描的Apache Tomcat服务器端口
-ad AUTH_DOMAIN, --auth-domain AUTH_DOMAIN：待认证的Windows域
-ai AUTH_DC_IP, --auth-dc-ip AUTH_DC_IP：域控制器的IP地址
-au AUTH_USER, --auth-user AUTH_USER：域账号的用户名
-ap AUTH_PASSWORD, --auth-password AUTH_PASSWORD：域账号的密码
-ah AUTH_HASH, --auth-hash AUTH_HASH：用户认证LM:NT哈希
```
<a name="YGhuO"></a>
## 工具使用样例 
![example.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663161803968-7b2d48bb-c1d6-4538-85f3-f11b393d2c1c.png#clientId=ud5a96d90-29e8-4&from=ui&id=ue3041263&originHeight=367&originWidth=1688&originalType=binary&ratio=1&rotation=0&showTitle=false&size=87118&status=done&style=none&taskId=uae042307-17b6-412f-a6a2-a3cc85e86bd&title=)![example_list_cves.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663161803992-7a5ea3be-6703-4f19-a9ba-790566b3144b.png#clientId=ud5a96d90-29e8-4&from=ui&id=uc849905e&originHeight=872&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=230054&status=done&style=none&taskId=u12e862eb-0afa-4542-b13c-53dadab9f58&title=)
<a name="VrUcc"></a>
## 项目地址 
**ApacheTomcatScanner：**[https://github.com/p0dalirius/ApacheTomcatScanner](https://github.com/p0dalirius/ApacheTomcatScanner)
