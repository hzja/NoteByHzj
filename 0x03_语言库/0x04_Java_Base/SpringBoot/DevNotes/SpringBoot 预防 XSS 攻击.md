JavaSpringBoot
<a name="pg9Cf"></a>
## **问题**
在前端Form表单的输入框中，用户没有正常输入，而是输入了一段代码：`</input><img src=1 onerror=alert1>` 这个正常保存没有问题。问题出在了列表查询的时候，上面的代码就生效了，由于图片的地址乱写的，所以这个alert就起作用了来看图。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691634827893-6e0fb8af-892a-4078-9f0e-40d84aa14a55.png#averageHue=%237b7b7b&clientId=u27111d39-5c0a-4&from=paste&id=u032a063e&originHeight=111&originWidth=610&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u66319123-6c5b-43a7-af89-f66e622014a&title=)<br />那根据这个原理，实际上如果没有做任何的限制，有心人就可以为所欲为了。可以在里面嵌入一些关键代码，把用户信息拿走。确实是个很严重的问题。
<a name="QDVF8"></a>
## **解决思路**
既然是因为输入框中输入了不该输入的东西，那自然就萌生一些想法：

- **校验输入内容**，不允许用户输入特殊字符，特殊标签
- **允许用户输入**，但是保存的时候将特殊的字符直接替换为空串
- **允许用户输入**，将特殊字符转译保存。

第一种方法，特殊字符过滤。既然要过滤特殊字符，那就得自己把所有的特殊字符列出来进行匹配，比较麻烦，而且要定义好什么才是特殊字符？况且用户本身不知道什么是特殊字符。突如其来的报错，会让用户有点摸不着头脑，不是很友好。<br />第二种方法，特殊字符替换为空串。未免有点太暴力。万一真的需要输入一点特殊的字符，保存完查出来发现少了好多东西，人家以为有BUG呢。也不是很好的办法。<br />第三种办法，特殊字符转译。这个办法不但用户数据不丢失，而且浏览器也不会执行代码。比较符合预期。<br />那办法确定了，怎么做呢？前端来做还是后端来做？想了想还是要后端来做。毕竟使用切面或者Filter可以一劳永逸。
<a name="dtLpi"></a>
## **最终实现**
看一下最终的代码实现（有些导入的包被删了）
<a name="knhCs"></a>
### 重新包装Request的代码
```java
import org.apache.commons.text.StringEscapeUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.servlet.ReadListener;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.Map;

/**
 * 重新包装一下Request。重写一些获取参数的方法，将每个参数都进行过滤
 */
public class XSSHttpServletRequestWrapper extends HttpServletRequestWrapper {
    private static final Logger logger = LoggerFactory.getLogger(XSSHttpServletRequestWrapper.class);

    private HttpServletRequest request;
    /**
     * 请求体 RequestBody
     */
    private String reqBody;

    /**
     * Constructs a request object wrapping the given request.
     *
     * @param request The request to wrap
     * @throws IllegalArgumentException if the request is null
     */
    public XSSHttpServletRequestWrapper(HttpServletRequest request) {
        super(request);
        logger.info("---xss XSSHttpServletRequestWrapper created-----");
        this.request = request;
        reqBody = getBodyString();
    }


    @Override
    public String getQueryString() {
        return StringEscapeUtils.escapeHtml4(super.getQueryString());
    }

    /**
     * The default behavior of this method is to return getParameter(String
     * name) on the wrapped request object.
     *
     * @param name
     */
    @Override
    public String getParameter(String name) {
        logger.info("---xss XSSHttpServletRequestWrapper work  getParameter-----");
        String parameter = request.getParameter(name);
        if (StringUtil.isNotBlank(parameter)) {
            logger.info("----filter before--name:{}--value:{}----", name, parameter);
            parameter = StringEscapeUtils.escapeHtml4(parameter);
            logger.info("----filter after--name:{}--value:{}----", name, parameter);
        }
        return parameter;
    }

    /**
     * The default behavior of this method is to return
     * getParameterValues(String name) on the wrapped request object.
     *
     * @param name
     */
    @Override
    public String[] getParameterValues(String name) {
        logger.info("---xss XSSHttpServletRequestWrapper work  getParameterValues-----");
        String[] parameterValues = request.getParameterValues(name);
        if (!CollectionUtil.isEmpty(parameterValues)) {
         // 经 “@Belief_7” 指正 这种方式不能更改parameterValues里面的值，要换成下面👇的写法
            //for (String value : parameterValues) {
            //    logger.info("----filter before--name:{}--value:{}----", name, value);
            //    value = StringEscapeUtils.escapeHtml4(value);
            //    logger.info("----filter after--name:{}--value:{}----", name, value);
            // }
            for (int i = 0; i < parameterValues.length; i++) 
         { 
             parameterValues[i] = StringEscapeUtils.escapeHtml4(parameterValues[i]); 
         } 
        }
        return parameterValues;
    }

    /**
     * The default behavior of this method is to return getParameterMap() on the
     * wrapped request object.
     */
    @Override
    public Map<String, String[]> getParameterMap() {
        logger.info("---xss XSSHttpServletRequestWrapper work  getParameterMap-----");
        Map<String, String[]> map = request.getParameterMap();
        if (map != null && !map.isEmpty()) {
            for (String[] value : map.values()) {
                /*循环所有的value*/
                for (String str : value) {
                    logger.info("----filter before--value:{}----", str, str);
                    str = StringEscapeUtils.escapeHtml4(str);
                    logger.info("----filter after--value:{}----", str, str);
                }
            }
        }
        return map;
    }

    /*重写输入流的方法，因为使用RequestBody的情况下是不会走上面的方法的*/
    /**
     * The default behavior of this method is to return getReader() on the
     * wrapped request object.
     */
    @Override
    public BufferedReader getReader() throws IOException {
        logger.info("---xss XSSHttpServletRequestWrapper work  getReader-----");
        return new BufferedReader(new InputStreamReader(getInputStream()));
    }

    /**
     * The default behavior of this method is to return getInputStream() on the
     * wrapped request object.
     */
    @Override
    public ServletInputStream getInputStream() throws IOException {
        logger.info("---xss XSSHttpServletRequestWrapper work  getInputStream-----");
        /*创建字节数组输入流*/
        final ByteArrayInputStream bais = new ByteArrayInputStream(reqBody.getBytes(StandardCharsets.UTF_8));
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
            public void setReadListener(ReadListener listener) {
            }

            @Override
            public int read() throws IOException {
                return bais.read();
            }
        };
    }


    /**
     * 获取请求体
     *
     * @return 请求体
     */
    private String getBodyString() {
        StringBuilder builder = new StringBuilder();
        InputStream inputStream = null;
        BufferedReader reader = null;

        try {
            inputStream = request.getInputStream();

            reader = new BufferedReader(new InputStreamReader(inputStream));

            String line;

            while ((line = reader.readLine()) != null) {
                builder.append(line);
            }
        } catch (IOException e) {
            logger.error("-----get Body String Error:{}----", e.getMessage(), e);
        } finally {
            if (inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException e) {
                    logger.error("-----get Body String Error:{}----", e.getMessage(), e);
                }
            }
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    logger.error("-----get Body String Error:{}----", e.getMessage(), e);
                }
            }
        }
        return builder.toString();
    }
}
```
<a name="qqWzi"></a>
### 定义过滤器
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.servlet.*;
import javax.servlet.http.HttpServletRequest;
import java.io.IOException;

