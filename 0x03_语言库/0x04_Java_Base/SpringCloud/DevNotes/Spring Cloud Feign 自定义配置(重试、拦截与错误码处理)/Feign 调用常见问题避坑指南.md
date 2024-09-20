JavaSpringCloudFeign
<a name="RWqBe"></a>
## 1、写在前面
很多时候在使用微服务框架的时候，就基本上少不了与feign打交道。<br />毕竟服务之间的调用，基本上都不会用http调了，这样显得有点麻烦了，feign更方便了。<br />而且feign，还自带了负载均衡的策略(ribbon提供)，如果服务是集群的，feign还能负载调用。<br />在用feign的日常中，难免会遇到些问题，下面来总结下在项目开发中遇到的问题。
<a name="kiHai"></a>
## 2、FEIGN 常见问题
<a name="ZccrO"></a>
### 2.1 400 Bad Request 问题
<a name="TwSUu"></a>
#### 2.1.1 问题描述
在使用feign调用的使用出现400 Bad request的问题。
<a name="sS7u6"></a>
#### 2.1.2 代码如下
```java
@PostMapping("/llsydn/getMenusByIdsAndTypes")
List<SysMenuDto> getMenusByIdsAndTypes(@RequestParam("menuIds") String menuIds,
                                       @RequestParam("menuType") String menuType);
```
这个 menuIds 数量比较多，导致400 错误。发现问题出在menuIds 跟在URL后面。
<a name="jrBqK"></a>
#### 2.1.3 解决办法
将方法修改为：
```java
@PostMapping("/llsydn/getMenusByIdsAndTypes")
List<SysMenuDto> getMenusByIdsAndTypes(@RequestBody MultiValueMap<String,String> queryParam);
```
调用方法修改成：
```java
public List<SysMenuDto> getMenusByIdsAndNotType(String menuIds, String menuType){
    MultiValueMap valueMap=new LinkedMultiValueMap();
    valueMap.add("menuIds",menuIds);
    valueMap.add("menuType",menuType);
    return systemClient.getMenusByIdsAndTypes(valueMap);
}
```
<a name="Zhb0j"></a>
### 2.2. 非法字符错误
<a name="DFHAK"></a>
#### 2.2.1 错误信息
在系统调用系统脚本的接口的时候抛出如下的错误。
```java
Illegal character ((CTRL-CHAR, code 31)): only regular white space (\r, \n, \t) is allowed between tokens
```
<a name="Uh0ib"></a>
#### 2.2.2 错误原因
是feign 调用的时候启用了**压缩**导致的。
<a name="YWrVd"></a>
#### 3.2.3 解决办法

- **1.关闭压缩即可。**

将配置改成：
```
feign.compression.request.enabled=false
feign.compression.response.enabled=false
```

- **2.或者使用okHttp**
```xml
<dependency>
  <groupId>io.github.openfeign</groupId>
  <artifactId>feign-okhttp</artifactId>
</dependency>
```
<a name="axA1q"></a>
### 2.3. 字符串中文乱码问题
<a name="Zfjca"></a>
#### 2.3.1 错误信息
在Feign调用时，传到目标服务的方法中，字符串里的中文变成问号了
<a name="k8Yee"></a>
#### 2.3.2 解决办法
在Feign的接口的注解中指定consumes字符集：
```java
@PostMapping(value = "/portal/core/appdata/install",consumes = "application/json;charset=UTF-8")
void install(@RequestBody String data);
```
如果此时data为`[{},{}]`格式的JSON字符串，即JSON数组字符串，又会报参数类型不匹配的错误，要把参数改为对象数组或者List对象：
```java
@PostMapping(value = "/portal/core/appdata/install",consumes = "application/json;charset=UTF-8")
void install(@RequestBody Object[] data);
```
<a name="XtuLw"></a>
### 2.4. too many Body parameters问题
<a name="PKj7C"></a>
#### 2.4.1 问题描述
feign的post请求只能有一个body feign的post方法中，只能使用一个`@RequestBody`或者不带该注解，不能使用多个`@RequestBody`。<br />否则会报错：nested exception is java.lang.IllegalStateException: Method has too many Body parameters。
<a name="TCmWo"></a>
#### 2.4.2 解决办法
只保留一个`@RequestBody`注解
<a name="JrniB"></a>
### 2.5. Read timed out问题
<a name="mwQFy"></a>
#### 2.5.1 问题描述
feign调用超时，会出现这个问题。<br />一般来说当业务需要处理的时间很大时，会出现这个问题。例如，上传excel文件。<br />那这里可以进行feign的超时时间设置。这里只针对指定的feign client
<a name="BYUbl"></a>
#### 2.5.2 解决办法
```java
@FeignClient(name = "systemClient")
public interface SystemClient {
 
    @RequestMapping(path = "/llsydn/importExcel", consumes = {"multipart/form-data"}) 
    JsonResult importExcel(@RequestPart(name="file") MultipartFile file);
}
```

- **yml配置**
```yaml
feign:
  httpclient:
    enabled: true
  client:
    config:
      default:
        #默认时间设置为10s
        ConnectTimeOut: 10000
        ReadTimeOut: 10000
      #调用system微服务，默认时间设置为30s
      systemClient:
        ConnectTimeOut: 30000
        ReadTimeOut: 30000
```
