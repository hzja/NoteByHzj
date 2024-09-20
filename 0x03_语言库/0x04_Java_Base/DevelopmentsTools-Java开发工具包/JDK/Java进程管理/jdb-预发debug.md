Java jdb
<a name="8Knnd"></a>
### `jdb -help` ：jdb的帮助命令
```bash
❯ jdb -help
用法: jdb <options> <class> <arguments>

其中, 选项包括:
    -help             输出此消息并退出
    -sourcepath <由 ";" 分隔的目录>
                      要在其中查找源文件的目录
    -attach <address>
                      使用标准连接器附加到指定地址处正在运行的 VM
    -listen <address>
                      等待正在运行的 VM 使用标准连接器在指定地址处连接
    -listenany
                      等待正在运行的 VM 使用标准连接器在任何可用地址处连接
    -launch
                      立即启动 VM 而不是等待 'run' 命令
    -listconnectors   列出此 VM 中的可用连接器
    -connect <connector-name>:<name1>=<value1>,...
                      使用所列参数值通过指定的连接器连接到目标 VM
    -dbgtrace [flags] 输出信息供调试jdb
    -tclient          在 HotSpot(TM) 客户机编译器中运行应用程序
    -tserver          在 HotSpot(TM) 服务器编译器中运行应用程序

转发到被调试进程的选项:
    -v -verbose[:class|gc|jni]
                      启用详细模式
    -D<name>=<value>  设置系统属性
    -classpath <由 ";" 分隔的目录>
                      列出要在其中查找类的目录
    -X<option>        非标准目标 VM 选项

<class> 是要开始调试的类的名称
<arguments> 是传递到 <class> 的 main() 方法的参数

要获得命令的帮助, 请在jdb提示下键入 'help'
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594995332921-d3b1ba34-e32b-4722-9136-231080ebb912.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3404&size=2485072&status=done&style=none&width=1134.6666666666667)
<a name="ef1CU"></a>
### `jdb -attach <Port>` ：连接到对应实例VM的端口进行调试
<a name="FjFhP"></a>
### `jdb -listconnectors` ：列出本机VM中可用的实例
```bash
❯ jdb -listconnectors

  连接器: com.sun.jdi.CommandLineLaunch, 传输: dt_shmem
    说明: 使用 Sun Java VM 命令行启动目标并附加到该目标

    参数: home, 默认值: C:\Program Files\Java\jdk1.8.0_192\jre
    说明: 用于启动应用程序的 SDK 或运行时环境的主目录

    参数: options (无默认值)
    说明: 已启动的 VM 选项

    所需的参数: main (无默认值)
    说明: 主类和参数, 或者如果 -jar 是一个选项, 则为主 jar 文件和参数

    参数: suspend, 默认值: true
    说明: 在执行主类之前, 将挂起所有线程

    所需的参数: quote, 默认值: "
    说明: 用于将以空格分隔的文本组合为一个命令行参数的字符

    所需的参数: vmexec, 默认值: java
    说明: Java VM 启动程序的名称

  连接器: com.sun.jdi.RawCommandLineLaunch, 传输: dt_shmem
    说明: 使用用户指定的命令行启动目标并附加到该目标

    所需的参数: command (无默认值)
    说明: 用于启动调试应用程序 VM 的原始命令

    所需的参数: quote, 默认值: "
    说明: 用于将以空格分隔的文本组合为一个命令行参数的字符

    所需的参数: address (无默认值)
    说明: 运行原始命令之后, 监听连接时使用的地址

  连接器: com.sun.jdi.SocketAttach, 传输: dt_socket
    说明: 通过套接字附加到其他 VM

    参数: timeout (无默认值)
    说明: 等待附加操作时的超时

    参数: hostname, 默认值: Fcant
    说明: VM 连接所附加到的计算机名称

    所需的参数: port (无默认值)
    说明: VM 连接所附加到的端口号

  连接器: com.sun.jdi.SocketListen, 传输: dt_socket
    说明: 接受由其他 VM 启动的套接字连接

    参数: timeout (无默认值)
    说明: 等待连接时的超时

    参数: port (无默认值)
    说明: 监听 VM 连接时使用的端口号

    参数: localAddress (无默认值)
    说明: 监听程序绑定到的本地地址

  连接器: com.sun.jdi.SharedMemoryAttach, 传输: dt_shmem
    说明: 通过共享内存附加到其他 VM

    参数: timeout (无默认值)
    说明: 等待附加操作时的超时

    所需的参数: name (无默认值)
    说明: VM 连接所附加到的共享内存区域的名称

  连接器: com.sun.jdi.SharedMemoryListen, 传输: dt_shmem
    说明: 接受由其他 VM 启动的共享内存连接

    参数: timeout (无默认值)
    说明: 等待连接时的超时

    参数: name (无默认值)
    说明: 监听 VM 连接时所在的共享内存区域的名称

  连接器: com.sun.jdi.ProcessAttach, 传输: local
    说明: 通过进程 ID (PID) 附加到被调试进程

    所需的参数: pid (无默认值)
    说明: PID

    参数: timeout (无默认值)
    说明: 等待附加操作时的超时

  连接器: sun.jvm.hotspot.jdi.SACoreAttachingConnector, 传输: filesystem
    说明: This connector allows you to attach to a core file using the Serviceability Agent

    所需的参数: javaExecutable (无默认值)
    说明: Pathname of Java Executable

    参数: core, 默认值: core
    说明: Pathname of a corefile from a Java Process

  连接器: sun.jvm.hotspot.jdi.SADebugServerAttachingConnector, 传输: RMI
    说明: This connector allows you to attach to a Java Process via a debug server with the Serviceability Agent

    所需的参数: debugServerName (无默认值)
    说明: Name of a remote SA Debug Server

  连接器: sun.jvm.hotspot.jdi.SAPIDAttachingConnector, 传输: local process
    说明: This connector allows you to attach to a Java process using the Serviceability Agent

    所需的参数: pid (无默认值)
    说明: PID of a Java process
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1594996039666-5de83b3b-7869-4ecf-97ee-91f779f5ce39.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2041333&status=done&style=none&width=1107.6666666666667)
