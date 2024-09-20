JavaSpring
<a name="OHBSZ"></a>
## 1、RestTemplate 概述
发送 http 请求，估计很多人用过 httpclient 和 okhttp，确实挺好用的，而 Spring web 中的 RestTemplate 和这俩的功能类似，也是用来发送 http 请求的，不过用法上面比前面的 2 位要容易很多。<br />Spring 框架提供的 RestTemplate 类可用于在应用中调用 rest 服务，它简化了与 http 服务的通信方式，统一了 RESTful 的标准，封装了 http 链接， 只需要传入 url 及返回值类型即可。相较于之前常用的 HttpClient，RestTemplate 是一种更优雅的调用 RESTful 服务的方式。<br />在 Spring 应用程序中访问第三方 REST 服务与使用 Spring RestTemplate 类有关。RestTemplate 类的设计原则与许多其他 Spring 模板类(例如 `JdbcTemplate`、`JmsTemplate`)相同，为执行复杂任务提供了一种具有默认行为的简化方法。<br />RestTemplate 默认依赖 JDK 提供 http 连接的能力（`HttpURLConnection`），如果有需要的话也可以通过 `setRequestFactory` 方法替换为例如 Apache HttpComponents、Netty 或 OkHttp 等其它 HTTP library。<br />考虑到 RestTemplate 类是为调用 REST 服务而设计的，因此它的主要方法与 REST 的基础紧密相连就不足为奇了，后者是 HTTP 协议的方法：HEAD、GET、POST、PUT、DELETE 和 OPTIONS。例如，RestTemplate 类具有 `headForHeaders()`、`getForObject()`、`postForObject()`、`put()`和 `delete()`等方法。<br />下面给大家上案例，案例是重点，通过案例，把常用的用法都给盘出来。
<a name="fSVNG"></a>
## 2、案例代码
[https://gitee.com/javacode2018/springmvc-series](https://gitee.com/javacode2018/springmvc-series)
<a name="s5izg"></a>
## 3、Get 请求
<a name="iTm6q"></a>
### 3.1、普通请求
接口代码
```java
@GetMapping("/test/get")
@ResponseBody
public BookDto get() {
    return new BookDto(1, "SpringMVC系列");
}
```
使用 `RestTemplate` 调用上面这个接口，通常有 2 种写法，如下
```java
@Test
public void test1() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/get";
    //getForObject方法，获取响应体，将其转换为第二个参数指定的类型
    BookDto bookDto = restTemplate.getForObject(url, BookDto.class);
    System.out.println(bookDto);
}

@Test
public void test2() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/get";
    //getForEntity方法，返回值为ResponseEntity类型
    // ResponseEntity中包含了响应结果中的所有信息，比如头、状态、body
    ResponseEntity<BookDto> responseEntity = restTemplate.getForEntity(url, BookDto.class);
    //状态码
    System.out.println(responseEntity.getStatusCode());
    //获取头
    System.out.println("头：" + responseEntity.getHeaders());
    //获取body
    BookDto bookDto = responseEntity.getBody();
    System.out.println(bookDto);
}
```
test1 输出
```
BookDto{id=1, name='SpringMVC系列'}
```
test2 输出
```
200 OK
头：[Content-Type:"application/json;charset=UTF-8", Transfer-Encoding:"chunked", Date:"Sat, 02 Oct 2021 07:05:15 GMT", Keep-Alive:"timeout=20", Connection:"keep-alive"]
BookDto{id=1, name='SpringMVC系列'}
```
<a name="wzilW"></a>
### 3.2、url 中含有动态参数
接口代码
```java
@GetMapping("/test/get/{id}/{name}")
@ResponseBody
public BookDto get(@PathVariable("id") Integer id, @PathVariable("name") String name) {
    return new BookDto(id, name);
}
```
使用 `RestTemplate` 调用上面这个接口，通常有 2 种写法，如下
```java
@Test
public void test3() {
    RestTemplate restTemplate = new RestTemplate();
    //url中有动态参数
    String url = "http://localhost:8080/chat16/test/get/{id}/{name}";
    Map<String, String> uriVariables = new HashMap<>();
    uriVariables.put("id", "1");
    uriVariables.put("name", "SpringMVC系列");
    //使用getForObject或者getForEntity方法
    BookDto bookDto = restTemplate.getForObject(url, BookDto.class, uriVariables);
    System.out.println(bookDto);
}

@Test
public void test4() {
    RestTemplate restTemplate = new RestTemplate();
    //url中有动态参数
    String url = "http://localhost:8080/chat16/test/get/{id}/{name}";
    Map<String, String> uriVariables = new HashMap<>();
    uriVariables.put("id", "1");
    uriVariables.put("name", "SpringMVC系列");
    //getForEntity方法
    ResponseEntity<BookDto> responseEntity = restTemplate.getForEntity(url, BookDto.class, uriVariables);
    BookDto bookDto = responseEntity.getBody();
    System.out.println(bookDto);
}
```
test3 输出
```
BookDto{id=1, name='SpringMVC系列'}
```
test4 输出
```
BookDto{id=1, name='SpringMVC系列'}
```
<a name="bupNR"></a>
### 3.3、接口返回值为泛型
接口代码
```java
@GetMapping("/test/getList")
@ResponseBody
public List<BookDto> getList() {
    return Arrays.asList(
        new BookDto(1, "Spring高手系列"),
        new BookDto(2, "SpringMVC系列")
    );
}
```
当接口的返回值为泛型的时候，这种情况比较特殊，使用 RestTemplate 调用上面这个接口，代码如下，需要用到`restTemplate.exchange`的方法，这个方法中有个参数是`ParameterizedTypeReference`类型，通过这个参数类指定泛型类型
```java
@Test
public void test5() {
    RestTemplate restTemplate = new RestTemplate();
    //返回值为泛型
    String url = "http://localhost:8080/chat16/test/getList";
    //若返回结果是泛型类型的，需要使用到exchange方法，
    //这个方法中有个参数是ParameterizedTypeReference类型，通过这个参数类指定泛型类型
    ResponseEntity<List<BookDto>> responseEntity =
            restTemplate.exchange(url,
                    HttpMethod.GET,
                    null,
                    new ParameterizedTypeReference<List<BookDto>>() {
                    });
    List<BookDto> bookDtoList = responseEntity.getBody();
    System.out.println(bookDtoList);
}
```
输出
```
[BookDto{id=1, name='Spring高手系列'}, BookDto{id=2, name='SpringMVC系列'}]
```
<a name="v3mre"></a>
### 3.4、下载小文件
接口代码如下，这个接口会下载服务器端的 1.txt 文件。
```java
/**
 * 下载文件
 *
 * @return
 */
@GetMapping("/test/downFile")
@ResponseBody
public HttpEntity<InputStreamResource> downFile() {
    //将文件流封装为InputStreamResource对象
    InputStream inputStream = this.getClass().getResourceAsStream("/1.txt");
    InputStreamResource inputStreamResource = new InputStreamResource(inputStream);
    //设置header
    MultiValueMap<String, String> headers = new HttpHeaders();
    headers.add(HttpHeaders.CONTENT_DISPOSITION, "attachment;filename=1.txt");
    HttpEntity<InputStreamResource> httpEntity = new HttpEntity<>(inputStreamResource);
    return httpEntity;
}
```
使用 `RestTemplate` 调用这个接口，代码如下，目前这个文件的内容比较少，可以直接得到一个数组。
```java
@Test
public void test6() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/downFile";
    //文件比较小的情况，直接返回字节数组
    ResponseEntity<byte[]> responseEntity = restTemplate.getForEntity(url, byte[].class);
    //获取文件的内容
    byte[] body = responseEntity.getBody();
    String content = new String(body);
    System.out.println(content);
}
```
注意：如果文件大的时候，这种方式就有问题了，会导致 oom，要用下面的方式了。
<a name="mDwpp"></a>
### 3.5、下载大文件
接口代码，继续使用上面下载 1.txt 的代码
```java
/**
 * 下载文件
 *
 * @return
 */
@GetMapping("/test/downFile")
@ResponseBody
public HttpEntity<InputStreamResource> downFile() {
    //将文件流封装为InputStreamResource对象
    InputStream inputStream = this.getClass().getResourceAsStream("/1.txt");
    InputStreamResource inputStreamResource = new InputStreamResource(inputStream);
    //设置header
    MultiValueMap<String, String> headers = new HttpHeaders();
    headers.add(HttpHeaders.CONTENT_DISPOSITION, "attachment;filename=1.txt");
    HttpEntity<InputStreamResource> httpEntity = new HttpEntity<>(inputStreamResource);
    return httpEntity;
}
```
此时使用 RestTemplate 调用这个接口，代码如下<br />文件比较大的时候，比如好几个 G，就不能返回字节数组了，会把内存撑爆，导致 OOM，需要使用 `execute` 方法了，这个方法中有个 `ResponseExtractor` 类型的参数，restTemplate 拿到结果之后，会回调{@link ResponseExtractor#extractData}这个方法，在这个方法中可以拿到响应流，然后进行处理，这个过程就是变读边处理，不会导致内存溢出
```java
@Test
public void test7() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/downFile";
    /**
     * 文件比较大的时候，比如好几个G，就不能返回字节数组了，会把内存撑爆，导致OOM
     * 需要这么玩：
     * 需要使用execute方法了，这个方法中有个ResponseExtractor类型的参数，
     * restTemplate拿到结果之后，会回调{@link ResponseExtractor#extractData}这个方法，
     * 在这个方法中可以拿到响应流，然后进行处理，这个过程就是变读边处理，不会导致内存溢出
     */
    String result = restTemplate.execute(url,
            HttpMethod.GET,
            null,
            new ResponseExtractor<String>() {
                @Override
                public String extractData(ClientHttpResponse response) throws IOException {
                    System.out.println("状态："+response.getStatusCode());
                    System.out.println("头："+response.getHeaders());
                    //获取响应体流
                    InputStream body = response.getBody();
                    //处理响应体流
                    String content = IOUtils.toString(body, "UTF-8");
                    return content;
                }
            }, new HashMap<>());

    System.out.println(result);
}
```
<a name="vhZB8"></a>
### 3.6、传递头
接口代码
```java
@GetMapping("/test/header")
@ResponseBody
public Map<String, List<String>> header(HttpServletRequest request) {
    Map<String, List<String>> header = new LinkedHashMap<>();
    Enumeration<String> headerNames = request.getHeaderNames();
    while (headerNames.hasMoreElements()) {
        String name = headerNames.nextElement();
        Enumeration<String> values = request.getHeaders(name);
        List<String> list = new ArrayList<>();
        while (values.hasMoreElements()) {
            list.add(values.nextElement());
        }
        header.put(name, list);
    }
    return header;
}
```
使用 RestTemplate 调用接口，请求头中传递数据，代码如下，注意代码①和②，这两处是关键，用到了`HttpHeaders`和`RequestEntity`

- 请求头放在 `HttpHeaders` 对象中
- `RequestEntity`：请求实体，请求的所有信息都可以放在 `RequestEntity` 中，比如 body 部分、头、请求方式、url 等信息
```java
@Test
public void test8() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/header";
    //①：请求头放在HttpHeaders对象中
    MultiValueMap<String, String> headers = new HttpHeaders();
    headers.add("header-1", "V1");
    headers.add("header-2", "Spring");
    headers.add("header-2", "SpringBoot");
    //②：RequestEntity：请求实体，请求的所有信息都可以放在RequestEntity中，比如body部分、头、请求方式、url等信息
    RequestEntity requestEntity = new RequestEntity(
        null, //body部分数据
        headers, //头
        HttpMethod.GET,//请求方法
        URI.create(url) //地址
    );
    ResponseEntity<Map<String, List<String>>> responseEntity = restTemplate.exchange(requestEntity,
                                                                                     new ParameterizedTypeReference<Map<String, List<String>>>() {
                                                                                     });
    Map<String, List<String>> result = responseEntity.getBody();
    System.out.println(result);
}
```
输出
```
{accept=[application/json, application/*+json], header-1=[V1], header-2=[Spring, SpringBoot], user-agent=[Java/1.8.0_121], host=[localhost:8080], connection=[keep-alive]}
```
<a name="fRBMa"></a>
### 3.7、综合案例：含头、url 动态参数
接口
```java
@GetMapping("/test/getAll/{path1}/{path2}")
@ResponseBody
public Map<String, Object> getAll(@PathVariable("path1") String path1,
                                  @PathVariable("path2") String path2,
                                  HttpServletRequest request) {
    Map<String, Object> result = new LinkedHashMap<>();
    result.put("path1", path1);
    result.put("path2", path2);
    //头
    Map<String, List<String>> header = new LinkedHashMap<>();
    Enumeration<String> headerNames = request.getHeaderNames();
    while (headerNames.hasMoreElements()) {
        String name = headerNames.nextElement();
        Enumeration<String> values = request.getHeaders(name);
        List<String> list = new ArrayList<>();
        while (values.hasMoreElements()) {
            list.add(values.nextElement());
        }
        header.put(name, list);
    }
    result.put("header", header);
    return result;
}
```
如下，使用 `RestTemplate` 调用接口，GET 方式、传递 header、path 中动态参数。
```java
@Test
public void test9() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/getAll/{path1}/{path2}";
    //①：请求头
    MultiValueMap<String, String> headers = new HttpHeaders();
    headers.add("header-1", "V1");
    headers.add("header-2", "Spring");
    headers.add("header-2", "SpringBoot");
    //②：url中的2个参数
    Map<String, String> uriVariables = new HashMap<>();
    uriVariables.put("path1", "v1");
    uriVariables.put("path2", "v2");
    //③：HttpEntity：HTTP实体，内部包含了请求头和请求体
    HttpEntity requestEntity = new HttpEntity(
        null,//body部分，get请求没有body，所以为null
        headers //头
    );
    //④：使用exchange发送请求
    ResponseEntity<Map<String, Object>> responseEntity = restTemplate.exchange(
        url, //url
        HttpMethod.GET, //请求方式
        requestEntity, //请求实体（头、body）
        new ParameterizedTypeReference<Map<String, Object>>() {
        },//返回的结果类型
        uriVariables //url中的占位符对应的值
    );
    Map<String, Object> result = responseEntity.getBody();
    System.out.println(result);
}
```
输出
```
{path1=v1, path2=v2, header={accept=[application/json, application/*+json], header-1=[V1], header-2=[Spring, SpringBoot], user-agent=[Java/1.8.0_121], host=[localhost:8080], connection=[keep-alive]}}
```
<a name="v8hPG"></a>
## 4、POST 请求
<a name="FL3tm"></a>
### 4.1、post 请求常见的 3 种类型
http 请求头中的 Content-Type 用来指定请求的类型，常见的有 3 种

| Content-Type | 说明 |
| --- | --- |
| application/x-www-form-urlencoded | 页面中普通的 form 表单提交时就是这种类型，表单中的元素会按照名称和值拼接好，然后之间用&连接，格式如：p1=v1&p2=v2&p3=v3<br />然后通过 urlencoded 编码之后丢在 body 中发送 |
| multipart/form-data | 页面中表单上传文件的时候，用到的就是这种格式 |
| application/json | 将发送的数据转换为 json 格式，丢在 http 请求的 body 中发送，后端接口通常用@RequestBody 配合对象来接收。 |

下面看则种方式的案例。
<a name="jvgDR"></a>
### 4.2、普通表单请求
普通表单默认为 application/x-www-form-urlencoded 类型的请求。<br />接口代码
```java
@PostMapping("/test/form1")
@ResponseBody
public BookDto form1(BookDto bookDto) {
    return bookDto;
}
```
使用 RestTemplate 调用接口
```java
@Test
public void test10() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form1";
    //①：表单信息，需要放在MultiValueMap中，MultiValueMap相当于Map<String,List<String>>
    MultiValueMap<String, String> body = new LinkedMultiValueMap<>();
    //调用add方法填充表单数据(表单名称:值)
    body.add("id","1");
    body.add("name","SpringMVC系列");
    //②：发送请求(url,请求体，返回值需要转换的类型)
    BookDto result = restTemplate.postForObject(url, body, BookDto.class);
    System.out.println(result);
}
```
如果想携带头信息，代码如下
```java
@Test
public void test11() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form1";
    //①：表单信息，需要放在MultiValueMap中，MultiValueMap相当于Map<String,List<String>>
    MultiValueMap<String, String> body = new LinkedMultiValueMap<>();
    //调用add方法放入表单元素(表单名称:值)
    body.add("id","1");
    body.add("name","SpringMVC系列");
    //②：请求头
    HttpHeaders headers = new HttpHeaders();
    //调用set方法放入请求头
    headers.set(HttpHeaders.CONTENT_TYPE, MediaType.APPLICATION_FORM_URLENCODED_VALUE);
    //③：请求实体：包含了请求体和请求头
    HttpEntity<MultiValueMap<String, String>> httpEntity = new HttpEntity<>(body, headers);
    //④：发送请求(url,请求实体，返回值需要转换的类型)
    BookDto result = restTemplate.postForObject(url, httpEntity, BookDto.class);
    System.out.println(result);
}
```
<a name="O95k7"></a>
### 4.3、上传本地文件
上传文件 Content-Type 为 multipart/form-data 类型。<br />接口如下，上传上传单个文件，返回值为一个 Map 类型，是泛型类型
```java
@PostMapping(value = "/test/form2")
@ResponseBody
public Map<String, String> form2(@RequestParam("file1") MultipartFile file1) {
    Map<String, String> fileMetadata = new LinkedHashMap<>();
    fileMetadata.put("文件名", file1.getOriginalFilename());
    fileMetadata.put("文件类型", file1.getContentType());
    fileMetadata.put("文件大小(byte)", String.valueOf(file1.getSize()));
    return fileMetadata;
}
```
使用 `RestTemplate` 调用接口，主要下面代码②上传的文件需要包装为`org.springframework.core.io.Resource`，常用的有 3 中[`FileSystemResource`、`InputStreamResource`、`ByteArrayResource`]，这里用到的是 `FileSystemResource` 来上传本地文件，另外 2 种（`InputStreamResource`、`ByteArrayResource`）用法就比较特殊了，见下个案例。
```java
@Test
public void test12() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form2";
    //①：表单信息，需要放在MultiValueMap中，MultiValueMap相当于Map<String,List<String>>
    MultiValueMap<String, Object> body = new LinkedMultiValueMap<>();
    //调用add方法放入表单元素(表单名称:值)
    //②：文件对应的类型，需要是org.springframework.core.io.Resource类型的，常见的有[FileSystemResource、InputStreamResource、ByteArrayResource]
    body.add("file1", new FileSystemResource(".\\src\\main\\java\\com\\javacode2018\\springmvc\\chat16\\dto\\UserDto.java"));
    //③：头
    HttpHeaders headers = new HttpHeaders();
    headers.add("header1", "v1");
    headers.add("header2", "v2");
    //④：请求实体
    RequestEntity<MultiValueMap<String, Object>> requestEntity = new RequestEntity<>(body, headers, HttpMethod.POST, URI.create(url));
    //⑤：发送请求(请求实体，返回值需要转换的类型)
    ResponseEntity<Map<String, String>> responseEntity = restTemplate.exchange(
        requestEntity,
        new ParameterizedTypeReference<Map<String, String>>() {
        });
    Map<String, String> result = responseEntity.getBody();
    System.out.println(result);
}
```
<a name="D6Ppl"></a>
### 4.4、通过流或字节数组的方式上传文件
有时候，上传的文件是通过流的方式或者字节数组的方式，那么就需要用到 `InputStreamResource`、`ByteArrayResource` 这俩了。
:::danger
注意：使用这俩的时候，需要重写 2 个方法，否则会上传失败

- `getFilename`：文件名称
- `contentLength`：长度
:::
```java
@Test
public void test13() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form2";
    //①：表单信息，需要放在MultiValueMap中，MultiValueMap相当于Map<String,List<String>>
    MultiValueMap<String, Object> body = new LinkedMultiValueMap<>();
    /**
     * ②：通过流的方式上传文件，流的方式需要用到InputStreamResource类，需要重写2个方法
     * getFilename：文件名称
     * contentLength：长度
     */
    InputStream inputStream = RestTemplateTest.class.getResourceAsStream("/1.txt");
    InputStreamResource inputStreamResource = new InputStreamResource(inputStream) {
        @Override
        public String getFilename() {
            return "1.txt";
        }

        @Override
        public long contentLength() throws IOException {
            return inputStream.available();
        }
    };
    body.add("file1", inputStreamResource);
    //③：头
    HttpHeaders headers = new HttpHeaders();
    headers.add("header1", "v1");
    headers.add("header2", "v2");
    //④：请求实体
    RequestEntity<MultiValueMap<String, Object>> requestEntity = new RequestEntity<>(body, headers, HttpMethod.POST, URI.create(url));
    //⑤：发送请求(请求实体，返回值需要转换的类型)
    ResponseEntity<Map<String, String>> responseEntity = restTemplate.exchange(
        requestEntity,
        new ParameterizedTypeReference<Map<String, String>>() {
        });
    Map<String, String> result = responseEntity.getBody();
    System.out.println(result);
}
```
<a name="SFq9G"></a>
### 4.5、复杂表单：多个普通元素+多文件上传
接口
```java
/**
 * 复杂的表单：包含了普通元素、多文件
 *
 * @param userDto
 * @return
 */
@PostMapping("/test/form3")
@ResponseBody
public Map<String, String> form3(UserDto userDto) {
    Map<String, String> result = new LinkedHashMap<>();
    result.put("name", userDto.getName());
    result.put("headImg", userDto.getHeadImg().getOriginalFilename());
    result.put("idImgList", Arrays.toString(userDto.getIdImgList().stream().
                                            map(MultipartFile::getOriginalFilename).toArray()));
    return result;
}
```
UserDto：包含了多个元素（姓名、头像、多张证件照），这种可以模拟复杂的表单
```java
public class UserDto {
    //姓名
    private String name;
    //头像
    private MultipartFile headImg;
    //多张证件照
    private List<MultipartFile> idImgList;

    //get set 省略了...
}
```
用 RestTemplate 调用这个接口，代码如下
```java
@Test
public void test14() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form3";
    //①：表单信息，需要放在MultiValueMap中，MultiValueMap相当于Map<String,List<String>>
    MultiValueMap<String, Object> body = new LinkedMultiValueMap<>();
    body.add("name", "路人");
    body.add("headImg", new FileSystemResource(".\\src\\main\\resources\\1.jpg"));
    //来2张证件照，元素名称一样
    body.add("idImgList", new FileSystemResource(".\\src\\main\\resources\\2.jpg"));
    body.add("idImgList", new FileSystemResource(".\\src\\main\\resources\\3.jpg"));
    //③：头
    HttpHeaders headers = new HttpHeaders();
    headers.add("header1", "v1");
    headers.add("header2", "v2");
    //④：请求实体
    RequestEntity<MultiValueMap<String, Object>> requestEntity = new RequestEntity<>(body, headers, HttpMethod.POST, URI.create(url));
    //⑤：发送请求(请求实体，返回值需要转换的类型)
    ResponseEntity<Map<String, String>> responseEntity = restTemplate.exchange(
            requestEntity,
            new ParameterizedTypeReference<Map<String, String>>() {
        });
    Map<String, String> result = responseEntity.getBody();
    System.out.println(result);
}
```
输出
```
{name=路人, headImg=1.jpg, idImgList=[2.jpg, 3.jpg]}
```
<a name="L7kfE"></a>
### 4.6、发送 json 格式数据：传递 Java 对象
接口
```java
/**
 * body中json格式的数据，返回值非泛型
 *
 * @param bookDto
 * @return
 */
@PostMapping("/test/form4")
@ResponseBody
public BookDto form4(@RequestBody BookDto bookDto) {
    return bookDto;
}
```
RestTemplate 调用接口
```java
@Test
public void test15() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form4";
    BookDto body = new BookDto(1, "SpringMVC系列");
    BookDto result = restTemplate.postForObject(url, body, BookDto.class);
    System.out.println(result);
}
```
输出
```
BookDto{id=1, name='SpringMVC系列'}
```
<a name="MRW9I"></a>
### 4.7、发送 json 格式数据：传递 Java 对象，返回值为泛型
接口
```java
/**
 * body中json格式的数据，返回值为泛型
 *
 * @param bookDtoList
 * @return
 */
@PostMapping("/test/form5")
@ResponseBody
public List<BookDto> form5(@RequestBody List<BookDto> bookDtoList) {
    return bookDtoList;
}
```
用 RestTemplate 调用这个接口，代码如下
```java
@Test
public void test16() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form5";
    //①：请求体，发送的时候会被转换为json格式数据
    List<BookDto> body = Arrays.asList(
            new BookDto(1, "SpringMVC系列"),
            new BookDto(2, "MySQL系列"));
    //②：头
    HttpHeaders headers = new HttpHeaders();
    headers.add("header1", "v1");
    headers.add("header2", "v2");
    //③：请求实体
    RequestEntity requestEntity = new RequestEntity(body, headers, HttpMethod.POST, URI.create(url));
    //④：发送请求(请求实体，返回值需要转换的类型)
    ResponseEntity<List<BookDto>> responseEntity = restTemplate.exchange(
            requestEntity,
            new ParameterizedTypeReference<List<BookDto>>() {
            });
    //⑤：获取结果
    List<BookDto> result = responseEntity.getBody();
    System.out.println(result);
}
```
输出
```
[BookDto{id=1, name='SpringMVC系列'}, BookDto{id=2, name='MySQL系列'}]
```
<a name="NEY0K"></a>
### 4.8、发送 json 字符串格式数据
上面 2 个 json 案例 body 都是 Java 对象，RestTemplate 默认自动配上 Content-Type=application/json<br />但是如果 body 的值是 json 格式字符串的时候，调用的时候需要在头中明确指定 Content-Type=application/json，写法如下：
```java
@Test
public void test17() {
    RestTemplate restTemplate = new RestTemplate();
    String url = "http://localhost:8080/chat16/test/form5";
    //①：请求体为一个json格式的字符串
    String body = "[{\"id\":1,\"name\":\"SpringMVC系列\"},{\"id\":2,\"name\":\"MySQL系列\"}]";
    /**
     * ②：若请求体为json字符串的时候，需要在头中设置Content-Type=application/json；
     * 若body是普通的java类的时候，无需指定这个，RestTemplate默认自动配上Content-Type=application/json
     */
    HttpHeaders headers = new HttpHeaders();
    headers.setContentType(MediaType.APPLICATION_JSON);
    //③：请求实体（body，头、请求方式，uri）
    RequestEntity requestEntity = new RequestEntity(body, headers, HttpMethod.POST, URI.create(url));
    //④：发送请求(请求实体，返回值需要转换的类型)
    ResponseEntity<List<BookDto>> responseEntity = restTemplate.exchange(
            requestEntity,
            new ParameterizedTypeReference<List<BookDto>>() {
            });
    //⑤：获取结果
    List<BookDto> result = responseEntity.getBody();
    System.out.println(result);
}
```
输出
```
[BookDto{id=1, name='SpringMVC系列'}, BookDto{id=2, name='MySQL系列'}]
```
<a name="roKpG"></a>
## 5、DELETE、PUT、OPTION 请求
<a name="be5Ok"></a>
### 5.1、DELETE 请求
```java
public void delete(String url, Object... uriVariables);
public void delete(String url, Map<String, ?> uriVariables);
public void delete(URI url);
```
<a name="sPUZB"></a>
### 5.2、PUT 请求
PUT 请求和 POST 请求类似，将类型改为 PUT 就可以了。
<a name="ahPCp"></a>
### 5.3、OPTIONS 请求
OPTIONS 请求用来探测接口支持哪些 http 方法
```java
public Set<HttpMethod> optionsForAllow(String url, Object... uriVariables);
public Set<HttpMethod> optionsForAllow(String url, Map<String, ?> uriVariables);
public Set<HttpMethod> optionsForAllow(URI url);
```
<a name="mwOZt"></a>
## 6、集成 `HttpClient`
RestTemplate 内部默认用的是 jdk 自带的 `HttpURLConnection` 发送请求的，性能上面并不是太突出。<br />可以将其替换为 httpclient 或者 okhttp。<br />先来看下如何替换为 `HttpClient`。<br />引入 Maven 配置
```xml
<dependency>
  <groupId>org.apache.httpcomponents</groupId>
  <artifactId>httpclient</artifactId>
  <version>4.5.7</version>
</dependency>
```
创建 RestTemplate 时指定 HttpClient 配置，代码如下
```java
public HttpClient httpClient() {
    HttpClientBuilder httpClientBuilder = HttpClientBuilder.create();
    try {
        //设置信任ssl访问
        SSLContext sslContext = new SSLContextBuilder().loadTrustMaterial(null, (arg0, arg1) -> true).build();
        httpClientBuilder.setSSLContext(sslContext);
        HostnameVerifier hostnameVerifier = NoopHostnameVerifier.INSTANCE;
        SSLConnectionSocketFactory sslConnectionSocketFactory = new SSLConnectionSocketFactory(sslContext, hostnameVerifier);
        Registry<ConnectionSocketFactory> socketFactoryRegistry = RegistryBuilder.<ConnectionSocketFactory>create()
                // 注册http和https请求
                .register("http", PlainConnectionSocketFactory.getSocketFactory())
                .register("https", sslConnectionSocketFactory).build();

        //使用Httpclient连接池的方式配置(推荐)，同时支持netty，okHttp以及其他http框架
        PoolingHttpClientConnectionManager poolingHttpClientConnectionManager = new PoolingHttpClientConnectionManager(socketFactoryRegistry);
        // 最大连接数
        poolingHttpClientConnectionManager.setMaxTotal(1000);
        // 同路由并发数
        poolingHttpClientConnectionManager.setDefaultMaxPerRoute(100);
        //配置连接池
        httpClientBuilder.setConnectionManager(poolingHttpClientConnectionManager);
        // 重试次数
        httpClientBuilder.setRetryHandler(new DefaultHttpRequestRetryHandler(0, true));
        //设置默认请求头
        List<Header> headers = new ArrayList<>();
        httpClientBuilder.setDefaultHeaders(headers);
        return httpClientBuilder.build();
    } catch (Exception e) {
        throw new RuntimeException(e);
    }
}

public ClientHttpRequestFactory clientHttpRequestFactory() {
    HttpComponentsClientHttpRequestFactory clientHttpRequestFactory = new HttpComponentsClientHttpRequestFactory(httpClient());
    // 连接超时(毫秒)，这里设置10秒
    clientHttpRequestFactory.setConnectTimeout(10 * 1000);
    // 数据读取超时时间(毫秒)，这里设置60秒
    clientHttpRequestFactory.setReadTimeout(60 * 1000);
    // 从连接池获取请求连接的超时时间(毫秒)，不宜过长，必须设置，比如连接不够用时，时间过长将是灾难性的
    clientHttpRequestFactory.setConnectionRequestTimeout(10 * 1000);
    return clientHttpRequestFactory;
}

public RestTemplate restTemplate(){
    //创建RestTemplate的时候，指定ClientHttpRequestFactory
    return new RestTemplate(this.clientHttpRequestFactory());
}

@Test
public void test18() {
    RestTemplate restTemplate = this.restTemplate();
    String url = "http://localhost:8080/chat16/test/get";
    //getForObject方法，获取响应体，将其转换为第二个参数指定的类型
    BookDto bookDto = restTemplate.getForObject(url, BookDto.class);
    System.out.println(bookDto);
}
```
<a name="sGDgx"></a>
## 7、集成 okhttp
引入 Maven 配置
```xml
<dependency>
  <groupId>com.squareup.okhttp3</groupId>
  <artifactId>okhttp</artifactId>
  <version>4.3.1</version>
</dependency>
```
创建 `RestTemplate`
```java
new RestTemplate(new OkHttp3ClientHttpRequestFactory());
```
<a name="nhmVY"></a>
## 8、总结
RestTemplate 使用确实非常容易，建议大家去看一下 RestTemplate 的源码，debug 跟踪一下过程，这样用起来就非常顺手了。
