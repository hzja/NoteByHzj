JavaSpringBoot<br />SpringBoot程序在打完Jar包以后，放置服务器进行部署，如果有人将安装包拉取下来，进行反编译，是可以还原代码的，那么有没有比较好好的措施不要让代码裸奔呢，就来探讨一下jar包的混淆。
<a name="B7Uv5"></a>
## 一、什么是编译
简单就是把代码跑一下，然后 .java文件 就被编译成了 .class 文件。![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202752384-ce0c8258-726c-4abb-a827-338063d8450e.png#averageHue=%233f3834&clientId=uc3e949d8-cec8-4&from=paste&id=u172f4291&originHeight=409&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae7871ec-9fc8-4a7e-abe2-876a113b0d0&title=)
<a name="wYBMp"></a>
## 二、什么是反编译
就是针对编译生成的 jar/war 包 里面的 .class 文件 逆向还原回来，可以看到代码内容。<br />比较常用的反编译工具 JD-GUI，直接把编译好的jar丢进去，大部分都能反编译看到源码：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202752992-d3c840a5-25c5-467e-b70e-6c87e80889a6.png#averageHue=%23f6f2f2&clientId=uc3e949d8-cec8-4&from=paste&id=ubb22db0c&originHeight=527&originWidth=818&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u714517e1-4926-4de2-95c4-bad1feef7e4&title=)<br />那如果不想给别人反编译看自己写的代码呢？
<a name="Kz0E6"></a>
## 三、混淆
这里的代码混淆 ，是其中一种手段。但反编译看到的不是真正的代码。<br />**先看一张效果示例图 ：**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202753191-580ab066-e11e-48da-a5d8-9364b24bc0c2.png#averageHue=%23f6f2f2&clientId=uc3e949d8-cec8-4&from=paste&id=u3fbcc3f7&originHeight=527&originWidth=818&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u020162f0-a930-4de5-a54d-b05993f393f&title=)
<a name="Nf3JX"></a>
## 四、开搞
先看一下混淆一个项目代码要做什么。<br />![2023-02-24-18-46-18.276095500.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1677240463087-ae177c41-d632-44b1-b694-f137b91adc1d.png#averageHue=%233c4144&clientId=u82f030bf-cf8e-4&from=ui&id=ufa054204&originHeight=606&originWidth=352&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=641657&status=done&style=none&taskId=uaea148cd-aa62-4aea-8425-7f23ff7a18e&title=)<br />一共就两步 <br />第一步， 在项目路径下，新增一份文件 proguard.cfg ：<br />proguard.cfg<br />注意点：<br />![2023-02-24-18-46-18.332827200.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1677240576464-4f594d44-4555-4cec-b115-035a8755be19.png#averageHue=%23756449&clientId=u82f030bf-cf8e-4&from=ui&id=u09b3dc5f&originHeight=609&originWidth=1054&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1929461&status=done&style=none&taskId=ub356e0b4-788e-40e8-85ec-e4eeb7889b7&title=)<br />其余的看注释，可以配置哪些类不参与混淆，哪些枚举保留，哪些方法名不混淆等等。<br />第二步，在pom文件上 加入proguard 混淆插件<br />build标签里面改动加入一下配置<br />注意点：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202753616-bd97b898-79f7-49e4-9305-b85f7e00eba7.png#averageHue=%234b413f&clientId=uc3e949d8-cec8-4&from=paste&id=u6364fc60&originHeight=89&originWidth=665&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ua7bc7015-f5ce-4c47-bdf1-21cc7bbc4f9&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202753690-ded5b4df-6878-42e3-9e15-81f54a034d4a.png#averageHue=%23322d2b&clientId=uc3e949d8-cec8-4&from=paste&id=u398e54e3&originHeight=323&originWidth=851&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5e2bbe7-c34a-4563-8a3a-75c203a11d0&title=)<br />然后可以看到： <br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202753839-426630f3-5fef-45d6-8639-0d2f5553545d.png#averageHue=%233c4144&clientId=uc3e949d8-cec8-4&from=paste&id=u69007b26&originHeight=792&originWidth=624&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u588c73dd-5d71-41a1-bbee-4511a58a3c8&title=)<br />然后点击package，正常执行编译打包流程就可以 ：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202753917-a4822bc8-c584-4572-9d45-442ed926a5a8.png#averageHue=%23a18a62&clientId=uc3e949d8-cec8-4&from=paste&id=u5af734fb&originHeight=445&originWidth=992&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55e4404b-b482-4fb5-98bf-5cfc252ba48&title=)<br />然后可以看到jar的生成： <br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202754013-298706b1-d8bb-413d-8405-305d2924da8b.png#averageHue=%23524c41&clientId=uc3e949d8-cec8-4&from=paste&id=uddff492d&originHeight=266&originWidth=493&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub60d39e4-4ebc-496c-bb26-4a0db713298&title=)<br />看看效果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1677202754103-645247cf-0858-43f5-b0fe-fa2259e2a261.png#averageHue=%23f6f4f3&clientId=uc3e949d8-cec8-4&from=paste&id=u8ea42bc1&originHeight=515&originWidth=818&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uafcd8321-d2b8-476b-9d7b-aaaf57482bb&title=)
```
#指定Java的版本
-target 1.8
#proguard会对代码进行优化压缩，他会删除从未使用的类或者类成员变量等
-dontshrink
#是否关闭字节码级别的优化，如果不开启则设置如下配置
-dontoptimize
#混淆时不生成大小写混合的类名，默认是可以大小写混合
-dontusemixedcaseclassnames
# 对于类成员的命名的混淆采取唯一策略
-useuniqueclassmembernames
#混淆时不生成大小写混合的类名，默认是可以大小写混合
-dontusemixedcaseclassnames
#混淆类名之后，对使用Class.forName('className')之类的地方进行相应替代
-adaptclassstrings
 
#对异常、注解信息予以保留
-keepattributes Exceptions,InnerClasses,Signature,Deprecated,SourceFile,LineNumberTable,*Annotation*,EnclosingMethod
# 此选项将保存接口中的所有原始名称（不混淆）-->
-keepnames interface ** { *; }
# 此选项将保存所有软件包中的所有原始接口文件（不进行混淆）
#-keep interface * extends * { *; }
#保留参数名，因为控制器，或者Mybatis等接口的参数如果混淆会导致无法接受参数，xml文件找不到参数
-keepparameternames
# 保留枚举成员及方法
-keepclassmembers enum * { *; }
# 不混淆所有类,保存原始定义的注释-
-keepclassmembers class * {
                        @org.springframework.context.annotation.Bean *;
                        @org.springframework.beans.factory.annotation.Autowired *;
                        @org.springframework.beans.factory.annotation.Value *;
                        @org.springframework.stereotype.Service *;
                        @org.springframework.stereotype.Component *;
                        }
 
#忽略warn消息
-ignorewarnings
#忽略note消息
-dontnote
#打印配置信息
-printconfiguration
-keep public class com.example.myproguarddemo.MyproguarddemoApplication {
        public static void main(java.lang.String[]);
    }
```
```xml
<build>
  <plugins>
    <plugin>
      <groupId>com.github.wvengen</groupId>
      <artifactId>proguard-maven-plugin</artifactId>
      <version>2.6.0</version>
      <executions>
        <!-- 以下配置说明执行mvn的package命令时候，会执行proguard-->
        <execution>
          <phase>package</phase>
          <goals>
            <goal>proguard</goal>
          </goals>
        </execution>
      </executions>
      <configuration>
        <!-- 就是输入Jar的名称，我们要知道，代码混淆其实是将一个原始的jar，生成一个混淆后的jar，那么就会有输入输出。 -->
        <injar>${project.build.finalName}.jar</injar>
        <!-- 输出jar名称，输入输出jar同名的时候就是覆盖，也是比较常用的配置。 -->
        <outjar>${project.build.finalName}.jar</outjar>
        <!-- 是否混淆 默认是true -->
        <obfuscate>true</obfuscate>
        <!-- 配置一个文件，通常叫做proguard.cfg,该文件主要是配置options选项，也就是说使用proguard.cfg那么options下的所有内容都可以移到proguard.cfg中 -->
        <proguardInclude>${project.basedir}/proguard.cfg</proguardInclude>
        <!-- 额外的jar包，通常是项目编译所需要的jar -->
        <libs>
          <lib>${java.home}/lib/rt.jar</lib>
          <lib>${java.home}/lib/jce.jar</lib>
          <lib>${java.home}/lib/jsse.jar</lib>
        </libs>
        <!-- 对输入jar进行过滤比如，如下配置就是对META-INFO文件不处理。 -->
        <inLibsFilter>!META-INF/**,!META-INF/versions/9/**.class</inLibsFilter>
        <!-- 这是输出路径配置，但是要注意这个路径必须要包括injar标签填写的jar -->
        <outputDirectory>${project.basedir}/target</outputDirectory>
        <!--这里特别重要，此处主要是配置混淆的一些细节选项，比如哪些类不需要混淆，哪些需要混淆-->
        <options>
          <!-- 可以在此处写option标签配置，不过我上面使用了proguardInclude，故而我更喜欢在proguard.cfg中配置 -->
        </options>
      </configuration>
    </plugin>
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <executions>
        <execution>
          <goals>
            <goal>repackage</goal>
          </goals>
          <configuration>
            <mainClass>com.example.myproguarddemo.MyproguarddemoApplication</mainClass>
          </configuration>
        </execution>
      </executions>
    </plugin>
  </plugins>
</build>
```

