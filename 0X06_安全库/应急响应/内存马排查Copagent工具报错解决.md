直接下载<br />[https://github.com/LandGrey/copagent/raw/release/cop.jar](https://github.com/LandGrey/copagent/raw/release/cop.jar)

报错如下<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639381458620-c5429f46-a344-430a-9544-b37f82c70898.png#clientId=ud8018feb-e504-4&from=paste&height=131&id=ubc9d93c5&originHeight=245&originWidth=966&originalType=binary&ratio=1&size=120314&status=done&style=none&taskId=ufbf87b53-b593-4a4f-876d-6ee176f553f&width=518)

解决 ： 安装tools.jar 放到jre目录 /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639381498827-f2f8572e-57d9-4707-80bd-a818d30b3d43.png#clientId=ud8018feb-e504-4&from=paste&height=145&id=u13884088&originHeight=131&originWidth=500&originalType=binary&ratio=1&size=46302&status=done&style=none&taskId=u85af4ff1-c631-47b2-aa33-bdde71d0e2e&width=555)<br /> <br />使用工具shiro_attack注入了几个内存马进行分析<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639411836886-36eea25c-7a4f-450d-8ac9-3b8a030f7c83.png#clientId=u7aee492b-f414-4&from=paste&height=406&id=u7c5d5d92&originHeight=585&originWidth=857&originalType=binary&ratio=1&size=300362&status=done&style=none&taskId=ue713d314-b322-4b3f-9d01-9ef5dab189e&width=595.5)

目录切换的坑，直接切换是切不了的，$前要加\   tips ：直接用tab换行进行cd切换<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639412337721-bd599d88-270b-466e-86f0-5c7a4218c2c9.png#clientId=u7aee492b-f414-4&from=paste&height=205&id=u0692e3ac&originHeight=324&originWidth=1001&originalType=binary&ratio=1&size=246923&status=done&style=none&taskId=uc23e670d-2f6f-467f-810b-c514d8643ec&width=633.5)<br /> TemplatesImpl$TransletClassLoader  <br /> becl  


[https://developer.aliyun.com/article/764933#slide-1](https://developer.aliyun.com/article/764933#slide-1)

SC：查看 JVM 已加载的类信息<br />通过 SC 我们可以看到我们这个类的详细信息，包括是从哪个 jar 包读取的，他是不是接口/枚举类等，甚至包括他是从哪个类加载器加载的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639408931108-49225410-ad14-409f-b08e-5c2f465cd1fe.png#clientId=u573b9407-8a63-4&from=paste&height=462&id=u163e3a81&originHeight=441&originWidth=387&originalType=binary&ratio=1&size=263539&status=done&style=none&taskId=uc3e0bac8-c467-4b56-847e-fa74d388b03&width=405.5)


内存马就是通过一些字节加载到jvm中生成class文件，从而实现jsp不落地，所以检测内存马需要对class文件进行检测


迅速 判断内存马类型图<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639409922508-d970acc4-4772-49fd-a70d-18a6ea0e0b10.png#clientId=u337fc6e7-7b33-4&from=paste&height=359&id=u31182e87&originHeight=717&originWidth=1271&originalType=binary&ratio=1&size=86501&status=done&style=none&taskId=u9f1e7d01-d135-4130-a1ca-bdb77d57afe&width=635.5)
