Java<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113952843-3fccb80b-d800-4e79-9b10-69d68dddb27c.png#clientId=u28f7d997-d13e-4&from=paste&id=ufb19d5fa&originHeight=385&originWidth=812&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u41baa77a-8864-47ed-8857-57dccbc3147&title=)
<a name="Lelqu"></a>
## 前言
在移动互联网，分布式，微服务盛行的今天，现在项目绝大部分都采用的微服务框架，前分离分离方式，（题外话：前重新的工作分配越来越明确，现在的前端都称为大前端，技术栈以及生态圈都已经非常成熟；以前官员人员瞧不起前端人员，那现在高层人员要重新认识一下前端，前端已经很成体系了）。<br />一般系统的大致整体架构图如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113952883-ffb6ff12-750e-468b-bf08-c8431b0d4c38.png#clientId=u28f7d997-d13e-4&from=paste&id=u0c5e0007&originHeight=874&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u75496eec-8dd4-49c0-a1d2-1fe96a9f8b3&title=)
<a name="YfQte"></a>
## 接口相互作用
前端和前端进行交互，前端按约定的请求URL路径，并合并相关参数，进入服务器接收请求，进行业务处理，返回数据给前端。<br />针对URL路径的restful风格，以及引用参数的公共请求头的要求（如：app_version，api_version，device等）。<br />前端服务器如何实现把数据返回给前端？
<a name="wFCKr"></a>
## 返回格式
最初返回给前端一般用JSON体方式，定义如下：
```json
{
	#返回状态码
	code:integer,
	#返回信息描述
	message:string,
	#返回值
	data:object
}
```
<a name="IIzFD"></a>
### CODE状态码
code返回状态码，一般是在开发的时候需要什么，就添加什么。<br />如接口要返回用户权限异常，加一个状态码为101，下一次又要加一个数据参数异常，就加一个102的状态码。这样虽然能够照常满足业务，但状态码太凌乱了。可以参考HTTP请求返回的状态码，下面是常见的HTTP状态码：<br />200 - 请求成功<br />301 - 资源（网页等）被永久转移到其它URL<br />404 - 请求的资源（网页等）不存在<br />500 - 内部服务器错误<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113952864-9d41f070-6677-499e-9176-d1b0c0b3b30e.png#clientId=u28f7d997-d13e-4&from=paste&id=ub76bfe81&originHeight=374&originWidth=1050&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf4644168-2aee-4268-99ac-cb60becb391&title=)<br />可以参考这样的设计，这样的好处就把错误类型归类到某个区间内，如果区间不够，可以设计成4个数字。<br />#1000～1999 区间表示参数错误<br />#2000～2999 区间表示用户错误<br />#3000～3999 区间表示接口异常<br />这样前端开发人员在得到返回值后，根据状态码就可以知道，大概什么错误，再根据消息相关的信息描述，可以快速定位。
<a name="ZtLAz"></a>
### 资讯
这个相对相对理解比较简单，就是发生错误时，如何友好的进行提示。一般的设计是和code状态码一起设计，如<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113952860-d5a79a19-11ec-40f6-96d3-4034695e056d.png#clientId=u28f7d997-d13e-4&from=paste&id=ue2cfa3d3&originHeight=460&originWidth=1076&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u61af9df9-3faf-4c64-bd7b-1d4b4c59c23&title=)<br />再在枚举中定义，状态码<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113952883-e7bfa2b1-6b9a-4193-bcd8-72c25131cf2b.png#clientId=u28f7d997-d13e-4&from=paste&id=u8e51126d&originHeight=1004&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u05f31cf2-952c-444a-aa48-6c252f1de2c&title=)<br />状态码和信息就会一一对应，比较好维护。
<a name="Gcamg"></a>
### 数据
返回数据体，JSON格式，根据不同的业务又不同的JSON体。要设计一个返回体类结果<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953190-f5b35a2c-2923-46a4-a1c9-92e9f3e805af.png#clientId=u28f7d997-d13e-4&from=paste&id=u42292e19&originHeight=587&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud5aadd09-3639-4cc0-89d5-0c689c0a050&title=)
<a name="rGc4k"></a>
## 控制层控制器
会在控制器层处理业务请求，并返回给前端，以orderorder为例<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953272-dbadd185-6235-4239-ab64-d3c3797ba9a6.png#clientId=u28f7d997-d13e-4&from=paste&id=uf9834f2d&originHeight=547&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue0d3bdc5-8ad9-47d9-959a-ca72463c42b&title=)<br />可以看到在获得订单对象之后，用的结果构造方法进行包装赋值，然后进行返回。有没有发现构造方法这样的包装是不是很麻烦，可以优化一下。
<a name="Ej9t6"></a>
## 美观美化
可以在结果类中，加入静态方法，一看就懂<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953319-e45bfe98-db39-4e1c-8169-ed2356072f1b.png#clientId=u28f7d997-d13e-4&from=paste&id=u0e1f0834&originHeight=944&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3c38a4d1-0b6f-4c30-aff7-77ebf18857a&title=)<br />来改造一下Controller<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953316-a2c39387-f19a-401f-8d3d-f804bedb289d.png#clientId=u28f7d997-d13e-4&from=paste&id=u6781fd33&originHeight=547&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u7006238b-9d92-46e9-a467-0f3379e9a90&title=)<br />代码是不是比较简洁了，也美观了。
<a name="TWVYF"></a>
## 优雅优化
上面看到在结果类中增加了静态方法，正确的业务处理代码简洁了。有没有发现这样有几个问题：

