<a name="YwEun"></a>
# 1、查找病毒的目录
<a name="oxPjo"></a>
## A.查看CPU
```
# top
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568595953495-2a427169-eed3-4d6f-aa36-e39d2bf55260.png#align=left&display=inline&height=515&originHeight=1546&originWidth=2365&size=296944&status=done&style=none&width=788.3333333333334)
<a name="3WXxe"></a>
## B.查看高PID命令执行的路径

```
# ps -aux | grep nginx
```

![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568595893619-c94e1662-d1cb-4146-99af-35968a89bc34.png#align=left&display=inline&height=358&originHeight=1075&originWidth=3258&size=277828&status=done&style=none&width=1086)<br />**未发现异常命令执行的程序目录**
<a name="zfve6"></a>
## C.在Docker中找到异常镜像**
<a name="i6JoW"></a>
### 1.查看系统安装的Docker镜像
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568598208990-17d48908-6acc-41be-8eb7-3556b6efd44e.png#align=left&display=inline&height=281&originHeight=843&originWidth=3258&size=165404&status=done&style=none&width=1086)
<a name="ZCzPm"></a>
### 2.删除异常镜像以及对应运行的容器
![image.png](https://cdn.nlark.com/yuque/0/2019/png/396745/1568598293830-aa3ece79-5f68-4a82-88bb-a2989a3a54b4.png#align=left&display=inline&height=515&originHeight=1546&originWidth=3258&size=380461&status=done&style=none&width=1086)

