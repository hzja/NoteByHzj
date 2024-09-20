Java
<a name="Jrvyq"></a>
## 功能介绍
<a name="mevEW"></a>
### IMPORT

- `**ExcelHandle**`** 核心处理器**
- `**ExcelWorkbookManage**`** excel所有工作表管理**
- `**ExcelInitConfig**`** 配置文件初始化**
- `**AbstractFileParser**`** 文件转换类**
<a name="WdbSy"></a>
### alanpoi import有何优势？

- **用户不需要额外引入poi等繁琐的jar**
- **毫秒级解析大文件，支持一键解析多sheet页签，不需要自己按照一定的格式循环匹配解析所有数据**
- **不管系统多么复杂，有多少个导入，alanpoi全部支持，而且准确返回需要的对象，减轻开发者工作量**
- **目前外界业务越来越复杂，对各个功能要求也越来越严格，当然导入也不例外，alanpoi支持错误一键回写到excel，对应到每一行**
- **alanpoi灵活可扩展，提供了**`**ExcelConsumeInterface**`**接口，可继承它，实现**`**valid**`**、**`**error**`**、**`**end**`**三个方法编写自己的业务**
   - **valid：方法参数返回excel所有数据，用户可进行自我校验**
   - **error：导入错误会回调**
   - **end：方法参数返回校验成功的数据，valid校验失败的数据不会返回，用户可以自己操作持久化或者其他业务**
<a name="D7RPG"></a>
## 怎么使用alanpoi实现导入
项目中使用：
```xml
<dependency>
    <groupId>com.alanpoi</groupId>
    <artifactId>alanpoi-analysis</artifactId>
    <version>1.3.0</version>
</dependency>
```
简单一句话：一配置一继承一调用
<a name="cRxQq"></a>
### 一配置
在项目resources目录中新建excel-config.xml文件，cosume中配置自己的消费类路径，继承`ExcelConsumeInterface`接口，sheet中的vo是把当前sheet序列化的对象路径，column中当然就是配置vo中的属性了。<br />其中name可选字段，填了就是按照这个匹配excel列名，不填就是按照offset顺序；导入包含多个sheet就配置多个
```xml
<?xml version = "1.0" encoding = "GB2312"?>
<exg name="excelId" version="1.0" file-type="excel">
	<excel id="ACCOUNT" consume="com.xxx.FinAccountImportHandler">
		<sheet index="0" row-start="1" column-start="0"
					 vo="com.xxx.vo.FinAccountImportVO">
			<column name="公司/供应商编号" offset="1">companyCode</column>
			<column name="公司/供应商名称" offset="2">companyName</column>
			<column name="银行账号" offset="3">bankAccount</column>
			<column name="开户银行" offset="4">bankName</column>
		</sheet>
	</excel>
</exg>
```
<a name="tzQYs"></a>
### 一继承
`consume`类继承`ExcelConsumeInterface`接口，实现方法
```java
/**
 * when error will 调用
 *
 * @param excelError
 */
void error(ExcelError excelError);

/**
 * custom valid data
 *
 * @param workbookId
 * @param sheetDataList
 */
void validData(String workbookId, List<ExcelSheetData> sheetDataList, Map<Serializable, Object> excelParam);

/**
 * @param sheetDataList return success data
 */
void end(List<ExcelSheetData> sheetDataList, Map<Serializable, Object> excelParam);
```
<a name="EXb4v"></a>
### 一调用
用户调用`ExcelExportUtil`类的`customImportData`即可，参数excelId就是excel-conifg.xml中配置的id
<a name="cAaRZ"></a>
## Export
<a name="NHmZ5"></a>
### 描叙
能够用一行代码实现绝不用第二行，如果一行不行，那就再加一行！
<a name="jeZuS"></a>
### 模式
注解模式导出：

