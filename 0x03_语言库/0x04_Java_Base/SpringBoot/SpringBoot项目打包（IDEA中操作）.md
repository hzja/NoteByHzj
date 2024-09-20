Java SpringBoot 
<a name="dPGOJ"></a>
## 1、在依赖中导入SpringBoot-Maven插件
不使用spring-boot-manven-plugin插件会导致依赖不全面，jar会中的MANIFEST.MF文件会缺少Start-Class和Main-Class，以至于在通过 `java -jar` 命令运行时，会报找不到主类异常
```xml
<build>
  <plugins>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
    </plugin>
  </plugins>
</build>
```
<a name="3ePzL"></a>
## 2、使用Maven的package命令打包
<a name="376JE"></a>
### 执行reimport将依赖导入
<a name="2V9to"></a>
### 跳过打包测试（否则会报错）
<a name="72WBL"></a>
### 执行打包命令`mvn package`
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596610430760-6673293c-2590-468e-8ca1-18fbf0774565.png#averageHue=%23f5f5f4&height=565&id=BZuGG&originHeight=1694&originWidth=1123&originalType=binary&ratio=1&rotation=0&showTitle=false&size=332795&status=done&style=shadow&title=&width=374.3333333333333)
<a name="Y6hmi"></a>
## 3、检查生成的jar包是否缺少Main-class和Start-Class
如果不同于下面的格式，缺少属性，则可能会启动失败
```
Manifest-Version: 1.0
Implementation-Title: center
Implementation-Version: 0.0.1-SNAPSHOT
Start-Class: com.ater.center.CenterApplication
Spring-Boot-Classes: BOOT-INF/classes/
Spring-Boot-Lib: BOOT-INF/lib/
Build-Jdk-Spec: 1.8
Spring-Boot-Version: 2.1.16.RELEASE
Created-By: Maven Archiver 3.4.0
Main-Class: org.springframework.boot.loader.JarLauncher
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596610781676-7c2e9076-2005-4460-a35f-dbae73e297ff.png#averageHue=%23fafaf9&height=495&id=YUAFV&originHeight=1486&originWidth=2852&originalType=binary&ratio=1&rotation=0&showTitle=false&size=180309&status=done&style=shadow&title=&width=950.6666666666666)
