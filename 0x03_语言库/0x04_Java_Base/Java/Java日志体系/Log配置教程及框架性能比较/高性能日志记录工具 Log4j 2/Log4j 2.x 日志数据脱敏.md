Java<br />许多系统为了安全需要对敏感信息（如手机号、邮箱、姓名、身份证号、密码、卡号、住址等）的日志打印要求脱敏后才能输出，这里分享一种log4j日志脱敏方式。
<a name="ufzHF"></a>
### 自定义Layout
```java
import org.apache.logging.log4j.core.Layout;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.config.Node;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginAttribute;
import org.apache.logging.log4j.core.config.plugins.PluginElement;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;
import org.apache.logging.log4j.core.layout.AbstractStringLayout;
import org.apache.logging.log4j.core.layout.PatternLayout;
import org.apache.logging.log4j.core.pattern.RegexReplacement;

import java.nio.charset.Charset;

@Plugin(name = "MyPatternLayout", category = Node.CATEGORY, elementType = Layout.ELEMENT_TYPE, printObject = true)
public class MyPatternLayout extends AbstractStringLayout {
	private PatternLayout patternLayout;
	private Boolean sensitive;
	private RegexReplacement[] replaces;
	
	protected MyPatternLayout(Charset charset, String pattern, Boolean sensitive, RegexReplacement[] replaces) {
		super(charset);
		patternLayout = PatternLayout.newBuilder().withPattern(pattern).build();
		this.sensitive = sensitive;
		this.replaces = replaces;
	}
	
	/**
	 * 插件构造工厂方法
	 *
	 * @param pattern   输出pattern
	 * @param charset   字符集
	 * @param sensitive 是否开启脱敏
	 * @param replaces  脱敏规则
	 * @return Layout<String>
	 */
	@PluginFactory
	public static Layout<String> createLayout(@PluginAttribute(value = "pattern") final String pattern,
											  @PluginAttribute(value = "charset", defaultString = "UTF-8") final Charset charset,
											  @PluginAttribute(value = "sensitive") final Boolean sensitive,
											  @PluginElement("replace") final RegexReplacement[] replaces) {
		return new MyPatternLayout(charset, pattern, sensitive, replaces);
	}
	
	
	@Override
	public String toSerializable(LogEvent event) {
		// 原日志信息
		String msg = this.patternLayout.toSerializable(event);
		
		if (Boolean.FALSE.equals(this.sensitive)) {
			// 不脱敏，直接返回
			return msg;
		}
		
		if (this.replaces == null || this.replaces.length == 0) {
			throw new RuntimeException("未配置脱敏规则，请检查配置重试");
		}
		
		for (RegexReplacement replace : this.replaces) {
			// 遍历脱敏正则 & 替换敏感数据
			msg = replace.format(msg);
		}
		
		// 脱敏后的日志
		return msg;
	}
}
```
<a name="PybCk"></a>
### 编写log4j配置
以下预设了8中常见规则，请自行根据实际情况修改
```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
    <properties>
        <!-- 文件输出格式 -->
        <property name="PATTERN">%d{yyyy-MM-dd HH:mm:ss.SSS} %5level --- [%t] %c : %msg%n</property>
    </properties>

    <appenders>
        <!-- 日志打印到控制台Appender -->
        <Console name="CONSOLE" target="system_out">
            <MyPatternLayout pattern="${PATTERN}" sensitive="true">
                <replace>
                    <!-- 11位的手机号：保留前3后4 -->
                    <regex>
                        <![CDATA[
    (mobile|手机号)(=|=\[|\":\"|:|：|=')(1)([3-9]{2})(\d{4})(\d{4})(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2$3$4****$6$7</replacement>
                </replace>
                <replace>
                    <!-- 固定电话：XXXX-XXXXXXXX或XXX-XXXXXXXX，保留区号+前2后2 -->
                    <regex>
                        <![CDATA[
    (tel|座机)(=|=\[|\":\"|:|：|=')([\d]{3,4}-)(\d{2})(\d{4})(\d{2})(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2$3$4****$6$7</replacement>
                </replace>

                <replace>
                    <!-- 地址：汉字+字母+数字+下划线+中划线，留前3个汉字 -->
                    <regex>
                        <![CDATA[
    (地址|住址|address)(=|=\[|\":\"|:|：|=')([\u4e00-\u9fa5]{3})(\w|[\u4e00-\u9fa5]|-)*(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2$3****$5</replacement>
                </replace>


                <replace>
                    <!-- 19位的卡号，保留后4 -->
                    <regex>
                        <![CDATA[
    (cardNo|卡号)(=|=\[|\":\"|:|：|=')(\d{15})(\d{4})(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2***************$4$5</replacement>
                </replace>

                <replace>
                    <!-- 姓名,2-4汉字，留前1-->
                    <regex>
                        <![CDATA[
    (name|姓名)(=|=\[|\":\"|:|：|=')([\u4e00-\u9fa5]{1})([\u4e00-\u9fa5]{1,3})(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2$3**$5</replacement>
                </replace>

                <replace>
                    <!--  密码 6位数字，全* -->
                    <regex>
                        <![CDATA[
     (password|密码|验证码)(=|=\[|\":\"|:|：|=')(\d{6})(\]|\"|'|)
       ]]>
                    </regex>
                    <replacement>$1$2******$4</replacement>
                </replace>

                <replace>
                    <!-- 身份证，18位（结尾为数字或X、x），保留前1后1 -->
                    <regex>
                        <![CDATA[
       (身份证号|idCard)(=|=\[|\":\"|:|：|=')(\d{1})(\d{16})([\d|X|x]{1})(\]|\"|)
       ]]>
                    </regex>
                    <replacement>$1$2$3****************$5$6</replacement>
                </replace>

                <replace>
                    <!-- 邮箱，保留@前的前1后1 -->
                    <regex>
                        <![CDATA[
       (\w{1})(\w*)(\w{1})@(\w+).com
       ]]>
                    </regex>
                    <replacement>$1****$3@$4.com</replacement>
                </replace>
            </MyPatternLayout>
        </Console>
    </appenders>

    <loggers>
        <!-- 控制台输出 -->
        <root level="info">
            <AppenderRef ref="CONSOLE"/>
        </root>
    </loggers>

</configuration>
```
注意：

