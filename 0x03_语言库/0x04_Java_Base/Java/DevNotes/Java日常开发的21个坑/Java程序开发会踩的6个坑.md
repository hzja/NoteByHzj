Java
<a name="ZQb76"></a>
## 1、用==号比较的坑
在项目中有没有见过，有些同事对`Integer`类型的两个参数使用==号比较是否相等？<br />那么这种用法对吗？<br />回答是看具体场景，不能说一定对，或不对。<br />有些状态字段，比如：orderStatus有：-1(未下单)，0（已下单），1（已支付），2（已完成），3（取消），5种状态。<br />这时如果用==判断是否相等：
```java
Integer orderStatus1 = new Integer(1);
Integer orderStatus2 = new Integer(1);
System.out.println(orderStatus1 == orderStatus2);
```
返回结果会是true吗？<br />答案：是false。<br />有些同学可能会反驳，Integer中不是有范围是：-128-127的缓存吗？<br />为什么是false？<br />先看看Integer的构造方法：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657875635906-3dd8a0df-8724-41e9-81ab-ae1a0b9dbe1f.jpeg#averageHue=%23504130&clientId=u1692c0bd-1982-4&from=paste&id=u0c298e89&originHeight=130&originWidth=472&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u8de6eef4-aee4-4eec-9caf-849b7da973e&title=)<br />它其实并没有用到缓存。<br />那么缓存是在哪里用的？<br />答案在`valueOf`方法中：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657875635850-b944bfb2-3d8e-4ae8-9e44-81ba1ce47739.jpeg#averageHue=%23463b2d&clientId=u1692c0bd-1982-4&from=paste&id=u7500ee42&originHeight=202&originWidth=892&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf83bc0c3-70f8-4930-be3b-1155f6cca94&title=)<br />如果上面的判断改成这样：
```java
String orderStatus1 = new String("1");
String orderStatus2 = new String("1");
System.out.println(Integer.valueOf(orderStatus1) == Integer.valueOf(orderStatus2));
```
返回结果会是true吗？<br />答案：还真是true。<br />要养成良好编码习惯，尽量少用`==`判断两个`Integer`类型数据是否相等，只有在上述非常特殊的场景下才相等。<br />而应该改成使用`equals`方法判断：
```java
Integer orderStatus1 = new Integer(1);
Integer orderStatus2 = new Integer(1);
System.out.println(orderStatus1.equals(orderStatus2));
```
运行结果为true。
<a name="NYt3w"></a>
## 2、`Objects.equals`的坑
假设现在有这样一个需求：判断当前登录的用户，如果是指定的系统管理员，则发送一封邮件。系统管理员没有特殊的字段标识，他的用户id=888，在开发、测试、生产环境中该值都是一样的。<br />这个需求真的太容易实现了：
```java
UserInfo userInfo = CurrentUser.getUserInfo();

if(Objects.isNull(userInfo)) {
   log.info("请先登录");
   return;
}

if(Objects.equals(userInfo.getId(),888L)) {
   sendEmail(userInfo):
}
```
从当前登录用户的上下文中获取用户信息，判断一下，如果用户信息为空，则直接返回。<br />如果获取到的用户信息不为空，接下来判断用户id是否等于888。

- 如果等于888，则发送邮件。
- 如果不等于888，则啥事也不干。

当用id=888的系统管理员账号登录之后，做了相关操作，满怀期待的准备收邮件的时候，却发现收了个寂寞。<br />后来，发现UserInfo类是这样定义的：
```java
@Data
public class UserInfo {
    private Integer id;
    private String name;
    private Integer age;
    private String address;
}
```
此时，有些小伙伴可能会说：没看出什么问题呀。<br />但要说的是这个代码确实有问题。<br />什么问题呢？<br />下面重点看看它的`equals`方法：
```java
public static boolean equals(Object a, Object b) {
    return (a == b) || (a != null && a.equals(b));
}
```
`equals`方法的判断逻辑如下：

1. 该方法先判断对象a和b的引用是否相等，如果相等则直接返回true。
2. 如果引用不相等，则判断a是否为空，如果a为空则返回false。
3. 如果a不为空，调用对象的`equals`方法进一步判断值是否相等。

这就要从`Integer`的`equals`方法说起来了。<br />它的equals方法具体代码如下：
```java
public boolean equals(Object obj) {
    if (obj instanceof Integer) {
        return value == ((Integer)obj).intValue();
    }
    return false;
}
```
先判断参数obj是否是Integer类型，如果不是，则直接返回false。如果是Integer类型，再进一步判断int值是否相等。<br />而上面这个例子中b是long类型，所以Integer的equals方法直接返回了false。<br />也就是说，如果调用了Integer的equals方法，必须要求入参也是Integer类型，否则该方法会直接返回false。<br />除此之外，还有Byte、Short、Double、Float、Boolean和Character也有类似的equals方法判断逻辑。<br />常见的坑有：

