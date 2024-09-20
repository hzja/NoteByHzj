分布式 分布式任务调度<br />分布式任务调度项目xxl-job的官网：[https://www.xuxueli.com/xxl-job/](https://www.xuxueli.com/xxl-job/)
<a name="gqSp6"></a>
### 1、从Github下载源码
Github仓库地址：[https://github.com/xuxueli/xxl-job](https://github.com/xuxueli/xxl-job)<br />选择自己习惯的方式进行下载，克隆或者下载zip压缩包<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600568334559-ca3e5c56-e1f0-4daf-a36a-ace69ad26653.png#averageHue=%23555555&height=162&id=c01ax&originHeight=487&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=537109&status=done&style=none&title=&width=1107.6666666666667)
<a name="8ZiMl"></a>
### 2、使用项目开发工具（IDEA）导入项目
<a name="x52hT"></a>
### 3、将项目目录下的doc/db/tables_xxl_job.sql数据库文件导入本地数据库
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600570358578-cc934ac7-fe50-4a79-8a4c-6fc377eed48d.png#averageHue=%233c3c3c&height=583&id=VEZGZ&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2013622&status=done&style=none&title=&width=1107.6666666666667)
<a name="5UkWV"></a>
### 4、修改 `xxl-job-admin` 项目配置文件中的数据库配置信息
配置为当前的本地数据库<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600570560312-a1170803-578e-4518-a51e-dd33365ad87e.png#averageHue=%23f8f8f7&height=687&id=gLCnT&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=951080&status=done&style=shadow&title=&width=1280)
<a name="kp0An"></a>
### 5、启动xxl-job-admin项目
根据日志可以看到项目在8080端口启动成功<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600570817133-2874f997-f50e-434a-ab5f-8a669eb30cf9.png#averageHue=%23f5f4f3&height=687&id=Ak33z&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=928244&status=done&style=shadow&title=&width=1280)
<a name="fd38b99c"></a>
### 6、访问任务调度管理页面
浏览器访问URL：[http://localhost:8080/xxl-job-admin/](http://localhost:8080/xxl-job-admin/toLogin)<br />进入登录界面，帐号默认 `admin` ，密码默认 `123456` ；<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600570894943-112535d9-c1d2-4db2-b133-040d9e316831.png#averageHue=%23c8d4dc&height=686&id=aKATo&originHeight=2059&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=376272&status=done&style=shadow&title=&width=1280)<br />登录成功即可看到管理页面<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600571008292-cbe9c4ef-4e74-40dc-8fe5-904a4f9e095d.png#averageHue=%23e1dda5&height=687&id=c2Wll&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=469023&status=done&style=shadow&title=&width=1280)
<a name="8Q2pS"></a>
### 7、任务调度使用测试
测试案例使用源码下的案例<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600571772247-89e2d031-5bf8-48a4-8438-d825f52ebd91.png#averageHue=%23f7f7f5&height=687&id=sq4Ft&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=814972&status=done&style=shadow&title=&width=1280)<br />启动测试案例项目<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572764215-70df438a-ba55-4d70-9bc9-9dcad866471a.png#averageHue=%23f5f4f3&height=687&id=wMSij&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=903784&status=done&style=shadow&title=&width=1280)
<a name="Jj4rr"></a>
#### 添加执行器
:::info
AppName需要和执行器代码配置中的**xxl.job.executor.appname值保持一致**
:::
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600571717155-fc09f506-1ff6-46b3-ac05-fc8d7b39d853.png#averageHue=%2352a25b&height=687&id=DR215&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=677994&status=done&style=shadow&title=&width=1280)
<a name="N9ofI"></a>
#### 添加任务
:::info
**JobHandler需要和**`**@XxlJob**`**注解中的值一样**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572027803-b9f2b7c5-5a5f-4f52-a549-ace6d5b4ee08.png#averageHue=%23f7f6f5&height=687&id=r7pW5&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=970249&status=done&style=none&title=&width=1280)
:::
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572218286-9bad066e-00c4-4e34-8a6d-a0fd114fcf48.png#averageHue=%23b5d27d&height=687&id=QuKNc&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=725129&status=done&style=none&title=&width=1280)
<a name="SUt1R"></a>
#### 执行任务
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572820495-cee709e0-4c09-4fb9-bc81-d2056a4dfec2.png#averageHue=%2338af67&height=686&id=sHr8b&originHeight=2057&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=603069&status=done&style=shadow&title=&width=1280)
<a name="J7rhC"></a>
#### 查看任务执行日志
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572875655-0c8fb9ed-c8bc-4074-adfa-9e55ca251e8d.png#averageHue=%2374b964&height=687&id=IH0eN&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=584657&status=done&style=shadow&title=&width=1280)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1600572908938-5b60fd38-2d88-424c-af4f-a930360fe1f6.png#averageHue=%23dfc290&height=687&id=VmOcx&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=466241&status=done&style=shadow&title=&width=1280)
