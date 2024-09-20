Java
<a name="MIEpI"></a>
## 一、介绍
在工作中需要在后台调用各种上传、下载、以及第三方服务接口，经过研究决定使用 HttpClient，封装了一个 HttpClient 工具类，简单轻松的实现get、post、put、delete 以及上传、下载请求，在此分享给大家。
<a name="tBBgl"></a>
## 二、实践应用
基于 HttpClient4.5.5 版本进行开发，也是现在最新的版本，之所以要提供版本说明，是因为 HttpClient 3 版本和 HttpClient 4 版本 API 差别还是很多大的，把 HttpClient 3 版本的代码拿到 HttpClient 4 上面运行不起来，会报错的。所以在使用之前，一定要注意 HtppClient 的版本问题。
<a name="Om0se"></a>
### 2.1、引用 HttpClient  依赖包
```xml
<dependency>
  <groupId>org.apache.httpcomponents</groupId>
  <artifactId>httpclient</artifactId>
  <version>4.5.6</version>
</dependency>
<dependency>
  <groupId>org.apache.httpcomponents</groupId>
  <artifactId>httpcore</artifactId>
  <version>4.4.10</version>
</dependency>
<dependency>
  <groupId>org.apache.httpcomponents</groupId>
  <artifactId>httpmime</artifactId>
  <version>4.5.6</version>
</dependency>
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>fastjson</artifactId>
  <version>1.2.68</version>
</dependency>
```
<a name="LwAbk"></a>
### 2.2、编写工具类（重点）
本次采用单利模式来初始化客户端，并用线程池来管理，同时支持http和https协议，项目启动之后，**无需手动关闭httpClient客户端**！
```java
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.exception.ExceptionUtils;
import org.apache.http.HttpEntity;
import org.apache.http.HttpEntityEnclosingRequest;
import org.apache.http.HttpStatus;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.*;
import org.apache.http.client.utils.HttpClientUtils;
import org.apache.http.config.Registry;
import org.apache.http.config.RegistryBuilder;
import org.apache.http.conn.socket.ConnectionSocketFactory;
import org.apache.http.conn.socket.PlainConnectionSocketFactory;
import org.apache.http.conn.ssl.NoopHostnameVerifier;
import org.apache.http.conn.ssl.SSLConnectionSocketFactory;
import org.apache.http.conn.ssl.TrustStrategy;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.entity.mime.HttpMultipartMode;
import org.apache.http.entity.mime.MultipartEntityBuilder;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.impl.conn.PoolingHttpClientConnectionManager;
import org.apache.http.ssl.SSLContexts;
import org.apache.http.util.EntityUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.net.ssl.SSLContext;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;
import java.util.Map;
import java.util.Objects;

public class HttpUtils {

    private static final Logger log = LoggerFactory.getLogger(HttpUtils.class);

    private HttpUtils() {}

    //多线程共享实例
    private static CloseableHttpClient httpClient;

    static {
        SSLContext sslContext = createSSLContext();
        SSLConnectionSocketFactory sslsf = new SSLConnectionSocketFactory(sslContext, NoopHostnameVerifier.INSTANCE);
        // 注册http套接字工厂和https套接字工厂
        Registry<ConnectionSocketFactory> socketFactoryRegistry = RegistryBuilder.<ConnectionSocketFactory> create()
                .register("http", PlainConnectionSocketFactory.INSTANCE)
                .register("https", sslsf)
                .build();
        // 连接池管理器
        PoolingHttpClientConnectionManager connMgr = new PoolingHttpClientConnectionManager(socketFactoryRegistry);
        connMgr.setMaxTotal(300);//连接池最大连接数
        connMgr.setDefaultMaxPerRoute(300);//每个路由最大连接数，设置的过小，无法支持大并发
        connMgr.setValidateAfterInactivity(5 * 1000); //在从连接池获取连接时，连接不活跃多长时间后需要进行一次验证
        // 请求参数配置管理器
        RequestConfig requestConfig = RequestConfig.custom()
                .setConnectTimeout(60000)
                .setSocketTimeout(60000)
                .setConnectionRequestTimeout(60000)
                .build();
        // 获取httpClient客户端
        httpClient = HttpClients.custom()
                .setConnectionManager(connMgr)
                .setDefaultRequestConfig(requestConfig)
                .build();
    }

    /**
     * GET请求
     * @param url
     * @return
     */
    public static String getUrl(String url) {
        return sendHttp(HttpMethod.GET, url, null, null);
    }

    /**
     * GET请求/带头部的信息
     * @param url
     * @param header
     * @return
     */
    public static String getUrl(String url, Map<String, String> header) {
        return sendHttp(HttpMethod.GET, url, header, null);
    }

    /**
     * POST请求/无参数
     * @param url
     * @return
     */
    public static String postJson(String url) {
        return postJson(url, null, null);
    }

    /**
     * POST请求/有参数
     * @param url
     * @param param
     * @return
     */
    public static String postJson(String url, String param) {
        return postJson(url, null, param);
    }

    /**
     * POST请求/无参数带头部
     * @param url
     * @param header
     * @return
     */
    public static String postJson(String url, Map<String, String> header) {
        return postJson(url, header, null);
    }

    /**
     * POST请求/有参数带头部
     * @param url
     * @param header
     * @param params
     * @return
     */
    public static String postJson(String url, Map<String, String> header, String params) {
        return sendHttp(HttpMethod.POST, url, header, params);
    }

    /**
     * 上传文件流
     * @param url
     * @param header
     * @param param
     * @param fileName
     * @param inputStream
     * @return
     */
    public static RequestResult postUploadFileStream(String url, Map<String, String> header, Map<String, String> param, String fileName, InputStream inputStream) {
        byte[] stream = inputStream2byte(inputStream);
        return postUploadFileStream(url, header, param, fileName, stream);
    }

    /**
     * 上传文件
     * @param url 上传地址
     * @param header 请求头部
     * @param param 请求表单
     * @param fileName 文件名称
     * @param stream 文件流
     * @return
     */
    public static RequestResult postUploadFileStream(String url, Map<String, String> header, Map<String, String> param, String fileName, byte[] stream) {
        String infoMessage =  new StringBuilder().append("request postUploadFileStream，url:").append(url).append("，header:").append(header.toString()).append("，param:").append(JSONObject.toJSONString(param)).append("，fileName:").append(fileName).toString();
        log.info(infoMessage);
        RequestResult result = new RequestResult();
        if(StringUtils.isBlank(fileName)){
            log.warn("上传文件名称为空");
            throw new RuntimeException("上传文件名称为空");
        }
        if(Objects.isNull(stream)){
            log.warn("上传文件流为空");
            throw new RuntimeException("上传文件流为空");
        }
        CloseableHttpResponse response = null;
        try {
            ContentType contentType = ContentType.MULTIPART_FORM_DATA.withCharset("UTF-8");
            HttpPost httpPost = new HttpPost(url);
            if (Objects.nonNull(header) && !header.isEmpty()) {
                for (Map.Entry<String, String> entry : header.entrySet()) {
                    httpPost.setHeader(entry.getKey(), entry.getValue());
                    if(log.isDebugEnabled()){
                        log.debug(entry.getKey() + ":" + entry.getValue());
                    }
                }
            }
            MultipartEntityBuilder builder = MultipartEntityBuilder.create();
            builder.setCharset(Charset.forName("UTF-8"));//使用UTF-8
            builder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);//设置浏览器兼容模式
            if (Objects.nonNull(param) && !param.isEmpty()) {
                for (Map.Entry<String, String> entry : param.entrySet()) {
                    if (log.isDebugEnabled()) {
                        log.debug(entry.getKey() + ":" + entry.getValue());
                    }
                    builder.addPart(entry.getKey(), new StringBody(entry.getValue(), contentType));
                }
            }
            builder.addBinaryBody("file",  stream, contentType, fileName);//封装上传文件
            httpPost.setEntity(builder.build());
            //请求执行，获取返回对象
            response = httpClient.execute(httpPost);
            log.info("postUploadFileStream response status:{}", response.getStatusLine());
            int statusCode = response.getStatusLine().getStatusCode();
            if (statusCode == HttpStatus.SC_OK || statusCode == HttpStatus.SC_NO_CONTENT) {
                result.setSuccess(true);
            }
            HttpEntity httpEntity = response.getEntity();
            if (Objects.nonNull(httpEntity)) {
                String content = EntityUtils.toString(httpEntity, "UTF-8");
                log.info("postUploadFileStream response body:{}", content);
                result.setMsg(content);
            }

        } catch (Exception e) {
            log.error(infoMessage + " failure", e);
            result.setMsg("请求异常");
        } finally {
            HttpClientUtils.closeQuietly(response);
        }
        return result;
    }

    /**
     * 从下载地址获取文件流(如果链接出现双斜杠，请用OKHttp)
     * @param url
     * @return
     */
    public static ByteArrayOutputStream getDownloadFileStream(String url) {
        String infoMessage = new StringBuilder().append("request getDownloadFileStream，url:").append(url).toString();
        log.info(infoMessage);
        ByteArrayOutputStream byteOutStream = null;
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(new HttpGet(url));
            log.info("getDownloadFileStream response status:{}", response.getStatusLine());
            int statusCode = response.getStatusLine().getStatusCode();
            if (statusCode == HttpStatus.SC_OK) {
                //请求成功
                HttpEntity entity = response.getEntity();
                if (entity != null && entity.getContent() != null) {
                    //复制输入流
                    byteOutStream = cloneInputStream(entity.getContent());
                }
            }
        } catch (Exception e) {
            log.error(infoMessage + " failure", e);
        } finally {
            HttpClientUtils.closeQuietly(response);
        }
        return byteOutStream;
    }
    
    /**
     * 表单请求
     * @param url 地址
     * @param header 请求头部
     * @param param 请求表单
     * @return
     */
    public static RequestResult sendPostForm(String url, Map<String, String> header, Map<String, String> param) {
        String infoMessage =  new StringBuilder().append("request postForm，url:").append(url).append("，header:").append(JacksonUtils.toJson(header)).append("，param:").append(JacksonUtils.toJson(param)).toString();
        if(log.isDebugEnabled()){
            log.debug(infoMessage);
        }
        RequestResult result = new RequestResult();
        CloseableHttpResponse response = null;
        try {
            HttpPost httpPost = new HttpPost(url);
            httpPost.addHeader("Content-type", "application/x-www-form-urlencoded");
            if (Objects.nonNull(header) && !header.isEmpty()) {
                for (Map.Entry<String, String> entry : header.entrySet()) {
                    httpPost.setHeader(entry.getKey(), entry.getValue());
                    if(log.isDebugEnabled()){
                        log.debug(entry.getKey() + ":" + entry.getValue());
                    }
                }
            }
            List<NameValuePair> nameValuePairs = new ArrayList<>();
            if (Objects.nonNull(param) && !param.isEmpty()) {
                for (Map.Entry<String, String> entry : param.entrySet()) {
                    if (log.isDebugEnabled()) {
                        log.debug(entry.getKey() + ":" + entry.getValue());
                    }
                    nameValuePairs.add(new BasicNameValuePair(entry.getKey(), entry.getValue()));
                }
            }
            httpPost.setEntity(new UrlEncodedFormEntity(nameValuePairs, Charset.forName("UTF-8")));
            //请求执行，获取返回对象
            response = httpClient.execute(httpPost);
            if(log.isDebugEnabled()){
                log.debug("postForm response status:{}", response.getStatusLine());
            }
            int statusCode = response.getStatusLine().getStatusCode();
            if (statusCode == HttpStatus.SC_OK || statusCode == HttpStatus.SC_NO_CONTENT) {
                result.setSuccess(true);
            }
            HttpEntity httpEntity = response.getEntity();
            if (Objects.nonNull(httpEntity)) {
                String content = EntityUtils.toString(httpEntity, "UTF-8");
                if(log.isDebugEnabled()){
                    log.debug("postForm response body:{}", content);
                }
                result.setMsg(content);
            }
        } catch (Exception e) {
            log.error(infoMessage + " failure", e);
            result.setMsg("请求异常");
        } finally {
            HttpClientUtils.closeQuietly(response);
        }
        return result;
    }


    /**
     * 发送http请求(通用方法)
     * @param httpMethod 请求方式（GET、POST、PUT、DELETE）
     * @param url        请求路径
     * @param header     请求头
     * @param params     请求body（json数据）
     * @return 响应文本
     */
    public static String sendHttp(HttpMethod httpMethod, String url, Map<String, String> header, String params) {
        String infoMessage = new StringBuilder().append("request sendHttp，url:").append(url).append("，method:").append(httpMethod.name()).append("，header:").append(JSONObject.toJSONString(header)).append("，param:").append(params).toString();
        log.info(infoMessage);
        //返回结果
        String result = null;
        CloseableHttpResponse response = null;
        long beginTime = System.currentTimeMillis();
        try {
            ContentType contentType = ContentType.APPLICATION_JSON.withCharset("UTF-8");
            HttpRequestBase request = buildHttpMethod(httpMethod, url);
            if (Objects.nonNull(header) && !header.isEmpty()) {
                for (Map.Entry<String, String> entry : header.entrySet()) {
                    //打印头部信息
                    if(log.isDebugEnabled()){
                        log.debug(entry.getKey() + ":" + entry.getValue());
                    }
                    request.setHeader(entry.getKey(), entry.getValue());
                }
            }
            if (StringUtils.isNotEmpty(params)) {
                if(HttpMethod.POST.equals(httpMethod) || HttpMethod.PUT.equals(httpMethod)){
                    ((HttpEntityEnclosingRequest) request).setEntity(new StringEntity(params, contentType));
                }
            }
            response = httpClient.execute(request);
            HttpEntity httpEntity = response.getEntity();
            log.info("sendHttp response status:{}", response.getStatusLine());
            if (Objects.nonNull(httpEntity)) {
                result = EntityUtils.toString(httpEntity, "UTF-8");
                log.info("sendHttp response body:{}", result);
            }
        } catch (Exception e) {
            log.error(infoMessage + " failure", e);
        } finally {
            HttpClientUtils.closeQuietly(response);//关闭返回对象
        }
        long endTime = System.currentTimeMillis();
        log.info("request sendHttp response time cost:" + (endTime - beginTime) + " ms");
        return result;
    }

    /**
     * 请求方法（全大些）
     */
    public enum HttpMethod {
        GET, POST, PUT, DELETE
    }

    /**
     * 上传返回结果封装
     */
    public static class RequestResult{
        private boolean isSuccess;//是否成功
        private String msg;//消息

        public boolean isSuccess() {
            return isSuccess;
        }

        public RequestResult setSuccess(boolean success) {
            isSuccess = success;
            return this;
        }

        public String getMsg() {
            return msg;
        }

        public RequestResult setMsg(String msg) {
            this.msg = msg;
            return this;
        }

        public RequestResult() {
            this.isSuccess = false;
        }
    }

    /**
     * 构建请求方法
     * @param method
     * @param url
     * @return
     */
    private static HttpRequestBase buildHttpMethod(HttpMethod method, String url) {
        if (HttpMethod.GET.equals(method)) {
            return new HttpGet(url);
        } else if (HttpMethod.POST.equals(method)) {
            return new HttpPost(url);
        } else if (HttpMethod.PUT.equals(method)) {
            return new HttpPut(url);
        } else if (HttpMethod.DELETE.equals(method)) {
            return new HttpDelete(url);
        } else {
            return null;
        }
    }

    /**
     * 配置证书
     * @return
     */
    private static SSLContext createSSLContext(){
        try {
            //信任所有,支持导入ssl证书
            TrustStrategy acceptingTrustStrategy = (cert, authType) -> true;
            SSLContext sslContext = SSLContexts.custom().loadTrustMaterial(null, acceptingTrustStrategy).build();
            return sslContext;
        } catch (Exception e) {
            log.error("初始化ssl配置失败", e);
            throw new RuntimeException("初始化ssl配置失败");
        }
    }

    /**
     * 复制文件流
     * @param input
     * @return
     */
    private static ByteArrayOutputStream cloneInputStream(InputStream input) {
        try {
            ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int len;
            while ((len = input.read(buffer)) > -1) {
                byteOutStream.write(buffer, 0, len);
            }
            byteOutStream.flush();
            return byteOutStream;
        } catch (IOException e) {
            log.warn("copy InputStream error，{}", ExceptionUtils.getStackTrace(e));
        }
        return null;
    }

    /**
     * 输入流转字节流
     * @param in
     * @return
     */
    private static byte[] inputStream2byte(InputStream in) {
        ByteArrayOutputStream bos = null;
        try {
            bos = new ByteArrayOutputStream();
            byte[] b = new byte[1024];
            int n;
            while ((n = in.read(b)) != -1) {
                bos.write(b, 0, n);
            }
            in.close();
            bos.close();
            byte[] buffer = bos.toByteArray();
            return buffer;
        } catch (IOException e) {
            log.warn("inputStream transfer byte error，{}", ExceptionUtils.getStackTrace(e));
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (IOException e) {
                log.error("clone inputStream error", e);
            }
            try {
                if (bos != null) {
                    bos.close();
                }
            } catch (IOException e) {
                log.error("clone outputStream error", e);
            }
        }
        return null;
    }

    public static void main(String[] args) {
        String url = "https://101.231.204.80:5000/gateway/api/queryTrans.do";
        String result = postJson(url);
        System.out.println(result);
    }
}
```
除了上传、下载请求之外，默认封装的请求参数格式都是application/json，如果不够，可以根据自己的业务场景进行封装处理！<br />其中`sendHttp`是一个支持GET、POST、PUT、DELETE请求的通用方法，上面介绍的`getUrl`、`postJosn`等方法，最终都会调用到这个方法！
<a name="vNgjZ"></a>
### 2.3、接口请求示例
工具包封装完成之后，在代码中使用起来也非常简单，直接采用工具类方法就可以直接使用，例如下面以post方式请求某个接口！
```java
public static void main(String[] args) throws Exception {
    String url = "https://101.231.204.80:5000/gateway/api/queryTrans.do";
    String result= HttpUtils.postJson(url);
    System.out.println(result);
}
```
<a name="Iziib"></a>
## 三、小结
在编写工具类的时候，需要注意的地方是，尽可能保证httpClient客户端全局唯一，也就是采用单利模式，如果每次请求都初始化一个客户端，结束之后又将其关闭，在高并发的接口请求场景下，性能效率急剧下降！<br />HttpClients客户端的初始化参数配置非常丰富，这里默认初始化的线程池为300，在实际的业务开发中，大家还可以结合自己的业务场景进行调优，具体的配置可以参考官网文档，地址：Apache HttpComponents
