<a name="bWzaD"></a>
### 一、思维导图
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629877623325-c4349945-b0b1-48a0-8b9d-68b3b9c74743.png#clientId=u0ff22bd3-d39b-4&from=paste&id=uf028493e&originHeight=950&originWidth=1114&originalType=url&ratio=1&status=done&style=none&taskId=u56b9e047-a787-443c-8567-cb7151829ae)<br />Java中API实现：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629878012763-c1184b4c-36fc-43ae-8011-10904cc1c533.png#clientId=u0ff22bd3-d39b-4&from=paste&height=282&id=u0d4beedf&originHeight=564&originWidth=1757&originalType=binary&ratio=1&size=290797&status=done&style=none&taskId=ufac53a1c-21ec-44f4-b77d-da6cefe41e1&width=878.5)
<a name="Q384j"></a>
### 二、序列化理解
```bash
#序列化和反序列化

序列化(Serialization): 将对象的状态信息转换为可以存储或传输的形式的过程。在序列化期间，对象将其当前状态写入到临时或持久性存储区。

反序列化：从存储区中读取该数据，并将其还原为对象的过程，成为反序列化。
```
<a name="IWneP"></a>
### 三、演示案例

- Java 反序列化及命令执行代码测试   
- WebGoat_Javaweb 靶场反序列化测试  


**补充知识点**<br />下方的特征可以作为序列化的标志参考:<br />一段数据以`rO0AB`开头，你基本可以确定这串就是JAVA序列化base64加密的数据。<br />或者如果以aced开头，那么他就是这一段java序列化的16进制。|


<a name="geb8H"></a>
#### webgoat反序列
```bash
jiang@ubuntu:~$ docker pull webgoat/webgoat-8.0
jiang@ubuntu:~$ docker run -d -p 8080:8080 webgoat/webgoat-8.0:latest
```
[http://10.1.1.7:8080/WebGoat/login](http://10.1.1.7:8080/WebGoat/login)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629880320441-1716fda7-b536-4a65-9d4c-c0a20a6f094b.png#clientId=u0ff22bd3-d39b-4&from=paste&height=259&id=u796dfd03&originHeight=518&originWidth=1602&originalType=binary&ratio=1&size=92808&status=done&style=none&taskId=u0ea831cc-c102-4661-899f-5dd7cff693c&width=801)<br />![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1629886664541-12f82ed3-a7ad-4eac-8281-62ec2d5437b5.png#clientId=u0ff22bd3-d39b-4&from=paste&id=u22654962&originHeight=223&originWidth=903&originalType=url&ratio=1&status=done&style=none&taskId=ue51c426d-091d-440b-83d6-ea9a3671c3b)

