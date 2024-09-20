Java<br />这里说的是Spring的`BeanUtils.copyProperties`。
<a name="x79FQ"></a>
## 场景
开发中经常遇到，把父类的属性拷贝到子类中。通常有2种方法：

- 一个一个set
- 用`BeanUtils.copyProperties`

很显然`BeanUtils`更加方便，也美观很多。<br />那么任何情况都能使用`BeanUtils`么，当然不是。要先了解他。
<a name="TRSzx"></a>
## `BeanUtils`是深拷贝，还是浅拷贝?
是浅拷贝。

- 浅拷贝：只是调用子对象的set方法，并没有将所有属性拷贝。(也就是说，引用的一个内存地址)
- 深拷贝：将子对象的属性也拷贝过去。
<a name="TaksV"></a>
## 什么情况适合用`BeanUtils`
如果都是单一的属性，那么不涉及到深拷贝的问题，适合用BeanUtils。
<a name="ltAPy"></a>
### 有子对象就一定不能用`BeanUtils`么
并不绝对，这个要区分考虑：

- 子对象还要改动。
- 子对象不怎么改动。

虽然有子对象，但是子对象并不怎么改动，那么用BeanUtils也是没问题的。
<a name="wah0Z"></a>
## 代码例子
下面用代码说明下。

- 翠山有个儿子无忌，儿子继承了他的face和height。
- 但是life应该是自己的。
- 后来翠山自刎而死，无忌也变成dead状态了。这就是浅拷贝，无忌用的life引用的翠山的life对象。

Father类：
```java
@Data
public class Father {
    private String face; // 长相
    private String height; // 身高
    private Life life; // 生命
}
```
Life 类：
```java
@Data
public class Life {
    private String status;
}
```
Son类和main方法：
```java
@Data
public class Son extends Father{
    private Life life;

    public static void main(String[] args) {
        Father cuishan = new Father();
        cuishan.setFace("handsome");
        cuishan.setHeight("180");
        Life cuishanLife = new Life();
        cuishanLife.setStatus("alive");
        cuishan.setLife(cuishanLife);
        Son wuji=new Son();
        BeanUtils.copyProperties(cuishan,wuji);

//        Life wujiLife = wuji.getLife();
//        wujiLife.setStatus("alive");
//        wuji.setLife(wujiLife);
//        cuishanLife.setStatus("dead"); // 翠山后来自刎了

        System.out.println(JSON.toJSONString(cuishan));
        System.out.println(JSON.toJSONString(wuji));
    }
}
```
上面注释出的代码可以如下替换：<br />case1和case2还是受浅拷贝的影响，case3不受。<br />case1：翠山自刎，无忌也挂了
```java
//        Life wujiLife = wuji.getLife();
//        wujiLife.setStatus("alive");
//        wuji.setLife(wujiLife);
//        cuishanLife.setStatus("dead"); // 翠山后来自刎了
```
case2：翠山自刎，无忌设置或者，翠山也活了
```java
//        cuishanLife.setStatus("dead"); // 翠山后来自刎了
//        Life wujiLife = wuji.getLife();
//        wujiLife.setStatus("alive");
//        wuji.setLife(wujiLife);
```
case3：翠山和无忌互不影响
```java
cuishanLife.setStatus("dead"); // 翠山自刎了  该行放在上下均可
// 无忌用个新对象 不受翠山影响了
Life wujiLife = new Life();
wujiLife.setStatus("alive");
wuji.setLife(wujiLife);
```
<a name="w5RK4"></a>
## dest ,src 还是 src,dest
常见的BeanUtils有2个：

- Spring有BeanUtils
- apache的commons也有BeanUtils。

区别如下：<br />![2021-08-19-00-03-30-804601.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1629302830472-e4f3a322-b2a9-4831-9885-c0b41dfd3b3a.png#clientId=u1f257dc1-5e7d-4&from=ui&id=ubb9a281b&originHeight=94&originWidth=769&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3057&status=done&style=shadow&taskId=uc301d40a-5a0d-46f1-a546-bbd11811866&title=)<br />这2个用哪个都行，但是要注意区别。因为他们2个的src和dest是正好相反的，要特别留意。
