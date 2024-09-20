Java Spring SSM
<a name="ajFlN"></a>
## 项目背景

1. 支持Restful风格的请求
2. 支持PageHelper分页插件
3. 支持Mybatis逆向工程配置文件
4. 使用阿里Druid管理数据源
<a name="A6AMl"></a>
## 1、在IDEA中创建新的Maven项目
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564657861423-8dc4f645-1f36-4c45-9fa9-fd484ceef07a.png#height=444&id=DUswK&originHeight=1222&originWidth=1941&originalType=binary&ratio=1&rotation=0&showTitle=false&size=212027&status=done&style=shadow&title=&width=705.8181818181819)

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564657927161-34db24b9-95b3-49a6-b2d0-f7663b38882b.png#height=643&id=cUACV&originHeight=1769&originWidth=2083&originalType=binary&ratio=1&rotation=0&showTitle=false&size=417320&status=done&style=none&title=&width=757.4545454545455)

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564658134353-d0996046-d30d-489b-b7fb-17f7ba4325e0.png#height=643&id=HXZ49&originHeight=1769&originWidth=2083&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114312&status=done&style=none&title=&width=757.4545454545455)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564658162779-c569dd8d-183b-45bb-93a5-0823e9e4ff80.png#height=643&id=OX4P8&originHeight=1769&originWidth=2083&originalType=binary&ratio=1&rotation=0&showTitle=false&size=110156&status=done&style=none&title=&width=757.4545454545455)
<a name="WeMxV"></a>
## 2、添加项目相关依赖jar包
<a name="M4eqM"></a>
### A.使用Maven管理jar包
```xml
	<properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.source>1.8</maven.compiler.source>
        <maven.compiler.target>1.8</maven.compiler.target>
        <spring.version>5.1.8.RELEASE</spring.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
        </dependency>

        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.2</version>
        </dependency>

        <dependency>
            <groupId>commons-fileupload</groupId>
            <artifactId>commons-fileupload</artifactId>
            <version>1.4</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-expression</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aspects</artifactId>
            <version>${spring.version}</version>
        </dependency>


        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>


        <!-- AOP相关的jar包依赖-->

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>${spring.version}</version>
        </dependency>

        <dependency>
            <groupId>aopalliance</groupId>
            <artifactId>aopalliance</artifactId>
            <version>1.0</version>
        </dependency>

        <dependency>
            <groupId>org.aspectj</groupId>
            <artifactId>aspectjweaver</artifactId>
            <version>1.9.2</version>
        </dependency>

        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>8.0.13</version>
        </dependency>

        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.4</version>
        </dependency>

        <!--Mybatis相关的jar包-->
        <dependency>
            <groupId>org.apache.ant</groupId>
            <artifactId>ant</artifactId>
            <version>1.9.6</version>
        </dependency>

        <dependency>
            <groupId>org.apache.ant</groupId>
            <artifactId>ant-launcher</artifactId>
            <version>1.9.6</version>
        </dependency>

        <dependency>
            <groupId>asm</groupId>
            <artifactId>asm</artifactId>
            <version>3.3.1</version>
        </dependency>

        <dependency>
            <groupId>cglib</groupId>
            <artifactId>cglib</artifactId>
            <version>3.2.6</version>
        </dependency>

        <dependency>
            <groupId>javassist</groupId>
            <artifactId>javassist</artifactId>
            <version>3.11.0.GA</version>
        </dependency>

        <dependency>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
            <version>1.2.17</version>
        </dependency>

        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>2.11.1</version>
        </dependency>

        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>2.11.0</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.4.6</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis.generator</groupId>
            <artifactId>mybatis-generator-core</artifactId>
            <version>1.3.7</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/com.baomidou/mybatis-plus -->
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus</artifactId>
            <version>3.1.2</version>
        </dependency>

        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>2.0.2</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/tk.mybatis/mapper -->
        <dependency>
            <groupId>tk.mybatis</groupId>
            <artifactId>mapper</artifactId>
            <version>4.1.5</version>
        </dependency>

        <dependency>
            <groupId>ognl</groupId>
            <artifactId>ognl</artifactId>
            <version>3.2.8</version>
        </dependency>

        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>1.7.25</version>
        </dependency>

        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-simple</artifactId>
            <version>2.0.0-alpha0</version>
        </dependency>

        <!-- https://mvnrepository.com/artifact/org.slf4j/slf4j-nop -->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-nop</artifactId>
            <version>2.0.0-alpha0</version>
        </dependency>

        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-log4j12</artifactId>
            <version>1.7.25</version>
        </dependency>

        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jsp-api</artifactId>
            <version>2.0</version>
        </dependency>

        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>servlet-api</artifactId>
            <version>2.5</version>
        </dependency>

        <dependency>
            <groupId>jstl</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version>
        </dependency>

        <!--jstl-->
        <dependency>
            <groupId>org.apache.taglibs</groupId>
            <artifactId>taglibs-standard-spec</artifactId>
            <version>1.2.5</version>
        </dependency>
        <dependency>
            <groupId>org.apache.taglibs</groupId>
            <artifactId>taglibs-standard-impl</artifactId>
            <version>1.2.5</version>
        </dependency>
        
        <!--引入pagehelper分页插件-->
        <dependency>
            <groupId>com.github.pagehelper</groupId>
            <artifactId>pagehelper</artifactId>
            <version>5.1.10</version>
        </dependency>

        <!--对象转json-->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.9.9</version>
        </dependency>

        <!--JSR303数据校验-->
        <dependency>
            <groupId>org.hibernate.validator</groupId>
            <artifactId>hibernate-validator</artifactId>
            <version>6.0.17.Final</version>
        </dependency>
        
        <!--引入阿里的druid数据源-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.19</version>
        </dependency>

    </dependencies>

    <build>
        <finalName>Spring</finalName>
        <plugins>
            <!--Mybatis逆向工程的插件-->
            <plugin>
                <groupId>org.mybatis.generator</groupId>
                <artifactId>mybatis-generator-maven-plugin</artifactId>
                <version>1.3.7</version>
                <configuration>
                    <!--指定本地的逆向工程设置文件-->
                    <configurationFile>src/main/resources/generator.xml</configurationFile>
                    <verbose>true</verbose>
                    <overwrite>true</overwrite>
                </configuration>
                <!--在插件配置数据库驱动包以防止插件运行找不到驱动包-->
                <dependencies>
                    <dependency>
                        <groupId>mysql</groupId>
                        <artifactId>mysql-connector-java</artifactId>
                        <version>8.0.13</version>
                    </dependency>
                </dependencies>
            </plugin>
            <plugin>
                <artifactId>maven-clean-plugin</artifactId>
                <version>3.1.0</version>
            </plugin>
            <!-- see http://maven.apache.org/ref/current/maven-core/default-bindings.html#Plugin_bindings_for_war_packaging -->
            <plugin>
                <artifactId>maven-resources-plugin</artifactId>
                <version>3.1.0</version>
            </plugin>
            <plugin>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.7.0</version>
            </plugin>
            <plugin>
                <artifactId>maven-surefire-plugin</artifactId>
                <version>2.22.2</version>
            </plugin>
            <plugin>
                <artifactId>maven-war-plugin</artifactId>
                <version>3.2.3</version>
            </plugin>
            <plugin>
                <artifactId>maven-install-plugin</artifactId>
                <version>2.5.2</version>
            </plugin>
            <plugin>
                <artifactId>maven-deploy-plugin</artifactId>
                <version>2.8.2</version>
            </plugin>
        </plugins>
    </build>
```
<a name="lMILM"></a>
### B.项目配置的jar包下载
[lib.zip](https://www.yuque.com/attachments/yuque/0/2019/zip/396745/1565063518841-d3a7f7c7-c404-42ea-a09e-ba5fedd37f9d.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2019%2Fzip%2F396745%2F1565063518841-d3a7f7c7-c404-42ea-a09e-ba5fedd37f9d.zip%22%2C%22name%22%3A%22lib.zip%22%2C%22size%22%3A29165377%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22uid%22%3A%22rc-upload-1565061643263-10%22%2C%22progress%22%3A%7B%22percent%22%3A0%7D%2C%22percent%22%3A0%2C%22id%22%3A%22tZnlR%22%2C%22card%22%3A%22file%22%7D)
<a name="wU6eM"></a>
## 3、在项目模块中添加Web模块
<a name="bXwr9"></a>
### A.创建Web模块
<a name="zQUoa"></a>
#### I.管理项目模块
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564709562767-48d1dc6f-db46-48a0-a4af-c9e2fd2b3eec.png#height=749&id=paPwM&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=817913&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="FkbPw"></a>
#### II.添加web模块
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564709678578-3eec9cc9-5e18-4123-b588-dd7e3794fd94.png#height=749&id=Mx1Xd&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=621193&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="zPQ1m"></a>
#### III.配置WebRoot和web.xml文件
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564709753306-1a6c37c7-4440-4237-be54-d0c2a119c429.png#height=749&id=Tyoji&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=475486&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="FRjSQ"></a>
#### IV.配置WebRoot
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710049294-52f12b44-1565-4073-9a4f-cad4820c7d70.png#height=749&id=K167Z&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=572887&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="kwTKE"></a>
#### V.配置web.xml-将web.xml文件放到刚才的WebRoot下面
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710290476-f3b60ca2-bb29-40c9-965e-d9ee04ad4ee9.png#height=749&id=aYeRO&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=539863&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710366049-49364bff-2618-4c7c-8ff4-7309542b8caa.png#height=749&id=VOPPV&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=505029&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="yNvGU"></a>
### B.部署Web应用
<a name="Hffv2"></a>
#### VI.创建部署包
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710448471-a3766650-be5b-4056-87d2-c493aa61ffec.png#height=749&id=IYBVy&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=491902&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710558939-bfb6bb8f-2d47-4e34-bdb8-0b8a34c69e7d.png#height=749&id=fY9I2&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=731207&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="HhkrO"></a>
#### VII.将Maven管理的jar包添加至WEB-INF下的lib中
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564745871312-1577cecb-6141-4e17-8f84-7287633b98aa.png#height=749&id=t3TTz&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=681685&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="lLN4t"></a>
#### VIII.配置Tomcat
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710696212-6cbe0637-79a2-4344-a2ff-54f0f77b878d.png#height=749&id=tVuo1&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=531735&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710819152-3ea04282-4b4b-4b4d-a5a2-3e17fa455553.png#height=749&id=TRTSP&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=532133&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564710932565-03dbaad7-3d1c-4140-ab7b-735a89ec2cdd.png#height=749&id=FGQIU&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=515036&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564711143675-1c3c9405-3a52-4589-9ecd-bdf6be0843a2.png#height=749&id=nDfen&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=613036&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="OlWl0"></a>
#### IVV.将应用部署包添加至Tomcat下
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564711292652-1ebe2c20-2d23-414e-945c-4c2a6f705f4c.png#height=749&id=poBlW&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=561077&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564711359374-5783bf50-d208-4218-8bae-2c673c7afcdf.png#height=749&id=JVB9w&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=470982&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="D9JRs"></a>
#### VV.如果没有部署包添加部署包的工程

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564711517951-a7b4953c-3624-44b1-afed-f86b73d3e3fe.png#height=749&id=ULMKn&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=453098&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="fdjH9"></a>
## 4、SpringMVC相关配置
<a name="QXGTD"></a>
### A.新建文件目录结构如下
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564713349613-bcbf36ea-b3b0-442b-820e-471a920d2b07.png#height=749&id=RdXuV&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1305574&status=done&style=shadow&title=&width=1396.3636363636363)
<a name="fYPTu"></a>
#### I.新建spring-mvc.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx.xsd
        http://www.springframework.org/schema/mvc
        http://www.springframework.org/schema/mvc/spring-mvc.xsd">
    <!-- 自动扫描该包，使SpringMVC认为包下用了@controller注解的类是控制器 -->
    <context:component-scan base-package="com.fcant.ssmcom.controller">
        <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller" />
    </context:component-scan>
    
    <!-- 注解驱动，以使得访问路径与方法的匹配可以通过注解配置 -->
    <mvc:annotation-driven >
       <mvc:message-converters register-defaults="true">
          <bean class="org.springframework.http.converter.StringHttpMessageConverter">
             <property name="supportedMediaTypes" value="text/plain;charset=UTF-8" />
          </bean>
       </mvc:message-converters>   
    </mvc:annotation-driven>
    
    <!-- 静态页面，如html,css,js,images可以访问 -->
    <mvc:default-servlet-handler />

    <!-- 定义跳转的文件的前后缀 ，视图模式配置-->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/jsp/" />
        <property name="suffix" value=".jsp" />
    </bean>

    <!-- 配置文件上传，如果没有使用文件上传可以不用配置，当然如果不配，那么配置文件中也不必引入上传组件包 -->
    <bean id="multipartResolver"
          class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
        <!-- 默认编码 -->
        <property name="defaultEncoding" value="utf-8" />
        <!-- 文件大小最大值 -->
        <property name="maxUploadSize" value="10485760000" />
        <!-- 内存中的最大值 -->
        <property name="maxInMemorySize" value="40960" />
        <!-- 启用是为了推迟文件解析，以便捕获文件大小异常 -->
        <property name="resolveLazily" value="true"/>
    </bean>
    
    <!--两个标准配置-->
    <!--将springMVC不能处理的请求交给tomcat-->
    <mvc:default-servlet-handler/>
    <!--能支持springMVC更高级的一些功能，JSR303校验。快捷的ajax...映射动态请求-->
    <mvc:annotation-driven/>

</beans>
```
<a name="cOdNU"></a>
#### II.配置web.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"
         version="4.0">

    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>

    <!-- 编码过滤器 -->
    <filter>
        <filter-name>encodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <async-supported>true</async-supported>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>encodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>

    <!-- Spring MVC servlet -->
    <servlet>
        <servlet-name>SpringMVC</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring-mvc.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
        <async-supported>true</async-supported>
    </servlet>
    <servlet-mapping>
        <servlet-name>SpringMVC</servlet-name>
        <!-- 此处可以可以配置成*.do，对应struts的后缀习惯 -->
        <url-pattern>/</url-pattern>
    </servlet-mapping>

</web-app>
```
<a name="SJ3Ar"></a>
#### III.新建index.jsp
```html
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    String path = request.getContextPath();
    String basePath = request.getScheme() + "://" + request.getServerName() + ":" + request.getServerPort() + path + "/";
%>
<html>
<head>
    <base href="<%=basePath%>">
    <title>Welcome</title>
</head>
<body>
<h3>欢迎</h3>
</body>
</html>
```
<a name="aRepi"></a>
#### IV.(EX)遇到ClassNotFound时将相关jar放到WEB-INF下的lib中,并将该文件目录添加至项目的Libraries
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564715669261-41f9e480-3003-4cc1-bdbd-bff4ab294947.png#height=749&id=IVEYR&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=938843&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564715804821-aea0f66d-9e09-4d8a-93c7-0d7e1defc39f.png#height=749&id=c9yTv&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=736518&status=done&style=none&title=&width=1396.3636363636363)
<a name="US2Ms"></a>
#### V.启动Tomcat并运行显示
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564715937155-dec38711-f864-4e61-9011-489dbe529d96.png#height=749&id=Z03cl&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=975014&status=done&style=none&title=&width=1396.3636363636363)
<a name="lA03r"></a>
## 5、日志配置
<a name="UIeq5"></a>
### I.日志文件存放位置
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565074327362-0e290adc-09d3-4eff-9b19-6f4043242d85.png#height=246&id=rl1yl&originHeight=676&originWidth=546&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65039&status=done&style=shadow&title=&width=198.54545454545453)
<a name="FP3Hh"></a>
### II.新建log4j.properties
```
log4j.rootLogger=DEBUG,STDOUT
log4j.appender.STDOUT=org.apache.log4j.ConsoleAppender
log4j.appender.STDOUT.Target=System.out
log4j.appender.STDOUT.layout=org.apache.log4j.PatternLayout
log4j.appender.STDOUT.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss}%m%n
```
<a name="eI1Wb"></a>
### III.配置log4j2.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="warn">
    <Appenders>
        <Console name="Console" target="SYSTEM_OUT">
            <PatternLayout pattern="[%-5p] %d %c - %m%n" />
        </Console>
        <File name="File" fileName="dist/my.log">
            <PatternLayout pattern="%m%n" />
        </File>
    </Appenders>

    <Loggers>
        <Logger name="mh.sample2.Log4jTest2" level="INFO">
            <AppenderRef ref="File" />
        </Logger>
        <Root level="INFO">
            <AppenderRef ref="Console" />
        </Root>
    </Loggers>
</Configuration>
```
<a name="ilo3t"></a>
## 6、Spring和Mybatis整合
<a name="xCBBQ"></a>
### A.配置数据源
<a name="SI8pN"></a>
#### I.新建jdbc.properties
```
driver=com.mysql.cj.jdbc.Driver
url=jdbc:mysql:///ssm?serverTimezone=UTC
username=root
password=root
#定义初始连接数
initialSize=0  
#定义最大连接数
maxActive=20  
#定义最大空闲
maxIdle=20  
#定义最小空闲
minIdle=1  
#定义最长等待时间
maxWait=60000  
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564717689597-173eafd4-3078-4f82-80c3-513623d3f92a.png#height=324&id=n9icF&originHeight=890&originWidth=1760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=232764&status=done&style=shadow&title=&width=640)
<a name="CeKe6"></a>
#### II.新建spring-mybatis.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context.xsd
        http://www.springframework.org/schema/aop
        http://www.springframework.org/schema/aop/spring-aop.xsd
        http://www.springframework.org/schema/tx
        http://www.springframework.org/schema/tx/spring-tx.xsd
        http://www.springframework.org/schema/mvc
        http://www.springframework.org/schema/mvc/spring-mvc.xsd">
    
    <!-- 自动扫描 -->
    <context:component-scan base-package="com.fcant.ssmcom" />

    <!-- 引入配置文件 -->
    <bean id="propertyConfigurer"
          class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
        <property name="location" value="classpath:jdbc.properties" />
    </bean>

    <!-- 配置数据源 -->
<!--    <bean class="org.springframework.jdbc.datasource.DriverManagerDataSource" id="dataSource">-->
        <!-- MySQL数据库驱动 -->
<!--        <property value="${driver}" name="driverClassName"/>-->
        <!-- 连接数据库的URL -->
<!--        <property value="${url}" name="url"/>-->
        <!-- 连接数据库的用户名 -->
<!--        <property value="${username}" name="username"/>-->
        <!-- 连接数据库的密码 -->
<!--        <property value="${password}" name="password"/>-->
<!--    </bean>-->

    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" init-method="init" destroy-method="close">
        <!-- 基本属性 url、user、password -->
        <property name="url" value="${url}" />
        <property name="username" value="${username}" />
        <property name="password" value="${password}" />
        <property name="driverClassName" value="${driver}" />

        <!-- 配置初始化大小、最小、最大 -->
        <property name="initialSize" value="3" />
        <property name="minIdle" value="3" />
        <property name="maxActive" value="20" />

        <!-- 配置获取连接等待超时的时间 -->
        <property name="maxWait" value="60000" />

        <!-- 配置间隔多久才进行一次检测，检测需要关闭的空闲连接，单位是毫秒 -->
        <property name="timeBetweenEvictionRunsMillis" value="60000" />

        <!-- 配置一个连接在池中最小生存的时间，单位是毫秒 -->
        <property name="minEvictableIdleTimeMillis" value="300000" />

        <property name="validationQuery" value="SELECT 1" />
        <property name="testWhileIdle" value="true" />
        <property name="testOnBorrow" value="false" />
        <property name="testOnReturn" value="false" />

        <!-- 打开PSCache，并且指定每个连接上PSCache的大小 -->
        <property name="poolPreparedStatements" value="true" />
        <property name="maxPoolPreparedStatementPerConnectionSize" value="20" />
    </bean>

    <!-- spring和MyBatis完美整合，不需要mybatis的配置映射文件 -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource" />
        <!-- 自动扫描mapping.xml文件 -->
        <property name="mapperLocations" value="classpath:mapper/*.xml"></property>
        <!-- PageHelper插件配置 -->
        <property name="plugins">
            <array>
                <bean class="com.github.pagehelper.PageInterceptor">
                    <property name="properties">
                        <value></value>
                    </property>
                </bean>
            </array>
        </property>
    </bean>

    <!-- DAO接口所在包名，Spring会自动查找其下的类 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.fcant.ssmcom.mapper" />
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"></property>
    </bean>

    <!-- (事务管理)transaction manager, use JtaTransactionManager for global tx -->
    <bean id="transactionManager"
          class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource" />
    </bean>
    
    <!-- 定义遇到异常就回滚 -->
    <tx:advice id="txadvice" transaction-manager="transactionManager">
        <tx:attributes>
            <tx:method name="add*" propagation="REQUIRED" rollback-for="Exception" />
            <tx:method name="del*" propagation="REQUIRED" rollback-for="Exception"/>
            <tx:method name="edit*" propagation="REQUIRED" rollback-for="Exception" />
            <tx:method name="update*" propagation="REQUIRED" rollback-for="Exception"/>
            <tx:method name="list*" propagation="REQUIRED" rollback-for="Exception"/>
        </tx:attributes>
    </tx:advice>

    <aop:config>
        <!-- 切入点的包名要与自己的service所在包一样 -->
        <aop:pointcut id="serviceMethod" expression="execution(* com.fcant.ssmcom.service.*.*(..))"/>
        <aop:advisor pointcut-ref="serviceMethod" advice-ref="txadvice"/>
    </aop:config>

</beans>
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564747896720-4e500d88-0788-4607-b5b8-08377b8513c1.png#height=749&id=iw0YW&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1214279&status=done&style=shadow&title=&width=1396.3636363636363)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565066424751-464da261-76f4-4f88-8dfe-9c265631a2f2.png#height=505&id=xzmCC&originHeight=1388&originWidth=3277&originalType=binary&ratio=1&rotation=0&showTitle=false&size=648265&status=done&style=shadow&title=&width=1191.6363636363637)
<a name="AxeeS"></a>
#### III.将spring-mybatis配置至web.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"
         version="4.0">

    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>

    <!-- Spring和mybatis的配置文件 -->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath:spring-mybatis.xml</param-value>
    </context-param>
    <!-- 编码过滤器 -->

    <!-- 编码过滤器 -->
    <filter>
        <filter-name>encodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <async-supported>true</async-supported>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>encodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>

    <!-- Spring监听器 -->
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <!-- 防止Spring内存溢出监听器 -->
    <listener>
        <listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
    </listener>

    <!-- Spring MVC servlet -->
    <servlet>
        <servlet-name>SpringMVC</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring-mvc.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
        <async-supported>true</async-supported>
    </servlet>
    <servlet-mapping>
        <servlet-name>SpringMVC</servlet-name>
        <!-- 此处可以可以配置成*.do，对应struts的后缀习惯 -->
        <url-pattern>/</url-pattern>
    </servlet-mapping>

</web-app>
```
<a name="meoBb"></a>
## 7、编写JavaBean以及Mapper接口和映射文件
避免配置在启动时找不到相应的文件而报错-也可以使用Mybatis的逆向工程生成
<a name="n2xhp"></a>
### A.自定义JavaBean以及Mapper接口和映射文件
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564791925021-a1df3fdc-9c59-4173-86c7-a4b54c2bf7ee.png#height=789&id=DRSQ7&originHeight=1576&originWidth=799&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112099&status=done&style=shadow&title=&width=400)
<a name="F5v2f"></a>
### B.逆向工程生成JavaBean以及Mapper接口和映射文件
[Mybatis连接MySQL数据库进行逆向工程 · 语雀](https://www.yuque.com/fcant/java/mybatis-generator?view=doc_embed)
<a name="TrUH5"></a>
## 8、更改配置文件相应的包
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792039038-d342dad6-3873-4c3e-b4dd-fc7e082dce24.png#height=1872&id=pneZ3&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1175065&status=done&style=none&title=&width=3490.9090152456756)<br />![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1564792088341-1fdaaf0a-1cdc-4860-bbab-2092acb769b4.png#height=1872&id=ECgsM&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1299449&status=done&style=none&title=&width=3490.9090152456756)
<a name="zzgHC"></a>
## 9、项目结构
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1565074433828-d9dcbde8-4ffa-4a26-b53f-0e723dbaab20.png#height=643&id=LxGKN&originHeight=1769&originWidth=835&originalType=binary&ratio=1&rotation=0&showTitle=false&size=124087&status=done&style=shadow&title=&width=303.6363636363636)
<a name="CTm2u"></a>
## 10、项目地址
[github](https://github.com/Fcscanf/SSMCOM/tree/53103bba1778feaf98de71f61ab6aa4676bd5261)
<a name="g4j5B"></a>
## 11、项目拓展-拦截器配置
[SpringMVC中拦截器的配置 · 语雀](https://www.yuque.com/fcant/java/springmvc002?view=doc_embed)

