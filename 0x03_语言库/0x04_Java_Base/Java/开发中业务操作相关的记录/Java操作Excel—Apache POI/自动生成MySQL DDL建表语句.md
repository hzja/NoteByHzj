Java
<a name="LDXcS"></a>
### 简介
项目中大部分情况下都是使用MySQL数据，而且主要使用的数据库类型是char、varchar、date、datetime、timestamp、int、tinyint等几种常见的数据类型；而且进行表设计时，一般都要出一份表设计文档，例如表设计模板如下<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1671288644781-5c4fb214-e36f-4ff6-abcb-0096c51e8560.png#averageHue=%23f2f2f1&clientId=uebbcc1a1-62c4-4&from=paste&id=ufc35741f&originHeight=313&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9c2a168e-5000-4f08-ae0c-47a21025a61&title=)<br />因为有了如此标准的表设计文档，就可以通过解析xlsx生成数据的建表语句。
<a name="wY5Fj"></a>
### pom文件引入
主要引入解析xlsx的poi、日志的logback类库
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.table</groupId>
  <artifactId>auto-table</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <properties>
    <maven.compiler.source>8</maven.compiler.source>
    <maven.compiler.target>8</maven.compiler.target>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>
  <dependencies>
    <!-- poi导入导出 -->
    <dependency>
      <groupId>org.apache.poi</groupId>
      <artifactId>poi</artifactId>
      <version>4.1.2</version>
    </dependency>
    <dependency>
      <groupId>org.apache.poi</groupId>
      <artifactId>poi-ooxml</artifactId>
      <version>4.1.2</version>
    </dependency>
    <dependency>
      <groupId>org.apache.poi</groupId>
      <artifactId>poi-ooxml-schemas</artifactId>
      <version>4.1.2</version>
    </dependency>
    <dependency>
      <groupId>org.apache.commons</groupId>
      <artifactId>commons-lang3</artifactId>
      <version>3.12.0</version>
    </dependency>
    <dependency>
      <groupId>cn.hutool</groupId>
      <artifactId>hutool-all</artifactId>
      <version>5.7.12</version>
    </dependency>
    <dependency>
      <groupId>org.slf4j</groupId>
      <artifactId>slf4j-api</artifactId>
      <version>1.7.7</version>
    </dependency>
    <dependency>
      <groupId>ch.qos.logback</groupId>
      <artifactId>logback-core</artifactId>
      <version>1.1.7</version>
    </dependency>
    <dependency>
      <groupId>ch.qos.logback</groupId>
      <artifactId>logback-access</artifactId>
      <version>1.1.7</version>
    </dependency>
    <dependency>
      <groupId>ch.qos.logback</groupId>
      <artifactId>logback-classic</artifactId>
      <version>1.1.7</version>
    </dependency>
  </dependencies>
</project>
```
<a name="GObKe"></a>
### 创建表实体和字段实体
表实体对象TableScriptDO
```java
package com.auto.table;

import java.io.Serializable;
import java.util.List;

import cn.hutool.json.JSONUtil;

public class TableScriptDO implements Serializable {

   /**
    * 表名
    */
   private String tableName;

   /**
    * 表注释
    */
   private String tableComment;

   private List<ColumnScriptDO> columnScriptDOs;

   //省略 getter、setter

}
```
字段实体对象ColumnScriptDO
```java
package com.auto.table;

import java.io.Serializable;

import cn.hutool.json.JSON;
import cn.hutool.json.JSONUtil;

public class ColumnScriptDO implements Serializable {

   /**
    * 列名
    */
   private String columnName;

   /**
    * 列注释
    */
   private String columnComment;

   /**
    * 主键名，主要有只说明是主键
    */
   private String primaryKeyName;

   /**
    * 是否自增
    */
   private boolean autoIncrease = false;

   /**
    * 字段类型
    */
   private String type;

   /**
    * 字段长度
    */
   private String length;

   /**
    * 是否可以为空，不能为空
    */
   private boolean canNull = false;

   /**
    * 默认值
    */
   private String defaultValue = "";

   //省略 getter、setter
}
```
<a name="bSoMk"></a>
### 解析电子表格xlsx
主要使用poi的接口解析表结构设计文档
```java
package com.auto.table;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.CellType;

