Java
<a name="Q0bm3"></a>
## 业务需求
从一个简单的仓库业务说起，仓库业务，会有进库记录，会有出库记录，会有库存，客户的需求就是需要一个库存盘点单，盘点单通俗来讲：将库存中每个商品的出入库记录都统计出来，看看每个商品出过多少货物，入过多少货物，本月库存多少，上月库存多少。
<a name="uOLd4"></a>
## 需求难点
一个货物会出过多次货物，入过多次货物，导出的 excel 就要做成 一对多 格式的导出<br />简单举例：<br />啤酒：入库2次，出库3次，最终体现在 excel 中效果如下图：<br />![2022-11-30-09-52-01.731076400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669773202595-c272017b-b8d4-4649-a5ad-c55903251d55.png#averageHue=%23ec7c53&clientId=u2b60f31b-9896-4&from=ui&id=vVDLh&originHeight=497&originWidth=1130&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62326&status=done&style=none&taskId=u3840b105-865f-4923-ae38-017e825a5e1&title=)
<a name="SJM7t"></a>
### 通过 EasyPoi 实现需求
EasyPoi 文档地址：[http://doc.wupaas.com/docs/easypoi/easypoi-1c0u4mo8p4ro8](http://doc.wupaas.com/docs/easypoi/easypoi-1c0u4mo8p4ro8)<br />SpringBoot 使用：
```xml
<dependency>
  <groupId>cn.afterturn</groupId>
  <artifactId>easypoi-base</artifactId>
  <version>4.2.0</version>
</dependency>
<dependency>
  <groupId>cn.afterturn</groupId>
  <artifactId>easypoi-annotation</artifactId>
  <version>4.2.0</version>
</dependency>
<dependency>
  <groupId>cn.afterturn</groupId>
  <artifactId>easypoi-web</artifactId>
  <version>4.2.0</version>
</dependency>
```
Gradle 使用：
```
implementation 'cn.afterturn:easypoi-base:4.2.0'
implementation 'cn.afterturn:easypoi-annotation:4.2.0'
implementation 'cn.afterturn:easypoi-web:4.2.0'
```
使用 EasyPoi 提供的注解，自定义导出类模板
```java
import cn.afterturn.easypoi.excel.annotation.Excel;
import cn.afterturn.easypoi.excel.annotation.ExcelCollection;
import cn.afterturn.easypoi.excel.annotation.ExcelIgnore;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Date;
import java.util.List;

/**
 * 导出 excel 模板类
 */
@Getter
@Setter
public class ExportTemplate implements Serializable {
    @Excel(name = "序号", needMerge = true, type = 10)
    private int index;
    
    @Excel(name = "商品名称", needMerge = true, width = 30.0)
    private String goodName;
    
    @Excel(name = "商品单位", needMerge = true)
    private String goodUnit;
    
    @Excel(name = "上月库存数量", needMerge = true, type = 10)
    private Integer lastMonthSurplusNum;
    
    @Excel(name = "本月库存数量", needMerge = true, type = 10)
    private Integer thisMonthSurplusNum;
    
    @ExcelCollection(name = "本月入库信息")
    private List<GoodInItem> goodInItems;
    
    @ExcelCollection(name = "本月出库信息")
    private List<GoodOutItem> goodOutItems;
    
    @Excel(name = "备注", needMerge = true, width = 30.0)
    private String remark;

    /**
     * 入库信息
     */
    @Getter
    @Setter
    public static class GoodInItem {
        @Excel(name = "入库日期", exportFormat = "yyyy-MM-dd", width = 20.50)
        private Date purchaseDate;
        
        @Excel(name = "入库号", width = 25.50)
        private String purchaseNum;
        
        @Excel(name = "入库单价", type = 10)
        private BigDecimal unitPrice;
        
        @Excel(name = "入库数量", type = 10)
        private Integer totalNum;
    }

    /**
     * 出库信息
     */
    @Getter
    @Setter
    public static class GoodOutItem {
        @Excel(name = "出库日期", exportFormat = "yyyy-MM-dd", width = 20.50)
        private Date outDate;
        
        @Excel(name = "出库号", width = 25.50)
        private String sellNum;
        
        @Excel(name = "出库数量", type = 10)
        private Integer totalNum;
        
        @Excel(name = "成本金额", type = 10)
        private BigDecimal priceIn;
        
        @Excel(name = "销售金额", type = 10)
        private BigDecimal priceOut;
    }
}
```
实体类中使用的注解作用解释：

- `**@Getter**`** lombok 注解**，用于给所有属性提供 getter 方法
- `**@Setter**`** lombok 注解**，用于给所有属性提供 setter 方法
- `**@Excel**`** easypoi 注解**，name 就等于导出 excel 的列名称，width 就是宽度，type 就是这个属性的类型，1表示文本，默认也是文本，10就是数字，needMerge 表示是否纵向合并单元格，也就是上下列合并
- `**@ExcelCollection**`** easypoi 注解**，name 就等于导出 excel 的列名称，被此注解标注的集合，就等于在其列下面创建对等数量的行，就类似于这种

![2022-11-30-09-52-01.894860700.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669773202880-14a18b01-d654-40ff-a5e5-a7d00fa0fa56.png#averageHue=%23fdf7f6&clientId=u2b60f31b-9896-4&from=ui&id=u599b42bc&originHeight=390&originWidth=1185&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56361&status=done&style=none&taskId=ue5f0ccce-19e8-4c53-b285-4912e06a276&title=)<br />最后模板弄好之后，就可以通过easypoi 的工具类来导出，easypoi 推荐的导出工具类如下：<br />![2022-11-30-09-52-02.127462500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669773202678-dec33c2a-7539-46fb-8fd9-babf7d5ff508.png#averageHue=%23faf7f6&clientId=u2b60f31b-9896-4&from=ui&id=u7kYS&originHeight=764&originWidth=1156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154918&status=done&style=none&taskId=u80a9bb5f-70b5-4f1f-a9e9-794b1bda39d&title=)<br />这个方法的三个参数表示含义解释：

- **ExportParams** ：参数表示Excel 导出参数设置类，easypoi 自定义的类
- **pojoClass**：要导出的类模板
- **dataSet**：数据集合
<a name="odEvj"></a>
### 具体实现
```java
@GetMapping(value = "export")
public void export(HttpServletRequest req, HttpServletResponse resp) {
    List<ExportTemplate> exportData = new ArrayList();
    // 步骤1：构建要导出excel的数据集合
    for (int i = 0; i < 5; i++) {
        ExportTemplate data = new ExportTemplate();
        data.setIndex(i);
        data.setGoodName("测试商品");
        data.setGoodUnit("瓶");
        data.setLastMonthSurplusNum(5); // 上月库存
        data.setThisMonthSurplusNum(3); // 本月库存
        // ... 剩下的就是类似的加值
        exportData.add(data);
    }

    try {
        // 步骤2：开始导出 excel
        ExportParams params = new ExportParams();
        params.setTitle("库存盘点单标题");
        params.setSheetName("库存盘点单工作表名称");
        params.setType(ExcelType.XSSF);
        Workbook workbook = ExcelExportUtil.exportExcel(params, ExportTemplate.class, exportData);

        String nowStr = DateTimeFormatter.ofPattern(LocalDateTime.now()).format("yyyyMMddHHmm"); // 时间串
        String fileName = nowStr + "_库存盘点单"; // 文件名称
        String tempDir = "C:/Users/huxim/Downloads";
        File filePath = new File(tempDir + File.separator);
        if (!filePath.exists()) filePath.mkdirs(); // 如果文件目录不存在就创建这个目录

        FileOutputStream fos = new FileOutputStream(tempDir + File.separator + fileName);
        workbook.write(fos);
        fos.close();

        resp.setContentType("application/octet-stream");
        resp.setCharacterEncoding("utf-8");
        response.addHeader("Content-disposition", "attachment; filename="
                           + this.makeDownloadFileName(req, fileName));
        IOUtils.copy(new FileInputStream(tempFile), response.getOutputStream());
        System.out.println("导出成功~~~");
    } catch (Exception e) {
        throw new RuntimeException("导出 excel 失败~~~");
    }
}

/**
 * 判断是否是 IE 浏览器
 * 返回对应的字符串格式
 */
public static String makeDownloadFileName(HttpServletRequest request, String fileName) {
    String agent = request.getHeader("User-Agent");
    byte[] bytes = fileName.getBytes(StandardCharsets.UTF_8);
    if (agent.contains("MSIE") || agent.contains("Trident") || agent.contains("Edge")) {
        // IE
        return new String(bytes, StandardCharsets.UTF_8);
    } else {
        return new String(bytes, StandardCharsets.ISO_8859_1);
    }
}
```
导出成功后的excel 就类似于如下这种：<br />![2022-11-30-09-52-02.602419100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669773202710-97e3afe7-e502-44cc-8b0d-3b4b0d49931d.png#averageHue=%23f9f9f8&clientId=u2b60f31b-9896-4&from=ui&id=Unxri&originHeight=1016&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=242373&status=done&style=none&taskId=uad4af456-af5e-4e0d-9ed5-4cc90f0b642&title=)
