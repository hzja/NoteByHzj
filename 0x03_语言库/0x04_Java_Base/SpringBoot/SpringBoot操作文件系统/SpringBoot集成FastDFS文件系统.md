Java SpringBoot FastDFS
<a name="zqxKZ"></a>
### 添加FastDFS的相关依赖
```xml
<dependency>
   <groupId>com.github.tobato</groupId>
   <artifactId>fastdfs-client</artifactId>
   <version>1.26.2</version>
</dependency>
```
<a name="7NImP"></a>
### 在springboot启动类上添加导入配置类的注解
```java
@Import(FdfsClientConfig.class)
@EnableMBeanExport(registration = RegistrationPolicy.IGNORE_EXISTING)
```
<a name="gU4Cr"></a>
### 创建FastDFSClient工具类
```java
import com.github.tobato.fastdfs.conn.FdfsWebServer;
import com.github.tobato.fastdfs.domain.StorePath;
import com.github.tobato.fastdfs.proto.storage.DownloadByteArray;
import com.github.tobato.fastdfs.service.FastFileStorageClient;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.multipart.MultipartFile;
 
import java.io.*;
 
@Component
public class FastDFSClient {
 
    private static Logger log =LoggerFactory.getLogger(FastDFSClient.class);
 
    private static FastFileStorageClient fastFileStorageClient;
 
    private static FdfsWebServer fdfsWebServer;
 
    @Autowired
    public void setFastDFSClient(FastFileStorageClient fastFileStorageClient, FdfsWebServer fdfsWebServer) {
        FastDFSClient.fastFileStorageClient = fastFileStorageClient;
        FastDFSClient.fdfsWebServer = fdfsWebServer;
    }
 
    /**
     * @param multipartFile 文件对象
     * @return 返回文件地址
     * @author qbanxiaoli
     * @description 上传文件
     */
    public static String uploadFile(MultipartFile multipartFile) {
        try {
            StorePath storePath = fastFileStorageClient.uploadFile(multipartFile.getInputStream(), multipartFile.getSize(), FilenameUtils.getExtension(multipartFile.getOriginalFilename()), null);
            return storePath.getFullPath();
        } catch (IOException e) {
            log.error(e.getMessage());
            return null;
        }
    }
 
    /**
     * @param multipartFile 图片对象
     * @return 返回图片地址
     * @author Fcant
     * @description 上传缩略图
     */
    public static String uploadImageAndCrtThumbImage(MultipartFile multipartFile) {
        try {
            StorePath storePath = fastFileStorageClient.uploadImageAndCrtThumbImage(multipartFile.getInputStream(), multipartFile.getSize(), FilenameUtils.getExtension(multipartFile.getOriginalFilename()), null);
            return storePath.getFullPath();
        } catch (Exception e) {
            log.error(e.getMessage());
            return null;
        }
    }
 
    /**
     * @param file 文件对象
     * @return 返回文件地址
     * @author Fcant
     * @description 上传文件
     */
    public static String uploadFile(File file) {
        try {
            FileInputStream inputStream = new FileInputStream(file);
            StorePath storePath = fastFileStorageClient.uploadFile(inputStream, file.length(), FilenameUtils.getExtension(file.getName()), null);
            return storePath.getFullPath();
        } catch (Exception e) {
            log.error(e.getMessage());
            return null;
        }
    }
 
    /**
     * @param file 图片对象
     * @return 返回图片地址
     * @author Fcant
     * @description 上传缩略图
     */
    public static String uploadImageAndCrtThumbImage(File file) {
        try {
            FileInputStream inputStream = new FileInputStream(file);
            StorePath storePath = fastFileStorageClient.uploadImageAndCrtThumbImage(inputStream, file.length(), FilenameUtils.getExtension(file.getName()), null);
            return storePath.getFullPath();
        } catch (Exception e) {
            log.error(e.getMessage());
            return null;
        }
    }
 
    /**
     * @param bytes byte数组
     * @param fileExtension 文件扩展名
     * @return 返回文件地址
     * @author Fcant
     * @description 将byte数组生成一个文件上传
     */
    public static String uploadFile(byte[] bytes, String fileExtension) {
        ByteArrayInputStream stream = new ByteArrayInputStream(bytes);
        StorePath storePath = fastFileStorageClient.uploadFile(stream, bytes.length, fileExtension, null);
        return storePath.getFullPath();
    }
 
    /**
     * @param fileUrl 文件访问地址
     * @param file    文件保存路径
     * @author Fcant
     * @description 下载文件
     */
    public static boolean downloadFile(String fileUrl, File file) {
        try {
            StorePath storePath = StorePath.praseFromUrl(fileUrl);
            byte[] bytes = fastFileStorageClient.downloadFile(storePath.getGroup(), storePath.getPath(), new DownloadByteArray());
            FileOutputStream stream = new FileOutputStream(file);
            stream.write(bytes);
        } catch (Exception e) {
            log.error(e.getMessage());
            return false;
        }
        return true;
    }
 
    /**
     * @param fileUrl 文件访问地址
     * @author Fcant
     * @description 删除文件
     */
    public static boolean deleteFile(String fileUrl) {
        if (StringUtils.isEmpty(fileUrl)) {
            return false;
        }
        try {
            StorePath storePath = StorePath.praseFromUrl(fileUrl);
            fastFileStorageClient.deleteFile(storePath.getGroup(), storePath.getPath());
        } catch (Exception e) {
            log.error(e.getMessage());
            return false;
        }
        return true;
    }
 
    // 封装文件完整URL地址
    public static String getResAccessUrl(String path) {
        String url = fdfsWebServer.getWebServerUrl() + path;
        log.info("上传文件地址为：\n" + url);
        return url;
    }
 
}
```
<a name="wS7QN"></a>
### 配置yml文件
```yaml
# 分布式文件系统fastdfs配置
fdfs:
  # socket连接超时时长
  soTimeout: 1500
  # 连接tracker服务器超时时长
  connectTimeout: 600
  pool:
    # 从池中借出的对象的最大数目
    max-total: 153
    # 获取连接时的最大等待毫秒数100
    max-wait-millis: 102
  # 缩略图生成参数，可选
  thumbImage:
    width: 150
    height: 150
  # 跟踪服务器tracker_server请求地址,支持多个，这里只有一个，如果有多个在下方加- x.x.x.x:port
  trackerList:
    - 192.168.127.131:22122
  #
  # 存储服务器storage_server访问地址
  web-server-url: http://192.168.127.131/
  spring:
    http:
      multipart:
        max-file-size: 100MB # 最大支持文件大小
        max-request-size: 100MB # 最大支持请求大小
```
<a name="CKZwm"></a>
### 测试类
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class FileClientApplicationTests {

    @Test
    public void Upload() {
        String fileUrl = this.getClass().getResource("/test.jpg").getPath();
        File file = new File(fileUrl);
        String str = FastDFSClient.uploadFile(file);
        FastDFSClient.getResAccessUrl(str);
    }

    @Test
    public void Delete() {
        FastDFSClient.deleteFile("group1/M00/00/00/rBEAClu8OiSAFbN_AAbhXQnXzvw031.jpg");
    }
}
```
运行测试类，返回的就是能访问文件的URL地址。
