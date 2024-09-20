Java jinfo
<a name="6Z6qI"></a>
# jinfo-查看运行中进程信息
jinfo（Configuration Info for Java）的作用是实时查看和调整虚拟机各项参数<br />使用`jps -v` 可以查看虚拟机启动时显示指定的参数列表，但是如果想知道未被显示指定的参数的系统默认值，除了去找资料外，就只能使用jinfo的-flag选项进行查询了。
<a name="KBrFc"></a>
## 参数说明
使用 jps 命令的 -v 参数可以查看虚拟机启动时显示指定的参数列表，获取 pid
```bash
jinfo [option] pid
```
使用 -flag <name> 选项打印虚拟机标记参数的值，name 表示虚拟机标记参数的名称
```bash
jinfo -flag PrintGC 21768
```
使用 -flag [+|-]<name> 选项可以开启或关闭虚拟机表示参数，+ 表示开启，- 表示关闭
```bash
jinfo -flag +PrintGC 21768
jinfo -flag -PrintGC 21768
```
使用 -flag <name> = <value> 可以设置虚拟机标记参数的值，但并不是每个参数都可以被动态修改
```bash
jinfo -flag HeapDumpPath=C:\error.hprof 21768
```
剩下的常用选项如表

| 选项 | 作用 |
| --- | --- |
| -flags | 打印虚拟机参数，如 -XX:NewSize |
| no option | 不带任何选项时，会同时打印虚拟机参数和系统参数 |
| -sysprops | 打印系统参数 |
| -h &#124; -help | 打印帮助信息 |

```bash
fcsca@FCANT /                                                                                                      [17:39]
❯ jinfo -h
Usage:
    jinfo [option] <pid>
        (to connect to running process)连接到运行的进程ID
    jinfo [option] <executable <core>
        (to connect to a core file)连接到核心文件
    jinfo [option] [server_id@]<remote server IP or hostname>
        (to connect to remote debug server)连接到远程服务

where <option> is one of:
    -flag <name>         to print the value of the named VM flag
    -flag [+|-]<name>    to enable or disable the named VM flag
    -flag <name>=<value> to set the named VM flag to the given value
    -flags               to print VM flags
    -sysprops            to print Java system properties
    <no option>          to print both of the above
    -h | -help           to print this help message
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594719889690-0b5bc36d-fa42-4b0c-869e-dd0472c25d33.png#averageHue=%233f3f3f&height=582&id=Nw6NA&originHeight=1747&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2142012&status=done&style=none&title=&width=1107.6666666666667)
<a name="ec6FS"></a>
## 操作
<a name="yAi7U"></a>
### `jinfo PID`：打印对应进程ID的信息
```bash
fcsca@FCANT /                                                                                                     [17:35]
❯ jinfo 26528
Attaching to process ID 26528, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
Java System Properties:

