Java SpringBoot
<a name="zLRic"></a>
## 普通变量
首先来看看，如何给普通变量注入值？<br />例如，application-dev.yml 配置文件有如下配置：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068413951-b331e815-9180-4aad-b2ef-fc67d0e2bf8c.webp#clientId=u46e3924b-0ed6-4&from=paste&id=u893ad2cc&originHeight=213&originWidth=757&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua98aa180-797b-4c53-b9b2-9855726161d&title=)<br />给普通变量赋值时，直接在变量声明之上添加`@Value()`注解即可，如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068414055-4d6dc108-0fd6-423f-a390-90b71a1fbecf.webp#clientId=u46e3924b-0ed6-4&from=paste&id=u8a1bf0de&originHeight=159&originWidth=740&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u165f5314-0eb4-4ddb-a8f5-13fef3fbc78&title=)
<a name="SPwUL"></a>
## 静态变量
例如给项目中的文件上传封装一个工具类。阿里云oss的相关信息配置在配置文件中，需要将信息注入到工具类中的变量。为了方便直接使用变量，把变量定义为静态变量。当要给静态变量注入值的时候，若是在静态变量声明之上直接添加`@Value()`注解是无效的。如下图所示。<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068413982-80b4d9fe-681c-436b-aa3d-cc9925e11a5d.webp#clientId=u46e3924b-0ed6-4&from=paste&id=uf92612fe&originHeight=414&originWidth=881&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ue2f06466-6df0-4bb2-a778-11b1721102f&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068414136-cfb18c29-20f3-40ce-8296-72813851ad0d.webp#clientId=u46e3924b-0ed6-4&from=paste&id=udec2111a&originHeight=539&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u274085ab-39fa-491d-b01b-a075b7bf089&title=)
<a name="FvCJo"></a>
### 解决方案
<a name="D0egx"></a>
#### 方案一
若要给静态变量赋值，可以使用`set()`方法注入。

1. 类上加入`@Component`注解
2. `@Value`注解标记`set`方法
3. 方法名（例如setOssUrl）和参数名（例如ossUrl）可以任意命名如下所示：

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068413931-37607433-b1d1-4cd2-96df-62e1ece2d7fa.webp#clientId=u46e3924b-0ed6-4&from=paste&id=uf93ff8e4&originHeight=671&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u7d490113-5a31-4ffb-b7fc-6e8abfa0ecb&title=)
<a name="vdpNc"></a>
#### 方案二
如果觉得`@Value`注解麻烦，可以使用`@ConfigurationProperties`注解代替，这样比较简洁

1. 前缀要写合适
2. 方法名（例如setOssUrl）必须和属性保持一致，例如写为`setUrl()`会注入失败
3. 类上加入`@Component`注解

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068414642-dfcdd02d-70ef-40c5-ad71-d38f11417671.webp#clientId=u46e3924b-0ed6-4&from=paste&id=uca40e260&originHeight=689&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u11eebbaf-ce57-48e3-8701-42c218a10cf&title=)<br />![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068414515-1a2d6af8-02b2-40fb-a896-d38813f66975.webp#clientId=u46e3924b-0ed6-4&from=paste&id=u5175c5d4&originHeight=137&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ud9b855a2-7f02-4441-9fd5-d326236864f&title=)
<a name="vvH5D"></a>
#### 方案三

1. 类上加入`@Component`注解
2. `@PostConstruct`注解修饰的方法中进行赋值操作

![](https://cdn.nlark.com/yuque/0/2022/webp/396745/1644068414492-8bc75a00-aba2-4f77-aa65-84d458c9463b.webp#clientId=u46e3924b-0ed6-4&from=paste&id=ue24cb634&originHeight=640&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub54c6a06-406c-4d66-9c5f-48e74cce26f&title=)
<a name="MlXuN"></a>
### 使用场景
那么问题来了，什么场景下需要把值注入到静态变量？
<a name="PGdJE"></a>
#### 场景一
这个主要看静态的使用场景。比如：一个配置文件中的一个变量，多个类中都要用到，在多个类中都通过`@Value`方式获取也可以，但是弊端是：同一个参数，就有了多处引用的地方，一旦出现问题，只通过使用的的地方不方便看到，要全局搜。如果使用`public static`注入，那么整个项目使用的这个配置参数都是一致的，统一的。
<a name="GhBeG"></a>
#### 场景二
工具类中将值注入静态变量，就可以直接在静态方法之中使用。
