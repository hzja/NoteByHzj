<a name="lOGIo"></a>
## cve-2021-22005-exp-main
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641798772918-5be79d0b-1581-4815-9d19-32e8e6450852.png#clientId=u2917c425-ea13-4&from=paste&height=72&id=u67cefc68&originHeight=151&originWidth=1208&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25260&status=done&style=none&taskId=ucc748c28-82d9-4319-9e85-8e990aeed32&title=&width=573.4000244140625)<br />webshell物理路径如下，但是访问会跳转。<br /> /usr/lib/vmware-sso/vmware-sts/webapps/ROOT/n2qhaf.jsp  

**不需要权限校验的路径**<br />C:\ProgramData\VMware\vCenterServer\data\perfcharts\tc-instance\webapps\statsreport\

对应的webshell<br />http://XXXX/statsreport/XXXSHELL.jsp


[https://vcenterserver.scmcc.com.cn/websso/resources/js/assets/csd_api_common.js](https://vcenterserver.scmcc.com.cn/websso/resources/js/assets/csd_api_common.js)
```java
/usr/lib/vmware-sso/vmware-sts/webapps/ROOT/resources/js/assets/csd_api_common.js
```
vmware-vsphere-client (不要用这个，这个是客户端的)
<a name="t1XYh"></a>
## cve-2021-21985
不出网，通过10.102.7.10服务器搭建jndi服务打cve-2021-21985<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641799356794-83c7a696-3cd9-47a5-90d7-1abf29f70829.png#clientId=u2917c425-ea13-4&from=paste&height=71&id=u077e4e71&originHeight=141&originWidth=834&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14018&status=done&style=none&taskId=ueaeb11f3-4e1f-4180-b37f-84f21e88336&title=&width=417)

打目标机器<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641740900338-57513134-fea8-4a20-9357-7e477d8dcfa5.png#clientId=uc037c156-f11a-4&from=paste&height=237&id=u3cc3e82f&originHeight=473&originWidth=1263&originalType=binary&ratio=1&rotation=0&showTitle=false&size=46349&status=done&style=none&taskId=u5b5879ea-8458-4838-a332-3cacef81fe2&title=&width=631.5)<br />通过10.102.7.10服务器反弹shell获取[https://10.102.7.2/](https://10.102.7.2/)  vmcenter集群权限<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641741038810-4b2f125b-ac56-4f96-b47b-27dd4c642063.png#clientId=uc037c156-f11a-4&from=paste&height=477&id=ub2c6f458&originHeight=606&originWidth=832&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35716&status=done&style=none&taskId=u0101a839-ac42-41bd-adf9-858de060afc&title=&width=655)<br />读取文件破解出vmcenter控制台的cookie<br /> /storage/db/vmware-vmdir/data.mdb<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1641741127245-cf9687d2-f996-49a1-aa25-cb0383597abe.png#clientId=uc037c156-f11a-4&from=paste&height=204&id=ub5c79ec9&originHeight=369&originWidth=1210&originalType=binary&ratio=1&rotation=0&showTitle=false&size=60138&status=done&style=none&taskId=u9eb57c60-a482-42c4-af62-4d9d8dbc491&title=&width=669)
