CDN<br />对于开发同学来说，CDN这个词，既熟悉又陌生。平时搞开发的时候很少需要碰这个，但却总能听到别人提起。都听说过它能加速，也大概知道个原因，但是往深了问。<br />**用了CDN就一定比不用更快吗？**<br />就感觉有些懵了。但没关系，换个角度重新认识下CDN。
<a name="zZOvO"></a>
## CDN是什么
对于**数字和文本类型**的数据，比方说名字和电话号码相关的信息。需要有个地方存起来。<br />通常会用**mysql数据库**去存。<br />![文本存在mysql中](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572473651-4305f68b-9188-4a25-8d85-72fc7a67ffb8.png#averageHue=%23e6ebee&clientId=ue93acb14-7bc9-4&from=paste&id=u83aee0f0&originHeight=432&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud80d2aa5-8ed5-4b3c-b7ed-64a8bc86df9&title=%E6%96%87%E6%9C%AC%E5%AD%98%E5%9C%A8mysql%E4%B8%AD "文本存在mysql中")<br />当需要重新将这一数据取出的时候，就需要去读mysql数据库。<br />但因为mysql的数据是存在磁盘上的，单台实例，读性能到差不多5kqps就已经很不错了。<br />看起来还凑合，但对于稍微大一点的系统，就稍微有点捉急了。<br />为了提升点性能，在mysql之前再加一层内存做缓存层，比如常说的redis，读数据优先到内存里读，读不到才到mysql里读，大大减少了读mysql的次数。有了这套组合拳，读性能轻松上万qps。<br />![mysql和redis](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572473663-1e75504d-a3df-462d-b25c-1f5a4e6aa7f0.png#averageHue=%23e5ebed&clientId=ue93acb14-7bc9-4&from=paste&id=u6b666e7f&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u4aff61f0-8165-4051-a9e4-a9fcf8e4945&title=mysql%E5%92%8Credis "mysql和redis")<br />好了，到这里，说的都是平时比较容易接触的开发场景。<br />但如果现在要处理的，**不再是上面提到的文本类数据，而是图片数据**。比如，有一张照片。那么问题来了。**这张图片数据应该存在哪？，又该从哪里读？**<br />回过头去看mysql和redis的场景，无非就是**存储层加缓存层**。<br />![存储层和缓存层](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572473666-a81e383d-aaa8-40bd-b5ff-7f84ddac7a74.png#averageHue=%23d8e5e6&clientId=ue93acb14-7bc9-4&from=paste&id=uaa31c388&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u23356422-5e0e-41ca-b858-3e39afad17f&title=%E5%AD%98%E5%82%A8%E5%B1%82%E5%92%8C%E7%BC%93%E5%AD%98%E5%B1%82 "存储层和缓存层")<br />对于图片这样的**文件对象**，**存储层**不太可能再用mysql，应该改用专业的**对象存储**，比如亚马逊的**S3**（Amazon Simple Storage Service，注意后面是三个S开头的单词，所以叫s3），或者阿里云的**oss**（Object Storage Service）。下面的内容，就用比较常见的oss去做解释。<br />而**缓存层**，也不能继续用redis了，需要改成使用**CDN**（**C**ontent **D**elivery **N**etwork，内容分发网络）。<br />**可以将CDN简单理解为对象存储对应的缓存层。**<br />![CDN和OSS](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572473708-4a50990e-fae8-49ae-ab77-072a866ad0d5.png#averageHue=%23d9e5e6&clientId=ue93acb14-7bc9-4&from=paste&id=u13b175c1&originHeight=405&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u8dcd4129-c578-456a-8ef1-c63388abbfd&title=CDN%E5%92%8COSS "CDN和OSS")<br />现在就可以回答上面的提问，对用户来说，这张图片数据存在了**对象存储**那，当有需要的时候，会从**CDN**那被读出来。
<a name="bC0ko"></a>
## CDN的工作原理
有了CDN和对象存储之后，现在来看下他们之间是怎么工作的。<br />平时看到的图片，可以右键复制查看它的URL。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474062-7aca7250-0e44-4873-90f9-6cc7bc3b55e1.png#averageHue=%238aa7a5&clientId=ue93acb14-7bc9-4&from=paste&id=u39f5ec25&originHeight=563&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uf6149bc7-45bd-411e-a653-bc5a58b2ecc&title=)<br />会发现图片的URL长这样。
```
https://cdn.img.top/1667106197000.png
```
其中前面的cdn.img.top就是CDN的域名，后面的1667106197000.png是图片的路径名。<br />当在浏览器输入这个URL就会发起HTTP GET请求，然后经历以下过程。<br />![CDN的查询流程](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474246-6ae844b2-caea-47b2-95a3-bb9216ff5ba8.png#averageHue=%23dfe7ea&clientId=ue93acb14-7bc9-4&from=paste&id=u522db2ab&originHeight=720&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u86ed2ea5-4c5a-4d23-ad12-c899710939e&title=CDN%E7%9A%84%E6%9F%A5%E8%AF%A2%E6%B5%81%E7%A8%8B "CDN的查询流程")<br />**第一阶段**: 电脑会先通过DNS协议获得cdn.img.top这个域名对应的IP。

- step1和step2：先查看浏览器缓存，再看操作系统里的/etc/hosts缓存，如果都没有，就会去询问最近的DNS服务器（比如房间里的家用路由器）。最近的DNS服务器上有没有对应的缓存，如果有则返回。
- step3：如果最近的DNS服务器上没有对应的缓存，就会去查询根域，一级域，二级域，三级域服务器。
- step4：然后，最近的DNS服务器会得到这个cdn.img.top域名的**别名（CNAME）**，比如cdn.img.top.w.kunlunaq.com。
- kunlunaq.com是阿里**CDN专用的DNS调度系统**。
- step5到step7：此时**最近的DNS服务器**会去请求这个kunlunaq.com，然后返回一个**离你最近的IP地址**返回给你。

**第二阶段**：对应上图里的step8。浏览器拿着这个IP去访问**cdn节点**，然后，cdn节点返回数据。<br />上面第一阶段流程里，提到了很多新的名词，比如CNAME，根域，一级域啥的。<br />**DNS的目的就是通过域名去获得IP地址**。<br />但这只是它的众多功能之一。<br />DNS消息有很多种类型，其中**A类型**，就是用域名去查域名对应的IP地址。而**CNAME类型**，则是用域名去查这个域名的**别名**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474501-965c315b-c50a-4184-91ad-8d73de85c92c.png#averageHue=%23b1d79e&clientId=ue93acb14-7bc9-4&from=paste&id=u1aa80b54&originHeight=617&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u69e742fb-72a1-46a5-b733-5feb1ba5e1b&title=)<br />对于**普通域名**，DNS解析后一般就能直接得到域名对应的IP 地址（又叫A类型记录，A指Address）。<br />比如下面，用dig命令发出DNS请求并打印过程数据。
```bash
$ dig +trace img.top
;; ANSWER SECTION:
img.top. 600 IN A 47.102.221.141
```
可以看到img.top直接解析得到对应的IP地址47.102.221.141。<br />但对于**cdn域名**，一波查询下来，先得到的却是一条CNAME的记录xx.kunlunaq.com，然后dig这个xx.kunlunaq.com才能得到对应的IP地址。
```bash
$ dig +trace cdn.img.top
cdn.img.top. 600 IN CNAME cdn.img.top.w.kunlunaq.com.

$ dig +trace cdn.xiaobaidebug.top.w.kunlunaq.com
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.243
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.241
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.244
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.249
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.248
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.242
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.250
cdn.img.top.w.kunlunaq.com. 300 IN A 122.228.7.251
```
看到这里，问题就又来了。
<a name="PFomT"></a>
### 为什么要加个CNAME那么麻烦？
CNAME里指向的，其实是**CDN专用的DNS域名服务器**，它对整个DNS体系来说，只是其中一台小小的DNS域名服务器，**看起来就跟其他域名服务器一样，平平无奇**。DNS请求也会正常打入这个服务器里。<br />但当请求真正打到它上面的时候，它的特别之处就体现出来了，当查询请求打入域名服务器时，**普通的DNS域名服务器**返回域名对应的部分IP就够了，但**CDN专用的DNS域名服务器**却会要求返回离调用方"**最近的**"服务器IP。<br />![CDN专用的DNS解析服务器会返回就近的CDN节点IP](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474239-d2cc344c-c0fe-47da-949b-a052133d487a.png#averageHue=%23e5ebed&clientId=ue93acb14-7bc9-4&from=paste&id=ud57e760b&originHeight=756&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue684ed43-effd-43d1-81fe-585c4a6b3a7&title=CDN%E4%B8%93%E7%94%A8%E7%9A%84DNS%E8%A7%A3%E6%9E%90%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%BC%9A%E8%BF%94%E5%9B%9E%E5%B0%B1%E8%BF%91%E7%9A%84CDN%E8%8A%82%E7%82%B9IP "CDN专用的DNS解析服务器会返回就近的CDN节点IP")
<a name="PZENi"></a>
### 怎么知道哪个服务器IP里调用方最近？
可以看到"最近"这个词其实是加了**双引号**的。<br />**CDN专用的DNS域名服务器**其实是CDN提供商提供的，比如阿里云当然知道自己的的**CDN节点**有哪些，以及这些CDN服务器目前的负载情况和响应延时甚至权重啥的，并且也能知道调用方的IP地址是什么，可以通过调用方的IP知道它所属的运营商以及大概所在地，根据条件筛选出**最合适**的CDN服务器，这就是所谓的"**最近**"。<br />举个例子。假设地理位置最近的CDN机房流量较多，响应较慢，但地理位置远一些的服务器却能更好的响应当前请求，那按理说**可能**会选择地理位置**远一些**的那台CDN服务器。<br />也就是说，选出来的服务器**不一定在地理位置最近**，但一定是当前**最合适**的服务器。
<a name="J6c6R"></a>
## 回源是什么
上面的图片URL，是https://cdn域名/图片地址.png的形式。<br />也就是说这张图片是访问CDN拿到的。<br />那么，**直接访问对象存储能不能拿到图片数据并展示？**<br />比如像下面这样。
```
https://oss域名/图片地址.png
```
这就像问，不走redis，直接从mysql中能不能读取到文本数据并展示一样。<br />**当然能。**<br />之前放在博客里的图片就是这么干的。<br />但这样成本更高，这里的**成本**，可以指**性能成本**，也可以指**调用成本**。看下下面这个图。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474662-a284bf83-08ec-4bf8-8754-85ff2b5b5752.png#averageHue=%23eae8e8&clientId=ue93acb14-7bc9-4&from=paste&id=u16e750c5&originHeight=272&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6a2d6ac9-ba35-4f81-8952-a98d4f3d554&title=)<br />可以看到直接请求oss的费用差不多是通过cdn请求oss的两倍，考虑到家境贫寒，同时也为了让博客获取图片的速度更快，就接入了CDN。<br />但看到这里，问题又又来了。<br />上面的截图里，红框里有个词叫"**回源**"。<br />**回源是什么？**<br />当访问https://cdn域名/图片地址.png时，请求会打到cdn服务器上面。<br />但cdn服务器本质上就是一层缓存，并不是数据源，**对象存储才是数据源**。<br />**第一次**访问cdn获取某张图片时，大概率在cdn里并没有这张图片的数据，因此需要**回**到数据**源**那去取出这份图片数据。然后再放到cdn上。下次再次访问cdn时，只要缓存不过期，就能命中缓存直接返回，这就不需要再回源。<br />于是访问的过程就变成了下面这样。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474645-f6e76f3e-001d-4186-91fa-1700478b215d.png#averageHue=%23e0e8eb&clientId=ue93acb14-7bc9-4&from=paste&id=u9a153ff3&originHeight=747&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u5c0f0979-de52-462e-851f-743569132ee&title=)<br />**那还有哪些情况会发生回源呢？**<br />除了上面提到的cdn上拿不到数据会回源站外，还有cdn上的**缓存过期失效了**也会导致回源站。<br />另外，就算有缓存，且缓存不过期，也可以通过cdn提供的**开放接口来触发主动回源**，但这个比较少机会能接触到。<br />另外，回源这个事情，其实用户是**感知不到**的，因为用户去读图片的时候，只能知道自己读到了还是读不到。<br />同样是**读到了**，还细分为是**从cdn那直接读的**，还是**cdn回源读对象存储之后返回的**。<br />![有缓存直接返回和没缓存回源的区别](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572474942-360f8c06-5610-4119-b83a-88848fd0f023.png#averageHue=%23e4ebed&clientId=ue93acb14-7bc9-4&from=paste&id=u3b4d875b&originHeight=540&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue642f5cb-13f1-491d-a04a-b977a5969dd&title=%E6%9C%89%E7%BC%93%E5%AD%98%E7%9B%B4%E6%8E%A5%E8%BF%94%E5%9B%9E%E5%92%8C%E6%B2%A1%E7%BC%93%E5%AD%98%E5%9B%9E%E6%BA%90%E7%9A%84%E5%8C%BA%E5%88%AB "有缓存直接返回和没缓存回源的区别")<br />那么，**有办法判断是否发生过回源吗？**<br />有。接着往下看。
<a name="rQLmU"></a>
## 怎么判断是否发生回源
以某里云的对象存储和CDN为例。<br />假设要请求下面这张图https://cdn.img.top/image/image-20220404094549469.png<br />为了更方便的查看响应数据的http header，可以用上postman。<br />通过GET方法去请求图片数据。<br />然后通过下面的tab切换查看response header信息。<br />![查看response header](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475056-7e96598b-7c6b-4cb2-a607-49774ee8f871.png#averageHue=%23e7ebe7&clientId=ue93acb14-7bc9-4&from=paste&id=u17473f84&originHeight=687&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ue54dbf8b-a8f8-4646-b606-8c81aa87f86&title=%E6%9F%A5%E7%9C%8Bresponse%20header "查看response header")<br />![回源的情况](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475091-f01a6fc2-2626-4ace-bdbc-cdc180fa28dd.png#averageHue=%23f7f6f6&clientId=ue93acb14-7bc9-4&from=paste&id=u4fa3aab9&originHeight=1105&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u2c2f543a-8e9b-4cfa-a9aa-3e69be5c0dd&title=%E5%9B%9E%E6%BA%90%E7%9A%84%E6%83%85%E5%86%B5 "回源的情况")<br />此时查看response header下的X-Cache的值是 MISS TCP_MISS。意思是未命中缓存导致CDN回源查oss，拿到数据后再返回。<br />那此时CDN里肯定是有这张图片的缓存了。可以试着**再执行**一次 GET 方法获取图片。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475217-edbc3152-837b-4d8b-8ff1-a4665fe0b162.png#averageHue=%23f7f6f6&clientId=ue93acb14-7bc9-4&from=paste&id=ue813ada3&originHeight=1091&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1a680ce6-4412-4ee8-b2ed-b77b2e636db&title=)<br />X-Cache的值就变成了 HIT TCP_MEM_HIT，这就是命中缓存了。<br />这个是某里云的做法，其他比如腾某云啥的，也都大差不差，几乎都可以从response header里找到相关的信息。
<a name="gWpa1"></a>
## 用了CDN一定比不用的更快吗？
看到这里就可以回答文章开头的问题了。<br />如果没有接入CDN，直接访问源站，流程是这样的。<br />![更新直接访问源站](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475416-7685127e-c6b1-47fd-8863-f2a7efa232d8.png#averageHue=%23e7ecef&clientId=ue93acb14-7bc9-4&from=paste&id=u2c25c129&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u144f441f-593f-4ee4-9b8c-685dded5fa0&title=%E6%9B%B4%E6%96%B0%E7%9B%B4%E6%8E%A5%E8%AE%BF%E9%97%AE%E6%BA%90%E7%AB%99 "更新直接访问源站")<br />但如果接入了CDN，且CDN上没有缓存数据，那就会触发回源。<br />![更新走了CDN还回源](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475635-ed6ff548-28fc-4492-a6ec-ee9a88ec569c.png#averageHue=%23e5ebed&clientId=ue93acb14-7bc9-4&from=paste&id=ue40efb87&originHeight=306&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=u9591719b-86d8-4f43-8328-a9f26c64388&title=%E6%9B%B4%E6%96%B0%E8%B5%B0%E4%BA%86CDN%E8%BF%98%E5%9B%9E%E6%BA%90 "更新走了CDN还回源")<br />相当于在原来的流程上还多了一层CDN的调用流程。<br />也就是，用了CDN时，**未命中CDN缓存**导致**回源**，就会比不用的时候更慢。<br />未命中缓存，可能是**cdn里压根就没这一数据**，也可能是**曾经有这条数据但后来过期失效了**。<br />这两种情况都正常，**大部分时候并不需要做任何处理。**<br />但对于极个别场景，可能需要做些优化。比如源站数据有大版本更新，就像**更换cdn域名**啥的，那在上线的那一刻用户全用新cdn域名去请求图片啥的，新CDN节点基本上百分百触发回源，严重的时候甚至可能会拖垮对象存储。这时候可能需要提前将热点数据筛选出来，利用工具预先请求一波，让CDN加载上热数据缓存。比如某里云上的CDN就有这样的"**刷新预热**"功能。<br />![cdn刷新预热](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475679-0dd7b79e-efc2-4552-9327-b4e91c482494.png#averageHue=%23f8f6f6&clientId=ue93acb14-7bc9-4&from=paste&id=ub7001a93&originHeight=906&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=true&status=done&style=none&taskId=ud397a017-37a2-49cb-a863-b629298ff82&title=cdn%E5%88%B7%E6%96%B0%E9%A2%84%E7%83%AD "cdn刷新预热")<br />当然也可以通过**灰度发布**的模式，先让少量用户体验新功能，让这些用户把cdn"热"起来，然后再逐步放开流量。<br />还有就是**曾经有这条数据但后来过期失效**了，对于热点数据，可以适当提高一下cdn数据的**缓存时间**。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572476112-c6c49612-834a-4cf2-adf1-3754bf5b3b34.png#averageHue=%23bebdbc&clientId=ue93acb14-7bc9-4&from=paste&id=u918e1337&originHeight=591&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud48ef503-a64a-4086-8ca2-6631e7c1d37&title=)
<a name="CpU4i"></a>
## 什么情况下不应该使用CDN？
从上面的描述看下来，CDN最大的优势在于，对于来自世界各地的用户，它可以**就近分配CDN节点**获取数据，并且**多次重复获取同一个文件**数据的时候，有缓存加速的作用。<br />这对于网页图片这样的场景，是再合适不过了。因为底层用的是**对象存储**，也就是说，只要是文件对象，比如**视频**啥的，都可以用这套流程接入cdn做加速。比如平时刷的某音某手短视频就是这么干的。<br />那反过来想想，问题就来了。<br />**什么情况下不应该使用CDN？**<br />如果有一个公司**内网**的服务，并且服务请求的图片等文件不太可能被**多次重复调用**，这时候其实没必要使用CDN。<br />注意上面两个**加粗**了的关键点。

- **内网服务**，是为了保证你是了解服务的请求来源的，也能拿到对象存储的**读权限**，并且如果对象存储也是公司内部的，那大概率跟你的服务已经在**同一个机房**里，这已经**很近了**。接入CDN也享受不到"就近分配CDN节点"所带来的好处。
- 图片或其他文件**不太可能被多次重复使用**，如果接入了CDN，那每次去访问CDN获取图片的时候，CDN节点上大概率没有你要的数据，相当于每次都需要**回源**到对象存储去取一把。那接入CDN相当于给自己加了一层代理，多一层代理，就多一层耗时。

![](https://cdn.nlark.com/yuque/0/2023/png/396745/1673572475997-d3a4dede-ee8a-4b88-8740-e61d3b38574e.png#averageHue=%23e6ecee&clientId=ue93acb14-7bc9-4&from=paste&id=ua849550e&originHeight=648&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u0ab51847-0ec5-48d4-963e-47e0dbca7b7&title=)<br />关于上面的第二点，如果需要一个明确的指标去说服自己，那我可以给你一个。从上面的介绍内容，可以通过cdn响应的http header中的X-Cache字段，看到一个请求是否触发过回源，统计次数，再除以总的请求数，就能得到回源的比例，比如回源比例高达90%，那还接啥cdn。
<a name="mWJYp"></a>
## 总结

- 对于文本类数据习惯用mysql做存储，redis做缓存。但属于文件类数据，比如视频图片，则需要使用oss等做对象存储，cdn做缓存。
- 用了CDN如果发生回源，那实际上会比不用的时候更慢一些。
- CDN最大的优势在于，对于来自世界各地的用户，它可以**就近分配CDN节点**获取数据，并且**多次重复获取同一个文件**数据的时候，有缓存加速的作用。如果服务和对象存储都在内网，并且文件数据也不太会有重复使用的可能性，那其实没必要接入cdn。
