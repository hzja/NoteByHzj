Java
<a name="DDg8i"></a>
## 文件摘要校验
有些小工具会附带一个校验器校验附带提供的checksum值，防止有人恶意篡改小工具，保证小工具可以放心使用。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1638319477044-b20f6d9b-3285-4fbe-bfc1-ea6a84d424bb.webp#clientId=u6afaeaa7-4806-4&from=paste&id=ubdb328c4&originHeight=302&originWidth=528&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63f19262-d643-49d8-8230-e27663687f2&title=)<br />文件Hash校验<br />如果两个文件的内容相同，那么它们的摘要应该是相同的。这个原理能不能鉴定两个文件是否相同呢？
<a name="dyzFI"></a>
### Java实现文件摘要
带着这个疑问，写了一个文件摘要提取工具类：
```java
/**
 * 提取文件 checksum 
 *
 * @param path      文件全路径
 * @param algorithm  算法名 例如 MD5、SHA-1、SHA-256等
 * @return  checksum
 * @throws NoSuchAlgorithmException the no such algorithm exception
 * @throws IOException              the io exception
 */
public static String extractChecksum(String path, String algorithm) throws NoSuchAlgorithmException, IOException {
    // 根据算法名称初始化摘要算法
    MessageDigest digest = MessageDigest.getInstance(algorithm);
    // 读取文件的所有比特
    byte[] fileBytes = Files.readAllBytes(Paths.get(path));
    // 摘要更新
    digest.update(fileBytes);
    //完成哈希摘要计算并返回特征值
    byte[] digested = digest.digest();
    // 进行十六进制的输出
    return HexUtils.toHexString(digested);
}
```
接下来做几组对照试验来证明猜想。
<a name="c8jz3"></a>
### 内容不变
首先要证明一个文件在内容不变的情况下摘要是否有变化，多次执行下面的代码，断言始终都是true。
```java
String path = "C:\\Users\\s1\\IdeaProjects\\demo\\src\\main\\resources\\application.yml";
String checksum = extractChecksum(path, "SHA-1");
String hash = "6bf4d6c101b4a7821226d3ec1f8d778a531bf265";
Assertions.assertEquals(hash,checksum);
```
而且把文件名改成application-dev.yml，甚至application-dev.txt摘要都是相同的。又把yml文件的内容作了改动，断言就false了。这证明了单个文件的情况下，内容不变，hash是不变的。
<a name="akpFc"></a>
### 文件复制
把yml文件复制了一份，改了文件名称和类型，不改变内容并存到了另一个目录中，来测试一下它们的摘要是否有变化。
```java
String path1 = "C:\\Users\\s1\\IdeaProjects\\demo\\src\\main\\resources\\application.yml";
String path2 = "C:\\Users\\s1\\IdeaProjects\\demo\\src\\main\\resources\\templates\\application-dev.txt";
String checksum1 = extractChecksum(path1, "SHA-1");
String checksum2 = extractChecksum(path2, "SHA-1");
String hash = "6bf4d6c101b4a7821226d3ec1f8d778a531bf265";
Assertions.assertEquals(hash,checksum1);
Assertions.assertEquals(hash,checksum2);
```
结果断言通过，不过改变了其中一个文件的内容后断言就不通过了。
<a name="yR5Kh"></a>
### 新建空文件
这里的新建空文件指的是没有进行任何操作的新建的空文件。<br />新建的空文件会根据特定的算法返回一个固定值，比如SHA-1算法下的空文件值是:
```java
da39a3ee5e6b4b0d3255bfef95601890afd80709
```
<a name="tFh45"></a>
### 结论
通过实验证明了：

- 在相同算法下，任何新建空文件的摘要值都是固定的。
- 任何两个内容相同的文件的摘要值都是相同的，和路径、文件名、文件类型无关。
- 文件的摘要值会随着文件内容的改变而改变。
<a name="CzNpj"></a>
## 文件摘要运用
根据上面的结论，文件摘要是可以防止同样内容的文件重复提交的， 存储的时候不但要存储文件的路径，还要存储文件的摘要值，可能需要注意新建空文件的的固定摘要问题。另外在Java12中提供了新的API来处理文件内容重复问题：<br />[Java12新特性](https://www.yuque.com/fcant/java/pm0y6k?view=doc_embed&inner=daad27f3)
