Java SpringMVC Netty<br />首先，必须要了解netty，说简单点：客户端通过TCP链接和服务器建立长连接，client和server都是通过管道（`ChannelPipeline`）的`addLast`方法的添加顺序来处理接收或者发送的数据。<br />这个和`struts`的`filter`的`doFilter`原理类似，处理完一个`filter`，如果后面还有其他的filter，就将数据`chain.doFilter`来继续处理。<br />然后，说说netty怎么来整合springMVC：当client和server建立连接后，在`addLast`的某个类中将client发来的请求，让`DispatcherServlet`来处理，然后将处理后的结果通过`ChannelHandlerContext`或者Channel将，结果`writeAndFlush`到client。
<a name="ItmAB"></a>
## 1、写一个netty sever的java代码
```java
package com.fcant.netty.server;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.servlet.DispatcherServlet;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

import com.fcant.netty.HttpServerInitializer;

public class NettyHttpServer {
    
    private int port;
    private static Logger log = LoggerFactory.getLogger(NettyHttpServer.class);
    private DispatcherServlet servlet;

    public NettyHttpServer(Integer port) {
        this.port = port;
    }
    public NettyHttpServer(Integer port, DispatcherServlet servlet) {
        this.port = port;
        this.servlet = servlet;
    }

    public void start(){
        EventLoopGroup bossGroup = new NioEventLoopGroup();
        EventLoopGroup workerGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap b = new ServerBootstrap();
            b.group(bossGroup, workerGroup)
                .channel(NioServerSocketChannel.class)
                .childHandler(new HttpServerInitializer(servlet))
                .option(ChannelOption.SO_BACKLOG, 128)
                .childOption(ChannelOption.SO_KEEPALIVE, true);

            System.out.println("NettyHttpServer Run successfully");
            // 绑定端口，开始接收进来的连接
            ChannelFuture f = b.bind(port).sync();
            // 等待服务器 socket 关闭 。在这个例子中，这不会发生，但你可以优雅地关闭你的服务器。
            f.channel().closeFuture().sync();
        } catch (Exception e) {
            log.error("NettySever start fail",e);
        } finally {
            workerGroup.shutdownGracefully();
            bossGroup.shutdownGracefully();
        }
    }
}
```
<a name="K30dA"></a>
## 2、初始化netty的channel管道
```java
package com.fcant.netty;

import org.springframework.web.servlet.DispatcherServlet;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.http.HttpContentCompressor;
import io.netty.handler.codec.http.HttpObjectAggregator;
import io.netty.handler.codec.http.HttpRequestDecoder;
import io.netty.handler.codec.http.HttpResponseEncoder;
import io.netty.handler.stream.ChunkedWriteHandler;



public class HttpServerInitializer extends ChannelInitializer<SocketChannel> {

    private DispatcherServlet servlet;
    
    public HttpServerInitializer(DispatcherServlet servlet) {
        this.servlet = servlet;
    }
    public HttpServerInitializer() {

    }

    @Override
    protected void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();
        pipeline.addLast("decoder", new HttpRequestDecoder());
        pipeline.addLast("encoder", new HttpResponseEncoder());
        pipeline.addLast("aggregator", new HttpObjectAggregator(2147483647));
        pipeline.addLast("chunkedWriter", new ChunkedWriteHandler());
        pipeline.addLast("deflater", new HttpContentCompressor());
        pipeline.addLast("handler", new HttpRequestHandler(servlet));
    }
}
```
<a name="xTtmM"></a>
## 3、在handler里面处理client发来的请求
```java

package com.fcant.netty;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.ServletContext;

import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.handler.codec.http.*;
import io.netty.handler.codec.http.multipart.DefaultHttpDataFactory;
import io.netty.handler.codec.http.multipart.HttpPostRequestDecoder;
import io.netty.handler.codec.http.multipart.InterfaceHttpData;
import io.netty.handler.codec.http.multipart.InterfaceHttpData.HttpDataType;
import io.netty.handler.codec.http.multipart.MemoryAttribute;
import io.netty.util.CharsetUtil;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.mock.web.MockHttpServletRequest;
import org.springframework.mock.web.MockHttpServletResponse;
import org.springframework.web.servlet.DispatcherServlet;
import org.springframework.web.util.UriComponents;
import org.springframework.web.util.UriComponentsBuilder;
import org.springframework.web.util.UriUtils;

public class HttpRequestHandler extends SimpleChannelInboundHandler<FullHttpRequest> {

    private static final Logger logger = LoggerFactory.getLogger(HttpRequestHandler.class);
    private final DispatcherServlet servlet;
    private final ServletContext servletContext;
    
    public HttpRequestHandler(DispatcherServlet servlet) {
        this.servlet = servlet;
        this.servletContext = servlet.getServletConfig().getServletContext();
    }

    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, Throwable e) throws Exception {
        logger.error(e.getMessage(),e);
        ctx.close();
    }

    protected void channelRead0(ChannelHandlerContext ctx, FullHttpRequest fullHttpRequest) throws Exception {
        boolean flag = HttpMethod.POST.equals(fullHttpRequest.getMethod())
            || HttpMethod.GET.equals(fullHttpRequest.getMethod());

        Map<String, String>  parammap = getRequestParams(ctx,fullHttpRequest);
        if(flag && ctx.channel().isActive()){
            //HTTP请求、GET/POST
            MockHttpServletResponse servletResponse = new MockHttpServletResponse();
            MockHttpServletRequest servletRequest =new MockHttpServletRequest(servletContext);
            // headers
            for (String name : fullHttpRequest.headers().names()) {
                for (String value : fullHttpRequest.headers().getAll(name)) {
                    servletRequest.addHeader(name, value);
                }
            }
            String uri = fullHttpRequest.getUri();
            uri = new String(uri.getBytes("ISO8859-1"), "UTF-8");
            uri = URLDecoder.decode(uri, "UTF-8");
            UriComponents uriComponents = UriComponentsBuilder.fromUriString(uri).build();
            String path = uriComponents.getPath();
            path = URLDecoder.decode(path, "UTF-8");
            servletRequest.setRequestURI(path);
            servletRequest.setServletPath(path);
            servletRequest.setMethod(fullHttpRequest.getMethod().name());

            if (uriComponents.getScheme() != null) {
                servletRequest.setScheme(uriComponents.getScheme());
            }
            if (uriComponents.getHost() != null) {
                servletRequest.setServerName(uriComponents.getHost());
            }
            if (uriComponents.getPort() != -1) {
                servletRequest.setServerPort(uriComponents.getPort());
            }

            ByteBuf content = fullHttpRequest.content();
            content.readerIndex(0);
            byte[] data = new byte[content.readableBytes()];
            content.readBytes(data);
            servletRequest.setContent(data);

            try {
                if (uriComponents.getQuery() != null) {
                    String query = UriUtils.decode(uriComponents.getQuery(),"UTF-8");
                    servletRequest.setQueryString(query);
                }
                if(parammap!=null&&parammap.size()>0){
                    for (String key : parammap.keySet()) {
                        servletRequest.addParameter(UriUtils.decode(key,"UTF-8"), UriUtils.decode(parammap.get(key) == null ? "": parammap.get(key), "UTF-8"));
                    }
                }

            } catch (UnsupportedEncodingException ex) {
                ex.printStackTrace();
            }
            this.servlet.service(servletRequest,servletResponse);

            HttpResponseStatus status = HttpResponseStatus.valueOf(servletResponse.getStatus());
            String result = servletResponse.getContentAsString();
            result = StringUtils.isEmpty(result)?"":result;
            FullHttpResponse response = new DefaultFullHttpResponse(HttpVersion.HTTP_1_1, status,Unpooled.copiedBuffer(result,CharsetUtil.UTF_8));
            response.headers().set("Content-Type", "text/json;charset=UTF-8");
            response.headers().set("Access-Control-Allow-Origin", "*");
            response.headers().set("Access-Control-Allow-Headers", "Content-Type,Content-Length, Authorization, Accept,X-Requested-With,X-File-Name");
            response.headers().set("Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS");
            response.headers().set("Content-Length", Integer.valueOf(response.content().readableBytes()));
            response.headers().set("Connection", "keep-alive");
            ChannelFuture writeFuture = ctx.writeAndFlush(response);
            writeFuture.addListener(ChannelFutureListener.CLOSE);
        }
    }

    /**
     * 获取post请求、get请求的参数保存到map中
     */
    private Map<String, String> getRequestParams(ChannelHandlerContext ctx, HttpRequest req){
        Map<String, String>requestParams=new HashMap<String, String>();
        // 处理get请求  
        if (req.getMethod() == HttpMethod.GET) {
            QueryStringDecoder decoder = new QueryStringDecoder(req.getUri());  
            Map<String, List<String>> parame = decoder.parameters();  
            Iterator<Entry<String, List<String>>> iterator = parame.entrySet().iterator();
            while(iterator.hasNext()){
                Entry<String, List<String>> next = iterator.next();
                requestParams.put(next.getKey(), next.getValue().get(0));
            }
        }
        // 处理POST请求  
        if (req.getMethod() == HttpMethod.POST) {
            HttpPostRequestDecoder decoder = new HttpPostRequestDecoder(  
                new DefaultHttpDataFactory(false), req);  
            List<InterfaceHttpData> postData = decoder.getBodyHttpDatas(); //
            for(InterfaceHttpData data:postData){
                if (data.getHttpDataType() == HttpDataType.Attribute) {  
                    MemoryAttribute attribute = (MemoryAttribute) data;  
                    requestParams.put(attribute.getName(), attribute.getValue());
                }
            }
        }
        return requestParams;
    }

}
```
<a name="WmFsq"></a>
## 4、初始化servlet并启动netty server
```java
package com.fcant;

import javax.servlet.ServletException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.mock.web.MockServletConfig;
import org.springframework.web.context.support.XmlWebApplicationContext;
import org.springframework.web.servlet.DispatcherServlet;

import com.fcant.common.config.PropConfig;
import com.fcant.netty.server.NettyHttpServer;

public class MagicWebServer {

    private static Logger logger = LoggerFactory.getLogger(MagicWebServer.class);

    public static void main(String[] args) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        Integer port = 6001;
        DispatcherServlet servlet = getDispatcherServlet(ctx);
        NettyHttpServer server = new NettyHttpServer(port,servlet);
        server.start();

    }

    public static DispatcherServlet getDispatcherServlet(ApplicationContext ctx){

        XmlWebApplicationContext mvcContext = new XmlWebApplicationContext();
        mvcContext.setConfigLocation("classpath:spring-servlet.xml");
        mvcContext.setParent(ctx);
        MockServletConfig servletConfig = new MockServletConfig(mvcContext.getServletContext(), "dispatcherServlet");
        DispatcherServlet dispatcherServlet = new DispatcherServlet(mvcContext);
        try {
            dispatcherServlet.init(servletConfig);
        } catch (ServletException e) {
            e.printStackTrace();
        }
        return dispatcherServlet;
    }
}
```
<a name="X2MwV"></a>
## 5、写一个controller，并测试一下http://127.0.0.1:6001/user/login
```java
package com.fcant.controller;

import java.util.HashMap;
import java.util.Map;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import com.alibaba.fastjson.JSONObject;

@Controller
@RequestMapping(value="/user",produces = "text/json;charset=utf-8")
public class UserController extends BaseController{

    @RequestMapping("/login")
    @ResponseBody
    public String login(String username,String pwd){
        JSONObject resultJson = new JSONObject();
        Map<String, String> loginResult = new HashMap<String, String>();
        loginResult.put("username", username);
        loginResult.put("age", "20");
        loginResult.put("sex", "boy");

        resultJson.put("code", 200);
        resultJson.put("msg", "登录成功");
        resultJson.put("result", loginResult);

        return JSONObject.toJSONString(resultJson);
    }
}
```
<a name="itSv1"></a>
## 在coding的时候遇到的问题

- 代码中的`applicationContext.xml`和`spring-servlet.xml`的按照SpringMVC的正常配置就行
- 如果返回到client端的代码有中文乱码，那么在`requestMapping`里面添加`produces = "text/json;charset=utf-8"`。