import cn.hutool.core.collection.CollUtil;

public class AutoTableXlsxUtils {

    private static final Logger log = LoggerFactory.getLogger(AutoTableXlsxUtils.class);

    /**
    * 获取workbook
    * 
    * @param fileName
    * @return
    * @throws IOException
    */
    public static XSSFWorkbook getXSSFWorkbook(String fileName) throws IOException {
        URL url = AutoTableMain.class.getClassLoader().getResource(fileName);
        InputStream input = url.openStream();
        // 读取后缀为“xlsx”的excel文件代码 （“xls”不适用，“xls”得用 HSSFWorkbook）
        XSSFWorkbook wb = new XSSFWorkbook(input);
        return wb;
    }

    /**
    * 获取table脚本对象
    * 
    * @param wb
    * @return
    */
    public static List<TableScriptDO> getTableScript(XSSFWorkbook wb) {
        List<TableScriptDO> list = new ArrayList<>();
        int sheetCount = wb.getNumberOfSheets();
        for (int i = 0; i < sheetCount; i++) {
            XSSFSheet sheet = wb.getSheetAt(i);
            TableScriptDO tableScriptDO = buildTableScriptFromSheet(sheet);
            if (tableScriptDO == null) {
                continue;
            }
            list.add(tableScriptDO);
        }
        return list;
    }

    /**
    * 
    * @param sheet
    * @return
    */
    private static TableScriptDO buildTableScriptFromSheet(XSSFSheet sheet) {
        if ("目录".equals(sheet.getSheetName())) {
            return null;
        }
        // 获取表名
        XSSFRow tableNameRow = sheet.getRow(1);
        XSSFCell tableNameCell = tableNameRow.getCell(2);
        String tableName = tableNameCell.getStringCellValue();
        if (StringUtils.isEmpty(tableName)) {
            return null;
        }
        TableScriptDO tableScriptDO = new TableScriptDO();
        tableScriptDO.setTableName(tableName);
        // 获取表注释
        XSSFRow tableCommentRow = sheet.getRow(2);
        XSSFCell tableCommentCell = tableCommentRow.getCell(0);
        String tableComment = tableCommentCell.getStringCellValue();
        if (StringUtils.isEmpty(tableComment)) {
            tableComment = sheet.getSheetName();
        }
        tableScriptDO.setTableComment(tableComment);
        List<ColumnScriptDO> columnScriptDOs = processColumnScript(sheet);
        if (CollUtil.isEmpty(columnScriptDOs)) {
            return null;
        }
        tableScriptDO.setColumnScriptDOs(columnScriptDOs);
        return tableScriptDO;
    }

