Java
<a name="ANAsP"></a>
## `JCTree`的介绍
`JCTree`是语法树元素的基类，包含一个重要的字段`pos`，该字段用于指明当前语法树节点（`JCTree`）在语法树中的位置，因此不能直接用`new`关键字来创建语法树节点，即使创建了也没有意义。<br />此外，结合访问者模式，将数据结构与数据的处理进行解耦，部分源码如下：
```java
public abstract class JCTree implements Tree, Cloneable, DiagnosticPosition {

    public int pos = -1;

    ...

    public abstract void accept(JCTree.Visitor visitor);

    ...
}
```
可以看到`JCTree`是一个抽象类，这里重点介绍几个`JCTree`的子类

1. `JCStatement`：**声明**语法树节点，常见的子类如下
   - `JCBlock`：**语句块**语法树节点
   - `JCReturn`：`**return**`**语句**语法树节点
   - `JCClassDecl`：**类定义**语法树节点
   - `JCVariableDecl`：**字段/变量定义**语法树节点
2. `JCMethodDecl`：**方法定义**语法树节点
3. `JCModifiers`：**访问标志**语法树节点
4. `JCExpression`：**表达式**语法树节点，常见的子类如下
   - `JCAssign`：**赋值语句**语法树节点
   - `JCIdent`：**标识符**语法树节点，可以是变量，类型，关键字等等
<a name="v5Fi6"></a>
## TreeMaker介绍
TreeMaker用于创建一系列的语法树节点，上面说了创建`JCTree`不能直接使用`new`关键字来创建，所以Java提供了一个工具，就是`TreeMaker`，它会在创建时创建的`JCTree`对象设置`pos`字段，所以必须使用上下文相关的`TreeMaker`对象来创建语法树节点。<br />具体的API介绍可以参照，TreeMakerAPI，接下来着重介绍一下常用的几个方法。
<a name="UNH0v"></a>
### `TreeMaker.Modifiers`
`TreeMaker.Modifiers`方法用于创建访问标志语法树节点（`JCModifiers`），源码如下
```java
public JCModifiers Modifiers(long flags) {
    return Modifiers(flags, List.< JCAnnotation >nil());
}

public JCModifiers Modifiers(long flags,
    List<JCAnnotation> annotations) {
        JCModifiers tree = new JCModifiers(flags, annotations);
        boolean noFlags = (flags & (Flags.ModifierFlags | Flags.ANNOTATION)) == 0;
        tree.pos = (noFlags && annotations.isEmpty()) ? Position.NOPOS : pos;
        return tree;
}
```

1. flags：访问标志
2. annotations：注解列表

其中`flags`可以使用枚举类`com.sun.tools.javac.code.Flags`来表示，例如可以这样用，就生成了下面的访问标志了。
```java
treeMaker.Modifiers(Flags.PUBLIC + Flags.STATIC + Flags.FINAL);

public static final
```
<a name="nPHgU"></a>
### `TreeMaker.ClassDef`
`TreeMaker.ClassDef`用于创建类定义语法树节点（`JCClassDecl`）,源码如下：
```java
public JCClassDecl ClassDef(JCModifiers mods,
    Name name,
    List<JCTypeParameter> typarams,
    JCExpression extending,
    List<JCExpression> implementing,
    List<JCTree> defs) {
        JCClassDecl tree = new JCClassDecl(mods,
                                     name,
                                     typarams,
                                     extending,
                                     implementing,
                                     defs,
                                     null);
        tree.pos = pos;
        return tree;
}
```

