JavaPOIExcel
<a name="OCJ9y"></a>
## 1、前言
poi框架可以支持在Java代码中，将数据导出成Excel，在实际开发中，往往还需要设置Excel字体，颜色，行高，列宽等属性，有时候还需要锁住单元格，防止别人将数据随意篡改。
<a name="IRa2U"></a>
## 2、锁住单元格
导出Excel，自然就有导入Excel了，比如导出一些数据出来，修改一些再导入进去，但是这时，一些基本信息不希望用户随意去修改，这里就用到了Excel的锁
```java
sheet.protectSheet(密码)
```
代码:
```java
// 创建Excel文件
HSSFWorkbook workbook = new HSSFWorkbook();
HSSFSheet sheet = workbook.createSheet(DateUtils.getDate("yyyyMMdd"));
//锁定sheet
sheet.protectSheet("zgd");
```
这样的话，这个sheet都会被锁定<br />但是又希望开放一些单元格可以修改，这个时候就要细粒度的进行设置了<br />创建一个cellStyle:
```java
 CellStyle unlockCell = workbook.createCellStyle();
 unlockCell.setLocked(false);
```
然后在不需要锁定的单元格上，给它这个 cellStyle
```java
 // 设置dataRow这一行的第i个单元格不锁定
 dataRow.getCell(i).setCellStyle(unlockCell);
```
**设置列宽 在锁定了sheet之后，会发现一个问题，就是列宽都不能改变了**<br />这个时候没办法，只能自己设置列宽了，现在网上找到的设置列宽的方法有以下几个:
<a name="W9wAD"></a>
### 1、自适应列宽度
```java
sheet.autoSizeColumn(1);
sheet.autoSizeColumn(1, true);
```
这两种方式都是自适应列宽度，但是注意这个方法在后边的版本才提供，poi的版本不要太老。注意：第一个方法在合并单元格的的单元格并不好使，必须用第二个方法。<br />经过测试，这种自适应的api在遇到行数多一点的数据的时候，就会耗费大量的时间，1000行花了2分钟!!!所以尽量不要用
```java
sheet.trackAllColumnsForAutoSizing();
sheet.autoSizeColumn(i);
```
而且这两个方法对英文数字还好，对中文支持的并不好。
<a name="gwVLo"></a>
### 2、用数组将大概的宽度设置好，手动set宽度
```java
int[] width = {xxx,xxx};
for循环
sheet.setColumnWidth(i,width[i]);
```
<a name="boHjT"></a>
### 3、自己根据一列数据中的最长的字符串长度设置宽度
所以还是得自己费心费力去diy :<br />判断这一列的最长字符串，然后
```java
int length = str.getBytes().length;
sheet.setColumnWidth((short)列数,(short)(length*256));
```
这里经过反复尝试，把最大宽度限制在10000到15000左右是比较合适的，然后剩下的就交给Excel的自动换行<br />像这里有很多行的数据，不知道哪一行的内容最长，这里简单提供两种思路(方法是很多的，能达到目的就行)：

1. **用一个**`**Map<Integer, List>**`**，key是指具体哪一列，List中放的是每行的这一列的内容的长度，每遍历一行的一列, 就**`**map.put(i, list.add(length))**`**，然后用**`**Collections.max(map.get(i))**`**来获取第i列的最长的长度。**
2. **还是一样，用一个map: **`**Map<Integer, Integer>**`**，key是指具体哪一列，value是每行的这一列的内容的长度，**`**map.put(i,Math.max(length,map.get(i)))**`**，来确保map中的key对应的value永远是目前的最大的长度。**

