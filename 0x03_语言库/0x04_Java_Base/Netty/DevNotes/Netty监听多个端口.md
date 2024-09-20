JavaNetty
<a name="qaPad"></a>
## 多实例来监听多端口
如果是多个实例监听多个端口，那么就会有多个bossGroup线程池（workGroup也是多个），这种通常是用在海量客户端接入时使用，如果客户端数量不多，其实也没有必要使用这种方式。
<a name="zQGQq"></a>
## 单实例监听多端口
利用Reactor主从模式，可以充分利用bossGroup线程池。
```java
import javax.annotation.PreDestroy;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.ChannelFutureListener;
import io.netty.channel.ChannelOption;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.epoll.EpollChannelOption;
import io.netty.channel.epoll.EpollEventLoopGroup;
import io.netty.channel.epoll.EpollServerSocketChannel;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.logging.LogLevel;
import io.netty.handler.logging.LoggingHandler;

/**
 * NioEventLoopGroup → EpollEventLoopGroup
   NioEventLoop → EpollEventLoop
   NioServerSocketChannel → EpollServerSocketChannel
   NioSocketChannel → EpollSocketChannel
   @Component
 */
public class NettyServer{
    private static final Logger logger = LoggerFactory.getLogger(NettyServer.class);
    ServerBootstrap serverBootstrap = new ServerBootstrap();
    EventLoopGroup boss =null;
    EventLoopGroup worker =null;
    ChannelFuture future = null;
    //厂商编码
    Integer factoryCode=null;

    boolean epoll=true;
    int port;
    public NettyServer(Integer fc,int port){
        this.factoryCode=fc;
        this.port=port;
    }

    @PreDestroy
    public void stop(){
        if(future!=null){
            future.channel().close().addListener(ChannelFutureListener.CLOSE);
            future.awaitUninterruptibly();
            boss.shutdownGracefully();
            worker.shutdownGracefully();
            future=null;
            logger.info(" 服务关闭 ");
        }
    }
    public void start(){
        logger.info(" nettyServer 正在启动");
        
        if(epoll){
            logger.info(" nettyServer 使用epoll模式");
            boss = new EpollEventLoopGroup();
            worker = new EpollEventLoopGroup();
        }
        else{
            logger.info(" nettyServer 使用nio模式");
            boss = new NioEventLoopGroup();
            worker = new NioEventLoopGroup();
        }
        
        logger.info("netty服务器在["+this.port+"]端口启动监听");
        
        serverBootstrap.group(boss,worker)
            .option(ChannelOption.SO_BACKLOG,1024)
            .option(EpollChannelOption.SO_REUSEPORT, true)
            .handler(new LoggingHandler(LogLevel.INFO))
            .option(ChannelOption.TCP_NODELAY,true)
            .childOption(ChannelOption.SO_KEEPALIVE,true)
            .childHandler(new NettyServerInitializer());
        
        if(epoll){
            serverBootstrap.channel(EpollServerSocketChannel.class);
        }else{
            serverBootstrap.channel(NioServerSocketChannel.class);
        }
        
        
        try{
            future = serverBootstrap.bind(8765).sync();
            future2 = serverBootstrap.bind(8766).sync();
            future.channel().closeFuture().addListener(new ChannelFutureListener()
            {
                @Override public void operationComplete(ChannelFuture future) throws Exception
                {       //通过回调只关闭自己监听的channel
                    future.channel().close();
                }
            });
            future2.channel().closeFuture().addListener(new ChannelFutureListener()
            {
                @Override public void operationComplete(ChannelFuture future) throws Exception
                {
                    future.channel().close();
                }
            });

            // 等待服务端监听端口关闭
           // future.channel().closeFuture().sync();
        }catch (Exception e){
            logger.info("nettyServer 启动时发生异常---------------{}",e);
            logger.info(e.getMessage());
        }finally {
             //这里一定要注释掉，因为上面没有阻塞了，不注释的话，这里会直接关闭的
            //boss.shutdownGracefully();
            //worker.shutdownGracefully();
        }
    }
}
```
在监听多个端口后，每个端口对应一种数据协议，在客户端channel初始化时，可以根据channel绑定的端口动态的选择解码器。
```java
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.concurrent.TimeUnit;
import org.xxx.android.netty.NettyConstants;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.bytes.ByteArrayEncoder;
import io.netty.handler.codec.string.StringEncoder;
import io.netty.handler.timeout.IdleStateHandler;

public class NettyServerInitializer extends ChannelInitializer<SocketChannel>{
    
    @Override
    protected void initChannel(SocketChannel socketChannel) throws Exception {
        ChannelPipeline pipeline = socketChannel.pipeline();
        pipeline.addLast(new IdleStateHandler(
                NettyConstants.SERVER_READ_IDEL_TIME_OUT,
                NettyConstants.SERVER_WRITE_IDEL_TIME_OUT,
                NettyConstants.SERVER_ALL_IDEL_TIME_OUT,
                TimeUnit.SECONDS));
        pipeline.addLast(new AcceptorIdleStateTrigger());

   
        pipeline.addLast(new StringEncoder());
        pipeline.addLast(new ByteArrayEncoder());
        int localPort = socketChannel.localAddress().getPort();
        //根据端口动态的选择解码器
        pipeline.addLast(new NettyServerDecoder(localPort));
        pipeline.addLast(new NettyServerHandler());
    }
}
```
