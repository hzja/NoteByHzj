Java 
<a name="QirUE"></a>
### 一、介绍
在实际的业务开发的时候，研发人员往往会碰到很多这样的一些场景，需要提供相关的电子凭证信息给用户，例如网银／支付宝／微信购物支付的电子发票、订单的库存打印单、各种电子签署合同等等，以方便用户查看、打印或者下载。<br />例如下图的电子发票！<br />![2021-09-15-09-33-23-102052.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631669616265-bb6186b7-b431-4b43-a4a2-18b0bc56fc81.jpeg#clientId=ua5cf1c4b-d957-4&from=ui&id=ub810a768&originHeight=619&originWidth=1080&originalType=binary&ratio=1&size=74719&status=done&style=shadow&taskId=ua15e10dd-b893-44b5-867b-a27550af1c8)<br />熟悉这块业务的童鞋，一定特别清楚，目前最常用的解决方案是：把相关的数据信息，通过一些技术手段生成对应的 PDF 文件，然后返回给用户，以便预览、下载或者打印。<br />不太熟悉这项技术的童鞋，也不用着急，一起来详细了解一下在线生成 PDF 文件的技术实现手段！
<a name="Mhv4J"></a>
### 二、案例实现
在介绍这个代码实践之前，先来了解一下这个第三方库：iText。<br />iText是著名的开放源码站点sourceforge一个项目，是用于生成PDF文档的一个java类库，通过iText不仅可以生成PDF或rtf的文档，而且还可以将XML、Html文件转化为PDF文件。<br />iText目前有两套版本，分别是iText5和iText7。iText5应该是网上用的比较多的一个版本。iText5因为是很多开发者参与贡献代码，因此在一些规范和设计上存在不合理的地方。iText7是后来官方针对iText5的重构，两个版本差别还是挺大的。不过在实际使用中，一般用到的都比较简单的 API，所以不用特别拘泥于使用哪个版本。
<a name="cuOjI"></a>
#### 2.1、添加 iText 依赖包
在使用它之前，先引人相关的依赖包！
```html
<dependencies>
  <!-- pdf:start -->
  <dependency>
    <groupId>com.itextpdf</groupId>
    <artifactId>itextpdf</artifactId>
    <version>5.5.11</version>
  </dependency>
  <dependency>
    <groupId>com.itextpdf.tool</groupId>
    <artifactId>xmlworker</artifactId>
    <version>5.5.11</version>
  </dependency>
  <!-- 支持中文 -->
  <dependency>
    <groupId>com.itextpdf</groupId>
    <artifactId>itext-asian</artifactId>
    <version>5.2.0</version>
  </dependency>
  <!-- 支持css样式渲染 -->
  <dependency>
    <groupId>org.xhtmlrenderer</groupId>
    <artifactId>flying-saucer-pdf-itext5</artifactId>
    <version>9.1.16</version>
  </dependency>
  <!-- 转换html为标准xhtml包 -->
  <dependency>
    <groupId>net.sf.jtidy</groupId>
    <artifactId>jtidy</artifactId>
    <version>r938</version>
  </dependency>
  <!-- pdf:end -->    
</dependencies>
```
<a name="Oh4c6"></a>
#### 2.2、简单实现
先来一个hello world，代码如下：
```java
public class CreatePDFMainTest {

    public static void main(String[] args) throws Exception {
        Document document = new Document(PageSize.A4);
        //第二步，创建Writer实例
        PdfWriter.getInstance(document, new FileOutputStream("hello.pdf"));
        //创建中文字体
        BaseFont bfchinese = BaseFont.createFont("STSong-Light", "UniGB-UCS2-H", BaseFont.NOT_EMBEDDED);
        Font fontChinese = new Font(bfchinese, 12, Font.NORMAL);
        //第三步，打开文档
        document.open();
        //第四步，写入内容
        Paragraph paragraph = new Paragraph("hello world", fontChinese);
        document.add(paragraph);
        //第五步，关闭文档
        document.close();
    }
}
```
打开hello.pdf文件，内容如下！<br />![2021-09-15-09-33-23-263021.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631669616246-92f83086-74b8-4396-aa5a-cc20bb61a732.jpeg#clientId=ua5cf1c4b-d957-4&from=ui&id=hJGkX&originHeight=258&originWidth=1080&originalType=binary&ratio=1&size=15228&status=done&style=shadow&taskId=u0cc1e2dc-0106-45aa-9b5f-2a1214d5c74)
<a name="xh9CA"></a>
#### 2.3、复杂实现
在实际的业务开发中，因为业务场景非常复杂，而且变化快，往往不会采用上面介绍的写入内容方式来生成文件，而是采用HTML文件转化为PDF文件。<br />例如下面这张入库单！<br />![](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631669616265-1280574b-61da-4892-a14d-96872c8dd803.jpeg#from=url&id=fnPpI&originHeight=335&originWidth=1080&originalType=binary&ratio=1&status=done&style=shadow)<br />应该如何快速实现呢？<br />首先采用html语言编写一个入库单页面，将其命令为printDemo.html，源代码如下：
```html
<html>
  <head></head>
  <body>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title>出库单</title>
    <div>
      <div>
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tbody>
            <tr>
              <td height="40" colspan="2"><h3 style="font-weight: bold; text-align: center; letter-spacing: 5px; font-size: 24px;">入库单</h3></td>
              <td width="12%" height="20" rowspan="2">
                <img style="width: 105px;height: 105px;" src="data:image/jpeg;base64,iVBORw0KGgoAAAANSUhEUgAAAH0AAAB9AQAAAACn+1GIAAAAqElEQVR42u3VMQ7DMAwDQP6A//8lx24qKRRw0s1yu8Uw4OQGIaHsBHUfLzzwAxCAInoZg6dI9dUUBIOyHEG56CmodAaxwtfbboLTVWpeU9+EDAH37m9CmkTYxDGUE0agMIakk3y4Ut8G37iom02M4bPniHWAtqFDTjjSGLrZvXAOmTnL1124C73r6Yo8Ane61k6eQeVjIM2h482D1RwScrpNjuH5R/0b3s6ZZNyKlt3iAAAAAElFTkSuQmCC" />
              </td>
            </tr>
            <tr>
              <td width="50%" height="30">操作人：xxx</td>
              <td width="50%" height="30" colspan="2">创建时间：2021-09-14 12:00:00</td>
            </tr>
          </tbody>
        </table>
      </div>
      <div style="margin-top: 5px; margin-bottom: 6px; margin-left: 4px"></div>
      <div>
        <table width="100%"
               style="border-collapse: collapse; border-spacing: 0;border:0px;">
          <tr style="height: 25px;">
            <td style="background: #eaeaea; text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;"
                width="10%">序号</td>
            <td style="background: #eaeaea; text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;"
                width="30%">商品</td>
            <td style="background: #eaeaea; text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;"
                width="30%">单位</td>
            <td style="background: #eaeaea; text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-right: 1px solid #000000;"
                width="30%">数量</td>
          </tr>
          <tr>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">1</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">xxx沐浴露</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">箱</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-right: 1px solid #000000;">3</td>
          </tr>
          <tr>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">2</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">xxx洗发水</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">箱</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-right: 1px solid #000000;">4</td>
          </tr>
          <tr>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">3</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">xxx洗衣粉</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000;">箱</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-right: 1px solid #000000;">5</td>
          </tr>
          <tr>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-bottom: 1px solid #000000;">4</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-bottom: 1px solid #000000;">xxx洗面奶</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-bottom: 1px solid #000000;">箱</td>
            <td style="text-align: center; border-left: 1px solid #000000; border-top: 1px solid #000000; border-right: 1px solid #000000; border-bottom: 1px solid #000000;">5</td>
          </tr>
        </table>
      </div>
    </div>
  </body>

</html>
```
接着，将html文件转成PDF文件，源码如下：
```java
public class CreatePDFMainTest {


    /**
     * 创建PDF文件
     * @param htmlStr
     * @throws Exception
     */
    private static void writeToOutputStreamAsPDF(String htmlStr) throws Exception {
        String targetFile = "pdfDemo.pdf";
        File targeFile = new File(targetFile);
        if(targeFile.exists()) {
            targeFile.delete();
        }

        //定义pdf文件尺寸，采用A4横切
        Document document = new Document(PageSize.A4, 25, 25, 15, 40);// 左、右、上、下间距
        //定义输出路径
        PdfWriter writer = PdfWriter.getInstance(document, new FileOutputStream(targetFile));
        PdfReportHeaderFooter header = new PdfReportHeaderFooter("", 8, PageSize.A4);
        writer.setPageEvent(header);
        writer.addViewerPreference(PdfName.PRINTSCALING, PdfName.NONE);
        document.open();

        // CSS
        CSSResolver cssResolver = new StyleAttrCSSResolver();
        CssAppliers cssAppliers = new CssAppliersImpl(new XMLWorkerFontProvider(){

            @Override
            public Font getFont(String fontname, String encoding, boolean embedded, float size, int style, BaseColor color) {
                try {
                    //用于中文显示的Provider
                    BaseFont bfChinese = BaseFont.createFont("STSongStd-Light", "UniGB-UCS2-H", BaseFont.NOT_EMBEDDED);
                    return new Font(bfChinese, size, style);
                } catch (Exception e) {
                    return super.getFont(fontname, encoding, size, style);
                }
            }
        });

        //html
        HtmlPipelineContext htmlContext = new HtmlPipelineContext(cssAppliers);
        htmlContext.setTagFactory(Tags.getHtmlTagProcessorFactory());
        htmlContext.setImageProvider(new AbstractImageProvider() {
            @Override
            public Image retrieve(String src) {
                //支持图片显示
                int pos = src.indexOf("base64,");
                try {
                    if (src.startsWith("data") && pos > 0) {
                        byte[] img = Base64.decode(src.substring(pos + 7));
                        return Image.getInstance(img);
                    } else if (src.startsWith("http")) {
                        return Image.getInstance(src);
                    }
                } catch (BadElementException ex) {
                    return null;
                } catch (IOException ex) {
                    return null;
                }
                return null;
            }

            @Override
            public String getImageRootPath() {
                return null;
            }
        });


        // Pipelines
        PdfWriterPipeline pdf = new PdfWriterPipeline(document, writer);
        HtmlPipeline html = new HtmlPipeline(htmlContext, pdf);
        CssResolverPipeline css = new CssResolverPipeline(cssResolver, html);

        // XML Worker
        XMLWorker worker = new XMLWorker(css, true);
        XMLParser p = new XMLParser(worker);
        p.parse(new ByteArrayInputStream(htmlStr.getBytes()));

        document.close();
    }

    /**
     * 读取 HTML 文件
     * @return
     */
    private static String readHtmlFile() {
        StringBuffer textHtml = new StringBuffer();
        try {
            File file = new File("printDemo.html");
            BufferedReader reader = new BufferedReader(new FileReader(file));
            String tempString = null;
            // 一次读入一行，直到读入null为文件结束
            while ((tempString = reader.readLine()) != null) {
                textHtml.append(tempString);
            }
            reader.close();
        } catch (IOException e) {
            return null;
        }
        return textHtml.toString();
    }

    public static void main(String[] args) throws Exception {
        //读取html文件
        String htmlStr = readHtmlFile();
        //将html文件转成PDF
        writeToOutputStreamAsPDF(htmlStr);
    }
}
```
运行程序，打开pdfDemo.pdf，结果如下！<br />![2021-09-15-09-33-23-428025.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631669616332-da1bd765-35b6-485d-b7d9-bc85f11f9ef7.jpeg#clientId=ua5cf1c4b-d957-4&from=ui&id=D1aqK&originHeight=513&originWidth=1080&originalType=binary&ratio=1&size=50341&status=done&style=shadow&taskId=u0bce8e69-2ce3-4721-b34f-4cc79cda0a2)
<a name="vyVRR"></a>
#### 2.4、变量替换方式
上面的html文件，是事先已经编辑好的，才能正常渲染。<br />但是在实际的业务开发的时候，例如下面的商品内容，完全是动态的，还是xxx-202109入库单的名称，以及二维码，都是动态的。<br />这个时候，可以采用freemarker模板引擎，通过定义变量来动态填充内容，直到转换出来的结果就是想要的html页面。<br />当然，还有一种办法，例如下面这个，也可以在html页面里面定义${name}变量，然后在读取完文件之后，将其变量进行替换成想填充的任何值，这其实也是模板引擎最核心的一个玩法。
```html
<html>
  <head>
    <meta charset="utf-8">
    <title></title>
  </head>
  <body>
    <div>您好：${name}</div>
    <div>欢迎，登录博客网站</div>
  </body>
</html>
```
<a name="Opbw0"></a>
### 三、总结
itext框架是一个非常实用的第三方pdf文件生成库，尤其是面对比较简单的pdf文件内容渲染的时候，它完全满足需求。<br />但是对于那种复杂的pdf文档，可能需要自己单独进行适配开发。具体的深度玩法，大家可以参阅itext官方API。
