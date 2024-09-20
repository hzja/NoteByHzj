Java<br />Feign的超时时间设置方式并不固定，它取决于Feign在项目中是如何使用的，不同的使用方式，超时时间设置方式也不大相同，甚至还可能有坑。
<a name="kBII9"></a>
## 前置知识
由于涉及到Feign的底层知识，如果不懂点Feign的基本概念的话，后面就看不下去了<br />所以为了方便不了解Feign的小伙伴也能够读得懂文章，这里简单地说说Feign的原理，点到为止，虽然不深入，但足够应付这篇文章了
<a name="FDliJ"></a>
### Feign的作用
在项目中，经常需要调用第三方提供的Http接口，此时就可以使用一些Http框架来实现，比如HttpClient
```java
public class HttpClientDemo {

    public static void main(String[] args) throws Exception {
        //创建一个HttpClient
        HttpClient httpClient = HttpClientBuilder.create().build();

        //构建一个get请求
        HttpGet httpGet = new HttpGet("http://192.168.100.1:8080/order/1");

        //发送请求，获取响应
        HttpResponse httpResponse = httpClient.execute(httpGet);
        HttpEntity httpEntity = httpResponse.getEntity();

        //读出响应值
        String response = EntityUtils.toString(httpEntity);

        System.out.println("Response: " + response);
    }

}
```
如果项目中只有一两个这种第三方接口这样写还行，但是一旦这种三方接口过多的话，每次都得这样组装参数，发送请求，写一堆同样的代码，就显然很麻烦了。<br />所以为了简化发送Http请求的开发，减少重复代码，Feign就出现了。<br />Feign是一个声明式的Http框架<br />当需要调用Http接口时，需要声明一个接口，加一些注解就可以了<br />而像组装参数、发送Http请求等重复性的工作都交给Feign来完成。
<a name="uB7nS"></a>
### Feign的原理
虽然有了接口，但是仅仅有接口是不够的，因为接口又不能创建对象，得需要对象。<br />Feign为了方便为接口创建对象，提供的`Feign.Builder`这个内部类<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753298720-a7f2c0a6-d54d-4cce-bc88-104f8f652ec8.png#averageHue=%23954f31&clientId=u82fc31fd-39d2-4&from=paste&id=ub1cb4568&originHeight=528&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u086202fe-c3b9-4053-af51-d23d30cc5fa&title=)<br />这个类的作用就是解析接口的上的注解，为接口生成一个动态代理对象，后面通过这个代理对象就可以发送请求了。<br />这个内部类有很多属性，这些属性都是Feign的核心组件。<br />在这些核心的组件中有一个叫Client的，上图中圈出来了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753298675-f99707a0-f19b-499d-af15-4fe25fe834f5.png#averageHue=%232c2b2b&clientId=u82fc31fd-39d2-4&from=paste&id=uc03ac314&originHeight=375&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u902a15e4-5341-49b3-aa75-3401b0da7a6&title=)<br />这个Client类划个重点，非常非常重要，本文讨论的东西跟他有密切关系。<br />它只有一个方法`Response execute(Request request, Options options)`<br />方法的第一个参数`Request`就是封装了http请求的url、请求方法，请求头、请求体之类的参数<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753298720-dac844dd-a286-4ea3-b83d-590dbf7da45e.png#averageHue=%2371553b&clientId=u82fc31fd-39d2-4&from=paste&id=uc4a272ac&originHeight=335&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4a3e2978-885f-45b5-91fd-adb9e78dc35&title=)<br />第二个参数`Options`就是本文的主题，封装了超时时间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753298732-df05913e-e977-4ea1-a035-bbf4689f13b7.png#averageHue=%23372e2b&clientId=u82fc31fd-39d2-4&from=paste&id=uba9c0df9&originHeight=380&originWidth=836&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3db981ae-9544-4e93-963c-74c6d4969c4&title=)<br />返回值`Response`就是封装了一些响应码status、响应头之类的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753298722-4f9835e4-20a4-4011-b4db-7e7a1728f20f.png#averageHue=%23302d2b&clientId=u82fc31fd-39d2-4&from=paste&id=u71c0469c&originHeight=374&originWidth=994&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uef917baa-3e62-42e9-b31e-ce20a38af65&title=)<br />所以通过方法的参数和返回值也可以猜出来，这个`Client`作用是用来组装Http请求参数，发送Http请求的<br />并且http请求超时时间是根据传给`Client`的`Options`参数来决定的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753299611-f637ff1b-6ee3-4b08-937f-30329478db92.png#averageHue=%23f3eedf&clientId=u82fc31fd-39d2-4&from=paste&id=ubc6caf67&originHeight=61&originWidth=301&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u95dd8fb6-31fc-41d3-a8c6-9db5fadde42&title=)
<a name="HmEus"></a>
## Feign单独使用时超时时间设置
Feign本身就是一个http客户端，可独立使用，Feign提供了两种超时时间设置方式
<a name="oSnwM"></a>
### 1、通过`Feign.Builder`设置
前面提到，`Feign.Builder`的作用是为接口的动态代理对象的<br />`Feign.Builder`里面有很多属性，其中就有关于超时时间的属性`Options`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753299697-0d61fc3e-1206-4409-ad56-db7199c15279.png#averageHue=%23312d2b&clientId=u82fc31fd-39d2-4&from=paste&id=u693ed2a2&originHeight=645&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u143ab58e-c657-4c90-ba67-a7ba216c5ca&title=)<br />如果不设置，那么超时时间就是默认的<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753299961-f80dfba5-131b-49a2-a80c-09188257281f.png#averageHue=%232e2d2c&clientId=u82fc31fd-39d2-4&from=paste&id=u2f30ceea&originHeight=172&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u97b4c31c-cd58-48fc-84ae-885b0a93283&title=)<br />默认的就是连接超时10s，读超时60s<br />所以可以通过设置`Feign.Builder`中的`options`来设置超时时间
<a name="ps0kL"></a>
#### 来个demo
环境准备，就是一个简单的SpringBoot项目，引入一个Feign的依赖
```xml
<dependencies>
  <dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-openfeign</artifactId>
    <version>2.2.5.RELEASE</version>
  </dependency>
</dependencies>
```
声明接口 + 注解
```java
public interface UserApi {

    @RequestLine("GET /user/{userId}")
    User queryUser(@Param("userId") Integer userId);

}
```
这里演示的是Feign原生的使用方式，脱离于SpringCloud环境，所以Spring的那些`@GetMappring`就不支持了，改用Feign本身提供的注解<br />测试代码
```java
public class FeignDemo {

    public static void main(String[] args) {
        UserApi client = Feign.builder()
                //设置连接和读超时间都是5s
                .options(new Request.Options(5, TimeUnit.SECONDS, 5, TimeUnit.SECONDS, true))
                .target(UserApi.class, "http://localhost:8088");

        User user = client.queryUser(123);
    }

}
```
这里面的请求路径都是不存在的，因为只关心传给`Client`的`Options`参数值<br />`Client`在不设置的时候，就用默认的实现`Client.Default`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753299897-d5a13645-8b6d-434a-a2d1-fead484320b9.png#averageHue=%23322e2d&clientId=u82fc31fd-39d2-4&from=paste&id=udb8a1b3a&originHeight=495&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u31f9325a-3102-47d4-b870-05aacde4dc6&title=)<br />断点打到`execute`方法的实现，运行，走起<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300102-dc511792-1c9a-42ce-b93c-9ce50369229e.png#averageHue=%23484947&clientId=u82fc31fd-39d2-4&from=paste&id=u5d64230a&originHeight=225&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8625bac8-331c-41c9-affb-9a60c500ff3&title=)<br />结果就是设置的5s
<a name="OM1YP"></a>
### 2、在接口方法参数设置
除了在通过`Feign.Builder`时设置之外，Feign还支持在接口的方法参数上设置<br />此时只需要在接口的方法上加一个`Options`类型的参数
```java
@RequestLine("GET /user/{userId}")
User queryUser(@Param("userId") Integer userId, Request.Options options);
```
这样在传参数时就可以设置超时时间了
```java
User user = client.queryUser(123, new Request.Options(3, TimeUnit.SECONDS, 3, TimeUnit.SECONDS, true));
```
同样地，debug就可以看见设置的3s了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300150-1fdd1b39-7936-4858-8655-13f26eaa87f7.png#averageHue=%235b6445&clientId=u82fc31fd-39d2-4&from=paste&id=u4a4f3e17&originHeight=236&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueb74b4cb-38d2-4bbd-9c18-72f0899adcc&title=)<br />这两种设置超时时间的主要区别就是方法参数设置超时时间的优先级高于`Feign.Builder`设置的超时时间<br />用一张图来总结一下上面的关系<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300361-dc990356-65fd-4592-899c-b551951c7db3.png#averageHue=%23f6f4ee&clientId=u82fc31fd-39d2-4&from=paste&id=u54200ebf&originHeight=101&originWidth=481&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u926c5663-8c36-427d-b5aa-876ad5022fb&title=)<br />所以，如果单独使用Feign的时候，就可以通过如上的两种方式来设置超时时间。
<a name="Stnwn"></a>
## SpringCloud下Feign单独使用超时时间设置
在SpringCloud环境下，只是对Feign进行了一层包装，所以即使没有Ribbon和注册中心，Feign也是可以单独使用的，但是用法有点变化

