Java<br />HttpClient优化思路：

1. **池化**
2. **长连接**
3. **httpclient和httpget复用**
4. **合理的配置参数（最大并发请求数，各种超时时间，重试次数）**
5. **异步 **
6. **多读源码**
<a name="SGVwD"></a>
## 1、背景
有个业务，会调用其他部门提供的一个基于http的服务，日调用量在千万级别。使用了httpclient来完成业务。之前因为qps上不去，就看了一下业务代码，并做了一些优化，记录在这里。<br />先对比前后：优化之前，平均执行时间是250ms；优化之后，平均执行时间是80ms，降低了三分之二的消耗，容器不再动不动就报警线程耗尽了，清爽~
<a name="o820V"></a>
## 2、分析
项目的原实现比较粗略，就是每次请求时初始化一个httpclient，生成一个httpPost对象，执行，然后从返回结果取出entity，保存成一个字符串，最后显式关闭response和client。一点点分析和优化：
<a name="We1xh"></a>
### 2.1 httpclient反复创建开销
httpclient是一个线程安全的类，没有必要由每个线程在每次使用时创建，全局保留一个即可。
<a name="Cwfjf"></a>
### 2.2 反复创建tcp连接的开销
tcp的三次握手与四次挥手两大裹脚布过程，对于高频次的请求来说，消耗实在太大。试想如果每次请求需要花费5ms用于协商过程，那么对于qps为100的单系统，1秒钟就要花500ms用于握手和挥手。又不是高级领导，程序员就不要搞这么大做派了，改成keep alive方式以实现连接复用！
<a name="ImXuD"></a>
### 2.3 重复缓存entity的开销
原本的逻辑里，使用了如下代码：
```java
HttpEntity entity = httpResponse.getEntity();
String response = EntityUtils.toString(entity);
```
这里相当于额外复制了一份content到一个字符串里，而原本的httpResponse仍然保留了一份content，需要被consume掉，在高并发且content非常大的情况下，会消耗大量内存。并且，需要显式的关闭连接，ugly。
<a name="YEjcR"></a>
## 3、实现
按上面的分析，主要要做三件事：一是单例的client，二是缓存的保活连接，三是更好的处理返回结果。一就不说了，来说说二。<br />提到连接缓存，很容易联想到数据库连接池。httpclient4提供了一个`PoolingHttpClientConnectionManager`作为连接池。接下来通过以下步骤来优化：
<a name="hK1k9"></a>
### 3.1 定义一个keep alive strategy
关于keep-alive，这里不展开说明，只提一点，是否使用keep-alive要根据业务情况来定，它并不是灵丹妙药。还有一点，keep-alive和time_wait/close_wait之间也有不少故事。<br />在本业务场景里，相当于有少数固定客户端，长时间极高频次的访问服务器，启用keep-alive非常合适<br />再多提一嘴，http的keep-alive 和tcp的KEEPALIVE不是一个东西。回到正文，定义一个strategy如下：
```java
ConnectionKeepAliveStrategy myStrategy = new ConnectionKeepAliveStrategy() {
	@Override
	public long getKeepAliveDuration(HttpResponse response, HttpContext context) {
		HeaderElementIterator it = new BasicHeaderElementIterator
			(response.headerIterator(HTTP.CONN_KEEP_ALIVE));
		while (it.hasNext()) {
			HeaderElement he = it.nextElement();
			String param = he.getName();
			String value = he.getValue();
			if (value != null && param.equalsIgnoreCase
				("timeout")) {
				return Long.parseLong(value) * 1000;
			}
		}
		return 60 * 1000;//如果没有约定，则默认定义时长为60s
	}
};
```
<a name="Cecxf"></a>
### 3.2 配置一个`PoolingHttpClientConnectionManager`
```java
PoolingHttpClientConnectionManager connectionManager = new PoolingHttpClientConnectionManager();
connectionManager.setMaxTotal(500);
connectionManager.setDefaultMaxPerRoute(50);//例如默认每路由最高50并发，具体依据业务来定
```
也可以针对每个路由设置并发数。
<a name="VhZmN"></a>
### 3.3 生成httpclient
```java
httpClient = HttpClients.custom()
    .setConnectionManager(connectionManager)
    .setKeepAliveStrategy(kaStrategy)
    .setDefaultRequestConfig(RequestConfig.custom().setStaleConnectionCheckEnabled(true).build())
    .build();
```
注意：使用`setStaleConnectionCheckEnabled`方法来逐出已被关闭的链接不被推荐。更好的方式是手动启用一个线程，定时运行`closeExpiredConnections`和`closeIdleConnections`方法，如下所示。
```java
public static class IdleConnectionMonitorThread extends Thread {
    
    private final HttpClientConnectionManager connMgr;
    private volatile boolean shutdown;
    
    public IdleConnectionMonitorThread(HttpClientConnectionManager connMgr) {
        super();
        this.connMgr = connMgr;
    }
 
    @Override
    public void run() {
        try {
            while (!shutdown) {
                synchronized (this) {
                    wait(5000);
                    // Close expired connections
                    connMgr.closeExpiredConnections();
                    // Optionally, close connections
                    // that have been idle longer than 30 sec
                    connMgr.closeIdleConnections(30, TimeUnit.SECONDS);
                }
            }
        } catch (InterruptedException ex) {
            // terminate
        }
    }
    
    public void shutdown() {
        shutdown = true;
        synchronized (this) {
            notifyAll();
        }
    }
    
}
```
<a name="XfmKv"></a>
### 3.4 使用httpclient执行method时降低开销
这里要注意的是，不要关闭connection。<br />一种可行的获取内容的方式类似于，把entity里的东西复制一份：
```java
res = EntityUtils.toString(response.getEntity(),"UTF-8");
EntityUtils.consume(response1.getEntity());
```
但是，更推荐的方式是定义一个`ResponseHandler`，方便你我他，不再自己catch异常和关闭流。在此可以看一下相关的源码：
```java
public <T> T execute(final HttpHost target, final HttpRequest request,
					 final ResponseHandler<? extends T> responseHandler, final HttpContext context)
	throws IOException, ClientProtocolException {
	Args.notNull(responseHandler, "Response handler");

	final HttpResponse response = execute(target, request, context);

	final T result;
	try {
		result = responseHandler.handleResponse(response);
	} catch (final Exception t) {
		final HttpEntity entity = response.getEntity();
		try {
			EntityUtils.consume(entity);
		} catch (final Exception t2) {
			// Log this exception. The original exception is more
			// important and will be thrown to the caller.
			this.log.warn("Error consuming content after an exception.", t2);
		}
		if (t instanceof RuntimeException) {
			throw (RuntimeException) t;
		}
		if (t instanceof IOException) {
			throw (IOException) t;
		}
		throw new UndeclaredThrowableException(t);
	}

	// Handling the response was successful. Ensure that the content has
	// been fully consumed.
	final HttpEntity entity = response.getEntity();
	EntityUtils.consume(entity);//看这里看这里
	return result;
}
```
可以看到，如果使用`resultHandler`执行`execute`方法，会最终自动调用`consume`方法，而这个`consume`方法如下所示：
```java
public static void consume(final HttpEntity entity) throws IOException {
	if (entity == null) {
		return;
	}
	if (entity.isStreaming()) {
		final InputStream instream = entity.getContent();
		if (instream != null) {
			instream.close();
		}
	}
}
```
可以看到最终它关闭了输入流。
<a name="fomhL"></a>
## 4、其他
通过以上步骤，基本就完成了一个支持高并发的httpclient的写法，下面是一些额外的配置和提醒：
<a name="pZmEx"></a>
### 4.1 httpclient的一些超时配置
CONNECTION_TIMEOUT是连接超时时间，SO_TIMEOUT是socket超时时间，这两者是不同的。连接超时时间是发起请求前的等待时间；socket超时时间是等待数据的超时时间。
```java
HttpParams params = new BasicHttpParams();
//设置连接超时时间
Integer CONNECTION_TIMEOUT = 2 * 1000; //设置请求超时2秒钟 根据业务调整
Integer SO_TIMEOUT = 2 * 1000; //设置等待数据超时时间2秒钟 根据业务调整

//定义了当从ClientConnectionManager中检索ManagedClientConnection实例时使用的毫秒级的超时时间
//这个参数期望得到一个java.lang.Long类型的值。如果这个参数没有被设置，默认等于CONNECTION_TIMEOUT，因此一定要设置。
Long CONN_MANAGER_TIMEOUT = 500L; //在httpclient4.2.3中我记得它被改成了一个对象导致直接用long会报错，后来又改回来了

params.setIntParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, CONNECTION_TIMEOUT);
params.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, SO_TIMEOUT);
params.setLongParameter(ClientPNames.CONN_MANAGER_TIMEOUT, CONN_MANAGER_TIMEOUT);
//在提交请求之前 测试连接是否可用
params.setBooleanParameter(CoreConnectionPNames.STALE_CONNECTION_CHECK, true);

//另外设置http client的重试次数，默认是3次；当前是禁用掉（如果项目量不到，这个默认即可）
httpClient.setHttpRequestRetryHandler(new DefaultHttpRequestRetryHandler(0, false));
```
<a name="uXgGJ"></a>
### 4.2 如果配置了Nginx的话，Nginx也要设置面向两端的keep-alive
现在的业务里，没有Nginx的情况反而比较稀少。Nginx默认和client端打开长连接而和server端使用短链接。注意client端的keepalive_timeout和keepalive_requests参数，以及upstream端的keepalive参数设置，这三个参数的意义在此也不再赘述。<br />以上就是全部设置。通过这些设置，成功地将原本每次请求250ms的耗时降低到了80左右，效果显著。<br />JAR包如下：
```xml
<!-- httpclient -->
<dependency>
	<groupId>org.apache.httpcomponents</groupId>
	<artifactId>httpclient</artifactId>
	<version>4.5.6</version>
</dependency>
```
代码如下：
```java
//Basic认证
private static final CredentialsProvider credsProvider = new BasicCredentialsProvider();
//httpClient
private static final CloseableHttpClient httpclient;
//httpGet方法
private static final HttpGet httpget;
//
private static final RequestConfig reqestConfig;
//响应处理器
private static final ResponseHandler<String> responseHandler;
//jackson解析工具
private static final ObjectMapper mapper = new ObjectMapper();

static {
	System.setProperty("http.maxConnections","50");
	System.setProperty("http.keepAlive", "true");
	//设置basic校验
	credsProvider.setCredentials(
		new AuthScope(AuthScope.ANY_HOST, AuthScope.ANY_PORT, AuthScope.ANY_REALM),
		new UsernamePasswordCredentials("", ""));
	//创建http客户端
	httpclient = HttpClients.custom()
		.useSystemProperties()
		.setRetryHandler(new DefaultHttpRequestRetryHandler(3,true))
		.setDefaultCredentialsProvider(credsProvider)
		.build();
	//初始化httpGet
	httpget = new HttpGet();
	//初始化HTTP请求配置
	reqestConfig = RequestConfig.custom()
		.setContentCompressionEnabled(true)
		.setSocketTimeout(100)
		.setAuthenticationEnabled(true)
		.setConnectionRequestTimeout(100)
		.setConnectTimeout(100).build();
	httpget.setConfig(reqestConfig);
	//初始化response解析器
	responseHandler = new BasicResponseHandler();
}
/*
* 功能：返回响应
* @date 2019/1/3 上午11:19
* @param [url]
* @return org.apache.http.client.methods.CloseableHttpResponse
* @exception
*/
public static String getResponse(String url) throws IOException {
	HttpGet get = new HttpGet(url);
	String response = httpclient.execute(get,responseHandler);
	return response;
}

/*
* 功能：发送http请求，并用net.sf.json工具解析
* @date 2018/8/15 下午2:21
* @param [url]
* @return org.json.JSONObject
* @exception
*/
public static JSONObject getUrl(String url) throws Exception{
	try {
		httpget.setURI(URI.create(url));
		String response = httpclient.execute(httpget,responseHandler);
		JSONObject json = JSONObject.fromObject(response);
		return json;
	} catch (IOException e) {
		e.printStackTrace();
	}
	return null;
}

/*
* 功能：发送http请求，并用jackson工具解析
* @date 2018/12/24 下午2:58
* @param [url]
* @return com.fasterxml.jackson.databind.JsonNode
* @exception
*/
public static JsonNode getUrl2(String url){
	try {
		httpget.setURI(URI.create(url));
		String response = httpclient.execute(httpget,responseHandler);
		JsonNode node = mapper.readTree(response);
		return node;
	} catch (IOException e) {
		e.printStackTrace();
	}
	return null;
}
/*
* 功能：发送http请求，并用fastjson工具解析
* @date 2018/12/24 下午2:58
* @param [url]
* @return com.fasterxml.jackson.databind.JsonNode
* @exception
*/
public static com.alibaba.fastjson.JSONObject getUrl3(String url){
	try {
		httpget.setURI(URI.create(url));
		String response = httpclient.execute(httpget,responseHandler);
		com.alibaba.fastjson.JSONObject jsonObject = com.alibaba.fastjson.JSONObject.parseObject(response);
		return jsonObject;
	} catch (IOException e) {
		e.printStackTrace();
	}
	return null;
}
```
