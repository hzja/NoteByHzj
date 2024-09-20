Java InputSteam
<a name="pvgzF"></a>
## 1、InputStream转化为String
<a name="fOdGp"></a>
### 1、使用 `InputStreamReader` 和 `StringBuilder` (JDK)
```java
public class InputStream2String {
    public static void main(String[] args) {
        try {
            InputStream inputStream = new FileInputStream("E:/duckAndJava/IO/testFile.txt");    //路径修改为本地文件所在的位置

            char[] buffer = new char[1024];    //根据需要的数组大小进行自定义
            StringBuilder out = new StringBuilder();
            Reader in = new InputStreamReader(inputStream, "UTF-8");
            for (int numRead; (numRead = in.read(buffer, 0, buffer.length)) > 0; ) {
                out.append(buffer, 0, numRead);
            }
            String myString =  out.toString();

            System.out.println("myString = " + myString);

        }catch (IOException e){
            e.printStackTrace();
        }
    }
}
```
<a name="C64a7"></a>
### 2、使用 `inputStream.read()` and `StringBuilder`
```java
StringBuilder sb = new StringBuilder();
for (int ch; (ch = inputStream.read()) != -1; ) {
    sb.append((char) ch);
}
String myString = sb.toString();
```
<a name="gw54R"></a>
### 3、使用 `ByteArrayOutputStream` and `inputStream.read`
```java
ByteArrayOutputStream result = new ByteArrayOutputStream();
byte[] buffer = new byte[1024];
for (int length; (length = inputStream.read(buffer)) != -1; ) {
    result.write(buffer, 0, length);
}
String myString = result.toString("UTF-8");
```
```java
ByteArrayOutputStream result = new ByteArrayOutputStream();
byte[] buffer = new byte[1024];
int length;
while ((length = inputStream.read(buffer)) != -1) {
    result.write(buffer, 0, length);
}
String str = result.toString(StandardCharsets.UTF_8.name());
return str;
```
```java
BufferedInputStream bis = new BufferedInputStream(inputStream);
ByteArrayOutputStream buf = new ByteArrayOutputStream();
int result = bis.read();
while(result != -1) {
    buf.write((byte) result);
    result = bis.read();
}
String str = buf.toString();
return str;
```
<a name="Xh1F8"></a>
### 4、使用 `BufferedInputStream` 和 `ByteArrayOutputStream`
```java
BufferedInputStream bis = new BufferedInputStream(inputStream);
ByteArrayOutputStream buf = new ByteArrayOutputStream();
for (int result = bis.read(); result != -1; result = bis.read()) {
    buf.write((byte) result);
}
String myString = buf.toString("UTF-8");
```
<a name="IOgXF"></a>
### 5、使用 `BufferedReader`
```java
String newLine = System.getProperty("line.separator");
BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
StringBuilder result = new StringBuilder();
for (String line; (line = reader.readLine()) != null; ) {
    if (result.length() > 0) {
        result.append(newLine);
    }
    result.append(line);
}
String myString = result.toString();
```
```java
StringBuilder sb = new StringBuilder();
String line;

BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
while ((line = br.readLine()) != null) {
    sb.append(line);
}
String str = sb.toString();
return str;
```
```java
String result = new BufferedReader(new InputStreamReader(inputStream))
        .lines().collect(Collectors.joining(System.lineSeparator()));
```
```java
String result = new BufferedReader(new InputStreamReader(inputStream))
       .lines().parallel().collect(Collectors.joining(System.lineSeparator()));
```
<a name="APIcS"></a>
### 6、使用 Stream API 或 parallel Stream API
```java
String myString = new BufferedReader(new InputStreamReader(inputStream)).lines().collect(Collectors.joining("\n"));
```
或
```java
String myString = new BufferedReader(new InputStreamReader(inputStream)).lines().parallel().collect(Collectors.joining("\n"));
```
<a name="xeXHA"></a>
### 7、使用 `StringWriter` 和`IOUtils.copy (Apache Commons)`
```java
StringWriter writer = new StringWriter();
IOUtils.copy(inputStream, writer, StandardCharsets.UTF_8.name());
return writer.toString();
```
甚至可以直接这样用
```java
String result = IOUtils.toString(inputStream, StandardCharsets.UTF_8);
```
<a name="vpWIj"></a>
### 8、使用`CharStreams`(Google Guava)
```java
String result = CharStreams.toString(new InputStreamReader(inputStream, Charsets.UTF_8));
```
```java
//方法十二：
String str = new String(ByteStreams.toByteArray(inputStream))
```
分别按照字符串长度来进行测试。<br />当使用的是一个小字符串（length=175），得到的性能测试结果如下：<br />![2021-05-26-13-57-32-615688.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622008853605-804f6bda-8dbc-4027-9343-c16f9da23d46.png#clientId=ud025398a-1db4-4&from=ui&id=ucb2c9b57&originHeight=231&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=false&size=444548&status=done&style=shadow&taskId=u1eaf9645-efc0-49d9-8885-91fcd2c94c1&title=)<br />当使用的是一个长字符串（length=50100），得到的性能测试结果如下：<br />![2021-05-26-13-57-32-743346.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622008865337-6ad9493f-5982-4990-8e91-5f1c6160a7d7.png#clientId=ud025398a-1db4-4&from=ui&id=uad86ccd3&originHeight=229&originWidth=650&originalType=binary&ratio=1&rotation=0&showTitle=false&size=447581&status=done&style=shadow&taskId=ueb74346a-49ae-4544-822f-37988f9912d&title=)<br />为了更加直观，按照字符串的长度与相应函数消耗的平均时间，做了如下的表格：<br />![2021-05-26-13-57-32-818147.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622008872691-4a545170-0752-490e-9d4b-e7fe83439abf.png#clientId=ud025398a-1db4-4&from=ui&id=u5e1ed4d7&originHeight=249&originWidth=649&originalType=binary&ratio=1&rotation=0&showTitle=false&size=485919&status=done&style=none&taskId=ubcfba08a-91a9-4971-bf38-b306daeb21f&title=)<br />更加直观的表格图，如下：<br />![2021-05-26-13-57-32-897193.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622008889650-80054825-17b0-49a6-8262-a18452460867.png#clientId=ud025398a-1db4-4&from=ui&id=ucae6fb95&originHeight=399&originWidth=639&originalType=binary&ratio=1&rotation=0&showTitle=false&size=766597&status=done&style=shadow&taskId=u3d7ca42e-be33-4154-91e3-26aa45ce07d&title=)
<a name="AEChd"></a>
### 9、JDK原生提供
```java
byte[] bytes = new byte[0];
bytes = new byte[inputStream.available()];
inputStream.read(bytes);
String str = new String(bytes);
```
```java
Scanner s = new Scanner(inputStream).useDelimiter("\\A");
String str = s.hasNext() ? s.next() : "";
```
```java
String resource = new Scanner(inputStream).useDelimiter("\\Z").next();
return resource;
```
<a name="PsuUo"></a>
## 2、String转化为InputStream
<a name="XZt2b"></a>
### 2.1 JDK原生提供
```java
InputStream is = new ByteArrayInputStream(str.getBytes());
```
<a name="YWxDV"></a>
### 2.2 Apache Common提供
```java
InputStream targetStream = IOUtils.toInputStream(str, StandardCharsets.UTF_8.name());
```
<a name="A4xxO"></a>
### 2.3 Google Guava提供
```java
InputStream targetStream =
        new ReaderInputStream(CharSource.wrap(str).openStream(), StandardCharsets.UTF_8.name());
```
