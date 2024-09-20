IDEA
<a name="HtySh"></a>
## 问题描述
IDEA运行Java类显示命令行太长
```
Error running 'TrainGatewayApplication': 
Command line is too long. Shorten command line for TrainGatewayApplication or also for 
Spring Boot default configuration.
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575976873893-331a679c-74df-40b6-861f-28716f35f334.png#averageHue=%23f8f6f4&height=728&id=AQP2k&originHeight=728&originWidth=1366&originalType=binary&ratio=1&rotation=0&showTitle=false&size=106893&status=done&style=shadow&title=&width=1366)
<a name="nN3lW"></a>
## 解决办法
<a name="jWkLF"></a>
### 1、修改项目工程的配置文件（建议使用）
:::success
建议使用：项目工程的配置文件只需要修改一次，后面运行其他测试用例都不会出现上面的报错。
:::
打开工程目录下的.idea文件夹下的workspace.xml文件，找到`<component name="PropertiesComponent">`标签，在最后一行添加属性
```xml
<property name="dynamic.classpath" value="true" />
```
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575978213065-9b2047d6-5802-4756-868f-b15dc3cc4e87.png#averageHue=%23fdf8f7&height=589&id=bUEBc&originHeight=589&originWidth=1341&originalType=binary&ratio=1&rotation=0&showTitle=false&size=147152&status=done&style=shadow&title=&width=1341)
<a name="NVLme"></a>
### 2、修改运行配置
> 不建议：下次运行新的配置则需要再次进行配置。

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651125355094-7414fd66-e59a-4d2b-aefd-26d9dcfe073d.png#averageHue=%23f3f2f2&clientId=u8544b2c3-34e7-4&from=paste&height=1117&id=ZD9gO&originHeight=1676&originWidth=2605&originalType=binary&ratio=1&rotation=0&showTitle=false&size=239849&status=done&style=shadow&taskId=u4ff6bd26-3e85-48b7-b435-7737c017393&title=&width=1736.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1651125370358-ef62e7db-72ea-4290-becc-7e13c336042d.png#averageHue=%23f3f3f2&clientId=u8544b2c3-34e7-4&from=paste&height=1117&id=zKfVE&originHeight=1676&originWidth=2605&originalType=binary&ratio=1&rotation=0&showTitle=false&size=200173&status=done&style=shadow&taskId=u3c6a1299-5ac3-4461-aff1-2cf6751297f&title=&width=1736.6666666666667)
