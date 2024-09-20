Java
<a name="Yg3UN"></a>
## 背景
在某些业务场景下，需要自己实现文件内容变更监听的功能，比如：监听某个文件是否发生变更，当变更时重新加载文件的内容。<br />看似比较简单的一个功能，但如果在某些JDK版本下，可能会出现意想不到的Bug。<br />本篇文章就带大家简单实现一个对应的功能，并分析一下对应的Bug和优缺点。
<a name="f58Bj"></a>
## 初步实现思路
监听文件变动并读取文件，简单的思路如下：

- 单起一个线程，定时获取文件最后更新的时间戳（单位：毫秒）；
- 对比上一次的时间戳，如果不一致，则说明文件被改动，则重新进行加载；

这里写一个简单功能实现（不包含定时任务部分）的demo：
```java
public class FileWatchDemo {

	/**
  * 上次更新时间
  */
	public static long LAST_TIME = 0L;

	public static void main(String[] args) throws IOException {

		String fileName = "/Users/zzs/temp/1.txt";
		// 创建文件，仅为实例，实践中由其他程序触发文件的变更
		createFile(fileName);

		// 执行2次
		for (int i = 0; i < 2; i++) {
			long timestamp = readLastModified(fileName);
			if (timestamp != LAST_TIME) {
				System.out.println("文件已被更新：" + timestamp);
				LAST_TIME = timestamp;
				// 重新加载，文件内容
			} else {
				System.out.println("文件未更新");
			}
		}
	}

	public static void createFile(String fileName) throws IOException {
		File file = new File(fileName);
		if (!file.exists()) {
			boolean result = file.createNewFile();
			System.out.println("创建文件：" + result);
		}
	}

	public static long readLastModified(String fileName) {
		File file = new File(fileName);
		return file.lastModified();
	}
}
```
在上述代码中，先创建一个文件（方便测试），然后两次读取文件的修改时间，并用LAST_TIME记录上次修改时间。如果文件的最新更改时间与上一次不一致，则更新修改时间，并进行业务处理。<br />示例代码中for循环两次，便是为了演示变更与不变更的两种情况。执行程序，打印日志如下：
```java
文件已被更新：1653557504000
文件未更新
```
执行结果符合预期。<br />这种解决方案很明显有两个缺点：

- 无法实时感知文件的变动，程序轮训毕竟有一个时间差；
- lastModified返回的时间单位是毫秒，如果同一毫秒内容出现两次改动，而定时任务查询时恰好落在两次变动之间，则后一次变动则无法被感知到。

