Java 泛型<br />泛型是通用设计上必不可少的元素，所以真正理解与正确使用泛型，是一门必修课！
<a name="X1322"></a>
## 大纲
![2021-05-07-09-09-24-726932.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620350800013-6df910d4-737c-44ea-abed-f85517375601.png#clientId=u2007e092-9dad-4&from=ui&id=ud49d5b03&originHeight=628&originWidth=882&originalType=binary&size=28805&status=done&style=shadow&taskId=u13ef6331-c553-4cb4-8ec9-de2fdb86019)
<a name="eH3W1"></a>
## 基础
因为本文重实践，而且面对的是Java开发人员群体，大家对泛型都有基础，所以泛型基础这块会快速过，帮助大家回忆下即可，后面主要的则重点是通配符
<a name="KC2Tx"></a>
### 编译期与运行期
编译期是指把源码交给编译器编译成计算机可执行文件的过程，运行期是指把编译后的文件交给计算机执行，直到程序结束。<br />在Java中就是把.java文件编译成.class文件，再把编译后的文件交给J V M加载执行，如下图<br />![2021-05-07-09-09-24-801950.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620350812770-106184af-0100-487e-93ea-d9f20b537aa3.png#clientId=u2007e092-9dad-4&from=ui&id=u73628f5c&originHeight=734&originWidth=1080&originalType=binary&size=85147&status=done&style=shadow&taskId=u56e5b36d-2633-45f5-90bc-671bbbc7be6)
<a name="sFf9I"></a>
### 泛型
泛型又叫“参数化类型”，这么抽象的专业词汇不好理解，阿星就用大白话的形式来解释。<br />人是铁，饭是刚，吃饭是刚需，要吃饭自然就少不了碗筷，但是没有规定碗只能盛饭，除了盛饭它还能盛汤、盛菜，制造者只造这个碗，不关心碗盛什么，具体要盛什么由使用者来决定，这就是泛型的概念。<br />泛型就是在定义类、接口、方法的时候指定某一种特定类型（碗），让类、接口、方法的使用者来决定具体用哪一种类型的参数（盛的东西）。<br />Java的泛型是在1.5引入的，只在编译期做泛型检查，运行期泛型就会消失，把这称为“泛型擦除”，最终类型都会变成`Object`。<br />在没有泛型之前，从集合中读取到的每一个对象都必须进行类型转换，如果不小心插入了错误的类型对象，在运行时的转换处理就会出错，有了泛型后，你可以告诉编译器每个集合接收的对象类型是什么，编译器在编译期就会做类型检查，告知是否插入了错误类型的对象，使得程序更加安全，也更加清楚。<br />最后插一句，泛型擦除与原生态类型（`List`就是原生态，`List<T>`非原生态）是为了照顾1.5以前设计上的缺陷，为兼容非泛型代码，所作出的折中策略，所以不推荐使用原生态类型，如果使用了原生态类型，就失去了泛型在安全性与描述性方面的优势。
<a name="WtUNX"></a>
### 泛型类
![2021-05-07-09-09-24-848373.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620350825350-64c46340-1c7e-460a-bba3-6dc5c897056b.png#clientId=u2007e092-9dad-4&from=ui&id=u9982f15b&originHeight=306&originWidth=549&originalType=binary&size=18507&status=done&style=shadow&taskId=uf677c9af-5fce-4aea-a76c-aeb4ed5f11a)<br />类上定义泛型，作用于类的成员变量与函数，代码实例如下
```java
public class GenericClass<T>{
    //成员变量
    private T t;
    
    public  void function(T t){

    }
    
    public T functionTwo(T t){
        //注意，这个不是泛型方法！！！
       return t;
    }
}
```
<a name="ey839"></a>
### 泛型接口
接口上定义泛型，作用于函数，代码实例如下
```java
public interface GenericInterface<T> {
    
    public T get();
    
    public void set(T t);

    public T delete(T t);
    
    default T defaultFunction(T t){
        return t;
    }
}
```
<a name="Px5zN"></a>
### 泛型函数
![2021-05-07-09-09-24-974472.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1620350841930-7d908c9a-b320-474c-b393-c021bb676d3f.png#clientId=u2007e092-9dad-4&from=ui&id=u7fb9f800&originHeight=652&originWidth=693&originalType=binary&size=49614&status=done&style=shadow&taskId=u569280f6-b9bf-4cec-8032-7b5071146ea)<br />函数返回类型旁加上泛型，作用于函数，代码实例如下
```java
public class GenericFunction {

    public <T> void function(T t) {
    }

    public <T> T functionTwo(T t) {
        return t;
    }

    public <T> String functionThree(T t) {
        return "";
    }
}
```
<a name="u4LKh"></a>
### 通配符
通配符是为了让Java泛型支持范围限定，这样使得泛型的灵活性提升，同时也让通用性设计有了更多的空间。

- `<?>`：无界通配符，即类型不确定，任意类型
- `<? extends T>`：上边界通配符，即`?`是继承自`T`的任意子类型，遵守只读不写
- `<? super T>`：下边界通配符，即`?`是`T`的任意父类型，遵守只写不读

「**通配符限定的范围是体现在确认“参数化类型”的时候，而不是“参数化类型”填充后**」，可能这句话不太好理解，来看看下面的代码
```java
/**
 * 1.创建泛型为Number的List类，Integer、Double、Long等都是Number的子类
 *   new ArrayList<>() 等价于 new ArrayList<Number>()
 */
List<Number> numberList = new ArrayList<Number>();

/**
 * 2.添加不同子类
 */
numberList.add(1);//添加Integer类型
numberList.add(0.5);//添加Double类型
numberList.add(10000L);//添加Long类型

/**
 * 3.创建泛型为Number的List类，Integer、Double、Long等都是Number的子类
 *   引用是泛型类别是Number，但具体实现指定的泛型是Integer
 */
List<Number> numberListTwo = new ArrayList<Integer>();//err 异常编译不通过

/**
 * 4.创建泛型为Integer的List类，把该对象的引用地址指向泛型为Number的List
 */
List<Integer> integerList = new ArrayList<Integer>();
List<Number> numberListThree = integerList;//err 异常编译不通过
```

- 第一步：创建一个泛型为`Number`的`List`，编译器检查泛型类别是否一致，一致编译通过（确认参数化类型）
- 第二步：泛型`Number`已经填充完毕，调用`add`函数，此时`add`入参泛型`T`已经填充为`Number`，`add`可入参`Number`或其子类
- 第三步：又创建一个泛型为`Number`的`List`，编译器检查泛型类别是否一致，不一致编译失败，提示错误（确认参数化类型）
- 第四步：其实与第三步一样，只是做了一个间接的引用（确认参数化类型）

如果要解决上面的编译不通过问题，就需要使用通配符，代码如下
```java
/**
 * 1.上边界通配符，Number与Number子类
 */
List<? extends Number> numberListFour = new ArrayList<Number>();
numberListFour = new ArrayList<Integer>();
numberListFour = new ArrayList<Double>();
numberListFour = new ArrayList<Long>();

/**
 * 2.下边界通配符,Integer与Integer父类
 */
List<? super Integer> integerList = new ArrayList<Integer>();
integerList = new ArrayList<Number>();
integerList = new ArrayList<Object>();

/**
 * 3. 无界通配符,类型不确定，任意类型
 */
List<?> list = new ArrayList<Integer>();
list = new ArrayList<Number>();
list = new ArrayList<Object>();
list = new ArrayList<String>();
```
最后再来说上边界通配符只读不写，下边界通配符只写不读到底是什么意思，用最简单的话来说

- `<? extends T>`上边界通配符不作为函数入参，只作为函数返回类型，比如`List<? extends T>`的使用`add`函数会编译不通过，`get`函数则没问题
- `<? super T>`下边界通配符不作为函数返回类型，只作为函数入参，比如`List<? super T>`的`add`函数正常调用，`get`函数也没问题，但只会返回`Object`，所以意义不大

大家只需要记住上面的规则即可，如果想知道为什么这样设计，可以去了解下`P E C S (producer-extends,consumer-super)`原则
<a name="GzUMW"></a>
## 最佳实践
<a name="SdIHx"></a>
### 无限通配符场景
使用泛型，类型参数不确定并且不关心实际的类型参数，就可以使用`<?>`，像下面的代码
```java
/**
 * 获取集合长度
 */
public static <T> int size(Collection<T> list){
    return list.size();
}

/**
 * 获取集合长度-2
 */
public static int sizeTwo(Collection<?>  list){
    return list.size();
}


/**
 * 获取任意Set两个集合交集数量
 */
public static <T,T2> int beMixedSum(Set<T> s1,Set<T2> s2){
    int i = 0;
    for (T t : s1) {
        if (s2.contains(t)) {
            i++;
        }
    }
    return i;
}

/**
 * 获取任意两个Set集合交集数量-2
 */
public static  int beMixedSumTwo(Set<?> s1,Set<?> s2){
    int i = 0;
    for (Object o : s1) {
        if (s2.contains(o)) {
            i++;
        }
    }
    return i;
}
```
`size`与`sizeTwo`这两个函数都可以正常使用，但是站在设计的角度，`sizeTwo`会更合适，函数的目标是返回任意集合的长度，入参采用`<T>`或`<?>`都可以接收，但是函数本身并不关心你是什么类型参数，仅仅只要返回长度即可，所以采用`<?>`。<br />`beMixedSum`与`beMixedSumTwo`这两个函数比较，道理同上面一样，`beMixedSumTwo`会更合适，函数的目标是返回两个任意Set集合的交集数量，`beMixedSum`函数虽然内部有使用到`<T>`，但是意义不大，因为`contains`入参是`Object`，函数本身并不关心你是什么类型参数，所以采用`<?>`。<br />忘了补充另一个场景，就是原生态类型，上述代码使用原生态类型函数使用也没问题，但是强烈不推荐，因为使用原生态就丢失了泛型带来的安全性与描述性！！！
<a name="u3i3F"></a>
### 上下边界通配符场景
首先泛型是不变的，换句话说`List<Object> != List<String>`，有时候需要更多灵活性，就可以通过上下边界通配符来做提升。
```java
/**
 * 集合工具类
 */
public class CollectionUtils<T>{
        
    /**
     * 复制集合-泛型
     */
    public List<T>  listCopy(Collection<T> collection){
        List<T> newCollection = new ArrayList<>();
        for (T t : collection) {
            newCollection.add(t);
        }
        return newCollection;
    }
    
}
```
上面声明了一个`CollectionUtils`类，拥有`listCopy`方法，传入任意一个集合返回新的集合，看似没有什么问题，也很灵活，那再看看下面这段代码。
```java
public static void main(String[] agrs){
    CollectionUtils<Number> collectionUtils = new CollectionUtils<>();
    List<Number>  list = new ArrayList<>();
    //list.add....
    List<Integer>  listTwo = new ArrayList<>();
    //listTwo.add....
    List<Double>  listThree = new ArrayList<>();
    //listThree.add....

    List<Number> list1 = collectionUtils.listCopy(list);
    list1 = collectionUtils.listCopy(listTwo);//err 编译异常
    list1 = collectionUtils.listCopy(listThree);//err 编译异常
}
```
创建`CollectionUtils`类，泛型的类型参数为`Number`，`listCopy`函数入参的泛型填充为`Number`，此时`listCopy`只支持泛型为`Number`的`List`，如果要让它同时支持泛型为`Number`子类的`List`，就需要使用上边界通配符，再追加一个方法
```java
/**
 * 集合工具
 */
public class CollectionUtils<T>{
        
    /**
     * 复制集合-泛型
     */
    public List<T>  listCopy(Collection<T> collection){
        List<T> newCollection = new ArrayList<>();
        for (T t : collection) {
            newCollection.add(t);
        }
        return newCollection;
    }
    
    /**
     * 复制集合-上边界通配符
     */
    public  List<T>  listCopyTwo(Collection<? extends T> collection){
        List<T> newCollection = new ArrayList<>();
        for (T t : collection) {
            newCollection.add(t);

        }
        return newCollection;
    }
}

public static void main(String[] agrs){
    CollectionUtils<Number> collectionUtils = new CollectionUtils<>();
    List<Number>  list = new ArrayList<>();
    //list.add....
    List<Integer>  listTwo = new ArrayList<>();
    //listTwo.add....
    List<Double>  listThree = new ArrayList<>();
    //listThree.add....

    List<Number> list1 = collectionUtils.listCopyTwo(list);
    list1 = collectionUtils.listCopyTwo(listTwo);
    list1 = collectionUtils.listCopyTwo(listThree);
}
```
现在使用`listCopyTwo`就没有问题，`listCopyTwo`对比`listCopy`它的适用范围更广泛也更灵活，`listCopy`能做的`listCopyTwo`能做，`listCopyTwo`能做的`listCopy`就不一定能做了，除此之外，细心的小伙伴肯定发现了，使用上边界通配符的`collection`在函数内只使用到了读操作，遵循了只读不写原则。<br />看完了上边界通配符，再来看看下边界通配符，依然是复制方法
```java
/**
 * 儿子
 */
public class Son extends Father{}

/**
 * 父亲
 */
public class Father  extends  Grandpa{}

/**
 * 爷爷
 */
public class Grandpa {}

/**
 * 集合工具
 */
public class CollectionUtils<T>{

    /**
     * 复制集合-泛型
     * target目标   src来源
     */
   public void copy(List<T> target,List<T> src){
        if (src.size() > target.size()){
            for (int i = 0; i < src.size(); i++) {
                target.set(i,src.get(i));
            }
        }
    }
    
}
```
定义了3个类，分别是`Son`儿子、`Father`父亲、`Grandpa`爷爷，它们是继承关系，作为集合元素，还声明了一个`CollectionUtils`类，拥有`copy`方法，传入两个集合，目标集合与来源集合，把来源集合元素复制到目标集合中，再看看下面这段代码
```java
public static void main(String[] agrs){
    CollectionUtils<Father> collectionUtils = new CollectionUtils<>();

    List<Father>  fatherTargets = new ArrayList<>();
    List<Father>  fatherSources = new ArrayList<>();
    //fatherSources.add...
    collectionUtils.copy(fatherTargets,fatherSources);
    
    //子类复制到父类
    List<Son> sonSources = new ArrayList<>();
    //sonSources.add...
    collectionUtils.copy(fatherTargets,sonSources);//err 编译异常
    
}
```
创建`CollectionUtils`类，泛型的类型参数为`Father`父亲，`copy`函数入参的泛型填充为`Father`，此时copy只支持泛型为`Father`的`List`，也就说，只支持泛型的类型参数为`Father`之间的复制，如果想支持把子类复制到父类要怎么做，先分析下`copy`函数，`copy`函数的入参`src`在函数内部只涉及到了`get`函数，即读操作（泛型只作为`get`函数返回类型），符合只读不写原则，可以采用上边界通配符，调整代码如下
```java
/**
 * 集合工具
 */
public class CollectionUtils<T>{

    /**
     * 复制集合-泛型
     * target目标   src来源
     */
    public void copy(List<T> target,List<? extends T> src){
        if (src.size() > target.size()){
            for (int i = 0; i < src.size(); i++) {
                target.set(i,src.get(i));
            }
        }
    }
}

public static void main(String[] agrs){
    CollectionUtils<Father> collectionUtils = new CollectionUtils<>();

    List<Father>  fatherTargets = new ArrayList<>();
    List<Father>  fatherSources = new ArrayList<>();
    //fatherSources.add...
    collectionUtils.copy(fatherTargets,fatherSources);
    
    //子类复制到父类
    List<Son> sonSources = new ArrayList<>();
    //sonSources.add...
    collectionUtils.copy(fatherTargets,sonSources);
    
    //把子类复制到父类的父类
    List<Grandpa> grandpaTargets = new ArrayList<>();
    collectionUtils.copy(grandpaTargets,sonSources);//err 编译异常
}
```
`src`入参调整为上边界通配符后，`copy`函数传入`List<Son> sonSources`就没问题了，此时的`copy`函数相较之前的更加灵活了，支持同类与父子类复制，接着又发现了一个问题，目前能复制到上一级父类，如果是多级父类，还无法支持，继续分析`copy`函数，`copy`函数的入参`target`在函数内部只涉及到了`add`函数，即写操作（泛型只作为`add`函数入参），符合只写不读原则，可以采用下边界通配符，调整代码如下
```java
/**
 * 集合工具
 */
public class CollectionUtils<T>{

    /**
     * 复制集合-泛型
     * target目标   src来源
     */
    public void copy(List<? super T>  target,List<? extends T> src){
        if (src.size() > target.size()){
            for (int i = 0; i < src.size(); i++) {
                target.set(i,src.get(i));
            }
        }
    }
}

public static void main(String[] agrs){
    CollectionUtils<Father> collectionUtils = new CollectionUtils<>();

    List<Father>  fatherTargets = new ArrayList<>();
    List<Father>  fatherSources = new ArrayList<>();
    //fatherSources.add...
    collectionUtils.copy(fatherTargets,fatherSources);
    
    //子类复制到父类
    List<Son> sonSources = new ArrayList<>();
    //sonSources.add...
    collectionUtils.copy(fatherTargets,sonSources);
    
    //把子类复制到父类的父类
    List<Grandpa> grandpaTargets = new ArrayList<>();
    collectionUtils.copy(grandpaTargets,sonSources);
}
```
`copy`函数终于是完善了，可以说现在是真正支持父子类复制，不难发现`copy`函数的设计还是遵循通配符原则的，`target`作为目标集合，只做写入，符合只写不读原则，采用了下边界通配符，`src`作为来源集合，只做读取，符合只读不写原则，采用了上边界通配符，最后设计出来的`copy`函数，它的灵活性与适用范围是远超`<T>`方式设计的。<br />最后总结一下，什么时候用通配符，**如果参数泛型类即要读也要写，那么就不推荐使用，使用正常的泛型即可，如果参数泛型类只读或写，就可以根据原则采用对应的上下边界**，是不是十分简单，最后再说一次读写的含义，这块确实很容易晕

- 读：所谓读是指参数泛型类，泛型只作为该参数类的函数返回类型，那这个函数就是读，`List`作为参数泛型类，它的`get`函数就是读
- 写：所谓写是指参数泛型类，泛型只作为该参数类的函数入参，那这个函数就是写，`List`作为参数泛型类，它的`add`函数就是读

可以思考下`Stream`的`forEach`函数与`map`函数的设计，在Java1.8 `Stream`中是大量用到了通配符设计
```java
/**
 * 下边界通配符
 */
void forEach(Consumer<? super T> action);

public interface Consumer<T> {

    //写方法
    void accept(T t);
}
```
```java
/**
 * 上下边界通配符
 */
<R> Stream<R> map(Function<? super T, ? extends R> mapper)

public interface Function<T, R> {

     //读写方法，T只作为入参符合写，R只作为返回值，符合读
    R apply(T t);
}
```
```java
//代码案例
public static void main(String[] agrs) {

    List<Father> fatherList = new ArrayList<>();

    Consumer<? super Father> action = new Consumer<Father>() {
        @Override
        public void accept(Father father) {
            //执行father逻辑
        }
    };

    //下边界通配符向上转型
    Consumer<? super Father> actionTwo = new Consumer<Grandpa>() {
        @Override
        public void accept(Grandpa grandpa) {
            //执行grandpa逻辑
        }
    };

    Function<? super Father, ? extends Grandpa> mapper = new Function<Father, Grandpa>() {
        @Override
        public Grandpa apply(Father father) {
            //执行father逻辑后返回Grandpa
            return new Grandpa();
        }
    };

    //下边界通配符向上转型，上边界通配符向下转型
    Function<? super Father, ? extends Grandpa> mapperTwo = new Function<Grandpa, Son>() {
        @Override
        public Son apply(Grandpa grandpa) {
            //执行grandpa逻辑后，返回Son
            return new Son();
        }
    };

    fatherList.stream().forEach(action);
    fatherList.stream().forEach(actionTwo);

    fatherList.stream().map(mapper);
    fatherList.stream().map(mapperTwo);
}
```
<a name="Z1zW7"></a>
### 有限制泛型场景
有限制泛型很简单了，应用场景就是需要对泛型的参数类型做限制，就可以使用它，比如下面这段代码
```java
public class GenericClass<T extends Grandpa> {
    
   
    public void test(T t){
        //....
    }

}

public static void main(String[] agrs){
    GenericClass<Grandpa> grandpaGeneric = new GenericClass<>();
    grandpaGeneric.test(new Grandpa());
    grandpaGeneric.test(new Father());
    grandpaGeneric.test(new Son());
    
    GenericClass<Father> fatherGeneric = new GenericClass<>();
    fatherGeneric.test(new Father());
    fatherGeneric.test(new Son());

    GenericClass<Son> sonGeneric = new GenericClass<>();
    sonGeneric.test(new Son());
    
    GenericClass<Object> ObjectGeneric = new GenericClass<>();//err 编译异常
    
}
```
`GenericClass`泛型参数化类型被限制为`Grandpa`或其子类，就这么简单，千万不要把有限制泛型与上边界通配符搞混了，这两个不是同一个东西（`<T extends Grandpa> != <? extends Grandpa>`），`<T extends Grandpa>`不需要遵循上边界通配符的原则，它就是简单的泛型参数化类型限制，而且没有`super`的写法。
<a name="xokId"></a>
### 递归泛型场景
在有限制泛型的基础上，又可以衍生出递归泛型，就是自身需要使用到自身，比如集合进行自定义元素大小比较的时候，通常会配合`Comparable`接口来完成，看看下面这段代码
```java
public class Person implements Comparable<Person> {

    private int age;

    public Person(int age) {
        this.age = age;
    }

    public int getAge() {
        return age;
    }

    @Override
    public int compareTo(Person o) {
        // 0代表相等 1代表大于  <0代表小于    
        return this.age - o.age;
    }
}


/**
 * 集合工具
 */
public class CollectionUtils{
    
    /**
     * 获取集合最大值
     */
    public static  <E extends Comparable<E>> E max(List<E> list){
        E result = null;
        for (E e : list) {
             if (result == null || e.compareTo(result) > 0){
                 result = e;
             }
        }
        return result;
    }
}


public static void main(String[] agrs){

    List<Person> personList = new ArrayList<>();
    personList.add(new Person(12));
    personList.add(new Person(19));
    personList.add(new Person(20));
    personList.add(new Person(5));
    personList.add(new Person(18));
    //返回年龄最大的Person元素 
    Person max = CollectionUtils.max(personList);

}
```
重点关注`max`泛型函数，`max`泛型函数的目标是返回集合最大的元素，内部比较元素大小，取最大值返回，也就说需要和同类型元素做比较，`<E extends Comparable<E>>`含义是，泛型`E`必须是`Comparable`或其子类/实现类，因为比较元素是同类型，所以`Comparable`泛型也是`E`,最终接收的`List`泛型参数化类型必须实现了`Comparable`接口，并且`Comparable`接口填充的泛型也是该参数化类型，就像上述代码一样。