1. mods：访问标志，可以通过`TreeMaker.Modifiers`来创建
2. name：类名
3. typarams：泛型参数列表
4. extending：父类
5. implementing：实现的接口
6. defs：类定义的详细语句，包括字段、方法的定义等等
<a name="f0VSY"></a>
### `TreeMaker.MethodDef`
`TreeMaker.MethodDef`用于创建方法定义语法树节点（`JCMethodDecl`），源码如下
```java
public JCMethodDecl MethodDef(JCModifiers mods,
    Name name,
    JCExpression restype,
    List<JCTypeParameter> typarams,
    List<JCVariableDecl> params,
    List<JCExpression> thrown,
    JCBlock body,
    JCExpression defaultValue) {
        JCMethodDecl tree = new JCMethodDecl(mods,
                                       name,
                                       restype,
                                       typarams,
                                       params,
                                       thrown,
                                       body,
                                       defaultValue,
                                       null);
        tree.pos = pos;
        return tree;
}

public JCMethodDecl MethodDef(MethodSymbol m,
    Type mtype,
    JCBlock body) {
        return (JCMethodDecl)
            new JCMethodDecl(
                Modifiers(m.flags(), Annotations(m.getAnnotationMirrors())),
                m.name,
                Type(mtype.getReturnType()),
                TypeParams(mtype.getTypeArguments()),
                Params(mtype.getParameterTypes(), m),
                Types(mtype.getThrownTypes()),
                body,
                null,
                m).setPos(pos).setType(mtype);
}
```

1. mods：访问标志
2. name：方法名
3. restype：返回类型
4. typarams：泛型参数列表
5. params：参数列表
6. thrown：异常声明列表
7. body：方法体
8. defaultValue：默认方法（可能是interface中的哪个default）
9. m：方法符号
10. mtype：方法类型。包含多种类型，泛型参数类型、方法参数类型、异常参数类型、返回参数类型。

返回类型`restype`填写null或者`treeMaker.TypeIdent(TypeTag.VOID)`都代表返回void类型。
<a name="OAvHm"></a>
### `TreeMaker.VarDef`
`TreeMaker.VarDef`用于创建字段/变量定义语法树节点（`JCVariableDecl`），源码如下
```java
public JCVariableDecl VarDef(JCModifiers mods,
    Name name,
    JCExpression vartype,
    JCExpression init) {
        JCVariableDecl tree = new JCVariableDecl(mods, name, vartype, init, null);
        tree.pos = pos;
        return tree;
}

public JCVariableDecl VarDef(VarSymbol v,
    JCExpression init) {
        return (JCVariableDecl)
            new JCVariableDecl(
                Modifiers(v.flags(), Annotations(v.getAnnotationMirrors())),
                v.name,
                Type(v.type),
                init,
                v).setPos(pos).setType(v.type);
}
```

1. mods：访问标志
2. name：参数名称
3. vartype：类型
4. init：初始化语句
5. v：变量符号
<a name="jDb1q"></a>
### `TreeMaker.Ident`
`TreeMaker.Ident`用于创建标识符语法树节点（`JCIdent`），源码如下
```java
public JCIdent Ident(Name name) {
        JCIdent tree = new JCIdent(name, null);
        tree.pos = pos;
        return tree;
}

public JCIdent Ident(Symbol sym) {
        return (JCIdent)new JCIdent((sym.name != names.empty)
                                ? sym.name
                                : sym.flatName(), sym)
            .setPos(pos)
            .setType(sym.type);
}

public JCExpression Ident(JCVariableDecl param) {
        return Ident(param.sym);
}
```
<a name="GBBZB"></a>
### `TreeMaker.Return`
`TreeMaker.Return`用于创建`return`语句（`JCReturn`），源码如下
```java
public JCReturn Return(JCExpression expr) {
        JCReturn tree = new JCReturn(expr);
        tree.pos = pos;
        return tree;
}
```
<a name="VzJJd"></a>
### `TreeMaker.Select`
`TreeMaker.Select`用于创建域访问/方法访问（这里的方法访问只是取到名字，方法的调用需要用`TreeMaker.Apply`）语法树节点（`JCFieldAccess`），源码如下
```java
public JCFieldAccess Select(JCExpression selected,
    Name selector) 
{
        JCFieldAccess tree = new JCFieldAccess(selected, selector, null);
        tree.pos = pos;
        return tree;
}

public JCExpression Select(JCExpression base,
    Symbol sym) {
        return new JCFieldAccess(base, sym.name, sym).setPos(pos).setType(sym.type);
}
```

