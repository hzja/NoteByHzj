JavaSpringCloud<br />zuul中如果两个filter的order一样，是如何排序的？
<a name="tzNrq"></a>
## zuul是干什么的
如果有使用过SpringCloud应该听说过zuul，它的定位是分布式微服务中的API网关服务，当然后面可能要被GateWay替代了。zuul是一个L7应用程序网关，提供了动态路由，监视，弹性，安全性等功能。zuul的大部分功能是通过filter实现的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125042-7989e67c-5619-451d-a0dd-da7cf79aa288.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u49768d54&originHeight=356&originWidth=754&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5773daa1-a84c-4114-b555-0d5861b9651&title=)<br />zuul定义了四种不同生命周期的filter<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125087-b8f43425-9b66-4762-95f9-7abe968684cd.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u7876dc8f&originHeight=416&originWidth=672&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u584217fe-a757-4939-a563-8d63cc20db7&title=)<br />为了方便操作，zuul内置了一些filter，这些filter主要通过`@EnableZuulServer`和`@EnableZuulProxy`注解开启相关功能。`@EnableZuulServer`注解开启的filter功能如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125079-104f3885-4b9c-437a-b5c3-c2aa2bd88f8e.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u9bd9af91&originHeight=299&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u99f4d617-3656-45c4-b854-fe0337a28f1&title=)<br />`@EnableZuulProxy`注解除了开启上面这些filter功能之外，还开启了如下的功能：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125017-ea9a3e70-d462-4bbd-b98e-60c3c4f0c7e5.png#clientId=uff64c4bd-7a0e-4&from=paste&id=uf1b7ba41&originHeight=320&originWidth=950&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9ad6c86a-ef8d-4f44-815d-ae76454a277&title=)
<a name="GcxxN"></a>
## 如何自定义filter
只需继承`ZuulFilter`类，实现它的`filterType`、`filterOrder`、`shouldFilter` 和 `run`方法即可，具体实现可参考如下代码：
```java
public class LogFilter extends ZuulFilter {

    @Override
    public String filterType() {
        return FilterConstants.PRE_TYPE;
    }

    @Override
    public int filterOrder() {
        return 1;
    }

    @Override
    public boolean shouldFilter() {
        return RequestContext.getCurrentContext().sendZuulResponse();
    }

    @Override
    public Object run() throws ZuulException {
        RequestContext currentContext = RequestContext.getCurrentContext();
        HttpServletRequest request = currentContext.getRequest();
        log.info("zuul pre filter-->" + request.getRequestURL() + "-->" + request.getMethod());
        return null;
    }
}
```
上面的四个方法有哪些作用呢？

| 方法名称 | 作用 |
| --- | --- |
| filterType | filter类型，包含：pre、routing、post和error四种类型 |
| filterOrder | 排序，该值越小，filter越早执行 |
| shouldFilter | 开关，表示是否需要执行该filter |
| run | filter具体的功能方法 |

