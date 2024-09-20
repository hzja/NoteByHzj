<a name="hHZJI"></a>
## **背景**
在 java 开发中经常会遇到这样的场景，代码需要在服务器上（测试或者沙箱）运行，但是每次修改代码都需要经历打包 -> 部署 -> 启动等一些列的流程，这一套流程下来短则几分钟，长则几十分钟都有可能，漫长的打包流程浪费了大量的青春，市面上有很多 java 热部署的插件，要么是只能修改方法体而不能新增字段和方法，要么就是只能在本地热部署，本人也是在工作中部署浪费了无数的青春，于是痛定思痛，作者决定写一款远程热部署插件来提高效率，现将该插件 HotSeconds 分享给大家 (本插件分为 HotSecondsClient 和 HotSecondsServer)。
<a name="dmxTw"></a>
## **使用场景**

- IDEA 本地修改代码直接一键化热部署到远程服务器，只需要几秒钟。
- 支持修改代码块，新增字段，新增方法，新增类。
- 支持一些开源框架比如 Spring，SpringMVC 和 MyBatis。比如新增一个 `@Autorwired` 字段或者新增 MyBatis 的接口或者修改.xml 也是可以支持热部署的。
- 最重要的一点，能支持自由扩展，比如公司内部的框架，或者一些新出的第三方框架，需要在热部署文件后执行一些刷新缓存或者上下文的操作，也能自定义。