第一个缺点，对业务的影响不大；第二个缺点的概率比较小，可以忽略不计；
<a name="ha4XY"></a>
## JDK的Bug登场
上面的代码实现，正常情况下是没什么问题的，但如果使用的Java版本为8或9时，则可能出现意想不到的Bug，这是由JDK本身的Bug导致的。<br />编号为**JDK-8177809**的Bug是这样描述的：<br />![JDK-8177809](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654174164866-2329ffab-58b7-4d8b-aa3d-7c5e184ccc4e.jpeg#clientId=u6918e92a-0d3f-4&from=paste&id=uec99fd0f&originHeight=414&originWidth=1039&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u803f95a4-adc9-4e16-97a0-a805cba03d8&title=JDK-8177809 "JDK-8177809")<br />Bug地址为：[https://bugs.java.com/bugdatabase/view_bug.do?bug_id=8177809](https://bugs.java.com/bugdatabase/view_bug.do?bug_id=8177809)<br />这个Bug的基本描述就是：在Java8和9的某些版本下，lastModified方法返回时间戳并不是毫秒，而是秒，也就是说返回结果的后三位始终为0。<br />来写一个程序验证一下：
```java
public class FileReadDemo {

	public static void main(String[] args) throws IOException, InterruptedException {

		String fileName = "/Users/zzs/temp/1.txt";
		// 创建文件
		createFile(fileName);

		for (int i = 0; i < 10; i++) {
			// 向文件内写入数据
			writeToFile(fileName);
			// 读取文件修改时间
			long timestamp = readLastModified(fileName);
			System.out.println("文件修改时间：" + timestamp);
			// 睡眠100ms
			Thread.sleep(100);
		}
	}

	public static void createFile(String fileName) throws IOException {
		File file = new File(fileName);
		if (!file.exists()) {
			boolean result = file.createNewFile();
			System.out.println("创建文件：" + result);
		}
	}

	public static void writeToFile(String fileName) throws IOException {
		FileWriter fileWriter = new FileWriter(fileName);
		// 写入随机数字
		fileWriter.write(new Random(1000).nextInt());
		fileWriter.close();
	}

	public static long readLastModified(String fileName) {
		File file = new File(fileName);
		return file.lastModified();
	}
}
```
在上述代码中，先创建一个文件，然后在for循环中不停的向文件写入内容，并读取修改时间。每次操作睡眠100ms。这样，同一秒就可以多次写文件和读修改时间。<br />执行结果如下：
```java
文件修改时间：1653558619000
文件修改时间：1653558619000
文件修改时间：1653558619000
文件修改时间：1653558619000
文件修改时间：1653558619000
文件修改时间：1653558619000
文件修改时间：1653558620000
文件修改时间：1653558620000
文件修改时间：1653558620000
文件修改时间：1653558620000
```
修改了10次文件的内容，只感知到了2次。JDK的这个bug让这种实现方式的第2个缺点无限放大了，同一秒发生变更的概率可比同一毫秒发生的概率要大太多了。<br />PS：在官方Bug描述中提到可以通过`Files.getLastModifiedTime`来实现获取时间戳，但这里验证的结果是依旧无效，可能不同版本有不同的表现吧。
<a name="FqniM"></a>
## 更新解决方案
Java 8目前是主流版本，不可能因为JDK的该bug就换JDK吧。所以，要通过其他方式来实现这个业务功能，那就是新增一个用来记录文件版本（version）的文件（或其他存储方式）。这个version的值，可在写文件时按照递增生成版本号，也可以通过对文件的内容做MD5计算获得。<br />如果能保证版本顺序生成，使用时只需读取版本文件中的值进行比对即可，如果变更则重新加载，如果未变更则不做处理。<br />如果使用MD5的形式，则需考虑MD5算法的性能，以及MD5结果的碰撞（概率很小，可以忽略）。<br />下面以版本的形式来展示一下demo：
```java
public class FileReadVersionDemo {
	
	public static int version = 0;
	
	public static void main(String[] args) throws IOException, InterruptedException {
		
		String fileName = "/Users/zzs/temp/1.txt";
		String versionName = "/Users/zzs/temp/version.txt";
		// 创建文件
		createFile(fileName);
		createFile(versionName);
		
		for (int i = 1; i < 10; i++) {
			// 向文件内写入数据
			writeToFile(fileName);
			// 同时写入版本
			writeToFile(versionName, i);
			// 监听器读取文件版本
			int fileVersion = Integer.parseInt(readOneLineFromFile(versionName));
			if (version == fileVersion) {
				System.out.println("版本未变更");
			} else {
				System.out.println("版本已变化，进行业务处理");
			}
			// 睡眠100ms
			Thread.sleep(100);
		}
	}
	
	public static void createFile(String fileName) throws IOException {
		File file = new File(fileName);
		if (!file.exists()) {
			boolean result = file.createNewFile();
			System.out.println("创建文件：" + result);
		}
	}
	
	public static void writeToFile(String fileName) throws IOException {
		writeToFile(fileName, new Random(1000).nextInt());
	}
	
	public static void writeToFile(String fileName, int version) throws IOException {
		FileWriter fileWriter = new FileWriter(fileName);
		fileWriter.write(version +"");
		fileWriter.close();
	}
	
	public static String readOneLineFromFile(String fileName) {
		File file = new File(fileName);
		String tempString = null;
		try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
			//一次读一行，读入null时文件结束
			tempString = reader.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return tempString;
	}
}
```
执行上述代码，打印日志如下：
```python
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
版本已变化，进行业务处理
```
可以看到，每次文件变更都能够感知到。当然，上述代码只是示例，在使用的过程中还是需要更多地完善逻辑。
<a name="PagXO"></a>
## 小结
实践了一个很常见的功能，起初采用很符合常规思路的方案来解决，结果恰好碰到了JDK的Bug，只好变更策略来实现。当然，如果业务环境中已经存在了一些基础的中间件还有更多解决方案。<br />学到了JDK Bug导致的连锁反应，同时也见证了：实践见真知。很多技术方案是否可行，还是需要经得起实践的考验才行。赶快检查一下代码实现，是否命中该Bug？
