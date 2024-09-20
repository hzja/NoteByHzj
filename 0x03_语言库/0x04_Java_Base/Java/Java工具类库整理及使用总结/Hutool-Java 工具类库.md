Hutool 是一个 Java 工具包，也只是一个工具包，它简化每一行代码，减少每一个方法。<br />官网地址：[http://www.hutool.cn/](http://www.hutool.cn/)
<a name="0iQYF"></a>
## 功能
一个 Java 基础工具类，对文件、流、加密解密、转码、正则、线程、XML 等 JDK 方法进行封装，组成各种 Util 工具类，同时提供以下组件：

- hutool-aop JDK 动态代理封装，提供非 IOC 下的切面支持
- hutool-bloomFilter 布隆过滤，提供一些 Hash 算法的布隆过滤
- hutool-cache 缓存
- hutool-core 核心，包括 Bean 操作、日期、各种 Util 等
- hutool-cron 定时任务模块，提供类 Crontab 表达式的定时任务
- hutool-crypto 加密解密模块
- hutool-db JDBC 封装后的数据操作，基于 ActiveRecord 思想
- hutool-dfa 基于 DFA 模型的多关键字查找
- hutool-extra 扩展模块，对第三方封装（模板引擎、邮件等）
- hutool-http 基于 HttpUrlConnection 的 Http 客户端封装
- hutool-log 自动识别日志实现的日志门面
- hutool-script 脚本执行封装，例如 Javascript
- hutool-setting 功能更强大的 Setting 配置文件和 Properties 封装
- hutool-system 系统参数调用封装（JVM 信息等）
- hutool-json JSON 实现
- hutool-captcha 图片验证码实现
<a name="yb7xR"></a>
## 安装
Maven项目在pom.xml添加以下依赖即可：
```xml
<dependency>
  <groupId>cn.hutool</groupId>
  <artifactId>hutool-all</artifactId>
  <version>4.6.3</version>
</dependency>
```
<a name="8xMij"></a>
## 工具类使用
<a name="mkXtl"></a>
### DateUtil（日期时间工具类）
日期时间工具类，定义了一些常用的日期时间操作方法。
```java
//Date、long、Calendar之间的相互转换
//当前时间
Date date = DateUtil.date();
//Calendar转Date
date = DateUtil.date(Calendar.getInstance());
//时间戳转Date
date = DateUtil.date(System.currentTimeMillis());
//自动识别格式转换
String dateStr = "2017-03-01";
date = DateUtil.parse(dateStr);
//自定义格式化转换
date = DateUtil.parse(dateStr, "yyyy-MM-dd");
//格式化输出日期
String format = DateUtil.format(date, "yyyy-MM-dd");
//获得年的部分
int year = DateUtil.year(date);
//获得月份，从0开始计数
int month = DateUtil.month(date);
//获取某天的开始、结束时间
Date beginOfDay = DateUtil.beginOfDay(date);
Date endOfDay = DateUtil.endOfDay(date);
//计算偏移后的日期时间
Date newDate = DateUtil.offset(date, DateField.DAY_OF_MONTH, 2);
// 日期加减
Date date = DateUtil.date(); // 2023-01-01 10:30:00
Date nextWeek = DateUtil.offsetWeek(date, 1);
System.out.println(nextWeek); // 输出：2023-01-08 10:30:00
//计算日期时间之间的偏移量
long betweenDay = DateUtil.between(date, newDate, DateUnit.DAY);
```
可以方便地进行日期时间的转换、格式化、计算、比较等操作。
```java
// 获取当前日期时间
DateUtil.date(); // 2021-09-15 16:36:31
// 日期时间格式化
DateUtil.format(new Date(), "yyyy-MM-dd HH:mm:ss"); // "2021-09-15 16:36:31"
// 日期时间计算
DateUtil.offset(new Date(), DateField.MONTH, 1); // 2021-10-15 16:36:31
// 日期时间转换
DateUtil.parse("2021-09-15", "yyyy-MM-dd"); // 2021-09-15 00:00:00
// 日期时间比较
DateUtil.compare(new Date(), DateUtil.parse("2022-01-01", "yyyy-MM-dd")); // -1
// 日期比较
Date date1 = DateUtil.parse("2023-01-01");
Date date2 = DateUtil.parse("2023-02-01");
boolean isBefore = DateUtil.isBefore(date1, date2);
System.out.println(isBefore); // 输出：true
```
<a name="X3t95"></a>
### StrUtil（字符串工具类）
字符串工具类，定义了一些常用的字符串操作方法。
```java
//判断是否为空字符串
String str = "test";
StrUtil.isEmpty(str);
StrUtil.isNotEmpty(str);
//去除字符串的前后缀
StrUtil.removeSuffix("a.jpg", ".jpg");
StrUtil.removePrefix("a.jpg", "a.");
//格式化字符串
String template = "这只是个占位符:{}";
String str2 = StrUtil.format(template, "我是占位符");
LOGGER.info("/strUtil format:{}", str2);
```
可以快速进行字符串的判断、转换、拼接、替换、截取等操作。
```java
// 判断字符串是否为空
StrUtil.isBlank(""); // true
// 字符串转数字
NumberUtil.parseInt("123"); // 123
// 去除字符串空格
StrUtil.trim(" abc "); // "abc"
// 字符串拼接
StrUtil.join(",", "a", "b", "c"); // "a,b,c"
// 字符串替换
StrUtil.replace("hello, world", "world", "Hutool"); // "hello, Hutool"
// 字符串截取
StrUtil.sub("hello, world", 0, 5); // "hello"
```
<a name="LCDZv"></a>
### NumberUtil（数字处理工具类）
数字处理工具类，可用于各种类型数字的加减乘除操作及判断类型。
```java
double n1 = 1.234;
double n2 = 1.234;
double result;
//对float、double、BigDecimal做加减乘除操作
result = NumberUtil.add(n1, n2);
result = NumberUtil.sub(n1, n2);
result = NumberUtil.mul(n1, n2);
result = NumberUtil.div(n1, n2);
//保留两位小数
BigDecimal roundNum = NumberUtil.round(n1, 2);
String n3 = "1.234";
//判断是否为数字、整数、浮点数
NumberUtil.isNumber(n3);
NumberUtil.isInteger(n3);
NumberUtil.isDouble(n3);
```
<a name="DX8VC"></a>
### MapUtil（Map操作工具类）
Map操作工具类，可用于创建Map对象及判断Map是否为空。
```java
//将多个键值对加入到Map中
Map<Object, Object> map = MapUtil.of(new String[][]{
    {"key1", "value1"},
    {"key2", "value2"},
    {"key3", "value3"}
});
//判断Map是否为空
MapUtil.isEmpty(map);
MapUtil.isNotEmpty(map);
AnnotationUtil
注解工具类，可用于获取注解与注解中指定的值。

//获取指定类、方法、字段、构造器上的注解列表
Annotation[] annotationList = AnnotationUtil.getAnnotations(HutoolController.class, false);
LOGGER.info("annotationUtil annotations:{}", annotationList);
//获取指定类型注解
Api api = AnnotationUtil.getAnnotation(HutoolController.class, Api.class);
LOGGER.info("annotationUtil api value:{}", api.description());
//获取指定类型注解的值
Object annotationValue = AnnotationUtil.getAnnotationValue(HutoolController.class, RequestMapping.class);
```
<a name="ugYg5"></a>
### SecureUtil（加密解密工具）
主要是在登录的时候还有修改密码的时候用到的，因为数据库里面的密码是 md5 加密处理的，所以登录的时候需要先加密之后再到数据库进行查询，使用 Hutool 的话，只需要调用 SecureUtil 中的 md5 方法就可以了。
```java
user = userService.userLoginByName(loginName,SecureUtil.md5(loginPwd));
```
可以进行MD5、SHA、AES、DES等加密解密操作，保护数据的安全性。
```java
//5加密
SecureUtil.md5("hello"): // 5d41402abc4b2a76b9719d911017c592
// SHA1加密
SecureUtil.sha1("hello"): // aaf4c61ddcc5e8a2dabede0f3b482ed9aea9434d
AES加密解密
String key ="1234567890abedef"
String str = "hello"
String encryptStr = AesUtil.encrypt(str, key): // 26d4a4a3b6d771c33b9a4d66f93c2ec
String decryptStr = AesUtil.decrypt(encryptStr, key); // hello
// DES加密解密
String key = "12345678"
String str = "hello"
String encryptStr = DesUtil.encrypt(str, key); // GtIvzFpQ2tI=
String decryptStr = DesUtil.decrypt(encryptStr, key): // hello
```
<a name="fybu3"></a>
### HtmlUtil（HTML 工具类）
`HtmlUtil.encode`，可以将一些字符转化为安全字符，防止 xss 注入和 SQL 注入，比如下面的评论提交。
```java
comment.setCommentAuthor(HtmlUtil.encode(comment.getCommentAuthor()));
```
这就是防止有黑客写一些可执行的 js 代码，然后提交评论，在后台面板就会执行这一段代码，比较危险，使用 encode 方法就可以将 标签给转化成，这样转化之后，js 代码就不会执行了。<br />另外，HtmlUtil 还提供了以下方法，有兴趣的可以去试一下。

- `HtmlUtil.restoreEscaped` 还原被转义的 HTML 特殊字符
- `HtmlUtil.encode` 转义文本中的 HTML 字符为安全的字符
- `HtmlUtil.cleanHtmlTag` 清除所有 HTML 标签
- `HtmlUtil.removeHtmlTag` 清除指定 HTML 标签和被标签包围的内容
- `HtmlUtil.unwrapHtmlTag` 清除指定 HTML 标签，不包括内容
- `HtmlUtil.removeHtmlAttr` 去除 HTML 标签中的属性
- `HtmlUtil.removeAllHtmlAttr` 去除指定标签的所有属性
- `HtmlUtil.filter` 过滤 HTML 文本，防止 XSS 攻击
<a name="Qdpyn"></a>
### CronUtil（定时任务）
这个工具不需要类似 quartz 这样的框架来做定时任务，而且 CronUtil 也不需要任何其他依赖，只需要在 resources 下建一个配置文件，然后在程序启动的时候将定时任务开启就行了，如 Halo 的定时备份功能（每天凌晨 1 点备份一次）。<br />cron.setting
```java
cc.ryanc.halo.web.controller.admin.BackupController.backupResources = 0 0 1 * * ?
cc.ryanc.halo.web.controller.admin.BackupController.backupDatabase = 0 0 1 * * ?
cc.ryanc.halo.web.controller.admin.BackupController.backupPosts = 0 0 1 * * ?
@Override
public void onApplicationEvent(ContextRefreshedEvent event){
 this.loadActiveTheme();
 this.loadOptions();
 this.loadFiles();
 this.loadThemes();
 //启动定时任务
 CronUtil.start();
 log.info("定时任务启动成功！");
}
```
<a name="HJHJL"></a>
### CaptchaUtil（验证码工具类）
验证码工具类，可用于生成图形验证码。
```java
//生成验证码图片
LineCaptcha lineCaptcha = CaptchaUtil.createLineCaptcha(200, 100);
try {
    request.getSession().setAttribute("CAPTCHA_KEY", lineCaptcha.getCode());
    response.setContentType("image/png");//告诉浏览器输出内容为图片
    response.setHeader("Pragma", "No-cache");//禁止浏览器缓存
    response.setHeader("Cache-Control", "no-cache");
    response.setDateHeader("Expire", 0);
    lineCaptcha.write(response.getOutputStream());
} catch (IOException e) {
    e.printStackTrace();
}
```
<a name="Bo41t"></a>
### UrlUtil工具类
UrlUtil工具类提供了对URL的处理方法，包括URL编码、解码、拼接等。
```java
import cn.hutool.core.util.UrlUtil;

public class UrlUtilExample {
    public static void main(String[] args) {
        // URL编码
        String url = "https://www.example.com/search?keyword=Java";
        String encodedUrl = UrlUtil.encode(url);
        System.out.println(encodedUrl); // 输出：https%3A%2F%2Fwww.example.com%2Fsearch%3Fkeyword%3DJava

        // URL解码
        String encodedUrl = "https%3A%2F%2Fwww.example.com%2Fsearch%3Fkeyword%3DJava";
        String decodedUrl = UrlUtil.decode(encodedUrl);
        System.out.println(decodedUrl); // 输出：https://www.example.com/search?keyword=Java

        // 拼接URL参数
        String baseUrl = "https://www.example.com/search";
        String keyword = "Java";
        String param = "page=1";
        String fullUrl = UrlUtil.url(baseUrl).param("keyword", keyword).param(param).build();
        System.out.println(fullUrl); // 输出：https://www.example.com/search?keyword=Java&page=1

        // 获取URL中的域名
        String url = "https://www.example.com/search?keyword=Java";
        String domain = UrlUtil.getDomain(url);
        System.out.println(domain); // 输出：www.example.com
    }
}
```
<a name="cN6j5"></a>
### 文件IO操作：可以进行文件的读取、写入、复制、移动、删除等操作。
```java
// 文件读取
String str = FileUtil.readUtf8String(new File("test.txt")); // "hello, world"
// 文件写入
FileUtil.writeUtf8String("hello, world", new File("test.txt"));
// 文件复制
FileUtil.copy(new File("src.txt"), new File("dest.txt"), true);
// 文件移动
FileUtil.move(new File("src.txt"), new File("dest.txt"), true);
// 文件删除
FileUtil.del(new File("test.txt"));
```
<a name="UzZoB"></a>
### 网络操作：可以进行HTTP请求、FTP文件上传下载、邮件发送等网络相关操作。
```java
// 发送HTTP GET请求
String result = HttpUtil.get("http://www.baidu.com");
// 发送HTTP POST请求
Map<String, Object> paramMap = new HashMap<>();
paramMap.put("username", "admin");
paramMap.put("password", "123456");
String result = HttpUtil.post("http://www.example.com/login", paramMap);
// 上传文件到FTP服务器
String host = "ftp.example.com";
int port = 21;
String username = "username";
String password = "password";
String path = "/upload";
String fileName = "test.txt";
File file = new File("test.txt");
FtpUtil.upload(host, port, username, password, path, fileName, file);
// 发送邮件
String to = "example@example.com";
String subject = "测试邮件";
String content = "这是一封测试邮件";
MailUtil.send(to, subject, content, false);
```
<a name="b1uGi"></a>
### BeanUtil（Bean操作）
JavaBean的工具类，可用于Map与JavaBean对象的互相转换以及对象属性的拷贝。
```java
PmsBrand brand = new PmsBrand();
brand.setId(1L);
brand.setName("小米");
brand.setShowStatus(0);
//Bean转Map
Map<String, Object> map = BeanUtil.beanToMap(brand);
LOGGER.info("beanUtil bean to map:{}", map);
//Map转Bean
PmsBrand mapBrand = BeanUtil.mapToBean(map, PmsBrand.class, false);
LOGGER.info("beanUtil map to bean:{}", mapBrand);
//Bean属性拷贝
PmsBrand copyBrand = new PmsBrand();
BeanUtil.copyProperties(brand, copyBrand);
LOGGER.info("beanUtil copy properties:{}", copyBrand);
```
可以快速对JavaBean进行赋值、拷贝、转换等操作。
```java
// 对象属性拷贝
User user1 = new User("Tom", 18);
User user2 = new User();
BeanUtil.copyProperties(user1, user2);
// 获取对象的属性值
User user = new User("Tom", 18);
String name = BeanUtil.getProperty(user, "name"); // "Tom"
// 设置对象的属性值
User user = new User();
BeanUtil.setProperty(user, "name", "Tom");
// 对象序列化
User user = new User("Tom", 18);
byte[] bytes = SerializeUtil.serialize(user);
// 对象反序列化
User user = SerializeUtil.deserialize(bytes);
```
<a name="c73vo"></a>
### 集合操作：可以进行集合的排序、过滤、查找、转换等操作。
```java
// 集合排序
List<Integer> list = Arrays.asList(3, 1, 5, 2, 4);
CollUtil.sort(list); // [1, 2, 3, 4, 5]
// 集合过滤
List<Integer> list = Arrays.asList(3, 1, 5, 2, 4);
List<Integer> filteredList = CollUtil.filter(list, (Filter<Integer>) i -> i % 2 == 0); // [2, 4]
// 集合查找
List<Integer> list = Arrays.asList(3, 1, 5, 2, 4);
int index = CollUtil.indexOf(list, 5); // 2
// 集合转换
List<Integer> list = Arrays.asList(1, 2, 3);
String str = CollUtil.join(list, ","); // "1,2,3"
```
<a name="r5QKe"></a>
### 其他常用操作：包括随机数生成、身份证号码验证、二维码生成等等。
```java
// 生成随机数
int random = RandomUtil.randomInt(1, 10);
// 验证身份证号码
boolean isValid = IdcardUtil.isValidCard("11010119900307607X");
// 生成二维码
QrCodeUtil.generate("http://www.example.com", 300, 300, new File("qrcode.png"));
```
