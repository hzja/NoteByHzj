JavaJVisualVM
<a name="RUWaK"></a>
## JVisualVM 简介
VisualVM 是Netbeans的profile子项目，已在JDK6.0 update 7 中自带，能够监控线程，内存情况，查看方法的CPU时间和内存中的对 象，已被GC的对象，反向查看分配的堆栈(如100个String对象分别由哪几个对象分配出来的)。在JDK_HOME/bin(默认是C:\Program Files\Java\jdk1.6.0_13\bin)目录下面，有一个jvisualvm.exe文件，双击打开，从UI上来看，这个软件是基于NetBeans开发的了。<br />VisualVM 提供了一个可视界面，用于查看 Java 虚拟机上运行的基于 Java 技术的应用程序的详细信息。VisualVM 对 Java Development Kit (JDK) 工具所检索的 JVM 软件相关数据进行组织，并通过一种使您可以快速查看有关多个 Java 应用程序的数据的方式提供该信息。<br />您可以查看本地应用程序或远程主机上运行的应用程序的相关数据。此外，还可以捕获有关 JVM 软件实例的数据，并将该数据保存到本地系统，以供后期查看或与其他用户共享。<br />双击启动 jvisualvm.exe，启动起来后和jconsole 一样同样可以选择本地和远程，如果需要监控远程同样需要配置相关参数。
<a name="IaRbW"></a>
### 主界面如下
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244186-4d4787e7-4a13-428e-802b-625f9c0293c1.png#clientId=ueb3395c4-a712-4&from=paste&id=ubb97553c&originHeight=621&originWidth=1021&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uabcc5ee5-2147-4bcc-a206-5331b26a7fa&title=)VisualVM可以根据需要安装不同的插件，每个插件的关注点都不同，有的主要监控GC，有的主要监控内存，有的监控线程等。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244207-77337c36-3793-4153-9194-51884229d8e3.png#clientId=ueb3395c4-a712-4&from=paste&id=u34c7abcf&originHeight=559&originWidth=911&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf2f41592-9aae-4979-9671-9057c947b3b&title=)
<a name="fJPso"></a>
### 插件安装

- **从主菜单中选择“工具”>“插件”。**
- **在“可用插件”标签中，选中该插件的“安装”复选框。单击“安装”。**
- **逐步完成插件安装程序。**