- 注解都换成SpringMVC的注解
- 接口上需要加`@FeignClient`注解
- 用`@EnableFeignClients`扫描这些接口

不过，默认情况下Feign还是需要结合Ribbon来使用的<br />如果只想单独使用Feign，那么就设置一下`@FeignClient`注解的url属性，指定请求的地址和端口就可以了<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300468-25f52e69-1253-4053-b7b8-965ed34f3681.png#averageHue=%23322d2b&clientId=u82fc31fd-39d2-4&from=paste&id=u8c2d96a7&originHeight=490&originWidth=938&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u92701c67-9e8e-4d34-bb5b-c0fc0d6d1f3&title=)<br />所以，既然只是包装，前面提到的两种方式设置超时时间当然可以继续使用：

- 通过`Feign.Builder`
- 通过接口的方法参数

方法参数设置形式跟前面提到的一模一样，但是通过`Feign.Builder`来设置却不太一样<br />由于SpringCloud会自己创建`Feign.Builder`，不需要创建，所以在设置`Options`时，Spring提供了两种快捷方式来设置<br />不过最终还是设置到`Feign.Builder`中
<a name="vnsD5"></a>
### 1、声明一个Options Bean
Spring在构建`Feign.Builder`的时，会从容器中查找`Options`这个Bean，然后设置到`Feign.Builder`中
```java
@Configuration
public class FeignConfiguration {

    @Bean
    public Request.Options options() {
        return new Request.Options(8, TimeUnit.SECONDS, 8, TimeUnit.SECONDS, true);
    }

}
```
此时debug就可以看到设置到`Feign.Builder`的代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300448-9cbbaa1a-7328-4d05-80d4-a8e9518ea5b7.png#averageHue=%23545655&clientId=u82fc31fd-39d2-4&from=paste&id=ud608acf5&originHeight=274&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4bcbf037-76c5-4a9f-8a35-dd8bbcc449a&title=)<br />这段代码在`FeignClientFactoryBean`中的`configureUsingConfiguration`方法中
<a name="UMZOJ"></a>
### 2、配置文件中设置
除了声明Bean之外，Spring还提供了通过配置文件的方式配置，如下：
```yaml
feign:
  client:
    config:
      default:
        connectTimeout: 10000
        readTimeout: 10000
```
同样地，debug就可以看见<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300601-cc0f37bc-6dd8-4cde-a801-49b9504a24d2.png#averageHue=%23414240&clientId=u82fc31fd-39d2-4&from=paste&id=u5ce77fbd&originHeight=490&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub5afdd19-b50e-47c2-904e-cf674f535ac&title=)<br />这段代码在`FeignClientFactoryBean`中的`configureUsingConfiguration`方法中<br />声明Bean和配置文件都可以设置，那么同时设置哪种优先级高呢？<br />如**无特殊配置**，遵守SpringBoot本身的配置规定<br />约定 > 配置 > 编码<br />所以基于这个规定，配置文件的配置优先级大于手动声明Bean的优先级。<br />到这，又学到了两种Spring为了方便设置`Feign.Builder`提供的配置方式：

