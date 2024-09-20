JavaHTTP
<a name="CDUTp"></a>
## **一、概述**
在实际开发过程中，经常需要调用对方提供的接口或者自己写的接口是否有接口。短信、天气等）。<br />Java项目中调用接口的方式有：

- 通过JDK网络类Java.net.HttpURLConnection；
- 通过common封装好HttpClient；
- 通过 Apache 封装好`CloseableHttpClient`；
- 通过SpringBoot-RestTemplate；
<a name="zuw80"></a>
## **二、Java调用http接口的方式**
<a name="G1EnW"></a>
### 2.1 通过JDK网络类`Java.net.HttpURLConnection`
原始比较的一种调用方法，这里get请求和post请求都可以找到一个方法里面。<br />实现过程：<br />**Get：**

1. 创建远程连接
2. 设置连接方式（get、post、put。。。）
3. 设置连接时间
4. 设置响应读取时间
5. 发起请求
6. 获取请求数据
7. 关闭连接

**Post：**

1. 创建远程连接
2. 设置连接方式（get、post、put。。。）
3. 设置连接时间
4. 设置响应读取时间
5. 当向远程服务器传送数据/写入数据时，需要设置为true（`setDoOutput`）
6. 当前向远程服务读取数据时，设置为true，该参数可有可无（`setDoInput`）
7. 设置参数格式的属性：（请求参数）
8. 设置鉴权信息：授权：（`setRequestProperty`）
9. 参数设置
10. 发起请求
11. 获取请求数据
12. 关闭连接

直接上代码：
```java
package com.fcant.springbootdemo.util.common.httpConnectionUtil;
 
import org.springframework.lang.Nullable;
 
import java.io.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

public class HttpURLConnectionUtil {
 
    /**
     * Http get请求
     * @param httpUrl 连接
     * @return 响应数据
     */
    public static String doGet(String httpUrl){
        //链接
        HttpURLConnection connection = null;
        InputStream is = null;
        BufferedReader br = null;
        StringBuffer result = new StringBuffer();
        try {
            //创建连接
            URL url = new URL(httpUrl);
            connection = (HttpURLConnection) url.openConnection();
            //设置请求方式
            connection.setRequestMethod("GET");
            //设置连接超时时间
            connection.setReadTimeout(15000);
            //开始连接
            connection.connect();
            //获取响应数据
            if (connection.getResponseCode() == 200) {
                //获取返回的数据
                is = connection.getInputStream();
                if (null != is) {
                    br = new BufferedReader(new InputStreamReader(is, "UTF-8"));
                    String temp = null;
                    while (null != (temp = br.readLine())) {
                        result.append(temp);
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (null != br) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (null != is) {
                try {
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            //关闭远程连接
            connection.disconnect();
        }
        return result.toString();
    }
 
    /**
     * Http post请求
     * @param httpUrl 连接
     * @param param 参数
     * @return
     */
    public static String doPost(String httpUrl, @Nullable String param) {
        StringBuffer result = new StringBuffer();
        //连接
        HttpURLConnection connection = null;
        OutputStream os = null;
        InputStream is = null;
        BufferedReader br = null;
        try {
            //创建连接对象
            URL url = new URL(httpUrl);
            //创建连接
            connection = (HttpURLConnection) url.openConnection();
            //设置请求方法
            connection.setRequestMethod("POST");
            //设置连接超时时间
            connection.setConnectTimeout(15000);
            //设置读取超时时间
            connection.setReadTimeout(15000);
            //DoOutput设置是否向httpUrlConnection输出，DoInput设置是否从httpUrlConnection读入，此外发送post请求必须设置这两个
            //设置是否可读取
            connection.setDoOutput(true);
            connection.setDoInput(true);
            //设置通用的请求属性
            connection.setRequestProperty("accept", "*/*");
            connection.setRequestProperty("connection", "Keep-Alive");
            connection.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
            connection.setRequestProperty("Content-Type", "application/json;charset=utf-8");
 
            //拼装参数
            if (null != param && param.equals("")) {
                //设置参数
                os = connection.getOutputStream();
                //拼装参数
                os.write(param.getBytes("UTF-8"));
            }
            //设置权限
            //设置请求头等
            //开启连接
            //connection.connect();
            //读取响应
            if (connection.getResponseCode() == 200) {
                is = connection.getInputStream();
                if (null != is) {
                    br = new BufferedReader(new InputStreamReader(is, "GBK"));
                    String temp = null;
                    while (null != (temp = br.readLine())) {
                        result.append(temp);
                        result.append("\r\n");
                    }
                }
            }
 
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //关闭连接
            if(br!=null){
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(os!=null){
                try {
                    os.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if(is!=null){
                try {
                    is.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            //关闭连接
            connection.disconnect();
        }
        return result.toString();
    }
 
    public static void main(String[] args) {
        String message = doPost("https://tcc.taobao.com/cc/json/mobile_tel_segment.htm?tel=13026194071", "");
        System.out.println(message);
    }
}
```
<a name="Tmbvs"></a>
### 2.2 通过apache通用封装好的HttpClient
httpClient的get或post请求方式步骤：

