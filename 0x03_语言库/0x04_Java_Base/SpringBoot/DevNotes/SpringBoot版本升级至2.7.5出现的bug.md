<a name="jDS4m"></a>
## 前言
最近项目组升级了SpringBoot版本，由之前的2.0.4升级到最新版本2.7.5，却引出了一个大Bug。<br />到底是怎么回事呢？
<a name="soIyN"></a>
## 1、案发现场
有一天，项目组的同事反馈说，之前有个接口在新的测试环境报错了，具体异常是：Missing argment level for method parameter of type Integer。<br />当时的第一反应有点懵，心想这个接口是一个老接口，有一年多的时间都没改过了，怎么会出问题呢？<br />他说近期另外一个同事为了部署阿里云服务器，把新测试环境SpringBoot的版本升级到了最新版。<br />之后，在测试的过程中，发现有个Get请求接口报异常了。<br />该接口代码类似于这样：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592472-3a7e1812-58db-45ac-bd2d-8dea2482ce0c.png#averageHue=%232d2c2b&clientId=u7bbdc397-407d-4&from=paste&id=ue1288965&originHeight=408&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u46a3feaa-ada1-4fbf-a782-a3542ebfaa7&title=)在getCategory接口中，有两个参数：

1. type表示大类，是必传的。
2. level表示要返回几级分类，比如：4级分类，就传4，是非必传的，默认就是查4级分类。

就是这样一个接口的level参数，前端没有传参，例如：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592497-4d0ecf72-f6c2-4fef-af8d-0117c2562800.png#averageHue=%23eff2f3&clientId=u7bbdc397-407d-4&from=paste&id=u627e6687&originHeight=78&originWidth=784&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u89fe5255-2190-4286-84d8-b6c5f2c7f25&title=)结果被Spring MVC拦截直接报错了。
<a name="TuTUc"></a>
## 2、报错的原因
从打印的异常信息看，现在level参数必须要传值了，之前是可传，可不传的。<br />后来本打算自定义Spring的转换器，修改一下校验规则，跟老版本保持一致。<br />这样那些基本接口就不用改了。<br />但后来发现，被spring-web-5.3.23的源码无情的打脸了。<br />在org.springframework.web.method.annotation包下的`AbstractNamedValueMethodArgumentResolver`类的`resolveArgument`方法中：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592476-3ec06404-148d-4780-b72b-c6c4df64d466.png#averageHue=%23302c2c&clientId=u7bbdc397-407d-4&from=paste&id=u4e490663&originHeight=629&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u68bd1aac-c3de-462d-b044-658cff5296f&title=)多了这样的校验。如果该参数为空，没有设置默认值，`required`属性为`true`，并且不是`Optional`类型，则执行`handleMissingValueAfterConversion`方法。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592537-cb0eefe0-138e-4018-a393-661c17468020.png#averageHue=%232f2e2c&clientId=u7bbdc397-407d-4&from=paste&id=u4121becc&originHeight=132&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u2e916729-ce99-4264-ba5e-1f4c623d773&title=)该方法会调用`handleMissingValue`方法，具体代码如图中所示：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592397-192f0109-165e-4eb8-a6b7-a361b21bb941.png#averageHue=%23302f2d&clientId=u7bbdc397-407d-4&from=paste&id=u1931f5e5&originHeight=127&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue3779851-b27e-4b7c-bc0d-3210c2e5603&title=)最后会抛出之前看到的那个异常。<br />原因最新版本的Spring中不允许Get接口的请求参数，在不使用`@RequestParam`注解时，值为空的情况出现了。
<a name="UT9qi"></a>
## 3、如何解决问题？
想要解决上面的报错问题，其实很简单，只需在level参数前加`@RequestParam`注解，并且设置`required`属性为`false`。<br />例如：![](https://cdn.nlark.com/yuque/0/2022/png/396745/1668324592781-f66ce93c-9e34-42f4-a0ea-526fe365b4ea.png#averageHue=%232d2c2b&clientId=u7bbdc397-407d-4&from=paste&id=u0e3d131d&originHeight=377&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u93bcea00-7df2-41d1-833b-361379ff437&title=)但是后面发现，项目中不只这一个接口要调整，其他好多同事的接口，也有类似的问题，需要修改的接口很多。<br />这个改动的工作量不小。<br />哭晕在测试。。。
<a name="I8Mjg"></a>
## 后话
这个问题有很多人中招，所以非常有必要把这个问题分享给大家，防微杜渐。<br />之前level参数不加`@RequestParam`注解，也没设置`required`属性，当时持有的心态是Spring有默认值，有些注解不加，程序也能正常运行，既然这样就可以少写点代码，并且在当时的版本测试过，没有出现过什么问题。<br />这种情况其实是Spring框架的一个bug，已经在最新版本中被修复了。。。<br />赶紧review一下代码，看看有没有类似的用法，不然迟早有一天也会中招。
