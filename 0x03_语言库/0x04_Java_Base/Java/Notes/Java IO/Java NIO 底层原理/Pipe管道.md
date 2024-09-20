Java Pipe
<a name="BjB8z"></a>
## 一、Pipe管道
Java NIO管道是2个线程之间的单向数据连接，Pipe有一个Source管道和一个Sink管道。数据会被写到sink通道，从source通道读取。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582027699737-f3524651-f62b-424b-a38f-ffb6f2d68516.png#averageHue=%23d4b07f&height=210&id=MrhLr&originHeight=210&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14664&status=done&style=shadow&title=&width=760)
<a name="8zyxY"></a>
## 二、案例
```java
@Test
public void pipeTest() throws IOException {
    // 1.获取管道
    Pipe pipe = Pipe.open();
    // 2.将缓冲区中的数据写入管道
    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
    Pipe.SinkChannel sinkChannel = pipe.sink();
    byteBuffer.put("通过单向管道发送数据".getBytes());
    byteBuffer.flip();
    sinkChannel.write(byteBuffer);
    // 3.读取缓冲区中的数据
    Pipe.SourceChannel sourceChannel = pipe.source();
    byteBuffer.flip();
    System.out.println(new String(byteBuffer.array(), 0, sourceChannel.read(byteBuffer)));
    sourceChannel.close();
    sinkChannel.close();
}
```