1. 生成一个`HttpClient`对象并设置相应的参数；
2. 生成一个`GetMethod`对象或`PostMethod`并设置响应的参数；
3. 用`HttpClient`生成的对象来执行`GetMethod`生成的Get方法；
4. 处理响应状态码；
5. 如果正常响应，处理HTTP响应内容；
6. 释放连接。

导入以下jar包：
```xml
<!--HttpClient-->
<dependency>
    <groupId>commons-httpclient</groupId>
    <artifactId>commons-httpclient</artifactId>
    <version>3.1</version>
</dependency>
 
<!--fastjson-->
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.32</version>
</dependency>
```
代码如下：
```java
package com.fcant.springbootdemo.util.common.httpConnectionUtil;
 
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.httpclient.*;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;
 
import java.io.IOException;

public class HttpClientUtil {
    /**
     * httpClient的get请求方式
     * 使用GetMethod来访问一个URL对应的网页实现步骤：
     * 1.生成一个HttpClient对象并设置相应的参数；
     * 2.生成一个GetMethod对象并设置响应的参数；
     * 3.用HttpClient生成的对象来执行GetMethod生成的Get方法；
     * 4.处理响应状态码；
     * 5.若响应正常，处理HTTP响应内容；
     * 6.释放连接。
     * @param url
     * @param charset
     * @return
     */
    public static String doGet(String url, String charset) {
        //1.生成HttpClient对象并设置参数
        HttpClient httpClient = new HttpClient();
        //设置Http连接超时为5秒
        httpClient.getHttpConnectionManager().getParams().setConnectionTimeout(5000);
        //2.生成GetMethod对象并设置参数
        GetMethod getMethod = new GetMethod(url);
        //设置get请求超时为5秒
        getMethod.getParams().setParameter(HttpMethodParams.SO_TIMEOUT, 5000);
        //设置请求重试处理，用的是默认的重试处理：请求三次
        getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER, new DefaultHttpMethodRetryHandler());
        String response = "";
        //3.执行HTTP GET 请求
        try {
            int statusCode = httpClient.executeMethod(getMethod);
            //4.判断访问的状态码
            if (statusCode != HttpStatus.SC_OK) {
                System.err.println("请求出错：" + getMethod.getStatusLine());
            }
            //5.处理HTTP响应内容
            //HTTP响应头部信息，这里简单打印
            Header[] headers = getMethod.getResponseHeaders();
            for(Header h : headers) {
                System.out.println(h.getName() + "---------------" + h.getValue());
            }
            //读取HTTP响应内容，这里简单打印网页内容
            //读取为字节数组
            byte[] responseBody = getMethod.getResponseBody();
            response = new String(responseBody, charset);
            System.out.println("-----------response:" + response);
            //读取为InputStream，在网页内容数据量大时候推荐使用
            //InputStream response = getMethod.getResponseBodyAsStream();
        } catch (HttpException e) {
            //发生致命的异常，可能是协议不对或者返回的内容有问题
            System.out.println("请检查输入的URL!");
            e.printStackTrace();
        } catch (IOException e) {
            //发生网络异常
            System.out.println("发生网络异常!");
        } finally {
            //6.释放连接
            getMethod.releaseConnection();
        }
        return response;
    }
 
    /**
     * post请求
     * @param url
     * @param json
     * @return
     */
    public static String doPost(String url, JSONObject json){
        HttpClient httpClient = new HttpClient();
        PostMethod postMethod = new PostMethod(url);
 
        postMethod.addRequestHeader("accept", "*/*");
        postMethod.addRequestHeader("connection", "Keep-Alive");
        //设置json格式传送
        postMethod.addRequestHeader("Content-Type", "application/json;charset=GBK");
        //必须设置下面这个Header
        postMethod.addRequestHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.81 Safari/537.36");
        //添加请求参数
        postMethod.addParameter("commentId", json.getString("commentId"));
 
        String res = "";
        try {
            int code = httpClient.executeMethod(postMethod);
            if (code == 200){
                res = postMethod.getResponseBodyAsString();
                System.out.println(res);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return res;
    }
 
    public static void main(String[] args) {
        System.out.println(doGet("http://tcc.taobao.com/cc/json/mobile_tel_segment.htm?tel=13026194071", "GBK"));
        System.out.println("-----------分割线------------");
        System.out.println("-----------分割线------------");
        System.out.println("-----------分割线------------");
 
        JSONObject jsonObject = new JSONObject();
        jsonObject.put("commentId", "13026194071");
        System.out.println(doPost("http://tcc.taobao.com/cc/json/mobile_tel_segment.htm?tel=13026194071", jsonObject));
    }
}
```
post请求的`jsonObject`参数也成功写入
<a name="xQYGt"></a>
### 2.3 通过Apache封装好的`CloseableHttpClient`
`CloseableHttpClient`是在`HttpClient`的基础上修改可以更新而来的，这里还涉及到请求头的设置（请求），利用fastjson转换请求或返回请求结果字符串为json格式，当然方式也是设置头token、 json的，这里只在下面说明。<br />导入以下jar包：
```xml
<!--CloseableHttpClient-->
<dependency>
    <groupId>org.apache.httpcomponents</groupId>
    <artifactId>httpclient</artifactId>
    <version>4.5.2</version>
</dependency>
 
<!--fastjson-->
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.32</version>
</dependency>
```
代码如下：
```java
package com.fcant.springbootdemo.util.common.httpConnectionUtil;
 
import com.alibaba.fastjson.JSONObject;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
 
import java.io.IOException;
import java.io.UnsupportedEncodingException;

public class CloseableHttpClientUtil {
 
    private static String tokenString = "";
    private static String AUTH_TOKEN_EXPIRED = "AUTH_TOKEN_EXPIRED";
    private static CloseableHttpClient httpClient = null;
 
    /**
     * 以get方式调用第三方接口
     * @param url
     * @param token
     * @return
     */
    public static String doGet(String url, String token) {
        //创建HttpClient对象
        CloseableHttpClient httpClient = HttpClientBuilder.create().build();
        HttpGet httpGet = new HttpGet(url);
        if (null != tokenString && !tokenString.equals("")) {
            tokenString = getToken();
        }
        //api_gateway_auth_token自定义header头，用于token验证使用
        httpGet.addHeader("api_gateway_auth_token",tokenString);
        httpGet.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.81 Safari/537.36");
        try {
            HttpResponse response = httpClient.execute(httpGet);
            if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                //返回json格式
                String res = EntityUtils.toString(response.getEntity());
                return res;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
 
    /**
     * 以post方式调用第三方接口
     * @param url
     * @param json
     * @return
     */
    public static String doPost(String url, JSONObject json) {
        if (null == httpClient) {
            httpClient = HttpClientBuilder.create().build();
        }
        HttpPost httpPost = new HttpPost(url);
        if (null != tokenString && tokenString.equals("")) {
            tokenString = getToken();
        }
        //api_gateway_auth_token自定义header头，用于token验证使用
        httpPost.addHeader("api_gateway_auth_token", tokenString);
        httpPost.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.81 Safari/537.36");
        try {
            StringEntity se = new StringEntity(json.toString());
            se.setContentEncoding("UTF-8");
            //发送json数据需要设置contentType
            se.setContentType("application/x-www-form-urlencoded");
            //设置请求参数
            httpPost.setEntity(se);
            HttpResponse response = httpClient.execute(httpPost);
            if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
                //返回json格式
                String res = EntityUtils.toString(response.getEntity());
                return res;
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (httpClient != null){
                try {
                    httpClient.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return null;
    }
 
    /**
     * 获取第三方接口的token
     */
    public static String getToken() {
        String token = "";
        JSONObject object = new JSONObject();
        object.put("appid", "appid");
        object.put("secretkey", "secretkey");
        if (null == httpClient) {
            httpClient = HttpClientBuilder.create().build();
        }
        HttpPost httpPost = new HttpPost("http://localhost/login");
        httpPost.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.81 Safari/537.36");
        try {
            StringEntity se = new StringEntity(object.toString());
            se.setContentEncoding("UTF-8");
            //发送json数据需要设置contentType
            se.setContentType("application/x-www-form-urlencoded");
            //设置请求参数
            httpPost.setEntity(se);
            HttpResponse response = httpClient.execute(httpPost);
            //这里可以把返回的结果按照自定义的返回数据结果，把string转换成自定义类
            //ResultTokenBO result = JSONObject.parseObject(response, ResultTokenBO.class);
            //把response转为jsonObject
            JSONObject result = (JSONObject) JSONObject.parseObject(String.valueOf(response));
            if (result.containsKey("token")) {
                token = result.getString("token");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return token;
    }
 
    /**
     * 测试
     */
    public static void test(String telephone) {
 
        JSONObject object = new JSONObject();
        object.put("telephone", telephone);
 
        //首先获取token
        tokenString = getToken();
        String response = doPost("http://localhost/searchUrl", object);
        //如果返回的结果是list形式的，需要使用JSONObject.parseArray转换
        //List<Result> list = JSONObject.parseArray(response, Result.class);
        System.out.println(response);
    }
 
    public static void main(String[] args) {
        test("12345678910");
    }
}
```
<a name="ycGjJ"></a>
### 2.4 通过SpringBoot-RestTemplate
springBoot-RestTemple是这些代码的集合的大成者，编写简单，目前可以采用的调用接口有：