    private static List<ColumnScriptDO> processColumnScript(XSSFSheet sheet) {
        int lastRowNum = sheet.getLastRowNum()+1;
        List<ColumnScriptDO> list = new ArrayList<>();
        for (int startRowNum = 4; startRowNum < lastRowNum; startRowNum++) {
            XSSFRow columnRow = sheet.getRow(startRowNum);
            XSSFCell columnNameCell = columnRow.getCell(1);
            XSSFCell columnCommentCell = columnRow.getCell(2);
            XSSFCell primaryKeyNameCell = columnRow.getCell(3);
            XSSFCell autoIncreaseCell = columnRow.getCell(4);
            XSSFCell typeCell = columnRow.getCell(5);
            XSSFCell lengthCell = columnRow.getCell(6);
            XSSFCell canNullCell = columnRow.getCell(7);
            XSSFCell defaultValueCell = columnRow.getCell(8);

            ColumnScriptDO columnScriptDO = new ColumnScriptDO();
            // 字段名
            String columnName = columnNameCell.getStringCellValue();
            if (StringUtils.isEmpty(columnName)) {
                log.info("columnName is empty");
                continue;
            }
            columnScriptDO.setColumnName(columnName.toLowerCase());
            // 字段注释
            columnScriptDO.setColumnComment(columnCommentCell.getStringCellValue());
            // 是否主键
            String primaryKeyName = primaryKeyNameCell.getStringCellValue();
            if (StringUtils.isNotEmpty(primaryKeyName)) {
                primaryKeyName = primaryKeyName.trim().toLowerCase();
            }
            columnScriptDO.setPrimaryKeyName(primaryKeyName);
            // 是否自增
            String autoIncrease = autoIncreaseCell.getStringCellValue();
            if (StringUtils.isNotEmpty(autoIncrease) && "y".equals(autoIncrease.trim().toLowerCase())) {
                columnScriptDO.setAutoIncrease(true);
            }
            // 字段类型
            String type = typeCell.getStringCellValue();
            if (!checkTypeVaild(type)) {
                log.info("type is not vaild");
                continue;
            }
            columnScriptDO.setType(type.trim().toLowerCase());
            // 长度
            String length = getCellValue(lengthCell);
            if (StringUtils.isEmpty(length)) {
                length = "";
            }
            length = length.trim();
            columnScriptDO.setLength(length);
            // 是否可以为空
            String canNull = canNullCell.getStringCellValue();
            if (StringUtils.isNotEmpty(canNull) && "y".equals(canNull.trim().toLowerCase())) {
                columnScriptDO.setCanNull(true);
            }
            // 默认值
            String defaultValue = getCellValue(defaultValueCell);
            if (StringUtils.isEmpty(defaultValue)) {
                defaultValue = "";
            }
            defaultValue = defaultValue.trim();
            columnScriptDO.setDefaultValue(defaultValue);

            list.add(columnScriptDO);
        }
        return list;
    }

    private static boolean checkTypeVaild(String type) {
        if (StringUtils.isEmpty(type)) {
            return false;
        }
        // TODO 其他类型判断

        return true;
    }

    public static String getCellValue(Cell cell) {
        String cellValue = "";
        if (cell == null) {
            return cellValue;
        }
        // 把数字当成String来读，避免出现1读成1.0的情况
        if (cell.getCellType() == CellType.NUMERIC) {
            cell.setCellType(CellType.STRING);
        }
        // 判断数据的类型
        switch (cell.getCellType()) {
            case NUMERIC: // 数字
                cellValue = String.valueOf(cell.getNumericCellValue());
                break;
            case STRING: // 字符串
                cellValue = String.valueOf(cell.getStringCellValue());
                break;
            case BOOLEAN: // Boolean
                cellValue = String.valueOf(cell.getBooleanCellValue());
                break;
            case FORMULA: // 公式
                cellValue = String.valueOf(cell.getCellFormula());
                break;
            case BLANK: // 空值
                cellValue = "";
                break;
            case ERROR: // 故障
                cellValue = "非法字符";
                break;
            default:
                cellValue = "未知类型";
                break;
        }
        return cellValue;
    }

}
```
<a name="DW7R5"></a>
### 生成MySQL建表DDL语句
通过解析xlsx后得到表**实体对象TableScriptDO**和**字段实体对象ColumnScriptDO**
```java
package com.auto.table;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import cn.hutool.core.collection.CollUtil;
import cn.hutool.core.io.FileUtil;

public class AutoTableScriptGenerator {

    private static final Logger log = LoggerFactory.getLogger(AutoTableScriptGenerator.class);

    private static String DROP_TABLE = "DROP TABLE IF EXISTS %s;\n";

    /**
    * 前缀
    */
    private static String TABLE_PREFIX = "CREATE TABLE `%s` (\n";

    /**
    * 后缀
    */
    private static String SUFFIX_PREFIX = ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci COMMENT='%s';\n";

