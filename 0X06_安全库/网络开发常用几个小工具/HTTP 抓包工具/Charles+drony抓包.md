Charles
<a name="dzfJF"></a>
## 为什么使用Charles+Drony来抓包?
在对app进行抓包时，其实用什么方案并不是唯一的，有Fiddler，也有Charles的，也有所说的这种方式，Charles+VPN的方式(这里的vpn指的是像Drony来进行转发的一类app)<br />方式并不是唯一的，适合的就是最好的！<br />之所以采用Charles+VPN方式，是因为现在的app很大可能会进行wifi代理检测！<br />而vpn转发请求和wifi代理检测没关系，所以并不太会受影响，所以现在这种方式越来越多！<br />当然，除了wifi代理检测，还有vpn转发检测，还有证书校验等，但是现在遇到最多的还是wifi代理检测！
<a name="kefJZ"></a>
## 工具下载
链接：https://pan.baidu.com/s/1TYelBGxO5n0sqVidN48djg <br />提取码：5c5u <br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200147773-f0e1d144-47a1-4cf1-bd32-3b3947ae612b.png#clientId=u0cbdd48f-4149-4&from=paste&id=u2933aa39&originHeight=313&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucc63b43d-3598-4349-aaa5-782eb9cc038&title=)
<a name="KkqI8"></a>
## 环境
pixel2   v10(已root)<br />Magisk   v23.0<br />Charles  v4.6.2<br />Drony    v1.3.154
<a name="ueFye"></a>
## Charles配置
<a name="UI3kI"></a>
### Charles环境要求

