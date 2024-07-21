加密算法Bcrypt
<a name="gTVDa"></a>
## 哈希（Hash）与加密（Encrypt）
哈希（Hash）是将目标文本转换成具有相同长度的、不可逆的杂凑字符串（或叫做消息摘要），而加密（Encrypt）是将目标文本转换成具有不同长度的、可逆的密文。

- 哈希算法往往被设计成生成具有相同长度的文本，而加密算法生成的文本长度与明文本身的长度有关。
- 哈希算法是不可逆的，而加密算法是可逆的。

HASH 算法是一种消息摘要算法，不是一种加密算法，但由于其单向运算，具有一定的不可逆性，成为加密算法中的一个构成部分。<br />JDK的String的Hash算法。代码如下：
```java
public int hashCode() {
    int h = hash;
    if (h == 0 && value.length > 0) {
        char val[] = value;
        for (int i = 0; i < value.length; i++) {
            h = 31 * h + val[i];
        }
        hash = h;
    }
    return h;
}
```
从JDK的API可以看出，它的算法等式就是`s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]`，其中`s[i]`就是索引为i的字符，n为字符串的长度。<br />HashMap的hash计算时先计算`hashCode()`，然后进行二次hash。代码如下：
```java
// 计算二次Hash    
int hash = hash(key.hashCode());

static int hash(int h) {
    h ^= (h >>> 20) ^ (h >>> 12);
	return h ^ (h >>> 7) ^ (h >>> 4);
}
```
可以发现，虽然算法不同，但经过这些移位操作后，对于同一个值使用同一个算法，计算出来的hash值一定是相同的。<br />**那么，hash为什么是不可逆的呢？**<br />假如有两个密码3和4，加密算法很简单就是3+4，结果是7，但是通过7不可能确定那两个密码是3和4，有很多种组合，这就是最简单的不可逆，所以只能通过暴力破解一个一个的试。<br />在计算过程中原文的部分信息是丢失了。一个MD5理论上是可以对应多个原文的，因为MD5是有限多个而原文是无限多个的。
<a name="jrmJx"></a>
## 不可逆的MD5为什么是不安全的？
因为hash算法是固定的，所以同一个字符串计算出来的hash串是固定的，所以，可以采用如下的方式进行破解。

1. **暴力枚举法**：简单粗暴地枚举出所有原文，并计算出它们的哈希值，看看哪个哈希值和给定的信息摘要一致。
2. **字典法**：黑客利用一个巨大的字典，存储尽可能多的原文和对应的哈希值。每次用给定的信息摘要查找字典，即可快速找到碰撞的结果。
3. **彩虹表**（rainbow）法：在字典法的基础上改进，以时间换空间。是现在破解哈希常用的办法。

对于单机来说，暴力枚举法的时间成本很高（以14位字母和数字的组合密码为例，共有1.24×10^25种可能，即使电脑每秒钟能进行10亿次运算，也需要4亿年才能破解），字典法的空间成本很高（仍以14位字母和数字的组合密码为例，生成的密码32位哈希串的对照表将占用5.7×10^14 TB的存储空间）。但是利用分布式计算和分布式存储，仍然可以有效破解MD5算法。因此这两种方法同样被黑客们广泛使用。
<a name="c6f5f"></a>
## 如何防御彩虹表的破解？
虽然彩虹表有着如此惊人的破解效率，但网站的安全人员仍然有办法防御彩虹表。最有效的方法就是“加盐”，即在密码的特定位置插入特定的字符串，这个特定字符串就是“盐（Salt）”，加盐后的密码经过哈希加密得到的哈希串与加盐前的哈希串完全不同，黑客用彩虹表得到的密码根本就不是真正的密码。即使黑客知道了“盐”的内容、加盐的位置，还需要对H函数和R函数进行修改，彩虹表也需要重新生成，因此加盐能大大增加利用彩虹表攻击的难度。<br />一个网站，如果加密算法和盐都泄露了，那针对性攻击依然是非常不安全的。因为同一个加密算法同一个盐加密后的字符串仍然还是一毛一样滴！
<a name="Elbbq"></a>
## 一个更难破解的加密算法Bcrypt
BCrypt是由Niels Provos和David Mazières设计的密码哈希函数，他是基于Blowfish密码而来的，并于1999年在USENIX上提出。<br />除了加盐来抵御rainbow table 攻击之外，bcrypt的一个非常重要的特征就是自适应性，可以保证加密的速度在一个特定的范围内，即使计算机的运算能力非常高，可以通过增加迭代次数的方式，使得**加密速度变慢**，从而可以抵御**暴力搜索**攻击。<br />Bcrypt可以简单理解为它内部自己实现了**随机加盐**处理。使用Bcrypt，每次加密后的密文是不一样的。<br />对一个密码，Bcrypt每次生成的hash都不一样，那么它是如何进行校验的？