- **Console使用了自己写的的**`**MyPatternLayout**`**，**`**MyPatternLayout**`**的两个属性pattern和sensitive，对应类**`**MyPatternLayout**`**的插件工厂方法的入参**
- `**MyPatternLayout**`**节点的子节点**`**replace**`**（可多个）是配置的脱敏正则表达式**
<a name="wGdON"></a>
### 正则匹配说明
```xml
<replace>
	<!-- 11位的手机号：保留前3后4 -->
	<regex>
		<![CDATA[
(mobile|手机号|phoneNo)(=|=\[|\":\"|:|：|=')(1)([3-9]{2})(\d{4})(\d{4})(\]|\"|'|)
						]]>
	</regex>
	<replacement>$1$2$3$4****$6$7</replacement>
</replace>
```
regex说明

- `**(mobile|手机号|phoneNo)**`**：脱敏关键字，多个之间以英文|分隔**
- `**(=|=\[|\":\"|:|：|=')**`**：关键字后的符号，多个之间以英文|分隔，详见下文匹配说明**
- `**(1)**`**：匹配数字1**
- `**([3-9]{2})**`**：匹配2位数字，取值为3-9间的数字**
- `**(\d{4})**`**：匹配4位数字**
- `**(\d{4})**`**：匹配4位数字**
- `**(\]|\"|'|)**`**：匹配值后的其他字符**
```java
// 代码
logger.infoMessage("mobile={}", "13511114444");
```
```java
# 脱敏后
2021-11-16 11:02:08.767  INFO --- [main] log.test.LogTest : mobile=135****4444
```
分组匹配示意图（同颜色为对应关系）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1654738890355-f5f57927-2d04-4b09-beca-2071a71040ad.png#averageHue=%23294b34&clientId=ue55b8548-1f1f-4&from=paste&id=u3b643594&originHeight=284&originWidth=934&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2adab26d-9be6-46e0-954a-80a0ffc20c0&title=)<br />`replacement`中的`$n`即对应第n对括号（从1开始），上图中共有7对括号，`$1$2$3$4****$6$7`则表示，仅有第5组内容被****替代，其他内容按原内容显示
<a name="OEYtX"></a>
#### 注意事项

