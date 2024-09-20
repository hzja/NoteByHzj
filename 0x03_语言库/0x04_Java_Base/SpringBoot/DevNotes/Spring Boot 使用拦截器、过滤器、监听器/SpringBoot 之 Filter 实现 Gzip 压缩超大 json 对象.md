JavaSpringBoot
<a name="RSe7u"></a>
### 简介
在项目中，存在传递超大 json 数据的场景。直接传输超大 json 数据的话，有以下两个弊端

- 占用网络带宽，而有些云产品就是按照带宽来计费的，间接浪费了钱
- 传输数据大导致网络传输耗时较长 为了避免直接传输超大 json 数据，可以对 json 数据进行 Gzip 压缩后，再进行网络传输。
- 请求头添加 Content-Encoding 标识，传输的数据进行过压缩
- Servlet Filter 拦截请求，对压缩过的数据进行解压
- `HttpServletRequestWrapper` 包装，把解压的数据写入请求体
<a name="uLZze"></a>
### pom.xml 引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.olive</groupId>
  <artifactId>request-uncompression</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>request-uncompression</name>
  <url>http://maven.apache.org</url>

  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.5.14</version>
    <relativePath /> <!-- lookup parent from repository -->
    </parent>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>8</maven.compiler.source>
    <maven.compiler.target>8</maven.compiler.target>
  </properties>

  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.projectlombok</groupId>
      <artifactId>lombok</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <dependency>
      <groupId>com.alibaba.fastjson2</groupId>
      <artifactId>fastjson2</artifactId>
      <version>2.0.14</version>
    </dependency>
    <dependency>
      <groupId>commons-io</groupId>
      <artifactId>commons-io</artifactId>
      <version>2.9.0</version>
    </dependency>
  </dependencies>
</project>
```
<a name="WOX51"></a>
### 创建压缩工具类
GzipUtils 类提供压缩解压相关方法
```java
package com.olive.utils;

import com.alibaba.fastjson2.JSON;
import com.olive.vo.ArticleRequestVO;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FileUtils;

import java.io.*;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

@Slf4j
public class GzipUtils {

    private static final String GZIP_ENCODE_UTF_8 = "UTF-8";

    /**
     * 字符串压缩为GZIP字节数组
     *
     * @param str
     * @return
     */
    public static byte[] compress(String str) {
        return compress(str, GZIP_ENCODE_UTF_8);
    }

