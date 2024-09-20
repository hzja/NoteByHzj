Java
```java
// DatagramChannel实现聊天室示例

@Test
public void send() throws IOException {
    DatagramChannel datagramChannel = DatagramChannel.open();
    datagramChannel.configureBlocking(false);
    ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
    Scanner scanner = new Scanner(System.in);
    while (scanner.hasNext()) {
        String input = scanner.next();
        byteBuffer.put((LocalDateTime.now().toString() + "SEND : " + input).getBytes());
        byteBuffer.flip();
        datagramChannel.send(byteBuffer, new InetSocketAddress("127.0.0.1", 9999));
        byteBuffer.clear();
    }
    datagramChannel.close();
}

@Test
public void receive() throws IOException {
    DatagramChannel datagramChannel = DatagramChannel.open();
    datagramChannel.configureBlocking(false);
    datagramChannel.bind(new InetSocketAddress(9999));
    Selector selector = Selector.open();
    datagramChannel.register(selector, SelectionKey.OP_READ);
    while (selector.select() > 0) {
        Iterator<SelectionKey> selectionKeyIterator = selector.selectedKeys().iterator();
        while (selectionKeyIterator.hasNext()) {
            SelectionKey selectionKey = selectionKeyIterator.next();
            if (selectionKey.isReadable()) {
                ByteBuffer byteBuffer = ByteBuffer.allocate(1024);
                datagramChannel.receive(byteBuffer);
                byteBuffer.flip();
                System.out.println(new String(byteBuffer.array(), 0, byteBuffer.limit()));
                byteBuffer.clear();
            }
        }
        selectionKeyIterator.remove();
    }
}
```

