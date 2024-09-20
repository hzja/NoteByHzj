Java @SafeVarargs<br />`@SafeVarargs` 是jdk1.7引入的适用于可变参数与泛型能够更好结合的一个注解。<br />`@SuppressWarnings` 产生编译期警告的示例代码
<a name="auBuu"></a>
### 官方解释
程序员认定带有注释的主体或者构造函数不会对其执行潜在的不安全操作将此注释应用于未经检查的方法或者构造器在"不可具体化"的和未经检查的参数类型警告关于所有数组参数创建的时候，除了强加使用`@Target`元注解的限制之外，编译器还被用在注解类型上来实现额外的限制下面几种情况会在使用`@SafeVarags`注解的时候产生编译时错误：在声明一个固定参数的方法或者构造函数的时候，也就是说如果认为方法或者构造方法是类型安全的，那么也就可以使用`@SafeVarargs`来跳过`@SuppressWarnings("unchecked")`检查。
<a name="Yyi9F"></a>
### 示例
```java
publicclassSafeVarargs{
    // 这其实不是一个安全的类型检查
    @SafeVarargs
    static void m(List<String>...lists){
        // 先会存储到 array[0] 的位置
        Object[] array=lists;
        List<Integer>tmpList=Arrays.asList(42);
        // array[0] 又保存了tmpList(Integer)进行覆盖
        // tmpList是一个List对象（类型已经擦除），赋值给Object类型的对象是允许的（向上转型），
        // 能够编译通过
        array[0] =tmpList;
        // 实际取出来的应该是 42
        Strings=lists[0].get(0);
    }
    public static void main(String[] args) {
        List<String>list1=Arrays.asList("one","two");
        m(list1);
    }
}
```
```
Exceptioninthread"main"java.lang.ClassCastException: java.lang.Integercannotbecasttojava.lang.String
atjava7.SafeVarargs.m(SafeVarargs.java:14)
atjava7.SafeVarargs.main(SafeVarargs.java:21)
Processfinishedwithexitcode1
```
<a name="ZRSGa"></a>
### 具体分析
`List<String>list1=Arrays.asList("one","two");`当程序执行到这一步，会创建一个list1 ，其内部存在两个固定的值[one,two]，调用`m(list1)`方法`m(list1);`接着`Object[] array=lists;`程序执行到这一步，会在array数组中的第0个位置上存储一个list1 对象`List<Integer>tmpList=Arrays.asList(42);`创建一个Integer的列表，存储一个integer类型的元素42，`array[0] =tmpList;`重新给array[0] 进行赋值，把原来位置的lists 进行覆盖，存储新的元素tmpList，`Strings=lists[0].get(0);`，预期的结果应该是取的值是42，但是实际上却报出了ClassCastException。因为最后array[0] 中的值 42，Integer 类型， 无法直接用String 对象进行接受，所以会报错。
