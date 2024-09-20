Java jcmd
<a name="vyS0s"></a>
## jcmd常用命令如下
| jcmd | 查看本地的java进程列表，获取其pid |
| --- | --- |
| jcmd pid help | 查看其支持的命令列表 |
| jcmd pid Thread.print -&#124;I | 打印线程栈 |
| jcmd pid VM.command_ line | 打印启动命令及参数 |
| jcmd pid GC.heap_ _dump /data/filename.dump | 查看JVM的Heap Dump |
| jcmd pid GC.class_ histogram | 查看类的统计信息 |
| jcmd pid VM.system_ properties | 查看系统属性内容 |
| jcmd pid VM.uptime | 查看虚拟机启动时间 |
| jcmd pid PerfCounter.print | 查看性能统计 |

<a name="ybWIl"></a>
### `jcmd` ：查看本地的java进程列表，获取其pid
```bash
❯ jcmd
24656
12948 com.smart.Application
27940 org.jetbrains.jps.cmdline.Launcher D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/javac2.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/maven-resolver-transport-http-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jna-platform.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-repository-metadata-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/lz4-java-1.7.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-artifact-3.6.1.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/plugins/java/lib/aether-dependency-resolver.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/maven-resolver-api-1.3.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/jdom.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/annotations.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/nanoxml-2.2.3.jar;D:/LinkSpace/Download/Jetbrains/ideaIU-2020.1.2.win/lib/guava-28.2-jre.jar;D:/LinkS
3476 org.jetbrains.idea.maven.server.RemoteMavenServer36
24088 sun.tools.jcmd.JCmd
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594997392394-c590cc7b-7429-471d-ac9c-1f441ac5f8b5.png#align=left&display=inline&height=313&originHeight=940&originWidth=3323&size=1226458&status=done&style=none&width=1107.6666666666667)
<a name="Zbxnv"></a>
###  jcmd <PID> help：查看对应进程支持的命令列表
```bash
❯ jcmd 12948 help
12948:
The following commands are available:
JFR.stop
JFR.start
JFR.dump
JFR.check
VM.native_memory
VM.check_commercial_features
VM.unlock_commercial_features
ManagementAgent.stop
ManagementAgent.start_local
ManagementAgent.start
VM.classloader_stats
GC.rotate_log
Thread.print
GC.class_stats
GC.class_histogram
GC.heap_dump
GC.finalizer_info
GC.heap_info
GC.run_finalization
GC.run
VM.uptime
VM.dynlibs
VM.flags
VM.system_properties
VM.command_line
VM.version
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594997679963-f8a620d5-afa0-4891-aa7d-2449876d092e.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=1955588&status=done&style=none&width=1107.6666666666667)
<a name="vGCao"></a>
### `jcmd <PID> GC.heap_info` ：打印进程的堆栈信息
```bash
❯ jcmd 12948 GC.heap_info
12948:
 PSYoungGen      total 860672K, used 559299K [0x000000076b400000, 0x00000007a2a80000, 0x00000007c0000000)
  eden space 833024K, 66% used [0x000000076b400000,0x000000078d363f80,0x000000079e180000)
  from space 27648K, 10% used [0x00000007a0f80000,0x00000007a124cd80,0x00000007a2a80000)
  to   space 37376K, 0% used [0x000000079e180000,0x000000079e180000,0x00000007a0600000)
 ParOldGen       total 250880K, used 67497K [0x00000006c1c00000, 0x00000006d1100000, 0x000000076b400000)
  object space 250880K, 26% used [0x00000006c1c00000,0x00000006c5dea690,0x00000006d1100000)
 Metaspace       used 60125K, capacity 62428K, committed 62552K, reserved 1103872K
  class space    used 7495K, capacity 7947K, committed 8064K, reserved 1048576K
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594998019931-7d5c5604-cfaf-4828-b6f2-e19eda07d0c1.png#align=left&display=inline&height=222&originHeight=666&originWidth=3323&size=827800&status=done&style=none&width=1107.6666666666667)
