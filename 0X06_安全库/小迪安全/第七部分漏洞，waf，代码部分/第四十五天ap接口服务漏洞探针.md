<a name="xQa9z"></a>
## 一、思维导图
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630212546733-d4c76855-654b-4539-9437-e28afea0dc22.png#clientId=ua31bad10-16ef-4&from=paste&id=ucc889d7f&originHeight=2931&originWidth=864&originalType=url&ratio=1&status=done&style=none&taskId=u2dd72cc9-2e49-4ab5-bca4-456d6ba2453)
<a name="Jxvic"></a>
## 二、测试思路
```bash
信息收集之信息利用
第一步：首先识别网站是否有cdn，waf等产品，有则绕过。
第二步：扫描收集到网站的端口信息，真实ip地址，ip绑定的其他域名。
第三步：网站敏感路径扫描
第四步：域名+端口敏感信息扫描
第五步：ip+端口敏感目录扫描

备注：字典不应该只是敏感路径，还应该有备份文件 zip rar tar tar.gz等格式文件
```
```bash
#端口服务类安全测试
根据前期信息收集针对目标端口服务类探针后进行的安全测试，主要涉及攻击方法：口令安全，WEB
类漏洞，版本漏洞等，其中产生的危害可大可小。属于端口服务/第三方服务类安全测试面。一般在
已知应用无思路的情况下选用的安全测试方案。

#API 接口-WebServiceRESTful API
https://xz.aliyun.com/t/2412
根据应用自身的功能方向决定，安全测试目标需有 API 接口调用才能进行此类测试，主要涉及的安
全问题：自身安全，配合 WEB，业务逻辑等，其中产生的危害可大可小，属于应用 API 接口网络服
务测试面，一般也是在存在接口调用的情况下的测试方案。


WSDL（网络服务描述语言，Web Services Description Language）是一门基于 XML 的语言，用于描述
Web Services 以及如何对它们进行访问。

#漏洞关键字：
配合 shodan，fofa,zoomeye 搜索也不错哦~
inurl:jws?wsdl
inurl:asmx?wsdl
inurl:aspx?wsdl
inurl:ascx?wsdl
inurl:ashx?wsdl
inurl:dll?wsdl
inurl:exe?wsdl
inurl:php?wsdl
inurl:pl?wsdl
inurl:?wsdl
filetype:wsdl wsdl
http://testaspnet.vulnweb.com/acuservice/service.asmx?WSDL
```
![常见漏洞.png](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311118292-df9b427e-e0c8-4914-8369-f16cee0fc205.png#clientId=u3c34c1cf-dde8-4&from=paste&height=1961&id=u4647df89&originHeight=3921&originWidth=1630&originalType=binary&ratio=1&size=722606&status=done&style=none&taskId=u5c16d9fd-9e92-4e3f-8187-aa83396de31&width=815)
<a name="dojhb"></a>
## 三、演示案例
<a name="PugX7"></a>
### 1、域名信息收集
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311229881-7f691fc7-68a7-4926-b5c9-15ae96a53c31.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u322b0510&originHeight=317&originWidth=393&originalType=url&ratio=1&status=done&style=none&taskId=ub53614a7-6568-499a-bdc8-35151f666c5)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515180426.png)<br />收集测试目标<br />域名访问和IP访问，目录可能会不同：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311230098-021be909-690b-46c2-abc1-39cccf2d84b8.png#clientId=u3c34c1cf-dde8-4&from=paste&id=uc9b4280c&originHeight=286&originWidth=543&originalType=url&ratio=1&status=done&style=none&taskId=uc8d6feef-f969-47cb-ad28-c2ae73b4962)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515180621.png)<br />收集时候不仅要扫描域名下的目录，还得扫描ip地址下的。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311229838-01400593-bf60-4774-af1f-a530943691f5.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u493d3f51&originHeight=252&originWidth=321&originalType=url&ratio=1&status=done&style=none&taskId=ub6965bf7-09bb-4071-952e-91f80a256c2)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515181029.png)
<a name="YHqCz"></a>
### 2、Goby端口扫描
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311167936-fc3a9551-a591-4ebc-af44-b96bc3f5361e.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u08d000e1&originHeight=508&originWidth=831&originalType=url&ratio=1&status=done&style=none&taskId=u61cccf91-6eea-46d5-9a61-6345a9eea62)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515181615.png)<br />也可以用Nmap。
<a name="M8I87"></a>
### 3、超级弱口令检测工具
下载地址：<br />https://github.com/shack2/SNETCracker/releases https://www.uedbox.com/post/57215/ <br />界面：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311167958-b259bb8b-d1f2-4f70-9418-7609ac8ba9cf.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u9fde117e&originHeight=729&originWidth=891&originalType=url&ratio=1&status=done&style=none&taskId=ue5b2b4bf-cce4-4aa6-93ee-93bf8db7bfc)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515182342.png)<br />前提是要支持外链，如果对方数据库是在内网，那就不行了。
<a name="V1UM9"></a>
## 四、网络服务
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311167931-070abf57-60a7-4345-b0ed-f8d005e3f84a.png#clientId=u3c34c1cf-dde8-4&from=paste&id=udf0030b8&originHeight=432&originWidth=443&originalType=url&ratio=1&status=done&style=none&taskId=ue92e99b9-3e52-438f-b398-65af97f33f8)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515182830.png)
<a name="t5NAZ"></a>
### WDSL语言
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311167848-c0db3ac7-9097-4550-abc3-7001db3b1830.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u37eb83c4&originHeight=320&originWidth=1224&originalType=url&ratio=1&status=done&style=none&taskId=u759017ad-402c-437d-ba36-6da7be4b12b)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515183154.png)
<a name="Cz9Hr"></a>
#### ①API接口测试
xz.aliyun.com/t/2412 
<a name="owBuJ"></a>
### AWVS扫描
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311167962-65449470-bd6d-4dd7-b6df-1602ccb6c8ec.png#clientId=u3c34c1cf-dde8-4&from=paste&id=ub07c0f17&originHeight=605&originWidth=1227&originalType=url&ratio=1&status=done&style=none&taskId=u1ff19d78-b441-4bbb-a8ce-bd94f9fb187)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184020.png)<br />扫描出来的结果又SQL注入漏洞<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311168635-01ad3918-5312-42cd-9cc0-7b747b0e149c.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u9fb3a5e5&originHeight=588&originWidth=1297&originalType=url&ratio=1&status=done&style=none&taskId=u3b703d16-940b-4699-b9ab-68f4b7dc739)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184301.png)<br />把数据包复制一下，在sqlmap安装目录，新建一个文档<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311168826-1a3dfc39-b7c4-4946-8cae-7466f2ceb241.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u11b99b49&originHeight=324&originWidth=633&originalType=url&ratio=1&status=done&style=none&taskId=u4e4a36a1-bc9b-4c7d-8f4f-840cd4c0cd9)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184337.png)<br />将数据包复制进去。<br />但是注意要把测试语句给删掉。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311168821-fb32d0d2-af17-4ddc-bbdb-cd4b77a8abd1.png#clientId=u3c34c1cf-dde8-4&from=paste&id=udf3f752a&originHeight=617&originWidth=960&originalType=url&ratio=1&status=done&style=none&taskId=ud5482eb7-08f3-45c2-8d34-636596f8753)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184410.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311168815-52c8fbc5-e9b4-4cbd-96ed-becbfb5f499f.png#clientId=u3c34c1cf-dde8-4&from=paste&id=ua6787984&originHeight=303&originWidth=783&originalType=url&ratio=1&status=done&style=none&taskId=u27626fd8-9c04-44dc-8bd9-5974e505564)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184456.png)<br />sqlmap<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311168947-2c1cfccd-7d75-4e49-8cba-151c1b3a2f13.png#clientId=u3c34c1cf-dde8-4&from=paste&id=udb4f4dd8&originHeight=168&originWidth=715&originalType=url&ratio=1&status=done&style=none&taskId=u1c1639cd-5c64-4db6-9e54-0b8638ad732)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184532.png)<br />--batch就是遇到yes or no的时候全部yes<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311169626-9a0076fd-a6db-4b9b-8a05-724f87885666.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u94371b45&originHeight=389&originWidth=1041&originalType=url&ratio=1&status=done&style=none&taskId=u6d8ea79c-04dc-43b7-a728-dc5700e8bd7)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184646.png)<br />列出表单<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311169644-cf5c7454-cf9d-4f92-a4a2-32664576ccb2.png#clientId=u3c34c1cf-dde8-4&from=paste&id=u8531513e&originHeight=70&originWidth=793&originalType=url&ratio=1&status=done&style=none&taskId=ub852b963-287e-4f1d-b02e-d7738d5b14a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184706.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630311169660-6bfe4540-2f11-4963-a6a4-14e3f27da033.png#clientId=u3c34c1cf-dde8-4&from=paste&id=ufc4fe4aa&originHeight=302&originWidth=807&originalType=url&ratio=1&status=done&style=none&taskId=uc07cd3d1-6eec-4bb1-a7b3-a89725e19bf)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515184718.png)
