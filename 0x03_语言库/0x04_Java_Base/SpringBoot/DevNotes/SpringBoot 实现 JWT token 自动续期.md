JavaSpringBoot
<a name="iEdV1"></a>
### 1、为什么要 token自动续期
token中一般会包含用户的基本信息，为了保证token的安全性，一般会将token的过期时间设置的比较短，但是这样会导致用户因为token过期需要频繁登录，因此需要token自动续期。
<a name="Tqb6q"></a>
### 2、如何实现token自动续期
登录时将token存入redis中，缓存有效期设置为 token有效时间的两倍（比token有效时间长即可）。
```java
//创建token
String token = JwtUtil.createToken(sysUser.getId(), user.getUserName());
//将token放入redis中,key为用户的手机号+"token"
redisUtil.set(sysUser.getPhone() + GlobalConstant.TOKEN, token, JwtUtil.EXPIRE_TIME*2);
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673605837471-61b23528-4864-4e57-9ec6-41967f2f3367.png#averageHue=%2320202a&clientId=u8f74bcf7-8fc3-4&from=paste&id=u251cb204&originHeight=654&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u617fa243-04ed-4633-93cf-fe306301a9e&title=)<br />在拦截器中重写`public boolean preHandle(HttpServletRequest request,HttpServletResponse response, Object handler)`方法
```java
/**
 * token自动续期
 *
 * @param sysUser 用户实体
 * @return 是否刷新成功
 */
private boolean refreshToken(SysUser sysUser) {
    String token = request.getHeader(GlobalConstant.TOKEN);
    String cacheToken = (String) (redisUtil.get(sysUser.getPhone() + GlobalConstant.TOKEN));
    //请求头中不存在token，返回false
    if (StringUtil.isEmpty(token)) {
        logger.error("请求头中token不存在");
        return false;
    }
    //用户是否登录只根据redis中token是否存在决定，redis中不存在token，返回false
    if (StringUtil.isEmpty(cacheToken)) {
        logger.error("用户未登录");
        return false;
    }
    try {
        //验证请求头中的token是否合法
        JwtUtil.verify(token);
    } catch (TokenExpiredException tokenExpiredException) {
        /*若抛出token过期异常，检查请求头中的token与redis中的token是否相同
        如果相同，说明用户仍在操作，只是请求头中的token已经过期，此时需要对token进行续期*/
        if (cacheToken.equals(token)) {
            //重新刷新redis中的token的过期时间
            redisUtil.set(sysUser.getPhone() + GlobalConstant.TOKEN, token, JwtUtil.EXPIRE_TIME * 2);
            return true;
        } else {
            return false;
        }
    } catch (Exception e) {
        //若抛出除token过期异常之外的其他异常，说明该token不合法
        logger.error("token不合法");
        return false;
    }
    return true;
}
```
![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673605837331-1d41d8ce-a1ff-466e-b899-53115c1bfa4f.png#averageHue=%231f1f29&clientId=u8f74bcf7-8fc3-4&from=paste&id=u627e0507&originHeight=842&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u28f3012d-f7cb-402d-b5a0-fc840e38a4e&title=)<br />拦截器所有代码如下
```java
@Component
public class LoginInterceptor implements HandlerInterceptor {

    private final Logger logger = LoggerFactory.getLogger(getClass());
    @Resource
    private SysUserDao sysUserDao;
    @Resource
    private RedisUtil redisUtil;
    @Resource
    private HttpServletRequest request;

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) {
        logger.info("进入拦截器 uri:" + request.getRequestURI());
        // 不是controller的方法不拦截
        if (!(handler instanceof HandlerMethod)) {
            return true;
        }
        //检查方法上是否有@PreAuth注解，没有则不拦截
        HandlerMethod handlerMethod = (HandlerMethod) handler;
        PreAuth preAuth = handlerMethod.getMethodAnnotation(PreAuth.class);
        if (preAuth == null) {
            return true;
        }
        Long userId = JwtUtil.getUserIdByToken(request);
        SysUser sysUser = sysUserDao.selectById(userId);
        //用户不存在，进行拦截
        if (sysUser == null) {
            logger.error("用户不存在");
            return false;
        }
        if (!refreshToken(sysUser)) {
            return false;
        }
        //判断用户是否有对应权限
        Set<String> authList = this.sysUserDao.queryAuthList(userId);
        if (!authList.contains(preAuth.value())) {
            logger.error("无权限");
            return false;
        }
        return true;
    }

    /**
     * token自动续期
     *
     * @param sysUser 用户实体
     * @return 是否刷新成功
     */
    private boolean refreshToken(SysUser sysUser) {
        String token = request.getHeader(GlobalConstant.TOKEN);
        String cacheToken = (String) (redisUtil.get(sysUser.getPhone() + GlobalConstant.TOKEN));
        //请求头中不存在token，返回false
        if (StringUtil.isEmpty(token)) {
            logger.error("请求头中token不存在");
            return false;
        }
        //用户是否登录只根据redis中token是否存在决定，redis中不存在token，返回false
        if (StringUtil.isEmpty(cacheToken)) {
            logger.error("用户未登录");
            return false;
        }
        try {
            //验证请求头中的token是否合法
            JwtUtil.verify(token);
        } catch (TokenExpiredException tokenExpiredException) {
            /*若抛出token过期异常，检查redis中的是否存在token以及请求头中的token与redis中的token是否相同
            如果相同，说明用户仍在操作，只是请求头中的token已经过期，此时需要对token进行续期*/
            if (cacheToken.equals(token)) {
                //重新刷新redis中的token的过期时间
                redisUtil.set(sysUser.getPhone() + GlobalConstant.TOKEN, token, JwtUtil.EXPIRE_TIME * 60 * 2);
                return true;
            } else {
                return false;
            }
        } catch (Exception e) {
            //若抛出除token过期异常之外的其他异常，说明该token不合法
            logger.error("token不合法");
            return false;
        }
        return true;
    }
}
```
JwtUtil工具类如下
```java
import com.admin.common.constant.GlobalConstant;
import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;

import javax.servlet.http.HttpServletRequest;
import java.util.Calendar;
import java.util.Date;

public class JwtUtil {
    /**
     * token私钥，不可以暴露
     */
    public static final String TOKEN_SECRET_KEY = "tokenSecretKey";
    /**
     * token过期时间（秒）
     */
    public static final int EXPIRE_TIME = 60;

    /**
     * 创建token
     *
     * @param userId   用户ID
     * @param userName 用户名
     * @return token
     */
    public static String createToken(Long userId, String userName) {
        Calendar calendar = Calendar.getInstance();
        calendar.add(Calendar.SECOND, EXPIRE_TIME);
        return JWT.create()
                //签发对象
                .withAudience(userId + "")
                //载荷
                .withClaim("userName", userName)
                //签发时间
                .withIssuedAt(new Date())
                //有效时间
                .withExpiresAt(calendar.getTime())
                //加密
                .sign(Algorithm.HMAC256(TOKEN_SECRET_KEY));
    }

    /**
     * 验证token合法性
     *
     * @param token token
     * @return token是否合法
     */
    public static void verify(String token) {
        JWT.require(Algorithm.HMAC256(TOKEN_SECRET_KEY)).build().verify(token);
    }

  	/**
     * 通过token获取userId
     *
     * @return userId
     */
    public static Long getUserIdByToken(HttpServletRequest request) {
        String token = request.getHeader(GlobalConstant.TOKEN);
        String userId = JWT.decode(token).getAudience().get(0);
        return Long.valueOf(userId);
    }
}
```
