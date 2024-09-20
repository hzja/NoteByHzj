JavaSpringBoot
<a name="IZiUS"></a>
### 使用JDK自带的工具生成证书

1. 确保安装了JDK并正确配置了环境变量；
2. 进入JAVA_HOME目录中的bin目录；
3. 在这个目录下执行
```bash
# keytool -genkey -alias (别名) -dname "CN=(姓名),OU=(组织单位名称),O=(组织名称),L=(城市名称),ST=(省),C=(国家)" -storetype (密钥仓库类型) -keyalg (生证书的算法名称) -keysize (密钥长度,证书大小) -keystore (指定生成证书的位置和证书名称) -validity (证书有效期，天单位)
keytool -genkey -alias uublog -dname "CN=Lhc,OU=SCYD,O=SCYD,L=CD,ST=SiChuan,C=CN" -storetype PKCS12 -keyalg RSA -keysize 2048 -keystore keystore.p12 -validity 365
```

4. 获取名为keystore.p12的证书，将其放入resources目录中，并在application.properties或application.yml中配置
```
#https端口号.  
server.port: 443  
#证书的路径.  
server.ssl.key-store: classpath:keystore.p12  
#证书密码，请修改为您自己证书的密码.  
server.ssl.key-store-password: 123456  
#秘钥库类型  
server.ssl.keyStoreType: PKCS12   
#证书别名  
#server.ssl.keyAlias: uublog
```
若指定的端口非443，则在访问的时添加具体的端口号

5. 将http重定向到https
```java
import org.apache.catalina.Context;  
import org.apache.catalina.connector.Connector;  
import org.apache.tomcat.util.descriptor.web.SecurityCollection;  
import org.apache.tomcat.util.descriptor.web.SecurityConstraint;  
import org.springframework.boot.web.embedded.tomcat.TomcatServletWebServerFactory;  
import org.springframework.context.annotation.Bean;  
import org.springframework.context.annotation.Configuration;  
  
/**   
 * @ClassName: SSLUtils   
 * @Description: Http重定向到Https  
 */  
@Configuration  
public class SSLUtils {  
  
    @Bean  
    public Connector connector() {  
        Connector connector = new Connector("org.apache.coyote.http11.Http11NioProtocol");  
        connector.setScheme("http");  
        connector.setSecure(false);  
        connector.setPort(80);  
        connector.setRedirectPort(443);  
        return connector;  
    }  
  
    @Bean  
    public TomcatServletWebServerFactory tomcatServletWebServerFactory(Connector connector) {  
        TomcatServletWebServerFactory webServerFactory = new TomcatServletWebServerFactory() {  
            @Override  
            protected void postProcessContext(Context context) {  
                SecurityConstraint securityConstraint = new SecurityConstraint();  
                securityConstraint.setUserConstraint("CONFIDENTIAL");  
                SecurityCollection securityCollection = new SecurityCollection();  
                securityCollection.addPattern("/*");  
                securityConstraint.addCollection(securityCollection);  
                context.addConstraint(securityConstraint);  
            }  
        };  
        webServerFactory.addAdditionalTomcatConnectors(connector);  
        return webServerFactory;  
    }  
}
```

6. 部署

若部署环境为windows，直接启动即可；若部署环境是Linux，先开通443端口，在进行部署
```bash
# 判断443端口是否已经开放  
firewall-cmd --query-port=443/tcp  
# 若未开放，则配置开启，并重新加载配置  
firewall-cmd --add-port=443/tcp --permanent  
firewall-cmd --reload
```

7. 以JDK生成证书的方式足够简单，但在Chrome和Edge浏览器上依旧显示不安全，因此下面就使用FreeSSL提供的免费证书

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663990549204-bf400eae-b928-447e-a91d-82a43fb32b9c.png#clientId=u269cb811-6ca7-4&from=paste&id=u8b108abe&originHeight=34&originWidth=648&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufeb4fbe4-000f-4f26-85f7-95de4f748ea&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663990549069-aa8149cd-4529-429a-9c8c-707fb386bab6.png#clientId=u269cb811-6ca7-4&from=paste&id=YQwA1&originHeight=104&originWidth=783&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u8e551394-a044-4474-9866-d7c0868cde0&title=)
<a name="D4HPl"></a>
### 使用FreeSSL提供的证书
FreeSSL.cn 是一个免费提供 HTTPS 证书申请、HTTPS 证书管理和 HTTPS 证书到期提醒服务的网站，旨在推进 HTTPS 证书的普及与应用，简化证书申请的流程。

1. 首先得有一个域名，推荐国外的GoDaddy，价格合适且不需要备案就可以进行解析；

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1663990760133-14a5f145-7188-41f8-94ae-4a8d51f15730.png#clientId=u269cb811-6ca7-4&from=paste&height=713&id=u0424b4e9&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1678483&status=done&style=none&taskId=uc56e27ae-b3ed-4540-8313-b3570981a54&title=&width=1536)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290478882-ddc9398a-fea2-4fe6-ad15-a64478a274a4.png?x-oss-process=image%2Fresize%2Cw_914%2Climit_0#from=url&id=wMTcg&originHeight=406&originWidth=914&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=)

2. FreeSSL网站上有对不同品牌的介绍了和证书生成方式的介绍，请参阅网站；
3. 一顿操作之后，已经下载证书到本地了，将证书放入resources目录中，并在配置文件中添加配置
```
#https端口号.  
server.port: 443  
#证书的路径.  
server.ssl.key-store: classpath:***.jks  
#证书密码，请修改为您自己证书的密码.  
server.ssl.key-store-password: ***  
#秘钥库类型  
server.ssl.keyStoreType: JKS
```

4. 将http重定向到https，代码同上
5. 打包并部署

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663990549170-e49d233f-34ff-43ae-8e34-955ee2859dac.png#clientId=u269cb811-6ca7-4&from=paste&id=u2d580c80&originHeight=69&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9914d984-b870-4e36-9e87-0c16a7b2dba&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1663990549429-0493a162-da8f-402a-a9a1-eded5cc5a89a.png#clientId=u269cb811-6ca7-4&from=paste&id=u4d79d75e&originHeight=44&originWidth=670&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ubd7ae076-6c84-45fe-a672-c0f5ebab4fe&title=)<br />通过查看日志，http请求成功重定向到到了https，在Chrome浏览器中也标示为安全网站