- `**delete()**` HTTP DELETE在特定操作的 URL 上对资源执行
- `**exchange()**` 在URL上执行特定的HTTP方法，返回包含对象的`ResponseEntity`，这个对象是从请求体中映射到的
- `**execute()**` 在上执行特定的HTTP方法，返回一个从响应体映射到的对象
- `**get()**` 发送一个HTTP GET请求返回的`ResponseEntity`包含了响应体，映射成的对象
- `**getForObject()**` 发送HTTP GET请求，返回的请求体将映射为一个对象
- `**postForEntity()**` POST 数据到一个URL，返回包含一个对象的`ResponseEntity`，这个对象是从响应体中映射的
- `**postForObject()**` POST 数据到一个 URL，返回根据响应体形成的对象
- `**headForHeaders()**` 发送HTTP HEAD请求，返回包含特定资源URL的HTTP头
- `**optionsForAllow()**` 发送HTTP OPTIONS请求，返回对特定URL的Allow头信息
- `**postForLocation()**` POST 数据到一个 URL，返回新创建资源的 URL
- `**put()**` PUT 资源到特定的 URL

首先导入springboot的web包
```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.0.4.RELEASE</version>
</parent>

<dependencies>
    <!--CloseableHttpClient-->
    <dependency>
        <groupId>org.apache.httpcomponents</groupId>
        <artifactId>httpclient</artifactId>
        <version>4.5.2</version>
    </dependency>

    <!--spring restTemplate-->
    <!-- @ConfigurationProperties annotation processing (metadata for IDEs)
            生成spring-configuration-metadata.json类，需要引入此类-->
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-configuration-processor</artifactId>
        <optional>true</optional>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-aop</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
        <exclusions>
            <exclusion>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-starter-tomcat</artifactId>
            </exclusion>
        </exclusions>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-jetty</artifactId>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-test</artifactId>
        <scope>test</scope>
    </dependency>
</dependencies>
```
在启动类同包下创建RestTemplateConfig.java类
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.client.ClientHttpRequestFactory;
import org.springframework.http.client.SimpleClientHttpRequestFactory;
import org.springframework.web.client.RestTemplate;

