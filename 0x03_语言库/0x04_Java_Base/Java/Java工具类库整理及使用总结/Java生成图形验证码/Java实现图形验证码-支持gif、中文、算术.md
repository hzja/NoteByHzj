Java 图形验证码<br />图形验证码是最经典，也是最常用的验证方式。介绍一个非常不错的类库：Java图形验证码，支持gif、中文、算术等类型，可用于Java Web、JavaSE等项目。<br />官网：[https://gitee.com/whvse/EasyCaptcha](https://gitee.com/whvse/EasyCaptcha)<br />效果图：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1601302042702-4a4ade57-c6fd-4fb0-9e32-029410c57d5a.png#align=left&display=inline&height=391&originHeight=1174&originWidth=1386&size=494560&status=done&style=shadow&width=462)
<a name="d93f6a42"></a>
### 项目引入easy-captcha
```xml
<dependencies>
  <dependency>
    <groupId>com.github.whvcse</groupId>
    <artifactId>easy-captcha</artifactId>
    <version>1.6.2</version>
  </dependency>
</dependencies>
```
<a name="ozn7s"></a>
### SpringBoot项目创建图形验证码
前后端分离项目中建议不要存储在session中；而使用分布式session，存储在redis中，redis存储需要一个key，key一同返回给前端用于验证输入。
```java
@Controller
public class CaptchaController {
    @Autowired
    private RedisUtil redisUtil;
    @ResponseBody
    @RequestMapping("/vcode/captcha")
    public JsonResult captcha(HttpServletRequest request, HttpServletResponse response) throws Exception {
        SpecCaptcha specCaptcha = new SpecCaptcha(130, 48, 5);
        String verCode = specCaptcha.text().toLowerCase();
        String key = UUID.randomUUID().toString();
        // 存入redis并设置过期时间为30分钟
        redisUtil.setEx(key, verCode, 30, TimeUnit.MINUTES);
        // 将key和base64返回给前端
        return JsonResult.ok().put("key", key).put("image", specCaptcha.toBase64());
    }
    @ResponseBody
    @PostMapping("/vcode/vaild")
    public JsonResult login(String username,String password,String verCode,String verKey){
        // 获取redis中的验证码
        String redisCode = redisUtil.get(verKey);
        // 判断验证码
        if (verCode==null || !redisCode.equals(verCode.trim().toLowerCase())) {
            return JsonResult.error("验证码不正确");
        }
    }  
}
```
<a name="59d4a548"></a>
### 前端使用ajax获取验证码并验证
```html
<img id="verImg" width="130px" height="48px"/>
<script>
  var verKey;
  // 获取验证码
  $.get('/vcode/captcha', function(res) {
    verKey = res.key;
    $('#verImg').attr('src', res.image);
  },'json');
  // 登录
  $.post('/vcode/login', {
    verKey: verKey,
    verCode: '8u6h',
    username: 'admin'，
    password: 'admin'
  }, function(res) {
    console.log(res);
  }, 'json');
</script>
```
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1601301843727-a0406ddc-a45f-4822-8dca-1db75525a233.gif#align=left&display=inline&height=111&originHeight=111&originWidth=91&size=0&status=done&style=none&width=91)
