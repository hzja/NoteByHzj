Java SpringBoot HTTPS
<a name="BI4AL"></a>
### 第一步、申请SSL证书
这里选了免费的 [https://freessl.cn/](https://freessl.cn/) ，大家可以根据自己需求进行。<br />这里选的 离线生成，需要解析验证域名，阿里云用户可选一键生成，但需要提供授权码，不推荐亚洲诚信不支持小程序！！只支持电脑！！别申请错了<br />PS：需要安装他要求的一个客户端软件，根据提示解析域名就会自动成功。<br />如果一直验证加载没反应就改用【一键申请】之后选择【我不是阿里云用户】，就会从网页验证解析改成软件验证，可秒过）<br />![2022-03-26-18-26-22-999777.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290478882-ddc9398a-fea2-4fe6-ad15-a64478a274a4.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=ui&id=u45bfce9e&originHeight=406&originWidth=914&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1115532&status=error&style=shadow&taskId=uc9682783-b684-4b32-a165-be60ee29a56&title=)
<a name="c8kgX"></a>
### 第二步、导出证书
每个平台导出方法不一样
<a name="kKIQC"></a>
#### 1. 通过这个网站软件导出证书
![2022-03-26-18-26-23-496466.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290491457-44192ae5-2e13-458a-be21-06be1f4b66a8.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=ui&id=u18dd6042&originHeight=283&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=918754&status=error&style=shadow&taskId=u2453c7f9-622f-41e0-9424-bfd03433acf&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290182096-c3a53e83-6705-4f73-864b-a5cf578e4f2a.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=paste&id=ua740219c&originHeight=203&originWidth=228&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=none&taskId=ud24a57f1-53ce-41e5-916e-e134d4b5b3f&title=)<br />![2022-03-26-18-26-24-221756.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290493919-667f0e37-1e53-47b1-b3eb-2228ffbe930c.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=ui&id=uf3b3a8ea&originHeight=468&originWidth=538&originalType=binary&ratio=1&rotation=0&showTitle=false&size=757123&status=error&style=shadow&taskId=uec0b0abf-76f7-4a7c-96b4-6ad7b750dfa&title=)<br />他会自动下载一个zip压缩包：<br />打开 iis 的（不是tomcat文件夹，目前阿里云等SSL申请渠道都只给pfx证书，使用jks证书可能到期后更改麻烦）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290182148-a6cc41ce-5f41-49bf-bd76-06ebd11c930c.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=paste&id=u4f7294e7&originHeight=142&originWidth=309&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=u55e1d3c9-987a-4c02-9caf-68fb4132b1f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290182761-285d2718-2acd-42ad-895f-249e5193101c.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=paste&id=u1d130251&originHeight=178&originWidth=467&originalType=url&ratio=1&rotation=0&showTitle=false&status=error&style=shadow&taskId=uc29af855-f580-4585-9667-a8f091d1a02&title=)<br />![2022-03-26-18-26-25-398716.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648290502788-a0f95549-6b81-40ed-a362-2014282870fb.png#clientId=ub3019ed1-2ffe-4&errorMessage=unknown%20error&from=ui&id=ud038ef36&originHeight=318&originWidth=972&originalType=binary&ratio=1&rotation=0&showTitle=false&size=929186&status=error&style=shadow&taskId=uf19c4c9b-2110-404a-899c-e0b0fcd032c&title=)
<a name="FUj6m"></a>
### 第三步、配置的代码
```yaml
server:
  port: 443
  ssl:
    key-store: classpath:aaaaa.pfx
    key-store-password: yourpassword
    keyStoreType: PKCS12
```
密码就是 导出证书 第五步 随便写的那个密码，对应上就行写啥无所谓没要求！文件名也是对应上就行没要求！