1. Long类型和Integer类型比较，比如：用户id的场景。
2. Byte类型和Integer类型比较，比如：状态判断的场景。
3. Double类型和Integer类型比较，比如：金额为0的判断场景。
<a name="wV3gy"></a>
## 3、BigDecimal的坑
通常会把一些小数类型的字段（比如：金额），定义成`BigDecimal`，而不是`Double`，避免丢失精度问题。<br />使用Double时可能会有这种场景：
```java
double amount1 = 0.02;
double amount2 = 0.03;
System.out.println(amount2 - amount1);
```
正常情况下预计amount2 - amount1应该等于0.01<br />但是执行结果，却为：
```java
0.009999999999999998
```
实际结果小于预计结果。<br />Double类型的两个参数相减会转换成二进制，因为Double有效位数为16位这就会出现存储小数位数不够的情况，这种情况下就会出现误差。<br />常识告诉我们使用BigDecimal能避免丢失精度。<br />但是使用BigDecimal能避免丢失精度吗？<br />答案是否定的。<br />为什么？
```java
BigDecimal amount1 = new BigDecimal(0.02);
BigDecimal amount2 = new BigDecimal(0.03);
System.out.println(amount2.subtract(amount1));
```
这个例子中定义了两个`BigDecimal`类型参数，使用构造函数初始化数据，然后打印两个参数相减后的值。<br />结果：
```java
0.0099999999999999984734433411404097569175064563751220703125
```
不科学呀，为啥还是丢失精度了？<br />Jdk中BigDecimal的构造方法上有这样一段描述：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657875635807-972df606-154f-45b8-a9b9-1ae5f4ec53b7.jpeg#averageHue=%23384634&clientId=u1692c0bd-1982-4&from=paste&id=ucdb70517&originHeight=228&originWidth=908&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u29d2a461-b51a-43bf-a9c0-5fce28269dd&title=)<br />大致的意思是此构造函数的结果可能不可预测，可能会出现创建时为0.1，但实际是0.1000000000000000055511151231257827021181583404541015625的情况。<br />由此可见，使用BigDecimal构造函数初始化对象，也会丢失精度。<br />那么，如何才能不丢失精度呢？
```java
BigDecimal amount1 = new BigDecimal(Double.toString(0.02));
BigDecimal amount2 = new BigDecimal(Double.toString(0.03));
System.out.println(amount2.subtract(amount1));
```
可以使用`Double.toString`方法，对double类型的小数进行转换，这样能保证精度不丢失。<br />其实，还有更好的办法：
```java
BigDecimal amount1 = BigDecimal.valueOf(0.02);
BigDecimal amount2 = BigDecimal.valueOf(0.03);
System.out.println(amount2.subtract(amount1));
```
使用`BigDecimal.valueOf`方法初始化`BigDecimal`类型参数，也能保证精度不丢失。在新版的阿里巴巴开发手册中，也推荐使用这种方式创建`BigDecimal`参数。
<a name="agWcO"></a>
## 4、Java8 filter的坑
对于Java8中的Stream用法，大家肯定再熟悉不过了。<br />通过对集合的Stream操作，可以实现：遍历集合、过滤数据、排序、判断、转换集合等等，N多功能。<br />这里重点说说数据的过滤。<br />在没有Java8之前，过滤数据一般是这样做的：
```java
public List<User> filterUser(List<User> userList) {
    if(CollectionUtils.isEmpty(userList)) {
        return Collections.emptyList();
    }
    
    List<User> resultList = Lists.newArrayList();
    for(User user: userList) {
        if(user.getId() > 1000 && user.getAge() > 18)   {
           resultList.add(user);
        }
    }
    return resultList;
}
```
通常需要另一个集合辅助完成这个功能。<br />但如果使用Java8的`filter`功能，代码会变得简洁很多，例如：
```java
public List<User> filterUser(List<User> userList) {
    if(CollectionUtils.isEmpty(userList)) {
        return Collections.emptyList();
    }
    
    return userList.stream()
    .filter(user -> user.getId() > 1000 && user.getAge() > 18)
    .collect(Collectors.toList());
}
```
代码简化了很多，完美。<br />但如果对过滤后的数据，做修改了：
```java
List<User> userList = queryUser();
List<User> filterList = filterUser(userList);
for(User user: filterList) {
   user.setName(user.getName() + "测试");
}

for(User user: userList) {
   System.out.println(user.getName());
}
```
当时可能只是想修改过滤后的数据，但实际上，会把元素数据一同修改了。<br />意不意外，惊不惊喜？<br />其根本原因是：过滤后的集合中，保存的是对象的引用，该引用只有一份数据。<br />也就是说，只要有一个地方，把该引用对象的成员变量的值，做修改了，其他地方也会同步修改。<br />如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1657875635876-19536eee-31a3-4562-a62c-5efdff2d209d.png#averageHue=%23c1c4a1&clientId=u1692c0bd-1982-4&from=paste&id=u878e6944&originHeight=594&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u9ab1696a-e2b0-48c2-b262-4c61972f803&title=)
<a name="vth4M"></a>
## 5、自动拆箱的坑
Java5之后，提供了自动装箱和自动拆箱的功能。<br />自动装箱是指：JDK会把基本类型，自动变成包装类型。<br />比如：
```java
Integer integer = 1;
```
等价于：
```java
Integer integer = new Integer(1);
```
而自动拆箱是指：JDK会把包装类型，自动转换成基本类型。<br />例如：
```java
Integer integer = new Integer(2);
int sum = integer + 5;
```
等价于：
```java
Integer integer = new Integer(2);
int sum = integer.intValue() + 5;
```
但实际工作中，在使用自动拆箱时，往往忘记了判空，导致出现`NullPointerException`异常。
<a name="jc147"></a>
### 5.1 运算
很多时候，需要对传入的数据进行计算，例如：
```java
public class Test2 {
    public static void main(String[] args) {
        System.out.println(add(new Integer(1), new Integer(2)));
    }

    private static Integer add(Integer a, Integer b) {
        return a + b;
    }
}
```
如果传入了null值：
```java
System.out.println(add(null, new Integer(2)));
```
则会直接报错。
<a name="GmDet"></a>
### 5.2 传参
有时候，定义的某个方法是基本类型，但实际上传入了包装类，比如：
```java
public static void main(String[] args) {
    Integer a = new Integer(1);
    Integer b = null;
    System.out.println(add(a, b));
}

private static Integer add(int a, int b) {
    return a + b;
}
```
如果出现add方法报`NullPointerException`异常，可能会懵逼，int类型怎么会出现空指针异常呢？<br />其实，这个问题出在：Integer类型的参数，其实际传入值为null，JDK字段拆箱，调用了它的`intValue`方法导致的问题。
<a name="ldFWq"></a>
## 6、`replace`的坑
很多时候在使用字符串时，想把字符串比如：ATYSDFA*Y中的字符A替换成字符B，第一个想到的可能是使用`replace`方法。<br />如果想把所有的A都替换成B，很显然可以用replaceAll方法，因为非常直观，光从方法名就能猜出它的用途。<br />那么问题来了：`replace`方法会替换所有匹配字符吗？<br />jdk的官方给出了答案。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1657875635815-0c854152-1e8e-49ae-8152-9bb8740fcc40.jpeg#averageHue=%23394634&clientId=u1692c0bd-1982-4&from=paste&id=u05cf4fba&originHeight=224&originWidth=1026&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3e1a5c54-91e2-428a-97b3-9645c92e07e&title=)<br />该方法会替换每一个匹配的字符串。<br />既然`replace`和`replaceAll`都能替换所有匹配字符，那么他们有啥区别呢？<br />`replace`有两个重载的方法。

- 其中一个方法的参数：char oldChar 和 char newChar，支持字符的替换。
```java
source.replace('A', 'B')
```

- 另一个方法的参数是：CharSequence target 和 CharSequence replacement，支持字符串的替换。
```java
source.replace("A", "B")
```
而`replaceAll`方法的参数是：String regex 和 String replacement，即基于正则表达式的替换。<br />例如对普通字符串进行替换：
```java
source.replaceAll("A", "B")
```
使用正则表达替换（将*替换成C）：
```java
source.replaceAll("\\*", "C")
```
顺便说一下，将`*`替换成`C`使用replace方法也可以实现：
```java
source.replace("*", "C")
```
小伙们看到看到二者的区别了没？使用replace方法无需对特殊字符进行转义。<br />不过，千万注意，切勿使用如下写法：
```java
source.replace("\\*", "C")
```
这种写法会导致字符串无法替换。<br />还有个小问题，如果只想替换第一个匹配的字符串该怎么办?<br />这时可以使用`replaceFirst`方法：
```java
source.replaceFirst("A", "B")
```
说实话，这里内容都很基础，但越基础的东西，越容易大意失荆州，更容易踩坑。
