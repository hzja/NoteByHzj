机器学习 神经网络
<a name="a8yvq"></a>
## 1、GAN 是什么
GAN 的全称是 Generative Adversarial Network，中文是**生成对抗网络**。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949673826-1d05be08-fb6a-473c-bba2-394209d527b5.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u9e1c7d1a&originHeight=326&originWidth=988&originalType=url&ratio=1&status=done&style=none&taskId=u8b7f6016-fd3a-4e78-972d-a6c241707ac)<br />一言以蔽之，GAN 包含了两个神经网络，**生成器**（generator）和**辨别器**（discriminator），两者互相博弈不断变强，即生成器产出的东西越来越逼真，辨别器的识别能力越来越牛逼。
<a name="rTAJg"></a>
## 2、造假和鉴定
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949673730-f4499658-4e6e-4d8f-b6ee-3671324d149d.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u37d381bb&originHeight=414&originWidth=855&originalType=url&ratio=1&status=done&style=none&taskId=u101633cf-1508-4836-85ce-9588fd9ce0f)<br />**生成器**和**辨别器**之间的关系很像**造假者**（counterfeiter）和**鉴定者**（Appraiser）之间的关系。

- **造假者**不断造出假货，目的就是蒙骗**鉴定者**，在此过程中其**造假**能力越来越高。
- **鉴定者**不断检验假货，目的就是识破**造假者**，在此过程中其**鉴定**能力越来越高。

GAN 是造假者的，也是**鉴定者**的，但归根结底还是**造假者**的。GAN 的最终目标是训练出一个“完美”的**造假者**，即能让生成让**鉴定者**都蒙圈的产品。<br />一动图胜千言，下图展示“**造假者**如何一步步生成逼真的蒙娜丽莎画而最终欺骗了**鉴定者**”的过程。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949673744-e66056b9-ae7e-4c4e-b0c6-7ceec9d093c9.gif#clientId=u22ec4da5-d95b-4&from=paste&id=uf0f55248&originHeight=436&originWidth=1010&originalType=url&ratio=1&status=done&style=none&taskId=uad39ebb6-1753-4a65-a3a9-cdc384c11b9)<br />在此过程中，每当造假者生成一幅图。鉴定者会给出反馈，造假者从中学到如何改进来画出一张逼真图。
<a name="OWjpq"></a>
## 3、造假鉴定网络？
回到神经网络，**造假者**用**生成器**来建模，**鉴定者**用**辨别器**来建模。

![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949673743-a8f1fdde-0702-4cd6-bf88-343512910176.gif#clientId=u22ec4da5-d95b-4&from=paste&id=ua538d0b0&originHeight=565&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u4554a10b-2925-4358-ae2d-07adaa495e4)<br />根据上面动图可知，**辨别器**的任务是区分哪些图片是真实的，哪些图片是**生成器**产生的。<br />**接下来用 Python 创建一个极简 GAN。**<br />**首先设置一个故事背景。**
<a name="P67Ln"></a>
## 4、故事背景
在倾斜岛（slanted island）上，每个人都是倾斜的，大概像左倾斜 45 度左右。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949673727-15cef046-5a8e-49bf-936d-6507a71e7de1.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u18f68986&originHeight=324&originWidth=478&originalType=url&ratio=1&status=done&style=none&taskId=uc2bd3fa1-c7ed-4aaa-bcb5-feacf66f25e)<br />岛主想做**人脸生成器**，由于岛上的人的脸部特征非常简单，因此用 2 * 2 像素的模糊人脸图片。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674160-7d8a5d9e-67ed-4faa-8d8b-491566de739a.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u6c57045c&originHeight=332&originWidth=804&originalType=url&ratio=1&status=done&style=none&taskId=u327b81cd-bfa0-4215-8238-18f68d179f9)<br />限于技术，岛主只用了个一层的神经网络。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674310-77a6c61f-c765-4dfe-870d-839416b28c26.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uc250e2b8&originHeight=324&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=udab7df5c-a3e3-48b2-afb5-381249a2b2c)<br />但在这个极度简单的设置下，一层的 GAN 也能生成“倾斜人脸”。
<a name="xo2Y3"></a>
## 5、辨别人脸
下图展示四个人脸的样子。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674241-41ffdaed-813b-410c-b91d-6ef2fc1b4366.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ub9ff81a4&originHeight=219&originWidth=960&originalType=url&ratio=1&status=done&style=none&taskId=uc2cb6681-5590-4a99-a9c5-e6e982cb369)<br />从 2*2 像素来表示人脸，深色代表此处有人脸，浅色代表此处没有人脸。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674668-17f3a736-0759-444b-a493-823f4bbcc467.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u1c94bf37&originHeight=451&originWidth=977&originalType=url&ratio=1&status=done&style=none&taskId=uae4fafd7-deb4-4b43-96b6-62bb84fc79c)<br />如果不是人脸呢？那么其 2*2 像素图中的元素就是随机的，如下所示。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674680-138d1faa-e8db-49c1-8d1a-06f18ecfcd34.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ucf22979d&originHeight=362&originWidth=1035&originalType=url&ratio=1&status=done&style=none&taskId=u30cef4c2-47f2-4768-a52f-2683f8ac389)<br />复习一下：