java.vendor = Oracle Corporation
preload.project.path = D:/LinkSpace/Work/WorkSpace/Code/IDEA/finchain-web
sun.java.launcher = SUN_STANDARD
idea.config.path = C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1
sun.management.compiler = HotSpot 64-Bit Tiered Compilers
sun.nio.ch.bugLevel =
idea.paths.selector = IntelliJIdea2020.1
kotlin.daemon.client.alive.path = "C:\Users\fcsca\AppData\Local\Temp\kotlin-idea-5395178358329969888-is-running"
os.name = Windows 10
sun.boot.class.path = C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\rt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\sunrsasign.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_192\jre\classes
sun.desktop = windows
idea.plugins.path = C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1\plugins
java.vm.specification.vendor = Oracle Corporation
java.runtime.version = 1.8.0_192-b12
io.netty.serviceThreadPrefix = Netty
user.name = fcsca
kotlin.incremental.compilation = true
idea.home.path = D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win
user.language = zh
jdt.compiler.useSingleThread = true
sun.boot.library.path = C:\Program Files\Java\jdk1.8.0_192\jre\bin
java.version = 1.8.0_192
user.timezone = GMT+08:00
kotlin.daemon.enabled =
sun.arch.data.model = 64
io.netty.initialSeedUniquifier = 5655166389141628370
java.endorsed.dirs = ""
sun.cpu.isalist = amd64
sun.jnu.encoding = GBK
file.encoding.pkg = sun.io
file.separator = \
java.specification.name = Java Platform API Specification
java.class.version = 52.0
user.country = CN
java.home = C:\Program Files\Java\jdk1.8.0_192\jre
java.vm.info = mixed mode
os.version = 10.0
path.separator = ;
java.vm.version = 25.192-b12
user.variant =
jps.fallback.jdk.version = 11.0.7
java.awt.printerjob = sun.awt.windows.WPrinterJob
sun.io.unicode.encoding = UnicodeLittle
awt.toolkit = sun.awt.windows.WToolkit
jps.log.dir = C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/log/build-log
io.netty.processId = 7921
user.script =
user.home = C:\Users\fcsca
java.specification.vendor = Oracle Corporation
java.library.path = C:\Program Files\Java\jdk1.8.0_192\bin;C:\WINDOWS\Sun\Java\bin;C:\WINDOWS\system32;C:\WINDOWS;D:\Program Files\Xshell\;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;c:\Program Files\Intel\WiFi\bin\;c:\Program Files\Common Files\Intel\WirelessCommon\;C:\WINDOWS\System32\OpenSSH\;D:\Program Files\Development\Git\Git\cmd;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\bin;C:\Program Files\Java\jdk1.8.0_192\bin;D:\LinkSpace\Download\DevelopPackage\MySQL\mysql-8.0.16-winx64\mysql-8.0.16-winx64\bin;D:\LinkSpace\Download\DevelopPackage\Node\node-v14.2.0-win-x64\node-v14.2.0-win-x64;C:\Program Files\dotnet\;D:\LinkSpace\Download\DevelopPackage\Gradle\gradle-6.5-all\gradle-6.5\bin;D:\LinkSpace\Download\DevelopPackage\Redis\Redis-x64-3.2.100;C:\Users\fcsca\AppData\Local\Microsoft\WindowsApps;C:\Program Files\Bandizip\;.
java.vendor.url = http://java.oracle.com/
jps.fallback.jdk.home = D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/jbr
java.vm.vendor = Oracle Corporation
java.runtime.name = Java(TM) SE Runtime Environment
sun.java.command = org.jetbrains.jps.cmdline.Launcher D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/javac2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/maven-resolver-transport-http-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jna-platform.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-repository-metadata-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/lz4-java-1.7.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-artifact-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/aether-dependency-resolver.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-api-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jdom.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/annotations.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/nanoxml-2.2.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/guava-28.2-jre.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-provider-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-model-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/httpcore-4.4.13.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/netty-buffer-4.1.47.Final.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-util-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/slf4j-api-1.7.25.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-impl-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/gson-2.8.6.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-model-builder-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/netty-codec-4.1.47.Final.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/plexus-interpolation-1.25.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/forms-1.1-preview.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/util.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jps-model.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/jps-builders-6.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-builder-support-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/idea_rt.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/netty-resolver-4.1.47.Final.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/log4j.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/jps-builders.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/maven-resolver-connector-basic-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/commons-logging-1.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/platform-api.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/oro-2.0.8.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/commons-lang3-3.9.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jna.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/resources_en.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/plexus-utils-3.2.0.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-spi-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/httpclient-4.5.12.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/asm-all-7.0.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/qdox-2.0-M10.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/maven-resolver-transport-file-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/netty-common-4.1.47.Final.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/plexus-component-annotations-1.7.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/protobuf-java-3.5.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/trove4j.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/netty-transport-4.1.47.Final.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/JavaEE/lib/jasper-v2-rt.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/gson-2.8.6.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/jarutils.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/guava-28.2-jre.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/common-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/gradle/lib/gradle-api-6.1.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/gson-2.8.6.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/jarutils.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/guava-28.2-jre.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/common-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/gradle/lib/gradle-api-6.1.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/IntelliLang/lib/intellilang-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/eclipse/lib/eclipse-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/eclipse/lib/common-eclipse-util.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/ant/lib/ant-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/Groovy/lib/groovy-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/Groovy/lib/groovy-rt-constants.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/maven/lib/maven-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/osmorc/lib/osmorc-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/osmorc/lib/biz.aQute.bndlib-5.0.0.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/osmorc/lib/biz.aQute.repository-5.0.0.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/osmorc/lib/biz.aQute.resolve-5.0.0.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/osmorc/lib/bundlor-all.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/aspectj/lib/aspectj-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/gradle-java/lib/gradle-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/javaFX/lib/javaFX-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/javaFX/lib/common-javaFX-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/uiDesigner/lib/jps/ui-designer-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/devkit/lib/devkit-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/flex/lib/flex-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/flex/lib/flex-shared.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/JavaEE/lib/javaee-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/JavaEE/lib/jps/jpa-jps-plugin.jar;C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/plugins/Kotlin/lib/jps/kotlin-jps-plugin.jar;C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/plugins/Kotlin/lib/kotlin-stdlib.jar;C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/plugins/Kotlin/lib/kotlin-reflect.jar;C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/plugins/Kotlin/lib/kotlin-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/jps/android-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/android-common.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/build-common.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/android-rt.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/android-extensions-ide.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/sdk-common-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/common-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/repository-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/sdklib-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/jarutils.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/layoutlib-api-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/android/lib/manifest-merger-26.6.0.2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/webSphereIntegration/lib/jps/webSphere-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/weblogicIntegration/lib/jps/weblogic-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/dmServer/lib/dmServer-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/GwtStudio/lib/gwt-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/Grails/lib/grails-jps-plugin.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/Grails/lib/grails-compiler-patch.jar org.jetbrains.jps.cmdline.BuildMain 127.0.0.1 58221 1c5335e7-7413-4244-a14b-ddde3a86e48f C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/compile-server
java.class.path = D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/jps-launcher.jar;C:/Program Files/Java/jdk1.8.0_192/lib/tools.jar
io.netty.machineId = 28:f0:76:ff:fe:16:65:0e
java.vm.specification.name = Java Virtual Machine Specification
jps.backward.ref.index.builder = true
java.vm.specification.version = 1.8
preload.config.path = C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/options
sun.cpu.endian = little
sun.os.patch.level =
java.awt.headless = true
java.io.tmpdir = C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/compile-server/finchain-web_f72b1d8/_temp_
java.vendor.url.bug = http://bugreport.sun.com/bugreport/
os.arch = amd64
java.awt.graphicsenv = sun.awt.Win32GraphicsEnvironment
java.ext.dirs = C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext;C:\WINDOWS\Sun\Java\lib\ext
rebuild.on.dependency.change = true
user.dir = C:\Users\fcsca\AppData\Local\JetBrains\IntelliJIdea2020.1\compile-server
line.separator =

