IDEA
<a name="D3PI5"></a>
# 1、控制台无法键入内容
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582021812282-a4369e07-e27a-41db-999b-c2c36489cfd3.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=128567&status=done&style=none&width=1366)
```java
@Test
public void testScanner() throws Exception{
       Scanner scanner = new Scanner(System.in);
       String line = scanner.nextLine();
       System.out.println(line);
}
```
<a name="oKHbn"></a>
# 2、解决办法
<a name="dIFpu"></a>
## A.在菜单栏Help -> Edit Custom VM Options
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582023226235-3e4c5f03-3956-459d-932c-3864c20888af.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=142916&status=done&style=none&width=1366)
<a name="yRUf3"></a>
## B.打开VM配置文件添加配置参数
```
-Deditable.java.test.console=true
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1582023391066-e90b30bd-e7f2-47ca-9a3a-18c8fc221234.png#align=left&display=inline&height=728&originHeight=728&originWidth=1366&size=120693&status=done&style=none&width=1366)
<a name="e2dMN"></a>
## C.重启IDEA即可解决