- **人脸**：对角线上是深色，非对角线上是浅色
- **非人脸**：任意四处都可能是深色或浅色

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674744-5da1bed8-96e8-4485-acc8-b1b4c93e4220.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u984836ed&originHeight=386&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u6a502b9c-3c5a-4ea6-ae2c-09211543b57)<br />像素可以用 0 到 1 的数值来表示：

- **人脸**：对角线上的数值大，非对角线上的数值小
- **非人脸**：任意四处都可能是 0-1 之间的任意数值

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674796-1edb7352-6775-475d-b844-454b3b65cf71.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u89f90a24&originHeight=592&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u5348b90d-0708-46aa-9c92-89f28e65ef2)<br />弄清了人脸照片和非人脸照片用不同特征的 2*2 数值矩阵表示之后，接下来两节来看如何构建**辨别器**（discriminator）和**生成器**（generator）。<br />先分析**辨别器**。
<a name="Jb5gP"></a>
## 6、辨别器
辨别器就是用来辨别人脸，那么当看到照片的像素值时，如何辨别呢？<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675064-d15f8adc-1c76-4b74-aa90-a70a2082fb1d.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u63aa9aea&originHeight=271&originWidth=834&originalType=url&ratio=1&status=done&style=none&taskId=u115f1964-1241-4fd9-8ecd-2db8d6bd17a)<br />简单！上节已经分析过：

- **人脸**：对角线上的数值大，非对角线上的数值小
- **非人脸**：任意四处都可能是 0-1 之间的任意数值

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674915-d54b7b02-4930-4deb-86b8-52881b649aeb.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u66608ff1&originHeight=361&originWidth=1054&originalType=url&ratio=1&status=done&style=none&taskId=ueb5f25e2-e3c1-4432-a81a-e3e1c695375)

---

如果要用一个数值表示人脸和非人脸，该用什么样的操作呢？也简单，如下图所示，加上 (1,1) 位置的元素，减去 (1,2) 位置的元素，减去 (2,1) 位置的元素，加上 (2,2) 位置的元素，得到一个数值就可以了。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949674987-83ac38ba-9dbd-4dc1-96e2-b8b82b90b9c9.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u54616daa&originHeight=362&originWidth=1047&originalType=url&ratio=1&status=done&style=none&taskId=u28a4f374-c108-4b86-b9bb-e7aa55cd999)

---