需要注意的是，要想使zuul的功能生效，切记要在SpringBoot启动类上定义`@EnableZuulServer`或`@EnableZuulProxy`注解，表示开启zuul的功能。
<a name="JMZIP"></a>
## `filterOrder`是如何排序的
先看看所有的`zuulFilter`在哪里执行的，谜底就在`FilterProcessor`类的`runFilters`方法中。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125121-fd7e85d9-d52c-426b-ac42-16f9a5b1ef27.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u09cd85d2&originHeight=553&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf5e14ffe-d494-4f0f-af7f-2e1f6edc979&title=)<br />该方法很简单，先获取所有`zuulFilter`，然后遍历所有`zuulFilter`，调用`processZuulFilter`方法执行具体的`zuulFilter`，然后将执行结果返回。<br />重点看看这个方法
```java
FilterLoader.getInstance().getFiltersByType(sType);
```
该方法的具体逻辑![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125487-bd9add9e-4c5c-4692-b1d6-4fa5c4f288d8.png#clientId=uff64c4bd-7a0e-4&from=paste&id=uf1fbbadc&originHeight=603&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u96e0e6c8-a71b-45d7-85fd-13c2f365496&title=)

1. **根据filterType从缓存中获取filter集合，如果缓存中有直接返回**
2. **如果缓存中没有，则创建filter集合，将所有**`**filter**`**中跟filterType的filter添加到filter集合中。**
3. **排序**`**filter**`**集合**
4. **将新创建的**`**filter**`**集合放入缓存。**

从上面可以看出`filter`的排序是通过如下方法执行的：
```java
Collections.sort(list);
```
该方法底层其实是通过list的`sort`方法实现的![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125485-9f70b710-37a4-4581-bee3-e83264b6c3eb.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u438c1252&originHeight=115&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u808df603-5132-434d-b86f-88f6effd356&title=)<br />看看`ArrayList`的`sort`方法，传入的`Comparator`为null<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125549-b9304873-0e76-4682-93ae-d9335f30750e.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u34f69ab8&originHeight=298&originWidth=830&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf61defeb-f15c-4d81-8b63-fc9a85f52d7&title=)它的底层又是通过`Arrays`类的静态方法`sort`实现的<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125647-ced110e0-defd-4d96-9cb7-9eb0bcd2fae1.png#clientId=uff64c4bd-7a0e-4&from=paste&id=ud5cf32b8&originHeight=290&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2f4a0102-9c7d-458b-9aba-135d8cf41cf&title=)由于上一步`Comparator`为null，则会执行`sort`方法。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125697-5fc7dbcd-34c1-48b2-b908-4c459afcc94c.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u6be9037d&originHeight=179&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u732bb67f-3d12-4a5b-978c-b7b8eeadd73&title=)该方法是通过`ComparableTimSort`类的`sort`方法实现的，这个方法是最核心的方法了<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125851-63919b75-9e87-4f71-b311-bb595594492d.png#clientId=uff64c4bd-7a0e-4&from=paste&id=ue4976d96&originHeight=754&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ua83280f8-f0ae-4d88-a8e5-3d0b6279754&title=)可以看到该方法其实是通过`binarySort`二分查找排序的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529125904-35d78d98-e905-4b0c-93ce-670e8dffaaea.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u76642385&originHeight=340&originWidth=668&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6b56894a-9e64-475a-b850-2236e3d96a7&title=)<br />通过`compareTo`方法比较大小。<br />回头再看看`ZuulFilter`类<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126008-9b83e99d-086b-4286-afcf-a008ab18d1a3.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u2244733a&originHeight=244&originWidth=600&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u064a8cc0-f0cd-44e4-9204-b0c8522d504&title=)<br />它实现了`Comparable`接口，重写了`compareTo`方法<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126175-44db8c16-54ef-4231-b229-7e935d19333d.png#clientId=uff64c4bd-7a0e-4&from=paste&id=uee843550&originHeight=128&originWidth=1002&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf80e9543-bd28-43e4-92b9-662bba13a0d&title=)<br />所以，看到这里可以得出结论：`ZuulFilter`是通过`Integer`的`compare`方法比较`filterOrder`参数值大小来排序的。
<a name="KZOEB"></a>
## 如果`filterOrder`一样如何排序？
看看`Integer`的`compare`方法具体的逻辑![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126144-f8c83a1e-a9c6-4f51-9287-df0d1d48df45.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u10eb8c63&originHeight=126&originWidth=722&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0c349c7d-a107-4984-af6a-5442264ebb4&title=)<br />如果x==y，则返回0，x<y，则返回 -1，否则返回1 前面在二分查找中，只有x<y时，才会交换位置。![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126338-96aeaea9-ecfa-4f5e-bd70-0a5853104c84.png#clientId=uff64c4bd-7a0e-4&from=paste&id=ub05dda76&originHeight=374&originWidth=716&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u57445c27-af72-46e1-908e-2d555aee38f&title=)<br />可以看到，这里得出这样的结论，如果`filterOrder`一样，则`Collections.sort(list);`排序时不交换位置，这按照`ZuulFilter`默认加载顺序。那么，ZuulFilter的默认加载顺序是怎么样的？<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126312-59693658-4806-4b5e-ae40-590b7828e102.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u543789cd&originHeight=485&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uc7404805-dda8-4069-acea-0a8e6734dc2&title=)它是通过`getAllFilters`方法获取`ZuulFilter`集合，该方法其实返回的是名称为`filters`的`ConcurrentHashMap`的`values`，即返回Set集合，是无序的。

- **重要的事情说三遍：如果**`**filterOrder**`**一样，**`**ZuulFilter**`**是无序的。**
- **重要的事情说三遍：如果**`**filterOrder**`**一样，**`**ZuulFilter**`**是无序的。**
- **重要的事情说三遍：如果**`**filterOrder**`**一样，**`**ZuulFilter**`**是无序的。**

所以，`filterOrder`切记不要定义相同的，不然可能会出现无法预知的执行结果。
<a name="Jntbq"></a>
## 两种排序方法
自定义排序其实有两种方法：

- **实现**`**Comparable**`**接口，重写**`**compareTo**`**方法，**
- **实现**`**Comparator**`**接口，重写**`**compare**`**方法**如果要使用`Collections.sort(list);`排序，它默认用的是第一种方法，上面的`filterOrder`之所以可以排序，是因为`Integer`实现了`Comparable`接口，重写了`compareTo`方法

![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126471-b623fac8-fc96-47ee-ab25-76a83cf93d20.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u4f9b7a70&originHeight=132&originWidth=836&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u070753a5-6487-40c5-80c8-355b84f6c0f&title=)<br />如果想自己定义排序规则可以通过实现`Comparator`接口，重写`compare`方法。
```java
Collections.sort(list,new Comparator<Integer>(){
    @Override
    public int compare(Integer o1, Integer o2) {
        return o2 - o1;
    }
});
```
它的底层也是通过二分查找实现的![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126611-5b487a61-d0a8-4d23-a099-cf41a9c05bf3.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u938d0572&originHeight=819&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ubb8e817d-4933-4834-ab0b-ae58a4fe342&title=)
<a name="KFlyI"></a>
### 那么这两种方法有什么区别呢？

- `**Comparable**`**接口位于java.lang包下，而**`**Comparator**`**接口位于java.util包下。**
- `**Comparable**`**接口是内部比较器，一个类如果想要使用**`**Collections.sort(list)**`** 方法进行排序，则需要实现该接口**
- `**Comparator**`**接口是外部比较器用于对那些没有实现**`**Comparable**`**接口或者对已经实现的**`**Comparable**`**中的排序规则不满意进行排序.无需改变类的结构，更加灵活。**
<a name="V77sp"></a>
## 彩蛋
zuul中是通过`filterOrder`参数的大小排序的，而在spring中是通过`@Order`注解排序的。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126528-300f6c29-d3ad-41bf-b5aa-c33241bf691c.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u30d81488&originHeight=488&originWidth=1008&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8713ad96-5d4e-4a2f-b812-4e65e1a064b&title=)默认情况下，如果不指定value值，则value是Integer的最大值。由于排序规则是value越小，则排在越靠前，所以如果不指定value值，则它排在最后。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126681-9ca0b57e-4d34-410b-a8ce-e29edb51e53e.png#clientId=uff64c4bd-7a0e-4&from=paste&id=ub1c4a0ef&originHeight=62&originWidth=662&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5eaf109e-94b6-421e-a1e1-faa31b21bda&title=)<br />Spring是通过`OrderComparator`类排序的，它实现了`Comparator`接口，它的`doCompare`方法实现的排序。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1659529126665-8fc3436c-a9ae-4ad0-85dc-4b366024c1a8.png#clientId=uff64c4bd-7a0e-4&from=paste&id=u9e291ad9&originHeight=356&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u5cd46448-d6ff-4c3b-bb2f-56cc9cfb981&title=)最终也是调用`Integer`类的`compare`方法，该方法前面已经介绍过了。
