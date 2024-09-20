IDEA Community版开源免费的。然而社区版却并不支持web开发。<br />这里介绍一下社区版配置Tomcat。
<a name="qebbC"></a>
#### ① 首先打开settings
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217176294-2e6d9a6c-931b-42a2-a92a-7ac6c40e48ea.png#averageHue=%23eaeae9&clientId=u537c4f22-698a-4&from=paste&height=515&id=u9704d8fe&originHeight=1287&originWidth=692&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=75296&status=done&style=shadow&taskId=u49de5d46-600d-4b6c-931a-51e52fe22fa&title=&width=276.8)
<a name="iyKCU"></a>
#### ② 找到plugins，在右边搜索Tomcat，找到Smart Tomcat，点击Install
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217236292-4289fe56-ab12-4a49-8387-9e78f43457ae.png#averageHue=%23aca461&clientId=u537c4f22-698a-4&from=paste&height=808&id=u8c7140d4&originHeight=2021&originWidth=2558&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=214517&status=done&style=shadow&taskId=uc86ceefd-4744-4a5d-961a-6e6f8c4dfa1&title=&width=1023.2)
<a name="lBa7D"></a>
#### ③ 可能会提示重启，不提示的话继续操作即可；重启好后，点击Edit Configurations；
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217298653-47967215-d939-40da-bef2-d47c21f3ea0b.png#averageHue=%23e8e8e7&clientId=u537c4f22-698a-4&from=paste&height=170&id=uf0291996&originHeight=424&originWidth=875&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=33078&status=done&style=shadow&taskId=uec04e557-16ea-4396-a029-8161363e293&title=&width=350)
<a name="HxLJ4"></a>
#### ④ 如果有Templates就展开，如果没有就选择左上角的加号；展开后点击Smart Tomcat 出现右边的界面即可；
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217597985-0f1d7784-9c17-43ad-a606-16e08908a795.png#averageHue=%23f5f4f3&clientId=u537c4f22-698a-4&from=paste&height=440&id=uc136075a&originHeight=1100&originWidth=625&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=81717&status=done&style=shadow&taskId=u2542e3c7-189d-4fcf-8482-865c5ab7361&title=&width=250)
<a name="M6qhr"></a>
#### ⑤ 配置Tomcat Server 和web项目，Tomcat Server选择对应Tomcat的根目录
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217826214-cad3a480-d168-46bc-bdf0-e4f9118b455c.png#averageHue=%23f4f4f4&clientId=u537c4f22-698a-4&from=paste&height=810&id=uc2bb5800&originHeight=2024&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=180975&status=done&style=shadow&taskId=ue05a2a36-2422-4dfa-ba32-ba29a203267&title=&width=1042)
<a name="Iz20r"></a>
#### ⑥ 点击Tomcat Server选项的Configuration，出现如下界面，点击 + ，选择Tomcat服务器，然后点击Apply
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217734589-dff0b8e7-3e31-4b17-ad43-def393701ab9.png#averageHue=%23cda870&clientId=u537c4f22-698a-4&from=paste&height=712&id=ub7c3805c&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=266729&status=done&style=shadow&taskId=ub82d6dc4-eee3-4f30-8e47-893393336d4&title=&width=951.2)
<a name="EVPe6"></a>
#### ⑦ 添加好之后如图所示，而Deploymnet选择需要启动的web项目，特别注意的是，这里选择web项目的webapps目录，同理如果是MyEclipse项目选择webContent目录，选择好了后，就可以启动项目了。
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681217949417-2bb82666-1355-4f7c-a871-510d71c07dbd.png#averageHue=%23f4f4f4&clientId=u537c4f22-698a-4&from=paste&height=810&id=udb1d7e1d&originHeight=2024&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=165061&status=done&style=shadow&taskId=u1652fa8a-96e4-45bd-8a58-e6ce19eee0d&title=&width=1042)
<a name="Iw28E"></a>
#### ⑧ 配置运行后打开浏览器
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681220132421-c68f222e-a953-4e43-bc62-b31d86503078.png#averageHue=%23f4f3f3&clientId=ue4b9a7d8-40e4-4&from=paste&height=810&id=u249f0710&originHeight=2024&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=238233&status=done&style=shadow&taskId=u68ed944f-28b2-46fa-a25f-feef848abe8&title=&width=1042)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681220221538-752a38ee-289a-4a20-ba8b-79b23c283562.png#averageHue=%23f3f3f2&clientId=ue4b9a7d8-40e4-4&from=paste&height=810&id=ufa4469ed&originHeight=2024&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=212550&status=done&style=shadow&taskId=u17f916f5-3886-46ac-92c8-e20518b9b90&title=&width=1042)<br />或者直接复制Smart Tomcat运行成功输出的URL也可：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1681227411715-e61fda25-1df7-4ce1-bfa2-4e762b2b137b.png#averageHue=%23f8f1ef&clientId=uf176d16f-38bd-4&from=paste&height=347&id=ue01d05f3&originHeight=868&originWidth=2696&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=389628&status=done&style=shadow&taskId=u40436139-73dc-4574-bfab-0607d154e3f&title=&width=1078.4)
<a name="iuAtj"></a>
#### ⑨ 至于打包，还是通过Maven插件来打包，这个插件未找到打包的方式，毕竟是免费的。
<a name="YEyWp"></a>
#### ⑩ 解决控制台乱码问题
配置Tomcat的conf文件夹下的logging.properties文件夹中的编码改为与项目编码一致。
