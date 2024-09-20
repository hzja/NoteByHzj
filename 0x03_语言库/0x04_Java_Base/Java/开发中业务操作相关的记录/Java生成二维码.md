Java 二维码
<a name="fJVqL"></a>
### 一、介绍
说到二维码，相信大家每天都会用到，尤其是在手机支付的场景，使用频率极广。<br />实际上二维码在1994年的时候就已经诞生了，由 Denso 公司研制而成，只是那个时候使用范围还不是很大。<br />早期的二维码由于很容易通过技术方式进行伪造，因此很少有企业愿意去使用他，随着技术的不断迭代和更新，二维码的安全性更进一步得到了提升，从而使得更多的企业愿意使用这项新技术，例如当下的移动支付，还有微信互推，扫码出行等等，极大的方便了网民们的购物、社交和出行！<br />在实际的业务开发过程中，二维码的使用场景开发也会经常出现在开发人员的面前，应该如何去处理呢？
<a name="MXJBf"></a>
### 二、代码实践
在 Java 生态体系里面，操作二维码的开源项目很多，如 SwetakeQRCode、BarCode4j、Zxing 等等。<br />介绍下简单易用的 google 公司的 zxing，zxing 不仅使用方便，而且可以还操作条形码或者二维码等，不仅有 java 版本，还有 Android 版。<br />开源库地址：

