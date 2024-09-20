Java SpringCloud Nacos
<a name="KejCc"></a>
## NACOS配置中心获取不到配置内容
由于bootstrap.yml文件需要bootstrap相关的jar包，需要加入bootstrap的依赖。然后再启动服务，就成功了。
```xml
<dependency>
  <groupId>org.springframework.cloud</groupId>
  <artifactId>spring-cloud-starter-bootstrap</artifactId>
</dependency>
```
<a name="Y25oa"></a>
## 版本不匹配会导致的问题
| JDK | SpringBoot | SpringCloud | SpringCloudAlibaba | 问题描述 | 问题截图 |
| --- | --- | --- | --- | --- | --- |
| 17 | 2.4.12 | 2020.0.4 | 2.2.5.RELEASE | 一直打印配置变更 | ![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636169566777-f87c9d8a-f4c7-4111-882f-66fe146e3b45.png#clientId=u58b55784-38e2-4&from=paste&height=680&id=ub3e04832&originHeight=2040&originWidth=3840&originalType=binary&ratio=1&size=941607&status=done&style=none&taskId=u0f11f813-9f72-4331-bf9e-4edfc377a88&width=1280) |
|  |  |  |  |  |  |

