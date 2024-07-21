一次红蓝中，发现某个端口存在目录遍历，泄露了c#源码，下载压缩包。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023584049-5396078a-0e5d-4c8a-a394-a2845a6aebc3.png#clientId=u20b68a41-b747-4&from=paste&height=278&id=u5749054c&originHeight=347&originWidth=807&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60792&status=done&style=none&taskId=u64ef4782-9b98-4f33-a78b-3ac4e3d5004&title=&width=645.6)

解压后查看对应的前端文件，找到对应的dll，进行反编译后，拿到c#后端的逻辑代码<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023658866-906d8850-91a2-40b9-a70e-bc489b38d02a.png#clientId=u20b68a41-b747-4&from=paste&height=228&id=uafda669d&originHeight=285&originWidth=453&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23430&status=done&style=none&taskId=ub3fe09e5-2111-4738-8765-50a16af78fa&title=&width=362.4)

查看源码发现有upload方法<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023623748-ee5a1ec1-0815-4a4f-922a-072c18bae852.png#clientId=u20b68a41-b747-4&from=paste&height=602&id=u47385173&originHeight=753&originWidth=1096&originalType=binary&ratio=1&rotation=0&showTitle=false&size=325379&status=done&style=none&taskId=uc75d473a-eb97-426a-9b7b-6391c8bb1c9&title=&width=876.8)

ConfigurationManager.AppSettings["UploadFilePath"]; 获取web.config中UploadFilePath的值，如果web.config中没有的话就取根目录\\<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023391693-493e6e36-8fdb-42f1-b019-78c9f76dff53.png#clientId=u20b68a41-b747-4&from=paste&height=190&id=uac389fdb&originHeight=238&originWidth=706&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31406&status=done&style=none&taskId=u0b92c7b7-4b74-4ba5-89c7-fc7024b677e&title=&width=564.8)<br />ls_FilePath受file.filecategory值影响
```csharp
string ls_FilePath = "";
	try
	{
		ls_FilePath = ConfigurationManager.AppSettings["UploadFilePath"];
	}
	catch
	{
		ls_FilePath = "\\";
	}
	ls_FilePath += file.filecategory;
```
filecategory只是被定义了string类型，没有被赋值，但是file可控，可以改变filecategory的值是什么<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662022561474-394fa15e-d374-4ccf-b68e-65685ada394c.png#clientId=u20b68a41-b747-4&from=paste&height=376&id=u2f0297b1&originHeight=470&originWidth=445&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45866&status=done&style=none&taskId=u43a908b7-be59-447e-b3e3-41c25e48271&title=&width=356)<br />然后就是一段文件写入的逻辑方法，并没有校验filename的后缀名，就可以写入((ls_FilePath + "\\" + file.filename)文件中<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662022679653-c37f69f7-0971-4ae3-9126-533b1aa04ea5.png#clientId=u20b68a41-b747-4&from=paste&height=368&id=ud8e4a40a&originHeight=460&originWidth=702&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58601&status=done&style=none&taskId=u04afd6f9-42c3-4218-bc62-976a162d79f&title=&width=561.6)

所以到最后只需要控制filename以及data的内容，就能写入webshell 但是这个前端是svc，访问页面后发现有wsdl接口<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023731636-2d12678f-bfb2-4ba6-ab1b-1f8be58ace78.png#clientId=u20b68a41-b747-4&from=paste&height=540&id=ua3f9d1ee&originHeight=675&originWidth=905&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53797&status=done&style=none&taskId=u9242b4b8-e0fb-4131-9a16-17d597abae7&title=&width=724)

用bp插件解包wsdl，就可以得到UploadFile的请求包，其中filename、filemonth、filecategory、data对应的就是源码中的内容，所以需要构造好路径和base64的木马内容，就getshell了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662023172545-ce668671-88f3-4a8c-9376-8a59a7e26459.png#clientId=u20b68a41-b747-4&from=paste&height=649&id=u2403b47c&originHeight=811&originWidth=1191&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51707&status=done&style=none&taskId=ua14a67d8-5941-454f-8057-dfddfdae5a2&title=&width=952.8)
