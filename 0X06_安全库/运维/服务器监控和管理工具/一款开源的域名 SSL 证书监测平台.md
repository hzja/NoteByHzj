因为这个开源项目是一个域名 SSL 证书监测平台。<br />都知道基于安全的要求，网站和小程序需要 SSL 证书，如果不使用 SSL 加密的话，被认为通信不安全。<br />尤其是现在小程序开发，几乎每开发一个小程序，可能就需要一个 SSL 证书，而 SSL 申请了是有时效性的，过期的时候，可能就导致程序无法访问了，需要及时更换。<br />那么如果有一个域名 SSL 证书监测平台就很有必要了。<br />这个开源的域名 SSL 证书监测平台是：**domain-admin**，是基于 Python + Vue3.js 技术栈实现的域名和 SSL 证书监测平台<br />核心功能：到期自动邮件提醒<br />用于解决，不同业务域名 SSL 证书，申请自不同的平台，到期后不能及时收到通知，导致线上访问异常，被老板责骂的问题<br />支持平台：macOS、Linux、Windows<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1688607383438-577f6fbd-88e6-45bf-872b-153b58671cb0.png#averageHue=%23c5d0a6&clientId=u96cde831-d98c-4&from=paste&id=u3b9c1b40&originHeight=601&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u219727c0-26e3-4877-b6ef-3634ff691a7&title=)<br />预览地址：[https://mouday.github.io/domain-admin-web/](https://mouday.github.io/domain-admin-web/)<br />账号密码随意，预览模式仅提供模拟数据，无法操作修改<br />功能：

- 权限
   - 用户登录
   - 用户退出
   - 修改密码
- 域名管理
   - 域名添加
   - 域名删除
   - 域名搜索
   - 域名导入、导出功能
   - 域名信息
- 证书监控
   - 定时监控
   - 到期邮件提醒
   - 微信提醒
   - 手动 / 自动更新证书信息
- 用户管理
   - 添加用户
   - 删除用户
   - 禁用 / 启用用户
- 监控日志
- 管理界面
   - api 接口（用于二次开发）
   - web 浏览器
   - 桌面

开源项目地址：[**https://github.com/mouday/domain-admin**](https://github.com/mouday/domain-admin)<br />开源项目作者：**mouday**
