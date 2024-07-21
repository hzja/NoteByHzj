TCP<br />TCP 三次握手和四次挥手过程中，途中某一步的报文丢失了，会发生什么？<br />一起来详细了解下，TCP 三次握手和四次挥手过程中每一步的异常情况。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773075-65e79795-c685-4f38-a931-5e23d289b18f.png#clientId=uddbf7830-9704-4&from=paste&id=uac4c4c7d&originHeight=462&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud71ca5ad-ddff-442a-bc80-cb59daa2086&title=)
<a name="d4enb"></a>
## TCP 三次握手丢包情况
<a name="ulP6Y"></a>
### **第一次握手丢失了，会发生什么？**
当客户端想和服务端建立 TCP 连接的时候，首先第一个发的就是 SYN 报文，然后进入到 SYN_SENT 状态。<br />在这之后，如果客户端迟迟收不到服务端的 SYN-ACK 报文（第二次握手），就会触发「超时重传」机制，重传 SYN 报文，而且**重传的 SYN 报文的序列号都是一样的**。<br />不同版本的操作系统可能超时时间不同，有 1 秒的，也有 3 秒的，这个超时时间是写死在内核里的，如果想要更改则需要重新编译内核，比较麻烦。<br />当客户端在 1 秒后还没收到服务端的 SYN-ACK 报文，客户端就会重发 SYN 报文，那到底重发几次呢？<br />在 Linux 里，客户端的 SYN 报文最大重传次数由 tcp_syn_retries内核参数控制，这个参数是可以自定义的，默认值一般是 5。
```bash
# cat /proc/sys/net/ipv4/tcp_syn_retries
5
```
通常，第一次超时重传是在 1 秒后，第二次超时重传是在 2 秒后，第三次超时重传是在 4 秒后，第四次超时重传是在 8 秒后，第五次超时重传是在 16 秒后。没错，**每次超时的时间是上一次的 2 倍**。<br />当第五次超时重传后，会继续等待 32 秒，如果服务端仍然没有回应 ACK，客户端就不再发送 SYN 包，然后断开 TCP 连接。<br />所以，总耗时是 1+2+4+8+16+32=63 秒，大约 1 分钟左右。<br />举个例子，假设 tcp_syn_retries 参数值为 3，那么当客户端的 SYN 报文一直在网络中丢失时，会发生下图的过程：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773085-f5e8bfdf-dd4e-4f55-bd55-35aba4aac369.png#clientId=uddbf7830-9704-4&from=paste&id=ucbfcb693&originHeight=1376&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u33ae1d45-fbe9-4604-b879-09fcd4ebbad&title=)<br />具体过程：

- **当客户端超时重传 3 次 SYN 报文后，由于  tcp_syn_retries 为 3，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到服务端的第二次握手（SYN-ACK 报文），那么客户端就会断开连接。**
<a name="g3vV9"></a>
### **第二次握手丢失了，会发生什么？**
当服务端收到客户端的第一次握手后，就会回 SYN-ACK 报文给客户端，这个就是第二次握手，此时服务端会进入 SYN_RCVD 状态。<br />第二次握手的 SYN-ACK 报文其实有两个目的 ：

- **第二次握手里的 ACK， 是对第一次握手的确认报文；**
- **第二次握手里的 SYN，是服务端发起建立 TCP 连接的报文。**

所以，如果第二次握手丢了，就会发生比较有意思的事情，具体会怎么样呢？<br />因为第二次握手报文里是包含对客户端的第一次握手的 ACK 确认报文，所以，如果客户端迟迟没有收到第二次握手，那么客户端就觉得可能自己的 SYN 报文（第一次握手）丢失了，于是**客户端就会触发超时重传机制，重传 SYN 报文**。<br />然后，因为第二次握手中包含服务端的 SYN 报文，所以当客户端收到后，需要给服务端发送 ACK 确认报文（第三次握手），服务端才会认为该 SYN 报文被客户端收到了。<br />那么，如果第二次握手丢失了，服务端就收不到第三次握手，于是**服务端这边会触发超时重传机制，重传 SYN-ACK 报文**。<br />在 Linux 下，SYN-ACK 报文的最大重传次数由 tcp_synack_retries内核参数决定，默认值是 5。
```bash
# cat /proc/sys/net/ipv4/tcp_synack_retries
5
```
因此，当第二次握手丢失了，客户端和服务端都会重传：

- **客户端会重传 SYN 报文，也就是第一次握手，最大重传次数由 tcp_syn_retries内核参数决定；**
- **服务端会重传 SYN-ACK 报文，也就是第二次握手，最大重传次数由 tcp_synack_retries 内核参数决定。**

