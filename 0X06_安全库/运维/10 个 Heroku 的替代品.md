很多人都喜欢尝试新的框架和工具，然后用它创建一个小项目，发布到 GitHub 上，并提供一个可用于演示的链接，这样大家就不需要下载项目、初始化、安装依赖，然后运行等一系列复杂的步骤。<br />过去，可以把项目上传到 Heroku，因为它可以免费托管项目，由于这些项目只是一些演示，所以配置低的免费机器就可以。但是现在，Heroku 宣布他们将关闭所有免费的 dynos、postgress 和 Redis 存储，所以要么升级到付费，要么寻找替代品。<br />为一些 Demo 付费是不值得的，所以，这里推荐几个免费的，可以作为 Heroku 的替代品。
<a name="sY7nN"></a>
## 1、Cloudflare Pages
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679113645154-3e8eaf82-cf0b-46d1-9811-e997ba178ab7.png#averageHue=%23fdfcfb&clientId=udf87d9a1-7098-4&from=paste&height=713&id=u0b17e9f6&originHeight=1783&originWidth=3794&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=423556&status=done&style=none&taskId=u44b0540e-bb77-4600-9210-3f9118b5a7e&title=&width=1517.6)<br />[Cloudflare Pages](https://pages.cloudflare.com/)无限带宽，无需任何更改即可支持 CDN，它允许自定义域，甚至它有一个非常慷慨的无服务器功能免费计划。
<a name="aNdU1"></a>
## 2、netlify
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679113607987-794906dc-d148-4e27-9152-95e8b23c4139.png#averageHue=%23f3c68e&clientId=udf87d9a1-7098-4&from=paste&height=712&id=u5dc0b002&originHeight=1780&originWidth=3797&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=761262&status=done&style=none&taskId=ue7072270-1976-42c8-b1d3-0e5cd20ac7a&title=&width=1518.8)<br />如果喜欢更流行知名度更高的，[Netlify](https://www.netlify.com/) 是个不错的选择，有上万的用户使用。
<a name="icgUQ"></a>
## 3、Deta 适用于 Node.js 和 Python
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679118531841-a212e032-1013-4c6d-9add-5b1b1c6cf82f.png#averageHue=%23f0e0f4&clientId=udf87d9a1-7098-4&from=paste&height=713&id=ua8bd9b60&originHeight=1783&originWidth=3795&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1476652&status=done&style=none&taskId=uf99a92c8-36f6-4b88-9cf3-e1e91ce8b26&title=&width=1518)<br />[Deta](https://www.deta.sh/)承诺将永远免费，可以在几秒钟内享受部署！
<a name="HAqit"></a>
## 4、Firebase (Google提供)
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679118771468-d0af0f34-5b3e-403b-93b2-5b9d4f1b059f.png#averageHue=%23317add&clientId=udf87d9a1-7098-4&from=paste&height=713&id=ud4cd8774&originHeight=1783&originWidth=3795&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1275807&status=done&style=none&taskId=u4aafccd5-f7c6-4b75-a5f7-728f9b84bae&title=&width=1518)<br />如果已经在使用其他 Google 服务并且希望彼此轻松集成，或者只是喜欢 Google 本身，[Firebase](https://firebase.google.com/) 提供了一个极好的免费计划！
<a name="XBEJ0"></a>
## 5、Render
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679118799989-5469f9c7-2fd6-49d1-b768-f80dc4362013.png#averageHue=%23ced5ec&clientId=udf87d9a1-7098-4&from=paste&height=713&id=u41c17989&originHeight=1783&originWidth=3794&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=372151&status=done&style=none&taskId=uf9d35231-cd1d-47ef-b2a5-add28fd0482&title=&width=1517.6)<br />[Render](https://render.com/)速度极快，还有很多免费的附加功能，如果需要一个免费提供快速服务器和 DB 和 Redis 的新提供商，那么这就是最佳选择！
<a name="NyBOe"></a>
## 6、Railway （多语言支持）
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679119098249-4ac1ed08-821b-49ef-abed-cca2dcdcd4b6.png#averageHue=%23181323&clientId=udf87d9a1-7098-4&from=paste&height=713&id=u554087c1&originHeight=1783&originWidth=3840&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1061261&status=done&style=none&taskId=ud0a81d45-400b-4514-81c7-11562f56b17&title=&width=1536)<br />如果需要开箱即用地支持 Node.JS、Django、Laravel、Kotlin、Spring、Ruby，[Railway](https://railway.app/)就是你的解决方案。它具有友好的用户界面并支持许多框架和语言。
<a name="Hws8B"></a>
## 7、Fly.io （利用 Dockerfile 部署）
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679119474267-f0d8e405-34b9-447c-aa5a-4bf9921355c7.png#averageHue=%23181a33&clientId=udf87d9a1-7098-4&from=paste&height=713&id=u1514232e&originHeight=1783&originWidth=3791&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1612404&status=done&style=none&taskId=uc5150906-1910-434c-bffb-8c01dc9b2fc&title=&width=1516.4)[Fly.io](https://fly.io/)，部署 Dockerfile 的最佳选择。
<a name="mRUDh"></a>
## 8、Fleek.co
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679119853226-781cc1ce-1d25-4eab-9cf0-0683b6d3cda3.png#averageHue=%23404040&clientId=udf87d9a1-7098-4&from=paste&height=713&id=u6e6dc450&originHeight=1783&originWidth=3796&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=651540&status=done&style=none&taskId=u3a392c00-a7e2-4ab8-bf8c-5506b10eaee&title=&width=1518.4)[Fleek.co](https://fleek.co/) 它就像在 Netlify 上部署一样简单，但支持 Web3，因此页面可以永久存储在 IPFS 上（即使域名过期），而无需处理 web3 开发的复杂性。
<a name="Rk6SR"></a>
## 9、Qoddi.com
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679120205635-5268574c-5f61-4c4f-8176-4a89294ce9b6.png#averageHue=%23e4e8ef&clientId=udf87d9a1-7098-4&from=paste&height=713&id=uec8bebbb&originHeight=1783&originWidth=3798&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=1062351&status=done&style=none&taskId=ua213f5ec-6011-4439-b26d-01011706954&title=&width=1519.2)<br />[Qoddi.com](https://qoddi.com/) 这个网站和其他类似，但有一个好处就是它与 Heroku buildpacks 兼容，因此可以将 Heroku 项目移至此处而无需进行任何更改！
<a name="RR5XA"></a>
## 10、Gigalixir.com
![image.png](https://cdn.nlark.com/yuque/0/2023/png/396745/1679119950300-e95a04ff-fd96-40eb-8222-7c32e40b73c7.png#averageHue=%23eef2f9&clientId=udf87d9a1-7098-4&from=paste&height=713&id=Kwsme&originHeight=1783&originWidth=3800&originalType=binary&ratio=2.5&rotation=0&showTitle=false&size=294878&status=done&style=none&taskId=ub41ee052-1efb-4f9f-b262-030c716d808&title=&width=1520)[Gigalixir.com](https://www.gigalixir.com/)，Elixir 开发人员可以使用这个，适用于免费制作原型和测试小型应用程序。
<a name="oPgTv"></a>
## 参考资料
Cloudflare Pages: [https://pages.cloudflare.com/](https://pages.cloudflare.com/)<br />Netlify: [https://www.netlify.com/](https://www.netlify.com/)<br />Deta: [https://www.deta.sh/](https://www.deta.sh/)<br />Firebase: [https://firebase.google.com/](https://firebase.google.com/)<br />Render: [https://render.com/](https://render.com/)<br />Railway: [https://railway.app/](https://railway.app/)<br />Fly.io: [https://fly.io/](https://fly.io/)<br />Fleek.co: [https://fleek.co/](https://fleek.co/)<br />Qoddi.com: [https://qoddi.com/](https://qoddi.com/)<br />Gigalixir.com: [https://www.gigalixir.com/](https://www.gigalixir.com/)
