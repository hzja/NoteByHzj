IDEA
<a name="mKHtJ"></a>
### 1、Debug简介和意义
<a name="bBQed"></a>
#### 什么是程序DeBug？

- Debug，是程序开发人员必会的一项调试程序的技能。
- 企业中程序开发和程序调试的比例为1:1.5，可以说如果不会调试程序，就没有办法从事编程工作。
<a name="nzF3d"></a>
#### Debug能做什么？

1. 追踪代码的运行流程。
2. 程序运行异常定位。
3. 线上问题追踪。
<a name="MDwtU"></a>
#### Debug对于程序学习者的意义

1. 通过调试能够更好的查看程序的执行流程。
2. 复杂的程序逻辑，通过老师的口述讲解，很难理解清楚，这个时候借助调试能够很好的帮助同学们理解程序。
3. 定位问题，提高解决问题的能力。
<a name="KUQG7"></a>
### 2、IDEA中的Debug步骤
<a name="wV239"></a>
#### 设置断点（F9）

- 调试程序（8个按钮）

![2021-10-06-13-10-02-193064.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633497428718-158ffb22-89c0-4617-a57b-fcc3532aeaf7.jpeg#clientId=ude8e8b72-9aec-4&from=ui&id=u8dc8fc53&originHeight=44&originWidth=340&originalType=binary&ratio=1&size=2988&status=done&style=none&taskId=u1d3929a8-7cd7-4045-b199-5337164d64d)

| 按钮 | 说明 |
| --- | --- |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982209-e61fe059-59bd-475c-a4d0-bde66a00b857.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u1f84dd69&originHeight=39&originWidth=43&originalType=url&ratio=1&status=done&style=none&taskId=u322b23c2-bc39-4c01-9631-65fa029aab1) | (Alt + F10)：如果光标在其它行或其它页面，点击这个按钮可跳转到当前代码执行的行 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982159-683367fb-c886-477d-9e7b-355465595599.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u250b174a&originHeight=36&originWidth=48&originalType=url&ratio=1&status=done&style=none&taskId=uf8925045-25bc-4c92-9562-ca31f75718b) | (F8)：步过，一行一行地往下走，如果这一行上有方法不会进入方法。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982213-c8d6d291-9423-4dd1-80c5-4494be5c18bc.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u6a6a88a6&originHeight=38&originWidth=48&originalType=url&ratio=1&status=done&style=none&taskId=u9bdfe52f-8d40-4cad-84ff-dc64b497431) | (F7)：步入。如果当前行有方法，可以进入方法内部，一般用于进入自定义方法内，不会进入官方类库的方法。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982185-be8c8623-fc80-4036-aaf0-5dd49bbf7601.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u9cf8e467&originHeight=39&originWidth=52&originalType=url&ratio=1&status=done&style=none&taskId=uf73ffcb7-b816-4c8c-bfd6-3b7ace4235d) | (Alt + Shift + F7)：强制步入，能进入任何方法，查看底层源码的时候可以用这个进入官方类库的方法。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982458-1b8f0478-ad44-46f3-8fff-74d6fafb3538.webp#clientId=ude8e8b72-9aec-4&from=paste&id=ufd2945a7&originHeight=38&originWidth=48&originalType=url&ratio=1&status=done&style=none&taskId=u7833aee3-73b8-4157-b6d9-7d2f1b50465) | (Shift + F8)：步出，从步入的方法内退出到方法调用处，此时方法已执行完毕，只是还没有完成赋值。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982538-15459f47-cdea-40b7-adf9-e55bf35b9229.webp#clientId=ude8e8b72-9aec-4&from=paste&height=70&id=u923fc821&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=uc6f65096-11d0-4c6c-8a99-f42fddacbe0&width=70) | 回退断点。回退到当前方法的调用处。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982584-6b2dbd71-36de-4118-870a-2fc43f894bc1.webp#clientId=ude8e8b72-9aec-4&from=paste&id=uebe6c67e&originHeight=37&originWidth=46&originalType=url&ratio=1&status=done&style=none&taskId=u60fe52df-3fad-4b81-a018-da151e279ef) | (Alt + F9)：运行到光标处，可以将光标定位到需要查看的那一行，然后使用这个功能，代码会运行至光标行，而不需要打断点。 |
| ![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496982617-5fa1b4fb-efb6-4135-8b64-433e83b60a7f.webp#clientId=ude8e8b72-9aec-4&from=paste&height=70&id=ueb6565cb&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u94748213-d20e-4d4e-b19a-2b55d31c67e&width=70) | (Alt + F8)：计算表达式。 |

<a name="h1L6O"></a>
#### 观察变量
![2021-10-06-13-10-03-986612.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633497417411-401036fa-e2fd-4299-8982-a4449ef26a78.jpeg#clientId=ude8e8b72-9aec-4&from=ui&id=u8f281202&originHeight=492&originWidth=1080&originalType=binary&ratio=1&size=52226&status=done&style=shadow&taskId=u0e1ae46b-fa3b-4a19-b4c9-2163556a805)

- 程序区查看变量
- Debugger的Variables中查看变量
- 鼠标悬停到变量名上会弹出当前变量的值
- 查看变量有三种方式：
- 查看输出
<a name="cZC50"></a>
### 3、跳转到当前代码执行的行
<a name="lZEoY"></a>
#### 跳转到当前代码执行的行（Alt + F10）
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496983048-afba5c57-1bfa-42b1-8d06-3b04f3733f34.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u4d01d2ef&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=ufa7bfe37-f1ac-47a9-a9de-2e653929089)
<a name="y3btX"></a>
#### 作用

- 使程序窗口切换到当前正在运行的程序处。
<a name="CXmWi"></a>
### 4、步过调试的使用
<a name="yGPk8"></a>
#### 步过调试按钮（F8）
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496983155-f3badabf-b477-45da-a3d1-14dc2c3fa634.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u8d8d49f8&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u09cc8eda-0d02-4b4e-9b8b-306270e788a)
<a name="Ev1yv"></a>
#### 作用

