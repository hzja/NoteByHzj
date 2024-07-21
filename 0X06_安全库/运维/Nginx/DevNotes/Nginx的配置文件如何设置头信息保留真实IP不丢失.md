在 Nginx 配置中设置头信息以保留客户端的真实 IP 地址通常是在使用反向代理的场景中需要的。当 Nginx 作为反向代理时，客户端的真实 IP 地址可能会在转发请求时丢失，因为到达应用服务器的请求看起来是从 Nginx 代理服务器发出的。为了解决这个问题，可以使用 X-Real-IP 或 X-Forwarded-For 头信息来转发真实的客户端 IP。<br />这里是一个基本的配置示例：
```nginx
server {
    listen 80

    location / {
        proxy_set_header Host $host
        proxy_set_header X-Real-IP $remote_addr
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for
        proxy_pass http://backend_server
    }
}
```

1. `proxy_set_header Host $host;` 将请求的 Host 头设置为原始请求的 Host 头。
2. `proxy_set_header X-Real-IP $remote_addr;` 将真实的客户端 IP 地址添加到请求头中。这里的 $remote_addr 是 Nginx 变量，代表客户端的 IP 地址。
3. `proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;` 添加一个 X-Forwarded-For 头，其中包含原始客户端的 IP 地址。`$proxy_add_x_forwarded_for` 会追加客户端的 IP 地址到已有的 X-Forwarded-For 头信息中，如果存在的话。

请确保将 http://backend_server 替换为后端服务器的实际地址。<br />此外，确保应用服务器或框架已配置为从这些头信息中读取真实的客户端 IP 地址。不同的应用和框架可能有不同的配置方法来支持这一点。
<a name="H8pPB"></a>
## 在java代码里怎么获取真是的客户端ip
在 Java 应用中获取真实的客户端 IP 地址通常涉及到解析 HTTP 请求的头信息。当应用部署在一个反向代理（如 Nginx）后面时，代理通常会在每个请求中添加特定的头信息（例如 X-Real-IP 或 X-Forwarded-For），以传递原始客户端的 IP 地址。<br />以下是一个基本的 Java 代码示例，展示了如何在一个基于 Servlet 的应用中获取这些头信息：
```java
import javax.servlet.http.HttpServletRequest;

public class NetworkUtils {

    public static String getClientIp(HttpServletRequest request) {
        String xForwardedForHeader = request.getHeader("X-Forwarded-For");
        if (xForwardedForHeader == null) {
            return request.getRemoteAddr();
        } else {
            
            return xForwardedForHeader.split(",")[0].trim();
        }
    }
}
```
在这个例子中，`getClientIp` 方法首先检查 X-Forwarded-For 头信息。如果这个头信息存在，方法会解析它并返回列表中的第一个 IP 地址，因为 X-Forwarded-For 可能包含由多个代理添加的多个 IP 地址，其中第一个通常是原始客户端的 IP 地址。如果 X-Forwarded-For 头信息不存在，方法会回退到使用 `HttpServletRequest` 的 `getRemoteAddr()` 方法，这通常返回直接建立连接的客户端（在这种情况下是代理服务器）的 IP 地址。<br />请注意，当处理来自不受信任源的 X-Forwarded-For 头信息时，需要谨慎，因为它可能被恶意客户端伪造。确保你的应用只在可信任的代理后面运行，并且考虑验证头信息中的 IP 地址。
