Java<br />来学习如何用Zip4j库创建受密码保护的压缩文件并将其解压。
<a name="jF1Hi"></a>
### 依赖
先把 zip4j 依赖关系添加到 pom.xml 文件中。
```xml
<dependency>
  <groupId>net.lingala.zip4j</groupId>
  <artifactId>zip4j</artifactId>
  <version>2.9.0</version>
</dependency>
```
<a name="IEJJz"></a>
### 压缩一个文件
首先，将使用ZipFile `addFile()`方法将一个名为aFile.txt的文件压缩到一个名为compressed.zip的有密码保护的 zip 文件。
```java
ZipParameters zipParameters = new ZipParameters();
zipParameters.setEncryptFiles(true);
zipParameters.setCompressionLevel(CompressionLevel.HIGHER);
zipParameters.setEncryptionMethod(EncryptionMethod.AES);

ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
zipFile.addFile(new File("aFile.txt"), zipParameters);
```
`setCompressionLevel`一行是可选的。可以从FASTEST到ULTRA级别中选择（默认是`NORMAL`）。<br />在这个例子中，使用了AES加密。如果想使用Zip标准加密，只需用`ZIP_STANDARD`替换AES。<br />注意，如果文件 "aFile.txt "在磁盘上不存在，该方法将抛出一个异常。net.lingala.zip4j.exception.ZipException File does not exist: …<br />为了解决这个问题，必须确保该文件是手动创建并放置在项目文件夹中，或者必须从Java中创建它。
```java
File fileToAdd = new File("aFile.txt");
if (!fileToAdd.exists()) {
    fileToAdd.createNewFile();
}
```
另外，在完成了新的ZipFile之后，需要及时关闭资源：
```java
zipFile.close();
```
<a name="RqTbK"></a>
### 压缩多个文件
修改一下代码，以便能够一次压缩多个文件。
```java
ZipParameters zipParameters = new ZipParameters();
zipParameters.setEncryptFiles(true);
zipParameters.setEncryptionMethod(EncryptionMethod.AES);

List<File> filesToAdd = Arrays.asList(
    new File("aFile.txt"),
    new File("bFile.txt")
);

ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
zipFile.addFiles(filesToAdd, zipParameters);
```
不使用`addFile`方法，而是使用`addFiles()`并传入一个List的文件。
<a name="AfyK4"></a>
### 压缩一个目录
可以简单地用`addFolder`代替`addFile`方法来压缩一个文件夹。
```java
ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
zipFile.addFolder(new File("/users/folder_to_add"), zipParameters);
```
<a name="ps9HA"></a>
### 创建一个分割的压缩文件
可以通过使用`createSplitZipFile`和`createSplitZipFileFromFolder`方法，``将压缩文件分割成几个文件。
```java
ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
int splitLength = 1024 * 1024 * 10; //10MB
zipFile.createSplitZipFile(Arrays.asList(new File("aFile.txt")), zipParameters, true, splitLength);
```
```java
zipFile.createSplitZipFileFromFolder(new File("/users/folder_to_add"), zipParameters, true, splitLength);
```
splitLength的单位是字节。
<a name="RhZxc"></a>
### 提取所有文件
提取文件也同样简单。可以用`extractAll()`方法从compressed.zip中提取所有文件。
```java
ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
zipFile.extractAll("/destination_directory");
```
<a name="A1GEe"></a>
### 提取单个文件
如果只想从compressed.zip中提取一个文件，可以使用`extractFile()`方法。
```java
ZipFile zipFile = new ZipFile("compressed.zip", "password".toCharArray());
zipFile.extractFile("aFile.txt", "/destination_directory");
```
<a name="tNnOX"></a>
### 总结
综上所述，已经学会了如何用Zip4j库在Java中创建受密码保护的压缩文件并将其解压。
