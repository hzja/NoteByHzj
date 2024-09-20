Java
<a name="OHxR0"></a>
## 前言
旧项目 MySQL Java 升级驱动，本来一切都好好的，但是升级到 8.x 的驱动后，发现入库的时间比实际时间相差 13 个小时，这就很奇怪了，如果相差 8 小时，那么还可以说是时区不对，从驱动源码分析看看。
<a name="avlUA"></a>
## 1、Demo
pom 依赖，构造一个真实案例，这里的 8.0.22 版本。
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
        <version>2.5.4</version>
    </dependency>
    <dependency>
        <groupId>org.mybatis.spring.boot</groupId>
        <artifactId>mybatis-spring-boot-starter</artifactId>
        <version>2.2.0</version>
        <exclusions>
            <exclusion>
                <artifactId>slf4j-api</artifactId>
                <groupId>org.slf4j</groupId>
            </exclusion>
        </exclusions>
    </dependency>
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>8.0.22</version>
        <scope>runtime</scope>
    </dependency>
</dependencies>
```
随意写一个 dao controller main。
```java
@SpringBootApplication
@MapperScan("com.feng.mysql.rep")
public class MySQLDateMain {
    public static void main(String[] args) {
        SpringApplication.run(MySQLDateMain.class, args);
    }
}

@RestController
public class UserController {

    @Autowired
    private UserRepository userRepository;

    @RequestMapping(value = "/Users/User", method = RequestMethod.POST)
    public String addUser(){
        UserEntity userEntity = new UserEntity();
        userEntity.setAge(12);
        userEntity.setName("tom");
        userEntity.setCreateDate(new Date(System.currentTimeMillis()));
        userEntity.setUpdateDate(new Timestamp(System.currentTimeMillis()));
        userRepository.insertUser(userEntity);
        return "ok";
    }
}

@Mapper
public interface UserRepository {

