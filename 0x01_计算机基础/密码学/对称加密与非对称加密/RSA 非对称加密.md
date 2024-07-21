RSA
<a name="BYxM3"></a>
## 前言
随着互联网越来越渗透入生活的方方面面，各种私密信息在网络中传播，为了保证信息的真实可靠，在对其安全性的要求也越来越高，对此，加密是一个永远不过时的话题。非对称加密，全站HTTPS等，深入了解，总会遇到RSA加密算法。在一些特殊行业，如博主工作的互联网金融，RSA加密算法的重要性更是非同一般。<br />可能由于密码学的枯燥，还有加密各种标准的混杂，加密与解密难以测试与验证，再加上大公司有大牛写框架，小公司多用demo，网上有关加密的知识不是很多，一开始尝试了解比较困难，所以总结一下最近学习的一些知识，希望能帮到需要的人。<br />主要聊一聊非对称加密的相关知识、RSA加密算法的数字证书操作，也会附带一些其他相关知识和使用心得。如文章有错漏之处，烦请指出，谢谢。
<a name="aw4NT"></a>
## 数字证书
<a name="UcehA"></a>
### 公钥传输问题
在密文传输过程中，客户端（Client C）向服务器（Server S）发送数据，C使用S的公钥加密，这样只有S使用自己的私钥解密才能拿到信息，其他人即使得到了数据，没有S的私钥也没用。<br />但是如果有一个黑客H告诉C自己是S，并将自己的假公钥发送给C，那么C用假公钥加密数据并将数据发送给了H，那么H就顺利得到了信息，无法起到数据加密的作用。<br />这就需要一个中间人来颁发一个身份证明来证明S是真的S。
<a name="FABSW"></a>
### 数字证书认证中心
这个中间人就是数字证书认证机构。<br />数字证书认证中心(Certificate Authority)（也被称为证书认证机构或CA）是指颁发证书、废除证书、更新证书、验证证书、管理密钥的机构。它能在认证某组织或个人后分发证书的机构，它验证的信息包括已签约的证书，当然它也负责吊销有危害的证书。
<a name="lLSub"></a>
### 数字证书
中间人颁发的身份证明就是数字证书。<br />数字证书是一个包含 证书拥有者公钥、证书拥有者信息、证书认证中心数字签名的文件。 拿到数字证书后，解析证书的证书认证机构数字签名确保证书是真的，且没有被篡改过后，取得其中的公钥，然后就可以使用此公钥与浏览器进行交互了。
<a name="HvAwq"></a>
### 根证书
CA 这么重要，可是怎么能证明 CA 是真的呢？这个不用担心，许多 CA 都有嵌入在浏览器中的根证书，所以浏览器能自动识别它们。在一些API交互中，如请求支付宝的接口时，已经在本地存储了支付宝的证书了。<br />不用担心本地的根证书安全问题，如果本地存储的根证书都被修改了，那么加解密也就没有什么意义了。
<a name="vGAkw"></a>
### 证书链
由于世界上需要证书的组织众多，任何一家 CA 也不能处理全部的认证请求。于是大大小小的 CA 出现了，可是每个客户端不可能把他们的证书作为根证书全存储起来。<br />于是CA建立自上而下的信任链，下级 CA 信任上级 CA，下级 CA 由上级 CA 颁发证书并认证。因为下级 CA 的证书是用上级 CA 的密钥加密的，而上级 CA 的密钥只有自己知道，因此别人无法冒充上级 CA 给别人发证书。<br />在进行证书认证时，服务器会发给客户端一个包含着“证书机构证书”的证书，会层层链接到最上层的 CA，本地拥有最上级的 CA 的证书，如果能证明此 CA 的真实性，那么也便能证明服务器证书的可靠。
<a name="QIZJQ"></a>
### 证书标准
X.509是目前最能用的证书标准， 证书由用户公共密钥和用户标识符组成。此外还包括版本号、证书序列号、CA标识符、签名算法标识、签发者名称、证书有效期等信息。这一标准的最新版本是X.509 v3，它定义了包含扩展信息的数字证书。该版数字证书提供了一个扩展信息字段，用来提供更多的灵活性及特殊应用环境下所需的信息传送。
<a name="X5mBf"></a>
## RSA加密标准
<a name="XwmKy"></a>
### 公钥加密标准
公钥加密标准（Public Key Cryptography Standards, PKCS），此系列标准的设计与发布皆由RSA信息安全公司所制定。包括证书申请、证书更新、证书作废表发布、扩展证书内容以及数字签名、数字信封的格式等方面的一系列相关协议。<br />目前在使用的最高版本为 PKCS#12，这版本也是工作中使用最多的版本，此外还接触过 PKCS#7 版本，java多用 PKCS#8 版本，下面分别说一下。
<a name="vXkg6"></a>
### PKCS#7：
定义一种通用的消息语法，包括数字签名和加密等用于增强的加密机制，PKCS#7与PEM兼容，所以不需其他密码操作，就可以将加密的消息转换成PEM消息。规范了以公开密钥基础设施（PKI）所产生之签名/密文之格式。其拓展数字证书在 S/MIME与CMS 的应用，PKCS#7一般主要用来做数字信封。
<a name="Hqrob"></a>
### PKCS#8
描述私有密钥信息格式，该信息包括公开密钥算法的私有密钥以及可选的属性集等，Apache读取证书私钥的标准，在JAVA平台中使用。（接入一些支付公司中经常会提供此格式的密钥，有些印象）
<a name="nIaoO"></a>
### PKCS#12：
描述个人信息交换语法标准。描述了将用户公钥、私钥、证书和其他相关信息打包的语法。 含有私钥，同时可以有公钥，有口令保护 格式一般为 .pfx。 由于它可以加密码保护，打开时需要一串特殊密码，所以相对安全些。
<a name="lIHR5"></a>
## 证书和密钥文件格式
需要注意：证书文件格式与加密标准并没有严格的对应关系，证书文件格式是存储证书的方式不同，可能存储的内容也略有不同。而加密标准是使用证书文件进行加解密的方式不同。
<a name="EZh5i"></a>
### pem格式
最普通的证书格式，以`-----BEGIN CERTIFICATE-----`开头，以`-----END CERTIFICATE-----`结尾；有些pem证书把私钥也放在了一个文件中，但是很多平台还是需求证书和私钥分开放在不同的文件中。 pem证书有以下特点：

