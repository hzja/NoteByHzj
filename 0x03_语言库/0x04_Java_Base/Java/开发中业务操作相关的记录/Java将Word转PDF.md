Java Word PDF<br />windows电脑安装wps office，并且设置wps office为默认启动 。（转换的office文档容易出问题，会出现打不开的问题）
:::danger
注：下面的操作，依赖jacb只能在windows系统使用，linux系统暂时无法使用
:::
<a name="gBNUB"></a>
### 1、引入pom依赖
```xml
<!-- word转pdf（依赖windows本地的wps） -->
<dependency>
  <groupId>com.jacob</groupId>
  <artifactId>jacob</artifactId>
  <version>1.18</version>
</dependency>
```
<a name="fzEt3"></a>
### 2、在jdk/bin目录下引入.dll文件
64位：jacob-1.18-x64.dll<br />32位：jacob-1.18-x86.dll
<a name="bEjYK"></a>
### 3、转换的Java代码
```java
import com.jacob.activeX.ActiveXComponent;
import com.jacob.com.Dispatch;
import java.io.File;

public class Word2Pdf {

    public static void main(String args[]) {
        ActiveXComponent app = null;
        String wordFile = "e:/测试word.docx";
        String pdfFile = "e:/测试pdf.pdf";

        System.out.println("开始转换...");
        // 开始时间
        long start = System.currentTimeMillis();
        try {
            // 打开word
            app = new ActiveXComponent("Word.Application");
            // 设置word不可见,很多博客下面这里都写了这一句话，其实是没有必要的，因为默认就是不可见的，如果设置可见就是会打开一个word文档，对于转化为pdf明显是没有必要的
            //app.setProperty("Visible", false);
            // 获得word中所有打开的文档
            Dispatch documents = app.getProperty("Documents").toDispatch();
            System.out.println("打开文件: " + wordFile);
            // 打开文档
            Dispatch document = Dispatch.call(documents, "Open", wordFile, false, true).toDispatch();
            // 如果文件存在的话，不会覆盖，会直接报错，所以我们需要判断文件是否存在
            File target = new File(pdfFile);
            if (target.exists()) {
                target.delete();
            }
            System.out.println("另存为: " + pdfFile);
            // 另存为，将文档报错为pdf，其中word保存为pdf的格式宏的值是17
            Dispatch.call(document, "SaveAs", pdfFile, 17);
            // 关闭文档
            Dispatch.call(document, "Close", false);
            // 结束时间
            long end = System.currentTimeMillis();
            System.out.println("转换成功，用时：" + (end - start) + "ms");
        }catch(Exception e) {
            e.getMessage();
            System.out.println("转换失败"+e.getMessage());
        }finally {
            // 关闭office
            app.invoke("Quit", 0);
        }
    }

} 
```