举个例子，假设 tcp_syn_retries  参数值为 1，tcp_synack_retries 参数值为 2，那么当第二次握手一直丢失时，发生的过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773216-157d0239-fe17-43a7-bf96-56ea5d08205a.png#clientId=uddbf7830-9704-4&from=paste&id=ufd26372c&originHeight=1284&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uba221336-6f99-47df-8643-45e5eda4a6d&title=)<br />具体过程：

- **当客户端超时重传 1 次 SYN 报文后，由于  tcp_syn_retries 为 1，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到服务端的第二次握手（SYN-ACK 报文），那么客户端就会断开连接。**
- **当服务端超时重传 2 次 SYN-ACK 报文后，由于 tcp_synack_retries 为 2，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到客户端的第三次握手（ACK 报文），那么服务端就会断开连接。**
<a name="Jq1Ig"></a>
### **第三次握手丢失了，会发生什么？**
客户端收到服务端的 SYN-ACK 报文后，就会给服务端回一个 ACK 报文，也就是第三次握手，此时客户端状态进入到 ESTABLISH 状态。<br />因为这个第三次握手的 ACK 是对第二次握手的 SYN 的确认报文，所以当第三次握手丢失了，如果服务端那一方迟迟收不到这个确认报文，就会触发超时重传机制，重传 SYN-ACK 报文，直到收到第三次握手，或者达到最大重传次数。<br />注意，**ACK 报文是不会有重传的，当 ACK 丢失了，就由对方重传对应的报文**。<br />举个例子，假设 tcp_synack_retries 参数值为 2，那么当第三次握手一直丢失时，发生的过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773098-a5b57ef0-1469-47eb-8b07-aa082dc690dc.png#clientId=uddbf7830-9704-4&from=paste&id=u088a0059&originHeight=1435&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9bee37c9-8a52-4af1-aa25-f01fd15bca5&title=)<br />具体过程：

- **当服务端超时重传 2 次 SYN-ACK 报文后，由于 tcp_synack_retries 为 2，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到客户端的第三次握手（ACK 报文），那么服务端就会断开连接。**
<a name="qjRrN"></a>
## TCP 四次挥手丢包情况
<a name="K1l6m"></a>
### **第一次挥手丢失了，会发生什么？**
当客户端（主动关闭方）调用 close 函数后，就会向服务端发送 FIN 报文，试图与服务端断开连接，此时客户端的连接进入到 FIN_WAIT_1 状态。<br />正常情况下，如果能及时收到服务端（被动关闭方）的 ACK，则会很快变为 FIN_WAIT2状态。<br />如果第一次挥手丢失了，那么客户端迟迟收不到被动方的 ACK 的话，也就会触发超时重传机制，重传 FIN 报文，重发次数由 tcp_orphan_retries 参数控制。<br />当客户端重传 FIN 报文的次数超过 tcp_orphan_retries 后，就不再发送 FIN 报文，则会再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到第二次挥手，那么直接进入到 close 状态。<br />举个例子，假设 tcp_orphan_retries 参数值为 3，当第一次挥手一直丢失时，发生的过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773166-3e51a52b-31e7-41a7-a0b0-62e31f8cc7b5.png#clientId=uddbf7830-9704-4&from=paste&id=uc9ed45c1&originHeight=1319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u542d3860-3c98-45f9-8e08-584d16dc802&title=)<br />具体过程：

- **当客户端超时重传 3 次 FIN 报文后，由于 tcp_orphan_retries 为 3，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到服务端的第二次挥手（ACK报文），那么客户端就会断开连接。**
<a name="mwszm"></a>
### **第二次挥手丢失了，会发生什么？**
当服务端收到客户端的第一次挥手后，就会先回一个 ACK 确认报文，此时服务端的连接进入到 CLOSE_WAIT 状态。<br />在前面也提了，ACK 报文是不会重传的，所以如果服务端的第二次挥手丢失了，客户端就会触发超时重传机制，重传 FIN 报文，直到收到服务端的第二次挥手，或者达到最大的重传次数。<br />举个例子，假设 tcp_orphan_retries 参数值为 2，当第二次挥手一直丢失时，发生的过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773542-a1e6ed32-e4aa-4fc5-8d8f-b1f43506b9da.png#clientId=uddbf7830-9704-4&from=paste&id=u065f1748&originHeight=1319&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u71bfc50b-6aa1-4f81-b2a9-748510d462a&title=)<br />具体过程：

- **当客户端超时重传 2 次 FIN 报文后，由于 tcp_orphan_retries 为 2，已达到最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到服务端的第二次挥手（ACK 报文），那么客户端就会断开连接。**

