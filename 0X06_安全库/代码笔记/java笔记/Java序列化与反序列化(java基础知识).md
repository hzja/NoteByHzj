<a name="tmYSN"></a>
## 为什么要序列号和反序列化

Web 服务器中的 Session 会话对象，当有10万用户并发访问，就有可能出现10万个 Session 对象，显然这种情况内存可能是吃不消的。<br />于是 Web 容器就会把一些 Session 先序列化，让他们离开内存空间，序列化到硬盘中，当需要调用时，再把保存在硬盘中的对象还原到内存中。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612369649432-fca3302a-41ea-4748-857e-d62e545e287a.png#align=left&display=inline&height=273&originHeight=364&originWidth=1164&size=23814&status=done&style=none&width=873)<br />在一个平台上序列化的对象可以在另一个完全不同的平台上反序列化该对象

<a name="0VqU0"></a>
## 实现反序列化和序列化对象的方法
ObjectInputStream 和 ObjectOutputStream类<br />↓                                ↓<br />对象输入流                    对象输出流

ObjectOutputStream 类中：通过使用 writeObject(Object object) 方法，将对象以二进制格式进行写入。<br />ObjectInputStream 类中：通过使用 readObject()方法，从输入流中读取二进制流，转换成对象。

一个类的对象要想序列化成功，必须满足两个条件：

- 该类必须实现 java.io.Serializable 接口。
- 该类的所有属性必须是可序列化的。

SerialVersionUid 序列化版本号的作用是用来区分我们所编写的类的版本，用于判断反序列化时类的版本是否一直，如果不一致会出现版本不一致异常。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612368501782-d0103594-6012-4056-bec8-da8f03a0b552.png#align=left&display=inline&height=216&originHeight=431&originWidth=920&size=32342&status=done&style=none&width=460)

<a name="wP8Fq"></a>
## 实验过程代码如下:
定义一个fanxuliehua类，实现Serializable接口<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612368717583-f9b8475f-7c71-4b0d-b9b0-26a66b834348.png#align=left&display=inline&height=447&originHeight=596&originWidth=676&size=23391&status=done&style=none&width=507)<br />序列化和反序列化Person类对象<br />序列化，使用ObjectOutputStream 类，通过使用 writeObject(Object object) 方法，将对象以二进制格式进行写入D盘中的aufeng.txt文件<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612368825678-cd6f0f7f-7d73-4423-bc44-8abf0bb26e24.png#align=left&display=inline&height=355&originHeight=473&originWidth=1140&size=33799&status=done&style=none&width=855)<br />反序列化使用ObjectInputStream 类，通过使用 readObject()方法，从(D盘中的aufeng.txt文件)输入流中读取二进制流，转换成对象。<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612368866043-77202d0e-2025-4e52-9c68-f7e269bfa054.png#align=left&display=inline&height=103&originHeight=137&originWidth=948&size=10261&status=done&style=none&width=711)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612429423955-31389d3c-a20d-4923-b650-6b11ea6945ac.png#align=left&display=inline&height=138&originHeight=138&originWidth=338&size=4723&status=done&style=none&width=338)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612369659180-ee9663d2-ad31-4ccd-8346-f4eed631e4c3.png#align=left&display=inline&height=273&originHeight=364&originWidth=1164&size=23814&status=done&style=none&width=873)
<a name="oBqbF"></a>
## 好文章

[https://www.sec-in.com/article/392](https://www.sec-in.com/article/392)
