Java
<a name="FZNQs"></a>
## `List<? extends T>`与`List<? super T>`的区别
这个问题，初级工程师在做面试题时会出现。高级工程师很少会遇到，但实际上，不少工作了 5 年左右的工程师也回答的不是很好。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1662087295055-445ee087-493b-43cc-a736-32a8406860d2.jpeg#clientId=uba66655a-4f88-4&from=paste&id=u824e08bc&originHeight=807&originWidth=921&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u189ccb76-3c65-453c-967a-75fc1f7e0ad&title=)<br />先来看一下名词解释：
<a name="BW2vq"></a>
### （1）`?`
`?`表示类型通配符，即具体传什么参数类型，在List定义时不用考虑。
<a name="ALhcg"></a>
### （2）`<T>`
这里的 `<>` 表示泛型，`T` 表示泛型中装载的类型为`T`类型，等到需要的时候，可以具体这个 `T`。在使用动态数组实现 ArrayList 的时候，如果希望这个 ArrayList 不仅仅支持一个类型的话，可以给这个 ArrayList 定义泛型，泛型中存放的是T类型。在实际创建出这个 ArrayList 对象时，可以指定泛型中的具体类型。
<a name="GE2yt"></a>
### （3）`<? extends T>`
类型上界，这里的 `?` 可以是 T 类型或者 T 的子类类型。
<a name="IfGcd"></a>
### （4）`<? super T>`
类型下界，这里的`?`可以是T类型或者T的超类类型，但不代表可以往里面添加任意超类类型的元素。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662087294981-c8f4f72c-7d39-4860-9550-4d5748e2078a.png#clientId=uba66655a-4f88-4&from=paste&id=uf799d15e&originHeight=715&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue573c737-9789-47fb-8d98-4f81db9a8da&title=)
<a name="caFba"></a>
## 在List中引入通配符界限限制的假设
不管是`List<? extends T>`还是`List<? super T>`，如果能读取元素，那么这个元素一定能转化为 T 类型，注意不是强制类型转换，强制类型转换是容易出现问题。<br />显然`List<? extends T>`内都是 T 的子类类型，能够向上转型为 T 类型，因此该 list 可以读取。<br />而`List<? super T>`内可以是 T 的超类类型，T 的超类转 T 是有可能出现异常的。<br />那干脆转化成 Object 类型不好吗，所有类的基类都是 Object，不属于强制类型转换。哥们，转换成 Object 了，那还图个啥？转换为 Object 类型是没有意义的。<br />假设`List<? extends T>`能添加元素，那么需要满足添加的任意元素需要能够直接转化成 T 的任何一个子类，T 的子类 A 和子类 B 是不能相互转化的，显然该 list 是不能添加元素的。<br />假设`List<? super T>`能添加元素，那么同样需要满足添加的任意元素能够直接转化成 T 的任何一个超类。此时添加 T 的子类元素就能满足该要求，因为 T 的任意子类可以向上转型成 T 的任何超类。
<a name="PG75F"></a>
## `List<? extends T>`
`List<? extends T>`是被设计用来读取数据的泛型，并且只能读取类型为 T 的元素。原因如下：<br />元素是可以进行向上转型的，因此，可以这样做来读取元素。
```java
List<? extends Number> list = new ArrayList<>(); 
Number number = list.get(0);
```
可以读取，但不能写入，比如以下的代码就直接报错。
```java
public class Main {

    static class A { }

    static class B extends A { }

    static class C extends A { }

    public static void main(String[] args) {
        List<? extends A> list = new ArrayList<>();
        list.add(new A());//编译报错
        list.add(new B());//编译报错
        list.add(new C());//编译报错
    }

}
```
A 的子类 B 与子类 C 是不能相互转换的，因此是不能往该 list 中添加元素。<br />虽然不能添加元素，但可以在初始化的时候，接受一个已经定义好的 list，而该 list 存放的类型一定相同。因此，`List<? extends T>`可直接接受一个定义好的 list。
```java
public static List<Integer> getList(){
    List<Integer> list=new ArrayList<>();
    list.add(1);
    return list;
}

// ....

public static void main(String[] args) {
    List<? extends Number> list = new ArrayList<>();
    list=getList();
}
```
<a name="pFxMJ"></a>
## `List<? super T>`
`List<? super T>`是被设计用来添加数据的泛型，并且只能添加 T 类型或其子类类型的元素。<br />为什么只能是 T 类型及其子类型元素，超类类型的元素不可以吗？<br />超类类型转化为 T 类型，是需要强制类型转换的，是容易出现异常的，无法保障的。<br />而传入 T 类型及其子类类型时，能够直接转化为 T 的任意超类类型。比如，下面的代码是可以运行的
```java
public class Main {

    static class A { }

    static class B extends A { }

    static class C extends A { }

    public static void main(String[] args) {
        List<? super A> list = new ArrayList<>();
        list.add(new A());
        list.add(new B());
        list.add(new C());
    }
}
```
该 list 也可以读取其中的元素，从第二节可以得出，只能用 Object 接收，没多大意义。
```java
List<? super Integer> list2 = new ArrayList<>();
list2.add(new Integer(1));
Object integer=list2.get(0);
```
如果使用 Object 类型来接收获取到的元素，那么元素本身的类型就会丢失，因此，不使用`List<? super T>`来获取元素。<br />如果非要使用`List<? super Integer>`中的 Integer 类型来接收获取到的元素，那么必须进行强制类型转换，是会出现异常的，无法保障。
```java
List<? super Integer> list2 = new ArrayList<>();
list2.add(new Integer(1));
Integer integer1= (Integer) list2.get(0);
```
<a name="Couvr"></a>
## 总结
（1）`List<? extends T>`适用于读取数据，读取出来的数据全部用T类型接收。如果往此 list 中添加 T 类型不同的子类的话，各种子类无法相互转换，因此不能添加元素，但可接受初始赋值。<br />（2）`List<? super T>`适用于添加元素，只能添加 T 类型或其子类类型。因为这些类型都能转换为T的任意超类类型（向上转型），因此可以对此 list 添加元素。只能用 Object 类型来接收获取到的元素，但是这些元素原本的类型会丢失。
<a name="beigF"></a>
## 更加通俗易懂的例子
举一些直观的例子。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662087295214-c78f7c18-790e-4e16-88a4-d53f9c4c4a35.png#clientId=uba66655a-4f88-4&from=paste&id=ud135941a&originHeight=637&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6e220fda-7d5f-4c9f-8dd2-13ad0d252c2&title=)<br />注意：向上转型是安全的，向下转型是不安全的，除非知道 List 中的真实类型，否则向下转型就会报错。
<a name="FbjQR"></a>
### extends
`List<? extends Number> foo3`意味着下面的赋值语句都是合法的：
```java
List<? extends Number> foo3 = new ArrayList<Number>();  // Number "extends" Number (in this context)
List<? extends Number> foo3 = new ArrayList<Integer>(); // Integer extends Number
List<? extends Number> foo3 = new ArrayList<Double>();  // Double extends Number
```

