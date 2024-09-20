JavaSpringBoot
<a name="LPH79"></a>
## 问题说明
SpringBoot项目中用到如下代码读取自定义配置：
```java
String path = Thread.currentThread()
    .getContextClassLoader()
    .getResource("fsconfig.properties")
    .getPath();
```
<a name="Yn5xI"></a>
### 如果不加`-Xbootclasspath/a`启动
```bash
java -jar com-olive-0.0.1-SNAPSHOT.jar
```
读取的path路径是jar内的：
```
file:/C:/Users/2230/Desktop/a/com-olive-0.0.1-SNAPSHOT.jar!/BOOT-INF/classes!/jdbc.properties
```
<a name="CeB3o"></a>
### 如果加`-Xbootclasspath/a`启动
```
java -Xbootclasspath/a:C:\Users\2230\Desktop\a\config -jar com-olive-0.0.1-SNAPSHOT.jar
```
读取的path路径是jar外的：
```
/C:/Users/2230/Desktop/a/config/jdbc.properties
```
<a name="AiCzo"></a>
## 解决方案说明
<a name="g0b3O"></a>
### BootStrap class扩展方案
Java 命令行提供了如何扩展 bootStrap 级别 class 的简单方法。

- `-Xbootclasspath`：完全取代基本核心的Java class 搜索路径。不常用，否则要重新写所有Java核心class
- `-Xbootclasspath/a`：后缀在核心class搜索路径后面。常用!!
- `-Xbootclasspath/p`：前缀在核心class搜索路径前面。不常用，避免引起不必要的冲突。

**语法如下：**<br />分隔符与classpath参数类似，unix使用`:`号，windows使用`;`号，这里以unix为例
```bash
java -Xbootclasspath/a:/usrhome/thirdlib.jar: -jar yourJarExe.jar
```
<a name="PpvgO"></a>
### extend class扩展方案
Java exten class 存放在{Java_home}\jre\lib\ext目录下。当调用Java时，对扩展class路径的搜索是自动的。总会搜索的。这样，解决的方案就很简单了，将所有要使用的第三方的jar包都复制到ext目录下。
<a name="LNBvi"></a>
### User class扩展方案
当使用-jar执行可执行Jar包时，JVM将Jar包所在目录设置为codebase目录，所有的class搜索都在这个目录下开始.所以如果使用了其他第三方的jar包，一个比较可以接受的可配置方案，就是利用jar包的Manifest扩展机制。<br />**步骤如下：**

1. 将需要的第三方的jar包，复制在同可执行jar所在的目录或某个子目录下。比如：jar包在/usrhome/yourJarExe.jar 那么可以把所有jar包复制到/usrhome目录下或/usrhome/lib等类似的子目录下。
2. 修改Manifest文件

在Manifest.mf文件里加入如下行
```
Class-Path:classes12.jar lib/thirdlib.jar
```
Class-Path 是可执行jar包运行依赖的关键词。详细内容可以参考<br />[https://docs.oracle.com/cd/E15289_01/JRCLR/optionx.htm](https://docs.oracle.com/cd/E15289_01/JRCLR/optionx.htm) <br />或者[https://docs.oracle.com/javase/8/docs/technotes/tools/findingclasses.html](https://docs.oracle.com/javase/8/docs/technotes/tools/findingclasses.html)<br />要注意的是 Class-Path 只是作为本地机器的CLASSPATH环境变量的一个缩写，也就是说用这个前缀表示在jar包执行机器上所有的CLASSPATH目录下寻找相应的第三方类/类库。并不能通过 Class-Path 来加载位于本身的jar包里面（或者网络上）的jar文件。因为从理论上来讲，jar发布包不应该再去包含其他的第三方类库（而应该通过使用说明来提醒用户去获取相应的支持类库）。如果由于特殊需要必须把其他的第三方类库（jar, zip, class等）直接打包在自己的jar包里面一起发布，就必须通过实现自定义的ClassLoader来按照自己的意图加载这些第三方类库。<br />以上三种方法推荐第一种，扩展性好，操作起来也最方便。<br />Java的安全机制随不同的JDK版本有不同的变化，会影响很多核心CLASS，比如Thread，所以很多大型商业软件，要求JDK的版本很严格。部分原因也在此。这也要求在发布自己编写的应用时候，不管大小，都要说明开发和测试的JDK版本。