人脸得到的分数是 2（较大），非人脸得到的分数是 -0.5（较小）。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675250-330ab59a-90cf-4c18-be08-d85e18e2e3bc.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u5d0c33fa&originHeight=474&originWidth=1057&originalType=url&ratio=1&status=done&style=none&taskId=u63b9c330-804b-4fef-9512-3082f8777e5)<br />设定一个**阈值 1**，得分大于 1 是人脸，小于 1 不是人脸。<br />将上述内容用神经网络来表示，就成了下面的极简**辨别器**了。注意除了“加减减加”矩阵 4 个元素之外，最后还加上一个**偏置项**（bias）得到最终得分。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675220-7dc6e78c-563c-46cd-bffe-fc0751e8f1de.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u9eea1445&originHeight=572&originWidth=707&originalType=url&ratio=1&status=done&style=none&taskId=ue271e8e4-2b68-4a49-85b2-f45fed82859)

---

**辨别器**最终要判断是否是人脸，因此产出是一个概率，需要用 sigmoid 函数将得分 1 转化成概率 0.73。给定概率阈值 0.5，由于 0.73 > 0.5，**辨别器**判断该图是人脸。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675269-ef1958fd-b5db-4d63-9172-a1a807bbed04.webp#clientId=u22ec4da5-d95b-4&from=paste&id=udafbb003&originHeight=612&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uc4d54111-18f7-424c-a349-25093b31f31)

---

对另一张非人脸的图，用同样操作，最后算出得分 -0.5，用 sigmoid 函数转换。给定概率阈值 0.5，由于 0.37 < 0.5，**辨别器**判断该图是人脸。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675353-ecae68ef-2f37-4811-9e00-4701f96b8559.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u7007ef71&originHeight=613&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u093e3542-74b0-4766-8b3a-6161f2841ff)
<a name="Lz3w2"></a>
## 7、生成器
**辨别器**目标是判断人脸。而**生成器**目标是生成人脸，那什么样的矩阵像素是人脸图呢？简单！该规则被已经分析多次了：

- **人脸**：对角线上的数值大，非对角线上的数值小
- **非人脸**：任意四处都可能是 0-1 之间的任意数值

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675436-3a39714c-c624-4067-8289-112484216e35.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u6661f130&originHeight=361&originWidth=1054&originalType=url&ratio=1&status=done&style=none&taskId=u975c52d3-45e4-4c93-9e96-e283dc86f86)

---

现在来看生成过程。第一步就是从 0-1 之间随机选取一个数，比如 0.7。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675755-7b425888-467d-4eff-9180-3b9f1c842f01.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u745106db&originHeight=591&originWidth=596&originalType=url&ratio=1&status=done&style=none&taskId=u74fd155a-f89d-4e9b-a79b-4508fe5f3c2)<br />回忆生成器的目的是生成人脸，即要保证最终 2*2 矩阵的对角线上的像素要大（用粗线表明），而非对角线上的像素要小（用细线表明）。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675634-3750eeeb-6343-4453-9d36-d349682c0516.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uf25bd185&originHeight=572&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ud33a68fb-c274-4220-bb3f-ccfac421cc2)

---

举例，生成矩阵 (1,1) 位置的值，w = 1, b = 1，计算的分 wz + b = 1.7。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675643-cb26af9f-1176-4bca-bdea-e227887fbe1d.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u1d192b94&originHeight=570&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ud7119aef-d4d2-42d8-a214-dace9b32315)

---

同理计算矩阵其他三个位置的得分。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675776-408fc1e4-d46f-4b9b-ae8b-979f37770a11.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u15f5f2fe&originHeight=575&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u5b0c69ae-a2f5-4445-839f-6c64bc46492)

---

最后都用 sigmoid 函数将得分转换一下，确保像素值在 0-1 之间。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675908-37b0778e-d888-4f53-a18d-e254ae4452b5.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u6ceaaf82&originHeight=569&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u54800c37-ea75-43f8-9e58-a638a7f6bb7)<br />注意按上图这样给权重 [1, -1, -1, 1] 和偏置 1，有因为 z 总是在 0 和 1 之间的一个正数，这样的一个神经网络（生成器）总可以生成一个像人脸的 2*2 的像素矩阵。<br />根据本节和上节的展示，已经知道什么样的**辨别器**可以判断人脸，什么样的**生成器**可以生成好的人脸，即什么样的 GAN 是个好 GAN。这些都是由权重和偏置决定的，接下来看看它们是怎么训练出来的。首先复习一下误差函数（error function）。
<a name="rbkH9"></a>
## 8、误差函数
通常把正类用 1 表示，负类用 0 表示。在本例中人脸是正类，用 1 表示；非人脸是负类，用 0 表示。<br />当标签为 1 时（人脸），**-ln(x) **是一个好的误差函数，因为

