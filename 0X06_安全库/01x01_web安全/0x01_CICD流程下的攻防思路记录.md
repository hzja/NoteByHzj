## 看过的一些文章记录
[https://mp.weixin.qq.com/s/36t7PuV8OD_clyFUR-M0IQ](https://mp.weixin.qq.com/s/36t7PuV8OD_clyFUR-M0IQ)

[https://github.com/cider-security-research/cicd-goat.git](https://github.com/cider-security-research/cicd-goat.git) CICD靶场

[https://github.com/rung/threat-matrix-cicd](https://github.com/rung/threat-matrix-cicd)

## 个人理解(CICD与SDL)
基本上互联网甲方都会有cicd流程，也就是devops的自动化流程(dev开发、ops运维)，我理解的话，其实就是把研发编码构建阶段和运维的部署检查监控阶段通过自动化结合起来。

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693150917980-4f0f0d62-2f94-4063-8fe7-1826a1b31e99.png#averageHue=%23fafaf9&clientId=ua5bfa931-e5dd-4&from=paste&height=411&id=u4dfb7420&originHeight=514&originWidth=1234&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=26570&status=done&style=none&taskId=u9b675830-4e7c-4093-b849-39417524b9c&title=&width=987.2)<br />CI流程：代码集成→代码静态检查→自动化测试→自动化打包构建→自动化镜像构建 ，（研发阶段）<br />CD流程：自动部署→持续监控，(运维阶段)<br />**那么其实SDL应用安全建设，其实就是把安全融入研发运维的CICD中，把安全融入产品的生命周期**

<a name="tKMW4"></a>

## 工具总结
知乎工具流程图([https://zhuanlan.zhihu.com/p/420442692?utm_id=0](https://zhuanlan.zhihu.com/p/420442692?utm_id=0))<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693152295804-04440087-396b-44c5-ab38-304d227fc144.png#averageHue=%23dbe7df&clientId=ua5bfa931-e5dd-4&from=paste&height=562&id=ud5935550&originHeight=703&originWidth=1295&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=431341&status=done&style=none&taskId=uf857c1e3-fbe5-4d13-80a1-f909ac156c6&title=&width=1036)<br />工具总结：
```
CI：静态代码检查阶段
GitHub，GitLab，SVM，BitBucket
SonarQube，Veracode，Appscan等SAST工具从内部检查代码

CI：构建
Jenkins，Bamboo CI，Circle CI，Travis CI，Maven，Azure DevOps

CI：测试阶段
技术：Selenium，Appium，Jmeter，SOAP UI，Tarantula

CD：部署、验证、监控
Zabbix、Nagios、Prometheus、Elastic Search、Splunk、Appdynamics、Tivoli

CD：反馈和协作工具
技术：JIRA、ServiceNow、Slack、电子邮件、Hipchat。
```

<a name="rCs4D"></a>

## kcon相关资料总结
[CICD攻击场景.pdf](https://www.yuque.com/attachments/yuque/0/2023/pdf/1345801/1693153249890-b7edcd27-68a0-4bc3-bb10-c629c95d4701.pdf?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2023%2Fpdf%2F1345801%2F1693153249890-b7edcd27-68a0-4bc3-bb10-c629c95d4701.pdf%22%2C%22name%22%3A%22CICD%E6%94%BB%E5%87%BB%E5%9C%BA%E6%99%AF.pdf%22%2C%22size%22%3A10440831%2C%22ext%22%3A%22pdf%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22u98785d43-6650-402e-93f7-ca01c85a802%22%2C%22taskType%22%3A%22upload%22%2C%22type%22%3A%22application%2Fpdf%22%2C%22__spacing%22%3A%22both%22%2C%22mode%22%3A%22title%22%2C%22id%22%3A%22ucdc8111e%22%2C%22margin%22%3A%7B%22top%22%3Atrue%2C%22bottom%22%3Atrue%7D%2C%22card%22%3A%22file%22%7D)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693153279829-2d421593-e923-45dd-b989-c46af1d47d50.png#averageHue=%23837f7f&clientId=ua5bfa931-e5dd-4&from=paste&id=u203ef566&originHeight=615&originWidth=1579&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=110412&status=done&style=none&taskId=ubeb0317b-54c8-451e-8bf8-efe4a6a5ae2&title=)

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693410915987-8786d7a0-7b70-4368-bec5-a1f9aeb1bccd.png#averageHue=%23dcd6bb&clientId=ua5bfa931-e5dd-4&from=paste&height=653&id=uf09421de&originHeight=816&originWidth=1376&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=1116422&status=done&style=none&taskId=u0f6e8fe8-7b0b-42ff-96ea-b0271d8d416&title=&width=1100.8)


<a name="oOGHC"></a>
## CI构建时的攻击思路
<a name="dKgM4"></a>
### 利用Dockerfile进行命令执行
有时候甲方会有一整套自动化的cicd流程平台，在单独的构建功能点里面或许可以编辑Dockerfile文件，这个时候我们可以尝试通过利用run参数去进行命令执行，从而达到流水线生产镜像时将恶意的命令执行构建到镜像中。

- 首先得知道已有的docker镜像

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693755970609-086d635e-10f2-4dca-a487-9eb4e321ef5b.png#averageHue=%23272b36&clientId=uf23c376e-7f4b-4&from=paste&height=102&id=u1a510d57&originHeight=128&originWidth=654&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=39030&status=done&style=none&taskId=u66a88bc6-1730-4161-b616-259246e5e7a&title=&width=523.2)

- 构建Dokcerfile
```
FROM cidersecurity/goat-gitea
RUN echo "aufeng" >>aufeng.txt
```
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756472606-a182c5c1-0677-406c-8144-75cf7078302c.png#averageHue=%23252933&clientId=uf23c376e-7f4b-4&from=paste&height=86&id=uc5f089ee&originHeight=108&originWidth=415&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=13291&status=done&style=none&taskId=u0fd2d621-b061-4252-bfde-ae0abdef751&title=&width=332)

- 镜像构建

docker build -t aufeng .<br />-t后面是新构建的镜像名称，点号是指该目录下的文件，build构建时会自动寻找Dockerfile进行构建<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756608022-f377b41d-f2ec-4db1-b760-e8270532f706.png#averageHue=%23252934&clientId=uf23c376e-7f4b-4&from=paste&height=300&id=ua3ebdfca&originHeight=375&originWidth=720&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=93751&status=done&style=none&taskId=ufffc29b4-a16e-470c-878a-d333594ef9f&title=&width=576)

- 进入container查看

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756670917-d42d7577-b71f-4f85-b987-5208951b8120.png#averageHue=%23262b36&clientId=uf23c376e-7f4b-4&from=paste&height=59&id=u94d360e4&originHeight=74&originWidth=579&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=18402&status=done&style=none&taskId=u6c022442-0769-47d9-b668-761bf865025&title=&width=463.2)<br />可以发现Dockerfile构建时run执行的命令已经成功执行<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693756769878-fb919752-bcfb-4c1d-9d93-b396a0bc9215.png#averageHue=%23242834&clientId=uf23c376e-7f4b-4&from=paste&height=156&id=u2f7d3ce4&originHeight=195&originWidth=750&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=50899&status=done&style=none&taskId=u5580e84b-d579-4500-b6be-49dd631ed43&title=&width=600)

<a name="XwBYC"></a>
### gitlab

<a name="CC5YL"></a>
### Jenkins权限控制不当泄露大量密钥
jenkins在cicd流程中的角色十分重要，有的互联网甲方Jenkins版本范围大概是2.26-2.46，不需要登陆也能观察到很多信息。<br />举个例子(野外靶场)：不需要登陆的情况下 可以看到多个项目<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693841421289-55b29e3b-c90c-4284-a861-b9b0312a21c5.png#averageHue=%23e8e8e7&clientId=u398e3d17-e5c2-4&from=paste&height=736&id=u829d0a7d&originHeight=920&originWidth=1884&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=160432&status=done&style=none&taskId=ua349a12a-a67b-422a-89f8-2ed0bd447b0&title=&width=1507.2)

进入项目查看console的log，或者Parameters等等地方，获取到泄露的路径，这些路径很多时候都是未授权就可以下载jar包或者压缩包，从中获取配置文件的密码。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693841661666-a4b362dd-8161-4140-bee1-d139afc56255.png#averageHue=%238b8a8a&clientId=u398e3d17-e5c2-4&from=paste&height=726&id=u43ee7289&originHeight=908&originWidth=1524&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=101230&status=done&style=none&taskId=u594fcf38-14e9-47e1-84f3-db8edd1ca9e&title=&width=1219.2)

如下图所示，通过Jenkins控制台等等其他地方获取到代码构建nexus的地址，并且可以未授权直接下载，攻击者从jar包反编译中获取账号密码。

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693841705540-5b0f86a6-e8d7-4cc0-a5b2-b90e39b3a3d8.png#averageHue=%23726f6f&clientId=u398e3d17-e5c2-4&from=paste&height=580&id=u1bee3883&originHeight=725&originWidth=1878&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=44519&status=done&style=none&taskId=u8bb1048c-e664-4f71-859c-28e61d81ab6&title=&width=1502.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1693841746544-092ba3fb-afde-4662-8132-9672fbe7bceb.png#averageHue=%2343424e&clientId=u398e3d17-e5c2-4&from=paste&height=179&id=u11b34065&originHeight=224&originWidth=724&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=18524&status=done&style=none&taskId=u0b29cdf5-fb8b-403c-8607-755a9017ccf&title=&width=579.2)

<a name="Lhnbz"></a>
### Jenkins解密凭证获取上下游系统账号密码
实战情况下，Jenkins权限控制不当通过控制台获取nexus构建镜像的jar地址后未授权下载，反编译获取到Jenkins后台的密码，这个时候可以利用Groovy script语法去dump获取账号密码(很可能会有上下游关联系统的账密，如sast、harbor等等)
```
com.cloudbees.plugins.credentials.SystemCredentialsProvider.getInstance().getCredentials().forEach{
  it.properties.each { prop, val ->
    println(prop + ' = "' + val + '"')
  }
  println("-----------------------")
}
```

Jenkins凭证解密、重置密码等等方法以下博客链接均有详细教程，不多做记录<br />[https://blog.csdn.net/qq_26545503/article/details/125194860](https://blog.csdn.net/qq_26545503/article/details/125194860)

<a name="Hb18U"></a>
### harbor投毒
harbor<br />[https://baijiahao.baidu.com/s?id=1774827837981108970&wfr=spider&for=pc](https://baijiahao.baidu.com/s?id=1774827837981108970&wfr=spider&for=pc)

<a name="LzjRS"></a>
### Jmeter
CI测试阶段，有时候会从代码审计上发现会有研发同事为了方便让Jmeter进行压测，会在后端代码里写了exec去执行Jmeter

<a name="f5MR9"></a>
### nexus投毒/Nday
[https://www.cnblogs.com/justuntil/p/10093090.html](https://www.cnblogs.com/justuntil/p/10093090.html)   nexus搭建使用<br />[https://www.freebuf.com/vuls/374880.html](https://www.freebuf.com/vuls/374880.html) nday利用<br />[http://www.taodudu.cc/news/show-6140334.html?action=onClick](http://www.taodudu.cc/news/show-6140334.html?action=onClick) 上传依赖
<a name="J1wGV"></a>
## CD攻击思路
<a name="TfTkD"></a>
### 运维阶段
运维平台配置中心泄露整个公司所以paas应用ak sk
