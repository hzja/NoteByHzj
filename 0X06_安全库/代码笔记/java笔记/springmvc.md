<a name="13KGc"></a>
# 通过IDEA新建Spring MVC项目
 <br />创建一个springmvc项目<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672663723-18d86f46-7637-4b63-b640-90c97c2b8740.png#align=left&display=inline&height=367&originHeight=733&originWidth=1284&status=done&style=none&width=642)<br />输入项目名和保存的地址<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672663991-408cf0ce-274c-48a9-b25f-17c9b17eb9db.png#align=left&display=inline&height=376&originHeight=751&originWidth=981&status=done&style=none&width=491)<br />创建好后的文件，idea会生成如下文件<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672664243-b14da2b6-2295-43fd-b688-95919a8d8f1f.png#align=left&display=inline&height=374&originHeight=498&originWidth=438&status=done&style=none&width=329)<br /> 
<a name="Rej7Z"></a>
# 配置tomcat运行web服务
按照如下图所示的步骤<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672664496-407e7fcd-0069-47f6-93ae-5123bec5892c.png#align=left&display=inline&height=460&originHeight=920&originWidth=1204&status=done&style=none&width=602)<br />配置好安装的tomcat的路径、web映射的url后。再deployment中添加war包<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672664729-2ea92b19-f2b4-4458-80b4-423ff63e6e03.png#align=left&display=inline&height=448&originHeight=896&originWidth=1339&status=done&style=none&width=670)<br />然后记得再Project Structure中添加丢失的文件，不然启动web项目会出错误<br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672665028-d124b2a0-f5d8-4c15-aea6-82545fadf5cc.png#align=left&display=inline&height=438&originHeight=875&originWidth=1475&status=done&style=none&width=738)<br />然后启动项目，成功启动！<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672665272-b9b1b063-53c9-4329-a22a-cbe0b33d4b4a.png#align=left&display=inline&height=132&originHeight=176&originWidth=701&status=done&style=none&width=526)
<a name="ZhUkc"></a>
# 添加Controller
在src目录中创建新包，然后在该包下创建一个类命名为controller<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672665455-d717e84b-00a9-4ae9-9d31-26e217f7be5c.png#align=left&display=inline&height=106&originHeight=106&originWidth=291&status=done&style=none&width=291)<br />**package **aufeng;<br />

**import **javax.servlet.http.HttpServletRequest;

**import **javax.servlet.http.HttpServletResponse;<br />

**import **org.springframework.web.servlet.ModelAndView;

**import **org.springframework.web.servlet.mvc.Controller;<br />

**public class **controller **implements **Controller {

    **public **ModelAndView handleRequest(HttpServletRequest request, HttpServletResponse response) **throws **Exception {

        ModelAndView mav = **new **ModelAndView(**"index.jsp"**);

        mav.addObject(**"message"**, **"Hello Spring MVC，i am aufeng"**);

        **return **mav;

    }<br />}
