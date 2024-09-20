Java jmap<br />jmap-对堆进行立项分析，检查对象创建、数量及对象所占内存大小。<br />jmap命令(Memory Map for Java)主要用于打印指定Java进程(或核心文件、远程调试服务器)的共享对象内存映射或堆内存细节。<br />jmap命令可以获得运行中的jvm的堆的快照，从而可以离线分析堆，以检查内存泄漏，检查一些严重影响性能的大对象的创建，检查系统中什么对象最多，各种对象所占内存的大小等等。可以使用jmap生成Heap Dump。<br />如果不想使用jmap命令，要想获取Java堆转储快照还有一些比较“暴力”的手段：譬如在前面用过的 `-XX:+HeapDumpOnOutOfMemoryError`参数，可以让虚拟机在OOM异常出现之后自动生成dump文件，通过`-XX:+HeapDumpOnCtrlBreak`参数可以使用[ctrl]+[Break]键让虚拟机生成dump文件，又或者在Linux系统下通过Kill -3 命令发送进程退出信息“恐吓”一下虚拟机，也能拿到dump文件。<br />jmap的作用并不仅仅是为了获取dump文件，他还可以查询`finalize`执行队列，java堆和永久代的详细信息，如空间使用率、当前用的是哪种收集器等。
<a name="2fhd8"></a>
## jmap命令介绍
`jmap`命令可以用于对堆进行立项分析，检查对象创建、数量及对象所占内存大小。一般使用`jmap -heap PID`用于打印堆使用情况。<br />`jmap` 打印出 Java 进程内存中 Object 的情况，或者将 JVM 中的堆以二进制输出成文本，命令内容如下

| <no option> | 无参，会打印目标JVM加载的每个共享对象的起始地址、映射大小以及共享对象文件的路径 |
| --- | --- |
| -dump:[live],format=b,file=<filename> | 使用hprof=进制输出heap内容到文件，live代表只输出存活对象 |
| -finalizerinfo | 打印正等候回收的对象的信息 |
| -heap | 打印heap的概要信息，GC使用的算法，heap的配置及wise heap的使用情况 |
| -histo[:live] | 打印每个class内存信息，VM的内部类会加上前缀“*”，live代表只输出存活对象 |
| -clstats | 打印classload和jvm heap方法区的信息，每个classloader的名字,活泼性,地址,父classloader和加载的class数量。(另外内部String的数 量和占用内存数也会打印出来) |
| -F | 强迫。在pid没有响应的时候使用-dump或者-histo参数;在这个模式下，live子参数无效 |
| -h&#124;-help | 打印辅助信息 |
| -J<flag> | 传递参数给jmap启动的jvm |

