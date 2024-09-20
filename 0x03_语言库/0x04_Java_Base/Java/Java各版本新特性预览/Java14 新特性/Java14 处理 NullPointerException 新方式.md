Java
<a name="Lbfhh"></a>
## 1、传统的 NullPointerException
编码过程中，经常会使用链式调用的方式来写代码，这样写起来很方便，也很清晰，但是，一旦出现NullPointerException，那就头大了，因为很难知道异常是在什么时候开始发生的。<br />举个简单的例子，就比如下面的代码，要找到公司某个员工的户籍所在地，这样来调用
```java
String city = employee.getDetailInfos().getRegistryAddress().getCity();
```
在链式调用的过程中，如果employee, `getDetailInfos()`或者 `getRegistryAddress()` 为空，JVM就会抛出 NullPointerException<br />那么导致异常的根本原因是什么？如果不使用调试器，很难确定哪个变量为空。而且，JVM也只会打印导致异常的方法、文件名和行号，仅此而已。<br />那么下面来了解Java 14如何通过 JEP 358 解决这个问题。
<a name="xh4ZH"></a>
## 2、增强型 NullPointerException
SAP在2006年为其商业JVM实现了增强型的 NullPointerException。2019年2月，它被提议作为OpenJDK社区的一个增强，之后很快，它成为了一个JEP。所以，该功能在2019年10月完成并在JDK 14版本推出 。<br />本质上，JEP 358 旨在通过描述某个变量是 “null” 来提高 JVM 生成的 “NullPointerException” 的可读性。JEP 358通过在方法、文件名和行号旁边描述为 null 的变量，带来了一个详细的 NullPointerException 消息。它通过分析程序的字节码指令来工作。因此，它能够精确地确定哪个变量或表达式是null。<br />最重要的是，JDK 14中默认关闭详细的异常消息 。要启用它，需要使用命令行选项：
```bash
-XX:+ShowCodeDetailsInExceptionMessages
```
<a name="avu84"></a>
### 2.1 详细的异常信息
考虑在激活 ShowCodeDetailsInExceptionMessages 标志的情况下再次运行代码：
```
Exception in thread "main" java.lang.NullPointerException:
  Cannot invoke "RegistryAddress.getCity()" because the return value of
"com.developlee.java14.helpfulnullpointerexceptions.HelpfulNullPointerException$DetailInfos.getRegistryAddress()" is null
  at com.developlee.java14.helpfulnullpointerexceptions.HelpfulNullPointerException.main(HelpfulNullPointerException.java:10)
```
这一次，从附加信息中，可以知道员工的个人详细信息丢失的注册地址导致了异常。从这个增强中获得的信息可以节省调试所用的时间。<br />JVM由两部分组成详细的异常消息。第一部分表示失败的操作，这是引用为 _null_ 的结果，而第二部分标识了 _null_ 引用 的原因：
```
Cannot invoke "String.toLowerCase()" because the return value of "getEmailAddress()" is null
```
为了生成异常消息，JEP 358 重构了将空引用推送到操作数堆栈上的部分源代码。
<a name="Rxnx0"></a>
## 3、技术方面
现在已经很好地理解了如何使用增强的NullPointerExceptions标识 null 引用，来看看它的一些技术方面。<br />首先，只有当JVM本身抛出一个 NullPointerException 时，才会进行详细的消息计算，如果在Java代码中显式抛出异常，则不会执行计算。原因是因为：在这些情况下，很可能已经在异常构造函数中传递了一条有意义的消息。<br />其次，**JEP 358** 懒汉式地计算消息，这意味着只有当打印异常消息时才调用增强的NullPointerException，而不是当异常发生时就调用。因此，对于通常的JVM流程不应该有任何性能影响，在那里可以捕获并重新抛出异常，因为并不会只想打印异常消息。<br />最后，详细的异常消息可能包含源代码 中的局部变量名。因此，可以认为这是一个潜在的安全风险。但是，只有在运行使用激活的 `-g` 标记编译的代码时，才会发生这种情况，该标记会生成调试信息并将其添加到类文件中。请考虑一个简单的示例，已编译该示例以包含以下附加调试信息：
```java
Employee employee = null;
employee.getName();
```
当执行以上代码时，异常信息中会打印本地变量名称：
```
"com.developlee.java14.helpfulnullpointerexceptions.HelpfulNullPointerException$Employee.getName()"
because "employee" is null
```
相反，在没有额外调试信息的情况下，JVM 只提供它在详细消息中所知道的变量：
```
Cannot invoke
  "com.developlee.java14.helpfulnullpointerexceptions.HelpfulNullPointerException$Employee.getName()"
because "<local1>" is null
```
JVM 打印编译器分配的变量索引，而不是本地变量名（employee）。<br />关于NullPointerException的处理到这里就结束了，通过Java14增强的NullPointerException，可以很快速的定位代码问题的原因所在，更快的调试代码，节约时间，提高效率。
