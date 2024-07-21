Web 网站是当今互联网上最主流的业务形态，随着开源 Web 框架和各种建站工具的兴起，搭建网站已经是一件成本非常低的事情，但是网站的安全性很少有人关注，WAF 这个品类也鲜为人知。
<a name="b401z"></a>
## WAF 是什么
WAF 是网站的防火墙，是 Web Application Firewall（Web 应用防火墙） 的缩写，通常以 Web 网关的形式存在，作为反向代理接入，可以保护网站不被黑客所攻击，可以识别常见的 Web 攻击并实施阻断，比如：SQL 注入、跨站脚本攻击（XSS）、跨站请求伪造（CSRF）、服务端请求伪造（SSRF）、WebShell 上传与通信等等。<br />推荐一款简单好用的网站防护工具 —— **雷池 WAF**。
<a name="rjMPV"></a>
## 雷池是什么
雷池是一款简单易用、广受好评的社区 WAF 项目，是一款底层基于 Nginx 的 Web 网关，作为反向代理接入网络，清洗来自黑客的恶意流量，保护你的网站不受黑客攻击。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563567-46904847-8032-4698-bd6a-4dd0c34fab91.png#averageHue=%23e2dac3&clientId=u56ad7339-9267-4&from=paste&id=u5448ab91&originHeight=675&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uca0bf889-39ff-427f-94b5-3a18aed45ff&title=)

- 主页：[_https://waf-ce.chaitin.cn/_](https://waf-ce.chaitin.cn/)
- GitHub：[_https://github.com/chaitin/safeline_](https://github.com/chaitin/safeline)
<a name="lCKqX"></a>
## 项目活跃度
雷池的项目团队长亭科技是网络安全行业的一颗新星，在 GitHub 还维护了 xray、rad、veinmind、passionfruit 等多个知名安全开源项目。<br />雷池项目的发布时间是今年 4 月份，更新速度相当惊人，四个月的时间发布了 32 个版本，被 20000+ 的网站站长所使用，在 GitHub 获得了 3k+ 的 star。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563569-6401d9b9-d3bd-4d1d-b2e0-2496819fabf7.png#averageHue=%23faf9f9&clientId=u56ad7339-9267-4&from=paste&id=ud90a0322&originHeight=533&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u984a0bf1-c6a4-41dc-b897-ce2f9046b56&title=)
<a name="LYti0"></a>
## 安装部署
雷池使用容器化部署，服务由多个 Docker 容器组成，作者还为使用者准备了简单方便的一条龙安装脚本，只要服务器能联网，自动拉取镜像、初始化配置文件、启动服务一气呵成。<br />克隆仓库，执行仓库中的 setup.sh 脚本即可安装
```bash
git clone git@github.com:chaitin/safeline.git
cd safeline
bash ./setup.sh
```
或直接使用在线安装脚本
```bash
bash -c "$(curl -fsSLk https://waf-ce.chaitin.cn/release/latest/setup.sh)"
```
安装完成后根据提示访问本地的 Web 服务端口即可开始使用。
<a name="n057P"></a>
## 工作原理
下图是一个简单的网站拓扑，外部用户发出请求，经过网络最终传递到网站服务器。<br />此时，若外部用户中存在恶意用户，那么由恶意用户发出的攻击请求也会经过网络最终传递到网站服务器。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563574-743e0b89-f92d-48c3-8aec-2d8099f3bc34.png#averageHue=%23000000&clientId=u56ad7339-9267-4&from=paste&id=u394ee79b&originHeight=262&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc27904ad-f74f-4daa-97a7-c2bebc8267a&title=)<br />雷池以反向代理方式接入，优先于网站服务器接收流量，对流量中的攻击行为进行检测和清洗，将清洗过后的流量转发给网站服务器。<br />通过以上行为，最终确保外部攻击流量无法触达网站服务器。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563592-325421cb-387d-406f-a526-cb537b738ccc.png#averageHue=%23000000&clientId=u56ad7339-9267-4&from=paste&id=uf5b5d905&originHeight=262&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u96615c2b-5b2e-4e16-8381-ce2cbfa70b8&title=)
<a name="Kuh4n"></a>
## 核心能力
<a name="LsgIO"></a>
### 防护 Web 攻击、CC 攻击、爬虫
雷池的 Web 攻击检测能力由语义分析算法驱动，不同于通用的语义分析，雷池的自动机引擎 yanshi 可以支持对片段语法进行分析，对 HTTP 请求参数进行自动化递归解码，提取请求参数中的攻击片段，并通过攻击打分模型对参数中的攻击代码进行识别，最终判定 HTTP 请求是否为恶意攻击，具备对通用攻击的防护能力，也具备泛化识别能力，可以实现无规则检测 0Day 攻击。<br />雷池集成了基于恶意 IP 情报、客户端指纹、鼠标键盘行为识别、访问频率限制的人机验证算法，可用来对抗爬虫、对抗扫描、对抗 CC 攻击。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563633-949bc1ef-8389-4a88-9fdd-5556769cc59e.png#averageHue=%23e0e0d9&clientId=u56ad7339-9267-4&from=paste&id=u2dc12ccc&originHeight=686&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9f3e5599-e07f-499a-8498-bf85ef03a0a&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563794-54c6a741-1879-46d6-8e00-c15e4b69d347.png#averageHue=%23f5f6f8&clientId=u56ad7339-9267-4&from=paste&id=u3f29b151&originHeight=315&originWidth=800&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8ad7bab1-db08-4c77-82e1-85ff3e032a9&title=)
<a name="ZI8fy"></a>
### Web 流量访问控制
雷池支持通过可视化页面配置网站的访问控制黑白名单，允许匹配源 IP、路径、域名、Header、Body 等条件。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563952-f8ddee7b-c35e-4212-b511-daf4a44cb12c.png#averageHue=%23dad1c3&clientId=u56ad7339-9267-4&from=paste&id=u8b545a30&originHeight=675&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u43ff1ec7-a024-41e7-88fd-0747f194844&title=)
<a name="vNWss"></a>
### 网站资源识别
雷池基于 HTTP 流量自动识别 Web 资源，从而生成 API 画像持续统计和分析；采用动态基线和预测分析技术分析异常访问行为，精准识别操作正常但请求异常的攻击行为，从正常行为中检测出异常流量。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694617563998-88e6ba33-0cf3-4491-b148-3c77ba7eff39.png#averageHue=%23d7c28f&clientId=u56ad7339-9267-4&from=paste&id=ufb29753a&originHeight=686&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf88bc9a1-06c6-4a28-a248-b3c9045d257&title=)
<a name="oTdWn"></a>
## 项目特性
<a name="zVKHg"></a>
### 安装便捷、使用简单
安装只需要一条命令，界面简单易操作，安全能力开箱即用，不用很强的安全技术背景即可发挥顶尖的安全防护效果。
<a name="FuXMf"></a>
### 安全防护能力足够强
语义分析驱动的 Web 攻击检测算法是长亭首创的技术，在安全行业具备很高的认可度，同时项目还具备 IP 威胁情报、动态限频、智能建模等多种只有企业级产品才有的高级防护能力。
<a name="bwpC0"></a>
### 高性能、高稳定性
在 Nginx 之上增加了攻击检测能力，轻松支撑 10000+ TPS 流量，扩展硬件即可增强能力，攻击检测带来的延迟在 1 毫秒级别。<br />GitHub：[_https://github.com/chaitin/safeline_](https://github.com/chaitin/safeline)
