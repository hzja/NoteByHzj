Java<br />对象创建的几种方法：

1. 使用`new`关键字
2. 使用`clone`方法
3. 反射机制
4. 反序列化

以上四种都可以产生java对象

- 1,3都会明确的显式的调用构造函数
- 2是在内存上对已有对象的影印 所以不会调用构造函数
- 4是从文件中还原类的对象 也不会调用构造函数
<a name="dLISz"></a>
## 何为`clone()`？

1. 拷贝对象返回的是一个新的对象，而不是一个对象的引用地址；
2. 拷贝对象已经包含原来对象的信息，而不是对象的初始信息，即每次拷贝动作不是针对一个全新对象的创建。
<a name="MDMHt"></a>
## `clone()`和`new`那个更快？
利用`clone`，在内存中进行数据块的拷贝，复制已有的对象，也是生成对象的一种方式。前提是类实现`Cloneable`接口，Cloneable接口没有任何方法，是一个空接口，也可以称这样的接口为标志接口，只有实现了该接口，才会支持`clone`操作。有的人也许会问了，java中的对象都有一个默认的父类Object。<br />Object中有一个`clone`方法，为什么还必须要实现`Cloneable`接口呢，这就是cloneable接口这个标志接口的意义，只有实现了这个接口才能实现复制操作，因为jvm在复制对象的时候，会检查对象的类是否实现了`Cloneable`这个接口，如果没有实现，则会报`CloneNotSupportedException`异常。类似这样的接口还有`Serializable`接口、`RandomAccess`接口等。<br />还有值得一提的是在执行`clone`操作的时候，不会调用构造函数。还有`clone`操作还会面临深拷贝和浅拷贝的问题。关于这方面的问题，网上有很多的相关知识了，不再累述了。由于通过复制操作得到对象不需要调用构造函数，只是内存中的数据块的拷贝，那是不是拷贝对象的效率是不是一定会比`new`的时候的快。<br />答案：不是。显然jvm的开发者也意识到通过`new`方式来生成对象占据了开发者生成对象的绝大部分，所以对于利用`new`操作生成对象进行了优化。<br />例如：
```java
public class Bean implements Cloneable {
	private String name;
	
	public Bean(String name) {
		this.name = name;
	}
	
	@Override
	protected Bean clone() throws CloneNotSupportedException {
		return (Bean) super.clone();
	}
}
```
```java
public class TestClass {
    private static final int COUNT = 10000 * 1000;
 
    public static void main(String[] args) throws CloneNotSupportedException {
 
        long s1 = System.currentTimeMillis();
 
        for (int i = 0; i < COUNT; i++) {
            Bean bean = new Bean("Fcant");
        }
 
        long s2 = System.currentTimeMillis();
 
        Bean bean = new Bean("Fcant");
        for (int i = 0; i < COUNT; i++) {
            Bean b = bean.clone();
        }
 
        long s3 = System.currentTimeMillis();
 
        System.out.println("new  = " + (s2 - s1));
        System.out.println("clone = " + (s3 - s2));
    }
}
```
打印结果：<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652159505077-4d36356e-c9cc-4103-9200-fd4775ea7605.png#clientId=uc5e55764-564d-4&from=paste&height=302&id=ub3f53826&originHeight=754&originWidth=1948&originalType=binary&ratio=1&rotation=0&showTitle=false&size=108123&status=done&style=shadow&taskId=u4d542ea6-7828-4e3c-a40d-ff7f790b748&title=&width=779.2)<br />`new`完胜`clone`，真的是这样吗？<br />下面在构造函数里做点简单的事情，例如字符串截取试试。只是修改Bean，其他不变再看打印
```java
public class Bean implements Cloneable {
    private String name;
    private String firstSign;//获取名字首字母
 
    public Bean(String name) {
        this.name = name;
        if (name.length() != 0) {
            firstSign = name.substring(0, 1);
            firstSign += "abc";
        }
    }
 
    @Override
    protected Bean clone() throws CloneNotSupportedException {
        return (Bean) super.clone();
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1652159637326-c6dd60a0-c570-466c-b5f6-32317341ade4.png#clientId=uc5e55764-564d-4&from=paste&height=287&id=u6fbfa365&originHeight=717&originWidth=1864&originalType=binary&ratio=1&rotation=0&showTitle=false&size=130565&status=done&style=shadow&taskId=uee392d9b-f555-4c40-9470-2506657f0f1&title=&width=745.6)
