IDEA<br />分场景介绍一下IDEA的调试技巧，看完掌握后调试bug的效率一定大大提升。
<a name="nhJg6"></a>
### 1、条件断点
循环中经常用到这个技巧，比如：遍历1个大List的过程中，想让断点停在某个特定值。<br />![2021-09-04-13-15-29-972843.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732640903-cb9943a6-ac35-4dcd-9a19-f3c6cb03b9c2.png#clientId=u3d4540ac-bdfe-4&from=ui&id=uadd52cd6&originHeight=522&originWidth=908&originalType=binary&ratio=1&size=59891&status=done&style=none&taskId=ud89af5e8-2c02-4ee7-8329-4b465c8a671)<br />条件断点<br />参考上图，在断点的位置，右击断点旁边的小红点，会出来一个界面，在Condition这里填入断点条件即可，这样调试时，就会自动停在i=10的位置。<br />![2021-09-04-13-15-30-077859.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732640179-2c1b82d8-7123-444c-bcfe-3f11b0efddde.png#clientId=u3d4540ac-bdfe-4&from=ui&id=KjcFB&originHeight=200&originWidth=820&originalType=binary&ratio=1&size=24856&status=done&style=none&taskId=u1abd869f-992a-45e9-a925-18059f176f7)<br />条件断点调试
<a name="Br6sg"></a>
### 2、回到"上一步"
该技巧最适合特别复杂的方法套方法的场景，好不容易跑起来，一不小心手一抖，断点过去了，想回过头看看刚才的变量值，如果不知道该技巧，只能再跑一遍。<br />![2021-09-04-13-15-30-263853.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732641000-66a5163a-2d74-4135-8dfd-16ab67816373.png#clientId=u3d4540ac-bdfe-4&from=ui&id=E2I2i&originHeight=832&originWidth=1062&originalType=binary&ratio=1&size=111352&status=done&style=none&taskId=uad512a9c-e481-439a-bfc4-beb2338b0a1)<br />参考上图，method1方法调用method2，当前断点的位置j=100，点击上图红色箭头位置的Drop Frame图标后，时间穿越了<br />![2021-09-04-13-15-30-521847.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732670062-40cf589f-942b-4143-b7fd-ac697476c55f.png#clientId=u3d4540ac-bdfe-4&from=ui&id=u20b121b0&originHeight=890&originWidth=1024&originalType=binary&ratio=1&size=106877&status=done&style=none&taskId=ud77006ae-1d7b-48fa-b82c-349c76289aa)<br />退回上一步<br />回到了method1刚开始调用的时候，变量i变成了99，是不是很6 :)
:::tips
注：好奇心是人类进步的阶梯，如果想知道为啥这个功能叫Drop Frame，而不是类似Back To Previous 之类的，可以去翻翻JVM的书，JVM内部以栈帧为单位保存线程的运行状态，drop frame即扔掉当前运行的栈帧，这样当前“指针”的位置，就自然到了上一帧的位置。
:::
<a name="wRMOP"></a>
### 3、多线程调试
多线程同时运行时，谁先执行，谁后执行，完全是看CPU心情的，无法控制先后，运行时可能没什么问题，但是调试时就比较麻烦了，最明显的就是断点乱跳，一会儿停这个线程，一会儿停在另一个线程，比如下图：<br />![2021-09-04-13-15-30-801847.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732670085-bba09e1f-2a7d-4786-84d2-aaa4577337c9.png#clientId=u3d4540ac-bdfe-4&from=ui&id=JXJPz&originHeight=434&originWidth=818&originalType=binary&ratio=1&size=81999&status=done&style=none&taskId=u55fafbcc-3272-4fb1-8ce8-f25c4dd7d51)<br />如果想希望下一个断点位置是第2句诗句，可能要失望了：<br />![2021-09-04-13-15-31-049540.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732716224-3c079eab-d5cd-4070-891b-bcd9445e9fa3.png#clientId=u3d4540ac-bdfe-4&from=ui&id=ub4452931&originHeight=450&originWidth=748&originalType=binary&ratio=1&size=82735&status=done&style=none&taskId=u960dae18-0343-4060-8dd9-55c5a8dc8b2)<br />如果想让线程在调试时，想按自己的愿意来，让它停在哪个线程就停在哪个线程，可以在图中3个断点的小红点上右击，<br />![2021-09-04-13-15-31-183080.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732716219-40071951-640a-4e10-a344-298d1ee25286.png#clientId=u3d4540ac-bdfe-4&from=ui&id=nYBDI&originHeight=430&originWidth=852&originalType=binary&ratio=1&size=50614&status=done&style=none&taskId=u7d0f361c-6152-4049-9356-a4f3bc2fe4d)<br />即：Suspend挂起的条件是按每个线程来，而非All。把这3个断点都这么设置后，再来一次试试<br />![2021-09-04-13-15-31-392076.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732716405-f6b027e8-8ec3-49bd-90a2-e052b5e422c8.png#clientId=u3d4540ac-bdfe-4&from=ui&id=gDUP2&originHeight=976&originWidth=950&originalType=binary&ratio=1&size=213336&status=done&style=none&taskId=u19a0e61c-3054-49e7-9a00-3be6987976c)<br />注意上图中的红框位置，断点停下来时，这个下拉框可以看到各个线程（注：给线程起个容易识别的名字是个好习惯！），可以选择线程“天空中的飞鸟”<br />![2021-09-04-13-15-31-566080.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732752950-6bfe9544-3a13-4b35-89af-0cfcf81c5c5d.png#clientId=u3d4540ac-bdfe-4&from=ui&id=u284ab78b&originHeight=908&originWidth=1060&originalType=binary&ratio=1&size=151238&status=done&style=none&taskId=u11063404-00ba-4775-87db-5262e99ca00)<br />断点如愿停在了第2句诗。
<a name="AbSBi"></a>
### 4、远程调试
这也是一个装B的利器，本机不用启动项目，只要有源代码，可以在本机直接远程调试服务器上的代码，打开姿势如下：
<a name="NB0Xn"></a>
#### 4.1 项目启动时，先允许远程调试
```bash
java -server -Xms512m -Xmx512m -Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=9081 -Djava.ext.dirs=. ${main_class}
```
起作用的就是
```bash
-Xdebug -Xnoagent -Djava.compiler=NONE -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=9081
```
:::danger
注意：远程调试从技术上讲，就是在本机与远程建立scoket通讯，所以端口不要冲突，而且本机要允许访问远程端口，另外这一段参数，放要在`-jar` 或 `${main_class}`的前面
:::
<a name="KmFt1"></a>
#### 4.2 idea中设置远程调试
![2021-09-04-13-15-31-717083.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732752941-43a2f60f-ad7e-4537-bfea-b4627c20439b.png#clientId=u3d4540ac-bdfe-4&from=ui&id=feOwK&originHeight=609&originWidth=1080&originalType=binary&ratio=1&size=143584&status=done&style=none&taskId=u7b979311-e936-4a00-b750-2b720bd345d)<br />然后就可以调试了<br />![2021-09-04-13-15-31-831075.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732752851-2bbeefe3-7882-4d37-8274-1ee4795ee614.png#clientId=u3d4540ac-bdfe-4&from=ui&id=HsF6P&originHeight=106&originWidth=492&originalType=binary&ratio=1&size=9032&status=done&style=none&taskId=u41bb09e6-f29f-4153-aea8-d16686bb748)<br />前提是本机有项目的源代码 ，在需要的地方打个断点，然后访问一个远程的url试试，断点就会停下来。
<a name="NsVWf"></a>
### 5、临时执行表达式/修改变量的运行值
调试时，可以临时执行一些表达式，参考下图：点击这二个图标中的任何1个都可以<br />![2021-09-04-13-15-31-959077.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732776654-0e3b406d-e1a6-4db7-8994-7ab207c73218.png#clientId=u3d4540ac-bdfe-4&from=ui&id=ud0d2780b&originHeight=456&originWidth=1022&originalType=binary&ratio=1&size=41135&status=done&style=none&taskId=u5f73de3d-c7ee-4b9a-b9cc-e868c2a4b4e)<br />点击+号后，就可以在新出现的输入框里输入表达式，比如i+5<br />![2021-09-04-13-15-32-111078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732776665-abf79919-a338-4d0c-9a73-04481793e682.png#clientId=u3d4540ac-bdfe-4&from=ui&id=yLYOY&originHeight=330&originWidth=1052&originalType=binary&ratio=1&size=34568&status=done&style=none&taskId=ua4b115e7-40b3-411b-af8c-e932ed66917)<br />然后回车，马上就能看到结果<br />![2021-09-04-13-15-32-220080.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732776702-7ff19a2f-94a8-4a78-8489-b67805a95f5f.png#clientId=u3d4540ac-bdfe-4&from=ui&id=MwOes&originHeight=386&originWidth=960&originalType=binary&ratio=1&size=38224&status=done&style=none&taskId=u7bb9c32e-aabe-41dc-9708-71bff9041ad)<br />当然，如果调试时，想动态修改变量的值，也很容易，在变量上右击，然后选择Set Value。<br />![2021-09-04-13-15-33-455078.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1630732776890-f8e725f8-d3df-456a-b707-8699248b2038.png#clientId=u3d4540ac-bdfe-4&from=ui&id=U71FZ&originHeight=386&originWidth=834&originalType=binary&ratio=1&size=75875&status=done&style=none&taskId=u1940f7d6-d809-4108-b93f-32b5748773d)
