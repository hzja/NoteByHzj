<a name="7kUjG"></a>
## 判断框架
判断是不是springboot框架很简单，直接看pom.xml有没有spring-boot<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626830381138-c9b52b0d-e0da-4db2-9cb0-5987163f9e86.png#height=377&id=SqA0J&originHeight=503&originWidth=865&originalType=binary&ratio=1&rotation=0&showTitle=false&size=50223&status=done&style=none&title=&width=649)
<a name="wd3bm"></a>
## 关于容器
**springboot是内置tomcat的。**<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624976817183-9659b147-edf4-43cd-8d97-0a648cf1faaf.png#height=479&id=K4XEj&originHeight=957&originWidth=1486&originalType=binary&ratio=1&rotation=0&showTitle=false&size=197285&status=done&style=none&title=&width=743)<br />**开发部署springboot到服务器上，一般都是jar包或者war包。**<br />springboot的路由十分简单<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612681264853-dde42b2a-d1e1-4cbd-9b71-715b78331085.png#height=377&id=lKAs6&originHeight=654&originWidth=420&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=242)<br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612681265124-a9a616e3-935b-4dc9-b8b4-ef4c371e3d7c.png#height=305&id=SEt6i&originHeight=610&originWidth=1106&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=553)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612683337540-1e9c20b5-cb4b-455b-a0d5-b162deb60c1e.png#height=334&id=HRn3P&originHeight=667&originWidth=1198&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68524&status=done&style=none&title=&width=599)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612683353491-92539ee3-3e0d-43f5-af3e-25894579ead5.png#height=134&id=ksFTG&originHeight=179&originWidth=629&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9048&status=done&style=none&title=&width=472)
<a name="rdSlh"></a>
## 创建Springboot
直接new一个project，命名为test5<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624844595775-81516082-1983-4d0f-bdda-4aefec39ff8a.png#height=479&id=WHJ6x&originHeight=957&originWidth=854&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51809&status=done&style=none&title=&width=427)<br />选中以下多个依赖，spring boot、spring web、JDBC API、MyBatis、Mysql<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624845063165-9618464d-1505-425e-b637-50808173523f.png#height=572&id=sQxBM&originHeight=762&originWidth=1110&originalType=binary&ratio=1&rotation=0&showTitle=false&size=238399&status=done&style=none&title=&width=833)<br />Project name为test5_springboot<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624844834582-495ac8d4-df7e-4c08-8549-1dc758687f57.png#height=479&id=i304a&originHeight=957&originWidth=1398&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59762&status=done&style=none&title=&width=699)<br />如果依赖包啥的出现以下报错问题，就配置一下maven包的位置<br />Cannot resolve plugin org.apache.maven.plugins:maven-deploy-plugin:2.8.2<br />创建好后，DemoApplication有个@SpringBootApplication的标志。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624890687003-7c264a2f-be6a-48bc-bb2c-2b25aacd802c.png#height=265&id=KmMxP&originHeight=353&originWidth=974&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25522&status=done&style=none&title=&width=731)
<a name="9cf7B"></a>
## 创建数据库配置文件
把原先的application.properties改为application.yml，内容如下主要连接sql中的sec_sql库<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624890867570-6286521c-59e7-493a-8401-c540cbe04665.png#height=444&id=XXQKd&originHeight=592&originWidth=1397&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70563&status=done&style=none&title=&width=1048)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624890959858-f241a570-0ff2-4415-9857-7b99e64f24e1.png#height=333&id=FLmV4&originHeight=444&originWidth=341&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22547&status=done&style=none&title=&width=256)<br />配置好数据库配置之后，springboot的页面就可以启动了，但是前端会没有内容显示。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891040124-dd4dd0f6-5651-47a2-a2c2-13e2353c1a62.png#height=226&id=fU4tQ&originHeight=301&originWidth=897&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29868&status=done&style=none&title=&width=673)
<a name="kNO6B"></a>
## Springboot路由和前端显示
然后在com.test5.demo中创建一个controller文件，并创建UserController.java文件顺序如下图所示<br />@RestController 把内容返回给前端。<br />@RequestMapping(**"/abc"**)  前端访问的路径<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891174120-b27964a0-45a0-48ff-b610-319c35dc5e69.png#height=323&id=WELus&originHeight=430&originWidth=1308&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64108&status=done&style=none&title=&width=981)

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624872579720-35661d65-d0bc-4c22-9e6f-a8feab0eb1a9.png#height=146&id=te1iS&originHeight=195&originWidth=636&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9514&status=done&style=none&title=&width=477)

