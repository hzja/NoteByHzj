Java HashMap
<a name="XzTRY"></a>
## **HashMap的数据结构**
HashMap是数组+链表+红黑树结构，这是在JDK1.8的时候设计的，为什么要这样设计呢？来分析下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648444221057-d8a84bfe-88b7-4a68-b811-0347839f8b5a.png#clientId=u1cc88323-ef78-4&from=paste&id=u6bb824ec&originHeight=730&originWidth=1031&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf2084333-71ee-4a79-a7c6-d1ffdd68c3f&title=)
<a name="Kipo6"></a>
## **HashMap红黑树分析**
在jdk1.8就是引入了红黑树这一设计，当hash表的单一个链表长度超过了8个的时候，链表的结构就会变成红黑树结构。可以设计就是可以避免在很极端的情况下，链表的长度是很长很长的，结果在查询的时候就会变得很慢了。<br />**红黑树查询：** 它的性能跟折半查找差不多类似，时间复杂度O（logn）；<br />**链表查询：** 需要遍历全部元素，时间复杂度O（n）；<br />可以说红黑树是一种类似平衡的二叉查找树，优点就是**平衡**，就是左右子树它们的高度近乎是一致的，这样来防止退化成为链表。
<a name="yaPiI"></a>
## **红黑树的特征**
（1）每个节点只能是红色或者黑色<br />（2）根节点必须是黑色<br />（3）红色的节点，它的叶节点只能是黑色<br />（4）从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点
<a name="CMF8G"></a>
## **重要方法之一：hash**
hash这个方法的最主要目的，就是为了提高存储<key,value>的数组的下标位置随机和分布均匀性，尽可能的不让出现hash冲突，看看源码。
```java
/**
* 分析1：hash(key)
* 作用：计算传入数据的哈希码（哈希值、Hash值）
* 该函数在JDK 1.7 和 1.8 中的实现不同，但原理一样 = 扰动函数 = 使得根据key生成的哈希码（hash值）分布更加均匀、更具备随机性，避免出现hash值冲突（即指不同key但生成同1个hash值）
* JDK 1.7 做了9次扰动处理 = 4次位运算 + 5次异或运算
* JDK 1.8 简化了扰动函数 = 只做了2次扰动 = 1次位运算 + 1次异或运算
*/

// JDK 1.7实现：将 键key 转换成 哈希码（hash值）操作  = 使用hashCode() + 4次位运算 + 5次异或运算（9次扰动）
static final int hash(int h) {
	h ^= k.hashCode(); 
	h ^= (h >>> 20) ^ (h >>> 12);
	return h ^ (h >>> 7) ^ (h >>> 4);
}

// JDK 1.8实现：将 键key 转换成 哈希码（hash值）操作 = 使用hashCode() + 1次位运算 + 1次异或运算（2次扰动）
// 1. 取hashCode值：h = key.hashCode() 
// 2. 高位参与低位的运算：h ^ (h >>> 16)  
static final int hash(Object key) {
	int h;
	return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
	// a. 当key = null时，hash值 = 0，所以HashMap的key 可为null      
	// 注：对比HashTable，HashTable对key直接hashCode（），若key为null时，会抛出异常，所以HashTable的key不可为null
	// b. 当key ≠ null时，则通过先计算出 key的 hashCode()（记为h），然后 对哈希码进行 扰动处理： 按位 异或（^） 哈希码自身右移16位后的二进制
}

/**
* 计算存储位置的函数分析：indexFor(hash, table.length)
* 注：该函数仅存在于JDK 1.7 ，JDK 1.8中实际上无该函数（直接用1条语句判断写出），但原理相同
* 为了方便讲解，故提前到此讲解
*/
static int indexFor(int h, int length) {  
	return h & (length-1); 
	// 将对哈希码扰动处理后的结果 与运算(&) （数组长度-1），最终得到存储在数组table的位置（即数组下标、索引）
}
```
**那为什么不用hashcode来处理出来的哈希码来作为下标位置呢**，原因就是会出现哈希码与数组的大小范围不匹配的这种情况，就无法匹配到存储的位置了。<br />**那为什么采用哈希码与运算（数组长度-1）计算下标？**<br />因为根据容量大小，按照需要取哈希码一定数量的地位作为数组的下标位置，来解决哈希码跟数组的大小范围不匹配的问题。
<a name="zlsn0"></a>
## **重要方法之二：put**
先看put的流程图，再看源码<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648444221043-f24ec53f-fee6-4670-b8f5-84419c9efee1.png#clientId=u1cc88323-ef78-4&from=paste&id=u9a2f3d88&originHeight=863&originWidth=806&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8d6c3532-bed0-4aee-a95d-af289f995a0&title=)
```java
/**
* 分析2：putVal(hash(key), key, value, false, true)
*/
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
			   boolean evict) {
	
	Node<K,V>[] tab; Node<K,V> p; int n, i;
	
	// 1. 若哈希表的数组tab为空，则 通过resize() 创建
	// 所以，初始化哈希表的时机 = 第1次调用put函数时，即调用resize() 初始化创建
	// 关于resize（）的源码分析将在下面讲解扩容时详细分析，此处先跳过
	if ((tab = table) == null || (n = tab.length) == 0)
		n = (tab = resize()).length;
	
	// 2. 计算插入存储的数组索引i：根据键值key计算的hash值 得到
	// 此处的数组下标计算方式 = i = (n - 1) & hash，同JDK 1.7中的indexFor（），上面已详细描述
	
	// 3. 插入时，需判断是否存在Hash冲突：
	// 若不存在（即当前table[i] == null），则直接在该数组位置新建节点，插入完毕
	// 否则，代表存在Hash冲突，即当前存储位置已存在节点，则依次往下判断：a. 当前位置的key是否与需插入的key相同、b. 判断需插入的数据结构是否为红黑树 or 链表
	if ((p = tab[i = (n - 1) & hash]) == null)
		tab[i] = newNode(hash, key, value, null);  // newNode(hash, key, value, null)的源码 = new Node<>(hash, key, value, next)
	
	else {
		Node<K,V> e; K k;
		
		// a. 判断 table[i]的元素的key是否与 需插入的key一样，若相同则 直接用新value 覆盖 旧value
		// 判断原则：equals（）
		if (p.hash == hash &&
			((k = p.key) == key || (key != null && key.equals(k))))
			e = p;
		
		// b. 继续判断：需插入的数据结构是否为红黑树 or 链表
		// 若是红黑树，则直接在树中插入 or 更新键值对
		else if (p instanceof TreeNode)
			e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value); ->>分析3
			
			// 若是链表,则在链表中插入 or 更新键值对
			// i.  遍历table[i]，判断Key是否已存在：采用equals（） 对比当前遍历节点的key 与 需插入数据的key：若已存在，则直接用新value 覆盖 旧value
			// ii. 遍历完毕后仍无发现上述情况，则直接在链表尾部插入数据
			// 注：新增节点后，需判断链表长度是否>8（8 = 桶的树化阈值）：若是，则把链表转换为红黑树
			
			else {
				for (int binCount = 0; ; ++binCount) {
					// 对于ii：若数组的下1个位置，表示已到表尾也没有找到key值相同节点，则新建节点 = 插入节点
					// 注：此处是从链表尾插入，与JDK 1.7不同（从链表头插入，即永远都是添加到数组的位置，原来数组位置的数据则往后移）
					if ((e = p.next) == null) {
						p.next = newNode(hash, key, value, null);
						
						// 插入节点后，若链表节点>数阈值，则将链表转换为红黑树
						if (binCount >= TREEIFY_THRESHOLD - 1) 
							treeifyBin(tab, hash); // 树化操作
						break;
					}
					
					// 对于i
					if (e.hash == hash &&
						((k = e.key) == key || (key != null && key.equals(k))))
						break;
					
					// 更新p指向下一个节点，继续遍历
					p = e;
				}
			}
		
		// 对i情况的后续操作：发现key已存在，直接用新value 覆盖 旧value & 返回旧value
		if (e != null) { 
			V oldValue = e.value;
			if (!onlyIfAbsent || oldValue == null)
				e.value = value;
			afterNodeAccess(e); // 替换旧值时会调用的方法（默认实现为空）
			return oldValue;
		}
	}
	
	++modCount;
	
	// 插入成功后，判断实际存在的键值对数量size > 最大容量threshold
	// 若 > ，则进行扩容 ->>分析4（但单独讲解，请直接跳出该代码块）
	if (++size > threshold)
		resize();
	
	afterNodeInsertion(evict);// 插入成功时会调用的方法（默认实现为空）
	return null;
	
}
```
<a name="WxaF1"></a>
## **重要方法之三：risize扩容机制**
在resize的方法里，定义了一个叫oldCap的参数，它是记录了原来的table长度，接着又定义了newCap参数来记录最新的table长度，注意newCap它是oldCap长度的两倍。然后循环原来的table，再把原来table的元素放到新的table里。具体看流程图+源码<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648444221086-b1340108-3075-4026-8485-6b3ec782ba1c.png#clientId=u1cc88323-ef78-4&from=paste&id=u96121964&originHeight=843&originWidth=877&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ube3b6807-40e9-4f00-b686-39414bbc149&title=)
```java
/**
* 分析4：resize（）
* 该函数有2种使用情况：1.初始化哈希表 2.当前数组容量过小，需扩容
*/
final Node<K,V>[] resize() {
	Node<K,V>[] oldTab = table; // 扩容前的数组（当前数组）
	int oldCap = (oldTab == null) ? 0 : oldTab.length; // 扩容前的数组的容量 = 长度
	int oldThr = threshold;// 扩容前的数组的阈值
	int newCap, newThr = 0;
	
	// 针对情况2：若扩容前的数组容量超过最大值，则不再扩充
	if (oldCap > 0) {
		if (oldCap >= MAXIMUM_CAPACITY) {
			threshold = Integer.MAX_VALUE;
			return oldTab;
		}
		
		// 针对情况2：若无超过最大值，就扩充为原来的2倍
		else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
				 oldCap >= DEFAULT_INITIAL_CAPACITY)
			newThr = oldThr << 1; // 通过右移扩充2倍
	}
	
	// 针对情况1：初始化哈希表（采用指定 or 默认值）
	else if (oldThr > 0) // initial capacity was placed in threshold
		newCap = oldThr;
	
	else {               // zero initial threshold signifies using defaults
		newCap = DEFAULT_INITIAL_CAPACITY;
		newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
	}
	
	// 计算新的resize上限
	if (newThr == 0) {
		float ft = (float)newCap * loadFactor;
		newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
				  (int)ft : Integer.MAX_VALUE);
	}
	
	threshold = newThr;
	@SuppressWarnings({"rawtypes","unchecked"})
	Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap];
	table = newTab;
	
	if (oldTab != null) {
		// 把每个bucket都移动到新的buckets中
		for (int j = 0; j < oldCap; ++j) {
			Node<K,V> e;
			if ((e = oldTab[j]) != null) {
				oldTab[j] = null;
				
				if (e.next == null)
					newTab[e.hash & (newCap - 1)] = e;
				else if (e instanceof TreeNode)
					((TreeNode<K,V>)e).split(this, newTab, j, oldCap);
				
				else { // 链表优化重hash的代码块
					Node<K,V> loHead = null, loTail = null;
					Node<K,V> hiHead = null, hiTail = null;
					Node<K,V> next;
					do {
						next = e.next;
						// 原索引
						if ((e.hash & oldCap) == 0) {
							if (loTail == null)
								loHead = e;
							else
								loTail.next = e;
							loTail = e;
						}
						// 原索引 + oldCap
						else {
							if (hiTail == null)
								hiHead = e;
							else
								hiTail.next = e;
							hiTail = e;
						}
					} while ((e = next) != null);
					// 原索引放到bucket里
					if (loTail != null) {
						loTail.next = null;
						newTab[j] = loHead;
					}
					// 原索引+oldCap放到bucket里
					if (hiTail != null) {
						hiTail.next = null;
						newTab[j + oldCap] = hiHead;
					}
				}
			}
		}
	}
	return newTab;
}
```
