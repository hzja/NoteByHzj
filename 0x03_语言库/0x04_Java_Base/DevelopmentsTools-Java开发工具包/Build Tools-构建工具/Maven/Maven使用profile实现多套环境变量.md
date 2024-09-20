Maven
<a name="LulG7"></a>
## 多环境变量配置
在SpringCloud多模块微服务项目中，在父pom配置profile实现多套环境，同时也要重写build标签，定位要过滤替换环境变量值的配置文件
```xml
<profiles>
  <profile>
    <id>dev</id>
    <properties>
      <!--                环境标识，需要与配置文件的名称相对应-->
      <profiles.active>dev</profiles.active>
    </properties>
    <activation>
      <!--                默认环境-->
      <activeByDefault>true</activeByDefault>
    </activation>
  </profile>
  <profile>
    <id>uat</id>
    <properties>
      <!--                环境标识，需要与配置文件的名称相对应-->
      <profiles.active>uat</profiles.active>
    </properties>
  </profile>
  <profile>
    <id>prod</id>
    <properties>
      <!--                环境标识，需要与配置文件的名称相对应-->
      <profiles.active>prod</profiles.active>
    </properties>
  </profile>
</profiles>
```
配置好该项后可以在IDEA中的Maven中看到对应的环境变量值，选中激活当前的环境变量即可<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636360121145-fe22bd02-3494-4bb5-a921-e28039287148.png#clientId=u27390f82-0cbc-4&from=paste&height=313&id=u2114ff4d&originHeight=939&originWidth=1027&originalType=binary&ratio=1&size=54290&status=done&style=shadow&taskId=uc91cd901-0e73-47d4-905b-bc998d14626&width=342.3333333333333)
<a name="Wx32t"></a>
## 在配置文件引用Maven环境变量值
```yaml
spring:
  application:
    name: eureka-server
  profiles:
    active: @profiles.active@
logging:
  level:
    root: info
```
<a name="DvQBH"></a>
## 配置Maven编译时需要替换环境变量值的过滤的文件类型
```xml
<build>
  <finalName>${project.name}-${project.version}</finalName>
  <resources>
    <resource>
      <directory>${basedir}/src/main/resources</directory>
      <filtering>true</filtering>
      <includes>
        <include>**/*.xml</include>
        <include>**/*.yml</include>
        <include>**/*.yaml</include>
        <include>**/*.properties</include>
      </includes>
    </resource>
  </resources>
</build>
```
<a name="F0vpE"></a>
## 注意事项
在自定义build标签后，Maven编译时会根据自定义的过滤文件类型，如果不配置忽略过滤的文件，则可能会发生意想不到的后果。
```xml
<build>
  <finalName>${project.name}-${project.version}</finalName>
  <resources>
    <resource>
      <directory>src/main/resources</directory>
      <filtering>true</filtering>
      <includes>
        <include>**/*.yml</include>
      </includes>
    </resource>
    <resource>
      <directory>src/main/resources</directory>
      <filtering>false</filtering>
      <includes>
        <include>**/*.xml</include>
        <include>**/*.ttf</include>
        <include>**/*.jpg</include>
        <include>**/*.png</include>
        <include>**/*.ftl</include>
      </includes>
    </resource>
  </resources>
  <plugins>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
    </plugin>
  </plugins>
</build>
```
如：
<a name="U4rZf"></a>
### 不配置过滤字体文件，编译后的字体文件不能正常使用
双击编译后的字体<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1636363271986-e6efac5d-d260-4a16-8cbb-c650e841aa67.png#clientId=u99918b89-64ae-4&from=paste&height=495&id=u46601721&originHeight=1484&originWidth=2256&originalType=binary&ratio=1&size=311675&status=done&style=shadow&taskId=u78592eab-4536-4f7f-ab00-a0b5399d6b3&width=752)<br />在使用itext的项目中具体报错为：
```
com.itextpdf.text.DocumentException: Table 'name' does not exist in image/font/simhei.ttf
        at com.itextpdf.text.pdf.TrueTypeFont.getBaseFont(TrueTypeFont.java:517)
        at com.itextpdf.text.pdf.TrueTypeFont.process(TrueTypeFont.java:675)
        at com.itextpdf.text.pdf.TrueTypeFontUnicode.process(TrueTypeFontUnicode.java:122)
        at com.itextpdf.text.pdf.TrueTypeFontUnicode.<init>(TrueTypeFontUnicode.java:99)
        at com.itextpdf.text.pdf.BaseFont.createFont(BaseFont.java:706)
        at com.itextpdf.text.pdf.BaseFont.createFont(BaseFont.java:626)
        at com.itextpdf.text.pdf.BaseFont.createFont(BaseFont.java:461)
        at com.cntaiping.ft.bankinsurance.image.util.LexPDF.insentFont(LexPDF.java:291)
        at com.cntaiping.ft.bankinsurance.image.util.LexPDF.getPDFBytes(LexPDF.java:52)
        at com.cntaiping.ft.bankinsurance.image.service.impl.ImageBuildServiceImpl.build(ImageBuildServiceImpl.java:75)
        at com.cntaiping.ft.bankinsurance.image.service.impl.ImageBuildServiceImpl.build(ImageBuildServiceImpl.java:91)
        at com.cntaiping.ft.bankinsurance.image.service.impl.ImageServiceImpl.create(ImageServiceImpl.java:88)
        at com.cntaiping.ft.bankinsurance.image.service.impl.ImageServiceImpl$$FastClassBySpringCGLIB$$7c164064.invoke(<generated>)
        at org.springwork.cglib.proxy.MethodProxy.invoke(MethodProxy.java:218)
        at org.springwork.aop.work.CglibAopProxy$DynamicAdvisedInterceptor.intercept(CglibAopProxy.java:687)
        at com.cntaiping.ft.bankinsurance.image.service.impl.ImageServiceImpl$$EnhancerBySpringCGLIB$$260adbfe.create(<generated>)
        at com.cntaiping.ft.bankinsurance.image.listener.ImageCreateStreamListener.onMessage(ImageCreateStreamListener.java:45)
        at com.cntaiping.ft.bankinsurance.image.listener.ImageCreateStreamListener.onMessage(ImageCreateStreamListener.java:20)
        at org.springwork.data.redis.stream.StreamPollTask.doLoop(StreamPollTask.java:142)
        at org.springwork.data.redis.stream.StreamPollTask.run(StreamPollTask.java:123)
        at java.lang.Thread.run(Thread.java:748)
```