_**有了上述特点，部署效率至少可以提升 100 倍，不过光是热部署改 bug 还远远不是本插件的目标，那只是牛刀小试，目标是在线开发，远程服务器就是执行端，而本地，仅仅是一个写代码端。**_
<a name="Kb6vq"></a>
## **使用演示**
安装完插件后，Debug 旁边有一个按钮，打开或者关闭连接远程<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230247-635d4973-6133-4a96-8d05-3d0b0a84add7.png#averageHue=%233c4142&clientId=uc3388af0-659c-4&from=paste&id=ue39dbf94&originHeight=64&originWidth=239&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u7d5b303f-141b-40a2-a07d-23302d3ddef&title=)<br />高版本的 IDEA 可以点 Run->HotSeconds Start/Stop 打开和关闭。<br />首次打开会弹出配置选项，可以配置本地哪些目录下的文件上传到远程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230295-c0a9ad17-40a1-4ad7-8802-b74cdda35bfb.png#averageHue=%2341474c&clientId=uc3388af0-659c-4&from=paste&id=u285d368f&originHeight=627&originWidth=746&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uba0d2971-923b-48dc-b16b-a4405e34d98&title=)
<a name="ZkY7C"></a>
### 热部署演示
原来代码长这样
```java
@Controller
@RequestMapping("/view")
public class HelloViewController {

    @RequestMapping("/test")
    @ResponseBody
    public String test() {
        return "hello,test";
    }
}
```
修改代码为
```java
@Controller
@RequestMapping("/view")
public class HelloViewController {

    @RequestMapping("/test")
    @ResponseBody
    public String test() {
        return "hello,"+name();
    }

    private String name(){
        return "Tom";
    }
}
```
右键，选中 Hot swap this file to remote<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230268-4b267703-a0a2-4a30-baf0-be5e60fb0d79.png#averageHue=%23444749&clientId=uc3388af0-659c-4&from=paste&id=ua8ec1a94&originHeight=265&originWidth=574&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1d16c449-c8dd-43c7-bf46-6203cbabbac&title=)<br />然后就能把代码热部署到远程，刷新浏览器页面，代码就生效了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230262-49c52dd1-db7a-4c89-a0f1-f24fcb478ce5.png#averageHue=%23f0efef&clientId=uc3388af0-659c-4&from=paste&id=u8b1dc212&originHeight=96&originWidth=557&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub42e47f1-a0a9-4b6f-bfaf-3ea26f9dfa0&title=)<br />接着新增一个注入的字段和 controller<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230416-2c2ad10c-7ba0-4666-a3e0-c6ebff60ce9b.png#averageHue=%23403d2a&clientId=uc3388af0-659c-4&from=paste&id=u43c6e87a&originHeight=682&originWidth=576&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0e98c123-98c6-469f-b2c6-2845e40599c&title=)<br />右键热部署一下，刷新网页可以看到结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230647-5e5ef89f-6e1d-49e4-8c0c-8ea96559e8f9.png#averageHue=%23f2f2f2&clientId=uc3388af0-659c-4&from=paste&id=u625c92c4&originHeight=157&originWidth=904&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb0ec4f1-cf59-4c8d-be05-41456524a7c&title=)<br />对 UserMapperXml.xml 修改一下 Mybatis 代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230730-58e05889-72a1-428d-b579-c9d0d9200d04.png#averageHue=%233d3e33&clientId=uc3388af0-659c-4&from=paste&id=u3f0bcff0&originHeight=535&originWidth=616&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4f6a41b2-5aae-4d6a-95b5-a0aebcdb3a8&title=)<br />执行右键热部署刷新网页就能看到结果<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050230871-37c9c7bf-2820-43f3-87d0-6c91d42fc1e4.png#averageHue=%23e5e5e4&clientId=uc3388af0-659c-4&from=paste&id=u3a87d513&originHeight=79&originWidth=585&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4d2ac32d-a506-449a-a336-9f4e3637467&title=)
<a name="l0Df6"></a>
### 远程编译并热部署
右键菜单下有远程编译并热部署的功能，主要是解决本地和远程 jdk 版本不一致的问题，已兼容 jre 启动问题和 lombok。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050231303-beb73e33-1fe7-4b39-af78-10841e7dcbff.png#averageHue=%23444749&clientId=uc3388af0-659c-4&from=paste&id=u35eab099&originHeight=220&originWidth=640&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubd157802-7fcb-4719-b9f3-e627fd7d3c7&title=)
<a name="ScU41"></a>
### 热部署 jar 包和 jar 包内的.class
打开一个 jar 包的.class，右键，可以看到两个选项，可以将当前的.class 热部署到远程，也可以将整个 jar 包热部署到远程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050231568-b3290b26-69b8-4c13-b66c-4cc275812e61.png#averageHue=%2345484a&clientId=uc3388af0-659c-4&from=paste&id=u657cda50&originHeight=351&originWidth=531&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufe3d93b3-c5a5-4a85-b911-c9ff1471bc1&title=)
<a name="SdrWM"></a>
### 自动热部署
在 Run->HotSeconds Settings->Settings 中可以设置自动热部署，只要本地文件修改了就会自动热部署到远程。<br />**改完记得保存一下 (ctrl+s/command+s)，不然会有延迟**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050231624-a24d87b7-25ac-46ed-a25d-e0c75b6f63e5.png#averageHue=%2342474c&clientId=uc3388af0-659c-4&from=paste&id=u80129d63&originHeight=446&originWidth=624&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93981fbc-5046-4d25-8209-4d7ce9d39d4&title=)
<a name="LfLOp"></a>
### 远程执行方法
附带了一个非常强大的功能，远程执行静态无参方法<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050231724-30d18b5f-2d13-4b75-bb09-935d7a8f74bd.png#averageHue=%233d423c&clientId=uc3388af0-659c-4&from=paste&id=u9344136f&originHeight=216&originWidth=639&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3dde564c-5036-4515-94e0-6befca8a47a&title=)<br />这样可以不用写接口就可以直接执行远程的函数，可以在这个函数里面调用一些有参数或者非静态的方法，中间需要比如 Spring 容器上下文自己去维护即可，非常方便。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1694050231892-6952d180-1e3b-4406-b234-11dc881ae5d6.png#averageHue=%233f342d&clientId=uc3388af0-659c-4&from=paste&id=uefdc1277&originHeight=282&originWidth=917&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u251bd8f6-3b62-45d2-bc17-b227149ebbc&title=)
<a name="VyTIp"></a>
## 安装文档
本插件分为 HotSecondsClient 和 HotSecondsServer，请点击下面文档查看详细的安装教程。<br />安装文档链接：[https://github.com/thanple/HotSecondsIDEA/blob/master/install/%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3.md](https://github.com/thanple/HotSecondsIDEA/blob/master/install/%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3.md)
