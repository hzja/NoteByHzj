Java SpringCloud 
<a name="SQ4h1"></a>
## 1、在配置中心配置变量值进行引用
<a name="lETGg"></a>
### 本地boostrap.yml配置示例
```yaml
spring:
  cloud:
    nacos:
      username: nacos
      password: nacos
      config:
        server-addr: 127.0.0.1:8848
        namespace: public
        file-extension: yml
        refresh-enabled: true
        shared-configs:
          - dataId: ${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}
            refresh: false
          - dataId: common.${spring.cloud.nacos.config.file-extension}
            refresh: false
```
<a name="yBDTR"></a>
### 配置中心引用配置值示例
```yaml
default:
  name: 10.0.1
info:
  name: ${default.name}
```
<a name="NCYQG"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636620202445-68ee44f6-44f0-4ea0-bf93-4a79dac61d85.png#clientId=uc7896918-f928-4&from=paste&height=259&id=u6b5fc62f&originHeight=776&originWidth=1432&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136187&status=done&style=shadow&taskId=u9fcf371e-5421-4fba-bf68-01381685bb0&title=&width=477.3333333333333)
<a name="dkG78"></a>
## 2、在本地bootstrap.yml配置值，在配置中心进行引用
<a name="aUiy1"></a>
### 本地boostrap.yml配置示例
```yaml
spring:
  cloud:
    nacos:
      username: nacos
      password: nacos
      config:
        server-addr: 127.0.0.1:8848
        namespace: public
        file-extension: yml
        refresh-enabled: true
        shared-configs:
          - dataId: ${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}
            refresh: false
          - dataId: common.${spring.cloud.nacos.config.file-extension}
            refresh: false
default:
  name: 10.9.1
```
<a name="fZRTV"></a>
### 配置中心引用配置值示例
```yaml
info:
  name: ${default.name}
```
<a name="ZCqYD"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636620124763-3caf1df2-0c88-4f66-bca5-da1e96c606e2.png#clientId=uc7896918-f928-4&from=paste&height=249&id=u05969f63&originHeight=748&originWidth=1373&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136089&status=done&style=shadow&taskId=u42d1f5ab-9310-4b08-81bb-f364ed2a16b&title=&width=457.6666666666667)<br />成功拿到配置值。
<a name="ofSqC"></a>
## 3、在命令行参数配置默认值，配置文件设置默认值，在配置中心进行引用
<a name="MgIz5"></a>
### 程序启动参数配置
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636561243366-3aba731b-fbaf-475e-9479-2a82a4e7ac59.png#clientId=uf38a40af-3b06-4&from=paste&height=585&id=u0b7b8b87&originHeight=1756&originWidth=2770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=303045&status=done&style=shadow&taskId=u3982d171-9146-456b-ba22-c6ec3d5ad2c&title=&width=923.3333333333334)
<a name="ZWzIp"></a>
### 在bootstrap.yml配置占位符默认值
```yaml
spring:
  cloud:
    nacos:
      username: nacos
      password: nacos
      config:
        server-addr: 127.0.0.1:8848
        namespace: public
        file-extension: yml
        refresh-enabled: true
        shared-configs:
          - dataId: ${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}
            refresh: false
          - dataId: common.${spring.cloud.nacos.config.file-extension}
            refresh: false
default:
  name: 10.9.1
```
<a name="PbDE1"></a>
### 配置中心应用值示例
```yaml
info:
  name: ${default.name}
```
<a name="FMz2F"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636620138939-13df5521-c677-4086-88ba-8bc63eba7b6b.png#clientId=uc7896918-f928-4&from=paste&height=256&id=ude7c8f53&originHeight=767&originWidth=1509&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138435&status=done&style=shadow&taskId=u96cca147-e7c0-402d-bfbb-6a17ff2c3a5&title=&width=503)
<a name="wwbm4"></a>
## 4、在命令行参数配置默认值，配置文件不设置默认值，在配置中心进行引用
<a name="IIwgG"></a>
### 程序启动参数配置
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636561243366-3aba731b-fbaf-475e-9479-2a82a4e7ac59.png#clientId=uf38a40af-3b06-4&from=paste&height=585&id=xCSng&originHeight=1756&originWidth=2770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=303045&status=done&style=shadow&taskId=u3982d171-9146-456b-ba22-c6ec3d5ad2c&title=&width=923.3333333333334)
<a name="ZqOK5"></a>
### 在bootstrap.yml配置占位符默认值
```yaml
spring:
  cloud:
    nacos:
      username: nacos
      password: nacos
      config:
        server-addr: 127.0.0.1:8848
        namespace: public
        file-extension: yml
        refresh-enabled: true
        shared-configs:
          - dataId: ${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}
            refresh: false
          - dataId: common.${spring.cloud.nacos.config.file-extension}
            refresh: false
```
<a name="C7Z3l"></a>
### 配置中心应用值示例
```yaml
info:
  name: ${default.name}
```
<a name="Fb3KJ"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636620138939-13df5521-c677-4086-88ba-8bc63eba7b6b.png#clientId=uc7896918-f928-4&from=paste&height=256&id=PLkh2&originHeight=767&originWidth=1509&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138435&status=done&style=shadow&taskId=u96cca147-e7c0-402d-bfbb-6a17ff2c3a5&title=&width=503)
<a name="xuQIx"></a>
## 5、在命令行参数不配置值，在变量配置默认值
<a name="TwPoJ"></a>
### 配置示例
```yaml
info:
  name: ${default.name:Fc}
  age: 18
fcant:
  name: fc hello
  age: 11
```
<a name="faoeP"></a>
### 结果
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636907005784-2625e6fe-4af4-4f80-9264-ec2ee2d6a3dc.png#clientId=u4b94e3eb-4288-4&from=paste&height=253&id=u6aeb2f34&originHeight=759&originWidth=1597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=146443&status=done&style=shadow&taskId=uc1c27858-6122-4f7d-b3f6-2a541ad714c&title=&width=532.3333333333334)
