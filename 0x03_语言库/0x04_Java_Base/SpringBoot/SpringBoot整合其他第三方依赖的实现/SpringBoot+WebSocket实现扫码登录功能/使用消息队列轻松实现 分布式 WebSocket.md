Java<br />分布式下WebSocket如何实现？一个服务中建立了链接，另一个服务的用户怎么获取消息？<br />**如果项目是分布式环境，登录的用户被Nginx的反向代理分配到多个不同服务器，那么在其中一个服务器建立了WebSocket连接的用户如何给在另外一个服务器上建立了WebSocket连接的用户发送消息呢？**<br />其实，要解决这个问题就需要实现分布式WebSocket，而分布式WebSocket一般可以通过以下两种方案来实现：

1. 将消息（<**用户id**，**消息内容**>）统一推送到一个消息队列（Redis、Kafka等）的的topic，然后每个应用节点都订阅这个topic，在接收到WebSocket消息后取出这个消息的“**消息接收者的用户ID/用户名**”，然后再比对自身是否存在相应用户的连接，如果存在则推送消息，否则丢弃接收到的这个消息（这个消息接收者所在的应用节点会处理）
2. 在用户建立WebSocket连接后，使用Redis缓存记录用户的WebSocket建立在哪个应用节点上，然后同样使用消息队列将消息推送到接收者所在的应用节点上面（实现上比方案一要复杂，但是网络流量会更低）
<a name="degAq"></a>
## 实现方案
<a name="xF9wc"></a>
### 1、定义一个WebSocket Channel枚举类
```java
public enum WebSocketChannelEnum {
    //测试使用的简易点对点聊天
    CHAT("CHAT", "测试使用的简易点对点聊天", "/topic/reply");

    WebSocketChannelEnum(String code, String description, String subscribeUrl) {
        this.code = code;
        this.description = description;
        this.subscribeUrl = subscribeUrl;
    }

    /**
     * 唯一CODE
     */
    private String code;
    /**
     * 描述
     */
    private String description;
    /**
     * WebSocket客户端订阅的URL
     */
    private String subscribeUrl;

    public String getCode() {
        return code;
    }

    public String getDescription() {
        return description;
    }

    public String getSubscribeUrl() {
        return subscribeUrl;
    }

    /**
     * 通过CODE查找枚举类
     */
    public static WebSocketChannelEnum fromCode(String code){
        if(StringUtils.isNoneBlank(code)){
            for(WebSocketChannelEnum channelEnum : values()){
                if(channelEnum.code.equals(code)){
                    return channelEnum;
                }
            }
        }

        return null;
    }

}
```
<a name="SB65e"></a>
### 2、配置基于Redis的消息队列
需要注意的是，在大中型正式项目中并不推荐使用Redis实现的消息队列，因为经过测试它并不是特别可靠，所以应该考虑使用Kafka、rabbitMQ等专业的消息队列中间件
```java
@Configuration
@ConditionalOnClass({JedisCluster.class})
public class RedisConfig {
 
    @Value("${spring.redis.timeout}")
    private String timeOut;
 
    @Value("${spring.redis.cluster.nodes}")
    private String nodes;
 
    @Value("${spring.redis.cluster.max-redirects}")
    private int maxRedirects;
 
    @Value("${spring.redis.jedis.pool.max-active}")
    private int maxActive;
 
    @Value("${spring.redis.jedis.pool.max-wait}")
    private int maxWait;
 
    @Value("${spring.redis.jedis.pool.max-idle}")
    private int maxIdle;
 
    @Value("${spring.redis.jedis.pool.min-idle}")
    private int minIdle;
 
    @Value("${spring.redis.message.topic-name}")
    private String topicName;
 
    @Bean
    public JedisPoolConfig jedisPoolConfig(){
        JedisPoolConfig config = new JedisPoolConfig();
        config.setMaxTotal(maxActive);
        config.setMaxIdle(maxIdle);
        config.setMinIdle(minIdle);
        config.setMaxWaitMillis(maxWait);
 
        return config;
    }
 
    @Bean
    public RedisClusterConfiguration redisClusterConfiguration(){
        RedisClusterConfiguration configuration = new RedisClusterConfiguration(Arrays.asList(nodes));
        configuration.setMaxRedirects(maxRedirects);
 
        return configuration;
    }
 
    /**
     * JedisConnectionFactory
     */
    @Bean
    public JedisConnectionFactory jedisConnectionFactory(RedisClusterConfiguration configuration,JedisPoolConfig jedisPoolConfig){
        return new JedisConnectionFactory(configuration,jedisPoolConfig);
    }
 
    /**
     * 使用Jackson序列化对象
     */
    @Bean
    public Jackson2JsonRedisSerializer<Object> jackson2JsonRedisSerializer(){
        Jackson2JsonRedisSerializer<Object> serializer = new Jackson2JsonRedisSerializer<Object>(Object.class);
 
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.setVisibility(PropertyAccessor.ALL, JsonAutoDetect.Visibility.ANY);
        objectMapper.enableDefaultTyping(ObjectMapper.DefaultTyping.NON_FINAL);
        serializer.setObjectMapper(objectMapper);
 
        return serializer;
    }
 
    /**
     * RedisTemplate
     */
    @Bean
    public RedisTemplate<String, Object> redisTemplate(JedisConnectionFactory factory, Jackson2JsonRedisSerializer<Object> jackson2JsonRedisSerializer){
        RedisTemplate<String, Object> redisTemplate = new RedisTemplate<>();
        redisTemplate.setConnectionFactory(factory);
 
        //字符串方式序列化KEY
        StringRedisSerializer stringRedisSerializer = new StringRedisSerializer();
        redisTemplate.setKeySerializer(stringRedisSerializer);
        redisTemplate.setHashKeySerializer(stringRedisSerializer);
 
        //JSON方式序列化VALUE
        redisTemplate.setValueSerializer(jackson2JsonRedisSerializer);
        redisTemplate.setHashValueSerializer(jackson2JsonRedisSerializer);
 
        redisTemplate.afterPropertiesSet();
 
        return redisTemplate;
    }
 
    /**
     * 消息监听器
     */
    @Bean
    MessageListenerAdapter messageListenerAdapter(MessageReceiver messageReceiver, Jackson2JsonRedisSerializer<Object> jackson2JsonRedisSerializer){
        //消息接收者以及对应的默认处理方法
        MessageListenerAdapter messageListenerAdapter = new MessageListenerAdapter(messageReceiver, "receiveMessage");
        //消息的反序列化方式
        messageListenerAdapter.setSerializer(jackson2JsonRedisSerializer);
 
        return messageListenerAdapter;
    }
 
    /**
     * message listener container
     */
    @Bean
    RedisMessageListenerContainer container(RedisConnectionFactory connectionFactory
            , MessageListenerAdapter messageListenerAdapter){
        RedisMessageListenerContainer container = new RedisMessageListenerContainer();
        container.setConnectionFactory(connectionFactory);
        //添加消息监听器
        container.addMessageListener(messageListenerAdapter, new PatternTopic(topicName));
 
        return container;
    }
 
}
```
需要注意的是，这里使用的配置如下所示：
```yaml
spring:
  ...
  #redis
  redis:
      cluster:
        nodes: namenode22:6379,datanode23:6379,datanode24:6379
        max-redirects: 6
      timeout: 300000
      jedis:
        pool:
          max-active: 8
          max-wait: 100000
          max-idle: 8
          min-idle: 0
      #自定义的监听的TOPIC路径
      message:
        topic-name: topic-test
```
<a name="aiNOo"></a>
### 3、定义一个Redis消息的处理者
```java
@Component
public class MessageReceiver {
    private final Logger logger = LoggerFactory.getLogger(getClass());

    @Autowired
    private SimpMessagingTemplate messagingTemplate;

    @Autowired
    private SimpUserRegistry userRegistry;

    /**
     * 处理WebSocket消息
     */
    public void receiveMessage(RedisWebsocketMsg redisWebsocketMsg) {
        logger.info(MessageFormat.format("Received Message: {0}", redisWebsocketMsg));
        //1. 取出用户名并判断是否连接到当前应用节点的WebSocket
        SimpUser simpUser = userRegistry.getUser(redisWebsocketMsg.getReceiver());

        if(simpUser != null && StringUtils.isNoneBlank(simpUser.getName())){
            //2. 获取WebSocket客户端的订阅地址
            WebSocketChannelEnum channelEnum = WebSocketChannelEnum.fromCode(redisWebsocketMsg.getChannelCode());

            if(channelEnum != null){
                //3. 给WebSocket客户端发送消息
                messagingTemplate.convertAndSendToUser(redisWebsocketMsg.getReceiver(), channelEnum.getSubscribeUrl(), redisWebsocketMsg.getContent());
            }
        }

    }
}
```
<a name="t4gN4"></a>
### 4、在Controller中发送WebSocket消息
```java
@Controller
@RequestMapping(("/wsTemplate"))
public class RedisMessageController {
    private final Logger logger = LoggerFactory.getLogger(getClass());
 
    @Value("${spring.redis.message.topic-name}")
    private String topicName;
 
    @Autowired
    private SimpMessagingTemplate messagingTemplate;
 
    @Autowired
    private SimpUserRegistry userRegistry;
 
    @Resource(name = "redisServiceImpl")
    private RedisService redisService;
 
    /**
     * 给指定用户发送WebSocket消息
     */
    @PostMapping("/sendToUser")
    @ResponseBody
    public String chat(HttpServletRequest request) {
        //消息接收者
        String receiver = request.getParameter("receiver");
        //消息内容
        String msg = request.getParameter("msg");
        HttpSession session = SpringContextUtils.getSession();
        User loginUser = (User) session.getAttribute(Constants.SESSION_USER);
 
        HelloMessage resultData = new HelloMessage(MessageFormat.format("{0} say: {1}", loginUser.getUsername(), msg));
        this.sendToUser(loginUser.getUsername(), receiver, WebSocketChannelEnum.CHAT.getSubscribeUrl(), JsonUtils.toJson(resultData));
 
        return "ok";
    }
 
    /**
     * 给指定用户发送消息，并处理接收者不在线的情况
     * @param sender 消息发送者
     * @param receiver 消息接收者
     * @param destination 目的地
     * @param payload 消息正文
     */
    private void sendToUser(String sender, String receiver, String destination, String payload){
        SimpUser simpUser = userRegistry.getUser(receiver);
 
        //如果接收者存在，则发送消息
        if(simpUser != null && StringUtils.isNoneBlank(simpUser.getName())){
            messagingTemplate.convertAndSendToUser(receiver, destination, payload);
        }
        //如果接收者在线，则说明接收者连接了集群的其他节点，需要通知接收者连接的那个节点发送消息
        else if(redisService.isSetMember(Constants.REDIS_WEBSOCKET_USER_SET, receiver)){
            RedisWebsocketMsg<String> redisWebsocketMsg = new RedisWebsocketMsg<>(receiver, WebSocketChannelEnum.CHAT.getCode(), payload);
 
            redisService.convertAndSend(topicName, redisWebsocketMsg);
        }
        //否则将消息存储到redis，等用户上线后主动拉取未读消息
        else{
            //存储消息的Redis列表名
            String listKey = Constants.REDIS_UNREAD_MSG_PREFIX + receiver + ":" + destination;
            logger.info(MessageFormat.format("消息接收者{0}还未建立WebSocket连接，{1}发送的消息【{2}】将被存储到Redis的【{3}】列表中", receiver, sender, payload, listKey));
 
            //存储消息到Redis中
            redisService.addToListRight(listKey, ExpireEnum.UNREAD_MSG, payload);
        }
 
    }
 
 
    /**
     * 拉取指定监听路径的未读的WebSocket消息
     * @param destination 指定监听路径
     * @return java.util.Map<java.lang.String,java.lang.Object>
     */
    @PostMapping("/pullUnreadMessage")
    @ResponseBody
    public Map<String, Object> pullUnreadMessage(String destination){
        Map<String, Object> result = new HashMap<>();
        try {
            HttpSession session = SpringContextUtils.getSession();
            //当前登录用户
            User loginUser = (User) session.getAttribute(Constants.SESSION_USER);
 
            //存储消息的Redis列表名
            String listKey = Constants.REDIS_UNREAD_MSG_PREFIX + loginUser.getUsername() + ":" + destination;
            //从Redis中拉取所有未读消息
            List<Object> messageList = redisService.rangeList(listKey, 0, -1);
 
            result.put("code", "200");
            if(messageList !=null && messageList.size() > 0){
                //删除Redis中的这个未读消息列表
                redisService.delete(listKey);
                //将数据添加到返回集，供前台页面展示
                result.put("result", messageList);
            }
        }catch (Exception e){
            result.put("code", "500");
            result.put("msg", e.getMessage());
        }
 
        return result;
    }
 
}
```
<a name="OxQgj"></a>
### 5、WebSocket相关配置
```java
@Configuration
@EnableWebSocketMessageBroker
public class WebSocketConfig implements WebSocketMessageBrokerConfigurer{
    @Autowired
    private AuthHandshakeInterceptor authHandshakeInterceptor;
 
    @Autowired
    private MyHandshakeHandler myHandshakeHandler;
 
    @Autowired
    private MyChannelInterceptor myChannelInterceptor;
 
    @Override
    public void registerStompEndpoints(StompEndpointRegistry registry) {
        registry.addEndpoint("/chat-websocket")
                .addInterceptors(authHandshakeInterceptor)
                .setHandshakeHandler(myHandshakeHandler)
                .withSockJS();
    }
 
    @Override
    public void configureMessageBroker(MessageBrokerRegistry registry) {
        //客户端需要把消息发送到/message/xxx地址
        registry.setApplicationDestinationPrefixes("/message");
        //服务端广播消息的路径前缀，客户端需要相应订阅/topic/yyy这个地址的消息
        registry.enableSimpleBroker("/topic");
        //给指定用户发送消息的路径前缀，默认值是/user/
        registry.setUserDestinationPrefix("/user/");
    }
 
    @Override
    public void configureClientInboundChannel(ChannelRegistration registration) {
        registration.interceptors(myChannelInterceptor);
    }
 
}
```
<a name="K6tJh"></a>
### 6、示例页面
```html
<head>
    <meta content="text/html;charset=UTF-8"/>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <meta http-equiv="X-UA-Compatible" content="IE=edge"/>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <title>Chat With STOMP Message</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/sockjs-client/1.1.4/sockjs.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/stomp.js/2.3.3/stomp.min.js"></script>
    <script th:src="@{/layui/layui.js}"></script>
    <script th:src="@{/layui/lay/modules/layer.js}"></script>
    <link th:href="@{/layui/css/layui.css}" rel="stylesheet">
    <link th:href="@{/layui/css/modules/layer/default/layer.css}" rel="stylesheet">
    <link th:href="@{/css/style.css}" rel="stylesheet">
    <style type="text/css">
        #connect-container {
            margin: 0 auto;
            width: 400px;
        }
 
        #connect-container div {
            padding: 5px;
            margin: 0 7px 10px 0;
        }
 
        .message input {
            padding: 5px;
            margin: 0 7px 10px 0;
        }
 
        .layui-btn {
            display: inline-block;
        }
    </style>
    <script type="text/javascript">
        var stompClient = null;
 
        $(function () {
            var target = $("#target");
            if (window.location.protocol === 'http:') {
                target.val('http://' + window.location.host + target.val());
            } else {
                target.val('https://' + window.location.host + target.val());
            }
        });
 
        function setConnected(connected) {
            var connect = $("#connect");
            var disconnect = $("#disconnect");
            var echo = $("#echo");
 
            if (connected) {
                connect.addClass("layui-btn-disabled");
                disconnect.removeClass("layui-btn-disabled");
                echo.removeClass("layui-btn-disabled");
            } else {
                connect.removeClass("layui-btn-disabled");
                disconnect.addClass("layui-btn-disabled");
                echo.addClass("layui-btn-disabled");
            }
 
            connect.attr("disabled", connected);
            disconnect.attr("disabled", !connected);
            echo.attr("disabled", !connected);
        }
 
        //连接
        function connect() {
            var target = $("#target").val();
 
            var ws = new SockJS(target);
            stompClient = Stomp.over(ws);
 
            stompClient.connect({}, function () {
                setConnected(true);
                log('Info: STOMP connection opened.');
 
                //连接成功后，主动拉取未读消息
                pullUnreadMessage("/topic/reply");
 
                //订阅服务端的/topic/reply地址
                stompClient.subscribe("/user/topic/reply", function (response) {
                    log(JSON.parse(response.body).content);
                })
            },function () {
                //断开处理
                setConnected(false);
                log('Info: STOMP connection closed.');
            });
        }
 
        //断开连接
        function disconnect() {
            if (stompClient != null) {
                stompClient.disconnect();
                stompClient = null;
            }
            setConnected(false);
            log('Info: STOMP connection closed.');
        }
 
        //向指定用户发送消息
        function sendMessage() {
            if (stompClient != null) {
                var receiver = $("#receiver").val();
                var msg = $("#message").val();
                log('Sent: ' + JSON.stringify({'receiver': receiver, 'msg':msg}));
 
                $.ajax({
                    url: "/wsTemplate/sendToUser",
                    type: "POST",
                    dataType: "json",
                    async: true,
                    data: {
                        "receiver": receiver,
                        "msg": msg
                    },
                    success: function (data) {
 
                    }
                });
            } else {
                layer.msg('STOMP connection not established, please connect.', {
                    offset: 'auto'
                    ,icon: 2
                });
            }
        }
 
        //从服务器拉取未读消息
        function pullUnreadMessage(destination) {
            $.ajax({
                url: "/wsTemplate/pullUnreadMessage",
                type: "POST",
                dataType: "json",
                async: true,
                data: {
                    "destination": destination
                },
                success: function (data) {
                    if (data.result != null) {
                        $.each(data.result, function (i, item) {
                            log(JSON.parse(item).content);
                        })
                    } else if (data.code !=null && data.code == "500") {
                        layer.msg(data.msg, {
                            offset: 'auto'
                            ,icon: 2
                        });
                    }
                }
            });
        }
 
        //日志输出
        function log(message) {
            console.debug(message);
        }
    </script>
</head>
<body>
    <noscript><h2 style="color: #ff0000">Seems your browser doesn't support Javascript! Websockets rely on Javascript being
        enabled. Please enable
        Javascript and reload this page!</h2></noscript>
    <div>
        <div id="connect-container" class="layui-elem-field">
            <legend>Chat With STOMP Message</legend>
            <div>
                <input id="target" type="text" class="layui-input" size="40" style="width: 350px" value="/chat-websocket"/>
            </div>
            <div>
                <button id="connect" class="layui-btn layui-btn-normal" onclick="connect();">Connect</button>
                <button id="disconnect" class="layui-btn layui-btn-normal layui-btn-disabled" disabled="disabled"
                        onclick="disconnect();">Disconnect
                </button>
 
            </div>
            <div class="message">
                <input id="receiver" type="text" class="layui-input" size="40" style="width: 350px" placeholder="接收者姓名" value=""/>
                <input id="message" type="text" class="layui-input" size="40" style="width: 350px" placeholder="消息内容" value=""/>
            </div>
            <div>
                <button id="echo" class="layui-btn layui-btn-normal layui-btn-disabled" disabled="disabled"
                        onclick="sendMessage();">Send Message
                </button>
            </div>
        </div>
    </div>
</body>
</html>
```