1. 每个方法的返回都是Result封装对象，没有业务含义
2. 在业务代码中，成功的时候调用`Result.success`，异常错误调用`Result.failure`。是不是很多余
3. 上面的代码，判断id是否为null，实际上可以使用hibernate validate做校验，没有必要在方法体中做判断。

最好的方式直接返回真实业务对象，最好不要改变之前的业务方式，如下图<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953599-98931d82-fac6-4789-9688-56a6286b7f7b.png#clientId=u28f7d997-d13e-4&from=paste&id=u627b6232&originHeight=488&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u38a21f34-1b9b-42c7-9828-997c5dda1e2&title=)<br />这个和平时的代码是一样的，非常直观，直接返回order对象，这样是不是很完美。那实现方案是什么呢？
<a name="VBekX"></a>
## 实现方案
在这个过程中，需要做几个事情

1. 定义一个注解`@ResponseResult`，表示此接口返回的值需要包装一下
2. 拦截请求，判断此请求是否需要被`@ResponseResult`注解
3. 核心步骤就是实现接口`ResponseBodyAdvice`和`@ControllerAdvice`，判断是否需要包装返回值，如果需要，就把控制器接口的返回值进行转换。
<a name="v4MxR"></a>
### 注解类
采用标记方法的返回值，是否需要包装<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953573-d6919030-bc9f-4a67-bdb3-8b555f57b04a.png#clientId=u28f7d997-d13e-4&from=paste&id=u6ca07100&originHeight=302&originWidth=836&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ud2bdb310-6006-4df5-ad21-1052137a0e2&title=)
<a name="wc9KV"></a>
### 拦截器
拦截请求，是否此请求返回的值需要包装，实际上就是运行的时候，解析`@ResponseResult`注解<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953765-519998a7-6990-4db6-8284-058f545c9550.png#clientId=u28f7d997-d13e-4&from=paste&id=u6e134f41&originHeight=573&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uca576861-99b9-4878-ab24-13abdcc8572&title=)<br />此代码核心思想，就是获取此请求，是否需要返回值包装，设置一个属性标记。
<a name="N0Xvq"></a>
### 重建返回体
![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953866-7606706b-312f-40b6-bef4-e512e1f393ee.png#clientId=u28f7d997-d13e-4&from=paste&id=u20a62d6c&originHeight=573&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4d0ce2a6-b5f5-4a1d-bcb5-35788fb28ad&title=)<br />上面的代码就是肯定肯定判断是否需要返回值包装，如果需要就直接包装。这里只处理了正常成功的包装，如果方法体报异常怎么办？处理异常也比较简单，只要判断body是否为异常类。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1650113953877-7a5f37cb-de8b-45e6-b580-9fc5f369e4d7.png#clientId=u28f7d997-d13e-4&from=paste&id=u3e917095&originHeight=492&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u59724cb3-14f9-4185-8615-c724dec4cc1&title=)
<a name="Bvyhz"></a>
### 改造控制器
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650113954098-e2a68cf0-8f56-40f5-b492-c7b90bb7ff2d.jpeg#clientId=u28f7d997-d13e-4&from=paste&id=u39225c8a&originHeight=504&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u028db442-991c-419e-a2e3-b5752d4bec6&title=)<br />在控制器类上或者方法体上加上`@ResponseResult`注解，这样就ok了，简单吧。到此返回的设计思路完成，是不是又简洁，又优雅。