- **JDK**
- **启用超级管理员**
- **关闭防火墙**
<a name="WeV0H"></a>
### 下载
Charles下载地址：[https://www.charlesproxy.com/download/](https://www.charlesproxy.com/download/)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200147714-aa8877f5-c7fa-4a84-bbcb-3a2699a5c175.png#clientId=u0cbdd48f-4149-4&from=paste&id=ubdb2b04a&originHeight=713&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u76477583-0b62-4f9f-a13e-77a2e9580b8&title=)
<a name="UbM2k"></a>
### 安装
下一步->下一步即可<br />安装成功打开
<a name="AanAJ"></a>
### 激活
注册码生成地址：[https://www.zzzmode.com/mytools/charles/](https://www.zzzmode.com/mytools/charles/)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200147735-ac98cf40-0a84-472a-8df9-8bd319b68d58.png#clientId=u0cbdd48f-4149-4&from=paste&id=uec03039d&originHeight=408&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u40d972b6-458b-4453-a9d7-6892eb95608&title=)<br />激活Charles<br />在Help->Registered输入key和生成的激活码即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200147827-604193e3-3765-49f1-b94c-4102d649380d.png#clientId=u0cbdd48f-4149-4&from=paste&id=u74048608&originHeight=523&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u765825e0-96da-406d-bae4-0350df544c8&title=)
<a name="LLcZB"></a>
### 设置代理
点击Porxy Settings<br />注意：此时Windows Proxy是去掉对勾的，表示不抓PC端。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148172-b3b854e9-9c65-4d0e-99b8-21ddc1b44463.png#clientId=u0cbdd48f-4149-4&from=paste&id=u2dfdc9f4&originHeight=719&originWidth=953&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ub91e33b0-b990-42b7-9740-ecae0196ff5&title=)<br />设置监听端口<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148187-4528a431-2477-4548-89e4-f66c6bf380d3.png#clientId=u0cbdd48f-4149-4&from=paste&id=u23520108&originHeight=647&originWidth=753&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u60cd399c-2060-4594-9226-e1cd9505825&title=)
<a name="rY1wo"></a>
### 配置SSL代理
点击SSL Proxying Settings<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148221-52c0a0a7-56ee-4529-a021-b0037159f538.png#clientId=u0cbdd48f-4149-4&from=paste&id=u9972a643&originHeight=635&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61a21249-d80b-4dfa-a214-52d885df427&title=)<br />添加代理，`*.*`即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148222-27b5984d-b727-415d-84fd-ae45067b84ae.png#clientId=u0cbdd48f-4149-4&from=paste&id=u52def693&originHeight=656&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6eac4274-6229-4f0a-83e4-c6e237b9951&title=)<br />到现在为止，Clarles就设置完毕<br />到此为止，Charles配置就算完成了，但是只能抓http的包，对https抓包的时候会发现，很多都是红色x号？如果要抓https，是需要继续配置证书的！！！
<a name="vft0D"></a>
## 配置证书
<a name="B9u9C"></a>
### 保存证书到指定位置
点击Save Charles Root Certi...<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148343-ee20bc2e-9728-46c1-84a7-4e4c039a5ccb.png#clientId=u0cbdd48f-4149-4&from=paste&id=u498908ab&originHeight=477&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucb1c0eb2-2aaf-4b37-803c-a3c18fe0fcf&title=)<br />选择一个路径保存<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148502-ef8a5a60-1901-4a9e-baee-54247611dbc5.png#clientId=u0cbdd48f-4149-4&from=paste&id=ud350a9c6&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uda02e602-76d2-4643-abbd-fe723700d6f&title=)
<a name="ePMu4"></a>
### 证书推送到手机上
将保存的证书推送到手机上
```bash
adb push C:\Users\Ti\Downloads\1.pem /sdcard/1.pem
```
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148517-35b8d5e6-c7be-489b-a8db-8a46fa529a20.png#clientId=u0cbdd48f-4149-4&from=paste&id=u336272e6&originHeight=395&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2917f134-4fe9-45aa-9ff9-a2bb48a05fd&title=)<br />成功推送到手机上<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148565-982d38e2-6fc1-4547-ab6d-e73aba4f4b3b.png#clientId=u0cbdd48f-4149-4&from=paste&id=u320dbdcd&originHeight=619&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueacc2d3d-7766-4821-8d3d-bd91bbe3619&title=)
<a name="JhXh5"></a>
### 手机端安装证书
点击从存储设备安装证书<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148588-0ba21ac0-5594-4588-91ce-f6914a2dbd9c.png#clientId=u0cbdd48f-4149-4&from=paste&id=uadce8f05&originHeight=605&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u304de715-0bc4-402c-a594-3973253327a&title=)<br />点击证书，安装<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148629-8b5d2176-254d-4644-bbd2-35065bf5ab54.png#clientId=u0cbdd48f-4149-4&from=paste&id=udae0cf0f&originHeight=647&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u239bf3c7-5e0d-466d-9690-5b2ad6bc175&title=)<br />证书名字随便写<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148823-862a7119-e061-4b4e-a1b5-76794d3c9fb6.png#clientId=u0cbdd48f-4149-4&from=paste&id=u6d25756f&originHeight=381&originWidth=388&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5a75b59a-2f54-4eb8-ba6c-84fa57037e8&title=)<br />安装好之后是可以在信任的凭据中看到的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148790-53ad92fb-fe83-4f34-8aa6-9ff3ba569f21.png#clientId=u0cbdd48f-4149-4&from=paste&id=ub2ce9593&originHeight=562&originWidth=387&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u208cf667-ec3c-46f0-b871-d1228bc3153&title=)<br />查看用户证书<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148948-b7ee2517-dfab-49c3-b4a2-f7218f79b97b.png#clientId=u0cbdd48f-4149-4&from=paste&id=ua22954c7&originHeight=246&originWidth=431&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud67f520f-cb5b-4483-97b0-47746ee290c&title=)
<a name="x2f0V"></a>
### 证书存放目录
> 系统证书路径 /etc/security/cacerts<br />用户证书路径 /data/misc/user/0/cacerts-added

虽然此时证书已经安装完成，但是可能对APP抓https还是失败的。<br />因为Android7以后，只信任系统证书，所以还需要将用户证书移动成系统证书。
<a name="qR5Td"></a>
### 移动证书
这里使用Magisk的Move Certificates模块来安装证书<br />直接安装即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148944-b22130d4-1ad8-451d-8822-c1021b2314eb.png#clientId=u0cbdd48f-4149-4&from=paste&id=u5d85b8c5&originHeight=743&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u152f415f-870a-403a-8d04-efeae9f7eba&title=)<br />然后重启手机，就会发现证书移动成功。
<a name="vVet7"></a>
## 某狗抓包
到现在为止还没用上drony，但是charles和证书都已经配置好了，对没有wifi代理检测的app是可以适用的！这里以某狗为例！
<a name="K9jwd"></a>
### 设置代理
设置wifi代理，填写正确的ip和端口，ip是电脑ip，端口上面配置过！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200148935-22c371f4-1b73-4820-9aad-57993dfeed46.png#clientId=u0cbdd48f-4149-4&from=paste&id=u7119fe7d&originHeight=517&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u69fc5edc-fd2f-4f48-ba97-98da793b4a0&title=)
<a name="eBu24"></a>
### 登录抓包
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149209-fd88aa1d-db92-4c41-ab74-ea0e343df672.png#clientId=u0cbdd48f-4149-4&from=paste&id=u09c5c012&originHeight=561&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf1621ef4-8ba3-4ffb-bfca-0fd439b9e6b&title=)<br />这样，就成功地抓出来登录的包了！
<a name="zG1TM"></a>
## 某物抓包
还是使用上面配置wifi代理的方式来进行对某物抓包！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149139-842f649e-18af-4d59-ad85-ca7e5af6dded.png#clientId=u0cbdd48f-4149-4&from=paste&id=u94daec2e&originHeight=538&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5ea964af-468c-49c4-a523-adda71adcd8&title=)<br />通过对抓的包研究发现，会找不到任何关于13苹果的信息和商品列表！！！<br />这什么情况？<br />这种情况水平人家可能是对wifi代理使用检测的！<br />不走代理，这样不就抓不到包了！
<a name="iNfLh"></a>
## 配置Drony
**PS**：如果打算使用drony的话，wifi代理就可以关了，wifi代理能干的事，drony都能干！<br />首先要安装drony，这就不用多说了吧！<br />安装完成之后，往左划一下进入设置，然后点击不是**无线网络...**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149216-b8fe5588-d5c2-4522-8e1d-70c4845ce523.png#clientId=u0cbdd48f-4149-4&from=paste&id=u17870fd7&originHeight=742&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uae8d9f0a-124f-4d29-ab46-e44df5f1b16&title=)
<a name="tvXbX"></a>
### 配置ip和端口
先设置**主机名**和**端口**，和wifi代理配置的是一样的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149225-88b42957-e5aa-40a9-9540-d5bfc3fae15f.png#clientId=u0cbdd48f-4149-4&from=paste&id=uc48ed0f7&originHeight=378&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uec8b7fd4-3626-4027-b56f-464ad021ab6&title=)
<a name="HftMW"></a>
### 配置过滤器
然后继续往下滑，滑到**过滤器**，选择这个**引导全部**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149355-4624ed9d-f9c3-46e6-a514-250a4325662c.png#clientId=u0cbdd48f-4149-4&from=paste&id=u6d3f27bd&originHeight=442&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u63750225-4cfa-4cca-974c-d9da0416fe0&title=)
<a name="G99ns"></a>
### 配置规则(添加要抓包的app)
然后点击**规则**<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149493-9b4ec178-9643-4f83-822a-428bd672d66d.png#clientId=u0cbdd48f-4149-4&from=paste&id=u6b5689d8&originHeight=410&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9211891-1c87-4105-a433-74719a5e10c&title=)<br />点击右上角+号<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149538-3950059a-3fac-41a9-a06d-5720e87513bf.png#clientId=u0cbdd48f-4149-4&from=paste&id=ub0b14913&originHeight=176&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5039be33-cd96-4ae5-a5f0-377177b04a3&title=)<br />添加一个app，行为是**允许所有**，然后点击右上角保存！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149581-c345f654-c224-4e15-b7a2-6454d3a0e525.png#clientId=u0cbdd48f-4149-4&from=paste&id=u22c3cf0b&originHeight=393&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9117ca35-62a6-4544-a9ec-86f783230c3&title=)<br />这样就出现在规则列表中了，并且只会抓这一个app的包。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149614-f2544e16-ced9-41ff-a6c8-ceb50242a6fd.png#clientId=u0cbdd48f-4149-4&from=paste&id=u8e046b85&originHeight=213&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud7f0a0d1-27df-41ca-b2a5-9d95bef6755&title=)<br />如果需要抓其他app，继续添加app即可！
<a name="C31Sq"></a>
### 启动drony
都配置完成后，回到主页面，点击：开，就可以对指定app进行转发了！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149731-cf193fac-5d8d-4c6e-9362-81318a02f6f0.png#clientId=u0cbdd48f-4149-4&from=paste&id=ub349fda1&originHeight=654&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4fa85346-2bf8-41ac-b03b-3292ba2d537&title=)
<a name="kmLLx"></a>
## 某物抓包2
如上都配置好之后再次抓包，就会发现多了很多数据包，并且找一下找到了想要的包和数据<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200149917-1e30b7c8-649e-4e02-bf44-d51cc6d43a60.png#clientId=u0cbdd48f-4149-4&from=paste&id=u842f7a26&originHeight=515&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0abb2ba7-af57-4f5d-902c-b7b85ff2b1b&title=)
<a name="BrvZK"></a>
## 总结
要是来说的话，使用charles+drony方式之后，其他什么方式都可以不用考虑了，这算是最靠谱的方案。
