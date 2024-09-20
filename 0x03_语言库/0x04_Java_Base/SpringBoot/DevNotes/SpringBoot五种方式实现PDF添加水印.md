JavaSpringBoot
<a name="CzzRx"></a>
## 简介
PDF（Portable Document Format，便携式文档格式）是一种流行的文件格式，它可以在多个操作系统和应用程序中进行查看和打印。在某些情况下，需要对 PDF 文件添加水印，以使其更具有辨识度或者保护其版权。本文将介绍如何使用 Spring Boot 来实现 PDF 添加水印的方式。
<a name="CnrYx"></a>
## 方式一：使用 Apache PDFBox 库
PDFBox 是一个流行的、免费的、用 Java 编写的库，它可以用来创建、修改和提取 PDF 内容。PDFBox 提供了许多 API，包括添加文本水印的功能。
<a name="H1rRy"></a>
### 添加 PDFBox 依赖
首先，在 pom.xml 文件中添加 PDFBox 的依赖：
```xml
<dependency>
  <groupId>org.apache.pdfbox</groupId>
  <artifactId>pdfbox</artifactId>
  <version>2.0.24</version>
</dependency>
```
<a name="jbVGG"></a>
### 添加水印
在添加水印之前，需要读取原始 PDF 文件：
```java
PDDocument document = PDDocument.load(new File("original.pdf"));
```
然后，遍历 PDF 中的所有页面，并使用 `PDPageContentStream` 添加水印：
```java
// 遍历 PDF 中的所有页面
for (int i = 0; i < document.getNumberOfPages(); i++) {
    PDPage page = document.getPage(i);
    PDPageContentStream contentStream = new PDPageContentStream(document, page, PDPageContentStream.AppendMode.APPEND, true, true);

    // 设置字体和字号
    contentStream.setFont(PDType1Font.HELVETICA_BOLD, 36);

    // 设置透明度
    contentStream.setNonStrokingColor(200, 200, 200);

    // 添加文本水印
    contentStream.beginText();
    contentStream.newLineAtOffset(100, 100); // 设置水印位置
    contentStream.showText("Watermark"); // 设置水印内容
    contentStream.endText();

    contentStream.close();
}
```
最后，需要保存修改后的 PDF 文件：
```java
document.save(new File("output.pdf"));
document.close();
```
<a name="d8n15"></a>
### 完整代码
下面是使用 PDFBox 来实现 PDF 添加水印的完整代码：
```java
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.PDPage;
import org.apache.pdfbox.pdmodel.PDPageContentStream;
import org.apache.pdfbox.pdmodel.font.PDType1Font;

import java.io.File;
import java.io.IOException;

public class PdfBoxWatermark {
    public static void main(String[] args) throws IOException {
        // 读取原始 PDF 文件
        PDDocument document = PDDocument.load(new File("original.pdf"));

        // 遍历 PDF 中的所有页面
        for (int i = 0; i < document.getNumberOfPages(); i++) {
            PDPage page = document.getPage(i);
            PDPageContentStream contentStream = new PDPageContentStream(document, page, PDPageContentStream.AppendMode.APPEND, true, true);

            // 设置字体和字号
            contentStream.setFont(PDType1Font.HELVETICA_BOLD, 36);

            // 设置透明度
            contentStream.setNonStrokingColor(200, 200, 200);

            // 添加文本水印
            contentStream.beginText();
            contentStream.newLineAtOffset(100, 100); // 设置水印位置
            contentStream.showText("Watermark"); // 设置水印内容
            contentStream.endText();

            contentStream.close();
        }

        // 保存修改后的 PDF 文件
        document.save(new File("output.pdf"));
        document.close();
    }
}
```
<a name="RshWB"></a>
## 方式二：使用 iText 库
iText 是一款流行的 Java PDF 库，它可以用来创建、读取、修改和提取 PDF 内容。iText 提供了许多 API，包括添加文本水印的功能。
<a name="sYRgQ"></a>
### 添加 iText 依赖
在 pom.xml 文件中添加 iText 的依赖：
```xml
<dependency>
  <groupId>com.itextpdf</groupId>
  <artifactId>itextpdf</artifactId>
  <version>5.5.13</version>
</dependency>
```
<a name="Yb6HQ"></a>
### 添加水印
在添加水印之前，需要读取原始 PDF 文件：
```java
PdfReader reader = new PdfReader("original.pdf");
PdfStamper stamper = new PdfStamper(reader, new FileOutputStream("output.pdf"));
```
然后，遍历 PDF 中的所有页面，并使用 `PdfContentByte` 添加水印：
```java
// 获取 PDF 中的页数
int pageCount = reader.getNumberOfPages();

// 添加水印
for (int i = 1; i <= pageCount; i++) {
    PdfContentByte contentByte = stamper.getUnderContent(i); // 或者 getOverContent()
    contentByte.beginText();
    contentByte.setFontAndSize(BaseFont.createFont(), 36f);
    contentByte.setColorFill(BaseColor.LIGHT_GRAY);
    contentByte.showTextAligned(Element.ALIGN_CENTER, "Watermark", 300, 400, 45);
    contentByte.endText();
}
```
最后，需要保存修改后的 PDF 文件并关闭文件流：
```java
stamper.close();
reader.close();
```
<a name="orUDu"></a>
### 完整代码
下面是使用 iText 来实现 PDF 添加水印的完整代码：
```java
import com.itextpdf.text.*;
import com.itextpdf.text.pdf.*;

import java.io.FileOutputStream;
import java.io.IOException;

public class ItextWatermark {
    public static void main(String[] args) throws IOException, DocumentException {
        // 读取原始 PDF 文件
        PdfReader reader = new PdfReader("original.pdf");
        PdfStamper stamper = new PdfStamper(reader, new FileOutputStream("output.pdf"));

        // 获取 PDF 中的页数
        int pageCount = reader.getNumberOfPages();

        // 添加水印
        for (int i = 1; i <= pageCount; i++) {
            PdfContentByte contentByte = stamper.getUnderContent(i); // 或者 getOverContent()
            contentByte.beginText();
            contentByte.setFontAndSize(BaseFont.createFont(), 36f);
            contentByte.setColorFill(BaseColor.LIGHT_GRAY);
            contentByte.showTextAligned(Element.ALIGN_CENTER, "Watermark", 300, 400, 45);
            contentByte.endText();
        }

        // 保存修改后的 PDF 文件并关闭文件流
        stamper.close();
        reader.close();
    }
}
```
<a name="xeWOX"></a>
## 方式三：用 Ghostscript 命令行
Ghostscript 是一款流行的、免费的、开源的 PDF 处理程序，它可以用来创建、读取、修改和提取 PDF 内容。Ghostscript 中提供了命令行参数来添加水印。
<a name="uI93z"></a>
### Ghostscript
首先需要在本地安装 Ghostscript 程序。可通过以下链接下载安装包：