- `**ExcelSheet**`**注解：用于导入类上，可制定sheet名，列头的颜色、字体、高度、宽度**
- `**ExcelColum**`**注解：用于导入类的属性上，可指定列头的名称，单元格的样式**
- `**DateFormat**`**注解：用于导入类的属性上, 可以按照指定格式输出到excel,默认"yyyy/MM/dd"**
- `**NumFormat**`**注解：用于导入类的属性上，可以按照指定格式输出到excel,默认"00.00"**

样例：
```java
@ExcelSheet(name = "测试", backColor = AlanColors.GREEN, font = "宋体", fontSize = 25)
@Data
public class ExportVO {
    @ExcelColumn(name = "名称", width = 32, link = "${url}")
    private String name;

    @ExcelColumn(name = "值")
    private String value;

    @ExcelColumn(name = "金额")
    @NumFormat(value = "0000.00##")
    private BigDecimal amount;

    @ExcelColumn(name = "时间格式化")
    @DateFormat(value = "yyyy-MM-dd hh:mm:ss")
    private Date dateTime;

    @DateFormat
    @ExcelColumn(name = "日期格式化")
    private java.sql.Date date;
    
    @ExcelColumn(isExist = false)
    private String url;
}
```
<a name="GyKhs"></a>
### 使用
<a name="S8u50"></a>
#### 方式一、直接导出到浏览器
```java
ExcelExportUtil.export(Colletion<?>,Class,HttpServletRequest,HttpServletResponse,fileName);
```
<a name="aQnic"></a>
#### 方式二、调用`getWorkbook`获取工作表，自行处理`workbook`
```java
ExcelExportUtil.getWorkbook(Collection<?> singleSheetData, Class<?> c)
```
<a name="iNJHX"></a>
### 高级使用
<a name="pKj9T"></a>
#### 示例一：导出指定列（动态导出列）
```java
List<ExportVO> list = new ArrayList<>();
for (int i = 0; i < 500; i++) {
    ExportVO exportVO = new ExportVO();
    exportVO.setName("name" + i);
    exportVO.setValue(new BigDecimal(123.11 + i * 0.09));
    exportVO.setAmount(new BigDecimal(6666.666 + i * 10));
    exportVO.setDate(new Date(132324343 + i * 100));
    exportVO.setDateTime(new java.util.Date());
    list.add(exportVO);
}
List<String> colList = new ArrayList<>();
//按照顺序仅导出add的列
colList.add("name");
colList.add("value");
//调用获取workbook对象；也可以直接调用exportSpecifyCol方法导出到浏览器
Workbook workbook = ExcelExportUtil.getWorkbookSpecifyCol(list, ExportVO.class, colList);
```
<a name="gm4J6"></a>
#### 示例二：多sheet页签导出
```java
List<ExportVO> list = new ArrayList<>();
List<Export2VO> list2 = new ArrayList<>();
for (int i = 0; i < 500; i++) {
	ExportVO exportVO = new ExportVO();
	exportVO.setName("name" + i);
	exportVO.setValue(new BigDecimal(123.11 + i * 0.09));
	exportVO.setAmount(new BigDecimal(6666.666 + i * 10));
	exportVO.setDate(new Date(132324343 + i * 100));
	exportVO.setDateTime(new java.util.Date());
	list.add(exportVO);
	Export2VO export2VO = new Export2VO();
	export2VO.setName("name" + i);
	export2VO.setValue("value" + i);
	export2VO.setAmount(new BigDecimal(6666.666 + i * 10));
	export2VO.setDate(new Date(132324343 + i * 100));
	export2VO.setDateTime(new java.util.Date());
	list2.add(export2VO);
}
Map<Class<?>, Collection<?>> map = new HashMap<>();
map.put(ExportVO.class, list);
map.put(Export2VO.class, list2);
//调用获取workbook对象；也可以直接调用exportByMultiSheet方法导出到浏览器
Workbook workbook = ExcelExportUtil.getWorkbookByMultiSheet(map);
```
代码已经开源，地址：[https://github.com/alan-et/alanpoi/tree/develop/alanpoi-analysis](https://github.com/alan-et/alanpoi/tree/develop/alanpoi-analysis)