在controller文件夹下创建一个Animal.java文件，里边都是类的构造函数，顺序是 ： 定义好类型→定义好构造函数(其实就是this赋值)→set和get<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891451288-de608f33-4841-4b42-90b4-51cba5184a3b.png#height=591&id=zTrPw&originHeight=788&originWidth=1314&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93284&status=done&style=none&title=&width=986)<br />右击生成处Constructor是构造函数的生成，然后就是Getter和Setter的设置。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891587913-ad504626-69af-4c5c-8ddc-385df0bfb4f2.png#height=458&id=PVY9Q&originHeight=610&originWidth=672&originalType=binary&ratio=1&rotation=0&showTitle=false&size=63466&status=done&style=none&title=&width=504)      →           ![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891615919-bf94008e-4c74-43da-ab46-abc901bd0412.png#height=294&id=Je5YH&originHeight=294&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10373&status=done&style=none&title=&width=294)<br />在UserController.java中添加abcd路径，并new一个Animal的对象。并返回对象的值。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624891948496-f0349c51-266b-4a04-b959-0145d511e8ab.png#height=329&id=UPhNy&originHeight=439&originWidth=856&originalType=binary&ratio=1&rotation=0&showTitle=false&size=48239&status=done&style=none&title=&width=642)<br />页面访问abcd所示<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624892041896-178cb9c2-9e25-46ba-90dc-c345408aefe6.png#height=179&id=wQsqf&originHeight=238&originWidth=561&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14044&status=done&style=none&title=&width=421)
<a name="H7L73"></a>
## springboot的数据库设置、mybatsic的映射
<a name="lZ7lm"></a>
### 1.建立实体类，跟数据库表字段保持一致
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624885070360-a3f9ad0d-d039-480d-a6c0-8971b2b62e93.png#height=200&id=cwg98&originHeight=266&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=162970&status=done&style=none&title=&width=449)<br />在com.test5.demo文件夹下创建一个entity的文件夹(和controller同一位置)，然后创建一个User.java文件，同样的也是要：定义好类型→定义好构造函数(其实就是this赋值)→set和get。<br />注意了，这个定义的字段需要和sec_sql数据库中的admin表中所有列的字段一样，包括类型。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624892346617-7e5c6ffc-bf54-464e-b673-cd3d75232cdc.png#height=467&id=nfkAR&originHeight=623&originWidth=1507&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114741&status=done&style=none&title=&width=1130)
<a name="CRW1k"></a>
### 2.建立mapper接口，定义要操作数据库的动作
和controller的文件夹同一目录，创建一个mapper，在mapper文件夹下面创建一个UserMapper.java文件，List<>中的User要对应。User.java是建立好的实体类(与数据库字段一致)<br />findAll()方法名是可以随意设置的。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624892976717-f3a60034-d40f-4f49-b821-9eb9a49180bb.png#height=440&id=ZJ4g6&originHeight=587&originWidth=839&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53763&status=done&style=none&title=&width=629)
<a name="n6jJh"></a>
### 3.建立mapper的xml文件，写具体的sql语句
这个xml文件需要在resources文件夹下创建一个mapper文件夹，并创建一个UserMapper.xml文件。

1、在User.java中已经定义数据库sql_sec中admin表所有列的字段<br />2、UserMapper.java里边定义了findAll方法。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624893371380-3dd1df09-8034-4414-9b70-8b031b7fd9b8.png#height=327&id=G1T2k&originHeight=436&originWidth=1481&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75383&status=done&style=none&title=&width=1111)
<a name="lBJgS"></a>
### 4.建立service类，处理业务逻辑
@Service 定义是service层，处理业务逻辑<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624893966655-7a24af75-1be7-4a6c-b9c9-2e40ee22b1c5.png#height=418&id=sdYGx&originHeight=557&originWidth=1007&originalType=binary&ratio=1&rotation=0&showTitle=false&size=62372&status=done&style=none&title=&width=755)
<a name="wdm3A"></a>
### 5.在controller类中展示处理结果
controller→service是必经之路。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624894192038-4919abcf-af6a-449f-877d-884075df46e4.png#height=521&id=lQ9wW&originHeight=694&originWidth=977&originalType=binary&ratio=1&rotation=0&showTitle=false&size=77030&status=done&style=none&title=&width=733)<br />访问浏览器的aufeng路径，成功把数据库的内容输出到页面。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624894273948-1a8080c2-a038-45f7-8707-8d3914b4fb48.png#height=214&id=ZnKSx&originHeight=285&originWidth=1155&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42996&status=done&style=none&title=&width=866)

