Java SpringBoot
<a name="yrttL"></a>
## 1、简介
magic-api 是一个基于Java的接口快速开发框架，编写接口将通过magic-api提供的UI界面完成，自动映射为HTTP接口，无需定义Controller、Service、Dao、Mapper、XML、VO等Java对象即可完成常见的HTTP API接口开发<br />【已有上千家中小型公司使用，上万名开发者用于接口配置开发。上百名开发者参与提交了功能建议，接近20多名贡献者参与。已被gitee长期推荐。从首个版本开始不断优化升级，目前版本稳定，开发者交流群活跃。】
<a name="cnR15"></a>
## 2、特性

- 支持MySQL、MariaDB、Oracle、DB2、PostgreSQL、SQLServer 等支持jdbc规范的数据库
- 支持非关系型数据库Redis、Mongodb
- 支持集群部署、接口自动同步。
- 支持分页查询以及自定义分页查询
- 支持多数据源配置，支持在线配置数据源
- 支持SQL缓存，以及自定义SQL缓存
- 支持自定义JSON结果、自定义分页结果
- 支持对接口权限配置、拦截器等功能
- 支持运行时动态修改数据源
- 支持Swagger接口文档生成
- 基于magic-script脚本引擎，动态编译，无需重启，实时发布
- 支持Linq式查询，关联、转换更简单
- 支持数据库事务、SQL支持拼接，占位符，判断等语法
- 支持文件上传、下载、输出图片
- 支持脚本历史版本对比与恢复
- 支持脚本代码自动提示、参数提示、悬浮提示、错误提示
- 支持导入Spring中的Bean、Java中的类
- 支持在线调试
- 支持自定义工具类、自定义模块包、自定义类型扩展、自定义方言、自定义列名转换等自定义操作
<a name="fb90V"></a>
## 3、快速开始
<a name="sm0kD"></a>
### maven引入
```xml
<!-- 以spring-boot-starter的方式引用 -->
<dependency>
  <groupId>org.ssssssss</groupId>
  <artifactId>magic-api-spring-boot-starter</artifactId>
  <version>1.4.1</version>
</dependency>
```
<a name="G4QwW"></a>
### 修改application.properties
```
server.port=9999
#配置web页面入口
magic-api.web=/magic/web
#配置文件存储位置。当以classpath开头时，为只读模式
magic-api.resource.location=/data/magic-api
```
<a name="HBTB4"></a>
### 在线编辑
访问http://localhost:9999/magic/web进行操作
<a name="uk0Q8"></a>
## 4、文档/演示

- 文档地址：[https://ssssssss.org](https://ssssssss.org)
- 在线演示：[https://magic-api.ssssssss.org](https://magic-api.ssssssss.org)
<a name="TTLVa"></a>
## 5、示例项目
[https://gitee.com/ssssssss-team/magic-api-example](https://gitee.com/ssssssss-team/magic-api-example)
<a name="bG8OC"></a>
## 6、项目截图
![2021-09-03-19-26-54-807055.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630668473496-078d5ef3-48c7-4c7a-bbe9-7879bc5138bb.png#clientId=u0995b231-9667-4&from=ui&id=u2cb8ae7d&originHeight=601&originWidth=1080&originalType=binary&ratio=1&size=115563&status=done&style=none&taskId=u81651b15-fbd8-4628-8673-da959c50ef4)![2021-09-03-19-26-55-092055.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1630668473813-6e8ebdc3-772c-43a4-9b28-ec9ffcf21f25.gif#clientId=u0995b231-9667-4&from=ui&id=ueda116b2&originHeight=556&originWidth=1079&originalType=binary&ratio=1&size=610258&status=done&style=none&taskId=u3853bb25-3c06-4d76-b570-28ceb99eb2e)![2021-09-03-19-26-55-402054.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1630668473780-bb26c9dd-37ec-4812-a797-101d5e75ab69.gif#clientId=u0995b231-9667-4&from=ui&id=u9f8496ab&originHeight=556&originWidth=1079&originalType=binary&ratio=1&size=493436&status=done&style=none&taskId=u2fcd3456-a60b-462f-bcd9-e725c9c91b2)![2021-09-03-19-26-55-510059.gif](https://cdn.nlark.com/yuque/0/2021/gif/396745/1630668473473-515309cc-a2e9-417b-bde5-505ad5cedb46.gif#clientId=u0995b231-9667-4&from=ui&id=u39492c7d&originHeight=252&originWidth=633&originalType=binary&ratio=1&size=21878&status=done&style=none&taskId=u2d06b71f-e41b-46b3-ae7d-df98eee9bcb)![2021-09-03-19-26-55-941058.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630668473616-8c0b99de-07b9-435e-b030-f1b070e9ffd1.png#clientId=u0995b231-9667-4&from=ui&id=u1194a05c&originHeight=599&originWidth=1080&originalType=binary&ratio=1&size=185035&status=done&style=none&taskId=u687954ab-b687-4df9-b967-9e8c79dcf79)![2021-09-03-19-26-56-053056.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630668474132-cef67140-6e9e-4ab0-84ac-8c0a503b3f40.png#clientId=u0995b231-9667-4&from=ui&id=u61679e52&originHeight=756&originWidth=1080&originalType=binary&ratio=1&size=166406&status=done&style=none&taskId=u20b60a28-674b-43ed-842c-2546deeb1d8)![2021-09-03-19-26-56-135057.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630668474581-3ff2ace4-fea1-4cc5-9e58-cf35e82c95ba.png#clientId=u0995b231-9667-4&from=ui&id=u0837adb1&originHeight=642&originWidth=1080&originalType=binary&ratio=1&size=92534&status=done&style=none&taskId=ua894331a-00f0-48db-95f1-bf5b257b983)![2021-09-03-19-26-56-271109.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630668474955-486a2747-da3a-4b2c-99c8-f7b4976efe61.png#clientId=u0995b231-9667-4&from=ui&id=ueafea3a8&originHeight=642&originWidth=1080&originalType=binary&ratio=1&size=250556&status=done&style=none&taskId=u2009481d-ab75-4b63-9a72-07d74c9a950)
