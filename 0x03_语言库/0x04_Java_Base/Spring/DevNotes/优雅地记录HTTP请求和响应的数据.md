Java<br />经常会遇到需要处理 http请求以及响应 body 的场景。<br />而这里比较大的一个问题是`servlet`的 `requestBody` 或 `responseBody` 流一旦被读取了就无法二次读取了。<br />针对这个问题，Spring本身提供了解决方案，即：

- `ContentCachingRequestWrapper`
- `ContentCachingResponseWrapper`。

编写一个过滤器:
```java
public abstract class HttpBodyRecorderFilter extends OncePerRequestFilter {

    private static final int DEFAULT_MAX_PAYLOAD_LENGTH = 1024 * 512;

    private int maxPayloadLength = DEFAULT_MAX_PAYLOAD_LENGTH;

    @Override

    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain)
            throws ServletException, IOException {

        boolean isFirstRequest = !isAsyncDispatch(request);

        HttpServletRequest requestToUse = request;

        if (isFirstRequest && !(request instanceof ContentCachingRequestWrapper)
            && (request.getMethod().equals(HttpMethod.PUT.name())
                        || request.getMethod().equals(HttpMethod.POST.name()))) {
            requestToUse = new ContentCachingRequestWrapper(request);
        }

        HttpServletResponse responseToUse = response;

        if (!(response instanceof ContentCachingResponseWrapper) && (request.getMethod().equals(HttpMethod.PUT.name())
                        || request.getMethod().equals(HttpMethod.POST.name()))) {
            responseToUse = new ContentCachingResponseWrapper(response);
        }

        boolean hasException = false;

        try {
            filterChain.doFilter(requestToUse, responseToUse);
        } catch (final Exception e) {
            hasException = true;
            throw e;
        } finally {
            int code = hasException ? 500 : response.getStatus();

            if (!isAsyncStarted(requestToUse) && (this.codeMatched(code, AdvancedHunterConfigManager.recordCode()))) {
                recordBody(createRequest(requestToUse), createResponse(responseToUse));
            } else {
                writeResponseBack(responseToUse);
            }

        }

    }

    protected String createRequest(HttpServletRequest request) {
        String payload = "";

        ContentCachingRequestWrapper wrapper = WebUtils.getNativeRequest(request, ContentCachingRequestWrapper.class);

        if (wrapper != null) {
            byte[] buf = wrapper.getContentAsByteArray();
            payload = genPayload(payload, buf, wrapper.getCharacterEncoding());
        }

        return payload;
    }

    protected String createResponse(HttpServletResponse resp) {
        String response = "";

        ContentCachingResponseWrapper wrapper = WebUtils.getNativeResponse(resp, ContentCachingResponseWrapper.class);

        if (wrapper != null) {
            byte[] buf = wrapper.getContentAsByteArray();

            try {
                wrapper.copyBodyToResponse();
            } catch (IOException e) {
                e.printStackTrace();
            }

            response = genPayload(response, buf, wrapper.getCharacterEncoding());
        }

        return response;

    }

    protected void writeResponseBack(HttpServletResponse resp) {
        ContentCachingResponseWrapper wrapper = WebUtils.getNativeResponse(resp, ContentCachingResponseWrapper.class);

        if (wrapper != null) {
            try {
                wrapper.copyBodyToResponse();
            } catch (IOException e) {
                LOG.error("Fail to write response body back", e);
            }
        }

    }

    private String genPayload(String payload, byte[] buf, String characterEncoding) {

        if (buf.length > 0 && buf.length < getMaxPayloadLength()) {
            try {
                payload = new String(buf, 0, buf.length, characterEncoding);
            } catch (UnsupportedEncodingException ex) {
                payload = "[unknown]";
            }
        }

        return payload;

    }

    public int getMaxPayloadLength() {
        return maxPayloadLength;
    }

    private boolean codeMatched(int responseStatus, String statusCode) {

        if (statusCode.matches("^[0-9,]*$")) {
            String[] filteredCode = statusCode.split(",");
            return Stream.of(filteredCode).map(Integer::parseInt).collect(Collectors.toList()).contains(responseStatus);
        } else {
            return false;
        }

    }

    protected abstract void recordBody(String payload, String response);

    protected abstract String recordCode();

}
```
这样自定义一个filter继承`HttpBodyRecorderFilter`，重写`recordBody`方法就能自定义自己的处理逻辑了。<br />另外，`recordCode`方法可用于定义在请求响应码为多少的时候才会去记录body，例如可以定义为只有遇到400或500时才记录body，用于错误侦测。<br />过滤器的匹配规则比较简单，如果想要像SpringMVC那样进行匹配，可以使用：`AntPathMatcher`。
```java
class PatternMappingFilterProxy implements Filter {

    private final Filter delegate;

    private final List<String> pathUrlPatterns = new ArrayList();

    private PathMatcher pathMatcher;

    public PatternMappingFilterProxy(Filter delegate, String... urlPatterns) {
        Assert.notNull(delegate, "A delegate Filter is required");
        this.delegate = delegate;
        int length = urlPatterns.length;
        pathMatcher = new AntPathMatcher();
        for (int index = 0; index < length; ++index) {
            String urlPattern = urlPatterns[index];
            this.pathUrlPatterns.add(urlPattern);
        }

    }

    @Override

    public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain)
        throws IOException, ServletException {

        HttpServletRequest httpRequest = (HttpServletRequest) request;

        String path = httpRequest.getRequestURI();

        if (this.matches(path)) {
            this.delegate.doFilter(request, response, filterChain);
        } else {
            filterChain.doFilter(request, response);
        }

    }

    private boolean matches(String requestPath) {

        for (String pattern : pathUrlPatterns) {
            if (pathMatcher.match(pattern, requestPath)) {
                return true;
            }
        }

        return false;
    }

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        this.delegate.init(filterConfig);
    }

    @Override

    public void destroy() {
        this.delegate.destroy();
    }

    public List<String> getPathUrlPatterns() {
        return pathUrlPatterns;
    }

    public void setPathUrlPatterns(List<String> urlPatterns) {
        pathUrlPatterns.clear();
        pathUrlPatterns.addAll(urlPatterns);
    }

}
```
这样子，`PatternMappingFilterProxy`装饰了真正的`HttpBodyRecorderFilter`，支持传入`urlPatterns`，从而实现像SpringMVC那样的ant style的匹配。例如对于以下接口：
```java
@PostMapping("/test/{id}")
public Object test(@PathVariable(value =  "id",required =  true)  final Integer index)  {

 //do something

}
```
可以设置`urlPattern`为`/test/{id:[0-9]+}`
