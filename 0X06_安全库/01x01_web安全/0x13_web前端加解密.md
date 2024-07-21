[https://www.qqxiuzi.cn/bianma/base64.htm](https://www.qqxiuzi.cn/bianma/base64.htm) BASE加解密<br />[https://www.cmd5.com/hash.aspx?s=123456](https://www.cmd5.com/hash.aspx?s=123456) cmd、各种sha、ntlm加密<br />[https://www.sojson.com/encrypt_aes.html](https://www.sojson.com/encrypt_aes.html) 各种加解密、二维码解码、正则匹配、

web前端常见七种算法：BASE64、MD5、SHA、HMAC、RSA 、AES、ECC

<a name="L8rN0"></a>

## BASE与MD5
有时候会混合加密，例如123456的MD5的32位或者16位加密之后再传一个base64加密<br />MD5(16、32位)→BASE(64、32、16)
```
MD5 32位→e10adc3949ba59abbe56e057f20f883e →base64 → ZTEwYWRjMzk0OWJhNTlhYmJlNTZlMDU3ZjIwZjg4M2U=                                                                                                                                                      
MD5 16位→49ba59abbe56e057                 →base64 → NDliYTU5YWJiZTU2ZTA1Nw==
```
<a name="NCt0x"></a>
## sha加密
SHA是安全散列算法，主要有 SHA-1，SHA-224，SHA-256，SHA-384，和SHA-512这几种单向散列算法

例如123456的加密
```
sha1:    6f1be467900cf5ae57ea2f34e3536635
sha224:		F8CDB04495DED47615258F9DC6A3F4707FD2405434FEFC3CBF4EF4E6
sha256:    8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92
sha384:			0A989EBC4A77B56A6E2BB7B19D995D185CE44090C13E2984B7ECC6D446D4B61EA9991B76A4C2F04B1B4D244841449454
sha512:			BA3253876AED6BC22D4A6FF53D8406C6AD864195ED144AB5C87621B6C233B548BAEAE6956DF346EC8C17F5EA10F35EE3CBC514797ED7DDD3145464E2A0BAB413
```

<a name="U4hNG"></a>
## AES
[https://www.sohu.com/a/201169795_466846](https://www.sohu.com/a/201169795_466846)<br />对称加密算法，AES 的加密公式为 C=E(K,P)，其中 K 为密钥，P 为明文，C 为密文。密钥的长度可以是 128、192 或 256 位。

AES有好几种加密模式: CBC、ECB、GCM(新版本的shiro采用了)

<a name="5HAZy"></a>

## 加解密文章
[https://www.freebuf.com/articles/web/223011.html](https://www.freebuf.com/articles/web/223011.html)<br />[https://www.freebuf.com/articles/web/219410.html](https://www.freebuf.com/articles/web/219410.html)
<a name="9LE1U"></a>
## 案例一
aHR0cDovL3d3dy4wNzU1dHQuY29tL3Avcy9sb2dpbg==

前端加解密，首先看网络请求，一条一条的翻，然后看请求，看哪个url是会发生加密的。这里发现是Login!loginSystem<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105766421-c59b63a9-faf4-453e-8f16-9783c44dd4dd.png#height=699&id=MRqAK&originHeight=699&originWidth=1920&originalType=binary&ratio=1&status=done&style=none&width=1920)<br />[http://c.biancheng.net/view/8015.html](http://c.biancheng.net/view/8015.html)<br />使用$.param()方法将数组或对象转化为字符串序列，以便用于 URL 查询字符串或 Ajax 请求。所以要对loginData进行搜索<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105766909-0ce6088d-77d7-4b1b-9937-a87c97e82290.png#height=370&id=su3Ke&originHeight=370&originWidth=1090&originalType=binary&ratio=1&status=done&style=none&width=1090)<br />F12搜索loginData，一直往下搜索，发现了encrypt字眼，在23717行发现，loginData.password会走$scope.encrypt()函数，<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105767522-196d5d03-faa9-4a45-84c7-4782988980f3.png#height=722&id=nRieF&originHeight=722&originWidth=1920&originalType=binary&ratio=1&status=done&style=none&width=1920)<br />全局搜索$scope.encrypt，终于发现了加密的密钥<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105768049-dc6f92ed-7e15-4b4a-9bb9-c6c9eea1d73b.png#height=267&id=njFQH&originHeight=267&originWidth=1230&originalType=binary&ratio=1&status=done&style=none&width=1230)<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105768593-6f652c72-9f21-4030-bd9e-180e7333ac1f.png#height=550&id=aYwQ4&originHeight=550&originWidth=1555&originalType=binary&ratio=1&status=done&style=none&width=1555)<br />看不懂没关系，因为我们可以利用它，让它加密我们的字典，然后就可以使用加密后的字典进行爆破，为了确认准确情况，需要对前端进行调试，先调好账号密码，然后第二步在数字上点击一下，然后再点击登陆，就会直接执行到断点所在的地方，然后按数字3的地方进行往下调试。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105769193-760c62a1-e928-47a1-b3b4-448a3ec148ca.png#height=764&id=P1G4K&originHeight=764&originWidth=1661&originalType=binary&ratio=1&status=done&style=none&width=1661)<br />慢慢的往下调试，并观察是否加密，在23717行，发现password字段加密了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105769690-eadfbbfa-79eb-485b-9636-20ecca6fedc4.png#height=102&id=J1xlI&originHeight=102&originWidth=1241&originalType=binary&ratio=1&status=done&style=none&width=1241)<br />利用console框进行调试，注意了，需要用console框的话，需要断点调试到loginData.password之前，才能在console框使用，不然会提示报错无法找到。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105770217-73b48cd9-7708-4330-8f99-68bdbd208812.png#height=478&id=NNxU5&originHeight=478&originWidth=1618&originalType=binary&ratio=1&status=done&style=none&width=1618)<br />利用循环数组的形式，把密文输出出来，然后就可以扔进burpsuit爆破了。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618105770647-208ddf67-f29f-425f-8eaa-060574754fc4.png#height=204&id=Kj6bW&originHeight=204&originWidth=497&originalType=binary&ratio=1&status=done&style=none&width=497)


## 案例二
[<br />](https://phmerchant.spdbph.com/sellerv2/user/login)<br />aHR0cHM6Ly9waG1lcmNoYW50LnNwZGJwaC5jb20vc2VsbGVydjIvdXNlci9sb2dpbg==

直接全局搜索encry，然后下断点调试，然后判断js进行输出<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1631017466321-129b8e48-5954-4e93-b8ce-6dd0da4fc7ca.png#clientId=ucc8e90ea-6b79-4&from=paste&height=488&id=uef43a138&originHeight=976&originWidth=1429&originalType=binary&ratio=1&size=124525&status=done&style=none&taskId=u4c1f20a9-6888-4383-92a6-827bc03b72c&width=714.5)