java.vm.name = Java HotSpot(TM) 64-Bit Server VM
file.encoding = GBK
java.specification.version = 1.8
kotlin.incremental.compilation.js = true
compile.parallel = false
io.netty.noUnsafe = true

VM Flags:
Non-default VM flags: -XX:CICompilerCount=3 -XX:InitialHeapSize=268435456 -XX:MaxHeapSize=734003200 -XX:MaxNewSize=244318208 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=89128960 -XX:OldSize=179306496 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
Command line:  -Xmx700m -Djava.awt.headless=true -Djava.endorsed.dirs="" -Djdt.compiler.useSingleThread=true -Dpreload.project.path=D:/LinkSpace/Work/WorkSpace/Code/IDEA/finchain-web -Dpreload.config.path=C:/Users/fcsca/AppData/Roaming/JetBrains/IntelliJIdea2020.1/options -Dcompile.parallel=false -Drebuild.on.dependency.change=true -Dio.netty.initialSeedUniquifier=5655166389141628370 -Dfile.encoding=GBK -Duser.language=zh -Duser.country=CN -Didea.paths.selector=IntelliJIdea2020.1 -Didea.home.path=D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win -Didea.config.path=C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1 -Didea.plugins.path=C:\Users\fcsca\AppData\Roaming\JetBrains\IntelliJIdea2020.1\plugins -Djps.log.dir=C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/log/build-log -Djps.fallback.jdk.home=D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/jbr -Djps.fallback.jdk.version=11.0.7 -Dio.netty.noUnsafe=true -Djava.io.tmpdir=C:/Users/fcsca/AppData/Local/JetBrains/IntelliJIdea2020.1/compile-server/finchain-web_f72b1d8/_temp_ -Djps.backward.ref.index.builder=true -Dkotlin.incremental.compilation=true -Dkotlin.incremental.compilation.js=true -Dkotlin.daemon.enabled -Dkotlin.daemon.client.alive.path="C:\Users\fcsca\AppData\Local\Temp\kotlin-idea-5395178358329969888-is-running"

fcsca@FCANT /                                                                                                     [17:38]
❯ jinfo 29492
Attaching to process ID 29492, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
Java System Properties:

spring.output.ansi.enabled = always
java.runtime.name = Java(TM) SE Runtime Environment
java.vm.version = 25.192-b12
sun.boot.library.path = C:\Program Files\Java\jdk1.8.0_192\jre\bin
java.vendor.url = http://java.oracle.com/
java.vm.vendor = Oracle Corporation
path.separator = ;
java.rmi.server.randomIDs = true
file.encoding.pkg = sun.io
java.vm.name = Java HotSpot(TM) 64-Bit Server VM
sun.os.patch.level =
sun.java.launcher = SUN_STANDARD
user.script =
user.country = CN
user.dir = D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web
java.vm.specification.name = Java Virtual Machine Specification
PID = 29492
intellij.debug.agent = true
java.runtime.version = 1.8.0_192-b12
java.awt.graphicsenv = sun.awt.Win32GraphicsEnvironment
os.arch = amd64
java.endorsed.dirs = C:\Program Files\Java\jdk1.8.0_192\jre\lib\endorsed
org.jboss.logging.provider = slf4j
line.separator =

