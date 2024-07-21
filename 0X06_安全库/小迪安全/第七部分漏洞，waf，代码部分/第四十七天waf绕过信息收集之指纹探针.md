<a name="k2hX1"></a>
## 一、思维导图
![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630374002096-80192c51-03ee-4cda-9c9a-5e4e2b0d3e70.png#clientId=u4dc60ef4-93fd-4&from=paste&id=u5b82cda0&originHeight=526&originWidth=1001&originalType=url&ratio=1&status=done&style=none&taskId=u1ee31e5a-c560-4d6a-855e-2b66bf94093)
<a name="sRqXm"></a>
## 二、案例演示目录
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482238977-f9fe21c7-5a6c-4cf7-ad67-86047aae9644.png#clientId=u64bde69a-9703-4&from=paste&id=u0778ace0&originHeight=388&originWidth=746&originalType=url&ratio=1&status=done&style=none&taskId=u938cea17-45a6-4b37-a6ee-0af013cc03b)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515225527.png)
<a name="GMDpF"></a>
## 三、代理池Proxy_pool项目搭建及使用解释
<a name="AnkIy"></a>
### 1、下载地址
先要安装Redis数据库，教程b站上有，可以参考一下。<br />https://github.com/jhao104/proxy_pool 安装依赖： pip install -r requirements.txt 
<a name="a7CMj"></a>
## 四、充钱代理池直接干safedog+BT+Aliyun探针
<a name="hlour"></a>
### 1、快代理
https://www.kuaidaili.com/ <br />买这种隧道代理的<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482237841-72c0f5dc-3689-4774-b83f-a4242d42ed75.png#clientId=u64bde69a-9703-4&from=paste&id=u8b882055&originHeight=545&originWidth=1348&originalType=url&ratio=1&status=done&style=none&taskId=u7bbdf944-9043-459d-8d4c-64a9d26e5a1)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515231634.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482237977-6517bc34-3be7-408c-830e-00a444725b7f.png#clientId=u64bde69a-9703-4&from=paste&id=u13b3824f&originHeight=612&originWidth=1170&originalType=url&ratio=1&status=done&style=none&taskId=u17155788-829b-4983-9503-5f9c50324d4)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515231726.png)<br />配置到脚本中就可以了：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482237943-4b8c8746-40dd-4523-a77d-0d105b5414be.png#clientId=u64bde69a-9703-4&from=paste&id=ue0bf37d5&originHeight=364&originWidth=859&originalType=url&ratio=1&status=done&style=none&taskId=u8cc28e99-01be-44a3-9e42-974e8801a2c)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515231811.png)<br />效果很好：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482237988-8489cf35-29e6-4c12-aeac-8936fe41da3f.png#clientId=u64bde69a-9703-4&from=paste&id=u5ef84bf4&originHeight=722&originWidth=1329&originalType=url&ratio=1&status=done&style=none&taskId=u7cb6a60c-3114-4904-b6dc-6868135518a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515231920.png)<br />查看宝塔日志：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482239054-6e41050e-ce4f-4654-9c38-708264a5ec63.png#clientId=u64bde69a-9703-4&from=paste&id=uddf5c963&originHeight=455&originWidth=719&originalType=url&ratio=1&status=done&style=none&taskId=u0665eb52-34bd-40b6-afb6-75bcecae830)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515231956.png)<br />拦截了别的ip hhh
<a name="wMPI6"></a>
## 五、Safedog-Awvs漏扫注入测试绕过-延时，白名单
<a name="bdTix"></a>
### 1、本地测试
本地测试sqlilabs，并且本地的安全狗也是开启状态，cc防护也开了。<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482239119-dadee2ec-20ca-4628-bbd0-558b049937e8.png#clientId=u64bde69a-9703-4&from=paste&id=u15fea7c9&originHeight=372&originWidth=1017&originalType=url&ratio=1&status=done&style=none&taskId=u01a937b8-3fd0-4558-b60b-688bfeedf42)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515232432.png)<br />直接开扫：<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482239976-975164ce-fccc-4996-94c1-bbd0abd56a23.png#clientId=u64bde69a-9703-4&from=paste&id=ud44e3a6b&originHeight=527&originWidth=1026&originalType=url&ratio=1&status=done&style=none&taskId=ude6f8a81-50cb-4234-80e1-370a7be8ee6)](https://gitee.com/darkerg/article-images/raw/master/typora/20210515232523.png)<br />速度很快会被安全狗拦截。可以修改User-Agent,也可以用bp拦截，然后通过按键精灵自己控制速度。
<a name="Q3V5w"></a>
## 六、BT（baota）-awvs+xray漏扫Payload绕过
<a name="d5dw4"></a>
### 1、awvs配置
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482239222-69303a0e-5820-4e51-ac22-690f69ac8e3a.png#clientId=u64bde69a-9703-4&from=paste&id=u45f236cd&originHeight=249&originWidth=750&originalType=url&ratio=1&status=done&style=none&taskId=uafe00455-4437-480f-80b6-7867f60f02a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516161713.png)
<a name="s519y"></a>
### 2、转发
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482241046-00855bc0-0809-401c-b7cb-fda874d114db.png#clientId=u64bde69a-9703-4&from=paste&id=u366fbbc6&originHeight=402&originWidth=620&originalType=url&ratio=1&status=done&style=none&taskId=ub9279379-0cf4-465f-bc99-7d7af80755d)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516161756.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482240549-cb2fc0fc-96dd-4271-9419-1cca10faa724.png#clientId=u64bde69a-9703-4&from=paste&id=u0c69357c&originHeight=180&originWidth=946&originalType=url&ratio=1&status=done&style=none&taskId=u80edc45d-9896-45aa-b612-1d98cc7189a)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516161900.png)<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482240653-1e77df15-c5d6-4cf8-b9cc-82415ad7132a.png#clientId=u64bde69a-9703-4&from=paste&id=u6cadfc3c&originHeight=281&originWidth=595&originalType=url&ratio=1&status=done&style=none&taskId=ub8ea8e92-d6bf-4f1d-a592-331bf15f90f)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516161911.png)<br />awvs转发给burp suite 然后再转发给xray。
<a name="e32Tw"></a>
## 七、充钱上代理池干
[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482240624-5ebc00b6-7dd7-4811-ab0c-adfa3a398eb7.png#clientId=u64bde69a-9703-4&from=paste&id=ua00e07db&originHeight=514&originWidth=770&originalType=url&ratio=1&status=done&style=none&taskId=u62459579-bc27-44ca-974b-4c6023c16d6)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516162211.png)<br />成功扫到了，迪哥讲课真有趣<br />[![](https://cdn.nlark.com/yuque/0/2021/png/2476579/1630482241065-16550052-4742-45db-a5bb-7c5e7246855c.png#clientId=u64bde69a-9703-4&from=paste&id=u57d94653&originHeight=583&originWidth=911&originalType=url&ratio=1&status=done&style=none&taskId=uc2b27af2-8db6-4e1f-8072-04b60e064ce)](https://gitee.com/darkerg/article-images/raw/master/typora/20210516162439.png)