1. selected：.运算符左边的表达式
2. selector：.运算符右边的表达式

下面给出一个例子，一语句生成的Java语句就是二语句<br />一. `TreeMaker.Select(treeMaker.Ident(names.fromString("this")), names.fromString("name"));`<br />二. `this.name`
<a name="iEXDE"></a>
### `TreeMaker.NewClass`
`TreeMaker.NewClass`用于创建`new`语句语法树节点（`JCNewClass`）,源码如下：
```java
public JCNewClass NewClass(JCExpression encl,
    List<JCExpression> typeargs,
    JCExpression clazz,
    List<JCExpression> args,
    JCClassDecl def) {
        JCNewClass tree = new JCNewClass(encl, typeargs, clazz, args, def);
        tree.pos = pos;
        return tree;
}
```

1. encl：不太明白此参数的含义，很多例子中此参数都设置为null
2. typeargs：参数类型列表
3. clazz：待创建对象的类型
4. args：参数列表
5. def：类定义
<a name="EWY35"></a>
### `TreeMaker.Apply`
`TreeMaker.Apply`用于创建方法调用语法树节点（`JCMethodInvocation`），源码如下：
```java
public JCMethodInvocation Apply(List<JCExpression> typeargs,
    JCExpression fn,
    List<JCExpression> args) {
        JCMethodInvocation tree = new JCMethodInvocation(typeargs, fn, args);
        tree.pos = pos;
        return tree;
}
```

1. typeargs：参数类型列表
2. fn：调用语句
3. args：参数列表
<a name="VEJ1r"></a>
### `TreeMaker.Assign`
`TreeMaker.Assign`用户创建赋值语句语法树节点（`JCAssign`），源码如下：
```java
ublic JCAssign Assign(JCExpression lhs,
    JCExpression rhs) {
        JCAssign tree = new JCAssign(lhs, rhs);
        tree.pos = pos;
        return tree;
}
```

1. lhs：赋值语句左边表达式
2. rhs：赋值语句右边表达式
<a name="YUnCX"></a>
### `TreeMaker.Exec`
`TreeMaker.Exec`用于创建可执行语句语法树节点（`JCExpressionStatement`），源码如下：
```java
public JCExpressionStatement Exec(JCExpression expr) {
        JCExpressionStatement tree = new JCExpressionStatement(expr);
        tree.pos = pos;
        return tree;
}
```
`TreeMaker.Apply`以及`TreeMaker.Assign`就需要外面包一层`TreeMaker.Exec`来获得一个`JCExpressionStatement`。
<a name="LKtcE"></a>
### `TreeMaker.Block`
`TreeMaker.Block`用于创建组合语句的语法树节点（JCBlock），源码如下：
```java
public JCBlock Block(long flags,
    List<JCStatement> stats) {
        JCBlock tree = new JCBlock(flags, stats);
        tree.pos = pos;
        return tree;
}
```

