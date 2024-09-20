JavaSpringBootFilter
<a name="PO8jg"></a>
## 1、业务背景
业务背景是公司的内部系统有一个广告保存接口，需要ADX那边将投放的广告数据进行保存供后续使用。 广告数据大概长这样：

- adName是广告名字
- adTag是广告渲染的HTML代码，超级大数据库中都是用text类型来存放的，最大的adTag足足有60kb大小…
```json
{
  "adName":"",
  "adTag":""
}
```
因此，对与请求数据那么大的接口肯定是需要作一个优化的否则太大的数据传输有以下几个弊端：

- 占用网络带宽，而有些云产品就是按照带宽来计费的，间接浪费了钱
- 传输数据大导致网络传输耗时

为了克服这几个问题团队中的老鸟产生一个想法：<br />请求广告保存接口时先将Json对象字符串进行GZIP压缩，那请求时传入的就是压缩后的数据，而GZIP的压缩效率是很高的，因此可以大大减小传输数据，而当数据到达广告保存接口前再将传来的数据进行解压缩，还原成JSON对象就完成了整个GZIP压缩数据的请求以及处理流程。<br />其实这样做也存在着弊端：

- 请求变复杂了
   - 接口调用方那边需要对数据进行压缩
   - 接口执行方那边需要对拿到的数据进行解压
- 需要额外占用更多的CPU计算资源
- 可能会影响到原有的其他接口

对于以上几点基于公司当前的业务可以这样解决：

- 对与需要占用而外的CPU计算资源来说，公司的内部系统属于IO密集型应用，因此用一些CPU资源来换取更快的网络传输其实是很划算的
- 使用过滤器在请求数据到达Controller之前对数据进行解压缩处理后重新写回到Body中，避免影响Controller的逻辑，代码零侵入
- 而对于改造接口的同时是否会影响到原来的接口这一点可以通过 HttpHeader 的Content-Encoding=gzip属性来区分是否需要对请求数据进行解压缩

那废话少说，下面给出实现方案
<a name="Pzq1r"></a>
## 2、实现思路
<a name="Hfqa9"></a>
##### 前置知识：

- Http 请求结构以及Content-Encoding 属性
- gzip压缩方式
- Servlet Filter
- HttpServletRequestWrapper
- Spring Boot
- Java 输入输出流