- 声明Options Bean
- 配置文件

把他们俩加到前面画的图中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300685-5edba607-39ee-42e4-867f-30742388c155.png#averageHue=%23312e23&clientId=u82fc31fd-39d2-4&from=paste&id=uf975a3d0&originHeight=131&originWidth=651&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u429b3512-0142-4858-8551-57d587d7e3b&title=)<br />所以，如果使用了SpringCloud提供的方式来使用Feign，那么就可以通过声明OptionsBean和配置文件的方式更加方便地来设置超时时间<br />最终其实还是通过`Feign.Builder`来设置的
<a name="VfZuD"></a>
## SpringCloud下通过Ribbon来设置
当Feign配合Ribbon使用时，除了上面两种方式之外，还可以通过Ribbon来设置超时时间。<br />Ribbon不是负载均衡组件，怎么可以设置超时时间？<br />其实这跟Ribbon的定位有关，除了负载均衡组件之外，Ribbon也干发送Http请求的事，也就是不配合Feign，他照样可以发送http请求。<br />来个简单demo<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300842-16ef9f63-e1fb-4b80-a127-ca9b1917459e.png#averageHue=%232c2c2b&clientId=u82fc31fd-39d2-4&from=paste&id=ud320cea6&originHeight=509&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud33760ce-5312-47df-90c9-aa3e58e6e7f&title=)<br />解释一下上面的代码意思

