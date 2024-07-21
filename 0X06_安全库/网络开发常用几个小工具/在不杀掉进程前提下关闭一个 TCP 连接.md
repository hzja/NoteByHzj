网络<br />在处理关闭 TCP 连接第一反应都是「杀掉进程」。<br />是的，这个是最粗暴的方式，杀掉客户端进程和服务端进程影响的范围会有所不同：

- 在客户端杀掉进程的话，就会发送 FIN 报文，来断开这个客户端进程与服务端建立的所有 TCP 连接，这种方式影响范围只有这个客户端进程所建立的连接，而其他客户端或进程不会受影响。
- 而在服务端杀掉进程影响就大了，此时所有的 TCP 连接都会被关闭，服务端无法继续提供访问服务。

所以，关闭进程的方式并不可取，最好的方式要精细到关闭某一条 TCP 连接。<br />有的小伙伴可能会说，伪造一个四元组相同的 RST 报文不就行了？<br />这个思路很好，但是不要忘了还有个序列号的问题，伪造的 RST 报文的序列号一定能被对方接受吗？<br />如果 RST 报文的序列号不能落在对方的滑动窗口内，这个 RST 报文会被对方丢弃的，就达不到关闭的连接的效果。<br />所以，**要伪造一个能关闭 TCP 连接的 RST 报文，必须同时满足「四元组相同」和「序列号正好落在对方的滑动窗口内」这两个条件**。<br />直接伪造符合预期的序列号是比较困难，因为如果一个正在传输数据的 TCP 连接，滑动窗口时刻都在变化，因此很难刚好伪造一个刚好落在对方滑动窗口内的序列号的 RST 报文。<br />办法还是有的，**可以伪造一个四元组相同的 SYN 报文，来拿到“合法”的序列号！**<br />因为如果处于 establish 状态的服务端，收到四元组相同的 SYN 报文后，**会回复一个 Challenge ACK，这个 ACK 报文里的「确认号」，正好是服务端下一次想要接收的序列号**，说白了，就是可以通过这一步拿到服务端下一次预期接收的序列号。<br />**然后用这个确认号作为 RST 报文的序列号，发送给服务端，此时服务端会认为这个 RST 报文里的序列号是合法的，于是就会释放连接！**<br />在 Linux 上有个叫 **killcx** 的工具，就是基于上面这样的方式实现的，它会主动发送 SYN 包获取 SEQ/ACK 号，然后利用 SEQ/ACK 号伪造两个 RST 报文分别发给客户端和服务端，这样双方的 TCP 连接都会被释放，这种方式活跃和非活跃的 TCP 连接都可以杀掉。<br />使用方式也很简单，只需指明客户端的 IP 和端口号。
```bash
./killcx <IP地址>:<端口号>
```
killcx 工具的工作原理，如下图。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648179356797-565e7313-25df-4f7b-8d00-d13fec2b76be.png#clientId=ud62bb342-f0af-4&from=paste&id=u0d80dee6&originHeight=839&originWidth=782&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u46a0dada-986e-4459-a13c-63699222ed0&title=)<br />它伪造客户端发送 SYN 报文，服务端收到后就会回复一个携带了正确「序列号和确认号」的 ACK 报文（Challenge ACK），然后就可以利用这个 ACK 报文里面的信息，**伪造两个 RST 报文**：

- 用 Challenge ACK 里的**确认号**伪造 RST 报文发送给服务端，服务端收到 RST 报文后就会释放连接。
- 用 Challenge ACK 里的**序列号**伪造 RST 报文发送给客户端，客户端收到 RST 也会释放连接。

正是通过这样的方式，成功将一个 TCP 连接关闭了！<br />这里给大家贴一个使用 killcx 工具关闭连接的抓包图，大家多看看序列号和确认号的变化。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648179356802-a4e8cee6-de83-4a90-a77b-8d367055c718.png#clientId=ud62bb342-f0af-4&from=paste&id=u66b48b49&originHeight=213&originWidth=586&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua0ed83f2-b901-459d-a932-3080deb9692&title=)<br />所以，以后抓包中，如果莫名奇妙出现一个 SYN 包，有可能对方接下来想要发起的 RST 攻击，直接将 TCP 连接断开！