java.io.tmpdir = C:\Users\fcsca\AppData\Local\Temp\
java.vm.specification.vendor = Oracle Corporation
user.variant =
os.name = Windows 10
sun.jnu.encoding = GBK
java.library.path = C:\Program Files\Java\jdk1.8.0_192\bin;C:\WINDOWS\Sun\Java\bin;C:\WINDOWS\system32;C:\WINDOWS;D:\Program Files\Xshell\;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;c:\Program Files\Intel\WiFi\bin\;c:\Program Files\Common Files\Intel\WirelessCommon\;C:\WINDOWS\System32\OpenSSH\;D:\Program Files\Development\Git\Git\cmd;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\bin;C:\Program Files\Java\jdk1.8.0_192\bin;D:\LinkSpace\Download\DevelopPackage\MySQL\mysql-8.0.16-winx64\mysql-8.0.16-winx64\bin;D:\LinkSpace\Download\DevelopPackage\Node\node-v14.2.0-win-x64\node-v14.2.0-win-x64;C:\Program Files\dotnet\;D:\LinkSpace\Download\DevelopPackage\Gradle\gradle-6.5-all\gradle-6.5\bin;D:\LinkSpace\Download\DevelopPackage\Redis\Redis-x64-3.2.100;C:\Users\fcsca\AppData\Local\Microsoft\WindowsApps;C:\Program Files\Bandizip\;.
spring.beaninfo.ignore = true
jboss.modules.system.pkgs = com.intellij.rt
java.class.version = 52.0
java.specification.name = Java Platform API Specification
spring.jmx.enabled = true
sun.management.compiler = HotSpot 64-Bit Tiered Compilers
spring.liveBeansView.mbeanDomain =
os.version = 10.0
user.home = C:\Users\fcsca
user.timezone = GMT+08:00
catalina.useNaming = false
java.awt.printerjob = sun.awt.windows.WPrinterJob
file.encoding = UTF-8
java.specification.version = 1.8
catalina.home = C:\Users\fcsca\AppData\Local\Temp\tomcat.1890005552553569967.8085
user.name = fcsca
java.class.path = C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\deploy.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\access-bridge-64.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\cldrdata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\dnsns.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jaccess.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\jfxrt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\localedata.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\nashorn.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunec.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunjce_provider.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunmscapi.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\sunpkcs11.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext\zipfs.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\javaws.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfxswt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\management-agent.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\plugin.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\rt.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\target;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\cn\hutool\hutool-all\4.5.6\hutool-all-4.5.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\projectlombok\lombok\1.16.18\lombok-1.16.18.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot\1.5.7.RELEASE\spring-boot-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-autoconfigure\1.5.7.RELEASE\spring-boot-autoconfigure-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\hamcrest\hamcrest-core\1.3\hamcrest-core-1.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-core\4.3.11.RELEASE\spring-core-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\freemarker\freemarker\2.3.23\freemarker-2.3.23.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\smart\smart-kernal\1.0\smart-kernal-1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\sf\barcode4j\barcode4j-light\2.0\barcode4j-light-2.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\xml-apis\xml-apis\1.4.01\xml-apis-1.4.01.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\belerweb\pinyin4j\2.5.0\pinyin4j-2.5.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\binarywang\weixin-java-pay\3.1.0\weixin-java-pay-3.1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\binarywang\weixin-java-common\3.1.0\weixin-java-common-3.1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\thoughtworks\xstream\xstream\1.4.9\xstream-1.4.9.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\xmlpull\xmlpull\1.1.3.1\xmlpull-1.1.3.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\xpp3\xpp3_min\1.1.4c\xpp3_min-1.1.4c.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\httpcomponents\httpmime\4.5.3\httpmime-4.5.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-io\commons-io\2.5\commons-io-2.5.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\binarywang\qrcode-utils\1.1\qrcode-utils-1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\jodd\jodd-http\3.7.1\jodd-http-3.7.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\jodd\jodd-core\3.7.1\jodd-core-3.7.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\jodd\jodd-upload\3.7.1\jodd-upload-3.7.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-beanutils\commons-beanutils\1.9.3\commons-beanutils-1.9.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\binarywang\weixin-java-mp\3.1.0\weixin-java-mp-3.1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\aliyun\aliyun-java-sdk-sms\3.0.0-rc1\aliyun-java-sdk-sms-3.0.0-rc1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\aliyun\aliyun-java-sdk-dysmsapi\1.0.0\aliyun-java-sdk-dysmsapi-1.0.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\sf\json-lib\json-lib\2.4\json-lib-2.4-jdk15.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-collections\commons-collections\3.2.2\commons-collections-3.2.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-lang\commons-lang\2.5\commons-lang-2.5.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\sf\ezmorph\ezmorph\1.0.6\ezmorph-1.0.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-web\1.5.7.RELEASE\spring-boot-starter-web-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter\1.5.7.RELEASE\spring-boot-starter-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\yaml\snakeyaml\1.17\snakeyaml-1.17.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-tomcat\1.5.7.RELEASE\spring-boot-starter-tomcat-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\tomcat\embed\tomcat-embed-core\8.5.20\tomcat-embed-core-8.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\tomcat\embed\tomcat-embed-el\8.5.20\tomcat-embed-el-8.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\tomcat\embed\tomcat-embed-websocket\8.5.20\tomcat-embed-websocket-8.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\hibernate\hibernate-validator\5.3.5.Final\hibernate-validator-5.3.5.Final.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\javax\validation\validation-api\1.1.0.Final\validation-api-1.1.0.Final.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\jboss\logging\jboss-logging\3.3.1.Final\jboss-logging-3.3.1.Final.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-web\4.3.11.RELEASE\spring-web-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-webmvc\4.3.11.RELEASE\spring-webmvc-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-security\1.5.7.RELEASE\spring-boot-starter-security-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\security\spring-security-config\4.2.3.RELEASE\spring-security-config-4.2.3.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\security\spring-security-web\4.2.3.RELEASE\spring-security-web-4.2.3.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-jdbc\1.5.7.RELEASE\spring-boot-starter-jdbc-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\tomcat\tomcat-jdbc\8.5.20\tomcat-jdbc-8.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\tomcat\tomcat-juli\8.5.20\tomcat-juli-8.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-jdbc\4.3.11.RELEASE\spring-jdbc-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-tx\4.3.11.RELEASE\spring-tx-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-logging\1.5.7.RELEASE\spring-boot-starter-logging-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\ch\qos\logback\logback-classic\1.1.11\logback-classic-1.1.11.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\ch\qos\logback\logback-core\1.1.11\logback-core-1.1.11.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\slf4j\jcl-over-slf4j\1.7.25\jcl-over-slf4j-1.7.25.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\slf4j\jul-to-slf4j\1.7.25\jul-to-slf4j-1.7.25.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\slf4j\log4j-over-slf4j\1.7.25\log4j-over-slf4j-1.7.25.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-mail\1.5.7.RELEASE\spring-boot-starter-mail-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\sun\mail\javax.mail\1.5.6\javax.mail-1.5.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-freemarker\1.5.7.RELEASE\spring-boot-starter-freemarker-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\boot\spring-boot-starter-aop\1.5.7.RELEASE\spring-boot-starter-aop-1.5.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\aspectj\aspectjweaver\1.8.10\aspectjweaver-1.8.10.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\kafka\spring-kafka\1.1.6.RELEASE\spring-kafka-1.1.6.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-messaging\4.3.11.RELEASE\spring-messaging-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\retry\spring-retry\1.2.1.RELEASE\spring-retry-1.2.1.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\kafka\kafka-clients\0.10.1.1\kafka-clients-0.10.1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\jpountz\lz4\lz4\1.3.0\lz4-1.3.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\xerial\snappy\snappy-java\1.1.2.6\snappy-java-1.1.2.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-context-support\4.3.11.RELEASE\spring-context-support-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-codec\commons-codec\1.10\commons-codec-1.10.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\commons\commons-lang3\3.6\commons-lang3-3.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\commons\commons-text\1.2\commons-text-1.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\guava\guava\23.0\guava-23.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\code\findbugs\jsr305\1.3.9\jsr305-1.3.9.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\errorprone\error_prone_annotations\2.0.18\error_prone_annotations-2.0.18.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\j2objc\j2objc-annotations\1.1\j2objc-annotations-1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\codehaus\mojo\animal-sniffer-annotations\1.14\animal-sniffer-annotations-1.14.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\security\spring-security-data\4.2.3.RELEASE\spring-security-data-4.2.3.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\data\spring-data-commons\1.13.7.RELEASE\spring-data-commons-1.13.7.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\security\spring-security-core\4.2.3.RELEASE\spring-security-core-4.2.3.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\aopalliance\aopalliance\1.0\aopalliance-1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\mysql\mysql-connector-java\5.1.44\mysql-connector-java-5.1.44.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\mybatis\mybatis-spring\1.3.1\mybatis-spring-1.3.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\mybatis\mybatis\3.4.5\mybatis-3.4.5.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\tk\mybatis\mapper\3.4.2\mapper-3.4.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\javax\persistence\persistence-api\1.0\persistence-api-1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\pagehelper\pagehelper\5.1.2\pagehelper-5.1.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\jsqlparser\jsqlparser\1.0\jsqlparser-1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\mybatis\spring\boot\mybatis-spring-boot-autoconfigure\1.3.1\mybatis-spring-boot-autoconfigure-1.3.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\mybatis\mybatis-typehandlers-jsr310\1.0.2\mybatis-typehandlers-jsr310-1.0.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\alibaba\fastjson\1.2.39\fastjson-1.2.39.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\alibaba\druid-spring-boot-starter\1.1.4\druid-spring-boot-starter-1.1.4.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\alibaba\druid\1.1.4\druid-1.1.4.jar;C:\Program Files\Java\jdk1.8.0_192\lib\jconsole.jar;C:\Program Files\Java\jdk1.8.0_192\lib\tools.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\jsonwebtoken\jjwt\0.7.0\jjwt-0.7.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\logstash\logback\logstash-logback-encoder\4.9\logstash-logback-encoder-4.9.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\junit\jupiter\junit-jupiter-api\5.7.0-M1\junit-jupiter-api-5.7.0-M1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apiguardian\apiguardian-api\1.1.0\apiguardian-api-1.1.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\opentest4j\opentest4j\1.2.0\opentest4j-1.2.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\junit\platform\junit-platform-commons\1.7.0-M1\junit-platform-commons-1.7.0-M1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\sf\ehcache\ehcache-core\2.6.11\ehcache-core-2.6.11.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\sun\jna\jna\3.0.9\jna-3.0.9.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\microsoft\sqlserver\sqljdbc4\4.0\sqljdbc4-4.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\quartz-scheduler\quartz\2.2.2\quartz-2.2.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\c3p0\c3p0\0.9.1.1\c3p0-0.9.1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\quartz-scheduler\quartz-jobs\2.2.2\quartz-jobs-2.2.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\jackson\module\jackson-module-jaxb-annotations\2.8.10\jackson-module-jaxb-annotations-2.8.10.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\core-1.0-SNAPSHOT.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\ra-toolkit-3.6.7.2.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\SADK-3.5.2.1.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\logback-cfca-jdk1.6-3.2.0.0.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\tjfoc-gmsm-0.0.1-SNAPSHOT-jar-with-dependencies.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\jackson\datatype\jackson-datatype-jsr310\2.8.10\jackson-datatype-jsr310-2.8.10.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\jackson\core\jackson-annotations\2.8.0\jackson-annotations-2.8.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\jackson\core\jackson-core\2.8.10\jackson-core-2.8.10.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\jackson\core\jackson-databind\2.8.10\jackson-databind-2.8.10.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\json\json\20080701\json-20080701.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\junit\junit\4.12-beta-3\junit-4.12-beta-3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\aliyun\oss\aliyun-sdk-oss\2.8.3\aliyun-sdk-oss-2.8.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\jdom\jdom\1.1\jdom-1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\pdfbox\pdfbox\2.0.12\pdfbox-2.0.12.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\pdfbox\fontbox\2.0.12\fontbox-2.0.12.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\commons-logging\commons-logging\1.2\commons-logging-1.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\poi\poi\3.17\poi-3.17.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\commons\commons-collections4\4.1\commons-collections4-4.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\poi\poi-ooxml\3.17\poi-ooxml-3.17.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\poi\poi-ooxml-schemas\3.17\poi-ooxml-schemas-3.17.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\xmlbeans\xmlbeans\2.6.0\xmlbeans-2.6.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\stax\stax-api\1.0.1\stax-api-1.0.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\virtuald\curvesapi\1.04\curvesapi-1.04.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\poi\poi-scratchpad\3.17\poi-scratchpad-3.17.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\deepoove\poi-tl\1.4.2\poi-tl-1.4.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\slf4j\slf4j-api\1.7.25\slf4j-api-1.7.25.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-context\4.3.11.RELEASE\spring-context-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-aop\4.3.11.RELEASE\spring-aop-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-beans\4.3.11.RELEASE\spring-beans-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-expression\4.3.11.RELEASE\spring-expression-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-swagger2\2.9.2\springfox-swagger2-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\swagger\swagger-annotations\1.5.20\swagger-annotations-1.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\swagger\swagger-models\1.5.20\swagger-models-1.5.20.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-spi\2.9.2\springfox-spi-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-core\2.9.2\springfox-core-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\net\bytebuddy\byte-buddy\1.8.12\byte-buddy-1.8.12.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-schema\2.9.2\springfox-schema-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-swagger-common\2.9.2\springfox-swagger-common-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-spring-web\2.9.2\springfox-spring-web-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\fasterxml\classmate\1.3.4\classmate-1.3.4.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\plugin\spring-plugin-core\1.2.0.RELEASE\spring-plugin-core-1.2.0.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\plugin\spring-plugin-metadata\1.2.0.RELEASE\spring-plugin-metadata-1.2.0.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\mapstruct\mapstruct\1.2.0.Final\mapstruct-1.2.0.Final.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\springfox\springfox-swagger-ui\2.9.2\springfox-swagger-ui-2.9.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\github\xiaoymin\swagger-bootstrap-ui\1.8.5\swagger-bootstrap-ui-1.8.5.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\bouncycastle\bcprov-jdk15on\1.64\bcprov-jdk15on-1.64.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\bouncycastle\bcpkix-jdk15on\1.64\bcpkix-jdk15on-1.64.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\itextpdf\itextpdf\5.5.13\itextpdf-5.5.13.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\itextpdf\itext-pdfa\5.5.13\itext-pdfa-5.5.13.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\itextpdf\itext-asian\5.2.0\itext-asian-5.2.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\itextpdf\tool\xmlworker\5.5.13\xmlworker-5.5.13.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\xhtmlrenderer\flying-saucer-pdf\9.0.7\flying-saucer-pdf-9.0.7.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\lowagie\itext\2.1.7\itext-2.1.7.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\bouncycastle\bcmail-jdk14\138\bcmail-jdk14-138.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\bouncycastle\bcprov-jdk14\138\bcprov-jdk14-138.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\bouncycastle\bctsp-jdk14\1.38\bctsp-jdk14-1.38.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\bouncycastle\bcprov-jdk14\1.38\bcprov-jdk14-1.38.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\bouncycastle\bcmail-jdk14\1.38\bcmail-jdk14-1.38.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\xhtmlrenderer\flying-saucer-core\9.0.7\flying-saucer-core-9.0.7.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\openoffice\juh\2.3.0\juh-2.3.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\openoffice\jurt\2.3.0\jurt-2.3.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\openoffice\ridl\2.3.0\ridl-2.3.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\openoffice\unoil\2.3.0\unoil-2.3.0.jar;D:\LinkSpace\Work\WorkSpace\Code\IDEA\finchain-web\libs\jodconverter-core-3.0-beta-4.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\httpcomponents\httpclient\4.5.3\httpclient-4.5.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\apache\httpcomponents\httpcore\4.4.6\httpcore-4.4.6.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\org\springframework\spring-test\4.3.11.RELEASE\spring-test-4.3.11.RELEASE.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\aliyun\aliyun-java-sdk-core\4.0.3\aliyun-java-sdk-core-4.0.3.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\javax\xml\bind\jaxb-api\2.1\jaxb-api-2.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\javax\xml\stream\stax-api\1.0-2\stax-api-1.0-2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\sun\xml\bind\jaxb-core\2.1.14\jaxb-core-2.1.14.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\sun\xml\bind\jaxb-impl\2.1\jaxb-impl-2.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\javax\activation\activation\1.1.1\activation-1.1.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\zxing\core\3.3.1\core-3.3.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\qiniu\qiniu-java-sdk\7.2.29\qiniu-java-sdk-7.2.29.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\squareup\okhttp3\okhttp\3.3.1\okhttp-3.3.1.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\squareup\okio\okio\1.8.0\okio-1.8.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\google\code\gson\gson\2.6.2\gson-2.6.2.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\com\qiniu\happy-dns-java\0.1.4\happy-dns-java-0.1.4.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\github\openfeign\feign-core\11.0\feign-core-11.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\github\openfeign\feign-jackson\11.0\feign-jackson-11.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\github\openfeign\form\feign-form\3.8.0\feign-form-3.8.0.jar;D:\LinkSpace\Download\DevelopPackage\Maven\apache-maven-3.6.1-bin\apache-maven-3.6.1\repo\io\github\openfeign\feign-slf4j\11.0\feign-slf4j-11.0.jar;D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\lib\idea_rt.jar;D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\plugins\java\lib\rt\debugger-agent.jar
com.sun.management.jmxremote =
java.vm.specification.version = 1.8
sun.arch.data.model = 64
sun.java.command = com.smart.Application
java.home = C:\Program Files\Java\jdk1.8.0_192\jre
user.language = zh
java.specification.vendor = Oracle Corporation
awt.toolkit = sun.awt.windows.WToolkit
java.vm.info = mixed mode
java.version = 1.8.0_192
java.ext.dirs = C:\Program Files\Java\jdk1.8.0_192\jre\lib\ext;C:\WINDOWS\Sun\Java\lib\ext
sun.boot.class.path = C:\Program Files\Java\jdk1.8.0_192\jre\lib\resources.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\rt.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\sunrsasign.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jsse.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jce.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\charsets.jar;C:\Program Files\Java\jdk1.8.0_192\jre\lib\jfr.jar;C:\Program Files\Java\jdk1.8.0_192\jre\classes
java.awt.headless = true
java.vendor = Oracle Corporation
catalina.base = C:\Users\fcsca\AppData\Local\Temp\tomcat.1890005552553569967.8085
spring.application.admin.enabled = true
file.separator = \
java.vendor.url.bug = http://bugreport.sun.com/bugreport/
sun.io.unicode.encoding = UnicodeLittle
sun.cpu.endian = little
sun.desktop = windows
sun.cpu.isalist = amd64

