<a name="SARKc"></a>
# 一、思维导图
![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1647419580877-3420c016-d5c2-4047-8778-31e74530dbad.webp#clientId=u67045f4b-839f-4&from=paste&id=u9f600998&originHeight=1707&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uc9206b7d-c10f-4368-8584-422a64d947b&title=)<br />代码审计

- **教学计划：**审计项目漏洞Demo->审计思路->完整源码框架->验证并利用漏洞
- **教学内容：**PHP，JAVA网站应用，引入框架类开发源码，相关审计工具及插件使用
- **必备知识点：**环境安装搭建使用，相关工具插件安装使用，掌握前期各种漏洞原理及利用
- **开始前准备：**审计目标的程序名，版本，当前环境(系统,中间件,脚本语言等信息),各种插件等
- **挖掘漏洞根本：**可控变量及特定函数，不存在过滤或过滤不严谨存在绕过导致的安全漏洞

![](https://cdn.nlark.com/yuque/0/2022/webp/2476579/1647594312343-fb1cd260-1d5b-4a9e-8b78-ff433d81a6fa.webp#clientId=u6e629701-0a44-4&from=paste&id=u3c043e08&originHeight=1105&originWidth=942&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue9016a58-4c5c-467a-bbf6-44578e10ea5&title=)
<a name="uCdeh"></a>
# 二、相关资源
涉及资源：

动态调试|Maccms SQL 注入分析(附注入盲注脚本)：[https://www.cnblogs.com/ichunqiu/p/9548754.html](https://www.cnblogs.com/ichunqiu/p/9548754.html)

Fortify：[https://pan.baidu.com/s/1Jw7idUwlfMZwX4eTb-9luw](https://pan.baidu.com/s/1Jw7idUwlfMZwX4eTb-9luw) 提取：w6dn 解压密码：shungg.cn

Seay源代码审计系统：[https://github.com/f1tz/cnseay](https://github.com/f1tz/cnseay)

MySQL-Monitor：[https://github.com/cw1997/MySQL-Monitor](https://github.com/cw1997/MySQL-Monitor)

oracleMonitor：[https://github.com/denglt/oracleMonitor](https://github.com/denglt/oracleMonitor) 作者：shtome 

<a name="ybwtc"></a>
# 三、代码审计
1.定点挖掘关键字：

- 可控变量-变量接受get post，接受关键字$_GET
- 特定函数-输出print、数据库操作等
- 特定关键字
   - sql注入漏洞-->搜索select\insert\update\sql执行语句
   - xss漏洞-->搜索echo\print等
   - 安全漏洞-->搜索$_GET$_POST等

2.定点挖掘功能点：

- 如挖掘文件上传，会员中心存在文件上传的操作地址，抓包分析找到源码中的特定文件代码段，进行文件上传代码分析挖掘。

3.拓展：视漏洞而定，举例

- sqlql注入 数据库监控-监控到当前页面和数据库的交互过程（SQL执行语句）
- 断点调试：访问页面对应代码进行断点调试（执行过程前后顺序，调用文件列表）
- 可使用Seay源代码审计系统进行数据库监控、全局搜索关键字等（下载：https://github.com/f1tz/cnseay）
- 动态调试|Maccms SQL 注入分析(附注入盲注脚本)：https://www.cnblogs.com/ichunqiu/p/9548754.html
- 双重编码绕过：源代码将%5c自动转为\，%25%35%63 转为%5c get出url编码

![](https://cdn.nlark.com/yuque/0/2022/jpeg/2476579/1647758673058-581e4382-850c-475c-9d61-c5572b5425ac.jpeg#clientId=u20ff8963-05a0-4&from=paste&id=u3a684d6a&originHeight=386&originWidth=776&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u9e52039c-8ac8-4166-aafc-d94eb37366d&title=)








[https://www.bilibili.com/read/cv13665913?spm_id_from=333.999.0.0](https://www.bilibili.com/read/cv13665913?spm_id_from=333.999.0.0) 出处：bilibili

