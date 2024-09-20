Java<br />报错内容：
```
java.lang.IllegalArgumentException: Request header is too large
```
<a name="I26VW"></a>
### 异常原因
根据Exception MessageRequest header is too large，就可以判断这个错误原因是HTTP请求头过大导致的。
<a name="cA6fG"></a>
### 如何解决
解决方法主要两个方向：
<a name="Gwg7v"></a>
#### 方向一：配置应用服务器使其允许的最大值 > 实用的请求头数据大小
如果用Spring Boot的话，只需要在配置文件里配置这个参数即可：
```
server.max-http-header-size=
```
<a name="tVFqz"></a>
#### 方向二：规避请求头过大的情况
虽然上面的配置可以在解决，但是如果无节制的使用header部分，那么这个参数就会变得很不可控。你来一段，他来一段，为了适配不出错，还得求个并集的最大值，保证万无一失...即便如此，未来可能还得扩...<br />所以，对于请求头部分的数据还是不建议放太大的数据，建议把这些数据放到body里更为合理。