@Configuration
public class RestTemplateConfig {
 
    @Bean
    public RestTemplate restTemplate(ClientHttpRequestFactory factory){
        return new RestTemplate(factory);
    }
 
    @Bean
    public ClientHttpRequestFactory simpleClientHttpRequestFactory(){
        SimpleClientHttpRequestFactory factory = new SimpleClientHttpRequestFactory();
        factory.setConnectTimeout(15000);
        factory.setReadTimeout(5000);
        return factory;
    }
}
```
然后在Service类（`RestTemplateToInterface`）中注入使用<br />具体代码如下：
```java
import com.alibaba.fastjson.JSONObject;
import com.swordfall.model.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.*;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

@Service
public class RestTemplateToInterface {
 
    @Autowired
    private RestTemplate restTemplate;
 
    /**
     * 以get方式请求第三方http接口 getForEntity
     * @param url
     * @return
     */
    public User doGetWith1(String url){
        ResponseEntity<User> responseEntity = restTemplate.getForEntity(url, User.class);
        User user = responseEntity.getBody();
        return user;
    }
 
    /**
     * 以get方式请求第三方http接口 getForObject
     * 返回值返回的是响应体，省去了我们再去getBody()
     * @param url
     * @return
     */
    public User doGetWith2(String url){
        User user  = restTemplate.getForObject(url, User.class);
        return user;
    }
 
    /**
     * 以post方式请求第三方http接口 postForEntity
     * @param url
     * @return
     */
    public String doPostWith1(String url){
        User user = new User("小白", 20);
        ResponseEntity<String> responseEntity = restTemplate.postForEntity(url, user, String.class);
        String body = responseEntity.getBody();
        return body;
    }
 
