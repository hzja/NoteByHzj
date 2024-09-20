最近在调研分布式任务如何选型，最终选择了比较年轻的PowerJob，下面会简单介绍下这个框架的使用以及它的运行流程。
<a name="oGOAC"></a>
## 一、选择PowerJob的原因
<a name="rshgS"></a>
### 1.1、同类产品对比
官方文档给出的同类产品对比图：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961773712-158662b5-6d67-45fe-8208-c0b2d9f7aa4a.png#averageHue=%23f1f3f4&clientId=u0feaca88-81ba-4&from=paste&id=u79ac58cf&originHeight=785&originWidth=846&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0c8524ad-5576-41b8-8a8f-099434f44cc&title=)<br />[https://www.yuque.com/powerjob/guidence/ztn4i5](https://www.yuque.com/powerjob/guidence/ztn4i5)
<a name="Llb9t"></a>
### 1.2、特点
定制方面：代码较简单，易于理解和改造，比如我们就集成了自己的服务发现平台来管理powerjob服务端节点。<br />功能方面：很全面，能想到的功能它全部支持<br />体积方面：非常轻量，代码量少，而且不依赖外部乱七八糟的服务（比如zk），仅需要一个mysql即可
<a name="R2stV"></a>
### 1.3、成熟度
产品上线仅3个月，已积累1.8k的star：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961773733-09425aa2-5c2b-459a-bbed-99729722183f.png#averageHue=%23fefcfc&clientId=u0feaca88-81ba-4&from=paste&id=u296a6f8a&originHeight=158&originWidth=448&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8350203b-2227-48f9-a71e-bf0e287f472&title=)<br />并且已有较大的公司和机构接入：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961773794-5193ffaa-26ee-476c-956e-e85811d50f84.png#averageHue=%23a6bcad&clientId=u0feaca88-81ba-4&from=paste&id=u00714bff&originHeight=274&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf413c742-9c4b-4dd3-a174-e9ba87517b0&title=)
<a name="vrdCM"></a>
## 二、PowerJob的工作流程
<a name="lyeNY"></a>
### 2.1、基本概念：app、worker、job、server
app可以理解为一个工程项目，worker可以理解成一个app的集群节点，而job则是一个任务（可以是简单的定时任务，也可以是复杂的MapReduce），它跟具体某个app对应，而一个app则可以拥有很多job，它们的关系如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961773695-65db4b96-23ac-4454-8c21-cb75b0b9efb7.png#averageHue=%23202018&clientId=u0feaca88-81ba-4&from=paste&id=u2f18f72f&originHeight=537&originWidth=600&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub0aecc6b-a938-4daa-b1c1-cbec9eab3d4&title=)<br />server即为PowerJob节点，主要负责任务的监听和派发，可以单点部署，也可以集群部署，它的工作流程详细参考2.2和2.3
<a name="mi90Z"></a>
### 2.2、app&server的绑定
首先，在worker里配置上所需的server节点信息（这些节点信息也可以从服务发现获取），在worker启动时会注册到server，此时server便拥有了所有app的worker信息：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961773667-2be8bc3f-0c36-47e9-bba6-d0aa1bf3d83a.png#averageHue=%23110b09&clientId=u0feaca88-81ba-4&from=paste&id=u7f58558a&originHeight=1124&originWidth=833&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u527cb53b-1af0-4b68-9aac-c990e49d434&title=)<br />这层绑定关系在worker们启动后即可确认，这时worker端会启动两个定时任务，一个是heartbeat，用来给server端发送心跳，这样server端即可知道对应app有多少个worker在运行了，另外一个是discovery，用来同步server端状态，如果有备份server，可以用来做高可用。<br />图里powerjob服务端是单点部署，这不高可用，下面来看下powerjob是如何实现服务高可用的。
<a name="ELGof"></a>
### 2.3、高可用
2.2的过程运转的不错，但是如果server端故障，那么所有的任务将直接终止无法执行，这是我们不愿意看到的，因此需要给powerjob备份一个节点，假设现在它有两个节点，那么当单点故障时，powerjob会通过discovery机制做故障转移：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961774684-5b4b5a45-5eb7-4daa-bbab-a94f93beb984.png#averageHue=%23120f0b&clientId=u0feaca88-81ba-4&from=paste&id=u6c3a9249&originHeight=1200&originWidth=673&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u234fe4e3-ab63-4deb-8920-364ca443c8e&title=)<br />目前基于集群部署，一般有3台机器，一台master，两台slave。
<a name="oy2gR"></a>
### 2.4、server端的调度
说完了整体的绑定流程，下面来详细看下server端是如何轮询和派发任务的：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961774730-5fdd9e7d-95e3-4c4c-94ad-433ff1048065.png#averageHue=%230e0c0b&clientId=u0feaca88-81ba-4&from=paste&id=u73aaa0c0&originHeight=1012&originWidth=1024&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6af71824-1e4a-4734-b981-6a8282b2615&title=)
<a name="uDMSx"></a>
### 2.5、部署顺序