```bash
❯ jmap -h
Usage:
    jmap [option] <pid>
        (to connect to running process)
    jmap [option] <executable <core>
        (to connect to a core file)
    jmap [option] [server_id@]<remote server IP or hostname>
        (to connect to remote debug server)

where <option> is one of:
    <none>               to print same info as Solaris pmap
    -heap                to print java heap summary
    -histo[:live]        to print histogram of java object heap; if the "live"
                         suboption is specified, only count live objects
    -clstats             to print class loader statistics
    -finalizerinfo       to print information on objects awaiting finalization
    -dump:<dump-options> to dump java heap in hprof binary format
                         dump-options:
                           live         dump only live objects; if not specified,
                                        all objects in the heap are dumped.
                           format=b     binary format
                           file=<file>  dump heap to <file>
                         Example: jmap -dump:live,format=b,file=heap.bin <pid>
    -F                   force. Use with -dump:<dump-options> <pid> or -histo
                         to force a heap dump or histogram when <pid> does not
                         respond. The "live" suboption is not supported
                         in this mode.
    -h | -help           to print this help message
    -J<flag>             to pass <flag> directly to the runtime system
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594716162605-db07f3fe-529f-45a7-95b9-ff1c3c67b1c4.png#averageHue=%23404040&height=583&id=kr2H6&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2078705&status=done&style=none&title=&width=1107.6666666666667)<br />主要选项：

| **选项** | **作用** |
| --- | --- |
| -dump | 生成java堆转储快照，格式为：-dump:[live,]format=b,file=<filename>，其中live子参数说明是否只dump出存活对象 |
| -finalizerinfo | 显示在F-Queue中等待Finalizer线程执行finalize方法的对象，只在linux/solaris平台下有效 |
| -heap | 显示堆详细信息，如使用哪种回收期、参数配置、分带状况等，只在linux/solaris平台下有效 |
| -histo | 显示堆中对象统计信息，包括类、实例数量和合计容量 |
| -permstat | 以ClassLoader为统计口径显示永久代内存状况，只在linux/solaris平台下有效 |
| -F | 当虚拟机进程对-dump选项没有响应时，可以使用这个选项强制生成dump快照，只在linux/solaris平台下有效 |

<a name="p7PTL"></a>
## jmap命令使用
<a name="HMjWV"></a>
### `jmap pid`：连接进程号
```bash
❯ jps
26528 Launcher
29492 Application
6276 Jps
17400
14412
fcsca@FCANT /                                                                                                      [17:53]
❯ jmap 29492
Attaching to process ID 29492, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
0x0000000063fe0000      8844K   C:\Program Files\Java\jdk1.8.0_192\jre\bin\server\jvm.dll
0x00000000651f0000      836K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\msvcr100.dll
0x0000000065320000      152K    C:\Program Files\Bonjour\mdnsNSP.dll
0x00007ff71e470000      220K    C:\Program Files\Java\jdk1.8.0_192\bin\java.exe
0x00007ffc709e0000      144K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\sunec.dll
0x00007ffc77f20000      68K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\nio.dll
0x00007ffc80ef0000      44K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\sunmscapi.dll
0x00007ffc840e0000      52K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\management.dll
0x00007ffc840f0000      140K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\instrument.dll
0x00007ffc8e960000      104K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\net.dll
0x00007ffc8ea00000      212K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\jdwp.dll
0x00007ffc8f930000      88K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\zip.dll
0x00007ffc8f9a0000      56K     C:\WINDOWS\System32\winrnr.dll
0x00007ffc8f9b0000      104K    C:\WINDOWS\system32\pnrpnsp.dll
0x00007ffc8f9d0000      88K     C:\WINDOWS\system32\napinsp.dll
0x00007ffc915d0000      2576K   C:\WINDOWS\WinSxS\amd64_microsoft.windows.common-controls_6595b64144ccf1df_6.0.18362.900_none_e6beb9d913147d17\COMCTL32.dll
0x00007ffc97850000      36K     C:\WINDOWS\SYSTEM32\WSOCK32.dll
0x00007ffc9d880000      40K     C:\WINDOWS\SYSTEM32\VERSION.dll
0x00007ffca4e50000      40K     C:\Windows\System32\rasadhlp.dll
0x00007ffca5bb0000      164K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\java.dll
0x00007ffca67e0000      36K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\dt_socket.dll
0x00007ffca78e0000      476K    C:\WINDOWS\System32\fwpuclnt.dll
0x00007ffca8950000      84K     C:\WINDOWS\system32\wshbth.dll
0x00007ffca8da0000      112K    C:\WINDOWS\system32\NLAapi.dll
0x00007ffcaa6b0000      180K    C:\WINDOWS\SYSTEM32\WINMMBASE.dll
0x00007ffcaab30000      144K    C:\WINDOWS\SYSTEM32\WINMM.dll
0x00007ffcaab70000      32K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\npt.dll
0x00007ffcaab80000      60K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\verify.dll
0x00007ffcab000000      112K    C:\WINDOWS\SYSTEM32\dhcpcsvc.DLL
0x00007ffcab020000      88K     C:\WINDOWS\SYSTEM32\dhcpcsvc6.DLL
0x00007ffcac3c0000      204K    C:\WINDOWS\system32\rsaenh.dll
0x00007ffcac5c0000      232K    C:\WINDOWS\SYSTEM32\IPHLPAPI.DLL
0x00007ffcac600000      812K    C:\WINDOWS\SYSTEM32\DNSAPI.dll
0x00007ffcac8b0000      412K    C:\WINDOWS\system32\mswsock.dll
0x00007ffcaca80000      48K     C:\WINDOWS\SYSTEM32\CRYPTBASE.dll
0x00007ffcacf60000      148K    C:\WINDOWS\SYSTEM32\USERENV.dll
0x00007ffcad040000      64K     C:\WINDOWS\System32\UMPDC.dll
0x00007ffcad050000      68K     C:\WINDOWS\System32\kernel.appcore.dll
0x00007ffcad070000      140K    C:\WINDOWS\System32\profapi.dll
0x00007ffcad0a0000      72K     C:\WINDOWS\System32\MSASN1.dll
0x00007ffcad0c0000      296K    C:\WINDOWS\System32\powrprof.dll
0x00007ffcad110000      1620K   C:\WINDOWS\System32\gdi32full.dll
0x00007ffcad2b0000      296K    C:\WINDOWS\System32\cfgmgr32.dll
0x00007ffcad300000      1000K   C:\WINDOWS\System32\ucrtbase.dll
0x00007ffcad400000      7680K   C:\WINDOWS\System32\windows.storage.dll
0x00007ffcadb80000      92K     C:\WINDOWS\System32\cryptsp.dll
0x00007ffcadba0000      1316K   C:\WINDOWS\System32\CRYPT32.dll
0x00007ffcadd50000      632K    C:\WINDOWS\System32\msvcp_win.dll
0x00007ffcaddf0000      512K    C:\WINDOWS\System32\bcryptPrimitives.dll
0x00007ffcadf20000      2704K   C:\WINDOWS\System32\KERNELBASE.dll
0x00007ffcae1d0000      132K    C:\WINDOWS\System32\win32u.dll
0x00007ffcae200000      152K    C:\WINDOWS\System32\bcrypt.dll
0x00007ffcae230000      676K    C:\WINDOWS\System32\shcore.dll
0x00007ffcae490000      712K    C:\WINDOWS\System32\KERNEL32.DLL
0x00007ffcaea20000      604K    C:\WINDOWS\System32\sechost.dll
0x00007ffcaeac0000      152K    C:\WINDOWS\System32\GDI32.dll
0x00007ffcaeaf0000      32K     C:\WINDOWS\System32\PSAPI.DLL
0x00007ffcaec60000      1152K   C:\WINDOWS\System32\RPCRT4.dll
0x00007ffcaeef0000      3284K   C:\WINDOWS\System32\combase.dll
0x00007ffcaf230000      7064K   C:\WINDOWS\System32\SHELL32.dll
0x00007ffcafbc0000      32K     C:\WINDOWS\System32\NSI.dll
0x00007ffcafcf0000      632K    C:\WINDOWS\System32\msvcrt.dll
0x00007ffcafd90000      444K    C:\WINDOWS\System32\WS2_32.dll
0x00007ffcafe00000      1616K   C:\WINDOWS\System32\USER32.dll
0x00007ffcaffa0000      652K    C:\WINDOWS\System32\ADVAPI32.dll
0x00007ffcb0050000      328K    C:\WINDOWS\System32\shlwapi.dll
0x00007ffcb00b0000      184K    C:\WINDOWS\System32\IMM32.DLL
0x00007ffcb0120000      1984K   C:\WINDOWS\SYSTEM32\ntdll.dll
fcsca@FCANT /                                                                                                      [17:54]
❯ jmap 26528
Attaching to process ID 26528, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12
0x0000000063fe0000      8844K   C:\Program Files\Java\jdk1.8.0_192\jre\bin\server\jvm.dll
0x00000000651f0000      836K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\msvcr100.dll
0x00007ff71e470000      220K    C:\Program Files\Java\jdk1.8.0_192\bin\java.exe
0x00007ffc77f20000      68K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\nio.dll
0x00007ffc8e340000      108K    D:\LinkSpace\Download\Jetbrains\ideaIU-2020.1.2.win\bin\IdeaWin64.dll
0x00007ffc8e960000      104K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\net.dll
0x00007ffc8f930000      88K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\zip.dll
0x00007ffc915d0000      2576K   C:\WINDOWS\WinSxS\amd64_microsoft.windows.common-controls_6595b64144ccf1df_6.0.18362.900_none_e6beb9d913147d17\COMCTL32.dll
0x00007ffc97850000      36K     C:\WINDOWS\SYSTEM32\WSOCK32.dll
0x00007ffc9d880000      40K     C:\WINDOWS\SYSTEM32\VERSION.dll
0x00007ffca5bb0000      164K    C:\Program Files\Java\jdk1.8.0_192\jre\bin\java.dll
0x00007ffcaa6b0000      180K    C:\WINDOWS\SYSTEM32\winmmbase.dll
0x00007ffcaab30000      144K    C:\WINDOWS\SYSTEM32\WINMM.dll
0x00007ffcaab80000      60K     C:\Program Files\Java\jdk1.8.0_192\jre\bin\verify.dll
0x00007ffcab000000      112K    C:\WINDOWS\SYSTEM32\dhcpcsvc.DLL
0x00007ffcab020000      88K     C:\WINDOWS\SYSTEM32\dhcpcsvc6.DLL
0x00007ffcac3c0000      204K    C:\WINDOWS\system32\rsaenh.dll
0x00007ffcac5c0000      232K    C:\WINDOWS\SYSTEM32\IPHLPAPI.DLL
0x00007ffcac600000      812K    C:\WINDOWS\SYSTEM32\DNSAPI.dll
0x00007ffcac8b0000      412K    C:\WINDOWS\system32\mswsock.dll
0x00007ffcaca80000      48K     C:\WINDOWS\SYSTEM32\CRYPTBASE.dll
0x00007ffcacf60000      148K    C:\WINDOWS\SYSTEM32\USERENV.dll
0x00007ffcad040000      64K     C:\WINDOWS\System32\UMPDC.dll
0x00007ffcad050000      68K     C:\WINDOWS\System32\kernel.appcore.dll
0x00007ffcad070000      140K    C:\WINDOWS\System32\profapi.dll
0x00007ffcad0c0000      296K    C:\WINDOWS\System32\powrprof.dll
0x00007ffcad110000      1620K   C:\WINDOWS\System32\gdi32full.dll
0x00007ffcad2b0000      296K    C:\WINDOWS\System32\cfgmgr32.dll
0x00007ffcad300000      1000K   C:\WINDOWS\System32\ucrtbase.dll
0x00007ffcad400000      7680K   C:\WINDOWS\System32\windows.storage.dll
0x00007ffcadb80000      92K     C:\WINDOWS\System32\cryptsp.dll
0x00007ffcadd50000      632K    C:\WINDOWS\System32\msvcp_win.dll
0x00007ffcaddf0000      512K    C:\WINDOWS\System32\bcryptPrimitives.dll
0x00007ffcadf20000      2704K   C:\WINDOWS\System32\KERNELBASE.dll
0x00007ffcae1d0000      132K    C:\WINDOWS\System32\win32u.dll
0x00007ffcae200000      152K    C:\WINDOWS\System32\bcrypt.dll
0x00007ffcae230000      676K    C:\WINDOWS\System32\shcore.dll
0x00007ffcae490000      712K    C:\WINDOWS\System32\KERNEL32.DLL
0x00007ffcaea20000      604K    C:\WINDOWS\System32\sechost.dll
0x00007ffcaeac0000      152K    C:\WINDOWS\System32\GDI32.dll
0x00007ffcaeaf0000      32K     C:\WINDOWS\System32\PSAPI.DLL
0x00007ffcaec60000      1152K   C:\WINDOWS\System32\RPCRT4.dll
0x00007ffcaeef0000      3284K   C:\WINDOWS\System32\combase.dll
0x00007ffcaf230000      7064K   C:\WINDOWS\System32\SHELL32.dll
0x00007ffcafbc0000      32K     C:\WINDOWS\System32\NSI.dll
0x00007ffcafcf0000      632K    C:\WINDOWS\System32\msvcrt.dll
0x00007ffcafd90000      444K    C:\WINDOWS\System32\WS2_32.dll
0x00007ffcafe00000      1616K   C:\WINDOWS\System32\USER32.dll
0x00007ffcaffa0000      652K    C:\WINDOWS\System32\ADVAPI32.dll
0x00007ffcb0050000      328K    C:\WINDOWS\System32\shlwapi.dll
0x00007ffcb00b0000      184K    C:\WINDOWS\System32\IMM32.DLL
0x00007ffcb0120000      1984K   C:\WINDOWS\SYSTEM32\ntdll.dll
❯ jmap -heap 26528
Attaching to process ID 26528, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.192-b12

