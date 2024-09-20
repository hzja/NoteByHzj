JavaBIO
<a name="hT2aa"></a>
## 1、Java BIO基本介绍
Java BlO就是传统的Java IO编程，其相关的类和接口在Java.io 包中BIO(blocking I/O)同步阻塞，服务器实现模式为一个连接一个线程，即客户端有连接请求时服务器端就需要启动一个线程进行处理，如果这个连接不做任何事情会造成不必要的线程开销，可以通过线程池机制改善（实现多个客户连接服务器）。
<a name="V5xqX"></a>
## 2、Java BIO工作机制
![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697884550545-3ee179a2-0a73-4add-ae1d-54cf6743d15d.jpeg)
<a name="F7H09"></a>
## 3、传统的BIO编程实例回顾
网络编程的基本模型是Client/Server模型，也就是两个进程之间进行相互通信，其中服务端提供位置信（绑定IP地址和端口），客户端通过连接操作向服务端监听的端口地址发起连接请求，基于TCP协议下进行三次握手连接，连接成功后，双方通过网络套接字（Socket）进行通信。<br />传统的同步阻塞模型开发中，服务端ServerSocket负责绑定IP地址，启动监听端口；客户端Socket负责发起连接操作。连接成功后，双方通过输入和输出流进行同步阻塞式通信。<br />基于BIO模式下的通信，客户端-服务端是完全同步，完全藕合的。
<a name="EinaY"></a>
### 客户端案例
```java
/**
 * 客户端
 */
public class Client {
    public static void main(String[] args) {
        try {
            //1.创建Socket对象请求服务端的连接
            Socket socket = new Socket("127.0.0.1",9999);
            //2.从Socket对象中获取一个字节输出流
            OutputStream os = socket.getOutputStream();
            //3.把字节输出流包装成一个打印流
            PrintStream ps = new PrintStream(os);
            //ps.print("hello World! 服务端，你好");
            ps.println("hello World! 服务端，你好");
            ps.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
<a name="jeVCs"></a>
### 服务端案例
```java
/**
 * 目标：客户端发送消息，服务端接收消息
 */