这里以 Eclipse(pid 22296)为例，双击后直接展开，主界面展示了系统和jvm两大块内容，点击右下方jvm参数和系统属性可以参考详细的参数信息.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244262-bf2d65fe-821d-4478-b190-71a04d84b311.png#clientId=ueb3395c4-a712-4&from=paste&id=u45e472c2&originHeight=624&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u81f1843c-4df6-4d82-ae6b-18832896e08&title=)因为VisualVM的插件太多，这里主要介绍三个主要使用的内容：监控、线程、Visual GC<br />监控的主页其实也就是，cpu、内存、类、线程的图表<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244271-96440558-be76-486c-bcb3-6f5fdd1b28c2.png#clientId=ueb3395c4-a712-4&from=paste&id=u2722d59d&originHeight=582&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e4da43f-84c8-4d31-86d6-b44126efbe7&title=)线程和jconsole功能没有太大的区别<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244199-de79203e-59ee-401f-a826-6537bad8552a.png#clientId=ueb3395c4-a712-4&from=paste&id=u10b88835&originHeight=582&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7c588fd2-cf41-4fa1-8647-fa454f60883&title=)Visual GC 是常常使用的一个功能，可以明显的看到年轻代、老年代的内存变化，以及gc频率、gc的时间等。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244527-2f0ef220-09a7-47f5-8137-57ce8768e816.png#clientId=ueb3395c4-a712-4&from=paste&id=uc8281708&originHeight=624&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44275972-f015-4627-961f-5e0fceb4e73&title=)以上的功能其实jconsole几乎也有，VisualVM更全面更直观一些，另外VisualVM非常多的其它功能，可以分析dump的内存快照，<br />dump出来的线程快照并且进行分析等，还有其它很多的插件大家可以去探索<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244596-2bc1666c-cc9c-45c6-97ff-f55ee42625e5.png#clientId=ueb3395c4-a712-4&from=paste&id=u6781b0e8&originHeight=471&originWidth=1059&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u64355497-b4fe-4699-91aa-63bc894f118&title=)
<a name="yqHvb"></a>
## 案例分析
<a name="AXFTA"></a>
### 准备模拟内存泄漏样例
<a name="jlUjC"></a>
#### 1、定义静态变量HashMap
<a name="giGOO"></a>
#### 2、分段循环创建对象，并加入HashMap
代码如下：
```java
import java.util.HashMap;
import java.util.Map;
public class CyclicDependencies {
	//声明缓存对象
	private static final Map map = new HashMap();
	public static void main(String args[]){
		try {
			Thread.sleep(10000);//给打开visualvm时间
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//循环添加对象到缓存
		for(int i=0; i<1000000;i++){
			TestMemory t = new TestMemory();
			map.put("key"+i,t);
		}
		System.out.println("first");
		//为dump出堆提供时间
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		for(int i=0; i<1000000;i++){
			TestMemory t = new TestMemory();
			map.put("key"+i,t);
		}
		System.out.println("second");
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		for(int i=0; i<3000000;i++){
			TestMemory t = new TestMemory();
			map.put("key"+i,t);
		}
		System.out.println("third");
		try {
			Thread.sleep(10000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		for(int i=0; i<4000000;i++){
			TestMemory t = new TestMemory();
			map.put("key"+i,t);
		}
		System.out.println("forth");
		try {
			Thread.sleep(Integer.MAX_VALUE);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println("qqqq");
	}
}
```
<a name="lmvLR"></a>
#### 3、配置jvm参数如下：
```bash
-Xms512m
-Xmx512m
-XX:-UseGCOverheadLimit
-XX:MaxPermSize=50m
```
<a name="bRHN2"></a>
#### 4、运行程序并打卡visualvm监控
<a name="o7bi5"></a>
### **使用JVisualVM分析内存泄漏**
<a name="aS1YA"></a>
#### 1、查看Visual GC标签，内容如下，这是输出first的截图
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244652-2b7eb6ba-c5e9-421f-b291-9ef8144cf197.png#clientId=ueb3395c4-a712-4&from=paste&id=uecefe4ff&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub96b3ce4-5659-4f96-8f59-7138bffe5a8&title=)<br />这是输出forth的截图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244716-adece5b5-b374-415c-bf3a-53a4d2b828aa.png#clientId=ueb3395c4-a712-4&from=paste&id=udcebf6a1&originHeight=582&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub73055dd-4e93-48d1-8259-26a8e5b044e&title=)<br />通过2张图对比发现：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244654-05adfeba-ac9d-41e4-967b-b33533668caf.png#clientId=ueb3395c4-a712-4&from=paste&id=u1b69edc3&originHeight=82&originWidth=398&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9d73a5f6-34eb-47e8-808c-73393e11fab&title=)<br />老生代一直在gc，当程序继续运行可以发现老生代gc还在继续：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042244874-e7887f5a-fee8-4703-8877-85a124aac8e1.png#clientId=ueb3395c4-a712-4&from=paste&id=u3692dfc1&originHeight=23&originWidth=446&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ufaa697a2-a249-4553-8a5c-34ee0091e62&title=)<br />增加到了7次，但是老生代的内存并没有减少。说明存在无法被回收的对象，可能是内存泄漏了。<br />如何分析是那个对象泄漏了呢？打开抽样器标签：点击后如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245008-9ad1b512-e1a3-4922-a4d7-cc78078b2c57.png#clientId=ueb3395c4-a712-4&from=paste&id=u96d0f6b2&originHeight=543&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u552c01d4-93c5-49f5-bfcf-d654f6490f9&title=)按照程序输出进行堆dump，当输出second时，dump一次，当输出forth时dump一次。<br />进入最后dump出来的堆标签，点击类：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245046-38de8cc4-e359-4886-9fdd-18dfa460dd0b.png#clientId=ueb3395c4-a712-4&from=paste&id=ufecebece&originHeight=44&originWidth=333&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2649259d-1865-4721-9360-b70e4152014&title=)<br />点击右上角：“与另一个堆存储对比”。如图选择第一次导出的dump内容比较：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245012-6901619b-cc43-4043-9877-fa306d761f5f.png#clientId=ueb3395c4-a712-4&from=paste&id=u8e132037&originHeight=324&originWidth=380&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua755e239-29b4-4a9a-9de6-f65b32ccde7&title=)比较结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245165-45f764aa-b6a0-47b1-9fe7-81149c497716.png#clientId=ueb3395c4-a712-4&from=paste&id=ud33475dc&originHeight=129&originWidth=666&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6aac961e-802f-4b54-bf88-7bedcd7fc91&title=)<br />可以看出在两次间隔时间内TestMemory对象实例一直在增加并且多了，说明该对象引用的方法可能存在内存泄漏。
<a name="OnD3J"></a>
### **如何查看对象引用关系呢？**
右键选择类TestMemory，选择“在实例视图中显示”，如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245177-7ded602b-6620-45f7-9702-eb936b740abd.png#clientId=ueb3395c4-a712-4&from=paste&id=ub751a1e1&originHeight=480&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u84cb09af-3dcd-41e5-8bdc-6f83e07be1f&title=)左侧是创建的实例总数，右侧上部为该实例的结构，下面为引用说明，从图中可以看出在类CyclicDependencies里面被引用了，并且被HashMap引用。<br />如此可以确定泄漏的位置，进而根据实际情况进行分析解决。
<a name="DnsxY"></a>
### **JVisualVM 远程监控 Tomcat**
<a name="JAnCJ"></a>
#### 1、修改远程tomcat的catalina.sh配置文件，在其中增加：
```bash
JAVA_OPTS="$JAVA_OPTS

-Djava.rmi.server.hostname=192.168.122.128
-Dcom.sun.management.jmxremote.port=18999
-Dcom.sun.management.jmxremote.ssl=false
-Dcom.sun.management.jmxremote.authenticate=false"
```
这次配置先不走权限校验。只是打开jmx端口。
<a name="d3kJq"></a>
#### 2、打开jvisualvm，右键远程，选择添加远程主机：
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245208-2a845bb2-c890-4ec1-b500-42b1d9328698.png#clientId=ueb3395c4-a712-4&from=paste&id=u06b0492f&originHeight=160&originWidth=378&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u109ff79b-7cea-4aef-80f5-034ee976ddd&title=)
<a name="hn29u"></a>
#### 3、输入主机的名称，直接写ip，如下：
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658042245306-21f0c660-485f-447e-9d4f-320d8fbb5d70.png#clientId=ueb3395c4-a712-4&from=paste&id=u9aa933ac&originHeight=40&originWidth=220&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u49326d4e-0612-4dfb-9ca0-9f1499752e5&title=)<br />右键新建的主机，选择添加JMX连接，输入在tomcat中配置的端口即可。
<a name="qb6sF"></a>
#### 4、双击打开。完毕！