- GitHub 开源地址：[https://github.com/zxing/zxing](https://github.com/zxing/zxing)
- zxing 二进制包下载地址：[http://repo1.maven.org/maven2/com/google/zxing](http://repo1.maven.org/maven2/com/google/zxing)
- zxing Maven 仓库地址：[https://mvnrepository.com/artifact/com.google.zxing](https://mvnrepository.com/artifact/com.google.zxing)

通过 Maven 仓库，可以很轻松的将其依赖包添加到自己的项目。
<a name="YDNlr"></a>
#### 2.1、添加依赖包
开发中如果是非 web 应用则导入 core 包即可，如果是 web 应用，则 core 与 javase 一起导入。
```xml
<!-- 如果是非 web 应用则导入 core 包即可，如果是 web 应用，则 core 与 javase 一起导入。-->
<dependency>
    <groupId>com.google.zxing</groupId>
    <artifactId>core</artifactId>
    <version>3.3.3</version>
</dependency>

<dependency>
    <groupId>com.google.zxing</groupId>
    <artifactId>javase</artifactId>
    <version>3.3.3</version>
</dependency>
```
<a name="nv9jn"></a>
#### 2.2、生成二维码
如何快速生成二维码呢？请看下面的测试代码！
```java
import com.google.zxing.BarcodeFormat;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;

import javax.imageio.ImageIO;
import javax.swing.filechooser.FileSystemView;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * 二维码、条形码工具类
 */
public class QRCodeWriteUtil {

    /**
     * CODE_WIDTH：二维码宽度，单位像素
     * CODE_HEIGHT：二维码高度，单位像素
     * FRONT_COLOR：二维码前景色，0x000000 表示黑色
     * BACKGROUND_COLOR：二维码背景色，0xFFFFFF 表示白色
     * 演示用 16 进制表示，和前端页面 CSS 的取色是一样的，注意前后景颜色应该对比明显，如常见的黑白
     */
    private static final int CODE_WIDTH = 400;
    private static final int CODE_HEIGHT = 400;
    private static final int FRONT_COLOR = 0x000000;
    private static final int BACKGROUND_COLOR = 0xFFFFFF;

    /**
     * 生成二维码 并 保存为图片
     * @param codeContent
     */
    public static void createCodeToFile(String codeContent) {
        try {
            //获取系统目录
            String filePathDir = FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath();
            //随机生成 png 格式图片
            String fileName = new Date().getTime() + ".png";

            /**com.google.zxing.EncodeHintType：编码提示类型,枚举类型
             * EncodeHintType.CHARACTER_SET：设置字符编码类型
             * EncodeHintType.ERROR_CORRECTION：设置误差校正
             *      ErrorCorrectionLevel：误差校正等级，L = ~7% correction、M = ~15% correction、Q = ~25% correction、H = ~30% correction
             *      不设置时，默认为 L 等级，等级不一样，生成的图案不同，但扫描的结果是一样的
             * EncodeHintType.MARGIN：设置二维码边距，单位像素，值越小，二维码距离四周越近
             * */
            Map<EncodeHintType, Object> hints = new HashMap();
            hints.put(EncodeHintType.CHARACTER_SET, "UTF-8");
            hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.M);
            hints.put(EncodeHintType.MARGIN, 1);

            /**
             * MultiFormatWriter:多格式写入，这是一个工厂类，里面重载了两个 encode 方法，用于写入条形码或二维码
             *      encode(String contents,BarcodeFormat format,int width, int height,Map<EncodeHintType,?> hints)
             *      contents:条形码/二维码内容
             *      format：编码类型，如 条形码，二维码 等
             *      width：码的宽度
             *      height：码的高度
             *      hints：码内容的编码类型
             * BarcodeFormat：枚举该程序包已知的条形码格式，即创建何种码，如 1 维的条形码，2 维的二维码 等
             * BitMatrix：位(比特)矩阵或叫2D矩阵，也就是需要的二维码
             */
            MultiFormatWriter multiFormatWriter = new MultiFormatWriter();
            BitMatrix bitMatrix = multiFormatWriter.encode(codeContent, BarcodeFormat.QR_CODE, CODE_WIDTH, CODE_HEIGHT, hints);

            /**java.awt.image.BufferedImage：具有图像数据的可访问缓冲图像，实现了 RenderedImage 接口
             * BitMatrix 的 get(int x, int y) 获取比特矩阵内容，指定位置有值，则返回true，将其设置为前景色，否则设置为背景色
             * BufferedImage 的 setRGB(int x, int y, int rgb) 方法设置图像像素
             *      x：像素位置的横坐标，即列
             *      y：像素位置的纵坐标，即行
             *      rgb：像素的值，采用 16 进制,如 0xFFFFFF 白色
             */
            BufferedImage bufferedImage = new BufferedImage(CODE_WIDTH, CODE_HEIGHT, BufferedImage.TYPE_INT_BGR);
            for (int x = 0; x < CODE_WIDTH; x++) {
                for (int y = 0; y < CODE_HEIGHT; y++) {
                    bufferedImage.setRGB(x, y, bitMatrix.get(x, y) ? FRONT_COLOR : BACKGROUND_COLOR);
                }
            }

            /**javax.imageio.ImageIO java 扩展的图像IO
             * write(RenderedImage im,String formatName,File output)
             *      im：待写入的图像
             *      formatName：图像写入的格式
             *      output：写入的图像文件，文件不存在时会自动创建
             *
             * 即将保存的二维码图片文件*/
            File codeImgFile = new File(filePathDir, fileName);
            ImageIO.write(bufferedImage, "png", codeImgFile);

            System.out.println("二维码图片生成成功：" + codeImgFile.getPath());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String codeContent1 = "Hello World";
        createCodeToFile(codeContent1);

        String codeContent2 = "https://www.baidu.com/";
        createCodeToFile(codeContent2);
    }
}
```
先创建一个内容为Hello World的二维码，然后在创建一个内容为https://www.baidu.com/链接地址的二维码。<br />运行程序之后，输出内容如下：
```
二维码图片生成成功：/Users/Desktop/1632403131016.png
二维码图片生成成功：/Users/Desktop/1632403131233.png
```
打开图片内容！<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632540623905-e5657189-8360-4f97-9c30-6331283d7649.webp#clientId=u24dee0b4-ed9a-4&from=paste&id=u85ce5580&originHeight=446&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uc48c9639-ee68-421d-8c75-98d1b557a93)<br />用微信扫一扫，结果如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632540623901-fdbf14e7-96dc-4131-b350-41c673bf3fd9.webp#clientId=u24dee0b4-ed9a-4&from=paste&id=uf6af2774&originHeight=665&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u357827c1-e04c-4278-908e-5e646263e30)
<a name="rubeu"></a>
#### 2.3、读取二维码
创建很容易，那么如何读取二维码内容呢？请看下面的测试代码：
```java
import com.google.zxing.*;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.common.HybridBinarizer;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

/**
 * 二维码、条形码工具类
 */
public class QRCodeReadUtil {

    /**
     * 解析二维码内容(文件)
     * @param file
     * @return
     * @throws IOException
     */
    public static String parseQRCodeByFile(File file) throws IOException {
        BufferedImage bufferedImage = ImageIO.read(file);
        return parseQRCode(bufferedImage);
    }

    /**
     * 解析二维码内容(网络链接)
     * @param url
     * @return
     * @throws IOException
     */
    public static String parseQRCodeByUrl(URL url) throws IOException {
        BufferedImage bufferedImage = ImageIO.read(url);
        return parseQRCode(bufferedImage);
    }

    private static String parseQRCode(BufferedImage bufferedImage){
        try {
            /**
             * com.google.zxing.client.j2se.BufferedImageLuminanceSource：缓冲图像亮度源
             * 将 java.awt.image.BufferedImage 转为 zxing 的 缓冲图像亮度源
             * 关键就是下面这几句：HybridBinarizer 用于读取二维码图像数据，BinaryBitmap 二进制位图
             */
            LuminanceSource source = new BufferedImageLuminanceSource(bufferedImage);
            BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
            Map<DecodeHintType, Object> hints = new HashMap<>();
            hints.put(DecodeHintType.CHARACTER_SET, "UTF-8");

            /**
             * 如果图片不是二维码图片，则 decode 抛异常：com.google.zxing.NotFoundException
             * MultiFormatWriter 的 encode 用于对内容进行编码成 2D 矩阵
             * MultiFormatReader 的 decode 用于读取二进制位图数据
             */
            Result result = new MultiFormatReader().decode(bitmap, hints);
            return result.getText();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("-----解析二维码内容失败-----");
        }
        return null;
    }

    public static void main(String[] args) throws IOException {
        File localFile = new File("/Users/Desktop/1632403131016.png");
        String content1 = parseQRCodeByFile(localFile);
        System.out.println(localFile + " 二维码内容：" + content1);
        URL url = new URL("http://cdn.pzblog.cn/1951b6c4b40fd81630903bf6f7037156.png");
        String content2 = parseQRCodeByUrl(url);
        System.out.println(url + " 二维码内容：" + content2);
    }
}
```
运行程序，输出内容如下：
```
/Users/Desktop/1632403131016.png 二维码内容：Hello World
http://cdn.pzblog.cn/1951b6c4b40fd81630903bf6f7037156.png 二维码内容：https://www.baidu.com/
```
<a name="OX7ga"></a>
#### 2.4、web 二维码交互展示
在实际的项目开发过程中，很多时候二维码都是根据参数实时输出到网页上进行显示的，它的实现原理类似验证码，例如下图，它们都是后台先生成内存图像`BufferedImage`，然后使用`ImageIO.write`写出来。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1632540624116-922f4770-7708-4970-b934-1e4d0da076bd.webp#clientId=u24dee0b4-ed9a-4&from=paste&id=u59cf1c64&originHeight=608&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u17656a1e-7dfd-412e-ba8a-f1ce3c4f9c5)<br />在线生成二维码的功能，其实也类似于此！<br />前端关键代码如下：
```html
<img src="http://xxxx/projectDemo/qrCode" alt="验证码,点击刷新！" onclick="this.src=this.src+'?temp='+Math.random();" class="content-code fl-r" />
```
后端关键代码如下：
```java
@Controller
public class SystemController {
 
    @GetMapping("qrCode")
    public void getQRCode(HttpServletResponse response) {
     String content = "Hello World";
        try {
            /**
             * 调用工具类生成二维码并输出到输出流中
             */
            QRCodeWriteUtil.createCodeToOutputStream(content, response.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
其中`createCodeToOutputStream`方法，源码如下：
```java
/**
 * 生成二维码 并 保存为图片
 * @param codeContent
 */
public static void createCodeToOutputStream(String codeContent, OutputStream outputStream) {
    try {

        /**com.google.zxing.EncodeHintType：编码提示类型,枚举类型
         * EncodeHintType.CHARACTER_SET：设置字符编码类型
         * EncodeHintType.ERROR_CORRECTION：设置误差校正
         *      ErrorCorrectionLevel：误差校正等级，L = ~7% correction、M = ~15% correction、Q = ~25% correction、H = ~30% correction
         *      不设置时，默认为 L 等级，等级不一样，生成的图案不同，但扫描的结果是一样的
         * EncodeHintType.MARGIN：设置二维码边距，单位像素，值越小，二维码距离四周越近
         * */
        Map<EncodeHintType, Object> hints = new HashMap();
        hints.put(EncodeHintType.CHARACTER_SET, "UTF-8");
        hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.M);
        hints.put(EncodeHintType.MARGIN, 1);

        /**
         * MultiFormatWriter:多格式写入，这是一个工厂类，里面重载了两个 encode 方法，用于写入条形码或二维码
         *      encode(String contents,BarcodeFormat format,int width, int height,Map<EncodeHintType,?> hints)
         *      contents:条形码/二维码内容
         *      format：编码类型，如 条形码，二维码 等
         *      width：码的宽度
         *      height：码的高度
         *      hints：码内容的编码类型
         * BarcodeFormat：枚举该程序包已知的条形码格式，即创建何种码，如 1 维的条形码，2 维的二维码 等
         * BitMatrix：位(比特)矩阵或叫2D矩阵，也就是需要的二维码
         */
        MultiFormatWriter multiFormatWriter = new MultiFormatWriter();
        BitMatrix bitMatrix = multiFormatWriter.encode(codeContent, BarcodeFormat.QR_CODE, CODE_WIDTH, CODE_HEIGHT, hints);

        /**java.awt.image.BufferedImage：具有图像数据的可访问缓冲图像，实现了 RenderedImage 接口
         * BitMatrix 的 get(int x, int y) 获取比特矩阵内容，指定位置有值，则返回true，将其设置为前景色，否则设置为背景色
         * BufferedImage 的 setRGB(int x, int y, int rgb) 方法设置图像像素
         *      x：像素位置的横坐标，即列
         *      y：像素位置的纵坐标，即行
         *      rgb：像素的值，采用 16 进制,如 0xFFFFFF 白色
         */
        BufferedImage bufferedImage = new BufferedImage(CODE_WIDTH, CODE_HEIGHT, BufferedImage.TYPE_INT_BGR);
        for (int x = 0; x < CODE_WIDTH; x++) {
            for (int y = 0; y < CODE_HEIGHT; y++) {
                bufferedImage.setRGB(x, y, bitMatrix.get(x, y) ? FRONT_COLOR : BACKGROUND_COLOR);
            }
        }
        
        ImageIO.write(bufferedImage, "png", outputStream);
        System.out.println("二维码图片生成成功");
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```
这种方式，如果是单体应用，其实没太大问题，在微服务开发的环境下有局限性。<br />因此还有另外一种玩法，那就是将生成的图片流转成`base64`的格式，然后返回给前端进行展示。<br />关键代码改造过程如下：
```java
//定义字节输出流，将bufferedImage写入
ByteArrayOutputStream out = new ByteArrayOutputStream();
ImageIO.write(bufferedImage, "png", out);
//将输出流转换成base64
String str64 = Base64.getEncoder().encodeToString(out.toByteArray());
```
最后，把`base64`内容以`json`的形式返回给前端，进行展示！
