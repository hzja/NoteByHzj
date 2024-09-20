Java SpringCloudAlibaba Sentinel
<a name="ahLuQ"></a>
## **Sentinel简介**
Sentinel它是面向了分布式服务构的对流量控制的一个组件，主要就是从限流、流量整型、熔断降级等等多个维度来对开发者保证微服务架构的稳定性。<br />Sentinel被设计得很精巧，只需要sentinel-core就可以了，可谓好用又强大！
<a name="fV0e5"></a>
## **Sentinel的基本原理**
Sentinel它是在内部创建一个责任链，责任链的话它是由一系列的`ProcessorSlot`对象来组成的，每个`ProcessorSlot`这个对象负责了不同的功能，然后外部请求是否可以 访问资源，这个时候需要通过责任链校验，只有校验通过了，才可以进行资源访问。如果校验失败就抛出异常。
<a name="ewM6f"></a>
### Sentinel的8个实现类
（1）`DegradeSlot`：用于服务降级，发现服务超时或者是报错的次数超过了限制的话，`DegradeSlot`就将会禁止再次访问该服务，需要等待一段时间后，`DegradeSlot`会试探性的把一个请求放过去，然后再根据这个请求的情况来判断是否要降级。<br />（2）`AuthoritySlot`：黑白名单校验，它是需要按照字符串来进行匹配，如果在黑名单里就是禁止访问的。<br />（3）`ClusterBuilderSlot`：构建一个ClusterNode对象，这个对象主要是用在统计访问的QPS、线程数等等。每一个资源对于一个`ClusterNode`。<br />（4）`SystemSlot`：用来校验QPS、线程数、CPU使用率是否已经超过了限制。<br />（5）`StatisticSlot`：这个用在多个维度来统计响应的时间、并发的线程数，处理成功/失败的数量、<br />（6）`FlowSlot`：用在流量监控，根据QPS或者并发的线程数来进行控制，当发现QPS高过设定值的时候，就会抛出异常，FlowSlot依赖StatisticSlot。<br />（7）`NodeSelectorSlot`：收集资源的路径、把这些资源调用的路径再以树结构存储 起来，根据调用的路径来进行限流。<br />（8）`LogSlot`：打印日志
<a name="mvh83"></a>
## **Slot的执行顺序**
![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1650257073502-e2cf0d27-fdca-4fa0-911c-283f20e9068d.jpeg)
<a name="bCSlJ"></a>
## `**SphU.entry**`
entry这个方法内部会创建一个叫`StringResourceWrapper`的对象，该对象表示被保护的资源。这个对象有三个参数。
```java
//资源名，也就是entry()方法的第一个入参
protected final String name;
//表示是入口流量（IN）还是出口流量（OUT），
//两个参数的区别在于是否被SystemSlot检查，IN会被检查，OUT不会，默认是OUT
protected final EntryType entryType;
//表示资源类型，sentinel提供了common、web、sql、api等类型，资源类型用于统计使用
protected final int resourceType;
```
<a name="FANA9"></a>
### Entry
有了资源的对象，接着就是创建Entry对象，这个对象是SphU的这个返回值。通过Entry对象应用程序就可以知道sentinel里的内部情况了。
```java
//resourceWrapper：是StringResourceWrapper对象，表示资源
//count：表示令牌数，默认是1，一般一个请求对应一个令牌，也可以指定一个请求对应多个令牌，如果令牌不够，则禁止访问
//prioritized：在FlowSlot里面使用，没找到具体的使用含义，有看懂的小伙伴可以告知一下
private Entry entryWithPriority(ResourceWrapper resourceWrapper, int count, boolean prioritized, Object... args)
	throws BlockException {
	//构建上下文对象，上下文对象存储在ThreadLocal中
	Context context = ContextUtil.getContext();
	if (context instanceof NullContext) {
		return new CtEntry(resourceWrapper, null, context);
	}
	//一般的线程第一次访问资源，context都是null，我也可以在应用程序中使用ContextUtil自己创建Context对象
	if (context == null) {
	 //下面创建了一个名字为sentinel_default_context的Context对象
		context = InternalContextUtil.internalEnter(Constants.CONTEXT_DEFAULT_NAME);
	}
//全局开关，可以使用它来关闭sentinel
	if (!Constants.ON) {
		return new CtEntry(resourceWrapper, null, context);
	}
//使用SPI构建slot链，每个slot对象都有一个next属性，可以使用该属性指定下一个slot对象
	ProcessorSlot<Object> chain = lookProcessChain(resourceWrapper);
	if (chain == null) {
		return new CtEntry(resourceWrapper, null, context);
	}
//创建Entry对象
	Entry e = new CtEntry(resourceWrapper, chain, context);
	try {
	 //对该请求，遍历每个slot对象
		chain.entry(context, resourceWrapper, null, count, prioritized, args);
	} catch (BlockException e1) {
		e.exit(count, args);
		throw e1;
	} catch (Throwable e1) {
		RecordLog.info("Sentinel unexpected exception", e1);
	}
	return e;
}
```
源码如上所示，可以看到先Context对象，这个对象会走遍整个请求的过程。一些共享的数据是可以放在这里，也可以 在应用的程序里创建Context对象。创建完Context对象之后，就会使用SPI来构建slot链，之后就是创建Entry对象，最后就是遍历slot链是不是可以请求访问资源。
<a name="syhxG"></a>
## `**Entry.exit()**`
当资源访问完后，就需要开始调用`exit`这个方法来告诉sentinel可以结束访问了，这时候sentinel就会做一些资源的清理跟数据统计的工作。
```java
protected void exitForContext(Context context, int count, Object... args) throws ErrorEntryFreeException {
	if (context != null) {
		if (context instanceof NullContext) {
			return;
		}
//如果Context对象记录的Entry对象不是当前对象，
//意味着entry.exit()与SphU.entry()不是成对出现的，
//sentinel要求两者必须成对出现，而且要一一对应，否则抛出异常
//Context有父子关系，这个在文章后面介绍
		if (context.getCurEntry() != this) {
			String curEntryNameInContext = context.getCurEntry() == null ? null
				: context.getCurEntry().getResourceWrapper().getName();
			// Clean previous call stack.
			CtEntry e = (CtEntry) context.getCurEntry();
			while (e != null) {
				e.exit(count, args);
				e = (CtEntry) e.parent;
			}
			String errorMessage = String.format("The order of entry exit can't be paired with the order of entry"
					+ ", current entry in context: <%s>, but expected: <%s>", curEntryNameInContext,
				resourceWrapper.getName());
			throw new ErrorEntryFreeException(errorMessage);
		} else {
			//在遍历每个slot的exit方法，每个slot清理和统计数据
			if (chain != null) {
				chain.exit(context, resourceWrapper, count, args);
			}
			//遍历exitHandlers，相当于回调，一般的DegradeSlot有回调，
			//DegradeSlot根据服务访问状态，决定是否将降级状态由HALF_OPEN变为OPEN
			callExitHandlersAndCleanUp(context);

			//设置为上一级Context对象
			context.setCurEntry(parent);
			if (parent != null) {
				((CtEntry) parent).child = null;
			}
			if (parent == null) {
				// Default context (auto entered) will be exited automatically.
				if (ContextUtil.isDefaultContext(context)) {
					ContextUtil.exit();
				}
			}
//设置当前对象的this.context = null
			clearEntryContext();
		}
	}
}
```
<a name="MkiKg"></a>
## **Context**
Context顾名思义上下文对象，这个对象是把一整个资源的访问过程都贯穿了，它保存在`ThreadLocal`里。创建Context是有多种方式的。
```java
//name表示Context的名称或者链路入口的名称，origin表示调用来源的名称，默认为空字符串
public static Context enter(String name, String origin);
public static Context enter(String name);
```
不管是哪种方式，到最后面都是要调用`ContextUtil.trueEnter`方法
```java
protected static Context trueEnter(String name, String origin) {
 //contextHolder是ThreadLocal<Context>类型
	Context context = contextHolder.get();
	if (context == null) {
	 //contextNameNodeMap持有系统所有的入口节点
		Map<String, DefaultNode> localCacheNameMap = contextNameNodeMap;
		DefaultNode node = localCacheNameMap.get(name);
		if (node == null) {
		 //sentinel最大只能支撑2000个入口节点，如果超过2000个，sentinel无法提供对资源的保护
			if (localCacheNameMap.size() > Constants.MAX_CONTEXT_NAME_SIZE) {
				setNullContext();
				return NULL_CONTEXT;
			} else {
				LOCK.lock();
				try {
					node = contextNameNodeMap.get(name);
					if (node == null) {
						if (contextNameNodeMap.size() > Constants.MAX_CONTEXT_NAME_SIZE) {
							setNullContext();
							return NULL_CONTEXT;
						} else {
						 //创建入口节点
							node = new EntranceNode(new StringResourceWrapper(name, EntryType.IN), null);
							//入口节点作为虚拟根节点的子节点
							Constants.ROOT.addChild(node);
							Map<String, DefaultNode> newMap = new HashMap<>(contextNameNodeMap.size() + 1);
							newMap.putAll(contextNameNodeMap);
							newMap.put(name, node);
							contextNameNodeMap = newMap;
						}
					}
				} finally {
					LOCK.unlock();
				}
			}
		}
		//创建Context对象，可以看到Context对象与入口节点一一对应
		context = new Context(node, name);
		//设置调用来源
		context.setOrigin(origin);
		contextHolder.set(context);
	}
	return context;
}
```
在Context对象有一个名称与一个入口节点的对象，入口节点它就对于了线程访问的第一个资源，Context对象是对应了线程对资源的访问，一个线程就对应一个Context对象。每一个入口节点的对象它都是虚拟的根对象root的子节点。
```java
//ROOT_ID=machine-root
public final static DefaultNode ROOT = new EntranceNode(new StringResourceWrapper(ROOT_ID, EntryType.IN),
	new ClusterNode(ROOT_ID, ResourceTypeConstants.COMMON));
```
每一次调用SphU.entry方法会在访问的链路上添加一个子节点，经过这个树就可以还原出来资源的访问路径了。每访问一个资源，Context对象都会使用curEntry这个属性记录下正在访问的资源所对应的Entry对象。