using thread-local object allocation.
Parallel GC with 4 thread(s)

Heap Configuration:
   MinHeapFreeRatio         = 0
   MaxHeapFreeRatio         = 100
   MaxHeapSize              = 734003200 (700.0MB)
   NewSize                  = 89128960 (85.0MB)
   MaxNewSize               = 244318208 (233.0MB)
   OldSize                  = 179306496 (171.0MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)

Heap Usage:
PS Young Generation
Eden Space:
   capacity = 67108864 (64.0MB)
   used     = 41904328 (39.96308135986328MB)
   free     = 25204536 (24.03691864013672MB)
   62.44231462478638% used
From Space:
   capacity = 11010048 (10.5MB)
   used     = 5094976 (4.85894775390625MB)
   free     = 5915072 (5.64105224609375MB)
   46.27569289434524% used
To Space:
   capacity = 11010048 (10.5MB)
   used     = 0 (0.0MB)
   free     = 11010048 (10.5MB)
   0.0% used
PS Old Generation
   capacity = 179306496 (171.0MB)
   used     = 155680 (0.148468017578125MB)
   free     = 179150816 (170.85153198242188MB)
   0.08682340209247076% used

6758 interned Strings occupying 575312 bytes.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594720618682-6a3f6dd6-808a-45be-90e4-b2c27637786c.png#averageHue=%23454545&height=583&id=ZbbR2&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2225184&status=done&style=none&title=&width=1107.6666666666667)
<a name="qpKdR"></a>
### `jmap executable core`：连接核心文件
<a name="REoel"></a>
### `jmap -heap <PID>` ：查看当前堆的使用信息
显示堆详细信息
```bash
❯ jmap -heap 7253
Attaching to process ID 7253, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.65-b01

using thread-local object allocation.
Parallel GC with 8 thread(s)

Heap Configuration: #堆配置，即JVM参数配置
   MinHeapFreeRatio         = 0 #最小堆使用比例
   MaxHeapFreeRatio         = 100 #最大堆使用比例
   MaxHeapSize              = 4164943872 (3972.0MB) #最大堆空间大小
   NewSize                  = 87031808 (83.0MB) #新生代分配大小
   MaxNewSize               = 1388314624 (1324.0MB) #最大新生代可分配大小
   OldSize                  = 175112192 (167.0MB) #老年代大小
   NewRatio                 = 2 #新生代比例
   SurvivorRatio            = 8 #新生代和survivor比例
   MetaspaceSize            = 21807104 (20.796875MB) #元数据空间大小
   CompressedClassSpaceSize = 1073741824 (1024.0MB) #指针空间大小
   MaxMetaspaceSize         = 17592186044415 MB #MaxMetaspaceSize=Compressed Class Space Size + Metaspace area (excluding the Compressed Class Space) Size
   G1HeapRegionSize         = 0 (0.0MB) #G1中的region大小

Heap Usage:
PS Young Generation
Eden Space: #eden区
   capacity = 1342177280 (1280.0MB)
   used     = 970477176 (925.519157409668MB)
   free     = 371700104 (354.48084259033203MB)
   72.30618417263031% used
From Space: #from区，即survivor1区
   capacity = 23068672 (22.0MB)
   used     = 22904896 (21.84381103515625MB)
   free     = 163776 (0.15618896484375MB)
   99.29005015980114% used
To Space: #to区，即survivor2区
   capacity = 23068672 (22.0MB)
   used     = 0 (0.0MB)
   free     = 23068672 (22.0MB)
   0.0% used
PS Old Generation #老年代使用情况
   capacity = 176685056 (168.5MB)
   used     = 52151696 (49.73573303222656MB)
   free     = 124533360 (118.76426696777344MB)
   29.5167555087398% used

31742 interned Strings occupying 3385416 bytes.
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594720667394-f16baff6-eda6-4ff3-9672-fe8171260209.png#averageHue=%233c3c3c&height=583&id=v8OhN&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2059790&status=done&style=none&title=&width=1107.6666666666667)![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594720675715-c1bac8e2-295f-4ef7-b14f-4c971e2abeda.png#averageHue=%233c3c3c&height=583&id=YmgAA&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2062482&status=done&style=none&title=&width=1107.6666666666667)
<a name="iKjyS"></a>
### `jmap -dump:live,format=b,file=heap.bin [PID]` ：将堆的信息以文件的形式保存
```bash
❯ jmap -dump:live,format=b,file=heap.bin 9244
Dumping heap to D:\LinkSpace\Download\DevelopPackage\Arthas\arthas-bin\heap.bin ...
Heap dump file created
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594887309585-4ab75d07-91a5-46a3-a761-c7b699b5df2c.png#averageHue=%233d3d3d&height=115&id=loF6B&originHeight=344&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=422443&status=done&style=none&title=&width=1107.6666666666667)<br />在对应的目录下可以看到生成的heap.bin文件<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594888270955-7076f3ed-4e93-4f06-8090-ee2c2f5829f8.png#averageHue=%23f9f8f7&height=554&id=E6fFy&originHeight=1663&originWidth=2250&originalType=binary&ratio=1&rotation=0&showTitle=false&size=385412&status=done&style=none&title=&width=750)<br />生成的 `heap.bin` 可以使用 `jhat` 命令打开，是以 html 的形式展示的。<br />[jhat-分析Java堆 · 语雀](https://www.yuque.com/fcant/sys/zmvept?inner=xwTdo&view=doc_embed)<br />可以使用jdk提供的jvisualvm.exe查看hprof文件<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1691943553143-e6300963-1877-4594-b6b2-53fd3e24ac0e.png#averageHue=%23f6f5f4&clientId=ubb2cf073-14a2-4&from=paste&height=689&id=u85723413&originHeight=1723&originWidth=2145&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=980896&status=done&style=none&taskId=u64e24a22-56e7-4904-abdc-65a5a69b2c0&title=&width=858)
<a name="burU4"></a>
### `jmap -histo`：显示堆中对象统计信息
显示堆中对象统计信息，包括类、实例数量和合计容量<br />**命令格式**：`jmap -histo[:live] <pid>`
```bash
C:\Users\Think>jmap -histo 28252

 num     #instances         #bytes  class name
----------------------------------------------
   1:        309006       16963968  [C
   2:          1081        7275840  [I
   3:         41164        3156952  [B
   4:         90125        2163000  java.lang.String
   5:         21000         672000  java.util.UUID
   6:         21000         336000  com.jvm.test8.Test8$User
   7:         21000         336000  com.jvm.test8.Test8$User$UserBuilder
   8:           799         300072  [Ljava.lang.Object;
   9:          7557         181368  java.lang.StringBuilder
  10:           772          87704  java.lang.Class
  11:          1026          65664  sun.nio.fs.WindowsFileAttributes
  12:          1026          49248  sun.nio.fs.WindowsPath$WindowsPathWithAttributes
  13:           837          33480  java.util.TreeMap$Entry
  14:          1032          33024  java.lang.ref.WeakReference
  15:           775          31000  sun.nio.fs.WindowsPath
  16:          1028          24672  sun.nio.fs.WindowsPathParser$Result
  17:           424          13568  java.io.File
  18:           168          12096  java.lang.reflect.Field
  19:           299          11960  java.util.LinkedHashMap$Entry
  20:           323          11200  [Ljava.lang.String;
  21:           173          11072  java.net.URL
  22:           341          10912  sun.misc.FDBigInteger
  23:           312           9984  java.util.Hashtable$Entry
  24:            66           8992  [Ljava.util.HashMap$Node;
  25:           267           8544  java.util.HashMap$Node
  26:           195           7800  java.lang.ref.Finalizer
  27:           264           6336  java.lang.StringBuffer
  28:           121           4840  java.lang.ref.SoftReference
  29:            29           4816  [Ljava.util.Hashtable$Entry;
  30:            50           4800  java.util.jar.JarFile$JarFileEntry
  31:           105           4552  [[C
  32:            53           4240  [Ljava.util.WeakHashMap$Entry;
  33:            74           4144  sun.misc.URLClassPath$JarLoader
  34:           258           4128  java.lang.Integer
  35:            50           4000  java.util.zip.ZipEntry
  36:            79           3792  java.net.NetworkInterface
  37:           150           3600  java.net.Parts
  38:           111           3552  java.util.concurrent.ConcurrentHashMap$Node
  39:           134           3216  java.security.Provider$ServiceKey
  40:            50           3200  java.util.jar.JarFile
  41:            55           3080  sun.nio.cs.UTF_8$Encoder
  42:             8           3008  java.lang.Thread
  43:            62           2976  java.util.HashMap
  44:            51           2856  java.util.zip.ZipFile$ZipFileInputStream
  45:           114           2736  java.io.ExpiringCache$Entry
  46:            53           2544  java.util.WeakHashMap
  47:            30           2400  java.lang.reflect.Constructor
  48:            56           2240  java.util.WeakHashMap$Entry
  49:            39           2184  java.util.zip.ZipFile$ZipFileInflaterInputStream
```
<a name="yNidK"></a>
### `jmap [server-id@]remote-hostname-or-IP`：连接远程debug服务的主机名或ip
