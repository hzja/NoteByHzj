1. 查看代码历史版本
2. 调整IDEA的虚拟内存
3. IDEA设置成Eclipse的快捷键
4. 设置提示词忽略大小写
5. 关闭代码检查
6. 设置文档注释模板
7. 显示方法分隔符
8. 设置多行tab
9. 快速匹配方法的大括号位置
10. 代码结尾补全
11. 模糊搜索方法
12. 预览某个类的代码
13. 查看方法在哪里被调用
14. 代码模板（代码快捷键）
15. 自动导包、自动移除没用的包
16. codota插件：可以优先显示使用频率较高的类、方法
17. 快速查看类、字段的注释
18. 括号颜色区分
19. 微服务项目中 将不同项目添加到同一个启动窗口
20. IDEA全局设置 （打开新窗口的设置）
21. Java mapper层代码文件和mapper.xml文件相互跳转
22. 设置IDEA背景图片
23. Maven tree （查看Maven jar包依赖关系）
24. 快捷键切换回上一个点击开的tab
25. IDEA自带的ssh连接工具
26. 代码调用链路图插件
27. 获取当前线程dump
28. IDEA同个项目不同端口多开

注意：不同IDEA版本菜单、目录可能有细微差别，自己稍加分析都能找到
<a name="NeJme"></a>
## 1、查看代码历史版本
鼠标在需要查看的Java类 右键 找到Local History >> Show History 点开即可看到历史版本，常用于自己忘记代码改了哪些内容 或需要恢复至某个版本 (注意 只能看近期修改 太久了也是看不到的)<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699944522230-6f77f199-b587-4b16-b910-17ccedac838d.png#averageHue=%23e3e2e2&clientId=uc65f809b-4d89-4&from=paste&height=816&id=ubc77fc8f&originHeight=2040&originWidth=1162&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=193488&status=done&style=shadow&taskId=u0fec1ba2-34ad-4fd8-89b2-93e95f67ee6&title=&width=464.8)
<a name="U9HwP"></a>
## 2、调整IDEA的虚拟内存
尽管本质都是去改变 .vmoptions配置文件，但推荐使用Change Memory Settings去调整，选择Edit Custom VM Options 或者在本地磁盘目录更改，通过某些方法破解的IDEA 很可能造成IDEA打不开的情况<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699944625372-f40a63cf-ab58-4529-90e2-e2dcd94dcbec.png#averageHue=%23e8e7e6&clientId=uc65f809b-4d89-4&from=paste&height=630&id=u57a2f31f&originHeight=1574&originWidth=592&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=102114&status=done&style=shadow&taskId=u50818512-1833-4726-bf94-06717d4a218&title=&width=236.8)
<a name="c1t45"></a>
## 3、IDEA设置成Eclipse的快捷键
这对Eclipse转IDEA的开发人员来说 非常友好，这样不需要记两套快捷键<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699944679667-b4d417d4-fa60-4109-b2b9-ebf499595a26.png#averageHue=%23cea76b&clientId=uc65f809b-4d89-4&from=paste&height=712&id=u065e8236&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=232242&status=done&style=shadow&taskId=uc6048151-daaa-4468-9033-3db84115759&title=&width=951.2)
<a name="zMXO6"></a>
## 4、设置提示词忽略大小写
把这个勾去掉，（有的IDEA版本是选择选项 选择none即可），例如String 输入string 、String 都可以提示<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699944771447-60260757-58dd-462e-bf52-b5e30857b597.png#averageHue=%23c9a268&clientId=uc65f809b-4d89-4&from=paste&height=712&id=ud4d92051&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=290314&status=done&style=shadow&taskId=u55a4dd03-a994-435f-9637-77b1ac8af00&title=&width=951.2)
<a name="vr48Q"></a>
## 5、关闭代码检查
与Eclipse类似，IDEA也可以自己关闭代码检查 减少资源使用，但不推荐全部关闭，把项目中不会使用到的关闭就好了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699944883825-34c86d11-6dfb-4f98-8d3d-2d1547f986e2.png#averageHue=%23cba265&clientId=uc65f809b-4d89-4&from=paste&height=712&id=uec63df9e&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=294896&status=done&style=shadow&taskId=uc218168e-b41c-49ab-88d5-1c382ed1940&title=&width=951.2)
<a name="WAoAJ"></a>
## 6、设置文档注释模板

