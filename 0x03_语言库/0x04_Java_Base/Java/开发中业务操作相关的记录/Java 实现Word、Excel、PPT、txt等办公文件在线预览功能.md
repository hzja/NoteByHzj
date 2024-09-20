Java<br />如何用 Java 实现word、excel、ppt、txt等办公文件在线预览功能？<br />java 实现办公文件在线预览功能是一个在工作中也许会遇到的需求，网上些公司专门提供这样的服务，不过需要收费。<br />如果想要免费的，可以用 openoffice，实现原理就是：<br />通过第三方工具openoffice，将word、excel、ppt、txt等文件转换为pdf文件流；当然如果装了Adobe Reader XI，那把pdf直接拖到浏览器页面就可以直接打开预览，前提就是浏览器支持pdf文件浏览。<br />这里介绍通过poi实现word、excel、ppt转pdf流，这样就可以在浏览器上实现预览了。<br />到官网下载 Apache OpenOffice： [https://www.openoffice.org/download/](https://www.openoffice.org/download/) 安装包，安装运行。（不同系统的安装方法不同）<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1635521294884-045383a4-7b4f-4ff0-a7c4-75445c187092.png#clientId=u76d39fa5-18cb-4&from=paste&height=594&id=u3aa69486&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&size=657283&status=done&style=shadow&taskId=u9434efc6-a49b-4601-b64f-d5e7df26924&width=1280)
<a name="eglke"></a>
### 1、在项目的pom文件中引入依赖
```xml
<!--openoffice-->
<dependency>
    <groupId>com.artofsolving</groupId>
    <artifactId>jodconverter</artifactId>
    <version>2.2.1</version>
</dependency>
```
<a name="kaWYH"></a>
### 2、将word、excel、ppt转换为pdf流的工具类代码
```java
import com.artofsolving.jodconverter.DefaultDocumentFormatRegistry;
import com.artofsolving.jodconverter.DocumentConverter;
import com.artofsolving.jodconverter.DocumentFormat;
import com.artofsolving.jodconverter.openoffice.connection.OpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.connection.SocketOpenOfficeConnection;
import com.artofsolving.jodconverter.openoffice.converter.StreamOpenOfficeDocumentConverter;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection

/** 
 * 文件格式转换工具类 
 */
public class FileConvertUtil {
    /*
     * 默认转换后文件后缀 
     */
    private static final String DEFAULT_SUFFIX = "pdf";
    /*
     * openoffice_port
     */
    private static final Integer OPENOFFICE_PORT = 8100;

    /**     
     * 方法描述 office文档转换为PDF(处理本地文件)      *     
     * @param sourcePath 源文件路径     
     * @param suffix     源文件后缀     
     * @return InputStream 转换后文件输入流     
     */
    public static InputStream convertLocaleFile(String sourcePath, String suffix) throws Exception {
        File inputFile = new File(sourcePath);
        InputStream inputStream = new FileInputStream(inputFile);
        return covertCommonByStream(inputStream, suffix);
    }

    /**     
     * 方法描述  office文档转换为PDF(处理网络文件)      *     
     * @param netFileUrl 网络文件路径     
     * @param suffix     文件后缀     
     * @return InputStream 转换后文件输入流     
     */
    public static InputStream convertNetFile(String netFileUrl, String suffix) throws Exception {
        // 创建URL
        URL url = new URL(netFileUrl);
        // 试图连接并取得返回状态码
        URLConnection urlconn = url.openConnection();
        urlconn.connect();
        HttpURLConnection httpconn = (HttpURLConnection) urlconn;
        int httpResult = httpconn.getResponseCode();
        if (httpResult == HttpURLConnection.HTTP_OK) {
            InputStream inputStream = urlconn.getInputStream();
            return covertCommonByStream(inputStream, suffix);
        }
        return null;
    }

    /**     
     * 方法描述  将文件以流的形式转换      *      
     * @param inputStream 源文件输入流      
     * @param suffix      源文件后缀      
     * @return InputStream 转换后文件输入流     
     */
    public static InputStream covertCommonByStream(InputStream inputStream, String suffix) throws Exception {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        OpenOfficeConnection connection = new SocketOpenOfficeConnection(OPENOFFICE_PORT);
        connection.connect();
        DocumentConverter converter = new StreamOpenOfficeDocumentConverter(connection);
        DefaultDocumentFormatRegistry formatReg = new DefaultDocumentFormatRegistry();
        DocumentFormat targetFormat = formatReg.getFormatByFileExtension(DEFAULT_SUFFIX);
        DocumentFormat sourceFormat = formatReg.getFormatByFileExtension(suffix);
        converter.convert(inputStream, sourceFormat, out, targetFormat);
        connection.disconnect();
        return outputStreamConvertInputStream(out);
    }

    /**     
     * 方法描述 outputStream转inputStream     
     */
    public static ByteArrayInputStream outputStreamConvertInputStream(final OutputStream out) throws Exception {
        ByteArrayOutputStream baos=(ByteArrayOutputStream) out;
        return new ByteArrayInputStream(baos.toByteArray());
    }

    public static void main(String[] args) throws IOException {
        //convertNetFile("http://172.16.10.21/files/home/upload/department/base/201912090541573c6abdf2394d4ae3b7049dcee456d4f7.doc", ".pdf");
        //convert("c:/Users/admin/Desktop/2.pdf", "c:/Users/admin/Desktop/3.pdf");
    }
}
```
<a name="RPIS5"></a>
### 3、service层在线预览方法代码
```java
/**  
 * @Description:系统文件在线预览接口 
 */
public void onlinePreview(String url, HttpServletResponse response) throws Exception {
    //获取文件类型
    String[] str = SmartStringUtil.split(url,"\\.");

    if(str.length==0){
        throw new Exception("文件格式不正确");
    }
    String suffix = str[str.length-1];
    if(!suffix.equals("txt") && !suffix.equals("doc") && !suffix.equals("docx") && !suffix.equals("xls")
            && !suffix.equals("xlsx") && !suffix.equals("ppt") && !suffix.equals("pptx")){
        throw new Exception("文件格式不支持预览");
    }
    InputStream in=FileConvertUtil.convertNetFile(url,suffix);
    OutputStream outputStream = response.getOutputStream();
    //创建存放文件内容的数组
    byte[] buff =new byte[1024];
    //所读取的内容使用n来接收
    int n;
    //当没有读取完时,继续读取,循环
    while((n=in.read(buff))!=-1){
        //将字节数组的数据全部写入到输出流中
        outputStream.write(buff,0,n);
    }
    //强制将缓存区的数据进行输出
    outputStream.flush();
    //关流
    outputStream.close();
    in.close();
}
```
<a name="WZxEb"></a>
### 4、Controller层代码
```java
@ApiOperation(value = "系统文件在线预览接口")
@PostMapping("/api/file/onlinePreview")
public void onlinePreview(@RequestParam("url") String url, HttpServletResponse response) throws Exception{
    fileService.onlinePreview(url,response);
}
```
<a name="L81mH"></a>
### 5、效果展示
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635520895531-235603b1-dfe4-4e7d-9f38-0368abb8401d.webp#clientId=uaa8edbbe-3d30-4&from=paste&id=u7a44ce54&originHeight=704&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u9e4e5a7f-43c3-471e-a2b3-cccbf8f56cb)在线预览execl![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635520895544-03d6e1bd-e83e-4c8e-a868-b8d1c4e74557.webp#clientId=uaa8edbbe-3d30-4&from=paste&id=u4d9cadf1&originHeight=536&originWidth=1080&originalType=url&ratio=1&status=done&style=shadow&taskId=u74c7bfdc-596e-40b8-a6ec-4ca2684c451)预览word