1. 部署PowerJob的server端（这个一般情况下都预先部署好了）
2. 编写自己的job类app项目，写好各类job，在自身配置文件里指定一个server来调度自己
3. 前往PowerJob客户端注册该app信息
4. 启动发布该app项目（此时app集群会跟对应的server绑定上）
5. 在PowerJob客户端利用该app登录，将该app里的job配置上去（此时便可指定cron表达式、并发度、是否mapreduce之类的信息）

经过上面的步骤，你在app内编写的job程序便可被对应的PowerJob的server调度到了，不过上面的过程是裸用powerjob时需要做的，现在已经被我们大仓简化了，具体用法会出使用文档。
<a name="XBV2C"></a>
## 三、任务类型&验证
<a name="Y0caE"></a>
### 3.1、如何定义PowerJob任务
任务类必须要实现powerjob提供的一些接口，它们继承关系图如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961774706-591bc043-07d4-4b7f-b06f-4012091483cf.png#averageHue=%231b1b1b&clientId=u0feaca88-81ba-4&from=paste&id=ua4dc1c95&originHeight=510&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc5b19d34-d8f8-43e8-a412-98d72687fe8&title=)<br />业务方只需要继承（实现）这些类（接口）即可，powerjob在执行任务时会率先从spring上下文里获取实例，如果你没使用spring，那么powerjob就会利用反射机制来触发你的业务逻辑（这在下方具体实例中有所体现，表单里填写的是类的全限定名）。
<a name="X18e9"></a>
### 3.1、任务类型-单机任务
这种就是普通定期执行的任务，属于最常用最普通的任务，现在来做下测试，测试用例代码如下：
```java
@Slf4j
@Component
public class StandaloneProcessor implements BasicProcessor { //实现BasicProcessor接口

    @Override
    public ProcessResult process(TaskContext context) { //核心触发逻辑
        log.info("简单定时任务-触发！，参数是：{}", context.getJobParams());
        return new ProcessResult(true, context + ": " + true);
    }
}
```
然后将job发布，发布完成后在powerjob平台对应app下配置该任务的调度信息：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961774861-8d7bbc8d-d3b7-41c1-89a5-5f332a097e07.png#averageHue=%23fdf3f2&clientId=u0feaca88-81ba-4&from=paste&id=ubd8b4ba6&originHeight=574&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud193af92-1012-49c2-b110-fe5e13488a7&title=)<br />配好之后就可以运行job了，来看看日志平台的打印：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961774943-00fc4c3e-791a-472d-948d-690bbd520448.png#averageHue=%23ccc85c&clientId=u0feaca88-81ba-4&from=paste&id=u109e9427&originHeight=240&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucf02ff56-4ac2-4e0f-a253-d6c94e6d425&title=)
<a name="qFP7q"></a>
### 3.2、任务类型-广播任务
<a name="md0HL"></a>
#### 3.2.1 广播任务改造
将上面的任务改成广播模式：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961775662-8400123c-9ac1-457a-bfb9-7fe8f5ff62c0.png#averageHue=%23fefbfb&clientId=u0feaca88-81ba-4&from=paste&id=u49eb6ba0&originHeight=117&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf805dd19-849f-4fa2-a554-8efd1968cfe&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961775855-06abe1fb-03f5-4d16-b070-828077a8c620.png#averageHue=%23cdc95e&clientId=u0feaca88-81ba-4&from=paste&id=uf91e92d2&originHeight=234&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue1fdd5c6-7c73-4b4e-b222-f50588cadce&title=)<br />其实就是由原先单机触发，广播给worker集群里每个节点都触发一次。
<a name="jCS4u"></a>
#### 3.2.2 广播模式接口实现
实例代码如下：
```java
@Slf4j
@Component
public class BroadcastProcessorDemo extends BroadcastProcessor { //继承BroadcastProcessor类

    @Override
    public ProcessResult preProcess(TaskContext context) throws Exception { //在所有节点广播执行前执行，只会在一台机器执行一次
        log.info("广播前，参数：{}", context.getJobParams());
        return new ProcessResult(true);
    }

    @Override
    public ProcessResult process(TaskContext taskContext) throws Exception { //核心逻辑，会广播给所有节点并行处理
        log.info("广播核心逻辑触发！参数：{}", taskContext.getJobParams());
        return new ProcessResult(true);
    }

    @Override
    public ProcessResult postProcess(TaskContext context, List<TaskResult> taskResults) throws Exception { //在所有节点广播执行完成后执行，只会在一台机器执行一次
        //通知执行结果，有点类似下面要测试的MapReduce的reduce方法
        log.info("广播任务执行完毕，reduce触发！TaskContext: {}，List<TaskResult>: {}",
                 JSONObject.toJSONString(context), JSONObject.toJSONString(taskResults));
        return new ProcessResult(true, "success");
    }
}
```
然后在powerjob设置该任务：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961776103-79536555-9e7d-4e42-b681-69be8bd90609.png#averageHue=%23fef5f4&clientId=u0feaca88-81ba-4&from=paste&id=ue1f90a03&originHeight=118&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4278799f-330b-4c71-9fa8-806654c1884&title=)<br />运行结果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961776220-a447826b-2ca6-4de5-a5db-63e0ab7df0b8.png#averageHue=%23d4cd66&clientId=u0feaca88-81ba-4&from=paste&id=ud9fcb288&originHeight=253&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5e039369-1ba3-42c9-ae9e-89fd89f08f6&title=)<br />广播模式执行流程如下（可以跟下方的MapReduce模式坐下对比）：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961776569-f760ccd4-e5b4-47ed-b08f-5152d1bdb84a.png#averageHue=%23151313&clientId=u0feaca88-81ba-4&from=paste&id=u53ad2c7a&originHeight=321&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u424a25d1-92a8-459b-9eaa-38ae416b68d&title=)
<a name="VHmUD"></a>
### 3.3、任务类型-Map（大任务拆分）
map就是一次大的任务可以被拆分成细碎的小批次任务进行分布式执行，测试用例代码如下：
```java
@Slf4j
@Component
public class MapProcessorDemo extends MapProcessor { //继承MapProcessor

    private static final int batchSize = 100; //单批发送数据量
    private static final int batchNum = 2; //一共2批，默认上限为200批，再多就要适当调整batchSize大小了

    @Override
    public ProcessResult process(TaskContext context) throws Exception {

        if (isRootTask()) { //如果是根任务（说明map刚被调度到），则触发任务拆分
            log.info("根任务，需要做任务拆分~");
            List<SubTask> subTasks = Lists.newLinkedList();
            for (int j = 0; j < batchNum; j++) {
                SubTask subTask = new SubTask();
                subTask.siteId = j;
                subTask.itemIds = Lists.newLinkedList();
                subTasks.add(subTask); //批次入集合
                for (int i = 0; i < batchSize; i++) { //内部id集合，这里只是举例，实际业务场景可以是从db里获取的业务id集合
                    subTask.itemIds.add(i);
                }
            }
            return map(subTasks, "MAP_TEST_TASK"); //最后调用map，触发这些批次任务的执行
        } else { //子任务，说明批次已做过拆分，此时被调度到时会触发下方逻辑
            SubTask subTask = (SubTask) context.getSubTask(); //直接从上下文对象里拿到批次对象
            log.info("子任务，拿到的批次实体为：{}", JSON.toJSONString(subTask));
            return new ProcessResult(true, "RESULT:true");
        }
    }

    @Getter
    @NoArgsConstructor
    @AllArgsConstructor
    private static class SubTask { //定义批次实体（业务方可自由发挥）
        private Integer siteId; //批次id
        private List<Integer> itemIds; //批次内部所携带的id（可以是自己的业务id）
    }
}
```
代码意义注释已给出，发布完成后可在powerjob平台配置，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961776723-38194dc9-18da-48c8-bb40-d5f4edf67102.png#averageHue=%23fdf7f6&clientId=u0feaca88-81ba-4&from=paste&id=u30cabefe&originHeight=130&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc755c2ba-e7db-4ecc-937f-544ea7b7182&title=)<br />然后看下运行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961777032-b44c8505-aefb-43f8-b2d7-64b7b51cfdbc.png#averageHue=%23d0cc6b&clientId=u0feaca88-81ba-4&from=paste&id=ue06274b7&originHeight=64&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u865d32f1-f757-449e-9a6c-55d2ee48480&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961777160-09b38527-0d94-4087-8a79-5539f26a5eec.png#averageHue=%23dcd981&clientId=u0feaca88-81ba-4&from=paste&id=ue774aa16&originHeight=173&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u87bbb302-f165-4f82-bfcb-b71a38a68a9&title=)<br />上面就是一次map任务触发的演示过程（注：被拆分的map子任务只要有一个失败，即认为整个map任务为失败状态，但不具备事务性）。<br />Map任务执行流程如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961777467-5e2cf154-a357-4b1a-9552-953615e83fae.png#averageHue=%231a1919&clientId=u0feaca88-81ba-4&from=paste&id=u071c73de&originHeight=248&originWidth=500&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1aafdb25-c1ba-434c-b844-b45e0587634&title=)
<a name="dBkgP"></a>
### 3.4、任务类型-MapReduce（大任务拆分与归并）
相比普通map，MapReduce在子任务执行完毕后可以知道它们的执行结果，并做出接下来的自定义逻辑处理，测试用例代码如下：
```java
@Slf4j
@Component
public class MapReduceProcessorDemo extends MapReduceProcessor { //需要继承MapReduceProcessor

    private static final int batchSize = 100;
    private static final int batchNum = 2;

    @Override
    public ProcessResult process(TaskContext context) { //该方法跟普通map方法实现一致，主要用来拆分子任务和执行子任务

        if (isRootTask()) {
            log.info("根任务，需要做任务拆分~");
            List<SubTask> subTasks = Lists.newLinkedList();
            for (int j = 0; j < batchNum; j++) {
                SubTask subTask = new SubTask();
                subTask.siteId = j;
                subTask.itemIds = Lists.newLinkedList();
                subTasks.add(subTask); //批次入集合
                for (int i = 0; i < batchSize; i++) {
                    subTask.itemIds.add(i);
                }
            }
            return map(subTasks, "MAP_TEST_TASK");
        } else {
            SubTask subTask = (SubTask) context.getSubTask();
            log.info("子任务，拿到的批次实体为：{}", JSON.toJSONString(subTask));
            return new ProcessResult(true, "RESULT:true");
        }
    }

    @Override
    public ProcessResult reduce(TaskContext context, List<TaskResult> taskResults) { //相比普通map任务，多出reduce方法，这里将两个参数全部打印出来
        log.info("子任务执行完毕，reduce触发！TaskContext: {}，List<TaskResult>: {}",
                 JSONObject.toJSONString(context), JSONObject.toJSONString(taskResults));
        return new ProcessResult(true, "RESULT:true");
    }

    @Getter
    @NoArgsConstructor
    @AllArgsConstructor
    private static class SubTask {
        private Integer siteId;
        private List<Integer> itemIds;
    }
}
```
代码意义注释已给出，发布完成后可在powerjob平台配置，如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961777773-6cfed722-f503-4176-a9fe-cfd2f6ce956d.png#averageHue=%23faf2f0&clientId=u0feaca88-81ba-4&from=paste&id=u8767c1e3&originHeight=107&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u33d3bfe7-95bd-46a7-b662-6ed7e149d21&title=)<br />现在看下运行结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961777933-2da9d50a-2c54-4c77-b290-232a3d93dcb5.png#averageHue=%23cbc85b&clientId=u0feaca88-81ba-4&from=paste&id=uc75b2199&originHeight=57&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud656f6ef-b669-4b8e-b4ca-af0be170af8&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961778219-7a8fccd6-f882-4348-b0ba-b7fa9ba1447d.png#averageHue=%23e2db95&clientId=u0feaca88-81ba-4&from=paste&id=u27f579e1&originHeight=251&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3aa6f4a4-edb0-4a71-9f2c-7d94325ec29&title=)<br />MapReduce的运行流程如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961778289-fac53659-8a13-4c4a-a407-b35130c93869.png#averageHue=%23141313&clientId=u0feaca88-81ba-4&from=paste&id=u5c529c9f&originHeight=319&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubfa3b6c9-bfdb-4a81-8655-c23bfc030c3&title=)
<a name="oHYlg"></a>
### 3.5、工作流
工作量，顾名思义，遵循任务A → 任务B → 任务C这个流程，只需要在表单里选中下方选项即可将任务本身设置成一个工作流任务：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961778289-45a88684-574c-40d9-943c-fd434774ba98.png#averageHue=%23fcece9&clientId=u0feaca88-81ba-4&from=paste&id=u07da0d3c&originHeight=78&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc659b944-9f33-4bb5-a4c3-7d720474e10&title=)<br />需要注意的是，工作流有自己的调度触发器，因此后面框框即便填了CRON表达式，也不会生效。<br />现在将前面实验中的所有任务都设置成工作流模式：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961778854-96f250f4-3cd0-431c-a689-1555f8418798.png#averageHue=%23fefbfa&clientId=u0feaca88-81ba-4&from=paste&id=ucd7955c0&originHeight=140&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u47c7b299-a603-40a9-8c74-1164debfd47&title=)<br />现在去工作流编辑里编辑工作流触发顺序：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961778771-85e80c81-a9de-420c-9356-6bcb6a6836ce.png#averageHue=%23fcf8f8&clientId=u0feaca88-81ba-4&from=paste&id=u23b91b59&originHeight=431&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2645bc5f-d8df-4b51-8d33-6abd8378942&title=)<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779275-e8425db4-f19e-426f-9d20-6d40074eca89.png#averageHue=%23fdf7f5&clientId=u0feaca88-81ba-4&from=paste&id=u3d8a6b51&originHeight=255&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud60afcdd-21bb-45f5-9b6b-eeb78d63d52&title=)<br />任务按照编排好的顺序，执行了下来。<br />现在把工作流改成下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779244-4a5ba57e-91a4-4bc8-979c-84fc9d4d7b3c.png#averageHue=%23f9f9f9&clientId=u0feaca88-81ba-4&from=paste&id=u82a50d51&originHeight=314&originWidth=598&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ub65eceb9-e52a-4d14-8fcc-8a677ed8919&title=)<br />触发顺序就成了下面这样：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779277-a46a2152-f139-4d95-9da7-5070850388fb.png#averageHue=%23fdf7f5&clientId=u0feaca88-81ba-4&from=paste&id=ue784aa65&originHeight=147&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ubf70c529-4ed9-4832-93f8-5be2bff18ef&title=)
<a name="FaxcQ"></a>
## 四、定时类型&验证
<a name="ofJoW"></a>
### 4.1、CRON表达式
前面的例子均通过该方式触发，支持一般CRON表达式，但是不支持秒级任务（即便配置了每秒执行一次，实际执行却是15s一次，秒级任务可以通过固定频率或固定延迟来做~），由CRON表达式触发的定时任务，在任务本身超时的情况下，仍然保持对应频率执行，比如，让某个简单定时任务每1min执行一次，但实际运行的业务逻辑调成2min，系统调度频率如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779646-fa81e19b-2535-450b-bb1d-1f0e857293c3.png#averageHue=%23fefbfa&clientId=u0feaca88-81ba-4&from=paste&id=ue407a640&originHeight=138&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2b79f5d9-cc1c-4357-9891-27ae55844d7&title=)<br />可以看到，即便是任务需要花费很长时间，任务也是按照每一分钟一次的频率调度，但接下来介绍的延时任务就不一样了。
<a name="OIVjS"></a>
### 4.2、固定频率
如果需要让某个任务按照固定某个频率执行，可以尝试使用固定频率来做：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779755-2e9f3e8d-1c11-40bb-bf9a-ca5fc91b2559.png#averageHue=%23fceeec&clientId=u0feaca88-81ba-4&from=paste&id=u4f04e0eb&originHeight=111&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4863fccc-3862-4df2-ba15-d47065241af&title=)<br />来看下它的调度结果：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961779890-24e071c5-be64-46ea-b6c9-0238f565f83f.png#averageHue=%23fbf7f0&clientId=u0feaca88-81ba-4&from=paste&id=u553cd0bf&originHeight=438&originWidth=461&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ueba687be-7d8f-46a2-99a1-c35f0f885d8&title=)
<a name="XdJ45"></a>
### 4.3、固定延迟
如果需要让某个任务按照固定某个频率延迟执行，沿用4.1的例子，现在配置成延迟任务：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961780061-d125bef7-a2da-442c-8188-fc978a24d5be.png#averageHue=%23fcf0ee&clientId=u0feaca88-81ba-4&from=paste&id=u2f791dfd&originHeight=96&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uca488668-7c81-42a0-9b80-e5f4209f2e9&title=)<br />它的调度结果如下：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1701961780072-0104b0ae-5f92-4f1e-8cef-b794077233f1.png#averageHue=%23cac756&clientId=u0feaca88-81ba-4&from=paste&id=u8b89f69e&originHeight=357&originWidth=900&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ufc03d440-6af7-4e99-b382-b32e2f703b7&title=)<br />可以看到，现在是2min调度一次，相比CRON和固定频率，这个调度是串行化的，后续的任务需要前面的任务执行完才可以执行。
<a name="V75u9"></a>
## 五、其他
<a name="KftLp"></a>
### 5.1、任务表单
如果对创建任务时表单的每一项不是很了解，请参考官方文档：[https://www.yuque.com/powerjob/guidence/nyio9g#v8uF4](https://www.yuque.com/powerjob/guidence/nyio9g#v8uF4)
<a name="HjTAr"></a>
### 5.2、如何配置工作流？
参考文档：[https://www.yuque.com/powerjob/guidence/ysug77#xgylz](https://www.yuque.com/powerjob/guidence/ysug77#xgylz)（不太好用，用的时候需要注意）
