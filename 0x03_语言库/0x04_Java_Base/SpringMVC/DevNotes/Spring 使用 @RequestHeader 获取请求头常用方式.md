JavaSpring<br />Spring 提供了`@RequestHeader` 注解用来获取请求头。
<a name="euJsa"></a>
## 1、获取单个请求头
例如，获取 user-id 请求头
```java
@RequestMapping("/getSingleHeader")
public Map<String, Object> getSingleHeader(@RequestHeader("user-id") String userId){
	Map<String, Object> result = new HashMap<>();
	result.put("code", 0);
	result.put("msg", "success");
	result.put("userId", userId);
	return result;
}
```
测试
```bash
curl -X POST \
  http://127.0.0.1:8080/getSingleHeader \
  -H 'user-id: A10010'
```
<a name="jehI7"></a>
## 2、一次性获取所有请求头
<a name="Y0jl3"></a>
### 方式一：使用 Map 接收所有请求头
```java
@RequestMapping("/getAllHeaders")
public Map<String, Object> listAllHeaders(@RequestHeader Map<String, String> headers) {
	Map<String, Object> result = new HashMap<>();
	headers.forEach((key, value) -> {
		// 日志中输出所有请求头
		System.out.println(String.format("Header '%s' = %s", key, value));
	});
	result.put("code", 0);
	result.put("msg", "success");
	result.put("headers", headers);
	return result;
}
```
测试
```bash
curl -X POST \
  http://127.0.0.1:8080/getAllHeaders \
  -H 'company-id: 1001' \
  -H 'user-id: A10010'
```
日志打印
```
Header 'host' = 127.0.0.1:8080
Header 'connection' = keep-alive
Header 'content-length' = 0
Header 'user-id' = A10010
Header 'company-id' = 1001
```
<a name="qAZDw"></a>
### 方式二：使用 MultiValueMap 接收所有请求头
```java
@RequestMapping("/listHeaders")
public Map<String, Object> listHeaders(@RequestHeader MultiValueMap<String, String> headers) {
	Map<String, Object> result = new HashMap<>();
	headers.forEach((key, value) -> {
		// 日志中输出所有请求头
		System.out.println(String.format("Header '%s' = %s", key, value));
	});
	result.put("code", 0);
	result.put("msg", "success");
	result.put("headers", headers);
	return result;
}
```
测试
```bash
curl -X POST \
  http://127.0.0.1:8080/listHeaders \
  -H 'company-id: 1001' \
  -H 'user-id: A10010'
```
日志打印
```
Header 'host' = [127.0.0.1:8080]
Header 'connection' = [keep-alive]
Header 'content-length' = [0]
Header 'user-id' = [A10010]
Header 'company-id' = [1001]
```
可以发现如果一个请求头存在多个值的时，可以使用 MultiValueMap 接收所有请求头。查看 MultiValueMap 源码，发现 MultiValueMap 实现了 `Map<K, List<V>>`
<a name="LEgfP"></a>
### 方式三：使用 HttpHeaders 接收所用请求头
```java
@RequestMapping("/getAllHttpHeaders")
public Map<String, Object> getAllHttpHeaders(@RequestHeader HttpHeaders headers) {
	headers.forEach((key, value) -> {
		// 日志中输出所有请求头
		System.out.println(String.format("getAllHttpHeaders '%s' = %s", key, value));
	});
	Map<String, Object> result = new HashMap<>();
	result.put("code", 0);
	result.put("msg", "success");
	result.put("headers", headers);
	return result;
}
```
测试
```bash
curl -X POST \
  http://127.0.0.1:8080/getAllHttpHeaders \
  -H 'company-id: 1001' \
  -H 'user-id: A10010'
```
日志打印
```
getAllHttpHeaders 'host' = [127.0.0.1:8080]
getAllHttpHeaders 'connection' = [keep-alive]
getAllHttpHeaders 'content-length' = [0]
getAllHttpHeaders 'user-id' = [A10010]
getAllHttpHeaders 'company-id' = [1001]
```
查看 HttpHeaders 源码发现 `MultiValueMap<String, String>`<br />如果在项目中需要接收多个请求，使用 MultiValueMap 和 HttpHeaders 的选择，可以节省参数个数，使得代码整洁；当然，如果就接受一两个请求头，直接使用 RequestHeader  注解就可以。