<a name="mToaC"></a>
## 关于springboot解析jsp的问题
主要是application.yml文件或者application.propri文件下设置了jsp所放置的目录(/templates/）。这样子，controller写好的路径/hello被访问到就会跳转到test.jsp文件。这样子jsp才会被解析到。**所以如果是springboot有任意文件上传应该也是没办法解析的了。而且springboot部署到服务器上常用jar包启动，就更难了。除非是通过war包部署到外部tomcat，或者是添加了jasper的解析库才可能有机会shell。**<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624980587084-4ba24e5f-8549-4df7-a393-93c07c807a11.png#height=281&id=vpjBH&originHeight=374&originWidth=866&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40996&status=done&style=none&title=&width=650)<br />http://ip:8081/hello  →映射出test.jsp的内容<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1624980779942-5b45cec1-ed21-4015-8dae-c7e9bee412aa.png#height=303&id=bObG2&originHeight=404&originWidth=1461&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57293&status=done&style=none&title=&width=1096)

pom中解析jsp的jasper解析库<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662101983447-d3ea9346-b92f-4eee-9932-8032ebeb3b23.png#clientId=u458bc996-e87f-4&from=paste&height=472&id=u51e91ceb&originHeight=590&originWidth=736&originalType=binary&ratio=1&rotation=0&showTitle=false&size=157926&status=done&style=none&taskId=u41716893-0f42-4f3c-99aa-54e90ace3df&title=&width=588.8)

<a name="UdRlH"></a>
## springboot创建filter链
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640229229790-d7317501-ad28-4fa7-b3d2-6bbdf428a736.png#clientId=uc54510b2-b7e2-4&from=paste&height=455&id=u795d3da4&originHeight=909&originWidth=1000&originalType=binary&ratio=1&rotation=0&showTitle=false&size=136088&status=done&style=none&taskId=u9214c37a-66fb-4d90-a5e4-9f134a4f872&title=&width=500)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640229244709-0a987ed3-753d-4c80-89bc-649e8d038ab5.png#clientId=uc54510b2-b7e2-4&from=paste&height=260&id=u8f3eb04a&originHeight=520&originWidth=1221&originalType=binary&ratio=1&rotation=0&showTitle=false&size=49056&status=done&style=none&taskId=ud9290ef0-7493-4991-84cb-132a251916a&title=&width=610.5)

然后再controller层添加bean<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640229279945-1f6ed938-48c2-4878-9684-8cd38c5b03a3.png#clientId=uc54510b2-b7e2-4&from=paste&height=308&id=u2e0fd44d&originHeight=616&originWidth=1368&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80423&status=done&style=none&taskId=u89ef6b52-8e79-4acc-8964-569ef582c9a&title=&width=684)
```java
    @Bean
    public FilterRegistrationBean testFilterRegistration() {

        FilterRegistrationBean registration = new FilterRegistrationBean();
        registration.setFilter(new Filter());
        registration.addUrlPatterns("/*");
        registration.addInitParameter("paramName", "paramValue");
        registration.setName("testFilter");
        registration.setOrder(1);
        return registration;
    }
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1640229444218-509c6b71-556e-4e77-8793-eb2a9708a4fa.png#clientId=uc54510b2-b7e2-4&from=paste&height=184&id=u4d23a325&originHeight=367&originWidth=1457&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72546&status=done&style=none&taskId=u5241edf9-5423-4e4c-a99c-92a643f185e&title=&width=728.5)


interceptor过滤器<br />controller中设置session<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641227109551-4d6697d5-ba19-4557-927d-465489f5d821.png#clientId=u982cb9c0-6948-4&from=paste&height=353&id=u34877b21&originHeight=705&originWidth=1275&originalType=binary&ratio=1&rotation=0&showTitle=false&size=671074&status=done&style=none&taskId=ue154db0b-c7c5-4391-92c2-27d8112cde9&title=&width=637.5)<br />设置session<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641218556660-b4a95f2e-6b89-4ee6-8e2c-8c344c4c4862.png#clientId=u982cb9c0-6948-4&from=paste&height=320&id=uf2326e32&originHeight=542&originWidth=924&originalType=binary&ratio=1&rotation=0&showTitle=false&size=373004&status=done&style=none&taskId=u117df1e5-1688-4d69-bdff-660979503ea&title=&width=546)

设置url拦截器<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641218647749-9c869420-93c5-4963-9401-a93ca253a941.png#clientId=u982cb9c0-6948-4&from=paste&height=244&id=u316d9578&originHeight=488&originWidth=1105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=462543&status=done&style=none&taskId=uee861e7b-ab2e-45e6-9942-8f1912bcee2&title=&width=552.5)

因为这个过滤器的存在，在登陆之前所有的资源都不能访问。所以在login.jsp上如果有图片，js和css，也不能够正常显示和工作。这样做当然是不行的，那么如何让js、css和图片文件即使在不登陆的情况下，也可以访问呢？ 

filter设置过滤器<br />[https://how2j.cn/k/filter/filter-auth/590.html#nowhere](https://how2j.cn/k/filter/filter-auth/590.html#nowhere)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641227319789-03dd21da-c27b-4867-8399-64df970d9b0c.png#clientId=u92357c43-61b0-4&from=paste&height=455&id=u54b7946e&originHeight=712&originWidth=787&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57031&status=done&style=none&taskId=uc4ba5c4c-37b4-47bf-a1ce-ec65fa1b485&title=&width=502.5)