VM Flags:
Non-default VM flags: -XX:-BytecodeVerificationLocal -XX:-BytecodeVerificationRemote -XX:CICompilerCount=3 -XX:InitialHeapSize=268435456 -XX:+ManagementServer -XX:MaxHeapSize=4265607168 -XX:MaxNewSize=1421869056 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=89128960 -XX:OldSize=179306496 -XX:TieredStopAtLevel=1 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
Command line:  -agentlib:jdwp=transport=dt_socket,address=127.0.0.1:60543,suspend=y,server=n -XX:TieredStopAtLevel=1 -Xverify:none -Dspring.output.ansi.enabled=always -Dcom.sun.management.jmxremote -Dspring.jmx.enabled=true -Dspring.liveBeansView.mbeanDomain -Dspring.application.admin.enabled=true -javaagent:D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\plugins\java\lib\rt\debugger-agent.jar -Dfile.encoding=UTF-8
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594720172216-6fef2aec-bee3-4663-a393-16048c32f589.png#averageHue=%23404040&height=583&id=bMVbr&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2165624&status=done&style=none&title=&width=1107.6666666666667)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594720279859-a37e5d61-ac06-41d6-b98b-3d84f5557eb0.png#averageHue=%23464646&height=583&id=OMKga&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2208962&status=done&style=none&title=&width=1107.6666666666667)
<a name="cJUyC"></a>
### `jinfo -flags <PID>` ：打印对应进程ID的VM信息
```bash
❯ jinfo -flags 3476
Attaching to process ID 3476, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
Non-default VM flags: -XX:CICompilerCount=3 -XX:InitialHeapSize=268435456 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357564416 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=89128960 -XX:OldSize=179306496 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
Command line:  -Djava.awt.headless=true -Dmaven.defaultProjectBuilder.disableGlobalModelCache=true -Xmx1024m -Didea.maven.embedder.version=3.6.1 -Dmaven.ext.class.path=D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\plugins\maven\lib\maven-event-listener.jar -Dfile.encoding=GBK
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594994536901-3eb7405c-9c3a-494b-9f15-7e6b8445e0f0.png#averageHue=%234e4e4e&height=251&id=vUszu&originHeight=752&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=911869&status=done&style=none&title=&width=1107.6666666666667)

- Non-default VM flags：非默认VM标志
- Command line：命令行中指定的jvm参数
<a name="WZqAu"></a>
### `jinfo -sysprops <PID>` ：打印进程ID的系统属性信息
```bash
❯ jinfo -sysprops 3476
Attaching to process ID 3476, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
java.runtime.name = Java(TM) SE Runtime Environment
java.vm.version = 25.192-b12
sun.boot.library.path = C:\Program Files\Java\jdk1.8.0_192\jre\bin
java.vendor.url = http://java.oracle.com/
java.vm.vendor = Oracle Corporation
maven.multiModuleProjectDirectory =
path.separator = ;
guice.disable.misplaced.annotation.check = true
file.encoding.pkg = sun.io
java.vm.name = Java HotSpot(TM) 64-Bit Server VM
sun.os.patch.level =
sun.java.launcher = SUN_STANDARD
user.script =
user.country = CN
user.dir = D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\bin
java.vm.specification.name = Java Virtual Machine Specification
java.runtime.version = 1.8.0_192-b12
java.awt.graphicsenv = sun.awt.Win32GraphicsEnvironment
os.arch = amd64
java.endorsed.dirs = C:\Program Files\Java\jdk1.8.0_192\jre\lib\endorsed
line.separator =

