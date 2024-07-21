<a name="x7q2F"></a>
## 搭建
[https://github.com/github/codeql-cli-binaries/releases](https://github.com/github/codeql-cli-binaries/releases)<br />[https://github.com/github/codeql/releases](https://github.com/github/codeql/releases)

这两个下载后解压，然后配置环境变量<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764640119-9d08ed0e-4b95-4f38-afd2-72139a9c2231.png#averageHue=%23f9f8f6&clientId=udd827dc5-6be8-4&from=paste&height=120&id=u65252a2a&originHeight=128&originWidth=445&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8972&status=done&style=none&taskId=u830bf1d0-014d-4c67-bd16-4ef57018a80&title=&width=416)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764715287-857cdaa4-de25-4078-b740-e6787c79e000.png#averageHue=%23f2eeea&clientId=udd827dc5-6be8-4&from=paste&height=47&id=u4392d55c&originHeight=50&originWidth=530&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3513&status=done&style=none&taskId=ub6f3bdc3-df3e-4e6c-a519-2f2e5a53784&title=&width=503)

这样子就是ok了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764743303-f558b412-3769-46ba-9460-81fc1548701c.png#averageHue=%23141210&clientId=udd827dc5-6be8-4&from=paste&height=462&id=u2ef5015e&originHeight=577&originWidth=814&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70755&status=done&style=none&taskId=uf6716be4-debe-444c-95a8-2a6c38f2914&title=&width=651.2)

然后在vscode上安装插件，安装好后配置一下codeql的路径，就ojbk了。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764799776-00e1f458-b5c8-4b05-94f5-8b5702ca7d05.png#averageHue=%23bfbc95&clientId=udd827dc5-6be8-4&from=paste&height=466&id=uf265f04b&originHeight=582&originWidth=1023&originalType=binary&ratio=1&rotation=0&showTitle=false&size=119996&status=done&style=none&taskId=ua009cb9a-3f90-4bf6-ab3f-853e4296110&title=&width=818.4)

<a name="Ejqz5"></a>
## 测试
新建个文件夹主要是用来存储codeql的数据库<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764848518-b66af7e3-799a-4fac-b614-490f33d1dd43.png#averageHue=%23f9f8f6&clientId=udd827dc5-6be8-4&from=paste&height=164&id=ub328d605&originHeight=163&originWidth=383&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11250&status=done&style=none&taskId=ue23cb67b-489a-48ca-959a-3feaedce9cb&title=&width=385.3999938964844)

然后执行以下这条命令，这里用的java-sec-code的漏洞环境
```
codeql database create D:\安全\codeql\databses\micro_service --language="java" --command="mvn clean install --file pom.xml" --source-root=E:\java源码\micro_service_seclab-main\micro_service_seclab-main --overwrite
```

如果用其他环境创建数据库的时候报错的话可以加个-DskipTests跳过<br />--command="mvn clean install -DskipTests --file pom.xml"<br /> 跳过测试，构建<br /> --command="mvn clean install --file pom.xml -Dmaven.test.skip=true"<br /> 无论项目结果如何,构建从不失败<br /> --command="mvn -fn clean install --file pom.xml -Dmaven.test.skip=true"

看到目录下有文件，而且编译成功那就ok了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673764946722-835abca5-8131-4c7b-9b23-2f1e15aa2b54.png#averageHue=%23faf9f8&clientId=udd827dc5-6be8-4&from=paste&height=196&id=u25202443&originHeight=245&originWidth=532&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17033&status=done&style=none&taskId=u34707099-9b1f-474f-a7ff-5d6d4947086&title=&width=425.6)

vs打开codeql-codeql-cli的项目，在合适的位置新建个sql<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673765083427-e1c5ac94-94e5-487a-b3bc-fd632a6af0ed.png#averageHue=%2325292c&clientId=udd827dc5-6be8-4&from=paste&height=557&id=u2e8467aa&originHeight=531&originWidth=357&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25660&status=done&style=none&taskId=u25ea2a8d-911d-4134-9fc4-7562e4a29fd&title=&width=374.6000061035156)<br />写好规则后就可以查询了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673765107588-bd25042d-70f9-4da4-903d-4168d72073d8.png#averageHue=%2327282b&clientId=udd827dc5-6be8-4&from=paste&height=126&id=uda34e5a5&originHeight=110&originWidth=417&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6824&status=done&style=none&taskId=u3cdb7f7f-077e-4348-84f8-91406c7a3a4&title=&width=477.6000061035156)

<a name="dqsYQ"></a>
## ql规则路径
D:\codeql\codeql-codeql-cli-v2.12.0\codeql-codeql-cli-v2.12.0\java\ql\consistency-queries


<a name="sMCPs"></a>
## codeQlpy
[https://github.com/webraybtl/codeQlpy](https://github.com/webraybtl/codeQlpy)<br />D:\ql\CodeQLpy-master\CodeQLpy-master<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673799388536-1e2769f6-df28-437b-852d-96ee04c0d75a.png#averageHue=%23032759&clientId=u8d74e05e-1ad9-4&from=paste&height=616&id=u7cfebbf1&originHeight=770&originWidth=915&originalType=binary&ratio=1&rotation=0&showTitle=false&size=85650&status=done&style=none&taskId=ue234cc31-c7d7-49c9-a8e4-52b5962f20c&title=&width=732)
<a name="kVcBQ"></a>
## 注意事项：
1、vscode打开了ql的数据库，这个时候就不能同时用，不然会被占用<br />2、![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673799472678-6182c438-be16-4407-ba44-289f8909e0ca.png#averageHue=%23fefdfc&clientId=u8d74e05e-1ad9-4&from=paste&height=312&id=u752b68d9&originHeight=390&originWidth=973&originalType=binary&ratio=1&rotation=0&showTitle=false&size=39936&status=done&style=none&taskId=u6a94e533-f236-4149-8f7d-da5fcf84abb&title=&width=778.4)


3、稍微改了下规则，提高了点效率。<br />D:\ql\CodeQLpy-master\CodeQLpy-master\plugins\java

4、出现这种情况，需要安装jdk11才可以<br />![852ec05ffe93d4c1d4c673ec4a0d08c.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673845282083-2f71549b-059c-4bd6-b417-45f0eeca37e2.png#averageHue=%23121110&clientId=u8d74e05e-1ad9-4&from=paste&height=246&id=u0ce1486d&originHeight=307&originWidth=932&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24659&status=done&style=none&taskId=u4df2adc6-2d5c-40b0-98e1-802303beb83&title=&width=745.6)

<a name="kC19B"></a>
## 使用方法
D:\codeql\CodeQLpy-master2\CodeQLpy-master>python main.py -t E:\java\amis.war -c<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1673845388570-f8e5a23a-8cfd-4c22-bd0d-f5ee80907505.png#averageHue=%2311100e&clientId=u8d74e05e-1ad9-4&from=paste&height=335&id=u18808ad4&originHeight=419&originWidth=810&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37454&status=done&style=none&taskId=u1d4b9d62-01bc-4fd5-a6f5-99717e115b4&title=&width=648)

python main.py -d D:\codeql\CodeQLpy-master2\CodeQLpy-master\out\database\amis.war
