JavaSpringBoot<br />都知道常用的 SpringBoot 项目最终在线上运行的时候都是通过启动 `java -jar xxx.jar` 命令来运行的。<br />有没有想过一个问题，那就是当执行 `java -jar` 命令后，到底底层做了什么就启动了 SpringBoot 应用呢？<br />或者说一个 SpringBoot 的应用到底是如何运行起来的呢？
<a name="xYclO"></a>
## 认识 jar
在介绍 `java -jar` 运行原理之前先看一下 jar 包里面都包含了哪些内容，准备一个 SpringBoot 项目，通过在 [https://start.spring.io/](https://start.spring.io/) 上可以快速创建一个 SpringBoot 项目，下载一个对应版本和报名的 zip 包。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374033774-9b76f002-c38a-4312-bea6-8e073d525500.jpeg#averageHue=%23f7f7f7&clientId=u5ee071af-77ef-4&from=paste&id=ucfcd6bc3&originHeight=578&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub86f7363-a892-4f11-a76f-9e6fd72b4a9&title=)<br />下载后的项目在 pom 依赖里面可以看到有如下依赖，这个插件是构建可执行 jar 的前提，所以如果想要打包成一个 jar 那必须在 pom 有增加这个插件，从 start.spring.io 上创建的项目默认是会带上这个插件的。
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
接下来执行 `mvn package`，执行完过后在项目的 target 目录里面可以看到有如下两个 jar 包，分别把这两个 jar 解压一下看看里面的内容，.original 后缀的 jar 需要把后面的 .original 去掉就可以解压了。jar 文件的解压跟平常的 zip 解压是一样的，jar 文件采用的是 zip 压缩格式存储，所以任何可以解压 zip 文件的软件都可以解压 jar 文件。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374033824-fe0eab47-6b25-4c1f-a080-b1b3008c09b9.jpeg#averageHue=%23333831&clientId=u5ee071af-77ef-4&from=paste&id=u4ea434df&originHeight=559&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4cb844ff-224d-45c0-a343-b9cdfd707fe&title=)<br />解压过后，对比两种解压文件，可以发现，两个文件夹中的内容还是有很大区别的，如下所示，左侧是 demo-jar-0.0.1-SNAPSHOT.jar 右侧是对应的 original jar。<br />其中有一些相同的文件夹和文件，比如 META-INF，application.properties 等，而且可以明显的看到左侧的压缩包中有项目需要依赖的所有库文件，存放于 lib 文件夹中。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374033827-175290ce-ebcf-4754-a0ea-34fc6ef82c78.jpeg#averageHue=%23e2e3e0&clientId=u5ee071af-77ef-4&from=paste&id=u141cdac1&originHeight=322&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc0f6e646-d729-4e6e-8ac5-e305d04f30e&title=)<br />所以可以大胆的猜测，左侧的压缩包就是 spring-boot-maven-plugin 这个插件把依赖的库以及相应的文件调整了一下目录结构而生成的，事实其实也是如此。
<a name="Bmh8E"></a>
## `java -jar` 原理
首先要知道的是这个 `java -jar` 不是什么新的东西，而是 java 本身就自带的命令，而且 `java -jar` 命令在执行的时候，命令本身对于这个 jar 是不是 SpringBoot 项目是不感知的，只要是符合 Java 标准规范的 jar 都可以通过这个命令启动。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374033775-f6a36225-cbda-4825-ba4b-3f3fd0ac0e2f.jpeg#averageHue=%233d3d3d&clientId=u5ee071af-77ef-4&from=paste&id=ue7772503&originHeight=474&originWidth=944&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub3167093-3b48-4745-bb21-25e0859538a&title=)<br />而在 Java 官方文档显示，当 `-jar` 参数存在的时候，jar 文件资源里面必须包含用 Main-Class 指定的一个启动类，而且同样根据规范这个资源文件 MANIFEST.MF 必须放在 /META-INF/ 目录下。对比上面解压后的文件，可以看到在左侧的资源文件 MANIFEST.MF 文件中有如图所示的一行。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374033777-69349fe6-d6c6-4d3f-906d-7f30868583e1.jpeg#averageHue=%23363732&clientId=u5ee071af-77ef-4&from=paste&id=ufd6a4647&originHeight=298&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uabdb9a82-e005-4768-9387-9574a1d23f7&title=)<br />可以看到这里的 Main-Class 属性配置的是 org.springframework.boot.loader.JarLauncher，而如果小伙伴更仔细一点的话，会发现项目的启动类也在这个文件里面，是通过 Start-Class 字段来表示的，Start-Class 这个属性不是 Java 官方的属性。<br />由此先大胆的猜测一下，当在执行 `java -jar` 的时候，由于 jar 里面存在 MANIFEST.MF 文件，并且其中包含了 Main-Class 属性且配置了 org.springframework.boot.loader.JarLauncher 类，通过调用 JarLauncher 类结合 Start-Class 属性引导出项目的启动类进行启动。接下来就通过源码来验证一下这个猜想。<br />因为 JarLauncher 类是在 spring-boot-loader 模块，所以在 pom 文件中增加如下依赖，就可以下载源码进行跟踪了。
```xml
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-loader</artifactId>
  <scope>provided</scope>
</dependency>
```
通过源码可以看到 JarLauncher 类的代码如下
```java
package org.springframework.boot.loader;

import org.springframework.boot.loader.archive.Archive;
import org.springframework.boot.loader.archive.Archive.EntryFilter;

public class JarLauncher extends ExecutableArchiveLauncher {

    static final EntryFilter NESTED_ARCHIVE_ENTRY_FILTER = (entry) -> {
        if (entry.isDirectory()) {
            return entry.getName().equals("BOOT-INF/classes/");
        }
        return entry.getName().startsWith("BOOT-INF/lib/");
    };

    public JarLauncher() {
    }

    protected JarLauncher(Archive archive) {
        super(archive);
    }

    @Override
    protected boolean isPostProcessingClassPathArchives() {
        return false;
    }

    @Override
    protected boolean isNestedArchive(Archive.Entry entry) {
        return NESTED_ARCHIVE_ENTRY_FILTER.matches(entry);
    }

    @Override
    protected String getArchiveEntryPathPrefix() {
        return "BOOT-INF/";
    }

    public static void main(String[] args) throws Exception {
        new JarLauncher().launch(args);
    }

}
```
其中有两个点可以关注一下，第一个是这个类有一个 main 方法，这也是为什么 `java -jar` 命令可以进行引导的原因，毕竟 java 程序都是通过 main 方法进行运行的。其次是这里面有两个路径 BOOT-INF/classes/ 和 BOOT-INF/lib/ 这两个路径正好是源码路径和第三方依赖路径。<br />而 JarLauncher 类里面的 `main()` 方法主要是运行 Launcher 里面的 `launch()` 方法，这几个类的关系图如下所示<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374034294-24fb6271-ad00-4958-bafe-1bbd720ec41e.jpeg#averageHue=%2341392e&clientId=u5ee071af-77ef-4&from=paste&id=u4fb253e8&originHeight=632&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7ccca4b0-cef1-4e32-b483-6b57d9dc589&title=)<br />跟着代码可以看到最终调用的是这个 `run()` 方法<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374034509-27eb1422-82ff-44bc-ad89-c7faa3654ac4.jpeg#averageHue=%233a3a39&clientId=u5ee071af-77ef-4&from=paste&id=ue20d9edb&originHeight=443&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u3a080a6e-c50a-4dfb-aed1-cd66dfbb7e6&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374034430-3105cb9a-1d63-4d1d-845c-1bfdfde5d640.jpeg#averageHue=%233d3c3b&clientId=u5ee071af-77ef-4&from=paste&id=u3b6b526a&originHeight=346&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ufb42b82e-51c5-4ac3-a5a0-248179ee070&title=)<br />而这里的参数 mainClass 和 launchClass 都是通过通过下面的逻辑获取的，都是通过资源文件里面的 Start-Class 来进行获取的，这里正是项目的启动类，由此可以看到上面的猜想是正确的。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1670374034443-3aeb3fe1-b8ca-4199-b601-524391668581.jpeg#averageHue=%23494847&clientId=u5ee071af-77ef-4&from=paste&id=u69b29dba&originHeight=292&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9ec1d540-1700-4cdb-b06b-1ad882cf727&title=)
<a name="uUqe3"></a>
## 扩展
上面的类图当中还可以看到除了有 JarLauncher 以外还有一个 WarLauncher 类，确实 SpringBoot 项目也是可以配置成 war 进行部署的。只需要将打包插件里面的 jar 更换成 war 即可。大家可以自行尝试重新打包解压进行分析，这里 war 包部署方式只研究学习就好了，SpringBoot 应用还是尽量都使用 Jar 的方式进行部署。
<a name="FdNuT"></a>
## 总结
通过上面的内容知道了当在执行 `java -jar` 的时候，根据 java 官方规范会引导 jar 包里面 MANIFEST.MF 文件中的 Main-Class 属性对应的启动类，该启动类中必须包含 `main()` 方法。<br />而对于 SpringBoot 项目构建的 jar 包，除了 Main-Class 属性外还会有一个 Start-Class 属性绑定的是项目的启动类，当在执行 `java -jar` 的时候优先引导的是 `org.springframework.boot.loader.JarLauncher#main` 方法，该方法内部会通过引导 Start-Class 属性来启动应用代码。
