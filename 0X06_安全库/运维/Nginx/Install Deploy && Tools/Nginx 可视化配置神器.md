Nginx<br />Nginx 是前后端开发工程师必须掌握的神器。该神器有很多使用场景，比如反向代理、负载均衡、动静分离、跨域等等。<br />![2021-10-06-13-24-11-971953.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633498067847-a0279578-89bf-4108-a0d2-2f11524cea2d.jpeg#averageHue=%23c3deb4&clientId=ubfe0028f-b769-4&from=ui&id=u7afe0652&originHeight=445&originWidth=794&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23750&status=done&style=none&taskId=u35827d2b-c70f-43c5-984c-a9086f1ffdd&title=)<br />把 Nginx 下载下来，打开 conf 文件夹的 nginx.conf 文件，Nginx 服务器的基础配置和默认的配置都存放于此。<br />配置是让程序员非常头疼的事，比如 Java 后端框架 SSM ，大量配置文件让不少人头皮发麻，所以才涌现了 Spring Boot 这样能简化配置的框架。<br />如果能够采用可视化的方式对 Nginx 进行配置，那该多好。在 GitHub 上发现了一款可以一键生成 Nginx 配置的神器，相当给力。<br />先来看看它都支持什么功能的配置：反向代理、HTTPS、HTTP/2、IPv6, 缓存、WordPress、CDN、Node.js 支持、 Python (Django) 服务器等等。<br />如果想在线进行配置，只需要打开网站：[https://nginxconfig.io/](https://nginxconfig.io/)，按照自己的需求进行操作就行了。<br />![初始配置](https://cdn.nlark.com/yuque/0/2021/png/396745/1621007255193-554d3992-114e-424a-8ace-dd9d29018cbe.png#averageHue=%23fdfdfd&clientId=u5a1a74cb-ba1d-4&from=paste&height=602&id=u6f260aae&originHeight=1805&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=true&size=356653&status=done&style=shadow&taskId=udef4d24d-73fa-48ec-bb56-3b4539dcee8&title=%E5%88%9D%E5%A7%8B%E9%85%8D%E7%BD%AE&width=1266 "初始配置")<br />![2021-10-06-13-24-12-361696.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498095201-5ef1252b-5bf2-4b34-89cf-278d98ca0f35.png#averageHue=%23fefefe&clientId=ubfe0028f-b769-4&from=ui&id=uc1a18916&originHeight=341&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=47743&status=done&style=shadow&taskId=u5e3063ae-4371-4c39-97a2-d8f55543af2&title=)![HTTPS证书配置](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498095271-57eb3be0-c807-4dfa-8882-9adcf64b8728.png#averageHue=%23fefefe&clientId=ubfe0028f-b769-4&from=ui&id=ufbf75b32&originHeight=448&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=true&size=62367&status=done&style=shadow&taskId=u228a8a9d-d0c8-4dfb-a620-91181c6385f&title=HTTPS%E8%AF%81%E4%B9%A6%E9%85%8D%E7%BD%AE "HTTPS证书配置")![2021-10-06-13-24-12-561690.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498095291-291ca266-73da-4b51-b8e1-11cdafb3d33f.png#averageHue=%23fefefe&clientId=ubfe0028f-b769-4&from=ui&id=u4b85dea8&originHeight=167&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26006&status=done&style=shadow&taskId=udd6bc09e-a32d-4c11-bb8e-c89cfe4ecce&title=)![2021-10-06-13-24-12-706691.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1633498095333-a54ceaa4-3c00-403f-b042-4a99b41ebc9e.png#averageHue=%23fdfdfd&clientId=ubfe0028f-b769-4&from=ui&id=uf014b384&originHeight=519&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=92726&status=done&style=shadow&taskId=ua2d5f2aa-b109-492f-a068-fdb835c96c1&title=)<br />![传输压缩配置](https://cdn.nlark.com/yuque/0/2022/png/396745/1667740626726-2fa9702c-438d-4c23-af39-294055a15f76.png#averageHue=%23fefefe&clientId=u8e18ce1e-76af-4&from=paste&id=u796091e2&originHeight=406&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=uf6a0e987-6a38-41dd-8945-3673c8a5de1&title=%E4%BC%A0%E8%BE%93%E5%8E%8B%E7%BC%A9%E9%85%8D%E7%BD%AE "传输压缩配置")<br />选择场景，填写好参数，系统就会自动生成配置文件。<br />功能：

- HTTPS
- HTTP/2
- IPv6
- certbot
- HSTS
- 安全标头
- SSL 配置文件
- OCSP 解析器
- 缓存
- gzip
- brotli
- 回退路由
- 反向代理
- www/non-www 重定向
- CDN
- PHP（TCP/socket、 WordPress、Drupal、Magento、Joomla)
- Node.js 支持
- Python (Django) 服务器等

开源地址：[github.com/digitalocean/nginxconfig.io](https://github.com/digitalocean/nginxconfig.io)<br />网站：[digitalocean.com/community/tools/nginx](https://digitalocean.com/community/tools/nginx)