这里使用的第二种：<br />设置自动换行后，不要设置固定的行高，否则超出的部分也会被遮住不显示
```java
// 创建Excel文件
HSSFWorkbook workbook = new HSSFWorkbook();
HSSFSheet sheet = workbook.createSheet("sheet");
//设置样式
CellStyle blackStyle = workbook.createCellStyle();
//自动换行*重要*
blackStyle.setWrapText(true);

//存储最大列宽
Map<Integer,Integer> maxWidth = new HashMap<>();
// 标题行
HSSFRow titleRow = sheet.createRow(0);
titleRow.setHeightInPoints(20);//目的是想把行高设置成20px
titleRow.createCell(0).setCellValue("sku编号");
titleRow.createCell(1).setCellValue("商品标题");
titleRow.createCell(2).setCellValue("商品名");
// 初始化标题的列宽,字体
for (int i= 0; i<=3;i++){
    maxWidth.put(i,titleRow.getCell(i).getStringCellValue().getBytes().length  * 256 + 200);
    titleRow.getCell(i).setCellStyle(blackStyle);//设置自动换行
}

for (Map<String, Object> map : list) {
    int currentRowNum = sheet.getLastRowNum() + 1;
    //数据行
    HSSFRow dataRow = sheet.createRow(currentRowNum);
    // 记录这一行的每列的长度
    List<Object> valueList = new ArrayList<Object>();

    String val0 = map.get("skuId") == null ? "—" : ((Double) (map.get("skuId"))).intValue()+"";
    valueList.add(val0);
    dataRow.createCell(0).setCellValue(val0);
    String val1 = map.get("title") == null ? "" : map.get("title").toString();
    valueList.add(val1);
    dataRow.createCell(1).setCellValue(val1);
    String val2 = map.get("goodsName") == null ? "" : map.get("goodsName").toString();
    valueList.add(val2);
    dataRow.createCell(2).setCellValue(val2);
    String val3 = map.get("catName") == null ? "" : map.get("catName").toString();
    valueList.add(val3);
    dataRow.createCell(3).setCellValue(val3);
    String val4 = map.get("brandName") == null ? "" : map.get("brandName").toString();

    for(int i = 0;i<=3;i++){
        int length = valueList.get(i).toString().getBytes().length  * 256 + 200;
        //这里把宽度最大限制到15000
        if (length>15000){
            length = 15000;
        }
        maxWidth.put(i,Math.max(length,maxWidth.get(i)));
        dataRow.getCell(i).setCellStyle(blackStyle);//设置自动换行
    }
}


for (int i= 0; i<=3;i++){
    //设置列宽
    sheet.setColumnWidth(i,maxWidth.get(i));
}
```
现在的话，列宽虽然是比较生硬的套用内容长度来设置，不过也比之前好多了，列宽是不能超过256*256的，否则会报错，所以这里设置的最大列宽为15000，超出的部分会自动换行
<a name="kgK6r"></a>
### 4、设置行高
行高就很简单了
```java
titleRow.setHeightInPoints(20);//目的是想把行高设置成20px
```
注意，设置了固定行高，自动换行就不会自适应行高了
<a name="mX0pU"></a>
### 5、设置字体，颜色
创建CellStyle，然后创建HSSFFont，再把HSSFFont注入给CellStyle，在把CellStyle给cell设置
```java
// 设置字体
CellStyle redStyle = workbook.createCellStyle();
HSSFFont redFont = workbook.createFont();
//颜色
redFont.setColor(Font.COLOR_RED);
//设置字体大小
redFont.setFontHeightInPoints((short) 10);
//字体
//redFont.setFontName("宋体");
redStyle.setFont(redFont);

HSSFCell cell13 = titleRow.createCell(13);
cell13.setCellStyle(redStyle);
cell13.setCellValue("注意:只允许修改销售价,供应价,市场价和库存");
```
<a name="CcgVX"></a>
### 6、合并单元格
合并单元格的话，建议先合并，合并之后，在合并的第一行第一列set值就可以了
```java
//这里代表在第0行开始,到0行结束,从0列开始,到10列结束,进行合并,也就是合并第0行的0-10个单元格
CellRangeAddress cellRange1 = new CellRangeAddress(0, 0, (short) 0, (short) 10);
sheet.addMergedRegion(cellRange1);
CellRangeAddress
```