这里提一下，当客户端收到第二次挥手，也就是收到服务端发送的 ACK 报文后，客户端就会处于 FIN_WAIT2 状态，在这个状态需要等服务端发送第三次挥手，也就是服务端的 FIN 报文。<br />对于 close 函数关闭的连接，由于无法再发送和接收数据，所以FIN_WAIT2 状态不可以持续太久，而 tcp_fin_timeout 控制了这个状态下连接的持续时长，默认值是 60 秒。<br />这意味着对于调用 close 关闭的连接，如果在 60 秒后还没有收到 FIN 报文，客户端（主动关闭方）的连接就会直接关闭，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773681-aa432b13-d626-4f76-bc3d-53242a667915.png#clientId=uddbf7830-9704-4&from=paste&id=ub705a1ee&originHeight=697&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1fbc7717-fff5-4374-b997-7c11fda5e87&title=)<br />但是注意，如果主动关闭方使用 shutdown 函数关闭连接，指定了只关闭发送方向，而接收方向并没有关闭，那么意味着主动关闭方还是可以接收数据的。<br />此时，如果主动关闭方一直没收到第三次挥手，那么主动关闭方的连接将会一直处于 FIN_WAIT2 状态（tcp_fin_timeout 无法控制 shutdown 关闭的连接）。如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773673-db598289-0d37-4904-9730-10b3b8dae5c9.png#clientId=uddbf7830-9704-4&from=paste&id=uf0215b42&originHeight=917&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7bc3a0f0-a649-42bb-b580-050318ccaf7&title=)
<a name="FA7Ru"></a>
### **第三次挥手丢失了，会发生什么？**
当服务端（被动关闭方）收到客户端（主动关闭方）的 FIN 报文后，内核会自动回复 ACK，同时连接处于 CLOSE_WAIT 状态，顾名思义，它表示等待应用进程调用 close 函数关闭连接。<br />此时，内核是没有权利替代进程关闭连接，必须由进程主动调用 close 函数来触发服务端发送 FIN 报文。<br />服务端处于 CLOSE_WAIT 状态时，调用了 close 函数，内核就会发出 FIN 报文，同时连接进入 LAST_ACK 状态，等待客户端返回 ACK 来确认连接关闭。<br />如果迟迟收不到这个 ACK，服务端就会重发 FIN 报文，重发次数仍然由 tcp_orphan_retries 参数控制，这与客户端重发 FIN 报文的重传次数控制方式是一样的。<br />举个例子，假设 tcp_orphan_retries = 3，当第三次挥手一直丢失时，发生的过程如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993774039-51e3610f-6007-47e7-a570-4ea7343a7279.png#clientId=uddbf7830-9704-4&from=paste&id=u9d3b9444&originHeight=1397&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud17dd423-77c7-45bf-9046-2a735146618&title=)<br />具体过程：

- **当服务端重传第三次挥手报文的次数达到了 3 次后，由于 tcp_orphan_retries 为 3，达到了重传最大次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到客户端的第四次挥手（ACK报文），那么服务端就会断开连接。**
- **客户端因为是通过 close 函数关闭连接的，处于 FIN_WAIT_2 状态是有时长限制的，如果 tcp_fin_timeout 时间内还是没能收到服务端的第三次挥手（FIN 报文），那么客户端就会断开连接。**
<a name="Tpjcr"></a>
### **第四次挥手丢失了，会发生什么？**
当客户端收到服务端的第三次挥手的 FIN 报文后，就会回 ACK 报文，也就是第四次挥手，此时客户端连接进入 TIME_WAIT 状态。<br />在 Linux 系统，TIME_WAIT 状态会持续 2MSL 后才会进入关闭状态。<br />然后，服务端（被动关闭方）没有收到 ACK 报文前，还是处于 LAST_ACK 状态。<br />如果第四次挥手的 ACK 报文没有到达服务端，服务端就会重发 FIN 报文，重发次数仍然由前面介绍过的 tcp_orphan_retries 参数控制。<br />举个例子，假设 tcp_orphan_retries 为 2，当第四次挥手一直丢失时，发生的过程如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661993773786-d42fb1d8-15e6-4151-a093-c5011d0c388c.png#clientId=uddbf7830-9704-4&from=paste&id=u2edfab5d&originHeight=1427&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u00f8cac2-ba53-4c5e-b9e6-5a7face1d00&title=)<br />具体过程：

- **当服务端重传第三次挥手报文达到 2 时，由于 tcp_orphan_retries 为 2， 达到了最大重传次数，于是再等待一段时间（时间为上一次超时时间的 2 倍），如果还是没能收到客户端的第四次挥手（ACK 报文），那么服务端就会断开连接。**
- **客户端在收到第三次挥手后，就会进入 TIME_WAIT 状态，开启时长为 2MSL 的定时器，如果途中再次收到第三次挥手（FIN 报文）后，就会重置定时器，当等待 2MSL 时长后，客户端就会断开连接。**
