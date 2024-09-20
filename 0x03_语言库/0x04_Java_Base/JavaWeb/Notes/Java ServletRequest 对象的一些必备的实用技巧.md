Java JavaWeb
<a name="TuEeR"></a>
## 1、前言
`ServletRequest` 是 Java Web 经常接触的 Servlet Api 。有些时候要经常对其进行一些操作。这里列举一些经常的难点操作。
<a name="gbgFn"></a>
## 2、提取 body 中的数据
前后端交互会在 body 中传递数据。如何从 body 中提取数据。通常会通过 IO 操作：
```java
/**
 * obtain request body
 *
 * @param request the ServletRequest
 * @return body string   it maybe is   null
 */
public static String obtainBody(ServletRequest request) {

    BufferedReader br = null;
    StringBuilder sb = new StringBuilder();

    try {
        br = request.getReader();
        String str;
        while ((str = br.readLine()) != null) {
            sb.append(str);
        }
        br.close();
    } catch (IOException e) {
        log.error(" requestBody read error");
    } finally {
        if (null != br) {
            try {
                br.close();
            } catch (IOException e) {
                log.error(" close io error");
            }
        }
    }
    return sb.toString();

}
```
看起来比较凌乱，各种异常处理，IO 开关操作，很不优雅。如果使用了 Java 8 可以这样简化这种操作：
```java
String body = request.getReader().lines().collect(Collectors.joining());
```
`BufferedReader` 提供了获取 Java 8 Stream 流的方法 `lines()`，可以通过以上方法非常方便的获取 `ServletRequest` 中的 body
<a name="bK4Qr"></a>
## 3、`ServletRequest` 中的流是一次性的
:::danger
不要以为上面的读取 body 操作是完美无瑕的，这里有一个坑。如果按照上面的操作 `ServletRequest` 中的 body 只能读取一次。传输的数据都是通过流来传输的。`ServletRequest` 中实际上都是通过：`ServletInputStream inputStream = request.getInputStream()`来获取输入流，然后通过 `read` 系列方法来读取。Java 中的 `InputStream` `read` 方法内部有一个`postion`，它的作用是标志当前流读取到的位置，每读取一次，位置就会移动一次，如果读到最后，`read` 方法会返回 `-1`，标志已经读取完了，如果想再次读取，可以调用 `reset` 方法，`position` 就会移动到上次调用 `mark` 的位置，`mark` 默认是 `0`，所以就能从头再读了。能否 `reset` 是有条件的，它取决于 `markSupported()`，`markSupported()` 方法返回是否可以进行 `mark`/`reset` 。<br />再回头看 `ServletInputStream`，其实现并没有重写 `reset` 方法并不支持 `mark`/`reset` 。所以`ServletRequest` 中的 IO流 只能读取一次 。
:::
<a name="FPN3a"></a>
## 4、可重复读取 `ServletRequest` 中的流
如果使用了个多个 Servlet Filter 进行链式调用并多次操作 `ServletRequest` 中的流应该怎么做？可以通过 Servlet Api 提供的 `javax.servlet.http.HttpServletRequestWrapper` 来对其进行包装。步骤必须为

1. 先`HttpServletRequestWrapper` 包装，包装类不能进行IO操作。
2. 然后操作被包装的类进行流操作。
3. 传递 包装类

以下是在一个 Servlet Filter 中的标准范例：
```java
public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {

    HttpServletRequest httpServletRequest = (HttpServletRequest) request;
    // 先包装 包装类 httpServletRequestWrapper 不能进行IO操作
    HttpServletRequestWrapper httpServletRequestWrapper = new HttpServletRequestWrapper(httpServletRequest);

    // 使用 原始的 httpServletRequest 进行业务操作
    String body = request.getReader().lines().collect(Collectors.joining());
    // 传递包装类
    chain.doFilter(httpServletRequestWrapper, response);
}
```
<a name="ujhEB"></a>
## 5、如何对 `ServletRequest` 进行 `setParameter()`
从前台传入数据的时候、后台通过 `HttpServletRequest` 中的 `getParameter(String name)` 方法对数据进行获取。如果后台想将数据放进去，下次请求或者其他请求时使用，只能通过`setAttribute(String name, Object o)` 放入然后从 `getAttribute(String name)` 获取， 无法通过 `getParameter(String name)` 获取。<br />首先说一下`getParameter(String name)` 是在数据从客户端到服务端之后才有效的，而 则是服务端内部的事情，只有在服务端调用了 `setAttribute(String name, Object o)` 之后，并且没有重定向（`redirect`），在没有到客户端之前 `getAttribute(String name)` 才有效。<br />如果希望在服务端中转过程中使用 `setParameter()`，可以通过 `getParameter(String name)` 委托给 `getAttribute(String name)` 来执行。相关实现依然通过 `javax.servlet.http.HttpServletRequestWrapper` 来实现。
```java
package cn.fcant.spring.security.filter;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

public class ParameterRequestWrapper extends HttpServletRequestWrapper {


  public ParameterRequestWrapper(HttpServletRequest request ) {
    super(request);

  }

  @Override
  public String getParameter(String name) {
    return (String) super.getAttribute(name);
  }
}
```
也可借鉴思路实现其它需要的功能。
<a name="oyuEM"></a>
## 6、总结
对 `ServletRequest` 的一些常用的操作进行了讲解。也是经常在实际开发中遇到的一些问题。当然也可以使用一些第三方包来解决这些问题。