- 当预测不准时（预测非人脸，假设 0.1），那么误差应该较大，- ln(0.1) 较大。 
- 当预测准时（预测人脸，假设 0.9），那么误差应该较小，-ln(0.9) 较小。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949675961-e4ef7f3e-9281-417f-a035-73b73ddad1cd.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uc4d9f83f&originHeight=316&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uffad20bb-e54a-462d-8101-741272bd16c)

---

当标签为 0 时（非人脸），**-ln(1-x)** 是一个好的误差函数。

- 当预测准时（预测非人脸，假设 0.1），那么误差应该较小，- ln(1-0.1) 较大。 
- 当预测不准时（预测人脸，假设 0.9），那么误差应该较大，-ln(1-0.9) 较小。

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949676285-31237666-8db4-499b-b80e-8b768f57d26f.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u0a2566fc&originHeight=435&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u3605a104-a5dd-4eb9-989f-3c4bd3953d1)

---

根据下面两张总结图再巩固一下 ln 函数作为误差函数的逻辑。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949676129-3410b6e1-bdb5-43e1-b7fb-1d9e263ba5c2.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u150a2593&originHeight=624&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u2c7df3eb-200c-4f17-8393-eb5a3316bdb)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949676529-d8f6669b-120b-41b0-b546-1dc374ea262a.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uf7de3d01&originHeight=654&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ud4e80678-9235-4a46-8783-52c78510c03)<br />接下来就是 GAN 中博弈，即**生成器**和**辨别器**放在一起会发生什么事情。
<a name="n72jM"></a>
## 9、生成器和辨别器放在一起
复习一下两者的结构。

- **生成器：**输入是一个 0-1 之间的随机数，输出是图片的像素矩阵
- **辨别器：**输入是图片像素矩阵，输出是一个概率值

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949676302-7b82afec-20f6-4c38-8e75-906ad6069aa6.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ueacc1b36&originHeight=595&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u0586c03e-4766-4075-b33c-121c69803c5)

---

下面动图展示了从**生成器**到**辨别器**的流程。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949676296-485b7810-3b93-42c4-afe6-66a26cda09f8.gif#clientId=u22ec4da5-d95b-4&from=paste&id=u776ab498&originHeight=532&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u03461ae1-a1bd-4102-8f9d-24f0a17b0bd)

---

因为该图片是从**生成器**来的，不是真实图片，因此一个好的**辨别器**会判断这不是脸，那么使用标签为 0 对应的误差函数，-ln(1-prediction)。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949676884-b518c2ae-0225-40d1-a923-304f4ca8083a.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ub5231fb0&originHeight=515&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uaf4c8c8e-359d-42eb-87db-e9f79b0f462)<br />反过来，一个好的**生成器**想骗过辨别器，即想让**辨别器**判断这是脸，那么使用标签为 1 对应的误差函数，-ln(prediction)。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949676764-de2acad3-bab9-49ff-b8a4-7761e5ae650d.gif#clientId=u22ec4da5-d95b-4&from=paste&id=u7f89a067&originHeight=526&originWidth=1078&originalType=url&ratio=1&status=done&style=none&taskId=ue4907bf4-ef30-471a-a89e-f6ddd991103)

---

好戏来了，用 G 表示**生成器**，D 表示**辨别器**，那么

- G(z) 是**生成器**的产出，即像素矩阵，它也是**辨别器**的输入
- D(G(z)) 是**辨别器**的产出，即概率，又是上面误差函数里的 prediction