    /**
     * 以post方式请求第三方http接口 postForEntity
     * @param url
     * @return
     */
    public String doPostWith2(String url){
        User user = new User("小白", 20);
        String body = restTemplate.postForObject(url, user, String.class);
        return body;
    }
 
    /**
     * exchange
     * @return
     */
    public String doExchange(String url, Integer age, String name){
        //header参数
        HttpHeaders headers = new HttpHeaders();
        String token = "asdfaf2322";
        headers.add("authorization", token);
        headers.setContentType(MediaType.APPLICATION_JSON);
 
        //放入body中的json参数
        JSONObject obj = new JSONObject();
        obj.put("age", age);
        obj.put("name", name);
 
        //组装
        HttpEntity<JSONObject> request = new HttpEntity<>(obj, headers);
        ResponseEntity<String> responseEntity = restTemplate.exchange(url, HttpMethod.POST, request, String.class);
        String body = responseEntity.getBody();
        return body;
    }
}
```
<a name="lCaLK"></a>
### 2.5 通过okhttp
现在也是蛮流行的，基于手机端很火的，这里分享一下OkHttpClient客户端，业务代码get、post请求直接调用http就好了。<br />pom文件引入依赖包
```xml
<dependency>
    <groupId>com.squareup.okhttp3</groupId>
    <artifactId>okhttp</artifactId>
    <version>3.10.0</version>
</dependency>
```
```java
@Slf4j
public class OkHttpClient {
    private static final MediaType JSON = MediaType.parse("application/json; charset=utf-8");
 
    private volatile static okhttp3.OkHttpClient client;
 
    private static final int MAX_IDLE_CONNECTION = Integer
            .parseInt(ConfigManager.get("httpclient.max_idle_connection"));
 
    private static final long KEEP_ALIVE_DURATION = Long
            .parseLong(ConfigManager.get("httpclient.keep_alive_duration"));
 
    private static final long CONNECT_TIMEOUT = Long.parseLong(ConfigManager.get("httpclient.connectTimeout"));
 
    private static final long READ_TIMEOUT = Long.parseLong(ConfigManager.get("httpclient. "));
 
    /**
     * 单例模式(双重检查模式) 获取类实例
     *
     * @return client
     */
    private static okhttp3.OkHttpClient getInstance() {
        if (client == null) {
            synchronized (okhttp3.OkHttpClient.class) {
                if (client == null) {
                    client = new okhttp3.OkHttpClient.Builder()
                            .connectTimeout(CONNECT_TIMEOUT, TimeUnit.SECONDS)
                            .readTimeout(READ_TIMEOUT, TimeUnit.SECONDS)
                            .connectionPool(new ConnectionPool(MAX_IDLE_CONNECTION, KEEP_ALIVE_DURATION,
                                    TimeUnit.MINUTES))
                            .build();
                }
            }
        }
        return client;
    }
 
    public static String syncPost(String url, String json) throws IOException {
        RequestBody body = RequestBody.create(JSON, json);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .build();
        try {
            Response response = OkHttpClient.getInstance().newCall(request).execute();
            if (response.isSuccessful()) {
                String result = response.body().string();
                log.info("syncPost response = {}, responseBody= {}", response, result);
                return result;
            }
            String result = response.body().string();
            log.info("syncPost response = {}, responseBody= {}", response, result);
            throw new IOException("三方接口返回http状态码为" + response.code());
        } catch (Exception e) {
            log.error("syncPost() url:{} have a ecxeption {}", url, e);
            throw new RuntimeException("syncPost() have a ecxeption {}" + e.getMessage());
        }
    }
 
    public static String syncGet(String url, Map<String, Object> headParamsMap) throws IOException {
        Request request;
        final Request.Builder builder = new Request.Builder().url(url);
        try {
            if (!CollectionUtils.isEmpty(headParamsMap)) {
                final Iterator<Map.Entry<String, Object>> iterator = headParamsMap.entrySet()
                        .iterator();
                while (iterator.hasNext()) {
                    final Map.Entry<String, Object> entry = iterator.next();
                    builder.addHeader(entry.getKey(), (String) entry.getValue());
                }
            }
            request = builder.build();
            Response response = OkHttpClient.getInstance().newCall(request).execute();
            String result = response.body().string();
            log.info("syncGet response = {},responseBody= {}", response, result);
            if (!response.isSuccessful()) {
                throw new IOException("三方接口返回http状态码为" + response.code());
            }
            return result;
        } catch (Exception e) {
            log.error("remote interface url:{} have a ecxeption {}", url, e);
            throw new RuntimeException("三方接口返回异常");
        }
    }
 
}
```
