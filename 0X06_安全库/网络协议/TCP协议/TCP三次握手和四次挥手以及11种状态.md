TCP 三次握手 四次挥手 TCP的11种状态
<a name="0ZkE7"></a>
### 1、三次握手
置位概念：根据TCP的包头字段，存在3个重要的标识ACK、SYN、FIN

- ACK：表示验证字段
- SYN：位数置1，表示建立TCP连接
- FIN：位数置1，表示断开TCP连接

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598412711257-e3644caa-cbbc-4d37-aba2-a13e77bda62f.png#averageHue=%23f6f6f6&height=424&id=oTsrq&originHeight=1272&originWidth=1495&originalType=binary&ratio=1&rotation=0&showTitle=false&size=249137&status=done&style=shadow&title=&width=498.3333333333333)<br />三次握手过程说明：1、由客户端发送建立TCP连接的请求报文，其中报文中包含seq序列号，是由发送端随机生成的，并且将报文中的SYN字段置为1，表示需要建立TCP连接。（SYN=1，seq=x，x为随机生成数值）<br />2、由服务端回复客户端发送的TCP连接请求报文，其中包含seq序列号，是由回复端随机生成的，并且将SYN置为1，而且会产生ACK字段，ACK字段数值是在客户端发送过来的序列号seq的基础上加1进行回复，以便客户端收到信息时，知晓自己的TCP建立请求已得到验证。（SYN=1，ACK=x+1，seq=y，y为随机生成数值）这里的ack加1可以理解为是确认和谁建立连接。<br />3、客户端收到服务端发送的TCP建立验证请求后，会使自己的序列号加1表示，并且再次回复ACK验证请求，在服务端发过来的seq上加1进行回复。（SYN=1，ACK=y+1，seq=x+1）<br />整个过程中对应的TCP状态如下：

- **CLOSED**：初始状态，表示TCP连接是"关闭着的"或"未打开的"
- **LISTEN**：表示服务器端的某个SOCKET处于监听状态，可以接受客户端的连接
- **SYN_RCVD**：表示服务器接收到了来自客户端请求连接的SYN报文。这个状态是在服务端的，但是它是一个中间状态，很短暂，平常我们用netstat或ss的时候，不太容易看到这种状态，但是遇到SYN flood之类的SYN攻击时，会出现大量的这种状态，即收不到三次握手最后一个客户端发来的ACK，所以一直是这个状态，不会转换到ESTABLISHED
- **SYN_SENT**：这个状态与SYN_RCVD状态相呼应，，它是TCP连接客户端的状态，当客户端SOCKET执行connect()进行连接时，它首先发送SYN报文，然后随机进入到SYN_SENT状态，并等待服务端的SYN和ACK，该状态表示客户端的SYN已发送
- **ESTABLISHED**：表示TCP连接已经成功建立，开始传输数据

以上就是三次握手的五种TCP状态，单从客户端服务端角度来区分的话，CLOSED和ESTABLISHED会在客户端和服务端都出现，而LISTEN和SYN_RCVD通常是出现在服务端，SYN_SENT出现在客户端
:::warning
但通常在服务器和客户端并不是绝对的，比如Nginx的服务器中，Nginx通常作为web代理服务器，它既是服务端，也是客户端，所以在查询统计TCP状态的时候，最好通过匹配端口来区分是客户端的还是服务端的，来更精确的定位问题
:::
<a name="LoR1m"></a>
### 2、四次挥手
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598414655606-7136ea2c-d43f-4f14-a48d-1d92ce55e258.png#averageHue=%23f5f5f5&height=420&id=tsB7G&originHeight=1260&originWidth=1427&originalType=binary&ratio=1&rotation=0&showTitle=false&size=250858&status=done&style=shadow&title=&width=475.6666666666667)<br />四次挥手过程说明：1、客户端发送断开TCP连接请求的报文，其中报文中包含seq序列号，是由发送端随机生成的，并且还将报文中的FIN字段置为1，表示需要断开TCP连接。（FIN=1，seq=x，x由客户端随机生成）<br />2、服务端会回复客户端发送的TCP断开请求报文，其包含seq序列号，是由回复端随机生成的，而且会产生ACK字段，ACK字段数值是在客户端发过来的seq序列号基础上加1进行回复，以便客户端收到信息时，知晓自己的TCP断开请求已经得到验证。（FIN=1，ACK=x+1，seq=y，y由服务端随机生成）<br />3、服务端在回复完客户端的TCP断开请求后，不会马上进行TCP连接的断开，服务端会先确保断开前，所有传输到A的数据是否已经传输完毕，一旦确认传输数据完毕，就会将回复报文的FIN字段置1，并且产生随机seq序列号。（FIN=1，ACK=x+1，seq=z，z由服务端随机生成）<br />4、客户端收到服务端的TCP断开请求后，会回复服务端的断开请求，包含随机生成的seq字段和ACK字段，ACK字段会在服务端的TCP断开请求的seq基础上加1，从而完成服务端请求的验证回复。（FIN=1，ACK=z+1，seq=h，h为客户端随机生成）<br />至此TCP断开的4次挥手过程完毕<br />整个过程中对应的TCP状态如下：

