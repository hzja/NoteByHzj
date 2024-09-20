Java 日志
<a name="VilCY"></a>
## 一、什么是RollingFile
`RollingFileAppender`是Log4j2中的一种能够实现日志文件滚动更新(`rollover`)的`Appender`。<br />`rollover`的意思是当满足一定条件(如文件达到了指定的大小，达到了指定的时间)后，就重命名原日志文件进行归档，并生成新的日志文件用于log写入。如果还设置了一定时间内允许归档的日志文件的最大数量，将对过旧的日志文件进行删除操作。<br />`RollingFile`实现日志文件滚动更新，依赖于`TriggeringPolicy`和`RolloverStrategy`。<br />其中，`TriggeringPolicy`为触发策略，其决定了何时触发日志文件的`rollover`，即When。<br />`RolloverStrategy`为滚动更新策略，其决定了当触发了日志文件的`rollover`时，如何进行文件的`rollover`，即How。<br />Log4j2提供了默认的`rollover`策略`DefaultRolloverStrategy`。<br />下面通过一个log4j2.xml文件配置简单了解`RollingFile`的配置。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log"
                 filePattern="logs/app-%d{yyyy-MM-dd HH}.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <TimeBasedTriggeringPolicy interval="1"/>
        <SizeBasedTriggeringPolicy size="250MB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```
上述配置文件中配置了一个`RollingFile`，日志写入logs/app.log文件中，每经过1小时或者当文件大小到达250M时，按照app-2017-08-01 12.log的格式对app.log进行重命名并归档，并生成新的文件用于写入log。<br />其中，`fileName`指定日志文件的位置和文件名称（如果文件或文件所在的目录不存在，会创建文件。）<br />`filePattern`指定触发`rollover`时，文件的重命名规则。`filePattern`中可以指定类似于`SimpleDateFormat`中的`date/time pattern`，如yyyy-MM-dd HH，或者%i指定一个整数计数器。

- `TimeBasedTriggeringPolicy`指定了基于时间的触发策略。
- `SizeBasedTriggeringPolicy`指定了基于文件大小的触发策略。
<a name="nVlNp"></a>
## 二、`TriggeringPolicy`
RollingFile的触发rollover的策略有

- `CronTriggeringPolicy`(Cron表达式触发)
- `OnStartupTriggeringPolicy`(JVM启动时触发)
- `SizeBasedTriggeringPolicy`(基于文件大小)
- `TimeBasedTriggeringPolicy`(基于时间)
- `CompositeTriggeringPolicy`(多个触发策略的混合，如同时基于文件大小和时间)

其中，`SizeBasedTriggeringPolicy`(基于日志文件大小)、`TimeBasedTriggeringPolicy`(基于时间)或同时基于文件大小和时间的混合触发策略最常用。
<a name="J8JCl"></a>
### `SizeBasedTriggeringPolicy`
`SizeBasedTriggeringPolicy`规定了当日志文件达到了指定的`size`时，触发`rollover`操作。`size`参数可以用KB、MB、GB等做后缀来指定具体的字节数，如20MB。
```xml
<SizeBasedTriggeringPolicy size="250MB"/>
```
<a name="RTQHK"></a>
### `TimeBasedTriggeringPolicy`
`TimeBasedTriggeringPolicy`规定了当日志文件名中的`date/time pattern`不再符合`filePattern`中的`date/time pattern`时，触发`rollover`操作。<br />比如，`filePattern`指定文件重命名规则为`app-%d{yyyy-MM-dd HH}.log`，文件名为`app-2017-08-25 11.log`，当时间达到2017年8月25日中午12点（2017-08-25 12），将触发`rollover`操作。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623582653533-1a1955c0-b147-47e3-8214-b792bed64ac7.png#averageHue=%23f5f2f0&clientId=u6d3d8767-006f-4&from=paste&id=ubfac896a&originHeight=317&originWidth=846&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=uecd210d6-4a95-4627-bc19-3903d86b8a9&title=)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Appenders>
    <RollingFile name="RollingFile" fileName="logs/app.log"
                 filePattern="logs/app-%d{yyyy-MM-dd HH}-%i.log">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
      </PatternLayout>
      <Policies>
        <TimeBasedTriggeringPolicy />
        <SizeBasedTriggeringPolicy size="250 MB"/>
      </Policies>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```
上述配置文件中，`filePattern`中yyyy-MM-dd HH最小时间粒度为小时，`TimeBasedTriggeringPolicy`中`interval`使用默认值1，将每1小时触发一次`rollover`。<br />若将`filePattern`改为`filePattern=“logs/app-%d{yyyy-MM-dd HH-mm}-%i.log”，yyyy-MM-dd HH-mm`最小时间粒度为分钟，将每1分钟触发一次`rollover`。
<a name="cvEXQ"></a>
### `CompositeTriggeringPolicy`
将多个`TriggeringPolicy`放到`Policies`中表示使用复合策略
```xml
<Policies>
    <TimeBasedTriggeringPolicy />
    <SizeBasedTriggeringPolicy size="250 MB"/>
