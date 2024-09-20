JavaSpringMVC
<a name="jy6Pk"></a>
## 1、SpringMVC 文件上传步骤
<a name="AN5GT"></a>
### 4 个步骤
1、添加 Maven 配置<br />2、SpringMVC 容器中定义 `MultipartResolver` 这个 bean<br />3、Controller 中使用 `MultipartFile` 接收上传的文件<br />4、调用 `MultipartFile#transferTo` 方法保存文件<br />5、指定请求类型为：multipart/form-data
<a name="YT0D9"></a>
### 步骤 1：添加 Maven 配置
```xml
<dependency>
  <groupId>commons-fileupload</groupId>
  <artifactId>commons-fileupload</artifactId>
  <version>1.4</version>
</dependency>
<dependency>
  <groupId>commons-io</groupId>
  <artifactId>commons-io</artifactId>
  <version>2.6</version>
</dependency>
```
<a name="rbm0i"></a>
### 步骤 2：定义 `MultipartResolver` 这个 bean
```xml
<bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver">   
  <property name="maxUploadSizePerFile" value="#{10*1024*1024}"/>
  <property name="maxUploadSize" value="#{100*1024*1024}"/>
</bean>
```

1. http 上传的请求类型为 multipart/form-data 类型，SpringMVC 内部需要为这种请求指定解析器，解析器的类型为`org.springframework.web.multipart.MultipartResolver`
2. `MultipartResolver` 有 2 个实现类，这里使用 `CommonsMultipartResolver` 作为解析器来解析文件上传的 http 请求
3. 注意 bean 名称必须为`multipartResolver`
4. 两个比较有用的属性
- `maxUploadSizePerFile`：单个文件大小限制（byte）
- `maxUploadSize`：整个请求大小限制（byte）
<a name="FTfju"></a>
### 步骤 3：控制器使用 MultipartFile 接收上传的文件
```java
@RequestMapping("/upload1.do")
public ModelAndView upload1(@RequestParam("file1") MultipartFile f1){
}
```
<a name="qubzw"></a>
### 步骤 4：调用 MultipartFile#transferTo 方法保存文件
```java
@RequestMapping("/upload1.do")
public ModelAndView upload1(@RequestParam("file1") MultipartFile f1){
    //destFile为目标文件，即将上传的文件写到destFile中
    f1.transferTo(destFile);
}
```
<a name="FJcAK"></a>
### 步骤 5：设置 http 请求类型为 multipart/form-data
上传文件，需要设置 form 表单的 enctype 属性值为 multipart/form-data<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901474778-6c654721-05f6-43bc-be31-cd78489a96f2.png#averageHue=%23fdfdfb&clientId=u83343c39-5fe1-4&from=paste&id=u90d25c51&originHeight=121&originWidth=802&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9ed71601-160c-479c-a3f9-8ae11f1469a&title=)<br />下面来介绍 4 种常用的上传文件的方式。
<a name="jqZoK"></a>
## 2、`MultipartFile`：上传的文件对象
SpringMVC 中使`MultipartFile`这个类来表示上传的文件，提供了一系列方法来获取上传的文件的信息。

| 方法 | 描述 |
| --- | --- |
| `String getName()` | 用来获取中 name 的名称 |
| `String getOriginalFilename()` | 获取文件的原始名称 |
| `String getContentType()` | 获取文件类型 |
| `long getSize()` | 获取文件大小（byte） |
| `byte[] getBytes()` | 获取文件内容 |
| `InputStream getInputStream()` | 获取文件流 |
| `void transferTo(File dest)` | 将上传的文件写到 dest 中 |

