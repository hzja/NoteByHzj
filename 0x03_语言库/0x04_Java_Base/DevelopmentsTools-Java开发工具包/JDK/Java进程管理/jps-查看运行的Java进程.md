Java jps
<a name="zTcZC"></a>
## jps-查看运行的Java进程
<a name="XbKId"></a>
### `jps -help` ：jps的帮助命令
```bash
❯ jps -help
usage: jps [-help]
       jps [-q] [-mlvV] [<hostid>]

Definitions:
    <hostid>:      <hostname>[:<port>]
```
jps命令用法如下

| jps -q | 仅输出VM标识符，不包括classname,jar,name,arguments in main method |
| --- | --- |
| jps -m | 输出main method的参数 |
| jps -I | 输出完全的包名，应用主类名，jar的完全路径名 |
| jps -v | 输出jvm参数 |
| jps -V | 输出通过flag文件传递到JVM中的参数(hotspotrc)文件或-XX:Flags=所指定的文件 |
| jps -Joption | 传递参数到vm，例如:-J-Xms512m |

执行 `jps` 可以获取本机 Java 程序的 pid
```bash
❯ jps -h
illegal argument: -h
usage: jps [-help]
       jps [-q] [-mlvV] [<hostid>]

Definitions:
    <hostid>:      <hostname>[:<port>]
fcsca@FCANT /                                                                                                     [17:32]
❯ jps
26528 Launcher
29492 Application
17400
8920 Jps
14412
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594719370282-899cf85a-3006-48d9-80cd-325648657f18.png#averageHue=%23323232&height=299&id=z90Qr&originHeight=896&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1052116&status=done&style=none&title=&width=1107.6666666666667)
<a name="LfayG"></a>
###  `jps -mlvV` ：获取到这个进程的 pid、jar 包的名字以及 JVM 参数等。
```bash
fcsca@FCANT D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin                                       [14:55]
❯ jps -mlvV
10944 org.jetbrains.jps.cmdline.Launcher D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/javac2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/maven-resolver-transport-http-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jna-platform.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-repository-metadata-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/lz4-java-1.7.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-artifact-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/aether-dependency-resolver.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-api-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jdom.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/annotations.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/nanoxml-2.2.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/guava-28.2-jre.jar;D:/LinkS -Xmx700m -Djava.awt.headless=true -Djava.endorsed.dirs="" -Djdt.compiler.useSingleThread=true -Dpreload.project.path=D:/LinkSpace/Work/WorkSpace/Code/IDEA/finchain-web -Dpreload.config.path=C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/options -Dcompile.parallel=false -Drebuild.on.dependency.change=true -Dio.netty.initialSeedUniquifier=4304941387663479476 -Dfile.encoding=GBK -Duser.language=zh -Duser.country=CN -Didea.paths.selector=IntelliJIdea2020.1 -Didea.home.path=D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win -Didea.config.path=C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1 -Didea.plugins.path=C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1\plugins -Djps.log.dir=C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/log/build-log -Djps.fallback.jdk.home=D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/jbr -Djps.fallback.jdk.version=11.0.7 -Dio.netty.noUnsafe=true -Djava.io.tmpdir=C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/compile-server/fin
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594883310917-b3933c82-4b67-4e02-b697-fd8a08718e70.png#averageHue=%234f4f4f&height=583&id=HleW5&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2514953&status=done&style=none&title=&width=1107.6666666666667)
:::danger
注意：jps命令只能获取到当前用户所属的Java进程，非当前用户所属的Java进程不会展示<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681034570321-cf0225f8-f206-49a8-9206-794b59c7c0cf.png#averageHue=%233b3b3b&clientId=u323a86d4-ec4f-4&from=paste&height=266&id=ud40e59dc&originHeight=665&originWidth=3323&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=665595&status=done&style=none&taskId=u541d59e9-7e71-49dd-9187-b654264b519&title=&width=1329.2)
:::