/**
 * Filter 过滤器，拦截请求转换为新的请求
 */
public class XssFilter implements Filter {
    private static final Logger logger = LoggerFactory.getLogger(XssFilter.class);

    /**
     * 初始化方法
     */
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        logger.info("----xss filter start-----");
    }
    /**
     * 过滤方法
     */
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        ServletRequest wrapper = null;
        if (request instanceof HttpServletRequest) {
            HttpServletRequest servletRequest = (HttpServletRequest) request;
            wrapper = new XSSHttpServletRequestWrapper(servletRequest);
        }

        if (null == wrapper) {
            chain.doFilter(request, response);
        } else {
            chain.doFilter(wrapper, response);
        }
    }
}
```
<a name="KEWQL"></a>
### 注册过滤器
注册过滤器有两种方式。这里用的下面的这种<br />一种通过`@WebFilter`注解的方式来配置，但这种启动类上要加`@ServletComponentScan` 注解来指定扫描路径<br />另外一种就是以Bean 的方式来注入（不知道放哪里，就把Bean放到启动类里面）
```java
/**
 * XSS 的Filter注入
 * 用来处理getParameter的参数
 * @return
 */
@Bean
public FilterRegistrationBean xssFilterRegistrationBean(){
    FilterRegistrationBean filterRegistrationBean = new FilterRegistrationBean();
    filterRegistrationBean.setFilter(new XssFilter());
    filterRegistrationBean.setOrder(1);
    filterRegistrationBean.setDispatcherTypes(DispatcherType.REQUEST);
    filterRegistrationBean.setEnabled(true);
    filterRegistrationBean.addUrlPatterns("/*");
    return filterRegistrationBean;
}
```
上面配的是使用`request.getParameter()`的时候生效的，但是当使用`@RequestBody`来接收参数的时候是不行的，所以还得有下面的代码：
<a name="xH1kQ"></a>
### 处理请求中的JSON数据
```java
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import org.apache.commons.text.StringEscapeUtils;
import java.io.IOException;

/**
 * 反序列化，用来处理请求中的JSON数据
 * 处理RequestBody方式接收的参数
 */
