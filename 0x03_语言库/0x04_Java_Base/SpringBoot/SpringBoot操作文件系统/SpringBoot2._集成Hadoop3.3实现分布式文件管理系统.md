SpringBoot2.* Hadoop3.3
<a name="pF2uO"></a>
## 前言：HDFS安装参照
[CentOS7搭建HDFS（Hadoop3.3）](https://www.yuque.com/fcant/bigdata/rx47e6?view=doc_embed)
<a name="yqGe5"></a>
## 1、在SpringBoot项目中添加Hadoop客户端操作的pom依赖
:::tips
Hadoop核心依赖包采用log4j方式记录日志，spring-boot-start-web 核心jar包日志采用logback 方式记录。这里采用spring-boot-start-web的日志记录方式而排除log4j的相关依赖
:::
```xml
<dependency>
  <groupId>org.apache.hadoop</groupId>
  <artifactId>hadoop-common</artifactId>
  <version>3.3.0</version>
  <exclusions>
    <exclusion>
      <groupId>org.slf4j</groupId>
      <artifactId>slf4j-log4j12</artifactId>
    </exclusion>
  </exclusions>
</dependency>
<dependency>
  <groupId>org.apache.hadoop</groupId>
  <artifactId>hadoop-hdfs</artifactId>
  <version>3.3.0</version>
  <exclusions>
    <exclusion>
      <groupId>org.slf4j</groupId>
      <artifactId>slf4j-log4j12</artifactId>
    </exclusion>
  </exclusions>
</dependency>
<dependency>
  <groupId>org.apache.hadoop</groupId>
  <artifactId>hadoop-client</artifactId>
  <version>3.3.0</version>
  <exclusions>
    <exclusion>
      <groupId>org.slf4j</groupId>
      <artifactId>slf4j-log4j12</artifactId>
    </exclusion>
  </exclusions>
</dependency>
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>fastjson</artifactId>
  <version>1.2.72</version>
</dependency>
```
<a name="18vft"></a>
## 2、Hadoop Client需要做的配置
<a name="KmISH"></a>
### 在客户端环境变量配置`HADOOP_USER_NAME`
因为在启动Hadoop时，使用root用户启动，Hadoop客户端会读取环境变量中配置的用户名，如果没有则默认会读取系统计算机管理员名称，会报无权限的异常。<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1608649099869-1ca15fd7-219e-4e5e-b5bd-e86cca6ec82d.png#height=479&id=g2NAm&originHeight=1438&originWidth=1584&originalType=binary&size=174229&status=done&style=shadow&width=528)
<a name="z7zAX"></a>
### 如果是访问云主机，则需要在本地配置主机名称和公网IP的映射
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621829698366-fae856ec-92ae-40fb-b9c2-4782927cd284.png#clientId=u1b06b9c3-860a-4&from=paste&height=583&id=u35dcc3ff&originHeight=1750&originWidth=3323&originalType=binary&size=2044495&status=done&style=none&taskId=uff84af24-f259-4327-9d5e-f7d7edc3219&width=1107.6666666666667)
<a name="75nDW"></a>
## 3、在Java中配置HDFS的信息
<a name="fY2nz"></a>
### A.在ymal配置文件配置主机信息
```yaml
application:
  config:
    hdfs:
      host: hdfs://120.79.178.68:9100
```
<a name="6gVWJ"></a>
### B.在Config配置文件引用配置参数即可
:::info
以下几个配置项说明：

1. `dfs.replication`：设置副本的数量，在JAVA API客户端设置该参数只是为了和服务端启动时设置的副本数量保持一致，不设置则默认为3个副本。
2. `dfs.client.use.datanode.hostname`：解决服务Java 操作HDFS时，NameNode 和DataNode 是通过局域网进行通信，NameNode返回地址为 DataNode 的私有 IP，本地无法访问的问题。
:::
> 问题的原因：NameNode节点存放的是文件目录，也就是文件夹、文件名称，本地可以通过公网访问 NameNode，所以可以进行文件夹的创建，当上传文件需要写入数据到DataNode时，NameNode 和DataNode 是通过局域网进行通信，NameNode返回地址为 DataNode 的私有 IP，本地无法访问。

```java
import io.renren.service.HdfsService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * HDFS相关配置
 *
 * @author Fcant
 * @since 1.0.0
 */
@Configuration
public class HdfsConfig {
    @Value("${application.config.hdfs.host}")
    private String defaultHdfsUri;

    @Bean
    public HdfsService getHbaseService(){
        org.apache.hadoop.conf.Configuration conf = new org.apache.hadoop.conf.Configuration();
        conf.set("fs.defaultFS",defaultHdfsUri);
        conf.set("dfs.client.use.datanode.hostname", "true");
        conf.set("dfs.replication", "1");
        return new HdfsService(conf,defaultHdfsUri);
    }
}
```
<a name="cdfmt"></a>
## 4、通用Java操作HDFS的Service
```java
import com.alibaba.fastjson.JSON;
import org.apache.commons.io.IOUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.PathFilter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.charset.Charset;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * HDFS相关的基本操作
 *
 * @author Fcant
 * @since 1.0.0
 */
public class HdfsService {

    private Logger logger = LoggerFactory.getLogger(HdfsService.class);
    private Configuration conf = null;

    /**
     * 默认的HDFS路径，比如：hdfs://192.168.197.130:9000
     */
    private String defaultHdfsUri;

    public HdfsService(Configuration conf,String defaultHdfsUri) {
        this.conf = conf;
        this.defaultHdfsUri = defaultHdfsUri;
    }

    /**
     * 获取HDFS文件系统
     * @return org.apache.hadoop.fs.FileSystem
     */
    private FileSystem getFileSystem() throws IOException {
        return FileSystem.get(conf);
    }

    /**
     * 创建HDFS目录
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir
     * @return boolean 是否创建成功
     */
    public boolean mkdir(String path){
        //如果目录已经存在，则直接返回
        if(checkExists(path)){
            return true;
        }else{
            FileSystem fileSystem = null;

            try {
                fileSystem = getFileSystem();

                //最终的HDFS文件目录
                String hdfsPath = generateHdfsPath(path);
                //创建目录
                return fileSystem.mkdirs(new Path(hdfsPath));
            } catch (IOException e) {
                logger.error(MessageFormat.format("创建HDFS目录失败，path:{0}",path),e);
                return false;
            }finally {
                close(fileSystem);
            }
        }
    }
    
    /**
     * 接口上传的文件上传到HDFS
     *
     * @param filePath 文件路径及文件名 Example：/business/2020/12/23/fbfc71d2-7/hello.png
     * @param file 接口收到的文件
     * @author Fcant 上午 10:36 2020/12/23/0023
     */
    public void uploadFileToHdfs(String filePath, MultipartFile file) {
        Path path = new Path(filePath);
        FSDataOutputStream outputStream;
        try {
            outputStream = getFileSystem().create(path);
            outputStream.write(file.getBytes());
            outputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 上传文件至HDFS
     * @author Fcant
     * @since 1.0.0
     * @param srcFile 本地文件路径，比如：D:/test.txt
     * @param dstPath HDFS的相对目录路径，比如：/testDir
     */
    public void uploadFileToHdfs(String srcFile, String dstPath){
        this.uploadFileToHdfs(false, true, srcFile, dstPath);
    }

    /**
     * 上传文件至HDFS
     * @author Fcant
     * @since 1.0.0
     * @param delSrc 是否删除本地文件
     * @param overwrite 是否覆盖HDFS上面的文件
     * @param srcFile 本地文件路径，比如：D:/test.txt
     * @param dstPath HDFS的相对目录路径，比如：/testDir
     */
    public void uploadFileToHdfs(boolean delSrc, boolean overwrite, String srcFile, String dstPath){
        //源文件路径
        Path localSrcPath = new Path(srcFile);
        //目标文件路径
        Path hdfsDstPath = new Path(generateHdfsPath(dstPath));

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            fileSystem.copyFromLocalFile(delSrc,overwrite,localSrcPath,hdfsDstPath);
        } catch (IOException e) {
            logger.error(MessageFormat.format("上传文件至HDFS失败，srcFile:{0},dstPath:{1}",srcFile,dstPath),e);
        }finally {
            close(fileSystem);
        }
    }

    /**
     * 判断文件或者目录是否在HDFS上面存在
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir、/testDir/a.txt
     * @return boolean
     */
    public boolean checkExists(String path){
        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            //最终的HDFS文件目录
            String hdfsPath = generateHdfsPath(path);

            //创建目录
            return fileSystem.exists(new Path(hdfsPath));
        } catch (IOException e) {
            logger.error(MessageFormat.format("'判断文件或者目录是否在HDFS上面存在'失败，path:{0}",path),e);
            return false;
        }finally {
            close(fileSystem);
        }
    }

    /**
     * 获取HDFS上面的某个路径下面的所有文件或目录（不包含子目录）信息
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir
     * @return java.util.List<java.util.Map<java.lang.String,java.lang.Object>>
     */
    public List<Map<String,Object>> listFiles(String path, PathFilter pathFilter){
        //返回数据
        List<Map<String,Object>> result = new ArrayList<>();

        //如果目录已经存在，则继续操作
        if(checkExists(path)){
            FileSystem fileSystem = null;

            try {
                fileSystem = getFileSystem();

                //最终的HDFS文件目录
                String hdfsPath = generateHdfsPath(path);

                FileStatus[] statuses;
                //根据Path过滤器查询
                if(pathFilter != null){
                    statuses = fileSystem.listStatus(new Path(hdfsPath),pathFilter);
                }else{
                    statuses = fileSystem.listStatus(new Path(hdfsPath));
                }

                if(statuses != null){
                    for(FileStatus status : statuses){
                        //每个文件的属性
                        Map<String,Object> fileMap = new HashMap<>(2);

                        fileMap.put("path",status.getPath().toString());
                        fileMap.put("isDir",status.isDirectory());

                        result.add(fileMap);
                    }
                }
            } catch (IOException e) {
                logger.error(MessageFormat.format("获取HDFS上面的某个路径下面的所有文件失败，path:{0}",path),e);
            }finally {
                close(fileSystem);
            }
        }

        return result;
    }


    /**
     * 从HDFS下载文件至本地
     * @author Fcant
     * @since 1.0.0
     * @param srcFile HDFS的相对目录路径，比如：/testDir/a.txt
     * @param dstFile 下载之后本地文件路径（如果本地文件目录不存在，则会自动创建），比如：D:/test.txt
     */
    public void downloadFileFromHdfs(String srcFile, String dstFile){
        //HDFS文件路径
        Path hdfsSrcPath = new Path(generateHdfsPath(srcFile));
        //下载之后本地文件路径
        Path localDstPath = new Path(dstFile);

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            fileSystem.copyToLocalFile(hdfsSrcPath,localDstPath);
        } catch (IOException e) {
            logger.error(MessageFormat.format("从HDFS下载文件至本地失败，srcFile:{0},dstFile:{1}",srcFile,dstFile),e);
        }finally {
            close(fileSystem);
        }
    }

    /**
     * 打开HDFS上面的文件并返回 InputStream
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/c.txt
     * @return FSDataInputStream
     */
    public FSDataInputStream open(String path){
        //HDFS文件路径
        Path hdfsPath = new Path(generateHdfsPath(path));

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            return fileSystem.open(hdfsPath);
        } catch (IOException e) {
            logger.error(MessageFormat.format("打开HDFS上面的文件失败，path:{0}",path),e);
        }

        return null;
    }

    /**
     * 打开HDFS上面的文件并返回byte数组，方便Web端下载文件
     * <p>new ResponseEntity<byte[]>(byte数组, headers, HttpStatus.CREATED);</p>
     * <p>或者：new ResponseEntity<byte[]>(FileUtils.readFileToByteArray(templateFile), headers, HttpStatus.CREATED);</p>
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/b.txt
     * @return FSDataInputStream
     */
    public byte[] openWithBytes(String path){
        //HDFS文件路径
        Path hdfsPath = new Path(generateHdfsPath(path));

        FileSystem fileSystem = null;
        FSDataInputStream inputStream = null;
        try {
            fileSystem = getFileSystem();
            inputStream = fileSystem.open(hdfsPath);

            return IOUtils.toByteArray(inputStream);
        } catch (IOException e) {
            logger.error(MessageFormat.format("打开HDFS上面的文件失败，path:{0}",path),e);
        }finally {
            if(inputStream != null){
                try {
                    inputStream.close();
                } catch (IOException e) {
                    // ignore
                }
            }
        }

        return null;
    }

    /**
     * 打开HDFS上面的文件并返回String字符串
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/b.txt
     * @return FSDataInputStream
     */
    public String openWithString(String path){
        //HDFS文件路径
        Path hdfsPath = new Path(generateHdfsPath(path));

        FileSystem fileSystem = null;
        FSDataInputStream inputStream = null;
        try {
            fileSystem = getFileSystem();
            inputStream = fileSystem.open(hdfsPath);

            return IOUtils.toString(inputStream, Charset.forName("UTF-8"));
        } catch (IOException e) {
            logger.error(MessageFormat.format("打开HDFS上面的文件失败，path:{0}",path),e);
        }finally {
            if(inputStream != null){
                try {
                    inputStream.close();
                } catch (IOException e) {
                    // ignore
                }
            }
        }

        return null;
    }

    /**
     * 打开HDFS上面的文件并转换为Java对象（需要HDFS上门的文件内容为JSON字符串）
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/c.txt
     * @return FSDataInputStream
     */
    public <T extends Object> T openWithObject(String path, Class<T> clazz){
        //1、获得文件的json字符串
        String jsonStr = this.openWithString(path);

        //2、使用com.alibaba.fastjson.JSON将json字符串转化为Java对象并返回
        return JSON.parseObject(jsonStr, clazz);
    }

    /**
     * 重命名
     * @author Fcant
     * @since 1.0.0
     * @param srcFile 重命名之前的HDFS的相对目录路径，比如：/testDir/b.txt
     * @param dstFile 重命名之后的HDFS的相对目录路径，比如：/testDir/b_new.txt
     */
    public boolean rename(String srcFile, String dstFile) {
        //HDFS文件路径
        Path srcFilePath = new Path(generateHdfsPath(srcFile));
        //下载之后本地文件路径
        Path dstFilePath = new Path(dstFile);

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            return fileSystem.rename(srcFilePath,dstFilePath);
        } catch (IOException e) {
            logger.error(MessageFormat.format("重命名失败，srcFile:{0},dstFile:{1}",srcFile,dstFile),e);
        }finally {
            close(fileSystem);
        }

        return false;
    }

    /**
     * 删除HDFS文件或目录
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/c.txt
     * @return boolean
     */
    public boolean delete(String path) {
        //HDFS文件路径
        Path hdfsPath = new Path(generateHdfsPath(path));

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();

            return fileSystem.delete(hdfsPath,true);
        } catch (IOException e) {
            logger.error(MessageFormat.format("删除HDFS文件或目录失败，path:{0}",path),e);
        }finally {
            close(fileSystem);
        }

        return false;
    }

    /**
     * 获取某个文件在HDFS集群的位置
     * @author Fcant
     * @since 1.0.0
     * @param path HDFS的相对目录路径，比如：/testDir/a.txt
     * @return org.apache.hadoop.fs.BlockLocation[]
     */
    public BlockLocation[] getFileBlockLocations(String path) {
        //HDFS文件路径
        Path hdfsPath = new Path(generateHdfsPath(path));

        FileSystem fileSystem = null;
        try {
            fileSystem = getFileSystem();
            FileStatus fileStatus = fileSystem.getFileStatus(hdfsPath);

            return fileSystem.getFileBlockLocations(fileStatus, 0, fileStatus.getLen());
        } catch (IOException e) {
            logger.error(MessageFormat.format("获取某个文件在HDFS集群的位置失败，path:{0}",path),e);
        }finally {
            close(fileSystem);
        }

        return null;
    }


    /**
     * 将相对路径转化为HDFS文件路径
     * @author Fcant
     * @since 1.0.0
     * @param dstPath 相对路径，比如：/data
     * @return java.lang.String
     */
    private String generateHdfsPath(String dstPath){
        String hdfsPath = defaultHdfsUri;
        if(dstPath.startsWith("/")){
            hdfsPath += dstPath;
        }else{
            hdfsPath = hdfsPath + "/" + dstPath;
        }

        return hdfsPath;
    }

    /**
     * close方法
     */
    private void close(FileSystem fileSystem){
        if(fileSystem != null){
            try {
                fileSystem.close();
            } catch (IOException e) {
                logger.error(e.getMessage());
            }
        }
    }
}
```
