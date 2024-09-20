JavaSpringBoot
<a name="Ow3yL"></a>
## 一、什么是幂等性
幂等是一个数学与计算机学概念，在数学中某一元运算为幂等时，其作用在任一元素两次后会和其作用一次的结果相同。<br />在计算机中编程中，一个幂等操作的特点是其任意多次执行所产生的影响均与一次执行的影响相同。幂等函数或幂等方法是指可以使用相同参数重复执行，并能获得相同结果的函数。这些函数不会影响系统状态，也不用担心重复执行会对系统造成改变。
<a name="cgXj1"></a>
## 二、什么是接口幂等性
在HTTP/1.1中，对幂等性进行了定义。它描述了一次和多次请求某一个资源对于资源本身应该具有同样的结果（网络超时等问题除外），即第一次请求的时候对资源产生了副作用，但是以后的多次请求都不会再对资源产生副作用。<br />这里的副作用是不会对结果产生破坏或者产生不可预料的结果。也就是说，其任意多次执行对资源本身所产生的影响均与一次执行的影响相同。
<a name="uPnRH"></a>
## 三、为什么需要实现幂等性
在接口调用时一般情况下都能正常返回信息不会重复提交，不过在遇见以下情况时可以就会出现问题，如：

- **前端重复提交表单：** 在填写一些表格时候，用户填写完成提交，很多时候会因网络波动没有及时对用户做出提交成功响应，致使用户认为没有成功提交，然后一直点提交按钮，这时就会发生重复提交表单请求。
- **用户恶意进行刷单：** 例如在实现用户投票这种功能时，如果用户针对一个用户进行重复提交投票，这样会导致接口接收到用户重复提交的投票信息，这样会使投票结果与事实严重不符。
- **接口超时重复提交：** 很多时候 HTTP 客户端工具都默认开启超时重试的机制，尤其是第三方调用接口时候，为了防止网络波动超时等造成的请求失败，都会添加重试机制，导致一个请求提交多次。
- **消息进行重复消费：** 当使用 MQ 消息中间件时候，如果发生消息中间件出现错误未及时提交消费信息，导致发生重复消费。

使用幂等性最大的优势在于使接口保证任何幂等性操作，免去因重试等造成系统产生的未知的问题。
<a name="xBb1x"></a>
## 四、引入幂等性后对系统的影响
幂等性是为了简化客户端逻辑处理，能放置重复提交等操作，但却增加了服务端的逻辑复杂性和成本，其主要是：

- 把并行执行的功能改为串行执行，降低了执行效率。
- 增加了额外控制幂等的业务逻辑，复杂化了业务功能；

所以在使用时候需要考虑是否引入幂等性的必要性，根据实际业务场景具体分析，除了业务上的特殊要求外，一般情况下不需要引入的接口幂等性。
<a name="y1AHR"></a>
## 五、Restful API 接口的幂等性
现在流行的 Restful 推荐的几种 HTTP 接口方法中，分别存在幂等行与不能保证幂等的方法，如下：

- √ 满足幂等
- x 不满足幂等
- - 可能满足也可能不满足幂等，根据实际业务逻辑有关
| 方法类型 | 是否幂等 | 描述 |
| --- | --- | --- |
| Get | √ | Get 方法用于获取资源。其一般不会也不应当对系统资源进行改变，所以是幂等的。 |
| Post | × | Post 方法一般用于创建新的资源。其每次执行都会新增数据，所以不是幂等的。 |
| Put | - | Put 方法一般用于修改资源。该操作则分情况来判断是不是满足幂等，更新操作中直接根据某个值进行更新，也能保持幂等。不过执行累加操作的更新是非幂等。 |
| Delete | - | Delete 方法一般用于删除资源。该操作则分情况来判断是不是满足幂等，当根据唯一值进行删除时，删除同一个数据多次执行效果一样。不过需要注意，带查询条件的删除则就不一定满足幂等了。例如在根据条件删除一批数据后，这时候新增加了一条数据也满足条件，然后又执行了一次删除，那么将会导致新增加的这条满足条件数据也被删除。 |