- **根据情况自行调整replace节点**
- **含脱敏关键字的正则，尽量列举全面**
- **值匹配正则（如上文的手机号的第3分组到倒数第2分组）：需要根据实际情况调整，特别是卡号、账号的规则，各家银行或有不同**
- **修改完配置后，务必进行测试，正则解析出错只有运行时可发现**
- **日志打印规范，根据第2分组**`**(=|=\[|\":\"|:|：|=')**`**可知，可匹配如下情况**
```java
@Test
public void test0() {
    // 等号
    logger.infoMessage("mobile={}", "13511114444");
    // 等号+[
    logger.infoMessage("mobile=[{}]", "13511114444");
    // 英文单引号+等号
    logger.infoMessage("mobile'='{}'", "13511114444");
    // 中文冒号
    logger.infoMessage("mobile：{}", "13511114444");
    // 英文冒号
    logger.infoMessage("mobile:{}", "13511114444");
    // 英文双引号+英文冒号
    logger.infoMessage("\"mobile\":\"{}\"", "13511114444");
}
```
```java
# 脱敏后
log.test.LogTest : mobile=135****4444
log.test.LogTest : mobile=[135****4444]
log.test.LogTest : mobile：135****4444
log.test.LogTest : mobile:135****4444
log.test.LogTest : 'mobile'='13511114444'
log.test.LogTest : "mobile":"135****4444"
```
对于不符合如上的情况，请调整代码或修改匹配正则
<a name="oHcTg"></a>
### 脱敏测试
普通字符串值直接输出
```java
@Test
public void test1() {
    //11位手机号
    logger.infoMessage("mobile={}", "13511114444");
    logger.infoMessage("mobile={},手机号：{}", "13511112222", "13511113333");
    logger.infoMessage("手机号：{}", "13511115555");
    //固定电话（带区号-）
    logger.infoMessage("tel：{},座机={}", "0791-83376222", "021-88331234");
    logger.infoMessage("tel：{}", "0791-83376222");
    logger.infoMessage("座机={}", "021-88331234");

    //地址
    logger.infoMessage("address：{}", "浙江省杭州市西湖区北京西路100号");
    logger.infoMessage("地址：{}", "上海市浦东区北京东路1-10号");

    //19位卡号
    logger.infoMessage("cardNo：{}", "6227002020000101222");

    //姓名
    logger.infoMessage("name={}, 姓名=[{}]，name={}，姓名：{}", "张三", "上官婉儿", "李云龙", "楚云飞");

    //密码
    logger.infoMessage("password：{}，密码={}", "123456", "456789");
    logger.infoMessage("password：{}", "123456");
    logger.infoMessage("密码={}", "123456");

    //身份证号码
    logger.infoMessage("idCard：{}，身份证号={}", "360123202111111122", "360123202111111122");
    logger.infoMessage("身份证号={}", "360123202111111122");

    //邮箱
    logger.infoMessage("邮箱:{}", "wxyz123@qq.com");
    logger.infoMessage("email={}", "wxyz123@qq.com");
}
```
```java
# 结果
log.test.LogTest : mobile=135****4444
log.test.LogTest : mobile=135****2222,手机号：135****3333
log.test.LogTest : 手机号：135****5555
log.test.LogTest : tel：0791-83****22,座机=021-88****34
log.test.LogTest : tel：0791-83****22
log.test.LogTest : 座机=021-88****34
log.test.LogTest : address：浙江省****
log.test.LogTest : 地址：上海市****
log.test.LogTest : cardNo：***************1222
log.test.LogTest : name=张**, 姓名=[上**]，name=李**，姓名：楚**
log.test.LogTest : password：******，密码=******
log.test.LogTest : password：******
log.test.LogTest : 密码=******
log.test.LogTest : idCard：3****************2，身份证号=3****************2
log.test.LogTest : 身份证号=3****************2
log.test.LogTest : 邮箱:w****3@qq.com
log.test.LogTest : email=w****3@qq.com
```
json和toString的脱敏输出
```java
@Test
public void test2() {
	User user = new User();
	user.setCardNo("6227002020000101222");
	user.setTel("0571-28821111");
	user.setAddress("浙江省西湖区西湖路288号钱江乐园2-101室");
	user.setEmail("zhangs12345@qq.com");
	user.setPassword("123456");
	user.setMobile("15911116789");
	user.setName("张三");
	user.setIdCard("360123202111111122");

	Job job = new Job();
	job.setAddress("浙江省西湖区西湖路288号钱江乐园2-101室");
	job.setTel("0571-12345678");
	job.setJobName("操作员");
	job.setSalary(2000);
	job.setCompany("股份有限公司");
	job.setPosition(Arrays.asList("需求", "开发", "测试", "上线"));

	user.setJob(job);

	//toString
	logger.infoMessage("用户信息：{}", user);
	//json
	logger.infoMessage("用户信息：{}", JSONUtil.toJsonStr(user));
}
```
```java
log.test.LogTest : 用户信息：User{name='张**', idCard='3****************2', cardNo='***************1222', mobile='159****6789', tel='0571-28****11', password='******', email='z****5@qq.com', address='浙江省****', job=Job{jobName='操作员', salary=2000, company='股份有限公司', address='浙江省****', tel='0571-12****78', position=[需求, 开发, 测试, 上线]}}

log.test.LogTest : 用户信息：{"password":"******","address":"浙江省****","idCard":"3****************2","name":"张**","mobile":"159****6789","tel":"0571-28****11","job":{"jobName":"操作员","address":"浙江省****","company":"股份有限公司","tel":"0571-12****78","position":["需求","开发","测试","上线"],"salary":2000},"cardNo":"***************1222","email":"z****5@qq.com"}
```
在线正则测试：[https://c.runoob.com/front-end/854/](https://c.runoob.com/front-end/854/)<br />参考链接：[**https://logging.apache.org/log4j/2.x/manual/extending.html**](https://logging.apache.org/log4j/2.x/manual/extending.html)
