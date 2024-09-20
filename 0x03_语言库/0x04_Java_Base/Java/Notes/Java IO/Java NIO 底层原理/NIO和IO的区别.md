Java NIO
<a name="yeSrO"></a>
## 1、NIO和IO的区别
<a name="lsrLL"></a>
### Java NIO和IO的区别
| **IO** | **NIO** |
| --- | --- |
| 面向流(Stream Oriented) | 面向缓冲区(Buffer Oriented) |
| 阻塞IO(Blocking IO) | 非阻塞IO(Non Blocking IO) |
|  | 选择器(Selectors) |

<a name="VjCzX"></a>
### Java NIO系统的核心
通道(Channel)和缓冲区(Buffer).<br />通道表示打开到IO设备（例如文件、套接字）的连接。若需要使用NIO系统，需要获取用于连接IO设备的通道以及用于容纳数据的缓冲区。然后操作缓冲区，对数据进行处理。<br />简言之，Channel负责传输，Buffer负责存储。