    public static void geneateScript(List<TableScriptDO> list, String sqlFileName) {
        if(CollUtil.isEmpty(list)) {
            return;
        }
        if(sqlFileName==null) {
            sqlFileName = "gen.sql";
        }
        if(!sqlFileName.toLowerCase().endsWith(".sql")) {
            sqlFileName = sqlFileName + ".sql";
        }
        List<String> tableScriptList = new ArrayList<>();
        for(int i=0; i<list.size(); i++) {
            TableScriptDO tableScriptDO = list.get(i);
            String tableScript = String.format(DROP_TABLE, tableScriptDO.getTableName());
            List<ColumnScriptDO> tableScriptDOs = tableScriptDO.getColumnScriptDOs();
            if(CollUtil.isEmpty(tableScriptDOs)) {
                continue;
            }

            String tableCreateScript = String.format(TABLE_PREFIX, tableScriptDO.getTableName());
            tableScript = tableScript + tableCreateScript;

            //处理中间的sql脚本生成
            List<ColumnScriptDO> columnScriptDOs = tableScriptDO.getColumnScriptDOs();
            if(CollUtil.isEmpty(columnScriptDOs)) {
                continue;
            }
            //所有字段
            String columns = "";
            String primaryKeyColumn = "";
            for(ColumnScriptDO columnScriptDO : columnScriptDOs) {
                StringBuffer columnScript = new StringBuffer();
                //字段名称
                columnScript.append("    `");
                columnScript.append(columnScriptDO.getColumnName().toLowerCase());
                columnScript.append("` ");

                //字段类型
                columnScript.append(columnScriptDO.getType().toUpperCase());
                //字段长度
                if(StringUtils.isNotEmpty(columnScriptDO.getLength()) && checkNeedAddLen(columnScriptDO.getType().toLowerCase())) {
                    columnScript.append("(");
                    columnScript.append(columnScriptDO.getLength().trim());
                    columnScript.append(")");
                }

                if(!columnScriptDO.isCanNull()) {
                    columnScript.append(" NOT NULL ");
                }

                if(columnScriptDO.getDefaultValue()==null) {
                    columnScript.append(" DEFAULT '" + columnScriptDO.getDefaultValue() + "' ");
                }else {
                    if(columnScriptDO.isCanNull()) {
                        columnScript.append(" DEFAULT NULL ");  
                    }
                }

                if(columnScriptDO.isAutoIncrease()) {
                    columnScript.append(" AUTO_INCREMENT "); 
                }

                //字段注释
                if(StringUtils.isEmpty(columnScriptDO.getColumnComment())) {
                    columnScript.append(",\n");
                }else {
                    columnScript.append("COMMENT '");
                    columnScript.append(columnScriptDO.getColumnComment());
                    columnScript.append("',\n");
                }
                columns = columns + columnScript.toString();

                if(StringUtils.isNotEmpty(columnScriptDO.getPrimaryKeyName())) {
                    primaryKeyColumn = columnScriptDO.getColumnName();
                }
            }
            tableScript = tableScript + columns;

            if(primaryKeyColumn!=null) {
                String primaryKeyScript = "    PRIMARY KEY (`" + primaryKeyColumn + "`)\n";
                tableScript = tableScript + primaryKeyScript;
            }else {
                tableScript = tableScript.substring(0, tableScript.length()-1);
            }

            String tableCommentScript = String.format(SUFFIX_PREFIX, tableScriptDO.getTableComment());
            tableScript = tableScript + tableCommentScript + "\n\n";
            tableScriptList.add(tableScript);
        }
        String allScript = "";
        for( String tableScript : tableScriptList) {
            allScript = allScript + tableScript;
        }
        File file = FileUtil.writeBytes(allScript.getBytes(), sqlFileName);
        log.info(file.getAbsolutePath());
        System.out.println(allScript);
    }

    /**
    * 检查是否需要添加长度
    * 
    * @param columnType
    * @return
    */
    private static boolean checkNeedAddLen(String columnType) {
        List<String> type = Arrays.asList(new String[] {"date", 
                                                        "datetime", 
                                                        "text", 
                                                        "timestamp", 
                                                        "json"});
        if(type.contains(columnType)) {
            return false;
        }
        return true;
    }

}
```
测试生成DDL语句
```java
try {
    XSSFWorkbook wb = AutoTableXlsxUtils.getXSSFWorkbook("Car-Custom_v2.0.xlsx");
    AutoTableScriptGenerator.geneateScript(AutoTableXlsxUtils.getTableScript(wb), "gen.sql");
} catch (Exception e) {
    e.printStackTrace();
}
```
可以解决90%的MySQL的建表语句生成，源码如下<br />[https://gitee.com/hjj520/springcloud-learn/tree/master/auto-table](https://gitee.com/hjj520/springcloud-learn/tree/master/auto-table)
