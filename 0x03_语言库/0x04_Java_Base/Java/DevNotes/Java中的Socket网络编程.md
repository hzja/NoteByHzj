Java Socket<br />网络编程有三个要素，分别是IP地址、端口号和通信协议。<br />首先，需要了解一下IP地址、端口号、通信协议的相关知识。
<a name="Y3qQx"></a>
## 一、IP地址
网络中的计算机使用IP地址来进行唯一标识，IP地址有IPv4和IPv6两种类型。IPv4采用十进制或二进制表示形式，十进制是一种比较常用的表示形式，如192.168.1.131，IPv6采用十六进制表示形式，一般不常用。<br />如何查看IP地址相关信息：<br />在Windows系统下，打开cmd，输入命令`ipconfig`，按回车即可查看。在Linux或Mac系统下，打开终端，使用`ifconfig`命令，按回车即可查看。
<a name="bh52Z"></a>
## 二、端口号
端口号是计算机中的应用程序的一个整数数字标号，用来区分不同的应用程序。<br />0 ~ 1024 为被系统使用或保留的端口号，0 ~ 65535为有效的端口号，也就是说要对一些程序定义端口号的时候，要选择1024 ~ 65535范围内的整数数字。<br />比如，以前学过的MySQL的端口号是3306，SQLServer的端口号是1433，查了一下Oracle的端口号是1521。<br />一定要把这些数据库对应的端口号，藏在深深的脑海里，以后在连接数据库的时候，会使用到端口号。
<a name="te3Db"></a>
## 三、通信协议
说的通俗一点，通信协议就是网络通信中的规则，分为TCP协议和UDP协议两种。
<a name="XVBfx"></a>
### 第一种：TCP协议
英文名：Transmission Control Protocol 中文名：传输控制协议 协议说明：TCP是一种面向连接的、可靠的、基于字节流的传输层通信协议。<br />举例：打电话，需要双方都接通，才能进行对话<br />特点：效率低，数据传输比较安全
<a name="nV6EG"></a>
### 第二种：UDP协议
英文名：User Datagram Protocol 中文名：数据报协议 协议说明：UDP是一种面向无连接的传输层通信协议。<br />举例：发短信，不需要双方建立连接，But，数据报的大小应限制在64k以内<br />特点：效率高，数据传输不安全，容易丢包
<a name="B3kkD"></a>
## 四、三要素关系图与网络模型图
<a name="qbh8X"></a>
### 1、网络编程三要素关系图
![2021-05-30-00-20-12-271851.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622305568920-c0939a4d-4b36-463d-940f-903469f16e73.png#clientId=ua6efaa88-2998-4&from=ui&id=ubed14e82&originHeight=434&originWidth=1080&originalType=binary&size=80871&status=done&style=shadow&taskId=uf3362f69-3f28-4cde-929d-bc2e5b4dc1c)<br />注：图中端口号、IP地址为演示，并非真实
<a name="l09vD"></a>
### 2、OSI参考模型与TCP/IP参考模型
![2021-05-30-00-20-12-411994.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622305581980-74020263-a4f0-4d88-8051-881745fe65d3.jpeg#clientId=ua6efaa88-2998-4&from=ui&id=ue78ff550&originHeight=764&originWidth=784&originalType=binary&size=49064&status=done&style=shadow&taskId=uefab4390-063b-4fe1-adfd-335771ee19a)
<a name="x9vXW"></a>
## 五、TCP编程
TCP是基于字节流的传输层通信协议，所以TCP编程是基于IO流编程。<br />对于客户端，需要使用`Socket`类来创建对象。对于服务器端，需要使用`ServerSocket`来创建对象，通过对象调用`accept()`方法来进行监听是否有客户端访问。<br />客户端与服务器端图解：<br />![2021-05-30-00-20-12-539169.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622305595225-1459551d-ab02-4c05-82fe-d791f9a33230.jpeg#clientId=ua6efaa88-2998-4&from=ui&id=ub070df20&originHeight=447&originWidth=1080&originalType=binary&size=52622&status=done&style=shadow&taskId=u7851281e-1058-470b-b674-889f85da8e9)<br />客户端与服务器端实现步骤：<br />前提：创建一个项目，在项目中创建两个模块（model），一个模块用来放客户端相关代码，一个模块用来放服务器端相关代码。<br />目录结构如下图：<br />![2021-05-30-00-20-12-669198.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622305606576-39e004a6-c48d-4a9e-a3e9-9e1edd04ebc5.jpeg#clientId=ua6efaa88-2998-4&from=ui&id=ue8c64d2e&originHeight=706&originWidth=654&originalType=binary&size=37664&status=done&style=shadow&taskId=uca74beae-d5c1-45f5-a0a3-15c10af8892)
<a name="U6NxF"></a>
### 客户端：
1、创建`Socket`对象，并指定服务器端应用程序的端口号和服务器端主机的IP地址。<br />2、使用`Socket`的对象调用`getOutputStream()`方法来获取字节输出流对象。<br />3、调用字节输出流的`write(byte[] buf)`或者`write(int b)`向服务器发送指定数据。<br />4、记得关闭流。
<a name="emyqF"></a>
### 服务器端：
1、创建`ServerSocket`对象，并指定该应用程序的端口号，端口号必须和客户端指定的端口号一样。<br />2、使用`ServerSocket`对象的`accept()`方法来监听客户端发送过来的请求，返回值为Socket对象。<br />3、调用`Socket`对象的`getInputStream()`方法获取字节输入流对象<br />4、调用字节输入流对象的`read(byte[] buf)`或`read()`方法获取数据。<br />5、记得关闭流。
<a name="VUjSo"></a>
### 实例：
客户端向服务器端发送信息，并显示在服务器端。
<a name="cUEaZ"></a>
#### Client类（客户端）
```java
package cn.tkrnet.client;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class Client {
    public static void main(String[] args) throws IOException {
        
        //创建Socket对象，指定要发送到服务器端的IP地址，以及服务器端应用程序接收的端口号
        //localhost代表本机IP地址
        Socket client = new Socket("localhost",9000);
        
        //获取输出流，用于向服务器端发送数据
        OutputStream os = client.getOutputStream();
        
        os.write("Java is my friend !".getBytes());
        System.out.println("信息已发送");

        //关闭流
        os.close();
        client.close();
    }
}
```
<a name="ih4Lg"></a>
#### Server类（服务器端）
```java
package cn.tkrnet.server;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
    public static void main(String[] args) throws IOException {
        System.out.println("--服务器端已开启--");

        //创建ServerSocket对象，这里的端口号必须与客户端的端口号相同
        ServerSocket server = new ServerSocket(9000);

        //调用方法accept()，用来监听客户端发来的请求
        Socket socket = server.accept();

        //获取输入流对象
        InputStream is = socket.getInputStream();

        //读取输入流中的数据
        int b = 0;
        while ((b =is.read()) != -1){
            System.out.print((char)b);
        }
        //关闭流
        is.close();
        socket.close();
        server.close();
    }
}
```
:::tips
提示：在运行程序时，一定要先运行服务器端的程序代码，再运行客户端的程序代码。因为客户端要向服务器发送请求，前提是服务器端要处于开启状态。
:::
Server类（服务器端）运行结果：
```java
--服务器端已开启--
```
Client类（客户端）运行结果：
```java
信息已发送
```
Client类（客户端）运行后，Server类（服务器端）收到信息，运行结果：
```java
--服务器端已开启--
Java is my friend !
```
<a name="FMh3u"></a>
### 实例分析：
服务器端启动后，服务器端的`accept()`方法一直处于监听状态，直到客户端连接了服务器，服务器端再从流中读取客户端发来的数据。
<a name="jiJjR"></a>
## 六、UDP编程
UDP使用数据报进行数据传输，没有客户端与服务器端之分，只有发送方与接收方，两者哪个先启动都不会报错，但是会出现数据丢包现象。发送的内容有字数限制，大小必须限制在64k以内。<br />发送方与接收方实现步骤：<br />前提：创建一个项目，在项目中创建两个模块（model），一个模块用来放发送方相关代码，一个模块用来放接收方相关代码。
<a name="xua49"></a>
### 目录结构如下图
![2021-05-30-00-20-12-811910.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1622305623465-fca93391-6265-493f-a09e-1fc96e948773.jpeg#clientId=ua6efaa88-2998-4&from=ui&id=u4b9ad55a&originHeight=676&originWidth=602&originalType=binary&size=44278&status=done&style=none&taskId=u627190c3-97cc-45e1-8c8a-f0edadbb013)
<a name="blOS3"></a>
### 发送方：
1、创建`DatagramSocket`对象，可以指定应用程序的端口号，也可以不指定。<br />2、准备需要发送的数据<br />3、创建`DatagramPacket`对象，用来对发送的数据进行打包，需要指定发送内容、发送多少、发送到哪里和接收方的端口号四个参数。<br />4、调用`DatagramSocket`对象的`send()`方法发送数据。<br />5、记得关闭流。
<a name="UywHz"></a>
### 接收方：
1、创建`DatagramSocket`对象，指定接收方的端口号，这个必须指定。<br />2、创建一个`byte`类型数组，用来接收发送方发送过来的数据。<br />3、创建`DatagramPacket`对象，准备接收数据。<br />4、调用`DatagramSocket`对象的`receive()`方法用于接收数据。<br />5、使用`String`类的构造方法将`byte`类型的数组中的数据转化成`String`类型并显示。<br />6、记得关闭流。
<a name="DJBO0"></a>
### 实例：
发送方发送信息，接收方接收信息，并显示。
<a name="NVQiE"></a>
#### Sender类（发送方）
```java
package cn.tkrnet.Sender;

import java.io.IOException;
import java.net.*;

public class Sender {
    public static void main(String[] args) throws IOException {

        //创建接受或发送的数据报套接字，并指定发送方的端口号为7770
        DatagramSocket ds = new DatagramSocket(7770);   //端口号也可以不指定
        System.out.println("---发送方---");

        //创建数据报对象，用来发送数据
        byte[] b = "Java is my friend ！".getBytes();

        //8800为接收方的端口号，netAddress.getByName("localhost")是获取主机的IP地址
        DatagramPacket dp = new DatagramPacket(b,b.length, InetAddress.getByName("localhost"),7788);

        ds.send(dp);    //发送数据报
        System.out.println("数据已发送");
        //关闭流
        ds.close();
    }
}
```
<a name="R3vE2"></a>
#### Receiver类（接收方）
```java
package cn.tkrnet.receiver;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Receiver {
    public static void main(String[] args) throws IOException {
        System.out.println("---接收方---");

        //创建数据报套接字对象，指定的端口号要和发送方发送数据的端口号相同
        // （不是发送方的端口号7770，是发送方发送数据的端口号7788）
        DatagramSocket ds = new DatagramSocket(7788);

        //创建接收数据报的对象
        byte[] b = new byte[1024];
        DatagramPacket dp = new DatagramPacket(b,b.length);

        //接收数据
        ds.receive(dp);
        System.out.println(new String(b,0,dp.getLength()));
        //关闭流
        ds.close();
    }
}
```
:::tips
提示：在运行程序时，先运行发送方程序，还是先运行接收方程序都不会报错，但是有可能会出现数据丢包，一般都先运行接收方的程序代码，再运行发送方的程序代码。
:::
Receiver类（接收方）运行结果：
```java
---接收方---
```
Sender类（发送方）运行结果：
```java
---发送方---
数据已发送
```
Sender类（发送方）运行后，Receiver类（接收方）接收到信息，运行结果：
```java
---接收方---
Java is my friend ！
```
<a name="Kp25a"></a>
### 实例分析：
只有接收方先启动运行，才会存在端口号为7788的程序，发送方才能发送数据到指定端口号7788，接收方才能接收数据。
