Java Arthas 火焰图<br />官方文档：[https://alibaba.github.io/arthas/profiler.html](https://alibaba.github.io/arthas/profiler.html)<br />`profiler` 命令支持生成应用热点的火焰图。本质上是通过不断的采样，然后把收集到的采样结果生成火焰图。<br />`profiler` 命令基本运行结构是 `profiler action [actionArg]`<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650503039-752ce9c4-8920-4e41-9a13-18b3e8a4f142.png#align=left&display=inline&height=583&originHeight=1750&originWidth=3323&size=2065652&status=done&style=none&width=1107.6666666666667)
<a name="5sIwK"></a>
### 启动profiler
```bash
[arthas@12650]$ profiler start
Started [cpu] profiling
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650529753-fd3bd61e-a0e0-4a8c-a607-3537ccd7efae.png#align=left&display=inline&height=72&originHeight=215&originWidth=3323&size=175263&status=done&style=none&width=1107.6666666666667)
> 默认情况下，生成的是cpu的火焰图，即event为`cpu`。可以用`--event`参数来指定。

<a name="QURQq"></a>
### 获取已采集的sample的数量
```bash
[arthas@12650]$ profiler getSamples
11
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650577334-77f86e66-5c33-4479-8f15-ef0f2cbb7629.png#align=left&display=inline&height=71&originHeight=213&originWidth=3323&size=169653&status=done&style=none&width=1107.6666666666667)
<a name="LQmFM"></a>
### 查看profiler状态
```bash
[arthas@12650]$ profiler status
[perf] profiling is running for 20 seconds
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650616373-e89cf13a-e3fd-4e06-968f-c801efd6b0c0.png#align=left&display=inline&height=73&originHeight=219&originWidth=3323&size=184236&status=done&style=none&width=1107.6666666666667)<br />可以查看当前profiler在采样哪种`event`和采样时间。
<a name="U5z4Q"></a>
### 停止profiler
```bash
[arthas@12650]$ profiler stop
OK
profiler output file: /opt/workspace/finchain/arthas-output/20200725-121242.svg
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650643014-7d3cb43a-06d3-41a0-aa62-8f2462ae4d4f.png#align=left&display=inline&height=91&originHeight=272&originWidth=3323&size=249021&status=done&style=none&width=1107.6666666666667)
<a name="QxUJw"></a>
### 生成svg格式结果
<a name="oyw4f"></a>
#### 指定输出目录
默认情况下，生成的结果保存到应用的`工作目录`下的`arthas-output`目录。可以通过 `--file`参数来指定输出结果路径。比如：
```bash
[arthas@12650]$ profiler stop --file /local/output.svg
OK
profiler output file: /local/output.svg
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650727344-c9b5d8e3-6c78-4e3a-84f0-44b7ecc4b4b1.png#align=left&display=inline&height=90&originHeight=271&originWidth=3323&size=246043&status=done&style=none&width=1107.6666666666667)
<a name="E5JIr"></a>
#### 生成html格式结果
默认情况下，结果文件是`svg`格式，如果想生成`html`格式，可以用`--format`参数指定：
```bash
[arthas@12650]$ profiler stop --format html
OK
profiler output file: /opt/workspace/finchain/arthas-output/20200725-121312.html
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650769433-7328296e-e379-41ff-bb27-87bf784e29a3.png#align=left&display=inline&height=90&originHeight=271&originWidth=3323&size=251923&status=done&style=none&width=1107.6666666666667)<br />或者在`--file`参数里用文件名指名格式。比如`--file /tmp/result.html` 。
<a name="HZe48"></a>
### 通过浏览器查看arthas-output下面的profiler结果
默认情况下，arthas使用3658端口，则可以打开： [http://localhost:3658/arthas-output/](http://localhost:3658/arthas-output/) 查看到`arthas-output`目录下面的profiler结果：<br />![](https://cdn.nlark.com/yuque/0/2020/jpeg/396745/1595085139002-36eb69ca-342d-4f88-b889-919df89205c3.jpeg#align=left&display=inline&height=726&originHeight=726&originWidth=1406&size=0&status=done&style=shadow&width=1406)<br />点击可以查看具体的结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596006882478-86a83e0a-0bd2-41af-8185-f75ba361af50.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3000&size=971745&status=done&style=none&width=1000)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595657909228-a3b51e83-01b2-480a-9e7f-52095af47911.png#align=left&display=inline&height=685&originHeight=2055&originWidth=3840&size=993193&status=done&style=shadow&width=1280)
> 如果是chrome浏览器，可能需要多次刷新。

<a name="nyaKW"></a>
### profiler支持的events
在不同的平台，不同的OS下面，支持的events各有不同。比如在macos下面：
```bash
$ profiler list
Basic events:
  cpu
  alloc
  lock
  wall
  itimer
```
在linux下面
```bash
$ profiler list
Basic events:  
  cpu  
  alloc  
  lock  
  wall  
  itimer
Perf events:  
  page-faults  
  context-switches  
  cycles  
  instructions  
  cache-references  
  cache-misses  
  branches  
  branch-misses  
  bus-cycles  
  L1-dcache-load-misses  
  LLC-load-misses  
  dTLB-load-misses  
  mem:breakpoint  
  trace:tracepoint
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595650795929-cb1ad5d7-615a-4b4c-bd7b-91d65ba90a57.png#align=left&display=inline&height=468&originHeight=1403&originWidth=3323&size=1593888&status=done&style=none&width=1107.6666666666667)<br />如果遇到OS本身的权限/配置问题，然后缺少部分event，可以参考`async-profiler`本身文档：[async-profiler](https://github.com/jvm-profiling-tools/async-profiler)<br />可以用`--event`参数指定要采样的事件，比如对`alloc`事件进入采样：<br />$ profiler start --event alloc
<a name="uWzC1"></a>
### 恢复采样
```bash
$ profiler resume
Started [cpu] profiling
```
`start`和`resume`的区别是：`start`是新开始采样，`resume`会保留上次`stop`时的数据。<br />通过执行`profiler getSamples`可以查看samples的数量来验证。
<a name="mYs3O"></a>
### 使用`execute`来执行复杂的命令
比如开始采样：
```bash
profiler execute 'start'
```
停止采样，并保存到指定文件里：
```bash
profiler execute 'stop,file=/tmp/result.svg'
```
具体的格式参考： [arguments.cpp#L34](https://github.com/jvm-profiling-tools/async-profiler/blob/v1.6/src/arguments.cpp#L34)
<a name="emUgf"></a>
### 查看所有支持的action
```bash
$ profiler actions
Supported Actions: [resume, dumpCollapsed, getSamples, start, list, execute, version, stop, load, dumpFlat, actions, dumpTraces, status]
```
<a name="raTKV"></a>
### 查看版本
```bash
$ profiler version
Async-profiler 1.6 built on Sep  9 2019
Copyright 2019 Andrei Pangin
```

