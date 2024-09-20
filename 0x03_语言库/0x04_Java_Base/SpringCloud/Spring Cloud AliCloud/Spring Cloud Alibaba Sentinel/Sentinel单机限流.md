Java SpringBoot
<a name="lCzov"></a>
### 创建 SpringBoot 服务
首先创建一个 SpringBoot 服务，在 pom.xml 文件中增加下面的配置
```xml
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-web</artifactId>
</dependency> 
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-core</artifactId>
	<version>1.8.4</version>
</dependency>
```
然后提供一个对外的 http 接口，通过访问接口来触发限流代码，接口代码如下<br />代码如下：
```java
package com.example.demo.controller;

import com.alibaba.csp.sentinel.SphO;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
* <br>
* <b>Function：</b><br>
* <b>Author：</b>@author Fcant<br>
* <b>Date：</b>2022-05-08 12:56<br>
* <b>Desc：</b>无<br>
*/
@RestController
public class LoginController {
	
	@GetMapping(value = "/login")
	public void login(String username, String password) {
		System.out.println("login");
		//模拟一百万条消息
		for (int i = 0; i < 1000000; i++) {
			boolean entry = false;
			try {
				entry = SphO.entry("HelloWorld");
				while (!entry) {
					try {
						Thread.sleep(50);
						System.out.println("entry false");
						entry = SphO.entry("HelloWorld");
					} catch (InterruptedException e) {
						
					}
				}
				System.out.println("entry true");
			} catch (Exception e) {
				
			} finally {
				if (entry) {
					SphO.exit();
				}
			}
		}
	}
}
```
调用接口过后，通过循环一百万次来模拟大流量，这里要解释以下几个内容

1. `SphO.entry("HelloWorld")`：是 Sentinel 的资源控制器，"HelloWord" 是资源的名称，**资源** 是 sentinel 的一个很重要的概念，所有的限流都是针对资源的操作；`SphO.entry()` 返回值是布尔值，为 true 表示资源可用，没有被限流，为 false 表示资源被限流；
2. 这里模拟在被限流了过后，程序等待一段时间，再去判断是否限流，只有在资源未被限流的时候，才能继续处理；
3. 在 `finally` 里面需要进行 `SphO.exit();` 操作，当被限流了以后，也就是`SphO.entry() == true`  后一定要执行 `SphO.exit();` 否则代码会创建多个Entry 对象，程序运行时间长了过后会导致内存泄露，引发 FullGC。

这个时候启动一个服务，调用一下接口，可以看到效果如下，很快就会运行完，并没有达到限流的效果，那是因为此刻还没有配置限流规则，所以没有触发到限流的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652141956006-fe4416e0-e578-42bc-bc11-b34c10159a96.jpeg#clientId=u582c10f3-a054-4&from=paste&id=ua11abaa7&originHeight=1301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u662597aa-343e-437e-aafd-64ae2d87b1e&title=)
<a name="uSuun"></a>
### 配置 sentinel 控制台
接下来安装一下 sentinel 的控制台，通过控制台来配置限流规则，从而达到限流的目的，控制台的搭建很简单，通过官方地址下载指定版本的 jar 然后本地运行即可。通过地址 [https://github.com/alibaba/Sentinel/releases/download/1.8.4/sentinel-dashboard-1.8.4.jar](https://github.com/alibaba/Sentinel/releases/download/1.8.4/sentinel-dashboard-1.8.4.jar) 进行下载。<br />然后通过命令`java -Dserver.port=8081 -Dcsp.sentinel.dashboard.server=localhost:8081 -Dproject.name=sentinel-dashboard -jar sentinel-dashboard-1.8.4.jar` 运行即可，如下所示<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652141956039-31a49520-79ce-44b6-8450-61e21b5c017e.jpeg#clientId=u582c10f3-a054-4&from=paste&id=u687da088&originHeight=442&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uec845d00-ee4c-4fb7-97bc-028306abfc3&title=)这里通过指定 8081 端口，用于访问 sentinel，启动成功过后，通过浏览器可以进行登录，默认的初始账号和密码都是 sentinel。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652141956000-c9b53d80-b689-4ef3-ba29-b88d0234d618.jpeg#clientId=u582c10f3-a054-4&from=paste&id=ufb402e15&originHeight=510&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6fd77d6e-1181-4072-9161-c7594ad870b&title=)因为上面的命令指定了 sentinel-dashboard 项目，所以默认只会看到 sentinel-dashboard 这个项目，这个时候需要，修改代码，在 pom.xml 中增加下面的配置
```xml
<dependency>
	<groupId>com.alibaba.csp</groupId>
	<artifactId>sentinel-transport-simple-http</artifactId>
	<version>1.8.4</version>
</dependency>
```
然后在 JVM 的启动参数中增加`-Dcsp.sentinel.dashboard.server=localhost:8081` 指明 sentinel 的地址和端口号，再启动应用。启动完过后，要手动调用一下接口，然后就可以看到程序项目连接到 sentinel 了。不过此时只是程序和 sentinel 连接成功，还没有限流规则，接下来要配置一下限流规则。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652141956039-ffa576ee-3f68-4e41-bf1f-9c9df71978ac.jpeg#clientId=u582c10f3-a054-4&from=paste&id=u65e60af8&originHeight=431&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0fda7695-1287-4ed9-be12-3f65b2f318f&title=)按照上图配置好了过后，再调用一次接口，可以看到此时处理速度明显慢了下来，每秒只有 20 个 QPS 能获取资源了，至此基于 sentinel 的单机限流QPS 20 的目标完成。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1652141956035-9bbbfbed-084c-4b5c-922d-054634499a6e.jpeg#clientId=u582c10f3-a054-4&from=paste&id=u3fcb6aac&originHeight=1301&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u02dfea7e-2266-4fca-a5c5-c88a53c0e18&title=)
