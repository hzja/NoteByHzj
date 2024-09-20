Java SpringBoot
<a name="p3z3N"></a>
### 1、在项目中引入SpringBoot Actuator依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-actuator</artifactId>
</dependency>
```
<a name="66fee537"></a>
### 2、日志打印显示暴露的端点
```
2019-12-07 16:59:00.362  INFO 17196 --- [  restartedMain] o.s.b.a.e.web.EndpointLinksResolver      : Exposing 2 endpoint(s) beneath base path '/actuator'
```
<a name="83730ae8"></a>
### 3、输入项目的主机IP和端口以及日志的路径进行访问
[http://localhost:8080/actuator](http://localhost:8080/actuator)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575734468877-d80d88e3-b1c5-4e5a-b8d9-e139420d09b8.png#align=left&display=inline&height=471&originHeight=471&originWidth=1366&size=55021&status=done&style=none&width=1366)![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575734496660-64d22d94-3309-4b7a-9cb1-99d9a10eb041.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=96475&status=done&style=none&width=1366)

[http://localhost:8080/actuator/health](http://localhost:8080/actuator/health)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575775045903-f34336c8-0311-497b-8b8b-e0c0266061e2.png#align=left&display=inline&height=326&originHeight=326&originWidth=498&size=21611&status=done&style=none&width=498)
<a name="9yGK9"></a>
### 4、在运行日志中可以看到本次进程的PID
```
2019-12-08 10:53:37.638  INFO 14160 --- [  restartedMain] c.f.s.SpringBootExpandApplication        : Starting SpringBootExpandApplication on LAPTOP-CMGC7APE with PID 14160 (D:\Dome\SpringBootHD\SpringBootExpand\target\classes started by Fcant in D:\Dome\SpringBootHD)
```
<a name="MLiMZ"></a>
### 5、按下Win+R打开运行，输入命令jconsole
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575774678229-60351387-af87-4b16-87a3-0384d06d2783.png#align=left&display=inline&height=230&originHeight=230&originWidth=399&size=14437&status=done&style=none&width=399)
<a name="7mWHq"></a>
### 6、查看Java进程的相关信息

<a name="8289952f"></a>
### 7、在配置文件配置暴露更多的信息

```yaml
management:
  endpoints:
    web:
      exposure:
        include: '*'
```
<a name="77a6bc6b"></a>
### 8、再次访问/actuator显示了更多的信息
日志中显示暴露了13个端点
```
2019-12-08 11:20:37.149  INFO 14160 --- [  restartedMain] o.s.b.a.e.web.EndpointLinksResolver      : Exposing 13 endpoint(s) beneath base path '/actuator'
```
再次访问URL<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575775575788-c5223a85-22e7-43c4-8677-4a2e0fd7704c.png#align=left&display=inline&height=725&originHeight=725&originWidth=1366&size=88543&status=done&style=none&width=1366)![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575775605079-d86b5262-ebf1-408b-9b7c-d8d8cea8ea4d.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=85502&status=done&style=none&width=1366)

```json
{
    "_links": {
        "self": {
            "href": "http://localhost:8080/actuator",
            "templated": false
        },
        "beans": {
            "href": "http://localhost:8080/actuator/beans",
            "templated": false
        },
        "caches-cache": {
            "href": "http://localhost:8080/actuator/caches/{cache}",
            "templated": true
        },
        "caches": {
            "href": "http://localhost:8080/actuator/caches",
            "templated": false
        },
        "health": {
            "href": "http://localhost:8080/actuator/health",
            "templated": false
        },
        "health-path": {
            "href": "http://localhost:8080/actuator/health/{*path}",
            "templated": true
        },
        "info": {
            "href": "http://localhost:8080/actuator/info",
            "templated": false
        },
        "conditions": {
            "href": "http://localhost:8080/actuator/conditions",
            "templated": false		
        },
        "configprops": {
            "href": "http://localhost:8080/actuator/configprops",
            "templated": false
        },
        "env": {
            "href": "http://localhost:8080/actuator/env",
            "templated": false
        },
        "env-toMatch": {
            "href": "http://localhost:8080/actuator/env/{toMatch}",
            "templated": true
        },
        "loggers": {
            "href": "http://localhost:8080/actuator/loggers",
            "templated": false
        },
        "loggers-name": {
            "href": "http://localhost:8080/actuator/loggers/{name}",
            "templated": true
        },
        "heapdump": {
            "href": "http://localhost:8080/actuator/heapdump",
            "templated": false
        },
        "threaddump": {
            "href": "http://localhost:8080/actuator/threaddump",
            "templated": false
        },
        "metrics": {
            "href": "http://localhost:8080/actuator/metrics",
            "templated": false
        },
        "metrics-requiredMetricName": {
            "href": "http://localhost:8080/actuator/metrics/{requiredMetricName}",
            "templated": true
        },
        "scheduledtasks": {
            "href": "http://localhost:8080/actuator/scheduledtasks",
            "templated": false
        },
        "mappings": {
            "href": "http://localhost:8080/actuator/mappings",
            "templated": false
        }
    }	
}
```

<a name="nSPcN"></a>
### 9、其他具体的使用如健康检查和Bean加载可以访问相应的路径
<a name="tIC8q"></a>
#### A.查看Bean的加载依赖-[http://localhost:8080/actuator/beans](http://localhost:8080/actuator/beans)
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1575776038766-f1afcb66-5b93-45fd-8a53-75b2fa56e9f9.png#align=left&display=inline&height=726&originHeight=726&originWidth=1366&size=99659&status=done&style=none&width=1366)