    @Insert("insert into User (name, age, createDate, updateDate) values (#{name}, #{age}, #{createDate}, #{updateDate})")
    int insertUser(UserEntity userEntity);
}
```
数据库设计：
```sql
CREATE TABLE `work`.`User`  (
  `id` int(10) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `age` int NULL DEFAULT NULL,
  `createDate` timestamp NULL DEFAULT NULL,
  `updateDate` datetime NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 29 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;
```
<a name="jcCUs"></a>
### 1.1 验证
系统时间<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756803240-65e9174e-5138-4e58-a553-1195aee2fff1.png#averageHue=%23f7f7f7&clientId=u87bba4a2-d53b-4&from=paste&height=294&id=ua0923e77&originHeight=735&originWidth=1083&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69681&status=done&style=shadow&taskId=u997df122-1697-4ad1-9171-877993e5879&title=&width=433.2)<br />调用 HTTP 接口 http://localhost:8080/Users/User<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756817873-bee09052-3755-4fb3-b186-abe03f91fe47.png#averageHue=%23f6ebea&clientId=u87bba4a2-d53b-4&from=paste&height=92&id=uc02c0bc8&originHeight=229&originWidth=1582&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69064&status=done&style=shadow&taskId=u1d148ada-e834-4453-8351-33966fd15a6&title=&width=632.8)<br />可以看到与真实时间相差 13 小时，诡异了，明明数据库时间是正确的， 而且系统时间也是正确的，那么可以就只能在驱动找原因。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756831519-4511c7f8-0833-4a13-99e7-fc645df86010.png#averageHue=%23111111&clientId=u87bba4a2-d53b-4&from=paste&height=108&id=u2edcd95c&originHeight=270&originWidth=1561&originalType=binary&ratio=1&rotation=0&showTitle=false&size=83045&status=done&style=shadow&taskId=ud7c8c3e4-f5b2-43a9-a9a9-2c362e9e282&title=&width=624.4)
<a name="oHMSd"></a>
## 2、问题原因分析
<a name="qAuv5"></a>
### 2.1 时区获取
上一步，看见系统时间，数据库时间都是正确的，那么做文章的推断就是驱动造成的，以 8.0.22 驱动为例。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756879451-5f8eb445-0364-4caf-a3a9-6543db799841.png#averageHue=%232f2422&clientId=u87bba4a2-d53b-4&from=paste&id=u698901d3&originHeight=245&originWidth=1567&originalType=binary&ratio=1&rotation=0&showTitle=false&size=100942&status=done&style=shadow&taskId=u00dfa3e0-a8ce-4b74-8595-9217ac6fa7a&title=)<br />使用的驱动是 com.mysql.cj.jdbc.Driver。<br />当应用启动后，首次发起数据库操作，就会创建 JDBC 的代码，MyBatis 把这事情干了，获取连接，从连接池，这里使用 HikariDataSource,HikariPool连接池。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756908061-c3dcc9e9-9296-499c-ab46-a8358f5e279d.png#averageHue=%23232322&clientId=u87bba4a2-d53b-4&from=paste&id=u2605c94f&originHeight=322&originWidth=1612&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154105&status=done&style=shadow&taskId=u68ed9e37-977c-4d5d-942d-5e7cb5f4749&title=)<br />在 com.mysql.cj.jdbc.ConnectionImpl 里面会初始化 session 的拦截器，属性Variables、列映射、自动提交信息等等，其中有一行代码初始化时区：
```java
this.session.getProtocol().initServerSession();
```
com.mysql.cj.protocol.a.NativeProtocol
```java
public void configureTimezone() {
    //获取MySQL server端的时区
    String configuredTimeZoneOnServer = this.serverSession.getServerVariable("time_zone");
    //如果是SYSTEM，则获取系统时区
    if ("SYSTEM".equalsIgnoreCase(configuredTimeZoneOnServer)) {
        configuredTimeZoneOnServer = this.serverSession.getServerVariable("system_time_zone");
    }
    //配置文件获取时区serverTimezone配置，即可以手动配置，这是一个解决问题的手段
    String canonicalTimezone = getPropertySet().getStringProperty(PropertyKey.serverTimezone).getValue();
    //未指定时区，且读取到MySQL时区，就
    if (configuredTimeZoneOnServer != null) {
        // user can override this with driver properties, so don't detect if that's the case
        if (canonicalTimezone == null || StringUtils.isEmptyOrWhitespaceOnly(canonicalTimezone)) {
            try {
                //规范时区？难道直接读取的不规范😅，这步很重要
                canonicalTimezone = TimeUtil.getCanonicalTimezone(configuredTimeZoneOnServer, getExceptionInterceptor());
            } catch (IllegalArgumentException iae) {
                throw ExceptionFactory.createException(WrongArgumentException.class, iae.getMessage(), getExceptionInterceptor());
            }
        }
    }

    if (canonicalTimezone != null && canonicalTimezone.length() > 0) {
        //设置时区，时间错位的源头
        this.serverSession.setServerTimeZone(
            TimeZone.getTimeZone(canonicalTimezone));
        // The Calendar class has the behavior of mapping unknown timezones to 'GMT' instead of throwing an exception, so we must check for this...
        //时区不规范，比如不是GMT，然而ID标识GMT
        if (!canonicalTimezone.equalsIgnoreCase("GMT") && this.serverSession.getServerTimeZone().getID().equals("GMT")) {
            throw ExceptionFactory.createException(WrongArgumentException.class, Messages.getString("Connection.9", new Object[] {
                    canonicalTimezone
                }),
                getExceptionInterceptor());
        }
    }
}
```
规范时区：
```java

/**
* Returns the 'official' Java timezone name for the given timezone
* 
* @param timezoneStr
*            the 'common' timezone name
* @param exceptionInterceptor
*            exception interceptor
* 
* @return the Java timezone name for the given timezone
*/
public static String getCanonicalTimezone(String timezoneStr, ExceptionInterceptor exceptionInterceptor) {
   if (timezoneStr == null) {
       return null;
   }

   timezoneStr = timezoneStr.trim();
   // handle '+/-hh:mm' form ...
   //顾名思义
   if (timezoneStr.length() > 2) {
       if ((timezoneStr.charAt(0) == '+' || timezoneStr.charAt(0) == '-') && Character.isDigit(timezoneStr.charAt(1))) {
           return "GMT" + timezoneStr;
       }
   }

   synchronized(TimeUtil.class) {
       if (timeZoneMappings == null) {
           loadTimeZoneMappings(exceptionInterceptor);
       }
   }

   String canonicalTz;
   //时区缓存去找关键字
   if ((canonicalTz = timeZoneMappings.getProperty(timezoneStr)) != null) {
       return canonicalTz;
   }

   throw ExceptionFactory.createException(InvalidConnectionAttributeException.class,
       Messages.getString("TimeUtil.UnrecognizedTimezoneId", new Object[] {
           timezoneStr
       }), exceptionInterceptor);
}
```
比如数据库时区是CST，拿到了：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756966394-e09412a2-3cd4-43b4-823d-da91de183ffc.png#averageHue=%23373d3e&clientId=u87bba4a2-d53b-4&from=paste&height=146&id=ub83d5dc8&originHeight=366&originWidth=1546&originalType=binary&ratio=1&rotation=0&showTitle=false&size=148951&status=done&style=shadow&taskId=ua00e922f-29c8-471a-8f44-af545a02a0f&title=&width=618.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756980203-82965eb5-cd4f-4482-b3b6-8491adc91647.png#averageHue=%230a0a0a&clientId=u87bba4a2-d53b-4&from=paste&height=265&id=uc0d16421&originHeight=662&originWidth=1564&originalType=binary&ratio=1&rotation=0&showTitle=false&size=143858&status=done&style=shadow&taskId=ubf58513b-f804-45fa-aa2d-c966ed41cec&title=&width=625.6)<br />这是系统时区，拿到的是 CST，根源是读取了内置的时区值：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661756991828-ca549380-5986-45fd-ae44-02ffc89965b7.png#averageHue=%23232323&clientId=u87bba4a2-d53b-4&from=paste&id=u44648ef1&originHeight=492&originWidth=1628&originalType=binary&ratio=1&rotation=0&showTitle=false&size=218165&status=done&style=shadow&taskId=ub2975f87-60c0-4c86-a9eb-232c535448d&title=)<br />然而这个文件没有 CST 时区定义，需要去 JDK 去拿，然后缓存。这就说明一个道理，CST 这个时区定义不明确。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757007271-7e862b03-783e-40c1-a149-6e810dca095c.png#averageHue=%23527047&clientId=u87bba4a2-d53b-4&from=paste&id=u4a92e574&originHeight=593&originWidth=1600&originalType=binary&ratio=1&rotation=0&showTitle=false&size=294674&status=done&style=shadow&taskId=uef1b95f4-4bf7-4ca9-8eaf-a9e84e7ee65&title=)<br />时区就是 CST 了，仅仅是 CST 时区而已，这里并不能说明 CST 有什么问题，真正的问题是 CST 怎么比东八区少 13 个小时呢？
```java
this.serverSession.setServerTimeZone(TimeZone.getTimeZone(canonicalTimezone));
```
根源就是这几句代码：
```java
public static TimeZone getTimeZone(String var0) {
    return ZoneInfoFile.getZoneInfo(var0);
}
```
开始初始化，sun.timezone.ids.oldmapping 这个一般不会设置。<br />读取 $JAVA_HOME/lib/tzdb.dat，这是一个 JDK 时区存储文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757044597-1d78a708-b969-432a-bc8e-51348d8cf8db.png#averageHue=%234c4835&clientId=u87bba4a2-d53b-4&from=paste&id=u44faa727&originHeight=412&originWidth=1647&originalType=binary&ratio=1&rotation=0&showTitle=false&size=178641&status=done&style=shadow&taskId=u770e655d-bb8e-4bdd-888a-2a10e4f762e&title=)<br />其中 PRC 就是中国时区，但是这个文件并未定义 CST。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757062824-901f1cd5-f732-4452-8349-8cf61edcbc21.png#averageHue=%23425777&clientId=u87bba4a2-d53b-4&from=paste&height=131&id=u40bf1915&originHeight=327&originWidth=1500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=109605&status=done&style=shadow&taskId=u3311a701-b3c3-467a-bbba-34154a4f388&title=&width=600)<br />CST在这里定义的：`addOldMapping();`
```java
private static void addOldMapping() {
    String[][] var0 = oldMappings;
    int var1 = var0.length;

    for (int var2 = 0; var2 < var1; ++var2) {
        String[] var3 = var0[var2];
        //这里就把CST时区设置为芝加哥时区
        aliases.put(var3[0], var3[1]);
    }

    if (USE_OLDMAPPING) {
        aliases.put("EST", "America/New_York");
        aliases.put("MST", "America/Denver");
        aliases.put("HST", "Pacific/Honolulu");
    } else {
        zones.put("EST", new ZoneInfo("EST", -18000000));
        zones.put("MST", new ZoneInfo("MST", -25200000));
        zones.put("HST", new ZoneInfo("HST", -36000000));
    }
}
```
`oldMappings` 是啥呢？
```java
private static String[][] oldMappings = new String[][] {
    {
        "ACT",
        "Australia/Darwin"
    }, {
        "AET",
        "Australia/Sydney"
    }, {
        "AGT",
        "America/Argentina/Buenos_Aires"
    }, {
        "ART",
        "Africa/Cairo"
    }, {
        "AST",
        "America/Anchorage"
    }, {
        "BET",
        "America/Sao_Paulo"
    }, {
        "BST",
        "Asia/Dhaka"
    }, {
        "CAT",
        "Africa/Harare"
    }, {
        "CNT",
        "America/St_Johns"
    }, {
        "CST",
        "America/Chicago"
    }, {
        "CTT",
        "Asia/Shanghai"
    }, {
        "EAT",
        "Africa/Addis_Ababa"
    }, {
        "ECT",
        "Europe/Paris"
    }, {
        "IET",
        "America/Indiana/Indianapolis"
    }, {
        "IST",
        "Asia/Kolkata"
    }, {
        "JST",
        "Asia/Tokyo"
    }, {
        "MIT",
        "Pacific/Apia"
    }, {
        "NET",
        "Asia/Yerevan"
    }, {
        "NST",
        "Pacific/Auckland"
    }, {
        "PLT",
        "Asia/Karachi"
    }, {
        "PNT",
        "America/Phoenix"
    }, {
        "PRT",
        "America/Puerto_Rico"
    }, {
        "PST",
        "America/Los_Angeles"
    }, {
        "SST",
        "Pacific/Guadalcanal"
    }, {
        "VST",
        "Asia/Ho_Chi_Minh"
    }
};
```
{"CST", "America/Chicago"}    😭<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757120567-50a5f997-1efa-45c3-b48f-72834d0b6524.png#averageHue=%23232322&clientId=u87bba4a2-d53b-4&from=paste&id=u6ff8cc8b&originHeight=330&originWidth=1581&originalType=binary&ratio=1&rotation=0&showTitle=false&size=129926&status=done&style=shadow&taskId=u18f511a9-2a2b-4d23-bc9b-bd852fcaa58&title=)
```java
private static ZoneInfo getZoneInfo0(String var0) {
    try {
        //缓存获取
        ZoneInfo var1 = (ZoneInfo) zones.get(var0);
        if (var1 != null) {
            return var1;
        } else {
            String var2 = var0;
            if (aliases.containsKey(var0)) {
                var2 = (String) aliases.get(var0);
            }

            int var3 = Arrays.binarySearch(regions, var2);
            if (var3 < 0) {
                return null;
            } else {
                byte[] var4 = ruleArray[indices[var3]];
                DataInputStream var5 = new DataInputStream(new ByteArrayInputStream(var4));
                var1 = getZoneInfo(var5, var2);
                //首次获取，存缓存
                zones.put(var0, var1);
                return var1;
            }
        }
    } catch (Exception var6) {
        throw new RuntimeException("Invalid binary time-zone data: TZDB:" + var0 + ", version: " + versionId, var6);
    }
}
```
就这样 CST 时区就被 JDK 认为是美国芝加哥的时区了，😖
<a name="OCpoU"></a>
### 2.2 时区设置
那么 JDBC 在哪里设置时间的呢?<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757158071-a2448cf2-7bf5-4487-828d-a23fe320e0c7.png#averageHue=%232a2724&clientId=u87bba4a2-d53b-4&from=paste&id=u08ba1709&originHeight=992&originWidth=1579&originalType=binary&ratio=1&rotation=0&showTitle=false&size=433123&status=done&style=shadow&taskId=u8467ed18-263e-43e5-b6dc-45fd61bc11a&title=)<br />进一步可以看到在服务器上时区都是 OK 的。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757176668-df40e3d7-aa42-4a2b-b2c3-790814708a52.png#averageHue=%233b3f3b&clientId=u87bba4a2-d53b-4&from=paste&id=u2a84d04d&originHeight=507&originWidth=1636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=278381&status=done&style=shadow&taskId=u9102d0d1-5adf-4c61-8dae-5f5b99564d6&title=)<br />但是，在 `com.mysql.cj.ClientPreparedQueryBindings` 的 `setTimestamp` 方法中，获取了 session 时区，然后 format，😅<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757207075-62e5ccf8-96f8-449a-8b25-f432e51b152e.png#averageHue=%23383e3f&clientId=u87bba4a2-d53b-4&from=paste&id=uaa9950f7&originHeight=427&originWidth=1634&originalType=binary&ratio=1&rotation=0&showTitle=false&size=261965&status=done&style=shadow&taskId=u9dc6f284-8bab-494f-82f4-50b58024281&title=)<br />时间从此丢失 13 小时，原因是 format 的锅，因为用的美国芝加哥时间格式化，如果使用 long 时间的话或者什么都不处理就没有问题。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757232063-f72e4c2c-7e55-4fe6-a47b-4221ecfdf08b.png#averageHue=%23684d2f&clientId=u87bba4a2-d53b-4&from=paste&id=u3845590b&originHeight=249&originWidth=1630&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136295&status=done&style=shadow&taskId=ue164df8d-eb26-4148-a04a-914393e51cb&title=)<br />SimpleDateFormat 设置 CST 时区，前面已经分析了，这个时区就是美国芝加哥时区。<br />JDK 会认为 CST 是美国芝加哥的时区，UTC-5，但是我们的时间是 UTC+8，换算成 US的时间就是，当前时间 - 8 - 5，即时间少 13 小时。这里不设置时区（即使用客户端时区）即可正常返回时间。<br />那么 CST 时区是什么呢？笔者写博客的时间 是2021-09-22，是 CST 的夏令时。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757258731-16562c41-8d15-4cbf-bc22-87ee6e1096dd.png#averageHue=%23efddd6&clientId=u87bba4a2-d53b-4&from=paste&id=u5d1a644c&originHeight=362&originWidth=1540&originalType=binary&ratio=1&rotation=0&showTitle=false&size=154063&status=done&style=shadow&taskId=ue438b780-db93-48d0-9b73-66db3969196&title=)<br />CST 是中部标准时间，现在是 UTC-5，即夏令时，冬季还会变成 UTC-6。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757278706-bf0e1b2b-25dd-4fb1-934f-bb4f0fe498b6.png#averageHue=%23ccd8f1&clientId=u87bba4a2-d53b-4&from=paste&id=u753e4065&originHeight=316&originWidth=1613&originalType=binary&ratio=1&rotation=0&showTitle=false&size=125584&status=done&style=shadow&taskId=u3b8b6005-8af8-4be8-ac27-69e3de4aeed&title=)<br />标准的 US 的 CST 时间是 UTC-6，当前的时间是 23:56。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757295848-ea5c042e-afdf-462d-9a2a-87f071afb647.png#averageHue=%23f4e2cd&clientId=u87bba4a2-d53b-4&from=paste&id=ub9d20548&originHeight=628&originWidth=1537&originalType=binary&ratio=1&rotation=0&showTitle=false&size=206700&status=done&style=shadow&taskId=u0158e9aa-9f16-4d8e-b7c3-4ed7fe1ddd2&title=)<br />关键在于 CST 定义非常模糊，而 MySQL 驱动调用 `SimpleDateFormat`，使用的 CST 为美国芝加哥时区，当前的季节为 UTC-5。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757310889-44bfd9f1-650e-4ac5-ad8a-9c201a98f3c3.png#averageHue=%23f0f0f0&clientId=u87bba4a2-d53b-4&from=paste&id=ucada0e3f&originHeight=624&originWidth=1560&originalType=binary&ratio=1&rotation=0&showTitle=false&size=219809&status=done&style=shadow&taskId=ud32980fc-1e29-4179-b2f5-c5e9638f2ec&title=)
<a name="ydBP3"></a>
## 3、解决办法
根据上面的分析，解决 CST 时区的方法非常多。

- 设置 MySQL Server 的时区为非 CST 时区；
- 设置 MySQL 的系统时区为非 CST 时区； 
- 通过参数增加 `serverTimezone`设置为明确的 MySQL 驱动的 properties 定义的时区；
- 修改 MySQL Java 驱动，获取时区通过客户端获取，比如当前运行环境，通过 JDK 获取。
<a name="mgBxM"></a>
### 3.1 解决办法详细说明
<a name="rUi4O"></a>
#### 设置 MySQL Server 的时区
```sql
set global time_zone = '+08:00';
```
或者修改 MySQL 的配置文件 /etc/mysql/mysql.conf.d/mysqld.cnf。<br />[mysqld] 节点下增加：
```
default-time-zone = '+08:00'
```
<a name="jbO8p"></a>
#### 设置系统时区
以 Ubuntu 为例：
```bash
timedatectl set-timezone Asia/Shanghai
```
<a name="UDifP"></a>
#### 参数增加 `serverTimezone`
```
jdbc:mysql://localhost:3306/work?useUnicode=true&characterEncoding=utf-8&useSSL=true&serverTimezone=Asia/Shanghai
```
<a name="EsSWZ"></a>
#### 修改MySQL驱动
比如获取时区通过 client 端获取，Date 数据使用什么时区，就使用这个时区 format，但是一般而言不会自己发布驱动，跟随 MySQL 官方更新，只有大厂有机会自己运营 MySQL 驱动。
<a name="aAbvX"></a>
### 3.2 官方解决方案
在浏览 MySQL 8.0.x 驱动发布的时候在 8.0.23 版本发现了特别的发布记录，在初始时使用 8.0.22 版本是有深意的，😄
> _MySQL :: MySQL Connector/J 8.0 Release Notes :: Changes in MySQL Connector/J 8.0.23 (2021-01-18, General Availability)_

![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757397816-285946e3-19f0-4e81-9051-8a38576e2272.png#averageHue=%23f0f0f0&clientId=u87bba4a2-d53b-4&from=paste&id=u6c193efb&originHeight=193&originWidth=1641&originalType=binary&ratio=1&rotation=0&showTitle=false&size=193600&status=done&style=shadow&taskId=u1782c727-7fb5-4373-b080-8ebd6affea6&title=)<br />看来官方修复了。😄 <br />来源码看看。果然，不配置就客户端获取时区了`TimeZone.getDefault();`
```java
public void configureTimeZone() {
    //先读配置connectionTimeZone
    String connectionTimeZone = getPropertySet().getStringProperty(PropertyKey.connectionTimeZone).getValue();

    TimeZone selectedTz = null;
    //如果没配参数，或者参数配LOCAL，就取客户端时区
    //配置其他选择，基本上参数决定了时区，不再MySQL server去获取时区了
    if (connectionTimeZone == null || StringUtils.isEmptyOrWhitespaceOnly(connectionTimeZone) || "LOCAL".equals(connectionTimeZone)) {
        selectedTz = TimeZone.getDefault();

    } else if ("SERVER".equals(connectionTimeZone)) {
        // Session time zone will be detected after the first ServerSession.getSessionTimeZone() call.
        return;

    } else {
        selectedTz = TimeZone.getTimeZone(ZoneId.of(connectionTimeZone)); // TODO use ZoneId.of(String zoneId, Map<String, String> aliasMap) for custom abbreviations support
    }

    //设置时区
    this.serverSession.setSessionTimeZone(selectedTz);

    //默认不再强制把时区塞进session 的 Variables中
    if (getPropertySet().getBooleanProperty(PropertyKey.forceConnectionTimeZoneToSession).getValue()) {
        // TODO don't send 'SET SESSION time_zone' if time_zone is already equal to the selectedTz (but it requires time zone detection)
        StringBuilder query = new StringBuilder("SET SESSION time_zone='");

        ZoneId zid = selectedTz.toZoneId().normalized();
        if (zid instanceof ZoneOffset) {
            String offsetStr = ((ZoneOffset) zid).getId().replace("Z", "+00:00");
            query.append(offsetStr);
            this.serverSession.getServerVariables().put("time_zone", offsetStr);
        } else {
            query.append(selectedTz.getID());
            this.serverSession.getServerVariables().put("time_zone", selectedTz.getID());
        }

        query.append("'");
        sendCommand(this.commandBuilder.buildComQuery(null, query.toString()), false, 0);
    }
}
```
再看看设置参数的地方，这里设计有点改变，通过 `QueryBindings` 接口抽象了处理逻辑：
```java
public void setTimestamp(int parameterIndex, Timestamp x) throws java.sql.SQLException {
    synchronized(checkClosed().getConnectionMutex()) {
        ((PreparedQuery << ? > ) this.query).getQueryBindings().setTimestamp(getCoreParameterIndex(parameterIndex), x, MysqlType.TIMESTAMP);
    }
}
```
实现 `com.mysql.cj.ClientPreparedQueryBindings`：
```java
public void bindTimestamp(int parameterIndex, Timestamp x, Calendar targetCalendar, int fractionalLength, MysqlType targetMysqlType) {
   if (fractionalLength < 0) {
       // default to 6 fractional positions
       fractionalLength = 6;
   }

   x = TimeUtil.adjustNanosPrecision(x, fractionalLength, !this.session.getServerSession().isServerTruncatesFracSecs());

   StringBuffer buf = new StringBuffer();

   if (targetCalendar != null) {
       buf.append(TimeUtil.getSimpleDateFormat("''yyyy-MM-dd HH:mm:ss", targetCalendar).format(x));
   } else {
       this.tsdf = TimeUtil.getSimpleDateFormat(this.tsdf, "''yyyy-MM-dd HH:mm:ss",
           targetMysqlType == MysqlType.TIMESTAMP && this.preserveInstants.getValue() ? this.session.getServerSession().getSessionTimeZone() :
           this.session.getServerSession().getDefaultTimeZone());
       buf.append(this.tsdf.format(x));
   }

   if (this.session.getServerSession().getCapabilities().serverSupportsFracSecs() && x.getNanos() > 0) {
       buf.append('.');
       buf.append(TimeUtil.formatNanos(x.getNanos(), 6));
   }
   buf.append('\'');

   setValue(parameterIndex, buf.toString(), targetMysqlType);
}
```
时区就是刚刚设置的，亚洲/上海。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757477527-e56bf172-e0ef-423a-9f71-82163482daf0.png#averageHue=%23363e42&clientId=u87bba4a2-d53b-4&from=paste&id=ufe7b1757&originHeight=558&originWidth=1592&originalType=binary&ratio=1&rotation=0&showTitle=false&size=236999&status=done&style=shadow&taskId=ud8e68cb4-9eb4-4822-86b5-21635ab65a3&title=)
<a name="DG02F"></a>
## 总结
一个时区问题，居然里面有这么多玩头，MySQL 居然在 8.0.23 才修复这个，难道 MySQL 认为大家都会配置时区，还是服务器都不使用 CST 时区。另外如果使用 UTC 时区，是一个精准的时区，表示 0 区时间，就会从一个坑跳另一个坑。所以，还是精准用 Asia/Shanghai 吧，或者驱动升级 8.0.23 及以上版本，不配置时区。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661757496779-448af800-e9da-438a-9c93-60d2f6c58ec4.png#averageHue=%23999a9a&clientId=u87bba4a2-d53b-4&from=paste&id=ubf6449e1&originHeight=521&originWidth=1570&originalType=binary&ratio=1&rotation=0&showTitle=false&size=117399&status=done&style=shadow&taskId=u55953236-8787-40cf-9685-b54703aeb77&title=)