<a name="VR690"></a>
## 7、显示方法分隔符
方便查看方法与方法之间的间隔，在代码不规范的项目中 很好用！<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699945284123-7202876f-c67a-4998-8780-072ba343195c.png#averageHue=%23c8a065&clientId=uc65f809b-4d89-4&from=paste&height=712&id=uad36822c&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=267144&status=done&style=shadow&taskId=u3ddaa12a-c28a-4125-89f7-4687ff9c45c&title=&width=951.2)
<a name="F58ky"></a>
## 8、设置多行tab
IDEA默认是选择显示单行的，把这个去掉，就可以显示多行tab了，在打开tab过多时的场景非常方便！<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699945351421-b54e544a-6220-44e4-8f29-7840569215a5.png#averageHue=%23c9a268&clientId=uc65f809b-4d89-4&from=paste&height=712&id=uc8918064&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=271335&status=done&style=shadow&taskId=ub488ebe3-27cc-457b-9016-3d91710a825&title=&width=951.2)<br />**tab过多自动关闭设置：**settings - editor - General - Editor tabs - tab limit 数值设大就好了<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699945397454-5418f4e7-e555-44e9-a10f-dfb4c547812f.png#averageHue=%23c9a36a&clientId=uc65f809b-4d89-4&from=paste&height=712&id=u45a938d0&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=255293&status=done&style=shadow&taskId=ufb686583-7ff6-4925-ad3a-ac1c92b8734&title=&width=951.2)
<a name="i845x"></a>
## 9、快速匹配方法的大括号位置
Ctrl+[ Ctrl+] 可以快速跳转到方法大括号的起止位置，配合方法分隔符使用，不怕找不到方法在哪儿分割了
<a name="j2C1u"></a>
## 10、代码结尾补全
例如一行代码补全分号，或者是if(xxx) 补全大括号，按Ctrl+Shift+Enter 无需切换鼠标光标，大幅度提升了编码效率
<a name="raFDT"></a>
## 11、模糊搜索方法
例如People类里面的test方法，按Ctrl+Shift+Alt+n 输入Peo.te 就可以查到该方法了，如果觉得这个快捷键难记也可以按Ctrl+Shift+r （查找某个文件名的快捷键下图中的Files），再手动选择Symbols
<a name="vtQW9"></a>
## 12、预览某个类的代码
例如People类里面的test方法，按Ctrl+Shift+a<br />例如在test类中，有句代码：`People p = new People();` 想稍微查看一下People这个类，但是tab已经够多了，Ctrl+Alt+b会打开新的标签，标签多了就混乱了，尤其一堆命名类似的tab，这时候可以按Ctrl+Shift+i 实现预览功能，不占tab<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946228638-198fb281-16b0-457a-9786-a1291778f88d.png#averageHue=%23f5f5f4&clientId=uc65f809b-4d89-4&from=paste&height=472&id=uadf9b7ec&originHeight=1181&originWidth=2282&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=384200&status=done&style=shadow&taskId=ua874950b-fdf9-40ad-a975-61351d86633&title=&width=912.8)
<a name="fVKWe"></a>
## 13、查看方法在哪里被调用
Ctrl+Alt+h 可以清楚看到方法在哪些地方被调用<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946284029-b582de8a-6ed7-4397-8c53-4184585971c7.png#averageHue=%23f7f7f7&clientId=uc65f809b-4d89-4&from=paste&height=234&id=ucef49e24&originHeight=584&originWidth=1286&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=56581&status=done&style=shadow&taskId=uf5d4b2ab-54d1-42b3-8c97-d35c135bdc6&title=&width=514.4)
<a name="e5V3t"></a>
## 14、代码模板(代码快捷键)
例如 : Eclipse 中的syso是打印控制台输出，但是IDEA默认是sout，如果非要改成syso 可以在Postfix Completion里面设置，类似的 fori等都是在里面设置<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946473695-8118882e-d0f8-454e-b111-073105b6402b.png#averageHue=%23f2e5c6&clientId=uc65f809b-4d89-4&from=paste&height=712&id=u7475d9b9&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=394763&status=done&style=shadow&taskId=u2969bc8b-2e2a-43f4-b72d-3eed93fc2e9&title=&width=951.2)
<a name="tBSRC"></a>
## 15、自动导包、自动移除没用的包
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946575990-e8f35f52-d193-470d-bfc1-15decb1fa547.png#averageHue=%23cda871&clientId=uc65f809b-4d89-4&from=paste&height=712&id=uc19f6340&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=262848&status=done&style=shadow&taskId=u1fefe3b8-88c3-4e93-a3a2-c07e9f4804f&title=&width=951.2)<br />**手动导包：Alt+Enter 手动移除未使用包: Ctrl+Alt+O**
<a name="OIycY"></a>
## 16、codota插件：可以优先显示使用频率较高的类、方法
这个根据个人是否要使用 有的时候工具只能作参考，自己记忆才能保证准确<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699943724415-1a43ae27-77f9-4770-8148-a8f89bc3d022.png#averageHue=%2354ae91&clientId=uc65f809b-4d89-4&from=paste&id=ua56e0140&originHeight=177&originWidth=641&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=uba97c36c-3e07-4ec9-adfd-69a0e08bdba&title=)
<a name="ZZPwG"></a>
## 17、快速查看类、字段的注释
这是一个很有用的小技巧 按F2可以快速查看某个类或某个字段的文档注释；基于这点 其它IDE应该也是可以快速查看文档注释内容 不仅仅是IDEA特性，这也是为什么阿里编码规范里面会明确声明实体类字段需要用文档注释 而不能使用双斜杠// 注释，还记得刚看到这个规范的时候 很不理解 特地去百度 看到有人说就是个习惯问题 很显然不能说服人，直到发现F2可以快速查看之后 恍然大悟！
<a name="C1zWa"></a>
## 18、括号颜色区分
Rainbow Brackets 插件 成对的括号用相同的颜色表示出来了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699943724527-e6709e94-7d5e-4452-8717-ba39b311f010.png#averageHue=%232e2d2c&clientId=uc65f809b-4d89-4&from=paste&id=ufa46c7f8&originHeight=125&originWidth=721&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8445f39b-e194-4d92-9d88-38cb6ed21f8&title=)
<a name="seGSQ"></a>
## 19、微服务项目中将不同项目添加到同一个启动窗口
步骤：View ——>Tool Windows ——> services ——>add services<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946597070-8cf01b02-0ba8-4e8a-b892-15c68e86a9b2.png#averageHue=%23eae9e9&clientId=uc65f809b-4d89-4&from=paste&height=581&id=u03cd7ccf&originHeight=1452&originWidth=1446&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=216076&status=done&style=shadow&taskId=ue5ba8f28-d722-47cf-8c0d-02b7faee604&title=&width=578.4)
<a name="oQZNq"></a>
## 20、IDEA全局设置（打开新窗口设置）
例如打开新窗口时，Maven配置会恢复 这时就需要对打开新窗口的设置进行修改 达到一个全局的目的。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946636414-bda5abd2-181f-48d3-8796-584ddec5565f.png#averageHue=%23e5e4e4&clientId=uc65f809b-4d89-4&from=paste&height=515&id=u980cd436&originHeight=1287&originWidth=1209&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=130444&status=done&style=shadow&taskId=u05d2680f-4e38-4fcd-a641-78bf296cc97&title=&width=483.6)<br />在低版本IDEA中 也是在File下 例如IDEA2018是叫other settings
<a name="mSZqD"></a>
## 21、Java mapper层代码文件和mapper.xml文件相互跳转
Free MyBatis plugin 插件<br />点击绿色箭头可以直接跳转 非常方便<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1699943725370-8cce3224-42f0-4731-a847-7f4fe743f48f.png#averageHue=%235c6562&clientId=uc65f809b-4d89-4&from=paste&id=ubf1009b3&originHeight=147&originWidth=136&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=shadow&taskId=u259eb0c3-613a-498b-834c-ce056c4bfde&title=)
<a name="arXiQ"></a>
## 22、设置IDEA背景图片
设置喜欢的图片作为背景图<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946777851-d4ca3d74-99c7-4803-af68-96bbf7086a13.png#averageHue=%23cba66e&clientId=uc65f809b-4d89-4&from=paste&height=712&id=u99f2355e&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=290685&status=done&style=shadow&taskId=u04fd2583-15b7-43e2-8301-28fc0860038&title=&width=951.2)
<a name="Kzn0A"></a>
## 23、Maven tree（查看Maven jar包依赖关系）
如果是使用 `mvn dependency:tree` 命令，结果是下图这样 很不方便查看<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946987468-098b9e40-66b7-4e31-b7d4-1248a45ec3a0.png#averageHue=%23f7f6f5&clientId=uc65f809b-4d89-4&from=paste&height=404&id=uea436ac4&originHeight=1011&originWidth=3419&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=441432&status=done&style=shadow&taskId=u1253234e-c1d5-466b-a5c7-833126944f7&title=&width=1367.6)<br />可以在IDEA右上角点击这个ShowDependencies按钮 以图片形式展示出来 图片按住Ctrl和鼠标滑轮 可以放大，这个在排除依赖冲突 查看jar包来源时 非常好用<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699947091507-53d8b6d8-2455-43ea-b200-e550359ad07b.png#averageHue=%23fafafa&clientId=uc65f809b-4d89-4&from=paste&height=739&id=u81340924&originHeight=1847&originWidth=2921&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=369461&status=done&style=shadow&taskId=u77d34010-bd14-419f-8346-b84ac95c48a&title=&width=1168.4)<br />有的时候，可能莫名其妙看不到这个图标，可以点一下设置<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946991032-5deebd8d-f59d-4aad-9a79-6c6119c27221.png#averageHue=%23f9f9f8&clientId=uc65f809b-4d89-4&from=paste&height=239&id=u1a7de8fc&originHeight=598&originWidth=1085&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=48656&status=done&style=shadow&taskId=ub6e0cd03-08fd-484f-8e6d-a4254e52429&title=&width=434)<br />把这个勾上，它就能显示了，等显示后，再把这个勾去掉<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699946986337-48d29342-5aaf-428d-8b50-4e3666eec375.png#averageHue=%23f2f2f2&clientId=uc65f809b-4d89-4&from=paste&height=334&id=u8e045e54&originHeight=836&originWidth=1081&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=74265&status=done&style=shadow&taskId=uf7567531-55d0-4247-8adf-cb1a86e29da&title=&width=432.4)
<a name="mZcBK"></a>
## 24、快捷键切换回上一个点开的tab
当打开了多个tab的时候，想要快速回到上一个点击的tab中 有的时候肉眼很难找<br />可以用快捷键 Alt + ← 键 (Eclipse版快捷键 IDEA默认快捷键需要自测)，有的时候在后面tab编辑了内容 按一次可能不够 需要再多按几次，相应的 Alt + → 切换到下一个点击的tab<br />常见应用场景：debug发生类跳转时 、利用快捷键在其它类中创建方法时<br />即使两个tab不相邻 也可以切换回去
<a name="WWCcb"></a>
## 25、IDEA自带的ssh连接工具
类似的IDEA还有自带的数据库连接工具 但是视图界面并不如Navicat 所以很多人都不选择用

