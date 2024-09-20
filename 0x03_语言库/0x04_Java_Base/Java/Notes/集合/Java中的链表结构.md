Java链表<br />一、前言<br />二、链表数据结构<br />三、链表分类类型

- 1. 单向链表
- 2. 双向链表
- 3. 循环链表

四、实现一个链表

- 1. 链表节点
- 2. 头插节点
- 3. 尾插节点
- 4. 拆链操作
- 5. 删除节点

五、链表使用测试<br />六、常见面试问题
<a name="gGv5m"></a>
## 一、前言
链表的历史<br />于1955-1956年，由兰德公司的Allen Newell、Cliff Shaw和Herbert A. Simon开发了链表，作为他们的信息处理语言的主要数据结构。链表的另一个早期出现是由 Hans Peter Luhn 在 1953 年 1 月编写的IBM内部备忘录建议在链式哈希表中使用链表。<br />到 1960 年代初，链表和使用这些结构作为主要数据表示的语言的实用性已经很好地建立起来。MIT 林肯实验室的 Bert Green于 1961 年 3 月在 IRE Transactions on Human Factors in Electronics 上发表了一篇题为“用于符号操作的计算机语言”的评论文章，总结了链表方法的优点。1964 年 4 月，Bobrow 和 Raphael 的一篇评论文章“列表处理计算机语言的比较”发表在 ACM 通讯中。
<a name="H8MY6"></a>
## 二、链表数据结构
在计算机科学中，链表是数据元素的线性集合，元素的线性顺序不是由它们在内存中的物理地址给出的。它是由一组节点组成的数据结构，每个元素指向下一个元素，这些节点一起，表示线性序列。<br />在最简单的链表结构下，每个节点由数据和指针（存放指向下一个节点的指针）两部分组成，这种数据结构允许在迭代时有效地从序列中的任何位置插入或删除元素。<br />链表的数据结构通过链的连接方式，提供了可以不需要扩容空间就更高效的插入和删除元素的操作，在适合的场景下它是一种非常方便的数据结构。但在一些需要遍历、指定位置操作、或者访问任意元素下，是需要循环遍历的，这将导致时间复杂度的提升。
<a name="fpBLd"></a>
## 三、链表分类类型
链表的主要表现形式分为；单向链表、双向链表、循环链表，接下来分别介绍下。
<a name="DKUEk"></a>
### 1、单向链表
单链表包含具有数据字段的节点以及指向节点行中的下一个节点的“下一个”字段。可以对单链表执行的操作包括插入、删除和遍历。
<a name="QpYaa"></a>
### 2、双向链表
在“双向链表”中，除了下一个节点链接之外，每个节点还包含指向序列中“前一个”节点的第二个链接字段。这两个链接可以称为'forward（'s'）和'backwards'，或'next'和'prev'（'previous'）。
<a name="e31Rk"></a>
### 3、循环链表
在列表的最后一个节点中，链接字段通常包含一个空引用，一个特殊的值用于指示缺少进一步的节点。一个不太常见的约定是让它指向列表的第一个节点。在这种情况下，列表被称为“循环”或“循环链接”；否则，它被称为“开放”或“线性”。它是一个列表，其中最后一个指针指向第一个节点。
<a name="h5t3m"></a>
## 四、实现一个链表
像 Java 的源码中本身就提供了非常多的数据结构，包括所学习的链表 LinkedList 在日常的开发使用中也是非常频繁。<br />所以在学习的过程中，以使用 Java 程序员本身常用的语言来分析学习，并通过简化结构的方式把 LinkedList 手写实现，让读者更能方便的理解链表。

