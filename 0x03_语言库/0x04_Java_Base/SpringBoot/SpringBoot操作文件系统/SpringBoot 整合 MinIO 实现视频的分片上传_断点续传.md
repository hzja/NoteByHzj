JavaSpringBootMinIO
<a name="u4nXb"></a>
## 1、前言
对于视频文件的上传可以采取断点续传，上传过程中，如果出现网络异常或程序崩溃导致文件上传失败时，将从断点记录处继续上传未上传完成的部分，断点续传依赖于MD5和分片上传，对于本demo分片上传的流程如图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694353430582-c874f504-5947-4e23-84f8-1a4908bd5019.png#averageHue=%23f7f7f6&clientId=u46941620-3e8a-4&from=paste&id=udf621bf9&originHeight=780&originWidth=771&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u76a2a5dc-1ec7-487f-b746-a9cf36b01a7&title=)<br />通过文件唯一标识MD5，在数据库中查询此前是否创建过该SysUploadTask，如果存在，直接返回TaskInfo；如果不存在，通过amazonS3获取到UploadId并新建一个SysUploadTask返回。<br />前端将文件分好片后，通过服务器得到每一片的一个预地址，然后由前端直接向minio服务器发起真正的上传请求，避免上传时占用应用服务器的带宽，影响系统稳定。最后再向后端服务器发起合并请求。
<a name="MhIKz"></a>
## 2、数据库结构
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694353430612-2e054aa3-a9fa-469e-8f43-c4c5b99be80e.png#averageHue=%23f9f8f7&clientId=u46941620-3e8a-4&from=paste&id=u3160d49e&originHeight=267&originWidth=1029&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u49ad343d-f6af-42f5-8d61-deaa34efb39&title=)
<a name="U6Ff8"></a>
## 3、后端实现
<a name="dAwn9"></a>
### 3.1、根据MD5获取是否存在相同文件
Controller层
```java
/**
 * 查询是否上传过，若存在，返回TaskInfoDTO
 * @param identifier 文件md5
 * @return
 */
@GetMapping("/{identifier}")
public GraceJSONResult taskInfo (@PathVariable("identifier") String identifier) {
    return GraceJSONResult.ok(sysUploadTaskService.getTaskInfo(identifier));
}
```
Service层
```java
/**
 * 查询是否上传过，若存在，返回TaskInfoDTO
 * @param identifier
 * @return
 */
public TaskInfoDTO getTaskInfo(String identifier) {
    SysUploadTask task = getByIdentifier(identifier);
    if (task == null) {
        return null;
    }
    TaskInfoDTO result = new TaskInfoDTO().setFinished(true).setTaskRecord(TaskRecordDTO.convertFromEntity(task)).setPath(getPath(task.getBucketName(), task.getObjectKey()));

    boolean doesObjectExist = amazonS3.doesObjectExist(task.getBucketName(), task.getObjectKey());
    if (!doesObjectExist) {
        // 未上传完，返回已上传的分片
        ListPartsRequest listPartsRequest = new ListPartsRequest(task.getBucketName(), task.getObjectKey(), task.getUploadId());
        PartListing partListing = amazonS3.listParts(listPartsRequest);
        result.setFinished(false).getTaskRecord().setExitPartList(partListing.getParts());
    }
    return result;
}
```
<a name="ZxB69"></a>
### 3.2、初始化一个上传任务
Controller层
```java
/**
 * 创建一个上传任务
 * @return
 */
@PostMapping
public GraceJSONResult initTask (@Valid @RequestBody InitTaskParam param) {
    return GraceJSONResult.ok(sysUploadTaskService.initTask(param));
}
```
Service层
```java
/**
 * 初始化一个任务
 */
public TaskInfoDTO initTask(InitTaskParam param) {

    Date currentDate = new Date();
    String bucketName = minioProperties.getBucketName();
    String fileName = param.getFileName();
    String suffix = fileName.substring(fileName.lastIndexOf(".")+1, fileName.length());
    String key = StrUtil.format("{}/{}.{}", DateUtil.format(currentDate, "YYYY-MM-dd"), IdUtil.randomUUID(), suffix);
    String contentType = MediaTypeFactory.getMediaType(key).orElse(MediaType.APPLICATION_OCTET_STREAM).toString();
    ObjectMetadata objectMetadata = new ObjectMetadata();
    objectMetadata.setContentType(contentType);
    InitiateMultipartUploadResult initiateMultipartUploadResult = amazonS3
            .initiateMultipartUpload(new InitiateMultipartUploadRequest(bucketName, key).withObjectMetadata(objectMetadata));
    String uploadId = initiateMultipartUploadResult.getUploadId();

    SysUploadTask task = new SysUploadTask();
    int chunkNum = (int) Math.ceil(param.getTotalSize() * 1.0 / param.getChunkSize());
    task.setBucketName(minioProperties.getBucketName())
            .setChunkNum(chunkNum)
            .setChunkSize(param.getChunkSize())
            .setTotalSize(param.getTotalSize())
            .setFileIdentifier(param.getIdentifier())
            .setFileName(fileName)
            .setObjectKey(key)
            .setUploadId(uploadId);
    sysUploadTaskMapper.insert(task);
    return new TaskInfoDTO().setFinished(false).setTaskRecord(TaskRecordDTO.convertFromEntity(task)).setPath(getPath(bucketName, key));
}
```
<a name="JWBGJ"></a>
### 3.3、获取每个分片的预签名上传地址
Controller层
```java
/**
 * 获取每个分片的预签名上传地址
 * @param identifier
 * @param partNumber
 * @return
 */
@GetMapping("/{identifier}/{partNumber}")
public GraceJSONResult preSignUploadUrl (@PathVariable("identifier") String identifier, @PathVariable("partNumber") Integer partNumber) {
    SysUploadTask task = sysUploadTaskService.getByIdentifier(identifier);
    if (task == null) {
        return GraceJSONResult.error("分片任务不存在");
    }
    Map<String, String> params = new HashMap<>();
    params.put("partNumber", partNumber.toString());
    params.put("uploadId", task.getUploadId());
    return GraceJSONResult.ok(sysUploadTaskService.genPreSignUploadUrl(task.getBucketName(), task.getObjectKey(), params));
}
```
Service层
```java
/**
 * 生成预签名上传url
 * @param bucket 桶名
 * @param objectKey 对象的key
 * @param params 额外的参数
 * @return
 */
public String genPreSignUploadUrl(String bucket, String objectKey, Map<String, String> params) {
    Date currentDate = new Date();
    Date expireDate = DateUtil.offsetMillisecond(currentDate, PRE_SIGN_URL_EXPIRE.intValue());
    GeneratePresignedUrlRequest request = new GeneratePresignedUrlRequest(bucket, objectKey)
            .withExpiration(expireDate).withMethod(HttpMethod.PUT);
    if (params != null) {
        params.forEach((key, val) -> request.addRequestParameter(key, val));
    }
    URL preSignedUrl = amazonS3.generatePresignedUrl(request);
    return preSignedUrl.toString();
}
```
<a name="Z8vbw"></a>
### 3.4、合并分片
Controller层
```java
/**
 * 合并分片
 * @param identifier
 * @return
 */
@PostMapping("/merge/{identifier}")
public GraceJSONResult merge (@PathVariable("identifier") String identifier) {
    sysUploadTaskService.merge(identifier);
    return GraceJSONResult.ok();
}
```
Service层
```java
/**
 * 合并分片
 * @param identifier
 */
public void merge(String identifier) {
    SysUploadTask task = getByIdentifier(identifier);
    if (task == null) {
        throw new RuntimeException("分片任务不存");
    }

    ListPartsRequest listPartsRequest = new ListPartsRequest(task.getBucketName(), task.getObjectKey(), task.getUploadId());
    PartListing partListing = amazonS3.listParts(listPartsRequest);
    List<PartSummary> parts = partListing.getParts();
    if (!task.getChunkNum().equals(parts.size())) {
        // 已上传分块数量与记录中的数量不对应，不能合并分块
        throw new RuntimeException("分片缺失，请重新上传");
    }
    CompleteMultipartUploadRequest completeMultipartUploadRequest = new CompleteMultipartUploadRequest()
            .withUploadId(task.getUploadId())
            .withKey(task.getObjectKey())
            .withBucketName(task.getBucketName())
            .withPartETags(parts.stream().map(partSummary -> new PartETag(partSummary.getPartNumber(), partSummary.getETag())).collect(Collectors.toList()));
    CompleteMultipartUploadResult result = amazonS3.completeMultipartUpload(completeMultipartUploadRequest);
}
```
<a name="IlAlZ"></a>
## 4、分片文件清理问题
视频上传一半不上传了，怎么清理碎片分片。<br />可以考虑在sys_upload_task表中新加一个status字段，表示是否合并分片，默认为false，merge请求结束后变更为true，通过一个定时任务定期清理为status为false的记录。另外MinIO自身对于临时上传的分片，会实施定时清理。