- 第一步，设置user服务的两个服务实例地址
- 第二步，获取user服务对应的RestClient，这RestClient就可以用来发送http请求
- 第三步，构建一个http请求
- 第四步，就是发送http请求，以负载均衡的方式

这样，此时就会从两个服务实例中根据负载均衡选取一个服务地址发送http请求，<br />Ribbon既然可以发送Http请求，那么自然而然就可以设置超时时间<br />Feign在整合Ribbon的时候，为了统一配置，就默认将自己的超时时间交由Ribbon管理<br />所以，在默认情况下，Feign的超时时间可以由Ribbon配置<br />而Ribbon默认连接和读超时时间只有1s，所以在默认情况下，Feign的超时时间只有1s。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300868-778d2847-18ae-42eb-9166-a6777df5bedc.png#averageHue=%23312d2c&clientId=u82fc31fd-39d2-4&from=paste&id=ueed9b157&originHeight=981&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4541d4d7-518d-4288-986a-3b24df5fd5a&title=)<br />`IClientConfig`是Ribbon的配置类，Ribbon所有的配置都可以从`IClientConfig`中获取。<br />所以，在默认情况下，很容易就发生超时，不过可以通过配置文件修改即可
```yaml
ribbon:
  ConnectTimeout: 5000
  ReadTimeout: 5000
```
发现没，上面说通过Ribbon设置Feign的超时时间，一直提到前面一直提到这个词<br />默认<br />什么情况下叫默认呢？<br />所谓的默认，就是当不主动设置Feign的超时时间的时候，就是默认。<br />换句话说，一旦通过上面说的那些配置方式设置Feign的超时时间，就不是默认了<br />此时通过Ribbon设置的超时时间就不会生效了
<a name="bd3CB"></a>
### Feign是如何在默认情况下将超时时间交给Ribbon管理的？
要想回答这个问题，就得先搬出前面反复提到的Client接口了。<br />在SpringCloud的环境下，有一个Client的实现，叫`LoadBalancerFeignClient`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753300912-60fa33a4-684f-48ba-b242-b1938695b9da.png#averageHue=%232f2c2b&clientId=u82fc31fd-39d2-4&from=paste&id=uaf866fc8&originHeight=697&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubcdcbb03-6a73-4177-a48a-d838027a43e&title=)<br />通过名字就可以看出，带有负载均衡的Client实现，负载均衡的实现肯定是交给Ribbon来实现的<br />所以当Feign配合Ribbon时用的就是这个Client实现<br />既然实现了Client接口，那就看看`execute`方法的实现逻辑<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301178-66cf0bf3-122d-401f-b258-e8f804ab54bc.png#averageHue=%232f2c2b&clientId=u82fc31fd-39d2-4&from=paste&id=u0932b1ee&originHeight=941&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u11908f67-5b60-4854-86d8-892bd7b5eab&title=)<br />图中`getClientConfig`方法就是判断使用Feign或者Ribbon配置的核心逻辑<br />核心的判断逻辑就是这一行
```java
options == DEFAULT_OPTIONS
```
`DEFAULT_OPTIONS`就是一个超时时间的常量<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301174-50e40fae-7dc2-40a3-a27f-b64ce50c1244.png#averageHue=%23332c2a&clientId=u82fc31fd-39d2-4&from=paste&id=ud2c08c93&originHeight=316&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5b48f18f-b005-44a3-be7d-3cfc59f36f6&title=)<br />当上述判断条件成立时，就会通过`this.clientFactory.getClientConfig(clientName)`获取到Ribbon配置<br />由于这是Ribbon的逻辑，这里就不深扒了，知道是这个意思就行<br />当条件不成立时，用`Options`构建一个`FeignOptionsClientConfig`<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301451-1891ce4e-021b-4a55-9272-b94b90ea28c7.png#averageHue=%23302c2b&clientId=u82fc31fd-39d2-4&from=paste&id=ua9c8e762&originHeight=888&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u378cceae-4f6f-40db-8d0c-b1dd70a82a4&title=)<br />`FeignOptionsClientConfig`就是简单地将`Options`配置读出来，设置到父类`DefaultClientConfigImpl`超时时间配置上<br />`DefaultClientConfigImpl`就算不知道是什么也无所谓，能看出的一件事就是，超时时间用的是传递给`Client`的`Options`参数<br />所以，综上，问题就变得非常easy了，那就是什么时候
```java
options == DEFAULT_OPTIONS
```
只有当这个条件成立时，才使用Ribbon的配置。<br />这里先来捋一捋前面提到的东西<br />前面反复提到，Client的Options最终只来自于两种配置