- **源码地址：**[https://github.com/fuzhengwei/java-algorithms](https://github.com/fuzhengwei/java-algorithms)** - Java 算法与数据结构**
- **源码详见：**[https://github.com/fuzhengwei/java-algorithms/data-structures/LinkedList.java](https://github.com/fuzhengwei/java-algorithms/data-structures/LinkedList.java)
<a name="ikto4"></a>
### 1、链表节点
```java
private static class Node<E> {
    E item;
    Node<E> next;
    Node<E> prev;
    
    public Node(Node<E> prev, E element, Node<E> next) {
        this.item = element;
        this.next = next;
        this.prev = prev;
    }
}
```

- **链表的数据结构核心根基就在于节点对象的使用，并在节点对象中关联当前节点的上一个和下一个节点。通过这样的方式构建出链表结构。**
- **但也因为在链表上添加每个元素的时候，都需要创建新的 Node 节点，所以这也是一部分耗时的操作。**
<a name="re5xu"></a>
### 2、头插节点
```java
void linkFirst(E e) {
    final Node<E> f = first;
    final Node<E> newNode = new Node<>(null, e, f);
    first = newNode;
    if (f == null)
        last = newNode;
    else
        f.prev = newNode;
    size++;
}
```

- **头插的操作流程，先把头节点记录下来。之后创建一个新的节点，新的节点构造函数的头节点入参为null，通过这样的方式构建出一个新的头节点。**
- **原来的头结点，设置 **`**f.prev**`** 连接到新的头节点，这样的就可以完成头插的操作了。另外如果原来就没有头节点，设置设置为新的节点即可。最后记录当前链表中节点的数量，也就是使用 LinkedList 获取 size 时候就是从这个值获取的。**
<a name="ILJNP"></a>
### 3、尾插节点
```java
void linkLast(E e) {
    final Node<E> l = last;
    final Node<E> newNode = new Node<>(l, e, null);
    last = newNode;
    if (l == null) {
        first = newNode;
    } else {
        l.next = newNode;
    }
    size++;
}
```

- **尾差节点与头插节点正好相反，通过记录当前的结尾节点，创建新的节点，并把当前的结尾节点，通过 **`**l.next**`** 关联到新创建的节点上。同时记录 size 节点数量值。**
<a name="BRcgo"></a>
### 4、拆链操作
```java
E unlink(Node<E> x) {
    final E element = x.item;
    final Node<E> next = x.next;
    final Node<E> prev = x.prev;
    if (prev == null) {
        first = next;
    } else {
        prev.next = next;
        x.prev = null;
    }
    if (next == null) {
        last = prev;
    } else {
        next.prev = prev;
        x.next = null;
    }
    x.item = null;
    size--;
    return element;
}
```

- `**unlink**`** 是一种拆链操作，只要给定一个元素，它就可以把当前这个元素的上一个节点和一个节点进行相连，之后把自己拆除。**
- **这个方法常用语 **`**remove**`** 移除元素操作，因为整个操作过程不需要遍历，拆除元素后也不需要复制新的空间，所以时间复杂读为 O(1)**
<a name="gNpE6"></a>
### 5、删除节点
```java
public boolean remove(Object o) {
    if (o == null) {
        for (Node<E> x = first; x != null; x = x.next) {
            if (x.item == null) {
                unlink(x);
                return true;
            }
        }
    } else {
        for (Node<E> x = first; x != null; x = x.next) {
            if (o.equals(x.item)) {
                unlink(x);
                return true;
            }
        }
    }
    return false;
}
```

- **删除元素的过程需要 for 循环判断比删除元素的值，找到对应的元素，进行删除。**
- **循环比对的过程是一个 O(n) 的操作，删除的过程是一个 O(1) 的操作。所以如果这个链表较大，删除的元素又都是贴近结尾，那么这个循环比对的过程也是比较耗时的。**
<a name="vpwjw"></a>
## 五、链表使用测试
```java
public static void main(String[] args) {
    List<String> list = new LinkedList<>();
    // 添加元素
    list.add("a");
    list.addFirst("b");
    list.addLast("c");
    // 打印列表
    list.printLinkList();
    // 头插元素
    list.addFirst("d");
    // 删除元素
    list.remove("b");
    // 打印列表
    list.printLinkList();
}
```
测试结果
```java
目前的列表，头节点：b 尾节点：c 整体：b，a，c，
目前的列表，头节点：d 尾节点：c 整体：d，a，c，

Process finished with exit code 0
```

- **按照测试链表对数据的操作过程，从测试结果可以看到，已经满足了链表数据结构的使用。**
<a name="TyovO"></a>
## 六、常见面试问题

- **描述一下链表的数据结构？**
- **Java 中 LinkedList 使用的是单向链表、双向链表还是循环链表？**
- **链表中数据的插入、删除、获取元素，时间复杂度是多少？**
- **什么场景下使用链表更合适？**
