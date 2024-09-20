参考：[https://docs.spring.io/spring-boot/docs/current/reference/html/deployment.html#deployment](https://docs.spring.io/spring-boot/docs/current/reference/html/deployment.html#deployment)<br />Spring Boot 的灵活打包选项在部署应用程序时提供了大量选择。可以将 Spring Boot 应用程序部署到各种云平台、容器映像（例如 Docker）或虚拟/真实机器上。
<a name="Fnbjp"></a>
## 1、解压jar包进行部署运行
如果从容器运行应用程序，则可以使用可执行 jar，但将其分解并以不同方式运行它通常也是一个优势。某些 PaaS 实现也可能选择在运行之前解压缩存档。例如，Cloud Foundry 就是这样运作的。运行解压存档的一种方法是启动适当的启动器，如下所示：
```bash
$ jar -xf myapp.jar
$ java org.springframework.boot.loader.JarLauncher
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622616703364-57e0d1b7-dbf5-49d2-b2f2-9687104bd559.png#clientId=ud845c6ff-8fb5-4&from=paste&height=628&id=uf87f690c&originHeight=1884&originWidth=3840&originalType=binary&size=2482315&status=done&style=none&taskId=u321266ff-6d57-4f6e-9fd4-1ded4034a2d&width=1280)<br />然后通过命令成功启动项目<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622616771998-f0251dee-1117-4355-b8eb-f0166c83b2c6.png#clientId=ud845c6ff-8fb5-4&from=paste&height=687&id=uc7bac5db&originHeight=2060&originWidth=3840&originalType=binary&size=2862378&status=done&style=shadow&taskId=u1a0abaeb-b080-4bd2-89a0-dbc4be07aa6&width=1280)
:::success
这实际上在启动时（取决于 jar 的大小）比从解压的存档中运行要快一些。<br />使用JarLauncher应用程序的主要方法具有可预测的类路径顺序的额外好处。jar 包含一个classpath.idx文件，该文件JarLauncher在构建类路径时使用。<br />classpath.idx文件内容如下：![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622617364278-a3f2eb29-ad66-47b4-8850-704c3215c90f.png#clientId=ud845c6ff-8fb5-4&from=paste&height=596&id=u7585adbf&originHeight=1789&originWidth=2103&originalType=binary&size=305659&status=done&style=none&taskId=u9b2ce7e9-9596-4d63-b3f0-e70c6a94b4f&width=701)
:::
<a name="kDUj7"></a>
### 总结
这样在部署项目时，只需要第一次在服务器上传jar包然后进行解压存档文件，之后只要没有依赖jar包的变化，只有业务的改动则只需要替换`./BOOT-INF/classes`文件即可。这样可以快速部署，如果有依赖的变化，则需要重新上传打包后的jar包进行配置信息的替换。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622617126308-f1f329cd-fdce-44aa-8f59-cbfdf6611526.png#clientId=ud845c6ff-8fb5-4&from=paste&height=503&id=pZs6y&originHeight=1508&originWidth=2132&originalType=binary&size=301451&status=done&style=none&taskId=u8a5b604d-93ba-4e43-a786-20019910d24&width=710.6666666666666)