public class XssJacksonDeserializer extends JsonDeserializer<String> {

    @Override
    public String deserialize(JsonParser jp, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        return StringEscapeUtils.escapeHtml4(jp.getText());
    }
}
```
<a name="PfGRL"></a>
### 处理返回值的JSON数据
```java
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.SerializerProvider;
import org.apache.commons.text.StringEscapeUtils;
import java.io.IOException;

/**
 * 处理向前端发送的JSON数据，将数据进行转译后发送
 */
public class XssJacksonSerializer extends JsonSerializer<String> {
    @Override
    public void serialize(String value, JsonGenerator jgen, SerializerProvider provider) throws IOException {
        jgen.writeString(StringEscapeUtils.escapeHtml4(value));
    }
}
```
<a name="TZOek"></a>
### 注册、配置自定义的序列化方法
```java
@Override
public void extendMessageConverters(List<HttpMessageConverter<?>> converters) {
    Jackson2ObjectMapperBuilder builder = new Jackson2ObjectMapperBuilder();
    ObjectMapper mapper = builder.build();
    /*注入自定义的序列化工具，将RequestBody的参数进行转译后传输*/
    SimpleModule simpleModule = new SimpleModule();
    // XSS序列化
    simpleModule.addSerializer(String.class, new XssJacksonSerializer());
    simpleModule.addDeserializer(String.class, new XssJacksonDeserializer());
    mapper.registerModule(simpleModule);
    converters.add(new MappingJackson2HttpMessageConverter(mapper));
}
```
<a name="PU8FV"></a>
### 测试
所有东西都配置完了，接下来进行愉快的测试阶段了。<br />依然在输入框中输入这段代码`</input><img src=1 onerror=alert1>`并进行保存。来看一下数据库中的保存结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691634827973-71d01253-f39c-484e-aa02-a70528e8d09f.png#averageHue=%23424241&clientId=u27111d39-5c0a-4&from=paste&id=B0CXW&originHeight=38&originWidth=549&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2c4fe732-4bf0-43e2-9102-efefe566857&title=)<br />可以看到数据库中保存的数据，已经经过转译了。那查询一下列表是什么样的呢？<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691634827920-d79fcec1-1bf9-4ea6-9ee7-fb8186169828.png#averageHue=%2366ad54&clientId=u27111d39-5c0a-4&from=paste&id=u180ae5f2&originHeight=149&originWidth=301&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u974c0967-2cf2-45f0-b93b-366c13fac6e&title=)<br />可以看到两条数据，上面的是经过转译的，正常的展示出来了。而下面的是没经过转译的，直接空白，并且弹了个窗。
<a name="pjYwf"></a>
## **总结**

- 就是注意要分情况处理。
- 拦截器处理一部分，并注意拦截器的注册方式
- Jackson的方式处理另一部分，也是注意配置方式
<a name="zVUPs"></a>
## **补充**
代码经过验证后，发现了一个问题。今天来补充一下。问题是这样的：<br />如果使用`@RequestBody`的形式接受参数，也就是需要使用自定义的序列化方式。然而有时候，业务需要传递一些JSON串到后端，如{\"username\":\"zx\",\"pwd\":\"123\"}（注意这是个字符串）。但是因为不管三七二十一直接暴力转译，导致里面的双引号以及其他符号都被转译了。那么当拿到这个字符串之后，再自己反序列化的时候就会出错了。<br />为了解决这个问题，在自定义的序列化方法中判断了一下这个字段的值是否是JSON形式，如果是JSON形式，那就不做处理，直接返回，以保证能够顺利反序列化。判断是否是JSON的方式，选择最简单的，判断首尾是否是{ } [ ]的组合。代码如下：
```java
public class XssJacksonDeserializer extends JsonDeserializer<String> {

    @Override
    public String deserialize(JsonParser jp, DeserializationContext ctxt) throws IOException, JsonProcessingException {
        // 判断一下 值是不是JSON的格式，如果是JSON的话，那就不处理了。
        /*判断JSON，可以用JSON.parse但是所有字段都Parse一下，未免有点太费性能，所以粗浅的认为，不是以{ 或者[ 开头的文本都不是JSON*/
        if (isJson(jp.getText())) {
            return jp.getText();
        }
        return StringEscapeUtils.escapeHtml4(jp.getText());
    }


    /**
     * 判断字符串是不是JSON
     *
     * @param str
     * @return
     */
    private boolean isJson(String str) {
        boolean result = false;
        if (StringUtil.isNotBlank(str)) {
            str = str.trim();
            if (str.startsWith("{") && str.endsWith("}")) {
                result = true;
            } else if (str.startsWith("[") && str.endsWith("]")) {
                result = true;
            }
        }
        return result;
    }
}
```
但是经过这样的改动之后，可能又没那么安全了。所以还是要看自己的取舍了。
