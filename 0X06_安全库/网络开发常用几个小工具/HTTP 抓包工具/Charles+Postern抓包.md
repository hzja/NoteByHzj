Charles
<a name="boQm6"></a>
## 为什么要说使用Charles+Postern
在实际抓手机App包场景中，有很多种方案，经典的就是Fiddler，但是Fiddler会有一个问题，如果App设置了不走代理这个选项，那Fiddler就不行了。<br />为什么Charles+Postern可以呢？<br />是因为Charles就没有直接监听到App，Charles是监听到了Postern上，Postern就是一个vpn，所以App设置不走代理也没用，Postern照样能监听到，然后Postern再转发到Charles上，这样就完成了抓包。<br />但是双向验证除外，双向验证Charles需要添加证书，后面再说。<br />当然，还有更好的方案，**r0capture** hook抓包了解一下？
<a name="Ym1nb"></a>
## 环境
pixel 2 Android 8版本(已root)<br />Magisk 23.0版本<br />Xposed 3.1.5版本
<a name="QeFN7"></a>
## Charles配置
<a name="ryBsG"></a>
### Charles环境要求

- **JDK**
- **启用超级管理员**
- **关闭防火墙**
<a name="eeRxP"></a>
### 下载
Charles下载地址：[https://www.charlesproxy.com/download/](https://www.charlesproxy.com/download/)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470256-be6e7aed-a1b2-451c-a238-90911cc6c4f7.png#clientId=u88eca28b-bab0-4&from=paste&id=u7702f30a&originHeight=713&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue52238d0-58cd-48af-85fc-fce17fffe3d&title=)
<a name="QNRes"></a>
### 安装
下一步->下一步即可<br />安装成功打开
<a name="MgRkv"></a>
### 激活
注册码生成地址：[https://www.zzzmode.com/mytools/charles/](https://www.zzzmode.com/mytools/charles/)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470289-8e43e074-4d30-4af5-ad14-c502fc5c5854.png#clientId=u88eca28b-bab0-4&from=paste&id=u7c63d7c0&originHeight=408&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uaa4aebc2-f083-49c3-a8b1-84b3b890a4c&title=)<br />激活Charles<br />在Help->Registered输入key和生成的激活码即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470328-63c3bb44-a9c8-403a-b312-05b7b26249e9.png#clientId=u88eca28b-bab0-4&from=paste&id=u4a116da0&originHeight=523&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6cf73186-7460-4c90-8aa8-36cb2706ef0&title=)
<a name="wdXBt"></a>
### 设置代理
点击Porxy Settings<br />注意：此时我的Windows Proxy是去掉对勾的，表示不抓PC端。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470322-b1b6e8fd-57de-4fb3-9bdd-db48739b6d5e.png#clientId=u88eca28b-bab0-4&from=paste&id=u4a9f1f71&originHeight=719&originWidth=953&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u768652fc-099f-42de-866e-4a285648a81&title=)<br />设置监听端口<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470705-dab848c0-0438-49f3-8ab2-d1a206f528a2.png#clientId=u88eca28b-bab0-4&from=paste&id=u73eb4525&originHeight=648&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udc7b5b89-2507-4935-94e5-24cb3018eab&title=)
<a name="i22ga"></a>
### 配置SSL代理
点击SSL Proxying Settings<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470743-061e41dd-eb11-4d06-9cc4-ec04ee94da8b.png#clientId=u88eca28b-bab0-4&from=paste&id=udc015115&originHeight=635&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u150867eb-e2d8-47a5-956b-73ada0dbfcd&title=)<br />添加代理，`*.*`即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470700-0adfd7fe-186b-4fec-92de-c8d28ad9c87b.png#clientId=u88eca28b-bab0-4&from=paste&id=u5f623632&originHeight=656&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7d5f94be-91bc-477f-9f03-eba43ea79b1&title=)<br />到现在为止，Clarles就设置完毕。
<a name="OPhkf"></a>
## Postern
<a name="kb5S5"></a>
### Postern简介
Postern并不是抓包工具，它是一个代理工具。<br />它可以将http请求转为socket，并且包转发到Charles上，这样就可以抓到更多的包。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200470781-9e50b706-664f-483d-a4db-eaa412263950.png#clientId=u88eca28b-bab0-4&from=paste&id=ueb7a5f90&originHeight=391&originWidth=350&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4b353423-e63e-45f4-8f37-2ca3ef061bc&title=)
<a name="tmTvw"></a>
### 配置代理规则
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471023-d5b34182-30ff-4470-83ba-6e496fe94ea7.png#clientId=u88eca28b-bab0-4&from=paste&id=ubb29c82b&originHeight=537&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3cb5c287-592f-4cd1-8fa1-4f7f6d9a37e&title=)
<a name="N99GS"></a>
#### 添加代理服务器
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471113-b93108fe-03b4-4198-9278-766c50961d4b.png#clientId=u88eca28b-bab0-4&from=paste&id=u68eb3b58&originHeight=435&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue01d9e86-7e9b-4f66-bf8a-25f90164281&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471241-6128c862-af72-49fe-bc22-3f850c86701a.png#clientId=u88eca28b-bab0-4&from=paste&id=u66c24e0c&originHeight=563&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf60c3dfe-e219-4804-9fbc-fa24f2e1af8&title=)<br />留下一个选项就行，其它的删掉即可
<a name="g6PJR"></a>
### 配置规则
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471265-6318f61a-1be2-4008-98cf-7e3ae1022fcb.png#clientId=u88eca28b-bab0-4&from=paste&id=uc6773f11&originHeight=511&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7b358ecc-8303-4025-9730-f567387061c&title=)
<a name="J7PCr"></a>
#### 添加规则
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471292-2bff4d48-eb8b-4bbb-b35d-668f0db519a4.png#clientId=u88eca28b-bab0-4&from=paste&id=u86db927e&originHeight=196&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2eb7e7e0-9289-4c91-b184-1f11e6bcdc3&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471403-4eb48c31-14f2-48be-9070-2c73ba60f7aa.png#clientId=u88eca28b-bab0-4&from=paste&id=ud318402b&originHeight=508&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uee5d5bbc-2271-4af1-bac8-0937b79ea83&title=)
<a name="Ee8CA"></a>
### 打开/关闭Postern
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471558-074d42d7-d1b4-4d7f-a21b-0c2dbdbba90d.png#clientId=u88eca28b-bab0-4&from=paste&id=u3a1e1b08&originHeight=495&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u87a2eab9-8b1f-40ff-bf6d-dde2f366873&title=)<br />如果Charles有提示出东西，一定要点击Allow<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471580-1b3f83eb-5e8e-4dac-932b-e9d3e29de3b8.png#clientId=u88eca28b-bab0-4&from=paste&id=ud1afe61c&originHeight=593&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ube02d33f-fefc-4814-bf3b-c3eedb41856&title=)<br />然后就可以愉快的抓包了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471721-60c8e9aa-95ce-4397-be9d-2ebc8cba4712.png#clientId=u88eca28b-bab0-4&from=paste&id=u9980aa64&originHeight=699&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u44349a03-02e9-46b0-b9c3-cd7bd0157b2&title=)<br />但是此时只能抓http的包，要想抓hppts的包，还需要设置证书才行。
<a name="PSTQg"></a>
## 配置证书
<a name="NGe3f"></a>
### 保存证书到指定位置
点击Save Charles Root Certi...<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471787-08c56834-a7f2-4e86-8b05-c99fdc8537c7.png#clientId=u88eca28b-bab0-4&from=paste&id=u27bf6a4e&originHeight=477&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc2cf6208-3183-40e0-afef-53cb236f547&title=)<br />选择一个路径保存<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471817-a6e3ed86-8500-4380-a5aa-861d484a1b4f.png#clientId=u88eca28b-bab0-4&from=paste&id=u9c98e069&originHeight=607&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u13856437-6b50-4ca2-8d1a-2794948d74c&title=)
<a name="ANZut"></a>
### 证书推送到手机上
将保存的证书推送到手机上<br />adb push C:\Users\Ti\Downloads\1.pem /sdcard/1.pem

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471845-39eff2f7-4c51-454e-a575-597b8ddb231a.png#clientId=u88eca28b-bab0-4&from=paste&id=u360d69e1&originHeight=395&originWidth=961&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1df5522e-7473-4161-a8d9-97a000c5a09&title=)<br />成功推送到手机上<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200471920-d8aa03f1-d582-49c2-95f1-dad8b68d400d.png#clientId=u88eca28b-bab0-4&from=paste&id=u28ca4db6&originHeight=619&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u22b42719-6d3e-4e63-aaa4-12615310d99&title=)
<a name="qYHKV"></a>
### 手机端安装证书
点击从存储设备安装证书<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472238-6f8565ed-ecdf-4d08-aa33-8fdaef2f7e2f.png#clientId=u88eca28b-bab0-4&from=paste&id=u3dacca39&originHeight=605&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue9f3b6fd-049a-444e-92d1-70afd2f3f15&title=)<br />点击证书，安装<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472241-3e0bfdbb-d56d-476f-b735-a9a251ea20bf.png#clientId=u88eca28b-bab0-4&from=paste&id=u52f2962b&originHeight=647&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4642620d-0f2d-486c-afde-000df4fc6c2&title=)<br />证书名字随便写<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472302-f7b404e2-0306-4e02-93c4-6b8abe9477b8.png#clientId=u88eca28b-bab0-4&from=paste&id=u3aae1e19&originHeight=381&originWidth=388&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udde56622-5f8c-4a0e-867e-11ca269bd3b&title=)<br />安装好之后是可以在信任的凭据中看到的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472346-cb84fe02-5393-470f-a74a-eb06f440f6ce.png#clientId=u88eca28b-bab0-4&from=paste&id=u1fc98553&originHeight=562&originWidth=387&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d323238-cb40-4790-b878-f4010e7ee68&title=)<br />查看用户证书<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472413-dd3c6eb7-8629-4f82-af22-a2b281bdd233.png#clientId=u88eca28b-bab0-4&from=paste&id=u586a1017&originHeight=246&originWidth=431&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0abffbf5-918d-4fc8-91eb-66829d23004&title=)
<a name="prCsc"></a>
### 证书存放目录
系统证书路径 /etc/security/cacerts<br />用户证书路径 /data/misc/user/0/cacerts-added

虽然此时证书已经安装完成，但是可能对APP抓https还是失败的。<br />因为Android7以后，只信任系统证书，所以还需要将用户证书移动成系统证书。
<a name="UA1Su"></a>
### 移动证书
这里使用Magisk的Move Certificates模块来安装证书<br />直接安装即可<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472651-54019ec3-9dad-4f8b-b688-ad45378d3573.png#clientId=u88eca28b-bab0-4&from=paste&id=ub4ce4468&originHeight=743&originWidth=418&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u36a392d7-66fc-4a04-ae36-d20e43ef95e&title=)<br />然后重启手机，你就会发现证书移动成功<br />现在，到现在就配置成功了，打开Postern，就可以开心的抓https啦！<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650200472676-fc071869-db9f-4397-8d7a-375a23064891.png#clientId=u88eca28b-bab0-4&from=paste&id=uad1a94aa&originHeight=592&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u58347e52-ffe3-427a-aa8d-82d467d552f&title=)
<a name="x3tmK"></a>
## Charles和Fiddler的比较
Fiddler虽然也可以完成对手机的抓包，但是如果App设置了不走代理，那Fiddler就失败了。<br />并且Fiddler需要修改手动wifi的代理，但是Charles就不需要，只需要配置好Postern和Charles的连接就好，并且可以对绝大部分的App进行抓包，不使用代理时，直接关闭Postern即可！
<a name="S6YnQ"></a>
## 总结
目前来说，Fiddler和Charles都是很流行的，但是相对来说，还是Charles+Postern适用范围更广一些，并且数据更好看一点！
