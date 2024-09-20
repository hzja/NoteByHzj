JVM<br />在 JVM 中，有两个非常重要的知识点，一个是 JVM 的内存布局（JVM 运行时的数据区域），另一个就是垃圾回收。而垃圾回收中又有两个重要的知识点，一个是如何确定 JVM 中的垃圾对象，另一个是使用不同的垃圾收集器进行垃圾回收。而本篇要讨论的是前者，后面的内容咱们下一篇再聊。<br />**垃圾（死亡）对象的判定有两种常用的算法：引用计数器算法和可达性分析算法。**
<a name="zRcaF"></a>
## 1、引用计数器算法
**引用计数算法（Reference Counting） 属于垃圾收集器的早期实现算法了，它指的是在创建对象时关联一个与之相对应的计数器，当此对象被使用时加 1，相反销毁时 -1。当此计数器为 0 时，则表示此对象未使用，可以被垃圾收集器回收。**<br />引用计数算法的优缺点很明显，其**优点是垃圾回收比较及时，实时性比较高**，只要对象计数器为 0，则可以直接进行回收操作；而**缺点是无法解决循环引用的问题**，比如以下代码：
```java
public class RefCounterTest {
    // 对象 A
    static class RefObjectA {
        private RefObjectB refObjectB;

        public void setRefObjectB(RefObjectB refObjectB) {
            this.refObjectB = refObjectB;
        }
    }
    // 对象 B
    static class RefObjectB {
        private RefObjectA refObjectA;

        public void setRefObjectA(RefObjectA refObjectA) {
            this.refObjectA = refObjectA;
        }
    }
    // 测试代码
    public static void main(String[] args) {
        RefObjectA objectA = new RefObjectA();
        RefObjectB objectB = new RefObjectB();
        objectA.setRefObjectB(objectB);
        objectB.setRefObjectA(objectA);
        objectA = null;
        objectB = null;
    }
}
```
如以上代码所示，即使是将 main 方法中的 objectA 和 objectB 都设置为 null，也就是这两个对象都彻底不使用了，但是因为二者存在相互引用的关系，所以它们所对应的对象计数器不为 0，这样循环引用导致垃圾数据无法被清除的事件就产生了。
<a name="rpFxZ"></a>
## 2、可达性分析算法
**可达性分析算法（Reachability Analysis） 是目前主流虚拟机中，使用最广泛的判断垃圾对象的实现算法，它指的是从对象的起点（GC Roots）开始向下搜索，如果对象到 GC Roots 没有任何引用链相连时，也就是说此对象到 GC Roots 不可达时，则表示此对象可以被垃圾回收器所回收**，如下图所示：![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676551612743-95131f6c-085b-4d57-8e73-f05f91c270a4.png#averageHue=%23f7f5f5&clientId=u6469c132-920a-4&from=paste&id=ua366a55e&originHeight=611&originWidth=1035&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc9d7d3a2-22d2-45ca-a0ed-bfd3b4bb770&title=)在 Java 语言中，可作为根节点（GC Roots）的对象有以下 4 类：

1. **Java 虚拟机栈中的引用对象**，也就是 Java 虚拟机栈帧中，本地变量表所存储的（引用）对象。在 Java 虚拟机栈帧中存储的对象都是将来执行时，要使用的对象，所以和引用对象相关的对象都不能被回收；
2. **本地方法栈中的引用对象**和 Java 虚拟机栈中的引用对象类似，也不能被回收；
3. **方法区中类静态属性引用的对象**也可以作为 GC Roots；
4. **方法区中常量引用的对象**也可以作为 GC Roots。因为常量是保存在常量池中的，属于全局可使用的对象，所以也能作为 GC Roots。
<a name="aifTQ"></a>
## 3、有关“引用”
不管是引用计数法还是可达性分析算法都与对象的“引用”有关，这说明**对象的引用决定了对象的生死**，而 Java 中的引用也比较复杂，它从 JDK 1.2 之后，（引用）分成了以下 4 种类型：

- **强引用**：在代码中普遍存在的，类似 `Object obj = new Object()` 这类引用，**只要强引用还在，垃圾收集器永远不会回收掉被引用的对象**；
- **软引用**：是一种相对强引用弱化一些的引用，可以让对象豁免一些垃圾收集，**只有当 JVM 认为内存不足时，才会去试图回收软引用指向的对象，JVM 会确保在抛出 OutOfMemoryError 之前，清理软引用指向的对象**；
- **弱引用**：非必需对象，但它的强度比软引用更弱，**被弱引用关联的对象只能生存到下一次垃圾收集发生之前**；
- **虚引用**：也称为幽灵引用或幻影引用，**是最弱的一种引用关系，无法通过虚引用来获取一个对象实例，为对象设置虚引用的目的只有一个，就是当着个对象被收集器回收时收到一条系统通知**。
<a name="hqMqY"></a>
## 总结
垃圾对象的判定有两种常用的算法：引用计数器算法和可达性分析算法。其中引用计数器算法实现简单、运行高效，但是存在循环引用的问题，所以主流的虚拟机使用的都是可达性分析算法，可达性分析算法是从对象的根节点 GC Roots 向下搜索，如果根节点相连就是正常的对象，否则为垃圾对象可以被垃圾回收器回收。