1. 虽然对同一个密码，每次生成的hash不一样，但是hash中包含了salt（hash产生过程：先随机生成salt，salt跟password进行hash）；
2. 在下次校验时，从hash中取出salt，salt跟password进行hash；得到的结果跟保存在DB中的hash进行比对。

在Spring Security 中 内置了`Bcrypt`加密算法，构建也很简单，代码如下：
```java
@Bean
public PasswordEncoder passwordEncoder(){
    return new BCryptPasswordEncoder();
}
```
生成的加密字符串格式如下：
```
$2b$[cost]$[22 character salt][31 character hash]
```
比如：
```
$2a$10$N9qo8uLOickgx2ZMRZoMyeIjZAgcfl7p92ldGxad68LJZdL17lhWy
\__/\/ \____________________/\_____________________________/
 Alg Cost      Salt                        Hash
```
上面例子中，$2a$ 表示的hash算法的唯一标志。这里表示的是Bcrypt算法。<br />10 表示的是代价因子，这里是**2的10次方**，也就是1024轮。<br />N9qo8uLOickgx2ZMRZoMye 是16个字节（128bits）的salt经过base64编码得到的22长度的字符。<br />最后的IjZAgcfl7p92ldGxad68LJZdL17lhWy是24个字节（192bits）的hash，经过bash64的编码得到的31长度的字符。
<a name="uCWMU"></a>
## `PasswordEncoder` 接口
这个接口是Spring Security 内置的，如下：
```java
public interface PasswordEncoder {
    String encode(CharSequence rawPassword);

    boolean matches(CharSequence rawPassword, String encodedPassword);

    default boolean upgradeEncoding(String encodedPassword) {
        return false;
    }
}
```
这个接口有三个方法：

- `encode`方法接受的参数是原始密码字符串，返回值是经过加密之后的hash值，hash值是不能被逆向解密的。这个方法通常在为系统添加用户，或者用户注册的时候使用。
- `matches`方法是用来校验用户输入密码`rawPassword`，和加密后的hash值`encodedPassword`是否匹配。如果能够匹配返回true，表示用户输入的密码`rawPassword`是正确的，反之返回fasle。也就是说虽然这个hash值不能被逆向解密，但是可以判断是否和原始密码匹配。这个方法通常在用户登录的时候进行用户输入密码的正确性校验。
- `upgradeEncoding`设计的用意是，判断当前的密码是否需要升级。也就是是否需要重新加密？需要的话返回true，不需要的话返回fasle。默认实现是返回false。

**例如，可以通过如下示例代码在进行用户注册的时候加密存储用户密码**
```java
//将User保存到数据库表，该表包含password列
user.setPassword(passwordEncoder.encode(user.getPassword()));
```
`BCryptPasswordEncoder` 是Spring Security推荐使用的`PasswordEncoder`接口实现类
```java
public class PasswordEncoderTest {
    @Test
    void bCryptPasswordTest(){
        PasswordEncoder passwordEncoder =  new BCryptPasswordEncoder();
        String rawPassword = "123456";  //原始密码
        String encodedPassword = passwordEncoder.encode(rawPassword); //加密后的密码

        System.out.println("原始密码" + rawPassword);
        System.out.println("加密之后的hash密码:" + encodedPassword);

        System.out.println(rawPassword + "是否匹配" + encodedPassword + ":"   //密码校验：true
                           + passwordEncoder.matches(rawPassword, encodedPassword));

        System.out.println("654321是否匹配" + encodedPassword + ":"   //定义一个错误的密码进行校验:false
                           + passwordEncoder.matches("654321", encodedPassword));
    }
}
```
上面的测试用例执行的结果是下面这样的。（注意：**对于同一个原始密码，每次加密之后的hash密码都是不一样的，这正是**`**BCryptPasswordEncoder**`**的强大之处**，它不仅不能被破解，想通过常用密码对照表进行大海捞针都无从下手），输出如下：
```
原始密码123456
加密之后的hash密码:$2a$10$zt6dUMTjNSyzINTGyiAgluna3mPm7qdgl26vj4tFpsFO6WlK5lXNm
123456是否匹配$2a$10$zt6dUMTjNSyzINTGyiAgluna3mPm7qdgl26vj4tFpsFO6WlK5lXNm:true
654321是否匹配$2a$10$zt6dUMTjNSyzINTGyiAgluna3mPm7qdgl26vj4tFpsFO6WlK5lXNm:false
```
BCrypt **产生随机盐**（盐的作用就是每次做出来的菜味道都不一样）。这一点很重要，因为这意味着每次encode将产生不同的结果。
