Nginx<br />Nginx的流式响应（streaming response）是指在Nginx作为反向代理服务器时，将响应内容一边接收，一边逐步发送给客户端的过程。这种响应方式可以提高用户体验和网络传输效率，常用于处理较大的响应内容。<br />在默认情况下，Nginx是通过缓存响应内容来处理请求的。也就是说，当Nginx接收到完整的响应后，才会将其发送给客户端。但是，在某些场景下，这种方式可能会导致延迟较高或者请求失败的情况发生。<br />因此，Nginx提供了流式响应的功能，让反向代理服务器能够在接收到响应的同时，逐步地将响应内容发送给客户端。这种方式可以减少等待时间，同时降低网络传输的负载压力。<br />要实现Nginx的流式响应，需要在Nginx配置文件中加入相应的指令，例如：也就是打字机效果，
<a name="F1WVu"></a>
## 不缓存，支持流式输出
```nginx
 proxy_cache off; # 关闭缓存
 proxy_buffering off; # 关闭代理缓冲
 chunked_transfer_encoding on; # 开启分块传输编码
 tcp_nopush on; # 开启TCP NOPUSH选项，禁止Nagle算法
 tcp_nodelay on; # 开启TCP NODELAY选项，禁止延迟ACK算法
 keepalive_timeout 300; # 设定keep-alive超时时间为65秒
```
这段Nginx配置是为了解决流式响应的问题。它通过以下几个配置来实现：

1. `proxy_cache off; # 关闭缓存` 关闭缓存，防止代理服务器缓存流式响应内容，导致客户端收不到完整响应。
2. `proxy_buffering off; # 关闭代理缓冲` 关闭代理服务器对响应的缓冲，防止其缓冲整个响应后再发送给客户端，无法实现流式传输效果。
3. `chunked_transfer_encoding on; # 开启分块传输编码` 开启分块传输编码，允许将响应分成多个块进行传输，实现流式传输。
4. `tcp_nopush on; # 开启TCP` NOPUSH选项，禁止Nagle算法 开启TCP NOPUSH选项，禁用Nagle算法，防止小块数据的合并，实时发送给客户端。
5. `tcp_nodelay on; # 开启TCP` NODELAY选项，禁止延迟ACK算法 开启TCP NODELAY选项，禁用延迟ACK算法，防止ACK包的延迟，及时发送给客户端。
6. `keepalive_timeout 300; # 设定keep-alive超时时间为65秒` 增加keepalive超时时间，防止在流式响应未完成时，代理与源服务器的连接就被关闭。

所以，通过关闭缓存和代理缓冲，开启分块编码、禁用Nagle与延迟ACK算法，以及增加keepalive超时时间，可以实现代理服务器与客户端间的流式响应传输。 使用这段配置，Nginx代理可以透传源服务器的流式响应给客户端，不会导致响应内容被截断或延迟，实现真正的流式体验。
