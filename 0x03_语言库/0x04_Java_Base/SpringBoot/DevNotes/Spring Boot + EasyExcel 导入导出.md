Java SpringBoot EasyExcel
<a name="doHlA"></a>
## 一、EasyExcel
EasyExcel 是阿里巴巴开源 poi 插件之一，主要解决了 poi 框架使用复杂，sax 解析模式不容易操作，数据量大起来容易 OOM，解决了 POI 并发造成的报错。<br />主要解决方式：通过解压文件的方式加载，一行一行的加载，并且抛弃样式字体等不重要的数据，降低内存的占用。<br />EasyExcel 优势

- 注解式自定义操作。
- 输入输出简单，提供输入输出过程的接口
- 支持一定程度的单元格合并等灵活化操作
<a name="EPnsB"></a>
## 二、常用注解
<a name="mEXXn"></a>
### `@ExcelProperty`
指定当前字段对应 excel 中的那一列。可以根据名字或者 Index 去匹配。当然也可以不写，默认第一个字段就是 index=0，以此类推。千万注意，要么全部不写，要么全部用 index，要么全部用名字去匹配。千万别三个混着用，除非非常了解源代码中三个混着用怎么去排序的。
<a name="SHihg"></a>
### `@ExcelIgnore`
默认所有字段都会和 excel 去匹配，加了这个注解会忽略该字段
<a name="HNsMD"></a>
### `@DateTimeFormat`
日期转换，用 String 去接收 excel 日期格式的数据会调用这个注解。里面的 value 参照 java.text.SimpleDateFormat
<a name="lwqLJ"></a>
### `@NumberFormat`
数字转换，用 String 去接收 excel 数字格式的数据会调用这个注解。里面的 value 参照 java.text.DecimalFormat
<a name="boQM3"></a>
### `@ExcelIgnoreUnannotated`
默认不加 ExcelProperty 的注解的都会参与读写，加了不会参与
<a name="SfrZ4"></a>
## 三、依赖
```xml
<!-- easyexcel 主要依赖  这一个基本上就够了-->
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>easyexcel</artifactId>
    <version>2.1.4</version>
</dependency>
<!-- servlet-api -->
<dependency>
    <groupId>javax.servlet</groupId>
    <artifactId>javax.servlet-api</artifactId>
    <version>4.0.1</version>
    <scope>provided</scope>
</dependency>
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.47</version>
</dependency>
```
<a name="Ys0yX"></a>
## 四、监听
```java
public class ExcelListener extends AnalysisEventListener {

    private List<Object> datas = new ArrayList<Object>();

    @Override
    public void invoke(Object o, AnalysisContext analysisContext) {
        datas.add(o);
        doSomething(o);
    }

    private void doSomething(Object object) {

    }

    public List<Object> getDatas() {
        return datas;
    }

    public void setDatas(List<Object> datas) {
        this.datas = datas;
    }

    @Override
    public void doAfterAllAnalysed(AnalysisContext analysisContext) {

    }
}
```
<a name="RoTds"></a>
## 五、接口导入 Excel
```java
try {
    String filename = file.getOriginalFilename();
    InputStream inputStream = file.getInputStream();
    ExcelListener listener = new ExcelListener();
    ExcelReader excelReader = new ExcelReader(inputStream, ExcelTypeEnum.XLS, null, listener);
    excelReader.read(new Sheet(1, 0, Test.class));
    List<Object> list = listener.getDatas();
    if (list.size() > 1) {
        for (int i = 0; i < list.size(); i++) {
            Testobj = (Test) list.get(i);
            JSONObject jo = new JSONObject();
        }
    }
} catch (Exception e) {
    System.out.println(e.getMessage());
}
```
<a name="zeGq9"></a>
## 六、接口导出 Excel
```java
HttpServletResponse response, HttpServletRequest request
 try {
    String filenames = "111111";
    String userAgent = request.getHeader("User-Agent");
    if (userAgent.contains("MSIE") || userAgent.contains("Trident")) {
        filenames = URLEncoder.encode(filenames, "UTF-8");
    } else {
        filenames = new String(filenames.getBytes("UTF-8"), "ISO-8859-1");
    }
    response.setContentType("application/vnd.ms-exce");
    response.setCharacterEncoding("utf-8");
    response.addHeader("Content-Disposition", "file);
    EasyExcel.write(response.getOutputStream(), Test.class).sheet("sheet").doWrite(testList);
} catch (Exception e) {
}
```
<a name="Puz0G"></a>
## 七、本地导入、本地导出
```java
List<Test> testList = new ArrayList<>();
try {
    String strUrl = "C:\\Users\\Administrator\\Desktop\\json.xlsx";
    File multipartFile = new File(strUrl);
    InputStream inputStream = new FileInputStream(multipartFile);

    ExcelListener listener = new ExcelListener();

    ExcelReader excelReader = new ExcelReader(inputStream, ExcelTypeEnum.XLS, null, listener);

    excelReader.read(new Sheet(1, 0, Test.class));

    List<Object> list = listener.getDatas();
    if (list.size() > 1) {
        for (int i = 0; i < list.size(); i++) {
            Testobj = (Test) list.get(i);
        }
    }
} catch (Exception e) {
    System.out.println(e.getMessage());
}
try {
    String strUrl = "C:\\Users\\Administrator\\Desktop\\json"+System.currentTimeMillis()+".xlsx";
    EasyExcel.write(strUrl,Test.class).sheet("sheet").doWrite(testList);
} catch (Exception e) {
}
```