- **FIN_WAIT_1**：这个状态在实际工作中很少能看到，当客户端想要主动关闭连接时，它会向服务端发送FIN报文，此时TCP状态就进入到FIN_WAIT_1的状态，而当服务端回复ACK，确认关闭后，则客户端进入到FIN_WAIT_2的状态，也就是只有在没有收到服务端ACK的情况下，FIN_WAIT_1状态才能看到，然后长时间收不到ACK，通常会在默认超时时间60s(由内核参数tcp_fin_timeout控制)后，直接进入CLOSED状态
- **FIN_WAIT_2**：这个状态相比较常见，也是需要注意的一个状态，FIN_WAIT_1在接收到服务端ACK之后就进入到FIN_WAIT_2的状态，然后等待服务端发送FIN，所以在收到对端FIN之前，TCP都会处于FIN_WAIT_2的状态，也就是，在主动断开的一端发现大量的FIN_WAIT_2状态时，需要注意，可能时网络不稳定或程序中忘记调用连接关闭，FIN_WAIT_2也有超时时间，也是由内核参数tcp_fin_timeout控制，当FIN_WAIT_2状态超时后，连接直接销毁
- **CLOSE_WAIT**：表示正在等待关闭，该状态只在被动端出现，即当主动断开的一端调用close()后发送FIN报文给被动端，被动段必然会回应一个ACK(这是由TCP协议层决定的)，这个时候，TCP连接状态就进入到CLOSE_WAIT
- **LAST_ACK**：当被动关闭的一方在发送FIN报文后，等待对方的ACK报文的时候，就处于LAST_ACK的状态，当收到对方的ACK之后，就进入到CLOSED状态了
- **TIME_WAIT**：该状态是最常见的状态，主动方在收到对方FIN后，就由FIN_WAIT_2状态进入到TIME_WAIT状态
- **CLOSING**：这个状态是一个比较特殊的状态，也比较少见，正常情况下不会出现，但是当双方同时都作为主动的一方，调用`close()`关闭连接的时候，两边都进入FIN_WAIT_1的状态，此时期望收到的是ACK包，进入FIN_WAIT_2的状态，但是却先收到了对方的FIN包，这个时候，就会进入到CLOSING的状态，然后给对方一个ACK，接收到ACK后直接进入到CLOSED状态
<a name="WFJgP"></a>
### 3、11种状态
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598415235693-2909b834-64fd-4b55-bbf5-ff70c3de2a1f.png#averageHue=%23f9f9f7&height=445&id=igEc3&originHeight=1335&originWidth=1701&originalType=binary&ratio=1&rotation=0&showTitle=false&size=252708&status=done&style=shadow&title=&width=567)<br />1、一开始，建立连接之前服务器和客户端的状态都为CLOSED；<br />2、服务器创建socket后开始监听，变为LISTEN状态；<br />3、客户端请求建立连接，向服务器发送SYN报文，客户端的状态变味SYN_SENT；<br />4、服务器收到客户端的报文后向客户端发送ACK和SYN报文，此时服务器的状态变为SYN_RCVD；<br />5、然后，客户端收到ACK、SYN，就向服务器发送ACK，客户端状态变为ESTABLISHED；<br />6、服务器端收到客户端的ACK后变为ESTABLISHED。此时3次握手完成，连接建立！<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598415672160-46ee1ca0-4257-4e6e-9426-fd0159c247a1.png#averageHue=%23f6f3f0&height=437&id=p70Wj&originHeight=1312&originWidth=1512&originalType=binary&ratio=1&rotation=0&showTitle=false&size=257438&status=done&style=shadow&title=&width=504)<br />由于TCP连接是全双工的，断开连接会比建立连接麻烦一点点。<br />1、客户端先向服务器发送FIN报文，请求断开连接，其状态变为FIN_WAIT1；<br />2、服务器收到FIN后向客户端发送ACK，服务器的状态围边CLOSE_WAIT；<br />3、客户端收到ACK后就进入FIN_WAIT2状态，此时连接已经断开了一半了。如果服务器还有数据要发送给客户端，就会继续发送；<br />4、直到发完数据，就会发送FIN报文，此时服务器进入LAST_ACK状态；<br />5、客户端收到服务器的FIN后，马上发送ACK给服务器，此时客户端进入TIME_WAIT状态；<br />6、再过了2MSL长的时间后进入CLOSED状态。服务器收到客户端的ACK就进入CLOSED状态。<br />至此，还有一个状态没有出来：CLOSING状态。<br />CLOSING状态表示：<br />客户端发送了FIN，但是没有收到服务器的ACK，却收到了服务器的FIN，这种情况发生在服务器发送的ACK丢包的时候，因为网络传输有时会有意外。

- LISTEN：等待从任何远端TCP 和端口的连接请求。
- SYN_SENT：发送完一个连接请求后等待一个匹配的连接请求。
- SYN_RECEIVED：发送连接请求并且接收到匹配的连接请求以后等待连接请求确认。
- ESTABLISHED：表示一个打开的连接，接收到的数据可以被投递给用户。连接的数据传输阶段的正常状态。
- FIN_WAIT_1：等待远端TCP 的连接终止请求，或者等待之前发送的连接终止请求的确认。
- FIN_WAIT_2：等待远端TCP 的连接终止请求。
- CLOSE_WAIT：等待本地用户的连接终止请求。
- CLOSING：等待远端TCP 的连接终止请求确认。
- LAST_ACK：等待先前发送给远端TCP 的连接终止请求的确认（包括它字节的连接终止请求的确认）
- TIME_WAIT：等待足够的时间过去以确保远端TCP 接收到它的连接终止请求的确认。
- TIME_WAIT 两个存在的理由：
   1. 可靠的实现tcp全双工连接的终止；
   2. 允许老的重复分节在网络中消逝。
- CLOSED：不在连接状态（这是为方便描述假想的状态，实际不存在）
