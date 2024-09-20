分享一个功能比较完善的 PDF 工具类，用搭积木的方式构建 PDF！
<a name="Ad2FX"></a>
## 项目介绍
x-easypdf 是一个基于 pdfbox/fop 二次封装的框架，目前拥有两大模块：

- pdfbox 模块：主打 pdf 编辑功能，以组件化的形式进行 pdf 的构建；
- fop 模块 ：主打 pdf 导出功能，采用数据源的方式对 xsl-fo 模板进行转换，同时提供 java 对象的方式构建 dom 模板文档，即使完全不懂 xsl-fo 的语法，也能轻松上手写出对应的模板。

两个模块均可单独使用，也可以结合使用，帮助开发者快速生成 pdf 文档。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163029150-f77be77a-74c0-4e81-bc9a-d676d64ff356.png#averageHue=%23fdfdfd&clientId=u9e4eea20-5276-4&from=paste&id=uf4584309&originHeight=577&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u782a831e-4804-4e75-b853-3a682dd8c5b&title=)<br />x-easypdf 的 x-easypdf 和 Gitee Scan 的代码扫描结果如下，质量还是很高：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163029150-ffe77264-14c7-4470-a434-d7a9e8ab46fc.png#averageHue=%23fefefe&clientId=u9e4eea20-5276-4&from=paste&id=u9bea8d20&originHeight=795&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7679ca0a-2a8e-43a6-8985-f7ecf5e839b&title=)
<a name="NT06s"></a>
## 项目特性

- 简单智能：自动换行与分页
- 表单填写：内置文档表单填写器（可填写表单中的文本域）
- 图像转换：内置文档图像器（可将文档整个转换为图片，或将文档中的某几页转换为图片）
- 文档提取：内置文档提取器（可提取文档中的文本与图片）
- 文档替换：内置文档替换器（可替换文档中的图片）
- 文档分析：内置文档分析器（可分析文档中的文本与图片信息，包含坐标、内容、字体大小等）
- 文档拆分：内置文档拆分器（可将文档按照页码拆分成一个或多个文档）
- 数字签名：内置文档签名器（可对文档进行数字签名）
- 内置字体：内置细体 | 正常 | 粗体三种开源中文字体（华为鸿蒙字体）
- 组件丰富：内置水印、页眉、页脚、文本、图片、表格、矩形、圆形、线条、布局、条形码（一维码/二维码）等组件
<a name="fTQSu"></a>
## 软件架构
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694163029116-a9e608f4-c21d-4ab6-993c-b4933042972a.png#averageHue=%23353331&clientId=u9e4eea20-5276-4&from=paste&id=uf2e43ba0&originHeight=763&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf9ec5cfd-e313-4298-a829-f1d8db9e740&title=)
<a name="U9cHm"></a>
## 快速入门
全功能模块依赖：
```xml
<dependency>
  <groupId>wiki.xsx</groupId>
  <artifactId>x-easypdf</artifactId>
  <version>版本号</version>
</dependency>
```
单模块依赖:
```xml
<!-- pdfbox模块 -->
<dependency>
  <groupId>wiki.xsx</groupId>
  <artifactId>x-easypdf-pdfbox</artifactId>
  <version>版本号</version>
</dependency>
<!-- fop模块 -->
<dependency>
  <groupId>wiki.xsx</groupId>
  <artifactId>x-easypdf-fop</artifactId>
  <version>版本号</version>
</dependency>
```
<a name="QWToZ"></a>
### 创建 PDF
```java
// 定义pdf输出路径
String outputPath = "E:\\pdf\\test\\pdfbox\\test.pdf";
// 构建文档
XEasyPdfHandler.Document.build().addPage(
    // 构建页面
    XEasyPdfHandler.Page.build(
        // 构建组件
        XEasyPdfHandler.Text.build("文本内容")
    )
    // 保存文档并关闭
).save(outputPath).close();
```
<a name="DB6ZN"></a>
### 编辑 PDF
```java
// 定义pdf输出路径
String sourcePath = "E:\\pdf\\test\\pdfbox\\test.pdf";
// 定义pdf输出路径
String outputPath = "E:\\pdf\\test\\pdfbox\\output.pdf";
// 读取文档
XEasyPdfDocument document = XEasyPdfHandler.Document.load(sourcePath);
// 获取页面
XEasyPdfPage page = document.getPageList().get(0);
// 创建文本组件
XEasyPdfText text = XEasyPdfHandler.Text.build("test");
// 将组件添加到页面
page.addComponent(text);
// 保存文档并关闭
document.save(outputPath).close();
```
<a name="FTTiV"></a>
### 转换 PDF
```java
// 定义fop配置文件路径
String configPath = "E:\\pdf\\template\\fop.xconf";
// 定义xsl-fo模板路径（目录）
String templatePath = "E:\\pdf\\template\\freemarker";
// 定义pdf输出路径
String outputPath = "E:\\pdf\\test\\fo\\Freemarker.pdf";
// 设置模板路径
XEasyPdfTemplateHandler.DataSource.Freemarker.setTemplatePath(templatePath);
// 定义数据map
Map<String, Object> data = new HashMap<>();
// 定义数据list
List<String> list = new ArrayList<>(2);
list.add("hello");
list.add("world");
// 设置值
data.put("list", list);
data.put("str", "hello world");
// 转换pdf
XEasyPdfTemplateHandler.Template.build().setConfigPath(configPath).setDataSource(
    // 构建数据源
    XEasyPdfTemplateHandler.DataSource.Freemarker.build()
    // 设置模板名称（模板路径下的文件名称）
    .setTemplateName("template.fo")
    // 设置模板数据
    .setTemplateData(data)
).transform(outputPath);
```
<a name="lZ8z6"></a>
## 总结
x-easypdf 是一个基于 pdfbox/fop 二次封装的框架，用搭积木的方式构建 pdf。

- 项目地址：[**https://x-easypdf.cn/**](https://x-easypdf.cn/)
- 官网：[**https://gitee.com/dromara/x-easypdf**](https://gitee.com/dromara/x-easypdf)
