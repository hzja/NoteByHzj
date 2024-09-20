<a name="f83wB"></a>
## 注释对齐
日常开发中，相信大家在添加注释时都知道使用快捷键 ctrl + / ，不过大家可能会遇见下面这种情况，
```java
public DiamondJumpContext(List<DiamondJumpType> diamondJumpTypes) {
//添加注释
    for (DiamondJumpType diamondJumpType : diamondJumpTypes) {
//            map.put(diamondJumpType.getType(), diamondJumpType);
    }
}
```
可以看到，使用快捷键 ctrl + / 添加注释代码或者注释已有代码时，默认是在首行位置，无法与下方 for 循环代码自动对齐，造成了视觉上的不美观，那如何解决这个问题嘞？其实解决方法就藏在 IDEA 里。<br />打开 IDEA 设置 Editor -> Code Style -> Java -> Code Generation，找到下方的 Comment code 区域， 取消 Line comment at first column 的默认勾选，将 Add a space at line comment start 以及 Enforce on reformat 打上勾就可以了。如下图所示，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695449785406-095a48e9-5648-400c-a36f-c1a03e1cefee.png#averageHue=%23c9a165&clientId=ueabf0c60-6534-4&from=paste&height=700&id=ua7e426fa&originHeight=1749&originWidth=2368&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=320926&status=done&style=none&taskId=u14861b89-e1ef-4cf4-b1dc-e444c4de590&title=&width=947.2)<br />然后重新给上面的示例代码打赏注释时，效果就是下面这样了，
```java
public DiamondJumpContext(List<DiamondJumpType> diamondJumpTypes) {
    // 添加注释
    for (DiamondJumpType diamondJumpType : diamondJumpTypes) {
        // map.put(diamondJumpType.getType(), diamondJumpType);
    }
}
```
可以很明显的看到，此时添加的注释默认是跟当前代码对齐，并且自动留出一个空格，代码的可阅读性有了很大提升。
<a name="RpsUC"></a>
## 快速创建包目录/文件夹
<a name="BXKDC"></a>
### 创建包目录
日常开发时，当大家想新建 Java 类时，考虑到项目的包目录划分可能需要新建一个包目录，那么大家是如何操作的嘞？是先创建好包目录后，在创建 Java 类吗？其实在 IDEA 中创建 Java 类时，是可以直接顺带创建包目录的，具体操作如下。<br />在创建 Java 类时，直接写出包目录名称通过 . 拼接即可，如下图新建 Java 类时填上 controller.TestController，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695449879918-8912a7cd-78f8-4fee-8f3a-4599f3cd89b4.png#averageHue=%23d9d9d9&clientId=ueabf0c60-6534-4&from=paste&height=279&id=u26a3766b&originHeight=697&originWidth=1247&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=50087&status=done&style=none&taskId=u20b31d67-b854-444c-82cd-778eaa4dc46&title=&width=498.8)<br />效果如下，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695449895658-d60df166-e882-45ea-99a7-d3685d8581ac.png#averageHue=%23f1f0ef&clientId=ueabf0c60-6534-4&from=paste&height=86&id=ue8c539c2&originHeight=214&originWidth=603&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=11720&status=done&style=none&taskId=u466ad55e-5151-4877-a232-1ec9b0737a5&title=&width=241.2)<br />可以看到，包目录自动创建成功。
<a name="Z5X9c"></a>
### 创建文件夹
快速创建文件夹的方式跟快速创建包目录差不多，只不过将 `.` 拼接改为 / 即可，如下图新建文件时填上 template/test.csv，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695449985976-27c85907-0f8b-4db0-a373-ef79f00fad72.png#averageHue=%23e7e7e7&clientId=ueabf0c60-6534-4&from=paste&height=102&id=u56391da0&originHeight=255&originWidth=1414&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=27742&status=done&style=none&taskId=ua276b1c6-fab0-47d2-830f-01451037ee5&title=&width=565.6)<br />效果如下，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450007790-f957578c-4bc4-4576-87a6-e64b210a1c90.png#averageHue=%23f5f4f3&clientId=ueabf0c60-6534-4&from=paste&height=142&id=ua9a494e9&originHeight=354&originWidth=723&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=22564&status=done&style=none&taskId=u6fc1a625-be5e-45b8-8180-68057ff6d8f&title=&width=289.2)<br />可以看到，文件夹自动创建成功。
<a name="ThGRd"></a>
## 快速复制类代码生成 Java 类
日常中有个场景，经常在上网浏览网上的技术资料，看到感兴趣的文章时，想要复制文章中的 demo 代码在本地跑一遍验证一下。于是就有了这个快付复制代码生成 Java 类的需求。起初老老实实通过常规操作在本地手动创建 Java 类，然后拷贝 demo 代码进行验证。后来了解到其实在 IDEA 中是有快付复制类代码生成 Java 类的功能，这里分享给大家。<br />假如当前想要快速复制下方的 Java 类代码到 IDEA 项目中，那改如何操作嘞？
```java
/**
 * 金刚位跳转策略配置
 */
@Component
public class DiamondJumpContext {

    private final Map<Integer, DiamondJumpType> map = new HashMap<>();

    /**
     * 由spring自动注入DiamondJumpType子类
     *
     * @param diamondJumpTypes 金刚位跳转类型集合
     */
    public DiamondJumpContext(List<DiamondJumpType> diamondJumpTypes) {
        for (DiamondJumpType diamondJumpType : diamondJumpTypes) {
            map.put(diamondJumpType.getType(), diamondJumpType);
        }
    }

    public DiamondJumpType getInstance(Integer jumpType) {
        return map.get(jumpType);
    }
}
```
其实操作是很简单的，可以通过全选复制上述 Java 类代码，然后打开 IDEA，鼠标选中指定的包目录后通过 ctrl + v 粘连即可。<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450098235-0d679de5-75cc-4e3e-9d66-6049a2427bf2.png#averageHue=%23fbfbfa&clientId=ueabf0c60-6534-4&from=paste&height=218&id=ua0bac1fc&originHeight=546&originWidth=719&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=30770&status=done&style=none&taskId=u03477c6e-13b9-48da-8f2b-c9850d7da35&title=&width=287.6)<br />效果如下，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450137532-f3b5eb7f-9648-4674-b031-ed00fcddd852.png#averageHue=%23f8f6f6&clientId=ueabf0c60-6534-4&from=paste&height=259&id=u01f0ca92&originHeight=647&originWidth=733&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=41248&status=done&style=none&taskId=u4db144ac-af58-40ed-8bf7-31dad07d250&title=&width=293.2)<br />可以看到，已经将 Java 类代码快速复制到本地文件中。
<a name="P2jNm"></a>
## 多实例启动
日常本地开发微服务项目时，想要验证一下网关的负载均衡以及感知服务上下线能力时，需要用到多实例启动。<br />那么什么是多实例启动？简单说就是能在本地同时启动多个同一服务。打个比方项目中有一个 MobileApplication 服务，现在想要同时启动两个 MobileApplication 服务，通过不同端口区分，该如何操作呢？IDEA 中其实是支持这个操作的。介绍如何启用多实例功能。<br />打开服务配置，点击 Modify options 选项，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450903609-7e297bd1-1cfc-4f9a-9956-041923693338.png#averageHue=%23f4f4f3&clientId=ueabf0c60-6534-4&from=paste&height=676&id=u255d59a0&originHeight=1691&originWidth=2605&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=242568&status=done&style=none&taskId=u604abe7e-0612-496a-bed8-57eaa79244c&title=&width=1042)<br />勾选打开 Operation System -> Alow multiple instance ，继续勾选打开 Java -> Program arguments，然后在 Program arguments 输入框中指定端口，在 Spring 项目中通过 --server.port=9999，如下图（注意每次启动实例时，这里的端口不能相同），<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695451149392-c1b3a693-aed3-416e-b6f0-ce01b2cd88d2.png#averageHue=%23f2f1f1&clientId=ueabf0c60-6534-4&from=paste&height=702&id=uc878f72a&originHeight=1756&originWidth=3341&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=432709&status=done&style=none&taskId=u16380b60-d01a-44da-9c54-62a1f610b2b&title=&width=1336.4)<br />效果图如下，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695451230754-eaa53ea8-99fb-47f5-b7f5-10722aa26a3f.png#averageHue=%23f4f3f3&clientId=ueabf0c60-6534-4&from=paste&height=296&id=uec688b00&originHeight=740&originWidth=867&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=52446&status=done&style=none&taskId=u405fba68-9a97-4286-ac6f-f185fdca219&title=&width=346.8)<br />可以看到，这里多实例启动的功能就 OK 了，在 Modify options 选项中还有许多常用功能，对于日常开发都是很多帮助的，比如 Java -> add VM options 可以用来添加 jvm 启动参数等。
<a name="A3dN2"></a>
## 分支比较
日常开发中，当项目上线时，经常会使用这个功能，用于比较当前新功能分支与 master 分支的代码差异。<br />假如新功能分支上线前经历了多个人员参与迭代以及冲突修复，作为项目主程的你需要在项目上线前进行代码 review，以确保代码质量，那该如何操作？其实 IDEA 已经做好了一切，这里将给大家介绍远程分支比较功能。<br />右键项目根目录，找到 Git -> Compare with Branch 选项，点击打开，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450376044-dc9b176b-5740-4c14-9adf-890c304a8951.png#averageHue=%23eaeae9&clientId=ueabf0c60-6534-4&from=paste&height=800&id=ub8a0bdad&originHeight=1999&originWidth=1444&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=301505&status=done&style=none&taskId=ub12abd61-76a5-4667-894c-aaea7a56ab8&title=&width=577.6)<br />此时 IDEA 会显示项目的所有分支列表，从中找到 master 分支，点击即可，<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1695450571417-bcff9434-8ece-41b2-87ca-56a2f7b26054.png#averageHue=%23f2f1f1&clientId=ueabf0c60-6534-4&from=paste&height=130&id=u5cccdf5f&originHeight=326&originWidth=915&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=21421&status=done&style=none&taskId=u90a114a1-801c-4f1b-9db6-f8ad3fb94cc&title=&width=366)<br />最后 IDEA 会显示所有的文件差异在项目左侧栏目，点击某个文件即可查看某个具体差异。<br />可以看到，通过 IDEA 提供的分支比较功能进行代码 review，提供了非常便利的操作。