- `Feign.Builder`
- 方法参数

所以`DEFAULT_OPTIONS`这个`Options`一定是通过上面两种方法中的其中一种设置的<br />而方法参数是不可能设置的成`DEFAULT_OPTIONS`<br />因为这是我们控制的，只要参数不传`DEFAULT_OPTIONS`，那么永远都不可能是`DEFAULT_OPTIONS`。<br />此时只剩下一种情况，那就是Spring在构建在`Feign.Builder`的时候，设置成`DEFAULT_OPTIONS`。<br />通过查找`DEFAULT_OPTIONS`的使用，可以追踪到这么一段代码<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301543-93e5a478-3ee0-43b9-a31d-3e8ed4e7927e.png#averageHue=%232d2c2b&clientId=u82fc31fd-39d2-4&from=paste&id=u37e93634&originHeight=252&originWidth=1010&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3a485089-d929-4ce5-9ab9-cda1a5db13c&title=)<br />这不就是前面提到的通过声明Bean的方式来设置超时时间<br />不同的是它加了`@ConditionalOnMissingBean`，这个注解就是说，一旦自己没有声明`Options`，就用他这个`Options`<br />到这终于真像大白了。<br />不设置超时时间，Spring就会给`Feign.Builder`加一个`DEFAULT_OPTIONS`这个`Options`<br />在执行的时候，发现是`DEFAULT_OPTIONS`，说明没有主动设置过超是时间，就会使用Ribbon的超时时间。<br />为了方便理清上面的逻辑，这里整一张图<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301645-f9c0fdad-ef63-4f5a-938f-b2299115aa88.png#averageHue=%23f8f7f6&clientId=u82fc31fd-39d2-4&from=paste&id=ucf8449dc&originHeight=432&originWidth=342&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u55f2bcf8-43f7-40cc-b300-224fa3d4f32&title=)<br />虽然Feign可以使用Ribbon的超时时间，但是Ribbon的配置的优先级是最最低的<br />方法参数 > Feign配置文件 > 声明Options > Ribbon配置
<a name="oCRiz"></a>
### Feign or Ribbon配置用哪个好？
其实个人更倾向于使用Ribbon的配置方式。<br />因为Ribbon除了可以设置超时时间之外，还可以配置重试机制、负载均衡等其它的配置<br />为了简化和统一管理配置，使用Ribbon来配置超时时间。<br />可能会有疑问，Feign也支持重试机制，为什么不选择Feign？<br />这是因为Feign重试机制没有Ribbon的好<br />Ribbon重试的时候会换一个服务实例来重试，因为原来出错的可能不可用<br />而Feign并不会换一个服务实例重试，他并不知道上一次使用的是哪个服务实例，这就导致可能会出现在一个不可用的服务实例上多次重试的情况。
<a name="n8aWw"></a>
## 引入Hystrix时超时时间设置
如果之前的确没有研究过关于Feign超时时间的配置关系，那么此时应该有所收获了。<br />但是这就结束了么？<br />不，事情没那么简单。<br />如果项目中使用了Hystrix，那么就得小心前面说的那些配置了。<br />由于Hystrix跟Feign毕竟是一家人，所以当引入Hystrix时，Feign就跟之前不一样了。<br />Hystrix会去干一件事，那就是给每个Feign的http接口保护起来，毕竟Hystrix就是干保镖这个事的。<br />但是这没保护还好，一保护问题就不自觉地出现了。<br />Hystrix在保护的时候，一旦发现被保护的接口执行的时间超过Hystrix设置的最大时间，就直接进行降级操作。<br />怎么降级的，这里咱不关心，咱关心的是这个Hystrix超时的最大值是多少。<br />因为一旦这个时间小于Feign的超时时间，那么就会出现Http接口正在执行，也没有异常，仅仅是因为执行时间长，就被降级了。<br />而Hystrix的默认的超时时间的最大值就只有1s。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1692753301724-93f6a3de-4058-4582-983b-5e61a76ceec0.png#averageHue=%23342f2c&clientId=u82fc31fd-39d2-4&from=paste&id=ue92a7dc0&originHeight=297&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u171e0f7f-cf0f-45f7-b16f-ddff325035c&title=)<br />所以就算Feign超时时间设置的再大，超过1s就算超时，然后被降级，太坑了。。<br />所以需要修改这个默认的超时时间的最大值，具体的配置项如下
```yaml
hystrix:
  command:
    default:
      execution:
        isolation:
          thread:
            timeoutInMilliseconds: 30000
```
并且时间上大致要符合下面这个原则<br />Hystrix超时时间 >= (连接超时时间 + 读超时时间) * 重试次数<br />重试次数前面也提到了，虽然一般不设置，但是为了严谨还是得加上，因为一次Http接口的执行时间肯定跟重试次数有关，重试次数越多，时间就越长。<br />而连接超时时间 + 读超时时间设置方式，前面提到很多次，不论是通过Feign本身设置还是通过Ribbon来设置，都是可以的
<a name="Uucw1"></a>
## 总结
了解了不同使用条件下Feign的超时时间设置，总结起来大致如下：

- 单独使用Feign时：通过`Feign.Builder`和方法参数
- SpringCloud环境下单独使用Feign：方法参数、配置文件、声明Options Bean
- 跟Ribbon配合使用：通过Ribbon的超时参数设置
- 跟Hystrix配合使用：修改默认的超时时间，尽量符合 Hystrix超时时间 >= (连接超时时间 + 读超时时间) * 重试次数
