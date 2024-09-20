Java
<a name="nPKf1"></a>
## 1、概述
Free Spire.PDF for Java是一个类库，可以直接在Java程序中创建和处理PDF文件，而无需安装任何其他软件。使用此类库可以轻松创建PDF文件。只需创建一个PdfDocument对象，再根据需要创建足够数量的PdfPageBase对象，然后向页面添加字符串，图像，表格，列表等任何其他元素，最后保存文档。
<a name="Lvz3X"></a>
## 2、准备工作
从[https://links.jianshu.com/go?to=https%3A%2F%2Fwww.e-iceblue.com%2FDownload%2Fpdf-for-java-free.html](https://links.jianshu.com/go?to=https%3A%2F%2Fwww.e-iceblue.com%2FDownload%2Fpdf-for-java-free.html)下载Free Spire.PDF for Java，并在Java项目中导入Spire.Pdf.jar文件。
<a name="dINYv"></a>
## 3、坐标系
Spire.PDF生成的页面由边距和内容区域组成。坐标系仅存在于内容区域中，允许用户将内容添加到指定坐标。Spire.PDF中的坐标系遵循以下规则。

- 坐标系的原点（0,0）即内容区域的左上角顶点。
- X轴向右延伸，Y轴向下延伸。

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641295216235-618a06fc-f02b-4d9d-aad6-65d11241002e.webp#clientId=ue3b82979-9f37-4&from=paste&id=ub9e7d5a5&originHeight=328&originWidth=241&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6fea216a-f645-42e4-8c9c-227751430d2&title=)
<a name="QwRAZ"></a>
## 4、创建自定义函数
Spire.PDF提供了有用的类和方法，例如`PdfPageBase`类，`drawString`方法和`drawImage`方法，可以借助它们进行页面设置，并添加文本和图像到指定坐标。为了使代码更便于阅读，这里预先定义了以下五个自定义函数来执行不同的任务。
<a name="OjZ52"></a>
### drawTitle
在PDF文件上绘制标题时，可能需要对齐文本。基于`drawString`创建的`drawTitle`函数允许将文本水平对齐，例如，居中对齐。
```java
public static void drawTitle(PdfPageBase page, String text, PdfTrueTypeFont font, PdfBrush brush, float x, float y, PdfTextAlignment alignment) {
    
    //通过PdfStringFormat类库设置文本对齐
    PdfStringFormat format = new PdfStringFormat();
    format.setAlignment(alignment);

    //在页面绘制题目
    page.getCanvas().drawString(text, font, brush, x, y, format);
}
```
<a name="SvwqU"></a>
### `drawParagraph`
`drawParagraph`函数与`drawTitle`不同，尽管它们采用类似的参数。此函数基于PdfTextWidget.draw方法创建，并返回`PdfLayoutResult`类的对象，该对象包含正在绘制的元素的边界信息。通过这样做，能够知道当前段落的结束位置以及从何处开始绘制下一段内容。
```java
public static PdfLayoutResult drawParagraph(PdfPageBase page, String text, PdfTrueTypeFont font, PdfBrush brush, float x, float y) {

    // 创建一个PdfTextWidget对象
    PdfTextWidget widget = new PdfTextWidget(text, font, brush);

    // dfLayoutType设置为分页，使内容自动分页
    PdfTextLayout layout = new PdfTextLayout();
    layout.setLayout(PdfLayoutType.Paginate);

    //在将要放置段落的地方创建一个矩形
    Rectangle2D.Float rect = new Rectangle2D.Float(0, y, (float) page.getClientSize().getWidth(), (float) page.getClientSize().getHeight());

    //在页面绘制段落
    PdfLayoutResult layoutResult = widget.draw(page, rect, layout);
    return layoutResult;
}
```
<a name="Thn6Y"></a>
### `drawImage`
`drawImage`函数简单地重载了原始方法。此函数不会返回`PdfLayoutResult`对象。为了知道以下内容开始的位置，需要通过PdfImage对象获取图像高度，然后计算Y坐标。
```java
public static void drawImage(PdfPageBase page, PdfImage image, float x, float y) {

    //在页面绘制图像
    page.getCanvas().drawImage(image, x, y);
}
```
<a name="yPi6P"></a>
### `drawTable`
使用`drawTable`函数在PDF页面上绘制表格时，需要在`String[][]`数组中指定表格数据。此函数还会返回到`PdfLayoutResult`对象，该对象可以获取表格的位置和大小。
```java
public static PdfLayoutResult drawTable(PdfPageBase page, String[][] dataSource, PdfTrueTypeFont font, float x, float y) {

    //创建一个PdfTable对象
    PdfTable table = new PdfTable();

    //填充指定单元格
    table.getStyle().setCellPadding(2);

    //设置字体
    table.getStyle().getDefaultStyle().setFont(font);

    //使用样本数据填充表格
    table.setDataSource(dataSource);

    //在页面绘制表格
    PdfLayoutResult layoutResult = table.draw(page, new Point2D.Float(x, y));
    return layoutResult;
}
```
<a name="mQL91"></a>
### `drawSortedList`
最后一个函数`drawSortedList`与`drawTable`非常相似，只是它需要一些独特的参数，如列表内容，顺序标记和列表缩进。
```java
public static PdfLayoutResult drawSortedList(PdfPageBase page, String listContent, PdfOrderedMarker marker, PdfTrueTypeFont font, PdfBrush brush, float x, float y, float indent) {
    //根据列表内容创建排序列表
    PdfSortedList sortedList = new PdfSortedList(listContent);

    //设置列表属性
    sortedList.setFont(font);
    sortedList.setBrush(brush);
    sortedList.setIndent(indent);
    sortedList.setMarker(marker);

    //在页面绘制列表
    PdfLayoutResult layoutResult = sortedList.draw(page, x, y);
    return layoutResult;

}
```
<a name="BFD2R"></a>
## 5、使用的代码
调用这些自定义函数，将能够在PDF页面的适当位置绘制元素。
```java
import com.spire.pdf.PdfDocument;
import com.spire.pdf.PdfNumberStyle;
import com.spire.pdf.PdfPageBase;
import com.spire.pdf.graphics.*;
import com.spire.pdf.lists.*;
import com.spire.pdf.tables.PdfTable;

import java.awt.*;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;

public class CreatePdf {
    public static void main(String[] args) {

        //创建一个PdfDocument对象
        PdfDocument doc = new PdfDocument();

        //添加页面
        PdfPageBase page = doc.getPages().add();
       
        //创建两个纯色笔刷
        PdfSolidBrush brush1 = new PdfSolidBrush(new PdfRGBColor(Color.BLUE));
        PdfSolidBrush brush2 = new PdfSolidBrush(new PdfRGBColor(Color.BLACK));
      
        //创建两种字体
        PdfTrueTypeFont font1 = new PdfTrueTypeFont(new Font("Arial Unicode MS", 
        Font.PLAIN,  14), true);
        PdfTrueTypeFont font2 = new PdfTrueTypeFont(new Font("Arial Unicode MS", 
        Font.PLAIN, 10), true);

        //初始化X轴和Y轴坐标
        float x = 0;
        float y = 0;
        
        //题目
        String title = "题目";
        
        //通过PdfTextAlignment类库将文本居中对齐
        PdfTextAlignment alignment = PdfTextAlignment.Center;
        
        //在文档中心绘制标题
        drawTitle(page, title, font1, brush1, (float) page.getClientSize().getWidth() / 2, y, 
        alignment);
        y = y + 30;

        //段落文本
        String paragraph = "测试文字测试文字测试文字测试文字测试文字测试文字测试文字 " +
        "测试文字测试文字测试文字测试文字测试文字测试文字测试文字测试文字测试文字 " +
        "测试文字测试文字测试文字测试文字测试文字测试文字测试文字测试文字测试文字)";

        //在页面绘制段落
        PdfLayoutResult layoutResult = drawParagraph(page, paragraph,font2, brush2, x, y);
        y = y + (float) layoutResult.getBounds().getHeight() + 10;

        //加载一个图像文件
        PdfImage image = PdfImage.fromImage("C:\\Users\\Administrator\\Desktop\\th.jpg");

        //在页面绘制图像
        drawImage(page, image, x, y);
        y = y + (float) image.getPhysicalDimension().getHeight() + 10;

        //定义样本数据
        String[] data = {"第一列;第二列;第三列",
                         "a1;a2;a3",
                         "b1;b2;b3"};
        String[][] dataSource = new String[data.length][];
        for (int i = 0; i < data.length; i++) {
        dataSource[i] = data[i].split("[;]", -1);
        }

        //在页面绘制表格
        layoutResult = drawTable(page, dataSource, font2, x, y);
        y = y + (float) layoutResult.getBounds().getHeight() + 10;

        //创建一个顺序标记
        PdfOrderedMarker marker = new PdfOrderedMarker(PdfNumberStyle.Upper_Roman, new   
        PdfFont(PdfFontFamily.Helvetica, 12f, PdfFontStyle.Bold));

        //定义列表内容
        String listString = "e苹果\n香蕉\n葡萄\n柚子\n西瓜";

        //在页面绘制列表
        drawSortedList(page, listString, marker, font2, brush2, x, y, 10);

         //保存文件
        doc.saveToFile("CreatePdf.pdf");
    }

}
```
<a name="zhqJ1"></a>
### 生成的PDF文件如下所示。
![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1641295216264-67ecc7ab-1c72-4859-a552-df67647bfcc8.webp#clientId=ue3b82979-9f37-4&from=paste&id=uaf64e59d&originHeight=383&originWidth=620&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u71c6d640-d99f-460d-a0f4-65b627e8c8e&title=)
<a name="rGT3R"></a>
## 6、将PDF转换为其他格式
可以直接通过`saveToFile`方法将生成的PDF（PdfDocument对象）保存为PDF文件，以及其他文件格式，如Doc，Docx，XPS，SVG和HTML。在下面的代码片段中，展示如何加载现有的PDF文档并进行文件格式转换。
```java
//创建一个PdfDocument对象
PdfDocument doc = new PdfDocument();

//加载现有的PDF文件
doc.loadFromFile("C:/Users/Administrator/Desktop/Input.pdf");

//保存为Doc文件格式
doc.saveToFile("output/ToDoc.doc",FileFormat.DOC);

//保存为Docx文件格式
doc.saveToFile("output/ToDocx.docx",FileFormat.DOCX);

//保存为Html文件格式
doc.saveToFile("output/ToHtml.html",FileFormat.HTML);

//保存为Xps文件格式
doc.saveToFile("output/ToXps.xps",FileFormat.XPS);

//保存为Svg文件格式
doc.saveToFile("output/ToSvg.svg",FileFormat.SVG);
```
除了上面提到的文件格式，Spire.PDF还支持将PDF转换为普通的图像格式，如PNG，JPEG，EMF和TIFF。代码有点难度，但并不复杂。
```java
//创建一个PdfDocument对象
PdfDocument doc = new PdfDocument();

//加载现有的PDF文件
doc.loadFromFile("C:/Users/Administrator/Desktop/Input.pdf");

//声明一个BufferedImage实例
BufferedImage image;

//循环浏览PDF页面
for (int i = 0; i < doc.getPages().getCount(); i++) {

  //使用saveAsImage方法将每个页面保存为BufferedImage
  image = doc.saveAsImage(i);

  //将BufferedImage保存为PNG文件格式
  File file = new File( String.format("ToImage-img-%d.png", i));
  ImageIO.write(image, "PNG", file);
}
```
以下是Free Spire.PDF支持的一些其他有用的功能列表。<br />Java添加页眉页脚到PDF<br />Java添加文本/图片水印到PDF<br />Java添加附件到PDF<br />Java添加图片/动态图章到 PDF<br />Java在PDF中应用不同的字体<br />Java填充PDF表单域<br />Java在PDF中添加表单域<br />Java在PDF中添加条形码<br />Java在PDF中添加形状<br />Java提取PDF文本内容/图片<br />Java加密和解密PDF<br />Java合并/拆分PDF文档<br />Java打印PDF文档
<a name="EqO2X"></a>
## 7、总结
借助 Free Spire.PDF 提供的便于使用的API，可以从头开始快速创建PDF文件并将PDF文件转换为其他文件格式，而无需花费大量学习时间。
