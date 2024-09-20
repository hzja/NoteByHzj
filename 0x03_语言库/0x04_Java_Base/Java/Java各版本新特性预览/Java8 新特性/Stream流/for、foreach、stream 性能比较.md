Java for foreach stream
<a name="CkolZ"></a>
### 10000数据
<a name="PbsDa"></a>
#### for循环
首先，10000数据的for循环，代码如下：
```java
public class ForTest {

    public static void main(String[] args) {
        Long startTime = System.currentTimeMillis();
        formMethod();
        Long endTime = System.currentTimeMillis();
        System.out.println("time_total:" + (endTime - startTime));
    }

    public static void formMethod(){
        for (int i = 0; i < 10000; i++) {
            System.out.println("__________for循环____________");
        }
    }
}
```
测试结果：<br />![2021-05-21-15-30-20-244835.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582444051-807837dd-1d3a-4348-b2c6-37b4d48942ee.png#clientId=ue62a7f14-562b-4&from=ui&id=u461eb288&originHeight=319&originWidth=846&originalType=binary&size=24870&status=done&style=shadow&taskId=u20cca322-6274-45d4-b738-f7a57bf7d70)<br />运行多次，时间基本落在100内，90左右。
<a name="f3Izs"></a>
#### foreach循环
同样数据量，代码如下：
```java
public class ForTest {

    public static void main(String[] args) {
        List<Integer> lists = new ArrayList<>();
        for (int i = 0; i < 10000; i++) {
            lists.add(i);
        }
        Long startTime = System.currentTimeMillis();
        formMethod(lists);
        Long endTime = System.currentTimeMillis();
        System.out.println("time_total:" + (endTime - startTime));
    }

    public static void formMethod(List<Integer> lists){
        lists.forEach(i->{
            System.out.println("__________forEach____________");
        });
    }
}
```
测试结果：<br />![2021-05-21-15-30-20-379494.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582453362-08fd527e-147f-4a05-8a0d-499af4a2b661.png#clientId=ue62a7f14-562b-4&from=ui&id=u7f2348d6&originHeight=358&originWidth=850&originalType=binary&size=31510&status=done&style=none&taskId=ua4abb5f0-96fe-4b30-8929-2cbfe25b536)<br />运行多次，时间基本落在150左右。增强型效果不如for循环~
<a name="yqXW3"></a>
#### Stream
同样数据量，代码如下：
```java
public class ForTest {

    public static void main(String[] args) {
        List<Integer> lists = new ArrayList<>();
        for (int i = 0; i < 10000; i++) {
            lists.add(i);
        }
        Long startTime = System.currentTimeMillis();
        formMethod(lists);
        Long endTime = System.currentTimeMillis();
        System.out.println("time_total:" + (endTime - startTime));
    }

    public static void formMethod(List<Integer> lists){
        lists.stream().forEach(i->{
            System.out.println("__________stream处理____________");
        });
    }
}
```
测试结果：<br />![2021-05-21-15-30-21-422670.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582466780-f36fc6d8-ac1f-4d73-bb6c-3c98840ae0f6.png#clientId=ue62a7f14-562b-4&from=ui&id=u30ab32dc&originHeight=310&originWidth=741&originalType=binary&size=22392&status=done&style=none&taskId=u6259b868-b90b-471b-80fe-8c955d866ca)<br />基本和增强型for循环效率差别不大。
:::success
结论：1万以内的数据，for循环的性能要高于foreach和stream；
:::
<a name="gZtbY"></a>
### 1000万数据
数据加到1000万，代码不变，看结果：
<a name="MpZb9"></a>
#### for循环
![2021-05-21-15-30-21-531380.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582481853-3d3d2d29-040c-44e9-ba64-23d4cfe2b4b0.png#clientId=ue62a7f14-562b-4&from=ui&id=u5f26e2cc&originHeight=355&originWidth=878&originalType=binary&size=31847&status=done&style=none&taskId=u1b45fa8b-8e53-43ee-adc3-927735ae19d)<br />时间落在43240附近。
<a name="jHjFl"></a>
#### foreach循环
![2021-05-21-15-30-21-678982.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582489778-ee7e4837-1d0b-4f7f-9935-d9718c706404.png#clientId=ue62a7f14-562b-4&from=ui&id=u06a31bc3&originHeight=359&originWidth=850&originalType=binary&size=34901&status=done&style=none&taskId=ue1a3277a-b3f5-4fcf-90b0-071deef5559)<br />基本和for循环效率差别不大。
<a name="kWbOY"></a>
#### Stream
![2021-05-21-15-30-21-861496.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1621582496875-d5048d5b-8330-471e-8a99-9624dcca5346.png#clientId=ue62a7f14-562b-4&from=ui&id=u0700981c&originHeight=358&originWidth=836&originalType=binary&size=34615&status=done&style=none&taskId=ud2a7f897-9219-4383-bfbe-5c124484b9b)<br />基本和for循环，增强型for循环效率差别不大。<br />Stream的优势在于，提供了并行处理（`parallelStream()`方法），即stream api提供了异步处理机制，可以充分利用CPU核数，大大提升效率！
:::success
结论：数据量上去之后，测试三种遍历方式，基本已经没有什么差距了，但是Stream提供并行处理，在数据量大了之后，效率会明显增强。（但是单核CPU，Stream并行处理可能会效率更慢）
:::
