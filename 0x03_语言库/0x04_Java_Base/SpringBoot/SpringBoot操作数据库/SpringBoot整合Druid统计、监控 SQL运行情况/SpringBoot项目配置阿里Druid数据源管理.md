Java SpringBoot Druid
<a name="w7K5U"></a>
## Environment
>>SpringBoot -V2.1.8<br />>>Win10<br />>>IDEA-2019.2.2<br />>>Java8
<a name="8MKKp"></a>
## 1、Github代码提交记录
[SpringBoot项目配置阿里云Druid数据源监控](https://github.com/Fcscanf/LKDShop/commit/657ad7be10aff32446d1e7a178cefcfcd9f7b720)
<a name="e0904d28"></a>
## 2、Maven工程引入pom依赖
```xml
<!--引入阿里的druid数据源-->
<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid</artifactId>
  <version>1.1.20</version>
</dependency>
```
<a name="9484c9d1"></a>
## 3、SpringBoot中添加配置类
<a name="jcDZz"></a>
### DruidConfig.java
```java
package com.fcant.shop.config;

import com.alibaba.druid.pool.DruidDataSource;
import com.alibaba.druid.support.http.StatViewServlet;
import com.alibaba.druid.support.http.WebStatFilter;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.boot.web.servlet.FilterRegistrationBean;
import org.springframework.boot.web.servlet.ServletRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.sql.DataSource;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * DruidConfig
 *
 * @author Fcscanf@樊乘乘
 * @description
 * @date 上午 11:18 2018-08-14
 */
@Configuration
public class DruidConfig {

    /**
     * 注册Druid
     *
     * @param
     * @return
     * @author Fcscanf@樊乘乘
     * @date 上午 11:24 2018-08-14
     */
    @ConfigurationProperties(prefix = "spring.datasource")
    @Bean
    public DataSource druid() {
        return new DruidDataSource();
    }

    //配置Druid监控

    /**
     * 配置Druid管理后台的Servlet
     *
     * @param
     * @return
     * @author Fcscanf@樊乘乘
     * @date 上午 11:25 2018-08-14
     */
    @Bean
    public ServletRegistrationBean statViewServlet() {
        ServletRegistrationBean bean = new ServletRegistrationBean(new StatViewServlet(), "/druid/*");
        //配置数据后台管理员
        Map<String, String> initParams = new HashMap<>();
        initParams.put("loginUsername", "admin");
        initParams.put("loginPassword", "509165");
        //值为空则默认允许所有人访问
        initParams.put("allow", "");
        //指定用户不能登录
        initParams.put("deny", "101.132.167.127");
        bean.setInitParameters(initParams);
        return bean;
    }

    /**
     * 配置Druid后台的监控Filter
     *
     * @param
     * @return
     * @author Fcscanf@樊乘乘
     * @date 上午 11:25 2018-08-14
     */
    @Bean
    public FilterRegistrationBean webStatFilter() {
        FilterRegistrationBean bean = new FilterRegistrationBean();
        bean.setFilter(new WebStatFilter());
        Map<String, String> initParams = new HashMap<>();
        initParams.put("exclusions", "*.js,*.css,/druid/*");
        bean.setInitParameters(initParams);
        bean.setUrlPatterns(Arrays.asList("/*"));
        return bean;
    }
}

```
<a name="O96Q4"></a>
## 4、application.yml进行配置
```yaml
spring:
  datasource:
    username: root
    password: 123456
    url: jdbc:mysql:///ssm?serverTimezone=UTC
    driver-class-name: com.mysql.cj.jdbc.Driver
    type: com.alibaba.druid.pool.DruidDataSource
```
<a name="oyALl"></a>
##  5、项目目录结构
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568690693805-1118b116-b2c1-4357-95e2-2508f1e123d9.png#align=left&display=inline&height=541&originHeight=1622&originWidth=988&size=119840&status=done&style=shadow&width=329.3333333333333)
<a name="VHLf0"></a>
## 6、运行结果示意图
<a name="Vxjve"></a>
### A.项目路径添加请求druid进入登录界面
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568690834955-fd61c49b-ebf5-4283-b0f5-05b9bb4173ed.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=367133&status=done&style=none&width=1280)
<a name="aG3vH"></a>
### B.进入项目的数据库监控管理
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568690857863-3f24adbb-924d-4ca3-8461-f2bbd908cf7b.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=545875&status=done&style=shadow&width=1280)