- [Windows](https://www.ghostscript.com/download/gsdnld.html)
- [macOS](https://www.ghostscript.com/download/gsdnld.html)
- [Linux](macOS)
<a name="J1j3p"></a>
### 添加水印
可以在终端中使用 Ghostscript 的命令行工具执行以下命令来实现：
```bash
gs -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -sOutputFile=output.pdf -c "newpath /Helvetica-Bold findfont 36 scalefont setfont 0.5 setgray 200 200 moveto (Watermark) show showpage" original.pdf
```
上述命令中，`-sDEVICE=pdfwrite` 表示输出为 PDF 文件；`-sOutputFile=output.pdf` 表示输出文件名为 output.pdf；最后一个参数 original.pdf 则表示原始 PDF 文件的路径；中间的字符串则表示添加的水印内容。
<a name="MARuv"></a>
### 注意事项
使用 Ghostscript 命令行添加水印时，会直接修改原始 PDF 文件，因此建议先备份原始文件。
<a name="X6DEH"></a>
## 方式四：Free Spire.PDF for Java
下面介绍一下使用 Free Spire.PDF for Java 实现 PDF 添加水印的方式。<br />Free Spire.PDF for Java 是一款免费的 Java PDF 库，它提供了一个简单易用的 API，用于创建、读取、修改和提取 PDF 内容。Free Spire.PDF for Java 也支持添加文本水印以及图片水印。
<a name="EjNLY"></a>
### 添加 Free Spire.PDF for Java 依赖
首先，在 pom.xml 文件中添加 Free Spire.PDF for Java 的依赖：
```xml
<dependency>
  <groupId>e-iceblue</groupId>
  <artifactId>free-spire-pdf-for-java</artifactId>
  <version>1.9.6</version>
</dependency>
```
<a name="QcRvR"></a>
### 添加文本水印
在添加水印之前，需要读取原始 PDF 文件：
```java
PdfDocument pdf = new PdfDocument();
pdf.loadFromFile("original.pdf");
```
然后，遍历 PDF 中的所有页面，并使用 PdfPageBase 添加水印：
```java
// 遍历 PDF 中的所有页面
for (int i = 0; i < pdf.getPages().getCount(); i++) {
    PdfPageBase page = pdf.getPages().get(i);

    // 添加文本水印
    PdfWatermark watermark = new PdfWatermark("Watermark");
    watermark.setFont(new PdfFont(PdfFontFamily.Helvetica, 36));
    watermark.setOpacity(0.5f);
    page.getWatermarks().add(watermark);
}
```
最后，需要保存修改后的 PDF 文件：
```java
pdf.saveToFile("output.pdf");
pdf.close();
```
<a name="OFrhM"></a>
### 添加图片水印
添加图片水印与添加文本水印类似，只需要将 PdfWatermark 的参数修改为图片路径即可。
```java
// 添加图片水印
PdfWatermark watermark = new PdfWatermark("watermark.png");
watermark.setOpacity(0.5f);
page.getWatermarks().add(watermark);
```
<a name="GtSOy"></a>
### 完整代码
下面是使用 Free Spire.PDF for Java 来实现 PDF 添加水印的完整代码：
```java
import com.spire.pdf.*;

public class FreeSpirePdfWatermark {
    public static void main(String[] args) {
        // 读取原始 PDF 文件
        PdfDocument pdf = new PdfDocument();
        pdf.loadFromFile("original.pdf");

        // 遍历 PDF 中的所有页面
        for (int i = 0; i < pdf.getPages().getCount(); i++) {
            PdfPageBase page = pdf.getPages().get(i);

            // 添加文本水印
            PdfWatermark watermark = new PdfWatermark("Watermark");
            watermark.setFont(new PdfFont(PdfFontFamily.Helvetica, 36));
            watermark.setOpacity(0.5f);
            page.getWatermarks().add(watermark);

            // 添加图片水印
            // PdfWatermark watermark = new PdfWatermark("watermark.png");
            // watermark.setOpacity(0.5f);
            // page.getWatermarks().add(watermark);
        }

        // 保存修改后的 PDF 文件
        pdf.saveToFile("output.pdf");
        pdf.close();
    }
}
```
<a name="RxWbw"></a>
## 方式五：Aspose.PDF for Java
Aspose.PDF for Java 是一个强大的 PDF 处理库，提供了添加水印的功能。结合 Spring Boot 使用 Aspose.PDF for Java 库添加 PDF 水印的方式如下：<br />首先，在 pom.xml 文件中添加 Aspose.PDF for Java 的依赖：
```xml
<dependency>
  <groupId>com.aspose</groupId>
  <artifactId>aspose-pdf</artifactId>
  <version>21.4</version>
</dependency>
```
在 Spring Boot 应用程序中调用 Aspose.PDF for Java 的 API 设置 PDF 水印。
<a name="PUsIE"></a>
### 添加文本水印
```java
@PostMapping("/addTextWatermark")
public ResponseEntity<byte[]> addTextWatermark(@RequestParam("file") MultipartFile file) throws IOException {
    // 加载 PDF 文件
    Document pdfDocument = new Document(file.getInputStream());
    TextStamp textStamp = new TextStamp("Watermark");
    textStamp.setWordWrap(true);
    textStamp.setVerticalAlignment(VerticalAlignment.Center);
    textStamp.setHorizontalAlignment(HorizontalAlignment.Center);
    pdfDocument.getPages().get_Item(1).addStamp(textStamp);

    // 保存 PDF 文件
    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
    pdfDocument.save(outputStream);
    return ResponseEntity.ok()
            .header(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=\"watermarked.pdf\"")
            .contentType(MediaType.APPLICATION_PDF)
            .body(outputStream.toByteArray());
}
```
<a name="uUAaP"></a>
### 添加图片水印
```java
@PostMapping("/addImageWatermark")
public ResponseEntity<byte[]> addImageWatermark(@RequestParam("file") MultipartFile file) throws IOException {
    // 加载 PDF 文件
    Document pdfDocument = new Document(file.getInputStream());
    ImageStamp imageStamp = new ImageStamp("watermark.png");
    imageStamp.setWidth(100);
    imageStamp.setHeight(100);
    imageStamp.setVerticalAlignment(VerticalAlignment.Center);
    imageStamp.setHorizontalAlignment(HorizontalAlignment.Center);
    pdfDocument.getPages().get_Item(1).addStamp(imageStamp);

    // 保存 PDF 文件
    ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
    pdfDocument.save(outputStream);
    return ResponseEntity.ok()
            .header(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=\"watermarked.pdf\"")
            .contentType(MediaType.APPLICATION_PDF)
            .body(outputStream.toByteArray());
}
```
注意，以上代码中的文件名、宽度、高度等参数需要根据实际情况进行调整。
<a name="zcZg0"></a>
### 完整代码
完整的 Spring Boot 控制器类代码如下：
```java
import com.aspose.pdf.*;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

@RestController
@RequestMapping("/api/pdf")
public class PdfController {
    @PostMapping("/addTextWatermark")
    public ResponseEntity<byte[]> addTextWatermark(@RequestParam("file") MultipartFile file) throws IOException {
        // 加载 PDF 文件
        Document pdfDocument = new Document(file.getInputStream());
        TextStamp textStamp = new TextStamp("Watermark");
        textStamp.setWordWrap(true);
        textStamp.setVerticalAlignment(VerticalAlignment.Center);
        textStamp.setHorizontalAlignment(HorizontalAlignment.Center);
        pdfDocument.getPages().get_Item(1).addStamp(textStamp);

        // 保存 PDF 文件
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        pdfDocument.save(outputStream);
        return ResponseEntity.ok()
        .header(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=\"watermarked.pdf\"")
        .contentType(MediaType.APPLICATION_PDF)
        .body(outputStream.toByteArray());
    }

    @PostMapping("/addImageWatermark")
    public ResponseEntity<byte[]> addImageWatermark(@RequestParam("file") MultipartFile file) throws IOException {
        // 加载 PDF 文件
        Document pdfDocument = new Document(file.getInputStream());
        ImageStamp imageStamp = new ImageStamp("watermark.png");
        imageStamp.setWidth(100);
        imageStamp.setHeight(100);
        imageStamp.setVerticalAlignment(VerticalAlignment.Center);
        imageStamp.setHorizontalAlignment(HorizontalAlignment.Center);
        pdfDocument.getPages().get_Item(1).addStamp(imageStamp);

        // 保存 PDF 文件
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        pdfDocument.save(outputStream);
        return ResponseEntity.ok()
        .header(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=\"watermarked.pdf\"")
        .contentType(MediaType.APPLICATION_PDF)
        .body(outputStream.toByteArray());
    }
}
```
这里使用了两个 RESTful API：/addTextWatermark 和 /addImageWatermark，分别用于添加文本水印和图片水印。在请求中通过 file 参数传递 PDF 文件。<br />下面介绍如何使用 Postman 来测试 Spring Boot 应用程序的 API。

1. 下载并安装 Postman。
2. 打开 Postman，选择 POST 请求方法。
3. 在 URL 地址栏中输入 http://localhost:8080/api/pdf/addTextWatermark。
4. 在 Headers 标签页中设置 Content-Type 为 multipart/form-data。
5. 在 Body 标签页中选择 form-data 类型，然后设置 key 为 file，value 选择本地的 PDF 文件。
6. 点击 Send 按钮发送请求，等待应答结果。

处理结果将会在响应的 Body 中返回，也可以选择浏览器下载或保存到本地磁盘。<br />以上就是使用 Aspose.PDF for Java 库结合 Spring Boot 添加 PDF 水印的方式。
<a name="VnAsY"></a>
## 结论
本文介绍了几种使用 Spring Boot 实现 PDF 添加水印的方式，分别是使用 Apache PDFBox 库、iText 库以及 Ghostscript 命令行等。选择哪种方式，可以根据项目需求和个人偏好来决定。无论采用哪种方式，都需要注意保护原始 PDF 文件，不要在不必要的情况下直接修改原始文件。
<a name="XkA2d"></a>
## Reference
**https://www.ghostscript.com/download/gsdnld.html:**[**https://www.ghostscript.com/download/gsdnld.html**](https://www.ghostscript.com/download/gsdnld.html)