- 第一步：配置账号密码

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699947172979-0280404c-4b56-4770-b50c-6e126d9180dc.png#averageHue=%23f1f1f0&clientId=uc65f809b-4d89-4&from=paste&height=712&id=ufa059f55&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=204685&status=done&style=shadow&taskId=u23f21d7a-aeba-4af3-92a5-107c71be157&title=&width=951.2)

- 第二步：开启ssh会话

![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699947184908-0574a632-aca5-43bb-8bbe-7b13392ffa86.png#averageHue=%23e9e8e7&clientId=uc65f809b-4d89-4&from=paste&height=500&id=uf0832eab&originHeight=1249&originWidth=577&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=84930&status=done&style=shadow&taskId=u875d3fc9-c3c0-4426-9662-d6e2d03a9e0&title=&width=230.8)
<a name="joGpv"></a>
## 26、代码调用链路图插件—**SequenceDiagram 插件**
IDEA自带的快捷键查看代码调用，只是以菜单形式展示，不太直观，如果是自己写的代码或比较规范的代码，那用自带的也就无所谓，如果是比较复杂的源码或不规范的代码，那使用 SequenceDiagram 会直观特别多。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699947266955-051af9bc-734c-4382-b882-befb0b3a068d.png#averageHue=%23e3c589&clientId=uc65f809b-4d89-4&from=paste&height=712&id=u096f9a25&originHeight=1781&originWidth=2378&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=289926&status=done&style=shadow&taskId=u2259a29f-2537-415e-9d10-a1e152cf76f&title=&width=951.2)<br />在要查看的Java文件里面鼠标右键，点击 Sequence Diagram<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699948089037-02400d17-e2d6-4344-8cb9-8cf9222351b6.png#averageHue=%23ebebea&clientId=u56d9053c-3035-4&from=paste&height=212&id=ubee0485c&originHeight=531&originWidth=872&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=43574&status=done&style=shadow&taskId=u14adf21e-273a-4f46-bcb4-c0d9a61d450&title=&width=348.8)<br />选择对应的方法：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699948140083-8cdd244e-1624-4949-985d-4167599ccab0.png#averageHue=%23efeeed&clientId=u56d9053c-3035-4&from=paste&height=123&id=u65623b05&originHeight=307&originWidth=437&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=16736&status=done&style=shadow&taskId=u58818a72-cd28-4290-b7a4-0b504d009d7&title=&width=174.8)<br />效果示例：<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699948111403-cefdc2dd-7b3f-4b21-b65a-2b77f7c8b816.png#averageHue=%23ebeae7&clientId=u56d9053c-3035-4&from=paste&height=293&id=u38d7edc2&originHeight=732&originWidth=919&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=45979&status=done&style=shadow&taskId=u68dc524f-1e11-46e3-986d-5060a0bfcb4&title=&width=367.6)
<a name="OKtZE"></a>
## 27、获取当前线程dump
在断点调试的时候，可以通过点击下图红色箭头指向的相机图标，获取当前线程的dump信息。<br />这个功能有什么用呢？可以通过线程名，分析当前是哪个线程执行的，在多线程环境下对代码运行分析起到辅助作用。<br />比如下图1，`run()`方法是通过main主线程执行的，只是方法调用，并没有启动多线程（这是熟知结论的实践证明）<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699969591020-2b9ffadd-f8b8-45f3-8bbc-0f9514bee30e.png#averageHue=%23faf9f9&clientId=u8c8e2e25-bb29-4&from=paste&height=580&id=ue6f775f5&originHeight=1449&originWidth=2910&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=306589&status=done&style=shadow&taskId=u787c1531-5cb5-4c03-b0cd-8b90573df2e&title=&width=1164)<br />当把run方法改成`start()`方法时，可以看到是线程thread0执行的。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699969673522-80970d6d-9bad-407a-aa5b-c969d1a9540b.png#averageHue=%23faf9f9&clientId=u8c8e2e25-bb29-4&from=paste&height=578&id=u7285ae52&originHeight=1445&originWidth=2848&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=298064&status=done&style=shadow&taskId=uea1738f5-a3f2-46b4-ab8f-6e2d3d5fb4c&title=&width=1139.2)
<a name="Z3qTZ"></a>
## 28、IDEA同个项目不同端口多开
这是个非常实用的功能，可以模拟集群测试负载均衡。<br />此外在开发阶段也是非常好用，开发过程中，让别人直接连自己本地测试是不是非常方便？<br />那自己又想打断点调试会影响别人怎么办呢 ？这个时候多开的作用就体现出来了！<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1699947346516-e2b8d5aa-0ad2-4337-a6ab-15e1d5b24cb0.png#averageHue=%23f4f3f3&clientId=uc65f809b-4d89-4&from=paste&height=676&id=ua962b2f2&originHeight=1691&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=300156&status=done&style=shadow&taskId=u53839c3a-7604-4189-ad39-be649fff68e&title=&width=1042)<br />勾选 Allow parallel run（Allow multiple instance） (不同版本IDEA 名称不一样)，也亲眼见过有老师是可以多开启动的， 但本地启动发现每次都会同时同端口启动多个，不知道是版本问题还是操作问题，这里用的是另一种有效的方法：<br />在 VM options 加上
```
# 8993是区别于 application.yml 配置中 port 的另一个端口，达到不同端口多开的效果
-Dserver.port=8993
```
