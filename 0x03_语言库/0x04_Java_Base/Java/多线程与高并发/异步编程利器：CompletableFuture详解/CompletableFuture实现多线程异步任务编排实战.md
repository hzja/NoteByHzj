Java
<a name="jdjo7"></a>
## 问题背景
问题：当查询接口较复杂时候，数据的获取都需要[**远程调用**]，必然需要花费更多的时间。假如查询文章详情页面，需要如下标注的时间才能完成，比如如下场景：

1. 查询文章详情 0.5s
2. 查询文章博主个人信息 0.5s
3. 查询文章评论 1s
4. 查询博主相关文章分类 1s
5. 相关推荐文章 1s

上面的描述只是举个例子不要在意这里的查询描述，看实际情况使用，有些相关的查询可以拆分接口实现，上面的描述只是为了举例子。<br />那么，用户需要4s后才能统计的数据。很显然是不能接受的。如果有多个线程同时完成这4步操作，也许只需要1s左右即可完成响应。
<a name="dCXCF"></a>
## `CompletableFuture`介绍
在Java 8中，新增加了一个包含50个方法左右的类：`CompletableFuture`，提供了非常强大的`Future`的扩展功能，可以简化异步编程的复杂性，提供了函数式编程的能力，可以通过回调的方式处理计算结果，并且提供了转换和组合`CompletableFuture`的方法。`CompletableFuture`类实现了Future接口，所以还是可以像以前一样通过`get`方法阻塞或者轮询的方式获得结果，但是这种方式不推荐使用。`CompletableFuture`和`FutureTask`同属于Future接口的实现类，都可以获取线程的执行结果。<br />![CompletableFuture介绍](https://cdn.nlark.com/yuque/0/2023/png/396745/1676987867248-af1fd1e1-241b-4f5c-bf01-da01349641cc.png#averageHue=%23f7f7f7&clientId=ufd863630-a59f-4&from=paste&height=517&id=u40087847&originHeight=1293&originWidth=2010&originalType=binary&ratio=2.5&rotation=0&showTitle=true&size=89743&status=done&style=none&taskId=ue67d67a1-187f-46c8-a258-34bb2d93f59&title=CompletableFuture%E4%BB%8B%E7%BB%8D&width=804 "CompletableFuture介绍")
<a name="St99D"></a>
## 具体场景
先从单个任务开始
<a name="F71cE"></a>
### `runAsync`：无返回值
```java
/**
 * runAsync无返回值
 */
CompletableFuture<Void> completableFuture = CompletableFuture.runAsync(() -> {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 2;
    System.out.println("运行结果：" + i);
}, executor);
```
<a name="VuMrQ"></a>
### `supplyAsync`：有返回值
**「**`**whenComplete**`**：能感知异常，能感知结果，但没办法给返回值<br />**`**exceptionally**`**：能感知异常，不能感知结果，能给返回值。相当于，如果出现异常就返回这个值」**
```java
/**
 * supplyAsync有返回值
 * whenComplete能感知异常，能感知结果，但没办法给返回值
 * exceptionally能感知异常，不能感知结果，能给返回值。相当于，如果出现异常就返回这个值
 */
CompletableFuture<Integer> future = CompletableFuture.supplyAsync(() -> {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 0;
    System.out.println("运行结果：" + i);
    return i;
}, executor).whenComplete((res,excption)->{
    //whenComplete虽然能得到异常信息，但是没办法修改返回值
    System.out.println("异步任务成功完成...结果是："+res+";异常是："+excption);
}).exceptionally(throwable -> {
    //exceptionally能感知异常，而且能返回一个默认值，相当于，如果出现异常就返回这个值
    return 10;
});
```
<a name="g8Lzz"></a>
### `supplyAsync`：有返回值
**「handle能拿到返回结果，也能得到异常信息，也能修改返回值」**
```java
/**
 * supplyAsync有返回值
 * handle能拿到返回结果，也能得到异常信息，也能修改返回值
 */
CompletableFuture<Integer> future = CompletableFuture.supplyAsync(() -> {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 4;
    System.out.println("运行结果：" + i);
    return i;
}, executor).handle((res,excption)->{
    if(excption!=null){
        return 0;
    }else {
        return res * 2;
    }
});
```
<a name="BulBs"></a>
## 两个任务编排
两任务组合（**「线程串行化」**）<br />可以是两任务的串行化，就是一个任务执行完了再执行下一个<br />也可以是多个任务的串行化，就是按照顺序一个个的执行
<a name="x2eE0"></a>
### `thenRunAsync`：不能接收上一次的执行结果，也没返回值
```java
/**
 * thenRunXXX 不能接收上一次的执行结果，也没返回值
 * .thenRunAsync(() -> {
 *      System.out.println("任务2启动了...");
 * }, executor);
 */
CompletableFuture<Void> future = CompletableFuture.supplyAsync(() -> {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 4;
    System.out.println("运行结果：" + i);
    return i;
}, executor).thenRunAsync(() -> {
    System.out.println("任务2启动了...");
}, executor);
```
<a name="rrNv3"></a>
### `thenAcceptAsync`：能接收上一次的执行结果，但没返回值
```java
/**
 * thenAcceptXXX 能接收上一次的执行结果，但没返回值
 * .thenAcceptAsync(res->{
 *      System.out.println("任务2启动了..."+res);
 *  },executor);
 */
CompletableFuture<Void> future = CompletableFuture.supplyAsync(() -> {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 4;
    System.out.println("运行结果：" + i);
    return i;
}, executor).thenAcceptAsync(res -> {
    System.out.println("任务2启动了..." + res);
}, executor);
```
<a name="Ll36t"></a>
### `thenApplyAsync`：能接收上一次的执行结果，又可以有返回值
```java
/**
 * thenApplyXXX 能接收上一次的执行结果，又可以有返回值
 * .thenApplyAsync(res -> {
 *      System.out.println("任务2启动了..." + res);
 *      return "hello " + res;
 *  }, executor);
 */
CompletableFuture<String> future = CompletableFuture.supplyAsync(() ->         {
    System.out.println("当前线程" + Thread.currentThread().getId());
    int i = 10 / 4;
    System.out.println("运行结果：" + i);
    return i;
}, executor).thenApplyAsync(res -> {
    System.out.println("任务2启动了..." + res);
    return "hello " + res;
}, executor);
```
<a name="k6OmA"></a>
## 三任务编排
先准备两个任务
```java
CompletableFuture<Object> future01 =CompletableFuture.supplyAsync(() -> {
    System.out.println("任务1线程" + Thread.currentThread().getId());
    int i = 10 / 4;
    System.out.println("任务1结束：");
    return i;
}, executor);
CompletableFuture<Object> future02 = CompletableFuture.supplyAsync(() -> {
    System.out.println("任务2线程" + Thread.currentThread().getId());
    try {
        Thread.sleep(3000);
        System.out.println("任务2结束：");
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    return "hello";
}, executor);
```
<a name="Wo6lf"></a>
### 三任务组合，前两个任务都完成，才执行任务3
**「**`**runAfterBothAsync**`**」**：**「任务01 任务02都完成了，再开始执行任务3，不感知任务1、2的结果的，也没返回值」**
```java
CompletableFuture<Void> future = future01.runAfterBothAsync(future02, () -> {
    System.out.println("任务3开始");
}, executor);
```
**「**`**thenAcceptBothAsync**`**：任务01 任务02都完成了，再开始执行任务3，能感知到任务1、2的结果，但没返回值」**
```java
CompletableFuture<Void> future = future01.thenAcceptBothAsync(future02, (f1, f2) -> {
    System.out.println("任务3开始...得到之前的结果：f1:" + f1 + ", f2:" + f2);
}, executor);
```
**「**`**thenCombineAsync**`**」**：**「任务01 任务02都完成了，再开始执行任务3，能感知到任务1、2的结果，而且自己可以带返回值」**
```java
CompletableFuture<String> future = future01.thenCombineAsync(future02, (f1, f2) -> {
    return f1+":"+f2+"：哈哈";
}, executor);
```
<a name="En4Xs"></a>
### 三任务组合，前两个任务只要有一个完成，就执行任务3
**「**`**runAfterEitherAsync**`**」**：**「两个任务只要有一个完成，就执行任务3，不感知结果，自己没返回值」**
```java
CompletableFuture<Void> future = future01.runAfterEitherAsync(future02, () -> {
    System.out.println("任务3开始...");
}, executor);
```
**「**`**acceptEitherAsync**`**：两个任务只要有一个完成，就执行任务3，感知结果，自己没返回值」**
```java
CompletableFuture<Void> future = future01.acceptEitherAsync(future02, (res) -> {
    System.out.println("任务3开始...之前的结果" + res);
}, executor);
```
**「**`**applyToEitherAsync**`**：两个任务只要有一个完成，就执行任务3，感知结果，自己有返回值」**
```java
CompletableFuture<String> future = future01.applyToEitherAsync(future02, (res) -> {
    System.out.println("任务3开始...之前的结果" + res);
    return "任务3的结果...";
}, executor);
```
<a name="BCwjw"></a>
## 多任务的编排
```java
/**
 * 多任务组合
 */
CompletableFuture<String> futureImg = CompletableFuture.supplyAsync(() -> {
    System.out.println("查询商品图片信息");
    return "hello.jpg";
},executor);

CompletableFuture<String> futureAttr = CompletableFuture.supplyAsync(() -> {
    System.out.println("查询商品属性信息");
    return "黑色+256G";
},executor);

CompletableFuture<String> futureDesc = CompletableFuture.supplyAsync(() -> {
    try {
        Thread.sleep(3000);
        System.out.println("查询商品介绍信息");
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    return "华为...";
},executor);
```
<a name="mgt1v"></a>
### `allOf`：所有任务都执行完
```java
/**
 * allOf 所有任务都执行完
 */
CompletableFuture<Void> allOf = CompletableFuture.allOf(futureImg, futureAttr, futureDesc);
allOf.get();//等待所有结果完成
```
<a name="YJEmA"></a>
### `anyOf`：其中有一个任务执行完就可以
```java
/**
 * anyOf 其中有一个任务执行完就可以
 */
CompletableFuture<Object> anyOf = CompletableFuture.anyOf(futureImg, futureAttr, futureDesc);
anyOf.get();
```
<a name="zIxMI"></a>
## 一个实际的例子
```java
public SkuItemVo item(Long skuId) {
    SkuItemVo skuItemVo = new SkuItemVo();

    //1、sku详细信息 sku_info
    SkuInfoEntity skuInfo = getById(skuId);
    skuItemVo.setInfo(skuInfo);

    //2、sku 图片信息 sku_img
    List<SkuImagesEntity> images = skuImagesService.getImagesBySkuId(skuId);
    skuItemVo.setImages(images);

    //3、spu 销售属性组合
    List<SkuItemSaleAttrVo> saleAttr = skuSaleAttrValueService.getSaleAttrBySpuId(skuInfo.getSpuId());
    skuItemVo.setSaleAttr(saleAttr);

    //4、spu 的介绍
    SpuInfoDescEntity spuInfoDesc = spuInfoDescService.getById(skuInfo.getSpuId());
    skuItemVo.setDesc(spuInfoDesc);

    //5、spu 规格参数信息
    List<SpuItemAttrGroupVo> groupAttrs = attrGroupService.getAttrGroupWithAttrsBySpuId(skuInfo.getSpuId(),skuInfo.getCatalogId());
    skuItemVo.setGroupAttrs(groupAttrs);

    return skuItemVo;
}
```
使用`CompletableFuture`异步编排后
```java
private SkuItemVo item(Long skuId) {
    SkuItemVo skuItemVo = new SkuItemVo();

    /**
     * 3、4、5需要依赖1的运行结果，需要返回skuInfo后从中获取spuId和catalogId
     * 而2不需要依赖1的运行结果
     */

    //1、sku详细信息 sku_info
    CompletableFuture<SkuInfoEntity> infoFuture = CompletableFuture.supplyAsync(() -> {
            SkuInfoEntity skuInfo = getById(skuId);
            skuItemVo.setInfo(skuInfo);
            return skuInfo;
    }, executor);

    //2、sku 图片信息 sku_img  2不需要等待上边1的执行结果
    CompletableFuture<Void> imageFuture = CompletableFuture.runAsync(() -> {
            List<SkuImagesEntity> images = skuImagesService.getImagesBySkuId(skuId);
            skuItemVo.setImages(images);
    }, executor);

    //下边的3、4、5都需要上边1的执行结果
    //所以下边的3、4、5都是基于上边1的执行结果 infoFuture 开始的
    //都是以infoFuture.thenAcceptAsync(skuInfo -> {})开始的
    CompletableFuture<Void> saleAttrFuture = infoFuture.thenAcceptAsync(skuInfo -> {
            //3、spu 销售属性组合  3
            List<SkuItemSaleAttrVo> saleAttr = skuSaleAttrValueService.getSaleAttrBySpuId(skuInfo.getSpuId());
            skuItemVo.setSaleAttr(saleAttr);
            System.out.println(saleAttr);
    }, executor);

    CompletableFuture<Void> descFuture = infoFuture.thenAcceptAsync(skuInfo -> {
            //4、spu 的介绍
            SpuInfoDescEntity spuInfoDesc = spuInfoDescService.getById(skuInfo.getSpuId());
            skuItemVo.setDesc(spuInfoDesc);
    }, executor);

    CompletableFuture<Void> attrGroupFuture = infoFuture.thenAcceptAsync(skuInfo -> {
            //5、spu 规格参数信息
            List<SpuItemAttrGroupVo> groupAttrs = attrGroupService.getAttrGroupWithAttrsBySpuId(skuInfo.getSpuId(),skuInfo.getCatalogId());
            System.out.println(groupAttrs);
            skuItemVo.setGroupAttrs(groupAttrs);
    }, executor);

    //等待所有任务完成
    try {
            CompletableFuture.allOf(saleAttrFuture,descFuture,attrGroupFuture,imageFuture).get() ;
    } catch (InterruptedException e) {
            log.error("查询商品详情异步编排错误: ");
            log.error(e.getMessage() );
    } catch (ExecutionException e) {
            log.error(e.getMessage() );
    }
    return skuItemVo;
}
```