1. flags：访问标志
2. stats：语句列表
<a name="xH6IG"></a>
## `com.sun.tools.javac.util.List`介绍
在操作抽象语法树的时候，有时会涉及到关于List的操作，但是这个List不是经常使用的`java.util.List`而是`com.sun.tools.javac.util.List`，这个List比较奇怪，是一个链式的结构，有头结点和尾节点，但是只有尾节点是一个List，这里作为了解就行了。
```java
public class List<A> extends AbstractCollection<A> implements java.util.List<A> {
    public A head;
    public List<A> tail;
    private static final List<?> EMPTY_LIST = new List<Object>((Object)null, (List)null) {
        public List<Object> setTail(List<Object> var1) {
            throw new UnsupportedOperationException();
        }

        public boolean isEmpty() {
            return true;
        }
    };

    List(A head, List<A> tail) {
        this.tail = tail;
        this.head = head;
    }

    public static <A> List<A> nil() {
        return EMPTY_LIST;
    }

    public List<A> prepend(A var1) {
        return new List(var1, this);
    }

    public List<A> append(A var1) {
        return of(var1).prependList(this);
    }

    public static <A> List<A> of(A var0) {
        return new List(var0, nil());
    }

    public static <A> List<A> of(A var0, A var1) {
        return new List(var0, of(var1));
    }

    public static <A> List<A> of(A var0, A var1, A var2) {
        return new List(var0, of(var1, var2));
    }

    public static <A> List<A> of(A var0, A var1, A var2, A... var3) {
        return new List(var0, new List(var1, new List(var2, from(var3))));
    }

    ...
}
```
<a name="crrWo"></a>
## `com.sun.tools.javac.util.ListBuffer`
由于`com.sun.tools.javac.util.List`使用起来不方便，所以又在其上面封装了一层，这个封装类是`ListBuffer`，此类的操作和平时经常使用的`java.util.List`用法非常类似。
```java
public class ListBuffer<A> extends AbstractQueue<A> {

    public static <T> ListBuffer<T> of(T x) {
        ListBuffer<T> lb = new ListBuffer<T>();
        lb.add(x);
        return lb;
    }

    /** The list of elements of this buffer.
     */
    private List<A> elems;

    /** A pointer pointing to the last element of 'elems' containing data,
     *  or null if the list is empty.
     */
    private List<A> last;

    /** The number of element in this buffer.
     */
    private int count;

    /** Has a list been created from this buffer yet?
     */
    private boolean shared;

    /** Create a new initially empty list buffer.
     */
    public ListBuffer() {
        clear();
    }

    /** Append an element to buffer.
     */
    public ListBuffer<A> append(A x) {
        x.getClass(); // null check
        if (shared) copy();
        List<A> newLast = List.<A>of(x);
        if (last != null) {
            last.tail = newLast;
            last = newLast;
        } else {
            elems = last = newLast;
        }
        count++;
        return this;
    }
    ........
}
```
<a name="hFbEQ"></a>
## com.sun.tools.javac.util.Names介绍
这个是为创建名称的一个工具类，无论是类、方法、参数的名称都需要通过此类来创建。它里面经常被使用到的一个方法就是`fromString()`，一般使用方法如下所示。
```java
Names names  = new Names()
names. fromString("setName");
```
<a name="THg1C"></a>
## 实战演练
上面大概了解了如何操作抽象语法树，接下来就来写几个真实的案例加深理解。
<a name="jaseP"></a>
### 变量相关
在类中经常操作的参数就是变量，那么如何使用抽象语法树的特性操作变量呢？接下来就将一些对于变量的一些操作。
<a name="os2bl"></a>
#### 生成变量
例如生成`private String age;`这样一个变量，借用上面讲的`VarDef`方法
```java
// 生成参数 例如：private String age;
treeMaker.VarDef(treeMaker.Modifiers(Flags.PRIVATE), names.fromString("age"), treeMaker.Ident(names.fromString("String")), null);
```
<a name="NCWDr"></a>
#### 对变量赋值
例如想生成`private String name = "BuXueWuShu"`，还是利用`VarDef`方法
```java
// private String name = "BuXueWuShu"
treeMaker.VarDef(treeMaker.Modifiers(Flags.PRIVATE),names.fromString("name"),treeMaker.Ident(names.fromString("String")),treeMaker.Literal("BuXueWuShu"))
```
<a name="F52Iw"></a>
#### 两个字面量相加
例如生成`String add = "a" + "b";`，借用上面讲的`Exec`方法和`Assign`方法
```java
// add = "a"+"b"
treeMaker.Exec(treeMaker.Assign(treeMaker.Ident(names.fromString("add")),treeMaker.Binary(JCTree.Tag.PLUS,treeMaker.Literal("a"),treeMaker.Literal("b"))))
```
<a name="qBeLO"></a>
#### +=语法
例如想生成`add += "test"`，则和上面字面量差不多。
```java
// add+="test"
treeMaker.Exec(treeMaker.Assignop(JCTree.Tag.PLUS_ASG, treeMaker.Ident(names.fromString("add")), treeMaker.Literal("test")))
```
<a name="EIjhJ"></a>
#### ++语法
例如想生成++i
```java
treeMaker.Exec(treeMaker.Unary(JCTree.Tag.PREINC,treeMaker.Ident(names.fromString("i"))))
```
<a name="J3z4U"></a>
### 方法相关
对于变量进行了操作，那么基本上都是要生成方法的，那么如何对方法进行生成和操作呢？接下来演示一下关于方法相关的操作方法。
<a name="WFXT9"></a>
#### 无参无返回值
可以利用上面讲到的`MethodDef`方法进行生成
```java
/*
    无参无返回值的方法生成
    public void test(){

    }
 */
// 定义方法体
ListBuffer<JCTree.JCStatement> testStatement = new ListBuffer<>();
JCTree.JCBlock testBody = treeMaker.Block(0, testStatement.toList());
    
JCTree.JCMethodDecl test = treeMaker.MethodDef(
        treeMaker.Modifiers(Flags.PUBLIC), // 方法限定值
        names.fromString("test"), // 方法名
        treeMaker.Type(new Type.JCVoidType()), // 返回类型
        com.sun.tools.javac.util.List.nil(),
        com.sun.tools.javac.util.List.nil(),
        com.sun.tools.javac.util.List.nil(),
        testBody, // 方法体
        null
);
```
<a name="KSlkK"></a>
#### 有参无返回值
可以利用上面讲到的`MethodDef`方法进行生成
```java
/*
    无参无返回值的方法生成
    public void test2(String name){
        name = "xxxx";
    }
 */
ListBuffer<JCTree.JCStatement> testStatement2 = new ListBuffer<>();
testStatement2.append(treeMaker.Exec(treeMaker.Assign(treeMaker.Ident(names.fromString("name")),treeMaker.Literal("xxxx"))));
JCTree.JCBlock testBody2 = treeMaker.Block(0, testStatement2.toList());

// 生成入参
JCTree.JCVariableDecl param = treeMaker.VarDef(treeMaker.Modifiers(Flags.PARAMETER), names.fromString("name"),treeMaker.Ident(names.fromString("String")), null);
com.sun.tools.javac.util.List<JCTree.JCVariableDecl> parameters = com.sun.tools.javac.util.List.of(param);

JCTree.JCMethodDecl test2 = treeMaker.MethodDef(
        treeMaker.Modifiers(Flags.PUBLIC), // 方法限定值
        names.fromString("test2"), // 方法名
        treeMaker.Type(new Type.JCVoidType()), // 返回类型
        com.sun.tools.javac.util.List.nil(),
        parameters, // 入参
        com.sun.tools.javac.util.List.nil(),
        testBody2,
        null
);
```
<a name="geNrK"></a>
#### 有参有返回值
```java
 /*
    有参有返回值
    public String test3(String name){
       return name;
    }
 */

ListBuffer<JCTree.JCStatement> testStatement3 = new ListBuffer<>();
testStatement3.append(treeMaker.Return(treeMaker.Ident(names.fromString("name"))));
JCTree.JCBlock testBody3 = treeMaker.Block(0, testStatement3.toList());

// 生成入参
JCTree.JCVariableDecl param3 = treeMaker.VarDef(treeMaker.Modifiers(Flags.PARAMETER), names.fromString("name"),treeMaker.Ident(names.fromString("String")), null);
com.sun.tools.javac.util.List<JCTree.JCVariableDecl> parameters3 = com.sun.tools.javac.util.List.of(param3);

JCTree.JCMethodDecl test3 = treeMaker.MethodDef(
        treeMaker.Modifiers(Flags.PUBLIC), // 方法限定值
        names.fromString("test4"), // 方法名
        treeMaker.Ident(names.fromString("String")), // 返回类型
        com.sun.tools.javac.util.List.nil(),
        parameters3, // 入参
        com.sun.tools.javac.util.List.nil(),
        testBody3,
        null
);
```
<a name="pyckC"></a>
### 特殊的
学完了如何进行定义参数，如何进行定义方法，其实还有好多语句需要学习，例如如何生成`new`语句，如何生成方法调用的语句，如何生成if语句。接下来就学习一些比较特殊的语法。
<a name="f0sQ7"></a>
#### new一个对象
```java
// 创建一个new语句 
CombatJCTreeMain combatJCTreeMain = new CombatJCTreeMain();
JCTree.JCNewClass combatJCTreeMain = treeMaker.NewClass(
        null,
        com.sun.tools.javac.util.List.nil(),
        treeMaker.Ident(names.fromString("CombatJCTreeMain")),
        com.sun.tools.javac.util.List.nil(),
        null
);
JCTree.JCVariableDecl jcVariableDecl1 = treeMaker.VarDef(
        treeMaker.Modifiers(Flags.PARAMETER),
        names.fromString("combatJCTreeMain"),
        treeMaker.Ident(names.fromString("CombatJCTreeMain")),
        combatJCTreeMain
);
```
<a name="cpcy5"></a>
#### 方法调用（无参）
```java
JCTree.JCExpressionStatement exec = treeMaker.Exec(
        treeMaker.Apply(
                com.sun.tools.javac.util.List.nil(),
                treeMaker.Select(
                        treeMaker.Ident(names.fromString("combatJCTreeMain")), // . 左边的内容
                        names.fromString("test") // . 右边的内容
                ),
                com.sun.tools.javac.util.List.nil()
        )
);
```
<a name="deTUi"></a>
#### 方法调用（有参）
```java
// 创建一个方法调用 combatJCTreeMain.test2("hello world!");
JCTree.JCExpressionStatement exec2 = treeMaker.Exec(
        treeMaker.Apply(
                com.sun.tools.javac.util.List.nil(),
                treeMaker.Select(
                        treeMaker.Ident(names.fromString("combatJCTreeMain")), // . 左边的内容
                        names.fromString("test2") // . 右边的内容
                ),
                com.sun.tools.javac.util.List.of(treeMaker.Literal("hello world!")) // 方法中的内容
        )
);
```
<a name="Roqo6"></a>
#### if语句
```java
/*
    创建一个if语句
    if("BuXueWuShu".equals(name)){
        add = "a" + "b";
    }else{
        add += "test";
    }
 */
// "BuXueWuShu".equals(name)
JCTree.JCMethodInvocation apply = treeMaker.Apply(
        com.sun.tools.javac.util.List.nil(),
        treeMaker.Select(
                treeMaker.Literal("BuXueWuShu"), // . 左边的内容
                names.fromString("equals") // . 右边的内容
        ),
        com.sun.tools.javac.util.List.of(treeMaker.Ident(names.fromString("name")))
);
//  add = "a" + "b"
JCTree.JCExpressionStatement exec3 = treeMaker.Exec(treeMaker.Assign(treeMaker.Ident(names.fromString("add")), treeMaker.Binary(JCTree.Tag.PLUS, treeMaker.Literal("a"), treeMaker.Literal("b"))));
//  add += "test"
JCTree.JCExpressionStatement exec1 = treeMaker.Exec(treeMaker.Assignop(JCTree.Tag.PLUS_ASG, treeMaker.Ident(names.fromString("add")), treeMaker.Literal("test")));

JCTree.JCIf anIf = treeMaker.If(
        apply, // if语句里面的判断语句
        exec3, // 条件成立的语句
        exec1  // 条件不成立的语句
);
```
