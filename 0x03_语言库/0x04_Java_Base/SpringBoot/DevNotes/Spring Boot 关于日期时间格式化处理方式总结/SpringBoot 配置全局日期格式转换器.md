JavaSpringBoot
<a name="I1Ohf"></a>
## 1、SpringBoot设置后台向前台传递Date日期格式
在SpringBoot应用中，`@RestController`注解的json默认序列化中，日期格式默认为：2020-12-03T15:12:26.000+00:00类型的显示。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665138546656-5475bc31-6c0f-444b-af92-d6dfc72f2f03.png#clientId=u7f1d16e0-7364-4&from=paste&id=u05fb4598&originHeight=53&originWidth=434&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua0fa1760-f033-4e4d-be13-04a5bf6f1db&title=)<br />在实际显示中，需要对其转换成需要的显示格式。
<a name="c94MP"></a>
### 方式1：配置文件修改
配置文件配置application.yml：
```yaml
spring:
  # 配置日期格式化
  jackson:
    date-format: yyyy-MM-dd HH:mm:ss  #时间戳统一转换为指定格式
    time-zone: GMT+8  # 时区修改为东8区
```
application.properties配置方式
```
spring.jackson.date-format=yyyy-MM-dd HH:mm:ss #时间戳统一转换为指定格式
spring.jackson.time-zone=GMT+8 # 时区修改为东8区
```
【注意】这里需要修改时区time-zone：数据库默认时区是格林尼治的时间，如果不设置，会比实际时间少8个小时（北京时间）。
<a name="ghVHL"></a>
### 方式2：在javabean实体类上加注解
<a name="ijvSY"></a>
#### I. `@JsonFormat`注解
`@JsonFormat`注解用于属性或方法上，将Date类型转换为需要的类型显示。
```java
//在pattern上设置自己需要的格式
@JsonFormat(pattern="yyyy-MM-dd HH:mm:ss")
private Date createTime;
```
<a name="i8iLR"></a>
#### II. `@DateTimeFormat`注解
在需要进行日期格式转换的Date属性上添加注解`@DateTimeFormat(pattern = "需要转换的格式")`
```java
//注解将yyyy-MM-dd的形式转换为Date数据
@DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")
private Date birthday;
```
<a name="iwdSc"></a>
#### III. `@Temporal`注解
通过`@Temporal`注解，实现日期格式转换，它自带属性参数，比如：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1665138546691-8aa325de-362d-42ce-bd9d-c26999f248c9.png#clientId=u7f1d16e0-7364-4&from=paste&id=u841c8d78&originHeight=159&originWidth=883&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u35d867ca-2124-46ae-a110-59b957273f6&title=)
```java
@Temporal(TemporalType.TIMESTAMP)
private Date createTime;
```
<a name="T0jkv"></a>
## 2、SpringBoot配置全局日期格式转换器
配置从页面接收的String和json格式的日期转换为Date类型。
<a name="uInu2"></a>
### 2.1 配置String类型表单传参转Date的转换器
```java
import org.springframework.core.convert.converter.Converter;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;


//Converter<S,T>  S: 代表的是源,将要转换的数据类型  T:目标类型,将会转成什么数据类型
@Component
public class GlobalFormDateConvert implements Converter<String, Date> {

    //静态初始化定义日期字符串参数列表（需要转换的）
    private static final List<String> paramList = new ArrayList<>();

    //静态初始化可能初夏你的日期格式
    private static final String param1 = "yyyy-MM";
    private static final String param2 = "yyyy-MM-dd";
    private static final String param3 = "yyyy-MM-dd HH:mm";
    private static final String param4 = "yyyy-MM-dd HH:mm:ss";

    //静态代码块，将日期参数加入到列表中
    static {
        paramList.add(param1);
        paramList.add(param2);
        paramList.add(param3);
        paramList.add(param4);
    }

    //自定义函数，将字符串转Date  参1：传入的日期字符串  参2：格式参数
    public Date parseDate(String source, String format) {
        System.out.println("parseDate转换日期");
        Date date = null;
        try {
            //日期格式转换器
            DateFormat dateFormat = new SimpleDateFormat(format);
            date = dateFormat.parse(source);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return date;
    }


    //convert转换方法 ，s是将会传递过来的日期的字符串
    @Override
    public Date convert(String source) {

        System.out.println("convert日期格式转换器");
        if(StringUtils.isEmpty(source)){
            return null;
        }
        source = source.trim();   //去除首尾空格
        DateFormat dateFormat = new SimpleDateFormat(param1);

        //正则表达式判断是哪一种格式参数
        if (source.matches("^\\d{4}-\\d{1,2}$")) {
            return parseDate(source, paramList.get(0));
        } else if (source.matches("^\\d{4}-\\d{1,2}-\\d{1,2}$")) {
            return parseDate(source, paramList.get(1));
        } else if (source.matches("^\\d{4}-\\d{1,2}-\\d{1,2} {1}\\d{1,2}:\\d{1,2}$")) {
            return parseDate(source, paramList.get(2));
        } else if (source.matches("^\\d{4}-\\d{1,2}-\\d{1,2} {1}\\d{1,2}:\\d{1,2}:\\d{1,2}$")) {
            return parseDate(source, paramList.get(3));
        } else {
            throw new IllegalArgumentException("还未定义该种字符串转Date的日期转换格式 --> 【日期格式】：" + source);
        }
    }
}
```
<a name="mkL1E"></a>
### 2.2 配置Json数据转Date的全局日期转换器
```java
import java.text.FieldPosition;
import java.text.ParsePosition;
import java.text.SimpleDateFormat;
import java.util.Date;
import com.fasterxml.jackson.databind.util.StdDateFormat;
import org.springframework.util.StringUtils;

public class GlobalJsonDateConvert extends StdDateFormat {

    //静态初始化final，共享
    public static final GlobalJsonDateConvert instance = new GlobalJsonDateConvert();

    //覆盖parse(String)这个方法即可实现
    @Override
    public Date parse(String dateStr, ParsePosition pos) {
        return getDate(dateStr, pos);
    }

    @Override
    public Date parse(String dateStr) {
        ParsePosition pos = new ParsePosition(0);
        return getDate(dateStr, pos);
    }

    private Date getDate(String dateStr, ParsePosition pos) {
        System.out.println("json格式日期转换");
        SimpleDateFormat sdf = null;
        if (StringUtils.isEmpty(dateStr)) {
            return null;
        } else if (dateStr.matches("^\\d{4}-\\d{1,2}$")) {
            sdf = new SimpleDateFormat("yyyy-MM");
            return sdf.parse(dateStr, pos);
        } else if (dateStr.matches("^\\d{4}-\\d{1,2}-\\d{1,2}$")) {
            sdf = new SimpleDateFormat("yyyy-MM-dd");
            return sdf.parse(dateStr, pos);
        } else if (dateStr.matches("^\\d{4}-\\d{1,2}-\\d{1,2} {1}\\d{1,2}:\\d{1,2}$")) {
            sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
            return sdf.parse(dateStr, pos);
        } else if (dateStr.matches("^\\d{4}-\\d{1,2}-\\d{1,2} {1}\\d{1,2}:\\d{1,2}:\\d{1,2}$")) {
            sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            return sdf.parse(dateStr, pos);
        } else if (dateStr.length() == 23) {
            sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
            return sdf.parse(dateStr, pos);
        }
        return super.parse(dateStr, pos);
    }

    @Override
    public StringBuffer format(Date date, StringBuffer toAppendTo, FieldPosition fieldPosition){
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return sdf.format(date, toAppendTo, fieldPosition);
    }

    @Override
    public GlobalJsonDateConvert clone() {
        return new GlobalJsonDateConvert();
    }
}
```
<a name="lvUXB"></a>
### 2.3 配置bean交给Spring管理
```java
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.xgf.online_mall.convert.GlobalFormDateConvert;
import com.xgf.online_mall.convert.GlobalJsonDateConvert;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.support.ConversionServiceFactoryBean;
import org.springframework.core.convert.ConversionService;
import org.springframework.core.convert.converter.Converter;
import org.springframework.http.MediaType;
import org.springframework.http.converter.json.MappingJackson2HttpMessageConverter;

import java.util.ArrayList;
import java.util.List;
import java.util.HashSet;
import java.util.Set;


@Configuration
public class WebConfig {

    //JSON格式 全局日期转换器配置
    @Bean
    public MappingJackson2HttpMessageConverter getMappingJackson2HttpMessageConverter() {
        MappingJackson2HttpMessageConverter mappingJackson2HttpMessageConverter = new MappingJackson2HttpMessageConverter();
        //设置日期格式
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.setDateFormat(GlobalJsonDateConvert.instance);
        objectMapper.setSerializationInclusion(JsonInclude.Include.NON_NULL);
        mappingJackson2HttpMessageConverter.setObjectMapper(objectMapper);
        //设置中文编码格式
        List<MediaType> list = new ArrayList<MediaType>();
        list.add(MediaType.APPLICATION_JSON_UTF8);
        mappingJackson2HttpMessageConverter.setSupportedMediaTypes(list);
        return mappingJackson2HttpMessageConverter;
    }

    //表单格式 全局日期转换器

    @Bean
    @Autowired
    public ConversionService getConversionService(GlobalFormDateConvert globalDateConvert){
        ConversionServiceFactoryBean factoryBean = new ConversionServiceFactoryBean();
        Set<Converter> converters = new HashSet<>();
        converters.add(globalDateConvert);
        factoryBean.setConverters(converters);
        return factoryBean.getObject();
    }
}
```
