Java SpringBoot Redis
<a name="bpMdL"></a>
## 业务场景
以订单功能为例说明下：<br />生成订单后一段时间不支付订单会自动关闭。最简单的想法是设置定时任务轮询，但是每个订单的创建时间不一样，定时任务的规则无法设定，如果将定时任务执行的间隔设置的过短，太影响效率。<br />还有一种想法，在用户进入订单界面的时候，判断时间执行相关操作。方式可能有很多，在这里介绍一种监听 Redis 键值对过期时间来实现订单自动关闭。
<a name="qvRzd"></a>
## 实现思路
在生成订单时，向 Redis 中增加一个 KV 键值对，K 为订单号，保证通过 K 能定位到数据库中的某个订单即可，V 可为任意值。<br />假设，生成订单时向 Redis 中存放 K 为订单号，V 也为订单号的键值对，并设置过期时间为 30 分钟，如果该键值对在 30 分钟过期后能够发送给程序一个通知，或者执行一个方法，那么即可解决订单关闭问题。<br />实现：通过监听 Redis 提供的过期队列来实现，监听过期队列后，如果 Redis 中某一个 KV 键值对过期了，那么将向监听者发送消息，监听者可以获取到该键值对的 K，注意，是获取不到 V 的，因为已经过期了，这就是上面所提到的，为什么要保证能通过 K 来定位到订单，而 V 为任意值即可。拿到 K 后，通过 K 定位订单，并判断其状态，如果是未支付，更新为关闭，或者取消状态即可。
<a name="yYhia"></a>
## 开启 Redis key 过期提醒
修改 redis 相关事件配置。找到 redis 配置文件 redis.conf，查看 notify-keyspace-events 配置项，如果没有，添加 notify-keyspace-events Ex，如果有值，则追加 Ex，相关参数说明如下：

- `K`：keyspace 事件，事件以 `keyspace@` 为前缀进行发布
- `E`：keyevent 事件，事件以 `keyevent@` 为前缀进行发布
- `g`：一般性的，非特定类型的命令，比如`del`，`expire`，`rename`等
- `$`：字符串特定命令
- `l`：列表特定命令
- `s`：集合特定命令
- `h`：哈希特定命令
- `z`：有序集合特定命令
- `x`：过期事件，当某个键过期并删除时会产生该事件
- `e`：驱逐事件，当某个键因 maxmemore 策略而被删除时，产生该事件
- `A`：`g$lshzxe`的别名，因此"AKE"意味着所有事件
<a name="NrPHT"></a>
## 引入依赖
在 pom.xml 中添加 org.springframework.boot:spring-boot-starter-data-redis 依赖
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
<a name="lgP9a"></a>
## 相关配置
定义配置 RedisListenerConfig 实现监听 Redis key 过期时间
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.listener.RedisMessageListenerContainer;

@Configuration
public class RedisListenerConfig {

    @Bean
    RedisMessageListenerContainer container(RedisConnectionFactory connectionFactory) {

        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        return container;
    }
}
```
定义监听器 `RedisKeyExpirationListener`，实现`KeyExpirationEventMessageListener`接口，查看源码发现，该接口监听所有 db 的过期事件 `keyevent@*:expired"`
```java
import org.springframework.data.redis.connection.Message;
import org.springframework.data.redis.listener.KeyExpirationEventMessageListener;
import org.springframework.data.redis.listener.RedisMessageListenerContainer;
import org.springframework.stereotype.Component;

/**
 * 监听所有db的过期事件__keyevent@*__:expired"
 */
@Component
public class RedisKeyExpirationListener extends KeyExpirationEventMessageListener {

    public RedisKeyExpirationListener(RedisMessageListenerContainer listenerContainer) {
        super(listenerContainer);
    }

    /**
     * 针对 redis 数据失效事件，进行数据处理
     * @param message
     * @param pattern
     */
    @Override
    public void onMessage(Message message, byte[] pattern) {

        // 获取到失效的 key，进行取消订单业务处理
        String expiredKey = message.toString();
        System.out.println(expiredKey);
    }
}
```