- base64编码；
- 有.pem，.crt，.cer，.key文件后缀；
- Apache等类似服务器使用pem格式证书；
<a name="xq7VY"></a>
### der格式
der格式是pem格式证书的二进制格式，证书和私钥都可以以der格式存储。 其特点为：

- 二进制格式；
- 以.cer或.der格式为后缀；
- 常被用于java平台；
<a name="LHsg3"></a>
### PKCS#7格式
它是一种PKCS#7格式以`-----BEGIN PKCS-----`开头，以`-----END PKCS7-----`结尾，它只能保存证书或证书链，不能保存私钥。 其特点为：

- base64编码；
- 文件后缀为 .p7p，.p7c；
- window或java tomcat等平台支持此类型；
<a name="M87Ed"></a>
### PKCS#12（pfx）格式
它能把服务器证书（包括公钥），中间证书和私钥存储在一起。特点为：

- 二进制文档；
- 以 .pfx 或.p12为后缀；
- 经常在windows系统内被用于导入导出证书和私钥；
- 打开可能需要额外密码；
<a name="IKsPq"></a>
### 密钥的保存
对于密钥（单指公私钥）的保存，并不需要特殊的格式，直接将base64编码后的密钥作为字符串存入文档即可。
<a name="XN6kK"></a>
## RSA加密操作
<a name="fOPJD"></a>
### 密钥生成和使用
```bash
#生产一个1024位的私钥, 保存在 rsa_private_key.pem 文件里
openssl genrsa -out rsa_private_key.pem 1024
#通过私钥生产公钥
openssl rsa -in rsa_private_key.pem -pubout -out pub.pem
```
此外，介绍一下openssl提供的一个测试加密速度的小工具，可以查看一定时间内某算法计算的次数，对各种加密算法的速度有一个大概的认识。
```bash
openssl speed algciper
eg: openssl speed rsa1024
    openssl speed des-ede3 
```
<a name="zdPVn"></a>
### 格式转换
从pfx文件中提取公私钥
```bash
#从pfx文件中获取到密匙对文件，有时会需要密码
openssl pkcs12 -in source.pfx -nocerts -nodes -out key.key 
#从密匙对文件中获取到私匙。
opensll rsa -in key.key -out pri.key 
#从密匙对文件中获取到公匙;
opensll rsa -in key.key -pubout -out pub.key 
#java语言用
openssl pkcs8 -in pri.key -out repri.key -outform der -nocrypt -topk8 
```
<a name="HdpC5"></a>
### 各种证书之间的互相转换
```bash
PEM to DER
openssl x509 -outform der -in certificate.pem -out certificate.der
—————————————————————————————————–
PEM to P7B
openssl crl2pkcs7 -nocrl -certfile certificate.cer -out certificate.p7b -certfile CAcert.cer
———————————————————————————————————————————-
PEM to PFX
openssl pkcs12 -export -out certificate.pfx -inkey privateKey.key -in certificate.crt -certfile CAcert.crt
——————————————————————————————————————————————————
DER to PEM
openssl x509 -inform der -in certificate.cer -out certificate.pem
————————————————————————————————
P7B to PEM
openssl pkcs7 -print_certs -in certificate.p7b -out certificate.cer
————————————————————————————————-
P7B to PFX
openssl pkcs7 -print_certs -in certificate.p7b -out certificate.cer
openssl pkcs12 -export -in certificate.cer -inkey privateKey.key -out certificate.pfx -certfile CAcert.cer
——————————————————————————————————————————————————-
PFX to PEM
openssl pkcs12 -in certificate.pfx -out certificate.cer -nodes
```
<a name="gGF47"></a>
## PHP中使用RSA
作为一个PHPer，当然还要提一下在 PHP 中如何使用 RSA 加密，所用工具依然是强大的openssl扩展：
```php
# 从pfx文件中提取私钥和证书(需要传入密码 $password)：
openssl_pkcs12_read($file_content, $key, $password);
$key['pkey'] => 私钥
$key['cert'] => 证书

# 解析x.509证书
openssl_x509_read($cert);

# 以pkcs7加密标准加解密/签名验签数据：需要注意pkcs7操作都需要使用一个临时文件，多进程时要考虑一下文件冲突问题，还有别忘了最后释放临时文件。
openssl_pkcs7_encrypt()/openssl_pkcs7_decrypt()
openssl_pkcs7_sign()/openssl_pkcs7_verify()

# 从字符串中获取公私钥：
openssl_pkey_get_private()/openssl_pkey_get_public()

# RSA以pkcs#12标准加解密/签名验签数据：
openssl_private(public)_encrypt()/openssl_private(public)_decrypt()
```
<a name="q346d"></a>
## 小结
最后不得不吐槽一下各种不统一标准的支付公司，可能由于系统老，有各种各样的奇葩的加解密要求。不像支付宝和微信这种大平台，纯纯的 RSA 操作，使用起来非常方便。
