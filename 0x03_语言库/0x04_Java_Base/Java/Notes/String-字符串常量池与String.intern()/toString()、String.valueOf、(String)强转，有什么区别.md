Java String
<a name="BstT3"></a>
## 一、代码实例
<a name="mZMdG"></a>
### 1、基本类型
（1）基本类型没有`toString()`方法<br />![2021-09-22-23-48-28-911769.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325922511-696ceae0-7228-4697-be5b-5f5a8ab0bb82.png#clientId=ud963913f-73dc-4&from=ui&id=u0f000888&originHeight=403&originWidth=1080&originalType=binary&ratio=1&size=1308310&status=done&style=shadow&taskId=u4db82e20-f983-48c6-aec3-851b679d8da)<br />（2）推荐使用<br />![2021-09-22-23-48-29-186769.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325923668-48c052c5-3d84-4f50-8b4c-5e9820f66646.png#clientId=ud963913f-73dc-4&from=ui&id=trW2a&originHeight=697&originWidth=1080&originalType=binary&ratio=1&size=2262701&status=done&style=shadow&taskId=ue4befac8-6c5d-4ddc-8bc3-2458f7c50a7)<br />（3）无法强转<br />![2021-09-22-23-48-29-365770.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325923916-f82efecc-50a7-499a-9c32-c3d284c3176b.png#clientId=ud963913f-73dc-4&from=ui&id=m474T&originHeight=878&originWidth=1080&originalType=binary&ratio=1&size=2850276&status=done&style=shadow&taskId=u8d4ba68e-0274-4df0-ba60-9fd550fda4c)<br />（String）是标准的类型转换，将Object类型转为String类型，使用(String)强转时，最好使用instanceof做一个类型检查，以判断是否可以进行强转，否则容易抛出ClassCastException异常。需要注意的是编写的时候，编译器并不会提示有语法错误，所以这个方法要谨慎的使用。<br />![2021-09-22-23-48-29-558772.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325949335-69802e88-37dc-491e-80d5-fb8b4114ddf5.png#clientId=ud963913f-73dc-4&from=ui&id=u7d893181&originHeight=413&originWidth=1080&originalType=binary&ratio=1&size=91802&status=done&style=shadow&taskId=uea138e1a-921c-4f9f-937f-9f33bbcd89f)<br />`instanceof`判断<br />![2021-09-22-23-48-29-683773.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325950633-4c89e644-6a10-4608-86cc-d8c8c00d397d.png#clientId=ud963913f-73dc-4&from=ui&id=Jvn6A&originHeight=748&originWidth=1055&originalType=binary&ratio=1&size=2372080&status=done&style=shadow&taskId=uf3a79cb1-c6c1-4b0a-8d98-c6a71c07abb)
<a name="frSNC"></a>
### 2、封装类型
（1）toString ok<br />![2021-09-22-23-48-29-845771.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632325950366-53f46d98-2180-45a9-b6cf-57c084352d5a.png#clientId=ud963913f-73dc-4&from=ui&id=hwCnE&originHeight=507&originWidth=1080&originalType=binary&ratio=1&size=1645921&status=done&style=shadow&taskId=u2218d896-63c7-46b6-b669-cee414be983)<br />（2）`String.valueOf()`<br />自然也是可以的。<br />（3）封装类型也无法强转<br />![2021-09-22-23-48-30-112768.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326001159-45a0967f-57c5-4424-a190-02bd14919a65.png#clientId=ud963913f-73dc-4&from=ui&id=u30904f18&originHeight=874&originWidth=1080&originalType=binary&ratio=1&size=2837288&status=done&style=shadow&taskId=u80ba631b-4a29-40b8-a0cf-e6e021826c7)
<a name="l4oCx"></a>
### 3、null值问题
<a name="jBYkZ"></a>
#### （1）`toString()`报空指针异常
![2021-09-22-23-48-30-264772.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326074354-65d8f902-e62d-4184-bb4e-9ba1b1f4322d.png#clientId=ud963913f-73dc-4&from=ui&id=rN8wk&originHeight=521&originWidth=1080&originalType=binary&ratio=1&size=1691372&status=done&style=shadow&taskId=u36e6058a-52fc-4ee5-b178-15057d47427)
<a name="RjOs6"></a>
#### （2）`String.valueOf()`返回字符串“null”
![2021-09-22-23-48-30-649776.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326074553-560237b5-875f-4143-ab0d-def3538cd6a7.png#clientId=ud963913f-73dc-4&from=ui&id=kV916&originHeight=721&originWidth=1010&originalType=binary&ratio=1&size=2188957&status=done&style=shadow&taskId=u08db7e2f-1e29-438d-8c8f-28b33b06015)
<a name="cfm71"></a>
#### （3）null值强转成功
![2021-09-22-23-48-30-788789.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326111220-f0a1b415-6562-4233-b253-9fdb5d70655e.png#clientId=ud963913f-73dc-4&from=ui&id=u41ffeed1&originHeight=480&originWidth=1038&originalType=binary&ratio=1&size=1497693&status=done&style=shadow&taskId=u697e3b7c-aef7-4d7f-9544-50e5e8a95b1)
<a name="p7R9b"></a>
## 二、源码分析
<a name="KVMfS"></a>
### 1、`toString()`
![2021-09-22-23-48-30-936824.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326110991-f45b86d6-7c20-46af-b50f-bc0c647383e1.png#clientId=ud963913f-73dc-4&from=ui&id=izGcS&originHeight=313&originWidth=1080&originalType=binary&ratio=1&size=1016143&status=done&style=shadow&taskId=ubadeabc6-af83-4b67-9e7a-dc10dbef752)<br />![2021-09-22-23-48-31-103716.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326111261-9f798ec1-aff1-443a-a79e-4807c92af11c.png#clientId=ud963913f-73dc-4&from=ui&id=ydBR2&originHeight=481&originWidth=1080&originalType=binary&ratio=1&size=1561520&status=done&style=shadow&taskId=u71d08ab6-1928-4696-8737-025c8019f98)
<a name="unuKk"></a>
### 2、`String.valueOf()`
![2021-09-22-23-48-31-222715.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1632326138523-78077ece-2565-4649-a3de-00950417b01d.png#clientId=ud963913f-73dc-4&from=ui&id=ub218e11d&originHeight=339&originWidth=1080&originalType=binary&ratio=1&size=1100556&status=done&style=none&taskId=u85788c8b-c395-4cf6-b4a9-0fa93acf69e)<br />`String.valueOf()`比`toString`多了一个非空判断。
<a name="Gp5zI"></a>
## 三、总结
<a name="jl2w5"></a>
### 1、`toString()`，可能会抛空指针异常
在这种使用方法中，因为java.lang.Object类里已有`public`方法`.toString()`，所以java对象都可以调用此方法。但在使用时要注意，必须保证object不是null值，否则将抛出`NullPointerException`异常。采用这种方法时，通常派生类会覆盖`Object`里的`toString()`方法。
<a name="ccsiK"></a>
### 2、`String.valueOf()`，推荐使用，返回字符串“null”
`String.valueOf()`方法是推荐使用的，因为它不会出现空指针异常，而且是静态的方法，直接通过String调用即可，只是有一点需要注意，就是上面提到的，如果为null，`String.valueOf()`返回结果是字符串“null”。而不是null。
<a name="qJabE"></a>
### 3、(String)强转，不推荐使用
（String）是标准的类型转换，将Object类型转为String类型，使用(String)强转时，最好使用`instanceof`做一个类型检查，以判断是否可以进行强转，否则容易抛出ClassCastException异常。需要注意的是编写的时候，编译器并不会提示有语法错误，所以这个方法要谨慎的使用。
