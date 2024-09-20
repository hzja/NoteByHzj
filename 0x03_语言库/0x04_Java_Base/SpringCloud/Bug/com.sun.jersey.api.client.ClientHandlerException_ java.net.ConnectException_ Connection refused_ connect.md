<a name="oW8lt"></a>
# 1、报错内容如下

```
"C:\Program Files\Java\jdk1.8.0_192\bin\java.exe" -XX:TieredStopAtLevel=1 -noverify -Dspring.output.ansi.enabled=always -Dcom.sun.management.jmxremote -Dspring.jmx.enabled=true -Dspring.liveBeansView.mbeanDomain -Dspring.application.admin.enabled=true -javaagent:D:\DevTools\ideaIU-2019.3.win\lib\idea_rt.jar=64942:D:\DevTools\ideaIU-2019.3.win\bin -Dfile.encoding=UTF-8 -classpath "C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\deploy.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\access-bridge-64.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\cldrdata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jaccess.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jfxrt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\nashorn.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunec.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunjce_provider.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunmscapi.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunpkcs11.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\zipfs.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\javaws.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfxswt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\management-agent.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\rt.jar;D:\Dome\SpringBootHD\SpringCloudEurekaServer\target\classes;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-starter-netflix-eureka-server\2.2.0.RELEASE\spring-cloud-starter-netflix-eureka-server-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-starter\2.2.0.RELEASE\spring-cloud-starter-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-context\2.2.0.RELEASE\spring-cloud-context-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\security\spring-security-crypto\5.2.1.RELEASE\spring-security-crypto-5.2.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-commons\2.2.0.RELEASE\spring-cloud-commons-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\security\spring-security-rsa\1.0.7.RELEASE\spring-security-rsa-1.0.7.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\bouncycastle\bcpkix-jdk15on\1.60\bcpkix-jdk15on-1.60.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\bouncycastle\bcprov-jdk15on\1.60\bcprov-jdk15on-1.60.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-netflix-eureka-server\2.2.0.RELEASE\spring-cloud-netflix-eureka-server-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-web\2.2.2.RELEASE\spring-boot-starter-web-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-json\2.2.2.RELEASE\spring-boot-starter-json-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\datatype\jackson-datatype-jdk8\2.10.1\jackson-datatype-jdk8-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\datatype\jackson-datatype-jsr310\2.10.1\jackson-datatype-jsr310-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\module\jackson-module-parameter-names\2.10.1\jackson-module-parameter-names-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-tomcat\2.2.2.RELEASE\spring-boot-starter-tomcat-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-core\9.0.29\tomcat-embed-core-9.0.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-el\9.0.29\tomcat-embed-el-9.0.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\tomcat\embed\tomcat-embed-websocket\9.0.29\tomcat-embed-websocket-9.0.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-validation\2.2.2.RELEASE\spring-boot-starter-validation-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\validation\jakarta.validation-api\2.0.1\jakarta.validation-api-2.0.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\hibernate\validator\hibernate-validator\6.0.18.Final\hibernate-validator-6.0.18.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\jboss\logging\jboss-logging\3.4.1.Final\jboss-logging-3.4.1.Final.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\classmate\1.5.1\classmate-1.5.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-web\5.2.2.RELEASE\spring-web-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-beans\5.2.2.RELEASE\spring-beans-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-webmvc\5.2.2.RELEASE\spring-webmvc-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-aop\5.2.2.RELEASE\spring-aop-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-expression\5.2.2.RELEASE\spring-expression-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-actuator\2.2.2.RELEASE\spring-boot-starter-actuator-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-actuator-autoconfigure\2.2.2.RELEASE\spring-boot-actuator-autoconfigure-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-actuator\2.2.2.RELEASE\spring-boot-actuator-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\micrometer\micrometer-core\1.3.1\micrometer-core-1.3.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\hdrhistogram\HdrHistogram\2.1.11\HdrHistogram-2.1.11.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\latencyutils\LatencyUtils\2.0.3\LatencyUtils-2.0.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-freemarker\2.2.2.RELEASE\spring-boot-starter-freemarker-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\freemarker\freemarker\2.3.29\freemarker-2.3.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-context-support\5.2.2.RELEASE\spring-context-support-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-netflix-hystrix\2.2.0.RELEASE\spring-cloud-netflix-hystrix-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-aop\2.2.2.RELEASE\spring-boot-starter-aop-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\aspectj\aspectjweaver\1.9.5\aspectjweaver-1.9.5.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-netflix-eureka-client\2.2.0.RELEASE\spring-cloud-netflix-eureka-client-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\eureka\eureka-client\1.9.13\eureka-client-1.9.13.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\codehaus\jettison\jettison\1.3.7\jettison-1.3.7.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\stax\stax-api\1.0.1\stax-api-1.0.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\netflix-commons\netflix-eventbus\0.3.0\netflix-eventbus-0.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\netflix-commons\netflix-infix\0.3.0\netflix-infix-0.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\commons-jxpath\commons-jxpath\1.3\commons-jxpath-1.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\joda-time\joda-time\2.10.5\joda-time-2.10.5.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\antlr\antlr-runtime\3.4\antlr-runtime-3.4.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\antlr\stringtemplate\3.2.1\stringtemplate-3.2.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\antlr\antlr\2.7.7\antlr-2.7.7.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\google\code\gson\gson\2.8.6\gson-2.8.6.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\commons\commons-math\2.2\commons-math-2.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\javax\ws\rs\jsr311-api\1.1.1\jsr311-api-1.1.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\servo\servo-core\0.12.21\servo-core-0.12.21.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\jersey\jersey-core\1.19.1\jersey-core-1.19.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\jersey\contribs\jersey-apache-client4\1.19.1\jersey-apache-client4-1.19.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\httpcomponents\httpclient\4.5.10\httpclient-4.5.10.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\httpcomponents\httpcore\4.4.12\httpcore-4.4.12.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\commons-codec\commons-codec\1.13\commons-codec-1.13.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\google\inject\guice\4.1.0\guice-4.1.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\aopalliance\aopalliance\1.0\aopalliance-1.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-annotations\2.10.1\jackson-annotations-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-core\2.10.1\jackson-core-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\core\jackson-databind\2.10.1\jackson-databind-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\jersey\jersey-servlet\1.19.1\jersey-servlet-1.19.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\jersey\jersey-server\1.19.1\jersey-server-1.19.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\jersey\jersey-client\1.19.1\jersey-client-1.19.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\eureka\eureka-core\1.9.13\eureka-core-1.9.13.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\woodstox\woodstox-core\5.2.1\woodstox-core-5.2.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\archaius\archaius-core\0.7.6\archaius-core-0.7.6.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\google\code\findbugs\jsr305\3.0.1\jsr305-3.0.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\google\guava\guava\16.0\guava-16.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\javax\inject\javax.inject\1\javax.inject-1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\dataformat\jackson-dataformat-xml\2.10.1\jackson-dataformat-xml-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\fasterxml\jackson\module\jackson-module-jaxb-annotations\2.10.1\jackson-module-jaxb-annotations-2.10.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\codehaus\woodstox\stax2-api\4.2\stax2-api-4.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\thoughtworks\xstream\xstream\1.4.11.1\xstream-1.4.11.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\xmlpull\xmlpull\1.1.3.1\xmlpull-1.1.3.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\xpp3\xpp3_min\1.1.4c\xpp3_min-1.1.4c.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-starter-netflix-archaius\2.2.0.RELEASE\spring-cloud-starter-netflix-archaius-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-netflix-ribbon\2.2.0.RELEASE\spring-cloud-netflix-ribbon-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-netflix-archaius\2.2.0.RELEASE\spring-cloud-netflix-archaius-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\commons-configuration\commons-configuration\1.8\commons-configuration-1.8.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\commons-lang\commons-lang\2.6\commons-lang-2.6.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-starter-netflix-ribbon\2.2.0.RELEASE\spring-cloud-starter-netflix-ribbon-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon\2.3.0\ribbon-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon-transport\2.3.0\ribbon-transport-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\reactivex\rxnetty-contexts\0.4.9\rxnetty-contexts-0.4.9.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\reactivex\rxnetty-servo\0.4.9\rxnetty-servo-0.4.9.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\hystrix\hystrix-core\1.5.18\hystrix-core-1.5.18.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\reactivex\rxnetty\0.4.9\rxnetty-0.4.9.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon-core\2.3.0\ribbon-core-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon-httpclient\2.3.0\ribbon-httpclient-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\commons-collections\commons-collections\3.2.2\commons-collections-3.2.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\netflix-commons\netflix-commons-util\0.3.0\netflix-commons-util-0.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon-loadbalancer\2.3.0\ribbon-loadbalancer-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\netflix-commons\netflix-statistics\0.1.1\netflix-statistics-0.1.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\reactivex\rxjava\1.3.8\rxjava-1.3.8.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-starter-loadbalancer\2.2.0.RELEASE\spring-cloud-starter-loadbalancer-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\cloud\spring-cloud-loadbalancer\2.2.0.RELEASE\spring-cloud-loadbalancer-2.2.0.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\projectreactor\reactor-core\3.3.1.RELEASE\reactor-core-3.3.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\reactivestreams\reactive-streams\1.0.3\reactive-streams-1.0.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\io\projectreactor\addons\reactor-extra\3.3.1.RELEASE\reactor-extra-3.3.1.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-cache\2.2.2.RELEASE\spring-boot-starter-cache-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\stoyanr\evictor\1.0.0\evictor-1.0.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\netflix\ribbon\ribbon-eureka\2.3.0\ribbon-eureka-2.3.0.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\slf4j\slf4j-api\1.7.29\slf4j-api-1.7.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\glassfish\jaxb\jaxb-runtime\2.3.2\jaxb-runtime-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\glassfish\jaxb\txw2\2.3.2\txw2-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\istack\istack-commons-runtime\3.0.8\istack-commons-runtime-3.0.8.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\jvnet\staxex\stax-ex\1.8.1\stax-ex-1.8.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\com\sun\xml\fastinfoset\FastInfoset\1.2.16\FastInfoset-1.2.16.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\activation\jakarta.activation-api\1.2.1\jakarta.activation-api-1.2.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter\2.2.2.RELEASE\spring-boot-starter-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot\2.2.2.RELEASE\spring-boot-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-context\5.2.2.RELEASE\spring-context-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-autoconfigure\2.2.2.RELEASE\spring-boot-autoconfigure-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\boot\spring-boot-starter-logging\2.2.2.RELEASE\spring-boot-starter-logging-2.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\ch\qos\logback\logback-classic\1.2.3\logback-classic-1.2.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\ch\qos\logback\logback-core\1.2.3\logback-core-1.2.3.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\logging\log4j\log4j-to-slf4j\2.12.1\log4j-to-slf4j-2.12.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\apache\logging\log4j\log4j-api\2.12.1\log4j-api-2.12.1.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\slf4j\jul-to-slf4j\1.7.29\jul-to-slf4j-1.7.29.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\annotation\jakarta.annotation-api\1.3.5\jakarta.annotation-api-1.3.5.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\yaml\snakeyaml\1.25\snakeyaml-1.25.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\jakarta\xml\bind\jakarta.xml.bind-api\2.3.2\jakarta.xml.bind-api-2.3.2.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-core\5.2.2.RELEASE\spring-core-5.2.2.RELEASE.jar;D:\ProTools\apache-maven-3.6.1-bin\apache-maven-3.6.1\Repository\org\springframework\spring-jcl\5.2.2.RELEASE\spring-jcl-5.2.2.RELEASE.jar" com.fcant.springcloudeurekaserver.SpringCloudEurekaServerApplication
2019-12-08 23:00:38.072  INFO 17992 --- [           main] trationDelegate$BeanPostProcessorChecker : Bean 'org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration' of type [org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration] is not eligible for getting processed by all BeanPostProcessors (for example: not eligible for auto-proxying)

  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::        (v2.2.2.RELEASE)

2019-12-08 23:00:38.492  INFO 17992 --- [           main] c.f.s.SpringCloudEurekaServerApplication : No active profile set, falling back to default profiles: default
2019-12-08 23:00:39.412  WARN 17992 --- [           main] o.s.boot.actuate.endpoint.EndpointId     : Endpoint ID 'service-registry' contains invalid characters, please migrate to a valid format.
2019-12-08 23:00:39.584  INFO 17992 --- [           main] o.s.cloud.context.scope.GenericScope     : BeanFactory id=2dd3e7c2-ae01-32d4-b6c2-2560a8750c46
2019-12-08 23:00:39.729  INFO 17992 --- [           main] trationDelegate$BeanPostProcessorChecker : Bean 'org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration' of type [org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration] is not eligible for getting processed by all BeanPostProcessors (for example: not eligible for auto-proxying)
2019-12-08 23:00:39.908  INFO 17992 --- [           main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat initialized with port(s): 8000 (http)
2019-12-08 23:00:39.916  INFO 17992 --- [           main] o.apache.catalina.core.StandardService   : Starting service [Tomcat]
2019-12-08 23:00:39.917  INFO 17992 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet engine: [Apache Tomcat/9.0.29]
2019-12-08 23:00:40.013  INFO 17992 --- [           main] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2019-12-08 23:00:40.013  INFO 17992 --- [           main] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 1508 ms
2019-12-08 23:00:40.085  WARN 17992 --- [           main] c.n.c.sources.URLConfigurationSource     : No URLs will be polled as dynamic configuration sources.
2019-12-08 23:00:40.085  INFO 17992 --- [           main] c.n.c.sources.URLConfigurationSource     : To enable URLs as dynamic configuration sources, define System property archaius.configurationSource.additionalUrls or make config.properties available on classpath.
2019-12-08 23:00:40.094  INFO 17992 --- [           main] c.netflix.config.DynamicPropertyFactory  : DynamicPropertyFactory is initialized with configuration sources: com.netflix.config.ConcurrentCompositeConfiguration@285f38f6
2019-12-08 23:00:40.831  INFO 17992 --- [           main] c.s.j.s.i.a.WebApplicationImpl           : Initiating Jersey application, version 'Jersey: 1.19.1 03/11/2016 02:08 PM'
2019-12-08 23:00:40.924  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON encoding codec LegacyJacksonJson
2019-12-08 23:00:40.925  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON decoding codec LegacyJacksonJson
2019-12-08 23:00:40.999  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML encoding codec XStreamXml
2019-12-08 23:00:40.999  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML decoding codec XStreamXml
2019-12-08 23:00:41.225  WARN 17992 --- [           main] o.s.c.n.a.ArchaiusAutoConfiguration      : No spring.application.name found, defaulting to 'application'
2019-12-08 23:00:41.225  WARN 17992 --- [           main] c.n.c.sources.URLConfigurationSource     : No URLs will be polled as dynamic configuration sources.
2019-12-08 23:00:41.225  INFO 17992 --- [           main] c.n.c.sources.URLConfigurationSource     : To enable URLs as dynamic configuration sources, define System property archaius.configurationSource.additionalUrls or make config.properties available on classpath.
2019-12-08 23:00:41.329  INFO 17992 --- [           main] o.s.s.concurrent.ThreadPoolTaskExecutor  : Initializing ExecutorService 'applicationTaskExecutor'
2019-12-08 23:00:42.163  WARN 17992 --- [           main] ockingLoadBalancerClientRibbonWarnLogger : You already have RibbonLoadBalancerClient on your classpath. It will be used by default. As Spring Cloud Ribbon is in maintenance mode. We recommend switching to BlockingLoadBalancerClient instead. In order to use it, set the value of `spring.cloud.loadbalancer.ribbon.enabled` to `false` or remove spring-cloud-starter-netflix-ribbon from your project.
2019-12-08 23:00:42.190  WARN 17992 --- [           main] iguration$LoadBalancerCaffeineWarnLogger : Spring Cloud LoadBalancer is currently working with default default cache. You can switch to using Caffeine cache, by adding it to the classpath.
2019-12-08 23:00:42.204  INFO 17992 --- [           main] o.s.c.n.eureka.InstanceInfoFactory       : Setting initial instance status as: STARTING
2019-12-08 23:00:42.226  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Initializing Eureka in region us-east-1
2019-12-08 23:00:42.535  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON encoding codec LegacyJacksonJson
2019-12-08 23:00:42.535  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON decoding codec LegacyJacksonJson
2019-12-08 23:00:42.535  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML encoding codec XStreamXml
2019-12-08 23:00:42.536  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML decoding codec XStreamXml
2019-12-08 23:00:42.604  INFO 17992 --- [           main] c.n.d.s.r.aws.ConfigClusterResolver      : Resolving eureka endpoints via configuration
2019-12-08 23:00:42.794  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Disable delta property : false
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Single vip registry refresh property : null
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Force full registry fetch : false
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Application is null : false
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Registered Applications size is zero : true
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Application version is -1: true
2019-12-08 23:00:42.795  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Getting all instance registry info from the eureka server
2019-12-08 23:00:44.855 ERROR 17992 --- [           main] c.n.d.s.t.d.RedirectingEurekaHttpClient  : Request execution error. endpoint=DefaultEndpoint{ serviceUrl='http://localhost:8761/eureka/}

com.sun.jersey.api.client.ClientHandlerException: java.net.ConnectException: Connection refused: connect
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:187) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.filter.GZIPContentEncodingFilter.handle(GZIPContentEncodingFilter.java:123) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.EurekaIdentityHeaderFilter.handle(EurekaIdentityHeaderFilter.java:27) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.sun.jersey.api.client.Client.handle(Client.java:652) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.handle(WebResource.java:682) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.access$200(WebResource.java:74) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource$Builder.get(WebResource.java:509) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.getApplicationsInternal(AbstractJerseyEurekaHttpClient.java:194) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.getApplications(AbstractJerseyEurekaHttpClient.java:165) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.MetricsCollectingEurekaHttpClient.execute(MetricsCollectingEurekaHttpClient.java:73) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.executeOnNewServer(RedirectingEurekaHttpClient.java:118) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.execute(RedirectingEurekaHttpClient.java:79) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:120) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.getAndStoreFullRegistry(DiscoveryClient.java:1069) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.fetchRegistry(DiscoveryClient.java:983) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:430) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:276) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:272) [eureka-client-1.9.13.jar:1.9.13]
	at org.springframework.cloud.netflix.eureka.CloudEurekaClient.<init>(CloudEurekaClient.java:67) [spring-cloud-netflix-eureka-client-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.cloud.netflix.eureka.EurekaClientAutoConfiguration$RefreshableEurekaClientConfiguration.eurekaClient(EurekaClientAutoConfiguration.java:324) [spring-cloud-netflix-eureka-client-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:154) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiate(ConstructorResolver.java:651) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:636) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$1(AbstractBeanFactory.java:359) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.cloud.context.scope.GenericScope$BeanLifecycleWrapper.getBean(GenericScope.java:389) ~[spring-cloud-context-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.cloud.context.scope.GenericScope.get(GenericScope.java:186) ~[spring-cloud-context-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:356) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.aop.target.SimpleBeanTargetSource.getTarget(SimpleBeanTargetSource.java:35) ~[spring-aop-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:192) ~[spring-aop-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at com.sun.proxy.$Proxy100.getApplications(Unknown Source) ~[na:na]
	at org.springframework.cloud.netflix.eureka.server.EurekaServerAutoConfiguration.peerAwareInstanceRegistry(EurekaServerAutoConfiguration.java:154) ~[spring-cloud-netflix-eureka-server-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:154) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiate(ConstructorResolver.java:651) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:636) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:276) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1287) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1207) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.resolveAutowiredArgument(ConstructorResolver.java:885) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.createArgumentArray(ConstructorResolver.java:789) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:539) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:276) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1287) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1207) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor$AutowiredFieldElement.inject(AutowiredAnnotationBeanPostProcessor.java:640) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.InjectionMetadata.inject(InjectionMetadata.java:116) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor.postProcessProperties(AutowiredAnnotationBeanPostProcessor.java:399) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1422) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:594) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:879) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:878) ~[spring-context-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:550) ~[spring-context-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.boot.web.servlet.context.ServletWebServerApplicationContext.refresh(ServletWebServerApplicationContext.java:141) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.refresh(SpringApplication.java:747) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.refreshContext(SpringApplication.java:397) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:315) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1226) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1215) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at com.fcant.springcloudeurekaserver.SpringCloudEurekaServerApplication.main(SpringCloudEurekaServerApplication.java:19) ~[classes/:na]
Caused by: java.net.ConnectException: Connection refused: connect
	at java.net.DualStackPlainSocketImpl.waitForConnect(Native Method) ~[na:1.8.0_192]
	at java.net.DualStackPlainSocketImpl.socketConnect(DualStackPlainSocketImpl.java:85) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188) ~[na:1.8.0_192]
	at java.net.PlainSocketImpl.connect(PlainSocketImpl.java:172) ~[na:1.8.0_192]
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392) ~[na:1.8.0_192]
	at java.net.Socket.connect(Socket.java:589) ~[na:1.8.0_192]
	at org.apache.http.conn.scheme.PlainSocketFactory.connectSocket(PlainSocketFactory.java:121) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.DefaultClientConnectionOperator.openConnection(DefaultClientConnectionOperator.java:180) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPoolEntry.open(AbstractPoolEntry.java:144) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPooledConnAdapter.open(AbstractPooledConnAdapter.java:134) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.tryConnect(DefaultRequestDirector.java:605) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.execute(DefaultRequestDirector.java:440) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.AbstractHttpClient.doExecute(AbstractHttpClient.java:835) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:118) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:56) ~[httpclient-4.5.10.jar:4.5.10]
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:173) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	... 100 common frames omitted

2019-12-08 23:00:44.856  WARN 17992 --- [           main] c.n.d.s.t.d.RetryableEurekaHttpClient    : Request execution failed with message: java.net.ConnectException: Connection refused: connect
2019-12-08 23:00:44.857 ERROR 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000 - was unable to refresh its cache! status = Cannot execute request on any known server

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.getAndStoreFullRegistry(DiscoveryClient.java:1069) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.fetchRegistry(DiscoveryClient.java:983) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:430) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:276) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.<init>(DiscoveryClient.java:272) [eureka-client-1.9.13.jar:1.9.13]
	at org.springframework.cloud.netflix.eureka.CloudEurekaClient.<init>(CloudEurekaClient.java:67) [spring-cloud-netflix-eureka-client-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.cloud.netflix.eureka.EurekaClientAutoConfiguration$RefreshableEurekaClientConfiguration.eurekaClient(EurekaClientAutoConfiguration.java:324) [spring-cloud-netflix-eureka-client-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:154) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiate(ConstructorResolver.java:651) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:636) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$1(AbstractBeanFactory.java:359) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.cloud.context.scope.GenericScope$BeanLifecycleWrapper.getBean(GenericScope.java:389) ~[spring-cloud-context-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.cloud.context.scope.GenericScope.get(GenericScope.java:186) ~[spring-cloud-context-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:356) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.aop.target.SimpleBeanTargetSource.getTarget(SimpleBeanTargetSource.java:35) ~[spring-aop-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:192) ~[spring-aop-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at com.sun.proxy.$Proxy100.getApplications(Unknown Source) ~[na:na]
	at org.springframework.cloud.netflix.eureka.server.EurekaServerAutoConfiguration.peerAwareInstanceRegistry(EurekaServerAutoConfiguration.java:154) ~[spring-cloud-netflix-eureka-server-2.2.0.RELEASE.jar:2.2.0.RELEASE]
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method) ~[na:1.8.0_192]
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62) ~[na:1.8.0_192]
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43) ~[na:1.8.0_192]
	at java.lang.reflect.Method.invoke(Method.java:498) ~[na:1.8.0_192]
	at org.springframework.beans.factory.support.SimpleInstantiationStrategy.instantiate(SimpleInstantiationStrategy.java:154) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiate(ConstructorResolver.java:651) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:636) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:276) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1287) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1207) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.resolveAutowiredArgument(ConstructorResolver.java:885) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.createArgumentArray(ConstructorResolver.java:789) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.ConstructorResolver.instantiateUsingFactoryMethod(ConstructorResolver.java:539) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.instantiateUsingFactoryMethod(AbstractAutowireCapableBeanFactory.java:1338) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBeanInstance(AbstractAutowireCapableBeanFactory.java:1177) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:557) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.config.DependencyDescriptor.resolveCandidate(DependencyDescriptor.java:276) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.doResolveDependency(DefaultListableBeanFactory.java:1287) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.resolveDependency(DefaultListableBeanFactory.java:1207) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor$AutowiredFieldElement.inject(AutowiredAnnotationBeanPostProcessor.java:640) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.InjectionMetadata.inject(InjectionMetadata.java:116) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.annotation.AutowiredAnnotationBeanPostProcessor.postProcessProperties(AutowiredAnnotationBeanPostProcessor.java:399) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.populateBean(AbstractAutowireCapableBeanFactory.java:1422) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:594) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:517) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.lambda$doGetBean$0(AbstractBeanFactory.java:323) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:222) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:321) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:202) [spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:879) ~[spring-beans-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:878) ~[spring-context-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:550) ~[spring-context-5.2.2.RELEASE.jar:5.2.2.RELEASE]
	at org.springframework.boot.web.servlet.context.ServletWebServerApplicationContext.refresh(ServletWebServerApplicationContext.java:141) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.refresh(SpringApplication.java:747) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.refreshContext(SpringApplication.java:397) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:315) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1226) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at org.springframework.boot.SpringApplication.run(SpringApplication.java:1215) ~[spring-boot-2.2.2.RELEASE.jar:2.2.2.RELEASE]
	at com.fcant.springcloudeurekaserver.SpringCloudEurekaServerApplication.main(SpringCloudEurekaServerApplication.java:19) ~[classes/:na]

2019-12-08 23:00:44.858  WARN 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Using default backup registry implementation which does not do anything.
2019-12-08 23:00:44.858  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Starting heartbeat executor: renew interval is: 30
2019-12-08 23:00:44.860  INFO 17992 --- [           main] c.n.discovery.InstanceInfoReplicator     : InstanceInfoReplicator onDemand update allowed rate per min is 4
2019-12-08 23:00:44.862  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Discovery Client initialized at timestamp 1575817244861 with initial instances count: 0
2019-12-08 23:00:44.884  INFO 17992 --- [           main] c.n.eureka.DefaultEurekaServerContext    : Initializing ...
2019-12-08 23:00:44.885  INFO 17992 --- [           main] c.n.eureka.cluster.PeerEurekaNodes       : Adding new peer nodes [http://localhost:8761/eureka/]
2019-12-08 23:00:44.888  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON encoding codec LegacyJacksonJson
2019-12-08 23:00:44.888  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON decoding codec LegacyJacksonJson
2019-12-08 23:00:44.888  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML encoding codec XStreamXml
2019-12-08 23:00:44.888  INFO 17992 --- [           main] c.n.d.provider.DiscoveryJerseyProvider   : Using XML decoding codec XStreamXml
2019-12-08 23:00:44.942  INFO 17992 --- [           main] c.n.eureka.cluster.PeerEurekaNodes       : Replica node URL:  http://localhost:8761/eureka/
2019-12-08 23:00:44.947  INFO 17992 --- [           main] c.n.e.registry.AbstractInstanceRegistry  : Finished initializing remote region registries. All known remote regions: []
2019-12-08 23:00:44.948  INFO 17992 --- [           main] c.n.eureka.DefaultEurekaServerContext    : Initialized
2019-12-08 23:00:44.957  INFO 17992 --- [           main] o.s.b.a.e.web.EndpointLinksResolver      : Exposing 2 endpoint(s) beneath base path '/actuator'
2019-12-08 23:00:45.009  INFO 17992 --- [           main] o.s.c.n.e.s.EurekaServiceRegistry        : Registering application UNKNOWN with eureka with status UP
2019-12-08 23:00:45.010  INFO 17992 --- [           main] com.netflix.discovery.DiscoveryClient    : Saw local status change event StatusChangeEvent [timestamp=1575817245010, current=UP, previous=STARTING]
2019-12-08 23:00:45.012  INFO 17992 --- [nfoReplicator-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000: registering service...
2019-12-08 23:00:45.014  INFO 17992 --- [      Thread-11] o.s.c.n.e.server.EurekaServerBootstrap   : Setting the eureka configuration..
2019-12-08 23:00:45.015  INFO 17992 --- [      Thread-11] o.s.c.n.e.server.EurekaServerBootstrap   : Eureka data center value eureka.datacenter is not set, defaulting to default
2019-12-08 23:00:45.016  INFO 17992 --- [      Thread-11] o.s.c.n.e.server.EurekaServerBootstrap   : Eureka environment value eureka.environment is not set, defaulting to test
2019-12-08 23:00:45.026  INFO 17992 --- [      Thread-11] o.s.c.n.e.server.EurekaServerBootstrap   : isAws returned false
2019-12-08 23:00:45.026  INFO 17992 --- [      Thread-11] o.s.c.n.e.server.EurekaServerBootstrap   : Initialized server context
2019-12-08 23:00:45.048  INFO 17992 --- [           main] o.s.b.w.embedded.tomcat.TomcatWebServer  : Tomcat started on port(s): 8000 (http) with context path ''
2019-12-08 23:00:45.049  INFO 17992 --- [           main] .s.c.n.e.s.EurekaAutoServiceRegistration : Updating port to 8000
2019-12-08 23:00:45.333  INFO 17992 --- [           main] c.f.s.SpringCloudEurekaServerApplication : Started SpringCloudEurekaServerApplication in 8.589 seconds (JVM running for 9.398)
2019-12-08 23:00:47.024 ERROR 17992 --- [nfoReplicator-0] c.n.d.s.t.d.RedirectingEurekaHttpClient  : Request execution error. endpoint=DefaultEndpoint{ serviceUrl='http://localhost:8761/eureka/}

com.sun.jersey.api.client.ClientHandlerException: java.net.ConnectException: Connection refused: connect
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:187) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.filter.GZIPContentEncodingFilter.handle(GZIPContentEncodingFilter.java:123) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.EurekaIdentityHeaderFilter.handle(EurekaIdentityHeaderFilter.java:27) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.sun.jersey.api.client.Client.handle(Client.java:652) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.handle(WebResource.java:682) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.access$200(WebResource.java:74) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource$Builder.post(WebResource.java:570) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.register(AbstractJerseyEurekaHttpClient.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.MetricsCollectingEurekaHttpClient.execute(MetricsCollectingEurekaHttpClient.java:73) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.executeOnNewServer(RedirectingEurekaHttpClient.java:118) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.execute(RedirectingEurekaHttpClient.java:79) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:120) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator$1.run(InstanceInfoReplicator.java:101) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]
Caused by: java.net.ConnectException: Connection refused: connect
	at java.net.DualStackPlainSocketImpl.waitForConnect(Native Method) ~[na:1.8.0_192]
	at java.net.DualStackPlainSocketImpl.socketConnect(DualStackPlainSocketImpl.java:85) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188) ~[na:1.8.0_192]
	at java.net.PlainSocketImpl.connect(PlainSocketImpl.java:172) ~[na:1.8.0_192]
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392) ~[na:1.8.0_192]
	at java.net.Socket.connect(Socket.java:589) ~[na:1.8.0_192]
	at org.apache.http.conn.scheme.PlainSocketFactory.connectSocket(PlainSocketFactory.java:121) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.DefaultClientConnectionOperator.openConnection(DefaultClientConnectionOperator.java:180) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPoolEntry.open(AbstractPoolEntry.java:144) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPooledConnAdapter.open(AbstractPooledConnAdapter.java:134) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.tryConnect(DefaultRequestDirector.java:605) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.execute(DefaultRequestDirector.java:440) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.AbstractHttpClient.doExecute(AbstractHttpClient.java:835) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:118) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:56) ~[httpclient-4.5.10.jar:4.5.10]
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:173) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	... 30 common frames omitted

2019-12-08 23:00:47.024  WARN 17992 --- [nfoReplicator-0] c.n.d.s.t.d.RetryableEurekaHttpClient    : Request execution failed with message: java.net.ConnectException: Connection refused: connect
2019-12-08 23:00:47.025  WARN 17992 --- [nfoReplicator-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000 - registration failed Cannot execute request on any known server

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator$1.run(InstanceInfoReplicator.java:101) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

2019-12-08 23:00:47.026  WARN 17992 --- [nfoReplicator-0] c.n.discovery.InstanceInfoReplicator     : There was a problem with the instance info replicator

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator$1.run(InstanceInfoReplicator.java:101) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

2019-12-08 23:01:14.862  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Disable delta property : false
2019-12-08 23:01:14.862  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Single vip registry refresh property : null
2019-12-08 23:01:14.862  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Force full registry fetch : false
2019-12-08 23:01:14.863  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Application is null : false
2019-12-08 23:01:14.864  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Registered Applications size is zero : true
2019-12-08 23:01:14.864  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Application version is -1: true
2019-12-08 23:01:14.864  INFO 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : Getting all instance registry info from the eureka server
2019-12-08 23:01:16.874 ERROR 17992 --- [tbeatExecutor-0] c.n.d.s.t.d.RedirectingEurekaHttpClient  : Request execution error. endpoint=DefaultEndpoint{ serviceUrl='http://localhost:8761/eureka/}

com.sun.jersey.api.client.ClientHandlerException: java.net.ConnectException: Connection refused: connect
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:187) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.filter.GZIPContentEncodingFilter.handle(GZIPContentEncodingFilter.java:123) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.EurekaIdentityHeaderFilter.handle(EurekaIdentityHeaderFilter.java:27) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.sun.jersey.api.client.Client.handle(Client.java:652) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.handle(WebResource.java:682) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.access$200(WebResource.java:74) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource$Builder.put(WebResource.java:529) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.sendHeartBeat(AbstractJerseyEurekaHttpClient.java:102) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$3.execute(EurekaHttpClientDecorator.java:92) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.MetricsCollectingEurekaHttpClient.execute(MetricsCollectingEurekaHttpClient.java:73) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$3.execute(EurekaHttpClientDecorator.java:92) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.executeOnNewServer(RedirectingEurekaHttpClient.java:118) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.execute(RedirectingEurekaHttpClient.java:79) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$3.execute(EurekaHttpClientDecorator.java:92) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:120) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$3.execute(EurekaHttpClientDecorator.java:92) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.renew(DiscoveryClient.java:864) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient$HeartbeatThread.run(DiscoveryClient.java:1423) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]
Caused by: java.net.ConnectException: Connection refused: connect
	at java.net.DualStackPlainSocketImpl.waitForConnect(Native Method) ~[na:1.8.0_192]
	at java.net.DualStackPlainSocketImpl.socketConnect(DualStackPlainSocketImpl.java:85) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188) ~[na:1.8.0_192]
	at java.net.PlainSocketImpl.connect(PlainSocketImpl.java:172) ~[na:1.8.0_192]
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392) ~[na:1.8.0_192]
	at java.net.Socket.connect(Socket.java:589) ~[na:1.8.0_192]
	at org.apache.http.conn.scheme.PlainSocketFactory.connectSocket(PlainSocketFactory.java:121) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.DefaultClientConnectionOperator.openConnection(DefaultClientConnectionOperator.java:180) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPoolEntry.open(AbstractPoolEntry.java:144) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPooledConnAdapter.open(AbstractPooledConnAdapter.java:134) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.tryConnect(DefaultRequestDirector.java:605) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.execute(DefaultRequestDirector.java:440) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.AbstractHttpClient.doExecute(AbstractHttpClient.java:835) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:118) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:56) ~[httpclient-4.5.10.jar:4.5.10]
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:173) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	... 27 common frames omitted

2019-12-08 23:01:16.874  WARN 17992 --- [tbeatExecutor-0] c.n.d.s.t.d.RetryableEurekaHttpClient    : Request execution failed with message: java.net.ConnectException: Connection refused: connect
2019-12-08 23:01:16.874 ERROR 17992 --- [tbeatExecutor-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000 - was unable to send heartbeat!

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$3.execute(EurekaHttpClientDecorator.java:92) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.sendHeartBeat(EurekaHttpClientDecorator.java:89) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.renew(DiscoveryClient.java:864) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient$HeartbeatThread.run(DiscoveryClient.java:1423) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

2019-12-08 23:01:16.881 ERROR 17992 --- [freshExecutor-0] c.n.d.s.t.d.RedirectingEurekaHttpClient  : Request execution error. endpoint=DefaultEndpoint{ serviceUrl='http://localhost:8761/eureka/}

com.sun.jersey.api.client.ClientHandlerException: java.net.ConnectException: Connection refused: connect
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:187) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.filter.GZIPContentEncodingFilter.handle(GZIPContentEncodingFilter.java:123) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.EurekaIdentityHeaderFilter.handle(EurekaIdentityHeaderFilter.java:27) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.sun.jersey.api.client.Client.handle(Client.java:652) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.handle(WebResource.java:682) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.access$200(WebResource.java:74) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource$Builder.get(WebResource.java:509) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.getApplicationsInternal(AbstractJerseyEurekaHttpClient.java:194) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.getApplications(AbstractJerseyEurekaHttpClient.java:165) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.MetricsCollectingEurekaHttpClient.execute(MetricsCollectingEurekaHttpClient.java:73) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.executeOnNewServer(RedirectingEurekaHttpClient.java:118) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.execute(RedirectingEurekaHttpClient.java:79) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:120) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.getAndStoreFullRegistry(DiscoveryClient.java:1069) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.fetchRegistry(DiscoveryClient.java:983) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.refreshRegistry(DiscoveryClient.java:1497) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient$CacheRefreshThread.run(DiscoveryClient.java:1464) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]
Caused by: java.net.ConnectException: Connection refused: connect
	at java.net.DualStackPlainSocketImpl.waitForConnect(Native Method) ~[na:1.8.0_192]
	at java.net.DualStackPlainSocketImpl.socketConnect(DualStackPlainSocketImpl.java:85) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188) ~[na:1.8.0_192]
	at java.net.PlainSocketImpl.connect(PlainSocketImpl.java:172) ~[na:1.8.0_192]
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392) ~[na:1.8.0_192]
	at java.net.Socket.connect(Socket.java:589) ~[na:1.8.0_192]
	at org.apache.http.conn.scheme.PlainSocketFactory.connectSocket(PlainSocketFactory.java:121) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.DefaultClientConnectionOperator.openConnection(DefaultClientConnectionOperator.java:180) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPoolEntry.open(AbstractPoolEntry.java:144) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPooledConnAdapter.open(AbstractPooledConnAdapter.java:134) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.tryConnect(DefaultRequestDirector.java:605) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.execute(DefaultRequestDirector.java:440) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.AbstractHttpClient.doExecute(AbstractHttpClient.java:835) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:118) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:56) ~[httpclient-4.5.10.jar:4.5.10]
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:173) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	... 30 common frames omitted

2019-12-08 23:01:16.881  WARN 17992 --- [freshExecutor-0] c.n.d.s.t.d.RetryableEurekaHttpClient    : Request execution failed with message: java.net.ConnectException: Connection refused: connect
2019-12-08 23:01:16.881 ERROR 17992 --- [freshExecutor-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000 - was unable to refresh its cache! status = Cannot execute request on any known server

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$6.execute(EurekaHttpClientDecorator.java:137) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.getApplications(EurekaHttpClientDecorator.java:134) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.getAndStoreFullRegistry(DiscoveryClient.java:1069) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.fetchRegistry(DiscoveryClient.java:983) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.refreshRegistry(DiscoveryClient.java:1497) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient$CacheRefreshThread.run(DiscoveryClient.java:1464) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

2019-12-08 23:01:17.026  INFO 17992 --- [nfoReplicator-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000: registering service...
2019-12-08 23:01:19.032 ERROR 17992 --- [nfoReplicator-0] c.n.d.s.t.d.RedirectingEurekaHttpClient  : Request execution error. endpoint=DefaultEndpoint{ serviceUrl='http://localhost:8761/eureka/}

com.sun.jersey.api.client.ClientHandlerException: java.net.ConnectException: Connection refused: connect
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:187) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.filter.GZIPContentEncodingFilter.handle(GZIPContentEncodingFilter.java:123) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.EurekaIdentityHeaderFilter.handle(EurekaIdentityHeaderFilter.java:27) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.sun.jersey.api.client.Client.handle(Client.java:652) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.handle(WebResource.java:682) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource.access$200(WebResource.java:74) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.sun.jersey.api.client.WebResource$Builder.post(WebResource.java:570) ~[jersey-client-1.19.1.jar:1.19.1]
	at com.netflix.discovery.shared.transport.jersey.AbstractJerseyEurekaHttpClient.register(AbstractJerseyEurekaHttpClient.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.MetricsCollectingEurekaHttpClient.execute(MetricsCollectingEurekaHttpClient.java:73) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.executeOnNewServer(RedirectingEurekaHttpClient.java:118) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RedirectingEurekaHttpClient.execute(RedirectingEurekaHttpClient.java:79) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:120) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) [eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]
Caused by: java.net.ConnectException: Connection refused: connect
	at java.net.DualStackPlainSocketImpl.waitForConnect(Native Method) ~[na:1.8.0_192]
	at java.net.DualStackPlainSocketImpl.socketConnect(DualStackPlainSocketImpl.java:85) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206) ~[na:1.8.0_192]
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188) ~[na:1.8.0_192]
	at java.net.PlainSocketImpl.connect(PlainSocketImpl.java:172) ~[na:1.8.0_192]
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392) ~[na:1.8.0_192]
	at java.net.Socket.connect(Socket.java:589) ~[na:1.8.0_192]
	at org.apache.http.conn.scheme.PlainSocketFactory.connectSocket(PlainSocketFactory.java:121) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.DefaultClientConnectionOperator.openConnection(DefaultClientConnectionOperator.java:180) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPoolEntry.open(AbstractPoolEntry.java:144) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.conn.AbstractPooledConnAdapter.open(AbstractPooledConnAdapter.java:134) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.tryConnect(DefaultRequestDirector.java:605) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.DefaultRequestDirector.execute(DefaultRequestDirector.java:440) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.AbstractHttpClient.doExecute(AbstractHttpClient.java:835) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:118) ~[httpclient-4.5.10.jar:4.5.10]
	at org.apache.http.impl.client.CloseableHttpClient.execute(CloseableHttpClient.java:56) ~[httpclient-4.5.10.jar:4.5.10]
	at com.sun.jersey.client.apache4.ApacheHttpClient4Handler.handle(ApacheHttpClient4Handler.java:173) ~[jersey-apache-client4-1.19.1.jar:1.19.1]
	... 29 common frames omitted

2019-12-08 23:01:19.033  WARN 17992 --- [nfoReplicator-0] c.n.d.s.t.d.RetryableEurekaHttpClient    : Request execution failed with message: java.net.ConnectException: Connection refused: connect
2019-12-08 23:01:19.033  WARN 17992 --- [nfoReplicator-0] com.netflix.discovery.DiscoveryClient    : DiscoveryClient_UNKNOWN/LAPTOP-CMGC7APE:8000 - registration failed Cannot execute request on any known server

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) [eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]

2019-12-08 23:01:19.034  WARN 17992 --- [nfoReplicator-0] c.n.discovery.InstanceInfoReplicator     : There was a problem with the instance info replicator

com.netflix.discovery.shared.transport.TransportException: Cannot execute request on any known server
	at com.netflix.discovery.shared.transport.decorator.RetryableEurekaHttpClient.execute(RetryableEurekaHttpClient.java:112) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator$1.execute(EurekaHttpClientDecorator.java:59) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.SessionedEurekaHttpClient.execute(SessionedEurekaHttpClient.java:77) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.shared.transport.decorator.EurekaHttpClientDecorator.register(EurekaHttpClientDecorator.java:56) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.DiscoveryClient.register(DiscoveryClient.java:847) ~[eureka-client-1.9.13.jar:1.9.13]
	at com.netflix.discovery.InstanceInfoReplicator.run(InstanceInfoReplicator.java:121) ~[eureka-client-1.9.13.jar:1.9.13]
	at java.util.concurrent.Executors$RunnableAdapter.call(Executors.java:511) [na:1.8.0_192]
	at java.util.concurrent.FutureTask.run(FutureTask.java:266) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$201(ScheduledThreadPoolExecutor.java:180) [na:1.8.0_192]
	at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(ScheduledThreadPoolExecutor.java:293) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1149) [na:1.8.0_192]
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:624) [na:1.8.0_192]
	at java.lang.Thread.run(Thread.java:748) [na:1.8.0_192]


```
<a name="NoR50"></a>
# 2、问题分析

- 此应用为注册中心，不向注册中心注册自己
- 注册中心职责是维护服务实例，不检索服务
<a name="EqSAj"></a>
# 3、解决办法
在配置文件配置不要把自己注册到注册中心，以及停止扫描服务

```yaml
eureka:
  client:
    # 此应用为注册中心，false：不向注册中心注册自己
    register-with-eureka: false
    # 注册中心职责是维护服务实例，false：不检索服务
    fetch-registry: false
```

