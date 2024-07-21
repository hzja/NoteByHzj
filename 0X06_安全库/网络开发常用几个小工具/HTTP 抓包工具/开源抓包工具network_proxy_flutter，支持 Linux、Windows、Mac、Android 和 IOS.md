免费开源的 Http、Https 抓包工具，支持 Windows、Mac、Android、IOS，全平台系统，使用 Flutter 框架开发。<br />支持手机扫码连接，不用手动配置 Wifi 代理，包括配置同步。所有终端都可以互相扫码连接转发流量。<br />默认 Https 不是开启的，需要点击加锁图标安装根证书后开启，因为不安装证书，https 会访问失败。

- Github 地址：[https://github.com/wanghongenpin/network_proxy_flutter](https://github.com/wanghongenpin/network_proxy_flutter)
- 国内下载地址：[https://gitee.com/wanghongenpin/network-proxy-flutter/releases](https://gitee.com/wanghongenpin/network-proxy-flutter/releases)

备注：Mac 首次打开会提示已损坏，需要到系统偏好设置 - 安全性与隐私 - 允许任何来源。

- iOS美版香港AppStore下载地址或直接搜ProxyPin：[https://apps.apple.com/app/proxypin/id6450932949](https://apps.apple.com/app/proxypin/id6450932949)
- iOS国内TF下载地址(有1万名额限制，满了会清理不使用的用户)：[https://testflight.apple.com/join/gURGH6B4](https://testflight.apple.com/join/gURGH6B4)
- TG：[https://t.me/proxypin_tg](https://t.me/proxypin_tg)

注：由于是 ios 个人开发者账号  没法上架 AppStore，后面可能会上架美版 AppStore。

- 接下来会持续完善功能和体验，模拟慢请求，UI 优化。
- 支持安卓微信小程序抓包，安卓分为系统证书和用户证书，下载的自签名根证书安装都是用户证书，微信不信任用户证书，不 Root 导致 Https 抓不了了，目前市场上所有抓包软件抓不了微信的包，后面单独做个运行空间插件，动态反编译修改配置，信任用户证书来解决。
- WebSocket 协议支持。

[![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695005159073-c92d33bc-f2c8-4e70-b21c-8466d168e2a4.png#averageHue=%23393838&clientId=u11f91d20-ce51-4&from=paste&id=ub4a550c2&originHeight=1358&originWidth=2558&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud4bd58c5-c6b1-48c1-a7a6-b6d73c1f992&title=)](https://user-images.githubusercontent.com/24794200/247049083-67a2feb1-f1c3-4c0c-8737-5abe62c34794.png) [![](https://cdn.nlark.com/yuque/0/2023/png/396745/1695005160520-e34f292e-4d5e-4d93-bfd3-b592bdfc72cc.png#averageHue=%23fcf9fb&clientId=u11f91d20-ce51-4&from=paste&height=750&id=u3605d765&originHeight=2532&originWidth=1170&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u280ec4f4-56f7-42ec-9ae9-40f2a4316d6&title=&width=346.4000244140625)](https://user-images.githubusercontent.com/24794200/251026263-1bb4b1ec-ec5c-44a7-add7-f0f94c8765b9.PNG)