java.io.tmpdir = C:\Users\fcsca\AppData\Local\Temp\
java.vm.specification.vendor = Oracle Corporation
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594994606204-14e44ce7-86f9-473b-ae8d-bf47500e827a.png#averageHue=%233e3e3e&height=583&id=JHVQP&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2101888&status=done&style=none&title=&width=1107.6666666666667)<br />打印的信息和`System.getProperties()`一样。
<a name="CbEC4"></a>
### `jinfo -flag <name> <pid>`: 打印指定的jvm参数信息
```
[root@localhost ~]# jinfo -flag InitialHeapSize 113595
-XX:InitialHeapSize=522190848
```
<a name="m2MEF"></a>
### `jinfo -flag [+|-] <name> <pid>` ：启用或者禁用指定的jvm参数
运行一段程序，下面这段程序vm参数设置为：-Xms5m -Xmx5m,运行过程中会参数OOM，在运行过程中，添加vm参数：`**+HeapDumpOnOutOfMemoryError**`：发生OOM的时候，让程序打印堆dump文件
```java
public class Test7 {
    private static final int _1M = 1024 * 1024;
    public static void main(String[] args) throws InterruptedException {
        List<Object> list = new ArrayList<>();
        for (int i = 0; i < 10; i++) {
            TimeUnit.SECONDS.sleep(3);
            list.add(new byte[_1M]);
        }
    }
}
```
运行过程中添加参数
```
C:\Users\Think>jps -l
44520 com.jvm.test7.Test7

C:\Users\Think>jinfo -flag +HeapDumpOnOutOfMemoryError 44520

C:\Users\Think>jinfo -flags 44520
Attaching to process ID 44520, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.121-b13
Non-default VM flags: -XX:CICompilerCount=4 -XX:+HeapDumpOnOutOfMemoryError -XX:InitialHeapSize=6291456 -XX:MaxHeapSize=6291456 -XX:MaxNewSize=2097152 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=2097152 -XX:OldSize=4194304 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -XX:+UseParallelGC
Command line:  -agentlib:jdwp=transport=dt_socket,address=127.0.0.1:53193,suspend=y,server=n -Xms5m -Xmx5m -Dfile.encoding=UTF-8
```
程序运行结果：
```
Connected to the target VM, address: '127.0.0.1:53193', transport: 'socket'
java.lang.OutOfMemoryError: Java heap space
Dumping heap to java_pid44520.hprof ...
Disconnected from the target VM, address: '127.0.0.1:53193', transport: 'socket'
Heap dump file created [4535342 bytes in 0.009 secs]
Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
    at com.jvm.test7.Test7.main(Test7.java:18)
```
<a name="QWSma"></a>
### `jinfo -flag <name>=<value> <pid>`：给指定的jvm参数设置值
