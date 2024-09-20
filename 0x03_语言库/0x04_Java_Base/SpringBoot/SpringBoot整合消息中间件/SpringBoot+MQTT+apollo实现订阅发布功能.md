Java SpringBoot MQTT apollo 订阅发布<br />对于项目，首先需要引入maven包：
<a name="e4Bwh"></a>
#### pom.xml
```xml
<!-- MQTT -->
<dependency>
    <groupId>org.springframework.integration</groupId>
    <artifactId>spring-integration-stream</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.integration</groupId>
    <artifactId>spring-integration-mqtt</artifactId>
</dependency>
```
其目标就是将MQTT用在项目组中<br />接着就是项目yml文件的配置，使用properties，照葫芦画瓢就行了：
<a name="bnBwq"></a>
#### applicaiton.yml
```yaml
mqtt:
  username: admin
  password: password
  host-url: tcp://127.0.0.1:8161  # 你自己服务器的地址和端口，这个需要改
  clientID: test1       # 这个改不改随意，但不同的客户端肯定不能一样
  default-topic: home/garden/fountain      # 默认主题
  timeout: 100
  keepalive: 100

# Tomcat
server:
  tomcat:
    uri-encoding: UTF-8
    max-threads: 1000
    min-spare-threads: 30
  port: 8088
```
注意host-url，这就是apollo的地址<br />来到第三步，此时就是项目内的文件：
<a name="uTFvl"></a>
#### MqttConfig文件
```java
@Component
@ConfigurationProperties("mqtt")
@Setter
@Getter
public class MqttConfig {
    @Autowired
    private MqttPushClient mqttPushClient;

    /**
     * 用户名
     */
    // @Value("username")
    private String username;
    /**
     * 密码
     */
    private String password;
    /**
     * 连接地址
     */
    private String hostUrl;
    /**
     * 客户Id
     */
    private String clientID;
    /**
     * 默认连接话题
     */
    private String defaultTopic;
    /**
     * 超时时间
     */
    private int timeout;
    /**
     * 保持连接数
     */
    private int keepalive;

    @Bean
    public MqttPushClient getMqttPushClient() {
        System.out.println("hostUrl: "+ hostUrl);
        System.out.println("clientID: "+ clientID);
        System.out.println("username: "+ username);
        System.out.println("password: "+ password);
        System.out.println("timeout: "+timeout);
        System.out.println("keepalive: "+ keepalive);
        mqttPushClient.connect(hostUrl, clientID, username, password, timeout, keepalive);
        // 以/#结尾表示订阅所有以test开头的主题
        mqttPushClient.subscribe(defaultTopic, 0);
        return mqttPushClient;
    }
}
```
目的就是配置所对应的消息<br />第四步就是发布以及订阅等功能：
<a name="yzGfG"></a>
#### MqttPushClient
```java
@Component
public class MqttPushClient {
    private static final Logger logger = LoggerFactory.getLogger(MqttPushClient.class);

    @Autowired
    private PushCallback pushCallback;

    private static MqttClient client;

    private static MqttClient getClient() {
        return client;
    }

    private static void setClient(MqttClient client) {
        MqttPushClient.client = client;
    }

    /**
     * 客户端连接
     *
     * @param host      ip+端口
     * @param clientID  客户端Id
     * @param username  用户名
     * @param password  密码
     * @param timeout   超时时间
     * @param keepalive 保留数
     */
    public void connect(String host, String clientID, String username, String password, int timeout, int keepalive) {
        MqttClient client;
        try {
            client = new MqttClient(host, clientID, new MemoryPersistence());
            MqttConnectOptions options = new MqttConnectOptions();
            options.setCleanSession(true);
            options.setUserName(username);
            options.setPassword(password.toCharArray());
            options.setConnectionTimeout(timeout);
            options.setKeepAliveInterval(keepalive);
            MqttPushClient.setClient(client);
            try {
                client.setCallback(pushCallback);
                client.connect(options);
            } catch (Exception e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 发布
     *
     * @param qos         连接方式
     * @param retained    是否保留
     * @param topic       主题
     * @param pushMessage 消息体
     */
    public void publish(int qos, boolean retained, String topic, String pushMessage) {
        MqttMessage message = new MqttMessage();
        message.setQos(qos);
        message.setRetained(retained);
        message.setPayload(pushMessage.getBytes());
        MqttTopic mTopic = MqttPushClient.getClient().getTopic(topic);
        if (null == mTopic) {
            logger.error("topic not exist");
        }
        MqttDeliveryToken token;
        try {
            token = mTopic.publish(message);
            token.waitForCompletion();
        } catch (MqttPersistenceException e) {
            e.printStackTrace();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    /**
     * 订阅某个主题
     *
     * @param topic 主题
     * @param qos   连接方式
     */
    public void subscribe(String topic, int qos) {
        logger.info("开始订阅主题" + topic);
        try {
            MqttPushClient.getClient().subscribe(topic, qos);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
```
订阅主题以及发布的方式等内容更多编写<br />最后在测试看看结果是否正确：
<a name="zEE8x"></a>
#### TestController
```java
@RestController
@RequestMapping("/")
public class TestController {

    @Autowired
    private MqttPushClient mqttPushClient;

    @GetMapping(value = "/publishTopic")
    public String publishTopic() {
        String topicString = "home/garden/fountain";
        mqttPushClient.publish(0, false, topicString, "测试一下发布消息");
        return "ok";
    }
    // 发送自定义消息内容（使用默认主题）
    @RequestMapping("/publishTopic/{data}")
    public String test1(@PathVariable("data") String data) {
        String topicString = "home/garden/fountain";
        mqttPushClient.publish(0,false,topicString, data);
        return "ok";
    }

    // 发送自定义消息内容，且指定主题
    @RequestMapping("/publishTopic/{topic}/{data}")
    public String test2(@PathVariable("topic") String topic, @PathVariable("data") String data) {
        mqttPushClient.publish(0,false,topic, data);
        return "ok";
    }
}
```
<br />可以使用MQTT.fx进行测试。用Postman发出，就能够查看最终的结果。