- 步过，一行一行地往下走，如果这一行上有方法不会进入方法。
- 常用于调试过程中不想进入调用的方法体的情况。
<a name="kRA5E"></a>
### 5、步入调试的使用
<a name="F5RvS"></a>
#### 步入调试

- 步过调试按钮（F7）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496983175-8f9c98b6-ad1b-40f7-88af-6c1efda12d7e.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u6e942522&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u6c02acd2-135b-4150-8eb0-871affa40d9)
<a name="x2Zwg"></a>
#### 作用

- 步入，一行一行地往下走，如果这一行上有方法，则进入方法内部。
- 一般用于进入自定义方法内，不会进入官方类库的方法。
<a name="zn0LK"></a>
### 6、强制步入调试的使用
<a name="skQsc"></a>
#### 强制步入调试

   - 强制步入调试按钮（Alt + Shift + F7）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496983841-0a6c7491-1f6a-4de1-8e4e-8694a50d167b.webp#clientId=ude8e8b72-9aec-4&from=paste&id=ub90e1700&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u9c1532c8-ffd3-4a36-af91-7cd60b9c402)
<a name="uZOhZ"></a>
#### 作用

- 进入官方类库方法
- 帮助学习和查看JDK源码
<a name="smvi0"></a>
### 7、步出调试的使用
<a name="lmxQO"></a>
#### 步出调试

- 步出调试按钮（Shift + F8）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496983961-6e206baa-03b2-41cf-ad5f-2c8e90b27147.webp#clientId=ude8e8b72-9aec-4&from=paste&id=ue489c86b&originHeight=38&originWidth=48&originalType=url&ratio=1&status=done&style=none&taskId=ubc4c69f6-eda8-41a3-9b4a-b11595f0b5a)
<a name="ztCvX"></a>
#### 作用

- 从方法内退出到方法调用处。
- 调试的时候，有时候会跳入到自己不想查看的方法体，这个时候使用步出。
<a name="fTPeE"></a>
### 8、回退断点调试的使用
<a name="cJErs"></a>
#### 回退断点

- 回退断点按钮 

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496984689-066a371f-2625-4eaa-92b5-0fe47ac8fed4.webp#clientId=ude8e8b72-9aec-4&from=paste&id=ua515532e&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u44e95be8-791e-4b31-8966-0222e6a6a17)
<a name="UC7Z3"></a>
#### 作用

- 回退到当前方法的调用处。
- 当想重新查看该方法体的执行过程时，不用重新启动Debug，可以使用回退断点方式。
<a name="OtCTo"></a>
### 9、运行到光标处
<a name="Ks7u5"></a>
#### 行到光标处

- 使程序运行到光标处，而无需设置断点。
- 运行光标处按钮 （F9）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496984824-27727cd3-3194-453a-92b0-d6af6990ae2b.webp#clientId=ude8e8b72-9aec-4&from=paste&id=u277e7a50&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u25290ea1-7bfd-4c47-a88e-7f83020ab1b)

- 作用
<a name="SJj7F"></a>
### 10、计算表达式
<a name="fKsfa"></a>
#### 计算表达式

- 计算表达式按钮（Alt + F8）

![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1633496984805-429ec4f2-85b9-4e2f-a571-6b2f822a87ec.webp#clientId=ude8e8b72-9aec-4&from=paste&id=uf9cf2775&originHeight=140&originWidth=140&originalType=url&ratio=1&status=done&style=none&taskId=u6b7c33d7-cac0-47a0-a085-bff283ba9e7)<br />![2021-10-06-13-10-05-192416.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633497445588-d1e22f7c-25bd-4d5e-8c3e-a6c5d0986ce0.jpeg#clientId=ude8e8b72-9aec-4&from=ui&id=ucd1df463&originHeight=628&originWidth=1080&originalType=binary&ratio=1&size=72466&status=done&style=shadow&taskId=uf7e73951-ca59-4105-a107-c6cedc51542)
<a name="du6Z0"></a>
#### 作用

- 设置变量，在计算表达式的框里，可以改变变量的值，这样有时候就能很方便去调试各种值的情况了。
<a name="W4hGD"></a>
### 11、条件断点
<a name="TNfht"></a>
#### 条件断点

- 右键单击断点处，可以设置进入断点的条件

![2021-10-06-13-10-05-406384.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633497463173-a590c8d2-8e10-44df-a4eb-4947f5d22c93.jpeg#clientId=ude8e8b72-9aec-4&from=ui&id=u4892a378&originHeight=793&originWidth=1080&originalType=binary&ratio=1&size=74841&status=done&style=shadow&taskId=u811cea36-73b6-4f9a-8ba9-54a1e788ab9)
<a name="orYu8"></a>
#### 作用

- 通过设置断点条件，在满足条件时，才停在断点处，否则直接运行。
<a name="crO6z"></a>
### 12、多线程调试
<a name="gOq8r"></a>
#### 步骤

- 多线程调试，需要调整断点挂起级别为Thread

![2021-10-06-13-10-05-606148.jpeg](https://cdn.nlark.com/yuque/0/2021/jpeg/396745/1633497463233-72f4eb07-db2f-4531-b101-732d11cd5107.jpeg#clientId=ude8e8b72-9aec-4&from=ui&id=XZQtP&originHeight=771&originWidth=1080&originalType=binary&ratio=1&size=73582&status=done&style=shadow&taskId=uabbea23d-542b-424c-bbfc-07b430fa99a)

- Frame中选择线程进行调试
