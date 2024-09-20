Java JDK<br />JDK作为每天必备的调用类库，里面大量提供了基础类供开发者使用。可以说离开jdk，Java代码寸步难行，jdk带来的便利可谓是不胜枚举，但同时这些方法在使用起来也存在一些坑，如果不注意就很容易掉入到陷阱里面，导致程序抛出错误。
<a name="ofWoF"></a>
## 一、`String.valueOf()`方法的陷阱
`String.valueOf()`是String提供的一个类型转换的方法，来看一下(代码简化过后的):
```java
// 调用用户服务根据用户id获取用户信息
Map<String, Object> userInfo = userService.getUserInfoById(userId);
Object userNameObject = userInfo.get("name");
String userName = String.valueOf(userNameObject);
// 判空
if(userName!=null&&userName.length()>0) {
    String message = getMessage(userName);
    smsService.send(message);
}
```
这段代码是简化过的，主要作用就是通过用户服务根据id获取用户信息发送短信，后来经过定位发现了问题所在：首先用户的名字里有特殊的emoji符号，数据库写入的时候有部分写入失败，因为当时的<br />数据库字符格式并无法兼容emoji，而获取的时候因为这个问题值为null了，接下来是重点：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568094906-3555830b-8321-4086-8405-b73d4172ba59.webp#averageHue=%232a2d2a&height=90&id=cEImJ&originHeight=90&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />这里是重点，也是最大的坑人之处，注意这里返回了一个"null"的字符串，而不是null。这两个是有很大区别的，当进行非空判断的时候，返回的是ture。也就是这个"null"的字符串它是符合判空条件的！<br />正确的姿势是在`String.valueOf`方法前必须判空：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568094900-9a4c2e45-bfa6-40aa-8988-5af302098735.webp#averageHue=%232a2c29&height=104&id=zJQ4F&originHeight=104&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="l98Yo"></a>
## 二、`Integer.parseInt()`方法的坑
`Integer.parseInt()`方法用于将字符串转化为Integer类型的方法，此方法的适用方向就显得比较窄，因为是String类型的参数，没有任何限定，当在传入一些比如50.0、20L、30d、40f这类数据的情况下。<br />来看一个栗子:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095011-f154af8d-af28-4888-9c38-3798999c600f.webp#averageHue=%232b2b2b&height=94&id=N7d89&originHeight=94&originWidth=1074&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1074)<br />会抛出异常NumberFormatException：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568094923-8d4c1f10-29a1-4ac6-b12e-3a4c0b411ddc.webp#averageHue=%232b2c27&height=141&id=ja2E9&originHeight=141&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />事实上对于这样的数据，比如小数、浮点数据、long型数据它都可以自动转换，而不是抛出烦人的报错信息，如果预先知道是整数或者小数，可以用Bigdecimal转换(注意此方法不适用于double和float、Long类型的数据，比如10d,20L)<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095047-13527b99-e9e1-4d16-9ec3-9812bb5110ac.webp#averageHue=%23333738&height=124&id=q9LUU&originHeight=124&originWidth=980&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=980)<br />对于浮点类型、long类型的数据可以用以下方法来处理:<br />推荐使用hutool的`NumberUtil.parseInt()`方法，充分考虑到了浮点、long、小数等类型数据可能带来的解析异常的问题。
<a name="cL2rA"></a>
## 三、Bigdecimal的除法
BigDecimal是处理金额最有效的数据类型，一般进行财务报表计算的时候为了防止金额出现错误，一般情况下都会采用Bigdecimal，而double、float都会存在些许的误差。用Bigdecimal进行了计算，而最终的结果返回却有问题，来看一个栗子:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095014-9edc6bba-97ef-4a61-a1c6-04f84e80ea6e.webp#averageHue=%233c3d34&height=146&id=poFLO&originHeight=146&originWidth=1010&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1010)<br />常见的除法用起来没有任何丝毫的问题，妥妥的没毛病，但是一旦程序中的数据出现以下情况，如果用Bigdecimal来接受前端的参数，而前端的参数是用户输入不确定的，一旦出现如下的数据，来看看结果:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568094944-454a8e98-2615-42fa-aadd-63665c613629.webp#averageHue=%233b3f42&height=134&id=mzWcM&originHeight=134&originWidth=1002&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1002)<br />执行结果一看，居然报错了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095068-b529c1f5-fcd0-4ba8-95a8-9594e75e9583.webp#averageHue=%23293338&height=101&id=wlHfU&originHeight=101&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />这就是BigDecimal的坑，一旦返回的结果是无限循环小数，就会抛出ArithmeticException。因此在进行Bigdecimal除法的时候，需要进行保留小数的处理，正确的处理姿势：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095122-fbd4a118-7ca2-4b78-b32e-0babe2b2938f.webp#averageHue=%232b2d2b&height=145&id=yPcpO&originHeight=145&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)
<a name="cbP1V"></a>
## 四、`Collections.emptyList()`此list非彼list
先来看一个sample:
```java
public List<String> getUserNameList(String userId) {
    List<String> resultList = Collections.emptyList();
    try {
        resultList = userDao.getUserName(userId);
    } catch (Exception ex) {
        logger.info(ex);
    }
    return resultList;
}
```
这样会抛出错误，主要问题在于`Collections.emptyList()`并非平时看到的List，此list不支持`add`、`remove`方法，否则会抛出operationNotSupportException:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095092-9416080d-5811-4eae-ab46-a2cc66480944.webp#averageHue=%23373b32&height=90&id=fsFJn&originHeight=90&originWidth=892&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=892)<br />结果抛出异常:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095034-2a5e9834-1af0-49fc-b7d8-fb2c31bc3b6c.webp#averageHue=%232b2f2c&height=126&id=Kd402&originHeight=126&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />原因是：`Collections.emptyList`返回的并不是平时认识的那个list，它是一个内部常量类：
```java
public static final List EMPTY_LIST = new EmptyList<>();
```
这个list并不具有add、remove元素的能力。
<a name="pQuGK"></a>
## 五、list可以一边删除一边遍历吗
答案是肯定可以的，要不然的话list怎么删除数据呢，不过要注意遍历的姿势，再来看一个简单的例子:<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095097-f315cd6e-b5a3-4db7-814a-1d73a7c02fa0.webp#averageHue=%2335373a&height=366&id=S0z4C&originHeight=366&originWidth=1022&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1022)<br />很不幸，又双叒叕报错了：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568094986-ec1303ff-ee16-4514-94a5-e1d27f2f4236.webp#averageHue=%232a2e2b&height=124&id=DO9WX&originHeight=124&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=1080)<br />仔细翻阅源码会发现，每次remove之前会检查元素的条数，如果发现预期的`modCount`和当前的`modCount`不一致就会抛出这个异常。`modCount`是list中用来记录修改次数的一个属性，当对元素进行统计的时候就会对该元素加1，而当对list边遍历边删除的话，就会造成excepted与`modCount`不一致，从而抛出异常。<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095096-944781e9-3efc-460d-b995-a3ba93c0ac8c.webp#averageHue=%232b2c2e&height=81&id=Z64gf&originHeight=81&originWidth=553&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=553)<br />正确的删除姿势就是使用`Iterator.remove`进行遍历删除，可以规避这个问题。
```java
List<Integer> list = new ArrayList<>();
list.add(1);
list.add(2);
list.add(3);
list.add(4);
Iterator<Integer> iterator = list.iterator();
while (iterator.hasNext()) {
    Integer integer = iterator.next();
    if (integer == 2) {
        iterator.remove();
    }
}
```
<a name="r1pao"></a>
## 六、总结
JDK的设计者有两个很大的特点:<br />①大多不会做非null判断<br />②出现错误直接`throw new Exception`，容错性很差。<br />在实际开发中，面对jdk一定要谨慎使用，jdk提供了便利的同时，也有一些使用上的盲区，应该养成多看源码，多注意错误性处理，防止在小问题上栽大跟头。
<a name="hO0cx"></a>
## 七、补充
<a name="IiJPb"></a>
### 7.1 Bigdecimal在比较的时候，最好使用compareTo方法，不要使用equals方法
如下案例，虽然Bigdecimal重写了equals方法，但是使用会存在问题：<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616568095154-33b7b4e4-cdd2-4751-af20-f477d86fa76a.png#averageHue=%232d2c2b&height=79&id=S1rSX&originHeight=79&originWidth=906&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=906)![](https://cdn.nlark.com/yuque/0/2021/png/396745/1616568095123-9259f6e7-5f3a-4bfe-b5ac-74be993aeb4e.png#averageHue=%23323232&height=52&id=I6Oxh&originHeight=52&originWidth=377&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=377)<br />1和1.0在比较的时候返回了false，这是因为在equals的源码中进行了数据的scale(也就是精度)的比较，如果不一致就会返回false，如果使用`compareTo`方法就不存在这个问题
<a name="DEwUq"></a>
### 7.2 MySQL的减法计算如果有null值结果就为null
`select 5-null`  结果会返回null，所以在进行MySQL计算的时候，对于有可能出现null值的列一定要进行`ifnull(field,0)`的转换，将null值转化为0，否则就会出现一些意想不到的数据错误和空指针问题<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095047-7f3af570-11bc-449f-acb9-a9f2f39a938d.webp#averageHue=%23dbebf5&height=135&id=gOZpX&originHeight=135&originWidth=424&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=424)<br />正确的操作是：<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095034-71f03158-d5d5-4977-b4e6-7b0cef3a7008.webp#averageHue=%23e5f1ef&height=132&id=ZLOpK&originHeight=132&originWidth=531&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=shadow&title=&width=531)
<a name="mRV4M"></a>
### 7.3 String的`split`方法在进行`||`分割的时候需要进行转义，否则结果会有问题
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095167-eb4f87d9-929b-45fb-bbdb-7d8ed660aea0.webp#averageHue=%23343731&height=304&id=Avqua&originHeight=304&originWidth=635&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=635)![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1616568095110-3a52d4a2-510c-43ab-9043-d945aceea7e9.webp#averageHue=%23313131&height=53&id=qGEVC&originHeight=53&originWidth=465&originalType=binary&ratio=1&rotation=0&showTitle=false&size=0&status=done&style=none&title=&width=465)
