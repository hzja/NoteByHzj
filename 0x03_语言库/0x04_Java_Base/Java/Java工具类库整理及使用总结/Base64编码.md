JavaBase64编码
<a name="iz10v"></a>
## Base64编码原理
base64编码就是用64个可打印的字符作为一个字符集来表示数据。这64个字符包括A-Z、a-z、0-9这62个常见字符，另外还有两个可打印字符在不同的系统中可能会有所不同，常见的是+和/这两个字符，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654698523810-30520608-fb57-460c-80e7-51c56d681993.jpeg#clientId=u6a0352f7-3b02-4&from=paste&id=u30ee1232&originHeight=826&originWidth=596&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u86ed1941-28ae-41bf-a96e-23704e27bbc&title=)<br />Base64编码本质上是一种将二进制数据转成文本数据的方案。对于非二进制数据，是先将其转换成二进制形式，然后每连续6比特（2的6次方=64）计算其十进制值，根据该值在上面的索引表中找到对应的字符，最终得到一个文本字符串。<br />假设要对 Hello! 进行Base64编码，按照ASCII表，其转换过程如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654698523822-fa646410-44d7-446a-9be4-bff7f56f5c04.jpeg#clientId=u6a0352f7-3b02-4&from=paste&id=u9ddfe602&originHeight=164&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u339d81ef-b6cf-4b88-8709-b768b861b44&title=)<br />可知 Hello! 的Base64编码结果为 SGVsbG8h ，原始字符串长度为6个字符，编码后长度为8个字符，每3个原始字符经Base64编码成4个字符。<br />但这里需要注意一个点：Base64编码是每3个原始字符编码成4个字符，如果原始字符串长度不能被3整除，那怎么办？使用0值来补充原始字符串。以 Hello!! 为例，其转换过程为：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1654698523892-a221ff8d-84e4-49b7-810e-686155d85b50.jpeg#clientId=u6a0352f7-3b02-4&from=paste&id=uf530a485&originHeight=327&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u75957ed4-07f5-406b-a799-b70805185f4&title=)<br />注：图表中蓝色背景的二进制0值是额外补充的。<br />Hello!! Base64编码的结果为 SGVsbG8hIQAA 。最后2个零值只是为了Base64编码而补充的，在原始字符中并没有对应的字符，那么Base64编码结果中的最后两个字符 AA 实际不带有效信息，所以需要特殊处理，以免解码错误。<br />标准Base64编码通常用 = 字符来替换最后的 A，即编码结果为 SGVsbG8hIQ==。因为 = 字符并不在Base64编码索引表中，其意义在于结束符号，在Base64解码时遇到 = 时即可知道一个Base64编码字符串结束。<br />如果Base64编码字符串不会相互拼接再传输，那么最后的 = 也可以省略，解码时如果发现Base64编码字符串长度不能被4整除，则先补充 = 字符，再解码即可。<br />解码是对编码的逆向操作，但注意一点：对于最后的两个 = 字符，转换成两个 A 字符，再转成对应的两个6比特二进制0值，接着转成原始字符之前，需要将最后的两个6比特二进制0值丢弃，因为它们实际上不携带有效信息。
<a name="NK1yL"></a>
## Base64编码有什么用
<a name="ZDLp2"></a>
### 邮件传送
以前发送邮件只支持可见字符的传送。由此，需要有一个方法将不可见的字符转换为可见的字符，Base64编码算法便可解决这个问题。
<a name="vd68V"></a>
### 图片的base64编码
图片的base64编码主要是为了用在网页上减少请求次数,base64编码可以将图片添加到css中，实现请求css即可下载下来图片，减少了再次请求图片的请求。
<a name="kttzx"></a>
## URL Base64算法
Base64编码值通过URL传输会出现问题，因为Base64编码中的“+”和“/”符号是不允许出现在URL中的。同样，符号“=”用做参数分隔符，也不允许出现在URL中;为了解决这些问题，有一种URL安全的Base64编码，URL安全的Base64编码特点：<br />(1) 不能被3整除时，不补=符号。<br />(2) 生成Base64编码中，"+"和"/"被替换成其他非URL保留字符，使其可以直接放入URL中传输。比如"+"和"/"被替换成"-"和"_"。
<a name="hZTWD"></a>
## Java Base64库
<a name="Pdh2B"></a>
### 标准Base64编解码
JDK8开始已经提供了Base64的实现<br />以 Hello!! 为例来验证其编码结果是不是前面所说的SGVsbG8hIQ==
```java
import java.util.Base64;

class Base64Test{
	//标准Base64编解码
	public void test0(){
		//编码
		System.out.println(Base64.getEncoder().encodeToString("Hello!!".getBytes()));
		
		//解码
		byte[] bytes = Base64.getDecoder().decode("SGVsbG8hIQ==");
		for (byte b : bytes){
			System.out.print((char)b);
		}
	}
}

public class Base64Example {
	public static void main(String[] args) {
		Base64Test base64Test = new Base64Test();
		base64Test.test0();
	}
}
```
> ******************【运行结果】******************<br />SGVsbG8hIQ==<br />Hello!!

可见程序输出也是：SGVsbG8hIQ==
<a name="KVLo9"></a>
### URL Base64算法
要使用URL Base64算法，需要引入common codec依赖包：
```xml
<dependency>
	<groupId>commons-codec</groupId>
	<artifactId>commons-codec</artifactId>
	<version>1.14</version>
</dependency>
```
```java
package Base64;

import java.util.Base64;

class Base64Test{
    //URL Base64算法
    public void test1(){
        System.out.println();
        String value = "Hello!!";
        System.out.println(Base64.getEncoder().encodeToString(value.getBytes()));
        System.out.println(org.apache.commons.codec.binary.Base64.encodeBase64URLSafeString(value.getBytes()));
    }
}

public class Base64Example {
    public static void main(String[] args) {
        Base64Test base64Test = new Base64Test();
        base64Test.test1();
    }
}
```
> ******************【运行结果】******************<br />SGVsbG8hIQ==<br />SGVsbG8hIQ

通过对比，确实相对于标准的Base64算法，URL Base64算法编码的结果没有=填充符了。