为了使**生成器**和**辨别器**都变强，希望最小化误差函数<br />-ln(D(G(z)) - ln(1-D(G(z))<br />其中 D(G(z)) 就是辨别器的 prediction。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949677533-d26412f8-052e-4c53-a2f9-c075e75a09fe.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u2e02cba3&originHeight=593&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u8152e148-4c18-4327-a535-7cba6e0a019)

---

将得到的误差函数对比 GAN 论文中的目标函数（下图），发现还是有些差别：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949677728-2e57a71d-c403-4399-b0fa-ae1b8993c921.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uc795bd25&originHeight=104&originWidth=611&originalType=url&ratio=1&status=done&style=none&taskId=u3783866c-4a15-4aea-ac1d-baf5f1a2ddf)<br />解释如下：<br />**辨别器**除了接收生成器产出的图片 G(z)，还会接收真实图片 x，在这时一个好的**辨别器**会判断这是脸，那么使用标签为 1 对应的误差函数，-ln(-prediction)。那么对于**辨别器**，需要最小化的误差函数是<br />-ln(D(x)) - ln(1-D(G(z))<br />将负号去掉，等价于最大化<br />ln(D(x)) + ln(1-D(G(z))<br />这个不就是 V(D,G) 么？此过程是固定**生成器**，来优化**辨别器**来识别假图片。<br />V(D, G) 最大化后，在固定**辨别器**，来优化**生成器**来生成以假乱真的图片。但是生成器的误差函数不是 -ln(D(G(z)) 吗？怎么能和 V(D, G) 扯上关系呢？其实 -ln(D(G(z)) 等价于 ln(1-D(G(z))，这时 V(D, G) 的第二项，而其第一项 ln(D(x)) 对于 G 是个常数，加不加都无所谓。<br />最后 V(D, G) 中的两项都有期望符号，在实际优化中就通过 n 个样本的统计平均值来实现。第一项期望中的 x 从真实数据分布 p_data(x) 中来，第一项期望中的 z 从特定概率分布 p_z(z) 中来。<br />综上，先通过 D 最大化 V(D,G) 再通过 G 最小化 V(D, G)。
<a name="wQg0a"></a>
## 10、训练 GAN
在训练中，当人脸来自生成器，通过最小化误差函数，辨别器输出概率值接近 0。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949677581-26ad995f-71ef-4495-a307-0a65950e8703.gif#clientId=u22ec4da5-d95b-4&from=paste&id=u58937130&originHeight=545&originWidth=1078&originalType=url&ratio=1&status=done&style=none&taskId=uf383d724-aa70-416a-8694-f15c0daab21)<br />当人脸来自真实图片，通过最小化误差函数，辨别器输出概率值接近 1。<br />![](https://cdn.nlark.com/yuque/0/2021/gif/396745/1636949678099-2a0c5480-1f83-4366-b8ec-bd0196fa0175.gif#clientId=u22ec4da5-d95b-4&from=paste&id=u902236d5&originHeight=593&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ua3c6bfc7-a3e3-480a-9d44-d0047943680)

---

当然所有神经网络的训练算法都是梯度下降了。
<a name="JjqJM"></a>
## 11、**数学推导**
**辨别器：**从像素矩阵到概率<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678036-06f35888-70af-40e7-9b66-1470736fcf32.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u3f64373c&originHeight=608&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u8d67df9b-ccfe-498a-9edc-84579b39df1)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678314-c187b6b9-0c93-465b-958e-1fa68efa25d8.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ue30b895d&originHeight=427&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u69569f53-0a0c-447a-a1a6-6ace4640551)<br />**生成器：**从随机数 z 到像素矩阵<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678285-259366d6-96ec-4bd3-8325-a8729f8fe1a5.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u46da9a9c&originHeight=608&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=ub5a7a95a-f1a6-4082-b9fe-8201b3c1fdb)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678306-05e67a88-3f35-46ba-83bf-8f52f87dbdd2.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uba8015ae&originHeight=522&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=uce893f6f-cd08-49d5-837e-13039c0d26c)<br />得到误差函数相对于生成器和辨别器中的权重和偏置的各种偏导数后，就可以写代码实现了。
<a name="GFBjB"></a>
## 12、**Python 实现 - 准备工作**
引入 numpy 和 matplotlib。
```python
import numpy as np
from numpy import random
from matplotlib import pyplot as plt
%matplotlib inline
```
编写绘画人脸像素的函数。
```python
def view_samples(samples, m, n):
    fig, axes = plt.subplots(figsize=(10, 10), 
                             nrows=m, ncols=n, 
                             sharey=True, sharex=True)
    for ax, img in zip(axes.flatten(), samples):
        ax.xaxis.set_visible(False)
        ax.yaxis.set_visible(False)
        im = ax.imshow(1-img.reshape((2,2)), cmap='Greys_r')  
    return fig, axes
```
画出四张人脸，注意其像素矩阵中对角线上的数值大，非对角线上的数值小。
```python
faces = [np.array([1,0,0,1]),
         np.array([0.9,0.1,0.2,0.8]),
         np.array([0.9,0.2,0.1,0.8]),
         np.array([0.8,0.1,0.2,0.9]),
         np.array([0.8,0.2,0.1,0.9])]
    
_ = view_samples(faces, 1, 4)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678629-4cd368eb-68e0-4f3a-a4ec-3e1e7322869e.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ufe1b71b5&originHeight=139&originWidth=575&originalType=url&ratio=1&status=done&style=none&taskId=ud88f4dcb-5168-47f1-ab3a-40374fd8c06)<br />画出二十张非人脸，注意其像素矩阵中的数都是随机的。
```python
noise = [np.random.randn(2,2) for i in range(20)]
def generate_random_image():
    return [np.random.random(), np.random.random(), np.random.random(), np.random.random()]

_ = view_samples(noise, 4,5)
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678642-cc12402c-6c5b-4b28-966b-66d43e702ed7.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uf952822f&originHeight=539&originWidth=575&originalType=url&ratio=1&status=done&style=none&taskId=ua9db56b5-81f6-488a-aaf7-f2d277ee852)
<a name="YrPp4"></a>
## 13、**Python 实现 - 构建辨别器**
首先实现 sigmoid 函数。
```python
def sigmoid(x):
    return np.exp(x)/(1.0+np.exp(x))
```
用面向对象编程（OOP）来编写**辨别器**，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678856-99078fe7-c86e-4d19-bc81-25261c7b82cc.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u283e939f&originHeight=721&originWidth=600&originalType=url&ratio=1&status=done&style=none&taskId=u66aeee8c-2727-46aa-910f-6d7441f9485)<br />其中