</Policies>
```
如上，同时使用了`TimeBasedTriggeringPolicy`、`SizeBasedTriggeringPolicy`，有一个条件满足，就会触发`rollover`。
<a name="qDJja"></a>
## 三、`DefaultRolloverStrategy`
`DefaultRolloverStrategy`指定了当触发`rollover`时的默认策略。<br />`DefaultRolloverStrategy`是Log4j2提供的默认的`rollover`策略，即使在log4j2.xml中没有显式指明，也相当于为`RollingFile`配置下添加了如下语句。`DefaultRolloverStrategy`默认的max为7。
```xml
<DefaultRolloverStrategy max="7"/>
```
`max`参数指定了计数器的最大值。一旦计数器达到了最大值，过旧的文件将被删除。
:::tips
注意：不要认为max参数是需要保留的日志文件的最大数目。
:::
`max`参数是与`filePattern`中的计数器`%i`配合起作用的，其具体作用方式与`filePattern`的配置密切相关。

1. 如果`filePattern`中仅含有`date/time pattern`，每次`rollover`时，将用当前的日期和时间替换文件中的日期格式对文件进行重命名。max参数将不起作用。

如，`filePattern="logs/app-%d{yyyy-MM-dd}.log"`

2. 如果`filePattern`中仅含有整数计数器（即%i），每次`rollover`时，文件重命名时的计数器将每次加1（初始值为1），若达到`max`的值，将删除旧的文件。

如，`filePattern="logs/app-%i.log"`

3. 如果`filePattern`中既含有`date/time pattern`，又含有`%i`，每次`rollover`时，计数器将每次加1，若达到max的值，将删除旧的文件，直到`data/time pattern`不再符合，被替换为当前的日期和时间，计数器再从1开始。

如，`filePattern="logs/app-%d{yyyy-MM-dd HH-mm}-%i.log"`<br />假设`fileName`为`logs/app.log`，`SizeBasedTriggeringPolicy`的size为10KB，`DefaultRolloverStrategy`的max为3。<br />根据`filePattern`配置的不同分为以下几种情况：
<a name="sxgsM"></a>
### 情况1：`filePattern`中仅含有`date/time pattern`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <RollingFile name="RollingFile" fileName="logs/app.log"
        filePattern="logs/app-%d{yyyy-MM-dd}.log">
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <SizeBasedTriggeringPolicy size="10KB"/>
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
`filePattern="logs/app-%d{yyyy-MM-dd}.log"`，指定当发生`rollover`时，将按照`app-%d{yyyy-MM-dd}.log`的格式对文件进行重命名。<br />每次触发`rollover`时，将按照如下方式对文件进行`rollover`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623582653470-5c151c86-3cdc-4911-a10d-6507bddb7239.png#averageHue=%23f7f6f4&clientId=u6d3d8767-006f-4&from=paste&id=uf1e3d69e&originHeight=333&originWidth=938&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=udb7aea52-ec7c-4c0d-ab88-64992fac9e8&title=)
<a name="FGO2M"></a>
### 情况2：`filePattern`中仅含有整数计数器（`%i`）
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <RollingFile name="RollingFile" fileName="logs/app.log"
        filePattern="logs/app-%i.log">
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <SizeBasedTriggeringPolicy size="10KB"/>
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
`filePattern="logs/app-%i.log"`，其余配置同上。<br />每次触发`rollover`时，将按照如下方式对文件进行`rollover`。<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623582653518-83b4a452-58b5-4506-a145-280feb754472.png#averageHue=%23faf9f8&clientId=u6d3d8767-006f-4&from=paste&id=u8decce2d&originHeight=613&originWidth=1080&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=shadow&taskId=u698cdc67-7699-400e-870a-9dea6fe4361&title=)
<a name="ZUpMX"></a>
### 情况3：如果`filePattern`中既含有`date/time pattern`，又含有`%i`计数器
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <RollingFile name="RollingFile" fileName="logs/app.log"
        filePattern="logs/app-%d{yyyy-MM-dd HH-mm}-%i.log">
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <TimeBasedTriggeringPolicy />
                <SizeBasedTriggeringPolicy size="10KB"/>
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
`filePattern="logs/app-%d{yyyy-MM-dd HH-mm}-%i.log"`,同时指定了`TimeBasedTriggeringPolicy`和`SizeBasedTriggeringPolicy`的触发策略，每1分钟或者文件大小达到10KB，将触发`rollover`。<br />每次触发`rollover`时，将按照如下方式对文件进行`rollover`。<br />总结：

1. `max`参数是与`filePattern`中的计数器`%i`配合起作用的，若`filePattern为filePattern="logs/app-%d{yyyy-MM-dd}.log">`，由于没有设置`%i`计数器，`max`参数将不起作用。
2. `max`参数不是需要保留的文件的最大个数。如情况3，日志文件`date/time pattern`不再符合`filePattern`时，计算器将被重置为1，日志总个数超过了max的指定值。

可认为`max`参数规定了一定时间范围内归档文件的最大个数。
<a name="xOkTD"></a>
## 四、`DeleteAction`
`DefaultRolloverStrategy`制定了默认的`rollover`策略，通过`max`参数可控制一定时间范围内归档的日志文件的最大个数。<br />Log4j 2.5 引入了`DeleteAction`，使用户可以自己控制删除哪些文件，而不仅仅是通过`DefaultRolloverStrategy`的默认策略。<br />注意：通过`DeleteAction`可以删除任何文件，而不仅仅像`DefaultRolloverStrategy`那样，删除最旧的文件，所以使用的时候需要谨慎！
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn" name="MyApp" packages="">
  <Properties>
    <Property name="baseDir">logs</Property>
  </Properties>
  <Appenders>
    <RollingFile name="RollingFile" fileName="${baseDir}/app.log"
          filePattern="${baseDir}/app-%d{yyyy-MM-dd}.log.gz">
      <PatternLayout pattern="%d %p %c{1.} [%t] %m%n" />
      <CronTriggeringPolicy schedule="0 0 0 * * ?"/>
      <DefaultRolloverStrategy>
        <Delete basePath="${baseDir}" maxDepth="2">
          <IfFileName glob="*/app-*.log.gz" />
          <IfLastModified age="60d" />
        </Delete>
      </DefaultRolloverStrategy>
    </RollingFile>
  </Appenders>
  <Loggers>
    <Root level="error">
      <AppenderRef ref="RollingFile"/>
    </Root>
  </Loggers>
</Configuration>
```
上述配置文件中，`Delete`部分便是配置`DeleteAction`的删除策略，指定了当触发`rollover`时，删除`baseDir`文件夹或其子文件下面的文件名符合app-*.log.gz且距离最后的修改日期超过60天的文件。<br />其中，`basePath`指定了扫描开始路径，为`baseDir`文件夹。`maxDepth`指定了目录扫描深度，为2表示扫描`baseDir`文件夹及其子文件夹。<br />`IfFileName`指定了文件名需满足的条件，`IfLastModified`指定了文件修改时间需要满足的条件。<br />`DeleteAction`常用参数如下：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623582653482-13ef44b7-0bf9-4f4c-9fa4-a4812ba8c044.png#averageHue=%23f8f7f5&clientId=u6d3d8767-006f-4&from=paste&id=u1f416fd6&originHeight=858&originWidth=944&originalType=url&ratio=3&rotation=0&showTitle=false&status=done&style=none&taskId=ubf8c35eb-7243-4ae8-a658-f332d4b18cd&title=)
<a name="v41ml"></a>
## 五、程序测试demo
```java
public class HelloWorld {
 
    public static void main(String[] args) {
        Logger logger = LogManager.getLogger(LogManager.ROOT_LOGGER_NAME);
        try{
            //通过打印i，日志文件中数字越小代表越老
            for(int i = 0; i < 50000; i++) {
                logger.info("{}", i);
                logger.info("logger.info\n");
                Thread.sleep(100);//为了防止50000条很快跑完，sleep一段时间
            }
        } catch (InterruptedException e) {}
    }
}
```
<a name="HdC8N"></a>
### 1、测试基于时间触发
`filePattern`最小时间粒度为秒，将每5秒触发一次`rollover`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <!--<RollingFile name="RollingFile" fileName="logs/app.log"-->
                     <!--filePattern="logs/app-%d{yyyy-MM-dd HH}-%i.log">-->
        <RollingFile name="RollingFile" fileName="logs/app.log"
                     filePattern="logs/app-%d{yyyy-MM-dd HH-mm-ss}.log">
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <!--当经过了interval时间后，将根据filePattern对文件进行重命名，并生成新的文件用于日志写入-->
                <TimeBasedTriggeringPolicy interval="5"/>
                <!--当日志文件大小大于size时，将根据filepattern对文件进行重命名，并生成新的文件用于日志写入-->
                <!--<SizeBasedTriggeringPolicy size="30KB"/>-->
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
<a name="ZErOc"></a>
### 2、测试基于文件大小的触发
日志文件达到5KB，将触发`rollover`
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <!--<RollingFile name="RollingFile" fileName="logs/app.log"-->
                     <!--filePattern="logs/app-%d{yyyy-MM-dd HH}-%i.log">-->
        <RollingFile name="RollingFile" fileName="logs/app.log"
                     filePattern="logs/app-%d{yyyy-MM-dd HH-mm-ss}.log">
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <!--当经过了interval时间后，将根据filePattern对文件进行重命名，并生成新的文件用于日志写入-->
                <!--<TimeBasedTriggeringPolicy interval="5"/>-->
                <!--当日志文件大小大于size时，将根据filepattern对文件进行重命名，并生成新的文件用于日志写入-->
                <SizeBasedTriggeringPolicy size="5KB"/>
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
<a name="W9nQn"></a>
### 3、测试`DefaultRolloverStrategy`的`max`参数和`%i`计数器的搭配使用
注意`filePattern`最小时间粒度为分钟，且含`%i`计数器
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="trace" name="MyApp" packages="">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <RollingFile name="RollingFile" fileName="logs/app.log"
                     filePattern="logs/app-%d{yyyy-MM-dd HH-mm}-%i.log">
        <!--<RollingFile name="RollingFile" fileName="logs/app.log"-->
                     <!--filePattern="logs/app-%d{yyyy-MM-dd HH-mm-ss}.log">-->
            <PatternLayout>
                <Pattern>%d %p %c{1.} [%t] %m%n</Pattern>
            </PatternLayout>
            <Policies>
                <!--当经过了interval时间后，将根据filePattern对文件进行重命名，并生成新的文件用于日志写入-->
                <!--<TimeBasedTriggeringPolicy interval="5"/>-->
                <!--当日志文件大小大于size时，将根据filepattern对文件进行重命名，并生成新的文件用于日志写入-->
                <SizeBasedTriggeringPolicy size="5KB"/>
            </Policies>
            <DefaultRolloverStrategy max="3"/>
        </RollingFile>
    </Appenders>
    <Loggers>
        <Root level="trace">
            <AppenderRef ref="Console"/>
            <AppenderRef ref="RollingFile"/>
        </Root>
    </Loggers>
</Configuration>
```
