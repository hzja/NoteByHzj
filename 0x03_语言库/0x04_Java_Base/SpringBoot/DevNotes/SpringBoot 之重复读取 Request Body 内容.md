JavaSpringBoot
<a name="zvZ2K"></a>
### 简介
在项目中，有两个场景会用到从 RequestBody 中读取内容。

- 打印请求日志
- 提供 Api 接口，在 Api 方法执行之前，从 RequestBody 中读取参数进行验签，验签通过后再执行 Api 方法的具体逻辑

这时就需要对 RequestBoy 进行重复读取。要实现对 RequestBody 进行重复读取的思路如下：

- 继承 `HttpServletRequestWrapper` 包装类，读取 RequestBody 的内容，然后缓存到 byte[] 数组 中；
- 增加过滤器，将包装类加入过滤器链中；
<a name="tnt6e"></a>
### 自定义 `HttpServletRequestWrapper` 包装类
由于采用采用application/json传输参数时，HttpServletRequest只能读取一次 body 中的内容。因为是读的字节流，读完就没了，因此需要需要做特殊处理。为实现述多次读取 Request 中的 Body 内容，需继承 `HttpServletRequestWrapper` 类，读取 Body 的内容，然后缓存到 `byte[]` 中；这样就可以实现多次读取 Body 的内容了。
```java
package com.olive.request; 
import org.apache.commons.io.IOUtils; 
import javax.servlet.ReadListener;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.io.*;

public class RequestWrapper extends HttpServletRequestWrapper {

    //参数字节数组
    private byte[] requestBody;

    //Http请求对象
    private HttpServletRequest request; 

    public RequestWrapper(HttpServletRequest request) throws IOException {
        super(request);
        this.request = request;
    }

    @Override
    public ServletInputStream getInputStream() throws IOException {
        /**
* 每次调用此方法时将数据流中的数据读取出来，然后再回填到InputStream之中
* 解决通过@RequestBody和@RequestParam（POST方式）读取一次后控制器拿不到参数问题
*/
        if (null == this.requestBody) {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            IOUtils.copy(request.getInputStream(), baos);
            this.requestBody = baos.toByteArray();
        }

        final ByteArrayInputStream bais = new ByteArrayInputStream(requestBody);
        return new ServletInputStream() {

            @Override
            public boolean isFinished() {
                return false;
            }

            @Override
            public boolean isReady() {
                return false;
            }

            @Override
            public void setReadListener(ReadListener listener) {

            }

            @Override
            public int read() {
                return bais.read();
            }
        };
    }

    public byte[] getRequestBody() {
        return requestBody;
    }

    @Override
    public BufferedReader getReader() throws IOException {
        return new BufferedReader(new InputStreamReader(this.getInputStream()));
    }
}
```
<a name="kxkDi"></a>
### 增加过滤器，转换Request为自定义Request
```java
package com.olive.filter;  

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

public class RequestBodyReaderFilter implements Filter {

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {

    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) {
        try {
            ServletRequest requestWrapper = null;
            if (request instanceof HttpServletRequest) {
                requestWrapper = new RequestWrapper((HttpServletRequest) request);
            }
            if (requestWrapper == null) {
                chain.doFilter(request, response);
            } else {
                chain.doFilter(requestWrapper, response);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ServletException e) {
            e.printStackTrace();
        } 
    }

    @Override
    public void destroy() {

    } 
}
```
<a name="I7FWu"></a>
### 注册Filter
```java
package com.olive.config;

import java.util.ArrayList;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import com.olive.filter.RequestBodyReaderFilter

@Configuration
public class WebConfig {
  
   @Bean
   public FilterRegistrationBean<RequestBodyReaderFilter> fequestBodyReaderFilter() {
      FilterRegistrationBean<RequestBodyReaderFilter> registrationBean = new FilterRegistrationBean<>();
      RequestBodyReaderFilter filter = new RequestBodyReaderFilter();
      registrationBean.setFilter(filter);
      ArrayList<String> urls = new ArrayList<>();
      urls.add("/*");//配置过滤规则
      registrationBean.setUrlPatterns(urls);
      registrationBean.setOrder(3);
      return registrationBean;
   }
   
}
```
