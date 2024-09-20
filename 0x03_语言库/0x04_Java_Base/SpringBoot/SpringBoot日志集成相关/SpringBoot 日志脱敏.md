JavaSpringBoot
<a name="b6FHX"></a>
## 前言
在书写代码的时候，会书写许多日志代码，但是有些敏感数据是需要进行安全脱敏处理的。<br />对于日志脱敏的方式有很多，常见的有①使用`conversionRule`标签，继承`MessageConverter`②书写一个脱敏工具类，在打印日志的时候对特定特字段进行脱敏返回。<br />两种方式各有优缺点：

- 第一种方式需要修改代码，不符合开闭原则。
- 第二种方式，需要在日志方法的参数进行脱敏，对原生日志有入侵行为。
<a name="CliRH"></a>
## 自定义脱敏组件（slf4j+logback）
一个项目在书写了很多打印日志的代码，但是后面有了脱敏需求，如果去手动改动代码，会花费大量时间。如果引入本组件，完成配置即可轻松完成脱敏。（仅需三步可轻松配置）
<a name="b29oe"></a>
### 一、自定义脱敏组件 - 脱敏效果演示
![2022-12-03-20-25-03.967488400.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670070367858-0354676a-cd5d-4965-9c52-ad5bf59cbfe6.png#averageHue=%232c2b2b&clientId=u871a7a32-2dac-4&from=ui&id=u18847ccc&originHeight=523&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55009&status=done&style=none&taskId=u26c630f7-c4f4-4911-8657-fdd692b73c2&title=)![2022-12-03-20-25-04.147272800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670070367827-46274a70-9c53-4780-a65a-5b83a741848a.png#averageHue=%232c2c2a&clientId=u871a7a32-2dac-4&from=ui&id=ud925da30&originHeight=263&originWidth=837&originalType=binary&ratio=1&rotation=0&showTitle=false&size=36816&status=done&style=none&taskId=u26a2f970-dc61-4372-8624-3e2d4a55389&title=)
<a name="xNa1s"></a>
### 二、自定义脱敏组件 - 使用方式
<a name="zKBw9"></a>
#### 1、引入Jar包依赖
前提是将Jar包打入本地仓库，Jar包地址见后文。
```xml
<dependency>
  <groupId>pers.liuchengyin</groupId>
  <artifactId>logback-desensitization</artifactId>
  <version>1.0.0</version>
</dependency>
```
<a name="Xhj3Z"></a>
#### 2、替换日志文件配置类(logback.xml)
日志打印方式都只需要替换成脱敏的类即可，如果业务不需要，则无需替换。
<a name="f7UTQ"></a>
##### ①`ConsoleAppender` - 控制台脱敏
```
// 原类
ch.qos.logback.core.ConsoleAppender
// 替换类
pers.liuchengyin.logbackadvice.LcyConsoleAppender
```
<a name="FqTG2"></a>
##### ②`RollingFileAppender` - 滚动文件
```
// 原类
ch.qos.logback.core.rolling.RollingFileAppender
// 替换类
pers.liuchengyin.logbackadvice.LcyRollingFileAppender
```
<a name="Ta93S"></a>
##### ③`FileAppender` - 文件
```
// 原类
ch.qos.logback.core.FileAppender
// 替换类
pers.liuchengyin.logbackadvice.LcyFileAppender
```
<a name="PgiCO"></a>
##### 替换示例：
```xml
<property name="CONSOLE_LOG_PATTERN"
  value="%yellow(%date{yyyy-MM-dd HH:mm:ss}) |%highlight(%-5level) |%blue(%thread) |%blue(%file:%line) |%green(%logger) |%cyan(%msg%n)"/>

<!-- ConsoleAppender 控制台输出日志 -->
<appender name="CONSOLE" class="pers.liuchengyin.logbackadvice.LcyConsoleAppender">
  <encoder>
    <pattern>
      ${CONSOLE_LOG_PATTERN}
    </pattern>
  </encoder>
</appender>
```
<a name="aEJkL"></a>
#### 3、添加脱敏配置文件(logback-desensitize.yml)
该配置文件应该放在resources文件下<br />![2022-12-03-20-25-04.632499300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670070476655-59243b35-8662-4c63-a439-32b76607f2db.png#averageHue=%233e4245&clientId=u871a7a32-2dac-4&from=ui&id=u6684611b&originHeight=148&originWidth=275&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5493&status=done&style=none&taskId=u160214c4-af2b-4a85-8d56-f10cf7ecaf8&title=)
<a name="QgmPt"></a>
### 三、自定义脱敏组件 - 脱敏规范
<a name="yiHQ6"></a>
#### 1、支持数据类型
八大基本类型及其包装类型、Map、List、业务里的Pojo对象、List<业务里的Pojo对象>、JSON字符串。
> 注：在配置文件中配置的时候，只需要配置对象里的属性值就行。

<a name="irbgH"></a>
#### 2、不支持的数据类型
List<八大基本类型及包装类型>，因为不知道脱敏的数据源具体是哪一个。
<a name="nRCQY"></a>
#### 3、匹配规则
key + 分割符 + value，目前仅支持冒号(:)和等号(=)，示例如下：
```java
log.info("your email:{}, your phone:{}", "123456789@qq.com","15310763497");
log.info("your email={}, your cellphone={}", "123456789@qq.com","15310763497");
```

- key：定义了对应需要脱敏的关键字，如上诉的email、phone等以及业务对象中的字段、Map中的Key、JSON中的Key
- value：需要脱敏的值，如上诉的123456789@qq.com、15310763497。
<a name="MGFW0"></a>
#### 4、日志规范
建议书写日志的时候尽量规范，对于key为中文的是没有办法脱敏的，规范程度可以见脱敏效果演示里的代码。
<a name="M7cT1"></a>
### 四、logback-desensitize.yml配置说明
```yaml
# 日志脱敏
log-desensitize:
  # 是否忽略大小写匹配，默认为true
  ignore: true
  # 是否开启脱敏，默认为false
  open: true
  # pattern下的key/value为固定脱敏规则
  pattern:
    # 邮箱 - @前第4-7位脱敏
    email: "@>(4,7)"
    # qq邮箱 - @后1-3位脱敏
    qqemail: "@<(1,3)"
    # 姓名 - 姓脱敏，如*杰伦
    name: 1,1
    # 密码 - 所有需要完全脱敏的都可以使用内置的password
    password: password
  patterns:
    # 身份证号，key后面的字段都可以匹配以下规则(用逗号分隔)
    - key: identity,idcard
      # 定义规则的标识
      custom:
        # defaultRegex表示使用组件内置的规则：identity表示身份证号 - 内置的18/15位
        - defaultRegex: identity
          position: 9,13
        # 内置的other表示如果其他规则都无法匹配到，则按该规则处理
        - defaultRegex: other
          position: 9,10
    # 电话号码，key后面的字段都可以匹配以下规则(用逗号分隔)
    - key: phone,cellphone,mobile
      custom:
        # 手机号 - 内置的11位手机匹配规则
        - defaultRegex: phone
          position: 4,7
        # 自定义正则匹配表达式：座机号(带区号，号码七位|八位)
        - customRegex: "^0[0-9]{2,3}-[0-9]{7,8}"
        # -后面的1-4位脱敏
          position: "-<(1,4)"
        # 自定义正则匹配表达式：座机号(不带区号)
        - customRegex: "^[0-9]{7,8}"
          position: 3,5
        # 内置的other表示如果其他规则都无法匹配到，则按该规则处理
        - defaultRegex: other
          position: 1,3
    # 这种方式不太推荐 - 一旦匹配不上，就不会脱敏
    - key: localMobile
      custom:
          customRegex: "^0[0-9]{2,3}-[0-9]{7,8}"
          position: 1,3
```
上面这个配置是相对完整的，一定要严格遵守层级配置格式。
<a name="bntYi"></a>
#### 自定义脱敏支持的方式
<a name="zRVFL"></a>
##### 1、key:value的方式

- phone：4,7，表示phone属性的4-7位进行脱敏
- 原始数据：13610357861
- 脱敏后：136****7861
<a name="bzS8V"></a>
##### 2、以符号作为起始、结束节点作为脱敏标志
`emai:"@>(4,7)"`，@为脱敏标志，>表示其为结束节点，<表示其为开始节点。即@>表示对@之前的进行脱敏，@<表示对@之后的进行脱敏。这个示例就是@前的数据的第4-7位进行脱敏。<br />注意：这种规则里的双引号、括号不能省略，其次:和=不能作为标志符号，因为和匹配规则有冲突

- 原始数据：123456789@qq.com
- "@>(4,7)"脱敏后：123****89@qq.com
- "@<(1,3)"脱敏后：123456789@***com
<a name="TLaLx"></a>
##### 3、自定义正则脱敏
```yaml
patterns:
  # 手机号
  - key: phone,mobile
    custom:
      # 手机号的正则
      - customRegex: "^1[0-9]{10}"
        # 脱敏范围
        position: 4,7
```
customRegex：正则表达式，如果符合该表达式，则使用其对应的脱敏规则(position)
<a name="Bq1fF"></a>
##### 4、一个字段，根据多种值含义进行自定义脱敏
比如说，username字段的值可以是手机号、也可以是邮箱，这个值动态改变的，前面几种方式都没办法解决，可以使用该方式。
```yaml
patterns:
  - key: username
    custom:
      # 手机号 - 11位
      - defaultRegex: phone
        position : 4,7
      # 邮箱 - @
   - defaultRegex: email
     position : "@>(3,12)"
   # 身份证 - 15/18位
   - defaultRegex: identity
     position : 1,3
   # 自定义正则
   - customRegex: "^1[0-9]{10}"
     position : 1,3
   # 都匹配不到时，按照这种规则来
   - defaultRegex: other
     position : 1,3
```
> 注意：上面示例中匹配规则里的 双引号和括号 都不能省略

该组件内置四种匹配规则：手机号、身份证号、邮箱、other(其他匹配不到时用的)，内置一种脱敏方式：password，表示完全脱敏，可用于pattren下的。<br />注：当pattern和patterns下的key有重复的时候，只会使用pattern下指定的方式进行脱敏。<br />Jar包地址和源码地址：<br />[https://github.com/liuchengyin01/LogbackDesensitization/tree/master/repo/pers/liuchengyin/logback-desensitization/1.0.0](https://github.com/liuchengyin01/LogbackDesensitization/tree/master/repo/pers/liuchengyin/logback-desensitization/1.0.0)<br />![2022-12-03-20-25-05.042452200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1670070681151-91fdeff3-98a1-4a74-b066-70a80c18a06a.png#averageHue=%23fefcfb&clientId=u871a7a32-2dac-4&from=ui&id=ub6e972bd&originHeight=401&originWidth=749&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30741&status=done&style=none&taskId=uc623009a-dde1-42e2-8e57-17b18087f20&title=)<br />Github地址：[https://github.com/liuchengyin01/LogbackDesensitization](https://github.com/liuchengyin01/LogbackDesensitization)
<a name="SAHGV"></a>
### Jar包打入Maven本地仓库的方式
1、下载Jar包，放在一个文件夹里<br />2、在这个文件夹里打开cmd(打开cmd，进入到这个文件夹)<br />3、执行命令(前提保证maven配置正常，使用`mvn -v`命令查看是否正常，如果显示版本号表示正常)
```bash
mvn install:install-file -DgroupId=pers.liuchengyin -DartifactId=logback-desensitization -Dversion=1.0.0 -Dpackaging=jar -Dfile=logback-desensitization-1.0.0.jar
```
命令说明：
```
-DgroupId
 表示jar对应的groupId  
 <groupId>pers.liuchengyin</groupId>
-DartifactId:
 表示jar对应的artifactId
 <artifactId>logback-desensitization</artifactId>
-Dversion
 表示jar对应的 version
 <version>1.0.0</version>
```
