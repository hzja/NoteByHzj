Java Excel 
<a name="EDqJc"></a>
## 1、POI简介
Apache POI - the Java API for Microsoft Documents，顾名思义，Apache的三方包，用来操作微软office文档的，多数时候用来操作excel，在使用过程中需要引入的修改依赖如下：
```xml
<dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi</artifactId>
    <version>3.9</version>
</dependency>
<dependency>
    <groupId>org.apache.poi</groupId>
    <artifactId>poi-ooxml</artifactId>
    <version>3.9</version>
</dependency>
```
POI的组件列表中，针对excel的主要是HSSF和XSSF组件，前者针对97-2007的通用版excel，即后缀xls；后者针对2007或更高版的excel，即后缀xlsx。<br />官方概要如下：
```
HSSF is the POI Project's pure Java implementation of the Excel '97(-2007) file format. 
XSSF is the POI Project's pure Java implementation of the Excel 2007 OOXML (.xlsx) file format.
```
<a name="q5Akj"></a>
## 2、POI核心类
看一下POI操作excel内容的类。
<a name="5I5f5"></a>
### 2.1 工作簿 Workbook
创建或维护Excel工作簿的所有类的超接口，Workbook，属于org.apache.poi.ss.usermodel包。<br />其下有两个实现类：

- HSSFWorkbook : 有读取.xls 格式和写入Microsoft Excel文件的方法。它与微软Office97-2003版本兼容
- XSSFWorkbook : 有读写Microsoft Excel和OpenOffice的XML文件的格式.xls或.xlsx的方法。它与MS-Office版本2007或更高版本兼容