<a name="iaVJT"></a>
## 3、单文件上传
控制器中使用一个 MultipartFile 来接收上传的文件，下面看代码。
<a name="tsue9"></a>
### 表单代码
```html
<form method="post" action="upload1.do" enctype="multipart/form-data">
  文件：<input type="file" name="file1"><br/>
  <input type="submit" value="提交">
</form>
```
表单中文件元素的 name 为 file1
<a name="Tiq6J"></a>
### 控制器代码
```java
/**
 * 单文件上传
 * 1、MultipartFile用来接收表单中上传的文件
 * 2、每个MultipartFile对应表单中的一个元素
 * 3、@RequestParam("f1")用来自动接受表单中的哪个元素？value用来指定表单元素的名称
 *
 * @param f1
 * @return
 * @throws IOException
 */
@RequestMapping("/upload1.do")
public ModelAndView upload1(@RequestParam("file1") MultipartFile f1) throws IOException {
    //获取文件名称
    String originalFilename = f1.getOriginalFilename();
    String destFilePath = String.format("E:\\idea\\springmvc-series\\chat04-uploadfile\\src\\main\\webapp\\upfile\\%s", originalFilename);
    File destFile = new File(destFilePath);
    //调用transferTo将上传的文件保存到指定的地址
    f1.transferTo(destFile);

    ModelAndView modelAndView = new ModelAndView();
    modelAndView.setViewName("/WEB-INF/view/result.jsp");
    modelAndView.addObject("msg", destFile.getAbsolutePath());
    return modelAndView;
}
```
上传的文件会被传入 update1 方法的第一个参数，注意第一个参数有个`@RequestParam("file1")`注解，这个注解的 value 就是表单中文件元素名称。
<a name="lgtST"></a>
## 4、多文件上传
当上传多个文件的时候，可以使用多个 `MultipartFile` 参数来接收上传的文件。
<a name="xb9Vq"></a>
### 表单代码
```html
<form method="post" action="upload2.do" enctype="multipart/form-data">
  文件1：<input type="file" name="file1"><br/>
  文件2：<input type="file" name="file2"><br/>
  <input type="submit" value="提交">
</form>
```
<a name="rAPCp"></a>
### 控制器代码
```java
/**
 * 多文件上传
 * 1、方法中指定多个MultipartFile，每个MultipartFile对应一个上传的文件
 * 2、@RequestParam("file1") 用来指定具体接受上传的表单中哪个元素的名称
 *
 * @param f1
 * @param f2
 * @return
 */
@RequestMapping("/upload2.do")
public ModelAndView upload2(@RequestParam("file1") MultipartFile f1,
                            @RequestParam("file2") MultipartFile f2) {
    System.out.println("f1：" + f1);
    System.out.println("f2：" + f2);
    ModelAndView modelAndView = new ModelAndView();
    modelAndView.setViewName("/WEB-INF/view/result.jsp");
    modelAndView.addObject("msg", null);
    return modelAndView;
}
```
上传 2 个图片，控制台输出
```
f1：MultipartFile[field="file1", filename=1.jpg, contentType=image/jpeg, size=145615]
f2：MultipartFile[field="file2", filename=2.jpg, contentType=image/jpeg, size=67713]
```
<a name="WqFcf"></a>
## 5、通过 `MultipartHttpServletRequest` 处理文件上传
<a name="sZb0h"></a>
### `MultipartHttpServletRequest` 接口
1、SpringMVC 接受到上传文件的的请求之后，会将请求转换为 `MultipartHttpServletRequest` 类型的对象<br />2、`MultipartHttpServletRequest` 中提供了一系列方法来获取请求中的所有参数信息<br />3、其中 `getParameterMap()`用来获取非文件类型的参数列表<br />4、`getMultiFileMap()`方法用来获取上传的文件列表<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901474876-2b759de2-f69b-4158-ba92-8d70b9d15010.png#averageHue=%23f6f5ef&clientId=u83343c39-5fe1-4&from=paste&id=u5dce8f45&originHeight=270&originWidth=524&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf7b9fd15-7b0a-4e53-8767-687c47ec91c&title=)<br />下面来个案例，使用 `MultipartHttpServletRequest` 来处理上传的文件请求。
<a name="m5pIg"></a>
### 表单代码
下面表单中有 2 个文本，2 个文件元素。
```html
<form method="post" action="upload3.do" enctype="multipart/form-data">
  姓名：<input name="name" value="路人"/> <br/>
  年龄：<input name="age" value="30"/><br/>
  文件1：<input type="file" name="file1"><br/>
  文件2：<input type="file" name="file2"><br/>
  <input type="submit" value="提交">
</form>
```
<a name="SSQJN"></a>
### 控制器代码
控制器中使用 `MultipartHttpServletRequest` 来获取所有参数信息，分了 2 部分获取<br />1、先使用 `request.getParameterMap()`获取非文件类型的参数，即可以获取表单中的 name 和 age 这 2 个参数的信息<br />2、通过 `request.getMultiFileMap()`获取文件类型的参数，即可以获取表单中 file1 和 file2 这 2 个文件的信息<br />稍后关注控制台的输出
```java
/**
 * 使用MultipartHttpServletRequest处理多文件上传
 * 上传文件的http请求会被转换为MultipartHttpServletRequest类型
 * MultipartHttpServletRequest中提供了很多很多方法用来获取请求中的参数
 *
 * @param request
 * @return
 */
@RequestMapping("/upload3.do")
public ModelAndView upload3(MultipartHttpServletRequest request) {
    //1.获取表单中非文件数据
    System.out.println("---------获取表单中非文件数据---------");
    Map parameterMap = request.getParameterMap();
    parameterMap.forEach((name, values) -> {
        System.out.println(String.format("%s:%s", name, Arrays.asList(values)));
    });
    //2、获取表单中文件数据
    System.out.println("---------获取表单中文件数据---------");
    MultiValueMap multiFileMap = request.getMultiFileMap();
    //2、遍历表单中元素信息
    multiFileMap.forEach((name, files) -> {
        System.out.println(String.format("%s:%s", name, files));
    });

    ModelAndView modelAndView = new ModelAndView();
    modelAndView.setViewName("/WEB-INF/view/result.jsp");
    modelAndView.addObject("msg", "上传成功");
    return modelAndView;
}
```
<a name="n53lq"></a>
### 使用 http client 发起请求
这里使用 IDEA 中的 http client 这个插件来调用接口，http client 插件是一个非常方便发起 http 请求的一个插件，测试接口特别容易，在 idea 中创建一个 UploadController.http 文件，文件内容如下：
```http
### 多文件上传
POST http://localhost:8080/chat04/upload3.do
Content-Type: multipart/form-data; boundary=bound1

--bound1
Content-Disposition: form-data; name="name"

路人
--bound1
Content-Disposition: form-data; name="age"

30
--bound1
Content-Disposition: form-data; name="file1"; filename="1.jpg"

< ./1.jpg
--bound1
Content-Disposition: form-data; name="file2"; filename="2.jpg"
Content-Type: image/jpeg

< ./2.jpg
--bound1--
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901474725-b762c17b-8c9b-4448-8743-104384d2c442.png#averageHue=%23faf9f8&clientId=u83343c39-5fe1-4&from=paste&id=u8c04e4a6&originHeight=198&originWidth=625&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucf40d6ff-9700-4e6f-beab-55b560ce4ab&title=)
<a name="DALKh"></a>
### 控制台输出
```
---------获取表单中非文件数据---------
name:[路人]
age:[30]
---------获取表单中文件数据---------
file1:[MultipartFile[field="file1", filename=1.jpg, contentType=*/*; charset=UTF-8, size=145615]]
file2:[MultipartFile[field="file2", filename=2.jpg, contentType=image/jpeg, size=67713]]
```
<a name="XTpW8"></a>
## 6、自定义对象接收多文件上传
<a name="ePCMU"></a>
### 表单代码
这个表单用来输入用户信息:<br />姓名、年龄、头像图片、2 张身份证图片
```html
<form method="post" action="upload4.do" enctype="multipart/form-data">
  姓名：<input name="name" value="路人"/> <br/>
  年龄：<input name="age" value="30"/><br/>
  头像图片：<input name="headImg" type="file"/><br/>
  多张身份证图片<br/>
  <input name="idCardImg" type="file"/><br/>
  <input name="idCardImg" type="file"/><br/>
  <input type="submit" value="提交">
</form>
```
<a name="QsqKT"></a>
### 自定义一个类
自定义了一个 UserDto，来接收上面表单的参数。
```java
import org.springframework.web.multipart.MultipartFile;

import java.util.List;

public class UserDto {
    //姓名
    private String name;
    //年龄
    private Integer age;
    //头像
    private MultipartFile headImg;
    //身份证（多张图像）
    private List idCardImg;

    //省略了get、set方法...
}
```
<a name="bfLWa"></a>
### 控制器代码
```java
@RequestMapping("/upload4.do")
public ModelAndView upload4(UserDto userDto) {
    System.out.println("姓名：" + userDto.getName());
    System.out.println("年龄：" + userDto.getAge());
    System.out.println("头像文件：" + userDto.getHeadImg());
    System.out.println("多张身份证文件：" + Arrays.asList(userDto.getIdCardImg()));

    ModelAndView modelAndView = new ModelAndView();
    modelAndView.setViewName("/WEB-INF/view/result.jsp");
    modelAndView.addObject("msg", "上传成功");
    return modelAndView;
}
```
<a name="bTTlR"></a>
### 测试代码
这里还是使用 http client 插件发起请求
```http
### 多文件上传
POST http://localhost:8080/chat04/upload4.do
Content-Type: multipart/form-data; boundary=bound1

--bound1
Content-Disposition: form-data; name="name"
Content-Type: text/plain

路人
--bound1
Content-Disposition: form-data; name="age"
Content-Type: text/plain

30
--bound1
Content-Disposition: form-data; name="headImg"; filename="1.jpg"

< ./1.jpg
--bound1
Content-Disposition: form-data; name="idCardImg"; filename="2.jpg"
Content-Type: image/jpeg

< ./2.jpg
--bound1
Content-Disposition: form-data; name="idCardImg"; filename="3.jpg"
Content-Type: image/jpeg

< ./3.jpg
--bound1--
```
<a name="LhLp6"></a>
### 控制台输出
输出如下，可以看到 UserDto 这个对象中的详细信息
```
姓名：路人
年龄：30
头像文件：MultipartFile[field="headImg", filename=1.jpg, contentType=*/*; charset=UTF-8, size=145615]
多张身份证文件：[[MultipartFile[field="idCardImg", filename=2.jpg, contentType=image/jpeg, size=67713], MultipartFile[field="idCardImg", filename=3.jpg, contentType=image/jpeg, size=39891]]]
```
<a name="wtjIN"></a>
## 7、扩展
`**MultipartResolver**`** 这个 bean 的名称为什么必须是 **`**multipartResolver**`**？**<br />SpringMVC 中会使用 `MultipartResolver` 来解析上传文件的请求，具体代码在`org.springframework.web.servlet.DispatcherServlet#doDispatch`中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901474698-855c7fe0-72c4-4caa-9c32-27f31f5ceede.png#averageHue=%23f8f7f4&clientId=u83343c39-5fe1-4&from=paste&id=u3ba2d69e&originHeight=286&originWidth=880&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u13c2c4e9-1d47-4187-905f-7ef79138ade&title=)<br />进入`checkMultipart`，如下，这里面关键代码就是会调用`this.multipartResolver.resolveMultipart(request)`来处理文件上传的请求<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901474665-41e47b5f-84fe-42e0-949a-28dc5f0bb489.png#averageHue=%23fcfbf9&clientId=u83343c39-5fe1-4&from=paste&id=ud052b3ab&originHeight=451&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u05caea6e-bbfc-44e1-a93a-e3d7913ad63&title=)<br />下面看一下 `this.multipartResolver` 从哪里来的，如下，是从 SpringMVC 容器中查找的，名称为`multipartResolver`，所以定义这个 bean 的时候，名称必须为这个。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683901475284-835ae926-8224-41b8-a1a7-df88679f9812.png#averageHue=%23faf9f7&clientId=u83343c39-5fe1-4&from=paste&id=ua1e752d7&originHeight=466&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1d9d5386-7ffd-4882-9bb3-e7fb0a675c7&title=)
