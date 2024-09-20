IDEA
<a name="PcgcO"></a>
## 一、JRebel+XRebel简介
IDEA上原生是不支持热部署的，当修改代码和配置文件等大部分操作时，都需要重启服务器。<br />JRebel是一款JAVA虚拟机插件，它使得JAVA程序员能在不进行重部署的情况下，即时看到代码的改变对一个应用程序带来的影响。JRebel使你能即时分别看到代码、类和资源的变化，从而跳过了构建和部署的过程，可以省去大量的部署用的时间。<br />目前对于idea热部署最好的解决方案就是安装JRebel。<br />XRebel 是不间断运行在 web 应用的交互式分析器，当发现问题会在浏览器中显示警告信息。XRebel 会实时监测应用代码的性能指标和可能会发生的问题。<br />官方文档：

- [https://manuals.jrebel.com/jrebel/index.html](https://manuals.jrebel.com/jrebel/index.html)
<a name="fcIyY"></a>
## 二、安装JRebel/XRebel
<a name="sbPxw"></a>
### 1、通过IDEA插件仓库查询JRebel即可
**这俩个插件是绑定在一起下载的，如图。**<br />![2023-02-11-17-07-32.478686300.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106544862-9c069723-8438-49ba-b8da-cd5b339a4e4b.png#averageHue=%23282b40&clientId=ueba445f3-05b7-4&from=ui&id=u3f046b83&originHeight=917&originWidth=1863&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=189057&status=done&style=none&taskId=u3370424e-9eba-4f39-bc02-b8fa9663bb4&title=)
<a name="xYHfB"></a>
### 2、由于该插件为收费，需要对插件进行激活

- 依次进入help->Jrebel->Activation
- 服务器地址：https://jrebel.qekang.com/{GUID}
- 生成GUID：在线GUID地址
- 将服务器地址与GUID拼接后填入Team URL
- 点击CHANGE LICENSE

到此，JRebel就激活完成了。<br />![2023-02-11-17-07-32.710190500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106545069-c405c66c-f3e5-4fcd-ac15-53740e5f3acf.png#averageHue=%232f3141&clientId=ueba445f3-05b7-4&from=ui&id=wNVqS&originHeight=1277&originWidth=1336&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=662095&status=done&style=none&taskId=u372ddd93-628e-4da4-8cd4-5ca4fb431d3&title=)<br />![2023-02-11-17-07-33.507732400.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106544775-4bd7d2d0-24fd-49be-a88f-f8a857bc4769.png#averageHue=%2325283b&clientId=ueba445f3-05b7-4&from=ui&id=TDB1z&originHeight=888&originWidth=889&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=58543&status=done&style=none&taskId=u8ba2073b-da4b-4eae-8282-d3bacb73c2f&title=)
<a name="aNeyE"></a>
## 三、设置JRebel
<a name="q9Bmu"></a>
### 1、设置为离线工作模式，点击WORK OFFLICE
![2023-02-11-17-07-33.649570500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106588413-73af0ad9-fd68-4f39-9be9-86debf57502d.png#averageHue=%2325283b&clientId=ueba445f3-05b7-4&from=ui&id=FlHUL&originHeight=1108&originWidth=1851&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=96478&status=done&style=none&taskId=u16083f10-0c0d-487d-ba6a-cd206f7262a&title=)<br />也可更改JRebel的自动刷新间隔
<a name="INUrU"></a>
### 2、为IDEA设置自动编译（一般默认就是自动编译）
![2023-02-11-17-07-33.827131000.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106588402-711b2a83-5621-4a0d-9134-4a26cbd782ba.png#averageHue=%23282b40&clientId=ueba445f3-05b7-4&from=ui&id=uf7b3ddf1&originHeight=503&originWidth=1211&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=55479&status=done&style=none&taskId=u5a0dad94-61c8-4eef-9f72-dfdce908d19&title=)<br />在advanced setting勾选Allow auto -make…<br />![2023-02-11-17-07-34.045914400.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106616865-22ac2ca6-6e8b-4a58-abca-e48f18922d2f.png#averageHue=%23272a3e&clientId=ueba445f3-05b7-4&from=ui&id=ue02dbc22&originHeight=483&originWidth=1347&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=46509&status=done&style=none&taskId=u04cb9ab0-70f2-4e49-99b5-8266659b5cf&title=)
<a name="KhTev"></a>
## 四、对项目进行热部署
<a name="YpfVI"></a>
### 1、打开下面的JRebel，选择需要进行热部署的服务
![2023-02-11-17-07-34.219987800.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106617005-74315cae-22de-4f81-9d05-7fe3fa492621.png#averageHue=%233d414e&clientId=ueba445f3-05b7-4&from=ui&id=lu2Sl&originHeight=528&originWidth=2239&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=334027&status=done&style=none&taskId=u2d9bc849-acd2-45bd-81c6-4893c1cc25a&title=)
<a name="eRFtg"></a>
### 2、在SpringBoot项目中，选择更新类和资源
![2023-02-11-17-07-34.529555200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106616882-49494d7e-f03b-4ecd-8ecb-cc466c6b33cf.png#averageHue=%2325283b&clientId=ueba445f3-05b7-4&from=ui&id=aFrLl&originHeight=865&originWidth=1368&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=61035&status=done&style=none&taskId=ucac4a5cc-3e00-4dcf-83c1-bad70ea3ac0&title=)
<a name="pVSHu"></a>
### 3、使用JRebel+XRebel(若仅需要热部署，可选择JRebel)启动项目，而不是原生启动
![2023-02-11-17-07-34.731305500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106637870-8f924040-60f9-4783-a32f-aae395d39d49.png#averageHue=%23232537&clientId=ueba445f3-05b7-4&from=ui&id=uff0c54e4&originHeight=331&originWidth=1289&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=137678&status=done&style=none&taskId=u56edcfbc-b6ec-4807-8aae-0c6aaf7b3c1&title=)
<a name="wPWj9"></a>
### 4、当本地有多个微服务时，在service中选择所有的微服务，并使用JRebel启动
![2023-02-11-17-07-34.979731900.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106637948-978c0099-c3ac-435e-97a5-d7c2171cbc50.png#averageHue=%23454757&clientId=ueba445f3-05b7-4&from=ui&id=d8jK2&originHeight=478&originWidth=1047&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=270005&status=done&style=none&taskId=ub820540e-84a1-4473-9987-d6564bba09b&title=)<br />这样，当任何一个服务中的代码改变时，直接Ctrl+Shift+F9，JRebel将会监测到代码改变并且Reload，可以继续愉快地继续开发了，避免了重新启动服务器，等待几十秒的时间。<br />![2023-02-11-17-07-35.247078700.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106669189-0dc9c7a5-eec6-488a-99b4-e8cf96d7132d.png#averageHue=%233b3c4a&clientId=ueba445f3-05b7-4&from=ui&id=u2eabdb45&originHeight=508&originWidth=1389&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=294446&status=done&style=none&taskId=ue1b2e221-40f4-43ad-a9bd-5677903b1da&title=)
<a name="K2Ld9"></a>
## 五、使用XRebel性能分析
<a name="AuAxx"></a>
### 1、XRebel访问
项目启动后访问地址为：服务器项目应用地址/xrebel<br />例如：http://localhost:8080/xrebel
<a name="i8ixh"></a>
### 2、激活XRebel
激活方式同JRebel
<a name="thK3D"></a>
### 3、功能
![2023-02-11-17-07-35.652784200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106669031-13432e68-2b14-4929-9d17-4194008b0deb.png#averageHue=%233d4048&clientId=ueba445f3-05b7-4&from=ui&id=F2OXc&originHeight=513&originWidth=213&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=21083&status=done&style=none&taskId=u08b62814-7386-468a-84a9-59c27f0407a&title=)

1. 能够捕捉到应用运行时发生的异常
2. 能够详细的观察某每一次的调用，而且能够非常详细的观察各个步骤的执行时间
3. 每个步骤还可以看到详细的源码执行流程
4. 在数据库操作上能够看到具体的耗时和格式化后的SQL语句
5. 可以查看详细的系统执行日志，可以下载到本地
6. 不仅支持单机模式下，还可以应用于微服务分布式

![2023-02-11-17-07-35.754472800.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106703720-c4677982-862e-4415-8e82-f4886b340c2b.png#averageHue=%23fafafa&clientId=ueba445f3-05b7-4&from=ui&id=ucdab37dc&originHeight=931&originWidth=1564&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=194917&status=done&style=none&taskId=ua290184d-5943-4399-ba9c-c2f0b0c102f&title=)![2023-02-11-17-07-36.063486300.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1676106703719-caba0d0e-7bc8-4791-853b-edf7c81527e0.png#averageHue=%23fdfcfc&clientId=ueba445f3-05b7-4&from=ui&id=u3f2e943a&originHeight=945&originWidth=1229&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=190942&status=done&style=none&taskId=uae49fc3a-f311-4c19-9722-4198aec30dd&title=)