- __init__() 是构建函数
- forward() 函数将像素矩阵打平成向量 x，乘上权重 w 加上偏置 b 得到得分，再通过 sigmoid() 函数转成概率
- error_form_image() 计算当接收**真实图片**为输入的误差函数
- error_form_noise() 计算当接收**生成器**为输入的误差函数
- derivatives_form_image() 计算当接收**真实图片**为输入误差函数对权重 w 和偏置 b 的偏导数 
- derivatives_form_noise() 计算当接收**生成器**为输入误差函数对权重 w 和偏置 b 的偏导数 
- update_form_image() 计算当接收**真实图片**为输入时的梯度下降法
- update_form_noise() 计算当接收**生成器**为输入时的梯度下降法
<a name="HWN3j"></a>
## 14、**Python 实现 - 构建生成器**
用面向对象编程（OOP）来编写**生成器**，代码如下：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678703-99712f28-dad2-477d-8eae-257a91efc32c.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u9f534c22&originHeight=531&originWidth=592&originalType=url&ratio=1&status=done&style=none&taskId=u73fbe233-5cc2-44a3-a88e-8541d568733)<br />其中

- __init__() 是构建函数
- forward() 函数将随机数 z 乘上权重 w 加上偏置 b 得到得分，再通过 sigmoid() 函数转成像素
- error() 计算当固定**辨别器**为输入的误差函数，分两步：
   - **生成器**的 forward() 函数得到像素
   - **辨别器**的 forward() 函数得到概率
