Netty ChannelHandler
<a name="wtd5M"></a>
## 1、前言
基于 ChannelHandler 去自定义专门处理业务逻辑的 Handler。使用 Netty 开发的客户端和服务端之间通信，通信只是数据的传输，但是接受到数据如何去处理，此时就需要用到自定义 Handler 去实现了。并且通常情况下，不同的业务需要对应不同的 Handler。
<a name="EGD3f"></a>
## 2、自定义 Handler 步骤
如果是接受对方传输数据并且做处理，则继承 `ChannelInboundHandlerAdapter`。<br />实例：
```java
public class InboundHandler1 extends ChannelInboundHandlerAdapter {
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        super.channelRead(ctx, msg);
    }
}
```
如果是向对方写数据时，则继承 `ChannelOutboundHandlerAdapter`。<br />实例：
```java
public class OutboundHandler1 extends ChannelOutboundHandlerAdapter {
    @Override
    public void write(ChannelHandlerContext ctx, Object msg, ChannelPromise promise) throws Exception {
        super.write(ctx, msg, promise);
    }
}
```
<a name="n7XdY"></a>
## 3、核心方法
方法说明
<a name="RLk9b"></a>
### `channelActive(ChannelHandlerContext ctx)`
在客户端连接建立成功之后被调用，并且只会调用一次。一般用来做一些初始化工作、登录认证等。
<a name="rHhv5"></a>
### `channelInactive(ChannelHandlerContext ctx)`
连接断开时，触发该事件，无论是客户端主动断开，还是服务端主动断开，客户端和服务端的该方法都会监听到事件。
<a name="WvwAn"></a>
### `channelRead(ChannelHandlerContext ctx, Object msg)`
当 channel 上面有数据到来时会触发 channelRead 事件，当数据到来时，eventLoop 被唤醒继而调用 `channelRead` 方法处理数据。
<a name="Qa89f"></a>
### `channelReadComplete(ChannelHandlerContext ctx)`
channelRead 期间做个判断，read 到 0 个字节或者是 read 到的字节数小于 buffer 的容量，满足以上条件就会调用 `channelReadComplete` 方法。
<a name="Mypgn"></a>
### `exceptionCaught(ChannelHandlerContext ctx, Throwable cause)`
发生异常时，触发该事件。<br />以上方法是自定义 Handler 重新最多的方法，其中 `channelRead()` 是重点掌握。
<a name="rJtEi"></a>
## 4、ChannelHandler 处理流程
![fc698cb09a139c35259c4cfbc2f46b09.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1634915890652-62238999-d872-447b-831e-330567436370.png#clientId=u415608c8-640d-4&from=ui&id=ud217846e&originHeight=549&originWidth=980&originalType=binary&ratio=1&size=75922&status=done&style=shadow&taskId=u01ec545f-54d1-4386-bdd3-8c62bb6520c)
<a name="dzy1K"></a>
## 5、简单业务开发
需求：实现客户端向服务端发送登录认证请求。
<a name="O3CA2"></a>
### 5.1 客户端
客户端实现的功能：在连接准备就绪时 `channelActive()` 发起登录认证。<br />实例：
```java
public class ClientLoginHandler extends ChannelInboundHandlerAdapter {
    //1.通道激活的时候，发送账号、密码
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        Map<String,String> map=new HashMap<String,String>();
        map.put("username","admin");
        map.put("password","1234567");
 
        //对象流序列化Map
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(os);
        oos.writeObject(map);
        byte[] bytes=os.toByteArray();
 
        //关闭流
        oos.close();
        os.close();
        
        //发送
        ctx.channel().writeAndFlush(Unpooled.copiedBuffer(bytes));
    }
}
```
代码说明：<br />`channelActive` 事件是通道建立时触发该事件，并且仅触发一次该事件，通常情况下，在 `channelActive` 里面实现登录认证；<br />客户端往服务端发送数据的时候需要使用对象流进行序列化，客户端接收服务端响应信息的时候，需要通过对象流进行反序列化；<br />Netty 底层是 `ByteBuf` 进行传输的，最终网络底层传输则是 `byte[]`，因此需要做序列化和反序列化操作。
<a name="UPkv6"></a>
### 5.2 服务端
服务端实现的功能：接受客户端的请求数据，并且做账户和密码的校验。<br />实例：
```java
public class ServerLoginHandler extends ChannelInboundHandlerAdapter {
    //1.读取客户端发送过来的数据
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        //1.转换ByteBuf
        ByteBuf buffer=(ByteBuf)msg;
        //2.定义一个byte数组，长度是ByteBuf的可读字节数
        byte[] bytes=new byte[buffer.readableBytes()];
        //3.往自定义的byte[]读取数据
        buffer.readBytes(bytes);
 
        //4.对象流反序列化
        ByteArrayInputStream is=new ByteArrayInputStream(bytes);
        ObjectInputStream iss=new ObjectInputStream(is);
        Map<String,String> map=(Map<String,String>)iss.readObject();
        
        //5.关闭流
        is.close();
        iss.close();
        
        //6.认证账号、密码，并且响应
        String username=map.get("username");
        String password=map.get("password");
        if(username.equals("admin")&&password.equals("123456")){
            ctx.channel().writeAndFlush(Unpooled.copiedBuffer("success".getBytes()));
        }else{
            ctx.channel().writeAndFlush(Unpooled.copiedBuffer("error".getBytes()));
            ctx.channel().closeFuture();
        }
    }
}
```
代码说明：<br />`channelRead` 方法，在客户端有数据可读取的时候会触发该方法；<br />接受到的数据不可以直接使用，并且先转换 ByteBuf，再转换 byte []，最后通过对象流转换成目标类型的数据；<br />解析出来的账号、密码，进行认证（这里是写死，真实是连接数据库进行校验），把认证结果响应给客户端。
<a name="IvMHK"></a>
## 6、复杂业务开发
上面的登录认证案例只是比较简单的一个业务，真实项目当中，肯定是很多的业务组合而成的，那么如何基于 Netty 的 Handler 去实现呢？
<a name="Q6xOT"></a>
### 6.1 共用一个 Handler
所有业务共用一个 Handler，由该 Handler 进行根据业务编码作为路由标识进行业务分发。<br />方案优缺点：<br />优点： 思路简单，适合业务不是很复杂的业务；<br />缺点： 如果业务很多的情况下，代码会变的非常的臃肿，不太好管理。<br />实例：
```java
public class LoginHandler extends ChannelInboundHandlerAdapter {
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        Map<String,String> map=(Map<String,String>)msg;
        String code=map.get("code");
        if(code.equals("login")){
            //具体逻辑
        }else if(code.equals("getUserInfo")){
            //具体逻辑
        }else{
            //..............
        }
    }
}
```
<a name="LGWrw"></a>
### 6.2 多个 Handler 手工流转
定义多个 Handler，每个 Handler 根据业务编码来判断是否需要处理，如果是则处理，否则继续向下流转。<br />方案优缺点：<br />优点： 把所有的业务解耦，不用所有业务都耦合在一起，使项目结构更清晰，更加容易维护；<br />缺点： 客户端和服务端都有维护一份业务编码，一旦编码发生变更，则需要找到具体业务点去调整，相对比较麻烦。<br />实例：
```java
public class LoginHandler extends ChannelInboundHandlerAdapter {
    @Override
    public void channelRead(ChannelHandlerContext ctx, Object msg) throws Exception {
        Map<String,String> map=(Map<String,String>)msg;
        String code=map.get("code");
        if(code.equals("login")){
			//逻辑处理
            
        }else{
            //手工流转下一个Handler
            super.channelRead(ctx, msg);
        }
    }
}
```
<a name="FIJxZ"></a>
### 6.3 `SimpleChannelInboundHandler`
根据客户端提交的参数类型，自动流转到指定的 Handler 去处理。<br />方案优缺点：<br />优点： ①把业务解耦，每个业务对应独立的 Handler；②不需要维护一份业务编码；<br />缺点： 所有的封装封装都得对应一个实体，实体数量会比较多，但是严格意义来说，也不能说是缺点，现在基本上都是面向对象来进行编程。<br />这种模式在真实开发当中是使用最广泛的。<br />实例：
```java
public class FirstHandler extends SimpleChannelInboundHandler<User> {
    protected void channelRead0(ChannelHandlerContext channelHandlerContext, User user) throws Exception {
        //处理业务逻辑
    }
}
```
