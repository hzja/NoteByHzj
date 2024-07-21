apk查壳工具检测<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632274809002-4ffd0cd0-8c38-4fa2-b53f-a41778b72ef7.png#clientId=ub8517e94-9fff-4&from=paste&height=219&id=ud344d8dd&originHeight=292&originWidth=506&originalType=binary&ratio=1&size=19824&status=done&style=none&taskId=u0ea8b0b6-e0bb-46f1-890d-b7a9fa8b62a&width=380)

<a name="HqzYS"></a>
## 第一步 apk→zip
需要将脱壳后的apk后缀修改为zip，然后解压缩，可以得到以下文件，其中classes.dex比较重要，因为里边有java代码。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632275237272-1d756102-4307-4f4d-8070-498b63646e5e.png#clientId=ub8517e94-9fff-4&from=paste&id=uc4901e15&originHeight=247&originWidth=323&originalType=binary&ratio=1&size=9951&status=done&style=none&taskId=u316d4f77-55ff-48fc-8ed0-8890c964c65)
<a name="Erc14"></a>
## 第二步 classes.dex→jar
将得到的classes.dex文件使用dex2jar转换成Java的jar包 <br />[https://sourceforge.net/projects/dex2jar/](https://sourceforge.net/projects/dex2jar/)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632275496224-5e2bd42d-da07-4c59-9c72-6ff4e09053f7.png#clientId=ub8517e94-9fff-4&from=paste&height=488&id=u44108c25&originHeight=651&originWidth=480&originalType=binary&ratio=1&size=55090&status=done&style=none&taskId=u14249f74-dd46-4fa4-929b-c0dde5b1c17&width=360)<br />使用d2j-dex2jar.bat脚本文件将classes.dex文件转换成我们所需的jar包
```go
d2j-dex2jar.bat classes.dex
```
  ![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632275693568-1c8ecba9-b49d-4ebd-8767-36911a7111a0.png#clientId=ub8517e94-9fff-4&from=paste&height=296&id=ud2921f1c&originHeight=394&originWidth=1009&originalType=binary&ratio=1&size=52846&status=done&style=none&taskId=u3df48fa9-352f-4f1b-ac8b-096f3297a76&width=757)

<a name="wWL3c"></a>
## 第三步 jar→jd-gui
jar包可以直接解压，也可以直接放进jd-gui里边<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632276305940-667938b2-24d9-4f34-9ed9-6c1bb67c3468.png#clientId=ub8517e94-9fff-4&from=paste&height=495&id=u5c396ea0&originHeight=660&originWidth=1197&originalType=binary&ratio=1&size=244283&status=done&style=none&taskId=udf6324ef-06e3-4e02-804e-72b7ae6785b&width=898)


如果没有壳的apk文件，可以直接扔到jadx里边进行反编译<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1632283163408-1f792cd7-bcfe-432e-8c60-69873e97d871.png#clientId=ub8517e94-9fff-4&from=paste&height=378&id=u31093e6a&originHeight=756&originWidth=1551&originalType=binary&ratio=1&size=161472&status=done&style=none&taskId=ud6cad8b2-e742-4b83-b097-bdf08ea3ad1&width=775.5)
