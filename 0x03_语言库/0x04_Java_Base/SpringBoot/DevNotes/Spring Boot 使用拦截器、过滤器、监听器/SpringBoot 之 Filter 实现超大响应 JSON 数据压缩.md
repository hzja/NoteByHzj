JavaSpringBoot
<a name="FpJem"></a>
### 简介
项目中，请求时发送超大 json 数据外；响应时也有可能返回超大 json 数据。这里通过 filter 实现对响应 json 数据的压缩。先了解一下以下两个概念：

- 请求头：Accept-Encoding : gzip告诉服务器，该浏览器支持 gzip 压缩
- 响应头：Content-Encoding : gzip告诉浏览器，输出信息使用了 gzip 进行压缩

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665188700022-2bd2f480-e12e-4100-bb39-101ed6129c9a.png#clientId=uecb62a17-c48e-4&from=paste&id=ub74b3d17&originHeight=644&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabc8bb5d-1aea-4214-b306-49689095d8c&title=)
<a name="BtFsE"></a>
### pom.xml 引入依赖
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.olive</groupId>
  <artifactId>response-compression</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>

  <name>response-compression</name>
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
<a name="rBUCu"></a>
### 对Response进行包装
`GzipResponseWrapper` 类重新定义了输出流，拦截需要输出的数据，直接缓存到 `ByteArrayOutputStream` 中。
```java
package com.olive.filter;

import lombok.extern.slf4j.Slf4j;

import javax.servlet.ServletOutputStream;
import javax.servlet.WriteListener;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;
import java.io.*;

@Slf4j
public class GzipResponseWrapper extends HttpServletResponseWrapper {

    /**
     * 字节数组缓冲流，用来保存截获到的输出数据
     */
    private ByteArrayOutputStream buffer;

    /**
     * 重新定义servlet输出流，改变输出目的地将响应内容输出到给定的字节数组缓冲流中
     */
    private GzipResponseWrapper.CustomServletOutputStream servletOutputStream;

    /**
     * 同上
     */
    private PrintWriter writer;

    public GzipResponseWrapper(HttpServletResponse response) {
        super(response);
        //original HttpServletResponse object
        buffer = new ByteArrayOutputStream();
        servletOutputStream = new GzipResponseWrapper.CustomServletOutputStream(buffer);
        try {
            writer = new PrintWriter(new OutputStreamWriter(buffer, response.getCharacterEncoding()), true);
        } catch (UnsupportedEncodingException e) {
            log.error("GZipHttpServletResponse", e);
        }
    }

    @Override
    public ServletOutputStream getOutputStream() throws IOException {
        return servletOutputStream;
    }

    @Override
    public PrintWriter getWriter() throws IOException {
        return writer;
    }

    @Override
    public void flushBuffer() throws IOException {
        if (servletOutputStream != null) {
            servletOutputStream.flush();
        }
        if (writer != null) {
            writer.flush();
        }
    }

    /**
     * 向外部提供一个获取截获数据的方法
     * @return 从response输出流中截获的响应数据
     */
    public byte[] getOutputData() throws IOException {
        flushBuffer();
        return buffer.toByteArray();
    }

    private static class CustomServletOutputStream extends ServletOutputStream {

        /**
         * 字节数组缓冲流，用来保存截获到的输出数据
         */
        private ByteArrayOutputStream buffer;

        public CustomServletOutputStream(ByteArrayOutputStream buffer) {
            this.buffer = buffer;
        }

        @Override
        public boolean isReady() {
            return true;
        }

        @Override
        public void setWriteListener(WriteListener listener) {
        }

        /**
         * 重写输出流相关的方法
         * 将输出数据写出到给定的ByteArrayOutputStream缓冲流中保存起来
         * @param b 输出的数据
         * @throws IOException
         */
        @Override
        public void write(int b) throws IOException {
            buffer.write(b);
        }
    }
}
```
<a name="iunRQ"></a>
### 定义GzipFilter对输出进行拦截
GzipFilter 拦截器获取缓存的需要输出的数据，进行压缩，在输出数据之前先设置响应头Content-Encoding : gzip。
```java
package com.olive.filter;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.HttpHeaders;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.*;
import java.util.zip.GZIPOutputStream;

/**
 * 压缩过滤器
 *
 * 功能:对于返回给客户端的数据进行gzip压缩,提高响应速度
 * 实现说明:
 *     要对response对象的输出数据进行gzip压缩,首先得拿到后面servlet(controller)进行业务处理后往response对象里写入的数据
 *     可以通过重写response对象,修改该对象内部的输出流,使该流写出数据时写出到给定的字节数组缓冲流当中,
 *     并在重写后的response对象内部提供一个获取该字节数组缓冲流的方法,这样就可以截获响应数据
 *     然后就可以对截获的响应数据通过Gzip输出流进行压缩输出即可;
 *     因为响应数据是gzip压缩格式,不是普通的文本格式所以需要通过response对象(响应头)告知浏览器响应的数据类型
 */
@Slf4j
public class GzipFilter implements Filter {

    private final String GZIP = "gzip";

    public void destroy() {
        log.info("GzipFilter destroy");
    }

    public void doFilter(ServletRequest req, ServletResponse resp, FilterChain chain) throws ServletException, IOException {
        log.info("GzipFilter start");
        HttpServletRequest request = (HttpServletRequest) req;
        HttpServletResponse response = (HttpServletResponse) resp;
        String acceptEncoding = request.getHeader(HttpHeaders.ACCEPT_ENCODING);
        //searching for 'gzip' in ACCEPT_ENCODING header
        if( acceptEncoding != null && acceptEncoding.indexOf(GZIP) >= 0){
            GzipResponseWrapper gzipResponseWrapper = new GzipResponseWrapper(response);
            //pass the customized response object to controller to capture the output data
            chain.doFilter(request, gzipResponseWrapper);
            //get captured data
            byte[] data = gzipResponseWrapper.getOutputData();
            log.info("截获到数据：" + data.length + " bytes");
            //get gzip data
            ByteArrayOutputStream gzipBuffer = new ByteArrayOutputStream();
            GZIPOutputStream gzipOut = new GZIPOutputStream(gzipBuffer);
            gzipOut.write(data);
            gzipOut.flush();
            gzipOut.close();
            byte[] gzipData = gzipBuffer.toByteArray();
            log.info("压缩后数据：" + gzipData.length + " bytes");
            //set response header and output
            response.setHeader(HttpHeaders.CONTENT_ENCODING, GZIP);
            response.getOutputStream().write(gzipData);
            response.getOutputStream().flush();
        }else{
            chain.doFilter(req, resp);
        }
    }

    public void init(FilterConfig config) throws ServletException {
        log.info("GzipFilter init");
    }

}
```
<a name="aF2SO"></a>
### 注册 GzipFilter 拦截器
```java
package com.olive.config;

import com.olive.filter.GzipFilter;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * 注册filter
 */
@Configuration
public class FilterRegistration {

    @Bean
    public FilterRegistrationBean<GzipFilter> gzipFilterRegistrationBean() {
        FilterRegistrationBean<GzipFilter> registration = new FilterRegistrationBean<>();
        //Filter可以new，也可以使用依赖注入Bean
        registration.setFilter(new GzipFilter());
        //过滤器名称
        registration.setName("gzipFilter");
        //拦截路径
        registration.addUrlPatterns("/*");
        //设置顺序
        registration.setOrder(1);
        return registration;
    }
}
```
<a name="Twg9b"></a>
### 定义 Controller
该 Controller 非常简单，主要读取一个大文本文件，作为输出的内容。
```java
package com.olive.controller;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import com.olive.vo.ArticleRequestVO;
import org.apache.commons.io.FileUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class TestController {

    @RequestMapping("/getArticle")
    public Map<String, Object> getArticle(){
        Map<String, Object> result = new HashMap<>();
        result.put("code", 200);
        result.put("msg", "success");
        byte[] bytes = null;
        try {
            bytes = FileUtils.readFileToByteArray(new File("C:\\Users\\2230\\Desktop\\资料\\项目\\CXSSBOOT_DB_DDL-1.0.9.sql"));
        }catch (Exception e){

        }
        String content = new String(bytes);
        ArticleRequestVO vo = new ArticleRequestVO();
        vo.setId(1L);
        vo.setTitle("BUG");
        vo.setContent(content);
        result.put("body", vo);
        return result;
    }

}
```
Controller 返回数据的 VO
```java
package com.olive.vo;

import lombok.Data;

import java.io.Serializable;

@Data
public class ArticleRequestVO implements Serializable {

    private Long id;

    private String title;

    private String content;

}
```
<a name="tFClZ"></a>
### 定义 SpringBoot 引导类
```java
package com.olive;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class);
    }

}
```
<a name="aXQdT"></a>
### 测试
测试的curl
```bash
curl -X POST http://127.0.0.1:8080/getArticle
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1665188926758-fe4116a6-1198-4de4-abd9-9cef3295a43b.png#clientId=uecb62a17-c48e-4&from=paste&height=463&id=u4ac7110e&originHeight=1158&originWidth=2699&originalType=binary&ratio=1&rotation=0&showTitle=false&size=286103&status=done&style=none&taskId=ub16b7652-b02f-4b9b-9836-6d2d75c2662&title=&width=1079.6)