1. 读取

给定上述可能的赋值语句，能保证从`List foo3`中取出什么样类型的对象？

- 可以读取一个`Number`对象，因为上面任意一个list都包含`Number`对象或者`Number`子类的对象（上面的Number、Integer、Double都可以转型成Number，并且是安全的，所以读取总是可以的）。如下代码就不会报错：
```java
List<? extends Number> foo4 = new ArrayList<Integer>();
Number number = foo4.get(0);
```

- 不能读取一个`Integer`对象，因为foo3可能指向的是`List<Double>`（与其运行时发现Double转成Integer报错，不如编译时就不让从foo3中取`Integer`对象）。如下代码编译时会报Incompatible types错的：
```java
List<? extends Number> foo4 = new ArrayList<Integer>();
Integer number = foo4.get(0);
```
因为编译的时候编译器只知道foo4引用是一个List<? extends Number>，要到运行时才会绑定到new ArrayList()，所以编译的时候是无法判断foo4指向的List中到底是什么类型，唯一能确定的就是这个类型是Number的子类（或者就是Number类）。

- 也不能读取一个`Double`对象，因为foo3可能指向的是`List<Integer>`。
1. 写入

给定上述可能的赋值语句，能往`List foo3`中添加什么类型的对象从而保证它对于所有可能的ArrayList都是合法的呢？

