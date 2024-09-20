Java
<a name="czVwd"></a>
## 前言
互联网的连接速度慢且不稳定，有可能由于网络故障导致断开连接。<br />在客户端下载一个大对象时，因网络断开导致上传下载失败的概率就会变得不可忽视。<br />![2023-02-08-21-21-00.337918400.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1675862514396-f2fb4545-918d-4eb1-95c0-e33807fd831c.png#averageHue=%23f9f9f8&clientId=u14968f97-c4dc-4&from=ui&id=ufd42edd6&originHeight=456&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1480365&status=done&style=none&taskId=u09e1e252-c893-4107-9427-512acafd03b&title=)<br />客户端在GET对象请求时通过设置Range头部来告诉接口服务需要从什么位置开始输出对象的数据。<br />判断是否支持断点下载，根据文档：14.35.1 Byte Ranges<br />[https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html](https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html)
```java
// 直接判断是否有 Accept-Ranges = bytes
boolean support = urlConnection.getHeaderField("Accept-Ranges").equals("bytes");
System.out.println("Partial content retrieval support = " + (support ? "Yes" : "No));
```
例如：
```bash
donald@donald-pro:~$ curl -i --range 0-9 http://localhost:8080/file/chunk/download
HTTP/1.1 206
Accept-Ranges: bytes
Content-Disposition: inline;filename=pom.xml
Content-Range: bytes 0-9/13485
Content-Length: 10
Date: Mon, 01 Nov 2021 09:53:25 GMT
```
直接判断头部 HEAD，例如：<br />HeadObject 接口用于获取某个文件（Object）的元信息。使用此接口不会返回文件内容。
```http
HEAD /ObjectName HTTP/1.1
Host: BucketName.oss-cn-hangzhou.aliyuncs.com
Date: GMT Date
Authorization: SignatureValue
```
需知，对应 HTTP 状态码：

- 206 Partial Content：HTTP Range 请求成功
- 416 Requested Range Not Satisfiable status：HTTP Range 请求超出界限
- 200 OK：不支持范围请求

小结如下：

1. HTTP 范围请求：需要 HTTP/1.1 及之上支持，如果双端某一段低于此版本，则认为不支持。
2. 通过响应头中的 Accept-Ranges 来确定是否支持范围请求。
3. 通过在请求头中添加 Range 这个请求头，来指定请求的内容实体的字节范围。
4. 在响应头中，通过 Content-Range 来标识当前返回的内容实体范围，并使用 Content-Length 来标识当前返回的内容实体范围长度。
5. 在请求过程中，可以通过 If-Range 来区分资源文件是否变动，它的值来自 ETag 或者 Last-Modifled。如果资源文件有改动，会重新走下载流程。
<a name="uHasY"></a>
## 生产实战
开发也得依靠依据，设定好边界，才能掌控全局。<br />有现成的文档，来看阿里云文档[https://help.aliyun.com/document_detail/39571.html](https://help.aliyun.com/document_detail/39571.html)

- Range: bytes=0-499：表示第0~499字节范围的内容。
- Range: bytes=500-999：表示第500~999字节范围的内容。
- Range: bytes=-500：表示最后500字节的内容。
- Range: bytes=500-：表示从第500字节开始到文件结束部分的内容。
- Range: bytes=0-：表示第一个字节到最后一个字节，即完整的文件内容。

HTTP Range 是否合法对应处理：

- 如果 HTTP Range 请求合法，响应返回值为 206 并在响应头中包含 Content-Range
- 如果 HTTP Range 请求不合法，或者指定范围不在有效区间，会导致 Range 不生效，响应返回值为200，并传送整个 Object 内容。

如下为 HTTP Range 请求不合法的示例及错误说明: 假设 Object 资源大小为1000字节，Range 有效区间为0~999

- Range: byte=0-499：格式错误，byte 应为 bytes。
- Range: bytes=0-1000：末字节 1000 超出有效区间。
- Range: bytes=1000-2000：指定范围超出有效区间。
- Range: bytes=1000-：首字节超出有效区间。
- Range: bytes=-2000：指定范围超出有效区间。

举一些栗子：
```bash
# 正常范围下载
donald@donald-pro:~$ curl -i --range 0-9 http://localhost:8080/file/chunk/download
HTTP/1.1 206 
Accept-Ranges: bytes
Content-Disposition: inline;filename=Screen_Recording_20211101-162729_Settings.mp4
Content-Range: bytes 0-9
Content-Type: application/force-download;charset=UTF-8
Content-Length: 16241985
Date: Wed, 03 Nov 2021 09:50:50 GMT
```
<a name="jPOIP"></a>
## 服务端 - 业务开发
这里以 SpringBoot 为栗子：

1. 对外支持 range 下载
2. 底层存储：使用 ceph
3. Controller 如下：
```java
@Slf4j
@RestController
public class Controller {
    @Autowired
    private FileService fileService;
    
    /**
     * 下载文件
     *
     * 对外提供
     *
     * @param fileId 文件Id
     * @param token token
     * @param accountId 帐号Id
     * @param response 响应
     */
    @GetMapping("/oceanfile/download")
    public void downloadOceanfile(@RequestParam String fileId,
                                  @RequestHeader(value = "Range") String range,
                                  HttpServletResponse response) {

        this.fileService.downloadFile(fileId, response, range);
    }
}
```

1. Service 如下：
```java
@Slf4j
@Service
public class FileService {
    @Autowired
    private CephUtils cephUtils;
    
    /**
     * 直接下载文件
     *
     * Tips： 支持断点下载
     * @param fileId 文件Id
     * @param response 返回
     * @param range 范围
     */
    public void downloadFile(String fileId, HttpServletResponse response, String range) {
        // 根据 fileId 获取文件信息
        FileInfo fileInfo = getFileInfo(fileId);

        String bucketName = fileInfo.getBucketName();
        String relativePath = fileInfo.getRelativePath();

        // 处理 range，范围信息
        RangeDTO rangeInfo = executeRangeInfo(range, fileInfo.getFileSize());

        // rangeInfo = null，直接下载整个文件
        if (Objects.isNull(rangeInfo)) {
        
            cephUtils.downloadFile(response, bucketName, relativePath);
            return;
        }
        // 下载部分文件
        cephUtils.downloadFileWithRange(response, bucketName, relativePath, rangeInfo);
    }

    private RangeDTO executeRangeInfo(String range, Long fileSize) {

        if (StringUtils.isEmpty(range) || !range.contains("bytes=") || !range.contains("-")) {

            return null;
        }

        long startByte = 0;
        long endByte = fileSize - 1;

        range = range.substring(range.lastIndexOf("=") + 1).trim();
        String[] ranges = range.split("-");

        if (ranges.length <= 0 || ranges.length > 2) {

            return null;
        }

        try {
            if (ranges.length == 1) {
                if (range.startsWith("-")) {

                    //1. 如：bytes=-1024  从开始字节到第1024个字节的数据
                    endByte = Long.parseLong(ranges[0]);
                } else if (range.endsWith("-")) {

                    //2. 如：bytes=1024-  第1024个字节到最后字节的数据
                    startByte = Long.parseLong(ranges[0]);
                }
            } else {
                //3. 如：bytes=1024-2048  第1024个字节到2048个字节的数据
                startByte = Long.parseLong(ranges[0]);
                endByte = Long.parseLong(ranges[1]);
            }
        } catch (NumberFormatException e) {
            startByte = 0;
            endByte = fileSize - 1;
        }
        
        if (startByte >= fileSize) {
            
            log.error("range error, startByte >= fileSize. " +
                    "startByte: {}, fileSize: {}", startByte, fileSize);
            return null;
        }

        return new RangeDTO(startByte, endByte);
    }
}
```