<a name="Fl9QK"></a>
## 六、如何实现幂等性
<a name="kSrHq"></a>
### 方案一：数据库唯一主键
<a name="vwDAN"></a>
#### 方案描述
数据库唯一主键的实现主要是利用数据库中主键唯一约束的特性，一般来说唯一主键比较适用于“插入”时的幂等性，其能保证一张表中只能存在一条带该唯一主键的记录。<br />使用数据库唯一主键完成幂等性时需要注意的是，该主键一般来说并不是使用数据库中自增主键，而是使用分布式 ID 充当主键（可以参考 Java 中分布式 ID 的设计方案 这篇文章），这样才能能保证在分布式环境下 ID 的全局唯一性。
<a name="LZ6f8"></a>
#### 适用操作

- 插入操作
- 删除操作
<a name="LpqQv"></a>
#### 使用限制

- 需要生成全局唯一主键 ID；
<a name="CYD49"></a>
#### 主要流程
![2022-07-29-10-00-58.187094400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659060490223-9c28981d-b12a-4eb6-92a6-6ee803bcb264.png#clientId=u847c3bfb-bc4e-4&from=ui&id=ucf9d671b&originHeight=458&originWidth=970&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1335466&status=done&style=none&taskId=u34deff78-5c7b-46f6-83c8-1850f773639&title=)<br />主要流程：

- ① 客户端执行创建请求，调用服务端接口。
- ② 服务端执行业务逻辑，生成一个分布式 ID，将该 ID 充当待插入数据的主键，然后执数据插入操作，运行对应的 SQL 语句。
- ③ 服务端将该条数据插入数据库中，如果插入成功则表示没有重复调用接口。如果抛出主键重复异常，则表示数据库中已经存在该条记录，返回错误信息到客户端。
<a name="lL92O"></a>
### 方案二：数据库乐观锁
<a name="b5gbr"></a>
#### 方案描述
数据库乐观锁方案一般只能适用于执行“更新操作”的过程，可以提前在对应的数据表中多添加一个字段，充当当前数据的版本标识。这样每次对该数据库该表的这条数据执行更新时，都会将该版本标识作为一个条件，值为上次待更新数据中的版本标识的值。
<a name="C0gYY"></a>
#### 适用操作

- 更新操作
<a name="jxvE4"></a>
#### 使用限制

- 需要数据库对应业务表中添加额外字段；
<a name="yzqyx"></a>
#### 描述示例
![2022-07-29-10-00-58.315869000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659060455666-bdcedccc-c747-46ed-b6e1-5548d8ce6a24.png#clientId=u847c3bfb-bc4e-4&from=ui&id=C5xPr&originHeight=337&originWidth=970&originalType=binary&ratio=1&rotation=0&showTitle=false&size=982664&status=done&style=none&taskId=u548f6e72-0b3e-491c-9404-8ae7e8f97a2&title=)<br />例如，存在如下的数据表中：

| id | name | price |
| --- | --- | --- |
| 1 | 小米手机 | 1000 |
| 2 | 苹果手机 | 2500 |
| 3 | 华为手机 | 1600 |

为了每次执行更新时防止重复更新，确定更新的一定是要更新的内容，通常都会添加一个 version 字段记录当前的记录版本，这样在更新时候将该值带上，那么只要执行更新操作就能确定一定更新的是某个对应版本下的信息。

| id | name | price | version |
| --- | --- | --- | --- |
| 1 | 小米手机 | 1000 | 10 |
| 2 | 苹果手机 | 2500 | 21 |
| 3 | 华为手机 | 1600 | 5 |

这样每次执行更新时候，都要指定要更新的版本号，如下操作就能准确更新 version=5 的信息：
```xml
UPDATE my_table SET price=price+50,version=version+1 WHERE id=1 AND version=5
```
上面 WHERE 后面跟着条件 id=1 AND version=5 被执行后，id=1 的 version 被更新为 6，所以如果重复执行该条 SQL 语句将不生效，因为 id=1 AND version=5 的数据已经不存在，这样就能保住更新的幂等，多次更新对结果不会产生影响。
<a name="AqSfC"></a>
### 方案三：防重 Token 令牌
<a name="t4Tpx"></a>
#### 方案描述
针对客户端连续点击或者调用方的超时重试等情况，例如提交订单，此种操作就可以用 Token 的机制实现防止重复提交。简单的说就是调用方在调用接口的时候先向后端请求一个全局 ID（Token），请求的时候携带这个全局 ID 一起请求（Token 最好将其放到 Headers 中），后端需要对这个 Token 作为 Key，用户信息作为 Value 到 Redis 中进行键值内容校验，如果 Key 存在且 Value 匹配就执行删除命令，然后正常执行后面的业务逻辑。如果不存在对应的 Key 或 Value 不匹配就返回重复执行的错误信息，这样来保证幂等操作。
<a name="qrUcS"></a>
#### 适用操作

- 插入操作
- 更新操作
- 删除操作
<a name="KB1rh"></a>
#### 使用限制

- 需要生成全局唯一 Token 串；
- 需要使用第三方组件 Redis 进行数据效验；
<a name="glP6i"></a>
#### 主要流程
![2022-07-29-10-00-58.410282100.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659060510125-eaf75704-49e6-49f1-b020-1726338a0f35.png#clientId=u847c3bfb-bc4e-4&from=ui&id=u8537433d&originHeight=459&originWidth=970&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1338389&status=done&style=none&taskId=ucbdf881e-48b5-47be-bac7-64e583626c6&title=)

- ① 服务端提供获取 Token 的接口，该 Token 可以是一个序列号，也可以是一个分布式 ID 或者 UUID 串。
- ② 客户端调用接口获取 Token，这时候服务端会生成一个 Token 串。
- ③ 然后将该串存入 Redis 数据库中，以该 Token 作为 Redis 的键（注意设置过期时间）。
- ④ 将 Token 返回到客户端，客户端拿到后应存到表单隐藏域中。
- ⑤ 客户端在执行提交表单时，把 Token 存入到 Headers 中，执行业务请求带上该 Headers。
- ⑥ 服务端接收到请求后从 Headers 中拿到 Token，然后根据 Token 到 Redis 中查找该 key 是否存在。
- ⑦ 服务端根据 Redis 中是否存该 key 进行判断，如果存在就将该 key 删除，然后正常执行业务逻辑。如果不存在就抛异常，返回重复提交的错误信息。

注意，在并发情况下，执行 Redis 查找数据与删除需要保证原子性，否则很可能在并发下无法保证幂等性。其实现方法可以使用分布式锁或者使用 Lua 表达式来注销查询与删除操作。
<a name="c2VwW"></a>
### 方案四、下游传递唯一序列号
<a name="DNmLR"></a>
#### 方案描述
所谓请求序列号，其实就是每次向服务端请求时候附带一个短时间内唯一不重复的序列号，该序列号可以是一个有序 ID，也可以是一个订单号，一般由下游生成，在调用上游服务端接口时附加该序列号和用于认证的 ID。<br />当上游服务器收到请求信息后拿取该 序列号 和下游 认证ID 进行组合，形成用于操作 Redis 的 Key，然后到 Redis 中查询是否存在对应的 Key 的键值对，根据其结果：

- 如果存在，就说明已经对该下游的该序列号的请求进行了业务处理，这时可以直接响应重复请求的错误信息。
- 如果不存在，就以该 Key 作为 Redis 的键，以下游关键信息作为存储的值（例如下游商传递的一些业务逻辑信息），将该键值对存储到 Redis 中 ，然后再正常执行对应的业务逻辑即可。
<a name="eqJAG"></a>
#### 适用操作

- 插入操作
- 更新操作
- 删除操作
<a name="d7OLE"></a>
#### 使用限制

- 要求第三方传递唯一序列号；
- 需要使用第三方组件 Redis 进行数据效验；
<a name="e9pHT"></a>
#### 主要流程
![2022-07-29-10-00-58.592060900.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1659060510177-ec96198f-24e4-4961-922b-9fc1104374e6.png#clientId=u847c3bfb-bc4e-4&from=ui&id=NOmx6&originHeight=479&originWidth=970&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1396703&status=done&style=none&taskId=udcd938f2-9d63-4ef2-9f9b-031ddf4b05a&title=)<br />主要步骤：

- ① 下游服务生成分布式 ID 作为序列号，然后执行请求调用上游接口，并附带“唯一序列号”与请求的“认证凭据ID”。
- ② 上游服务进行安全效验，检测下游传递的参数中是否存在“序列号”和“凭据ID”。
- ③ 上游服务到 Redis 中检测是否存在对应的“序列号”与“认证ID”组成的 Key，如果存在就抛出重复执行的异常信息，然后响应下游对应的错误信息。如果不存在就以该“序列号”和“认证ID”组合作为 Key，以下游关键信息作为 Value，进而存储到 Redis 中，然后正常执行接来来的业务逻辑。

上面步骤中插入数据到 Redis 一定要设置过期时间。这样能保证在这个时间范围内，如果重复调用接口，则能够进行判断识别。如果不设置过期时间，很可能导致数据无限量的存入 Redis，致使 Redis 不能正常工作。
<a name="neb2n"></a>
## 七、实现接口幂等示例
这里使用防重 Token 令牌方案，该方案能保证在不同请求动作下的幂等性，实现逻辑可以看上面写的”防重 Token 令牌”方案，接下来写下实现这个逻辑的代码。
<a name="YfQe0"></a>
### 1、Maven 引入相关依赖
这里使用 Maven 工具管理依赖，这里在 pom.xml 中引入 SpringBoot、Redis、lombok 相关依赖。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
				 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>2.3.4.RELEASE</version>
	</parent>
	
	<groupId>mydlq.club</groupId>
	<artifactId>springboot-idempotent-token</artifactId>
	<version>0.0.1</version>
	<name>springboot-idempotent-token</name>
	<description>Idempotent Demo</description>
	
	<properties>
		<java.version>1.8</java.version>
	</properties>
	
	<dependencies>
		<!--springboot web-->
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>
		<!--springboot data redis-->
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-data-redis</artifactId>
		</dependency>
		<dependency>
			<groupId>org.apache.commons</groupId>
			<artifactId>commons-pool2</artifactId>
		</dependency>
		<!--lombok-->
		<dependency>
			<groupId>org.projectlombok</groupId>
			<artifactId>lombok</artifactId>
		</dependency>
	</dependencies>
	
	<build>
		<plugins>
			<plugin>
				<groupId>org.springframework.boot</groupId>
				<artifactId>spring-boot-maven-plugin</artifactId>
			</plugin>
		</plugins>
	</build>
	
</project>
```
<a name="Hq5q5"></a>
### 2、配置连接 Redis 的参数
在 application 配置文件中配置连接 Redis 的参数。Spring Boot 基础就不介绍了，最新教程推荐看下面的教程。<br />如下：
```yaml
spring:
  redis:
    ssl: false
    host: 127.0.0.1
    port: 6379
    database: 0
    timeout: 1000
    password:
    lettuce:
      pool:
        max-active: 100
        max-wait: -1
        min-idle: 0
        max-idle: 20
```
<a name="aMVSz"></a>
### 3、创建与验证 Token 工具类
创建用于操作 Token 相关的 Service 类，里面存在 Token 创建与验证方法，其中：

- **Token 创建方法：** 使用 UUID 工具创建 Token 串，设置以 “idempotent_token:“+“Token串” 作为 Key，以用户信息当成 Value，将信息存入 Redis 中。
- **Token 验证方法：** 接收 Token 串参数，加上 Key 前缀形成 Key，再传入 value 值，执行 Lua 表达式（Lua 表达式能保证命令执行的原子性）进行查找对应 Key 与删除操作。执行完成后验证命令的返回结果，如果结果不为空且非0，则验证成功，否则失败。
```java
import java.util.Arrays;
import java.util.UUID;
import java.util.concurrent.TimeUnit;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.data.redis.core.script.DefaultRedisScript;
import org.springframework.data.redis.core.script.RedisScript;
import org.springframework.stereotype.Service;

@Slf4j
@Service
public class TokenUtilService {

    @Autowired
    private StringRedisTemplate redisTemplate;

    /**
     * 存入 Redis 的 Token 键的前缀
     */
    private static final String IDEMPOTENT_TOKEN_PREFIX = "idempotent_token:";

    /**
     * 创建 Token 存入 Redis，并返回该 Token
     *
     * @param value 用于辅助验证的 value 值
     * @return 生成的 Token 串
     */
    public String generateToken(String value) {
        // 实例化生成 ID 工具对象
        String token = UUID.randomUUID().toString();
        // 设置存入 Redis 的 Key
        String key = IDEMPOTENT_TOKEN_PREFIX + token;
        // 存储 Token 到 Redis，且设置过期时间为5分钟
        redisTemplate.opsForValue().set(key, value, 5, TimeUnit.MINUTES);
        // 返回 Token
        return token;
    }

    /**
     * 验证 Token 正确性
     *
     * @param token token 字符串
     * @param value value 存储在Redis中的辅助验证信息
     * @return 验证结果
     */
    public boolean validToken(String token, String value) {
        // 设置 Lua 脚本，其中 KEYS[1] 是 key，KEYS[2] 是 value
        String script = "if redis.call('get', KEYS[1]) == KEYS[2] then return redis.call('del', KEYS[1]) else return 0 end";
        RedisScript<Long> redisScript = new DefaultRedisScript<>(script, Long.class);
        // 根据 Key 前缀拼接 Key
        String key = IDEMPOTENT_TOKEN_PREFIX + token;
        // 执行 Lua 脚本
        Long result = redisTemplate.execute(redisScript, Arrays.asList(key, value));
        // 根据返回结果判断是否成功成功匹配并删除 Redis 键值对，若果结果不为空和0，则验证通过
        if (result != null && result != 0L) {
            log.info("验证 token={},key={},value={} 成功", token, key, value);
            return true;
        }
        log.info("验证 token={},key={},value={} 失败", token, key, value);
        return false;
    }

}
```
<a name="b8scL"></a>
### 4、创建测试的 Controller 类
创建用于测试的 Controller 类，里面有获取 Token 与测试接口幂等性的接口，内容如下：
```java
import lombok.extern.slf4j.Slf4j;
import mydlq.club.example.service.TokenUtilService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@Slf4j
@RestController
public class TokenController {

    @Autowired
    private TokenUtilService tokenService;

    /**
     * 获取 Token 接口
     *
     * @return Token 串
     */
    @GetMapping("/token")
    public String getToken() {
        // 获取用户信息（这里使用模拟数据）
        // 注：这里存储该内容只是举例，其作用为辅助验证，使其验证逻辑更安全，如这里存储用户信息，其目的为:
        // - 1)、使用"token"验证 Redis 中是否存在对应的 Key
        // - 2)、使用"用户信息"验证 Redis 的 Value 是否匹配。
        String userInfo = "mydlq";
        // 获取 Token 字符串，并返回
        return tokenService.generateToken(userInfo);
    }

    /**
     * 接口幂等性测试接口
     *
     * @param token 幂等 Token 串
     * @return 执行结果
     */
    @PostMapping("/test")
    public String test(@RequestHeader(value = "token") String token) {
        // 获取用户信息（这里使用模拟数据）
        String userInfo = "mydlq";
        // 根据 Token 和与用户相关的信息到 Redis 验证是否存在对应的信息
        boolean result = tokenService.validToken(token, userInfo);
        // 根据验证结果响应不同信息
        return result ? "正常调用" : "重复调用";
    }

}
```
<a name="H0kbt"></a>
### 5、创建 SpringBoot 启动类
创建启动类，用于启动 SpringBoot 应用。基础教程就不介绍了，建议看下下面的教程，很全了。
```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }

}
```
<a name="nxGYF"></a>
### 6、写测试类进行测试
写个测试类进行测试，多次访问同一个接口，测试是否只有第一次能否执行成功。
```java
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;

@Slf4j
@SpringBootTest
@RunWith(SpringRunner.class)
public class IdempotenceTest {

    @Autowired
    private WebApplicationContext webApplicationContext;

    @Test
    public void interfaceIdempotenceTest() throws Exception {
        // 初始化 MockMvc
        MockMvc mockMvc = MockMvcBuilders.webAppContextSetup(webApplicationContext).build();
        // 调用获取 Token 接口
        String token = mockMvc.perform(MockMvcRequestBuilders.get("/token")
                .accept(MediaType.TEXT_HTML))
                .andReturn()
                .getResponse().getContentAsString();
        log.info("获取的 Token 串：{}", token);
        // 循环调用 5 次进行测试
        for (int i = 1; i <= 5; i++) {
            log.info("第{}次调用测试接口", i);
            // 调用验证接口并打印结果
            String result = mockMvc.perform(MockMvcRequestBuilders.post("/test")
                    .header("token", token)
                    .accept(MediaType.TEXT_HTML))
                    .andReturn().getResponse().getContentAsString();
            log.info(result);
            // 结果断言
            if (i == 0) {
                Assert.assertEquals(result, "正常调用");
            } else {
                Assert.assertEquals(result, "重复调用");
            }
        }
    }

}
```
显示如下：
```java
[main] IdempotenceTest:  获取的 Token 串：980ea707-ce2e-456e-a059-0a03332110b4
[main] IdempotenceTest:  第1次调用测试接口
[main] IdempotenceTest:  正常调用
[main] IdempotenceTest:  第2次调用测试接口
[main] IdempotenceTest:  重复调用
[main] IdempotenceTest:  第3次调用测试接口
[main] IdempotenceTest:  重复调用
[main] IdempotenceTest:  第4次调用测试接口
[main] IdempotenceTest:  重复调用
[main] IdempotenceTest:  第5次调用测试接口
[main] IdempotenceTest:  重复调用
```
<a name="oQowj"></a>
## 八、最后总结
幂等性是开发当中很常见也很重要的一个需求，尤其是支付、订单等与金钱挂钩的服务，保证接口幂等性尤其重要。在实际开发中，需要针对不同的业务场景需要灵活的选择幂等性的实现方式：

- 对于下单等存在唯一主键的，可以使用“唯一主键方案”的方式实现。
- 对于更新订单状态等相关的更新场景操作，使用“乐观锁方案”实现更为简单。
- 对于上下游这种，下游请求上游，上游服务可以使用“下游传递唯一序列号方案”更为合理。
- 类似于前端重复提交、重复下单、没有唯一ID号的场景，可以通过 Token 与 Redis 配合的“防重 Token 方案”实现更为快捷。

上面只是给与一些建议，再次强调一下，实现幂等性需要先理解自身业务需求，根据业务逻辑来实现这样才合理，处理好其中的每一个结点细节，完善整体的业务流程设计，才能更好的保证系统的正常运行。最后做一个简单总结

| 方案名称 | 适用方法 | 实现复杂度 | 方案缺点 |
| --- | --- | --- | --- |
| 数据库唯一主键 | 插入操作 删除操作 | 简单 | - 只能用于插入操作；- 只能用于存在唯一主键场景； |
| 数据库乐观锁 | 更新操作 | 简单 | - 只能用于更新操作；- 表中需要额外添加字段； |
| 请求序列号 | 插入操作 更新操作 删除操作 | 简单 | - 需要保证下游生成唯一序列号；- 需要 Redis 第三方存储已经请求的序列号； |
| 防重 Token 令牌 | 插入操作 更新操作 删除操作 | 适中 | - 需要 Redis 第三方存储生成的 Token 串； |

