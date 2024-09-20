JavaWeblogic
<a name="AX0tJ"></a>
## 方式一、在Weblogic管理页面配置GZIP压缩的Web应用程序
参考官方文档：[https://docs.oracle.com/en/middleware/fusion-middleware/weblogic-server/12.2.1.4/wlach/taskhelp/web_applications/ConfigureGZIPCompression.html](https://docs.oracle.com/en/middleware/fusion-middleware/weblogic-server/12.2.1.4/wlach/taskhelp/web_applications/ConfigureGZIPCompression.html)
:::danger
注意：需要以管理员身份进入才能更改<br />[![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1669973095844-52ba285d-d6db-47ae-a239-fbad0493de76.png#averageHue=%23f3f2f1&clientId=udbfff7ef-757d-4&from=paste&height=713&id=u287a644b&originHeight=1783&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=455305&status=done&style=none&taskId=u747192f0-cfa8-4581-b76d-fff884f026b&title=&width=1536)](https://docs.oracle.com/en/middleware/fusion-middleware/weblogic-server/12.2.1.4/wlach/taskhelp/console/UseTheChangeCenter.html)
:::
可以启用和配置内容编码GZIP在域级别或Web应用程序级别进行压缩。特定Web应用程序的值将覆盖域级别的值。 要为域中的所有Web应用程序配置GZIP压缩：

1. 如果还没有这样做，在 管理控制台的更改中心中单击**锁定&编辑**（[see Use the Change Center](http://docs.oracle.com/middleware/1221/wls/WLACH/taskhelp/console/UseTheChangeCenter.html)）。
2. 在域结构树中，选择您的域。
3. 选择**配置> Web应用程序**。
4. 定义 GZIP压缩以下域范围内的Web应用程序配置设置：
   - **gzip压缩启用**：启用或禁用GZIP压缩，所有Web应用程序。默认情况下，该值未启用。
   - **GZIP压缩最小。内容长度**：配置触发GZIP压缩的最小内容长度。默认值为2048.
   - **GZIP压缩内容类型**：配置要包含在压缩中的内容的类型。默认值是text/html，text/xml，text/plain。点击保存以保存所有更改。
5. 要激活这些更改，请在管理控制台的更改中心中单击**激活更改**。

并非所有更改都立即生效 - 有些需要重新启动
<a name="HuBfC"></a>
## 方式二、使用第三方依赖
一、首先去[http://sourceforge.net/projects/filterlib](http://sourceforge.net/projects/filterlib)网站下载tk-filters-1.0.1.zip。<br />[tk-filters-1.0.1.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1669946226858-cc6e4bf6-e260-4df7-a09e-e857c47284f8.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2022%2Fzip%2F396745%2F1669946226858-cc6e4bf6-e260-4df7-a09e-e857c47284f8.zip%22%2C%22name%22%3A%22tk-filters-1.0.1.zip%22%2C%22size%22%3A97412%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22uf3537370-f707-442e-a956-f7de4e116ca%22%2C%22taskType%22%3A%22upload%22%2C%22__spacing%22%3A%22both%22%2C%22id%22%3A%22u4a9bb171%22%2C%22margin%22%3A%7B%22top%22%3Atrue%2C%22bottom%22%3Atrue%7D%2C%22card%22%3A%22file%22%7D)<br />二、解压这个tk-filters-1.0.1.zip压缩文件，将解压后的文件tk-filters.jar放在Ext项目的WEB-INF/lib/下。<br />三、编辑解压后的文件tk-filters\conf\tk-filters.properties
```
GZIPFilter.Enabled=false(默认为false,true打开GZIP压缩功能)
GZIPFilter.LogStats=false(默认为false,true打开GZIP压缩功能日志,可以在后台看到压缩比例信息)
CacheFilter.Enabled=false(默认为false,true打开GZIP缓存功能)
```
:::danger
注：可以自行选择想打开的功能，再将此文件复制到Ext项目的WEB-INF/class文件夹下（这边eclipse3.5是WEB-INF/classes下面）
:::
四、打开Ext项目的WEB-INF/web.xml文件
```xml
<!-- GZIPFilter压缩定义 设置此项时tk-filters.properties的GZIPFilter.Enabled=true才可用-->
<filter>
  <filter-name>GZIPFilter</filter-name>
  <filter-class>com.tacitknowledge.filters.gzipfilter.GZIPFilter</filter-class>
</filter>
<!-- GZIPFilter 设置自己想要压缩的文件类型-->
<filter-mapping>
  <filter-name>GZIPFilter</filter-name>
  <url-pattern>*.js</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>GZIPFilter</filter-name>
  <url-pattern>*.jsp</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>GZIPFilter</filter-name>
  <url-pattern>*.css</url-pattern>
</filter-mapping>
<!-- CacheFilter缓存定义 设置此项时tk-filters.properties的CacheFilter.Enabled=true才可用 -->
<filter>
  <filter-name>CacheFilter</filter-name>
  <filter-class>com.tacitknowledge.filters.cache.CacheHeaderFilter</filter-class>
</filter>
<!-- CacheFilter 设置自己想要缓存的文件类型-->
<filter-mapping>
  <filter-name>CacheFilter</filter-name>
  <url-pattern>*.gif</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>CacheFilter</filter-name>
  <url-pattern>*.jpg</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>CacheFilter</filter-name>
  <url-pattern>*.png</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>CacheFilter</filter-name>
  <url-pattern>*.js</url-pattern>
</filter-mapping>
<filter-mapping>
  <filter-name>CacheFilter</filter-name>
  <url-pattern>*.css</url-pattern>
</filter-mapping>
```
五、测试：用的也是FireFox，以及FireBug调试测试。项目中用的Ext是3.2.1，ext-all.js为662KB。压缩后得到的大小仅为181K，压缩率达到了27%，页面加载速度得到了很大的提高。