- 不能添加一个`Integer`对象，因为foo3可能指向的是`List<Double>`。如下代码是会编译报错的：
```java
List<? extends Number> foo4 = new ArrayList<Integer>();
foo4.add(new Integer(1));
```
因为编译期间是无法知道foo4指向的ArrayList中到底放的是什么类型，只有到运行时才知道（就是Java所谓的晚绑定或运行时绑定）。与其到运行时发现往一个ArrayList中add一个Integer导致抛出类型转换异常，倒不如编译时就报错，即使ArrayList中放的就是Integer类型。

- 不能添加一个`Double`对象，因为foo3可能指向的是`List<Integer>`。
- 不能添加一个`Number`对象，因为foo3可能指向的是`List<Integer>`。

**「总结一下」**：不能往`List<? extends T>`中添加任何对象，因为不能保证List真正指向哪个类型，所以不能确定添加的对象就是List所能接受的类型。能保证的，仅仅是可以从List中读取的时候，获得的肯定是一个`T`类型的对象（即使是`T`类型的子类对象也是`T`类型的）。
<a name="kpwWS"></a>
### super
现在考虑`List<? super T>`<br />包含通配符的声明`List<? super Integer> foo3`意味着下面任何一个赋值语句都是合法的：
```java
List<? super Integer> foo3 = new ArrayList<Integer>();  // Integer is a "superclass" of Integer (in this context)
List<? super Integer> foo3 = new ArrayList<Number>();   // Number is a superclass of Integer
List<? super Integer> foo3 = new ArrayList<Object>();   // Object is a superclass of Integer
```

1. 读取

给定上述可能的赋值语句，当读取`List foo3`中的元素的时候，能保证接收到什么类型的对象呢？

- 不能保证是一个`Integer`对象，因为foo3可能指向一个`List<Number>`或者`List<Object>`。
- 不能保证是一个`Number`对象，因为foo3可能指向一个`List<Object>`。
- 能保证的仅仅是它一定是一个`Object`类的实例或者`Object`子类的实例（但是不知道到底是哪个子类）。
1. 写入

给定上述可能的赋值语句，能往`List foo3`中添加什么类型的对象从而保证它对于所有可能的`ArrayList`都是合法的呢？

- 可以添加一个`Integer`实例，因为`Integer`类型对于上述所有的list都是合法的。
- 可以添加任何`Integer`子类的实例，因为一个`Integer`子类的实例都可以向上转型成上面列表中的元素类型。
- 不可以添加`Double`类型，因为foo3可能指向的是`ArrayList<Integer>`。
- 不可以添加`Number`类型，因为foo3可能指向的是`ArrayList<Integer>`。
- 不可以添加`Object`类型，因为foo3可能指向的是`ArrayList<Integer>`。
<a name="epJN7"></a>
### PECS
PECS是"Producer Extends,Consumer Super"（生产者用Extends，消费者用Super）的缩写。

- "Producer Extends"的意思是，如果需要一个`List`去生产`T`类型values（也就是说需要去list中读取`T`类型实例），需要声明这个`List`中的元素为`? extends T`，例如`List<? extends Integer>`，但是不能往里面添加元素。
- "Consumer Super"的意思是，如果需要一个`List`去消费`T`类型values（也就是说需要往list中添加`T`类型实例），需要声明这个`List`中的元素为`? super T`，例如`List<? super Integer>`。但是不能保证从这个list中读取出来对象类型。
- 如果既需要往list中写，也需要从list中读，那么就不能用通配符`?`，必须用精确的类型，比如`List<Integer>`。
- 可以参考JDK源码中的`Collections`类的`copy`方法，来理解PECS。
