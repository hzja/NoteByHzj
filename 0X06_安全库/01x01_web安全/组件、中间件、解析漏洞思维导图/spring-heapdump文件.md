黑盒经常会下载到heapdump文件，这个文件里边可以获取到密码、shiro-key等信息

<a name="Yhrkb"></a>
## heapdump_tool.jar
[https://github.com/wyzxxz/heapdump_tool](https://github.com/wyzxxz/heapdump_tool)<br />java -jar .\heapdump_tool.jar .\heapdump<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648090108031-e83d7d67-df8e-4a3d-a532-ca1943ef7b1f.png#clientId=u52e1fbfe-cdfd-4&from=paste&height=254&id=ud1623c22&originHeight=338&originWidth=1213&originalType=binary&ratio=1&size=66770&status=done&style=none&taskId=ua618a265-24c9-40d4-972d-a51fa70bf4c&width=910)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648090678371-c43b3164-5cd5-45fa-842c-22aee2a102d5.png#clientId=u52e1fbfe-cdfd-4&from=paste&height=708&id=u0124a698&originHeight=944&originWidth=744&originalType=binary&ratio=1&size=176827&status=done&style=none&taskId=u7e93b173-9b5e-4183-8040-1574dea600f&width=558)

<a name="E2yk9"></a>
## Eclipse Memory Analyzer
操作方式如下图所示，获取所有明文密码
```java
select * from java.util.Hashtable$Entry x WHERE (toString(x.key).contains("pass"))
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1648090884565-0be94282-13e3-4d72-89c6-32fb6ac8bb99.png#clientId=u52e1fbfe-cdfd-4&from=paste&height=728&id=u0d4382be&originHeight=971&originWidth=1501&originalType=binary&ratio=1&size=177287&status=done&style=none&taskId=u479ab90b-fe16-4a7a-a18d-f2d1b20dbc3&width=1126)

