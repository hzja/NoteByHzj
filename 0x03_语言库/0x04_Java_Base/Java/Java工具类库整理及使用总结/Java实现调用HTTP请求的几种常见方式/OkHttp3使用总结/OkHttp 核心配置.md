Java
<a name="Xq9oa"></a>
## 一、摘要
OkHttp 的网络请求操作方式，虽然能满足基本日常网络请求使用，但是离实际的业务需求定制化开发还远远不够，这里重点介绍 OkHttp 的一些配置项的使用方式。
<a name="cHGEG"></a>
## 二、案例实践
本次采用okhttp3库版本为3.14.9，依赖包如下！
```xml
<dependency>
  <groupId>com.squareup.okhttp3</groupId>
  <artifactId>okhttp</artifactId>
  <version>3.14.9</version>
</dependency>
```
<a name="qH7A6"></a>
### 2.1、全局设置超时时长
如果想设置全局配置请求的超时时间，可以通过如下方式实现！<br />案例如下！
```java
// 设置请求配置相关参数
OkHttpClient client = new OkHttpClient().newBuilder()
        .connectTimeout(5, TimeUnit.SECONDS)     //设置socket连接超时时间
        .readTimeout(5, TimeUnit.SECONDS)        // 设置数据读取连接超时时间
        .writeTimeout(5, TimeUnit.SECONDS)       // 设置数据写入连接超时时间
        .retryOnConnectionFailure(true)                  //是否自动重连
        .build();
```
<a name="HBaZz"></a>
### 2.2、全局配置连接池
默认的情况下，共享连接池的配置为：最多有 5 个空闲连接，每个空闲连接最多保持 5 分钟，源码如下：
```java
public final class ConnectionPool {
    final RealConnectionPool delegate;

    public ConnectionPool() {
        this(5, 5L, TimeUnit.MINUTES);
    }

    //....
}
```
当然，也可以自定义连接池配置，实现方式如下！<br />案例如下！
```java
// 设置请求配置相关参数，配置共享连接池
OkHttpClient client = new OkHttpClient().newBuilder()
        .connectionPool(new ConnectionPool(5, 10, TimeUnit.MINUTES))
        .build();
```
表示最多有 5 个空闲连接，每个空闲连接最多保持 10 分钟。
<a name="PGNRq"></a>
###  2.3、配置全局拦截器
某些时候，想为通过使用 okhttp 的网路请求，统一加上固定的请求头部或者对返回参数进行统一解析，可以添加一个请求拦截器。<br />案例如下！
```java
// 设置请求配置相关参数，添加一个拦截器
OkHttpClient client = new OkHttpClient().newBuilder().addInterceptor(new Interceptor() {
    @Override
    public Response intercept(Chain chain) throws IOException {
        Request request = chain.request();
        System.out.println("请求前拦截，请求参数：" + request.toString());
        Response response = chain.proceed(request);
        System.out.println("请求后拦截，返回参数：" + response.toString());
        return response;
    }
})
.build();
```
拦截器在实际的开发中，使用的非常频繁，比如想知道每个请求阶段的详细日志，OkHttp 已经预制了一个日志拦截器，只需要添加相应的依赖包文件，即可进行打印。
```xml
<!--添加日志拦截器-->
<dependency>
  <groupId>com.squareup.okhttp3</groupId>
  <artifactId>logging-interceptor</artifactId>
  <version>3.14.9</version>
</dependency>
```
在请求头部里面添加它，即可对每个阶段的请求日志进行详细打印。
```java
// 日志拦截器
HttpLoggingInterceptor logInterceptor = new HttpLoggingInterceptor();
logInterceptor.setLevel(HttpLoggingInterceptor.Level.BASIC);

// 设置请求配置相关参数
OkHttpClient client = new OkHttpClient().newBuilder()
        .addInterceptor(logInterceptor)
        .build();
```
<a name="WvF4B"></a>
### 2.4、全局配置缓存
在调试接口的时候，可能已经注意到，完成了很多不必要的请求，给服务器带来了额外的负载。如果请求服务端返回的结果都是一样的，此时配置缓存，可以很大的缓解服务端的请求压力。<br />在 OkHttp  中有一个默认的缓存实现，只需要指定缓存位置和它的大小即可。<br />案例如下！
```java
// 设置请求配置相关参数，配置缓存文件
OkHttpClient client = new OkHttpClient().newBuilder()
        .cache(new Cache(new File("/local/cacheDirectory"), 10 * 1024 * 1024))
        .build();
```
如果某个接口不想走缓存，每次直接进行网络请求，可以通过在请求配置上指定`CacheControl.FORCE_NETWORK`，设置此次请求能使用网络，不用缓存。<br />案例如下！
```java
// 设置请求配置相关参数，全局配置缓存
OkHttpClient client = new OkHttpClient().newBuilder()
        .cache(new Cache(new File("/local/cacheDirectory"), 10 * 1024 * 1024))
        .build();

// 配置GET请求，配置FORCE_NETWORK，强制走网路请求
Request request = new Request.Builder()
        .url(url)
        .cacheControl(CacheControl.FORCE_NETWORK)
        .build();
```
<a name="BSuUK"></a>
### 2.5、全局配置代理
有些环境，无法直接请求服务端的接口，而是需要通过代理服务器进行请求，此时如何配置呢？<br />案例如下！
```java
// 配置代理服务器的ip和端口
Proxy proxy = new Proxy(
    Proxy.Type.HTTP,
    new InetSocketAddress("192.168.1.100", 8080) // the local proxy
);

// 设置请求配置相关参数
OkHttpClient client = new OkHttpClient().newBuilder()
.proxy(proxy)
.build();
```
<a name="Eqq5j"></a>
### 2.6、全局配置 https
有些环境，需要以 https 的安全协议进行请求，此时如何配置呢？<br />案例如下！
```java
import okhttp3.*;
import okhttp3.internal.platform.Platform;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.net.ssl.*;
import java.security.SecureRandom;
import java.security.cert.X509Certificate;

public class TestClientHttps {

    public static void main(String[] args)  {
        String url = "https://www.baidu.com/";

        OkHttpClient client = new OkHttpClient().newBuilder()
        .sslSocketFactory(getSSLSocketFactory(), getX509TrustManager())
        .hostnameVerifier(getHostnameVerifier())
        .build();

        Request request = new Request.Builder()
        .url(url)
        .get()
        .build();

        // 发起同步请求
        try (Response response = client.newCall(request).execute()){
            // 打印返回结果
            System.out.println(response.body().string());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取SSLSocketFactory
     * @return
     */
    public static SSLSocketFactory getSSLSocketFactory() {
        try {
            SSLContext sslContext = SSLContext.getInstance("SSL");
            sslContext.init(null, getTrustManager(), new SecureRandom());
            return sslContext.getSocketFactory();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * 获取TrustManager
     * @return
     */
    private static TrustManager[] getTrustManager() {
        TrustManager[] trustAllCerts = new TrustManager[]{
            new X509TrustManager() {
                @Override
                public void checkClientTrusted(X509Certificate[] chain, String authType) {
                }

                @Override
                public void checkServerTrusted(X509Certificate[] chain, String authType) {
                }

                @Override
                public X509Certificate[] getAcceptedIssuers() {
                    return new X509Certificate[]{};
                }
            }
        };
        return trustAllCerts;
    }


    /**
     * 获取HostnameVerifier，验证主机名
     * @return
     */
    public static HostnameVerifier getHostnameVerifier() {
        HostnameVerifier hostnameVerifier = (s, sslSession) -> true;
        return hostnameVerifier;
    }


    /**
     * X509TrustManager：证书信任器管理类
     * @return
     */
    public static X509TrustManager getX509TrustManager() {
        X509TrustManager x509TrustManager = new X509TrustManager() {
            //检查客户端的证书是否可信
            @Override
            public void checkClientTrusted(X509Certificate[] chain, String authType) {

            }
            //检查服务器端的证书是否可信
            @Override
            public void checkServerTrusted(X509Certificate[] chain, String authType) {

            }

            @Override
            public X509Certificate[] getAcceptedIssuers() {
                return new X509Certificate[0];
            }
        };
        return x509TrustManager;
    }
}
```
<a name="WMBPV"></a>
### 2.7、全局配置 WebSocket
某些场景下，希望客户端与服务器实现长链接的模式进行数据通行，使用 OkHttp 客户端如何处理呢？<br />案例如下！
```java
OkHttpClient client = new OkHttpClient();

// 服务端请求地址
String socketServerUrl = "ws://mytodoserver.com/realtime";
Request request = new Request.Builder().url(socketServerUrl).build();

// connecting to a socket and receiving messages
client.newWebSocket(request, new WebSocketListener() {
    @Override
    public void onClosed(@NotNull WebSocket webSocket, int code, @NotNull String reason) {
        super.onClosed(webSocket, code, reason);
        //TODO: implement your own event handling
    }

    @Override
    public void onClosing(@NotNull WebSocket webSocket, int code, @NotNull String reason) {
        super.onClosing(webSocket, code, reason);
        //TODO: implement your own event handling
    }

    @Override
    public void onFailure(@NotNull WebSocket webSocket, @NotNull Throwable t, @Nullable Response response) {
        super.onFailure(webSocket, t, response);
        //TODO: implement your own event handling
    }

    @Override
    public void onMessage(@NotNull WebSocket webSocket, @NotNull String text) {
        super.onMessage(webSocket, text);
        //TODO: implement your own event handling for incoming messages
    }

    @Override
    public void onMessage(@NotNull WebSocket webSocket, @NotNull ByteString bytes) {
        super.onMessage(webSocket, bytes);
        //TODO: implement your own event handling for incoming messages
    }

    @Override
    public void onOpen(@NotNull WebSocket webSocket, @NotNull Response response) {
        super.onOpen(webSocket, response);
        //TODO: implement your own event handling
    }
});
```
<a name="EKBn6"></a>
### 2.8、完整的全局配置参数介绍
OkHttp 支持的全局配置参数非常多，以上只是介绍了一些大家常用的配置，详细的参数配置如下！
```java
// 设置请求配置相关参数
OkHttpClient client = new OkHttpClient.Builder()
        .cache(cache) // configure cache, see above
        .proxy(proxy) // configure proxy, see above
        .certificatePinner(certificatePinner) // certificate pinning, see above
        .addInterceptor(interceptor) // app level interceptor, see above
        .addNetworkInterceptor(interceptor) // network level interceptor, see above
        .authenticator(authenticator) // authenticator for requests (it supports similar use-cases as "Authorization header" earlier
        .callTimeout(10000) // default timeout for complete calls
        .readTimeout(10000) // default read timeout for new connections
        .writeTimeout(10000) // default write timeout for new connections
        .dns(dns) // DNS service used to lookup IP addresses for hostnames
        .followRedirects(true) // follow requests redirects
        .followSslRedirects(true) // follow HTTP tp HTTPS redirects
        .connectionPool(connectionPool) // connection pool used to recycle HTTP and HTTPS connections
        .retryOnConnectionFailure(true) // retry or not when a connectivity problem is encountered
        .cookieJar(cookieJar) // cookie manager
        .dispatcher(dispatcher) // dispatcher used to set policy and execute asynchronous requests
        .build();
```
如果有需要的，可以参考官方文档，地址如下：[https://square.github.io/okhttp/4.x/okhttp/okhttp3/-ok-http-client/-builder/](https://square.github.io/okhttp/4.x/okhttp/okhttp3/-ok-http-client/-builder/)
<a name="wMsuk"></a>
## 三、小结
以上就是针对 OkHttp 在配置使用上，做了一次简单的内容总结，尤其是客户端的全局超时配置、全局连接池配置、全局拦截器等功能，在实际的开发中，使用的比较多；关于 OkHttp 的原理介绍会，后续的文章中进行介绍。
