JavaSpringBoot
<a name="iwgKz"></a>
# Spring Boot 控制 HTTP缓存
HTTP协议的缓存策略可以缩短网页请求资源的距离，减少延迟，节省网络流量，并且由于缓存文件可以重复利用，降低网络负荷，加快客户端响应。<br />如何利用 HTTP缓存 进行一些操作是最近思考的问题。查看了Spring Boot对HTTP缓存的支持也只有`org.springframework.web.filter.ShallowEtagHeaderFilter`和`plus.wcj.heifer.boot.HeiferBootAutoConfiguration#addResourceHandlers`这个两个方式能对HTTP缓存进行骚操作，

- `org.springframework.web.filter.ShallowEtagHeaderFilter`只支持静态资源的缓存协商，看了一下注释说明只节省带宽，而不是服务器性能。
- `plus.wcj.heifer.boot.HeiferBootAutoConfiguration#addResourceHandlers`可以支持HTTP API，但是不支持RESTful API。

去GitHub寻找一下有没有支持HTTP缓存的轮子，一顿寻找下来颗粒无收，那索性就自己造轮子吧。
<a name="BJe4p"></a>
# 缓存协商机制
维护缓存过期网上有很多的文章，这里就不再赘述了，下面列出一些文章

- [https://developer.mozilla.org/en-US/docs/Web/HTTP/Caching](https://developer.mozilla.org/en-US/docs/Web/HTTP/Caching)
- [https://web.dev/i18n/en/http-cache/](https://web.dev/i18n/en/http-cache/)

因为HTTP协议的版本问题，就采用了HTTP协议1.0的实现了。

1. request
- If-None-Match
2. response
- Cache-control
- ETag

就用以上三个headers来实现缓存协商机制，Cache-control用于控制缓存过期时间，If-None-Match和ETag用于缓存协商，基于上面3个标签来实现HTTP缓存和缓存协商。
<a name="SUIvM"></a>
# 轮子使用
轮子已经造好了并且已经发布到Maven中央仓库了，这里就略过实现过程了，感兴趣的可以去看一下源码。

- githu地址：[https://github.com/galaxy-sea/libby-http-cache-control](https://github.com/galaxy-sea/libby-http-cache-control)
- maven中央库：[https://repo1.maven.org/maven2/plus/wcj/libby-http-cache-control/2022.1.0/](https://repo1.maven.org/maven2/plus/wcj/libby-http-cache-control/2022.1.0/)

pom引入，
```xml
<dependency>
	<groupId>plus.wcj</groupId>
	<artifactId>libby-http-cache-control</artifactId>
	<version>2022.1.0</version>
</dependency>
```
一个简单的小DEMO， 访问 /html就会出现一个简单的网页了，然后大家就点击上面的button开始测试吧，
```java
@RestController
@RequestMapping
public class CacheController {
	private Map<String, Long> data = new HashMap<>();
	
	@GetMapping("cache/{id}")
	@HttpCacheControl(key = "#id",value = "max-age=10, public")
	public Long get(@PathVariable String id) {
		return data.computeIfAbsent(id, s -> System.currentTimeMillis());
	}
	
	@PostMapping("cache/{id}")
	@HttpETagBind(key = "#id")
	public void post(@PathVariable String id) {
		data.put(id, System.currentTimeMillis());
	}
	
	@GetMapping("html")
	public ResponseEntity<String> html() {
		return ResponseEntity
			.ok()
			.body("<!DOCTYPE html><html><head><meta charset=\"utf-8\"><script src=\"https://cdn.staticfile.org/jquery/1.10.2/jquery.min.js\"></script><script>$(document).ready(function(){$(\"button:nth-child(1)\").click(function(){$.get(\"./cache/1\",function(data,status){$(\"ol\").append(\"<li>cache1: data: \"+data+\"</li>\")})});$(\"button:nth-child(2)\").click(function(){$.get(\"./cache/2\",function(data,status){$(\"ol\").append(\"<li>cache2: data: \"+data+\"</li>\")})});$(\"button:nth-child(3)\").click(function(){$.post(\"./cache/1\",function(data,status){$(\"ol\").append(\"<li>cache1: modify cache1</li>\")})});$(\"button:nth-child(4)\").click(function(){$.post(\"./cache/2\",function(data,status){$(\"ol\").append(\"<li>cache2: modify cache2</li>\")})})});</script></head><body><button>get cache1</button><button>get cache2</button><button>modify cache1</button><button>modify cache2</button><ol><li>start test</li></ol></body></html>");
	}
}
```
<a name="FEPU7"></a>
# 缓存问题
因为HTTP协议只是一个规范，所以不同的客户端和代理服务对HTTP实现各不相同，不同的客户端+代理服务的组合会出现不同的缓存效果。
<a name="Jcrz4"></a>
## 浏览器缓存 问题
测试的时候建议使用多个浏览器进行测试，对于没有实现 HTTP缓存标准 的发包工具，是不会去维护Cache-control、If-None-Match、ETag等缓存标签。<br />如果API设计风格 RESTful风格，浏览器（Chrome、firefox）会维护If-None-Match标签。

1. GET cache/{id}
2. POST cache/{id}
3. GET cache/{id}

浏览器会在步骤2删除本地缓存，那么步骤3的If-None-Match就不会传输了。<br />总结 Chrome、firefox对HTTP缓存的实现是最标准的，API发包工具不维护HTTP缓存，
<a name="d0uNF"></a>
## 代理缓存 问题
在进行缓存测试的发现一个Nginx、Apache APISIX、Spring Cloud Gateway等等网关的一些小问题。<br />`CacheControl: max-age=10, public`，从理论上来说public应该让代理服务把数据缓存在本地（有些网关需要增加配置才能开启 代理缓存），但是不同的网关对HTTP缓存实现的不同，会表现不同的行为。<br />如果使用了(共享)代理缓存请配合 运维兄弟 多多测试。 <br />如果使用了(共享)代理缓存请配合 运维兄弟 多多测试。 <br />如果使用了(共享)代理缓存请配合 运维兄弟 多多测试。