实现流程图：<br />![2022-11-25-16-00-58.194384000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669363505926-bbf986cf-0414-4811-b5f6-1e5ddcf27fec.png#averageHue=%23f6f6f2&clientId=uf4aede12-91ce-4&from=ui&id=udbfffdb8&originHeight=742&originWidth=1814&originalType=binary&ratio=1&rotation=0&showTitle=false&size=167000&status=done&style=none&taskId=u1ebaca3a-96a3-441c-908b-76d57a7daa3&title=)
<a name="ATidd"></a>
##### 核心代码：
创建一个SpringBoot项目，先编写一个接口，功能很简单就是传入一个Json对象并返回，以模拟将广告数据保存到数据库
```java
@Slf4j
@RestController
public class AdvertisingController {

    @PostMapping("/save")
    public Advertising saveProject(@RequestBody Advertising advertising) {
        log.info("获取内容"+ advertising);
        return advertising;
    }
}

@Data
public class Advertising {
    private String adName;
    private String adTag;
}
```
编写并注册一个拦截器
```java
@Slf4j
@Component
public class GZIPFilter implements Filter {

    private static final String CONTENT_ENCODING = "Content-Encoding";
    private static final String CONTENT_ENCODING_TYPE = "gzip";

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        log.info("init GZIPFilter");
    }

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        long start = System.currentTimeMillis();
        HttpServletRequest httpServletRequest = (HttpServletRequest)servletRequest;

        String encodeType = httpServletRequest.getHeader(CONTENT_ENCODING);
        if (CONTENT_ENCODING_TYPE.equals(encodeType)) {
            log.info("请求:{} 需要解压", httpServletRequest.getRequestURI());
            UnZIPRequestWrapper unZIPRequestWrapper = new UnZIPRequestWrapper(httpServletRequest);
            filterChain.doFilter(unZIPRequestWrapper,servletResponse);
        }
        else {
            log.info("请求:{} 无需解压", httpServletRequest.getRequestURI());
            filterChain.doFilter(servletRequest,servletResponse);
        }
        log.info("耗时：{}ms", System.currentTimeMillis() - start);
    }

    @Override
    public void destroy() {
        log.info("destroy GZIPFilter");
    }
}

/**
 * @ClassName: FilterRegistration
 */
@Configuration
public class FilterRegistration {

    @Resource
    private GZIPFilter gzipFilter;

    @Bean
    public FilterRegistrationBean<GZIPFilter> gzipFilterRegistrationBean() {
        FilterRegistrationBean<GZIPFilter> registration = new FilterRegistrationBean<>();
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
实现`RequestWrapper`实现解压和写回Body的逻辑
```java
/**
 * @ClassName: UnZIPRequestWrapper
 * @Description: JsonString经过压缩后保存为二进制文件 -> 解压缩后还原成JsonString转换成byte[] 写回body中
 */
@Slf4j
public class UnZIPRequestWrapper extends HttpServletRequestWrapper {

    private final byte[] bytes;

    public UnZIPRequestWrapper(HttpServletRequest request) throws IOException {
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
            this.bytes = GZIPUtils.uncompressToByteArray(body);
        } catch (IOException ex) {
            log.info("解压缩步骤发生异常！");
            ex.printStackTrace();
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
附上压缩工具类
```java
public class GZIPUtils {
 
    public static final String GZIP_ENCODE_UTF_8 = "UTF-8";

    /**
     * 字符串压缩为GZIP字节数组
     * @param str
     * @return
     */
    public static byte[] compress(String str) {
        return compress(str, GZIP_ENCODE_UTF_8);
    }
 
    /**
     * 字符串压缩为GZIP字节数组
     * @param str
     * @param encoding
     * @return
     */
    public static byte[] compress(String str, String encoding) {
        if (str == null || str.length() == 0) {
            return null;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        GZIPOutputStream gzip;
        try {
            gzip = new GZIPOutputStream(out);
            gzip.write(str.getBytes(encoding));
            gzip.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return out.toByteArray();
    }
 
    /**
     * GZIP解压缩
     * @param bytes
     * @return
     */
    public static byte[] uncompress(byte[] bytes) {
        if (bytes == null || bytes.length == 0) {
            return null;
        }
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        ByteArrayInputStream in = new ByteArrayInputStream(bytes);
        try {
            GZIPInputStream ungzip = new GZIPInputStream(in);
            byte[] buffer = new byte[256];
            int n;
            while ((n = ungzip.read(buffer)) >= 0) {
                out.write(buffer, 0, n);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return out.toByteArray();
    }
 
    /**
     * 解压并返回String
     * @param bytes
     * @return
     */
    public static String uncompressToString(byte[] bytes) throws IOException {
        return uncompressToString(bytes, GZIP_ENCODE_UTF_8);
    }

    /**
     *
     * @param bytes
     * @return
     */
    public static byte[] uncompressToByteArray(byte[] bytes) throws IOException {
        return uncompressToByteArray(bytes, GZIP_ENCODE_UTF_8);
    }
 
    /**
     * 解压成字符串
     * @param bytes 压缩后的字节数组
     * @param encoding 编码方式
     * @return 解压后的字符串
     */
    public static String uncompressToString(byte[] bytes, String encoding) throws IOException {
        byte[] result = uncompressToByteArray(bytes, encoding);
        return new String(result);
    }

    /**
     * 解压成字节数组
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
        try {
            GZIPInputStream ungzip = new GZIPInputStream(in);
            byte[] buffer = new byte[256];
            int n;
            while ((n = ungzip.read(buffer)) >= 0) {
                out.write(buffer, 0, n);
            }
            return out.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
            throw new IOException("解压缩失败！");
        }
    }

    /**
     * 将字节流转换成文件
     * @param filename
     * @param data
     * @throws Exception
     */
    public static void saveFile(String filename,byte [] data)throws Exception{
        if(data != null){
            String filepath ="/" + filename;
            File file  = new File(filepath);
            if(file.exists()){
                file.delete();
            }
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(data,0,data.length);
            fos.flush();
            fos.close();
            System.out.println(file);
        }
    }
}
```
<a name="vtFAW"></a>
## 3、测试效果
注意一个大坑：千万不要直接将压缩后的`byte[]`当作字符串进行传输，否则会发现压缩后的请求数据竟然比没压缩后的要大得多🐶！一般有两种传输压缩后的`byte[]`的方式：

- 将压缩后的`byet[]`进行base64编码再传输字符串，这种方式会损失掉一部分GZIP的压缩效果，适用于压缩结果要存储在Redis中的情况
- 将压缩后的`byte[]`以二进制的形式写入到文件中，请求时直接在body中带上文件即可，用这种方式可以不损失压缩效果

Postman测试Gzip压缩数据请求：

- 请求头指定数据压缩方式：

![2022-11-25-16-00-58.369675000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669363631882-e2becf74-2512-45bf-ba47-40f806807631.png#averageHue=%23f9f9f9&clientId=uf4aede12-91ce-4&from=ui&id=zp7il&originHeight=676&originWidth=1706&originalType=binary&ratio=1&rotation=0&showTitle=false&size=109438&status=done&style=none&taskId=u42f17799-5292-4261-b8ce-3e505f687b9&title=)

- Body带上压缩后的`byte[]`写入的二进制文件

![2022-11-25-16-00-58.634647000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669363631879-58232728-37d0-4985-9886-c334b39ca55f.png#averageHue=%23fbfafa&clientId=uf4aede12-91ce-4&from=ui&id=R2Vzm&originHeight=698&originWidth=1694&originalType=binary&ratio=1&rotation=0&showTitle=false&size=79866&status=done&style=none&taskId=ua0209466-3410-4f1f-ae54-86ddd0f0585&title=)

- 执行请求，服务端正确处理了请求并且请求size缩小了将近一半，效果还是很不错的，这样GZIP压缩数据的请求的处理就完成了，完整的项目代码在下方⬇️

![2022-11-25-16-00-58.822474000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669363632089-3e15796f-3e96-4c69-9674-92b67d06d047.png#averageHue=%23faf9f9&clientId=uf4aede12-91ce-4&from=ui&id=udef7af9c&originHeight=1114&originWidth=1704&originalType=binary&ratio=1&rotation=0&showTitle=false&size=328183&status=done&style=none&taskId=u18eef5c2-6257-4b50-8055-aed1fc5f38d&title=)
<a name="kw5J0"></a>
## 4、Demo地址
[https://gitee.com/wx_1bceb446a4/gziptest](https://gitee.com/wx_1bceb446a4/gziptest)