- derivatives() 计算当固定**辨别器**为输入误差函数对权重 w 和偏置 b 的偏导数，对着上一节数学公式看代码 
- update() 计算当固定**辨别器**为输入时的梯度下降法
<a name="rAb8z"></a>
## 15、**Python 实现 - 训练 GAN**
设定 1000 期（epoch），即将数据遍历 1000 遍开始训练，记录每期**生成器**和**辨别器**的误差。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678988-dfd4a9dd-24b0-4cdd-a41a-4947a351bc87.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u1b5bc1d8&originHeight=665&originWidth=700&originalType=url&ratio=1&status=done&style=none&taskId=u4d64dc32-4e72-4d8f-a6f8-2e575d7eed2)<br />画出**生成器**和**辨别器**的误差函数图，发现**生成器**逐步趋于稳定。
```python
plt.plot(errors_generator)
plt.title("Generator error function")
plt.legend("gen")
plt.show()
plt.plot(errors_discriminator)
plt.legend('disc')
plt.title("Discriminator error function")
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949678974-a850e903-f615-4c7d-8628-9f50a57fdc58.webp#clientId=u22ec4da5-d95b-4&from=paste&id=uf37ead3a&originHeight=264&originWidth=372&originalType=url&ratio=1&status=done&style=none&taskId=u580556e7-ae01-40af-b43d-21047e7b1c3)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949679014-3f4dc26e-dcc2-4639-8e74-d7e43e18fdaf.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u65d010c4&originHeight=264&originWidth=372&originalType=url&ratio=1&status=done&style=none&taskId=u01561027-b9f8-46ff-804a-51fb3fa0924)
<a name="qmU3q"></a>
## 16、**Python 实现 - 结果展示**
生成图片。
```python
generated_images = []
for i in range(4):
    z = random.random()
    generated_image = G.forward(z)
    generated_images.append(generated_image)
_ = view_samples(generated_images, 1, 4)
for i in generated_images:
    print(i)
```
```python
[0.94688171 0.03401213 0.04080795 0.96308679]
[0.95653992 0.03437852 0.03579494 0.97063836]
[0.95056667 0.03414339 0.03893305 0.96599501]
[0.94228203 0.03386046 0.04309146 0.95941292]
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949679197-aef262dc-18f7-4025-8d31-69c2f66e4e60.webp#clientId=u22ec4da5-d95b-4&from=paste&id=u5f05df7d&originHeight=139&originWidth=575&originalType=url&ratio=1&status=done&style=none&taskId=u823800dd-d5bb-4e18-9443-44762959ec1)<br />打印出最终 GAN 的参数，即**生成器**和**辨别器**的权重和偏置。
```python
print("Generator weights", G.weights)
print("Generator biases", G.biases)
print("Discriminator weights", D.weights)
print("Discriminator bias", D.bias)
```
```python
Generator weights [ 0.70702123 0.03720449 -0.45703394 0.79375751]
Generator biases [ 2.48490157 -3.36725912 -2.90139211 2.8172726 ]
Discriminator weights [ 0.60175083 -0.29127513 -0.40093314 0.37759987]
Discriminator bias -0.8955103005797729
```
带有权重和偏置的 GAN 如下所示。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1636949679487-272e3489-314d-49fd-85c7-653089b8a548.webp#clientId=u22ec4da5-d95b-4&from=paste&id=ubad0a135&originHeight=583&originWidth=1080&originalType=url&ratio=1&status=done&style=none&taskId=u9681f850-859d-406d-8a51-8a201fbfa9f)<br />图中粗线对应大权重，细线对应小或者负权重。对照前面**生成器**要生成逼真人脸的目标来看（即 2*2 矩阵的对角线上的值大），是不是这个权重很合理。
