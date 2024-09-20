SpringBoot 反爬虫<br />kk-anti-reptile 是适用于基于 spring-boot 开发的分布式系统的反爬虫组件。
<a name="LLHts"></a>
### 系统要求

- 基于 spring-boot 开发(spring-boot1.x, spring-boot2.x均可)
- 需要使用 redis
<a name="HI0xM"></a>
### 工作流程
kk-anti-reptile 使用基于 Servlet 规范的的 Filter 对请求进行过滤，在其内部通过 spring-boot 的扩展点机制，实例化一个 Filter，并注入到 Spring 容器 FilterRegistrationBean 中，通过 Spring 注入到 Servlet 容器中，从而实现对请求的过滤。在 kk-anti-reptile 的过滤 Filter 内部，又通过责任链模式，将各种不同的过滤规则织入，并提供抽象接口，可由调用方进行规则扩展。Filter 调用则链进行请求过滤，如过滤不通过，则拦截请求，返回状态码 509，并输出验证码输入页面，输出验证码正确后，调用过滤规则链对规则进行重置。目前规则链中有如下两个规则
<a name="icVlT"></a>
#### ip-rule
ip-rule 通过时间窗口统计当前时间窗口内请求数，小于规定的最大请求数则可通过，否则不通过。时间窗口、最大请求数、ip 白名单等均可配置。
<a name="fUJDT"></a>
#### ua-rule
ua-rule 通过判断请求携带的 User-Agent，得到操作系统、设备信息、浏览器信息等，可配置各种维度对请求进行过滤。
<a name="Aue5H"></a>
### 命中规则后
命中爬虫和防盗刷规则后，会阻断请求，并生成接除阻断的验证码，验证码有多种组合方式，如果客户端可以正确输入验证码，则可以继续访问。验证码有中文、英文字母+数字、简单算术三种形式，每种形式又有静态图片和 GIF 动图两种图片格式，即目前共有如下六种，所有类型的验证码会随机出现，目前技术手段识别难度极高，可有效阻止防止爬虫大规模爬取数据<br />![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1600434821608-e0352c1f-6cf3-4f77-9951-fa59fd1677b0.gif#align=left&display=inline&height=48&originHeight=48&originWidth=130&size=0&status=done&style=shadow&width=130)
<a name="ea30737d"></a>
### 接入使用
后端接入非常简单，只需要引用 kk-anti-reptile 的 maven 依赖，并配置启用 kk-anti-reptile 即可加入 maven 依赖
```xml
<dependency>
    <groupId>cn.keking.project<groupId>
    <artifactId>kk-anti-reptile<artifactId>
    <version>1.0.0-SNAPSHOT<version>
<dependency>
```
配置启用
```yaml
kk-anti-reptileanti.reptile.manager.enabled=true
```
前端需要在统一发送请求的 ajax 处加入拦截，拦截到请求返回状态码 509 后弹出一个新页面，并把响应内容转出到页面中，然后向页面中传入后端接口 baseUrl 参数即可，以使用 axios 请求为例：
```javascript
import axios from 'axios';
import {baseUrl} from './config';
axios.interceptors.response.use(
  data => {
    return data;
  },
  error => {
    if (error.response.status === 509) {
      let html = error.response.data;
      let verifyWindow = window.open("","_blank","height=400,width=560");
      verifyWindow.document.write(html);
      verifyWindow.document.getElementById("baseUrl").value = baseUrl;
    }
  }
);
export default axios;
```
<a name="sQeXO"></a>
### 注意
<a name="BSR7c"></a>
#### apollo-client 需启用 bootstrap
使用 apollo 配置中心的用户，由于组件内部用到 `@ConditionalOnProperty`，要在 application.properties/bootstrap.properties 中加入如下样例配置，(apollo-client 需要 0.10.0 及以上版本）详见 apollo bootstrap 说明
```yaml
apollo.bootstrap.enabled = true
```
<a name="iLBJK"></a>
#### 需要有 Redisson
连接如果项目中有用到 Redisson，kk-anti-reptile 会自动获取 RedissonClient 实例对象; 如果没用到，需要在配置文件加入如下 Redisson 连接相关配置:
```yaml
spring.redisson.address=redis://192.168.1.204:6379
spring.redisson.password=xxx
```
<a name="zvAmJ"></a>
### 配置一览表
在 spring-boot 中，所有配置在配置文件都会有自动提示和说明<br />所有配置都以 anti.reptile.manager 为前缀，如下为所有配置项及说明:

| NAME | 描述 | 默认值 | 示例 |
| --- | --- | --- | --- |
| enabled | 是否启用反爬虫插件 | true | true |
| include-urls | 需要反爬的接口列表，以/开头，以','分隔 | 空 | /client,/user |
| ip-rule.enabled | 是否启用IP Rule | true | true |
| ip-rule.expiration-time | 时间窗口长度(ms) | 5000 | 5000 |
| ip-rule.request-max-size | 单个时间窗口内，最大请求数 | 20 | 20 |
| ip-rule.ignore-ip | IP白名单，支持后缀'*'通配，以','分隔 | 空 | 192.168.*,127.0.0.1 |
| ua-rule.enabled | 是否启用User-Agent Rule | true | true |
| ua-rule.allowed-linux | 是否允许Linux系统访问 | false | false |
| ua-rule.allowed-mobile | 是否允许移动端设备访问 | true | true |
| ua-rule.allowed-pc | 是否允许移PC设备访问 | true | true |
| ua-rule.allowed-iot | 是否允许物联网设备访问 | false | false |
| ua-rule.allowed-proxy | 是否允许代理访问 | false | false |


