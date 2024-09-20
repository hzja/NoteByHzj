Java<br />正常情况下，在Java中入参是不建议用做返回值的。除了造成代码不易理解、语义不清等问题外，可能还埋下了陷阱等你入坑。
<a name="YzO69"></a>
## 问题背景
比如有这么一段代码：
```python
@Named
public class AService {
private SupplyAssignment localSupply = new SupplyAssignment();
    @Inject
    private BService bervice;

    public List<Supply> calcSupplyAssignment()
       List<Supply> supplyList = bService.getLocalSupplyList(this.localSupply);
        …
       return supplyList;
    }
}
```
上面代码，服务A希望调用服务B，以获取supplyList，但同时，服务A又希望修改`localSupply`的状态值，未能避免修改`calcSupplyAssignment`接口的（不想改返回的类型），将`localSupply`作为了入参但同时也用作了返回值。<br />服务B代码如下：
```python
@Named
public class BService {

public List<Supply> getLocalSupplyList (SupplyAssignment localSupply)
    SupplyAssignment supplyAssignment = this.getSupplyAssignment();
        // 希望localSupply被重新赋值后返回
        localSupply = supplyAssignment;
        …
        return supplyList;

    }
}
```
在服务B代码内部，服务A的入参`localSupply`被传入，希望重新被`supplyAssignment`赋值而后返回新值。然而，这样做是无效的。
<a name="O1a6Q"></a>
## 问题原因
先来看下编程语言中关于参数传递的类型：

- 值传递（pass by value）是指在调用函数时将实际参数复制一份传递到函数中，这样在函数中如果对参数进行修改，将不会影响到实际参数。
- 引用传递（pass by reference）是指在调用函数时将实际参数的地址直接传递到函数中，那么在函数中对参数所进行的修改，将影响到实际参数。

因为Java程序设计语言是采用的值传递，因为Java没有指针的概念。也就是说方法得到的是所有参数值的一个拷贝，方法并不能修改传递给它的任何参数变量的内容。<br />因此，上述代码中，服务A调用服务B时，服务B的参数`localSupply`实际上是服务A的`localSupply`的一个拷贝，当然，这两个都是指向了同一个地址对象`supplyAssignment1`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658971240431-a32fde2c-7d3b-447b-9343-6df0108727be.png#clientId=u83663b65-e860-4&from=paste&id=u881b2dcd&originHeight=399&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u650d7c63-3cfb-4370-a3ac-7b78ad370d8&title=)<br />当在服务B内部对参数`localSupply`进行重新赋值是`localSupply = supplyAssignment`，实际上，只是对B的参数`localSupply`做了从新赋值，B的参数`localSupply`会指向一个新的地址对象`supplyAssignment2`。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658971240517-f55cebd5-9ef5-4cd0-8bdc-7b68b07285fb.png#clientId=u83663b65-e860-4&from=paste&id=u95b704bf&originHeight=409&originWidth=677&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1a46fe36-7e8e-4017-a003-26a6f1d1495&title=)<br />从上图可以清晰看到，因此，服务A的`localSupply`和B的参数`localSupply`已经指向了不同的对象了，对B的参数`localSupply`做任何的修改，都不会影响服务A的`localSupply`的原值。<br />这就是问题的原因，希望服务B来修改服务A入参的状态，并将改后的值返回给服务A，但并不奏效。
<a name="HjdSo"></a>
## 解决方案
<a name="Kh04r"></a>
### 方案1：入参不要用作返回值
有时确实想要入参做返回值，那看方案2。
<a name="kT0tO"></a>
### 方案2：入参不要赋值新对象
这个方案就是直接在入参的对象上做状态的修改，而不要去赋值新对象。还是这个图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1658971240524-b61d51fb-a6cb-4c5f-b77b-10ea9fd805c0.png#clientId=u83663b65-e860-4&from=paste&id=ufbe12780&originHeight=399&originWidth=661&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ueb8a1d85-52f3-405f-90b6-94ca071781c&title=)<br />图片<br />在这个图中，只要是一直在B的参数`localSupply`修改的是`supplyAssignment1`的状态值，那结果就能反馈到服务A的`localSupply`上。如何实现？看下下面代码：
```python
@Named
public class BService {
    public List<Supply> getLocalSupplyList (SupplyAssignment localSupply)
        SupplyAssignment supplyAssignment = this.getSupplyAssignment();
        // 针对localSupply不能新建引用，只能重新赋值属性
        BeanUtils.copyProperties(supplyAssignment, localSupply);
        …
        return supplyList;
    }
}
```
在上面的方法中，用到了Spring的工具类`BeanUtils`，该类的`copyProperties`方法的实质是将`supplyAssignment`的属性值，赋值到了`localSupply`的属性上。<br />这意味着修改的B的参数`localSupply`上的属性，而并未新建对象。
