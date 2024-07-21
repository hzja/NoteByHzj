<a name="lYXb6"></a>
## idea的调试常规方法

> F7是会进入当前方法体进行调试
> F8是进入下一步，不会进入方法
> F9跳入下一个断点调试。
> alt+Shift+F7 ， 进入下一步，如果当前断点是一个方法，方法还有方法则循环进入
> 当你f7跟进函数，发现它不是你要找的，你可以用shift+f8跳出这个方法回到上一层继续找
> 2.如果想恢复Ctrl+z 掉的内容，快捷键为：Ctrl + Shift + Z


<a name="wo6XE"></a>
## idea远程调试
本地打断点，不用重新点击运行。<br />直接在Run/Debug Config设置中如下配置<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1625498698395-6ceee8db-76f4-474a-b956-ff2b0afff484.png#align=left&display=inline&height=173&originHeight=345&originWidth=1339&size=40617&status=done&style=none&width=669.5)<br />mvn打包成jar包<br />java -Xrunjdwp:transport=dt_socket,suspend=n,server=y,address=12346 -jar java-sec-code-1.0.0.jar

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1625498663940-21b4b15e-0657-443d-8034-8a132f97e661.png#align=left&display=inline&height=499&originHeight=665&originWidth=1226&size=153385&status=done&style=none&width=920)<br />代码运行的时候要选择刚刚远程调试的配置设置。然后就可以直接打断点调试了，但是要点击旁边那个虫。每次打完断点都要点击这个<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1625500423282-3885ded4-17b8-48e2-a4b8-a449c4635e57.png#align=left&display=inline&height=112&originHeight=149&originWidth=246&size=7171&status=done&style=none&width=185)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1625498928783-ae2ffb1c-8cea-4454-85d6-7c717978a1cb.png#align=left&display=inline&height=313&originHeight=626&originWidth=1574&size=122613&status=done&style=none&width=787)