    /**
     * 字符串压缩为GZIP字节数组
     *
     * @param str
     * @param encoding
     * @return
     */
    public static byte[] compress(String str, String encoding) {
        if (str == null || str.length() == 0) {
            return null;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        GZIPOutputStream gzip = null;
        try {
            gzip = new GZIPOutputStream(out);
            gzip.write(str.getBytes(encoding));
        } catch (IOException e) {
           log.error("compress>>", e);
        }finally {
            if(gzip!=null){
                try {
                    gzip.close();
                } catch (IOException e) {
                }
            }
        }
        return out.toByteArray();
    }

    /**
     * GZIP解压缩
     *
     * @param bytes
     * @return
     */
    public static byte[] uncompress(byte[] bytes) {
        if (bytes == null || bytes.length == 0) {
            return null;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        ByteArrayInputStream in = new ByteArrayInputStream(bytes);
        GZIPInputStream unGzip = null;
        try {
            unGzip = new GZIPInputStream(in);
            byte[] buffer = new byte[256];
            int n;
            while ((n = unGzip.read(buffer)) >= 0) {
                out.write(buffer, 0, n);
            }
        } catch (IOException e) {
            log.error("uncompress>>", e);
        }finally {
            if(unGzip!=null){
                try {
                    unGzip.close();
                } catch (IOException e) {
                }
            }
        }
        return out.toByteArray();
    }

    /**
     * 解压并返回String
     *
     * @param bytes
     * @return
     */
    public static String uncompressToString(byte[] bytes) throws IOException {
        return uncompressToString(bytes, GZIP_ENCODE_UTF_8);
    }

    /**
     * @param bytes
     * @return
     */
    public static byte[] uncompressToByteArray(byte[] bytes) throws IOException {
        return uncompressToByteArray(bytes, GZIP_ENCODE_UTF_8);
    }

    /**
     * 解压成字符串
     *
     * @param bytes    压缩后的字节数组
     * @param encoding 编码方式
     * @return 解压后的字符串
     */
    public static String uncompressToString(byte[] bytes, String encoding) throws IOException {
        byte[] result = uncompressToByteArray(bytes, encoding);
        return new String(result);
    }

    /**
     * 解压成字节数组
     *
     * @param bytes
     * @param encoding
     * @return
     */
    public static byte[] uncompressToByteArray(byte[] bytes, String encoding) throws IOException {
        if (bytes == null || bytes.length == 0) {
            return null;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        ByteArrayInputStream in = new ByteArrayInputStream(bytes);
        GZIPInputStream unGzip = null;
        try {
            unGzip = new GZIPInputStream(in);
            byte[] buffer = new byte[256];
            int n;
            while ((n = unGzip.read(buffer)) >= 0) {
                out.write(buffer, 0, n);
            }
            return out.toByteArray();
        } catch (IOException e) {
            log.error("uncompressToByteArray>>", e);
            throw new IOException("解压缩失败！");
        }finally {
            if(unGzip!=null){
                unGzip.close();
            }
        }
    }

    /**
     * 将字节流转换成文件
     *
     * @param filename
     * @param data
     * @throws Exception
     */
    public static void saveFile(String filename, byte[] data) throws Exception {
        FileOutputStream fos = null;
        try {
            if (data != null) {
                String filepath = "/" + filename;
                File file = new File(filepath);
                if (file.exists()) {
                    file.delete();
                }
                fos = new FileOutputStream(file);
                fos.write(data, 0, data.length);
                fos.flush();
                System.out.println(file);
            }
        }catch (Exception e){
            throw e;
        }finally {
            if(fos!=null){
                fos.close();
            }
        }
    }

}
```
<a name="oi1zy"></a>
### 对Request进行包装
`UnZipRequestWrapper` 读取输入流，然进行解压；解压完后，再把解压出来的数据封装到输入流中。
```java
package com.olive.filter;

import com.olive.utils.GzipUtils;
import lombok.extern.slf4j.Slf4j;

import javax.servlet.ReadListener;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.io.*;

/**
 *  Json String 经过压缩后保存为二进制文件 -> 解压缩后还原成 Jso nString转换成byte[]写回body中
 */
@Slf4j
public class UnZipRequestWrapper extends HttpServletRequestWrapper {

    private final byte[] bytes;

    public UnZipRequestWrapper(HttpServletRequest request) throws IOException {
        super(request);
        try (BufferedInputStream bis = new BufferedInputStream(request.getInputStream());
             ByteArrayOutputStream baos = new ByteArrayOutputStream()) {
            final byte[] body;
            byte[] buffer = new byte[1024];
            int len;
            while ((len = bis.read(buffer)) > 0) {
                baos.write(buffer, 0, len);
            }
            body = baos.toByteArray();
            if (body.length == 0) {
                log.info("Body无内容，无需解压");
                bytes = body;
                return;
            }
            this.bytes = GzipUtils.uncompressToByteArray(body);
        } catch (IOException ex) {
            log.error("解压缩步骤发生异常！", ex);
            throw ex;
        }
    }

    @Override
    public ServletInputStream getInputStream() throws IOException {
        final ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        return new ServletInputStream() {

            @Override
            public boolean isFinished() {
                return false;
            }

            @Override
            public boolean isReady() {
                return false;
            }

            @Override
            public void setReadListener(ReadListener readListener) {

            }

            public int read() throws IOException {
                return byteArrayInputStream.read();
            }
        };
    }

    @Override
    public BufferedReader getReader() throws IOException {
        return new BufferedReader(new InputStreamReader(this.getInputStream()));
    }

}
```
<a name="OdwUQ"></a>
### 定义GzipFilter对请求进行拦截
GzipFilter 拦截器根据请求头是否包含Content-Encoding=application/gzip，如果包含就对数据进行解压；否则就直接放过。
```java
package com.olive.filter;

import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

/**
 * 解压filter
 */
@Slf4j
@Component
public class GzipFilter implements Filter {

    private static final String CONTENT_ENCODING = "Content-Encoding";

    private static final String CONTENT_ENCODING_TYPE = "application/gzip";

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        log.info("init GzipFilter");
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        long start = System.currentTimeMillis();
        HttpServletRequest httpServletRequest = (HttpServletRequest)request;
        String encodeType = httpServletRequest.getHeader(CONTENT_ENCODING);
        if (encodeType!=null && CONTENT_ENCODING_TYPE.equals(encodeType)) {
            log.info("请求:{} 需要解压", httpServletRequest.getRequestURI());
            UnZipRequestWrapper unZipRequest = new UnZipRequestWrapper(httpServletRequest);
            chain.doFilter(unZipRequest, response);
        }else {
            log.info("请求:{} 无需解压", httpServletRequest.getRequestURI());
            chain.doFilter(request,response);
        }
        log.info("耗时：{}ms", System.currentTimeMillis() - start);
    }

    @Override
    public void destroy() {
        log.info("destroy GzipFilter");
    }
}
```
<a name="keMNc"></a>
### 注册 GzipFilter 拦截器
```java
package com.olive.config;


import com.olive.filter.GzipFilter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * 注册filter
 */
@Configuration
public class FilterRegistration {

    @Autowired
    private GzipFilter gzipFilter;

    @Bean
    public FilterRegistrationBean<GzipFilter> gzipFilterRegistrationBean() {
        FilterRegistrationBean<GzipFilter> registration = new FilterRegistrationBean<>();
        //Filter可以new，也可以使用依赖注入Bean
        registration.setFilter(gzipFilter);
        //过滤器名称
        registration.setName("gzipFilter");
        //拦截路径
        registration.addUrlPatterns("/*");
        //设置顺序
        registration.setOrder(1);
        return registration;
    }
}
```
<a name="XjDEz"></a>
### 定义 Controller
该 Controller 非常简单，主要是输入请求的数据
```java
package com.olive.controller;

import java.util.HashMap;
import java.util.Map;

import com.alibaba.fastjson2.JSON;
import com.olive.vo.ArticleRequestVO;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    @RequestMapping("/getArticle")
    public Map<String, Object> getArticle(@RequestBody ArticleRequestVO articleRequestVO){
        Map<String, Object> result = new HashMap<>();
        result.put("code", 200);
        result.put("msg", "success");
        System.out.println(JSON.toJSONString(articleRequestVO));
        return result;
    }

}
```
Controller 参数接收VO
```java
package com.olive.vo;

import lombok.Data;

import java.io.Serializable;

@Data
public class ArticleRequestVO implements Serializable {

