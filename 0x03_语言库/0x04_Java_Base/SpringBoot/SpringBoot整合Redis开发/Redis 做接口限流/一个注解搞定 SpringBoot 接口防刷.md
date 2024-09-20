JavaSpringBoot<br />技术要点：SpringBoot的基本知识，Redis基本操作
<a name="q82Z9"></a>
### 首先是写一个注解类：
```java
import java.lang.annotation.Retention;
import java.lang.annotation.Target;

import static java.lang.annotation.ElementType.METHOD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

@Retention(RUNTIME)
@Target(METHOD)
public @interface AccessLimit {

	int seconds();
	int maxCount();
	boolean needLogin()default true;
}
```
<a name="Nq82k"></a>
### 接着就是在`Interceptor`拦截器中实现：
```java
import com.alibaba.fastjson.JSON;
import com.example.demo.action.AccessLimit;
import com.example.demo.redis.RedisService;
import com.example.demo.result.CodeMsg;
import com.example.demo.result.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.method.HandlerMethod;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;
 
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.OutputStream;
 
@Component
public class FangshuaInterceptor extends HandlerInterceptorAdapter {
 
    @Autowired
    private RedisService redisService;
 
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
 
        //判断请求是否属于方法的请求
        if(handler instanceof HandlerMethod){
 
            HandlerMethod hm = (HandlerMethod) handler;
 
            //获取方法中的注解,看是否有该注解
            AccessLimit accessLimit = hm.getMethodAnnotation(AccessLimit.class);
            if(accessLimit == null){
                return true;
            }
            int seconds = accessLimit.seconds();
            int maxCount = accessLimit.maxCount();
            boolean login = accessLimit.needLogin();
            String key = request.getRequestURI();
            //如果需要登录
            if(login){
                //获取登录的session进行判断
                //.....
                key+=""+"1";  //这里假设用户是1,项目中是动态获取的userId
            }
 
            //从redis中获取用户访问的次数
            AccessKey ak = AccessKey.withExpire(seconds);
            Integer count = redisService.get(ak,key,Integer.class);
            if(count == null){
                //第一次访问
                redisService.set(ak,key,1);
            }else if(count < maxCount){
                //加1
                redisService.incr(ak,key);
            }else{
                //超出访问次数
                render(response,CodeMsg.ACCESS_LIMIT_REACHED); //这里的CodeMsg是一个返回参数
                return false;
            }
        }
 
        return true;
 
    }
    private void render(HttpServletResponse response, CodeMsg cm)throws Exception {
        response.setContentType("application/json;charset=UTF-8");
        OutputStream out = response.getOutputStream();
        String str  = JSON.toJSONString(Result.error(cm));
        out.write(str.getBytes("UTF-8"));
        out.flush();
        out.close();
    }
}
```
<a name="ytqRV"></a>
### 再把`Interceptor`注册到SpringBoot中
```java
import com.example.demo.ExceptionHander.FangshuaInterceptor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;

@Configuration
public class WebConfig extends WebMvcConfigurerAdapter {
 
    @Autowired
    private FangshuaInterceptor interceptor;
 
 
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        registry.addInterceptor(interceptor);
    }
}
```
<a name="QaRZ3"></a>
### 接着在Controller中加入注解
```java
import com.example.demo.result.Result;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class FangshuaController {

	@AccessLimit(seconds=5, maxCount=5, needLogin=true)
	@RequestMapping("/fangshua")
	@ResponseBody
	public Result<String> fangshua(){
		return Result.success("请求成功");
	}
}
```
