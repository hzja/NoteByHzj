SpringBoot Filter
<a name="vGKI9"></a>
## 过滤器Filter

- Filter是依赖于Servlet容器，属于Servlet规范的一部分，而拦截器则是独立存在的，可以在任何情况下使用。
- Filter的执行由Servlet容器回调完成，而拦截器通常通过动态代理的方式来执行。
- Filter的生命周期由Servlet容器管理，而拦截器则可以通过IoC容器来管理，因此可以通过注入等方式来获取其他Bean的实例，因此使用会更方便。
<a name="AYtQy"></a>
## 过滤器的配置
现在通过过滤器来实现记录请求执行时间的功能，其实现如下：
```java
public class LogCostFilter implements Filter {
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
    }
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        long start = System.currentTimeMillis();
        filterChain.doFilter(servletRequest,servletResponse);
        System.out.println("Execute cost="+(System.currentTimeMillis()-start));
    }
    @Override
    public void destroy() {
    }
}
```
这段代码的逻辑比较简单，就是在方法执行前先记录时间戳，然后通过过滤器链完成请求的执行，在返回结果之间计算执行的时间。这里需要主要，这个类必须继承Filter类，这个是Servlet的规范，这个跟以前的Web项目没区别。<br />但是，有了过滤器类以后，以前的web项目可以在web.xml中进行配置，但是spring boot项目并没有web.xml这个文件，那怎么配置？在Spring boot中，需要FilterRegistrationBean来完成配置。其实现过程如下：
```java
@Configuration
public class FilterConfig {
    @Bean
    public FilterRegistrationBean registFilter() {
        FilterRegistrationBean registration = new FilterRegistrationBean();
        registration.setFilter(new LogCostFilter());
        registration.addUrlPatterns("/*");
        registration.setName("LogCostFilter");
        registration.setOrder(1);
        return registration;
    }
}
```
这样配置就完成了，需要配置的选项主要包括实例化Filter类，然后指定url的匹配模式，设置过滤器名称和执行顺序，这个过程和在web.xml中配置其实没什么区别，只是形式不同而已。现在可以启动服务器访问任意URL：<br />![640.webp](https://cdn.nlark.com/yuque/0/2021/webp/396745/1617942589117-c822fa4e-81ca-4ba9-b0c9-bc5bd6e7b545.webp#height=384&id=AxqJw&originHeight=384&originWidth=952&originalType=binary&ratio=1&size=22858&status=done&style=shadow&width=952)
<a name="m2WUx"></a>
### 基于注解方式的配置
直接用`@WebFilter`就可以进行配置，同样，可以设置url匹配模式，过滤器名称等。这里需要注意一点的是`@WebFilter`这个注解是Servlet3.0的规范，并不是Spring boot提供的。除了这个注解以外，还需在配置类中加另外一个注解：`@ServletComponetScan`，指定扫描的包。
```java
@SpringBootApplication
@MapperScan("com.fcant.blog.dao")
@ServletComponentScan("com.fcant.blog.filters")
public class Application {
    public static void main(String[] args) throws Exception {
        SpringApplication.run(Application.class, args);
    }
}
```
首先需要写一个方法继承Filter类，写如下两个自己的Filter类，首先是FirstFilter类，其中`@Order`里边的数字越小代表越先被该Filter过滤，`@WebFilter`代表这是个Filter类并把这个类注入到容器中：
```java
import java.io.IOException;
 
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
 
import org.springframework.core.annotation.Order;
 
@Order(1)
@WebFilter(filterName="firstFilter", urlPatterns="/*")
public class FirstFilter implements Filter {
 
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        
    }
 
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        System.out.println("first filter 1");
        chain.doFilter(request, response);
        System.out.println("first filter 2");
    }
 
    @Override
    public void destroy() {
        
    }
}
```
 然后是第二个Filter，SecondFilter类：
```java
import java.io.IOException;
 
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.annotation.WebFilter;
 
import org.springframework.core.annotation.Order;
 
@Order(2)
@WebFilter(filterName="secondFilter", urlPatterns="/*")
public class SecondFilter implements Filter {
 
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
 
    }
 
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        System.out.println("second filter 1");
        System.out.println("before:" + response);
        chain.doFilter(request, response);
        System.out.println("after:" + response);
        System.out.println("second filter 2");
    }
 
    @Override
    public void destroy() {
        
    }
}
```
然后把Controller类也写出来：
```java
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
 
import javax.annotation.Resource;
 
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
 
import com.example.executor_test.task.OldTask;
import com.example.executor_test.task.OldTaskThread;
 
@RestController
public class TestController {
    
    @GetMapping("/test1")
    public String test1() {
        System.out.println("method in controller");
        return "test1";
    }
 
}
```
最后是springboot的主方法入口，注意，由于使用注解注入的Filter，所以要在下边这个Application类中加入`@ServletComponentScan`注解：
```java
import org.omg.CORBA.PRIVATE_MEMBER;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.web.servlet.ServletComponentScan;
import org.springframework.context.ConfigurableApplicationContext;
 
import com.example.executor_test.task.OldTaskThread;
 
@SpringBootApplication
@ServletComponentScan
public class ExecutorTestApplication {
 
    public static void main(String[] args) {    
        ConfigurableApplicationContext applicationContext = SpringApplication.run(ExecutorTestApplication.class, args);
    }
}
```
首先来看一下执行结果，启动后访问127.0.0.1:8080/test1。<br />可以看出代码执行的流程，首先请求被firstfilter截获，打印出first filter 1，然后去执行 `chain.doFilter(request, response)` ，这句话代表着请求会转发给过滤器链上下一个对象，也就是secondfilter，所以打印出`secondfilter`里的second filter 1，接下来再执行`secondfilter`里的`chain.dofilter()`方法，请求再转发给下一个对象，由于没有其他的filter了，所以会转发给controller，打印出了controller类中的method in controller，接下来再去内存栈里调用secondfilter的`print("second filter 2")`，然后再去内存栈里调用`firstfilter`的`print("first filter 1")`。所以如果在自己实现的Filter类的doFilter方法里不加`chain.doFilter(req, rep)`是万万不行的，那样会导致请求到了这个filter里就不再往下走了，永远进不了controller中。<br />也可以在`print("before:" + response)`和`print("after:" + response)`这两个地方打上断点，然后调试一下，发现在before那里的response里是什么都么有的，而在after那里的response里则是已经有了test1字符串，也就是说controller类test1方法的返回值已经添加进了response，所以如果想对请求的response做一下过滤处理，那么一定要在`chain.doFilter(res, rep)`之后业务逻辑。
<a name="tiiir"></a>
## 责任链模式的实现
接下来看一下`Filter`和`FilterChain`都是怎么用责任链模式实现的，来模拟一下简单的实现SpringBoot中的Filter接口和`FilterChain`类：<br />首先是自己写的`Filter`接口，里边就一个`doFilter`方法：
```java
package filterchain_pattern;
public interface Filter {
    public void doFilter(Request request, Response response, FilterChain chain);
}
```
接下来是自己写的`FilterChain`类：
```java
package filterchain_pattern;

import java.util.ArrayList;
import java.util.List;

public class FilterChain implements Filter {
    private List<Filter> filters = new ArrayList<>();
    int index = 0;
    public FilterChain addFilter(Filter filter) {
        filters.add(filter);
        return this;
    }

    @Override

    public void doFilter(Request request, Response response, FilterChain chain) {
        if(index == filters.size()) {
            return;
        }
        Filter filter = filters.get(index);
        index++;
        filter.doFilter(request, response, chain);
    }
}
```
接下来模拟Request类和Response类：
```java
package filterchain_pattern;

public class Request {
    public String requestStr;
}
```
```java
package filterchain_pattern;
public class Response {
    public String responseStr;
}
```
然后下一个Filter接口的实现类HTMLFilter类，该类会将`requestStr`中的`<>`替换成`[]`，并给responseStr添加-HTML response filter字符串，并在控制台打印出来：
```java
package filterchain_pattern;

public class HTMLFilter implements Filter {
    @Override
    public void doFilter(Request request, Response response, FilterChain chain) {
        request.requestStr = request.requestStr.replace("<", "[").replace(">", "]") + "--------HTML Request Filter";
        System.out.println("HTML Filter request Str:" + request.requestStr);
        chain.doFilter(request, response, chain);
        response.responseStr = response.responseStr + "-HTML response filter";
        System.out.println("HTML Filter response Str:" + response.responseStr);
    }
}
```
然后是另外一个Filter接口的实现类`SensitiveFilter`类， 该类会给requestStr添加一段字符串，给responseStr添加一段字符串，并在控制台打印出来：
```java
package filterchain_pattern;

public class SensitiveFilter implements Filter {

    @Override
    public void doFilter(Request request, Response response, FilterChain chain) {
        request.requestStr = request.requestStr + "---------------Sensitive request Filter";
        System.out.println("sensitiveFilter request str:" + request.requestStr);
        chain.doFilter(request, response, chain);
        response.responseStr = response.responseStr + "---------------------sensitive response filter";
        System.out.println("sensitiveFilter response str:" + response.responseStr);
    }
}
```
最后是Main方法类：
```java
package filterchain_pattern;

public class MainTest {
    public static void main(String[] args) {
        String msg = "<html>testMsg</html>";
        Request request = new Request();
        request.requestStr = msg;
        Response response = new Response();
        response.responseStr = "responseStr";
        FilterChain fc = new FilterChain();
        fc.addFilter(new HTMLFilter()).addFilter(new SensitiveFilter());
        fc.doFilter(request, response, fc);
    }
}
```
这就是责任链模式的实际应用了。