    private Long id;
    
    private String title;
    
    private String content;

}
```
<a name="aoAw3"></a>
### 定义 Springboot 引导类
```java
package com.olive;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class);
    }

}
```
<a name="LP9IG"></a>
### 测试

- 非压缩请求测试
```bash
curl -X POST \
  http://127.0.0.1:8080/getArticle \
  -H 'content-type: application/json' \
  -d '{
 "id":1,
 "title": "java",
 "content":"xxxxxxxxxx"
}'
```

- 压缩请求测试

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665124930082-22bb6a20-300c-4c6e-8d25-41c1cb50ae44.png#averageHue=%23f9f6f6&clientId=ua7569319-3222-4&from=paste&id=udc33e6e4&originHeight=475&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u688c738f-ad7c-4888-b3fb-68bef63f7c8&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665124930033-a7f6a2b9-dca7-407a-a6f2-5f712bc81cb7.png#averageHue=%23f9f8f7&clientId=ua7569319-3222-4&from=paste&id=u6330423a&originHeight=474&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue3033ed2-bca6-46fe-bae1-1fa22433643&title=)<br />不要直接将压缩后的 `byte[]` 数组当作字符串进行传输，否则压缩后的请求数据比没压缩后的还要大得多！ 项目中一般采用以下两种传输压缩后的 `byte[]` 的方式：

- 将压缩后的 `byet[]` 进行 Base64 编码再传输字符串，这种方式会损失掉一部分 GZIP 的压缩效果，适用于压缩结果要存储在 Redis 中的情况
- 将压缩后的 `byte[]` 以二进制的形式写入到文件中，请求时直接在 body 中带上文件即可，用这种方式可以不损失压缩效果

测试采用第二种方式，采用以下代码把原始数据进行压缩
```java
public static void main(String[] args) {
    ArticleRequestVO vo = new ArticleRequestVO();
    vo.setId(1L);
    vo.setTitle("bug");
    try {
        byte[] bytes = FileUtils.readFileToByteArray(new File("C:\\Users\\2230\\Desktop\\资料\\项目\\CXSSBOOT_DB_DDL-1.0.9.sql"));
        vo.setContent(new String(bytes));
        byte[] dataBytes = compress(JSON.toJSONString(vo));
        saveFile("d:/vo.txt", dataBytes);
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
压缩后数据存储到d:/vo.txt，然后在 postman 中安装下图选择<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665124930079-23eaf99e-a6f7-476b-aee7-4d18ca196cf0.png#averageHue=%23f9f6f3&clientId=ua7569319-3222-4&from=paste&id=ubb263907&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u095580c5-9781-42a7-884c-1ca35b2c804&title=)
