Java SpringBoot
<a name="SQprN"></a>
### 一、minio简介
> 引用官网：MinIO是根据GNU Affero通用公共许可证v3.0发布的高性能对象存储。它与Amazon S3云存储服务兼容。使用MinIO构建用于机器学习，分析和应用程序数据工作负载的高性能基础架构。

官网地址：[https://min.io/](https://min.io/)<br />文档地址：[https://docs.min.io/](https://docs.min.io/)
<a name="CuGNT"></a>
### 二、使用docker 搭建minio 服务
GNU / Linux和macOS
```bash
docker run -p 9000:9000 \
  --name minio1 \
  -v /mnt/data:/data \
  -e "MINIO_ROOT_USER=AKIAIOSFODNN7EXAMPLE" \
  -e "MINIO_ROOT_PASSWORD=wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY" \
  minio/minio server /data
```
windows
```bash
docker run -p 9000:9000 \
  --name minio1 \
  -v D:\data:/data \
  -e "MINIO_ROOT_USER=AKIAIOSFODNN7EXAMPLE" \
  -e "MINIO_ROOT_PASSWORD=wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY" \
  minio/minio server /data
```

- MINIO_ROOT_USER：为用户key
- MINIO_ROOT_PASSWORD：为用户密钥

以上搭建的都是单机版的。想要了解分布式的方式请查看官网文档。<br />当启动后在浏览器访问http://localhost:9000就可以访问minio的图形化界面了。
<a name="PdEug"></a>
### 三、下面开始搭建SpringBoot环境
初始化一个SpringBoot项目大家都会，这里不多做介绍。
<a name="RpFHV"></a>
#### 需要引入的依赖
```xml
<!-- thymeleaf模板渲染引擎-->
<dependency>
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-thymeleaf</artifactId>
</dependency>
<!-- 操作minio的java客户端-->
<dependency>
  <groupId>io.minio</groupId>
  <artifactId>minio</artifactId>
  <version>8.2.1</version>
</dependency>
<!-- lombok插件-->
<dependency>
  <groupId>org.projectlombok</groupId>
  <artifactId>lombok</artifactId>
  <optional>true</optional>
</dependency>
```
依赖可以官方文档里找：[https://docs.min.io/docs/java-client-quickstart-guide.html](https://docs.min.io/docs/java-client-quickstart-guide.html)
<a name="iFq34"></a>
#### 配置文件
```yaml
spring:
  servlet:
    multipart:
      max-file-size: 10MB
      max-request-size: 10MB
#minio配置
  minio:
    access-key: AKIAIOSFODNN7EXAMPLE      #key就是docker初始化是设置的，密钥相同
    secret-key: wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY
    url: http://localhost:9000
    bucket-name: wdhcr
  thymeleaf:
    cache: false
```
创建minio的配置类:
```java
@Configuration
@ConfigurationProperties(prefix = "spring.minio")
@Data
public class MinioConfiguration {
    private String accessKey;

    private String secretKey;

    private String url;

    private String bucketName;

    @Bean
    public MinioClient minioClient() {
        return MinioClient.builder()
                .endpoint(url)
                .credentials(accessKey, secretKey)
                .build();
    }
}
```
使用配置属性绑定进行参数绑定，并初始化一个minio client对象放入容器中。
<a name="rEmBN"></a>
#### 封装的minio client 操作minio的简单方法的组件
```java
@Component
public class MinioComp {

    @Autowired
    private MinioClient minioClient;

    @Autowired
    private MinioConfiguration configuration;

    /**
     * @description: 获取上传临时签名
     * @dateTime: 2021/5/13 14:12
     */
    public Map getPolicy(String fileName, ZonedDateTime time) {
        PostPolicy postPolicy = new PostPolicy(configuration.getBucketName(), time);
        postPolicy.addEqualsCondition("key", fileName);
        try {
            Map<String, String> map = minioClient.getPresignedPostFormData(postPolicy);
            HashMap<String, String> map1 = new HashMap<>();
            map.forEach((k,v)->{
               map1.put(k.replaceAll("-",""),v);
           });
            map1.put("host",configuration.getUrl()+"/"+configuration.getBucketName());
            return map1;
        } catch (ErrorResponseException e) {
            e.printStackTrace();
        } catch (InsufficientDataException e) {
            e.printStackTrace();
        } catch (InternalException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidResponseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (ServerException e) {
            e.printStackTrace();
        } catch (XmlParserException e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * @description: 获取上传文件的url
     * @dateTime: 2021/5/13 14:15
     */
    public String getPolicyUrl(String objectName, Method method, int time, TimeUnit timeUnit) {
        try {
            return minioClient.getPresignedObjectUrl(GetPresignedObjectUrlArgs.builder()
                    .method(method)
                    .bucket(configuration.getBucketName())
                    .object(objectName)
                    .expiry(time, timeUnit).build());
        } catch (ErrorResponseException e) {
            e.printStackTrace();
        } catch (InsufficientDataException e) {
            e.printStackTrace();
        } catch (InternalException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidResponseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (XmlParserException e) {
            e.printStackTrace();
        } catch (ServerException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * @description: 上传文件
     * @dateTime: 2021/5/13 14:17
     */
    public void upload(MultipartFile file, String fileName) {
        // 使用putObject上传一个文件到存储桶中。
        try {
            InputStream inputStream = file.getInputStream();
            minioClient.putObject(PutObjectArgs.builder()
                    .bucket(configuration.getBucketName())
                    .object(fileName)
                    .stream(inputStream, file.getSize(), -1)
                    .contentType(file.getContentType())
                    .build());
        } catch (ErrorResponseException e) {
            e.printStackTrace();
        } catch (InsufficientDataException e) {
            e.printStackTrace();
        } catch (InternalException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidResponseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (ServerException e) {
            e.printStackTrace();
        } catch (XmlParserException e) {
            e.printStackTrace();
        }
    }
  /**
     * @description: 根据filename获取文件访问地址
     * @dateTime: 2021/5/17 11:28
     */
    public String getUrl(String objectName, int time, TimeUnit timeUnit) {
        String url = null;
        try {
            url = minioClient.getPresignedObjectUrl(GetPresignedObjectUrlArgs.builder()
                    .method(Method.GET)
                    .bucket(configuration.getBucketName())
                    .object(objectName)
                    .expiry(time, timeUnit).build());
        } catch (ErrorResponseException e) {
            e.printStackTrace();
        } catch (InsufficientDataException e) {
            e.printStackTrace();
        } catch (InternalException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidResponseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (XmlParserException e) {
            e.printStackTrace();
        } catch (ServerException e) {
            e.printStackTrace();
        }
        return url;
    }
}
```
简单说明：

- 使用MultipartFile接收前端文件流，再上传到minio。
- 构建一个formData的签名数据，给前端，让前端之前上传到minio。
- 构建一个可以上传的临时URL给前端，前端通过携带文件请求该URL进行上传。
- 使用filename请求服务端获取临时访问文件的URL。（最长时间为7 天，想要永久性访问，需要其他设置，这里不做说明。）
<a name="VYCV6"></a>
#### 展示页面html，使用的是VUE+element-ui进行渲染
```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <!-- import CSS -->
    <link rel="stylesheet" href="https://unpkg.com/element-ui/lib/theme-chalk/index.css">
    <script src="https://cdn.jsdelivr.net/npm/vue@2/dist/vue.js"></script>
    <title>上传图片</title>
</head>
<body>
<div id="app">

    <el-row :gutter="2">
        <el-col :span="8">
            <div class="div-center-class">
                <div class="">
                    <center><h3>传统上传</h3></center>
                    <el-upload
                            class="upload-demo"
                            action="#"
                            drag
                            :http-request="uploadHandle">
                        <i class="el-icon-upload"></i>
                        <div class="el-upload__text">将文件拖到此处，或<em>点击上传</em></div>
                        <div class="el-upload__tip" slot="tip">只能上传jpg/png文件，且不超过500kb</div>
                    </el-upload>
                    <div v-if="imgUrl">
                        <img :src="imgUrl" style="width: 40px;height: 40px"></img>
                    </div>
                </div>
            </div>
        </el-col>
        <el-col :span="8">
            <div class="div-center-class">
                <div class="">
                    <center><h3>前端formData直传</h3></center>
                    <el-upload
                            class="upload-demo"
                            action="#"
                            drag
                            :http-request="httpRequestHandle">
                        <i class="el-icon-upload"></i>
                        <div class="el-upload__text">将文件拖到此处，或<em>点击上传</em></div>
                        <div class="el-upload__tip" slot="tip">只能上传jpg/png文件，且不超过500kb</div>
                    </el-upload>
                    <div v-if="directUrl">
                        <img :src="directUrl" style="width: 40px;height: 40px"></img>
                    </div>
                </div>
            </div>
        </el-col>
        <el-col :span="8">
            <div class="div-center-class">
                <div class="">
                    <center><h3>前端Url直传</h3></center>
                    <el-upload
                            class="upload-demo"
                            action="#"
                            drag
                            :http-request="UrlUploadHandle">
                        <i class="el-icon-upload"></i>
                        <div class="el-upload__text">将文件拖到此处，或<em>点击上传</em></div>
                        <div class="el-upload__tip" slot="tip">只能上传jpg/png文件，且不超过500kb</div>
                    </el-upload>
                    <div v-if="uploadUrl">
                        <img :src="uploadUrl" style="width: 40px;height: 40px"></img>
                    </div>
                </div>
            </div>
        </el-col>
    </el-row>
</div>
</body>
<!-- import Vue before Element -->
<script src="https://unpkg.com/vue/dist/vue.js"></script>
<!-- import JavaScript -->
<script src="https://unpkg.com/element-ui/lib/index.js"></script>
<!--import  axios -->
<script src="https://unpkg.com/axios/dist/axios.min.js"></script>
<script>
    new Vue({
        el: '#app',
        data: function () {
            return {
                imgUrl: '',
                directUrl: '',
                uploadUrl: ''
            }
        },
        methods: {

            uploadHandle(options) {
                let {file} = options;
                this.traditionPost(file);
            },
            traditionPost(file) {
                _that = this
                const form = new FormData();
                form.append("fileName", file.name);
                form.append("file", file);
                this.axiosPost("post", "/upload", form).then(function (res) {
                    if (res.status === 200) {
                        _that.imgUrl = res.data.data
                    } else {
                        alert("上传失败！")
                    }
                })
            },
            getpolicy(file) {
                _that = this
                axios.get('policy?fileName=' + file.name)
                    .then(function (response) {
                        let {xamzalgorithm, xamzcredential, policy, xamzsignature, xamzdate, host} = response.data.data;
                        let formData = new FormData();
                        formData.append("key", file.name);
                        formData.append("x-amz-algorithm", xamzalgorithm);  // 让服务端返回200，不设置则默认返回204。
                        formData.append("x-amz-credential", xamzcredential);
                        formData.append("policy", policy);
                        formData.append("x-amz-signature", xamzsignature);
                        formData.append("x-amz-date", xamzdate);
                        formData.append("file", file);
                        // 发送 POST 请求
                        _that.axiosPost("post", host, formData).then(function (res) {
                            if (res.status === 204) {
                                axios.get('url?fileName=' + file.name).then(function (res) {
                                    _that.directUrl = res.data.data;
                                })
                            } else {
                                alert("上传失败！")
                            }
                        })
                    })
            },
            httpRequestHandle(options) {
                let {file} = options;
                this.getpolicy(file);
            },

            UrlUploadHandle(options) {
                let {file} = options;
                this.getUploadUrl(file);
            },
            getUploadUrl(file) {
                _that = this
                console.log(file)
                axios.get('uploadUrl?fileName=' + file.name)
                    .then(function (response) {
                        let url = response.data.data;
                        // 发送 put 请求
                        let config = {'Content-Type': file.type}
                        _that.axiosPost("put", url, file, config).then(function (res) {
                            if (res.status === 200) {
                                axios.get('url?fileName=' + file.name).then(function (res) {
                                    _that.uploadUrl = res.data.data;
                                })
                            } else {
                                alert("上传失败！")
                            }
                        })
                    })
            },
            //封装
            //axios封装post请求
            axiosPost(method, url, data, config) {
                let result = axios({
                    method: method,
                    url: url,
                    data: data,
                    headers: config
                }).then(resp => {
                    return resp
                }).catch(error => {
                    return "exception=" + error;
                });
                return result;
            }

        }
    })
</script>
<style>
    .div-center-class {
        padding: 28% 0%;
        text-align: center;
        background: beige;
    }
</style>
</html>
```
![页面效果](https://cdn.nlark.com/yuque/0/2022/png/396745/1644552366066-72b108bf-b0db-46a2-80b5-62f5ff5a8f16.png#averageHue=%23f8f8ea&clientId=ub5cdbeee-3296-4&from=paste&height=713&id=u64ba8081&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=true&size=144076&status=done&style=shadow&taskId=u1ecc9ac4-47f3-42e5-87f5-f622b520d75&title=%E9%A1%B5%E9%9D%A2%E6%95%88%E6%9E%9C&width=1536 "页面效果")<br />可以分别体验不同的实现效果。<br />以上就是使用SpringBoot搭建基于minio的高性能存储服务的全部步骤了。