public class Server {
    public static void main(String[] args) {
        try {
            System.out.println("===服务端启动===");
            //1.定义一个ServerSocket对象进行服务端的端口注册
            ServerSocket ss = new ServerSocket(9999);
            //2. 监听客户端的Socket连接请求
            Socket socket = ss.accept();
            //3.从socket管道中得到一个字节输入流对象
            InputStream is = socket.getInputStream();
            //4.把字节输入流包装成一个缓存字符输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String msg;
            /*while ((msg = br.readLine()) != null){
                System.out.println("服务端接收到：" + msg);
            }*/
            if ((msg = br.readLine()) != null){
                System.out.println("服务端接收到：" + msg);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
<a name="ATluN"></a>
### 输出
```
===服务端启动===
服务端接收到：hello World! 服务端，你好
```
<a name="vuWTp"></a>
### 小结
在以上通信中，服务端会一直等待客户端的消息，如果客户端没有进行消息的发送，服务端将一直进入阻塞状态同时服务端是按照行获取消息的，这意味育客户端也必须按照行进行消息的发送，否则服务端将进入等待消息的阻塞状态！
<a name="nGc5p"></a>
## 4、BIO模式下多发和多收消息
在上面的案例中，只能实现客户端发送消息，服务端接收消息，并不能实现反复的收消息和反复的发消息，只需要在客户端案例中，加上反复按照行发送消息的逻辑即可！案例代码如下：
<a name="uVSQu"></a>
### 客户端代码
```java
/**
 * 客户端
 */
public class Client {
    public static void main(String[] args) {
        try {
            //1.创建Socket对象请求服务端的连接
            Socket socket = new Socket("127.0.0.1",9999);
            //2.从Socket对象中获取一个字节输出流
            OutputStream os = socket.getOutputStream();
            //3.把字节输出流包装成一个打印流
            PrintStream ps = new PrintStream(os);
            Scanner sc = new Scanner(System.in);
            while (true){
                System.out.print("请说：");
                String msg = sc.nextLine();
                ps.println(msg);
                ps.flush();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
<a name="YJnG5"></a>
### 服务端代码
```java
/**
 * 目标：服务端可以反复的接收消息，客户端可以反复的发送消息
 */
public class Server {
    public static void main(String[] args) {
        try {
            System.out.println("===服务端启动===");
            //1.定义一个ServerSocket对象进行服务端的端口注册
            ServerSocket ss = new ServerSocket(9999);
            //2. 监听客户端的Socket连接请求
            Socket socket = ss.accept();
            //3.从socket管道中得到一个字节输入流对象
            InputStream is = socket.getInputStream();
            //4.把字节输入流包装成一个缓存字符输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String msg;
            while ((msg = br.readLine()) != null){
                System.out.println("服务端接收到：" + msg);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
<a name="Ja4VW"></a>
### 输出
```
客户端：
请说：hello
请说：what are you doing?
请说：
服务端：
===服务端启动===
服务端接收到：hello
服务端接收到：what are you doing?
```
<a name="lDDQS"></a>
## 5、BIO模式下接收多个客户端
<a name="yse36"></a>
### 概述
在上述的案例中，一个服务端只能接收一个客户端的通信请求，那么如果服务端需要处理很多个客户端的消息通信请求应该如何处理呢，此时就需要在服务端引入线程了，也就是说客户端每发起一个请求，服务端就创建一个新的线程来处理这个客户端的请求，这样就实现了一个客户端一个线程的模型，图解模式如下：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697899051964-e760f11a-606f-4fc8-970b-82a62dc4116b.jpeg)
<a name="iSmIe"></a>
### 客户端案例代码
```java
/**
 * 客户端
 */
public class Client {
    public static void main(String[] args) {
        try {
            //1.请求与服务端的Socket对象连接
            Socket socket = new Socket("127.0.0.1",9999);
            //2. 得到一个打印流
            PrintStream ps = new PrintStream(socket.getOutputStream());
            //3. 使用循环不断的发送消息给服务端接收
            Scanner sc = new Scanner(System.in);
            while (true){
                System.out.println("请说：");
                String msg =sc.nextLine();
                ps.println(msg);
                ps.flush();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="n0pRI"></a>
### 服务端案例代码
```java
/**
* 目标：服务端可以实现同时接收多个客户端的Socket通信需求
* 思路：服务端没接收到一个客户端socket请求对象之后都交给一个独立的线程来处理客户端的数据交互需求 
*/
public class Server {
    public static void main(String[] args) {
        try {
            //1.注册端口
            ServerSocket ss = new ServerSocket(9999);
            //2.定义一个死循环，负责不断的接收客户端的Socket的连接请求
            while(true){
                Socket socket = ss.accept();
                //3.创建一个独立的线程来处理与这个客户端的socket通信需求
                new ServerThreadReader(socket).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="qge9o"></a>
### 线程类
```java
public class ServerThreadReader extends Thread {
    private Socket socket;
    public ServerThreadReader(Socket socket){
        this.socket = socket;
    } 
    @Override
    public void run() {
        try {
            //从socket对象中得到一个字节输入流
            InputStream is = socket.getInputStream();
            //使用缓存字符输入流包装字节输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String msg;
            while ((msg = br.readLine()) != null){
                System.out.println(msg);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
<a name="rPLv1"></a>
### 输出
```
===client1:
请说：
ppp
请说：
你在干嘛？
请说：
我是第一个client
请说：
===client2:
请说：
lll
请说：
还钱！！！
请说：
我是第二个client
请说：
===client3:
请说：
我是第三个client
请说：
===server:
lll
ppp
你在干嘛？
还钱！！！
我是第二个client
我是第一个client
我是第三个client
```
<a name="NagIy"></a>
### 小结

- 每个Socket接收到，都会创建一个线程，线程的竞争、切换上下文影响性能
- 每个线程都会占用栈空间和CPU资源
- 并不是每个socket都进行lO操作，无意义的线程处理
- 客户端的并发访问增加时。服务端将呈现1:1的线程开销，访问量越大，系统将发生线程栈溢出
- 线程创建失败，最终导致进程宕机或者僵死，从而不能对外提供服务
<a name="MGgm5"></a>
## 6、伪异步I/O编程
<a name="rAkB0"></a>
### 概述
在上述案例中：客户端的并发访问增加时。服务端将呈现1:1的线程开销，访问量越大，系统将发生线程栈溢出，线程创建失败，最终导致进程宕机或者僵死，从而不能对外提供服务。<br />接下来采用一个伪异步I/O的通信框架，采用线程池和任务队列实现，当客户端接入时，将客户端的Socket封装成一个Task(该任务实现Java.lang.Runnable(线程任务接口）交给后端的线程池中进行处理。JDK的线程池维护一个消息队列和N个活跃的线程，对消息队列中Socket任务进行处理，由于线程池可以设置消息队列的大小和最大线程数，因此，它的资源占用是可控的，无论多少个客户端并发访问，都不会导致资源的耗尽和宕机。<br />如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697899360862-0457710a-5e94-401c-b80c-e4bc062bf72d.jpeg)
<a name="NM9Ov"></a>
### 客户端源码分析
```java
/**
 * 客户端
 */
public class Client {
    public static void main(String[] args) {
        try {
            //1.请求与服务端的Socket对象连接
            Socket socket = new Socket("127.0.0.1",9999);
            //2. 得到一个打印流
            PrintStream ps = new PrintStream(socket.getOutputStream());
            //3. 使用循环不断的发送消息给服务端接收
            Scanner sc = new Scanner(System.in);
            while (true){
                System.out.println("请说：");
                String msg =sc.nextLine();
                ps.println(msg);
                ps.flush();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
线程池处理类
```java
public class HandlerSocketServerPool {

    //1. 创建一个线程池的成员变量用于存储一个线程池对象
    private ExecutorService executorService;

    /**
     * 2.创建这个类的的对象的时候就需要初始化线程池对象
     * public ThreadPoolExecutor(int corePoolSize,
     * int maximumPoolSize,
     * long keepAliveTime,
     * TimeUnit unit,
     * BlockingQueue<Runnable> workQueue)
     */
    public HandlerSocketServerPool(int maxThreadNum, int queueSize) {
        executorService = new ThreadPoolExecutor(3, maxThreadNum, 120,
                TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(queueSize));
    }

    /**
     * 3.提供一个方法来提交任务给线程池的任务队列来暂存，等待线程池来处理
     */
    public void execute(Runnable target) {
        executorService.execute(target);
    }

}
```
<a name="zkzJZ"></a>
### Socket任务类
```java
public class ServerRunnableTarget implements Runnable {
    private Socket socket;

    public ServerRunnableTarget(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        //处理接收到的客户端socket通信需求
        try {
            //1.从socket管道中得到一个字节输入流对象
            InputStream is = socket.getInputStream();
            //2.把字节输入流包装成一个缓存字符输入流
            BufferedReader br = new BufferedReader(new InputStreamReader(is));
            String msg;
            while ((msg = br.readLine()) != null) {
                System.out.println("服务端收到：" + msg);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="suIUE"></a>
### 服务端源码
```java
/**
 * 目标：开发实现伪异步通讯架构
 * 思路：服务端没接收到一个客户端socket请求对象之后都交给一个独立的线程来处理客户端的数据交互需求
 */
public class Server {
    public static void main(String[] args) {
        try {
            //1.注册端口
            ServerSocket ss = new ServerSocket(9999);
            //2.定义一个死循环，负责不断的接收客户端的Socket的连接请求
            //初始化一个线程池对象
            HandlerSocketServerPool pool = new HandlerSocketServerPool(3, 10);
            while (true) {
                Socket socket = ss.accept();
                //3.把socket对象交给一个线程池进行处理
                //把socket封装成一个任务对象交给线程池处理
                Runnable target = new ServerRunnableTarget(socket);
                pool.execute(target);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="G32a1"></a>
### 输出
```
服务端收到：client1
服务端收到：client2
服务端收到：client3
服务端收到：client4
    java.net.SocketException: Connection reset
    at java.base/java.net.SocketInputStream.read(SocketInputStream.java:186)
    at java.base/java.net.SocketInputStream.read(SocketInputStream.java:140)
    at java.base/sun.nio.cs.StreamDecoder.readBytes(StreamDecoder.java:284)
    at java.base/sun.nio.cs.StreamDecoder.implRead(StreamDecoder.java:326)
    at java.base/sun.nio.cs.StreamDecoder.read(StreamDecoder.java:178)
    at java.base/java.io.InputStreamReader.read(InputStreamReader.java:185)
    at java.base/java.io.BufferedReader.fill(BufferedReader.java:161)
    at java.base/java.io.BufferedReader.readLine(BufferedReader.java:326)
    at java.base/java.io.BufferedReader.readLine(BufferedReader.java:392)
    at com.zhangxudong.ServerRunnableTarget.run(ServerRunnableTarget.java:23)
    at java.base/java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1128)
    at java.base/java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:628)
    at java.base/java.lang.Thread.run(Thread.java:834)
服务端收到：client5
```
<a name="eAQdD"></a>
### 小结
伪异步旧采用了线程池实现，因此避免了为每个请求创建一个独立线程造成线程资源耗尽的问题，但由于底层依然是采用的同步阻塞模型，因此无法从根采上解决问题。<br />如果单个消息处理的缓慢，或者服务器线程池中的全部线程都被阻塞，那么后续socket的I/O消息都将在队列中排队。新的Socket请求将被拒绝，客户端会发生大量连接超时。
<a name="uSoGq"></a>
## 7、基于BIO形式下的文件上传
<a name="uOrh0"></a>
### 目标
支持任意类型文件形式的上传
<a name="aUzu3"></a>
### 客户端开发
```java
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import java.net.Socket;

/**
 * 目标：实现客户端上传任意类型的文件数据给服务端保存起来
 */
public class Client {
    public static void main(String[] args) {
        try (InputStream is = new
                FileInputStream("C:\\Users\\Think\\Downloads\\1.jpg");) {
            //1.请求与服务端的Socket连接
            Socket socket = new Socket("127.0.0.1", 8888);
            //2.把字节输出流包装成一个数据输出流(DataOutputStream可以做分段数据发送)
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
            //3.先发送上传文件的后缀给服务器
            dos.writeUTF(".jpg");
            //4.把文件数据发送给服务端进行接收
            byte[] buffer = new byte[1024];
            int len;
            while ((len = is.read(buffer)) > 0) {
                dos.write(buffer, 0, len);
            }
            dos.flush();
            //通知服务端，我客户端这边的数据已经发送完毕了
            socket.shutdownOutput();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="JUSED"></a>
### 服务端开发
```java
import java.net.ServerSocket;
import java.net.Socket;

/**
 * 目标：服务端开发，可以实现接收客户端的任意类型文件，并保存到服务器端磁盘
 */
public class Server {
    public static void main(String[] args) {
        try {
            ServerSocket ss = new ServerSocket(8888);
            while (true) {
                Socket socket = ss.accept();
                //交给一个独立的线程来处理与这个客户端的文件通信需求
                new ServerReadThread(socket).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="ltS4T"></a>
### Socket线程处理类
```java
import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.UUID;

public class ServerReadThread extends Thread {
    private Socket socket;

    public ServerReadThread(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            //1.得到一个数据输入流来读取客户端发送过来的数据
            DataInputStream dis = new DataInputStream(socket.getInputStream());
            //2.读取客户端发送过来的文件类型
            String suffix = dis.readUTF();
            System.out.println("服务端已经成功接收到了文件类型：" + suffix);
            //3.定义一个字节输出管道，负责把客户端发来的文件数据写出去
            OutputStream os = new
            FileOutputStream("C:\\Users\\Think\\Desktop\\server\\"
                             + UUID.randomUUID().toString() + suffix);
            //4.从数据输入流中读取文件数据，写出到字节输出流中去
            byte[] buffer = new byte[1024];
            int len;
            while ((len = dis.read(buffer)) > 0) {
                os.write(buffer, 0, len);
            }
            os.close();
            System.out.println("服务端接收文件保存成功！");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="PxAbB"></a>
### 输出
先启动Server，后启动Client
```
服务端已经成功接收到了文件类型：.jpg
服务端接收文件保存成功！
```
<a name="z6PTF"></a>
### 小结

- 同步阻塞模式下（BIO)，客户端怎么发，服务端就必须对应的怎么收。如客户端用的是`DataOutputStream`，那么服务端就该用`DataInputStream`，客户端`dos.writeUTF(".jpg");`服务端就该`String suffix = dis.readUTF();`
- 客户端发完数据后必须通知服务端自己已经发完`socket.shutdownOutput()`，否则服务端会一直等待。
<a name="L03Yt"></a>
## 8、Java BIO模式下的端口转发思想
需求：需要实现一个客户端的消息可以发送给所有的客户端去接收。（群聊实现）<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1697899728758-748482c0-86a8-418b-b3c0-5129d79384f3.jpeg)
<a name="jjTuU"></a>
### 客户端开发
```java
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        try {
            //1.请求与服务端的Socket对象连接
            Socket socket = new Socket("127.0.0.1", 9999);
            //收消息
            Thread clientThread = new ClientReaderThread(socket);
            clientThread.start();
            while (true) {
                //发消息
                OutputStream os = socket.getOutputStream();
                PrintStream ps = new PrintStream(os);
                //3. 使用循环不断的发送消息给服务端接收
                Scanner sc = new Scanner(System.in);
                //System.out.print("client send message：");
                String msg = sc.nextLine();
                ps.println(msg);
                ps.flush();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="wRPqu"></a>
### 客户端线程处理类
```java
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class ClientReaderThread extends Thread {
    private Socket socket;

    public ClientReaderThread(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            while (true) {
                InputStream is = socket.getInputStream();
                //4.把字节输入流包装成一个缓存字符输入流
                BufferedReader br = new BufferedReader(new InputStreamReader(is));
                String msg;
                if ((msg = br.readLine()) != null) {
                    System.out.println(msg);
                }
            }
        } catch (Exception e) {
        }
    }
}
```
<a name="cN7mN"></a>
### 服务端开发
```java
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

/**
 * 目标：BIO模式下的端口转发思想-服务端实现
 * 服务端实现需求：
 * 1.注册端口
 * 2.接收客户端的socket连接，交给一个独立的线程来处理
 * 3.把当前连接的客户端socket存入到一个所谓的在线socket集合中保存
 * 4.接收客户端的消息，然后推动给当前所有的在线socket接收
 */
public class Server {
    //定义一个静态集合
    public static List<Socket> allSocketOnLine = new ArrayList<>();

    public static void main(String[] args) {
        try {
            ServerSocket ss = new ServerSocket(9999);
            while (true) {
                Socket socket = ss.accept();
                //把登录的客户端socket存入到一个在线集合中去
                allSocketOnLine.add(socket);
                //为当前登录成功的socket分配一个独立的线程来处理与之通信
                new ServerReaderThread(socket).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
<a name="xIxmm"></a>
### 服务端线程处理类
```java
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;

public class ServerReaderThread extends Thread {
    private Socket socket;

    public ServerReaderThread(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            //1.从socket中去获取当前客户端的输入流
            BufferedReader br = new BufferedReader(new
                                                   InputStreamReader(socket.getInputStream()));
            String msg;
            while ((msg = br.readLine()) != null) {
                System.out.println("服务器收到消息：" + msg);
                //2.服务端接收到了客户端的消息后，需要推送给所有的当前在线的socket
                sendMsgToAllClient(msg, socket);
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("当前有人下线了！");
            //从在线socket集合中移除本socket
            Server.allSocketOnLine.remove(socket);
        }
    }

    /**
     * 把当前客户端发送来的消息推送给全部在线的socket
     *
     * @param msg
     */
    private void sendMsgToAllClient(String msg, Socket socket) throws Exception {
        for (Socket sk : Server.allSocketOnLine) {
            //只发送给除自己以外的其他客户端
            if (socket != sk) {
                PrintStream ps = new PrintStream(sk.getOutputStream());
                ps.println(msg);
                ps.flush();
            }
        }
    }
}
```
<a name="J9izq"></a>
### 输出
先启动服务端Server，然后启动3个客户端Client
```
服务器收到消息：大家好，我是客户端一
服务器收到消息：哈哈哈哈
服务器收到消息：大家好，我是client2
服务器收到消息：嘻嘻嘻嘻
服务器收到消息：hello everyone
服务器收到消息：i am client3

#客户端一
大家好，我是客户端一 --发送
哈哈哈哈 --发送
大家好，我是client2 --接收
嘻嘻嘻嘻 --接收
hello everyone --接收
i am client3 --接收

#客户端二
大家好，我是客户端一 --接收
哈哈哈哈 --接收
大家好，我是client2 --发送
嘻嘻嘻嘻 --发送
hello everyone --接收
i am client3 --接收

#客户端三
大家好，我是客户端一 --接收
哈哈哈哈 --接收
大家好，我是client2 --接收
嘻嘻嘻嘻 --接收
hello everyone --发送
i am client3 --发送
```
<a name="yrkdS"></a>
## 9、基于BIO模式下即时通讯
基于BIO模式下的即时通信，需要解决客户端到客户端的通信，也就是需要实现客户端与客户端的端口消息转发逻辑。
<a name="DraLu"></a>
### 项目功能演示
<a name="GqpAF"></a>
#### 项目案例说明
本项目案例为即时通信的软件项目，适合基础加强的大案例，具备综合性。学习本项目案例至少需要具备如下Java SE技术点：

1. Java面向对象设计，语法设计
2. 多线程技术
3. IO流技术
4. 网络通信相关技术
5. 集合框架
6. 项目开发思维
7. Java常用api使用<br />……
<a name="zSPsU"></a>
#### 功能清单简单说明

1. 客户端登录功能可以启动客户端进行登录，客户端登录只需要输入用户名和服务端IP地址即可。
2. 在线人数实时更新客户端用户登录后，需要同步更新所有客户端的联系人信息栏。
3. 离线人数更新检测到有客户端下线后，需要同步更新所有客户端的联系人信息栏。
4. 群聊任意一个客户端的消息，可以推动给当前所有的客户端接收。
5. 私聊任意一个客户端消息，可以推动给当前所有客户端接收。
6. @消息可以选择某个员工，然后发出的消息可以@该用户，但是其他所有人都能收到消息。
7. 消息用户和消息时间点服务端可以实时记录该用户的消息时间点，然后进行消息的多路转发或则选择。
<a name="vXenZ"></a>
#### 项目结构
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1697899856306-d7cddad8-5905-4403-9ba0-3c73044f58ae.png#averageHue=%23fbfaf8&clientId=u296fe45a-d2cb-4&from=paste&height=214&id=u6771e56d&originHeight=536&originWidth=502&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=132614&status=done&style=none&taskId=u42fe6297-0b2d-48aa-ae0e-5aecddaae25&title=&width=200.8)
<a name="LA4qW"></a>
#### 技术选型分析
Java GUI，BIO
<a name="zj8oZ"></a>
### 服务端设计
<a name="imMR9"></a>
#### 服务端接收多个客户端逻辑
服务端需要接收多个客户端的接入

1. 服务端需要接收多个客户端，目前采取的策略是一个客户端对应一个服务端线程
2. 服务端除了要注册端口以外，还需要为每个客户端分配一个独立线程处理与之通信

服务端主体代码，主要进行端口注册，和接收客户端，分配线程处理该客户端请求
<a name="Mk72g"></a>
#### 服务端接收登陆消息以及监测离线
在上面实现了服务端可以接收多个客户端，然后服务端可以接收多个客户端连接，接下来要接收客户端的登陆消息。<br />需要在服务端处理客户端线程的登陆消息。需要注意的是，服务端需要接收客户端的消息可能有很多种，分别是登陆消息，群聊消息，私聊消息和@消息。这里需要约定如果客户端发送消息之前需要先发送消息的类型，类型使用信号值标志（1，2，3）。

- 1代表接收的是登陆消息
- 2代表群发| @消息
- 3代表了私聊消息

服务端的线程中有异常校验机制，一旦发现客户端下线会在异常机制中处理，然后移除当前客户端用户，把最新的用户列表发回给全部客户端进行在线人数更新。
<a name="lURFo"></a>
#### 服务端接收群聊消息
在上面实现了接收客户端的登陆消息，然后提取当前在线的全部的用户名称和当前登陆的用户名称，发送给全部在线用户更新自己的在线人数列表。接下来要接收客户端发来的群聊消息，然后推送给当前在线的所有客户端。
<a name="r5kAv"></a>
#### 服务端接收私聊消息
解决私聊消息的推送逻辑，私聊消息需要知道推送给某个具体的客户端。可以接收到客户端发来的私聊用户名称，根据用户名称定位该用户的Socket管道，然后单独推送消息给该Socket管道。<br />代码实现
<a name="NU9FI"></a>
#### 代码实现
Constants：
```java
public class Constants {
    /** 常量 */
    public static final int PORT = 7778;
    /** 协议分隔符 不知道视频中是什么有玩意儿 ♣♣㏘♣④④♣, 简单的用逗号分隔，功能可以实现*/
    public static final String SPILIT = ",";
}
```
ServerChat：
```java
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class ServerChat {
    /**
     * 定义一个集合存放所有在线的socket
     * 在线集合只需要一个，存储客户端socket的同时还需要知道这个Socket的客户端名称
     */
    public static Map<Socket, String> onLineSockets = new HashMap<>();

    public static void main(String[] args) {
        try {
            //注册端口
            ServerSocket serverSocket = new ServerSocket(Constants.PORT);
            //循环一直等待所有可能的客户端连接
            while (true) {
                Socket socket = serverSocket.accept();
                //把客户端的socket管道单独配置一个线程来处理
                new ServerReader(socket).start();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
ServerReader：
```java
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Set;

public class ServerReader extends Thread {
    private Socket socket;

    public ServerReader(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        DataInputStream dis = null;
        try {
            dis = new DataInputStream(socket.getInputStream());
            //1.循环一直等待客户端的消息
            while (true) {
                //2.读取当前的消息类型：登录，群发，私聊，@消息
                int flag = dis.readInt();
                if (flag == 1) {
                    //先将当前登录的客户端socket存到在线人数的socket集合中
                    String name = dis.readUTF();
                    System.out.println(name + "----->" +
                            socket.getRemoteSocketAddress());
                    ServerChat.onLineSockets.put(socket, name);
                }
                writeMsg(flag, dis);
            }
        } catch (Exception e) {
            System.out.println("---有人下线了---");
            ServerChat.onLineSockets.remove(socket);
            try {
                //重新更新在线人数并发给所有客户端
                writeMsg(1, dis);
            } catch (Exception e1) {
                e1.printStackTrace();
            }
        }
    }

    private void writeMsg(int flag, DataInputStream dis) throws Exception {
        //DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
        //定义一个变量存放最终的消息形式
        String msg = null;
        if (flag == 1) {
            //读取所有在线人数发给所有客户端去更新自己的在线人数列表
            StringBuilder rs = new StringBuilder();
            /** onlineNames = [波仔,zhangsan,波妞]*/
            Collection<String> onlineNames = ServerChat.onLineSockets.values();
            //判断是否存在在线人数
            if (onlineNames != null && onlineNames.size() > 0) {
                for (String name : onlineNames) {
                    rs.append(name + Constants.SPILIT);
                }
                //波仔003197♣♣㏘♣④④♣zhangsan003197♣♣㏘♣④④♣波妞003197♣♣㏘♣④④♣
                //去掉最后一个分隔符
                msg = rs.substring(0, rs.lastIndexOf(Constants.SPILIT));
                //将消息发送给所有的客户端
                sendMsgToAll(flag, msg);
            }
        } else if (flag == 2 || flag == 3) {
            //读到消息 群发的 或则 @消息
            String newMsg = dis.readUTF();//消息
            //得到发件人
            String sendName = ServerChat.onLineSockets.get(socket);
            // 内容--
            StringBuilder msgFinal = new StringBuilder();
            //时间
            SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss EEE");
            if (flag == 2) {//群发消息 和 @消息
                msgFinal.append(sendName).append("").append(sdf.format(System.currentTimeMillis() * 2)).append("\r\n");
                msgFinal.append(" ").append(newMsg).append("\r\n");
                sendMsgToAll(flag, msgFinal.toString());
            } else if (flag == 3) {//私聊消息
                msgFinal.append(sendName).append("").append(sdf.format(System.currentTimeMillis() * 2)).append(" 对您私发\r\n");
                msgFinal.append(" ").append(newMsg).append("\r\n");
                //私发
                //得到发给谁
                String destName = dis.readUTF();
                sendMsgToOne(destName, msgFinal.toString());
            }
        }
    }

    /**
     * @param destName 对谁私发
     * @param msg      发的消息内容
     * @throws Exception
     */
    private void sendMsgToOne(String destName, String msg) throws Exception {
        //拿到所有在线的socket管道 给这些管道写出消息
        Set<Socket> allOnLineSockets = ServerChat.onLineSockets.keySet();

        for (Socket sk : allOnLineSockets) {
            //得到当前需要私发的Socket
            //只对这个名字对应的socket私发消息
            if (ServerChat.onLineSockets.get(sk).trim().equals(destName)) {
                DataOutputStream dos = new
                        DataOutputStream(sk.getOutputStream());
                dos.writeInt(2);//消息类型
                dos.writeUTF(msg);
                dos.flush();
            }
        }
    }

    private void sendMsgToAll(int flag, String msg) throws Exception {
        //拿到所有的在线socket管道 给这些管道写出消息
        Set<Socket> allOnLineSockets = ServerChat.onLineSockets.keySet();
        for (Socket sk : allOnLineSockets) {
            DataOutputStream dos = new DataOutputStream(sk.getOutputStream());
            dos.writeInt(flag);//消息类型
            dos.writeUTF(msg);
            dos.flush();
        }
    }
}
```
<a name="ZzVb1"></a>
### 客户端设计
<a name="WPlBV"></a>
#### 启动客户端界面，登录，刷新在线
启动客户端界面，登陆，刷新在线人数列表<br />客户端界面主要是GUI设计，主体页面分为登陆界面和聊天窗口，以及在线用户列表。<br />登陆输入服务端ip和用户名后，要请求与服务端的登陆，然后立即为当前客户端分配一个读线程处理客户端的读数据消息。因为客户端可能随时会接收到服务端那边转发过来的各种即时消息信息。客户端登陆完成，服务端收到登陆的用户名后，会立即发来最新的用户列表给客户端更新。
<a name="kvc6T"></a>
#### 客户端发送消息逻辑
目标：客户端发送群聊消息，@消息，以及私聊消息。<br />实现步骤<br />客户端启动后，在聊天界面需要通过发送按钮推送群聊消息，@消息，以及私聊消息。
<a name="k4P3m"></a>
#### 代码实现
ClientChat：
```java
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.DataOutputStream;
import java.net.Socket;

/**
 * 客户端界面
 */
public class ClientChat implements ActionListener {
    /**
     * 1.设计界面
     */
    private JFrame win = new JFrame();
    /**
     * 2.消息内容框架
     */
    public JTextArea smsContent = new JTextArea(23, 50);
    /**
     * 3.发送消息的框
     */
    private JTextArea smsSend = new JTextArea(4, 40);
    /** 4.在线人数的区域 */
    /** 存放人的数据 */
    /**
     * 展示在线人数的窗口
     */
    public JList<String> onLineUsers = new JList<>();
    //是否私聊按钮
    private JCheckBox isPrivateBn = new JCheckBox("私聊");
    //消息按钮
    private JButton sendBn = new JButton("发送");
    //登录界面
    private JFrame loginView;
    private JTextField ipEt, nameEt, idEt;
    private Socket socket;

    public static void main(String[] args) {
        new ClientChat().initView();
    }

    private void initView() {
        /** 初始化聊天窗口的界面 */
        win.setSize(650, 600);
        /** 展示登录界面 */
        displayLoginView();
        /** 展示聊天界面 */
        //displayChatView();
    }

    private void displayChatView() {
        JPanel bottomPanel = new JPanel(new BorderLayout());
        //---------------------------------------------------
        //将消息框和按钮添加到窗口的底端
        win.add(bottomPanel, BorderLayout.SOUTH);
        bottomPanel.add(smsSend);
        JPanel btns = new JPanel(new FlowLayout(FlowLayout.LEFT));
        btns.add(sendBn);
        btns.add(isPrivateBn);
        bottomPanel.add(btns, BorderLayout.EAST);
        //-----------------------------------------------
        //给发送消息按钮绑定点击事件监听器
        //将展示消息区centerPanel添加到窗口的中间
        smsContent.setBackground(new Color(0xdd, 0xdd, 0xdd));
        //让展示消息区可以滚动
        win.add(new JScrollPane(smsContent), BorderLayout.CENTER);
        smsContent.setEditable(false);
        //-------------------------------------------------
        //用户列表和是否私聊放到窗口的最右边
        Box rightBox = new Box(BoxLayout.Y_AXIS);
        onLineUsers.setFixedCellWidth(120);
        onLineUsers.setVisibleRowCount(13);
        rightBox.add(new JScrollPane(onLineUsers));
        win.add(rightBox, BorderLayout.EAST);
        //-------------------------------------------------
        //关闭窗口退出当前程序
        win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        win.pack();//swing 加上这句 就可以拥有关闭窗口的功能
        /** 设置窗口居中，显示出来 */
        setWindowCenter(win, 650, 600, true);
        //发送按钮绑定点击事件
        sendBn.addActionListener(this);
    }

    private void displayLoginView() {
        /**
         * 先让用户进行登录
         * 服务端ip
         * 用户名
         * id
         */
        /** 显示一个qq的登录框 */
        loginView = new JFrame("登录");
        loginView.setLayout(new GridLayout(3, 1));
        loginView.setSize(400, 230);
        JPanel ip = new JPanel();
        JLabel label = new JLabel("IP:");
        ip.add(label);
        ipEt = new JTextField(20);
        ip.add(ipEt);
        loginView.add(ip);
        JPanel name = new JPanel();
        JLabel label1 = new JLabel("姓名:");
        name.add(label1);
        nameEt = new JTextField(20);
        name.add(nameEt);
        loginView.add(name);
        JPanel btnView = new JPanel();
        JButton login = new JButton("登录");
        btnView.add(login);
        JButton cancle = new JButton("取消");
        btnView.add(cancle);
        loginView.add(btnView);
        //关闭窗口退出当前程序
        loginView.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setWindowCenter(loginView, 400, 260, true);
        /** 给登录和取消绑定点击事件 */
        login.addActionListener(this);
        cancle.addActionListener(this);
    }

    private void setWindowCenter(JFrame frame, int width, int height, boolean
            flag) {/** 得到所在系统所在屏幕的宽高 */
        Dimension ds = frame.getToolkit().getScreenSize();
        /** 拿到电脑的宽 */
        int width1 = ds.width;
        /** 高 */
        int height1 = ds.height;
        System.out.println(width1 + "*" + height1);
        /** 设置窗口的左上角坐标 */
        frame.setLocation(width1 / 2 - width / 2, height1 / 2 - height / 2);
        frame.setVisible(flag);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        //得到点击的事件源
        JButton btn = (JButton) e.getSource();
        switch (btn.getText()) {
            case "登录":
                String ip = ipEt.getText().toString();
                String name = nameEt.getText().toString();
                //校验参数是否为空
                //错误提示
                String msg = "";
                //12.1.2.0
                // \d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\
                if (ip == null ||
                        !ip.matches("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}")) {
                    msg = "请输入合法的服务器ip地址";
                } else if (name == null || !name.matches("\\S{1,}")) {
                    msg = "姓名必须1个字符以上";
                }
                if (!msg.equals("")) {
                    //msg有内容说明参数有为空
                    //参数一：弹出放到那个窗口里面
                    JOptionPane.showMessageDialog(loginView, msg);
                } else {
                    try {
                        //参数都合法了
                        //当前登录的用户，去服务器登录
                        /** 先把当前用户的名称展示到界面*/
                        win.setTitle(name);
                        //去服务端登录连接一个socket管道
                        socket = new Socket(ip, Constants.PORT);
                        //为客户端的socket分配一个线程 专门负责收消息
                        new ClientReader(this, socket).start();
                        //带上用户信息过去
                        DataOutputStream dos = new
                                DataOutputStream(socket.getOutputStream());
                        dos.writeInt(1);//登录消息
                        dos.writeUTF(name.trim());
                        dos.flush();
                        //关闭当前窗口 弹出聊天界面
                        loginView.dispose();//登录窗口销毁
                        displayChatView();//展示了聊天窗口了
                    } catch (Exception e1) {
                        e1.printStackTrace();
                    }
                }
                break;
            case "取消":
                /** 退出系统*/
                System.exit(0);
                break;
            case "发送":
                //得到发送消息的内容
                String msgSend = smsSend.getText().toString();
                if (!msgSend.trim().equals("")) {
                    //发消息给服务端
                    try {
                        //判断是否对谁发消息
                        String selectName = onLineUsers.getSelectedValue();
                        int flag = 2;// 群发 @消息
                        if (selectName != null && !selectName.equals("")) {
                            msgSend = ("@" + selectName + "," + msgSend);
                            //判断是否选中私发
                            if (isPrivateBn.isSelected()) {
                                //私发
                                flag = 3;//私发消息
                            }
                        }
                        DataOutputStream dos = new
                                DataOutputStream(socket.getOutputStream());
                        dos.writeInt(flag);//群发消息 发送给所有人
                        dos.writeUTF(msgSend);
                        if (flag == 3) {
                            //告诉服务器端我对谁私发
                            dos.writeUTF(selectName.trim());
                        }
                        dos.flush();
                    } catch (Exception e1) {
                        e1.printStackTrace();
                    }
                }
                smsSend.setText(null);
                break;
        }
    }
}
```
ClientReader：
```java
import java.io.DataInputStream;
import java.net.Socket;
import java.util.Arrays;

public class ClientReader extends Thread {
    private Socket socket;
    private ClientChat clientChat;

    public ClientReader(ClientChat clientChat, Socket socket) {
        this.clientChat = clientChat;
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            DataInputStream dis = new DataInputStream(socket.getInputStream());
            /** 循环一直等待客户端的消息 */
            while (true) {
                /** 读取当前的消息类型： 登录，群发，私聊，@消息 */
                int flag = dis.readInt();
                if (flag == 1) {
                    //在线人数消息回来了
                    String nameDatas = dis.readUTF();
                    System.out.println(nameDatas);
                    //展示到在线人数的界面
                    String[] names = nameDatas.split(Constants.SPILIT);
                    System.out.println(Arrays.toString(names));
                    clientChat.onLineUsers.setListData(names);
                } else if (flag == 2) {
                    //群发，私聊 ，@消息 都是直接显示的
                    String msg = dis.readUTF();
                    clientChat.smsContent.append(msg);
                    //让消息界面滚动到低端
                    clientChat.smsContent.setCaretPosition(clientChat.smsContent.getText().length()
                                                          );
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