<a name="JtGe3"></a>
# 配置xml文件
配置web.xml文件 <**servlet-name**>dispatcher</**servlet-name**>的命名需要和dispatcher-servlet.xml一致<br />_<?_**xml version="1.0" encoding="UTF-8"**_?><br />_<br /><**web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"<br />**<br />         **xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"<br />**<br />         **xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee [http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd"](http://xmlns.jcp.org/xml/ns/javaee/web-app_4_0.xsd%22)<br />**<br />         **version="4.0"**>

    <**listener**><br />        <**listener-class**>org.springframework.web.context.ContextLoaderListener</**listener-class**><br />    </**listener**><br />    <**servlet**><br />        <**servlet-name**>dispatcher</**servlet-name**><br />        <**servlet-class**>org.springframework.web.servlet.DispatcherServlet</**servlet-class**><br />        <**load-on-startup**>1</**load-on-startup**><br />    </**servlet**><br />    <**servlet-mapping**><br />        <**servlet-name**>dispatcher</**servlet-name**><br />        <**url-pattern**>/</**url-pattern**><br />    </**servlet-mapping**><br /></**web-app**><br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672665718-261967c6-8427-4b5b-bc6e-c909a5750dba.png#align=left&display=inline&height=328&originHeight=601&originWidth=1368&status=done&style=none&width=746)<br /> <br />配置dispaer-servlet.xml文件，访问路径/abc会交给id=controller的bean处理id=controller的bean配置为aufeng包下的controller类<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672665941-c585f491-6abf-4b94-988b-bf6757666c0a.png#align=left&display=inline&height=274&originHeight=484&originWidth=1316&status=done&style=none&width=746)<br />在web文件夹路径下创建一个index.jsp文件，并用el表达式接受controller的信息<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672666200-87154226-3a65-492e-9ef8-329b9a6e3a14.png#align=left&display=inline&height=237&originHeight=420&originWidth=1321&status=done&style=none&width=746)<br /> <br /><%@ **page language**="**java**" **contentType**="**text/html; charset=UTF-8**"

         **pageEncoding**="**UTF-8**" **isELIgnored**="**false**"%>

<**h1**>**${**message**}**</**h1**><br /> <br />这样一个简单得springmvc就配置完成了<br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672666461-aa2f0406-35ee-4cfc-8bd8-6e545cb84258.png#align=left&display=inline&height=152&originHeight=203&originWidth=832&status=done&style=none&width=624)<br /> <br />思路：从web.xml中得<servlet-name>定位到dispatcher-servlet.xml文件<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672666717-2a4c2899-4e2c-4982-ae3f-245946df2ae3.png#align=left&display=inline&height=334&originHeight=522&originWidth=1165&status=done&style=none&width=746)<br />在dispatcher-servlet中定位mappings，在<property>标签中找到/abc目录指向controller，然后再<bean>中寻找controller对应到class，就是aufeng包下得controller类文件。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672666920-225038e3-cd8a-407d-b5a4-843ce77a7416.png#align=left&display=inline&height=403&originHeight=537&originWidth=871&status=done&style=none&width=653)<br /> <br /> <br /> 
<a name="dITKB"></a>
# 视图定位
访问url  [http://localhost:8080/newspringmvc_war_exploded/abc](http://localhost:8080/newspringmvc_war_exploded/abc) ，会直接跳到/WEB-INF/pages目录下的hello.jsp文件。<br />Dispatcher-servlet.xml中修改如下<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672667172-ae31d263-014a-46e3-a0f0-420a87ba258e.png#align=left&display=inline&height=339&originHeight=605&originWidth=1330&status=done&style=none&width=746)<br />代码为<br />_<?_**xml version="1.0" encoding="UTF-8"**_?><br />_<br /><**beans xmlns="http://www.springframework.org/schema/beans"<br />**<br />       **xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"<br />**<br />       **xmlns:context="http://www.springframework.org/schema/context"<br />**<br />       **xmlns:mvc="http://www.springframework.org/schema/mvc"<br />**<br />       **xsi:schemaLocation="http://www.springframework.org/schema/beans [http://www.springframework.org/schema/beans/spring-beans.xsd"](http://www.springframework.org/schema/beans/spring-beans.xsd%22)**>

    <**bean id="viewResolver"<br />**<br />          **class="org.springframework.web.servlet.view.InternalResourceViewResolver"**><br />        <**property name="prefix" value="/WEB-INF/pages/" **/><br />        <**property name="suffix" value=".jsp" **/><br />    </**bean**>

    <**bean id="simpleUrlHandlerMapping"<br />**<br />          **class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping"**><br />        <**property name="mappings"**><br />            <**props**><br />                <**prop key="/abc"**>controller</**prop**><br />            </**props**><br />        </**property**><br />    </**bean**><br />    <**bean id="controller" class="aufeng.controller"**></**bean**>

</**beans**><br /> <br />controller控制器中把旧代码注释掉。传入参数hello<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672667441-14eccd4d-401d-48d5-acde-ef328ab6a822.png#align=left&display=inline&height=248&originHeight=440&originWidth=1324&status=done&style=none&width=746)<br /> <br />hello.jsp代码如下<br /><%@ **page contentType**="**text/html;charset=UTF-8**" **language**="**java**" %><br /><**html**><br /><**head**><br />    <**title**>springmvc</**title**><br /></**head**><br /><**body**><br />hello springmvc;<br /><**h1**>**${**message**}**</**h1**><br /></**body**><br /></**html**><br />运行效果<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672667703-6c9eb9f4-3e93-4d92-8da1-1419797de4a9.png#align=left&display=inline&height=193&originHeight=257&originWidth=844&status=done&style=none&width=633)<br /> <br /> 
<a name="R0m5A"></a>
# SpringMVC注解
 
<a name="akLIy"></a>
# 失败篇
但是后面还是失败了<br />关于spring”通配符的匹配很全面, 但无法找到元素 'context:component-scan' 的声明“的错误<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672667970-c6309820-31de-4c34-b863-4d88f9eb277e.png#align=left&display=inline&height=364&originHeight=854&originWidth=1751&status=done&style=none&width=746)<br /> <br /> <br /><**servlet-name**>dispatcher</**servlet-name**> 中的dispatcher需要和dispatcher-servlet.xml的名字一致，<br /> <br /><**url-pattern**>/</**url-pattern**> 是指url的路径<br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672668219-bf36a304-73f7-41f3-bbb9-00356b0e1937.png#align=left&display=inline&height=355&originHeight=590&originWidth=1240&status=done&style=none&width=746)<br /> <br />Dispatcher-server.xml文件中 <**context:component-scan base-package="aufeng"**/> 是指扫描  控制器中的包<br /> <br /><**property name="prefix" value="/WEB-INF/pages/" **/><br /><**property name="suffix" value=".jsp"  **/><br /> <br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672668494-1c6f9b50-9fde-4fcc-8433-e36d2cf27c51.png#align=left&display=inline&height=290&originHeight=479&originWidth=1231&status=done&style=none&width=746)<br />控制器中的mvc路径下的hello路径调用hello.jsp文件，因为<**property name="suffix" value=".jsp"  **/>，所以return “hello”不带后缀， url为   ip/web映射路径/mvc/hello<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672668714-efaa6097-2864-4422-a39c-dd6e699c3a80.png#align=left&display=inline&height=314&originHeight=419&originWidth=994&status=done&style=none&width=746)<br /> 
<a name="FLlXZ"></a>
# SpringMVC常用注解
@Controller<br />负责注册一个bean 到spring 上下文中。<br /> <br />@RequestMapping<br />注解为控制器指定可以处理哪些 URL 请求。<br />![](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612672668983-62e68015-f0fc-44d0-8506-f56894629b65.png#align=left&display=inline&height=513&originHeight=684&originWidth=968&status=done&style=none&width=726)

<a name="h8CiM"></a>
# 无注解路由
主要还是看dispatcher-servlet.xml文件<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612677787837-2f9ece89-b099-49ec-a7ba-448d81e87741.png#align=left&display=inline&height=229&originHeight=458&originWidth=1210&size=46665&status=done&style=none&width=605)

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612679236718-a3323ba8-f69f-4221-b794-74917ea6c60c.png#align=left&display=inline&height=221&originHeight=441&originWidth=1292&size=44367&status=done&style=none&width=646)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612679270372-f7af0e75-88a0-4b60-858c-ac95fc29c3de.png#align=left&display=inline&height=174&originHeight=232&originWidth=840&size=17540&status=done&style=none&width=630)
<a name="oHqaD"></a>
# 有注解路由

在类前面加上@Controller  表示该类是一个控制器<br />在方法handleRequest 前面加上 @RequestMapping("/index") 表示路径/index会映射到该方法上<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612681206235-53a56c1d-df87-4aab-bc1a-6bcaade74494.png#align=left&display=inline&height=328&originHeight=328&originWidth=1119&size=107402&status=done&style=none&width=1119)

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612680768363-5127f32c-94c8-4e59-aefe-e5f521ffdfc0.png#align=left&display=inline&height=239&originHeight=318&originWidth=966&size=39012&status=done&style=none&width=725)<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612680928088-c4fa0b71-1cf2-42a6-a29a-19f4ba0111df.png#align=left&display=inline&height=178&originHeight=237&originWidth=857&size=17649&status=done&style=none&width=643)
