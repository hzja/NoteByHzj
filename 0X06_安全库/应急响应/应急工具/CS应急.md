<a name="dS0F5"></a>
## 工具组合拳
应急步骤：先断网→BeaconEye.exe定位异常进程→清理掉木马→常见的应急排查思路排查定时任务→电脑重启→先打开appnetworkcounter→联网观察是否有异常进程→beaconeye继续运行定位→根据appnetworkcounter上联网的进程→process monitor监控分析看是不是恶意

有时候上机服务器排查，发现目标cs马的sleep可能比较长，不太好排查，这时候我会用appnetworkcounter工具记录所有外联的进程，这个工具一开始是会记录的比较少，因为他只捕捉有外联的服务进程<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666283914073-cfdfcaf2-eda2-4a2e-9d23-f2e4d940049f.png#clientId=u031400fa-bb80-4&from=paste&height=454&id=ud8c92cee&originHeight=568&originWidth=1426&originalType=binary&ratio=1&rotation=0&showTitle=false&size=88257&status=done&style=none&taskId=u9311c7c0-6d57-4b3f-bdba-5d5c1e2ce1c&title=&width=1140.8)

那么这个时候就要看那些系统进程，常见被cs用来进程迁移的，例如这个svchost.exe(7188)通过火绒剑查看对应的进程、或者用process monitor<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666284429645-a1dd01ed-2a92-49c9-912f-2f1c1f51137a.png#clientId=u031400fa-bb80-4&from=paste&height=602&id=u38701b19&originHeight=752&originWidth=1408&originalType=binary&ratio=1&rotation=0&showTitle=false&size=119982&status=done&style=none&taskId=u1006eb9d-cd30-42e2-a4be-7cb9b8994e1&title=&width=1126.4)

通过查看进程就可以看到执行过什么命令<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666284485408-c891a036-090f-4be9-923e-211f5b749250.png#clientId=u031400fa-bb80-4&from=paste&height=502&id=u90aa1838&originHeight=627&originWidth=1849&originalType=binary&ratio=1&rotation=0&showTitle=false&size=96234&status=done&style=none&taskId=u79bc2d2a-d3b6-4f07-8faa-e1815ad7ef9&title=&width=1479.2)

<a name="XbNxj"></a>
## cs检测工具
断网情况下，可以用BeaconEye.exe工具直接查杀(好用<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666368314481-df479e77-06bd-4157-b6b5-ae67fb733e2b.png#clientId=uff34d32e-a28f-4&from=paste&height=574&id=u8ba644d1&originHeight=718&originWidth=1350&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69867&status=done&style=none&taskId=u88f1f3bc-f6f7-4691-9b3d-73b5c65fd20&title=&width=1080)

cs 检测工具CobaltStrikeScan.exe(一般般，可以搭配着用<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666368595412-01662945-e896-400c-9e51-b92a566b1f58.png#clientId=uff34d32e-a28f-4&from=paste&height=637&id=u83f12464&originHeight=796&originWidth=692&originalType=binary&ratio=1&rotation=0&showTitle=false&size=127142&status=done&style=none&taskId=ua2d5cb6d-7f65-41b3-80f6-4f41aff29df&title=&width=553.6)

<a name="ZoXh5"></a>
## 线程注入检测
cs上线后通常都会inject一下进程，往存在的进程注入一段线程<br />可以用这个脚本去检测 .\Get-InjectedThreads.ps1   powershell需要启动管理员权限<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666628586139-68a3a4e9-027a-45ed-a6fb-b38fe54f7a6d.png#clientId=u2beda34f-89c0-4&from=paste&height=658&id=u2f7dc289&originHeight=823&originWidth=889&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94209&status=done&style=none&taskId=u953e9f52-c6f3-4287-bb0c-73ba075fe33&title=&width=711.2)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666659483593-3fff8d20-4ec5-4fbb-bc3f-eedc3d800710.png#clientId=u673d0265-0e57-4&from=paste&height=424&id=u2e1b3ec3&originHeight=530&originWidth=843&originalType=binary&ratio=1&rotation=0&showTitle=false&size=61578&status=done&style=none&taskId=u46efbc81-2649-4bb1-bcf9-00c7307e44a&title=&width=674.4)
<a name="qeI1k"></a>
## dll劫持检测
通过appnetworkcounter定位到奇怪的服务联网<br />打开资源管理器<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666454340581-a56d29f7-7725-4ed5-a9f3-071f80d3e408.png#clientId=u49fa7eb6-5dbb-4&from=paste&height=536&id=uc05f8294&originHeight=670&originWidth=834&originalType=binary&ratio=1&rotation=0&showTitle=false&size=78745&status=done&style=none&taskId=u06473711-f75f-4f69-9f73-2c821fde443&title=&width=667.2)

把威胁服务的exe勾选，然后把所有dll复制出来<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666454309151-4eb5fa16-f11f-4f03-81b8-b32e5348fe9d.png#clientId=u49fa7eb6-5dbb-4&from=paste&height=823&id=u30fb70dc&originHeight=1029&originWidth=1652&originalType=binary&ratio=1&rotation=0&showTitle=false&size=368435&status=done&style=none&taskId=uc702224b-d0f4-4644-a815-defcc0cca7a&title=&width=1321.6)

通过notepad的操作批量生成dll的md5，但是需要同版本情况下进行对比<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1666405062838-a1193687-fb34-4730-bad9-ceef4f207fb0.png#clientId=ue03fc5c8-1962-4&from=paste&height=636&id=u84c62c95&originHeight=795&originWidth=1748&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154798&status=done&style=none&taskId=uc2c48193-854a-4019-8fc4-0efa6167a3d&title=&width=1398.4)

方法二：<br />对外联的进程排查，进去后查看Memory哪个存在RWX，可读可写可执行的内存，然后找到对应的地址<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1669194947181-11f9ae1e-4e77-4125-96ae-62b891fcc008.png#clientId=u4785306e-6c0c-4&from=paste&height=602&id=u622e3552&originHeight=753&originWidth=900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63064&status=done&style=none&taskId=ue915010b-16db-483d-b3ae-9e1f2118426&title=&width=720)

根据对应的地址然后找到注入的dll文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1669195196577-ed77eeb7-184b-4b37-891a-94e738b67b85.png#clientId=u4785306e-6c0c-4&from=paste&height=602&id=ud66e7bd5&originHeight=753&originWidth=900&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64883&status=done&style=none&taskId=u4701974c-e199-4365-8140-ec7fc646e7c&title=&width=720)
