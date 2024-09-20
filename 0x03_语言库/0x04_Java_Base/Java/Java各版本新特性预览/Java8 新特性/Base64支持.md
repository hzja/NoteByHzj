Java8 Base64<br />在 Java 7中，经常需要使用第三方库就可以进行 Base64 编码。<br />在 Java 8中，Base64 编码已经成为 Java 类库的标准，实例如下：
```java
public class Tester {
    public static void main(String[] args) {
        final String text = "Base64 finally in Java 8!";
        final String encoded = Base64.getEncoder().encodeToString(text.getBytes(StandardCharsets.UTF_8));
        System.out.println(encoded);
        final String decoded = new String(Base64.getDecoder().decode(encoded), StandardCharsets.UTF_8);
        System.out.println(decoded);
    }
}
```
输出结果：
```
QmFzZTY0IGZpbmFsbHkgaW4gSmF2YSA4IQ==
Base64 finally in Java 8!
```
新的 Base64API 也支持 URL 和 MINE 的编码解码，详情可以查看具体类方法。
