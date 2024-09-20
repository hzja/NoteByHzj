Java
<a name="LlOCn"></a>
## 简介
Java io操作是开发中比较常用的技术，但是如果每次都使用原生的IO流来操作会显得比较繁琐。<br />Common IO 是一个工具库，用来帮助开发IO功能， 它包括6个主要部分

- Utility classes – 工具类，包括一些静态方法来执行常用任务
- Input – 输入，`InputStream` 和 `Reader` 实现
- Output – 输出，`OutputStream` 和 `Writer` 实现
- Filters – 过滤器，多种文件过滤器实现(定义了 `IOFileFilter`接口，同时继承了 `FileFilter` 和 `FilenameFilter` 接口)
- `Comparators` – 比较器，用于文件比较的多种`java.util.Comparatot`实现
- File `Monitor`–文件监控 

添加maven依赖：
```xml
<dependency>
    <groupId>commons-io</groupId>
    <artifactId>commons-io</artifactId>
    <version>2.6</version>
</dependency>
```
<a name="Gyo0Z"></a>
## 一、工具类 
IOUtils 包含一些工具类，用于处理读，写和拷贝，这些方法基于 `InputStream`，`OutputStream`，`Reader` 和 `Writer`工作.<br />`FileUtils` 包含一些工具类，它们基于File对象工作，包括读，写，拷贝和比较文件<br />`FilenameUtils` 包含一些工具类，它们基于文件名工作而不是`File`对象。这个类旨在 在Unix和Windows环境下保持一致，帮助在两个环境下过渡(如从开发环境到生成环境)<br />`FileSystemUtils` 包含一些工具类，基于文件系统访问功能不被JDK支持。目前，只有一个方法就是得到驱动器空余空间。注意这使用命令行而不是 native code。<br />`EndianUtils` 包含静态方法来交换Java基本类型和流的字节序<br />`SwappedDataInputStream` 实现了`DataInput`接口。可以从文件中读取非本地字节序。 
<a name="f5V46"></a>
### IOUtils的使用：
```java
IOUtils.copy(InputStream input, OutputStream output) // 此方法有多个重载方法，满足不同的输入输出流
IOUtils.copy(InputStream input, OutputStream output, int bufferSize)
IOUtils.copy(InputStream input, Writer output, String inputEncoding)
IOUtils.copy(Reader input, Writer output)
IOUtils.copy(Reader input, OutputStream output, String outputEncoding)
// 这个方法适合拷贝较大的数据流，比如2G以上
IOUtils.copyLarge(Reader input, Writer output) // 默认会用1024*4的buffer来读取
IOUtils.copyLarge(Reader input, Writer output, char[] buffer)

IOUtils.toInputStream(String input, String encoding) // 通过文本获取输入流 ， 可以指定编码格式
IOUtils.toInputStream(String input, Charset encoding)
IOUtils.toBufferedInputStream(InputStream input) // 获取一个缓冲输入流，默认缓冲大小 1KB
IOUtils.toBufferedInputStream(InputStream input, int size) // 获取一个指定缓冲流的大小的输入流
IOUtils.toBufferedReader(Reader reader) // 获取一个字符缓冲流
IOUtils.toBufferedReader(Reader reader, int size)

// 获取缓冲流
IOUtils.buffer(InputStream inputStream)
IOUtils.buffer(OutputStream outputStream)
IOUtils.buffer(Reader reader)
IOUtils.buffer(Writer writer)

// 将输入流转换成字符串
IOUtils.toString(Reader input)
IOUtils.toString(byte[] input, String encoding)
IOUtils.toString(InputStream input, Charset encoding)
IOUtils.toString(InputStream input, String encoding)
IOUtils.toString(URI uri, String encoding)
IOUtils.toString(URL url, String encoding)

// 将输入流转换成字符数组
IOUtils.toByteArray(InputStream input)
IOUtils.toByteArray(InputStream input, int size)
IOUtils.toByteArray(URI uri)
IOUtils.toByteArray(URL url)
IOUtils.toByteArray(URLConnection urlConn)
IOUtils.toByteArray(Reader input, String encoding)

// 字符串读写
IOUtils.readLines(Reader input)
IOUtils.readLines(InputStream input, Charset encoding)
IOUtils.readLines(InputStream input, String encoding)
IOUtils.writeLines(Collection<?> lines, String lineEnding, Writer writer)
IOUtils.writeLines(Collection<?> lines, String lineEnding, OutputStream output, Charset encoding)
IOUtils.writeLines(Collection<?> lines, String lineEnding, OutputStream output, String encoding)
// 从一个流中读取内容
IOUtils.read(InputStream input, byte[] buffer)
IOUtils.read(InputStream input, byte[] buffer, int offset, int length) IOUtils.read(Reader input, char[] buffer)
IOUtils.read(Reader input, char[] buffer, int offset, int length)
// 把数据写入到输出流中
IOUtils.write(byte[] data, OutputStream output)
IOUtils.write(byte[] data, Writer output, Charset encoding)
IOUtils.write(byte[] data, Writer output, String encoding)
IOUtils.write(char[] data, Writer output)
IOUtils.write(char[] data, OutputStream output, Charset encoding) IOUtils.write(char[] data, OutputStream output, String encoding)
IOUtils.write(String data, Writer output)
IOUtils.write(CharSequence data, Writer output)

// 从一个流中读取内容，如果读取的长度不够，就会抛出异常
IOUtils.readFully(InputStream input, int length)
IOUtils.readFully(InputStream input, byte[] buffer)
IOUtils.readFully(InputStream input, byte[] buffer, int offset, int length) IOUtils.readFully(Reader input, char[] buffer)
IOUtils.readFully(Reader input, char[] buffer, int offset, int length)

IOUtils.contentEquals(InputStream input1, InputStream input2) // 比较两个流是否相等
IOUtils.contentEquals(Reader input1, Reader input2)
IOUtils.contentEqualsIgnoreEOL(Reader input1, Reader input2) // 比较两个流，忽略换行符

IOUtils.skip(InputStream input, long toSkip) // 跳过指定长度的流
IOUtils.skip(Reader input, long toSkip)
IOUtils.skipFully(InputStream input, long toSkip) // 如果忽略的长度大于现有的长度，就会抛出异常
IOUtils.skipFully(Reader input, long toSkip)

// 读取流，返回迭代器
IOUtils.lineIterator(Reader reader)
IOUtils.lineIterator(InputStream input, Charset encoding)
IOUtils.lineIterator(InputStream input, String encoding)

// 关闭流
IOUtils.close(URLConnection conn)
// 其他的关闭方法推荐使用 Closeable.close()

FileUtils的使用：
// 复制文件夹
FileUtils.copyDirectory(File srcDir, File destDir) // 复制文件夹（文件夹里面的文件内容也会复制）
FileUtils.copyDirectory(File srcDir, File destDir, FileFilter filter) // 复制文件夹，带有文件过滤功能
FileUtils.copyDirectoryToDirectory(File srcDir, File destDir) // 以子目录的形式将文件夹复制到到另一个文件夹下

// 复制文件
FileUtils.copyFile(File srcFile, File destFile) // 复制文件
FileUtils.copyFile(File input, OutputStream output) // 复制文件到输出流
FileUtils.copyFileToDirectory(File srcFile, File destDir) // 复制文件到一个指定的目录
FileUtils.copyInputStreamToFile(InputStream source, File destination) // 把输入流里面的内容复制到指定文件
FileUtils.copyURLToFile(URL source, File destination) // 把URL 里面内容复制到文件(可以下载文件)
FileUtils.copyURLToFile(URL source, File destination, int connectionTimeout, int readTimeout)

// 把字符串写入文件
FileUtils.writeStringToFile(File file, String data, String encoding)
FileUtils.writeStringToFile(File file, String data, String encoding, boolean append)
// 把字节数组写入文件
FileUtils.writeByteArrayToFile(File file, byte[] data)
FileUtils.writeByteArrayToFile(File file, byte[] data, boolean append) FileUtils.writeByteArrayToFile(File file, byte[] data, int off, int len) FileUtils.writeByteArrayToFile(File file, byte[] data, int off, int len, boolean append)
// 把集合里面的内容写入文件
// encoding：文件编码，lineEnding：每行以什么结尾
FileUtils.writeLines(File file, Collection<?> lines)
FileUtils.writeLines(File file, Collection<?> lines, boolean append)
FileUtils.writeLines(File file, Collection<?> lines, String lineEnding)
FileUtils.writeLines(File file, Collection<?> lines, String lineEnding, boolean append)
FileUtils.writeLines(File file, String encoding, Collection<?> lines)
FileUtils.writeLines(File file, String encoding, Collection<?> lines, boolean append)
FileUtils.writeLines(File file, String encoding, Collection<?> lines, String lineEnding)
FileUtils.writeLines(File file, String encoding, Collection<?> lines, String lineEnding, boolean append)
// 往文件里面写内容
FileUtils.write(File file, CharSequence data, Charset encoding)
FileUtils.write(File file, CharSequence data, Charset encoding, boolean append)
FileUtils.write(File file, CharSequence data, String encoding)
FileUtils.write(File file, CharSequence data, String encoding, boolean append)

// 文件移动
FileUtils.moveDirectory(File srcDir, File destDir) // 文件夹在内的所有文件都将移动FileUtils.moveDirectoryToDirectory(File src, File destDir, boolean createDestDir) // 以子文件夹的形式移动到另外一个文件下
FileUtils.moveFile(File srcFile, File destFile) // 移动文件
FileUtils.moveFileToDirectory(File srcFile, File destDir, boolean createDestDir) // 以子文件的形式移动到另外一个文件夹下
FileUtils.moveToDirectory(File src, File destDir, boolean createDestDir) // 移动文件或者目录到指定的文件夹内

// 清空和删除文件夹
FileUtils.deleteDirectory(File directory) // 删除文件夹，包括文件夹和文件夹里面所有的文件
FileUtils.cleanDirectory(File directory) // 清空文件夹里面的所有的内容
FileUtils.forceDelete(File file) // 删除，会抛出异常
FileUtils.deleteQuietly(File file) // 删除，不会抛出异常

// 创建文件夹
FileUtils.forceMkdir(File directory) // 创建文件夹(可创建多级)
FileUtils.forceMkdirParent(File file) // 创建文件的父级目录

// 获取文件输入/输出流
FileUtils.openInputStream(File file)
FileUtils.openOutputStream(File file)
// 读取文件
FileUtils.readFileToByteArray(File file) // 把文件读取到字节数组
FileUtils.readFileToString(File file, Charset encoding) // 把文件读取成字符串
FileUtils.readFileToString(File file, String encoding)
FileUtils.readLines(File file, Charset encoding) // 把文件读取成字符串集合
FileUtils.readLines(File file, String encoding)

// 测试两个文件的修改时间
FileUtils.isFileNewer(File file, Date date)
FileUtils.isFileNewer(File file, File reference)
FileUtils.isFileNewer(File file, long timeMillis)
FileUtils.isFileOlder(File file, Date date)
FileUtils.isFileOlder(File file, File reference)
FileUtils.isFileOlder(File file, long timeMillis)
 
// 文件/文件夹的迭代
FileUtils.iterateFiles(File directory, IOFileFilter fileFilter, IOFileFilter dirFilter)
FileUtils.iterateFiles(File directory, String[] extensions, boolean recursive)
FileUtils.iterateFilesAndDirs(File directory, IOFileFilter fileFilter, IOFileFilter dirFilter)
FileUtils.lineIterator(File file)
FileUtils.lineIterator(File file, String encoding)

FileUtils.listFiles(File directory, IOFileFilter fileFilter, IOFileFilter dirFilter)
FileUtils.listFiles(File directory, String[] extensions, boolean recursive)
FileUtils.listFilesAndDirs(File directory, IOFileFilter fileFilter, IOFileFilter dirFilter)

// 其他
FileUtils.isSymlink(File file) // 判断是否是符号链接
FileUtils.directoryContains(File directory, File child) // 判断文件夹内是否包含某个文件或者文件夹
FileUtils.sizeOf(File file) // 获取文件或者文件夹的大小
FileUtils.getTempDirectory()// 获取临时目录文件
FileUtils.getTempDirectoryPath()// 获取临时目录路径
FileUtils.getUserDirectory()// 获取用户目录文件
FileUtils.getUserDirectoryPath()// 获取用户目录路径
FileUtils.touch(File file) // 创建文件
FileUtils.contentEquals(File file1, File file2) // 比较两个文件内容是否相同
```
<a name="pWG02"></a>
### `FilenameUtils`的使用
```java
FilenameUtils.concat(String basePath, String fullFilenameToAdd) // 合并目录和文件名为文件全路径
FilenameUtils.getBaseName(String filename) // 去除目录和后缀后的文件名
FilenameUtils.getExtension(String filename) // 获取文件的后缀
FilenameUtils.getFullPath(String filename) // 获取文件的目录
FilenameUtils.getName(String filename) // 获取文件名
FilenameUtils.getPath(String filename) // 去除盘符后的路径
FilenameUtils.getPrefix(String filename) // 盘符
FilenameUtils.indexOfExtension(String filename) // 获取最后一个.的位置
FilenameUtils.indexOfLastSeparator(String filename) // 获取最后一个/的位置
FilenameUtils.normalize(String filename) // 获取当前系统格式化路径
FilenameUtils.removeExtension(String filename) // 移除文件的扩展名
FilenameUtils.separatorsToSystem(String path) // 转换分隔符为当前系统分隔符
FilenameUtils.separatorsToUnix(String path) // 转换分隔符为linux系统分隔符
FilenameUtils.separatorsToWindows(String path) // 转换分隔符为windows系统分隔符
FilenameUtils.equals(String filename1, String filename2) // 判断文件路径是否相同，非格式化
FilenameUtils.equalsNormalized(String filename1, String filename2) // 判断文件路径是否相同，格式化
FilenameUtils.directoryContains(String canonicalParent, String canonicalChild) // 判断目录下是否包含指定文件或目录
FilenameUtils.isExtension(String filename, String extension) // 判断文件扩展名是否包含在指定集合(数组、字符串)中
FilenameUtils.wildcardMatch(String filename, String wildcardMatcher) // 判断文件扩展名是否和指定规则匹配
```
<a name="paG7w"></a>
### `FileSystemUtils`的使用
```java
// 仅有这一个方法，及其重载方法
FileSystemUtils.freeSpaceKb() throws IOException
//推荐使用
java.nio.file.FileStore.getUsableSpace() 
```
<a name="sx3fl"></a>
## 二、输入、输出
在org.apache.commons.io.input包下有许多`InputStrem`类的实现，来测试一个最实用的类，`TeeInputStream`，将`InputStream`以及`OutputStream`作为参数传入其中，自动实现将输入流的数据读取到输出流中。
```java
@Test
public void test() throws IOException {
    // XmlStreamReader
    File xml = new File("E:\\test\\data\\io\\test.xml");
    XmlStreamReader xmlReader = new XmlStreamReader(xml);
    System.out.println("XML encoding: " + xmlReader.getEncoding());
    String INPUT = "This should go to the output.";
    // TeeInputStream
    ByteArrayInputStream in = new ByteArrayInputStream(INPUT.getBytes("US-ASCII"));
    ByteArrayOutputStream out = new ByteArrayOutputStream();
    TeeInputStream tee = new TeeInputStream(in, out, true);
    tee.read(new byte[INPUT.length()]);
    System.out.println("Output stream: " + out.toString());
}
```
同样，在 org.apache.commons.io.output包中同样有`OutputStream`类的实现，这里介绍`TeeOutputStream`，它可以将输出流进行分流，换句话说可以用一个输入流将数据分别读入到两个不同的输出流。 
```java
@Test
public void test8() throws IOException {
    String INPUT = "This should go to the output.";
    // TeeOutputStream
    ByteArrayInputStream in = new ByteArrayInputStream(INPUT.getBytes("US-ASCII"));
    ByteArrayOutputStream out1 = new ByteArrayOutputStream();
    ByteArrayOutputStream out2 = new ByteArrayOutputStream();
    TeeOutputStream teeOut = new TeeOutputStream(out1, out2);
    TeeInputStream teeIn = new TeeInputStream(in, teeOut, true);
    teeIn.read(new byte[INPUT.length()]);
    System.out.println("Output stream 1: " + out1.toString());
    System.out.println("Output stream 2: " + out2.toString());
}
```
<a name="uAEuS"></a>
## 三、`Filters`过滤器
<a name="RE5aT"></a>
### 1. 基本功能过滤器 
<a name="XEWZ7"></a>
#### 类型：
FileFileFilter       仅接受文件<br />DirectoryFilter          仅接受目录
<a name="FC5JM"></a>
#### 名称：
PrefixFileFilter          基于前缀（不带路径的文件名)<br />SuffixFileFilter          基于后缀(不带路径的文件名)<br />NameFileFilter         基于文件名称(不带路径的文件名)<br />WildcardFileFilter    基于通配符(不带路径的文件名)<br />RegexFileFilter         基于正则表达式
<a name="NkUFI"></a>
#### 时间：
AgeFileFilter                    基于最后修改时间<br />MagicNumberFileFileter   基于Magic Number
<a name="TNTMd"></a>
#### 大小：
EmptyFileFilter         基于文件或目录是否为空<br />SizeFileFilter           基于文件尺寸
<a name="kgkAc"></a>
#### 隐藏属性：
HiddenFileFilter    基于文件或目录是否隐藏
<a name="cLcZP"></a>
#### 读写属性：
CanReadFileFilter       基于是否可读<br />CanWriteFileFilter      基于是否可写入<br />DelegateFileFilter       将普通的`FileFilter`和`FilenameFilter`包装成`IOFileFilter`
<a name="fOqa0"></a>
### 2. 逻辑关系过滤器
AndFileFilter       基于AND逻辑运算<br />OrFileFilter        基于OR逻辑运算<br />NotFileFilter       基于NOT逻辑运算<br />TrueFileFilter           不进行过滤<br />FalseFileFilter          过滤所有文件及目录<br />工具类：FileFilterUtils     <br />提供一些工厂方法用于生成各类文件过滤器<br />提供一些静态方法用于对指定的File集合进行过滤
```java
FileFilterUtils.ageFileFilter(Date cutoffDate)
FileFilterUtils.and(IOFileFilter... filters)
FileFilterUtils.asFileFilter(FileFilter filter)
FileFilterUtils.directoryFileFilter()
FileFilterUtils.falseFileFilter()
FileFilterUtils.fileFileFilter()
FileFilterUtils.filter(IOFileFilter filter, File... files)
FileFilterUtils.filterList(IOFileFilter filter, File... files)
FileFilterUtils.filterSet(IOFileFilter filter, File... files)
FileFilterUtils.nameFileFilter(String name)
FileFilterUtils.notFileFilter(IOFileFilter filter)
FileFilterUtils.or(IOFileFilter... filters)
FileFilterUtils.prefixFileFilter(String prefix)
FileFilterUtils.sizeFileFilter(long threshold)
FileFilterUtils.suffixFileFilter(String suffix)
FileFilterUtils.trueFileFilter()
```
```java
@Test
public void test() throws IOException {
    String PARENT_DIR = "E:\\test\\data\\io\\filter";
    File dir = new File(PARENT_DIR);
    String[] acceptedNames = { "test", "testTxt.txt" };
    // 匹配文件名
    for (String file : dir.list(new NameFileFilter(acceptedNames, IOCase.INSENSITIVE))) {
        System.out.println("File found, named: " + file);
    }
    System.out.println("=========================================");
    // 根据通配符匹配
    for (String file : dir.list(new WildcardFileFilter("*est*"))) {
        System.out.println("Wildcard file found, named: " + file);
    }
    System.out.println("=========================================");
    // 匹配前缀
    for (String file : dir.list(new PrefixFileFilter("test"))) {
        System.out.println("Prefix file found, named: " + file);
    }
    System.out.println("=========================================");
    // 匹配后缀
    for (String file : dir.list(new SuffixFileFilter(".txt"))) {
        System.out.println("Suffix file found, named: " + file);
    }
    System.out.println("=========================================");
    // 逻辑或
    for (String file : dir
            .list(new OrFileFilter(new WildcardFileFilter("*est*"), new SuffixFileFilter(".txt")))) {
        System.out.println("Or file found, named: " + file);
    }
    System.out.println("=========================================");
    // 逻辑与
    for (String file : dir.list(new AndFileFilter(new WildcardFileFilter("*est*"),
            new NotFileFilter(new SuffixFileFilter(".txt"))))) {
        System.out.println("And/Not file found, named: " + file);
    }
}
```
<a name="svedp"></a>
## 四、Comparators比较器
org.apache.commons.io.comparator包下只有这四个类：<br />`CompositeFileComparator`<br />`DefaultFileComparator`<br />`DirectoryFileComparator`<br />`ExtensionFileComparator`<br />`LastModifiedFileComparator`<br />`NameFileComparator`<br />`PathFileComparator`<br />`SizeFileComparator`<br />都有着四个方法：
```java
compare(File file1, File file2)
sort(File... files)
sort(List<File> files)
toString() 
```
```java
@Test
public void test() {
    String dir = "E:\\test\\data\\io\\comparator";
    // NameFileComparator 按名称
    File dirFile = new File(dir);
    NameFileComparator comparator = new NameFileComparator(IOCase.SENSITIVE);
    File[] files = comparator.sort(dirFile.listFiles());
    System.out.println("\nSorted by name files in parent directory: ");
    for (File f : files) {
        System.out.println("t" + f.getAbsolutePath());
    }

    // SizeFileComparator 按大小
    SizeFileComparator sizeComparator = new SizeFileComparator(true);
    File[] sizeFiles = sizeComparator.sort(dirFile.listFiles());
    System.out.println("\nSorted by size files in parent directory: ");
    for (File f : sizeFiles) {
        System.out.println(f.getName() + " with size (kb): " + f.length());
    }

    // LastModifiedFileComparator 按修改时间
    LastModifiedFileComparator lastModified = new LastModifiedFileComparator();
    File[] lastModifiedFiles = lastModified.sort(dirFile.listFiles());
    System.out.println("\nSorted by last modified files in parent directory: ");
    for (File f : lastModifiedFiles) {
        Date modified = new Date(f.lastModified());
        System.out.println(f.getName() + " last modified on: " + modified);
    }
}
```
<a name="NfnVk"></a>
## 五、Monitor文件监控
原理：<br />由文件监控类`FileAlterationMonitor`中的线程按指定的间隔不停的扫描文件观察器`FileAlterationObserver`，如果有文件的变化，则根据相关的文件比较器，判断文件时新增，还是删除，还是更改。（默认为1000毫秒执行一次扫描） <br />使用方法：<br />1、创建一个`FileAlterationObserver`对象，传入一个要监控的目录，这个对象会观察这些变化。<br />2、通过调用`addListener()`方法，为`observer`对象添加一个 `FileAlterationListener`对象。你可以通过很多种方式来创建，继承适配器类或者实现接口或者使用匿名内部类，实现所需要的监控方法。<br />3、创建一个`FileAlterationMonitor`对象，将已经创建好的`observer`对象添加其中并且传入时间间隔参数（单位是毫秒）。<br />4、调用`start()`方法即可开启监视器，如果你想停止监视器，调用`stop()`方法即可。
```java
// 继承FileAlterationListenerAdaptor适配器类或者是实现FileAlterationListener接口
class MyFileListener extends FileAlterationListenerAdaptor {
    private Log log = LogFactory.getLog(MyFileListener.class);
    // 文件创建
    @Override
    public void onFileCreate(File file) {
        log.info("[新建]:" + file.getAbsolutePath());
    }
    // 文件修改
    @Override
    public void onFileChange(File file) {
        log.info("[修改]:" + file.getAbsolutePath());
    }
    // 文件删除
    @Override
    public void onFileDelete(File file) {
        log.info("[删除]:" + file.getAbsolutePath());
    }
    // 目录创建
    @Override
    public void onDirectoryCreate(File directory) {
        log.info("[新建]:" + directory.getAbsolutePath());
    }
    // 目录修改
    @Override
    public void onDirectoryChange(File directory) {

        log.info("[修改]:" + directory.getAbsolutePath());

    }
    // 目录删除
    @Override
    public void onDirectoryDelete(File directory) {
        log.info("[删除]:" + directory.getAbsolutePath());

    }
    @Override
    public void onStart(FileAlterationObserver observer) {
        super.onStart(observer);
    }
    @Override
    public void onStop(FileAlterationObserver observer) {
        super.onStop(observer);
    }
}


public static void main(String[] args) throws Exception {
    String rootDir = "E:\\test\\data\\io\\monitor";
    // 创建一个文件观察器用于处理文件的格式
    // FileAlterationObserver _observer = new FileAlterationObserver(rootDir,
    // FileFilterUtils.and(FileFilterUtils.fileFileFilter(),
    // FileFilterUtils.suffixFileFilter(".txt")), null);// 过滤文件格式
    FileAlterationObserver observer = new FileAlterationObserver(rootDir);
    observer.addListener(new MyFileListener()); // 设置文件变化监听器
    FileAlterationMonitor monitor = new FileAlterationMonitor(5000, observer);// 创建文件变化监听器,间隔5秒
    monitor.start();// 开始监控
}
```
