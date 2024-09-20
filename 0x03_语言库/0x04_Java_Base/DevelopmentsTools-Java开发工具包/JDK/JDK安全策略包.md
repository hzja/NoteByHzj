JDK<br />JCE是java加密扩展包，由于美国出口限制规定，JCE对部分国家是限制出口的，致使其加密长度有所缩减。默认支持AES128，如果要使用AES256，还需要替换一下两个文件。请按照如下步骤进行替换。<br />用无限制扩展包替换jdk安装目录下的local_policy.jar和US_export_policy.jar<br />例如：<br />jdk安装目录下: <br />C:\Program Files\Java\jdk1.8.0_31<br />替换文件所在的目录：<br />C:\Program Files\Java\jdk1.8.0_31\jre\lib\security
<a name="Y42nY"></a>
## jdk8替换包下载地址
[http://www.oracle.com/technetwork/java/javase/downloads/jce8-download-2133166.html](http://www.oracle.com/technetwork/java/javase/downloads/jce8-download-2133166.html)<br />JDK替换包已下载好的文件：[jce_policy-8.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1618328156052-040bfefd-1469-454d-80da-675aa085270e.zip?_lake_card=%7B%22uid%22%3A%221618328155636-0%22%2C%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1618328156052-040bfefd-1469-454d-80da-675aa085270e.zip%22%2C%22name%22%3A%22jce_policy-8.zip%22%2C%22size%22%3A12417%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22status%22%3A%22done%22%2C%22percent%22%3A0%2C%22id%22%3A%22K5uwP%22%2C%22card%22%3A%22file%22%7D)
<a name="4hG8f"></a>
## jdk7替换包下载地址
[http://www.oracle.com/technetwork/java/embedded/embedded-se/downloads/jce-7-download-432124.html](http://www.oracle.com/technetwork/java/embedded/embedded-se/downloads/jce-7-download-432124.html)
<a name="mGjdS"></a>
## jdk6替换包下载地址
[http://www.oracle.com/technetwork/java/embedded/embedded-se/downloads/jce-6-download-429243.html](http://www.oracle.com/technetwork/java/embedded/embedded-se/downloads/jce-6-download-429243.html)<br />下载完后，可看到这两个jar包：local_policy.jar和US_export_policy.jar<br />替换 `%JRE_HOME%\lib\security` 下这两个jar，如果装了JDK，也替换这个 `%JDK_HOME%\jre\lib\security` 目录下的这两个jar。