所以在针对不同版本的excel时，需要对应以上使用不同的Workbook。构造函数中，常用的：
<a name="WxP3y"></a>
#### HSSFWorkbook
```java
HSSFWorkbook()
HSSFWorkbook(java.io.InputStream s)
```
<a name="7d92P"></a>
#### XSSFWorkbook
```
XSSFWorkbook()
XSSFWorkbook(java.io.File file)
XSSFWorkbook(java.io.InputStream is)
```
<a name="f36339e4"></a>
### 2.2 标签页 Sheet
**HSSFSheet** 和 **XSSFSheet** 都是Sheet接口的实现类，Sheet可以使用Workbook的两个方法获得：
```java
workbook.createSheet();
workbook.createSheet(String sheetName);
```
<a name="oJsl6"></a>
### 2.3 行 Row
同理，Row是 **HSSFRow** 和 **XSSFRow** 的接口，通过Sheet获取：
```java
sheet.createRow(int rownum);
```
<a name="dcd6a6c9"></a>
### 2.4 单元格 Cell
同理，Cell是 **HSSFCell** 和 **XSSFCell** 的接口，通过Row获取：
```java
row.createCell(int column);
row.createCell(int column, int type);
```
<a name="zitBd"></a>
## 3、创建和读取
POI中包含字体，公式，超链接等对应的封装类，这里只提出了核心的几个，需要了解更多的需要自行展开。
<a name="AHrW4"></a>
### 3.1 创建空白工作簿
```java
import java.io.*;
import org.apache.poi.xssf.usermodel.*;
public class CreateWorkBook {
   public static void main(String[] args)throws Exception {
      XSSFWorkbook workbook = new XSSFWorkbook(); 
      FileOutputStream out = new FileOutputStream(new File("createworkbook.xlsx"));
      workbook.write(out);
      out.close();
      System.out.println("createworkbook.xlsx written successfully");
   }
}
```
<a name="hSaoh"></a>
### 3.2 打开现有的工作簿
```java
import java.io.*;
import org.apache.poi.xssf.usermodel.*;
public class OpenWorkBook{
   public static void main(String args[])throws Exception{ 
      File file = new File("openworkbook.xlsx");
      FileInputStream fIP = new FileInputStream(file);
      XSSFWorkbook workbook = new XSSFWorkbook(fIP);
      if(file.isFile() && file.exists()){
         System.out.println("openworkbook.xlsx file open successfully.");
      }else {
         System.out.println("Error to open openworkbook.xlsx file.");
      }
   }
}
```
<a name="49d7ea6a"></a>
### 3.3 任意对象List转至为Excel文档
可用注解定义标签名和列名，可以将某个类的List转换为对应的Excel文档，列名如果在不使用注解的情况下默认为属性名：<br />类：
```java
@Excel(name = "学生标签页")
public class Student {
    @Excel(name = "姓名")
    private String name;
    private boolean male;
    @Excel(name = "身高")
    private int height;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public boolean isMale() {
        return male;
    }
    public void setMale(boolean male) {
        this.male = male;
    }
    public int getHeight() {
        return height;
    }
    public void setHeight(int height) {
        this.height = height;
    }
}
```
测试方法：
```java
public static void main(String[] args) {
    List<Student> list = new ArrayList<Student>();
    Student student1 = new Student();
    student1.setName("小红");
    student1.setMale(false);
    student1.setHeight(167);
    Student student2 = new Student();
    student2.setName("小明");
    student2.setMale(true);
    student2.setHeight(185);
    list.add(student1);
    list.add(student2);
    File file = new File("C:/Users/Dulk/Desktop/1314.xls");
    createExcel(list, file);
}
```
注解：
```java
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
@Retention(RetentionPolicy.RUNTIME)
public @interface Excel {
    public String name() default "";
}
```
方法：
```java
import org.apache.log4j.Logger;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
public class ExcelUtil {
    private static Logger log = Logger.getLogger(ExcelUtil.class);
    
    public static Workbook gainWorkbook(File file) throws ExcelException {
        if (!isExcel(file)) {
            throw new ExcelException("文件不是Excel类型");
        }
        
        if (!file.exists()) {
            try {
                OutputStream os = new FileOutputStream(file);
                Workbook workbook = isOlderEdition(file) ? new HSSFWorkbook() : new XSSFWorkbook();
                workbook.write(os);
                log.debug("文件不存在，新建该Excel文件");
                os.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            InputStream is = new FileInputStream(file);
            return isOlderEdition(file) ? new HSSFWorkbook(is) : new XSSFWorkbook(is);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
    
    private static boolean isOlderEdition(File file) {
        return file.getName().matches(".+\\.(?i)xls");
    }
    
    private static boolean isExcel(File file) {
        String fileName = file.getName();
        String regXls = ".+\\.(?i)xls";
        String regXlsx = ".+\\.(?i)xlsx";
        return fileName.matches(regXls) || fileName.matches(regXlsx);
    }
    
    public static <E> Workbook createExcel(List<E> list, File file) {
        String sheetName = "default";
        if (list.size() == 0) {
            return null;
        }
        Workbook workbook = null;
        try {
            Class clazz = list.get(0).getClass();
            Field[] fields = clazz.getDeclaredFields();
            if (clazz.isAnnotationPresent(Excel.class)) {
                Excel excel = (Excel) clazz.getAnnotation(Excel.class);
                sheetName = excel.name();
            }
            workbook = gainWorkbook(file);
            Sheet sheet = workbook.createSheet(sheetName);
            
            Row line = sheet.createRow(0);
            for (int k = 0; k < fields.length; k++) {
                Cell cell = line.createCell(k);
                String columnName = fields[k].getName();
                if (fields[k].isAnnotationPresent(Excel.class)) {
                    Excel excel = fields[k].getAnnotation(Excel.class);
                    columnName = excel.name();
                }
                cell.setCellValue(columnName);
            }
            
            for (int i = 1; i <= list.size(); i++) {
                Row row = sheet.createRow(i);
                for (int j = 1; j <= fields.length; j++) {
                    Cell cell = row.createCell(j - 1);
                    String fieldName = fields[j - 1].getName();
                    String fieldFirstLetterUpper = fieldName.substring(0, 1).toUpperCase();
                    String prefix = "get";
                    if ("boolean".equals(fields[j - 1].getType().getName())) {
                        prefix = "is";
                    }
                    String methodName = prefix + fieldFirstLetterUpper + fieldName.substring(1);
                    Method method = clazz.getMethod(methodName);
                    cell.setCellValue(String.valueOf(method.invoke(list.get(i - 1))));
                }
            }
            log.debug("List读入完毕");
            OutputStream os = new FileOutputStream(file);
            workbook.write(os);
            os.close();
        } catch (ExcelException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return workbook;
    }
}
```
