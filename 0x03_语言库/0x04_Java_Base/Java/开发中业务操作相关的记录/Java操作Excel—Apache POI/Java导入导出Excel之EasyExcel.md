Java EasyExcel
<a name="CQuZz"></a>
## EasyExcel快速开始
<a name="IuaVw"></a>
### EasyExcel官方文档
[https://www.yuque.com/easyexcel/doc/easyexcel](https://www.yuque.com/easyexcel/doc/easyexcel)
<a name="sdtAk"></a>
### 导入EasyExcel相关的依赖
```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>easyexcel</artifactId>
    <version>2.1.2</version>
</dependency>
```
<a name="I0SAC"></a>
## 导入
<a name="JIS97"></a>
### 简单导入的示例
<a name="cjcZA"></a>
#### 建立导入对应实体类
```java
@Data
public class ReqCustomerDailyImport {
    /**
     * 客户名称
     */
    @ExcelProperty(index = 0)
    private String customerName;
    /**
     * MIS编码
     */
    @ExcelProperty(index = 1)
    private String misCode;
    /**
     * 月度滚动额
     */
    @ExcelProperty(index = 3)
    private BigDecimal monthlyQuota;
    /**
     * 最新应收账款余额
     */
    @ExcelProperty(index = 4)
    private BigDecimal accountReceivableQuota;
    /**
     * 本月利率(年化）
     */
    @ExcelProperty(index = 5)
    private BigDecimal dailyInterestRate;
}
```
<a name="sI4RJ"></a>
#### Controller代码
```java
@PostMapping("/import")
public void importCustomerDaily(@RequestParam MultipartFile file) throws IOException {
    InputStream inputStream = file.getInputStream();
    List<ReqCustomerDailyImport> reqCustomerDailyImports = EasyExcel.read(inputStream)
            .head(ReqCustomerDailyImport.class)
            // 设置sheet,默认读取第一个
            .sheet()
            // 设置标题所在行数
            .headRowNumber(2)
            .doReadSync();
}
```
<a name="Uid2u"></a>
#### 运行结果
在实体对象使用`@ExcelProperty`注解，读取时指定该class，即可读取，并且自动过滤了空行，对于excel的读取及其简单。不过此时发现一个问题，
<a name="6riln"></a>
### 导入时校验字段，字段类型转换
```java
List<ReqCustomerDailyImport> reqCustomerDailyImports = EasyExcel.read(inputStream)
            // 这个转换是成全局的， 所有java为string,excel为string的都会用这个转换器。
            // 如果就想单个字段使用请使用@ExcelProperty 指定converter
            .registerConverter(new StringConverter())
            // 注册监听器，可以在这里校验字段
            .registerReadListener(new CustomerDailyImportListener())
            .head(ReqCustomerDailyImport.class)
            .sheet()
            .headRowNumber(2)
            .doReadSync();
}
```
<a name="a3x9P"></a>
#### 监听器
```java
public class CustomerDailyImportListener extends AnalysisEventListener {
    List misCodes = Lists.newArrayList();
    /**
     * 每解析一行，回调该方法
     * @param data
     * @param context
     */
    @Override
    public void invoke(Object data, AnalysisContext context) {
        String misCode = ((ReqCustomerDailyImport) data).getMisCode();
        if (StringUtils.isEmpty(misCode)) {
            throw new RuntimeException(String.format("第%s行MIS编码为空，请核实", context.readRowHolder().getRowIndex() + 1));
        }
        if (misCodes.contains(misCodes)) {
            throw new RuntimeException(String.format("第%s行MIS编码已重复，请核实", context.readRowHolder().getRowIndex() + 1));
        } else {
            misCodes.add(misCode);
        }
    }
    /**
     * 出现异常回调
     * @param exception
     * @param context
     * @throws Exception
     */
    @Override
    public void onException(Exception exception, AnalysisContext context) throws Exception {
        // ExcelDataConvertException:当数据转换异常的时候，会抛出该异常，此处可以得知第几行，第几列的数据
        if (exception instanceof ExcelDataConvertException) {
            Integer columnIndex = ((ExcelDataConvertException) exception).getColumnIndex() + 1;
            Integer rowIndex = ((ExcelDataConvertException) exception).getRowIndex() + 1;
            String message = "第" + rowIndex + "行，第" + columnIndex + "列" + "数据格式有误，请核实";
            throw new RuntimeException(message);
        } else if (exception instanceof RuntimeException) {
            throw exception;
        } else {
            super.onException(exception, context);
        }
    }
    /**
     * 解析完全部回调
     * @param context
     */
    @Override
    public void doAfterAllAnalysed(AnalysisContext context) {
        misCodes.clear();
    }
}
```
<a name="3Rd2N"></a>
#### 转换器
```java
public class StringConverter implements Converter<String> {
    @Override
    public Class supportJavaTypeKey() {
        return String.class;
    }
    @Override
    public CellDataTypeEnum supportExcelTypeKey() {
        return CellDataTypeEnum.STRING;
    }
    /**
     * 将excel对象转成Java对象，这里读的时候会调用
     *
     * @param cellData            NotNull
     * @param contentProperty     Nullable
     * @param globalConfiguration NotNull
     * @return
     */
    @Override
    public String convertToJavaData(CellData cellData, ExcelContentProperty contentProperty,
                                    GlobalConfiguration globalConfiguration) {
        return "自定义：" + cellData.getStringValue();
    }
    /**
     * 将Java对象转成String对象，写出的时候调用
     *
     * @param value
     * @param contentProperty
     * @param globalConfiguration
     * @return
     */
    @Override
    public CellData convertToExcelData(String value, ExcelContentProperty contentProperty,
                                       GlobalConfiguration globalConfiguration) {
        return new CellData(value);
    }
}
```
可以看出注册了一个监听器：`CustomerDailyImportListener`,还注册了一个转换器：`StringConverter`。流程为：框架读取一行数据，先执行转换器，当一行数据转换完成，执行监听器的回调方法，如果转换的过程中，出现转换异常，也会回调监听器中的`onException`方法。因此，可以在监听器中校验数据，在转换器中转换数据类型或者格式。
<a name="RgMi1"></a>
### 导入相关常用API
<a name="yh7B5"></a>
#### 注解

- `ExcelProperty` 指定当前字段对应excel中的那一列。可以根据名字或者I`index`去匹配。当然也可以不写，默认第一个字段就是`index=0`，以此类推。千万注意，要么全部不写，要么全部用`index`，要么全部用名字去匹配。千万别三个混着用，除非了解源代码中三个混着用怎么去排序的。
- `ExcelIgnore` 默认所有字段都会和excel去匹配，加了这个注解会忽略该字段。
- `DateTimeFormat` 日期转换，用`String`去接收excel日期格式的数据会调用这个注解。里面的`value`参照`java.text.SimpleDateFormat`。
- `NumberFormat` 数字转换，用`String`去接收excel数字格式的数据会调用这个注解。里面的`value`参照`java.text.DecimalFormat`。
<a name="GZkvY"></a>
#### EasyExcel相关参数

- `readListener` 监听器，在读取数据的过程中会不断的调用监听器。
- `converter` 转换器，默认加载了很多转换器。也可以自定义，如果使用的是`registerConverter`，那么该转换器是全局的，如果要对单个字段生效，可以在`ExcelProperty`注解的`converter`指定转换器。
- `headRowNumber` 需要读的表格有几行头数据。默认有一行头，也就是认为第二行开始起为数据。
- `head `与`clazz`二选一。读取文件头对应的列表，会根据列表匹配数据，建议使用`class`。
- `autoTrim` 字符串、表头等数据自动`trim`。
- `sheetNo` 需要读取Sheet的编码，建议使用这个来指定读取哪个Sheet。
- `sheetName` 根据名字去匹配Sheet,excel 2003不支持根据名字去匹配。
<a name="4bP6S"></a>
## 导出
<a name="nt9zU"></a>
### 导出的简单示例
<a name="d6qiQ"></a>
#### 建立导出对应实体类
```java
@Data
@Builder
public class RespCustomerDailyImport {
    @ExcelProperty("客户编码")
    private String customerName;
    @ExcelProperty("MIS编码")
    private String misCode;
    @ExcelProperty("月度滚动额")
    private BigDecimal monthlyQuota;
    @ExcelProperty("最新应收账款余额")
    private BigDecimal accountReceivableQuota;
    @NumberFormat("#.##%")
    @ExcelProperty("本月利率(年化）")
    private BigDecimal dailyInterestRate;
}
```
<a name="dn8NG"></a>
#### Controller代码
```java
@GetMapping("/export")
public void export(HttpServletResponse response) throws IOException {
    // 生成数据
    List<RespCustomerDailyImport> respCustomerDailyImports = Lists.newArrayList();
    for (int i = 0; i < 50; i++) {
        RespCustomerDailyImport respCustomerDailyImport = RespCustomerDailyImport.builder()
                .misCode(String.valueOf(i))
                .customerName("customerName" + i)
                .monthlyQuota(new BigDecimal(String.valueOf(i)))
                .accountReceivableQuota(new BigDecimal(String.valueOf(i)))
                .dailyInterestRate(new BigDecimal(String.valueOf(i))).build();
        respCustomerDailyImports.add(respCustomerDailyImport);
    }
    
    response.setContentType("application/vnd.ms-excel");
    response.setCharacterEncoding("utf-8");
    // 这里URLEncoder.encode可以防止中文乱码 当然和easyexcel没有关系
    String fileName = URLEncoder.encode("导出", "UTF-8");
    response.setHeader("Content-disposition", "attachment;filename=" + fileName + ".xlsx");
    EasyExcel.write(response.getOutputStream(), RespCustomerDailyImport.class)
            .sheet("sheet0")
            // 设置字段宽度为自动调整，不太精确
            .registerWriteHandler(new LongestMatchColumnWidthStyleStrategy())
            .doWrite(respCustomerDailyImports);
}
```
<a name="mV8Jn"></a>
### 导出相关常用API
<a name="BFN2L"></a>
#### 注解

- `ExcelProperty` 指定写到第几列，默认根据成员变量排序。`value`指定写入的名称，默认成员变量的名字。
- `ExcelIgnore` 默认所有字段都会写入excel，这个注解会忽略这个字段。
- `DateTimeFormat` 日期转换，将`Date`写到excel会调用这个注解。里面的`value`参照`java.text.SimpleDateFormat`。
- `NumberFormat` 数字转换，用`Number`写excel会调用这个注解。里面的`value`参照`java.text.DecimalFormat`。
<a name="wVtnR"></a>
#### EasyExcel相关参数

- `needHead` 监听器是否导出头。
- `useDefaultStyle` 写的时候是否是使用默认头。
- `head` 与`clazz`二选一。写入文件的头列表，建议使用`class`。
- `autoTrim` 字符串、表头等数据自动`trim`。
- `sheetNo` 需要写入的编码。默认0。
- `sheetName` 需要些的Sheet名称，默认同sheetNo。
<a name="PGL0T"></a>
## 总结
可以看出不管是excel的读取还是写入，都是一个注解加上一行代码完成，可以少很多解析的代码，极大减少了重复的工作量。EasyExcel还支持更多场景，例如读，可以读多个sheet，也可以解析一行数据或者多行数据做一次入库操作；写操作中，支持复杂头，指定列写入，重复多次写入，多个sheet写入，根据模板写入等等。更多的例子可以参考EasyExcel官方文档。
