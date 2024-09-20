Redis<br />Redis是一个非常流行的key-value存储系统，而作为其官方推荐的Java版客户端 Jedis也非常强大和稳定。<br />在单个客户端中，如果需要读写大量数据，可以考虑采用管道（Pipeline）方式。如果采用管道方式，那么多条命令可以通过批量的方式一次性地发送到服务器，而结果也会一次性返回到客户端。<br />本文将介绍Redis 使用管道（Pipeline）方式提升操作性能。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658842179643-8a229adc-4f33-4455-980e-98dd207cf706.jpeg#clientId=u43247f52-6c17-4&from=paste&id=u09162b50&originHeight=401&originWidth=1000&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf3d451cc-25e7-4684-9435-1822f6a7f8f&title=)
<a name="HInBY"></a>
## 一、管道（Pipeline）
未使用管道方式执行N条命令，如图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658842179704-be8e9d14-418b-4ed3-9924-348d14bb49b1.jpeg#clientId=u43247f52-6c17-4&from=paste&id=u3b48ad84&originHeight=734&originWidth=1038&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa3669bc-7b94-44bb-9b21-ce326c46d60&title=)<br />管道（Pipeline）：一次向Redis发送多条命令。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658842179709-2d883194-9b87-4f18-abc6-3d96d66af909.png#clientId=u43247f52-6c17-4&from=paste&id=ud216fc7c&originHeight=538&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub747fa47-893f-46f3-b46b-13ab642c311&title=)<br />客户端可以一次性发送多个请求而不用等待服务器的响应，待所有命令都发送完后再一次性读取服务的响应，这样可以极大的降低多条命令执行的网络传输开销，管道执行多条命令的网络开销实际上只相当于一次命令执行的网络开销。<br />需要注意到是用Pipeline方式打包命令发送，Redis必须在处理完所有命令前先缓存起所有命令的处理结果。打包的命令越多，缓存消耗内存也越多。<br />所以并不是打包的命令越多越好。pipeline中发送的每个Command都会被Server立即执行，如果执行失败，将会在此后的响应中得到信息；也就是Pipeline并不是表达“所有Command都一起成功”的语义，管道中前面命令失败，后面命令不会有影响，继续执行。
<a name="oylrr"></a>
## 二、管道执行示例代码
创建示例项目，测试一下管道执行的性能。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658842179578-f30a2711-8bac-4e16-8b9b-96c4be91c79b.png#clientId=u43247f52-6c17-4&from=paste&id=u9599f5b5&originHeight=552&originWidth=732&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufeed2b0b-9f9b-4b93-b5be-d03e460eb4e&title=)<br />PipelineApp 示例代码如下所示：
```java
public class PipelineApp {
    public static void main(String[] args) {
        Jedis jedis = new Jedis("localhost", 6379);
        jedis.auth("123456"); // 开始时间
        long startTime = System.currentTimeMillis();
        for (int i = 0; i < 10000; i++) {
            jedis.set("key_" + i, String.valueOf(i));
            jedis.get("key_" + i);
        } // 结束时间
        long endTime = System.currentTimeMillis();
        System.out.println("正常执行时间(ms)：" + (endTime - startTime));

        startTime = System.currentTimeMillis();
        Pipeline pipeline = jedis.pipelined();
        for (int i = 0; i < 10000; i++) {
            pipeline.set("key_" + i, String.valueOf(i));
            pipeline.get("key_" + i);
        }
        pipeline.sync();
        endTime = System.currentTimeMillis();
        System.out.println("管道执行时间(ms)：" + (endTime - startTime));
    }
}
```
启动应用，输出结果如下所示：<br />正常执行时间(ms)：1802<br />管道执行时间(ms)：78<br />可以看到以管道方式运行的耗时会远小于非管道方式运行的耗时，特别是客户端与服务端的网络延迟越大，性能体能越明显。所以，在项目中如果需要大批量向Redis 服务器读写数据，那么建议使用管道方式。
<a name="W5Gpi"></a>
## 三、Pipeline正确使用方式
使用pipeline组装的命令个数不能太多，不然数据量过大，增加客户端的等待时间，还可能造成网络阻塞，可以将大量命令的拆分多个小的pipeline命令完成。<br />管道不会管所有的命令是否都执行成功，只是逐条地执行命令。管道不能保证原子性，不支持事务。可以使用lua脚本来实现原子性。
