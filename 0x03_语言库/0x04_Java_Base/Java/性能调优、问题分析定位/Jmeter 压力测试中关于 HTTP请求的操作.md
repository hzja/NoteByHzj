Jmeter
<a name="O7BfJ"></a>
## 1、简介
JMeter是开源软件Apache基金会下的一个性能测试工具，用来测试部署在服务器端的应用程序的性能。近来，JMeter因为其使用简单，现在也被社区作为接口测试工具，JMeter就是那个能帮助模拟大量用户访问网站的一个软件。而且它是开源的。
<a name="mTNGX"></a>
## 2、JMeter下载和安装
JMeter可以在JMeter的官方网站下载，下载链接如下图所示：<br />官网连接：[https://jmeter.apache.org/](https://jmeter.apache.org/)<br />下载链接：[https://dlcdn.apache.org//jmeter/binaries/apache-jmeter-5.4.1.zip](https://dlcdn.apache.org//jmeter/binaries/apache-jmeter-5.4.1.zip)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632289256130-2b5ebba8-d272-4581-bd7e-eea7da2381ff.png#clientId=u91355705-9d73-4&from=paste&height=601&id=u0c7204f0&originHeight=1803&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=689703&status=done&style=shadow&taskId=u43fbd347-df8d-4857-9b9c-bdd65243bf4&title=&width=1266)<br />安装包：[apache-jmeter-5.4.1.zip](https://www.yuque.com/attachments/yuque/0/2021/zip/396745/1632320800749-752b6e87-9e32-4e72-a6c6-9192f88dac17.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2021%2Fzip%2F396745%2F1632320800749-752b6e87-9e32-4e72-a6c6-9192f88dac17.zip%22%2C%22name%22%3A%22apache-jmeter-5.4.1.zip%22%2C%22size%22%3A74032019%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22source%22%3A%22%22%2C%22status%22%3A%22done%22%2C%22mode%22%3A%22title%22%2C%22download%22%3Atrue%2C%22taskId%22%3A%22u20bea7c7-c612-455f-81cd-e8322a8c1e6%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22ud78c7daf%22%2C%22card%22%3A%22file%22%7D)
<a name="IZHIw"></a>
### 解决JMeter在高分辨率下图标小的问题
<a name="OxLzd"></a>
#### 1、编写批处理文件
新建一个名为“适配高分辨率.txt”的文本文档，在其中写入如下代码：
```powershell
echo off
set FILE_NAME=jmeter.properties
echo .
echo .
echo language=zh_CN>> %FILE_NAME%
echo jmeter.hidpi.mode=true>> %FILE_NAME%
echo jmeter.hidpi.scale.factor=2.0>> %FILE_NAME%
echo jmeter.toolbar.icons.size=48x48 >> %FILE_NAME%
echo jmeter.tree.icons.size=48x48 >> %FILE_NAME%
echo sampleresult.default.encoding=utf-8>> %FILE_NAME%
echo jsyntaxtextarea.font.family=楷体>> %FILE_NAME%
echo jsyntaxtextarea.font.size=30>> %FILE_NAME%
pause
```
这段代码的作用是开放Jmeter适配高分辨率的能力，并且完成默认像素设置，顺便设置了下语言为中文。<br />代码粘贴完成后保存，并将文件后缀从txt改为bat。这样就做好了一个批处理脚本。
<a name="V5pkg"></a>
#### 2、执行批处理文件
将刚刚做好的“适配高分辨率.bat”文件放入Jmeter的bin目录下，双击执行。执行完成后，启动Jmeter。<br />这时可以看到页面样式已经适配了4k的分辨率，大小恢复正常。<br />但，如果经过刚刚两步操作后，界面依旧没有恢复正常，那请执行下一步操作。
<a name="luc5Z"></a>
#### 3、自由缩放页面大小
打开页面上方的“选项”菜单，可以看到里面有一个“放大”和一个“缩小”按钮，点击放大即可让页面恢复到合适的大小。
<a name="wK4hx"></a>
#### 4、表格显示过小的解决方案
如果发现页面上有部分模块样式展示异常如下图：<br />请在“选项-外观”菜单里切换一下主题（切成其他的再切回来），操作完成后，页面即可恢复正常。[<br />](https://blog.51cto.com/u_13558591/4986620)
<a name="jqipL"></a>
## 3、Http请求模拟

---

<a name="eXfIC"></a>
### 1、新建线程组
操作：鼠标右键测试计划 -> 添加 -> Threads(Users) -> 线程组 -> 修改测试计划名称<br />![新建线程组](https://cdn.nlark.com/yuque/0/2021/png/396745/1632321377250-e6c5d1f8-6def-41ff-91ef-ddae345bc119.png#clientId=u05f870a6-bc11-4&from=paste&height=284&id=uaad7b8ed&originHeight=851&originWidth=1381&originalType=binary&ratio=1&rotation=0&showTitle=true&size=121187&status=done&style=shadow&taskId=udea7b32e-d6b9-4346-9726-ee8e4d76191&title=%E6%96%B0%E5%BB%BA%E7%BA%BF%E7%A8%8B%E7%BB%84&width=460.3333333333333 "新建线程组")<br />“线程组”中常用到的一些设置。<br />1）线程数：这里就是指虚拟用户数，默认的输入是“1”，则表明模拟一个虚拟用户访问被测系统，如果想模拟100个用户，则此处输入100。<br />2）Ramp-Up Period (in seconds): 虚拟用户增长时长。举个栗子：比如测试的是一个考勤系统，那么实际用户登录使用考勤系统的时候并不是大家喊1、2、3 - 走起，然后一起登录。实际使用场景可能是9点钟上班，那么从8:30开始，考勤系统会陆陆续续有人开始登录，直到9:10左右，那么如果完全按照用户的使用场景，设计该测试的时候此处应输入40（分钟）* 60（秒）= 2400。但是实际测试一般不会设置如此长的Ramp-Up时间，一般情况下，可以估计出登录频率最高的时间长度，比如此处可能从8:55到9:00登录的人最多，那这里设置成300秒，如果“线程数”输入为100，则意味着在5分钟内100用户登录完毕。<br />3）循环次数：该处设置一个虚拟用户做多少次的测试。默认为1，意味着一个虚拟用户做完一遍事情之后，该虚拟用户停止运行。如果选中“永远”，则意味着测试运行起来之后就根本停不下来了，除非强制停止。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632321429054-58bfe93a-ba87-4df9-9100-457161719c67.png#clientId=u05f870a6-bc11-4&from=paste&height=547&id=ud3aa64db&originHeight=1640&originWidth=3040&originalType=binary&ratio=1&rotation=0&showTitle=false&size=229501&status=done&style=none&taskId=ud717b38b-dc11-44b9-85d1-9a7168403cf&title=&width=1013.3333333333334)
<a name="FaREs"></a>
### 2、添加取样器HTTP请求
操作：鼠标右键线程组 -> 添加 -> Sampler -> HTTP请求 -> 填写请求参数<br />![添加取样器](https://cdn.nlark.com/yuque/0/2021/png/396745/1632321493564-e9489538-8acd-413b-9ab7-f71d016a4969.png#clientId=u05f870a6-bc11-4&from=paste&height=439&id=u1dab8cfb&originHeight=1316&originWidth=1775&originalType=binary&ratio=1&rotation=0&showTitle=true&size=254516&status=done&style=shadow&taskId=u3e369dfb-e6e4-4ca2-81e3-f0522f6a759&title=%E6%B7%BB%E5%8A%A0%E5%8F%96%E6%A0%B7%E5%99%A8&width=591.6666666666666 "添加取样器")<br />![添加参数](https://cdn.nlark.com/yuque/0/2022/png/396745/1656926214868-77f872c4-d140-4e9d-bb0c-41f29398d011.png#clientId=u25fb80f3-9eb8-4&from=paste&height=656&id=ue8b50f92&originHeight=1640&originWidth=3040&originalType=binary&ratio=1&rotation=0&showTitle=true&size=172813&status=done&style=shadow&taskId=uebb0c394-7285-4664-aec4-4b294b9b746&title=%E6%B7%BB%E5%8A%A0%E5%8F%82%E6%95%B0&width=1216 "添加参数")
<a name="MVAAJ"></a>
### 3、开始Http请求
<a name="WbPXN"></a>
#### a. 添加察看结果树监听器，方便查看请求结果
操作：**鼠标右键线程组 -> 添加 -> 监听器 -> 察看结果树<br />![察看结果树](https://cdn.nlark.com/yuque/0/2021/png/396745/1632321587745-a91cc03d-f465-4188-b985-db24bd00e18b.png#clientId=u05f870a6-bc11-4&from=paste&height=547&id=ue69ee1b4&originHeight=1640&originWidth=1777&originalType=binary&ratio=1&rotation=0&showTitle=true&size=277986&status=done&style=shadow&taskId=u7bc991a1-773b-4a79-95e5-55ede29a4b4&title=%E5%AF%9F%E7%9C%8B%E7%BB%93%E6%9E%9C%E6%A0%91&width=592.3333333333334 "察看结果树")
<a name="rTpN8"></a>
#### b. 点击**工具栏上单击启动按钮（绿色箭头）**启动测试计划
![开始验证](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631376918236-760ac42a-de7d-4655-b5e7-a83999e5f73f.webp#clientId=ubc93e8bc-687e-4&from=paste&id=u4e5f9c41&originHeight=400&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u4c2a0aa0-41de-44a8-b9aa-a6a9e421804&title=%E5%BC%80%E5%A7%8B%E9%AA%8C%E8%AF%81 "开始验证")<br />PS：响应数据默认显示格式为Text，可切换为JSON Path Tester格式<br />![切换格式](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631376918818-d9b98e93-c32b-4d00-8e6e-d18172c9138e.webp#clientId=ubc93e8bc-687e-4&from=paste&id=u51e0b1e8&originHeight=309&originWidth=520&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ubfce0e43-97c6-46bd-9e75-367c409fc7f&title=%E5%88%87%E6%8D%A2%E6%A0%BC%E5%BC%8F "切换格式")<br />![JSON Path Tester](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631376918837-3df72865-3d1e-4408-a262-a1f31a65a1b9.webp#clientId=ubc93e8bc-687e-4&from=paste&id=u2754f1f2&originHeight=432&originWidth=504&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=ua31f223b-8998-4e4e-a183-d8131f60fed&title=JSON%20Path%20Tester "JSON Path Tester")
<a name="cSQVa"></a>
## 4、响应参数获取

---

之所以需要获取响应参数，主要是因为常常有这样的需求，要求将上一个接口的返回值作为下一个接口的请求参数。举个栗子：获取即将上映电影中第一部电影的条目信息，这里需要完成两个步骤：

- 获取即将上映电影中的第一部电影的id，接口地址：https://api.douban.com/v2/movie/coming_soon?count=2
- 得到id后，请求接口获取电影条目信息，接口地址：http://api.douban.com/v2/movie/subject/${id}

![接口返回值](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631377033340-847b1aa5-77bd-437f-99d1-f9928eb04cf3.jpeg#clientId=ubc93e8bc-687e-4&from=ui&id=u2f4f594c&originHeight=509&originWidth=624&originalType=binary&ratio=1&rotation=0&showTitle=true&size=23563&status=done&style=shadow&taskId=u7d7f81a5-514a-43b8-8dd5-06463a8c9f4&title=%E6%8E%A5%E5%8F%A3%E8%BF%94%E5%9B%9E%E5%80%BC "接口返回值")
<a name="VAb6w"></a>
### 1、添加后置处理器JSON Extractor
操作：将鼠标放置“获取即将上映电影”的HTTP请求上，并按右键 -> 添加 -> 后置处理器 -> JSON Extractor -> 填入变量名等信息<br />![添加后置处理器](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631376918937-55547b5f-c252-4523-bfb2-5bd578293af8.webp#clientId=ubc93e8bc-687e-4&from=paste&id=u7c65ccd5&originHeight=260&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u3cfad3b3-0004-4486-a881-9d4fe02dde4&title=%E6%B7%BB%E5%8A%A0%E5%90%8E%E7%BD%AE%E5%A4%84%E7%90%86%E5%99%A8 "添加后置处理器")<br />![变量名等信息填写](https://cdn.nlark.com/yuque/0/2021/webp/396745/1631376918932-4980df8f-dba3-459b-909a-687a5ebbcf60.webp#clientId=ubc93e8bc-687e-4&from=paste&id=u3fc46109&originHeight=253&originWidth=640&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=shadow&taskId=u253e5d7a-adc0-414c-a91c-882784f95c8&title=%E5%8F%98%E9%87%8F%E5%90%8D%E7%AD%89%E4%BF%A1%E6%81%AF%E5%A1%AB%E5%86%99 "变量名等信息填写")
<a name="UJvNB"></a>
### 2、添加取样器HTTP请求获取电影条目信息及参数使用
![获取电影条目信息](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631377052571-5593118b-8106-45e1-877b-e7593376e330.jpeg#clientId=ubc93e8bc-687e-4&from=ui&id=u53226963&originHeight=194&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=true&size=14812&status=done&style=shadow&taskId=ubdf203ab-d18b-4a1a-a1ee-4bbe609bc15&title=%E8%8E%B7%E5%8F%96%E7%94%B5%E5%BD%B1%E6%9D%A1%E7%9B%AE%E4%BF%A1%E6%81%AF "获取电影条目信息")
<a name="m7hj6"></a>
### 3、启动测试计划
在“察看结果树”中可以看到，获取电影条目信息的HTTP请求中，请求参数正确。同时也得要正确的响应参数。<br />![请求参数](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631377052579-73924b3e-a7a2-4f5b-b7c7-86b2965530f7.jpeg#clientId=ubc93e8bc-687e-4&from=ui&id=gRBLe&originHeight=274&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=true&size=17616&status=done&style=shadow&taskId=u0d2025ef-7f18-4caa-808a-6fc0d2c7e81&title=%E8%AF%B7%E6%B1%82%E5%8F%82%E6%95%B0 "请求参数")<br />![响应参数](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631377052583-22aa3053-ba39-4eb7-99e8-7da57d92b2f0.jpeg#clientId=ubc93e8bc-687e-4&from=ui&id=ve8XR&originHeight=467&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=true&size=29256&status=done&style=shadow&taskId=ud31ebf83-a637-4c53-a547-6bbca667fc6&title=%E5%93%8D%E5%BA%94%E5%8F%82%E6%95%B0 "响应参数")<br />PS：如果需要多变量取值的话，可在JSON Extractor的配置中通过分号(;)将多个变量名分开。<br />需要注意的是，多个变量名的话，还需要填写Default Vaules，不然启动测试计划后会找不到变量，导致请求失败。<br />笔者这里是获取id和year两个变量，设置默认值为0和0，这个可随意设置，影响不大。<br />![多变量取值](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1631377052581-5acbc461-6628-42b6-9a35-bba18f6af0b8.jpeg#clientId=ubc93e8bc-687e-4&from=ui&id=qqmP5&originHeight=294&originWidth=640&originalType=binary&ratio=1&rotation=0&showTitle=true&size=23002&status=done&style=shadow&taskId=u8750f498-170a-40d3-a4e5-ed2f5a0d764&title=%E5%A4%9A%E5%8F%98%E9%87%8F%E5%8F%96%E5%80%BC "多变量取值")
