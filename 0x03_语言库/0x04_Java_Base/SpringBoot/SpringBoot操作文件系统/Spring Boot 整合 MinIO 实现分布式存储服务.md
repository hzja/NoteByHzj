JavaSpringBootMinIO
<a name="lEV7Y"></a>
## 什么是MinIO？
Minio 是个基于 Golang 编写的开源对象存储套件，基于Apache License v2.0开源协议，虽然轻量，却拥有着不错的性能。它兼容亚马逊S3云存储服务接口。可以很简单的和其他应用结合使用，例如 NodeJS、Redis、MySQL等。
<a name="vix2N"></a>
### 1、应用场景
MinIO 的应用场景除了可以作为私有云的对象存储服务来使用，也可以作为云对象存储的网关层，无缝对接 Amazon S3 或者 MicroSoft Azure 。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658322630743-894c8091-a44e-4e72-8608-d15325b790f8.jpeg#clientId=u2975268f-459d-4&from=paste&id=u0403ccd6&originHeight=388&originWidth=612&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6d5669d8-f3d4-4bdd-badf-23dbd3af6d1&title=)
<a name="i75hH"></a>
### 2、特点

1. **高性能**：作为一款高性能存储，在标准硬件条件下，其读写速率分别可以达到 55Gb/s 和 35Gb/s。并且MinIO 支持一个对象文件可以是任意大小，从几kb到最大5T不等。
2. **可扩展**：不同MinIO集群可以组成联邦，并形成一个全局的命名空间，并且支持跨越多个数据中心。
3. **云原生**：容器化、基于K8S的编排、多租户支持。
4. **Amazon S3兼容**：使用 Amazon S3 v2 / v4 API。可以使用Minio SDK，Minio Client，AWS SDK 和 AWS CLI 访问Minio服务器。
5. **SDK支持**：
   1. GO SDK：[https://github.com/minio/minio-go](https://github.com/minio/minio-go)
   2. JavaSDK：[https://github.com/minio/minio-java](https://github.com/minio/minio-java)
   3. PythonSDK：[https://github.com/minio/minio-py](https://github.com/minio/minio-py)
6. **图形化界面**：有操作页面
7. **支持纠删码**：MinIO使用纠删码、Checksum来防止硬件错误和静默数据污染。在最高冗余度配置下，即使丢失1/2的磁盘也能恢复数据。

功能很强大，这里只是抛砖引玉，有兴趣的朋友自己去探索吧~
<a name="ZsKeX"></a>
## 安装MinIO
安装非常简单，笔者这里使用docker安装，步骤如下：
<a name="ypP2y"></a>
### 1、获取镜像
执行命令如下：
```bash
docker pull minio/minio
```
<a name="hWmUt"></a>
### 2、启动镜像
执行命令如下：
```bash
docker run -p 9000:9000 -p 9001:9001 --name minio -d --restart=always -e "MINIO_ACCESS_KEY=admin" -e "MINIO_SECRET_KEY=admin" -v /home/data:/data -v /home/config:/root/.minio minio/minio server --console-address ":9000" --address ":9001" /data
```
命令解释如下：

- `**-p**`：**9000**是图形界面的端口，**9001**是API的端口，在使用SDK连接需要用到
- `**MINIO_ACCESS_KEY**`：指定图形界面的用户名
- `**MINIO_SECRET_KEY**`：指定图形界面的密码

按照上述两个步骤启动成功即可。
<a name="cQbZi"></a>
### 3、图形界面操作
安装成功后直接访问地址：**http:/ip:9000/login**，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322630743-56bf30d8-c128-4089-a15e-ec2e772081aa.png#clientId=u2975268f-459d-4&from=paste&id=u599779cb&originHeight=565&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubbfc9c8a-dcaa-4ea6-b46e-795b1532af5&title=)<br />输入用户名和密码登录成功后，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322630876-ee2950f9-ec24-48c7-a8f8-8fe4dad5c340.png#clientId=u2975268f-459d-4&from=paste&id=u1a6130a6&originHeight=519&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11fd8b5c-4542-4b71-86b9-e94811c5cb3&title=)<br />菜单很多，这里就不再详细介绍了，笔者这里直接在**Buckets**菜单中创建一个桶为**test**，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322630780-38f48d8a-991c-4ff4-934a-3a02db6a199b.png#clientId=u2975268f-459d-4&from=paste&id=u1a4f0af5&originHeight=407&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue8340f39-2202-45c2-8cb7-360e137126a&title=)<br />并且设置这个桶的隐私规则为**public**，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322630805-92460261-f0d8-4cc7-8f4c-1b3952e47423.png#clientId=u2975268f-459d-4&from=paste&id=u8f5c99bd&originHeight=424&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucd4dafda-c88a-4bfe-9954-47b40597e7a&title=)<br />MinIO到此已经安装设置成功了
<a name="yCx9T"></a>
## Spring Boot 整合MinIO 上传文件
虽然MinIO在图形界面提供了手动上传的操作，但是也可以通过SDK的方式去上传，下面介绍一下Spring Boot 整合MinIO上传文件。
<a name="eAUrQ"></a>
### 1、获取accessKey和secretKey
这里的**accessKey**和**secretKey**并不是图形界面登录名和密码，获取很简单，直接在图形界面中操作，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322631104-1c36ecfd-d10f-4b4b-ba8a-119190a15389.png#clientId=u2975268f-459d-4&from=paste&id=u239caa29&originHeight=446&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u028d909f-6beb-4da6-ac12-a4acd1ab8fe&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322631228-6e2da813-77e8-4923-a0c8-9887b1949ff3.png#clientId=u2975268f-459d-4&from=paste&id=u2f8f46db&originHeight=461&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5774016d-6504-4036-ac1a-a5f31eaa81b&title=)
<a name="Md8si"></a>
### 2、添加依赖
添加MinIO的依赖，如下：
```xml
<dependency>
	<groupId>io.minio</groupId>
	<artifactId>minio</artifactId>
	<version>8.2.1</version>
</dependency>
```
<a name="LccnD"></a>
### 3、添加配置
这里对SDK做了简单的封装，案例源码都会提供，下面只列出部分代码。<br />在**aplication.yml**配置中添加MInIO相关的配置，如下：
```yaml
minio:
  # 访问的url
  endpoint: http://192.168.47.148
  # API的端口
  port: 9001
  # 秘钥
  accessKey: HQGWFYLWGC6FVJ0CQFOG
  secretKey: pUGhAgQhZDxJaLmN3uz65YX7Bb3FyLdLglBvcCr1
  secure: false
  bucket-name: test # 桶名 我这是给出了一个默认桶名
  image-size: 10485760 # 我在这里设定了 图片文件的最大大小
  file-size: 1073741824 # 此处是设定了文件的最大大小
```
<a name="Wr8V5"></a>
### 4、添加配置类绑定配置值
```java
import io.minio.MinioClient;
import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Data
@Configuration
@ConfigurationProperties(prefix = "minio")
public class MinioProperties {
	
	/**
	* 是一个URL，域名，IPv4或者IPv6地址")
	*/
	private String endpoint;
	
	/**
	*     //"TCP/IP端口号"
	*/
	private Integer port;
	
	/**
	*     //"accessKey类似于用户ID，用于唯一标识你的账户"
	*/
	private String accessKey;
	
	/**
	*     //"secretKey是你账户的密码"
	*/
	private String secretKey;
	
	/**
	*     //"如果是true，则用的是https而不是http,默认值是true"
	*/
	private boolean secure;
	
	/**
	*     //"默认存储桶"
	*/
	private String bucketName;
	
	/**
	* 图片的最大大小
	*/
	private long imageSize;
	
	/**
	* 其他文件的最大大小
	*/
	private long fileSize;
	
	
	/**
	* 官网给出的 构造方法
	* 此类是 客户端进行操作的类
	*/
	@Bean
	public MinioClient minioClient() {
		MinioClient minioClient =
			MinioClient.builder()
			.credentials(accessKey, secretKey)
			.endpoint(endpoint,port,secure)
			.build();
		return minioClient;
	}
}
```
<a name="zU6qk"></a>
### 5、添加MinioUtil工具类
```java
import com.java.family.minio.config.MinioProperties;
import io.minio.*;
import io.minio.http.Method;
import io.minio.messages.Bucket;
import io.minio.messages.DeleteError;
import io.minio.messages.DeleteObject;
import io.minio.messages.Item;
import lombok.SneakyThrows;
import org.springframework.stereotype.Component;
import org.springframework.web.multipart.MultipartFile;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.TimeUnit;

@Component
public class MinioUtil {

    private final MinioClient minioClient;

    private final MinioProperties minioProperties;

    public MinioUtil(MinioClient minioClient, MinioProperties minioProperties) {
        this.minioClient = minioClient;
        this.minioProperties = minioProperties;
    }

    /**
     * 检查存储桶是否存在
     *
     * @param bucketName 存储桶名称
     * @return
     */
    @SneakyThrows
    public boolean bucketExists(String bucketName) {
        boolean found =
                minioClient.bucketExists(BucketExistsArgs.builder().bucket(bucketName).build());
        if (found) {
            System.out.println(bucketName + " exists");
        } else {
            System.out.println(bucketName + " does not exist");
        }
        return found;
    }

    /**
     * 创建存储桶
     *
     * @param bucketName 存储桶名称
     */
    @SneakyThrows
    public boolean makeBucket(String bucketName) {
        boolean flag = bucketExists(bucketName);
        if (!flag) {
            minioClient.makeBucket(
                    MakeBucketArgs.builder()
                            .bucket(bucketName)
                            .build());

            return true;
        } else {
            return false;
        }
    }

    /**
     * 列出所有存储桶名称
     *
     * @return
     */
    @SneakyThrows
    public List<String> listBucketNames() {
        List<Bucket> bucketList = listBuckets();
        List<String> bucketListName = new ArrayList<>();
        for (Bucket bucket : bucketList) {
            bucketListName.add(bucket.name());
        }
        return bucketListName;
    }

    /**
     * 列出所有存储桶
     *
     * @return
     */
    @SneakyThrows
    public List<Bucket> listBuckets() {
        return minioClient.listBuckets();
    }


    /**
     * 删除存储桶
     *
     * @param bucketName 存储桶名称
     * @return
     */
    @SneakyThrows
    public boolean removeBucket(String bucketName) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            Iterable<Result<Item>> myObjects = listObjects(bucketName);
            for (Result<Item> result : myObjects) {
                Item item = result.get();
                // 有对象文件，则删除失败
                if (item.size() > 0) {
                    return false;
                }
            }
            // 删除存储桶，注意，只有存储桶为空时才能删除成功。
            minioClient.removeBucket(RemoveBucketArgs.builder().bucket(bucketName).build());
            flag = bucketExists(bucketName);
            if (!flag) {
                return true;
            }
        }
        return false;
    }

    /**
     * 列出存储桶中的所有对象名称
     *
     * @param bucketName 存储桶名称
     * @return
     */
    @SneakyThrows
    public List<String> listObjectNames(String bucketName) {
        List<String> listObjectNames = new ArrayList<>();
        boolean flag = bucketExists(bucketName);
        if (flag) {
            Iterable<Result<Item>> myObjects = listObjects(bucketName);
            for (Result<Item> result : myObjects) {
                Item item = result.get();
                listObjectNames.add(item.objectName());
            }
        }else{
            listObjectNames.add("存储桶不存在");
        }
        return listObjectNames;
    }


    /**
     * 列出存储桶中的所有对象
     *
     * @param bucketName 存储桶名称
     * @return
     */
    @SneakyThrows
    public Iterable<Result<Item>> listObjects(String bucketName) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            return minioClient.listObjects(
                    ListObjectsArgs.builder().bucket(bucketName).build());
        }
        return null;
    }

    /**
     * 文件上传
     *
     * @param bucketName
     * @param multipartFile
     */
    @SneakyThrows
    public void putObject(String bucketName, MultipartFile multipartFile, String filename, String fileType) {
        InputStream inputStream = new ByteArrayInputStream(multipartFile.getBytes());
        minioClient.putObject(
                PutObjectArgs.builder().bucket(bucketName).object(filename).stream(
                        inputStream, -1, minioProperties.getFileSize())
                        .contentType(fileType)
                        .build());
    }


    /**
     * 文件访问路径
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     * @return
     */
    @SneakyThrows
    public String getObjectUrl(String bucketName, String objectName) {
        boolean flag = bucketExists(bucketName);
        String url = "";
        if (flag) {
            url = minioClient.getPresignedObjectUrl(
                    GetPresignedObjectUrlArgs.builder()
                            .method(Method.GET)
                            .bucket(bucketName)
                            .object(objectName)
                            .expiry(2, TimeUnit.MINUTES)
                            .build());
            System.out.println(url);
        }
        return url;
    }


    /**
     * 删除一个对象
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     */
    @SneakyThrows
    public boolean removeObject(String bucketName, String objectName) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            minioClient.removeObject(
                    RemoveObjectArgs.builder().bucket(bucketName).object(objectName).build());
            return true;
        }
        return false;
    }

    /**
     * 以流的形式获取一个文件对象
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     * @return
     */
    @SneakyThrows
    public InputStream getObject(String bucketName, String objectName) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            StatObjectResponse statObject = statObject(bucketName, objectName);
            if (statObject != null && statObject.size() > 0) {
                InputStream stream =
                        minioClient.getObject(
                                GetObjectArgs.builder()
                                        .bucket(bucketName)
                                        .object(objectName)
                                        .build());
                return stream;
            }
        }
        return null;
    }

    /**
     * 获取对象的元数据
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     * @return
     */
    @SneakyThrows
    public StatObjectResponse statObject(String bucketName, String objectName) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            StatObjectResponse stat =
                    minioClient.statObject(
                            StatObjectArgs.builder().bucket(bucketName).object(objectName).build());
            return stat;
        }
        return null;
    }

    /**
     * 删除指定桶的多个文件对象,返回删除错误的对象列表，全部删除成功，返回空列表
     *
     * @param bucketName  存储桶名称
     * @param objectNames 含有要删除的多个object名称的迭代器对象
     * @return
     */
    @SneakyThrows
    public boolean removeObject(String bucketName, List<String> objectNames) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            List<DeleteObject> objects = new LinkedList<>();
            for (int i = 0; i < objectNames.size(); i++) {
                objects.add(new DeleteObject(objectNames.get(i)));
            }
            Iterable<Result<DeleteError>> results =
                    minioClient.removeObjects(
                            RemoveObjectsArgs.builder().bucket(bucketName).objects(objects).build());
            for (Result<DeleteError> result : results) {
                DeleteError error = result.get();
                System.out.println(
                        "Error in deleting object " + error.objectName() + "; " + error.message());
                return false;
            }
        }
        return true;
    }

    /**
     * 以流的形式获取一个文件对象（断点下载）
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     * @param offset     起始字节的位置
     * @param length     要读取的长度 (可选，如果无值则代表读到文件结尾)
     * @return
     */
    @SneakyThrows
    public InputStream getObject(String bucketName, String objectName, long offset, Long length) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            StatObjectResponse statObject = statObject(bucketName, objectName);
            if (statObject != null && statObject.size() > 0) {
                InputStream stream =
                        minioClient.getObject(
                                GetObjectArgs.builder()
                                        .bucket(bucketName)
                                        .object(objectName)
                                        .offset(offset)
                                        .length(length)
                                        .build());
                return stream;
            }
        }
        return null;
    }


    /**
     * 通过InputStream上传对象
     *
     * @param bucketName 存储桶名称
     * @param objectName 存储桶里的对象名称
     * @param inputStream     要上传的流
     * @param contentType     要上传的文件类型 MimeTypeUtils.IMAGE_JPEG_VALUE
     * @return
     */
    @SneakyThrows
    public boolean putObject(String bucketName, String objectName, InputStream inputStream,String contentType) {
        boolean flag = bucketExists(bucketName);
        if (flag) {
            minioClient.putObject(
                    PutObjectArgs.builder().bucket(bucketName).object(objectName).stream(
                            inputStream, -1, minioProperties.getFileSize())
                            .contentType(contentType)
                            .build());
            StatObjectResponse statObject = statObject(bucketName, objectName);
            if (statObject != null && statObject.size() > 0) {
                return true;
            }
        }
        return false;
    }
}
```
<a name="Kr1lD"></a>
### 6、文件类型工具类
```java
import cn.hutool.core.io.FileTypeUtil;
import org.springframework.web.multipart.MultipartFile;

import java.io.IOException;
import java.io.InputStream;

/**
 * 文件类型工具类
 * TODO 其中只是定义了部分文件格式，有缺失的自己添加
 */
public class FileTypeUtils {

    private final static String IMAGE_TYPE = "image/";
    private final static String AUDIO_TYPE = "audio/";
    private final static String VIDEO_TYPE = "video/";
    private final static String APPLICATION_TYPE = "application/";
    private final static String TXT_TYPE = "text/";

    public static String getFileType(MultipartFile multipartFile) {
        InputStream inputStream = null;
        String type = null;
        try {
            inputStream = multipartFile.getInputStream();
            type = FileTypeUtil.getType(inputStream);
            System.out.println(type);
            if (type.equalsIgnoreCase("JPG") || type.equalsIgnoreCase("JPEG")
                    || type.equalsIgnoreCase("GIF") || type.equalsIgnoreCase("PNG")
                    || type.equalsIgnoreCase("BMP") || type.equalsIgnoreCase("PCX")
                    || type.equalsIgnoreCase("TGA") || type.equalsIgnoreCase("PSD")
                    || type.equalsIgnoreCase("TIFF")) {
                return IMAGE_TYPE+type;
            }
            if (type.equalsIgnoreCase("mp3") || type.equalsIgnoreCase("OGG")
                    || type.equalsIgnoreCase("WAV") || type.equalsIgnoreCase("REAL")
                    || type.equalsIgnoreCase("APE") || type.equalsIgnoreCase("MODULE")
                    || type.equalsIgnoreCase("MIDI") || type.equalsIgnoreCase("VQF")
                    || type.equalsIgnoreCase("CD")) {
                return AUDIO_TYPE+type;
            }
            if (type.equalsIgnoreCase("mp4") || type.equalsIgnoreCase("avi")
                    || type.equalsIgnoreCase("MPEG-1") || type.equalsIgnoreCase("RM")
                    || type.equalsIgnoreCase("ASF") || type.equalsIgnoreCase("WMV")
                    || type.equalsIgnoreCase("qlv") || type.equalsIgnoreCase("MPEG-2")
                    || type.equalsIgnoreCase("MPEG4") || type.equalsIgnoreCase("mov")
                    || type.equalsIgnoreCase("3gp")) {
                return VIDEO_TYPE+type;
            }
            if (type.equalsIgnoreCase("doc") || type.equalsIgnoreCase("docx")
                    || type.equalsIgnoreCase("ppt") || type.equalsIgnoreCase("pptx")
                    || type.equalsIgnoreCase("xls") || type.equalsIgnoreCase("xlsx")
                    || type.equalsIgnoreCase("zip")||type.equalsIgnoreCase("jar")) {
                return APPLICATION_TYPE+type;
            }
            if (type.equalsIgnoreCase("txt")) {
                return TXT_TYPE+type;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
```
<a name="v7hwa"></a>
### 7、定义业务接口及实现类
```java
import io.minio.messages.Bucket;
import org.springframework.web.multipart.MultipartFile;
import java.io.InputStream;
import java.util.List;

public interface MinioService {

    /**
     * 判断 bucket是否存在
     *
     * @param bucketName
     * @return
     */
    boolean bucketExists(String bucketName);

    /**
     * 创建 bucket
     *
     * @param bucketName
     */
    void makeBucket(String bucketName);

    /**
     * 列出所有存储桶名称
     * @return
     */
    List<String> listBucketName();

    /**
     * 列出所有存储桶 信息
     *
     * @return
     */
    List<Bucket> listBuckets();

    /**
     * 根据桶名删除桶
     * @param bucketName
     */
    boolean removeBucket(String bucketName);

    /**
     * 列出存储桶中的所有对象名称
     * @param bucketName
     * @return
     */
    List<String> listObjectNames(String bucketName);

    /**
     * 文件上传
     *
     * @param multipartFile
     * @param bucketName
     */
    String putObject( MultipartFile multipartFile, String bucketName,String fileType);

    /**
     * 文件流下载
     * @param bucketName
     * @param objectName
     * @return
     */
    InputStream downloadObject(String bucketName, String objectName);


    /**
     * 删除文件
     * @param bucketName
     * @param objectName
     */
    boolean removeObject(String bucketName, String objectName);



    /**
     * 批量删除文件
     * @param bucketName
     * @param objectNameList
     * @return
     */
    boolean removeListObject(String bucketName, List<String> objectNameList);

    /**
     * 获取文件路径
     * @param bucketName
     * @param objectName
     * @return
     */
    String getObjectUrl(String bucketName,String objectName);
}
```
```java
import com.java.family.minio.config.MinioProperties;
import com.java.family.minio.service.MinioService;
import com.java.family.minio.utils.MinioUtil;
import io.minio.MinioClient;
import io.minio.messages.Bucket;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;


import java.io.InputStream;
import java.util.List;
import java.util.UUID;

@Service
public class MinioServiceImpl implements MinioService {

    @Autowired
    private  MinioUtil minioUtil;
    @Autowired
    private  MinioProperties minioProperties;

    @Override
    public boolean bucketExists(String bucketName) {
        return minioUtil.bucketExists(bucketName);
    }


    @Override
    public void makeBucket(String bucketName) {
        minioUtil.makeBucket(bucketName);
    }

    @Override
    public List<String> listBucketName() {
        return minioUtil.listBucketNames();
    }

    @Override
    public List<Bucket> listBuckets() {
        return minioUtil.listBuckets();
    }

    @Override
    public boolean removeBucket(String bucketName) {
        return minioUtil.removeBucket(bucketName);
    }


    @Override
    public List<String> listObjectNames(String bucketName) {
        return minioUtil.listObjectNames(bucketName);
    }


    @Override
    public String putObject(MultipartFile file, String bucketName,String fileType) {
        try {
            bucketName = StringUtils.isNotBlank(bucketName) ? bucketName : minioProperties.getBucketName();
            if (!this.bucketExists(bucketName)) {
                this.makeBucket(bucketName);
            }
            String fileName = file.getOriginalFilename();

            String objectName = UUID.randomUUID().toString().replaceAll("-", "")
                    + fileName.substring(fileName.lastIndexOf("."));
            minioUtil.putObject(bucketName, file, objectName,fileType);
            return minioProperties.getEndpoint()+":"+minioProperties.getPort()+"/"+bucketName+"/"+objectName;
        } catch (Exception e) {
            e.printStackTrace();
            return "上传失败";
        }
    }

    @Override
    public InputStream downloadObject(String bucketName, String objectName) {
        return minioUtil.getObject(bucketName,objectName);
    }

    @Override
    public boolean removeObject(String bucketName, String objectName) {
        return minioUtil.removeObject(bucketName, objectName);
    }

    @Override
    public boolean removeListObject(String bucketName, List<String> objectNameList) {
        return minioUtil.removeObject(bucketName,objectNameList);
    }

    @Override
    public String getObjectUrl(String bucketName,String objectName) {
        return minioUtil.getObjectUrl(bucketName, objectName);
    }
}
```
<a name="aR30i"></a>
### 8、新建上传文件接口
这里定义了一个上传文件接口，如下：
```java
import com.java.family.minio.service.MinioService;
import com.java.family.minio.utils.FileTypeUtils;
import org.apache.tomcat.util.http.fileupload.IOUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RequestMapping("/minio")
@RestController
public class MinioController {

    @Autowired
    private  MinioService minioService;

    @PostMapping("/upload")
    public String uploadFile(MultipartFile file, String bucketName) {
        String fileType = FileTypeUtils.getFileType(file);
        if (fileType != null) {
            return minioService.putObject(file, bucketName, fileType);
        }
        return "不支持的文件格式。请确认格式,重新上传！！！";
    }

    @PostMapping("/addBucket/{bucketName}")
    public String addBucket(@PathVariable String bucketName) {
        minioService.makeBucket(bucketName);
        return "创建成功！！！";
    }

    @GetMapping("/show/{bucketName}")
    public List<String> show(@PathVariable String bucketName) {
        return minioService.listObjectNames(bucketName);
    }

    @GetMapping("/showBucketName")
    public List<String> showBucketName() {
        return minioService.listBucketName();
    }

    @GetMapping("/showListObjectNameAndDownloadUrl/{bucketName}")
    public Map<String, String> showListObjectNameAndDownloadUrl(@PathVariable String bucketName) {
        Map<String, String> map = new HashMap<>();
        List<String> listObjectNames = minioService.listObjectNames(bucketName);
        String url = "localhost:8085/minio/download/" + bucketName + "/";
        listObjectNames.forEach(System.out::println);
        for (int i = 0; i <listObjectNames.size() ; i++) {
            map.put(listObjectNames.get(i),url+listObjectNames.get(i));
        }
        return map;
    }

    @DeleteMapping("/removeBucket/{bucketName}")
    public String delBucketName(@PathVariable String bucketName) {
        return minioService.removeBucket(bucketName) == true ? "删除成功" : "删除失败";
    }

    @DeleteMapping("/removeObject/{bucketName}/{objectName}")
    public String delObject(@PathVariable("bucketName") String bucketName, @PathVariable("objectName") String objectName) {
        return minioService.removeObject(bucketName, objectName) == true ? "删除成功" : "删除失败";
    }

    @DeleteMapping("/removeListObject/{bucketName}")
    public String delListObject(@PathVariable("bucketName") String bucketName, @RequestBody List<String> objectNameList) {
        return minioService.removeListObject(bucketName, objectNameList) == true ? "删除成功" : "删除失败";
    }


    @RequestMapping("/download/{bucketName}/{objectName}")
    public void download(HttpServletResponse response, @PathVariable("bucketName") String bucketName, @PathVariable("objectName") String objectName) {
        InputStream in = null;
        try {
            in = minioService.downloadObject(bucketName, objectName);
            response.setHeader("Content-Disposition", "attachment;filename="
                    + URLEncoder.encode(objectName, "UTF-8"));
            response.setCharacterEncoding("UTF-8");
            //将字节从InputStream复制到OutputStream 。
            IOUtils.copy(in, response.getOutputStream());
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }
}
```
<a name="bKs0X"></a>
### 9、测试
上述4个步骤已经整合完成了，下面直接调用接口上传一张图片试一下，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322631265-7d731470-013c-40f1-960f-386390ccdbd3.png#clientId=u2975268f-459d-4&from=paste&id=u268ec15b&originHeight=478&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u941dc897-e3c8-4566-854d-dab24f33182&title=)<br />接口返回的**URL**就是文件的访问地址，直接输入浏览器访问即可。<br />在MInIO中也可以看到存储的文件，如下图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322631345-2c00dfa0-6061-4be4-9482-0ae45b0a1add.png#clientId=u2975268f-459d-4&from=paste&id=u8ff9b5fb&originHeight=446&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7dc8fcae-2632-4388-b591-3a409554f9d&title=)<br />如果需要分享给别人，也可以手动分享，有效期是7天，一旦过了这个有效期将会失效，如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658322631420-cbe09eb8-aac8-4fff-8ebe-a7ea72690425.png#clientId=u2975268f-459d-4&from=paste&id=ua6ffd35b&originHeight=445&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8bfa1857-2b83-4dae-902b-bd59b66ef43&title=)
<a name="ufbeV"></a>
## 总结
MInIO虽然是个开源项目，但是功能非常强大，小型项目中完全可以用它实现对象存储，也可以使用MinIO搭建一个免费的图床。
