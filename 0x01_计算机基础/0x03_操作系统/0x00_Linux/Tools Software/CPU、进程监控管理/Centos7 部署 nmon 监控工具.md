Linux CentOS7 Nmon<br />Nmon是一款计算机性能系统监控工具，因为它免费，体积小，安装简单，耗费资源低，广泛应用于AIX和Linux系统。
<a name="7claJ"></a>
### Nmon官网
[http://nmon.sourceforge.net/pmwiki.php](http://nmon.sourceforge.net/pmwiki.php)
<a name="ZXWH4"></a>
### 下载软件包
[nmon16g_x86.tar.gz](https://www.yuque.com/attachments/yuque/0/2021/gz/396745/1609907567800-890431a2-4565-4e74-95e1-cfb63f5b6693.gz?_lake_card=%7B%22uid%22%3A%221609907566031-0%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fgz%2F396745%2F1609907567800-890431a2-4565-4e74-95e1-cfb63f5b6693.gz%22%2C%22name%22%3A%22nmon16g_x86.tar.gz%22%2C%22size%22%3A692079%2C%22type%22%3A%22application%2Fx-gzip%22%2C%22ext%22%3A%22gz%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22gR5DI%22%2C%22refSrc%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fgz%2F396745%2F1609907567800-890431a2-4565-4e74-95e1-cfb63f5b6693.gz%22%2C%22card%22%3A%22file%22%7D)
```bash
[root@localhost tools]# wget http://sourceforge.net/projects/nmon/files/nmon16d_x86.tar.gz
```
<a name="jARRw"></a>
### 解压
```bash
[root@localhost tools]# tar xf nmon16d_x86.tar.gz
[root@localhost tools]# ls
nmon_power_64le_ubuntu16  nmon_x86_64_linux       nmon_x86_debian7   nmon_x86_macpuppy          nmon_x86_rhel6
nmon_power_64_linux       nmon_x86_64_mint16      nmon_x86_debian8   nmon_x86_mint16            nmon_x86_sles12
nmon16d_x86.tar.gz        nmon_power_64_rhel6       nmon_x86_64_mint17      nmon_x86_fedora17  nmon_x86_mint17            nmon_x86_sles13
nmon_power_32_linux       nmon_power_64_rhel7       nmon_x86_64_opensuse13  nmon_x86_fedora18  nmon_x86_mint7             nmon_x86_tahrpuppy
nmon_power_32_rhel6       nmon_power_64_sles11      nmon_x86_64_rhel6       nmon_x86_fedora19  nmon_x86_mint8             nmon_x86_ubuntu10
nmon_power_32_sles11      nmon_x86_64_centos6       nmon_x86_64_rhel7       nmon_x86_fedora20  nmon_x86_opensuse12        nmon_x86_ubuntu13
nmon_power_64_kvm2        nmon_x86_64_centos7       nmon_x86_64_sles13      nmon_x86_fedora21  nmon_x86_opensuse13        nmon_x86_ubuntu15
nmon_power_64le_fedora22  nmon_x86_64_debian6       nmon_x86_64_ubuntu13    nmon_x86_fedora22  nmon_x86_peppermint4       nmon_x86_ubuntu8
nmon_power_64le_linux     nmon_x86_64_debian8       nmon_x86_64_ubuntu15    nmon_x86_knoppix5  nmon_x86_precisepuppy      nmon_x86_ubuntu9
nmon_power_64le_rhel7     nmon_x86_64_fedora17      nmon_x86_centos6        nmon_x86_knoppix6  nmon_x86_puppy_GNU_2.0.0   nmon_x86_zorin6
nmon_power_64le_ubuntu14  nmon_x86_64_fedora20      nmon_x86_debian5        nmon_x86_knoppix7  nmon_x86_puppy_GNU_2.6.15
nmon_power_64le_ubuntu15  nmon_x86_64_fedora21      nmon_x86_debian6        nmon_x86_linux     nmon_x86_puppy_GNU_2.6.24
```
<a name="AOSSz"></a>
### 选择对应版本重命名并添加可执行权限启动
```bash
[root@localhost tools]# mv nmon16g_x86_rhel72 nmon
[root@localhost tools]# chmod +x nmon
[root@localhost tools]# ./nmon
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609910101461-12e3a83f-feac-4222-beae-0374b6e9db54.png#align=left&display=inline&height=451&originHeight=1352&originWidth=3323&size=1706110&status=done&style=none&width=1107.6666666666667)![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609907553744-fcc9e065-f47f-4eec-b49e-2d92f4b08487.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=423958&status=done&style=none&width=1107.6666666666667)
<a name="db06c78d"></a>
### 使用
<a name="6Pqod"></a>
#### 按c查看cpu信息
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609907509775-5bc77d74-aa59-4d3d-861a-a23ff2b4c5e9.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=295572&status=done&style=none&width=1107.6666666666667)
<a name="A0cuP"></a>
#### 按m 查看内存信息
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609907528698-5185e516-19a3-46da-b95f-805d1cacbb6a.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=363807&status=done&style=none&width=1107.6666666666667)
<a name="gsp6M"></a>
### 使用Nmon采集系统性能并生成报告
<a name="Y75KJ"></a>
#### 1、采集报告
```bash
[root@iZuligp6e1dyzfZ nmon]# ./nmon -s3 -c60 -f -m ./

##########################命令参数说明##########################
-s3  为每3s收集一次性能信息，
-c60  为收集60次，
-f  为生成的文件名包含该文件创建时间，
-m   ./为指定测试报告存储路径为当前目录下
```
:::tips
执行完收集信息的命令后，生成了以主机名+创建年月+创建时间的文件，并且可以看到有个nmon的后台进程，即为收集服务器性能信息的进程，直到60次收集执行完之后，该进程才会结束。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609914071733-526432cc-c8e5-482e-807b-984c6c5c0c89.png#align=left&display=inline&height=218&originHeight=655&originWidth=3323&size=766542&status=done&style=none&width=1107.6666666666667)
:::
<a name="dCoZY"></a>
#### 2、分析生成的报告
下载 nmon analyser分析工具，该工具可以将上面生成的.nmon文件转化为图表，更为直观。<br />下载地址：[https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/Power%20Systems/page/nmon_analyser](https://www.ibm.com/developerworks/community/wikis/home?lang=en#!/wiki/Power%2520Systems/page/nmon_analyser)<br />安装包：[nmon_analyser_v66.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1609914085605-22b89c80-9f65-483d-999a-f575ec284515.zip?_lake_card=%7B%22uid%22%3A%221609914084564-0%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1609914085605-22b89c80-9f65-483d-999a-f575ec284515.zip%22%2C%22name%22%3A%22nmon_analyser_v66.zip%22%2C%22size%22%3A750464%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22eY92R%22%2C%22card%22%3A%22file%22%7D)<br />解压安装包后，里面有一个xlsm文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609914810595-dd804ea9-9f5a-4d54-9547-b71b47ab1a7c.png#align=left&display=inline&height=492&originHeight=1477&originWidth=2018&size=271605&status=done&style=shadow&width=672.6666666666666)<br />打开该文件后可以看到宏工具按钮<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1609914866163-a12e5fb2-a344-4745-9b6f-1db84a221465.png#align=left&display=inline&height=668&originHeight=2005&originWidth=3840&size=742148&status=done&style=shadow&width=1280)<br />然后选择生成的nmon文件生成相应的分析报告。
