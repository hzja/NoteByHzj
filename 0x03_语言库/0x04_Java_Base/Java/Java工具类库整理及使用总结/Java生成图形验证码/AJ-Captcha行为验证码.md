现在登录什么账号，基本都用到验证码，多数是短信验证码，不知道大家是什么感受，每次一登录一个账号就要去手机清理一下短信，不然小红点看着难受。不过近两年行为验证码异军突起，大多数网页都用到这类验证码，虽然有的文字验证码有点费眼睛，但能减少收短信/按数字键的频率，那也是很有效率的一种方式。<br />看看这个行为验证码的开源项目-----AJ-Captcha。
<a name="CIqDF"></a>
### AJ-Captcha介绍
AJ-Captcha行为验证码，包含滑动拼图、文字点选两种方式，UI支持弹出和嵌入两种方式。后端基于Java实现，提供纯Java.jar和SpringBoot Starter。前端提供了Android、iOS、Futter、Uni-App、ReactNative、Vue、Angular、Html、Php等多端示例。行为验证码采用嵌入式集成方式，接入方便，安全，高效。抛弃了传统字符型验证码展示-填写字符-比对答案的流程，采用验证码展示-采集用户行为-分析用户行为流程，用户只需要产生指定的行为轨迹，不需要键盘手动输入，极大优化了传统验证码用户体验不佳的问题；同时，快速、准确地返回人机判定结果。目前对外提供两种类型的验证码，其中包含滑动拼图、文字点选。如图1-1、1-2所示。若希望不影响原UI布局，可采用弹出式交互。<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1691918371394-506a582d-3bd7-4b19-879b-8bb4a3077dd9.gif#averageHue=%23c6cfe8&clientId=u82c95884-9287-4&from=paste&id=u0a1323e8&originHeight=326&originWidth=422&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=udc33508e-2c7e-41ee-8204-c7a5cbcb96a&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/gif/396745/1691918371442-602c6137-41ea-4b3f-9e7e-365f82b5a226.gif#averageHue=%2328536e&clientId=u82c95884-9287-4&from=paste&id=u7cf99974&originHeight=324&originWidth=414&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc1a8e714-e678-4053-9e7b-5f727810b7a&title=)
<a name="bmxsE"></a>
### 交互流程
① 用户访问应用页面，请求显示行为验证码<br />② 用户按照提示要求完成验证码拼图/点击<br />③ 用户提交表单，前端将第二步的输出一同提交到后台<br />④ 验证数据随表单提交到后台后，后台需要调用`captchaService.verification`做二次校验。<br />⑤ 第4步返回校验通过/失败到产品应用后端，再返回到前端。如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1691918371426-0578e452-9016-46a8-8fe9-488f6f393a0d.png#averageHue=%23f6f6f6&clientId=u82c95884-9287-4&from=paste&id=u9915fd9f&originHeight=537&originWidth=1020&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufb036f15-e8a3-463c-a4df-b3a42383ca2&title=)
<a name="KZLGz"></a>
### 安装
项目地址：[https://gitee.com/anji-plus/captcha](https://gitee.com/anji-plus/captcha)<br />先启动后端，导入Eclipse或者Intellij，启动service/SpringBoot的StartApplication。<br />再启动前端，使用visual code打开文件夹view/vue，启动。
```bash
npm install
npm run dev
```
<a name="mZohS"></a>
### 最后
AJ-Captcha行为验证码不仅脱离传统的数字等类型的验证形式，而且能解脱键盘，只需要拖拽图片到合适位置/点击文字即可完成验证。
